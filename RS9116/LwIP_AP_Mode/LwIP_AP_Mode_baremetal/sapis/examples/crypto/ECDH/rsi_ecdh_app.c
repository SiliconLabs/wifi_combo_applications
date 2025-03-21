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

//! Include file for ECDH Apis
#include "rsi_crypto.h"

#ifdef RSI_ENABLE_DEBUG_PRINT
#include <stdio.h>
#endif

//! Receive data length
#define RECV_BUFFER_SIZE   1027

//! Memory length for driver
#define GLOBAL_BUFF_LEN   15000

//! Wlan task priority
#define RSI_ECDH_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_ECDH_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

/*256 Multiplication */
uint8_t pm_sx[32] = {0xc9,  0x97, 0xda, 0x89, 0x39, 0xab, 0x7c, 0xb7, 0xa0, 0x8f, 0x1a, 0x22, 0xb3, 0x19, 0x75, 0x61, 0x08, 0x15, 0x27, 0x0f, 0x7e, 0xd2, 0xed, 0x82, 0xe6, 0x36, 0x8d, 0xbc, 0xbe, 0x44, 0x24,0xde};
uint8_t pm_sy[32] = {0x56, 0xa2, 0x42, 0x30, 0x24, 0x0b, 0x35, 0xb6, 0x76, 0xc5, 0xce, 0x53, 0x0f, 0xe8, 0x2d, 0x70, 0x59, 0x66, 0xe6, 0xd1, 0xaa, 0xa5, 0x01, 0xfc, 0x80, 0x53, 0x6e, 0xf3, 0x7f, 0xae, 0x93, 0xc0};
uint8_t pm_sz[32] = {0x1};

uint8_t pm_d[32] = {0xfd,  0x06, 0xfb, 0x2f, 0x8b, 0x46, 0x22, 0x65, 0x8b, 0x70, 0x72, 0x30, 0x93, 0xa8, 0xc7, 0xd0, 0x8d, 0x3f, 0xf4, 0x92, 0xb9, 0xa5, 0xc6, 0xb6, 0xe6, 0xc1, 0xde, 0xaf, 0x53, 0x47, 0x1e, 0xc5};

uint8_t pm_rx[32] = {0x17, 0x2d, 0x63, 0x4e, 0x4f, 0x21, 0x07, 0x19, 0xf3, 0x87, 0xf5, 0x34, 0x5b, 0x15, 0xca, 0x8d, 0x0b, 0xb8, 0x10, 0xe9, 0x24, 0xb8, 0xe9, 0xdd, 0xb3, 0x0a, 0x52, 0x40, 0x3a, 0x0e, 0x90, 0x18};
uint8_t pm_ry[32] = {0x5a, 0x8d, 0x07, 0x55, 0x07, 0xb9, 0x00, 0xb4, 0xd9, 0x56, 0x01, 0x04, 0x77, 0xf5, 0x8a, 0x0d, 0xfd, 0xe4, 0xcf, 0x1b, 0xfb, 0xac, 0xb6, 0x12, 0x7b, 0x8a, 0x98, 0xf2, 0xd0, 0x88, 0x77, 0x0d};
uint8_t pm_rz[32] = {0x5a, 0x45, 0x50, 0x03, 0xeb, 0xb8, 0xd8, 0xd3, 0xda, 0x88, 0xa4, 0x61, 0xa5, 0x98, 0x7a, 0x37, 0x84, 0xb5, 0x53, 0x4c, 0x97, 0x68, 0x57, 0x97, 0xec, 0x04, 0x37, 0x80, 0x5c, 0xe1, 0x30, 0x66};

/* 256 addition and 256 subtraction */
uint8_t pa_sx[32] = {0xc9, 0x97, 0xda, 0x89, 0x39, 0xab, 0x7c, 0xb7, 0xa0, 0x8f, 0x1a, 0x22, 0xb3, 0x19, 0x75, 0x61, 0x08, 0x15, 0x27, 0x0f, 0x7e, 0xd2, 0xed, 0x82, 0xe6, 0x36, 0x8d, 0xbc, 0xbe, 0x44, 0x24, 0xde};
uint8_t pa_sy[32] = {0x56, 0xa2, 0x42, 0x30, 0x24, 0x0b, 0x35, 0xb6, 0x76, 0xc5, 0xce, 0x53, 0x0f, 0xe8, 0x2d, 0x70, 0x59, 0x66, 0xe6, 0xd1, 0xaa, 0xa5, 0x01, 0xfc, 0x80, 0x53, 0x6e, 0xf3, 0x7f, 0xae, 0x93, 0xc0};
uint8_t pa_sz[32] = {0x1};

