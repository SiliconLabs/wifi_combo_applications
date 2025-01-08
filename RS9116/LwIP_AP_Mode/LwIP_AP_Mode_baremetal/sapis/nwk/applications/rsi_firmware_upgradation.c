/**
 * @file    rsi_fwup.c
 * @version 0.1
 * @date    19 Oct 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains firmware upgradation APIs
 *
 *  @section Description  This file contains firmware upgradation APIs 
 *  API's.
 *
 *
 */

#include "rsi_driver.h"
#include "rsi_firmware_upgradation.h"

/*==============================================*/
/**
 * @fn          int32_t rsi_fwup(uint8_t type, uint8_t *content, uint16_t length)
 * @brief       This is a helper function for actual APIs
 * @param[in]   type, firmware upgrade chunk type 
 * @param[in]   content, firmware content
 * @param[in]   length, length of the content
 * @param[out]  None 
 * @return      int32_t
 *              0  =  success
 *              3  =  Firmware upgradation completed successfully
 *              <0 =  failure 
 *
 * @section description
 * This is a helper function and called in actual firmware upgradation APIs
 * This API will take care of filling upgradation request and sends it to device
 *
 */
static int32_t rsi_fwup(uint8_t type, uint8_t *content, uint16_t length)
{
  int32_t status = RSI_SUCCESS;
  
  rsi_pkt_t       *pkt;
 
  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_req_fwup_t *fwup = NULL;

  //! Check if length exceeds
  if(length > RSI_MAX_FWUP_CHUNK_SIZE)
  {
    return RSI_ERROR_INVALID_PARAM;
  }
  //! take lock on wlan control block
  rsi_mutex_lock(&wlan_cb->wlan_mutex);

  //! allocate command buffer  from wlan pool
  pkt = rsi_pkt_alloc(&wlan_cb->wlan_tx_pool);
  
  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&wlan_cb->wlan_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  fwup =  (rsi_req_fwup_t *)pkt->data;

  //! Fill packet type
  rsi_uint16_to_2bytes(fwup->type, type);

  //! Fill packet length
  rsi_uint16_to_2bytes(fwup->length, length);

  //! Fill packet content
  memcpy(fwup->content, content, length);

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FWUP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;

}

/*==============================================*/
/**
 * @fn          int32_t rsi_fwup_start(uint8_t *rps_header)
 * @brief       This API is used to send the RPS header content of the firmware file
 * @param[in]   rps_header, pointer to the rps header content
 * @param[out]  None
 * @return      int32_t
 *              0  =  success
 *              <0 =  failure 
 *
 * @section description
 * This API is used to send the RPS header content of the firmware file
 *
 */
int32_t rsi_fwup_start(uint8_t *rps_header)
{
  int32_t status = RSI_SUCCESS;

  status = rsi_fwup(RSI_FWUP_RPS_HEADER, rps_header, RSI_RPS_HEADER_SIZE);

  return status;
}

/*==============================================*/
/**
 * @fn          int32_t rsi_fwup_load(uint8_t *content, uint16_t length)
 * @brief       This API is used to send the RPS file content
 * @param[in]   content, pointer to the firmware file content
 * @param[out]  None
 * @return      int32_t
 *              0  =  success
 *              3  =  Firmware upgradation is completed successfully
 *              <0 =  failure 
 *
 * @section description
 * This API is used to send the firmware file content
 *
 */
int32_t rsi_fwup_load(uint8_t *content, uint16_t length)
{
  int32_t status = RSI_SUCCESS;

  status = rsi_fwup(RSI_FWUP_RPS_CONTENT, content, length);

  return status;

}
