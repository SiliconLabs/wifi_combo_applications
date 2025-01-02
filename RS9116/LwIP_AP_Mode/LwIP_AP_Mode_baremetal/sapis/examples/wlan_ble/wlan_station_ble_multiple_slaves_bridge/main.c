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

/*================================================================================
 * @brief : This file contains example application for Wlan Station BLE Multiple
 * Slaves Bridge
 * @section Description :
 * The coex application demonstrates how information can be exchanged seamlessly
 * using two wireless protocols (WLAN and BLE) running in the same device.
 =================================================================================*/

/**
 * Include files
 **/

#include "rsi_driver.h"
#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//! Memory length for driver
#define GLOBAL_BUFF_LEN    15000

//! Wlan client mode 
#define RSI_WLAN_CLIENT_MODE    0

//! BTLE Coex mode 
#define RSI_BTLE_MODE           13

//! Parameter to run forever loop
#define RSI_FOREVER             1 

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Function prototypes
extern void rsi_wlan_app_task(void);
extern int32_t rsi_wlan_app_init (void);
extern void rsi_ble_app_init (void);
extern void rsi_ble_app_task (void);

int32_t rsi_wlan_ble_app(void)
{
	int32_t     status = RSI_SUCCESS;

	//! Driver initialization
	status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
	if((status < 0) || (status > GLOBAL_BUFF_LEN))
	{
		return status;
	}

	//! Redpine module intialisation
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nDevice Initialization Success\r\n");
	}

	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_CLIENT_MODE, RSI_BTLE_MODE);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWireless Initialization Success\r\n");
	}

	//! WLAN initialization
	status =  rsi_wlan_app_init();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN App Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWLAN App Initialization Success\r\n");
	}


	//! BLE initialization
	rsi_ble_app_init();

	while(RSI_FOREVER)
	{
		//! WLAN application tasks
		rsi_wlan_app_task();

		//! BLE application tasks
		rsi_ble_app_task();

		//! wireless driver tasks
		rsi_wireless_driver_task();

  }
}

//! Forever in wireless driver task 
void main_loop()
{
	while (RSI_FOREVER)
	{
		rsi_wireless_driver_task ();
	}
}

//! main funtion definition
int main(void)
{
	int32_t status;

	//! Call WLAN BLE application
	status = rsi_wlan_ble_app();

	//! Application main loop
	main_loop();

	return status;

}
