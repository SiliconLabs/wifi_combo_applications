/*******************************************************************************
* @file  rsi_ble_throughput_app.c
* @brief 
*******************************************************************************
* # License
* <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
 * @brief : This file contains example application to measure the BLE Throughput
 * for notifications and indications in different PHY data rates
 * @section Description :
 * The RS9116W device is configured as a GATT server and a peripheral device.
 * Based on the type of MCU, a push-button is configured as an interrupt for
 * controlling the data transfer. If the button is pressed, the RS9116W device
 * continuously sends the data until it pressed again.
 * The application has also the provision to choose the PHY rate and connection
 * parameters.
 =================================================================================*/

/**
 * Include files
 * */

//! BLE include file to refer BLE APIs
#include <rsi_ble_apis.h>
#include <rsi_ble_config.h>
#include <rsi_ble_common_config.h>
#include <rsi_bt_common_apis.h>
#include <rsi_bt_common.h>
#include <stdio.h>
#include "rsi_driver.h"
//! Common include file
#include <rsi_common_apis.h>


#ifdef RSI_M4_INTERFACE
	#include "rsi_board.h"
#endif


//! local device name
#define RSI_BLE_DEVICE_NAME (void *)"Throughput Test"

#define MITM_REQ                  0x01

#define MAX_MTU_SIZE 							240

//! application event list
#define RSI_BLE_CONN_EVENT                0x01
#define RSI_BLE_DISCONN_EVENT             0x02
#if SMP_ENABLE
	#define RSI_BLE_SMP_REQ_EVENT             0x03
	#define RSI_BLE_SMP_RESP_EVENT            0x04
	#define RSI_BLE_SMP_PASSKEY_EVENT         0x05
	#define RSI_BLE_SMP_FAILED_EVENT          0x06
	#define RSI_BLE_ENCRYPT_STARTED_EVENT     0x07
	#define RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT 0x08
	#define RSI_BLE_SC_PASSKEY_EVENT          0x09
	#define RSI_BLE_LTK_REQ_EVENT             0x0A
#endif
#define RSI_BLE_RECEIVE_REMOTE_FEATURES   0x0B
#define RSI_APP_EVENT_DATA_LENGTH_CHANGE  0x0C
#define RSI_APP_EVENT_PHY_UPDATE_COMPLETE 0x0D
#define RSI_BLE_CONN_UPDATE_EVENT         0x0E
#define RSI_BLE_MTU_EVENT                 0xF
#define RSI_BLE_GATT_WRITE_EVENT          0x10
#define RSI_BLE_MORE_DATA_REQ_EVENT       0x11
#define RSI_DATA_TRANSMIT_EVENT           0x12
#define RSI_TRANSMISSION_NOTIFICATION_ENABLE_EVENT  0x13
#define RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT 0x14
#define RSI_CONNECTION_PARAMETERS_EVENT   0X15
#define RSI_DEV_ADDR_LEN 									6
//! error code
#define BT_HCI_COMMAND_DISALLOWED 				0x4E0C


#define WRITE_WITHOUT_RESPONSE   0   //Notifications
#define WRITE_WITH_RESPONSE      1   //Indications

#define THROUGHPUT_TYPE 				WRITE_WITHOUT_RESPONSE

#define RSI_BLE_CLIENT_CHAR_DESP 0x2901

#ifdef RSI_WITH_OS
	//! BLE task stack size
	#define RSI_BT_TASK_STACK_SIZE 1000

	//! BT task priority
	#define RSI_BT_TASK_PRIORITY 1

	//! Number of packet to send or receive
	#define NUMBER_OF_PACKETS 1000

	//! Wireless driver task priority
	#define RSI_DRIVER_TASK_PRIORITY 2

	//! Wireless driver task stack size
	#define RSI_DRIVER_TASK_STACK_SIZE 3000

#endif

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! global parameters list
static uint32_t ble_app_event_mask;
static uint8_t str_remote_address[18];
static uint8_t remote_dev_address[6] = {0};
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
static uint16_t rsi_ble_att1_val_hndl;
static uint8_t device_found            = 0;
static uint64_t counter=0;
static uint16_t connection_phy_handle =0, connection_interval_handle=0, slave_latency_handle=0,supervision_timeout_handle=0,pdu_handle=0,mtu_handle=0;
static uint16_t ota_service_handle;
static uint8_t start_timer_var;
static uint8_t minimum_connection_interval=0,maximum_connection_interval =0;
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_map1;
float throughput =0;
float timing=0;
rsi_semaphore_handle_t ble_main_task_sem;
uint8_t conn_params_updated     = 0;
uint8_t notifications_handle,device_connected;
uint8_t data_transfer_handle;
uint8_t transfer_on;
uint8_t notifications_data_handle, indications_data_handle, data_sent_handle;
uint8_t send_buf[230] ={0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75};
uint16_t throughput_result_handle=0;
uint8_t transmission_on = 0;
uint32_t start_timer=0;
uint32_t stop_timer=0;
uint16_t latest_connection_interval;
uint8_t pdu_length[1]={0};
int32_t event_id;


void rsi_wireless_driver_task(void);

