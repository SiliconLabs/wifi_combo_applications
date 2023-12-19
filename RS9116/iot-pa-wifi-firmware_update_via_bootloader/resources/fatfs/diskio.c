/***************************************************************************//**
 * @file
 * @brief SDv2 (in SDIO mode) control module
 * @version 0.0.1
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/


#include "diskio.h"
#include "sdio.h"
#include "em_cmu.h"

static DSTATUS stat = STA_NOINIT;  /* Disk status */
static UINT CardType;

/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
  BYTE drv  /* Physical drive number (0) */
)
{

  if (drv) return STA_NOINIT;                   /* Supports only single drive */
  if (stat & STA_NODISK) return stat;           /* No card in the socket */

  if (stat&STA_NOINIT)
  {
    // Initialization of SDIO and Card
    SDIO_Init(SDIO,
              400000,             // 400kHz
              cmuClock_HFPER);
	uint8_t a_u8 = SDIO_GetActCardStateType();
	switch(a_u8){
	  case SDHC_SDXC:
	  {
	    CardType = CT_SD2;
	    break;
	  }
	}
  }
  stat &= ~STA_NOINIT;                        /* Clear STA_NOINIT */
  return stat;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
  BYTE drv                        /* Physical drive nmuber (0) */
)
{
  if (drv) return STA_NOINIT;     /* Supports only single drive */
  return stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
  BYTE drv,       /* Physical drive nmuber (0) */
  BYTE *buff,     /* Pointer to the data buffer to store read data */
  DWORD sector,   /* Start sector number (LBA) */
  BYTE count      /* Sector count (1..255) */
)
{
  if (drv || !count) return RES_PARERR;
  if (stat & STA_NOINIT) return RES_NOTRDY;

  if (!(CardType & CT_BLOCK)) sector = sector;  /* Convert to byte address if needed */

  while(count != 0)
  {
	  SDIO_ReadSingleBlock(SDIO,sector,(uint32_t*)buff);
	  sector++;
	  count--;
  }
  return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
  BYTE drv,           /* Physical drive nmuber (0) */
  const BYTE *buff,   /* Pointer to the data to be written */
  DWORD sector,       /* Start sector number (LBA) */
  BYTE count          /* Sector count (1..255) */
)
{
  if (drv || !count) return RES_PARERR;
  if (stat & STA_NOINIT) return RES_NOTRDY;
  if (stat & STA_PROTECT) return RES_WRPRT;

  while(count != 0)
  {
	  SDIO_WriteSingleBlock(SDIO,sector,(uint32_t*)buff);
	  sector++;
	  count--;
  }
  return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions
 * !!!!!!!!!!!!!!!!!!!!!!!!!
 * with STUBBED RETURN VALUES
 * !!!!!!!!!!!!!!!!!!!!!!!!!                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
  BYTE drv,     /* Physical drive nmuber (0) */
  BYTE ctrl,    /* Control code */
  void *buff    /* Buffer to send/receive data block */
)
{
  DRESULT res;

  if (drv) return RES_PARERR;
  if (stat & STA_NOINIT) return RES_NOTRDY;

  res = RES_ERROR;
  switch (ctrl) {
    case CTRL_SYNC :                /* Flush dirty buffer if present */
        res = RES_OK;
      break;

    case CTRL_INVALIDATE :          /* Used when unmounting */
      stat = STA_NOINIT;            /* Set disk status */
      res = RES_OK;
      break;

    case GET_SECTOR_COUNT :         /* Get number of sectors on the disk (WORD) */
      *(DWORD*)buff = 0x200;
      res = RES_OK;
      break;

    case GET_SECTOR_SIZE :          /* Get sectors on the disk (WORD) */
      *(WORD*)buff = 512;
      res = RES_OK;
      break;

    case GET_BLOCK_SIZE :           /* Get erase block size in unit of sectors (DWORD) */
      *(DWORD*)buff = 0xFF;
      res = RES_OK;
      break;

    default:
      res = RES_PARERR;
  }

  return res;
}
