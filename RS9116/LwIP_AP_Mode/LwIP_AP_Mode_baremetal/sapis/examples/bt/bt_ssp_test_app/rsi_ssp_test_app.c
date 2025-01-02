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

//! BT include file to refer BT APIs
#include<rsi_bt_apis.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>
#include<rsi_bt_config.h>
#include<rsi_bt.h>
#include <stdio.h>

//! Common include file 
#include<rsi_common_apis.h>

//! application defines 
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif
#define RSI_BT_LOCAL_NAME             "BT_SSP_SNIFF"
#define PIN_CODE                      "1234"

//! application events list
#define RSI_APP_EVENT_CONNECTED       1
#define RSI_APP_EVENT_PINCODE_REQ     2
#define RSI_APP_EVENT_LINKKEY_SAVE    3
#define RSI_APP_EVENT_AUTH_COMPLT     4
#define RSI_APP_EVENT_DISCONNECTED    5
#define RSI_APP_EVENT_LINKKEY_REQ     6
#define RSI_APP_EVENT_SPP_CONN        7
#define RSI_APP_EVENT_SPP_DISCONN     8
#define RSI_APP_EVENT_SPP_RX          9

/** ssp related defines********/
#define RSI_APP_EVENT_PASSKEY_DISPLAY 10
#define RSI_APP_EVENT_PASSKEY_REQUEST 11
#define RSI_APP_EVENT_SSP_COMPLETE    12
#define RSI_APP_EVENT_CONFIRM_REQUEST 13

/*** SNIFF RELATED DEFINES********/
#define RSI_APP_EVENT_MODE_CHANGED    14
#define RSI_APP_EVENT_SNIFF_SUBRATING 15

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 1000

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

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN] = {0};

//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t   local_name = {0};
static uint8_t str_conn_bd_addr[18];
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t remote_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t data[200];
static uint16_t data_len;

/*==============================================*/
/**
 * @fn         rsi_bt_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BT initialization.
 */
