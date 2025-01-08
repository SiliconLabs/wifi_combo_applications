/**
 * @file     rsi_socket.c
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
 *  @brief : This file contains socket APIs
 *
 *  @section Description  This file contains socket APIs
 *
 *
 */
#include "rsi_driver.h"
#include "rsi_wlan_non_rom.h"


//! Socket information pool pointer
rsi_socket_info_t *rsi_socket_pool;
rsi_socket_info_non_rom_t *rsi_socket_pool_non_rom;
extern rsi_wlan_cb_non_rom_t wlan_cb_non_rom;
int32_t  rsi_socket_connect(global_cb_t *global_cb_p, int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength);
/*==============================================*/
/**
 * @fn          int rsi_fd_isset(uint32_t fd, struct rsi_fd_set_s *fds_p)
 * @brief       Checks for the bit set in the bitmap 
 * @param[in]   fd , socket id
 * @param[in]   fds_p , pointer to the rsi_fd_set_structure 
 * @param[out]  None
 * @return      
 *               0  - If fails
 *              >0 - If success
 */

int rsi_fd_isset(uint32_t fd, struct rsi_fd_set_s *fds_p) {
  uint32_t mask = 1 << (fd% 32);
  return fds_p->fd_array[fd / 32] & mask;
}
/*==============================================*/
/**
 * @fn          void rsi_set_fd(uint32_t fd, struct rsi_fd_set_s *fds_p)
 * @brief       Sets the bit for the file descriptor fd in the file descriptor set rsi_fd_set.  * @param[in]   fd , socket id
 * @param[in]   fd , socket id
 * @param[in]   fds_p , pointer to the rsi_fd_set_structure 
 * @param[out]  None
 */

void rsi_set_fd(uint32_t fd, struct rsi_fd_set_s *fds_p) {
  uint32_t mask = 1 << (fd % 32);
  fds_p->fd_array[fd / 32] |= mask;
}
/*==============================================*/
/**
 * @fn          void rsi_fd_clr(uint32_t fd, struct rsi_fd_set_s *fds_p) 
 * @brief       Clears the bit in the bitmap 
 * @param[in]   fd , socket id
 * @param[in]   fds_p , pointer to the rsi_fd_set_structure 
 * @param[out]  None
 */

void rsi_fd_clr(uint32_t fd, struct rsi_fd_set_s *fds_p) {
  uint32_t mask = 1 << (fd % 32);
  fds_p->fd_array[fd / 32] &= ~mask;
}
/*==============================================*/
/**
 * @fn         int32_t rsi_socket_async(int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
 * @brief      Creates socket
 * @param[in]  protocolFamily , Type of the socket family to create
 * @param[in]  type , type of the socket to create 
 * @param[in]  protocol , to enable SSL over TCP 
 * @param[in]  callback , callback function to read data ayncronously from socket 
 *
 * @param[out] None
 * @return     
 *             <0 - If fails
 *             >= - If success
 *
 * @section description
 * This function creates socket
 *
 */


int32_t  rsi_socket_async(int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
{
#ifdef RSI_IPV6_ENABLE
  return rsi_socket_async_non_rom(global_cb_p, protocolFamily, type, protocol, callback);
#endif
#ifdef ROM_WIRELESS
    return ROMAPI_WL->socket_async(global_cb_p, protocolFamily, type, protocol, callback);
#else
    return api_wl->socket_async(global_cb_p, protocolFamily, type, protocol, callback);
#endif
}
/*==============================================*/
/**
 * @fn          int32_t rsi_socket(int32_t protocolFamily, int32_t type, int32_t protocol)
 * @brief       Creates socket
 * @param[in]   protocolFamily , Type of the socket family to create
 * @param[in]   type , type of the socket to create 
 * @param[in]   protocol , to enable SSL over TCP 
 * @param[out]  None
 * @return      
 *              <0  - If fails
 *              >=0 - If success
 *
 * @section description
 * This function creates socket
 *
 */

int32_t rsi_socket(int32_t protocolFamily, int32_t type, int32_t protocol)
{
  if (protocol  & BIT(0))
  {
    if(protocol & BIT(13))
    {	
      wlan_cb_non_rom.tls_version = (RSI_SOCKET_FEAT_SSL | RSI_SSL_V_1_0);
    }
    else if (protocol & BIT(14))			
    {
      wlan_cb_non_rom.tls_version = (RSI_SOCKET_FEAT_SSL | RSI_SSL_V_1_1);
    }
    else if (protocol & BIT(15))
    {
      wlan_cb_non_rom.tls_version = (RSI_SOCKET_FEAT_SSL |RSI_SSL_V_1_2);
    }
    else
    {
      wlan_cb_non_rom.tls_version = RSI_SOCKET_FEAT_SSL;
    }
  }
#ifdef RSI_IPV6_ENABLE
    return rsi_socket_async_non_rom(global_cb_p, protocolFamily, type, protocol, NULL);
#endif
#ifdef ROM_WIRELESS
    return ROMAPI_WL->socket_async(global_cb_p, protocolFamily, type, protocol, NULL);
#else
    return api_wl->socket_async(global_cb_p, protocolFamily, type, protocol, NULL);
#endif
}

/*==============================================*/
/**
 * @fn          int32_t  rsi_bind(int32_t sockID, struct rsi_sockaddr *localAddress, int32_t addressLength)
 * @brief       Assign address to socket
 * @param[in]   sockID, socket descriptor
 * @param[in]   localAddress, address which needs to be assign 
 * @param[in]   addressLength, length of the socket address 
 * @param[out]  None
 * @return
 *              <0 - If fails
 *               0 - If success
 *
 * @section description
 * This assigns address to the socket
 *
 *
 */
int32_t  rsi_bind(int32_t sockID, struct rsi_sockaddr *localAddress, int32_t addressLength)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->bind(global_cb_p, sockID, localAddress, addressLength);
#else
  return api_wl->bind(global_cb_p, sockID, localAddress, addressLength);
#endif
}
/*==============================================*/
/**
 * @fn          int32_t  rsi_connect(int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength)
 * @brief       connect the socket to specified remoteAddress
 * @param[in]   sockID, socket descriptor 
 * @param[in]   remoteAddress, remote peer address structure 
 * @param[in]   addressLength, remote peer address structrue length 
 * @param[out]  None
 * @return
 *
 *              <0 - If fails
 *               0 - If success
 *
 * @section description
 * This function is use to connect the socket to specified remote address
 *
 */

