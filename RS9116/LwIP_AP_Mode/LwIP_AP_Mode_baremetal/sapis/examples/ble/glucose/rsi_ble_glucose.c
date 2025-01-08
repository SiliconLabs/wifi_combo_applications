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

#include <string.h>
#include "rsi_driver.h"
//! for SFLOAT calculation
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

//! Specify GATT role client/server
#define SERVER                                    0
#define CLIENT                                    1
#define GATT_ROLE                                 SERVER

//! [ble_gls] is a tag for every print
#ifdef RSI_DEBUG_PRINTS
#define LOG_PRINT(fmt, args...)                   printf("\n[ble_gls] " fmt, ##args)
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
#define  RSI_BLE_GLUCOSE_SERVICE_UUID             0x1808
#define  RSI_BLE_GLUCOSE_MEASUREMENT_UUID         0x2A18
#define  RSI_BLE_GLUCOSE_MEASUREMENT_CONTEXT_UUID 0x2A34
#define  RSI_BLE_GLUCOSE_FEATURE_UUID             0x2A51
#define  RSI_BLE_RECORD_ACCESS_CONTROL_POINT_UUID 0x2A52

//! local device name
#define RSI_BLE_APP_GLUCOSE                       "GLS"

//! Address type of the device to connect
#define RSI_BLE_DEV_ADDR_TYPE LE_PUBLIC_ADDRESS

//! Address of the device to connect
#define RSI_BLE_DEV_ADDR                           "00:23:A7:80:70:B9"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME					   "REDPINE_DEV"



//! local device IO capability
#define  RSI_BLE_SMP_IO_CAPABILITY                0x02
#define  RSI_BLE_SMP_PASSKEY                      0

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
#define  RSI_BLE_APP_CHAR_MEAS_SEND_EVENT         0x07
#define  RSI_BLE_APP_CHAR_CONTEXT_SEND_EVENT      0x08
#define  RSI_BLE_APP_CHAR_RECORD_DEL_EVENT        0x09
#define  RSI_BLE_SMP_REQ_EVENT                    0x0a
#define  RSI_BLE_SMP_RESP_EVENT                   0x0b
#define  RSI_BLE_SMP_PASSKEY_EVENT                0x0c
#define  RSI_BLE_SMP_FAILED_EVENT                 0x0d
#define  RSI_BLE_ENCRYPT_STARTED_EVENT            0x0e
#define  RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT        0x0f
#define  RSI_BLE_LTK_REQ_EVENT                    0x10

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

#define GLUCOSE_MAX_RECORD                        3

//! glucose measurement flags
#define GLUCOSE_MEAS_FLAG_TIME_OFFSET             0
#define GLUCOSE_MEAS_FLAG_TYPE_SAMPLE_LOC         2
#define GLUCOSE_MEAS_FLAG_CONCENTRATION           4
#define GLUCOSE_MEAS_FLAG_SENSOR_STATUS_ANN       8
#define GLUCOSE_MEAS_FLAG_CONTEXT_FOLLOWS         16

//! glucose measurement type
//! Lower nibble of type_sample_location
#define GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD        1
#define GLUCOSE_TYPE_CAPILLARY_PLASMA             2
#define GLUCOSE_TYPE_VENOUS_WHOLE_BLOOD           3
#define GLUCOSE_TYPE_VENOUS_PLASMA                4
#define GLUCOSE_TYPE_ARTERIAL_WHOLE_BLOOD         5
#define GLUCOSE_TYPE_ARTERIAL_PLASMA_BLOOD        6
#define GLUCOSE_TYPE_UNDERTERMINED_WHOLE_BLOOD    7
#define GLUCOSE_TYPE_UNDERTERMINATED_PLASMA       8
#define GLUCOSE_TYPE_IFS                          9
#define GLUCOSE_TYPE_CONTROL_SOLUTION             10

//! glucose measurement sample location
//! Higher nibble of type_sample_location
#define GLUCOSE_SAMPLE_LOC_FINGER                 (1 << 4)
#define GLUCOSE_SAMPLE_LOC_AST                    (2 << 4)
#define GLUCOSE_SAMPLE_LOC_EARLOBE                (3 << 4)
#define GLUCOSE_SAMPLE_LOC_CONTROL_SOLUTION       (4 << 4)
#define GLUCOSE_SAMPLE_LOC_NOT_AVAILABLE          (15 << 4)

//! glucose measurement context flags
#define GLUCOSE_CONTEXT_FLAG_CARBOHYD             0
#define GLUCOSE_CONTEXT_FLAG_MEAL                 1
#define GLUCOSE_CONTEXT_FLAG_TESTER_HEALTH        2
#define GLUCOSE_CONTEXT_FLAG_EXECISE              3
#define GLUCOSE_CONTEXT_FLAG_MEDICATION           4
#define GLUCOSE_CONTEXT_FLAG_MEDI_UNIT            5
#define GLUCOSE_CONTEXT_FLAG_HBA1C                6
#define GLUCOSE_CONTEXT_FLAG_EXTENDED             7
#define GLUCOSE_CONTEXT_FLAG_ALL                  0xFFF

//! glucose measurement contex tester
//! lower nibble of tester_health
#define GLUCOSE_TESTER_SELF                       1
#define GLUCOSE_TESTER_PROFESSIONAL               2
#define GLUCOSE_TESTER_LAB_TEST                   3
#define GLUCOSE_TESTER_VAL_NOT_AVAILABLE          15

//! glucose measurement contex health
//! Higher nibble of tester_health
#define GLUCOSE_HEALTH_MINOR_ISSUES               (1 << 4)
#define GLUCOSE_HEALTH_MAJOR_ISSUES               (2 << 4)
#define GLUCOSE_HEALTH_DURING_MENSES              (3 << 4)
#define GLUCOSE_HEALTH_UNDER_STRESS               (4 << 4)
#define GLUCOSE_HEALTH_NO_ISSUES                  (5 << 4)
#define GLUCOSE_HEALTH_VAL_NOT_AVAILABLE          (15 << 4)

//! glucose measurement contex meal
#define GLUCOSE_MEAL_PREPANDIAL_BEFORE            1
#define GLUCOSE_MEAL_PREPANDIAL_AFTER             2
#define GLUCOSE_MEAL_FASTING                      3
#define GLUCOSE_MEAL_CASUAL                       4
#define GLUCOSE_MEAL_BEDTIME                      5

//! glucose measurement context carbohydrate ID
#define GLUCOSE_CARBOHDY_ID_BREAKFAST             1
#define GLUCOSE_CARBOHDY_ID_LUNCH                 2
#define GLUCOSE_CARBOHDY_ID_DINNER                3
#define GLUCOSE_CARBOHDY_ID_SNACK                 4
#define GLUCOSE_CARBOHDY_ID_DRINK                 5
#define GLUCOSE_CARBOHDY_ID_SUPPER                6
#define GLUCOSE_CARBOHDY_ID_BRUNCH                7

#define GLUCOSE_CTRL_POINT_MAX_SIZE               20

//! glucose control point opcode
#define GLUCOSE_OPCODE_REPORT_REC                 1
#define GLUCOSE_OPCODE_DELETE_REC                 2
#define GLUCOSE_OPCODE_ABORT_OPERATOR             3
#define GLUCOSE_OPCODE_REPORT_NUM_REC             4
#define GLUCOSE_OPCODE_RESPONSE_NUM_REC           5
#define GLUCOSE_OPCODE_RESPONSE_CODE              6

//! glucose control point opcode
#define GLUCOSE_OPERATOR_NULL                     0
#define GLUCOSE_OPERATOR_ALL_REC                  1
#define GLUCOSE_OPERATOR_LESS_EQU                 2
#define GLUCOSE_OPERATOR_GREATER_EQU              3
#define GLUCOSE_OPERATOR_WITHIN_RANGE             4
#define GLUCOSE_OPERATOR_FIRST_REC                5
#define GLUCOSE_OPERATOR_LAST_REC                 6

//! glucose control point response code value
#define GLUCOSE_RSP_SUCCESS                       1
#define GLUCOSE_RSP_OPCODE_NOT_SUPPORT            2
#define GLUCOSE_RSP_INVALID_OPERATOR              3
#define GLUCOSE_RSP_OPERATOR_NOT_SUPPORT          4
#define GLUCOSE_RSP_INVALID_OPERAND               5
#define GLUCOSE_RSP_NO_RECORD_FOUND               6
#define GLUCOSE_RSP_ABORT_UNSUCCESSFUL            7
#define GLUCOSE_RSP_PROCUDURE_NOT_COMPLETED       8
#define GLUCOSE_RSP_OPERAND_NOT_SUPPORT           9

//! glucose control point filter type 
#define GLUCOSE_FILTER_TYPE_SEQ_NUM               1
#define GLUCOSE_FILTER_TYPE_TIME                  2

//! for date time calculation
#define MAX_VALID_YR                              9999
#define MIN_VALID_YR                              1800
#define DT1_EQUALS_DT2                            0
#define DT1_GRETER_THAN_DT2                       1
#define DT1_LESS_THAN_DT2                         -1
#define DT1_DT2_INVALID                           -2

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
#define SFLOAT_EXP_TEST_RANGE                     0x0008

#define MITM_REQ                                  0x01

//! enum for SFLOAT convertion
typedef enum {
  SFLOAT_POSITIVE_INFINITY = 0x07FE,
  SFLOAT_NaN =               0x07FF,
  SFLOAT_NRes =              0x0800,
  SFLOAT_RESERVED_VALUE =    0x0801,
  SFLOAT_NEGATIVE_INFINITY = 0x0802
} ReservedSFloatValues;

/* date time structure
 * represented using Gregorian calendar
 */
typedef struct date_time_s
{
  uint16_t year;             //1582-9999
  uint8_t month;             //0-12, 1-jan
  uint8_t day;               //1-31
  uint8_t hour;              //0-23
  uint8_t minute;            //0-59
  uint8_t second;            //0-59
}date_time_t;

//! enum for record state
typedef enum {
  GLUCOSE_RECORD_STATE_VALID,
  GLUCOSE_RECORD_STATE_FILTERED,
  GLUCOSE_RECORD_MEASUREMENT_STATE_SEND,
  GLUCOSE_RECORD_CONTEXT_STATE_SEND
} record_state_t;

