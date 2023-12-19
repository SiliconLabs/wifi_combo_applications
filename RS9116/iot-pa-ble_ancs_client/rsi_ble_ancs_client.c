/*******************************************************************************
 * @file  rsi_ble_ancs_client.c
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
/*==============================================================================
 * @brief :  * This application demonstrates how to receive Apple Notification
 Center Service (ANCS)  notifications by configuring the RS9116 NCP device as
 a GATT client.

 * @section Description :
 * ANCS notifications include phone calls, emails, calendar entries, and so
 * on. If the RS9116 NCP device receives the notification related to an
 * incoming call, then it automatically queries the iOS device for caller and
 * Apple ID information. And it also has the provision to print the all
 * received ANCS notifications in the serial teriminal.
 =============================================================================*/

/**
 * Include files
 * */

//! BLE include file to refer BLE APIs
#include "rsi_driver.h"
#include <rsi_ble_apis.h>
#include <rsi_ble_config.h>
#include <rsi_bt_common.h>
#include <rsi_bt_common_apis.h>
#include <string.h>

//! Common include file
#include <rsi_common_apis.h>

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

#ifdef FW_LOGGING_ENABLE
//! Firmware logging includes
#include "sl_fw_logging.h"
#endif

#ifdef FW_LOGGING_ENABLE
//! Memory length of driver updated for firmware logging
#define BT_GLOBAL_BUFF_LEN (15000 + (FW_LOG_QUEUE_SIZE * MAX_FW_LOG_MSG_LEN))
#else
//! Memory length for the driver
#define BT_GLOBAL_BUFF_LEN 15000
#endif

//! Memory to initialize the driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

#ifdef FW_LOGGING_ENABLE
/*=======================================================================*/
//!    Firmware logging configurations
/*=======================================================================*/
//! Firmware logging task defines
#define RSI_FW_TASK_STACK_SIZE (512 * 2)
#define RSI_FW_TASK_PRIORITY 2
//! Firmware logging variables
extern rsi_semaphore_handle_t fw_log_app_sem;
rsi_task_handle_t fw_log_task_handle = NULL;
//! Firmware logging prototypes
void sl_fw_log_callback(uint8_t *log_message, uint16_t log_message_length);
void sl_fw_log_task(void);
#endif

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY 1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY 2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE 3000
void rsi_wireless_driver_task(void);
#endif

//! local device Name
#define RSI_BLE_LOCAL_NAME "ANCS"

//! local device IO capability
#define RSI_BLE_SMP_IO_CAPABILITY 0x03

/*============================================================================*/
//!    Application structures
/*============================================================================*/
typedef struct rsi_ble_event_profile_list_s {
  /**remote device address*/
  uint8_t dev_addr[RSI_DEV_ADDR_LEN];
  /**number of profiles found*/
  uint8_t number_of_profiles;
  uint8_t reserved;
  /**list of found profiles. The maximum value is 5*/
  profile_descriptors_t profile_desc[30];
} rsi_ble_event_profile_list_t;

// GATT attribute descriptors response structure
typedef struct rsi_ble_att_descs_s {
  /** Number of descriptors found */
  uint8_t num_of_att;
  /** Reserved */
  uint8_t reserved[3];
  /** Attribute descriptors list. \n
      The maximum value is 5. */
  att_desc_t att_desc[20];
} rsi_ble_att_descs_t;

/* Fields of Apple Notification */
typedef struct ancNotifChr_s {
  uint8_t ancEventID;
  uint8_t ancEventFlags;
  uint8_t ancCategoryId;
  uint8_t ancCategoryCount;
  uint32_t ancNotificationUID;
} ancNotifChr_t;

// missed call app identifier information
typedef struct app_identifier_s {
  uint8_t length[2];
  uint8_t app_identifier_name[5];
} app_identifier_t;

// missed caller name
typedef struct app_caller_name_s {
  uint8_t length[2];
  uint8_t caller_name[25];
} app_caller_name_t;

//! Application global parameters.
uint8_t char_fetch_var = 0;
uint8_t att_desp_var = 0, att_var = 0;
int16_t ancs_service_handle = 0, notification_source_handle = 0,
        control_point_handle = 0, data_source_handle = 0;
int16_t temp_primary_char_service_handle = 0,
        temp_secondary_char_service_handle = 0;
uint8_t enable_notification[2] = {1};
uuid_t ancs_service = {0};
uuid_t notification_source = {0};
uuid_t control_point = {0};
uuid_t data_source = {0};
uint8_t str_remote_address[18] = {'\0'};
uint8_t adv_data[31] = {0x02, 0x01, 0x05, 0x03, 0x19, 0xC1,
                        0x00, 0x03, 0x03, 0x12, 0x18};

