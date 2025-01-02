/**
 * @file     rsi_bt_gap_apis.c
 * @version  0.1
 * @date     03 Sep 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains BT GAP API's which needs to be called from application
 *
 *  @section Description  This file contains BT GAP API's called from application
 *
 */
#ifdef RSI_BT_ENABLE		
#include "rsi_driver.h" 
#include "rsi_bt.h"
#include "rsi_bt_apis.h"
#include "rsi_bt_config.h"

/* NOTE***********************************************
 * For all the API's device address shall be provided as ASCII string.
 * Ex: "00:12:34:AB:CD:EF"
 ****************************************************/

/*==============================================*/
/**
 * @fn         rsi_bt_set_local_class_of_device
 * @brief      sets the local device COD
 * @param[in]  class_of_device, name to be set to the local device COD
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function sets the given name to the local device COD
 */
int32_t rsi_bt_set_local_class_of_device(uint32_t class_of_device)
{
  rsi_bt_req_set_local_cod_t bt_req_set_local_cod = {0};
  bt_req_set_local_cod.class_of_device = class_of_device;

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_LOCAL_COD, &bt_req_set_local_cod, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_get_local_class_of_device
 * @brief      request the local COD name
 * @param[out] resp, response buffer to hold the response of this API
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used know the local device COD.
 */
int32_t rsi_bt_get_local_class_of_device(uint8_t *resp)
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_QUERY_LOCAL_COD, NULL, resp);
}

/*==============================================*/
/**
 * @fn         rsi_bt_start_discoverable
 * @brief      request the local device to enter discovery mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the BT Module in discovery mode.
 */
int32_t rsi_bt_start_discoverable(void)
{
  rsi_bt_req_set_discv_mode_t bt_req_set_discv_mode = {0};
  bt_req_set_discv_mode.mode = START_DISCOVERY;
  bt_req_set_discv_mode.time_out = 0;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_DISCV_MODE, &bt_req_set_discv_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_start_limited_discoverable
 * @brief      request the local device to enter limited discovery mode
 * @param[in]  time_out_ms, Limited discovery mode time_out in ms.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the BT Module in limited discovery mode.
 */
int32_t rsi_bt_start_limited_discoverable(int32_t time_out_ms)
{
  rsi_bt_req_set_discv_mode_t bt_req_set_discv_mode = {0};
  bt_req_set_discv_mode.mode = START_LIMITED_DISCOVERY;
  bt_req_set_discv_mode.time_out = time_out_ms;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_DISCV_MODE, &bt_req_set_discv_mode, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_write_current_iac_lap
 * @brief      writeing the current iac lap to the controller          
 * @param[in]  no_of_iaps, number of iap lap count               
 * @param[in]  iap_lap_list, pointer to the iap laps lis           
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the BT Module in limited discovery mode.
 */
#define IAP_LAP_SIZE                3
int32_t rsi_bt_write_current_iac_lap(uint8_t no_of_iaps,uint8_t *iap_lap_list)
{
  rsi_bt_req_set_current_iap_lap_t bt_req_iap_lap = {0};
  uint8_t ix = 0;
  uint8_t len = 0;

  bt_req_iap_lap.cnt = no_of_iaps;
  
  for(ix=0; ix < no_of_iaps; ix++) {
    len = ix * IAP_LAP_SIZE; 
    memcpy(&bt_req_iap_lap.list[len], (uint8_t *)&iap_lap_list[len], IAP_LAP_SIZE);
  }

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_CURRENT_IAC_LAP, &bt_req_iap_lap, NULL);
}


/*==============================================*/
/**
 * @fn         rsi_bt_stop_discoverable
 * @brief      request the local device to exit discovery mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to exit the BT Module from discovery mode.
 */
int32_t rsi_bt_stop_discoverable(void)
{
  rsi_bt_req_set_discv_mode_t bt_req_set_discv_mode = {0};
  bt_req_set_discv_mode.mode = STOP_DISCOVERY;
  bt_req_set_discv_mode.time_out = 0;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_DISCV_MODE, &bt_req_set_discv_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_get_discoverable_status
 * @brief      request the local device discovery mode status
 * @param[out] resp, response buffer to hold the response of this API
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to get BT Module discovery mode status.
 */
//! Change this
int32_t rsi_bt_get_discoverable_status(uint8_t *resp)
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_QUERY_DISCV_MODE, NULL, resp);
}

