/***************************************************************************//**
 * @file
 * @brief Query command Application
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licenser of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be mis-represented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    mis-represented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 ******************************************************************************/

#include "cmsis_os2.h"
#include "sl_wifi.h"
#include "sl_net.h"
#include "sl_utility.h"
#include "sl_si91x_driver.h"
#include "sl_si91x_core_utilities.h"
#ifdef SLI_SI91X_MCU_INTERFACE
#include "sl_net_default_values.h"
#include "sl_sntp.h"
#include "sl_net_dns.h"
#include "sl_wifi_callback_framework.h"
#endif

/******************************************************
 *                      Macros
 ******************************************************/
#define AP_MODE 1
#define STA_MODE 1
///*=======================================================================*/
//// NWP buffer allocation parameters
///*=======================================================================*/
//
#ifndef TX_POOL_RATIO
#define TX_POOL_RATIO 1
#endif

#ifndef RX_POOL_RATIO
#define RX_POOL_RATIO 1
#endif

#ifndef GLOBAL_POOL_RATIO
#define GLOBAL_POOL_RATIO 1
#endif
//
/*****************************************************
 *                      Socket configuration
 *****************************************************/

#define SNTP_METHOD         SL_SNTP_UNICAST_MODE
#define FLAGS               0
#define NTP_SERVER_IP       "0.pool.ntp.org" // Mostly "162.159.200.123"
#define DATA_BUFFER_LENGTH  50
#define SNTP_TIMEOUT        50
#define SNTP_API_TIMEOUT    0
#define ASYNC_WAIT_TIMEOUT  60000
#define DNS_TIMEOUT         20000
#define MAX_DNS_RETRY_COUNT 5
#define DNS_SERVER1_IP      "8.8.8.8"
#define DNS_SERVER2_IP      "8.8.4.4"
#define FW_LOC ((uint8_t *)0x4011000)

/******************************************************
 *                    Constants
 ******************************************************/
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
sl_ip_address_t ip_address           = { 0 };
sl_net_wifi_client_profile_t profile = { 0 };

static const sl_wifi_device_configuration_t station_configuration = {
    .boot_option = LOAD_NWP_FW,
    .mac_address = NULL,
    .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
    .region_code = US,
    .boot_config = { .oper_mode = SL_SI91X_CLIENT_MODE,
        .coex_mode = SL_SI91X_WLAN_ONLY_MODE,
        .feature_bit_map =
            (SL_SI91X_FEAT_SECURITY_OPEN | SL_SI91X_FEAT_AGGREGATION | SL_SI91X_FEAT_WPS_DISABLE),
            .tcp_ip_feature_bit_map = (SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID | SL_SI91X_TCP_IP_FEAT_SNTP_CLIENT |  SL_SI91X_TCP_IP_FEAT_DNS_CLIENT
#if (THROUGHPUT_TYPE == TLS_TX || THROUGHPUT_TYPE == TLS_RX)
                | SL_SI91X_TCP_IP_FEAT_SSL
#endif
            ),
            .custom_feature_bit_map =
                (SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID | SL_SI91X_CUSTOM_FEAT_SOC_CLK_CONFIG_160MHZ|SL_SI91X_CUSTOM_FEAT_RTC_FROM_HOST),
                .ext_custom_feature_bit_map = (SL_SI91X_EXT_FEAT_XTAL_CLK | MEMORY_CONFIG
#if defined(SLI_SI917) || defined(SLI_SI915)
                    | SL_SI91X_EXT_FEAT_FRONT_END_SWITCH_PINS_ULP_GPIO_4_5_0
#endif
                ),
                .bt_feature_bit_map         = 0,
                .ext_tcp_ip_feature_bit_map = (SL_SI91X_EXT_TCP_IP_WINDOW_DIV | SL_SI91X_CONFIG_FEAT_EXTENTION_VALID
                    | SL_SI91X_EXT_TCP_IP_WAIT_FOR_SOCKET_CLOSE
#if (THROUGHPUT_TYPE == TLS_TX || THROUGHPUT_TYPE == TLS_RX)
                    | SL_SI91X_EXT_TCP_IP_FEAT_SSL_THREE_SOCKETS
#endif
                ),
                .ble_feature_bit_map     = 0,
                .ble_ext_feature_bit_map = 0,
                .config_feature_bit_map  = SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP },
                .ta_pool = { .tx_ratio_in_buffer_pool     = TX_POOL_RATIO,
                    .rx_ratio_in_buffer_pool     = RX_POOL_RATIO,
                    .global_ratio_in_buffer_pool = GLOBAL_POOL_RATIO }
};

sl_wifi_interface_t interface_1 = SL_WIFI_AP_2_4GHZ_INTERFACE;
/******************************************************
 *               Function Declarations
 ******************************************************/
static void application_start(void *argument);
void print_wireless_info(const sl_si91x_rsp_wireless_info_t *info);
void print_advanced_scan_configuration(const sl_wifi_advanced_scan_configuration_t *configuration);
void print_ap_configuration(const sl_wifi_ap_configuration_t  *configuration);
void print_performance_profile(const sl_wifi_performance_profile_t *profile);
void print_statistics(const sl_wifi_statistics_t *statistics);
void print_operational_statistics(const sl_wifi_operational_statistics_t *operational_statistics);
const char* get_wifi_tdi_string(int value);
const char* get_wifi_rate_protocol_string(int value);
const char* get_wifi_security_string(int value);
const char* get_wifi_encryption_string(int value);
const char* get_ap_keepalive_string(int value);
const char* get_wifi_band_string(int value);
const char* get_wifi_bandwidth_string(int value);
const char* get_wifi_credential_string(int value);
const char* get_performance_profile_string(int value);
const char* get_wifi_interface_string(sl_wifi_interface_t interface);


/******************************************************
 *               Function Definitions
 ******************************************************/
