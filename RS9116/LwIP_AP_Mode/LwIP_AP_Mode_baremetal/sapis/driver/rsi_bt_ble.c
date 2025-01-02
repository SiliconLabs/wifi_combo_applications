/**
 * @file     rsi_bt.c
 * @version  0.1
 * @date     02 Sep 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains functions related to Bluetooth Low-Energy(BLE) control block
 *
 *  @section Description  This file contains functions related to BLE control block
 *
 *
 */

/**
 * Include files
 * */
#if  (defined(RSI_BT_ENABLE)	|| defined(RSI_BLE_ENABLE))		
#include "rsi_driver.h"
#include "rsi_bt.h"
#include "rsi_hid.h"
#include "rsi_bt_common.h"
#include "rsi_ble.h"
#include "rsi_bt_config.h"
#include "rsi_ble_config.h"
#include "stdio.h"

#ifdef SAPIS_BT_STACK_ON_HOST
#include "bt_stack_interface.h"
#endif

#include "rsi_ant.h"
#include "rsi_bt_sbc_codec.h"
/*
 * Global Variables
 * */

/**
 * @fn          uint16_t rsi_bt_get_proto_type(uint16 rsp_type, rsi_bt_cb_t **bt_cb) 
 * @brief       Gets BT protocol using the packet type
 * @param[in]   pkt, pointer to packet 
 * @param[out]  Returns the protocol type (BT COMMON / BT classic / BLE )
 * @return      void
 *
 * @section description
 * This function determines the BT protocol (BT COMMON / BT classic / BLE ) using the packet type
 *
 */
uint16_t rsi_bt_get_proto_type(uint16_t rsp_type, rsi_bt_cb_t **bt_cb)
{
  uint16_t return_value = 0xFF;
  //static uint16_t local_prototype;
  //static rsi_bt_cb_t *local_cb;


  if (rsp_type == RSI_BT_EVT_DISCONNECTED) 
  {
    /* if (!((rsi_driver_cb->common_cb->ps_coex_mode & RSI_BT_BLE_MODE_BITS) == RSI_BT_DUAL_MODE)) {
     *bt_cb = local_cb;
     return local_prototype;
     } else { */
#ifdef RSI_BT_ENABLE
    if (rsi_driver_cb->bt_common_cb->dev_type == RSI_BT_CLASSIC_DEVICE) {
      return_value = RSI_PROTO_BT_CLASSIC;
      *bt_cb = rsi_driver_cb->bt_classic_cb;
    }
#endif
#ifdef RSI_BLE_ENABLE
    if (rsi_driver_cb->bt_common_cb->dev_type == RSI_BT_LE_DEVICE) {
      return_value = RSI_PROTO_BLE;
      *bt_cb = rsi_driver_cb->ble_cb;
    }
#endif			

    return return_value;
    //}
  }

  //! Determine the protocol type by looking at the packet type
  if ((rsp_type == RSI_BT_EVENT_CARD_READY) ||
      ((rsp_type >= RSI_BT_SET_LOCAL_NAME) &&
       (rsp_type <= RSI_BT_GET_LOCAL_DEV_ADDR))||
      ((rsp_type >= RSI_BT_REQ_INIT ) &&
       (rsp_type <= RSI_BT_SET_ANTENNA_SELECT)) ||
      (rsp_type == RSI_BT_SET_FEATURES_BITMAP) ||
      (rsp_type == RSI_BT_SET_ANTENNA_TX_POWER_LEVEL) ||
      (rsp_type == RSI_BLE_ONLY_OPER_MODE) ||
      (rsp_type == RSI_BLE_REQ_PWRMODE) ||
      (rsp_type == RSI_BLE_REQ_SOFTRESET) ||
      (rsp_type == RSI_BT_REQ_PER_CMD) ||
      (rsp_type == RSI_BT_VENDOR_SPECIFIC ))
  {
    return_value = RSI_PROTO_BT_COMMON;
    *bt_cb = rsi_driver_cb->bt_common_cb;
  }
#ifdef RSI_BT_ENABLE				
  else if (((rsp_type >= RSI_BT_REQ_SET_PROFILE_MODE) &&
        (rsp_type <= RSI_BT_REQ_PBAP_CONTACTS)) ||
      ((rsp_type >= RSI_BT_REQ_HID_CONNECT) && 
       (rsp_type <= RSI_BT_REQ_HID_SDP_ATT_INIT)) ||
      (rsp_type == RSI_BT_REQ_LINKKEY_REPLY) ||
      ((rsp_type >= RSI_BT_REQ_PER_TX) &&
       (rsp_type <= RSI_BT_REQ_CW_MODE)) ||
      ((rsp_type >= RSI_BT_REQ_SNIFF_MODE) && 
       (rsp_type <= RSI_BT_REQ_SET_SSP_MODE)) ||
      (rsp_type == RSI_BT_REQ_SET_EIR) ||
      ((rsp_type >= RSI_BT_REQ_A2DP_CLOSE) &&
       (rsp_type <= RSI_BT_REQ_ENABLE_DEVICE_UNDER_TESTMODE)) ||
      (rsp_type == RSI_BT_REQ_A2DP_GET_CONFIG) || 
      (rsp_type == RSI_BT_REQ_A2DP_SET_CONFIG) ||
      ((rsp_type >= RSI_BT_REQ_AVRCP_SET_ABS_VOL) &&
       (rsp_type <= RSI_BT_REQ_GATT_CONNECT)) ||
      ((rsp_type >= RSI_BT_EVT_ROLE_CHANGE_STATUS) &&
       (rsp_type <= RSI_BT_EVT_AVDTP_STATS)) ||
      ((rsp_type >= RSI_BT_EVT_SPP_RECEIVE) &&
       (rsp_type <= RSI_BT_EVT_SPP_DISCONNECTED)) ||
      ((rsp_type >= RSI_BT_EVT_A2DP_CONNECTED) &&
       (rsp_type <= RSI_BT_EVT_A2DP_CODEC_CFG)) || 
      ((rsp_type >= RSI_BT_EVT_AVRCP_CONNECTED) &&
       (rsp_type <= RSI_BT_EVT_AVRCP_SET_ABS_VOL)) || 
      ((rsp_type >= RSI_BT_EVT_HFP_CONN) &&
       (rsp_type <= RSI_BT_EVT_HFP_CALLHELDSTATUS)) || 
      ((rsp_type >= RSI_BT_EVT_PBAP_CONN) &&
       (rsp_type <= RSI_BT_EVT_PBAP_DATA)) ||
      (rsp_type == RSI_BT_EVENT_HID_CONN) ||
      (rsp_type == RSI_BT_EVENT_HID_RXDATA) )
      {
        return_value = RSI_PROTO_BT_CLASSIC;
        *bt_cb = rsi_driver_cb->bt_classic_cb;
      }
  else if((rsp_type == RSI_BT_REQ_IAP_SET_ACCESSORY_INFO)
      ||(rsp_type == RSI_BT_REQ_IAP_SET_PROTOCOL_TYPE)
      ||(rsp_type == RSI_BT_REQ_IAP1_IDENTIFICATION)
      ||(rsp_type == RSI_BT_REQ_IAP2_IDENTIFICATION)
      ||(rsp_type == RSI_BT_REQ_IAP_FIND_PROTOCOL_TYPE)
      ||(rsp_type == RSI_BT_EVENT_IAP_DISCONN)
      ||(rsp_type == RSI_BT_EVENT_IAP_ACC_AUTH_STARTED)
      ||(rsp_type == RSI_BT_EVENT_IAP_ACC_AUTH_FAILED)
      ||(rsp_type == RSI_BT_EVENT_IAP2_AUTH_START)              
      ||(rsp_type == RSI_BT_EVENT_IAP2_AUTH_COMPLETE)           
      ||(rsp_type == RSI_BT_EVENT_IAP2_AUTH_FAILED   )          
      ||(rsp_type == RSI_BT_EVENT_IAP2_IDENTIFICATION_START)    
      ||(rsp_type == RSI_BT_EVENT_IAP2_IDENTIFICATION_COMPLETE) 
      ||(rsp_type == RSI_BT_EVENT_IAP2_IDENTIFICATION_REJECT)
      ||(rsp_type == RSI_BT_EVENT_IAP_ACC_AUTH_COMPLETE)
      ||(rsp_type == RSI_BT_REQ_IAP2_CONTROL_SESSION_DATA_TX)
      ||(rsp_type == RSI_BT_EVENT_IAP2_RX_DATA)
      ||(rsp_type == RSI_BT_REQ_IAP2_RECV_CMD_LIST)                
      ||(rsp_type == RSI_BT_REQ_IAP2_SEND_CMD_LIST)                
      ||(rsp_type == RSI_BT_REQ_IAP2_INIT_FILE_TRANSFER)
      ||(rsp_type == RSI_BT_REQ_IAP2_DE_INIT_FILE_TRANSFER)
      ||(rsp_type == RSI_BT_REQ_IAP2_SEND_FILE_TRANSFER_STATE)
      ||(rsp_type == RSI_BT_REQ_IAP2_SEND_FILE_TRANSFER_DATA)
      ||(rsp_type == RSI_BT_EVENT_IAP2_RX_FILE_TRANSFER_STATE)
      ||(rsp_type == RSI_BT_EVENT_IAP2_RX_FILE_TRANSFER_DATA)
      || (rsp_type ==RSI_BT_EVENT_IAP_CONN ))
  {
    return_value = RSI_PROTO_BT_CLASSIC;
    *bt_cb = rsi_driver_cb->bt_classic_cb;
  }
#endif
#ifdef RSI_BLE_ENABLE	
  else if (((rsp_type >= RSI_BLE_REQ_ADV) &&
        (rsp_type <= RSI_BLE_REQ_EXECUTE_WRITE )) ||
      ((rsp_type >= RSI_BLE_ADD_SERVICE) &&
       (rsp_type <= RSI_BLE_CMD_INDICATE)) ||
      (rsp_type  == RSI_BLE_SET_ADVERTISE_DATA) ||
      ((rsp_type >= RSI_BLE_GET_LE_PING) &&
       (rsp_type <= RSI_BLE_CMD_READ_RESP)) ||
      (rsp_type == RSI_BLE_SET_SCAN_RESPONSE_DATA) ||
      ((rsp_type >= RSI_BLE_LE_WHITE_LIST) &&
       (rsp_type <= RSI_BLE_CBFC_DISCONN)) ||
      ((rsp_type >= RSI_BLE_LE_LTK_REQ_REPLY) &&
       (rsp_type <= RSI_BLE_PER_RX_MODE)) ||
      (rsp_type == RSI_BLE_CMD_ATT_ERROR) ||
      ((rsp_type >= RSI_BLE_REQ_PROFILES_ASYNC) &&
       (rsp_type <= RSI_BLE_EXECUTE_LONGDESCWRITE_ASYNC)) ||
      (rsp_type == RSI_BLE_SET_SMP_PAIRING_CAPABILITY_DATA) ||
      (rsp_type == RSI_ANT_CMD) ||
      ((rsp_type >= RSI_BLE_EVENT_GATT_ERROR_RESPONSE) &&
       (rsp_type <= RSI_BLE_EVENT_LE_MORE_DATA_REQ)) )
  {

    return_value = RSI_PROTO_BLE;
    *bt_cb = rsi_driver_cb->ble_cb;
  }
#endif
  /*if (return_value != RSI_PROTO_BT_COMMON)
  {
    local_prototype = return_value;
    local_cb = *bt_cb;
  }*/

  return return_value;
}

/**
 * @fn void rsi_bt_tx_done(rsi_bt_cb_t *bt_cb)
         
 * @brief       Handles protocol specific bt data transfer completion
 * @param[in]   bt_cb BT common/BT classic /BLE control block
 * @param[out]  None
 * @return      void
 *
 * @section description
 * This function handles the protocol specific bt data transfer completion
 *
 */
void rsi_bt_tx_done(rsi_bt_cb_t *bt_cb)
{
  //! If the command is not a synchronous / blocking one
  if(!bt_cb->sync_rsp)
  {
    //! Set bt_common status as success
    rsi_bt_set_status(bt_cb, RSI_SUCCESS);

    //! Post the semaphore which is waiting on bt_common_sem
    rsi_semaphore_post(&bt_cb->bt_sem);
  }
}

/**
 * @fn          void rsi_bt_common_tx_done(rsi_pkt_t *pkt)
 * @brief       Handles BT data transfer completion
 * @param[in]   pkt, pointer to packet 
 * @param[out]  None
 * @return      void
 *
 * @section description
 * This function handles BT data transfer completion
 *
 */
void rsi_bt_common_tx_done(rsi_pkt_t *pkt)
{
  uint8_t *host_desc = NULL;
  uint8_t protocol_type = 0;
  uint16_t rsp_type = 0;
  rsi_bt_cb_t *bt_cb    = NULL;

  //! Get Host Descriptor
  host_desc = pkt->desc;

  //! Get Command response Type
  rsp_type = rsi_bytes2R_to_uint16(host_desc + RSI_BT_RSP_TYPE_OFFSET);

  //! Get the protocol Type
  protocol_type = rsi_bt_get_proto_type(rsp_type, &bt_cb);

  if(protocol_type == 0xFF)
  {
    return;
  }

  //! Call BT transfer done
  rsi_bt_tx_done(bt_cb);
}

/**
 *
 * @fn          uint32_t rsi_get_bt_state(rsi_bt_cb_t *bt_cb)
        
 * @brief      Return BT status
 * @param[in]  bt_cb BT control block
 * @param[out] None
 * @return     status
 *
 *
 * @section description
 * This function returns BT status
 *
 *
 */
uint32_t rsi_get_bt_state(rsi_bt_cb_t *bt_cb)
{
  return bt_cb->state;
}

/*==============================================*/
/**
 * @fn         void rsi_bt_set_status(rsi_bt_cb_t *bt_cb, int32_t status)

 * @brief       Sets the BT status
 * @param[in]   bt_cb, BT control block
 * @param[in]   status, status value to be set 
 * @param[out]  None
 * @return      void
 *
 * @section description
 * This function sets BT status
 *
 */
void rsi_bt_set_status(rsi_bt_cb_t *bt_cb, int32_t status)
{
  bt_cb->status = status;
}


/*==============================================*/
/**
 * @fn          uint32_t rsi_bt_get_status(rsi_bt_cb_t *bt_cb)
 * @brief       Gets the bt status
 * @param[in]   bt_cb BT control block
 * @param[out]  None
 * @return      void
 *
 * @section description
 * This function gets bt status
 *
 */
uint32_t rsi_bt_get_status(rsi_bt_cb_t *bt_cb)
{
  return bt_cb->status;
}


/**
 * @fn           int32_t rsi_driver_process_bt_resp (rsi_bt_cb_t *bt_cb, rsi_pkt_t *pkt, void (*rsi_bt_async_callback_handler)  
 * @brief       Processes BT receive commands
 * @param[in ]  bt_cb BT control block
 * @param[in]   pkt, pointer to received RX packet
 * @param[in]   bt_aync_callback_handler
 * @param[out]  None
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function processes BT RX packets
 *
 */
int32_t rsi_driver_process_bt_resp (rsi_bt_cb_t *bt_cb, rsi_pkt_t *pkt, 
    void (*rsi_bt_async_callback_handler) (rsi_bt_cb_t *cb, uint16_t type, uint8_t *data, uint16_t length) )
{
  uint16_t rsp_type = 0;
  int16_t  status = RSI_SUCCESS;
  uint8_t *host_desc = NULL;
  uint8_t  *payload;
  uint16_t  payload_length;

  //! Get Host Descriptor
  host_desc = pkt->desc;

  //! Get Command response Type
  rsp_type = rsi_bytes2R_to_uint16(host_desc + RSI_BT_RSP_TYPE_OFFSET);

  //! Get Payload start pointer
  payload = pkt->data;

  //! Get Payload length
  payload_length = (rsi_bytes2R_to_uint16(host_desc) & 0xFFF);

  //! Get Status
  status  = rsi_bytes2R_to_uint16(host_desc + RSI_BT_STATUS_OFFSET);

  //! Update the status in bt_cb
  rsi_bt_set_status(bt_cb, status);

  //! Check bt_cb for any task is waiting for response
  if (bt_cb->expected_response_type == rsp_type)
  {
    //! Clear expected response type
    bt_cb->expected_response_type = 0;

    //! Copy the expected response to response structure/buffer, if any, passed in API 
    if (bt_cb->expected_response_buffer != NULL)
    {
      //! If (payload_length <= RSI_BLE_GET_MAX_PAYLOAD_LENGTH(expected_response_type)) //TODO Give the proper error code
      memcpy(bt_cb->expected_response_buffer, payload, payload_length);

      //! Save expected_response pointer to a local variable, since it is being cleared below
      payload = bt_cb->expected_response_buffer;

      //! Clear the expected response pointer
      bt_cb->expected_response_buffer = NULL;
    }

    //! Check if it is sync response
    if (bt_cb->sync_rsp)
    {
      //! Clear sync rsp variable
      bt_cb->sync_rsp = 0;

      //! Signal the bt semaphore
      rsi_semaphore_post(&bt_cb->bt_sem);
    }
    else
    {
      if (rsi_bt_async_callback_handler != NULL) {
        //! Call callbacks handler
        rsi_bt_async_callback_handler(bt_cb, rsp_type, payload, payload_length);
      }
		}
	}
  else
  {
      if (rsi_bt_async_callback_handler != NULL) {
    //! Call callbacks handler
    rsi_bt_async_callback_handler(bt_cb, rsp_type, payload, payload_length);
      }
  }

  return status;
}


/**
 * @fn          int32_t rsi_driver_process_bt_resp_handler(rsi_pkt_t *pkt)
 * @brief       Processes BT receive commands
 * @param[in]   pkt, pointer to received RX packet 
 * @param[out]  None
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function processes BT RX packets
 *
 */
