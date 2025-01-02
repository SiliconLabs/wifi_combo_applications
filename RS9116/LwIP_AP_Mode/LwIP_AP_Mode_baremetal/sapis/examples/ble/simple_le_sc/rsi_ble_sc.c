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
 * @brief : This file contains example application for BLE Secure Connections
 * @section Description :
 * This application demonstrates how to configure the Redpine device in Central
 * mode and connects with remote slave device and how to enable SMP (Security
 * Manager Protocol) pairing and a Secured passkey pairing.
 =================================================================================*/

/**
 * Include files
 * */

//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<stdio.h>
#include "rsi_driver.h"
//! Common include file
#include<rsi_common_apis.h>

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! local device name
#define RSI_BLE_DEVICE_NAME                 "BLE_SMP_SC"

//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY           0x00
#define  RSI_BLE_SMP_PASSKEY                 0

#ifdef RSI_BLE_PING
//! Ping time out                            
#define  RSI_BLE_PING_TIME_OUT               5000
#endif

//! application event list
#define  RSI_BLE_CONN_EVENT                  0x01
#define  RSI_BLE_DISCONN_EVENT               0x02
#define  RSI_BLE_SMP_REQ_EVENT               0x03
#define  RSI_BLE_SMP_RESP_EVENT              0x04
#define  RSI_BLE_SMP_PASSKEY_EVENT           0x05
#define  RSI_BLE_SMP_FAILED_EVENT            0x06
#define  RSI_BLE_ENCRYPT_STARTED_EVENT       0x07
#define  RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT   0x08
#define  RSI_BLE_SC_PASSKEY_EVENT            0x09
#define  RSI_BLE_LTK_REQ_EVENT               0x0A


#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE  1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY   1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000

void rsi_wireless_driver_task(void);

#endif
//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t str_remote_address[18];
static uint8_t remote_dev_address[6];
static uint32_t  numeric_value;
static rsi_bt_event_encryption_enabled_t  encrypt_keys;
static rsi_bt_event_le_ltk_request_t ble_ltk_req;
static rsi_bt_event_le_security_keys_t app_ble_sec_keys;
#ifdef RSI_BLE_PING
uint16_t timeout_resp;
#endif

