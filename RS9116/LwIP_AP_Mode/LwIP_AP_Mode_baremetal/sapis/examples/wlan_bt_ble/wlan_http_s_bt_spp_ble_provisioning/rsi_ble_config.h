/**
 * @file         rsi_ble_config.h
 * @version      0.1
 * @date         01 Feb 2020 *
 *  Copyright(C) Redpine Signals 2020
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
 */

#ifndef RSI_BLE_CONFIG_H
#define RSI_BLE_CONFIG_H

/***********************************************************************************************************************************************/
//  ! INCLUDES
/***********************************************************************************************************************************************/
#ifdef FRDM_K28F
#include "rsi_common_app.h"
#endif
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>
#include "rsi_ble_apis.h"



/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
//! BLE_DUAL_MODE_BT_A2DP_SOURCE_WIFI_HTTP_S_RX SAPI BLE CONFIG DEFINES
/***********************************************************************************************************************************************/
/***********************************************************************************************************************************************/
#define RSI_BLE_APP_GATT_TEST                        (void *)"SI_COEX_MAX_DEMO"            //! local device name
#define RSI_BLE_MAX_NBR_SLAVES                       1
#define RSI_BLE_MAX_NBR_MASTERS                      1
#define	TOTAL_CONNECTIONS							 RSI_BLE_MAX_NBR_SLAVES+RSI_BLE_MAX_NBR_MASTERS

//! enable this macro to test the application using ble provisioning
#define	BLE_PROVISIONING_ENABLED						1
#define MAX_DATA_LENGTH1								20


#define CONN_BY_ADDR                                  	1
#define CONN_BY_NAME                                  	2
#define CONNECT_OPTION                                	CONN_BY_NAME//CONN_BY_ADDR

#if(CONNECT_OPTION == CONN_BY_NAME)
/***********************************************************************************************************************************************/
//! Remote Device Name to connect
/***********************************************************************************************************************************************/
#define RSI_REMOTE_DEVICE_NAME1                       "REDPINE_DEV"//"Hotspot"//"Skv"//"Hotspot"
#define RSI_REMOTE_DEVICE_NAME2                       "slave22"
#define RSI_REMOTE_DEVICE_NAME3                       "slave3"
#else


#define RSI_BLE_DEV_ADDR_TYPE                         LE_PUBLIC_ADDRESS                         //! Address type of the device to connect - LE_PUBLIC_ADDRESS / LE_RANDOM_ADDRESS
/***********************************************************************************************************************************************/
//! Address of the devices to connect
/***********************************************************************************************************************************************/
#define RSI_BLE_DEV_1_ADDR                             "00:1A:7D:DA:71:16"
#define RSI_BLE_DEV_2_ADDR                             "00:1A:7D:DA:71:74"
#define RSI_BLE_DEV_3_ADDR                             "00:1A:7D:DA:73:13"
#endif

/***********************************************************************************************************************************************/
//! BLE connection specific configurations
/***********************************************************************************************************************************************/
/*=================Master1 configurations=====================*/
//! configure below macro to enable secure connection
#define SMP_ENABLE_M1                                	0
// Add remote device to whitelist
#define ADD_TO_WHITELIST_M1								0

#if BLE_PROVISIONING_ENABLED
//! configure below macro to discover remote profiles
#define PROFILE_QUERY_M1								0
//! configure below macro to perform data transfer
#define DATA_TRANSFER_M1								0
#else
//! configure below macro to discover remote profiles
#define PROFILE_QUERY_M1								1
//! configure below macro to perform data transfer
#define DATA_TRANSFER_M1								1
#endif
//!configure below macros to select type of data transfer
//! set below macro to receive 'gatt notifications' from remote device
#define RX_NOTIFICATIONS_FROM_M1						0
//! set below macro to receive 'gatt indications' from remote device
#define RX_INDICATIONS_FROM_M1							0
//! set below macro to Transmit 'gatt notifications' to remote device
#define TX_NOTIFICATIONS_TO_M1							1
//! set below macro to Transmit 'gatt write with response' to remote device
#define	TX_WRITES_TO_M1									0
//! set below macro to Transmit 'gatt write without response' to remote device
#define TX_WRITES_NO_RESP_TO_M1							0
//! set below macro to Transmit 'gatt indications' to remote device
#define TX_INDICATIONS_TO_M1							0
//! Configure below macro to select data length extension ON/OFF
#define DLE_ON_M1										0
#if DLE_ON_M1
#define DLE_BUFFER_MODE_M1   	                        1
#define DLE_BUFFER_COUNT_M1                             2
#define RSI_BLE_MAX_DATA_LEN_M1                         230
#else
#define DLE_BUFFER_MODE_M1                              0
#define DLE_BUFFER_COUNT_M1                             2
#define RSI_BLE_MAX_DATA_LEN_M1                         20                            //! max data length
#endif
//! Configure below macros to select connection paramaters while data transfer
#define CONN_INTERVAL_M1								36 //! for conn interval of 45ms
#define CONN_LATENCY_M1									0
#define CONN_SUPERVISION_TIMEOUT_M1						400


