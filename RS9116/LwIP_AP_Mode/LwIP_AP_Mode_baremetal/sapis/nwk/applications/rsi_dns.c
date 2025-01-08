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

#include "rsi_driver.h"

/*==============================================*/
/**
 * @fn         rsi_dns_req(uint8_t ip_version, uint8_t *url_name, uint8_t *primary_server_address, uint8_t *secondary_server_address, 
 *                         rsi_rsp_dns_query_t *dns_query_resp, uint16_t length)
 * @brief      To query the ip address of the URL name 
 * @param[in]  ip_version , ip version of the DNS client 
 * @param[in]  url_name , URL address of the domine 
 * @param[in]  primary_server_address , DNS server primary address, if user is not giving primary DNS server address give it NULL
 * @param[in]  secondary_server_address , DNS server secondary address, if user is not giving secondary DNS server IP address give this param NULL
 * @param[in]  dns_query_resp , buffer address provided by the application to fill the DNS query response
 * @param[in]  length , Length of the buffer provided by the application 
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to query the ip address of the URL name and posts the scan respone to 
 * application. Application should call this api to get the DNS qery response.
 *
 */

int32_t rsi_dns_req(uint8_t ip_version, uint8_t *url_name, uint8_t *primary_server_address, uint8_t *secondary_server_address, 
                    rsi_rsp_dns_query_t *dns_query_resp, uint16_t length)
{
  rsi_req_dns_server_add_t  *dns_srever_add;
  rsi_req_dns_query_t       *dns_query;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
  uint8_t                   dns_mode = 0;


  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  if(wlan_cb->opermode == RSI_WLAN_CONCURRENT_MODE || wlan_cb->opermode == RSI_WLAN_ACCESS_POINT_MODE )
  {
    //! In concurrent mode or AP mode, state should be in RSI_WLAN_STATE_CONNECTED to accept this command 
    if((wlan_cb->state < RSI_WLAN_STATE_CONNECTED))
    {
      //!command given in wrong state
      return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
    }
  }
  else
  {
    //! if state is not in ipconfig done state
    if((wlan_cb->state < RSI_WLAN_STATE_IP_CONFIG_DONE))
    {
      //! Command given in wrong state 
      return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
    }
  }

  //! Check for invalid parameters
  if(((ip_version != RSI_IP_VERSION_4) && (ip_version != RSI_IP_VERSION_6)))
  {  
    //! Throw error in case of invalid parameters
    return RSI_ERROR_INVALID_PARAM;
  }

  //! Check for invalid parameters
  if((dns_query_resp == NULL) || (length == 0))
  {
    //! Throw error in case of invalid parameters
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

  dns_srever_add = (rsi_req_dns_server_add_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_dns_server_add_t));

  //! Set ip version 
  rsi_uint16_to_2bytes(dns_srever_add->ip_version, ip_version);

  //! Set DNS mode 
  dns_mode = (primary_server_address != NULL) ? RSI_STATIC : RSI_DHCP;

  //! Set user selected DNS mode 
  rsi_uint16_to_2bytes(dns_srever_add->dns_mode, dns_mode);

  if(ip_version == RSI_IP_VERSION_4)
  {

    if(primary_server_address)
    {
      //! Fill Primary IP address
      memcpy(dns_srever_add->ip_address1.primary_dns_ipv4, primary_server_address, RSI_IPV4_ADDRESS_LENGTH);
    }
    if(secondary_server_address) 
    {
      memcpy(dns_srever_add->ip_address2.secondary_dns_ipv4, secondary_server_address, RSI_IPV4_ADDRESS_LENGTH);
    }
  }
  else
  {
    if(primary_server_address)
    {
      //! Fill Primary IP address
      memcpy(dns_srever_add->ip_address1.primary_dns_ipv6, primary_server_address, RSI_IPV6_ADDRESS_LENGTH);
    }
    if(secondary_server_address) 
    {
      memcpy(dns_srever_add->ip_address2.secondary_dns_ipv6, secondary_server_address, RSI_IPV6_ADDRESS_LENGTH);
    }
  }

  //! send DNS server add command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_DNS_SERVER_ADD, pkt);

  if(status != RSI_SUCCESS)
  {
    //! free the packet
    rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

    //! unlock mutex
    rsi_mutex_unlock(&wlan_cb->wlan_mutex);

    //! Return the status
    return status;

  }


  dns_query = (rsi_req_dns_query_t *)pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_dns_query_t));

  //! attach the buffer given by user
  wlan_cb->app_buffer = (uint8_t *)dns_query_resp;

  //! length of the buffer provided by user 
  wlan_cb->app_buffer_length = length; 

  //! Set ip version 
  rsi_uint16_to_2bytes(dns_query->ip_version, ip_version);

  //! Set URL name 
  rsi_strcpy(dns_query->url_name, url_name);


  //! Set DNS server number 
  rsi_uint16_to_2bytes(dns_query->dns_server_number, 1);

  //! send DNS query command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_DNS_QUERY, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}



