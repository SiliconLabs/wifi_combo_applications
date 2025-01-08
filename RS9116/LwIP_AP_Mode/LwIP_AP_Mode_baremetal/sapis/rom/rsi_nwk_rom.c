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
 #ifndef ROM_WIRELESS
#include "rsi_driver.h"

/*==============================================*/
/**
 * @fn         int32_t ROM_WL_rsi_driver_send_data(global_cb_t *global_cb_p, uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr
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

int32_t ROM_WL_rsi_driver_send_data(global_cb_t *global_cb_p, uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr)
{
  int32_t  status = RSI_SUCCESS;
  uint8_t  *host_desc;
  uint32_t total_send_size = 0;
  uint16_t headerLen;
  rsi_req_socket_send_t *send;

  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;
  
  //! Allocate packet to send data
  rsi_pkt_t *pkt = ROM_WL_rsi_pkt_alloc(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool);

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
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, send->ip_version, 4);    

    //! Fill destination IPv4 address
    memcpy((uint8_t *)send->dest_ip_addr.ipv4_address,(uint8_t *) ntohl(&(((struct rsi_sockaddr_in *)destAddr)->sin_addr.s_addr)), RSI_IPV4_ADDRESS_LENGTH);

    //! Fill destination port
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, send->dest_port, ntohs(((struct rsi_sockaddr_in *)destAddr)->sin_port));


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
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, send->ip_version, 6);    

    //! Fill destination IPv6 address

    //! Fill destination port
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, send->dest_port, ntohs(((struct rsi_sockaddr_in6 *)destAddr)->sin6_port));

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
  ROM_WL_rsi_uint16_to_2bytes(global_cb_p, send->length, length);

  //! Fill data offset
  ROM_WL_rsi_uint16_to_2bytes(global_cb_p, send->data_offset, headerLen);

  //! Total send length includes header length and payload length
  total_send_size = (headerLen + length);

  headerLen -= (sizeof(rsi_req_socket_send_t) - sizeof(send->send_buffer));

  //! Copy data to be sent
  memcpy(&send->send_buffer[headerLen], buffer, length);  

  //! Fill host descriptor
  ROM_WL_rsi_uint16_to_2bytes(global_cb_p, host_desc, (total_send_size & 0xFFF));

  //! Fill packet type
  host_desc[1] |= (RSI_WLAN_DATA_Q << 4);

  //! Enqueue packet to WLAN TX queue
  ROM_WL_rsi_enqueue_pkt(global_cb_p, &rsi_driver_cb->wlan_tx_q, pkt);

  //! Set TX packet pending event
  ROM_WL_rsi_set_event(global_cb_p, RSI_TX_EVENT);

  //! Wait on wlan semaphore
  RSI_SEMAPHORE_WAIT(&rsi_driver_cb->wlan_cb->wlan_sem, RSI_WAIT_FOREVER);    

  //! Reset expected response type
  rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_CLEAR;
  
  //! Get wlan status
  status = RSI_WLAN_GET_STATUS();

  //! Free packet
  ROM_WL_rsi_pkt_free(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  //! Return status
  return status;

}


/*==============================================*/
/**
 * @fn         int32_t ROM_WL_rsi_driver_process_recv_data(global_cb_t *global_cb_p,rsi_pkt_t *pkt)
 * @brief      Processes data receive packet
 * @param[in]   global_cb_p,pointer to the global control block
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

int32_t ROM_WL_rsi_driver_process_recv_data(global_cb_t *global_cb_p,rsi_pkt_t *pkt)
{
  uint32_t data_length, data_offset;
  int8_t  sockID;
  uint8_t websocket_info = 0;
  rsi_socket_info_t *sock_info;
  rsi_rsp_socket_recv_t *recv;
  uint8_t  *host_desc;
  uint16_t payload_length;
  int32_t frame_type=0;
  int32_t status = RSI_SUCCESS;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! Get payload pointer
  recv = (rsi_rsp_socket_recv_t *)pkt->data;
  
  //! Get host descriptor
  host_desc = pkt->desc;

  frame_type = host_desc[2];
  
  //! Get payoad length
  payload_length = (ROM_WL_rsi_bytes2R_to_uint16(global_cb_p, host_desc) & 0xFFF);

  //! check status
  status  = ROM_WL_rsi_bytes2R_to_uint16(global_cb_p, host_desc + RSI_STATUS_OFFSET);

  //! update the status in wlan_cb
  RSI_WLAN_SET_STATUS(status);


  if(frame_type == 0x01)
  {
	 rsi_driver_cb->wlan_cb->callback_list.raw_data_receive_handler(0,pkt->data,payload_length);
  }
  else
  {
  //! Get socket descriptor
  sockID = ROM_WL_rsi_get_application_socket_descriptor(global_cb_p, recv->socket_id[0]);

  //! Get web socket information
  websocket_info = recv->socket_id[1];

  if((sockID < 0) || (sockID >= NUMBER_OF_SOCKETS))
  {
    return -1;
  }

  //! Get socket info
  sock_info = &rsi_socket_pool[sockID];
  
  //! Get received data length
  data_length = ROM_WL_rsi_bytes2R_to_uint16(global_cb_p, recv->length);

  //! Get actual data offset
  data_offset = ROM_WL_rsi_bytes2R_to_uint16(global_cb_p, recv->offset);

  if(!(sock_info->sock_bitmap & RSI_SOCKET_FEAT_SYNCHRONOUS))
  {
    
    if(sock_info->sock_receive_callback != NULL)
    {
      if((sock_info->sock_bitmap & RSI_SOCKET_FEAT_WEBS_SUPPORT))
      {
	    *(uint8_t *)(recv + data_offset - 1) = websocket_info;
		
        //! Call registered callback if asynchronous callback
        sock_info->sock_receive_callback(sockID, ((uint8_t *)recv + data_offset - 1), data_length + 1);
      }
      else
      {
        //! Call registered callback if asynchronous callback
        sock_info->sock_receive_callback(sockID, ((uint8_t *)recv + data_offset), data_length);
      }
    }
    return RSI_SUCCESS;
  }

  if(global_cb_p->rx_buffer_mem_copy == 1)
  {
    //! If socket receive available length is still present
    if(sock_info->sock_recv_available_length != 0)
    {
      return -1;
    }

    //! Memcopy to socket receive buffer
    memcpy(sock_info->sock_recv_buffer, ((uint8_t *)recv + data_offset), data_length);

    //! Increase socket recieve available length
    sock_info->sock_recv_available_length = data_length;
  }
  else
  {
    if(sock_info->recv_buffer)
    {
      //! Memcopy to socket receive buffer
      memcpy(sock_info->recv_buffer, ((uint8_t *)recv + data_offset), data_length);

      sock_info->recv_buffer_length = data_length;
    }
    else
    {
      sock_info->recv_buffer_length = 0;
    }
  }

  if((sock_info->sock_type & 0xF) == SOCK_DGRAM)
  {
    //! Save destination port number
    sock_info->destination_port = ROM_WL_rsi_bytes2R_to_uint16(global_cb_p, recv->dest_port);

    //! Save destination IP address
    if((sock_info->sock_type >> 4) == AF_INET)
    {
      memcpy(sock_info->destination_ip_addr.ipv4, recv->dest_ip_addr.ipv4_address, RSI_IPV4_ADDRESS_LENGTH);
    }
    else
    {
      memcpy(sock_info->destination_ip_addr.ipv6, recv->dest_ip_addr.ipv6_address, RSI_IPV6_ADDRESS_LENGTH);
    }
  }

  //! Post semaphore for data read request
  if(rsi_driver_cb->wlan_cb->expected_response  == RSI_WLAN_RSP_SOCKET_READ_DATA)
  {
    //! Clear expected response
    rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_CLEAR;

    //! signal the wlan semaphore
    RSI_SEMAPHORE_POST(&rsi_driver_cb->wlan_cb->wlan_sem);
  }
}
  return RSI_SUCCESS;

}
#endif

