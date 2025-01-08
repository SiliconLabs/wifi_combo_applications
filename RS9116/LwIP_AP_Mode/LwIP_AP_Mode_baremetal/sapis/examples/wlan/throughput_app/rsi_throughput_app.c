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
 * @brief : This file contains example application for Throughput Measurement
 * @section Description :
 * This application will demonstrate the throughput measurement, measuring the
 * throughput from Client to Server and vice versa.
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

//! WLAN include file for configuration
#include "rsi_wlan_config.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

#define TX_RX_RATIO_ENABLE          1

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#ifndef LINUX_PLATFORM
#include "rsi_hal.h"
#endif

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
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

#define RSI_HIGH_PERFORMANCE_SOCKET         1
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

//Module port number
#define PORT_NUM          5001

//! Server port number
#define SERVER_PORT       5001

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


#ifdef RSI_M4_INTERFACE
#define ICACHE2_ADDR_TRANSLATE_1_REG  *(volatile uint32_t *)(0x20280000 + 0x24)
#define MISC_CFG_SRAM_REDUNDANCY_CTRL *(volatile uint32_t *)(0x46008000 + 0x18)
#define MISC_CONFIG_MISC_CTRL1        *(volatile uint32_t *)(0x46008000 + 0x44)
#define MISC_QUASI_SYNC_MODE          *(volatile uint32_t *)(0x46008000 + 0x84)

#define SOC_PLL_REF_FREQUENCY         40000000  /*<! PLL input REFERENCE clock 40MHZ */ 
#define PS4_SOC_FREQ                  119000000 /*<! PLL out clock 100MHz            */
void switch_m4_frequency(void);
#endif

//! Type of throughput
#define  UDP_TX                     0
#define  UDP_RX                     1
#define  TCP_TX                     2
#define  TCP_RX                     3
#define  SSL_TX                     4
#define  SSL_RX                     5

#define SOCKET_ASYNC_FEATURE        1
//! Power measurement type
//#define THROUGHPUT_TYPE  UDP_TX
//#define THROUGHPUT_TYPE  UDP_RX
//#define THROUGHPUT_TYPE  TCP_RX
#define THROUGHPUT_TYPE  TCP_TX
//#define THROUGHPUT_TYPE  SSL_TX
//#define THROUGHPUT_TYPE  SSL_RX

#if((THROUGHPUT_TYPE  == SSL_TX) || (THROUGHPUT_TYPE  == SSL_RX))
#define SSL               			1
//! Load certificate to device flash :
//! Certificate could be loaded once and need not be loaded for every boot up
#define LOAD_CERTIFICATE  			1

#endif
#define RSI_SSL_BIT_ENABLE  		0

//! Throughput parameters
uint64_t num_bits = 0;
uint32_t xfer_time;
uint32_t t_start = 0;
uint32_t t_end;
float throughput;

volatile uint8_t data_recvd = 0;
volatile uint64_t num_bytes = 0;

//! Memory to initialize driv
uint8_t global_buf[GLOBAL_BUFF_LEN];
uint32_t total_tx_bytes = 0;
uint32_t total_rx_bytes;
uint32_t secs;
static uint32_t last_tx_print_time = 0;
static uint32_t last_rx_print_time;
uint8_t ip_buff[20];


#if SSL
//! Include SSL CA certificate
#include "cacert.pem"
#endif

extern void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length);

//! Call back for Socket Async
void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	num_bytes += length;
}

/*====================================================*/
/**
 * @fn         void Throughput(void)
 * @brief      Function to calculate throughput
 * @param[in]  void
 * @return     void
 * @section description
 *====================================================*/
