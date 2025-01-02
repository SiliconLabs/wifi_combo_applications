/**
 * @file    rsi_three_ssl_client_sockets.c
 * @version 1.0
 * @date    15 Apr 2020
 *
 *  Copyright(C) Redpine Signals 2020
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for three TCP client socket connections over SSL with the certificates loaded in to FLASH
 *
 *  @section Description  This file contains example application for three TCP client socket connections over SSL with the SSL certificates loaded in to FLASH
 *
 *
 */

/**
 * Include files
 * */

//! Driver include file to refer driver APIs
#include "rsi_driver.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_utils.h"
//! Certificates to be loaded
#include "cacert.pem"
#include "starfieldca.pem"
#include "client_certificate.crt"
#include "privatekey.key"

//! Access point SSID to connect
#define SSID              "REDPINE_AP"

//!Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO        0

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               NULL

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE         1

//! If DHCP mode is disabled give IP statically
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

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_ADDR       0x650AA8C0

//! AWS Domain name
#define AWS_DOMAIN_NAME       "a25jwtlmds8eip-ats.iot.us-east-2.amazonaws.com"

//! SSL bitmap 1- Enable 0- Disable
#define RSI_SSL_BIT_ENABLE     1

//! Device port number for 1st connection
#define DEVICE_PORT1           5001

//! Device port number for 2nd connection
#define DEVICE_PORT2           5003

//! Device port number for 3rd connection
#define DEVICE_PORT3           5005

//! Server port number for 1st connection
#define SERVER_PORT1           5001

//! Server port number for 2nd connection
#define SERVER_PORT2           5003

//! Server port number for 3rd connection
#define SERVER_PORT3           8883

//! Memory length for driver
#define GLOBAL_BUFF_LEN        15000

#define MAX_SSL_CERTS_TO_FLASH   3

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY      1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY    2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE    500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];


