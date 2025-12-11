/*******************************************************************************
* @file  ble_app.c
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

/*================================================================================
 * @brief : This file contains example application for WiFi Station BLE
 * Provisioning
 * @section Description :
 * This application explains how to get the WLAN connection functionality using
 * BLE provisioning.
 * Silicon Labs Module starts advertising and with BLE Provisioning the Access Point
 * details are fetched.
 * Silicon Labs device is configured as a WiFi station and connects to an Access Point.
 =================================================================================*/

/**
 * Include files
 * */

//! SL Wi-Fi SDK includes
#include "sl_board_configuration.h"
#include "sl_constants.h"
#include "sl_wifi.h"
#include "sl_net_ip_types.h"
#include "cmsis_os2.h"
#include "sl_utility.h"
// BLE include file to refer BLE APIs
#include <rsi_ble_apis.h>
#include <rsi_bt_common_apis.h>
#include <rsi_common_apis.h>
#include <string.h>

#include "ble_config.h"
#include "wifi_config.h"
#include <wifi_app.h>
#include <ble_app.h>
#include <motor.h>
#include <app.h>
#include <led.h>
#include <nvm3_flash.h>
#include "timer.h"

#if DISPLAY
#include "lcd_logic.h"
#endif

// BLE characteristic service uuid
#define RSI_BLE_NEW_SERVICE_UUID 0xAABB
#define RSI_BLE_ATTRIBUTE_1_UUID 0x1AA1
#define RSI_BLE_ATTRIBUTE_2_UUID 0x1BB1
#define RSI_BLE_ATTRIBUTE_3_UUID 0x1CC1
// local device name
#define RSI_BLE_APP_DEVICE_NAME "BLE_CONFIGURATOR"

#define RSI_FW_VERSION 0x01

// Maximum length of SSID
#define RSI_SSID_LEN 34
// MAC address length
#define RSI_MAC_ADDR_LEN 6
// Maximum Acccess points that can be scanned
#define RSI_AP_SCANNED_MAX 11

#define RSI_BLE_MAX_DATA_LEN 66 ///< max data length

//! connection update param
#define CONN_INTERVAL_MIN           0x320 //0x08
#define CONN_INTERVAL_MAX           0x320 //0x08
#define CONN_INTERVAL_DEFAULT_MIN   0xC8
#define CONN_INTERVAL_DEFAULT_MAX   0xC8
#define SUPERVISION_TIMEOUT_DEFAULT 400
#define CONN_LATENCY                0

#define UUID_SIZE 16
/********************************************************************************************************
 *                                               DATA TYPES
 *********************************************************************************************************
 */

// global parameters list

uuid_t dev_serv = {0};
uuid_t dev_lock_char = {0};
uuid_t source_char = {0};
uuid_t addr_char = {0};
rsi_ble_resp_add_serv_t dev_serv_response = {0};
uint16_t dev_lock_handle;
uint16_t lock_source_handle;
uint16_t addr_handle;

//uint8_t data[20] = { 0 };
static volatile uint32_t ble_app_event_map;
rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static uint8_t rsi_ble_att1_val_hndl;
static uint16_t rsi_ble_att2_val_hndl;
uint16_t rsi_ble_att3_val_hndl;
uint16_t rsi_ble_att4_val_hndl;
uint16_t rsi_ble_att5_val_hndl;
extern ap_cred_t ap_credentials;
uint8_t remote_dev_addr[18] = { 0 };
static rsi_ble_event_mtu_t app_ble_mtu_event;
static rsi_ble_event_conn_update_t event_conn_update_complete;
static rsi_ble_event_remote_features_t remote_dev_feature;
static rsi_ble_event_data_length_update_t updated_data_len_params;
extern SystemStatus_t SystemStatus;
extern char temp_string;
bool new_cred;

sl_wifi_scan_result_t *scanresult = NULL;

volatile bool is_ble_advertising = false;
extern Source_operation_t SoP;
extern uint8_t connected, disassosiated;
extern uint8_t retry;
extern sl_net_ip_configuration_t ip_address;
extern uint8_t magic_word;
// Creating a global variable for lock unlock status
extern sl_gpio_t gpio_port_pin_1;
#include "sl_driver_gpio.h"
#include "sl_si91x_rgb_led_instances.h"
extern osSemaphoreId_t ble_thread_sem;
extern bool send_lock_state;
extern uint16_t scanbuf_size;
extern Source_operation_t SoP;
bool ble_connected = false;
extern bool erase_from_mqtt;
extern bool mqtt_disconnected;
extern osSemaphoreId_t data_received_semaphore;
/******************************************************
 *               Function Declarations
 ******************************************************/
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn);
void init_dev_service(void);
void rsi_ble_add_dev_serv(void);
static void rsi_ble_add_dev_lock_char(void);
static void rsi_ble_add_source_char(void);
static void rsi_ble_add_address_char(void);
static void rsi_ble_prepare_128bit_uuid(uint8_t temp_service[UUID_SIZE],
                                        uuid_t *temp_uuid);
static void rsi_ble_add_char_val_att_custom(void *serv_handler, uint16_t handle,
                                     uuid_t att_type_uuid, uint8_t val_prop,
                                     uint8_t *data, uint8_t data_len,
                                     uint8_t auth_read);
static void rsi_ble_add_char_serv_att_custom(void *serv_handler, uint16_t handle,
                                      uint8_t val_prop, uint16_t att_val_handle,
                                      uuid_t att_val_uuid);




/*==============================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute
 * @param[in]  serv_handler, service handler.
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  att_val_handle, characteristic value handle
 * @param[in]  att_val_uuid, characteristic value uuid
 * @return     none.
 * @section description
 * This function is used at application to add characteristic attribute
 */
