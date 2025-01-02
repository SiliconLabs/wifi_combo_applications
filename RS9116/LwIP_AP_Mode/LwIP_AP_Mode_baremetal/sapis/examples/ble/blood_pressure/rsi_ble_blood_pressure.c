/*************************************************************************
 * 
 * Copyright (c) 2019 Redpine Signals Incorporated. All Rights Reserved.
 * 
 * NOTICE:  All  information  contained  herein is, and  remains  the  property of 
 * Redpine Signals Incorporated. The intellectual and technical concepts contained
 * herein  are  proprietary to  Redpine Signals Incorporated and may be covered by 
 * U.S. and Foreign Patents, patents in process, and are protected by trade secret
 * or copyright law. Dissemination  of this  information or reproduction  of  this
 * material is strictly forbidden unless prior written permission is obtained from
 * Redpine Signals Incorporated.
 */

/* *
 * Include files
 * */

//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>

//! Common include file 
#include<rsi_common_apis.h>
#include "rsi_driver.h"
//! for SFLOAT calculation
#include <float.h>
#include <math.h>

//! Specify GATT role client/server
#define SERVER                                    0
#define CLIENT                                    1
#define GATT_ROLE                                 SERVER

#ifdef RSI_DEBUG_PRINTS
//! [ble_bls] is a tag for every print
#define LOG_PRINT(fmt, args...)                   printf("\n[ble_bls] " fmt, ##args)
#else
#define LOG_PRINT(fmt, args...)
#endif

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! BLE attribute service types uuid values
#define  RSI_BLE_CHAR_SERV_UUID                   0x2803
#define  RSI_BLE_CLIENT_CHAR_UUID                 0x2902

//! BLE characteristic service uuid
#define  RSI_BLE_BLOOD_PRESSURE_SERVICE_UUID      0x1810
#define  RSI_BLE_BLOOD_PRESSURE_MEASUREMENT_UUID  0x2A35
#define  RSI_BLE_INTERMEDIATE_CUFF_PRESSURE_UUID  0x2A36
#define  RSI_BLE_BLOOD_PRESSURE_FEATURE_UUID      0x2A49

//! local device name
#define RSI_BLE_APP_BLOOD_PRESSURE                "BLS"

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR                           "00:23:A7:80:70:B9"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME					   "REDPINE_DEV"


//! attribute properties
#define  RSI_BLE_ATT_PROPERTY_READ                0x02
#define  RSI_BLE_ATT_PROPERTY_WRITE               0x08
#define  RSI_BLE_ATT_PROPERTY_WRITE_WITHOUT_RESP  0x04
#define  RSI_BLE_ATT_PROPERTY_NOTIFY              0x10
#define  RSI_BLE_ATT_PROPERTY_INDICATE            0x20

//! application event list
#define  RSI_APP_EVENT_ADV_REPORT                 0x00
#define  RSI_BLE_CONN_EVENT                       0x01
#define  RSI_BLE_DISCONN_EVENT                    0x02
#define  RSI_BLE_GATT_WRITE_EVENT                 0x03
#define  RSI_BLE_GATT_PROFILE_RESP_EVENT          0x04
#define  RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT    0x05
#define  RSI_BLE_GATT_CHAR_DESC_RESP_EVENT        0x06

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                    1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                      1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                  2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                3000

void rsi_wireless_driver_task(void);

#endif

/* * Special values are assigned to express the following:
 * NaN (Not a Number) [exponent 0, mantissa +(2^11 –1) → 0x07FF]
 * NRes (Not at this Resolution) [exponent 0, mantissa –(2^11) → 0x0800]
 * + INFINITY [exponent 0, mantissa +(2^11 –2) → 0x07FE]
 * – INFINITY [exponent 0, mantissa –(2^11 –2) → 0x0802]
 * Reserved for future use [exponent 0, mantissa –(2^11 –1) → 0x0801]
 * */

//! for SFLOAT calculation
#define SFLOAT_MAX                                20450000000.0
#define SFLOAT_MIN                                (-SFLOAT_MAX)
#define SFLOAT_EPSILON                            1e-8
#define SFLOAT_MANTISSA_MAX                       0x07FD
#define SFLOAT_EXPONENT_MAX                       7
#define SFLOAT_EXPONENT_MIN                       -8
#define SFLOAT_PRECISION                          10000