int32_t  rsi_connect(int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->connect(global_cb_p, sockID, remoteAddress, addressLength);
#else
  return rsi_socket_connect(global_cb_p, sockID, remoteAddress, addressLength);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_listen(int32_t sockID, int32_t backlog)
 * @brief      This function is used to make socket to listen for remote connecion request in passive mode
 * @param[in]  sockID, socket descriptor 
 * @param[in]  backlog, maximum number of pending connections requests 
 * @param[out] None
 * @return
 *              <0 - If fails
 *               0 - If success
 *
 * @section description
 * This function is used to make socket to listen for remote connecion request in passive mode
 *
 */

int32_t  rsi_listen(int32_t sockID, int32_t backlog)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->listen(global_cb_p, sockID, backlog);
#else
  return api_wl->listen(global_cb_p, sockID, backlog);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_accept(int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength)
 * @brief      This function is used to accept connection request from remote peer
 * @param[in]  sockID, socket descriptor 
 * @param[in]  ClientAddress, Remote peer address 
 * @param[in]  addressLength, Remote peer address length
 * @param[out] None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to accept connection request from remote peer
 *
 */

int32_t  rsi_accept(int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength)
{
   return rsi_accept_non_rom( sockID,ClientAddress,addressLength);
}
int32_t  rsi_accept_non_rom(int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength)
{
  struct rsi_sockaddr_in  peer4_address;
  struct rsi_sockaddr_in6 peer6_address;
  int32_t status = RSI_SUCCESS;
  rsi_req_socket_accept_t *accept;
  rsi_pkt_t *pkt = NULL;
  rsi_socket_info_t *sock_info;
  int8_t accept_sock_id;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! Check socket is TCP or not
  if((rsi_socket_pool[sockID].sock_type & 0xF) != SOCK_STREAM)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EPROTOTYPE);

    return RSI_SOCK_ERROR;
  }

  //! If socket is not binded
  if(rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_LISTEN && \
      (rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_CONNECTED))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }
  if(rsi_socket_pool[sockID].ltcp_socket_type != RSI_LTCP_PRIMARY_SOCKET)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EINVAL);

    return RSI_SOCK_ERROR;
  }

  sock_info = &rsi_socket_pool[sockID];

  //! check maximum backlogs count
  if(sock_info->backlogs == sock_info->backlog_current_count)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(ENOBUFS);

    return RSI_SOCK_ERROR;
  }

  if(rsi_socket_pool[sockID].sock_state == RSI_SOCKET_STATE_CONNECTED)
  {
    //! Create new instance for socket
    accept_sock_id = rsi_socket_async((sock_info->sock_type >> 4), (sock_info->sock_type & 0xF), (sock_info->sock_bitmap), sock_info->sock_receive_callback);

    if((accept_sock_id >= 0) && (accept_sock_id < NUMBER_OF_SOCKETS))
    {

      //! Set socket as secondary socket
      rsi_socket_pool[accept_sock_id].ltcp_socket_type = RSI_LTCP_SECONDARY_SOCKET;

      //! Save local port number
      rsi_socket_pool[accept_sock_id].source_port = rsi_socket_pool[sockID].source_port;
    }
    else
    {

      return RSI_SOCK_ERROR;
    }
  }
  else
  {
    accept_sock_id = sockID;
  }

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Allocate packet
  pkt = rsi_pkt_alloc(&rsi_driver_cb->wlan_cb->wlan_tx_pool);
  if(pkt == NULL)
  {
    //! Release mutex lock
    RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

    //! Set error
    RSI_WLAN_SET_STATUS(RSI_ERROR_PKT_ALLOCATION_FAILURE);

    return RSI_SOCK_ERROR;
  }

  //! send socket accept command
  accept = (rsi_req_socket_accept_t *)pkt->data;

  //! Fill socket descriptor
  accept->socket_id = rsi_socket_pool[sockID].sock_id;

  //! Fill local port number
  rsi_uint16_to_2bytes( accept->source_port, rsi_socket_pool[sockID].source_port);

  //! Set waiting socket ID
  rsi_driver_cb->wlan_cb->waiting_socket_id = accept_sock_id;

  //! Send socket accept command
  status = RSI_DRIVER_WLAN_SEND_CMD(RSI_WLAN_REQ_SOCKET_ACCEPT, pkt);

  //! check status,fill out params with remote peer address details if success and return status  
  if(status == RSI_SUCCESS)
  {
    //! Copy remote peer ip address
    if(ClientAddress && *addressLength != 0)
    {
      //! Handle the IPv4 socket type
      if((rsi_socket_pool[accept_sock_id].sock_type >> 4) == AF_INET)
      {

        //! Update the Client address with socket family, remote host IPv4 address and port
        peer4_address.sin_family      = AF_INET;
        memcpy(&peer4_address.sin_addr.s_addr, (ntohl(rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv4)), RSI_IPV4_ADDRESS_LENGTH);
        peer4_address.sin_port        = ntohs((uint16_t)rsi_socket_pool[accept_sock_id].destination_port);

        //! Copy the peer address/port info to the ClientAddress.  
        //! Truncate if addressLength is smaller than the size of struct rsi_sockaddr_in
        if(*addressLength > sizeof(struct rsi_sockaddr_in))
        {
          *addressLength = sizeof(struct rsi_sockaddr_in);
        }
        memcpy(ClientAddress, &peer4_address, *addressLength);
      }
      else if((rsi_socket_pool[accept_sock_id].sock_type >> 4) == AF_INET6)
      {
        peer6_address.sin6_family = AF_INET6;
        peer6_address.sin6_port = ntohs((uint16_t)rsi_socket_pool[accept_sock_id].destination_port);        

        peer6_address.sin6_addr._S6_un._S6_u32[0] = (uint32_t)ntohl(&rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv6[0]);
        peer6_address.sin6_addr._S6_un._S6_u32[1] = (uint32_t)ntohl(&rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv6[4]);
        peer6_address.sin6_addr._S6_un._S6_u32[2] = (uint32_t)ntohl(&rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv6[8]);
        peer6_address.sin6_addr._S6_un._S6_u32[3] = (uint32_t)ntohl(&rsi_socket_pool[accept_sock_id].destination_ip_addr.ipv6[12]);

        if((*addressLength) > sizeof(struct rsi_sockaddr_in6))
        {
          *addressLength = sizeof(struct rsi_sockaddr_in6);
        }

        memcpy(ClientAddress, &peer6_address, *addressLength);
      }
    }

    //! Change state to connected
    rsi_socket_pool[accept_sock_id].sock_state = RSI_SOCKET_STATE_CONNECTED;

    //! Increase backlog current count
    rsi_socket_pool[sockID].backlog_current_count++;
  }

  //! Free packet
  rsi_pkt_free(&rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }

  //! Return status
  return accept_sock_id;
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_recvfrom(int32_t sockID, int8_t *buffer, int32_t buffersize, int32_t flags,struct rsi_sockaddr *fromAddr, int32_t *fromAddrLen)
 * @brief      This function is used to receive data from remote peer 
 * @param[in]  sockID, socket descriptor 
 * @param[in]  buffer, application buffer pointer to hold receive data
 * @param[in]  buffersize, requested bytes for read
 * @param[in]  flags, reserved
 * @param[in]  fromAddr, remote peer address 
 * @param[in]  fromAddrLen, remote peer address length
 * @param[out] None 
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 *
 * @section description
 * This function is used to receive data from remote peer 
 *
 */

int32_t  rsi_recvfrom(int32_t sockID, int8_t *buffer, int32_t buffersize, int32_t flags,struct rsi_sockaddr *fromAddr, int32_t *fromAddrLen)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->recvfrom(global_cb_p, sockID, buffer, buffersize, flags, fromAddr, fromAddrLen);
#else
  return api_wl->recvfrom(global_cb_p, sockID, buffer, buffersize, flags, fromAddr, fromAddrLen);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_recv(int32_t sockID, void *rcvBuffer, int32_t bufferLength, int32_t flags)
 * @brief      This function is used to receive data from remote peer 
 * @param[in]  sockID, socket descriptor 
 * @param[in]  buffer, application buffer pointer to hold receive data
 * @param[in]  buffersize, requested bytes for read
 * @param[in]  flags, reserved
 * @return
 *
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to receive data from remote peer 
 *
 */