static void rsi_ble_add_char_serv_att(void *serv_handler,
                                      uint16_t handle,
                                      uint8_t val_prop,
                                      uint16_t att_val_handle,
                                      uuid_t att_val_uuid)
{
  rsi_ble_req_add_att_t new_att = { 0 };

  // preparing the attribute service structure
  new_att.serv_handler       = serv_handler;
  new_att.handle             = handle;
  new_att.att_uuid.size      = 2;
  new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
  new_att.property           = RSI_BLE_ATT_PROPERTY_READ;

  // preparing the characteristic attribute value
  new_att.data_len = 6;
  new_att.data[0]  = val_prop;
  rsi_uint16_to_2bytes(&new_att.data[2], att_val_handle);
  rsi_uint16_to_2bytes(&new_att.data[4], att_val_uuid.val.val16);

  // add attribute to the service
  rsi_ble_add_attribute(&new_att);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_val_att
 * @brief      this function is used to add characteristic value attribute.
 * @param[in]  serv_handler, new service handler.
 * @param[in]  handle, characteristic value attribute handle.
 * @param[in]  att_type_uuid, attribute uuid value.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  data, characteristic value data pointer.
 * @param[in]  data_len, characteristic value length.
 * @return     none.
 * @section description
 * This function is used at application to create new service.
 */

static void rsi_ble_add_char_val_att(void *serv_handler,
                                     uint16_t handle,
                                     uuid_t att_type_uuid,
                                     uint8_t val_prop,
                                     uint8_t *data,
                                     uint8_t data_len)
{
  rsi_ble_req_add_att_t new_att = { 0 };

  // preparing the attributes
  new_att.serv_handler = serv_handler;
  new_att.handle       = handle;
  memcpy(&new_att.att_uuid, &att_type_uuid, sizeof(uuid_t));
  new_att.property = val_prop;

  // preparing the attribute value
  new_att.data_len = RSI_MIN(sizeof(new_att.data), data_len);
  memcpy(new_att.data, data, new_att.data_len);

  // add attribute to the service
  rsi_ble_add_attribute(&new_att);

  // check the attribute property with notification
  if (val_prop & RSI_BLE_ATT_PROPERTY_NOTIFY) {
    // if notification property supports then we need to add client characteristic service.

    // preparing the client characteristic attribute & values
    memset(&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler       = serv_handler;
    new_att.handle             = handle + 1;
    new_att.att_uuid.size      = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
    new_att.property           = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE;
    new_att.data_len           = 2;

    // add attribute to the service
    rsi_ble_add_attribute(&new_att);
  }

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_chat_add_new_serv
 * @brief      this function is used to add new service i.e., simple chat service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */
static uint32_t rsi_ble_add_configurator_serv(void)
{
  uuid_t new_uuid                       = { 0 };
  rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
  uint8_t data[RSI_BLE_MAX_DATA_LEN]    = { 0 };

  new_uuid.size      = 2; // adding new service
  new_uuid.val.val16 = RSI_BLE_NEW_SERVICE_UUID;

  rsi_ble_add_service(new_uuid, &new_serv_resp);

  new_uuid.size      = 2; // adding characteristic service attribute to the service
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 1,
                            RSI_BLE_ATT_PROPERTY_WRITE,
                            new_serv_resp.start_handle + 2,
                            new_uuid);

  rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2; // adding characteristic value attribute to the service
  new_uuid.size         = 2;
  new_uuid.val.val16    = RSI_BLE_ATTRIBUTE_1_UUID;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 2,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_WRITE,
                           data,
                           RSI_BLE_MAX_DATA_LEN);

  new_uuid.size      = 2; // adding characteristic service attribute to the service
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 3,
                            RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE,
                            new_serv_resp.start_handle + 4,
                            new_uuid);

  rsi_ble_att2_val_hndl = new_serv_resp.start_handle + 4; // adding characteristic value attribute to the service
  new_uuid.size         = 2;
  new_uuid.val.val16    = RSI_BLE_ATTRIBUTE_2_UUID;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 4,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE,
                           data,
                           RSI_BLE_MAX_DATA_LEN);

  new_uuid.size      = 2; // adding characteristic service attribute to the service
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_3_UUID;
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 5,
                            RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
                            new_serv_resp.start_handle + 6,
                            new_uuid);

  rsi_ble_att3_val_hndl = new_serv_resp.start_handle + 6; // adding characteristic value attribute to the service
  new_uuid.size         = 2;
  new_uuid.val.val16    = RSI_BLE_ATTRIBUTE_3_UUID;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 6,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
                           data,
                           RSI_BLE_MAX_DATA_LEN);


  return 0;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events()
{
  ble_app_event_map = 0;
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      sets the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
void rsi_ble_app_set_event(uint32_t event_num)
{
  ble_app_event_map |= BIT(event_num);

  osSemaphoreRelease(ble_thread_sem);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
void rsi_ble_app_clear_event(uint32_t event_num)
{
  ble_app_event_map &= ~BIT(event_num);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_ble_app_get_event(void)
{
  uint32_t ix;

  for (ix = 0; ix < 32; ix++) {
    if (ble_app_event_map & (1 << ix)) {
      return ix;
    }
  }

  return (-1);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_enh_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy(conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  conn_event_to_app.status = resp_enh_conn->status;
  rsi_ble_app_set_event(RSI_BLE_ENH_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_connect_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
  memcpy(remote_dev_addr, resp_conn->dev_addr, 6);
  memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event(RSI_BLE_ENH_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[out]  resp_disconnect, disconnected remote device information
 * @param[out]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  UNUSED_PARAMETER(reason);
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_disconnect_t));
  rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_conn_update_complete_event
 * @brief      invoked when conn update complete event is received
 * @param[out] rsi_ble_event_conn_update_complete contains the controller
 * support conn information.
 * @param[out] resp_status contains the response status (Success or Error code)
 * @return     none.
 * @section description
 * This Callback function indicates the conn update complete event is received
 */
void rsi_ble_on_conn_update_complete_event(rsi_ble_event_conn_update_t *rsi_ble_event_conn_update_complete,
                                           uint16_t resp_status)
{
  UNUSED_PARAMETER(resp_status);
  rsi_6byte_dev_address_to_ascii(remote_dev_addr, (uint8_t *)rsi_ble_event_conn_update_complete->dev_addr);
  memcpy(&event_conn_update_complete, rsi_ble_event_conn_update_complete, sizeof(rsi_ble_event_conn_update_t));
  rsi_ble_app_set_event(RSI_BLE_CONN_UPDATE_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_remote_features_event
 * @brief      invoked when LE remote features event is received.
 * @param[out] rsi_ble_event_remote_features, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the remote device features
 */
void rsi_ble_on_remote_features_event(rsi_ble_event_remote_features_t *rsi_ble_event_remote_features)
{
  memcpy(&remote_dev_feature, rsi_ble_event_remote_features, sizeof(rsi_ble_event_remote_features_t));
  rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
}
/*============================================================================*/
/**
 * @fn         rsi_ble_data_length_change_event
 * @brief      invoked when data length is set
 * @param[out] rsi_ble_data_length_update, data length information
 * @section description
 * This Callback function indicates data length is set
 */
void rsi_ble_data_length_change_event(rsi_ble_event_data_length_update_t *rsi_ble_data_length_update)
{
  memcpy(&updated_data_len_params, rsi_ble_data_length_update, sizeof(rsi_ble_event_data_length_update_t));
  rsi_ble_app_set_event(RSI_BLE_DATA_LENGTH_CHANGE);
}
/*==============================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      invoked  when an MTU size event is received
 * @param[out]  rsi_ble_mtu, it indicates MTU size.
 * @return     none.
 * @section description
 * This callback function is invoked  when an MTU size event is received
 */
static void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *rsi_ble_mtu)
{
  memcpy(&app_ble_mtu_event, rsi_ble_mtu, sizeof(rsi_ble_event_mtu_t));
  rsi_6byte_dev_address_to_ascii(remote_dev_addr, app_ble_mtu_event.dev_addr);
  rsi_ble_app_set_event(RSI_BLE_MTU_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      this is call back function, it invokes when write/notify events received.
 * @param[out]  event_id, it indicates write/notification event id.
 * @param[out]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This is a callback function
 */
static void rsi_ble_on_gatt_write_event(uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  UNUSED_PARAMETER(event_id);
  uint16_t cmdid;

  //  Requests will come from Mobile app
  if (((rsi_ble_att1_val_hndl) == *((uint16_t *)rsi_ble_write->handle)) || ((dev_lock_handle) == *((uint16_t *)rsi_ble_write->handle)) ){
    cmdid = rsi_ble_write->att_value[0];

    if(cmdid == 0x31 || cmdid == 0x30){
    memcpy(&cmdid,rsi_ble_write->att_value,2);
    }

    switch (cmdid) {

      // lock request
      case RSI_BLE_CMD_SMART_LOCK:
      {
        rsi_ble_app_set_event (RSI_APP_SMART_LOCK);
      }
      break;
      // unlock request
      case RSI_BLE_CMD_SMART_UNLOCK:
      {
        rsi_ble_app_set_event (RSI_APP_SMART_UNLOCK);
      }
      break;

      // Scan command request
      case RSI_BLE_CMD_WLAN_SCAN_REQ:
      {
        LOG_PRINT("Received scan request\r\n");
        retry = 0;
        wifi_app_set_event(WIFI_APP_SCAN_STATE);
      } break;

      // Sending SSID
      case RSI_BLE_CMD_WLAN_SSID:
      {
        memset(&ap_credentials, 0, sizeof(ap_cred_t));
        strcpy((char *)ap_credentials.ssid, (const char *)&rsi_ble_write->att_value[3]);
        erase_from_mqtt = false;
        rsi_ble_app_set_event(RSI_SSID);
      } break;

      // Sending Security type
      case RSI_BLE_CMD_WLAN_SEC_TYPE:
      {
        ap_credentials.sec_type = ((rsi_ble_write->att_value[3]) - '0');
        new_cred=true;
        rsi_ble_app_set_event(RSI_SECTYPE);
      } break;

      // Sending PSK
      case RSI_BLE_CMD_WLAN_PSK:
      {
        strcpy((char *)ap_credentials.pwd, (const char *)&rsi_ble_write->att_value[3]);
        new_cred=true;
        wifi_app_set_event(WIFI_APP_JOIN_STATE);
      } break;

      // WLAN Status Request
      case RSI_BLE_CMD_WLAN_STATUS_REQ:
      {
        LOG_PRINT("WLAN status request received\r\n");
        if (connected) {
          rsi_ble_app_set_event(RSI_WLAN_ALREADY);
        } else {
          rsi_ble_app_set_event(RSI_WLAN_NOT_ALREADY);
        }
      } break;

      // WLAN disconnect request
      case RSI_BLE_CMD_WLAN_DISCONN_REQ:
      {
        LOG_PRINT("WLAN disconnect request received\r\n");
        wifi_app_set_event(WIFI_APP_DISCONN_NOTIFY_STATE);
      } break;

      // FW version request
      case RSI_BLE_CMD_WLAN_FW_VERSION_REQ: {
        rsi_ble_app_set_event(RSI_APP_FW_VERSION);
        LOG_PRINT("FW version request\r\n");
      } break;

      //RSI_BLE_CMD_SMART_QUERY_STATE
      case RSI_BLE_CMD_SMART_QUERY_STATE:{
        rsi_ble_app_set_event(RSI_APP_SMART_QUERY_STATE);
      }break;

      default:
        LOG_PRINT("Default command case \r\n\n");
        break;
    }
  }
}

void send_initial_lock_state_to_ble(void)
{
  rsi_ble_set_local_att_value(dev_lock_handle, 1, &SystemStatus.lock_status);
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_init
 * @brief      initialize the BLE module.
 * @param[in]  none
 * @return     none.
 * @section description
 * This function is used to initialize the BLE module
 */
void rsi_ble_configurator_init(void)
{
  //  initializing the application events map
  rsi_ble_app_init_events();

  rsi_ble_add_configurator_serv(); // adding simple BLE chat service
  init_dev_service();        // Adding new services for dmp.

  // registering the GAP callback functions
  rsi_ble_gap_register_callbacks(NULL,
                                 rsi_ble_on_connect_event,
                                 rsi_ble_on_disconnect_event,
                                 NULL,
                                 NULL,
                                 rsi_ble_data_length_change_event,
                                 rsi_ble_on_enhance_conn_status_event,
                                 NULL,
                                 rsi_ble_on_conn_update_complete_event,
                                 NULL);
  //! registering the GAP extended call back functions
  rsi_ble_gap_extended_register_callbacks(rsi_ble_on_remote_features_event, NULL);

  // registering the GATT callback functions
  rsi_ble_gatt_register_callbacks(NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  rsi_ble_on_gatt_write_event,
                                  NULL,
                                  NULL,
                                  NULL,
                                  rsi_ble_on_mtu_event,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL);

  // Set local name
  rsi_bt_set_local_name((uint8_t *)RSI_BLE_APP_DEVICE_NAME);

  uint8_t ble_mac[15]={0};
  rsi_bt_get_local_device_address(ble_mac);

  static response_data_t response_data = {
      // AD structure #1: Flags
      2,           // length (Type + 1 data byte)
      0x01,        // AD Type: Flags
      0x04 | 0x02, // Value: LE General Discoverable (0x02) | BR/EDR Not Supported (0x04) = 0x06

      // AD structure #2: Complete Local Name
      DEVNAME_LEN +1,  // length = Type(1) + name bytes(16) = 17 (0x11)
      0x09,             // AD Type: Complete Local Name (0x09)
      { 'B','L','E','_','C','O','N','F','I','G','U','R','A','T','O','R'},

      // AD structure #3: Complete List of 16-bit Service Class UUIDs
      UUID_LEN + 1,     // length = Type(1) + UUID(2) = 3 (0x03)
      0x03,             // AD Type: Complete List of 16-bit UUIDs (0x03)
      { 0x63, 0xFD }    // UUID 0xFD63 in little-endian
    };


  rsi_ble_set_advertise_data((uint8_t *)&response_data, sizeof(response_data));


  //set device in advertising mode.
  rsi_ble_app_set_event(RSI_BLE_ADVERTISE);
  if(read_credentials_from_nvm(&ap_credentials))
  {
    //access point credentials already in flash
    wifi_app_set_event(WIFI_APP_JOIN_STATE);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_task
 * @brief      this function will execute when BLE events are raised.
 * @param[in]  none.
 * @return     none.
 * @section description
 */

void rsi_ble_configurator_task(void *argument)
{
  UNUSED_PARAMETER(argument);

  int32_t status = 0;
  int32_t event_id;
  uint8_t data[RSI_BLE_MAX_DATA_LEN] = { 0 };
  uint8_t scan_ix, length;
  uint8_t k;

  scanresult = (sl_wifi_scan_result_t *)malloc(scanbuf_size);
  if (scanresult == NULL) {
    LOG_PRINT("Failed to allocate memory for scan result\r\n");
    return;
  }
  memset(scanresult, 0, scanbuf_size);
  while (1) {
    // checking for events list
    event_id = rsi_ble_app_get_event();
    if (event_id == -1) {
      osSemaphoreAcquire(ble_thread_sem, osWaitForever);
      // if events are not received loop will be continued.
      continue;
    }

    switch (event_id) {

      case RSI_BLE_ADVERTISE:{
        rsi_ble_app_clear_event(RSI_BLE_ADVERTISE);
        if(!is_ble_advertising){
          int8_t ret = rsi_ble_start_advertising();
          LOG_PRINT("\r\nBLE Advertising Started with status %d...\r\n",ret);
        }
        is_ble_advertising = true;
      }break;

      case RSI_BLE_STOP_ADVERTISE:{
        rsi_ble_app_clear_event(RSI_BLE_STOP_ADVERTISE);
        if(is_ble_advertising){
            int8_t ret = rsi_ble_stop_advertising();
            LOG_PRINT("\r\nBLE Advertising Stopped with status %d...\r\n",ret);
        }
        is_ble_advertising = false;
      }break;

      case RSI_BLE_DISCONN_REMOTE_DEVICE:{
        rsi_ble_app_clear_event(RSI_BLE_DISCONN_REMOTE_DEVICE);
        if(ble_connected){
        uint32_t ret = rsi_ble_disconnect((int8_t *)conn_event_to_app.dev_addr);
        LOG_PRINT("\r\nDisconnected from remote ble device with status %lx...\r\n",ret);
        }
        ble_connected = false;
      }break;


      case RSI_BLE_ENH_CONN_EVENT: {
        // event invokes when connection was completed
        SystemStatus.ble_connect =true;
#if DISPLAY
        demoUIDisplayApp(SystemStatus);
#endif
        // clear the served event
        rsi_ble_app_clear_event(RSI_BLE_ENH_CONN_EVENT);

#if SL_SI91X_TICKLESS_MODE
        //sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
#endif
        LOG_PRINT("\r\nConnected - remote_dev_addr : %s\r\n",
                  rsi_6byte_dev_address_to_ascii(remote_dev_addr, conn_event_to_app.dev_addr));

        //MTU exchange
        status = rsi_ble_mtu_exchange_event(conn_event_to_app.dev_addr, BLE_MTU_SIZE);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\n MTU request failed with error code %lx", status);
        }
      }
      ble_connected = true;
      break;

      case RSI_BLE_DISCONN_EVENT: { // event invokes when disconnection was completed
        // clear the served event
        rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);

        SystemStatus.ble_connect =false;
        SystemStatus.ble_ops = false;
#if DISPLAY
        demoUIDisplayApp(SystemStatus);
#endif
#if SL_SI91X_TICKLESS_MODE
        //sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
#endif
        LOG_PRINT("\r\nDisconnected - remote_dev_addr : %s\r\n",rsi_6byte_dev_address_to_ascii(remote_dev_addr, disconn_event_to_app.dev_addr));
        is_ble_advertising = false;
        rsi_ble_app_set_event(RSI_BLE_ADVERTISE);
        ble_connected = false;
      } break;

      case RSI_APP_FW_VERSION: {
        sl_wifi_firmware_version_t firmware_version = { 0 };

        rsi_ble_app_clear_event(RSI_APP_FW_VERSION);
        memset(data, 0, RSI_BLE_MAX_DATA_LEN);

        status = sl_wifi_get_firmware_version(&firmware_version);
        if (status == SL_STATUS_OK) {
          data[0] = 0x08;
          data[1] = sizeof(sl_wifi_firmware_version_t);
          memcpy(&data[2], &firmware_version, sizeof(sl_wifi_firmware_version_t));

          rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, data);
          print_firmware_version(&firmware_version);
        }
      } break;

      case RSI_APP_SMART_QUERY_STATE:
      {
        rsi_ble_app_clear_event(RSI_APP_SMART_QUERY_STATE);
        memset(data,0, RSI_BLE_MAX_DATA_LEN);

        data[0]=0x09;
        data[1]=1;
        data[2]=SystemStatus.lock_status;

        rsi_ble_set_local_att_value(rsi_ble_att3_val_hndl, RSI_BLE_MAX_DATA_LEN, data);
      }
      break;

        // Connected SSID name (response to '7' command if connection is already established)
      case RSI_WLAN_ALREADY: {
        rsi_ble_app_clear_event(RSI_WLAN_ALREADY);

        memset(data, 0, RSI_BLE_MAX_DATA_LEN);

        data[1] = connected; /*This index will indicate wlan AP connect or disconnect status to Android app*/
        data[0] = 0x07;
        rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, data);
      } break;

        // NO WLAN connection (response to '7' command if connection is there already)
      case RSI_WLAN_NOT_ALREADY: {
        rsi_ble_app_clear_event(RSI_WLAN_NOT_ALREADY);
        memset(data, 0, RSI_BLE_MAX_DATA_LEN);
        data[0] = 0x07;
        data[1] = 0x00;
        rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, data);
      } break;

      case RSI_BLE_WLAN_DISCONN_NOTIFY: {
        rsi_ble_app_clear_event(RSI_BLE_WLAN_DISCONN_NOTIFY);
        memset(data, 0, RSI_BLE_MAX_DATA_LEN);
        data[1] = 0x01;
        data[0] = 0x04;
        rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, data);
      } break;

      case RSI_BLE_WLAN_TIMEOUT_NOTIFY: {
        rsi_ble_app_clear_event(RSI_BLE_WLAN_TIMEOUT_NOTIFY);
        memset(data, 0, RSI_BLE_MAX_DATA_LEN);
        data[0] = 0x02;
        data[1] = 0x00;
        rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, data);
      } break;

      case RSI_BLE_WLAN_DISCONNECT_STATUS: {
        rsi_ble_app_clear_event(RSI_BLE_WLAN_DISCONNECT_STATUS);
        memset(data, 0, RSI_BLE_MAX_DATA_LEN);
        data[0] = 0x01;
        rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, data);
      } break;

      case RSI_SSID: {
        rsi_ble_app_clear_event(RSI_SSID);
      } break;

      case RSI_SECTYPE: {
        rsi_ble_app_clear_event(RSI_SECTYPE);
        if (ap_credentials.sec_type == 0) {
          wifi_app_set_event(WIFI_APP_JOIN_STATE);
        }
      } break;

        // Scan results from device (response to '3' command)
      case RSI_BLE_WLAN_SCAN_RESP: //Send the SSID data to mobile ble application WYZBEE CONFIGURATOR
      {
        rsi_ble_app_clear_event(RSI_BLE_WLAN_SCAN_RESP); // clear the served event

        memset(data, 0, RSI_BLE_MAX_DATA_LEN);
        data[0] = 0x03;
        data[1] = scanresult->scan_count;
        rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, data);

        for (scan_ix = 0; scan_ix < scanresult->scan_count; scan_ix++) {
          memset(data, 0, RSI_BLE_MAX_DATA_LEN);
          data[0] = scanresult->scan_info[scan_ix].security_mode;
          data[1] = ',';
          strcpy((char *)data + 2, (const char *)scanresult->scan_info[scan_ix].ssid);
          length = strlen((char *)data + 2);
          length = length + 2;

          rsi_ble_set_local_att_value(rsi_ble_att3_val_hndl, RSI_BLE_MAX_DATA_LEN, data);
          osDelay(10);
        }

        LOG_PRINT("Displayed scan list in Silabs app\r\n\n");
      } break;

      // WLAN connection response status (response to '2' command)
      case RSI_BLE_WLAN_JOIN_STATUS: //Send the connected status to mobile ble application WYZBEE CONFIGURATOR
      {
        sl_mac_address_t mac_addr = { 0 };

        sl_ip_address_t ip = { 0 };
        ip.type            = ip_address.type;
        ip.ip.v4.value     = ip_address.ip.v4.ip_address.value;

        // clear the served event
        rsi_ble_app_clear_event(RSI_BLE_WLAN_JOIN_STATUS);

        memset(data, 0, RSI_BLE_MAX_DATA_LEN);
        data[0] = 0x02;
        data[1] = 0x01;
        data[2] = ',';

        // Copy the MAC address
        status = sl_wifi_get_mac_address(SL_WIFI_CLIENT_INTERFACE, &mac_addr);
        if (status == SL_STATUS_OK) {
          for (k = 0; k < 6; k++) {
            data[k + 3] = mac_addr.octet[k];
          }
        } else {
          k = 6;
        }
        data[k + 3] = ',';

        // IP Address
        for (int i = 0; k < 10; k++, i++) {
          data[k + 4] = ip.ip.v4.bytes[i];
        }

        rsi_ble_set_local_att_value(rsi_ble_att2_val_hndl,
                                    RSI_BLE_MAX_DATA_LEN,
                                    data); // set the local attribute value.
        LOG_PRINT("AP joined successfully\r\n\n");
        if(ble_connected){
          status = rsi_ble_conn_params_update(conn_event_to_app.dev_addr,
                                              CONN_INTERVAL_MIN,
                                              CONN_INTERVAL_MAX,
                                              CONN_LATENCY,
                                              SUPERVISION_TIMEOUT);
          if (status != RSI_SUCCESS) {
            LOG_PRINT("\r\nconn params update cmd failed with status "
                      "= %lx \r\n",
                      status);
          }
        }
        //rsi_ble_app_set_event(RSI_BLE_DISCONN_REMOTE_DEVICE);
        //   conn_params_updated = 1;
        //  rsi_ble_app_clear_event(RSI_APP_BLE_GATT_SERVICE_RESP_DESCRIPTOR);
      } break;
      case RSI_BLE_MTU_EVENT: {
        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_MTU_EVENT);
        //! event invokes when write/notification events received
        status = rsi_ble_conn_params_update(conn_event_to_app.dev_addr,
                                            CONN_INTERVAL_DEFAULT_MIN,
                                            CONN_INTERVAL_DEFAULT_MAX,
                                            CONN_LATENCY,
                                            SUPERVISION_TIMEOUT_DEFAULT);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\n rsi_ble_conn_params_update command failed : %lx", status);
        }
      } break;
      case RSI_BLE_CONN_UPDATE_EVENT: {
        rsi_ble_app_clear_event(RSI_BLE_CONN_UPDATE_EVENT);

      } break;
      case RSI_BLE_RECEIVE_REMOTE_FEATURES: {
        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);

        if (remote_dev_feature.remote_features[0] & 0x20) {
          status = rsi_ble_set_data_len(conn_event_to_app.dev_addr, TX_LEN, TX_TIME);
          if (status != RSI_SUCCESS) {
            LOG_PRINT("\n set data length cmd failed with error code = "
                      "%lx \n",
                      status);
            rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
          }
        }

      } break;
      case RSI_BLE_DATA_LENGTH_CHANGE: {
        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_DATA_LENGTH_CHANGE);
      } break;
      case RSI_APP_SMART_LOCK: {
        rsi_ble_app_clear_event(RSI_APP_SMART_LOCK);

        memset(data, 0, sizeof(data)); //updating 0 (BLE ) source.
        rsi_ble_set_local_att_value(lock_source_handle, 1, data);

        osDelay(10);

        SystemStatus.lock_status = 1;
        SystemStatus.ble_ops = true;
        SystemStatus.wifi_ops = false;

#if DISPLAY
        demoUIDisplayApp(SystemStatus);
#endif
        data[0] =  SystemStatus.lock_status;

        memset(&SoP,0,sizeof(SoP));
        strcat(SoP.source,"BLE");
        strcat(SoP.command,"LOCK");

        rsi_ble_set_local_att_value(dev_lock_handle, 1, data);

        osDelay(10);

        data[0]=0x0B;
        data[1]=1;
        data[2]=SystemStatus.lock_status;
        rsi_ble_set_local_att_value(rsi_ble_att3_val_hndl, 20, data);

        rsi_app_set_event(LOCK_CMD_RECEIVED_STATE);
      }break;

      case RSI_APP_SMART_UNLOCK :{
        rsi_ble_app_clear_event(RSI_APP_SMART_UNLOCK);

        memset(data, 0, sizeof(data)); //updating 0 (BLE ) source.
        rsi_ble_set_local_att_value(lock_source_handle, 1, data);
        osDelay(10);

        SystemStatus.lock_status = 0;
        SystemStatus.ble_ops = true;
        SystemStatus.wifi_ops = false;
#if DISPLAY
        demoUIDisplayApp(SystemStatus);
#endif
        data[0] = SystemStatus.lock_status;

        memset(&SoP,0,sizeof(SoP));
        strcat(SoP.source,"BLE");
        strcat(SoP.command,"UNLOCK");

        rsi_ble_set_local_att_value(dev_lock_handle, 1, data);

        osDelay(10);

        data[0]=0x0B;
        data[1]=1;
        data[2]=SystemStatus.lock_status;
        rsi_ble_set_local_att_value(rsi_ble_att3_val_hndl, 20, data);

        rsi_app_set_event(UNLOCK_CMD_RECEIVED_STATE);
      }break;
      default:
        break;
    }
    osDelay(10);
  }

  free(scanresult);
}