/*=================Master2 configurations=====================*/
//! configure below macro to enable secure connection
#define SMP_ENABLE_M2                                	0
// Add remote device to whitelist
#define ADD_TO_WHITELIST_M2								0
//! configure below macro to discover remote profiles
#define PROFILE_QUERY_M2								1
//! configure below macro to perform data transfer
#define DATA_TRANSFER_M2								1
//!configure below macros to select type of data transfer
//! set below macro to receive 'gatt notifications' from remote device
#define RX_NOTIFICATIONS_FROM_M2						0
//! set below macro to receive 'gatt indications' from remote device
#define RX_INDICATIONS_FROM_M2							0
//! set below macro to Transmit 'gatt notifications' to remote device
#define TX_NOTIFICATIONS_TO_M2							1
//! set below macro to Transmit 'gatt write with response' to remote device
#define	TX_WRITES_TO_M2									0
//! set below macro to Transmit 'gatt write without response' to remote device
#define TX_WRITES_NO_RESP_TO_M2							0
//! set below macro to Transmit 'gatt indications' to remote device
#define TX_INDICATIONS_TO_M2							0
//#define RSI_BLE_CLIENT_WRITE_SERVICE_UUID_M2      		0x180D                          //! Heart Rate service uuid
//#define RSI_BLE_CLIENT_WRITE_CHAR_UUID_M2				0x2A39							//! Heart Rate control Point
//#define RSI_BLE_CLIENT_WRITE_NO_RESP_SERVICE_UUID_M2   	0x1802                          //! Immediate Alert service uuid
//#define RSI_BLE_CLIENT_WRITE_NO_RESP_CHAR_UUID_M2   	0x2A06                          //! Alert level char uuid
//#define RSI_BLE_CLIENT_INIDCATIONS_SERVICE_UUID_M2    	0x1809                          //! Health thermometer Alert service uuid
//#define RSI_BLE_CLIENT_INIDCATIONS_CHAR_UUID_M2   		0x2902                          //! Temperature measurement
//#define RSI_BLE_CLIENT_NOTIFICATIONS_SERVICE_UUID_M2	0x180D                          //! Heart Rate service uuid
//#define RSI_BLE_CLIENT_NOTIFICATIONS_CHAR_UUID_M2		0x2A37							//! Heart Rate measurement
//! Configure below macro to select data length extension ON/OFF
#define DLE_ON_M2										0
#if DLE_ON_M2
#define DLE_BUFFER_MODE_M2   	                        1
#define DLE_BUFFER_COUNT_M2                             2
#define RSI_BLE_MAX_DATA_LEN_M2                         230
#else
#define DLE_BUFFER_MODE_M2                              0
#define DLE_BUFFER_COUNT_M2                             2
#define RSI_BLE_MAX_DATA_LEN_M2                         20                            //! max data length
#endif
//! Configure below macros to select connection paramaters while data transfer
#define CONN_INTERVAL_M2								400	//! for conn interval of 500ms
#define CONN_LATENCY_M2									0
#define CONN_SUPERVISION_TIMEOUT_M2						400