int32_t  rsi_recv(int32_t sockID, void *rcvBuffer, int32_t bufferLength, int32_t flags)
{
  int32_t fromAddrLen = 0;
  
#ifdef ROM_WIRELESS
  return ROMAPI_WL->recvfrom(global_cb_p, sockID, rcvBuffer, bufferLength, flags, NULL, &fromAddrLen);
#else
  return api_wl->recvfrom(global_cb_p, sockID, rcvBuffer, bufferLength, flags, NULL, &fromAddrLen);
#endif
}

/*==============================================*/
/**
 * @fn          int32_t rsi_sendto(int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen)
 * @brief       This function is used to send data to specific remote peer on a given socket synchronously
 * @param[in]   sockID, socket descriptor 
 * @param[in]   msg, pointer to data which needs to be send to remote peer 
 * @param[in]   msgLength, length of data to send 
 * @param[in]   flags, reserved 
 * @param[in]   destAddr, remote peer address to send data 
 * @param[in]   destAddrLen, rmeote peer address length
 * @param[out]  None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send data to specific remote peer on a given socket
 *
 */

int32_t rsi_sendto(int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, NULL);
#else
  return api_wl->sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, NULL);
#endif
}

/*==============================================*/
/**
 * @fn          int32_t rsi_sendto_async(int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen,
                     void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
 * @brief       This function is used to send data to specific remote peer on a given socket asynchronously
 * @param[in]   sockID, socket descriptor 
 * @param[in]   msg, pointer to data which needs to be send to remote peer 
 * @param[in]   msgLength, length of data to send 
 * @param[in]   flags, reserved 
 * @param[in]   destAddr, remote peer address to send data 
 * @param[in]   destAddrLen, rmeote peer address length
 * @param[in]   data_transfer_complete_handler, pointer to the callback function which will be called after data transfer completion
 * @param[out]  None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send data to specific remote peer on a given socket
 *
 */
int32_t rsi_sendto_async(int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen,
                     void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, data_transfer_complete_handler);
#else
  return api_wl->sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, data_transfer_complete_handler);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t rsi_send(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags)
 * @brief      This function is used to send data on a given socket syncronously
 * @param[in]  sockID, socket descriptor 
 * @param[in]  msg, pointer to data which needs to be send to remote peer 
 * @param[in]  msgLength, length of data to send 
 * @param[in]  flags, reserved 
 * @param[out] None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send data on a given socket
 *
 */

int32_t rsi_send(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags)
{
#ifdef RSI_UART_INTERFACE
 wlan_cb_non_rom.rsi_uart_data_ack_check = sockID;
#endif
#ifdef ROM_WIRELESS
  return ROMAPI_WL->send_async(global_cb_p, sockID, msg, msgLength, flags, NULL);
#else
  return api_wl->send_async(global_cb_p, sockID, msg, msgLength, flags, NULL);
#endif
}


#ifndef RSI_M4_INTERFACE

/*==============================================*/
/**
 * @fn         void rsi_reset_per_socket_info(uint8_t sockID)
 * @brief      This function is used to reset all the socket related information
 * @param[in]  sockID, socket descriptor 
 * @param[out] None
 * @return
 *
 * @section description
 * This function is used to reset all the socket related information 
 *
 */

void rsi_reset_per_socket_info(uint8_t sockID)
{
  rsi_socket_info_non_rom_t *rsi_sock_info_non_rom_p = &rsi_socket_pool_non_rom[sockID];
  rsi_sock_info_non_rom_p->more_data = 0;
  rsi_sock_info_non_rom_p->offset = 0;
  rsi_sock_info_non_rom_p->buffer = 0;
  rsi_sock_info_non_rom_p->rem_len = 0;
  //! reset socket_bitmap
  wlan_cb_non_rom.socket_bitmap &= ~BIT(sockID);
  rsi_sock_info_non_rom_p->rsi_sock_data_tx_done_cb = NULL;
}

/*==============================================*/
/**
 * @fn         void rsi_chunk_data_tx_done_cb(uint8_t sockID)
 * @brief      This is a callback function which is used to send one chunk of data at a time 
 * @param[in]  sockID, socket descriptor 
 * @param[out] None
 * @return
 *
 * @section description
 * This is a callback function which is used to send one chunk of data at a time
 *
 */

void rsi_chunk_data_tx_done_cb(uint8_t sockID)
{
  int16_t status = 0;
  uint16_t total_data_sent,chunk_size = 0;
  rsi_socket_info_non_rom_t *rsi_sock_info_non_rom_p = &rsi_socket_pool_non_rom[sockID];
  if(rsi_sock_info_non_rom_p->rem_len)
  {
#ifndef RSI_M4_INTERFACE
    //! masking the socket event
    rsi_mask_event(RSI_SOCKET_EVENT);
#endif    
    rsi_sock_info_non_rom_p->more_data = 1;
    chunk_size = MIN(CHUNK_SIZE, rsi_sock_info_non_rom_p->rem_len);
    //! Sends next chunk of data and returns the total data sent in success case
    status = rsi_send_async(sockID, rsi_sock_info_non_rom_p->buffer + rsi_sock_info_non_rom_p->offset, chunk_size, rsi_sock_info_non_rom_p->flags, rsi_chunk_data_tx_done_cb);
    if(status < 0)
    {
      status = rsi_wlan_get_status();
      total_data_sent = rsi_sock_info_non_rom_p->offset;
      if(status != RSI_TX_BUFFER_FULL)
      {
        if(rsi_sock_info_non_rom_p->rsi_sock_data_tx_done_cb != NULL)
        {
          rsi_sock_info_non_rom_p->rsi_sock_data_tx_done_cb(sockID,status,total_data_sent);
        }		
        rsi_reset_per_socket_info(sockID);
      }
    }
    else
    {
      rsi_sock_info_non_rom_p->rem_len -= status;		
      rsi_sock_info_non_rom_p->offset += status;
    }
#ifndef RSI_M4_INTERFACE
    //! unmasking the socket event
    rsi_unmask_event(RSI_SOCKET_EVENT);
#endif    
  }
  else //! Incase of total data is sent
  {
    total_data_sent = rsi_sock_info_non_rom_p->offset;
    if(rsi_sock_info_non_rom_p->rsi_sock_data_tx_done_cb != NULL)
    {
      rsi_sock_info_non_rom_p->rsi_sock_data_tx_done_cb(sockID,RSI_SUCCESS,total_data_sent);
    }
    rsi_reset_per_socket_info(sockID);
    if(wlan_cb_non_rom.socket_bitmap == 0)
    {
#ifndef RSI_M4_INTERFACE
      //! Clearing the socket event
      rsi_clear_event(RSI_SOCKET_EVENT);
#endif      

    }
  }
}

