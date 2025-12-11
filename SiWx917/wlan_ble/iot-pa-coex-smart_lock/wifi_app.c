/*******************************************************************************
 * @file  wifi_app.c
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
 * */

//! SL Wi-Fi SDK includes
#include "sl_constants.h"
#include "sl_wifi.h"
#include "sl_wifi_callback_framework.h"
#include "sl_net.h"
#include "sl_net_si91x.h"
#include "sl_utility.h"
#include "sl_status.h"
#include "sl_wifi_device.h"
#include "sl_net_wifi_types.h"
#include "sl_si91x_driver.h"
#include "sl_board_configuration.h"
#include "errno.h"
#include "socket.h"
#include "sl_si91x_socket.h"
#include "sl_wifi_device.h"
#include "sl_event_handler.h"
#include "sl_si91x_driver.h"
#include "sl_si91x_gpio.h"
#include "ble_config.h"
#include "wifi_config.h"

#ifdef SLI_SI91X_MCU_INTERFACE
#include "sl_si91x_power_manager.h"
#include "rsi_rom_power_save.h"
#include "sl_si91x_driver_gpio.h"
//! I2C related files
#include "i2c_leader_example.h"
#include "rsi_ps_config.h"
#endif

#include "cmsis_os2.h"
#include <string.h>
#include <stdio.h>

#include "wifi_config.h"
#include "rsi_common_apis.h"
#include "rsi_bt_common_apis.h"


#include <rsi_ble_apis.h>

#include "rsi_debug.h"
#include "timer.h"
#include <wifi_app.h>
#include <nvm3_flash.h>
#include <motor.h>
#include <ble_app.h>
#include "app.h"
#include <led.h>

#include "FreeRTOSConfig.h"

#if (defined(SLI_SI91X_MCU_INTERFACE) && (SL_SI91X_TICKLESS_MODE == 0))
#include "sl_si91x_m4_ps.h"
#endif

extern const sl_wifi_device_configuration_t config;
/******************************************************
 *                    Constants
 ******************************************************/
#if AWS
#include "aws_client_certificate.pem.crt.h"
#include "aws_client_private_key.pem.key.h"
#include "aws_starfield_ca.pem.h"

#elif ADAFRUIT
#include "cacert.pem.h"
#endif
/*
 *********************************************************************************************************
 *                                         LOCAL GLOBAL VARIABLES
 *********************************************************************************************************
 */

sl_wifi_scan_result_t *scan_result          = NULL;
static volatile bool scan_complete          = false;
static volatile sl_status_t callback_status = SL_STATUS_OK;
uint16_t scanbuf_size = (sizeof(sl_wifi_scan_result_t) + (SL_WIFI_MAX_SCANNED_AP * sizeof(scan_result->scan_info[0])));

sl_wifi_performance_profile_v2_t wifi_profile = { .profile = ASSOCIATED_POWER_SAVE_LOW_LATENCY };
osSemaphoreId_t data_received_semaphore;
char temp_string[100]={0};

volatile uint8_t connected = 0, timeout = 0;
uint8_t disconnected = 0, disassosiated = 0;
uint8_t a = 0;
sl_net_ip_configuration_t ip_address        = { 0 };

static volatile uint32_t wlan_app_event_map;
#if !(defined(SLI_SI91X_MCU_INTERFACE) && ENABLE_POWER_SAVE)
volatile uint8_t publish_msg = 0;
#endif

uint8_t retry = 1;
extern uint8_t magic_word;
uint8_t  periodic_timer_counter = 0;
Source_operation_t SoP;
ap_cred_t ap_credentials;
uint32_t wakeup_time_ms = WAKEUP_TIME_MS;
bool mqtt_disconnected = false;
bool is_i2c_enabled = I2C_SENSOR_PERI_ENABLE;
bool volatile on_mqtt_connect = false;
extern bool new_cred;
bool erase_from_mqtt = false;
bool ble_from_mqtt = false;
bool device_in_sleep = false;
char client_id[25]={0};
uint8_t yield;
uint8_t disconnect_flag = 0;
extern osTimerId_t retry_handle;
extern osTimerId_t motor_handle;
extern osTimerId_t auto_lock_handle;
fd_set read_fds;
volatile uint8_t check_for_recv_data = 0;
extern osSemaphoreId_t select_sem;
volatile uint16_t count = 0;
extern volatile uint8_t pub_state, qos1_publish_handle, select_given;

int32_t status1            = RSI_SUCCESS;
#define RSI_FD_ISSET(x, y) rsi_fd_isset(x, y)

#define SL_SI91X_UULP_GPIO_2_PORT RTE_UULP_GPIO_2_PORT
#define SL_SI91X_UULP_GPIO_2_PIN  RTE_UULP_GPIO_2_PIN
#define SL_SI91X_UULP_GPIO_2_MODE RTE_UULP_GPIO_2_MODE

static sl_si91x_gpio_pin_config_t uulp_gpio_pin_config = { { SL_SI91X_UULP_GPIO_2_PORT, SL_SI91X_UULP_GPIO_2_PIN },
                                                          GPIO_INPUT };

#define RISE_EDGE_INTERRUPT 4U ///< Interrupt on rising edge of the button press
#define FALL_EDGE_INTERRUPT 8U ///< Interrupt on falling edge of the button press
#define RISE_EDGE_AND_FALL_EDGE_INTERRUPT 12U ///< Interrupt on rising edge and falling edge of the button press

#define AVL_INTR_NO      0    // available interrupt number

extern SystemStatus_t SystemStatus;

// Application level WLAN reconnection attempt counter and retry limit
#define APP_RECONN_LOOP_CTR_LIM 3
uint8_t app_reconn_loop_ctr = 0;
typedef struct wlan_app_cb_s {
  //! wifi application state
  volatile wifi_app_state_t state;

  //! length of buffer to copy
  uint32_t length;

  //! application buffer
  uint8_t buffer[RSI_APP_BUF_SIZE];

  //! to check application buffer availability
  uint8_t buf_in_use;

  //! application events bit map
  uint32_t event_map;

} sl_wlan_app_cb_t;

static sl_net_wifi_client_profile_t wifi_client_profile = {
    .config = {
        .ssid.value = WIFI_CLIENT_PROFILE_SSID,
        .ssid.length = sizeof(WIFI_CLIENT_PROFILE_SSID)-1,
        .channel.channel = SL_WIFI_AUTO_CHANNEL,
        .channel.band = SL_WIFI_AUTO_BAND,
        .channel.bandwidth = SL_WIFI_AUTO_BANDWIDTH,
        .bssid = {{0}},
        .bss_type = SL_WIFI_BSS_TYPE_INFRASTRUCTURE,
        .security = SL_WIFI_WPA2,
        .encryption = SL_WIFI_CCMP_ENCRYPTION,
        .client_options = 0,
        .credential_id = SL_NET_DEFAULT_WIFI_CLIENT_CREDENTIAL_ID,
    },
    .ip = {
        .mode = SL_IP_MANAGEMENT_DHCP,
        .type = SL_IPV4,
        .host_name = DHCP_HOST_NAME,
        .ip = {{{0}}},

    }
};
#if AWS
IoT_Error_t rc = FAILURE;
AWS_IoT_Client client   = { 0 };
AWS_IoT_Client mqtt_client = { 0 };
IoT_Publish_Message_Params publish_iot_msg = { 0 };
#elif ADAFRUIT
sl_mqtt_client_t client = { 0 };

uint8_t is_execution_completed = 0;

sl_mqtt_client_credentials_t *client_credentails = NULL;

sl_mqtt_client_configuration_t mqtt_client_configuration = { .is_clean_session = IS_CLEAN_SESSION,
#if ENCRYPT_CONNECTION
                                                             .tls_flags = SL_MQTT_TLS_ENABLE | SL_MQTT_TLS_TLSV_1_2
                                                                          | SL_MQTT_TLS_CERT_INDEX_1,
#endif
                                                             .client_port = CLIENT_PORT };

sl_mqtt_broker_t mqtt_broker_configuration = {
  .port                    = MQTT_BROKER_PORT,
  .is_connection_encrypted = ENCRYPT_CONNECTION,
  .connect_timeout         = MQTT_CONNECT_TIMEOUT,
  .keep_alive_interval     = KEEP_ALIVE_INTERVAL,
  .keep_alive_retries      = MQTT_KEEPALIVE_RETRIES,
};

