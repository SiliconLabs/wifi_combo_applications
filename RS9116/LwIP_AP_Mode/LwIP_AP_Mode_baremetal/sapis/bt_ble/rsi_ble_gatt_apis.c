/**
 * @file     rsi_ble_gatt_apis.c
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

//! GATT Client Functions
/**********************************************************************
 * In all GAT client APIs, all actions (get/set/write) are referred to
 * the GATT server running on the remote / connected device.
 *********************************************************************/
/*==============================================*/
/**
 * @fn         rsi_ble_get_profiles_async
 * @brief      Get the supported profiles/services of the connected / remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[out] p_profile_list, profiles/services information will be filled in this
 *                             after retrieving from the remote device
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used get the remote device supported profiles list.
 */

int32_t rsi_ble_get_profiles_async(uint8_t *dev_addr,
                             uint16_t start_handle,
                             uint16_t end_handle,
                             rsi_ble_resp_profiles_list_t  *p_prof_list)
{
  rsi_ble_req_profiles_list_t  req_profiles = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_profiles.dev_addr, (int8_t *)dev_addr);
  req_profiles.start_handle = start_handle;
  req_profiles.end_handle   = end_handle;

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_PROFILES_ASYNC, &req_profiles, p_prof_list);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_profile_async
 * @brief      Get the supported profile/service of the connected / remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  profile_uuid, Services/profiles are searched using profile_uuid
 * @param[out] p_profile, profile/service information will be filled in this structure
 *                        after retrieving from the remote device
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used get the remote device supported profile
 */
int32_t rsi_ble_get_profile_async(uint8_t *dev_addr,
                             uuid_t   profile_uuid,
                             profile_descriptors_t  *p_profile)
{
  rsi_ble_req_profile_t  req_profile = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_profile.dev_addr, (int8_t *)dev_addr);
  memcpy (&req_profile.profile_uuid, &profile_uuid, sizeof (uuid_t));

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_PROFILE_ASYNC, &req_profile, p_profile);
}
/*==============================================*/
/**
 * @fn         rsi_ble_get_char_services_async 
 * @brief      Get service characteristics of the connected / remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[out] p_char_services_list, service characteristics details are filled in this structure
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the service characteristics of the remote GATT server
 */
int32_t rsi_ble_get_char_services_async(uint8_t  *dev_addr,
                                   uint16_t  start_handle,
                                   uint16_t  end_handle,
                                   rsi_ble_resp_char_services_t  *p_char_serv_list)
{
  rsi_ble_req_char_services_t  req_char_services = {{0}};

  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_char_services.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (req_char_services.start_handle, start_handle);
  rsi_uint16_to_2bytes (req_char_services.end_handle, end_handle);

  return rsi_bt_driver_send_cmd(RSI_BLE_GET_CHARSERVICES_ASYNC, &req_char_services , p_char_serv_list);
}


/*==============================================*/
/**
 * @fn         rsi_ble_get_inc_services_async  
 * @brief      Get the supported include services of the connected / remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[out] p_inc_serv_list, include service characteristics details are filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the include service characteristics of the remote GATT server
 */
int32_t rsi_ble_get_inc_services_async(uint8_t  *dev_addr,
                                  uint16_t  start_handle,
                                  uint16_t  end_handle,
                                  rsi_ble_resp_inc_services_t  *p_inc_serv_list)
{
  rsi_ble_req_inc_services_t  req_inc_services = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_inc_services.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (req_inc_services.start_handle, start_handle);
  rsi_uint16_to_2bytes (req_inc_services.end_handle, end_handle);

  return rsi_bt_driver_send_cmd(RSI_BLE_GET_INCLUDESERVICES_ASYNC, &req_inc_services , p_inc_serv_list);
}