/*==============================================*/
/**
 * @fn         int32_t rsi_send_large_data_async(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags, 
 void (*rsi_sock_data_tx_done_cb)(uint8_t sockID, int16_t status,uint16_t total_data_sent)
 * @brief      This function is used to send large data on a given socket asynchronously
 * @param[in]  sockID, socket descriptor 
 * @param[in]  msg, pointer to data which needs to be send to remote peer 
 * @param[in]  msgLength, length of data to send 
 * @param[in]  flags, reserved 
 * @param[in]  rsi_sock_data_tx_done_cb, pointer to the callback function which will be called after one chunk of data transfer completion
 * @param[out] status
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send large data on a given socket
 *
 */

int32_t rsi_send_large_data_async(int32_t sockID,const int8_t *msg,int32_t msgLength, int32_t flags,void (*rsi_sock_data_tx_done_cb)(uint8_t sockID,int16_t status,uint16_t total_data_sent))
{
  int16_t status = 0;
  uint16_t  data_sent = 0 ,total_data_sent = 0, chunk_size = 0;	
  rsi_socket_info_non_rom_t *rsi_sock_info_non_rom_p = &rsi_socket_pool_non_rom[sockID];

  //! check if the socket is already doing data transfer, if so return busy	
  if((wlan_cb_non_rom.socket_bitmap & BIT(sockID)))
  {
    return -1; 
  }

  chunk_size = MIN(CHUNK_SIZE, msgLength);
  rsi_sock_info_non_rom_p->rsi_sock_data_tx_done_cb = rsi_sock_data_tx_done_cb;
  rsi_sock_info_non_rom_p->buffer = (int8_t *) msg;
  rsi_sock_info_non_rom_p->flags = flags;
  wlan_cb_non_rom.socket_bitmap |= BIT(sockID);
  //! returns total num of bytes sent in success case
  status = rsi_send_async(sockID, rsi_sock_info_non_rom_p->buffer, chunk_size, flags, rsi_chunk_data_tx_done_cb);
  //! If chunk of data is not sent no need to update the values
  if(status < 0)	
  {
    status = rsi_wlan_get_status();
    //! Check if not buffer full then reset all information and return 
    if(status != RSI_TX_BUFFER_FULL)
    {
      rsi_reset_per_socket_info(sockID);
      return status;	
    }
  }
  else  //! Incase of BUFFER FULL condition no need to update the values
  {	
    rsi_sock_info_non_rom_p->rem_len = msgLength - status;
    rsi_sock_info_non_rom_p->offset = status;
    //! Setting the socket event
    rsi_set_event(RSI_SOCKET_EVENT);
  }

  return RSI_SUCCESS;
}

/*==============================================*/
/**
 * @fn          uint32_t rsi_find_socket_data_pending(uint32_t event_map)
 * @brief       Returns the SockID from the event map
 * @param[in]   uint32_t event_map, event map 
 * @param[out]  None,
 * @return
 *
 *
 * @section description
 * This function is used to find on which socket the data is pending,
 * according to the event which is set from the map
 *
 *
 */

uint32_t rsi_find_socket_data_pending(uint32_t event_map)
{
  uint8_t i;
  for(i=0 ; i < RSI_NUMBER_OF_SOCKETS ; i++)
  {
    if(event_map & BIT(i))
    {
      break;
    }
	}
  return i;  
}

/*==============================================*/
/**
 * @fn          void rsi_socket_event_handler(void)
 * @brief       Handles Socket event
 * @param[in]   None, 
 * @param[out]  None,
 * @return
 *
 *
 * @section description
 * This funciton retrieves the packet from protocol TX data pending queue
 * and forwards to the module.
 *
 *
 */

void rsi_socket_event_handler(void)
{
	int32_t sockID = 0;
	
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;
	
	if(wlan_cb_non_rom.socket_bitmap)
	{
		sockID = rsi_find_socket_data_pending(wlan_cb_non_rom.socket_bitmap);
	
		rsi_chunk_data_tx_done_cb(sockID);
	}
  else
	{
		rsi_clear_event(RSI_SOCKET_EVENT);
	}  	
}

#endif
/*==============================================*/
/**
 * @fn         int32_t rsi_send_async(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags, 
                   void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
 * @brief      This function is used to send data on a given socket asynchronously
 * @param[in]  sockID, socket descriptor 
 * @param[in]  msg, pointer to data which needs to be send to remote peer 
 * @param[in]  msgLength, length of data to send 
 * @param[in]  flags, reserved 
 * @param[in]  data_transfer_complete_handler, pointer to the callback function which will be called after data transfer completion
 * @param[out] None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to send data on a given socket
 *
 */
int32_t rsi_send_async(int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags, 
    void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->send_async(global_cb_p, sockID, msg, msgLength, flags, data_transfer_complete_handler);
#else
  return api_wl->send_async(global_cb_p, sockID, msg, msgLength, flags, data_transfer_complete_handler);
#endif
}

/*==============================================*/
/**
 * @fn          int32_t  rsi_shutdown(int32_t sockID, int32_t how)
 * @brief       This function is used to close the socket
 * @param[in]   sockID, socket descriptor 
 * @param[in]   how, to select type of socket close 
 * @param[out]  None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to close the socket
 *
 */
int32_t  rsi_shutdown(int32_t sockID, int32_t how)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->shutdown(global_cb_p, sockID, how);
#else
  return api_wl->shutdown(global_cb_p, sockID, how);
#endif
}


/*==============================================*/
/**
 * @fn          int32_t rsi_check_state(int32_t type)
 * @brief       Checks wlan state
 * @param[in]   type, socket family ttype 
 * @param[out]  None
 * @return
 *              1 - If not in IP config state
 *              0 - If in IP cofig state
 *
 * @section description
 * This function is used to check the wlan state
 *
 */
int32_t rsi_check_state(int32_t type)
{
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;

  if(rsi_driver_cb->wlan_cb->opermode == RSI_WLAN_ACCESS_POINT_MODE)
  {
    if(RSI_CHECK_WLAN_STATE() != RSI_WLAN_STATE_CONNECTED)
    {
      return 1;
    }
  }
  else
  {
    if(type == AF_INET)
    {
      if(RSI_CHECK_WLAN_STATE() != RSI_WLAN_STATE_IP_CONFIG_DONE)
      {
        return 1;
      }
    }
    else
    {
      if(RSI_CHECK_WLAN_STATE() != RSI_WLAN_STATE_IPV6_CONFIG_DONE)
      {
        return 1;
      }
    }
  }
  return 0;
}


/*==============================================*/
/**
 * @fn          int32_t rsi_get_application_socket_descriptor(int32_t sock_id)
 * @brief       Gets application socket descriptor from module socket descriptor
 * @param[in]   sock_id, module's socket descriptor
 * @param[out]  None
 * @return
 *             <0 - If index is not found
 *             >0 - application index
 *
 * @section description
 * This function is used to get the application socket descriptor from module socket descriptor
 *
 */
int32_t rsi_get_application_socket_descriptor(int32_t sock_id)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_get_application_socket_descriptor(global_cb_p, sock_id);
#else
  return api_wl->rsi_get_application_socket_descriptor(global_cb_p, sock_id);
#endif
}

