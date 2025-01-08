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
#include "rsi_driver.h"
#ifndef RSI_SAMPLE_HAL
#include <TriLED_gpio.h>
#include <timetick.h>
#endif
#include<string.h>

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
#define  RSI_BLE_CHAR_SERV_UUID         0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID       0x2902

//! immediate alert service uuid
#define  RSI_BLE_NEW_SERVICE_UUID       0x1802
//! Alert level characteristic
#define  RSI_BLE_ATTRIBUTE_1_UUID       0x2A06

//! rssi threshold value
#define  RSI_BLE_RSSI_THRESHOLD_VALUE   40

//! max data length
#define  RSI_BLE_MAX_DATA_LEN           20

//! max number of scan list
#define MAX_NUMBER_OF_SCAN_LIST         5

//! local device name
#define RSI_BLE_APP_NAME        "Immediate_Alert"

//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ                   0x02
#define  RSI_BLE_ATT_PROPERTY_WRITE                  0x08
#define  RSI_BLE_ATT_PROPERTY_NOTIFY                 0x10

//! application event list
#define  RSI_APP_EVENT_ADV_REPORT    0x00
#define  RSI_BLE_CONN_EVENT          0x02
#define  RSI_BLE_DISCONN_EVENT       0x03
#define  RSI_BLE_GATT_WRITE_EVENT    0x04
#define  RSI_BLE_GATT_PROFILE        0x05
#define  RSI_BLE_GATT_CHAR_SERVICES  0x06
#define  RSI_BLE_GET_RSSI            0x07

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR                          "00:23:A7:80:70:B9"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME					   "REDPINE_DEV"

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                      1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                        1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS                           1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                    2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                  3000

void rsi_wireless_driver_task(void);

