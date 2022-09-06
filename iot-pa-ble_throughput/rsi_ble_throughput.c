/*******************************************************************************
 * @file  rsi_ble_throughput.c
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

/*==============================================================================
 * @brief : This file contains example application to measure the BLE Throughput
 * for notifications and indications in different PHY data rates
 * @section Description :
 * The RS9116W device is configured as a GATT server and a peripheral device.
 * Based on the type of MCU, a push-button is configured as an interrupt for
 * controlling the data transfer. If the button is pressed, the RS9116W device
 * continuously sends the data until it pressed again.
 * The application has also the provision to choose the PHY rate and connection
 * parameters.
 =============================================================================*/

/**
 * Include files
 * */

//! BLE include file to refer BLE APIs
#include "rsi_driver.h"
#include <rsi_ble_apis.h>
#include <rsi_ble_common_config.h>
#include <rsi_ble_config.h>
#include <rsi_bt_common.h>
#include <rsi_bt_common_apis.h>
#include <stdio.h>
//! Common include file
#include <rsi_common_apis.h>

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

#ifdef RSI_WITH_OS

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE 3000

//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 1000

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY 1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY 2

#endif

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];
uint8_t conn_params_updated = 0;
uint8_t notifications_handle, device_connected;
uint8_t data_transfer_handle;
uint8_t transfer_on;
uint8_t notifications_data_handle, indications_data_handle, data_sent_handle;
uint8_t transmission_on = 0;
uint8_t pdu_length[1] = {0};
uint16_t throughput_result_handle = 0;
uint16_t latest_connection_interval;
uint32_t start_timer = 0;
uint32_t stop_timer = 0;
int32_t event_id;

//! global parameters list
static uint8_t minimum_connection_interval = 0, maximum_connection_interval = 0;
static uint8_t device_found = 0;
static uint8_t start_timer_var;
static uint8_t str_remote_address[18];
static uint8_t remote_dev_address[6] = {0};
static uint16_t connection_phy_handle = 0, connection_interval_handle = 0,
                mtu_size_handle = 0, slave_latency_handle = 0,
                supervision_timeout_handle = 0, pdu_size_handle = 0;
static uint16_t rsi_ble_att1_val_hndl;
static uint32_t ble_app_event_mask;
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_map1;
static uint64_t counter = 0;

#if SMP_ENABLE
static uint32_t numeric_value;
static rsi_bt_event_encryption_enabled_t encrypt_keys;
static rsi_bt_event_le_ltk_request_t ble_ltk_req;
static rsi_bt_event_le_security_keys_t app_ble_sec_keys;
#endif

static rsi_ble_event_phy_update_t rsi_app_phy_update_complete;
static rsi_ble_event_conn_update_t rsi_app_conn_update_complete;
static rsi_ble_event_remote_features_t remote_dev_feature;
static rsi_ble_event_mtu_t rsi_app_ble_mtu_event;
static rsi_ble_event_write_t app_ble_write_event;
static rsi_ble_resp_add_serv_t throghput_test_serv_response;
static rsi_ble_resp_add_serv_t throughput_info_service_response;

float throughput = 0;
float timing = 0;

rsi_semaphore_handle_t ble_main_task_sem;

uint8_t send_buf[230] = {
    0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
    0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
    0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73,
    0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65,
    0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71,
    0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63,
    0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61,
    0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,
    0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
    0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
    0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73,
    0x74, 0x75};