#define MITM_REQ        0x01


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
	memcpy (remote_dev_address, resp_enh_conn->dev_addr, 6);
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
	memcpy (remote_dev_address, resp_conn->dev_addr, 6);
	LOG_PRINT("connect - str_remote_address : %s\r\n",
			rsi_6byte_dev_address_to_ascii (str_remote_address, resp_conn->dev_addr));

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
	memcpy (remote_dev_address, resp_disconnect->dev_addr, 6);
	LOG_PRINT("connect - str_remote_address : %s\r\n",
			rsi_6byte_dev_address_to_ascii (str_remote_address, resp_disconnect->dev_addr));

	rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}

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
void rsi_ble_on_smp_request (rsi_bt_event_smp_req_t  *remote_dev_address)
{
	memcpy (remote_dev_address, remote_dev_address->dev_addr, 6);
	LOG_PRINT("smp_req - str_remote_address : %s\r\n",
			rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
	rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
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
void rsi_ble_on_smp_response (rsi_bt_event_smp_resp_t  *remote_dev_address)
{
	memcpy (remote_dev_address, remote_dev_address->dev_addr, 6);
	LOG_PRINT("smp_resp - str_remote_address : %s\r\n",
			rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
	rsi_ble_app_set_event (RSI_BLE_SMP_RESP_EVENT);
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
void rsi_ble_on_smp_passkey (rsi_bt_event_smp_passkey_t  *remote_dev_address)
{
	memcpy (remote_dev_address, remote_dev_address->dev_addr, 6);
	LOG_PRINT("smp_passkey - str_remote_address : %s\r\n",
			rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
	rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_EVENT);
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
void rsi_ble_on_smp_passkey_display (rsi_bt_event_smp_passkey_display_t *smp_passkey_display)
{
	memcpy (remote_dev_address, smp_passkey_display->dev_addr, 6);
	LOG_PRINT("remote addr: %s, passkey: %s \r\n",
			rsi_6byte_dev_address_to_ascii (str_remote_address, smp_passkey_display->dev_addr),
			smp_passkey_display->passkey );
	rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);

}

void rsi_ble_on_sc_passkey (rsi_bt_event_sc_passkey_t *sc_passkey)
{
	memcpy (remote_dev_address, sc_passkey->dev_addr, 6);
	LOG_PRINT("remote addr: %s, passkey: %06d \r\n",
			rsi_6byte_dev_address_to_ascii (str_remote_address, sc_passkey->dev_addr),
			sc_passkey->passkey );
	numeric_value = sc_passkey->passkey;
	rsi_ble_app_set_event (RSI_BLE_SC_PASSKEY_EVENT);
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
	rsi_ble_app_set_event (RSI_BLE_LTK_REQ_EVENT);
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
void rsi_ble_on_le_security_keys  (rsi_bt_event_le_security_keys_t *rsi_ble_event_le_security_keys)
{
	memcpy (&app_ble_sec_keys, rsi_ble_event_le_security_keys, sizeof (rsi_bt_event_le_security_keys_t));
	LOG_PRINT("remote_ediv: 0x%x\r\n", app_ble_sec_keys.remote_ediv);
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
void rsi_ble_on_smp_failed (uint16_t  status, rsi_bt_event_smp_failed_t  *remote_dev_address)
{
	memcpy (remote_dev_address, remote_dev_address->dev_addr, 6);
	LOG_PRINT("smp_failed status: 0x%x, str_remote_address: %s\r\n", status,
			rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
	rsi_ble_app_set_event (RSI_BLE_SMP_FAILED_EVENT);
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
void rsi_ble_on_encrypt_started (uint16_t  status, rsi_bt_event_encryption_enabled_t *enc_enabled)
{
	LOG_PRINT("start encrypt status: %d \r\n", status);
  memcpy (&encrypt_keys, enc_enabled, sizeof (rsi_bt_event_encryption_enabled_t));
	rsi_ble_app_set_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_smp_test_app
 * @brief      This application is used for testing the SMP protocol.
 * @param[in]  none. 
 * @return     none.
 * @section description
 * This function is used to test the SMP Protocol.
 */
int32_t rsi_ble_smp_test_app (void)
{
	int32_t status = 0;
	int32_t  event_id;
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
		LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nDevice Initialization Success\r\n");
	}
#endif

	//! WC initialization
	status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWireless Initialization Success\r\n");
	}

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
			NULL,NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events ();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_DEVICE_NAME);

  //! prepare advertise data //local/device name
  adv[3] = strlen (RSI_BLE_DEVICE_NAME) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5], RSI_BLE_DEVICE_NAME);

	//! set advertise data
	rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_DEVICE_NAME) + 5);

	//! start addvertising
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

#ifdef RSI_BLE_PING
			//! get ping time out
			rsi_ble_get_le_ping_timeout(remote_dev_address, &timeout_resp);

			//! set le ping time out
			rsi_ble_set_le_ping_timeout (remote_dev_address, RSI_BLE_PING_TIME_OUT);

			//! get ping time out
			rsi_ble_get_le_ping_timeout(remote_dev_address, &timeout_resp);
#endif

			//! initiating the SMP pairing process
			status = rsi_ble_smp_pair_request (remote_dev_address, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
		}
		break;

		case RSI_BLE_DISCONN_EVENT:
		{
			//! event invokes when disconnection was completed

			//! clear the served event
			rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);

			//! start addvertising
			rsi_ble_start_advertising ();
		}
		break;

		case RSI_BLE_SMP_REQ_EVENT:
		{
			//! initiate SMP protocol as a Master

			//! clear the served event
			rsi_ble_app_clear_event (RSI_BLE_SMP_REQ_EVENT);

			//! initiating the SMP pairing process
			status = rsi_ble_smp_pair_request (remote_dev_address, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
		}
		break;

		case RSI_BLE_SMP_RESP_EVENT:
		{
			//! initiate SMP protocol as a Master

			//! clear the served event
			rsi_ble_app_clear_event (RSI_BLE_SMP_RESP_EVENT);

			//! initiating the SMP pairing process
			status = rsi_ble_smp_pair_response (remote_dev_address, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
		}
		break;

		case RSI_BLE_SMP_PASSKEY_EVENT:
		{
			//! initiate SMP protocol as a Master

			//! clear the served event
			rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_EVENT);

			//! initiating the SMP pairing process
			status = rsi_ble_smp_passkey (remote_dev_address, RSI_BLE_SMP_PASSKEY);
		}
		break;
		case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT:
		{
			//! clear the served event
			rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
		}
		break;

		case RSI_BLE_SC_PASSKEY_EVENT:
		{
			rsi_ble_app_clear_event (RSI_BLE_SC_PASSKEY_EVENT);
			rsi_ble_smp_passkey (remote_dev_address, numeric_value);
		}
		break;

		case RSI_BLE_LTK_REQ_EVENT:
		{
			rsi_ble_app_clear_event (RSI_BLE_LTK_REQ_EVENT);
			if ((encrypt_keys.localediv == ble_ltk_req.localediv) && 
          !(memcmp(ble_ltk_req.localrand,encrypt_keys.localrand,8))) {
				rsi_ble_ltk_req_reply (remote_dev_address, 1, encrypt_keys.localltk);
			} else {
				rsi_ble_ltk_req_reply (remote_dev_address, 0, NULL);
			}
		}
		break;

		case RSI_BLE_SMP_FAILED_EVENT:
		{
			//! initiate SMP protocol as a Master

			//! clear the served event
			rsi_ble_app_clear_event (RSI_BLE_SMP_FAILED_EVENT);
		}
		break;

		case RSI_BLE_ENCRYPT_STARTED_EVENT:
		{
			//! start the encrypt event

			//! clear the served event
			rsi_ble_app_clear_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
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
	//! Call Simple SMP protocol test Application
	rsi_ble_smp_test_app();

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
		LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nDevice Initialization Success\r\n");
	}

	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

	//! OS case
	//! Task created for BLE task
	rsi_task_create(rsi_ble_smp_test_app, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

	return status;
#endif
}
