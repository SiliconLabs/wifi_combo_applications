/*******************************************************************************
 * @file  rsi_ble_config.h
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef RSI_BLE_CONFIG_H
#define RSI_BLE_CONFIG_H

#include <rsi_data_types.h>

/******************************************************
 * *                      Macros
 * ******************************************************/

#define RSI_BLE_SET_RAND_ADDR         "00:23:A7:12:34:56"

#define CLEAR_WHITELIST               0x00
#define ADD_DEVICE_TO_WHITELIST       0x01
#define DELETE_DEVICE_FROM_WHITELIST  0x02

#define ALL_PHYS                      0x00

#define RSI_BLE_DEV_ADDR_RESOLUTION_ENABLE 0

#define RSI_OPERMODE_WLAN_BLE         13

#ifdef RSI_M4_INTERFACE
#define RSI_BLE_MAX_NBR_ATT_REC       20
#define RSI_BLE_MAX_NBR_SLAVES        1
#define RSI_BLE_NUM_CONN_EVENTS       2
#else
#define RSI_BLE_MAX_NBR_ATT_REC       80
#define RSI_BLE_MAX_NBR_SLAVES        1
#define RSI_BLE_NUM_CONN_EVENTS       30
#endif
#define RSI_BLE_MAX_NBR_ATT_SERV      10

#define RSI_BLE_MAX_NBR_MASTERS       1
#define RSI_BLE_GATT_ASYNC_ENABLE     1
#define RSI_BLE_GATT_INIT             0

/* Number of BLE GATT RECORD SIZE IN (n*16 BYTES), eg:(0x40*16)=1024 bytes */
#define RSI_BLE_NUM_REC_BYTES         0x40

#define RSI_BLE_INDICATE_CONFIRMATION_FROM_HOST 0

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN        15000

//! local device IO capability
#define RSI_BLE_SMP_IO_CAPABILITY 0x03

//! enabling the security
#define SMP_ENABLE                1
/*=======================================================================*/
//! THROUGHPUT TYPE
/*=======================================================================*/

#define NOTIFICATIONS             0
#define INDICATIONS               1

#define THROUGHPUT_TYPE           NOTIFICATIONS
/*=======================================================================*/
//! application event list
/*=======================================================================*/
#define RSI_BLE_CONN_EVENT                          0x01
#define RSI_BLE_DISCONN_EVENT                       0x02
#if SMP_ENABLE
#define RSI_BLE_SMP_REQ_EVENT                       0x03
#define RSI_BLE_SMP_RESP_EVENT                      0x04
#define RSI_BLE_SMP_PASSKEY_EVENT                   0x05
#define RSI_BLE_SMP_FAILED_EVENT                    0x06
#define RSI_BLE_ENCRYPT_STARTED_EVENT               0x07
#define RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT           0x08
#define RSI_BLE_SC_PASSKEY_EVENT                    0x09
#define RSI_BLE_LTK_REQ_EVENT                       0x0A
#endif
#define RSI_BLE_RECEIVE_REMOTE_FEATURES             0x0B
#define RSI_APP_EVENT_DATA_LENGTH_CHANGE            0x0C
#define RSI_APP_EVENT_PHY_UPDATE_COMPLETE           0x0D
#define RSI_BLE_CONN_UPDATE_EVENT                   0x0E
#define RSI_BLE_MTU_EVENT                           0x0F
#define RSI_BLE_GATT_WRITE_EVENT                    0x10
#define RSI_BLE_MORE_DATA_REQ_EVENT                 0x11
#define RSI_DATA_TRANSMIT_EVENT                     0x12
#define RSI_TRANSMISSION_NOTIFICATION_ENABLE_EVENT  0x13
#define RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT 0x14
#define RSI_CONNECTION_PARAMETERS_EVENT             0x15

//! local device name
#define RSI_BLE_DEVICE_NAME         (void *)"Throughput Test"

#define MITM_REQ                    0x01

#define MAX_MTU_SIZE                240

#define RSI_DEV_ADDR_LEN            6

#define LAST_BYTE                   0x64

#define START_BYTE                  0x00

#define UUID_SIZE                   16
//! error code
#define BT_HCI_COMMAND_DISALLOWED   0x4E0C

#define RSI_BLE_CLIENT_CHAR_DESP    0x2901

/*=======================================================================*/
//! Advertising command parameters
/*=======================================================================*/

#define RSI_BLE_ADV_TYPE              UNDIR_CONN
#define RSI_BLE_ADV_FILTER_TYPE       ALLOW_SCAN_REQ_ANY_CONN_REQ_ANY
#define RSI_BLE_ADV_DIR_ADDR_TYPE     LE_PUBLIC_ADDRESS
#define RSI_BLE_ADV_DIR_ADDR          "00:15:83:6A:64:17"

#define RSI_BLE_ADV_INT_MIN           0x100
#define RSI_BLE_ADV_INT_MAX           0x200
#define RSI_BLE_ADV_CHANNEL_MAP       0x07

//! Advertise status
//! Start the advertising process
#define RSI_BLE_START_ADV             0x01
//! Stop the advertising process
#define RSI_BLE_STOP_ADV              0x00

//! BLE Tx Power Index On Air
#define RSI_BLE_PWR_INX               30

//! BLE Active H/w Pwr Features
#define BLE_DISABLE_DUTY_CYCLING      0
#define BLE_DUTY_CYCLING              1
#define BLR_DUTY_CYCLING              2
#define BLE_4X_PWR_SAVE_MODE          4
#define RSI_BLE_PWR_SAVE_OPTIONS      BLE_DISABLE_DUTY_CYCLING

