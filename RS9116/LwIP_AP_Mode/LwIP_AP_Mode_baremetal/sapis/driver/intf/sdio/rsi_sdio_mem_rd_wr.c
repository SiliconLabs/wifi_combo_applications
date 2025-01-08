/**
 * @file       rsi_sdio_mem_rd_wr.c 
 * @version    0.1
 * @date       15 Aug 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief SDIO MEMRDWR: SPI Memory Read/Write functions, r/w memory on the module via the SPI interface
 *
 * @section Description
 * This file contains the SDIO Memory Read/Write functions to the module
 *
 *
 */



/**
 * Includes
 */
#include "rsi_driver.h"
#ifdef RSI_SDIO_INTERFACE
/**
 * Global Variables
 */
/*===========================================================================*/
/** 
 * @fn          int16_t rsi_mem_wr(uint32_t addr, uint16_t len, uint8_t *dBuf)
 * @brief       Performs a memory write to the Wi-Fi module
 * @param[in]   uint32_t addr, address to write to
 * @param[in]   uint16_t, len, number of bytes to write
 * @param[in]   uint8_t *dBuf, pointer to the buffer of data to write
 * @param[out]  none
 * @return      errCode
 *              0  = SUCCESS, -1 = FAILURE
 *
 * ABH Master Write (Internal Legacy Name)
 */

int16_t rsi_mem_wr(uint32_t addr, uint16_t len, uint8_t *dBuf)
{
  uint8_t data;	
  int16_t retval = RSI_SUCCESS;

  data = ((addr & 0xff000000) >> 24);
  //!writing MSB to 0xFB
  retval = rsi_reg_wr(0xfb, &data);
  if (retval != RSI_SUCCESS)
    return retval;

  data = ((addr & 0x00ff0000) >> 16);
  //!writing MSB to 0xFA
  retval = rsi_reg_wr(0xfa, &data);
  if (retval != RSI_SUCCESS)
    return retval;

  return rsi_sdio_writeb((addr & 0xffff) | SD_MASTER_ACCESS , len, dBuf);
}


/*===========================================================================*/
/**
 * @fn          int16_t rsi_mem_rd(uint32_t addr, uint16_t len, uint8_t *dBuf)
 * @brief       Performs a memory read from the Wi-Fi module
 * @param[in]   uint32_t, address to read from
 * @param[in]   uint16_t, len, number of bytes to read
 * @param[in]   uint8_t *dBuf, pointer to the buffer to receive the data into
 * @param[out]  none
 * @return      errCode
 *              0  = SUCCESS
 *
 * ABH Master Read (Internal Legacy Name)
 */
int16_t rsi_mem_rd(uint32_t addr, uint16_t len, uint8_t *dBuf)
{
  int16_t retval = RSI_SUCCESS;
  uint8_t data;	

  data = ((addr & 0xff000000) >> 24);
  //!writing MSB to 0xFB
  retval = rsi_reg_wr(0xfb, &data);
  if (retval != RSI_SUCCESS)
    return retval;

  data = ((addr & 0x00ff0000) >> 16);
  //!writing MSB to 0xFA
  retval = rsi_reg_wr(0xfa, &data);
  if (retval != RSI_SUCCESS)
    return retval;

  return rsi_sdio_readb((addr & 0xffff) | SD_MASTER_ACCESS, len, dBuf);
}


/*==================================================*/
/**
 * @fn          int16_t rsi_device_interrupt_status(uint8_t *int_status)
 * @brief       Returns the value of the Interrupt register
 * @param[in]   status
 * @param[out]  none
 * @return      errorcode
 *               0 = Success
 *              -1 = failure 
 */
int16_t rsi_device_interrupt_status(uint8_t *int_status)
{
   int16_t                   retval;
  while(1) 
  {
    //! read the interrupt register
    retval = rsi_mem_rd(RSI_HOST_INTF_STATUS_REG,2, int_status);    

    if(retval ==0) 
    {
      break;
    }
  }

  return retval;
}
#endif
