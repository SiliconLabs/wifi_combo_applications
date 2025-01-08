 /*************************************************************************
 *
 * Copyright (c) 2019 Redpine Signals Incorporated. All Rights Reserved.
 *
 * NOTICE:  All  information  contained  herein is, and  remains  the  property of
 * Redpine Signals Incorporated. The intellectual and technical concepts contained
 * herein  are  proprietary to  Redpine Signals Incorporated and may be covered by
 * U.S. and Foreign Patents, patents in process, and are protected by trade secret
 * or copyright law. Dissemination  of this  information or reproduction  of  this
 * material is strictly forbidden unless prior written permission is obtained from
 * Redpine Signals Incorporated.
 */

/*================================================================================
 * @brief : This file contains example application for Wlan Station BLE Multiple
 * Slaves Bridge
 * @section Description :
 * This Application explains user how to Create chat service, Configure device in
 * Central mode, Connect to multiple slaves one by one, Receive data sent by Smart
 * phone and forward to WLAN task, Send data received by WLAN task and Smart phone
 =================================================================================*/

/**
 * Include files
 * */


//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<string.h>
#include<stdio.h>
//! Common include file 
#include<rsi_common_apis.h>
#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//! Set this macro to enable LE ULP
#define BLE_PS_ENABLE         0

//! Power Save Profile Mode
#define PSP_TYPE              RSI_MAX_PSP

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the devices to connect
#define RSI_BLE_DEV_1_ADDR      "00:1B:DC:07:31:E5"
#define RSI_BLE_DEV_2_ADDR      "00:1A:7D:DA:81:0D"
#define RSI_BLE_DEV_3_ADDR      "00:1A:7D:DA:71:0A"
#define RSI_BLE_DEV_4_ADDR      "C0:FF:EE:C0:FF:EE"
#define RSI_BLE_DEV_5_ADDR      "00:1A:7D:DA:71:15"
#define RSI_BLE_DEV_6_ADDR      "00:1A:7D:DA:72:13"
#define RSI_BLE_DEV_7_ADDR      "00:1A:7D:DB:71:21"
#define RSI_BLE_DEV_8_ADDR      "00:1A:7D:DA:71:0C"

//! Maximum number of multiple slaves supported.
#define MAX_NUM_OF_SLAVES       8

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Maximum number of advertise reports to hold
#define NO_OF_ADV_REPORTS              8


//! BLE attribute service types uuid values
#define  RSI_BLE_CHAR_SERV_UUID         0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID       0x2902

//! BLE characteristic service uuid
#define  RSI_BLE_NEW_SERVICE_UUID       0xAABB
#define  RSI_BLE_ATTRIBUTE_1_UUID       0x1AA1
#define  RSI_BLE_ATTRIBUTE_2_UUID       0x1BB1


//! local device name
#define RSI_BLE_APP_DEVICE_NAME        "CO-EX_MULTIPLE_SLAVES"

//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ      0x02
#define  RSI_BLE_ATT_PROPERTY_WRITE     0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY    0x10

//! application event list
#define  RSI_BLE_ADV_REPORT_EVENT       0x00
#define  RSI_BLE_CONN_EVENT             0x01
#define  RSI_BLE_DISCONN_EVENT          0x02
#define  RSI_BLE_SCAN_RESTART_EVENT     0x03
#define  RSI_BLE_SLAVE1_START_NOTIFY    0x04
#define  RSI_BLE_SLAVE1_STOP_NOTIFY     0x05
#define  RSI_BLE_SLAVE2_START_NOTIFY    0x06
#define  RSI_BLE_SLAVE2_STOP_NOTIFY     0x07
#define  RSI_BLE_SLAVE3_START_NOTIFY    0x08
#define  RSI_BLE_SLAVE3_STOP_NOTIFY     0x09
#define  RSI_BLE_SLAVE4_START_NOTIFY    0x0A
#define  RSI_BLE_SLAVE4_STOP_NOTIFY     0x0B
#define  RSI_BLE_SLAVE5_START_NOTIFY    0x0C
#define  RSI_BLE_SLAVE5_STOP_NOTIFY     0x0D
#define  RSI_BLE_SLAVE6_START_NOTIFY    0x0E
#define  RSI_BLE_SLAVE6_STOP_NOTIFY     0x0F
#define  RSI_BLE_SLAVE7_START_NOTIFY    0x10
#define  RSI_BLE_SLAVE7_STOP_NOTIFY     0x11
#define  RSI_BLE_SLAVE8_START_NOTIFY    0x12
#define  RSI_BLE_SLAVE8_STOP_NOTIFY     0x13