sl_mqtt_client_message_t message_to_be_published = {
  .qos_level            = QOS_OF_PUBLISH_MESSAGE,
  .is_retained          = IS_MESSAGE_RETAINED,
  .is_duplicate_message = IS_DUPLICATE_MESSAGE,
  .topic                = (uint8_t *)PUBLISH_TOPIC,
  .topic_length         = strlen(PUBLISH_TOPIC),
};

sl_mqtt_client_last_will_message_t last_will_message = {
  .is_retained         = IS_LAST_WILL_RETAINED,
  .will_qos_level      = QOS_OF_LAST_WILL,
  .will_topic          = (uint8_t *)LAST_WILL_TOPIC,
  .will_topic_length   = strlen(LAST_WILL_TOPIC),
  .will_message        = (uint8_t *)LAST_WILL_MESSAGE,
  .will_message_length = strlen(LAST_WILL_MESSAGE),
};
#endif
volatile bool timer_stop_received = false;
volatile bool mqtt_event_received = false;
/*
 *********************************************************************************************************
 *                                               DATA TYPES
 *********************************************************************************************************
 */
extern void wifi_app_send_to_ble(uint16_t msg_type, uint8_t *data, uint16_t data_len);
bool retry_timeout = false;
static sl_status_t show_scan_results();
uint8_t conn_status;
volatile bool send_lock_state = false;

// WLAN include file for configuration
extern osSemaphoreId_t wlan_thread_sem;
sl_ipv4_address_t *fetch_ip;
char ip_add[32];
void gpio_uulp_pin_interrupt_callback(uint32_t pin_intr)
{
  sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);

  (void)(pin_intr);
  //NPSS GPIO-2 interrupt clr
  (*(volatile uint32_t *)(0x12080000UL + 0x08)) = 0x08; //NPSS GPIO-2 interrupt clr
  while (sl_si91x_gpio_get_uulp_npss_pin(2) == 0)
    ; // waiting for the button release
  send_lock_state = true;
  rsi_app_set_event(BUTTON_INTERRUPT_RCVD);
  osSemaphoreRelease(data_received_semaphore);
}
/*==============================================*/
/**
 * @fn         wifi_app_set_event
 * @brief      sets the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
void wifi_app_set_event(uint32_t event_num)
{
  wlan_app_event_map |= BIT(event_num);

  osSemaphoreRelease(wlan_thread_sem);
  return;
}

/*==============================================*/
/**
 * @fn         wifi_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
void wifi_app_clear_event(uint32_t event_num)
{
  if(event_num == WIFI_CLEAR_ALL_STATES)
  {
      wlan_app_event_map &= 0;
  }
  else
  {
      wlan_app_event_map &= ~BIT(event_num);
  }
  return;
}

/*==============================================*/
/**
 * @fn         wifi_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
int32_t wifi_app_get_event(void)
{
  uint32_t ix;

  for (ix = 0; ix < 32; ix++) {
    if (wlan_app_event_map & (1 << ix)) {
      return ix;
    }
  }

  return (-1);
}

// rejoin failure call back handler in station mode
sl_status_t join_callback_handler(sl_wifi_event_t event, char *result, uint32_t result_length, void *arg)
{
  UNUSED_PARAMETER(event);
  UNUSED_PARAMETER(result);
  UNUSED_PARAMETER(result_length);
  UNUSED_PARAMETER(arg);

  LOG_PRINT("\r\n Join failed, in join callback handler \r\n");

  // update wlan application state
  disconnected = 1;
  connected    = 0;

  /* in case of AP turned off during publish with QoS1 application was waiting for select_sem semaphore*/
  if (pub_state == 1) {
      LOG_PRINT("\r\n before sem giving");
        osSemaphoreRelease(select_sem);
      }
#if AWS
  rc = aws_iot_mqtt_disconnect(&mqtt_client);
  if (SUCCESS != rc) {
    LOG_PRINT("MQTT Disconnection with error %d:\r\n",rc);
  } else {
    LOG_PRINT("MQTT Disconnection Successful\r\n");
  }
#endif
  mqtt_disconnected = true;

  wifi_app_set_event(WIFI_APP_ERROR_STATE);

  osSemaphoreRelease(data_received_semaphore);

  return SL_STATUS_OK;
}

void remote_terminate(int socket_id, uint16_t port_number, uint32_t bytes_sent)
{
  LOG_PRINT("Remote Terminate on socket %d, port %d , bytes_sent %ld\r\n", socket_id, port_number, bytes_sent);

  pub_state = 0;
#if AWS
  rc = aws_iot_mqtt_disconnect(&mqtt_client);
  if (SUCCESS != rc) {
    LOG_PRINT("MQTT Disconnection with error %d:\r\n",rc);
  } else {
    LOG_PRINT("MQTT Disconnection Successful\r\n");
  }
#endif
  mqtt_disconnected = true;
  wifi_app_set_event(WIFI_APP_ERROR_STATE);
  osSemaphoreRelease(data_received_semaphore);
}

void rsi_wlan_app_call_backs_init(void)
{
  //! Initialize join fail call back
  sl_wifi_set_join_callback(join_callback_handler, NULL);
  sl_si91x_set_remote_termination_callback(remote_terminate);
}

#if AWS
void async_socket_select(fd_set *fd_read, fd_set *fd_write, fd_set *fd_except, int32_t status1)
{
  UNUSED_PARAMETER(fd_except);
  UNUSED_PARAMETER(fd_write);
  UNUSED_PARAMETER(status1);

  //!Check the data pending on this particular socket descriptor
  if (FD_ISSET(mqtt_client.networkStack.socket_id, fd_read)) {
    if (pub_state != 1) { //This check is for handling PUBACK in QOS1
        osSemaphoreRelease(data_received_semaphore);
        check_for_recv_data = 1;
#if ENABLE_POWER_SAVE
      wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
#else
      wifi_app_set_event(WIFI_APP_MQTT_RECEIVE_STATE);
#endif
    } else if (pub_state == 1) { //This check is for handling PUBACK in QOS1
      osSemaphoreRelease(select_sem);
    }
  }
}

/**
 * @brief This parameter will avoid infinite loop of publish and exit the program after certain number of publishes
 */
static void iot_subscribe_callback_handler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen, IoT_Publish_Message_Params *params, void *pData)
{
  UNUSED_PARAMETER(pClient);
  UNUSED_PARAMETER(topicName);
  UNUSED_PARAMETER(topicNameLen);
  UNUSED_PARAMETER(pData);

  char dt[1000] = { 0 };
  uint32_t len;
  len = params->payloadLen;

  strncpy(dt, params->payload, len);
  LOG_PRINT("\r\nData received from mqtt = %s\r\n", dt);
#if !(defined(SLI_SI91X_MCU_INTERFACE) && ENABLE_POWER_SAVE)
    publish_msg = 1;
#endif

  memset(&SoP,0,sizeof(SoP));

  if (!strcmp (dt, "lock"))
  {

    if(!is_timer_running(motor_handle))
    {
      strcat(SoP.source,"MQTT");
      strcat(SoP.command,"LOCK");
      
      send_lock_state_to_ble();

      rsi_app_set_event(LOCK_CMD_RECEIVED_STATE);
      osDelay(10);
    }
  }
  else if(!strcmp (dt, "unlock"))
  {
    strcat(SoP.source,"MQTT");
    strcat(SoP.command,"UNLOCK");

    send_unlock_state_to_ble();

    rsi_app_set_event(UNLOCK_CMD_RECEIVED_STATE);
    osDelay(10);
  }
  else if(!strcmp (dt, "query"))
  {
    LOG_PRINT("Received Query\n ");
     if(SystemStatus.lock_status)
       {
         strcat(SoP.source,"MQTT");
         strcat(SoP.command,"LOCK");
       }
     else
       {
         strcat(SoP.source,"MQTT");
         strcat(SoP.command,"UNLOCK");
       }
     wifi_app_set_event(WIFI_APP_MQTT_PUBLISH_STATE);
  }
  else if(!strcmp (dt, "erase"))
  {
    rc = aws_iot_mqtt_disconnect(&mqtt_client);
    if (SUCCESS != rc) {
      LOG_PRINT("MQTT Disconnection with error %d:\r\n",rc);
    } else {
      LOG_PRINT("MQTT Disconnection Successful\r\n");
    }
    rsi_app_set_event(FLASH_ERASE_RECEIVED_STATE);
    erase_from_mqtt = true;
  }
  wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
}