//! glucose measurement structure
typedef struct glucose_meas_value_s
{
  record_state_t state;
  uint8_t flags;
  uint16_t seq_num;
  date_time_t base_time;
  uint16_t time_offset;
  float concentration;
  /* The Type nibble and the Sample Location nibble comprise one octet.
   * Therefore, when one nibble is present,
   * both nibbles shall be present.*/
  uint8_t type_sample_loc;
  // Sensor status annunciation
  uint16_t sansor_status_ann;
}glucose_meas_t;

//! glucose context structure
typedef struct glucose_context_value_s
{
  uint8_t flags;
  uint16_t seq_num;
  uint8_t extended_flags;
  uint8_t carbohyd_id;
  float carbohyd_val;
  uint8_t meal;
  /* The Tester nibble and the health nibble comprise one octet.
   * Therefore, when one nibble is present,
   * both nibbles shall be present.*/
  uint8_t tester_health;
  uint16_t exercise_duration;
  uint8_t exercise_intensity;
  uint8_t medication_id;
  float medication_val;
  // haemoglobin A1c
  float hba1c;
}glucose_context_t;

//! glucose context structure
typedef struct ctrl_point_cmd_s
{
  uint8_t len;
  uint8_t opcode;
  uint8_t operator;
  uint8_t filter_type;
  void* param1;
  void* param2;
} glucose_ctrl_point_t;

//! function prototype
static void rsi_ble_add_char_serv_att (void *serv_handler, uint16_t handle,
    uint8_t val_prop, uint16_t att_val_handle, uuid_t att_val_uuid);
static void rsi_ble_add_char_val_att (void *serv_handler, uint16_t handle, uuid_t att_type_uuid,
    uint8_t  val_prop, uint8_t *data, uint8_t  data_len);
static uint32_t rsi_ble_add_glucose_serv (void);
static bool is_leap(int16_t year);
static bool is_valid_date_time(date_time_t dt);
static int8_t compare_date_time (date_time_t dt1, date_time_t dt2);
static void rsi_ble_ctrl_point_send_rsp_code(uint8_t rsp_code, uint8_t opcode);
static bool set_first_valid_record(void);
static bool set_last_valid_record(void);
static bool set_all_valid_record(bool set_bit);
static bool set_within_range_valid_record(uint8_t filter_type, uint16_t* p_param1, uint16_t* p_param2);
static bool set_less_equal_valid_record(uint8_t filter_type, void* p_param);
static bool set_greater_equal_valid_record(uint8_t filter_type, uint16_t* p_param);
static bool rsi_ble_ctrl_point_handle_operator(glucose_ctrl_point_t* p_ctrl_point_cmd);
static uint16_t get_number_of_valid_record(void);
static void rsi_ble_ctrl_point_send_num_record_rsp(uint16_t num_rec);
static void rsi_ble_ctrl_point_handle_cmd(glucose_ctrl_point_t* p_ctrl_point_cmd);
static bool rsi_ble_ctrl_point_opcode_check(glucose_ctrl_point_t* ctrl_point_cmd);
static void rsi_ble_ctrl_point_data_process(uint8_t len, uint8_t *p_data);
static uint8_t ctrl_point_next_filtered_index(uint8_t curr_index);
static uint8_t glucose_meas_fill_value(glucose_meas_t val, uint8_t *p_data);
static uint8_t glucose_context_fill_value(glucose_context_t val, uint8_t *p_data);
static void rsi_ble_glucose_server_handle_write_event(void);
//! function prototype
static uint8_t glucose_ctrl_point_fill_value(glucose_ctrl_point_t, uint8_t *);

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;
static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;
static rsi_ble_event_write_t app_ble_write_event;
static uint16_t rsi_ble_glucose_val_hndl;
static uint16_t rsi_ble_glucose_context_val_hndl;
static uint16_t rsi_ble_ctrl_point_val_hndl;
static const double reserved_float_values[5] = {INFINITY, NAN, NAN, NAN, -INFINITY};
static uint8_t str_remote_address[18];
static uint8_t remote_addr_type=0;
static uint8_t remote_name[31];
static uint8_t remote_dev_addr[18] = {0};
static uint8_t remote_dev_bd_addr[6] = {0};
static uint8_t device_found=0;


uint16_t seq_param1 = 2;
date_time_t time_param1 = {2017,9,28,3,5,0};
date_time_t time_param2 = {2017,9,28,4,5,0};

//! use any one input for testing diffrent commands
glucose_ctrl_point_t glucose_ctrl_point_val = {
  0,
  GLUCOSE_OPCODE_REPORT_REC,
  GLUCOSE_OPERATOR_WITHIN_RANGE,
  GLUCOSE_FILTER_TYPE_TIME,
  &time_param1,
  &time_param2
  
  //! testing one cmd at a time
  /*0,
  GLUCOSE_OPCODE_REPORT_REC,
  GLUCOSE_OPERATOR_GREATER_EQU,
  GLUCOSE_FILTER_TYPE_TIME,
  &time_param1,
  NULL*/
  
  /*0,
  GLUCOSE_OPCODE_REPORT_REC,
  GLUCOSE_OPERATOR_LESS_EQU,
  GLUCOSE_FILTER_TYPE_TIME,
  &time_param1,
  NULL*/
  
  /*0,
  GLUCOSE_OPCODE_REPORT_REC,
  GLUCOSE_OPERATOR_LESS_EQU,
  GLUCOSE_FILTER_TYPE_SEQ_NUM,
  &seq_param1,
  NULL*/

  /*0,
  GLUCOSE_OPCODE_REPORT_REC,
  GLUCOSE_OPERATOR_ALL_REC,
  0,
  NULL,
  NULL*/
};

#if(GATT_ROLE == SERVER)
//! enum for global state
typedef enum {
  GLUCOSE_MEASUREMENT_NOTIFY_ENABLE,
  GLUCOSE_CONTEXT_NOTIFY_ENABLE,
  GLUCOSE_CTRL_POINT_INDICATE_ENABLE,
  CONNECTED,
  ENCRYPT_EN,
  ADVERTISE,
  SCAN
} app_state_t;

app_state_t app_state ;

glucose_meas_t glucose_meas_val[GLUCOSE_MAX_RECORD] =
{
  {
    // state
    (record_state_t)(1 << GLUCOSE_RECORD_STATE_VALID),
    // flags
    GLUCOSE_MEAS_FLAG_TYPE_SAMPLE_LOC | GLUCOSE_MEAS_FLAG_CONCENTRATION,
    // sequence number
    1,
    // base time
    {2017,9,28,2,15,0},
    // time offset
    0,
    // concentration 5.5 mmol/L -> 0.0055
    0.0055,
    // type and sample location
    GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD | GLUCOSE_SAMPLE_LOC_FINGER,
    // sensor status annuciation
    0
  },
  
  {
    // state
    (1 << GLUCOSE_RECORD_STATE_VALID),
    // flags
    GLUCOSE_MEAS_FLAG_TYPE_SAMPLE_LOC | GLUCOSE_MEAS_FLAG_CONCENTRATION,
    // sequence number
    2,
    // base time
    {2017,9,28,3,15,0},
    // time offset
    0,
    // concentration 6.1 mmol/L -> 0.0061
    0.0061,
    // type and sample location
    GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD | GLUCOSE_SAMPLE_LOC_FINGER,
    // sensor status annuciation
    0
  },
  
  {
    // state
    (1 << GLUCOSE_RECORD_STATE_VALID),
    // flags
    GLUCOSE_MEAS_FLAG_TYPE_SAMPLE_LOC | GLUCOSE_MEAS_FLAG_CONCENTRATION,
    // sequence number
    3,
    // base time
    {2017,9,28,4,15,0},
    // time offset
    0,
    // concentration 5 mmol/L -> 0.005
    0.005,
    // type and sample location
    GLUCOSE_TYPE_CAPILLARY_WHOLE_BLOOD | GLUCOSE_SAMPLE_LOC_FINGER,
    // sensor status annuciation
    0
  }
};

glucose_context_t glucose_context_val[GLUCOSE_MAX_RECORD] =
{
  {
    // flags
    (uint8_t)(GLUCOSE_CONTEXT_FLAG_ALL & ~(GLUCOSE_CONTEXT_FLAG_EXTENDED | GLUCOSE_CONTEXT_FLAG_MEDICATION)),
    // sequence number
    1,
    // extened flag
    0,
    // carbohyd ID 
    3,
    // carbohydrate value
    1.0,  
    // meal
    1,
    // tester health
    GLUCOSE_TESTER_SELF | GLUCOSE_HEALTH_NO_ISSUES,
    // exercise duration (in second)
    1800,
    // exercise intensity (in percentage)
    70,
    // medication ID
    1,
    // medication value
    100,
    // HbA1c % 
    50 
  },

  {
    // flags
    0x00,
    // sequence number
    2,
    // extened flag
    0,
    // carbohyd ID 
    0,
    // carbohydrate value
    10,  
    // meal
    0,
    // tester health
    0,
    // exercise duration (in second)
    1800,
    // exercise intensity (in percentage)
    70,
    // medication ID
    0,
    // medication value
    100,
    // HbA1c (in percentage)
    6.0 
  },

  {
    // flags
    0x00,
    // sequence number
    3,
    // extened flag
    0,
    // carbohyd ID 
    0,
    // carbohydrate value
    10,  
    // meal
    0,
    // tester health
    0,
    // exercise duration (in second)
    1800,
    // exercise intensity (in percentage)
    70,
    // medication ID
    0,
    // medication value
    100,
    // HbA1c (in percentage)
    6.4
  }
};

uint16_t glucose_meas_index = 0;

#elif(GATT_ROLE == CLIENT)
//! enum for global state
typedef enum {
  CONNECTED,
  ENCRYPT_EN,
  ADVERTISE,
  SCAN
} app_state_t;

//! function prototype
static void rsi_ble_glucose_client_handle_write_event(void);

