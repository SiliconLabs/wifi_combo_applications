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
#include "rsi_driver.h"

//! COMMON include file to refer wlan APIs

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h" 
#include "rsi_wlan_non_rom.h"

#include "rsi_wlan_non_rom.h"
//! socket include file to refer socket APIs
#include "rsi_socket.h"

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

//! Device port number
#define DEVICE_PORT       5001

//! Server port number
#define SERVER_PORT       5001

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x010AA8C0

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

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
void rsi_wlan_async_module_state(uint16_t status, const uint8_t *payload, const uint16_t payload_length);
void rsi_wlan_status(int state);

int32_t rsi_wlan_async_stats()
{
  int32_t     client_socket;
  struct      rsi_sockaddr_in server_addr, client_addr;
  int32_t     status       = RSI_SUCCESS;
  int32_t     packet_count = 0;
  uint8_t max_tcp_retry = RSI_MAX_TCP_RETRIES;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif

	rsi_wlan_register_callbacks(RSI_WLAN_ASYNC_STATS, rsi_wlan_async_module_state);

  //! WC initialization
  status = rsi_wireless_init(0, 0);
	rsi_wlan_status(rsi_wlan_get_state());
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Scan for Access points
  status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
	rsi_wlan_status(rsi_wlan_get_state());
  if(status != RSI_SUCCESS)
  {
    return status;
  }
 
  //! Connect to an Access point
  status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
	rsi_wlan_status(rsi_wlan_get_state());
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
	rsi_wlan_status(rsi_wlan_get_state());
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
  rsi_task_create(rsi_tcp_client, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call rsi_wlan_async_stats application
  status = rsi_wlan_async_stats();

  //! Application main loop
  main_loop();
#endif
 return status;

}

void rsi_wlan_async_module_state(uint16_t status, const uint8_t *payload, const uint16_t payload_length)
{
    int i = 0, j = 0;
    char *unknown = "unknown";
    char *higher_nibble = unknown;
    char *lower_nibble = unknown;
    char *reason_code = unknown;
    uint8_t bssid_string[18];

    if(payload_length != sizeof(rsi_state_notification_t))
        
    return;

    rsi_state_notification_t *state = (rsi_state_notification_t*) payload;

    for(i = 0; i < WLAN_MODULE_STATES; i++)
    {
        //higher nibble information
        if(STATE[i].bit == (state->StateCode & 0xF0))
        {
            higher_nibble = STATE[i].string;
        }
        //lower nibble information
        if(STATE[i].bit == (state->StateCode & 0x0F))
        {
            lower_nibble = STATE[i].string;
        }
    }

    for(j = 0; j < WLAN_REASON_CODES; j++)
    {
        //!reason code info
        if(REASONCODE[j].bit == (state->reason_code & 0xFF))
        {
            reason_code = REASONCODE[j].string;
        }
    }

    if(higher_nibble == unknown && lower_nibble == unknown  && reason_code == unknown)
        return;


    rsi_6byte_dev_address_to_ascii((int8_t *)bssid_string, state->rsi_bssid);

    //display different content based on the information provided
    if(state->rsi_channel == 0 && state->rsi_rssi == 100)
    {
        LOG_PRINT("Module state: \n\"%s\" reason: \"%s\" (%s)\n"
        ,higher_nibble, lower_nibble, bssid_string);
    }
    else if(state->rsi_channel == 0 && state->rsi_rssi != 100)
    {
        LOG_PRINT("Module state: \n\"%s\" reason: \"%s\" rssi: -%u (%s)\n"
        ,higher_nibble, lower_nibble, state->rsi_rssi, bssid_string);
    }
    else if(state->rsi_channel != 0 && state->rsi_rssi == 100)
    {
        LOG_PRINT("Module state: \n\"%s\" reason: \"%s\" chan: %u (%s)\n"
        ,higher_nibble, lower_nibble, state->rsi_channel, bssid_string);
    }
    else
    {
        LOG_PRINT("Module state: \"%s\" reason: \"%s\" Reason_code: \"%s\" chan: %u rssi: -%u (%s)\n"
        ,higher_nibble, lower_nibble, reason_code, state->rsi_channel, state->rsi_rssi, bssid_string);
    }
}


void rsi_wlan_status(int state)
{
  switch(state)
  {
    case 0: 
      LOG_PRINT("RSI_WLAN_STATE_NONE\n");
      break;
    case 1: 
      LOG_PRINT("RSI_WLAN_STATE_OPERMODE_DONE\n");
      break;
    case 2:
      LOG_PRINT("RSI_WLAN_STATE_BAND_DONE\n");
      break;
    case 3:
      LOG_PRINT("RSI_WLAN_STATE_INIT_DONE\n");
      break;
    case 4:
      LOG_PRINT("RSI_WLAN_STATE_SCAN_DONE\n");
      break;
    case 5: 
      LOG_PRINT("RSI_WLAN_STATE_CONNECTED\n");
      break;
    case 6: 
      LOG_PRINT("RSI_WLAN_STATE_IP_CONFIG_DONE\n");
      break;
    case 7: 
      LOG_PRINT("RSI_WLAN_STATE_IPV6_CONFIG_DONE\n");
      break;
    case 8: 
      LOG_PRINT("RSI_WLAN_STATE_AUTO_CONFIG_GOING_ON\n");
      break;
    case 9: 
      LOG_PRINT("RSI_WLAN_STATE_AUTO_CONFIG_DONE\n");
      break;
    case 10:
      LOG_PRINT("RSI_WLAN_STATE_AUTO_CONFIG_FAILED\n");
      break;
  }
}