static void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data)
{
  IoT_Error_t rc = FAILURE;
  LOG_PRINT("\r\nMQTT disconnected abruptly and pClient state is: %d\r\n", pClient->clientStatus.clientState);
  if (NULL == pClient) {
    return;
  }
  IOT_UNUSED(data);
  if (aws_iot_is_autoreconnect_enabled(pClient)) {
    LOG_PRINT("Auto Reconnect is enabled, Reconnecting attempt will start now\r\n");
  } else {
    LOG_PRINT("Auto Reconnect not enabled. Starting manual reconnect...\r\n");
    rc = aws_iot_mqtt_attempt_reconnect(pClient);
    if (NETWORK_RECONNECTED == rc) {
      LOG_PRINT("Manual Reconnect Successful\r\n");
    } else {
      LOG_PRINT("Manual Reconnect \r\n");
    }
  }
}

sl_status_t load_certificates_in_flash(void)
{
  sl_status_t status;

  // Load SSL CA certificate
  status = sl_net_set_credential(SL_NET_TLS_SERVER_CREDENTIAL_ID(0),
                                 SL_NET_SIGNING_CERTIFICATE,
                                 aws_starfield_ca,
                                 sizeof(aws_starfield_ca) - 1);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nLoading TLS CA certificate in to FLASH Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }

  // Load SSL Client certificate
  status = sl_net_set_credential(SL_NET_TLS_CLIENT_CREDENTIAL_ID(0),
                                 SL_NET_CERTIFICATE,
                                 aws_client_certificate,
                                 sizeof(aws_client_certificate) - 1);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nLoading TLS Client certificate in to FLASH Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }

  // Load SSL Client private key
  status = sl_net_set_credential(SL_NET_TLS_CLIENT_CREDENTIAL_ID(0),
                                 SL_NET_PRIVATE_KEY,
                                 aws_client_private_key,
                                 sizeof(aws_client_private_key) - 1);
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nLoading TLS Client private key in to FLASH Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }

  return SL_STATUS_OK;
}
#endif

int32_t rsi_wlan_mqtt_certs_init(void)
{
  sl_status_t status = RSI_SUCCESS;
#if AWS
  status = load_certificates_in_flash();
  if (status != SL_STATUS_OK) {
    LOG_PRINT("\r\nUnexpected error while loading certificates: 0x%lx\r\n", status);
    return status;
  }
#endif
  rsi_wlan_app_call_backs_init();

  return status;
}

static sl_status_t show_scan_results()
{
  SL_WIFI_ARGS_CHECK_NULL_POINTER(scan_result);
  uint8_t *bssid = NULL;
  LOG_PRINT("%lu Scan results:\r\n", scan_result->scan_count);

  if (scan_result->scan_count) {
    LOG_PRINT("\r\n   %s %24s %s", "SSID", "SECURITY", "NETWORK");
    LOG_PRINT("%12s %12s %s\r\n", "BSSID", "CHANNEL", "RSSI");

    for (int a = 0; a < (int)scan_result->scan_count; ++a) {
      bssid = (uint8_t *)&scan_result->scan_info[a].bssid;
      LOG_PRINT("%-24s %4u,  %4u, ",
                scan_result->scan_info[a].ssid,
                scan_result->scan_info[a].security_mode,
                scan_result->scan_info[a].network_type);
      LOG_PRINT("  %02x:%02x:%02x:%02x:%02x:%02x, %4u,  -%u\r\n",
                bssid[0],
                bssid[1],
                bssid[2],
                bssid[3],
                bssid[4],
                bssid[5],
                scan_result->scan_info[a].rf_channel,
                scan_result->scan_info[a].rssi_val);
    }
  }

  return SL_STATUS_OK;
}

