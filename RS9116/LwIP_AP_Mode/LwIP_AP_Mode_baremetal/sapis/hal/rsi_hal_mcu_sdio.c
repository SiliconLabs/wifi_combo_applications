/**
 * @file       rsi_hal_mcu_sdio.c
 * @version    0.1
 * @date       11 March 2019
 *
 * Copyright(C) Redpine Signals 2019
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief: HAL SDIO API
 *
 * @Description:
 * This file Contains all the API's related to SDIO HAL 
 *
 */


/**
 * Includes
 */

#include "rsi_driver.h"

/*==============================================*/
/**
 * @fn          void rsi_sdio_write_multiple(uint8_t *tx_data, uint16_t Addr, uint16_t no_of_blocks)
 * @brief       This API is used to write the packet on to the SDIO interface in block mode.
 * @param[in]   tx_data is the buffer to be written to sdio.
 * @param[in]   Addr of the mem to which the data has to be written.
 * @param[in]   no_of_blocks is the blocks present to be transfered.
 * @return      0 in case of success ,*              - 1 in case of failure
 * @section description 
 *  This API is used to write the packet on to the SDIO interface
 * 
 *
 */
int16_t rsi_sdio_write_multiple(uint8_t *tx_data, uint16_t Addr, uint16_t no_of_blocks)
{
}

/*==============================================*/
/**
 * @fn          uint8_t rsi_sdio_read_multiple(uint8_t *read_buff, uint32_t Addr)
 * @brief       API is used to read no of bytes in blocked mode from device.
 * @param[in]   read_buff is the buffer to be stored with the data read from device.
 * @param[in]   Addr of the mem to be read.
 * @return      0 in case of success ,*              - 1 in case of failure
 * @section description This function gets the packet coming from the module and 
 * copies to the buffer pointed 
 * 
 *
 */

uint8_t rsi_sdio_read_multiple(uint8_t *read_buff, uint32_t Addr)
{
}

/*==============================================*/
/**
 * @fn          uint8_t sdio_reg_writeb(uint32_t Addr, uint8_t *dBuf)
 * @brief       API is used to write 1 byte of data to sdio slave register space.
 * @param[in]   Addr of the reg to be written.
 * @param[in]   Buffer of data to be written to sdio slave reg.
 * @return      0 in case of success ,*              - 1 in case of failure
 * @section description This function writes 1 byte of data to the slave device 
 * 
 *
 */
uint8_t sdio_reg_writeb(uint32_t Addr, uint8_t *dBuf)
{
}

/*==============================================*/
/**
 * @fn          uint8_t sdio_reg_readb(uint32_t Addr, uint8_t *dBuf)
 * @brief       API is used to read 1 byte of data from sdio slave register space.
 * @param[in]   Addr of the reg to be read.
 * @param[in]   Buffer of data to be read from sdio slave reg.
 * @return      0 in case of success ,*              - 1 in case of failure
 * @section description This function gets the 1 byte of data from the slave device 
 * 
 *
 */
uint8_t sdio_reg_readb(uint32_t Addr, uint8_t *dBuf)
{
}

/*==============================================*/
/**
 * @fn          int16_t rsi_sdio_readb(uint32_t addr, uint16_t len, uint8_t *dBuf)
 * @brief       API is used to read n bytes of data from device space in byte mode.
 * @param[in]   Addr of the data to be read.
 * @param[in]   Buffer of data to be read from sdio device.
 * @return      0 in case of success ,*              - 1 in case of failure
 * @section description This function gets the n  bytes of data from the device
 * 
 *
 */
int16_t rsi_sdio_readb(uint32_t addr, uint16_t len, uint8_t *dBuf)
{
}

/*==============================================*/
/**
 * @fn          int16_t rsi_sdio_writeb(uint32_t addr, uint16_t len, uint8_t *dBuf)
 * @brief       API is used to write n bytes of data to device space in byte mode.
 * @param[in]   Addr of the data to be written.
 * @param[in]   Buffer of data to be written to sdio device.
 * @return      0 in case of success ,*              - 1 in case of failure
 * @section description This function writes the n bytes of data to the device
 * 
 *
 */
int16_t rsi_sdio_writeb(uint32_t addr, uint16_t len, uint8_t *dBuf)
{
}
/*=============================================*/
/**
 * @fn                  int16_t rsi_sdio_init(void)
 * @brief               Start the SDIO interface
 * @param[in]           none
 * @param[out]          none
 * @return              errCode
 * @section description         
 * This API initializes the Wi-Fi modules Slave SDIO interface.
 */
int16_t rsi_mcu_sdio_init(void)
{
}