//! enum for SFLOAT convertion
typedef enum {
  SFLOAT_POSITIVE_INFINITY = 0x07FE,
  SFLOAT_NaN =               0x07FF,
  SFLOAT_NRes =              0x0800,
  SFLOAT_RESERVED_VALUE =    0x0801,
  SFLOAT_NEGATIVE_INFINITY = 0x0802
} ReservedSFloatValues;

//! blood pressure value structure
typedef struct blood_pressure_value_s
{
  int8_t flags;
  float systolic;
  float diastolic;
  float MAP;
  int16_t time_stamp;
  int16_t pluse_rate;
  int8_t uid;
  int16_t status;
}blood_pressure_value_t;

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint16_t rsi_ble_bloodpressure_val_hndl;
static uint16_t rsi_ble_cuffpressure_val_hndl;
static const double reserved_float_values[5] = {INFINITY, NAN, NAN, NAN, -INFINITY};

#if(GATT_ROLE == SERVER)
//! enum for status flag
enum {
  CONNECTED,
  INDICATE,
  NOTIFY
};
int8_t status_flag = 0;
blood_pressure_value_t blood_pressure_val = {0x00,83,50,61,0,0,0,0};
blood_pressure_value_t cuff_pressure_val = {0x00,83,0xFF07,0xFF07,0,0,0,0};
#endif

//! function prototype
uint8_t blood_pressure_cal_value(blood_pressure_value_t, uint8_t *);

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute..
 * @param[in]  serv_handler, service handler.
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  att_val_handle, characteristic value handle
 * @param[in]  att_val_uuid, characteristic value uuid
 * @return     none.
 * @section description
 * This function is used at application to add characteristic attribute
 */
static void rsi_ble_add_char_serv_att (void *serv_handler,
    uint16_t handle,
    uint8_t val_prop,
    uint16_t att_val_handle,
    uuid_t att_val_uuid)
{
  rsi_ble_req_add_att_t  new_att = {0};

  //! preparing the attribute service structure
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  new_att.att_uuid.size = 2;
  new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
  new_att.property = RSI_BLE_ATT_PROPERTY_READ;

  //! preparing the characteristic attribute value
  new_att.data_len = 6;
  new_att.data[0] = val_prop;
  rsi_uint16_to_2bytes (&new_att.data[2], att_val_handle);
  rsi_uint16_to_2bytes (&new_att.data[4], att_val_uuid.val.val16);

  //! Add attribute to the service
  rsi_ble_add_attribute (&new_att);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_val_att
 * @brief      this function is used to add characteristic value attribute.
 * @param[in]  serv_handler, new service handler.
 * @param[in]  handle, characteristic value attribute handle.
 * @param[in]  att_type_uuid, attribute uuid value.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  data, characteristic value data pointer.
 * @param[in]  data_len, characteristic value length.
 * @return     none.
 * @section description
 * This function is used at application to create new service.
 */

static void rsi_ble_add_char_val_att (void *serv_handler,
    uint16_t handle,
    uuid_t   att_type_uuid,
    uint8_t  val_prop,
    uint8_t *data,
    uint8_t  data_len)
{
  rsi_ble_req_add_att_t  new_att = {0};

  //! preparing the attributes
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  memcpy (&new_att.att_uuid, &att_type_uuid, sizeof (uuid_t));
  new_att.property = val_prop;

  //! preparing the attribute value
  new_att.data_len = RSI_MIN(sizeof (new_att.data), data_len);
  memcpy (new_att.data, data, new_att.data_len);

  //! add attribute to the service
  rsi_ble_add_attribute (&new_att);

  //! check the attribute property with notification
  if (val_prop & RSI_BLE_ATT_PROPERTY_NOTIFY || val_prop & RSI_BLE_ATT_PROPERTY_INDICATE)
  {
    //! if notification property supports then we need to add client characteristic service.

    //! preparing the client characteristic attribute & values
    memset (&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler = serv_handler;
    new_att.handle = handle + 1;
    new_att.att_uuid.size = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
    new_att.property = RSI_BLE_ATT_PROPERTY_READ | RSI_BLE_ATT_PROPERTY_WRITE | RSI_BLE_ATT_PROPERTY_WRITE_WITHOUT_RESP;
    new_att.data_len = 2;

    //! add attribute to the service
    rsi_ble_add_attribute (&new_att);
  }

  return;
}

#if(GATT_ROLE == SERVER)
/*==============================================*/
/**
 * @fn         rsi_ble_simple_chat_add_new_serv
 * @brief      this function is used to add new servcie i.e.., blood pressure service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */

static uint32_t rsi_ble_add_blood_pressure_serv (void)
{
  uint8_t len = 0;
  uuid_t  new_uuid = {0};
  uint8_t   data[25] = {0};
  uint16_t  feature = 0;
  rsi_ble_resp_add_serv_t  new_serv_resp = {0};

  //! adding new service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_BLOOD_PRESSURE_SERVICE_UUID;
  rsi_ble_add_service (new_uuid,&new_serv_resp);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_BLOOD_PRESSURE_MEASUREMENT_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 1,
      RSI_BLE_ATT_PROPERTY_INDICATE,
      new_serv_resp.start_handle + 2,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_bloodpressure_val_hndl = new_serv_resp.start_handle + 2;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_BLOOD_PRESSURE_MEASUREMENT_UUID;
  len = blood_pressure_cal_value(blood_pressure_val, data);
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 2,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_INDICATE,
      (uint8_t *)&data,
      len);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_INTERMEDIATE_CUFF_PRESSURE_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 4,
      RSI_BLE_ATT_PROPERTY_NOTIFY,
      new_serv_resp.start_handle + 5,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_cuffpressure_val_hndl = new_serv_resp.start_handle + 5;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_INTERMEDIATE_CUFF_PRESSURE_UUID;
  len = blood_pressure_cal_value(cuff_pressure_val, data);
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 5,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_NOTIFY,
      (uint8_t *)&data,
      len);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_BLOOD_PRESSURE_FEATURE_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 7,
      RSI_BLE_ATT_PROPERTY_READ,
      new_serv_resp.start_handle + 8,
      new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_BLOOD_PRESSURE_FEATURE_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 8,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_READ,
      (uint8_t *)&feature,
      sizeof (feature));
			return 0;
}
#endif