//! global variables
uint8_t remote_dev_addr[18] = {0};
uint8_t conn_dev_addr[18] = {0};
uint8_t remote_dev_bd_addr[6] = {0};

uint16_t num_of_conn_slaves = 0;
uint16_t conn_req_pending = 0;

//! Enumeration for commands used in application
typedef enum rsi_app_cmd_e
{
  RSI_SLAVE1_DATA               = 0,
  RSI_SLAVE2_DATA               = 1,
  RSI_SLAVE3_DATA               = 2,
  RSI_SLAVE4_DATA               = 3,
  RSI_SLAVE5_DATA               = 4,
  RSI_SLAVE6_DATA               = 5,
  RSI_SLAVE7_DATA               = 6,
  RSI_SLAVE8_DATA               = 7,
  RSI_SLAVE1_START_NOTIFICATION = 8,
  RSI_SLAVE1_STOP_NOTIFICATION  = 9, 
  RSI_SLAVE2_START_NOTIFICATION = 10,
  RSI_SLAVE2_STOP_NOTIFICATION  = 11, 
  RSI_SLAVE3_START_NOTIFICATION = 12,
  RSI_SLAVE3_STOP_NOTIFICATION  = 13, 
  RSI_SLAVE4_START_NOTIFICATION = 14,
  RSI_SLAVE4_STOP_NOTIFICATION  = 15, 
  RSI_SLAVE5_START_NOTIFICATION = 16,
  RSI_SLAVE5_STOP_NOTIFICATION  = 17, 
  RSI_SLAVE6_START_NOTIFICATION = 18,
  RSI_SLAVE6_STOP_NOTIFICATION  = 19, 
  RSI_SLAVE7_START_NOTIFICATION = 20,
  RSI_SLAVE7_STOP_NOTIFICATION  = 21, 
  RSI_SLAVE8_START_NOTIFICATION = 22,
  RSI_SLAVE8_STOP_NOTIFICATION  = 23
}rsi_app_cmd_t;

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint16_t rsi_app_no_of_adv_reports_rcvd = 0;
static uint16_t rsi_scan_in_progress;
//static uint16_t rsi_ble_att1_val_hndl;
//static uint16_t rsi_ble_att2_val_hndl;

