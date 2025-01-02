/**
 * @file     rsi_bt_sdp_apis.c
 * @version  0.1
 * @date     3 May 2019
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

/*==============================================*/


/**
 * @fn         rsi_bt_set_sdp_attr_id
 * @brief      Sets the Attribute ID
 * @param[in]  sdp_attribute_record_pointer, attribute_id, attribute_buf_len 
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the attribute data to the records pointer.
 */
uint32_t  rsi_bt_set_sdp_attr_id (rsi_sdp_att_record_t *att_rec,
                                  uint16_t             attr_id,
                                  uint16_t             pattr_buf_idx,
                                  uint16_t             attr_buf_len)
{
  uint8_t       cnt;
  attr_id_array_t  attr;
  uint8_t       j;


  if (att_rec->attr_array_cnt >= MAX_SDP_ATTR) 
  {
    return 0x4E99;
  }

  /* Framing service attribute in SDP attribute structure       */
  att_rec->attr_array[att_rec->attr_array_cnt].attr_buf_ptr_idx = pattr_buf_idx;
  att_rec->attr_array[att_rec->attr_array_cnt].attr_buf_len = attr_buf_len;
  att_rec->attr_array[att_rec->attr_array_cnt].attr_id     = attr_id;
  att_rec->attr_array_cnt++;

  for (cnt = 0; cnt < att_rec->attr_array_cnt - 1; cnt++) 
  {
    /* Arranging attributes in Ascending order in Attribute array */
    for (j = 0; j < att_rec->attr_array_cnt - 1 - cnt; j++) 
    {
      if (att_rec->attr_array[j + 1].attr_id < att_rec->attr_array[j].attr_id) 
      {
        attr = att_rec->attr_array[j];
        att_rec->attr_array[j] = att_rec->attr_array[j + 1];
        att_rec->attr_array[j + 1] = attr;
      }
    }
  }
  return RSI_SUCCESS;
}

/**
 * @fn         rsi_bt_add_sdp_attribute
 * @brief      Adds the attribute to particular record
 * @param[in]  sdp_attribute_record_pointer, attribute_id, param_lenght, attribute_data_8, attribute_data_16     
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the attribute data to the records pointer.
 */
uint32_t  rsi_bt_add_sdp_attribute(rsi_sdp_att_record_t *att_rec, uint16_t  attr_id, uint8_t att_data_8, uint16_t att_data_16,uint8_t is_boolean, uint8_t param_len)
{

  uint32_t  status = RSI_SUCCESS;
  uint8_t  *pattr_buf;
  uint16_t idx= 0;
  uint8_t  attr_type;
 
  if(is_boolean)
  {
    attr_type = BT_ATTR_TYPE_BOOLEAN;
  }
  else
  {
    attr_type = BT_ATTR_TYPE_UNSIGNED_INTEGER;
  }

  attr_type = (attr_type << 3);
  if (param_len == 1) 
  {
    attr_type = attr_type | 0;
  } 
  else if (param_len == 2) 
  {
    attr_type = attr_type | 1;
  }

  pattr_buf = (uint8_t*)&att_rec->buf_array[att_rec->buf_len];


  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                               /* Attribute type unsigned integer 1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id);                            /* Attribute ID                    2 Octets */
  idx += 2;

  if(attr_id == BT_ATTR_ID_BROWSE_GROUP_LIST)
  {
    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x35);                               /* Attribute type unsigned integer 1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x03);                               /* Attribute type unsigned integer 1 Octet  */
    idx += 1;
    attr_type |= (BT_ATTR_TYPE_UUID << 3);
  }


  BT_MEM_WR_BE_08U(pattr_buf + idx,  attr_type);                         /* Attribute type sequence         1 Octet  */
  idx += 1;

  if(param_len == 1)
  {
    BT_MEM_WR_BE_08U(pattr_buf + idx, att_data_8);
    idx += 1;
  }
  else
  {
    BT_MEM_WR_BE_16U(pattr_buf + idx, att_data_16);
    idx += 2;
  }

  status = rsi_bt_set_sdp_attr_id(att_rec, attr_id, att_rec->buf_len, idx);                   /* Adds attribute to service record*/
  att_rec->buf_len += idx;
  return status;    
}