void compute_throughput(void)
{
	num_bits = num_bytes * 8;                   //! number of bits
	xfer_time = ((t_end - t_start)/1000); 	            //! data transfer time
	throughput = (((float)(num_bits)/xfer_time)/1000000); //!Throughput calculation
#ifdef DEBUG_UART
	LOG_PRINT("throughput in mbps=%f",(throughput));
#endif

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
#else
	current_time = rsi_hal_gettickcount();  
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

int32_t rsi_throughput_app()
{
	int32_t     client_socket;
	int32_t     server_socket, new_socket;
	struct      rsi_sockaddr_in server_addr, client_addr;
	int32_t     status       = RSI_SUCCESS;
#if !SOCKET_ASYNC_FEATURE	
	uint32_t    recv_size = 0;
	//! buffer to receive data over TCP/UDP client socket
	int8_t recv_buffer[BUF_SIZE];
#endif
	int32_t     addr_size;
  uint8_t     high_performance_socket = RSI_HIGH_PERFORMANCE_SOCKET;
  uint8_t     ssl_bit_map = RSI_SSL_BIT_ENABLE ; 
#if !(DHCP_MODE)
	uint32_t    ip_addr      = DEVICE_IP;
	uint32_t    network_mask = NETMASK;
	uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
	int8_t     send_buf[BUF_SIZE];
	uint16_t    i = 0;




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
	//! Send feature frame
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nSend Feature Frame Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nSend Feature Frame Success\r\n");
	}

	rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);       //! Initialize remote terminate call back

#if LOAD_CERTIFICATE
#if SSL
	//! Load SSL CA certificate
	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, cacert, (sizeof(cacert)-1));
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nLoad SSL CA Certificate Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nLoad SSL CA Certificate Success\r\n");
	}
#endif
#endif

#if TX_RX_RATIO_ENABLE
  status = rsi_wlan_buffer_config();
	if(status != RSI_SUCCESS)
	{
		return status;
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
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, ip_buff, sizeof(ip_buff),0);
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

	for (i = 0 ; i < BUF_SIZE; i++)
	{
		send_buf[i] = i;
	}

	switch(THROUGHPUT_TYPE)
	{
	case UDP_TX:
	{
		LOG_PRINT("\r\nUDP_TX TEST\r\n");
		//! Create socket
		client_socket = rsi_socket(AF_INET, SOCK_DGRAM, 0);
		if(client_socket < 0)
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

		//! Set server address family
		server_addr.sin_family = AF_INET;

		//! Set server port number, using htons function to use proper byte order
		server_addr.sin_port = htons(SERVER_PORT);

		//! Set IP address to localhost
		server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

		LOG_PRINT("\r\nUDP TX start\r\n");
		while(1)
		{
			//! Send data on socket
			status = rsi_sendto(client_socket, send_buf, BUF_SIZE, 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
			if(status < 0)
			{
				status = rsi_wlan_get_status();
				rsi_shutdown(client_socket, 0);
				LOG_PRINT("\r\nFailed to send data to UDP Server, Error Code : 0x%X\r\n",status);
				return status;
			}

			//! Measure throughput per second, 0 - Tx and 1 - Rx
			measure_throughput(BUF_SIZE, 0);

		}
	}
	case UDP_RX:
	{
		LOG_PRINT("\r\nUDP_RX TEST\r\n");
		//! Create socket
#if SOCKET_ASYNC_FEATURE
		server_socket = rsi_socket_async(AF_INET, SOCK_DGRAM, 0, socket_async_recive);
#else    
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
		server_addr.sin_port = htons(SERVER_PORT);

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

		addr_size = sizeof(server_addr);

		LOG_PRINT("\r\nUDP RX start\r\n");
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
					LOG_PRINT("\r\nFailed to receive data from UDP client, Error Code : 0x%X\r\n",status);
					return status;
				}

				//! subtract received bytes
				recv_size -= status;

				//! Measure throughput per second, 0 - Tx and 1 - Rx
				measure_throughput(status, 1);

			} while(recv_size > 0);

		}
