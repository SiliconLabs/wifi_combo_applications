/*******************************************************************************
* @file  rsi_wlan_config.h
* @brief 
*******************************************************************************
* # License
* <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef RSI_CONFIG_H
#define RSI_CONFIG_H

#include "rsi_wlan_defines.h"

// Enable feature
#define RSI_ENABLE 1
// Disable feature
#define RSI_DISABLE 0

// To enable concurrent mode
#define CONCURRENT_MODE RSI_DISABLE

// opermode command parameters
/*=======================================================================*/
// To set wlan feature select bit map
#define RSI_FEATURE_BIT_MAP (FEAT_SECURITY_OPEN | FEAT_AGGREGATION | FEAT_ULP_GPIO_BASED_HANDSHAKE)

// TCP IP BYPASS feature check
#define RSI_TCP_IP_BYPASS RSI_DISABLE

// TCP/IP feature select bitmap for selecting TCP/IP features
#define RSI_TCP_IP_FEATURE_BIT_MAP (TCP_IP_FEAT_EXTENSION_VALID | TCP_IP_FEAT_DHCPV4_CLIENT)

// To set custom feature select bit map
#define RSI_CUSTOM_FEATURE_BIT_MAP FEAT_CUSTOM_FEAT_EXTENTION_VALID

// To set Extended custom feature select bit map
#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP (EXT_FEAT_LOW_POWER_MODE | EXT_FEAT_XTAL_CLK_ENABLE)

#define RSI_EXT_TCPIP_FEATURE_BITMAP CONFIG_FEAT_EXTENTION_VALID

// config feature bit map
#define RSI_CONFIG_FEATURE_BITMAP (RSI_FEAT_SLEEP_GPIO_SEL_BITMAP | ENABLE_ENHANCED_MAX_PSP)

/*=======================================================================*/

// Feature frame parameters
/*=======================================================================*/
#define PLL_MODE      0
#define RF_TYPE       1 // 0 - External RF 1- Internal RF
#define WIRELESS_MODE 0
#define ENABLE_PPP    0
#define AFE_TYPE      1
#define FEATURE_ENABLES \
  (RSI_FEAT_FRAME_PREAMBLE_DUTY_CYCLE | RSI_FEAT_FRAME_LP_CHAIN | RSI_FEAT_FRAME_IN_PACKET_DUTY_CYCLE)
/*=======================================================================*/

// Band command parameters
/*=======================================================================*/

// RSI_BAND_2P4GHZ(2.4GHz) or RSI_BAND_5GHZ(5GHz) or RSI_DUAL_BAND
#define RSI_BAND RSI_BAND_2P4GHZ

/*=======================================================================*/

// set region command parameters
/*=======================================================================*/

// RSI_ENABLE or RSI_DISABLE Set region support
#define RSI_SET_REGION_SUPPORT RSI_DISABLE //@ RSI_ENABLE or RSI_DISABLE set region

// If 1:region configurations taken from user ;0:region configurations taken from beacon
#define RSI_SET_REGION_FROM_USER_OR_BEACON 1

// 0-Default Region domain ,1-US, 2-EUROPE, 3-JAPAN
#define RSI_REGION_CODE 3

// 0- Without On Board Antenna , 1- With On Board Antenna
#define RSI_MODULE_TYPE 1

/*=======================================================================*/

// set region AP command parameters
/*=======================================================================*/

// RSI_ENABLE or RSI_DISABLE Set region AP support
#define RSI_SET_REGION_AP_SUPPORT RSI_DISABLE

// If 1:region configurations taken from user ;0:region configurations taken from firmware
#define RSI_SET_REGION_AP_FROM_USER RSI_DISABLE

// "US" or "EU" or "JP" or other region codes
#define RSI_COUNTRY_CODE "US "

/*=======================================================================*/

// Rejoin parameters
/*=======================================================================*/

// RSI_ENABLE or RSI_DISABLE rejoin parameters
#define RSI_REJOIN_PARAMS_SUPPORT RSI_DISABLE

// Rejoin retry count. If 0 retries infinity times
#define RSI_REJOIN_MAX_RETRY 0

// Periodicity of rejoin attempt
#define RSI_REJOIN_SCAN_INTERVAL 4

// Beacon missed count
#define RSI_REJOIN_BEACON_MISSED_COUNT 40

// RSI_ENABLE or RSI_DISABLE retry for first time join failure
#define RSI_REJOIN_FIRST_TIME_RETRY RSI_DISABLE

/*=======================================================================*/


/*=======================================================================*/

//RSI_ENABLE or RSI_DISABLE to set RTS threshold config
#define RSI_WLAN_CONFIG_ENABLE RSI_ENABLE

#define CONFIG_RTSTHRESHOLD 1

#define RSI_RTS_THRESHOLD 2346

/*=======================================================================*/


/*=======================================================================*/

// High Throughput capabilities related information
/*=======================================================================*/

// RSI_ENABLE or RSI_DISABLE 11n mode in AP mode
#define RSI_MODE_11N_ENABLE RSI_DISABLE