/*==============================================*/
/**
 * @fn         rsi_bt_set_connectable
 * @brief      request the local device to set connectablity mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the BT Module in connectable mode.
 */
int32_t rsi_bt_set_connectable(void)
{
  rsi_bt_req_set_connectability_mode_t bt_req_set_connectability_mode = {0};
  bt_req_set_connectability_mode.connectability_mode = CONNECTABLE;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_CONNECTABILITY_MODE, &bt_req_set_connectability_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_set_afh_host_channel_classification
 * @brief      request the local device to set afh channel classification
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the No.of channels to be used 
 */
int32_t rsi_bt_set_afh_host_channel_classification (uint8_t enable, uint8_t *channel_map)
{
  rsi_bt_req_set_afh_host_channel_classification_t afh_host_channel_classification = {0};

  afh_host_channel_classification.channel_assessment_mode = RSI_WRITE_ASSESSMENT_MODE;
  afh_host_channel_classification.enable = enable;
  if (channel_map != NULL) {
    memcpy (afh_host_channel_classification.channel_classification , channel_map, CHANNEL_MAP_LEN);
  }  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_AFH_HOST_CHANNEL_CLASSIFICATION, &afh_host_channel_classification, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_get_afh_host_channel_classification
 * @brief      request the local device to get afh channel assessment mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to get the afh channel assessmnet mode 
 */
int32_t rsi_bt_get_afh_host_channel_classification (uint8_t *status)
{
  rsi_bt_req_set_afh_host_channel_classification_t afh_host_channel_classification = {0};

  afh_host_channel_classification.channel_assessment_mode = RSI_READ_ASSESSMENT_MODE;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_AFH_HOST_CHANNEL_CLASSIFICATION, &afh_host_channel_classification, status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_set_non_connectable
 * @brief      request the local device to set connectablity mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the BT Module in non-connectable mode.
 */
int32_t rsi_bt_set_non_connectable(void)
{
  rsi_bt_req_set_connectability_mode_t bt_req_set_connectability_mode = {0};
  bt_req_set_connectability_mode.connectability_mode = NON_CONNECTABLE;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_CONNECTABILITY_MODE, &bt_req_set_connectability_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_get_connectable_status
 * @brief      request the local device connectablity status
 * @param[out] resp, response buffer to hold the response of this API
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to get BT Module connectablity status.
 */
int32_t rsi_bt_get_connectable_status(uint8_t *resp)
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_QUERY_CONNECTABILITY_MODE, NULL, resp);
}

/*==============================================*/
/**
 * @fn         rsi_bt_enable_authentication
 * @brief      request the local device to enable authentication
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to enable authentication.
 */
int32_t rsi_bt_enable_authentication(void)
{
  rsi_bt_req_set_pair_mode_t bt_req_set_pair_mode = {0};
  bt_req_set_pair_mode.pair_mode = RSI_ENABLE;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_PAIR_MODE, &bt_req_set_pair_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_disable_authentication
 * @brief      request the local device to disable authentication
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to disable authentication.
 */
int32_t rsi_bt_disable_authentication(void)
{
  rsi_bt_req_set_pair_mode_t bt_req_set_pair_mode = {0};
  bt_req_set_pair_mode.pair_mode = RSI_DISABLE;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_PAIR_MODE, &bt_req_set_pair_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_get_authentication
 * @brief      request authentication
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to initiate authentication.
 */
int32_t rsi_bt_get_authentication(void)
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_QUERY_PAIR_MODE, NULL, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_remote_name_request_async
 * @brief      request the remote device name
 * @param[in]  remote_address, remote device address
 * @param[out] bt_event_remote_device_name, response buffer to hold the name of remote device
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used know the remote device name.
 */
int32_t rsi_bt_remote_name_request_async(int8_t *remote_dev_addr, rsi_bt_event_remote_device_name_t *bt_event_remote_device_name)
{
  rsi_bt_req_remote_name_t bt_req_remote_name = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_remote_name.dev_addr, remote_dev_addr);
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_REMOTE_NAME_REQUEST, &bt_req_remote_name, (void *)bt_event_remote_device_name);
}

/*==============================================*/
/**
 * @fn         rsi_bt_remote_name_request_cancel
 * @brief      cancels the remote device name request
 * @param[in]  remote_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used cancel the remote device name request.
 */
int32_t rsi_bt_remote_name_request_cancel(int8_t *remote_dev_addr)
{ 
  rsi_bt_req_remote_name_cancel_t bt_req_remote_name_cancel = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_remote_name_cancel.dev_addr, remote_dev_addr);
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_REMOTE_NAME_REQUEST_CANCEL, &bt_req_remote_name_cancel, NULL);
}


