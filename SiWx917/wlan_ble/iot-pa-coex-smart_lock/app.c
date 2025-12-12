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
 * @brief : This file contains example application for Wlan Station BLE
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
 **/
//! SL Wi-Fi SDK includes
#include "sl_board_configuration.h"
#include "sl_net.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "cmsis_os2.h"
#include "sl_si91x_driver.h"
//BLE Specific inclusions
#include <rsi_ble_apis.h>
#include "ble_config.h"
#include "rsi_ble_common_config.h"
#include <wifi_app.h>
#include "rsi_debug.h"
#include "wifi_app.h"
#include <motor.h>
#include "ble_app.h"
#include <app.h>
#include <led.h>
#include <nvm3_flash.h>
#include "timer.h"
#if (defined(SLI_SI91X_MCU_INTERFACE) && I2C_SENSOR_PERI_ENABLE)
#include "i2c_leader_example.h"
#endif
#if DISPLAY
#include <logo_bitmaps.h>
#include <lcd_logic.h>

extern GLIB_Context_t glibContext;
#endif

// APP version
#define APP_FW_VERSION "0.4"

// TCP IP BYPASS feature check
#define RSI_TCP_IP_BYPASS RSI_DISABLE

// Function prototypes
extern void wifi_app_task(void);
extern int32_t rsi_wlan_mqtt_certs_init(void);
int I2C_Transfer(void);
void rsi_app_task(void *argument);

extern Source_operation_t SoP;
extern osTimerId_t motor_handle;
static volatile uint32_t app_event_map;
extern ap_cred_t ap_credentials;
SystemStatus_t SystemStatus = {0,false,false,false,false};

extern osTimerId_t auto_lock_handle;
osSemaphoreId_t wlan_thread_sem;
osSemaphoreId_t ble_thread_sem;
osSemaphoreId_t app_thread_sem;
osSemaphoreId_t i2c_sem;
extern osSemaphoreId_t select_sem;
extern osSemaphoreId_t data_received_semaphore;

const sl_wifi_device_configuration_t config = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .region_code = US,
  .boot_config = { .oper_mode       = SL_SI91X_CLIENT_MODE,
                   .coex_mode       = SL_SI91X_WLAN_BLE_MODE,
                   .feature_bit_map = (SL_SI91X_FEAT_ULP_GPIO_BASED_HANDSHAKE | SL_SI91X_FEAT_DEV_TO_HOST_ULP_GPIO_1
#ifdef SLI_SI91X_MCU_INTERFACE
                                       | SL_SI91X_FEAT_WPS_DISABLE
#endif
                                       ),
                   .tcp_ip_feature_bit_map = (SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT
#if (!RSI_TCP_IP_BYPASS)
                                              | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID | SL_SI91X_TCP_IP_FEAT_DNS_CLIENT
                                              | SL_SI91X_TCP_IP_FEAT_SSL
#endif
                                              ),
                   .custom_feature_bit_map = (SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID),
                   .ext_custom_feature_bit_map =
                     (SL_SI91X_EXT_FEAT_LOW_POWER_MODE | SL_SI91X_EXT_FEAT_XTAL_CLK | MEMORY_CONFIG
#ifdef SLI_SI917
                      | SL_SI91X_EXT_FEAT_FRONT_END_SWITCH_PINS_ULP_GPIO_4_5_0
#endif
                      | SL_SI91X_EXT_FEAT_BT_CUSTOM_FEAT_ENABLE),
                   .bt_feature_bit_map = (SL_SI91X_BT_RF_TYPE | SL_SI91X_ENABLE_BLE_PROTOCOL),
                   .ext_tcp_ip_feature_bit_map =
                     (SL_SI91X_CONFIG_FEAT_EXTENTION_VALID | SL_SI91X_EXT_TCP_IP_TOTAL_SELECTS(1)
#if ADAFRUIT
                      | SL_SI91X_EXT_EMB_MQTT_ENABLE
#endif
#ifdef RSI_PROCESS_MAX_RX_DATA
                      | SL_SI91X_EXT_TCP_MAX_RECV_LENGTH
#endif
                      ),
                   //!ENABLE_BLE_PROTOCOL in bt_feature_bit_map
                   .ble_feature_bit_map =
                     ((SL_SI91X_BLE_MAX_NBR_PERIPHERALS(RSI_BLE_MAX_NBR_PERIPHERALS)
                       | SL_SI91X_BLE_MAX_NBR_CENTRALS(RSI_BLE_MAX_NBR_CENTRALS)
                       | SL_SI91X_BLE_MAX_NBR_ATT_SERV(RSI_BLE_MAX_NBR_ATT_SERV)
                       | SL_SI91X_BLE_MAX_NBR_ATT_REC(RSI_BLE_MAX_NBR_ATT_REC))
                      | SL_SI91X_FEAT_BLE_CUSTOM_FEAT_EXTENTION_VALID | SL_SI91X_BLE_PWR_INX(RSI_BLE_PWR_INX)
                      | SL_SI91X_BLE_PWR_SAVE_OPTIONS(RSI_BLE_PWR_SAVE_OPTIONS)
                      | SL_SI91X_916_BLE_COMPATIBLE_FEAT_ENABLE
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
                   .config_feature_bit_map = (SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP) }
};

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