/*==============================================*/
/**
 * @fn         wifi_app_send_to_ble
 * @brief      this function is used to send data to ble app.
 * @param[in]   msg_type, it indicates write/notification event id.
 * @param[in]  data, raw data pointer.
 * @param[in]  data_len, raw data length.
 * @return     none.
 * @section description
 */
void wifi_app_send_to_ble(uint16_t msg_type, uint8_t *data, uint16_t data_len)
{
  if(!magic_word)
  {
    switch (msg_type) {
      case WIFI_APP_SCAN_RESP:
        memset(scanresult, 0, data_len);
        memcpy(scanresult, (sl_wifi_scan_result_t *)data, data_len);

        rsi_ble_app_set_event(RSI_BLE_WLAN_SCAN_RESP);
        break;
      case WIFI_APP_CONNECTION_STATUS:
        rsi_ble_app_set_event(RSI_BLE_WLAN_JOIN_STATUS);
        break;
      case WIFI_APP_DISCONNECTION_STATUS:
        rsi_ble_app_set_event(RSI_BLE_WLAN_DISCONNECT_STATUS);
        break;
      case WIFI_APP_DISCONNECTION_NOTIFY:
        rsi_ble_app_set_event(RSI_BLE_WLAN_DISCONN_NOTIFY);
        break;
      case WIFI_APP_TIMEOUT_NOTIFY:
        rsi_ble_app_set_event(RSI_BLE_WLAN_TIMEOUT_NOTIFY);
        break;
      default:
        break;
    }
  }
}
/** adding dmp services and char **/
void init_dev_service(void)
{
  //! Adding the "DMP Service"
  //! UUID: FD63
  rsi_ble_add_dev_serv();

  //! Adding the "LIGHT" characteristic service attribute
  //! UUID:76e137ac-b15f-49d7-9c4c-e278e6492ad9
  rsi_ble_add_dev_lock_char();

  //! Adding the "SOURCE" characteristic service attribute
  //! UUID:2f16ee52-0bfd-4597-85d4-a5141fdbae15
  rsi_ble_add_source_char();

  //! Adding the "ADDRESS" characteristic service attribute
  //! UUID:82a1cb54-3921-4c9c-ba34-34f78bab9a1b
  rsi_ble_add_address_char();
}

