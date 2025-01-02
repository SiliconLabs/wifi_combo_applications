
/**
 * @file    rsi_http_client.c
 * @version 0.1
 * @date    2 Feb 2016
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains HTTP-GET/POST/PUT APIs
 *
 *  @section Description  This file contains HTTP-GET/POST/PUT APIs
 *
 *
 */
//! Include Driver header file
#include "rsi_driver.h"

//! Include Application header file
#include "rsi_http_client.h"

/*
 * Global Variables
 * */
extern rsi_driver_cb_t *rsi_driver_cb;
uint8_t* rsi_itoa(uint32_t val, uint8_t *str);
/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_get_async(uint8_t flags, uint8_t *ip_address, uint16_t port_no, uint8_t *resource,
                                                uint8_t *host_name, uint8_t *extended_header, uint8_t *user_name, uint8_t *password,
                                                void(*http_client_get_response_handler)(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata))
 * @brief      To get the requested http page of given url 
 * @param[in]  flags,  to select version and security , BIT(0) : 0 - IPv4 , 1 - IPv6, BIT(1): 0 - HTTP , 1 - HTTPS
 * @param[in]  ip_address, server IP address
 * @param[in]  port_no, port number : 80 - HTTP , 443 - HTTPS
 * @param[in]  resource, URL string for requested resource
 * @param[in]  host_name, host name
 * @param[in]  extended_header, extender header if present
 * @param[in]  username, username for server Authentication
 * @param[in]  password, password for server Authentication
 * @param[in]  http_client_get_response_handler, call back when asyncronous response comes for the request 
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to get the http data for the requested URL 
 *
 */

int32_t rsi_http_client_get_async(uint8_t flags, uint8_t *ip_address, uint16_t port, 
                            uint8_t *resource, uint8_t *host_name,
                            uint8_t *extended_header, uint8_t *user_name, uint8_t *password,
                            void(*http_client_response_handler)(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata))
{
  return rsi_http_client_async(RSI_HTTP_GET, flags, ip_address, port, resource, host_name, extended_header, user_name, password, NULL, 0, http_client_response_handler);

}


/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_post(uint8_t flags, uint8_t *ip_address, uint16_t port, uint8_t *resource, uint8_t *host_name,
 *                                          uint8_t *extended_header, uint8_t *user_name, uint8_t *password,uint8_t *post_data, uint16_t post_data_length,
                                            void(*http_client_get_response_handler)(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata))
 * @brief      To get the requested http page of given url 
 * @param[in]  flags,  to select version and security , BIT(0) : 0 - IPv4 , 1 - IPv6, BIT(1): 0 - HTTP , 1 - HTTPS
 * @param[in]  ip_address, server IP address
 * @param[in]  port, port number : 80 - HTTP , 443 - HTTPS, non standard ports are also allowed
 * @param[in]  resource, URL string for requested resource
 * @param[in]  host_name, host name
 * @param[in]  extended_header, extender header if present
 * @param[in]  username, username for server Authentication
 * @param[in]  password, password for server Authentication
 * @param[in]  post_data, http data to be posted to server
 * @param[in]  post_data_length, http data length to be posted to server
 * @param[in]  http_client_post_response_handler, call back when asyncronous response comes for the request 
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to post the http data for the requested URL to http server 
 *
 */
int32_t rsi_http_client_post_async(uint8_t flags, uint8_t *ip_address, uint16_t port, 
                            uint8_t *resource, uint8_t *host_name, uint8_t *extended_header, 
                            uint8_t *user_name, uint8_t *password, uint8_t *post_data, uint32_t post_data_length,
                            void(*http_client_response_handler)(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata))
{
  return rsi_http_client_async(RSI_HTTP_POST, flags, ip_address, port, resource, host_name, extended_header, user_name, password, post_data, post_data_length, http_client_response_handler);

}


