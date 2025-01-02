/**
 * @file     rsi_bt_common_apis.c
 * @version  0.1
 * @date     01 Oct 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains BLE GAP API's which needs to be called from application
 *
 *  @section Description  This file contains BLE GAP API's called from application
 *
 *
 */
#if  (defined(RSI_BT_ENABLE)	|| defined(RSI_BLE_ENABLE))		
#include<rsi_driver.h>
#include<rsi_bt_common.h>
#include<rsi_bt.h>

/*==============================================*/
/**
 * @fn         rsi_bt_set_local_name
 * @brief      sets the local BT/BLE device name
 * @param[in]  local_name, name to be set to the local BT/BLE device
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function sets the given name to the local BT/BLE device
 */
int32_t rsi_bt_set_local_name(int8_t *local_name)
{
  uint8_t name_len;
  rsi_bt_req_set_local_name_t bt_req_set_local_name = {0};
  name_len = RSI_MIN(strlen((const char *)local_name), (RSI_DEV_NAME_LEN-1));

  memcpy(bt_req_set_local_name.name, local_name, name_len);
  bt_req_set_local_name.name[name_len] = 0;

  return rsi_bt_driver_send_cmd(RSI_BT_SET_LOCAL_NAME, &bt_req_set_local_name, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_get_local_name
 * @brief      requests the local BT/BLE device name
 * @param[out] resp, response buffer to hold the response of this API
 * @return     int32_t 
 *             0  = success
 *             !0 = failure 
 * @section description
 * This function is used know the name of the local BT/BLE device
 */
int32_t rsi_bt_get_local_name(rsi_bt_resp_get_local_name_t *bt_resp_get_local_name)
{
  return rsi_bt_driver_send_cmd(RSI_BT_GET_LOCAL_NAME, NULL, bt_resp_get_local_name);
}

/*==============================================*/
/**
 * @fn         rsi_bt_get_rssi 
 * @brief      request the RSSI of the connected BT/BLE device
 * @param[out] resp, response buffer to hold the response of this API
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used know the RSSI of the connected BT/BLE device
 */
int32_t rsi_bt_get_rssi(int8_t *dev_addr, uint8_t *resp)
{
  rsi_bt_get_rssi_t bt_rssi = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_rssi.dev_addr, dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_GET_RSSI, &bt_rssi, resp);
}

/*==============================================*/
/**
 * @fn         rsi_bt_get_device_address
 * @brief      request the local BT/BLE device address 
 * @param[out] resp, response buffer to hold the response of this API
 * @return     int32_t 
 *             0  = success
 *             !0 = failure 
 * @section description
 * This function is used to know the local BT/BLE device address
 */
int32_t rsi_bt_get_local_device_address(uint8_t *resp)
{
  return rsi_bt_driver_send_cmd(RSI_BT_GET_LOCAL_DEV_ADDR, NULL, resp);
}

/*==============================================*/
/**
 * @fn         rsi_bt_init
 * @brief      Initializes the BT/BLE device
 * @param[in]  void
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to initialise the BT/BLE device.
 * Its recommended to use this API after rsi_bt_deinit API
 */
int32_t rsi_bt_init(void)
{
    return rsi_bt_driver_send_cmd(RSI_BT_REQ_INIT, NULL, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_deinit
 * @brief      de-initializes/stops the BT/BLE device
 * @param[in]  void
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to de-initialize the BT/BLE device.
 * rsi_bt_init API shall be used after rsi_bt_deinit API
 */
int32_t rsi_bt_deinit(void)
{
    return rsi_bt_driver_send_cmd(RSI_BT_REQ_DEINIT, NULL, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_set_antenna
 * @brief      sets the antenna
 * @param[in]  antenna_value, either internal/external antenna
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to select either internal/external antenna on the chip
 */
int32_t rsi_bt_set_antenna(uint8_t antenna_value)
{
  rsi_ble_set_antenna_t ble_set_antenna = {0};
 
  ble_set_antenna.value = antenna_value;
  return rsi_bt_driver_send_cmd(RSI_BT_SET_ANTENNA_SELECT, &ble_set_antenna, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_set_feature_bitmap
 * @brief      set or enable the  BT or BLE features at firmware
 * @param[in]  feature_bit_map, features bit map list
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set or select the features bit map
 */
int32_t rsi_bt_set_feature_bitmap(uint32_t feature_bit_map)
{
  rsi_bt_set_feature_bitmap_t bt_features = {0};
 
  bt_features.bit_map = feature_bit_map;
  return rsi_bt_driver_send_cmd(RSI_BT_SET_FEATURES_BITMAP, &bt_features, NULL);
}



/*==============================================*/
/**
 * @fn         rsi_bt_set_antenna_tx_power_level
 * @brief      sets the antenna transmit power levell
 * @param[in]  protocol mode , tx-power -power value 
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the transmit power of antenna on the chip
 */
int32_t rsi_bt_set_antenna_tx_power_level(uint8_t protocol_mode, int8_t tx_power)
{
  rsi_bt_set_antenna_tx_power_level_t bt_set_antenna_tx_power_level = {0};
 
  bt_set_antenna_tx_power_level.protocol_mode = protocol_mode;
  bt_set_antenna_tx_power_level.tx_power = tx_power;
  return rsi_bt_driver_send_cmd(RSI_BT_SET_ANTENNA_TX_POWER_LEVEL, &bt_set_antenna_tx_power_level, NULL);
}
/*==============================================*/
/**
 * @fn          int32_t rsi_bt_power_save_profile(uint8_t psp_mode, uint8_t psp_type)
 * @brief       Sets power save profile
 * @param[in]   psp_mode.The options are : 
 *              0 - Active
 *              1 - Sleep without SoC turn off when connected else disconnected deep sleep
 *              2 - Sleep with SoC turn off (connected/disconnected)
 *              8 - Deep sleep
 * @param[in]   psp_type. The options are :
 *              0 - Max power save
 *              1 - Fast PSP
 *              2 - UAPSD 
 * @param[out]  none
 * @return      command response status
 *
 * @section description
 * This function sets the different power save profile
 *
 */
int32_t rsi_bt_power_save_profile(uint8_t psp_mode, uint8_t psp_type)
{
  int32_t status = RSI_SUCCESS;
  
  //!Get commmon cb pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //!Updating present bt power save type
  rsi_common_cb->power_save.bt_psp_type = psp_type;

  //!Updating present bt power save mode
  rsi_common_cb->power_save.bt_psp_mode = psp_mode;
  
  status = rsi_sleep_mode_decision(rsi_common_cb);

  return status; 
}

/*==============================================*/
/**
 * @fn          int32_t rsi_bt_per_stats(uint8_t cmd_type, struct rsi_bt_per_stats_s *rsi_bt_per_stats)
 * @param[in]   cmd_type: 
 * @param[in]  rsi_bt_per_stats pointer address which contains the response strucutre.
 * @return      command response status
 *
 * @section description
 * This function sets the different power save profile
 *
 */
int32_t rsi_bt_per_stats(uint8_t cmd_type, struct rsi_bt_per_stats_s *per_stats)
{
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_PER_CMD, &cmd_type, per_stats);
}

/**
 * @fn         rsi_ble_vendor_avdtp_stats_enable
 * @brief      This api gives vendor specific command to the controller to set avdtp stats enable.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to issue vendor specific command for setting avdtp stats enable in controller on given inputs.
 * 
 */
int32_t rsi_ble_vendor_avdtp_stats_enable (uint8_t avdtp_stats_enable)
{
    rsi_bt_vendor_avdtp_stats_t  rsi_bt_vendor_avdtp_stats;
    
    rsi_bt_vendor_avdtp_stats.opcode[0] = (BT_VENDOR_AVDTP_STATS_CMD_OPCODE & 0xFF);
    rsi_bt_vendor_avdtp_stats.opcode[1] = ((BT_VENDOR_AVDTP_STATS_CMD_OPCODE >> 8) & 0xFF);
    rsi_bt_vendor_avdtp_stats.avdtp_stats_enable = avdtp_stats_enable;
  
    return rsi_bt_driver_send_cmd(RSI_BT_VENDOR_SPECIFIC, &rsi_bt_vendor_avdtp_stats, NULL);
}

#endif