/*=================Slave1 configurations=====================*/
//! configure below macro to enable secure connection
#define SMP_ENABLE_S1                                	0
// Add remote device to whitelist
#define ADD_TO_WHITELIST_S1								0
//! configure below macro to discover remote profiles
#define PROFILE_QUERY_S1								1
//! configure below macro to perform data transfer
#define DATA_TRANSFER_S1								1
//!configure below macros to select type of data transfer
//! set below macro to receive 'gatt notifications' from remote device
#define RX_NOTIFICATIONS_FROM_S1						1
//! set below macro to receive 'gatt indications' from remote device
#define RX_INDICATIONS_FROM_S1							0
//! set below macro to Transmit 'gatt notifications' to remote device
#define TX_NOTIFICATIONS_TO_S1							0
//! set below macro to Transmit 'gatt write with response' to remote device
#define	TX_WRITES_TO_S1									0
//! set below macro to Transmit 'gatt write without response' to remote device
#define TX_WRITES_NO_RESP_TO_S1							0
//! set below macro to Transmit 'gatt indications' to remote device
#define TX_INDICATIONS_TO_S1							0
//#define RSI_BLE_CLIENT_WRITE_SERVICE_UUID_S1      		0x180D                          //! Heart Rate service uuid
//#define RSI_BLE_CLIENT_WRITE_CHAR_UUID_S1				0x2A39							//! Heart Rate control Point
//#define RSI_BLE_CLIENT_WRITE_NO_RESP_SERVICE_UUID_S1   	0x1802                          //! Immediate Alert service uuid
//#define RSI_BLE_CLIENT_WRITE_NO_RESP_CHAR_UUID_S1   	0x2A06                          //! Alert level char uuid
//#define RSI_BLE_CLIENT_INIDCATIONS_SERVICE_UUID_S1    	0x1809                          //! Health thermometer Alert service uuid
//#define RSI_BLE_CLIENT_INIDCATIONS_CHAR_UUID_S1   		0x2902                          //! Temperature measurement
//#define RSI_BLE_CLIENT_NOTIFICATIONS_SERVICE_UUID_S1	0x180D                          //! Heart Rate service uuid
//#define RSI_BLE_CLIENT_NOTIFICATIONS_CHAR_UUID_S1		0x2A37							//! Heart Rate measurement
//! Configure below macro to select data length extension ON/OFF
#define DLE_ON_S1										0
#if DLE_ON_S1
#define DLE_BUFFER_MODE_S1   	                        1
#define DLE_BUFFER_COUNT_S1                             2
#define RSI_BLE_MAX_DATA_LEN_S1                         230
#else
#define DLE_BUFFER_MODE_S1                              0
#define DLE_BUFFER_COUNT_S1                             2
#define RSI_BLE_MAX_DATA_LEN_S1                         20                            //! max data length
#endif
//! Configure below macros to select connection paramaters while data transfer
#define CONN_INTERVAL_S1								240 //! for conn interval of 300ms
#define CONN_LATENCY_S1									0
#define CONN_SUPERVISION_TIMEOUT_S1						400



/*=================Slave2 configurations=====================*/
//! configure below macro to enable secure connection
#define SMP_ENABLE_S2                                	0
// Add remote device to whitelist
#define ADD_TO_WHITELIST_S2								0
//! configure below macro to discover remote profiles
#define PROFILE_QUERY_S2								1
//! configure below macro to perform data transfer
#define DATA_TRANSFER_S2								1
//!configure below macros to select type of data transfer
//! set below macro to receive 'gatt notifications' from remote device
#define RX_NOTIFICATIONS_FROM_S2						1
//! set below macro to receive 'gatt indications' from remote device
#define RX_INDICATIONS_FROM_S2							0
//! set below macro to Transmit 'gatt notifications' to remote device
#define TX_NOTIFICATIONS_TO_S2							0
//! set below macro to Transmit 'gatt write with response' to remote device
#define	TX_WRITES_TO_S2									0
//! set below macro to Transmit 'gatt write without response' to remote device
#define TX_WRITES_NO_RESP_TO_S2							0
//! set below macro to Transmit 'gatt indications' to remote device
#define TX_INDICATIONS_TO_S2							0
//! Configure below macro to select data length extension ON/OFF
#define DLE_ON_S2										0
#if DLE_ON_S2
#define DLE_BUFFER_MODE_S2   	                        1
#define DLE_BUFFER_COUNT_S2                             2
#define RSI_BLE_MAX_DATA_LEN_S2                         230
#else
#define DLE_BUFFER_MODE_S2                              0
#define DLE_BUFFER_COUNT_S2                             2
#define RSI_BLE_MAX_DATA_LEN_S2                         20                            //! max data length
#endif
//! Configure below macros to select connection paramaters while data transfer
#define CONN_INTERVAL_S2								240 //! for conn interval of 300ms
#define CONN_LATENCY_S2									0
#define CONN_SUPERVISION_TIMEOUT_S2						400