/*==============================================*/
/**
 * @fn         rsi_bt_inquiry
 * @brief      request the local device to start inquiry
 * @param[in]  inquiry_type, inquiry type.
 * @param[in]  inquiry_duration, duration of inquiry.
 * @param[in]  max_devices, maximum devices.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to start inquiry.
 */
int32_t rsi_bt_inquiry(uint8_t inquiry_type, uint32_t inquiry_duration, uint8_t max_devices)
{
  rsi_bt_req_inquiry_t bt_req_inquiry = {0};
  if(inquiry_type & BIT(7)) {
    bt_req_inquiry.lap[0] = 0x00;
    bt_req_inquiry.lap[1] = 0x8B;
    bt_req_inquiry.lap[2] = 0x9E;
  } else {
    bt_req_inquiry.lap[0] = 0x33;
    bt_req_inquiry.lap[1] = 0x8B;
    bt_req_inquiry.lap[2] = 0x9E;
  }
  bt_req_inquiry.inquiry_type = (inquiry_type & ~(BIT(7)));
  bt_req_inquiry.duration = inquiry_duration;
  bt_req_inquiry.maximum_devices_to_find = max_devices;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_INQUIRY, &bt_req_inquiry, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_cancel_inquiry
 * @brief      request the local device to cancel inquiry
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to cancel inquiry.
 */
int32_t rsi_bt_cancel_inquiry(void)
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_INQUIRY_CANCEL, NULL, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_enable_device_under_testmode
 * @brief      request the local device to enable test mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to enable test mode.
 */
int32_t rsi_bt_enable_device_under_testmode(void)
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_ENABLE_DEVICE_UNDER_TESTMODE, NULL, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_connect
 * @brief      request to initiate the connection with the remote device
 * @param[in]  remote_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to initiate the connection request
 */
int32_t rsi_bt_connect(int8_t *remote_dev_addr)
{
  rsi_bt_req_connect_t bt_req_connect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_connect.dev_addr, remote_dev_addr);
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_BOND, &bt_req_connect, NULL);
}


/*==============================================*/
/**
 * @fn         rsi_bt_gatt_connect
 * @brief      request to initiate the gatt connection with the remote device
 * @param[in]  remote_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to initiate the gatt connection request
 */

int32_t rsi_bt_gatt_connect (int8_t *remote_dev_addr)
{
  rsi_bt_req_gatt_connect_t bt_req_gatt_connect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_gatt_connect.dev_addr, remote_dev_addr);
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_GATT_CONNECT, &bt_req_gatt_connect, NULL);

}
/*==============================================*/
/**
 * @fn         rsi_bt_cancel_connect
 * @brief      request the connection cancel with the remote device
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to cancel the connection request
 * */
int32_t rsi_bt_cancel_connect(int8_t *remote_dev_address)
{
  rsi_bt_req_connect_cancel_t bt_req_connect_cancel = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_connect_cancel.dev_addr, remote_dev_address);

  //! Send connect cancel command
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_BOND_CANCEL, &bt_req_connect_cancel, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_disconnect
 * @brief      request the disconnect with the remote device
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to disconnect the physical connection
 * */
int32_t rsi_bt_disconnect(int8_t *remote_dev_address)
{
  rsi_bt_req_disconnect_t bt_req_disconnect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_disconnect.dev_addr, remote_dev_address);

  //! Send disconnect command
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_UNBOND, &bt_req_disconnect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_accept_ssp_confirm
 * @brief      accepts the user confirmation
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to give the confirmation for the values sent by remote BT device at the time of bonding. 
 * */
