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
 * @brief : This file contains example application for WLAN Station BLE Throughput
 * @section Description :
 * The coex application demonstrates throughput measurement of wifi while BLE is
 * in connection.
 * The coex application has WLAN and BLE tasks and acts as an interface between
 * Smartphone and PC. Smartphone interacts with BLE task, while Both PC and
 * Redpine WLAN would be connected to a Wireless Access Point, thus both are
 * connected together wirelessly interacts with WLAN task.
 * When Smartphone connects and sends message to Redpine device, BT task accepts.
 * Similarly, data transfer will happen for Station between AP.
 =================================================================================*/

/**
 * Include files
 **/

#include "rsi_driver.h"

//! Error include files
#include "rsi_error.h"
#ifdef RSI_M4_INTERFACE
#include "rsi_chip.h"
#include "rsi_board.h"
#endif

#ifdef RSI_WITH_OS
//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#endif

//!BLE Specific inclusions
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>

//! Memory length for driver
#define GLOBAL_BUFF_LEN    15000

#ifdef RSI_WITH_OS
//!Wlan BLE task priority
#define RSI_WLAN_BLE_TASK_PRIORITY 	1

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   2

//! Ble task priority
#define RSI_BLE_TASK_PRIORITY   3

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   4

//! Wlan BT task stack size
#define RSI_WLAN_BLE_TASK_STACK_SIZE 1000

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  1000

//! BT task stack size
#define RSI_BLE_TASK_STACK_SIZE 1000

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000

#endif

//! Wlan client mode 
#define RSI_WLAN_CLIENT_MODE    0

//! BTLE Coex mode 
#define RSI_BTLE_MODE           13

//! Parameter to run forever loop
#define RSI_FOREVER             1 
#ifdef RSI_M4_INTERFACE
#define ICACHE2_ADDR_TRANSLATE_1_REG  *(volatile uint32_t *)(0x20280000 + 0x24)
#define MISC_CFG_SRAM_REDUNDANCY_CTRL *(volatile uint32_t *)(0x46008000 + 0x18)
#define MISC_CONFIG_MISC_CTRL1        *(volatile uint32_t *)(0x46008000 + 0x44)
#define MISC_QUASI_SYNC_MODE          *(volatile uint32_t *)(0x46008000 + 0x84)

#define SOC_PLL_REF_FREQUENCY         40000000  /*<! PLL input REFERENCE clock 40MHZ */ 
#define PS4_SOC_FREQ                  119000000 /*<! PLL out clock 100MHz            */
void switch_m4_frequency(void);
#endif

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

uint8_t ble_connected;

//! Function prototypes
extern void rsi_wlan_app_task(void);
extern void rsi_ble_app_init (void);
extern void rsi_ble_app_task (void);

#ifdef RSI_WITH_OS
rsi_semaphore_handle_t commonsem,wlan_thread_sem,ble_thread_sem;

int32_t rsi_wlan_ble_app_init(void)
{
	int32_t     status = RSI_SUCCESS;
	rsi_task_handle_t wlan_task_handle = NULL;
	rsi_task_handle_t ble_task_handle = NULL;

	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_CLIENT_MODE, RSI_OPERMODE_WLAN_BLE);

	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWireless Initialization Success\r\n");
	}

	//! BLE initialization
	rsi_ble_app_init();

	//FIXME
	//rsi_wlan_app_init();

	//! Task created for WLAN task
	rsi_task_create(rsi_wlan_app_task, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

	//! Task created for BLE task
	rsi_task_create( rsi_ble_app_task, "ble_task",RSI_BLE_TASK_STACK_SIZE, NULL, RSI_BLE_TASK_PRIORITY , &ble_task_handle);

	rsi_semaphore_wait(&commonsem,0);
}

#endif

int32_t rsi_wlan_ble_app(void)
{
	int32_t     status = RSI_SUCCESS;

	//! WiSeConnect initialization
	status = rsi_wireless_init(RSI_WLAN_CLIENT_MODE, RSI_OPERMODE_WLAN_BLE);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWireless Initialization Success\r\n");
	}

	//! BLE initialization
	rsi_ble_app_init();

  while(RSI_FOREVER)
  {
    
    //! BLE application tasks
    rsi_ble_app_task();

    //! WLAN application tasks
    if(ble_connected)
    {
    rsi_wlan_app_task();
    ble_connected=0;
    }
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
#ifdef RSI_M4_INTERFACE
	switch_m4_frequency();
#endif

#ifdef RSI_WITH_OS
	rsi_task_handle_t wlan_ble_task_handle = NULL;
	rsi_task_handle_t driver_task_handle = NULL;
#endif



	//! Driver initialization
	status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
	if((status < 0) || (status > GLOBAL_BUFF_LEN))
	{
		return status;
	}

	//! Redpine module intialization
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

	//Start BT Stack
	intialize_bt_stack(STACK_BTLE_MODE);

#ifdef RSI_WITH_OS

	rsi_semaphore_create(&wlan_thread_sem,0);
	rsi_semaphore_create(&ble_thread_sem,0);
	rsi_semaphore_create(&commonsem,0);
	//! OS case
	//! Task created for COMMON task
	rsi_task_create( rsi_wlan_ble_app_init, "common_task", RSI_WLAN_BLE_TASK_STACK_SIZE, NULL, RSI_WLAN_BLE_TASK_PRIORITY, &wlan_ble_task_handle);

	//! Task created for Driver task
	rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

	//! OS TAsk Start the scheduler
	rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call WLAN BLE application
	status = rsi_wlan_ble_app();

	//! Application main loop
	main_loop();
#endif
	return status;
}
#ifdef RSI_M4_INTERFACE
void switch_m4_frequency()
{
	/*Switch M4 SOC clock to Reference clock*/
	/*Default keep M4 in reference clock*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_ULPREFCLK,0);
	/*Configure the PLL frequency*/
	RSI_CLK_SetSocPllFreq(M4CLK , PS4_SOC_FREQ , SOC_PLL_REF_FREQUENCY);
	/*Switch M4 clock to PLL clock for speed operations*/
	RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);	
}
#endif

