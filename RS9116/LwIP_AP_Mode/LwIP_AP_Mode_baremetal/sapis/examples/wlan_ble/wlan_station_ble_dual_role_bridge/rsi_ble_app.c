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
 * @brief : This file contains example application for WLAN Station BLE Dual Role
 * Bridge
 * @section Description :
 * This Application explains user how to Create chat service, Configure device in
 * advertise mode, Connect from Smart phone, Configure device in master mode,
 * Connect to remote device, Receive data sent by Smart phone and forward to WLAN
 * task, Send data received by WLAN task and send to Smart phone
 =================================================================================*/

/**
 * Include files
 * */


//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_common_apis.h>
#include<rsi_bt_common.h>
#include<stdio.h>
#include<string.h>

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
#define RSI_BLE_DEV_ADDR      "B4:99:4C:64:BC:AF"  

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Master connection first
#define BLE_DUAL_ROLE_FIRST_MASTER 1

//! Maximum number of advertise reports to hold
#define NO_OF_ADV_REPORTS              5


//! BLE attribute service types uuid values
#define  RSI_BLE_CHAR_SERV_UUID         0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID       0x2902

//! BLE characteristic service uuid
#define  RSI_BLE_NEW_SERVICE_UUID       0xAABB
#define  RSI_BLE_ATTRIBUTE_1_UUID       0x1AA1
#define  RSI_BLE_ATTRIBUTE_2_UUID       0x1BB1

//! max data length
#define  RSI_BLE_MAX_DATA_LEN           20

//! local device name
#define RSI_BLE_APP_DEVICE_NAME        "CO-EX_DUAL_ROLE"

//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ      0x02
#define  RSI_BLE_ATT_PROPERTY_WRITE     0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY    0x10

//! application event list
#define  RSI_APP_EVENT_ADV_REPORT       0x00
#define  RSI_BLE_CONN_EVENT             0x01
#define  RSI_BLE_DISCONN_EVENT          0x02
#define  RSI_BLE_SCAN_RESTART_EVENT     0x03
#define  RSI_BLE_GATT_WRITE_EVENT       0x04
#define  RSI_BLE_SLAVE_START_NOTIFY     0x05
#define  RSI_BLE_SLAVE_STOP_NOTIFY      0x06

uint8_t remote_dev_addr[18] = {0};
uint8_t conn_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
uint8_t device_found = 0;

uint16_t le_master_connected;
uint16_t le_slave_connected;

//! Enumeration for commands used in application
typedef enum rsi_app_cmd_e
{
  RSI_SLAVE_DATA         = 0,
  RSI_MASTER_DATA        = 1,
  RSI_CHAT               = 2,
  RSI_START_NOTIFICATION = 3,
  RSI_STOP_NOTIFICATION  = 4
}rsi_app_cmd_t;

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint16_t rsi_app_no_of_adv_reports_rcvd = 0;
static uint8_t  rsi_ble_app_data[100];
static uint16_t rsi_ble_app_data_len;
static uint16_t rsi_ble_att1_val_hndl;
static uint16_t rsi_ble_att2_val_hndl;

