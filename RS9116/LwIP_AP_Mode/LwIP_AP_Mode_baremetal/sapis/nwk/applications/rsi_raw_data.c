/**
 * @file    rsi_dns.c
 * @version 0.1
 * @date    15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains DNS API
 *
 *  @section Description  This file contains DNS API
 *
 *
 */

/*==============================================*/
/**
 * @fn         rsi_send_raw_data(uint8_t* buffer, uint32_t length)
 * @brief      This API is used to send RAW data to Module 
 * @param[in]  buffer, This is the pointer to the buffer to send 
 * @param[in]  length, This is the length of the buffer to send
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * 
 *
 */

#include "rsi_driver.h"
uint16_t rsi_send_raw_data(uint8_t* buffer, uint32_t length)
{
  int32_t  status = RSI_SUCCESS;
  uint8_t  *host_desc;
  rsi_pkt_t *pkt;

  //! If buffer is not valid
  if((buffer == NULL) ||(length == 0))
  {
    //! return packet allocation failure error
    return RSI_ERROR_INVALID_PARAM;
  }

  //! Allocate packet to send data
  pkt = rsi_pkt_alloc(&rsi_driver_cb->wlan_cb->wlan_tx_pool);

  if(pkt == NULL)
  {
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  //! Get host descriptor pointer
  host_desc = pkt->desc;

  //! Memset host descriptor
  memset(host_desc, 0, RSI_HOST_DESC_LENGTH);

  //! Fill host descriptor
  rsi_uint16_to_2bytes(host_desc, (length & 0xFFF));

  //! Fill packet type
  host_desc[1] |= (RSI_WLAN_DATA_Q << 4);

    //! Fill frame type
  host_desc[2] = 0x1;

  //! Copy data to be sent
  memcpy(pkt->data, buffer, length);  

  //! Enqueue packet to WLAN TX queue
  rsi_enqueue_pkt(&rsi_driver_cb->wlan_tx_q, pkt);


  //! Magic number to set asyncronous response
  rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;

  //! Set TX packet pending event
  rsi_set_event(RSI_TX_EVENT);

  //! Wait on wlan semaphore
  rsi_semaphore_wait(&rsi_driver_cb->wlan_cb->wlan_sem, RSI_WAIT_FOREVER);    

  //! Clear expected response
  rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_CLEAR;
  //! Get wlan status
  status = rsi_wlan_get_status();

  //! Free packet
  rsi_pkt_free(&rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  //! Return status
  return status;

}