static rsi_ble_event_adv_report_t   rsi_app_adv_reports_to_app[NO_OF_ADV_REPORTS];
static rsi_ble_event_conn_status_t  conn_event_to_app[10];
static rsi_ble_event_disconnect_t   disconn_event_to_app;


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
static void rsi_ble_add_char_serv_att (void *serv_handler,
    uint16_t handle,
    uint8_t val_prop,
    uint16_t att_val_handle,
    uuid_t att_val_uuid)
{
  rsi_ble_req_add_att_t  new_att = {0};

  //! preparing the attribute service structure
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  new_att.att_uuid.size = 2;
  new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
  new_att.property = RSI_BLE_ATT_PROPERTY_READ;

  //! preparing the characteristic attribute value
  new_att.data_len = 6;
  new_att.data[0] = val_prop;
  rsi_uint16_to_2bytes (&new_att.data[2], att_val_handle);
  rsi_uint16_to_2bytes (&new_att.data[4], att_val_uuid.val.val16);

  //! add attribute to the service
  rsi_ble_add_attribute (&new_att);

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

static void rsi_ble_add_char_val_att (void *serv_handler,
    uint16_t handle,
    uuid_t   att_type_uuid,
    uint8_t  val_prop,
    uint8_t *data,
    uint8_t  data_len)
{
  rsi_ble_req_add_att_t  new_att = {0};

  //! preparing the attributes
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  memcpy (&new_att.att_uuid, &att_type_uuid, sizeof (uuid_t));
  new_att.property = val_prop;

  //! preparing the attribute value
  new_att.data_len = RSI_MIN(sizeof (new_att.data), data_len);
  memcpy (new_att.data, data, new_att.data_len);

  //! add attribute to the service
  rsi_ble_add_attribute (&new_att);

  //! check the attribute property with notification
  if (val_prop & RSI_BLE_ATT_PROPERTY_NOTIFY)
  {
    //! if notification property supports then we need to add client characteristic service.

    //! preparing the client characteristic attribute & values
    memset (&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler = serv_handler;
    new_att.handle = handle + 1;
    new_att.att_uuid.size = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
    new_att.property = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE;
    new_att.data_len = 2;

    //! add attribute to the service
    rsi_ble_add_attribute (&new_att);
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
static uint32_t rsi_ble_add_simple_chat_serv (void)
{
  uuid_t  new_uuid = {0};
  rsi_ble_resp_add_serv_t  new_serv_resp = {0};
  uint8_t   data[20] = {"redpine_sampletest_1"};

  //! adding new service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_NEW_SERVICE_UUID;
  rsi_ble_add_service (new_uuid, &new_serv_resp);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 1,
      RSI_BLE_ATT_PROPERTY_WRITE,
      new_serv_resp.start_handle + 2,
      new_uuid);

  //! adding characteristic value attribute to the service
//  rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_1_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 2,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_WRITE,
      data,
      sizeof (data));

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 3,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
      new_serv_resp.start_handle + 4,
      new_uuid);

  //! adding characteristic value attribute to the service
//  rsi_ble_att2_val_hndl = new_serv_resp.start_handle + 4; //commented to remove warning while compiling
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_ATTRIBUTE_2_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 4,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
      data,
      sizeof (data));
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
  ble_app_event_mask = 0xFFFFFFFF;
	ble_app_event_mask = ble_app_event_mask ; //To suppress warning while compiling 

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
  uint32_t  ix;

  for (ix = 0; ix < 32; ix++)
  {
    if (ble_app_event_map & (1 << ix))
    {
      return ix;
    }
  }

  return (-1);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_adv_report_event
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
void rsi_ble_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report)
{
  uint8_t  str_adv_addr[18] = {0};

  memcpy(&rsi_app_adv_reports_to_app[(rsi_app_no_of_adv_reports_rcvd)% (NO_OF_ADV_REPORTS)], adv_report, sizeof(rsi_ble_event_adv_report_t));
  rsi_app_no_of_adv_reports_rcvd++;
  if(rsi_app_no_of_adv_reports_rcvd == NO_OF_ADV_REPORTS)
  {
    rsi_app_no_of_adv_reports_rcvd = 0;
    LOG_PRINT ("restart scan\r\n");
    rsi_ble_app_set_event (RSI_BLE_SCAN_RESTART_EVENT);
  }
  rsi_6byte_dev_address_to_ascii ((uint8_t *)str_adv_addr, (uint8_t *)adv_report->dev_addr);	
  LOG_PRINT ("\n DEV_ADDR: %s\r\n", str_adv_addr);
  if ((!strcmp (RSI_BLE_DEV_1_ADDR, (char *)str_adv_addr)) || 
      (!strcmp (RSI_BLE_DEV_2_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_3_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_4_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_5_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_6_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_7_ADDR, (char *)str_adv_addr)) ||
      (!strcmp (RSI_BLE_DEV_8_ADDR, (char *)str_adv_addr))) {
    memcpy (conn_dev_addr, str_adv_addr, sizeof (str_adv_addr));
    rsi_ble_app_set_event (RSI_BLE_ADV_REPORT_EVENT);
  }
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

  uint8_t str_conn_device[18] = {0};

  rsi_6byte_dev_address_to_ascii ((uint8_t *)str_conn_device, (uint8_t *)resp_enh_conn->dev_addr);	
  if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR)) 
  {
    conn_event_to_app[0].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[0].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[0].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_2_ADDR)) 
  {
    conn_event_to_app[1].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[1].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[1].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_3_ADDR))
  {
    conn_event_to_app[2].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[2].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[2].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_4_ADDR))
  {
    conn_event_to_app[3].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[3].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[3].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_5_ADDR))
  {
    conn_event_to_app[4].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[4].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[4].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_6_ADDR))
  {
    conn_event_to_app[5].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[5].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[5].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_7_ADDR))
  {
    conn_event_to_app[6].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[6].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[6].status = resp_enh_conn->status;
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_8_ADDR))
  {
    conn_event_to_app[7].dev_addr_type = resp_enh_conn->dev_addr_type;
    memcpy (conn_event_to_app[7].dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
    conn_event_to_app[7].status = resp_enh_conn->status;
  }
  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
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
  uint8_t str_conn_device[18] = {0};

  rsi_6byte_dev_address_to_ascii ((uint8_t *)str_conn_device, (uint8_t *)resp_conn->dev_addr);	
  if(!strcmp((char *)str_conn_device, RSI_BLE_DEV_1_ADDR)) 
  {
    memcpy(&conn_event_to_app[0], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_2_ADDR)) 
  {
    memcpy(&conn_event_to_app[1], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_3_ADDR))
  {
    memcpy(&conn_event_to_app[2], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_4_ADDR))
  {
    memcpy(&conn_event_to_app[3], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_5_ADDR))
  {
    memcpy(&conn_event_to_app[4], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_6_ADDR))
  {
    memcpy(&conn_event_to_app[5], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_7_ADDR))
  {
    memcpy(&conn_event_to_app[6], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_conn_device, RSI_BLE_DEV_8_ADDR))
  {
    memcpy(&conn_event_to_app[7], resp_conn, sizeof(rsi_ble_event_conn_status_t));
  }
  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
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
  uint8_t str_disconn_device[18] = {0};

  rsi_6byte_dev_address_to_ascii ((uint8_t *)str_disconn_device, (uint8_t *)resp_disconnect->dev_addr);	
  if(!strcmp((char *)str_disconn_device, RSI_BLE_DEV_1_ADDR)) 
  {
    memset(&conn_event_to_app[0], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_2_ADDR)) 
  {
    memset(&conn_event_to_app[1], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_3_ADDR))
  {
    memset(&conn_event_to_app[2], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_4_ADDR))
  {
    memset(&conn_event_to_app[3], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_5_ADDR))
  {
    memset(&conn_event_to_app[4], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_6_ADDR))
  {
    memset(&conn_event_to_app[5], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_7_ADDR))
  {
    memset(&conn_event_to_app[6], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  else if (!strcmp((char *)str_disconn_device, RSI_BLE_DEV_8_ADDR))
  {
    memset(&conn_event_to_app[7], 0, sizeof(rsi_ble_event_conn_status_t));
  }
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
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
static void rsi_ble_on_gatt_write_event (uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  //! send BLE data to wlan
  rsi_6byte_dev_address_to_ascii ((uint8_t *)remote_dev_addr, (uint8_t *)rsi_ble_write->dev_addr);	
  LOG_PRINT("\n DEV_ADDR: %s\r\n", remote_dev_addr);
  if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_1_ADDR))  
  {
    rsi_ble_app_send_to_wlan(RSI_SLAVE1_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
  }
  else if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_2_ADDR)) 
  {
      rsi_ble_app_send_to_wlan(RSI_SLAVE2_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
  }
  else if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_3_ADDR)) 
  {
      rsi_ble_app_send_to_wlan(RSI_SLAVE3_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
  }
  else if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_4_ADDR)) 
  {
      rsi_ble_app_send_to_wlan(RSI_SLAVE4_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
  }
  else if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_5_ADDR)) 
  {
      rsi_ble_app_send_to_wlan(RSI_SLAVE5_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
  }
  else if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_6_ADDR)) 
  {
      rsi_ble_app_send_to_wlan(RSI_SLAVE6_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
  }
  else if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_7_ADDR)) 
  {
      rsi_ble_app_send_to_wlan(RSI_SLAVE7_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
  }
  else if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_8_ADDR)) 
  {
      rsi_ble_app_send_to_wlan(RSI_SLAVE8_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
  }
}

