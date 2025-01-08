/**
 * @file     rsi_ble_l2cap_conn.c
 * @version  0.1
 * @date     18 Aug 2017
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains BLE GATT API's which needs to be called from application
 *
 *  @section Description  This file contains BLE GATT API's called from application
 *
 *
 */
#ifdef RSI_BLE_ENABLE		
#include "rsi_driver.h"
#include "rsi_ble.h"
#include "rsi_ble_apis.h"
#include "rsi_ble_config.h"
#include "rsi_bt_common_apis.h"

/* NOTE***************************************************************
 * For all the API's device address shall be provided as ASCII string.
 * Ex: "00:12:34:AB:CD:EF"
 *********************************************************************/

//! LE L2CAP credit based flow control feature functions

/*==============================================*/
/**
 * @fn         rsi_ble_cbfc_connreq 
 * @brief      initiating the cbfc connection request with remote device.
 * @param[in]  dev_addr, remote device BD addr.
 * @param[in]  psm, protocol service multiplexing id.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the local attribute value
 */
uint32_t rsi_ble_cbfc_connreq (uint8_t *dev_addr,
                               uint16_t psm)
{
  rsi_ble_cbfc_conn_req_t  conn_req = {{0}, 0};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)conn_req.dev_addr, (int8_t *)dev_addr);
  conn_req.psm = psm;

  return rsi_bt_driver_send_cmd(RSI_BLE_CBFC_CONN_REQ, &conn_req , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_cbfc_connresp 
 * @brief      responding the cbfc connection request received from remote device.
 * @param[in]  dev_addr, remote device BD addr.
 * @param[in]  lcid, local channel id, received from CBFC conn request event.
 * @param[in]  result, accept/reject the connection request.
 *             0 - reject the connection request
 *             !0 - accept the connection request
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the local attribute value
 */
uint32_t rsi_ble_cbfc_connresp (uint8_t *dev_addr,
                                uint16_t lcid,
                                uint8_t  result)
{
  rsi_ble_cbfc_conn_resp_t  conn_resp = {{0}, 0};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)conn_resp.dev_addr, (int8_t *)dev_addr);
  conn_resp.lcid   = lcid;
  conn_resp.result = result;

  return rsi_bt_driver_send_cmd(RSI_BLE_CBFC_CONN_RESP, &conn_resp , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_cbfc_data_tx 
 * @brief      transfer the cbfc data to remote device.
 * @param[in]  dev_addr, remote device BD addr.
 * @param[in]  lcid, local channel id, received from CBFC conn request event.
 * @param[in]  len, length of the data.
 * @param[in]  p_data, linear data buffer.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the local attribute value
 */
uint32_t rsi_ble_cbfc_data_tx (uint8_t  *dev_addr,
                               uint16_t  lcid,
                               uint16_t  len,
                               uint8_t  *p_data)
{
  rsi_ble_cbfc_data_tx_t  tx_data = {{0}, 0};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)tx_data.dev_addr, (int8_t *)dev_addr);
  tx_data.lcid = lcid;
  tx_data.len  = RSI_MIN(len, sizeof (tx_data.data));
  memcpy (tx_data.data, p_data, tx_data.len);

  return rsi_bt_driver_send_cmd(RSI_BLE_CBFC_TX_DATA, &tx_data , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_cbfc_disconnect 
 * @brief      disconnect the cbfc connection with remote device.
 * @param[in]  dev_addr, remote device BD addr.
 * @param[in]  lcid, local channel id, received from CBFC conn request event.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the local attribute value
 */
uint32_t rsi_ble_cbfc_disconnect (uint8_t  *dev_addr,
                                  uint16_t  lcid)
{
  rsi_ble_cbfc_disconn_t  disconn = {{0}, 0};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)disconn.dev_addr, (int8_t *)dev_addr);
  disconn.lcid = lcid;

  return rsi_bt_driver_send_cmd(RSI_BLE_CBFC_DISCONN, &disconn , NULL);
}

#endif
