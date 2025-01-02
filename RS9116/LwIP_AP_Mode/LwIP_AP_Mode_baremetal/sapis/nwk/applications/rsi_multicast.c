/**
 * @file    rsi_multicast.c
 * @version 0.1
 * @date    08 Oct 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains multicast API
 *
 *  @section Description  This file contains multicast API
 *
 *
 */

#include "rsi_driver.h"

/*==============================================*/
/**
 * @fn          int32_t rsi_multicast(uint8_t flags, int8_t *ip_address, uint8_t command_type)
 * @brief       This is a helper function for actual APIs
 * @param[in]   flags,  to select version and security , BIT(0) : 0 - IPv4 , 1 - IPv6
 * @param[in]   ip_address, multicast IP address
 * @param[in]   command_type, type of the command JOIN/LEAVE
 * @param[out]  None
 * @return      int32_t
 *              0  =  success
 *              <0 =  failure 
 *
 * @section description
 * This function is helper function and called in actual multiucast APIs
 * This function will take of filling multicast request and sends it to device
 *
 */

static int32_t rsi_multicast(uint8_t flags, int8_t *ip_address, uint8_t command_type)
{
  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;
  rsi_req_multicast_t *multicast;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;


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

  multicast =  (rsi_req_multicast_t *)pkt->data;

  //! Fill IP version and IP address
  if(flags & RSI_IPV6)
  {
    //! Fill IPv6 version
    rsi_uint16_to_2bytes(multicast->ip_version, RSI_IP_VERSION_6);

    //! Fill IPv6 address
    memcpy(multicast->multicast_address.ipv6_address, ip_address, RSI_IPV6_ADDRESS_LENGTH);
  }
  else
  {
    //! Fill IPv4 version
    rsi_uint16_to_2bytes(multicast->ip_version, RSI_IP_VERSION_4);

    //! Fill IPv4 address
    memcpy(multicast->multicast_address.ipv4_address, ip_address, RSI_IPV4_ADDRESS_LENGTH);
  }

  //! Fill command type
  rsi_uint16_to_2bytes(multicast->type, command_type);

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_MULTICAST, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;
}

/*==============================================*/
/**
 * @fn          int32_t rsi_multicast_join(uint8_t flags, int8_t *ip_address)
 * @brief       This API is used to join a multicast IP address
 * @param[in]   flags,  to select version and security , BIT(0) : 0 - IPv4 , 1 - IPv6
 * @param[in]   ip_address, multicast IP address to join
 * @param[out]  None
 * @return      int32_t
 *              0  =  success
 *              <0 =  failure 
 *
 * @section description
 * This API is used to join a multicast IP address
 *
 */
int32_t rsi_multicast_join(uint8_t flags, int8_t *ip_address)
{
  int32_t  status = RSI_SUCCESS;

  status = rsi_multicast(flags, ip_address, RSI_MULTICAST_JOIN);

  return status;
}


/*==============================================*/
/**
 * @fn          int32_t rsi_multicast_leave(uint8_t flags, int8_t *ip_address)
 * @brief       This API is used to leave a multicast IP address
 * @param[in]   flags,  to select version and security , BIT(0) : 0 - IPv4 , 1 - IPv6
 * @param[in]   ip_address, multicast IP address to leave
 * @param[out]  None
 * @return      int32_t
 *              0  =  success
 *              <0 =  failure 
 *
 * @section description
 * This API is used to leave a multicast IP address
 *
 */
int32_t rsi_multicast_leave(uint8_t flags, int8_t *ip_address)
{
  int32_t  status = RSI_SUCCESS;

  status = rsi_multicast(flags, ip_address, RSI_MULTICAST_LEAVE);

  return status;
}


