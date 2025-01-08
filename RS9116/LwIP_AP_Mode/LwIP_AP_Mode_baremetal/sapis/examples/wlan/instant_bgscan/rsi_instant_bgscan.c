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

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_utils.h"
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

//! Device port number
#define DEVICE_PORT       5001

//! Server port number
#define SERVER_PORT       5001

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x640AA8C0

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Memory length for driver
#define GLOBAL_BUFF_LEN   15000

#define APP_BUFF_LEN   200
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

uint8_t bgscan_results[APP_BUFF_LEN];

int32_t rsi_instant_bgscan()
{
  int32_t     status       = RSI_SUCCESS;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif




  //! WC initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Scan for Access points
  status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Connect to an Access point
  status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Configure IP 
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0,0);
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  status = rsi_wlan_bgscan_profile(RSI_INSTANT_BGSCAN,(rsi_rsp_scan_t *) bgscan_results, APP_BUFF_LEN);
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

    //! Redpine module intialisation
    status = rsi_device_init(LOAD_NWP_FW);
    if(status != RSI_SUCCESS)
    {
      return status;
    }

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_instant_bgscan, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call instant bg scan application
  status = rsi_instant_bgscan();

  //! Application main loop
  main_loop();
#endif
 return status;

}

