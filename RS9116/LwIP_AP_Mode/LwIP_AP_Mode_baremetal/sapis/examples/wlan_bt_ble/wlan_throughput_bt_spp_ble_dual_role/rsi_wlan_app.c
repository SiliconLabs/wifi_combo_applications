
/**
 * @file    rsi_wlan_app.c
 * @version 0.1
 * @date    01 May 2020
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
 *
 *  @brief : This file contains example application for TCP+SSL client socket
 *
 *  @section Description  This file contains example application for TCP+SSL client socket
 *
 *
 */
/*=======================================================================*/
//  ! INCLUDES
/*=======================================================================*/
#ifdef FRDM_K28F
#include "rsi_common_app.h"
#endif
#include "stdlib.h"
#include "rsi_driver.h"
#include "rsi_utils.h"
#include <rsi_wlan_non_rom.h>
//#include <rsi_bt_common_apis.h>
#include "rsi_common_config.h"
#include "rsi_wlan_config.h"
#if RSI_ENABLE_WLAN_TEST

/*=======================================================================*/
//   ! MACROS
/*=======================================================================*/
#if((THROUGHPUT_TYPE  == SSL_TX) || (THROUGHPUT_TYPE  == SSL_RX))
#define SSL               			1
#if SSL
#include "servercert.pem"                //! Include SSL CA certificate
#endif
//! Load certificate to device flash :
//! Certificate could be loaded once and need not be loaded for every boot up
#define LOAD_CERTIFICATE  			1
#endif
#define RSI_SSL_BIT_ENABLE  		0


/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/
int32_t client_socket;                              //! client socket id
//! Throughput parameters
uint32_t pkts = 0;
uint64_t num_bits = 0;
uint64_t total_bits = 0;
uint32_t xfer_time, Taskswitchtime;
uint32_t TotalTransfer=0;
uint32_t total_time = 0;
uint64_t xfer_time_usec;
uint32_t t_start = 0;
uint32_t t_end;
float throughput;
float throughput_mbps;
volatile uint8_t data_recvd = 0;
volatile uint64_t num_bytes = 0;
uint32_t	pkt_rx_cnt = 0;
uint32_t total_tx_bytes = 0;
uint32_t total_rx_bytes;
uint32_t secs;
struct rsi_sockaddr_in server_addr, client_addr;    //! server and client IP addresses
rsi_wlan_app_cb_t rsi_wlan_app_cb;                  //! application control block
//! buffer to receive data over TCP/UDP client socket
int8_t recv_buffer[BUF_SIZE];
int8_t     send_buf[BUF_SIZE];
#if RSI_ENABLE_BT_TEST
extern volatile	bool	bt_connected;
#endif
#if RSI_ENABLE_BLE_TEST
extern volatile	bool	ble_connected;
#endif

/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/
extern	rsi_semaphore_handle_t 	ble_main_task_sem, ble_slave_conn_sem, bt_app_sem, ant_app_sem, wlan_app_sem, bt_inquiry_sem, ble_scan_sem;
#if WLAN_SYNC_REQ
extern 	rsi_semaphore_handle_t	sync_coex_ble_sem, sync_coex_ant_sem, sync_coex_bt_sem;
#endif
extern	bool	rsi_ble_running, rsi_bt_running, rsi_ant_running, rsi_wlan_running, wlan_radio_initialized, powersave_cmd_given;
extern 	rsi_mutex_handle_t power_cmd_mutex;

/*=======================================================================*/
//   ! EXTERN FUNCTIONS
/*=======================================================================*/
extern uint64_t ip_to_reverse_hex(char *ip);

/*=======================================================================*/
//   ! PROCEDURES
/*=======================================================================*/
void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length);
void measure_throughput (uint32_t total_bytes, uint32_t start_time, uint32_t end_time);


/*************************************************************************/
//!  CALLBACK FUNCTIONS
/*************************************************************************/
/*====================================================*/
/**
 * @fn          void rsi_wlan_app_callbacks_init(void)
 * @brief       To initialize WLAN application callback
 * @param[in]   void
 * @return      void
 * @section description
 * This callback is used to initialize WLAN
 * ==================================================*/
void rsi_wlan_app_callbacks_init(void)
{

	rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);
	rsi_wlan_register_callbacks(RSI_JOIN_FAIL_CB, rsi_join_fail_handler);                                   //! Initialize join fail call back
	rsi_wlan_register_callbacks(RSI_IP_FAIL_CB, rsi_ip_renewal_fail_handler);                               //! Initialize IP renewal fail call back
	rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);       //! Initialize remote terminate call back
	rsi_wlan_register_callbacks(RSI_IP_CHANGE_NOTIFY_CB, rsi_ip_change_notify_handler);                     //! Initialize IP change notify call back
	rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, rsi_stations_connect_notify_handler);       //! Initialize IP change notify call back
	rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, rsi_stations_disconnect_notify_handler); //! Initialize IP change notify call back

}