sl_status_t wlan_app_scan_callback_handler(sl_wifi_event_t event,
                                           sl_wifi_scan_result_t *result,
                                           uint32_t result_length,
                                           void *arg)
{
  UNUSED_PARAMETER(result_length);
  UNUSED_PARAMETER(arg);

  scan_complete = true;

  if (SL_WIFI_CHECK_IF_EVENT_FAILED(event)) {
    callback_status = *(sl_status_t *)result;
    return SL_STATUS_FAIL;
  }
  SL_VERIFY_POINTER_OR_RETURN(scan_result, SL_STATUS_NULL_POINTER);
  memset(scan_result, 0, scanbuf_size);
  memcpy(scan_result, result, scanbuf_size);

  callback_status = show_scan_results();

  return SL_STATUS_OK;
}
void wifi_app_task(void)
{
  int32_t status          = RSI_SUCCESS;
  int32_t event_id        = 0;

#if !(defined(SLI_SI91X_MCU_INTERFACE) && ENABLE_POWER_SAVE)
  uint32_t start_time         = 0;
  uint8_t publish_timer_start = 0;
#endif

  sl_mac_address_t mac_addr = { 0 };
  char mac_id[18];
  sl_wifi_get_mac_address(SL_WIFI_CLIENT_INTERFACE, &mac_addr);
  sprintf(mac_id,
          "%x:%x:%x:%x:%x:%x",
          mac_addr.octet[0],
          mac_addr.octet[1],
          mac_addr.octet[2],
          mac_addr.octet[3],
          mac_addr.octet[4],
          mac_addr.octet[5]);
  LOG_PRINT("\r\nMAC ID: %s \r\n", mac_id);
  sprintf(client_id, "silabs_%s", mac_id);

#if AWS
    IoT_Client_Init_Params mqttInitParams   = iotClientInitParamsDefault;
    IoT_Client_Connect_Params connectParams = iotClientConnectParamsDefault;

    mqttInitParams.enableAutoReconnect       = false;
    mqttInitParams.pHostURL                  = AWS_IOT_MQTT_HOST;
    mqttInitParams.port                      = AWS_IOT_MQTT_PORT;
    mqttInitParams.pRootCALocation           = (char *)aws_starfield_ca;
    mqttInitParams.pDeviceCertLocation       = (char *)aws_client_certificate;
    mqttInitParams.pDevicePrivateKeyLocation = (char *)aws_client_private_key;
    mqttInitParams.mqttCommandTimeout_ms     = 20000;
    mqttInitParams.tlsHandshakeTimeout_ms    = 5000;
    mqttInitParams.isSSLHostnameVerify       = true;
    mqttInitParams.disconnectHandler         = disconnectCallbackHandler;
    mqttInitParams.disconnectHandlerData     = NULL;

    connectParams.keepAliveIntervalInSec = KEEP_ALIVE_TIME_SEC;
    connectParams.isCleanSession         = true;
    connectParams.MQTTVersion            = MQTT_3_1_1;
    connectParams.pClientID              = client_id;
    connectParams.clientIDLen            = (uint16_t)strlen(client_id);
    connectParams.isWillMsgPresent       = true;

    connectParams.pUsername   = MQTT_USERNAME;
    connectParams.usernameLen = strlen(MQTT_USERNAME);
    connectParams.pPassword   = MQTT_PASSWORD;
    connectParams.passwordLen = strlen(MQTT_PASSWORD);

    char will_msg[100] ={0};
    sprintf(will_msg,"Device with client id %s disconnected",client_id);

    connectParams.will.pTopicName = LWT_TOPIC;
    connectParams.will.topicNameLen = strlen(LWT_TOPIC);
    connectParams.will.pMessage = will_msg;
    connectParams.will.msgLen = strlen(will_msg);

#endif

#if (defined(SLI_SI91X_MCU_INTERFACE) && I2C_SENSOR_PERI_ENABLE)
    // Initialize I2C
    sl_i2c_init_instances();
    si70xx_example_init();
    si70xx_example_process_action();
    sl_i2c_driver_deinit(I2C);
#endif

  // Allocate memory for scan buffer
  scan_result = (sl_wifi_scan_result_t *)malloc(scanbuf_size);
  if (scan_result == NULL) {
    LOG_PRINT("Failed to allocate memory for scan result\r\n");
    return;
  }
  memset(scan_result, 0, scanbuf_size);
  while (1) {

    // checking for events list
    event_id = wifi_app_get_event();
    if (event_id == -1) {

        osSemaphoreAcquire(wlan_thread_sem, osWaitForever);

      // if events are not received loop will be continued.
      continue;
    }
    switch (event_id) {
      case WIFI_APP_INITIAL_STATE: {
        wifi_app_clear_event(WIFI_APP_INITIAL_STATE);

        // update wlan application state
        if (magic_word) {
          // clear the served event
          wifi_app_set_event(WIFI_APP_JOIN_STATE);
        } else {
          wifi_app_set_event(WIFI_APP_SCAN_STATE);
        }
      } break;

      case WIFI_APP_SCAN_STATE: {
        wifi_app_clear_event(WIFI_APP_SCAN_STATE);

        sl_wifi_scan_configuration_t wifi_scan_configuration = { 0 };
        wifi_scan_configuration                              = default_wifi_scan_configuration;

        sl_wifi_set_scan_callback(wlan_app_scan_callback_handler, NULL);

        status = sl_wifi_start_scan(SL_WIFI_CLIENT_2_4GHZ_INTERFACE, NULL, &wifi_scan_configuration);
        if (SL_STATUS_IN_PROGRESS == status) {
          const uint32_t start = osKernelGetTickCount();

          while (!scan_complete && (osKernelGetTickCount() - start) <= WIFI_SCAN_TIMEOUT) {
            osThreadYield();
          }
          status = scan_complete ? callback_status : SL_STATUS_TIMEOUT;
        }
        if (status != SL_STATUS_OK) {
          LOG_PRINT("\r\nWLAN Scan Wait Failed, Error Code : 0x%lX\r\n", status);
          wifi_app_set_event(WIFI_APP_SCAN_STATE);
          osDelay(1000);
        } else {
          // update wlan application state
          wifi_app_send_to_ble(WIFI_APP_SCAN_RESP, (uint8_t *)scan_result, scanbuf_size);
        }
      } break;

      case WIFI_APP_JOIN_STATE: {
        wifi_app_clear_event(WIFI_APP_JOIN_STATE);
        if(erase_from_mqtt){
            break;
        }
        wifi_client_profile.config.ssid.length = strlen((char *)ap_credentials.ssid);
        memcpy(wifi_client_profile.config.ssid.value, ap_credentials.ssid, wifi_client_profile.config.ssid.length);
        wifi_client_profile.config.security = ap_credentials.sec_type;
        wifi_client_profile.config.encryption    = SL_WIFI_DEFAULT_ENCRYPTION;

        if (wifi_client_profile.config.security != SL_WIFI_OPEN) {
          status = sl_net_set_credential(wifi_client_profile.config.credential_id, SL_NET_WIFI_PSK, ap_credentials.pwd, strlen((char *)ap_credentials.pwd));
          if (status != SL_STATUS_OK) {
            LOG_PRINT("Credentials set failed, status : %lu\r\n", status);
            continue;
          }
        }

        status =
          sl_net_set_profile(SL_NET_WIFI_CLIENT_INTERFACE, SL_NET_DEFAULT_WIFI_CLIENT_PROFILE_ID, &wifi_client_profile);
        if (SL_STATUS_OK == status) {
          wifi_client_profile.config.ssid.length = strlen((char *)ap_credentials.ssid);
          memcpy(wifi_client_profile.config.ssid.value, ap_credentials.ssid, wifi_client_profile.config.ssid.length);
          wifi_client_profile.config.security = ap_credentials.sec_type;

          // Enabling required MFP bits in join feature bitmap for WPA3 Personal mode security type
          if (wifi_client_profile.config.security == 7) {
            status = sl_si91x_set_join_configuration(
              SL_WIFI_CLIENT_2_4GHZ_INTERFACE,
              SL_SI91X_JOIN_FEAT_MFP_CAPABLE_REQUIRED | SL_SI91X_JOIN_FEAT_LISTEN_INTERVAL_VALID);
            if (status != SL_STATUS_OK) {
              LOG_PRINT("\r\n join configuration settings for WPA3 failed\r\n");
            }
          } else if (wifi_client_profile.config.security == 8) {
            status = sl_si91x_set_join_configuration(
              SL_WIFI_CLIENT_2_4GHZ_INTERFACE,
              SL_SI91X_JOIN_FEAT_MFP_CAPABLE_ONLY | SL_SI91X_JOIN_FEAT_LISTEN_INTERVAL_VALID);
            if (status != SL_STATUS_OK) {
              LOG_PRINT("\r\n join configuration settings for WPA3 failed\r\n");
            }
          } else {
            status = sl_si91x_set_join_configuration(SL_WIFI_CLIENT_2_4GHZ_INTERFACE,
                                                     SL_SI91X_JOIN_FEAT_LISTEN_INTERVAL_VALID);
            if (status != SL_STATUS_OK) {
              LOG_PRINT("\r\n join configuration settings for WPA3 failed\r\n");
            }
          }
          static sl_wifi_advanced_client_configuration_t advanced_client_configuration = { 0 };
          advanced_client_configuration.max_retry_attempts = 5;
          advanced_client_configuration.first_time_retry_enable = true;
          advanced_client_configuration.beacon_missed_count = 40;
          sl_status_t st_code = sl_wifi_set_advanced_client_configuration(SL_WIFI_CLIENT_2_4GHZ_INTERFACE,&advanced_client_configuration);
          if(st_code != 0){
              LOG_PRINT("\r\nset_advanced_client_configuration return value : 0x%lX\r\n", st_code);
          }

          do {
              status = sl_net_up(SL_NET_WIFI_CLIENT_INTERFACE, SL_NET_DEFAULT_WIFI_CLIENT_PROFILE_ID);
            app_reconn_loop_ctr++;
          } while ((status != SL_STATUS_OK) && (app_reconn_loop_ctr < APP_RECONN_LOOP_CTR_LIM));
          app_reconn_loop_ctr = 0;
        }
        if (status != RSI_SUCCESS) {
          timeout = 1;
          wifi_app_send_to_ble(WIFI_APP_TIMEOUT_NOTIFY, (uint8_t *)&timeout, 1);
          LOG_PRINT("\r\nWLAN Connect Failed, Error Code : 0x%lX\r\n", status);

          // update wlan application state
          disconnected = 1;
          connected    = 0;

          if(status == 0x10021){
            sl_net_deinit(SL_NET_WIFI_CLIENT_INTERFACE);
            sl_net_init(SL_NET_WIFI_CLIENT_INTERFACE, &config, NULL, NULL);
            rsi_ble_configurator_init();
          }
          else{
              wifi_app_set_event(WIFI_APP_ERROR_STATE);
          }
        } else {

          green_led();
          LOG_PRINT("\nWLAN Connection Success\n");
          SystemStatus.wifi_connect = true;
#if DISPLAY
          demoUIDisplayApp(SystemStatus);
#endif
          periodic_stop_timer(retry_handle);

          periodic_timer_counter = 0;

          // update wlan application state
          wifi_app_set_event(WIFI_APP_CONNECTED_STATE);
        }
     }
      break;

      case WIFI_APP_CONNECTED_STATE: {
        wifi_app_clear_event(WIFI_APP_CONNECTED_STATE);

        ip_address.type      = SL_IPV4;
        ip_address.mode      = SL_IP_MANAGEMENT_DHCP;
        ip_address.host_name = DHCP_HOST_NAME;

        // Configure IP
        status = sl_si91x_configure_ip_address(&ip_address, SL_SI91X_WIFI_CLIENT_VAP_ID);
        if (status != RSI_SUCCESS) {
          a++;
          if (a == 3) {
            a       = 0;
            timeout = 1;
            status1  = sl_wifi_disconnect(SL_WIFI_CLIENT_INTERFACE);
            if (status1 == RSI_SUCCESS) {
              connected     = 0;
              disassosiated = 1;
              wifi_app_send_to_ble(WIFI_APP_TIMEOUT_NOTIFY, (uint8_t *)&timeout, 1);
            }
          }
          if(status == 7 || status == 8 || status == 0x19){
            wifi_app_set_event(WIFI_APP_JOIN_STATE);}
          else if(status == 0x1FF9C || status == 0x1FFFC || status == 0x10025 || status == 0x10021){
            wifi_app_set_event(WIFI_APP_CONNECTED_STATE);}
          else{
            LOG_PRINT("\n Please Handle this error????\r\n");}

          LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%lX\r\n", status);
          break;
        } else {
          a             = 0;
          connected     = 1;
          conn_status   = 1;
          disconnected  = 0;
          disassosiated = 0;

          fetch_ip = &ip_address.ip.v4.ip_address;
          sprintf(ip_add,
                  "%d.%d.%d.%d",
                  fetch_ip->bytes[0],
                  fetch_ip->bytes[1],
                  fetch_ip->bytes[2],
                  fetch_ip->bytes[3]);
          LOG_PRINT("\r\nIP Address:%s \r\n", ip_add);

          // update wlan application state
          wifi_app_set_event(WIFI_APP_IPCONFIG_DONE_STATE);
          wifi_app_send_to_ble(WIFI_APP_CONNECTION_STATUS, (uint8_t *)&connected, 1);
        }

        if(new_cred)
        {
          new_cred = false;
          store_credentials_to_nvm(ap_credentials.ssid,ap_credentials.pwd,ap_credentials.sec_type);
        }
      } break;

      case WIFI_APP_IPCONFIG_DONE_STATE: {
        wifi_app_clear_event(WIFI_APP_IPCONFIG_DONE_STATE);
        wifi_app_set_event(WIFI_APP_MQTT_INIT_STATE);

        LOG_PRINT("WIFI App IPCONFIG Done State\r\n");
      } break;

      case WIFI_APP_DISCONNECTED_STATE: {
        wifi_app_clear_event(WIFI_APP_DISCONNECTED_STATE);
        retry = 1;
        wifi_app_send_to_ble(WIFI_APP_DISCONNECTION_STATUS, (uint8_t *)&disconnected, 1);
        wifi_app_set_event(WIFI_APP_JOIN_STATE);
        LOG_PRINT("WIFI App Disconnected State\r\n");
      } break;

      case WIFI_APP_DISCONN_NOTIFY_STATE: {
        wifi_app_clear_event(WIFI_APP_DISCONN_NOTIFY_STATE);

        erase_credentials();
        magic_word = 0;
        memset(&ap_credentials, 0, sizeof(ap_cred_t));
        status = sl_wifi_disconnect(SL_WIFI_CLIENT_INTERFACE);
        if (status == RSI_SUCCESS) {
#if RSI_WISE_MCU_ENABLE
          rsi_flash_erase((uint32_t)FLASH_ADDR_TO_STORE_AP_DETAILS);
#endif
          LOG_PRINT("\r\nWLAN Disconnected\r\n");
          disassosiated   = 1;
          connected       = 0;
          yield           = 0;
          disconnect_flag = 0; // reset flag to allow disconnecting again

          wifi_app_send_to_ble(WIFI_APP_DISCONNECTION_NOTIFY, (uint8_t *)&disassosiated, 1);
        } else {
          LOG_PRINT("\r\nWIFI Disconnect Failed, Error Code : 0x%lX\r\n", status);
        }
        rsi_ble_app_set_event(RSI_BLE_ADVERTISE);
      } break;

      case WIFI_APP_ERASE_FLASH:{
        wifi_app_clear_event(WIFI_APP_ERASE_FLASH);
        erase_credentials();
        wifi_app_set_event(WIFI_APP_DISCONN_NOTIFY_STATE);
      }break;

      /* ************ mqtt events *********** */
      case WIFI_APP_MQTT_INIT_STATE: {
        wifi_app_clear_event(WIFI_APP_MQTT_INIT_STATE);
#if AWS
        rc = aws_iot_mqtt_init(&mqtt_client, &mqttInitParams);
        if (SUCCESS != rc) {
            wifi_app_set_event(WIFI_APP_MQTT_INIT_STATE);
          LOG_PRINT("\r\nMqtt Initialization failed with error: %d\r\n", rc);
        } else {
          select_given = 0;
          pub_state = 0;
          wifi_app_set_event(WIFI_APP_MQTT_CONNECT_STATE);
        }
#elif ADAFRUIT
        sl_status_t status;
        status = mqtt_client_init();
        if (status == SL_STATUS_OK) {
            wifi_app_set_event(WIFI_APP_MQTT_CONNECT_STATE);
        }
#endif
      } break;


      case WIFI_APP_MQTT_CONNECT_STATE: {
        wifi_app_clear_event(WIFI_APP_MQTT_CONNECT_STATE);
#if AWS
        rc = aws_iot_mqtt_connect(&mqtt_client, &connectParams);
        if (SUCCESS != rc) {
          if (rc == NETWORK_ALREADY_CONNECTED_ERROR) {
            LOG_PRINT("Network is already connected\r\n");
            wifi_app_set_event(WIFI_APP_MQTT_PUBLISH_STATE);
          } else {
            LOG_PRINT("\r\nMqtt Connect failed with error: %d\r\n", rc);
            mqtt_disconnected = true;
            wifi_app_set_event(WIFI_APP_ERROR_STATE);
          }
        } else {
          periodic_stop_timer(retry_handle);
          LOG_PRINT("\r\nConnected to AWS IoT Cloud\n");

          mqtt_disconnected = false;
          wifi_app_set_event(WIFI_APP_MQTT_SUBSCRIBE_STATE);
        }
#elif ADAFRUIT
      sl_status_t status;
      status =
      sl_mqtt_client_connect(&client, &mqtt_broker_configuration, &last_will_message, &mqtt_client_configuration, 0);
      if (status != SL_STATUS_IN_PROGRESS) {
        LOG_PRINT("Failed to connect to mqtt broker: 0x%lx\r\n", status);
        mqtt_disconnected = true;
        mqtt_client_cleanup();
        break;
      }
      while(!mqtt_event_received);
      mqtt_event_received = false;
#endif
      } break;

#if AWS
      case WIFI_APP_MQTT_AUTO_RECONNECT_SET_STATE: {
        wifi_app_clear_event(WIFI_APP_MQTT_AUTO_RECONNECT_SET_STATE);
        rc = aws_iot_mqtt_autoreconnect_set_status(&mqtt_client, false);
        if (SUCCESS != rc) {
          if (NETWORK_DISCONNECTED_ERROR == rc) {
            LOG_PRINT("MQTT auto reconnect error : %d\r\n", rc);
            wifi_app_set_event(WIFI_APP_MQTT_CONNECT_STATE);
          } else if (NETWORK_ATTEMPTING_RECONNECT == rc) {
            // If the client is attempting to reconnect we will skip the rest of the loop.
            continue;
          }
          LOG_PRINT("Unable to set Auto Reconnect to true\r\n ");
          wifi_app_set_event(WIFI_APP_MQTT_AUTO_RECONNECT_SET_STATE);
        } else {
            wifi_app_set_event(WIFI_APP_MQTT_SUBSCRIBE_STATE);
        }
      } break;

      case WIFI_APP_MQTT_SUBSCRIBE_STATE: {
        wifi_app_clear_event(WIFI_APP_MQTT_SUBSCRIBE_STATE);
        LOG_PRINT("\r\nAWS IOT MQTT Subscribe...\r\n");
        rc = aws_iot_mqtt_subscribe(&mqtt_client,
                                    SUBSCRIBE_TO_TOPIC,
                                    strlen(SUBSCRIBE_TO_TOPIC),
                                    SUBSCRIBE_QOS,
                                    (void *)iot_subscribe_callback_handler,
                                    NULL);

        if (SUCCESS != rc) {
          if (NETWORK_DISCONNECTED_ERROR == rc) {
            LOG_PRINT("\r\nSubscribe error : %d\r\n", rc);
            mqtt_disconnected = true;
            wifi_app_set_event(WIFI_APP_MQTT_CONNECT_STATE);
          } else if (NETWORK_ATTEMPTING_RECONNECT == rc) {
            // If the client is attempting to reconnect we will skip the rest of the loop.
            continue;
          }
          wifi_app_set_event(WIFI_APP_ERROR_STATE);
        }
        else{
          LOG_PRINT("Subscribed to %s topic with QoS%d\r\n", SUBSCRIBE_TO_TOPIC, SUBSCRIBE_QOS);
          periodic_stop_timer(retry_handle);
          mqtt_disconnected = false;
          on_mqtt_connect = true;
          wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
        }
      } break;

      case WIFI_APP_AWS_SELECT_CONNECT_STATE: {
          wifi_app_clear_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
          if(connected && (mqtt_disconnected == false)){
            if (!select_given) {
              select_given = 1;
              memset(&read_fds, 0, sizeof(fd_set));

              FD_SET(mqtt_client.networkStack.socket_id, &read_fds);

              status1 =
                sl_si91x_select(mqtt_client.networkStack.socket_id + 1, &read_fds, NULL, NULL, NULL, async_socket_select);
            }
            if (check_for_recv_data) {
               check_for_recv_data = 0;
               select_given        = 0;
               wifi_app_set_event(WIFI_APP_MQTT_RECEIVE_STATE);
             }
            else if (SUBSCRIBE_QOS == QOS1 || PUBLISH_QOS == QOS1){
               if (qos1_publish_handle == 0 || on_mqtt_connect) {
                   wifi_app_set_event(WIFI_APP_MQTT_PUBLISH_STATE);
               } else {
#if ENABLE_POWER_SAVE
                 qos1_publish_handle = 0;
                 wifi_app_set_event(WIFI_APP_SLEEP_STATE);
#else
                 wifi_app_set_event(WIFI_APP_MQTT_PUBLISH_STATE);
#endif
               }
             }
           else if (SUBSCRIBE_QOS == QOS0 || PUBLISH_QOS == QOS0) {
               wifi_app_set_event(WIFI_APP_MQTT_PUBLISH_STATE);
             }
          }
          else{
              if(!connected){
              wifi_app_set_event(WIFI_APP_JOIN_STATE);}
          }
      } break;

      case WIFI_APP_MQTT_DISCONNECT: {
        wifi_app_clear_event(WIFI_APP_MQTT_DISCONNECT);
        rc = aws_iot_mqtt_disconnect(&mqtt_client);
        if (SUCCESS != rc) {
          LOG_PRINT("MQTT Disconnection with error %d:\r\n",rc);
        } else {
          LOG_PRINT("MQTT Disconnection Successful\r\n");
        }
        mqtt_disconnected = true;
        wifi_app_set_event(WIFI_APP_MQTT_INIT_STATE);
      } break;

      case WIFI_APP_MQTT_RECEIVE_STATE: {
        wifi_app_clear_event(WIFI_APP_MQTT_RECEIVE_STATE);
        if(connected && (mqtt_disconnected == false)){

          rc = aws_iot_shadow_yield(&mqtt_client, 1);
          if (SUCCESS != rc) {
            LOG_PRINT("\rYield failed\n");
            mqtt_disconnected = true;
            }
            wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
        }
      } break;

      case WIFI_APP_MQTT_PUBLISH_STATE: {
        wifi_app_clear_event(WIFI_APP_MQTT_PUBLISH_STATE);
#if ENABLE_POWER_SAVE
        osDelay(50);
        if(is_timer_running(motor_handle) || is_timer_running(auto_lock_handle))
        {
            break;
        }
#endif
        if (NETWORK_ATTEMPTING_RECONNECT == rc) {
          // If the client is attempting to reconnect we will skip the rest of the loop.
          continue;
        }
        rc = 0;
#if !(defined(SLI_SI91X_MCU_INTERFACE) && ENABLE_POWER_SAVE)
        if ((!publish_timer_start) || publish_msg) {
#endif
        publish_iot_msg.qos        = PUBLISH_QOS;
        publish_iot_msg.isRetained = 0;

        if(on_mqtt_connect)
        {
          memset(&SoP,0,sizeof(SoP));
          memset(temp_string,0,sizeof(temp_string));
          sprintf(temp_string,"%s SmartLock is Active Now",client_id);
        }
        else if(SoP.source[0] != '\0')
        {
          memset(temp_string,0,sizeof(temp_string));
          sprintf(temp_string,"%s from %s",SoP.command,SoP.source);
          publish_iot_msg.payload    = temp_string;
          publish_iot_msg.payloadLen = strlen(temp_string);
        }
        else if(is_i2c_enabled)
        {
          memset(temp_string,0,sizeof(temp_string));
          snprintf(temp_string, sizeof(temp_string) - 1, "Current Temperature in Celsius: %.2f", sensor_data);

        }

        publish_iot_msg.payload    = temp_string;
        publish_iot_msg.payloadLen = strlen(temp_string);

        if((SoP.source[0] != '\0') || is_i2c_enabled || on_mqtt_connect)// || send_lock_state)
        {
          memset(&SoP,0,sizeof(SoP));
          on_mqtt_connect = false;
          send_lock_state = false;

          if (SUBSCRIBE_QOS == QOS1 || PUBLISH_QOS == QOS1) {
              pub_state = 1;
            }
            LOG_PRINT("\rData to be published: %s\n",(char *) publish_iot_msg.payload);

            // mqtt publish with QOS0
            rc = aws_iot_mqtt_publish(&mqtt_client, PUBLISH_ON_TOPIC, strlen(PUBLISH_ON_TOPIC), &publish_iot_msg);
            if (rc != SUCCESS) {
              LOG_PRINT("\r\nMqtt Publish for QOS%d failed with error: %d\r\n", PUBLISH_QOS, rc);
              wifi_app_set_event(WIFI_APP_MQTT_DISCONNECT);
            }
            if (rc == MQTT_REQUEST_TIMEOUT_ERROR) {
              LOG_PRINT("\r\nACK not received for QoS%d publish\r\n", PUBLISH_QOS);
            }
            if (rc == SUCCESS)
            {
              LOG_PRINT("\rQoS%d publish is successful\r\n", PUBLISH_QOS);
              periodic_stop_timer(retry_handle);
#if ENABLE_POWER_SAVE
              if (SUBSCRIBE_QOS == QOS1 || PUBLISH_QOS == QOS1) {
                  wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
              } else if (SUBSCRIBE_QOS == QOS0 || PUBLISH_QOS == QOS0) {
                  wifi_app_set_event(WIFI_APP_SLEEP_STATE);
              }
#else
            wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
#endif
            }
        }
        else
        {
#if ENABLE_POWER_SAVE
          wifi_app_set_event(WIFI_APP_SLEEP_STATE);
#else
          /* To Handle keepalive */
          pub_state=0;
          rc = aws_iot_shadow_yield(&mqtt_client, 1);
          if (SUCCESS == rc) {
            select_given        = 0;
            LOG_PRINT("\rYield is successful\n");
          }else{
            LOG_PRINT("\rYield failed\n");
            }
          wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
#endif
        }

#if !(defined(SLI_SI91X_MCU_INTERFACE) && ENABLE_POWER_SAVE)
      publish_msg = 0;
      if (!publish_timer_start) {
        publish_timer_start = 1;
        start_time          = sl_si91x_host_get_timestamp();
      }
    } else {
          if (sl_si91x_host_elapsed_time(start_time) >= PUBLISH_PERIODICITY) {
            start_time          = 0;
            publish_timer_start = 0;
            publish_msg         = 1;
          }
        }
        wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
#endif
      } break;
#endif
      case WIFI_APP_ERROR_STATE: {
        wifi_app_clear_event(WIFI_APP_ERROR_STATE);
        if(periodic_timer_counter == PERIODIC_INTERVALS)
        {
           periodic_timer_counter = 0;
           put_system_in_sleep(wakeup_time_ms);
            continue;
        }
        if(retry_timeout){
            retry_timeout = false;
          if(!connected){
               wifi_app_set_event(WIFI_APP_JOIN_STATE);
               break;
           }
          if(connected){
            wifi_app_set_event(WIFI_APP_MQTT_INIT_STATE);
            break;
           }
        }
        else{
            start_periodic_timer(retry_handle,PERIODIC_TIMEOUT_MS);
        }
      } break;

      case WIFI_APP_SLEEP_STATE: {
        wifi_app_clear_event(WIFI_APP_SLEEP_STATE);
#if AWS
        if(is_timer_running(motor_handle) || SoP.source[0])
        {
            wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
            break;
        }

        if (select_given == 1 && (check_for_recv_data != 1)){
            put_system_in_sleep(wakeup_time_ms);
        }
#elif ADAFRUIT
        if(connected){
            put_system_in_sleep(wakeup_time_ms);
        }
#endif
      } break;
      case WIFI_APP_IDLE_STATE: {
        wifi_app_clear_event(WIFI_APP_IDLE_STATE);

        if(connected && mqtt_disconnected == false)
        {
          #if AWS
              wifi_app_set_event(WIFI_APP_MQTT_RECEIVE_STATE);
          #elif ADAFRUIT
              wifi_app_set_event(WIFI_APP_SLEEP_STATE);
          #endif
        }
      }break;
      default:
        LOG_PRINT("Default case received\r\n");
        wifi_app_clear_event(WIFI_CLEAR_ALL_STATES);
        break;
     }
   }

  if (scan_result != NULL) {
    free(scan_result);
  }

  osDelay(10);
}
#if ADAFRUIT
void mqtt_client_cleanup()
{
  SL_CLEANUP_MALLOC(client_credentails);
  is_execution_completed = 1;
}

