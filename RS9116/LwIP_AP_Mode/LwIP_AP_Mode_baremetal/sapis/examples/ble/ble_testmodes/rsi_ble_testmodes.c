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

//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>
#include<rsi_bt_common.h>

//! Common include file 
#include<rsi_common_apis.h>

#include <string.h>

#define RSI_SEL_ANTENNA                RSI_SEL_INTERNAL_ANTENNA//RSI_SEL_EXTERNAL_ANTENNA
#define RSI_BLE_TX_CHANNEL             0x10
#define RSI_BLE_RX_CHANNEL             0x10

#define RSI_BLE_1MBPS                  0x1
#define RSI_BLE_2MBPS                  0x2
#define RSI_BLE_125KBPS                0x3
#define RSI_BLE_500KBPS                0x4
#define RSI_BLE_TX_PHY                 RSI_BLE_1MBPS
#define RSI_BLE_RX_PHY                 RSI_BLE_1MBPS

#define RSI_BLE_TX_PAYLOAD_LEN         0x20


#define PRBS9_SEQ                     0x0
#define FOUR_ONES_FOUR_ZEROS          0x1
#define ALT_ONES_AND_ZEROS            0x2
#define PRBS15_SEQ                    0x3
#define ALL_ONES                      0x4
#define ALL_ZEROS                     0x5
#define FOUR_ZEROS_FOUR_ONES          0x6
#define ALT_ZERO_ALT_ONE              0x7


#define RSI_BLE_TX_PAYLOAD_TYPE       PRBS9_SEQ 


#define RSI_BLE_TESTMODE_TRANSMIT      1
#define RSI_BLE_TESTMODE_RECEIVE       2
#define RSI_CONFIG_TEST_MODE            RSI_BLE_TESTMODE_TRANSMIT


//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE  1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY   1

//! Number of packet to send or receive
#define NUMBER_OF_PACKETS 1000

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  3000

void rsi_wireless_driver_task(void);

#endif

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN] = {0};

//! Application global parameters.
static rsi_bt_resp_get_local_name_t  rsi_app_resp_get_local_name = {0};
static uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = {0};
static uint8_t rsi_app_resp_device_state = 0;
static uint8_t rsi_app_resp_rssi = 0;
static uint8_t rsi_app_async_event_map = 0;
static rsi_ble_event_conn_status_t rsi_app_connected_device = {0};
static rsi_ble_event_disconnect_t rsi_app_disconnected_device = {0};

/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events()
{
	rsi_app_async_event_map = 0;
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      sets the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
static void rsi_ble_app_set_event(uint32_t event_num)
{
	rsi_app_async_event_map |= BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_event(uint32_t event_num)
{
	rsi_app_async_event_map &= ~BIT(event_num);
	return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_ble_app_get_event(void)
{
	uint32_t  ix;

	for (ix = 0; ix < 32; ix++)
	{
		if (rsi_app_async_event_map & (1 << ix))
		{
		   return ix;
		}
	}

	return (RSI_FAILURE);
}

/*==============================================*/
/**
 * @fn         rsi_ble_testmode
 * @brief      Tests the BLE GAP peripheral role.
 * @param[in]  none
  * @return    none.
 * @section description
 * This function is used to test the BLE peripheral role and simple GAP API's.
 */
int32_t rsi_ble_testmode(void)
{
	int32_t status = 0;
	int32_t temp_event_map = 0;
	uint8_t remote_dev_addr[18] = {0};
	uint8_t adv[31] = {2,1,6};

#ifndef RSI_WITH_OS
	//! Driver initialization
	status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
	{
		return status;
	}

  //! Redpine module intialisation 
  status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		return status;
	}
#endif

	//! WC initialization
	status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
	if(status != RSI_SUCCESS)
	{
		return status;
	}

if(RSI_CONFIG_TEST_MODE == RSI_BLE_TESTMODE_TRANSMIT)
{

//! start the tx test mode in controller
  rsi_ble_tx_test_mode (RSI_BLE_TX_CHANNEL, /* channel number*/
      RSI_BLE_TX_PHY, /* phy - 1Mbps selected */
      RSI_BLE_TX_PAYLOAD_LEN, //255,  /* data_length */
      RSI_BLE_TX_PAYLOAD_TYPE); /* packet payload sequence */

}
else if(RSI_CONFIG_TEST_MODE == RSI_BLE_TESTMODE_RECEIVE)
{

rsi_ble_rx_test_mode (RSI_BLE_RX_CHANNEL, /* channel number*/
      RSI_BLE_RX_PHY, /* phy - 1Mbps selected */
      0x00); /* standard modulation */

}

	return 0;
}


/*==============================================*/
/**
 * @fn         main_loop
 * @brief      Schedules the Driver task.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function schedules the Driver task.
 */
void main_loop (void)
{
	while (1) {
		rsi_wireless_driver_task ();
	}
}

/*==============================================*/
/**
 * @fn         main
 * @brief      main function
 * @param[in]  none.
 * @return     none.
 * @section description
 * This is the main function to call Application
 */
int main (void)
{
  int32_t status;
#ifdef RSI_WITH_OS
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif

#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

#ifndef RSI_WITH_OS

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);
  
  //! Call BLE Peripheral application
  status = rsi_ble_testmode();

  //! Application main loop
  main_loop();

  return status;
#endif
#ifdef RSI_WITH_OS
 //! Driver initialization
  status = rsi_driver_init(global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! Redpine module intialisation
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_ble_peripheral, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif

}
