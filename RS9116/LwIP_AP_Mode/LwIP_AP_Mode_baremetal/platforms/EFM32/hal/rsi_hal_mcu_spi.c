/*************************************************************************
 *
 * Copyright (c) 2019 Redpine Signals Incorporated. All Rights Reserved.
 *
 * NOTICE:  All  information  contained  herein is, and  remains  the  property of
 * Redpine Signals Incorporated. The intellectual and technical concepts contained
 * herein  are  proprietary to  Redpine Signals Incorporated and may be covered by
 * U.S. and Foreign Patents, patents in process, and are protected by trade secret
 * or copyright law. Dissemination  of this  information or reproduction  of  this
 * material is strictly forbidden unless prior written permission is obtained from
 * Redpine Signals Incorporated.
 */

/**
 * Includes
 */
#include "rsi_driver.h"





#include <string.h>
#include <stdio.h>
#include "spidrv.h"
#include "sl_spidrv_instances.h"
#include "sl_sleeptimer.h"
#include "em_gpio.h"
#include "dmadrv.h"
#include "em_assert.h"
#include "ecode.h"

//SPIDRV_HandleData_t sl_spidrv_exp_handle_data;
//SPIDRV_Handle_t sl_spidrv_exp_handle = &sl_spidrv_exp_handle_data;



#define SPI_HANDLE                  sl_spidrv_exp_handle



/**
 * Global Variables
 */
uint8_t i;
uint8_t dummy[1600];
uint32_t error;
static volatile bool transfer_complete = false;


// Callback fired when data is transmitted
void transfer_callback(SPIDRV_HandleData_t *handle,
                       Ecode_t transfer_status,
                       int items_transferred)
{
  (void)&handle;
  (void)items_transferred;

  // Post semaphore to signal to application
  // task that transfer is successful
  if (transfer_status == ECODE_EMDRV_SPIDRV_OK) {
    transfer_complete = true;
    //GPIO_IntClear(0xFFFF);
  }
}


/*==================================================================*/
/**
 * @fn         int16_t cs_enable()
 * @param[out] None
 * @return     0, 0=success
 * @section description
 * This API is used to enable the spi chip select pin in SPI interface.
 */
void cs_enable(void)
{
  //! enables the spi chip select pin on SPI interface
  //! CS pin controlled by hardware.;

}

/*==================================================================*/
/**
 * @fn         int16_t cs_disable()
 * @param[out] None
 * @return     0, 0=success
 * @section description
 * This API is used to disable the spi chip select pin in SPI interface.
 */
void cs_disable(void)
{

  //! disables the spi chip select pin on SPI interface
  //! CS pin controlled by hardware.

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
 * This API is used to transfer/receive data to the Wi-Fi module through the SPI interface.
 */

int16_t rsi_spi_transfer(uint8_t *tx_buff, uint8_t *rx_buff, uint16_t transfer_length,uint8_t mode)
{
  Ecode_t ecode;
  uint16_t length;
  length=transfer_length;
  if(tx_buff == NULL)
    {
      tx_buff = (uint8_t *)&dummy;
    }
  else if(rx_buff == NULL)
    {
      rx_buff = (uint8_t *)&dummy;
    }

  /* Transmitting data */
    transfer_complete = false;


  error=SPIDRV_MTransfer(SPI_HANDLE, tx_buff, rx_buff, length, transfer_callback);

 while (!transfer_complete) ;
  return 0;
}