/**
 * @fn         rsi_bt_add_sdp_hid_language_attribute
 * @brief      Adds the sdp hid language attribute to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, lang id, lang attr base
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the attribute data to the records pointer.
 */
uint32_t rsi_bt_add_sdp_hid_language_attribute(rsi_sdp_att_record_t  *att_record_data, uint16_t lang_id, uint16_t lang_attr_base)
{
  uint16_t  attr_id;
  uint8_t   *pattr_buf;
  uint16_t  idx;
  uint32_t   status = RSI_SUCCESS;

  idx       = 0;
  attr_id   = BT_ATTR_ID_HID_LANG_ID_BASE_LIST;
  pattr_buf = (uint8_t *)&att_record_data->buf_array[att_record_data->buf_len];


  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                               /* Attribute type unsigned integer 1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id);                            /* Attribute ID                    2 Octets */
  idx += 2;
  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x35);                               /* Attribute type sequence         1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_08U(pattr_buf + idx, 8);
  idx += 1;


  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x35);                                   /* Attribute type sequence         1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_08U(pattr_buf + idx, 6);
  idx += 1;


  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                               /* Attribute type unsigned integer 1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_16U(pattr_buf + idx, lang_id);                            /* Attribute ID                    2 Octets */
  idx += 2;

  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                               /* Attribute type unsigned integer 1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_16U(pattr_buf + idx, lang_attr_base);                     /* Attribute ID                    2 Octets */
  idx += 2;

  status = rsi_bt_set_sdp_attr_id(att_record_data, attr_id, att_record_data->buf_len, idx);                   /* Adds attribute to service record         */
  att_record_data->buf_len += idx;
  return status;
}

/**
 * @fn         rsi_bt_add_sdp_hid_descriptor_list
 * @brief      Adds the sdp hid descriptor attribute to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, buffer_pointer, buffer_len
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the attribute data to the records pointer.
 */
uint32_t  rsi_bt_add_sdp_hid_descriptor_list(rsi_sdp_att_record_t  *att_record_data, uint8_t *buff_ptr, uint8_t buff_len)
{
  uint16_t attr_id;
  uint8_t  *pattr_buf;
  uint16_t idx;
  uint32_t status = RSI_SUCCESS;

  idx       = 0;
  attr_id   = BT_ATTR_ID_HID_DESCRIPTIOR_LIST;
  pattr_buf = (uint8_t*)&att_record_data->buf_array[att_record_data->buf_len];

  /* Attribute type unsigned integer 1 Octet  */
  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                              
  idx += 1;
  /* Attribute ID 2 Octets */
  BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id);                    
  idx += 2;

  /* Copy Desc Value exactly , it contains full structure */
  memcpy(pattr_buf + idx, buff_ptr, buff_len);
  idx += buff_len;

  /* Adds attribute to service record */
  status = rsi_bt_set_sdp_attr_id(att_record_data, attr_id, att_record_data->buf_len, idx);                 
  att_record_data->buf_len += idx;
  return status;
}

/**
 * @fn         rsi_bt_add_sdp_service_attribute
 * @brief      Adds the sdp service attribute to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, service name pointer, len of the service name, attribute id
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the service name attribute to the service record.
 */
uint32_t rsi_bt_add_sdp_service_attribute(rsi_sdp_att_record_t *att_rec, char *service_name, uint8_t  name_len, uint16_t attr_id)
{
    uint8_t      *pattr_buf;
    uint16_t       idx;
    uint8_t      attr_type;
    uint32_t  status = RSI_SUCCESS;
    idx       = 0;

    attr_type = BT_ATTR_TYPE_TEXT_STRING;
    attr_type = (attr_type << 3);                                          
    attr_type = (attr_type | 5); 
    pattr_buf = (uint8_t*)&att_rec->buf_array[att_rec->buf_len];

    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                              
    /* Attribute type unsigned integer 1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id);                            
    /* Attribute ID 2 Octets */
    idx += 2;
    BT_MEM_WR_BE_08U(pattr_buf + idx, attr_type);
    /* Attribute type Text string  1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_08U(pattr_buf + idx, name_len);     
    idx += 1;
    memcpy(pattr_buf + idx, service_name, name_len);
    idx += name_len;

    status = rsi_bt_set_sdp_attr_id(att_rec, attr_id, att_rec->buf_len, idx);      
    /* Adds attribute to service record */
    att_rec->buf_len += idx;
    return status;
}