/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events(){
  ble_app_event_map  = 0;
  ble_app_event_mask = 0xFFFFFFFF;
  ble_app_event_mask = ble_app_event_mask; //To suppress warning while compiling
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
void rsi_ble_app_set_event(uint32_t event_num){
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

/*==============================================*/
/**
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_event(uint32_t event_num){
  if (event_num < 32) {
     ble_app_event_map &= ~BIT(event_num);
   } else {
     ble_app_event_map1 &= ~BIT((event_num - 32));
   }
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_clear_all_events
 * @brief      clears the all event.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_all_event(void){
  ble_app_event_map = 0;
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
static int32_t rsi_ble_app_get_event(void){
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
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  memcpy(remote_dev_address, resp_enh_conn->dev_addr, 6);
  LOG_PRINT("\r\nConnected - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, resp_enh_conn->dev_addr));
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
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
  memcpy(remote_dev_address, resp_conn->dev_addr, 6);
  LOG_PRINT("\r\nConnected - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, resp_conn->dev_addr));
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
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  UNUSED_PARAMETER(reason); //This statement is added only to resolve compilation warning, value is unchanged
  memcpy(remote_dev_address, resp_disconnect->dev_addr, 6);
  LOG_PRINT("\r\nDisconnected - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, resp_disconnect->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_DISCONN_EVENT);
}

#if SMP_ENABLE
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
void rsi_ble_on_smp_request(rsi_bt_event_smp_req_t *event_smp_req)
{
  memcpy(remote_dev_address, event_smp_req->dev_addr, 6);
  LOG_PRINT("\r\nsmp_req - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, event_smp_req->dev_addr));
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
void rsi_ble_on_smp_response(rsi_bt_event_smp_resp_t *bt_event_smp)
{
  memcpy(remote_dev_address, bt_event_smp->dev_addr, 6);
  LOG_PRINT("\r\nsmp_resp - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, bt_event_smp->dev_addr));
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
void rsi_ble_on_smp_passkey(rsi_bt_event_smp_passkey_t *event_smp_passkey)
{
  memcpy(remote_dev_address, event_smp_passkey->dev_addr, 6);
  LOG_PRINT("\r\nsmp_passkey - str_remote_address : %s\r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, event_smp_passkey->dev_addr));
  rsi_ble_app_set_event(RSI_BLE_SMP_PASSKEY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_passkey_display 
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_smp_passkey_display(rsi_bt_event_smp_passkey_display_t *smp_passkey_display)
{
  memcpy(remote_dev_address, smp_passkey_display->dev_addr, 6);
  LOG_PRINT("\r\nremote addr: %s, passkey: %s \r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, smp_passkey_display->dev_addr),
            smp_passkey_display->passkey);
  rsi_ble_app_set_event(RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_sc_passkey
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
void rsi_ble_on_sc_passkey(rsi_bt_event_sc_passkey_t *sc_passkey)
{
  memcpy(remote_dev_address, sc_passkey->dev_addr, 6);
  LOG_PRINT("\r\nremote addr: %s, passkey: %06ld \r\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, sc_passkey->dev_addr),
            sc_passkey->passkey);
  numeric_value = sc_passkey->passkey;
  rsi_ble_app_set_event(RSI_BLE_SC_PASSKEY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_le_ltk_req_event
 * @brief      invoked when ltk request event is received
 * @param[in]  le_ltk_req, ltk request parameters.
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_le_ltk_req_event(rsi_bt_event_le_ltk_request_t *le_ltk_req)
{
  LOG_PRINT(" \r\n rsi_ble_on_le_ltk_req_event \r\n");
  memcpy(&ble_ltk_req, le_ltk_req, sizeof(rsi_bt_event_le_ltk_request_t));
  rsi_ble_app_set_event(RSI_BLE_LTK_REQ_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_le_security_keys
 * @brief      its invoked when le secure keys events is received.
 * @param[in]  rsi_ble_event_le_security_keys, secure event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when security keys event is received after encription started. 
 */
void rsi_ble_on_le_security_keys(rsi_bt_event_le_security_keys_t *rsi_ble_event_le_security_keys)
{
  memcpy(&app_ble_sec_keys, rsi_ble_event_le_security_keys, sizeof(rsi_bt_event_le_security_keys_t));
  LOG_PRINT("\r\n remote_ediv: 0x%x\r\n", app_ble_sec_keys.remote_ediv);
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
void rsi_ble_on_smp_failed(uint16_t status, rsi_bt_event_smp_failed_t *event_smp_failed)
{
  memcpy(remote_dev_address, event_smp_failed->dev_addr, 6);
  LOG_PRINT("\r\nsmp_failed status: 0x%x, str_remote_address: %s\r\n",
            status,
            rsi_6byte_dev_address_to_ascii(str_remote_address, event_smp_failed->dev_addr));
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
  LOG_PRINT("\r\nstart encrypt status: %d \r\n", status);
  memcpy(&encrypt_keys, enc_enabled, sizeof(rsi_bt_event_encryption_enabled_t));
	for(int ltk_var=0;ltk_var<16;ltk_var++)
	{
	LOG_PRINT("%x",encrypt_keys.localltk[ltk_var]);
	}
  rsi_ble_app_set_event(RSI_BLE_ENCRYPT_STARTED_EVENT);
}

#endif
/*==============================================*/
/**
 * @fn         rsi_ble_fill_128bit_uuid
 * @brief      this function is used to form 128bit uuid of apple device from 32 bit uuid.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */
static void rsi_ble_fill_128bit_uuid (uint32_t uuid_32bit, uint16_t data2, uint16_t data3, uint8_t *data4, uuid_t *serv_uuid)
{
	uint8_t ix;
	serv_uuid->size = 16;
	rsi_uint32_to_4bytes((uint8_t *)&serv_uuid->val.val128.data1, uuid_32bit);
	rsi_uint16_to_2bytes((uint8_t *)&serv_uuid->val.val128.data2, data2);
	rsi_uint16_to_2bytes((uint8_t *)&serv_uuid->val.val128.data3, data3);
	rsi_uint16_to_2bytes(&serv_uuid->val.val128.data4[0], *(uint16_t *)data4);
	for (ix =0 ; ix < 6;ix++) {
		serv_uuid->val.val128.data4[2] = data4[2]; 
		serv_uuid->val.val128.data4[3] = data4[3]; 
		serv_uuid->val.val128.data4[4] = data4[4]; 
		serv_uuid->val.val128.data4[5] = data4[5]; 
		serv_uuid->val.val128.data4[6] = data4[6]; 
		serv_uuid->val.val128.data4[7] = data4[7]; 
	}
	return;
}
/*==============================================*/
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
static void rsi_ble_add_char_serv_att(void *serv_handler,
                                      uint16_t handle,
                                      uint8_t val_prop,
                                      uint16_t att_val_handle,
                                      uuid_t att_val_uuid)
{
  rsi_ble_req_add_att_t new_att = { 0 };

  //! preparing the attribute service structure
  new_att.serv_handler       = serv_handler;
  new_att.handle             = handle;
  new_att.att_uuid.size      = 2;
  new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
  new_att.property           = RSI_BLE_ATT_PROPERTY_READ;

	//! preparing the characteristic attribute value
	if (att_val_uuid.size == 16)
  {
    new_att.data_len = 4 + att_val_uuid.size;
    new_att.data[0] = val_prop;
    rsi_uint16_to_2bytes (&new_att.data[2], att_val_handle);
    memcpy(&new_att.data[4], &att_val_uuid.val.val128,sizeof(att_val_uuid.val.val128));
  }
  else
  {
    new_att.data_len = 4 ;
    new_att.data[0] = val_prop;
    rsi_uint16_to_2bytes (&new_att.data[2], att_val_handle);
    rsi_uint16_to_2bytes (&new_att.data[4], att_val_uuid.val.val16);
  }

  //! Add attribute to the service
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
                                     uint8_t data_len,
                                     uint8_t auth_read)
{
  rsi_ble_req_add_att_t new_att = { 0 };

  //! preparing the attributes
  new_att.serv_handler  = serv_handler;
  new_att.handle        = handle;
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
  if (val_prop & RSI_BLE_ATT_PROPERTY_NOTIFY || val_prop & RSI_BLE_ATT_PROPERTY_INDICATE) {
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
		    //! preparing the client characteristic attribute & values
    memset(&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler       = serv_handler;
    new_att.handle             = handle + 2;
    new_att.att_uuid.size      = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_DESP;
    new_att.property           = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE;
    new_att.data_len           = 2;
		
		rsi_ble_add_attribute(&new_att);
		
  }

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_simple_chat_serv
 * @brief      this function is used to add new servcie i.e.., simple chat service.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used at application to create new service.
 */
static void rsi_ble_add_simple_chat_serv(void)
{
  uuid_t new_uuid                       = { 0 };
  rsi_ble_resp_add_serv_t new_serv_resp = { 0 };
  uint8_t data1[232]                      = {'0'};
  uint8_t connection_parameter_data[2]= {0};

	//! adding the "OTA service" primary service (UUID: 1d14d6ee-fd63-4fa1-bfa4-8f47b42119f0)
  new_uuid.size = 16;
  uint8_t data4[8]={ 0xa4, 0xbf, 0x47,0x8f, 0xf0,0x19, 0x21, 0xb4,};
  rsi_ble_fill_128bit_uuid(0x1d14d6ee,0xfd63,0x4fa1, data4,&new_uuid);
  rsi_ble_add_service(new_uuid, &new_serv_resp);
	
	//! adding the "OTA Control attribute" characteristic service attribute (UUID:f7bf3564-fb6d-4e53-88a4-5e37e0326063)
  new_uuid.size = 16;
  uint8_t data5[8]={ 0xa4, 0x88,0x37, 0x5e, 0x63, 0x60, 0x32, 0xe0,};
  rsi_ble_fill_128bit_uuid(0xf7bf3564,0xfb6d,0x4e53, data5,&new_uuid);
	
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 1,
                            RSI_BLE_ATT_PROPERTY_WRITE,
                            new_serv_resp.start_handle + 2,
                            new_uuid);
	
	//!characteristic attribute value
  rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size         = 16;
	rsi_ble_fill_128bit_uuid(0xf7bf3564,0xfb6d,0x4e53, data5,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            RSI_BLE_ATT_PROPERTY_WRITE,
                            (uint8_t *)&data1,
                            sizeof (data1), 0);
	ota_service_handle = rsi_ble_att1_val_hndl; 

	//! adding the "Throughput Test Service" Primary serice(UUID:bbb99e70-fff7-46cf-abc7-2d32c71820f2) 
  new_uuid.size = 16;
	rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 3;	
  uint8_t data6[8]={ 0xc7, 0xab,0x32, 0x2d, 0xf2,0x20, 0x18, 0xc7,};
  rsi_ble_fill_128bit_uuid(0xbbb99e70,0xfff7,0x46cf, data6,&new_uuid);
  rsi_ble_add_service(new_uuid, &new_serv_resp);
	
	//! adding the "Indications" characteristic service attribute to the "Throughput Test Service"i.e,(UUID:6109b631-a643-4a51-83d2-2059700ad49f)
	rsi_ble_att1_val_hndl = 0;
	rsi_ble_att1_val_hndl = new_serv_resp.start_handle;
  new_uuid.size = 16;
  uint8_t data7[8]={ 0xd2, 0x83,0x59, 0x20, 0x9f, 0xd4, 0x0a, 0x70,};
  rsi_ble_fill_128bit_uuid(0x6109b631,0xa643,0x4a51, data7,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            RSI_BLE_ATT_PROPERTY_INDICATE,
                            rsi_ble_att1_val_hndl+2,
                            new_uuid);

	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2; 
	
	rsi_ble_fill_128bit_uuid(0x6109b631,0xa643,0x4a51, data7,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            RSI_BLE_ATT_PROPERTY_INDICATE,	
                            (uint8_t *)&data1,
                            sizeof (data1), 0);
	indications_data_handle = rsi_ble_att1_val_hndl;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl +2;
	
	//! adding the "Notifications" characteristic service attribute to the "Throughput Test Service" i.e,(UUID:47b73dd6-dee3-4da1-9be0-f5c539a9a4be).
	new_uuid.size = 16;
  uint8_t data8[8]={ 0xe0, 0x9b,0xc5, 0xf5, 0xbe, 0xa4, 0xa9, 0x39,};
  rsi_ble_fill_128bit_uuid(0x47b73dd6,0xdee3,0x4da1, data8,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            RSI_BLE_ATT_PROPERTY_NOTIFY,
                            rsi_ble_att1_val_hndl + 2,
                            new_uuid);

	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl+2;
	rsi_ble_fill_128bit_uuid(0x47b73dd6,0xdee3,0x4da1, data8,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            RSI_BLE_ATT_PROPERTY_NOTIFY,	
                            (uint8_t *)&data1,
                            sizeof (data1), 0);
	notifications_data_handle = rsi_ble_att1_val_hndl; //13
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl +2;
	
	//! adding the "Transmission ON" characteristic service attribute to the "Throughput Test Service"i.e,(UUID:be6b6be1-cd8a-4106-9181-5ffe2bc67718)
	new_uuid.size = 16;
  uint8_t data9[8]={ 0x81, 0x91,0xfe, 0x5f, 0x18, 0x77, 0xc6, 0x2b,};
  rsi_ble_fill_128bit_uuid(0xbe6b6be1,0xcd8a,0x4106, data9,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_WRITE_NO_RESPONSE),
                            rsi_ble_att1_val_hndl + 2,
                            new_uuid);
	
	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl+2;
	rsi_ble_fill_128bit_uuid(0xbe6b6be1,0xcd8a,0x4106, data9,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_WRITE_NO_RESPONSE),
                            (uint8_t *)&data1,
                            sizeof (data1), 0);
	data_transfer_handle = rsi_ble_att1_val_hndl; 
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl +2;
	
	//! adding the "Throughput Result" characteristic service attribute to the "Throughput Test Service" i.e,(UUID:adf32227-b00f-400c-9eeb-b903a6cc291b)
	new_uuid.size = 16;
  uint8_t data10[8]={ 0xeb, 0x9e,0x03, 0xb9, 0x1b, 0x29, 0xcc, 0xa6,};
  rsi_ble_fill_128bit_uuid(0xadf32227,0xb00f,0x400c, data10,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            (RSI_BLE_ATT_PROPERTY_INDICATE | RSI_BLE_ATT_PROPERTY_READ),
                            rsi_ble_att1_val_hndl + 2,
                            new_uuid);
	
	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl+2; 
	rsi_ble_fill_128bit_uuid(0xadf32227,0xb00f,0x400c, data10,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            (RSI_BLE_ATT_PROPERTY_INDICATE | RSI_BLE_ATT_PROPERTY_READ),	
                            (uint8_t *)&data1,
                            sizeof (data1), 0);
	throughput_result_handle = rsi_ble_att1_val_hndl; 
	
	//! adding the "Throughput Information Service" Primary serice(UUID:ba1e0e9f-4d81-bae3-f748-3ad55da38b46) 
  new_uuid.size = 16;
  uint8_t data11[8]={ 0x48, 0xf7,0xd5, 0x3a, 0x46,0x8b,0xa3, 0x5d,};
  rsi_ble_fill_128bit_uuid(0xba1e0e9f,0x4d81,0xbae3, data11,&new_uuid);
  rsi_ble_add_service(new_uuid, &new_serv_resp);
	
  //! adding the "Connection PHY" characteristic service attribute to the "Throughput Information Service" i.e,(UUID:00a82b93-0feb-2739-72be-abda1f5993d0)
	rsi_ble_att1_val_hndl = 0;
	rsi_ble_att1_val_hndl = new_serv_resp.start_handle;
  new_uuid.size = 16;
  uint8_t data12[8]={ 0xbe, 0x72,0xda, 0xab, 0xd0, 0x93, 0x59, 0x1f,};
  rsi_ble_fill_128bit_uuid(0x00a82b93,0x0feb,0x2739, data12,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
                            rsi_ble_att1_val_hndl+2,
                            new_uuid);
	

	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
	rsi_ble_fill_128bit_uuid(0x00a82b93,0x0feb,0x2739, data12,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),	
                            (uint8_t *)&connection_parameter_data,
                            sizeof (connection_parameter_data), 0);
	connection_phy_handle = rsi_ble_att1_val_hndl; 
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl +2;
	
	//! adding the "Connection Interval" characteristic service attribute to the "Throughput Information Service" i.e,(UUID:0a32f5a6-0a6c-4954-f413-a698faf2c664)
  new_uuid.size = 16;
  uint8_t data13[8]={ 0x13, 0xf4,0x98, 0xa6, 0x64, 0xc6, 0xf2, 0xfa,};
  rsi_ble_fill_128bit_uuid(0x0a32f5a6,0x0a6c,0x4954, data13,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
                            rsi_ble_att1_val_hndl+2,
                            new_uuid);
	

	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
	rsi_ble_fill_128bit_uuid(0x0a32f5a6,0x0a6c,0x4954, data13,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),	
                            (uint8_t *)&connection_parameter_data,
                            sizeof (connection_parameter_data), 0);
	connection_interval_handle = rsi_ble_att1_val_hndl;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl +2;
	
 //! adding the "Slave Latency" characteristic service attribute to the "Throughput Information Service" i.e,(UUID:ff629b92-332b-e7f7-975f-0e535872ddae)
  new_uuid.size = 16;
  uint8_t data14[8]={ 0x5f, 0x97, 0x53, 0x0e, 0xae, 0xdd, 0x72, 0x58};
  rsi_ble_fill_128bit_uuid(0xff629b92,0x332b,0xe7f7, data14,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
                            rsi_ble_att1_val_hndl+2,
                            new_uuid);
	

	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
	rsi_ble_fill_128bit_uuid(0xff629b92,0x332b,0xe7f7, data14,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),	
                            (uint8_t *)&connection_parameter_data,
                            sizeof (connection_parameter_data), 0);
	slave_latency_handle =rsi_ble_att1_val_hndl;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl +2;
	
	//! adding the "Supervision timeout" characteristic service attribute to the "Throughput Information Service"(UUID:67e2c4f2-2f50-914c-a611-adb3727b056d)
  new_uuid.size = 16;
  uint8_t data15[8]={ 0x11, 0xa6, 0xb3, 0xad, 0x6d, 0x05, 0x7b, 0x72};
  rsi_ble_fill_128bit_uuid(0x67e2c4f2,0x2f50,0x914c, data15,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
                            rsi_ble_att1_val_hndl+2,
                            new_uuid);
	

	new_uuid.size = 16;
  rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
	rsi_ble_fill_128bit_uuid(0x67e2c4f2,0x2f50,0x914c, data15,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),	
                            (uint8_t *)&connection_parameter_data,
                            sizeof (connection_parameter_data), 0);
	supervision_timeout_handle = rsi_ble_att1_val_hndl;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl +2;
	
 	//! adding the "PDU Size" characteristic service attribute to the "Throughput Information Service"(UUID:30cc364a-0739-268c-4926-36f112631e0c)
  new_uuid.size = 16;
  uint8_t data16[8]={ 0x26, 0x49, 0xf1, 0x36, 0x0c, 0x1e, 0x63, 0x12};
  rsi_ble_fill_128bit_uuid(0x30cc364a,0x0739,0x268c, data16,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
                            rsi_ble_att1_val_hndl+2,
                            new_uuid);
	

	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
	rsi_ble_fill_128bit_uuid(0x30cc364a,0x0739,0x268c, data16,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),	
                            (uint8_t *)&connection_parameter_data,
                            sizeof (connection_parameter_data), 0);
	pdu_handle =rsi_ble_att1_val_hndl;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl +2;
	
	//! adding the "MTU Size" characteristic service attribute to the "Throughput Information Service"(UUID:3816df2f-d974-d915-d26e-78300f25e86e)
  new_uuid.size = 16;
  uint8_t data17[8]={ 0x6e, 0xd2, 0x30, 0x78, 0x6e, 0xe8, 0x25, 0x0f};
  rsi_ble_fill_128bit_uuid(0x3816df2f,0xd974,0xd915, data17,&new_uuid);
  rsi_ble_add_char_serv_att(new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl+1,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),
                            rsi_ble_att1_val_hndl+2,
                            new_uuid);
	

	new_uuid.size = 16;
	rsi_ble_att1_val_hndl = rsi_ble_att1_val_hndl + 2;
	rsi_ble_fill_128bit_uuid(0x3816df2f,0xd974,0xd915, data17,&new_uuid);
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            rsi_ble_att1_val_hndl,
                            new_uuid,
                            (RSI_BLE_ATT_PROPERTY_NOTIFY | RSI_BLE_ATT_PROPERTY_READ),	
                            (uint8_t *)&connection_parameter_data,
                            sizeof (connection_parameter_data), 0);
	mtu_handle =rsi_ble_att1_val_hndl;
}