uint16_t rsi_driver_process_bt_resp_handler(rsi_pkt_t *pkt)
{
  uint8_t *host_desc = NULL;
  uint8_t protocol_type = 0;
  uint16_t rsp_type = 0;
  //uint16_t rsp_len = 0;
  int16_t  status = RSI_SUCCESS;
  rsi_bt_cb_t *bt_cb    = NULL;
  rsi_ble_event_disconnect_t *temp_data = NULL;

  //! Get Host Descriptor
  host_desc = pkt->desc;

  //! Get Command response Type
  rsp_type = rsi_bytes2R_to_uint16(host_desc + RSI_BT_RSP_TYPE_OFFSET);
  //rsp_len = rsi_bytes2R_to_uint16(host_desc + RSI_BT_RSP_LEN_OFFSET) & RSI_BT_RSP_LEN_MASK;

  if (/*((rsi_driver_cb->common_cb->ps_coex_mode & RSI_BT_BLE_MODE_BITS) == RSI_BT_DUAL_MODE) &&  */
      (rsp_type == RSI_BLE_EVENT_DISCONNECT)) {

    //rsi_driver_cb->bt_common_cb->dev_type = ((rsi_ble_event_disconnect_t *)pkt->data)->dev_type;
       temp_data =  (rsi_ble_event_disconnect_t *)pkt->data;
       rsi_driver_cb->bt_common_cb->dev_type = temp_data->dev_type;
  }

  //! Get the protocol Type
  protocol_type = rsi_bt_get_proto_type(rsp_type, &bt_cb);

  if(protocol_type == 0xFF)
  {
    return 0;
  }
  //! Call the corresponding protocol process rsp handler
  if (protocol_type == RSI_PROTO_BT_COMMON)
  {
    //! Call BT common process rsp handler
    status = rsi_driver_process_bt_resp(bt_cb, pkt, NULL);
  }
#ifdef RSI_BT_ENABLE
  else if (protocol_type == RSI_PROTO_BT_CLASSIC)
  {
    //! Call BT classic process response handler
    status = rsi_driver_process_bt_resp(bt_cb, pkt, rsi_bt_callbacks_handler);
  }
#endif    
#ifdef RSI_BLE_ENABLE
  else
  {
    //! Call BLE process response handler
    status = rsi_driver_process_bt_resp(bt_cb, pkt, rsi_ble_callbacks_handler);
  }
#endif
  return status;
}

/**
 * @fn         int8_t rsi_bt_cb_init(rsi_bt_cb_t *bt_cb)
 * @brief      Initializes bt control block structure
 * @param[in]  bt_cb, pointer to bt_cb structure
 * @param[out] None
 * @return     None
 *
 * @section description
 * This function initializes bt control block structure
 *
 */
int8_t rsi_bt_cb_init(rsi_bt_cb_t *bt_cb)
{
  int8_t retval = RSI_ERR_NONE;

  //! validate input parameter
  if (bt_cb == NULL)
  {
    return RSI_ERROR_INVALID_PARAM;
  }

  //! initialize bt control block with default values
  bt_cb->state = 0;	//! TODO
  bt_cb->status = 0;

  //! Create bt mutex
  bt_cb->expected_response_type = 0;
  bt_cb->expected_response_buffer = NULL;

  //! Create bt semaphore
  rsi_semaphore_create(&bt_cb->bt_sem, 0);
  bt_cb->app_buffer = 0;

  return retval;
}

uint16_t rsi_bt_global_cb_init(rsi_driver_cb_t *driver_cb, uint8_t *buffer)
{
  rsi_bt_global_cb_t *bt_global_cb = driver_cb->bt_global_cb;
  uint16_t  total_size = 0;

#ifdef RSI_BLE_ENABLE
  bt_global_cb->ble_specific_cb = (rsi_ble_cb_t*)buffer;

  driver_cb->ble_cb->bt_global_cb = bt_global_cb;
  total_size += sizeof (rsi_ble_cb_t);
#endif

#ifdef RSI_BT_ENABLE
  bt_global_cb->bt_specific_cb = (rsi_bt_classic_cb_t*)(buffer + total_size);

  driver_cb->bt_classic_cb->bt_global_cb = bt_global_cb;
  total_size += sizeof (rsi_bt_classic_cb_t);
#endif

  return total_size;
}

/**
 * @fn         rsi_bt_common_init
 * @brief      Waits for BT card ready
 * @param[in]  none
 * @return     none 
 * @section description
 * */

void rsi_bt_common_init(void)
{
  //! Get bt_common_cb structure pointer
  rsi_bt_cb_t *bt_common_cb = rsi_driver_cb->bt_common_cb;

  //! take lock on ble control block
  rsi_mutex_lock(&rsi_driver_cb->bt_single_mutex);

  //! Save expected response type
  bt_common_cb->expected_response_type = RSI_BT_EVENT_CARD_READY;
  bt_common_cb->sync_rsp = 1;

  //! Wait on BLE semaphore
  rsi_semaphore_wait(&bt_common_cb->bt_sem, RSI_WAIT_FOREVER);

  //! BT card ready is received

  //! release lock
  rsi_mutex_unlock(&rsi_driver_cb->bt_single_mutex);
}


#ifdef RSI_BT_ENABLE
/**
 * @fn          rsi_bt_register_gap_callbacks 
 * @brief       This function registers the GAP callbacks
 * @param[in]   rsi_bt_on_role_change_t               bt_on_role_change_status_event                  : Connection status callback
 * @param[in]   rsi_bt_on_connect_t                   bt_on_conn_status_event                         : Connection status callback
 * @param[in]   rsi_bt_on_unbond_t                    bt_on_unbond_status                             : Connection status callback
 * @param[in]   rsi_bt_on_disconnect_t                bt_on_disconnect_event                          : Disconnection status callback
 * @param[in]   rsi_bt_on_scan_resp_t                 bt_on_scan_resp_event                           : Scan report callback
 * @param[in]   rsi_bt_on_remote_name_resp_t          bt_on_remote_name_resp_event                    : Remote name report callback
 * @param[in]   rsi_bt_on_passkey_display_t           bt_on_passkey_display_event                     : Passkey display report callback
 * @param[in]   rsi_bt_on_remote_name_request_cancel_t          bt_on_remote_name_request_cancel_event: Remote name request cancel status callback
 * @param[in]   rsi_bt_on_confirm_request_t           bt_on_confirm_request_event                     : Authentication status callback
 * @param[in]   rsi_bt_on_pincode_request_t           bt_on_pincode_request_event                     : Pincode request status callback
 * @param[in]   rsi_bt_on_passkey_request_t           bt_on_passkey_request_event                     : Passkey request status callback
 * @param[in]   rsi_bt_on_inquiry_complete_t          bt_on_inquiry_complete_event                    : Inquiry report callback
 * @param[in]   rsi_bt_on_auth_complete_t             bt_on_auth_complete_event                       : Authentication status callback
 * @param[in]   rsi_bt_on_linkkey_request_t           bt_on_linkkey_request_event                     : Linkkey request report callback
 * @param[in]   rsi_bt_on_ssp_complete_t              bt_on_ssp_complete_event                        : SSP status callback
 * @param[in]   rsi_bt_on_linkkey_save_t              bt_on_linkkey_save_event                        : Linkkey save status callback
 * @param[in]   rsi_bt_on_get_services_t              bt_on_get_services_event                        : Get services report callback
 * @param[in]   rsi_bt_on_search_service_t            bt_on_search_service_event                      : Search service status callback
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the GAP callbacks
 *
 */
void rsi_bt_gap_register_callbacks (
    rsi_bt_on_role_change_t               bt_on_role_change_status_event,     
    rsi_bt_on_connect_t                   bt_on_conn_status_event,     
    rsi_bt_on_unbond_t                    bt_on_unbond_status ,
    rsi_bt_on_disconnect_t                bt_on_disconnect_event       ,
    rsi_bt_on_scan_resp_t                 bt_on_scan_resp_event        ,
    rsi_bt_on_remote_name_resp_t          bt_on_remote_name_resp_event ,
    rsi_bt_on_passkey_display_t           bt_on_passkey_display_event  ,
    rsi_bt_on_remote_name_request_cancel_t          bt_on_remote_name_request_cancel_event,
    rsi_bt_on_confirm_request_t           bt_on_confirm_request_event, 
    rsi_bt_on_pincode_request_t           bt_on_pincode_request_event, 
    rsi_bt_on_passkey_request_t           bt_on_passkey_request_event, 
    rsi_bt_on_inquiry_complete_t          bt_on_inquiry_complete_event,
    rsi_bt_on_auth_complete_t             bt_on_auth_complete_event  ,
    rsi_bt_on_linkkey_request_t           bt_on_linkkey_request_event,
    rsi_bt_on_ssp_complete_t              bt_on_ssp_complete_event   ,
    rsi_bt_on_linkkey_save_t              bt_on_linkkey_save_event   ,
    rsi_bt_on_get_services_t              bt_on_get_services_event   ,
    rsi_bt_on_search_service_t            bt_on_search_service_event ,
    rsi_bt_on_mode_chnage_t               bt_on_mode_change_event    ,
    rsi_bt_on_sniff_subrating_t           bt_on_sniff_subrating_event
    )
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;

  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_on_role_change_status_event         =  bt_on_role_change_status_event    ;
  bt_specific_cb->bt_on_conn_status_event                =  bt_on_conn_status_event    ;
  bt_specific_cb->bt_on_unbond_status                    =  bt_on_unbond_status    ;
  bt_specific_cb->bt_on_disconnect_event                 =  bt_on_disconnect_event      ;
  bt_specific_cb->bt_on_scan_resp_event                  =  bt_on_scan_resp_event       ;//! inq resp
  bt_specific_cb->bt_on_remote_name_resp_event           =  bt_on_remote_name_resp_event;
  bt_specific_cb->bt_on_passkey_display_event            =  bt_on_passkey_display_event ;
  bt_specific_cb->bt_on_remote_name_request_cancel_event =  bt_on_remote_name_request_cancel_event;
  bt_specific_cb->bt_on_confirm_request_event            =  bt_on_confirm_request_event ;
  bt_specific_cb->bt_on_pincode_request_event            =  bt_on_pincode_request_event ;
  bt_specific_cb->bt_on_passkey_request_event            =  bt_on_passkey_request_event ;
  bt_specific_cb->bt_on_inquiry_complete_event           =  bt_on_inquiry_complete_event;
  bt_specific_cb->bt_on_auth_complete_event              =  bt_on_auth_complete_event   ;
  bt_specific_cb->bt_on_linkkey_request_event            =  bt_on_linkkey_request_event ;
  bt_specific_cb->bt_on_ssp_complete_event               =  bt_on_ssp_complete_event    ;
  bt_specific_cb->bt_on_linkkey_save_event               =  bt_on_linkkey_save_event    ;
  bt_specific_cb->bt_on_get_services_event               =  bt_on_get_services_event    ;
  bt_specific_cb->bt_on_search_service_event             =  bt_on_search_service_event  ;
  bt_specific_cb->bt_on_mode_change_event                =  bt_on_mode_change_event;
  bt_specific_cb->bt_on_sniff_subrating_event            =  bt_on_sniff_subrating_event;
  }
/**
 * @fn          rsi_bt_gatt_extended_register_callbacks 
 * @brief       This function registers the GAP extended callbacks
 * @param[in]   rsi_bt_on_gatt_connection_t               bt_on_gatt_connection_event                  : GATT Connection status callback
 * @param[in]   rsi_bt_on_gatt_disconnection_t            bt_on_gatt_disconnection_event               : GATT Disconnection status callback
 * *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the GAP callbacks
 *
 */


void rsi_bt_gatt_extended_register_callbacks (
    rsi_bt_on_gatt_connection_t           bt_on_gatt_connection_event,
    rsi_bt_on_gatt_disconnection_t        bt_on_gatt_disconnection_event)
{

  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;
  
  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_on_gatt_connection_event             =  bt_on_gatt_connection_event;
  bt_specific_cb->bt_on_gatt_disconnection_event          =  bt_on_gatt_disconnection_event;

}


/**
 * @fn          rsi_bt_avdtp_events_register_callbacks 
 * @brief       This function registers the AVDTP Event callbacks
 * @param[in]   rsi_bt_on_avdtp_stats_t               bt_on_avdtp_stats_event                 
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the avdtp stats callbacks
 *
 */

void rsi_bt_avdtp_events_register_callbacks (
    rsi_bt_on_avdtp_stats_t               bt_on_avdtp_stats_event)
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;

  bt_specific_cb->bt_on_avdtp_stats_event     =  bt_on_avdtp_stats_event;
}
/**
 * @fn          rsi_bt_spp_register_callbacks 
 * @brief       This function registers the SPP callbacks
 * @param[in]   rsi_bt_on_spp_connect_t         bt_on_spp_connect_event              : spp connection status callback
 * @param[in]   rsi_bt_on_spp_disconnect_t      bt_on_spp_disconnect_event           : spp disconnection status callback
 * @param[in]   rsi_bt_on_spp_rx_data_t         bt_on_spp_rx_data_event              : spp data transfer status callback
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the spp callbacks
 *
 */
void rsi_bt_spp_register_callbacks (
    rsi_bt_on_spp_connect_t         bt_on_spp_connect_event,
    rsi_bt_on_spp_disconnect_t      bt_on_spp_disconnect_event,
    rsi_bt_on_spp_rx_data_t         bt_on_spp_rx_data_event
    )
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;


  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_on_spp_connect_event     =  bt_on_spp_connect_event;
  bt_specific_cb->bt_on_spp_disconnect_event  =  bt_on_spp_disconnect_event;
  bt_specific_cb->bt_on_spp_rx_data_event     =  bt_on_spp_rx_data_event;
}

/**
 * @fn          rsi_bt_hid_register_callbacks 
 * @brief       This function registers the HID callbacks
 * @param[in]   rsi_bt_on_hid_connect_t         bt_on_hid_connect_event              : hid connection status callback
 * @param[in]   rsi_bt_on_hid_rx_data_t         bt_on_hid_rx_data_event              : hid data transfer status callback
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the hid callbacks
 *
 */
void rsi_bt_hid_register_callbacks (
    rsi_bt_on_hid_connect_t            bt_on_hid_connect_event,
    rsi_bt_on_hid_rx_data_t            bt_on_hid_rx_data_event,
    rsi_bt_on_hid_handshake_t          bt_on_hid_handshake_event,
    rsi_bt_on_hid_control_t            bt_on_hid_control_event,
    rsi_bt_on_hid_get_report_t         bt_on_hid_get_report,
    rsi_bt_on_hid_set_report_t         bt_on_hid_set_report,
    rsi_bt_on_hid_get_protocol_t       bt_on_hid_get_proto,
    rsi_bt_on_hid_set_protocol_t       bt_on_hid_set_proto
    )
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;


  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_on_hid_connect_event     =  bt_on_hid_connect_event;
  bt_specific_cb->bt_on_hid_rx_data_event     =  bt_on_hid_rx_data_event;
  bt_specific_cb->bt_on_hid_handshake_event   =  bt_on_hid_handshake_event;
  bt_specific_cb->bt_on_hid_control_event     =  bt_on_hid_control_event;
  bt_specific_cb->bt_on_hid_get_report        =  bt_on_hid_get_report; 
  bt_specific_cb->bt_on_hid_set_report        =  bt_on_hid_set_report;
  bt_specific_cb->bt_on_hid_get_proto         =  bt_on_hid_get_proto;
  bt_specific_cb->bt_on_hid_set_proto         =  bt_on_hid_set_proto;
}

/**
 * @fn          rsi_bt_a2dp_register_callbacks 
 * @brief       This function registers the A2DP callbacks
 * @param[in]   rsi_bt_on_a2dp_connect_t         bt_on_a2dp_connect_event              : a2dp connection status callback
 * @param[in]   rsi_bt_on_a2dp_disconnect_t      bt_on_a2dp_disconnect_event           : a2dp disconnection status callback
 * @param[in]   rsi_bt_on_a2dp_configure_t       bt_on_a2dp_configure_event            : a2dp configure status callback
 * @param[in]   rsi_bt_on_a2dp_open_t            bt_on_a2dp_open_event                 : a2dp  open callback
 * @param[in]   rsi_bt_on_a2dp_start_t           bt_on_a2dp_start_event                : a2dp start status callback
 * @param[in]   rsi_bt_on_a2dp_suspend_t         bt_on_a2dp_suspend_event              : a2dp suspend status callback
 * @param[in]   rsi_bt_on_a2dp_abort_t           bt_on_a2dp_abort_event                : a2dp abort status callback
 * @param[in]   rsi_bt_on_a2dp_close_t           bt_on_a2dp_close_event                : a2dp close status callback
 * @param[in]   rsi_bt_on_a2dp_encode_data_t     bt_on_a2dp_encode_data_event          : a2dp encode data stauts callback
 * @param[in]   rsi_bt_on_a2dp_pcm_data_t        bt_on_a2dp_pcm_data_event             : a2dp pcm data status callback
 * @param[in]   rsi_bt_on_a2dp_data_req_t        bt_on_a2dp_data_req_event             : a2dp data request status callback
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the a2dp callbacks
 *
 */
void rsi_bt_a2dp_register_callbacks (
    rsi_bt_on_a2dp_connect_t         bt_on_a2dp_connect_event,
    rsi_bt_on_a2dp_disconnect_t      bt_on_a2dp_disconnect_event,
    rsi_bt_on_a2dp_configure_t       bt_on_a2dp_configure_event,
    rsi_bt_on_a2dp_open_t            bt_on_a2dp_open_event,
    rsi_bt_on_a2dp_start_t           bt_on_a2dp_start_event,
    rsi_bt_on_a2dp_suspend_t         bt_on_a2dp_suspend_event,
    rsi_bt_on_a2dp_abort_t           bt_on_a2dp_abort_event,
    rsi_bt_on_a2dp_close_t           bt_on_a2dp_close_event,
    rsi_bt_on_a2dp_encode_data_t     bt_on_a2dp_encode_data_event,
    rsi_bt_on_a2dp_pcm_data_t        bt_on_a2dp_pcm_data_event,
    rsi_bt_on_a2dp_data_req_t        bt_on_a2dp_data_req_event
    )
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;


  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_on_a2dp_connect_event     =  bt_on_a2dp_connect_event;
  bt_specific_cb->bt_on_a2dp_disconnect_event  =  bt_on_a2dp_disconnect_event;
  bt_specific_cb->bt_on_a2dp_configure_event   =  bt_on_a2dp_configure_event;
  bt_specific_cb->bt_on_a2dp_open_event        =  bt_on_a2dp_open_event;
  bt_specific_cb->bt_on_a2dp_start_event       =  bt_on_a2dp_start_event;
  bt_specific_cb->bt_on_a2dp_suspend_event     =  bt_on_a2dp_suspend_event;
  bt_specific_cb->bt_on_a2dp_abort_event       =  bt_on_a2dp_abort_event;
  bt_specific_cb->bt_on_a2dp_close_event       =  bt_on_a2dp_close_event;
  bt_specific_cb->bt_on_a2dp_encode_data_event =  bt_on_a2dp_encode_data_event;
  bt_specific_cb->bt_on_a2dp_pcm_data_event    =  bt_on_a2dp_pcm_data_event;
  bt_specific_cb->bt_on_a2dp_data_req_event    =  bt_on_a2dp_data_req_event;
}