// HT caps bit map.
#define RSI_HT_CAPS_BIT_MAP \
  (RSI_HT_CAPS_NUM_RX_STBC | RSI_HT_CAPS_SHORT_GI_20MHZ | RSI_HT_CAPS_GREENFIELD_EN | RSI_HT_CAPS_SUPPORT_CH_WIDTH)

/*=======================================================================*/

// Scan command parameters
/*=======================================================================*/

// scan channel bit map in 2.4GHz band,valid if given channel to scan is 0
#define RSI_SCAN_CHANNEL_BIT_MAP_2_4 0

// scan channel bit map in 5GHz band ,valid if given channel to scan is 0
#define RSI_SCAN_CHANNEL_BIT_MAP_5 0

// scan_feature_bitmap ,valid only if specific channel to scan and ssid are given
#define RSI_SCAN_FEAT_BITMAP 0

/*=======================================================================*/

// Enterprise configuration command parameters
/*=======================================================================*/

// Enterprise method ,should be one of among TLS, TTLS, FAST or PEAP
#define RSI_EAP_METHOD "TTLS"
// This parameter is used to configure the module in Enterprise security mode
#define RSI_EAP_INNER_METHOD "\"auth=MSCHAPV2\""
// Private Key Password is required for encrypted private key, format is like "\"12345678\""
#define RSI_PRIVATE_KEY_PASSWORD ""

/*=======================================================================*/

// Join command parameters
/*=======================================================================*/

// Tx power level
#define RSI_POWER_LEVEL RSI_POWER_LEVEL_HIGH

// RSI_JOIN_FEAT_STA_BG_ONLY_MODE_ENABLE or RSI_JOIN_FEAT_LISTEN_INTERVAL_VALID
#define RSI_JOIN_FEAT_BIT_MAP RSI_JOIN_FEAT_LISTEN_INTERVAL_VALID

// listen interval
#define RSI_LISTEN_INTERVAL 1000

// Transmission data rate. Physical rate at which data has to be transmitted.
#define RSI_DATA_RATE RSI_DATA_RATE_AUTO

/*=======================================================================*/

// Ipconf command parameters
/*=======================================================================*/

// DHCP client host name
#define RSI_DHCP_HOST_NAME "dhcp_client"



// Power save command parameters
/*=======================================================================*/
// set handshake type of power mode
#define RSI_HAND_SHAKE_TYPE GPIO_BASED
//#define RSI_HAND_SHAKE_TYPE MSG_BASED

// 0 - LP, 1- ULP mode with RAM retention and 2 - ULP with Non RAM retention
#define RSI_SELECT_LP_OR_ULP_MODE RSI_ULP_WITH_RAM_RET
// set DTIM aligment required
// 0 - module wakes up at beacon which is just before or equal to listen_interval
// 1 - module wakes up at DTIM beacon which is just before or equal to listen_interval
#define RSI_DTIM_ALIGNED_TYPE 0

// Monitor interval for the FAST PSP mode
// default is 50 ms, and this parameter is valid for FAST PSP only
#define RSI_MONITOR_INTERVAL 50

// Number of DTIMs to skip during powersave
#define RSI_NUM_OF_DTIM_SKIP 0

//WMM PS parameters
// set WMM enable or disable
#define RSI_WMM_PS_ENABLE RSI_DISABLE

// set WMM enable or disable
// 0- TX BASED 1 - PERIODIC
#define RSI_WMM_PS_TYPE 0

// set WMM wake up interval
#define RSI_WMM_PS_WAKE_INTERVAL 20

// set WMM UAPSD bitmap
#define RSI_WMM_PS_UAPSD_BITMAP 15

/*=======================================================================*/

// Timeout for join or scan
/*=======================================================================*/

//RSI_ENABLE or RSI_DISABLE Timeout support
#define RSI_TIMEOUT_SUPPORT RSI_DISABLE

// roaming threshold value
#define RSI_TIMEOUT_BIT_MAP 1

// roaming hysteresis value
#define RSI_TIMEOUT_VALUE 1500

// Timeout for ping request
/*=======================================================================*/

//Timeout for PING_REQUEST
#define RSI_PING_REQ_TIMEOUT_MS 1000


// Client Profile Parameters
/* ===================================================================================== */

