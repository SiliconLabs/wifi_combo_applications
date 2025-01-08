/**
 * @file          rsi_mdnsd.h
 * @version       0.1
 * @date          30-Oct 2015 
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains mdnsd related information
 *
 *  @section Description  This file contains prototypes and defines related to MDNSD  
 *
 *
 */

#ifndef RSI_MDNSD_H
#define RSI_MDNSD_H

/******************************************************
 * *                      Macros
 * ******************************************************/
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
int32_t rsi_mdnsd_init(uint8_t ip_version, uint16_t ttl, uint8_t *host_name);
int32_t rsi_mdnsd_register_service(uint16_t port, uint16_t ttl, uint8_t more, uint8_t *service_ptr_name, uint8_t *service_name, uint8_t *service_text);
int32_t rsi_mdnsd_deinit(void);
#endif
