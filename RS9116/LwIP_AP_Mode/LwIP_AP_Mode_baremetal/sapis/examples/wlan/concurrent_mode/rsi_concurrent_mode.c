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
#include "rsi_wlan_non_rom.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"
#include "rsi_utils.h"
//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Access point SSID to connect
#define SSID                  "REDPINE"

//! Security type Access Point to connect
#define STA_SECURITY_TYPE     RSI_OPEN

//! Password of AP to connect
#define STA_PSK          "1234567890"

//! SSID of Access point to be created
#define AP_SSID                  "REDPINE_AP"

//! channel number of created Access point. Configure same channel number of AP which STA is connecting.
#define AP_CHANNEL_NO       6

//! Security type of created Access point
#define AP_SECURITY_TYPE     RSI_WPA2

//! Encryption type of created Access point
#define AP_ENCRYPTION_TYPE     RSI_CCMP

//! Password of created Access point
#define AP_PSK               "1234567890"


//! Beacon Interval of created Access point
#define BEACON_INTERVAL        100

//! DTIM Count of created Access Point
#define DTIM_COUNT             4

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE             1

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP             0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY               0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK               0x00FFFFFF 

#endif

//! Device port number
#define DEVICE_PORT       5001

//! Server port number
#define SERVER_PORT       5001

//! Server IP address. Should be in reverse long format
//! E.g: 0x7800A8C0 == 192.168.0.120
#define SERVER_IP_ADDRESS 0x6B01A8C0

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Memory length for driver
#define GLOBAL_BUFF_LEN 15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500
//! vap id to create socket

#define RSI_VAP_ID  0  

#include<string.h>

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

int32_t rsi_concurrent_mode()
{
  int32_t     client_socket;
  int8_t      vap_id=RSI_VAP_ID;
  struct      rsi_sockaddr_in server_addr, client_addr;
  int32_t     status       = RSI_SUCCESS;
  int32_t     packet_count = 0;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
   uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif

  //! WC initialization
  status = rsi_wireless_init(9, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Connect to an Acces point
  status = rsi_wlan_connect((int8_t *)SSID, STA_SECURITY_TYPE, STA_PSK);
  if(status != RSI_SUCCESS)
  {

    return status;
  }


  //! Configure IP for station mode 
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0, 0);
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
#endif

  if(status != RSI_SUCCESS)
    {
      return status;
    }

  //! Start Access point
  status =  rsi_wlan_ap_start((int8_t *)AP_SSID, AP_CHANNEL_NO, AP_SECURITY_TYPE, AP_ENCRYPTION_TYPE, AP_PSK, BEACON_INTERVAL, DTIM_COUNT);
  if(status != RSI_SUCCESS)
  {

    return status;
  }
  //! Create socket
   client_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
   if(client_socket < 0)
   {
     status = rsi_wlan_get_status();
     return status;
   }

   //! Memset client structrue
   memset(&client_addr, 0, sizeof(client_addr));

   //! Set family type
   client_addr.sin_family= AF_INET;

   //! Set local port number
   client_addr.sin_port = htons(DEVICE_PORT);

   //! Bind socket
   status = rsi_bind(client_socket, (struct rsi_sockaddr *) &client_addr, sizeof(client_addr));
   if(status != RSI_SUCCESS)
   {
     status = rsi_wlan_get_status();
     rsi_shutdown(client_socket, 0);
     return status;
   }
 
   //! Set server structure
   memset(&server_addr, 0, sizeof(server_addr));

   //! Set server address family
   server_addr.sin_family = AF_INET;

   //! Set server port number, using htons function to use proper byte order
   server_addr.sin_port = htons(SERVER_PORT);

   //! Set IP address to localhost
   server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

   status=rsi_setsockopt(client_socket, SOL_SOCKET, SO_SOCK_VAP_ID, &vap_id, sizeof(vap_id));
   if(status != RSI_SUCCESS)
   {
     return status;
   }

   //! Connect to server socket
   status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
   if(status != RSI_SUCCESS)
   {
     status = rsi_wlan_get_status();
     rsi_shutdown(client_socket, 0);
     return status;
   }

   while(packet_count < NUMBER_OF_PACKETS)
   {
     //! Send data on socket
     status = rsi_send(client_socket,(int8_t *)"Hello from TCP client!!!", (sizeof("Hello from TCP client!!!") - 1), 0);
     if(status < 0)
     {
       status = rsi_wlan_get_status();
       rsi_shutdown(client_socket, 0);
       return status;
     }

     packet_count++;
   }

  rsi_shutdown(client_socket, 0);
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

   //! Redpine module intialisation
   status = rsi_device_init(LOAD_NWP_FW);
   if(status != RSI_SUCCESS)
   {
     return status;
   }

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_concurrent_mode, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call concurrent mode  application
  status = rsi_concurrent_mode();

  //! Application main loop
  main_loop();
#endif
return status;
}
