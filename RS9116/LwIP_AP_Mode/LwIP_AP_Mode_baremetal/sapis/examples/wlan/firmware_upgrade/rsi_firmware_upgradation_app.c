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
 * @brief : This file contains example application for firmware upgradation from
 * server
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

//! Error include files
#include "rsi_error.h"

#include "rsi_bootup_config.h"
#include "rsi_utils.h"
//! OS include file to refer OS specific functionality
#include "rsi_os.h"


//! socket include file to firmware upgrade APIs
#include "rsi_firmware_upgradation.h"

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#include "rsi_chip.h"
#endif

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
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x640AA8C0

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


//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

int32_t rsi_firmware_upgradation_app()
{
  int32_t     client_socket;
  struct      rsi_sockaddr_in server_addr, client_addr;
  int32_t     status       = RSI_SUCCESS;
  int32_t     recv_size = 0;
  uint8_t     resp_buf[20];	
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
  uint8_t     send_buffer[3];
  uint8_t      recv_buffer[RECV_BUFFER_SIZE];
  uint32_t    chunk = 1, fwup_chunk_length, recv_offset = 0, fwup_chunk_type;

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

	status = rsi_wlan_get(RSI_FW_VERSION,resp_buf,18);

	LOG_PRINT("\r\nFirmware version before upgrade : %s\r\n",resp_buf);

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
  client_addr.sin_port = htons(DEVICE_PORT);

	//! Bind socket
	status = rsi_bind(client_socket, (struct rsi_sockaddr *) &client_addr, sizeof(client_addr));
	if(status != RSI_SUCCESS)
	{
		status = rsi_wlan_get_status();
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
		LOG_PRINT("\r\nSocket Create Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nConnect to TCP Server Success\r\n");
	}

	LOG_PRINT("\r\nFirmware upgrade start\r\n");

  while(1)
  {
    //! Fill packet type
    if(chunk == 1)
    {
      send_buffer[0] = RSI_FWUP_RPS_HEADER;
    }
    else
    {
      send_buffer[0] = RSI_FWUP_RPS_CONTENT;
    }

    //! Fill packet number
    rsi_uint16_to_2bytes(&send_buffer[1], chunk);

		//! Send firmware upgrade request to remote peer
		status = rsi_send(client_socket,(int8_t *)send_buffer, 3, 0);
		if(status < 0)
		{
			status = rsi_wlan_get_status();
			LOG_PRINT("\r\nFailed to Send data to TCP Server, Error Code : 0x%X\r\n",status);
			return status;
		}

		//! Get first 3 bytes from remote peer
		recv_offset = 0;
		recv_size = 3;
		do
		{
			status = rsi_recv(client_socket, (recv_buffer + recv_offset), recv_size, 0);
			if(status < 0)
			{
				status = rsi_wlan_get_status();
				LOG_PRINT("\r\nFailed to Receive data, Error Code : 0x%X\r\n",status);
				return status;
			}

      //! Subtract received bytes from required bytes
      recv_size -= status;

      //! Move the receive offset
      recv_offset += status;

    }while(recv_size > 0);

    //! Get the received chunk type
    fwup_chunk_type   = recv_buffer[0];

    //! Get the received chunk length
    fwup_chunk_length = rsi_bytes2R_to_uint16(&recv_buffer[1]);



		//! Get packet of chunk length from remote peer
		recv_offset = 0;
		recv_size = fwup_chunk_length;
		do
		{
			status = rsi_recv(client_socket, (recv_buffer + recv_offset), recv_size, 0);
			if(status < 0)
			{
				status = rsi_wlan_get_status();
				LOG_PRINT("\r\nFailed to Receive data from remote peer, Error Code : 0x%X\r\n",status);
				return status;
			}

      //! Subtract received bytes from required bytes
      recv_size -= status;

      //! Move the receive offset
      recv_offset += status;

    }while(recv_size > 0);

    //! Call corresponding firmware upgrade API based on the chunk type
    if(fwup_chunk_type == RSI_FWUP_RPS_HEADER)
    {
      //! Send RPS header which is received as first chunk 
      status = rsi_fwup_start(recv_buffer);
    }
    else
    {
      //! Send RPS content
      status = rsi_fwup_load(recv_buffer, fwup_chunk_length);
    }

    if(status != RSI_SUCCESS)
    {
      if(status == 3)
      {
        //! Close the socket
        rsi_shutdown(client_socket, 0);

        return 0;
      }
      return status;
    }
    chunk++;
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

  }
}

int main()
{
  volatile int32_t status;
  uint8_t     resp_buf[20];
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
	if(status == 0)
	{		
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
	}
	else
	{
		LOG_PRINT("\r\nFirmware upgrade failed\n");
	}
	//! Application main loop
	main_loop();
#endif
  return status;

}

