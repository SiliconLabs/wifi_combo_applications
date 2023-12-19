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
#include "em_gpio.h"
#include "em_ldma.h"
#include "em_usart.h"


/**
 * Global Variables
 */

uint8_t dummy[4096];


#ifdef DMA_ENABLED

#define RX_DMA_CHANNEL 0
#define TX_DMA_CHANNEL 1

// Descriptor and config for the LDMA operation for sending data
LDMA_Descriptor_t ldmaTXDescriptor;
LDMA_TransferCfg_t ldmaTXConfig;

// Descriptor and config for the LDMA operation for receiving data
LDMA_Descriptor_t ldmaRXDescriptor;
LDMA_TransferCfg_t ldmaRXConfig;


volatile uint32_t rx_transfer_completed, tx_transfer_completed;

void LDMA_IRQHandler()
{

  uint32_t flags = LDMA_IntGet();
  if(flags & (1 << RX_DMA_CHANNEL))
    {
      rx_transfer_completed = 1;
      LDMA_IntClear(1 << RX_DMA_CHANNEL);
    }
  else if(flags & (1 << TX_DMA_CHANNEL))
    {
      tx_transfer_completed = 1;
      LDMA_IntClear(1 << TX_DMA_CHANNEL);
    }

}

#endif


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

  if(tx_buff == NULL)
    {
      tx_buff = (uint8_t *)&dummy;
    }
  else if(rx_buff == NULL)
    {
      rx_buff = (uint8_t *)&dummy;
    }

  /* Transmitting data */

#ifdef DMA_ENABLED
  // Peripheral to memory transfer, Source: USART0->RXDATA, Destination: RxBuffer, Bytes to receive: 10
  ldmaRXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_P2M_BYTE(&(USART0->RXDATA), rx_buff, transfer_length);
  // One byte will transfer everytime the USART RXDATAV flag is high
  ldmaRXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART0_RXDATAV);

  // Memory to peripheral transfer, Source: TxBuffer, Destination: USART0->TXDATA, Bytes to receive: 10
  ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(tx_buff, &(USART0->TXDATA), transfer_length);
  // One byte will transfer everytime the USART TXBL flag is high
  ldmaTXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART0_TXBL);

  //Starting both ldma transfers are different channels
  LDMA_StartTransfer(RX_DMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
  LDMA_StartTransfer(TX_DMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);

  while(!(rx_transfer_completed && tx_transfer_completed));

  rsi_delay_ms(5);

#else
  uint16_t i = 0u;
  for (i = 0; i < transfer_length; i++)
    {
      rx_buff[i] = USART_SpiTransfer(USART0, tx_buff[i]);
    }
#endif

  return 0;
}