void rsi_ble_add_dev_serv(void)
{
  uuid_t new_uuid                       = { 0 };
  rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
  new_uuid.size      = 2; // adding new service
  new_uuid.val.val16 =  0xFD63;

  rsi_ble_add_service(new_uuid, &new_serv_resp);
  dev_serv_response.serv_handler = new_serv_resp.serv_handler;
  dev_serv_response.start_handle = new_serv_resp.start_handle;

}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_dev_lock_char
 * @brief      this function is used to create the lock status Characteristic
 * characteristic service
 * @param[in]  none.
 * @return     none.
 * @section description
 * Information about the "Total number of chunks" and "Size of Last chunk" is
 * written on this characteristic UUID:76e137ac-b15f-49d7-9c4c-e278e6492ad9
 */
void rsi_ble_add_dev_lock_char(void)
{

  uint8_t data[RSI_BLE_MAX_DATA_LEN]    = {0};

  uint8_t lock_char_serv[UUID_SIZE] = {
      0x76, 0xe1, 0x37, 0xac,
      0xb1, 0x5f,
      0x49, 0xd7,
      0x9c, 0x4c, 0xe2, 0x78,
      0xe6, 0x49, 0x2a, 0xd9
  };

  dev_lock_char.size = UUID_SIZE;

  rsi_ble_prepare_128bit_uuid(lock_char_serv, &dev_lock_char);
  rsi_ble_add_char_serv_att_custom(dev_serv_response.serv_handler,
                            dev_serv_response.start_handle + 8,
                            RSI_BLE_ATT_PROPERTY_WRITE |RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_INDICATE,
                            dev_serv_response.start_handle + 9,
                            dev_lock_char);

  rsi_ble_add_char_val_att_custom(dev_serv_response.serv_handler,
                           dev_serv_response.start_handle + 9,
                           dev_lock_char,
                           RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_INDICATE,
                           data, RSI_BLE_MAX_DATA_LEN, 0);
  dev_lock_handle = dev_serv_response.start_handle + 9;

}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_source_char
 * @brief      this function is used to create the SOURCE Characteristic
 * characteristic service
 * @param[in]  none.
 * @return     none.
 * @section description
 * Information about the "Total number of chunks" and "Size of Last chunk" is
 * written on this characteristic UUID:2f16ee52-0bfd-4597-85d4-a5141fdbae15
 */
