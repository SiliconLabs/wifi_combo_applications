/**
 * @file     rsi_bt_hid_apis.c
 * @version  0.1
 * @date     15 Apr 2019
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
#include "rsi_bt_sdp.h"
#include "rsi_hid.h"

/* NOTE***********************************************
 * For all the API's device address shall be provided as ASCII string.
 * Ex: "00:12:34:AB:CD:EF"
 ****************************************************/

bt_sdp_proto_desc_list_elem_t sdp_hid_control_proto_desc_list[2] = 
{
  {
    BT_PROTOCOL_UUID_L2CAP,
    1,  {BT_HID_CONTROL_PSM}
  },
  {
    BT_PROTOCOL_UUID_HID,
    0,
    {0}
  }
};

bt_sdp_proto_desc_list_elem_t sdp_hid_interrupt_proto_desc_list[2] = 
{
  {
    BT_PROTOCOL_UUID_L2CAP,
    1,  {BT_HID_INTERRUPT_PSM}
  },
  {
    BT_PROTOCOL_UUID_HID,
    0,
    {0}
  }
};




/*==============================================*/
/**
 * @fn         rsi_bt_hid_init
 * @brief      sets profile mode
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to set the profile mode.
 */
int32_t rsi_bt_hid_init(void)
{ 
  rsi_bt_req_profile_mode_t bt_req_hid_init = {(1<<6)};
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_SET_PROFILE_MODE, &bt_req_hid_init, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_bt_hid_connect
 * @brief      requests HID connection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used to initiate HID connection.
 * */
int32_t rsi_bt_hid_connect(uint8_t *remote_dev_addr)
{
  rsi_bt_req_connect_t bt_req_hid_connect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_hid_connect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_HID_CONNECT, &bt_req_hid_connect, NULL);
}

int32_t rsi_bt_hid_profile_data(uint8_t *remote_dev_addr, uint8_t *data, uint16_t len, uint8_t cid)
{
  rsi_bt_hid_profile_data_t bt_req_hid_profile_data = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_hid_profile_data.dev_addr, (int8_t *)remote_dev_addr);
  *(uint16_t *)&bt_req_hid_profile_data.len[0] = len;
  bt_req_hid_profile_data.channel_id = cid;
  memcpy(bt_req_hid_profile_data.buf, data, len);
  return rsi_bt_driver_send_cmd(RSI_BT_REQ_HID_PROFILE_DATA, &bt_req_hid_profile_data, NULL);
}

int32_t rsi_bt_hid_send_handshake(uint8_t *remote_dev_addr,uint8_t status)
{
  uint8_t buf;
  buf = (HID_MSG_HANDSHAKE << 4) | (status) ;
  return rsi_bt_hid_profile_data(remote_dev_addr, &buf, HID_MSG_HANDSHAKE_LEN, HID_CHANNEL_CONTROL);
}

int32_t rsi_bt_hid_send_control(uint8_t *remote_dev_addr,uint8_t control)
{
  uint8_t buf;
  buf = (HID_MSG_CONTROL << 4) | (control) ;
  return rsi_bt_hid_profile_data(remote_dev_addr, &buf, HID_MSG_CONTROL_LEN, HID_CHANNEL_CONTROL);
}

int32_t rsi_bt_hid_get_report(uint8_t *remote_dev_addr, uint8_t report_id,uint8_t report_type)
{
  uint8_t buf[4] = {0};
  uint16_t len=0;
  buf[0] = (HID_MSG_GET_REPORT << 4) | report_type;
  buf[1] = report_id;
  len = 2;
  return rsi_bt_hid_profile_data(remote_dev_addr, buf, len, HID_CHANNEL_CONTROL);
}

int32_t rsi_bt_hid_set_report(uint8_t *remote_dev_addr, uint8_t report_id,uint8_t report_type, uint8_t *data, uint16_t data_len)
{

  uint8_t buf[48] = {0};
  buf[0] = (HID_MSG_SET_REPORT << 4) | report_type;
  memcpy(&buf[2], data, data_len);
  return rsi_bt_hid_profile_data(remote_dev_addr, buf, (data_len + 1), HID_CHANNEL_CONTROL);
}

int32_t rsi_bt_hid_get_protocol(uint8_t *remote_dev_addr)
{
  uint8_t buf;
  buf = HID_MSG_GET_PROTO << 4;
  return rsi_bt_hid_profile_data(remote_dev_addr, &buf, HID_MSG_GET_REPORT_LEN, HID_CHANNEL_CONTROL);
}

int32_t rsi_bt_hid_set_protocol(uint8_t *remote_dev_addr, uint8_t protocol)
{
  uint8_t buf;
  buf = (HID_MSG_SET_PROTO << 4) | protocol;
  return rsi_bt_hid_profile_data(remote_dev_addr, &buf, HID_MSG_SET_PROTO_LEN, HID_CHANNEL_CONTROL);
}

