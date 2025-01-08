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
 * @brief : This file contains example application for HTTP/HTTPS Client Post Data
 * @section Description :
 * This application demonstrates how to create Redpine device as HTTP/HTTPS client
 * and do GET and POST operations with the HTTP/HTTPS server opened on remote peer.
 * In this application, the device configures as Wi-Fi station and connects to
 * Access point and do HTTP/HTTPS Post and Get operation with HTTP/HTTPS server
 * opened on remote peer.
 =================================================================================*/

/**
 * Include files
 * */
#include "rsi_wlan_config.h"
//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h" 
#include "rsi_wlan_non_rom.h"

//! socket include file to refer socket APIs
#include "rsi_http_client.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
#include "rsi_nwk.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_utils.h"

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//! include the certificate 
#include "cacert.pem"

//! include the index html page
#include "index.txt"

//! standard defines

//! to Enable IPv6 set this bit in FLAGS, Default is IPv4 
#define   HTTPV6          1

//! Set HTTPS_SUPPORT to use HTTPS feature
#define   HTTPS_SUPPORT   2

//! Set HTTP_POST_DATA to use HTTP POST LARGE DATA feature
#define   HTTP_POST_DATA  32

//! Set HTTP_V_1_1 to use HTTP version 1.1
#define   HTTP_V_1_1      64

#define   RSI_HTTP_END_OF_DATA   1
#define   RSI_HTTP_POST_RSP      2
#define   RSI_HTTP_POST_DATA_RSP 4
#define   RSI_HTTP_POST_RCV_RSP  8

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
//! for example select required flag bits,  Eg:(HTTPS_SUPPORT | HTTPV6)
#define FLAGS         (HTTP_POST_DATA | HTTP_V_1_1)  

//! Server port number
#define HTTP_PORT            80

//! HTTP Server IP address.
#define HTTP_SERVER_IP_ADDRESS "192.168.10.1"

//! HTTP resource name
#define HTTP_URL           "/index.html"           

//! set HTTP hostname
#define HTTP_HOSTNAME     "192.168.10.1" 

//! set HTTP extended header
//! if NULL , driver fills default extended header
#define HTTP_EXTENDED_HEADER    NULL  

//! set HTTP hostname
#define USERNAME               "admin" 

//! set HTTP hostname
#define PASSWORD               "admin" 



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

//! Application buffer length
#define APP_BUFF_LEN           2000

//! Max HTTP POST DATA buffer length
#define MAX_HTTP_CLIENT_POST_DATA_BUFFER_LENGTH 900

//! Max HTTP Packet count 
#define RSI_HTTP_MAX_PKT_COUNT_ABORT 10

//! Http abort 
#define RSI_ENABLE_HTTP_ABORT     0

//! HTTP response received
volatile uint8_t  rsp_received;

//! End of http put file/content
volatile uint8_t  end_of_file = 0;
volatile uint8_t  end_of_data = 0;

//! http get receive done
volatile uint8_t http_recv_done;
volatile uint8_t http_post_start_done;
volatile uint8_t http_post_data_done;

//! data buffer length
uint32_t buf_len;

//! Application Buffer
uint8_t app_buf[APP_BUFF_LEN];

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! prototypes
#if RSI_HTTP_STATUS_INDICATION_EN
void rsi_http_get_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata, uint16_t status_code);
void rsi_http_post_data_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata, uint16_t status_code);
#else 
void rsi_http_get_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata);
void rsi_http_post_data_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata);
#endif

