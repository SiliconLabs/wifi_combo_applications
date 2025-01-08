/**
 * @file     rsi_bt_spp_apis.c
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
 *  @brief : This file contains BT SPP API's which needs to be called from application
 *
 *  @section Description  This file contains BT SPP API's called from application
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
 * @fn         rsi_bt_spp_init
 * @brief      sets profile mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the profile mode.
 */
int32_t rsi_bt_spp_init(void)
{ 
  rsi_bt_req_profile_mode_t bt_req_spp_init = {1};
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_PROFILE_MODE, &bt_req_spp_init, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_spp_connect
 * @brief      requests SPP connection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to initiate SPP connection.
 * */
int32_t rsi_bt_spp_connect(uint8_t *remote_dev_addr)
{
  rsi_bt_req_connect_t bt_req_spp_connect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_spp_connect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SPP_CONNECT, &bt_req_spp_connect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_spp_disconnect
 * @brief      requests SPP disconnection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for SPP disconnection.
 * */
int32_t rsi_bt_spp_disconnect(uint8_t *remote_dev_addr)
{
  rsi_bt_req_disconnect_t bt_req_spp_disconnect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_spp_disconnect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SPP_DISCONNECT, &bt_req_spp_disconnect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_spp_transfer
 * @brief      requests SPP transfer.
 * @param[in]  remote_dev_address, remote device address
 * @param[in]  data, data for transmission
 * @param[in]  length, data length for transfer
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for SPP data transfer.
 * */
int32_t rsi_bt_spp_transfer(uint8_t *remote_dev_addr, uint8_t *data, uint16_t length)
{
  uint16_t xfer_len = 0;
  
  rsi_bt_req_spp_transfer_t bt_req_spp_transfer = {{0}};
  xfer_len = RSI_MIN(length, RSI_BT_MAX_PAYLOAD_SIZE);
  bt_req_spp_transfer.data_length[0] = (uint8_t)(xfer_len & 0x00FF);
  bt_req_spp_transfer.data_length[1] = (xfer_len & 0xFF00) >> 8;
 
  memcpy(bt_req_spp_transfer.data, data, xfer_len);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SPP_TRANSFER, &bt_req_spp_transfer, NULL);
}
#endif
