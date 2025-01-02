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
#ifndef RSI_WLAN_CONFIG_H
#define RSI_WLAN_CONFIG_H

#include "rsi_wlan_defines.h"
#include "rsi_common_config.h"

/*=======================================================================================================*/
//!	 APP CONFIG defines
/*=======================================================================================================*/
#define SSID               							"Tplink"            //! Access point SSID to connect
#define SECURITY_TYPE								RSI_WPA2            //! Security type -  RSI_WPA2 or RSI_OPEN
#define CHANNEL_NO              					0					//! Channel in which device should scan, 0 - scan all channels
#define PSK                							"12345678"     //! Password
#define PORT_NUM          							5001				//! Module port number
#define SERVER_PORT       							5001				//! Server port number
#define SERVER_IP_ADDRESS  							"192.168.0.101"		//! Server IP address
#define DHCP_MODE          							1                   //! DHCP mode 1- Enable 0- Disable
#if !(DHCP_MODE)														//! If DHCP mode is disabled give IP statically
#define DEVICE_IP          							0x6500A8C0       	//! IP address of the module  - E.g: 0x650AA8C0 == 192.168.0.101
#define GATEWAY            							0x0100A8C0         	//! IP address of Gateway  - E.g: 0x010AA8C0 == 192.168.0.1
#define NETMASK            							0x00FFFFFF         	//! IP address of netmask - E.g: 0x00FFFFFF == 255.255.255.0
#endif
//! Type of throughput
#define  UDP_TX                     				0
#define  UDP_RX                    					1
#define  TCP_TX                     				2
#define  TCP_RX                     				3
#define  SSL_TX                     				4
#define  SSL_RX                     				5
#define THROUGHPUT_TYPE  							UDP_TX				//! measurement type
#define THROUGHPUT_AVG_TIME							60000				//! throughput average time in ms
#define	MAX_TX_PKTS									10000				//! Applies in SSL TX, TCP_RX and UDP_RX calculate throughput after transmitting MAX_TX_PKTS
//! Memory length for send buffer
#if((THROUGHPUT_TYPE  == TCP_TX) || (THROUGHPUT_TYPE  == TCP_RX))
#define BUF_SIZE   1460
#elif ((THROUGHPUT_TYPE  == UDP_TX) || (THROUGHPUT_TYPE  == UDP_RX))
#define BUF_SIZE   1470
#elif ((THROUGHPUT_TYPE  == SSL_TX) || (THROUGHPUT_TYPE  == SSL_RX))
#define BUF_SIZE   1370
#endif
#define RSI_DNS_CLIENT								0                	//! Enable if using DNS client (when using server hostname instead of ip addr)
#define SOCKET_ASYNC_FEATURE						1
#define TX_RX_RATIO_ENABLE   						0
#define RSI_APP_BUF_SIZE        					1600

//! Enumeration for states in application
typedef enum rsi_wlan_app_state_e
{
	RSI_WLAN_INITIAL_STATE          = 0,
	RSI_WLAN_UNCONNECTED_STATE      = 1,
	RSI_WLAN_SCAN_DONE_STATE        = 2,
	RSI_WLAN_CONNECTED_STATE        = 3,
	RSI_WLAN_IPCONFIG_DONE_STATE    = 4,
	RSI_WLAN_DISCONNECTED_STATE     = 5,
	RSI_WLAN_SOCKET_CONNECTED_STATE = 6,
	RSI_POWER_SAVE_STATE  	        = 7,
	RSI_WLAN_IDLE_STATE		= 8,

}rsi_wlan_app_state_t;

//! WLAN application control block
typedef struct rsi_wlan_app_cb_s
{
	rsi_wlan_app_state_t state;        //! WLAN application state
	uint32_t length;                   //! length of buffer to copy
	uint8_t buffer[RSI_APP_BUF_SIZE];  //! application buffer
	uint8_t buf_in_use;                //! to check application buffer availability
	uint32_t event_map;                //! application events bit map
}rsi_wlan_app_cb_t;


