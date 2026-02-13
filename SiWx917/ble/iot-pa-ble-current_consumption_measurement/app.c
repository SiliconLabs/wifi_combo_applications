/*******************************************************************************
 * @file  app.c
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

/**
 * Include files
 * */

//! SL Wi-Fi SDK includes
#include "sl_board_configuration.h"
#include "sl_constants.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "cmsis_os2.h"
#include "sl_utility.h"
#include "FreeRTOSConfig.h"
//! BLE include files to refer BLE APIs
#include <string.h>

#include "ble_config.h"
#include "rsi_ble_apis.h"
#include "rsi_ble_common_config.h"
#include "rsi_bt_common.h"
#include "rsi_bt_common_apis.h"
#include "rsi_common_apis.h"
#include "sl_si91x_driver.h"
#include "app.h"
#ifdef SLI_SI91X_MCU_INTERFACE
#include "sli_si91x_power_manager.h"
#include "rsi_m4.h"
#endif
/*=======================================================================*/
//!    Application powersave configurations
/*=======================================================================*/
#if ENABLE_POWER_SAVE
sl_wifi_performance_profile_v2_t wifi_profile = { .profile = ASSOCIATED_POWER_SAVE_LOW_LATENCY };

int32_t rsi_initiate_power_save(void);
#endif


#define LOCAL_DEV_ADDR_LEN   18
/*=======================================================================*/
//   ! GLOBAL VARIABLES
/*=======================================================================*/
//! Memory to initialize driver
uint8_t wlan_radio_initialized = 0, powersave_cmd_given = 0;
uint8_t device_found                       = false;
uint8_t remote_dev_addr[18]                = { 0 };
uint8_t local_dev_addr[LOCAL_DEV_ADDR_LEN] = { 0 };
uint8_t remote_dev_bd_addr[6]              = { 0 };
static rsi_bt_resp_get_local_name_t rsi_app_resp_get_local_name = { 0 };
static uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN]      = { 0 };
static rsi_ble_event_conn_status_t rsi_app_connected_device     = { 0 };
static rsi_ble_event_disconnect_t rsi_app_disconnected_device   = { 0 };
static rsi_ble_event_conn_update_t rsi_app_conn_update_complete = { 0 };
float latest_connection_interval;
uint8_t rsi_ble_states_bitmap;
osSemaphoreId_t ble_slave_conn_sem;
osSemaphoreId_t ble_main_task_sem;
static volatile uint32_t ble_app_event_map;
static volatile uint32_t ble_app_event_map1;

