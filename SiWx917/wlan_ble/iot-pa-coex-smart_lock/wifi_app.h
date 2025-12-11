/*******************************************************************************
 * @file  wifi_app.h
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

#ifndef WIFI_APP_H_
#define WIFI_APP_H_

#include "wifi_config.h"
#include <sl_wifi_constants.h>
#include "sl_si91x_power_manager_wakeup_source_config.h"
#include "app.h"

#define WIFI_CLIENT_PROFILE_SSID "YOUR_INITIAL_AP_SSID"

#define DHCP_HOST_NAME    NULL
#define TIMEOUT_MS        15000
#define WIFI_SCAN_TIMEOUT 15000

#define ENABLE_POWER_SAVE           1
#define I2C_SENSOR_PERI_ENABLE      0
typedef uint32_t     TickType_t;
#define portMAX_DELAY              ( TickType_t ) 0xffffffffUL

#if AWS
#include "aws_iot_config.h"
#include "aws_iot_shadow_interface.h"
#include "aws_iot_log.h"
#include "aws_iot_error.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"

#define SUBSCRIBE_TO_TOPIC        "aws_requests"      //! Subscribe Topic to receive the message from cloud
#define PUBLISH_ON_TOPIC          "si91x_status"    //! Publish Topic to send the status from application to cloud
#define LWT_TOPIC                 "si91x_LWT"       //! LWT Topic to send the device disconnected state to other clients
#define MQTT_PUBLISH_PAYLOAD      "Heart beat from SiWx917"
#define SUBSCRIBE_QOS             QOS0              //! Quality of Service for subscribed topic "SUBSCRIBE_TO_TOPIC"
#define PUBLISH_QOS               QOS0              //! Quality of Service for publish topic "PUBLISH_ON_TOPIC"
#define PUBLISH_PERIODICITY       60000             //! Publish periodicity in milliseconds
#define MQTT_USERNAME             "username"
#define MQTT_PASSWORD             "password"
#endif

#if ADAFRUIT

#include "sl_mqtt_client.h"

#ifdef SLI_SI91X_ENABLE_IPV6
#define MQTT_BROKER_IP "2401:4901:1290:10de::1000"
#else
#define MQTT_BROKER_IP "52.7.84.197"//"io.adafruit.com"
#endif

#define MQTT_BROKER_PORT 1883

#define CLIENT_PORT 1

#define SUBSCRIBE_TOPIC        "username/feeds/lock_state"
#define QOS_OF_SUBSCRIPTION    SL_MQTT_QOS_LEVEL_0

#define PUBLISH_TOPIC          "username/feeds/si91x_status"
#define PUBLISH_MESSAGE        "Test"
#define QOS_OF_PUBLISH_MESSAGE 0

#define IS_DUPLICATE_MESSAGE 0
#define IS_MESSAGE_RETAINED  0
#define IS_CLEAN_SESSION     0

#define LAST_WILL_TOPIC       "username/feeds/si91x_LWT"
#define LAST_WILL_MESSAGE     "SmartLock has been disconnected from network"
#define QOS_OF_LAST_WILL      0
#define IS_LAST_WILL_RETAINED 0

#define ENCRYPT_CONNECTION     0
#define CERTIFICATE_INDEX      0
#define KEEP_ALIVE_INTERVAL    (WAKEUP_TIME_MS/1000)+15//2000
#define MQTT_CONNECT_TIMEOUT   5000
#define MQTT_KEEPALIVE_RETRIES 0

#define SEND_CREDENTIALS 1

#define USERNAME "username"
#define PASSWORD "password"
#endif

typedef enum {
  SSID_NVM_KEY              = 1, ///< key to store ssid in nvm
  PWD_NVM_KEY               = 2, ///< key to store password in nvm
  SEC_TYPE_NVM_KEY          = 3, ///< key to store Security type in nvm
  SAVE_CREDENTIALS_NVM_KEY  = 4, ///< Key to detect if credentials are stored in flash
} ap_cred_nvm_keys_t;

typedef struct {
  uint8_t ssid[64];     ///< SSID value
  uint8_t pwd[66];      ///< Password value
  sl_wifi_security_t sec_type;  ///< Security type
} ap_cred_t;

void wifi_app_set_event(uint32_t event_num);
void put_system_in_sleep(int32_t wakeup_time);
sl_status_t send_dev_active_status();
void uulp_button_init(void);
void my_gpio_uulp_pin_interrupt_callback(uint32_t pin_intr);
sl_status_t mqtt_publish(void);
#if ADAFRUIT
void mqtt_client_message_handler(void *client, sl_mqtt_client_message_t *message, void *context);
void mqtt_client_event_handler(void *client, sl_mqtt_client_event_t event, void *event_data, void *context);
void mqtt_client_error_event_handler(void *client, sl_mqtt_client_error_status_t *error);
void mqtt_client_cleanup();
void print_char_buffer(char *buffer, uint32_t buffer_length);
sl_status_t mqtt_publish();
sl_status_t mqtt_client_init();
#endif
#endif /* WIFI_APP_H_ */
