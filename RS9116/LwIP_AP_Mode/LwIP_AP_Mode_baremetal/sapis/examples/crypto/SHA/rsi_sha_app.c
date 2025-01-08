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
#define RSI_SHA_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_SHA_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

#define SHA "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"
uint8_t digest_out[SHA_512_DIGEST_LEN] = {0x8e, 0x95, 0x9b, 0x75, 0xda, 0xe3, 0x13, 0xda, 0x8c, 0xf4, 0xf7, 0x28, 0x14, 0xfc, 0x14, 0x3f, 0x8f, 0x77, 0x79, 0xc6, 0xeb, 0x9f, 0x7f, 0xa1, 0x72, 0x99, 0xae, 0xad, 0xb6, 0x88, 0x90, 0x18, 0x50, 0x1d, 0x28, 0x9e, 0x49, 0x00, 0xf7, 0xe4, 0x33, 0x1b, 0x99, 0xde, 0xc4, 0xb5, 0x43, 0x3a, 0xc7, 0xd3, 0x29, 0xee, 0xb6, 0xdd, 0x26, 0x54, 0x5e, 0x96, 0xe5, 0x5b, 0x87, 0x4b, 0xe9, 0x09}; 

//! Function to calculate sha digest
int32_t rsi_crypto_sha_app()
{
	int32_t status = RSI_SUCCESS;

	//Buffers to store responses
	uint8_t digest[SHA_512_DIGEST_LEN];

	//! WC initialization
	status = rsi_wireless_init(0, 0);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//! Memset before filling
	memset(digest, 0, SHA_512_DIGEST_LEN);

	//!To compute digest using SHA
	status = rsi_sha(SHA_512, SHA, strlen(SHA), digest);
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n SHA Failed 0x%x \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n SHA success\n");
	printf("\n\n\nDone\n\n\n");
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
	rsi_task_handle_t sha_task_handle = NULL;

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
	//! Task created for SHA task
	rsi_task_create( rsi_aes_encry_decry_app, "sha_task", RSI_SHA_TASK_STACK_SIZE, NULL, RSI_SHA_TASK_PRIORITY, &sha_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call SHA digest computation application
	status = rsi_crypto_sha_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}