sl_status_t mqtt_client_init()
{
  sl_status_t status;

#if ENCRYPT_CONNECTION
  // Load SSL CA certificate
  status = sl_net_set_credential(SL_NET_TLS_SERVER_CREDENTIAL_ID(CERTIFICATE_INDEX),
                                 SL_NET_SIGNING_CERTIFICATE,
                                 cacert,
                                 sizeof(cacert) - 1);
  if (status != SL_STATUS_OK) {
      LOG_PRINT("\r\nLoading TLS CA certificate in to FLASH Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nLoad TLS CA certificate at index %d Success\r\n", 0);
#endif

  if (SEND_CREDENTIALS) {
    uint16_t username_length, password_length;

    username_length = strlen(USERNAME);
    password_length = strlen(PASSWORD);

    uint32_t malloc_size = sizeof(sl_mqtt_client_credentials_t) + username_length + password_length;

    client_credentails = malloc(malloc_size);
    if (client_credentails == NULL)
      return SL_STATUS_ALLOCATION_FAILED;
    memset(client_credentails, 0, malloc_size);
    client_credentails->username_length = username_length;
    client_credentails->password_length = password_length;

    memcpy(&client_credentails->data[0], USERNAME, username_length);
    memcpy(&client_credentails->data[username_length], PASSWORD, password_length);

    status = sl_net_set_credential(SL_NET_MQTT_CLIENT_CREDENTIAL_ID(0),
                                   SL_NET_MQTT_CLIENT_CREDENTIAL,
                                   client_credentails,
                                   malloc_size);

    if (status != SL_STATUS_OK) {
      mqtt_client_cleanup();
      LOG_PRINT("Failed to set credentials: 0x%lx\r\n ", status);

      return status;
    }
    LOG_PRINT("Set credentials Success \r\n ");

    free(client_credentails);
    mqtt_client_configuration.credential_id = SL_NET_MQTT_CLIENT_CREDENTIAL_ID(0);
  }

  status = sl_mqtt_client_init(&client, mqtt_client_event_handler);
  if (status != SL_STATUS_OK) {
      LOG_PRINT("Failed to init mqtt client: 0x%lx\r\n", status);

    mqtt_client_cleanup();
    return status;
  }
  LOG_PRINT("Init mqtt client Success \r\n");

  status = sl_net_inet_addr(MQTT_BROKER_IP, &mqtt_broker_configuration.ip.ip.v4.value);
  if (status != SL_STATUS_OK) {
      LOG_PRINT("Failed to convert IP address \r\n");

    mqtt_client_cleanup();
    return status;
  }
  mqtt_broker_configuration.ip.type = SL_IPV4;

  return SL_STATUS_OK;
}

void mqtt_client_message_handler(void *client, sl_mqtt_client_message_t *message, void *context)
{
  UNUSED_PARAMETER(client);
  UNUSED_PARAMETER(context);
  LOG_PRINT("Message Received on Topic: ");

  print_char_buffer((char *)message->topic, message->topic_length);
  print_char_buffer((char *)message->content, message->content_length);

  if (!strncmp ((char *)message->content, "lock",message->content_length))
  {
      if(!is_timer_running(motor_handle)){
        memset(&SoP,0,sizeof(SoP));
        strcat(SoP.source,"MQTT");
        strcat(SoP.command,"LOCK");
        mqtt_publish();
        rsi_app_set_event(LOCK_CMD_RECEIVED_STATE);
        osDelay(10);
      }
  }

  if (!strncmp ((char *)message->content, "unlock",message->content_length))
  {
      memset(&SoP,0,sizeof(SoP));
      strcat(SoP.source,"MQTT");
      strcat(SoP.command,"UNLOCK");
      mqtt_publish();
      rsi_app_set_event(UNLOCK_CMD_RECEIVED_STATE);
  }
}

void print_char_buffer(char *buffer, uint32_t buffer_length)
{
  for (uint32_t index = 0; index < buffer_length; index++) {
    LOG_PRINT("%c", buffer[index]);
  }

  LOG_PRINT("\r\n");
}

void mqtt_client_error_event_handler(void *client, sl_mqtt_client_error_status_t *error)
{
  UNUSED_PARAMETER(client);
  LOG_PRINT("\r\n mqtt error received in mqtt_client_error_event_handler: %d\r\n", *error);
  mqtt_client_cleanup();
  wifi_app_set_event(WIFI_APP_ERROR_STATE);
  mqtt_event_received = true;
}

void mqtt_client_event_handler(void *client, sl_mqtt_client_event_t event, void *event_data, void *context)
{
  switch (event) {
    case SL_MQTT_CLIENT_CONNECTED_EVENT: {
      sl_status_t status;
      mqtt_event_received = true;
      LOG_PRINT("\r\nConnect to mqtt broker Success \r\n");
      send_dev_active_status();
      status = sl_mqtt_client_subscribe(client,
                                        (uint8_t *)SUBSCRIBE_TOPIC,
                                        strlen(SUBSCRIBE_TOPIC),
                                        QOS_OF_SUBSCRIPTION,
                                        0,
                                        mqtt_client_message_handler,
                                        SUBSCRIBE_TOPIC);
      if (status != SL_STATUS_IN_PROGRESS) {
        LOG_PRINT("Failed to subscribe : 0x%lx\r\n", status);

        mqtt_client_cleanup();
        wifi_app_set_event(WIFI_APP_ERROR_STATE);
        break;
      }
      break;
    }

    case SL_MQTT_CLIENT_MESSAGE_PUBLISHED_EVENT: {
      sl_mqtt_client_message_t *published_message = (sl_mqtt_client_message_t *)context;

      LOG_PRINT("Published message successfully on topic: ");
      print_char_buffer((char *)published_message->topic, published_message->topic_length);
      break;
    }

    case SL_MQTT_CLIENT_SUBSCRIBED_EVENT: {
      char *subscribed_topic = (char *)context;

      LOG_PRINT("Subscribed to Topic: %s\r\n", subscribed_topic);
      mqtt_disconnected = false;
      periodic_stop_timer(retry_handle);

      wifi_app_set_event(WIFI_APP_SLEEP_STATE);
      break;
    }

    case SL_MQTT_CLIENT_UNSUBSCRIBED_EVENT: {
      char *unsubscribed_topic = (char *)context;

      LOG_PRINT("Unsubscribed from topic: %s\r\n", unsubscribed_topic);

      sl_mqtt_client_disconnect(client, 0);
      break;
    }

    case SL_MQTT_CLIENT_DISCONNECTED_EVENT: {
      LOG_PRINT("Disconnected from MQTT broker\r\n");
      mqtt_client_cleanup();
      mqtt_disconnected = true;
      if(!erase_from_mqtt)
        wifi_app_set_event(WIFI_APP_MQTT_INIT_STATE);
      break;
    }

    case SL_MQTT_CLIENT_ERROR_EVENT: {
      mqtt_client_error_event_handler(client, (sl_mqtt_client_error_status_t *)event_data);
      break;
    }
    default:
      break;
  }
}

sl_status_t mqtt_publish()
{
  if((SoP.source[0] != '\0') && !mqtt_disconnected)
  {
    char temp_string[100]={0};
    memset(temp_string,0,sizeof(temp_string));
    sprintf(temp_string,"%s from %s",SoP.command,SoP.source);
    message_to_be_published.content  = (uint8_t*) temp_string;
    message_to_be_published.content_length   = strlen(temp_string);

    memset(&SoP,0,sizeof(SoP));

    sl_status_t status;
    status = sl_mqtt_client_publish(&client, &message_to_be_published, 0, &message_to_be_published);
    if (status != SL_STATUS_IN_PROGRESS) {
      LOG_PRINT("Failed to publish message: 0x%lx\r\n", status);
    }
    wifi_app_set_event(WIFI_APP_SLEEP_STATE);
  }

  return SL_STATUS_OK;
}

sl_status_t send_dev_active_status()
{

  char temp_string[100]={0};
  memset(temp_string,0,sizeof(temp_string));
  sprintf(temp_string,"%s SmartLock is Active Now",client_id);
  message_to_be_published.content  = (uint8_t *)temp_string;
  message_to_be_published.content_length   = strlen(temp_string);

  sl_status_t status;
  status = sl_mqtt_client_publish(&client, &message_to_be_published, 0, &message_to_be_published);
  if (status != SL_STATUS_IN_PROGRESS) {
    LOG_PRINT("Failed to publish message: 0x%lx\r\n", status);
  }

  return SL_STATUS_OK;
}
#endif

/*******************************************************************************
 *  This API handles UULP GPIO pin interrupt 0 request
 ******************************************************************************/
void my_gpio_uulp_pin_interrupt_callback(uint32_t pin_intr)
{
  (void)(pin_intr);
  //NPSS GPIO-2 interrupt clr
  (*(volatile uint32_t *)(0x12080000UL + 0x08)) = 0x08; //NPSS GPIO-2 interrupt clr
  __disable_irq();
  while (sl_si91x_gpio_get_uulp_npss_pin(2) == 0)
    ; // waiting for the button release
  send_lock_state = true;
  rsi_app_set_event(BUTTON_INTERRUPT_RCVD);
  __enable_irq();
#if (SL_SI91X_TICKLESS_MODE == ENABLE)
  osSemaphoreRelease(data_received_semaphore);
#endif
}

void uulp_button_init(void)
{
  sl_status_t status;
  uulp_pad_config_t uulp_pad;
  do {
    uulp_pad.gpio_padnum = uulp_gpio_pin_config.port_pin.pin; // UULP GPIO pin number 2 is selected
    uulp_pad.pad_select  = SET;                      // UULP GPIO PAD is selected
    uulp_pad.mode        = CLR;                      // UULP GPIO mode 0 is selected
    uulp_pad.direction   = SET;                      // UULP GPIO direction is selected
    uulp_pad.receiver    = SET;                      // UULP GPIO receiver is enabled

    // Configure UULP GPIO pin 2 using driver pin configuration API.
    // Using this API by default GPIO mode is set as MODE 0. If any other mode is selected for any GPIO use
    // corresponding API sl_gpio_driver_set_pin_mode() is for mode setting.
    status = sl_gpio_set_configuration(uulp_gpio_pin_config);
    if (status != SL_STATUS_OK) {
      // Prints if pin configuration fails
      DEBUGOUT("sl_gpio_set_configuration, Error code: %lu", status);
      break; // breaks if error occurs
    }

    // Configure the UULP GPIO pin mode, receiver enable, direction and polarity.
    status = sl_si91x_gpio_driver_set_uulp_pad_configuration(&uulp_pad);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_gpio_driver_set_uulp_pad_configuration, Error code: %lu", status);
      break;
    }

    // Configure uulp pin interrupt. The pin interrupt in this application is performed using external triggering from button.
    // Press button0 for triggering UULP GPIO instance pin interrupt based on event selected.
    status = sl_gpio_driver_configure_interrupt(&uulp_gpio_pin_config.port_pin,
                                                UULP_GPIO_INTR_2,
                                                (sl_gpio_interrupt_flag_t)(RISE_EDGE_INTERRUPT),
                                                (sl_gpio_irq_callback_t)&my_gpio_uulp_pin_interrupt_callback,
                                                AVL_INTR_NO);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_gpio_configure_interrupt, Error code: %lu", status);
      break;
    }
  } while (false);
}