/*====================================================*/
/**
 * @fn         void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
 * @brief      Function to create Async socket
 * @param[in]  uint32_t sock_no, uint8_t *buffer, uint32_t length
 * @return     void
 * @section description
 * Callback for Socket Async Receive
 * ====================================================*/
void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	num_bytes += length;
#if SOCKET_ASYNC_FEATURE
#if (THROUGHPUT_TYPE  == UDP_RX) || (THROUGHPUT_TYPE  == TCP_RX)
	pkt_rx_cnt++;
	if (pkt_rx_cnt == MAX_TX_PKTS)
	{
		t_end = rsi_hal_gettickcount();
		data_recvd = 1;
	}
#endif
#endif
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
	xfer_time = t_end - t_start; 	            //! data transfer time
	throughput = ((float)(num_bits)/xfer_time); //!Throughput calculation

	throughput /= 1000;
	LOG_PRINT("\r\nThroughput in mbps : %2.2f\r\n",throughput);

	LOG_PRINT("Time taken in sec: %lu \r\n", (xfer_time/1000));
}

/*====================================================*/
/**
 * @fn         void measure_throughput(uint32_t total_bytes, uint32_t start_time, uint32_t end_time)
 * @brief      Function to calculate throughput
 * @param[in]  void
 * @return     void
 * @section description
 *====================================================*/
void measure_throughput (uint32_t total_bytes, uint32_t start_time, uint32_t end_time)
{
	float through_put;
	through_put = ((float)(total_bytes * 8)/((end_time-start_time)));
	through_put /= 1000;
	LOG_PRINT("\r\nThroughput in mbps is : %3.2f\r\n",through_put);
	LOG_PRINT("Time taken in sec: %lu \r\n", (uint32_t)((end_time-start_time)/1000));
}

#if SSL
/*====================================================*/
/**
 * @fn         int32_t rsi_app_load_ssl_cert()
 * @brief      Function to load SSL certificate
 * @param[in]  void
 * @return     void
 *====================================================*/
int32_t rsi_app_load_ssl_cert()
{
	int32_t	status = RSI_SUCCESS;
	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, NULL ,0); //! erase existing certificate
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("CA cert erase failed \r\n");
		return status;
	}

	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, servercert, (sizeof(servercert)-1)); //! Load SSL CA certificate
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("CA cert load failed \r\n");
		return status;
	}
	return status;
}
#endif

/*====================================================*/
/**
 * @fn         int32_t  rsi_wlan_app_task(void)
 * @brief      Function to work with application task
 * @param[in]  void
 * @return     void
 *=====================================================*/

