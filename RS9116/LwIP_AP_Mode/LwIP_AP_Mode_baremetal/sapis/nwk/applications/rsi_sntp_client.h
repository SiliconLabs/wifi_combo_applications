/**
 * @file     rsi_sntp_client.h
 * @version  0.1
 * @date     1 May 2017 
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains sntp client related information
 *
 *  @section Description  This file contains sntp client relating information
 *
 *
 */
#ifndef RSI_SNTP_CLIENT_H
#define RSI_SNTP_CLIENT_H
/******************************************************
 * *                      Macros
 * ******************************************************/
#ifndef RSI_IPV6
#define   RSI_IPV6                         1
#endif


//!SNTP Method to USe 
#define RSI_SNTP_BROADCAST_MODE            1

//!SNTP Method to USe 
#define RSI_SNTP_UNICAST_MODE              2


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
int32_t rsi_sntp_client_create_async(uint8_t flags, uint8_t *server_ip, uint8_t sntp_method, uint16_t sntp_timeout,  
           void (*rsi_sntp_client_create_response_handler)(uint16_t status,const uint8_t cmd_type, const uint8_t *buffer ));
int32_t rsi_sntp_client_gettime(uint16_t length, uint8_t *sntp_time_rsp);
int32_t rsi_sntp_client_gettime_date(uint16_t length, uint8_t *sntp_time_date_rsp);
int32_t rsi_sntp_client_server_info(uint16_t length, uint8_t *sntp_server_response);
int32_t rsi_sntp_client_delete_async(void);
#endif