/**
 * @fn         rsi_bt_add_sdp_service_classid
 * @brief      Adds the sdp service class id attribute to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, service class uuid
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the service name attribute to the service record.
 */
uint32_t  rsi_bt_add_sdp_service_classid (rsi_sdp_att_record_t *att_rec,
                                          uint16_t         serv_class_uuid)
{
    uint16_t     attr_id;
    uint8_t      attr_type;
    uint8_t     *pattr_buf;
    uint16_t     idx;
    uint32_t      status = RSI_SUCCESS;


    idx       = 0;
    attr_id   = BT_ATTR_ID_SERVICE_CLASS_ID_LIST;
    attr_type = BT_ATTR_TYPE_UUID;
    pattr_buf = (uint8_t*)&att_rec->buf_array[att_rec->buf_len];
    attr_type = (attr_type << 3);                                           
    attr_type = attr_type | 1;

    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                               
    /* Attribute type unsigned integer 1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id);  
    /* Attribute ID   2 Octets */
    idx += 2;

    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x35);                               
    idx += 1;
    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x03);                               
    idx += 1;

    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x19);
    /* Attribute type UUID 1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, serv_class_uuid);  
    /* Service class id 2 Octets */
    idx += 2;

    status = rsi_bt_set_sdp_attr_id(att_rec, attr_id, att_rec->buf_len, idx);
    /* Adds attribute to service record */
    att_rec->buf_len += idx;
    return status;
}


/**
 * @fn         rsi_bt_add_sdp_service_handle
 * @brief      Adds the sdp service handle attribute to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, service handle
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the service name attribute to the service record.
 */
uint32_t  rsi_bt_add_sdp_service_handle (rsi_sdp_att_record_t *att_rec,
                                         uint32_t         serv_hndl)
{
    uint16_t       attr_id;
    uint8_t      attr_type;
    uint8_t      *pattr_buf;
    uint16_t       idx;
    uint32_t     status = RSI_SUCCESS;


    idx       = 0;
    attr_id   = BT_ATTR_ID_SERVICE_RECORD_HANDLE;
    attr_type = BT_ATTR_TYPE_UNSIGNED_INTEGER;
    pattr_buf = (uint8_t*)&att_rec->buf_array[att_rec->buf_len];
    attr_type = (attr_type << 3);                            
    attr_type = attr_type | 2;    

    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09); 
    /* Attribute type unsigned integer 1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id);
    /* Attribute ID    2 Octets */
    idx += 2;
    BT_MEM_WR_BE_08U(pattr_buf + idx, attr_type);   
    /* Attribute type unsigned integer 1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_32U(pattr_buf + idx, serv_hndl);
    /* Service record handle 4 Octets */
    idx += 4;

    status = rsi_bt_set_sdp_attr_id(att_rec, attr_id, att_rec->buf_len, idx);                   
    /* Adds attribute to service record */
    att_rec->buf_len += idx;

    return status;
}


/**
 * @fn         rsi_bt_add_sdp_protocol_list
 * @brief      Adds the sdp protocol list attribute to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, sdp protocol list, list cnt, attribute id
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the sdp protocol attribute list to the service record.
 */