int32_t  rsi_wlan_app_task(void)
{
	int32_t	status = RSI_SUCCESS;
	uint8_t ip[20]={0};
#if !(DHCP_MODE)
	uint32_t    ip_addr      = DEVICE_IP;
	uint32_t    network_mask = NETMASK;
	uint32_t    gateway      = GATEWAY;
#else
	uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
	uint32_t i;
	struct      rsi_sockaddr_in server_addr, client_addr;
	int32_t     server_socket, new_socket;
	int32_t     addr_size;
#if !SOCKET_ASYNC_FEATURE
	uint32_t    recv_size = 0;
	uint32_t	total_bytes_rx = 0,
#endif
	uint8_t     high_performance_socket = RSI_HIGH_PERFORMANCE_SOCKET;
	uint8_t     ssl_bit_map = RSI_SSL_BIT_ENABLE ;
	uint32_t	total_bytes_tx = 0, tt_start = 0, tt_end = 0, pkt_cnt = 0;

	for (i = 0 ; i < BUF_SIZE; i++)
	{
		send_buf[i] = i;
	}

	while(1)
	{
		switch(rsi_wlan_app_cb.state)
		{
		case RSI_WLAN_INITIAL_STATE:
		{
			rsi_wlan_app_callbacks_init();                           //! register callback to initialize WLAN
			rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;      //! update WLAN application state to unconnected state

#if TX_RX_RATIO_ENABLE
			status = rsi_wlan_buffer_config();
			if(status != RSI_SUCCESS)
			{
				return status;
			}
#endif

#if ENABLE_POWER_SAVE
			rsi_mutex_lock(&power_cmd_mutex);
			if (!powersave_cmd_given)
			{
				status = rsi_initiate_power_save();
				if(status != RSI_SUCCESS)
				{
					LOG_PRINT("failed to keep module in power save \r\n");
					return status;
				}
				powersave_cmd_given = true;
			}
			rsi_mutex_unlock(&power_cmd_mutex);
			LOG_PRINT("Module is in deepsleep \r\n");
#endif
		}
		//no break
		case RSI_WLAN_UNCONNECTED_STATE:
		{
			LOG_PRINT("WLAN scan started \r\n");
			status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("scan failed \r\n");
				break;
			}
			else
			{
				rsi_wlan_app_cb.state = RSI_WLAN_SCAN_DONE_STATE;       //! update WLAN application state to connected state
#if ENABLE_POWER_SAVE
				LOG_PRINT("Module is in standby \r\n");
#endif
				LOG_PRINT("scan done state \r\n");
			}

		}
		case RSI_WLAN_SCAN_DONE_STATE:
		{
			status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("WLAN connection failed \r\n");
				break;
			}
			else
			{
				rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;       //! update WLAN application state to connected state
				LOG_PRINT("WLAN connected state \r\n");
			}
		}
		//no break
		case RSI_WLAN_CONNECTED_STATE:
		{
			//! Configure IP
#if DHCP_MODE
			status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode,0, 0, 0, ip, sizeof(ip),0);
#else
			status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("IP Config failed \r\n");
				break;
			}
			else
			{
				rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
				LOG_PRINT("WLAN ipconfig done state \r\n");
				LOG_PRINT("RSI_STA IP ADDR: %d.%d.%d.%d \r\n",ip[6],ip[7],ip[8],ip[9]);
#if (SSL && LOAD_CERTIFICATE)
				status = rsi_app_load_ssl_cert();        //Function to load certificate
				if(status != RSI_SUCCESS)
				{
					break;
				}
#endif
			}

#if WLAN_SYNC_REQ
			//! start wifi tx/rx after bt and ble start-up activities
			if(rsi_ble_running)
			{
				rsi_semaphore_post(&sync_coex_ble_sem);  // BLE will proceed with Adv.
				rsi_delay_ms(100);
			}

			if(rsi_bt_running)
			{
				rsi_semaphore_post(&sync_coex_bt_sem);  // BLE will proceed with Adv.
				rsi_delay_ms(100);
			}
