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

/**
 * Include files
 * */

//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>

//! Common include file 
#include<rsi_common_apis.h>

#include <string.h>
#include "rsi_driver.h"
//! application defines 
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                                      printf
#else 
#define LOG_PRINT
#endif

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Memory to initialize the driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! BLE attribute service types uuid values
#define  RSI_BLE_CHAR_SERV_UUID                        0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID                      0x2902
#define  RSI_BLE_CHAR_PRESENTATION_FORMATE_UUID        0x2904

//! BLE characteristic service uuid
#define  RSI_BLE_BATTERY_SERVICE_UUID                  0x180F
#define  RSI_BLE_BATTERY_LEVEL_UUID                    0x2A19

//! max data length 
#define  RSI_BLE_MAX_DATA_LEN                          1

//! local device name
#define RSI_LOCAL_DEVICE_NAME                        "BLE_BATTERY_SERVICE"

//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ                     0x02
#define  RSI_BLE_ATT_PROPERTY_WRITE                    0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY                   0x10

//! application event list
#define RSI_APP_EVENT_ADV_REPORT                       0x00
#define  RSI_BLE_CONN_EVENT                            0x01
#define  RSI_BLE_DISCONN_EVENT                         0x02
#define  RSI_BLE_GATT_PROFILE                          0x03
#define  RSI_BLE_GATT_CHAR_SERVICES                    0x04
#define  RSI_BLE_GATT_DESC_SERVICES                    0x05

#ifdef RSI_WITH_OS
//! BLE task stack size 
#define RSI_BT_TASK_STACK_SIZE                          1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                            1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                        2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                      3000

void rsi_wireless_driver_task(void);

#endif

#define SERVER                                          0
#define CLIENT                                          1

#define GATT_ROLE                                       SERVER

#if(GATT_ROLE == CLIENT)
//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE                           LE_PUBLIC_ADDRESS 

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR                                "00:23:A7:92:4D:8F"   

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME					   "REDPINE_DEV"

#endif

//! Characteristic Presenatation Format Fields
#define RSI_BLE_UINT8_FORMAT                            0x04
#define RSI_BLE_EXPONENT                                0x00
#define RSI_BLE_PERCENTAGE_UNITS_UUID                   0x27AD
#define RSI_BLE_NAME_SPACE                              0x01
#define RSI_BLE_DESCRIPTION                             0x010B

//!Characteristic Configuration Property value
#define RSI_BLE_NOTIFY_VALUE                            0x01
#define RSI_BLE_INDICATE_VALUE                          0X02

//! Notify status
#define NOTIFY_DISABLE  0x00
#define NOTIFY_ENABLE   0x01

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint16_t rsi_ble_battery_val_hndl;
static uint8_t notify_enabled;
static profile_descriptors_t  get_profile;
static rsi_ble_resp_char_services_t get_char_services;
static rsi_ble_resp_att_descs_t  get_desc_services;
#if(GATT_ROLE == CLIENT)
static uint8_t device_found=0;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
#endif


/*==============================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute..
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

	//! Add attribute to the service
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
 * @fn         rsi_ble_add_battery_service_serv
 * @brief      this function is used to add new servcie i.e.., battery service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */

static uint32_t rsi_ble_add_battery_service_serv (void)
{
  uuid_t  new_uuid = {0};
  rsi_ble_resp_add_serv_t  new_serv_resp = {0};
  uint8_t   data[1] = {90};
  rsi_ble_pesentation_format_t  presentation_format;
  uint8_t  format_data[7];

  //!adding descriptor fileds
  format_data[0] = presentation_format.format      = RSI_BLE_UINT8_FORMAT;
  format_data[1] = presentation_format.exponent    = RSI_BLE_EXPONENT;
  presentation_format.unit        = RSI_BLE_PERCENTAGE_UNITS_UUID;
  memcpy (&format_data[2], &presentation_format.unit, sizeof (presentation_format.unit));
  format_data[4] = presentation_format.name_space  = RSI_BLE_NAME_SPACE;
  presentation_format.description = RSI_BLE_DESCRIPTION;
  memcpy (&format_data[5], &presentation_format.description, sizeof (presentation_format.description));


  //! adding new service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_BATTERY_SERVICE_UUID;
  rsi_ble_add_service (new_uuid, &new_serv_resp);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_BATTERY_LEVEL_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 1,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
      new_serv_resp.start_handle + 2,
      new_uuid);
  //! adding characteristic value attribute to the service
  rsi_ble_battery_val_hndl = new_serv_resp.start_handle + 2;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_BATTERY_LEVEL_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 2,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_NOTIFY,
      data,
      sizeof (data));

  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_CHAR_PRESENTATION_FORMATE_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 4,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_READ ,
      format_data,
      sizeof(format_data)); 

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
 * @brief      set the specific event.
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

