/*******************************************************************************
* @file  rsi_provisioning_app.c
* @brief 
*******************************************************************************
* # License
* <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/

/*================================================================================
 * @brief : This file contains example application for Provisioning application
 * @section Description :
 * The provisioning application demonstrates how to provide provisioning to 
 * connect desired Access Point using the HTTP server, MDNS functionality of
 * Silabs device.
 =================================================================================*/

/**
 * Include files
 * */
//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer WLAN APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer WLAN APIs
#include "rsi_wlan_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_nwk.h"

//! Include the provisioning sample webpage
#include "provisioning.txt"

#include "rsi_json_handlers.h" 
#include "rsi_mdnsd.h"

//! configuration Parameters

//! Access point SSID to connect
#define SSID                                "SILABS_AP1234"

//!Channel number 
#define CHANNEL_NO                             11

//! Security type
#define SECURITY_TYPE                        RSI_WPA2

//! Encryption type
#define ENCRYPTION_TYPE                      RSI_NONE

//! Password
#define PSK                                 "1234567890"

//! Beacon interval
#define  BEACON_INTERVAL                       100

//! DTIM interval 
#define DTIM_INTERVAL                           4

//! IP version 
#define MDNSD_IP_VERSION                        4

//! Time to live for MDNSD host name 
#define MDNSD_INIT_TTL                         300

//! Time to live for added service
#define MDNSD_SERVICE_TTL                      300

//! MDNSD service port
#define MDNSD_SERVICE_PORT                     80

//! MDNSD more services bit: set to 0 if it is last service
#define MDNSD_SERVICE_MORE                     0

//! MDNSD host name
#define MDNSD_HOST_NAME                    "wiseconnect.local."

//! MDNSD service pointer name
#define MDNSD_POINTER_NAME                 "_http._tcp.local."

//! MDNSD service name
#define MDNSD_SERVICE_NAME                 "wiseconnect._http._tcp.local"

//! MDNSD service text 
#define MDNSD_SERVICE_TEXT                 "text_field"

//! File name of the webpage and JSON object
#define FILE_NAME                          "provisioning.html"

//! WEB_PAGE associated to JSON 8- ENABLE 0- Disable
#define WEB_PAGE_ASSOCIATED_TO_JSON            8

//! Flags
#define FLAGS                           WEB_PAGE_ASSOCIATED_TO_JSON

//! IP address of the module (E.g: 192.168.10.1)
#define DEVICE_IP                          "192.168.10.1"

//! IP address of Gateway (E.g: 192.168.10.1)
#define GATEWAY                            "192.168.10.1"

//! IP address of Netmask (E.g: 255.255.255.0)
#define NETMASK                           "255.255.255.0"


#define HTTP_SERVER_USERNAME                "admin"

#define HTTP_SERVER_PASSWORD                "silabs"

//! JSON object data buffer
uint8_t json_object_buffer[512];
uint8_t scan_result[513];

//! JSON object data structure
rsi_json_object_t  json_object_data;

//! Memory length for driver
#define GLOBAL_BUFF_LEN                     15000

//! WLAN task priority
#define RSI_WLAN_TASK_PRIORITY                1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY              2

//! WLAN task stack size
#define RSI_WLAN_TASK_STACK_SIZE             500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE           500

//! standard defines
uint8_t  rsp_received;

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];
uint8_t mac_addr[6];

//! Function prototypes
void main_loop(void);

//! Scan submit loop
int32_t scan_loop(void);

//! JSON object update handler
void rsi_json_object_update_handler(uint8_t *filename, uint8_t *json_object_str, uint32_t length, uint32_t status);

//! JSON object event handler
void rsi_json_object_event_handler(uint32_t status, uint8_t *json_object_str, uint32_t length);

//! webpage request handler 
void rsi_webpage_request_handler(uint8_t type, uint8_t *url_name,uint8_t *post_content_buffer, uint32_t post_content_length, uint32_t status);

//! wireless firmware upgrade handler 
void rsi_wireless_fw_upgrade_handler(uint8_t type, uint32_t status);

int32_t rsi_wc_provisioning(void);

#ifdef RSI_WITH_OS
  rsi_task_handle_t driver_task_handle = NULL;
#endif

