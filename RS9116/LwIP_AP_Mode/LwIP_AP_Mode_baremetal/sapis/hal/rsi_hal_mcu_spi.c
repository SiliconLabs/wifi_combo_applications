/**
 * @file       rsi_hal_mcu_spi.c
 * @version    0.1
 * @date       18 sept 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief: HAL SPI API
 *
 * @Description:
 * This file Contains all the API's related to HAL 
 *
 */


/**
 * Includes
 */
#include "rsi_driver.h"



/**
 * Global Variables
 */
/*==================================================================*/
/**
 * @fn         int16_t cs_enable()
 * @param[out] None
 * @return     0, 0=success
 * @section description  
 * This API is used to enable the spi chip select pin in SPI interface.
 */
void cs_enable()
{
	//! enables the spi chip select pin on SPI interface
	
}

/*==================================================================*/
/**
 * @fn         int16_t cs_disable()
 * @param[out] None
 * @return     0, 0=success
 * @section description  
 * This API is used to disable the spi chip select pin in SPI interface.
 */
void cs_disable()
{
	
	//! disables the spi chip select pin on SPI interface

}
/*==================================================================*/
/**
 * @fn         int16_t rsi_spi_transfer(uint8_t *ptrBuf,uint16_t bufLen,uint8_t *valBuf,uint8_t mode)
 * @param[in]  uint8_t *tx_buff, pointer to the buffer with the data to be transfered
 * @param[in]  uint8_t *rx_buff, pointer to the buffer to store the data received
 * @param[in]  uint16_t transfer_length, Number of bytes to send and receive
 * @param[in]  uint8_t mode, To indicate mode 8 BIT/32 BIT mode transfers.
 * @param[out] None
 * @return     0, 0=success
 * @section description  
 * This API is used to tranfer/receive data to the Wi-Fi module through the SPI interface.
 */
int16_t rsi_spi_transfer(uint8_t *tx_buff, uint8_t *rx_buff, uint16_t transfer_length,uint8_t mode)
{

  uint16_t i;
    
  return 0;
}


