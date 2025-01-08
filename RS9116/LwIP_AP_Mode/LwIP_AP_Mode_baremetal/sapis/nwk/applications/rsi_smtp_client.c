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

#include "rsi_smtp_client.h"
/*==============================================*/
/**
 * @fn                int32_t rsi_smtp_client_create(uint8_t flags, uint8_t *username, uint8_t *password, uint8_t *from_address, 
                                                    uint8_t *client_domain, uint8_t auth_type, uint8_t *server_ip, uint32_t port)
 * @brief             This function is used to create an SMTP client 
 * @param[in]   ,     flags,  to enable IPv6, set BIT(0), Bydefault it selects IPv4 
 * @param[in]   ,     username, username for authentication 
 * @param[in]   ,     password, password for authentication
 * @param[in]   ,     from_address, sender's address
 * @param[in]   ,     client_domain, domain name of the client
 * @param[in]   ,     auth_type, client authentication type: 1- SMTP_CLIENT_AUTH_LOGIN 3-SMTP_CLIENT_AUTH_PLAIN
 * @param[in]   ,     server_ip, SMTP server address 
 * @param[in]   ,     port, SMTP TCP port 
 * @param[out]  ,     None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to create SMTP client  
 *
 *
 */

int32_t rsi_smtp_client_create(uint8_t flags, uint8_t *username, uint8_t *password, uint8_t *from_address, 
                              uint8_t *client_domain, uint8_t auth_type, uint8_t *server_ip, uint32_t port)
{
  rsi_req_smtp_client_t     *smtp_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
  uint16_t                   smtp_length = 0;
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

  if((auth_type != RSI_SMTP_CLIENT_AUTH_LOGIN) && (auth_type !=  RSI_SMTP_CLIENT_AUTH_PLAIN))
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

  smtp_client = (rsi_req_smtp_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_smtp_client_t));

  //! Fill command type
  smtp_client->command_type = RSI_SMTP_CLIENT_CREATE;

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_req_smtp_client_t) - RSI_SMTP_BUFFER_LENGTH;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));
  
   //! send HTTP Get request command
   status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SMTP_CLIENT, pkt);
   
  if(status != RSI_SUCCESS)
  {
    //! free the packet
    rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

    //! unlock mutex
    rsi_mutex_unlock(&wlan_cb->wlan_mutex);

    //! Return the status
    return status;

  }

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_smtp_client_t));

  //! Fill command type
  smtp_client->command_type = RSI_SMTP_CLIENT_INIT;

  //! Fill the authentication type
  smtp_client->smtp_struct.smtp_client_init.auth_type = auth_type;

  if(!(flags & RSI_IPV6)) 
  {
    //! Fill the IP version
    smtp_client->smtp_struct.smtp_client_init.ip_version = RSI_IP_VERSION_4;

    //! Set IP address to localhost
    memcpy(smtp_client->smtp_struct.smtp_client_init.server_ip_address.ipv4_address, server_ip,RSI_IPV4_ADDRESS_LENGTH);
  }
  else
  {
    smtp_client->smtp_struct.smtp_client_init.ip_version = RSI_IP_VERSION_6;
    
    //! Set IP address to localhost
    memcpy(smtp_client->smtp_struct.smtp_client_init.server_ip_address.ipv6_address, server_ip, RSI_IPV6_ADDRESS_LENGTH);
  }
  
  //! Fill the server port 
  rsi_uint32_to_4bytes(smtp_client->smtp_struct.smtp_client_init.server_port, port);
 
  //! copy username
  rsi_strcpy(smtp_client->smtp_buffer, username);
  smtp_length = rsi_strlen(username) + 1;

  //! copy password
  rsi_strcpy((smtp_client->smtp_buffer) + smtp_length, password);
  smtp_length += rsi_strlen(password) + 1;

  //! copy from address
  rsi_strcpy((smtp_client->smtp_buffer) + smtp_length, from_address);
  smtp_length += rsi_strlen(from_address) + 1;

  //! copy client domain
  rsi_strcpy((smtp_client->smtp_buffer) + smtp_length, client_domain);
  smtp_length += rsi_strlen(client_domain) + 1;

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_req_smtp_client_t) - RSI_SMTP_BUFFER_LENGTH + smtp_length;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));
  
  //! send HTTP Get request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SMTP_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}