/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * this function is used during ble initialization.
 */
static void rsi_ble_app_init_events()
{
  ble_app_event_map = 0;
  ble_app_event_mask = 0xffffffff;
	ble_app_event_mask = ble_app_event_mask ; //To suppress warning while compiling 

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      set the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * this function is used to set/raise the specific event.
 */
      static void rsi_ble_app_set_event(uint32_t event_num)
{
  ble_app_event_map |= BIT(event_num);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * this function is used to clear the specific event.
 */
static void rsi_ble_app_clear_event(uint32_t event_num)
{
  ble_app_event_map &= ~BIT(event_num);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * this function returns the highest priority event among all the set events
 */
      static int32_t rsi_ble_app_get_event(void)
{
  uint32_t  ix;

  for (ix = 0; ix < 32; ix++)
  {
    if (ble_app_event_map & (1 << ix))
    {
      return ix;
    }
  }

  return (-1);
}
/*==============================================*/
/**
 * @fn         rsi_ble_simple_central_on_adv_report_event
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
void rsi_ble_simple_central_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report)
{

  if (device_found == 1) {
    return;
  }

  memset(remote_name, 0, 31);
  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);
  memcpy (remote_dev_bd_addr, (uint8_t *)adv_report->dev_addr, 6);
  if(((device_found==0) && ((strcmp((const char *)remote_name, RSI_REMOTE_DEVICE_NAME))==0)) ||
      ((remote_addr_type == RSI_BLE_DEV_ADDR_TYPE)&&((strcmp((const char *)remote_dev_addr,RSI_BLE_DEV_ADDR)==0))))
  { 
    device_found=1;
    rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
	conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  conn_event_to_app.status = resp_enh_conn->status;
	rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_connect_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * this callback function indicates the status of the connection
 */
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
  memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_CONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * this callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_DISCONN_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * this callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_gatt_write_event (uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  memcpy (&app_ble_write_event, rsi_ble_write, sizeof (rsi_ble_event_write_t));
#if(GATT_ROLE == SERVER)
  if ((rsi_ble_bloodpressure_val_hndl + 1) == *((uint16_t *)app_ble_write_event.handle))
  {
    //! 0x02 for indication
    if(app_ble_write_event.att_value[0] == 0x02)
    {
      //! set a bit
      status_flag |= (1 << INDICATE);
    }
    else if(app_ble_write_event.att_value[0] == 0)
    {
      //! clear a bit
      status_flag &= ~(1 << INDICATE);
    }
    LOG_PRINT ("indicate val %s\n", (status_flag & (1 << INDICATE))? "Enabled":"Disabled");
  }
  else if ((rsi_ble_cuffpressure_val_hndl + 1) == *((uint16_t *)app_ble_write_event.handle))
  {
    //! 0x01 for notification
    if(app_ble_write_event.att_value[0] == 0x01)
    {
      //! set a bit
      status_flag |= (1 << NOTIFY);
    }
    else if(app_ble_write_event.att_value[0] == 0)
    {
      //! clear a bit
      status_flag &= ~(1 << NOTIFY);
    }
    LOG_PRINT ("notify val %s\n",(status_flag & (1 << NOTIFY))? "Enabled":"Disabled");
  }
#endif
  rsi_ble_app_set_event (RSI_BLE_GATT_WRITE_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_profiles_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] p_ble_resp_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_on_profiles_event (uint16_t resp_status, profile_descriptors_t *rsi_ble_resp_profile)
{
  rsi_ble_app_set_event (RSI_BLE_GATT_PROFILE_RESP_EVENT);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_char_services_event
 * @brief      invoked when response is received for characteristic services details
 * @param[out] p_ble_resp_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_on_char_services_event (uint16_t resp_status, rsi_ble_resp_char_services_t *rsi_ble_resp_char_services)
{
  rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_att_desc_event
 * @brief      its invoked when < > events are received.
 * @param[in]  event_id, it indicates < > event id.
 * @param[in]  rsi_ble_resp_att_desc, < > event parameters.
 * @return     none.
 * @section description
 * this callback function is invoked when < > events are received
 */

static void rsi_ble_on_att_desc_event (uint16_t resp_status, rsi_ble_resp_att_descs_t *rsi_ble_resp_att_desc)
{
  rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_DESC_RESP_EVENT);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_round
 * @brief      convert float/double value to integer value.
 * @param[in]  value, float/duble value.
 * @return     32 bit integer value.
 * @section description
 * this function converts float value to sfloat(ieee 11073 std.).
 */
uint32_t rsi_round (double value)
{
  return ((uint32_t) value);
}

/*==============================================*/
/**
 * @fn         float_to_16bit_sfloat
 * @brief      convert float value to sfloat(ieee 11073 std).
 * @param[in]  data, float type
 * @return     16 bit sfloat value.
 * @section description
 * this function converts float value to sfloat(ieee 11073 std.).
 */

static uint16_t float_to_16bit_sfloat(float data) 
{
  //! exponent is in base 10 i.e. 10**x
  //! in sfloat, 4bits is for exponent i.e. 0xEMMM
  int8_t exponent = 0;
  //! in sfloat, 12bits is for mantissa i.e. 0xEMMM
  double mantissa_f;
  uint32_t mantissa;
  uint16_t result = SFLOAT_NaN;
  double sgn = data > 0 ? +1 : -1;
  double smantissa,rmantissa,mdiff;

  //! check for NAN (not a number)
  if (isnan(data)) 
  {
    goto end;
  }
  else if ((double)data > SFLOAT_MAX) 
  {
    result = SFLOAT_POSITIVE_INFINITY;
    goto end;
  }
  else if ((double)data < SFLOAT_MIN) 
  {
    result = SFLOAT_NEGATIVE_INFINITY;
    goto end;
  }
  else if (data >= -SFLOAT_EPSILON &&
     (double) data <= SFLOAT_EPSILON)
  {
    result = 0;
    goto end;
  }

  mantissa_f = fabs(data);
  // scale up if number is too big
  while (mantissa_f > SFLOAT_MANTISSA_MAX) {
    mantissa_f /= 10.0;
    ++exponent;
    if (exponent > SFLOAT_EXPONENT_MAX) {
      if (sgn > 0) {
        result = SFLOAT_POSITIVE_INFINITY;
      } else {
        result = SFLOAT_NEGATIVE_INFINITY;
      }
      goto end;
    }
  }
  // scale down if number is too small
  while (mantissa_f < 1) {
    mantissa_f *= 10;
    --exponent;
    if (exponent < SFLOAT_EXPONENT_MIN) {
      result = 0;
      goto end;
    }
  }

  // scale down if number needs more precision
  smantissa = rsi_round(mantissa_f * SFLOAT_PRECISION);
  rmantissa = rsi_round(mantissa_f) * SFLOAT_PRECISION;
  mdiff = fabs(smantissa - rmantissa);
  while (mdiff > 0.5 && exponent > SFLOAT_EXPONENT_MIN &&
      (mantissa_f * 10) <= SFLOAT_MANTISSA_MAX) 
  {
    mantissa_f *= 10;
    --exponent;
    smantissa = rsi_round(mantissa_f * SFLOAT_PRECISION);
    rmantissa = rsi_round(mantissa_f) * SFLOAT_PRECISION;
    mdiff = fabs(smantissa - rmantissa);
  }

  mantissa = (int) rsi_round(sgn * mantissa_f);
  //! in 16bits sfloat 4bits is for exponent and 12bits is for mantissa i.e. 0xEMMM
  result = ((exponent & 0xF) << 12) | (mantissa & 0xFFF);
end:
  return result;
}

/*==============================================*/
/**
 * @fn         sfloat_16bit_to_float
 * @brief      convert sfloat(ieee 11073 std) to float.
 * @param[in]  data, 16 bit sfloat type
 * @return     float value.
 * @section description
 * this function converts 16bit sfloat(ieee 11073 std.) to float.
 */

static float sfloat_16bit_to_float(uint16_t data)
{ 
  float output = 0;
  //! in sfloat, 12bits is for mantissa i.e. 0xEMMM
  uint16_t mantissa = data & 0x0FFF;
  //! in sfloat, 4bit is for exponent i.e. 0xEMMM
  int8_t exponent = data >> 12;
  double magnitude = 0;
  //! Exponent range is 7 to -8
  if (exponent >= 0x0008)
  {
    exponent = -((0x000F + 1) - exponent);
  }

  //! Mantissa range SFLOAT_NEGATIVE_INFINITY to SFLOAT_POSITIVE_INFINITY
  if (mantissa >= SFLOAT_POSITIVE_INFINITY && mantissa<= SFLOAT_NEGATIVE_INFINITY) 
  {
    output = reserved_float_values[mantissa - SFLOAT_POSITIVE_INFINITY];
  }
  else
  {
    //! checking resolution SFLOAT_NRes(Not at this Resolution)
    if (mantissa >= SFLOAT_NRes)
    {
      mantissa = -((0x0FFF + 1) - mantissa);
    }
    //! exponent is in base 10 i.e. 10**x
    magnitude = pow(10.0f, exponent);
    output = (mantissa * magnitude);
  }
  return output;
}

/*=============================================*/
/**
 * @fn         blood_pressure_cal_value 
 * @brief      Creates the blood pressure mesurment data.
 * @param[in]  val, it indicates blood pressure mesurement strcture.
 * @param[out] p_data, blood pressure mesurement strcture in buffer pointer.
 * @return     length of blood pressure data buffer.
 * @section description
 */

uint8_t blood_pressure_cal_value(blood_pressure_value_t val, uint8_t *p_data)
{
  int len = 0;
  int16_t temp_systolic = float_to_16bit_sfloat(val.systolic);
  int16_t temp_diastolic = float_to_16bit_sfloat(val.diastolic);
  int16_t temp_MAP = float_to_16bit_sfloat(val.MAP);
  //! payload 1st byte is flags
  p_data[len++] = val.flags;

  //! According to flag, we need to fill data
  //! if flags 0th bit is 1 Blood pressure for Systolic, Diastolic and MAP in units of kPa
  if (!(val.flags & BIT(0)))
  {
    p_data[len++] = temp_systolic & 0xFF;
    p_data[len++] = (temp_systolic >> 8) & 0xFF;

    p_data[len++] = temp_diastolic & 0xFF;
    p_data[len++] = (temp_diastolic >> 8) & 0xFF;

    p_data[len++] = temp_MAP & 0xFF;
    p_data[len++] = (temp_MAP >> 8) & 0xFF;
  }
  //! if flags 0th bit is 0 Blood pressure for Systolic, Diastolic and MAP in units of mmHg
  else
  {
    p_data[len++] = temp_systolic & 0xFF;
    p_data[len++] = (temp_systolic >> 8) & 0xFF;

    p_data[len++] = temp_diastolic & 0xFF;
    p_data[len++] = (temp_diastolic >> 8) & 0xFF;

    p_data[len++] = temp_MAP & 0xFF;
    p_data[len++] = (temp_MAP >> 8) & 0xFF;
  }

  //! need to add more condition

  return len;
}

/*=============================================*/
/*==============================================*/
/**
 * @fn         rsi_ble_blood_pressure_gatt_server
 * @brief      this is the application of blood pressure application.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This function is used to test the blood pressure application.
 */

int32_t rsi_ble_blood_pressure_gatt_server(void)
{

  int32_t status = 0;
  int32_t event_id;
  int8_t data[25] = {0};
  int8_t local_dev_addr[18] = {0};
  static uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = {0};
#if (GATT_ROLE == SERVER)
  uint8_t len;
  uint8_t adv[31] = {2,1,6};
#elif (GATT_ROLE == CLIENT)
  uint8_t ix,jx;
  uuid_t service_uuid;
  uint8_t temp_handle = 0;
  profile_descriptors_t  ble_servs = {0};
  rsi_ble_resp_char_services_t char_servs = {0};
  rsi_ble_resp_att_descs_t  att_desc = {0};
#endif

#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! RS9116 intialisation 
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if (GATT_ROLE == SERVER)
  //! adding BLE blood pressure service
  rsi_ble_add_blood_pressure_serv ();
#endif

  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(
      rsi_ble_simple_central_on_adv_report_event,
      rsi_ble_on_connect_event,
      rsi_ble_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_enhance_conn_status_event,
      NULL,
      NULL,
      NULL);

  //! registering the GATT callback functions
  rsi_ble_gatt_register_callbacks (NULL,
      rsi_ble_on_profiles_event,
      rsi_ble_on_char_services_event,
      NULL,
      rsi_ble_on_att_desc_event,
      NULL,
      NULL,
      rsi_ble_on_gatt_write_event,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL);

  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! Set local name
  rsi_bt_set_local_name((uint8_t *)RSI_BLE_APP_BLOOD_PRESSURE);

  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  rsi_6byte_dev_address_to_ascii ((uint8_t *)local_dev_addr,rsi_app_resp_get_dev_addr);
  LOG_PRINT("local device addr: %s \n",local_dev_addr);

#if(GATT_ROLE == SERVER)
  //! prepare advertise data //local/device name
  adv[3] = strlen (RSI_BLE_APP_BLOOD_PRESSURE) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5], RSI_BLE_APP_BLOOD_PRESSURE);

  //! set advertise data
  rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_APP_BLOOD_PRESSURE) + 5);

  LOG_PRINT ("adv start\n");
  //! set device in advertising mode.
  rsi_ble_start_advertising ();
#endif

#if(GATT_ROLE == CLIENT)
  //! start scanning
  status = rsi_ble_start_scanning();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! waiting for events from controller.
  while (1) {

    //! Application main loop
#ifndef RSI_WITH_OS
    rsi_wireless_driver_task();
#endif
    //! checking for events list
    event_id = rsi_ble_app_get_event ();

    if (event_id == -1) 
    {
#if(GATT_ROLE == SERVER)
      //! test a bit bit_fld & (1 << n)
      if(status_flag & (1 << CONNECTED))
      {
#ifdef __linux__
        usleep(3000000);
#endif
#ifdef RSI_M4_INTERFACE 
        rsi_delay_ms (5000);
#endif

        if(status_flag & (1 << INDICATE))
        {
          LOG_PRINT ("indicate \n");
          len = blood_pressure_cal_value(blood_pressure_val,(uint8_t *) data);
          rsi_ble_set_local_att_value (rsi_ble_bloodpressure_val_hndl, len,(uint8_t *) data);
        }

        if(status_flag & (1 << NOTIFY))
        {
          LOG_PRINT ("notify \n");
          len = blood_pressure_cal_value(cuff_pressure_val,(uint8_t  *) data);
          rsi_ble_set_local_att_value (rsi_ble_cuffpressure_val_hndl, len,(uint8_t  *) data);
        }
      }

#endif
      continue;
    }

    switch (event_id) {
#if (GATT_ROLE == CLIENT)
      case RSI_APP_EVENT_ADV_REPORT:
        {
          //! advertise report event.

          //! clear the advertise report event.
          rsi_ble_app_clear_event (RSI_APP_EVENT_ADV_REPORT);

          status = rsi_ble_connect(remote_addr_type, (int8_t *)remote_dev_bd_addr);
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("connect status: 0x%X\r\n", status);
          }

        }
    break;
#endif
    case RSI_BLE_CONN_EVENT:
    {
      //! event invokes when connection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_CONN_EVENT);
          LOG_PRINT ("conn \n");
#if(GATT_ROLE == SERVER)
          //! set a bit
          status_flag |= (1 << CONNECTED);
#endif

#if(GATT_ROLE == CLIENT)
          //rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, (uint8_t *)conn_event_to_app.dev_addr);
          service_uuid.size = 2;
          service_uuid.val.val16 = RSI_BLE_BLOOD_PRESSURE_SERVICE_UUID;
retry:          
          //! query particular profile details from the connected remote device.
          status = rsi_ble_get_profile (remote_dev_addr, service_uuid, &ble_servs);
          if(status != 0)
          {
            goto retry;
          }
#endif
        }
        break;

      case RSI_BLE_DISCONN_EVENT:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_DISCONN_EVENT);
          LOG_PRINT ("disconn \n");

#if(GATT_ROLE == SERVER)
          //! clear flag
          status_flag = 0;

adv:
          LOG_PRINT ("adv start\n");
          //! set device in advertising mode.
          status = rsi_ble_start_advertising ();
          if (status != RSI_SUCCESS)
          {
            goto adv;
          }
#endif

#if(GATT_ROLE == CLIENT)
      //! start scanning
      device_found=0;
      status = rsi_ble_start_scanning();
      if(status != RSI_SUCCESS)
      {
        LOG_PRINT ("start_scanning status: 0x%X\r\n", status);
      }
#endif
        }
        break;

      case RSI_BLE_GATT_WRITE_EVENT:
        {
          //! event invokes when write/notification events received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE_EVENT);
#if(GATT_ROLE == CLIENT)
          if (rsi_ble_bloodpressure_val_hndl != 0)
          {
            if (rsi_ble_bloodpressure_val_hndl == *((uint16_t *)app_ble_write_event.handle))
            {
              //! unit in mm Hg
              if((app_ble_write_event.att_value[0] & 1) == 0)
              {
                LOG_PRINT("blood pressure measurement value:\n\tSystolic: %g mmHg\n\tDiastolic: %g mmHg\n\tMean: %g mmHg\n",
                    sfloat_16bit_to_float(app_ble_write_event.att_value[1] | (app_ble_write_event.att_value[2] << 8)),
                    sfloat_16bit_to_float(app_ble_write_event.att_value[3] | (app_ble_write_event.att_value[4] << 8)),
                    sfloat_16bit_to_float(app_ble_write_event.att_value[5] | (app_ble_write_event.att_value[6] << 8)));
              }
              //! unit in k Pa
              else
              {
                LOG_PRINT("blood pressure measurement value:\n\tSystolic: %g kPa\n\tDiastolic: %g kPa\n\tMean: %g kPa\n",
                    sfloat_16bit_to_float(app_ble_write_event.att_value[1] | (app_ble_write_event.att_value[2] << 8)),
                    sfloat_16bit_to_float(app_ble_write_event.att_value[3] | (app_ble_write_event.att_value[4] << 8)),
                    sfloat_16bit_to_float(app_ble_write_event.att_value[5] | (app_ble_write_event.att_value[6] << 8)));

              }

            }
          }

          if (rsi_ble_cuffpressure_val_hndl != 0)
          {
            if (rsi_ble_cuffpressure_val_hndl == *((uint16_t *)app_ble_write_event.handle))
            {
              //! unit in mm Hg
              if((app_ble_write_event.att_value[0] & 1) == 0)
              {
                LOG_PRINT("intermediate cuff pressure value: %g mmHg\n",
                    sfloat_16bit_to_float(app_ble_write_event.att_value[1] | (app_ble_write_event.att_value[2] << 8)));
              }
              //! unit in k Pa
              else
              {
                LOG_PRINT("intermediate cuff pressure value: %g kPa\n",
                    sfloat_16bit_to_float(app_ble_write_event.att_value[1] | (app_ble_write_event.att_value[2] << 8)));

              }
            }
          }

#endif

        }
        break;

#if(GATT_ROLE == CLIENT)     
      case RSI_BLE_GATT_PROFILE_RESP_EVENT:
        {
          //! event invokes when get profile response received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_PROFILE_RESP_EVENT);

          if(0 != *(uint16_t *)ble_servs.start_handle && 0 != *(uint16_t *)ble_servs.end_handle)
          {
            //! query characteristic services, with in the particular range, from the connected remote device.
            rsi_ble_get_char_services (remote_dev_bd_addr,
                *(uint16_t *)ble_servs.start_handle,
                *(uint16_t *)ble_servs.end_handle,
                &char_servs);
          }
        }
        break;

      case RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT:
        {
          //! event invokes when get characteristics of the service response received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);

          //! verifying the blood pressure characteristic
          for (ix = 0; ix < char_servs.num_of_services; ix++)
          {
            LOG_PRINT ("serv_handle: 0x%04x - property: 0x%02x, char_handle: 0x%04x, char_uuid: 0x%04x \n",
                char_servs.char_services[ix].handle,
                char_servs.char_services[ix].char_data.char_property,
                char_servs.char_services[ix].char_data.char_handle,
                char_servs.char_services[ix].char_data.char_uuid.val.val16);

            if (char_servs.char_services[ix].char_data.char_uuid.val.val16 == RSI_BLE_BLOOD_PRESSURE_MEASUREMENT_UUID) 
            {
              rsi_ble_bloodpressure_val_hndl = char_servs.char_services[ix].char_data.char_handle;

              //! query attribute descriptor, with in the particular range, from the connected remote device.
              rsi_ble_get_att_descriptors (remote_dev_bd_addr, 
                  char_servs.char_services[ix].handle, 
                  (char_servs.char_services[ix /*+ 1*/].handle+2/*-1*/), 
                  &att_desc);
              //! Wait for respone of above cmd(get_att_descriptors_async)
              temp_handle = ix;
              break;
            }
          }      

        }
        break;

      case RSI_BLE_GATT_CHAR_DESC_RESP_EVENT:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_DESC_RESP_EVENT);
          for (ix = 0; ix < att_desc.num_of_att; ix++)
          {
            LOG_PRINT("char_uuid: 0x%04x\n",att_desc.att_desc[ix].att_type_uuid.val.val16);
            if (att_desc.att_desc[ix].att_type_uuid.val.val16 == RSI_BLE_CLIENT_CHAR_UUID) 
            {
              if (att_desc.att_desc[ix-1].att_type_uuid.val.val16 == RSI_BLE_BLOOD_PRESSURE_MEASUREMENT_UUID) 
              {
                LOG_PRINT ("CCCD indicate\n");
                //0x0002 to enable indication
                data[0] = 0x02;
                data[1] = 0x00;
                //! Set attribute value of the connected remote device.Ack will not be received from the remote device.
                rsi_ble_set_att_value (remote_dev_bd_addr, 
                    *((uint16_t *)att_desc.att_desc[ix].handle),
                    2,
                    data);
                memset (&att_desc, 0, sizeof(rsi_ble_resp_att_descs_t));
                //! query for next att_descriptors
                for (jx = temp_handle+1; jx < char_servs.num_of_services; jx++)
                {
                  LOG_PRINT ("serv_handle: 0x%04x - property: 0x%02x, char_handle: 0x%04x, char_uuid: 0x%04x \n",
                      char_servs.char_services[jx].handle,
                      char_servs.char_services[jx].char_data.char_property,
                      char_servs.char_services[jx].char_data.char_handle,
                      char_servs.char_services[jx].char_data.char_uuid.val.val16);

                  if (char_servs.char_services[jx].char_data.char_uuid.val.val16 == RSI_BLE_INTERMEDIATE_CUFF_PRESSURE_UUID) 
                  {
                    rsi_ble_cuffpressure_val_hndl = char_servs.char_services[jx].char_data.char_handle;
                    //! query attribute descriptor, with in the particular range, from the connected remote device.
                    rsi_ble_get_att_descriptors (remote_dev_bd_addr, 
                        char_servs.char_services[jx].handle, 
                        (char_servs.char_services[jx /*+ 1*/].handle+2/*-1*/), 
                        &att_desc);
                    temp_handle = jx;
                  }

                }
              }
              else if (att_desc.att_desc[ix-1].att_type_uuid.val.val16 == RSI_BLE_INTERMEDIATE_CUFF_PRESSURE_UUID)
              {
                LOG_PRINT ("CCCD notify\n");
                //0x0001 to enable notification
                data[0] = 0x01;
                data[1] = 0x00;
                //! Set attribute value of the connected remote device.Ack will not be received from the remote device.
                rsi_ble_set_att_value (remote_dev_bd_addr, 
                    *((uint16_t *)att_desc.att_desc[ix].handle),
                    2,
                    data);
              }

            }
          }
        }
        break;

#endif
    }
  }
}

/*==============================================*/
/**
 * @fn         main_loop
 * @brief      Schedules the Driver task.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function schedules the Driver task.
 */
void main_loop (void)
{
  while (1){
    rsi_wireless_driver_task();
  }
}

/*==============================================*/
/**
 * @fn         main
 * @brief      main function
 * @param[in]  none.
 * @return     none.
 * @section description
 * This is the main function to call Application
 */
int main (void)
{
#ifdef RSI_WITH_OS
  int32_t status;
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif



#ifndef RSI_WITH_OS

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! Call blood pressure Server Application
  rsi_ble_blood_pressure_gatt_server();

  //! Application main loop
  main_loop();

  return 0;
#endif
#ifdef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! RS9116 initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_ble_blood_pressure_gatt_server, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}