// To configure data rate
#define RSI_CONFIG_CLIENT_DATA_RATE RSI_DATA_RATE_AUTO
// To configure wlan feature bitmap
#define RSI_CONFIG_CLIENT_WLAN_FEAT_BIT_MAP 0
// To configure tcp/ip feature bitmap
#define RSI_CONFIG_CLIENT_TCP_IP_FEAT_BIT_MAP BIT(2)
// To configure custom feature bit map
#define RSI_CONFIG_CLIENT_CUSTOM_FEAT_BIT_MAP 0
// To configure TX power
#define RSI_CONFIG_CLIENT_TX_POWER RSI_POWER_LEVEL_HIGH
// To configure listen interval
#define RSI_CONFIG_CLIENT_LISTEN_INTERVAL 0
// To configure SSID
#define RSI_CONFIG_CLIENT_SSID "SILABS_AP"
// RSI_BAND_2P4GHZ(2.4GHz) or RSI_BAND_5GHZ(5GHz) or RSI_DUAL_BAND
#define RSI_CONFIG_CLIENT_BAND RSI_BAND_2P4GHZ
// To configure channel number
#define RSI_CONFIG_CLIENT_CHANNEL 0
// To configure security type
#define RSI_CONFIG_CLIENT_SECURITY_TYPE 0 //RSI_WPA
// To configure encryption type
#define RSI_CONFIG_CLIENT_ENCRYPTION_TYPE 0
// To configure PSK
#define RSI_CONFIG_CLIENT_PSK "1234567890"
// To configure PMK
#define RSI_CONFIG_CLIENT_PMK ""
// Client Network parameters
// TCP_STACK_USED BIT(0) - IPv4, BIT(1) -IPv6, (BIT(0) | BIT(1)) - Both IPv4 and IPv6
#define RSI_CONFIG_CLIENT_TCP_STACK_USED BIT(0)
// DHCP mode 1- Enable 0- Disable
// If DHCP mode is disabled given IP statically
#define RSI_CONFIG_CLIENT_DHCP_MODE RSI_DHCP
// IP address of the module
// E.g: 0x0A0AA8C0 == 192.168.10.10
#define RSI_CONFIG_CLIENT_IP_ADDRESS 0x0A0AA8C0
// IP address of netmask
// E.g: 0x00FFFFFF == 255.255.255.0
#define RSI_CONFIG_CLIENT_SN_MASK_ADDRESS 0x00FFFFFF
// IP address of Gateway
// E.g: 0x010AA8C0 == 192.168.10.1
#define RSI_CONFIG_CLIENT_GATEWAY_ADDRESS 0x010AA8C0
// scan channel bit map in 2.4GHz band,valid if given channel to scan is 0
#define RSI_CONFIG_CLIENT_SCAN_FEAT_BITMAP 0
// Scan channel magic code
#define RSI_CONFIG_CLIENT_MAGIC_CODE 0x4321
// scan channel bit map in 2.4GHz band,valid if given channel to scan is 0
#define RSI_CONFIG_CLIENT_SCAN_CHAN_BITMAP_2_4_GHZ 0
// scan channel bit map in 5GHz band ,valid if given channel to scan is 0
#define RSI_CONFIG_CLIENT_SCAN_CHAN_BITMAP_5_0_GHZ 0

// P2P Profile parameters
/* ================================================================================= */

// To configure data rate
#define RSI_CONFIG_P2P_DATA_RATE RSI_DATA_RATE_AUTO
// To configure wlan feature bitmap
#define RSI_CONFIG_P2P_WLAN_FEAT_BIT_MAP 0
// To configure P2P tcp/ip feature  bitmap
#define RSI_CONFIG_P2P_TCP_IP_FEAT_BIT_MAP BIT(2)
// To configure P2P custom feature bitmap
#define RSI_CONFIG_P2P_CUSTOM_FEAT_BIT_MAP 0
// TO configure P2P tx power level
#define RSI_CONFIG_P2P_TX_POWER RSI_POWER_LEVEL_HIGH
// Set P2P go intent
#define RSI_CONFIG_P2P_GO_INTNET 16 // Support only Autonomous GO mode
// Set device name
#define RSI_CONFIG_P2P_DEVICE_NAME "WSC1.1"
// Set device operating channel
#define RSI_CONFIG_P2P_OPERATING_CHANNEL 11
// Set SSID postfix
#define RSI_CONFIG_P2P_SSID_POSTFIX "WSC_1_0_0"
// Set P2P join SSID
#define RSI_CONFIG_P2P_JOIN_SSID "SILABS_AP"
// Set psk key
#define RSI_CONFIG_P2P_PSK_KEY "12345678"
// P2P Network parameters
// TCP_STACK_USED BIT(0) - IPv4, BIT(1) -IPv6, (BIT(0) | BIT(1)) - Both IPv4 and IPv6
#define RSI_CONFIG_P2P_TCP_STACK_USED BIT(0)
// DHCP mode 1- Enable 0- Disable
// If DHCP mode is disabled given IP statically
#define RSI_CONFIG_P2P_DHCP_MODE 1
// IP address of the module
// E.g: 0x0A0AA8C0 == 192.168.10.10
#define RSI_CONFIG_P2P_IP_ADDRESS 0x0A0AA8C0
// IP address of netmask
// E.g: 0x00FFFFFF == 255.255.255.0
#define RSI_CONFIG_P2P_SN_MASK_ADDRESS 0x00FFFFFF
// IP address of Gateway
// E.g: 0x010AA8C0 == 192.168.10.1
#define RSI_CONFIG_P2P_GATEWAY_ADDRESS 0x010AA8C0

#endif