rsi_semaphore_handle_t ble_main_task_sem;
rsi_ble_event_profile_by_uuid_t ble_profiles;
rsi_ble_event_read_by_type1_t ble_char_services;
rsi_ble_event_read_by_type3_t ble_char_services_async;

static uint8_t remote_dev_bd_addr[6] = {0};
static rsi_ble_event_conn_status_t rsi_app_connected_device;
static rsi_ble_event_disconnect_t rsi_app_disconnected_device;
static rsi_ble_event_mtu_t rsi_app_ble_mtu_event;
static rsi_bt_event_encryption_enabled_t encrypt_keys;
static rsi_bt_event_le_ltk_request_t ble_ltk_req;
static rsi_bt_event_le_security_keys_t app_ble_sec_keys;
static uint32_t numeric_value;
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_map1;
static rsi_ble_resp_att_descs_t att_desc_list;
static rsi_ble_event_write_t app_ble_write_event;
static ancNotifChr_t ancs_notification_data;
static app_identifier_t identifier_info;
static app_caller_name_t caller_info;

/*============================================================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events() {
  ble_app_event_map = 0;
  ble_app_event_map1 = 0;
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

  rsi_semaphore_post(&ble_main_task_sem);

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
 * @fn         rsi_ble_central_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_central_on_enhance_conn_status_event(
    rsi_ble_event_enhance_conn_status_t *resp_enh_conn) {
  rsi_app_connected_device.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy(rsi_app_connected_device.dev_addr, resp_enh_conn->dev_addr,
         RSI_DEV_ADDR_LEN);
  rsi_app_connected_device.status = resp_enh_conn->status;
  //! unblock connection semaphore
  rsi_ble_app_set_event(RSI_APP_EVENT_CONNECTED);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_central_on_conn_status_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_central_on_conn_status_event(
    rsi_ble_event_conn_status_t *resp_conn) {
  memcpy(&rsi_app_connected_device, resp_conn,
         sizeof(rsi_ble_event_conn_status_t));
  //! unblock connection semaphore
  rsi_ble_app_set_event(RSI_APP_EVENT_CONNECTED);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_central_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[out]  resp_disconnect, disconnected remote device information
 * @param[out]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
void rsi_ble_central_on_disconnect_event(
    rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason) {
  memcpy(&rsi_app_disconnected_device, resp_disconnect,
         sizeof(rsi_ble_event_disconnect_t));
  LOG_PRINT("\r\nDisconnected reason:0x%x \r\n", reason);
  rsi_ble_app_set_event(RSI_APP_EVENT_DISCONNECTED);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      invoked  when an MTU size event is received
 * @param[out]  rsi_ble_mtu, it indicates MTU size.
 * @return     none.
 * @section description
 * This callback function is invoked  when an MTU size event is received
 */
