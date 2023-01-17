/***************************************************************************//**
 * @file sdio.c
 * @brief SDIO handler library - initial
 * @version 0.0.1
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories, Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.@n
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.@n
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#include "sdio.h"
#include "em_device.h"
#include "em_cmu.h"

/******************************************************************************
 * Local types
 *****************************************************************************/

/******************************************************************************
 * Local prototypes
 *****************************************************************************/
static uint32_t SDIO_S_SetCommandResponseType(uint32_t cmd_u32);
static uint32_t SDIO_S_SetDataPresentSelect(uint32_t cmd_u32);
static uint32_t SDIO_S_SetCommandSpecificCrcAndIndexCheck(uint32_t cmd_u32);
static uint32_t SDIO_S_SendCMDWithOutDAT( SDIO_TypeDef *sdio_t,
                                        uint32_t cmd_u32,
                                        uint32_t cmdArg_u32);
static void SDIO_S_LowLevelRegisterInit(SDIO_TypeDef *sdio_t,
                                        uint32_t sdioFreq_u32,
                                        CMU_Clock_TypeDef mainClock_t);
static void SDIO_S_CardInitialization_and_Identification(SDIO_TypeDef *sdio_t);
static void SDIO_S_CardStdbyToTrsfrMode(SDIO_TypeDef *sdio_t);
static void SDIO_S_TimeoutSettingonDATLine(SDIO_TypeDef *sdio_t);


/******************************************************************************
 * Local variables
 *****************************************************************************/
static struct
{
  uint32_t cardType_u3  :3;   // Card Type
  uint32_t cardRCA_u16  :16;      // Card Relative Address
  uint32_t F8_u1        :1;       // CMD8 valid flag
  uint32_t SDIO_u1      :1;         // SDIO type flag
  uint32_t CCS_u1       :1;       // CARD Capacity Status bit's flag
  uint32_t S18A_u1      :1;     // 1.8V Support flag
} SDIO_ActCardState_st;

/**************************************************************************//**
 * @brief General initialization function for the SDIO host and for
 *      SD cards
 *
 * @Inputs:
 *     SDIO_TypeDef *sdio_t: address of the MCU's SDIO register related
 *                 memory mapped register
 *     uint32_t sdioFreq_u32: desired SDIO clock frequency (between host and
 *                            sd card)
 *     CMU_Clock_TypeDef mainClock_t: used main clock source
 *****************************************************************************/
void SDIO_Init(SDIO_TypeDef *sdio_t,
               uint32_t sdioFreq_u32,
               CMU_Clock_TypeDef mainClock_t)
{
  CMU_ClockEnable(cmuClock_SDIO, true);
  SDIO_S_LowLevelRegisterInit(sdio_t, sdioFreq_u32, mainClock_t);
  SDIO_S_CardInitialization_and_Identification(sdio_t);
  SDIO_S_CardStdbyToTrsfrMode(sdio_t);

}

/**************************************************************************//**
 * @brief This function copies 512byte data from SD card to MCU
 *
 * @Inputs:
 *     SDIO_TypeDef *sdio_t: address of the MCU's SDIO register related
 *                 memory mapped register
 *
 *     SD_origin_u32: address of the SD card block, from where the data will
 *            be copied
 *
 *     *destination_pu32: address of the data space, where the data will be
 *              transfered from SD card. It shall be 512byte
 *****************************************************************************/