void put_system_in_sleep(int32_t wakeup_time)
{
#if ENABLE_POWER_SAVE

  osDelay(100);

  if(connected){
      //! initiating power save in BLE mode
      if (rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE) != RSI_SUCCESS) {
        LOG_PRINT("\r\n Failed to initiate power save in BLE mode \r\n");
      }

      sl_wifi_performance_profile_v2_t performance_profile = { .profile         = ASSOCIATED_POWER_SAVE_LOW_LATENCY,
                                                            .listen_interval = 1000 };
      sl_status_t rc = 0;
      rc = sl_wifi_set_performance_profile_v2(&performance_profile);
      if (rc != SL_STATUS_OK) {
        LOG_PRINT("\r\nAssociated Power save configuration Failed, Error Code : %lu\r\n", rc);
      }
  }
  else{ //unconnected state
     //! initiating power save in BLE mode
     if (rsi_bt_power_save_profile(PSP_MODE1, PSP_TYPE) != RSI_SUCCESS) {
       LOG_PRINT("\r\n Failed to initiate power save in BLE mode \r\n");
     }

     sl_wifi_performance_profile_v2_t performance_profile = { .profile         = DEEP_SLEEP_WITH_RAM_RETENTION,
                                                           .listen_interval = 1000 };
     sl_status_t rc = 0;
     rc = sl_wifi_set_performance_profile_v2(&performance_profile);
     if (rc != SL_STATUS_OK) {
       LOG_PRINT("\r\nstandby Power save configuration Failed, Error Code : %lu\r\n", rc);
     }
  }