//! To register HTTP server related call-backs
void rsi_http_server_cbs_init(void)
{
  //! Register JSON object update callback
  rsi_wlan_nwk_register_json_update_cb(RSI_WLAN_NWK_JSON_UPDATE_CB, rsi_json_object_update_handler);
  
  //! Register JSON event callback
  rsi_wlan_nwk_register_json_event_cb(RSI_WLAN_NWK_JSON_EVENT_CB, rsi_json_object_event_handler);

  //! Register webpage request callback
  rsi_wlan_nwk_register_webpage_req_cb(RSI_WLAN_NWK_URL_REQ_CB, rsi_webpage_request_handler);

  //! Register wireless firmware upgrade callback
  rsi_wlan_nwk_register_wireless_fw_upgrade_cb(RSI_WLAN_NWK_FW_UPGRADE_CB, rsi_wireless_fw_upgrade_handler);
}

//! Provisioning Application 
int32_t rsi_provisioning_app()
{
  int32_t status = RSI_SUCCESS;
  uint8_t *json_object_str;

  uint32_t ip_addr      = ip_to_reverse_hex((char *)DEVICE_IP);
  uint32_t network_mask = ip_to_reverse_hex((char *)NETMASK);
  uint32_t gateway      = ip_to_reverse_hex((char *)GATEWAY);

  //! Driver Initialization
   status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
   if((status < 0) || (status > GLOBAL_BUFF_LEN)) {
		 LOG_PRINT("\r\nDriver Initialization Failed, Error Code : 0x%lX\r\n", status);
     return status;
   }

   //! Silabs Module Initialization
   status = rsi_device_init(LOAD_NWP_FW);
   if(status != RSI_SUCCESS) {
		 LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n", status);
     return status;
   }
   LOG_PRINT("\r\nDevice Initialization Success\r\n");

#ifdef RSI_WITH_OS  
  //! Task created for Driver task
  rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                  (uint8_t *)"driver_task",
                  RSI_DRIVER_TASK_STACK_SIZE,
                  NULL,
                  RSI_DRIVER_TASK_PRIORITY,
                  &driver_task_handle);
#endif
	
  //! Wireless Initialization
  status = rsi_wireless_init(6, 0);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nWireless Initialization Success\r\n");
	
  status = rsi_http_credentials((int8_t *)HTTP_SERVER_USERNAME ,(int8_t *)HTTP_SERVER_PASSWORD );
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nHTTP Credentials Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nHTTP Credentials Success\r\n");
  
  //! register HTTP server call backs
  rsi_http_server_cbs_init();

	//! Status will return ERROR for the first time because there is no pre-loaded webpage to erase
  status = rsi_webpage_erase(NULL);
	if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nWebpage Erase Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nWebpage Erase Success\r\n");
	
  //! Load sample webpage
  status  = rsi_webpage_load(FLAGS, FILE_NAME, provisioning, (sizeof(provisioning)-1));
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWebpage Load Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nWebpage Load Success\r\n");

  //! Initialize the JSON object data structure with default values
  rsi_json_object_init(&json_object_data);

  //! Prepare JSON object string from JSON object data structure
  json_object_str = rsi_json_object_stringify(json_object_buffer, &json_object_data);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nJSON object stringify Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nJSON object stringify Success\r\n");

  //! Create JSON object data to the associated webpage
  status = rsi_json_object_create(0, FILE_NAME, json_object_str, strlen(json_object_str));
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nJSON object create Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nJSON object create Success\r\n");

  //! Configure IP 
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
  if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
	LOG_PRINT("\r\nIP Config Success\r\n");
  
  //! Start Access point
  status = rsi_wlan_ap_start((int8_t *)SSID, CHANNEL_NO, SECURITY_TYPE, ENCRYPTION_TYPE, PSK, BEACON_INTERVAL, DTIM_INTERVAL);
  if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nWLAN AP Start Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
	LOG_PRINT("\r\nWLAN AP Start Success\r\n");

  //! MDNSD service Initialization
  status =  rsi_mdnsd_init((uint8_t)MDNSD_IP_VERSION, (uint16_t)MDNSD_INIT_TTL, (uint8_t *)MDNSD_HOST_NAME);
  if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nMDNSD Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nMDNSD Initialization Success\r\n");

  //! Add required services
  status = rsi_mdnsd_register_service(MDNSD_SERVICE_PORT, MDNSD_SERVICE_TTL, MDNSD_SERVICE_MORE, MDNSD_POINTER_NAME, MDNSD_SERVICE_NAME, MDNSD_SERVICE_TEXT);
  if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nMDNSD Register service Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
	LOG_PRINT("\r\nMDNSD Register service Success\r\n");
  
  //! wait for the success response 
  status = scan_loop();
  if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nScan Loop Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
	LOG_PRINT("\r\nScan Loop Success\r\n");
  
  //! De-initialization MDNSD service
  status = rsi_mdnsd_deinit();
  if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nMDNSD De-initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
	LOG_PRINT("\r\nMDNSD De-initialization Success\r\n");

  //! Connect the AP based on Updated JSON object data values
  rsi_wc_provisioning();
}