/*=======================================================================*/
//!	Powersave configurations
/*=======================================================================*/
#define ENABLE_POWER_SAVE  							0		//! Set to 1 for powersave mode
#define	PSP_MODE                                    RSI_SLEEP_MODE_2
#define PSP_TYPE									RSI_MAX_PSP

/*=======================================================================*/
//! Power save command parameters
/*=======================================================================*/
//! set handshake type of power mode
#define RSI_HAND_SHAKE_TYPE                       GPIO_BASED

//! Enable feature
#define RSI_ENABLE                 					1
//! Disable feature
#define RSI_DISABLE               	 				0

/*=======================================================================*/
//! opermode command paramaters
/*=======================================================================*/
//! To set wlan feature select bit map
#define RSI_FEATURE_BIT_MAP          	(FEAT_ULP_GPIO_BASED_HANDSHAKE | FEAT_DEV_TO_HOST_ULP_GPIO_1 | FEAT_SECURITY_OPEN | FEAT_AGGREGATION)

//! TCP IP BYPASS feature check
#define RSI_TCP_IP_BYPASS            	RSI_DISABLE

#define RSI_TCP_IP_FEATURE_BIT_MAP   	(TCP_IP_FEAT_DHCPV4_CLIENT | TCP_IP_FEAT_SSL | TCP_IP_FEAT_DNS_CLIENT | TCP_IP_FEAT_EXTENSION_VALID)

#define RSI_EXT_TCPIP_FEATURE_BITMAP   	(BIT(16) | EXT_DYNAMIC_COEX_MEMORY | EXT_TCP_IP_WINDOW_DIV | EXT_TCP_IP_TOTAL_SELECTS_4 | EXT_TCP_IP_BI_DIR_ACK_UPDATE )

//! Enable CUSTOM_FEAT_SOC_CLK_CONFIG_160MHZ only in Wlan mode
//! To set custom feature select bit map
#if (RSI_ENABLE_BT_TEST || RSI_ENABLE_BLE_TEST)
#define RSI_CUSTOM_FEATURE_BIT_MAP   	FEAT_CUSTOM_FEAT_EXTENTION_VALID
#else
#define RSI_CUSTOM_FEATURE_BIT_MAP   	FEAT_CUSTOM_FEAT_EXTENTION_VALID | CUSTOM_FEAT_SOC_CLK_CONFIG_160MHZ
#endif

//! To set Extended custom feature select bit map
#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP  (/*EXT_FEAT_LOW_POWER_MODE | */ EXT_FEAT_XTAL_CLK_ENABLE | EXT_FEAT_384K_MODE)

/*=======================================================================*/
//! Feature frame parameters
/*=======================================================================*/
#if (RSI_ENABLE_BT_TEST || RSI_ENABLE_BLE_TEST)
#define PLL_MODE              0
#else
#define PLL_MODE              1
#endif

#define RF_TYPE               1 //! 0 - External RF 1- Internal RF
#define WIRELESS_MODE         0
#define ENABLE_PPP            0
#define AFE_TYPE              1
#define FEATURE_ENABLES       0

/*=======================================================================*/
//! High Throughput Capabilies related information
/*=======================================================================*/
//! HT caps supported
//! HT caps bit map.
#define RSI_HT_CAPS_BIT_MAP          ( RSI_HT_CAPS_NUM_RX_STBC           \
		| RSI_HT_CAPS_SHORT_GI_20MHZ        \
		| RSI_HT_CAPS_GREENFIELD_EN         \
		| RSI_HT_CAPS_SUPPORT_CH_WIDTH )
/*=======================================================================*/
//! Rejoin parameters
/*=======================================================================*/
//! RSI_ENABLE or RSI_DISABLE rejoin params
#define RSI_REJOIN_PARAMS_SUPPORT       RSI_ENABLE
//! Rejoin retry count. If 0 retries infinity times
#define RSI_REJOIN_MAX_RETRY           	1
#include <rsi_wlan_common_config.h>
#endif

