/*******************************************************************************
 * @file  app.c
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
 * @brief : This file contains example application for BLE OTA Firmware Upgrade
 * @section Description :
 * application demonstrates the procedure to update the SiWx91x module's
 firmware by receiving a firmware file from a remote device via BLE. The
 application has the provision to upgrade both TA and M4 firmwares.
 =================================================================================*/

/**
 * Include files
 * */
//! SL Wi-Fi SDK includes
#include "cmsis_os2.h"
#include "errno.h"
#include "sl_board_configuration.h"
#include "sl_constants.h"
#include "sl_net.h"
#include "sl_si91x_driver.h"
#include "sl_utility.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"

//! BLE include file to refer BLE APIs
#include <stdio.h>
#include <string.h>

#include "ble_config.h"
#include "rsi_ble.h"
#include "rsi_ble_apis.h"
#include "rsi_ble_common_config.h"
#include "rsi_bt_common.h"
#include "rsi_bt_common_apis.h"
#include "rsi_common_apis.h"
#include "sl_net_constants.h"
//#include "sl_si91x_watchdog_timer.h"

#ifdef SLI_SI91X_MCU_INTERFACE
#include "sl_si91x_hal_soc_soft_reset.h"
#endif
#ifdef FW_LOGGING_ENABLE
//! Firmware logging includes
#include "sl_fw_logging.h"
#endif

#ifdef FW_LOGGING_ENABLE
//! Memory length of driver updated for firmware logging
#define BT_GLOBAL_BUFF_LEN (15000 + (FW_LOG_QUEUE_SIZE * MAX_FW_LOG_MSG_LEN))
//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN 15000
#endif

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

// WDT constants
#define SL_WDT_INTERRUPT_TIME 15    // WDT Interrupt Time
#define SL_WDT_SYSTEM_RESET_TIME 17 // WDT System Reset Time
#define SL_WDT_WINDOW_TIME 0        // WDT Window Time

//! global parameters list
static uint8_t chunk_data = 0;
static uint16_t chunk_number = 1;
static uint16_t mtu_size = 0;
static uint32_t ble_app_event_mask;
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_map1;
static rsi_ble_event_remote_features_t remote_dev_feature;
uint8_t firmware_header_data[64] = {0};
uint8_t firmware_chunk_fw_payload[230] = {0};
osSemaphoreId_t ble_main_task_sem;

uint8_t str_remote_address[BD_ADDR_STRING_SIZE] = {0};
uint8_t str_local_dev_address_6byte[BD_ADDR_STRING_SIZE] = {0};
uint8_t str_local_dev_address[BD_ADDR_STRING_SIZE] = {0};
uint8_t current_fw_version[20] = {0};
char current_fw_version_convert[20] = {0};
uint8_t new_fw_version[FIRMWARE_VERSION_SIZE] = {0};
uint8_t ota_service_uuid = {0};
uint8_t connection_update_param = 0;
uint8_t data_tf_start = 0, buf_config_var = 0;

uint16_t conn_int;
uint16_t total_chunks;
uint16_t last_chunk;
uint16_t ota_fw_tf_handle, ota_fw_control_handle;

uint32_t start_timer, stop_timer;

uuid_t ota_service = {0};
uuid_t ota_info_char_service = {0};
uuid_t ota_data_char_service = {0};
uuid_t ota_fw_version_char_service = {0};
uuid_t ota_bd_add_char_service = {0};
rsi_ble_event_mtu_t remote_mtu;
rsi_ble_event_conn_status_t conn_event_to_app;
rsi_ble_event_conn_update_t rsi_app_conn_update_complete;
rsi_ble_event_disconnect_t disconn_event_to_app;
rsi_ble_event_write_t app_ble_write_event;
rsi_ble_resp_add_serv_t ota_serv_response;

sl_wifi_firmware_version_t version = {0};

