
/**
 * @file       rsi_bt.h
 * @version    0.1
 * @date       23 Sep 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief :   This file contains the Bt functionality related. 
 *
 *  @section Description  This file contains all the BT structures. 
 *
 */

#ifndef RSI_BT_HID_H
#define RSI_BT_HID_H

#include<rsi_data_types.h>
#include<rsi_driver.h>
#include<rsi_bt_common.h>
#include<rsi_bt_apis.h>

/* HID Profile MACROS*/
#define HID_MSG_HANDSHAKE_LEN  0x1
#define HID_MSG_CONTROL_LEN    0x1
#define HID_MSG_GET_PROTO_LEN  0x1
#define HID_MSG_SET_PROTO_LEN  0x1
#define HID_MSG_GET_REPORT_LEN 0x1
#define HID_MSG_SET_REPORT_LEN 0x1


#define HID_MSG_HANDSHAKE  0x0
#define HID_MSG_CONTROL    0x1
#define HID_MSG_GET_REPORT 0x4
#define HID_MSG_SET_REPORT 0x5
#define HID_MSG_GET_PROTO  0x6
#define HID_MSG_SET_PROTO  0x7
#define HID_MSG_DATA       0xA

#define HID_REPORT_TYPE_RESERVED   0x00
#define HID_REPORT_TYPE_INPUT      0x01
#define HID_REPORT_TYPE_OUTPUT     0x02
#define HID_REPORT_TYPE_FEATURE    0x03

#define HID_MSG_HANDSHAKE_SUCCESSFUL       0x00
#define HID_MSG_HANDSHAKE_NOT_READY        0x01
#define HID_MSG_HANDSHAKE_ERR_INV_REP_ID   0x02
#define HID_MSG_HANDSHAKE_ERR_UNSUPP_REQ   0x03
#define HID_MSG_HANDSHAKE_ERR_INV_PARAM    0x04
#define HID_MSG_HANDSHAKE_ERR_UNKNOWN      0x0E
#define HID_MSG_HANDSHAKE_ERR_FATAL        0x0F

#define HID_MSG_CONTROL_SUSPEND              0x03
#define HID_MSG_CONTROL_EXIT_SUSPEND         0x04
#define HID_MSG_CONTROL_VIRT_CABLE_UNPLUG    0x05

#define HID_MSG_PROTOCOL_BOOT     0x00 
#define HID_MSG_PROTOCOL_REPORT   0x01

#define HID_CHANNEL_CONTROL     0x00
#define HID_CHANNEL_INTERRUPT   0x01

#define RSI_HID_DEVICE_VERSION_NUM   0x0100
#define RSI_PARSER_VERSION           0x0111
#define RSI_HID_DEV_SUBCLASS         0x0040
#define RSI_HID_COUNTRY_CODE         0x0000
#define RSI_VIRTUAL_CABLE            0x0001
#define RSI_RECONNECT_INITIATE       0x0001
#define RSI_HID_LANG_BASE            0x0100
#define RSI_SDP_DISBALE              0x0000
#define RSI_BATTERY_POWER            0x0001
#define RSI_REMOTE_WAKE              0x0001
#define RSI_PROFILE_VER              0x0100
#define RSI_SUPERVISION_TIMEOUT      0x0C80
#define RSI_NORMALLY_CONNECTABLE     0x0000
#define RSI_BOOT_DEVICE              0x0001
#define RSI_SSR_HOST_MIN_LATENCY     0x0320
#define RSI_SSR_HOST_MAX_LATENCY     0x0640
#define RSI_HID_LANG_ID              0x0409
#endif