/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_async(uint8_t type, uint8_t flags, uint8_t *ip_address, uint16_t port, uint8_t *resource, uint8_t *host_name,
                                             uint8_t *extended_header, uint8_t *user_name, uint8_t *password,uint8_t *post_data, uint16_t post_data_length,
                                             void(*callback)(uint8_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata))
 * @brief      To get the requested http page of given url 
 * @param[in]  type, 0 - HTTPGET , 1 - HTTPPOST
 * @param[in]  flags,  to select version and security , BIT(0) : 0 - IPv4 , 1 - IPv6, BIT(1): 0 - HTTP , 1 - HTTPS
 * @param[in]  ip_address, server IP address
 * @param[in]  port, port number : 80 - HTTP , 443 - HTTPS
 * @param[in]  resource, URL string for requested resource
 * @param[in]  host_name, host name
 * @param[in]  extended_header, extender header if present
 * @param[in]  username, username for server Authentication
 * @param[in]  password, password for server Authentication
 * @param[in]  post_data, http data to be posted to server
 * @param[in]  post_data_length, http data length to be posted to server
 * @param[in]  callback, call back when asyncronous response comes for the request 
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to post the http data for the requested URL to http server 
 *
 */

int32_t rsi_http_client_async(uint8_t type, uint8_t flags, uint8_t *ip_address, uint16_t port, 
                            uint8_t *resource, uint8_t *host_name, uint8_t *extended_header, 
                            uint8_t *user_name, uint8_t *password, uint8_t *post_data, uint32_t post_data_length,
                            void(*callback)(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata))
{
  rsi_req_http_client_t     *http_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status = RSI_SUCCESS;
  uint8_t                    https_enable = 0;
  uint16_t                   http_length = 0; 
  uint32_t                   send_size = 0; 
  uint8_t                   *host_desc = NULL;
  uint8_t                    tmp_str[7] = {0};


  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  //! Register http client response notify call back handler to NULL
  rsi_driver_cb->nwk_cb->nwk_callbacks.http_client_response_handler = NULL;

  if(wlan_cb->opermode == RSI_WLAN_CONCURRENT_MODE || wlan_cb->opermode == RSI_WLAN_ACCESS_POINT_MODE )
  {
  //! In concurrent mode and AP mode state should be in RSI_WLAN_STATE_CONNECTED to accept this command 
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

  //! register callback
  if(callback != NULL)
  {
    //! Register http client response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.http_client_response_handler = callback;
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

  http_client = (rsi_req_http_client_t *) pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_req_http_client_t));

  //! Fill ipversion 
  if(flags & RSI_IPV6)
  {  
    //! Set ipv6 version 
    rsi_uint16_to_2bytes(http_client->ip_version, RSI_IP_VERSION_6);
  }
  else
  {
    //! Set ipv4 version 
    rsi_uint16_to_2bytes(http_client->ip_version, RSI_IP_VERSION_4);

  }

  if(flags & RSI_SSL_ENABLE)
  {
    //! Set https feature
    https_enable = 1;

  }

  //! set default by NULL delimiter
  https_enable |= BIT(1);

  if(flags & RSI_SUPPORT_HTTP_POST_DATA)
  {
    //! set HTTP post big data feature bit
    https_enable |= RSI_SUPPORT_HTTP_POST_DATA;
  }

  if(flags & RSI_SUPPORT_HTTP_V_1_1)
  {
    //! set HTTP version 1.1 feature bit
    https_enable |= RSI_SUPPORT_HTTP_V_1_1;
  }

  //! Fill https features parameters
  rsi_uint16_to_2bytes(http_client->https_enable, https_enable);

  //! Fill port no
  http_client->port = port;

  //! Copy username
  rsi_strcpy(http_client->buffer, user_name);
  http_length = rsi_strlen(user_name) + 1;

  //! Copy  password
  rsi_strcpy((http_client->buffer) + http_length, password);
  http_length += rsi_strlen(password) + 1;

  //! Check for HTTP_V_1.1 and Empty host name
  if((flags & RSI_SUPPORT_HTTP_V_1_1) && (rsi_strlen(host_name) == 0))
  {
    host_name = ip_address;
  }

  //! Copy  Host name
  rsi_strcpy((http_client->buffer) + http_length, host_name);
  http_length += rsi_strlen(host_name) + 1;

  //! Copy IP address
  rsi_strcpy((http_client->buffer) + http_length, ip_address);
  http_length += rsi_strlen(ip_address) + 1;

  //! Copy URL resource
  rsi_strcpy((http_client->buffer) + http_length, resource);
  http_length += rsi_strlen(resource) + 1;

  if(extended_header != NULL)
  {

    //! Copy Extended header
    rsi_strcpy((http_client->buffer) + http_length, extended_header);
    http_length += rsi_strlen(extended_header);
  }

  if(type)
  {
    //! Check for HTTP post data feature
    if(flags & RSI_SUPPORT_HTTP_POST_DATA)
    {
      post_data = rsi_itoa(post_data_length, tmp_str);

      post_data_length = rsi_strlen(post_data);
    }

    //! Copy Httppost data
    memcpy((http_client->buffer) + http_length + 1, post_data, post_data_length);

    http_length += (post_data_length + 1);
  }

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_req_http_client_t) - RSI_HTTP_BUFFER_LEN + http_length;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint32_to_4bytes(host_desc, (send_size & 0xFFF));

  //! Magic number to set asyncronous reswponse
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;


  if(type)
  {
    //! send HTTP Post request command
    status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_HTTP_CLIENT_POST, pkt);
  }
  else
  {
    //! send HTTP Get request command
    status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_HTTP_CLIENT_GET, pkt);
  }
  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}


