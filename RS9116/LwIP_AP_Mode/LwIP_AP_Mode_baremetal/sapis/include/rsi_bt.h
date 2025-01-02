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

#ifndef RSI_BT_H1
#define RSI_BT_H1

#include<rsi_data_types.h>
#include<rsi_driver.h>
#include<rsi_bt_common.h>
#include<rsi_bt_apis.h>

/******************************************************
 * *                      Macros
 * ******************************************************/
#define RSI_MAX_PINCODE_REPLY_SIZE                  17
#define CHANNEL_MAP_LEN                             10
#define BT_VENDOR_PTT_TYPE_CMD_OPCODE           0xFC17
#define BT_VENDOR_AVDTP_STATS_CMD_OPCODE           0xFC18

/******************************************************
 * *                BT classic AFH defines
 * ******************************************************/
#define RSI_WRITE_ASSESSMENT_MODE                   0x01
#define RSI_READ_ASSESSMENT_MODE                    0x00
#define RSI_ASSESSMENT_MODE_DISABLE                 0x00
#define RSI_ASSESSMENT_MODE_ENABLE                  0x01 

/******************************************************
 * *                    Constants
 * ******************************************************/

/******************************************************
 * *                 Type Definitions
 * ******************************************************/

/******************************************************
 * *                   Enumerations
 * ******************************************************/

