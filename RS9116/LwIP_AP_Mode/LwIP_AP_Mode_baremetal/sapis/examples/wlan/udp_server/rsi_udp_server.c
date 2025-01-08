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
 * @brief : This file contains example application for UDP Server
 * @section Description :
 * The UDP server application demonstrates how to open and use a standard UDP
 * server socket and receives data on socket sent by remote peer.
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
#include "rsi_utils.h"
//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//! Access point SSID to connect
#define SSID               "REDPINE_AP"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               NULL

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE          0

//! If DHCP mode is disabled give IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP          0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY            0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK            0x00FFFFFF 

#endif

//! local port number
#define DEVICE_PORT        5001

//! Number of packets to send or receive
#define NUMBER_OF_PACKETS  1000

//! Receive data length
#define RECV_BUFFER_SIZE   100 

//! Memory length for driver
#define GLOBAL_BUFF_LEN    15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! It should be set to 1 If data is to be received asynchronously
#define SOCKET_ASYNC  0

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! This is the call back which is called when data is received on registered socketid
void  rsi_wlan_async_data_recv(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	//! Application processes the UDP Data received 
}

int32_t rsi_udp_server()
{
	int32_t     server_socket;
	struct      rsi_sockaddr_in server_addr, client_addr;
	int32_t     status = RSI_SUCCESS;
	uint32_t    recv_size = 0, packet_count = 0;
	int32_t    addr_size;
#if !(DHCP_MODE)
	uint32_t    ip_addr      = DEVICE_IP;
	uint32_t    network_mask = NETMASK;
	uint32_t    gateway      = GATEWAY;
#else
	uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);

#endif

	//! buffer to receive data over UDP server socket
	int8_t recv_buffer[RECV_BUFFER_SIZE];

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
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0,0);
#else
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
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

#if SOCKET_ASYNC
	server_socket = rsi_socket_async(AF_INET, SOCK_DGRAM, 0,rsi_wlan_async_data_recv);
#else
	//! Create socket
	server_socket = rsi_socket(AF_INET, SOCK_DGRAM, 0);
#endif  
	if(server_socket < 0)
	{
		status = rsi_wlan_get_status();
		LOG_PRINT("\r\nSocket Create Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nSocket Create Success\r\n");				
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
		LOG_PRINT("\r\nBind Failed, Error code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nBind Success\r\n");
	}

#if (SOCKET_ASYNC == 0)
	addr_size = sizeof(server_addr);

	LOG_PRINT("\r\nUDP RX start\r\n");
	while(packet_count < NUMBER_OF_PACKETS)
	{
		recv_size = RECV_BUFFER_SIZE;

		do
		{
			//! Receive data on socket
			status = rsi_recvfrom(server_socket, recv_buffer, recv_size, 0, (struct rsi_sockaddr *)&client_addr, &addr_size);
			if(status < 0)
			{
				status = rsi_wlan_socket_get_status(server_socket);
				if(status == RSI_RX_BUFFER_CHECK)
				{
					continue;
				}
				rsi_shutdown(server_socket, 0);
				LOG_PRINT("\r\nFailed to receive data to UDP Client, Error Code : 0x%X\r\n",status);
				return status;
			}

			//! subtract received bytes 
			recv_size -= status;

		} while(recv_size > 0);

		packet_count++;
	}
#endif
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
	rsi_task_create(rsi_udp_server, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call UDP server application
	status = rsi_udp_server();

	//! Application main loop
	main_loop();
#endif
	return status;

}

