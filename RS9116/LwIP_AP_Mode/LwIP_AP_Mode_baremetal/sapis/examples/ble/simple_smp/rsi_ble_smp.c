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

#include <stdio.h>
#include <string.h>
#include "rsi_driver.h"
//! Common include file
#include<rsi_common_apis.h>

#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT  printf
#else 
#define LOG_PRINT
#endif


//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000
#define MASTER 1
#define SLAVE 0

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! local device name
#define RSI_BLE_DEVICE_NAME            "BLE_SIMPLE_SMP"
#define RSI_SCAN_RESP_DATA					   "REDPINE"
#define ROLE                            SLAVE

//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY      0x03
#define  RSI_BLE_SMP_PASSKEY            0

//! application event list
#define RSI_APP_EVENT_ADV_REPORT        0x00
#define  RSI_BLE_CONN_EVENT             0x01
#define  RSI_BLE_DISCONN_EVENT          0x02
#define  RSI_BLE_SMP_REQ_EVENT          0x03
#define  RSI_BLE_SMP_RESP_EVENT         0x04
#define  RSI_BLE_SMP_PASSKEY_EVENT      0x05
#define  RSI_BLE_SMP_FAILED_EVENT       0x06
#define  RSI_BLE_ENCRYPT_STARTED_EVENT  0x07
#define  RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT 0x08
#define  RSI_BLE_LTK_REQ_EVENT          0x09
#define  RSI_BLE_MTU_EVENT              0x0A
#if (ROLE == MASTER)
//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR                          "00:23:A7:80:70:B9"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME					   "REDPINE_DEV"

#endif

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                        1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                          1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS                             1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                      2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                    3000

void rsi_wireless_driver_task(void);

