/*************************************************************************
 * 
 * Copyright (c) 2019 Redpine Signals Incorporated. All Rights Reserved.
 * 
 * NOTICE:  All  information  contained  herein is, and  remains  the  property of 
 * Redpine Signals Incorporated. The intellectual and technical concepts contained
 * herein  are  proprietary to  Redpine Signals Incorporated and may be covered by 
 * U.S. and Foreign Patents, patents in process, and are protected by trade secret
 * or copyright law. Dissemination  of this  information or reproduction  of  this
 * material is strictly forbidden unless prior written permission is obtained from
 * Redpine Signals Incorporated.
 */

/**
 * Include files
 * */

//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h" 

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Access point SSID to connect
#define SSID              "REDPINE_AP"       

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               NULL

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE         1

//!TCP Max retries
#define RSI_MAX_TCP_RETRIES    10

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP         0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 

#endif
#define RECV_BUFFER_SIZE   1000 

//! Device port number
#define DEVICE_PORT       5001

//! Server port number
#define SERVER_PORT       5001
//! Parameter to run forever loop
#define RSI_FOREVER             1 

//! Memory length for driver
#define GLOBAL_BUFF_LEN   15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];
#define MAX_SOCK     1
uint32_t  new_socket[MAX_SOCK];
uint32_t  server_socket[MAX_SOCK];
struct    rsi_sockaddr_in  client_addr[MAX_SOCK];
int32_t     client_socket;
struct    rsi_sockaddr_in server_addr[MAX_SOCK];
int32_t     status       = RSI_SUCCESS;
int32_t     packet_count = 0, recv_size =0, addr_size;
uint8_t max_tcp_retry = RSI_MAX_TCP_RETRIES;
#if !(DHCP_MODE)
uint32_t    ip_addr      = DEVICE_IP;
uint32_t    network_mask = NETMASK;
uint32_t    gateway      = GATEWAY;
#endif
	uint8_t count = 0;
int8_t recv_buffer[RECV_BUFFER_SIZE];                                             
rsi_fd_set  read_fds;
rsi_fd_set  write_fds;
rsi_fd_set  except_fds;
struct rsi_timeval time;
//! Enumeration for states in applcation 
typedef enum rsi_wlan_app_state_e
{
  RSI_WLAN_INITIAL_STATE          = 0,
  RSI_WLAN_UNCONNECTED_STATE      = 1,
  RSI_WLAN_CONNECTED_STATE        = 2,
  RSI_WLAN_IPCONFIG_DONE_STATE    = 3,
  RSI_WAIT_FOR_SELECT_CONFIRM     = 4,
  RSI_WLAN_DATA_RECEIVE_STATE     = 5
}rsi_wlan_app_state_t;
//! wlan application control block
typedef struct rsi_wlan_app_cb_s
{
  //! wlan application state 
  rsi_wlan_app_state_t state;

}rsi_wlan_app_cb_t;

//! application control block
rsi_wlan_app_cb_t rsi_wlan_app_cb;


uint32_t socket_select_response =0;
void async_socket_select(rsi_fd_set *fd_read,rsi_fd_set *fd_write,rsi_fd_set *fd_except,int32_t status)
{
  memset(&read_fds, 0, sizeof(rsi_fd_set));
  memcpy(&read_fds,fd_read,sizeof(rsi_fd_set));
  socket_select_response = 1;
}