void SDIO_ReadSingleBlock(SDIO_TypeDef *sdio_t,
                          uint32_t SD_origin_u32,
                          uint32_t* destination_pu32)
{
  SDIO_S_TimeoutSettingonDATLine(sdio_t);

  uint32_t* localbuffptr_pu32;
  uint32_t tmpReg_u32 = 0;

  // clear previous command complete & Buffer Read Ready Int & Trans Complete int
  sdio_t->IFCR = (_SDIO_IFCR_CMDCOM_MASK);
  sdio_t->IFCR = (_SDIO_IFCR_BFRRDRDY_MASK);
  sdio_t->IFCR = (_SDIO_IFCR_TRANCOM_MASK);

  // 1. Set Block Size Reg
  sdio_t->BLKSIZE |= 0x200 << _SDIO_BLKSIZE_TFRBLKSIZE_SHIFT; // 512byte

  // 2. Set Block Count Reg

  // 3. Set Argument 1 Reg
  sdio_t->CMDARG1 = SD_origin_u32;

  // 4. Set Transfer mode Reg
  tmpReg_u32 |= SDIO_TFRMODE_MULTSINGBLKSEL_SINGLEBLK
             | SDIO_TFRMODE_DATDIRSEL_ENABLE // read 1 (card to host)
             | SDIO_TFRMODE_AUTOCMDEN_ACMDDISABLED
             | SDIO_TFRMODE_BLKCNTEN_DISABLE
             | SDIO_TFRMODE_DMAEN_DISABLE

             // 5. Set Command Reg
             | SDIO_S_SetCommandResponseType(CMD17)
             | SDIO_TFRMODE_CMDCRCCHKEN_DISABLE
             | SDIO_TFRMODE_CMDINDXCHKEN_DISABLE
             | SDIO_TFRMODE_DATPRESSEL_DATA
             | SDIO_TFRMODE_CMDTYPE_NORMAL
             | (CMD17 << _SDIO_TFRMODE_CMDINDEX_SHIFT);

  sdio_t->TFRMODE = tmpReg_u32;

  // 6. wait for Command Complete
  while (!(sdio_t->IFCR & _SDIO_IFCR_CMDCOM_MASK));

  // 7. clear previous command complete int
  sdio_t->IFCR = (_SDIO_IFCR_CMDCOM_MASK);

  // 10. wait for Buffer Read Ready int
  while (!(sdio_t->IFCR & _SDIO_IFCR_BFRRDRDY_MASK));

  // 11. clear previous Buffer Read Ready Int
  sdio_t->IFCR = (_SDIO_IFCR_BFRRDRDY_MASK);

  // 12. Set Block Data
  localbuffptr_pu32 = (uint32_t*)(&sdio_t->BUFDATPORT);
  for (int i = 0; i < 128; i++)
  {
    uint32_t tmpData_u32 = *localbuffptr_pu32;
    *destination_pu32 = tmpData_u32;
    destination_pu32++;
  }

  // add delay for workaround by SiLabs support team(nino)
  //for(uint32_t del = 0; del < 10000; del++);
  // 19. Wait for transfer completed int
  while (!(sdio_t->IFCR & _SDIO_IFCR_TRANCOM_MASK));

  // 20. CLear Transfer completed status
  while (sdio_t->IFCR & (_SDIO_IFCR_TRANCOM_MASK))
  {
    sdio_t->IFCR = (_SDIO_IFCR_TRANCOM_MASK);
  }
}

/**************************************************************************//**
 * @brief This function copies 512byte data from MCU to the memory card
 *
 * @Inputs:
 *     SDIO_TypeDef *sdio_t: address of the MCU's SDIO register related
 *                 memory mapped register
 *
 *     SD_dest_u32: address of the SD card block, where the data will be
 *            copied
 *
 *     *origin_pu32: address of the data, from the data will be transfered
 *            to SD card. It shall be 512byte
 *****************************************************************************/