#endif
#define MITM_REQ    0x01
//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t str_remote_address[18];
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
rsi_bt_event_encryption_enabled_t glbl_enc_enabled;
rsi_bt_event_le_ltk_request_t temp_le_ltk_req;
rsi_ble_event_mtu_t app_ble_mtu_event;

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
#if (ROLE == MASTER)
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
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
  memcpy ((int8_t *)remote_dev_bd_addr, resp_enh_conn->dev_addr, 6);
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
  memcpy ((int8_t *)remote_dev_bd_addr, resp_conn->dev_addr, 6);
  LOG_PRINT ("connect - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((uint8_t *)str_remote_address, resp_conn->dev_addr));

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
  memcpy ((int8_t *)remote_dev_bd_addr, resp_disconnect->dev_addr, 6);
  LOG_PRINT ("disconnect - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((uint8_t *)str_remote_address, resp_disconnect->dev_addr));

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
  memcpy ((int8_t *)remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("\n\n smp_req - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((uint8_t *)str_remote_address, remote_dev_address->dev_addr));
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
  memcpy ((int8_t*)remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_resp - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((uint8_t*)str_remote_address, remote_dev_address->dev_addr));
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
  memcpy ((int8_t *)remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_passkey - str_remote_address : %s\r\n",
      rsi_6byte_dev_address_to_ascii ((uint8_t *)str_remote_address, remote_dev_address->dev_addr));
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

	LOG_PRINT ("passkey: %s \r\n", smp_passkey_display->passkey );
    rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);

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
  memcpy ((int8_t *)remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_failed status: 0x%x, str_remote_address: %s\r\n", status,
      rsi_6byte_dev_address_to_ascii ((uint8_t *)str_remote_address, remote_dev_address->dev_addr));
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
  memcpy(&glbl_enc_enabled, enc_enabled, sizeof(rsi_bt_event_encryption_enabled_t));
  rsi_ble_app_set_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_le_ltk_req_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_le_ltk_req_event(rsi_bt_event_le_ltk_request_t *le_ltk_req)
{
  LOG_PRINT (" \r\n rsi_ble_on_le_ltk_req_event \r\n");
  memcpy(&temp_le_ltk_req, le_ltk_req, sizeof(rsi_bt_event_le_ltk_request_t));
  rsi_ble_app_set_event (RSI_BLE_LTK_REQ_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_mtu_event (rsi_ble_event_mtu_t *rsi_ble_mtu)
{
  memcpy (&app_ble_mtu_event, rsi_ble_mtu, sizeof (rsi_ble_event_mtu_t));
  rsi_ble_app_set_event (RSI_BLE_MTU_EVENT);
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
    uint8_t already_paired = 0;

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

	//! registering the SMP callback functions
	rsi_ble_smp_register_callbacks(
      rsi_ble_on_smp_request,
      rsi_ble_on_smp_response,
      rsi_ble_on_smp_passkey,
      rsi_ble_on_smp_failed,
      rsi_ble_on_encrypt_started,
      rsi_ble_on_smp_passkey_display,
      NULL,
      rsi_ble_on_le_ltk_req_event,
      NULL,
      NULL,NULL);

  //! registering the GATT callback functions
	rsi_ble_gatt_register_callbacks (NULL,
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
            NULL,
            NULL);

	//!  initializing the application events map
	rsi_ble_app_init_events ();

	//! Set local name
	rsi_bt_set_local_name(RSI_BLE_DEVICE_NAME);

#if (ROLE == MASTER)
   	//! start scanning
	status = rsi_ble_start_scanning();
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#else
	//!preparing scan response data
	 adv[3] = strlen (RSI_SCAN_RESP_DATA) + 1;
     adv[4] = 9;
     strcpy ((char *)&adv[5], RSI_SCAN_RESP_DATA);

	//!set scan response data
	rsi_ble_set_scan_response_data (adv, strlen (RSI_SCAN_RESP_DATA	) + 5);

	//! prepare advertise data //local/device name
    adv[3] = strlen (RSI_BLE_DEVICE_NAME) + 1;
    adv[4] = 9;
    strcpy ((char *)&adv[5], RSI_BLE_DEVICE_NAME);

	//! set advertise data
	rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_DEVICE_NAME) + 5);


	//! start the advertising
	status = rsi_ble_start_advertising();
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
        continue;
      }

      switch (event_id) {
#if (ROLE == MASTER)
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

          }
          break;

        case RSI_BLE_DISCONN_EVENT:
          {
            //! event invokes when disconnection was completed

            //! clear the served event
            rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
#if (ROLE == SLAVE)
            //! start the advertising
            status = rsi_ble_start_advertising();
            if(status != RSI_SUCCESS)
            {
              return status;
            }
#endif
#if (ROLE == MASTER)
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
        case RSI_BLE_MTU_EVENT:
          {
            //! event invokes when write/notification events received

            //! clear the served event
            rsi_ble_app_clear_event (RSI_BLE_MTU_EVENT);
#if (ROLE == MASTER)
            if(!already_paired)
            {
              //! If not paired even once
              //! initiating the SMP pairing process
              status = rsi_ble_smp_pair_request (app_ble_mtu_event.dev_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
            }
#endif
          }
          break;
        case RSI_BLE_LTK_REQ_EVENT:
          {
            //! event invokes when disconnection was completed

            //! clear the served event
            rsi_ble_app_clear_event (RSI_BLE_LTK_REQ_EVENT);
            //rsi_6byte_dev_address_to_ascii (remote_dev_bd_addr,temp_le_ltk_req.dev_addr);

#if (ROLE == SLAVE)
            if((temp_le_ltk_req.localediv == glbl_enc_enabled.localediv) && 
                !((memcmp(temp_le_ltk_req.localrand, glbl_enc_enabled.localrand,8)))
              )
            {
              LOG_PRINT("\n positive reply\n");
              //! give le ltk req reply cmd with positive reply
              status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr, 1, glbl_enc_enabled.localltk);
            }
            else
            {
              LOG_PRINT("\n negative reply\n");
              //! give le ltk req reply cmd with negative reply
              status = rsi_ble_ltk_req_reply(temp_le_ltk_req.dev_addr, 0, NULL);
            }
#endif
          }
          break;

        case RSI_BLE_SMP_REQ_EVENT:
          {
            //! initiate SMP protocol as a Master

            //! clear the served event
            rsi_ble_app_clear_event (RSI_BLE_SMP_REQ_EVENT);

#if (ROLE == MASTER)
            if(already_paired)
              //! If once paired
              //! initiating the SMP pairing process
              status = rsi_ble_smp_pair_request (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
#endif
          }
          break;

        case RSI_BLE_SMP_RESP_EVENT:
          {
            //! initiate SMP protocol as a Master

            //! clear the served event
            rsi_ble_app_clear_event (RSI_BLE_SMP_RESP_EVENT);

            //! initiating the SMP pairing process
            status = rsi_ble_smp_pair_response (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
          }
          break;

        case RSI_BLE_SMP_PASSKEY_EVENT:
          {
            //! initiate SMP protocol as a Master

            //! clear the served event
            rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_EVENT);

            //! initiating the SMP pairing process
            status = rsi_ble_smp_passkey (remote_dev_bd_addr, RSI_BLE_SMP_PASSKEY);
          }
          break;
        case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT:
          {
            //! clear the served event
            rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
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
            already_paired = 1;
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
    return status;
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
