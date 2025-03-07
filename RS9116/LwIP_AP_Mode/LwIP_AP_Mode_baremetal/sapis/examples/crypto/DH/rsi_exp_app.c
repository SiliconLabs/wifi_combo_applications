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
#define RSI_DH_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_DH_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

uint8_t prime[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc9, 0xf, 0xda, 0xa2, 0x21, 0x68, 0xc2, 0x34, 0xc4, 0xc6, 0x62, 0x8b, 0x80, 0xdc, 0x1c, 0xd1, 0x29, 0x2, 0x4e, 0x8, 0x8a, 0x67, 0xcc, 0x74, 0x2, 0xb, 
  0xbe, 0xa6, 0x3b, 0x13, 0x9b, 0x22, 0x51, 0x4a, 0x8, 0x79, 0x8e, 0x34, 0x4, 0xdd, 0xef, 0x95, 0x19, 0xb3, 0xcd, 0x3a, 0x43, 0x1b, 0x30, 0x2b, 0xa, 0x6d, 0xf2, 0x5f, 0x14, 0x37, 0x4f, 0xe1, 0x35, 0x6d, 0x6d, 
  0x51, 0xc2, 0x45, 0xe4, 0x85, 0xb5, 0x76, 0x62, 0x5e, 0x7e, 0xc6, 0xf4, 0x4c, 0x42, 0xe9, 0xa6, 0x37, 0xed, 0x6b, 0xb, 0xff, 0x5c, 0xb6, 0xf4, 0x6, 0xb7, 0xed, 0xee, 0x38, 0x6b, 0xfb, 0x5a, 0x89, 0x9f, 0xa5, 
  0xae, 0x9f, 0x24, 0x11, 0x7c, 0x4b, 0x1f, 0xe6, 0x49, 0x28, 0x66, 0x51, 0xec, 0xe4, 0x5b, 0x3d, 0xc2, 0x0, 0x7c, 0xb8, 0xa1, 0x63, 0xbf, 0x5, 0x98, 0xda, 0x48, 0x36, 0x1c, 0x55, 0xd3, 0x9a, 0x69, 0x16, 0x3f, 
  0xa8, 0xfd, 0x24, 0xcf, 0x5f, 0x83, 0x65, 0x5d, 0x23, 0xdc, 0xa3, 0xad, 0x96, 0x1c, 0x62, 0xf3, 0x56, 0x20, 0x85, 0x52, 0xbb, 0x9e, 0xd5, 0x29, 0x7, 0x70, 0x96, 0x96, 0x6d, 0x67, 0xc, 0x35, 0x4e, 0x4a, 0xbc, 
  0x98, 0x4, 0xf1, 0x74, 0x6c, 0x8, 0xca, 0x23, 0x73, 0x27, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t exp[] = {0x48, 0x7e, 0xdd, 0xd, 0x2f, 0x55, 0x95, 0x3c, 0xa5, 0x4d, 0xa5, 0xb7, 0xb5, 0xc5, 0x1f, 0x65, 0x3, 0x2a, 0xaa, 0xe, 0x28, 0xc0, 0xbc, 0xbe, 0xb4, 0xb0, 0x30, 0x28, 0x64, 0x5e, 0xde, 0x57, 0x49, 0x63, 
  0xc2, 0x99, 0x1f, 0x24, 0x7c, 0x5a, 0xc, 0xff, 0xba, 0x56, 0x40, 0x82, 0x17, 0x0, 0xc1, 0xcf, 0x69, 0x7f, 0xe0, 0x32, 0x64, 0x62, 0xe2, 0x85, 0x7, 0xea, 0xa, 0xb, 0xb0, 0x54, 0x6c, 0x7e, 0x49, 0x8c, 0x63, 
  0x33, 0xb9, 0x61, 0xf9, 0x26, 0x14, 0x8e, 0xe3, 0x2, 0xc5, 0x5c, 0x5e, 0xdc, 0x31, 0xc7, 0x68, 0x1c, 0x40, 0x1b, 0xd9, 0x13, 0x92, 0xf0, 0x22, 0x9c, 0x2a, 0x2d, 0x22, 0xb6, 0x74, 0xaf, 0x4a, 0x32, 0xc7, 0x0, 
  0x3b, 0x1c, 0xa5, 0xe, 0xb1, 0xc0, 0x82, 0xa7, 0xed, 0xba, 0x42, 0xad, 0x36, 0xf1, 0xe2, 0xfd, 0xb, 0xec, 0x72, 0x77, 0x85, 0x4a, 0xe7, 0x18, 0xc4, 0x1c, 0xd3, 0x6b, 0x65, 0xd4, 0x6a, 0xbd, 0x9c, 0x5e, 0x64, 
  0x70, 0x71, 0x7c, 0x2b, 0x3a, 0x62, 0x17, 0xf6, 0xa8, 0xf7, 0x64, 0xfa, 0x7a, 0x46, 0xc8, 0x40, 0xaa, 0x7b, 0xad, 0x14, 0xa7, 0x93, 0x30, 0x8a, 0x1c, 0x66, 0xfd, 0x93, 0x20, 0x6, 0x27, 0x8a, 0x47, 0xa0, 
  0x91, 0x98, 0xce, 0x42, 0x3e, 0x87, 0xff, 0x90, 0x1c, 0x7c, 0x87, 0xd1, 0x2, 0x52, 0x97, 0x0, 0x6a, 0x53, 0x13};
uint8_t base[] ={0x2};
uint8_t req_out[] = {0x6a, 0x90, 0x9c, 0xc9, 0x80, 0xab, 0xdf, 0xd9, 0x2e, 0x67, 0xdd, 0xbb, 0xd, 0x30, 0xe3, 0x52, 0x2f, 0x79, 0x74, 0x61, 0x8b, 0x67, 0xc5, 0xcc, 0x2c, 0x95, 0x2d, 0x78, 0x69, 0x3a, 0xba, 0xaa, 0x92, 0x10, 
  0xbf, 0x34, 0x9c, 0xb5, 0x3e, 0xd, 0xe1, 0x0, 0x75, 0x7d, 0xaa, 0x5b, 0xb0, 0x3f, 0x79, 0xbf, 0xe2, 0xf6, 0x52, 0xf2, 0xd0, 0xa4, 0x73, 0xa1, 0xcd, 0x1e, 0xc, 0x18, 0x79, 0x6b, 0xcd, 0x2d, 0x85, 0xef, 0xe3, 
  0xdd, 0xd1, 0x2, 0x3f, 0xdb, 0xba, 0x66, 0xfc, 0xfc, 0xef, 0x18, 0x41, 0xb4, 0x4b, 0x8d, 0x20, 0xef, 0xe5, 0xb1, 0xf6, 0xa8, 0x37, 0x75, 0x87, 0x76, 0xb1, 0xc, 0x9, 0x52, 0x30, 0x48, 0xe7, 0x6c, 0xdf, 0x8e, 
  0x19, 0x33, 0xbc, 0x64, 0x0, 0x4, 0x60, 0xaf, 0xaa, 0x3d, 0x33, 0x34, 0x21, 0x5c, 0x5e, 0x14, 0x4, 0x4e, 0x96, 0xdf, 0xfd, 0x72, 0x14, 0xa6, 0xb1, 0x66, 0xef, 0x9f, 0xdf, 0x70, 0x88, 0xa1, 0xd9, 0xf4, 0x80, 
  0x75, 0x54, 0x37, 0xb2, 0xa3, 0x3b, 0xe0, 0x1b, 0xb, 0x5, 0xae, 0xc4, 0x97, 0x52, 0xbd, 0xc7, 0x86, 0xcd, 0x71, 0x76, 0xd7, 0xed, 0x53, 0xb1, 0x4a, 0x1f, 0x4f, 0xc0, 0xb8, 0xcd, 0x9, 0xa2, 0x8f, 0xcf, 0x22, 
  0xb6, 0x3b, 0x7b, 0x43, 0xa0, 0x27, 0x58, 0x10, 0x96, 0xec, 0x8e, 0xf9, 0xda, 0xa1, 0xbc, 0xee, 0x64, 0xb7};

//! Function to calculate DH key 
int32_t rsi_crypto_exp_app()
{
	int32_t status = RSI_SUCCESS;

	//Buffers to store responses
  uint8_t exp_result[1000];
	
	//! WC initialization
	status = rsi_wireless_init(0, 0);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
	
	//! Memset before filling
	memset(exp_result, 0, 1000); //memset response buffers
	
	//!To generarte exponentiation key
    status = rsi_exponentiation(prime, sizeof(prime), base, sizeof(base), exp, sizeof(exp), exp_result );
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n Exponentiation Fail Status 0x%x \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n Exponentiation Done \n");
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

	rsi_task_handle_t dh_task_handle = NULL;

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
	//! Task created for DH key calculation task
	rsi_task_create( rsi_aes_encry_decry_app, "dh_task", RSI_DH_TASK_STACK_SIZE, NULL, RSI_DH_TASK_PRIORITY, &dh_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call DH key calculation application
	status = rsi_crypto_exp_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}