/*==============================================*/
/**
 * @fn          void rsi_clear_sockets(uint8_t sockID)
 * @brief       function to clear socket information
 * @param[in]   sockID, socket descriptor
 * @param[out]  None
 * @return      void
 *
 * @section description
 * This function is used to clear socket information
 *
 */
void rsi_clear_sockets(uint8_t sockID)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_clear_sockets(global_cb_p, sockID);
#else
  api_wl->rsi_clear_sockets(global_cb_p, sockID);
#endif
}



/*==============================================*/
/**
 * @fn           int rsi_setsockopt(int sockID, int level, int option_name,const void *option_value, socklen_t option_len)
 * @brief        This function is used to set the socket options 
 * @param[in]    sockID, socket descriptor 
 * @param[in]    level,
 * @param[in]    option_name,
 * @param[in]    option_value,
 * @param[in]    option_len, 
 * @return        
 *
 *
 * @section description
 * This API is used to set the socket options
 *
 *
 */

int rsi_setsockopt(int sockID, int level, int option_name,const void *option_value, rsi_socklen_t option_len)
{

  struct rsi_timeval *timeout = NULL;

  uint16_t timeout_val;


  //! If sockID is not in available range
  if(sockID < 0 || sockID >= RSI_NUMBER_OF_SOCKETS)
  {
    //! Set error
    rsi_wlan_set_status(EBADF);

    return RSI_SOCK_ERROR;
  }


  //! Configure per socket maximum tcp retries count
  if((option_name == SO_MAXRETRY ))
  {
    rsi_socket_pool_non_rom[sockID].max_tcp_retries = *(uint8_t *)option_value;
    return RSI_SUCCESS;
  }

  //! Configure tcp keep alive time
  if((option_name == SO_TCP_KEEP_ALIVE ))
  {
    rsi_uint16_to_2bytes(rsi_socket_pool_non_rom[sockID].tcp_keepalive_initial_time,*(uint16_t *)option_value);
    return RSI_SUCCESS;
  }

  //! Configure ssl socket bit map
  if((option_name == SO_SSL_ENABLE ))
  {

    rsi_socket_pool_non_rom[sockID].ssl_bitmap =  *(uint8_t *)option_value;
    return RSI_SUCCESS;
  }

  //! Configure high performance socket
  if((option_name == SO_HIGH_PERFORMANCE_SOCKET ))
  {
    rsi_socket_pool_non_rom[sockID].high_performance_socket =  *(uint8_t *)option_value;
    return RSI_SUCCESS;
  }

  if((option_name == SO_SOCK_VAP_ID ))
  {
    rsi_socket_pool_non_rom[sockID].vap_id =  *(uint8_t *)option_value;
    return RSI_SUCCESS;
  }
#ifndef RSI_M4_INTERFACE
  if((option_name == SO_RCVBUF ))
  {
    rsi_socket_pool[sockID].recv_buffer = (uint8_t *)option_value;
    rsi_socket_pool[sockID].recv_buffer_length = option_len;
    return RSI_SUCCESS;
  }
#endif
  if((option_name != SO_RCVTIMEO ) || (level != SOL_SOCKET))
  {
    rsi_wlan_set_status(EINVAL);

    return RSI_SOCK_ERROR;
  }  
  //! If socket is in not created state
  if(rsi_socket_pool[sockID].sock_state == RSI_SOCKET_STATE_INIT) 
  {
    //! Set error
    rsi_wlan_set_status(EBADF);

    return RSI_SOCK_ERROR;
  }

  timeout = (struct rsi_timeval *)option_value;

  timeout_val = (timeout->tv_usec/1000) + (timeout->tv_sec * 1000);

  //! This feature is available only if Synchrounous bitmap is set
  if(rsi_socket_pool[sockID].sock_bitmap & RSI_SOCKET_FEAT_SYNCHRONOUS)
  {
    rsi_socket_pool[sockID].read_time_out = timeout_val;
  }
  else
  {
    //! Set error
    rsi_wlan_set_status(ENOPROTOOPT);

    return RSI_SOCK_ERROR;
  }
  return RSI_SUCCESS;
}

/*==============================================*/
/**
 * @fn          int32_t  rsi_select(int32_t nfds, rsi_fd_set *readfds, rsi_fd_set *writefds, rsi_fd_set *exceptfds, struct rsi_timeval *timeout,void (*callback)(uint32_t fd_read) 
 * @brief
 * @param[in]   int32_t nfds
 * @param[in]   rsi_fd_set *readfds
 * @param[in]   rsi_fd_set *writefds
 * @param[in]   rsi_fd_set *exceptfds
 * @param[out]
 * @return
 *              0 = time out 
 *             >0 = success
 *             <0 = fail
 *
 */

int32_t  rsi_select(int32_t nfds,  rsi_fd_set *readfds,rsi_fd_set *writefds, rsi_fd_set *exceptfds, struct rsi_timeval *timeout,void (*callback)(uint32_t fd_read))
{
  int32_t status = RSI_SUCCESS;
  rsi_req_socket_select_t *select;
  rsi_pkt_t *pkt = NULL;

  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  uint32_t select_bitmap =0;
  int8_t socket_desc=0;
  int8_t index = 0;

  //! If nfds  is  not in available range
  if(nfds < 0 || nfds >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    rsi_wlan_set_status(EINVAL);

    return RSI_SOCK_ERROR;
  }

  // throw error for socket state
  for(index=0;index <= nfds; index++)
  {
    if(((readfds->fd_array[0]) & BIT(index)) && ( rsi_socket_pool[index].sock_state < RSI_SOCKET_STATE_CONNECTED)) 
    {
      //! Set error
      rsi_wlan_set_status(EBADF);

      return RSI_SOCK_ERROR;
    }

  }

  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Allocate packet
  pkt = rsi_pkt_alloc(&rsi_driver_cb->wlan_cb->wlan_tx_pool);
  if(pkt == NULL)
  {
    //! Release mutex lock
    RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

    //! Set error
    RSI_WLAN_SET_STATUS(RSI_ERROR_PKT_ALLOCATION_FAILURE);

    return RSI_SOCK_ERROR;
  }

  //! Memset before filling
  select = (rsi_req_socket_select_t *)pkt->data;
  memset(select, 0, sizeof(rsi_req_socket_select_t));

  //!updating the bitmap with the sock ids
  for(index=0; index<nfds; index++)
  {
    if((readfds->fd_array[0]) & BIT(index))
    {
      socket_desc = rsi_socket_pool[index].sock_id;
      select_bitmap |= BIT(socket_desc); 
    }

  }

  //saving the address to a pointer variable to access
  wlan_cb_non_rom.fd_set_ptr = (uint32_t *)readfds;

  //! filling the select req struct 
  select-> num_fd = nfds;
  select-> rsi_read_fds.fd_array[0] = select_bitmap;

  if(timeout != NULL)
  {
    select->rsi_select_timeval.tv_sec = timeout->tv_sec;
    select->rsi_select_timeval.tv_usec = timeout->tv_usec;
  }
  if(callback)
  {
    //! Set socket asynchronous receive callback
    wlan_cb_non_rom.sock_select_callback = callback;
    rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS; 
  }
  else
  {
  rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_SELECT_REQUEST; 
  }
  status = RSI_DRIVER_WLAN_SEND_CMD(RSI_WLAN_REQ_SELECT_REQUEST, pkt);

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Free packet
  rsi_pkt_free(&rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  // returning value of count
  return readfds->fd_count ;

}



uint8_t calculate_buffers_required(uint8_t type, uint16_t length)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->calculate_buffers_required(global_cb_p, type, length);
#else
  return api_wl->calculate_buffers_required(global_cb_p, type, length);
#endif
}
/*==============================================*/
/**
 * @fn         int32_t  rsi_accept_async(int32_t sockID, void (*callback)(int32_t sock_id, int16_t dest_port,int8_t *ip_addr, int16_t ip_version))
 * @brief      This function is used to accept_async connection request from remote peer  
 * @param[in]  sockID, socket descriptor 
 * @param[in]  callback , callback function to indicate the socket parameters connected 
 * @param[out] None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to accept_async connection request from remote peer
 *
 */


