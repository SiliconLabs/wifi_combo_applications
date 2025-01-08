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

#include "rsi_pop3_client.h"


/*==============================================*/
/**
 * @fn                int32_t rsi_pop3_session_create_async(uint8_t flags, uint8_t *server_ip_address, uint16_t server_port_number, 
                                uint8_t auth_type, uint8_t *username, uint8_t *password,
                                void(*rsi_pop3_response_handler)(uint16_t status, uint8_t type, uint8_t *buffer))
 * @brief             This function is used to create an POP3 client session
 * @param[in]   ,     flags,  to enable IPv6, set BIT(0), Bydefault it selects IPv4 
 * @param[in]   ,     username, username for authentication 
 * @param[in]   ,     password, password for authentication
 * @param[in]   ,     auth_type, client authentication type: reserved
 * @param[in]   ,     server_ip_address, POP3 server address 
 * @param[in]   ,     server_port_number, POP3 server TCP port 
 * @param[out]  ,     None
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to create POP3 client session
 *
 *
 */

int32_t rsi_pop3_session_create_async(uint8_t flags, uint8_t *server_ip_address, uint16_t server_port_number, 
                                uint8_t auth_type, uint8_t *username, uint8_t *password,
                                void(*rsi_pop3_client_mail_response_handler)(uint16_t status, uint8_t type, const uint8_t *buffer))
{
  rsi_req_pop3_client_t     *pop3_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
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

  //! register callback
  if(rsi_pop3_client_mail_response_handler != NULL)
  {
    //! Register pop3 client response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_pop3_client_mail_response_handler = rsi_pop3_client_mail_response_handler;
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

  pop3_client = (rsi_req_pop3_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_pop3_client_t));

  //! Fill command type
  pop3_client->command_type = POP3_CLIENT_SESSION_CREATE;

  if(!(flags & RSI_IPV6)) 
  {
    //! Fill the IP version
    pop3_client->pop3_struct.pop3_client_session_create.ip_version = RSI_IP_VERSION_4;

    //! Set IP address to localhost
    memcpy(pop3_client->pop3_struct.pop3_client_session_create.server_ip_address.ipv4_address, server_ip_address,RSI_IPV4_ADDRESS_LENGTH);
  }
  else
  {
    pop3_client->pop3_struct.pop3_client_session_create.ip_version = RSI_IP_VERSION_6;

    //! Set IP address to localhost
    memcpy(pop3_client->pop3_struct.pop3_client_session_create.server_ip_address.ipv6_address, server_ip_address, RSI_IPV6_ADDRESS_LENGTH);
  }

  //! Fill POP3 server port number
  rsi_uint16_to_2bytes(pop3_client->pop3_struct.pop3_client_session_create.server_port_number, server_port_number);

  //! Fill authentication type
  pop3_client->pop3_struct.pop3_client_session_create.auth_type = auth_type;

  //! Fill username
  rsi_strcpy(pop3_client->pop3_struct.pop3_client_session_create.username, username);

  //! Fill password
  rsi_strcpy(pop3_client->pop3_struct.pop3_client_session_create.password, password);


  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (sizeof(rsi_req_pop3_client_t) & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
  
  //! send pop3 client session create request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_POP3_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;

}


/*==============================================*/
/**
 * @fn         int32_t rsi_pop3_session_delete(void)
 * @brief      This API deletes pop3 client session.
 * @param[in]  None
 * @param[out] None
 * @return     success if 0,else failure
 *
 *
 * @section description
 * This API deletes pop3 client session.
 *
 *
 */

int32_t rsi_pop3_session_delete(void)
{

  rsi_req_pop3_client_t     *pop3_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
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

  pop3_client = (rsi_req_pop3_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_pop3_client_t));

  //! Fill command type
  pop3_client->command_type = POP3_CLIENT_SESSION_DELETE;

    //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (sizeof(rsi_req_pop3_client_t) & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
  
  //! send pop3 client session create request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_POP3_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}




/*==============================================*/
/**
 * @fn          int32_t rsi_pop3_get_mail_stats(void)
 * @brief       This API gets the mail stats.
 * @param[in]   None 
 * @param[out]  None
 * @return      success if 0,else failure
 *
 *
 * @section description
 * This API gets the mail stats.
 *
 *
 */
int32_t rsi_pop3_get_mail_stats(void)
{

  rsi_req_pop3_client_t     *pop3_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
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

  pop3_client = (rsi_req_pop3_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_pop3_client_t));

  //! Fill command type
  pop3_client->command_type = POP3_CLIENT_GET_MAIL_STATS;

    //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (sizeof(rsi_req_pop3_client_t) & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
  
  //! send pop3 client session create request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_POP3_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}


/*==============================================*/
/**
 * @fn          int32_t rsi_pop3_get_mail_list(uint16_t mail_index)
 * @brief       This API gets the size of the mail for the passed mail index.
 * @param[in]   mail_index ,mail index to get the size of the mail
 * @param[out]  None
 * @return      success if 0,else failure
 *
 *
 * @section description
 * This API gets the size of the mail for the passed mail index.
 *
 *
 */

int32_t rsi_pop3_get_mail_list(uint16_t mail_index)
{

  rsi_req_pop3_client_t     *pop3_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
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

  pop3_client = (rsi_req_pop3_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_pop3_client_t));

  //! Fill command type
  pop3_client->command_type = POP3_CLIENT_GET_MAIL_LIST;

  //! Fill POP3 mail index number
  rsi_uint16_to_2bytes(pop3_client->pop3_struct.pop3_client_mail_index, mail_index);

  //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (sizeof(rsi_req_pop3_client_t) & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
  
  //! send pop3 client session create request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_POP3_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}



/*==============================================*/
/**
 * @fn            int32_t rsi_pop3_retrive_mail(uint16_t mail_index)
 * @brief         This API retrieves the mail content for the passed mail index
 * @param[in]     mail_index,This is the mail index to get the mail content for the passed index
 * @param[out]    None
 * @return        success if 0 ,else failure
 *
 *
 * @section description
 * This API retrieves the mail content for the passed mail index
 *
 *
 */


int32_t rsi_pop3_retrive_mail(uint16_t mail_index)
{

  rsi_req_pop3_client_t     *pop3_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
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

  pop3_client = (rsi_req_pop3_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_pop3_client_t));

  //! Fill command type
  pop3_client->command_type = POP3_CLIENT_RETR_MAIL;
  
  //! Fill POP3 mail index number
  rsi_uint16_to_2bytes(pop3_client->pop3_struct.pop3_client_mail_index, mail_index);

    //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (sizeof(rsi_req_pop3_client_t) & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
  
  //! send pop3 client session create request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_POP3_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}


/*==============================================*/
/**
 * @fn          int32_t rsi_pop3_mark_mail(uint16_t mail_index)
 * @brief       This API marks a mail as deleted for the passed mail index.
 * @param[in]   mail_index,This is the mail index to mark the mail as deleted
 * @param[out]  None
 * @return      Success if 0,else failure
 *
 *
 * @section description
 * This API marks a mail as deleted for the passed mail index.
 *
 *
 */

int32_t rsi_pop3_mark_mail(uint16_t mail_index)
{

  rsi_req_pop3_client_t     *pop3_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
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

  pop3_client = (rsi_req_pop3_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_pop3_client_t));

  //! Fill command type
  pop3_client->command_type = POP3_CLIENT_MARK_MAIL;
  
  //! Fill POP3 mail index number
  rsi_uint16_to_2bytes(pop3_client->pop3_struct.pop3_client_mail_index, mail_index);

    //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (sizeof(rsi_req_pop3_client_t) & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
  
  //! send pop3 client session create request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_POP3_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}



/*==============================================*/
/**
 * @fn         int32_t rsi_pop3_unmark_mail(void)
 * @brief      This API unmarks all the marked (deleted) mails in the current session.
 * @param[in]  None
 * @param[out] None
 * @return     Success if 0,else failure
 *
 *
 * @section description
 * This API unmarks all the marked (deleted) mails in the current session.
 *
 *
 */

int32_t rsi_pop3_unmark_mail(void)
{

  rsi_req_pop3_client_t     *pop3_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
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

  pop3_client = (rsi_req_pop3_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_pop3_client_t));

  //! Fill command type
  pop3_client->command_type = POP3_CLIENT_UNMARK_MAIL;

    //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (sizeof(rsi_req_pop3_client_t) & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
  
  //! send pop3 client session create request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_POP3_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}


/*==============================================*/
/**
 * @fn           int32_t rsi_pop3_get_server_status(void)
 * @brief        This API gets the pop3 server status.
 * @param[in]    None
 * @param[out]   None
 * @return       success if 0,else failure
 *
 *
 * @section description
 *  This API gets the pop3 server status.
 *
 *
 */

int32_t rsi_pop3_get_server_status(void)
{

  rsi_req_pop3_client_t     *pop3_client;
  rsi_pkt_t                 *pkt;
  int32_t                    status   = RSI_SUCCESS;
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

  pop3_client = (rsi_req_pop3_client_t *) pkt->data;

  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_req_pop3_client_t));

  //! Fill command type
  pop3_client->command_type = POP3_CLIENT_GET_SERVER_STATUS;

    //! get the host descriptor
  host_desc = (pkt->desc);

  //! Fill data length in the packet host descriptor
  rsi_uint16_to_2bytes(host_desc, (sizeof(rsi_req_pop3_client_t) & 0xFFF));
  
  //! Magic number to set asyncronous response
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
  
  //! send pop3 client session create request command
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_POP3_CLIENT, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status
  return status;
}
