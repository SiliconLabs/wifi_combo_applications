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
#include<string.h>
#include<stdio.h>
#include "rsi_driver.h"
//! Common include file 
#include<rsi_common_apis.h>

//! application defines 
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! BLE attribute service types uuid values
#define  RSI_BLE_CHAR_SERV_UUID                 0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID               0x2902

//! BLE characteristic service uuid
#define  RSI_BLE_IMMEDIATE_ALERT_SERVICE_UUID   0x1802
#define  RSI_BLE_LINK_LOSS_SERVICE_UUID         0x1803
#define  RSI_BLE_TX_POWER_SERVIEC_UUID          0x1804

#define  RSI_BLE_ATT_ALERT_LEVEL_UUID           0x2A06
#define  RSI_BLE_ATT_TX_POWER_UUID              0x2A07

//! max data length
//#define  RSI_BLE_MAX_DATA_LEN           20

//! local device name
#define RSI_BLE_APP_PROXIMITY_REPORTER         "BLE_PROXIMITY_REPORTER"

//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ              0x02
#define  RSI_BLE_ATT_WRITE_WITHOUT_RESP         0x04
#define  RSI_BLE_ATT_PROPERTY_WRITE             0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY            0x10

//! application event list
#define  RSI_BLE_CONN_EVENT                     0x01
#define  RSI_BLE_DISCONN_EVENT                  0x02
#define  RSI_BLE_LINK_LOSS_WRITE_EVENT          0x03
#define  RSI_BLE_IMME_ALT_WRITE_EVENT           0x04

//TX power limits
#define TX_MIN_VALUE                            0x5 
#define TX_MAX_VALUE                            0x12

//alert types
#define NO_ALERT                                0
#define MILD_ALERT                              1
#define HIGH_ALERT                              2


#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                  1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                    1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS                       1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE              3000

void rsi_wireless_driver_task(void);

#endif


//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t linkloss_event;
static rsi_ble_event_write_t imm_alt_event;
static uint16_t rsi_ble_link_loss_hndl;
static uint16_t rsi_ble_imme_alt_hndl;
static uint16_t rsi_ble_tx_power_hndl;

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
	memcpy ((void *)&new_att.att_uuid, (void *)&att_type_uuid, sizeof (uuid_t));
	new_att.property = val_prop;

	//! preparing the attribute value
	new_att.data_len = RSI_MIN(sizeof (new_att.data), data_len);
	memcpy (new_att.data, data, new_att.data_len);

	//! add attribute to the service
	rsi_ble_add_attribute (&new_att);

		return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_proximity_serv
 * @brief      this function is used to add new servcie i.e.., proximity service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */

