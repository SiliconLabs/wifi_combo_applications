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

#include "rsi_driver.h"

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN    15000


//! BT Coex mode 
#define RSI_BT_BLE_MODE           9

//! Parameter to run forever loop
#define RSI_FOREVER             1 

//! Memory to initialize driver
uint8_t global_buf[BT_GLOBAL_BUFF_LEN];

//! Function prototypes
extern void rsi_bt_app_init  (void);
extern void rsi_ble_app_init (void);
extern void rsi_bt_app_task  (void);
extern void rsi_ble_app_task (void);

void switch_proto_async(uint16_t mode, uint8_t *bt_disabled_status)
{
}


int32_t rsi_bt_ble_app(void)
{
  int32_t     status = RSI_SUCCESS;

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

  //! WiSeConnect initialization
  status = rsi_wireless_init(RSI_WLAN_CLIENT_MODE, RSI_BT_BLE_MODE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

	status = rsi_switch_proto(1, NULL);
	if(status != RSI_SUCCESS)
	{
		//return status;
	}


  //! BT initialization
  rsi_bt_app_init();

  //! BLE initialization
  rsi_ble_app_init();

  while(RSI_FOREVER)
  {
    //! BLE application tasks
    rsi_bt_app_task();

    //! BLE application tasks
    rsi_ble_app_task();

    //! wireless driver tasks
    rsi_wireless_driver_task();

  }
  return 0;
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



  //! Call BT BLE application
  status = rsi_bt_ble_app();

  //! Application main loop
  main_loop();

  return status;

}
