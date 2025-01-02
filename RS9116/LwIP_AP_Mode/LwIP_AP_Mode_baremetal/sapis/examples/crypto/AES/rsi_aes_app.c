/*************************************************************************
 * 
 * Copyright (c) 2019 Redpine Signals Incorporated. All Rights Reserved.
 * 
 * NOTICE:  All  information  contained  herein is, and  remains  the  property of 
 * Redpine Signals Incorporated. The intellectual and technical concepts contained
 * herein  are  proprietary to  Redpine Signals Incorporated and may be covered by 
 * U.S. and Foreign Patents, patents in process, and are protected by trade secret
 * or copyright law. Dissemination  of this  information or reproduction  of  this
 * material is strictly forbidden unless prior written permission is obtained from
 * Redpine Signals Incorporated.
 */

/**
 * Include files
 * */

//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h" 
#include "rsi_wlan_non_rom.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Include file for crypto Apis
#include "rsi_crypto.h"

#ifdef RSI_ENABLE_DEBUG_PRINT
#include <stdio.h>
#endif

//! Receive data length
#define RECV_BUFFER_SIZE   1027

//! Memory length for driver
#define GLOBAL_BUFF_LEN   15000

//! Wlan task priority
#define RSI_AES_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_AES_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

/*AEC-CTR 256*/
uint8_t key_ctr[] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};

uint8_t iv_ctr[] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

uint8_t msg_ctr[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};

uint8_t cipher_ctr[] = {0x60, 0x1e, 0xc3, 0x13, 0x77, 0x57, 0x89, 0xa5, 0xb7, 0xa7, 0xf5, 0x04, 0xbb, 0xf3, 0xd2, 0x28};

// AES ECB/CBC 256 
uint8_t key[] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};

uint8_t iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

uint8_t msg[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};

uint8_t cipher_cbc[] = {0xf5, 0x8c, 0x4c, 0x04, 0xd6, 0xe5, 0xf1, 0xba, 0x77, 0x9e, 0xab, 0xfb, 0x5f, 0x7b, 0xfb, 0xd6};

uint8_t cipher_ecb[] = {0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c, 0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8};

//! Function to use AES to encrypt and decrypt data 
int32_t rsi_crypto_aes_app()
{
  int32_t status = RSI_SUCCESS;
  
	//Buffers to store responses
  uint8_t aes_encry_data[256];
  uint8_t aes_decry_data[256];
  
	//! WC initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
	//!Encrypt using AES CBC mode 256bit Key
  memset(aes_encry_data, 0, 256); //memset response buffers
  status = rsi_aes(CBC_MODE, AES_ENCRYPTION, msg, sizeof(msg), key, AES_KEY_SIZE_256, iv, aes_encry_data);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES CBC Encryption Fail Status 0x%x \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Encryption Done with AES CBC mode \n");
#endif

  //!Decrypt using AES-CBC mode, 256bit key
  memset(aes_decry_data, 0, 256); //memset response buffers
  status = rsi_aes(CBC_MODE, AES_DECRYPTION, aes_encry_data, sizeof(msg) , key, AES_KEY_SIZE_256, iv, aes_decry_data);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES CBC Decryption Fail Status 0x%x \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES CBC mode \n ");
#endif

  //!Encrypt using AES ECB mode 256bit Key
  memset(aes_encry_data, 0, 256); //memset response buffers
  status = rsi_aes(ECB_MODE, AES_ENCRYPTION, msg, sizeof(msg), key, AES_KEY_SIZE_256, NULL, aes_encry_data);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES ECB Encryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Encryption Done with AES ECB mode \n");
#endif

  //!Decrypt using AES-CBC mode, 256bit key
  memset(aes_decry_data, 0, 256); //memset response buffers
  status = rsi_aes(ECB_MODE, AES_DECRYPTION, aes_encry_data, sizeof(msg) , key, AES_KEY_SIZE_256, NULL, aes_decry_data);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES ECB Decryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES ECB mode \n ");
#endif

  //!Encrypt using AES CTR mode 256bit Key
  memset(aes_encry_data, 0, 256); //memset response buffers
  status = rsi_aes(CTR_MODE, AES_ENCRYPTION, msg_ctr, sizeof(msg_ctr), key_ctr, AES_KEY_SIZE_256, iv_ctr, aes_encry_data);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES CTR Encryption Fail Status %d \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Encryption Done with AES CTR mode  \n");
#endif

  //!Decrypt using AES-CTR mode, 256bit key
  memset(aes_decry_data, 0, 256); //memset response buffers
  status = rsi_aes(CTR_MODE, AES_DECRYPTION, aes_encry_data, sizeof(msg_ctr) , key_ctr, AES_KEY_SIZE_256, iv_ctr, aes_decry_data);
  if(status != RSI_SUCCESS)
  {
#ifdef RSI_ENABLE_DEBUG_PRINT
    printf("\n AES CTR Decryption Fail Status 0x%x \n", status);
#endif
    return status;
  }
#ifdef RSI_ENABLE_DEBUG_PRINT
  printf("\n Decryption Done with AES CTR mode \n ");
#endif

  return 0;
}

void main_loop(void)
{
	while(1)
	{
		////////////////////////
		//! Application code ///
		////////////////////////

		//! event loop
		rsi_wireless_driver_task();

	}
}

int main()
{
	int32_t status;

#ifdef RSI_WITH_OS

	rsi_task_handle_t aes_task_handle = NULL;

	rsi_task_handle_t driver_task_handle = NULL;
#endif



	//! Driver initialization
	status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
	if((status < 0) || (status > GLOBAL_BUFF_LEN))
	{
		return status;
	}

	//! RS9116 intialisation
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

#ifdef RSI_WITH_OS
	//! OS case
	//! Task created for AES task
	rsi_task_create( rsi_aes_encry_decry_app, "aes_task", RSI_AES_TASK_STACK_SIZE, NULL, RSI_AES_TASK_PRIORITY, &aes_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call aes encryption and decryption application
	status = rsi_crypto_aes_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}