/*==============================================*/
/**
 * @fn         rsi_ble_data_length_change_event
 * @brief      invoked when data length is set
 * @section description
 * This Callback function indicates data length is set
 */
void rsi_ble_data_length_change_event(rsi_ble_event_data_length_update_t *rsi_ble_data_length_update)
{
  LOG_PRINT("\r\nMax_tx_octets: %d \r\n", rsi_ble_data_length_update->MaxTxOctets);
  LOG_PRINT("Max_tx_time: %d \r\n", rsi_ble_data_length_update->MaxTxTime);
  LOG_PRINT("Max_rx_octets: %d \r\n", rsi_ble_data_length_update->MaxRxOctets);
  LOG_PRINT("Max_rx_time: %d \r\n", rsi_ble_data_length_update->MaxRxTime);
  rsi_ble_app_set_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
}

/*==============================================*/
/**
 * @fn         rsi_ble_phy_update_complete_event
 * @brief      when phy update complete event is received
 * @param[out] rsi_ble_event_phy_update_complete  contains the controller support phy information.
 * @return     none.
 * @section description
 * This Callback function indicates PHY update complete event is received
 */
void rsi_ble_phy_update_complete_event(rsi_ble_event_phy_update_t *rsi_ble_event_phy_update_complete)
{
  memcpy(&rsi_app_phy_update_complete, rsi_ble_event_phy_update_complete, sizeof(rsi_ble_event_phy_update_t));

  LOG_PRINT("\r\n Tx Phy rate = %x  and Rx Phy rate = %x \n",
            rsi_ble_event_phy_update_complete->TxPhy,
            rsi_ble_event_phy_update_complete->RxPhy);
  rsi_ble_app_set_event(RSI_APP_EVENT_PHY_UPDATE_COMPLETE);
}
/*==============================================*/
/**
 * @fn         rsi_ble_on_conn_update_complete_event
 * @brief      invoked when conn update complete event is received
 * @param[out] rsi_ble_event_conn_update_complete contains the controller support conn information. 
 * @param[out] resp_status contains the response status (Success or Error code)
 * @return     none.
 * @section description
 * This Callback function indicated the conn update complete event is received
 */