int32_t rsi_bt_hid_send_interrupt_data(uint8_t *remote_dev_addr, uint8_t *data, uint16_t data_len)
{

  uint8_t buf[48] = {0};
  buf[0] = (HID_MSG_DATA << 4) | HID_REPORT_TYPE_INPUT;
  memcpy(&buf[1], data, data_len);
  return rsi_bt_hid_profile_data(remote_dev_addr, buf, (data_len + 1), HID_CHANNEL_INTERRUPT);
}

/*==============================================*/
/**
 * @fn         rsi_bt_hid_disconnect
 * @brief      requests HID disconnection.
 * @param[in]  remote_dev_address, remote device address
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This API is used for HID disconnection.
 * */
int32_t rsi_bt_hid_disconnect(uint8_t *remote_dev_addr)
{
  rsi_bt_req_disconnect_t bt_req_hid_disconnect = {{0}};
  rsi_ascii_dev_address_to_6bytes_rev(bt_req_hid_disconnect.dev_addr, (int8_t *)remote_dev_addr);

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_HID_DISCONNECT, &bt_req_hid_disconnect, NULL);
}


int32_t rsi_bt_hid_service_initialize(char *service_name, char *service_description, 
                                      char *service_provider, uint8_t *desc_buf, uint16_t desc_buf_len)
{

  uint32_t status  = 0;
  uint16_t profile_ver = 0x100;
  
  rsi_sdp_att_record_t record_data = {0};
  
  bt_sdp_lang_attr_id_elem_t bt_sdp_lang_attr_id_elem = 
  {
    BT_SDP_LANG_ATTR_NATURAL_LANG_ID,
    BT_SDP_LANG_ATTR_ENCODING_UTF8,
    BT_SDP_LANG_ATTR_BASE_ATTR_ID
  };

  status = rsi_bt_add_sdp_service_record_handle(&record_data, 0x1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_service_classid(&record_data, BT_SERVICE_UUID_HID);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_protocol_list(&record_data, &sdp_hid_control_proto_desc_list[0], 2, BT_ATTR_ID_PROTOCOL_DESC_LIST);  
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_BROWSE_GROUP_LIST, 0, BT_SERVICE_UUID_PUBLIC_BROWSE_GROUP, 0, 2);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_language_base_attributeid_list(&record_data, &bt_sdp_lang_attr_id_elem, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  status = rsi_bt_add_sdp_profile_descriptor_list(&record_data, BT_SERVICE_UUID_HID, profile_ver);  
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_protocol_list(&record_data, &sdp_hid_interrupt_proto_desc_list[0], 2, BT_ATTR_ID_ADDITIONAL_PROTOCOL_DESC_LIST);  
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_service_attribute(&record_data, service_name, strlen(service_name), BT_ATTR_ID_ENGLISH_SERVICE_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_service_attribute(&record_data, service_description, strlen(service_description), BT_ATTR_ID_ENGLISH_SERVICE_DESCRIPTION);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_service_attribute(&record_data, service_provider, strlen(service_provider), BT_ATTR_ID_ENGLISH_PROVIDER_NAME);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_VERSION_NUM, 0, RSI_HID_DEVICE_VERSION_NUM, 0, 2);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_PARSER_VERSION, 0, RSI_PARSER_VERSION, 0, 2);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_DEVICE_SUBCLASS, RSI_HID_DEV_SUBCLASS, 0 ,0, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_COUNTRY_CODE, RSI_HID_COUNTRY_CODE, 0 ,0, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_VIRTUAL_CABLE, RSI_VIRTUAL_CABLE, 0 ,1, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_RECONNECT_INITIATE, RSI_RECONNECT_INITIATE, 0 ,1, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  status = rsi_bt_add_sdp_hid_descriptor_list(&record_data, desc_buf, desc_buf_len);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_hid_language_attribute(&record_data, RSI_HID_LANG_ID, RSI_HID_LANG_BASE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_SDP_DISABLE, RSI_SDP_DISBALE, 0 ,1, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_BATTERY_POWER, RSI_BATTERY_POWER, 0 ,1, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_REMOTE_WAKE, RSI_REMOTE_WAKE, 0 ,1, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_PROFILE_VERSION, 0, RSI_PROFILE_VER ,0, 2);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_SUPERVISION_TIMEOUT, 0, RSI_SUPERVISION_TIMEOUT, 0, 2);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_NORMALLY_CONNECTABLE, RSI_NORMALLY_CONNECTABLE, 0 ,1, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_BOOT_DEVICE, RSI_BOOT_DEVICE, 0 ,1, 1);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_SSR_HOST_MAX_LATENCY, 0, RSI_SSR_HOST_MAX_LATENCY, 0, 2);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status = rsi_bt_add_sdp_attribute(&record_data, BT_ATTR_ID_HID_SSR_HOST_MIN_LATENCY, 0, RSI_SSR_HOST_MIN_LATENCY, 0, 2);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  return rsi_bt_driver_send_cmd(RSI_BT_REQ_HID_SDP_ATT_INIT, &record_data, NULL);
}
#endif
