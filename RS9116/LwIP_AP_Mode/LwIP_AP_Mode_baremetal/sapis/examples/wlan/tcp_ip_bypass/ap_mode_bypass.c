/*
 * ap_mode_bypass.c
 *
 *  Created on: Sep 25, 2024
 *      Author: vikastur
 */


/*******************************************************************************
 * @file  rsi_ap_start.c
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
 * @brief : This file contains example application for starting access point.
 * @section Description :
 * The AP start application demonstrates how to configure the SiLabs device as
 * a soft Access point and allows stations to connect to it.
 * It also enables TCP data transmission from the connected Wi-Fi station to
 * SiLabsAccess Point.
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
#include "rsi_wlan_non_rom.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_bootup_config.h"

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

#include "string.h"
#include "rsi_driver.h"


//LWIP

#include "lwip/netif.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "lwip/etharp.h"
#include "lwip/apps/lwiperf.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"
#include "sl_status.h"
#include "lwip/apps/fs.h"
#include "lwip/timeouts.h"

//DHCP Server
#include "dhcp_server.h"

/************************** Access Point Static Default ****************************/
#define AP_IP_ADDR0   (uint8_t) 192    ///< Static IP: IP address value 0
#define AP_IP_ADDR1   (uint8_t) 168    ///< Static IP: IP address value 1
#define AP_IP_ADDR2   (uint8_t) 1     ///< Static IP: IP address value 2
#define AP_IP_ADDR3   (uint8_t) 1     ///< Static IP: IP address value 3

/*NETMASK*/
#define AP_NETMASK_ADDR0   (uint8_t) 255 ///< Static IP: Netmask value 0
#define AP_NETMASK_ADDR1   (uint8_t) 255 ///< Static IP: Netmask value 1
#define AP_NETMASK_ADDR2   (uint8_t) 255 ///< Static IP: Netmask value 2
#define AP_NETMASK_ADDR3   (uint8_t) 0   ///< Static IP: Netmask value 3

/*Gateway Address*/
#define AP_GW_ADDR0   (uint8_t) 192        ///< Static IP: Gateway value 0
#define AP_GW_ADDR1   (uint8_t) 168        ///< Static IP: Gateway value 1
#define AP_GW_ADDR2   (uint8_t) 1        ///< Static IP: Gateway value 2
#define AP_GW_ADDR3   (uint8_t) 1        ///< Static IP: Gateway value 3

err_t low_level_output(struct netif *netif, struct pbuf *p);

struct netif ap_netif;
struct dhcp *v4dhcp;
#if 1
/* SoftAP IP address */
uint8_t ap_ip_addr0 = AP_IP_ADDR0;
uint8_t ap_ip_addr1 = AP_IP_ADDR1;
uint8_t ap_ip_addr2 = AP_IP_ADDR2;
uint8_t ap_ip_addr3 = AP_IP_ADDR3;
uint8_t ap_netmask_addr0 = AP_NETMASK_ADDR0;
uint8_t ap_netmask_addr1 = AP_NETMASK_ADDR1;
uint8_t ap_netmask_addr2 = AP_NETMASK_ADDR2;
uint8_t ap_netmask_addr3 = AP_NETMASK_ADDR3;
uint8_t ap_gw_addr0 = AP_GW_ADDR0;
uint8_t ap_gw_addr1 = AP_GW_ADDR1;
uint8_t ap_gw_addr2 = AP_GW_ADDR2;
uint8_t ap_gw_addr3 = AP_GW_ADDR3;
#endif
/////////

//! Access point SSID to be created
#define SSID "LwIP_Test_AP"

//! Channel number
#define CHANNEL_NO 7

//! Security type
#define SECURITY_TYPE RSI_WPA2

//! Encryption type
#define ENCRYPTION_TYPE RSI_CCMP

//! Password
#define PSK "12345678"

//! Beacon interval
#define BEACON_INTERVAL 100

//! DTIM interval
#define DTIM_INTERVAL 4

//! IP address of the module
#define DEVICE_IP  "192.168.10.1"

//! IP address of Gateway
#define GATEWAY "192.168.10.1"

//! IP address of netmask
#define NETMASK "255.255.255.0"

//! Device port number
#define DEVICE_PORT 5001

//! Number of packets to receive
#define NUMBER_OF_PACKETS 1000