int32_t  rsi_accept_async(int32_t sockID, void (*callback)(int32_t sock_id, int16_t dest_port, uint8_t *ip_addr, int16_t ip_version))
{
  int32_t status = RSI_SUCCESS;
  rsi_req_socket_accept_t *accept;
  rsi_pkt_t *pkt = NULL;
  rsi_socket_info_t *sock_info;
  int8_t accept_sock_id;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! Check socket is TCP or not
  if((rsi_socket_pool[sockID].sock_type & 0xF) != SOCK_STREAM)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EPROTOTYPE);

    return RSI_SOCK_ERROR;
  }

  //! If socket is not binded
  if(rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_LISTEN && \
      (rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_CONNECTED))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }
  if(rsi_socket_pool[sockID].ltcp_socket_type != RSI_LTCP_PRIMARY_SOCKET)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EINVAL);

    return RSI_SOCK_ERROR;
  }

  sock_info = &rsi_socket_pool[sockID];

  //! check maximum backlogs count
  if(sock_info->backlogs == sock_info->backlog_current_count)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(ENOBUFS);

    return RSI_SOCK_ERROR;
  }

  if(rsi_socket_pool[sockID].sock_state == RSI_SOCKET_STATE_CONNECTED)
  {
    //! Create new instance for socket
    accept_sock_id = rsi_socket_async((sock_info->sock_type >> 4), (sock_info->sock_type & 0xF), (sock_info->sock_bitmap & RSI_SOCKET_FEAT_SSL), sock_info->sock_receive_callback);

    if((accept_sock_id >= 0) && (accept_sock_id < NUMBER_OF_SOCKETS))
    {

      //! Set socket as secondary socket
      rsi_socket_pool[accept_sock_id].ltcp_socket_type = RSI_LTCP_SECONDARY_SOCKET;

      //! Save local port number
      rsi_socket_pool[accept_sock_id].source_port = rsi_socket_pool[sockID].source_port;
    }
    else
    {

      return RSI_SOCK_ERROR;
    }
  }
  else
  {
    accept_sock_id = sockID;
  }

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Allocate packet
  pkt = rsi_pkt_alloc(&rsi_driver_cb->wlan_cb->wlan_tx_pool);
  if(pkt == NULL)
  {
    //! Release mutex lock
    RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

    //! Set error
    RSI_WLAN_SET_STATUS(RSI_ERROR_PKT_ALLOCATION_FAILURE);

    return RSI_SOCK_ERROR;
  }

    //! Set socket connection indication callback
	rsi_socket_pool_non_rom[accept_sock_id].accept_call_back_handler = callback;
  
	//! send socket accept command
  accept = (rsi_req_socket_accept_t *)pkt->data;

  //! Fill socket descriptor
  accept->socket_id = rsi_socket_pool[sockID].sock_id;

  //! Fill local port number
  rsi_uint16_to_2bytes(accept->source_port, rsi_socket_pool[sockID].source_port);

  //! Set waiting socket ID
  rsi_driver_cb->wlan_cb->waiting_socket_id = accept_sock_id;

  rsi_driver_cb->wlan_cb->expected_response =RSI_WLAN_RSP_ASYNCHRONOUS; 
  //! Send socket accept command
  status = RSI_DRIVER_WLAN_SEND_CMD(RSI_WLAN_REQ_SOCKET_ACCEPT, pkt);

  //! Free packet
  rsi_pkt_free(&rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }

  //! Return status
  return accept_sock_id;

}

/*==============================================*/
/**
 * @fn          int32_t  rsi_get_app_socket_descriptor(uint8_t *src_port)
 * @brief      Gets application socket descriptor from available socket pool
 * @param[in]  src_port, pointer to the socket source port
 * @param[out]  None
 * @return
 *             <0 - If index is not found
 *             >0 - application index
 *
 * @section description
 * This function is used to get the application socket descriptor from available socket pool
 *
 */
int32_t rsi_get_app_socket_descriptor(uint8_t *src_port)
{
  int i;
	uint16_t source_port;
	
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;
	
	source_port = rsi_bytes2R_to_uint16(src_port);

  for(i = 0; i < NUMBER_OF_SOCKETS; i++)
  {
    if((rsi_socket_pool[i].source_port == source_port) && (rsi_socket_pool[i].sock_state != RSI_SOCKET_STATE_CONNECTED))
    {
      break;
    }
  }

  if(i >= NUMBER_OF_SOCKETS)
  {
    return -1;
  }

  return i;
}

/*==============================================*/
/**
 * @fn          int32_t rsi_get_primary_socket_id(uint8_t *port_number)
 * @brief       function to get the primary socket ID from port number
 * @param[in]   port_number, pointer to port number
 * @param[out]  None
 * @return 
 *              <0  - If socket is not found
 *              >=0 - socket descriptor
 *
 * @section description
 * This function is used to get the primary socket ID from port number
 *
 */
int32_t rsi_get_primary_socket_id(uint8_t *port_num)
{
	int i;
	uint16_t port_number; 
	
	rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

	port_number = rsi_bytes2R_to_uint16(port_num);

	for(i = 0; i < NUMBER_OF_SOCKETS; i++)
	{
		if((rsi_socket_pool[i].source_port == port_number) && (rsi_socket_pool[i].ltcp_socket_type == RSI_LTCP_PRIMARY_SOCKET))
		{
			break;
		}
	}

	if(i >= NUMBER_OF_SOCKETS)
	{
		return -1;
	}
	return i;
}
#ifndef RSI_M4_INTERFACE
/*==============================================*/
/**
 * @fn         int32_t rsi_certificate_valid(uint16_t valid, uint16_t socket_id)
 * @brief      This function is used to send the certificate valid status to the module  
 * @param[in]  valid, This field indicates whether the server certificate is valid or not
 *											1 - indicates that the server certificate is valid
 *											0 - indicates that the server certificate is not valid 
 * @param[in]  socket_id , Socket identifier 
 * @param[out] None
 * @return     
 *			   <0 - If fails
 *             0  - If success
 * @section description
 * After validating the server certificate received from module, host can indicate the status to module using this API 
 */