/*=================Slave3 configurations=====================*/
//! configure below macro to enable secure connection
#define SMP_ENABLE_S3                                	0
// Add remote device to whitelist
#define ADD_TO_WHITELIST_S3								0
//! configure below macro to discover remote profiles
#define PROFILE_QUERY_S3								1
//! configure below macro to perform data transfer
#define DATA_TRANSFER_S3								1
//!configure below macros to select type of data transfer
//! set below macro to receive 'gatt notifications' from remote device
#define RX_NOTIFICATIONS_FROM_S3						1
//! set below macro to receive 'gatt indications' from remote device
#define RX_INDICATIONS_FROM_S3							0
//! set below macro to Transmit 'gatt notifications' to remote device
#define TX_NOTIFICATIONS_TO_S3							0
//! set below macro to Transmit 'gatt write with response' to remote device
#define	TX_WRITES_TO_S3									0
//! set below macro to Transmit 'gatt write without response' to remote device
#define TX_WRITES_NO_RESP_TO_S3							0
//! set below macro to Transmit 'gatt indications' to remote device
#define TX_INDICATIONS_TO_S3							0
//! Configure below macro to select data length extension ON/OFF
#define DLE_ON_S3										0
#if DLE_ON_S3
#define DLE_BUFFER_MODE_S3   	                        1
#define DLE_BUFFER_COUNT_S3                             2
#define RSI_BLE_MAX_DATA_LEN_S3                         230
#else
#define DLE_BUFFER_MODE_S3                              0
#define DLE_BUFFER_COUNT_S3                             2
#define RSI_BLE_MAX_DATA_LEN_S3                         20                            //! max data length
#endif
//! Configure below macros to select connection paramaters while data transfer
#define CONN_INTERVAL_S3								240 //! for conn interval of 300ms
#define CONN_LATENCY_S3									0
#define CONN_SUPERVISION_TIMEOUT_S3						400



#define RSI_BLE_CLIENT_WRITE_SERVICE_UUID_M1      		0x180D                          //! Heart Rate service uuid
#define RSI_BLE_CLIENT_WRITE_CHAR_UUID_M1				0x2A39							//! Heart Rate control Point
#define RSI_BLE_CLIENT_WRITE_NO_RESP_SERVICE_UUID_M1   	0x1802                          //! Immediate Alert service uuid
#define RSI_BLE_CLIENT_WRITE_NO_RESP_CHAR_UUID_M1   	0x2A06                          //! Alert level char uuid
#define RSI_BLE_CLIENT_INIDCATIONS_SERVICE_UUID_M1    	0x1809                          //! Health thermometer Alert service uuid
#define RSI_BLE_CLIENT_INIDCATIONS_CHAR_UUID_M1   		0x2A1C                          //! Temperature measurement
#define RSI_BLE_CLIENT_NOTIFICATIONS_SERVICE_UUID_M1	0x180D                          //! Heart Rate service uuid
#define RSI_BLE_CLIENT_NOTIFICATIONS_CHAR_UUID_M1		0x2A37							//! Heart Rate measurement


#define	RSI_DEBUG_EN									0								//! To get deep understanding of BLE execution flow
																						//!	Enable this macro to get debug logs
#define RSI_BLE_GATT_ASYNC_ENABLE                    	1
#define RSI_BLE_INDICATE_CONFIRMATION_FROM_HOST      	1
#define RSI_BLE_MTU_EXCHANGE_FROM_HOST               	1
#define MAX_MTU_SIZE                                    240
#define UPDATE_CONN_PARAMETERS                          0                                        //! To update connection parameters of remote master connection
#define MITM_ENABLE                                     1
#define RSI_BLE_MAX_CHAR_DESCRIPTORS		  			5
#define RSI_MAX_PROFILE_CNT								8
#define SMP_ENABLE									 	1
#define SIG_VUL                                         0
#define RESOLVE_ENABLE                      			0