#endif
   break;    
	}
	case TCP_TX:
	{
		LOG_PRINT("\r\nTCP_TX TEST\r\n");
		//! Create socket
		client_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
		if(client_socket < 0)
		{
			status = rsi_wlan_get_status();
			LOG_PRINT("\r\nSocket Create Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSocket Create Success\r\n");
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
			LOG_PRINT("\r\nBind Failed, Error code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nBind Success\r\n");
		}

		//! Set server structure
		memset(&server_addr, 0, sizeof(server_addr));

		//! Set server address family
		server_addr.sin_family = AF_INET;

		//! Set server port number, using htons function to use proper byte order
		server_addr.sin_port = htons(SERVER_PORT);

		//! Set IP address to localhost
		server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

		//! Connect to server socket
		status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
		if(status != RSI_SUCCESS)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(client_socket, 0);
			LOG_PRINT("\r\nConnect to Server Socket Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nConnect to Server Socket Success\r\n");
		}

		LOG_PRINT("\r\nTCP TX Start\r\n");

		while(1)
		{
			//! Send data on socket
			status = rsi_send(client_socket,send_buf, BUF_SIZE, 0);
			if(status < 0)
			{
				status = rsi_wlan_get_status();
				rsi_shutdown(client_socket, 0);
				LOG_PRINT("\r\nFailed to Send data to TCP Server, Error Code : 0x%X\r\n",status);
				return status;
			}

			//! Measure throughput per second, 0 - Tx and 1 - Rx
			measure_throughput(BUF_SIZE, 0);

		}
	}
	case TCP_RX:
	{
		LOG_PRINT("\r\nTCP RX Test\r\n");
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
			LOG_PRINT("\r\nSocket Create Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSocket Create Success\r\n");
		}

		status=rsi_setsockopt(server_socket, SOL_SOCKET, SO_HIGH_PERFORMANCE_SOCKET, &high_performance_socket, sizeof(high_performance_socket));
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("\r\nSet Socket Option Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSet Socket Option Success\r\n");
		}

		//! Set server structure
		memset(&server_addr, 0, sizeof(server_addr));

		//! Set family type
		server_addr.sin_family= AF_INET;

		//! Set local port number
		server_addr.sin_port = htons(SERVER_PORT);

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

		//! Socket listen
		status = rsi_listen(server_socket, 1);
		if(status != RSI_SUCCESS)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			LOG_PRINT("\r\nListen Failed, Error code : 0x%X\r\n",status);
			return status;
		}
    else
		{
			LOG_PRINT("\r\nListen Success\r\n");
		}

		addr_size = sizeof(server_socket);

		//! Socket accept
		new_socket = rsi_accept(server_socket, (struct rsi_sockaddr *)&client_addr, &addr_size);
		if(new_socket < 0)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(server_socket, 0);
			LOG_PRINT("\r\nSocket Accept Failed, Error code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSocket Accept Success\r\n");
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
					LOG_PRINT("\r\nTCP Recv Failed, Error code : 0x%X\r\n",status);
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
		break;
	}
	case SSL_TX:
	{
		//! Create socket
		client_socket = rsi_socket(AF_INET, SOCK_STREAM, 1);	
		if(client_socket < 0)
		{
			status = rsi_wlan_get_status();
			LOG_PRINT("\r\nSocket Create Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSocket Create Success\r\n");
		}

		status=rsi_setsockopt(client_socket, SOL_SOCKET, SO_SSL_ENABLE, &ssl_bit_map, sizeof(ssl_bit_map));
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("\r\nSet Socket Option Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSet Socket Option Success\r\n");
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
			LOG_PRINT("\r\nBind Failed, Error code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nBind Success\r\n");
		}

		//! Set server structure
		memset(&server_addr, 0, sizeof(server_addr));

		//! Set server address family
		server_addr.sin_family = AF_INET;

		//! Set server port number, using htons function to use proper byte order
		server_addr.sin_port = htons(SERVER_PORT);

		//! Set IP address to localhost
		server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

		//! Connect to server socket
		status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
		if(status != RSI_SUCCESS)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(client_socket, 0);
			LOG_PRINT("\r\nFailed to Connect to SSL Server, Error code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nConnect to SSL Server Success\r\n");
		}

		LOG_PRINT("\r\ nSSL TX Start \r\n");
		while(1)
		{
			//! Send data on socket
			status = rsi_send(client_socket,send_buf, BUF_SIZE, 0);
			if(status < 0)
			{
				status = rsi_wlan_get_status();
				rsi_shutdown(client_socket, 0);
				LOG_PRINT("\r\nFailed to Send data to SSL Server, Error Code : 0x%X\r\n",status);
				return status;
			}

			//! Measure throughput per second, 0 - Tx and 1 - Rx
			measure_throughput(BUF_SIZE, 0);
		}
	}
	case SSL_RX:
	{
		LOG_PRINT("\r\n SSL RX Test\r\n");
#if HIGH_PERFORMANCE_ENABLE
		status = rsi_socket_config();
		if(status < 0)
		{
			status = rsi_wlan_get_status();
			return status;
		}
#endif
		  //! Create socket

		client_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 1 , socket_async_recive);	
		if(client_socket < 0)
		{
			status = rsi_wlan_get_status();
			LOG_PRINT("\r\nSocket Create Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSocket Create Success\r\n");
		}
	
		status=rsi_setsockopt(client_socket, SOL_SOCKET, SO_SSL_ENABLE, &ssl_bit_map, sizeof(ssl_bit_map));
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("\r\nSet Socket Option Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSet Socket Option Success\r\n");
		}

		status = rsi_setsockopt( client_socket, SOL_SOCKET, SO_HIGH_PERFORMANCE_SOCKET, &high_performance_socket, sizeof(high_performance_socket));
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("\r\nSet Socket Option Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nSet Socket Option Success\r\n");
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
			LOG_PRINT("\r\nBind Failed, Error code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nBind Success\r\n");
		}

		//! Set server structure
		memset(&server_addr, 0, sizeof(server_addr));

		//! Set server address family
		server_addr.sin_family = AF_INET;

		//! Set server port number, using htons function to use proper byte order
		server_addr.sin_port = htons(SERVER_PORT);

		//! Set IP address to localhost
		server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

		//! Connect to server socket
		status = rsi_connect(client_socket, (struct rsi_sockaddr *) &server_addr, sizeof(server_addr));
		if(status != RSI_SUCCESS)
		{
			status = rsi_wlan_get_status();
			rsi_shutdown(client_socket, 0);
			LOG_PRINT("\r\nFailed to Connect to Server, Error code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nConnect to Server Success\r\n");
		}

		LOG_PRINT("\r\nSSL RX Start\r\n");
		t_start = rsi_hal_gettickcount();           //! capture time-stamp before data transfer is starts

		while(1)
		{
			if( data_recvd )
			{
				compute_throughput();
				break;
			}
			else
			{
				rsi_wireless_driver_task();
			}
		}	
	}

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

#ifdef RSI_M4_INTERFACE
	switch_m4_frequency();
	SysTick_Config(SystemCoreClock / 1000);

#endif

#ifdef RSI_WITH_OS	
	//! OS case
	//! Task created for WLAN task
	rsi_task_create(rsi_throughput_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call UDP client application
	status = rsi_throughput_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}

#ifdef RSI_M4_INTERFACE
void switch_m4_frequency(void)
{
	/*Switch M4 SOC clock to Reference clock*/
	/*Default keep M4 in reference clock*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);
	/*Configure the PLL frequency*/
	RSI_CLK_SetSocPllFreq(M4CLK , PS4_SOC_FREQ , SOC_PLL_REF_FREQUENCY);
	/*Switch M4 clock to PLL clock for speed operations*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);	
}
#endif

/*====================================================*/
/**
 * @fn         void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to terminate stations remote socket
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	data_recvd = 1;        					  //! Set data receive flag
	
	t_end = rsi_hal_gettickcount();           //! capture time-stamp after data transfer is completed	
}

