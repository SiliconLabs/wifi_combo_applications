/*******************************************************************************
* @file  rsi_concurrent_mode.c
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
#include "rsi_utils.h"
//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_driver.h"

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//! SSID of Access point to be created
#define AP_SSID "RSI_AP"

//! Security type of created Access point
#define AP_SECURITY_TYPE RSI_WPA2

//! Encryption type of created Access point
#define AP_ENCRYPTION_TYPE RSI_CCMP

//! Password of created Access point
#define AP_PSK "12345678"

//! Beacon Interval of created Access point
#define BEACON_INTERVAL 100

//! DTIM Count of created Access Point
#define DTIM_COUNT 10

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE 1

//! Access point SSID to connect
#define SSID "TPLINK_ALLEGION"

//! Password of AP to connect
#define STA_PSK "12345678"

//! Security type Access Point to connect
#define STA_SECURITY_TYPE RSI_WPA2


//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP "192.168.10.101" //0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY "192.168.10.1" //0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK "255.255.255.0" //0x00FFFFFF

#endif

//! Device port number
#define MOD_DEVICE_PORT 5001  //Client port number (In your case, when RS9116W in AP mode)

//IP address of the module(In your case, when RS9116W in AP mode)
#define MOD_DEVICE_IP "192.168.10.1"

//! IP address of Gateway
#define MOD_GATEWAY "192.168.10.1"

//! IP address of netmask
#define MOD_NETMASK "255.255.255.0"

//Server port number (In your case, when RS9116w in client mode)
#define SERVER_PORT 5002

//! Server IP address of the server (In your case, when RS9116w in station mode)
#define SERVER_IP_ADDRESS "192.168.0.104"

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

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
//! vap id to create socket

#define RSI_VAP_ID 0

#define RECV_BUFFER_SIZE 1000
uint8_t recv_buffer[RECV_BUFFER_SIZE];

uint8_t station_mac[6];

#if ENABLE_POWER_SAVE
//! Power Save Profile mode
#define PSP_MODE RSI_SLEEP_MODE_2

//! Power Save Profile type
#define PSP_TYPE RSI_MAX_PSP

int32_t rsi_wlan_power_save_profile(uint8_t psp_mode, uint8_t psp_type);
#endif

#include <string.h>

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];
uint64_t ip_to_reverse_hex(char *ip);

void stations_connect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  LOG_PRINT("\r\nStation Connected\r\n");
  UNUSED_PARAMETER(status);       //This statement is added only to resolve compilation warning, value is unchanged
  UNUSED_CONST_PARAMETER(length); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(station_mac, buffer, 6);
}

void stations_disconnect_notify_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
  LOG_PRINT("\r\nStation Disconnected\r\n");
  UNUSED_PARAMETER(status);       //This statement is added only to resolve compilation warning, value is unchanged
  UNUSED_PARAMETER(buffer);       //This statement is added only to resolve compilation warning, value is unchanged
  UNUSED_CONST_PARAMETER(length); //This statement is added only to resolve compilation warning, value is unchanged
}

int32_t rsi_concurrent_mode()
{

  int32_t server_socket, new_socket;
   struct rsi_sockaddr_in mod_server_addr, mod_client_addr;
  int32_t client_socket;
  int8_t vap_id = RSI_VAP_ID;
  struct rsi_sockaddr_in server_addr, client_addr;
  int32_t status       = RSI_SUCCESS;
  int32_t packet_count = 0;
  uint8_t ip_buff[20];
#if !(DHCP_MODE)
  uint32_t ip_addr      = ip_to_reverse_hex(DEVICE_IP);
  uint32_t network_mask = ip_to_reverse_hex(NETMASK);
  uint32_t gateway      = ip_to_reverse_hex(GATEWAY);
#else
  uint8_t dhcp_mode = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
#ifdef RSI_WITH_OS
  rsi_task_handle_t driver_task_handle = NULL;
#endif

  //! Driver initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > GLOBAL_BUFF_LEN)) {
    return status;
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
  //! SiLabs module intialisation
  status = rsi_device_init(LOAD_NWP_FW);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nDevice Initialization Success\r\n");
  }

  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, stations_connect_notify_handler);
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, stations_disconnect_notify_handler);
  //! WC initialization
  status = rsi_wireless_init(9, 0);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nWireless Initialization Success\r\n");
  }
  uint32_t mod_ip_addr      = ip_to_reverse_hex(MOD_DEVICE_IP);
  uint32_t mod_network_mask = ip_to_reverse_hex(MOD_NETMASK);
  uint32_t mod_gateway      = ip_to_reverse_hex(MOD_GATEWAY);
  int32_t addr_size;

  status = rsi_config_ipaddress(RSI_IP_VERSION_4,
                                            RSI_STATIC,
                                            (uint8_t *)&mod_ip_addr,
                                            (uint8_t *)&mod_network_mask,
                                            (uint8_t *)&mod_gateway,
                                            NULL,
                                            0,
                                            1);
              if (status != RSI_SUCCESS) {
                LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%lX\r\n", status);
                return status;
              } else {
                LOG_PRINT("\r\nIP Config Success\r\n");
              }

  //! Start Access point
  status = rsi_wlan_ap_start((int8_t *)AP_SSID,
                             1,
                             AP_SECURITY_TYPE,
                             AP_ENCRYPTION_TYPE,
                             (uint8_t *)AP_PSK,
                             BEACON_INTERVAL,
                             DTIM_COUNT);

  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nAP Start Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nAP Start Success\r\n");
  }
//rsi_delay_ms(60000);

#if ENABLE_POWER_SAVE
  //! Apply power save profile
  status = rsi_wlan_power_save_profile(PSP_MODE, PSP_TYPE);
  if (status != RSI_SUCCESS) {
    return status;
  }
#endif

  server_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket < 0) {
      status = rsi_wlan_get_status();
      LOG_PRINT("\r\nSocket Create Failed, Error Code : 0x%lX\r\n", status);
      return status;
    } else {
      LOG_PRINT("\r\nSocket Create Success\r\n");
    }

    //! Set server structure
    memset(&mod_server_addr, 0, sizeof(mod_server_addr));

    //! Set family type
    mod_server_addr.sin_family = AF_INET;

    //! Set local port number
    mod_server_addr.sin_port = htons(MOD_DEVICE_PORT);

    //! Bind socket
    status = rsi_bind(server_socket, (struct rsi_sockaddr *)&mod_server_addr, sizeof(mod_server_addr));
    if (status != RSI_SUCCESS) {
      status = rsi_wlan_get_status();
      rsi_shutdown(server_socket, 0);
      LOG_PRINT("\r\nBind Failed, Error code : 0x%lX\r\n", status);
      return status;
    } else {
      LOG_PRINT("\r\nBind Success\r\n");
    }
    vap_id = 1;
    status = rsi_setsockopt(server_socket, SOL_SOCKET, SO_SOCK_VAP_ID, &vap_id, sizeof(vap_id));
    if (status != RSI_SUCCESS) {
      return status;
    }

    //! Socket listen
    status = rsi_listen(server_socket, 1);
    if (status != RSI_SUCCESS) {
      status = rsi_wlan_get_status();
      rsi_shutdown(server_socket, 0);
      LOG_PRINT("\r\nListen Failed, Error code : 0x%lX\r\n", status);
      return status;
    } else {
      LOG_PRINT("\r\nListen Success\r\n");
    }

    addr_size = sizeof(server_socket);

    //! Socket accept
    new_socket = rsi_accept(server_socket, (struct rsi_sockaddr *)&mod_client_addr, &addr_size);
    if (new_socket < 0) {
      status = rsi_wlan_get_status();
      rsi_shutdown(server_socket, 0);
      LOG_PRINT("\r\nSocket Accept Failed, Error code : 0x%lX\r\n", status);
      return status;
    } else {
      LOG_PRINT("\r\nSocket Accept Success\r\n");
    }
    uint32_t recv_size = 0;
     memset(recv_buffer, 0, RECV_BUFFER_SIZE);
     packet_count=0;

     while (packet_count < NUMBER_OF_PACKETS) {
       recv_size = RECV_BUFFER_SIZE;

       do {
         //! Receive data on socket
         status = rsi_recvfrom(new_socket, recv_buffer, recv_size, 0, (struct rsi_sockaddr *)&mod_client_addr, &addr_size);
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
      // LOG_PRINT("\r\npacket_count : %d\r\n", packet_count);

       packet_count++;
     }

  status = rsi_wlan_ap_stop();
    if (status != RSI_SUCCESS) {
      LOG_PRINT("\r\nAP Stop Failed, Error Code : 0x%lX\r\n", status);
      return status;
    } else {
      LOG_PRINT("\r\nAP Stop Success\r\n");
    }
    vap_id = 0;

    status = rsi_send_feature_frame();
      if (status != RSI_SUCCESS) {
        return status;
      }

  rsi_rsp_scan_t result;
    int32_t length = sizeof(result);
    memset(&result, 0, length);
    status = rsi_wlan_scan((int8_t *)SSID, 1, &result, length);

    if (status != RSI_SUCCESS) {
      LOG_PRINT("\r\nWLAN Scan Failed, Error Code : 0x%lX\r\n", status);
      return status;
    }
    uint8_t channel_no = result.scan_info->rf_channel;
    LOG_PRINT("\r\nScan successful channel: 0x%lX\r\n", channel_no);

    //! Connect to an Access point
    status = rsi_wlan_connect((int8_t *)SSID, STA_SECURITY_TYPE, STA_PSK);
    if (status != RSI_SUCCESS) {
      LOG_PRINT("\r\nConnect to Access point Failed, Error Code : 0x%lX\r\n", status);
      return status;
    } else {
      LOG_PRINT("\r\nConnect to Access point Success\r\n");
    }

      #if DHCP_MODE
        status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, ip_buff, sizeof(ip_buff), 0);
      #else
        status            = rsi_config_ipaddress(RSI_IP_VERSION_4,
                                      RSI_STATIC,
                                      (uint8_t *)&ip_addr,
                                      (uint8_t *)&network_mask,
                                      (uint8_t *)&gateway,
                                      NULL,
                                      0,
                                      0);
      #endif

        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%lX\r\n", status);
          return status;
        }
        LOG_PRINT("\r\nIP Config Success\r\n");
        LOG_PRINT("RSI_STA IP ADDR: %d.%d.%d.%d \r\n", ip_buff[6], ip_buff[7], ip_buff[8], ip_buff[9]);

        client_socket = rsi_socket(AF_INET, SOCK_STREAM, 0);
          if (client_socket < 0) {
            LOG_PRINT("\r\nSocket Create Failed\r\n");
            status = rsi_wlan_get_status();
            return status;
          } else {
            LOG_PRINT("\r\nSocket Create Success\r\n");
          }

          //! Memset client structrue
          memset(&client_addr, 0, sizeof(client_addr));

          //! Set family type
          client_addr.sin_family = AF_INET;

          //! Set local port number
          client_addr.sin_port = htons(MOD_DEVICE_PORT);

          //! Bind socket
          status = rsi_bind(client_socket, (struct rsi_sockaddr *)&client_addr, sizeof(client_addr));
          if (status != RSI_SUCCESS) {
            LOG_PRINT("\r\nBind Failed, Error Code : 0x%lX\r\n", status);
            status = rsi_wlan_get_status();
            rsi_shutdown(client_socket, 0);
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

          //! Set IP address to localhost
          server_addr.sin_addr.s_addr = ip_to_reverse_hex(SERVER_IP_ADDRESS);

          status = rsi_setsockopt(client_socket, SOL_SOCKET, SO_SOCK_VAP_ID, &vap_id, sizeof(vap_id));
          if (status != RSI_SUCCESS) {
            return status;
          }

          //! Connect to server socket
          status = rsi_connect(client_socket, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
          if (status != RSI_SUCCESS) {
            LOG_PRINT("\r\nConnect to Server Socket Failed, Error Code : 0x%lX\r\n", status);
            status = rsi_wlan_get_status();
            rsi_shutdown(client_socket, 0);
            return status;
          } else {
            LOG_PRINT("\r\nConnect to Server Socket Success \r\n");
          }
          packet_count=0;
          while (packet_count < NUMBER_OF_PACKETS) {
            //! Send data on socket
            status = rsi_send(client_socket, (int8_t *)"Hello from TCP client!!!", (sizeof("Hello from TCP client!!!") - 1), 0);
            if (status < 0) {
              status = rsi_wlan_get_status();
              rsi_shutdown(client_socket, 0);
              return status;
            }

            packet_count++;
          }

          LOG_PRINT("\r\nData sent successfully to Server Socket\r\n");
          rsi_shutdown(client_socket, 0);



    status = rsi_wlan_disconnect();
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\nDisconnect to Access point Failed, Error Code : 0x%lX\r\n", status);
          return status;
        } else {
          LOG_PRINT("\r\nDisconnect to Access point Success\r\n");
        }

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
#endif

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create((rsi_task_function_t)(int32_t)rsi_concurrent_mode,
                  (uint8_t *)"wlan_task",
                  RSI_WLAN_TASK_STACK_SIZE,
                  NULL,
                  RSI_WLAN_TASK_PRIORITY,
                  &wlan_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call concurrent mode  application
  status = rsi_concurrent_mode();

  //! Application main loop
  main_loop();
#endif
  return status;
}