/*==============================================*/
/**
 * @fn         rsi_ble_get_char_value_by_uuid_async
 * @brief      Get characteristic value by UUID (char_uuid)
 * @param[in]  dev_addr, remote device address
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[in]  char_uuid, UUID of the characteristic
 * @param[out] p_char_value, characteristic value is filled in this structure.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to get the characteristic value using UUID
 */
int32_t rsi_ble_get_char_value_by_uuid_async (uint8_t  *dev_addr,
                                        uint16_t  start_handle,
                                        uint16_t  end_handle,
                                        uuid_t    char_uuid,
                                        rsi_ble_resp_att_value_t *p_char_val)
{
  rsi_ble_req_char_val_by_uuid_t  req_char_val = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_char_val.dev_addr, (int8_t *)dev_addr);

  rsi_uint16_to_2bytes (req_char_val.start_handle, start_handle);
  rsi_uint16_to_2bytes (req_char_val.end_handle, end_handle);
  memcpy (&req_char_val.char_uuid, &char_uuid, sizeof (uuid_t));

  return rsi_bt_driver_send_cmd(RSI_BLE_READCHARVALUEBYUUID_ASYNC, &req_char_val  , p_char_val);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_att_descriptors_async 
 * @brief      Get the attribute descriptors list from remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[out] p_att_desc, attribute descriptors are filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the attribute descriptors details of the remote device
 */
int32_t rsi_ble_get_att_descriptors_async(uint8_t *dev_addr,
                                     uint16_t start_handle,
                                     uint16_t end_handle,
                                     rsi_ble_resp_att_descs_t  *p_att_desc)
{
  rsi_ble_req_att_descs_t  req_att_desc = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_att_desc.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes(req_att_desc.start_handle, start_handle);
  rsi_uint16_to_2bytes(req_att_desc.end_handle, end_handle);

  return rsi_bt_driver_send_cmd(RSI_BLE_GET_ATTRIBUTE_ASYNC, &req_att_desc, p_att_desc);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_att_value_async 
 * @brief      Get the attribute by handle
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, handle of attribute
 * @param[out] p_att_val, attribute value is filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the attribute value using handle
 */
int32_t rsi_ble_get_att_value_async(uint8_t *dev_addr,
                               uint16_t handle,
                               rsi_ble_resp_att_value_t  *p_att_val)
{
  rsi_ble_req_att_value_t  req_att_val = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_att_val.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (req_att_val.handle, handle);

  return rsi_bt_driver_send_cmd(RSI_BLE_GET_DESCRIPTORVALUE_ASYNC, &req_att_val, p_att_val);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_multiple_att_values_async 
 * @brief      Get multiple attribute values by using multiple handles
 * @param[in]  dev_addr, remote device address 
 * @param[in]  num_of_handles, number of handles in the list
 * @param[in]  handles, list of attribute handles
 * @param[out] p_att_vals, Attribute values are filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the multiple attribute values by using multiple handles
 */

int32_t rsi_ble_get_multiple_att_values_async(uint8_t *dev_addr,
                                         uint8_t num_of_handlers,
                                         uint16_t *handles,
                                         rsi_ble_resp_att_value_t *p_att_vals)
{
  rsi_ble_req_multi_att_values_t  req_att_vals = {{0}};
  uint8_t ix;

  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_att_vals.dev_addr, (int8_t *)dev_addr);
  req_att_vals.num_of_handles = RSI_MIN (num_of_handlers, RSI_BLE_MAX_REQ_LIST);
  for (ix = 0; ix < req_att_vals.num_of_handles; ix++) {
    req_att_vals.handles[ix] = handles[ix];
  }

  return rsi_bt_driver_send_cmd(RSI_BLE_GET_MULTIPLEVALUES_ASYNC , &req_att_vals, p_att_vals);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_long_att_value_async 
 * @brief      Get the long attribute value by using handle and offset
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, attribute handle
 * @param[in]  offset, offset with in the attribute value
 * @param[out] p_att_vals, attribute value is filled in this structure
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the attribute value by using handle and offset
 */

int32_t rsi_ble_get_long_att_value_async(uint8_t *dev_addr,
                                    uint16_t handle,
                                    uint16_t offset,
                                    rsi_ble_resp_att_value_t *p_att_vals)
{
  rsi_ble_req_long_att_value_t  req_long_att_val = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_long_att_val.dev_addr, (int8_t *)dev_addr);
  req_long_att_val.handle = handle;
  req_long_att_val.offset = offset;

  return rsi_bt_driver_send_cmd(RSI_BLE_GET_LONGDESCVALUES_ASYNC , &req_long_att_val, p_att_vals);
}
/*==============================================*/
/**
 * @fn         rsi_ble_set_att_value_async 
 * @brief      Set attribute value
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, attribute handle
 * @param[in]  data_len, attribute value length
 * @param[in]  p_data, attribute value .
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to set the attribute value (data_len and p_data) represented by handle
 */
int32_t rsi_ble_set_att_value_async (uint8_t *dev_addr,
                                   uint16_t  handle,
                                   uint8_t data_len,
                                   uint8_t  *p_data)
{
  rsi_ble_set_att_value_t  set_att_val = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)set_att_val.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (set_att_val.handle, handle);
  set_att_val.length = RSI_MIN (sizeof (set_att_val.att_value), data_len);
  memcpy (set_att_val.att_value, p_data, set_att_val.length);


  return rsi_bt_driver_send_cmd(RSI_BLE_SET_DESCVALUE_ASYNC , &set_att_val, NULL);
}
/*==============================================*/
/**
 * @fn         rsi_ble_prepare_write_async  
 * @brief      Prepare attribute value
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, attribute handle
 * @param[in]  offset, attribute value offset
 * @param[in]  data_len, attribute value length
 * @param[in]  p_data, attribute value .
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to prepare the attribute value at remote device
 */