#define RSI_BT_MAX_ACCESSORY_INFO_LEN 70
#define RSI_BT_IAP_MAX_DATA_LEN  300
//! enumeration for BT  command request codes
typedef enum rsi_bt_cmd_request_e
{
  RSI_BT_REQ_SET_LOCAL_COD                       = 0x0003, 
  RSI_BT_REQ_QUERY_LOCAL_COD                     = 0x0004,
  RSI_BT_REQ_SET_PROFILE_MODE                    = 0x0008,
  RSI_BT_REQ_SET_DISCV_MODE                      = 0x0009,
  RSI_BT_REQ_QUERY_DISCV_MODE                    = 0x000A,
  RSI_BT_REQ_SET_CONNECTABILITY_MODE             = 0x000B,
  RSI_BT_REQ_QUERY_CONNECTABILITY_MODE           = 0x000C,
  RSI_BT_REQ_SET_PAIR_MODE                       = 0x000D,
  RSI_BT_REQ_QUERY_PAIR_MODE                     = 0x000E,
  RSI_BT_REQ_REMOTE_NAME_REQUEST                 = 0x000F,
  RSI_BT_REQ_REMOTE_NAME_REQUEST_CANCEL          = 0x0010,
  RSI_BT_REQ_INQUIRY                             = 0x0011,
  RSI_BT_REQ_INQUIRY_CANCEL                      = 0x0012,
  RSI_BT_REQ_BOND                                = 0x0013,
  RSI_BT_REQ_BOND_CANCEL                         = 0x0014,
  RSI_BT_REQ_UNBOND                              = 0x0015,
  RSI_BT_REQ_USER_CONFIRMATION                   = 0x0018,
  RSI_BT_REQ_PASSKEY_REPLY                       = 0x0019,
  RSI_BT_REQ_PINCODE_REPLY                       = 0x001A,
  RSI_BT_REQ_QUERY_ROLE                          = 0x001B,
  RSI_BT_REQ_SET_ROLE                            = 0x001C,
  RSI_BT_REQ_QUERY_SERVICES                      = 0x001D,
  RSI_BT_REQ_SEARCH_SERVICE                      = 0x001E,
  RSI_BT_REQ_SPP_CONNECT                         = 0x001F,
  RSI_BT_REQ_SPP_DISCONNECT                      = 0x0020,
  RSI_BT_REQ_SPP_TRANSFER                        = 0x0021,
  
  RSI_BT_REQ_A2DP_CONNECT                        = 0x0022,
  RSI_BT_REQ_A2DP_DISCONNECT                     = 0x0023,
  
  RSI_BT_REQ_AVRCP_CONNECT                       = 0X0024,
  RSI_BT_REQ_AVRCP_DISCONNECT                    = 0X0025,
  RSI_BT_REQ_AVRCP_PLAY                          = 0X0026,
  RSI_BT_REQ_AVRCP_PAUSE                         = 0X0027,
  RSI_BT_REQ_AVRCP_STOP                          = 0X0028,
  RSI_BT_REQ_AVRCP_NEXT                          = 0X0029,
  RSI_BT_REQ_AVRCP_PREVIOUS                      = 0X002A,
  
  RSI_BT_REQ_AVRCP_VOL_UP                        = 0X002B,
  RSI_BT_REQ_AVRCP_VOL_DOWN                      = 0X002C,
  
  RSI_BT_REQ_HFP_CONNECT                         = 0x002D,
  RSI_BT_REQ_HFP_DISCONNECT                      = 0x002E,
  RSI_BT_REQ_HFP_PHONEOPERATOR                   = 0x002F,
  RSI_BT_REQ_HFP_CALLACCEPT                      = 0x0030,
  RSI_BT_REQ_HFP_CALLREJECT                      = 0x0031, 
  RSI_BT_REQ_HFP_DIALNUM                         = 0x0032,
  RSI_BT_REQ_HFP_DIALMEM                         = 0x0033,
  RSI_BT_REQ_HFP_REDIAL                          = 0x0034,
  RSI_BT_REQ_HFP_VOICERECOGNITIONACTIVE          = 0x0035,
  RSI_BT_REQ_HFP_VOICERECOGNITIONDEACTIVE        = 0x0036,
  RSI_BT_REQ_HFP_SPKGAIN                         = 0x0037,
  RSI_BT_REQ_HFP_MICGAIN                         = 0x0038,
  RSI_BT_REQ_HFP_GETCALLS                        = 0x0039,
  RSI_BT_REQ_HFP_AUDIO                           = 0x003A,
  
  
  RSI_BT_REQ_PBAP_CONNECT                        = 0x003B,
  RSI_BT_REQ_PBAP_DISCONNECT                     = 0x003C,
  RSI_BT_REQ_PBAP_CONTACTS                       = 0x003D,

  RSI_BT_REQ_HID_CONNECT                         = 0x003E,
  RSI_BT_REQ_HID_DISCONNECT                      = 0x003F,
  RSI_BT_REQ_HID_PROFILE_DATA                    = 0x0040,
  RSI_BT_REQ_HID_SDP_ATT_INIT                    = 0x0041,

  RSI_BT_REQ_LINKKEY_REPLY                       = 0x0091,
  RSI_BT_REQ_PER_TX                              = 0x0098,
  RSI_BT_REQ_PER_RX                              = 0x0099,
  RSI_BT_REQ_CW_MODE                             = 0x009B,
  RSI_BT_REQ_SNIFF_MODE                          = 0x009D,
  RSI_BT_REQ_SNIFF_EXIT_MODE                     = 0x009E,
  RSI_BT_REQ_SNIFF_SUBRATING_MODE                = 0x009F,
  RSI_BT_REQ_SET_SSP_MODE                        = 0x00A0,
  RSI_BT_REQ_SET_EIR                             = 0X00A9,

 
  RSI_BT_REQ_A2DP_CLOSE                          = 0x00CC,
  RSI_BT_REQ_A2DP_ABORT                          = 0x00CD,
  RSI_BT_REQ_A2DP_START                          = 0x00CE,
  RSI_BT_REQ_A2DP_SUSPEND                        = 0x00CF,
  RSI_BT_REQ_A2DP_PCM_MP3_DATA                   = 0x00D0,
  RSI_BT_REQ_A2DP_SBC_AAC_DATA                   = 0x00D1,
  
  RSI_BT_REQ_SET_AFH_HOST_CHANNEL_CLASSIFICATION = 0x00D2,
  
  //AVRCP 1.3 controller role commands
  RSI_BT_REQ_AVRCP_GET_CAPABILITES               = 0X00D3,
  RSI_BT_REQ_AVRCP_GET_ATTS_LIST                 = 0X00D4,
  RSI_BT_REQ_AVRCP_GET_ATT_VALS_LIST             = 0X00D5,
  RSI_BT_REQ_AVRCP_GET_CUR_ATT_VAL               = 0X00D6,
  RSI_BT_REQ_AVRCP_SET_CUR_ATT_VAL               = 0X00D7,
  RSI_BT_REQ_AVRCP_GET_ELEMENT_ATT               = 0X00D8,
  RSI_BT_REQ_AVRCP_GET_PLAY_STATUS               = 0X00D9,
  RSI_BT_REQ_AVRCP_REG_NOTIFICATION              = 0X00DA,
  RSI_BT_REQ_AVRCP_REMOTE_VERSION                = 0X00DB,
  RSI_BT_REQ_AVRCP_ATT_TEXT                      = 0X00DC,
  RSI_BT_REQ_AVRCP_ATT_VALS_TEXT                 = 0X00DD,
  RSI_BT_REQ_AVRCP_BATTERY_STATUS                = 0X00DE,
  RSI_BT_REQ_AVRCP_CHAR_SETS                     = 0X00DF,

  //AVRCP target role commands
  RSI_BT_REQ_AVRCP_GET_CAPABILITES_RESP          = 0X00E0,
  RSI_BT_REQ_AVRCP_GET_ATTS_LIST_RESP            = 0X00E1,
  RSI_BT_REQ_AVRCP_GET_ATT_VALS_LIST_RESP        = 0X00E2,
  RSI_BT_REQ_AVRCP_GET_CUR_ATT_VAL_RESP          = 0X00E3,
  RSI_BT_REQ_AVRCP_SET_CUR_ATT_VAL_RESP          = 0X00E4,
  RSI_BT_REQ_AVRCP_GET_ELEMENT_ATT_RESP          = 0X00E5,
  RSI_BT_REQ_AVRCP_GET_PLAY_STATUS_RESP          = 0X00E6,
  RSI_BT_REQ_AVRCP_REG_NOTIFICATION_RESP         = 0X00E7,
  RSI_BT_REQ_AVRCP_ATT_TEXT_RESP                 = 0X00E8,
  RSI_BT_REQ_AVRCP_ATT_VALS_TEXT_RESP            = 0X00E9,
  RSI_BT_REQ_AVRCP_BATTERY_STATUS_RESP           = 0X00EA,
  RSI_BT_REQ_AVRCP_CHAR_SETS_RESP                = 0X00EB,
  RSI_BT_REQ_AVRCP_NOTIFICATION                  = 0X00EC,
  RSI_BT_REQ_AVRCP_CMD_REJECT                    = 0X00ED,
 
  RSI_BT_REQ_ADD_DEVICE_ID                       = 0x00EE,
  RSI_BT_REQ_SET_CURRENT_IAC_LAP                 = 0x00EF,
	
  //Testing Commands
	RSI_BT_REQ_ENABLE_DEVICE_UNDER_TESTMODE        = 0x00F0,

  RSI_BT_REQ_A2DP_GET_CONFIG                     = 0x00FE,
  RSI_BT_REQ_A2DP_SET_CONFIG                     = 0x00FF,

  RSI_BT_REQ_AVRCP_SET_ABS_VOL                   = 0x0102,
  RSI_BT_REQ_AVRCP_SET_ABS_VOL_RESP              = 0X0103,
  RSI_BT_REQ_GATT_CONNECT                        = 0x0104,
    RSI_BT_REQ_IAP_CONN                                 =  0x0105, 
  RSI_BT_REQ_IAP_DISCONN                              =  0x0106, 
  RSI_BT_REQ_IAP_SET_ACCESSORY_INFO                   =  0x0107, 
  RSI_BT_REQ_IAP_SET_APP_PROTOCOL_INFO                =  0x0108,
  RSI_BT_REQ_IAP_FIND_PROTOCOL_TYPE                   =  0x0109,
  RSI_BT_REQ_IAP_SET_PROTOCOL_TYPE                    =  0X010A,
  RSI_BT_REQ_IAP1_IDENTIFICATION                     =  0X010B,
  RSI_BT_REQ_IAP1_DEVICE_AUTHENTICATION              =  0X010C,
  RSI_BT_REQ_IAP1_SET_ASSISTIVE_TOUCH                =  0X010D,
  RSI_BT_REQ_IAP1_SET_VOICE_OVER                     =  0X010E,
  RSI_BT_REQ_IAP1_GET_IPOD_INFO                      =  0X010F,   
  RSI_BT_REQ_IAP1_SET_EXTENDED_INTFMODE              =  0X0110,
  RSI_BT_REQ_IAP1_GET_LINGO_PROTOCOL_VERSION         =  0X0111,
  RSI_BT_REQ_IAP1_SET_IPOD_PREFERENCES               =  0X0112,
  RSI_BT_REQ_IAP1_GET_IPOD_PREFERENCES               =  0X0113,
  RSI_BT_REQ_IAP1_SET_UIMODE                         =  0X0114,
  RSI_BT_REQ_IAP1_GET_UIMODE                         =  0X0115,
  RSI_BT_REQ_IAP1_SET_EVENT_NOTIFICATION             =  0X0116,
  RSI_BT_REQ_IAP1_GET_EVENT_NOTIFICATION             =  0X0117,
  RSI_BT_REQ_IAP1_GET_IPOD_LINGO_OPTIONS             =  0X0118,
  RSI_BT_REQ_IAP1_SUPP_EVENT_NOTIFICATIONS           =  0X0119,
  RSI_BT_REQ_IAP1_LAUNCH_APPLICATION                 =  0X011A,
  RSI_BT_REQ_IAP1_GET_CURRENT_APP_BUNDLE_NAME        =  0X011B,
  RSI_BT_REQ_IAP1_GET_LOCALIZATION_INFO              =  0X011C,
  RSI_BT_REQ_IAP1_REQ_WIFI_CONN_INFO                 =  0X011D,
  RSI_BT_REQ_IAP1_APP_DATA_SESSION_ACK               =  0X011E,
  RSI_BT_REQ_IAP1_APP_ACCESSORY_DATA_TRANSFER        =  0X011F,
  RSI_BT_REQ_IAP1_GET_VOICE_OVER_PARAMETER           =  0X0120,
  RSI_BT_REQ_IAP1_SET_VOICE_OVER_PARAMETER           =  0X0121,
  RSI_BT_REQ_IAP1_SET_VOICE_OVER_CONTEXT             =  0X0122,
  RSI_BT_REQ_IAP1_VOICE_OVER_EVENT                   =  0X0123,
  RSI_BT_REQ_IAP1_VOICE_OVER_TEXT_EVENT              =  0X0124,
  RSI_BT_REQ_IAP1_VOICE_OVER_TOUCH_EVENT             =  0X0125,
  RSI_BT_REQ_IAP1_VOICE_OVER_XYZ                     =  0X0126,
  RSI_BT_REQ_IAP1_CURR_VOICE_OVER_ITEM_PROPERTY      =  0X0127,
  RSI_BT_REQ_IAP1_CURR_VOICE_OVER_LABEL              =  0X0128,
  RSI_BT_REQ_IAP1_CURR_VOICE_OVER_VALUE              =  0X0129,
  RSI_BT_REQ_IAP1_CURR_VOICE_OVER_HINT               =  0X012A,       
  RSI_BT_REQ_IAP1_CURR_VOICE_OVER_TRAIT              =  0X012B,
  RSI_BT_REQ_IAP1_IPOD_OUT_BUTTON                    =  0X012C,
  RSI_BT_REQ_IAP1_VIDEO_BUTTON                       =  0X012D,
  RSI_BT_REQ_IAP1_AUDIO_BUTTON                       =  0X012E,
  RSI_BT_REQ_IAP1_CONTEXT_BUTTON                     =  0X012F,
  RSI_BT_REQ_IAP1_RADIO_BUTTON                       =  0X0130,
  RSI_BT_REQ_IAP1_CAMERA_BUTTON                      =  0X0131,
  RSI_BT_REQ_IAP1_ROTATION_INPUT                     =  0X0132,
  RSI_BT_REQ_IAP1_REG_HID_DESCRIPTOR                 =  0X0133,
  RSI_BT_REQ_IAP1_SEND_HID_REPORT                    =  0X0134,
  RSI_BT_REQ_IAP1_UNREG_HID_DESCRIPTOR               =  0X0135,
  RSI_BT_REQ_IAP2_IDENTIFICATION                     =  0X0136,
  RSI_BT_REQ_IAP2_IDENTIFICATION_CANCEL              =  0X0137,
  RSI_BT_REQ_IAP2_UPDATE_ACCESSORY_INFO              =  0X0138,
  RSI_BT_REQ_IAP2_DEVICE_AUTHENTICATION              =  0X0139,
  RSI_BT_REQ_IAP2_NOWPLAYING_UPDATES_START           =  0X013A,
  RSI_BT_REQ_IAP2_NOWPLAYING_UPDATES_STOP            =  0X013B,
  RSI_BT_REQ_IAP2_NOW_PLAYING_ARTWORK_FILE_ACTION    =  0X013C,
  RSI_BT_REQ_IAP2_CONTROL_SESSION_DATA_TX            =  0X013D,
  RSI_BT_REQ_IAP2_RECV_CMD_LIST                      =  0X013E,
	RSI_BT_REQ_IAP2_SEND_CMD_LIST                      =  0X013F,
	RSI_BT_REQ_IAP2_INIT_FILE_TRANSFER                 =  0X0140,
	RSI_BT_REQ_IAP2_DE_INIT_FILE_TRANSFER              =  0X0141,
	RSI_BT_REQ_IAP2_SEND_FILE_TRANSFER_STATE           =  0X0142,
  RSI_BT_REQ_IAP2_SEND_FILE_TRANSFER_DATA            =  0X0143
}rsi_bt_cmd_request_t;