uint8_t pa_tx[32] = {0x6b, 0x98, 0xe0, 0x35, 0x2e, 0xf9, 0x8c, 0xbb, 0x75, 0x95, 0xc8, 0x61, 0xc8, 0x0d, 0x54, 0x39, 0x2e, 0x21, 0x16, 0x53, 0xb2, 0xb3, 0xf6, 0x62, 0x4e, 0xd4, 0xa1, 0x8d, 0x0f, 0xb0, 0xa8, 0x55};
uint8_t pa_ty[32] = {0x16, 0x43, 0xb2, 0xc8, 0xd8, 0xe9, 0x56, 0xb6, 0x7a, 0x9e, 0x8b, 0x59, 0x52, 0x8a, 0x1a, 0xf6, 0x90, 0xdd, 0xc3, 0xc4, 0x2a, 0xd8, 0x35, 0x48, 0x70, 0x6c, 0x2d, 0x9c, 0x20, 0xc3, 0x21, 0x54};
uint8_t pa_tz[32] = {0x1};

uint8_t pa_rx[32] = {0xa3, 0xba, 0xfe, 0x8b, 0x04, 0xbd, 0xd0, 0x2d, 0xb6, 0xa0, 0x7e, 0x21, 0x0e, 0x6d, 0x21, 0x26, 0x10, 0x56, 0xc8, 0x71, 0xb7, 0x69, 0xed, 0xa1, 0xa0, 0xf8, 0x6a, 0x2e, 0x70, 0x59, 0xb9, 0x44};
uint8_t pa_ry[32] = {0x9e, 0xf0, 0xf5, 0x43, 0xfd, 0xe2, 0x15, 0xe1, 0x70, 0xe7, 0x20, 0xcf, 0xf5, 0xf1, 0xbb, 0x39, 0xd2, 0xc7, 0x24, 0x15, 0xdf, 0x21, 0x30, 0x7c, 0xf5, 0x03, 0x4d, 0x6b, 0x1f, 0x33, 0x20, 0x63};
uint8_t pa_rz[32] = {0x5e, 0xff, 0xf9, 0x53, 0x0b, 0xb2, 0xef, 0xfb, 0x2a, 0xfa, 0x51, 0xc0, 0xea, 0x0b, 0x21, 0x28, 0xda, 0xf3, 0x10, 0xbc, 0xcb, 0x1e, 0xf7, 0x1f, 0x98, 0x62, 0xeb, 0x2e, 0xaf, 0x94, 0x7b, 0x88};

uint8_t ps_rx[32] = {0xd3, 0x84, 0xe5, 0x80, 0x9e, 0x07, 0x6a, 0xd8, 0x68, 0xb4, 0x2e, 0x54, 0xee, 0xce, 0x3b, 0x6d, 0x7e, 0xeb, 0xfe, 0x86, 0xce, 0x64, 0x9e, 0x40, 0x50, 0x16, 0xf1, 0x5c, 0x23, 0xd7, 0x12, 0x1b};
uint8_t ps_ry[32] = {0x8b, 0xbb, 0xb5, 0xec, 0x5c, 0x55, 0x4f, 0xcd, 0x9b, 0x52, 0x4c, 0xd4, 0xa9, 0xc6, 0xde, 0x2d, 0x14, 0xcc, 0x87, 0xf0, 0xfc, 0x7a, 0x0d, 0x25, 0x5c, 0x72, 0xf4, 0xb8, 0x19, 0xde, 0x28, 0x43};
uint8_t ps_rz[32] = {0x5e, 0xff, 0xf9, 0x53, 0x0b, 0xb2, 0xef, 0xfb, 0x2a, 0xfa, 0x51, 0xc0, 0xea, 0x0b, 0x21, 0x28, 0xda, 0xf3, 0x10, 0xbc, 0xcb, 0x1e, 0xf7, 0x1f, 0x98, 0x62, 0xeb, 0x2e, 0xaf, 0x94, 0x7b, 0x88};

/* 256 double */
uint8_t pd_sx[32] = {0xc9, 0x97, 0xda, 0x89, 0x39, 0xab, 0x7c, 0xb7, 0xa0, 0x8f, 0x1a, 0x22, 0xb3, 0x19, 0x75, 0x61, 0x08, 0x15, 0x27, 0x0f, 0x7e, 0xd2, 0xed, 0x82, 0xe6, 0x36, 0x8d, 0xbc, 0xbe, 0x44, 0x24, 0xde};
uint8_t pd_sy[32] = {0x56, 0xa2, 0x42, 0x30, 0x24, 0x0b, 0x35, 0xb6, 0x76, 0xc5, 0xce, 0x53, 0x0f, 0xe8, 0x2d, 0x70, 0x59, 0x66, 0xe6, 0xd1, 0xaa, 0xa5, 0x01, 0xfc, 0x80, 0x53, 0x6e, 0xf3, 0x7f, 0xae, 0x93, 0xc0};
uint8_t pd_sz[32] = {0x1};