uint32_t rsi_bt_add_sdp_protocol_list (rsi_sdp_att_record_t *att_rec, bt_sdp_proto_desc_list_elem_t *list, uint8_t list_cnt,uint16_t attr_id)
{
  uint8_t     *pattr_buf;
  uint16_t     idx;
  uint8_t      proto_idx;
  uint8_t      param_idx;
  uint32_t     status = RSI_SUCCESS;

  idx       = 0;
  pattr_buf = (uint8_t*)&att_rec->buf_array[att_rec->buf_len];


  uint8_t tmp_len = 0;
  for (proto_idx = 0; proto_idx < list_cnt; proto_idx++) 
  {
    tmp_len += 2 + (2 + 1) + ((1 + 2)*  list[proto_idx].num_params); 
  }
  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);            
  idx += 1;
  BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id); 
  idx += 2;
  if(attr_id == BT_ATTR_ID_ADDITIONAL_PROTOCOL_DESC_LIST)
  {
    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x35); 
    idx += 1;
    BT_MEM_WR_BE_08U(pattr_buf + idx, tmp_len + 2);
    idx += 1;
  }
  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x35); 
  idx += 1;
  BT_MEM_WR_BE_08U(pattr_buf + idx, tmp_len);
  idx += 1;

  for (proto_idx = 0; proto_idx < list_cnt; proto_idx++) 
  {
    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x35);                                   
    idx += 1;
    BT_MEM_WR_BE_08U(pattr_buf + idx, 2 + 1 +  (3 * list[proto_idx].num_params));
    idx += 1;

    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x19);                                   
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, list[proto_idx].proto_uuid);                          
    idx += 2;

    for (param_idx = 0; param_idx < list[proto_idx].num_params; param_idx++) 
    {
      BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                          
      idx += 1;
      BT_MEM_WR_BE_16U(pattr_buf + idx, list[proto_idx].params[param_idx]);           
      idx += 2;
    }
  }

  /* Adds attribute to service record */
  status = rsi_bt_set_sdp_attr_id(att_rec, attr_id, att_rec->buf_len, idx);                   
  att_rec->buf_len += idx;
  return status;
}


/**
 * @fn         rsi_bt_add_sdp_language_base_attributeid_list
 * @brief      Adds the sdp language base attribute list to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, bt sdp language attribute element list, list count
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the sdp language base attribute list to the service record.
 */
uint32_t rsi_bt_add_sdp_language_base_attributeid_list (rsi_sdp_att_record_t *att_rec, bt_sdp_lang_attr_id_elem_t *list, uint8_t list_cnt)
{
  uint8_t     *pattr_buf;
  uint16_t    idx;
  uint8_t     cnt;
  uint32_t    status = RSI_SUCCESS;


  idx       = 0;
  pattr_buf = (uint8_t*)&att_rec->buf_array[att_rec->buf_len];

  /* Attribute type unsigned integer : SDP_DATA_TYPE_UINT 0x08 | SDP_DATA_SIZE_16 0x1 */
  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);
  idx += 1;
  /* Attribute ID : lang base attr ID list*/
  BT_MEM_WR_BE_16U(pattr_buf + idx, BT_ATTR_ID_LANGUAGE_BASE_ATTR_ID_LIST);
  idx += 2;

  /* Attribute type sequence : SDP_DATA_TYPE_SEQ 0x30 | SDP_DATA_SIZE_2B 0x6 */
  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x36); 
  idx += 1;
  /* Element data sequence length : member(3+6 bytes) * list_cnt */
  BT_MEM_WR_BE_16U(pattr_buf + idx, 9 * list_cnt);
  idx += 2;

  for (cnt = 0; cnt < list_cnt; cnt++)
  {
    /* Natural language ID */
    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, (list + cnt)->lang_id);
    idx += 2;
    /* Language encoding */
    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, (list + cnt)->lang_encode_id);
    idx += 2;
    /* Base attribute ID for the language */
    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, (list + cnt)->base_attr_id);
    idx += 2;
  } 
  
  /* Adds attribute to service record         */
  status = rsi_bt_set_sdp_attr_id(att_rec, BT_ATTR_ID_LANGUAGE_BASE_ATTR_ID_LIST, att_rec->buf_len, idx);                
  /* Adds attribute to service record   */

  /* update attr buf len*/
  att_rec->buf_len += idx;

  return status;
}



/**
 * @fn         rsi_bt_add_sdp_profile_descriptor_list
 * @brief      Adds the sdp profile descriptor to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, profile uuid, profile version
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the sdp language base attribute list to the service record.
 */