typedef enum rsi_bt_cmd_resp_e
{
  RSI_BT_RSP_CARD_READY                          = 0x0505,
  RSI_BT_RSP_SET_LOCAL_COD                       = 0x0003,
  RSI_BT_RSP_QUERY_LOCAL_COD                     = 0x0004,
  RSI_BT_RSP_LINKKEY_REPLY                       = 0x0091,
  RSI_BT_RSP_SET_PROFILE_MODE                    = 0x0008,
  RSI_BT_RSP_SET_DISCV_MODE                      = 0x0009,
  RSI_BT_RSP_QUERY_DISCV_MODE                    = 0x000A,
  RSI_BT_RSP_SET_CONNECTABILITY_MODE             = 0x000B,
  RSI_BT_RSP_QUERY_CONNECTABILITY_MODE           = 0x000C,
  RSI_BT_RSP_SET_PAIR_MODE                       = 0x000D,
  RSI_BT_RSP_QUERY_PAIR_MODE                     = 0x000E,
  RSI_BT_RSP_REMOTE_NAME_REQUEST                 = 0x000F,
  RSI_BT_RSP_REMOTE_NAME_REQUEST_CANCEL          = 0x0010,
  RSI_BT_RSP_INQUIRY                             = 0x0011,
  RSI_BT_RSP_INQUIRY_CANCEL                      = 0x0012,
  RSI_BT_RSP_BOND                                = 0x0013,
  RSI_BT_RSP_BOND_CANCEL                         = 0x0014,
  RSI_BT_RSP_UNBOND                              = 0x0015,
  RSI_BT_RSP_USER_CONFIRMATION                   = 0x0018,
  RSI_BT_RSP_PASSKEY_REPLY                       = 0x0019,
  RSI_BT_RSP_PINCODE_REPLY                       = 0x001A,
  RSI_BT_RSP_QUERY_ROLE                          = 0x001B,
  RSI_BT_RSP_QUERY_SERVICES                      = 0x001D,
  RSI_BT_RSP_SEARCH_SERVICE                      = 0x001E,
  RSI_BT_RSP_SPP_CONNECT                         = 0x001F,
  RSI_BT_RSP_SPP_DISCONNECT                      = 0x0020,
  RSI_BT_RSP_SPP_TRANSFER                        = 0x0021,
  
  /*pbap responses */
  RSI_BT_RSP_PBAP_CONNECT                        = 0x003B,
  RSI_BT_RSP_PBAP_DISCONNEC                      = 0x003C,
  RSI_BT_RSP_PBAP_CONTACTS                       = 0x003D, 
  
  RSI_BT_RSP_PER_TX                              = 0x0098,
  RSI_BT_RSP_PER_RX                              = 0x0099,
  RSI_BT_RSP_PER_STATS                           = 0x009A,
  RSI_BT_RSP_CW_MODE                             = 0x009B,

	/* sniff responses */
  RSI_BT_RSP_SNIFF_MODE                          = 0x009D,
  RSI_BT_RSP_SNIFF_EXIT_MODE                     = 0x009E,
  RSI_BT_RSP_SNIFF_SUBRATING_MODE                = 0x009F,
  RSI_BT_RSP_SET_SSP_MODE                        = 0x00A0,
  RSI_BT_RSP_SET_EIR                             = 0X00A9,	
 

  //AVRCP resp id's
  RSI_BT_RSP_AVRCP_GET_CAPABILITES               = 0X00D3,
  RSI_BT_RSP_AVRCP_GET_ATTS_LIST                 = 0X00D4,
  RSI_BT_RSP_AVRCP_GET_ATT_VALS_LIST             = 0X00D5,
  RSI_BT_RSP_AVRCP_GET_CUR_ATT_VAL               = 0X00D6,
  RSI_BT_RSP_AVRCP_SET_CUR_ATT_VAL               = 0X00D7,
  RSI_BT_RSP_AVRCP_GET_ELEMENT_ATT               = 0X00D8,
  RSI_BT_RSP_AVRCP_GET_PLAY_STATUS               = 0X00D9,
  RSI_BT_RSP_AVRCP_REG_NOTIFICATION              = 0X00DA,
  RSI_BT_RSP_AVRCP_REMOTE_VERSION                = 0X00DB,
  RSI_BT_RSP_AVRCP_ATT_TEXT                      = 0X00DC,
  RSI_BT_RSP_AVRCP_ATT_VALS_TEXT                 = 0X00DD,
  RSI_BT_RSP_AVRCP_BATTERY_STATUS                = 0X00DE,
  RSI_BT_RSP_AVRCP_CHAR_SETS                     = 0X00DF,
  
  RSI_BT_RESP_ADD_DEVICE_ID                       = 0x00EE,
  RSI_BT_RESP_SET_CURRENT_IAC_LAP                 = 0x00EF,
  
  RSI_BT_RESP_AVRCP_SET_ABS_VOL                   = 0x0102,
  RSI_BT_RESP_AVRCP_SET_ABS_VOL_RESP              = 0X0103,
  RSI_BT_RESP_GATT_CONNECT                        = 0x0104,

}rsi_bt_cmd_resp_t;

