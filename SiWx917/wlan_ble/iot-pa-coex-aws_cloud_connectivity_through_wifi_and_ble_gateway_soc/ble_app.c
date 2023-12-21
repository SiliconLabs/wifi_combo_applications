/*******************************************************************************
* @file  ble_app.c
* @brief
*******************************************************************************
* # License
* <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "math.h"

#include "ble_config.h"
#include "wifi_config.h"

// BLE attribute service types uuid values
#define RSI_BLE_CHAR_SERV_UUID           0x2803
#define RSI_BLE_CLIENT_CHAR_UUID         0x2902
#define RSI_BLE_THERMOMETER_SERVICE_UUID 0x1809

// BLE characteristic service uuid
#define RSI_BLE_NEW_SERVICE_UUID 0xAABB
#define RSI_BLE_ATTRIBUTE_1_UUID 0x1AA1
#define RSI_BLE_ATTRIBUTE_2_UUID 0x1BB1
#define RSI_BLE_ATTRIBUTE_3_UUID 0x1CC1

//BLE Characteristics for Temperature reading
#define RSI_BLE_TEMPERATURE_MEASUREMENT_UUID  0x2A1C
#define RSI_BLE_TEMPERATURE_TYPE_UUID         0x2A1D
#define RSI_BLE_INTERMEDIATE_TEMPERATURE_UUID 0x2A1E
#define RSI_BLE_MEASUREMENT_INTERVAL_UUID     0x2A21

// attribute properties
#define RSI_BLE_ATT_PROPERTY_READ     0x02
#define RSI_BLE_ATT_PROPERTY_WRITE    0x08
#define RSI_BLE_ATT_PROPERTY_NOTIFY   0x10
#define RSI_BLE_ATT_PROPERTY_INDICATE 0x20
// max data length
#define RSI_BLE_MAX_DATA_LEN 66

// local device name
#define RSI_BLE_APP_DEVICE_NAME "BLE_CONFIGURATOR"

// Address of the device to connect
#define RSI_BLE_DEV_ADDR "00:1E:7C:25:E9:4D"

// Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME "Thermometer Example"
// local device name
#define RSI_BLE_HEART_RATE_PROFILE "BLE_THERMO_CLIENT"

// Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

// local device IO capability
#define RSI_BLE_SMP_IO_CAPABILITY 0x00
#define RSI_BLE_SMP_PASSKEY       0

// application event list
#define RSI_BLE_ENH_CONN_EVENT 0x01
#define RSI_BLE_DISCONN_EVENT  0x02
#define RSI_BLE_WLAN_SCAN_RESP 0x03
#define RSI_BLE_SMP_REQ_EVENT  0x05

#define RSI_BLE_SMP_RESP_EVENT        0x06
#define RSI_BLE_SMP_PASSKEY_EVENT     0x07
#define RSI_BLE_SMP_FAILED_EVENT      0x08
#define RSI_BLE_ENCRYPT_STARTED_EVENT 0x09

#define RSI_CUSTOMERID_EVENT        0x0B
#define RSI_CUSTOMERID_EVENT1       0x0C
#define RSI_SSID                    0x0D
#define RSI_SECTYPE                 0x0E
#define RSI_BLE_WLAN_DISCONN_NOTIFY 0x0F

#define RSI_WLAN_ALREADY                      0x10
#define RSI_WLAN_NOT_ALREADY                  0x11
#define RSI_BLE_WLAN_TIMEOUT_NOTIFY           0x12
#define RSI_APP_FW_VERSION                    0x13
#define RSI_BLE_WLAN_DISCONNECT_STATUS        0x14
#define RSI_BLE_WLAN_JOIN_STATUS              0x15
#define RSI_BLE_MTU_EXCHANGE                  0x16
#define RSI_BLE_GATT_PROFILE_RESP_EVENT       0x18
#define RSI_BLE_GATT_SEND_DATA                0x19
#define RSI_BLE_GATT_PROFILES_RESP_EVENT      0x1A
#define RSI_BLE_GATT_CHAR_DESC_RESP_EVENT     0x1B
#define RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT 0x1C
#define RSI_APP_EVENT_ADV_REPORT              0x1D
#define RSI_APP_EVENT_CONNECTED               0x1E
#define RSI_APP_EVENT_DISCONNECTED            0x1F
#define RSI_FW_VERSION                        0x01

// Maximum length of SSID
#define RSI_SSID_LEN 34
// MAC address length
#define RSI_MAC_ADDR_LEN 6
// Maximum Acccess points that can be scanned
#define RSI_AP_SCANNED_MAX 11

//! connection update param
#define CONN_INTERVAL_MIN         0x320
#define CONN_INTERVAL_MAX         0x320
#define CONN_INTERVAL_DEFAULT_MIN 0xC8
#define CONN_INTERVAL_DEFAULT_MAX 0xC8
#define CONN_LATENCY              0

/********************************************************************************************************
 *                                               DATA TYPES
 *********************************************************************************************************
 */

