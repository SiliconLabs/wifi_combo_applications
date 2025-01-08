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

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

#include "rsi_bootup_config.h"
//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Network include file
#include "rsi_nwk.h"

#include "rsi_dhcp_user_class.h"

#include "rsi_utils.h"

//! configurattion Parameters

//! Access point SSID to connect
#define SSID              "REDPINE"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO              0

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               NULL

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE         1 

//! DHCP HOSTNAME feature 1- Enable 0- Disable
#define DHCP_HOST_NAME    0

//! DHCP OPTION-81 1- Enable 0- Disable
#define DHCP_OPTION_77    1
#define DHCP_UNICAST_OFFER   1 

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP              0x3301A8C0

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

//! USER CLASS Max count 2
#define RSI_DHCP_USER_CLASS_COUNT 2 

//! User Class mode 1- RFC Format 2- Windows Format
#define RSI_DHCP_USER_CLASS_MODE  2 

//! USER CLASS DATA MAX LENGTH 64 bytes
#define RSI_DHCP_USER_CLASS_DATA_1          "UserClass-Redpine1"   
#define RSI_DHCP_USER_CLASS_DATA_LENGTH_1   18
#define RSI_DHCP_USER_CLASS_DATA_2          "UserClass-Redpine2"  
#define RSI_DHCP_USER_CLASS_DATA_LENGTH_2   18  
   
uint8_t *usr_cls_data_arr[RSI_DHCP_USER_CLASS_COUNT] = {RSI_DHCP_USER_CLASS_DATA_1, RSI_DHCP_USER_CLASS_DATA_2};
uint8_t usr_cls_length_arr[RSI_DHCP_USER_CLASS_COUNT] = {RSI_DHCP_USER_CLASS_DATA_LENGTH_1, RSI_DHCP_USER_CLASS_DATA_LENGTH_2};

user_class_data_t    user_class_data[RSI_DHCP_USER_CLASS_COUNT];     
//! standard defines

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! User class response handler
void rsi_dhcp_usr_cls_response_handler(uint16_t status);
volatile uint8_t usr_cls_rsp_received;

//! User class data

//! DHCP User Class Application 
int32_t rsi_dhcp_user_class_app()
{
	int32_t     status            = RSI_SUCCESS;
#if !(DHCP_MODE) 
	uint32_t    ip_addr           = DEVICE_IP;
	uint32_t    network_mask      = NETMASK;
	uint32_t    gateway           = GATEWAY;

#endif
	uint8_t     count             = RSI_DHCP_USER_CLASS_COUNT; 
	uint8_t     mode              = RSI_DHCP_USER_CLASS_MODE; 
  uint8_t     ii                = 0;

#if DHCP_MODE
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif

#if DHCP_OPTION_77
	dhcp_mode |= RSI_DHCP_OPTION77; 
#endif

#if (DHCP_HOST_NAME && (!DHCP_OPTION_77))
	dhcp_mode |= RSI_DHCP_HOSTNAME;
#endif

#if (DHCP_MODE && (DHCP_UNICAST_OFFER))
	dhcp_mode |= RSI_DHCP_UNICAST_OFFER;
#endif

  //! Fill user class data 
  for(ii=0; ii<count; ii++)
  {
    user_class_data[ii].length = usr_cls_length_arr[ii];
    memcpy(&user_class_data[ii].data[ii], usr_cls_data_arr[ii], usr_cls_length_arr[ii]); 
  }

	//! WC initialization
	status = rsi_wireless_init(0, 0);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! Scan for Access points
	status = rsi_wlan_scan((int8_t *)SSID, 0, NULL, 0);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! Connect to an Access point
	status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
        
  //! DHCP USER CLASS Command
  status = rsi_dhcp_user_class((uint8_t) mode, (uint8_t) count, (user_class_data_t *)&user_class_data[0], rsi_dhcp_usr_cls_response_handler);

  //! wait for the success response 
  do
  {
#ifndef RSI_WITH_OS
    //! event loop 
    rsi_wireless_driver_task();
#endif

  }while(!usr_cls_rsp_received);

  if(usr_cls_rsp_received == 2)
  {
    return  rsi_wlan_get_status();
  }

  usr_cls_rsp_received = 0;

	//! Configure IP 
#if DHCP_MODE
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0, 0);
#else
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
#endif
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
	rsi_task_create(rsi_dhcp_user_class_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call DHCP user class application
	status = rsi_dhcp_user_class_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}


//! User class response handler
void rsi_dhcp_usr_cls_response_handler(uint16_t status)
{
  //! Check for SUCCESS response
	if(status == RSI_SUCCESS)
	{
		usr_cls_rsp_received = 1;
	}
	else
	{
		usr_cls_rsp_received = 2;
	}
}