#endif
		}
		//no break
		case RSI_WLAN_IPCONFIG_DONE_STATE:
		{
//! When BT/BLE is configured, measure wlan throughput only after those connections
#if WLAN_THROUGHPUT_AFTER_BT_BLE_CONN
#if RSI_ENABLE_BT_TEST
			if(!bt_connected)
			{
				LOG_PRINT("\r\n waiting for bt connection \r\n");
				while(!bt_connected);			//! looping in while for flag to set, but semaphores are advisable
			}
#endif
#if RSI_ENABLE_BLE_TEST
			if(!ble_connected)
			{
				LOG_PRINT("\r\n waiting for ble connection \r\n");
				while(!ble_connected);			//! looping in while for flag to set, but semaphores are advisable
			}
#endif
#endif
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
				server_addr.sin_port = htons(PORT_NUM);

				//! Set IP address to localhost
				//!server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;
				server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

				LOG_PRINT("\r\n WLAN Throughput measurement started \r\n");
				LOG_PRINT("\r\nUDP TX start\r\n");

				//! take the current time
				tt_start = rsi_hal_gettickcount();

				while(1)
				{
					//! Send data on socket
					status = rsi_sendto(client_socket, send_buf, BUF_SIZE, 0, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
					if(status < 0)
					{
						status = rsi_wlan_get_status();
						rsi_shutdown(client_socket, 0);
						LOG_PRINT("\r\nFailed to send data to UDP Server, Error Code : 0x%X\r\n",status);
						LOG_PRINT("\r\n client socket is shutdown \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
					}
					pkt_cnt++;
					//! store the time after sending data
					tt_end = rsi_hal_gettickcount();
					//! Measure throughput if avg time is reached
					if(tt_end - tt_start >= THROUGHPUT_AVG_TIME)
					{
						total_bytes_tx = pkt_cnt * BUF_SIZE;
//! Measure throughput for every interval of THROUGHPUT_AVG_TIME
#if CONTINUOUS_THROUGHPUT
						//! Measure throughput
						measure_throughput(total_bytes_tx, tt_start, tt_end);
						//! reset to initial value
						total_bytes_tx = 0;
						pkt_cnt = 0;
						tt_start = rsi_hal_gettickcount();
#else
						LOG_PRINT("\r\n UDP TX completed \r\n");
						//! Measure throughput
						measure_throughput(total_bytes_tx, tt_start, tt_end);
						LOG_PRINT("\r\n WLAN Throughput measurement completed \r\n");
						LOG_PRINT("\r\n Module is still in connected state \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
#endif
					}
				}
			}
			break;
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
#if 0
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
				server_addr.sin_port = htons(PORT_NUM);

				//! Set IP address to localhost
				//!server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;
				server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

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

#else
				//! Set server structure
				memset(&server_addr, 0, sizeof(server_addr));

				//! Set family type
				server_addr.sin_family= AF_INET;

				//! Set local port number
				server_addr.sin_port = htons(PORT_NUM);

				//! Set IP address to localhost
				//!server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;
				server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

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
#endif
#if SOCKET_ASYNC_FEATURE
				LOG_PRINT("\r\n WLAN Throughput measurement started \r\n");
				LOG_PRINT("\r\nUDP RX Start\r\n");
				t_start = rsi_hal_gettickcount();           //! capture time-stamp before data transfer starts
				while(1)
				{
					if( data_recvd )
					{
						LOG_PRINT("\r\n UDP RX completed \r\n");
						compute_throughput();
						LOG_PRINT("\r\n WLAN Throughput measurement completed \r\n");
						LOG_PRINT("\r\n Module is still in connected state \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
					}
				}
#else

				while(1)
				{
					recv_size = BUF_SIZE;

					//! take the current time
					tt_start = rsi_hal_gettickcount();

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
						total_bytes_rx += status;
						//! store the time after sending data
						tt_end = rsi_hal_gettickcount();
						//! Measure throughput if avg time is reached
						if(tt_end - tt_start >= THROUGHPUT_AVG_TIME)
						{
							//! Measure throughput
							measure_throughput(total_bytes_rx, tt_start, tt_end);
							//! reset to initial value
							total_bytes_rx = 0;
							tt_start = rsi_hal_gettickcount();
						}
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
				server_addr.sin_port = htons(PORT_NUM);

				//! Set IP address to localhost
				//!server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

				server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

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
				LOG_PRINT("\r\n WLAN Throughput measurement started \r\n");
				LOG_PRINT("\r\nTCP TX Start\r\n");

				//! take the current time
				tt_start = rsi_hal_gettickcount();

				while(1)
				{
					//! Send data on socket
					status = rsi_send(client_socket,send_buf, BUF_SIZE, 0);
					if(status < 0)
					{
						status = rsi_wlan_get_status();
						rsi_shutdown(client_socket, 0);
						LOG_PRINT("\r\nFailed to Send data to TCP Server, Error Code : 0x%X\r\n",status);
						LOG_PRINT("\r\n client socket is shutdown \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
					}
					pkt_cnt++;
					//! store the time after sending data
					tt_end = rsi_hal_gettickcount();
					//! Measure throughput if avg time is reached
					if(tt_end - tt_start >= THROUGHPUT_AVG_TIME)
					{
						total_bytes_tx = pkt_cnt * BUF_SIZE;
#if CONTINUOUS_THROUGHPUT
						//! Measure throughput
						measure_throughput(total_bytes_tx, tt_start, tt_end);
						//! reset to initial value
						total_bytes_tx = 0;
						pkt_cnt = 0;
						tt_start = rsi_hal_gettickcount();
#else
						LOG_PRINT("\r\n TCP TX completed \r\n");
						//! Measure throughput
						measure_throughput(total_bytes_tx, tt_start, tt_end);
						LOG_PRINT("\r\n WLAN Throughput measurement completed \r\n");
						LOG_PRINT("\r\n Module is still in connected state \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
#endif
					}
				}
			}
			break;
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
				server_addr.sin_port = htons(PORT_NUM);

				//! Set IP address to localhost
				//!server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;
				server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

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
#if SOCKET_ASYNC_FEATURE
				LOG_PRINT("\r\n WLAN Throughput measurement started \r\n");
				LOG_PRINT("\r\nTCP RX Start\r\n");
				t_start = rsi_hal_gettickcount();           //! capture time-stamp before data transfer is starts
				while(1)
				{
					if( data_recvd )
					{
						LOG_PRINT("\r\n TCP RX completed \r\n");
						compute_throughput();
						LOG_PRINT("\r\n WLAN Throughput measurement completed \r\n");
						LOG_PRINT("\r\n Module is still in connected state \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
					}
				}
#else
				while(1)
				{
					recv_size = BUF_SIZE;
					//! take the current time
					tt_start = rsi_hal_gettickcount();
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

						total_bytes_rx += status;
						//! store the time after sending data
						tt_end = rsi_hal_gettickcount();
						//! Measure throughput if avg time is reached
						if(tt_end - tt_start >= THROUGHPUT_AVG_TIME)
						{
							//! Measure throughput
							measure_throughput(total_bytes_rx, tt_start, tt_end);
							//! reset to initial value
							total_bytes_rx = 0;
							tt_start = rsi_hal_gettickcount();
						}
					} while(recv_size > 0);

				}
#endif
			}
			break;
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
				server_addr.sin_port = htons(PORT_NUM);

				//! Set IP address to localhost
				//!server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;

				server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

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
				LOG_PRINT("\r\n WLAN Throughput measurement started \r\n");
				LOG_PRINT("\r\nSSL TX Start \r\n");
				tt_start = rsi_hal_gettickcount();
				while(1)
				{
					//! Send data on socket
					status = rsi_send(client_socket,send_buf, BUF_SIZE, 0);
					if(status < 0)
					{
						status = rsi_wlan_get_status();
						rsi_shutdown(client_socket, 0);
						LOG_PRINT("\r\nFailed to Send data to SSL Server, Error Code : 0x%x\r\n", status);
						LOG_PRINT("\r\n client socket is shutdown \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
					}
					pkt_cnt++;
					//! measure throughput if pkt_cnt reaches max transmit packets
					if (pkt_cnt == MAX_TX_PKTS)
					{
						total_bytes_tx = pkt_cnt * BUF_SIZE;
						while(!data_recvd);
						rsi_shutdown(client_socket, 0);
						LOG_PRINT("SSL client closed\n");
						LOG_PRINT("SSL TX test end\n");
						data_recvd = 0;
						measure_throughput(total_bytes_tx, tt_start, t_end);
						total_bytes_tx = 0;
						pkt_cnt = 0;
						LOG_PRINT("\r\n WLAN Throughput measurement completed \r\n");
						LOG_PRINT("\r\n Module is still in connected state \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
					}
				}
			}
			break;
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
				server_addr.sin_port = htons(PORT_NUM);

				//! Set IP address to localhost
				//!server_addr.sin_addr.s_addr = SERVER_IP_ADDRESS;
				server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

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

				LOG_PRINT("\r\n WLAN Throughput measurement started \r\n");
				LOG_PRINT("\r\nSSL RX Start\r\n");
				t_start = rsi_hal_gettickcount();           //! capture time-stamp before data transfer is starts
				while(1)
				{
					if( data_recvd )
					{
						LOG_PRINT("\r\n SSL RX completed \r\n");
						compute_throughput();
						LOG_PRINT("\r\n WLAN Throughput measurement completed \r\n");
						LOG_PRINT("\r\n Module is still in connected state \r\n");
						rsi_wlan_app_cb.state = RSI_WLAN_IDLE_STATE;
						break;
					}
				}
			}
			}
			break;
		}
		case RSI_WLAN_IDLE_STATE:
		{
			//! Task has no work to do in this state, so adding a delay of 5sec
			rsi_os_task_delay(5000);
			break;
		}
		default:
			break;
		}

	}
}

