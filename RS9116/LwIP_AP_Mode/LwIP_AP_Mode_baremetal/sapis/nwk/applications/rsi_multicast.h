/**
 * @file     rsi_multicast.h
 * @version  0.1
 * @date     10 Oct 2015 
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains multicast related information
 *
 *  @section Description  This file contains multicast relating information
 *
 *
 */
#ifndef RSI_MULTICAST_H
#define RSI_MULTICAST_H
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
int32_t rsi_multicast_join(uint8_t flags, int8_t *ip_address);
int32_t rsi_multicast_leave(uint8_t flags, int8_t *ip_address);

#endif

