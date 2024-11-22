/*******************************************************************************
* @file  rsi_firmware_update_app.c
* @brief
*******************************************************************************
* # License
* <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/

/*================================================================================
 * @brief : This file contains example application for firmware upgradation from
 * server
 * @section Description :
 * This application demonstrates how to upgrade new firmware to Silicon Labs device
 * using remote TCP server.
 =================================================================================*/

/**
 * Include files
 * */

// include file to refer data types
#include "rsi_data_types.h"
#include "common.h"

// COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

// WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"
#include "rsi_wlan_non_rom.h"

// socket include file to refer socket APIs
#include "rsi_socket.h"

// Error include files
#include "rsi_error.h"

#include "rsi_bootup_config.h"
#include "rsi_utils.h"
// OS include file to refer OS specific functionality
#include "rsi_os.h"

// socket include file to firmware upgrade APIs
#include "rsi_firmware_upgradation.h"
#include "string.h"

// Receive data length
#define RECV_BUFFER_SIZE 1027

// Memory length for driver
#define GLOBAL_BUFF_LEN 15000

// Wlan task priority
#define RSI_APPLICATION_TASK_PRIORITY 1

// Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY 2

// Wlan task stack size
#define RSI_APPLICATION_TASK_STACK_SIZE 1024

// Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE 500

// Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

int32_t application()
{
  uint8_t ip_buff[20];
  int32_t client_socket;
  struct rsi_sockaddr_in server_addr, client_addr;
  int32_t status    = RSI_SUCCESS;
  int32_t recv_size = 0;
  uint8_t recv_buffer[RECV_BUFFER_SIZE];
  uint32_t chunk = 1, fwup_chunk_length, recv_offset = 0, fwup_chunk_type;

  // Driver initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > GLOBAL_BUFF_LEN)) {
    return status;
  }

  // Silicon Labs module intialisation
#ifdef TEST_TA_NONSECURE_FW
  status = rsi_device_init(BURN_NWP_FW);
#endif
#ifdef TEST_TA_SECURE_FW
  status = rsi_device_init(BURN_NWP_FW);
#endif
#ifdef TEST_M4_NONSECURE_FW
  status = rsi_device_init(BURN_M4_FW);
#endif
#ifdef TEST_M4_SECURE_FW
  status = rsi_device_init(BURN_M4_FW);
#endif
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nDevice Initialization Success\r\n");
}

// main function definition
int main(void)
{
  application();

  while (1) {
    rsi_wireless_driver_task();
  }
}
