/**
 * @file    rsi_spi_reg_rd_wr.c
 * @version	0.1
 * @date 		15 Aug 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief SDIO REGRDWR: SDIO Register Read/Write functions
 *
 * @section Description
 * This file contains the SDIO based Register R/W functionss
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


/**===========================================================================
 * @fn          int16_t rsi_reg_rd(uint8_t regAddr, uint8_t *dBuf)
 * @brief       reads from a register in the Wi-Fi module
 * @param[in]   uint8_t regAddr, address of sdio register to be read
 * @param[in]   uint8_t *dBuf, pointer to the buffer of data to be read, assumed to be at least a byte long
 * @return       0  = SUCCESS
 * 
 * @section description 
 * Reads from a sdio register with an address specified.
 *
 */
int16_t rsi_reg_rd(uint8_t regAddr, uint8_t *dBuf)
{
  return sdio_reg_readb(regAddr, dBuf);
}

/**===========================================================================
 * @fn          int16_t rsi_reg_wr(uint8_t regAddr, uint8_t *dBuf)
 * @brief       writes to a register in the Wi-Fi module
 * @param[in]   uint8_t regAddr, address of sdio register to be written
 * @param[in]   uint8_t *dBuf, pointer to the buffer of data to write, assumed to be at least a byte long
 * @return       0  = SUCCESS
 * 
 * @section description 
 * Writes to a sdio register with an address specified.
 *
 */

int16_t rsi_reg_wr(uint8_t regAddr, uint8_t *dBuf)
{
  return sdio_reg_writeb(regAddr, dBuf);
}

#endif
