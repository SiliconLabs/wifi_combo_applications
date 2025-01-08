/**
 * @file    rsi_ftp.c
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
 *  @brief : This file contains FTP API's
 *
 *  @section Description  This file contains FTP(File transfer protocol) application related 
 *  API's.
 *
 *
 */


#include "rsi_driver.h"

/*==============================================*/
/**
 * @fn         int32_t rsi_ftp_connect(uint16_t flags, int8_t *server_ip, int8_t *username, int8_t *password, uint32_t server_port)
 * @brief      Connects to the FTP server on the given FTP server port  
 * @param[in]   Network flags,Each bit has its own significance
 *              Bit(0) -IP version 
 *                 0 - IPv4 
 *                 1 - IPv6 
 * @param[in]  server_ip , IPv4/IPv6 address of the FTP server.  
 * @param[in]  username , username for the FTP server(credentials)
 * @param[in]  password , password for the FTP server(credentials)
 * @param[in]  server_port ,FTP server port number
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This API is used to create FTP objects and connect to the FTP server on the given server port
 * This should be the first command for accessing FTP.
 * 
 */


int32_t rsi_ftp_connect(uint16_t flags, int8_t *server_ip, int8_t *username, int8_t *password, uint32_t server_port)
{

  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;
  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *file_ops = NULL;

  rsi_ftp_connect_t *ftp_connect = NULL;

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

  file_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as FTP Create
  file_ops->command_type = RSI_FTP_CREATE;

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  if(status != RSI_SUCCESS)
  {
    //! free the packet
    rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

    //! unlock mutex
    rsi_mutex_unlock(&wlan_cb->wlan_mutex);

    //! Return the status if error in sending command occurs
    return status;
  }
 
  ftp_connect =  (rsi_ftp_connect_t *)pkt->data;

  //! Memset the packet
  memset(pkt,0,sizeof(rsi_ftp_connect_t));

  //! Set command type as FTP connect
  ftp_connect->command_type = RSI_FTP_CONNECT;


  if(!(flags & RSI_IPV6))
  {
    //! fill IP version
    ftp_connect->ip_version = RSI_IP_VERSION_4;

    //! Fill IP address
    memcpy(ftp_connect->server_ip_address.ipv4_address, server_ip, RSI_IPV4_ADDRESS_LENGTH);
  }
  else
  {
    //! fill IP version
    ftp_connect->ip_version = RSI_IP_VERSION_6;


    //! Fill IPv6 address
    memcpy(ftp_connect->server_ip_address.ipv6_address, server_ip, RSI_IPV6_ADDRESS_LENGTH);
  }
  if(username)
  {
    //! Copy login username
    rsi_strcpy(ftp_connect->username, username);
  }
  
  if(password)
  {
    //! Copy login password
    rsi_strcpy(ftp_connect->password, password);
  }

  //! copy FTP Server port  
  rsi_uint32_to_4bytes(ftp_connect->server_port,server_port);

  //! send set FTP Connect command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;

}

/*==============================================*/
/**
 * @fn         int32_t rsi_ftp_disconnect()
 * @brief      Disconnects from FTP server and destroys FTP objects created 
 * @param[in]  None  
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to disconnect from the FTP server and destroy the FTP objects.
 * Once FTP objects are destroyed,FTP cannot be accessed.For the further accessing,FTP 
 * objects should be created again
 *
 */
int32_t rsi_ftp_disconnect(void)
{
  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;
  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as FTP Create
  ftp_ops->command_type = RSI_FTP_DISCONNECT;

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  if(status != RSI_SUCCESS)
  {
    //! free the packet
    rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

    //! unlock mutex
    rsi_mutex_unlock(&wlan_cb->wlan_mutex);

    //! Return the status if error in sending command occurs
    return status;
  }
  
  //! Memset the packet
  memset(pkt,0,sizeof(rsi_ftp_file_ops_t));
 
   //! get command type as FTP Create
  ftp_ops->command_type = RSI_FTP_DESTROY;

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);
  
  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;

}