/*==============================================*/
/**
 * @fn                      int32_t rsi_smtp_client_mail_send_async(uint8_t *mail_recipient_address, uint8_t priority,
 *                                                           uint8_t *mail_subject, uint8_t *mail_body, uint16_t mail_body_length,
 *                                                           void(*smtp_client_mail_response_handler)(uint16_t status, const uint8_t cmd_type);
    
 * @brief                   This function is used to send a mail to the SMTP mail server
 * @param[in]   ,           mail_recipient_address, mail recipient address 
 * @param[in]   ,           priority, priority value at which the mail is delivered
 * @param[in]   ,           mail_subject, Subject line text
 * @param[in]   ,           mail_body, mail message
 * @param[in]   ,           mail_body_length, length of the mail body
 * @param[in]   ,           smtp_client_mail_response_handler, call back when the response to the mail sent comes 
 * @param[out]  ,           None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 *
 * @section description
 * This function is used to send a mail to the SMTP mail server
 */


int32_t rsi_smtp_client_mail_send_async(uint8_t *mail_recipient_address, uint8_t priority, uint8_t *mail_subject, uint8_t *mail_body, uint16_t mail_body_length,
                                                           void(*smtp_client_mail_response_handler)(uint16_t status, const uint8_t cmd_type))
{
  rsi_req_smtp_client_t         *smtp_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
  uint16_t                   smtp_length = 0; 
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
  
  if((priority != RSI_SMTP_MAIL_PRIORITY_LOW) && 
    (priority != RSI_SMTP_MAIL_PRIORITY_NORMAL) &&
    (priority != RSI_SMTP_MAIL_PRIORITY_HIGH)) 
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

  //! register callback
  if(smtp_client_mail_response_handler != NULL)
  {
    //! Register smtp client response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.smtp_client_mail_response_handler = smtp_client_mail_response_handler;
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
  smtp_client = (rsi_req_smtp_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_smtp_client_t));

  //! Fill command type
  smtp_client->command_type = RSI_SMTP_CLIENT_MAIL_SEND;

  smtp_client->smtp_struct.smtp_mail_send.smtp_feature = priority;
 
  //! copy smtp mail body length
  rsi_uint16_to_2bytes(smtp_client->smtp_struct.smtp_mail_send.mail_body_length, mail_body_length);

  //! copy mail recipient address
  rsi_strcpy(smtp_client->smtp_buffer, mail_recipient_address);
  smtp_length = rsi_strlen(mail_recipient_address) + 1;

  //! copy mail subject
  rsi_strcpy((smtp_client->smtp_buffer) + smtp_length, mail_subject);
  smtp_length += rsi_strlen(mail_subject) + 1;

  //! copy mail message
  memcpy((smtp_client->smtp_buffer) + smtp_length, mail_body, mail_body_length);
  smtp_length += mail_body_length;

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_req_smtp_client_t) - RSI_SMTP_BUFFER_LENGTH + smtp_length;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;
  
  //! send smtp mail send request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SMTP_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;


}


/*==============================================*/
/**
 * @fn                 int32_t rsi_smtp_client_delete_async(void(*smtp_client_delete_response_handler)(uint16_t status,const uint8_t cmd_type);
 * @brief              This function is used to delete the smtp client
 * @param[in]   ,      None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 *
 * @section description
 * This function is used to delete smtp client  
 */

int32_t rsi_smtp_client_delete_async(void(*smtp_client_delete_response_handler)(uint16_t status,const uint8_t cmd_type))
{

  rsi_req_smtp_client_t         *smtp_client;
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

  //! register callback
  if(smtp_client_delete_response_handler != NULL)
  {
    //! Register smtp client  delete response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.smtp_client_delete_response_handler = smtp_client_delete_response_handler;
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
  smtp_client = (rsi_req_smtp_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_smtp_client_t));

  //! Fill command type
  smtp_client->command_type = RSI_SMTP_CLIENT_DEINIT;

  //! Using host descriptor to set payload length
  send_size = sizeof(rsi_req_smtp_client_t) - RSI_SMTP_BUFFER_LENGTH;

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;
  
  //! send SMTP Get request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_SMTP_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}