int32_t rsi_bt_accept_ssp_confirm(int8_t *remote_dev_address)
{
  rsi_bt_req_user_confirmation_t bt_req_user_confirmation = {{0}};
  
  bt_req_user_confirmation.confirmation = ACCEPT;
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_user_confirmation.dev_addr, remote_dev_address);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_USER_CONFIRMATION, &bt_req_user_confirmation, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_reject_ssp_confirm
 * @brief      rejects the User Confirmation
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to reject the confirmation for the values sent by remote BT device at the time of bonding. 
 * */
int32_t rsi_bt_reject_ssp_confirm(int8_t *remote_dev_address)
{
  rsi_bt_req_user_confirmation_t bt_req_user_confirmation = {{0}};
  
  bt_req_user_confirmation.confirmation = REJECT;
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_user_confirmation.dev_addr, remote_dev_address);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_USER_CONFIRMATION, &bt_req_user_confirmation, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_passkey
 * @brief      requests the passkey
 * @param[in]  remote_dev_address, remote device address
 * @param[in] passkey, response buffer to hold the response of this API
 * @param[in] reply_type, reply type - positive or negative
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to request the passkey. 
 * */
int32_t rsi_bt_passkey(int8_t  *remote_dev_addr, uint32_t passkey, uint8_t reply_type)
{
  rsi_bt_req_passkey_reply_t bt_req_passkey_reply = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_passkey_reply.dev_addr, remote_dev_addr);
  bt_req_passkey_reply.pass_key = passkey;
  bt_req_passkey_reply.reply_type = reply_type;
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_PASSKEY_REPLY, &bt_req_passkey_reply, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_pincode_request_reply
 * @brief      accepts pincode
 * @param[in]  remote_dev_address, remote device address
 * @param[in]  pin_code, pin code input
 * @param[in]  reply_type, reply type - positive or negative
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to accept the pincode.
 * */
int32_t rsi_bt_pincode_request_reply(int8_t  *remote_dev_addr, uint8_t *pin_code, uint8_t reply_type)
{
  rsi_bt_req_pincode_reply_t bt_req_pincode_reply = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_pincode_reply.dev_addr, remote_dev_addr);
  bt_req_pincode_reply.reply_type = reply_type;
  memcpy(bt_req_pincode_reply.pincode, pin_code, RSI_MIN(strlen(pin_code),RSI_MAX_PINCODE_REPLY_SIZE));

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_PINCODE_REPLY, &bt_req_pincode_reply, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_linkkey_request_reply
 * @brief      accepts pincode
 * @param[in]  remote_dev_address, remote device address
 * @param[in]  pin_code, pin code input
 * @param[in]  reply_type, reply type - positive or negative
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to accept the pincode.
 * */
int32_t rsi_bt_linkkey_request_reply (int8_t  *remote_dev_addr, uint8_t *linkkey, uint8_t reply_type)
{
  rsi_bt_req_linkkey_reply_t  bt_linkkey_req_reply = {{0}};
 

  rsi_ascii_dev_address_to_6bytes_rev(bt_linkkey_req_reply.dev_addr, remote_dev_addr);
  bt_linkkey_req_reply.reply_type = reply_type;
  if (linkkey != NULL) {
    memcpy (bt_linkkey_req_reply.linkkey, linkkey, 16);
  }

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_LINKKEY_REPLY, &bt_linkkey_req_reply, NULL);
}


/*==============================================*/
/**
 * @fn         rsi_bt_get_local_device_role
 * @brief      requests local device role
 * @param[in]  remote_dev_address, remote device address
 * @param[out] resp, response buffer to hold the response of this API
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to request the role of local device.
 * */
int32_t rsi_bt_get_local_device_role(int8_t  *remote_dev_addr, uint8_t *resp)
{
  rsi_bt_req_query_role_t bt_req_query_role = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_query_role.dev_addr, remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_QUERY_ROLE, &bt_req_query_role, resp);
}

/*==============================================*/
/**
 * @fn         rsi_bt_set_local_device_role
 * @brief      setting the local device role
 * @param[in]  remote_dev_address, set_role 
 * @param[out] resp, response buffer to hold the response of this API
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to set the role of local device.
 * */
