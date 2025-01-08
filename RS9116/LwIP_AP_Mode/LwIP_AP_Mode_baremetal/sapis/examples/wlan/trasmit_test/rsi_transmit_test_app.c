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

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! Transmit test power 
#define RSI_TX_TEST_POWER             4                      

//! Transmit test rate
#define RSI_TX_TEST_RATE              RSI_RATE_1              

//! Transmit test length
#define RSI_TX_TEST_LENGTH            30                      

//! Transmit test mode
#define RSI_TX_TEST_MODE              RSI_BURST_MODE      

//! Transmit test channel
#define RSI_TX_TEST_CHANNEL           1                       

//! Select Intenal antenna or uFL connector
#define RSI_ANTENNA           1

//! Antenna gain in 2.4GHz band
#define RSI_ANTENNA_GAIN_2G           0
                       
//! Antenna gain in 5GHz band
#define RSI_ANTENNA_GAIN_5G           0

//! Memory length for driver
#define GLOBAL_BUFF_LEN 15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500
//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Variable used to stop receiving stats
volatile uint8_t stop_receiving_stats = 0; 

//!variable used to initialise stats received
uint32_t stats_received = 0;


//! wlan per stats structure
typedef struct rsi_per_stats_rsp_s{
uint8_t tx_pkts[2];
uint8_t reserved_1[2];
uint8_t tx_retries[2];
uint16_t crc_pass;
uint16_t crc_fail;
uint8_t cca_stk[2];
uint8_t cca_not_stk[2];
uint8_t pkt_abort[2];
uint8_t fls_rx_start[2];
uint8_t cca_idle[2];
uint8_t reserved_2[26];
uint8_t rx_retries[2];
uint8_t reserved_3[2];
uint16_t cal_rssi;
uint8_t reserved_4[4];
uint8_t xretries[2];
uint8_t max_cons_pkts_dropped[2];
uint8_t reserved_5[2];
uint8_t bss_broadcast_pkts[2];
uint8_t bss_multicast_pkts[2];
uint8_t bss_filter_matched_multicast_pkts[2];
}rsi_per_stats_rsp_t;


//! call back to receive RX Stats data
void rsi_wlan_stats_receive_handler(uint16_t status, const uint8_t *buffer, const uint16_t length)
{

  rsi_per_stats_rsp_t *per_stats_p = (rsi_per_stats_rsp_t *)buffer;

  stats_received ++;

  //! For the buffer parameters information is available in SAPI's documentation
  if(stats_received == 10)
  {
    stop_receiving_stats = 1;
  }

}


int32_t rsi_transmit_test_app()
{
  int32_t     status       = RSI_SUCCESS;


  //! WC initialization
  status = rsi_wireless_init(8, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Register Wlan receive stats call back handler 
  rsi_wlan_register_callbacks(RSI_WLAN_RECEIVE_STATS_RESPONSE_CB,rsi_wlan_stats_receive_handler);

  //! Wlan radio init 
  status = rsi_wlan_radio_init();
  if(status != RSI_SUCCESS)
  {
    //! Return the status if error in sending command occurs
    return status;
  }

  //! To selct Internal antenna or uFL connector 
  status = rsi_wireless_antenna(RSI_ANTENNA, RSI_ANTENNA_GAIN_2G , RSI_ANTENNA_GAIN_5G);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! transmit test start
  status = rsi_transmit_test_start(RSI_TX_TEST_POWER, RSI_TX_TEST_RATE, RSI_TX_TEST_LENGTH,
                                    RSI_TX_TEST_MODE, RSI_TX_TEST_CHANNEL);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Add  delay here to see the TX packets on AIR
  rsi_delay_ms(1000);


  //! Stop TX transmit
  status = rsi_transmit_test_stop();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  ////////////////////////////////////////
  //! Transmit data/TX from the peer//////
  ////////////////////////////////////////


  //! Start/Recieve publishing RX stats  
  status =  rsi_wlan_receive_stats_start(RSI_TX_TEST_CHANNEL);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  
  while(!stop_receiving_stats)
  {
#ifndef RSI_WITH_OS
	  rsi_wireless_driver_task();
#endif
  }

  //! Stop Receiving RX stats
  status =  rsi_wlan_receive_stats_stop();
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  
  return status;
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

  rsi_task_handle_t wlan_task_handle = NULL;

  rsi_task_handle_t driver_task_handle = NULL;
#endif



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

#ifdef RSI_WITH_OS
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_transmit_test_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call Transmit test application
  status = rsi_transmit_test_app();

  //! Application main loop
  main_loop();
#endif
  return status;

}