//! Receive data length
#define RECV_BUFFER_SIZE 1000

//! Memory length for driver
#define GLOBAL_BUFF_LEN 15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY 1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY 2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE 500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE 500

#define SOCKET_ASYNC_FEATURE 1

//! To turnoff the AP at the end
#define AP_TURN_OFF 0

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Receive buffer
uint8_t recv_buffer[RECV_BUFFER_SIZE];

uint8_t station_mac[6];
uint32_t received_length = 0;
int32_t packet_count     = 0;
uint8_t fw_version[20];
uint8_t mac_address[6];
uint8_t ip_address[4];
#ifdef RSI_WITH_OS
#if SOCKET_ASYNC_FEATURE
rsi_semaphore_handle_t ap_start_app;
#endif
#endif

//LWIP
uint8_t sta_connected= 0;
#define SOFTAP_NETIF0  'a'
#define SOFTAP_NETIF1  'p'

// Define the DHCP pool start and end IP addresses
#define DHCP_POOL_START_IP "192.168.1.1"
#define DHCP_POOL_END_IP   "192.168.1.5"

void low_level_init(struct netif *netif);
/***************************************************************************//**
 * Transmits packet(s).
 *
 * @param netif the lwip network interface structure
 * @param p the packet to send
 * @returns ERR_OK if successful
 ******************************************************************************/
err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  struct pbuf *q;
  int32_t status =RSI_SUCCESS;

  // printf("\r\n inside low_level_output \r\n");
  for(q=p; q!=NULL; q=q->next)
    {
      status = rsi_send_raw_data((uint8_t *)p->payload, p->len);
      if (status != ERR_OK)
        {
          return status;
        }
    }
}


void print_ip_config(struct netif *netif) {
  printf("IP: %s\n", ip4addr_ntoa(&netif->ip_addr));
  printf("Netmask: %s\n", ip4addr_ntoa(&netif->netmask));
  printf("Gateway: %s\n", ip4addr_ntoa(&netif->gw));
}



void wlan_data_receive_handler (uint16_t status, uint8_t *buffer, const uint32_t length)
{
#if 1
  struct pbuf *pkt = NULL;
  struct netif *netif;
  int err;

  netif = &ap_netif;

  //printf("\r\n inside wlan_data_receive_handler \r\n");
  //  for (size_t i = 0; i < length; i++) {
  //      printf("%c", buffer[i]);
  //  }

  if (netif != NULL)
    {

      pkt = pbuf_alloc (PBUF_RAW, length, PBUF_POOL);

      if (pkt != NULL)
        {
          memcpy (pkt->payload, buffer, length);
          pkt->len = length;
          netif->input (pkt, netif);
        }
      else{
          printf("\r\n error in pbuf_alloc \r\n");
      }

    }
#else

  struct pbuf *p = NULL;
  printf("\r\n wlan_data_receive_handler \r\n");
  for (size_t i = 0; i < length; i++) {
      printf("%c", buffer[i]);
  }
  p= pbuf_alloc(PBUF_TRANSPORT, (uint16_t)length, PBUF_RAM);
  if (p != NULL) {
      memcpy(p->payload, buffer, length);
      if (ap_netif.input(p, &ap_netif) != ERR_OK) {
          pbuf_free(p);
      }
      else{
          printf("\r\n error in calling the input function of LwIP \r\n");
      }
  }
  else{
      printf("\r\n error in sending data to LwIP \r\n");
  }
#endif
}






void check_dhcp_assignment(struct netif *netif) {
  if (dhcp_supplied_address(netif)) {
      printf("DHCP server assigned IP address: %s\n", ip4addr_ntoa(&netif->ip_addr));
  } else {
      //printf("No IP address assigned by DHCP server.\n");
  }
}


//! This callback function is called when Station is connected.
//! Buffer has the MAC address of the station connected
void stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  // printf("\r\nStation Connected\r\n");
  // UNUSED_PARAMETER(status);       //This statement is added only to resolve compilation warning, value is unchanged
  // UNUSED_CONST_PARAMETER(length); //This statement is added only to resolve compilation warning, value is unchanged
  sta_connected =1;
  printf("\r\n STA connected \r\n");
  memcpy(station_mac, buffer, 6);
  // printf("\r\n MAC of STA %x\r\n",station_mac);
  printf ("\r\nSTA Mac address is :%x:%x:%x:%x:%x:%x\r\n", station_mac[0],station_mac[1],station_mac[2],
          station_mac[3],station_mac[4],station_mac[5]);
}