//  LOG_PRINT("Device going to sleep for %ldsec\r\n",wakeup_time/1000);
  sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);

  device_in_sleep = true;

#if (SL_SI91X_TICKLESS_MODE)
    if (osSemaphoreAcquire(data_received_semaphore, wakeup_time) == osOK) {
    }
#else
    sl_si91x_power_manager_sleep();
#endif
//    LOG_PRINT("Device Woke up from sleep after %ldsec\r\n",wakeup_time/1000);

  //! initiating power save in BLE mode
  if (rsi_bt_power_save_profile(PSP_MODE, PSP_TYPE) != RSI_SUCCESS) {
    LOG_PRINT("\r\n Failed to initiate power save in BLE mode \r\n");
  }

  sl_wifi_performance_profile_v2_t performance_profile = { .profile         = HIGH_PERFORMANCE,
                                                        .listen_interval = 1000 };
  sl_status_t rc = 0;
  rc = sl_wifi_set_performance_profile_v2(&performance_profile);
  if (rc != SL_STATUS_OK) {
    LOG_PRINT("\r\nHigh Power save configuration Failed, Error Code : %lu\r\n", rc);
  }

  device_in_sleep = false;
  DEBUGINIT();

  sl_gpio_driver_deinit();
  sl_gpio_driver_init();

  sl_gpio_initialization();

  uulp_button_init();

   if(!connected || mqtt_disconnected){
     wifi_app_set_event(WIFI_APP_ERROR_STATE);
    }
  else{
#if AWS
      wifi_app_set_event(WIFI_APP_MQTT_RECEIVE_STATE);
#elif ADAFRUIT
      wifi_app_set_event(WIFI_APP_SLEEP_STATE);
#endif
  }
