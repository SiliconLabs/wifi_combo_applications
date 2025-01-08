/**
 * @file     rsi_nwk.c
 * @version  0.1
 * @date     15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contians networking functionality
 *
 *  @section Description  This file contains networking functionality
 *
 *
 */
/**
 * Include files
 * */
#include "rsi_driver.h"

/*==============================================*/
/**
 * @fn         int32_t rsi_driver_send_data(uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr)
 * @brief      This function is used to send data packet
 * @param[in]  sockID, socket descriptor on which data needs to be send
 * @param[in]  buffer, pointer to data 
 * @param[in]  length, data length
 * @param[in]  destAddr, pointer to destination socket details 
 * @param[out] None
 * @return    
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This 
 *
 *
 */
int32_t rsi_driver_send_data(uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr)
{
#ifdef RSI_IPV6_ENABLE
  return rsi_driver_send_data_non_rom(global_cb_p, sockID, buffer, length, destAddr);
#endif
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_driver_send_data(global_cb_p, sockID, buffer, length, destAddr);
#else
  return api_wl->rsi_driver_send_data(global_cb_p, sockID, buffer, length, destAddr);
#endif
}

/*==============================================*/
/**
 * @fn         int32_t rsi_driver_process_recv_data(rsi_pkt_t *pkt)
 * @brief      Processes data receive packet
 * @param[in]  pkt, pointer to data packet  
 * @param[out] None
 * @return     
 *             Non zero - If fails
 *             0 - If success
 *
 * @section description
 * This function processes data receive packet  
 *
 */

int32_t rsi_driver_process_recv_data(rsi_pkt_t *pkt)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_driver_process_recv_data(global_cb_p, pkt);
#else
  return api_wl->rsi_driver_process_recv_data(global_cb_p, pkt);
#endif
}


/*====================================================*/
/**
 * @fn          uint16_t rsi_wlan_register_callbacks(rsi_nwk_callback_id_t callback_id, void (*callback_handler_ptr)(void))
 * @brief       Registers the call backs
 * @param[in]   uint32_t callback_id, call back number which needs to be registered
 * @param[in]   void (*callback_handler_ptr)(void), call back handler which needs to be registered for a given call back
 * @return      
 *                1 -  error
 *                0 - Success
 * @section description 
 * This function is used to register the call back
 */

uint16_t rsi_nwk_register_callbacks(rsi_nwk_callback_id_t callback_id, void (*callback_handler_ptr)(uint8_t command_type , uint32_t status, const uint8_t *buffer, const uint32_t length))
{

  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }
  if(callback_id == RSI_NWK_ERROR_CB)  //! check for NULL or not
  {
    //! Register join fail call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.nwk_error_call_back_handler = callback_handler_ptr;
  }
	
  return 0;
}


/*====================================================*/
/**
 * @fn          uint16_t rsi_wlan_register_callbacks(uint32_t callback_id, void (*callback_handler_ptr)((uint16_t status, const uint8_t *buffer, const uint16_t length))
 * @brief       Registers the call backs
 * @param[in]   uint32_t callback_id, call back number which needs to be registered
 * @param[in]   void (*callback_handler_ptr)(void), call back handler which needs to be registered for a given call back
 * @return      
 *                1 -  error
 *                0 - Success
 * @section description 
 * This function is used to register the call back
 */


uint16_t rsi_wlan_nwk_register_json_update_cb(uint32_t callback_id, void (*callback_handler_ptr)(uint8_t *filename, uint8_t *json_object, uint32_t length, uint32_t status))
{
  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }

  //! Register json update handler
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_json_object_update_handler = callback_handler_ptr;
  
  return 0;
}



uint16_t rsi_wlan_nwk_register_json_event_cb(uint32_t callback_id, void (*callback_handler_ptr)(uint32_t status, uint8_t *json_object, uint32_t length))
{
  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }

  //! Register json update handler
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_json_object_event_handler = callback_handler_ptr;
  
  return 0;
}

uint16_t rsi_wlan_nwk_register_webpage_req_cb(uint32_t callback_id, void (*callback_handler_ptr)(uint8_t type, uint8_t *url_name, uint8_t *post_content_buffer, uint32_t post_content_length, uint32_t status))
{
  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }

  //! Register webpage request handler
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_webpage_request_handler = callback_handler_ptr;

  return 0;
}

uint16_t rsi_wlan_nwk_register_wireless_fw_upgrade_cb(uint32_t callback_id, void (*callback_handler_ptr)(uint8_t type, uint32_t status))
{
  if(callback_id > RSI_MAX_NUM_CALLBACKS)
  {
    /*
     *Return , if the callback number exceeds the RSI_MAX_NUM_CALLBACKS ,or
     * the callback is already registered
     */
    return 1;
  }

  //! Register wireless firmware upgrade call back handler
  rsi_driver_cb->nwk_cb->nwk_callbacks.rsi_wireless_fw_upgrade_handler = callback_handler_ptr;
  
  return 0;
}