void rsi_ble_add_source_char(void)
{
  uint8_t source_char_serv[UUID_SIZE] = {
      0x2f, 0x16, 0xee, 0x52,
      0x0b, 0xfd,
      0x45, 0x97,
      0x85, 0xd4, 0xa5, 0x14,
      0x1f, 0xdb, 0xae, 0x15
  };
  source_char.size = UUID_SIZE;

  rsi_ble_prepare_128bit_uuid(source_char_serv, &source_char);
  rsi_ble_add_char_serv_att_custom(dev_serv_response.serv_handler,
                            dev_serv_response.start_handle + 11,
                            RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_INDICATE,
                            dev_serv_response.start_handle + 12,
                            source_char);

  uint8_t data[RSI_BLE_MAX_DATA_LEN]    = {0};
  rsi_ble_add_char_val_att_custom(dev_serv_response.serv_handler,
                           dev_serv_response.start_handle + 12,
                           source_char,
                           RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_INDICATE,
                           data, RSI_BLE_MAX_DATA_LEN, 0);

  lock_source_handle = dev_serv_response.start_handle + 12;

}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_address_char
 * @brief      this function is used to create the ADDRESS Characteristic
 * characteristic service
 * @param[in]  none.
 * @return     none.
 * @section description
 * Information about the "Total number of chunks" and "Size of Last chunk" is
 * written on this characteristic UUID:82a1cb54-3921-4c9c-ba34-34f78bab9a1b
 */

