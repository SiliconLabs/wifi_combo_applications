/**
 * @file
 * @version
 * @date
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief :
 *
 *  @section Description  This file contains 
 *
 *
 */
#ifndef RSI_WEB_SOCKET_H
#define RSI_WEB_SOCKET_H

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
int32_t rsi_web_socket_create(int8_t flags, uint8_t *server_ip_addr, uint16_t server_port, uint16_t device_port,
                                    uint8_t *webs_resource_name, uint8_t *webs_host_name, int32_t *socket_id, 
                                    void (*web_socket_data_receive_notify_callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length));
int32_t rsi_web_socket_send_async(uint32_t sockID, uint8_t opcode, int8_t *msg, int32_t msg_length);
int32_t rsi_web_socket_close(int32_t sockID);
#endif

