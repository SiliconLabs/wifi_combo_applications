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
#include<stdio.h>

//! Common include file 
#include<rsi_common_apis.h>

#ifdef RSI_DEBUG_PRINTS
#define  LOG_PRINT   printf
#else
#define  LOG_PRINT
#endif

#define RSI_BT_LOCAL_NAME             "BT_DEV_005"
#define RSI_SEL_ANTENNA                RSI_SEL_INTERNAL_ANTENNA//RSI_SEL_EXTERNAL_ANTENNA
//#define REMOTE_BD_ADDR                "00:1B:DC:07:2C:F0" //"E4:92:FB:F7:28:BA"

//! application events list
#define RSI_APP_EVENT_CONNECTED                         1
#define RSI_APP_EVENT_PINCODE_REQ                       2
#define RSI_APP_EVENT_LINKKEY_SAVE                      3
#define RSI_APP_EVENT_AUTH_COMPLT                       4
#define RSI_APP_EVENT_DISCONNECTED                      5
#define RSI_APP_EVENT_INQ_COMPLT                        6
#define RSI_APP_EVENT_INQ_RESP                          7
#define RSI_APP_EVENT_UNBOND                            8
#define RSI_APP_EVENT_QUERY_SERVICES                    9
#define RSI_APP_EVENT_REMOTE_NAME_RESP                 10
#define RSI_APP_EVENT_LINKKEY_REQ                      11
#define RSI_APP_EVENT_REMOTE_NAME_CANCEL_RESP          12
#define RSI_APP_EVENT_SPP_CONN                         13
#define RSI_APP_EVENT_SPP_DISCONN                      14
#define RSI_APP_EVENT_SPP_RX                           15

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN] = {0};

//! Application global parameters.
static uint32_t rsi_app_async_event_map = 0;
static rsi_bt_resp_get_local_name_t   local_name = {0};
//static int8_t  rsi_app_resp_peer_device_name[RSI_DEV_NAME_LEN] = {0};
static uint8_t local_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t rsi_app_resp_get_bond_dev_addr[18] = {0};
static uint8_t remote_bond_dev_addr[6] = {0};
static uint8_t rsi_app_resp_get_unbond_dev_addr[18] = {0};
static uint8_t remote_unbond_dev_addr[6] = {0};
static uint8_t rsi_app_resp_device_state = 0;
static int8_t rsi_app_resp_rssi = 0;
static rsi_bt_event_user_linkkey_save_t rsi_bt_user_linkkey = {0};
static rsi_bt_event_inquiry_response_t rsi_bt_inquiry_response = {0};
static rsi_bt_resp_query_services_t bt_resp_query_services = {0};
static rsi_bt_event_remote_device_name_t remote_device_name = {0};
static rsi_bt_event_user_linkkey_save_t rsi_app_linkkey_save = {0};
static uint8_t data[200];
static uint16_t data_len;

//static uint8_t channel_map[CHANNEL_MAP_LEN] = {255,255,255,255,00,00,00,00,00,15}; // Minimum 20 channels needed
static uint8_t channel_map[CHANNEL_MAP_LEN] = {0xff, 0xff, 0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x0f}; // Minimum 20 channels needed
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
 * @fn         rsi_bt_app_on_conn_status_event
 * @brief      invoked when connection complete event is received
 * @param[out] bond_response, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection.
 */
