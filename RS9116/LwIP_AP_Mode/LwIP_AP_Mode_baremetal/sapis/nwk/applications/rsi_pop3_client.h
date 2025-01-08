/**
 * @file     rsi_pop3_client.h
 * @version  0.1
 * @date     2 Dec 2015 
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains pop3 client related information
 *
 *  @section Description  This file contains pop3 client relating information
 *
 *
 */
#ifndef RSI_POP3_CLIENT_H
#define RSI_POP3_CLIENT_H
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


//! To create the POP3 session
int32_t rsi_pop3_session_create(uint8_t flags, uint8_t *server_ip_address, uint16_t server_port_number, 
                                uint8_t auth_type, uint8_t *username, uint8_t *password,
                                void(*rsi_pop3_response_handler)(uint16_t status, uint8_t type, uint8_t *buffer));

//! To delete the POP3 sssion
int32_t rsi_pop3_session_delete(void);

//! To get the Number of mails count and total size of the mails
int32_t rsi_pop3_get_mail_stats(void);

//! To get the mail size of the passed mail index
int32_t rsi_pop3_get_mail_list(uint16_t mail_index);

//! To get the mail header and mail body of the passed mail index
int32_t rsi_pop3_retrive_mail(uint16_t mail_index);

//! The mail will be marked as deleted on the POP3 server for the passed mail index
int32_t rsi_pop3_mark_mail(uint16_t mail_index);

//! Any messages/mails marked as deleted in the current session will unmarked
int32_t rsi_pop3_unmark_mail(void); /* or rsi_pop3_reset(void);*/

//! To get the connection status of the POP3 server
int32_t rsi_pop3_get_server_status(void);

#endif
