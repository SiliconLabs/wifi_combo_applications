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
 * @brief : This file contains example application for OTA Firmware upgradation
 * from server
 * @section Description :
 * This application demonstrates how to upgrade new firmware to Redpine device
 * using remote TCP server.
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

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_utils.h"
#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#include "rsi_chip.h"
#endif

//! socket include file to firmware upgrade APIs
#include "rsi_firmware_upgradation.h"

//! Access point SSID to connect
#define SSID              "REDPINE_AP"

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               ""


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
//! E.g: 0x640AA8C0 == 192.168.0.101
#define SERVER_IP_ADDRESS 0x6500A8C0

//! Receive data length
#define RECV_BUFFER_SIZE   1027

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

//! OTAF Server port number
#define OTAF_SERVER_PORT                  5001

//! OTAF TCP receive timeout
#define OTAF_RX_TIMEOUT                   200

//! OTAF TCP retry count
#define OTAF_TCP_RETRY_COUNT              20

//! OTA Firmware upgradation retry count
#define OTAF_RETRY_COUNT                  10

//! OTAF response handler
void rsi_ota_fw_up_response_handler(uint16_t status, uint16_t chunk_number);

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! standard defines
volatile uint8_t  rsp_received;

//! OTAF retry count
volatile uint16_t otaf_retry_cnt;

static uint16_t    chunk_number = 1;

int32_t rsi_firmware_upgradation_app()
{
  int32_t     status       = RSI_SUCCESS;
  uint8_t resp_buf[20]; 
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);

#endif
  uint32_t    otaf_server_addr  = SERVER_IP_ADDRESS;

  //! Set OTAF retry count
  otaf_retry_cnt = OTAF_RETRY_COUNT;

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
	status = rsi_wlan_scan((int8_t *)SSID, 0, NULL, 0);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN AP Scan Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWLAN AP Scan Success\r\n");
	}

	status = rsi_wlan_get(RSI_FW_VERSION,resp_buf,18);

	LOG_PRINT("\r\nFirmware version before upgrade : %s\r\n",resp_buf);

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

	LOG_PRINT("\r\nFirmware upgrade start\r\n");
	do
	{
		status = rsi_ota_firmware_upgradation(RSI_IP_VERSION_4, (uint8_t *)&otaf_server_addr, OTAF_SERVER_PORT, chunk_number, OTAF_RX_TIMEOUT, OTAF_TCP_RETRY_COUNT, rsi_ota_fw_up_response_handler);

		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("\r\nOTA Firmware Upgradation Failed, Error Code : 0x%X\r\n", status);
			return status;
		}


    //! wait for the success response 
    do
    {
      //! event loop 
#ifndef RSI_WITH_OS
      rsi_wireless_driver_task();
#endif

    }while(!rsp_received);

    //! Check for FWUP success
    if(rsp_received == 1)
    {
      break;
    }

    //! Check for FWUP failure 
    if((rsp_received == 2) && (otaf_retry_cnt))
    {
      otaf_retry_cnt--;
    }
    
    //! Check for FWUP failure  with retry count
    if((rsp_received== 2) && (otaf_retry_cnt == 0))
    {
      return  rsi_wlan_get_status();
    }

    //! reset rsp received
    rsp_received = 0;

	} while(otaf_retry_cnt);

	LOG_PRINT("\r\nFirmware upgrade complete\r\n");

	rsi_wireless_deinit();

#ifdef RSI_M4_INTERFACE 	
	RSI_CLK_M4ssRefClkConfig(M4CLK, ULP_32MHZ_RC_CLK);
#endif	
	
	status = rsi_wireless_init(0,0);
			
	if(status == 0)
	{
		status = rsi_wlan_get(RSI_FW_VERSION,resp_buf,18);

		LOG_PRINT("\r\nFirmware version after upgrade : %s\r\n",resp_buf);
	}
	//! reset rsp received
  rsp_received = 0;

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
  rsi_task_create( rsi_firmware_upgradation_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call firware upgradation application
  status = rsi_firmware_upgradation_app();

  //! Application main loop
  main_loop();
#endif
  return status;

}


//! OTAF respone handler
void rsi_ota_fw_up_response_handler(uint16_t status, uint16_t chunk_no)
{

  if(status == RSI_SUCCESS)
  {
    rsp_received = 1;
  }
  else
  { 
    rsp_received = 2;
    chunk_number = chunk_no;
  } 

  return;
}