//! Event Frame Codes
typedef enum rsi_bt_event_e
{
  RSI_BT_EVT_ROLE_CHANGE_STATUS                  = 0x1000,
  RSI_BT_EVT_UNBOND_STATUS                       = 0x1001,
  RSI_BT_EVT_BOND_RESPONSE                       = 0x1002,
  RSI_BT_EVT_INQUIRY_RESPONSE                    = 0x1003,
  RSI_BT_EVT_REMOTE_DEVICE_NAME                  = 0x1004,
  RSI_BT_EVT_REMOTE_NAME_REQUEST_CANCEL          = 0x1005,
  RSI_BT_EVT_DISCONNECTED                        = 0x1006,
  RSI_BT_EVT_USER_CONFIRMATION_REQUEST           = 0x1007,
  RSI_BT_EVT_USER_PASKEY_DISPLAY                 = 0x1008,
  RSI_BT_EVT_USER_PINCODE_REQUEST                = 0x1009,
  RSI_BT_EVT_USER_PASSKEY_REQUEST                = 0x100A,
  RSI_BT_EVT_INQ_COMPLETE                        = 0x100B,
  RSI_BT_EVT_AUTH_COMPLETE                       = 0x100C,
  RSI_BT_EVT_USER_LINKKEY_REQUEST                = 0x100D,
  RSI_BT_EVT_USER_LINKKEY_SAVE                   = 0x100E,
  RSI_BT_EVT_SSP_COMPLETE 	                     = 0x100F,

  RSI_BT_EVT_MODE_CHANGED                        = 0x1010,
  RSI_BT_EVT_SNIFF_SUBRATING_CHANGED             = 0x1011,

  RSI_BT_EVT_USER_PASSKEY_NOTIFY                 = 0x1012,
  RSI_BT_EVT_GATT_CONNECTED                      = 0x1013,
  RSI_BT_EVT_GATT_DISCONNECTED                   = 0x1014,
  /* AVDTP Stats Event */
  RSI_BT_EVT_AVDTP_STATS                         = 0x1015,

  /*SPP profile Event ID's*/
  RSI_BT_EVT_SPP_RECEIVE                         = 0x1100,
  RSI_BT_EVT_SPP_CONNECTED                       = 0x1101,
  RSI_BT_EVT_SPP_DISCONNECTED                    = 0x1102,

  /*A2DP profile Event ID's*/
  RSI_BT_EVT_A2DP_CONNECTED                      = 0x1200,
  RSI_BT_EVT_A2DP_DISCONNECTED                   = 0x1201,
  RSI_BT_EVT_A2DP_CONFIGURED                     = 0x1202,
  RSI_BT_EVT_A2DP_OPEN                           = 0x1203,
  RSI_BT_EVT_A2DP_START                          = 0x1204,
  RSI_BT_EVT_A2DP_SUSPEND                        = 0x1205,
  RSI_BT_EVT_A2DP_ABORT                          = 0x1206,
  RSI_BT_EVT_A2DP_CLOSE                          = 0x1207,
  RSI_BT_EVT_A2DP_ENCODE_DATA                    = 0x1208,
  RSI_BT_EVT_A2DP_PCM_DATA                       = 0x1209,
  RSI_BT_EVT_A2DP_MORE_DATA_REQ                  = 0x120A,
  RSI_BT_EVT_A2DP_CODEC_CFG                      = 0x120B,

  /*AVRCP profile Event ID's*/
  RSI_BT_EVT_AVRCP_CONNECTED                     = 0x1300,
  RSI_BT_EVT_AVRCP_DISCONNECTED                  = 0x1301,
  RSI_BT_EVT_AVRCP_PLAY                          = 0x1302,
  RSI_BT_EVT_AVRCP_PAUSE                         = 0x1303,
  RSI_BT_EVT_AVRCP_NEXT                          = 0x1304,
  RSI_BT_EVT_AVRCP_PREVIOUS                      = 0x1305,
  RSI_BT_EVT_AVRCP_STOP                          = 0x1306,
  RSI_BT_EVT_AVRCP_VOL_UP                        = 0x1307,
  RSI_BT_EVT_AVRCP_VOL_DOWN                      = 0x1308,
  RSI_BT_EVT_AVRCP_GET_ELEM_RESP                 = 0x1309,
  RSI_BT_EVT_AVRCP_NOTIFY                        = 0x130A,

  /* AVRCP target  EVENT ID's */
  RSI_BT_EVT_AVRCP_GAT_CAPS                      = 0x1311,
  RSI_BT_EVT_AVRCP_GET_APP_SUPP_ATTS             = 0x1312,
  RSI_BT_EVT_AVRCP_GET_APP_SUUP_ATT_VALS         = 0x1313,
  RSI_BT_EVT_AVRCP_GET_APP_CUR_ATT_VAL           = 0x1314,
  RSI_BT_EVT_AVRCP_SET_APP_CUR_ATT_VAL           = 0x1315,
  RSI_BT_EVT_AVRCP_GET_APP_ATT_TEXT              = 0x1316,
  RSI_BT_EVT_AVRCP_GET_APP_ATT_VAL_TEXT          = 0x1317,
  RSI_BT_EVT_AVRCP_INFORM_CHAR_SETS              = 0x1318,
  RSI_BT_EVT_AVRCP_INFORM_BATT_STATUS            = 0x1319,
  RSI_BT_EVT_AVRCP_GET_SONG_ATTS                 = 0x131A,
  RSI_BT_EVT_AVRCP_GET_PLAY_STATUS               = 0x131B,
  RSI_BT_EVT_AVRCP_REG_NOTIFICATION              = 0x131C,
  RSI_BT_EVT_AVRCP_SET_ABS_VOL                   = 0x131D,

  /*HFP profile Event ID's*/
  RSI_BT_EVT_HFP_CONN                             = 0x1400,
  RSI_BT_EVT_HFP_DISCONN                          = 0x1401,
  RSI_BT_EVT_HFP_RING                             = 0x1402,
  RSI_BT_EVT_HFP_CALLCALLERID                     = 0x1403,
  RSI_BT_EVT_HFP_AUDIOCONNECTED	                 = 0x1404,
  RSI_BT_EVT_HFP_AUDIODISCONNECTED                = 0x1405,
  RSI_BT_EVT_HFP_DIALCOMPLETE                     = 0x1406,
  RSI_BT_EVT_HFP_ANSWERCOMPLETE                   = 0x1407,
  RSI_BT_EVT_HFP_HANGUPCOMPLETE                   = 0x1408,
  RSI_BT_EVT_HFP_SENDDTMFCOMPLETE                 = 0x1409,
  RSI_BT_EVT_HFP_CALLWAIT                         = 0x140A,
  RSI_BT_EVT_HFP_VOICERECOGDEACTIVATED            = 0x140B,
  RSI_BT_EVT_HFP_VOICERECOGACTIVATED              = 0x140C,
  RSI_BT_EVT_HFP_SERVICENOTFOUND                  = 0x140D,
  RSI_BT_EVT_HFP_CALL_STATUS                      = 0x140E,
  RSI_BT_EVT_HFP_SIGNALSTRENGTH                   = 0x140F,
  RSI_BT_EVT_HFP_BATTERYLEVEL                     = 0x1410,
  RSI_BT_EVT_HFP_PHONESERVICE                     = 0x1411,
  RSI_BT_EVT_HFP_ROAMINGSTATUS                    = 0x1412,
  RSI_BT_EVT_HFP_CALLSETUP                        = 0x1413,
  RSI_BT_EVT_HFP_CALLHELDSTATUS                   = 0x1414,

  /*PBAP profile Event ID's*/
  RSI_BT_EVT_PBAP_CONN                            = 0x1450,
  RSI_BT_EVT_PBAP_DISCONN                         = 0x1451,
  RSI_BT_EVT_PBAP_DATA                            = 0x1452,

  //HID profile events
  RSI_BT_EVENT_HID_CONN                           = 0x1460,
  RSI_BT_EVENT_HID_RXDATA                         = 0x1461,

 RSI_BT_EVENT_IAP_CONN                           =           0x152E,
 RSI_BT_EVENT_IAP_DISCONN                        =           0x152F,
 RSI_BT_EVENT_IAP_ACC_AUTH_STARTED               =           0x1530,
 RSI_BT_EVENT_IAP_ACC_AUTH_FAILED                =           0x1531,
 RSI_BT_EVENT_IAP_ACC_AUTH_COMPLETE              =           0x1532,
 RSI_BT_EVENT_IAP2_AUTH_START                    =           0x1533,
 RSI_BT_EVENT_IAP2_AUTH_COMPLETE                 =           0x1534,
 RSI_BT_EVENT_IAP2_AUTH_FAILED                   =           0x1535,
 RSI_BT_EVENT_IAP2_IDENTIFICATION_START          =           0x1536,
 RSI_BT_EVENT_IAP2_IDENTIFICATION_COMPLETE       =           0x1537,
 RSI_BT_EVENT_IAP2_IDENTIFICATION_REJECT         =           0x1538,
 RSI_BT_EVENT_IAP2_RX_DATA                       =           0x1539,
 RSI_BT_EVENT_IAP2_RX_FILE_TRANSFER_STATE        =           0x153A,
 RSI_BT_EVENT_IAP2_RX_FILE_TRANSFER_DATA         =           0x153B

}rsi_bt_event_t;

/******************************************************
 * *                    Structures
 * ******************************************************/

//!Set local COD name
typedef struct rsi_bt_req_set_local_cod_s
{
  uint32_t class_of_device;
}rsi_bt_req_set_local_cod_t;

//! Set Profile mode
typedef struct rsi_bt_req_profile_mode_s 
{
  uint8_t  profile_mode;
}rsi_bt_req_profile_mode_t;

//! Set Discovery mode
typedef struct rsi_bt_req_set_discv_mode_s
{
  uint8_t  mode;
  uint8_t  reserved[3];
  int32_t  time_out;
} rsi_bt_req_set_discv_mode_t;

//! Query Discovery mode response 
typedef struct rsi_bt_resp_query_discv_mode_s
{
  uint8_t  discovery_mode;
} rsi_bt_resp_query_discv_mode_t;

//! Set Connectability mode
typedef struct rsi_bt_req_set_connectability_mode
{
  uint8_t  connectability_mode;
}rsi_bt_req_set_connectability_mode_t;