#if(GATT_ROLE == CLIENT)
  if (device_found == 1) {
    return;
  }

  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);

  if((device_found==0) && ((strcmp(remote_name, RSI_REMOTE_DEVICE_NAME))==0) ||
      ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  { 
    device_found=1;
    rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);
  }
#endif
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_peripheral_on_enhance_conn_status_event
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
	memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
	rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_get_profile_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] p_ble_resp_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_on_get_profile_event (uint16_t resp_status, profile_descriptors_t *rsi_ble_resp_profile)
{
    memcpy(&get_profile, rsi_ble_resp_profile, sizeof(profile_descriptors_t));
    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
    return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_char_services_event
 * @brief      invoked when response is received for characteristic services command
 * @param[out] p_ble_resp_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_on_char_services_event (uint16_t resp_status, rsi_ble_resp_char_services_t *rsi_ble_resp_char_services)
{
	memcpy(&get_char_services, rsi_ble_resp_char_services, sizeof(rsi_ble_resp_char_services_t));
	rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_att_desc_event
 * @brief      invoked when response is received for the characteristic descriptors command
 * @param[out] p_ble_resp_att_desc, list of descriptors.
 * @return     none
 * @section description
 */
static  void rsi_ble_on_att_desc_event (uint16_t resp_status, rsi_ble_resp_att_descs_t *rsi_ble_resp_att_desc)
{
	memcpy(&get_desc_services, rsi_ble_resp_att_desc, sizeof(rsi_ble_resp_att_descs_t));
	rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
	return;
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
	memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
	rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
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
static void rsi_ble_on_gatt_write_event (uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  memcpy (&app_ble_write_event, rsi_ble_write, sizeof (rsi_ble_event_write_t));
  if (rsi_ble_battery_val_hndl + 1 == *((uint16_t *)app_ble_write_event.handle)) {
    if(app_ble_write_event.att_value[0] == NOTIFY_ENABLE){
      notify_enabled = NOTIFY_ENABLE;
    }else if(app_ble_write_event.att_value[0] == NOTIFY_DISABLE){
      notify_enabled = NOTIFY_DISABLE;
    }
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_battery_service
 * @brief      this is the application of Battery Service application.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 *
 * This function is used to test the battery service application.
 */
int32_t rsi_ble_battery_service (void)
{
  int32_t status = 0;
  int32_t  event_id;
  int8_t   data[31] = {100} ;
  uint8_t adv[31] = {2,1,6},ix=0;
  uuid_t  query_serv = {0};

#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! Redpine module intialisation 
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if(GATT_ROLE == SERVER)
  //! adding BLE Battery Service service
  rsi_ble_add_battery_service_serv ();
#endif

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
      rsi_ble_on_get_profile_event,
      rsi_ble_on_char_services_event,
      NULL,
      rsi_ble_on_att_desc_event,
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

  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! Set local name
  rsi_bt_set_local_name(RSI_LOCAL_DEVICE_NAME);

#if(GATT_ROLE == SERVER)
  //! prepare advertise data local device name
  adv[3] = strlen (RSI_LOCAL_DEVICE_NAME) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5], RSI_LOCAL_DEVICE_NAME);

  //! set advertise data
  rsi_ble_set_advertise_data (adv, strlen (RSI_LOCAL_DEVICE_NAME) + 5);

  //! set device in advertising mode.
  rsi_ble_start_advertising ();
#endif

#if(GATT_ROLE == CLIENT)
  //! start scanning
  status = rsi_ble_start_scanning();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! waiting for events from controller.
  while (1) {

    //! Application main loop
#ifndef RSI_WITH_OS
    rsi_wireless_driver_task();
#endif
    //! checking for events list
    event_id = rsi_ble_app_get_event ();
    if (event_id == -1) {
#if(GATT_ROLE == SERVER)
      if(notify_enabled){
#ifdef RSI_M4_INTERFACE
        rsi_delay_ms(5000);
#endif
#ifdef __linux__
        sleep(5);
#endif
        if(data[0] > 0){

          data[0] = data[0] - 1;

        }else{

          data[0] = 100;

        }
        rsi_ble_set_local_att_value (rsi_ble_battery_val_hndl, RSI_BLE_MAX_DATA_LEN, (uint8_t *)data);
      }
#endif
      continue;
    }

    switch (event_id) {
#if(GATT_ROLE == CLIENT)
      case RSI_APP_EVENT_ADV_REPORT:
        {
          //! advertise report event.
          //! clear the advertise report event.
          rsi_ble_app_clear_event (RSI_APP_EVENT_ADV_REPORT);

          status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_bd_addr);
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("connect status: 0x%X\r\n", status);
          }       
        }
        break;
#endif
      case RSI_BLE_CONN_EVENT:
        {
          //! event invokes when connection was completed
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);
#if(GATT_ROLE == CLIENT)
          //rsi_6byte_dev_address_to_ascii (remote_dev_addr, conn_event_to_app.dev_addr);
          query_serv.size=2;
          query_serv.val.val16=RSI_BLE_BATTERY_SERVICE_UUID;
          //!Get profile start handle and end handle by service uuid
          rsi_ble_get_profile (conn_event_to_app.dev_addr, query_serv, &get_profile);
#endif
        }
        break;

      case RSI_BLE_DISCONN_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);

          //! set device in advertising mode.
#if(GATT_ROLE == SERVER)
adv:
          status = rsi_ble_start_advertising ();
          if (status != RSI_SUCCESS) {
            goto adv;
          }
#else
          //! start scanning
          device_found=0;
          status = rsi_ble_start_scanning();
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("start_scanning status: 0x%X\r\n", status);
          }
#endif
        }
        break;