void SDIO_WriteSingleBlock( SDIO_TypeDef *sdio_t,
                            uint32_t SD_dest_u32,
                            uint32_t* origin_pu32)
{

  SDIO_S_TimeoutSettingonDATLine(sdio_t);

  uint32_t* localbuffptr_pu32;
  uint32_t tmpReg_u32 = 0;

  // clear previous command complete & Buffer Write Ready Int & Trans Complete int
  sdio_t->IFCR = (_SDIO_IFCR_CMDCOM_MASK);
  sdio_t->IFCR = (_SDIO_IFCR_BFRWRRDY_MASK);
  sdio_t->IFCR = (_SDIO_IFCR_TRANCOM_MASK);

  // 1. Set Block Size Reg
  sdio_t->BLKSIZE |= 0x200 << _SDIO_BLKSIZE_TFRBLKSIZE_SHIFT; // 512byte

  // 2. Set Block Count Reg

  // 3. Set Argument 1 Reg
  sdio_t->CMDARG1 = SD_dest_u32;

  // 4. Set Transfer mode Reg
  tmpReg_u32 |= SDIO_TFRMODE_MULTSINGBLKSEL_SINGLEBLK
      | SDIO_TFRMODE_DATDIRSEL_DISABLE // write 0 (host to card)
      | SDIO_TFRMODE_AUTOCMDEN_ACMDDISABLED 
    | SDIO_TFRMODE_BLKCNTEN_DISABLE
      | SDIO_TFRMODE_DMAEN_DISABLE

      // 5. Set Command Reg
      | SDIO_S_SetCommandResponseType(CMD24) 
    | SDIO_TFRMODE_CMDCRCCHKEN_DISABLE
      | SDIO_TFRMODE_CMDINDXCHKEN_DISABLE 
    | SDIO_TFRMODE_DATPRESSEL_DATA
    | SDIO_TFRMODE_CMDTYPE_NORMAL
    | (CMD24 << _SDIO_TFRMODE_CMDINDEX_SHIFT);

  sdio_t->TFRMODE = tmpReg_u32;

  // 6. wait for Command Complete
  while (!(sdio_t->IFCR & _SDIO_IFCR_CMDCOM_MASK));

  // 7. clear previous command complete int
  sdio_t->IFCR = (_SDIO_IFCR_CMDCOM_MASK);

  // 10. wait for Buffer Write Ready int
  while (!(sdio_t->IFCR & _SDIO_IFCR_BFRWRRDY_MASK));

  // 11. clear previous Buffer Write Ready Int
  sdio_t->IFCR = (_SDIO_IFCR_BFRWRRDY_MASK);

  // 12. Set Block Data
  localbuffptr_pu32 = (uint32_t*)(&(sdio_t->BUFDATPORT));
  for (int i = 0; i < 128; i++)
  {
    uint32_t tmpData_u32 = *origin_pu32;
    *localbuffptr_pu32 = tmpData_u32;
    origin_pu32++;
  }

  // 19. Wait for transfer completed int
  while (!(sdio_t->IFCR & _SDIO_IFCR_TRANCOM_MASK));

  // 20. CLear Transfer completed status
  while (sdio_t->IFCR & (_SDIO_IFCR_TRANCOM_MASK))
  {
    sdio_t->IFCR = (_SDIO_IFCR_TRANCOM_MASK);
  }

}

/**************************************************************************//**
 * @brief Low level procedure for the commands, which do not use data transfer
 *      between host and SD card under execution
 *
 * @Inputs:
 *     SDIO_TypeDef *sdio_t: address of the MCU's SDIO register related
 *                 memory mapped register
 *
 *     cmd_u32: the used command
 *
 *     cmdArg_u32: the command related argument
 *****************************************************************************/
uint32_t SDIO_S_SendCMDWithOutDAT(SDIO_TypeDef *sdio_t,
                                uint32_t cmd_u32,
                                uint32_t cmdArg_u32)
{
  uint32_t tmpReg_u32;
  uint32_t regVal_u32 = sdio_t->PRSSTAT;
  //1. Check Command Inhibit Used
  while (regVal_u32 & _SDIO_PRSSTAT_CMDINHIBITCMD_MASK)
  {
    regVal_u32 = sdio_t->PRSSTAT;
  }

  // 2. Check command with busy
  if (cmd_u32 == CMD7 ||
      cmd_u32 == CMD12)
  {
    // with busy
    // Check Abort Commands
    if (cmd_u32 == CMD12)
    {
      // abort command?
    }
    else
    {
      while (sdio_t->PRSSTAT & _SDIO_PRSSTAT_CMDINHIBITDAT_MASK);
    }
  }

  /* 5. Set Argument 1 Reg */
  sdio_t->CMDARG1 = cmdArg_u32;

  /* 6. Set Command Reg */
  tmpReg_u32 = sdio_t->TFRMODE;
  tmpReg_u32 &= (~_SDIO_TFRMODE_RESPTYPESEL_MASK);
  tmpReg_u32 |= SDIO_S_SetCommandResponseType(cmd_u32);
  tmpReg_u32 &= (~_SDIO_TFRMODE_CMDCRCCHKEN_MASK);
  tmpReg_u32 &= (~_SDIO_TFRMODE_CMDINDXCHKEN_MASK);
  tmpReg_u32 &= (~_SDIO_TFRMODE_DATPRESSEL_MASK);
  tmpReg_u32 |= SDIO_S_SetCommandSpecificCrcAndIndexCheck(cmd_u32);
  tmpReg_u32 |= SDIO_S_SetDataPresentSelect(cmd_u32);
  tmpReg_u32 &= (~_SDIO_TFRMODE_CMDTYPE_MASK);

  tmpReg_u32 &= (~_SDIO_TFRMODE_CMDINDEX_MASK);
  tmpReg_u32 |= (cmd_u32 << _SDIO_TFRMODE_CMDINDEX_SHIFT);
  sdio_t->TFRMODE = tmpReg_u32;

  // add delay for workaround by SiLabs support team(nino)
  for(uint32_t del = 0; del < 10000; del++);

  // Sequence to Finalize Command
  // 1. Wait for Command Complete
  while (!(sdio_t->IFCR & _SDIO_IFCR_CMDCOM_MASK));
  // 2. clear previous command complete int
  while ((sdio_t->IFCR & _SDIO_IFCR_CMDCOM_MASK))
  {
    sdio_t->IFCR = (_SDIO_IFCR_CMDCOM_MASK);
  }

  return 1;
}