app_state_t app_state = 0;

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
  if(((device_found==0) && ((strcmp((const char *)remote_name, RSI_REMOTE_DEVICE_NAME))==0) )||
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
 * this is a callback function
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
 * @fn         rsi_ble_on_smp_request 
 * @brief      its invoked when smp request event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP request events is received(we are in Master mode)
 * Note: slave requested to start SMP request, we have to send SMP request command
 */
static void rsi_ble_on_smp_request (rsi_bt_event_smp_req_t  *remote_dev_address)
{
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_req - str_remote_address : %s",
      rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
  rsi_ble_app_set_event (RSI_BLE_SMP_REQ_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_response 
 * @brief      its invoked when smp response event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP response events is received(we are in slave mode) 
 * Note: Master initiated SMP protocol, we have to send SMP response command
 */
static void rsi_ble_on_smp_response (rsi_bt_event_smp_resp_t  *remote_dev_address)
{
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_resp - str_remote_address : %s",
    rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
  rsi_ble_app_set_event (RSI_BLE_SMP_RESP_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_passkey 
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
static void rsi_ble_on_smp_passkey (rsi_bt_event_smp_passkey_t  *remote_dev_address)
{
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_passkey - str_remote_address : %s",
    rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
  rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_failed 
 * @brief      its invoked when smp failed event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP failed events is received
 */
static void rsi_ble_on_smp_failed (uint16_t  status, rsi_bt_event_smp_failed_t  *remote_dev_address)
{
  memcpy (remote_dev_bd_addr, remote_dev_address->dev_addr, 6);
  LOG_PRINT ("smp_failed status: 0x%x, str_remote_address: %s", status,
    rsi_6byte_dev_address_to_ascii (str_remote_address, remote_dev_address->dev_addr));
  rsi_ble_app_set_event (RSI_BLE_SMP_FAILED_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_encrypt_started 
 * @brief      its invoked when encryption started event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when encryption started events is received
 */
static void rsi_ble_on_encrypt_started (uint16_t  status)
{
  LOG_PRINT ("start encrypt status: %d", status);
  app_state |= (1 << ENCRYPT_EN);
  rsi_ble_app_set_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_passkey_display 
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
static void rsi_ble_on_smp_passkey_display (rsi_bt_event_smp_passkey_display_t *smp_passkey_display)
{
  LOG_PRINT ("smp_passkey: %s ", smp_passkey_display->passkey );
  rsi_ble_app_set_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_le_ltk_req_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates linkkey request information
 */
static void rsi_ble_on_le_ltk_req_event(rsi_bt_event_le_ltk_request_t *le_ltk_req)
{
  LOG_PRINT (" \r\n rsi_ble_on_le_ltk_req_event \r\n");
  memcpy (remote_dev_bd_addr, le_ltk_req->dev_addr, 6);
  rsi_ble_app_set_event (RSI_BLE_LTK_REQ_EVENT);
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
  //! in sfloat, 4bits is for exponent i.e. 0xemmm
  int8_t exponent = 0;
  //! in sfloat, 12bits is for mantissa i.e. 0xemmm
  double mantissa_f;
  uint32_t mantissa;
  uint16_t result = SFLOAT_NaN;
  double sgn = data > 0 ? +1 : -1;
  double smantissa,rmantissa,mdiff;

  //! check for nan (not a number)
  if (isnan(data)) 
  {
    goto end;
  }
  else if ((double)data > SFLOAT_MAX) 
  {
    result = SFLOAT_POSITIVE_INFINITY;
    goto end;
  }
  else if (data < SFLOAT_MIN) 
  {
    result = SFLOAT_NEGATIVE_INFINITY;
    goto end;
  }
  else if (data >= -SFLOAT_EPSILON &&
      (double)data <= SFLOAT_EPSILON)
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
  //! in sfloat, 12bits is for mantissa i.e. 0xemmm
  uint16_t mantissa = data & 0x0fff;
  //! in sfloat, 4bit is for exponent i.e. 0xemmm
  int8_t exponent = data >> 12;
  double magnitude = 0;
  //! exponent range is 7 to -8
  if (exponent >= SFLOAT_EXP_TEST_RANGE)
  {
    exponent = -((0x000f + 1) - exponent);
  }

  //! mantissa range sfloat_negative_infinity to sfloat_positive_infinity
  if (mantissa >= SFLOAT_POSITIVE_INFINITY && mantissa<= SFLOAT_NEGATIVE_INFINITY) 
  {
    output = reserved_float_values[mantissa - SFLOAT_POSITIVE_INFINITY];
  }
  else
  {
    //! checking resolution sfloat_nres(not at this resolution)
    if (mantissa >= SFLOAT_NRes)
    {
      mantissa = -((0x0fff + 1) - mantissa);
    }
    //! exponent is in base 10 i.e. 10**x
    magnitude = pow(10.0f, exponent);
    output = (mantissa * magnitude);
  }
  return output;
}

#if(GATT_ROLE == SERVER)
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

/*==============================================*/
/**
 * @fn         rsi_ble_simple_chat_add_new_serv
 * @brief      this function is used to add new servcie i.e.., glucose service.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */

static uint32_t rsi_ble_add_glucose_serv (void)
{
  uint8_t len = 0;
  uuid_t  new_uuid = {0};
  uint8_t   data[25] = {0};
  uint16_t  feature = 0;
  rsi_ble_resp_add_serv_t  new_serv_resp = {0};

  //! adding new service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_GLUCOSE_SERVICE_UUID;
  rsi_ble_add_service (new_uuid, &new_serv_resp);

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_GLUCOSE_MEASUREMENT_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 1,
      RSI_BLE_ATT_PROPERTY_NOTIFY,
      new_serv_resp.start_handle + 2,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_glucose_val_hndl = new_serv_resp.start_handle + 2;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_GLUCOSE_MEASUREMENT_UUID;
  len = glucose_meas_fill_value(glucose_meas_val[0], data);
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 2,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_NOTIFY,
      (uint8_t *)&data,
      len);
/*
  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_GLUCOSE_MEASUREMENT_CONTEXT_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 4,
      RSI_BLE_ATT_PROPERTY_NOTIFY,
      new_serv_resp.start_handle + 5,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_glucose_context_val_hndl = new_serv_resp.start_handle + 5;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_GLUCOSE_MEASUREMENT_CONTEXT_UUID;
  len = glucose_context_fill_value(glucose_context_val[0], data);
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 5,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_NOTIFY,
      (uint8_t *)&data,
      len);
*/
  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_GLUCOSE_FEATURE_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 4,//7,
      RSI_BLE_ATT_PROPERTY_READ,
      new_serv_resp.start_handle + 5,//8,
      new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_GLUCOSE_FEATURE_UUID;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 5,//8,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_READ,
      (uint8_t *)&feature,
      sizeof (feature));

  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_RECORD_ACCESS_CONTROL_POINT_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 6,//7,//9,
      RSI_BLE_ATT_PROPERTY_INDICATE | RSI_BLE_ATT_PROPERTY_WRITE,
      new_serv_resp.start_handle + 7,//8,//10,
      new_uuid);

  //! adding characteristic value attribute to the service
  rsi_ble_ctrl_point_val_hndl = new_serv_resp.start_handle + 7;//8;//10;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_RECORD_ACCESS_CONTROL_POINT_UUID;
  //len = glucose_ctrl_point_fill_value(glucose_ctrl_point_val, data);
  
  len = 0;
  rsi_ble_add_char_val_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + 7,//8,//10,
      new_uuid,
      RSI_BLE_ATT_PROPERTY_INDICATE | RSI_BLE_ATT_PROPERTY_WRITE,
      (uint8_t *)&data,
      sizeof(data));
return 0;
}

/*==============================================*/
/**
 * @fn         is_leap
 * @brief      leap year check.
 * @param[in]  int16_t year.
 * @return     bool
 *             0  =  leap year
 *             !0 =  not a leap year
 * @section description
 * This function is used returns true if given year is valid.
 */

static bool is_leap(int16_t year)
{
// Return true if year is a multiple pf 4 and
// not multiple of 100.
// OR year is multiple of 400.
return (((year%4==0) && (year%100!=0)) ||
		(year%400==0));
}

/*==============================================*/
/**
 * @fn         rsi_ble_simple_chat_add_new_serv
 * @brief      check date time validity
 * @param[in]  date_time_t dt
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used returns true if given date time is valid else false.
 */
static bool is_valid_date_time(date_time_t dt)
{
	// If date time are not in given range
	if (dt.year > MAX_VALID_YR || dt.year < MIN_VALID_YR)
  {
    return false;
  }
  if (dt.month < 1 || dt.month > 12)
  {
    return false;
  }
  if (dt.day <= 1 || dt.day > 31)
  {
    return false;
  }
  if (dt.hour <= 1 || dt.hour > 23)
  {
    return false;
  }
	if (dt.minute <= 1 || dt.minute > 59)
  {
    return false;
  }
  if (dt.second <= 1 || dt.second > 59)
  {
    return false;
  }

	// Handle February month with leap year
	if (dt.month == 2)
	{
		if (is_leap(dt.year))
		return (dt.day <= 29);
		else
		return (dt.day <= 28);
	}

	// Months of April, June, Sept and Nov
	// must have number of days less than
	// or equal to 30.
	if (dt.month==4 || dt.month==6 || dt.month==9 || dt.month==11)
		return (dt.day <= 30);

	return true;
}

/*==============================================*/
/**
 * @fn         compare_date_time
 * @brief      compare given two date time parameter.
 * @param[in]  date_time_t dt1
 * @param[in]  date_time_t dt2
 * @return     int8_t
 *             DT1_EQUALS_DT2 - both date_times are equal
 *             DT1_GRETER_THAN_DT2 - first one is greater than second
 *             DT1_LESS_THAN_DT2 - first one is less than second
 *             DT1_DT2_INVALID - both date_times are invalid
 * @section description
 * This function is used compare given two date time parameter.
 */
static int8_t compare_date_time (date_time_t dt1, date_time_t dt2)
{
  if(!is_valid_date_time(dt1))
  {
    return DT1_DT2_INVALID;
  }

  if(!is_valid_date_time(dt2))
  {
    return DT1_DT2_INVALID;
  }
  
  if (dt1.year < dt2.year)
  {
    return DT1_LESS_THAN_DT2;
  }
  else if (dt1.year > dt2.year)
  {
    return DT1_GRETER_THAN_DT2;
  }
  else if (dt1.month < dt2.month)
  {
    return DT1_LESS_THAN_DT2;
  }
  else if (dt1.month > dt2.month)
  {
    return DT1_GRETER_THAN_DT2;
  }
  else if (dt1.day < dt2.day)
  {
    return DT1_LESS_THAN_DT2;
  }
  else if(dt1.day > dt2.day)
  {
    return DT1_GRETER_THAN_DT2;
  }
  else if (dt1.hour < dt2.hour)
  {
    return DT1_LESS_THAN_DT2;
  }
  else if(dt1.hour > dt2.hour)
  {
    return DT1_GRETER_THAN_DT2;
  }
  else if (dt1.minute < dt2.minute)
  {
    return DT1_LESS_THAN_DT2;
  }
  else if(dt1.minute > dt2.minute)
  {
    return DT1_GRETER_THAN_DT2;
  }
  else if (dt1.second < dt2.second)
  {
    return DT1_LESS_THAN_DT2;
  }
  else if(dt1.second > dt2.second)
  {
    return DT1_GRETER_THAN_DT2;
  }
  else
    return DT1_EQUALS_DT2;
}

/*==============================================*/
/**
 * @fn         rsi_ble_ctrl_point_send_rsp_code
 * @brief      send response to client.
 * @param[in]  uint8_t    rsp_code, response code 
 * @param[in]  uint8_t    opcode
 * @return     none.
 * @section description
 * this function is used to send response code.
 */
static void rsi_ble_ctrl_point_send_rsp_code(uint8_t rsp_code, uint8_t opcode)
{
  if(app_state & (1 << CONNECTED))
  {
    if(app_state & (1 << GLUCOSE_CTRL_POINT_INDICATE_ENABLE))
    {
      uint8_t data[4] = {0};
      uint8_t len = 4;
      data[0] = GLUCOSE_OPCODE_RESPONSE_CODE;
      data[1] = GLUCOSE_OPERATOR_NULL;
      data[2] = opcode;
      data[3] = rsp_code;

      rsi_ble_set_local_att_value (rsi_ble_ctrl_point_val_hndl, len, data);
    }
  }
  LOG_PRINT("rsi_ble_ctrl_point_send_rsp_code\n");
}

/*==============================================*/
/**
 * @fn         set_first_valid_record
 * @brief      Set the state in the first valid record
 * @param[ ]   none
 * @return     bool - true: found record,
 *                    false: not found
 * @section description
 * this function is used set the bit 1 - glucose_record_state_filtered
 * (first valid record) of record state field.
 */
static bool set_first_valid_record(void)
{
  uint8_t i;
  for(i = 0; i < GLUCOSE_MAX_RECORD; i++)
  {
    if(glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_VALID))
    {
      glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
      glucose_meas_index = i;
      return true;
    }
  }
  return false;
}

/*==============================================*/
/**
 * @fn         set_last_valid_record
 * @brief      Set the state in the last valid record
 * @param[ ]   none
 * @return     bool - true: found record,
 *                    false: not found
 * @section description
 * this function is used set the bit 1 - glucose_record_state_filtered
 * (last valid record) of record state field.
 */
static bool set_last_valid_record()
{
  int8_t i;
  for(i = (GLUCOSE_MAX_RECORD-1); i >= 0; i--)
  {
    if(glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_VALID))
    {
      glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
      glucose_meas_index = i;
     
      return true;
    }
  }
  return false;
}

/*==============================================*/
/**
 * @fn         set_all_valid_record
 * @brief      Set/Clear the state in the all valid record
 * @param[in]  bool - set_bit, 1: set, 0:clear
 * @return     bool - true: found record,
 *                    false: not found
 * @section description
 * this function is used set/clear the bit 1 - glucose_record_state_filtered
 * (all valid record) of record state field.
 */
static bool set_all_valid_record(bool set_bit)
{
  bool ret = false;
  uint8_t i;
  //! initialize with invalid index
  uint8_t start_index = GLUCOSE_MAX_RECORD;
  for(i = 0; i < GLUCOSE_MAX_RECORD; i++)
  {
    if(glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_VALID))
    {
      if(set_bit)
      {
        glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
      }
      else
      {
        glucose_meas_val[i].state &= ~(1 << GLUCOSE_RECORD_STATE_FILTERED);
      }
      if(GLUCOSE_MAX_RECORD == start_index)
      {
        start_index = i;
      }
      ret = true;
    }
  }
  
  //! update valid start index
  if (GLUCOSE_MAX_RECORD != start_index)
  {
    glucose_meas_index = start_index;
  }
  else
  {
    glucose_meas_index = 0;
  }

  return ret;
}

/*==============================================*/
/**
 * @fn         set_within_range_valid_record
 * @brief      Set the state in the all valid record,
 *             which are within the range of passed parameters
 * @param[in]  uint8_t   - filter_type: seq_num/time
 * @param[in]  uint16_t* - pointer to parameter 1
 * @param[in]  uint16_t* - pointer to parameter 2
 * @return     bool - true: found record,
 *                    false: not found
 * @section description
 * this function is used to set the bit 1 - glucose_record_state_filtered
 * (all valid record greater then equals to passed parameter) of record state field.
 */
static bool set_within_range_valid_record(uint8_t filter_type, uint16_t* p_param1, uint16_t* p_param2)
{
  bool ret = false;
  int8_t cmp_time1_ret;
  int8_t cmp_time2_ret;
  uint8_t i;
  //! initialize with invalid index
  uint8_t start_index = GLUCOSE_MAX_RECORD;
  uint16_t seq_param1;
  uint16_t seq_param2;
  date_time_t time_param1;
  date_time_t time_param2;

  if (GLUCOSE_FILTER_TYPE_SEQ_NUM == filter_type)
  {
    seq_param1 = *((uint16_t *)p_param1);
    seq_param2 = *((uint16_t *)p_param2);
  }
  else if (GLUCOSE_FILTER_TYPE_TIME == filter_type)
  {
    time_param1 = *((date_time_t *)p_param1);
    time_param2 = *((date_time_t *)p_param2);
  }

  for(i = 0; i < GLUCOSE_MAX_RECORD; i++)
  {
    if(glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_VALID))
    {
      switch (filter_type)
      {
        case GLUCOSE_FILTER_TYPE_SEQ_NUM:
          if((glucose_meas_val[i].seq_num <= seq_param2) && (glucose_meas_val[i].seq_num >= seq_param1))
          {
            glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
            if(GLUCOSE_MAX_RECORD == start_index)
            {
              start_index = i;
            }
            ret = true;
          }
          break;

        case GLUCOSE_FILTER_TYPE_TIME:
            cmp_time1_ret = compare_date_time(glucose_meas_val[i].base_time, time_param1);
            cmp_time2_ret = compare_date_time(glucose_meas_val[i].base_time, time_param2);
            // param time 1 >= glucose record time is <= param time 2
            if((cmp_time1_ret == DT1_EQUALS_DT2 && cmp_time2_ret == DT1_EQUALS_DT2) ||
                (cmp_time1_ret == DT1_EQUALS_DT2 && cmp_time2_ret == DT1_LESS_THAN_DT2) ||
                (cmp_time1_ret == DT1_GRETER_THAN_DT2 && cmp_time2_ret == DT1_EQUALS_DT2) ||
                (cmp_time1_ret == DT1_GRETER_THAN_DT2 && cmp_time2_ret == DT1_LESS_THAN_DT2))
            {
              glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
              if(GLUCOSE_MAX_RECORD == start_index)
              {
                start_index = i;
              }
              ret = true;
            }
          break;
      }
    }
  }
  
  //! update valid start index
  if (GLUCOSE_MAX_RECORD != start_index)
  {
    glucose_meas_index = start_index;
  }
  else
  {
    glucose_meas_index = 0;
  }

  return ret;
}

/*==============================================*/
/**
 * @fn         set_less_eaual_valid_record
 * @brief      Set the state in the all valid record,
 *             which are less than and equals to passed param
 * @param[in]  uint8_t   - filter_type: seq_num/time
 * @param[in]  uint16_t* - pointer to parameter 1
 * @return     bool - true: found record,
 *                    false: not found
 * @section description
 * this function is used to set the bit 1 - glucose_record_state_filtered
 * (all valid record less then equals to passed parameter) of record state field.
 */
static bool set_less_equal_valid_record(uint8_t filter_type, void* p_param)
{
  bool ret = false;
  int8_t cmp_time_ret;
  uint8_t i;
  //! initialize with invalid index
  uint8_t start_index = GLUCOSE_MAX_RECORD;
  uint16_t seq_param;
  date_time_t time_param;

  if (GLUCOSE_FILTER_TYPE_SEQ_NUM == filter_type)
  {
    seq_param = *((uint16_t *)p_param);
  }
  else if (GLUCOSE_FILTER_TYPE_TIME == filter_type)
  {
    time_param = *((date_time_t *)p_param);
  }

  for(i = 0; i < GLUCOSE_MAX_RECORD; i++)
  {
    if(glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_VALID))
    {
      switch (filter_type)
      {
        case GLUCOSE_FILTER_TYPE_SEQ_NUM:
          if(glucose_meas_val[i].seq_num <= seq_param)
          {
            glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
            if(GLUCOSE_MAX_RECORD == start_index)
            {
              start_index = i;
            }
            ret = true;
          }
          break;

        case GLUCOSE_FILTER_TYPE_TIME:
            cmp_time_ret = compare_date_time(glucose_meas_val[i].base_time, time_param);
            switch (cmp_time_ret)
            {
              // glucose record time is <= param time
              case DT1_LESS_THAN_DT2:
              case DT1_EQUALS_DT2:
                glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
                if(GLUCOSE_MAX_RECORD == start_index)
                {
                  start_index = i;
                }
                ret = true;
                break;
              
              // glucose record time is > param time
              case DT1_GRETER_THAN_DT2:
                break;

              // glucose record time or param time is invalid
              case DT1_DT2_INVALID:
                break;

            }
          break;
      }
    }
  }
  
  //! update valid start index
  if (GLUCOSE_MAX_RECORD != start_index)
  {
    glucose_meas_index = start_index;
  }
  else
  {
    glucose_meas_index = 0;
  }

  return ret;
}

/*==============================================*/
/**
 * @fn         set_greater_eaual_valid_record
 * @brief      Set the state in the all valid record,
 *             which are greater than and equals to passed param
 * @param[in]  uint8_t   - filter_type: seq_num/time
 * @param[in]  uint16_t* - pointer to parameter 1
 * @return     bool - true: found record,
 *                    false: not found
 * @section description
 * this function is used to set the bit 1 - glucose_record_state_filtered
 * (all valid record greater then equals to passed parameter) of record state field.
 */
static bool set_greater_equal_valid_record(uint8_t filter_type, uint16_t* p_param)
{
  bool ret = false;
  int8_t cmp_time_ret;
  uint8_t i;
  //! initialize with invalid index
  uint8_t start_index = GLUCOSE_MAX_RECORD;
  uint16_t seq_param;
  date_time_t time_param;

  if (GLUCOSE_FILTER_TYPE_SEQ_NUM == filter_type)
  {
    seq_param = *((uint16_t *)p_param);
  }
  else if (GLUCOSE_FILTER_TYPE_TIME == filter_type)
  {
    time_param = *((date_time_t *)p_param);
  }
  
  for(i = 0; i < GLUCOSE_MAX_RECORD; i++)
  {
    if(glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_VALID))
    {
      switch (filter_type)
      {
        case GLUCOSE_FILTER_TYPE_SEQ_NUM:
          if(glucose_meas_val[i].seq_num >= seq_param)
          {
            glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
            if(GLUCOSE_MAX_RECORD == start_index)
            {
              start_index = i;
            }
            ret = true;
          }
          break;

        case GLUCOSE_FILTER_TYPE_TIME:
            cmp_time_ret = compare_date_time(glucose_meas_val[i].base_time, time_param);
            switch (cmp_time_ret)
            {
              // glucose record time is >= param time
              case DT1_GRETER_THAN_DT2:
              case DT1_EQUALS_DT2:
                glucose_meas_val[i].state |= (1 << GLUCOSE_RECORD_STATE_FILTERED);
                if(GLUCOSE_MAX_RECORD == start_index)
                {
                  start_index = i;
                }
                ret = true;
                break;
              
              // glucose record time is < param time
              case DT1_LESS_THAN_DT2:
                break;

              // glucose record time or param time is invalid
              case DT1_DT2_INVALID:
                break;

            }
          break;
      }
    }
  }
  
  //! update valid start index
  if (GLUCOSE_MAX_RECORD != start_index)
  {
    glucose_meas_index = start_index;
  }
  else
  {
    glucose_meas_index = 0;
  }

  return ret;
}