const char* get_wifi_interface_string(sl_wifi_interface_t interface) {
  switch (interface) {
    case SL_WIFI_INVALID_INTERFACE:
      return "SL_WIFI_INVALID_INTERFACE";
    case SL_WIFI_CLIENT_INTERFACE:
      return "SL_WIFI_CLIENT_INTERFACE";
    case SL_WIFI_AP_INTERFACE:
      return "SL_WIFI_AP_INTERFACE";
    case SL_WIFI_2_4GHZ_INTERFACE:
      return "SL_WIFI_2_4GHZ_INTERFACE";
    case SL_WIFI_5GHZ_INTERFACE:
      return "SL_WIFI_5GHZ_INTERFACE";
    case SL_WIFI_TRANSCEIVER_INTERFACE:
      return "SL_WIFI_TRANSCEIVER_INTERFACE";
    case SL_WIFI_CLIENT_2_4GHZ_INTERFACE:
      return "SL_WIFI_CLIENT_2_4GHZ_INTERFACE";
    case SL_WIFI_AP_2_4GHZ_INTERFACE:
      return "SL_WIFI_AP_2_4GHZ_INTERFACE";
    case SL_WIFI_CLIENT_5GHZ_INTERFACE:
      return "SL_WIFI_CLIENT_5GHZ_INTERFACE";
    case SL_WIFI_AP_5GHZ_INTERFACE:
      return "SL_WIFI_AP_5GHZ_INTERFACE";
    case SL_WIFI_ALL_INTERFACES:
      return "SL_WIFI_ALL_INTERFACES";
    default:
      return "UNKNOWN_INTERFACE";
  }
}
// Function to print the wireless information
void print_wireless_info(const sl_si91x_rsp_wireless_info_t *info) {
  printf("\n********Wireless Info:*********\n");
  printf("WLAN State:             %u\n", info->wlan_state);
  printf("Channel Number:         %u\n", info->channel_number);
  printf("SSID:                   %s\n", info->ssid);
  print_mac_address((const sl_mac_address_t *)info->mac_address);
  printf("Security Type:          %u(%s)\n", info->sec_type, get_wifi_security_string(info->sec_type));
  printf("PSK/PMK:                ");
  for(int i = 0; i < 64; i++)
    {
      printf("%X", info->psk_pmk[i]);
    }
  printf("\r\n");
  print_sl_ipv4_address((const sl_ipv4_address_t *)info->ipv4_address);
  print_sl_ipv6_address((const sl_ipv6_address_t *)info->ipv6_address);
}

void print_advanced_scan_configuration(const sl_wifi_advanced_scan_configuration_t *configuration) {
  printf("\n********Advanced scan configurations of the device:*********\n");
  printf("trigger_level:              %ld dBm\n", configuration->trigger_level);
  printf("trigger_level_change:       %lu dBm\n", configuration->trigger_level_change);
  printf("active_channel_time:        %u ms\n", configuration->active_channel_time);
  printf("passive_channel_time:       %u ms\n",configuration->passive_channel_time);
  printf("enable_instant_scan:        %u\n", configuration->enable_instant_scan);
  printf("enable_multi_probe:         %u\n", configuration->enable_multi_probe);
}
const char* get_wifi_rate_protocol_string(int value) {
  switch (value) {
    case SL_WIFI_RATE_PROTOCOL_B_ONLY:
      return "SL_WIFI_RATE_PROTOCOL_B_ONLY";
    case SL_WIFI_RATE_PROTOCOL_G_ONLY:
      return "SL_WIFI_RATE_PROTOCOL_G_ONLY";
    case SL_WIFI_RATE_PROTOCOL_N_ONLY:
      return "SL_WIFI_RATE_PROTOCOL_N_ONLY";
    case SL_WIFI_RATE_PROTOCOL_AC_ONLY:
      return "SL_WIFI_RATE_PROTOCOL_AC_ONLY";
    case SL_WIFI_RATE_PROTOCOL_AX_ONLY:
      return "SL_WIFI_RATE_PROTOCOL_AX_ONLY";
    case SL_WIFI_RATE_PROTOCOL_AUTO:
      return "SL_WIFI_RATE_PROTOCOL_AUTO";
    default:
      return "Unknown Protocol";
  }
}

const char* get_wifi_security_string(int value) {
  switch (value) {
    case SL_WIFI_OPEN:                       return "SL_WIFI_OPEN";
    case SL_WIFI_WPA:                        return "SL_WIFI_WPA";
    case SL_WIFI_WPA2:                       return "SL_WIFI_WPA2";
    case SL_WIFI_WEP:                        return "SL_WIFI_WEP";
    case SL_WIFI_WPA_ENTERPRISE:             return "SL_WIFI_WPA_ENTERPRISE";
    case SL_WIFI_WPA2_ENTERPRISE:            return "SL_WIFI_WPA2_ENTERPRISE";
    case SL_WIFI_WPA_WPA2_MIXED:             return "SL_WIFI_WPA_WPA2_MIXED";
    case SL_WIFI_WPA3:                       return "SL_WIFI_WPA3";
    case SL_WIFI_WPA3_TRANSITION:            return "SL_WIFI_WPA3_TRANSITION";
    case SL_WIFI_WPA3_ENTERPRISE:            return "SL_WIFI_WPA3_ENTERPRISE";
    case SL_WIFI_WPA3_TRANSITION_ENTERPRISE: return "SL_WIFI_WPA3_TRANSITION_ENTERPRISE";
    case SL_WIFI_SECURITY_UNKNOWN:           return "SL_WIFI_SECURITY_UNKNOWN";
    default:                                 return "Unknown Security Type";
  }
}

