/**
 * @file     rsi_bt_pbap_apis.c
 * @version  0.1
 * @date     03 feb 2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains BT PBAP API's which needs to be called from application
 *
 *  @section Description  This file contains BT PBAP API's called from application
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
 * @fn         rsi_bt_pbap_init
 * @brief      sets profile mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the profile mode.
 */
int32_t rsi_bt_pbap_init(void)
{ 
  rsi_bt_req_profile_mode_t bt_req_pbap_init = {RSI_PBAP_PROFILE_BIT};
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_PROFILE_MODE, &bt_req_pbap_init, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_pbap_connect
 * @brief      requests PBAP connection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
		  ../bt_ble/rsi_bt_hfp_apis.o
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to initiate PBAP connection.
 * */
int32_t rsi_bt_pbap_connect(uint8_t *remote_dev_addr)
{
  rsi_bt_req_pbap_connect_t bt_req_pbap_connect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_pbap_connect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_PBAP_CONNECT, &bt_req_pbap_connect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_pbap_disconnect
 * @brief      requests PBAP disconnection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for PBAP disconnection.
 * */
int32_t rsi_bt_pbap_disconnect(uint8_t *remote_dev_addr)
{
  rsi_bt_req_pbap_disconnect_t bt_req_pbap_disconnect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_pbap_disconnect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_PBAP_DISCONNECT, &bt_req_pbap_disconnect, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_pbap_contacts
 * @brief      requests PBAP contacts.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for PBAP disconnection.
 * */
int32_t rsi_bt_pbap_contacts(uint8_t *remote_dev_addr,uint8_t folder_id, uint8_t nbr_contacts,uint8_t start_ix)
{
  rsi_bt_req_pbap_contacts_t bt_req_pbap_contacts = {{0}};
  bt_req_pbap_contacts.folder_id = folder_id;
  bt_req_pbap_contacts.start_ix = start_ix;
  bt_req_pbap_contacts.nbr_contacts = nbr_contacts;

  rsi_ascii_dev_address_to_6bytes_rev(bt_req_pbap_contacts.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_PBAP_CONTACTS, &bt_req_pbap_contacts, NULL);
}



#endif
