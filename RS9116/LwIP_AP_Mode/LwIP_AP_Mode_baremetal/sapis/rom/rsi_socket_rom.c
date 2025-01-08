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
#ifndef ROM_WIRELESS
#include "rsi_driver.h"


/*==============================================*/
/**
 * @fn         int32_t socket_async(int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
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

int32_t  ROM_WL_socket_async(global_cb_t *global_cb_p, int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
{
  int i;

  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! Check supported protocol family
  if((protocolFamily != AF_INET))
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
/*==============================================*/
/**
 * @fn          int32_t socket(int32_t protocolFamily, int32_t type, int32_t protocol)
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
int32_t ROM_WL_socket(global_cb_t *global_cb_p, int32_t protocolFamily, int32_t type, int32_t protocol)
{
  int32_t status = RSI_SUCCESS;

  //! Call socket create
  status = ROM_WL_socket_async(global_cb_p, protocolFamily, type, protocol, NULL);

  return status;

}

/*==============================================*/
/**
 * @fn          int32_t  bind(int32_t sockID, struct rsi_sockaddr *localAddress, int32_t addressLength)
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
int32_t  ROM_WL_bind(global_cb_t *global_cb_p ,int32_t sockID, struct rsi_sockaddr *localAddress, int32_t addressLength)
{
  int32_t local_port;
  int32_t status = RSI_SUCCESS;
  uint8_t i;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }


  //! If socket is not created
  if(rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_CREATE)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! Check for a valid input local address and address length input buffer
  if ((localAddress == RSI_NULL ) || (addressLength == 0))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EFAULT);

    return RSI_SOCK_ERROR;

  }

  //! Check address length
  if(((localAddress -> sa_family == AF_INET) && (addressLength != sizeof(struct rsi_sockaddr_in))) ||
      ((localAddress -> sa_family == AF_INET6) && (addressLength != sizeof(struct rsi_sockaddr_in6))))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EAFNOSUPPORT);

    return RSI_SOCK_ERROR;
  }


  //! Check address family
  if((rsi_socket_pool[sockID].sock_type >> 4) != localAddress->sa_family)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EAFNOSUPPORT);

    return RSI_SOCK_ERROR;
  }

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Pickup the local port
  if(localAddress -> sa_family == AF_INET)  
  {
    local_port = ntohs(((struct rsi_sockaddr_in *) localAddress) -> sin_port);    
  }
  else
  {
    local_port = ntohs(((struct rsi_sockaddr_in6 *) localAddress) -> sin6_port);
  }

  //! Check whether local port is already used or not
  for(i = 0; i < NUMBER_OF_SOCKETS; i++)  
  {
    if(rsi_socket_pool[i].source_port == local_port)
    {
      //! Release mutex lock
      RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

      //! Set error
      RSI_WLAN_SET_STATUS(EADDRINUSE);

      return RSI_SOCK_ERROR;
    }
  }

  //! Port number is not used save the port number
  rsi_socket_pool[sockID].source_port = local_port;

  //! Send socket create if it is UDP
  if((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_DGRAM)
  {
    status = global_cb_p->rom_apis_p->ROM_WL_rsi_socket_create(global_cb_p, sockID, RSI_SOCKET_LUDP, 0);

    //! Set socket state as bind
    rsi_socket_pool[sockID].sock_state = RSI_SOCKET_STATE_CONNECTED;  
  }
  else
  {
    //! Set socket state as bind
    rsi_socket_pool[sockID].sock_state = RSI_SOCKET_STATE_BIND;  
  }


  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }

  //! return success
  return RSI_SUCCESS;
}
/*==============================================*/
/**
 * @fn int32_t  ROM_WL_connect(global_cb_t *global_cb_p, int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength)
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

int32_t  ROM_WL_connect(global_cb_t *global_cb_p, int32_t sockID, struct rsi_sockaddr *remoteAddress, int32_t addressLength)
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

  if(status == RSI_SUCCESS)
  {
    //! Change state to connected
    rsi_socket_pool[sockID].sock_state = RSI_SOCKET_STATE_CONNECTED;
  }

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }

  return RSI_SUCCESS;

}
/*==============================================*/
/**
 * @fn           int32_t  ROM_WL_listen(global_cb_t *global_cb_p, int32_t sockID, int32_t backlog) 
 * @brief      This function is used to make socket to listen for remote connecion request in passive mode
 * @param[in]  global_cb_p,pointer to the global control block
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

int32_t  ROM_WL_listen(global_cb_t *global_cb_p, int32_t sockID, int32_t backlog)
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

  //! Check socket is TCP or not
  if((rsi_socket_pool[sockID].sock_type & 0xF) != SOCK_STREAM)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EPROTOTYPE);

    return RSI_SOCK_ERROR;
  }

  //! If socket is not binded
  if(rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_BIND)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! Set minimum backlog 
  if(backlog < TCP_LISTEN_MIN_BACKLOG)
  {
    backlog = TCP_LISTEN_MIN_BACKLOG;
  }

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Create socket
  status = global_cb_p->rom_apis_p->ROM_WL_rsi_socket_create(global_cb_p, sockID, RSI_SOCKET_TCP_SERVER, backlog);

  if(status == RSI_SUCCESS)
  {
    //! Update state to listen is success
    rsi_socket_pool[sockID].sock_state = RSI_SOCKET_STATE_LISTEN;

    //! Set as master socket
    rsi_socket_pool[sockID].ltcp_socket_type = RSI_LTCP_PRIMARY_SOCKET;

    //! Save backlogs
    rsi_socket_pool[sockID].backlogs = backlog;
  }

  //! If fails memset socket info

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }
  return RSI_SUCCESS;

}
/*==============================================*/
/**
 * @fn         int32_t  ROM_WL_accept(global_cb_t *global_cb_p, int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength)
 * @brief      This function is used to accept connection request from remote peer  
 * @param[in]  global_cb_p,pointer to the global control block
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

int32_t  ROM_WL_accept(global_cb_t *global_cb_p, int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength)
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
    accept_sock_id = ROM_WL_socket_async(global_cb_p, (sock_info->sock_type >> 4), (sock_info->sock_type & 0xF), (sock_info->sock_bitmap & RSI_SOCKET_FEAT_SSL), sock_info->sock_receive_callback);

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
  pkt = ROM_WL_rsi_pkt_alloc(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool);
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
  ROM_WL_rsi_uint16_to_2bytes(global_cb_p, accept->source_port, rsi_socket_pool[sockID].source_port);

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
  ROM_WL_rsi_pkt_free(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

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
 * @fn         int32_t  ROM_WL_recvfrom(global_cb_t *global_cb_p, int32_t sockID, int8_t *buffer, int32_t buffersize, int32_t flags,struct rsi_sockaddr *fromAddr, int32_t *fromAddrLen) * @brief      This function is used to receive data from remote peer 
 * @param[in]  global_cb_p,pointer to the global control block
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

int32_t  ROM_WL_recvfrom(global_cb_t *global_cb_p, int32_t sockID, int8_t *buffer, int32_t buffersize, int32_t flags,struct rsi_sockaddr *fromAddr, int32_t *fromAddrLen)
{
  struct rsi_sockaddr_in  peer4_address;
  struct rsi_sockaddr_in6 peer6_address;
  int32_t status = RSI_SUCCESS;

  int32_t copy_length = 0;
  rsi_req_socket_read_t *data_recv;
  rsi_pkt_t *pkt = NULL;

  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *sock_info       = &global_cb_p->rsi_socket_pool[sockID];
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! If socket is not in connected state
  if(rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_CONNECTED)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(global_cb_p->rx_buffer_mem_copy != 1)
  {
    sock_info->recv_buffer = (uint8_t *)buffer;

    sock_info->recv_buffer_length = buffersize;
  }
  //! If nothing is left
  if(sock_info->sock_recv_available_length == 0 || global_cb_p->rx_buffer_mem_copy != 1 )
  {
    //! Allocate packet
    pkt = ROM_WL_rsi_pkt_alloc(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool);
    if(pkt == NULL)
    {
      //! Release mutex lock
      RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

      //! set error
      RSI_WLAN_SET_STATUS(RSI_ERROR_PKT_ALLOCATION_FAILURE);

      return RSI_SOCK_ERROR;
    }

    //! Send data recieve command
    data_recv = (rsi_req_socket_read_t *)pkt->data;

    //! memset the recv packet
    memset(data_recv,0,sizeof(rsi_req_socket_read_t));

    //! Fill socket ID
    data_recv->socket_id = sock_info->sock_id;
    if(buffersize >  1460)
    {
      buffersize = 1460;
    }

    //! Fill number of requesting bytes for receive
    ROM_WL_rsi_uint32_to_4bytes(global_cb_p, data_recv->requested_bytes, buffersize);
	
    //! Fill number of requesting bytes for receive
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, data_recv->read_timeout,rsi_socket_pool[sockID].read_time_out);

    //! Set waiting socket ID
    rsi_driver_cb->wlan_cb->waiting_socket_id = sockID;

    //! Send socket receive request command
    status = RSI_DRIVER_WLAN_SEND_CMD(RSI_WLAN_REQ_SOCKET_READ_DATA, pkt);

    if(status != RSI_SUCCESS)
    {
      //! Free packet
      ROM_WL_rsi_pkt_free(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

      //! Release mutex lock
      RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

      return RSI_SOCK_ERROR;
    }
  }

  if(global_cb_p->rx_buffer_mem_copy == 1)
  {
    //! Get minimum of requested length and available length
    if(buffersize > sock_info->sock_recv_available_length)
    {
      copy_length = sock_info->sock_recv_available_length;
    }
    else
    {
      copy_length = buffersize;
    }

    //! Copy available buffer
    memcpy(buffer, &sock_info->sock_recv_buffer[sock_info->sock_recv_buffer_offset], copy_length);

    //! Increase buffer offset
    sock_info->sock_recv_buffer_offset += copy_length;

    if(sock_info->sock_recv_available_length >= copy_length)
    {
      //! Decrease available length
      sock_info->sock_recv_available_length -= copy_length;
    }
    else
    {
      sock_info->sock_recv_available_length = 0;
    }

    if(sock_info->sock_recv_available_length == 0)
    {
      //! Reset offset value after reading total packet
      sock_info->sock_recv_buffer_offset = 0;
    }
  }
  else
  {
    copy_length = sock_info->recv_buffer_length;
  }

  //! If fromAddr is not NULL then copy the IP address
  if(fromAddr && (*fromAddrLen != 0))  
  {
    if((sock_info->sock_type >> 4) == AF_INET)
    {
      //! Update the Client address with socket family, remote host IPv4 address and port
      peer4_address.sin_family      = AF_INET;
      memcpy(&peer4_address.sin_addr.s_addr, (ntohl(rsi_socket_pool[sockID].destination_ip_addr.ipv4)), RSI_IPV4_ADDRESS_LENGTH);
      peer4_address.sin_port        = ntohs((uint16_t)rsi_socket_pool[sockID].destination_port);

      //! Copy the peer address/port info to the ClientAddress
      //! Truncate if addressLength is smaller than the size of struct rsi_sockaddr_in
      if(*fromAddrLen > sizeof(struct rsi_sockaddr_in))
      {
        *fromAddrLen = sizeof(struct rsi_sockaddr_in);
      }
      memcpy(fromAddr, &peer4_address, *fromAddrLen);
    }
    else
    {
      peer6_address.sin6_family = AF_INET6;
      peer6_address.sin6_port = ntohs((uint16_t)rsi_socket_pool[sockID].destination_port);        

      peer6_address.sin6_addr._S6_un._S6_u32[0] = (uint32_t)ntohl(&rsi_socket_pool[sockID].destination_ip_addr.ipv6[0]);
      peer6_address.sin6_addr._S6_un._S6_u32[1] = (uint32_t)ntohl(&rsi_socket_pool[sockID].destination_ip_addr.ipv6[4]);
      peer6_address.sin6_addr._S6_un._S6_u32[2] = (uint32_t)ntohl(&rsi_socket_pool[sockID].destination_ip_addr.ipv6[8]);
      peer6_address.sin6_addr._S6_un._S6_u32[3] = (uint32_t)ntohl(&rsi_socket_pool[sockID].destination_ip_addr.ipv6[12]);

      //! Copy the peer address/port info to the ClientAddress
      //! Truncate if addressLength is smaller than the size of struct rsi_sockaddr_in
      if((*fromAddrLen) > sizeof(struct rsi_sockaddr_in6))
      {
        *fromAddrLen = sizeof(struct rsi_sockaddr_in6);
      }
      memcpy(fromAddr, &peer6_address, *fromAddrLen);
    }
  }

  //! Free packet
  ROM_WL_rsi_pkt_free(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  return copy_length;
}
/*==============================================*/
/**
 * @fn         int32_t ROM_WL_recv(global_cb_t *global_cb_p, int32_t sockID, void *rcvBuffer, int32_t bufferLength, int32_t flags)
 * @brief      This function is used to receive data from remote peer 
 * @param[in]  global_cb_p,pointer to the global control block
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
int32_t  ROM_WL_recv(global_cb_t *global_cb_p, int32_t sockID, void *rcvBuffer, int32_t bufferLength, int32_t flags)
{
  int32_t status = 0;
  int32_t fromAddrLen = 0;

  //! call recvfrom() with fromaddr and fromaddrlen as null
  status = ROM_WL_recvfrom(global_cb_p, sockID, rcvBuffer, bufferLength, flags, NULL, &fromAddrLen);

  return status;
}

/*==============================================*/
/**
 * @fn          int32_t  ROM_WL_sendto(global_cb_t *global_cb_p, int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen)
 * @brief       This function is used to send data to specific remote peer on a given socket synchronously
 * @param[in]   global_cb_p,pointer to the global control block
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

int32_t ROM_WL_sendto(global_cb_t *global_cb_p, int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen)
{
  int32_t status = RSI_SUCCESS;

  status = ROM_WL_sendto_async(global_cb_p, sockID, msg, msgLength, flags, destAddr, destAddrLen, NULL);

  return status;

}

/*==============================================*/
/**
 * @fn          int32_t  ROM_WL_sendto_async(global_cb_t *global_cb_p, int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen,
                     void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
 *
 * @brief       This function is used to send data to specific remote peer on a given socket asynchronously
 * @param[in]  global_cb_p,pointer to the global control block
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
int32_t ROM_WL_sendto_async(global_cb_t *global_cb_p, int32_t sockID, int8_t *msg, int32_t msgLength, int32_t flags, struct rsi_sockaddr *destAddr, int32_t destAddrLen,
                     void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
{
  int32_t status = RSI_SUCCESS;
  int32_t maximum_limit = 0;
  uint8_t buffers_required;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *sock_info       = &global_cb_p->rsi_socket_pool[sockID];
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! If socket is in init state for UDP
  if(((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_DGRAM) && (rsi_socket_pool[sockID].sock_state == RSI_SOCKET_STATE_INIT))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EPROTOTYPE);

    return RSI_SOCK_ERROR;
  }
  if(((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_STREAM) && (rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_CONNECTED))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EPROTOTYPE);

    return RSI_SOCK_ERROR;
  }

  //! Check socket family
  if(destAddr->sa_family != (rsi_socket_pool[sockID].sock_type >> 4))
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EFAULT);

    return RSI_SOCK_ERROR;
  }

  //! Register the callback 
  rsi_driver_cb->nwk_cb->nwk_callbacks.data_transfer_complete_handler = data_transfer_complete_handler;

  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_DGRAM) && (rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_CONNECTED))
  {
    //! Create socket
    status = global_cb_p->rom_apis_p->ROM_WL_rsi_socket_create(global_cb_p, sockID, RSI_SOCKET_LUDP, 0);
    if(status != RSI_SUCCESS)
    {
      //! Release mutex lock
      RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);
      return status;
    }

    //! Change state to connected
    rsi_socket_pool[sockID].sock_state = RSI_SOCKET_STATE_CONNECTED;
  }

  //! Find maximum limit based on the protocol
  if((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_DGRAM)
  {
    //! If it is a UDP socket
    maximum_limit = 1472;
  }
  else if(((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_STREAM) && \
      (rsi_socket_pool[sockID].sock_bitmap & RSI_SOCKET_FEAT_SSL))
  {
    //! If it is a SSL socket/SSL websocket
    maximum_limit = 1390;
  }
  else if(((rsi_socket_pool[sockID].sock_type & 0xF) == SOCK_STREAM) && \
      (rsi_socket_pool[sockID].sock_bitmap & RSI_SOCKET_FEAT_WEBS_SUPPORT))
  {
    //! If it is a websocket
    maximum_limit = 1450;
  }
  else
  {
    maximum_limit = 1460;
  }

  //! Check maximum allowed length value
  if(msgLength > maximum_limit)
  {
    msgLength = maximum_limit;
  }
 
  if(sock_info->sock_bitmap & RSI_SOCKET_FEAT_TCP_ACK_INDICATION)
  {
    //! Return if buffers are not available
    if(sock_info->current_available_buffer_count == 0)
    {
      //! Set no buffers status and return
      RSI_WLAN_SET_STATUS(ENOBUFS);
      //! Release mutex lock
      RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

      return RSI_SOCK_ERROR;
    }

    //! Calculate buffers required for sending this length
    buffers_required = ROM_WL_calculate_buffers_required(global_cb_p, sock_info->sock_type, msgLength);

    if(buffers_required > sock_info->current_available_buffer_count)
    {
      //! Calculate length can be sent with available buffers
      msgLength = global_cb_p->rom_apis_p->ROM_WL_calculate_length_to_send(global_cb_p, sock_info->sock_type, sock_info->current_available_buffer_count);
      sock_info->current_available_buffer_count = 0;
    }
    else
    {
      //! Subtract buffers required to send this packet
      if(sock_info->current_available_buffer_count > buffers_required)
      {
        sock_info->current_available_buffer_count -= buffers_required;
      }
      else
      {
        sock_info->current_available_buffer_count = 0;
      }
    }
	
    //! Set expected response based on the callback
    if(data_transfer_complete_handler != NULL)
    {
#ifdef RSI_UART_INTERFACE
      rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_UART_DATA_ACK;
#else
      rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
#endif
    }
    else
    {
      rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_TCP_ACK_INDICATION;
    }
  }
  else
  {
#ifdef RSI_UART_INTERFACE
      rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_UART_DATA_ACK;
#else
      rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
#endif
  }

  //! Send data send command
  status = RSI_DRIVER_SEND_DATA(sockID, (uint8_t *)msg, msgLength, destAddr);

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status == RSI_SUCCESS)
  {
    return msgLength;
  }
  else
  {
    return RSI_SOCK_ERROR;
  }

}
/*==============================================*/
/**
 * @fn         int32_t ROM_WL_send(global_cb_t *global_cb_p, int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags)
 * @brief      This function is used to send data on a given socket syncronously
 * @param[in]  global_cb_p,pointer to the global control block
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

int32_t ROM_WL_send(global_cb_t *global_cb_p, int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags)
{
  int32_t status = RSI_SUCCESS;

  status = ROM_WL_send_async(global_cb_p, sockID, msg, msgLength, flags, NULL);

  //! Return status
  return status;
}



/*==============================================*/
/**
 * @fn         int32_t ROM_WL_send_async(global_cb_t *global_cb_p, int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags, 
                   void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
 * @brief      This function is used to send data on a given socket asynchronously
 * @param[in]  global_cb_p,pointer to the global control block
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
int32_t ROM_WL_send_async(global_cb_t *global_cb_p, int32_t sockID, const int8_t *msg, int32_t msgLength, int32_t flags, 
                   void (*data_transfer_complete_handler)(uint8_t sockID, uint16_t length))
{
  struct       rsi_sockaddr_in  fromAddr4;
  struct       rsi_sockaddr_in6 fromAddr6;
  int32_t status = 0;
  
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  if((rsi_socket_pool[sockID].sock_type >> 4) == AF_INET)
  {
    //! Set socket family for IPv4
    fromAddr4.sin_family = AF_INET;

    //! Set destination port
    fromAddr4.sin_port = rsi_socket_pool[sockID].destination_port;

    //! Set destination IPv4 address
    memcpy(&fromAddr4.sin_addr.s_addr, rsi_socket_pool[sockID].destination_ip_addr.ipv4, RSI_IPV4_ADDRESS_LENGTH);

    status = ROM_WL_sendto_async(global_cb_p, sockID, (int8_t *)msg, msgLength, flags, (struct rsi_sockaddr *)&fromAddr4, sizeof(fromAddr4), data_transfer_complete_handler);
  }
  else
  {
    //! Set socket family for IPv6
    fromAddr6.sin6_family = AF_INET6;

    //! Set destination port
    fromAddr6.sin6_port = rsi_socket_pool[sockID].destination_port;

    //! Set destination IPv6 address
    memcpy(fromAddr6.sin6_addr._S6_un._S6_u8, rsi_socket_pool[sockID].destination_ip_addr.ipv6, RSI_IPV6_ADDRESS_LENGTH);

    status = ROM_WL_sendto_async(global_cb_p, sockID, (int8_t *)msg, msgLength, flags, (struct rsi_sockaddr *)&fromAddr6, sizeof(fromAddr6), data_transfer_complete_handler);
  }

  //! Return status
  return status;
}

/*==============================================*/
/**
 * @fn          int32_t ROM_WL_shutdown(global_cb_t *global_cb_p, int32_t sockID, int32_t how)
 * @brief       This function is used to close the socket
 * @param[in]  global_cb_p,pointer to the global control block
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
int32_t  ROM_WL_shutdown(global_cb_t *global_cb_p, int32_t sockID, int32_t how)
{
  int32_t status = RSI_SUCCESS;

  rsi_req_socket_close_t *close;
  rsi_pkt_t *pkt = NULL;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! If sockID is not in available range
  if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
  {
    //! Set error
    RSI_WLAN_SET_STATUS(EBADF);

    return RSI_SOCK_ERROR;
  }

  //! If socket is not in connected state dont send socket close command
  if(rsi_socket_pool[sockID].sock_state != RSI_SOCKET_STATE_CONNECTED)
  {
    if(rsi_socket_pool[sockID].ltcp_socket_type != RSI_LTCP_PRIMARY_SOCKET)
    {
      //! Memeset socket info
      memset(&rsi_socket_pool[sockID], 0, sizeof(rsi_socket_info_t));

      //! Return success
      return RSI_SUCCESS;
    }
  }


  //! Acquire mutex lock
  RSI_MUTEX_LOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  //! Allocate packet
  pkt = ROM_WL_rsi_pkt_alloc(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool);
  if(pkt == NULL)
  {
    //! Release mutex lock
    RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

    //! Set error
    RSI_WLAN_SET_STATUS(RSI_ERROR_PKT_ALLOCATION_FAILURE);

    return RSI_SOCK_ERROR;
  }

  //! Fill socket close command
  close = (rsi_req_socket_close_t *)pkt->data;

  if(how == 1)
  {
    //! Port based close for LTCP
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, close->socket_id, 0);
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, close->port_number, rsi_socket_pool[sockID].source_port);  
  }
  else
  {
    //! Socket descriptor based close
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, close->socket_id, rsi_socket_pool[sockID].sock_id);
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, close->port_number, 0);  
  }

  //! Set waiting socket ID
  rsi_driver_cb->wlan_cb->waiting_socket_id = sockID;

  //! Send socket close command
  status = RSI_DRIVER_WLAN_SEND_CMD( RSI_WLAN_REQ_SOCKET_CLOSE, pkt);


  //! Free packet
  ROM_WL_rsi_pkt_free(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  //! Release mutex lock
  RSI_MUTEX_UNLOCK(&rsi_driver_cb->wlan_cb->wlan_mutex);

  if(status != RSI_SUCCESS)
  {
    return RSI_SOCK_ERROR;
  }

  //! Return status
  return RSI_SUCCESS;
}


/*==============================================*/
/**
 * @fn          int32_t  ROM_WL_rsi_socket_create(global_cb_t *global_cb_p, int32_t sockID, int32_t type, int32_t backlog)
 * @brief       Prepares and sends socket create to module
 * @param[in]  global_cb_p,pointer to the global control block
 * @param[in]   sockID, socket descriptor 
 * @param[in]   type, type of socket to create
 * @param[in]   backlog, number of backlogs for LTCP socket
 * @param[out]  None
 * @return
 *             <0 - If fails
 *              0 - If success
 *
 * @section description
 * This function is used to check the wlan state
 *
 */