/*==============================================*/
/**
 * @fn         rsi_ble_ctrl_point_handle_operator
 * @brief      handle control point command (operator)
 * @param[in]  glucose_ctrl_point_t* - pointer to control point structure
 * @return     bool - true if response is handaled else false
 * @section description
 * this function is used to process ctrl point message (handling operator).
 */
static bool rsi_ble_ctrl_point_handle_operator(glucose_ctrl_point_t* p_ctrl_point_cmd)
{
  bool ret = false;

  switch(p_ctrl_point_cmd->operator)
  {
    case GLUCOSE_OPERATOR_NULL:
      rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_OPERATOR_NOT_SUPPORT, p_ctrl_point_cmd->opcode);
      ret = true;
      break;

    case GLUCOSE_OPERATOR_ALL_REC:
        ret = set_all_valid_record(true);
      break;

    case GLUCOSE_OPERATOR_FIRST_REC:
        ret = set_first_valid_record();
      break;

    case GLUCOSE_OPERATOR_LAST_REC:
        ret = set_last_valid_record();
      break;

    case GLUCOSE_OPERATOR_WITHIN_RANGE:
        ret = set_within_range_valid_record(p_ctrl_point_cmd->filter_type, p_ctrl_point_cmd->param1, p_ctrl_point_cmd->param2);
      break;

    case GLUCOSE_OPERATOR_LESS_EQU:
        ret = set_less_equal_valid_record(p_ctrl_point_cmd->filter_type, p_ctrl_point_cmd->param1);
      break;

    case GLUCOSE_OPERATOR_GREATER_EQU:
        ret = set_greater_equal_valid_record(p_ctrl_point_cmd->filter_type, p_ctrl_point_cmd->param1);
      break;

    default:
      rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_INVALID_OPERATOR, p_ctrl_point_cmd->opcode);
      ret = true;
      break;
  }
  if (ret)
  {
    if(GLUCOSE_OPCODE_REPORT_REC == p_ctrl_point_cmd->opcode)
    {
      rsi_ble_app_set_event (RSI_BLE_APP_CHAR_MEAS_SEND_EVENT);
    }
    else if(GLUCOSE_OPCODE_DELETE_REC == p_ctrl_point_cmd->opcode)
    {
      rsi_ble_app_clear_event (RSI_BLE_APP_CHAR_MEAS_SEND_EVENT);
      rsi_ble_app_set_event (RSI_BLE_APP_CHAR_RECORD_DEL_EVENT);
    }

  }
  return ret;
}