/*==============================================*/
/**
 * @fn         int32_t rsi_ftp_file_write(int8_t *file_name)
 * @brief      Opens a file to write 
 * @param[in]  file_name , filename/file name with path to open
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to open a file in the specified path on the FTP server
 * 
 */
int32_t rsi_ftp_file_write(int8_t *file_name)
{
 
  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as FTP file write
  ftp_ops->command_type = RSI_FTP_FILE_WRITE;

  //! copy the filename/path
  rsi_strcpy(ftp_ops->file_name, file_name);

  //! send  FTP  command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;


}


/*==============================================*/
/**
 * @fn          int32_t rsi_ftp_file_write_content(uint16_t flags, int8_t *file_content,int16_t content_length,uint8_t end_of_file)
 * @brief       Writes the content into the file opened using rsi_ftp_file_write()API
 * @param[in]   flags,Network flags,Each bit has its own significance
 *              Bit(0) -IP version 
 *                 0 - IPv4 
 *                 1 - IPv6 
 * @param[in]   file_content , data to be written into the file 
 * @param[in]   content_length , length of file content to be sent
 * @param[in]   end_of_file ,  Represents whether end of file is reached or not
 *              0 – More data is coming to write into file
 *              1 – Current chunk is the last chunk and no more data is coming
 * @return      int32_t 
 *              0  =  success
 *              <0 = failure 
 * @section description
 * This function is used to write the content into the file. 
 *
 */
int32_t rsi_ftp_file_write_content(uint16_t flags, int8_t *file_content,int16_t content_length,uint8_t end_of_file)
{

  int32_t  status = RSI_SUCCESS;
  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_write_t *ftp_file_write = NULL;

  rsi_pkt_t       *pkt;
  
  uint32_t         file_offset = 0;

  uint32_t         chunk_size = 0;

  uint32_t         send_size = 0;

  uint8_t          head_room = 0;

  uint8_t *host_desc = NULL;


 
  if(!(flags & RSI_IPV6))
  {
    //! Headroom for IPv4
    head_room = RSI_TCP_FRAME_HEADER_LEN;
  }
  else
  {
    //! Headroom for IPv6
    head_room = RSI_TCP_V6_FRAME_HEADER_LEN;
  }

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
 
  
 ftp_file_write =  (rsi_ftp_file_write_t *)pkt->data;

 while(content_length)
 {
   //! Memset the packet
   memset(pkt,0,(sizeof(pkt) + sizeof(rsi_ftp_file_write_t)));

   //! get command type as file delete
   ftp_file_write->command_type = RSI_FTP_FILE_WRITE_CONTENT;


   if(content_length > RSI_FTP_MAX_CHUNK_LENGTH)
   {
     chunk_size = RSI_FTP_MAX_CHUNK_LENGTH; 

#ifndef RSI_UART_INTERFACE
     //! Magic number to set asyncronous reswponse
      rsi_driver_cb->wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS;
#endif
     
     //! copy end of file 
     ftp_file_write->end_of_file = 0;

   }
   else
   {
     chunk_size = content_length;

     //! copy end of file 
     ftp_file_write->end_of_file = end_of_file;

#ifndef RSI_UART_INTERFACE
     if(!end_of_file)
     {

       //! Magic number to set asyncronous reswponse
       wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

     }
#endif

   }
   //! Copy file content
   memcpy((((uint8_t *)ftp_file_write) + head_room ), (file_content + file_offset), chunk_size);

   //! Add headroom to send size
   send_size = chunk_size + head_room ;
    
   //! get the host descriptor
   host_desc = (pkt->desc);

   //! Fill data length in the packet host descriptor
   rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

   //! send set FTP Create command 
   status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);
   
   //! If  fails ,donot send other chunks
   if(status != RSI_SUCCESS)
   {
     break;
   }

   //! Increase file offset
   file_offset  += chunk_size;

    //! Decrese file remaining size
    content_length -= chunk_size;
 }

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;



}