void rsi_bt_app_on_conn_status_event(uint16_t resp_status, rsi_bt_event_bond_t *bond_response)
{
  memcpy ((int8_t *)remote_bond_dev_addr, bond_response->dev_addr, 6);
  LOG_PRINT ("\non_conn_status: rsi_app_resp_get_bond_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_bond_dev_addr, bond_response->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_CONNECTED);
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_on_unbond_status_event
 * @brief      invoked when unbond/disconnect status event is received
 * @param[out] unbond_status, unbonding remote device information
 * @return     none.
 * @section description
 * This callback function indicates disconnection from remote device
 */
void rsi_bt_app_on_unbond_status_event(uint16_t resp_status, rsi_bt_event_unbond_t *unbond_status)
{
  memcpy ((int8_t *)remote_unbond_dev_addr, unbond_status->dev_addr, 6);
  LOG_PRINT ("\non_disconn_status: rsi_app_resp_get_unbond_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_unbond_dev_addr, unbond_status->dev_addr));

	rsi_bt_app_set_event (RSI_APP_EVENT_UNBOND);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_pincode_req_event
 * @brief      invoked when pincode request event is received
 * @param[out] user_pincode_request, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the pincode request from remote device
 */
void rsi_bt_app_on_pincode_req_event(uint16_t resp_status, rsi_bt_event_user_pincode_request_t *user_pincode_request)
{
  memcpy ((int8_t *)remote_bond_dev_addr, user_pincode_request->dev_addr, 6); 
  LOG_PRINT ("\non_pincode_req: rsi_app_resp_get_bond_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_bond_dev_addr, user_pincode_request->dev_addr));

	rsi_bt_app_set_event (RSI_APP_EVENT_PINCODE_REQ);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_linkkey_req_event
 * @brief      invoked when linkkey request event is received
 * @param[out] user_linkkey_req, pairing remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey request from remote device
 */
void rsi_bt_app_on_linkkey_req_event(uint16_t resp_status, rsi_bt_event_user_linkkey_request_t *user_linkkey_request)
{
  memcpy ((int8_t *)remote_bond_dev_addr, user_linkkey_request->dev_addr, 6);
  LOG_PRINT ("\non_linkkey_req: rsi_app_resp_get_bond_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_bond_dev_addr, user_linkkey_request->dev_addr));

	rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_REQ);
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_on_linkkey_save_event
 * @brief      invoked when linkkey save event is received
 * @param[out] user_linkkey_save, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey save from local device
 */
void rsi_bt_app_on_linkkey_save_event(uint16_t status, rsi_bt_event_user_linkkey_save_t *user_linkkey_save)
{
	memcpy(&rsi_app_linkkey_save, user_linkkey_save, sizeof(rsi_bt_event_user_linkkey_save_t));
  memcpy ((int8_t *)remote_bond_dev_addr, user_linkkey_save->dev_addr, 6);
  LOG_PRINT ("\non_linkkey_save: rsi_app_resp_get_bond_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_bond_dev_addr, user_linkkey_save->dev_addr));

	rsi_bt_app_set_event (RSI_APP_EVENT_LINKKEY_SAVE);
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_on_inq_complete_event
 * @brief      invoked when inquiry complete event is received
 * @return     none.
 * @section description
 * This callback function indicates inquiry complete.
 */
void rsi_bt_app_on_inq_complete_event(uint16_t resp_status)
{
	rsi_bt_app_set_event (RSI_APP_EVENT_INQ_COMPLT);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_auth_complete_event
 * @brief      invoked when authentication complete event is received
 * @param[out] auth_complete, authentication complete information
 * @return     none.
 * @section description
 * This callback function when authentication complete is recieved.
 */
void rsi_bt_app_on_auth_complete_event(uint16_t resp_status, rsi_bt_event_auth_complete_t *auth_complete)
{
  memcpy ((int8_t *)remote_bond_dev_addr, auth_complete->dev_addr, 6);
  LOG_PRINT ("\non_auth_complete: rsi_app_resp_get_bond_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_bond_dev_addr, auth_complete->dev_addr));
  if(resp_status == 0)
  {
    rsi_bt_app_set_event (RSI_APP_EVENT_AUTH_COMPLT);
  }
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_on_remote_name_resp_event
 * @brief      invoked when remote device name response event is received
 * @param[out] remote_name, paired remote device name information
 * @return     none.
 * @section description
 * This callback function is called when get remote device name cmd response is recieved.
 */
void rsi_bt_app_on_remote_name_resp_event(uint16_t resp_status, rsi_bt_event_remote_device_name_t *remote_name)
{
	memcpy(&remote_device_name, remote_name, sizeof(rsi_bt_event_remote_device_name_t));
	rsi_bt_app_set_event (RSI_APP_EVENT_REMOTE_NAME_RESP);
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_on_remote_name_cancel_resp_event
 * @brief      invoked when remote device name request cancel event is received
 * @param[out] user_linkkey_req, paired remote device information
 * @return     none.
 * @section description
 * This callback function indicates the linkkey save from local device
 */
void rsi_bt_app_on_remote_name_cancel_resp_event(uint16_t resp_status, rsi_bt_event_remote_name_request_cancel_t *remote_name_cancel)
{
	memcpy(&remote_device_name, remote_name_cancel, sizeof(rsi_bt_event_remote_name_request_cancel_t));
	rsi_bt_app_set_event (RSI_APP_EVENT_REMOTE_NAME_CANCEL_RESP);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_inq_resp_event
 * @brief      invoked when inquiry response is received
 * @param[out] inq_resp, inquiry response information
 * @return     none.
 * @section description
 * This callback function is called when response to do inquiry command is recieved. 
 */
void rsi_bt_app_on_inq_resp_event(uint16_t resp_status, rsi_bt_event_inquiry_response_t *inq_resp)
{
	memcpy(&rsi_bt_inquiry_response, inq_resp, sizeof(rsi_bt_event_inquiry_response_t));
	rsi_bt_app_set_event (RSI_APP_EVENT_INQ_RESP);
}

/*==============================================*/
/**
 * @fn         rsi_bt_app_on_disconn_event
 * @brief      invoked when disconnect event is received
 * @param[out] resp_status, disconnect status/error
 * @param[out] bt_disconnected, disconnected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the disconnected device information
 */
void rsi_bt_app_on_disconn_event(uint16_t resp_status, rsi_bt_event_disconnect_t *bt_disconnected)
{
  memcpy ((int8_t *)remote_unbond_dev_addr, bt_disconnected->dev_addr, 6);
  LOG_PRINT ("\non_disconn_status: rsi_app_resp_get_disconnect_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_unbond_dev_addr, bt_disconnected->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_DISCONNECTED);
}


/*==============================================*/
/**
 * @fn         rsi_bt_app_on_query_services_event
 * @brief      invoked when serch services response event is received
 * @param[out] query_services, response information to get services command.
 * @return     none.
 * @section description
 * This callback function is used to get services of the remote device.
 */
void rsi_bt_app_on_query_services_event(uint16_t resp_status, rsi_bt_resp_query_services_t *query_services)
{
	LOG_PRINT (" \n Query services response = %d",resp_status);
	memcpy(&bt_resp_query_services, query_services, sizeof(rsi_bt_resp_query_services_t));
	rsi_bt_app_set_event (RSI_APP_EVENT_QUERY_SERVICES);
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
  memcpy ((int8_t *)remote_bond_dev_addr, spp_connect->dev_addr, 6);
  LOG_PRINT ("\non_spp_connect: rsi_app_resp_get_bond_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_bond_dev_addr, spp_connect->dev_addr));
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
  memcpy ((int8_t *)remote_bond_dev_addr, spp_disconn->dev_addr, 6);
  LOG_PRINT ("\non_spp_disconnect: rsi_app_resp_get_bond_dev_addr: %s\r\n",
      rsi_6byte_dev_address_to_ascii((int8_t *)rsi_app_resp_get_bond_dev_addr, spp_disconn->dev_addr));
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_DISCONN);
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

  data_len = spp_receive->data_len;
  memcpy (data, spp_receive->data, spp_receive->data_len);
  LOG_PRINT ("\nspp_rx: data_len: %d, data: ", spp_receive->data_len);
  for (ix = 0; ix < spp_receive->data_len; ix++)
  {
	  LOG_PRINT (" 0x%02x,", spp_receive->data[ix]);
  }
  LOG_PRINT ("\r\n");
  LOG_PRINT ("data: %s", spp_receive->data);
	rsi_bt_app_set_event (RSI_APP_EVENT_SPP_RX);
}

/*==============================================*/
/**
 * @fn         rsi_bt_sample_app
 * @brief      Tests the BT GAP peripheral role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the BT master and slave roles.
 */
int32_t rsi_bt_sample_app(void)
{
  int32_t status = 0;
  int32_t temp_event_map = 0;
  int32_t time_out_ms = 100;
  uint8_t i = 0;
  int32_t tx_ix = 0;
  uint8_t device_state = 0;
  uint8_t str_bd_addr[18] = {0};
  uint32_t class_of_device;
  uint8_t spp_tx_data_len = 0;
  uint8_t mode = 0;
 
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
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BT_CLASSIC);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! BT Classic register GAP callbacks:
  rsi_bt_gap_register_callbacks(
      NULL,//role_change
      rsi_bt_app_on_conn_status_event, 
      rsi_bt_app_on_unbond_status_event,
      rsi_bt_app_on_disconn_event,
      rsi_bt_app_on_inq_resp_event,
      rsi_bt_app_on_remote_name_resp_event,
      NULL,//passkey_display
      rsi_bt_app_on_remote_name_cancel_resp_event,
      NULL,//confirm_req
      rsi_bt_app_on_pincode_req_event,
      NULL,//passkey_req
      rsi_bt_app_on_inq_complete_event,
      rsi_bt_app_on_auth_complete_event,
      rsi_bt_app_on_linkkey_req_event,
      NULL,//ssp_complete
      rsi_bt_app_on_linkkey_save_event,
      rsi_bt_app_on_query_services_event,
      NULL,
      NULL,
      NULL, //search_services
      NULL);

  //! initialize the event map
  rsi_bt_app_init_events ();


  //! Test API_1 -   get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(local_dev_addr);
  if(status != RSI_SUCCESS)
  {
    //return status;
  }
  LOG_PRINT ("\r\nlocal_bd_address: %s\r\n",
      rsi_6byte_dev_address_to_ascii ((int8_t *)str_bd_addr, local_dev_addr));

  //! Test API_2 - set the local device name
  status = rsi_bt_set_local_name(RSI_BT_LOCAL_NAME);
  if(status != RSI_SUCCESS)
  {
    //return status;
  }

  //! Test API_3 - get the local device name
  status = rsi_bt_get_local_name(&local_name);
  if(status != RSI_SUCCESS)
  {
    //return status;
  }
  LOG_PRINT ("\n local_dev_name = %s", local_name.name);
  
#if 0
  //! Test API_4 - Get COD
  status = rsi_bt_get_local_class_of_device(&class_of_device);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\n class_of_device = 0x%x", class_of_device);
  class_of_device |= BIT(22);   //! Telephony

  //! Test API_5 - Set COD
  status = rsi_bt_set_local_class_of_device(class_of_device);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  class_of_device = 0;
  status = rsi_bt_get_local_class_of_device(&class_of_device);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  LOG_PRINT ("\n class_of_device = 0x%x", class_of_device);
#endif
#ifdef SLAVE_MODE_ENABLE
#if 0
  //! Test API_26 - Set limited discovery mode
  status = rsi_bt_start_limited_discoverable(time_out_ms);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#else
  //! Test API_6 - start the discover mode
  // Unlimited discovery
  status = rsi_bt_start_discoverable();
  if(status != RSI_SUCCESS)
  {
    //return status;
  }
#endif

  //! Test API_7 - start the discover mode
  status = rsi_bt_get_discoverable_status(&device_state);
  if(status != RSI_SUCCESS)
  {
    //return status;
  }
  LOG_PRINT ("\n discovery_mode = %d", (device_state));

#if 0
  //! Test API_25 - Set device in Non-connectable mode
  status = rsi_bt_set_non_connectable();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#else
  //! Test API_8 - start the connectability mode
  status = rsi_bt_set_connectable();
  if(status != RSI_SUCCESS)
  {
    //return status;
  }
#endif

  status = rsi_bt_set_afh_host_channel_classification(1,&channel_map);
  if(status != RSI_SUCCESS)
  {
    LOG_PRINT ("\n Set AFH Failed \n");
    LOG_PRINT ("\n status : %x \n", status);
    //return status;
  }
  else
  {
    LOG_PRINT ("\n Set AFH Successed \n");
  }

  status = rsi_bt_get_afh_host_channel_classification(&mode);
  if(status != RSI_SUCCESS)
  {
    LOG_PRINT ("\n err mode = %d \n",mode);
  }
  else
  {
    LOG_PRINT ("\n mode = %d \n",mode);
  }
  //! Test API_9 - Get connectability status
  status = rsi_bt_get_connectable_status(&device_state);
  if(status != RSI_SUCCESS)
  {
    //return status;
  }
  LOG_PRINT ("\n connectability_mode = %d", (device_state));
#else
do_inquiry :
          //! start BT inquiry
LOG_PRINT ("\n Starting inquiry ");
          status = rsi_bt_inquiry(0 , 500, 0x02);
          if(status != RSI_SUCCESS)
          {
            goto do_inquiry;
          }
#endif

#if 1
  //! Test API_10 - initilize the SPP profile
  status = rsi_bt_spp_init ();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! register the SPP profile callback's
  rsi_bt_spp_register_callbacks (rsi_bt_app_on_spp_connect,
                                 rsi_bt_app_on_spp_disconnect,
                                 rsi_bt_app_on_spp_data_rx);
#endif


#if 0
  //! Test API_24 - initiate SPP connection with remote device
  status = rsi_bt_spp_connect (REMOTE_BD_ADDR);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! Test API_25 - set bt classic antenna transmit power level
   status = rsi_bt_set_antenna_tx_power_level(1,19);
  if(status != RSI_SUCCESS){
    return status;
  }


#if 0
  //! Test API_11 - Enable authentication
  status = rsi_bt_enable_authentication();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  //! Test API_12 - Disable authentication
  status = rsi_bt_disable_authentication();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  while(1)
  {
    //! Application main loop

    //! run the non OS scheduler
    rsi_wireless_driver_task();

    //! checking for received events
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
        	LOG_PRINT (" \n *********************connected_event*****************");
          //! remote device connected event

          //! clear the connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_CONNECTED);


          //! get the RSSI value with connected remote device
          //! Test API_14 - Get RSSI of remote device.
          status = rsi_bt_get_rssi((int8_t *)remote_bond_dev_addr, &rsi_app_resp_rssi);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
          LOG_PRINT ("\n RSSI of connected device = %d", rsi_app_resp_rssi);

#if 0
          //! Test API_17 - Get remote device name request cancel
          status = rsi_bt_remote_name_request_cancel(remote_bond_dev_addr);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
#endif
        }
        break;

      case RSI_APP_EVENT_UNBOND:
        {
        	LOG_PRINT (" \n *********************Unbond_event*****************");
          rsi_bt_app_clear_event(RSI_APP_EVENT_UNBOND);
        }
        break;
      case RSI_APP_EVENT_PINCODE_REQ:
        {
          uint8_t pin_code[RSI_MAX_PINCODE_REPLY_SIZE] = {'4','3','2','1'};
		  
          LOG_PRINT (" \n *********************Pin_code_request_event*****************");
          rsi_bt_app_clear_event(RSI_APP_EVENT_PINCODE_REQ);


          //! Test API_18 - accept pincode
          //! sending the pincode requet reply
          status = rsi_bt_pincode_request_reply((int8_t *)remote_bond_dev_addr, (uint8_t *)&pin_code, 1);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
#if 1
          //! Test API_15 - Get local device role: 0 - Master, 1 - Slave.
          status = rsi_bt_get_local_device_role((int8_t *)remote_bond_dev_addr, &device_state);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
          LOG_PRINT ("\n Role of local device = %d", device_state);

          remote_device_name.name_length = RSI_DEV_NAME_LEN;
          //! Test API_16 - Get remote device name.
          status = rsi_bt_remote_name_request_async((int8_t *)remote_bond_dev_addr, &remote_device_name);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
#endif
#if 0
          //! Test API_20 - BT disconnect/ unbond.
          status = rsi_bt_disconnect(remote_bond_dev_addr);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
#endif
        }
        break;
      case RSI_APP_EVENT_LINKKEY_REQ:
        {
        	LOG_PRINT (" \n *********************Link_key_request_event*****************");
          rsi_bt_app_clear_event(RSI_APP_EVENT_LINKKEY_REQ);

          //! sending the linkkey request negative reply
          //! Test API_19 - accept pincode
          status = rsi_bt_linkkey_request_reply((int8_t *)remote_bond_dev_addr, NULL, 0);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
        }
        break;
      case RSI_APP_EVENT_LINKKEY_SAVE:
        {
        	LOG_PRINT (" \n *********************Link_key_save_event*****************");
          rsi_bt_app_clear_event (RSI_APP_EVENT_LINKKEY_SAVE);
        }
        break;
      case RSI_APP_EVENT_INQ_COMPLT:
        {
        	LOG_PRINT (" \n *********************Inquiry_complete_event*****************");
          rsi_bt_app_clear_event (RSI_APP_EVENT_INQ_COMPLT);

#if 1
          //! Test API_21 - BT connect/ bond.
          status = rsi_bt_connect((int8_t *)remote_bond_dev_addr);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
#endif
        }
        break;
      case RSI_APP_EVENT_REMOTE_NAME_RESP:
        {
        	LOG_PRINT (" \n *********************Remote_name_req_event*****************");
          rsi_bt_app_clear_event (RSI_APP_EVENT_REMOTE_NAME_RESP);
          LOG_PRINT ("\n remote_device_name = %s", remote_device_name.remote_device_name);
        }
        break;
      case RSI_APP_EVENT_REMOTE_NAME_CANCEL_RESP:
        {
        	LOG_PRINT (" \n *********************Remote_name_req_cancel_event*****************");
          rsi_bt_app_clear_event (RSI_APP_EVENT_REMOTE_NAME_CANCEL_RESP);
          LOG_PRINT ("\n remote_device_name_req canceled for  %s", remote_device_name.remote_device_name);
        }
        break;
      case RSI_APP_EVENT_INQ_RESP:
        {
        	LOG_PRINT (" \n *********************Inquiry_resp_event*****************");
          rsi_bt_app_clear_event (RSI_APP_EVENT_INQ_RESP);
          LOG_PRINT ("\n inquiry_type = %d",rsi_bt_inquiry_response.inquiry_type );
          for(i = 0; i< 6; i++)
          {
        	  LOG_PRINT ("\n remote_device_addr = 0x%x",rsi_bt_inquiry_response.dev_addr[i]);
          }

          //! converts the 6 byte MAC address to string format
          //rsi_6byte_dev_address_to_ascii ((int8_t *)rsi_app_resp_get_bond_dev_addr, rsi_bt_inquiry_response.dev_addr);
        }
        break;
      case RSI_APP_EVENT_AUTH_COMPLT:
        {
        	LOG_PRINT (" \n *********************Auth_complete*****************");
          rsi_bt_app_clear_event (RSI_APP_EVENT_AUTH_COMPLT);
        }
        break;
      case RSI_APP_EVENT_QUERY_SERVICES:
        {
        	LOG_PRINT (" \n *********************Query_services_complete*****************");
          rsi_bt_app_clear_event (RSI_APP_EVENT_QUERY_SERVICES);
        }
        break;
      case RSI_APP_EVENT_DISCONNECTED:
        {
          //! remote device connected event

        	LOG_PRINT (" \n *********************Disconnected_event*****************");
          //! clear the disconnected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_DISCONNECTED);

#if 0
          //! Test API_22 - BT cancel connect.
          status = rsi_bt_cancel_connect(remote_bond_dev_addr);
          if(status != RSI_SUCCESS)
          {
            //return status;
          }
#endif


        }
        break;
      case RSI_APP_EVENT_SPP_CONN:
        {
          //! spp connected event

          //! clear the spp connected event.
          rsi_bt_app_clear_event (RSI_APP_EVENT_SPP_CONN);
          strcpy ((char *)data, "spp_test_sample_1");
          spp_tx_data_len = strlen ((char *)data);
          data[spp_tx_data_len] = (tx_ix++)%10;
          rsi_bt_spp_transfer (remote_bond_dev_addr, data, spp_tx_data_len);
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
          //! Test API_23 - ssp transfer.
          rsi_bt_spp_transfer (remote_bond_dev_addr, data, data_len);
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

  //! Call BLE Peripheral application
  status = rsi_bt_sample_app();

  //! Application main loop
  main_loop();

  return status;

}