/*==============================================*/
/**
 * @fn         int32_t rsi_driver_send_data_non_rom(global_cb_t *global_cb_p, uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr
 * @brief      This function is used to send data packet
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]  sockID, socket descriptor on which data needs to be send
 * @param[in]  buffer, pointer to data 
 * @param[in]  length, data length
 * @param[in]  destAddr, pointer to destination socket details 
 * @param[out] None
 * @return    
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This 
 *
 *
 */
#ifdef RSI_IPV6_ENABLE
int32_t rsi_driver_send_data_non_rom(global_cb_t *global_cb_p, uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr)
{
  int32_t  status = RSI_SUCCESS;
  uint8_t  *host_desc;
  uint32_t total_send_size = 0;
  uint16_t headerLen;
  rsi_req_socket_send_t *send;

  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;
  
  //! Allocate packet to send data
  rsi_pkt_t *pkt = rsi_pkt_alloc(&rsi_driver_cb->wlan_cb->wlan_tx_pool);

  if(pkt == NULL)
  {
    return RSI_SOCK_ERROR;
  }

  //! Get data pointer
  send = (rsi_req_socket_send_t *)pkt->data;

  //! Get host descriptor pointer
  host_desc = pkt->desc;

  //! Memset host descriptor
  memset(host_desc, 0, RSI_HOST_DESC_LENGTH);

  //! Memset before filling
  memset(send , 0, (sizeof(rsi_req_socket_send_t) - sizeof(send->send_buffer)));

  if(((rsi_socket_pool[sockID].sock_type >> 4) & 0xF) == AF_INET)
  {
    //! Fill IP version as 4
    rsi_uint16_to_2bytes(send->ip_version, 4);    

    //! Fill destination IPv4 address
    memcpy((uint8_t *)send->dest_ip_addr.ipv4_address,(uint8_t *) ntohl(&(((struct rsi_sockaddr_in *)destAddr)->sin_addr.s_addr)), RSI_IPV4_ADDRESS_LENGTH);

    //! Fill destination port
    rsi_uint16_to_2bytes(send->dest_port, ntohs(((struct rsi_sockaddr_in *)destAddr)->sin_port));


    //! Calculate header length
    if((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_STREAM)
    {
      //! TCP IPv4 header 
      headerLen = RSI_TCP_FRAME_HEADER_LEN;
    }
    else
    {
      //! UDP IPv4 header
      headerLen = RSI_UDP_FRAME_HEADER_LEN;
    }
  }
  else
  {
    //! Fill IP version as 6
    rsi_uint16_to_2bytes(send->ip_version, 6);    

    //! Fill destination IPv6 address
     memcpy(send->dest_ip_addr.ipv6_address,(uint8_t *)ntohl(&((struct rsi_sockaddr_in6 *)destAddr)->sin6_addr._S6_un._S6_u8), RSI_IPV6_ADDRESS_LENGTH);
    
     //! Fill destination port
    rsi_uint16_to_2bytes(send->dest_port, ntohs(((struct rsi_sockaddr_in6 *)destAddr)->sin6_port));

    //! Calculate header length
    if((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_STREAM)
    {
      //! TCP IPv6 header length
      headerLen = RSI_TCP_V6_FRAME_HEADER_LEN;      
    }
    else
    {
      //! UDP IPv6 header length
      headerLen = RSI_UDP_V6_FRAME_HEADER_LEN;
    }
  }

  //! if it is a web socket, setting web socket data header length
  if(rsi_socket_pool[sockID].sock_bitmap & RSI_SOCKET_FEAT_WEBS_SUPPORT)
  {
    //! If websocket is enabled
    headerLen += ((length < 126) ? 6 : 8);
  }
  
  //! Fill socket desriptor
  send->socket_id[0] = rsi_socket_pool[sockID].sock_id;

  //! Fill opcode used in websocket
  send->socket_id[1] = rsi_socket_pool[sockID].opcode;
  
  //! Fill send buffer length
  rsi_uint16_to_2bytes(send->length, length);

  //! Fill data offset
  rsi_uint16_to_2bytes(send->data_offset, headerLen);

  //! Total send length includes header length and payload length
  total_send_size = (headerLen + length);

  headerLen -= (sizeof(rsi_req_socket_send_t) - sizeof(send->send_buffer));

  //! Copy data to be sent
  memcpy(&send->send_buffer[headerLen], buffer, length);  

  //! Fill host descriptor
  rsi_uint16_to_2bytes(host_desc, (total_send_size & 0xFFF));

  //! Fill packet type
  host_desc[1] |= (RSI_WLAN_DATA_Q << 4);

  //! Enqueue packet to WLAN TX queue
  rsi_enqueue_pkt(&rsi_driver_cb->wlan_tx_q, pkt);

  //! Set TX packet pending event
  rsi_set_event(RSI_TX_EVENT);

  //! Wait on wlan semaphore
  RSI_SEMAPHORE_WAIT(&rsi_driver_cb->wlan_cb->wlan_sem, RSI_WAIT_FOREVER);    

  //! Reset expected response type
  rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_CLEAR;
  
  //! Get wlan status
  status = RSI_WLAN_GET_STATUS();

  //! Free packet
  rsi_pkt_free(&rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  //! Return status
  return status;

}
#endif

