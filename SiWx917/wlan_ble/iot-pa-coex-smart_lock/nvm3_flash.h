/*******************************************************************************
 * @file  nvm3_app.h
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
/*************************************************************************
 *
 */
#ifndef NVM3_FLASH_H_
#define NVM3_FLASH_H_

//! NVM dependencies
#include "nvm3_default.h"
#include "nvm3_default_config.h"
#include <wifi_app.h>
#include <string.h>

#define NVM3_DEFAULT_HANDLE nvm3_defaultHandle

// Maximum number of data objects saved
#define MAX_OBJECT_COUNT 10

// Max and min keys for data objects
#define MIN_DATA_KEY NVM3_KEY_MIN
#define MAX_DATA_KEY (MIN_DATA_KEY + MAX_OBJECT_COUNT - 1)

// Key of write counter object
#define WRITE_COUNTER_KEY MAX_OBJECT_COUNT

// Key of delete counter object
#define DELETE_COUNTER_KEY (WRITE_COUNTER_KEY + 1)

void initialise_counters(void);
void nvm3_app_write(uint32_t key, unsigned char *data, uint32_t len);
Ecode_t nvm3_app_read(nvm3_ObjectKey_t key, uint8_t* buffer);
void nvm3_app_delete(uint32_t key);
void store_credentials_to_nvm(unsigned char *ssid,unsigned char *pwd,sl_wifi_security_t sec_type);
void erase_credentials(void);
uint8_t read_credentials_from_nvm(ap_cred_t *ap_credentials);

#endif /* NVM3_FLASH_H_ */