const char* get_wifi_encryption_string(int value) {
  switch (value) {
    case SL_WIFI_DEFAULT_ENCRYPTION:       return "SL_WIFI_DEFAULT_ENCRYPTION";
    case SL_WIFI_NO_ENCRYPTION:            return "SL_WIFI_NO_ENCRYPTION";
    case SL_WIFI_WEP_ENCRYPTION:           return "SL_WIFI_WEP_ENCRYPTION";
    case SL_WIFI_TKIP_ENCRYPTION:          return "SL_WIFI_TKIP_ENCRYPTION";
    case SL_WIFI_CCMP_ENCRYPTION:          return "SL_WIFI_CCMP_ENCRYPTION";
    case SL_WIFI_EAP_TLS_ENCRYPTION:       return "SL_WIFI_EAP_TLS_ENCRYPTION";
    case SL_WIFI_EAP_TTLS_ENCRYPTION:      return "SL_WIFI_EAP_TTLS_ENCRYPTION";
    case SL_WIFI_EAP_FAST_ENCRYPTION:      return "SL_WIFI_EAP_FAST_ENCRYPTION";
    case SL_WIFI_PEAP_MSCHAPV2_ENCRYPTION: return "SL_WIFI_PEAP_MSCHAPV2_ENCRYPTION";
    case SL_WIFI_EAP_LEAP_ENCRYPTION:      return "SL_WIFI_EAP_LEAP_ENCRYPTION";
    default:                               return "Unknown Encryption Type";
  }
}
const char* get_ap_keepalive_string(int value) {
  switch (value) {
    case SL_SI91X_AP_KEEP_ALIVE_DISABLE:          return "SL_SI91X_AP_KEEP_ALIVE_DISABLE";
    case SL_SI91X_AP_DEAUTH_BASED_KEEP_ALIVE:    return "SL_SI91X_AP_DEAUTH_BASED_KEEP_ALIVE";
    case SL_SI91X_AP_NULL_BASED_KEEP_ALIVE:      return "SL_SI91X_AP_NULL_BASED_KEEP_ALIVE";
    default:                                     return "Unknown Keepalive Type";
  }
}

const char* get_wifi_band_string(int value) {
  switch (value) {
    case SL_WIFI_AUTO_BAND:   return "SL_WIFI_AUTO_BAND";
    case SL_WIFI_BAND_900MHZ: return "SL_WIFI_BAND_900MHZ";
    case SL_WIFI_BAND_2_4GHZ: return "SL_WIFI_BAND_2_4GHZ";
    case SL_WIFI_BAND_5GHZ:   return "SL_WIFI_BAND_5GHZ";
    case SL_WIFI_BAND_6GHZ:   return "SL_WIFI_BAND_6GHZ";
    case SL_WIFI_BAND_60GHZ:  return "SL_WIFI_BAND_60GHZ";
    default:                  return "Unknown Wi-Fi Band";
  }
}

const char* get_wifi_bandwidth_string(int value) {
  switch (value) {
    case SL_WIFI_AUTO_BANDWIDTH:   return "SL_WIFI_AUTO_BANDWIDTH";
    case SL_WIFI_BANDWIDTH_20MHz:  return "SL_WIFI_BANDWIDTH_20MHz";
    default:                       return "Unknown Wi-Fi Bandwidth";
  }
}

const char* get_wifi_credential_string(int value) {
  switch (value) {
    case SL_WIFI_PSK_CREDENTIAL:   return "SL_WIFI_PSK_CREDENTIAL";
    case SL_WIFI_PMK_CREDENTIAL:   return "SL_WIFI_PMK_CREDENTIAL";
    case SL_WIFI_WEP_CREDENTIAL:   return "SL_WIFI_WEP_CREDENTIAL";
    case SL_WIFI_EAP_CREDENTIAL:   return "SL_WIFI_EAP_CREDENTIAL";
    case SL_WIFI_USER_CREDENTIAL:  return "SL_WIFI_USER_CREDENTIAL";
    default:                       return "Unknown Wi-Fi Credential Type";
  }
}

const char* get_wifi_tdi_string(int value) {
  switch (value) {
    case SL_WIFI_TDI_NONE: return "SL_WIFI_TDI_NONE";
    case SL_WIFI_TDI_SAE:  return "SL_WIFI_TDI_SAE";
    default:               return "Unknown Wi-Fi TDI Type";
  }
}
void print_ap_configuration(const sl_wifi_ap_configuration_t  *configuration) {
  printf("\n********ap configuration:*********\n");
  printf("ssid: ");
  for(int i = 0; i < 32; i++)
    {
      if(configuration->ssid.value[i] != '\0')
        printf("%c", configuration->ssid.value[i]);
    }
  printf("\r\n");
  printf("security:                 %u(%s)\n", configuration->security, get_wifi_security_string(configuration->security));
  printf("encryption:               %u(%s)\n",configuration->encryption, get_wifi_encryption_string(configuration->encryption));
  printf("channel:                  %d \nband =                    %d(%s)\nbandwidth =               %d(%s)\n", configuration->channel.channel, configuration->channel.band,get_wifi_band_string(configuration->channel.band), configuration->channel.bandwidth,get_wifi_bandwidth_string(configuration->channel.bandwidth));
  printf("rate protocol:            %d(%s)\n", configuration->rate_protocol, get_wifi_rate_protocol_string(configuration->rate_protocol));
  printf("options for Hidden ssid:  %d\n", configuration->options);
  printf("credential_id:            %lu(%s)\n", configuration->credential_id, get_wifi_credential_string(configuration->credential_id));
  printf("keepalive_type:           %u(%s)\n", configuration->keepalive_type,get_ap_keepalive_string(configuration->keepalive_type));
  printf("beacon_interval:          %ums\n", configuration->beacon_interval);
  printf("client_idle_timeout:      %lums\n",configuration->client_idle_timeout);
  printf("dtim_beacon_count:        %u\n",configuration->dtim_beacon_count);
  printf("maximum_clients:          %u\n", configuration->maximum_clients);
  printf("Flag to stop beaconing \nwhen there are no associated clients:   %u\n", configuration->beacon_stop);
  printf("tdi_flags:                %d(%s)\n", configuration->tdi_flags, get_wifi_tdi_string(configuration->tdi_flags));
  printf("is_11n_enabled:           %u\n", configuration->is_11n_enabled);
}

const char* get_performance_profile_string(int value) {
  switch (value) {
    case HIGH_PERFORMANCE:                     return "HIGH_PERFORMANCE";
    case ASSOCIATED_POWER_SAVE:                return "ASSOCIATED_POWER_SAVE";
    case ASSOCIATED_POWER_SAVE_LOW_LATENCY:    return "ASSOCIATED_POWER_SAVE_LOW_LATENCY";
    case DEEP_SLEEP_WITHOUT_RAM_RETENTION:     return "DEEP_SLEEP_WITHOUT_RAM_RETENTION";
    case DEEP_SLEEP_WITH_RAM_RETENTION:        return "DEEP_SLEEP_WITH_RAM_RETENTION";
    default:                                   return "Unknown Performance Profile";
  }
}
void print_performance_profile(const sl_wifi_performance_profile_t *profile){
  printf("\r\n********Power and Performance profile of the Device:*********\r\n");
  printf("Profile:   %d(%s)\n",profile->profile, get_performance_profile_string(profile->profile));
}