/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_abort(void)
 * @brief      To abort if any http client request is in progress
 * @param[in]  none
 * @param[out] none
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to abort any on going http request 
 *
 */

int32_t rsi_http_client_abort(void)
{
  rsi_pkt_t       *pkt;
  int8_t          status = RSI_SUCCESS;

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

  //! send join command to start wps 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_HTTP_ABORT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;
}



/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_put_create(void)
 * @brief      To create the http put client 
 * @param[in]  none
 * @param[out] none
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to create the http put client
 *
 */

int32_t rsi_http_client_put_create(void)
{
  rsi_pkt_t       *pkt;
  int8_t          status = RSI_SUCCESS;
  uint16_t        send_size = 0; 
  uint8_t         *host_desc = NULL;

  rsi_http_client_put_req_t *http_put_req;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  //! Register http client response notify call back handler to NULL
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_http_client_put_response_handler = NULL;

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

  http_put_req = (rsi_http_client_put_req_t *)pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_http_client_put_req_t));

  //! Fill command type
  http_put_req->command_type = HTTP_CLIENT_PUT_CREATE;

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_http_client_put_req_t) - HTTP_CLIENT_PUT_MAX_BUFFER_LENGTH;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

  //! send http put command  
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_HTTP_CLIENT_PUT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}


/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_put_delete(void)
 * @brief      To delete the created http put client
 * @param[in]  none
 * @param[out] none
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to delete the created http put client 
 *
 */

int32_t rsi_http_client_put_delete(void)
{
  rsi_pkt_t       *pkt;
  int8_t          status = RSI_SUCCESS;
  uint16_t        send_size = 0; 
  uint8_t         *host_desc = NULL;

  rsi_http_client_put_req_t *http_put_req;

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

  http_put_req = (rsi_http_client_put_req_t *)pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_http_client_put_req_t));

  //! Fill command type
  http_put_req->command_type = HTTP_CLIENT_PUT_DELETE;

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_http_client_put_req_t) - HTTP_CLIENT_PUT_MAX_BUFFER_LENGTH;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Magic number to set asyncronous reswponse
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

  //! send http put command  
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_HTTP_CLIENT_PUT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}