static const sl_wifi_device_configuration_t
  config = { .boot_option = LOAD_NWP_FW,
             .mac_address = NULL,
             .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
             .region_code = WORLD_DOMAIN,
             .boot_config = {
               .oper_mode              = SL_SI91X_CLIENT_MODE,
               .coex_mode              = SL_SI91X_WLAN_BLE_MODE,
               .feature_bit_map        = (SL_SI91X_FEAT_WPS_DISABLE | SL_SI91X_FEAT_ULP_GPIO_BASED_HANDSHAKE
                                   | SL_SI91X_FEAT_DEV_TO_HOST_ULP_GPIO_1),
               .tcp_ip_feature_bit_map = (SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
               .custom_feature_bit_map = (SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID),
               .ext_custom_feature_bit_map =
                 (SL_SI91X_EXT_FEAT_LOW_POWER_MODE | SL_SI91X_EXT_FEAT_XTAL_CLK | MEMORY_CONFIG
                  | SL_SI91X_EXT_FEAT_FRONT_END_SWITCH_PINS_ULP_GPIO_4_5_0 | (SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE)),
               .bt_feature_bit_map         = (SL_SI91X_BT_RF_TYPE | SL_SI91X_ENABLE_BLE_PROTOCOL),
               .ext_tcp_ip_feature_bit_map = (SL_SI91X_CONFIG_FEAT_EXTENTION_VALID),
               //!ENABLE_BLE_PROTOCOL in bt_feature_bit_map
               .ble_feature_bit_map =
                 ((SL_SI91X_BLE_MAX_NBR_PERIPHERALS(RSI_BLE_MAX_NBR_PERIPHERALS)
                   | SL_SI91X_BLE_MAX_NBR_CENTRALS(RSI_BLE_MAX_NBR_CENTRALS)
                   | SL_SI91X_BLE_MAX_NBR_ATT_SERV(RSI_BLE_MAX_NBR_ATT_SERV)
                   | SL_SI91X_BLE_MAX_NBR_ATT_REC(RSI_BLE_MAX_NBR_ATT_REC))
                  | SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID | SL_SI91X_BLE_PWR_INX(RSI_BLE_PWR_INX)
                  | SL_SI91X_BLE_PWR_SAVE_OPTIONS(RSI_BLE_PWR_SAVE_OPTIONS) | SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE
#if RSI_BLE_GATT_ASYNC_ENABLE
                  | SL_SI91X_BLE_GATT_ASYNC_ENABLE
#endif
                  ),
               .ble_ext_feature_bit_map =
                 ((SL_SI91X_BLE_NUM_CONN_EVENTS(RSI_BLE_NUM_CONN_EVENTS)
                   | SL_SI91X_BLE_NUM_REC_BYTES(RSI_BLE_NUM_REC_BYTES))
#if RSI_BLE_INDICATE_CONFIRMATION_FROM_HOST
                  | SL_SI91X_BLE_INDICATE_CONFIRMATION_FROM_HOST //indication response from app
#endif
#if RSI_BLE_MTU_EXCHANGE_FROM_HOST
                  | SL_SI91X_BLE_MTU_EXCHANGE_FROM_HOST //MTU Exchange request initiation from app
#endif
#if RSI_BLE_SET_SCAN_RESP_DATA_FROM_HOST
                  | (SL_SI91X_BLE_SET_SCAN_RESP_DATA_FROM_HOST) //Set SCAN Resp Data from app
#endif
#if RSI_BLE_DISABLE_CODED_PHY_FROM_HOST
                  | (SL_SI91X_BLE_DISABLE_CODED_PHY_FROM_HOST) //Disable Coded PHY from app
#endif
#if BLE_SIMPLE_GATT
                  | SL_SI91X_BLE_GATT_INIT
#endif
                  ),
               .config_feature_bit_map = (SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP | SL_SI91X_ENABLE_ENHANCED_MAX_PSP) } };

const osThreadAttr_t thread_attributes = {
  .name       = "application_thread",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityNormal,
  .tz_module  = 0,
  .reserved   = 0,
};
/*=======================================================================*/
//   ! EXTERN VARIABLES
/*=======================================================================*/

/*=======================================================================*/
//   ! EXTERN FUNCTIONS
/*=======================================================================*/

/*========================================================================*/
//!  CALLBACK FUNCTIONS
/*=======================================================================*/

/*=======================================================================*/
//   ! PROCEDURES
/*=======================================================================*/
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
  ble_app_event_map  = 0;
  ble_app_event_map1 = 0;
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
void rsi_ble_app_set_event(uint32_t event_num)
{
  if (event_num < 32) {
    ble_app_event_map |= BIT(event_num);
  } else {
    ble_app_event_map1 |= BIT((event_num - 32));
  }
  osSemaphoreRelease(ble_main_task_sem);

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
static int32_t rsi_ble_app_get_event(void)
{
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
 * @fn         rsi_ble_on_connect_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
  memcpy(&rsi_app_connected_device, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event(RSI_APP_EVENT_CONNECTED);

  //! unblock connection semaphore
  osSemaphoreRelease(ble_slave_conn_sem);
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
void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  UNUSED_PARAMETER(reason); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(&rsi_app_disconnected_device, resp_disconnect, sizeof(rsi_ble_event_disconnect_t));
  rsi_ble_app_set_event(RSI_APP_EVENT_DISCONNECTED);
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
  rsi_app_connected_device.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy(rsi_app_connected_device.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  rsi_app_connected_device.status = resp_enh_conn->status;
  rsi_ble_app_set_event(RSI_APP_EVENT_CONNECTED);
  //! unblock connection semaphore
  if (ble_slave_conn_sem) {
    osSemaphoreRelease(ble_slave_conn_sem);
  }
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
 * This Callback function indicated the conn update complete event is received
 */
void rsi_ble_on_conn_update_complete_event(
    rsi_ble_event_conn_update_t *rsi_ble_event_conn_update_complete,
    uint16_t resp_status) {
  UNUSED_PARAMETER(resp_status);
  memcpy(&rsi_app_conn_update_complete, rsi_ble_event_conn_update_complete,
         sizeof(rsi_ble_event_conn_update_t));
  latest_connection_interval =
      rsi_ble_event_conn_update_complete->conn_interval;
  rsi_ble_app_set_event(RSI_APP_EVENT_CONN_UPDATE);
}
#if ENABLE_POWER_SAVE
/*==============================================*/
/**
 * @fn         rsi_initiate_power_save
 * @brief      send power save command to RS9116 module
 *
 * @param[out] none
 * @return     status of commands, success-> 0, failure ->-1
 * @section description
 * This function sends command to keep module in power save
 */
int32_t rsi_initiate_power_save(void)
{
  int32_t status = RSI_SUCCESS;

  LOG_PRINT("\r\n keep module in to power save \r\n");
  //! initiating power save in BLE mode
  status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n Failed to initiate power save in BLE mode \r\n");
    return status;
  }

  //! initiating power save in wlan mode
  status = sl_wifi_set_performance_profile_v2(&wifi_profile);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\n Failed to initiate power save in Wi-Fi mode :%ld\r\n", status);
    return status;
  }

  LOG_PRINT("\r\n Module is in power save \r\n");
  return status;
}
#endif

/*==============================================*/
/**
 * @fn         ble_app_task
 * @brief      Tests the BLE GAP peripheral role.
 * @param[in]  none
 * @return    none.
 * @section description
 * This function is used to test the BLE peripheral/central role and simple GAP API's.
 */

void ble_peripheral_power_save(void *argument)
{
  UNUSED_PARAMETER(argument);

  int32_t status                     = 0;
  int32_t temp_event_map             = 0;
  sl_wifi_firmware_version_t version = { 0 };
  uint8_t adv_data[37] ={0x02,0x01,0x06,0x1B,0x09,0x43,0x75,0x72,0x72,0x65,0x6E,0x74,0x5F,0x63,0x6F,0x6E,0x73,0x75,0x6D,0x70,0x74,0x69,0x6F,0x6E,0x5F,0x6E,0x75,0x6D,0x62,0x65,0x72};

//#if (defined SLI_SI91X_MCU_INTERFACE && ENABLE_POWER_SAVE)
//  sl_si91x_hardware_setup();
//#endif /* SLI_SI91X_MCU_INTERFACE */

  status = sl_wifi_init(&config, NULL, sl_wifi_default_event_handler);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\n Wi-Fi Initialization Failed, Error Code : 0x%lX\r\n", status);
    return;
  } else {
    LOG_PRINT("\r\n Wi-Fi Initialization Success\n");
  }

  //! Firmware version Prints
  status = sl_wifi_get_firmware_version(&version);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nFirmware version Failed, Error Code : 0x%lX\r\n", status);
  } else {
    print_firmware_version(&version);
  }

  //! BLE register GAP callbacks
  rsi_ble_gap_register_callbacks(NULL,
                                 rsi_ble_on_connect_event,
                                 rsi_ble_on_disconnect_event,
                                 NULL,
                                 NULL,
                                 NULL,
                                 rsi_ble_on_enhance_conn_status_event,
                                 NULL,
                                 rsi_ble_on_conn_update_complete_event,
                                 NULL);
  //! create ble main task if ble protocol is selected
  ble_main_task_sem = osSemaphoreNew(1, 0, NULL);

  //! initialize the event map
  rsi_ble_app_init_events();

  //! get the local device MAC address.
  status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n ble get local device address cmd failed with reason code : %lX \n", status);
    return;
  }
  rsi_6byte_dev_address_to_ascii(local_dev_addr, (uint8_t *)rsi_app_resp_get_dev_addr);
  LOG_PRINT("\n Get local device address: %s \n", local_dev_addr);

  //! set the local device name
  status = rsi_bt_set_local_name((uint8_t *)RSI_BLE_LOCAL_NAME);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n ble set local name cmd failed with reason code : %lX \n", status);
    return;
  }
  LOG_PRINT("\n Local name set to: %s\n", RSI_BLE_LOCAL_NAME);

  //! get the local device name
  status = rsi_bt_get_local_name(&rsi_app_resp_get_local_name);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n ble get local name cmd failed with reason code : %lX \n", status);
    return;
  }
  LOG_PRINT("\n Get local name: %s\n", rsi_app_resp_get_local_name.name);

  ble_slave_conn_sem = osSemaphoreNew(1, 0, NULL);

  //! set advertise data
  rsi_ble_set_advertise_data(adv_data, sizeof(adv_data));

  //! start the advertising
  LOG_PRINT("\r\n Minimum advertising interval:%.2fsec\n", (RSI_BLE_ADV_INT_MIN * 0.625)/1000);
  LOG_PRINT("\r\n Maximum advertising interval:%.2fsec\n", (RSI_BLE_ADV_INT_MAX * 0.625)/1000);
  if(RSI_BLE_ADV_TYPE == UNDIR_NON_CONN)
    {
      LOG_PRINT("\r\n Advertising type is advertising and unconnectable\n");
    }
  else
    {
      LOG_PRINT("\r\n Advertising type is advertising and connectable\n");
    }

  status = rsi_ble_start_advertising();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n ble start advertising failed with reason code : %lX \n", status);
    return;
  }
  else{
      LOG_PRINT("\n Start advertising \n");
  }


