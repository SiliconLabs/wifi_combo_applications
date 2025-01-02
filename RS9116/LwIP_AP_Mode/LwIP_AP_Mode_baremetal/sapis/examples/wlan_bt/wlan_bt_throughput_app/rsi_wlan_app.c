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

//! WLAN include file for configuration
#include "rsi_wlan_config.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"
#include "rsi_utils.h"
//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#ifdef RSI_M4_INTERFACE
#include "rsi_chip.h"
#endif


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

//! If DHCP mode is disabled give IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP         0x6500A8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 

#endif

//! port number
#define PORT_NUM       5001

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x640AA8C0

//! Memory length for driver
#define GLOBAL_BUFF_LEN   15000

//! Memory length for send buffer
#define BUF_SIZE   1400

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   1

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! client, server and new socket ids

int32_t     client_socket, server_socket, new_socket;

//! client and server ip addresses 
struct      rsi_sockaddr_in server_addr, client_addr;


#ifdef RSI_M4_INTERFACE
#define ICACHE2_ADDR_TRANSLATE_1_REG  *(volatile uint32_t *)(0x20280000 + 0x24)
#define MISC_CFG_SRAM_REDUNDANCY_CTRL *(volatile uint32_t *)(0x46008000 + 0x18)
#define MISC_CONFIG_MISC_CTRL1        *(volatile uint32_t *)(0x46008000 + 0x44)
#define MISC_QUASI_SYNC_MODE          *(volatile uint32_t *)(0x46008000 + 0x84)

#define SOC_PLL_REF_FREQUENCY         40000000  /*<! PLL input REFERENCE clock 40MHZ */ 
#define PS4_SOC_FREQ                  119000000 /*<! PLL out clock 100MHz            */
void switch_m4_frequency();
#endif

//! Type of throughput
#define  UDP_TX                     0
#define  UDP_RX                     1
#define  TCP_TX                     2
#define  TCP_RX                     3

#define SOCKET_ASYNC_FEATURE        1
//! Power measurement type
#define THROUGHPUT_TYPE  UDP_TX

//! Memory to initialize driver
uint32_t total_tx_bytes = 0;
uint32_t total_rx_bytes;
uint32_t secs;
static uint32_t last_tx_print_time = 0;
static uint32_t last_rx_print_time;

//extern volatile uint32_t _dwTickCount;

uint32_t received_length = 0;

//! Call back for Socket Async
void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	received_length += length;
}



void measure_throughput(uint32_t  pkt_length, uint32_t tx_rx)
{
	static uint32_t current_time;
	static uint32_t last_print_time;
	uint32_t total_bytes;
	float through_put;
#ifdef LINUX_PLATFORM
	struct rsi_timeval tv1;
	gettimeofday(&tv1, NULL);
	current_time = tv1.tv_sec * 1000000 + tv1.tv_usec;
#endif
	if(tx_rx == 0)
	{
		total_tx_bytes += pkt_length;
		total_bytes = total_tx_bytes;
		last_print_time = last_tx_print_time;
	}
	else
	{
		total_rx_bytes += pkt_length;
		total_bytes = total_rx_bytes;
		last_print_time = last_rx_print_time;
	}
#ifdef LINUX_PLATFORM
if((current_time-last_print_time)>=1000000)    //!for 1 sec
#else
	if((current_time-last_print_time)>=30000)    //!for 30 sec
#endif
	{
#ifdef LINUX_PLATFORM
		through_put = ((float)(total_bytes)/((current_time-last_print_time))) * 8;
#else
		through_put = ((float)(total_bytes)/((current_time-last_print_time))) * 8;
		through_put /= 1000;
#endif
		if(tx_rx == 0)
		{
#ifdef LINUX_PLATFORM
			printf("\nSecs: %d     Bytes Transmitted %d,Throughput for last %d seconds is = %3.1f Mbps\n",secs++,total_bytes,(current_time-last_print_time)/1000000, through_put);
#endif
			last_tx_print_time = current_time;
			total_tx_bytes = 0;
		}
		else
		{
#ifdef LINUX_PLATFORM
			printf("\nSecs: %d     Bytes Received %d,Throughput for last %d seconds is = %3.1f Mbps\n",secs++,total_bytes,(current_time-last_print_time)/1000000, through_put);
#endif
			last_rx_print_time = current_time;
			total_rx_bytes = 0;
		}
	}
return;
}