void print_statistics(const sl_wifi_statistics_t *statistics){
  printf("\r\n********Wi-Fi statistics of the Device:*********\r\n");
  printf("beacon_lost_count:    %lu\n",statistics->beacon_lost_count);
  printf("beacon_rx_count:      %lu\n",statistics->beacon_rx_count);
  printf("mcast_rx_count:       %lu\n", statistics->mcast_rx_count);
  printf("mcast_tx_count:       %lu\n", statistics->mcast_tx_count);
  printf("ucast_rx_count:       %lu\n", statistics->ucast_rx_count);
  printf("ucast_tx_count:       %lu\n", statistics->ucast_tx_count);
  printf("overrun_count:        %lu\n", statistics->overrun_count);

}

void print_operational_statistics(const sl_wifi_operational_statistics_t *operational_statistics){
  sl_wifi_interface_t interface;
  interface = interface_1;
  printf("\r\n********operational statistics of the Device:*********\r\n");
  printf("operating_mode:        %u(%s)\n",operational_statistics->operating_mode, get_wifi_interface_string(interface));
  printf("dtim_period:           %u\n",operational_statistics->dtim_period);
  printf("ideal_beacon_info:     %u\n", *operational_statistics->ideal_beacon_info);
  printf("busy_beacon_info:      %u\n", *operational_statistics->busy_beacon_info);
  printf("beacon_interval:       %uTus\n",*operational_statistics->beacon_interval);
}

static sl_status_t module_status_handler(sl_wifi_event_t event, void *data, uint32_t data_length, void *arg)
{
  UNUSED_PARAMETER(event);
  UNUSED_PARAMETER(arg);
  sl_si91x_module_state_stats_response_t *notif = (sl_si91x_module_state_stats_response_t *)data;
  SL_DEBUG_LOG("---> Module status handler event with length : %lu\r\n", data_length);
  SL_DEBUG_LOG("  <> Time stamp : %lu, state_code : 0x%02X, reason_code : 0x%02X, channel : %u, rssi : %u.\n",
               notif->timestamp,
               notif->state_code,
               notif->reason_code,
               notif->channel,
               notif->rssi);
  SL_DEBUG_LOG("  <> BSSID : %x:%x:%x:%x:%x:%x.\n",
               notif->bssid[0],
               notif->bssid[1],
               notif->bssid[2],
               notif->bssid[3],
               notif->bssid[4],
               notif->bssid[5]);
  return SL_STATUS_OK;
}

static void print_char_buffer(char *buffer, uint32_t buffer_length)
{
  uint32_t i = 0;

  for (i = 0; i < buffer_length; i++) {
      printf("%c", buffer[i]);
  }

  printf("\r\n");
  return;
}
uint64_t ip_to_reverse_hex(char *ip)
{
  uint32_t ip1, ip2, ip3, ip4;
  uint64_t ip_hex;
  uint32_t status;

  status = sscanf(ip, "%lu.%lu.%lu.%lu", &ip1, &ip2, &ip3, &ip4);
  if (status != 4) {
      return 0x00000000; // Problem if we actually pass 0.0.0.0
  }
  ip_hex = (uint64_t)ip1;
  ip_hex |= (uint64_t)(ip2 << 8);
  ip_hex |= (uint64_t)(ip3 << 16);
  ip_hex |= (uint64_t)(ip4 << 24);
  return ip_hex;
}

static uint8_t callback_event = 0;
static sl_status_t cb_status  = SL_STATUS_FAIL;
static uint8_t exec_status    = 0;
static char *event_type[]     = { [SL_SNTP_CLIENT_START]           = "SNTP Client Start",
    [SL_SNTP_CLIENT_GET_TIME]        = "SNTP Client Get Time",
    [SL_SNTP_CLIENT_GET_TIME_DATE]   = "SNTP Client Get Time and Date",
    [SL_SNTP_CLIENT_GET_SERVER_INFO] = "SNTP Client Get Server Info",
    [SL_SNTP_CLIENT_STOP]            = "SNTP Client Stop" };