/*==============================================*/
/**
 * @fn         get_number_of_valid_record
 * @brief      Get number of valid records
 * @param[ ]   none
 * @return     uint16_t - number of records
 * @section description
 * this function is used get number of valid records from glucose measurement database.
 */
static uint16_t get_number_of_valid_record(void)
{
  uint8_t i;
  uint16_t count = 0;
  for(i = 0; i < GLUCOSE_MAX_RECORD; i++)
  {
    if(glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_VALID))
    {
      count++;
    }
  }
  return count;
}

/*==============================================*/
/**
 * @fn         rsi_ble_ctrl_point_send_num_record_rsp
 * @brief      process cotr the event parameter.
 * @param[in]  uint8_t   num_rec - number of records 
 * @return     none.
 * @section description
 * this function is used to process ctrl point data.
 */
static void rsi_ble_ctrl_point_send_num_record_rsp(uint16_t num_rec)
{
   if(app_state & (1 << CONNECTED))
  {
    if(app_state & (1 << GLUCOSE_CTRL_POINT_INDICATE_ENABLE))
    {
      uint8_t data[4] = {0};
      uint8_t len = 4;
      data[0] = GLUCOSE_OPCODE_RESPONSE_NUM_REC;
      data[1] = GLUCOSE_OPERATOR_NULL;
      data[2] = num_rec & 0xff;
      data[3] = (num_rec >> 8) & 0xff;
      rsi_ble_set_local_att_value (rsi_ble_ctrl_point_val_hndl, len, data);
    }
  }
  LOG_PRINT("rsi_ble_ctrl_point_send_num_record_rsp\n");
}

/*==============================================*/
/**
 * @fn         rsi_ble_ctrl_point_handle_cmd
 * @brief      handle control point command
 * @param[out] glucose_ctrl_point_t* - pointer to control point structure
 * @return     none.
 * @section description
 * this function is used to process ctrl point message (handling opcode).
 */
static void rsi_ble_ctrl_point_handle_cmd(glucose_ctrl_point_t* p_ctrl_point_cmd)
{

  switch (p_ctrl_point_cmd->opcode)
  {
    case GLUCOSE_OPCODE_REPORT_REC:
      if(!rsi_ble_ctrl_point_handle_operator(p_ctrl_point_cmd))
      {
        rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_NO_RECORD_FOUND, p_ctrl_point_cmd->opcode);
      }
      break;
    
    case GLUCOSE_OPCODE_DELETE_REC:
      if(!rsi_ble_ctrl_point_handle_operator(p_ctrl_point_cmd))
      {
        rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_NO_RECORD_FOUND, p_ctrl_point_cmd->opcode);
      }
      break;
    
    case GLUCOSE_OPCODE_REPORT_NUM_REC:
      rsi_ble_ctrl_point_send_num_record_rsp(get_number_of_valid_record());
      break;

    case GLUCOSE_OPCODE_ABORT_OPERATOR:
      rsi_ble_app_clear_event (RSI_BLE_APP_CHAR_MEAS_SEND_EVENT);
      rsi_ble_app_clear_event (RSI_BLE_APP_CHAR_RECORD_DEL_EVENT);
      rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_SUCCESS, GLUCOSE_OPCODE_ABORT_OPERATOR);
      break;

    default:
  
      break;
  }

}

/*==============================================*/
/**
 * @fn         rsi_ble_ctrl_point_opcode_check
 * @brief      process cotr the event parameter.
 * @param[in]  uint8_t      length of the data
 * @param[in]  uint8_t *    p_data - pointer to ctrl point data
 * @return     none.
 * @section description
 * this function is used to process ctrl point data.
 */
static bool rsi_ble_ctrl_point_opcode_check(glucose_ctrl_point_t* ctrl_point_cmd)
{
  bool ret = false;
  switch (ctrl_point_cmd->opcode)
  {
    case GLUCOSE_OPCODE_REPORT_REC:
    case GLUCOSE_OPCODE_DELETE_REC:
    case GLUCOSE_OPCODE_REPORT_NUM_REC:
      if (ctrl_point_cmd->operator == GLUCOSE_OPERATOR_NULL)
      {
        rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_INVALID_OPERATOR, ctrl_point_cmd->opcode);
      }
      else
      {
        ret = true;
      }
      break;

    case GLUCOSE_OPCODE_ABORT_OPERATOR:
      if (ctrl_point_cmd->operator != GLUCOSE_OPERATOR_NULL)
      {
        rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_INVALID_OPERATOR, ctrl_point_cmd->opcode);
      }
      else
      {
        ret = true;
      }
      break;

    default:
      rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_OPCODE_NOT_SUPPORT, ctrl_point_cmd->opcode);
  }
  return ret;
}

/*==============================================*/
/**
 * @fn         rsi_ble_ctrl_point_data_process
 * @brief      process cotr the event parameter.
 * @param[in]  uint8_t      length of the data
 * @param[in]  uint8_t *    p_data - pointer to ctrl point data
 * @return     none.
 * @section description
 * this function is used to process ctrl point data.
 */

