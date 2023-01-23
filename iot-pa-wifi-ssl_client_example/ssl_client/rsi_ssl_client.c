/*******************************************************************************
* @file  rsi_ssl_client.c
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
 * @brief : This file contains example application for SSL Client
 * @section Description :
 * This application demonstrates how to open and use a standard TCP client
 * socket with secure connection using SSL and sends data on socket.
 =================================================================================*/

/**
 * Include files
 * */

//! Driver include file to refer driver APIs
#include "rsi_driver.h"

//! WLAN include file to refer WLAN APIs
#include "rsi_wlan_apis.h"
#include "rsi_wlan_non_rom.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

#include "rsi_utils.h"

//! Access point SSID to connect
#define SSID                        "SILABS_AP"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO                       0

//! Security type
#define SECURITY_TYPE               RSI_WPA2

//! Password
#define PSK                         "1234567890"

#define RSI_SSL_BIT_ENABLE             0

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE                      1

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module (E.g: 192.168.10.101)
#define DEVICE_IP  "192.168.10.101"

//! IP address of Gateway (E.g: 192.168.10.1)
#define GATEWAY    "192.168.10.1"

//! IP address of Netmask (E.g: 255.255.255.0)
#define NETMASK    "255.255.255.0"

#endif

//! Device port number
#define DEVICE_PORT                        5001

//! Server port number
#define SERVER_PORT                        5001

//! Server IP address. (E.g: 192.168.10.100)
#define SERVER_IP_ADDRESS               "192.168.10.100"

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS                   1000

//! Memory length for driver
#define GLOBAL_BUFF_LEN                     15000

//! WLAN task priority
#define RSI_WLAN_TASK_PRIORITY               1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY             2

//! WLAN task stack size
#define RSI_WLAN_TASK_STACK_SIZE             500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE           500

//! Enable SSL or not
#define SSL                                   1

//! Load certificate to device flash :
//! Certificate could be loaded once and need not be loaded for every boot up
#define LOAD_CERTIFICATE                      1

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

#if SSL
//! Include SSL CA certificate
#include "cacert.pem"
#endif

int32_t rsi_ssl_client()
{
  int32_t client_socket;
  struct rsi_sockaddr_in server_addr, client_addr;
  int32_t status       = RSI_SUCCESS;
  int32_t packet_count = 0;
  uint8_t ssl_bit_map  = RSI_SSL_BIT_ENABLE;
#if !(DHCP_MODE)
  uint32_t ip_addr      = ip_to_reverse_hex((char *)DEVICE_IP);
  uint32_t network_mask = ip_to_reverse_hex((char *)NETMASK);
  uint32_t gateway      = ip_to_reverse_hex((char *)GATEWAY);
#else
  uint8_t dhcp_mode = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif

#ifdef RSI_WITH_OS
  rsi_task_handle_t driver_task_handle = NULL;
#endif

  //! Driver Initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > GLOBAL_BUFF_LEN)) {
    return status;
  }

  //! Silabs Module Initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nDevice Initialization Success\r\n");
  }
	
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
  status = rsi_wireless_init(0, 0);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nWireless Initialization Success\r\n");
  }

#if LOAD_CERTIFICATE
#if SSL
  //! Load SSL CA certificate
  status = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, cacert, (sizeof(cacert) - 1));
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nLoad SSL CA Certificate Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nLoad SSL CA Certificate Success\r\n");
  }
#endif
#endif

  //! Scan Access points
  status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWLAN AP Scan Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nWLAN AP Scan Success\r\n");
  }

  //! Connect to an Access point
  status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWLAN AP Connect Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nWLAN AP Connect Success\r\n");
  }

  //! Configure IP
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0, 0);
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
#endif
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nIP Config Success\r\n");
  }

  //! Create socket
#if SSL
  client_socket = rsi_socket(AF_INET, SOCK_STREAM, 1);
#else
  client_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
#endif
  if (client_socket < 0) {
    status = rsi_wlan_get_status();
    LOG_PRINT("\r\nSocket Create Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nSocket Create Success\r\n");
  }

  status = rsi_setsockopt(client_socket, SOL_SOCKET, SO_SSL_ENABLE, &ssl_bit_map, sizeof(ssl_bit_map));
  if (status != RSI_SUCCESS) {
    status = rsi_wlan_get_status();
    LOG_PRINT("\r\nSet Socket Options Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nSet Socket Options Success\r\n");
  }

  //! Memset client structure
  memset(&client_addr, 0, sizeof(client_addr));

  //! Set family type
  client_addr.sin_family = AF_INET;

  //! Set local port number
  client_addr.sin_port = htons(DEVICE_PORT);

  //! Bind socket
  status = rsi_bind(client_socket, (struct rsi_sockaddr *)&client_addr, sizeof(client_addr));
  if (status != RSI_SUCCESS) {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket, 0);
    LOG_PRINT("\r\nBind Failed, Error code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nBind Success\r\n");
  }

  //! Set server structure
  memset(&server_addr, 0, sizeof(server_addr));

  //! Set server address family
  server_addr.sin_family = AF_INET;

  //! Set server port number, using htons function to use proper byte order
  server_addr.sin_port = htons(SERVER_PORT);

  //! Set IP address to local-host
  server_addr.sin_addr.s_addr = ip_to_reverse_hex((char *)SERVER_IP_ADDRESS);

  //! Connect to server socket
  status = rsi_connect(client_socket, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
  if (status != RSI_SUCCESS) {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket, 0);
    LOG_PRINT("\r\nConnect to Server Socket Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nConnect to Server Socket Success\r\n");
  }

	LOG_PRINT("\r\nSSL Tx data start...\r\n");
	
  while (packet_count < NUMBER_OF_PACKETS) {
    //! Send data on socket
    status = rsi_send(client_socket, (int8_t *)"Hello from SSL TCP client!!!", (sizeof("Hello from SSL TCP client!!!") - 1), 0);
    if (status < 0) {
      status = rsi_wlan_get_status();
      rsi_shutdown(client_socket, 0);
      LOG_PRINT("\r\nFailed to Send data on socket, Error Code : 0x%lX\r\n", status);
      return status;
    }
    packet_count++;
  }
	LOG_PRINT("\r\nSSL Tx data end...\r\n");

  return 0;
}

void main_loop(void)
{
  while (1) {
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
#endif

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create((rsi_task_function_t)rsi_ssl_client,
                  (uint8_t *)"wlan_task",
                  RSI_WLAN_TASK_STACK_SIZE,
                  NULL,
                  RSI_WLAN_TASK_PRIORITY,
                  &wlan_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call SSL client application
  status = rsi_ssl_client();

  //! Application main loop
  main_loop();
#endif
  return status;
}
