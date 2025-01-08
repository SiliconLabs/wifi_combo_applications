/**
 * @file    rsi_ota_fw_up.c
 * @version 0.1
 * @date    6 Oct 2016
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
 * @fn          int32_t rsi_ota_firmware_upgradation(uint8_t ip_version, uint8_t *server_ip, uint32_t server_port, uint16_t chunk_number, uint16_t time_out, uint16_t tcp_retry_count);
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

int32_t rsi_ota_firmware_upgradation(uint8_t flags, uint8_t *server_ip, uint32_t server_port, uint16_t chunk_number, uint16_t timeout, 
                                      uint16_t tcp_retry_count, void(*ota_fw_up_response_handler)(uint16_t status, uint16_t chunk_number))
{
  int32_t status = RSI_SUCCESS;
  
  rsi_pkt_t       *pkt;
 
  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_req_ota_fwup_t *otaf_fwup = NULL;


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

  otaf_fwup =  (rsi_req_ota_fwup_t *)pkt->data;

  //! register callback
  if(ota_fw_up_response_handler != NULL)
  {
    //! Register smtp client response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_ota_fw_up_response_handler = ota_fw_up_response_handler;
  }
  else
	{
		//! free the packet
		rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

		//! unlock mutex
		rsi_mutex_unlock(&wlan_cb->wlan_mutex);

		//! return invalid command error
		return RSI_ERROR_INVALID_PARAM;
	}

  //! Check ip version
  if(!(flags & RSI_IPV6)) 
  {
    //! Fill the IP version
    otaf_fwup->ip_version = RSI_IP_VERSION_4;

    //! Set IP address to localhost
    memcpy(otaf_fwup->server_ip_address.ipv4_address, server_ip, RSI_IPV4_ADDRESS_LENGTH);
  }
  else
  {
    //! Fill the IP version
    otaf_fwup->ip_version = RSI_IP_VERSION_6;
    
    //! Set IP address to localhost
    memcpy(otaf_fwup->server_ip_address.ipv6_address, server_ip, RSI_IPV6_ADDRESS_LENGTH);
  }


  //! Fill server port number
  rsi_uint32_to_4bytes(otaf_fwup->server_port, server_port);

  //! Fill chunk number
  rsi_uint16_to_2bytes(otaf_fwup->chunk_number, chunk_number);

  //! Fill timeout
  rsi_uint16_to_2bytes(otaf_fwup->timeout, timeout);
  
  //! Fill tcp retry count
  rsi_uint16_to_2bytes(otaf_fwup->retry_count, tcp_retry_count);

  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

  //! Send OTAF command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_OTA_FWUP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;

}