#define SLAVE1										0
#define SLAVE2										1
#define SLAVE3										2
#define MASTER1										RSI_BLE_MAX_NBR_SLAVES
#define MASTER2										RSI_BLE_MAX_NBR_SLAVES+1

/***********************************************************************************************************************************************/
//! application events list
/***********************************************************************************************************************************************/
typedef enum rsi_ble_state_e{
	RSI_BLE_DISCONN_EVENT  =  0,
	RSI_APP_EVENT_ADV_REPORT,
	RSI_BLE_CONN_EVENT,
	RSI_BLE_ENHC_CONN_EVENT,
	RSI_BLE_MTU_EVENT,
	RSI_BLE_MORE_DATA_REQ_EVT,
	RSI_BLE_RECEIVE_REMOTE_FEATURES,
	RSI_BLE_SCAN_RESTART_EVENT,
	RSI_APP_EVENT_DATA_LENGTH_CHANGE,
	RSI_APP_EVENT_PHY_UPDATE_COMPLETE,
	RSI_BLE_SMP_REQ_EVENT,
	RSI_BLE_SMP_RESP_EVENT,
	RSI_BLE_SMP_PASSKEY_EVENT,
	RSI_BLE_SMP_FAILED_EVENT,
	RSI_BLE_ENCRYPT_STARTED_EVENT,
	RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT,
	RSI_BLE_SC_PASSKEY_EVENT,
	RSI_BLE_LTK_REQ_EVENT,
	RSI_BLE_SECURITY_KEYS_EVENT,
	RSI_BLE_CONN_UPDATE_COMPLETE_EVENT,
	RSI_APP_EVENT_REMOTE_CONN_PARAM_REQ,
	RSI_BLE_CONN_UPDATE_REQ,
	RSI_BLE_GATT_ERROR,
	RSI_BLE_READ_REQ_EVENT,
	RSI_BLE_GATT_DESC_SERVICES,
	RSI_BLE_GATT_CHAR_SERVICES,
	RSI_BLE_GATT_PROFILE,
	RSI_BLE_GATT_PROFILES,
	RSI_BLE_REQ_GATT_PROFILE,
	RSI_BLE_GATT_WRITE_EVENT,
	RSI_BLE_GATT_PREPARE_WRITE_EVENT,
	RSI_BLE_GATT_EXECUTE_WRITE_EVENT,
	RSI_BLE_BUFF_CONF_EVENT,
	RSI_DATA_RECEIVE_EVENT,
	RSI_DATA_TRANSMIT_EVENT,
	RSI_CONN_UPDATE_REQ_EVENT,
	RSI_BLE_WRITE_EVENT_RESP,
	RSI_BLE_GATT_INDICATION_CONFIRMATION,
	RSI_BLE_REQ_WHITELIST,
	RSI_BLE_APP_USER_INPUT,
	RSI_BLE_SELECT_DATATRANSFER,
#if BLE_PROVISIONING_ENABLED
	RSI_APP_FW_VERSION,
	RSI_WLAN_ALREADY,
	RSI_WLAN_NOT_ALREADY,
	RSI_BLE_WLAN_DISCONN_NOTIFY,
	RSI_BLE_WLAN_TIMEOUT_NOTIFY,
	RSI_BLE_WLAN_DISCONNECT_STATUS,
	RSI_BLE_WLAN_SCAN_RESP,
	RSI_BLE_WLAN_JOIN_STATUS,
#endif
}rsi_ble_state_t;