#endif
//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static profile_descriptors_t  rsi_ble_service;
static rsi_ble_resp_char_services_t char_servs;
static uint16_t rsi_ble_att1_val_hndl;


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

  if (device_found == 1) {
    return;
  }

  memset(remote_name, 0, 31);
  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
  if(((device_found==0) && ((strcmp((const char *)remote_name, RSI_REMOTE_DEVICE_NAME))==0) )||
      ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp((const char *)remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  { 
    device_found=1;
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
	memcpy((void *)&disconn_event_to_app, (const void *)resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
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
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_profile
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] p_ble_resp_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_profile (uint16_t resp_status, profile_descriptors_t *rsi_ble_resp_profile)
{
    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
    return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_char_services
 * @brief      invoked when response is received for characteristic services details
 * @param[out] p_ble_resp_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_char_services (uint16_t resp_status, rsi_ble_resp_char_services_t *rsi_ble_resp_char_services)
{
	rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
	return;
}


/*==============================================*/
/**
 * @fn         rsi_ble_immediate_alert_client
 * @brief      this is the application of GATT client for immediate alert service.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used to test the immediate alert service client role.
 */
void rsi_ble_immediate_alert_client (void)
{
	int32_t status = 0;
	int32_t event_id;
	int8_t rssi;
	uint8_t  ix, data;
	uuid_t  service_uuid;
	uint32_t last_time, present_time;

#ifndef RSI_WITH_OS
	//! Driver initialization
	status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
	{
		return;
	}

  //! Redpine module intialisation 
  status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return;
	}
#endif

	//! WC initialization
	status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
	if(status != RSI_SUCCESS)
	{
		return;
	}

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
			rsi_ble_profile,
			rsi_ble_char_services,
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
      NULL
      );

	//!  initializing the application events map
	rsi_ble_app_init_events ();

	//! Set local name
	rsi_bt_set_local_name((uint8_t *)RSI_BLE_APP_NAME);

    //! start scanning
    status = rsi_ble_start_scanning();
    if(status != RSI_SUCCESS)
    {
      return;
    }
	
  //! waiting for events from controller.
  while (1) {

		//! Application main loop
#ifndef RSI_WITH_OS
		rsi_wireless_driver_task();
#endif
		//! checking for events list
		event_id = rsi_ble_app_get_event ();
		if (event_id == -1) {
			//! calculate the time difference with last_time
#ifndef RSI_SAMPLE_HAL
			present_time = rsi_hal_gettickcount();
#endif
			if ((rsi_ble_att1_val_hndl != 0) && ((present_time - last_time) > 1000)) {
				rsi_ble_app_set_event (RSI_BLE_GET_RSSI);
			}
			continue;
		}

    switch (event_id) {
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

			case RSI_BLE_CONN_EVENT:
			{
				//! event invokes when connection was completed

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);

				//! initiating the alert data and handle
				rsi_ble_att1_val_hndl = 0;
				data = 0;

				//! search the immediate alert service
				service_uuid.size = 2;
				service_uuid.val.val16 = RSI_BLE_NEW_SERVICE_UUID;
        rsi_ble_get_profile(remote_dev_addr, service_uuid, &rsi_ble_service);
			}
			break;

			case RSI_BLE_DISCONN_EVENT:
			{
				//! event invokes when disconnection was completed

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
               device_found=0;
               //! start scanning
               status = rsi_ble_start_scanning();
               if(status != RSI_SUCCESS)
               {
                 LOG_PRINT ("start_scanning status: 0x%X\r\n", status);
               }
		       //! reset the alert data and handle
			   rsi_ble_att1_val_hndl = 0;
			   data = 0;

			}
			break;

			case RSI_BLE_GATT_WRITE_EVENT:
			{
				//! event invokes when write/notification events received

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE_EVENT);
			}
			break;

			case RSI_BLE_GATT_PROFILE:
			{
				//! event invokes when get profile response received

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE);

				//! get characteristics of the immediate alert servcie
				rsi_ble_get_char_services(conn_event_to_app.dev_addr, *(uint16_t *)rsi_ble_service.start_handle, *(uint16_t *)rsi_ble_service.end_handle, &char_servs);
			}
			break;

			case RSI_BLE_GATT_CHAR_SERVICES:
			{
				//! event invokes when get characteristics of the service response received

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES);

				//! verifying the immediate alert characteristic
				for (ix = 0; ix < char_servs.num_of_services; ix++) {
					if (char_servs.char_services[ix].char_data.char_uuid.val.val16 == RSI_BLE_ATTRIBUTE_1_UUID) {
						rsi_ble_att1_val_hndl = char_servs.char_services[ix].char_data.char_handle;

						//! set the event to calculate RSSI value
						rsi_ble_app_set_event (RSI_BLE_GET_RSSI);
#ifndef RSI_SAMPLE_HAL
						last_time = rsi_hal_gettickcount();
#endif
						break;
					}
				}
			}
			break;

			case RSI_BLE_GET_RSSI:
			{
				//! event invokes when time is expired

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GET_RSSI);

				if (rsi_ble_att1_val_hndl != 0) {

					//! calculate the RSSI value
					rsi_bt_get_rssi ((uint8_t *)conn_event_to_app.dev_addr, (int8_t *)&rssi);

					if (rssi >= RSI_BLE_RSSI_THRESHOLD_VALUE ) {
						if (data == 0) {
							data = 1;
							//! set the middle alert at GATT server
							rsi_ble_set_att_cmd (conn_event_to_app.dev_addr, rsi_ble_att1_val_hndl, 1, &data);
						}
					} else {
						if (data == 1) {
							data = 0;
							//! set the low alert in GATT server
							rsi_ble_set_att_cmd (conn_event_to_app.dev_addr, rsi_ble_att1_val_hndl, 1, &data);
						}
					}
				}

				//! tacking tick count
#ifndef RSI_SAMPLE_HAL
				last_time = rsi_hal_gettickcount();
#endif
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
  
  //! Call Simple chat GATT Server Application
  rsi_ble_immediate_alert_client();

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
  rsi_task_create(rsi_ble_immediate_alert_client, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