/*============================================================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events(void) {
  ble_app_event_map = 0;
  ble_app_event_mask = 0xFFFFFFFF;
  ble_app_event_mask = ble_app_event_mask; // To suppress warning while
                                           // compiling
  return;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      set the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
void rsi_ble_app_set_event(uint32_t event_num) {
  if (event_num < 32) {
    ble_app_event_map |= BIT(event_num);
  } else {
    ble_app_event_map1 |= BIT((event_num - 32));
  }
#ifdef RSI_WITH_OS
  rsi_semaphore_post_from_isr(&ble_main_task_sem);
#else
  rsi_semaphore_post(&ble_main_task_sem);
#endif
  return;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_event(uint32_t event_num) {
  if (event_num < 32) {
    ble_app_event_map &= ~BIT(event_num);
  } else {
    ble_app_event_map1 &= ~BIT((event_num - 32));
  }
  return;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_app_clear_all_events
 * @brief      clears the all event.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_all_event(void) {
  ble_app_event_map = 0;
  return;
}

/*============================================================================*/
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
static int32_t rsi_ble_app_get_event(void) {
  uint32_t ix;

  for (ix = 0; ix < 64; ix++) {
    if (ix < 32) {
      if (ble_app_event_map & (1 << ix)) {
        return ix;
      }
    } else {
      if (ble_app_event_map1 & (1 << (ix - 32))) {
        return ix;
      }
    }
  }

  return (-1);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(
    rsi_ble_event_enhance_conn_status_t *resp_enh_conn) {
  memcpy(remote_dev_address, resp_enh_conn->dev_addr, 6);
  LOG_PRINT("\r\nConnected - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           resp_enh_conn->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_connect_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn) {
  memcpy(remote_dev_address, resp_conn->dev_addr, 6);
  LOG_PRINT(
      "\r\nConnected - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii(str_remote_address, resp_conn->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void
rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect,
                            uint16_t reason) {
  UNUSED_PARAMETER(reason); // This statement is added only to resolve
                            // compilation warning, value is unchanged
  memcpy(remote_dev_address, resp_disconnect->dev_addr, 6);
  LOG_PRINT("\r\nDisconnected - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           resp_disconnect->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
}

#if SMP_ENABLE
/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_request
 * @brief      its invoked when smp request event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP request events is received(we are
 * in Master mode) Note: slave requested to start SMP request, we have to send
 * SMP request command
 */
void rsi_ble_on_smp_request(rsi_bt_event_smp_req_t *event_smp_req) {
  memcpy(remote_dev_address, event_smp_req->dev_addr, 6);
  LOG_PRINT("\r\nsmp_req - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           event_smp_req->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_REQ_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_response
 * @brief      its invoked when smp response event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP response events is received(we are
 * in slave mode) Note: Master initiated SMP protocol, we have to send SMP
 * response command
 */
void rsi_ble_on_smp_response(rsi_bt_event_smp_resp_t *bt_event_smp) {
  memcpy(remote_dev_address, bt_event_smp->dev_addr, 6);
  LOG_PRINT("\r\nsmp_resp - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           bt_event_smp->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_RESP_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_passkey
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_smp_passkey(rsi_bt_event_smp_passkey_t *event_smp_passkey) {
  memcpy(remote_dev_address, event_smp_passkey->dev_addr, 6);
  LOG_PRINT("\r\nsmp_passkey - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           event_smp_passkey->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_PASSKEY_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_passkey_display
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_smp_passkey_display(
    rsi_bt_event_smp_passkey_display_t *smp_passkey_display) {
  memcpy(remote_dev_address, smp_passkey_display->dev_addr, 6);
  LOG_PRINT("\r\nremote addr: %s, passkey: %s \r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           smp_passkey_display->dev_addr),
            smp_passkey_display->passkey);
  rsi_ble_app_set_event(RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_sc_passkey
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_sc_passkey(rsi_bt_event_sc_passkey_t *sc_passkey) {
  memcpy(remote_dev_address, sc_passkey->dev_addr, 6);
  LOG_PRINT(
      "\r\nremote addr: %s, passkey: %06ld \r\n",
      rsi_6byte_dev_address_to_ascii(str_remote_address, sc_passkey->dev_addr),
      sc_passkey->passkey);
  numeric_value = sc_passkey->passkey;
  rsi_ble_app_set_event(RSI_BLE_SC_PASSKEY_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_le_ltk_req_event
 * @brief      invoked when ltk request event is received
 * @param[in]  le_ltk_req, ltk request parameters.
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void
rsi_ble_on_le_ltk_req_event(rsi_bt_event_le_ltk_request_t *le_ltk_req) {
  LOG_PRINT(" \r\n rsi_ble_on_le_ltk_req_event \r\n");
  memcpy(&ble_ltk_req, le_ltk_req, sizeof(rsi_bt_event_le_ltk_request_t));
  rsi_ble_app_set_event(RSI_BLE_LTK_REQ_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_le_security_keys
 * @brief      its invoked when le secure keys events is received.
 * @param[in]  rsi_ble_event_le_security_keys, secure event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when security keys event is received after
 * encription started.
 */
void rsi_ble_on_le_security_keys(
    rsi_bt_event_le_security_keys_t *rsi_ble_event_le_security_keys) {
  memcpy(&app_ble_sec_keys, rsi_ble_event_le_security_keys,
         sizeof(rsi_bt_event_le_security_keys_t));
  LOG_PRINT("\r\n remote_ediv: 0x%x\r\n", app_ble_sec_keys.remote_ediv);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_failed
 * @brief      its invoked when smp failed event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP failed events is received
 */
void rsi_ble_on_smp_failed(uint16_t status,
                           rsi_bt_event_smp_failed_t *event_smp_failed) {
  memcpy(remote_dev_address, event_smp_failed->dev_addr, 6);
  LOG_PRINT("\r\nsmp_failed status: 0x%x, str_remote_address: %s\r\n", status,
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           event_smp_failed->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_FAILED_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_encrypt_started
 * @brief      its invoked when encryption started event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when encryption started events is received
 */
void rsi_ble_on_encrypt_started(
    uint16_t status, rsi_bt_event_encryption_enabled_t *enc_enabled) {
  LOG_PRINT("\r\nstart encrypt status: %d \r\n", status);
  memcpy(&encrypt_keys, enc_enabled, sizeof(rsi_bt_event_encryption_enabled_t));
  for (int ltk_var = 0; ltk_var < 16; ltk_var++) {
    LOG_PRINT("%x", encrypt_keys.localltk[ltk_var]);
  }
  LOG_PRINT("\n");
  rsi_ble_app_set_event(RSI_BLE_ENCRYPT_STARTED_EVENT);
}

#endif
/*============================================================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute.
 * @param[in]  serv_handler, service handler.
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  att_val_handle, characteristic value handle
 * @param[in]  att_val_uuid, characteristic value uuid
 * @return     none.
 * @section description
 * This function is used at application to add characteristic attribute
 */
static void rsi_ble_add_char_serv_att(void *serv_handler, uint16_t handle,
                                      uint8_t val_prop, uint16_t att_val_handle,
                                      uuid_t att_val_uuid) {
  rsi_ble_req_add_att_t new_att = {0};

  //! preparing the attribute service structure
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  new_att.att_uuid.size = 2;
  new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
  new_att.property = RSI_BLE_ATT_PROPERTY_READ;

  //! preparing the characteristic attribute value
  if (att_val_uuid.size == 16) {
    new_att.data_len = 4 + att_val_uuid.size;
    new_att.data[0] = val_prop;
    rsi_uint16_to_2bytes(&new_att.data[2], att_val_handle);
    memcpy(&new_att.data[4], &att_val_uuid.val.val128,
           sizeof(att_val_uuid.val.val128));
  } else {
    new_att.data_len = 4;
    new_att.data[0] = val_prop;
    rsi_uint16_to_2bytes(&new_att.data[2], att_val_handle);
    rsi_uint16_to_2bytes(&new_att.data[4], att_val_uuid.val.val16);
  }

  //! Add attribute to the service
  rsi_ble_add_attribute(&new_att);

  return;
}

/*============================================================================*/
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

static void rsi_ble_add_char_val_att(void *serv_handler, uint16_t handle,
                                     uuid_t att_type_uuid, uint8_t val_prop,
                                     uint8_t *data, uint8_t data_len,
                                     uint8_t auth_read) {
  rsi_ble_req_add_att_t new_att = {0};

  //! preparing the attributes
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  new_att.config_bitmap = auth_read;
  memcpy(&new_att.att_uuid, &att_type_uuid, sizeof(uuid_t));
  new_att.property = val_prop;

  if (data != NULL)
    memcpy(new_att.data, data, RSI_MIN(sizeof(new_att.data), data_len));

  //! preparing the attribute value
  new_att.data_len = data_len;

  //! add attribute to the service
  rsi_ble_add_attribute(&new_att);

  //! check the attribute property with notification
  if (val_prop & RSI_BLE_ATT_PROPERTY_NOTIFY ||
      val_prop & RSI_BLE_ATT_PROPERTY_INDICATE) {
    //! if notification property supports then we need to add client
    //! characteristic service. preparing the client characteristic attribute &
    //! values
    memset(&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler = serv_handler;
    new_att.handle = handle + 1;
    new_att.att_uuid.size = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
    new_att.property = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE;
    new_att.data_len = 2;

    //! add attribute to the service
    rsi_ble_add_attribute(&new_att);
    //! preparing the client characteristic attribute & values
    memset(&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler = serv_handler;
    new_att.handle = handle + 2;
    new_att.att_uuid.size = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_DESP;
    new_att.property = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE;
    new_att.data_len = 2;

    rsi_ble_add_attribute(&new_att);
  }

  return;
}
/*============================================================================*/
/**
 * @fn         rsi_ble_prepare_128bit_uuid
 * @brief      this function is used to prepare the 128bit UUID
 * @param[in]  temp_service,received 128-bit service.
 * @param[out]  temp_uuid,formed 128-bit service structure.
 * @return     none.
 * @section description
 * This function prepares the 128bit UUID
 */
static void rsi_ble_prepare_128bit_uuid(uint8_t temp_service[UUID_SIZE],
                                        uuid_t *temp_uuid) {
  temp_uuid->val.val128.data1 =
      ((temp_service[0] << 24) | (temp_service[1] << 16) |
       (temp_service[2] << 8) | (temp_service[3]));
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
 * @fn         rsi_ble_add_throguhput_test_serv
 * @brief      this function is used to create the Throughput Test Service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Throughput Test
 * service.(UUID:bbb99e70-fff7-46cf-abc7-2d32c71820f2)
 */
static void rsi_ble_add_throghput_test_serv(void) {
  uuid_t new_uuid = {0};
  rsi_ble_resp_add_serv_t new_serv_resp = {0};
  uint8_t throughput_test_service[UUID_SIZE] = {
      0xbb, 0xb9, 0x9e, 0x70, 0xff, 0xf7, 0x46, 0xcf,
      0xab, 0xc7, 0x2d, 0x32, 0xc7, 0x18, 0x20, 0xf2};
  new_uuid.size = UUID_SIZE;
  rsi_ble_prepare_128bit_uuid(throughput_test_service, &new_uuid);
  rsi_ble_add_service(new_uuid, &new_serv_resp);
  throghput_test_serv_response.serv_handler = new_serv_resp.serv_handler;
  throghput_test_serv_response.start_handle = new_serv_resp.start_handle;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_indication_char_serv
 * @brief      this function is used to create the Indication characteristic
 * service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Indication characteristic
 * service.(UUID:6109b631-a643-4a51-83d2-2059700ad49f)
 */
static void rsi_ble_add_indication_char_serv(void) {
  uuid_t new_uuid = {0};
  uint8_t data1[232] = {'0'};
  rsi_ble_att1_val_hndl = 0;
  rsi_ble_att1_val_hndl = throghput_test_serv_response.start_handle;
  uint8_t indication_char_serv[UUID_SIZE] = {0x61, 0x09, 0xb6, 0x31, 0xa6, 0x43,
                                             0x4a, 0x51, 0x83, 0xd2, 0x20, 0x59,
                                             0x70, 0x0a, 0xd4, 0x9f};
  new_uuid.size = UUID_SIZE;
  rsi_ble_prepare_128bit_uuid(indication_char_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throghput_test_serv_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      RSI_BLE_ATT_PROPERTY_INDICATE, rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = throghput_test_serv_response.start_handle + 2;
  rsi_ble_add_char_val_att(throghput_test_serv_response.serv_handler,
                           rsi_ble_att1_val_hndl, new_uuid,
                           RSI_BLE_ATT_PROPERTY_INDICATE, (uint8_t *)&data1,
                           sizeof(data1), 0);
  indications_data_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_notification_char_serv
 * @brief      this function is used to create the Notification characteristic
 * service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Notification characteristic service
 * (UUID:47b73dd6-dee3-4da1-9be0-f5c539a9a4be)
 */
static void rsi_ble_add_notification_char_serv(void) {
  uuid_t new_uuid = {0};
  uint8_t data1[232] = {'0'};
  uint8_t notification_char_serv[UUID_SIZE] = {
      0x47, 0xb7, 0x3d, 0xd6, 0xde, 0xe3, 0x4d, 0xa1,
      0x9b, 0xe0, 0xf5, 0xc5, 0x39, 0xa9, 0xa4, 0xbe};
  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_prepare_128bit_uuid(notification_char_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throghput_test_serv_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      RSI_BLE_ATT_PROPERTY_NOTIFY, rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_add_char_val_att(throghput_test_serv_response.serv_handler,
                           rsi_ble_att1_val_hndl, new_uuid,
                           RSI_BLE_ATT_PROPERTY_NOTIFY, (uint8_t *)&data1,
                           sizeof(data1), 0);
  notifications_data_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_transmission_on_char_serv
 * @brief      this function is used to create the Transmission ON
 * characteristic service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Transmission ON characteristic service
 * (UUID:be6b6be1-cd8a-4106-9181-5ffe2bc67718)
 */
static void rsi_ble_add_transmission_on_char_serv(void) {
  uuid_t new_uuid = {0};

  uint8_t data1[2] = {'0'};
  uint8_t transmission_on_serv[UUID_SIZE] = {0xbe, 0x6b, 0x6b, 0xe1, 0xcd, 0x8a,
                                             0x41, 0x06, 0x91, 0x81, 0x5f, 0xfe,
                                             0x2b, 0xc6, 0x77, 0x18};
  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_prepare_128bit_uuid(transmission_on_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throghput_test_serv_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ |
       RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_WRITE_NO_RESPONSE),
      rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_add_char_val_att(
      throghput_test_serv_response.serv_handler, rsi_ble_att1_val_hndl,
      new_uuid,
      (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ |
       RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_WRITE_NO_RESPONSE),
      (uint8_t *)&data1, sizeof(data1), 0);
  data_transfer_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_throughput_result_char_serv
 * @brief      this function is used to create the Throughput Result
 * characteristic service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Throughput Result characteristic service
 * (UUID:adf32227-b00f-400c-9eeb-b903a6cc291b)
 */
static void rsi_ble_add_throughput_result_char_serv(void) {
  uuid_t new_uuid = {0};

  uint8_t data1[4] = {'0'};
  uint8_t throughput_result_serv[UUID_SIZE] = {
      0xad, 0xf3, 0x22, 0x27, 0xb0, 0x0f, 0x40, 0x0c,
      0x9e, 0xeb, 0xb9, 0x03, 0xa6, 0xcc, 0x29, 0x1b};
  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_prepare_128bit_uuid(throughput_result_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throghput_test_serv_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      (RSI_BLE_ATT_PROPERTY_INDICATE | RSI_BLE_ATT_PROPERTY_READ),
      rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_add_char_val_att(
      throghput_test_serv_response.serv_handler, rsi_ble_att1_val_hndl,
      new_uuid, (RSI_BLE_ATT_PROPERTY_INDICATE | RSI_BLE_ATT_PROPERTY_READ),
      (uint8_t *)&data1, sizeof(data1), 0);
  throughput_result_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_throughput_info_serv
 * @brief      this function is used to create the Throughput Information
 * Service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Throughput Information
 * service.(UUID:ba1e0e9f-4d81-bae3-f748-3ad55da38b46)
 */
static void rsi_ble_add_throughput_info_serv(void) {
  uuid_t new_uuid = {0};
  rsi_ble_resp_add_serv_t new_serv_resp = {0};
  uint8_t throughput_info_service[UUID_SIZE] = {
      0xba, 0x1e, 0x0e, 0x9f, 0x4d, 0x81, 0xba, 0xe3,
      0xf7, 0x48, 0x3a, 0xd5, 0x5d, 0xa3, 0x8b, 0x46};
  new_uuid.size = UUID_SIZE;
  rsi_ble_prepare_128bit_uuid(throughput_info_service, &new_uuid);
  rsi_ble_add_service(new_uuid, &new_serv_resp);
  throughput_info_service_response.serv_handler = new_serv_resp.serv_handler;
  throughput_info_service_response.start_handle = new_serv_resp.start_handle;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_connection_phy_char_serv
 * @brief      this function is used to create the Connection PHY characteristic
 * service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Connection PHY characteristic
 * service.(UUID:00a82b93-0feb-2739-72be-abda1f5993d0)
 */
static void rsi_ble_add_connection_phy_char_serv(void) {
  uuid_t new_uuid = {0};

  uint8_t connection_parameter_data[2] = {0};
  rsi_ble_att1_val_hndl = 0;
  rsi_ble_att1_val_hndl = throughput_info_service_response.start_handle;
  uint8_t connection_phy_char_serv[UUID_SIZE] = {
      0x00, 0xa8, 0x2b, 0x93, 0x0f, 0xeb, 0x27, 0x39,
      0x72, 0xbe, 0xab, 0xda, 0x1f, 0x59, 0x93, 0xd0};
  new_uuid.size = UUID_SIZE;
  rsi_ble_prepare_128bit_uuid(connection_phy_char_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = throughput_info_service_response.start_handle + 2;
  rsi_ble_add_char_val_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl,
      new_uuid, (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      (uint8_t *)&connection_parameter_data, sizeof(connection_parameter_data),
      0);
  connection_phy_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_connection_interval_char_serv
 * @brief      this function is used to create the Connection Interval
 * characteristic service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Connection interval characteristic service
 * (UUID:0a32f5a6-0a6c-4954-f413-a698faf2c664)
 */
static void rsi_ble_add_connection_interval_char_serv(void) {
  uuid_t new_uuid = {0};

  uint8_t connection_parameter_data[2] = {0};
  uint8_t connection_interval_char_serv[UUID_SIZE] = {
      0x0a, 0x32, 0xf5, 0xa6, 0x0a, 0x6c, 0x49, 0x54,
      0xf4, 0x13, 0xa6, 0x98, 0xfa, 0xf2, 0xc6, 0x64};
  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_prepare_128bit_uuid(connection_interval_char_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_add_char_val_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl,
      new_uuid, (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      (uint8_t *)&connection_parameter_data, sizeof(connection_parameter_data),
      0);
  connection_interval_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_slave_latency_char_serv
 * @brief      this function is used to create the Slave Latency characteristic
 * service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Slave Latency characteristic service
 * (UUID:ff629b92-332b-e7f7-975f-0e535872ddae)
 */
static void rsi_ble_add_slave_latency_char_serv(void) {
  uuid_t new_uuid = {0};

  uint8_t connection_parameter_data[2] = {0};
  uint8_t slave_latency_char_serv[UUID_SIZE] = {
      0xff, 0x62, 0x9b, 0x92, 0x33, 0x2b, 0xe7, 0xf7,
      0x97, 0x5f, 0x0e, 0x53, 0x58, 0x72, 0xdd, 0xae};
  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_prepare_128bit_uuid(slave_latency_char_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_add_char_val_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl,
      new_uuid, (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      (uint8_t *)&connection_parameter_data, sizeof(connection_parameter_data),
      0);
  slave_latency_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_supervision_timeout_char_serv
 * @brief      this function is used to create the Supervision timeout
 * characteristic service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create Supervision timeout characteristic service
 * (UUID:67e2c4f2-2f50-914c-a611-adb3727b056d)
 */
static void rsi_ble_add_supervision_timeout_char_serv(void) {
  uuid_t new_uuid = {0};

  uint8_t connection_parameter_data[2] = {0};
  uint8_t supervision_timeout_char_serv[UUID_SIZE] = {
      0x67, 0xe2, 0xc4, 0xf2, 0x2f, 0x50, 0x91, 0x4c,
      0xa6, 0x11, 0xad, 0xb3, 0x72, 0x7b, 0x05, 0x6d};
  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_prepare_128bit_uuid(supervision_timeout_char_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_add_char_val_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl,
      new_uuid, (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      (uint8_t *)&connection_parameter_data, sizeof(connection_parameter_data),
      0);
  supervision_timeout_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_pdu_size_char_serv
 * @brief      this function is used to create the PDU size characteristic
 * service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create PDU size characteristic service
 * (UUID:30cc364a-0739-268c-4926-36f112631e0c)
 */
static void rsi_ble_add_pdu_size_char_serv(void) {
  uuid_t new_uuid = {0};
  uint8_t connection_parameter_data[2] = {0};
  uint8_t pdu_size_char_serv[UUID_SIZE] = {0x30, 0xcc, 0x36, 0x4a, 0x07, 0x39,
                                           0x26, 0x8c, 0x49, 0x26, 0x36, 0xf1,
                                           0x12, 0x63, 0x1e, 0x0c};
  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_prepare_128bit_uuid(pdu_size_char_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_add_char_val_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl,
      new_uuid, (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      (uint8_t *)&connection_parameter_data, sizeof(connection_parameter_data),
      0);
  pdu_size_handle = rsi_ble_att1_val_hndl;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_mtu_size_char_serv
 * @brief      this function is used to create the MTU size characteristic
 * service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create MTU size characteristic service
 * (UUID:3816df2f-d974-d915-d26e-78300f25e86e)
 */
static void rsi_ble_add_mtu_size_char_serv(void) {
  uuid_t new_uuid = {0};
  uint8_t connection_parameter_data[2] = {0};
  uint8_t mtu_size_char_serv[UUID_SIZE] = {0x38, 0x16, 0xdf, 0x2f, 0xd9, 0x74,
                                           0xd9, 0x15, 0xd2, 0x6e, 0x78, 0x30,
                                           0x0f, 0x25, 0xe8, 0x6e};
  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_prepare_128bit_uuid(mtu_size_char_serv, &new_uuid);
  rsi_ble_add_char_serv_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl + 1,
      (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      rsi_ble_att1_val_hndl + 2, new_uuid);

  new_uuid.size = UUID_SIZE;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
  rsi_ble_add_char_val_att(
      throughput_info_service_response.serv_handler, rsi_ble_att1_val_hndl,
      new_uuid, (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
      (uint8_t *)&connection_parameter_data, sizeof(connection_parameter_data),
      0);
  mtu_size_handle = rsi_ble_att1_val_hndl;
}
/*============================================================================*/
/**
 * @fn         rsi_ble_add_throguhput_serv
 * @brief      this function is used to add Throughput service and corresponding
 *             characteristic services.
 * service.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used at application to create new service.
 */
static void rsi_ble_add_throguhput_serv(void) {
  //! adding the "Throughput Test Service" Primary service
  //! (UUID:bbb99e70-fff7-46cf-abc7-2d32c71820f2)
  rsi_ble_add_throghput_test_serv();

  //! adding the "Indications" characteristic service attribute to the
  //! "Throughput Test Service" (UUID:6109b631-a643-4a51-83d2-2059700ad49f)
  rsi_ble_add_indication_char_serv();

  //! adding the "Notifications" characteristic service attribute to the
  //! "Throughput Test Service" (UUID:47b73dd6-dee3-4da1-9be0-f5c539a9a4be)
  rsi_ble_add_notification_char_serv();

  //! adding the "Transmission ON" characteristic service attribute to the
  //! "Throughput Test Service" (UUID:be6b6be1-cd8a-4106-9181-5ffe2bc67718)
  rsi_ble_add_transmission_on_char_serv();

  //! adding the "Throughput Result" characteristic service attribute to the
  //! "Throughput Test Service" (UUID:adf32227-b00f-400c-9eeb-b903a6cc291b)
  rsi_ble_add_throughput_result_char_serv();

  //! adding the "Throughput Information Service" Primary
  //! service(UUID:ba1e0e9f-4d81-bae3-f748-3ad55da38b46)
  rsi_ble_add_throughput_info_serv();

  //! adding the "Connection PHY" characteristic service attribute to the
  //! "Throughput Information Service"
  //!  (UUID:00a82b93-0feb-2739-72be-abda1f5993d0)
  rsi_ble_add_connection_phy_char_serv();

  //! adding the "Connection Interval" characteristic service attribute to the
  //! "Throughput Information Service"
  //! (UUID:0a32f5a6-0a6c-4954-f413-a698faf2c664)
  rsi_ble_add_connection_interval_char_serv();

  //! adding the "Slave Latency " characteristic service attribute to the
  //! "Throughput Information Service"
  //! (UUID:ff629b92-332b-e7f7-975f-0e535872ddae)
  rsi_ble_add_slave_latency_char_serv();

  //! adding the "Supervision timeout" characteristic service attribute to the
  //! "Throughput Information Service"
  //! (UUID:67e2c4f2-2f50-914c-a611-adb3727b056d)
  rsi_ble_add_supervision_timeout_char_serv();

  //! adding the "PDU size" characteristic service attribute to the
  //! "Throughput Information Service"
  //! (UUID:30cc364a-0739-268c-4926-36f112631e0c)
  rsi_ble_add_pdu_size_char_serv();

  //! adding the "MTU size" characteristic service attribute to the
  //! "Throughput Information Service"
  //! (UUID:3816df2f-d974-d915-d26e-78300f25e86e)
  rsi_ble_add_mtu_size_char_serv();
}

/*============================================================================*/
/**
 * @fn         rsi_ble_data_length_change_event
 * @brief      invoked when data length is set
 * @section description
 * This Callback function indicates data length is set
 */
void rsi_ble_data_length_change_event(
    rsi_ble_event_data_length_update_t *rsi_ble_data_length_update) {
  LOG_PRINT("\r\nMax_tx_octets: %d \r\n",
            rsi_ble_data_length_update->MaxTxOctets);
  LOG_PRINT("Max_tx_time: %d \r\n", rsi_ble_data_length_update->MaxTxTime);
  LOG_PRINT("Max_rx_octets: %d \r\n", rsi_ble_data_length_update->MaxRxOctets);
  LOG_PRINT("Max_rx_time: %d \r\n", rsi_ble_data_length_update->MaxRxTime);
  rsi_ble_app_set_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_phy_update_complete_event
 * @brief      when phy update complete event is received
 * @param[out] rsi_ble_event_phy_update_complete  contains the controller
 * support phy information.
 * @return     none.
 * @section description
 * This Callback function indicates PHY update complete event is received
 */
void rsi_ble_phy_update_complete_event(
    rsi_ble_event_phy_update_t *rsi_ble_event_phy_update_complete) {
  memcpy(&rsi_app_phy_update_complete, rsi_ble_event_phy_update_complete,
         sizeof(rsi_ble_event_phy_update_t));

  LOG_PRINT("\r\n Tx Phy rate = %x  and Rx Phy rate = %x \n",
            rsi_ble_event_phy_update_complete->TxPhy,
            rsi_ble_event_phy_update_complete->RxPhy);
  rsi_ble_app_set_event(RSI_APP_EVENT_PHY_UPDATE_COMPLETE);
}
/*============================================================================*/
/**
 * @fn         rsi_ble_on_conn_update_complete_event
 * @brief      invoked when conn update complete event is received
 * @param[out] rsi_ble_event_conn_update_complete contains the controller
 * support conn information.
 * @param[out] resp_status contains the response status (Success or Error code)
 * @return     none.
 * @section description
 * This Callback function indicated the conn update complete event is received
 */
void rsi_ble_on_conn_update_complete_event(
    rsi_ble_event_conn_update_t *rsi_ble_event_conn_update_complete,
    uint16_t resp_status) {
  memcpy(remote_dev_address, rsi_ble_event_conn_update_complete->dev_addr, 6);
  memcpy(&rsi_app_conn_update_complete, rsi_ble_event_conn_update_complete,
         sizeof(rsi_ble_event_conn_update_t));
  latest_connection_interval =
      rsi_ble_event_conn_update_complete->conn_interval;
  latest_connection_interval = latest_connection_interval * 1.25;
  LOG_PRINT("\r\n Connection parameters update completed \n ");
  LOG_PRINT("\r\n Connection interval = %d, \t Latency = %d, \tSupervision "
            "Timeout = %d \n",
            latest_connection_interval,
            rsi_ble_event_conn_update_complete->conn_latency,
            rsi_ble_event_conn_update_complete->timeout);
  if ((conn_params_updated == 0) &&
      (rsi_app_conn_update_complete.conn_interval != 0xC8)) {
    rsi_ble_app_set_event(RSI_CONNECTION_PARAMETERS_EVENT);
  }
  rsi_ble_app_set_event(RSI_BLE_CONN_UPDATE_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_simple_peripheral_on_remote_features_event
 * @brief      invoked when LE remote features event is received.
 * @param[in] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_peripheral_on_remote_features_event(
    rsi_ble_event_remote_features_t *rsi_ble_event_remote_features) {
  LOG_PRINT("\r\nFeature received: %x \n",
            *(uint8_t *)rsi_ble_event_remote_features->remote_features);
  memcpy(&remote_dev_feature, rsi_ble_event_remote_features,
         sizeof(rsi_ble_event_remote_features_t));
  rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked  MTU size event are received
 */
static void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *rsi_ble_mtu) {
  memcpy(&rsi_app_ble_mtu_event, rsi_ble_mtu, sizeof(rsi_ble_event_mtu_t));
  LOG_PRINT("\r\n MTU size received from remote device(%s) is %d\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           rsi_app_ble_mtu_event.dev_addr),
            rsi_app_ble_mtu_event.mtu_size);
  rsi_ble_app_set_event(RSI_BLE_MTU_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are
 * received
 */
static void rsi_ble_on_gatt_write_event(uint16_t event_id,
                                        rsi_ble_event_write_t *rsi_ble_write) {
  memcpy(&app_ble_write_event, rsi_ble_write, sizeof(rsi_ble_event_write_t));
  rsi_ble_app_set_event(RSI_BLE_GATT_WRITE_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_more_data_req_event
 * @brief      its invoked when le more data request is received
 * @param[out] rsi_ble_more_data_evt contains the LE Device Buffer Indication
 * information.
 * @return     none.
 * @section description
 * This callback function is invoked when indication confirmation response is
 * received from the module.
 */
static void rsi_ble_more_data_req_event(
    rsi_ble_event_le_dev_buf_ind_t *rsi_ble_more_data_evt) {
  //! set conn specific event
  rsi_ble_app_set_event(RSI_BLE_MORE_DATA_REQ_EVENT);
}

/*============================================================================*/
/**
 * @fn         ble_on_indicate_confirmation_event
 * @brief      its invoked when indication confirmation event is received.
 * @param[out]  event_status contains the response status \n
 *              0 - Success \n
 *              Non-Zero Value  - Failure \n
 * @param[out]  rsi_ble_event_set_att_rsp contains the profile response
 * information.
 * @return     none.
 * @section description
 * This callback function is invoked when indication confirmation response is
 * received from the module.
 */
static void ble_on_indicate_confirmation_event(
    uint16_t event_status, rsi_ble_set_att_resp_t *rsi_ble_event_set_att_rsp) {
  rsi_ble_app_set_event(RSI_DATA_TRANSMIT_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_throughput_test_app
 * @brief      This application is used for evaluate the application throughput.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This application is used for evaluate the application throughput for
 * Notifications & Indications.
 */
int32_t rsi_ble_throughput_test_app(void) {
  uint32_t status = 0;
  uint8_t adv[31] = {2, 1, 6};
#ifdef RSI_WITH_OS
  rsi_task_handle_t driver_task_handle = NULL;
#endif

#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > BT_GLOBAL_BUFF_LEN)) {
    return status;
  }

  //! SiLabs module initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%X\r\n",
              status);
    return status;
  } else {
    LOG_PRINT("\r\nDevice Initialization Success\r\n");
  }
#endif
#ifdef RSI_WITH_OS
  //! SiLabs module initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n",
              status);
    return status;
  } else {
    LOG_PRINT("\r\nDevice Initialization Success\r\n");
  }
  //! Task created for Driver task
  rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                  (uint8_t *)"driver_task", RSI_DRIVER_TASK_STACK_SIZE, NULL,
                  RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);
#endif

  //! configure the host MCU push button.
  rsi_button_config();

  //! Wireless initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n",
              status);
    return status;
  } else {
    LOG_PRINT("\r\nWireless Initialization Success\r\n");
  }

  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(
      NULL, rsi_ble_on_connect_event, rsi_ble_on_disconnect_event, NULL,
      rsi_ble_phy_update_complete_event, rsi_ble_data_length_change_event,
      rsi_ble_on_enhance_conn_status_event, NULL,
      rsi_ble_on_conn_update_complete_event, NULL);

  //! registering the GAP extended call back functions
  rsi_ble_gap_extended_register_callbacks(
      rsi_ble_simple_peripheral_on_remote_features_event,
      rsi_ble_more_data_req_event);

  //! registering the GATT call back functions
  rsi_ble_gatt_register_callbacks(
      NULL, NULL, NULL, NULL, NULL, NULL, NULL, rsi_ble_on_gatt_write_event,
      NULL, NULL, NULL, rsi_ble_on_mtu_event, NULL, NULL, NULL, NULL, NULL,
      NULL, NULL, NULL, NULL, ble_on_indicate_confirmation_event, NULL);

#if SMP_ENABLE
  //! registering the SMP callback functions
  rsi_ble_smp_register_callbacks(
      rsi_ble_on_smp_request, rsi_ble_on_smp_response, rsi_ble_on_smp_passkey,
      rsi_ble_on_smp_failed, rsi_ble_on_encrypt_started,
      rsi_ble_on_smp_passkey_display, rsi_ble_on_sc_passkey,
      rsi_ble_on_le_ltk_req_event, rsi_ble_on_le_security_keys, NULL, NULL);
#endif

  //! create ble main task if ble protocol is selected
  rsi_semaphore_create(&ble_main_task_sem, 0);

  //!  initializing the application events map
  rsi_ble_app_init_events();

  //! adding simple BLE Throughput service
  rsi_ble_add_throguhput_serv();

  //! Set local name
  status = rsi_bt_set_local_name(RSI_BLE_DEVICE_NAME);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nSet local name failed Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nSet local name Success\r\n");
  }

  //! prepare advertise data //local/device name
  adv[3] = strlen(RSI_BLE_DEVICE_NAME) + 1;
  adv[4] = 9;
  strcpy((char *)&adv[5], RSI_BLE_DEVICE_NAME);

  //! set advertise data
  status = rsi_ble_set_advertise_data(adv, strlen(RSI_BLE_DEVICE_NAME) + 5);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nSet Advertising data failed Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nSet Advertising data Success\r\n");
  }

  //! start advertising
  status = rsi_ble_start_advertising();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n start advertising cmd failed with error code = %lx \n",
              status);
    return status;
  } else {
    LOG_PRINT("\r\n RS9116W device Advertising as Throughput Test\r\n");
  }

  //! call BLE throughput application
  status = rsi_throughput_ble_app();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n Error code = %lx \n", status);
    return status;
  }
  //! Application main loop
  return 0;
}

int rsi_throughput_ble_app() {
  uint32_t status = 0;
  //! waiting for events from controller.
  while (1) {
#ifndef RSI_WITH_OS
    rsi_wireless_driver_task();
#endif
    //! checking for events list
    event_id = rsi_ble_app_get_event();

    if (event_id == -1) {
      rsi_semaphore_wait(&ble_main_task_sem, 0);
      continue;
    }

    switch (event_id) {

    // -------------------------------------------------------------------------
    // This event invokes when connection was completed
    case RSI_BLE_CONN_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);

      //! stop the advertising
      status = rsi_ble_stop_advertising();
      if (status == RSI_SUCCESS) {
        LOG_PRINT("\r\n RS9116W device stops the advertising \n");
      }

      device_connected = 1;

#if (THROUGHPUT_TYPE == NOTIFICATIONS)
      data_sent_handle = notifications_data_handle;
#elif (THROUGHPUT_TYPE == INDICATIONS)
      data_sent_handle = indications_data_handle;
#endif

#ifdef SMP_ENABLE
      //! initiating the SMP pairing process
      status = rsi_ble_smp_pair_request(remote_dev_address,
                                        RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n smp request cmd failed with error code = %lx \n",
                  status);
      }
#endif
      rsi_delay_ms(100);
      //! Setting MTU Exchange event
      status = rsi_ble_mtu_exchange_event(remote_dev_address, MAX_MTU_SIZE);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n mtu request cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when disconnection was completed
    case RSI_BLE_DISCONN_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);

      //! clear all pending events
      rsi_ble_app_clear_all_event();

      device_found = 0;
      conn_params_updated = 0;
      device_connected = 0;

      //! start addvertising
      status = rsi_ble_start_advertising();
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n start advertising cmd failed with error code = %lx \n",
                  status);
      } else {
        LOG_PRINT("\r\n Started Advertising \n");
      }
      rsi_ble_app_set_event(RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT);
    } break;

#if SMP_ENABLE
    // -------------------------------------------------------------------------
    // This event invokes when the SMP request was received
    case RSI_BLE_SMP_REQ_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_REQ_EVENT);

      //! initiating the SMP pairing process
      status = rsi_ble_smp_pair_request(remote_dev_address,
                                        RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n smp pair request cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when the SMP response was received
    case RSI_BLE_SMP_RESP_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_RESP_EVENT);

      //! initiating the SMP pairing process
      status = rsi_ble_smp_pair_response(remote_dev_address,
                                         RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n smp pair response cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when the SMP passkey event was received.
    case RSI_BLE_SMP_PASSKEY_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_PASSKEY_EVENT);

      //! initiating the SMP pairing process
      status = rsi_ble_smp_passkey(remote_dev_address, RSI_BLE_SMP_PASSKEY);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n smp passkey cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a SMP passkey display event was received.
    case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a SMP secure passkey event was received
    case RSI_BLE_SC_PASSKEY_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SC_PASSKEY_EVENT);

      //! initiating the SMP pairing process
      status = rsi_ble_smp_passkey(remote_dev_address, numeric_value);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n secure connection passkey cmd failed with error code = "
                  "%lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a LTK request was received
    case RSI_BLE_LTK_REQ_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_LTK_REQ_EVENT);

      //! Send the local long term key negitive reply
      status = rsi_ble_ltk_req_reply(remote_dev_address, 0, NULL);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n Ltk req reply cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a SMP connection was failed
    case RSI_BLE_SMP_FAILED_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_FAILED_EVENT);

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a device starts the encryption
    case RSI_BLE_ENCRYPT_STARTED_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_ENCRYPT_STARTED_EVENT);

    } break;

#endif

    // -------------------------------------------------------------------------
    // This event invokes when a device received the remote device features
    case RSI_BLE_RECEIVE_REMOTE_FEATURES: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
      if (remote_dev_feature.remote_features[0] & 0x20) {

        //! Sets the TX octets and the TX time
        status = rsi_ble_set_data_len(remote_dev_address, TX_LEN, TX_TIME);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n set data length cmd failed with error code = %lx \n",
                    status);
          rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
        }
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a device received the data length exchange event
    case RSI_APP_EVENT_DATA_LENGTH_CHANGE: {
      //! clear the disconnected event
      rsi_ble_app_clear_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);

      if (remote_dev_feature.remote_features[1] & 0x01) {
        // rsi_delay_ms(100);
        //! Set TX and RX PHY data rate
        status = rsi_ble_setphy((int8_t *)remote_dev_address, TX_PHY_RATE,
                                RX_PHY_RATE, CODED_PHY_RATE);
        if (status != RSI_SUCCESS) {
          if (status != BT_HCI_COMMAND_DISALLOWED) {
            // retry the same command
            rsi_ble_app_set_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
          } else {
            LOG_PRINT("\r\n set phy cmd failed with error code = %lx \n",
                      status);
          }
        }
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a device received the PHY update complete event
    case RSI_APP_EVENT_PHY_UPDATE_COMPLETE: {
      //! clear the phy update complete event.
      rsi_ble_app_clear_event(RSI_APP_EVENT_PHY_UPDATE_COMPLETE);

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a device received the connection parameters
    // update event
    case RSI_BLE_CONN_UPDATE_EVENT: {
      //! clear the connection update event.
      rsi_ble_app_clear_event(RSI_BLE_CONN_UPDATE_EVENT);
      rsi_delay_ms(10);
      //! Update the connection interval attribute value
      status = rsi_ble_set_local_att_value(
          connection_interval_handle, 2,
          (uint8_t *)&rsi_app_conn_update_complete.conn_interval);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nconnetion interval update failure:0x%lX\n", status);
      } else {
        LOG_PRINT("\r\nconnetion interval update success\n");
      }

      //! Update the slave latency attribute value
      status = rsi_ble_set_local_att_value(
          slave_latency_handle, 2,
          (uint8_t *)&rsi_app_conn_update_complete.conn_latency);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nconnetion latency update failure:0x%lX\n", status);
      } else {
        LOG_PRINT("\r\nconnetion latency update success\n");
      }

      //! Update the supervision timeout attribute value
      uint16_t supervision_timeout_var =
          rsi_app_conn_update_complete.timeout / 10;
      status = rsi_ble_set_local_att_value(supervision_timeout_handle, 2,
                                           (uint8_t *)&supervision_timeout_var);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nsupervision timeout update failure:0x%lX\n", status);
      } else {
        LOG_PRINT("\r\nsupervision timeout update success\n");
      }

      //! Update the RX PHY rate attribute value
      status = rsi_ble_set_local_att_value(connection_phy_handle, 2,
                                           &rsi_app_phy_update_complete.RxPhy);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nphy update failure:0x%lX\n", status);
      } else {
        LOG_PRINT("\r\nphy update success\n");
      }
      //! Update the PDU length attribute value
      pdu_length[0] = sizeof(send_buf);
      status = rsi_ble_set_local_att_value(pdu_size_handle, 2, &pdu_length[0]);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nPDU size update failure:0x%lX\n", status);
      } else {
        LOG_PRINT("\r\nPDU size update success\n");
      }

      //! set the data transmit event
      rsi_ble_app_set_event(RSI_DATA_TRANSMIT_EVENT);

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a device received the MTU exchange event
    case RSI_BLE_MTU_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_MTU_EVENT);
      rsi_delay_ms(10);
      //! Configure the buf mode for Notify and WO response commands for the
      //! remote device
      status = rsi_ble_set_wo_resp_notify_buf_info(
          remote_dev_address, DLE_BUFFER_MODE, DLE_BUFFER_COUNT);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n failed to set the buffer configuration mode, "
                  "error:0x%lX \r\n",
                  status);
        break;
      } else {
        LOG_PRINT("\r\n Buf configuration done for notify and set_att cmds buf "
                  "mode = %d , max buff count =%d \n",
                  DLE_BUFFER_MODE, DLE_BUFFER_COUNT);
      }

      //! Update the MTU size attribute value
      status = rsi_ble_set_local_att_value(
          mtu_size_handle, 2, (uint8_t *)&rsi_app_ble_mtu_event.mtu_size);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nMTU size update failure:0x%lX\n", status);
      } else {
        LOG_PRINT("\r\nMTU size update success\n");
      }

      //! Requests the connection parameters change with the remote device
      status = rsi_ble_conn_params_update(
          remote_dev_address, CONN_INTERVAL_DEFAULT_MIN,
          CONN_INTERVAL_DEFAULT_MAX, CONN_LATENCY, SUPERVISION_TIMEOUT);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n conn params update cmd failed with status = %lx \r\n",
                  status);
      } else {
        LOG_PRINT("\r\nDefault Connection intervals are updated\n");
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a device received the data from the remote device
    case RSI_BLE_GATT_WRITE_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE_EVENT);

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a device received the data transmit event
    case RSI_DATA_TRANSMIT_EVENT: {
      if (transmission_on == 1) {
        if (start_timer_var == 0x01) {
          start_timer_var = 0;
          LOG_PRINT("\r\nstarted\r\n");
          start_timer = rsi_hal_gettickcount();
        }
#if (THROUGHPUT_TYPE == NOTIFICATIONS)
        //! send the data
        status = rsi_ble_set_local_att_value(data_sent_handle, sizeof(send_buf),
                                             (uint8_t *)send_buf);
        if (status != RSI_SUCCESS) {
          if (status == RSI_ERROR_BLE_DEV_BUF_FULL) {
            //! clear the served event
            rsi_ble_app_clear_event(RSI_DATA_TRANSMIT_EVENT);
            break;
          } else {
            LOG_PRINT("\r\n write with response failed with status = %lx \r\n",
                      status);
          }
        } else {
          counter = counter + 1;
          ++send_buf[0];
          if (send_buf[0] == LAST_BYTE) {
            send_buf[0] = START_BYTE;
          }
          break;
        }
#elif (THROUGHPUT_TYPE == INDICATIONS)
        //! send the data
        status = rsi_ble_indicate_value(remote_dev_address, data_sent_handle,
                                        sizeof(send_buf), (uint8_t *)send_buf);
        if (status != RSI_SUCCESS) {
          if (status == RSI_ERROR_BLE_DEV_BUF_FULL) {
            //! clear the served event
            rsi_ble_app_clear_event(RSI_DATA_TRANSMIT_EVENT);
            break;
          }
        } else {
          counter = counter + 1;
          ++send_buf[0];
          if (send_buf[0] == LAST_BYTE) {
            send_buf[0] = START_BYTE;
          }
          break;
        }
#endif
      } else {
        //! clear the served event
        rsi_ble_app_clear_event(RSI_DATA_TRANSMIT_EVENT);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a device received the more data request event
    case RSI_BLE_MORE_DATA_REQ_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_MORE_DATA_REQ_EVENT);

      //! Resumeing the data transfer
      rsi_ble_app_set_event(RSI_DATA_TRANSMIT_EVENT);
    } break;

      // -----------------------------------------------------------------------
      // This event invokes when a device received the Notifications enable
      // event
    case RSI_TRANSMISSION_NOTIFICATION_ENABLE_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_TRANSMISSION_NOTIFICATION_ENABLE_EVENT);

      uint8_t enable_notify[1] = {1};

      //! enable Notifications
      status = rsi_ble_set_local_att_value(data_transfer_handle, 1,
                                           &enable_notify[0]);
      if (status == RSI_SUCCESS) {
        LOG_PRINT("\r\n Remote device enabled the notification \n");
        //! set the data tranfer event
        counter = 0;
        start_timer_var = 1;
        //! set the connection parameter
        rsi_ble_app_set_event(RSI_CONNECTION_PARAMETERS_EVENT);
      }

    } break;

      // -----------------------------------------------------------------------
      // This event invokes when a device received the Notifications disable
      // event
    case RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT);

      //! Throughput calculation
      transfer_on = 0;
      stop_timer = rsi_hal_gettickcount();
      timing = (((float)(stop_timer - start_timer)) / 1000);
      throughput = (((float)(counter * 230 * 8)) / timing);

      uint8_t disabel_notify[1] = {0};

      //# if (THROUGHPUT_TYPE == NOTIFICATIONS)
      //! Disable Notifications
      status = rsi_ble_set_local_att_value(data_transfer_handle, 1,
                                           &disabel_notify[0]);
      rsi_delay_ms(100);
      //! Update the throughput attribute value
      status =
          rsi_ble_indicate_value(remote_dev_address, throughput_result_handle,
                                 4, (uint8_t *)&throughput);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n Indication error:0x%lX \n", status);
      }
      LOG_PRINT("\r\nThroughput : %.07f bps\n", throughput);
      LOG_PRINT("\r\nThroughput : %.07f kbps\n", (throughput / 1000));
      LOG_PRINT("\r\n Time duration in sec:%0.2f \n", timing);
      //! set connection parameters event
      rsi_ble_app_set_event(RSI_CONNECTION_PARAMETERS_EVENT);

    } break;

      // -----------------------------------------------------------------------
      // This event invokes when a device received the connection parameter
    case RSI_CONNECTION_PARAMETERS_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_CONNECTION_PARAMETERS_EVENT);
      if (conn_params_updated == 0) {
        minimum_connection_interval = CONN_INTERVAL_DEFAULT_MIN;
        maximum_connection_interval = CONN_INTERVAL_DEFAULT_MAX;
      } else {
        minimum_connection_interval = CONN_INTERVAL_MIN;
        maximum_connection_interval = CONN_INTERVAL_MAX;
      }
      status = rsi_ble_conn_params_update(
          remote_dev_address, minimum_connection_interval,
          maximum_connection_interval, CONN_LATENCY, SUPERVISION_TIMEOUT);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n conn params update cmd failed with status = %lx \r\n",
                  status);
      } else {
        LOG_PRINT("\r\nConnection intervals are updated\n");
      }

    } break;
    default: {
    }
    }
  }
}
/*============================================================================*/
/**
 * @fn         data_transfer
 * @brief      this function is used to start or stop the data transfer
 * @param[in]  transfer_on, start or stop the data transfer
 * @return     none
 * @section description
 * This function is used to start or stop the data transfer to the remote
 * device.
 */
void data_transfer(uint8_t transfer_on) {
  if (transfer_on == 0x01) {
    transmission_on = 1;
    conn_params_updated = 1;
    rsi_ble_app_set_event(RSI_TRANSMISSION_NOTIFICATION_ENABLE_EVENT);
  } else {
    transmission_on = 0;
    conn_params_updated = 0;
    rsi_ble_app_set_event(RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT);
  }
}
/*============================================================================*/
/**
 * @fn         main_loop
 * @brief      Schedules the Driver task.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function schedules the Driver task.
 */
void main_loop(void) {
  while (1) {
    rsi_wireless_driver_task();
  }
}

/*============================================================================*/
/**
 * @fn         main
 * @brief      main function
 * @param[in]  none.
 * @return     none.
 * @section description
 * This is the main function to call Application
 */
int main(void) {

#ifdef RSI_WITH_OS
  int32_t status = 0;
  rsi_task_handle_t bt_task_handle = NULL;
#endif

#ifndef RSI_WITH_OS
  // Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! Call BLE throughput test Application
  rsi_ble_throughput_test_app();

  //! Application main loop
  main_loop(void);

  return 0;
#endif

#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > BT_GLOBAL_BUFF_LEN)) {
    return status;
  }
  // Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create((rsi_task_function_t)rsi_ble_throughput_test_app,
                  (uint8_t *)"ble_task", RSI_BT_TASK_STACK_SIZE, NULL,
                  RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