//! Advertise types

/* Advertising will be visible(discoverable) to all the devices.
 * Scanning/Connection is also accepted from all devices
 * */
#define UNDIR_CONN    0x80

/* Advertising will be visible(discoverable) to the particular device
 * mentioned in RSI_BLE_ADV_DIR_ADDR only.
 * Scanning and Connection will be accepted from that device only.
 * */
#define DIR_CONN      0x81

/* Advertising will be visible(discoverable) to all the devices.
 * Scanning will be accepted from all the devices.
 * Connection will be not be accepted from any device.
 * */
#define UNDIR_SCAN    0x82

/* Advertising will be visible(discoverable) to all the devices.
 * Scanning and Connection will not be accepted from any device
 * */
#define UNDIR_NON_CONN 0x83

/* Advertising will be visible(discoverable) to the particular device
 * mentioned in RSI_BLE_ADV_DIR_ADDR only.
 * Scanning and Connection will be accepted from that device only.
 * */
#define DIR_CONN_LOW_DUTY_CYCLE 0x84

//! Advertising flags
#define LE_LIMITED_DISCOVERABLE 0x01
#define LE_GENERAL_DISCOVERABLE 0x02
#define LE_BR_EDR_NOT_SUPPORTED 0x04

//! Advertise filters
#define ALLOW_SCAN_REQ_ANY_CONN_REQ_ANY               0x00
#define ALLOW_SCAN_REQ_WHITE_LIST_CONN_REQ_ANY        0x01
#define ALLOW_SCAN_REQ_ANY_CONN_REQ_WHITE_LIST        0x02
#define ALLOW_SCAN_REQ_WHITE_LIST_CONN_REQ_WHITE_LIST 0x03

//! Address types
#define LE_PUBLIC_ADDRESS             0x00
#define LE_RANDOM_ADDRESS             0x01
#define LE_RESOLVABLE_PUBLIC_ADDRESS  0x02
#define LE_RESOLVABLE_RANDOM_ADDRESS  0x03

/*=======================================================================*/

/*=======================================================================*/
//! Connection parameters
/*=======================================================================*/
#define LE_SCAN_INTERVAL          0x0100
#define LE_SCAN_WINDOW            0x0050

#define CONNECTION_INTERVAL_MIN   0x00A0
#define CONNECTION_INTERVAL_MAX   0x00A0

#define CONNECTION_LATENCY        0x0000
/*=======================================================================*/

/*=======================================================================*/
//! Scan command parameters
/*=======================================================================*/

#define RSI_BLE_SCAN_TYPE         SCAN_TYPE_ACTIVE
#define RSI_BLE_SCAN_FILTER_TYPE  SCAN_FILTER_TYPE_ALL

//! Scan status
#define RSI_BLE_START_SCAN                0x01
#define RSI_BLE_STOP_SCAN                 0x00

//! Scan types
#define SCAN_TYPE_ACTIVE                  0x01
#define SCAN_TYPE_PASSIVE                 0x00

//! Scan filters
#define SCAN_FILTER_TYPE_ALL              0x00
#define SCAN_FILTER_TYPE_ONLY_WHITE_LIST  0x01

#define RSI_SEL_INTERNAL_ANTENNA          0x00
#define RSI_SEL_EXTERNAL_ANTENNA          0x01
/*=======================================================================*/

//! Remote device bd address
#define RSI_BLE_REMOTE_DEV_ADDR "00:12:45:AB:1D:32"
//! Remote device name
#define RSI_REMOTE_DEVICE_NAME "Note10"

#define CONN_BY_ADDR 1
#define CONN_BY_NAME 2

#define CONNECT_OPTION CONN_BY_NAME

//! connection update params
#define CONN_INTERVAL_MIN           0x08
#define CONN_INTERVAL_MAX           0x08
#define CONN_INTERVAL_DEFAULT_MIN   0xC8
#define CONN_INTERVAL_DEFAULT_MAX   0xC8
#define CONN_LATENCY                0
#define SUPERVISION_TIMEOUT         800

//! Tx Datalength parameters
#define TX_LEN  0xFB
#define TX_TIME 0x0148
// DATA RATE:
//  0x02 - 2Mbps
//  0x01 - 1Mbps
//  0x04 - Coded PHY
//! Phy parameter
#define TX_PHY_RATE    0x02 // 0x01//0x04
#define RX_PHY_RATE    0x02 // 0x01//0x04
// CODED_PHY_RATE: 0x01
#define CODED_PHY_RATE 0x00

//! Notify status
#define NOTIFY_DISABLE  0x00
#define NOTIFY_ENABLE   0x01

//! Indication status
#define INDICATION_DISABLE  0x00
#define INDICATION_ENABLE   0x02

#define DLE_ON 1

#if DLE_ON
#define DLE_BUFFER_MODE       1
#define DLE_BUFFER_COUNT      6
#define RSI_BLE_MAX_DATA_LEN  232
#else
#define DLE_BUFFER_MODE 0
#define DLE_BUFFER_COUNT 2
#define RSI_BLE_MAX_DATA_LEN 20

#endif
#endif

void data_transfer(uint8_t transfer_on);
void rsi_ble_app_set_event(uint32_t event_num);
int rsi_throughput_ble_app(void);
void rsi_button_config(void);