int32_t rsi_wc_provisioning(void)
{
  int32_t     status       = RSI_SUCCESS;
  uint8_t     *json_object_str;
  uint8_t ssid[33];

  //! Get Configuration parameters from JSON object data structure
  uint8_t     *temp       = json_object_data.ssid;
  strcpy(ssid, temp);

  uint8_t     channel     = json_object_data.channel;
  uint8_t     sec_enable  = json_object_data.sec_enable;
  uint8_t     sec_type    = json_object_data.sec_type;
  uint8_t     *psk        = json_object_data.psk;

  //! Check for security enable
  if(!sec_enable) {
    sec_type = RSI_OPEN; //! Set to OPEN security mode
    psk      =  NULL;    //! Set to empty PSK
  }

#ifdef RSI_WITH_OS  
	rsi_task_destroy(driver_task_handle); 
#endif
	
  //! Disconnect from the connected AP
  status = rsi_wireless_deinit();
  if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nWireless De-initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
	LOG_PRINT("\r\nWireless De-initialization Success\r\n");

#ifdef RSI_WITH_OS 
  //! Driver Initialization
   status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
   if((status < 0) || (status > GLOBAL_BUFF_LEN)) {
		 LOG_PRINT("\r\nDriver Initialization Failed, Error Code : 0x%lX\r\n", status);
     return status;
   }

	 //! Silabs Module Initialization
   status = rsi_device_init(LOAD_NWP_FW);
   if(status != RSI_SUCCESS) {
		 LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n", status);
     return status;
   }
   LOG_PRINT("\r\nDevice Initialization Success\r\n");

  //! Task created for Driver task
  rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                  (uint8_t *)"driver_task",
                  RSI_DRIVER_TASK_STACK_SIZE,
                  NULL,
                  RSI_DRIVER_TASK_PRIORITY,
                  &driver_task_handle);

#endif	
	
  //! Wireless Initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS) {
		LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
	LOG_PRINT("\r\nWireless Initialization Success\r\n");

  //! Set client connected state
  json_object_data.conec = 3;

  //! Prepare JSON object string from JSON object data structure
  json_object_str = rsi_json_object_stringify(json_object_buffer, &json_object_data);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nJSON object stringify Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
//  LOG_PRINT("\r\nJSON object stringify Success\r\n");

  //! Create JSON object data to the associated webpage
  status = rsi_json_object_create(0, FILE_NAME, json_object_str, strlen(json_object_str));
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nJSON object create Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
//  LOG_PRINT("\r\nJSON object create Success\r\n");

  //Selected SSID information
  LOG_PRINT("\r\nSelected AP Details:\r\n");
  LOG_PRINT("\r\nSSID : %s\r\n", ssid);
  LOG_PRINT("\r\nChannel : %d\r\n", channel);
  LOG_PRINT("\r\nSecurity Enable : %d\r\n", sec_enable);
  LOG_PRINT("\r\Security Type : %d\r\n", sec_type);
  LOG_PRINT("\r\nPSK : %s\r\n", psk);

  //! Scan for Access points
    status = rsi_wlan_scan((int8_t *)ssid, channel, NULL, 0);
    if (status != RSI_SUCCESS) {
      LOG_PRINT("\r\nWLAN AP Scan Failed, Error Code : 0x%lX\r\n", status);
      return status;
    }
    LOG_PRINT("\r\nWLAN AP Scan Success\r\n");

  //! Connect to an Access point
  status = rsi_wlan_connect((int8_t *)ssid, (rsi_security_mode_t)sec_type, psk);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWLAN AP Connect Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nWLAN AP Connect Success\r\n");

  //! Configure IP through DHCP 
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, NULL, 0,0);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nIP Config Success\r\n");