int32_t rsi_socket_select()
{
  int32_t     status = RSI_SUCCESS;
  switch(rsi_wlan_app_cb.state)
  {
    case RSI_WLAN_INITIAL_STATE:
      {
        //! update wlan application state
        rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE; 
      }
    case RSI_WLAN_UNCONNECTED_STATE:
      {  
        //! Connect to an Access point
        status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
        if(status != RSI_SUCCESS)
        {
          break;
        }
        else
        {
          //! update wlan application state
          rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE; 
        }
      }
    case RSI_WLAN_CONNECTED_STATE:
      {
        //! Configure IP 
#if DHCP_MODE
        status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP,0, 0, 0, NULL, 0,0);
#else
        status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
        if(status != RSI_SUCCESS)
        {
          break;
        }
        else
        {
          //! update wlan application state
          rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE; 
        }
      }   
    case RSI_WLAN_IPCONFIG_DONE_STATE:
      { 
        for(count = 0; count < MAX_SOCK; count++)
        {
        //! Create socket
          server_socket[count] = rsi_socket(AF_INET, SOCK_STREAM, 0);
        if(server_socket[count] < 0)
        {
          status = rsi_wlan_get_status();
          break; 
        }

        //! Set server structure
          memset(&server_addr[count], 0, sizeof(server_addr[count]));  

        //! Set family type
          server_addr[count].sin_family= AF_INET;

        //! Set local port number
          server_addr[count].sin_port = htons(DEVICE_PORT + count);



        //! Bind socket
          status = rsi_bind(server_socket[count], (struct rsi_sockaddr *) &server_addr[count], sizeof(server_addr[count]));
        if(status != RSI_SUCCESS)
        {
          status = rsi_wlan_get_status();
            rsi_shutdown(server_socket[count], 0);
          break; 
        }

        //! Socket listen
          status = rsi_listen(server_socket[count], 1);
        if(status != RSI_SUCCESS)
        {
          status = rsi_wlan_get_status();
            rsi_shutdown(server_socket[count], 0);
          break; 
        }

          addr_size = sizeof(server_socket[count]);
        //! Socket accept
        }
        for(count = 0; count < MAX_SOCK; count++)
        {
          new_socket[count] = rsi_accept(server_socket[count], (struct rsi_sockaddr *)&client_addr[count], &addr_size);
          if(new_socket[count] < 0)
        {
          status = rsi_wlan_get_status();
          rsi_shutdown(server_socket[count], 0);
          break; 
          }
        }
        rsi_wlan_app_cb.state = RSI_WAIT_FOR_SELECT_CONFIRM;
        break;

      }
    case RSI_WAIT_FOR_SELECT_CONFIRM:
        {
          memset(&read_fds, 0, sizeof(rsi_fd_set));  
        for(count = 0; count < MAX_SOCK; count++)
        {
          if(!(rsi_getsockopt(new_socket[count],SOL_SOCKET,SO_CHECK_CONNECTED_STATE,NULL,NULL)))
          {
            RSI_FD_SET(new_socket[count], &read_fds);
          }
        }
        status = rsi_select(new_socket[MAX_SOCK - 1] + 1, &read_fds,NULL,NULL,NULL,async_socket_select);
        if(status <  0 )
          {
          break;
          }
        else
        {
          //! update wlan application state
          rsi_wlan_app_cb.state = RSI_WLAN_DATA_RECEIVE_STATE; 
        }
      }
    case RSI_WLAN_DATA_RECEIVE_STATE:
      {
        if(socket_select_response)
          {
            recv_size = RECV_BUFFER_SIZE;
          {
            for(count = 0; count < MAX_SOCK; count++)
            {
              if(RSI_FD_ISSET(new_socket[count], &read_fds))
            {
              //! Receive data on socket
                status = rsi_recvfrom(new_socket[count], recv_buffer, recv_size, 0, (struct rsi_sockaddr *)&client_addr, &addr_size);
              if(status < 0)
              {
                  status = rsi_wlan_get_status();
                if(status == RSI_RX_BUFFER_CHECK)
                {
                  break;
                }
              }
              }
              //! subtract received bytes 
            }
            socket_select_response =0;
            rsi_wlan_app_cb.state = RSI_WAIT_FOR_SELECT_CONFIRM; 
          }
        }
      }
  }
}

int main()
{
  int32_t status;
#ifdef RSI_WITH_OS

  rsi_task_handle_t wlan_task_handle = NULL;

  rsi_task_handle_t driver_task_handle = NULL;
#endif


  //! Driver initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if((status < 0) || (status > GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! Redpine module intialisation
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! WiSeConnect initialization
  status = rsi_wireless_init(0,0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }


#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_socket_select, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  while(RSI_FOREVER)
  {
    //! WLAN application tasks
    rsi_socket_select();

    //! wireless driver tasks
    rsi_wireless_driver_task();

  }
#endif
  return status;

}