/*==============================================*/
/**
 * @fn         rsi_dns_update(uint8_t ip_version, uint8_t *url_name, uint8_t *server_address, uint16_t ttl, void(*rsi_dns_update_rsp_handler)(uint16_t status));
 * @brief      To query the ip address of the URL name 
 * @param[in]  ip_version , ip version of the DNS client 
 * @param[in]  zone_name , ZONE name of the domine 
 * @param[in]  host_name , HOST name of the domine 
 * @param[in]  server_address , DNS server address
 * @param[in]  ttl , TTL value of the DNS hostname
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to update  the hostname of the module 
 * application. Application should call this api to get the DNS update response.
 *
 */

int32_t rsi_dns_update(uint8_t ip_version, uint8_t *zone_name, uint8_t *host_name, uint8_t *server_address, uint16_t ttl, void(*dns_update_rsp_handler)(uint16_t status))
{
  rsi_req_dns_server_add_t  *dns_srever_add;
  rsi_req_dns_update_t      *dns_update;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
  uint8_t                   dns_mode = 0;


  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  if(wlan_cb->opermode == RSI_WLAN_CONCURRENT_MODE || wlan_cb->opermode == RSI_WLAN_ACCESS_POINT_MODE )
  {
    //! In concurrent mode or AP mode, state should be in RSI_WLAN_STATE_CONNECTED to accept this command 
    if((wlan_cb->state < RSI_WLAN_STATE_CONNECTED))
    {
      //!command given in wrong state
      return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
    }
  }
  else
  {
    //! if state is not in ipconfig done state
    if((wlan_cb->state < RSI_WLAN_STATE_IP_CONFIG_DONE))
    {
      //! Command given in wrong state 
      return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
    }
  }

  //! Check for invalid parameters
  if(((ip_version != RSI_IP_VERSION_4) && (ip_version != RSI_IP_VERSION_6)))
  {  
    //! Throw error in case of invalid parameters
    return RSI_ERROR_INVALID_PARAM;
  }


  //! register callback
  if(dns_update_rsp_handler != NULL)
  {
    //! Register smtp client response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_dns_update_rsp_handler = dns_update_rsp_handler;
  }
  else
  {
    //! return invalid command error
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

  dns_srever_add = (rsi_req_dns_server_add_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_dns_server_add_t));

  //! Set ip version 
  rsi_uint16_to_2bytes(dns_srever_add->ip_version, ip_version);

  //! Set DNS mode 
  dns_mode = (server_address != NULL) ? RSI_STATIC : RSI_DHCP;

  //! Set user selected DNS mode 
  rsi_uint16_to_2bytes(dns_srever_add->dns_mode, dns_mode);

  if(ip_version == RSI_IP_VERSION_4)
  {

    if(server_address)
    {
      //! Fill Primary IP address
      memcpy(dns_srever_add->ip_address1.primary_dns_ipv4, server_address, RSI_IPV4_ADDRESS_LENGTH);
    }
  }
  else
  {
    if(server_address)
    {
      //! Fill Primary IP address
      memcpy(dns_srever_add->ip_address1.primary_dns_ipv6, server_address, RSI_IPV6_ADDRESS_LENGTH);
    }
  }

  //! send DNS server add command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_DNS_SERVER_ADD, pkt);

  if(status != RSI_SUCCESS)
  {
    //! free the packet
    rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

    //! unlock mutex
    rsi_mutex_unlock(&wlan_cb->wlan_mutex);

    //! Return the status
    return status;

  }


  dns_update = (rsi_req_dns_update_t *)pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_dns_update_t));

  //! Set ip version 
  dns_update->ip_version = ip_version;

  //! Set ZONE name 
  rsi_strcpy(dns_update->zone_name, zone_name);

  //! Set HOST name 
  rsi_strcpy(dns_update->host_name, host_name);

  //! Fill DNS server number 
  rsi_uint16_to_2bytes(dns_update->dns_server_number, 1);
  
  //! Fill DNS TTL value 
  rsi_uint16_to_2bytes(dns_update->ttl, ttl);

  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

  //! send DNS update command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_DNS_UPDATE, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}