/**************************************************************************//**
 * @brief Set the data transition related timeout (max time)
 *
 * @Inputs:
 *     SDIO_TypeDef *sdio_t: address of the MCU's SDIO register related
 *                 memory mapped register
 *
 *****************************************************************************/
static void SDIO_S_TimeoutSettingonDATLine(SDIO_TypeDef *sdio_t)
{

  // 1. Calculate divisor to determine timeout errors

  // 2. Set Data Timeout Counter Value
  sdio_t->CLOCKCTRL |= (0xFE << _SDIO_CLOCKCTRL_DATTOUTCNTVAL_SHIFT);
}

/**************************************************************************//**
 * @brief This function is responsible to set CRC and Index check if it is
 *      mandatory for the command
 *
 * @Inputs:
 *     cmd_u32: the used command
 *
 * @Outputs: function's return value: TRFMODE register related flags for the
 *        functionality (CRC and Index)
 *****************************************************************************/
static uint32_t SDIO_S_SetCommandSpecificCrcAndIndexCheck(uint32_t cmd_u32)
{
  uint32_t retVal_u32 = 0;

  if (cmd_u32 == CMD8)
  {
    retVal_u32 = (SDIO_TFRMODE_CMDCRCCHKEN_ENABLE
                 | SDIO_TFRMODE_CMDINDXCHKEN_ENABLE);
  }

  return retVal_u32;
}

/**************************************************************************//**
 * @brief Helper function for SDIO_S_SendCMDWithOutDAT(). The function's return value
 *      will be the command related data presence (or shortage)
 *
 * @Inputs: cmd_u32: SD memory card command
 *
 * @outputs: function's return value: command related register value
 *****************************************************************************/
static uint32_t SDIO_S_SetDataPresentSelect(uint32_t cmd_u32)
{
  if (  cmd_u32 == CMD7 ||
      cmd_u32 == CMD12)
  {
    return SDIO_TFRMODE_DATPRESSEL_DATA;
  }
  else
  {
    return SDIO_TFRMODE_DATPRESSEL_NODATA;
  }
}

/**************************************************************************//**
 * @brief Helper function for Init_and_Identification function. Based on the
 *      command name, the return value will be the command related response.
 *
 * @Inputs:
 *     uint32_t cmd_u32: SD memory card command
 *
 * @Outputs:
 *     Function's return value: the desired command related response type
 *****************************************************************************/
