/**
 * @file          rsi_dhcp_user_class.h
 * @version       0.1
 * @date          30-May 2017 
 *
 *  Copyright(C) Redpine Signals 2017
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains DHCP user class, Option-77 related information
 *
 *  @section Description  This file contains prototypes and defines related to DHCP user class  
 *
 *
 */

#ifndef RSI_OPTION_77_H
#define RSI_OPTION_77_H

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
int32_t rsi_dhcp_user_class(uint8_t mode, uint8_t count, user_class_data_t *user_class_data, void(*dhcp_usr_cls_rsp_handler)(uint16_t status));

#endif