int32_t rsi_ble_prepare_write_async(uint8_t *dev_addr,
                                    uint16_t  handle,
                                    uint16_t offset,
                                    uint8_t data_len,
                                    uint8_t  *p_data)
{
  rsi_ble_req_prepare_write_t  req_prepare_write = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_prepare_write.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (req_prepare_write.handle, handle);
  rsi_uint16_to_2bytes (req_prepare_write.offset, offset);
  req_prepare_write.length    = RSI_MIN (sizeof (req_prepare_write.att_value), data_len);
  memcpy (req_prepare_write.att_value, p_data, req_prepare_write.length);


  return rsi_bt_driver_send_cmd(RSI_BLE_SET_PREPAREWRITE_ASYNC , &req_prepare_write , NULL);
}
/*==============================================*/
/**
 * @fn         rsi_ble_execute_write_async  
 * @brief      Execute the prepared attribute values
 * @param[in]  dev_addr, remote device address 
 * @param[in]  exe_flag, execute flag to write the values or not 
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to execute the prepared the attribute values
 */

int32_t rsi_ble_execute_write_async(uint8_t *dev_addr,
                                    uint8_t  exe_flag)
{
  rsi_ble_req_execute_write_t  req_exe_write = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_exe_write.dev_addr, (int8_t *)dev_addr);
  req_exe_write.flag = exe_flag;

  return rsi_bt_driver_send_cmd(RSI_BLE_EXECUTE_LONGDESCWRITE_ASYNC , &req_exe_write, NULL);
}


/*==============================================*/
/**
 * @fn         rsi_ble_get_profiles
 * @brief      Get the supported profiles/services of the connected / remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[out] p_profile_list, profiles/services information will be filled in this
 *                             after retrieving from the remote device
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used get the remote device supported profiles list.
 */

