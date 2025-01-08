/**
 * @file     rsi_fwup.h
 * @version  0.1
 * @date     19 Oct 2015 
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains firmware upgradation related information
 *
 *  @section Description  This file contains firmware upgradation relating information
 *
 *
 */
#ifndef RSI_FWUP_H
#define RSI_FWUP_H
/******************************************************
 * *                      Macros
 * ******************************************************/
//! Firmware upgrade packet types
#define RSI_FWUP_RPS_HEADER           1          
#define RSI_FWUP_RPS_CONTENT          0 


/******************************************************
 * *                    Constants
 * ******************************************************/
/******************************************************
 * *                   Enumerations
 * ******************************************************/
/******************************************************
 * *                 Type Definitions
 * ******************************************************/


/******************************************************
 * *                    Structures
 * ******************************************************/
/******************************************************
 * *                 Global Variables
 * ******************************************************/
/******************************************************
 * *               Function Declarations
 * ******************************************************/
int32_t rsi_fwup_start(uint8_t *rps_header);
int32_t rsi_fwup_load(uint8_t *content, uint16_t length);
int32_t rsi_ota_firmware_upgradation(uint8_t flags, uint8_t *server_ip, uint32_t server_port, uint16_t chunk_number, uint16_t timeout, uint16_t tcp_retry_count, void(*ota_fw_up_response_handler)(uint16_t status, uint16_t chunk_number));
#endif