void rsi_ble_on_conn_update_complete_event(rsi_ble_event_conn_update_t *rsi_ble_event_conn_update_complete,
                                           uint16_t resp_status)
{
  memcpy(remote_dev_address, rsi_ble_event_conn_update_complete->dev_addr, 6);
	memcpy(&rsi_app_conn_update_complete,rsi_ble_event_conn_update_complete,sizeof(rsi_ble_event_conn_update_t));
	latest_connection_interval = rsi_ble_event_conn_update_complete->conn_interval;
	latest_connection_interval = latest_connection_interval *1.25;
  LOG_PRINT("\r\n Connection parameters update completed \n ");
  LOG_PRINT("\r\n Connection interval = %d, \t Latency = %d, \tSupervision Timeout = %d \n",
            latest_connection_interval,
            rsi_ble_event_conn_update_complete->conn_latency,
            rsi_ble_event_conn_update_complete->timeout);
	if((conn_params_updated == 0)&&(rsi_app_conn_update_complete.conn_interval != 0xC8)){
		rsi_ble_app_set_event(RSI_CONNECTION_PARAMETERS_EVENT);
	}
  rsi_ble_app_set_event(RSI_BLE_CONN_UPDATE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_peripheral_on_remote_features_event
 * @brief      invoked when LE remote features event is received.
 * @param[in] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_peripheral_on_remote_features_event(rsi_ble_event_remote_features_t *rsi_ble_event_remote_features)
{
  LOG_PRINT("\r\nFeature received: %x \n", *(uint8_t *)rsi_ble_event_remote_features->remote_features);
  memcpy(&remote_dev_feature, rsi_ble_event_remote_features, sizeof(rsi_ble_event_remote_features_t));
  rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked  MTU size event are received
 */
static void rsi_ble_on_mtu_event(rsi_ble_event_mtu_t *rsi_ble_mtu)
{
  memcpy(&rsi_app_ble_mtu_event, rsi_ble_mtu, sizeof(rsi_ble_event_mtu_t));
  LOG_PRINT("\r\n MTU size received from remote device(%s) is %d\n",
            rsi_6byte_dev_address_to_ascii(str_remote_address, rsi_app_ble_mtu_event.dev_addr),
            rsi_app_ble_mtu_event.mtu_size);
	rsi_ble_app_set_event(RSI_BLE_MTU_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_gatt_write_event(uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  memcpy(&app_ble_write_event, rsi_ble_write, sizeof(rsi_ble_event_write_t));
  rsi_ble_app_set_event(RSI_BLE_GATT_WRITE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_more_data_req_event
 * @brief      its invoked when le more data request is received
 * @param[out] rsi_ble_more_data_evt contains the LE Device Buffer Indication information.
 * @return     none.
 * @section description
 * This callback function is invoked when indication confirmation response is received from the module.
 */
static void rsi_ble_more_data_req_event(rsi_ble_event_le_dev_buf_ind_t *rsi_ble_more_data_evt)
{
  //! set conn specific event
  rsi_ble_app_set_event(RSI_BLE_MORE_DATA_REQ_EVENT);
}
/*==============================================*/
/**
 * @fn         ble_on_indicate_confirmation_event
 * @brief      its invoked when indication confirmation event is received.
 * @param[out]  event_status contains the response status \n 
 *              0 - Success \n 
 *              Non-Zero Value	-	Failure \n
 * @param[out]  rsi_ble_event_set_att_rsp contains the profile response information. 
 * @return     none.
 * @section description
 * This callback function is invoked when indication confirmation response is received from the module.
 */
static void ble_on_indicate_confirmation_event(uint16_t event_status,rsi_ble_set_att_resp_t *rsi_ble_event_set_att_rsp)
{
	rsi_ble_app_set_event(RSI_DATA_TRANSMIT_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_throughput_test_app
 * @brief      This application is used for testing the SMP protocol.
 * @param[in]  none. 
 * @return     none.
 * @section description
 * This function is used to test the SMP Protocol.
 */
int32_t rsi_ble_throughput_test_app(void)
{
  uint32_t status = 0;
  uint8_t adv[31]         = { 2, 1, 6 };
#ifdef RSI_WITH_OS
  rsi_task_handle_t driver_task_handle = NULL;
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
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%X\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nDevice Initialization Success\r\n");
  }
#endif
#ifdef RSI_WITH_OS
  //! SiLabs module intialisation
  status = rsi_device_init(LOAD_NWP_FW);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%X\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nDevice Initialization Success\r\n");
  }
  //! Task created for Driver task
  rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                  (uint8_t *)"driver_task",
                  RSI_DRIVER_TASK_STACK_SIZE,
                  NULL,
                  RSI_DRIVER_TASK_PRIORITY,
                  &driver_task_handle);
#endif
	rsi_button_config();
  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%X\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nWireless Initialization Success\r\n");
  }
  rsi_ble_gap_register_callbacks(NULL,
                                 rsi_ble_on_connect_event,
                                 rsi_ble_on_disconnect_event,
                                 NULL,
                                 rsi_ble_phy_update_complete_event,
                                 rsi_ble_data_length_change_event,
                                 rsi_ble_on_enhance_conn_status_event,
                                 NULL,
                                 rsi_ble_on_conn_update_complete_event,
                                 NULL);

  rsi_ble_gap_extended_register_callbacks(rsi_ble_simple_peripheral_on_remote_features_event,
                                          rsi_ble_more_data_req_event);

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
                                  ble_on_indicate_confirmation_event,
                                  NULL);

	#if SMP_ENABLE
		//! registering the SMP callback functions
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
	#endif
  //! create ble main task if ble protocol is selected
  rsi_semaphore_create(&ble_main_task_sem, 0);
  //!  initializing the application events map
  rsi_ble_app_init_events();

  rsi_ble_add_simple_chat_serv();

  //! Set local name
  status=rsi_bt_set_local_name(RSI_BLE_DEVICE_NAME);
	if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nSet local name failed Error Code : 0x%X\r\n", status);
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
    LOG_PRINT("\r\nSet Advertising data failed Error Code : 0x%X\r\n", status);
    return status;
  } else {
    LOG_PRINT("\r\nSet Advertising data Success\r\n");
  }
  //! start addvertising
  status = rsi_ble_start_advertising();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n start advertising cmd failed with error code = %lx \n", status);
		return status;
  } else {
    LOG_PRINT("\r\n RS9116W device Advertising as Throughput Test\r\n");
  }

  status = rsi_throughput_ble_app();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\n Error code = %lx \n", status);
		return status;
  }
    //! Application main loop
return 0;
}

int rsi_throughput_ble_app()
{
  int32_t status =0;
	  //! waiting for events from controller.
  while (1) 
	{
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
      case RSI_BLE_CONN_EVENT: {
        //! event invokes when connection was completed
				
        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_CONN_EVENT);
				status = rsi_ble_stop_advertising();
				 if (status == RSI_SUCCESS) {
          LOG_PRINT("\r\n RS9116W device stops the advertising \n");
        }
        device_connected =1;
				#if(THROUGHPUT_TYPE == WRITE_WITHOUT_RESPONSE)
					data_sent_handle = notifications_data_handle;
				#elif(THROUGHPUT_TYPE == WRITE_WITH_RESPONSE)
					data_sent_handle = indications_data_handle;
				#endif
	#ifdef SMP_ENABLE
				 //! initiating the SMP pairing process
        status = rsi_ble_smp_pair_request(remote_dev_address, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n smp request cmd failed with error code = %lx \n", status);
        }
	#endif
				 //! Setting MTU Exchange event
        status = rsi_ble_mtu_exchange_event(remote_dev_address, MAX_MTU_SIZE);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n mtu request cmd failed with error code = %lx \n", status);
        }
      } break;

      case RSI_BLE_DISCONN_EVENT: {
        //! event invokes when disconnection was completed

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_DISCONN_EVENT);

        //! clear all pending events
        rsi_ble_app_clear_all_event();

        device_found        = 0;
        conn_params_updated = 0;
        device_connected 		= 0;
				
        //! start addvertising
        status = rsi_ble_start_advertising();
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n start advertising cmd failed with error code = %lx \n", status);
        } else {
          LOG_PRINT("\r\n Started Advertising \n");
        }
      } break;
	#if SMP_ENABLE
      case RSI_BLE_SMP_REQ_EVENT: {
        //! initiate SMP protocol as a Master

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_SMP_REQ_EVENT);
        
        //! initiating the SMP pairing process
        status = rsi_ble_smp_pair_request(remote_dev_address, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n smp pair request cmd failed with error code = %lx \n", status);
        }
      } break;

      case RSI_BLE_SMP_RESP_EVENT: {
        //! initiate SMP protocol as a Master

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_SMP_RESP_EVENT);

        //! initiating the SMP pairing process
        status = rsi_ble_smp_pair_response(remote_dev_address, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n smp pair response cmd failed with error code = %lx \n", status);
        }
      } break;

      case RSI_BLE_SMP_PASSKEY_EVENT: {
        //! initiate SMP protocol as a Master

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_SMP_PASSKEY_EVENT);

        //! initiating the SMP pairing process
        status = rsi_ble_smp_passkey(remote_dev_address, RSI_BLE_SMP_PASSKEY);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n smp passkey cmd failed with error code = %lx \n", status);
        }
      } break;
      case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT: {
        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
      } break;

      case RSI_BLE_SC_PASSKEY_EVENT: {
        rsi_ble_app_clear_event(RSI_BLE_SC_PASSKEY_EVENT);
        status = rsi_ble_smp_passkey(remote_dev_address, numeric_value);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n secure connection passkey cmd failed with error code = %lx \n", status);
        }
      } break;

      case RSI_BLE_LTK_REQ_EVENT: {
        rsi_ble_app_clear_event(RSI_BLE_LTK_REQ_EVENT);
        status = rsi_ble_ltk_req_reply(remote_dev_address, 0, NULL);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n Ltk req reply cmd failed with error code = %lx \n", status);
        }
      } break;

      case RSI_BLE_SMP_FAILED_EVENT: {
        //! initiate SMP protocol as a Master

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_SMP_FAILED_EVENT);
      } break;

      case RSI_BLE_ENCRYPT_STARTED_EVENT: {
        //! start the encrypt event
				
        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_ENCRYPT_STARTED_EVENT);
      } break;

	#endif
      case RSI_BLE_RECEIVE_REMOTE_FEATURES: {
        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
         if (remote_dev_feature.remote_features[0] & 0x20) {
          status = rsi_ble_set_data_len(remote_dev_address, TX_LEN, TX_TIME);
          if (status != RSI_SUCCESS) {
            LOG_PRINT("\r\n set data length cmd failed with error code = %lx \n", status);
            rsi_ble_app_set_event(RSI_BLE_RECEIVE_REMOTE_FEATURES);
          }
        } 
			 else if (remote_dev_feature.remote_features[1] & 0x01) {
					status = rsi_ble_setphy((int8_t *)remote_dev_address, TX_PHY_RATE, RX_PHY_RATE, CODED_PHY_RATE);
          if (status != RSI_SUCCESS) {
            if (status != BT_HCI_COMMAND_DISALLOWED) {
              //retry the same command
              rsi_ble_app_set_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
            } 
          }
        }			
      } break;

      case RSI_APP_EVENT_DATA_LENGTH_CHANGE: {
        //! clear the disconnected event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);

        if (remote_dev_feature.remote_features[1] & 0x01) {
					rsi_delay_ms(100);
          status = rsi_ble_setphy((int8_t *)remote_dev_address, TX_PHY_RATE, RX_PHY_RATE, CODED_PHY_RATE);
          if (status != RSI_SUCCESS) {
            if (status != BT_HCI_COMMAND_DISALLOWED) {
              //retry the same command
              rsi_ble_app_set_event(RSI_APP_EVENT_DATA_LENGTH_CHANGE);
            } 
						else{
							LOG_PRINT("\r\n set phy cmd failed with error code = %lx \n", status);
						}
          }
        }
      } break;

      case RSI_APP_EVENT_PHY_UPDATE_COMPLETE: {
        //! phy update complete event

        //! clear the phy update complete event.
        rsi_ble_app_clear_event(RSI_APP_EVENT_PHY_UPDATE_COMPLETE);
      } break;

      case RSI_BLE_CONN_UPDATE_EVENT: {
				//! clear the connection update event.
        rsi_ble_app_clear_event(RSI_BLE_CONN_UPDATE_EVENT);
				status = rsi_ble_set_local_att_value(connection_interval_handle,2,(uint8_t*)&rsi_app_conn_update_complete.conn_interval);
				if(status !=RSI_SUCCESS){
					LOG_PRINT("\r\nconnetion interval update failure:0x%X\n",status);
				}
				else {
					LOG_PRINT("\r\nconnetion interval update success\n");
				}
				status = rsi_ble_set_local_att_value(slave_latency_handle,2,(uint8_t*)&rsi_app_conn_update_complete.conn_latency);
				if(status !=RSI_SUCCESS){
					LOG_PRINT("\r\nconnetion latency update failure:0x%X\n",status);
				}
				else{
					LOG_PRINT("\r\nconnetion latency update success\n");
				}
				uint16_t supervision_timeout_var = rsi_app_conn_update_complete.timeout/10;
				status = rsi_ble_set_local_att_value(supervision_timeout_handle,2,(uint8_t*)&supervision_timeout_var);	
				if(status !=RSI_SUCCESS){
					LOG_PRINT("\r\nsupervision timeout update failure:0x%X\n",status);
				}
				else{
					LOG_PRINT("\r\nsupervision timeout update success\n");
				}
				status = rsi_ble_notify_value(remote_dev_address,connection_phy_handle,2,&rsi_app_phy_update_complete.RxPhy);
				if(status !=RSI_SUCCESS){
					LOG_PRINT("\r\nphy update failure:0x%X\n",status);
				}
				else{
					LOG_PRINT("\r\nphy update success\n");
				}
				rsi_ble_app_set_event(RSI_DATA_TRANSMIT_EVENT);
      } break;

      case RSI_BLE_MTU_EVENT: {
        //! event invokes when write/notification events received

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_MTU_EVENT);
        status = rsi_ble_set_wo_resp_notify_buf_info(remote_dev_address, DLE_BUFFER_MODE, DLE_BUFFER_COUNT);
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\n failed to set the buffer configuration mode, error:0x%X \r\n", status);
          break;
        } else {
          LOG_PRINT("\r\n Buf configuration done for notify and set_att cmds buf mode = %d , max buff count =%d \n",
                    DLE_BUFFER_MODE,
                    DLE_BUFFER_COUNT);
        }
      	status = rsi_ble_set_local_att_value(mtu_handle,2,(uint8_t*)&rsi_app_ble_mtu_event.mtu_size);	
				if(status !=RSI_SUCCESS){
					LOG_PRINT("\r\nMTU size update failure:0x%X\n",status);
				}
				else{
					LOG_PRINT("\r\nMTU size update success\n");
				}		
				pdu_length[0] =sizeof(send_buf);
				status = rsi_ble_set_local_att_value(pdu_handle,2,&pdu_length[0]);	
				if(status !=RSI_SUCCESS){
					LOG_PRINT("\r\nPDU size update failure:0x%X\n",status);
				}
				else{
					LOG_PRINT("\r\nPDU size update success\n");
				}
				 status = rsi_ble_conn_params_update(remote_dev_address,CONN_INTERVAL_DEFAULT_MIN,CONN_INTERVAL_DEFAULT_MAX,CONN_LATENCY,SUPERVISION_TIMEOUT);
         if (status != RSI_SUCCESS) {
            LOG_PRINT("\r\n conn params update cmd failed with status = %lx \r\n", status);
          } else {
            LOG_PRINT("\r\nDefault Connection intervals are updated");
          }
      } break;

      case RSI_BLE_GATT_WRITE_EVENT: {

        //! event invokes when write/notification events receive
				#if(RSI_BLE_GATT_ASYNC_ENABLE==1)
							//! clear the served event
					rsi_ble_app_clear_event(RSI_BLE_GATT_WRITE_EVENT);
				#endif
      } break;

      case RSI_DATA_TRANSMIT_EVENT: {
        if (transmission_on == 1) {
					if(start_timer_var == 0x01){
						start_timer_var = 0;
						LOG_PRINT("\r\nstarted\r\n");
						start_timer = rsi_hal_gettickcount();
					}
				#if(THROUGHPUT_TYPE == WRITE_WITHOUT_RESPONSE)
					status = rsi_ble_set_local_att_value(data_sent_handle,sizeof(send_buf),(uint8_t *)send_buf);
          if (status != RSI_SUCCESS) {
            if (status == RSI_ERROR_BLE_DEV_BUF_FULL) {
              //! wait for the more data request received from the device
              rsi_ble_app_clear_event(RSI_DATA_TRANSMIT_EVENT);
              break;
            } else {
              LOG_PRINT("\r\n write with response failed with status = %lx \r\n", status);
            }
          }
					else{
					  counter=counter+1;
						++send_buf[0];
						if(send_buf[0] == 0x64){
							send_buf[0]=0x00;													
						}
						break;
					}
				#elif(THROUGHPUT_TYPE == WRITE_WITH_RESPONSE)
					status = rsi_ble_indicate_value(remote_dev_address,data_sent_handle,sizeof(send_buf),(uint8_t *)send_buf);
          if (status != RSI_SUCCESS) 
					{
            if (status == RSI_ERROR_BLE_DEV_BUF_FULL) {
              //! wait for the more data request received from the device
              rsi_ble_app_clear_event(RSI_DATA_TRANSMIT_EVENT);
              break;
            } 
          }
					else{
					  counter=counter+1;
						++send_buf[0];
						if(send_buf[0] == 0x64){
							send_buf[0]=0x00;					
						}
						break;
					}
			#endif
        } else {
          //! clear the served event
          rsi_ble_app_clear_event(RSI_DATA_TRANSMIT_EVENT);
        }
      } break;

      case RSI_BLE_MORE_DATA_REQ_EVENT: {

        //! clear the served event
        rsi_ble_app_clear_event(RSI_BLE_MORE_DATA_REQ_EVENT);

        //! Resumeing the data transfer on the more data req from the device.
        rsi_ble_app_set_event(RSI_DATA_TRANSMIT_EVENT);
      } break;
      
			case RSI_TRANSMISSION_NOTIFICATION_ENABLE_EVENT: {
				 //! clear the served event
					rsi_ble_app_clear_event(RSI_TRANSMISSION_NOTIFICATION_ENABLE_EVENT);
					uint8_t enable_notify[1]={1};
					status = rsi_ble_set_local_att_value(data_transfer_handle,1,&enable_notify[0]);
					if(status == RSI_SUCCESS) {
							LOG_PRINT("\r\n Remote device enabled the notification \n");
							//! set the data tranfer event
							counter=0;
						  start_timer_var =1;
							rsi_ble_app_set_event(RSI_CONNECTION_PARAMETERS_EVENT);						  						
  				}				
			} break;
			case RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT: {
				//! clear the served event
        rsi_ble_app_clear_event(RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT);
				transfer_on = 0;
				stop_timer =rsi_hal_gettickcount();
				timing =(((float)(stop_timer -start_timer))/1000);
				throughput=(((float)(counter*230*8))/timing);
				uint8_t disabel_notify[1]={0};
# if (THROUGHPUT_TYPE == WRITE_WITHOUT_RESPONSE)
				status = rsi_ble_set_local_att_value(data_transfer_handle,1,&disabel_notify[0]);
        if(status == RSI_SUCCESS){
					status = rsi_ble_set_local_att_value(throughput_result_handle,2,(uint8_t *)&throughput);
				}					
				status = rsi_ble_indicate_value(remote_dev_address,throughput_result_handle,4,(uint8_t *)&throughput); 
				if(status == RSI_SUCCESS){
					LOG_PRINT("\r\nThroughput : %.02f bps\n",throughput);
					LOG_PRINT("\r\nThroughput : %.02f kbps\n",(throughput/1000));
					LOG_PRINT("\r\n Time duration in sec:%0.2f \n",timing);
					rsi_ble_app_set_event(RSI_CONNECTION_PARAMETERS_EVENT);	
					}
#elif (THROUGHPUT_TYPE == WRITE_WITH_RESPONSE)
				rsi_delay_ms(1);
				status = rsi_ble_notify_value(remote_dev_address,data_transfer_handle,1,&disabel_notify[0]);
        if(status == RSI_SUCCESS){				
				 status = rsi_ble_indicate_value(remote_dev_address,throughput_result_handle,4,(uint8_t *)&throughput); 
         if(status!= RSI_SUCCESS){
             LOG_PRINT("\r\n Indication error:0x%X \n",status);
          }
				LOG_PRINT("\r\nThroughput : %.07f bps\n",throughput);
			  LOG_PRINT("\r\nThroughput : %.07f kbps\n",(throughput/1000));
				LOG_PRINT("\r\n Time duration in sec:%0.2f \n",timing);

				rsi_ble_app_set_event(RSI_CONNECTION_PARAMETERS_EVENT);
				}
#endif

			} break;			
			case RSI_CONNECTION_PARAMETERS_EVENT: {
				//! clear the served event
				rsi_ble_app_clear_event(RSI_CONNECTION_PARAMETERS_EVENT);
				if(conn_params_updated == 0){
 				 minimum_connection_interval = CONN_INTERVAL_DEFAULT_MIN;
				 maximum_connection_interval = CONN_INTERVAL_DEFAULT_MAX;
				}
				else{
				 minimum_connection_interval = CONN_INTERVAL_MIN;
				 maximum_connection_interval = CONN_INTERVAL_MAX;
				}
				status = rsi_ble_conn_params_update(remote_dev_address,minimum_connection_interval,maximum_connection_interval,CONN_LATENCY,SUPERVISION_TIMEOUT);
				if (status != RSI_SUCCESS) {
					LOG_PRINT("\r\n conn params update cmd failed with status = %lx \r\n", status);
				} else {
					LOG_PRINT("\r\nConnection intervals are updated");
				}				
			}break;
      default: {
      }
    }
	}
}
/*==============================================*/
/**
 * @fn         data_transfer
 * @brief      this function is used to start or stop the data transfer
 * @param[in]  transfer_on, start or stop the data transfer
 * @return     none
 * @section description
 * This function is used to start or stop the data transfer to the remote device.
 */