uint8_t pd_rx[32] = {0x57, 0xb1, 0x75, 0xe2, 0x22, 0xe8, 0xbc, 0x9e, 0x4e, 0x70, 0x02, 0x78, 0x7c, 0x7e, 0x36, 0xcc, 0x2b, 0xc5, 0x4d, 0x58, 0x87, 0xf5, 0x6b, 0xd4, 0x0f, 0xc5, 0x45, 0xcb, 0xdb, 0x5c, 0x30, 0x81};
uint8_t pd_ry[32] = {0x85, 0xa1, 0x25, 0xa6, 0x86, 0xb7, 0xa8, 0x16, 0x75, 0xe1, 0xe7, 0xf1, 0xd7, 0x79, 0xae, 0xdb, 0x69, 0x67, 0x0d, 0xce, 0xd4, 0x81, 0x58, 0x77, 0xb1, 0x53, 0xb3, 0x3d, 0x07, 0xc2, 0x32, 0xbb};
uint8_t pd_rz[32] = {0xad, 0x44, 0x85, 0x60, 0x48, 0x16, 0x6a, 0x6c, 0xed, 0x8a, 0x9d, 0xa7, 0x1d, 0xd0, 0x5b, 0xe0, 0xb2, 0xcc, 0xcc, 0xa3, 0x55, 0x4b, 0x03, 0xf8, 0x00, 0xa7, 0xdc, 0xe6, 0x00, 0x5d, 0x27, 0x81}; 

/* affinify vectors */
uint8_t sx[24] = {0x0b, 0xac, 0xf7, 0x91, 0xdf, 0xec, 0x14, 0x73, 0x69, 0xb0, 0x37, 0x07, 0x63, 0x46, 0x61, 0xbc, 0xfd, 0xce, 0xf5, 0x9e, 0x9d, 0x30, 0x05, 0x92};
uint8_t sy[24] = {0x36, 0x14, 0x11, 0x1d, 0xdb, 0x01, 0xd8, 0x61, 0xd2, 0x9c, 0xa3, 0x06, 0x12, 0x00, 0xd7, 0xd3, 0x45, 0x2a, 0x75, 0x37, 0xec, 0x8b, 0xe7, 0x9c};
uint8_t sz[24] = {0xa6, 0x77, 0x1e, 0x4d, 0xcf, 0x11, 0x72, 0x39, 0x8a, 0xf2, 0xd8, 0xe1, 0x22, 0x13, 0x6b, 0x43, 0xc8, 0x6f, 0x1a, 0x5d, 0x5c, 0xf4, 0x96, 0xb5};

//! Function performs the ECDH operations 
int32_t rsi_crypto_ecdh_app()
{
	int32_t status = RSI_SUCCESS;

	//! Buffers to store responses
	uint8_t rx[32] = {0};
	uint8_t ry[32] = {0};
	uint8_t rz[32] = {0};

	//! WC initialization
	status = rsi_wireless_init(0, 0);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

	//!ECDH point multiplication
	status = rsi_ecdh_point_multiplication(ECDH_256, pm_d, pm_sx, pm_sy, pm_sz, rx, ry, rz);
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n ECDH multiplication Fail Status 0x%x \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n ECDH multiplication Done \n");
#endif

	//!ECDH point addition
	status = rsi_ecdh_point_addition(ECDH_256, pa_sx, pa_sy, pa_sz, pa_tx, pa_ty, pa_tz, rx, ry, rz);
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n ECDH addition Fail Status 0x%x \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n ECDH addition Done \n");
#endif

	//!ECDH point subtraction
	status = rsi_ecdh_point_subtraction(ECDH_256, pa_sx, pa_sy, pa_sz, pa_tx, pa_ty, pa_tz, rx, ry, rz);
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n ECDH subtraction Fail Status 0x%x \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n ECDH subtraction Done \n");
#endif

	//!ECDH point double
	status = rsi_ecdh_point_double(ECDH_256, pd_sx, pd_sy, pd_sz, rx, ry, rz);
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n ECDH point double Fail Status 0x%x \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n ECDH point double Done \n");
#endif

	//!ECDH affinify
  status = rsi_ecdh_point_affine(ECDH_192, sx, sy, sz, rx, ry, rz);
	if(status != RSI_SUCCESS)
	{
#ifdef RSI_ENABLE_DEBUG_PRINT
		printf("\n ECDH affinity Fail Status 0x%x \n", status);
#endif
		return status;
	}
#ifdef RSI_ENABLE_DEBUG_PRINT
	printf("\n ECDH affinity Done \n");
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

	rsi_task_handle_t ecdh_task_handle = NULL;

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
	//! Task created for ECDH task
	rsi_task_create( rsi_aes_encry_decry_app, "ecdh_task", RSI_ECDH_TASK_STACK_SIZE, NULL, RSI_ECDH_TASK_PRIORITY, &ecdh_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call ECDH application
	status = rsi_crypto_ecdh_app();

	//! Application main loop
	main_loop();
#endif
	return status;

}