/*==============================================*/
/**
 * @fn          int32_t rsi_ftp_file_read_async(int8_t *file_name, void (*call_back_handler_ptr)(uint16_t status,int8_t *file_content, uint16_t content_length, uint8_t end_of_file))
 * @brief       Reads the file content   
 * @param[in]   file_name , file name to read the content from FTP server 
 * @param[in]   call_back_handler_ptr , call back to be called when data is read from the file specified from FTP server 
 *              status, status info .below paramters are valid only if status is 0
 *              file_content , data stream or file content
 *              content_length ,length of data
 *              end_of_file: indicates end of file
 *               if 1 – No more data
 *                0 – more data present
 * @return      int32_t 
 *              0  =  success
 *              <0 = failure 
 * @section description
 * This function is used to read the content from the file content from the specified file on the FTP server
 * 
 *
 */
int32_t rsi_ftp_file_read_aysnc(int8_t *file_name, void (*call_back_handler_ptr)(uint16_t status, uint8_t *file_content, uint16_t content_length, uint8_t end_of_file))
{

  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  if(call_back_handler_ptr != NULL)
  {
    //! Register FTP file read response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.ftp_file_read_call_back_handler = call_back_handler_ptr;
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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as FTP file Read
  ftp_ops->command_type = RSI_FTP_FILE_READ;

  //! copy the filename/path
  rsi_strcpy(ftp_ops->file_name, file_name);

  
  //! Magic number to set asyncronous reswponse
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

  //! send  FTP  command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;

}




/*==============================================*/
/**
 * @fn         int32_t rsi_ftp_file_delete(int8_t *file_name)
 * @brief      Delete the specified given file
 * @param[in]  file_name , filename/file name with path to delete
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to delete the file in the specified path on the FTP server
 * 
 */
int32_t rsi_ftp_file_delete(int8_t *file_name)
{

   int32_t  status = RSI_SUCCESS;

   rsi_pkt_t       *pkt;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as file delete
  ftp_ops->command_type = RSI_FTP_FILE_DELETE;

  //! copy the filename/path to delete
  rsi_strcpy(ftp_ops->file_name, file_name);

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;



}

/*==============================================*/
/**
 * @fn         rsi_ftp_file_rename(int8_t *old_file_name, int8_t *new_file_name)
 * @brief      Renames the old file with the new name
 * @param[in]  old_file_name , filename/file name which has to be renamed
 * @param[in]  new_file_name , filename/file name 
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to rename the file with the new name on the FTP server
 * 
 */
int32_t rsi_ftp_file_rename(int8_t *old_file_name, int8_t *new_file_name)
{

  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;
  
  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_rename_t *file_rename = NULL;

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

  file_rename =  (rsi_ftp_file_rename_t *)pkt->data;

  //! get command type as file rename
  file_rename->command_type = RSI_FTP_FILE_RENAME;

  //! copy the filename/path of old file name
  rsi_strcpy(file_rename->old_file_name, old_file_name);

  //! copy the filename/path of new file name
  rsi_strcpy(file_rename->new_file_name, new_file_name);

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;



}


/*==============================================*/
/**
 * @fn         int32_t  rsi_ftp_directory_create(int8_t *directory_name)
 * @brief      Creates a directory with the specified name
 * @param[in]  directory_name ,directory name(with path if required) to create
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to create a directory on the FTP server
 * 
 */
int32_t rsi_ftp_directory_create(int8_t *directory_name)
{

  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as directory create
  ftp_ops->command_type = RSI_FTP_DIRECTORY_CREATE;

  //! copy the directory name to create
  rsi_strcpy(ftp_ops->file_name, directory_name);

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;



}

/*==============================================*/
/**
 * @fn         int32_t  rsi_ftp_directory_delete(int8_t *directory_name)
 * @brief      Deletes a directory with the specified name
 * @param[in]  directory_name ,directory name(with path if required) to delete
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to delete a directory on the FTP server
 * 
 */
int32_t rsi_ftp_directory_delete(int8_t *directory_name)
{
  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as directory delete
  ftp_ops->command_type = RSI_FTP_DIRECTORY_DELETE;

  //! copy the filename/path to delete
  rsi_strcpy(ftp_ops->file_name, directory_name);

  //! send set FTP  command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;


}

/*==============================================*/
/**
 * @fn         int32_t  rsi_ftp_directory_set(int8_t *directory_path)
 * @brief      Changes the current working directory to the given directory path
 * @param[in]  directory_path ,directory path(with path if required) to change
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to change the current working directory on the FTP server
 * 
 */
int32_t rsi_ftp_directory_set(int8_t *directory_path)
{
  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as directory delete
  ftp_ops->command_type = RSI_FTP_DIRECTORY_SET;

  //! copy the filename/path to delete
  rsi_strcpy(ftp_ops->file_name, directory_path);

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;


}

/*==============================================*/
/**
 * @fn         int32_t rsi_ftp_directory_list_async(int8_t *directory_path,void (*call_back_handler_ptr)(uint16_t status,int8_t *directory_list, uint16_t length , uint8_t end_of_list)) 
 * @brief      Lists the directories present in the directory path specified.
 * @param[in]  directory_path ,directory path(with path if required) to list
 * @param[in]  call_back_handler_ptr ,call back which has to be called when directory list is obtained from FTP server asynchronously
 *              status, status info .below paramters are valid only if status is 0
 *             directory_list ,data of directory list coming from the FTP server
 *             length         ,length of the data which is valid
 *             end_of_list    ,indicates the end/presence of data yet to come 
 *                            1 - data ends with this chunk
 *                            0 - more data is pending 
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to get the list of directories in the specified directory on the FTP server
 * 
 */
int32_t rsi_ftp_directory_list_async(int8_t *directory_path,void (*call_back_handler_ptr)(uint16_t status, uint8_t *directory_list, uint16_t length , uint8_t end_of_list))
{
 
  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  if(call_back_handler_ptr != NULL)
  {
    //! Register FTP directory list response notify call back handler
    rsi_driver_cb->nwk_cb->nwk_callbacks.ftp_directory_list_call_back_handler = call_back_handler_ptr;
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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! get command type as directory list
  ftp_ops->command_type = RSI_FTP_DIRECTORY_LIST;

  //! copy the directory path to list
  rsi_strcpy(ftp_ops->file_name, directory_path);

  
  //! Magic number to set asyncronous reswponse
  wlan_cb->expected_response = RSI_WLAN_RSP_ASYNCHRONOUS ;

  //! send set FTP  command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;


}


/*==============================================*/
/**
 * @fn         int32_t rsi_ftp_mode_set(uint8_t mode)
 * @brief      To set the FTP client mode as PASSIVE or ACTIVE
 * @param[in]  None  
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to set the FTP client mode (PASSIVE/ ACTIVE).
 * Once FTP objects are destroyed,FTP cannot be accessed.For the further accessing,FTP 
 * objects should be created again
 *
 */
int32_t rsi_ftp_mode_set(uint8_t mode)
{
  int32_t  status = RSI_SUCCESS;

  rsi_pkt_t       *pkt;

  //! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  rsi_ftp_file_ops_t *ftp_ops = NULL;

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

  ftp_ops =  (rsi_ftp_file_ops_t *)pkt->data;

  //! Check for PASSIVE mode
  if(mode)
  {
    //! get command type as FTP PASSIVE
    ftp_ops->command_type = RSI_FTP_PASSIVE;
  }
  else
  {
    //! get command type as FTP ACTIVE
    ftp_ops->command_type = RSI_FTP_ACTIVE;
  }

  //! send set FTP Create command 
  status = rsi_driver_wlan_send_cmd(RSI_WLAN_REQ_FTP, pkt);

  //! free the packet
  rsi_pkt_free(&wlan_cb->wlan_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&wlan_cb->wlan_mutex);

  //! Return the status if error in sending command occurs
  return status;

}