int32_t rsi_ble_get_profiles(uint8_t *dev_addr,
                             uint16_t start_handle,
                             uint16_t end_handle,
                             rsi_ble_resp_profiles_list_t  *p_prof_list)
{
  rsi_ble_req_profiles_list_t  req_profiles = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_profiles.dev_addr, (int8_t *)dev_addr);
  req_profiles.start_handle = start_handle;
  req_profiles.end_handle   = end_handle;

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_PROFILES, &req_profiles, p_prof_list);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_profile
 * @brief      Get the supported profile/service of the connected / remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  profile_uuid, Services/profiles are searched using profile_uuid
 * @param[out] p_profile, profile/service information will be filled in this structure
 *                        after retrieving from the remote device
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used get the remote device supported profile
 */
int32_t rsi_ble_get_profile (uint8_t *dev_addr,
                             uuid_t   profile_uuid,
                             profile_descriptors_t  *p_profile)
{
  rsi_ble_req_profile_t  req_profile = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_profile.dev_addr, (int8_t *)dev_addr);
  memcpy (&req_profile.profile_uuid, &profile_uuid, sizeof (uuid_t));

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_PROFILE, &req_profile, p_profile);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_char_services 
 * @brief      Get service characteristics of the connected / remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[out] p_char_services_list, service characteristics details are filled in this structure
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the service characteristics of the remote GATT server
 */
int32_t rsi_ble_get_char_services(uint8_t  *dev_addr,
                                   uint16_t  start_handle,
                                   uint16_t  end_handle,
                                   rsi_ble_resp_char_services_t  *p_char_serv_list)
{
  rsi_ble_req_char_services_t  req_char_services = {{0}};

  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_char_services.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (req_char_services.start_handle, start_handle);
  rsi_uint16_to_2bytes (req_char_services.end_handle, end_handle);

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_CHAR_SERVICES, &req_char_services , p_char_serv_list);
}


/*==============================================*/
/**
 * @fn         rsi_ble_get_inc_services  
 * @brief      Get the supported include services of the connected / remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[out] p_inc_serv_list, include service characteristics details are filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the include service characteristics of the remote GATT server
 */
int32_t rsi_ble_get_inc_services(uint8_t  *dev_addr,
                                  uint16_t  start_handle,
                                  uint16_t  end_handle,
                                  rsi_ble_resp_inc_services_t  *p_inc_serv_list)
{
  rsi_ble_req_inc_services_t  req_inc_services = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_inc_services.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (req_inc_services.start_handle, start_handle);
  rsi_uint16_to_2bytes (req_inc_services.end_handle, end_handle);

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_INC_SERVICES, &req_inc_services , p_inc_serv_list);
}


/*==============================================*/
/**
 * @fn         rsi_ble_get_char_value_by_uuid
 * @brief      Get characteristic value by UUID (char_uuid)
 * @param[in]  dev_addr, remote device address
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[in]  char_uuid, UUID of the characteristic
 * @param[out] p_char_value, characteristic value is filled in this structure.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to get the characteristic value using UUID
 */
int32_t rsi_ble_get_char_value_by_uuid (uint8_t  *dev_addr,
                                        uint16_t  start_handle,
                                        uint16_t  end_handle,
                                        uuid_t    char_uuid,
                                        rsi_ble_resp_att_value_t *p_char_val)
{
  rsi_ble_req_char_val_by_uuid_t  req_char_val = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_char_val.dev_addr, (int8_t *)dev_addr);

  rsi_uint16_to_2bytes (req_char_val.start_handle, start_handle);
  rsi_uint16_to_2bytes (req_char_val.end_handle, end_handle);
  memcpy (&req_char_val.char_uuid, &char_uuid, sizeof (uuid_t));

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_READ_BY_UUID, &req_char_val  , p_char_val);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_att_descriptors 
 * @brief      Get the attribute descriptors list from remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  start_handle, Start handle (index) of the remote device's service records
 * @param[in]  end_handle, End handle (index) of the remote device's service records
 * @param[out] p_att_desc, attribute descriptors are filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the attribute descriptors details of the remote device
 */