static uint32_t SDIO_S_SetCommandResponseType(uint32_t cmd_u32)
{
  /*Check the command in the valid range*/
  if (cmd_u32 >= 64)
  {
    return SDIO_TFRMODE_RESPTYPESEL_NORESP;
  }

  /*Set cmd related response type*/
  /*no response*/
  if (cmd_u32 == CMD0)
  {
    return SDIO_TFRMODE_RESPTYPESEL_NORESP;
  }
  /*R1 response*/
  else if ( cmd_u32 == CMD55
        || cmd_u32 == CMD58
      || cmd_u32 == CMD17
          || cmd_u32 == CMD24
      || cmd_u32 == ACMD6
      || cmd_u32 == CMD13)
  {
    return SDIO_TFRMODE_RESPTYPESEL_RESP48;
  }
  /*R1b response*/
  else if (cmd_u32 == CMD7)
  {
    return SDIO_TFRMODE_RESPTYPESEL_BUSYAFTRESP;
  }
  /*R2 response*/
  else if (cmd_u32 == CMD2)
  {
    return SDIO_TFRMODE_RESPTYPESEL_RESP136;
  }
  /*R3 response*/
  else if (cmd_u32 == ACMD41)
  {
    return SDIO_TFRMODE_RESPTYPESEL_RESP48;
  }
  /*R6 response - see: 4.9.5 */
  else if ( cmd_u32 == CMD3 ||
        cmd_u32 == CMD5)
  {
    return SDIO_TFRMODE_RESPTYPESEL_RESP48;
  }
  /*R7 response*/
  else if (cmd_u32 == CMD8)
  {
    return SDIO_TFRMODE_RESPTYPESEL_RESP48;
  }

  return SDIO_TFRMODE_RESPTYPESEL_NORESP;
}

/**************************************************************************//**
 * @brief Low level SDIO host initialization
 *      For more information see:
 *      SD_Host_Controller_Simplified_Specification_Ver3.00 Chapter2.x
 *
 * @Inputs:
 *     SDIO_TypeDef *sdio_t: address of the MCU's SDIO register related
 *                 memory mapped register
 *     uint32_t sdioFreq_u32: desired SDIO clock frequency (between host and
 *                              sd card)
 *       CMU_Clock_TypeDef mainClock_t: used main clock source
 *****************************************************************************/
