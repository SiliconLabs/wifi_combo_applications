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
#include <string.h>
#include <stdio.h>

//! Common include file 
#include<rsi_common_apis.h>

#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE                      LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR                           "00:23:A7:80:70:B9"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME					   "REDPINE_DEV"

//! Memory length for the driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Memory to initialize the driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];


//! Application supported events list
#define RSI_APP_EVENT_ADV_REPORT                             0
#define RSI_APP_EVENT_CONNECTED                              1
#define RSI_APP_EVENT_DISCONNECTED                           2
#define RSI_APP_EVENT_CBFC_CONN_REQ                          3
#define RSI_APP_EVENT_CBFC_CONN_CMPL                         4
#define RSI_APP_EVENT_CBFC_RX_DATA                           5
#define RSI_APP_EVENT_CBFC_DISCONN                           6
#define RSI_BLE_PSM_VALUE                                    0x23

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                               1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                                 1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS                                    1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                             2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                           3000

void rsi_wireless_driver_task(void);

#endif

//! Application global parameters.
static uint8_t rsi_app_async_event_map = 0;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
static rsi_ble_event_conn_status_t     rsi_app_connected_device;
static rsi_ble_event_disconnect_t      rsi_app_disconnected_device;
//static rsi_ble_event_cbfc_conn_req_t   rsi_app_cbfc_conn_req;
static rsi_ble_event_cbfc_conn_complete_t rsi_app_cbfc_conn_cmpl;

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
	rsi_app_async_event_map = 0;
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
	rsi_app_async_event_map |= BIT(event_num);
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
	rsi_app_async_event_map &= ~BIT(event_num);
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
		if (rsi_app_async_event_map & (1 << ix))
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
 * @fn         rsi_ble_simple_peripheral_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_central_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  rsi_app_connected_device.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (rsi_app_connected_device.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  rsi_app_connected_device.status = resp_enh_conn->status;
  rsi_ble_app_set_event (RSI_APP_EVENT_CONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_central_on_conn_status_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_simple_central_on_conn_status_event(rsi_ble_event_conn_status_t *resp_conn)
{
	memcpy(&rsi_app_connected_device, resp_conn, sizeof(rsi_ble_event_conn_status_t));

	rsi_ble_app_set_event (RSI_APP_EVENT_CONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_central_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This Callback function indicates disconnected device information and status
 */
void rsi_ble_simple_central_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
	memcpy(&rsi_app_disconnected_device, resp_disconnect, sizeof(rsi_ble_event_disconnect_t ));

	rsi_ble_app_set_event (RSI_APP_EVENT_DISCONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_cbfc_conn_req_event
 * @brief      invoked when BLE L2CAP CBFC connect request event is received
 * @param[in]  rsi_ble_cbfc_conn_req, connection requested remote device information
 * @return     none.
 * @section description
 * This Callback function indicates BLE L2CAP CBFC connect request from remote device.
 */
void rsi_ble_on_cbfc_conn_req_event (rsi_ble_event_cbfc_conn_req_t *rsi_ble_cbfc_conn_req)
{
  uint8_t remote_dev_addr[18] = {0};

  memcpy (remote_dev_bd_addr, rsi_ble_cbfc_conn_req->dev_addr, 6);
  LOG_PRINT ( "remote_dev_addr: %s, lcid: 0x%x, %d\r\n" ,
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_ble_cbfc_conn_req->dev_addr), 
      rsi_ble_cbfc_conn_req->psm, rsi_ble_cbfc_conn_req->lcid, rsi_ble_cbfc_conn_req->lcid);

  //memcpy (&rsi_app_cbfc_conn_req, rsi_ble_cbfc_conn_req, sizeof (rsi_ble_event_cbfc_conn_req_t));
	rsi_ble_app_set_event (RSI_APP_EVENT_CBFC_CONN_REQ);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_cbfc_conn_complete_event
 * @brief      invoked when BLE L2CAP CBFC connect complete status event is received
 * @param[in]  rsi_ble_cbfc_conn_complete, connection completed information with remote device.
 * @param[in]  status, connection completed status with remote device.
 * @return     none.
 * @section description
 * This Callback function indicates BLE L2CAP CBFC connect complete status with remote devivce.
 */
void rsi_ble_on_cbfc_conn_complete_event (rsi_ble_event_cbfc_conn_complete_t *rsi_ble_cbfc_conn_complete, uint16_t status)
{
  uint8_t remote_dev_addr[18] = {0};

  memcpy (remote_dev_bd_addr, rsi_ble_cbfc_conn_complete->dev_addr, 6);
  LOG_PRINT ("remote_dev_addr: %s, psm: 0x%x, lcid: 0x%x, %d\r\n",
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_ble_cbfc_conn_complete->dev_addr), 
      rsi_ble_cbfc_conn_complete->psm, rsi_ble_cbfc_conn_complete->lcid, rsi_ble_cbfc_conn_complete->lcid);

  memcpy (&rsi_app_cbfc_conn_cmpl, rsi_ble_cbfc_conn_complete, sizeof (rsi_ble_event_cbfc_conn_complete_t));
	rsi_ble_app_set_event (RSI_APP_EVENT_CBFC_CONN_CMPL);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_cbfc_rx_data_event
 * @brief      invoked when BLE L2CAP CBFC data receive event is received
 * @param[in]  rsi_ble_cbfc_rx_data, CBFC receive data info from remote device.
 * @return     none.
 * @section description
 * This Callback function indicates BLE L2CAP CBFC data received from remote devivce.
 */
void rsi_ble_on_cbfc_rx_data_event (rsi_ble_event_cbfc_rx_data_t *rsi_ble_cbfc_rx_data)
{
  uint8_t ix;
  uint8_t remote_dev_addr[18] = {0};

  memcpy (remote_dev_bd_addr, rsi_ble_cbfc_rx_data->dev_addr, 6);
  LOG_PRINT ("remote_dev_addr: %s, lcid: 0x%x, %d, data_len: %d, data:\r\n",
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_ble_cbfc_rx_data->dev_addr),
      rsi_ble_cbfc_rx_data->lcid, rsi_ble_cbfc_rx_data->lcid, rsi_ble_cbfc_rx_data->len);

  for (ix= 0; ix < rsi_ble_cbfc_rx_data->len; ix++) {
    LOG_PRINT ("0x%x, ",rsi_ble_cbfc_rx_data->data[ix]);
  }
  LOG_PRINT ("\r\n");

	rsi_ble_app_set_event (RSI_APP_EVENT_CBFC_RX_DATA);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_cbfc_disconn_event
 * @brief      invoked when BLE L2CAP CBFC disconnect event is received
 * @param[in]  rsi_ble_cbfc_disconn, CBFC disconnected remote device info.
 * @return     none.
 * @section description
 * This Callback function indicates BLE L2CAP CBFC disconnect with remote devivce.
 */
void rsi_ble_on_cbfc_disconn_event (rsi_ble_event_cbfc_disconn_t *rsi_ble_cbfc_disconn)
{
  uint8_t remote_dev_addr[18] = {0};

  memcpy (remote_dev_bd_addr, rsi_ble_cbfc_disconn->dev_addr, 6);
  LOG_PRINT ("remote_dev_addr: %s, lcid: 0x%x, %d\r\n" ,
      rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_ble_cbfc_disconn->dev_addr),
      rsi_ble_cbfc_disconn->lcid,
      rsi_ble_cbfc_disconn->lcid);

	rsi_ble_app_set_event (RSI_APP_EVENT_CBFC_DISCONN);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_central
 * @brief      Tests the BLE GAP central role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the BLE central role and simple GAP API's.
 */
int32_t rsi_ble_central(void)
{
	int32_t status = 0;
	int32_t temp_event_map = 0;
  uint8_t ix;
  uint8_t cbfc_data[10] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,0x39, 0x30};

#ifndef RSI_WITH_OS
	//! Driver initialization
	status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
	{
		return status;
	}
  
  //! RS9116 intialisation 
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

	//! BLE register GAP callbacks
	rsi_ble_gap_register_callbacks(
      rsi_ble_simple_central_on_adv_report_event,
      rsi_ble_simple_central_on_conn_status_event,
      rsi_ble_simple_central_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_simple_central_on_enhance_conn_status_event,
      NULL,
      NULL,
      NULL);

  rsi_ble_l2cap_cbsc_register_callbacks (
      rsi_ble_on_cbfc_conn_req_event, 
      rsi_ble_on_cbfc_conn_complete_event,
      rsi_ble_on_cbfc_rx_data_event,
      rsi_ble_on_cbfc_disconn_event);

	//! initialize the event map
	rsi_ble_app_init_events ();

  //! start scanning
  status = rsi_ble_start_scanning();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  while(1)
  {
    //! Application main loop
#ifndef RSI_WITH_OS
		//! run the non os scheduler
		rsi_wireless_driver_task();
#endif
		//! checking for received events
		temp_event_map = rsi_ble_app_get_event ();
		if (temp_event_map == RSI_FAILURE) {
			//! if events are not received, loop will be continued
			continue;
		}

		switch(temp_event_map)
		{
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

			case RSI_APP_EVENT_CONNECTED:
			{
				//! remote device connected event

				//! clear the connected event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CONNECTED);

        //!
        //rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_app_connected_device.dev_addr);
        
        //! initiated the CBFC connection request.
        rsi_ble_cbfc_connreq (rsi_app_connected_device.dev_addr, RSI_BLE_PSM_VALUE);
			}
			break;

			case RSI_APP_EVENT_DISCONNECTED:
			{
				//! remote device disconnected event

				//! clear the disconnected event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
                device_found=0;
                //! start scanning
                status = rsi_ble_start_scanning();
                if(status != RSI_SUCCESS)
                {
                    LOG_PRINT ("start_scanning status: 0x%X\r\n", status);
                }

            }
			break;

      case RSI_APP_EVENT_CBFC_CONN_REQ:
      {
				//! clear the CBFC connection request event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CBFC_CONN_REQ);
      }
      break;

      case RSI_APP_EVENT_CBFC_RX_DATA:
      {
				//! clear the CBFC data receive event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CBFC_RX_DATA);
      }
      case RSI_APP_EVENT_CBFC_CONN_CMPL:
      {
				//! clear the CBFC connection completed status event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CBFC_CONN_CMPL);

        //! convert the 6 byte remote BD-address to ascii BD-address
        //rsi_6byte_dev_address_to_ascii (remote_dev_addr, rsi_app_cbfc_conn_cmpl.dev_addr);

        for (ix = 0; ix < sizeof (cbfc_data); ix++)
        {
          cbfc_data[ix]++;
          if (cbfc_data[ix] >= 0x3a) {
            cbfc_data[ix] = 0x30;
          }
        }
        //! send the CBFC data TX with length of 10 bytes.
        rsi_ble_cbfc_data_tx (rsi_app_cbfc_conn_cmpl.dev_addr, rsi_app_cbfc_conn_cmpl.lcid, sizeof (cbfc_data), cbfc_data);
      }
      break;

      case RSI_APP_EVENT_CBFC_DISCONN:
      {
				//! clear the CBFC disconnected event.
				rsi_ble_app_clear_event (RSI_APP_EVENT_CBFC_DISCONN);
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
  int32_t status;
#ifdef RSI_WITH_OS
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif



#ifndef RSI_WITH_OS
  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);
  
  //! Call BLE Central application
  status = rsi_ble_central();

  //! Application main loop
  main_loop();

  return status;
#endif
#ifdef RSI_WITH_OS
 //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! RS9116 initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_ble_central, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
