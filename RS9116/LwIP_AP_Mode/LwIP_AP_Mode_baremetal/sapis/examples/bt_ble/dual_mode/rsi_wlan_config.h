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

//! Enable feature
#define RSI_ENABLE                 1
//! Disable feature
#define RSI_DISABLE                0

//! To enable concurrent mode
#define CONCURRENT_MODE                      RSI_DISABLE


//! opermode command paramaters
/*=======================================================================*/
//! To set wlan feature select bit map
#define RSI_FEATURE_BIT_MAP          (FEAT_SECURITY_OPEN )

//! TCP IP BYPASS feature check
#define RSI_TCP_IP_BYPASS            RSI_DISABLE

//! TCP/IP feature select bitmap for selecting TCP/IP features    
#define RSI_TCP_IP_FEATURE_BIT_MAP  (TCP_IP_FEAT_DHCPV4_CLIENT | TCP_IP_FEAT_EXTENSION_VALID)

//! To set custom feature select bit map 
#define RSI_CUSTOM_FEATURE_BIT_MAP  FEAT_CUSTOM_FEAT_EXTENTION_VALID

//! To set Extended custom feature select bit map 
#define RSI_EXT_CUSTOM_FEATURE_BIT_MAP   EXT_FEAT_384K_MODE  

#define RSI_EXT_TCPIP_FEATURE_BITMAP   (EXT_DYNAMIC_COEX_MEMORY)
/*=======================================================================*/

#define TCP_RX_WINDOW_DIV_FACTOR          44
//! including the common defines
#include "rsi_wlan_common_config.h"
#endif