//! Query Connectability mode response 
typedef struct rsi_bt_resp_query_connectability_mode_s
{
  uint8_t  connectability_mode;
} rsi_bt_resp_query_connectability_mode_t;

//! Set pair mode
typedef struct rsi_bt_pair_mode_s
{
  uint8_t  pair_mode;
}rsi_bt_req_set_pair_mode_t;

//! Query pair mode response
typedef struct rsi_bt_resp_query_pair_mode_s
{
  uint8_t  pair_mode;
} rsi_bt_resp_query_pair_mode_t;

//! Remote Name Request
typedef struct rsi_bt_req_remote_name_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
}rsi_bt_req_remote_name_t;

//! Remote Name Request Cancel
typedef struct rsi_bt_req_remote_name_cancel_s    
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
}rsi_bt_req_remote_name_cancel_t;

//! Inquiry command structure
typedef struct rsi_bt_req_inquiry_s
{
  uint8_t  inquiry_type;
  uint8_t  lap[3];
  uint32_t  duration;
  uint8_t  maximum_devices_to_find;
  uint8_t  reserved1[3];
} rsi_bt_req_inquiry_t;

//! Connect command 
typedef struct rsi_bt_req_connect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
}rsi_bt_req_connect_t;

typedef struct rsi_bt_hid_profile_data_s
{
#define MAX_PROFILE_DATA_BUFFER_LEN  1040
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  channel_id;
  uint8_t  len[2];
  uint8_t  buf[MAX_PROFILE_DATA_BUFFER_LEN];
}rsi_bt_hid_profile_data_t;

//! Gatt Connect command
typedef struct rsi_bt_req_gatt_connect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
}rsi_bt_req_gatt_connect_t;

//! Connect cancel command
typedef struct rsi_bt_req_connect_cancel_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
}rsi_bt_req_connect_cancel_t;

//! Un-bond command
typedef struct rsi_bt_req_disconnect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
}rsi_bt_req_disconnect_t;

//! User confirmation command structure
typedef struct rsi_bt_req_user_confirmation_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
	uint8_t  reserved[2];
  uint8_t  confirmation;
} rsi_bt_req_user_confirmation_t;

//! Passkey request reply command structure
typedef struct rsi_bt_req_passkey_reply_s
{
  uint8_t   dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t   reply_type;
  uint8_t   reserved;
  uint32_t   pass_key;
} rsi_bt_req_passkey_reply_t;

//! Pincode request reply command structure
typedef struct rsi_bt_req_pincode_reply_s
{
  uint8_t   dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t   reply_type;
  uint8_t   reserved;
  uint8_t   pincode[RSI_MAX_PINCODE_REPLY_SIZE];
} rsi_bt_req_pincode_reply_t;

//! Query Master-Slave role command 
typedef struct rsi_bt_req_query_role_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_query_role_t;

//! Set Master-Slave role command
typedef struct rsi_bt_req_set_role_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  role;
} rsi_bt_req_set_role_t;

//! Master-Slave role response
typedef struct rsi_bt_resp_query_role_s 
{
  uint8_t  role;
} rsi_bt_resp_query_role_t;

//! Set AFH Host channel classification command
typedef struct rsi_bt_req_set_afh_host_channel_classification_s
{
  uint8_t channel_assessment_mode;
  uint8_t enable;
  uint8_t channel_classification[CHANNEL_MAP_LEN];
} rsi_bt_req_set_afh_host_channel_classification_t;

//! Write Current IAP_LAP command
typedef struct rsi_bt_req_set_current_iap_lap_s
{
  uint8_t cnt;
  uint8_t list[6];  // presently limiting the list to 2 iap_laps
} rsi_bt_req_set_current_iap_lap_t;

//! Search Service command structure 
typedef struct rsi_bt_req_search_service_s
{
  uint8_t dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t reserved[2];
  uint32_t   service_uuid;
} rsi_bt_req_search_service_t;

//! SPP Connect command
typedef struct rsi_bt_req_spp_connect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_spp_connect_t;

//! SPP disconnect command
typedef struct rsi_bt_req_spp_disconnect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_spp_disconnect_t;

//! SPP transfer command stucture 
typedef struct rsi_bt_req_spp_transfer_s
{
  uint8_t  data_length[2];
  uint8_t  data[RSI_BT_MAX_PAYLOAD_SIZE];
} rsi_bt_req_spp_transfer_t;

//! Linkkey request reply command structure
typedef struct rsi_bt_req_linkkey_reply_s
{
  uint8_t   dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t   reply_type;
  uint8_t   reserved;
  uint8_t   linkkey[RSI_LINK_KEY_LEN];
} rsi_bt_req_linkkey_reply_t;


//!Sniff Mode
typedef struct rsi_bt_req_sniff_mode_s
{
  uint8_t   dev_addr[6];
  uint16_t  sniff_max_intv;
  uint16_t  sniff_min_intv;
  uint16_t  sniff_attempt;
  uint16_t  sniff_tout;
} rsi_bt_req_sniff_mode_t;


//!Sniff Exit Mode
typedef struct rsi_bt_req_sniff_exit_mode_s
{
  uint8_t   dev_addr[6];
} rsi_bt_req_sniff_exit_mode_t;

//!Sniff Subrating Mode
typedef struct rsi_bt_req_sniff_subrating_mode_s
{
  uint8_t   dev_addr[6];
  uint16_t  max_latency;
  uint16_t  min_remote_tout;
  uint16_t  min_local_tout;
} rsi_bt_req_sniff_subrating_mode_t;

//!Set SSP Mode
typedef struct rsi_bt_req_set_ssp_mode_s
{
  uint8_t   pair_mode;
  uint8_t   IO_capability;
} rsi_bt_req_set_ssp_mode_t;

//!Set EIR data
typedef struct rsi_bt_set_eir_data_s
{   
    uint8_t  fec_required;
	uint8_t  data_len;
	uint8_t  eir_data[200];
}rsi_bt_set_eir_data_t;

//! LP HP chain change command
typedef struct rsi_bt_vendor_ptt_type_s
{
  uint8_t opcode[2];
  uint8_t  len;
  uint8_t  mode;
}rsi_bt_vendor_ptt_type_t;

//! A2DP Connect command
typedef struct rsi_bt_req_a2dp_connect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_a2dp_connect_t;
//! IAP Connect 
typedef struct rsi_bt_req_iap_connect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  version;
} rsi_bt_req_iap_connect_t;


typedef struct rsi_bt_req_iap_disconnect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  version;
} rsi_bt_req_iap_disconnect_t;
//! A2DP disconnect command
typedef struct rsi_bt_req_a2dp_disconnect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_a2dp_disconnect_t;

typedef struct rsi_bt_req_iap_accessory_info_s
{ 
  uint8_t  info_type;                     
  uint8_t  info_len;
  uint8_t  info_data[RSI_BT_MAX_ACCESSORY_INFO_LEN];
} rsi_bt_req_iap_accessory_info_t;


typedef struct rsi_bt_iap_payload_s
{

  uint16_t  len;
  uint8_t  data[1000];

}rsi_bt_iap_payload_t;


typedef struct rsi_bt_iap_cmds_info_s
{

  uint16_t  len;
  uint16_t  cmd[20];

}rsi_bt_iap_cmds_info_t;


typedef struct rsi_bt_iap_File_transfer_info_s
{

  uint16_t  len;
  uint8_t   state;
  uint8_t   FileTransferId;
  uint8_t   FileTransferSessionId;
  uint8_t   data[RSI_BT_IAP_MAX_DATA_LEN];

}rsi_bt_iap_File_transfer_info_t;

typedef struct rsi_bt_req_iap_set_voice_over_s
{ 
  uint8_t  mode;           
  uint8_t  restore;
} rsi_bt_req_iap_set_voice_over_t;
typedef struct rsi_bt_req_iap_set_proto_s
{
  uint8_t type ;
}rsi_bt_req_iap_set_proto_t;



//! A2DP pcm/mp3 data structure
typedef struct rsi_bt_req_a2dp_pcm_mp3_data_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
	uint16_t pcm_mp3_data_len : 12;
  uint16_t  audio_type : 4;
	uint8_t  pcm_mp3_data[512]; //[RSI_BT_MAX_PAYLOAD_SIZE];
} rsi_bt_req_a2dp_pcm_mp3_data_t;