int32_t ROM_WL_rsi_socket_create(global_cb_t *global_cb_p, int32_t sockID, int32_t type, int32_t backlog)
{
  rsi_pkt_t *pkt = NULL;
  rsi_req_socket_t *socket_create;
  int32_t status = 0;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  //! Socket Parameters
  pkt = ROM_WL_rsi_pkt_alloc(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool);

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
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, socket_create->ip_version, 4);
    memcpy(socket_create->dest_ip_addr.ipv4_address, rsi_socket_pool[sockID].destination_ip_addr.ipv4, RSI_IPV4_ADDRESS_LENGTH);
  }
  else
  {
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, socket_create->ip_version, 6);
    memcpy(socket_create->dest_ip_addr.ipv6_address, rsi_socket_pool[sockID].destination_ip_addr.ipv6, RSI_IPV6_ADDRESS_LENGTH);
  }

  //! Fill local port
  ROM_WL_rsi_uint16_to_2bytes(global_cb_p, socket_create->module_socket, rsi_socket_pool[sockID].source_port);

  //! Fill destination port
  ROM_WL_rsi_uint16_to_2bytes(global_cb_p, socket_create->dest_socket, rsi_socket_pool[sockID].destination_port); 

  //! Fill socket type
  ROM_WL_rsi_uint16_to_2bytes(global_cb_p, socket_create->socket_type, type); 

  //! Set backlogs
  if(type == RSI_SOCKET_TCP_SERVER)
  {
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, socket_create->max_count, backlog);   
    socket_create->socket_bitmap |= RSI_SOCKET_FEAT_LTCP_ACCEPT;
  }
  else
  {
    ROM_WL_rsi_uint16_to_2bytes(global_cb_p, socket_create->max_count, 0);   
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

  //! Send socket create command
  status = RSI_DRIVER_WLAN_SEND_CMD( RSI_WLAN_REQ_SOCKET_CREATE, pkt);

  //! Free packet
  ROM_WL_rsi_pkt_free(global_cb_p, &rsi_driver_cb->wlan_cb->wlan_tx_pool, pkt);

  return status;

}