// global parameters list
uint8_t data[20] = { 0 };
uint8_t coex_ssid[50];
int8_t provisioner_bd_addr[6] = { 0 };
static volatile uint32_t ble_app_event_map;
rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
//static uint8_t rsi_ble_app_data[100];
static uint8_t rsi_ble_att1_val_hndl;
static uint16_t rsi_ble_att2_val_hndl;
static uint16_t rsi_ble_att3_val_hndl;
static uint16_t temp_mgmt_hndl;
static uint8_t device_found = 0;
uint8_t pwd[RSI_BLE_MAX_DATA_LEN];
uint8_t sec_type;

static uint8_t remote_name[32];
static uint8_t remote_addr_type      = 0;
static uint8_t remote_dev_addr[18]   = { 0 };
static uint8_t remote_dev_bd_addr[6] = { 0 };
static profile_descriptors_t rsi_ble_service;
static rsi_ble_resp_char_services_t char_servs;
static rsi_ble_resp_att_descs_t att_desc;
sl_wifi_scan_result_t *scanresult = NULL;

extern uint8_t connected;
uint16_t indication_handle     = 0;
uint8_t str_remote_address[18] = { '\0' };
int8_t notify_start            = 0;
float efr_temp                 = 0;

//extern uint8_t rsp_buf[18];
extern uint8_t retry;
uint8_t bt_role = 0;
extern sl_net_ip_configuration_t ip_address;

extern osSemaphoreId_t ble_thread_sem;
extern uint16_t scanbuf_size;

rsi_ble_event_write_t app_ble_write_event;
extern void sl_wifi_app_set_event(uint32_t event_num);
extern int32_t sl_wifi_app_get_event(void);
extern void sl_wifi_app_clear_event(uint32_t event_num);

rsi_ble_event_mtu_t app_ble_mtu_event;
rsi_ble_resp_att_descs_t attr_desc_list;
rsi_ble_event_profile_by_uuid_t profiles_list;
/*==============================================*/

typedef struct temp_measurement_s {
  uint8_t flags;
  float float_val;
} temp_measurement_t;