int32_t rsi_ble_get_att_descriptors(uint8_t *dev_addr,
                                     uint16_t start_handle,
                                     uint16_t end_handle,
                                     rsi_ble_resp_att_descs_t  *p_att_desc)
{
  rsi_ble_req_att_descs_t  req_att_desc = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_att_desc.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes(req_att_desc.start_handle, start_handle);
  rsi_uint16_to_2bytes(req_att_desc.end_handle, end_handle);

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_DESC, &req_att_desc, p_att_desc);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_att_value 
 * @brief      Get the attribute by handle
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, handle of attribute
 * @param[out] p_att_val, attribute value is filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the attribute value using handle
 */
int32_t rsi_ble_get_att_value(uint8_t *dev_addr,
                               uint16_t handle,
                               rsi_ble_resp_att_value_t  *p_att_val)
{
  rsi_ble_req_att_value_t  req_att_val = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_att_val.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (req_att_val.handle, handle);

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_READ_VAL, &req_att_val, p_att_val);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_multiple_att_values 
 * @brief      Get multiple attribute values by using multiple handles
 * @param[in]  dev_addr, remote device address 
 * @param[in]  num_of_handles, number of handles in the list
 * @param[in]  handles, list of attribute handles
 * @param[out] p_att_vals, Attribute values are filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the multiple attribute values by using multiple handles
 */

int32_t rsi_ble_get_multiple_att_values(uint8_t *dev_addr,
                                         uint8_t num_of_handlers,
                                         uint16_t *handles,
                                         rsi_ble_resp_att_value_t *p_att_vals)
{
  rsi_ble_req_multi_att_values_t  req_att_vals = {{0}};
  uint8_t ix;

  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_att_vals.dev_addr, (int8_t *)dev_addr);
  req_att_vals.num_of_handles = RSI_MIN (num_of_handlers, RSI_BLE_MAX_REQ_LIST);
  for (ix = 0; ix < req_att_vals.num_of_handles; ix++) {
    req_att_vals.handles[ix] = handles[ix];
  }

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_MULTIPLE_READ , &req_att_vals, p_att_vals);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_long_att_value 
 * @brief      Get the long attribute value by using handle and offset
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, attribute handle
 * @param[in]  offset, offset with in the attribute value
 * @param[out] p_att_vals, attribute value is filled in this structure
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the attribute value by using handle and offset
 */

int32_t rsi_ble_get_long_att_value(uint8_t *dev_addr,
                                    uint16_t handle,
                                    uint16_t offset,
                                    rsi_ble_resp_att_value_t *p_att_vals)
{
  rsi_ble_req_long_att_value_t  req_long_att_val = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_long_att_val.dev_addr, (int8_t *)dev_addr);
  req_long_att_val.handle = handle;
  req_long_att_val.offset = offset;

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_LONG_READ , &req_long_att_val, p_att_vals);
}

/*==============================================*/
/**
 * @fn         rsi_ble_set_att_value 
 * @brief      Set attribute value
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, attribute handle
 * @param[in]  data_len, attribute value length
 * @param[in]  p_data, attribute value .
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to set the attribute value (data_len and p_data) represented by handle
 */
int32_t rsi_ble_set_att_value (uint8_t *dev_addr,
                                   uint16_t  handle,
                                   uint8_t data_len,
                                   uint8_t  *p_data)
{
  rsi_ble_set_att_value_t  set_att_val = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)set_att_val.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (set_att_val.handle, handle);
  set_att_val.length = RSI_MIN (sizeof (set_att_val.att_value), data_len);
  memcpy (set_att_val.att_value, p_data, set_att_val.length);


  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_WRITE , &set_att_val, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_set_att_cmd  
 * @brief      Set attribute value with out waiting for any ack from remote device
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, attribute handle
 * @param[in]  data_len, attribute value length
 * @param[in]  p_data, attribute value .
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to set the attribute value (data_len and p_data) represented by handle.
 * It does not wait for ack from remote device
 */