int32_t rsi_certificate_valid(uint16_t valid, uint16_t socket_id)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t *pkt = NULL;
	rsi_req_cert_valid_t *cert_valid;

		//! Acquire mutex lock
	RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

	//! Allocate packet
	pkt = rsi_pkt_alloc(&rsi_driver_cb->wlan_cb->wlan_tx_pool);
	if(pkt == NULL)
	{
		//! Release mutex lock
		RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

		//! Set error
		RSI_WLAN_SET_STATUS(RSI_ERROR_PKT_ALLOCATION_FAILURE);

		return RSI_SOCK_ERROR;
	}

	cert_valid = (rsi_req_cert_valid_t *)pkt->data;

	cert_valid->socket_id = socket_id;
	cert_valid->status = valid;

	rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS; 

	//! Send cert valid command
	status = RSI_DRIVER_WLAN_SEND_CMD(RSI_WLAN_REQ_CERT_VALID, pkt);

	//! Release mutex lock
	RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);
	
	//! Free packet
	rsi_pkt_free(&rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  return status;
}


/*==============================================*/
/**
 * @fn          int32_t  rsi_socket_create_async(global_cb_t *global_cb_p, int32_t sockID, int32_t type, int32_t backlog)
 * @brief       Prepares and sends socket create to module
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   sockID, socket descriptor 
 * @param[in]   type, type of socket to create
 * @param[in]   backlog, number of backlogs for LTCP socket
 * @param[out]  None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This API is used to send socket create request to the module and receives the response asynchronously
 *
 */

int32_t rsi_socket_create_async(global_cb_t *global_cb_p, int32_t sockID, int32_t type, int32_t backlog)
{
  rsi_pkt_t *pkt = NULL;
  rsi_req_socket_t *socket_create;
  int32_t status = 0;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! Socket Parameters
  pkt = rsi_pkt_alloc(&rsi_driver_cb->wlan_cb->wlan_tx_pool);

  if(pkt == NULL)
  {
    return RSI_SOCK_ERROR;
  }

  //! Get data pointer
  socket_create = (rsi_req_socket_t *)pkt->data;

  //! Memset before filling
  memset(socket_create, 0, sizeof(rsi_req_socket_t));

  //! Fill IP verison and destination IP address
  if((rsi_socket_pool[sockID].sock_type >> 4) == AF_INET)
  {
    rsi_uint16_to_2bytes(socket_create->ip_version, 4);
    memcpy(socket_create->dest_ip_addr.ipv4_address, rsi_socket_pool[sockID].destination_ip_addr.ipv4, RSI_IPV4_ADDRESS_LENGTH);
  }
  else
  {
    rsi_uint16_to_2bytes(socket_create->ip_version, 6);
    memcpy(socket_create->dest_ip_addr.ipv6_address, rsi_socket_pool[sockID].destination_ip_addr.ipv6, RSI_IPV6_ADDRESS_LENGTH);
  }

  //! Fill local port
  rsi_uint16_to_2bytes(socket_create->module_socket, rsi_socket_pool[sockID].source_port);

  //! Fill destination port
  rsi_uint16_to_2bytes(socket_create->dest_socket, rsi_socket_pool[sockID].destination_port); 

  //! Fill socket type
  rsi_uint16_to_2bytes(socket_create->socket_type, type); 

  //! Set backlogs
  if(type == RSI_SOCKET_TCP_SERVER)
  {
    rsi_uint16_to_2bytes(socket_create->max_count, backlog);   
    socket_create->socket_bitmap |= RSI_SOCKET_FEAT_LTCP_ACCEPT;
  }
  else
  {
    rsi_uint16_to_2bytes(socket_create->max_count, 0);   
  }

  //! Check for SSL feature and fill it in ssl bitmap
  if(rsi_socket_pool[sockID].sock_bitmap & RSI_SOCKET_FEAT_SSL)
  {
    socket_create->ssl_bitmap |= RSI_SOCKET_FEAT_SSL;
  }

  //! Check for SYNCHRONOUS feature and fill it in socket bitmap
  if(rsi_socket_pool[sockID].sock_bitmap & RSI_SOCKET_FEAT_SYNCHRONOUS)
  {
    socket_create->socket_bitmap |= BIT(0);
  }

  //! Check TCP ACK indication bit
  if(rsi_socket_pool[sockID].sock_bitmap & RSI_SOCKET_FEAT_TCP_ACK_INDICATION)
  {
    socket_create->socket_bitmap |= RSI_SOCKET_FEAT_TCP_ACK_INDICATION;
  }

  socket_create->socket_bitmap |= RSI_SOCKET_FEAT_TCP_RX_WINDOW;

  //! Set the RX window size
  socket_create->rx_window_size = rsi_socket_pool[sockID].rx_window_buffers;
  
  //! Get certificate index
  socket_create->socket_cert_inx = rsi_socket_pool[sockID].socket_cert_inx;
  
//! Fill Initial timeout for Socket
  rsi_uint16_to_2bytes(socket_create->tcp_keepalive_initial_time, RSI_SOCKET_KEEPALIVE_TIMEOUT);
 
  //! Fill VAP_ID 
  socket_create->vap_id = RSI_VAP_ID;

  //! Set waiting socket ID
  rsi_driver_cb->wlan_cb->waiting_socket_id = sockID;
      
	rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;

  //! Send socket create command
  status = RSI_DRIVER_WLAN_SEND_CMD( RSI_WLAN_REQ_SOCKET_CREATE, pkt);

  //! Free packet
  rsi_pkt_free( &rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  return status;

}

/*==============================================*/
/**
 * @fn          int32_t rsi_get_socket_descriptor(uint8_t *src_port, uint8_t *dst_port, uint8_t *ip_addr, uint16_t ip_version)
 * @brief       function to get the primary socket ID from port number
 * @param[in]   src_port, pointer to source port number
 * @param[in]   dst_port, pointer to destination port number
 * @param[in]   ip_addr, pointer to destinaion ip address
 * @param[in]   ip_version, ip version either IPV4/IPV6
 * @param[out]  None
 * @return 
 *              <0  - If socket is not found
 *              >=0 - socket descriptor
 *
 * @section description
 * This function is used to get the primary socket ID based on source port number, destination port number, destination ip address and the ip version.
 *
 */

int32_t rsi_get_socket_descriptor(uint8_t *src_port, uint8_t *dst_port, uint8_t *ip_addr, uint16_t ip_version)
{
  int i;
	uint16_t source_port;
	uint16_t destination_port;
	
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;
	
	source_port = rsi_bytes2R_to_uint16(src_port);
	destination_port = rsi_bytes2R_to_uint16(dst_port);
					

  for(i = 0; i < NUMBER_OF_SOCKETS; i++)
	{
		//! Compare source port and destination port
		if((rsi_socket_pool[i].source_port == source_port) && (rsi_socket_pool[i].destination_port == destination_port))
		{

			//! Compare destination IP addess
			if(ip_version == 4)
			{
				if(!(memcmp(rsi_socket_pool[i].destination_ip_addr.ipv4, ip_addr, RSI_IPV4_ADDRESS_LENGTH)))
				{
					break;
				}
			}
			else
			{
				if(!(memcmp(rsi_socket_pool[i].destination_ip_addr.ipv6, ip_addr, RSI_IPV6_ADDRESS_LENGTH)))
				{
					break;
				}
			}
		}
	}

  if(i >= NUMBER_OF_SOCKETS)
  {
    return -1;
  }

  return i;
}
#endif

/*==============================================*/
/**
 * @fn         int32_t rsi_socket_async_non_rom(int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
 * @brief      Creates socket
 * @param[in]  protocolFamily , Type of the socket family to create
 * @param[in]  type , type of the socket to create 
 * @param[in]  protocol , to enable SSL over TCP 
 * @param[in]  callback , callback function to read data ayncronously from socket 
 *
 * @param[out] None
 * @return     
 *             <0 - If fails
 *             >= - If success
 *
 * @section description
 * This function creates socket
 *
 */

#ifdef RSI_IPV6_ENABLE
int32_t  rsi_socket_async_non_rom(global_cb_t *global_cb_p, int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
{
  int i;

  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! Check supported protocol family
  if((protocolFamily != AF_INET) && (protocolFamily != AF_INET6))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EAFNOSUPPORT);    

    return RSI_SOCK_ERROR;
  }

  //! Check supported socket type
  if((type != SOCK_STREAM) && (type != SOCK_DGRAM) /*&& (type != SOCK_RAW)*/ )
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EPROTOTYPE);

    return RSI_SOCK_ERROR;
  }

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! search for free entry in socket pool
  for(i = 0; i < NUMBER_OF_SOCKETS; i++)
  {
    if(rsi_socket_pool[i].sock_state == RSI_SOCKET_STATE_INIT)
    {
      //! Memset socket info
      memset(&rsi_socket_pool[i], 0, sizeof(rsi_socket_info_t));

      //! Change the socket state to create
      rsi_socket_pool[i].sock_state = RSI_SOCKET_STATE_CREATE;

      //! Set socket type
      rsi_socket_pool[i].sock_type = type;

      //! Set protocol type
      rsi_socket_pool[i].sock_type |= (protocolFamily << 4);

      if(callback)
      {
        //! Set socket asynchronous receive callback
        rsi_socket_pool[i].sock_receive_callback = callback;
      }
      else
      {
        //! If callback is registered set socket as asynchronous
        rsi_socket_pool[i].sock_bitmap |= RSI_SOCKET_FEAT_SYNCHRONOUS;
      }

      if(protocol & RSI_SOCKET_FEAT_SSL)
      {
        //! Check SSL enabled or not
        rsi_socket_pool[i].sock_bitmap |= RSI_SOCKET_FEAT_SSL;
      }

      if(protocol & RSI_SOCKET_FEAT_TCP_ACK_INDICATION)
      {
        //! Set TCP ACK indication bit
        rsi_socket_pool[i].sock_bitmap |= RSI_SOCKET_FEAT_TCP_ACK_INDICATION;


        if(((protocol >> 3) & 0XF) == 0)
        {
          rsi_socket_pool[i].max_available_buffer_count = rsi_socket_pool[i].current_available_buffer_count = 4;
        }
        else
        {
          rsi_socket_pool[i].max_available_buffer_count = rsi_socket_pool[i].current_available_buffer_count = ((protocol >> 3) & 0xF);
        }
      }

      if(protocol & RSI_SOCKET_FEAT_CERT_INDEX)
      {
        //! Set certificate index bit
        rsi_socket_pool[i].sock_bitmap |= RSI_SOCKET_FEAT_CERT_INDEX;

        rsi_socket_pool[i].socket_cert_inx  = (protocol >> 12);

      }
      else
      {
        rsi_socket_pool[i].socket_cert_inx  = 0;
      }

	  
      //! Get RX window buffers     
      rsi_socket_pool[i].rx_window_buffers = ((protocol >> 7) & 0xF);

      break;
    }
  }

  if(i >= NUMBER_OF_SOCKETS)
  {
    //! Release mutex lock
    RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

    //! Set error
    RSI_WLAN_SET_STATUS(ENFILE);

    return RSI_SOCK_ERROR;
  }

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Return available index
  return i;
}
#endif
/*==============================================*/
/**
 * @fn int32_t  rsi_socket_connect(global_cb_t *global_cb_p, int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength)
 * @brief       connect the socket to specified remoteAddress		  
 * @param[in]  global_cb_p,pointer to the global control block
 * @param[in]   sockID, socket descriptor 
 * @param[in]   remoteAddress, remote peer address structure 
 * @param[in]   addressLength, remote peer address structrue length 
 * @param[out]  None
 * @return
 *
 *              <0 - If fails
 *               0 - If success
 *
 * @section description
 * This function is use to connect the socket to specified remote address
 *
 */