#endif
}
void periodic_retry_timer_callback(int arg)
{
    if(arg == 1){
      periodic_timer_counter++;
      periodic_stop_timer(retry_handle);
      retry_timeout = true;
      wifi_app_set_event(WIFI_APP_ERROR_STATE);
      return;
    }

    if(arg == 2)
    {
        periodic_stop_timer(motor_handle);
        timer_stop_received = true;
        osSemaphoreRelease(data_received_semaphore);
#if AWS
        if(SystemStatus.wifi_connect){
            wifi_app_set_event(WIFI_APP_MQTT_PUBLISH_STATE);
        }
#endif
        return;
    }

    if(arg == 3)
    {
        DEBUGOUT("\r\n *******unlock timeout, auto-lock the device******* \r\n");
        memset(&SoP,0,sizeof(SoP));
        strcat(SoP.source,"TIMEOUT");
        strcat(SoP.command,"AUTO-LOCK");
#if AWS
//        if(SystemStatus.wifi_connect){
//            wifi_app_set_event(WIFI_APP_AWS_SELECT_CONNECT_STATE);
//        }
#else //ADAFRUIT
        mqtt_publish();
#endif
        rsi_app_set_event(LOCK_CMD_RECEIVED_STATE);
        sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
        osDelay(10);
        return;
    }
}

