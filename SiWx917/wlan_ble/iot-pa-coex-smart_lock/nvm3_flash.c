/*******************************************************************************
 * @file  nvm3_app.c
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

#include <nvm3_flash.h>
#include "rsi_debug.h"

uint8_t magic_word = 0;
extern ap_cred_t ap_credentials;

void initialise_counters(void)
{
  uint32_t type;
  size_t len;
  Ecode_t err;

  // check if the designated keys contain counters, and initialise if needed.
  err = nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, &type, &len);
  if ((err != ECODE_NVM3_OK) || (type != NVM3_OBJECTTYPE_COUNTER)) {
    nvm3_writeCounter(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, 0);
  }

  err = nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, DELETE_COUNTER_KEY, &type, &len);
  if ((err != ECODE_NVM3_OK) || (type != NVM3_OBJECTTYPE_COUNTER)) {
    nvm3_writeCounter(NVM3_DEFAULT_HANDLE, DELETE_COUNTER_KEY, 0);
  }
}

void nvm3_app_write(uint32_t key, unsigned char *data, uint32_t len)
{
  do {
    // check for NVM3 maximum key value
    if (key > MAX_DATA_KEY) {
      LOG_PRINT("Invalid key\r\n");
      break;
    }
    // check for NVM3 write success or not
    if (ECODE_NVM3_OK == nvm3_writeData(NVM3_DEFAULT_HANDLE, key, (unsigned char *)data, len)) {
//      LOG_PRINT("Stored data at key %lu\r\n", key);
      // Track number of writes in counter object
      nvm3_incrementCounter(NVM3_DEFAULT_HANDLE, WRITE_COUNTER_KEY, NULL);
    } else {
      LOG_PRINT("Error storing data\r\n");
    }
  } while (false);

  return;
}

Ecode_t nvm3_app_read(nvm3_ObjectKey_t key, uint8_t* buffer)
{
  uint32_t type;
  size_t len;
  Ecode_t err=0;

  do {
    // check for NVM3 maximum key value
    if (key > MAX_DATA_KEY) {
      LOG_PRINT("Invalid key\r\n");
      break;
    }
    err = nvm3_getObjectInfo(NVM3_DEFAULT_HANDLE, key, &type, &len);
    if (err != NVM3_OBJECTTYPE_DATA || type != NVM3_OBJECTTYPE_DATA) {
//      LOG_PRINT("Key does not contain data object\r\n");
      break;
    }
    err = nvm3_readData(NVM3_DEFAULT_HANDLE, key, buffer, len);
    // check for error code
    if (ECODE_NVM3_OK == err) {
      buffer[len] = '\0';
      LOG_PRINT("%s\r\n", buffer);
    } else {
      LOG_PRINT("Error reading data from key %lu\r\n", key);
    }
  } while (false);

  return err;
}

void nvm3_app_delete(uint32_t key)
{
  if (key > MAX_DATA_KEY) {
    LOG_PRINT("Invalid key\r\n");
  } else {
    // check for NVM3 delete object success or not
    if (ECODE_NVM3_OK == nvm3_deleteObject(NVM3_DEFAULT_HANDLE, key)) {
//      LOG_PRINT("Deleted data at key %lu\r\n", key);
      // Track number or deletes in counter object
      nvm3_incrementCounter(NVM3_DEFAULT_HANDLE, DELETE_COUNTER_KEY, NULL);
    } else {
//      LOG_PRINT("Error deleting key\r\n");
    }
  }
  return;
}

void store_credentials_to_nvm(unsigned char *ssid,unsigned char *pwd,sl_wifi_security_t security_type)
{
  uint8_t data_saved[1] = {0};
  uint8_t sec_type[1] = {0};

  sec_type[0]=security_type;

  LOG_PRINT("\r\nstore wifi credentials in nvm\r\n");
  nvm3_app_delete(SSID_NVM_KEY);
  nvm3_app_write(SSID_NVM_KEY, ssid, strlen((char *)ssid));

  nvm3_app_delete(PWD_NVM_KEY);
  nvm3_app_write(PWD_NVM_KEY, pwd, strlen((char *)pwd));

  nvm3_app_delete(SEC_TYPE_NVM_KEY);
  nvm3_app_write(SEC_TYPE_NVM_KEY,sec_type,1);

  data_saved[0]=1;
  nvm3_app_write(SAVE_CREDENTIALS_NVM_KEY,data_saved,1);
}

uint8_t read_credentials_from_nvm(ap_cred_t *ap_credentials)
{
  uint8_t save_flash[1] = {0};
  uint8_t sec_type[1] = {0};

  if(nvm3_app_read(SAVE_CREDENTIALS_NVM_KEY,save_flash) == ECODE_NVM3_OK)
  {
    magic_word = save_flash[0];
    if(magic_word == 1)
    {
      LOG_PRINT("\nAP credentials already present in nvm\r\n");

      nvm3_app_read(SSID_NVM_KEY,ap_credentials->ssid);
      nvm3_app_read(PWD_NVM_KEY,ap_credentials->pwd);
      nvm3_app_read(SEC_TYPE_NVM_KEY,sec_type);
      ap_credentials->sec_type = sec_type[0];

      return 1;
    }
  }
  return 0;
}

void erase_credentials(void)
{
#if 0 //to delete all data in NVM3
  Ecode_t err = nvm3_eraseAll(NVM3_DEFAULT_HANDLE);
  if (ECODE_NVM3_OK == err) {
    LOG_PRINT("Deleting all data stored in NVM3\r\n");
  }
#endif
  LOG_PRINT("Deleting all data stored in NVM3\r\n");
  nvm3_app_delete(SSID_NVM_KEY);
  nvm3_app_delete(PWD_NVM_KEY);
  nvm3_app_delete(SEC_TYPE_NVM_KEY);
  nvm3_app_delete(SAVE_CREDENTIALS_NVM_KEY);
}