static void sntp_client_event_handler(sl_sntp_client_response_t *response,
                                      uint8_t *user_data,
                                      uint16_t user_data_length)
{
  uint16_t length = 0;
  printf("\nReceived %s SNTP event with status %s\n",
         event_type[response->event_type],
         (0 == response->status) ? "Success" : "Failed");
  if (0 == response->status) {
      if (response->data_length > user_data_length) {
          length = user_data_length;
      } else {
          length = response->data_length;
      }
      memcpy(user_data, response->data, length);
  }
  callback_event = response->event_type;
  cb_status      = response->status;

  if (SL_SNTP_CLIENT_STOP == response->event_type) {
      exec_status = 1;
  }
  return;
}
sl_status_t embedded_sntp_client(void)
{
  uint32_t start = 0;
  sl_status_t status;
  sl_ip_address_t address          = { 0 };
  sl_sntp_client_config_t config   = { 0 };
  uint8_t data[DATA_BUFFER_LENGTH] = { 0 };
  sl_sntp_server_info_t serverInfo = { 0 };
  int32_t dns_retry_count          = MAX_DNS_RETRY_COUNT;

  // Convert DNS server IP addresses to sl_ip_address_t structures
  sl_ip_address_t primary_dns_server;
  sl_ip_address_t secondary_dns_server;

  primary_dns_server.type        = SL_IPV4;
  primary_dns_server.ip.v4.value = ip_to_reverse_hex(DNS_SERVER1_IP);

  secondary_dns_server.type        = SL_IPV4;
  secondary_dns_server.ip.v4.value = ip_to_reverse_hex(DNS_SERVER2_IP);

  // Create sl_net_set_dns_address_t structure
  sl_net_dns_address_t dns_address;
  dns_address.primary_server_address   = &primary_dns_server;
  dns_address.secondary_server_address = &secondary_dns_server;

  // Set DNS server addresses
  status = sl_net_set_dns_server(SL_NET_WIFI_CLIENT_INTERFACE, &dns_address);
  do {
      status = sl_net_dns_resolve_hostname(NTP_SERVER_IP, DNS_TIMEOUT, SL_NET_DNS_TYPE_IPV4, &address);
      dns_retry_count--;
  } while ((dns_retry_count != 0) && (status != SL_STATUS_OK));

  printf("Ip Address : %u.%u.%u.%u\n",
         address.ip.v4.bytes[0],
         address.ip.v4.bytes[1],
         address.ip.v4.bytes[2],
         address.ip.v4.bytes[3]);

  config.server_host_name = address.ip.v4.bytes;
  config.sntp_method      = SNTP_METHOD;
  config.sntp_timeout     = SNTP_TIMEOUT;
  config.event_handler    = sntp_client_event_handler;
  config.flags            = FLAGS;
  cb_status = SL_STATUS_FAIL;
  status    = sl_sntp_client_start(&config, SNTP_API_TIMEOUT);
  if ((SNTP_API_TIMEOUT == 0) && (SL_STATUS_IN_PROGRESS == status)) {
      start = osKernelGetTickCount();

      while ((SL_SNTP_CLIENT_START != callback_event) && (osKernelGetTickCount() - start) <= ASYNC_WAIT_TIMEOUT) {
          osThreadYield();
      }

      if (cb_status != SL_STATUS_OK) {
          printf("Failed to start Async SNTP client: 0x%lx\r\n", status);
          return cb_status;
      }
  } else {
      if (status == SL_STATUS_OK) {
          printf("SNTP Client started successfully\n");
      } else {
          printf("Failed to start SNTP client: 0x%lx\r\n", status);
          return status;
      }
  }
  cb_status = SL_STATUS_FAIL;
  status    = sl_sntp_client_get_time(data, DATA_BUFFER_LENGTH, SNTP_API_TIMEOUT);
  if ((SNTP_API_TIMEOUT == 0) && (SL_STATUS_IN_PROGRESS == status)) {
      start = osKernelGetTickCount();

      while ((SL_SNTP_CLIENT_GET_TIME != callback_event) && (osKernelGetTickCount() - start) <= ASYNC_WAIT_TIMEOUT) {
          osThreadYield();
      }

      if (cb_status != SL_STATUS_OK) {
          printf("Failed to get async time from ntp server : 0x%lx\r\n", status);
          return cb_status;
      }
  } else {
      if (status == SL_STATUS_OK) {
          //   printf("SNTP Client got TIME successfully\n");
      } else {
          printf("Failed to get time from ntp server : 0x%lx\r\n", status);
          return status;
      }
  }
  print_char_buffer((char *)data, strlen((const char *)data));
  cb_status = SL_STATUS_FAIL;
  status    = sl_sntp_client_get_time_date(data, DATA_BUFFER_LENGTH, SNTP_API_TIMEOUT);
  if ((SNTP_API_TIMEOUT == 0) && (SL_STATUS_IN_PROGRESS == status)) {
      start = osKernelGetTickCount();

      while ((SL_SNTP_CLIENT_GET_TIME_DATE != callback_event) && (osKernelGetTickCount() - start) <= ASYNC_WAIT_TIMEOUT) {
          osThreadYield();
      }

      if (cb_status != SL_STATUS_OK) {
          printf("Failed to get Async date and time from ntp server : 0x%lx\r\n", status);
          return cb_status;
      }
  } else {
      if (status == SL_STATUS_OK) {
          printf("SNTP Client got TIME and DATE successfully\n");
      } else {
          printf("Failed to get date and time from ntp server : 0x%lx\r\n", status);
          return status;
      }
  }
  print_char_buffer((char *)data, strlen((const char *)data));

  cb_status = SL_STATUS_FAIL;
  status    = sl_sntp_client_get_server_info(&serverInfo, SNTP_API_TIMEOUT);
  if ((SNTP_API_TIMEOUT == 0) && (SL_STATUS_IN_PROGRESS == status)) {
      start = osKernelGetTickCount();

      while ((SL_SNTP_CLIENT_GET_SERVER_INFO != callback_event)
          && (osKernelGetTickCount() - start) <= ASYNC_WAIT_TIMEOUT) {
          osThreadYield();
      }

      if (cb_status != SL_STATUS_OK) {
          printf("Failed to get async ntp server info : 0x%lx\r\n", status);
          return cb_status;
      }
  } else {
      if (status == SL_STATUS_OK) {
          printf("SNTP Client got server info successfully\n");
      } else {
          printf("Failed to get ntp server info : 0x%lx\r\n", status);
          return status;
      }
  }
  printf("Got Server IP version as : %u\n", serverInfo.ip_version);
  if (4 == serverInfo.ip_version) {
      printf("IPv4 Address is : %u.%u.%u.%u\n",
             serverInfo.server_ip_address.ipv4_address[0],
             serverInfo.server_ip_address.ipv4_address[1],
             serverInfo.server_ip_address.ipv4_address[2],
             serverInfo.server_ip_address.ipv4_address[3]);
  } else {
      printf("IPv6 Address is : %lx:%lx:%lx:%lx\n",
             serverInfo.server_ip_address.ipv6_address[0],
             serverInfo.server_ip_address.ipv6_address[1],
             serverInfo.server_ip_address.ipv6_address[2],
             serverInfo.server_ip_address.ipv6_address[3]);
  }
  if(serverInfo.sntp_method==2){
      printf("SNTP Server Method :      %u(broadcast mode)\n", serverInfo.sntp_method);
  }
  else if(serverInfo.sntp_method==1){
      printf("SNTP Server Method :      %u(unicast mode)\n", serverInfo.sntp_method);
  }


  cb_status = SL_STATUS_FAIL;
  status    = sl_sntp_client_stop(SNTP_API_TIMEOUT);
  if ((SNTP_API_TIMEOUT == 0) && (SL_STATUS_IN_PROGRESS == status)) {
      start = osKernelGetTickCount();

      while ((SL_SNTP_CLIENT_STOP != callback_event) && (osKernelGetTickCount() - start) <= ASYNC_WAIT_TIMEOUT) {
          osThreadYield();
      }

      if (cb_status != SL_STATUS_OK) {
          printf("Failed to stop Async SNTP client: 0x%lx\r\n", status);
          return cb_status;
      }
  } else {
      if (status == SL_STATUS_OK) {
      } else {
          printf("Failed to stop SNTP client: 0x%lx\r\n", status);
          return status;
      }

      exec_status = 1;
  }
  printf("Done\r\n");

  while (0 == exec_status) {
      osThreadYield();
  }
  printf("SNTP client execution completed \r\n");
  return SL_STATUS_OK;
}
/******************************************************
 *               Application code
 ******************************************************/