static rsi_ble_event_adv_report_t   rsi_app_adv_reports_to_app[NO_OF_ADV_REPORTS];
static rsi_ble_event_conn_status_t  conn_event_to_app;
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
  rsi_ble_att1_val_hndl = new_serv_resp.start_handle + 2;
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
  rsi_ble_att2_val_hndl = new_serv_resp.start_handle + 4;
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
  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  memcpy(&rsi_app_adv_reports_to_app[(rsi_app_no_of_adv_reports_rcvd)% (NO_OF_ADV_REPORTS)], adv_report, sizeof(rsi_ble_event_adv_report_t));
  rsi_app_no_of_adv_reports_rcvd++;
  if(rsi_app_no_of_adv_reports_rcvd == NO_OF_ADV_REPORTS)
  {
    rsi_ble_app_set_event (RSI_BLE_SCAN_RESTART_EVENT);
    rsi_app_no_of_adv_reports_rcvd = 0;
  }
  rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, (uint8_t *)adv_report->dev_addr);	
  LOG_PRINT("\n DEV_ADDR: %s\r\n", remote_dev_addr);

  if(((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE) && ((strcmp(remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  {
    device_found=1;
    memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
		rsi_6byte_dev_address_to_ascii ((int8_t *)conn_dev_addr, (uint8_t *)adv_report->dev_addr);	
    rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);
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
	conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  conn_event_to_app.status = resp_enh_conn->status;
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
  rsi_6byte_dev_address_to_ascii ((uint8_t *)remote_dev_addr, (uint8_t *)resp_conn->dev_addr);	
  memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
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
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  rsi_6byte_dev_address_to_ascii ((uint8_t *)remote_dev_addr, (uint8_t *)resp_disconnect->dev_addr);	
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
  if (rsi_ble_att1_val_hndl == *((uint16_t *)rsi_ble_write->handle)) {
    if((!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_ADDR))) 
    {
      rsi_ble_app_send_to_wlan(RSI_SLAVE_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
    }
    else 
    {
      rsi_ble_app_send_to_wlan(RSI_MASTER_DATA, rsi_ble_write->att_value, rsi_ble_write->length);
    }
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
  if(msg_type == RSI_CHAT)
  {
    rsi_ble_app_data_len = RSI_MIN (sizeof (rsi_ble_app_data), data_len);
    memcpy (&rsi_ble_app_data, data, rsi_ble_app_data_len);
    rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
  }
  else if(msg_type == RSI_START_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE_START_NOTIFY);
  }
  else if (msg_type == RSI_STOP_NOTIFICATION)
  {
    rsi_ble_app_set_event (RSI_BLE_SLAVE_STOP_NOTIFY);
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
uint32_t rsi_ble_app_init (void)
{
  uint8_t adv[31] = {2,1,6};
  uint8_t loal_addr[6] = {0};
  uint8_t str_loal_addr[18] = {0};
  int32_t status = 0;

  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! adding simple BLE chat service
  rsi_ble_add_simple_chat_serv ();

  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(
      rsi_ble_simple_central_on_adv_report_event,
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
  rsi_bt_set_local_name ((uint8_t *)RSI_BLE_APP_DEVICE_NAME);

  //get local bdaddress
  rsi_bt_get_local_device_address (loal_addr);
  rsi_6byte_dev_address_to_ascii ((uint8_t *)str_loal_addr, (uint8_t *)loal_addr);	
  LOG_PRINT ("loal_addr: %s\r\n", str_loal_addr);

  //! prepare advertise data - local device name
  adv[3] = strlen (RSI_BLE_APP_DEVICE_NAME) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5], RSI_BLE_APP_DEVICE_NAME);
  //! set advertise data
  rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_APP_DEVICE_NAME) + 5);

#if BLE_DUAL_ROLE_FIRST_MASTER
  //! start scanning
  rsi_app_no_of_adv_reports_rcvd = 0;
  status = rsi_ble_start_scanning();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#else
  //! set device in advertising mode.
  status = rsi_ble_start_advertising ();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif  
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

    case RSI_BLE_CONN_EVENT:
      {
        //! event invokes when connection was completed

        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);

        if(!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_ADDR)) {
          LOG_PRINT("\n We got connected as Master\n");
          le_slave_connected = 1;
					device_found =0;
        } else {
          LOG_PRINT("\n We got connected as Slave\n");
          le_master_connected = 1;
		}
		
        if (!le_slave_connected)
        {
scan1:
        	      LOG_PRINT("\n Scanning \n");
        	      rsi_app_no_of_adv_reports_rcvd = 0;
        	      status = rsi_ble_start_scanning();
        	      if ((status != RSI_SUCCESS) && (status == 0x4E11))
        	            {
        	                 goto scan1;
        	            }

        }
        if (!le_master_connected)
        {
          LOG_PRINT("\n Advertising \n");
          
          status = rsi_ble_start_advertising ();
          if(status != RSI_SUCCESS)
          {
            return status;
          }

        }		

        if(!strcmp((char *)conn_dev_addr, RSI_BLE_DEV_ADDR))
        {
          rsi_ble_app_set_event (RSI_BLE_SLAVE_START_NOTIFY);
        }

		if ((le_slave_connected == 1) && (le_master_connected == 1)) {

#if BLE_PS_ENABLE          
          LOG_PRINT("\n Power save Enable command \n");
          status = rsi_bt_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
#endif          
        }
      }
      break;

    case RSI_BLE_DISCONN_EVENT:
      {
        //! event invokes when disconnection was completed

        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
#if BLE_PS_ENABLE
        status = rsi_bt_power_save_profile(RSI_ACTIVE, PSP_TYPE);
          
        if(status != RSI_SUCCESS)
        {
          return status;
        }
#endif
        if (!strcmp((char *)remote_dev_addr,RSI_BLE_DEV_ADDR))
        {
          le_slave_connected = 0;
        } else {
          le_master_connected = 0;
        }

        if (le_slave_connected == 0) 
        {
scan:
          LOG_PRINT("\n Scanning \n");
          rsi_app_no_of_adv_reports_rcvd = 0;
					device_found=0;
          status = rsi_ble_start_scanning();
          if((status != RSI_SUCCESS) && (status == 0x4E0C))
          {
            goto scan;
          }
        } else {
          //! set device in advertising mode.
adv:
          LOG_PRINT("\n Advertising\n");
          status = rsi_ble_start_advertising ();
          if ((status != RSI_SUCCESS) && (status == 0x4E0C))
          {
            goto adv;
          }
        }
      }
      break;

    case RSI_BLE_GATT_WRITE_EVENT:
      {
        //! event invokes when write/notification events received

        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE_EVENT);

        //! set the local attribute value.
        rsi_ble_set_local_att_value (rsi_ble_att2_val_hndl, RSI_BLE_MAX_DATA_LEN, rsi_ble_app_data);
      }
      break;

      case RSI_APP_EVENT_ADV_REPORT:
      {
        //! advertise report event.
        if(!strcmp((char *)conn_dev_addr,RSI_BLE_DEV_ADDR))
        {
          LOG_PRINT("\n Stop Scanning \n");
          status = rsi_ble_stop_scanning();
          if (status != RSI_SUCCESS)
          {
            return status;
          }
    
          LOG_PRINT("\n Connect command \n");

          status = rsi_ble_connect(remote_addr_type, (uint8_t *)remote_dev_bd_addr);
          if(status != RSI_SUCCESS)
          {
            return status;
          }

        }
        else {

		}
        //! clear the advertise report event.
        rsi_ble_app_clear_event (RSI_APP_EVENT_ADV_REPORT);
      }
      break;
    
      case RSI_BLE_SCAN_RESTART_EVENT:
      //! clear the served event
      rsi_ble_app_clear_event (RSI_BLE_SCAN_RESTART_EVENT);

      LOG_PRINT("\n Device found. Stop scanning \n");
      status = rsi_ble_stop_scanning();
      if (status != RSI_SUCCESS)
      {
        return status;
      }
      
      LOG_PRINT("\n Start scanning \n");
      rsi_app_no_of_adv_reports_rcvd = 0;
      status = rsi_ble_start_scanning();
      if(status != RSI_SUCCESS)
      {
        return status;
      }
      break;

      case RSI_BLE_SLAVE_START_NOTIFY:
      {
        slave_val = 0x0001;
        rsi_ble_set_att_value_async (rsi_ascii_dev_address_to_6bytes_rev(remote_dev_bd_addr,RSI_BLE_DEV_ADDR), 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE_START_NOTIFY);
      }
      break;

      case RSI_BLE_SLAVE_STOP_NOTIFY:
      {
        slave_val = 0x0000;
        rsi_ble_set_att_value_async (rsi_ascii_dev_address_to_6bytes_rev(remote_dev_bd_addr,RSI_BLE_DEV_ADDR), 0x60, 2, (uint8_t *)&slave_val);
        rsi_ble_app_clear_event (RSI_BLE_SLAVE_STOP_NOTIFY);
      }
      break;
  }

  return 0;
}