/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_put_start(uint8_t flags, uint8_t *ip_address, uint32_t port_number, 
                            uint8_t *resource, uint8_t *host_name, uint8_t *extended_header, 
                            uint8_t *user_name, uint8_t *password, uint32_t content_length,
                            void(*callback)(uint16_t status, uint8_t type, const uint8_t moredata))
 * @brief      To start the http client put process
 * @param[in]  flags,  to select version and security , BIT(0) : 0 - IPv4 , 1 - IPv6, BIT(1): 0 - HTTP , 1 - HTTPS
 * @param[in]  ip_address, server IP address
 * @param[in]  port_number, server port number : 80 - HTTP , 443 - HTTPS
 * @param[in]  resource, URL string for requested resource
 * @param[in]  host_name, host name
 * @param[in]  extended_header, extender header if present
 * @param[in]  user_name, username for server Authentication
 * @param[in]  password, password for server Authentication
 * @param[in]  content_length, http data total length
 * @param[in]  post_data_length, http data length to be posted to server
 * @param[in]  callback, call back when asyncronous response comes for the request
 * @param[out] none
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to start the http client put process  
 *
 */

int32_t rsi_http_client_put_start(uint8_t flags, uint8_t *ip_address, uint32_t port_number, 
                            uint8_t *resource, uint8_t *host_name, uint8_t *extended_header, 
                            uint8_t *user_name, uint8_t *password, uint32_t content_length,
                            void(*callback)(uint16_t status, uint8_t type, const uint8_t *buffer, uint16_t length, const uint8_t end_of_put_pkt))
{
  rsi_pkt_t                     *pkt;
  int8_t                         status = RSI_SUCCESS;
  uint8_t                        https_enable = 0;
  uint16_t                       http_length = 0; 
  uint16_t                       send_size = 0; 
  uint8_t                       *host_desc = NULL;
  rsi_http_client_put_req_t     *http_put_req;
  rsi_http_client_put_start_t   *http_put_start;

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

  //! register callback
  if(callback != NULL)
  {
    //! Register http client response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_http_client_put_response_handler = callback;
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

  http_put_req = (rsi_http_client_put_req_t *)pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_http_client_put_req_t));

  http_put_start = &http_put_req->http_client_put_struct.http_client_put_start;

  //! Fill command type
  http_put_req->command_type = HTTP_CLIENT_PUT_START;

  //! Fill ipversion 
  if(flags & RSI_IPV6)
  {  
    //! Set ipv6 version 
    http_put_start->ip_version = RSI_IP_VERSION_6;
  }
  else
  {
    //! Set ipv4 version 
    http_put_start->ip_version = RSI_IP_VERSION_4;

  }

  if(flags & RSI_SSL_ENABLE)
  {
    //! Set https feature
    https_enable = 1;

    //! set default by NULL delimiter
    https_enable |= BIT(1);

    //! Fill https features parameters
    rsi_uint16_to_2bytes(http_put_start->https_enable, https_enable);
  }

  //! Fill http server port number
  rsi_uint32_to_4bytes(http_put_start->port_number, port_number);

  //! Fill Total resource content length
  rsi_uint32_to_4bytes(http_put_start->content_length, content_length);

  //! Copy username
  rsi_strcpy(http_put_req->http_put_buffer, user_name);
  http_length = rsi_strlen(user_name) + 1;

  //! Copy  password
  rsi_strcpy((http_put_req->http_put_buffer) + http_length, password);
  http_length += rsi_strlen(password) + 1;

  //! Copy  Host name
  rsi_strcpy((http_put_req->http_put_buffer) + http_length, host_name);
  http_length += rsi_strlen(host_name) + 1;

  //! Copy IP address
  rsi_strcpy((http_put_req->http_put_buffer) + http_length, ip_address);
  http_length += rsi_strlen(ip_address) + 1;

  //! Copy URL resource
  rsi_strcpy((http_put_req->http_put_buffer) + http_length, resource);
  http_length += rsi_strlen(resource) + 1;

  if(extended_header != NULL)
  {
    //! Copy Extended header
    rsi_strcpy((http_put_req->http_put_buffer) + http_length, extended_header);
    http_length += rsi_strlen(extended_header);
  }

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_http_client_put_req_t) - HTTP_CLIENT_PUT_MAX_BUFFER_LENGTH + http_length;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

  //! Magic number to set asyncronous reswponse
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

  //! send http put command  
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_HTTP_CLIENT_PUT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}


