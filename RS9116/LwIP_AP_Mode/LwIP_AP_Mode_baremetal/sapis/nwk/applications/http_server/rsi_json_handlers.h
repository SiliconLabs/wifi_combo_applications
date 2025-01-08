/**
 * @file     rsi_json_handlers.h
 * @version  1.0
 * @date     26 Oct 2015
 *
 * Copyright(C) Redpine Signals 2014
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief Example usage for Application data to displayed over webpage
 * 
 * @section Description
 * This file contains the definitions of data structures & APIs used to 
 * handle the JSON object data. This file also utilizes the JSON parser to convert to & from JSON
 * representation of data.
 *
 *
 */


/**
 * Includes
 */
#ifndef RSI_JSON_HANDLERS_H
#define RSI_JSON_HANDLERS_H


/* Scan response json object structure */
typedef struct rsi_json_object_scan_res_s {

  uint8_t   ssid[33];
  uint8_t   channel;
  uint8_t   sec_type;

} rsi_json_object_scan_res_t;

/* User Data Structure */
typedef struct rsi_json_object_s {
    uint8_t   ssid[33];
    uint8_t   channel;
    uint8_t   opmode;
    uint8_t   conec;
    uint32_t  accelerometer_y;
    uint8_t   psk[65];
    uint32_t  sec_enable;
    uint32_t  checkbox_2;
    uint8_t   sec_type;
    rsi_json_object_scan_res_t json_scan_res_object[11];
} rsi_json_object_t;


/**
 * Global Variables
 */
extern rsi_json_object_t  json_object_data;

/**
 * Function declarations
 */

/* This function is used to convert the structure to JSON string form */
uint8_t* rsi_json_object_stringify(uint8_t *json_object_string, rsi_json_object_t* json_object);


/* This function is used to initialize the structure with default values
   These values can be retrieved from weboage or something similar
 */
void rsi_json_object_init(rsi_json_object_t* json_object);


/* This function is used to update the json object data structure with the updates
   received from the browser.
 */
uint8_t rsi_json_object_update(rsi_json_object_t *json_object, uint8_t *json, uint8_t *json_file_name);


/* Helper function declarations */

/* This function is used to extract filename out of the received json update data. */
uint8_t* rsi_json_extract_filename(uint8_t* json, uint8_t* buffer);

/* Helper function which actually performs the update */
void rsi_json_object_data_update_helper(rsi_json_object_t* json_object, uint8_t* json);

int rsi_is_int(uint8_t c);
int rsi_is_float(uint8_t c);

void rsi_json_extract_float(uint8_t* json, uint8_t* key, float* val);
void rsi_json_extract_int(uint8_t* json, uint8_t* key, uint8_t * val);
void rsi_json_extract_string(uint8_t* json, uint8_t* key, uint8_t* string);
void rsi_json_extract_boolean(uint8_t* json, uint8_t* key, int* val);
void rsi_json_object_scan_list_update(rsi_json_object_t* json_object, rsi_rsp_scan_t *scan_rsp);


#endif
