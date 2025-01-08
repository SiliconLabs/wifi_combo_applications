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
 * @brief : This file contains example application for Wlan Station BLE
 * Provisioning
 * @section Description :
 * This application explains how to get the WLAN connection functionality using
 * BLE provisioning.
 * Redpine Module starts advertising and with BLE Provisioning the Access Point
 * details are fetched.
 * Redpine device is configured as a WiFi station and connects to an Access Point.
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

//! WLAN include file for configuration
#include<rsi_wlan_config.h>

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

#include "rsi_wlan_non_rom.h"

#include "string.h"
#include "rsi_hal.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#include "rsi_chip.h"
#endif


//! Access point SSID to connect
#define SSID              "REDPINE_AP"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE      RSI_OPEN

//! Password
#define PSK                NULL

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE          1

//! If DHCP mode is disabled give IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP          0x6500A8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY            0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK            0x00FFFFFF 

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

/*
 *********************************************************************************************************
 *                                         LOCAL GLOBAL VARIABLES
 *********************************************************************************************************
 */


rsi_rsp_scan_t  scan_result;
uint16_t        scanbuf_size;
uint8_t         connected=0,timeout=0;
uint8_t disconnected=0 , disassosiated = 0;
uint8_t a=0;
uint8_t rsp_buf[18];

/*
 *********************************************************************************************************
 *                                               DATA TYPES
 *********************************************************************************************************
 */


typedef enum rsi_app_cmd_e
{
	RSI_DATA = 0,
	RSI_WLAN_SCAN_RESP = 1,
	RSI_WLAN_CONN_STATUS = 2,
	RSI_WLAN_DISCONN_STATUS = 3,
	RSI_WLAN_DISCONN_NOTIFY = 4,
	RSI_WLAN_TIMEOUT_NOTIFY  =5

}rsi_app_cmd_t;


extern uint8_t  coex_ssid[50],dis_ssid[50], pwd[34], sec_type;
extern uint8_t cid[100];
uint8_t retry=1;

uint8_t conn_status;

extern uint8_t magic_word;

#define RSI_APP_BUF_SIZE        1600

extern rsi_wlan_app_cb_t rsi_wlan_app_cb;
extern void rsi_wlan_app_send_to_ble (uint16_t  msg_type, uint8_t *data, uint16_t data_len);

//extern volatile uint32_t _dwTickCount;

uint32_t received_length = 0;

//! Call back for Socket Async
void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	received_length += length;
}

//! rejoin failure call back handler in station mode
void rsi_join_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	//! update wlan application state
	disconnected = 1;connected=0;
	rsi_wlan_app_cb.state = RSI_WLAN_DISCONNECTED_STATE;
}

void rsi_wlan_app_call_backs_init(void)
{
	//! Initialze join fail call back
	rsi_wlan_register_callbacks(RSI_JOIN_FAIL_CB, rsi_join_fail_handler);
}