void rsi_http_post_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata);
//! Http client Application 
int32_t rsi_http_client_app()
{
  int32_t     status       = RSI_SUCCESS;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
  uint8_t      flags       = FLAGS;
#if RSI_ENABLE_HTTP_ABORT	
  int count = 0;
#endif	
  uint32_t offset          = 0;
  uint32_t chunk_length    = 0;
  uint8_t  *file_content   = NULL;
  
  //! Get the length of the index html page
  uint32_t file_size = (sizeof(rsi_index) - 1);

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
	if(flags & HTTPS_SUPPORT)
	{
		//! Load certificates
		status  = rsi_wlan_set_certificate(5, cacert, (sizeof(cacert)-1));
		if(status != RSI_SUCCESS)
		{
			LOG_PRINT("\r\nLoad Certificate Failed, Error Code : 0x%X\r\n",status);
			return status;
		}
		else
		{
			LOG_PRINT("\r\nLoad Certificate Success\r\n");
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

  //! send http post request for the given url with given http data
  status = rsi_http_client_post_async((uint8_t)flags, (uint8_t *)HTTP_SERVER_IP_ADDRESS, (uint16_t)HTTP_PORT, 
      (uint8_t *)HTTP_URL, (uint8_t *)HTTP_HOSTNAME,
      (uint8_t *)HTTP_EXTENDED_HEADER, (uint8_t *)USERNAME,
      (uint8_t *)PASSWORD, NULL, file_size, rsi_http_post_response_handler);

  //! wait for Driver task to complete
  do
  {
    //! event loop 
    rsi_wireless_driver_task();

  }while(!http_post_start_done);

  if(http_post_start_done != 1)
  {
    return  rsi_wlan_get_status();
  }

  //! HTTP client post success response got received
  //! resetting http recv done
  http_post_start_done = 0;
  

  //! HTTP client POST DATA packet
  while(!end_of_data)
  {
    //! Get the current length that you want to send
    chunk_length = ((file_size - offset) > MAX_HTTP_CLIENT_POST_DATA_BUFFER_LENGTH) 
      ? MAX_HTTP_CLIENT_POST_DATA_BUFFER_LENGTH : (file_size - offset);

    //! Get http content
    file_content = (uint8_t *)(rsi_index + offset);

#if 0
    //! Send resource content to the HTTP server for the given URL and total content length 
    if((count % 10) == 0)
    {
      Sleep(1);
    }
#endif

    //! send http post data request for the given url with given http data
    status = rsi_http_client_post_data((uint8_t *)file_content, (uint16_t)chunk_length, rsi_http_post_data_response_handler);

    //! wait for the success response 
    do
    {
      //! event loop 
#ifndef RSI_WITH_OS
      rsi_wireless_driver_task();
#endif

#if RSI_ENABLE_HTTP_ABORT
     //!HTTP ABORT 
     count++;
     if(count> RSI_HTTP_MAX_PKT_COUNT_ABORT)
     {
        count = 0;
        status = rsi_http_client_abort();
        if(status != RSI_SUCCESS)
        {
          //! Error 
          return;
        }
        end_of_data=1;
        http_post_data_done = 1;
      }
#endif

    }while(!http_post_data_done);

    if(http_post_data_done == 3)
    {
      return  rsi_wlan_get_status();
    }

    //! Recevied HTTP post data command response
    //! Move content offset by chunk length
    offset += chunk_length;

    http_post_data_done = 0;
  }

  //! wait for Driver task to complete
  do
  {
    //! event loop 
    rsi_wireless_driver_task();

  }while(!http_recv_done);


  if(http_recv_done != 2)
  {
    return  rsi_wlan_get_status();
  }

  //! Recevied HTTP post data from HTTP server
  //! resetting buffer length
  buf_len = 0;

  //! resetting http recv done
  http_recv_done = 0;


  //! Set flags with HTTP 1.1 version to process HTTP get reuest
  flags = HTTP_V_1_1; 

  //! send http get request for the given url
  status = rsi_http_client_get_async((uint8_t)flags, (uint8_t *)HTTP_SERVER_IP_ADDRESS, (uint16_t)HTTP_PORT, 
      (uint8_t *)HTTP_URL, (uint8_t *)HTTP_HOSTNAME,
      (uint8_t *)HTTP_EXTENDED_HEADER, (uint8_t *)USERNAME, (uint8_t *)PASSWORD,
      rsi_http_get_response_handler);

  //! wait for Driver task to complete
  do
  {
    //! event loop 
    rsi_wireless_driver_task();

  }while(!http_recv_done);


  //! resetting buffer length
  buf_len = 0;

  //! resetting http recv done
  http_recv_done = 0;

  return status;
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

	//! Redpine module intialization
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
  rsi_task_create(rsi_http_client_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call HTTP client application
  status = rsi_http_client_app();

  //! Application main loop
  main_loop();
#endif
  return status;

}



//! http get response notify call back handler 
#if RSI_HTTP_STATUS_INDICATION_EN
void rsi_http_get_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata, uint16_t status_code)
#else 
void rsi_http_get_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata)
#endif
{
  if(buf_len + length > APP_BUFF_LEN)
  {
    //! Application buffer over flowed
    return ;
  }

  if(!moredata)
  {
    //! copy received data to application buffer
    memcpy(app_buf + buf_len, buffer, length);

    //! update buffer length
    buf_len += length;
  }
  else
  {
    if(length)
    {
      //! copy received data to application buffer
      memcpy(app_buf + buf_len, buffer, length);

      //! update buffer length
      buf_len += length;
    }
    //! set to exit from driver task
    http_recv_done = 1;
  }
}





//! http post response notify call back handler 
void rsi_http_post_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata)
{
  if(status == RSI_SUCCESS)
  {
    if((FLAGS & HTTP_POST_DATA) && (moredata & RSI_HTTP_POST_RSP))
    {
      //! Set HTTP POST start response received 
      http_post_start_done = 1;
    }
  }
  else
  {
    //! Error response received
    http_post_start_done = 2;
  }
}


//! http post data response notify call back handler 
#if RSI_HTTP_STATUS_INDICATION_EN
void rsi_http_post_data_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata, uint16_t status_code)
#else 
void rsi_http_post_data_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length, const uint32_t moredata)
#endif
{
  if(buf_len + length > APP_BUFF_LEN)
  {
    //! Application buffer over flowed
    return ;
  }

  if(status == RSI_SUCCESS)
  {
    if(FLAGS & HTTP_POST_DATA)
    {
      
      if(moredata & RSI_HTTP_POST_DATA_RSP)
      {
        http_post_data_done = 1;

        //! Set End of data
        end_of_data  = (moredata & RSI_HTTP_END_OF_DATA) ? 1 : 0;
      }
      else if(moredata & RSI_HTTP_POST_RCV_RSP)
      {
        if(!(moredata & RSI_HTTP_END_OF_DATA))
        {
          //! copy received data to application buffer
          memcpy(app_buf + buf_len, buffer, length);

          //! update buffer length
          buf_len += length;
        }
        else
        {
          if(length)
          {
            //! copy received data to application buffer
            memcpy(app_buf + buf_len, buffer, length);

            //! update buffer length
            buf_len += length;
          }

          //! set to exit from driver task
          http_recv_done = 2;
        }
      }
    }
  }
  else
  {
    http_post_data_done = 3;
    http_recv_done = 3;
  }
}