/**
 * @fn          rsi_bt_avrcp_register_callbacks 
 * @brief       This function registers the AVRCP callbacks
 * @param[in]   rsi_bt_on_avrcp_connect_t         bt_on_avrcp_connect_event              : avrcp connection status callback
 * @param[in]   rsi_bt_on_avrcp_disconnect_t      bt_on_avrcp_disconnect_event           : avrcp disconnection status callback
 * @param[in]   rsi_bt_on_avrcp_play_t            bt_on_avrcp_play_event                 : avrcp play status callback
 * @param[in]   rsi_bt_on_avrcp_pause_t           bt_on_avrcp_pause_event                : avrcp pause status callback
 * @param[in]   rsi_bt_on_avrcp_stop_t            bt_on_avrcp_stop_event                 : avrcp stop status callback
 * @param[in]   rsi_bt_on_avrcp_next_t            bt_on_avrcp_next_event                 : avrcp next status callback
 * @param[in]   rsi_bt_on_avrcp_previous_t        bt_on_avrcp_previous_event             : avrcp previous status callback
 * @param[in]   rsi_bt_on_avrcp_vol_up_t          bt_on_avrcp_vol_up_event               : avrcp vol up status callback
 * @param[in]   rsi_bt_on_avrcp_vol_down_t        bt_on_avrcp_vol_down_event             : avrcp vol down status callback
 * @param[in]   rsi_bt_on_avrcp_get_elem_attr_resp_t bt_on_avrcp_get_elem_attr_resp_event : avrcp elem attr resp status callback
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the spp callbacks
 *
 */
void rsi_bt_avrcp_register_callbacks (
    rsi_bt_on_avrcp_connect_t         bt_on_avrcp_connect_event,
    rsi_bt_on_avrcp_disconnect_t      bt_on_avrcp_disconnect_event,
    rsi_bt_on_avrcp_play_t            bt_on_avrcp_play_event,
    rsi_bt_on_avrcp_pause_t           bt_on_avrcp_pause_event,
    rsi_bt_on_avrcp_stop_t            bt_on_avrcp_stop_event,
    rsi_bt_on_avrcp_next_t            bt_on_avrcp_next_event,
    rsi_bt_on_avrcp_previous_t        bt_on_avrcp_previous_event,
    rsi_bt_on_avrcp_vol_up_t          bt_on_avrcp_vol_up_event,
    rsi_bt_on_avrcp_vol_down_t        bt_on_avrcp_vol_down_event,
    rsi_bt_on_avrcp_get_elem_attr_resp_t bt_on_avrcp_get_elem_attr_resp_event,
    rsi_bt_on_avrcp_notify_event_t    bt_on_avrcp_notify_event)
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;


  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_on_avrcp_connect_event     =  bt_on_avrcp_connect_event;
  bt_specific_cb->bt_on_avrcp_disconnect_event  =  bt_on_avrcp_disconnect_event;
  bt_specific_cb->bt_on_avrcp_play_event        =  bt_on_avrcp_play_event;
  bt_specific_cb->bt_on_avrcp_pause_event       =  bt_on_avrcp_pause_event;
  bt_specific_cb->bt_on_avrcp_stop_event        =  bt_on_avrcp_stop_event;
  bt_specific_cb->bt_on_avrcp_next_event        =  bt_on_avrcp_next_event;
  bt_specific_cb->bt_on_avrcp_previous_event    =  bt_on_avrcp_previous_event;
  bt_specific_cb->bt_on_avrcp_vol_up_event      =  bt_on_avrcp_vol_up_event;
  bt_specific_cb->bt_on_avrcp_vol_down_event    =  bt_on_avrcp_vol_down_event;
  bt_specific_cb->bt_on_avrcp_get_elem_attr_resp_event = bt_on_avrcp_get_elem_attr_resp_event;
  bt_specific_cb->bt_on_avrcp_notify_event      =  bt_on_avrcp_notify_event;
}

/**
 * @fn          rsi_bt_avrcp_target_register_callbacks 
 * @brief       This function registers the avrcp target callbacks
 * @param[in]   rsi_bt_on_avrcp_get_cap_event_t                  bt_on_avrcp_gat_cap,
 * @param[in]   rsi_bt_on_avrcp_get_app_supp_atts_event_t        bt_on_avrcp_get_app_att_list,
 * @param[in]   rsi_bt_on_avrcp_get_app_suup_att_vals_event_t    bt_on_avrcp_get_app_att_vals_list,
 * @param[in]   rsi_bt_on_avrcp_get_app_cur_att_val_event_t      bt_on_avrcp_get_app_cur_att_val,
 * @param[in]   rsi_bt_on_avrcp_set_app_cur_att_val_event_t      bt_on_avrcp_set_app_cur_att_val,
 * @param[in]   rsi_bt_on_avrcp_get_app_att_text_event_t         bt_on_avrcp_get_app_att_text,
 * @param[in]   rsi_bt_on_avrcp_get_app_att_vals_text_event_t    bt_on_avrcp_get_app_att_vals_text,
 * @param[in]   rsi_bt_on_avrcp_supp_char_sets_event_t           bt_on_avrcp_supp_char_sets,
 * @param[in]   rsi_bt_on_avrcp_batt_status_event_t              bt_on_avrcp_batt_status,
 * @param[in]   rsi_bt_on_avrcp_get_song_atts_event_t            bt_on_avrcp_get_song_atts,
 * @param[in]   rsi_bt_on_avrcp_get_play_status_event_t          bt_on_avrcp_get_player_status,
 * @param[in]   rsi_bt_on_avrcp_reg_notify_event_t               bt_on_avrcp_reg_notifivation);
 * @param[in]   rsi_bt_on_avrcp_set_abs_vol_event_t              bt_on_avrcp_set_abs_vol);
 *
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the spp callbacks
 *
 */
void rsi_bt_avrcp_target_register_callbacks (
    rsi_bt_on_avrcp_get_cap_event_t                  bt_on_avrcp_gat_cap,
    rsi_bt_on_avrcp_get_app_supp_atts_event_t        bt_on_avrcp_get_app_att_list,
    rsi_bt_on_avrcp_get_app_suup_att_vals_event_t    bt_on_avrcp_get_app_att_vals_list,
    rsi_bt_on_avrcp_get_app_cur_att_val_event_t      bt_on_avrcp_get_app_cur_att_val,
    rsi_bt_on_avrcp_set_app_cur_att_val_event_t      bt_on_avrcp_set_app_cur_att_val,
    rsi_bt_on_avrcp_get_app_att_text_event_t         bt_on_avrcp_get_app_att_text,
    rsi_bt_on_avrcp_get_app_att_vals_text_event_t    bt_on_avrcp_get_app_att_vals_text,
    rsi_bt_on_avrcp_supp_char_sets_event_t           bt_on_avrcp_supp_char_sets,
    rsi_bt_on_avrcp_batt_status_event_t              bt_on_avrcp_batt_status,
    rsi_bt_on_avrcp_get_song_atts_event_t            bt_on_avrcp_get_song_atts,
    rsi_bt_on_avrcp_get_play_status_event_t          bt_on_avrcp_get_player_status,
    rsi_bt_on_avrcp_reg_notify_event_t               bt_on_avrcp_reg_notifivation,
    rsi_bt_on_avrcp_set_abs_vol_event_t              bt_on_avrcp_set_abs_vol)
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;


  //! Assign the call backs to the respective call back
 bt_specific_cb->bt_on_avrcp_gat_cap               = bt_on_avrcp_gat_cap;
 bt_specific_cb->bt_on_avrcp_get_app_att_list      = bt_on_avrcp_get_app_att_list;
 bt_specific_cb->bt_on_avrcp_get_app_att_vals_list = bt_on_avrcp_get_app_att_vals_list;
 bt_specific_cb->bt_on_avrcp_get_app_cur_att_val   = bt_on_avrcp_get_app_cur_att_val;
 bt_specific_cb->bt_on_avrcp_set_app_cur_att_val   = bt_on_avrcp_set_app_cur_att_val;
 bt_specific_cb->bt_on_avrcp_get_app_att_text      = bt_on_avrcp_get_app_att_text;
 bt_specific_cb->bt_on_avrcp_get_app_att_vals_text = bt_on_avrcp_get_app_att_vals_text;
 bt_specific_cb->bt_on_avrcp_supp_char_sets        = bt_on_avrcp_supp_char_sets;
 bt_specific_cb->bt_on_avrcp_batt_status           = bt_on_avrcp_batt_status;
 bt_specific_cb->bt_on_avrcp_get_song_atts         = bt_on_avrcp_get_song_atts;
 bt_specific_cb->bt_on_avrcp_get_player_status     = bt_on_avrcp_get_player_status;
 bt_specific_cb->bt_on_avrcp_reg_notifivation      = bt_on_avrcp_reg_notifivation;
 bt_specific_cb->bt_on_avrcp_set_abs_vol           = bt_on_avrcp_set_abs_vol;

  return;
}

void rsi_bt_hfp_register_callbacks (
     rsi_bt_on_hfp_connect_t 			bt_on_hfp_connect_event,
     rsi_bt_on_hfp_disconnect_t         	bt_on_hfp_disconnect_event,
     rsi_bt_on_hfp_ring_t               	bt_on_hfp_ring_event,
     rsi_bt_on_hfp_callcallerid_t      		bt_on_hfp_callcallerid_event,
     rsi_bt_on_hfp_audioconnected_t    		bt_on_hfp_audioconnected_event,
     rsi_bt_on_hfp_audiodisconnected_t 		bt_on_hfp_audiodisconnected_event,
     rsi_bt_on_hfp_dialcomplete_t          	bt_on_hfp_dialcomplete_event,
     rsi_bt_on_hfp_answercomplete_t        	bt_on_hfp_answercomplete_event,
     rsi_bt_on_hfp_hangupcomplete_t        	bt_on_hfp_hangupcomplete_event,
     rsi_bt_on_hfp_senddtmfcomplete_t      	bt_on_hfp_senddtmfcomplete_event,
     rsi_bt_on_hfp_callwait_t              	bt_on_hfp_callwait_event,                     
     rsi_bt_on_hfp_callvoicerecogdeactivated_t  bt_on_hfp_callvoicerecogdeactivated_event,                     
     rsi_bt_on_hfp_callvoicerecogactivated_t    bt_on_hfp_callvoicerecogactivated_event,                     
     rsi_bt_on_hfp_servicenotfound_t            bt_on_hfp_servicenotfound_event,
     rsi_bt_app_on_hfp_callstatus_t             bt_on_hfp_callstatus_event,
     rsi_bt_app_on_hfp_signalstrength_t         bt_on_hfp_signalstrength_event,
     rsi_bt_app_on_hfp_batterylevel_t           bt_on_hfp_batterylevel_event,
     rsi_bt_app_on_hfp_phoneservice_t           bt_on_hfp_phoneservice_event,
     rsi_bt_app_on_hfp_roamingstatus_t          bt_on_hfp_roamingstatus_event,
     rsi_bt_app_on_hfp_callsetup_t              bt_on_hfp_callsetup_event,
     rsi_bt_app_on_hfp_callheld_t               bt_on_hfp_callheld_event)
  
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;


  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_on_hfp_connect_event     =  bt_on_hfp_connect_event;
  bt_specific_cb->bt_on_hfp_disconnect_event  =  bt_on_hfp_disconnect_event;
  bt_specific_cb->bt_on_hfp_ring_event  =  bt_on_hfp_ring_event;
  bt_specific_cb->bt_on_hfp_callcallerid_event  =  bt_on_hfp_callcallerid_event;
  bt_specific_cb->bt_on_hfp_audioconnected_event  =  bt_on_hfp_audioconnected_event;
  bt_specific_cb->bt_on_hfp_audiodisconnected_event  =  bt_on_hfp_audiodisconnected_event;
  bt_specific_cb->bt_on_hfp_dialcomplete_event  =  bt_on_hfp_dialcomplete_event;
  bt_specific_cb->bt_on_hfp_answercomplete_event  =  bt_on_hfp_answercomplete_event;
  bt_specific_cb->bt_on_hfp_hangupcomplete_event  =  bt_on_hfp_hangupcomplete_event;
  bt_specific_cb->bt_on_hfp_senddtmfcomplete_event  =  bt_on_hfp_senddtmfcomplete_event;
  bt_specific_cb->bt_on_hfp_callwait_event  =  bt_on_hfp_callwait_event;
  bt_specific_cb->bt_on_hfp_callvoicerecogdeactivated_event  =  bt_on_hfp_callvoicerecogdeactivated_event;
  bt_specific_cb->bt_on_hfp_callvoicerecogactivated_event  =  bt_on_hfp_callvoicerecogactivated_event;
  bt_specific_cb->bt_on_hfp_servicenotfound_event  =  bt_on_hfp_servicenotfound_event;
  bt_specific_cb->bt_on_hfp_callstatus_event  =  bt_on_hfp_callstatus_event;
  bt_specific_cb->bt_on_hfp_signalstrength_event  =  bt_on_hfp_signalstrength_event;
  bt_specific_cb->bt_on_hfp_batterylevel_event  =  bt_on_hfp_batterylevel_event;
  bt_specific_cb->bt_on_hfp_phoneservice_event  =  bt_on_hfp_phoneservice_event;
  bt_specific_cb->bt_on_hfp_roamingstatus_event  =  bt_on_hfp_roamingstatus_event;
  bt_specific_cb->bt_on_hfp_callsetup_event  =  bt_on_hfp_callsetup_event;
  bt_specific_cb->bt_on_hfp_callheld_event  =  bt_on_hfp_callheld_event;

}
/**
 * @fn          rsi_bt_pbap_register_callbacks 
 * @brief       This function registers the PBAP callbacks
 * @param[in]   rsi_bt_on_pbap_connect_t         bt_on_pbap_connect_event              : pbap connection status callback
 * @param[in]   rsi_bt_on_pbap_disconnect_t      bt_on_pbap_disconnect_event           : pbap disconnection status callback
 * @param[in]   rsi_bt_on_pbap_data_t            bt_on_pbap_data_event                 : pbap data status callback
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the pbap callbacks
 *
 */
void rsi_bt_pbap_register_callbacks (
    rsi_bt_on_pbap_connect_t         bt_on_pbap_connect_event,
    rsi_bt_on_pbap_disconnect_t      bt_on_pbap_disconnect_event,
    rsi_bt_on_pbap_data_t            bt_on_pbap_data_event)
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;

  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_on_pbap_connect_event     =  bt_on_pbap_connect_event;
  bt_specific_cb->bt_on_pbap_disconnect_event  =  bt_on_pbap_disconnect_event;
  bt_specific_cb->bt_on_pbap_data_event        =  bt_on_pbap_data_event;
}
void rsi_bt_iap_register_callbacks (
    rsi_bt_app_iap_conn_t                           bt_app_iap_conn,
    rsi_bt_app_iap_disconn_t                        bt_app_iap_disconn,
    rsi_bt_app_iap_acc_auth_strt_t                  bt_app_iap_acc_auth_strt,
    rsi_bt_app_iap_acc_auth_fail_t                  bt_app_iap_acc_auth_fail,
    rsi_bt_app_iap_acc_auth_complete_t              bt_app_iap_acc_auth_complete,
    rsi_bt_app_iap2_auth_start_t                    bt_app_iap2_auth_start,
    rsi_bt_app_iap2_auth_complete_t                 bt_app_iap2_auth_complete,
    rsi_bt_app_iap2_auth_fail_t                     bt_app_iap2_auth_fail,
    rsi_bt_app_iap2_identification_start_t          bt_app_iap2_identification_start,
    rsi_bt_app_iap2_identification_complete_t       bt_app_iap2_identification_complete,
    rsi_bt_app_iap2_identification_reject_t         bt_app_iap2_identification_reject,
    rsi_bt_app_on_iap2_data_rx_t                    bt_app_on_iap2_data_rx,
    rsi_bt_app_iap2_File_Tx_state_t                 bt_app_iap2_File_Transfer_state,
    rsi_bt_app_iap2_File_Transfer_rx_data_t         bt_app_iap2_File_Transfer_data)
{
  //! Get bt cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = rsi_driver_cb->bt_classic_cb->bt_global_cb->bt_specific_cb;

  //! Assign the call backs to the respective call back
  bt_specific_cb->bt_app_iap_conn                  =               bt_app_iap_conn;
  bt_specific_cb->bt_app_iap_disconn               =  bt_app_iap_disconn;
  bt_specific_cb->bt_app_iap_acc_auth_strt         =  bt_app_iap_acc_auth_strt;
  bt_specific_cb->bt_app_iap_acc_auth_fail         =  bt_app_iap_acc_auth_fail;
  bt_specific_cb->bt_app_iap_acc_auth_complete     =  bt_app_iap_acc_auth_complete;
  bt_specific_cb->bt_app_iap2_auth_start           =  bt_app_iap2_auth_start;
  bt_specific_cb->bt_app_iap2_auth_complete        =  bt_app_iap2_auth_complete;
  bt_specific_cb->bt_app_iap2_auth_fail            =  bt_app_iap2_auth_fail;
  bt_specific_cb->bt_app_iap2_identification_start      =  bt_app_iap2_identification_start;
  bt_specific_cb->bt_app_iap2_identification_complete   =  bt_app_iap2_identification_complete;
  bt_specific_cb->bt_app_iap2_identification_reject     =  bt_app_iap2_identification_reject;
  bt_specific_cb->bt_app_on_iap2_data_rx                =  bt_app_on_iap2_data_rx;
  bt_specific_cb->bt_app_iap2_File_Tx_state             =  bt_app_iap2_File_Transfer_state;
  bt_specific_cb->bt_app_iap2_File_Transfer_rx_data   =  bt_app_iap2_File_Transfer_data;
}

#endif