/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_put_pkt(uint8_t *file_content, uint16_t current_chunk_length)
 * @brief      To put http data onto the http server for the created URL resource
 * @param[in]  file_content, HTTP data content
 * @param[in]  current_chunk_length, HTTP data current chunk length
 * @param[out] none
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to http data onto the http server for the created URL resource 
 *
 */

int32_t rsi_http_client_put_pkt(uint8_t *file_content, uint16_t current_chunk_length)
{
  rsi_pkt_t                      *pkt;
  int8_t                          status = RSI_SUCCESS;
  rsi_http_client_put_req_t      *http_put_req;
  rsi_http_client_put_data_req_t *http_put_data;
  uint16_t                        send_size = 0; 
  uint8_t                        *host_desc = NULL;


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

  http_put_req = (rsi_http_client_put_req_t *)pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_http_client_put_req_t));

  http_put_data = &http_put_req->http_client_put_struct.http_client_put_data_req;

  //! Fill command type
  http_put_req->command_type = HTTP_CLIENT_PUT_PKT;

  //! Fill http put current_chunk_length
  rsi_uint16_to_2bytes(http_put_data->current_length, current_chunk_length);

  //! Fill resource content
  memcpy((uint8_t *)http_put_req->http_put_buffer, file_content, current_chunk_length);

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_http_client_put_req_t) - HTTP_CLIENT_PUT_MAX_BUFFER_LENGTH + current_chunk_length;

  //! Magic number to set asyncronous reswponse
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

  //! send http put command  
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_HTTP_CLIENT_PUT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}



/*==============================================*/
/**
 * @fn         int32_t rsi_http_client_post_data(uint8_t *file_content, uint16_t current_chunk_length, 
 *                         void(* rsi_http_post_data_response_handler)(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata))
 * @brief      To post http data onto the http server for the URL resource
 * @param[in]  file_content, HTTP data content
 * @param[in]  current_chunk_length, HTTP data current chunk length
 * @param[in]  callback, call back when asyncronous response comes for the request
 * @param[out] none
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to http data onto the http server for the created URL resource 
 *
 */

int32_t rsi_http_client_post_data(uint8_t *file_content, uint16_t current_chunk_length, 
               void(* rsi_http_post_data_response_handler)(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata))
{
  rsi_pkt_t                      *pkt;
  int8_t                          status = RSI_SUCCESS;
  rsi_http_client_post_data_req_t      *http_post_data;
  uint16_t                        send_size = 0; 
  uint8_t                        *host_desc = NULL;


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

  //! register callback
  if(rsi_http_post_data_response_handler != NULL)
  {
    //! Register http client response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_http_client_post_data_response_handler = rsi_http_post_data_response_handler;
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
  
  http_post_data = (rsi_http_client_post_data_req_t *)pkt->data;

  //! memset the packet data to insert NULL between fields
  memset(&pkt->data, 0, sizeof(rsi_http_client_post_data_req_t));


  //! Fill http post data current_chunk_length
  rsi_uint16_to_2bytes(http_post_data->current_length, current_chunk_length);

  //! Fill resource content
  memcpy((uint8_t *)http_post_data->http_post_data_buffer, file_content, current_chunk_length);


  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_http_client_post_data_req_t) - HTTP_CLIENT_POST_DATA_MAX_BUFFER_LENGTH + current_chunk_length;

  //! Magic number to set asyncronous reswponse
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

  //! send http put command  
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_HTTP_CLIENT_POST_DATA, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}