typedef struct rsi_ble_conn_config_s
{
	uint8_t conn_id;
	bool smp_enable;
	bool add_to_whitelist;
	bool profile_discovery;
	bool data_transfer;
//	bool bidir_datatransfer;
    bool rx_notifications;
//    bool rx_write;
//    bool rx_write_no_response;
    bool rx_indications;
	bool tx_notifications;
    bool tx_write;
    bool tx_write_no_response;
    bool tx_indications;
    uint16_t	tx_write_clientservice_uuid;
    uint16_t	tx_write_client_char_uuid;
    uint16_t	tx_wnr_client_service_uuid;
    uint16_t	tx_wnr_client_char_uuid;
    uint16_t	rx_indi_client_service_uuid;
    uint16_t	rx_indi_client_char_uuid;
    uint16_t	rx_notif_client_service_uuid;
    uint16_t	rx_notif_client_char_uuid;
    struct buff_mode_sel_s
    {
    	bool buffer_mode;
    	uint8_t buffer_cnt;
    	uint8_t max_data_length;
    }buff_mode_sel;
    struct conn_param_update_s
    {
    	uint16_t conn_int;
    	uint8_t conn_latncy;
    	uint16_t supervision_to;
    }conn_param_update;
}rsi_ble_conn_config_t;

#if BLE_PROVISIONING_ENABLED
typedef enum rsi_app_cmd_e
{
	RSI_DATA = 0,
	RSI_WLAN_SCAN_RESP = 1,
	RSI_WLAN_CONN_STATUS = 2,
	RSI_WLAN_DISCONN_STATUS = 3,
	RSI_WLAN_DISCONN_NOTIFY = 4,
	RSI_WLAN_TIMEOUT_NOTIFY  =5
}rsi_app_cmd_t;
#endif

#if RESOLVE_ENABLE
#define RRIVACY_ENABLE                      0
#endif

#define SET_SMP_CONFIGURATION               0x01
#if SET_SMP_CONFIGURATION
#define LOCAL_OOB_DATA_FLAG_NOT_PRESENT 	( 0x00 )

#define AUTH_REQ_BONDING_BITS 				( ( 1 << 0 ) )
#define AUTH_REQ_MITM_BIT 					( 1 << 2 )
#define AUTH_REQ_SC_BIT 					( 1 << 3 )
#define AUTH_REQ_BITS						( AUTH_REQ_BONDING_BITS | AUTH_REQ_MITM_BIT | AUTH_REQ_SC_BIT )
#define MAXIMUM_ENC_KEY_SIZE_16				( 16 )

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H 3.6.1 Key Distribution and Generation
#define ENC_KEY_DIST 						( 1 << 0 )
#define ID_KEY_DIST 						( 1 << 1 )
#define SIGN_KEY_DIST 						( 1 << 2 )
#define LINK_KEY_DIST 						( 1 << 3 )
#if RESOLVE_ENABLE
#define RESPONDER_KEYS_TO_DIST 				( ENC_KEY_DIST | SIGN_KEY_DIST | ID_KEY_DIST)
#else
#define RESPONDER_KEYS_TO_DIST 				( ENC_KEY_DIST | SIGN_KEY_DIST)
#endif
#if RESOLVE_ENABLE
#define INITIATOR_KEYS_TO_DIST 				( ID_KEY_DIST | ENC_KEY_DIST | SIGN_KEY_DIST )
#else
#define INITIATOR_KEYS_TO_DIST 				( ENC_KEY_DIST | SIGN_KEY_DIST )
#endif
#endif


//! Notify status
#define NOTIFY_DISABLE  0x00
#define NOTIFY_ENABLE   0x01
//write status
#define WRITE_ENABLE   0x01
#define WRITE_DISABLE  0x00
//! Indication status
#define INDICATION_DISABLE             0x00
#define INDICATION_ENABLE              0x02
#define BIT64(a) 										((long long int)1 << a)
//! error codes
#define RSI_ERROR_IN_BUFFER_ALLOCATION					0x4e65
#define RSI_END_OF_PROFILE_QUERY						0x4a0a
#define RSI_TO_CHECK_ALL_CMD_IN_PROGRESS				0

/***********************************************************************************************************************************************/
//! Characteristic Presenatation Format Fields
/***********************************************************************************************************************************************/
#define RSI_BLE_UINT8_FORMAT                            0x04
#define RSI_BLE_EXPONENT                                0x00
#define RSI_BLE_PERCENTAGE_UNITS_UUID                   0x27AD
#define RSI_BLE_NAME_SPACE                              0x01
#define RSI_BLE_DESCRIPTION                             0x010B