static void rsi_ble_ctrl_point_data_process(uint8_t len, uint8_t *p_data)
{
  glucose_ctrl_point_t ctrl_point_cmd = {0};
  date_time_t time1, time2;
  uint16_t seq_num1, seq_num2;
  bool opcode_valid = false;
  bool operator_valid = false;
 
  //! fill control point structure
  ctrl_point_cmd.len = len;
  if(len>=1)
  {
    ctrl_point_cmd.opcode = p_data[0];
  }
  if(len>=2)
  {
    ctrl_point_cmd.operator = p_data[1];
  }

  //! clear filter bit from state field
  set_all_valid_record(false);
  
  //! check opcode valid or not
  opcode_valid = rsi_ble_ctrl_point_opcode_check(&ctrl_point_cmd);
 
  //! if opcode is valid then chek operator valid or not
  if (opcode_valid)
  {
    switch (ctrl_point_cmd.operator)
    {
      case GLUCOSE_OPERATOR_NULL:
      case GLUCOSE_OPERATOR_ALL_REC:
      case GLUCOSE_OPERATOR_FIRST_REC:
      case GLUCOSE_OPERATOR_LAST_REC:
        if (2 == ctrl_point_cmd.len)
        {
          operator_valid = true;
        }
        else
        {
          rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_INVALID_OPERAND, ctrl_point_cmd.opcode);
        }
        break;

      case GLUCOSE_OPERATOR_LESS_EQU:
      case GLUCOSE_OPERATOR_GREATER_EQU:
        {
          ctrl_point_cmd.filter_type = p_data[2];
          if ((5 == ctrl_point_cmd.len) && (GLUCOSE_FILTER_TYPE_SEQ_NUM == ctrl_point_cmd.filter_type))
          {
            seq_num1 = p_data[3] | (p_data[4] << 8);
            ctrl_point_cmd.param1 = &seq_num1;
            operator_valid = true;
          }
          else if ((10 == ctrl_point_cmd.len) && (GLUCOSE_FILTER_TYPE_TIME == ctrl_point_cmd.filter_type))
          {
            time1.year = p_data[3] | (p_data[4] << 8);
            time1.month = p_data[5];
            time1.day = p_data[6];
            time1.hour = p_data[7];
            time1.minute = p_data[8];
            time1.second = p_data[9];
            ctrl_point_cmd.param1 = &time1;
            operator_valid = true;
          }
          else
          {
            rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_INVALID_OPERAND, ctrl_point_cmd.opcode);
          }
        }
        break;

      case GLUCOSE_OPERATOR_WITHIN_RANGE:
        {
          ctrl_point_cmd.filter_type = p_data[2];
          if ((7 == ctrl_point_cmd.len) && (GLUCOSE_FILTER_TYPE_SEQ_NUM == ctrl_point_cmd.filter_type))
          {
            seq_num1 = p_data[3] | (p_data[4] << 8);
            ctrl_point_cmd.param1 = &seq_num1;
            seq_num2 = p_data[5] | (p_data[6] << 8);
            ctrl_point_cmd.param2 = &seq_num2;
            operator_valid = true;
          }
          else if ((17 == ctrl_point_cmd.len) && (GLUCOSE_FILTER_TYPE_TIME == ctrl_point_cmd.filter_type))
          {
            time1.year = p_data[3] | (p_data[4] << 8);
            time1.month = p_data[5];
            time1.day = p_data[6];
            time1.hour = p_data[7];
            time1.minute = p_data[8];
            time1.second = p_data[9];
            ctrl_point_cmd.param1 = &time1;
            time2.year = p_data[10] | (p_data[11] << 8);
            time2.month = p_data[12];
            time2.day = p_data[13];
            time2.hour = p_data[14];
            time2.minute = p_data[15];
            time2.second = p_data[16];
            ctrl_point_cmd.param2 = &time2;
            operator_valid = true;
          }
          else
          {
            rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_INVALID_OPERAND, ctrl_point_cmd.opcode);
          }
        }
        break;

      default:
        rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_INVALID_OPERAND, ctrl_point_cmd.opcode);
    }
  }
  
  /* if opcode and operator both are valid, then handle command and send data accordingly.
   * i.e. enable the functions such as 
   * counting records, transmitting records and clearing records based on filter criterion.
   */
  if (operator_valid)
  {
    rsi_ble_ctrl_point_handle_cmd(&ctrl_point_cmd);
  }

}

/*=============================================*/
/**
 * @fn         ctrl_point_next_filtered_index 
 * @brief      search for next filtered record
 * @param[in]  curr_index, current glucose record index.
 * @return     record index.
 * @section description
 * returns next filterd index of glucose database
 */
static uint8_t ctrl_point_next_filtered_index(uint8_t curr_index)
{
  uint8_t i;
  
  if(curr_index >= GLUCOSE_MAX_RECORD)
  {
    return GLUCOSE_MAX_RECORD;
  }
  
  for(i = curr_index; i < GLUCOSE_MAX_RECORD; i++)
  {
    if((glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_VALID)) &&
        (glucose_meas_val[i].state & (1 << GLUCOSE_RECORD_STATE_FILTERED)))
    {
      break;
    }
  }
  return i;
}

/*=============================================*/
/**
 * @fn         glucose_meas_fill_value 
 * @brief      creates the glucose mesurment data.
 * @param[in]  val, it indicates glucose mesurement strcture.
 * @param[out] p_data, glucose mesurement strcture in buffer pointer.
 * @return     length of glucose data buffer.
 * @section description
 */

static uint8_t glucose_meas_fill_value(glucose_meas_t val, uint8_t *p_data)
{
  uint8_t len = 0;
  
  //! payload 1st byte is flags
  p_data[len++] = val.flags;

  //! according to flag, we need to fill data
  
  //! sequence number
  p_data[len++] = val.seq_num & 0xff;
  p_data[len++] = (val.seq_num >> 8) & 0xff;

  //! base time
  p_data[len++] = val.base_time.year & 0xff;
  p_data[len++] = (val.base_time.year >> 8) & 0xff;
  p_data[len++] = val.base_time.month;
  p_data[len++] = val.base_time.day;
  p_data[len++] = val.base_time.hour;
  p_data[len++] = val.base_time.minute;
  p_data[len++] = val.base_time.second;

  //! time offset present
  if (val.flags & BIT(0))
  {
    p_data[len++] = val.time_offset & 0xff;
    p_data[len++] = (val.time_offset >> 8) & 0xff;  
  }

  //! glucose concentration, type and sample location present
  if (val.flags & BIT(1))
  {
    uint16_t temp_conc = 0;
    //! glucose concentration units
    if(val.flags & BIT(2))
    {
      // mol/l
    }
    else
    {
      // kg/l
    }
    
    temp_conc = float_to_16bit_sfloat(val.concentration);
    p_data[len++] = temp_conc & 0xff;
    p_data[len++] = (temp_conc >> 8) & 0xff;
    p_data[len++] = val.type_sample_loc;
  }

  //! sensor status annunciation
  if(val.flags & BIT(3))
  {
    p_data[len++] = val.sansor_status_ann & 0xff;
    p_data[len++] = (val.sansor_status_ann >> 8) & 0xff;
  }

  //! context information follows
  if(val.flags & BIT(4))
  {
    //! set flag to send glucose context 
  }
  
  return len;
}

/*=============================================*/
/**
 * @fn         glucose_context_fill_value 
 * @brief      creates the glucose mesurment context data.
 * @param[in]  val, it indicates glucose mesurement strcture.
 * @param[out] p_data, glucose mesurement strcture in buffer pointer.
 * @return     length of glucose data buffer.
 * @section description
 */

static uint8_t glucose_context_fill_value(glucose_context_t val, uint8_t *p_data)
{
  uint8_t len = 0;

  //! payload 1st byte is flags
  p_data[len++] = val.flags;

  //! sequence number
  p_data[len++] = val.seq_num & 0xff;
  p_data[len++] = (val.seq_num >> 8) & 0xff;

  //! extended flags present
  if (val.flags & BIT(7))
  {
    p_data[len++] = val.extended_flags & 0xff;
    p_data[len++] = (val.extended_flags >> 8) & 0xff;
  }

  //! carbohydrate id and carbohydrate present
  if (val.flags & BIT(0))
  {
    uint16_t temp_carbo = 0;
    p_data[len++] = val.carbohyd_id & 0xff;
    p_data[len++] = (val.carbohyd_id >> 8) & 0xff;

    temp_carbo = float_to_16bit_sfloat(val.carbohyd_val);
    p_data[len++] = temp_carbo & 0xff;
    p_data[len++] = (temp_carbo >> 8) & 0xff;
  }

  //! meal present
  if (val.flags & BIT(1))
  {
    p_data[len++] = val.meal & 0xff;
    p_data[len++] = (val.meal >> 8) & 0xff;
  }

  //! tester-health present 
  if (val.flags & BIT(2))
  {
    p_data[len++] = val.tester_health & 0xff;
  }

  //! exercise duration and exercise intensity present
  if (val.flags & BIT(3))
  {
    p_data[len++] = val.exercise_duration & 0xff;
    p_data[len++] = (val.exercise_duration >> 8) & 0xff;
    p_data[len++] = val.exercise_intensity & 0xff;
  }

  //! medication id and medication present
  if (val.flags & BIT(4))
  {
    uint16_t temp_medi = 0;
    p_data[len++] = val.medication_id & 0xff;

    temp_medi = float_to_16bit_sfloat(val.medication_val);
    //! medication value units
    if(val.flags & BIT(5))
    {
      // liters
    }
    else
    {
      // kilograms
    }
    p_data[len++] = temp_medi & 0xff;
    p_data[len++] = (temp_medi >> 8) & 0xff;

  }

  //! hbA1c Present
  if (val.flags & BIT(6))
  {
    uint16_t temp_hba1c = 0;
    temp_hba1c = float_to_16bit_sfloat(val.hba1c);
    p_data[len++] = temp_hba1c & 0xFF;
    p_data[len++] = (temp_hba1c >> 8) & 0xFF;
  }

  return len;
}

/*=============================================*/
/**
 * @fn         rsi_ble_glucose_server_handle_write_event 
 * @brief      handles server side write events.
 * @param[in]  none
 * @return     none
 * @section description
 * handles server side write events and set respective flags.
 */

static void rsi_ble_glucose_server_handle_write_event(void)
{
  if ((rsi_ble_glucose_val_hndl + 1) == *((uint16_t *)app_ble_write_event.handle))
  {
    //! 0x01 for notification
    if(app_ble_write_event.att_value[0] == 0x01)
    {
      uint8_t len = 0;
      uint8_t data[25] = {0};
			
      //! set a bit
      app_state |= (1 << GLUCOSE_MEASUREMENT_NOTIFY_ENABLE);

      //For testing sending 1st record, on enabling notification.
      len = glucose_meas_fill_value(glucose_meas_val[glucose_meas_index], data);
      rsi_ble_set_local_att_value (rsi_ble_glucose_val_hndl, len, data);
    }
    else if(app_ble_write_event.att_value[0] == 0)
    {
      //! clear a bit
      app_state &= ~(1 << GLUCOSE_MEASUREMENT_NOTIFY_ENABLE);
    }
    LOG_PRINT ("glucose measurement notify %d\n", app_state & (1 << GLUCOSE_MEASUREMENT_NOTIFY_ENABLE));
  }
  else if ((rsi_ble_glucose_context_val_hndl + 1) == *((uint16_t *)app_ble_write_event.handle))
  {
    //! 0x01 for notification
    if(app_ble_write_event.att_value[0] == 0x01)
    {
      //! set a bit
      app_state |= (1 << GLUCOSE_CONTEXT_NOTIFY_ENABLE);
    }
    else if(app_ble_write_event.att_value[0] == 0)
    {
      //! clear a bit
      app_state &= ~(1 << GLUCOSE_CONTEXT_NOTIFY_ENABLE);
    }
    LOG_PRINT ("glucose measurement context notify %d\n",app_state & (1 << GLUCOSE_CONTEXT_NOTIFY_ENABLE));
  }
  else if ((rsi_ble_ctrl_point_val_hndl + 1) == *((uint16_t *)app_ble_write_event.handle))
  {
    //! 0x02 for indication
    if(app_ble_write_event.att_value[0] == 0x02)
    {
      //! set a bit
      app_state |= (1 << GLUCOSE_CTRL_POINT_INDICATE_ENABLE);
    }
    else if(app_ble_write_event.att_value[0] == 0)
    {
      //! clear a bit
      app_state &= ~(1 << GLUCOSE_CTRL_POINT_INDICATE_ENABLE);
    }

    LOG_PRINT ("glucose control point indicate ");
  }
  else if ((rsi_ble_ctrl_point_val_hndl) == *((uint16_t *)app_ble_write_event.handle))
  {
#ifdef SMP_ENABLED
#else
      app_state |= (1 << ENCRYPT_EN);
#endif
    if(app_state & (1 << ENCRYPT_EN))
    {
     rsi_ble_ctrl_point_data_process(app_ble_write_event.length,app_ble_write_event.att_value);
     LOG_PRINT ("glucose control point val: length %d, opcode %d, oprator %d\n",app_ble_write_event.length,
                                                app_ble_write_event.att_value[0],
                                                app_ble_write_event.att_value[1]
                                                );

    }
    else
    {
      LOG_PRINT ("glucose control point write Authentication required");
      //! initiating the SMP pairing process
      //rsi_ble_smp_pair_request (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      
      // For testing, enabling encryption. Need to change
      app_state |= (1 << ENCRYPT_EN);
      rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_PROCUDURE_NOT_COMPLETED, app_ble_write_event.att_value[0]);
    }
  }
  else
  {
    LOG_PRINT("write else part");
  }
}