void app_init(const void *unused)
{
  UNUSED_PARAMETER(unused);
  osThreadNew((osThreadFunc_t)application_start, NULL, &thread_attributes);
}

static void application_start(void *argument)
{
  UNUSED_PARAMETER(argument);
  sl_status_t status;
  sl_mac_address_t mac_addr                   = { 0 };
  sl_wifi_firmware_version_t firmware_version = { 0 };
  sl_si91x_rsp_wireless_info_t info={0};
  uint32_t fw_image_size             = 0;
  sl_wifi_rate_t mask;
  sl_wifi_rate_protocol_t rate_protocol;


  printf("\n********Application start********");
#if AP_MODE
  status = sl_net_init(SL_NET_WIFI_AP_INTERFACE, NULL, NULL, NULL);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to start Wi-Fi AP interface: 0x%lx\r\n", status);
      return;
  }
  printf("\r\n********AP Interface********\n");

  sl_wifi_max_tx_power_t max_tx_power;
  max_tx_power.join_tx_power = 20;
  max_tx_power.scan_tx_power = 20;
  status = sl_wifi_set_max_tx_power(SL_WIFI_AP_2_4GHZ_INTERFACE,max_tx_power);
  if (status != SL_STATUS_OK)
    {
      printf("\r\nFailed to set max tx power: 0x%lx\r\n", status);
      return;
    }

  status = sl_wifi_get_mac_address(SL_WIFI_CLIENT_INTERFACE, &mac_addr);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to get mac address: 0x%lx\r\n", status);
      return;
  }
  //! Device MAC address
  printf("\r\nDevice MAC address: %x:%x:%x:%x:%x:%x\r\n",
         mac_addr.octet[0],
         mac_addr.octet[1],
         mac_addr.octet[2],
         mac_addr.octet[3],
         mac_addr.octet[4],
         mac_addr.octet[5]);
  //!Firmware Version
  status = sl_wifi_get_firmware_version(&firmware_version);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to fetch firmware version: 0x%lx\r\n", status);
      return;
  } else {
      print_firmware_version(&firmware_version);
  }

  //! Firmware size Prints
  status = sl_wifi_get_firmware_size(FW_LOC, &fw_image_size);
  if (status != SL_STATUS_OK) {
      printf("Unable to fetch firmware size. Status: 0x%lx\n", status);
  } else{
      printf("firmware size =      %lu\n", fw_image_size);
  }
  //! Firmware status
  printf("\nFirmware Status:       0x%lx\n", sl_si91x_get_saved_firmware_status()); 

  //! NWP Get Configuration for OPN
  char output[20];
  sl_si91x_nwp_get_configuration_t config = {.sub_command_type = GET_OPN_BOARD_CONFIG};
  status = sl_si91x_get_nwp_config(&config,(unsigned char *)output);
  output[16]='\0';

  if (status != SL_STATUS_OK) {
      printf("\r\nsl_si91x_get_nwp_config failed with status 0x%lu\r\n", status);
      return;
  } else {
      printf("\r\nOPN number of the Device:'%s'\r\n", output);
  }

  //! efuse read
  sl_si91x_efuse_read_t efuse_read = {.efuse_read_addr_offset = 154, .efuse_read_data_len = 6};
  uint8_t efuse_read_buf[6]={0};
  status = sl_si91x_efuse_read(&efuse_read, efuse_read_buf);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to read efuse : 0x%lx\r\n", status);
  }
  else{
      for (int i=0;i<6;i++){
          printf("%x ",efuse_read_buf[i]);
      }
      printf("\r\nefuse_read success \r\n");
  }

  status = sl_net_up(SL_NET_WIFI_AP_INTERFACE, SL_NET_DEFAULT_WIFI_AP_PROFILE_ID);
  if (status != SL_STATUS_OK)
    {
      printf("\r\nFailed to bring Wi-Fi AP interface up: 0x%lx\r\n", status);
      return;
    }
  printf("\r\nAP mode started\r\n");

  //! TX Power prints
  status = sl_wifi_get_max_tx_power(SL_WIFI_AP_2_4GHZ_INTERFACE,&max_tx_power);
  if (status != SL_STATUS_OK)
    {
      printf("\r\nFailed to get max tx power: 0x%lx\r\n", status);
      return;
    }
  printf("Max TX Power:         %d\n", max_tx_power.join_tx_power);
  // printf("scan TX Power: %d\n", max_tx_power.scan_tx_power);
  //! Wireless info Prints

  status = sl_wifi_get_wireless_info(&info);
  if (status == SL_STATUS_OK) {
      print_wireless_info(&info);
  } else {
      printf("Failed to get wireless info Status: %lu\n", status);
  }

  //!Get Antenna not implemented
  //      status = sl_wifi_get_antenna(SL_WIFI_AP_2_4GHZ_INTERFACE,&antenna);
  //             if (status != SL_STATUS_OK)
  //             {
  //                 printf("\r\nFailed to antenna : 0x%lx\r\n", status);
  //                 return;
  //             }
  //           printf("\r\n Antenna = %u\r\n", antenna);

  const char* get_wifi_rate_protocol_string(int value) {
    switch (value) {
      case SL_WIFI_RATE_PROTOCOL_B_ONLY:
        return "SL_WIFI_RATE_PROTOCOL_B_ONLY";
      case SL_WIFI_RATE_PROTOCOL_G_ONLY:
        return "SL_WIFI_RATE_PROTOCOL_G_ONLY";
      case SL_WIFI_RATE_PROTOCOL_N_ONLY:
        return "SL_WIFI_RATE_PROTOCOL_N_ONLY";
      case SL_WIFI_RATE_PROTOCOL_AC_ONLY:
        return "SL_WIFI_RATE_PROTOCOL_AC_ONLY";
      case SL_WIFI_RATE_PROTOCOL_AX_ONLY:
        return "SL_WIFI_RATE_PROTOCOL_AX_ONLY";
      case SL_WIFI_RATE_PROTOCOL_AUTO:
        return "SL_WIFI_RATE_PROTOCOL_AUTO";
      default:
        return "Unknown Protocol";
    }
  }
  const char* get_wifi_rate_string(int rate) {
    switch (rate) {
      case SL_WIFI_AUTO_RATE:
        return "SL_WIFI_AUTO_RATE";
      case SL_WIFI_RATE_11B_1:
        return "SL_WIFI_RATE_11B_1";
      case SL_WIFI_RATE_11B_2:
        return "SL_WIFI_RATE_11B_2";
      case SL_WIFI_RATE_11B_5_5:
        return "SL_WIFI_RATE_11B_5_5";
      case SL_WIFI_RATE_11B_11:
        return "SL_WIFI_RATE_11B_11";
      case SL_WIFI_RATE_11G_6:
        return "SL_WIFI_RATE_11G_6";
      case SL_WIFI_RATE_11G_9:
        return "SL_WIFI_RATE_11G_9";
      case SL_WIFI_RATE_11G_12:
        return "SL_WIFI_RATE_11G_12";
      case SL_WIFI_RATE_11G_18:
        return "SL_WIFI_RATE_11G_18";
      case SL_WIFI_RATE_11G_24:
        return "SL_WIFI_RATE_11G_24";
      case SL_WIFI_RATE_11G_36:
        return "SL_WIFI_RATE_11G_36";
      case SL_WIFI_RATE_11G_48:
        return "SL_WIFI_RATE_11G_48";
      case SL_WIFI_RATE_11G_54:
        return "SL_WIFI_RATE_11G_54";
      case SL_WIFI_RATE_11N_MCS0:
        return "SL_WIFI_RATE_11N_MCS0";
      case SL_WIFI_RATE_11AX_MCS7:
        return "SL_WIFI_RATE_11AX_MCS7";
      case SL_WIFI_RATE_INVALID:
        return "SL_WIFI_RATE_INVALID";
      default:
        return "Unknown Rate";
    }
  }
  //!Transmit rate
  status = sl_wifi_get_transmit_rate(SL_WIFI_AP_2_4GHZ_INTERFACE, &rate_protocol, &mask);

  if (status == SL_STATUS_OK) {
      printf("\r\nTransmit rate details of the device: \nData rate protocol is:       %d(%s)\nData rate is:         %d(%s)\r\n",rate_protocol,get_wifi_rate_protocol_string(rate_protocol),mask,get_wifi_rate_string(mask));
  }
  //!listen interval
  sl_wifi_listen_interval_t listen_interval;
  status = sl_wifi_get_listen_interval(SL_WIFI_AP_2_4GHZ_INTERFACE, &listen_interval);
  if (status == SL_STATUS_OK) {
      printf("\r\nlisten interval configured for the device =     %lu ms\n",listen_interval.listen_interval);
  }
  //!advanced scan configuration
  sl_wifi_advanced_scan_configuration_t configuration;
  status = sl_wifi_get_advanced_scan_configuration(&configuration);
  if (status == SL_STATUS_OK) {
      print_advanced_scan_configuration(&configuration);
  }
  //!AP configurations
  sl_wifi_ap_configuration_t ap_configuration;
  status = sl_wifi_get_ap_configuration(SL_WIFI_AP_2_4GHZ_INTERFACE, &ap_configuration);
  if (status == SL_STATUS_OK) {
      print_ap_configuration(&ap_configuration);
  }
  //!Clients Info
  sl_wifi_client_info_response_t client_info = { 0 };
  status = sl_wifi_get_ap_client_info(SL_WIFI_AP_INTERFACE, &client_info);
  printf("********Clients info:********\n");
  if (client_info.client_count <= 0) {
      printf("\r\nNo clients connected !!\n");
  }
  for (uint16_t station_info_index = 0; station_info_index < client_info.client_count; station_info_index++) {
      sl_wifi_client_info_t *station_info = &client_info.client_info[station_info_index];
      printf("%d MAC Address is %x:%x:%x:%x:%x:%x",
             station_info_index + 1,
             station_info->mac_adddress.octet[0],
             station_info->mac_adddress.octet[1],
             station_info->mac_adddress.octet[2],
             station_info->mac_adddress.octet[3],
             station_info->mac_adddress.octet[4],
             station_info->mac_adddress.octet[5]);

      print_sl_ip_address(&station_info->ip_address);
  }
  //! clients list
  sl_wifi_interface_t interface1={0};
  sl_mac_address_t client_list[SL_WIFI_MAX_CLIENT_COUNT] = { 0 };
  status = sl_wifi_get_ap_client_list(interface1, SL_WIFI_MAX_CLIENT_COUNT, client_list);
  printf("Mac address list of clients connected to AP:\n");
  for (uint16_t index = 0; client_list[index].octet[0] != 0; index++) {
      printf("\n %d -> %x:%x:%x:%x:%x:%x",
             index,
             client_list[index].octet[0],
             client_list[index].octet[1],
             client_list[index].octet[2],
             client_list[index].octet[3],
             client_list[index].octet[4],
             client_list[index].octet[5]);
  }

  //! clients count
  uint32_t client_list_count;
  status = sl_wifi_get_ap_client_count(interface1, &client_list_count);
  if (status == SL_STATUS_OK) {
      printf("Client count =    %lu\n", client_list_count);
  }
  //! Power and Performance
  sl_wifi_performance_profile_t profile;
  status= sl_wifi_get_performance_profile(&profile);
  if (status == SL_STATUS_OK){
      print_performance_profile(&profile);
  }
  //!statistics
  sl_wifi_statistics_t statistics;
  status = sl_wifi_get_statistics(SL_WIFI_AP_2_4GHZ_INTERFACE, &statistics);
  if (status == SL_STATUS_OK) {
      print_statistics(&statistics);
  }
  //!Operational statistics
  sl_wifi_operational_statistics_t operational_statistics;
  status = sl_wifi_get_operational_statistics(interface_1, &operational_statistics);
  if (status == SL_STATUS_OK) {
      print_operational_statistics(&operational_statistics);
  }
  //!net profile
  sl_net_profile_id_t profile_id = SL_NET_DEFAULT_WIFI_AP_PROFILE_ID;
  sl_net_wifi_ap_profile_t profile1;
  status = sl_net_get_profile(SL_NET_WIFI_AP_INTERFACE, profile_id , &profile1);
  print_ap_configuration(&(profile1.config));
  ip_address.type = SL_IPV4;
  memcpy(&ip_address.ip.v4.bytes, &profile1.ip.ip.v4.ip_address.bytes, sizeof(sl_ipv4_address_t));
  print_sl_ip_address(&ip_address);
  //! RTC timer
  sl_si91x_module_rtc_time_t tm_rtc;
  tm_rtc.tm_sec = 1;
  tm_rtc.tm_min = 1;
  tm_rtc.tm_hour = 1;
  tm_rtc.tm_mday = 1;
  tm_rtc.tm_mon = 1;
  tm_rtc.tm_wday = 1;
  tm_rtc.tm_year = 2024;
  status = sl_si91x_set_rtc_timer(&tm_rtc);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to set RTC : 0x%lx\r\n", status);
      return;
  }

  printf("********RTC Timer:********\n");
  printf("seconds:    %lusec\n", tm_rtc.tm_sec);
  printf("minutes:    %lumin\n", tm_rtc.tm_min);
  printf("hours:      %luhr\n", tm_rtc.tm_hour);
  printf("month_day:  %lu\n", tm_rtc.tm_mday);
  printf("month:      %lu\n", tm_rtc.tm_mon);
  printf("year:       %lu\n", tm_rtc.tm_year);
  printf("day:        %lu\n", tm_rtc.tm_wday);


  osDelay(10000);
  printf("\r\n delay 10s\r\n");
  status = sl_si91x_get_rtc_timer(&tm_rtc);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to get RTC : 0x%lx\r\n", status);
      return;
  }

  printf("********RTC Timer:********\n");
  printf("seconds:   %lusec\n", tm_rtc.tm_sec);
  printf("minutes:   %lumin\n", tm_rtc.tm_min);
  printf("hours:     %luhr\n", tm_rtc.tm_hour);
  printf("month_day: %lu\n", tm_rtc.tm_mday);
  printf("month:     %lu\n", tm_rtc.tm_mon);
  printf("year:      %lu\n", tm_rtc.tm_year);
  printf("day:       %lu\n", tm_rtc.tm_wday);
  //!Deinit AP Mode
  status = sl_net_deinit(SL_NET_WIFI_AP_INTERFACE);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to do deinit : 0x%lx\r\n", status);
      //return;
  }
  printf("\r\nDeinit of AP interface success\r\n");