/*==============================================*/
/**
 * @fn          int32_t  ROM_WL_rsi_get_application_socket_descriptor(global_cb_t *global_cb_p, int32_t sock_id)
 * @brief      Gets application socket descriptor from module socket descriptor
 * @param[in]  global_cb_p,pointer to the global control block
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
int32_t ROM_WL_rsi_get_application_socket_descriptor(global_cb_t *global_cb_p, int32_t sock_id)
{
  int i;
  
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

  for(i = 0; i < NUMBER_OF_SOCKETS; i++)
  {
    if(rsi_socket_pool[i].sock_id == sock_id)
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
 * @fn          void ROM_WL_rsi_clear_sockets(global_cb_t *global_cb_p, uint8_t sockID)
 * @brief       function to clear socket information
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   sockID, socket descriptor
 * @param[out]  None
 * @return      void
 *
 * @section description
 * This function is used to clear socket information
 *
 */
void ROM_WL_rsi_clear_sockets(global_cb_t *global_cb_p, uint8_t sockID)
{
  int i;
  uint8_t  sock_id;
  uint16_t port_number;
  
  rsi_driver_cb_t   *rsi_driver_cb   = global_cb_p->rsi_driver_cb;
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;
  
  if(sockID == RSI_CLEAR_ALL_SOCKETS)
  {
    for(i = 0; i < NUMBER_OF_SOCKETS; i++)
    {
      //! Memset socket info
      memset(&rsi_socket_pool[i], 0, sizeof(rsi_socket_info_t));
    }
  }
  else if(sockID == RSI_LTCP_PORT_BASED_CLOSE)
  {
    //! LTCP port based close
    sockID = rsi_driver_cb->wlan_cb->waiting_socket_id; 

    port_number = rsi_socket_pool[sockID].source_port;

    for(i = 0; i < NUMBER_OF_SOCKETS; i++)
    {
      if((rsi_socket_pool[i].source_port == port_number) && ((rsi_socket_pool[i].sock_type & 0xF) == SOCK_STREAM))
      {
        //! Memset socket info
        memset(&rsi_socket_pool[i], 0, sizeof(rsi_socket_info_t));
      }
    }

  }
  else
  {
    if(sockID < 0 || sockID >= NUMBER_OF_SOCKETS)
    { 
      return;
    }
	
    if((rsi_socket_pool[sockID].ltcp_socket_type == RSI_LTCP_PRIMARY_SOCKET)) 
    {
      if(rsi_socket_pool[sockID].backlog_current_count)
      {
        rsi_socket_pool[sockID].sock_state = RSI_SOCKET_STATE_LISTEN;
        rsi_socket_pool[sockID].backlog_current_count--;

      }
    }
    else if(rsi_socket_pool[sockID].ltcp_socket_type == RSI_LTCP_SECONDARY_SOCKET)
    {
      sock_id = ROM_WL_rsi_get_primary_socket_id(global_cb_p, rsi_socket_pool[sockID].source_port);

      //! Decrease backlog current count
      rsi_socket_pool[sock_id].backlog_current_count--;

      //! Memset socket info
      memset(&rsi_socket_pool[sockID], 0, sizeof(rsi_socket_info_t));

    }
    else
    {
      //! Memset socket info
      memset(&rsi_socket_pool[sockID], 0, sizeof(rsi_socket_info_t));
    }
  }
}