const osThreadAttr_t ble_thread_attributes = {
  .name       = "ble_thread",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 2048,
  .priority   = osPriorityNormal,
  .tz_module  = 0,
  .reserved   = 0,
};

const osThreadAttr_t app_thread_attributes = {
  .name       = "app_thread",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 2048,
  .priority   = osPriorityNormal,
  .tz_module  = 0,
  .reserved   = 0,
};

void rsi_wlan_ble_app_init(void *argument)
{
  UNUSED_PARAMETER(argument);
  int32_t status = RSI_SUCCESS;
  
  sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
  sl_gpio_driver_init();

#if DISPLAY
  memlcd_app_init();
  demoUIDisplayApp(SystemStatus);
#endif    

  select_sem              = osSemaphoreNew(1, 0, NULL);
  data_received_semaphore = osSemaphoreNew(1, 0, NULL);

  //! Wi-Fi initialization
  status = sl_net_init(SL_NET_WIFI_CLIENT_INTERFACE, &config, NULL, NULL);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nWi-Fi Initialization Failed, Error Code : 0x%lX\r\n", status);
    return;
  }

  wlan_thread_sem = osSemaphoreNew(1, 0, NULL);
  if (wlan_thread_sem == NULL) {
    LOG_PRINT("Failed to create wlan_thread_sem\r\n");
    return;
  }

  ble_thread_sem = osSemaphoreNew(1, 0, NULL);
  if (ble_thread_sem == NULL) {
    LOG_PRINT("Failed to create ble_thread_sem\r\n");
    return;
  }

  app_thread_sem = osSemaphoreNew(1, 0, NULL);
  if (app_thread_sem == NULL) {
    LOG_PRINT("Failed to create app_thread_sem\r\n");
    return;
  }

#ifdef SLI_SI91X_MCU_INTERFACE
  uint8_t xtal_enable = 1;
  status              = sl_si91x_m4_ta_secure_handshake(SL_SI91X_ENABLE_XTAL, 1, &xtal_enable, 0, NULL);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nFailed to bring m4_ta_secure_handshake: 0x%lx\r\n", status);
    return;
  }
#endif

  sl_gpio_initialization();

  create_all_timers();

  if(!check_lock_state())
  {
    LOG_PRINT("\r\nSMARTLOCK in UNLOCKED state\r\n");
    rsi_app_set_event(LOCK_CMD_RECEIVED_STATE);
  }

  if (osThreadNew((osThreadFunc_t)rsi_ble_configurator_task, NULL, &ble_thread_attributes) == NULL) {
    LOG_PRINT("Failed to create BLE thread\r\n");
  }

  if (osThreadNew((osThreadFunc_t)rsi_app_task, NULL, &app_thread_attributes) == NULL) {
    LOG_PRINT("Failed to create app thread\r\n");
  }

  status = rsi_wlan_mqtt_certs_init();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nSet Certificate Issue, Error Code : 0x%lX\r\n", status);
    return;
  }

  Ecode_t err = nvm3_initDefault();
  if(err != ECODE_OK){
      LOG_PRINT("\r\nNVM3 init status %lx \r\n", err);
  }

  // Initialise the counter objects to track writes and deletes.
  initialise_counters();

  // BLE initialization
  rsi_ble_configurator_init();
  uulp_button_init();
  wifi_app_task();

  return;
}

