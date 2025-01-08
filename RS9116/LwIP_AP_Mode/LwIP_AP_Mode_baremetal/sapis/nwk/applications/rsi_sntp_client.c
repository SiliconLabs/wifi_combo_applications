/**
 * @file
 * @version
 * @date
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief :
 *
 *  @section Description  This file contains 
 *
 *
 */

#include "rsi_driver.h"

#include "rsi_sntp_client.h"

#include "rsi_nwk.h"

/*==============================================*/
/**
 * @fn                int32_t rsi_sntp_client_create_async(uint8_t flags, uint8_t *server_ip, uint8_t sntp_method,  
                           void(*sntp_client_create_response_handler)(uint16_t status,const uint8_t cmd_type, const uint8_t *buffer ))
 * @brief             This function is used to create an SNTP client 
 * @param[in]   ,     flags,  to enable IPv6, set BIT(0), Bydefault it selects IPv4 
 * @param[in]   ,     sntp_method, client authentication mode: 1- RSI_SNTP_BROADCAST_MODE   2- RSI_SNTP_UNICAST_MODE
 * @param[in]   ,     server_ip, SNTP server address
 * @param[in]   ,     sntp_client_create_response_handler,call back when the create response comes from host 
 * @param[out]  ,     None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to create SNTP client  
 *
 *
 */

int32_t rsi_sntp_client_create_async(uint8_t flags, uint8_t *server_ip, uint8_t sntp_method, uint16_t sntp_timeout, 
		void(*rsi_sntp_client_create_response_handler)(uint16_t status, const uint8_t cmd_type, const uint8_t *buffer ))

{
	rsi_sntp_client_t          *sntp_client;
	rsi_pkt_t                  *pkt;
	int32_t                    status   = RSI_SUCCESS;
	uint16_t                   send_size = 0;
	uint8_t                    *host_desc = NULL;


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

	//! register callback
	if(rsi_sntp_client_create_response_handler != NULL)
	{
		//! Register pop3 client response notify call back handler
		rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_sntp_client_create_response_handler = rsi_sntp_client_create_response_handler;
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

	sntp_client = (rsi_sntp_client_t *)pkt->data;

	//! memset the packet data
	memset(&pkt->data, 0, sizeof(rsi_sntp_client_t));

	//! Fill command type
	sntp_client->command_type = RSI_SNTP_CREATE;

	if ((sntp_method != RSI_SNTP_BROADCAST_MODE) && (sntp_method != RSI_SNTP_UNICAST_MODE))
	{
		//! default SNTP is in  UNICAST mode 
		sntp_method = RSI_SNTP_UNICAST_MODE;
	}

	//! Fill SNTP method
	sntp_client->sntp_method = sntp_method;

	//! sntp time out 
	rsi_uint16_to_2bytes(sntp_client->sntp_timeout, sntp_timeout);

	//! Check for IP version
	if(!(flags & RSI_IPV6)) 
	{
		sntp_client->ip_version = RSI_IP_VERSION_4;
		memcpy(sntp_client->server_ip_address.ipv4_address, server_ip, RSI_IPV4_ADDRESS_LENGTH);
	}
	else 
	{
		sntp_client->ip_version = RSI_IP_VERSION_6;
		memcpy(sntp_client->server_ip_address.ipv4_address, server_ip,RSI_IPV6_ADDRESS_LENGTH);

	}

	//! Using host descriptor to set payload length
	send_size = sizeof(rsi_sntp_client_t);

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! Magic number to set asyncronous response
	wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

	//! send SNTP Get request command
	status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SNTP_CLIENT, pkt);

	//! free the packet
	rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&wlan_cb->wlan_mutex);

	//! Return the status
	return status;

}



/*==============================================*/
/**
 * @fn                int32_t rsi_sntp_client_gettime(void)
 * @brief             This function is used to get SNTP client time 
 * @param[in]   ,     length, Reponse buffer length
 * @param[in]   ,     sntp_time_rsp, Pointer to the SNTP Reponse buffer 
 * @param[out]  ,     None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to get the time
 *
 *
 */

int32_t rsi_sntp_client_gettime(uint16_t length, uint8_t *sntp_time_rsp)
{

	rsi_sntp_client_t     *sntp_client;
	rsi_pkt_t                 *pkt;
	int32_t                    status   = RSI_SUCCESS;
	uint16_t                   send_size = 0;
	uint8_t                   *host_desc = NULL;


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


  //! attach the buffer given by user
  wlan_cb->app_buffer = (uint8_t *)sntp_time_rsp;

  //! length of the buffer provided by user 
  wlan_cb->app_buffer_length = length; 

	//! memset the packet data
	memset(&pkt->data, 0, sizeof(rsi_sntp_client_t));
	
  sntp_client = (rsi_sntp_client_t *)  pkt->data;

	//! Fill command type
	sntp_client->command_type = RSI_SNTP_GETTIME;

	//! Using host descriptor to set payload length
	send_size = sizeof(rsi_sntp_client_t);

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send SNTP Get request command
	status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SNTP_CLIENT, pkt);

	//! free the packet
	rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&wlan_cb->wlan_mutex);

	//! Return the status
	return status;

}