//! This callback function is called when Station is disconnected.
//! Buffer has the MAC address of the station disconnected
void stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  printf("\r\nStation Disconnected\r\n");
   //UNUSED_PARAMETER(status);       //This statement is added only to resolve compilation warning, value is unchanged
   //UNUSED_PARAMETER(buffer);       //This statement is added only to resolve compilation warning, value is unchanged
   //UNUSED_CONST_PARAMETER(length); //This statement is added only to resolve compilation warning, value is unchanged
}

//! Call back for Socket Async
void socket_async_recive(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
   //UNUSED_PARAMETER(sock_no); //This statement is added only to resolve compilation warning, value is unchanged
  // UNUSED_PARAMETER(buffer);  //This statement is added only to resolve compilation warning, value is unchanged
  //  UNUSED_PARAMETER(length);  //This statement is added only to resolve compilation warning, value is unchanged
  received_length += length;
  packet_count++;
}

void rsi_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  // UNUSED_PARAMETER(status);
  //  UNUSED_PARAMETER(buffer);
  //  UNUSED_CONST_PARAMETER(length);
   printf("\r\nRemote socket terminated\r\n");

#ifdef RSI_WITH_OS
#if SOCKET_ASYNC_FEATURE
  rsi_semaphore_post(&ap_start_app);
#endif
#endif
}

err_t my_netif_init(struct netif *netif)
{


  //Step2. LwIP initialization
  netif->name[0] = SOFTAP_NETIF0;
  netif->name[1] = SOFTAP_NETIF1;

  // Set up LwIP link functions
  netif->linkoutput = low_level_output;
  netif->output = etharp_output;  // Typically for ARP handling

  low_level_init(netif);
  ap_netif = *netif;

  return ERR_OK;
}

int32_t rsi_ap_start()
{
  int32_t status        = RSI_SUCCESS;
#if 0
  int32_t server_socket, new_socket;
  struct rsi_sockaddr_in server_addr, client_addr;

  uint32_t ip_addr      = ip_to_reverse_hex(DEVICE_IP);
  uint32_t network_mask = ip_to_reverse_hex(NETMASK);
  uint32_t gateway      = ip_to_reverse_hex(GATEWAY);
  int32_t addr_size;
#endif
  // sl_status_t status;
  ip_addr_t  ap_ipaddr;
  ip_addr_t  ap_netmask;
  ip_addr_t  ap_gw;

  // Driver initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > GLOBAL_BUFF_LEN)) {
      return status;
  }

  //! Silabs module initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if (status != RSI_SUCCESS) {
      printf("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nDevice Initialization Success\r\n");
  }

  //! Register callback for Station connected and disconnected events
  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, stations_connect_notify_handler);
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, stations_disconnect_notify_handler);
  rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);

#ifdef RSI_WITH_OS
  rsi_task_handle_t driver_task_handle = NULL;

#if SOCKET_ASYNC_FEATURE
  rsi_semaphore_create(&ap_start_app, 0);
#endif

  //! Task created for Driver task
  rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                  (uint8_t *)"driver_task",
                  RSI_DRIVER_TASK_STACK_SIZE,
                  NULL,
                  RSI_DRIVER_TASK_PRIORITY,
                  &driver_task_handle);