void app_init(void)
{
  DEBUGINIT();
  osThreadNew((osThreadFunc_t)rsi_wlan_ble_app_init, NULL, &thread_attributes);
}

/*==============================================*/
/**
 * @fn         rsi_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
int32_t rsi_app_get_event(void)
{
  uint32_t ix;

  for (ix = 0; ix < 32; ix++) {
    if (app_event_map & (1 << ix)) {
      return ix;
    }
  }

  return (-1);
}

/*==============================================*/
/**
 * @fn         rsi_app_set_event
 * @brief      sets the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
void rsi_app_set_event(uint32_t event_num)
{
  app_event_map |= BIT(event_num);

  osSemaphoreRelease(app_thread_sem);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
void rsi_app_clear_event(uint32_t event_num)
{
  app_event_map &= ~BIT(event_num);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_app_task
 * @brief      this function will execute when application events are raised.
 * @param[in]  none.
 * @return     none.
 * @section description
 */

void rsi_app_task(void *argument)
{
  UNUSED_PARAMETER(argument);
  volatile int32_t event_id;

  while(1)
    {
      // checking for events list
      event_id = rsi_app_get_event();
      if (event_id == -1) {
          osSemaphoreAcquire(app_thread_sem, osWaitForever);
        // if events are not received loop will be continued.
        continue;
      }
      switch (event_id) {
        case AUTO_LOCK_RECEIVED:{
          rsi_app_clear_event(AUTO_LOCK_RECEIVED);
          sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
//          LOG_PRINT("start auto lock timer\r\n");
          start_periodic_timer(auto_lock_handle,AUTO_LOCK_TIMEOUT_MS);
        }
          break;
        case LOCK_CMD_RECEIVED_STATE :{
          rsi_app_clear_event(LOCK_CMD_RECEIVED_STATE);
          periodic_stop_timer(auto_lock_handle);
          if(strcmp(SoP.command,"AUTO-LOCK")==0)
          {
              send_lock_state_to_ble();
              if(SystemStatus.wifi_connect){
                  wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
              }
          }
          lock();
        }break;

        case UNLOCK_CMD_RECEIVED_STATE :{
          rsi_app_clear_event(UNLOCK_CMD_RECEIVED_STATE);
          unlock();
        }break;

        case FLASH_ERASE_RECEIVED_STATE :{
          rsi_app_clear_event(FLASH_ERASE_RECEIVED_STATE);
          wifi_app_set_event(WIFI_APP_DISCONN_NOTIFY_STATE);
        }break;
        case BUTTON_INTERRUPT_RCVD:{
          rsi_app_clear_event(BUTTON_INTERRUPT_RCVD);
          memset(&SoP,0,sizeof(SoP));
          strcat(SoP.source,"BUTTON");
#if SILABS_SMARTLOCK_HARDWARE
          if(!check_lock_state())
          {
            strcat(SoP.command,"LOCK");
            rsi_app_set_event(LOCK_CMD_RECEIVED_STATE);
          }
          else
          {
            strcat(SoP.command,"UNLOCK");
            rsi_app_set_event(UNLOCK_CMD_RECEIVED_STATE);
          }

#else
          if(SystemStatus.lock_status) //state is LOCK
          {
            strcat(SoP.command,"UNLOCK");
            rsi_app_set_event(UNLOCK_CMD_RECEIVED_STATE);
          }
          else
          {
            strcat(SoP.command,"LOCK");
            rsi_app_set_event(LOCK_CMD_RECEIVED_STATE);
          }
#endif
          send_button_activity_to_ble();
        }break;

        default:
          break;
      }
      osDelay(10);
    }
}