typedef struct {
  uint8_t mantissa_l;
  uint8_t mantissa_m;
  int8_t mantissa_h;
  int8_t exponent;
} IEEE_11073_float;
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
static void rsi_ble_app_set_event(uint32_t event_num)
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
static void rsi_ble_app_clear_event(uint32_t event_num)
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
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy(conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);

  conn_event_to_app.status = resp_enh_conn->status;
  if (bt_role == 0) {
    rsi_ble_app_set_event(RSI_BLE_ENH_CONN_EVENT);
  } else {
    rsi_ble_app_set_event(RSI_APP_EVENT_CONNECTED);
  }
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
  memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  if (bt_role == 0) {
    rsi_ble_app_set_event(RSI_BLE_ENH_CONN_EVENT);
  } else {
    rsi_ble_app_set_event(RSI_APP_EVENT_CONNECTED);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  UNUSED_PARAMETER(reason);
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_disconnect_t));
  if (bt_role == 0) {
    rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
  } else {
    rsi_ble_app_set_event(RSI_APP_EVENT_DISCONNECTED);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_request
 * @brief      its invoked when smp request event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP request events is received(we are in Master mode)
 * Note: slave requested to start SMP request, we have to send SMP request command
 */
void rsi_ble_on_smp_request(rsi_bt_event_smp_req_t *remote_dev_address)
{
  UNUSED_PARAMETER(remote_dev_address);
  rsi_ble_app_set_event(RSI_BLE_SMP_REQ_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_response
 * @brief      its invoked when smp response event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP response events is received(we are in slave mode)
 * Note: Master initiated SMP protocol, we have to send SMP response command
 */
void rsi_ble_on_smp_response(rsi_bt_event_smp_resp_t *remote_dev_address)
{
  UNUSED_PARAMETER(remote_dev_address);
  rsi_ble_app_set_event(RSI_BLE_SMP_RESP_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_passkey
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_smp_passkey(rsi_bt_event_smp_passkey_t *remote_dev_address)
{
  UNUSED_PARAMETER(remote_dev_address);
  rsi_ble_app_set_event(RSI_BLE_SMP_PASSKEY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_failed
 * @brief      its invoked when smp failed event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP failed events is received
 */
void rsi_ble_on_smp_failed(uint16_t status, rsi_bt_event_smp_failed_t *remote_dev_address)
{
  UNUSED_PARAMETER(remote_dev_address);
  UNUSED_PARAMETER(status);
  rsi_ble_app_set_event(RSI_BLE_SMP_FAILED_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_encrypt_started
 * @brief      its invoked when encryption started event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when encryption started events is received
 */
void rsi_ble_on_encrypt_started(uint16_t status, rsi_bt_event_encryption_enabled_t *enc_enabled)
{
  UNUSED_PARAMETER(enc_enabled);
  UNUSED_PARAMETER(status);

  rsi_ble_app_set_event(RSI_BLE_ENCRYPT_STARTED_EVENT);
}

/*==============================================*/
/**
* @fn         rsi_ble_on_mtu_event
* @brief      its invoked when write/notify/indication events are received.
* @param[in]  event_id, it indicates write/notification event id.
* @param[in]  rsi_ble_write, write event parameters.
* @return     none.
* @section description
* This callback function is invoked when write/notify/indication events are received
*/

/*==============================================*/
/**
 * @fn         rsi_ble_on_char_services_event
 * @brief      invoked when response is received for characteristic services details
 * @param[out] rsi_ble_resp_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_on_char_services_event(uint16_t resp_status,
                                           rsi_ble_resp_char_services_t *rsi_ble_resp_char_services)
{
  UNUSED_PARAMETER(resp_status); //This statement is added only to resolve compilation warning, value is unchanged
  UNUSED_PARAMETER(
    rsi_ble_resp_char_services); //This statement is added only to resolve compilation warning, value is unchanged
  rsi_ble_app_set_event(RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_att_desc_event
 * @brief      invoked upon receiving the attribute value
 * @param[out] rsi_ble_resp_att_desc, contains the attribute value.
 * @return     none
 * @section description
 */
static void rsi_ble_on_att_desc_event(uint16_t resp_status, rsi_ble_resp_att_descs_t *rsi_ble_resp_att_desc)
{
  UNUSED_PARAMETER(resp_status); //This statement is added only to resolve compilation warning, value is unchanged
  UNUSED_PARAMETER(
    rsi_ble_resp_att_desc); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&attr_desc_list, rsi_ble_resp_att_desc, sizeof(rsi_ble_resp_att_descs_t));
  rsi_ble_app_set_event(RSI_BLE_GATT_CHAR_DESC_RESP_EVENT);
  return;
}

/**
 * @fn         rsi_ble_profiles_list_event
 * @brief      invoked when response is received for get list of services.
 * @param[out] p_ble_resp_profiles, profiles list details
 * @return     none
 * @section description
 */
static void rsi_ble_profiles_list_event(uint16_t event_status, rsi_ble_event_profiles_list_t *rsi_ble_event_profiles)
{
  UNUSED_PARAMETER(event_status); //This statement is added only to resolve compilation warning, value is unchanged
  UNUSED_PARAMETER(
    rsi_ble_event_profiles); //This statement is added only to resolve compilation warning, value is unchanged
  rsi_ble_app_set_event(RSI_BLE_GATT_PROFILES_RESP_EVENT);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_profile_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] rsi_ble_event_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_profile_event(uint16_t event_status, rsi_ble_event_profile_by_uuid_t *rsi_ble_event_profile)
{
  UNUSED_PARAMETER(event_status); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&profiles_list, rsi_ble_event_profile, sizeof(rsi_ble_event_profile_by_uuid_t));
  rsi_ble_app_set_event(RSI_BLE_GATT_PROFILE_RESP_EVENT);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_health_thermometer_serv
 * @brief      this function is used to add new servcie i.e.., health thermometer serv
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */
static uint32_t rsi_ble_add_health_thermometer_serv(void)
{
  uuid_t new_uuid                       = { 0 };
  rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
  temp_measurement_t measurement = { 0, 27.6 }; // for temperature measurement

  //! adding new service
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_THERMOMETER_SERVICE_UUID;
  rsi_ble_add_service(new_uuid, &new_serv_resp);

  //! adding characteristic service attribute to the service
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_TEMPERATURE_MEASUREMENT_UUID;
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 1,
                            RSI_BLE_ATT_PROPERTY_INDICATE,
                            new_serv_resp.start_handle + 2,
                            new_uuid);

  //! adding characteristic value attribute to the service
  temp_mgmt_hndl     = new_serv_resp.start_handle + 2;
  new_uuid.size      = 2;
  new_uuid.val.val16 = RSI_BLE_TEMPERATURE_MEASUREMENT_UUID;
  rsi_ble_add_char_val_att(new_serv_resp.serv_handler,
                           new_serv_resp.start_handle + 2,
                           new_uuid,
                           RSI_BLE_ATT_PROPERTY_INDICATE,
                           (uint8_t *)&measurement,
                           sizeof(measurement));
  return 0;
}
/*==============================================*/
/**
 * @fn         rsi_ble_simple_central_on_adv_report_event
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
void rsi_ble_simple_central_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report)
{

  if (device_found == 1) {
    return;
  }

  memset(remote_name, 0, 31);
  BT_LE_ADPacketExtract(remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii(remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  memcpy(remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
  if (((device_found == 0) && ((strcmp((const char *)remote_name, RSI_REMOTE_DEVICE_NAME)) == 0))
      || ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)
          && ((strcmp((const char *)remote_dev_addr, RSI_BLE_DEV_ADDR) == 0)))) {
    device_found = 1;
    rsi_ble_app_set_event(RSI_APP_EVENT_ADV_REPORT);
  }
}

// Translate a IEEE-11073 Temperature Value to a float Value
float translate_IEEE_11073_temperature_to_float(IEEE_11073_float const *IEEE_11073_value)
{
  int32_t mantissa = 0;
  uint8_t mantissa_l;
  uint8_t mantissa_m;
  int8_t mantissa_h;
  int8_t exponent;

  // Wrong Argument: NULL pointer is passed
  if (!IEEE_11073_value) {
    return NAN;
  }

  // Caching Fields
  mantissa_l = IEEE_11073_value->mantissa_l;
  mantissa_m = IEEE_11073_value->mantissa_m;
  mantissa_h = IEEE_11073_value->mantissa_h;
  exponent   = IEEE_11073_value->exponent;

  // IEEE-11073 Standard NaN Value Passed
  if ((mantissa_l == 0xFF) && (mantissa_m == 0xFF) && (mantissa_h == 0x7F) && (exponent == 0x00)) {
    return NAN;
  }

  // Converting a 24bit Signed Value to a 32bit Signed Value
  mantissa |= mantissa_h;
  mantissa <<= 8;
  mantissa |= mantissa_m;
  mantissa <<= 8;
  mantissa |= mantissa_l;
  mantissa <<= 8;
  mantissa >>= 8;

  return ((float)mantissa) * pow(10.0f, (float)exponent);
}

/*==============================================*/

/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      this is call back function, it invokes when write/notify events received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This is a callback function
 */
static void rsi_ble_on_gatt_write_event(uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  UNUSED_PARAMETER(event_id);
  uint8_t cmdid;
  uint8_t eventid_temp;
  memcpy(&app_ble_write_event, rsi_ble_write, sizeof(rsi_ble_event_write_t));
  LOG_PRINT("rsi_ble_on_gatt_write_event: handle: 0x%02x\r\n", *(uint16_t *)rsi_ble_write->handle);
  //  Requests will come from Mobile app
  if ((rsi_ble_att1_val_hndl) == *((uint16_t *)rsi_ble_write->handle)) {
    cmdid = rsi_ble_write->att_value[0];

    switch (cmdid) {
      // Scan command request
      case '3':
      {
        LOG_PRINT("Received scan request\n");
        retry = 0;
        memset(data, 0, sizeof(data));
        sl_wifi_app_set_event(SL_WIFI_SCAN_STATE);
      } break;

      // Sending SSID
      case '2':
      {
        LOG_PRINT("join command is passed\n");
        memset(coex_ssid, 0, sizeof(coex_ssid));
        strcpy((char *)coex_ssid, (const char *)&rsi_ble_write->att_value[3]);

        rsi_ble_app_set_event(RSI_SSID);
      } break;

      // Sending Security type
      case '5':
      {
        LOG_PRINT("security command is passed\n");
        sec_type = (rsi_ble_write->att_value[3]) - '0';
        rsi_ble_app_set_event(RSI_SECTYPE);
      } break;

      // Sending PSK
      case '6':
      {
        memset(data, 0, sizeof(data));
        strcpy((char *)pwd, (const char *)&rsi_ble_write->att_value[3]);
        LOG_PRINT("psk received\n");
        sl_wifi_app_set_event(SL_WIFI_JOIN_STATE);
      } break;

      // WLAN Status Request
      case '7':
      {
        LOG_PRINT("WLAN status request received\n");
        memset(data, 0, sizeof(data));
        if (connected) {
          rsi_ble_app_set_event(RSI_WLAN_ALREADY);
        } else {
          rsi_ble_app_set_event(RSI_WLAN_NOT_ALREADY);
        }
      } break;

      // WLAN disconnect request
      case '4':
      {
        LOG_PRINT("WLAN disconnect request received\n");
        memset(data, 0, sizeof(data));
        sl_wifi_app_set_event(SL_WIFI_DISCONN_NOTIFY_STATE);
      } break;

      // FW version request
      case '8': {
        memset(data, 0, sizeof(data));
        rsi_ble_app_set_event(RSI_APP_FW_VERSION);
        LOG_PRINT("FW version request\n");
      } break;

      default:
        LOG_PRINT("Default command case \n\n");
        break;
    }
  } else {
    eventid_temp = rsi_ble_app_get_event();
    rsi_ble_app_clear_event(eventid_temp);
    rsi_ble_app_set_event(SL_BLE_GATT_WRITE_EVENT);
  }
  return;
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
  uint8_t adv[31] = { 2, 1, 6 };

  //  initializing the application events map
  rsi_ble_app_init_events();

  // adding simple BLE chat service
  rsi_ble_add_configurator_serv();

  // adding Health thermometer service
  rsi_ble_add_health_thermometer_serv();

  // registering the GAP callback functions
  rsi_ble_gap_register_callbacks(rsi_ble_simple_central_on_adv_report_event,
                                 rsi_ble_on_connect_event,
                                 rsi_ble_on_disconnect_event,
                                 NULL,
                                 NULL,
                                 NULL,
                                 rsi_ble_on_enhance_conn_status_event,
                                 NULL,
                                 NULL,
                                 NULL);

  // registering the GATT callback functions
  rsi_ble_gatt_register_callbacks(NULL,
                                  NULL,
                                  rsi_ble_on_char_services_event,
                                  NULL,
                                  rsi_ble_on_att_desc_event,
                                  NULL,
                                  NULL,
                                  rsi_ble_on_gatt_write_event,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  rsi_ble_profiles_list_event,
                                  rsi_ble_profile_event,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL);

  // registering the SMP call back functions
  rsi_ble_smp_register_callbacks(rsi_ble_on_smp_request,
                                 rsi_ble_on_smp_response,
                                 rsi_ble_on_smp_passkey,
                                 rsi_ble_on_smp_failed,
                                 rsi_ble_on_encrypt_started,
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL);

  // Set local name
  rsi_bt_set_local_name((uint8_t *)RSI_BLE_APP_DEVICE_NAME);

  // prepare advertise data //local device name
  adv[3] = strlen(RSI_BLE_APP_DEVICE_NAME) + 1;
  adv[4] = 9;
  strcpy((char *)&adv[5], RSI_BLE_APP_DEVICE_NAME);

  // set advertise data
  rsi_ble_set_advertise_data(adv, strlen(RSI_BLE_APP_DEVICE_NAME) + 5);

  // set device in advertising mode.
  rsi_ble_start_advertising();
  LOG_PRINT("\r\nBLE Advertising Start...\r\n");
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
  //uint8_t len;
  uint8_t wlan_temp_eventid;
  uint8_t connected   = 0;
  uuid_t service_uuid = { 0 };
  uint8_t ix;
  uint8_t indicate_data[2] = { 2, 0 };
  uint8_t i;
  //uint16_t notification_handle;
  uint8_t data[RSI_BLE_MAX_DATA_LEN] = { 0 };
  uint8_t scan_ix, length;
  uint8_t k;

  scanresult = (sl_wifi_scan_result_t *)malloc(scanbuf_size);
  if (scanresult == NULL) {
    LOG_PRINT("Failed to allocate memory for scan result\n");
    return;
  }

  while (1) {
    // checking for events list
    event_id = rsi_ble_app_get_event();
    if (event_id == -1) {
      osSemaphoreAcquire(ble_thread_sem, osWaitForever);
      // if events are not received loop will be continued.
      continue;
    }

    switch (event_id) {
      case RSI_BLE_ENH_CONN_EVENT: {
        // event invokes when connection was completed

        // clear the served event
        rsi_ble_app_clear_event(RSI_BLE_ENH_CONN_EVENT);
        memcpy(provisioner_bd_addr, (int8_t *)conn_event_to_app.dev_addr, 6);
        //MTU exchange
        status = rsi_ble_mtu_exchange_event(conn_event_to_app.dev_addr, BLE_MTU_SIZE);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\n rsi_ble_mtu_exchange_event command failed : %lx", status);
        } else {
          LOG_PRINT("\n rsi_ble_mtu_exchange_event command success \n");
        }
      } break;

      case RSI_BLE_DISCONN_EVENT: {
        // event invokes when disconnection was completed

        // clear the served event
        rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);

        // set device in advertising mode.
adv:
// These lines are commented to avoid wlan disconnection if ble is disconnected.
#if 0
        if (connected) {
          LOG_PRINT("disconnect command is passed\n");
          memset(data, 0, RSI_BLE_MAX_DATA_LEN);
          sl_wifi_app_set_event(RSI_WLAN_DISCONN_NOTIFY_STATE);
        }
#endif
        status = rsi_ble_start_advertising();
        if (status != RSI_SUCCESS) {
          goto adv;
        }
      } break;

      case SL_BLE_GATT_WRITE_EVENT: {
        // event invokes when write/notification events received

        // clear the served event
        rsi_ble_app_clear_event(SL_BLE_GATT_WRITE_EVENT);
        if (bt_role == 1) {

          efr_temp =
            translate_IEEE_11073_temperature_to_float((IEEE_11073_float *)((app_ble_write_event.att_value) + 1));
          char unit = translate_flags_to_temperature_unit(app_ble_write_event.att_value[0]);
          LOG_PRINT("\r\n handle: 0x%x, len: %d, TEMPERATURE: %6.2f %c \r\n",
                    *((uint16_t *)app_ble_write_event.handle),
                    app_ble_write_event.length,
                    efr_temp,
                    unit);
        }
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
        if (sec_type == '0' || sec_type == 0) {
          sl_wifi_app_set_event(SL_WIFI_JOIN_STATE);
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

        LOG_PRINT("Displayed scan list in Silabs app\n\n");
      } break;

      // WLAN connection response status (response to '2' command)
      case RSI_BLE_WLAN_JOIN_STATUS:
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
      }

      break;
      case RSI_APP_EVENT_ADV_REPORT: {
        // advertise report event.

        // clear the advertise report event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_ADV_REPORT);
        LOG_PRINT("\r\nIn Advertising Event\r\n");

        // initiate stop scanning command.
        status = rsi_ble_stop_scanning();
        if (status != RSI_SUCCESS) {
          LOG_PRINT("start_scanning status: 0x%lX\r\n", status);
          //    return status;
        }

        status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_bd_addr);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("connect status: 0x%lX\r\n", status);
        }
      } break;

      case RSI_APP_EVENT_CONNECTED: {
        // remote device connected event

        // clear the connected event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_CONNECTED);
        rsi_ble_mtu_exchange_event(conn_event_to_app.dev_addr, 27);
        rsi_6byte_dev_address_to_ascii(str_remote_address, conn_event_to_app.dev_addr);
        LOG_PRINT("\r\n Module connected to address : %s \r\n", str_remote_address);
        connected              = 1;
        service_uuid.size      = 2;
        service_uuid.val.val16 = RSI_BLE_THERMOMETER_SERVICE_UUID;
retry:
        status = rsi_ble_get_profile_async(conn_event_to_app.dev_addr, service_uuid, &rsi_ble_service);
        if (status != 0) {
          goto retry;
        }

      } break;

      case RSI_APP_EVENT_DISCONNECTED: {
        // remote device disconnected event

        // clear the disconnected event.
        notify_start = 0;
        rsi_ble_app_clear_event(RSI_APP_EVENT_DISCONNECTED);
        LOG_PRINT("\r\nModule got Disconnected\r\n");

#if ENABLE_POWER_SAVE

        LOG_PRINT("\r\n keep module in to active state \r\n");
        //! initiating Active mode in BT mode
        status = rsi_bt_power_save_profile(RSI_ACTIVE, PSP_TYPE);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n Failed to keep Module in ACTIVE mode \r\n");
          return status;
        }

        //! initiating Active mode in WLAN mode
        status = rsi_wlan_power_save_profile(RSI_ACTIVE, PSP_TYPE);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n Failed to keep Module in ACTIVE mode \r\n");
          return status;
        }

#endif

        connected    = 0;
        device_found = 0;
        LOG_PRINT("\r\n BLE Scanning started\r\n");
        //! start scanning
        status = rsi_ble_start_scanning();
        if (status != RSI_SUCCESS) {
          LOG_PRINT("start_scanning status: 0x%lX\r\n", status);
        }
#if ENABLE_POWER_SAVE

        LOG_PRINT("\r\n keep module in to power save \r\n");
        status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
        if (status != RSI_SUCCESS) {
          return status;
        }

        status = rsi_wlan_power_save_profile(PSP_MODE, PSP_TYPE);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n Failed to keep Module in power save \r\n");
          return status;
        }
        LOG_PRINT("\r\n Module is in power save \r\n");
#endif
      } break;

      case RSI_BLE_GATT_PROFILES_RESP_EVENT: {
        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_GATT_PROFILES_RESP_EVENT);
        service_uuid.size      = 2;
        service_uuid.val.val16 = RSI_BLE_THERMOMETER_SERVICE_UUID;
        rsi_ble_get_profile_async(conn_event_to_app.dev_addr, service_uuid, NULL);

      } break;

      case RSI_BLE_GATT_PROFILE_RESP_EVENT: {
        //! event invokes when get profile response received

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_GATT_PROFILE_RESP_EVENT);

        //! get characteristics of the immediate alert servcie
        rsi_ble_get_char_services(conn_event_to_app.dev_addr,
                                  *(uint16_t *)profiles_list.start_handle,
                                  *(uint16_t *)profiles_list.end_handle,
                                  &char_servs);
      } break;

      case RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT: {
        //! event invokes when get characteristics of the service response received

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);

        //! verifying the immediate alert characteristic
        for (ix = 0; ix < char_servs.num_of_services; ix++) {
          LOG_PRINT("Character services of thermometer profile : ");
          LOG_PRINT(" uuid: 0x%04x\r\n", char_servs.char_services[ix].char_data.char_uuid.val.val16);

          if (char_servs.char_services[ix].char_data.char_uuid.val.val16 == RSI_BLE_TEMPERATURE_MEASUREMENT_UUID) {
            temp_mgmt_hndl = char_servs.char_services[ix].char_data.char_handle;
            rsi_ble_get_att_descriptors(conn_event_to_app.dev_addr,
                                        char_servs.char_services[ix].handle + 2,
                                        (char_servs.char_services[ix + 1].handle - 1),
                                        &att_desc);
          }
        }

        //!CCCD indication to remote device
        for (i = 0; i < char_servs.num_of_services; i++) {
          if ((char_servs.char_services[i].char_data.char_property) & RSI_BLE_ATT_PROPERTY_INDICATE) {
            indication_handle = char_servs.char_services[i].char_data.char_handle + 1;
            LOG_PRINT("\r\n indication handle found: %d \r\n", indication_handle);
            rsi_ble_set_att_value_async(remote_dev_bd_addr, indication_handle, 2, indicate_data);
          }
        }

      } break;

      case RSI_BLE_GATT_CHAR_DESC_RESP_EVENT:
        rsi_ble_app_clear_event(RSI_BLE_GATT_CHAR_DESC_RESP_EVENT);

        for (ix = 0; ix < attr_desc_list.num_of_att; ix++) {
          LOG_PRINT("handle: 0x%04x - 0x%04x\r\n",
                    *((uint16_t *)attr_desc_list.att_desc[ix].handle),
                    attr_desc_list.att_desc[ix].att_type_uuid.val.val16);
          if (attr_desc_list.att_desc[ix].att_type_uuid.val.val16 == RSI_BLE_CLIENT_CHAR_UUID) {
            data[0] = 0x01;
            data[1] = 0x00;
            rsi_ble_set_att_cmd(conn_event_to_app.dev_addr,
                                *((uint16_t *)attr_desc_list.att_desc[ix].handle),
                                2,
                                (uint8_t *)data);
          }
          LOG_PRINT("Indication enabled \n");
          wlan_temp_eventid = sl_wifi_app_get_event();
          sl_wifi_app_clear_event(wlan_temp_eventid);
          sl_wifi_app_set_event(SL_WLAN_SET_MQTT);
        }
        break;
      default:
        break;
    }
  }

  free(scanresult);
}