void rsi_ble_add_address_char(void)
{
  uint8_t data[8]    = {0};
  uint8_t addr_char_serv[UUID_SIZE] = {
      0x82, 0xa1, 0xcb, 0x54,
      0x39, 0x21,
      0x4c, 0x9c,
      0xba, 0x34, 0x34, 0xf7,
      0x8b, 0xab, 0x9a, 0x1b
  };
  addr_char.size = UUID_SIZE;
  rsi_ble_prepare_128bit_uuid(addr_char_serv, &addr_char);
  rsi_ble_add_char_serv_att_custom(dev_serv_response.serv_handler,
                            dev_serv_response.start_handle + 14,
                            RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_INDICATE,
                            dev_serv_response.start_handle + 15,
                            addr_char);

  rsi_ble_add_char_val_att_custom(dev_serv_response.serv_handler,
                           dev_serv_response.start_handle + 15,
                           addr_char,
                           RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_INDICATE,
                           data, 8, 0);
  addr_handle = dev_serv_response.start_handle + 15;

}

/*============================================================================*/
/**
 * @fn         rsi_ble_prepare_128bit_uuid
 * @brief      this function is used to prepare the 128bit UUID
 * @param[in]  temp_service,received 128-bit service.
 * @param[out] temp_uuid,formed 128-bit service structure.
 * @return     none.
 * @section description
 * This function prepares the 128bit UUID
 */