#if ENABLE_POWER_SAVE
  if (!powersave_cmd_given) {
    LOG_PRINT("\r\n Initiating PowerSave\r\n");
    status = rsi_initiate_power_save();
    if (status != RSI_SUCCESS) {
      LOG_PRINT("\r\n Failed to initiate power save in BLE mode \r\n");
      return;
    }
    powersave_cmd_given = 1;
  }

#endif
#ifdef SLI_SI91X_MCU_INTERFACE
#if (BLE_ROLE == ADVERTISING_ROLE)
    LOG_PRINT("\r\n M4 Power save\r\n");
    status = sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS0);
    if (status != RSI_SUCCESS) {
      LOG_PRINT("\r\n M4 power save failed with reason code : %lX \n", status);
      return;
    }
#endif
#endif
  while (1) {
    //! Application main loop
    //! checking for received events
    temp_event_map = rsi_ble_app_get_event();
    if (temp_event_map == RSI_FAILURE) {
      //! if events are not received loop will be continued.
#if ((SL_SI91X_TICKLESS_MODE == 0) && (defined SLI_SI91X_MCU_INTERFACE && ENABLE_POWER_SAVE))
      //! if events are not received loop will be continued.
      if  ((ble_app_event_map == 0) && (ble_app_event_map1 == 0)) {
        P2P_STATUS_REG &= ~M4_wakeup_TA;
        LOG_PRINT("\r\n triggering M4 sleep\r\n");
          sl_si91x_power_manager_sleep();
      }
#else
      osSemaphoreAcquire(ble_main_task_sem, osWaitForever);
#endif
      continue;
    }

    //! if any event is received, it will be served.
    switch (temp_event_map) {
      case RSI_APP_EVENT_CONNECTED: {
        //! remote device connected event
        LOG_PRINT("\n Connection is success .............\n");
        status = rsi_ble_conn_params_update((uint8_t *)rsi_app_connected_device.dev_addr,
                                            CONNECTION_INTERVAL_MIN,
                                            CONNECTION_INTERVAL_MAX,
                                                    CONNECTION_LATENCY,
                                                    SUPERVISION_TIMEOUT);
                if (status != RSI_SUCCESS) {
                  LOG_PRINT("\r\nconn params update cmd failed with status "
                      "= %lx \r\n",
                            status);
                }
                else{
                    LOG_PRINT("\r\nconn params update cmd success ");
                }
        //! clear the connected event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_CONNECTED);

      } break;
      case RSI_APP_EVENT_DISCONNECTED: {
        //! remote device disconnected event
        LOG_PRINT("\n In disconnect event............ \n ");
        //! clear the disconnected event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_DISCONNECTED);

        LOG_PRINT("\n Keep module in to active state \n");
#if ENABLE_POWER_SAVE
        LOG_PRINT("\r\n Keep module in to active state \r\n");
        //! initiating Active mode in BT mode
        status = rsi_bt_power_save_profile(RSI_ACTIVE, PSP_TYPE);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n Failed to keep Module in ACTIVE mode \r\n");
          return;
        }

        //! initiating power save in wlan mode
        wifi_profile.profile = HIGH_PERFORMANCE;
        status               = sl_wifi_set_performance_profile_v2(&wifi_profile);
        if (status != SL_STATUS_OK) {
          LOG_PRINT("\r\n Failed to keep module in HIGH_PERFORMANCE mode \r\n");
          return;
        }
#endif

          //! set device in advertising mode.
          LOG_PRINT("\n Start advertising \n");
adv:
          status = rsi_ble_start_advertising();
          if (status != RSI_SUCCESS) {
            LOG_PRINT("\r\n ble start advertising failed with reason code : %lX \n", status);
            goto adv;
          }

#if ENABLE_POWER_SAVE
        LOG_PRINT("\r\n Keep module in to power save \r\n");
        status = rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE);
        if (status != RSI_SUCCESS) {
          return;
        }

        //! initiating power save in wlan mode
        wifi_profile.profile = ASSOCIATED_POWER_SAVE;
        status               = sl_wifi_set_performance_profile_v2(&wifi_profile);
        if (status != SL_STATUS_OK) {
          LOG_PRINT("\r\n Failed to keep module in power save \r\n");
          return;
        }
        LOG_PRINT("\r\n Module is in power save \r\n");
#endif
      } break;
      case RSI_APP_EVENT_CONN_UPDATE:{
        latest_connection_interval = latest_connection_interval * 1.25;
        LOG_PRINT("\r\nConnection interval : %.2fmsec\n",latest_connection_interval);
        LOG_PRINT("\r\nLatency : %d",
                  rsi_app_conn_update_complete.conn_latency);
        LOG_PRINT("\r\nSupervision Timeout : %dmsec \n",
                  rsi_app_conn_update_complete.timeout);
        rsi_ble_app_clear_event(RSI_APP_EVENT_CONN_UPDATE);
      }break;
    }
  }
}

void app_init(void)
{
  osThreadNew((osThreadFunc_t)ble_peripheral_power_save, NULL, &thread_attributes);
}
