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
 * @brief : This file contains example application for Websocket Client
 * @section Description :
 * This application demonstrates how to configure device in client mode to open
 * Web socket to transmit data over Websocket to Web Server.
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
#include "rsi_web_socket.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_utils.h"
#include "string.h"
//! include the certificate 
#include "cacert.pem"

//! standard defines

//! to Enable IPv6 set this bit in FLAGS, Default is IPv4 
#define   IPV6           1

//! Set WEBSOCKET_SSL to use SSL feature
#define   WEBSOCKET_SSL   2
#define   CONNECTION_CLOSE_OPCODE 8
//! configurattion Parameters

//! Access point SSID to connect
#define SSID              "REDPINE_AP"


//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               NULL

//! Flags
//! for example select required flag bits,  Eg:(WEBSOCKET_SSL | IPV6)
#define FLAGS         0  

//! IP address of the remote device 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define SERVER_IP              0x6400A8C0

//! Server port number
#define SERVER_PORT            5001

//! Device port number
#define DEVICE_PORT            5002

//! webs resource name
#define WEB_SOCKET_RESOURCE_NAME  NULL

//! webs host name
#define WEB_SOCKET_HOST_NAME      NULL

//! message
#define MESSAGE                  "This is web socket test"

//! no of packets
#define NUMBER_OF_PACKETS        5

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

//! Load certificate to device flash :
//! Certificate could be loaded once and need not be loaded for every boot up
#define LOAD_CERTIFICATE  1

//! standard defines
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

#define FIN_BIT                128 

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Indicate websocket data receive
volatile uint8_t  websocket_data_received;

//! Function prototypes
void rsi_websocket_data_receive_handler(uint32_t sock_no, uint8_t *buffer, uint32_t length);

//! web socket client Application in station mode
int32_t rsi_websocket_client_app()
{
	int32_t     status            = RSI_SUCCESS;
	int32_t     packet_count      = 0,sockID;
	int8_t      flags             = FLAGS;
	uint32_t    server_ip_addr    = SERVER_IP;
	uint8_t     opcode            = 0;
#if !(DHCP_MODE) 
	uint32_t    ip_addr            = DEVICE_IP;
	uint32_t    network_mask       = NETMASK;
	uint32_t    gateway            = GATEWAY;
#else
	uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
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

#if LOAD_CERTIFICATE
	if(flags & WEBSOCKET_SSL)
	{

		//! Load certificates
		status  = rsi_wlan_set_certificate(5, cacert, (sizeof(cacert)-1));
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("\r\nLoad CA Certificate Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nLoad CA Certificate Success\r\n");			
		}
	}
#endif

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

	//! create web socket client and connect to server
	status = rsi_web_socket_create(flags,(uint8_t *)&server_ip_addr,(uint16_t)SERVER_PORT, (uint16_t)DEVICE_PORT,
			WEB_SOCKET_RESOURCE_NAME, WEB_SOCKET_HOST_NAME, &sockID,
			rsi_websocket_data_receive_handler);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWeb Socket Create Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWeb Socket Create Success\r\n");				
	}

	LOG_PRINT("\r\nData TX start\r\n");
	//! send data to web socket server
	while(packet_count < NUMBER_OF_PACKETS)
	{
		status = rsi_web_socket_send_async(sockID, opcode, MESSAGE, strlen((const char *)MESSAGE));
		if(status < 0)
		{
			status = rsi_wlan_get_status();
			rsi_web_socket_close(sockID);
			LOG_PRINT("\r\nFailed to send data to Web Socket Server, Error Code : 0x%X\r\n",status);
			return status;
		}

		packet_count++;

		//! set final bit for last packet
		if(packet_count == NUMBER_OF_PACKETS - 1)
		{
			//! set Fin bit to say final packet
			opcode |= FIN_BIT;
		}
	}

	do
	{
		//! event loop 
		rsi_wireless_driver_task();

	}while(!websocket_data_received);


	//! close client websocket
	status = rsi_web_socket_close(sockID);

	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nClient Web Socket Close Failed, Error code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nClient Web Socket Close Success\r\n");
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
	rsi_task_create(rsi_websocket_client_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call web socket client application
	status = rsi_websocket_client_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}

//! websocket data receive notify call back handler 
void rsi_websocket_data_receive_handler(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
  websocket_data_received = 1;
}