static void SDIO_S_LowLevelRegisterInit(SDIO_TypeDef *sdio_t,
                                        uint32_t sdioFreq_u32,
                                        CMU_Clock_TypeDef mainClock_t)
{
  /*
   * Board specific register adjustment
   * Route soldered microSD card slot
   */
  sdio_t->ROUTELOC0 =   SDIO_ROUTELOC0_DATLOC_LOC1
            | SDIO_ROUTELOC0_CDLOC_LOC3
            //| SDIO_ROUTELOC0_WPLOC_LOC3
            | SDIO_ROUTELOC0_CLKLOC_LOC1;
  sdio_t->ROUTELOC1 =   SDIO_ROUTELOC1_CMDLOC_LOC1;

  sdio_t->ROUTEPEN =  SDIO_ROUTEPEN_CLKPEN
            | SDIO_ROUTEPEN_CMDPEN
            | SDIO_ROUTEPEN_D0PEN
            | SDIO_ROUTEPEN_D1PEN
            | SDIO_ROUTEPEN_D2PEN
            | SDIO_ROUTEPEN_D3PEN;

  /*
   * General SDIO register adjustment
   */
  sdio_t->CTRL =    (0 << _SDIO_CTRL_ITAPDLYEN_SHIFT)
            | (0 << _SDIO_CTRL_ITAPDLYSEL_SHIFT)
            | (0 << _SDIO_CTRL_ITAPCHGWIN_SHIFT)
            | (1 << _SDIO_CTRL_OTAPDLYEN_SHIFT)
            | (8 << _SDIO_CTRL_OTAPDLYSEL_SHIFT);

  sdio_t->CFG0 =        (0x20 << _SDIO_CFG0_TUNINGCNT_SHIFT)
                        | (0x30 << _SDIO_CFG0_TOUTCLKFREQ_SHIFT)
                        | (1 << _SDIO_CFG0_TOUTCLKUNIT_SHIFT)
                        | (0xD0 << _SDIO_CFG0_BASECLKFREQ_SHIFT)
                        | (SDIO_CFG0_MAXBLKLEN_1024B)
                        | (1 << _SDIO_CFG0_C8BITSUP_SHIFT)
                        | (1 << _SDIO_CFG0_CADMA2SUP_SHIFT)
                        | (1 << _SDIO_CFG0_CHSSUP_SHIFT)
                        | (1 << _SDIO_CFG0_CSDMASUP_SHIFT)
                        | (1 << _SDIO_CFG0_CSUSPRESSUP_SHIFT)
                        | (1 << _SDIO_CFG0_C3P3VSUP_SHIFT)
                        | (1 << _SDIO_CFG0_C3P0VSUP_SHIFT)
                        | (1 << _SDIO_CFG0_C1P8VSUP_SHIFT);

  sdio_t->CFG1 =        (0 << _SDIO_CFG1_ASYNCINTRSUP_SHIFT)
                        | (SDIO_CFG1_SLOTTYPE_EMSDSLOT)
                        | (1 << _SDIO_CFG1_CSDR50SUP_SHIFT)
                        | (1 << _SDIO_CFG1_CSDR104SUP_SHIFT)
                        | (1 << _SDIO_CFG1_CDDR50SUP_SHIFT)
                        | (1 << _SDIO_CFG1_CDRVASUP_SHIFT)
                        | (1 << _SDIO_CFG1_CDRVCSUP_SHIFT)
                        | (1 << _SDIO_CFG1_CDRVDSUP_SHIFT)
                        | (1 << _SDIO_CFG1_RETUNTMRCTL_SHIFT)
                        | (1 << _SDIO_CFG1_TUNSDR50_SHIFT)
                        | (0 << _SDIO_CFG1_RETUNMODES_SHIFT)
                        | (1 << _SDIO_CFG1_SPISUP_SHIFT)
                        | (1 << _SDIO_CFG1_ASYNCWKUPEN_SHIFT);

  sdio_t->CFGPRESETVAL0 = (0 << _SDIO_CFGPRESETVAL0_INITSDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL0_INITCLKGENEN_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL0_INITDRVST_SHIFT)
                        | (0x4 << _SDIO_CFGPRESETVAL0_DSPSDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL0_DSPCLKGENEN_SHIFT)
                        | (0x3 << _SDIO_CFGPRESETVAL0_DSPDRVST_SHIFT);

  sdio_t->CFGPRESETVAL1 = (2 << _SDIO_CFGPRESETVAL1_HSPSDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL1_HSPCLKGENEN_SHIFT)
                        | (2 << _SDIO_CFGPRESETVAL1_HSPDRVST_SHIFT)
                        | (4 << _SDIO_CFGPRESETVAL1_SDR12SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL1_SDR12CLKGENEN_SHIFT)
                        | (1 << _SDIO_CFGPRESETVAL1_SDR12DRVST_SHIFT);

  sdio_t->CFGPRESETVAL2 = (2 << _SDIO_CFGPRESETVAL2_SDR25SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL2_SDR25CLKGENEN_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL2_SDR25DRVST_SHIFT)
                        | (1 << _SDIO_CFGPRESETVAL2_SDR50SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL2_SDR50CLKGENEN_SHIFT)
                        | (1 << _SDIO_CFGPRESETVAL2_SDR50DRVST_SHIFT);

  sdio_t->CFGPRESETVAL3 = (0 << _SDIO_CFGPRESETVAL3_SDR104SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL3_SDR104CLKGENEN_SHIFT)
                        | (2 << _SDIO_CFGPRESETVAL3_SDR104DRVST_SHIFT)
                        | (2 << _SDIO_CFGPRESETVAL3_DDR50SDCLKFREQ_SHIFT)
                        | (0 << _SDIO_CFGPRESETVAL3_DDR50CLKGENEN_SHIFT)
                        | (3 << _SDIO_CFGPRESETVAL3_DDR50DRVST_SHIFT);

  sdio_t->IFENC =       SDIO_IFENC_CMDCOMEN
                        | SDIO_IFENC_TRANCOMEN
                        | SDIO_IFENC_BUFWRRDYEN
                        | SDIO_IFENC_BUFRDRDYEN
                        | SDIO_IFENC_CMDCRCERREN
                        | SDIO_IFENC_CMDINDEXERREN
                        | SDIO_IFENC_DATTOUTERREN
                        | SDIO_IFENC_DATCRCERREN;

  {
    // Calculate the divisor for SD clock frequency
    uint32_t divisor_u32 = CMU_ClockFreqGet(mainClock_t) / sdioFreq_u32;
    sdio_t->CLOCKCTRL = ((divisor_u32 << _SDIO_CLOCKCTRL_SDCLKFREQSEL_SHIFT))
                        | (SDIO_CLOCKCTRL_INTCLKEN)
                        | (SDIO_CLOCKCTRL_SDCLKEN);
  }

  sdio_t->HOSTCTRL1 =   (SDIO_HOSTCTRL1_SDBUSVOLTSEL_3P0V)
                        | (SDIO_HOSTCTRL1_SDBUSPOWER)
                        | (SDIO_HOSTCTRL1_DATTRANWD_SD4);
}

