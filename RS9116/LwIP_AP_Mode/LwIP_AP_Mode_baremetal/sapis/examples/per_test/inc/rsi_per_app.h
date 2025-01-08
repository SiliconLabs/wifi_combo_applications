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

/**
 * Includes
 */
#ifndef RSI_WLAN_PER_H
#define RSI_WLAN_PER_H 

#ifdef __cplusplus
extern "C" {
#endif


#include "rsi_bt_common.h"
#include "rsi_bt_common_apis.h"
//! include file to refer data types
#include "rsi_data_types.h"
//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"
#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"
//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_driver.h"
#include "rsi_per_serial.h"

//wlan per stats structure
typedef struct rsi_per_stats_rsp_s{
uint8_t tx_pkts[2];
uint8_t reserved_1[2];
uint8_t tx_retries[2];
uint16_t crc_pass;
uint16_t crc_fail;
uint8_t cca_stk[2];
uint8_t cca_not_stk[2];
uint8_t pkt_abort[2];
uint8_t fls_rx_start[2];
uint8_t cca_idle[2];
uint8_t reserved_2[26];
uint8_t rx_retries[2];
uint8_t reserved_3[2];
uint16_t cal_rssi;
uint8_t reserved_4[4];
uint8_t xretries[2];
uint8_t max_cons_pkts_dropped[2];
uint8_t reserved_5[2];
uint8_t bss_broadcast_pkts[2];
uint8_t bss_multicast_pkts[2];
uint8_t bss_filter_matched_multicast_pkts[2];
}rsi_per_stats_rsp_t;

int32_t rsi_send_per_features(void);
int32_t rsi_wlan_per(uint8_t test_state);
int32_t rsi_send_per_features(void);
int32_t rsi_common_command(void);
int32_t rsi_wlan_per_tx_start(void);
int32_t rsi_wlan_per_tx_stop(void);
int32_t rsi_receive_start(void);
int32_t rsi_receive_stop(void);
int32_t rsi_ble_transmit_start(void);
int32_t rsi_ble_transmit_stop(void);
int32_t rsi_ble_receive_start(void);
int32_t rsi_ble_receive_stop(void);
void rsi_ble_stats_receive(void);
int32_t rsi_bt_receive_start(void);
int32_t rsi_bt_receive_stop(void);
int32_t rsi_bt_transmit_start(void);
int32_t rsi_bt_transmit_stop(void);
void rsi_bt_stats_receive(void);

#ifdef __cplusplus
}
#endif

#endif