int32_t rsi_three_ssl_client_sockets_app()
{
  int32_t     client_socket1;
  int32_t     client_socket2;
  int32_t     client_socket3;
  struct      rsi_sockaddr_in client_addr1, server_addr1;
  struct      rsi_sockaddr_in client_addr2, server_addr2;
  struct      rsi_sockaddr_in client_addr3, server_addr3;
  int32_t     status       = RSI_SUCCESS;
  uint8_t     ssl_bit_map  = RSI_SSL_BIT_ENABLE ; 
  uint32_t ssl_cert_bit_map = RSI_CERT_INDEX_0;
  uint8_t     i=0; 
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
  uint32_t    server_address =  0;
  rsi_rsp_dns_query_t dns_query_rsp;

  //! WC initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  for(i=0;i<MAX_SSL_CERTS_TO_FLASH;i++) 
  {
    //! Clearing SSL CA certificate loaded in to FLASH if any with index i
    status  = rsi_wlan_set_certificate_index(RSI_SSL_CA_CERTIFICATE, i, NULL, NULL);
    if(status != RSI_SUCCESS)
    {
      return status;
    }

    //! Clearing SSL Client certificate loaded in to FLASH if any with index i
    status  = rsi_wlan_set_certificate_index(RSI_SSL_CLIENT, i, NULL, NULL);
    if(status != RSI_SUCCESS)
    {
      return status;
    }

    //! Clearing SSL private key loaded in to FLASH if any with index i
    status  = rsi_wlan_set_certificate_index(RSI_SSL_CLIENT_PRIVATE_KEY, i, NULL, NULL);
    if(status != RSI_SUCCESS)
    {
      return status;
    }
  }

  //! Loading SSL CA certificate in to FLASH with index 0
  status = rsi_wlan_set_certificate_index(RSI_SSL_CA_CERTIFICATE, RSI_CERT_INDEX_0,cacert, (sizeof(cacert)-1) );
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Loading SSL CA certificate in to FLASH with index 1
  status = rsi_wlan_set_certificate_index(RSI_SSL_CA_CERTIFICATE, RSI_CERT_INDEX_1,cacert, (sizeof(cacert)-1) );
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Loading SSL CA certificate in to FLASH with index 2
  status  = rsi_wlan_set_certificate_index(RSI_SSL_CA_CERTIFICATE,RSI_CERT_INDEX_2, starfieldca, (sizeof(starfieldca)-1));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Loading SSL Client certificate in to FLASH with index 2
  status  = rsi_wlan_set_certificate_index(RSI_SSL_CLIENT,RSI_CERT_INDEX_2, awscertificate, (sizeof(awscertificate)-1));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Loading SSL private key in to FLASh with index 2
  status  = rsi_wlan_set_certificate_index(RSI_SSL_CLIENT_PRIVATE_KEY,RSI_CERT_INDEX_2, awsprivatekey, (sizeof(awsprivatekey)-1));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Scan Access points
  status = rsi_wlan_scan((int8_t *)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
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

  //! Configure IP 
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0,0);
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  do
  {
    //! Getting IP address of the AWS server using DNS request
    status = rsi_dns_req(RSI_IP_VERSION_4, (uint8_t *)AWS_DOMAIN_NAME, NULL, NULL, &dns_query_rsp, sizeof(dns_query_rsp));

  }while(status != RSI_SUCCESS);

  server_address = rsi_bytes4R_to_uint32(dns_query_rsp.ip_address[0].ipv4_address);

  //! 1st Socket ceration 
  client_socket1 = rsi_socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket1 < 0)
  {
    status = rsi_wlan_get_status();
    return status;
  }
  ssl_cert_bit_map = RSI_CERT_INDEX_0;
  //! Setting SSL socket option
  status=rsi_setsockopt(client_socket1, SOL_SOCKET, SO_SSL_ENABLE, &ssl_bit_map, sizeof(ssl_bit_map));
  if(status != RSI_SUCCESS)
  {
    return status;
  }
	status = rsi_setsockopt(client_socket1,SOL_SOCKET,SO_CERT_INDEX,&ssl_cert_bit_map,sizeof(ssl_cert_bit_map));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Memset client structure
  memset(&client_addr1, 0, sizeof(client_addr1));  

  //! Set family type
  client_addr1.sin_family= AF_INET;

  //! Set local port number
  client_addr1.sin_port = htons(DEVICE_PORT1);

  //! Bind socket
  status = rsi_bind(client_socket1, (struct rsi_sockaddr *) &client_addr1, sizeof(client_addr1));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket1, 0);
    return status;
  }

  //! Set server structure
  memset(&server_addr1, 0, sizeof(server_addr1));  

  //! Set server address family
  server_addr1.sin_family = AF_INET;

  //! Set server port number, using htons function to use proper byte order
  server_addr1.sin_port = htons(SERVER_PORT1);

  //! Set IP address to localhost
  server_addr1.sin_addr.s_addr = SERVER_ADDR; 

  //! Connect to server socket
  status = rsi_connect(client_socket1, (struct rsi_sockaddr *) &server_addr1, sizeof(server_addr1));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket1, 0);
    return status;
  }


  //! 2nd Socket ceration 
  client_socket2 = rsi_socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket2 < 0)
  {
    status = rsi_wlan_get_status();
    return status;
  }
  ssl_cert_bit_map = RSI_CERT_INDEX_1;
  //! Setting SSL socket option
  status=rsi_setsockopt(client_socket2, SOL_SOCKET, SO_SSL_ENABLE, &ssl_bit_map, sizeof(ssl_bit_map));
  if(status != RSI_SUCCESS)
  {
    return status;
  }
	status = rsi_setsockopt(client_socket2,SOL_SOCKET,SO_CERT_INDEX,&ssl_cert_bit_map,sizeof(ssl_cert_bit_map));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Memset client structure
  memset(&client_addr2, 0, sizeof(client_addr2));  

  //! Set family type
  client_addr2.sin_family= AF_INET;

  //! Set local port number
  client_addr2.sin_port = htons(DEVICE_PORT2);

  //! Bind socket
  status = rsi_bind(client_socket2, (struct rsi_sockaddr *) &client_addr2, sizeof(client_addr2));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket2, 0);
    return status;
  }

  //! Set server structure
  memset(&server_addr2, 0, sizeof(server_addr2));  

  //! Set server address family
  server_addr2.sin_family = AF_INET;

  //! Set server port number, using htons function to use proper byte order
  server_addr2.sin_port = htons(SERVER_PORT2);

  //! Set IP address to localhost
  server_addr2.sin_addr.s_addr = SERVER_ADDR; 

  //! Connect to server socket
  status = rsi_connect(client_socket2, (struct rsi_sockaddr *) &server_addr2, sizeof(server_addr2));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket2, 0);
    return status;
  }


  //! 3rd Socket ceration 
  client_socket3 = rsi_socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket3 < 0)
  {
    status = rsi_wlan_get_status();
    return status;
  }
  ssl_cert_bit_map = RSI_CERT_INDEX_2;
  //! Setting SSL socket option
	status = rsi_setsockopt(client_socket3,SOL_SOCKET,SO_CERT_INDEX,&ssl_cert_bit_map,sizeof(ssl_cert_bit_map));
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  status=rsi_setsockopt(client_socket3, SOL_SOCKET, SO_SSL_ENABLE, &ssl_bit_map, sizeof(ssl_bit_map));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Memset client structure
  memset(&client_addr3, 0, sizeof(client_addr3));  

  //! Set family type
  client_addr3.sin_family= AF_INET;

  //! Set local port number
  client_addr3.sin_port = htons(DEVICE_PORT3);

  //! Bind socket
  status = rsi_bind(client_socket3, (struct rsi_sockaddr *) &client_addr3, sizeof(client_addr3));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket3, 0);
    return status;
  }

  //! Set server structure
  memset(&server_addr3, 0, sizeof(server_addr3));  

  //! Set server address family
  server_addr3.sin_family = AF_INET;

  //! Set server port number, using htons function to use proper byte order
  server_addr3.sin_port = htons(SERVER_PORT3);

  //! Set IP address to localhost
  server_addr3.sin_addr.s_addr = server_address; 

  //! Connect to server socket
  status = rsi_connect(client_socket3, (struct rsi_sockaddr *) &server_addr3, sizeof(server_addr3));
  if(status != RSI_SUCCESS)
  {
    status = rsi_wlan_get_status();
    rsi_shutdown(client_socket3, 0);
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
	rsi_task_create(rsi_three_ssl_client_sockets_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call SSL client with certificates loaded in to RAM application
	status = rsi_three_ssl_client_sockets_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}

