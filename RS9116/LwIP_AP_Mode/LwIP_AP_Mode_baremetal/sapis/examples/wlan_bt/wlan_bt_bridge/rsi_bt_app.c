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

#define SPP_SLAVE    0
#define SPP_MASTER   1

#define SPP_MODE     SPP_MASTER

#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT                     printf
#else 
#define LOG_PRINT
#endif

#if (SPP_MODE == SPP_MASTER)
#define RSI_BT_LOCAL_NAME             "SPP_MASTER"
#else
#define RSI_BT_LOCAL_NAME             "SPP_SLAVE"
#endif

#define PIN_CODE                      "4321"
#define REMOTE_BD_ADDR                "4c:4f:ee:4f:1f:b9"//00:1B:DC:07:2C:F0" //"E4:92:FB:F7:28:BA"

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

#define RSI_LINKKEY_REPLY_SIZE        16

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE 3000

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
//! Enumeration for commands used in application
typedef enum rsi_app_cmd_e
{
  RSI_DATA = 0
}rsi_app_cmd_t;

//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t   local_name = {0};
static uint8_t str_conn_bd_addr[18];
static uint8_t remote_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t spp_data[200];
static uint16_t spp_data_len;
static uint8_t data[RSI_BT_MAX_PAYLOAD_SIZE];
static uint16_t data_len;
static uint8_t  linkkey[RSI_LINKKEY_REPLY_SIZE];
#ifdef RSI_WITH_OS
extern rsi_semaphore_handle_t bt_thread_sem;
#endif

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
#ifdef RSI_WITH_OS
	rsi_semaphore_post(&bt_thread_sem);