//! A2DP sbc data command
typedef struct rsi_bt_req_a2dp_sbc_aac_data_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
	uint16_t sbc_aac_data_len : 12;
  uint16_t  audio_type : 4;
	uint8_t  sbc_aac_data[RSI_BT_MAX_PAYLOAD_SIZE];
} rsi_bt_req_a2dp_sbc_aac_data_t;

//! A2DP request start command
typedef struct rsi_bt_req_a2dp_start_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_a2dp_start_t;

//! A2DP request suspend command
typedef struct rsi_bt_req_a2dp_suspend_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_a2dp_suspend_t;

//! A2DP request close command
typedef struct rsi_bt_req_a2dp_close_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_a2dp_close_t;

//! A2DP request abort command
typedef struct rsi_bt_req_a2dp_abort_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_a2dp_abort_t;

//! AVRCP profile command
//! AVRCP profile connect command
typedef struct rsi_bt_req_avrcp_conn_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_conn_t;

//! AVRCP profile disconnect command
typedef struct rsi_bt_req_avrcp_disconnect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_disconnect_t;

typedef struct rsi_bt_a2dp_sbc_codec_cap_s
{
  uint8_t      ChannelMode:4;
  uint8_t      Freq:4;
  uint8_t      AllocMethod:2;
  uint8_t      SubBands:2;
  uint8_t      BlockLength:4;
  uint8_t      MinBitPool;
  uint8_t      MaxBitPool;
}rsi_bt_a2dp_sbc_codec_cap_t;

typedef struct rsi_bt_req_a2dp_get_config_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_a2dp_get_config_t;

typedef struct rsi_bt_resp_a2dp_get_config_s
{
  rsi_bt_a2dp_sbc_codec_cap_t sbc_cap;
  uint32_t  status;
} rsi_bt_resp_a2dp_get_config_t;

typedef struct rsi_bt_req_a2dp_set_config_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  rsi_bt_a2dp_sbc_codec_cap_t sbc_cap;
} rsi_bt_req_a2dp_set_config_t;

typedef struct rsi_bt_resp_a2dp_set_config_s
{
  uint32_t err_status;
} rsi_bt_resp_a2dp_set_config_t;

//! AVRCP profile play song command
typedef struct rsi_bt_req_avrcp_play_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_play_t;

//! AVRCP profile pause song command
typedef struct rsi_bt_req_avrcp_pause_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_pause_t;

//! AVRCP profile stop song command
typedef struct rsi_bt_req_avrcp_stop_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_stop_t;

//! AVRCP profile next song command
typedef struct rsi_bt_req_avrcp_next_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_next_t;

//! AVRCP profile previous song command
typedef struct rsi_bt_req_avrcp_previous_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_previous_t;

//! AVRCP profile vol up command
typedef struct rsi_bt_req_avrcp_vol_up_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_vol_up_t;

//! AVRCP profile vol down command
typedef struct rsi_bt_req_avrcp_vol_down_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_vol_down_t;

//! AVRCP profile get capabilities command
typedef struct rsi_bt_req_avrcp_get_capabilities_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  type;
} rsi_bt_req_avrcp_get_capabilities_t;

typedef struct rsi_bt_rsp_avrcp_get_capabilities_s
{
  uint32_t  nbr_ids;
  uint32_t  ids[RSI_MAX_ATT];
} rsi_bt_rsp_avrcp_get_capabilities_t;

//! AVRCP profile get attributes list command
typedef struct rsi_bt_req_avrcp_get_att_list_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_get_att_list_t;

typedef struct rsi_bt_rsp_avrcp_get_atts_list_s
{
  uint8_t  nbr_ids;
  uint8_t  att_ids[RSI_MAX_ATT];
} rsi_bt_rsp_avrcp_get_atts_list_t;


//! AVRCP profile get attribute values list command
typedef struct rsi_bt_req_avrcp_get_att_vals_list_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  att_id;
} rsi_bt_req_avrcp_get_att_vals_list_t;

typedef struct rsi_bt_rsp_avrcp_get_att_vals_list_s
{
  uint8_t  nbr_vals;
  uint8_t  att_vals[RSI_MAX_ATT];
} rsi_bt_rsp_avrcp_get_att_vals_list_t;


//! AVRCP profile get current attribute value command
typedef struct rsi_bt_req_avrcp_get_cur_att_val_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  nbr_atts;
  uint8_t  att_list[RSI_MAX_ATT];
} rsi_bt_req_avrcp_get_cur_att_val_t;

typedef struct rsi_bt_rsp_avrcp_get_cur_att_val_s
{
  uint8_t  nbr_vals;
  uint8_t  att_vals[RSI_MAX_ATT];
} rsi_bt_rsp_avrcp_get_cur_att_val_t;


//! AVRCP profile Set current attribute value command

typedef struct rsi_bt_req_avrcp_set_cur_att_val_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  nbr_atts;
  uint8_t  reserved;
  att_val_t  att_val_list[RSI_MAX_ATT];
} rsi_bt_req_avrcp_set_cur_att_val_t;

//! AVRCP profile get song attribute value command
typedef struct rsi_bt_req_avrcp_get_ele_att_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  nbr_atts;
  uint8_t  reserved;
  uint32_t att_list[RSI_MAX_ATT];
} rsi_bt_req_avrcp_get_ele_att_t;

typedef struct normal_time_s {
    uint32_t  org_time;
    uint32_t  min;
    uint32_t  sec;
} normal_time_t;

typedef struct att_text_s {
  uint8_t   id;
  uint8_t   att_text[31];
} att_text_t;

typedef struct player_att_text_s {
    att_text_t  list[7];
    uint8_t  nbr_atts;
} player_att_text_t;

typedef struct attr_list_s {
  uint32_t id;
  uint16_t char_set_id;
  uint16_t attr_len;
  uint8_t attr_val[499];
} attr_list_t;

typedef struct elem_attr_list_s {
  uint8_t num_attrs;
  attr_list_t attr_list[7];
} elem_attr_list_t;

typedef struct rsi_bt_rsp_avrcp_get_ele_att_s
{
  player_att_text_t player_attr;   
} rsi_bt_rsp_avrcp_get_ele_att_t;

typedef struct rsi_bt_rsp_avrcp_elem_attr_s
{
  elem_attr_list_t elem_attr_list;   
} rsi_bt_rsp_avrcp_elem_attr_t;

//! AVRCP profile get player status command
typedef struct rsi_bt_req_avrcp_get_player_status_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_get_player_status_t;

typedef struct rsi_bt_rsp_avrcp_get_player_status_s
{
    uint32_t      song_len;
    uint32_t      song_pos;
    uint8_t       play_status;
} rsi_bt_rsp_avrcp_get_player_status_t;

//! AVRCP profile register notification event command
typedef struct rsi_bt_req_avrcp_reg_notification_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  event_id;
} rsi_bt_req_avrcp_reg_notification_t;


//! AVRCP profile get player status command
typedef struct rsi_bt_req_avrcp_remote_version_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_avrcp_remote_version_t;

typedef struct rsi_bt_rsp_avrcp_remote_version_s
{
  uint16_t  version;
} rsi_bt_rsp_avrcp_remote_version_t;

//! AVRCP profile get attribute value text command
typedef struct rsi_bt_req_avrcp_get_att_val_text_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  att_id;
  uint8_t  nbr_vals;
  uint8_t  vals[RSI_MAX_ATT];
} rsi_bt_req_avrcp_get_att_val_text_t;

//! AVRCP profile send the battery status command
typedef struct rsi_bt_req_avrcp_batt_status_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  batt_status;
} rsi_bt_req_avrcp_batt_status_t;

//! AVRCP profile send the support character sets command
typedef struct char_sets_s {
    uint16_t  supp_vals[5];
    uint8_t   nbr_sets;
}char_sets_t;

typedef struct rsi_bt_req_avrcp_char_sets_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  Reserved[2];
  char_sets_t  char_sets;
} rsi_bt_req_avrcp_char_sets_t;

typedef struct rsi_bt_avrcp_set_abs_vol_s
{
  uint8_t dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t rev;
  uint8_t abs_vol;
} rsi_bt_avrcp_set_abs_vol_t;


#define  AVRCP_SUPP_CMP_CAP_ID       2
#define  AVRCP_SUPP_NOTIFYS_CAP_ID   3
typedef struct rsi_bt_avrcp_cap_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  cap_type;
  uint8_t  nbr_caps;
  uint32_t caps[10];
} rsi_bt_avrcp_cap_resp_t;

