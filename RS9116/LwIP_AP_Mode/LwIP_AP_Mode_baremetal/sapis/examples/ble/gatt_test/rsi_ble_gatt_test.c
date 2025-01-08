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

//! BLE include files to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_ble.h>
#include<string.h>
#include<stdio.h>
//! Common include file 
#include<rsi_common_apis.h>

#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif
//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! maximum advertise list
#define  NO_OF_ADV_REPORTS  5

//! local device name
#define RSI_BLE_APP_GATT_TEST  "RSI_BLE_GATT_TEST"

//! application events list
#define  RSI_BLE_ADV_REPORT_EVENT     0x01
#define  RSI_BLE_CONN_EVENT           0x02
#define  RSI_BLE_DISCONN_EVENT        0x03
#define  RSI_BLE_GATT_WRITE_EVENT     0x04
#define  RSI_BLE_GATT_PROFILES        0x05
#define  RSI_BLE_GATT_PROFILE         0x06
#define  RSI_BLE_GATT_CHAR_SERVICES   0x07
#define  RSI_BLE_GATT_INC_SERVICES    0x08
#define  RSI_BLE_GATT_DESC_SERVICES   0x09
#define  RSI_BLE_GATT_READ            0x0A
#define  RSI_BLE_GATT_WRITE           0x0B
#define  RSI_BLE_GATT_ERROR           0x0C
#define  RSI_BLE_GATT_DESC_VAL        0x0D

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

//! application global variables list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
static app_num_scan_list;
static rsi_ble_event_adv_report_t app_scan_list[NO_OF_ADV_REPORTS];
static rsi_ble_event_conn_status_t app_conn_event;
static rsi_ble_event_disconnect_t app_disconn_event;
static rsi_ble_event_write_t app_ble_write_event;
static rsi_ble_resp_profiles_list_t  ble_profiles;
static rsi_ble_resp_query_profile_descriptor_t  ble_profile;
static rsi_ble_resp_char_services_t ble_char_services;
static rsi_ble_resp_inc_services_t  ble_inc_serv;
static rsi_ble_resp_att_descs_t  ble_desc_services;
static rsi_ble_resp_att_value_t  ble_read_val;


