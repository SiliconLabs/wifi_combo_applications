/**
 * @file       rsi_ble.h
 * @version    0.1
 * @date       03 Sep 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief :   This file contains the BLE functionality related. 
 *
 *  @section Description  This file contains all the BLE structures. 
 *
 */

#ifndef RSI_ANT_BLE_H
#define RSI_ANT_BLE_H

#include<rsi_data_types.h>
#include<rsi_bt_common.h>
#include <rsi_ble_apis.h>

//! enumeration for ANT command request codes
typedef enum rsi_ant_cmd_request_e
{
  ANT_ATM_CMD                           = 0x0001,
  ANT_GET_VERSION                       = 0x0002,
  ANT_GET_STATS                         = 0x0003,
  ANT_GET_ACTIVITY_STATS                = 0x0004,
}rsi_ant_cmd_request_t;


//! enumeration for ANT command response codes
typedef enum rsi_ant_cmd_resp_e
{
  ANT_ATM_CMD_RESP                     = 0x0001,
  ANT_GET_VERSION_RESP                 = 0x0002,
  ANT_GET_STATS_RESP                   = 0x0003,
  ANT_GET_ACTIVITY_STATS_RESP          = 0x0004,
}rsi_ant_cmd_resp_t;



//! enumeration for ANT Asyncronous Events
typedef enum rsi_ant_event_e
{
  RSI_ANT_SCHED_STATS                     = 0x1530,
  RSI_ANT_ACTIVITY_STATS                  = 0x1531,
}rsi_ant_event_t;

/********************************************************
 * *                 Structure Definitions
 * ******************************************************/

//Ant Atm cmd mode structure
typedef struct rsi_ant_atm_mode_s {
  uint8_t cmd_ix;
  uint8_t flags;
}rsi_ant_atm_mode_t;

//Ant get versions cmd structure
typedef struct rsi_ant_get_ver_s {
  uint8_t cmd_ix;
}rsi_ant_get_ver_t;

//Ant get stats cmd structure
typedef struct rsi_ant_get_stats_s {
  uint8_t cmd_ix;
}rsi_ant_get_stats_t;

//Ant get versions resp structure
typedef struct rsi_ant_get_ver_resp_s {
   uint8_t   ant_version[6];
}rsi_ant_get_ver_resp_t;

//Ant get versions resp structure
typedef struct rsi_ant_get_stats_resp_s {
   uint8_t   ant_scheduling_stats[10];
}rsi_ant_get_stats_resp_t;

//Ant get versions resp structure
typedef struct rsi_ant_get_activity_stats_resp_s {
   uint8_t   ant_activity_stats[14];
}rsi_ant_get_activity_stats_resp_t;

//Ant activity stats
typedef struct rsi_ant_activity_stats_s {
  uint16_t reserved;
  uint16_t activity_status_ok;
  uint16_t activities_aborted; 
  uint16_t activity_status_too_late;
  uint16_t activity_status_no_sync;
  uint16_t activity_status_incomplete;
  uint16_t activity_status_bad_crc;
  uint16_t activity_status_vnd_error_start;
}rsi_ant_activity_stats_t;

//Ant scheduling stats
typedef struct rsi_ant_schedule_stats_s {
  uint16_t reserved;
  uint16_t activities_blocked;
  uint16_t activities_aborted;
  uint16_t schedules_blocked;
  uint16_t radio_ops_complete;
  uint16_t schedules_attempted; 
}rsi_ant_schedule_stats_t;

#endif
