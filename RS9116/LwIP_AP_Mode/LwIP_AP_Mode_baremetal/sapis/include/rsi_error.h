/**
 * @file         rsi_error.h
 * @version      0.1 
 * @date         15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains the errors used in the driver
 *
 *  @section Description   This file contains the errors used in the driver
 *
 *
 */
#ifndef RSI_ERROR_H
#define RSI_ERROR_H
/******************************************************
 * *                      Macros
 * ******************************************************/
#define RSI_ERROR_REMOTE_TERNIMATE 0xFF62
/******************************************************
 * *                    Constants
 * ******************************************************/
/******************************************************
 * *                   Enumerations
 * ******************************************************/
//! enumeration for error codes used in driver
typedef enum rsi_error_e
{
  RSI_ERROR_NONE =0,
  RSI_ERROR_TIMEOUT                         = -1, //! Time out error
  RSI_ERROR_INVALID_PARAM                   = -2, //! Invalid param error
  RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE    = -3,
  RSI_ERROR_PKT_ALLOCATION_FAILURE          = -4,
  RSI_ERROR_COMMAND_NOT_SUPPORTED           = -5,
  RSI_ERROR_INSUFFICIENT_BUFFER             = -6,
  RSI_ERROR_IN_OS_OPERATION                 = -7,
  RSI_ERROR_INVALID_MEMORY                  = -8,
  RSI_ERROR_BOOTUP_OPTIONS_NOT_SAVED        = -9,
  RSI_ERROR_BOOTUP_OPTIONS_CHECKSUM_FAIL    = -10,
  RSI_ERROR_BOOTLOADER_VERSION_NOT_MATCHING = -11,
  RSI_ERROR_WAITING_FOR_BOARD_READY         = -12,
  RSI_ERROR_INVALID_ADDRESS                 = -13,
  RSI_ERROR_VALID_FIRMWARE_NOT_PRESENT      = -14,
  RSI_ERROR_INVALID_OPTION                  = -15,
  RSI_ERROR_EXCEEDS_MAX_CALLBACKS           = -16,
  RSI_ERROR_SET_ITIMER                      = -17,
  RSI_ERROR_SIG_ACTION                      = -18,
  RSI_ERROR_NOT_IN_CONNECTED_STATE          = -19,
  RSI_ERROR_NOT_IN_IPCONFIG_STATE           = -20,
  RSI_ERROR_SPI_BUSY                        = -21,
  RSI_ERROR_SPI_FAIL                        = -22,
  RSI_ERROR_SPI_TIMEOUT                     = -23,
  RSI_ERROR_CARD_READY_TIMEOUT              = -24,
  RSI_ERROR_BOARD_READY_TIMEOUT             = -25,
  RSI_ERROR_INVALID_PACKET                  = -26,
  RSI_ERROR_FW_UPGRADE_TIMEOUT              = -27,
  RSI_ERROR_FW_LOAD_OR_UPGRADE_TIMEOUT      = -28,
  RSI_ERROR_GPIO_WAKEUP_TIMEOUT             = -29,
  RSI_ERROR_RESPONSE_TIMEOUT                = -30,
  RSI_ERROR_BLE_DEV_BUF_FULL                = -31,
  RSI_ERROR_NWK_CMD_IN_PROGRESS             = -32,
  RSI_ERROR_SOCKET_CMD_IN_PROGRESS          = -33,
  RSI_ERROR_WLAN_CMD_IN_PROGRESS            = -34,
  RSI_ERROR_COMMON_CMD_IN_PROGRESS          = -35,
  RSI_ERROR_ANT_DEV_BUF_FULL                = -36,
  RSI_ERROR_BT_BLE_CMD_IN_PROGRESS          = -37,
  RSI_ERROR_BLE_ATT_CMD_IN_PROGRESS         = -38,
  RSI_ERROR_MEMORY_NOT_ALIGNED              = -39
}rsi_error_t;


//! Firmware load request Error codes
#define INVALID_FIRMWARE_PRESENT      -2
#define INVALID_OPTION                -3
#define TIMER_EXPIRED                 -4

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



#endif