/*==============================================*/
/**
 * @fn          int32_t ROM_WL_rsi_get_primary_socket_id(global_cb_t *global_cb_p, uint16_t port_number)
 * @brief       function to get the primary socket ID from port number
 * @param[in]  global_cb_p,pointer to the global control block
 * @param[in]   port_number, port number
 * @param[out]  None
 * @return 
 *              <0  - If socket is not found
 *              >=0 - socket descriptor
 *
 * @section description
 * This function is used to clear socket information
 *
 */
int32_t ROM_WL_rsi_get_primary_socket_id(global_cb_t *global_cb_p, uint16_t port_number)
{
  int i;
  
  rsi_socket_info_t *rsi_socket_pool = global_cb_p->rsi_socket_pool;

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

/*==============================================*/
/**
 * @fn          uint8_t ROM_WL_calculate_buffers_required(global_cb_t *global_cb_p, uint8_t type, uint16_t length)
 * @brief       This api caluculates the buffers required for the data packet to transmit
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   type, type of socket to create
 * @param[in]   length,length of the message

 * @return The number of buffers required for a data packet
 *
 *
 * @section description
 * This function is used to calculate number of buffers required for the 
 * data packet
 *
 */
uint8_t ROM_WL_calculate_buffers_required(global_cb_t *global_cb_p, uint8_t type, uint16_t length)
{

  uint8_t header_size, buffers_required = 1;
  uint16_t first_buffer_available_size, remaining_length;

  //! Calculate header size including extra 2bytes based on the proto type
  if((type & 0xF) == SOCK_STREAM)
  {
    header_size = 56;
  }
  else
  {
    header_size = 44;
  }

  //! Increase header size by 20 for IPv6 case
  if((type >> 4) == AF_INET6)
  {
    header_size += 20;
  }

  remaining_length = length;

  first_buffer_available_size = (512 - header_size - 252);
  
  if(length <= first_buffer_available_size)
  {
    return 1;
  }

  remaining_length -= first_buffer_available_size;

  do
  {
    buffers_required++;
    if(remaining_length > 512)
    {
      remaining_length -= 512;
    }
    else
    {
      remaining_length = 0;
    }

  }while(remaining_length);

  return buffers_required;
}
/*==============================================*/
/**
 * @fn         uint16_t ROM_WL_calculate_length_to_send(global_cb_t *global_cb_p, uint8_t type, uint8_t buffers)
 * @brief      This API caluculates the msg length sent using available buffers      
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]  type,type of the socket stream
 * @param[in]  buffers,available buffers
 * @param[out]
 * @return
 *
 *
 * @section description
 * This API caluculates the msg length sent using available buffers       
 *
 *
 */

uint16_t ROM_WL_calculate_length_to_send(global_cb_t *global_cb_p, uint8_t type, uint8_t buffers)
{
  uint8_t header_size;
  uint16_t length;

  //! Calculate header size including extra 2bytes based on the proto type
  if((type & 0xF) == SOCK_STREAM)
  {
    header_size = 56;
  }
  else
  {
    header_size = 44;
  }

  //! Increase header size by 20 for IPv6 case
  if((type >> 4) == AF_INET6)
  {
    header_size += 20;
  }
 
  length = (512 - header_size - 252);

  if(buffers == 1)
  {
    return length;
  }

  buffers--;

  while(buffers)
  {
    length += 512;
	buffers--;
  }

  return length;

}
#endif