int32_t rsi_ble_set_att_cmd  (uint8_t *dev_addr,
                                  uint16_t  handle,
                                  uint8_t data_len,
                                  uint8_t  *p_data)
{
  rsi_ble_set_att_cmd_t  set_att_cmd = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)set_att_cmd.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (set_att_cmd.handle, handle);
  set_att_cmd.length = RSI_MIN (sizeof (set_att_cmd.att_value), data_len);
  memcpy (set_att_cmd.att_value, p_data, set_att_cmd.length);


  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_WRITE_NO_ACK , &set_att_cmd, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_req_set_long_att_value
 * @brief      Set long attribute value
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, attribute handle
 * @param[in]  offset, attribute value offset
 * @param[in]  data_len, attribute value length
 * @param[in]  p_data, attribute value .
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to set the long attribute value
 */

int32_t rsi_ble_set_long_att_value(uint8_t *dev_addr,
                                         uint16_t  handle,
                                         uint16_t offset,
                                         uint8_t data_len,
                                         uint8_t  *p_data)
{
  rsi_ble_set_long_att_value_t  set_long_att = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)set_long_att.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (set_long_att.handle, handle);
  rsi_uint16_to_2bytes (set_long_att.offset, offset);
  set_long_att.length    = RSI_MIN (sizeof (set_long_att.att_value), data_len);
  memcpy (set_long_att.att_value, p_data, set_long_att.length);


  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_LONG_WRITE , &set_long_att, NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_prepare_write  
 * @brief      Prepare attribute value
 * @param[in]  dev_addr, remote device address 
 * @param[in]  handle, attribute handle
 * @param[in]  offset, attribute value offset
 * @param[in]  data_len, attribute value length
 * @param[in]  p_data, attribute value .
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to prepare the attribute value at remote device
 */

int32_t rsi_ble_prepare_write(uint8_t *dev_addr,
                                    uint16_t  handle,
                                    uint16_t offset,
                                    uint8_t data_len,
                                    uint8_t  *p_data)
{
  rsi_ble_req_prepare_write_t  req_prepare_write = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_prepare_write.dev_addr, (int8_t *)dev_addr);
  rsi_uint16_to_2bytes (req_prepare_write.handle, handle);
  rsi_uint16_to_2bytes (req_prepare_write.offset, offset);
  req_prepare_write.length    = RSI_MIN (sizeof (req_prepare_write.att_value), data_len);
  memcpy (req_prepare_write.att_value, p_data, req_prepare_write.length);


  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_PREPARE_WRITE , &req_prepare_write , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_execute_write  
 * @brief      Execute the prepared attribute values
 * @param[in]  dev_addr, remote device address 
 * @param[in]  exe_flag, execute flag to write the values or not 
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to execute the prepared the attribute values
 */

int32_t rsi_ble_execute_write  (uint8_t *dev_addr,
                                    uint8_t  exe_flag)
{
  rsi_ble_req_execute_write_t  req_exe_write = {{0}};


  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)req_exe_write.dev_addr, (int8_t *)dev_addr);
  req_exe_write.flag = exe_flag;

  return rsi_bt_driver_send_cmd(RSI_BLE_REQ_EXECUTE_WRITE , &req_exe_write, NULL);
}

/******************************************************
 * *        GATT Server API's Declarations
 * In GATT server, all actions (add/set/get) are referred to the GATT server running on the local device
 * ******************************************************/

/*==============================================*/
/**
 * @fn         rsi_ble_add_service 
 * @brief      Add new service to local GATT Server
 * @param[in]  service_uuid, new service uuid value 
 * @param[in]  num_of_attributes, number of attributes required in the service.
 * @param[in]  total_data_size, total data size required in the service
 * @param[out] p_resp_serv, new service handler is filled in this structure.
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to add new service to lcal GATT Server
 */