#endif

  //! Wireless initialization
  status = rsi_wireless_init(6, 0);
  if (status != RSI_SUCCESS) {
      printf("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nWireless Initialization Success\r\n");
  }

  status = rsi_wlan_get (RSI_FW_VERSION, fw_version, sizeof(fw_version));
  if (status != RSI_SUCCESS)
    {
      printf ("\r\nFirmware Version Request Failed, Error Code : 0x%X\r\n", status);
      return status;
    }
  printf ("\r\nFirmware Version is: %s\r\n", fw_version);

  //! Send feature frame
  status = rsi_send_feature_frame ();
  if (status != RSI_SUCCESS)
    {
      printf ("\r\nFeature Frame Failed, Error Code :0x%x \r\n", status);
      return status;
    }
  printf ("\r\nFeature Frame Success\r\n");
  status = rsi_wlan_radio_init ();
  if (status != RSI_SUCCESS)
    {
      printf ("\r\nradio init Request Failed, Error Code : 0x%X\r\n", status);
      return status;
    }

  status = rsi_wlan_get (RSI_MAC_ADDRESS, mac_address, sizeof(mac_address));
  if (status != RSI_SUCCESS)
    {
      printf ("\r\nMac address Request Failed, Error Code : 0x%X\r\n", status);
      return status;
    }

  printf ("\r\nMac address is :%x:%x:%x:%x:%x:%x\r\n", mac_address[0],mac_address[1],mac_address[2],
          mac_address[3],mac_address[4],mac_address[5]);

  //! Enable Broadcast data filter
  status = rsi_wlan_filter_broadcast (5000, 1, 1);
  if (status != RSI_SUCCESS)
    {
      printf ("\r\nBroadcast Data Filtering Failed with Error Code : 0x%X\r\n",
              status);
      return status;
    }
  printf ("\r\nBroadcast Data Filtering Enabled\r\n");


  //LWIP



  rsi_wlan_register_callbacks (RSI_WLAN_DATA_RECEIVE_NOTIFY_CB,
                               wlan_data_receive_handler);



#if 0


  IP_ADDR4(&ap_ipaddr,ap_ip_addr0,ap_ip_addr1,ap_ip_addr2,ap_ip_addr3);
  IP_ADDR4(&ap_netmask,ap_netmask_addr0,ap_netmask_addr1,ap_netmask_addr2,ap_netmask_addr3);
  IP_ADDR4(&ap_gw,ap_gw_addr0,ap_gw_addr1,ap_gw_addr2,ap_gw_addr3);

  netif_set_default (&ap_netif);

  // netif_set_addr(&ap_netif,&ap_ipaddr,&ap_netmask,&ap_gw);

  netif_set_ipaddr(&ap_netif,&ap_ipaddr);
  netif_set_netmask(&ap_netif,&ap_netmask);
  netif_set_gw(&ap_netif,&ap_gw);
  netif_set_up (&ap_netif);
  netif_add (&ap_netif, &ap_ipaddr, &ap_netmask, &ap_gw, NULL,
             &ap_ethernetif_init, &ethernet_input);

  ap_netif.name[0] = 'S';

  ap_netif.name[1] = 'A';

  ap_netif.output = etharp_output;
  ap_netif.linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init (&ap_netif);
  dhcp_set_struct(&ap_netif,&v4dhcp);
  // dhcpserver_start();
  //  netif_set_link_up();
  // dhcp_start(ap_netif);
#endif










#if 0
  //! Configure IP
  status = rsi_config_ipaddress(RSI_IP_VERSION_4,
                                RSI_STATIC,
                                (uint8_t *)&ip_addr,
                                (uint8_t *)&network_mask,
                                (uint8_t *)&gateway,
                                NULL,
                                0,
                                0);
  if (status != RSI_SUCCESS) {
      printf("\r\nIP Config Failed, Error Code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nIP Config Success\r\n");
  }
#endif
  //! Start Access point
  status = rsi_wlan_ap_start((int8_t *)SSID,
                             CHANNEL_NO,
                             SECURITY_TYPE,
                             ENCRYPTION_TYPE,
                             (uint8_t *)PSK,
                             BEACON_INTERVAL,
                             DTIM_INTERVAL);
  if (status != RSI_SUCCESS) {
      printf("\r\nAP Start Failed, Error Code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nAP Start Success\r\n");
  }

  /////////////////////////////////////////

#if 1 //browser


  lwip_init ();
  IP_ADDR4(&ap_ipaddr,192,168,1,1);
  IP_ADDR4(&ap_netmask,255,255,255,0);
  IP_ADDR4(&ap_gw,192,168,1,1);


  netif_add (&ap_netif, &ap_ipaddr, &ap_netmask, &ap_gw, NULL,
             &ap_ethernetif_init, &ethernet_input);

  netif_set_default (&ap_netif);

  netif_set_up(&ap_netif);



  ap_netif.output = etharp_output;
  ap_netif.linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init (&ap_netif);


  printf ("Device IP address : %d.%d.%d.%d\r\n",
          ip_address[0]=  (uint8_t) (ap_netif.ip_addr.addr & 0xff),
          ip_address[1]=  (uint8_t) (ap_netif.ip_addr.addr >> 8),
          ip_address[2]=  (uint8_t) (ap_netif.ip_addr.addr >> 16),
          ip_address[3]=  (uint8_t) (ap_netif.ip_addr.addr >> 24));