void rsi_bt_callbacks_handler(rsi_bt_cb_t *bt_classic_cb, uint16_t rsp_type, uint8_t *payload, uint16_t payload_length)
{
  //! Get ble cb struct pointer
  rsi_bt_classic_cb_t *bt_specific_cb = bt_classic_cb->bt_global_cb->bt_specific_cb;

  //! Get common cb pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

  uint16_t status = 0;

  //! updating the response status;
  status  = rsi_bt_get_status (bt_classic_cb);

  //! Check each cmd_type like decode_resp_handler and call the respective callback
  switch (rsp_type)
  {
    case RSI_BT_EVT_ROLE_CHANGE_STATUS:
      {
        if(bt_specific_cb->bt_on_role_change_status_event != NULL)
        {
          bt_specific_cb->bt_on_role_change_status_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_UNBOND_STATUS:
      {
        if(bt_specific_cb->bt_on_unbond_status != NULL)
        {
          bt_specific_cb->bt_on_unbond_status (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_BOND_RESPONSE:
      {
        if(bt_specific_cb->bt_on_conn_status_event != NULL)
        {
          bt_specific_cb->bt_on_conn_status_event (status, (void *)payload );
        }
      }
      break;
	  case RSI_BT_EVT_INQUIRY_RESPONSE:
      {
        if(bt_specific_cb->bt_on_scan_resp_event != NULL)
        {
          bt_specific_cb->bt_on_scan_resp_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_REMOTE_DEVICE_NAME:
      {
        if(bt_specific_cb->bt_on_remote_name_resp_event != NULL)
        {
          bt_specific_cb->bt_on_remote_name_resp_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_REMOTE_NAME_REQUEST_CANCEL:
      {
        if(bt_specific_cb->bt_on_remote_name_request_cancel_event != NULL)
        {
          bt_specific_cb->bt_on_remote_name_request_cancel_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_DISCONNECTED: //! TODO: how is it diff from unbond?
      {
        if(bt_specific_cb->bt_on_disconnect_event != NULL)
        {
          //!Reset powe save mode as Disconnect received
          common_cb->power_save.current_ps_mode = 0;

          //! Reset module state as Disconnect received
          common_cb->power_save.module_state = 0;

          bt_specific_cb->bt_on_disconnect_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_USER_CONFIRMATION_REQUEST:
      {
        if(bt_specific_cb->bt_on_confirm_request_event != NULL)
        {
          bt_specific_cb->bt_on_confirm_request_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_USER_PASSKEY_NOTIFY:
    case RSI_BT_EVT_USER_PASKEY_DISPLAY:
      {
        if(bt_specific_cb->bt_on_passkey_display_event != NULL)
        {
          bt_specific_cb->bt_on_passkey_display_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_AVDTP_STATS:
      {
        if(bt_specific_cb->bt_on_avdtp_stats_event != NULL)
        {
          bt_specific_cb->bt_on_avdtp_stats_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_USER_PINCODE_REQUEST:
      {
        if(bt_specific_cb->bt_on_pincode_request_event != NULL)
        {
          bt_specific_cb->bt_on_pincode_request_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_USER_PASSKEY_REQUEST:
      {
        if(bt_specific_cb->bt_on_passkey_request_event != NULL)
        {
          bt_specific_cb->bt_on_passkey_request_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_INQ_COMPLETE:
     {
        if(bt_specific_cb->bt_on_inquiry_complete_event != NULL)
        {
          bt_specific_cb->bt_on_inquiry_complete_event (status);
        }
      }
      break;
    case RSI_BT_EVT_AUTH_COMPLETE:
      {
        if(bt_specific_cb->bt_on_auth_complete_event != NULL)
        {
          bt_specific_cb->bt_on_auth_complete_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_USER_LINKKEY_REQUEST:
      {
        if(bt_specific_cb->bt_on_linkkey_request_event != NULL)
        {
          bt_specific_cb->bt_on_linkkey_request_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_USER_LINKKEY_SAVE:
      {
        if(bt_specific_cb->bt_on_linkkey_save_event != NULL)
        {
          bt_specific_cb->bt_on_linkkey_save_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_SPP_RECEIVE:  
      {
        if(bt_specific_cb->bt_on_spp_rx_data_event != NULL)
        {
          bt_specific_cb->bt_on_spp_rx_data_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_SPP_CONNECTED:
      {
        if(bt_specific_cb->bt_on_spp_connect_event != NULL)
        {
          bt_specific_cb->bt_on_spp_connect_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_SPP_DISCONNECTED:
      {
        if(bt_specific_cb->bt_on_spp_disconnect_event != NULL)
        {
          bt_specific_cb->bt_on_spp_disconnect_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_RSP_QUERY_SERVICES:
      {
        if(bt_specific_cb->bt_on_get_services_event != NULL)
        {
          bt_specific_cb->bt_on_get_services_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_RSP_SEARCH_SERVICE:
      {
        //! TODO:
        /*if(bt_specific_cb->bt_on_search_service_event != NULL)
          {
          bt_specific_cb->bt_on_search_service_event (status, (void *)payload );
          }*/
      }
      break;

    case RSI_BT_EVT_MODE_CHANGED:
      {
        if(bt_specific_cb->bt_on_mode_change_event != NULL)
        {
          bt_specific_cb->bt_on_mode_change_event  (status, (void *)payload );
        }
      }
      break;	

    case RSI_BT_EVT_SNIFF_SUBRATING_CHANGED:
      {
        if(bt_specific_cb->bt_on_sniff_subrating_event != NULL)
        {
          bt_specific_cb->bt_on_sniff_subrating_event  (status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_GATT_CONNECTED:
      {
        if(bt_specific_cb->bt_on_gatt_connection_event != NULL)
        {
          bt_specific_cb->bt_on_gatt_connection_event  (status, (void *)payload );
        }
      }
      break;
	  
    case RSI_BT_EVT_GATT_DISCONNECTED:
      {
        if(bt_specific_cb->bt_on_gatt_disconnection_event != NULL)
        {
          bt_specific_cb->bt_on_gatt_disconnection_event  (status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_SSP_COMPLETE:
      {
        if(bt_specific_cb->bt_on_ssp_complete_event != NULL)
        {
          bt_specific_cb->bt_on_ssp_complete_event  (status, (void *)payload );
        }
      }
      break;	
    case RSI_BT_EVT_A2DP_CONNECTED:
      {
        if(bt_specific_cb->bt_on_a2dp_connect_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_connect_event (status, (rsi_bt_event_a2dp_connect_t *)payload );
        }
      }
      break;
    case RSI_BT_EVT_A2DP_DISCONNECTED:
      {
        if(bt_specific_cb->bt_on_a2dp_disconnect_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_disconnect_event (status, (rsi_bt_event_a2dp_disconnect_t *)payload );
        }
      }
      break;
			
    case RSI_BT_REQ_A2DP_PCM_MP3_DATA:
    {
        if(bt_specific_cb->bt_on_a2dp_pcm_data_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_pcm_data_event (status, (rsi_bt_event_a2dp_pcm_data_t *)payload );
        }
      }
	  break;

    case RSI_BT_EVT_A2DP_CONFIGURED:
      {
        if(bt_specific_cb->bt_on_a2dp_configure_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_configure_event (status, (rsi_bt_event_a2dp_configure_t *)payload );
        }
      }
      break;
    case RSI_BT_EVT_A2DP_OPEN:
      {
        if(bt_specific_cb->bt_on_a2dp_open_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_open_event (status, (rsi_bt_event_a2dp_open_t *)payload);
        }
      }
      break;
    case RSI_BT_EVT_A2DP_START:
      {
        if(bt_specific_cb->bt_on_a2dp_start_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_start_event (status, (rsi_bt_event_a2dp_start_t *)payload);
#if (BT_HOST_SBC == 1)
	  bt_evt_a2dp_start (((rsi_bt_event_a2dp_start_t *)payload)->rem_mtu_size);
#endif
        }
      }
      break;
    case RSI_BT_EVT_A2DP_SUSPEND:
      {
        if(bt_specific_cb->bt_on_a2dp_suspend_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_suspend_event (status, (rsi_bt_event_a2dp_suspend_t *)payload);
        }
      }
      break;
    case RSI_BT_EVT_A2DP_ABORT:
      {
        if(bt_specific_cb->bt_on_a2dp_abort_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_abort_event (status, (rsi_bt_event_a2dp_abort_t *)payload);
        }
      }
      break;
    case RSI_BT_EVT_A2DP_CLOSE:
      {
        if(bt_specific_cb->bt_on_a2dp_close_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_close_event (status, (rsi_bt_event_a2dp_close_t *)payload);
        }
      }
      break;
    case RSI_BT_EVT_A2DP_ENCODE_DATA:
      {
        if(bt_specific_cb->bt_on_a2dp_encode_data_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_encode_data_event (status, (rsi_bt_event_a2dp_encode_data_t *)payload);
        }
      }
      break;
    case RSI_BT_EVT_A2DP_MORE_DATA_REQ:
      {
        if(bt_specific_cb->bt_on_a2dp_data_req_event != NULL)
        {
          bt_specific_cb->bt_on_a2dp_data_req_event (status, (rsi_bt_event_a2dp_more_data_req_t *)payload );
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_CONNECTED:
      {
        if(bt_specific_cb->bt_on_avrcp_connect_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_connect_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_AVRCP_DISCONNECTED:
      {
        if(bt_specific_cb->bt_on_avrcp_disconnect_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_disconnect_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_AVRCP_PLAY:
      {
        if(bt_specific_cb->bt_on_avrcp_play_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_play_event (status,(void *)payload);
        }
      }
      break;
    case RSI_BT_EVT_AVRCP_PAUSE:
      {
        if(bt_specific_cb->bt_on_avrcp_pause_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_pause_event (status,(void *)payload);
        }
      }
      break;
    case RSI_BT_EVT_AVRCP_STOP:
      {
        if(bt_specific_cb->bt_on_avrcp_stop_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_stop_event (status,(void *)payload);
        }
      }
      break;
    case RSI_BT_EVT_AVRCP_NEXT:
      {
        if(bt_specific_cb->bt_on_avrcp_next_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_next_event (status ,(void *)payload);
        }
      }
      break;
    case RSI_BT_EVT_AVRCP_PREVIOUS:
      {
        if(bt_specific_cb->bt_on_avrcp_previous_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_previous_event (status,(void *)payload);
        }
      }
      break;
    case RSI_BT_EVT_AVRCP_VOL_UP:
      {
        if(bt_specific_cb->bt_on_avrcp_vol_up_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_vol_up_event (status ,(void *)payload);
        }
      }
      break;
    case RSI_BT_EVT_AVRCP_VOL_DOWN:
      {
        if(bt_specific_cb->bt_on_avrcp_vol_down_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_vol_down_event (status ,(void *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GET_ELEM_RESP:
      {
        if(bt_specific_cb->bt_on_avrcp_get_elem_attr_resp_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_get_elem_attr_resp_event (status ,(void *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_NOTIFY:
      {
        if(bt_specific_cb->bt_on_avrcp_notify_event != NULL)
        {
          bt_specific_cb->bt_on_avrcp_notify_event (status,(void *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GAT_CAPS:
      {
        if(bt_specific_cb->bt_on_avrcp_gat_cap != NULL)
        {
          bt_specific_cb->bt_on_avrcp_gat_cap (payload, payload[6]);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GET_APP_SUPP_ATTS:
      {
        if(bt_specific_cb->bt_on_avrcp_get_app_att_list != NULL)
        {
          bt_specific_cb->bt_on_avrcp_get_app_att_list (payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GET_APP_SUUP_ATT_VALS:
      {
        if(bt_specific_cb->bt_on_avrcp_get_app_att_vals_list != NULL)
        {
          bt_specific_cb->bt_on_avrcp_get_app_att_vals_list (payload, payload[6]);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GET_APP_CUR_ATT_VAL:
      {
        if(bt_specific_cb->bt_on_avrcp_get_app_cur_att_val != NULL)
        {
          bt_specific_cb->bt_on_avrcp_get_app_cur_att_val ((rsi_bt_event_avrcp_get_cur_att_val_t *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_SET_APP_CUR_ATT_VAL:
      {
        if(bt_specific_cb->bt_on_avrcp_set_app_cur_att_val != NULL)
        {
          bt_specific_cb->bt_on_avrcp_set_app_cur_att_val ((rsi_bt_event_avrcp_set_att_val_t *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GET_APP_ATT_TEXT:
      {
        if(bt_specific_cb->bt_on_avrcp_get_app_att_text != NULL)
        {
          bt_specific_cb->bt_on_avrcp_get_app_att_text ((rsi_bt_event_avrcp_get_cur_att_val_t *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GET_APP_ATT_VAL_TEXT:
      {
        if(bt_specific_cb->bt_on_avrcp_get_app_att_vals_text != NULL)
        {
          bt_specific_cb->bt_on_avrcp_get_app_att_vals_text ((rsi_bt_event_avrcp_vals_text_req_t *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_INFORM_CHAR_SETS:
      {
        if(bt_specific_cb->bt_on_avrcp_supp_char_sets != NULL)
        {
          bt_specific_cb->bt_on_avrcp_supp_char_sets ((rsi_bt_event_avrcp_char_sets_t *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_INFORM_BATT_STATUS:
      {
        if(bt_specific_cb->bt_on_avrcp_batt_status != NULL)
        {
          bt_specific_cb->bt_on_avrcp_batt_status (payload, payload[6]);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GET_SONG_ATTS:
      {
        if(bt_specific_cb->bt_on_avrcp_get_song_atts != NULL)
        {
          bt_specific_cb->bt_on_avrcp_get_song_atts ((rsi_bt_event_avrcp_ele_att_t *)payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_GET_PLAY_STATUS:
      {
        if(bt_specific_cb->bt_on_avrcp_get_player_status != NULL)
        {
          bt_specific_cb->bt_on_avrcp_get_player_status (payload);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_REG_NOTIFICATION:
      {
        if(bt_specific_cb->bt_on_avrcp_reg_notifivation != NULL)
        {
          bt_specific_cb->bt_on_avrcp_reg_notifivation (payload, payload[6]);
        }
      }
      break;

    case RSI_BT_EVT_AVRCP_SET_ABS_VOL:
      {
        if(bt_specific_cb->bt_on_avrcp_set_abs_vol != NULL)
        {
          bt_specific_cb->bt_on_avrcp_set_abs_vol ((rsi_bt_event_avrcp_set_abs_vol_t *)payload);
        }
      }
      break;

    case RSI_BT_EVT_HFP_CONN:
      {
        if(bt_specific_cb->bt_on_hfp_connect_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_connect_event (status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_DISCONN:
      {
        if(bt_specific_cb->bt_on_hfp_disconnect_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_disconnect_event (status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_HFP_RING:
      {
        if(bt_specific_cb->bt_on_hfp_ring_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_ring_event(status,(void *)payload); //, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_HFP_CALLCALLERID:
      {
        if(bt_specific_cb->bt_on_hfp_callcallerid_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_callcallerid_event (status,(void *)payload ); //(status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_HFP_AUDIOCONNECTED:
      {
        if(bt_specific_cb->bt_on_hfp_audioconnected_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_audioconnected_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_HFP_AUDIODISCONNECTED:
      {
        if(bt_specific_cb->bt_on_hfp_audiodisconnected_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_audiodisconnected_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;


    case RSI_BT_EVT_HFP_DIALCOMPLETE:
      {
        if(bt_specific_cb->bt_on_hfp_dialcomplete_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_dialcomplete_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_HFP_ANSWERCOMPLETE:
      {
        if(bt_specific_cb->bt_on_hfp_answercomplete_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_answercomplete_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_HFP_HANGUPCOMPLETE:
      {
        if(bt_specific_cb->bt_on_hfp_hangupcomplete_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_hangupcomplete_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_HFP_SENDDTMFCOMPLETE:
      {
        if(bt_specific_cb->bt_on_hfp_senddtmfcomplete_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_senddtmfcomplete_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_CALLWAIT:
      {
        if(bt_specific_cb->bt_on_hfp_callwait_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_callwait_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_VOICERECOGDEACTIVATED:
      {
        if(bt_specific_cb->bt_on_hfp_callvoicerecogdeactivated_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_callvoicerecogdeactivated_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_VOICERECOGACTIVATED:
      {
        if(bt_specific_cb->bt_on_hfp_callvoicerecogactivated_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_callvoicerecogactivated_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_SERVICENOTFOUND:
      {
        if(bt_specific_cb->bt_on_hfp_servicenotfound_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_servicenotfound_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;	

    case RSI_BT_EVT_HFP_CALL_STATUS:
      {
        if(bt_specific_cb->bt_on_hfp_callstatus_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_callstatus_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_SIGNALSTRENGTH:
      {
        if(bt_specific_cb->bt_on_hfp_signalstrength_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_signalstrength_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;


    case RSI_BT_EVT_HFP_BATTERYLEVEL:
      {
        if(bt_specific_cb->bt_on_hfp_batterylevel_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_batterylevel_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;

    case RSI_BT_EVT_HFP_PHONESERVICE:
      {
        if(bt_specific_cb->bt_on_hfp_phoneservice_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_phoneservice_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_ROAMINGSTATUS:
      {
        if(bt_specific_cb->bt_on_hfp_roamingstatus_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_roamingstatus_event (status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_CALLSETUP:
      {
        if(bt_specific_cb->bt_on_hfp_callsetup_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_callsetup_event(status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
    case RSI_BT_EVT_HFP_CALLHELDSTATUS:
      {
        if(bt_specific_cb->bt_on_hfp_callheld_event != NULL)
        {
          bt_specific_cb->bt_on_hfp_callheld_event(status, (void *)payload); //(status, (void *)payload );
        }
      }
      break;
   case RSI_BT_EVENT_IAP_CONN:
      {
        if(bt_specific_cb->bt_app_iap_conn != NULL)
        {
          bt_specific_cb->bt_app_iap_conn (status, (rsi_bt_event_iap_t *)payload );
        }
      }
      break;
       case RSI_BT_EVENT_IAP_DISCONN:
      {
        if(bt_specific_cb->bt_app_iap_disconn != NULL)
        {
          bt_specific_cb->bt_app_iap_disconn (status, (rsi_bt_event_iap_t *)payload );
        }
      }
      break;
 case RSI_BT_EVENT_IAP_ACC_AUTH_STARTED:
      {
        if(bt_specific_cb->bt_app_iap_acc_auth_strt != NULL)
        {
          bt_specific_cb->bt_app_iap_acc_auth_strt (status, (rsi_bt_event_iap_t *)payload );
        }
      }
      break;
 case RSI_BT_EVENT_IAP_ACC_AUTH_FAILED:
      {
        if(bt_specific_cb->bt_app_iap_acc_auth_fail != NULL)
        {
          bt_specific_cb->bt_app_iap_acc_auth_fail (status, (rsi_bt_event_iap_t *)payload );
        }
      }
      break;
 case RSI_BT_EVENT_IAP2_AUTH_START:
      {
        if(bt_specific_cb->bt_app_iap2_auth_start != NULL)
        {
          bt_specific_cb->bt_app_iap2_auth_start (status,(rsi_bt_event_iap_t *)payload );
        }
      }
      break;
 case RSI_BT_EVENT_IAP2_AUTH_COMPLETE:
      {
        if(bt_specific_cb->bt_app_iap2_auth_complete != NULL)
        {
          bt_specific_cb->bt_app_iap2_auth_complete (status, (rsi_bt_event_iap_t *)payload );
        }
      }
      break;
 case RSI_BT_EVENT_IAP2_AUTH_FAILED:
      {
        if(bt_specific_cb->bt_app_iap2_auth_fail != NULL)
        {
          bt_specific_cb->bt_app_iap2_auth_fail (status, (rsi_bt_event_iap_t *)payload );
        }
      }
      break;
 case RSI_BT_EVENT_IAP2_IDENTIFICATION_START:
      {
        if(bt_specific_cb->bt_app_iap2_identification_start != NULL)
        {
          bt_specific_cb->bt_app_iap2_identification_start (status, (rsi_bt_event_iap_t *)payload );
        }
      }
      break;
 case RSI_BT_EVENT_IAP2_IDENTIFICATION_COMPLETE:
      {
        if(bt_specific_cb->bt_app_iap2_identification_complete != NULL)
        {
          bt_specific_cb->bt_app_iap2_identification_complete (status, (rsi_bt_event_iap2_receive_t *)payload );
        }
      }
      break;
 case RSI_BT_EVENT_IAP2_IDENTIFICATION_REJECT:
      {
        if(bt_specific_cb->bt_app_iap2_identification_reject != NULL)
        {
          bt_specific_cb->bt_app_iap2_identification_reject (status, (rsi_bt_event_iap2_receive_t *)payload );
        }
      }
      break;

 case RSI_BT_EVENT_IAP2_RX_DATA:
      {
        if(bt_specific_cb->bt_app_on_iap2_data_rx != NULL)
        {
          bt_specific_cb->bt_app_on_iap2_data_rx (status, (rsi_bt_event_iap2_receive_t *)payload );
        }
      }
      break;

 case RSI_BT_EVENT_IAP2_RX_FILE_TRANSFER_STATE:
      {
        if(bt_specific_cb->bt_app_iap2_File_Tx_state != NULL)
        {
          bt_specific_cb->bt_app_iap2_File_Tx_state (status, (rsi_bt_event_iap2_receive_t *)payload );
        }
       }
       break;
  
 case RSI_BT_EVENT_IAP2_RX_FILE_TRANSFER_DATA:
      {
        if(bt_specific_cb->bt_app_iap2_File_Transfer_rx_data != NULL)
        {

          bt_specific_cb->bt_app_iap2_File_Transfer_rx_data (status, (rsi_bt_event_iap2_receive_t *)payload );
        }
      }
      break;

 case RSI_BT_EVENT_IAP_ACC_AUTH_COMPLETE:
      {
        if(bt_specific_cb->bt_app_iap_acc_auth_complete != NULL)
        {
          bt_specific_cb->bt_app_iap_acc_auth_complete (status, (rsi_bt_event_iap_t *)payload );
        }
      }
      break;
 case RSI_BT_EVT_PBAP_CONN:
      {
        if(bt_specific_cb->bt_on_pbap_connect_event != NULL)
        {
          bt_specific_cb->bt_on_pbap_connect_event (status, (rsi_bt_event_pbap_connect_t *)payload );
        }
      }
      break;
    case RSI_BT_EVT_PBAP_DISCONN:
      {
        if(bt_specific_cb->bt_on_pbap_disconnect_event != NULL)
        {
          bt_specific_cb->bt_on_pbap_disconnect_event (status, (rsi_bt_event_pbap_disconnect_t *)payload );
        }
      }
      break;
   case RSI_BT_EVT_PBAP_DATA:
      {

        if(bt_specific_cb->bt_on_pbap_data_event != NULL)
        {
          bt_specific_cb->bt_on_pbap_data_event (status, (rsi_bt_event_pbap_data_t *)payload );
        }
      }
      break;
  case RSI_BT_EVENT_HID_CONN:
      {
        if(bt_specific_cb->bt_on_hid_connect_event != NULL)
        {
          bt_specific_cb->bt_on_hid_connect_event (status, (rsi_bt_event_hid_connect_t *)payload );
        }
      }
      break;
  case RSI_BT_EVENT_HID_RXDATA:
      {
rsi_bt_event_hid_receive_t *hid_rx_event = (rsi_bt_event_hid_receive_t *)payload;

#if 1
          switch((hid_rx_event->data[0]) >> 4)
          {
            case HID_MSG_HANDSHAKE:
              {
                if(bt_specific_cb->bt_on_hid_handshake_event != NULL)
                {
                  bt_specific_cb->bt_on_hid_handshake_event (status, (rsi_bt_event_hid_receive_t *)payload );
                }
              }
              break;
            case HID_MSG_CONTROL:
              {
                if(bt_specific_cb->bt_on_hid_control_event != NULL)
                {
                  bt_specific_cb->bt_on_hid_control_event (status, (rsi_bt_event_hid_receive_t *)payload );
                }
              }
              break;
            case HID_MSG_GET_REPORT:
              {
                if(bt_specific_cb->bt_on_hid_get_report != NULL)
                {
                  bt_specific_cb->bt_on_hid_get_report (status, (rsi_bt_event_hid_receive_t *)payload );
                }
              }
              break;
            case HID_MSG_SET_REPORT:
              {
                if(bt_specific_cb->bt_on_hid_set_report != NULL)
                {
                  bt_specific_cb->bt_on_hid_set_report (status, (rsi_bt_event_hid_receive_t *)payload );
                }
              }
              break;
            case HID_MSG_GET_PROTO:
              {
                if(bt_specific_cb->bt_on_hid_get_proto != NULL)
                {
                  bt_specific_cb->bt_on_hid_get_proto (status, (rsi_bt_event_hid_receive_t *)payload );
                }
              }
              break;
            case HID_MSG_SET_PROTO:
              {
                if(bt_specific_cb->bt_on_hid_set_proto != NULL)
                {
                  bt_specific_cb->bt_on_hid_set_proto (status, (rsi_bt_event_hid_receive_t *)payload );
                }
              }
            case HID_MSG_DATA:
              {
                if(bt_specific_cb->bt_on_hid_rx_data_event != NULL)
                {
                  bt_specific_cb->bt_on_hid_rx_data_event (status, (rsi_bt_event_hid_receive_t *)payload );
                }
              }
              break;
            default:

              break;
          }

#endif
      }
      break;



  }
}

/**
 * @fn          rsi_ble_register_gap_callbacks 
 * @brief       This function registers the GAP callbacks
 * @param[in]   rsi_ble_on_adv_report_event_t   ble_on_adv_report_event : Advertise report callback
 * @param[in]   rsi_ble_on_connect_t  ble_on_conn_status_event: Connection status callback
 * @param[in]	rsi_ble_on_disconnect_t   ble_on_disconnect_event : Disconnection status callback
 * @param[out]
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the GAP callbacks
 *
 */
#ifdef RSI_BLE_ENABLE
void rsi_ble_gap_register_callbacks (
    rsi_ble_on_adv_report_event_t   ble_on_adv_report_event,
    rsi_ble_on_connect_t            ble_on_conn_status_event,
    rsi_ble_on_disconnect_t         ble_on_disconnect_event,
    rsi_ble_on_le_ping_payload_timeout_t  ble_on_le_ping_time_expired_event,
    rsi_ble_on_phy_update_complete_t      ble_on_phy_update_complete_event,
    rsi_ble_on_data_length_update_t        rsi_ble_on_data_length_update_event,
    rsi_ble_on_enhance_connect_t            ble_on_enhance_conn_status_event,
    rsi_ble_on_directed_adv_report_event_t  ble_on_directed_adv_report_event,
    rsi_ble_on_conn_update_complete_t      ble_on_conn_update_complete_event)
{
  //! Get ble cb struct pointer
  rsi_ble_cb_t *ble_specific_cb = rsi_driver_cb->ble_cb->bt_global_cb->ble_specific_cb;

  //! Assign the call backs to the respective call back
  ble_specific_cb->ble_on_adv_report_event  =  ble_on_adv_report_event;
  ble_specific_cb->ble_on_conn_status_event =  ble_on_conn_status_event;
  ble_specific_cb->ble_on_disconnect_event  =  ble_on_disconnect_event;
  ble_specific_cb->ble_on_le_ping_time_expired_event = ble_on_le_ping_time_expired_event;
  ble_specific_cb->ble_on_phy_update_complete_event = ble_on_phy_update_complete_event;
  ble_specific_cb->rsi_ble_on_data_length_update_event = rsi_ble_on_data_length_update_event;
  ble_specific_cb->ble_on_enhance_conn_status_event =  ble_on_enhance_conn_status_event;
  ble_specific_cb->ble_on_directed_adv_report_event = ble_on_directed_adv_report_event;
  ble_specific_cb->ble_on_conn_update_complete_event = ble_on_conn_update_complete_event;
}

void rsi_ble_gap_extended_register_callbacks (
    rsi_ble_on_remote_features_t      ble_on_remote_features_event,
    rsi_ble_on_le_more_data_req_t     ble_on_le_more_data_req_event)
{
  //! Get ble cb struct pointer
  rsi_ble_cb_t *ble_specific_cb = rsi_driver_cb->ble_cb->bt_global_cb->ble_specific_cb;

  //! Assign the call backs to the respective call back
  ble_specific_cb->ble_on_remote_features_event = ble_on_remote_features_event;
  ble_specific_cb->ble_on_le_more_data_req_event = ble_on_le_more_data_req_event;
}


void rsi_ant_register_callbacks (
    rsi_ant_resp_handler_t   ant_async_resp_handler)
{
  //! Get ble cb struct pointer
  rsi_ble_cb_t *ble_specific_cb = rsi_driver_cb->ble_cb->bt_global_cb->ble_specific_cb;

  //! Assign the call backs to the respective call back
  ble_specific_cb->ant_async_resp_handler  =  ant_async_resp_handler;
}


/**
 * @fn          rsi_ble_smp_register_callbacks 
 * @brief       This function registers the SMP callbacks
 * @param[in]   rsi_ble_on_smp_request_t   ble_on_smp_request_event : smp request callback
 * @param[in]   rsi_ble_on_smp_response_t  ble_on_smp_response_event: smp response callback
 * @param[in]	  rsi_ble_on_smp_passkey_t   ble_on_smp_passkey_event : smp passkey callback
 * @param[in]	  rsi_ble_on_smp_failed_t  ble_on_smp_failed_event: smp failed callback
 * @param[in]	  rsi_ble_on_encrypt_started_t  ble_on_smp_encryptrd: encription enabled callback
 * @param[in]	  rsi_ble_on_smp_passkey_display_t  ble_on_smp_passkey_display_event: smp passkey display callback
 * @param[in]	  rsi_ble_on_sc_passkey_t  ble_sc_passkey_event: sc passkey display callback
 * @param[out]
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the GAP callbacks
 *
 */
void rsi_ble_smp_register_callbacks (
    rsi_ble_on_smp_request_t   ble_on_smp_request_event,
    rsi_ble_on_smp_response_t   ble_on_smp_response_event,
    rsi_ble_on_smp_passkey_t   ble_on_smp_passkey_event,
    rsi_ble_on_smp_failed_t   ble_on_smp_failed_event,
    rsi_ble_on_encrypt_started_t  ble_on_smp_encryptrd,
    rsi_ble_on_smp_passkey_display_t  ble_on_smp_passkey_display_event,
    rsi_ble_on_sc_passkey_t         ble_sc_passkey_event,
    rsi_ble_on_le_ltk_req_event_t  ble_on_le_ltk_req_event,
    rsi_ble_on_le_security_keys_t  ble_on_le_security_keys_event)
{
  //! Get ble cb struct pointer
  rsi_ble_cb_t *ble_specific_cb = rsi_driver_cb->ble_cb->bt_global_cb->ble_specific_cb;

  //! Assign the call backs to the respective call back
  ble_specific_cb->ble_on_smp_request_event       =  ble_on_smp_request_event;
  ble_specific_cb->ble_on_smp_response_event      =  ble_on_smp_response_event;
  ble_specific_cb->ble_on_smp_passkey_event       =  ble_on_smp_passkey_event;
  ble_specific_cb->ble_on_smp_fail_event          =  ble_on_smp_failed_event;
  ble_specific_cb->ble_on_smp_encrypt_started     =  ble_on_smp_encryptrd;
  ble_specific_cb->ble_on_smp_passkey_display     =  ble_on_smp_passkey_display_event;
  ble_specific_cb->ble_on_sc_passkey              =  ble_sc_passkey_event;
  ble_specific_cb->ble_on_le_ltk_req_event        = ble_on_le_ltk_req_event;
  ble_specific_cb->ble_on_le_security_keys_event  = ble_on_le_security_keys_event;

}

/**
 * @fn          rsi_ble_gatt_register_callbacks
 * @brief       This function registers the GATT callbacks
 * @param[in]   rsi_ble_on_profiles_list_resp_t  ble_on_profiles_list_resp: Callback for rsi_ble_get_profiles command
 * @param[in]   rsi_ble_on_profile_resp_t        ble_on_profile_resp: Callback for rsi_ble_get_profile command
 * @param[in]   rsi_ble_on_char_services_resp_t  ble_on_char_services_resp: Callback for rsi_ble_get_char_services command
 * @param[in]   rsi_ble_on_inc_services_resp_t   ble_on_inc_services_resp:  Callback for rsi_ble_get_inc_services command
 * @param[in]   rsi_ble_on_att_desc_resp_t       ble_on_att_desc_resp:  Callback for rsi_ble_get_att_descriptors command
 * @param[in]   rsi_ble_on_read_resp_t           ble_on_read_resp:  Callback for all read requests command
 * @param[in]   rsi_ble_on_write_resp_t          ble_on_write_resp:  Callback for all write commands
 * @param[in]   rsi_ble_on_gatt_write_event_t    blw_on_gatt_event: Callback for all GATT events
 * @param[in]   rsi_ble_on_gatt_error_resp_t     ble_on_gatt_error_resp_event: Callback for GATT error events
 * @param[in]   rsi_ble_on_gatt_desc_val_event_t ble_on_gatt_desc_val_resp_event: Callback for GATT descriptor value event
 * @param[in]  ble_on_profiles_list_event, callback function for profiles list event
 * @param[in]  ble_on_profile_by_uuid_event, callback function for profile event
 * @param[in]  rsi_ble_on_event_read_by_char_services_t, callback function for char services event
 * @param[in]  rsi_ble_on_event_read_by_inc_services_t, callback function for inc services event
 * @param[in]  rsi_ble_on_event_read_att_value_t, callback function for read att value event
 * @param[in]  rsi_ble_on_event_read_resp_t, callback function for read att event
 * @param[in]  rsi_ble_on_event_write_resp_t, callback function for write event
 * @param[in]  rsi_ble_on_event_indicate_confirmation_t, callback function for indicate confirmation event
 * @param[in]  rsi_ble_on_event_prepare_write_resp_t, callback function for prepare write event
 * @param[out]
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function registers the GAP callbacks
 *
 */
void rsi_ble_gatt_register_callbacks ( 
    rsi_ble_on_profiles_list_resp_t           ble_on_profiles_list_resp,
    rsi_ble_on_profile_resp_t                 ble_on_profile_resp,
    rsi_ble_on_char_services_resp_t           ble_on_char_services_resp,
    rsi_ble_on_inc_services_resp_t            ble_on_inc_services_resp,
    rsi_ble_on_att_desc_resp_t                ble_on_att_desc_resp,
    rsi_ble_on_read_resp_t                    ble_on_read_resp,
    rsi_ble_on_write_resp_t                   ble_on_write_resp,
    rsi_ble_on_gatt_write_event_t             ble_on_gatt_event,
    rsi_ble_on_gatt_prepare_write_event_t     ble_on_gatt_prepare_write_event,
    rsi_ble_on_execute_write_event_t          ble_on_execute_write_event,
    rsi_ble_on_read_req_event_t               ble_on_read_req_event,
    rsi_ble_on_mtu_event_t                    ble_on_mtu_event,
    rsi_ble_on_gatt_error_resp_t              ble_on_gatt_error_resp_event,
    rsi_ble_on_gatt_desc_val_event_t          ble_on_gatt_desc_val_resp_event,
    rsi_ble_on_event_profiles_list_t          ble_on_profiles_list_event,
    rsi_ble_on_event_profile_by_uuid_t        ble_on_profile_by_uuid_event,
    rsi_ble_on_event_read_by_char_services_t  ble_on_read_by_char_services_event,
    rsi_ble_on_event_read_by_inc_services_t   ble_on_read_by_inc_services_event,     
    rsi_ble_on_event_read_att_value_t         ble_on_read_att_value_event,
    rsi_ble_on_event_read_resp_t              ble_on_read_resp_event,
    rsi_ble_on_event_write_resp_t             ble_on_write_resp_event,
    rsi_ble_on_event_indicate_confirmation_t  ble_on_indicate_confirmation_event,
    rsi_ble_on_event_prepare_write_resp_t     ble_on_prepare_write_resp_event)
{
  //! Get ble specific cb struct pointer
  rsi_ble_cb_t *ble_specific_cb = rsi_driver_cb->ble_cb->bt_global_cb->ble_specific_cb;

  //! Assign the call backs to the respective call back
  ble_specific_cb->ble_on_profiles_list_resp          =  ble_on_profiles_list_resp;
  ble_specific_cb->ble_on_profile_resp                =  ble_on_profile_resp;     
  ble_specific_cb->ble_on_char_services_resp          =  ble_on_char_services_resp;  
  ble_specific_cb->ble_on_inc_services_resp           =  ble_on_inc_services_resp;  
  ble_specific_cb->ble_on_att_desc_resp               =  ble_on_att_desc_resp;  
  ble_specific_cb->ble_on_read_resp                   =  ble_on_read_resp;       
  ble_specific_cb->ble_on_write_resp                  =  ble_on_write_resp;
  ble_specific_cb->ble_on_gatt_events                 =  ble_on_gatt_event;
  ble_specific_cb->ble_on_prepare_write_event         =  ble_on_gatt_prepare_write_event;
  ble_specific_cb->ble_on_execute_write_event         =  ble_on_execute_write_event;
  ble_specific_cb->ble_on_read_req_event              =  ble_on_read_req_event;
  ble_specific_cb->ble_on_mtu_event                   =  ble_on_mtu_event;
  ble_specific_cb->ble_on_gatt_error_resp_event       =  ble_on_gatt_error_resp_event;
  ble_specific_cb->ble_on_profiles_list_event         =  ble_on_profiles_list_event;
  ble_specific_cb->ble_on_gatt_desc_val_resp_event    =  ble_on_gatt_desc_val_resp_event;
  ble_specific_cb->ble_on_profile_by_uuid_event       =  ble_on_profile_by_uuid_event; 
  ble_specific_cb->ble_on_read_by_char_services_event =  ble_on_read_by_char_services_event;
  ble_specific_cb->ble_on_read_by_inc_services_event  =  ble_on_read_by_inc_services_event; 
  ble_specific_cb->ble_on_read_att_value_event        =  ble_on_read_att_value_event;     
  ble_specific_cb->ble_on_read_resp_event             =  ble_on_read_resp_event;          
  ble_specific_cb->ble_on_write_resp_event            =  ble_on_write_resp_event;           
  ble_specific_cb->ble_on_indicate_confirmation_event =  ble_on_indicate_confirmation_event;           
  ble_specific_cb->ble_on_prepare_write_resp_event    =  ble_on_prepare_write_resp_event ;
  
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_l2cap_cbsc_register_callbacks
 * @brief      registers L2CAP CBSC callback functions
 * @param[in]  ble_on_cbsc_conn_req, callback function for CBFC connection request event
 * @param[in]  ble_on_cbsc_conn_complete, callback function for CBFC connection complete status event
 * @param[in]  ble_on_cbsc_rx_data, callback function for CBFC data receive event
 * @param[in]  ble_on_cbsc_disconn, callback function for CBFC disconnect event
 *  * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function registers the function pointers for GATT responses
 */
void rsi_ble_l2cap_cbsc_register_callbacks (
     rsi_ble_on_cbfc_conn_req_event_t   ble_on_cbsc_conn_req,
     rsi_ble_on_cbfc_conn_complete_event_t   ble_on_cbsc_conn_complete,
     rsi_ble_on_cbfc_rx_data_event_t    ble_on_cbsc_rx_data,
     rsi_ble_on_cbfc_disconn_event_t    ble_on_cbsc_disconn)
{
  //! Get ble specific cb struct pointer
  rsi_ble_cb_t *ble_specific_cb = rsi_driver_cb->ble_cb->bt_global_cb->ble_specific_cb;

  //! Assign the call backs to the respective call back
  ble_specific_cb->ble_on_cbfc_conn_req_event      = ble_on_cbsc_conn_req;
  ble_specific_cb->ble_on_cbfc_conn_complete_event = ble_on_cbsc_conn_complete;
  ble_specific_cb->ble_on_cbfc_rx_data_event       = ble_on_cbsc_rx_data;
  ble_specific_cb->ble_on_cbfc_disconn_event       = ble_on_cbsc_disconn;

  return;
}

void rsi_ble_callbacks_handler(rsi_bt_cb_t *ble_cb, uint16_t rsp_type, uint8_t *payload, uint16_t payload_length)
{
  //! Get ble cb struct pointer
  rsi_ble_cb_t *ble_specific_cb = ble_cb->bt_global_cb->ble_specific_cb;
  uint16_t status = 0;

  //! updating the response status;
  status  = rsi_bt_get_status (ble_cb);

  //! Check each cmd_type like decode_resp_handler and call the respective callback
  switch (rsp_type)
  {
    case RSI_BLE_EVENT_ADV_REPORT:
      {
        if (ble_specific_cb->ble_on_adv_report_event != NULL)
        {
          ble_specific_cb->ble_on_adv_report_event ((rsi_ble_event_adv_report_t*)payload );
        }
        break;
      }
    case RSI_BLE_EVENT_CONN_STATUS:
      {
        if (ble_specific_cb->ble_on_conn_status_event != NULL) {
          ble_specific_cb->ble_on_conn_status_event((rsi_ble_event_conn_status_t *)payload );
        }
        //rsi_bt_set_status(ble_cb, RSI_BLE_STATE_CONNECTION);
        break;
      }
	  
    case RSI_BLE_EVENT_ENHANCE_CONN_STATUS:
      {
        if (ble_specific_cb->ble_on_enhance_conn_status_event != NULL) {
          ble_specific_cb->ble_on_enhance_conn_status_event((rsi_ble_event_enhance_conn_status_t *)payload );
        }
        //rsi_bt_set_status(ble_cb, RSI_BLE_STATE_CONNECTION);
        break;
      }
 
    case RSI_BLE_EVENT_DISCONNECT:
      {
        if (ble_specific_cb->ble_on_disconnect_event != NULL) {
          ble_specific_cb->ble_on_disconnect_event((rsi_ble_event_disconnect_t *)payload, ble_cb->status);
        }
        //rsi_bt_set_status(ble_cb, RSI_BLE_STATE_DSICONNECT);
        break;
      }
    case RSI_BLE_EVENT_GATT_ERROR_RESPONSE:
      {
        if (ble_specific_cb->ble_on_gatt_error_resp_event != NULL)
        {
          ble_specific_cb->ble_on_gatt_error_resp_event (status, (rsi_ble_event_error_resp_t  *)payload);
        }
        break;
      }
    case RSI_BLE_EVENT_GATT_DESC_VAL_RESPONSE:
      {
        if (ble_specific_cb->ble_on_gatt_desc_val_resp_event != NULL)
        {
          ble_specific_cb->ble_on_gatt_desc_val_resp_event (status, (rsi_ble_event_gatt_desc_t  *)payload);
        }
        break;
      }

     case RSI_BLE_EVENT_GATT_PRIMARY_SERVICE_LIST:
      {
        if (ble_specific_cb->ble_on_profiles_list_event != NULL)
        {
          ble_specific_cb->ble_on_profiles_list_event (status, (rsi_ble_event_profiles_list_t  *)payload);
        }
        break;
      }
     case RSI_BLE_EVENT_GATT_PRIMARY_SERVICE_BY_UUID:
      {
        if (ble_specific_cb->ble_on_profile_by_uuid_event != NULL)
        {
          ble_specific_cb->ble_on_profile_by_uuid_event (status, (rsi_ble_event_profile_by_uuid_t *)payload);
        }
        break;
      }
    case RSI_BLE_EVENT_GATT_READ_CHAR_SERVS:
      {
        if (ble_specific_cb->ble_on_read_by_char_services_event != NULL)
        {
          ble_specific_cb->ble_on_read_by_char_services_event (status, (rsi_ble_event_read_by_type1_t  *)payload);
        }
        break;
      }
 case RSI_BLE_EVENT_GATT_READ_INC_SERVS:
      {
        if (ble_specific_cb->ble_on_read_by_inc_services_event != NULL)
        {
          ble_specific_cb->ble_on_read_by_inc_services_event (status, (rsi_ble_event_read_by_type2_t  *)payload);
        }
        break;
      }
case RSI_BLE_EVENT_GATT_READ_VAL_BY_UUID:
      {
        if (ble_specific_cb->ble_on_read_att_value_event != NULL)
        {
          ble_specific_cb->ble_on_read_att_value_event (status, (rsi_ble_event_read_by_type3_t  *)payload);
        }
        break;
      }
   case  RSI_BLE_EVENT_GATT_READ_RESP:
   case   RSI_BLE_EVENT_GATT_READ_BLOB_RESP:
   case    RSI_BLE_EVENT_GATT_READ_MULTIPLE_RESP:
      {
        if (ble_specific_cb->ble_on_read_resp_event != NULL)
        {
          ble_specific_cb->ble_on_read_resp_event (status, (rsi_ble_event_att_value_t  *)payload);
        }
        break;
      }
 case RSI_BLE_EVENT_GATT_WRITE_RESP:
 case RSI_BLE_EVENT_GATT_EXECUTE_WRITE_RESP:
      {
        if (ble_specific_cb->ble_on_write_resp_event != NULL)
        {
          ble_specific_cb->ble_on_write_resp_event (status, (rsi_ble_set_att_resp_t  *)payload);
        }
        break;
      }
 case RSI_BLE_EVENT_GATT_INDICATE_CONFIRMATION:
      {
        if (ble_specific_cb->ble_on_indicate_confirmation_event != NULL)
        {
          ble_specific_cb->ble_on_indicate_confirmation_event (status, (rsi_ble_set_att_resp_t  *)payload);
        }
      }
      break;
 case RSI_BLE_EVENT_GATT_PREPARE_WRITE_RESP:
      {
        if (ble_specific_cb->ble_on_prepare_write_resp_event != NULL)
        {
          ble_specific_cb->ble_on_prepare_write_resp_event (status, (rsi_ble_prepare_write_resp_t  *)payload);
        }
        break;
      }
   case RSI_ANT_CMD:
      {
        if (ble_specific_cb->ant_async_resp_handler != NULL)
        {
          ble_specific_cb->ant_async_resp_handler (payload);
        }
      }
      break;

    case RSI_BLE_EVENT_SMP_REQUEST:
      {
        if (ble_specific_cb->ble_on_smp_request_event != NULL) {
          ble_specific_cb->ble_on_smp_request_event((rsi_bt_event_smp_req_t *)payload);
        }
      }
      break;

    case RSI_BLE_EVENT_SMP_RESPONSE:
      {
        if (ble_specific_cb->ble_on_smp_response_event != NULL) {
          ble_specific_cb->ble_on_smp_response_event((rsi_bt_event_smp_resp_t *)payload);
        }
      }
      break;

    case RSI_BLE_EVENT_SMP_PASSKEY:
      {
        if (ble_specific_cb->ble_on_smp_passkey_event != NULL) {
          ble_specific_cb->ble_on_smp_passkey_event((rsi_bt_event_smp_passkey_t *)payload);
        }
      }
      break;

    case RSI_BLE_EVENT_SMP_FAILED:
      {
        if (ble_specific_cb->ble_on_smp_fail_event != NULL) {
          ble_specific_cb->ble_on_smp_fail_event(status, (rsi_bt_event_smp_failed_t *)payload);
        }
      }
      break;

    case RSI_BLE_EVENT_ENCRYPT_STARTED:
      {
        if (ble_specific_cb->ble_on_smp_encrypt_started != NULL) {
          ble_specific_cb->ble_on_smp_encrypt_started(status, (rsi_bt_event_encryption_enabled_t *)payload);
        }
      }
      break;
    case RSI_BLE_EVENT_SMP_PASSKEY_DISPLAY_EVENT:
      {
        if (ble_specific_cb->ble_on_smp_passkey_display != NULL) {
          ble_specific_cb->ble_on_smp_passkey_display((rsi_bt_event_smp_passkey_display_t*)payload);
        }
      }
      break;    
    case RSI_BLE_RSP_PROFILES:
      {
        if (ble_specific_cb->ble_on_profiles_list_resp != NULL)
        {
          ble_specific_cb->ble_on_profiles_list_resp (status, (rsi_ble_resp_profiles_list_t  *)payload);
        }
        break;
      }
    case RSI_BLE_RSP_PROFILE:
      {
        if (ble_specific_cb->ble_on_profile_resp != NULL)
        {
          ble_specific_cb->ble_on_profile_resp (status, (profile_descriptors_t *)payload);
        }
        break;
      }
    case RSI_BLE_RSP_CHAR_SERVICES:
      {
        if (ble_specific_cb->ble_on_char_services_resp != NULL)
        {
          ble_specific_cb->ble_on_char_services_resp (status, (rsi_ble_resp_char_services_t*)payload);
        }
        break;
      }
    case RSI_BLE_RSP_INC_SERVICES:
      {
        if (ble_specific_cb->ble_on_inc_services_resp != NULL)
        {
          ble_specific_cb->ble_on_inc_services_resp (status, (rsi_ble_resp_inc_services_t*)payload);
        }
        break;
      }
    case RSI_BLE_RSP_DESC:
      {
        if (ble_specific_cb->ble_on_att_desc_resp != NULL)
        {
          ble_specific_cb->ble_on_att_desc_resp(status, (rsi_ble_resp_att_descs_t*)payload);
        }
        break;
      }
    case RSI_BLE_RSP_READ_BY_UUID:
    case RSI_BLE_RSP_READ_VAL:
    case RSI_BLE_RSP_MULTIPLE_READ:
    case RSI_BLE_RSP_LONG_READ:
      {
        if (ble_specific_cb->ble_on_read_resp != NULL)
        {
          ble_specific_cb->ble_on_read_resp (status, rsp_type, (rsi_ble_resp_att_value_t*)payload);
        }
        break;
      }
    case RSI_BLE_RSP_WRITE:
    case RSI_BLE_RSP_WRITE_NO_ACK:
    case RSI_BLE_RSP_LONG_WRITE:
    case RSI_BLE_RSP_PREPARE_WRITE:
    case RSI_BLE_RSP_EXECUTE_WRITE:
      {
        if (ble_specific_cb->ble_on_write_resp != NULL)
        {
          ble_specific_cb->ble_on_write_resp (status, rsp_type);
        }
        break;
      }
    case RSI_BLE_EVENT_GATT_NOTIFICATION:
    case RSI_BLE_EVENT_GATT_INDICATION:
    case RSI_BLE_EVENT_GATT_WRITE:
      if (ble_specific_cb->ble_on_gatt_events != NULL)
      {
        ble_specific_cb->ble_on_gatt_events (rsp_type, (rsi_ble_event_write_t *)payload);
      }
      break;
    case RSI_BLE_EVENT_MTU:
      if (ble_specific_cb->ble_on_mtu_event != NULL)
      {
        ble_specific_cb->ble_on_mtu_event ((rsi_ble_event_mtu_t *)payload);
      }
      break;

    case RSI_BLE_EVENT_LE_PING_TIME_EXPIRED:
      if (ble_specific_cb->ble_on_le_ping_time_expired_event != NULL)
      {
        ble_specific_cb->ble_on_le_ping_time_expired_event ((rsi_ble_event_le_ping_time_expired_t *)payload);
      }
      break;

    case RSI_BLE_EVENT_PREPARE_WRITE:
      if (ble_specific_cb->ble_on_prepare_write_event != NULL)
      {
        ble_specific_cb->ble_on_prepare_write_event (rsp_type, (rsi_ble_event_prepare_write_t *)payload);
      }
      break;

    case RSI_BLE_EVENT_EXECUTE_WRITE:
      if (ble_specific_cb->ble_on_execute_write_event != NULL)
      {
        ble_specific_cb->ble_on_execute_write_event (rsp_type, (rsi_ble_execute_write_t *)payload);
      }
      break;

    case RSI_BLE_EVENT_READ_REQ:
      if (ble_specific_cb->ble_on_read_req_event != NULL)
      {
        ble_specific_cb->ble_on_read_req_event (rsp_type, (rsi_ble_read_req_t *)payload);
      }
      break;

    case RSI_BLE_EVENT_PHY_UPDATE_COMPLETE:
      {
        if (ble_specific_cb->ble_on_phy_update_complete_event != NULL)
        {
          ble_specific_cb->ble_on_phy_update_complete_event ((rsi_ble_event_phy_update_t*)payload );
        }
      }
      break;
	  
	  case RSI_BLE_EVENT_DATA_LENGTH_UPDATE_COMPLETE:
        if (ble_specific_cb->rsi_ble_on_data_length_update_event != NULL)
        {
          ble_specific_cb->rsi_ble_on_data_length_update_event ( (rsi_ble_event_data_length_update_t *)payload);
        }
        break;

    case RSI_BLE_EVENT_SC_PASSKEY:
        {
          if (ble_specific_cb->ble_on_sc_passkey != NULL) {
            ble_specific_cb->ble_on_sc_passkey ((rsi_bt_event_sc_passkey_t *) payload);
          }
        }
        break;

    case RSI_BLE_EVENT_DIRECTED_ADV_REPORT:
        {
          if (ble_specific_cb->ble_on_directed_adv_report_event != NULL)
          {
            ble_specific_cb->ble_on_directed_adv_report_event ((rsi_ble_event_directedadv_report_t*)payload );
          }
        }
        break;
		
	case RSI_BLE_EVENT_LE_LTK_REQUEST:
      if (ble_specific_cb->ble_on_le_ltk_req_event != NULL)
      {
        ble_specific_cb->ble_on_le_ltk_req_event ((rsi_bt_event_le_ltk_request_t *)payload);
      }
      break;

    case RSI_BLE_EVENT_SECURITY_KEYS:
      {
        if (ble_specific_cb->ble_on_le_security_keys_event != NULL)
        {
          ble_specific_cb->ble_on_le_security_keys_event ((rsi_bt_event_le_security_keys_t *)payload);
        }
      }  
      break;

    case RSI_BLE_EVENT_PSM_CONN_REQ:
        {
          if (ble_specific_cb->ble_on_cbfc_conn_req_event != NULL)
          {
            ble_specific_cb->ble_on_cbfc_conn_req_event ((rsi_ble_event_cbfc_conn_req_t *)payload);
          }
        }
        break;

    case RSI_BLE_EVENT_PSM_CONN_COMPLETE:
        {
          if (ble_specific_cb->ble_on_cbfc_conn_complete_event != NULL)
          {
            ble_specific_cb->ble_on_cbfc_conn_complete_event ((rsi_ble_event_cbfc_conn_complete_t *)payload, status);
          }
        }
        break;

    case RSI_BLE_EVENT_PSM_RX_DATA:
        {
          if (ble_specific_cb->ble_on_cbfc_rx_data_event != NULL)
          {
            ble_specific_cb->ble_on_cbfc_rx_data_event ((rsi_ble_event_cbfc_rx_data_t *) payload);
          }
        }
        break;

    case RSI_BLE_EVENT_PSM_DISCONNECT:
        {
          if (ble_specific_cb->ble_on_cbfc_disconn_event != NULL)
          {
            ble_specific_cb->ble_on_cbfc_disconn_event((rsi_ble_event_cbfc_disconn_t *) payload);
          }
        }
        break;

   
    case RSI_BLE_EVENT_CONN_UPDATE_COMPLETE:
      {
        if (ble_specific_cb->ble_on_conn_update_complete_event != NULL)
        {
          ble_specific_cb->ble_on_conn_update_complete_event ((rsi_ble_event_conn_update_t*)payload, status );
        }
      }
      break;
    case RSI_BLE_EVENT_REMOTE_FEATURES:
      {
        if (ble_specific_cb->ble_on_remote_features_event != NULL)
        {
          ble_specific_cb->ble_on_remote_features_event ((rsi_ble_event_remote_features_t *)payload);
        }
      }
      break;
    case RSI_BLE_EVENT_LE_MORE_DATA_REQ:
      {
        if (ble_specific_cb->ble_on_le_more_data_req_event != NULL)
        {
          ble_specific_cb->ble_on_le_more_data_req_event ();
        }
      }
      break;
  }
}
#endif
/**
 * @fn          uint16_t rsi_bt_prepare_common_pkt(uint16_t cmd_type, void *cmd_struct, rsi_pkt_t *pkt)
 * @brief       This functions fills the BT command packet payload
 * @param[in]   cmd_type, type of the command
 * @param[in]   cmd_stuct, pointer of the command structure
 * @param[out]  pkt, pointer of the packet to fill the contents of the payload
 * @return      payload_size, size of the payload
 *              
 * @section description
 * This functions forms the payload of the BT command packet 
 */
uint16_t rsi_bt_prepare_common_pkt(uint16_t cmd_type, void *cmd_struct, rsi_pkt_t *pkt)
{
  uint16_t payload_size = 0;

  switch(cmd_type)
  {
    case RSI_BT_REQ_SET_LOCAL_COD:
    {
    	payload_size = sizeof(rsi_bt_req_set_local_cod_t);
    	memcpy(pkt->data, cmd_struct, payload_size);
    }
    break;

    case RSI_BT_SET_LOCAL_NAME:
    {
    	payload_size = sizeof(rsi_bt_req_set_local_name_t);
    	memcpy(pkt->data, cmd_struct, payload_size);
    }
    break;
    case RSI_BT_GET_RSSI:
      {
        payload_size = sizeof(rsi_bt_get_rssi_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_GET_LOCAL_DEV_ADDR:
    case RSI_BT_GET_LOCAL_NAME:
    case RSI_BT_REQ_QUERY_LOCAL_COD:
    case RSI_BT_REQ_INIT:
    case RSI_BT_REQ_DEINIT:
      break;
    case RSI_BT_SET_ANTENNA_SELECT:
      {
        payload_size = sizeof(rsi_ble_set_antenna_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_SET_FEATURES_BITMAP:
      {
        payload_size = sizeof(rsi_bt_set_feature_bitmap_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	case RSI_BT_SET_ANTENNA_TX_POWER_LEVEL:
      {
        payload_size = sizeof(rsi_bt_set_antenna_tx_power_level_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
			
  case RSI_BLE_ONLY_OPER_MODE:
      {
        payload_size = sizeof(rsi_ble_oper_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

  case RSI_BLE_REQ_PWRMODE:
      {
        payload_size = sizeof(rsi_ble_power_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	case RSI_BLE_REQ_SOFTRESET:
      break;
	case RSI_BT_REQ_PER_CMD:
      {
        pkt->data[0] = *(uint8_t *)cmd_struct;
        switch(pkt->data[0])
        {
          case HCI_BT_PER_STATS_CMD_ID:
            payload_size = 1;
            pkt->data[0] = *(uint8_t *)cmd_struct;
            break;
#ifdef RSI_BLE_ENABLE 
          case HCI_BLE_TRANSMIT_CMD_ID:
            payload_size = sizeof(rsi_ble_per_transmit_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;
          case HCI_BLE_RECEIVE_CMD_ID:
            payload_size = sizeof(rsi_ble_per_receive_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;
#endif
#ifdef RSI_BT_ENABLE
          case HCI_BT_TRANSMIT_CMD_ID: 
            payload_size = sizeof(rsi_bt_tx_per_params_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;
          case HCI_BT_RECEIVE_CMD_ID:  
            payload_size = sizeof(rsi_bt_rx_per_params_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;
#endif
        }
      }
      break;
      case RSI_BT_VENDOR_SPECIFIC:
      {
        pkt->data[0] = ((uint8_t *)cmd_struct)[0];
        pkt->data[1] = ((uint8_t *)cmd_struct)[1];
        switch((pkt->data[0] | (pkt->data[1] << 8)))
        {
          case BLE_VENDOR_RF_TYPE_CMD_OPCODE:
            payload_size = sizeof(rsi_ble_vendor_rf_type_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;  
          case BT_VENDOR_PTT_TYPE_CMD_OPCODE:
            payload_size = sizeof(rsi_bt_vendor_ptt_type_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;  
          case BT_VENDOR_AVDTP_STATS_CMD_OPCODE:
            payload_size = sizeof(rsi_bt_vendor_avdtp_stats_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;  
          default:
            break;
        }
      }
      break;
    default:
      break;
  }
  
  //! return payload_size
  return payload_size;
}

#ifdef RSI_BT_ENABLE
typedef uint8_t UINT08;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
#define  BT_MEM_WR_BE_16U(addr, val)   {(*(((UINT08     *)(addr)) + 0)) = ((UINT08    )((((UINT16    )(val)) &     0xFF00) >> (1 * 8))); \
                                        (*(((UINT08     *)(addr)) + 1)) = ((UINT08    )((((UINT16    )(val)) &     0x00FF) >> (0 * 8))); }

#define  BT_MEM_WR_BE_32U(addr, val)   {(*(((UINT08     *)(addr)) + 0)) = ((UINT08    )((((UINT32    )(val)) &     0xFF000000) >> (3 * 8))); \
                                        (*(((UINT08     *)(addr)) + 1)) = ((UINT08    )((((UINT32    )(val)) &     0x00FF0000) >> (2 * 8))); \
                                        (*(((UINT08     *)(addr)) + 2)) = ((UINT08    )((((UINT32    )(val)) &     0x0000FF00) >> (1 * 8))); \
                                        (*(((UINT08     *)(addr)) + 3)) = ((UINT08    )((((UINT32    )(val)) &     0x000000FF) >> (0 * 8)));}

/**
 * @fn          uint16_t rsi_bt_prepare_classic_pkt(uint16_t cmd_type, void *cmd_struct, rsi_pkt_t *pkt)
 * @brief       This functions fills the BT Classic command packet payload
 * @param[in]   cmd_type, type of the command
 * @param[in]   cmd_stuct, pointer of the command structure
 * @param[out]  pkt, pointer of the packet to fill the contents of the payload
 * @return      payload_size, size of the payload
 *              
 * @section description
 * This functions forms the payload of the BT Classic command packet 
 */
uint16_t rsi_bt_prepare_classic_pkt(uint16_t cmd_type, void *cmd_struct, rsi_pkt_t *pkt)
{
  uint16_t payload_size = 0;
  rsi_bt_req_spp_transfer_t *p_spp_data;
  rsi_bt_cb_t *bt_cb = rsi_driver_cb->bt_classic_cb;
  rsi_bt_iap_payload_t *iap_data;
  switch(cmd_type)
  {
	case RSI_BT_REQ_LINKKEY_REPLY:
      {
        payload_size = sizeof(rsi_bt_req_linkkey_reply_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        break;
      }
    case RSI_BT_REQ_SET_PROFILE_MODE:
      {
        payload_size = sizeof(rsi_bt_req_profile_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        break;
      }
    case RSI_BT_REQ_SET_DISCV_MODE:
      {
        payload_size = sizeof(rsi_bt_req_set_discv_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_QUERY_DISCV_MODE:
      {
        break;
      }
    case RSI_BT_REQ_SET_CONNECTABILITY_MODE:
      {
        payload_size = sizeof(rsi_bt_req_set_connectability_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_QUERY_CONNECTABILITY_MODE:
      {
        break;
      }
    case RSI_BT_REQ_SET_PAIR_MODE:
      {
        payload_size = sizeof(rsi_bt_req_set_pair_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_QUERY_PAIR_MODE:
      {
        break;
      }
    case RSI_BT_REQ_REMOTE_NAME_REQUEST:
      {
        payload_size = sizeof(rsi_bt_req_remote_name_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_REMOTE_NAME_REQUEST_CANCEL:
      {
        payload_size = sizeof(rsi_bt_req_remote_name_cancel_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_INQUIRY:
      {
        payload_size = sizeof(rsi_bt_req_inquiry_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_INQUIRY_CANCEL:
      break;
    case RSI_BT_REQ_BOND:
      {
        payload_size = sizeof(rsi_bt_req_connect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_BOND_CANCEL:
      {
        payload_size = sizeof(rsi_bt_req_connect_cancel_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_UNBOND:
      {
        payload_size = sizeof(rsi_bt_req_disconnect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_USER_CONFIRMATION:
      {
        payload_size = sizeof(rsi_bt_req_user_confirmation_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_PASSKEY_REPLY:
      {
        payload_size = sizeof(rsi_bt_req_passkey_reply_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_PINCODE_REPLY:
      {
        payload_size = sizeof(rsi_bt_req_pincode_reply_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_QUERY_ROLE:
      {
        payload_size = sizeof(rsi_bt_req_query_role_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_SET_ROLE:
      {
        payload_size = sizeof(rsi_bt_req_set_role_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_QUERY_SERVICES:
      {
        payload_size = sizeof(rsi_bt_req_query_services_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        bt_cb->sync_rsp = 0;
      }
      break;
    case RSI_BT_REQ_SEARCH_SERVICE:
      {
        payload_size = sizeof(rsi_bt_req_search_service_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        bt_cb->sync_rsp = 0;
      }
      break;
    case RSI_BT_REQ_SPP_CONNECT:
      {
        payload_size = sizeof(rsi_bt_req_connect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_SPP_DISCONNECT:
      {
        payload_size = sizeof(rsi_bt_req_disconnect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_IAP_SET_ACCESSORY_INFO:
      {
        payload_size = sizeof(rsi_bt_req_iap_accessory_info_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_IAP_SET_PROTOCOL_TYPE:
      {
        payload_size = sizeof(rsi_bt_req_iap_set_proto_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_IAP_CONN:
      {
        payload_size = sizeof(rsi_bt_req_iap_connect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_IAP_DISCONN:
      {
        payload_size = sizeof(rsi_bt_req_iap_disconnect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_IAP2_CONTROL_SESSION_DATA_TX :
      {
        
        iap_data = (rsi_bt_iap_payload_t *)cmd_struct;
        payload_size = iap_data->len;
        memcpy(pkt->data, iap_data->data, payload_size);
      
      }
      break;

     case RSI_BT_REQ_IAP2_RECV_CMD_LIST :
     case RSI_BT_REQ_IAP2_SEND_CMD_LIST :
      {
        payload_size = ((rsi_bt_iap_cmds_info_t *)cmd_struct)->len;
 
        memcpy(pkt->data, ((rsi_bt_iap_cmds_info_t *)cmd_struct)->cmd, payload_size);
      
      } 
     break;
    
     case RSI_BT_REQ_IAP2_SEND_FILE_TRANSFER_STATE :
     case RSI_BT_REQ_IAP2_SEND_FILE_TRANSFER_DATA :
      {
        payload_size = ((rsi_bt_iap_File_transfer_info_t *)cmd_struct)->len;

        memcpy(pkt->data, (uint8_t *)cmd_struct +2, payload_size);
      
      } 
     break; 

    case RSI_BT_REQ_IAP1_IDENTIFICATION:
    case RSI_BT_REQ_IAP2_IDENTIFICATION:
    case RSI_BT_REQ_IAP1_DEVICE_AUTHENTICATION:
    case RSI_BT_REQ_IAP2_INIT_FILE_TRANSFER:      
    case RSI_BT_REQ_IAP2_DE_INIT_FILE_TRANSFER:   
    
    break;
    
    case RSI_BT_REQ_SPP_TRANSFER:
      {
        //payload_size = sizeof(rsi_bt_req_spp_transfer_t);
        p_spp_data = (rsi_bt_req_spp_transfer_t *)cmd_struct;
        payload_size = (p_spp_data->data_length[0] + (p_spp_data->data_length[1] << 8)) + 2;
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_SNIFF_MODE:
      {
        payload_size = sizeof(rsi_bt_req_sniff_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case  RSI_BT_REQ_SNIFF_EXIT_MODE :
      {
        payload_size = sizeof(rsi_bt_req_sniff_exit_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case  RSI_BT_REQ_SNIFF_SUBRATING_MODE:
      {
        payload_size = sizeof(rsi_bt_req_sniff_subrating_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case  RSI_BT_REQ_SET_SSP_MODE:
      {
        payload_size = sizeof(rsi_bt_req_set_ssp_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }

      break;
	 case RSI_BT_REQ_SET_EIR :
      {
        payload_size = sizeof(rsi_bt_set_eir_data_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_CW_MODE:
      {
      }
      break;
    case RSI_BT_REQ_A2DP_CONNECT:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_connect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_A2DP_DISCONNECT:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_disconnect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
			
    case RSI_BT_REQ_A2DP_PCM_MP3_DATA:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_pcm_mp3_data_t);
        payload_size -= (sizeof(((rsi_bt_req_a2dp_pcm_mp3_data_t *)cmd_struct)->pcm_mp3_data) - ((rsi_bt_req_a2dp_pcm_mp3_data_t *)cmd_struct)->pcm_mp3_data_len);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
		
    case RSI_BT_REQ_A2DP_SBC_AAC_DATA:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_sbc_aac_data_t);
        payload_size -= (sizeof(((rsi_bt_req_a2dp_sbc_aac_data_t *)cmd_struct)->sbc_aac_data) - ((rsi_bt_req_a2dp_sbc_aac_data_t *)cmd_struct)->sbc_aac_data_len);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	 
    case RSI_BT_REQ_A2DP_CLOSE:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_close_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
		
    case RSI_BT_REQ_A2DP_ABORT:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_abort_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
		
    case RSI_BT_REQ_A2DP_START:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_start_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_A2DP_SUSPEND:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_suspend_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
		
    case RSI_BT_REQ_AVRCP_CONNECT:
	  {
        payload_size = sizeof(rsi_bt_req_avrcp_conn_t);
        memcpy(pkt->data, cmd_struct, payload_size);
	  }
      break;
    case RSI_BT_REQ_AVRCP_DISCONNECT:
      {
        payload_size = sizeof(rsi_bt_req_avrcp_disconnect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_PLAY:
	  {
        payload_size = sizeof(rsi_bt_req_avrcp_play_t);
        memcpy(pkt->data, cmd_struct, payload_size);
	  }
      break;
	case RSI_BT_REQ_AVRCP_PAUSE:
      {
        payload_size = sizeof(rsi_bt_req_avrcp_pause_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	case RSI_BT_REQ_AVRCP_STOP:
      {
        payload_size = sizeof(rsi_bt_req_avrcp_stop_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_NEXT:
	  {
        payload_size = sizeof(rsi_bt_req_avrcp_next_t);
        memcpy(pkt->data, cmd_struct, payload_size);
	  }
      break;
	case RSI_BT_REQ_AVRCP_PREVIOUS:
      {
        payload_size = sizeof(rsi_bt_req_avrcp_previous_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_VOL_UP:
	  {
        payload_size = sizeof(rsi_bt_req_avrcp_vol_up_t);
        memcpy(pkt->data, cmd_struct, payload_size);
	  }
      break;
    case RSI_BT_REQ_AVRCP_VOL_DOWN:
	  {
        payload_size = sizeof(rsi_bt_req_avrcp_vol_down_t);
        memcpy(pkt->data, cmd_struct, payload_size);
	  }
      break;

    case RSI_BT_REQ_HFP_CONNECT:
      {
        payload_size = sizeof(rsi_bt_req_hfp_connect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_HFP_DISCONNECT:
      {
        payload_size = sizeof(rsi_bt_req_hfp_disconnect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_HFP_PHONEOPERATOR:
      {
        payload_size = sizeof(rsi_bt_req_hfp_phoneoperator_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
	 break;
    case RSI_BT_REQ_HFP_CALLREJECT:
      {
        payload_size = sizeof(rsi_bt_req_hfp_callreject_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
	 break;
       case RSI_BT_REQ_HFP_DIALNUM:
      {
        payload_size = sizeof(rsi_bt_req_hfp_dialnum_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
      case RSI_BT_REQ_HFP_DIALMEM:
      {
        payload_size = sizeof(rsi_bt_req_hfp_dialmem_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
   case RSI_BT_REQ_HFP_VOICERECOGNITIONACTIVE:
      {
        payload_size = sizeof(rsi_bt_req_hfp_voicerecognitionactive_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
     break;
    case RSI_BT_REQ_HFP_VOICERECOGNITIONDEACTIVE:
     {
      payload_size = sizeof(rsi_bt_req_hfp_voicerecognitiondeactive_t);
       memcpy(pkt->data, cmd_struct, payload_size);
     }
     break;
    case RSI_BT_REQ_HFP_SPKGAIN:
     { 
      payload_size = sizeof(rsi_bt_req_hfp_spkgain_t);
       memcpy(pkt->data, cmd_struct, payload_size);
     }
     break;
     case RSI_BT_REQ_HFP_MICGAIN:
      {
       payload_size = sizeof(rsi_bt_req_hfp_micgain_t);
       memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
     case RSI_BT_REQ_HFP_GETCALLS:
      {
      payload_size = sizeof(rsi_bt_req_hfp_getcalls_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      } 
    break;
    case RSI_BT_REQ_HFP_AUDIO:
       {
       payload_size = sizeof(rsi_bt_req_hfp_audio_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
      case RSI_BT_REQ_PBAP_CONNECT:
      {
        payload_size = sizeof(rsi_bt_req_pbap_connect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_PBAP_DISCONNECT:
      {
        payload_size = sizeof(rsi_bt_req_pbap_disconnect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_PBAP_CONTACTS:
      {
        payload_size = sizeof(rsi_bt_req_pbap_contacts_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
    case RSI_BT_REQ_SET_AFH_HOST_CHANNEL_CLASSIFICATION:
      {                                                           
        payload_size = sizeof(rsi_bt_req_set_afh_host_channel_classification_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_GET_CAPABILITES:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_get_capabilities_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_GET_ATTS_LIST:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_get_att_list_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_GET_ATT_VALS_LIST:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_get_att_vals_list_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_GET_CUR_ATT_VAL:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_get_cur_att_val_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_SET_CUR_ATT_VAL:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_set_cur_att_val_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_GET_ELEMENT_ATT:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_get_ele_att_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_GET_PLAY_STATUS:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_get_player_status_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_REG_NOTIFICATION:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_reg_notification_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_REMOTE_VERSION:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_remote_version_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_AVRCP_ATT_TEXT:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_get_cur_att_val_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_ATT_VALS_TEXT:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_get_att_val_text_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_BATTERY_STATUS:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_batt_status_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_CHAR_SETS:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_cap_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_SET_ABS_VOL:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_set_abs_vol_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_GET_CAPABILITES_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_req_avrcp_char_sets_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_GET_ATTS_LIST_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_att_list_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_GET_ATT_VALS_LIST_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_att_vals_list_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_GET_CUR_ATT_VAL_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_cur_att_vals_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_SET_CUR_ATT_VAL_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_set_att_vals_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_GET_ELEMENT_ATT_RESP:
      {                                                           
        //payload_size = sizeof(rsi_bt_avrcp_elem_attr_resp_t);
        //memcpy(pkt->data, cmd_struct, payload_size);
        uint16_t data_idx = 0;
        uint8_t ix = 0;
        rsi_bt_avrcp_elem_attr_resp_t *elem_attr = cmd_struct;

        memcpy(pkt->data, elem_attr, 9); data_idx += 9;
        for (ix = 0; ix < elem_attr->elem_attr_list.num_attrs; ix++) {
          uint16_t tmp_len = 0;
          tmp_len = elem_attr->elem_attr_list.attr_list[ix].attr_len;
          BT_MEM_WR_BE_32U(pkt->data + data_idx, elem_attr->elem_attr_list.attr_list[ix].id); data_idx += 4;
          BT_MEM_WR_BE_16U(pkt->data + data_idx, elem_attr->elem_attr_list.attr_list[ix].char_set_id); data_idx += 2;
          BT_MEM_WR_BE_16U(pkt->data + data_idx, tmp_len); data_idx += 2;
          memcpy(pkt->data + data_idx, elem_attr->elem_attr_list.attr_list[ix].attr_val,
              tmp_len);
          data_idx += tmp_len; 
        }
        payload_size = data_idx;
      }
      break;

    case RSI_BT_REQ_AVRCP_GET_PLAY_STATUS_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_play_status_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_REG_NOTIFICATION_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_reg_notify_interim_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_ATT_TEXT_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_att_text_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_ATT_VALS_TEXT_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_att_text_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_BATTERY_STATUS_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_reg_notify_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_CHAR_SETS_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_reg_notify_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_NOTIFICATION:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_notify_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_AVRCP_CMD_REJECT:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_cmd_reject_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    
    case RSI_BT_REQ_AVRCP_SET_ABS_VOL_RESP:
      {                                                           
        payload_size = sizeof(rsi_bt_avrcp_set_abs_vol_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

		case RSI_BT_REQ_ENABLE_DEVICE_UNDER_TESTMODE:
		break;
    case RSI_BT_REQ_A2DP_GET_CONFIG:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_get_config_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_A2DP_SET_CONFIG:
      {
        payload_size = sizeof(rsi_bt_req_a2dp_set_config_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_ADD_DEVICE_ID:
    {
      payload_size = sizeof(rsi_bt_req_add_device_id_t);
      memcpy(pkt->data,cmd_struct,payload_size);
    }
    break;
    case RSI_BT_REQ_SET_CURRENT_IAC_LAP:
    {
      payload_size = sizeof(rsi_bt_req_set_current_iap_lap_t);
      memcpy(pkt->data,cmd_struct,payload_size);
    }
    break;
    case RSI_BT_REQ_HID_CONNECT:
    case RSI_BT_REQ_HID_DISCONNECT:
      {
        payload_size = sizeof(rsi_bt_req_connect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_HID_PROFILE_DATA:
      {
        rsi_bt_hid_profile_data_t *hid_profile_data = (rsi_bt_hid_profile_data_t*)cmd_struct;
        payload_size = (sizeof(rsi_bt_hid_profile_data_t) - (MAX_PROFILE_DATA_BUFFER_LEN) + (*(uint16_t*)&hid_profile_data->len[0]));
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BT_REQ_HID_SDP_ATT_INIT:
      {
        payload_size = sizeof(rsi_sdp_att_record_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BT_REQ_GATT_CONNECT:
      {
        payload_size = sizeof (rsi_bt_req_gatt_connect_t);
        memcpy (pkt->data, cmd_struct, payload_size);
      }
      break;
	  
    default:
      break;
  }
  
  //! return payload_size
  return payload_size;
}
#endif

#ifdef RSI_BLE_ENABLE
/**
 * @fn          uint16_t rsi_bt_prepare_le_pkt(uint16_t cmd_type, void *cmd_struct, rsi_pkt_t *pkt)
 * @brief       This functions fills the LE command packet payload
 * @param[in]   cmd_type, type of the command
 * @param[in]   cmd_stuct, pointer of the command structure
 * @param[out]  pkt, pointer of the packet to fill the contents of the payload
 * @return      payload_size, size of the payload
 *              
 * @section description
 * This functions forms the payload of the LE command packet 
 */
uint16_t rsi_bt_prepare_le_pkt(uint16_t cmd_type, void *cmd_struct, rsi_pkt_t *pkt)
{
  uint16_t payload_size = 0;
  rsi_bt_cb_t *le_cb = rsi_driver_cb->ble_cb;

  switch(cmd_type)
  {
    case RSI_BLE_REQ_ADV:
      {
        payload_size = sizeof(rsi_ble_req_adv_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BLE_SET_ADVERTISE_DATA:
      {
        payload_size = sizeof(rsi_ble_req_adv_data_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	case RSI_BLE_SET_SCAN_RESPONSE_DATA:
      {
        payload_size = sizeof(rsi_ble_req_scanrsp_data_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;  
    case RSI_BLE_REQ_SCAN:
      {
        payload_size = sizeof(rsi_ble_req_scan_t);
        memcpy(pkt->data, cmd_struct, payload_size); 
      }
      break;
    case RSI_BLE_REQ_CONN:
      {
        payload_size = sizeof(rsi_ble_req_conn_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	  
    case RSI_BLE_CMD_CONN_PARAMS_UPDATE:
      {
        payload_size = sizeof(rsi_ble_cmd_conn_params_update_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BLE_REQ_DISCONNECT:
      {
        payload_size = sizeof(rsi_ble_req_disconnect_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BLE_REQ_START_ENCRYPTION:
      {
        payload_size = sizeof(rsi_ble_strat_encryption_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BLE_REQ_SMP_PAIR:
      {
        payload_size = sizeof(rsi_ble_req_smp_pair_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BLE_SMP_PAIR_RESPONSE:
      {
        payload_size = sizeof(rsi_ble_smp_response_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BLE_SMP_PASSKEY:
      {
        payload_size = sizeof(rsi_ble_smp_passkey_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

      //GATT cases
    case RSI_BLE_REQ_PROFILES:
      {
        payload_size = sizeof(rsi_ble_req_profiles_list_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_PROFILE:
      {
        payload_size = sizeof(rsi_ble_req_profile_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_CHAR_SERVICES:
      {
        payload_size = sizeof(rsi_ble_req_char_services_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_INC_SERVICES:
      {
        payload_size = sizeof(rsi_ble_req_inc_services_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_READ_BY_UUID:
      {
        payload_size = sizeof(rsi_ble_req_char_val_by_uuid_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_DESC:
      {
        payload_size = sizeof(rsi_ble_req_att_descs_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_READ_VAL:
      {
        payload_size = sizeof(rsi_ble_req_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_MULTIPLE_READ:
      {
        payload_size = sizeof(rsi_ble_req_multi_att_values_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_LONG_READ:
      {
        payload_size = sizeof(rsi_ble_req_long_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_WRITE:
      {
        payload_size = sizeof(rsi_ble_set_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_WRITE_NO_ACK:
      {
        payload_size = sizeof(rsi_ble_set_att_cmd_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_LONG_WRITE:
      {
        payload_size = sizeof(rsi_ble_set_long_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_PREPARE_WRITE:
      {
        payload_size = sizeof(rsi_ble_req_prepare_write_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_REQ_EXECUTE_WRITE:
      {
        payload_size = sizeof(rsi_ble_req_execute_write_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;
    case RSI_BLE_ADD_SERVICE:
      {
        payload_size = sizeof(rsi_ble_req_add_serv_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BLE_ADD_ATTRIBUTE:
      {
        payload_size = sizeof(rsi_ble_req_add_att_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BLE_SET_LOCAL_ATT_VALUE:
      {
        payload_size = sizeof(rsi_ble_set_local_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BLE_CMD_NOTIFY:
      {
        payload_size = sizeof(rsi_ble_notify_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	case RSI_BLE_CMD_INDICATE:
      {
        payload_size = sizeof(rsi_ble_notify_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BLE_GET_LOCAL_ATT_VALUE:
      {
        payload_size = sizeof(rsi_ble_get_local_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
    case RSI_BLE_GET_LE_PING:
      {
        payload_size = sizeof(rsi_ble_get_le_ping_timeout_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BLE_SET_LE_PING:
      {
        payload_size = sizeof(rsi_ble_set_le_ping_timeout_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BLE_ENCRYPT:
       {
         payload_size = sizeof(rsi_ble_encrypt_t);
         memcpy(pkt->data, cmd_struct, payload_size);
       }
       break;

    case RSI_BLE_SET_RANDOM_ADDRESS:
      {
        uint8_t dummy_rand_addr[6] = {0}; 
        rsi_ble_req_rand_t *rsi_ble_rand = (rsi_ble_req_rand_t *)pkt->data;
        memcpy(rsi_ble_rand, cmd_struct, sizeof(rsi_ble_req_rand_t));
        if(memcmp(rsi_ble_rand->rand_addr, dummy_rand_addr, 6) == 0)     
        {
        rsi_ascii_dev_address_to_6bytes_rev(rsi_ble_rand->rand_addr, (int8_t *)RSI_BLE_SET_RAND_ADDR);
        }
        //! fill payload size
        payload_size = sizeof(rsi_ble_req_rand_t);
      }
	  break;
	  
	  case RSI_BLE_CMD_READ_RESP:
      {
        payload_size = sizeof(rsi_ble_gatt_read_response_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BLE_LE_WHITE_LIST:
      {
        payload_size = sizeof(rsi_ble_white_list_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	  
	  case RSI_BLE_RSP_REMOVE_SERVICE:
      {
        payload_size = sizeof(rsi_ble_gatt_remove_serv_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	  
  	  case RSI_BLE_RSP_REMOVE_ATTRIBUTE:
      {
        payload_size = sizeof(rsi_ble_gatt_remove_att_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	  
	  case RSI_BLE_PROCESS_RESOLV_LIST:
      {
        payload_size = sizeof(rsi_ble_resolvlist_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

      case RSI_BLE_GET_RESOLVING_LIST_SIZE:
      break;
	  
      case RSI_BLE_SET_ADDRESS_RESOLUTION_ENABLE:
      {
        payload_size = sizeof(rsi_ble_set_addr_resolution_enable_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	  
      case RSI_BLE_SET_PRIVACY_MODE:
      {
        payload_size = sizeof(rsi_ble_set_privacy_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

      case RSI_BLE_REQ_READ_PHY:
      {
        payload_size = sizeof(rsi_ble_req_read_phy_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	  
      case RSI_BLE_REQ_SET_PHY:
      {
        payload_size = sizeof(rsi_ble_set_phy_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
	  
      case RSI_BLE_SET_DATA_LEN:
      {
        payload_size = sizeof(rsi_ble_setdatalength_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break; 

      case RSI_BLE_CBFC_CONN_REQ:
      {
        payload_size = sizeof(rsi_ble_cbfc_conn_req_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

      case RSI_BLE_CBFC_CONN_RESP:
      {
        payload_size = sizeof(rsi_ble_cbfc_conn_resp_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

      case RSI_BLE_CBFC_TX_DATA:
      {
        payload_size = sizeof(rsi_ble_cbfc_data_tx_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

      case RSI_BLE_CBFC_DISCONN:
      {
        payload_size = sizeof(rsi_ble_cbfc_disconn_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

    case RSI_BLE_CMD_ATT_ERROR:
      {
        payload_size = sizeof(rsi_ble_att_error_response_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break; 


    case RSI_BLE_LE_LTK_REQ_REPLY:
      {
        payload_size = sizeof(rsi_ble_set_le_ltkreqreply_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break; 

      case RSI_BLE_RX_TEST_MODE:
      {
        payload_size = sizeof(rsi_ble_rx_test_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;

      case RSI_BLE_TX_TEST_MODE:
      {
        payload_size = sizeof(rsi_ble_tx_test_mode_t);
        memcpy(pkt->data, cmd_struct, payload_size);
      }
      break;
       case RSI_BLE_REQ_PROFILES_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_profiles_list_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

      case RSI_BLE_REQ_PROFILE_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_profile_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;
case RSI_BLE_GET_CHARSERVICES_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_char_services_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_GET_INCLUDESERVICES_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_inc_services_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_READCHARVALUEBYUUID_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_char_val_by_uuid_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_GET_ATTRIBUTE_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_att_descs_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_GET_DESCRIPTORVALUE_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_GET_MULTIPLEVALUES_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_multi_att_values_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

    case RSI_BLE_GET_LONGDESCVALUES_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_long_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;
    case RSI_BLE_SET_DESCVALUE_ASYNC:
      {
        payload_size = sizeof(rsi_ble_set_att_value_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;
    case RSI_BLE_SET_PREPAREWRITE_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_prepare_write_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;
   case RSI_BLE_EXECUTE_LONGDESCWRITE_ASYNC:
      {
        payload_size = sizeof(rsi_ble_req_execute_write_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;
    case RSI_BLE_SET_SMP_PAIRING_CAPABILITY_DATA:
      {
        payload_size = sizeof(rsi_ble_set_smp_pairing_capabilty_data_t);
        memcpy(pkt->data, cmd_struct, payload_size);
        le_cb->sync_rsp = 0;
      }
      break;

      case RSI_BLE_END_TEST_MODE:
      break;
      case RSI_ANT_CMD:
      {
        pkt->data[0] = ((uint8_t *)cmd_struct)[0];
        switch(pkt->data[0])
        {
          case ANT_ATM_CMD:
            payload_size = sizeof(rsi_ant_atm_mode_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;
          case ANT_GET_VERSION:
            payload_size = sizeof(rsi_ant_get_ver_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;
          case ANT_GET_STATS:
            payload_size = sizeof(rsi_ant_get_stats_t);
            memcpy(pkt->data, cmd_struct, payload_size);
            break;
        }
      }
       break;
    default:
      break;
  }

  //! return payload_size
  return payload_size;
}
#endif

/**
 * @fn          int32_t rsi_bt_driver_send_cmd(uint16_t cmd, void *cmd_struct, void *resp)
 * @brief       This functions fills commands and places into Bt TX queue
 * @param[in]   cmd, type of the command to send 
 * @param[in]   cmd_stuct, pointer of the packet structure to send
 * @param[out]
 * @return     
 *              !0 - If fails
 *              0  - If success
 *
 * @section description
 * This functions fills commands and places into BT TX queue 
 *
 *
 */
int32_t rsi_bt_driver_send_cmd(uint16_t cmd, void *cmd_struct, void *resp)
{
  uint16_t payload_size = 0;
  uint16_t protocol_type = 0;
  int32_t  status       = RSI_SUCCESS;
  rsi_pkt_t *pkt        = NULL;
  uint8_t *host_desc    = NULL;
  rsi_bt_cb_t *bt_cb    = NULL;

  protocol_type = rsi_bt_get_proto_type(cmd, &bt_cb);

  if(protocol_type == 0xFF)
  {
    //! unlock mutex
    rsi_mutex_unlock(&rsi_driver_cb->bt_single_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  //! take lock on ble control block
  rsi_mutex_lock(&rsi_driver_cb->bt_single_mutex);


#ifdef BT_STACK_IN_HOST
  //! If allocation of packet fails
  if(!bt_stack_buf_avail)
  {
    //! unlock mutex
    rsi_mutex_unlock(&rsi_driver_cb->bt_single_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
#endif

  //! allocate command buffer from ble pool
  pkt = rsi_pkt_alloc(&bt_cb->bt_tx_pool);

  //! Get host descriptor pointer
  host_desc = (pkt->desc);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&rsi_driver_cb->bt_single_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  //! Memset host descriptor
  memset(host_desc, 0, RSI_HOST_DESC_LENGTH);


  bt_cb->sync_rsp = 1;

  payload_size = 0; 

  if(protocol_type == RSI_PROTO_BT_COMMON)
  {
    //! Memset data
    memset(pkt->data, 0, (RSI_BT_COMMON_CMD_LEN - sizeof(rsi_pkt_t)));
    payload_size = rsi_bt_prepare_common_pkt(cmd, cmd_struct, pkt);
    rsi_driver_cb->bt_single_tx_q = &rsi_driver_cb->bt_common_tx_q;
  }
#ifdef RSI_BT_ENABLE
  else if(protocol_type == RSI_PROTO_BT_CLASSIC)
  {
    //! Memset data
    memset(pkt->data, 0, (RSI_BT_CLASSIC_CMD_LEN - sizeof(rsi_pkt_t)));
    payload_size = rsi_bt_prepare_classic_pkt(cmd, cmd_struct, pkt);
    rsi_driver_cb->bt_single_tx_q = &rsi_driver_cb->bt_classic_tx_q;
  }
#endif
#ifdef RSI_BLE_ENABLE
  else if(protocol_type == RSI_PROTO_BLE)
  {
    //! Memset data
    memset(pkt->data, 0, (RSI_BLE_CMD_LEN - sizeof(rsi_pkt_t)));
    payload_size = rsi_bt_prepare_le_pkt(cmd, cmd_struct, pkt);
    rsi_driver_cb->bt_single_tx_q = &rsi_driver_cb->ble_tx_q;
  }
#endif
  //! Fill payload length
  rsi_uint16_to_2bytes(host_desc, (payload_size & 0xFFF));

  //! Fill frame type
  host_desc[1] |= (RSI_BT_Q << 4);

  //! Fill frame type
  rsi_uint16_to_2bytes(&host_desc[2], cmd);


  //! Save expected response type
  bt_cb->expected_response_type = cmd;

  //! Save expected response type
  bt_cb->expected_response_buffer = resp;

#ifdef SAPIS_BT_STACK_ON_HOST
	send_cmd_from_app_to_bt_stack(host_desc, pkt->data, payload_size);
#else
 	 //! Enqueue packet to BT TX queue
  	rsi_enqueue_pkt(rsi_driver_cb->bt_single_tx_q, pkt);

  	//! Set TX packet pending event
  	rsi_set_event(RSI_TX_EVENT);
#endif

#ifdef SAPIS_BT_STACK_ON_HOST
  if(bt_cb->sync_rsp)
  {
#endif
	  //! Wait on BT semaphore
	  rsi_semaphore_wait(&bt_cb->bt_sem, RSI_WAIT_FOREVER);

  //! get command response status
  status =  rsi_bt_get_status(bt_cb);

  //! free the packet
  rsi_pkt_free(&bt_cb->bt_tx_pool, pkt);

  //! release lock
  rsi_mutex_unlock(&rsi_driver_cb->bt_single_mutex);
		
  if (cmd == RSI_BLE_ONLY_OPER_MODE)
  {
		  //! Save expected response type
    bt_cb->expected_response_type = RSI_BT_EVENT_CARD_READY;
    bt_cb->sync_rsp = 1;

	rsi_semaphore_wait(&bt_cb->bt_sem, RSI_WAIT_FOREVER);
  }
#ifdef SAPIS_BT_STACK_ON_HOST
  }
#endif
  //! Return status
  return status;

}

#ifndef SAPIS_BT_STACK_ON_HOST
 //Start BT-BLE Stack
int32_t intialize_bt_stack(uint8_t mode)
{
	//Dummy Function
	return 0;
}

#endif
#endif

/*==============================================*/