/*==============================================*/
/**
 * @fn         sl_wifi_app_send_to_ble
 * @brief      this function is used to send data to ble app.
 * @param[in]   msg_type, it indicates write/notification event id.
 * @param[in]  data, raw data pointer.
 * @param[in]  data_len, raw data length.
 * @return     none.
 * @section description
 * This is a callback function
 */
void sl_wifi_app_send_to_ble(uint16_t msg_type, uint8_t *data, uint16_t data_len)
{
  switch (msg_type) {
    case SL_WIFI_SCAN_RESP:
      memset(scanresult, 0, data_len);
      memcpy(scanresult, (sl_wifi_scan_result_t *)data, data_len);

      rsi_ble_app_set_event(RSI_BLE_WLAN_SCAN_RESP);
      break;
    case SL_WIFI_CONNECTION_STATUS:
      rsi_ble_app_set_event(RSI_BLE_WLAN_JOIN_STATUS);
      break;
    case SL_WIFI_DISCONNECTION_STATUS:
      rsi_ble_app_set_event(RSI_BLE_WLAN_DISCONNECT_STATUS);
      break;
    case SL_WIFI_DISCONNECTION_NOTIFY:
      rsi_ble_app_set_event(RSI_BLE_WLAN_DISCONN_NOTIFY);
      break;
    case SL_WIFI_TIMEOUT_NOTIFY:
      rsi_ble_app_set_event(RSI_BLE_WLAN_TIMEOUT_NOTIFY);
      break;
    default:
      break;
  }
}