int32_t rsi_wlan_app_task()
{
	int32_t     status = RSI_SUCCESS;
	uint8_t     ipconfig[20];

	switch(rsi_wlan_app_cb.state)
	{
	case RSI_WLAN_INITIAL_STATE:
	{
		rsi_delay_ms(6);
		//! update wlan application state

#if (WYZBEE_CONFIGURATOR || BLE_SCANNER)
		rsi_wlan_app_call_backs_init();

		//	rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;
		if(magic_word)
		{
			rsi_wlan_app_cb.state = RSI_WLAN_FLASH_STATE;
		}
		else
		{
			rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;
		}

#elif(STATIC_AP_OPEN || STATIC_AP_WPA2_SECURE || STATIC_AP_WPA_SECURE || STATIC_AP_MIXED)
		rsi_wlan_app_cb.state = RSI_WLAN_SCAN_STATE;
#endif
	}
	break;

	case RSI_WLAN_UNCONNECTED_STATE:
	{
		//Any additional code if required
	}
	break;

	case RSI_WLAN_SCAN_STATE:
	{
		scanbuf_size = sizeof(scan_result);
		memset(&scan_result,0, sizeof(scan_result));

		status = rsi_wlan_scan(0,0,&scan_result,scanbuf_size);
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("\r\nWLAN Scan Failed, Error Code : 0x%X\r\n",status);
			break;
		}
		else
		{
			//! update wlan application state

#if WYZBEE_CONFIGURATOR
			rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;
			rsi_wlan_app_send_to_ble (RSI_WLAN_SCAN_RESP, (uint8_t*)&scan_result, scanbuf_size);

#elif(STATIC_AP_OPEN || STATIC_AP_WPA2_SECURE || STATIC_AP_WPA_SECURE || STATIC_AP_MIXED)
			rsi_wlan_app_cb.state = RSI_WLAN_JOIN_STATE;
#endif
		}
	}
	break;

	case RSI_WLAN_JOIN_STATE:
	{
#if WYZBEE_CONFIGURATOR
		if(sec_type == 0 || sec_type == '0')
		{
			status = rsi_wlan_connect((int8_t *)coex_ssid, RSI_OPEN, NULL);
		}
		else if(sec_type == 1 || sec_type == '1')
		{
			status = rsi_wlan_connect((int8_t *)coex_ssid, RSI_WPA, pwd);
		}
		else if(sec_type == 2 || sec_type == '2')
		{
			status = rsi_wlan_connect((int8_t *)coex_ssid, RSI_WPA2, pwd);
		}
		else if(sec_type == 6 || sec_type == '6')
		{
			status = rsi_wlan_connect((int8_t *)coex_ssid, RSI_WPA_WPA2_MIXED, pwd);
		}
#endif

#if STATIC_AP_OPEN
		status = rsi_wlan_connect((int8_t *)static_ap_ssid, RSI_OPEN, NULL);
#endif

#if STATIC_AP_WPA2_SECURE
		status = rsi_wlan_connect((int8_t *)static_ap_ssid, RSI_WPA2, static_ap_pwd);
#endif

#if STATIC_AP_WPA_SECURE
		status = rsi_wlan_connect((int8_t *)static_ap_ssid, RSI_WPA, static_ap_pwd);
#endif

#if STATIC_AP_MIXED
		status = rsi_wlan_connect((int8_t *)static_ap_ssid, RSI_WPA_WPA2_MIXED, static_ap_pwd);
#endif

		if(status != RSI_SUCCESS)
		{
#if WYZBEE_CONFIGURATOR
			a++;
			if(a == 5)
			{
				a = 0;
				timeout = 1;
				rsi_wlan_app_send_to_ble ( RSI_WLAN_TIMEOUT_NOTIFY, (uint8_t*)&timeout, 1);
				rsi_wlan_app_cb.state = RSI_WLAN_ERROR_STATE;
			}
#endif
			LOG_PRINT("\r\nWLAN Connect Failed, Error Code : 0x%X\r\n",status);
			break;
		}
		else
		{
			a = 0;
			//! update wlan application state
			rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
		}
	}
	break;

	case RSI_WLAN_FLASH_STATE:
	{
		if(retry)
		{
#if WYZBEE_CONFIGURATOR
			if(sec_type == 0 || sec_type == '0')
			{
				status = rsi_wlan_connect((int8_t *)coex_ssid, RSI_OPEN, NULL);
			}
			else if(sec_type == 1 || sec_type == '1')
			{
				status = rsi_wlan_connect((int8_t *)coex_ssid, RSI_WPA, pwd);
			}
			else if(sec_type == 2 || sec_type == '2')
			{
				status = rsi_wlan_connect((int8_t *)coex_ssid, RSI_WPA2, pwd);
			}
			else if(sec_type == 6 || sec_type == '6')
			{
				status = rsi_wlan_connect((int8_t *)coex_ssid, RSI_WPA_WPA2_MIXED, pwd);
			}
#endif
			if(status != RSI_SUCCESS)
			{
				LOG_PRINT("\r\nWLAN Connect Failed, Error Code : 0x%X\r\n",status);
				break;
			}
			else
			{
				rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE;
			}
		}
	}
	break;

	case RSI_WLAN_CONNECTED_STATE:
	{
		//! Configure IP
#if DHCP_MODE
		status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0,rsp_buf,sizeof(rsp_buf),0);
#else
		status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
		if(status != RSI_SUCCESS)
		{
#if WYZBEE_CONFIGURATOR
			a++;
			if(a == 3)
			{
				a = 0;
				timeout =1;
				status=rsi_wlan_disconnect();
				if(status == RSI_SUCCESS)
				{
					connected=0;disassosiated = 1;
					rsi_wlan_app_send_to_ble ( RSI_WLAN_TIMEOUT_NOTIFY, (uint8_t*)&timeout, 1);
					rsi_wlan_app_cb.state = RSI_WLAN_ERROR_STATE;
				}
			}
#endif
			LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%X\r\n", status);
			break;
		}
		else
		{
			a = 0;
			connected = 1;conn_status = 1; disconnected = 0; disassosiated = 0;
			//! update wlan application state
			rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE;

#if WYZBEE_CONFIGURATOR
			rsi_wlan_app_send_to_ble ( RSI_WLAN_CONN_STATUS, (uint8_t*)&connected, 1);
#endif
		}
	}
	break;

	case RSI_WLAN_IPCONFIG_DONE_STATE:
	{


	}
	break;

	case RSI_WLAN_ERROR_STATE:
	{


	}
	break;

	case RSI_WLAN_DISCONNECTED_STATE:
	{
		retry = 1;
		rsi_wlan_app_send_to_ble ( RSI_WLAN_DISCONN_STATUS, (uint8_t*)&disconnected, 1);
		rsi_wlan_app_cb.state = RSI_WLAN_FLASH_STATE;
	}
	break;

	case RSI_WLAN_DISCONN_NOTIFY_STATE:
	{
		status=rsi_wlan_disconnect();
		if(status == RSI_SUCCESS)
		{
#if RSI_WISE_MCU_ENABLE
			rsi_flash_erase((uint32_t)FLASH_ADDR_TO_STORE_AP_DETAILS);
#endif
			LOG_PRINT("\r\nWLAN Disconnect Failed, Error Code : 0x%X\r\n", status);
			disassosiated = 1;connected = 0;
			rsi_wlan_app_send_to_ble(RSI_WLAN_DISCONN_NOTIFY , (uint8_t*)&disassosiated,1);
			rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;
		}
	}
	break;

	default:

		break;
	}
}

