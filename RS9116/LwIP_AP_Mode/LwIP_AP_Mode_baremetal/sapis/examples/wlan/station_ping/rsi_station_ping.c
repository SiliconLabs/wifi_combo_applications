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

/*================================================================================
 * @brief : This file contains example application for Station Ping
 * @section Description :
 * The application demonstrates how to configure Redpine device in client mode to
 * send ping request to target IP address.
 =================================================================================*/

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

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_utils.h"
//! configurattion Parameters

//! Access point SSID to connect
#define SSID              "REDPINE_AP"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               NULL

//! To provide the cmd type for wlan set 
#define CMD_TYPE         RSI_JOIN_BSSID

//! To configure the AP mac
#define AP_BSSID         {0x10,0xfe,0xed,0xe9,0xa9,0x30}

//! IP address of the remote device 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define REMOTE_IP              0x9200A8C0

//! ping size
#define PING_SIZE              100

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE         1 

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP              0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY                0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK                0x00FFFFFF 

#endif

//! Number of packet to send
#define NUMBER_OF_PACKETS        20

//! Memory length for driver
#define GLOBAL_BUFF_LEN        15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! connect through PMK
//! To Enable keep 1 else 0
#define CONNECT_WITH_PMK            0 

//! Type
#define PMK_TYPE                    3 

//!size of pmk
#define PMK_SIZE                    32

//! standard defines

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];
uint8_t ping_rsp_received;
void rsi_ping_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length);
static void main_loop(void);

//! Ping Application in station mode
int32_t rsi_station_ping_app()
{
	int32_t     status            = RSI_SUCCESS;
	uint8_t     join_bssid[6]     = AP_BSSID;
	uint32_t    remote_ip_addr    = REMOTE_IP;
	uint16_t     size             = PING_SIZE;
	int32_t     packet_count      = 0;
#if !(DHCP_MODE) 
	uint32_t    ip_addr             = DEVICE_IP;
	uint32_t    network_mask        = NETMASK;
	uint32_t    gateway             = GATEWAY;
#else
	uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
#if CONNECT_WITH_PMK
  uint8_t pmk[PMK_SIZE];
#endif

	//! WC initialization
	status = rsi_wireless_init(0, 0);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWireless Initialization Success\r\n");
	}

	//! enable debug log prints
	status = rsi_common_debug_log(0,15);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nEnable debug log prints Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nEnable debug log prints Failed Success\r\n");
	}

	//! Scan for Access points
	status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN AP Scan Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWLAN AP Scan Success\r\n");
	}

#if CONNECT_WITH_PMK
	//! Generate pmk
	status = rsi_wlan_pmk_generate(PMK_TYPE, (int8_t *)PSK, (int8_t *)SSID, pmk, PMK_SIZE);
	if(status !=RSI_SUCCESS)
	{
		LOG_PRINT("\r\nGenerate pmk Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nGenerate pmk Success\r\n");
	}
#endif

	//!To set the wlan option
	status=rsi_wlan_set(CMD_TYPE,join_bssid,6);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nSet WLAN Option Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nSet WLAN Option Success\r\n");
	}

#if CONNECT_WITH_PMK
	//! Connect to an Access point through PMK method
	if(SECURITY_TYPE == RSI_WPA2_PMK)
	{
		status = rsi_wlan_connect(SSID, SECURITY_TYPE, pmk);
	}
#else
	//! Connect to an Access point
	status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN AP Connect Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWLAN AP Connect Success\r\n");
	}
#endif

	//! Configure IP 
#if DHCP_MODE
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0, 0);
#else
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
#endif
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%X\r\n", status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nIP Config Success\r\n");
	}

	while(packet_count < NUMBER_OF_PACKETS)
	{


		status = rsi_wlan_ping_async(0, (uint8_t *)&remote_ip_addr, size, rsi_ping_response_handler);
		ping_rsp_received = 0;
		packet_count++;

#ifndef RSI_WITH_OS
		//! wait for Driver task to complete
		main_loop();
#else
		while(!ping_rsp_received);
#endif

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

		if(ping_rsp_received == 1)
		{
			ping_rsp_received = 0;
			break;
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
		LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nDevice Initialization Success\r\n");
	}

#ifdef RSI_WITH_OS
	//! OS case
	//! Task created for WLAN task
	rsi_task_create(rsi_station_ping_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call Ping application
	status = rsi_station_ping_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}

//! ping response notify call back handler 
void rsi_ping_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length)
{
	ping_rsp_received = 1;
}