#ifndef RSI_WITH_OS
  //! wait for the success response 
  main_loop();
#endif
  
#ifdef RSI_WITH_OS
  //! Connect the AP based on Updated JSON object data values
  rsi_wc_provisioning();
#endif

  return status;
}

int32_t scan_loop(void)
{
  int32_t     status       = RSI_SUCCESS;
  uint8_t     *json_object_str;

  while(1)
  {
#ifndef RSI_WITH_OS
    //! event loop 
    rsi_wireless_driver_task();
#endif

    if(rsp_received == 3) {

      //!Delete JSON object data before updating scan results
        status = rsi_json_object_delete(FILE_NAME);
        if(status != RSI_SUCCESS) {
            return status;
      }

      //! Scan for Access points
      status = rsi_wlan_scan(NULL, 0, (rsi_rsp_scan_t *)scan_result, sizeof(rsi_rsp_scan_t));
      if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\nWLAN AP Scan Failed, Error Code : 0x%lX\r\n", status);
          return status;
      }
      LOG_PRINT("\r\nWLAN AP Scan Success\r\n");

      rsi_json_object_scan_list_update(&json_object_data, (rsi_rsp_scan_t *)scan_result);

      json_object_data.conec = 2;

      //! Prepare JSON object string from JSON object data structure
      json_object_str = rsi_json_object_stringify(json_object_buffer, &json_object_data);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nJSON object stringify Failed, Error Code : 0x%lX\r\n", status);
        return status;
      }
//      LOG_PRINT("\r\nJSON object stringify Success\r\n");

      //! Create JSON object data to the associated webpage
      status = rsi_json_object_create(0, FILE_NAME, json_object_str, strlen(json_object_str));
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nJSON object create Failed, Error Code : 0x%lX\r\n", status);
        return status;
      }
//      LOG_PRINT("\r\nJSON object create Success\r\n");

      rsp_received = 0;
    }

    if(rsp_received)
    {
      rsp_received = 0;
      return status;
    }
  }
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

    if(rsp_received)
    {
      rsp_received = 0;
      break;
    }
  }
}

int main()
{
  int32_t status = RSI_SUCCESS;
	
 #ifdef RSI_WITH_OS
   rsi_task_handle_t wlan_task_handle = NULL;
 #endif

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
 
  rsi_task_create((rsi_task_function_t)(int32_t)rsi_provisioning_app,
                  (uint8_t *)"wlan_task",
                  RSI_WLAN_TASK_STACK_SIZE,
                  NULL,
                  RSI_WLAN_TASK_PRIORITY,
                  &wlan_task_handle);
	
  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call  Provisioning application
  status = rsi_provisioning_app();

  //! Application main loop
  main_loop();
#endif
  return status;
}

//! JSON object update handler
void rsi_json_object_update_handler(uint8_t *filename, uint8_t *json_object_str, uint32_t length, uint32_t status)
{
  if(status == RSI_SUCCESS)
  {
    //! Update the JSON object data structure with received JSON object data
    rsi_json_object_update(&json_object_data, json_object_str, filename);
    rsp_received = 1;
  }
}

void rsi_json_object_event_handler(uint32_t status, uint8_t *json_object_str, uint32_t length)
{
  if(status == RSI_SUCCESS)
  {
    //! Update the JSON object data structure with received JSON object data
    rsp_received = 3;
  }
}

//! webpage request handler 
void rsi_webpage_request_handler(uint8_t type, uint8_t *url_name,uint8_t *post_content_buffer, uint32_t post_content_length, uint32_t status)
{
  if(status == RSI_SUCCESS)
  {
    rsp_received = 1;
  }
}

//! wireless firmware upgrade handler 
void rsi_wireless_fw_upgrade_handler(uint8_t type, uint32_t status)
{
  if(status == RSI_SUCCESS)
  {
    rsp_received = 1;
  }
}