void rsi_ble_prepare_128bit_uuid(uint8_t temp_service[UUID_SIZE], uuid_t *temp_uuid)
{
  temp_uuid->val.val128.data1 = ((temp_service[0] << 24) | (temp_service[1] << 16) | (temp_service[2] << 8) | (temp_service[3]));
  temp_uuid->val.val128.data2 = ((temp_service[5]) | (temp_service[4] << 8));
  temp_uuid->val.val128.data3 = ((temp_service[7]) | (temp_service[6] << 8));
  temp_uuid->val.val128.data4[0] = temp_service[9];
  temp_uuid->val.val128.data4[1] = temp_service[8];
  temp_uuid->val.val128.data4[2] = temp_service[11];
  temp_uuid->val.val128.data4[3] = temp_service[10];
  temp_uuid->val.val128.data4[4] = temp_service[15];
  temp_uuid->val.val128.data4[5] = temp_service[14];
  temp_uuid->val.val128.data4[6] = temp_service[13];
  temp_uuid->val.val128.data4[7] = temp_service[12];
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_char_val_att_custom
 * @brief      this function is used to add characteristic value attribute.
 * @param[in]  serv_handler, new service handler.
 * @param[in]  handle, characteristic value attribute handle.
 * @param[in]  att_type_uuid, attribute UUID value.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  data, characteristic value data pointer.
 * @param[in]  data_len, characteristic value length.
 * @return     none.
 * @section description
 * This function is used to add characteristic value attribute.
 */

void rsi_ble_add_char_val_att_custom(void *serv_handler, uint16_t handle,
                                     uuid_t att_type_uuid, uint8_t val_prop,
                                     uint8_t *data, uint8_t data_len,
                                     uint8_t auth_read)
{
  rsi_ble_req_add_att_t new_att = {0};

  //! preparing the attributes
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  new_att.config_bitmap = auth_read;
  memcpy(&new_att.att_uuid, &att_type_uuid, sizeof(uuid_t));
  new_att.property = val_prop;

  if (data != NULL)
    memcpy(new_att.data, data, sizeof(new_att.data));

  //! preparing the attribute value
  new_att.data_len = data_len;

  //! add attribute to the service
  rsi_ble_add_attribute(&new_att);

  //! check the attribute property with notification
  if (val_prop & RSI_BLE_ATT_PROPERTY_INDICATE){
    //! if notification property supports then we need to add client characteristic service.

    //! preparing the client characteristic attribute & values
    memset(&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler       = serv_handler;
    new_att.handle             = handle + 1;
    new_att.att_uuid.size      = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
    new_att.property           = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE;
    new_att.data_len           = 2;

    //! add attribute to the service
    rsi_ble_add_attribute(&new_att);
  }

  return;
}

void rsi_ble_add_char_serv_att_custom(void *serv_handler, uint16_t handle,
                                      uint8_t val_prop, uint16_t att_val_handle,
                                      uuid_t att_val_uuid)
{
  rsi_ble_req_add_att_t new_att = {0};

  //! preparing the attribute service structure
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  new_att.att_uuid.size = 2;
  new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
  new_att.property = RSI_BLE_ATT_PROPERTY_READ;

  //! preparing the characteristic attribute value
  if (att_val_uuid.size == UUID_SIZE) {
    new_att.data_len = 4 + att_val_uuid.size;
    new_att.data[0] = val_prop;
    rsi_uint16_to_2bytes(&new_att.data[2], att_val_handle);
    memcpy(&new_att.data[4], &att_val_uuid.val.val128,
           sizeof(att_val_uuid.val.val128));
  }

  else {
    new_att.data_len = 4;
    new_att.data[0] = val_prop;
    rsi_uint16_to_2bytes(&new_att.data[2], att_val_handle);
    rsi_uint16_to_2bytes(&new_att.data[4], att_val_uuid.val.val16);
  }

  //! Add attribute to the service
  rsi_ble_add_attribute(&new_att);

  return;
}

void send_lock_state_to_ble(void)
{
  uint8_t data = 0;

  SystemStatus.lock_status = 1;
  SystemStatus.wifi_ops = true;
  SystemStatus.ble_ops = false;
#if DISPLAY
  demoUIDisplayApp(SystemStatus);
#endif
  data = 1; //updating 1 AWS source.
  rsi_ble_set_local_att_value(lock_source_handle, 1, &data);
  data = SystemStatus.lock_status;

  osDelay(10);

  rsi_ble_set_local_att_value(dev_lock_handle,1, &data);
  uint8_t data1[10]={0};
  data1[0]=0x0B;
  data1[1]=1;
  data1[2]=SystemStatus.lock_status;
  rsi_ble_set_local_att_value(rsi_ble_att3_val_hndl, 10, data1);

  osDelay(10);
}

void send_unlock_state_to_ble(void)
{
  uint8_t data = 0;

  SystemStatus.lock_status = 0;
  SystemStatus.wifi_ops = true;
  SystemStatus.ble_ops = false;
#if DISPLAY
  demoUIDisplayApp(SystemStatus);
#endif
  data = 1; //updating 1 AWS source.
  rsi_ble_set_local_att_value(lock_source_handle, 1, &data);

  osDelay(10);

  data = SystemStatus.lock_status;
  rsi_ble_set_local_att_value(dev_lock_handle,1, &data);
  uint8_t data1[10]={0};
  data1[0]=0x0B;
  data1[1]=1;
  data1[2]=SystemStatus.lock_status;
  rsi_ble_set_local_att_value(rsi_ble_att3_val_hndl, 10, data1);

  osDelay(10);
}

void send_button_activity_to_ble(void)
{
  uint8_t data = 0;

  sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
#if DISPLAY
  memlcd_app_init();
#endif

  data = 2; //updating 2 local source.
  rsi_ble_set_local_att_value(lock_source_handle, 1, &data);

  if(SystemStatus.lock_status){
    SystemStatus.ble_ops = false;
    SystemStatus.wifi_ops = false;
    SystemStatus.lock_status = 0;
#if DISPLAY
    demoUIDisplayApp(SystemStatus);
#endif
  }
  else{
    SystemStatus.ble_ops = false;
    SystemStatus.wifi_ops = false;
    SystemStatus.lock_status = 1;
#if DISPLAY
    demoUIDisplayApp(SystemStatus);
#endif
  }

  data = SystemStatus.lock_status;
  rsi_ble_set_local_att_value(dev_lock_handle,1, &data);

  osDelay(10);

  uint8_t data1[10]={0};
  data1[0]=0x0B;
  data1[1]=1;
  data1[2]=SystemStatus.lock_status;
  rsi_ble_set_local_att_value(rsi_ble_att3_val_hndl, 10, data1);

  osDelay(10);
  if(connected)
  {
    send_lock_state = true;
    wifi_app_set_event(WIFI_APP_MQTT_PUBLISH_STATE);
  }
  sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
}