#elif(GATT_ROLE == CLIENT)

/*=============================================*/
/**
 * @fn         rsi_ble_glucose_client_handle_write_event 
 * @brief      handles client side write events.
 * @param[in]  none
 * @return     none
 * @section description
 * handles client side write events and set respective flags.
 */
static void rsi_ble_glucose_client_handle_write_event(void)
{
  if (rsi_ble_glucose_val_hndl != 0)
  {
    if (rsi_ble_glucose_val_hndl == *((uint16_t *)app_ble_write_event.handle))
    {
      LOG_PRINT("Glucose measurement value:\nSequence Number : %d",
          app_ble_write_event.att_value[1] | (app_ble_write_event.att_value[2] << 8));
    }
  }

  //! optional characterstisc condition
  /*if (rsi_ble_glucose_context_val_hndl != 0)
  {
    if (rsi_ble_glucose_context_val_hndl == *((uint16_t *)app_ble_write_event.handle))
    {
      LOG_PRINT("Glucose measurement contex value:"); 
    }
  }*/
}
#endif

/*=============================================*/
/**
 * @fn         glucose_ctrl_point_fill_value 
 * @brief      Creates the glucose control point data.
 * @param[in]  val, it indicates glucose ctrl point strcture.
 * @param[out] p_data, glucose ctrl point strcture in buffer pointer.
 * @return     length of glucose data buffer.
 * @section description
 */

static uint8_t glucose_ctrl_point_fill_value(glucose_ctrl_point_t val, uint8_t *p_data)
{
  uint8_t len = 0;
  uint16_t *seq_num1, *seq_num2;
  date_time_t *time1, *time2;

  p_data[len++] = val.opcode;
  p_data[len++] = val.operator;
  
  switch(val.operator)
  {
    case GLUCOSE_OPERATOR_LESS_EQU:
    case GLUCOSE_OPERATOR_GREATER_EQU:
      p_data[len++] = val.filter_type;
      if (val.filter_type == GLUCOSE_FILTER_TYPE_SEQ_NUM)
      {
        seq_num1 = val.param1;
        p_data[len++] = (*seq_num1) & 0xff;
        p_data[len++] = (*seq_num1 >> 8) & 0xff;
      }
      else
      {
        time1 = val.param1;
        p_data[len++] = (time1->year) & 0xff;
        p_data[len++] = ((time1->year) >> 8) & 0xff;
        p_data[len++] = (time1->month);
        p_data[len++] = (time1->day);
        p_data[len++] = time1->hour;
        p_data[len++] = time1->minute;
        p_data[len++] = time1->second;
      }
      break;
    case GLUCOSE_OPERATOR_WITHIN_RANGE:
      p_data[len++] = val.filter_type;
      if (val.filter_type == GLUCOSE_FILTER_TYPE_SEQ_NUM)
      {
        seq_num1 = val.param1;
        seq_num2 = val.param2;
        p_data[len++] = (*seq_num1) & 0xff;
        p_data[len++] = (*seq_num1 >> 8) & 0xff;
        p_data[len++] = (*seq_num2) & 0xff;
        p_data[len++] = (*seq_num2 >> 8) & 0xff;
      }
      else
      {
        time1 = val.param1;
        time2 = val.param2;
        p_data[len++] = (time1->year) & 0xff;
        p_data[len++] = ((time1->year) >> 8) & 0xff;
        p_data[len++] = (time1->month);
        p_data[len++] = (time1->day);
        p_data[len++] = time1->hour;
        p_data[len++] = time1->minute;
        p_data[len++] = time1->second;
        p_data[len++] = (time2->year) & 0xff;
        p_data[len++] = ((time2->year) >> 8) & 0xff;
        p_data[len++] = (time2->month);
        p_data[len++] = (time2->day);
        p_data[len++] = time2->hour;
        p_data[len++] = time2->minute;
        p_data[len++]= time2->second;
      }
      break;
    default:
      break;
  }
  return len;
}


/*==============================================*/
/**
 * @fn         rsi_ble_glucose_gatt_server
 * @brief      this is the application of glucose application.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This function is used to test the glucose application.
 */