static void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *rsi_ble_mtu) {
  memcpy(&rsi_app_ble_mtu_event, rsi_ble_mtu, sizeof(rsi_ble_event_mtu_t));
  rsi_ble_app_set_event(RSI_APP_BLE_MTU_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_gatt_error_event
 * @brief      its invoked when the gatt error event is received
 * @param[out]  resp_status, it indicates the gatt error response
 * @param[out]  rsi_ble_gatt_error, it indicates the gatt error information.
 * @return     none.
 * @section description
 * This callback function is invoked  when the gatt error is received
 */
static void
rsi_ble_on_gatt_error_event(uint16_t resp_status,
                            rsi_ble_event_error_resp_t *rsi_ble_gatt_error) {
  if (resp_status != 0X4A02) {
    rsi_ble_app_set_event(RSI_APP_BLE_GATT_SERVICE_DESCRIPTOR);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_profiles_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] rsi_ble_event_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void
rsi_ble_profile_event(uint16_t event_status,
                      rsi_ble_event_profile_by_uuid_t *rsi_ble_event_profile) {
  UNUSED_PARAMETER(event_status); // This statement is added only to resolve
                                  // compilation warning, value is unchanged
  memcpy(&ble_profiles, rsi_ble_event_profile,
         sizeof(rsi_ble_event_profile_by_uuid_t));
  rsi_ble_app_set_event(RSI_APP_BLE_GATT_PROFILES);
  return;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_char_services_event
 * @brief      invoked when response is received for characteristic services
 * details
 * @param[out]  resp_status, it indicates the response status
 * @param[out] rsi_ble_event_char_services, list of characteristic services.
 * @return     none
 * @section description
 */

static void rsi_ble_char_services_event(
    uint16_t resp_status,
    rsi_ble_event_read_by_type3_t *rsi_ble_event_char_services_uuid) {
  if (resp_status != 0x4A0A) {
    memcpy(&ble_char_services_async, rsi_ble_event_char_services_uuid,
           sizeof(rsi_ble_event_read_by_type3_t));
    ++char_fetch_var;
    rsi_ble_app_set_event(RSI_APP_BLE_GATT_CHAR_SERVICES);
  }
}
// ble_on_read_by_char_services_event
static void ble_on_read_by_char_services_event(
    uint16_t resp_status,
    rsi_ble_event_read_by_type1_t *rsi_ble_event_char_services_uuid) {
  if (resp_status != 0x4A0A) {
    memcpy(&ble_char_services, rsi_ble_event_char_services_uuid,
           sizeof(rsi_ble_event_read_by_type1_t));
    ++char_fetch_var;
    rsi_ble_app_set_event(RSI_APP_BLE_GATT_CHAR_SERVICES);
  }
}

/*============================================================================*/
/**
 * @fn         ble_on_att_desc_event
 * @brief      invoked when response is received for attribute descriptor
 * details
 * @param[out]  resp_status, it indicates the response status
 * @param[out] rsi_ble_resp_att_desc, list of attribute descriptor.
 * @return     none
 * @section description
 */

static void
ble_on_att_desc_event(uint16_t resp_status,
                      rsi_ble_resp_att_descs_t *rsi_ble_resp_att_desc) {
  if (resp_status != 0x4e60) {
    memcpy(&att_desc_list, rsi_ble_resp_att_desc,
           sizeof(rsi_ble_resp_att_descs_t));
    ++att_desp_var;
    att_var = 1;
  }
  rsi_ble_app_set_event(RSI_APP_BLE_GATT_SERVICE_RESP_DESCRIPTOR);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[out]  resp_status, it indicates the response status
 * @param[out]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are
 * received
 */
static void rsi_ble_on_gatt_write_event(uint16_t resp_status,
                                        rsi_ble_event_write_t *rsi_ble_write) {
  memcpy(&app_ble_write_event, rsi_ble_write, sizeof(rsi_ble_event_write_t));
  rsi_ble_app_set_event(RSI_APP_BLE_GATT_WRITE_EVENT);
}

/*============================================================================*/
//!    ANCS services
/*============================================================================*/
//! "ANCS service"  service
//! (UUID:7905F431-B5CE-4E99-A40F-4B1E122D00D0)
uint8_t ancs_primary_service[UUID_SIZE] = {0x79, 0x05, 0xF4, 0x31, 0xB5, 0xCE,
                                           0x4E, 0x99, 0xA4, 0x0F, 0x4B, 0x1E,
                                           0x12, 0x2D, 0x00, 0xD0};

//! "Notification source" characteristic service
//! (UUID:9FBF120D-6301-42D9-8C58-25E699A21DBD)
uint8_t notification_source_char_serv[UUID_SIZE] = {
    0x9f, 0xbf, 0x12, 0x0d, 0x63, 0x01, 0x42, 0xd9,
    0x8c, 0x58, 0x25, 0xe6, 0x99, 0xa2, 0x1d, 0xbd};

//! "Control Point" characteristic service
//! (UUID:69D1D8F3-45E1-49A8-9821-9BBDFDAAD9D9)
uint8_t contol_point_char_serv[UUID_SIZE] = {0x69, 0xd1, 0xd8, 0xf3, 0x45, 0xe1,
                                             0x49, 0xa8, 0x98, 0x21, 0x9b, 0xbd,
                                             0xfd, 0xaa, 0xd9, 0xd9};

//! "Data source" characteristic service
//! (UUID:22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB)
uint8_t data_source_char_serv[UUID_SIZE] = {0x22, 0xea, 0xc6, 0xe9, 0x24, 0xd6,
                                            0x4b, 0xb5, 0xbe, 0x44, 0xb3, 0x6a,
                                            0xce, 0x7c, 0x7b, 0xfb};

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_request
 * @brief      its invoked when smp request event is received.
 * @param[in]  remote_dev_bd_addr, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP request events is received(we are
 * in Master mode) Note: slave requested to start SMP request, we have to send
 * SMP request command
 */
void rsi_ble_on_smp_request(rsi_bt_event_smp_req_t *event_smp_req) {
  memcpy(remote_dev_bd_addr, event_smp_req->dev_addr, 6);
  LOG_PRINT("\r\nsmp_req - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           event_smp_req->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_REQ_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_response
 * @brief      its invoked when smp response event is received.
 * @param[in]  remote_dev_bd_addr, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP response events is received(we are
 * in slave mode) Note: Master initiated SMP protocol, we have to send SMP
 * response command
 */
void rsi_ble_on_smp_response(rsi_bt_event_smp_resp_t *bt_event_smp) {
  memcpy(remote_dev_bd_addr, bt_event_smp->dev_addr, 6);
  LOG_PRINT("\r\nsmp_resp - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           bt_event_smp->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_RSP_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_passkey
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_bd_addr, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_smp_passkey(rsi_bt_event_smp_passkey_t *event_smp_passkey) {
  memcpy(remote_dev_bd_addr, event_smp_passkey->dev_addr, 6);
  LOG_PRINT("\r\nsmp_passkey - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           event_smp_passkey->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_PASSKEY_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_failed
 * @brief      its invoked when smp failed event is received.
 * @param[in]  remote_dev_bd_addr, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP failed events is received
 */
void rsi_ble_on_smp_failed(uint16_t status,
                           rsi_bt_event_smp_failed_t *event_smp_failed) {
  memcpy(remote_dev_bd_addr, event_smp_failed->dev_addr, 6);
  LOG_PRINT("\r\nsmp_failed status: 0x%x, str_remote_address: %s\r\n", status,
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           event_smp_failed->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_FAILED_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_smp_passkey_display
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_bd_addr, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_smp_passkey_display(
    rsi_bt_event_smp_passkey_display_t *smp_passkey_display) {
  memcpy(remote_dev_bd_addr, smp_passkey_display->dev_addr, 6);
  LOG_PRINT("\r\nremote addr: %s, passkey: %s \r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address,
                                           smp_passkey_display->dev_addr),
            smp_passkey_display->passkey);
  rsi_ble_app_set_event(RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
}

void rsi_ble_on_sc_passkey(rsi_bt_event_sc_passkey_t *sc_passkey) {
  memcpy(remote_dev_bd_addr, sc_passkey->dev_addr, 6);
  LOG_PRINT(
      " Pass_key event remote addr: %s, passkey: %06ld \r\n",
      rsi_6byte_dev_address_to_ascii(str_remote_address, sc_passkey->dev_addr),
      sc_passkey->passkey);
  numeric_value = sc_passkey->passkey;
  rsi_ble_app_set_event(RSI_BLE_SC_PASSKEY_EVENT);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_on_encrypt_started
 * @brief      its invoked when encryption started event is received.
 * @param[in]  remote_dev_bd_addr, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when encryption started events is received
 */
void rsi_ble_on_encrypt_started(
    uint16_t status, rsi_bt_event_encryption_enabled_t *enc_enabled) {
  LOG_PRINT("\r\nstart encrypt status: %d \r\n", status);
  memcpy(&encrypt_keys, enc_enabled, sizeof(rsi_bt_event_encryption_enabled_t));
  rsi_ble_app_set_event(RSI_BLE_ENCRYPT_STARTED_EVENT);
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
  LOG_PRINT(" \r\nrsi_ble_on_le_ltk_req_event \r\n");
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
  LOG_PRINT("\r\nremote_ediv: 0x%x\r\n", app_ble_sec_keys.remote_ediv);
}

/*============================================================================*/
/**
 * @fn         incoming_call_data_extraction
 * @brief      prints the received mobile information.
 * @param[in]  missed_call_info, received mobile information
 * @return    status.
 * @section description
 * This function is used to prints the received mobile information.
 */
void incoming_call_data_extraction(rsi_ble_event_write_t missed_call_info) {
  uint8_t temp_notification_uid[4] = {0};
  uint32_t notification_var = 0;
  LOG_PRINT("\r\nResponse:- \r\n");
  LOG_PRINT("\r\nGet Notification Attributes \r\n");
  memcpy(&temp_notification_uid[0], &missed_call_info.att_value[1], 4);
  notification_var = rsi_bytes4R_to_uint32(&temp_notification_uid[0]);
  LOG_PRINT("\r\nNotification UID:%lu\r\n", notification_var);
  LOG_PRINT("\r\nAttributes:\r\n");
  int a = 5;
  uint8_t call_length = 0;
  while (a < missed_call_info.length) {
    if (missed_call_info.att_value[a] == APP_IDENTIFIER) {
      memcpy(&identifier_info.length,
             &missed_call_info.att_value[a + NEXT_HANDLE], 2);
      memcpy(&identifier_info.app_identifier_name[0],
             &missed_call_info.att_value[a + 3], identifier_info.length[0]);
      a = a + identifier_info.length[0] + 3;
      LOG_PRINT("\r\nApp ID:");
      for (int b = 0; b < identifier_info.length[0]; b++) {
        LOG_PRINT("%c", identifier_info.app_identifier_name[b]);
      }
      LOG_PRINT("\r\n");
    } else if (missed_call_info.att_value[a] == TITLE) {
      memcpy(&caller_info.length, &missed_call_info.att_value[a + NEXT_HANDLE],
             1);
      call_length = caller_info.length[0];
      memcpy(&caller_info.caller_name[0], &missed_call_info.att_value[a + 3],
             call_length);
      LOG_PRINT("\r\nCaller:");
      for (int b = 0; b < call_length; b++) {
        LOG_PRINT("%c", caller_info.caller_name[b]);
      }
      LOG_PRINT("\r\n");
      break;
    }
  }
}

/*============================================================================*/
/**
 * @fn         received_packet_processing
 * @brief      Process the received data from a remote device
 * @param[in]  none
 * @return    status.
 * @section description
 * This function is used to process the received data from a remote device
 */
static void received_packet_processing(uint8_t ancCategoryId,
                                       uint8_t ancCategoryCount) {
  switch (ancCategoryId) {
  case ANC_CAT_OTHER: {
    LOG_PRINT("\r\nYou have %u new notifications\r\n", ancCategoryCount);
  } break;
  case ANC_CAT_INCOMINGCALL: {
    if (ancCategoryCount > 0) {
      LOG_PRINT("\r\nYou have %u new incoming calls\r\n", ancCategoryCount);
      rsi_delay_ms(2);
      rsi_ble_app_set_event(RSI_APP_INCOMING_CALL_INFORMATION);
    }
  } break;
  case ANC_CAT_MISSEDCALL: {
    LOG_PRINT("\r\nYou have %u new missed calls\r\n", ancCategoryCount);
  } break;
  case ANC_CAT_VOICEMAIL: {
    LOG_PRINT("\r\nYou have %u new voice mails\r\n", ancCategoryCount);
  } break;
  case ANC_CAT_SOCIAL: {
    LOG_PRINT("\r\nYou have %u new text messages\r\n", ancCategoryCount);
  } break;
  case ANC_CAT_SCHEDULE: {
    LOG_PRINT("\r\nYou have %u new appointments\r\n", ancCategoryCount);
  } break;
  case ANC_CAT_EMAIL: {
    LOG_PRINT("\r\nYou have %u new emails\r\n", ancCategoryCount);
  } break;
  case ANC_CAT_NEWS: {
    LOG_PRINT("\r\nYou have %u news waiting\r\n", ancCategoryCount);
  } break;
  case ANC_CAT_HEALTHANDFITNESS: {
    LOG_PRINT("\r\nYou have %u new health and fitness notifications\r\n",
              ancCategoryCount);
  } break;
  case ANC_CAT_BUSINESSANDFINANCE: {
    LOG_PRINT("\r\nYou have %u new business and finance notifications\r\n",
              ancCategoryCount);
  } break;
  case ANC_CAT_LOCATION: {
    LOG_PRINT("\r\nYou have %u new location notifications\r\n",
              ancCategoryCount);
  } break;
  case ANC_CAT_ENTERTAINMENT: {
    LOG_PRINT("\r\nYou have %u new entertainment notifications\r\n",
              ancCategoryCount);
  } break;

  default: {
  }
  }
}

/*============================================================================*/
/**
 * @fn         rsi_ble_ancs_client
 * @brief      Transfer the ANCS compatible data  to the remote device
 * @param[in]  none
 * @return    status.
 * @section description
 * This function is used to transfer the ANCS compatible data  to the remote
 * device.
 */
int32_t rsi_ble_ancs_client(void) {
  int32_t status = 0;
  int32_t temp_event_map = 0;
  /* Name */
  adv_data[11] = strlen(RSI_BLE_LOCAL_NAME) + 1;
  adv_data[12] = 0x09;
  strcpy((char *)&adv_data[13], (char *)RSI_BLE_LOCAL_NAME);

#ifdef RSI_WITH_OS
  rsi_task_handle_t driver_task_handle = NULL;
#endif
#ifdef FW_LOGGING_ENABLE
  // Fw log component level
  sl_fw_log_level_t fw_component_log_level;
#endif
#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > BT_GLOBAL_BUFF_LEN)) {
    return status;
  }

  //! SiLabs module intialisation
  status = rsi_device_init(LOAD_NWP_FW);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n",
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

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n",
              status);
    return status;
  } else {
    LOG_PRINT("\r\nWireless Initialization Success\r\n");
  }
#ifdef FW_LOGGING_ENABLE
  //! Set log levels for firmware components
  sl_set_fw_component_log_levels(&fw_component_log_level);

  //! Configure firmware logging
  status = sl_fw_log_configure(FW_LOG_ENABLE, FW_TSF_GRANULARITY_US,
                               &fw_component_log_level, FW_LOG_BUFFER_SIZE,
                               sl_fw_log_callback);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nFirmware Logging Init Failed\r\n");
  }
#ifdef RSI_WITH_OS
  //! Create firmware logging semaphore
  rsi_semaphore_create(&fw_log_app_sem, 0);
  //! Create firmware logging task
  rsi_task_create((rsi_task_function_t)sl_fw_log_task, (uint8_t *)"fw_log_task",
                  RSI_FW_TASK_STACK_SIZE, NULL, RSI_FW_TASK_PRIORITY,
                  &fw_log_task_handle);
#endif
#endif

  //! BLE register GAP callbacks
  rsi_ble_gap_register_callbacks(NULL,
                                 rsi_ble_central_on_conn_status_event,
                                 rsi_ble_central_on_disconnect_event,
                                 NULL,
                                 NULL,
                                 NULL,
                                 rsi_ble_central_on_enhance_conn_status_event,
                                 NULL,
                                 NULL,
                                 NULL);

  //! BLE register GATT callbacks
  rsi_ble_gatt_register_callbacks(NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  ble_on_att_desc_event,
                                  NULL,
                                  NULL,
                                  rsi_ble_on_gatt_write_event,
                                  NULL,
                                  NULL,
                                  NULL,
                                  rsi_ble_on_mtu_event,
                                  rsi_ble_on_gatt_error_event,
                                  NULL,
                                  NULL,
                                  rsi_ble_profile_event,
                                  ble_on_read_by_char_services_event,
                                  NULL,
                                  rsi_ble_char_services_event,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL);

  rsi_ble_smp_register_callbacks(rsi_ble_on_smp_request,
                                 rsi_ble_on_smp_response,
                                 rsi_ble_on_smp_passkey,
                                 rsi_ble_on_smp_failed,
                                 rsi_ble_on_encrypt_started,
                                 rsi_ble_on_smp_passkey_display,
                                 rsi_ble_on_sc_passkey,
                                 rsi_ble_on_le_ltk_req_event,
                                 rsi_ble_on_le_security_keys,
                                 NULL,
                                 NULL);

  rsi_semaphore_create(&ble_main_task_sem, 0);

  //! initialize the event map
  rsi_ble_app_init_events();

  //!  Prepare 128bit UUID for ANCS primary service
  rsi_ble_prepare_128bit_uuid(ancs_primary_service, &ancs_service);

  //!  Prepare 128bit UUID for Notification source characteristic service
  rsi_ble_prepare_128bit_uuid(notification_source_char_serv,
                              &notification_source);

  //!  Prepare 128bit UUID for Control Point characteristic service
  rsi_ble_prepare_128bit_uuid(contol_point_char_serv, &control_point);

  //!  Prepare 128bit UUID for Data source characteristic service
  rsi_ble_prepare_128bit_uuid(data_source_char_serv, &data_source);

  status = rsi_bt_set_local_name((uint8_t *)RSI_BLE_LOCAL_NAME);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nSet Local Name, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nSet Local Name Success\r\n");
  }
  //! set advertise data
  status = rsi_ble_set_advertise_data(adv_data, strlen(RSI_BLE_LOCAL_NAME) +
                                                    ADV_FLAGS_DATA_LENGTH);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nSet Advertise Data Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nSet Advertise Data Success\r\n");
  }

  //! Set scan response data
  status = rsi_ble_set_scan_response_data(adv_data, 0);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nSet Scan Response Data Failed, Error Code : 0x%lX\r\n",
              status);
    return status;
  } else {
    LOG_PRINT("\r\nSet Scan Response Data Success\r\n");
  }

  //! start the advertising
  status = rsi_ble_start_advertising();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nStart Advertising Failed, Error Code : 0x%lX\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nAdvertising\r\n");
  }

  while (1) {
    //! Application main loop
#ifndef RSI_WITH_OS
    //! run the non os scheduler
    rsi_wireless_driver_task();
#endif
    //! checking for received events
    temp_event_map = rsi_ble_app_get_event();
    if (temp_event_map == RSI_FAILURE) {
      //! if events are not received, loop will be continued
      rsi_semaphore_wait(&ble_main_task_sem, 0);
      continue;
    }

    switch (temp_event_map) {

    // -------------------------------------------------------------------------
    // This event indicates that the MTU event is received
    case RSI_APP_BLE_MTU_EVENT: {
      rsi_ble_app_clear_event(RSI_APP_BLE_MTU_EVENT);
      LOG_PRINT("\r\nMTU size received from a remote device is %d\r\n",
                rsi_app_ble_mtu_event.mtu_size);
      rsi_delay_ms(1);
      status = rsi_ble_smp_pair_request(rsi_app_connected_device.dev_addr,
                                        RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nsmp request cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event indicates that a GATT service at the remote GATT database was
    // discovered
    case RSI_APP_BLE_GATT_PROFILES: {
      rsi_ble_app_clear_event(RSI_APP_BLE_GATT_PROFILES);
      ancs_service_handle = (*((uint16_t *)ble_profiles.start_handle));
      control_point.size = UUID_SIZE;
      status = rsi_ble_get_char_services_async(
          remote_dev_bd_addr, ancs_service_handle, 0xFFFF, NULL);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nCharacteristic service fetch failure:0x%lX\r\n", status);
      }
    } break;

    // -------------------------------------------------------------------------
    // This event indicates that a GATT characteristic service at the remote
    // GATT database was discovered
    case RSI_APP_BLE_GATT_CHAR_SERVICES: {

      for (int temp_char_var = 0;
           temp_char_var <= ble_char_services.num_of_services;
           temp_char_var++) {
        if (memcmp(&ble_char_services.char_services[temp_char_var]
                        .char_data.char_uuid.val.val128,
                   &control_point.val.val128, sizeof(uuid128_t)) == 0) {
          memcpy(&control_point_handle,
                 &ble_char_services.char_services[temp_char_var].handle, 2);
        } else if (memcmp(&ble_char_services.char_services[temp_char_var]
                               .char_data.char_uuid.val.val128,
                          &notification_source.val.val128,
                          sizeof(uuid128_t)) == 0) {
          memcpy(&notification_source_handle,
                 &ble_char_services.char_services[temp_char_var].handle, 2);
        } else if (memcmp(&ble_char_services.char_services[temp_char_var]
                               .char_data.char_uuid.val.val128,
                          &data_source.val.val128, sizeof(uuid128_t)) == 0) {
          memcpy(&data_source_handle,
                 &ble_char_services.char_services[temp_char_var].handle, 2);
        }
      }

      status = rsi_ble_get_att_descriptors(
          remote_dev_bd_addr, control_point_handle + 2,
          notification_source_handle - 1, NULL);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nattribute descriptor fetching failure:0x%lX\r\n",
                  status);
      }
      rsi_ble_app_clear_event(RSI_APP_BLE_GATT_CHAR_SERVICES);
    } break;

    // -------------------------------------------------------------------------
    // This event indicates that a GATT characteristic service descriptors at
    // the remote GATT database was discovered
    case RSI_APP_BLE_GATT_SERVICE_RESP_DESCRIPTOR: {

      rsi_ble_app_clear_event(RSI_APP_BLE_GATT_SERVICE_RESP_DESCRIPTOR);
      for (int ix = 0; ix < att_desc_list.num_of_att; ix++) {
        if ((att_desc_list.att_desc[ix].att_type_uuid.val.val16 ==
             CLIENT_CHAR_CONFIGURATION) ||
            (att_desc_list.att_desc[ix].att_type_uuid.val.val16 ==
             CHAR_EXTENDED_PROPERTIES)) {
          //! Enable the notification
          status = rsi_ble_set_att_value(
              remote_dev_bd_addr,
              (*((uint16_t *)att_desc_list.att_desc[ix].handle)), 2,
              &enable_notification[0]);
          if (status != RSI_SUCCESS) {
            LOG_PRINT("\r\nset att value failure:0x%lX\r\n", status);
          } else {
            if (att_desp_var == THIRD_ITERATION) {
              LOG_PRINT("\r\nNotifications are enabled\r\n");
            }
          }
        }
      }
      if (att_desp_var == FIRST_ITERATION) {
        memcpy(&temp_primary_char_service_handle, &notification_source_handle,
               2);
        memcpy(&temp_secondary_char_service_handle, &data_source_handle, 2);
      } else if (att_desp_var == SECOND_ITERATION) {
        memcpy(&temp_primary_char_service_handle, &data_source_handle, 2);
        memcpy(&temp_secondary_char_service_handle, &data_source_handle, 2);
        temp_secondary_char_service_handle =
            temp_secondary_char_service_handle + 4;
      } else {
        break;
      }
      //! Get the characteristic descriptors list of the Control point
      //! characteristic service
      status = rsi_ble_get_att_descriptors(
          remote_dev_bd_addr, temp_primary_char_service_handle + 2,
          temp_secondary_char_service_handle - 1, NULL);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nattribute descriptor fetching failure:0x%lX\r\n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event indicates that the received data from the remote device
    case RSI_APP_BLE_GATT_WRITE_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_APP_BLE_GATT_WRITE_EVENT);
      if (app_ble_write_event.length == RECEIVED_REQ_DATA_LEN) {
        memcpy(&ancs_notification_data.ancEventID,
               &app_ble_write_event.att_value[0], ID_LEN);
        memcpy(&ancs_notification_data.ancEventFlags,
               &app_ble_write_event.att_value[1], ID_LEN);
        memcpy(&ancs_notification_data.ancCategoryId,
               &app_ble_write_event.att_value[2], ID_LEN);
        memcpy(&ancs_notification_data.ancCategoryCount,
               &app_ble_write_event.att_value[3], ID_LEN);
        memcpy(&ancs_notification_data.ancNotificationUID,
               &app_ble_write_event.att_value[4], NOTIFICATION_ID_LEN);
        received_packet_processing(ancs_notification_data.ancCategoryId,
                                   ancs_notification_data.ancCategoryCount);
      } else {
        if (app_ble_write_event.handle[0] ==
            (data_source_handle + NEXT_HANDLE)) {
          //! prints the received mobile information
          incoming_call_data_extraction(app_ble_write_event);
        }
      }

    } break;

    // -------------------------------------------------------------------------
    // This event indicates that the missed call is received
    case RSI_APP_INCOMING_CALL_INFORMATION: {
      rsi_ble_app_clear_event(RSI_APP_INCOMING_CALL_INFORMATION);
      uint8_t missed_call_req_info[9] = {0};
      missed_call_req_info[0] = GET_NOTIFICATION_ATTRIBUTE;
      memcpy(&missed_call_req_info[1],
             &ancs_notification_data.ancNotificationUID, NOTIFICATION_ID_LEN);
      missed_call_req_info[5] = APP_IDENTIFIER;
      missed_call_req_info[6] = TITLE;
      memcpy(&missed_call_req_info[7], (uint8_t *)TITLE_LEN, CMD_ID_LEN);
      //! request the mobile information
      status = rsi_ble_set_att_value(
          remote_dev_bd_addr, control_point_handle + NEXT_HANDLE,
          INCOMING_CALL_REQ_LEN, &missed_call_req_info[0]);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nset att value failure:0x%lX\r\n", status);
      } else {
        LOG_PRINT("\r\nData sent to a remote device\r\n");
      }

    } break;

    // -------------------------------------------------------------------------
    // This event indicates that the connection was completed
    case RSI_APP_EVENT_CONNECTED: {
      //! clear the connected event.
      rsi_ble_app_clear_event(RSI_APP_EVENT_CONNECTED);
      rsi_6byte_dev_address_to_ascii(str_remote_address,
                                     rsi_app_connected_device.dev_addr);
      LOG_PRINT("\r\nModule connected to address : %s \r\n",
                str_remote_address);
      memcpy(&remote_dev_bd_addr, &rsi_app_connected_device.dev_addr, 6);
      //! Request the MTU
      status = rsi_ble_mtu_exchange_event(rsi_app_connected_device.dev_addr,
                                          MAX_MTU_SIZE);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nsmp request cmd failed with error code = %lx \n",
                  status);
      }
    } break;

    // -------------------------------------------------------------------------
    // This event indicates that the disconnection was completed
    case RSI_APP_EVENT_DISCONNECTED: {
      //! clear the disconnected event.
      rsi_ble_app_clear_event(RSI_APP_EVENT_DISCONNECTED);
      LOG_PRINT("\r\nModule got Disconnected\r\n");
      //! start scanning
      status = rsi_ble_start_advertising();
      if (status != RSI_SUCCESS) {
        LOG_PRINT("start_scanning status: 0x%lX\r\n", status);
      }
      else{
          LOG_PRINT("\r\nRestarted advertising\r\n");
      }
    } break;

    // -------------------------------------------------------------------------
    // This event invokes when the SMP request was received
    case RSI_BLE_SMP_REQ_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_REQ_EVENT);

      //! initiating the SMP pairing process
      status = rsi_ble_smp_pair_request(rsi_app_connected_device.dev_addr,
                                        RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nsmp pair request cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when the SMP response was received
    case RSI_BLE_SMP_RSP_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_RSP_EVENT);

      //! initiating the SMP pairing process
      status = rsi_ble_smp_pair_response(rsi_app_connected_device.dev_addr,
                                         RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nsmp pair response cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when the SMP passkey event was received.
    case RSI_BLE_SMP_PASSKEY_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_PASSKEY_EVENT);

      //! initiating the SMP pairing process
      status = rsi_ble_smp_passkey(rsi_app_connected_device.dev_addr,
                                   RSI_BLE_SMP_PASSKEY);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nsmp passkey cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a SMP passkey display event was received.
    case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);

    } break;

    case RSI_BLE_SC_PASSKEY_EVENT: {
      LOG_PRINT("\r\nIn SC passkey event\r\n");
      rsi_ble_app_clear_event(RSI_BLE_SC_PASSKEY_EVENT);
      status = rsi_ble_smp_passkey(remote_dev_bd_addr, numeric_value);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\n smp passkey cmd failed with reason = %lx \n", status);
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
      ancs_service.size = UUID_SIZE;
      //! Get the supported profiles
      status =
          rsi_ble_get_profile_async(remote_dev_bd_addr, ancs_service, NULL);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nprofile fetch failure:0x%lX\r\n", status);
      }
    } break;

    // -------------------------------------------------------------------------
    // This event invokes when a LTK request was received
    case RSI_BLE_LTK_REQ_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_LTK_REQ_EVENT);

      //! Send the local long term key negitive reply
      status = rsi_ble_ltk_req_reply(remote_dev_bd_addr, 0, NULL);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nLtk req reply cmd failed with error code = %lx \n",
                  status);
      }

    } break;

    default: {
    }
    }
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
  int32_t status;
#ifdef RSI_WITH_OS
  rsi_task_handle_t bt_task_handle = NULL;
#endif

#ifndef RSI_WITH_OS
  // Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! Call BLE Central application
  status = rsi_ble_ancs_client();

  //! Application main loop
  main_loop();

  return status;
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
  rsi_task_create((rsi_task_function_t)(int32_t)rsi_ble_ancs_client,
                  (uint8_t *)"ble_task", RSI_BT_TASK_STACK_SIZE, NULL,
                  RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
