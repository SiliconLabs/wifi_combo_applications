/**
 * @file    main.c
 * @version 0.1
 * @date    15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains example application for TCP server socket
 *
 *  @section Description  This file contains example application for TCP server socket 
 *
 *
 */

#include "rsi_driver.h"
#ifdef RSI_M4_INTERFACE
#include "rsi_chip.h"
#endif

//! Memory length for driver
#define GLOBAL_BUFF_LEN    15000

//! Wlan client mode 
#define RSI_WLAN_CLIENT_MODE    0

//! BT Coex mode 
#define RSI_BT_MODE           5

//! Parameter to run forever loop
#define RSI_FOREVER             1 

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

uint8_t bt_connected;
  int32_t status;
//! Function prototypes
extern void rsi_wlan_app_task(void);
extern void rsi_bt_app_init (void);
extern void rsi_bt_app_task (void);
#ifdef RSI_M4_INTERFACE
#define ICACHE2_ADDR_TRANSLATE_1_REG  *(volatile uint32_t *)(0x20280000 + 0x24)
#define MISC_CFG_SRAM_REDUNDANCY_CTRL *(volatile uint32_t *)(0x46008000 + 0x18)
#define MISC_CONFIG_MISC_CTRL1        *(volatile uint32_t *)(0x46008000 + 0x44)
#define MISC_QUASI_SYNC_MODE          *(volatile uint32_t *)(0x46008000 + 0x84)

#define SOC_PLL_REF_FREQUENCY         40000000  /*<! PLL input REFERENCE clock 40MHZ */ 
#define PS4_SOC_FREQ                  119000000 /*<! PLL out clock 100MHz            */
void switch_m4_frequency();
#endif

int32_t rsi_wlan_bt_app(void)
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
    return status;
  }

  //! WiSeConnect initialization
  status = rsi_wireless_init(RSI_WLAN_CLIENT_MODE, RSI_BT_MODE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! BT initialization
  rsi_bt_app_init();

  while(RSI_FOREVER)
  {
   
    //! BT application tasks
    rsi_bt_app_task();
 
    //! WLAN application tasks
   if(bt_connected)
   {
    rsi_wlan_app_task();
   }
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

#ifdef RSI_M4_INTERFACE
	switch_m4_frequency();
#endif
  



  //! Call WLAN BT application
  status = rsi_wlan_bt_app();

  //! Application main loop
  main_loop();

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
