/**
 * @file    rsi_wlan_http_s.c
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
#include <rsi_bt_common_apis.h>
#include "rsi_sock_test.h"
#include "rsi_common_config.h"
#include "rsi_socket.h"
#include "rsi_wlan_config.h"
#if RSI_ENABLE_WLAN_TEST

#if SSL
#include "servercert.pem"                //! Include SSL CA certificate
#endif


/*=======================================================================*/
//   ! MACROS
/*=======================================================================*/


/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/
struct rsi_sockaddr_in server_addr, client_addr;    //! server and client IP addresses

rsi_task_handle_t http_socket_task_handle[SOCKTEST_INSTANCES_MAX] = {NULL};
redpine_socktest_ctx_t * ctx;
rsi_wlan_app_cb_t rsi_wlan_app_cb;                  //! application control block
int32_t client_socket;                              //! client socket id
//! Throughput parameters
uint32_t pkts = 0;
uint64_t num_bits = 0;
uint64_t total_bits = 0;
uint32_t xfer_time;
uint32_t total_time = 0;
uint64_t xfer_time_usec;
uint32_t t_end;
float throughput;
float throughput_mbps;
volatile uint8_t data_recvd = 0;
volatile uint64_t num_bytes = 0;


/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/
extern redpine_socktest_ctx_t   redpine_socktest_ctx[SOCKTEST_INSTANCES_MAX];
extern	rsi_semaphore_handle_t 	ble_main_task_sem, ble_slave_conn_sem, bt_app_sem, ant_app_sem, wlan_app_sem, bt_inquiry_sem, ble_scan_sem;
#if WLAN_SYNC_REQ
extern 	rsi_semaphore_handle_t	sync_coex_ble_sem, sync_coex_ant_sem, sync_coex_bt_sem;
#endif
extern	bool	rsi_ble_running, rsi_bt_running, rsi_ant_running, rsi_wlan_running, wlan_radio_initialized, powersave_cmd_given;
extern 	rsi_mutex_handle_t power_cmd_mutex;
extern rsi_semaphore_handle_t wlan_app_sem;
/*=======================================================================*/
//   ! EXTERN FUNCTIONS
/*=======================================================================*/


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
	volatile uint8_t download_complete = 0;
	uint8_t no_of_iterations = 0;

	while(1)
	{
		switch(rsi_wlan_app_cb.state)
		{
		case RSI_WLAN_INITIAL_STATE:
		{
			rsi_wlan_app_callbacks_init();                           //! register callback to initialize WLAN
			rsi_wlan_app_cb.state = RSI_WLAN_SCAN_STATE;

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
		break;
		case RSI_WLAN_UNCONNECTED_STATE:
		{
			//! do nothing
		}
		break;
		case RSI_WLAN_SCAN_STATE:
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
				rsi_wlan_app_cb.state = RSI_WLAN_JOIN_STATE;       //! update WLAN application state to connected state
#if ENABLE_POWER_SAVE
				LOG_PRINT("Module is in standby \r\n");
#endif
				LOG_PRINT("wlan scan done \r\n");
			}

		}
		break;
		case RSI_WLAN_JOIN_STATE:
		{
			status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("WLAN Connect Failed, Error Code : 0x%X\r\n",status);
				break;
			}
			else
			{
				rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;       //! update WLAN application state to connected state
				LOG_PRINT("WLAN connected state \r\n");
			}

		}
		break;
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
		}
		//no break
		case RSI_WLAN_IPCONFIG_DONE_STATE:
		{
#if HIGH_PERFORMANCE_ENABLE
			status = rsi_socket_config();
			if(status < 0)
			{
				LOG_PRINT("high-performance socket config failed \r\n");
				status = rsi_wlan_get_status();
				break;
			}
			//LOG_PRINT("high-performance socket config success \r\n");
#endif

			for(i=0; i<SOCKTEST_INSTANCES_MAX; i++)
			{
				redpine_socktest_ctx[i].threadid = i;
				rsi_semaphore_create(&redpine_socktest_ctx[i].http_soc_wait_sem, 0);
				status = rsi_task_create((rsi_task_function_t)perform_redpine_sock_test, (uint8_t *)"socket_task1", RSI_HTTP_SOCKET_TASK_STACK_SIZE, &redpine_socktest_ctx[i], RSI_HTTP_SOCKET_TASK_PRIORITY, &http_socket_task_handle[i]);
				if(status != RSI_ERROR_NONE)
				{
					LOG_PRINT("Thread creation failed %d \r\r",redpine_socktest_ctx[i].threadid);
					while(1);
				}
			}
			for(i=0; i<SOCKTEST_INSTANCES_MAX; i++)
			{
				rsi_semaphore_post(&redpine_socktest_ctx[i].http_soc_wait_sem);
			}

			do{
				rsi_semaphore_wait(&wlan_app_sem, 0);

				for(i=0; i<SOCKTEST_INSTANCES_MAX; i++)
				{
					if(http_socket_task_handle[i] == NULL)
					{
						download_complete = 1;
					}
					else
					{
						download_complete = 0;
						break;
					}
				}
			}while(!download_complete);

			if(download_complete)
			{
				rsi_os_task_delay(50);
				download_complete = 0;
#if !CONTINUOUS_HTTP_DOWNLOAD
				no_of_iterations++;
#endif
				if(no_of_iterations == NO_OF_ITERATIONS)
				{
					for(i=0; i<SOCKTEST_INSTANCES_MAX; i++)
					{
						LOG_PRINT("Thread id: %d \r\n",redpine_socktest_ctx[i].threadid);
						LOG_PRINT("Tests Success: %d \r\n",redpine_socktest_ctx[i].num_successful_test);
						LOG_PRINT("Tests failed: %d \r\n",redpine_socktest_ctx[i].num_failed_test);
					}
					LOG_PRINT("download completed \r\n");
					//! Demo completed
					//! required execution completed, so destroy the task
					rsi_task_destroy(NULL);
				}
				else
				{
#if RX_DATA

#if HTTPS_DOWNLOAD
					LOG_PRINT("HTTPS download completed \r\n");
#else
					LOG_PRINT("HTTP download completed \r\n");
#endif
#endif
				}
				rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;
			}
			break;
		}
		case RSI_WLAN_SOCKET_CONNECTED_STATE:
		{
			break;
		}
		case RSI_WLAN_DISCONNECTED_STATE:
		{
			rsi_wlan_app_cb.state = RSI_WLAN_JOIN_STATE;
		}
		break;
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
	rsi_wlan_app_cb.state = RSI_WLAN_JOIN_STATE;     //! update wlan application state
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
	data_recvd = 1;        //Set data receive flag
	t_end = rsi_hal_gettickcount();           //! capture time-stamp after data transfer is completed
	rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE; //! update wlan application state
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

}
#endif
