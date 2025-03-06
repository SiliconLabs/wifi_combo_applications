/***************************************************************************/ /**
 * @file
 * @brief Secure storage Application
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "cmsis_os2.h"
#include "sl_si91x_driver.h"
#include <stdint.h>
#include <stdio.h>
/******************************************************
 *                    Constants
 ******************************************************/
// Register base address for MCU storage
#define MCU_STORAGE_WRITE_KEY  *(volatile uint32_t *)(0x24048700)  // Address of the MCU_STORAGE_KEY register
#define MCU_STORAGE_REG0     *(volatile uint32_t *)(0x24048580)  // Address of the MCU_STORAGE_REG0 register


// Define the ENABLE_KEY value
#define ENABLE_KEY           0x91437B2B
#define DISABLE_KEY          0xCCCCCCCC


/******************************************************
 *               Variable Definitions
 ******************************************************/
const osThreadAttr_t thread_attributes = {
  .name       = "app",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityLow,
  .tz_module  = 0,
  .reserved   = 0,
};

/******************************************************
 *               Function Declarations
 ******************************************************/
static void application_start(void *argument);

/******************************************************
 *               Function Definitions
 ******************************************************/
void enable_mcu_storage_access() {
    // Write the ENABLE_KEY value to the MCU_STORAGE_KEY register
    MCU_STORAGE_WRITE_KEY = ENABLE_KEY;

    // Print the status and confirm the key written
    printf("Access to MCU storage enabled. Key programmed:0x%08X\n", ENABLE_KEY);
}


// Function to disable access to MCU_STORAGE_KEY
void disable_mcu_storage_access() {
    // Write the DISABLE_KEY value to the MCU_STORAGE_KEY register
    MCU_STORAGE_WRITE_KEY = DISABLE_KEY;

    // Print the status and confirm the key written
    printf("Access to MCU storage disabled. Key programmed:0x%08X\n", DISABLE_KEY);
}


// Function to write encrypted data to MCU storage register
void write_encrypted_to_mcu_storage(uint32_t data, uint32_t encryption_key) {
    // Encrypt the data (XOR)
    uint32_t encrypted_data = data ^ encryption_key;  // XOR encryption

    // Print encrypted data for debugging
    printf("Encrypted data:%lX\n", encrypted_data);

    // Write the encrypted data to the storage register
    MCU_STORAGE_REG0 = encrypted_data;
    printf("Encrypted data written to MCU storage register:%lX\n", MCU_STORAGE_REG0);
}


// Function to read and decrypt data from MCU storage register
void read_decrypted_from_mcu_storage(uint32_t encryption_key) {
    // Read the encrypted data
    uint32_t encrypted_data = MCU_STORAGE_REG0;

    // Decrypt the data (XOR)
    uint32_t decrypted_data = encrypted_data ^ encryption_key;  // XOR decryption

    // Print decrypted data
    printf("Decrypted data retrieved:%lX\n", decrypted_data);
}

void app_init(const void *unused)
{
  UNUSED_PARAMETER(unused);
  osThreadNew((osThreadFunc_t)application_start, NULL, &thread_attributes);
}

static void application_start(void *argument)
{
  UNUSED_PARAMETER(argument);
  uint32_t data = 0x12345678;
  uint32_t data2 = 0x22334455;

   // Encryption key (32-bit)
   uint32_t encryption_key = 0x87654321;
   printf("Data to be written to the MCU storage register = %lx\n", data);
   // Enable access to the MCU storage
   enable_mcu_storage_access();
   // Try writing data without enabling access
   write_encrypted_to_mcu_storage(data, encryption_key);
   printf("Register value before sleep= %lx\n", MCU_STORAGE_REG0);
   osDelay(20000);
   printf("Register value after sleep= %lx\n", MCU_STORAGE_REG0);
   // Read back and decrypt the data
   read_decrypted_from_mcu_storage(encryption_key);
   // Disable access again
   disable_mcu_storage_access();
   printf("Data2 to be written to the MCU storage register after disabling the secure mode = %lx\n", data2);
   write_encrypted_to_mcu_storage(data2, encryption_key);
   if (data2!= MCU_STORAGE_REG0)
     {
       printf("As the secure mode is disabled and the register value cannot be overwritten\n");
     }

 }