/***********************************************************************************************************************************************/
//! Notification related macros
/***********************************************************************************************************************************************/
#define HEART_RATE_PROFILE_UUID                          0x180D       //! Heart rate profile UUID
#define HEART_RATE_MEASUREMENT                           0x2A37       //! Heart rate measurement
#define APPLE_NOTIFICATION_CENTER_SERVICE_UUID           0x7905F431   //! Apple notification center service UUID
#define APPLE_NOTIFICATION_SOURCE_UUID                   0x9FBF120D   //! Apple Notification source uuid

#define ENABLE_NOTIFICATION_SERVICE                      HEART_RATE_PROFILE_UUID

#if ENABLE_NOTIFICATION_SERVICE == HEART_RATE_PROFILE_UUID
#define ENABLE_NOTIFICATION_UUID						 HEART_RATE_MEASUREMENT
#elif ENABLE_NOTIFICATION_SERVICE == APPLE_NOTIFICATION_CENTER_SERVICE_UUID
#define ENABLE_NOTIFICATION_UUID						 APPLE_NOTIFICATION_SOURCE_UUID
#endif

//! BLE characteristic custom service uuid
#define  RSI_BLE_CUSTOM_SERVICE_UUID                   0x1AA1
#define  RSI_BLE_CUSTOM_LEVEL_UUID                     0x1BB1


/***********************************************************************************************************************************************/
//! BLE client characteristic service and attribute uuid.
/***********************************************************************************************************************************************/
#define  RSI_BLE_NEW_CLIENT_SERVICE_UUID                0x180D                          //! immediate alert service uuid
#define  RSI_BLE_CLIENT_ATTRIBUTE_1_UUID                0x2A37                          //! Alert level characteristic 

#define  BLE_ATT_REC_SIZE                               500
#define  NO_OF_VAL_ATT                                  5                               //! Attribute value count

/***********************************************************************************************************************************************/
//! user defined structure
/***********************************************************************************************************************************************/
typedef struct rsi_ble_att_list_s {
	uuid_t char_uuid;
	uint16_t handle;
	uint16_t value_len;
	uint16_t max_value_len;
	uint8_t char_val_prop;
	void *value;
} rsi_ble_att_list_t;

typedef struct rsi_ble_s {
	uint8_t DATA[BLE_ATT_REC_SIZE];
	uint16_t DATA_ix;
	uint16_t att_rec_list_count;
	rsi_ble_att_list_t att_rec_list[NO_OF_VAL_ATT];
} rsi_ble_t;


/***********************************************************************************************************************************************/
//! Advertising command parameters
/***********************************************************************************************************************************************/
#define  RSI_BLE_ADV_INT_MIN                         0x152//0x06a8
#define  RSI_BLE_ADV_INT_MAX                         0x152//0x06a8


/***********************************************************************************************************************************************/
//! Connection parameters
/***********************************************************************************************************************************************/
#define LE_SCAN_INTERVAL                             0x0035
#define LE_SCAN_WINDOW                               0x0017

#define CONNECTION_INTERVAL_MIN                      0x00C8//0x00A0
#define CONNECTION_INTERVAL_MAX                      0x00C8//0x00A0

#define SUPERVISION_TIMEOUT                          0x0C80

#define LE_SCAN_INTERVAL_CONN                        0x0050
#define LE_SCAN_WINDOW_CONN                          0x0050

/***********************************************************************************************************************************************/
//! Connection parameters for RSI as master to remote device as slave connection
/***********************************************************************************************************************************************/
#define M2S12_CONNECTION_INTERVAL_MIN                0x00C8
#define M2S12_CONNECTION_INTERVAL_MAX                0x00C8

#define M2S12_CONNECTION_LATENCY                     0x0000
#define M2S12_SUPERVISION_TIMEOUT                    (4 * M2S12_CONNECTION_INTERVAL_MIN)

/***********************************************************************************************************************************************/

#define RSI_OPERMODE_WLAN_BLE                       9//13//9//(0x109)

#include "rsi_ble_common_config.h"
#endif /* SAPIS_EXAMPLES_RSI_DEMO_APPS_INC_RSI_BLE_COMMON_BLE_MULTI_SLAVE_MASTER_H */