/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events(void)
{
	ble_app_event_map = 0;
	ble_app_event_mask = 0xFFFFFFFF;
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
 * @brief      clear the specific event.
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
static int32_t rsi_ble_app_get_event()
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
 * @fn         rsi_ble_on_adv_reports
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
static void rsi_ble_on_adv_reports (rsi_ble_event_adv_report_t *adv_report)
{
  if (device_found == 1) {
    return;
  }

  memset(remote_name, 0, 31);

  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
  if((device_found==0) && ((strcmp(remote_name, RSI_REMOTE_DEVICE_NAME))==0) ||
      ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp(remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  { 
    device_found=1;
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
	app_conn_event.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (app_conn_event.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  app_conn_event.status = resp_enh_conn->status;
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
	memcpy(&app_conn_event, resp_conn, sizeof(rsi_ble_event_conn_status_t));
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
	memcpy(&app_disconn_event, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
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
	memcpy (&app_ble_write_event, rsi_ble_write, sizeof (rsi_ble_event_write_t));
	rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_profiles_list_event
 * @brief      invoked when response is received for get list of services.
 * @param[out] p_ble_resp_profiles, profiles list details
 * @return     none
 * @section description
 */
static void rsi_ble_profiles_list_event (uint16_t resp_status, rsi_ble_event_profiles_list_t  *rsi_ble_event_profiles)
{
	rsi_ble_app_set_event (RSI_BLE_GATT_PROFILES);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_profile_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] rsi_ble_event_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_profile_event (uint16_t resp_status, rsi_ble_event_profile_by_uuid_t *rsi_ble_event_profile)
{
    LOG_PRINT("\n in rsi_ble_profile_event\n");
    rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE);
    return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_char_services_event
 * @brief      invoked when response is received for characteristic services details
 * @param[out] rsi_ble_event_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_char_services_event (uint16_t resp_status, rsi_ble_event_read_by_type1_t *rsi_ble_event_char_services)
{
	rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_inc_services_event
 * @brief      invoked when response is received for the include services details
 * @param[out] rsi_ble_event_inc_services, list of include  services.
 * @return     none
 * @section description
 */
static void rsi_ble_inc_services_event (uint16_t resp_status, rsi_ble_event_read_by_type2_t *rsi_ble_event_inc_services)
{
    rsi_ble_app_set_event (RSI_BLE_GATT_INC_SERVICES);
    return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_att_desc_event
 * @brief      invoked when response is received for the characteristic descriptors details
 * @param[out] rsi_ble_event_att_desc, list of descriptors.
 * @return     none
 * @section description
 */
static  void rsi_ble_on_att_desc_event (uint16_t resp_status, rsi_ble_event_read_by_type3_t *rsi_ble_event_att_desc)
{
	rsi_ble_app_set_event (RSI_BLE_GATT_DESC_SERVICES);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_read_event
 * @brief      invoked when response is received for get attribute value
 * @param[out] resp_id, received response id.
 * @param[out] rsi_ble_event_att_val, attribute value details
 * @return     none
 * @section description
 */
static void rsi_ble_on_read_event (uint16_t resp_status, uint16_t resp_id, rsi_ble_event_att_value_t *rsi_ble_event_att_val)
{
	rsi_ble_app_set_event (RSI_BLE_GATT_READ);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_write_event
 * @brief      this function will invoke when set the attribute value complete
 * @param[out] resp_id, response id
 * @param[out] status, status of the response
 * @return     none
 * @section description
 */
static void rsi_ble_write_event (uint16_t resp_status, rsi_ble_set_att_resp_t *rsi_ble_event_set_att_rsp)
{
  memcpy (remote_dev_bd_addr, rsi_ble_event_set_att_rsp->dev_addr, 6);
  rsi_ble_app_set_event (RSI_BLE_GATT_WRITE);
}
/*==============================================*/
/**
 * @fn         rsi_ble_gatt_error_event
 * @brief      this function will invoke when set the attribute value complete
 * @param[out] rsi_ble_gatt_error, event structure
 * @param[out] status, status of the response
 * @return     none
 * @section description
 */
static void rsi_ble_gatt_error_event (uint16_t resp_status, rsi_ble_event_error_resp_t *rsi_ble_gatt_error)
{
  memcpy (remote_dev_bd_addr, rsi_ble_gatt_error->dev_addr, 6);
  rsi_ble_app_set_event (RSI_BLE_GATT_ERROR);
}
/*==============================================*/
/**
 * @fn         rsi_ble_gatt_error_event
 * @brief      this function will invoke when set the attribute value complete
 * @param[out] rsi_ble_gatt_desc_val, event structure
 * @param[out] status, status of the response
 * @return     none
 * @section description
 */
static void rsi_ble_gatt_desc_val_event (uint16_t resp_status, rsi_ble_event_gatt_desc_t *rsi_ble_gatt_desc_val)
{
  memcpy (remote_dev_bd_addr, rsi_ble_gatt_desc_val->dev_addr, 6);
  rsi_ble_app_set_event (RSI_BLE_GATT_DESC_VAL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_gatt_test
 * @brief      this is the application of ble GATT client api's test cases.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used at application.
 */
int32_t rsi_ble_simple_gatt_test (void)
{
	int32_t status = 0;
	int32_t  event_id;
	int8_t   data[20] = {0};
	uint8_t  remote_dev_addr[18] = {0};
	uuid_t   search_serv;
	rsi_ble_resp_local_att_value_t  local_att_val;
	uint8_t loop;

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

	//! WC initialization
	status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! registering the GAP call back functions
	rsi_ble_gap_register_callbacks(
      rsi_ble_on_adv_reports,
      rsi_ble_on_connect_event,
      rsi_ble_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_enhance_conn_status_event,
      NULL,
      NULL,
      NULL);

	//! registering the GATT call back functions
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
      rsi_ble_gatt_error_event,
      rsi_ble_gatt_desc_val_event,
      rsi_ble_profiles_list_event,
      rsi_ble_profile_event,
      rsi_ble_char_services_event,
      rsi_ble_inc_services_event,
      rsi_ble_on_att_desc_event,
      rsi_ble_on_read_event,
      rsi_ble_write_event,
      NULL,
      NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events ();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_APP_GATT_TEST);


	//! set device in advertising mode.
    status = rsi_ble_start_advertising ();
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
			continue;
		}

		switch (event_id) {
		    case RSI_BLE_ADV_REPORT_EVENT:
		    {
          //! advertise report event.
          //! clear the advertise report event.
          rsi_ble_app_clear_event (RSI_BLE_ADV_REPORT_EVENT);
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
				//rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
//				rsi_ble_get_profiles_async (app_conn_event.dev_addr, 1, 0xffff, NULL);
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

			case RSI_BLE_GATT_WRITE_EVENT:
			{
				//! event invokes when write/notification events received

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE_EVENT);
#if 1
          status = rsi_ble_indicate_confirm (app_conn_event.dev_addr);
          if (status != RSI_SUCCESS)
          {
            LOG_PRINT ("\n indication confirm failed \t reason = %x \n", status);
          }
#endif
			}
			break;

			case RSI_BLE_GATT_PROFILES:
			{
				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILES);
        LOG_PRINT("\nin gatt test:rsi_ble_gatt_profiles\n");
				//rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
        memset(&search_serv,0,sizeof(search_serv));
				search_serv.size=0x02;
				search_serv.val.val16=0x1800;
				rsi_ble_get_profile_async (app_conn_event.dev_addr, search_serv, NULL);
				//rsi_ble_get_char_services_async(remote_dev_addr, 1, 0x10, NULL);
			}
			break;

			case RSI_BLE_GATT_PROFILE:
			{
				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE);

				//rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
				rsi_ble_get_char_services_async(app_conn_event.dev_addr, 1, 0xFFFF, NULL);
			}
			break;

			case RSI_BLE_GATT_CHAR_SERVICES:
			{
				rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES);
				//rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
				rsi_ble_get_inc_services_async(app_conn_event.dev_addr, 1, 0x10, NULL);

			}
			break;

			case RSI_BLE_GATT_INC_SERVICES:
			{
				rsi_ble_app_clear_event (RSI_BLE_GATT_INC_SERVICES);
				//rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
				rsi_ble_get_att_descriptors_async(app_conn_event.dev_addr, 1, 0xFFFF, NULL);

			}
			break;

			case RSI_BLE_GATT_DESC_SERVICES:
			{
				rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_SERVICES);
				//rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
				rsi_ble_get_att_value_async(app_conn_event.dev_addr, 3, NULL);
				loop = 0;
			}
			break;

			case RSI_BLE_GATT_READ:
			{
				rsi_ble_app_clear_event (RSI_BLE_GATT_READ);
				//rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, app_conn_event.dev_addr);
				if (loop == 0)
				{
					search_serv.size = 2;
					search_serv.val.val16 = 0x2A00;
					rsi_ble_get_char_value_by_uuid_async(app_conn_event.dev_addr, 0x01, 0x05, search_serv, NULL);
				} else if (loop == 1) {
					uint16_t handles[2];

					handles[0] = 0x03;
					handles[1] = 0x05;
					rsi_ble_get_multiple_att_values_async(app_conn_event.dev_addr, 2, handles, NULL);
				} else if (loop == 2) {
					rsi_ble_get_long_att_value_async(app_conn_event.dev_addr, 3, 0, NULL);
				} else {
					rsi_ble_set_att_value_async (app_conn_event.dev_addr, 3, 12, "mallikarjuna");
				}
				loop++;
			}
			break;

			case RSI_BLE_GATT_WRITE:
			{
				rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE);
				rsi_ble_set_att_cmd (remote_dev_bd_addr, 3, 12, "mallikarjuna");
			}
			break;
         	case RSI_BLE_GATT_ERROR:
			{
				rsi_ble_app_clear_event (RSI_BLE_GATT_ERROR);
                 scanf("loop is %d:",&loop);
				//rsi_ble_set_att_cmd (remote_dev_addr, 3, 12, "mallikarjuna");
        LOG_PRINT("get att descriptor value");
				rsi_ble_get_att_descriptors_async(remote_dev_bd_addr, 1, 0x10, NULL);
			}
			break;
        
            case RSI_BLE_GATT_DESC_VAL:
			{
				rsi_ble_app_clear_event (RSI_BLE_GATT_DESC_VAL);
        LOG_PRINT("\n get att value \n");
				rsi_ble_get_att_value_async(remote_dev_bd_addr, 3, NULL);
				loop = 0;
			}
			break;
		}
	}

	return 0;
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
  
  //! Call Simple  GATT  Application
  rsi_ble_simple_gatt_test();

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
  rsi_task_create(rsi_ble_simple_gatt_test, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