uint32_t rsi_convert_4R_to_BIG_Endian_uint32(uint32_t *pw)
{
	uint32_t val;
	uint8_t* pw1 = (uint8_t *)pw;
	val = pw1[0];
	val <<= 8;
	val |= pw1[1] & 0x000000ff;
	val <<= 8;
	val |= pw1[2] & 0x000000ff;
	val <<= 8;
	val |= pw1[3] & 0x000000ff;
	return val;
}

/*====================================================*/
/**
 * @fn         void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler in station mode at rejoin failure
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	LOG_PRINT("\r\n Module Failed to rejoin the AP \r\n");
	rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;     //! update wlan application state
}

/*====================================================*/
/**
 * @fn         void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      IP renewal failure call back handler in station mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_ip_renewal_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	LOG_PRINT("In IP renewal handler \r\n");
	//! update wlan application state
	rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
}


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
	rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
	t_end = rsi_hal_gettickcount();           //! capture time-stamp after data transfer is completed
}

/*====================================================*/
/**
 * @fn         void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify IP change in Station mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_ip_change_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	LOG_PRINT("In IP change notify handler \r\n");
	//! update wlan application state
	rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
}


/*====================================================*/
/**
 * @fn         void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify stations connect in AP mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	LOG_PRINT("In Station Connect \r\n");
}

/*====================================================*/
/**
 * @fn         void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
 * @brief      Callback handler to notify stations disconnect in AP mode
 * @param[in]  uint16_t status, uint8_t *buffer, const uint32_t length
 * @return     void
 *=====================================================*/
void rsi_stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	LOG_PRINT("In Station Disconnect \r\n");
}
#endif