/*==============================================*/
/**
 * @fn         rsi_wlan_app_send_to_ble
 * @brief      this function is used to send data to ble app.
 * @param[in]   msg_type, it indicates write/notification event id.
 * @param[in]  data, raw data pointer.
 * @param[in]  data_len, raw data length.
 * @return     none.
 * @section description
 * This is a callback function
 */
void rsi_wlan_app_send_to_ble (uint16_t  msg_type, uint8_t *data, uint16_t data_len)
{
  if(msg_type == RSI_SLAVE1_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE1_START_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE2_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE2_START_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE3_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE3_START_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE4_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE4_START_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE5_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE5_START_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE6_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE6_START_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE7_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE7_START_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE8_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE8_START_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE1_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE1_STOP_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE2_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE2_STOP_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE3_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE3_STOP_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE4_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE4_STOP_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE5_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE5_STOP_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE6_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE6_STOP_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE7_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE7_STOP_NOTIFY);
  }
  else if (msg_type == RSI_SLAVE8_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE8_STOP_NOTIFY);
  }
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
int32_t rsi_ble_app_init (void)
{
  int32_t status = 0;
  
  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! adding simple BLE chat service
  rsi_ble_add_simple_chat_serv ();

  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(
      rsi_ble_on_adv_report_event,
      rsi_ble_on_connect_event,
      rsi_ble_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_enhance_conn_status_event,
      NULL,
      NULL,
      NULL);

  //! registering the GATT callback functions
  rsi_ble_gatt_register_callbacks (NULL,
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
      NULL,
      NULL);

  //! Set local name
  rsi_bt_set_local_name (RSI_BLE_APP_DEVICE_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

	LOG_PRINT("\n Start scanning \n");
	rsi_app_no_of_adv_reports_rcvd = 0;
	rsi_ble_start_scanning();
	rsi_scan_in_progress = 1;
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nBLE Start Scanning Failed, Error Code : 0x%X\r\n",status);
		return status;
	}

	return status;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_task
 * @brief      this function will execute when BLE events are raised.
 * @param[in]  none.
 * @return     none.
 * @section description
 */

int32_t rsi_ble_app_task (void)
{
  int32_t status = 0;
  int32_t  event_id;
  uint16_t  slave_val;

  //! checking for events list
  event_id = rsi_ble_app_get_event ();
  if (event_id == -1) {
    return -1;
  }

  switch (event_id) {
    case RSI_BLE_ADV_REPORT_EVENT:
      {
        LOG_PRINT("\n Advertise report received \n");
        if(conn_req_pending == 0)
        {
          if(num_of_conn_slaves < MAX_NUM_OF_SLAVES)
          {
            if((!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_1_ADDR)) || 
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_2_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_3_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_4_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_5_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_6_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_7_ADDR)) ||
               (!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_8_ADDR)))
            {
              LOG_PRINT("\n Device found. Stop scanning \n");
              status = rsi_ble_stop_scanning();
              rsi_scan_in_progress = 0;
              if (status != RSI_SUCCESS)
              {
                return status;
              }
              LOG_PRINT("\n Connect command \n");
              status = rsi_ble_connect(RSI_BLE_DEV_ADDR_TYPE, rsi_ascii_dev_address_to_6bytes_rev(remote_dev_bd_addr, conn_dev_addr));
              if(status != RSI_SUCCESS)
              {
                return status;
              }
              conn_req_pending = 1;
            }
          }
        }
        //! clear the advertise report event.
        rsi_ble_app_clear_event (RSI_BLE_ADV_REPORT_EVENT);
      }
      break;
    case RSI_BLE_CONN_EVENT:
      {
        num_of_conn_slaves++;

        LOG_PRINT("\n Device connected \n ");
        LOG_PRINT("\n Number of devices connected:%d \n", num_of_conn_slaves);

        conn_req_pending = 0;

        rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);

        if(num_of_conn_slaves < MAX_NUM_OF_SLAVES)
        {
          LOG_PRINT("\n Start scanning \n");
          rsi_app_no_of_adv_reports_rcvd = 0;
          status = rsi_ble_start_scanning();
          rsi_scan_in_progress = 1;
          if(status != RSI_SUCCESS)
          {
            return status;
          }
        }

