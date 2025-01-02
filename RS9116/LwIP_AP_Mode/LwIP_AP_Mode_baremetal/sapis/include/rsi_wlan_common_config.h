/**
 * @file         rsi_wlan_common_config.h
 * @version      0.1
 * @date         15 Aug 2018
 *
 *  Copyright(C) Redpine Signals 2018
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
#ifndef RSI_COMMON_CONFIG_H
#define RSI_COMMON_CONFIG_H

#include "rsi_wlan_defines.h"

//! Enable feature
#define RSI_ENABLE                 1
//! Disable feature
#define RSI_DISABLE                0

//! buffer allocation parameters
/*=======================================================================*/
#define TX_RX_RATIO_ENABLE          RSI_DISABLE

#define TX_POOL_RATIO                  1 
#define RX_POOL_RATIO                  1 
#define GLOBAL_POOL_RATIO              1 
/*=======================================================================*/

#endif


//! Unblock call
/*=======================================================================*/
#define BUFFER_FULL_HANDLING             RSI_DISABLE
/*=======================================================================*/
