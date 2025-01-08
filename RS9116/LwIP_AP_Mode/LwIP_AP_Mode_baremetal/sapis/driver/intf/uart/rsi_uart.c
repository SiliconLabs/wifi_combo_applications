/**
 * @file     rsi_uart.c
 * @version  0.1
 * @date     15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains UART HAL porting functionality
 *
 *  @section Description  This file contains UART HAL porting functionality
 *
 *
 */
 
#ifdef RSI_UART_INTERFACE
 #include "stm32f4xx_hal.h"
 #include "stm32f4xx_hal_uart.h"
 extern UART_HandleTypeDef huart1;
 
#include <stdlib.h>
#include <stdio.h>
#include "rsi_data_types.h"
#include "rsi_error.h"
#include "rsi_utils.h"
#include "rsi_pkt_mgmt.h"
#include "rsi_os.h"
#include "rsi_queue.h"
#include "rsi_uart.h"
#include "rsi_events.h"

rsi_linux_app_cb_t rsi_linux_app_cb;



/*==============================================*/
/**
 * @fn          int16_t rsi_frame_read(uint8_t *pkt_buffer)
 * @brief       HAL API to read the packet coming on UART API's
 * @param[in]   pkt_buffer
 * @return      Success
 * @section description This function gets the packet coming from the module and 
 * copies to the buffer pointed 
 * 
 *
 */

int16_t rsi_frame_read(uint8_t *pkt_buffer)
{
	
	memcpy(pkt_buffer , uart_rev_buf+4 , desired_len);

	return 0;

}

/*==============================================*/
/**
 * @fn          void rsi_frame_write(rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param)
 * @brief       This API is used to write the packet on to the UART interface
 * @param[in]   uFrameDscFrame , frame descriptor of the packet
 * @param[in]   payloadparam , payload of the packet
 * @param[in]   size_param , length of the packet 
 * @return      0 in case of success ,
 *              - 1 in case of failure
 * @section description 
 *  This API is used to write the packet on to the UART interface
 * 
 *
 */
int16_t rsi_frame_write(rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param)
{
  int16_t retval = 0;

  //! API to write packet to UART interface
  retval = rsi_uart_send((uint8_t *)uFrameDscFrame, (size_param + RSI_FRAME_DESC_LEN));
  while(huart1.State!=HAL_UART_STATE_READY);
  while(huart1.TxXferCount != 0);
  return retval;
}


/*==============================================*/
/**
 * @fn          int32_t rsi_uart_init(void)
 * @brief       UART initialisation  
 * @return      0 if success and -1 if failure
 * @section description
 * This API initialises the UART interface
 * module. 
 * 
 *
 */

int32_t rsi_uart_init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 921600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	return 0;

}

/*==============================================*/
/**
 * @fn          int32_t rsi_uart_deinit(void)
 * @brief       UART de-initialisation  
 * @return      0 if success and -1 if failure
 * @section description
 * This API initialises the UART interface
 * module. 
 * 
 *
 */

int32_t rsi_uart_deinit(void)
{

  return 0;
}
#endif