#endif

  //  printf("\r\n DHCP configured IP \r\n");

  // print_ip_config(&ap_netif);

  // dhcp_start(&ap_netif);
  dhcpserver_start(); //Enable this for DHCP server. Working.




  while(1)
    {
      rsi_wireless_driver_task ();
      sys_check_timeouts ();
      //check_dhcp_assignment(&ap_netif);
    }




#if 0
  //! Create socket
#if SOCKET_ASYNC_FEATURE
  server_socket = rsi_socket_async(AF_INET, SOCK_STREAM, 0, socket_async_recive);
#else
  server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
#endif

  if (server_socket < 0) {
      status = rsi_wlan_get_status();
      printf("\r\nSocket Create Failed, Error Code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nSocket Create Success\r\n");
  }

  //! Set server structure
  memset(&server_addr, 0, sizeof(server_addr));

  //! Set family type
  server_addr.sin_family = AF_INET;

  //! Set local port number
  server_addr.sin_port = htons(DEVICE_PORT);

  //! Bind socket
  status = rsi_bind(server_socket, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
  if (status != RSI_SUCCESS) {
      status = rsi_wlan_get_status();
      rsi_shutdown(server_socket, 0);
      printf("\r\nBind Failed, Error code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nBind Success\r\n");
  }

  //! Socket listen
  status = rsi_listen(server_socket, 1);
  if (status != RSI_SUCCESS) {
      status = rsi_wlan_get_status();
      rsi_shutdown(server_socket, 0);
      printf("\r\nListen Failed, Error code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nListen Success\r\n");
  }

  addr_size = sizeof(server_socket);

  //! Socket accept
  new_socket = rsi_accept(server_socket, (struct rsi_sockaddr *)&client_addr, &addr_size);
  if (new_socket < 0) {
      status = rsi_wlan_get_status();
      rsi_shutdown(server_socket, 0);
      printf("\r\nSocket Accept Failed, Error code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nSocket Accept Success\r\n");
  }

#ifdef RSI_WITH_OS
#if SOCKET_ASYNC_FEATURE
  rsi_semaphore_wait(&ap_start_app, 0);
#endif
#endif

#if !SOCKET_ASYNC_FEATURE

  uint32_t recv_size = 0;
  memset(recv_buffer, 0, RECV_BUFFER_SIZE);

  while (packet_count < NUMBER_OF_PACKETS) {
      recv_size = RECV_BUFFER_SIZE;

      do {
          //! Receive data on socket
          status = rsi_recvfrom(new_socket, recv_buffer, recv_size, 0, (struct rsi_sockaddr *)&client_addr, &addr_size);
          if (status < 0) {
              status = rsi_wlan_socket_get_status(new_socket);
              if (status == RSI_RX_BUFFER_CHECK) {
                  continue;
              }

              rsi_shutdown(server_socket, 0);
              return status;
          }

          //! subtract received bytes
          recv_size -= status;

      } while (recv_size > 0);

      packet_count++;
  }

  //! Disconnect the station which is connected
  status = rsi_wlan_disconnect_stations(station_mac);
  if (status != RSI_SUCCESS) {
      printf("\r\nStation Disconnect Failed, Error code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nStation Disconnect Success\r\n");
  }

#endif

#if AP_TURN_OFF

  //! Stop Access point
  status = rsi_wlan_ap_stop();
  if (status != RSI_SUCCESS) {
      printf("\r\nAP Stop Failed, Error Code : 0x%lX\r\n", status);
      return status;
  } else {
      printf("\r\nAP Stop Success\r\n");
  }

#endif
#endif
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
  int32_t status = RSI_SUCCESS;

#ifdef RSI_WITH_OS
  rsi_task_handle_t wlan_task_handle = NULL;
  //! OS case
  //! Task created for WLAN task
  rsi_task_create((rsi_task_function_t)(int32_t)rsi_ap_start,
                  (uint8_t *)"wlan_task",
                  RSI_WLAN_TASK_STACK_SIZE,
                  NULL,
                  RSI_WLAN_TASK_PRIORITY,
                  &wlan_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call TCP server application in AP mode
  status = rsi_ap_start();

  //! Application main loop
  main_loop();
#endif
  return status;
}