#endif
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
	if (resp_status == 0) {
		rsi_bt_app_set_event(RSI_APP_EVENT_CONNECTED);
	} else {
		rsi_bt_app_set_event(RSI_APP_EVENT_DISCONNECTED);
	}
  memcpy ((int8_t *)remote_dev_addr, conn_event->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on_conn: str_conn_bd_addr: %s, resp_status: 0x%x\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, conn_event->dev_addr), resp_status);
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
	rsi_bt_app_set_event (RSI_APP_EVENT_PINCODE_REQ);
  memcpy ((int8_t *)remote_dev_addr, user_pincode_request->dev_addr, RSI_DEV_ADDR_LEN);
  LOG_PRINT ("on_pin_code_req: str_conn_bd_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_pincode_request->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_on_linkkey_req
 * @brief      invoked when linkkey request event is received
 * @param[out] user_linkkey_req, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey request from remote device
 */
void rsi_ble_app_on_linkkey_req (uint16_t status, rsi_bt_event_user_linkkey_request_t  *user_linkkey_req)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_REQ);
	memcpy ((int8_t *)remote_dev_addr, user_linkkey_req->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("linkkey_req: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_req->dev_addr));
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
	rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_SAVE);
	memcpy ((int8_t *)remote_dev_addr, user_linkkey_save->dev_addr, RSI_DEV_ADDR_LEN);
  memcpy(linkkey, user_linkkey_save->linkKey, RSI_LINKKEY_REPLY_SIZE);
	LOG_PRINT ("linkkey_save: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_linkkey_save->dev_addr));
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
	if(resp_status == 0)
	{
		rsi_bt_app_set_event (RSI_APP_EVENT_AUTH_COMPLT);
	}
	else {
    rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
		memset (linkkey, 0, RSI_LINKKEY_REPLY_SIZE);
	}
	memcpy ((int8_t *)remote_dev_addr, auth_complete->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("auth_complete: str_conn_bd_addr: %s, resp_status: 0x%x\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, auth_complete->dev_addr), resp_status);
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
	rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
	memcpy ((int8_t *)remote_dev_addr, bt_disconnected->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("on_disconn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, bt_disconnected->dev_addr));
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
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_CONN);
	memcpy ((int8_t *)remote_dev_addr, spp_connect->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("spp_conn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, spp_connect->dev_addr));
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
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_DISCONN);
	memcpy ((int8_t *)remote_dev_addr, spp_disconn->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("spp_disconn: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, spp_disconn->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_display
 * @brief      invoked when passkey display event is received
 * @param[out] resp_status, event status
 * @param[out] bt_event_user_passkey_display, passkey information
 * @return     none.
 * @section description
 * This callback function indicates the passkey display event
 */

void rsi_bt_on_passkey_display (uint16_t resp_status, rsi_bt_event_user_passkey_display_t *bt_event_user_passkey_display)
{
	uint8_t ix; 
	rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_DISPLAY);
  //rsi_6byte_dev_address_to_ascii(str_conn_bd_addr, bt_event_user_passkey_display->passkey);
  memcpy (data,bt_event_user_passkey_display->passkey ,4);
  for (ix = 0; ix < 4; ix++)
  {
    LOG_PRINT(" 0x%02x,", bt_event_user_passkey_display->passkey[ix]);
  }
	
  LOG_PRINT ("\r\n");
  LOG_PRINT ("passkey: %d", *((uint32_t *)bt_event_user_passkey_display->passkey));
  LOG_PRINT ("\r\n");
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_passkey_request
 * @brief      invoked when passkey request event is received
 * @param[out] resp_status, event status
 * @param[out] user_passkey_request, passkey information
 * @return     none.
 * @section description
 * This callback function indicates the passkey request event
 */
void rsi_bt_on_passkey_request (uint16_t resp_status, rsi_bt_event_user_passkey_request_t *user_passkey_request)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_PASSKEY_REQUEST);
	memcpy ((int8_t *)remote_dev_addr, user_passkey_request->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("passkey_request: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, user_passkey_request->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_ssp_complete
 * @brief      invoked when ssp complete event is received
 * @param[out] resp_status, event status
 * @param[out] ssp_complete, secure simple pairing information
 * @return     none.
 * @section description
 * This callback function indicates the spp status
 */
void rsi_bt_on_ssp_complete (uint16_t resp_status, rsi_bt_event_ssp_complete_t *ssp_complete)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_SSP_COMPLETE);
	memcpy ((int8_t *)remote_dev_addr, ssp_complete->dev_addr, RSI_DEV_ADDR_LEN);
	LOG_PRINT ("ssp_complete: str_conn_bd_addr: %s\r\n",
	    rsi_6byte_dev_address_to_ascii((int8_t *)str_conn_bd_addr, ssp_complete->dev_addr));
}

/*==============================================*/
/**
 * @fn         rsi_bt_on_confirm_request
 * @brief      invoked when ssp confirm request event is received
 * @param[out] resp_status, event status
 * @param[out] ssp_complete, secure simple pairing requested remote device information
 * @return     none.
 * @section description
 * This callback function indicates the ssp confirm event
 */
void rsi_bt_on_confirm_request (uint16_t resp_status, rsi_bt_event_user_confirmation_request_t *user_confirmation_request)
{
	uint8_t ix; 
	rsi_bt_app_set_event (RSI_APP_EVENT_CONFIRM_REQUEST);

  memcpy (data,user_confirmation_request->confirmation_value ,4);
  for (ix = 0; ix < 4; ix++)
  {
    LOG_PRINT (" 0x%02x,", user_confirmation_request->confirmation_value[ix]);
  }
  LOG_PRINT ("\r\n");
  LOG_PRINT ("data: %s",user_confirmation_request->confirmation_value );
}

/*==============================================*/
/**
 * @fn         rsi_wlan_app_send_to_bt
 * @brief      this function is used to send data to bt app.
 * @param[in]   msg_type, it indicates write/notification event id.
 * @param[in]  data, raw data pointer.
 * @param[in]  data_len, raw data length.
 * @return     none.
 * @section description
 * This is a callback function
 */
void rsi_wlan_app_send_to_bt (uint16_t  msg_type, uint8_t *data, uint16_t data_len)
{
  spp_data_len = RSI_MIN (200, data_len);
  memcpy (spp_data, data, spp_data_len);
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_RX);
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

/*  LOG_PRINT ("spp_rx: data_len: %d, data: ", spp_data_len);
  for (ix = 0; ix < spp_receive->data_len; ix++)
  {
    LOG_PRINT (" 0x%02x,", spp_receive->data[ix]);
  }
  LOG_PRINT ("\r\n");
  LOG_PRINT ("data: %s", spp_receive->data);
*/

  rsi_bt_app_send_to_wlan(RSI_DATA, spp_receive->data, spp_receive->data_len);
}

void switch_proto_async(uint16_t mode, uint8_t *bt_disabled_status)
{
  LOG_PRINT("\n IN ASYNC \n");
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_init
 * @brief      Tests the BT Classic SPP Slave role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the SPP Slave role.
 */
int32_t rsi_bt_app_init (void)
{
  int32_t status = 0;
  uint8_t str_bd_addr[18] = {0};
  uint8_t eir_data[200] = {2,1,0};


  status = rsi_switch_proto(1, NULL);
  if(status != RSI_SUCCESS)
  {
	  LOG_PRINT("\n bt enable fail \n");
	  //return status;
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
		  rsi_bt_app_on_pincode_req,
		  rsi_bt_on_passkey_request,//passkey request
		  NULL,//inquiry complete
		  rsi_bt_app_on_auth_complete,
		  rsi_ble_app_on_linkkey_req,//linkkey request
		  rsi_bt_on_ssp_complete,//ssp coplete
		  rsi_ble_app_on_linkkey_save,
		  NULL, //get services
		  NULL,
		  NULL,
		  NULL, //search service
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
  
  //! prepare Extended Response Data 
  eir_data[3] = strlen (RSI_BT_LOCAL_NAME) + 1;
  eir_data[4] = 9;
  strcpy ((char *)&eir_data[5], RSI_BT_LOCAL_NAME);
  //! set eir data
  rsi_bt_set_eir_data (eir_data, strlen (RSI_BT_LOCAL_NAME) + 5);
#if(SPP_MODE != SPP_MASTER)
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
#else
  //! stop the discover mode
  status = rsi_bt_stop_discoverable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! stop the connectability mode
  status = rsi_bt_set_non_connectable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! initilize the SPP profile
  status = rsi_bt_spp_init ();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  LOG_PRINT("spp init success\n");
  //! register the SPP profile callback's
  rsi_bt_spp_register_callbacks (rsi_bt_app_on_spp_connect,
                                 rsi_bt_app_on_spp_disconnect,
                                 rsi_bt_app_on_spp_data_rx);
#if(SPP_MODE == SPP_MASTER)
  status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr, REMOTE_BD_ADDR));
  if(status != RSI_SUCCESS)
    {
      return status;
    }
  LOG_PRINT("bt_conn resp is 0x%x \n", status);
#endif

  return 0;

}

int32_t rsi_bt_app_task ()
{
  int32_t tx_ix = 0;
  uint8_t spp_tx_data_len = 0;
  int32_t status = 0;
  int32_t temp_event_map = 0;
  uint8_t str_bd_addr[18] = {0};

#ifdef RSI_WITH_OS
  while(1)
  {
#endif
	  //! Application main loop

	  //! checking for received events
	  temp_event_map = rsi_bt_app_get_event ();
	  if (temp_event_map == RSI_FAILURE) {
#ifdef RSI_WITH_OS
		  //! if events are not received loop will be continued.
		  rsi_semaphore_wait(&bt_thread_sem,0);
		  continue;
#else
		  //! if events are not received loop will be continued.
		  return 0;
#endif
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
          uint8_t *pin_code = (uint8_t *)PIN_CODE;

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
#if (SPP_MODE == SPP_MASTER)
          rsi_delay_ms (500);
          status = rsi_bt_spp_connect(remote_dev_addr);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
#endif
        }
        break;
      case RSI_APP_EVENT_DISCONNECTED:
        {
          //! remote device connected event

          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);
#if (SPP_MODE == SPP_MASTER)
          status = rsi_bt_connect(rsi_ascii_dev_address_to_6bytes_rev(remote_dev_addr, REMOTE_BD_ADDR));
          if(status != RSI_SUCCESS)
          {
            return status;
          }
          LOG_PRINT("bt_conn resp is 0x%x \n", status);
#endif
        }
        break;
      case RSI_APP_EVENT_LINKKEY_REQ:
        {
          //! linkkey request event

          //! clear the linkkey request event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_REQ);
          if ((linkkey[0] == 0) && (linkkey[15] == 0))
          {
            LOG_PRINT("linkkey request negative reply\r\n");
            //! sending the linkkey request negative reply
            rsi_bt_linkkey_request_reply ((int8_t *)remote_dev_addr, NULL, 0);
          } 
          else
          {
            LOG_PRINT("linkkey request positive reply\r\n");
            //! sending the linkkey request positive reply
            rsi_bt_linkkey_request_reply ((int8_t *)remote_dev_addr, linkkey, 1);
          }
	   	}
		break;

      case RSI_APP_EVENT_SPP_CONN:
        {
          //! spp connected event

          //! clear the spp connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_CONN);
#if(SPP_MODE == SPP_MASTER)
          strcpy ((char *)data, "spp_test_sample_1");
          spp_tx_data_len = strlen ((char *)data);
          data[spp_tx_data_len] = (tx_ix++)%10;
          status = rsi_bt_spp_transfer (remote_dev_addr, data, strlen (data));
#endif
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

        //! SPP data transfer (loop back)
        rsi_bt_spp_transfer (remote_dev_addr, spp_data, spp_data_len);
      }
      break;

      case RSI_APP_EVENT_PASSKEY_DISPLAY:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_PASSKEY_DISPLAY);
        }
        break;

      case RSI_APP_EVENT_PASSKEY_REQUEST:
        {
          //! clear the ssp receive event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_PASSKEY_REQUEST);
        }
        break;

      case RSI_APP_EVENT_SSP_COMPLETE:
	{
		//! clear the ssp receive event.
		rsi_bt_app_clear_event (RSI_APP_EVENT_SSP_COMPLETE);
		LOG_PRINT(" SSP conection completed\n");
	}
	break;

      case RSI_APP_EVENT_CONFIRM_REQUEST:
	{
		//! clear the ssp receive event.
		rsi_bt_app_clear_event (RSI_APP_EVENT_CONFIRM_REQUEST);
		LOG_PRINT("Confirmation is requested\n");
		rsi_bt_accept_ssp_confirm((int8_t *)remote_dev_addr);

	}
	break;

  }

#ifdef RSI_WITH_OS
	}
#endif
  return 0;
}