static void rsi_bt_app_init_events()
{
	rsi_app_async_event_map = 0;
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_set_event
 * @brief      sets the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
static void rsi_bt_app_set_event(uint32_t event_num)
{
	rsi_app_async_event_map |= BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_bt_app_clear_event(uint32_t event_num)
{
	rsi_app_async_event_map &= ~BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_bt_app_get_event(void)
{
	uint32_t  ix;

	for (ix = 0; ix < 32; ix++)
	{
		if (rsi_app_async_event_map & (1 << ix))
		{
		   return ix;
		}
	}

	return (RSI_FAILURE);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_conn
 * @brief      invoked when connection complete event is received
 * @param[out] resp_status, connection status of the connected device.
 * @param[out] conn_event, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_bt_app_on_conn (uint16_t resp_status, rsi_bt_event_bond_t *conn_event)
{
   memcpy ((int8_t *)remote_dev_addr, conn_event->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on_conn: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, conn_event->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_pincode_req
 * @brief      invoked when pincode request event is received
 * @param[out] user_pincode_request, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the pincode request from remote device
 */
void rsi_bt_app_on_pincode_req(uint16_t resp_status, rsi_bt_event_user_pincode_request_t *user_pincode_request)
{
  memcpy ((int8_t *)remote_dev_addr, user_pincode_request->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on_pin_coe_req: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_pincode_request->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_PINCODE_REQ);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_linkkey_req
 * @brief      invoked when linkkey request event is received
 * @param[out] user_linkkey_req, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey request from remote device
 */
void rsi_bt_app_on_linkkey_req (uint16_t status, rsi_bt_event_user_linkkey_request_t  *user_linkkey_req)
{
  memcpy ((int8_t *)remote_dev_addr, user_linkkey_req->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("linkkey_req: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_req->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_REQ);
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_on_linkkey_save
 * @brief      invoked when linkkey save event is received
 * @param[out] user_linkkey_req, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey save from local device
 */
void rsi_ble_app_on_linkkey_save (uint16_t status, rsi_bt_event_user_linkkey_save_t *user_linkkey_save)
{
	memcpy ((int8_t *)remote_dev_addr, user_linkkey_save->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("linkkey_save: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_save->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_SAVE);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_auth_complete
 * @brief      invoked when authentication event is received
 * @param[out] resp_status, authentication status
 * @param[out] auth_complete, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the pairing status and remote device information
 */
void rsi_bt_app_on_auth_complete (uint16_t resp_status, rsi_bt_event_auth_complete_t *auth_complete)
{
	memcpy ((int8_t *)remote_dev_addr, auth_complete->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("auth_complete: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, auth_complete->dev_addr));
  if(resp_status == 0)
  {
    rsi_bt_app_set_event (RSI_APP_EVENT_AUTH_COMPLT);
  }
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_disconn
 * @brief      invoked when disconnect event is received
 * @param[out] resp_status, disconnect status/error
 * @param[out] bt_disconnected, disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the disconnected device information
 */
void rsi_bt_app_on_disconn (uint16_t resp_status, rsi_bt_event_disconnect_t *bt_disconnected)
{
 	memcpy ((int8_t *)remote_dev_addr, bt_disconnected->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("on_disconn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_spp_connect
 * @brief      invoked when spp profile connected event is received
 * @param[out] spp_connect, spp connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the spp connected remote device information
 */
void rsi_bt_app_on_spp_connect (uint16_t resp_status, rsi_bt_event_spp_connect_t *spp_connect)
{
	memcpy ((int8_t *)remote_dev_addr, spp_connect->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("spp_conn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, spp_connect->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_CONN);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_spp_disconnect
 * @brief      invoked when spp profile disconnected event is received
 * @param[out] spp_disconn, spp disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the spp disconnected event
 */
void rsi_bt_app_on_spp_disconnect (uint16_t resp_status, rsi_bt_event_spp_disconnect_t *spp_disconn)
{
	memcpy ((int8_t *)remote_dev_addr, spp_disconn->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("spp_disconn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, spp_disconn->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_DISCONN);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_display
 * @brief      invoked when passkey diaplay event is received
 * @param[out] passkey display, remote device passkey information
 * @return     none.
 * @section description
 * This callback function indicates the passkey display event
 */
void rsi_bt_on_passkey_display (uint16_t resp_status, rsi_bt_event_user_passkey_display_t *bt_event_user_passkey_display)
{
  LOG_PRINT ("passkey: %d", bt_event_user_passkey_display->passkey );
	rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_DISPLAY);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_request
 * @brief      invoked when passkey request event is received
 * @param[out] passkey request, passkey request to remote device 
 * @return     none.
 * @section description
 * This callback function indicates the passkey request event
 */

void rsi_bt_on_passkey_request (uint16_t resp_status, rsi_bt_event_user_passkey_request_t *user_passkey_request)
{
	memcpy ((int8_t *)remote_dev_addr, user_passkey_request->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("passkey_request: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_passkey_request->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_REQUEST);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_ssp_complete
 * @brief      invoked when ssp complete event is received
 * @param[out] ssp complete, ssp completed remote device information 
 * @return     none.
 * @section description
 * This callback function indicates the ssp complete event
 */
void rsi_bt_on_ssp_complete (uint16_t resp_status, rsi_bt_event_ssp_complete_t *ssp_complete)
{
	memcpy ((int8_t *)remote_dev_addr, ssp_complete->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("ssp_complete: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, ssp_complete->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_SSP_COMPLETE);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_confirm_request
 * @brief      invoked when confirmation request event is received
 * @param[out] confirmation request,confirmation request to remote device  
 * @return     none.
 * @section description
 * This callback function indicates the confirmation request event
 */
void rsi_bt_on_confirm_request (uint16_t resp_status, rsi_bt_event_user_confirmation_request_t *user_confirmation_request)
{
  LOG_PRINT ("data: %s",user_confirmation_request->confirmation_value );
	rsi_bt_app_set_event (RSI_APP_EVENT_CONFIRM_REQUEST);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_mode_change
 * @brief      invoked when mode chande event is received
 * @param[out] mode change,mode change request to remote device  
 * @return     none.
 * @section description
 * This callback function indicates the mode change event
 */
void rsi_bt_on_mode_change (uint16_t resp_status, rsi_bt_event_mode_change_t  *mode_change)
{
  memcpy ((int8_t *)remote_dev_addr, mode_change->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("mode_change_event: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, mode_change->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_MODE_CHANGED);
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_sniff_subrating
 * @brief      invoked when sniff subrating event is received
 * @param[out] sniff subrating,shiff subrating request to remote device  
 * @return     none.
 * @section description
 * This callback function indicates the shiff subrating event
 */
void rsi_bt_on_sniff_subrating (uint16_t resp_status,rsi_bt_event_sniff_subrating_t  *mode_change)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_SNIFF_SUBRATING);
  memcpy ((int8_t *)remote_dev_addr, mode_change->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("mode_change_event: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, mode_change->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_spp_data_rx
 * @brief      invoked when spp data rx event is received
 * @param[out] spp_receive, spp data from remote device
 * @return     none.
 * @section description
 * This callback function indicates the spp data received event
 */
void rsi_bt_app_on_spp_data_rx (uint16_t resp_status, rsi_bt_event_spp_receive_t *spp_receive)
{
  uint16_t ix;

	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_RX);
  data_len = spp_receive->data_len;
  memcpy (data, spp_receive->data, spp_receive->data_len);
  LOG_PRINT ("spp_rx: data_len: %d, data: ", spp_receive->data_len);
  for (ix = 0; ix < spp_receive->data_len; ix++)
  {
    LOG_PRINT (" 0x%02x,", spp_receive->data[ix]);
  }
  LOG_PRINT ("\r\n");
  LOG_PRINT ("data: %s", spp_receive->data);
}

/*==============================================*/
/**
 * @fn         rsi_bt_spp_master
 * @brief      Tests the BT Classic SPP Slave role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the SPP Slave role.
 */
int32_t rsi_bt_ssp_test_app (void)
{
  int32_t status = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[18] = {0};

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
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BT_CLASSIC);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! BT register GAP callbacks:
  rsi_bt_gap_register_callbacks(
      NULL, //role_change
      rsi_bt_app_on_conn, 
      NULL, //
      rsi_bt_app_on_disconn,
      NULL,//scan_resp
      NULL,//remote_name_req
      rsi_bt_on_passkey_display,//passkey_display
      NULL,//remote_name_req+cancel
      rsi_bt_on_confirm_request,//confirm req
      NULL,//rsi_bt_app_on_pincode_req,
      rsi_bt_on_passkey_request,//passkey request
      NULL,//inquiry complete
      rsi_bt_app_on_auth_complete,
      rsi_bt_app_on_linkkey_req,//linkkey request
     	rsi_bt_on_ssp_complete,//ssp coplete
      rsi_ble_app_on_linkkey_save,
      NULL, //get services
      NULL,
			rsi_bt_on_mode_change,
			rsi_bt_on_sniff_subrating, //search service
      NULL);
  //! initialize the event map
  rsi_bt_app_init_events ();

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(local_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_bd_address: %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_bd_addr, local_dev_addr));

  //! set the local device name
  status = rsi_bt_set_local_name(RSI_BT_LOCAL_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! get the local device name
  status = rsi_bt_get_local_name(&local_name);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\r\nlocal_name: %s\r\n", local_name.name);
  
  //! start the discover mode
  status = rsi_bt_start_discoverable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! start the connectability mode
  status = rsi_bt_set_connectable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! start the ssp mode
  status = rsi_bt_set_ssp_mode(1,1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! initilize the SPP profile
  status = rsi_bt_spp_init ();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! register the SPP profile callback's
  rsi_bt_spp_register_callbacks (rsi_bt_app_on_spp_connect,
                                 rsi_bt_app_on_spp_disconnect,
                                 rsi_bt_app_on_spp_data_rx);

  while(1)
  {
    //! Application main loop

#ifndef RSI_WITH_OS
    //! run the non OS scheduler
    rsi_wireless_driver_task();
#endif
    //! checking for received events;
    temp_event_map = rsi_bt_app_get_event ();
    if (temp_event_map == RSI_FAILURE) {
      //! if events are not received loop will be continued.
      continue;
    }

    //! if any event is received, it will be served.
    switch(temp_event_map)
    {
      case RSI_APP_EVENT_CONNECTED:
        {
          //! remote device connected event

          //! clear the connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONNECTED);
        }
        break;

      case RSI_APP_EVENT_PINCODE_REQ:
        {
          //! pincode request event
          uint8_t *pin_code = PIN_CODE;

          //! clear the pincode request event.
          rsi_bt_app_clear_event(RSI_APP_EVENT_PINCODE_REQ);

          //! sending the pincode requet reply
          status = rsi_bt_pincode_request_reply((int8_t *)remote_dev_addr, pin_code, 1);
          if(status != RSI_SUCCESS)
          {
            return status;
          }
        }
        break;
      case RSI_APP_EVENT_LINKKEY_SAVE:
        {
          //! linkkey save event

          //! clear the likkey save event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_SAVE);
        }
        break;
      case RSI_APP_EVENT_AUTH_COMPLT:
        {
          //! authentication complete event

          //! clear the authentication complete event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_AUTH_COMPLT);
        }
        break;
      case RSI_APP_EVENT_DISCONNECTED:
        {
          //! remote device connected event

          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
        }
        break;
      case RSI_APP_EVENT_LINKKEY_REQ:
        {
          //! linkkey request event

          //! clear the linkkey request event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_REQ);

          LOG_PRINT("linkkey_req: %s\r\n", str_conn_bd_addr);
          //! sending the linkkey request negative reply
          rsi_bt_linkkey_request_reply ((int8_t *)remote_dev_addr, NULL, 0);
        }
        break;

      case RSI_APP_EVENT_SPP_CONN:
        {
          //! spp connected event

          //! clear the spp connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_CONN);

          LOG_PRINT("\n initiating sniff mode command here \n");

          /* here we call the sniff_mode command*/
					rsi_bt_sniff_mode(remote_dev_addr,0xC0,0xA0,0x04,0x02);
                
        }
        break;

      case RSI_APP_EVENT_SPP_DISCONN:
        {
          //! spp disconnected event

          //! clear the spp disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_DISCONN);
        }
        break;

      case RSI_APP_EVENT_SPP_RX:
        {
          //! spp receive event

          //! clear the spp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_RX);
          LOG_PRINT("\nintiaiting sniff_stop command\n");
          rsi_bt_sniff_exit_mode(remote_dev_addr);

          //! SPP data transfer (loop back)
          rsi_bt_spp_transfer (remote_dev_addr, data, data_len);
        }
        break;

      case RSI_APP_EVENT_PASSKEY_DISPLAY:
        {
          //! clear the passkey display event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_PASSKEY_DISPLAY);
        }
        break;

      case RSI_APP_EVENT_PASSKEY_REQUEST:
        {
          //! clear the passkey request event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_PASSKEY_REQUEST);
        }
        break;

      case RSI_APP_EVENT_SSP_COMPLETE:
        {
          //! clear the ssp complete event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SSP_COMPLETE);
					LOG_PRINT(" SSP conection completed\n");
        }
        break;

      case RSI_APP_EVENT_CONFIRM_REQUEST:
        {
          //! clear the ssp confirmation event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONFIRM_REQUEST);
					LOG_PRINT("Confirmation is requested\n");
					rsi_bt_accept_ssp_confirm((int8_t *)remote_dev_addr);

        }
        break;

      case RSI_APP_EVENT_MODE_CHANGED:
        {
          //! clear mode change event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_MODE_CHANGED);
					LOG_PRINT("MODE_CHANGE IS COMPLETED\n");
        }
        break;

      case RSI_APP_EVENT_SNIFF_SUBRATING:
        {
          //! clear the sniff subrating event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SNIFF_SUBRATING);
					LOG_PRINT("SNIFF SUBRATING IS COMPLETED\n");
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
  int32_t status;

#ifdef RSI_WITH_OS
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif



#ifndef RSI_WITH_OS
  //Start BT-BLE Stack
  intialize_bt_stack(STACK_BT_MODE);
  
  //! Call BLE Peripheral application
  status = rsi_bt_ssp_test_app();

  //! Application main loop
  main_loop();

  return status;
#endif
#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN );
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

  //Start BT-BLE Stack
  intialize_bt_stack(STACK_BT_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_bt_ssp_test_app, "bt_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  rsi_start_os_scheduler();
  return status;
#endif

}