int32_t rsi_bt_set_local_device_role(int8_t  *remote_dev_addr,uint8_t set_role, uint8_t *resp)
{
  rsi_bt_req_set_role_t bt_req_set_role = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_set_role.dev_addr, remote_dev_addr);
  bt_req_set_role.role = set_role;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_ROLE, &bt_req_set_role, resp);
}


/*==============================================*/
/**
 * @fn         rsi_bt_get_services_async
 * @brief      queries remote device services
 * @param[in]  remote_dev_address, remote device address
 * @param[in]  num_of_responses, number of services expected
 * @param[out] bt_resp_query_services, response struct to hold the response of this API
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to query the services of local device.
 * */
//int32_t rsi_bt_get_services(uint8_t *remote_dev_addr, uint32_t *resp, uint16_t num_of_responses)
int32_t rsi_bt_get_services_async(int8_t *remote_dev_addr, rsi_bt_resp_query_services_t *bt_resp_query_services)
{
  rsi_bt_req_query_services_t bt_req_query_services = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_query_services.dev_addr, remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_QUERY_SERVICES, &bt_req_query_services, bt_resp_query_services);
}

/*==============================================*/
/**
 * @fn         rsi_bt_search_service_async
 * @brief      queries local device services
 * @param[in]  remote_dev_address, remote device address
 * @param[in]  service_uuid, uuid of the service for search
 * @param[out] bt_resp_search_service, response struct to hold the response of this API
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to search service of the given uuid.
 * */