int32_t  rsi_socket_connect(global_cb_t *global_cb_p, int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength)
{
  int32_t status = RSI_SUCCESS;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! If socket is not binded
  if(rsi_socket_pool[sockID].sock_state == RSI_SOCKET_STATE_INIT) 
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! Check whether supplied address structure
  if(remoteAddress == RSI_NULL)   
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EFAULT);

    return RSI_SOCK_ERROR;
  }

  //! Check address length
  if(((remoteAddress -> sa_family == AF_INET) && (addressLength != sizeof(struct rsi_sockaddr_in))) || \
      ((remoteAddress -> sa_family == AF_INET6) && (addressLength != sizeof(struct rsi_sockaddr_in6))))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EAFNOSUPPORT);

    return RSI_SOCK_ERROR;
  }

  //! Check if the remote address family matches the local BSD socket address family
  if(remoteAddress->sa_family != (rsi_socket_pool[sockID].sock_type >> 4))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EAFNOSUPPORT);

    return RSI_SOCK_ERROR;
  }

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! save destination port number ip address and port number to use in udp send()
  if(remoteAddress->sa_family == AF_INET)  
  {
    //! Save destination IPv4 address
    memcpy(rsi_socket_pool[sockID].destination_ip_addr.ipv4, &(((struct rsi_sockaddr_in *) remoteAddress )->sin_addr.s_addr), RSI_IPV4_ADDRESS_LENGTH);
    rsi_socket_pool[sockID].destination_port =  htons(((struct rsi_sockaddr_in *) remoteAddress ) -> sin_port);  
  }
  else
  {
    memcpy(rsi_socket_pool[sockID].destination_ip_addr.ipv6, ((struct rsi_sockaddr_in6 *) remoteAddress )->sin6_addr._S6_un._S6_u32, RSI_IPV6_ADDRESS_LENGTH);    
    rsi_socket_pool[sockID].destination_port =  htons(((struct rsi_sockaddr_in6 *) remoteAddress ) -> sin6_port);    
  }

  if((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_STREAM)
  {
    //! send socket create command
    status = global_cb_p->rom_apis_p->ROM_WL_rsi_socket_create(global_cb_p , sockID, RSI_SOCKET_TCP_CLIENT, 0);
  }
  else if(((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_DGRAM) && (rsi_socket_pool[sockID].sock_state == RSI_SOCKET_STATE_CREATE))
  {
    //! send socket create command
    status = global_cb_p->rom_apis_p->ROM_WL_rsi_socket_create(global_cb_p, sockID, RSI_SOCKET_LUDP, 0);
  }

  //! Fill socket ID
  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }

  return RSI_SUCCESS;

}