/*==============================================*/
/**
 * @fn                int32_t rsi_sntp_client_gettime_date(void)
 * @brief             This function is used to get SNTP client time and date  
 * @param[in]   ,     length, Reponse buffer length
 * @param[in]   ,     sntp_time_date_rsp, Pointer to the SNTP Reponse buffer 
 * @param[out]  ,     None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to get the time date from SNTP server
 *
 *
 */

int32_t rsi_sntp_client_gettime_date(uint16_t length, uint8_t *sntp_time_date_rsp)
{
	rsi_sntp_client_t         *sntp_client;
	rsi_pkt_t                 *pkt;
	int32_t                    status   = RSI_SUCCESS;
	uint16_t                   send_size = 0;
	uint8_t                   *host_desc = NULL;


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

  //! attach the buffer given by user
  wlan_cb->app_buffer = (uint8_t *)sntp_time_date_rsp;

  //! length of the buffer provided by user 
  wlan_cb->app_buffer_length = length; 

	//! memset the packet data
	memset(&pkt->data, 0, sizeof(rsi_sntp_client_t));
  
  sntp_client = (rsi_sntp_client_t *)  pkt->data;

	//! Fill command type
	sntp_client->command_type = RSI_SNTP_GETTIME_DATE;

	//! Using host descriptor to set payload length
	send_size = sizeof(rsi_sntp_client_t);

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send SNTP Get request command
	status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SNTP_CLIENT, pkt);

	//! free the packet
	rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&wlan_cb->wlan_mutex);

	//! Return the status
	return status;

}


/*==============================================*/
/**
 * @fn                int32_t rsi_sntp_client_server_info(void)
 * @brief             This function is used to get SNTP client server information
 * @param[in]   ,     length, Reponse buffer length
 * @param[in]   ,     sntp_server_response, Pointer to the SNTP Reponse buffer 
 * @param[out]  ,     None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to create SNTP client
 *
 *
 */

int32_t rsi_sntp_client_server_info(uint16_t length, uint8_t *sntp_server_response)
{

	rsi_sntp_client_t         *sntp_client;
	rsi_pkt_t                 *pkt;
	int32_t                    status   = RSI_SUCCESS;
	uint16_t                   send_size = 0;
	uint8_t                   *host_desc = NULL;


	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! if state is not in card ready received state
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

  //! attach the buffer given by user
  wlan_cb->app_buffer = (uint8_t *)sntp_server_response;

  //! length of the buffer provided by user 
  wlan_cb->app_buffer_length = length; 

	//! memset the packet data
	memset(&pkt->data, 0, sizeof(rsi_sntp_client_t));
	
  sntp_client = (rsi_sntp_client_t *)  pkt->data;

	//! Fill command type
	sntp_client->command_type = RSI_SNTP_GET_SERVER_INFO;

	//! Using host descriptor to set payload length
	send_size = sizeof(rsi_sntp_client_t);
	
  //! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send SNTP  Get request command
	status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SNTP_CLIENT, pkt);

	//! free the packet
	rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&wlan_cb->wlan_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn                int32_t rsi_sntp_client_delete_async(void)
 * @brief             This function is  to delete the sntp client 
 * @param[in]   ,     None
 * @param[out]  ,     None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to create SNTP client
 *
 *
 */

int32_t rsi_sntp_client_delete_async(void)
{
	rsi_sntp_client_t     *sntp_client;
	rsi_pkt_t                 *pkt;
	int32_t                    status   = RSI_SUCCESS;
	uint16_t                   send_size = 0;
	uint8_t                   *host_desc = NULL;


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

	//! memset the packet data
	memset(&pkt->data, 0, sizeof(rsi_sntp_client_t));
	
  sntp_client = (rsi_sntp_client_t *)  pkt->data;

	//! Fill command type
	sntp_client->command_type = RSI_SNTP_DELETE;
	
  //! Using host descriptor to set payload length
	send_size = sizeof(rsi_sntp_client_t);
	
  //! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! Magic number to set asyncronous response
	wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

	//! send SNTP Get request command
	status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SNTP_CLIENT, pkt);

	//! free the packet
	rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&wlan_cb->wlan_mutex);

	//! Return the status
	return status;
}
