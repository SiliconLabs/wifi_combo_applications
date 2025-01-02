/**
 * @file          rsi_mdnsd.c
 * @version       0.1  
 * @date          30-Oct 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains APIS related to MDNS service functionality
 *
 *  @section Description  This file contains MDNSD APIS 
 *
 *
 */

//! Include Driver header file
#include "rsi_driver.h"

/*==============================================*/
/**
 * @fn               int32_t rsi_mdnsd_init(uint8_t ip_version, uint16_t ttl, uint8_t *host_name)
 * @brief            This function is used to initialise MDNSD service
 * @param[in]        ip_version, 4 - IPv4 ,6 - IPv6 
 * @param[in]        ttl, time to live 
 * @param[in]        host_name, host name
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to initaialize MDNSD service 
 *
 */
int32_t rsi_mdnsd_init(uint8_t ip_version, uint16_t ttl, uint8_t *host_name)
{
  rsi_req_mdnsd_t            *mdnsd;
  rsi_pkt_t                 *pkt;
  int32_t                    status = RSI_SUCCESS;
  uint16_t                   send_size = 0; 
  uint8_t                   *host_desc = NULL;


  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  if((rsi_strlen(host_name) + 1) > MDNSD_BUFFER_SIZE)
  {
     return RSI_ERROR_INSUFFICIENT_BUFFER;
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

  mdnsd = (rsi_req_mdnsd_t *) pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_req_mdnsd_t));

  //! Fill command type
  mdnsd->command_type = RSI_MDNSD_INIT;

  //! Fill ip version
  mdnsd->mdnsd_struct.mdnsd_init.ip_version = ip_version;

  //! Fill time to live
  rsi_uint16_to_2bytes(mdnsd->mdnsd_struct.mdnsd_init.ttl, ttl);

  //! Copy host name
  rsi_strcpy(mdnsd->buffer, host_name);
 
  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_req_mdnsd_t) - MDNSD_BUFFER_SIZE + rsi_strlen(mdnsd->buffer) + 1;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

  //! send MDNSD request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_MDNSD, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}

/*==============================================*/
/**
 * @fn         int32_t rsi_mdnsd_register_service(uint8_t port, uint16_t ttl, uint8_t more, uint8_t *service_ptr_name, uint8_t *service_name, uint8_t *service_text)
 * @brief      This API is used to register services  
 * @param[in]  port, MDNSD server port number 
 * @param[in]  ttl, time to live 
 * @param[in]  more, set this bit if more services are yet to be added
 * @param[in]  service_ptr_name,service pointer name
 * @param[in]  service_name, service name
 * @param[in]  service_text, service text
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This API is used to register services  
 *
 *
 */
int32_t rsi_mdnsd_register_service(uint16_t port, uint16_t ttl, uint8_t more, uint8_t *service_ptr_name, uint8_t *service_name, uint8_t *service_text)
{
  rsi_req_mdnsd_t            *mdnsd;
  rsi_pkt_t                 *pkt;
  int32_t                    status = RSI_SUCCESS;
  uint16_t                   send_size = 0; 
  uint8_t                   *host_desc = NULL;


  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  if((rsi_strlen(service_ptr_name) + rsi_strlen(service_name) + rsi_strlen(service_text) + 3) > MDNSD_BUFFER_SIZE)
  {
     return RSI_ERROR_INSUFFICIENT_BUFFER;
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

  mdnsd = (rsi_req_mdnsd_t *) pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_req_mdnsd_t));

  //! Fill command type
  mdnsd->command_type = RSI_MDNSD_REGISTER_SERVICE;

  //! Fill port number
  rsi_uint16_to_2bytes(mdnsd->mdnsd_struct.mdnsd_register_service.port, port);

  //! Fill time to live
  rsi_uint16_to_2bytes(mdnsd->mdnsd_struct.mdnsd_register_service.ttl, ttl);

  //! more
  mdnsd->mdnsd_struct.mdnsd_register_service.more = more;
  
  //! Copy service pointer name
  rsi_strcpy(mdnsd->buffer, service_ptr_name);
  send_size = rsi_strlen(service_ptr_name) + 1;
 
  //! Copy service name
  rsi_strcpy((mdnsd->buffer) + send_size, service_name);
  send_size += rsi_strlen(service_name) + 1;
 
  //! Copy service text
  rsi_strcpy((mdnsd->buffer) + send_size, service_text);
  send_size += rsi_strlen(service_text) + 1;
  
  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_req_mdnsd_t) - MDNSD_BUFFER_SIZE + send_size;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

  //! send MDNSD request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_MDNSD, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}

/*==============================================*/
/**
 * @fn         int32_t rsi_mdnsd_deinit(void)
 * @brief      This API is used to deinit the MDNSD service
 * @param[in]  None 
 * @param[out] None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This API is used to deinit the MDNSD service
 *
 */

int32_t rsi_mdnsd_deinit(void)
{
  rsi_req_mdnsd_t            *mdnsd;
  rsi_pkt_t                 *pkt;
  int32_t                    status = RSI_SUCCESS;
  uint16_t                   send_size = 0; 
  uint8_t                   *host_desc = NULL;


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

  mdnsd = (rsi_req_mdnsd_t *) pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_req_mdnsd_t));

  //! Fill command type
  mdnsd->command_type = RSI_MDNSD_DEINIT;

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_req_mdnsd_t) - MDNSD_BUFFER_SIZE;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

  //! send MDNSD request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_MDNSD, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}