#endif

#if STA_MODE
  printf("\r\n ********Client Interface********\n");
  status = sl_net_init(SL_NET_WIFI_CLIENT_INTERFACE,&station_configuration, NULL, NULL);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to start Wi-Fi Client interface: 0x%lx\r\n", status);
      return;
  }
  // printf("\r\nWi-Fi client interface init success\r\n");

  status = sl_wifi_get_mac_address(SL_WIFI_CLIENT_INTERFACE, &mac_addr);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to get mac address: 0x%lx\r\n", status);
      return;
  }
  printf("\r\nDevice MAC address: %x:%x:%x:%x:%x:%x\r\n",
         mac_addr.octet[0],
         mac_addr.octet[1],
         mac_addr.octet[2],
         mac_addr.octet[3],
         mac_addr.octet[4],
         mac_addr.octet[5]);

  status = sl_wifi_get_firmware_version(&firmware_version);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to fetch firmware version:  0x%lx\r\n", status);
      return;
  } else {
      print_firmware_version(&firmware_version);
  }

  //! Firmware size Prints
  status = sl_wifi_get_firmware_size(FW_LOC, &fw_image_size);
  if (status != SL_STATUS_OK) {
      printf("Unable to fetch firmware size. Status: 0x%lx\n", status);

  } else{
      printf("firmware size =      %lu\n", fw_image_size);
  }
  //! TXpower prints
  status = sl_wifi_get_max_tx_power(SL_WIFI_CLIENT_2_4GHZ_INTERFACE,&max_tx_power);
  if (status != SL_STATUS_OK)
    {
      printf("\r\nFailed to set max tx power: 0x%lx\r\n", status);
      return;
    }
  printf("Max TX Power:        %d\n", max_tx_power.join_tx_power);

  //! Pairwise master key


  sl_wifi_set_callback(SL_WIFI_STATS_RESPONSE_EVENTS, module_status_handler, NULL);
  status = sl_net_up(SL_NET_WIFI_CLIENT_INTERFACE, SL_NET_DEFAULT_WIFI_CLIENT_PROFILE_ID);
  if (status != SL_STATUS_OK) {
      printf("\r\nFailed to connect to AP: 0x%lx\r\n", status);
      return;
  }
  printf("\r\nWi-Fi client connected\r\n");
  embedded_sntp_client();

  status = sl_net_get_profile(SL_NET_WIFI_CLIENT_INTERFACE, SL_NET_DEFAULT_WIFI_CLIENT_PROFILE_ID, &profile);
  if (status != SL_STATUS_OK) {
      printf("Failed to get client profile: 0x%lx\r\n", status);
      return;
  }
  //  printf("\r\nSuccess to get client profile\r\n");
  //! Wireless info Prints
  status = sl_wifi_get_wireless_info(&info);

  if (status == SL_STATUS_OK) {
      print_wireless_info(&info);
  } else {
      printf("Failed to get wireless info. Status: %ld\n", status);
  }

  //!Signal Strength
  int32_t signal_strength = 0;
  sl_wifi_interface_t STA_interface = SL_WIFI_CLIENT_2_4GHZ_INTERFACE;
  status = sl_wifi_get_signal_strength(STA_interface, &signal_strength);
  printf("Signal strength=       %lddbm\n", signal_strength);

  //!TSF prints
  sl_wifi_tsf64_t tsf = { 0 };
  status = sl_wifi_get_sta_tsf(STA_interface, &tsf);
  if (status != SL_STATUS_OK) {
      printf("\r\nGet station TSF failed, Error Code : 0x%lX", status);
  } else {
      printf("\r\nGet station TSF success: tsf_m=0x%lx, tsf_l=0x%lx\r\n", tsf.tsf_m, tsf.tsf_l);
  }
}
#endif
