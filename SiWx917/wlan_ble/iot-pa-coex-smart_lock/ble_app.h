/*******************************************************************************
 * @file  ble_app.h
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
/*************************************************************************
 *
 */

#ifndef BLE_APP_H_
#define BLE_APP_H_

// application event list
#define RSI_BLE_ENH_CONN_EVENT 0x01
#define RSI_BLE_DISCONN_EVENT  0x02
#define RSI_BLE_WLAN_SCAN_RESP 0x03

#define RSI_SSID                        0x0D
#define RSI_SECTYPE                     0x0E
#define RSI_BLE_WLAN_DISCONN_NOTIFY     0x0F
#define RSI_WLAN_ALREADY                0x10
#define RSI_WLAN_NOT_ALREADY            0x11
#define RSI_BLE_WLAN_TIMEOUT_NOTIFY     0x12
#define RSI_APP_FW_VERSION              0x13
#define RSI_BLE_WLAN_DISCONNECT_STATUS  0x14
#define RSI_BLE_WLAN_JOIN_STATUS        0x15
#define RSI_BLE_MTU_EVENT               0x16
#define RSI_BLE_CONN_UPDATE_EVENT       0x17
#define RSI_BLE_RECEIVE_REMOTE_FEATURES 0x18
#define RSI_BLE_DATA_LENGTH_CHANGE      0x20
#define RSI_APP_SMART_QUERY_STATE       0x19
#define RSI_BLE_ADVERTISE               0x5
#define RSI_APP_SMART_LOCK              0x1A
#define RSI_APP_SMART_UNLOCK            0x1B
#define RSI_BLE_STOP_ADVERTISE          0x1C
#define RSI_BLE_DISCONN_REMOTE_DEVICE   0x1D

//! GATT WRITE ID's for WLAN
#define RSI_BLE_CMD_WLAN_SCAN_REQ       0x33
#define RSI_BLE_CMD_WLAN_SSID           0x32
#define RSI_BLE_CMD_WLAN_SEC_TYPE       0x35
#define RSI_BLE_CMD_WLAN_PSK            0x36
#define RSI_BLE_CMD_WLAN_STATUS_REQ     0x37
#define RSI_BLE_CMD_WLAN_DISCONN_REQ    0x34
#define RSI_BLE_CMD_WLAN_FW_VERSION_REQ 0x38

#define RSI_BLE_CMD_SMART_QUERY_STATE   0x39
#define RSI_BLE_CMD_SMART_UNLOCK        0x0
#define RSI_BLE_CMD_SMART_LOCK          0x01

// attribute properties
#define RSI_BLE_ATT_PROPERTY_READ   0x02
#define RSI_BLE_ATT_PROPERTY_WRITE  0x08
#define RSI_BLE_ATT_PROPERTY_NOTIFY 0x10
#define RSI_BLE_ATT_PROPERTY_INDICATE 0x20

// BLE attribute service types uuid values
#define RSI_BLE_CHAR_SERV_UUID   0x2803
#define RSI_BLE_CLIENT_CHAR_UUID 0x2902

#define DEVNAME_LEN 16
#define UUID_LEN    2   // 16-bit UUID length

/**
 * @brief System status structure containing connection and operational states
 */
typedef struct {
    uint8_t lock_status;  ///< Device lock status (locked/unlocked)
    bool ble_connect;     ///< BLE connection status
    bool wifi_connect;    ///< WiFi connection status
    bool ble_ops;         ///< BLE operations active flag
    bool wifi_ops;        ///< WiFi operations active flag
} SystemStatus_t;

typedef struct {
  uint8_t flags_len;                // Length of the Flags field.
  uint8_t flags_type;               // Type of the Flags field.
  uint8_t flags;                    // Flags field.
  uint8_t short_name_len;           // Length of Shortened Local Name.
  uint8_t short_name_type;          // Shortened Local Name.
  uint8_t short_name[DEVNAME_LEN];  // Shortened Local Name.
  uint8_t uuid_length;              // Length of UUID.
  uint8_t uuid_type;                // Type of UUID.
  uint8_t uuid[UUID_LEN];           // 128-bit UUID.
} response_data_t;

void rsi_ble_app_set_event(uint32_t event_num);
void rsi_ble_app_clear_event(uint32_t event_num);
void rsi_ble_configurator_task(void *argument);
void rsi_ble_configurator_init(void);
void send_initial_lock_state_to_ble(void);
void send_lock_state_to_ble(void);
void send_unlock_state_to_ble(void);
void send_button_activity_to_ble(void);
//void send_lock_state(void);
#endif /* BLE_APP_H_ */
