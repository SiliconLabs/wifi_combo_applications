/**
 * @file     rsi_bt_iap_apis.c
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

/*==============================================*/
/**
 * @fn         rsi_bt_iap_init
 * @brief      sets profile mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the profile mode.
 */
int32_t rsi_bt_iap_init(void)
{ 
  rsi_bt_req_profile_mode_t bt_req_iap_init = {32};
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_PROFILE_MODE, &bt_req_iap_init, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_iap_conn
 * @brief      request the iap connection                           
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 */

int32_t rsi_bt_iap_conn(uint8_t *remote_dev_addr, uint8_t version)
{
  rsi_bt_req_iap_connect_t bt_req_iap_connect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_iap_connect.dev_addr, (int8_t *)remote_dev_addr);
  bt_req_iap_connect.version = version;

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP_CONN, &bt_req_iap_connect, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_iap_disconn
 * @brief      request the IAP disconnection from the remote device
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 */

int32_t rsi_bt_iap_disconn(uint8_t *remote_dev_addr, uint8_t version)
{
  rsi_bt_req_iap_disconnect_t bt_req_iap_disconnect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_iap_disconnect.dev_addr, (int8_t *)remote_dev_addr);
  bt_req_iap_disconnect.version = version;

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP_DISCONN, &bt_req_iap_disconnect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_iap_set_accessory_info
 * @brief      sets the accessory information of the redpine module
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 */

int32_t rsi_bt_iap_set_accessory_info(uint8_t type, uint8_t length, uint8_t *data)
{
  rsi_bt_req_iap_accessory_info_t bt_req_accessory_info = {{0}};

  bt_req_accessory_info.info_type = type;
  bt_req_accessory_info.info_len  = length;
  memcpy(&(bt_req_accessory_info.info_data),(uint8_t *) data, length);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP_SET_ACCESSORY_INFO, &bt_req_accessory_info, NULL);

}
/*==============================================*/
/**
 * @fn         rsi_bt_iap_set_voice_over
 * @brief      activates the voice over in the remote apple device 
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 */

int32_t rsi_bt_iap_set_voice_over(uint8_t mode, uint8_t restore)
{
  rsi_bt_req_iap_set_voice_over_t bt_req_set_voice_over = {{0}};
  
  bt_req_set_voice_over.mode = mode;
  bt_req_set_voice_over.restore  = restore;

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP_SET_ACCESSORY_INFO, &bt_req_set_voice_over, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_iap_set_protocol_type
 * @brief      sets the IAP protocol type                          
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 */

int32_t rsi_bt_iap_set_protocol_type(uint8_t type)
{
  rsi_bt_req_iap_set_proto_t bt_req_set_proto;
  bt_req_set_proto.type = type;

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP_SET_PROTOCOL_TYPE, &bt_req_set_proto, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_iap_find_protocol_type
 * @brief      Queries for the protocol type                       
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 */

int32_t rsi_bt_iap_find_protocol_type(uint8_t *resp)
{

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP_FIND_PROTOCOL_TYPE, NULL, resp);
}
/*==============================================*/
/**
 * @fn         rsi_bt_iap_send_control_session_data
 * @brief      sends the control session data                        
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 */


int32_t rsi_bt_iap_send_control_session_data(rsi_bt_iap_payload_t *payload)
{

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP2_CONTROL_SESSION_DATA_TX, payload, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_iap1_identification
 * @brief       sends the IAP 1 identification                      
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the BT Module in discovery mode.
 */

int32_t rsi_bt_iap1_identification()
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP1_IDENTIFICATION, NULL, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_iap2_identification
 * @brief      sends the IAP 2 identitification request                          
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the BT Module in discovery mode.
 */

int32_t rsi_bt_iap2_identification()
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP2_IDENTIFICATION, NULL, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_iap1_device_authentication
 * @brief     
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the BT Module in discovery mode.
 */

int32_t rsi_bt_iap1_device_authentication()
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP1_DEVICE_AUTHENTICATION, NULL, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_iap_update_send_cmds_info
 * @brief      Updates the list of ID's  that accessory will send to Apple device
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to update the ID's list in Identification messages
 */

int32_t rsi_bt_iap2_update_send_cmds_info(rsi_bt_iap_cmds_info_t *cmd_list)
{

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP2_SEND_CMD_LIST, cmd_list, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_iap_update_recv_cmds_info
 * @brief      Updates the list of ID's  that accessory will receive from Apple device
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to update the ID's list in Identification messages
 */
int32_t rsi_bt_iap2_update_recv_cmds_info(rsi_bt_iap_cmds_info_t *cmd_list)
{

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP2_RECV_CMD_LIST, cmd_list, NULL);
}


/*==============================================*/
/**
 * @fn         rsi_bt_iap2_Init_File_Transfer
 * @brief      Initilializes file transfer 
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to update the ID's list in Identification messages
 */
int32_t rsi_bt_iap2_Init_File_Transfer()
{

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP2_INIT_FILE_TRANSFER, NULL, NULL);
}


/*==============================================*/
/**
 * @fn         rsi_bt_iap2_deInit_File_Transfer
 * @brief      De Initilializes file transfer
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to update the ID's list in Identification messages
 */
int32_t rsi_bt_iap2_deInit_File_Transfer()
{

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP2_DE_INIT_FILE_TRANSFER, NULL, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_iap2_Send_File_Transfer_State
 * @brief      Sends the File transfer state like success/Failure/setup/Start/Pause 
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 */
int32_t rsi_bt_iap2_Send_File_Transfer_State(rsi_bt_iap_File_transfer_info_t *payload_info)
{

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP2_SEND_FILE_TRANSFER_STATE, payload_info, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_iap2_Send_File_Transfer_data
 * @brief      Sends the File Trasnfer data                                                
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to update the ID's list in Identification messages
 */
int32_t rsi_bt_iap2_Send_File_Transfer_data(rsi_bt_iap_File_transfer_info_t *payload_info)
{

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_IAP2_SEND_FILE_TRANSFER_DATA, payload_info, NULL);
}

#endif
