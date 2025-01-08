/**
 * @file     rsi_bt_avrcp_apis.c
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
 * @fn         rsi_bt_avrcp_init
 * @brief      sets profile mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the profile mode.
 */
int32_t rsi_bt_avrcp_init(void)
{ 
  rsi_bt_req_profile_mode_t bt_req_avrcp_init = {RSI_AVRCP_PROFILE_BIT};
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_PROFILE_MODE, &bt_req_avrcp_init, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_conn
 * @brief      requests avrcp connection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to initiate avrcp connection.
 * */
int32_t rsi_bt_avrcp_conn(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_conn_t bt_req_avrcp_connect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_connect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_CONNECT, &bt_req_avrcp_connect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_disconn
 * @brief      requests avrcp disconnection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to initiate avrcp disconnection.
 * */
int32_t rsi_bt_avrcp_disconn(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_disconnect_t bt_req_avrcp_disconnect = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_disconnect.dev_addr, (int8_t *)remote_dev_addr);
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_DISCONNECT, &bt_req_avrcp_disconnect, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_play
 * @brief      requests avrcp to play the audio song.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_play(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_play_t bt_req_avrcp_play = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_play.dev_addr, (int8_t *)remote_dev_addr);
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_PLAY, &bt_req_avrcp_play, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_pause
 * @brief      requests avrcp to pause the song .
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_pause(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_pause_t bt_req_avrcp_pause = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_pause.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_PAUSE, &bt_req_avrcp_pause, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_stop
 * @brief      requests avrcp to stop the song .
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_stop(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_stop_t bt_req_avrcp_stop = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_stop.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_STOP, &bt_req_avrcp_stop, NULL);
}


/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_next
 * @brief      requests avrcp to play the next song.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_next(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_next_t bt_req_avrcp_next = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_next.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_NEXT, &bt_req_avrcp_next, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_previous
 * @brief      requests avrcp tp play the previous song.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_previous(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_previous_t bt_req_avrcp_previous = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_previous.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_PREVIOUS, &bt_req_avrcp_previous, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_vol_up
 * @brief      requests avrcp to vol up.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_vol_up(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_vol_up_t bt_req_avrcp_vol_up = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_vol_up.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_VOL_UP, &bt_req_avrcp_vol_up, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_vol_down
 * @brief      requests avrcp to vol down.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_vol_down(uint8_t *remote_dev_addr)
{
  rsi_bt_req_avrcp_vol_down_t bt_req_avrcp_vol_down = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_vol_down.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_VOL_DOWN, &bt_req_avrcp_vol_down, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_capabilities
 * @brief      This fucntion is used to get the media player support events and campany IDS.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_capabilities(uint8_t *remote_dev_addr, uint8_t capability_type, rsi_bt_rsp_avrcp_get_capabilities_t *cap_list)
{
  rsi_bt_req_avrcp_get_capabilities_t bt_req_avrcp_cap = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_cap.dev_addr, (int8_t *)remote_dev_addr);
  bt_req_avrcp_cap.type = capability_type;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_CAPABILITES, &bt_req_avrcp_cap, cap_list);
}


/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_att_list
 * @brief      This fucntion is used to get the media player support attribute list from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_att_list (uint8_t *remote_dev_addr, rsi_bt_rsp_avrcp_get_atts_list_t *att_list)
{
  rsi_bt_req_avrcp_get_att_list_t bt_req_avrcp_att = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_avrcp_att.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_ATTS_LIST, &bt_req_avrcp_att, att_list);
}


/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_att_vals_list
 * @brief      This fucntion is used to get the media player support attribute values list from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_att_vals_list (uint8_t *remote_dev_addr, uint8_t att_id, rsi_bt_rsp_avrcp_get_att_vals_list_t *att_vals_list)
{
  rsi_bt_req_avrcp_get_att_vals_list_t avrcp_att_vals = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(avrcp_att_vals.dev_addr, (int8_t *)remote_dev_addr);
  avrcp_att_vals.att_id = att_id;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_ATT_VALS_LIST, &avrcp_att_vals, att_vals_list);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_cur_att_val
 * @brief      This fucntion is used to get the media player attribute value from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_cur_att_val (uint8_t *remote_dev_addr, uint8_t *att_list, uint8_t nbr_atts, rsi_bt_rsp_avrcp_get_cur_att_val_t *att_vals_list)
{
  rsi_bt_req_avrcp_get_cur_att_val_t avrcp_atts = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(avrcp_atts.dev_addr, (int8_t *)remote_dev_addr);
  avrcp_atts.nbr_atts = nbr_atts;
  for (ix = 0; ix < nbr_atts; ix++) {
    avrcp_atts.att_list[ix] = att_list[ix];
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_CUR_ATT_VAL, &avrcp_atts, att_vals_list);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_set_cur_att_val
 * @brief      This fucntion is used to set the media player attribute value at remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_set_cur_att_val (uint8_t *remote_dev_addr, att_val_t *val_list,uint8_t nbr_atts)
{
  rsi_bt_req_avrcp_set_cur_att_val_t att_val_list = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(att_val_list.dev_addr, (int8_t *)remote_dev_addr);
  att_val_list.nbr_atts = nbr_atts;
  for (ix = 0; ix < nbr_atts; ix++) {
    att_val_list.att_val_list[ix].att_id = val_list[ix].att_id;
    att_val_list.att_val_list[ix].att_id = val_list[ix].att_val;
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_SET_CUR_ATT_VAL, &att_val_list, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_element_att
 * @brief      This fucntion is used to get the media attributes from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_element_att (uint8_t *remote_dev_addr, uint8_t  *att_ids, uint8_t nbr_atts, rsi_bt_rsp_avrcp_get_ele_att_t *att_vals)
{
  rsi_bt_req_avrcp_get_ele_att_t att_list = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(att_list.dev_addr, (int8_t *)remote_dev_addr);
  att_list.nbr_atts = nbr_atts;
  for (ix = 0; ix < nbr_atts; ix++) {
    att_list.att_list[ix] = att_ids[ix];
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_ELEMENT_ATT, &att_list, att_vals);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_play_status
 * @brief      This fucntion is used to get the media player status from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_play_status (uint8_t *remote_dev_addr, rsi_bt_rsp_avrcp_get_player_status_t *play_status)
{
  rsi_bt_req_avrcp_get_player_status_t play_status_req = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(play_status_req.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_PLAY_STATUS, &play_status_req, play_status);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_reg_notification
 * @brief      This fucntion is used to register the media player notification events at remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_reg_notification (uint8_t *remote_dev_addr, uint8_t  event_id)
{
  rsi_bt_req_avrcp_reg_notification_t reg_notify_req = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(reg_notify_req.dev_addr, (int8_t *)remote_dev_addr);
  reg_notify_req.event_id = event_id;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_REG_NOTIFICATION, &reg_notify_req, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_remote_version
 * @brief      This fucntion is used to get the AVRCP profile version from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_remote_version (uint8_t *remote_dev_addr, rsi_bt_rsp_avrcp_remote_version_t *version)
{
  rsi_bt_req_avrcp_remote_version_t profile_version = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(profile_version.dev_addr, (int8_t *)remote_dev_addr);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_REMOTE_VERSION, &profile_version, version);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_att_text
 * @brief      This fucntion is used to get the AVRCP profile player attribute text from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_att_text (uint8_t *remote_dev_addr, uint8_t nbr_atts, uint8_t *p_atts, player_att_text_t *p_att_text_resp)
{
  rsi_bt_req_avrcp_get_cur_att_val_t att_text = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(att_text.dev_addr, (int8_t *)remote_dev_addr);
  att_text.nbr_atts = nbr_atts;
  for (ix = 0; ix < nbr_atts; ix++) {
    att_text.att_list[ix] = p_atts[ix];
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_ATT_TEXT, &att_text, p_att_text_resp);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_get_att_val_text
 * @brief      This fucntion is used to get the AVRCP profile player attribute values text from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_get_att_val_text (uint8_t *remote_dev_addr, uint8_t att_id, uint8_t nbr_vals, uint8_t *p_vals, player_att_text_t *p_att_text_resp)
{
  rsi_bt_req_avrcp_get_att_val_text_t att_val_text = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(att_val_text.dev_addr, (int8_t *)remote_dev_addr);
  att_val_text.att_id   = att_id;
  att_val_text.nbr_vals = nbr_vals;
  for (ix = 0; ix < nbr_vals; ix++) {
    att_val_text.vals[ix] = p_vals[ix];
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_ATT_VALS_TEXT, &att_val_text, p_att_text_resp);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_batt_status
 * @brief      This fucntion is used to send the device battery status to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_batt_status (uint8_t *remote_dev_addr, uint8_t batt_level)
{
  rsi_bt_req_avrcp_batt_status_t batt_status = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(batt_status.dev_addr, (int8_t *)remote_dev_addr);
  batt_status.batt_status   = batt_level;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_BATTERY_STATUS, &batt_status, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_supp_char_sets
 * @brief      This fucntion is used to send the device battery status to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_supp_char_sets (uint8_t *remote_dev_addr, uint8_t  nbr_sets, uint16_t *p_sets)
{
  rsi_bt_req_avrcp_char_sets_t char_sets = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(char_sets.dev_addr, (int8_t *)remote_dev_addr);
  char_sets.char_sets.nbr_sets   = nbr_sets;
  for (ix = 0; ix < nbr_sets; ix++) {
    char_sets.char_sets.supp_vals[ix] = p_sets[ix];
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_CHAR_SETS, &char_sets, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_set_abs_vol_resp
 * @brief      This fucntion is used to send absolute volume resonse to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_set_abs_vol (uint8_t *remote_dev_addr, uint8_t abs_vol, uint8_t *p_resp_abs_vol)
{
  rsi_bt_avrcp_set_abs_vol_t abs_vol_req = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(abs_vol_req.dev_addr, (int8_t *)remote_dev_addr);
  abs_vol_req.abs_vol = abs_vol;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_SET_ABS_VOL, &abs_vol_req, p_resp_abs_vol);
}


/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_cap_resp
 * @brief      This fucntion is used to send the device capcabilites to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_cap_resp (uint8_t *remote_dev_addr, uint8_t cap_type, uint8_t nbr_caps, uint32_t  *p_caps)
{
  rsi_bt_avrcp_cap_resp_t cap_resp = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(cap_resp.dev_addr, (int8_t *)remote_dev_addr);
  cap_resp.cap_type   = cap_type;
  cap_resp.nbr_caps   = nbr_caps;
  for (ix = 0; ix < nbr_caps; ix++) {
    cap_resp.caps[ix] = p_caps[ix];
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_CAPABILITES_RESP, &cap_resp, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_att_list_resp
 * @brief      This fucntion is used to send the support attributes to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_att_list_resp (uint8_t *remote_dev_addr, uint8_t nbr_atts, uint8_t *p_atts)
{
  rsi_bt_avrcp_att_list_resp_t att_resp = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(att_resp.dev_addr, (int8_t *)remote_dev_addr);
  att_resp.nbr_atts   = nbr_atts;
  for (ix = 0; ix < nbr_atts; ix++) {
    att_resp.atts[ix] = p_atts[ix];
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_ATTS_LIST_RESP, &att_resp, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_att_val_list_resp
 * @brief      This fucntion is used to send the support attributes to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_att_val_list_resp (uint8_t *remote_dev_addr, uint8_t nbr_vals, uint8_t *p_vals)
{
  rsi_bt_avrcp_att_vals_list_resp_t att_vals_resp = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(att_vals_resp.dev_addr, (int8_t *)remote_dev_addr);
  att_vals_resp.nbr_vals   = nbr_vals;
  for (ix = 0; ix < nbr_vals; ix++) {
    att_vals_resp.vals[ix] = p_vals[ix];
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_ATT_VALS_LIST_RESP, &att_vals_resp, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_cur_att_val_resp
 * @brief      This fucntion is used to send the support attributes to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_cur_att_val_resp (uint8_t *remote_dev_addr, uint8_t nbr_atts, att_val_t *p_att_vals)
{
  rsi_bt_avrcp_cur_att_vals_resp_t att_vals_resp = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(att_vals_resp.dev_addr, (int8_t *)remote_dev_addr);
  att_vals_resp.att_list.nbr_atts = nbr_atts;
  for (ix = 0; ix < nbr_atts; ix++) {
    att_vals_resp.att_list.att_vals[ix].att_id = p_att_vals[ix].att_id;
    att_vals_resp.att_list.att_vals[ix].att_val = p_att_vals[ix].att_val;
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_CUR_ATT_VAL_RESP, &att_vals_resp, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_set_cur_att_val_resp
 * @brief      This fucntion is used to send the support attributes to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_set_cur_att_val_resp (uint8_t *remote_dev_addr, uint8_t status)
{
  rsi_bt_avrcp_set_att_vals_resp_t set_att_vals_resp = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(set_att_vals_resp.dev_addr, (int8_t *)remote_dev_addr);
  set_att_vals_resp.status = status;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_SET_CUR_ATT_VAL_RESP, &set_att_vals_resp, NULL);
}

#define BT_AVRCP_UTF_8_CHAR_SET 0x006A
/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_ele_att_resp
 * @brief      This fucntion is used to send the song attributes to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_ele_att_resp (uint8_t *remote_dev_addr, uint8_t num_attrs, attr_list_t *p_attr_list)
{
  uint8_t ix; 
  rsi_bt_avrcp_elem_attr_resp_t elem_attr = {{0}};

  rsi_ascii_dev_address_to_6bytes_rev(elem_attr.dev_addr, (int8_t *)remote_dev_addr);
  elem_attr.elem_attr_list.num_attrs = num_attrs;
  for (ix = 0; ix < num_attrs; ix++) {
    elem_attr.elem_attr_list.attr_list[ix].id = p_attr_list[ix].id;
    elem_attr.elem_attr_list.attr_list[ix].char_set_id = BT_AVRCP_UTF_8_CHAR_SET;
    elem_attr.elem_attr_list.attr_list[ix].attr_len = p_attr_list[ix].attr_len;
    memcpy (elem_attr.elem_attr_list.attr_list[ix].attr_val, p_attr_list[ix].attr_val, p_attr_list[ix].attr_len);
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_ELEMENT_ATT_RESP, &elem_attr, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_play_status_resp
 * @brief      This fucntion is used to send the player status to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_play_status_resp (uint8_t *remote_dev_addr, uint8_t play_status, uint32_t song_len, uint32_t song_pos)
{
  rsi_bt_avrcp_play_status_resp_t  player_status = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(player_status.dev_addr, (int8_t *)remote_dev_addr);
  player_status.play_status = play_status;
  player_status.song_len = song_len;
  player_status.song_pos = song_pos;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_GET_PLAY_STATUS_RESP, &player_status, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_reg_notify_resp
 * @brief      This fucntion is used to send the player status to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_reg_notify_resp (uint8_t *remote_dev_addr, uint8_t event_id, uint8_t event_data_len, uint8_t *event_data)
{
  rsi_bt_avrcp_reg_notify_interim_resp_t  reg_event = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(reg_event.dev_addr, (int8_t *)remote_dev_addr);
  reg_event.event_id = event_id;

  if (event_id == 0x02 /* EVENT_TRACK_CHANGED */)
    memcpy(reg_event.reg_notify_val.curr_track_idx, event_data, event_data_len);
  else if (event_id == 0x0d /* EVENT_VOL_CHANGED */)
    memcpy(&reg_event.reg_notify_val.abs_vol, event_data, event_data_len);
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_REG_NOTIFICATION_RESP, &reg_event, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_att_text_resp
 * @brief      This fucntion is used to send supporting attribute text resonse to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_att_text_resp (uint8_t *remote_dev_addr, uint8_t nbr_atts, att_text_t *p_att_text)
{
  rsi_bt_avrcp_att_text_resp_t  att_text = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(att_text.dev_addr, (int8_t *)remote_dev_addr);
  att_text.atts_text.nbr_atts = nbr_atts;
  for (ix = 0; ix < nbr_atts; ix++)
  {
    att_text.atts_text.list[ix].id = p_att_text[ix].id;
    memcpy (att_text.atts_text.list[ix].att_text, p_att_text[ix].att_text, strlen ((char *)p_att_text[ix].att_text));
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_ATT_TEXT_RESP, &att_text, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_att_vals_text_resp
 * @brief      This fucntion is used to send supporting attribute values text resonse to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_att_vals_text_resp (uint8_t *remote_dev_addr, uint8_t nbr_vals, att_text_t *p_vals_text)
{
  rsi_bt_avrcp_att_text_resp_t  vals_text = {{0}};
  uint8_t  ix;
  
  
  rsi_ascii_dev_address_to_6bytes_rev(vals_text.dev_addr, (int8_t *)remote_dev_addr);
  vals_text.atts_text.nbr_atts = nbr_vals;
  for (ix = 0; ix < nbr_vals; ix++)
  {
    vals_text.atts_text.list[ix].id = p_vals_text[ix].id;
    memcpy (vals_text.atts_text.list[ix].att_text, p_vals_text[ix].att_text, strlen ((char *)p_vals_text[ix].att_text));
  }
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_ATT_VALS_TEXT_RESP, &vals_text, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_batt_status_resp
 * @brief      This fucntion is used to send battery status resonse to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_batt_status_resp (uint8_t *remote_dev_addr, uint8_t status)
{
  rsi_bt_avrcp_reg_notify_resp_t  batt_status = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(batt_status.dev_addr, (int8_t *)remote_dev_addr);
  batt_status.status = status;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_BATTERY_STATUS_RESP, &batt_status, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_char_set_status_resp
 * @brief      This fucntion is used to send character set resonse to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_char_set_status_resp (uint8_t *remote_dev_addr, uint8_t status)
{
  rsi_bt_avrcp_reg_notify_resp_t  char_set = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(char_set.dev_addr, (int8_t *)remote_dev_addr);
  char_set.status = status;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_CHAR_SETS_RESP, &char_set, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_notify
 * @brief      This fucntion is used to send player notification to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_notify (uint8_t *remote_dev_addr, uint8_t event_id, notify_val_t *p_notify_val)
{
  rsi_bt_avrcp_notify_t  notify = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(notify.dev_addr, (int8_t *)remote_dev_addr);
  notify.event_id = event_id;
  memcpy (&notify.notify_val, p_notify_val, sizeof (notify_val_t));
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_NOTIFICATION, &notify, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_cmd_reject
 * @brief      This fucntion is used to reject the received request from remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_cmd_reject (uint8_t *remote_dev_addr, uint8_t pdu_id, uint8_t status)
{
  rsi_bt_avrcp_cmd_reject_t  cmd_reject = {{0}};
  
  
  rsi_ascii_dev_address_to_6bytes_rev(cmd_reject.dev_addr, (int8_t *)remote_dev_addr);
  cmd_reject.pdu_id = pdu_id;
  cmd_reject.status = status;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_CMD_REJECT, &cmd_reject, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_avrcp_set_abs_vol_resp
 * @brief      This fucntion is used to send absolute volume resonse to remote device.
 * @section description
 * this api is used after AVRCP profile connection.
 * */
int32_t rsi_bt_avrcp_set_abs_vol_resp (uint8_t *remote_dev_addr, uint8_t abs_vol)
{
  rsi_bt_avrcp_set_abs_vol_resp_t abs_vol_resp = {{0}};
  
  rsi_ascii_dev_address_to_6bytes_rev(abs_vol_resp.dev_addr, (int8_t *)remote_dev_addr);
  abs_vol_resp.abs_vol = abs_vol;
  
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_AVRCP_SET_ABS_VOL_RESP, &abs_vol_resp, NULL);
}

#endif