uint32_t  rsi_bt_add_sdp_profile_descriptor_list (rsi_sdp_att_record_t *att_rec, uint16_t profile_uuid, uint16_t profile_version)
{
  uint16_t     attr_id;
  uint8_t      attr_type;
  uint8_t      *pattr_buf;
  uint16_t     idx;
  uint32_t     status = RSI_SUCCESS;

  idx       = 0;
  attr_id   = BT_ATTR_ID_BLUETOOTH_PROFILE_DESC_LIST;
  attr_type = BT_ATTR_TYPE_UNSIGNED_INTEGER;
  pattr_buf = (uint8_t*)&att_rec->buf_array[att_rec->buf_len];
  attr_type = (attr_type << 3);                                              
  attr_type = attr_type | 1;


  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                               
  /* Attribute type unsigned integer 1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id);  
  /* Attribute ID 2 Octets */
  idx += 2;
  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x36);
  /* Attribute type sequence 1 Octet  */
  idx += 1;
  /* Attribute length 2 Octets */
  BT_MEM_WR_BE_16U(pattr_buf + idx, 0);
  idx += 2;

  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x35);
  /* Attribute type sequence  1 Octet  */
  idx += 1;
  /* Attribute length 2 Octets */
  BT_MEM_WR_BE_08U(pattr_buf + idx, 2 + 1 + ((2 + 1) * 1));
  idx += 1;


  BT_MEM_WR_BE_08U(pattr_buf + idx, 0x19);
  /* Attribute type UUID  1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_16U(pattr_buf + idx, profile_uuid); 
  /* Protocol UUID 2 Octets */
  idx += 2;

  BT_MEM_WR_BE_08U(pattr_buf + idx, attr_type);
  /* Attribute type unsigned integer 1 Octet  */
  idx += 1;
  BT_MEM_WR_BE_16U(pattr_buf + idx, profile_version);
  /* Parameter for communication     2 Octets */
  idx += 2;

  BT_MEM_WR_BE_16U(pattr_buf + 4, idx - 6);

  status = rsi_bt_set_sdp_attr_id(att_rec, attr_id, att_rec->buf_len, idx);
  /* Adds attribute to service record */
  att_rec->buf_len += idx;
  return status;
}

/**
 * @fn         rsi_bt_add_sdp_service_record_handle
 * @brief      Adds the sdp profile descriptor to the sdp service record
 * @param[in]  sdp_attribute_record_pointer, service_handle
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used to add the sdp service handle attribute list to the service record.
 */
uint32_t  rsi_bt_add_sdp_service_record_handle (rsi_sdp_att_record_t *att_rec,
                                                uint32_t         serv_hndl)
{
    uint16_t     attr_id;
    uint8_t      attr_type;
    uint8_t      *pattr_buf;
    uint16_t     idx;
    uint32_t     status = RSI_SUCCESS;

    idx       = 0;
    attr_id   = BT_ATTR_ID_SERVICE_RECORD_HANDLE;
    attr_type = BT_ATTR_TYPE_UNSIGNED_INTEGER;
    pattr_buf = (uint8_t*)&att_rec->buf_array[att_rec->buf_len];
    attr_type = (attr_type << 3);                                             
    attr_type = attr_type | 2;                                                

    BT_MEM_WR_BE_08U(pattr_buf + idx, 0x09);                                  
    /* Attribute type unsigned integer 1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_16U(pattr_buf + idx, attr_id);                                
    /* Attribute ID  2 Octets */
    idx += 2;
    BT_MEM_WR_BE_08U(pattr_buf + idx, attr_type);                              
    /* Attribute type unsigned integer 1 Octet  */
    idx += 1;
    BT_MEM_WR_BE_32U(pattr_buf + idx, serv_hndl);                              
    /* Service record handle   4 Octets */
    idx += 4;

    status = rsi_bt_set_sdp_attr_id(att_rec, attr_id, att_rec->buf_len, idx);                   
    /* Adds attribute to service record */
    att_rec->buf_len += idx;

    return status;
}
#endif