#if(GATT_ROLE == CLIENT)
      case RSI_BLE_GATT_PROFILE:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE);
          //rsi_6byte_dev_address_to_ascii (remote_dev_addr, conn_event_to_app.dev_addr);
          LOG_PRINT("\n\nGatt profile:\nStart handle: 0x%x  \nEnd handle:0x%x \nUuid:0x%x \n\n",
              *((uint16_t *)get_profile.start_handle),
              *((uint16_t *)get_profile.end_handle),
              get_profile.profile_uuid.val.val16 );
          //! Get characteristic services within battery service profile
          rsi_ble_get_char_services (conn_event_to_app.dev_addr, *((uint16_t *)get_profile.start_handle), *((uint16_t *)get_profile.end_handle), &get_char_services);
        }
        break;
      case RSI_BLE_GATT_CHAR_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES);
          //rsi_6byte_dev_address_to_ascii (remote_dev_addr, conn_event_to_app.dev_addr);
          for(ix=0;ix<get_char_services.num_of_services;ix++){ 

            LOG_PRINT("\n\nCharecteristic services:\nChar_serv_handle:0x%x \nProperty:0x%x \nChar_value_handle:0x%x \nUuid:0x%x \n\n",
                (get_char_services.char_services[ix].handle),
                (get_char_services.char_services[ix].char_data.char_property),
                (get_char_services.char_services[ix].char_data.char_handle),
                get_char_services.char_services[ix].char_data.char_uuid.val.val16);

            if(ix<(get_char_services.num_of_services - 1)){
              //!Get descriptors from next handle of characteristic definition to previous handle of the next characteristic service 
              rsi_ble_get_att_descriptors(conn_event_to_app.dev_addr,
                  (get_char_services.char_services[ix].char_data.char_handle + 1), 
                  get_char_services.char_services[ix+1].handle - 1,   
                  &get_desc_services);
            }
            else{ 
              //! Get descriptors from next handle of characteristic definition to profile end handle because it has only one service
              rsi_ble_get_att_descriptors(conn_event_to_app.dev_addr,
                  (get_char_services.char_services[ix].char_data.char_handle + 1), 
                  *((uint16_t *)get_profile.end_handle),  
                  &get_desc_services);
            }
          }
          LOG_PRINT("\n");
        }
        break;
      case RSI_BLE_GATT_DESC_SERVICES:
        {
          rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_SERVICES);
          //rsi_6byte_dev_address_to_ascii (remote_dev_addr, conn_event_to_app.dev_addr);
          LOG_PRINT("\n\nDescriptors:");
          LOG_PRINT("\nNo.of attributes: %d \n",get_desc_services.num_of_att);
          for(ix=0;ix<get_desc_services.num_of_att;ix++){
            LOG_PRINT("\nHandle:0x%x  Uuid:0x%x \n",
                *((uint16_t *)get_desc_services.att_desc[ix].handle),
                get_desc_services.att_desc[ix].att_type_uuid.val.val16);

            if(get_desc_services.att_desc[ix].att_type_uuid.val.val16 == RSI_BLE_CLIENT_CHAR_UUID)
            {
                data[1]=0x0;
              if(get_char_services.char_services[0].char_data.char_property & RSI_BLE_ATT_PROPERTY_NOTIFY){
                data[0]=RSI_BLE_NOTIFY_VALUE;
              }
              else{
                data[0]=RSI_BLE_INDICATE_VALUE;
              }
              rsi_ble_set_att_value(conn_event_to_app.dev_addr,*((uint16_t *)get_desc_services.att_desc[ix].handle),2,data); 
            }
          }	
        }
        break;
#endif
    }
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
void main_loop (void)
{
	while (1) {
		rsi_wireless_driver_task ();
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
int main (void)
{
#ifdef RSI_WITH_OS
  int32_t status;
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif



#ifndef RSI_WITH_OS

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);
  
  //! Call Battery Service GATT Server Application
  rsi_ble_battery_service ();

  //! Application main loop
  main_loop();

  return 0;
#endif
#ifdef RSI_WITH_OS
 //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! Redpine module intialisation
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_ble_simple_chat_gatt_server, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