/**************************************************************************//**
 * @brief Initialization and identification sequence for the SDSC, SDHC and
 *      SDXC cards.
 *      This function covers the entry point of the card initialization
 *      (reset), then card identification mode, and at the end the card will
 *      enter to the data transfer mode.
 *      For the whole description see: SD Host Controller Simplified
 *      Specification Version 3.00 Chapter 3.6
 *
 * @Inputs:
 *     SDIO_TypeDef *sdio_t: address of the MCU's SDIO register related
 *                 memory mapped register
 *
 * @Outputs:
 *     SDIO_ActCardState_st: the initialized SD card related informations
 *****************************************************************************/
static void SDIO_S_CardInitialization_and_Identification(SDIO_TypeDef *sdio_t)
{

  uint32_t tempVar_u32 = 0x0;
  uint8_t attemptCnt_u8 = 2;

  // 1. Reset Card
  SDIO_S_SendCMDWithOutDAT(sdio_t, CMD0, 0);

  while (attemptCnt_u8 != 0)
  {
    // 2. Voltage Check
    tempVar_u32 = sdio_t->RESP0;

    SDIO_S_SendCMDWithOutDAT(sdio_t, CMD8, 1 << _CMD8_27V_36V_SHIFT);
    // 3. Check response
    if (sdio_t->RESP0 == tempVar_u32)
    {
      // no response
      SDIO_ActCardState_st.F8_u1 = 0;
      attemptCnt_u8 = 0;
    } else
    {
      // valid response
      if ((!(sdio_t->IFCR & SDIO_IFCR_CMDCRCERR)) &&
          (!(sdio_t->IFCR & SDIO_IFCR_CMDINDEXERR)) &&
          ((sdio_t->RESP0 & 1 << _CMD8_27V_36V_SHIFT) == 1 << _CMD8_27V_36V_SHIFT))
      {
        // Good response
        SDIO_ActCardState_st.F8_u1 = 1;
        attemptCnt_u8 = 0;
      } else
      {
        attemptCnt_u8--;
        if (attemptCnt_u8 == 1)
        {
          // 4. Unusable card
          return;
        }
      }
    }
  }

  // 5. Get SDIO OCR (CMD5) Voltage window = 0
  tempVar_u32 = sdio_t->RESP0;

  SDIO_ActCardState_st.SDIO_u1 = 0;

  // 11. Check F8 flag
  if (SDIO_ActCardState_st.F8_u1 == 1)
  {
    // 19.Get OCR(ACMD41) Voltage window = 0
    SDIO_S_SendCMDWithOutDAT(sdio_t, CMD55, RCA_DEFAULT << _CMD55_RCA_SHIFT);
    tempVar_u32 = sdio_t->RESP0;
    SDIO_S_SendCMDWithOutDAT(sdio_t, ACMD41, 0 << _ACMD41_VW_SHIFT); // voltage window = 0

    // 20. Check OCR
    if (tempVar_u32 != sdio_t->RESP0)
    {
      // OCR OK
      // 23. Check busy
      while (!(sdio_t->RESP0 & (1 << _ACMD41_RESPONSE_BUSY_BIT_SHIFT)))
      {
        // 21. Initialization (ACMD41 HCS=1, S18R, XPC) Set Voltage Window
        SDIO_S_SendCMDWithOutDAT(sdio_t, CMD55,
        RCA_DEFAULT << _CMD55_RCA_SHIFT);
        SDIO_S_SendCMDWithOutDAT(sdio_t, ACMD41, 1 << _ACMD41_VW_31_32_SHIFT |
                                                 1 << _ACMD41_HCS_SHIFT |
                                                 1 << _ACMD41_XPC_SHIFT |
                                                 1 << _ACMD41_S18R_SHIFT);
        SDIO_ActCardState_st.CCS_u1 = ((sdio_t->RESP0 & 1 << 30) >> 30);
        SDIO_ActCardState_st.S18A_u1 = ((sdio_t->RESP0 & 1 << 24) >> 24);
      }

      // 24. Check CCS
      if (SDIO_ActCardState_st.CCS_u1 == 0)
      {
        // 25. Set SDSC Ver2.00 or Ver3.00
        SDIO_ActCardState_st.cardType_u3 = SDSC_Ver200_or_Ver300;
      } else
      {
        // 26. Set SDHC SDXC
        SDIO_ActCardState_st.cardType_u3 = SDHC_SDXC;
        // 27. Signal Voltage Switch Procedure
      }

    }
  } else
  {
    // F8=0
    // 12.Get OCR(ACMD41) Voltage window = 0
    SDIO_S_SendCMDWithOutDAT(sdio_t, CMD55, RCA_DEFAULT << _CMD55_RCA_SHIFT);
    tempVar_u32 = sdio_t->RESP0;
    SDIO_S_SendCMDWithOutDAT(sdio_t, ACMD41, 0 << _ACMD41_VW_SHIFT); // voltage window = 0

    // 13. Check OCR
    if (tempVar_u32 != sdio_t->RESP0)
    {
      // OCR OK
      // 16. Check busy
      while (!(sdio_t->RESP0 & (1 << _ACMD41_RESPONSE_BUSY_BIT_SHIFT)))
      {
        // 21. Initialization (ACMD41 HCS=1, S18R, XPC) Set Voltage Window
        SDIO_S_SendCMDWithOutDAT(sdio_t, CMD55,
        RCA_DEFAULT << _CMD55_RCA_SHIFT);
        SDIO_S_SendCMDWithOutDAT(sdio_t, ACMD41, 1 << _ACMD41_VW_31_32_SHIFT);
        SDIO_ActCardState_st.CCS_u1 = ((sdio_t->RESP0 & 1 << 30) >> 30);
        SDIO_ActCardState_st.S18A_u1 = ((sdio_t->RESP0 & 1 << 24) >> 24);
        SDIO_ActCardState_st.cardType_u3 = SDSC_Ver101_or_Ver110;
      }
    } else
    {
      // OCR NOK
      // 17. Not SD Card
      SDIO_ActCardState_st.cardType_u3 = Not_SD_Card;
      return;
    }
  }

  // 32. Get CID (Card ID)
  SDIO_S_SendCMDWithOutDAT(sdio_t, CMD2, 0);

  // 33. Get RCA (relative card address)
  SDIO_S_SendCMDWithOutDAT(sdio_t, CMD3, 0);
  // The upper 16 bit of the response is the RCA
  SDIO_ActCardState_st.cardRCA_u16 = ((sdio_t->RESP0  & 0xFFFF0000) >> 16);
}