static const sl_wifi_device_configuration_t config = {
    .boot_option = LOAD_NWP_FW,
    .mac_address = NULL,
    .band = SL_SI91X_WIFI_BAND_2_4GHZ,
    .region_code = US,
    .boot_config = {
        .oper_mode = SL_SI91X_CLIENT_MODE,
        .coex_mode = SL_SI91X_WLAN_BLE_MODE,
#ifdef RSI_M4_INTERFACE
        .feature_bit_map = (SL_SI91X_FEAT_WPS_DISABLE | RSI_FEATURE_BIT_MAP),
#else
        .feature_bit_map = RSI_FEATURE_BIT_MAP,
#endif
#if RSI_TCP_IP_BYPASS
        .tcp_ip_feature_bit_map = RSI_TCP_IP_FEATURE_BIT_MAP,
#else
        .tcp_ip_feature_bit_map =
            (RSI_TCP_IP_FEATURE_BIT_MAP | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
#endif
        .custom_feature_bit_map = (SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID),
        .ext_custom_feature_bit_map =
            (SL_SI91X_EXT_FEAT_LOW_POWER_MODE | SL_SI91X_EXT_FEAT_XTAL_CLK |
             MEMORY_CONFIG
#ifdef SLI_SI917
             | SL_SI91X_EXT_FEAT_FRONT_END_SWITCH_PINS_ULP_GPIO_4_5_0
#endif
             | SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE),
        .bt_feature_bit_map =
            (SL_SI91X_BT_RF_TYPE | SL_SI91X_ENABLE_BLE_PROTOCOL
#if (RSI_BT_GATT_ON_CLASSIC)
             | SL_SI91X_BT_ATT_OVER_CLASSIC_ACL /* to support att over classic
                                                   acl link */
#endif
             ),
        .ext_tcp_ip_feature_bit_map = (SL_SI91X_CONFIG_FEAT_EXTENTION_VALID),
        //! ENABLE_BLE_PROTOCOL in bt_feature_bit_map
        .ble_feature_bit_map =
            ((SL_SI91X_BLE_MAX_NBR_PERIPHERALS(RSI_BLE_MAX_NBR_PERIPHERALS) |
              SL_SI91X_BLE_MAX_NBR_CENTRALS(RSI_BLE_MAX_NBR_CENTRALS) |
              SL_SI91X_BLE_MAX_NBR_ATT_SERV(RSI_BLE_MAX_NBR_ATT_SERV) |
              SL_SI91X_BLE_MAX_NBR_ATT_REC(RSI_BLE_MAX_NBR_ATT_REC)) |
             SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID |
             SL_SI91X_BLE_PWR_INX(RSI_BLE_PWR_INX) |
             SL_SI91X_BLE_PWR_SAVE_OPTIONS(RSI_BLE_PWR_SAVE_OPTIONS) |
             SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE
#if RSI_BLE_GATT_ASYNC_ENABLE
             | SL_SI91X_BLE_GATT_ASYNC_ENABLE
#endif
             ),
        .ble_ext_feature_bit_map =
            ((SL_SI91X_BLE_NUM_CONN_EVENTS(RSI_BLE_NUM_CONN_EVENTS) |
              SL_SI91X_BLE_NUM_REC_BYTES(RSI_BLE_NUM_REC_BYTES))
#if RSI_BLE_INDICATE_CONFIRMATION_FROM_HOST
             | SL_SI91X_BLE_INDICATE_CONFIRMATION_FROM_HOST // indication
                                                            // response from app
#endif
#if RSI_BLE_MTU_EXCHANGE_FROM_HOST
             | SL_SI91X_BLE_MTU_EXCHANGE_FROM_HOST // MTU Exchange request
                                                   // initiation from app
#endif
#if RSI_BLE_SET_SCAN_RESP_DATA_FROM_HOST
             | (SL_SI91X_BLE_SET_SCAN_RESP_DATA_FROM_HOST) // Set SCAN Resp Data
                                                           // from app
#endif
#if RSI_BLE_DISABLE_CODED_PHY_FROM_HOST
             | (SL_SI91X_BLE_DISABLE_CODED_PHY_FROM_HOST) // Disable Coded PHY
                                                          // from app
#endif
#if RSI_BLE_GATT_INIT
             | SL_SI91X_BLE_GATT_INIT
#endif
             ),
        .config_feature_bit_map = (SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP)}};

const osThreadAttr_t thread_attributes = {
    .name = "application_thread",
    .attr_bits = 0,
    .cb_mem = 0,
    .cb_size = 0,
    .stack_mem = 0,
    .stack_size = 4096,
    .priority = 0,
    .tz_module = 0,
    .reserved = 0,
};

sl_status_t update_firmware(void);

/*============================================================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute.
 * @param[in]  serv_handler, service handler.
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  att_val_handle, characteristic value handle
 * @param[in]  att_val_uuid, characteristic value UUID
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

/*============================================================================*/
/**
 * @fn         rsi_ble_add_char_val_att
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
    memcpy(new_att.data, data, sizeof(new_att.data));

  //! preparing the attribute value
  new_att.data_len = data_len;

  //! add attribute to the service
  rsi_ble_add_attribute(&new_att);

  return;
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
 * @fn         rsi_ble_add_ota_serv
 * @brief      this function is used to create the OTA Service
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to create the OTA service
 * UUID: 1D14D6EE-FD63-4FA1-BFA4-8F47B42119F0
 */

static void rsi_ble_add_ota_serv(void) {

  rsi_ble_resp_add_serv_t new_serv_resp = {0};
  uint8_t ota_service_uuid[UUID_SIZE] = {0x1d, 0x14, 0xd6, 0xee, 0xfd, 0x63,
                                  0x4f, 0xa1, 0xbf, 0xa4, 0x8f, 0x47,
                                  0xb4, 0x21, 0x19, 0xf0};
  rsi_ble_prepare_128bit_uuid(ota_service_uuid, &ota_service);
  ota_service.size = UUID_SIZE;
  rsi_ble_add_service(ota_service, &new_serv_resp);
  ota_serv_response.serv_handler = new_serv_resp.serv_handler;
  ota_serv_response.start_handle = new_serv_resp.start_handle;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_ota_data_char_serv
 * @brief      this function is used to create the "OTA Data" charactersitic
 * service
 * @param[in]  none.
 * @return     none.
 * @section description
 * Firmware chunks are written on this characteristic
 * UUID: 984227F3-34FC-4045-A5D0-2C581F81A153
 */
static void rsi_ble_add_ota_data_char_serv(void) {
  uint8_t ota_data_char_serv[UUID_SIZE] = {0x98, 0x42, 0x27, 0xf3, 0x34, 0xfc,
                                    0x40, 0x45, 0xa5, 0xd0, 0x2c, 0x58,
                                    0x1f, 0x81, 0xa1, 0x53};

  ota_data_char_service.size = UUID_SIZE;
  rsi_ble_prepare_128bit_uuid(ota_data_char_serv, &ota_data_char_service);

  rsi_ble_add_char_serv_att(ota_serv_response.serv_handler,
                            ota_serv_response.start_handle + 3,
                            RSI_BLE_ATT_PROPERTY_WRITE,
                            ota_serv_response.start_handle + 4,
                            ota_data_char_service);

  rsi_ble_add_char_val_att(ota_serv_response.serv_handler,
                           ota_serv_response.start_handle + 4,
                           ota_data_char_service,
                           RSI_BLE_ATT_PROPERTY_WRITE,
                           &chunk_data,
                           RSI_BLE_MAX_DATA_LEN, 0);
  ota_fw_tf_handle = ota_serv_response.start_handle + 4;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_ota_info_char_serv
 * @brief      this function is used to create the OTA Information
 * characteristic service
 * @param[in]  none.
 * @return     none.
 * @section description
 * Information about the "Total number of chunks" and "Size of Last chunk" is
 * written on this characteristic UUID:F7BF3564-FB6D-4E53-88A4-5E37E0326063
 */
static void rsi_ble_add_ota_info_char_serv(void) {

  uint8_t ota_info_char_serv[UUID_SIZE] = {0xf7, 0xbf, 0x35, 0x64, 0xfb, 0x6d,
                                    0x4e, 0x53, 0x88, 0xa4, 0x5e, 0x37,
                                    0xe0, 0x32, 0x60, 0x63};
  ota_info_char_service.size = UUID_SIZE;

  rsi_ble_prepare_128bit_uuid(ota_info_char_serv, &ota_info_char_service);
  rsi_ble_add_char_serv_att(ota_serv_response.serv_handler,
                            ota_serv_response.start_handle + 1,
                            RSI_BLE_ATT_PROPERTY_WRITE,
                            ota_serv_response.start_handle + 2,
                            ota_info_char_service);

  rsi_ble_add_char_val_att(ota_serv_response.serv_handler,
                           ota_serv_response.start_handle + 2,
                           ota_info_char_service,
                           RSI_BLE_ATT_PROPERTY_WRITE,
                           NULL, 5, 0);
  ota_fw_control_handle = ota_serv_response.start_handle + 2;
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_ota_fw_version_char_serv
 * @brief      This function is used to create Firmware version characteristic
 * @param[in]  none.
 * @return     none.
 * @section Description
 * This function is used to create Firmware version characteristic
 * UUID:4F4A2368-8CCA-451E-BFFF-CF0E2EE23E9F
 */
static void rsi_ble_add_ota_fw_version_char_serv(void) {

  uint8_t fw_version_char_serv[UUID_SIZE] = {0x4f, 0x4a, 0x23, 0x68, 0x8c, 0xca,
                                      0x45, 0x1e, 0xbf, 0xff, 0xcf, 0x0e,
                                      0x2e, 0xe2, 0x3e, 0x9f};

  ota_fw_version_char_service.size = UUID_SIZE;
  rsi_ble_prepare_128bit_uuid(fw_version_char_serv,
                              &ota_fw_version_char_service);
  rsi_ble_add_char_serv_att(ota_serv_response.serv_handler,
                            ota_serv_response.start_handle + 5,
                            RSI_BLE_ATT_PROPERTY_READ,
                            ota_serv_response.start_handle + 6,
                            ota_fw_version_char_service);
  rsi_ble_add_char_val_att(ota_serv_response.serv_handler,
                           ota_serv_response.start_handle + 6,
                           ota_fw_version_char_service,
                           RSI_BLE_ATT_PROPERTY_READ,
                           &version, 20, 0);
}

/*============================================================================*/
/**
 * @fn         rsi_ble_add_ota_bd_add_char_serv
 * @brief      This function is used to create BD address characteristic
 * @param[in]  none.
 * @return     none.
 * @section Description
 * This function is used to create BD address characteristic
 * UUID:4M4A2368-8CCA-451E-BFFF-CF0E2EE23E9F
 */
static void rsi_ble_add_ota_bd_add_char_serv(void) {

  uint8_t bd_add_char_serv[UUID_SIZE] = {0x4b, 0x4a, 0x23, 0x68, 0x8c, 0xca,
                                  0x45, 0x1e, 0xbf, 0xff, 0xcf, 0x0e,
                                  0x2e, 0xe2, 0x3e, 0x9f};

  ota_bd_add_char_service.size = UUID_SIZE;
  rsi_ble_prepare_128bit_uuid(bd_add_char_serv, &ota_bd_add_char_service);
  rsi_ble_add_char_serv_att(ota_serv_response.serv_handler,
                            ota_serv_response.start_handle + 7,
                            RSI_BLE_ATT_PROPERTY_READ,
                            ota_serv_response.start_handle + 8,
                            ota_bd_add_char_service);

  rsi_ble_add_char_val_att(ota_serv_response.serv_handler,
                           ota_serv_response.start_handle + 8,
                           ota_bd_add_char_service,
                           RSI_BLE_ATT_PROPERTY_READ,
                           str_local_dev_address, 20, 0);
}

/////////////////////////////////////

/*============================================================================*/
/**
 * @fn         rsi_ble_add_ota_fwup_serv
 * @brief      This function is used to add the OTA Service and its
 * characteristics in the module
 * @param[in]  none.
 * @return     none.
 * @section description
 * The entire firmware update process is done on this service and
 */
static void rsi_ble_add_ota_fwup_serv(void) {
  //! Adding the "OTA Service" Primary service
  //! UUID: 1D14D6EE-FD63-4FA1-BFA4-8F47B42119F0
  rsi_ble_add_ota_serv();

  //! Adding the "OTA Information" characteristic service attribute
  //! UUID: F7BF3564-FB6D-4E53-88A4-5E37E0326063
  rsi_ble_add_ota_info_char_serv();

  //! Adding the "OTA Data" characteristic service attribute
  //! UUID: 984227F3-34FC-4045-A5D0-2C581F81A153
  rsi_ble_add_ota_data_char_serv();

  //! Adding the "OTA Firmware version" characteristic service attribute
  //! UUID: 4F4A2368-8CCA-451E-BFFF-CF0E2EE23E9F
  rsi_ble_add_ota_fw_version_char_serv();

  //! Adding the "OTA BD Address" characteristic service attribute
  //! UUID: 4B4A2368-8CCA-451E-BFFF-CF0E2EE23E9F
  rsi_ble_add_ota_bd_add_char_serv();
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
static void rsi_ble_app_init_events() {
  ble_app_event_map = 0;
  ble_app_event_mask = 0xFFFFFFFF;
  ble_app_event_mask = ble_app_event_mask; // To suppress warning while
                                           // compiling
  return;
}

/*==============================================*/
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

  if (ble_main_task_sem) {
    osSemaphoreRelease(ble_main_task_sem);
  }
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
static void rsi_ble_app_clear_event(uint32_t event_num) {

  if (event_num < 32) {
    ble_app_event_map &= ~BIT(event_num);
  } else {
    ble_app_event_map1 &= ~BIT((event_num - 32));
  }

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

/*==============================================*/
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
  conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy(conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  conn_event_to_app.status = resp_enh_conn->status;
  conn_int = resp_enh_conn->conn_interval;
  rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
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
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn) {
  memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event(RSI_BLE_CONN_EVENT);
}

/*==============================================*/
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
  UNUSED_PARAMETER(reason);
  memcpy(&disconn_event_to_app, resp_disconnect,
         sizeof(rsi_ble_event_disconnect_t));
  rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
  LOG_PRINT("\r\nReason for disconnection: %x \r\n", reason);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_phy_update_complete_event
 * @brief      invoked when PHY update event is received
 * @param[in]
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_phy_update_complete_event(
    rsi_ble_event_phy_update_t *rsi_ble_event_phy_update_complete) {
  UNUSED_PARAMETER(rsi_ble_event_phy_update_complete);
  rsi_ble_app_set_event(RSI_BLE_PHY_UPDATE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_phy_update_complete_event
 * @brief      invoked when connection parameters update event is received
 * @param[in]
 * @param[in]
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_conn_update_complete_event(
    rsi_ble_event_conn_update_t *rsi_ble_event_conn_update_complete,
    uint16_t resp_status) {
  UNUSED_PARAMETER(resp_status);
  memcpy(&rsi_app_conn_update_complete, rsi_ble_event_conn_update_complete,
         sizeof(rsi_ble_event_conn_update_t));
  conn_int = (rsi_app_conn_update_complete.conn_interval);
  ++buf_config_var;
  rsi_ble_app_set_event(RSI_BLE_CONN_UPDATE_EVENT);
}

/*==============================================*/
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
  UNUSED_PARAMETER(event_id);
  memcpy(&app_ble_write_event, rsi_ble_write, sizeof(rsi_ble_event_write_t));
  rsi_ble_app_set_event(RSI_BLE_GATT_WRITE_EVENT);
}

/*==============================================*/
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
static void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *rsi_ble_event_mtu) {
  mtu_size = rsi_ble_event_mtu->mtu_size;
  memcpy(&remote_mtu, rsi_ble_event_mtu, sizeof(rsi_ble_event_mtu_t));
  rsi_ble_app_set_event(RSI_BLE_MTU_EX_EVENT);
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
  memcpy(&remote_dev_feature, rsi_ble_event_remote_features,
         sizeof(rsi_ble_event_remote_features_t));
  rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
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
  UNUSED_PARAMETER(rsi_ble_more_data_evt);
  rsi_ble_app_set_event(RSI_BLE_MORE_DATA_REQ_EVENT);
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
  UNUSED_PARAMETER(rsi_ble_data_length_update);
  rsi_ble_app_set_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
}

/*==============================================*/
/**
 * @fn         rsi_ble_ota_fwup_gatt_server
 * @brief      this is the application of heart rate profile.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This function is used to test the simple chat application.
 */
void rsi_ble_ota_fwup_gatt_server(void *argument) {
  UNUSED_PARAMETER(argument);

  sl_status_t status = SL_STATUS_FAIL;

#ifdef FW_LOGGING_ENABLE
  // Fw log component level
  sl_fw_log_level_t fw_component_log_level;
#endif

  status = sl_wifi_init(&config, NULL, sl_wifi_default_event_handler);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nWi-Fi Initialization Failed, Error Code : 0x%lX\r\n",
              status);
    return;
  } else {
    printf("\r\n Wi-Fi Initialization Successful\n");
  }

#ifdef FW_LOGGING_ENABLE
  //! Set log levels for firmware components
  sl_set_fw_component_log_levels(&fw_component_log_level);

  //! Configure firmware logging
  status = sl_fw_log_configure(FW_LOG_ENABLE,
                               FW_TSF_GRANULARITY_US,
                               &fw_component_log_level,
                               FW_LOG_BUFFER_SIZE,
                               sl_fw_log_callback);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n Firmware Logging Init Failed\r\n");
  }
  //! Create firmware logging semaphore
  rsi_semaphore_create(&fw_log_app_sem, 0);
  //! Create firmware logging task
  rsi_task_create((rsi_task_function_t)sl_fw_log_task, (uint8_t *)"fw_log_task",
                  RSI_FW_TASK_STACK_SIZE,
                  NULL,
                  RSI_FW_TASK_PRIORITY,
                  &fw_log_task_handle);
#endif

  status = update_firmware();
  printf("FW UP duration : %ld\n", ((stop_timer - start_timer) / 1000));
}

sl_status_t update_firmware() {
  sl_status_t status = SL_STATUS_FAIL;

  int32_t event_id = 0;
  uint8_t adv[31] = {2, 1, 6};

  //! Getting the Module's BD address
  status = rsi_bt_get_local_device_address(str_local_dev_address_6byte);

  if (status != RSI_SUCCESS) {
    LOG_PRINT(
        "\r\n Could not get BD Address of the module, Error Code : 0x%lX\r\n",
        status);
    return status;
  }

  else {
    rsi_6byte_dev_address_to_ascii(str_local_dev_address,
                                   str_local_dev_address_6byte);
    LOG_PRINT("\r\n BD Address of the module : %s\r\n", str_local_dev_address);
  }

  status = sl_wifi_get_firmware_version(&version);
  VERIFY_STATUS_AND_RETURN(status);
  printf("\r\nFirmware version before update:\r\n");
  print_firmware_version(&version);

  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(NULL,
                                 rsi_ble_on_connect_event,
                                 rsi_ble_on_disconnect_event,
                                 NULL,
                                 rsi_ble_on_phy_update_complete_event,
                                 NULL,
                                 rsi_ble_on_enhance_conn_status_event,
                                 NULL,
                                 rsi_ble_on_conn_update_complete_event,
                                 NULL);
  //! registering the GAP extended call back functions
  rsi_ble_gap_extended_register_callbacks(
      rsi_ble_simple_peripheral_on_remote_features_event,
      rsi_ble_more_data_req_event);
  //! registering the GATT callback functions
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

  //! Adding the ota fwup server
  rsi_ble_add_ota_fwup_serv();

  //! create ble main task if ble protocol is selected
  ble_main_task_sem = osSemaphoreNew(1, 0, NULL);

  //!  initializing the application events map
  rsi_ble_app_init_events();

  //! Setting the local name of the Module to "BLE_OTA_FWUP"
  rsi_bt_set_local_name((uint8_t *)RSI_BLE_OTA_FWUP_PROFILE);

  //! Preparing the advertising data
  adv[3] = strlen(RSI_BLE_OTA_FWUP_PROFILE) + 1;
  adv[4] = 9;
  strcpy((char *)&adv[5], RSI_BLE_OTA_FWUP_PROFILE);

  //! Setting the advertising data
  rsi_ble_set_advertise_data(adv, strlen(RSI_BLE_OTA_FWUP_PROFILE) + 5);

  //! Setting the device in advertising mode.
  status = rsi_ble_start_advertising();
  if (status != RSI_SUCCESS) {
    return status;
  } else {
    LOG_PRINT("\r\n Advertising started\r\n");
  }
  //! waiting for events from controller.
  while (1)

  { //! Application main loop

    event_id = rsi_ble_app_get_event();
    if (event_id == -1) {
      if (ble_main_task_sem) {
        osSemaphoreAcquire(ble_main_task_sem, osWaitForever);
      }
      continue;
    }

    switch (event_id) {

    case RSI_BLE_CONN_EVENT: {

      //! Converting the 6 byte address to ASCII.
      rsi_6byte_dev_address_to_ascii(str_remote_address,
                                     conn_event_to_app.dev_addr);
      LOG_PRINT("\r\n Module connected to address : %s \r\n",
                str_remote_address);

      //! MTU update request only if it was not initially set to 232.
      //  if (mtu_size < RSI_BLE_MAX_DATA_LEN) {
      status = rsi_ble_mtu_exchange_event(conn_event_to_app.dev_addr,
                                          RSI_BLE_MAX_DATA_LEN);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\n MTU request failed with status = %lx \r\n", status);
      }
      //  }

      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);

    } break; //! end of RSI_BLE_CONN_EVENT case

    case RSI_BLE_MTU_EX_EVENT: {

      //! MTU update request only if it was not initially set to 232.
      if (mtu_size > RSI_BLE_MAX_DATA_LEN) {
        status = rsi_ble_mtu_exchange_event(conn_event_to_app.dev_addr,
                                            RSI_BLE_MAX_DATA_LEN);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n MTU request failed with status = %lx \r\n", status);
        } else {
          LOG_PRINT("\r\n MTU Requested\r\n");
        }
      }

      //! Else if MTU is already equal to 232, then proceed to PHY update event
      else {
        status = rsi_ble_setphy((int8_t *)conn_event_to_app.dev_addr,
                                TX_PHY_RATE,
                                RX_PHY_RATE,
                                CODDED_PHY_RATE);
      }

      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_MTU_EX_EVENT);

    } break; //! end of RSI_BLE_MTU_EX_EVENT case

    case RSI_BLE_PHY_UPDATE_EVENT: {

      if (conn_int != CONNECTION_INTERVAL_MIN) {
        status = rsi_ble_conn_params_update(conn_event_to_app.dev_addr,
                                            CONNECTION_INTERVAL_MIN,
                                            CONNECTION_INTERVAL_MAX,
                                            CONNECTION_LATENCY,
                                            SUPERVISION_TIMEOUT);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n Connection Parameters update request failed with "
                    "status = %lx \r\n",
                    status);
        }
      }

      rsi_ble_app_clear_event(RSI_BLE_PHY_UPDATE_EVENT);

    } break; //! End of RSI_BLE_PHY_UPDATE_EVENT

    case RSI_BLE_CONN_UPDATE_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_CONN_UPDATE_EVENT);
      if (buf_config_var == 1) {
        //! Configure the buf mode for Notify and WO response commands for the
        //! remote device
        status = rsi_ble_set_wo_resp_notify_buf_info(conn_event_to_app.dev_addr,
                                                     DLE_BUFFER_MODE,
                                                     DLE_BUFFER_COUNT);
        if (status != RSI_SUCCESS) {
          break;
        } else {
          LOG_PRINT(
              "\r\nBuf configuration done for notify and set_att cmds buf "
              "mode = %d , max buff count =%d \n",
              DLE_BUFFER_MODE, DLE_BUFFER_COUNT);
        }
      }

    } break;
    // -------------------------------------------------------------------------
    // This event invokes when a device received the remote device features
    case RSI_BLE_RECEIVE_REMOTE_FEATURES: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);

      if (remote_dev_feature.remote_features[0] & 0x20) {
        status =
            rsi_ble_set_data_len(conn_event_to_app.dev_addr,
                                 TX_LEN,
                                 TX_TIME);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\n set data length cmd failed with error code = "
                    "%lx \n",
                    status);
          rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
        }
      }

    } break;
    // -------------------------------------------------------------------------
    // This event invokes when a device received the more data request event
    case RSI_BLE_MORE_DATA_REQ_EVENT: {
      //! clear the served event
      rsi_ble_app_clear_event(RSI_BLE_MORE_DATA_REQ_EVENT);

    } break;
    case RSI_APP_EVENT_DATA_LENGTH_CHANGE: {
      rsi_ble_app_clear_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
    } break;

    //! This event is invoked when the module gets diconnected before completion
    //! of firmware Update.
    case RSI_BLE_DISCONN_EVENT: {

      rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);
      LOG_PRINT("\r\n Module got Disconnected\r\n");
      //! set device in advertising mode.
    adv:
      status = rsi_ble_start_advertising();
      if (status != RSI_SUCCESS) {
        goto adv;
      } else {
        LOG_PRINT("\r\n module advertising \r\n");
      }

    } break;

    //! This event is invoked when GATT write operation on the module occurs
    case RSI_BLE_GATT_WRITE_EVENT: {

      if (app_ble_write_event.handle[0] == ota_fw_control_handle) {
        if (app_ble_write_event.att_value[0] == 0) {
          data_tf_start = 1;
          chunk_number = 1;
        } else if (app_ble_write_event.att_value[0] == 3) {
          data_tf_start = 1;
        }

      } else if (app_ble_write_event.handle[0] == ota_fw_tf_handle) {
        if (data_tf_start == 1) {
          if (chunk_number == 1) {
            memcpy(&firmware_header_data[0], &app_ble_write_event.att_value[0],
                   64);
            memcpy(&firmware_chunk_fw_payload[0],
                   &app_ble_write_event.att_value[0],
                   app_ble_write_event.length);
            status = sl_si91x_fwup_start(firmware_header_data);
            LOG_PRINT(
                "\r\n Firmware transfer in progress. Please wait...  \r\n");
            start_timer = osKernelGetTickCount();
            chunk_number++;
            status = sl_si91x_fwup_load(firmware_chunk_fw_payload,
                                        app_ble_write_event.length);
          } else {
            status = sl_si91x_fwup_load(app_ble_write_event.att_value,
                                        app_ble_write_event.length);
            //! If "status" for loading the last chunk is 3, then all chunks
            //! received and loaded successfully
            if (status == FW_UP_SUCCESS) {
              stop_timer = osKernelGetTickCount();
#if (FW_UPGRADE_TYPE == TA_FW_UP)
              LOG_PRINT("\r\n Time in sec:%ld\r\n",
                        (stop_timer - start_timer) / 1000);
              LOG_PRINT("\r\n TA Firmware transfer complete!\r\n");
              LOG_PRINT("\r\n Safe upgrade in Progress. Please wait....\r\n");

              status = sl_net_deinit(SL_NET_WIFI_CLIENT_INTERFACE);
              printf("\r\nWi-Fi Deinit status : %lx\r\n", status);
              VERIFY_STATUS_AND_RETURN(status);

              osDelay(29000);
              status =
                  sl_wifi_init(&config, NULL, sl_wifi_default_event_handler);
              if (status != SL_STATUS_OK) {
                LOG_PRINT(
                    "\r\nWi-Fi Initialization Failed, Error Code : 0x%lX\r\n",
                    status);
                return status;
              } else {
                printf("\r\n Wi-Fi Initialization Successful\n");
              }

              status = sl_wifi_get_firmware_version(&version);
              VERIFY_STATUS_AND_RETURN(status);

              printf("\r\nFirmware version after update:\r\n");
              print_firmware_version(&version);
              return SL_STATUS_OK;

#endif
#if (FW_UPGRADE_TYPE == M4_FW_UP)
              LOG_PRINT("\r\n Time in sec:%ld\r\n",
                        (stop_timer - start_timer) / 1000);
              LOG_PRINT("\r\n M4 Firmware transfer complete!\r\n");
              sl_si91x_soc_soft_reset();
#endif
              return status;
            }
          }
        }
      }

      //! Clearing the served event
      rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE_EVENT);

    } break;

    default: {
      break;
    }
    }
  }
}

void app_init(const void *unused) {
  UNUSED_PARAMETER(unused);

  osThreadNew((osThreadFunc_t)rsi_ble_ota_fwup_gatt_server, NULL,
              &thread_attributes);
}