typedef struct rsi_bt_avrcp_att_list_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  nbr_atts;
  uint8_t  atts[10];
} rsi_bt_avrcp_att_list_resp_t;

typedef struct rsi_bt_avrcp_att_vals_list_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  nbr_vals;
  uint8_t  vals[10];
} rsi_bt_avrcp_att_vals_list_resp_t;


typedef struct rsi_bt_avrcp_cur_att_vals_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  reserved[2];
  att_val_list_t  att_list;
} rsi_bt_avrcp_cur_att_vals_resp_t;

typedef struct rsi_bt_avrcp_set_att_vals_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  status;
} rsi_bt_avrcp_set_att_vals_resp_t;

typedef struct rsi_bt_avrcp_elem_attr_resp_s
{
  uint8_t dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t reserved[2];
  elem_attr_list_t elem_attr_list;
} rsi_bt_avrcp_elem_attr_resp_t;

typedef struct rsi_bt_avrcp_play_status_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  play_status;
  uint8_t  reserved;
  uint32_t song_len;
  uint32_t song_pos;
} rsi_bt_avrcp_play_status_resp_t;

typedef union reg_notify_val_u {
  uint8_t play_status;
  uint8_t curr_track_idx[8];
  uint8_t curr_playback_pos[4];
  uint8_t batter_status;
  uint8_t system_status;
  uint8_t player_app_setting[3];
  uint8_t abs_vol;
} reg_notify_val_t;

typedef struct rsi_bt_avrcp_reg_notify_interim_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  event_id;
  uint8_t  reserved;
  reg_notify_val_t reg_notify_val;
} rsi_bt_avrcp_reg_notify_interim_resp_t;

typedef struct rsi_bt_avrcp_reg_notifiy_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  status;
} rsi_bt_avrcp_reg_notify_resp_t;

typedef struct rsi_bt_avrcp_att_text_resp_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  reserved[2];
  player_att_text_t atts_text;
} rsi_bt_avrcp_att_text_resp_t;

/* typedef union notify_val_s {
  uint8_t   player_status;
  uint32_t  track_num;
  uint32_t  track_pos;
  uint8_t   battery_status;
  uint8_t   system_status;
  att_val_list_t  att_val_list;
} notify_val_t; */

typedef struct rsi_bt_avrcp_notify_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  event_id;
  uint8_t  reserved;
  notify_val_t notify_val;
} rsi_bt_avrcp_notify_t;

typedef struct rsi_bt_avrcp_cmd_reject_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  pdu_id;
  uint8_t  status;
} rsi_bt_avrcp_cmd_reject_t;

typedef struct rsi_bt_avrcp_set_abs_vol_resp_s
{
  uint8_t dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t abs_vol;
} rsi_bt_avrcp_set_abs_vol_resp_t;


//! HFP Connect command
typedef struct rsi_bt_req_hfp_connect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_hfp_connect_t;

//! HFP disconnect command
typedef struct rsi_bt_req_hfp_disconnect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_hfp_disconnect_t;

//! HFP phone operator command
typedef struct rsi_bt_req_hfp_phoneoperator_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_hfp_phoneoperator_t;

//! HFP call reject command
typedef struct rsi_bt_req_hfp_callreject_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_hfp_callreject_t;

//! HFP dial num command
typedef struct rsi_bt_req_hfp_dialnum_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
   uint8_t phone_no[10]; 
} rsi_bt_req_hfp_dialnum_t;

//! HFP dial mem command
typedef struct rsi_bt_req_hfp_dialmem_s
{
    uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
    uint32_t  mem_ix;
} rsi_bt_req_hfp_dialmem_t;