void data_transfer(uint8_t transfer_on)
{
			if(transfer_on == 0x01){
				transmission_on =1;
				conn_params_updated =1;
			  rsi_ble_app_set_event(RSI_TRANSMISSION_NOTIFICATION_ENABLE_EVENT);
			}
			else{
				transmission_on =0;
				conn_params_updated =0;
			  rsi_ble_app_set_event(RSI_TRANSMISSION_NOTIFICATION_DISABLE_EVENT);
			}			
}
/*==============================================*/
/**
 * @fn         main_loop
 * @brief      Schedules the Driver task.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function schedules the Driver task.
 */
void main_loop(void)
{
  while (1) {
    rsi_wireless_driver_task();
  }
}

/*==============================================*/
/**
 * @fn         main
 * @brief      main function
 * @param[in]  none.
 * @return     none.
 * @section description
 * This is the main function to call Application
 */
int main(void)
{

#ifdef RSI_WITH_OS
  int32_t status =0;
  rsi_task_handle_t bt_task_handle     = NULL;
#endif

#ifndef RSI_WITH_OS
  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);
  //! Call Simple SMP protocol test Application
  rsi_ble_throughput_test_app();

  //! Application main loop
  main_loop();

  return 0;
#endif
	
#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > BT_GLOBAL_BUFF_LEN)) {
    return status;
  }
  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create((rsi_task_function_t)rsi_ble_throughput_test_app,
                  (uint8_t *)"ble_task",
                  RSI_BT_TASK_STACK_SIZE,
                  NULL,
                  RSI_BT_TASK_PRIORITY,
                  &bt_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
