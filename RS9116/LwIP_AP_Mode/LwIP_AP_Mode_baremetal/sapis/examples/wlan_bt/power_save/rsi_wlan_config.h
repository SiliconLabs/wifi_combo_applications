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

#ifndef RSI_CONFIG_H
#define RSI_CONFIG_H

#include "rsi_wlan_defines.h"
/***********************************************************************************************************************************************/
//! WLAN SAPI CONFIG DEFINES
/***********************************************************************************************************************************************/
//! Enable feature
#define RSI_ENABLE                      1
//! Disable feature
#define RSI_DISABLE                     0
//! To enable concurrent mode
#define CONCURRENT_MODE                 RSI_DISABLE

//! BT power control
#define BT_PWR_CTRL_DISABLE             0
#define BT_PWR_CTRL                     1
#define BT_PWR_INX                      10

//! Enable/Disable Powersave
#define ENABLE_POWER_SAVE               1
#define PSP_TYPE                        RSI_MAX_PSP

/*=======================================================================*/
//! Opermode command parameters
/*=======================================================================*/

#define RSI_FEATURE_BIT_MAP          	(FEAT_ULP_GPIO_BASED_HANDSHAKE | FEAT_DEV_TO_HOST_ULP_GPIO_1)                //! To set wlan feature select bit map
#define RSI_TCP_IP_BYPASS            	RSI_DISABLE                                                                  //! TCP IP BYPASS feature check
#define RSI_TCP_IP_FEATURE_BIT_MAP   	(TCP_IP_FEAT_DHCPV4_CLIENT)                                                  //! TCP/IP feature select bitmap for selecting TCP/IP features
#define RSI_EXT_TCPIP_FEATURE_BITMAP    0

#define RSI_CUSTOM_FEATURE_BIT_MAP   	FEAT_CUSTOM_FEAT_EXTENTION_VALID                                             //! To set custom feature select bit map

#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP  (EXT_FEAT_LOW_POWER_MODE | EXT_FEAT_XTAL_CLK_ENABLE | EXT_FEAT_384K_MODE)


/*=======================================================================*/
//! Band command parameters
/*=======================================================================*/
//! RSI_BAND_2P4GHZ(2.4GHz) or RSI_BAND_5GHZ(5GHz) or RSI_DUAL_BAND
#define RSI_BAND                         RSI_BAND_2P4GHZ //! RSI_BAND_5GHZ or RSI_BAND_2P4GHZ

/*=======================================================================*/
//! Power save command parameters
/*=======================================================================*/
//! set handshake type of power mode
#define RSI_HAND_SHAKE_TYPE              GPIO_BASED

//! 0 - LP, 1- ULP mode with RAM retention and 2 - ULP with Non RAM retention
#define RSI_SELECT_LP_OR_ULP_MODE        RSI_ULP_WITH_RAM_RET


//! set DTIM aligment required
//! 0 - module wakes up at beacon which is just before or equal to listen_interval
//! 1 - module wakes up at DTIM beacon which is just before or equal to listen_interval
#define RSI_DTIM_ALIGNED_TYPE            0

//! Monitor interval for the FAST PSP mode
//! default is 50 ms, and this parameter is valid for FAST PSP only
#define RSI_MONITOR_INTERVAL             50
/***********************************************************************************************************************************************/

//! Number of DTIMs to skip during powersave
#define RSI_NUM_OF_DTIM_SKIP             0

#define TCP_RX_WINDOW_DIV_FACTOR          44
//! including the common defines
#include "rsi_wlan_common_config.h"

#endif