int32_t rsi_ble_glucose_gatt_server(void)
{

  int32_t status = 0;
  int32_t event_id;
  uint8_t data[25] = {0};
  int8_t local_dev_addr[18] = {0};
  static uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = {0};
  uint8_t len;
#if (GATT_ROLE == SERVER)
  uint8_t adv[31] = {2,1,6};
#elif (GATT_ROLE == CLIENT)
  uint8_t ix,jx;
  static uint8_t char_srv_index = 0;
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
  //! adding BLE Glucose service
  rsi_ble_add_glucose_serv ();
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
  rsi_ble_gatt_register_callbacks (
      NULL,
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
      NULL
      );

  //! registering the SMP callback functions
	rsi_ble_smp_register_callbacks(
      rsi_ble_on_smp_request,
      rsi_ble_on_smp_response,
      rsi_ble_on_smp_passkey,
      rsi_ble_on_smp_failed,
      rsi_ble_on_encrypt_started,
      rsi_ble_on_smp_passkey_display,
      NULL,
      rsi_ble_on_le_ltk_req_event,
      NULL,
      NULL,NULL);

  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! Set local name
  rsi_bt_set_local_name((uint8_t *)RSI_BLE_APP_GLUCOSE);

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
  adv[3] = strlen (RSI_BLE_APP_GLUCOSE) + 1;
  adv[4] = 9;
  strcpy ((char *)&adv[5], RSI_BLE_APP_GLUCOSE);

  //! set advertise data
  rsi_ble_set_advertise_data (adv, strlen (RSI_BLE_APP_GLUCOSE) + 5);

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
    
#endif
      continue;
    }

    switch (event_id) 
    {

#if(GATT_ROLE == CLIENT)
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

          memcpy ((int8_t *)remote_dev_bd_addr, (uint8_t *)conn_event_to_app.dev_addr, 6);
#if(GATT_ROLE == SERVER)
          //! set a bit
          app_state |= (1 << CONNECTED);


#elif(GATT_ROLE == CLIENT)
          //! clear start handler of characteristics
          rsi_ble_glucose_val_hndl = 0;
          rsi_ble_glucose_context_val_hndl = 0;
          rsi_ble_ctrl_point_val_hndl = 0;

          //! clear attributes info (i.e. Each rows of gatt glucose service table)
          memset (&ble_servs, 0, sizeof(profile_descriptors_t));
          memset (&char_servs, 0, sizeof(rsi_ble_resp_char_services_t));
          memset (&att_desc, 0, sizeof(rsi_ble_resp_att_descs_t));

          service_uuid.size = 2;
          service_uuid.val.val16 = RSI_BLE_GLUCOSE_SERVICE_UUID;
get_prof:          
          //! query particular profile details from the connected remote device.
          status = rsi_ble_get_profile (remote_dev_bd_addr, service_uuid, &ble_servs);
          if(status != 0)
          {
            goto get_prof;
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
          app_state = 0;

adv:
          LOG_PRINT ("adv start\n");
          //! set device in advertising mode.
          status = rsi_ble_start_advertising ();
          if (status != RSI_SUCCESS)
          {
            goto adv;
          }
#elif(GATT_ROLE == CLIENT)
scan:
          //! start scanning
          device_found=0;
          status = rsi_ble_start_scanning();
          if (status != RSI_SUCCESS)
          {
            goto scan;
          }
#endif
        }
        break;
        
      case RSI_BLE_SMP_REQ_EVENT:
      {
        //! initiate SMP protocol as a Master

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_SMP_REQ_EVENT);

        LOG_PRINT ("smp_req \n");
        
        //! initiating the SMP pairing process
        status = rsi_ble_smp_pair_request (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      }
      break;

      case RSI_BLE_SMP_RESP_EVENT:
      {
        //! initiate SMP protocol as a Master

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_SMP_RESP_EVENT);

        LOG_PRINT ("smp_resp \n");
        //! initiating the SMP pairing process
        status = rsi_ble_smp_pair_response (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
      }
      break;

      case RSI_BLE_SMP_PASSKEY_EVENT:
      {
        //! initiate SMP protocol as a Master

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_EVENT);

        LOG_PRINT ("smp_passkey \n");
        //! initiating the SMP pairing process
        status = rsi_ble_smp_passkey (remote_dev_bd_addr, RSI_BLE_SMP_PASSKEY);
      }
      break;

      case RSI_BLE_SMP_FAILED_EVENT:
      {
        //! initiate SMP protocol as a Master

				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_SMP_FAILED_EVENT);
        LOG_PRINT ("smp_failed \n");
      }
      break;

      case RSI_BLE_ENCRYPT_STARTED_EVENT:
      {
        //! start the encrypt event
        
				//! clear the served event
				rsi_ble_app_clear_event (RSI_BLE_ENCRYPT_STARTED_EVENT);
        LOG_PRINT ("smp_encrypt \n");
#if(GATT_ROLE == CLIENT)
        len = glucose_ctrl_point_fill_value(glucose_ctrl_point_val, data);
        //rsi_ble_set_local_att_value (rsi_ble_ctrl_point_val_hndl, len, data);

        rsi_ble_set_att_value(remote_dev_bd_addr, 
            rsi_ble_ctrl_point_val_hndl,
            len,
            data);
        LOG_PRINT ("ctrol point send cmd \n");
#endif

      }
      break;

      case RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT:
      {
        //! clear the served event
	      rsi_ble_app_clear_event (RSI_BLE_SMP_PASSKEY_DISPLAY_EVENT);
        LOG_PRINT ("smp_passkey_disp \n");

        //rsi_ble_smp_passkey (remote_dev_bd_addr, RSI_BLE_SMP_PASSKEY);        
      }
      break;

      case RSI_BLE_LTK_REQ_EVENT:
      {
        //! event invokes when disconnection was completed

        //! clear the served event
        rsi_ble_app_clear_event (RSI_BLE_LTK_REQ_EVENT);

        //! give le ltk req reply cmd with negative reply
        status = rsi_ble_ltk_req_reply(remote_dev_bd_addr,0,NULL);
      }
      break;

      case RSI_BLE_GATT_WRITE_EVENT:
        {
          //! event invokes when write/notification events received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_WRITE_EVENT);

#if(GATT_ROLE == SERVER)
          rsi_ble_glucose_server_handle_write_event();
#elif(GATT_ROLE == CLIENT)
          rsi_ble_glucose_client_handle_write_event();
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
            rsi_ble_get_char_services(remote_dev_bd_addr,
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

          LOG_PRINT("char_srv_index %d",char_srv_index);

          if(char_servs.num_of_services == char_srv_index)
          {
#ifdef SMP_ENABLED
#else
            app_state |= (1 << ENCRYPT_EN);
#if(GATT_ROLE == CLIENT)
            len = glucose_ctrl_point_fill_value(glucose_ctrl_point_val, data);
            //rsi_ble_set_local_att_value (rsi_ble_ctrl_point_val_hndl, len, data);

            rsi_ble_set_att_value(remote_dev_bd_addr, 
                rsi_ble_ctrl_point_val_hndl,
                len,
                data);
            LOG_PRINT ("ctrol point send cmd \n");
#endif
#endif
            if(!(app_state & (1 << ENCRYPT_EN)))
            {
              //! initiating the SMP pairing process
              rsi_ble_smp_pair_request (remote_dev_bd_addr, RSI_BLE_SMP_IO_CAPABILITY, MITM_REQ);
            }
          }

          //! verifying the glucose characteristic
          for ( ; char_srv_index < char_servs.num_of_services; char_srv_index++)
          {
            LOG_PRINT ("serv_handle: 0x%04x - property: 0x%02x, char_handle: 0x%04x, char_uuid: 0x%04x \n",
                char_servs.char_services[char_srv_index].handle,
                char_servs.char_services[char_srv_index].char_data.char_property,
                char_servs.char_services[char_srv_index].char_data.char_handle,
                char_servs.char_services[char_srv_index].char_data.char_uuid.val.val16);

            switch (char_servs.char_services[char_srv_index].char_data.char_uuid.val.val16)
            {

              case RSI_BLE_GLUCOSE_MEASUREMENT_UUID:
              case RSI_BLE_GLUCOSE_MEASUREMENT_CONTEXT_UUID:
              case RSI_BLE_RECORD_ACCESS_CONTROL_POINT_UUID: 
              case RSI_BLE_GLUCOSE_FEATURE_UUID:

                //! store charteristics handler
                switch (char_servs.char_services[char_srv_index].char_data.char_uuid.val.val16)
                {
                  case RSI_BLE_GLUCOSE_MEASUREMENT_UUID:
                    rsi_ble_glucose_val_hndl = char_servs.char_services[char_srv_index].char_data.char_handle;
                    break;
                  
                  case RSI_BLE_GLUCOSE_MEASUREMENT_CONTEXT_UUID:
                    rsi_ble_glucose_context_val_hndl = char_servs.char_services[char_srv_index].char_data.char_handle;
                    break;

                  case RSI_BLE_RECORD_ACCESS_CONTROL_POINT_UUID: 
                    rsi_ble_ctrl_point_val_hndl = char_servs.char_services[char_srv_index].char_data.char_handle;
                    break;
                }


                //! query attribute descriptor, with in the particular range, from the connected remote device.
                if(char_srv_index < (char_servs.num_of_services - 1))
                {
                  rsi_ble_get_att_descriptors(remote_dev_bd_addr,
                      char_servs.char_services[char_srv_index].handle,
                      char_servs.char_services[char_srv_index+1].handle-1,
                      &att_desc);
                }
                else
                {
                  rsi_ble_get_att_descriptors(remote_dev_bd_addr,
                      char_servs.char_services[char_srv_index].handle,
                      *(uint16_t *)ble_servs.end_handle,
                      &att_desc);
                }

                break;

             
              default:
                //! continue for loop, becouse needed char not found.
                continue;
            }

            //! break for loop, becouse needed characteristic found.
            //! next event will be CCCD for founded char.
            char_srv_index++;
            break;
          }
        }
        break;

      case RSI_BLE_GATT_CHAR_DESC_RESP_EVENT:
        {
          uint8_t temp_prop = 0;
          uint8_t char_desc_index = 0;
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_GATT_CHAR_DESC_RESP_EVENT);
         
          for (char_desc_index = 0; char_desc_index < att_desc.num_of_att; char_desc_index++)
          {
            LOG_PRINT("char_handle 0x%04x, char_uuid: 0x%04x \n",
                (att_desc.att_desc[char_desc_index].handle[0] | (att_desc.att_desc[char_desc_index].handle[1] << 8)),
                att_desc.att_desc[char_desc_index].att_type_uuid.val.val16);
            if(RSI_BLE_CHAR_SERV_UUID == att_desc.att_desc[char_desc_index].att_type_uuid.val.val16)
            {
              
              temp_prop = char_servs.char_services[char_srv_index-1].char_data.char_property;
            }

            if(RSI_BLE_CLIENT_CHAR_UUID == att_desc.att_desc[char_desc_index].att_type_uuid.val.val16)
            {
              //! check the attribute property with notification
              if (temp_prop & RSI_BLE_ATT_PROPERTY_NOTIFY)
              {
                LOG_PRINT ("CCCD notify char_srv_index %d\n",char_srv_index-1);
                //0x0001 to enable notification
                data[0] = 0x01;
                data[1] = 0x00;
                //! Set attribute value of the connected remote device.Ack will not be received from the remote device.
                rsi_ble_set_att_value(remote_dev_bd_addr, 
                    *((uint16_t *)att_desc.att_desc[char_desc_index].handle),
                    2,
                    data);
              }

              //! check the attribute property with indication
              if (temp_prop & RSI_BLE_ATT_PROPERTY_INDICATE)
              {
                LOG_PRINT ("CCCD indicate char_srv_index %d\n",char_srv_index-1);
                //0x0002 to enable indication
                data[0] = 0x02;
                data[1] = 0x00;
                //! Set attribute value of the connected remote device.Ack will not be received from the remote device.
                rsi_ble_set_att_value(remote_dev_bd_addr, 
                    *((uint16_t *)att_desc.att_desc[char_desc_index].handle),
                    2,
                    data);
              }

            }
          }

          //! set event for next charteristics
          rsi_ble_app_set_event (RSI_BLE_GATT_CHAR_SERVICES_RESP_EVENT);
          memset (&att_desc, 0, sizeof(rsi_ble_resp_att_descs_t));

        }
        break;

#elif(GATT_ROLE == SERVER)

      case RSI_BLE_APP_CHAR_MEAS_SEND_EVENT:
        {
          //! event invokes when control point command report received

          //! clear the served event
          //rsi_ble_app_clear_event (RSI_BLE_APP_CHAR_MEAS_SEND_EVENT);
          
          //! test a bit
          if(app_state & (1 << CONNECTED))
          {
            if(app_state & (1 << GLUCOSE_MEASUREMENT_NOTIFY_ENABLE))
            {
              LOG_PRINT ("glucose measuement notify \n");
              len = glucose_meas_fill_value(glucose_meas_val[glucose_meas_index], data);
              rsi_ble_set_local_att_value (rsi_ble_glucose_val_hndl, len, data);

            }
            
            if(app_state & (1 << GLUCOSE_CONTEXT_NOTIFY_ENABLE))
            {
              LOG_PRINT ("glucose measuement context notify \n");
              len = glucose_context_fill_value(glucose_context_val[glucose_meas_index], data);
              rsi_ble_set_local_att_value (rsi_ble_glucose_context_val_hndl, len, data);
            }
           
            //! get next filtered index
            glucose_meas_index = ctrl_point_next_filtered_index(glucose_meas_index + 1);

            if (glucose_meas_index >= GLUCOSE_MAX_RECORD)
            {
              //! all records sent, sent success response and clear the app event
              rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_SUCCESS, GLUCOSE_OPCODE_REPORT_REC);
              rsi_ble_app_clear_event (RSI_BLE_APP_CHAR_MEAS_SEND_EVENT);
            }
            else
            {
              //! event is already set for sending next record
            }
            
            //! put some delay for sending next record for testing
#ifdef __linux__
            usleep(3000000);
#endif
#ifdef RSI_M4_INTERFACE 
			rsi_delay_ms (3000);
#endif
          }
        }
        break;

      case RSI_BLE_APP_CHAR_RECORD_DEL_EVENT:
        {
          //! event invokes when control point command report received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_APP_CHAR_RECORD_DEL_EVENT);
         
          if (app_state & (1 << CONNECTED))
          {
            LOG_PRINT ("glucose record delete event \n");
            while(glucose_meas_index < GLUCOSE_MAX_RECORD)
            {
              glucose_meas_val[glucose_meas_index].state &= ~(1 << GLUCOSE_RECORD_STATE_VALID);
              glucose_meas_index = ctrl_point_next_filtered_index(glucose_meas_index + 1);
            }
            
            rsi_ble_ctrl_point_send_rsp_code(GLUCOSE_RSP_SUCCESS, GLUCOSE_OPCODE_DELETE_REC);
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

  //! Call Glucose Server Application
  rsi_ble_glucose_gatt_server();

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
  rsi_task_create(rsi_ble_glucose_gatt_server, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}