#if BLE_PS_ENABLE
        else if(num_of_conn_slaves == MAX_NUM_OF_SLAVES)
        {
          LOG_PRINT("\n %d slaves connected \n", MAX_NUM_OF_SLAVES);
          LOG_PRINT("\n Power save Enable command \n");

          status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
        }
#endif        
      }
      break;

    case RSI_BLE_DISCONN_EVENT:
      {
        num_of_conn_slaves--;

        LOG_PRINT("\n Device disconnected\n ");
        LOG_PRINT("\n Number of connected devices:%d\n", num_of_conn_slaves);

#if BLE_PS_ENABLE
        LOG_PRINT("\n Power save Enable command \n");

        status = rsi_bt_power_save_profile(RSI_ACTIVE, PSP_TYPE);
        if(status != RSI_SUCCESS)
        {
          return status;
        }
#endif
        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
        
scan: 
        if(!rsi_scan_in_progress) 
        {
          rsi_app_no_of_adv_reports_rcvd = 0;
          status = rsi_ble_start_scanning();
          rsi_scan_in_progress = 1;
          if((status != RSI_SUCCESS) )
          {
            goto scan;
          }
        }
      }
      break;

    case RSI_BLE_SCAN_RESTART_EVENT:
      //! clear the served event
      rsi_ble_app_clear_event (RSI_BLE_SCAN_RESTART_EVENT);

      LOG_PRINT("\n Device found. Stop scanning \n");
      status = rsi_ble_stop_scanning();
      rsi_scan_in_progress = 0;
      if (status != RSI_SUCCESS)
      {
        return status;
      }
      
      LOG_PRINT("\n Start scanning \n");
      rsi_app_no_of_adv_reports_rcvd = 0;
      status = rsi_ble_start_scanning();
      rsi_scan_in_progress = 1;
      if(status != RSI_SUCCESS)
      {
        return status;
      }
      break;

      case RSI_BLE_SLAVE1_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_1_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE1_START_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE1_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_1_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE1_STOP_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE2_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_2_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE2_START_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE2_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_2_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE2_STOP_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE3_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_3_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE3_START_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE3_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_3_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE3_STOP_NOTIFY);
      }
      break;
      case RSI_BLE_SLAVE4_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_4_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE4_START_NOTIFY);
      }
      break;
      case RSI_BLE_SLAVE4_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_4_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE4_STOP_NOTIFY);
      }
      break;
      case RSI_BLE_SLAVE5_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_5_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE5_START_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE5_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_5_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE5_STOP_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE6_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_6_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE6_START_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE6_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_6_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE6_STOP_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE7_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_7_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE7_START_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE7_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_7_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE7_STOP_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE8_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_8_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE8_START_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE8_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (RSI_BLE_DEV_8_ADDR, 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE8_STOP_NOTIFY);
      }
      break;
  }

  return 0;
}