//! HFP voice recognition active command
typedef struct rsi_bt_req_hfp_voicerecognitionactive_s
{
     uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_hfp_voicerecognitionactive_t;

//! HFP voice recognition deactive command
typedef struct rsi_bt_req_hfp_voicerecognitiondeactive_s
{
     uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_hfp_voicerecognitiondeactive_t;

//! HFP profile command
typedef struct rsi_bt_req_hfp_micgain_s
{
     uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
     uint8_t  mic_gain;
} rsi_bt_req_hfp_micgain_t;

//! HFP profile command
typedef struct rsi_bt_req_hfp_spkgain_s
{
     uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
     uint8_t  spk_gain;
} rsi_bt_req_hfp_spkgain_t;

//! HFP profile command
typedef struct rsi_bt_req_hfp_getcalls_s
{
     uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_hfp_getcalls_t;

//! HFP profile command
typedef struct rsi_bt_req_hfp_audio_s
{
     uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_hfp_audio_t;

//! PBAP Connect command
typedef struct rsi_bt_req_pbap_connect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_pbap_connect_t;

//! PBAP disconnect command
typedef struct rsi_bt_req_pbap_disconnect_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
} rsi_bt_req_pbap_disconnect_t;
//! PBAP contacts command
typedef struct rsi_bt_req_pbap_contacts_s
{
  uint8_t  dev_addr[RSI_DEV_ADDR_LEN];
  uint8_t  folder_id;
  uint8_t  nbr_contacts;
  uint16_t  start_ix;
} rsi_bt_req_pbap_contacts_t;

typedef struct rsi_bt_req_add_device_id_s
{
  uint16_t spec_id;
  uint16_t vendor_id;
  uint16_t product_id;
  uint16_t version;
  int      primary_rec;
  uint16_t vendor_id_source;
}rsi_bt_req_add_device_id_t;

//! avdtp stats command structure
typedef struct rsi_bt_vendor_avdtp_stats_s
{
  uint8_t opcode[2];
  uint8_t avdtp_stats_enable;
}rsi_bt_vendor_avdtp_stats_t;




//! Driver BT Classic control block
struct rsi_bt_classic_cb_s
{
  //! GAP Callbacks
  rsi_bt_on_role_change_t               bt_on_role_change_status_event;     
  rsi_bt_on_connect_t                   bt_on_conn_status_event;     
  rsi_bt_on_unbond_t                    bt_on_unbond_status;
  rsi_bt_on_disconnect_t                bt_on_disconnect_event;
  rsi_bt_on_scan_resp_t                 bt_on_scan_resp_event;
  rsi_bt_on_remote_name_resp_t          bt_on_remote_name_resp_event;
  rsi_bt_on_passkey_display_t           bt_on_passkey_display_event;
  rsi_bt_on_remote_name_request_cancel_t          bt_on_remote_name_request_cancel_event;
  rsi_bt_on_confirm_request_t           bt_on_confirm_request_event; 
  rsi_bt_on_pincode_request_t           bt_on_pincode_request_event; 
  rsi_bt_on_passkey_request_t           bt_on_passkey_request_event; 
  rsi_bt_on_inquiry_complete_t          bt_on_inquiry_complete_event;
  rsi_bt_on_auth_complete_t             bt_on_auth_complete_event;
  rsi_bt_on_linkkey_request_t           bt_on_linkkey_request_event;
  rsi_bt_on_ssp_complete_t              bt_on_ssp_complete_event;
  rsi_bt_on_linkkey_save_t              bt_on_linkkey_save_event;
  rsi_bt_on_get_services_t              bt_on_get_services_event;
  rsi_bt_on_search_service_t            bt_on_search_service_event;
  rsi_bt_on_mode_chnage_t               bt_on_mode_change_event;
  rsi_bt_on_sniff_subrating_t	          bt_on_sniff_subrating_event;

  //! BT GATT Callbacks
  rsi_bt_on_gatt_connection_t           bt_on_gatt_connection_event;
  rsi_bt_on_gatt_disconnection_t        bt_on_gatt_disconnection_event;


  //! SPP Callbacks
  rsi_bt_on_spp_connect_t         bt_on_spp_connect_event;
  rsi_bt_on_spp_disconnect_t      bt_on_spp_disconnect_event;
  rsi_bt_on_spp_rx_data_t         bt_on_spp_rx_data_event;

  //! A2DP Callbacks
  rsi_bt_on_a2dp_connect_t         bt_on_a2dp_connect_event;
  rsi_bt_on_a2dp_disconnect_t      bt_on_a2dp_disconnect_event;
  rsi_bt_on_a2dp_configure_t       bt_on_a2dp_configure_event;
  rsi_bt_on_a2dp_open_t            bt_on_a2dp_open_event;
  rsi_bt_on_a2dp_start_t           bt_on_a2dp_start_event;
  rsi_bt_on_a2dp_suspend_t         bt_on_a2dp_suspend_event;
  rsi_bt_on_a2dp_abort_t           bt_on_a2dp_abort_event;
  rsi_bt_on_a2dp_close_t           bt_on_a2dp_close_event;
  rsi_bt_on_a2dp_encode_data_t     bt_on_a2dp_encode_data_event;
  rsi_bt_on_a2dp_pcm_data_t        bt_on_a2dp_pcm_data_event;
  rsi_bt_on_a2dp_data_req_t        bt_on_a2dp_data_req_event;
  //AVRCP Callbacks
  rsi_bt_on_avrcp_connect_t         bt_on_avrcp_connect_event;
  rsi_bt_on_avrcp_disconnect_t      bt_on_avrcp_disconnect_event;
  rsi_bt_on_avrcp_play_t            bt_on_avrcp_play_event;
  rsi_bt_on_avrcp_pause_t           bt_on_avrcp_pause_event;
  rsi_bt_on_avrcp_stop_t            bt_on_avrcp_stop_event;
  rsi_bt_on_avrcp_next_t            bt_on_avrcp_next_event;
  rsi_bt_on_avrcp_previous_t        bt_on_avrcp_previous_event;
  rsi_bt_on_avrcp_vol_up_t          bt_on_avrcp_vol_up_event;
  rsi_bt_on_avrcp_vol_down_t        bt_on_avrcp_vol_down_event;
  rsi_bt_on_avrcp_get_elem_attr_resp_t bt_on_avrcp_get_elem_attr_resp_event;
  rsi_bt_on_avrcp_notify_event_t    bt_on_avrcp_notify_event;

  //! AVRCP target Callbacks
  rsi_bt_on_avrcp_get_cap_event_t                  bt_on_avrcp_gat_cap;
  rsi_bt_on_avrcp_get_app_supp_atts_event_t        bt_on_avrcp_get_app_att_list;
  rsi_bt_on_avrcp_get_app_suup_att_vals_event_t    bt_on_avrcp_get_app_att_vals_list;
  rsi_bt_on_avrcp_get_app_cur_att_val_event_t      bt_on_avrcp_get_app_cur_att_val;
  rsi_bt_on_avrcp_set_app_cur_att_val_event_t      bt_on_avrcp_set_app_cur_att_val;
  rsi_bt_on_avrcp_get_app_att_text_event_t         bt_on_avrcp_get_app_att_text;
  rsi_bt_on_avrcp_get_app_att_vals_text_event_t    bt_on_avrcp_get_app_att_vals_text;
  rsi_bt_on_avrcp_supp_char_sets_event_t           bt_on_avrcp_supp_char_sets;
  rsi_bt_on_avrcp_batt_status_event_t              bt_on_avrcp_batt_status;
  rsi_bt_on_avrcp_get_song_atts_event_t            bt_on_avrcp_get_song_atts;
  rsi_bt_on_avrcp_get_play_status_event_t          bt_on_avrcp_get_player_status;
  rsi_bt_on_avrcp_reg_notify_event_t               bt_on_avrcp_reg_notifivation;
  rsi_bt_on_avrcp_set_abs_vol_event_t              bt_on_avrcp_set_abs_vol;

  //!HFP call backs
  rsi_bt_on_hfp_connect_t 			bt_on_hfp_connect_event;
  rsi_bt_on_hfp_disconnect_t         	bt_on_hfp_disconnect_event;
  rsi_bt_on_hfp_ring_t               	bt_on_hfp_ring_event;
  rsi_bt_on_hfp_callcallerid_t      		bt_on_hfp_callcallerid_event;
  rsi_bt_on_hfp_audioconnected_t    		bt_on_hfp_audioconnected_event;
  rsi_bt_on_hfp_audiodisconnected_t 		bt_on_hfp_audiodisconnected_event;
  rsi_bt_on_hfp_dialcomplete_t          	bt_on_hfp_dialcomplete_event;
  rsi_bt_on_hfp_answercomplete_t        	bt_on_hfp_answercomplete_event;
  rsi_bt_on_hfp_hangupcomplete_t        	bt_on_hfp_hangupcomplete_event;
  rsi_bt_on_hfp_senddtmfcomplete_t      	bt_on_hfp_senddtmfcomplete_event;
  rsi_bt_on_hfp_callwait_t              	bt_on_hfp_callwait_event;                     
  rsi_bt_on_hfp_callvoicerecogdeactivated_t  bt_on_hfp_callvoicerecogdeactivated_event;                     
  rsi_bt_on_hfp_callvoicerecogactivated_t    bt_on_hfp_callvoicerecogactivated_event;                     
  rsi_bt_on_hfp_servicenotfound_t            bt_on_hfp_servicenotfound_event;
  rsi_bt_app_on_hfp_callstatus_t             bt_on_hfp_callstatus_event;
  rsi_bt_app_on_hfp_signalstrength_t         bt_on_hfp_signalstrength_event;
  rsi_bt_app_on_hfp_batterylevel_t           bt_on_hfp_batterylevel_event;
  rsi_bt_app_on_hfp_phoneservice_t           bt_on_hfp_phoneservice_event;
  rsi_bt_app_on_hfp_roamingstatus_t          bt_on_hfp_roamingstatus_event;
  rsi_bt_app_on_hfp_callsetup_t              bt_on_hfp_callsetup_event;
  rsi_bt_app_on_hfp_callheld_t               bt_on_hfp_callheld_event;

  //!PBAP call backs
  rsi_bt_on_pbap_connect_t 			         bt_on_pbap_connect_event;
  rsi_bt_on_pbap_disconnect_t         	     bt_on_pbap_disconnect_event;
  rsi_bt_on_pbap_data_t                      bt_on_pbap_data_event;
   
  /* AVDTP STATS Callbacks */
  rsi_bt_on_avdtp_stats_t                      bt_on_avdtp_stats_event;
  //! HID Callbacks
  rsi_bt_on_hid_connect_t            bt_on_hid_connect_event;
  rsi_bt_on_hid_rx_data_t            bt_on_hid_rx_data_event;
  rsi_bt_on_hid_handshake_t          bt_on_hid_handshake_event;
  rsi_bt_on_hid_control_t            bt_on_hid_control_event;
  rsi_bt_on_hid_get_report_t         bt_on_hid_get_report;
  rsi_bt_on_hid_set_report_t         bt_on_hid_set_report;
  rsi_bt_on_hid_get_protocol_t       bt_on_hid_get_proto;
  rsi_bt_on_hid_set_protocol_t       bt_on_hid_set_proto;
 
  rsi_bt_app_iap_conn_t                       bt_app_iap_conn;
  rsi_bt_app_iap_disconn_t                    bt_app_iap_disconn;
  rsi_bt_app_iap_acc_auth_strt_t              bt_app_iap_acc_auth_strt;
  rsi_bt_app_iap_acc_auth_fail_t              bt_app_iap_acc_auth_fail;
  rsi_bt_app_iap_acc_auth_complete_t          bt_app_iap_acc_auth_complete;
  rsi_bt_app_iap2_auth_start_t                bt_app_iap2_auth_start;
  rsi_bt_app_iap2_auth_complete_t             bt_app_iap2_auth_complete;
  rsi_bt_app_iap2_auth_fail_t                 bt_app_iap2_auth_fail;
  rsi_bt_app_iap2_identification_start_t      bt_app_iap2_identification_start;
  rsi_bt_app_iap2_identification_complete_t   bt_app_iap2_identification_complete;
  rsi_bt_app_iap2_identification_reject_t     bt_app_iap2_identification_reject;
  rsi_bt_app_on_iap2_data_rx_t                bt_app_on_iap2_data_rx;
  rsi_bt_app_iap2_File_Tx_state_t             bt_app_iap2_File_Tx_state; 
  rsi_bt_app_iap2_File_Transfer_rx_data_t     bt_app_iap2_File_Transfer_rx_data;
};

/******************************************************
 * * BT Classic internal function declarations
 * ******************************************************/
void rsi_bt_callbacks_handler(rsi_bt_cb_t *bt_cb, uint16_t rsp_type, uint8_t *payload, uint16_t payload_length);

#endif
