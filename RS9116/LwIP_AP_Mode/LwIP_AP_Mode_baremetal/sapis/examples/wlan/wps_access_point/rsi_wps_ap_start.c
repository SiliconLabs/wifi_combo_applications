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
#include<stdio.h>
//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h" 
#include "rsi_wlan_non_rom.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
//! Access point SSID to be created
#define SSID              "REDPINE_AP"

//! Channel number
#define CHANNEL_NO              11

//! Security type
#define SECURITY_TYPE     RSI_WPA2

//! Encryption type
#define ENCRYPTION_TYPE   RSI_CCMP

//! Password
#define PSK               "1234567890"

//! Beacon interval
#define  BEACON_INTERVAL        100

//! DTIM interval 
#define DTIM_INTERVAL           4


//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP         0x010AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 

//! Device port number
#define DEVICE_PORT       5001

//! Number of packets to receive
#define NUMBER_OF_PACKETS  1000

//! Receive data length
#define RECV_BUFFER_SIZE   1000 

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

uint8_t station_mac[6];

//! This callback function is called when Station is connected.
//! Buffer has the MAC address of the station connected
void   stations_connect_notify_handler(uint16_t status,uint8_t *buffer, const uint32_t length)
{

  memcpy(station_mac, buffer,6);
}

//! This callback function is called when Station is disconnected.
//! Buffer has the MAC address of the station disconnected
void   stations_disconnect_notify_handler(uint16_t status,uint8_t *buffer, const uint32_t length)
{

}



int32_t rsi_ap_start()
{
  int32_t     server_socket, new_socket;
  struct      rsi_sockaddr_in server_addr, client_addr;
  int32_t     status       = RSI_SUCCESS;
  int32_t     packet_count = 0, recv_size  = 0, addr_size;
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
  //! buffer to receive data over TCP client socket
  int8_t recv_buffer[RECV_BUFFER_SIZE];

  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, stations_connect_notify_handler);
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, stations_disconnect_notify_handler);


  //! WC initialization
  status = rsi_wireless_init(6, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  //! Configure IP 
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
  
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  //! Start Access point
  status =  rsi_wlan_ap_start((int8_t *)SSID, CHANNEL_NO, SECURITY_TYPE, ENCRYPTION_TYPE, PSK, BEACON_INTERVAL, DTIM_INTERVAL);
    if(status != RSI_SUCCESS)
    {
      return status;
    }

	//! Start WPS mode
	status = rsi_wlan_wps_push_button_event((int8_t *)SSID);
    
	if(status != RSI_SUCCESS)
    {
      return status;
    }

  //! Create socket
  server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket < 0)
  {
    status = rsi_wlan_get_status();
    return status;
  }

  //! Set server structure
  memset(&server_addr, 0, sizeof(server_addr));  

  //! Set family type
  server_addr.sin_family= AF_INET;

  //! Set local port number
  server_addr.sin_port = htons(DEVICE_PORT);



  //! Bind socket
  status = rsi_bind(server_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(server_socket, 0);
    return status;
  }

  //! Socket listen
  status = rsi_listen(server_socket, 1);
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(server_socket, 0);
    return status;
  }

  addr_size = sizeof(server_socket);

  //! Socket accept
  new_socket = rsi_accept(server_socket, (struct rsi_sockaddr *)&client_addr, &addr_size);
  if(new_socket < 0)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(server_socket, 0);
    return status;
  }

  while(packet_count < NUMBER_OF_PACKETS)
  {
    recv_size = RECV_BUFFER_SIZE;

    do
    {
      //! Receive data on socket
      status = rsi_recvfrom(new_socket, recv_buffer, recv_size, 0, (struct rsi_sockaddr *)&client_addr, &addr_size);
      if(status < 0)
      {
        status = rsi_wlan_socket_get_status(new_socket);
        if(status == RSI_RX_BUFFER_CHECK)
        {
          continue;
        }
        rsi_shutdown(server_socket, 0);
        return status;
      }

      //! subtract received bytes 
      recv_size -= status;

    } while(recv_size > 0);

    packet_count++;
  }
  
  //! Disconnect the station which is connected
  status = rsi_wlan_disconnect_stations(station_mac);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  return 0;
}



void main_loop(void)
{
  while(1)
  {
    ////////////////////////
    //! Application code ///
    ////////////////////////

    //! event loop 
	rsi_wireless_driver_task();

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

   //! WiSeconnect/WiSeMCU intialisation
   status = rsi_device_init(LOAD_NWP_FW);
   if(status != RSI_SUCCESS)
   {
     return status;
   }

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_ap_start, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call TCP server application in AP mode
  status = rsi_ap_start();

  //! Application main loop
  main_loop();
#endif
  return status;

}

