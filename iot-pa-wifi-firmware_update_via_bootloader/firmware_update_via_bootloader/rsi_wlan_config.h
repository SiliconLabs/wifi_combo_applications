/**
 * @file         rsi_wlan_config_DEMO_22.h
 * @version      0.1
 * @date         15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains user configurable details to configure the device
 *
 *  @section Description  This file contains user configurable details to configure the device
 *
 *
 */

#include "stdint.h"
#include "stdio.h"

//! To enable or disable the fast firmware upgradation feature
#define RSI_FAST_FW_UP                    1

/*=======================================================================*/
//! Opermode command parameters
/*=======================================================================*/
//! To set WLAN feature select bit map
#define RSI_FEATURE_BIT_MAP           FEAT_SECURITY_OPEN

//! TCP IP BYPASS feature check
#define RSI_TCP_IP_BYPASS             RSI_DISABLE

//! TCP/IP feature select bitmap for selecting TCP/IP features
#define RSI_TCP_IP_FEATURE_BIT_MAP    (TCP_IP_FEAT_DHCPV4_CLIENT)

#define RSI_EXT_TCPIP_FEATURE_BITMAP  0
#define RSI_BT_FEATURE_BITMAP         0

 //! To set custom feature select bit map
#define RSI_CUSTOM_FEATURE_BIT_MAP    0