int32_t rsi_ble_add_service (uuid_t  service_uuid,
                             rsi_ble_resp_add_serv_t *p_resp_serv)
{
  rsi_ble_req_add_serv_t  new_service = {{0},0};


  memcpy (&new_service.service_uuid, &service_uuid, sizeof (uuid_t));

  return rsi_bt_driver_send_cmd(RSI_BLE_ADD_SERVICE, &new_service , p_resp_serv);
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_attribute 
 * @brief      Add new attribute to a specific service
 * @param[in]  p_attribute, new attribute to be added to service
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to add new attribute to a service
 */

int32_t rsi_ble_add_attribute (rsi_ble_req_add_att_t  *p_attribute)
{
  return rsi_bt_driver_send_cmd(RSI_BLE_ADD_ATTRIBUTE, p_attribute , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_set_local_att_value 
 * @brief      change the local attribute value.
 * @param[in]  handle, local attributre handle 
 * @param[in]  data_len, attribute value len 
 * @param[in]  p_data, attribute value 
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to change the local attribute value.
 */

int32_t rsi_ble_set_local_att_value (uint16_t  handle,
                                     uint16_t  data_len,
                                     uint8_t *p_data)
{
  rsi_ble_set_local_att_value_t  rec_data = {0};


  rec_data.handle    = handle;
  rec_data.data_len = RSI_MIN (data_len, sizeof(rec_data.data));
  memcpy (rec_data.data, p_data, rec_data.data_len);

  return rsi_bt_driver_send_cmd(RSI_BLE_SET_LOCAL_ATT_VALUE, &rec_data , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_notify_value 
 * @brief      Notify the local value to remote device.
 * @param[in]  handle, local attributre handle 
 * @param[in]  data_len, attribute value len 
 * @param[in]  p_data, attribute value 
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to change the local attribute value.
 */
int32_t rsi_ble_notify_value (uint8_t  *dev_addr, 
                              uint16_t  handle, 
                              uint16_t  data_len,
                              uint8_t  *p_data)
{
  rsi_ble_notify_att_value_t  rec_data = {{0}};

  //memcpy(rec_data.dev_addr, dev_addr, 6);
  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)rec_data.dev_addr, (int8_t *)dev_addr);

  rec_data.handle    = handle;
  rec_data.data_len = RSI_MIN (data_len, sizeof(rec_data.data));
  memcpy (rec_data.data, p_data, rec_data.data_len);

  return rsi_bt_driver_send_cmd(RSI_BLE_CMD_NOTIFY, &rec_data , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_indicate_value 
 * @brief      Indicate the local value to remote device.
 * @param[in]  handle, local attributre handle 
 * @param[in]  data_len, attribute value len 
 * @param[in]  p_data, attribute value 
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to indicate the change of local attribute value to remote device.
 */
int32_t rsi_ble_indicate_value (uint8_t  *dev_addr, 
                              uint16_t  handle, 
                              uint16_t  data_len,
                              uint8_t  *p_data)
{
  rsi_ble_notify_att_value_t  rec_data = {{0}};

//  memcpy(rec_data.dev_addr, dev_addr, 6);
  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)rec_data.dev_addr, (int8_t *)dev_addr);

  rec_data.handle    = handle;
  rec_data.data_len = RSI_MIN (data_len, sizeof(rec_data.data));
  memcpy (rec_data.data, p_data, rec_data.data_len);

  return rsi_bt_driver_send_cmd(RSI_BLE_CMD_INDICATE, &rec_data , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_get_local_att_value 
 * @brief      Get the local attribute value
 * @param[in]  handle, local attribute handle
 * @param[out] p_resp_local_att_val, local attribute value is filled in this structure
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to get the local attribute value
 */
int32_t rsi_ble_get_local_att_value (uint16_t  handle,
                                     rsi_ble_resp_local_att_value_t *p_resp_local_att_val)
{
  rsi_ble_get_local_att_value_t  local_read_val = {0};


  local_read_val.handle = handle;

  return rsi_bt_driver_send_cmd(RSI_BLE_GET_LOCAL_ATT_VALUE, &local_read_val , p_resp_local_att_val);
}

/*==============================================*/
/**
 * @fn         rsi_ble_gatt_read_response 
 * @brief      send the attribute value with read response command.
 * @param[in]  dev_addr, remote device BD Address
 * @param[in]  read_type, 0 - read response, 1- read blob response.
 * @param[in]  handle, local attribute handle
 * @param[in]  offset, local attribute attribute value start offset
 * @param[in]  length, local attribute value length
 * @param[in]  p_data, local attribute value pointer
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to send the local attribute value to remote device
 */
int32_t rsi_ble_gatt_read_response(uint8_t  *dev_addr,
                                   uint8_t   read_type,
                                   uint16_t  handle,
                                   uint16_t  offset,
                                   uint16_t  length,
                                   uint8_t  *p_data)
{
  rsi_ble_gatt_read_response_t  local_read_blob_resp= {{0}};

  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)local_read_blob_resp.dev_addr, (int8_t *)dev_addr);
  local_read_blob_resp.type = read_type;
  //local_read_blob_resp.handle = handle;
  //local_read_blob_resp.offset = offset;
  local_read_blob_resp.data_len = RSI_MIN (length, sizeof (local_read_blob_resp.data));
  memcpy (local_read_blob_resp.data, p_data, local_read_blob_resp.data_len);//local_read_blob_resp.data_len);

  return rsi_bt_driver_send_cmd(RSI_BLE_CMD_READ_RESP, &local_read_blob_resp , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_remove_gatt_service 
 * @brief      Remove the GATT service record.
 * @param[in]  service_handle, GATT service record handle
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to remove the gatt service record
 */
int32_t rsi_ble_remove_gatt_service (uint32_t  service_handler)
{
  rsi_ble_gatt_remove_serv_t  rem_serv;

  rem_serv.serv_hndler = service_handler;

  return rsi_bt_driver_send_cmd(RSI_BLE_RSP_REMOVE_SERVICE, &rem_serv , NULL);
}

/*==============================================*/
/**
 * @fn         rsi_ble_remove_gatt_attibute 
 * @brief      Remove the GATT attributre record.
 * @param[in]  service_handle, GATT service record handle
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to remove the gatt attribute record
 */
int32_t rsi_ble_remove_gatt_attibute (uint32_t  service_handler, uint16_t att_hndl)
{
  rsi_ble_gatt_remove_att_t  rem_att;

  rem_att.serv_hndler = service_handler;
  rem_att.att_hndl    = att_hndl;

  return rsi_bt_driver_send_cmd(RSI_BLE_RSP_REMOVE_ATTRIBUTE, &rem_att , NULL);
}
/*==============================================*/
/**
 * @fn         rsi_ble_att_error_response 
 * @brief      Send att error response
 * @param[in]  handle, requested attribute handle
 * @param[in]  opcode, requested opcode value
 * @param[in]  error_code, error response
 * @return     int32_t 
 *             0  =  success
 *             !0 = failure 
 * @section description
 * This function is used to send att error response
 */
int32_t rsi_ble_att_error_response (uint8_t *dev_addr,
                                     uint16_t  handle,
                                     uint8_t opcode,
                                     uint8_t err)
{
 rsi_ble_att_error_response_t    att_error = {0}; 
  
  
  rsi_ascii_dev_address_to_6bytes_rev((uint8_t *)att_error.dev_addr, (int8_t *)dev_addr);

  att_error.att_handle = handle;
  att_error.req_opcode = opcode;
  att_error.err_code   = err;
  
  return rsi_bt_driver_send_cmd(RSI_BLE_CMD_ATT_ERROR, &att_error , NULL);
}

#endif