/**************************************************************************//**
 * @brief Perform state change (from card identification mode to data transfer
 *      mode) in the SD card
 *
 * @Inputs:
 *     SDIO_TypeDef *sdio_t: address of the MCU's SDIO register related
 *                 memory mapped register
 *
 *     SDIO_ActCardState_st.cardRCA_u16: actual SD card related relative
 *                       address
 *
 *****************************************************************************/
static void SDIO_S_CardStdbyToTrsfrMode(SDIO_TypeDef *sdio_t)
{

  // 1. Send status task register - R1 response
  SDIO_S_SendCMDWithOutDAT(sdio_t, CMD13,
      SDIO_ActCardState_st.cardRCA_u16 << _CMD13_RCA_SHIFT);
  // 2. Select the actual card to active
  SDIO_S_SendCMDWithOutDAT(sdio_t, CMD7,
      SDIO_ActCardState_st.cardRCA_u16 << _CMD7_RCA_SHIFT);
  // 3. Change to 4bit mode
  SDIO_S_SendCMDWithOutDAT(sdio_t, CMD55,
      SDIO_ActCardState_st.cardRCA_u16 << _CMD55_RCA_SHIFT);
  SDIO_S_SendCMDWithOutDAT(sdio_t, ACMD6, ACMD6_4BIT_BUSWIDTH_SET);
}

/**************************************************************************//**
 * @brief Return with the type of the initialized card
 *
 * @Inputs:
 *       none
 *
 * @Outputs:
 *       return: the type of the card
 *****************************************************************************/
uint8_t SDIO_GetActCardStateType(void)
{
    return ((uint8_t)(SDIO_ActCardState_st.cardType_u3));
}