static uint32_t rsi_ble_add_proximity_serv (void)
{
	uuid_t  new_uuid = {0};
	rsi_ble_resp_add_serv_t  new_serv_resp = {0};
  uint8_t    maxvalue = TX_MAX_VALUE;
	uint8_t    level = 0; 

	//! adding new service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_LINK_LOSS_SERVICE_UUID;
	rsi_ble_add_service (new_uuid, &new_serv_resp);

	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATT_ALERT_LEVEL_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
                             new_serv_resp.start_handle + 1,
                             RSI_BLE_ATT_PROPERTY_READ |RSI_BLE_ATT_PROPERTY_WRITE,
                             new_serv_resp.start_handle + 2,
                             new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_link_loss_hndl = new_serv_resp.start_handle + 2;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATT_ALERT_LEVEL_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
                            new_serv_resp.start_handle + 2,
                            new_uuid,
                            RSI_BLE_ATT_PROPERTY_READ |RSI_BLE_ATT_PROPERTY_WRITE,
                            &level,
                            sizeof (level));

        //! adding new service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_IMMEDIATE_ALERT_SERVICE_UUID;
	rsi_ble_add_service (new_uuid, &new_serv_resp);


	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATT_ALERT_LEVEL_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			                       new_serv_resp.start_handle + 3,
			                       RSI_BLE_ATT_WRITE_WITHOUT_RESP, 
			                       new_serv_resp.start_handle + 4,
			                       new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_imme_alt_hndl = new_serv_resp.start_handle + 4;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATT_ALERT_LEVEL_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
			                      new_serv_resp.start_handle + 4,
			                      new_uuid,
			                      RSI_BLE_ATT_WRITE_WITHOUT_RESP, 
			                      &level,
			                      sizeof (level));

        //! adding new service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_TX_POWER_SERVIEC_UUID;
	rsi_ble_add_service (new_uuid, &new_serv_resp);
 	//! adding characteristic service attribute to the service
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATT_TX_POWER_UUID;
	rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
			                       new_serv_resp.start_handle + 5,
			                       RSI_BLE_ATT_PROPERTY_READ ,
			                       new_serv_resp.start_handle + 6,
			                       new_uuid);

	//! adding characteristic value attribute to the service
	rsi_ble_tx_power_hndl = new_serv_resp.start_handle + 6;
	new_uuid.size = 2;
	new_uuid.val.val16 = RSI_BLE_ATT_TX_POWER_UUID;
	rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
			                      new_serv_resp.start_handle + 6,
			                      new_uuid,
			                      RSI_BLE_ATT_PROPERTY_READ ,
			                      &maxvalue,
			                      sizeof (maxvalue));


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
  uint8_t   ix;

  LOG_PRINT ("\r\nhandle: 0x%x, length: %d, data: ", *((uint16_t *)rsi_ble_write->handle), rsi_ble_write->length);
  for (ix = 0; ix < rsi_ble_write->length; ix++) {
      LOG_PRINT ("%x, ", rsi_ble_write->att_value[ix]);
  }
  LOG_PRINT ("\r\n");

	if (rsi_ble_link_loss_hndl == *((uint16_t *)rsi_ble_write->handle)) {
      memcpy (&linkloss_event, rsi_ble_write, sizeof (rsi_ble_event_write_t));
      rsi_ble_app_set_event (RSI_BLE_LINK_LOSS_WRITE_EVENT);
	} else if (rsi_ble_imme_alt_hndl == *((uint16_t *)rsi_ble_write->handle)){
      memcpy (&imm_alt_event, rsi_ble_write, sizeof (rsi_ble_event_write_t));
      rsi_ble_app_set_event (RSI_BLE_IMME_ALT_WRITE_EVENT);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_proximity_gatt_server
 * @brief      this is the application of proximity profile application.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This function is used to test the simple chat application.
 */
int32_t  rsi_ble_proximity_gatt_server(void)
{
	int32_t status = 0;
	int32_t  event_id;
//	int8_t   data[20] = {0};
	uint8_t adv[31] = {2,1,6};

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

	//! adding  BLE Proximity service
	
	rsi_ble_add_proximity_serv();

	//! registering the GAP callback functions
	rsi_ble_gap_register_callbacks(NULL,
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
	rsi_ble_gatt_register_callbacks (NULL,NULL,NULL,NULL,NULL,NULL,NULL,rsi_ble_on_gatt_write_event,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events ();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_APP_PROXIMITY_REPORTER);

	//! prepare advertise data //local/device name
  adv[3] = strlen (RSI_BLE_APP_PROXIMITY_REPORTER) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5],RSI_BLE_APP_PROXIMITY_REPORTER);

	//! set advertise data
	rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_APP_PROXIMITY_REPORTER) + 5);

	//! set device in advertising mode.
	rsi_ble_start_advertising ();

	//! waiting for events from controller.
	while (1) {

		//! Application main loop
#ifndef RSI_WITH_OS
		rsi_wireless_driver_task();
#endif
		//! checking for events list
		event_id = rsi_ble_app_get_event ();

		if (event_id == -1) {
			continue;
		}

		switch (event_id) {
			case RSI_BLE_CONN_EVENT:
			{
				//! event invokes when connection was completed

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);
			}
			break;

			case RSI_BLE_DISCONN_EVENT:
			{
				//! event invokes when disconnection was completed

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);

				//! set device in advertising mode.
adv:
				status = rsi_ble_start_advertising ();
				if (status != RSI_SUCCESS) {
					goto adv;
				}
			}
			break;

			case RSI_BLE_LINK_LOSS_WRITE_EVENT:
			{
				//! event invokes when write/notification events received

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_LINK_LOSS_WRITE_EVENT);

			}
			break;

      case RSI_BLE_IMME_ALT_WRITE_EVENT:
      {

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_IMME_ALT_WRITE_EVENT);
      }
      break;
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
  
  //! Call Proximity GATT Server Application
  rsi_ble_proximity_gatt_server();

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
  rsi_task_create( rsi_ble_proximity_gatt_server, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