int32_t rsi_wlan_app_task()
{
  int32_t     status       = RSI_SUCCESS;
  uint32_t    recv_size = 0;
  int32_t     addr_size;
  uint8_t     high_performance_socket = RSI_HIGH_PERFORMANCE_SOCKET;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);

#endif
  int8_t     send_buf[BUF_SIZE];
  uint16_t    i = 0;

	//! buffer to receive data over TCP/UDP client socket
  int8_t recv_buffer[BUF_SIZE];

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
		printf("Error \n");
		return status;
	}

	for (i = 0 ; i < BUF_SIZE; i++)
	{
		send_buf[i] = i;
	}
//case RSI_WLAN_IPCONFIG_DONE_STATE:
  {
	switch(THROUGHPUT_TYPE)
	{
	case UDP_TX:
	{
		//! Create socket
		client_socket = rsi_socket(AF_INET, SOCK_DGRAM, 0);
		if(client_socket < 0)
		{
			status = rsi_wlan_get_status();
			return status;
		}

		//! Set server structure
		memset(&server_addr, 0, sizeof(server_addr));

		//! Set server address family
		server_addr.sin_family = AF_INET;

		//! Set server port number, using htons function to use proper byte order
		server_addr.sin_port = htons(PORT_NUM);

		//! Set IP address to localhost
		server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

		while(1)
		{
			//! Send data on socket
			status = rsi_sendto(client_socket, send_buf, BUF_SIZE, 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
			if(status < 0)
			{
				status = rsi_wlan_get_status();
				rsi_shutdown(client_socket, 0);
				return status;
			}

			//! Measure throughput per second, 0 - Tx and 1 - Rx
			measure_throughput(BUF_SIZE, 0);

		}
	}
	case UDP_RX:
	{
		//! Create socket
#if SOCKET_ASYNC_FEATURE
		server_socket = rsi_socket_async(AF_INET, SOCK_DGRAM, 0, socket_async_recive);
#else    
		server_socket = rsi_socket(AF_INET, SOCK_DGRAM, 0);
#endif    
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
		server_addr.sin_port = htons(PORT_NUM);

		//! Bind socket
		status = rsi_bind(server_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
		if(status != RSI_SUCCESS)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			return status;
		}

		addr_size = sizeof(server_addr);

#if !SOCKET_ASYNC_FEATURE
    
		while(1)
		{
			recv_size = BUF_SIZE;

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
					return status;
				}

				//! subtract received bytes
				recv_size -= status;

				//! Measure throughput per second, 0 - Tx and 1 - Rx
				measure_throughput(status, 1);

			} while(recv_size > 0);

		}
#endif    
	}
	case TCP_TX:
	{
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
		client_addr.sin_port = htons(PORT_NUM);

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
		server_addr.sin_port = htons(PORT_NUM);

		//! Set IP address to localhost
		server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

		//! Connect to server socket
		status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
		if(status != RSI_SUCCESS)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(client_socket, 0);
			return status;
		}

		while(1)
		{
			//! Send data on socket
			status = rsi_send(client_socket,send_buf, BUF_SIZE, 0);
			if(status < 0)
			{
				status = rsi_wlan_get_status();
				rsi_shutdown(client_socket, 0);
				return status;
			}

			//! Measure throughput per second, 0 - Tx and 1 - Rx
			measure_throughput(BUF_SIZE, 0);

		}
	}
	case TCP_RX:
	{
#if HIGH_PERFORMANCE_ENABLE

        status = rsi_socket_config();
		if(status < 0)
		{
			status = rsi_wlan_get_status();
			return status;
		}
		
#endif 	
	
		//! Create socket
#if SOCKET_ASYNC_FEATURE
		server_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 0, socket_async_recive);
#else    
		server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
#endif    
		if(server_socket < 0)
		{
			status = rsi_wlan_get_status();
			return status;
    }
    status=rsi_setsockopt(server_socket, SOL_SOCKET, SO_HIGH_PERFORMANCE_SOCKET, &high_performance_socket, sizeof(high_performance_socket));
    if(status != RSI_SUCCESS)
    {
      return status;
    }

		//! Set server structure
		memset(&server_addr, 0, sizeof(server_addr));

		//! Set family type
		server_addr.sin_family= AF_INET;

		//! Set local port number
		server_addr.sin_port = htons(PORT_NUM);


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
#if !SOCKET_ASYNC_FEATURE
		while(1)
		{
			recv_size = BUF_SIZE;

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

				//! Measure throughput per second, 0 - Tx and 1 - Rx
				measure_throughput(status, 1);

			} while(recv_size > 0);

		}
#endif
	}
	}
	return 0;
  }
}