int32_t rsi_bt_search_service_async(int8_t *remote_dev_addr, uint32_t service_uuid)
{
  rsi_bt_req_search_service_t bt_req_search_service = {{0}};
  bt_req_search_service.service_uuid = service_uuid;
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_search_service.dev_addr, remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SEARCH_SERVICE, &bt_req_search_service, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_sniff_mode
 * @brief      request the local device enter into sniff mode
 * @param[in]  remote_dev_addr, remote device address
 * @param[in]  sniff_max_intv,  Sniff maximum Interval
 * @param[in]  sniff_min_intv, Sniff Minimum Interval
 * @param[in]  sniff_attempt, Sniff Attempt
 * @param[in]  sniff_tout, Sniff timeout
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to start inquiry.
 */
int32_t rsi_bt_sniff_mode(uint8_t *remote_dev_addr, uint16_t sniff_max_intv, uint16_t sniff_min_intv, uint16_t sniff_attempt, uint16_t sniff_tout)
{
  rsi_bt_req_sniff_mode_t bt_req_sniff_mode = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_sniff_mode.dev_addr, (int8_t *)remote_dev_addr);
  bt_req_sniff_mode.sniff_max_intv = sniff_max_intv;
  bt_req_sniff_mode.sniff_min_intv = sniff_min_intv;
  bt_req_sniff_mode.sniff_attempt  = sniff_attempt;
  bt_req_sniff_mode.sniff_tout     = sniff_tout;

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SNIFF_MODE, &bt_req_sniff_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_sniff_exit_mode
 * @brief      request the local device enter into sniff mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to start inquiry.
 */
int32_t rsi_bt_sniff_exit_mode(uint8_t *remote_dev_addr)
{
  rsi_bt_req_sniff_exit_mode_t bt_req_sniff_exit_mode = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_sniff_exit_mode.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SNIFF_EXIT_MODE, &bt_req_sniff_exit_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_sniff_subrating_mode
 * @brief      request the local device enter into sniff mode
 * @param[in]  remote_dev_addr, remote device address
 * @param[in]  max_latency, Maximum Latency 
 * @param[in]  min_remote_tout, Minimum remote timeout
 * @param[in]  min_local_tout, Minimum local timeout
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to start inquiry.
 */
int32_t rsi_bt_sniff_subrating_mode(uint8_t *remote_dev_addr, uint16_t max_latency, uint16_t min_remote_tout, uint16_t min_local_tout)
{
  rsi_bt_req_sniff_subrating_mode_t bt_req_sniff_subrating_mode = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_sniff_subrating_mode.dev_addr, (int8_t *)remote_dev_addr);
  bt_req_sniff_subrating_mode.max_latency     = max_latency;
  bt_req_sniff_subrating_mode.min_remote_tout = min_remote_tout;
  bt_req_sniff_subrating_mode.min_local_tout  = min_local_tout;

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SNIFF_SUBRATING_MODE, &bt_req_sniff_subrating_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_set_ssp_mode
 * @brief      request the local device to enable/disable SSP mode
 * @param[in]  pair_mode, enable/disable the SSP mode
 * @param[in]  IOcapability, io capability of local device
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to start inquiry.
 */
int32_t rsi_bt_set_ssp_mode(uint8_t pair_mode, uint8_t IOcapability)
{
  rsi_bt_req_set_ssp_mode_t bt_req_set_ssp_mode = {0};
  
  bt_req_set_ssp_mode.pair_mode    = pair_mode;
  bt_req_set_ssp_mode.IO_capability = IOcapability; 

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_SSP_MODE, &bt_req_set_ssp_mode, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_set_eir_data
 * @brief      request the local device to set the eir data
 * @param[in]  none
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function requests the local device to set advertising information
 * */
int32_t rsi_bt_set_eir_data(uint8_t *data, uint16_t data_len)
{
  rsi_bt_set_eir_data_t  eirdata = {0};


  eirdata.fec_required = 0;
  eirdata.data_len = RSI_MIN ((data_len + 1), sizeof (eirdata.eir_data)); // adding 1byte(for fec_required variable)

  memcpy (eirdata.eir_data, data, eirdata.data_len);

  //! Send stop advertise command
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_EIR, &eirdata, NULL);

}
/*==============================================*/
/**
 * @fn         rsi_bt_per_rx
 * @brief      start the bt-classic per rx in the controller
 * @param[in]  none
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to configure the per receive parameters in the controller and start/stop the PER.
 * */
int32_t rsi_bt_per_rx(uint32_t *bt_perrx)
{
   return rsi_bt_driver_send_cmd(RSI_BT_REQ_PER_CMD, bt_perrx, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_per_tx
 * @brief      start the bt-classic per tx in the controller
 * @param[in]  none
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to configure the per transmit parameters in the controller and start/stop the PER.
 * */
int32_t rsi_bt_per_tx(uint32_t *bt_pertx)
{
   return rsi_bt_driver_send_cmd(RSI_BT_REQ_PER_CMD, bt_pertx, NULL);
}

/*==============================================*/
/*
 * @fn         rsi_bt_add_device_id
 * @brief      To add device identification for local device
 * @param[in]  Specification ID
 *             Vendor ID
 *             Product ID
 *             Version
 *             Primary record
 *             Vendor ID source
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the device id in the SDP.
  
-------------------------------------------------------------------------------------------------*/
int32_t rsi_bt_add_device_id(uint16_t spec_id,
                             uint16_t vendor_id,
                             uint16_t product_id,
                             uint16_t version,
                             int primary_rec,
                             uint16_t vendor_id_source)
{
  rsi_bt_req_add_device_id_t add_device_id = {0};
 
  add_device_id.spec_id = spec_id;
  add_device_id.vendor_id = vendor_id;
  add_device_id.product_id = product_id;
  add_device_id.version = version;
  add_device_id.primary_rec = primary_rec;
  add_device_id.vendor_id_source = vendor_id_source;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_ADD_DEVICE_ID,&add_device_id,NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_ptt_req
 * @brief      start the bt-classic LP HP transitions in the controller
 * @param[in]  none
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to configure the LP HP transitions in the controller.
 * */
int32_t rsi_bt_ptt_req(uint8_t mode)
{
    rsi_bt_vendor_ptt_type_t  bt_lphpchange;
    
    bt_lphpchange.opcode[0] = (BT_VENDOR_PTT_TYPE_CMD_OPCODE & 0xFF);
    bt_lphpchange.opcode[1] = ((BT_VENDOR_PTT_TYPE_CMD_OPCODE >> 8) & 0xFF);
    bt_lphpchange.len=1;
    bt_lphpchange.mode = mode;
   return rsi_bt_driver_send_cmd(RSI_BT_VENDOR_SPECIFIC, &bt_lphpchange, NULL);
}
#endif
