/*******************************************************************************
* @file  wifi_config.h
* @brief
*******************************************************************************
* # License
* <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
 * @file         wifi_config.h
 * @version      0.1
 *
 *  @brief : This file contains user configurable details to configure the device
 *
 *  @section Description  This file contains user configurable details to configure the device
 *
 *
 */
#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#define RSI_APP_BUF_SIZE 1600

//! Enumeration for states in application
typedef enum sl_wifi_app_state_e {
  SL_WIFI_INITIAL_STATE                 = 0,
  SL_WIFI_UNCONNECTED_STATE             = 1,
  SL_WIFI_CONNECTED_STATE               = 2,
  SL_WIFI_IPCONFIG_DONE_STATE           = 3,
  SL_WIFI_SOCKET_CONNECTED_STATE        = 4,
  SL_WIFI_SCAN_STATE                    = 5,
  SL_WIFI_JOIN_STATE                    = 6,
  SL_WIFI_SOCKET_RECEIVE_STATE          = 7,
  SL_WIFI_MQTT_INIT_STATE               = 8,
  SL_WIFI_MQTT_CONNECT_STATE            = 9,
  SL_BLE_GATT_WRITE_EVENT               = 10,
  SL_WIFI_DISCONNECTED_STATE            = 11,
  SL_WIFI_DISCONN_NOTIFY_STATE          = 12,
  SL_AWS_SELECT_CONNECT_STATE           = 13,
  SL_SLEEP_STATE                        = 14,
  SL_WIFI_DATA_RECEIVE_STATE            = 15,
  SL_SD_WRITE_STATE                     = 16,
  SL_WIFI_DEMO_COMPLETE_STATE           = 17,
  SL_WIFI_MQTT_AUTO_RECONNECT_SET_STATE = 18,
  SL_WIFI_MQTT_SUBSCRIBE_STATE          = 19,
  SL_WIFI_MQTT_PUBLISH_STATE            = 20,
  SL_WIFI_ERROR_STATE                   = 21,
  SL_WIFI_FLASH_STATE                   = 22,
  SL_IDLE_STATE                         = 23,
  SL_WLAN_MQTT_DISCONNECT               = 24,
  SEND_DATA                             = 25,
  RECEIVE_DATA                          = 26,
  SL_WLAN_SET_MQTT                      = 27,

} sl_wifi_app_state_t;

typedef enum rsi_app_cmd_e {
  SL_DATA                      = 0,
  SL_WIFI_SCAN_RESP            = 1,
  SL_WIFI_CONNECTION_STATUS    = 2,
  SL_WIFI_DISCONNECTION_STATUS = 3,
  SL_WIFI_DISCONNECTION_NOTIFY = 4,
  SL_WIFI_TIMEOUT_NOTIFY       = 5
} rsi_app_cmd_t;
#define CONCURRENT_MODE     RSI_DISABLE
#define RSI_FEATURE_BIT_MAP (FEAT_SECURITY_OPEN | FEAT_AGGREGATION | FEAT_ULP_GPIO_BASED_HANDSHAKE)
#define RSI_TCP_IP_BYPASS   RSI_DISABLE
#define RSI_TCP_IP_FEATURE_BIT_MAP                                                               \
  (TCP_IP_FEAT_DHCPV4_CLIENT | TCP_IP_FEAT_SSL | TCP_IP_FEAT_DNS_CLIENT | TCP_IP_TOTAL_SOCKETS_1 \
   | TCP_IP_FEAT_EXTENSION_VALID)
#define RSI_CUSTOM_FEATURE_BIT_MAP FEAT_CUSTOM_FEAT_EXTENTION_VALID
#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP                                               \
  (EXT_FEAT_384K_MODE | EXT_FEAT_LOW_POWER_MODE | EXT_FEAT_RSA_KEY_WITH_4096_SUPPORT \
   | EXT_FEAT_SSL_CERT_WITH_4096_KEY_SUPPORT)
#define RSI_EXT_TCPIP_FEATURE_BIT_MAP \
  (EXT_DYNAMIC_COEX_MEMORY | EXT_TCP_IP_FEAT_SSL_MEMORY_CLOUD | CONFIG_FEAT_EXTENTION_VALID)
#define RSI_CONFIG_FEATURE_BITMAP RSI_FEAT_SLEEP_GPIO_SEL_BITMAP
#define RSI_BT_FEATURE_BITMAP     (BT_RF_TYPE | ENABLE_BLE_PROTOCOL)
#define RSI_BAND
#endif
