/***************************************************************************/ /**
 * @file
 * @brief TCP Client Example Application
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "errno.h"
#include "netdb.h"
#include "sl_wifi_callback_framework.h"
#include "sl_status.h"
#include "sl_board_configuration.h"
#include "cmsis_os2.h"
#include "sl_wifi.h"
#include "socket.h"
#include "sl_utility.h"
#include "sl_net.h"
#include "sl_net_si91x.h"
#include "sl_net_wifi_types.h"
#include <string.h>
#include <stdint.h>
#include "sl_si91x_socket.h"
#include "sl_mdns.h"

/******************************************************
 *                      Macros
 ******************************************************/
#define SERVER_IP   "192.168.1.8"
#define SERVER_PORT 5001

#define DEVICE_IP   "192.168.1.1"
#define DEVICE_PORT 5005

#define CLIENT_MESSAGE "WIFISDK"

#define MDNS_HOST_NAME       "wiseconnect.local."
#define MDNS_SERVICE_TYPE    "_http._tcp.local."
#define MDNS_SERVICE_NAME    "wiseconnect._http._tcp.local"
#define MDNS_SERVICE_MESSAGE "HTTP service_1234567890qwertyuioplkjhgfdsazxcvbnmmnbvcdfghjnbvcdfgh"
#define MDNS_SERVICE_PORT    80
#define MDNS_SERVICE_TTL     300
/******************************************************
 *               Variable Definitions
 ******************************************************/
const osThreadAttr_t thread_attributes = {
  .name       = "app",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityLow,
  .tz_module  = 0,
  .reserved   = 0,

};

static uint8_t is_data_received      = 0;
sl_ip_address_t ip_address           = { 0 };
sl_net_wifi_client_profile_t profile = { 0 };
uint32_t bytes_read                  = 0;

/******************************************************
 *               Function Declarations
 ******************************************************/
static void application_start(void *argument);
void receive_data_from_client();
/******************************************************
 *               Function Definitions
 ******************************************************/
void data_callback(uint32_t sock_no, const uint8_t *buffer, uint32_t length)
{
  UNUSED_PARAMETER(sock_no);
  UNUSED_PARAMETER(buffer);
  bytes_read += length;
  is_data_received = 1;
}

void app_init(const void *unused)
{
  UNUSED_PARAMETER(unused);
  osThreadNew((osThreadFunc_t)application_start, NULL, &thread_attributes);
}

static void application_start(void *argument)
{
  UNUSED_PARAMETER(argument);
  sl_status_t status             = SL_STATUS_FAIL;
  sl_mdns_t mdns                 = { 0 };
  sl_mdns_configuration_t config = { .protocol  = SL_MDNS_PROTO_UDP,
                                     .type      = SL_IPV4_VERSION,
                                     .host_name = MDNS_HOST_NAME };
  sl_mdns_service_t service      = { .instance_name   = MDNS_SERVICE_NAME,
                                     .service_type    = MDNS_SERVICE_TYPE,
                                     .service_message = MDNS_SERVICE_MESSAGE,
                                     .port            = MDNS_SERVICE_PORT,
                                     .ttl             = MDNS_SERVICE_TTL };

  status = sl_net_init(SL_NET_WIFI_CLIENT_INTERFACE, &sl_wifi_default_client_configuration, NULL, NULL);
  if (status != SL_STATUS_OK) {
    printf("\r\nFailed to start Wi-Fi Client interface: 0x%lx\r\n", status);
    return;
  }
  printf("\r\nWi-Fi Init Success \r\n");

  status = sl_net_up(SL_NET_WIFI_CLIENT_INTERFACE, SL_NET_DEFAULT_WIFI_CLIENT_PROFILE_ID);
  if (status != SL_STATUS_OK) {
    printf("\r\nFailed to bring Wi-Fi client interface up: 0x%lx\r\n", status);
    return;
  }
  printf("\r\nWi-Fi client connected\r\n");

  status = sl_net_get_profile(SL_NET_WIFI_CLIENT_INTERFACE, SL_NET_DEFAULT_WIFI_CLIENT_PROFILE_ID, &profile);
  if (status != SL_STATUS_OK) {
    printf("\r\nFailed to get client profile: 0x%lx\r\n", status);
    return;
  }
  printf("\r\nClient profile is fetched successfully.\r\n");

  ip_address.type = SL_IPV4;
  memcpy(&ip_address.ip.v4.bytes, &profile.ip.ip.v4.ip_address.bytes, sizeof(sl_ipv4_address_t));
  print_sl_ip_address(&ip_address);

  // Initialize MDNS service
  status = sl_mdns_init(&mdns, (const sl_mdns_configuration_t *)&config, NULL);
  if (status != SL_STATUS_OK) {
    printf("\r\nFailed to initialize MDNS : 0x%lx\r\n", status);
    return;
  }
  printf("\r\nMDNS initialized\r\n");

  // Add an interface to MDNS Instance
  status = sl_mdns_add_interface(&mdns, SL_NET_WIFI_CLIENT_INTERFACE);
  if (status != SL_STATUS_OK) {
    printf("\r\nFailed to add interface to MDNS : 0x%lx\r\n", status);
    return;
  }
  printf("\r\nInterface Added to MDNS\r\n");

  // Add a service to MDNS Instance
  status = sl_mdns_register_service(&mdns, SL_NET_WIFI_CLIENT_INTERFACE, &service);
  if (status != SL_STATUS_OK) {
    printf("\r\nFailed to register service to MDNS: 0x%lx\r\n", status);
    return;
  }
  printf("\r\nService Added to MDNS\r\n");

  receive_data_from_client();
}

void receive_data_from_client()
{
  int client_socket                 = -1;
  int return_value                  = 0;
  struct sockaddr_in client_address = { 0 };
  struct sockaddr_in server_address = { 0 };
  socklen_t socket_length           = sizeof(struct sockaddr_in);

  client_address.sin_family = AF_INET;
  client_address.sin_port   = DEVICE_PORT;

  server_address.sin_family = AF_INET;
  server_address.sin_port   = SERVER_PORT;

  sl_net_inet_addr(SERVER_IP, &server_address.sin_addr.s_addr);
  sl_net_inet_addr(DEVICE_IP, &client_address.sin_addr.s_addr);

  // Create socket
  client_socket = sl_si91x_socket_async(AF_INET, SOCK_DGRAM, 0, &data_callback);
  if (client_socket < 0) {
    printf("\r\nSocket creation failed with bsd error: %d\r\n", errno);
    return;
  }
  printf("\r\nUDP Socket Create Success\r\n");

  return_value = sl_si91x_bind(client_socket, (struct sockaddr *)&client_address, socket_length);
  if (return_value < 0) {
    printf("\r\nBind failed: %d\r\n", errno);
    return;
  }
  printf("\r\nBind success\r\n");

  while (!is_data_received) {
    osThreadYield();
  }

  return_value = sl_si91x_sendto(client_socket,
                                 (uint8_t *)CLIENT_MESSAGE,
                                 strlen(CLIENT_MESSAGE),
                                 0,
                                 (struct sockaddr *)&server_address,
                                 sizeof(server_address));

  if (return_value <= 0) {
    printf("\r\n Send failed \r\n");
    return;
  }

  printf("\r\nBytes read = %ld\r\n", bytes_read);
  // Socket close
  close(client_socket);
  printf("\r\nSocket close success\r\n");
  return;
}
