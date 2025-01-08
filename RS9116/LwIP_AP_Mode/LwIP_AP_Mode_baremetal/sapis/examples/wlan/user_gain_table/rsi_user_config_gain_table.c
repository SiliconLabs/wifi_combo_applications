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

//! Include files

//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

//! WLAN include file for configuration
#include "rsi_wlan_config.h"

//! Preprocessor Defines

//! Transmit test power 
#define RSI_TX_TEST_POWER             127                      

//! Transmit test length
#define RSI_TX_TEST_LENGTH            1000  

//! Transmit test mode
#define RSI_TX_TEST_MODE              RSI_BURST_MODE  

//! Select Intenal antenna or uFL connector
#define RSI_ANTENNA                   1

//! Antenna gain in 2.4GHz band
#define RSI_ANTENNA_GAIN_2G           0
                       
//! Antenna gain in 5GHz band
#define RSI_ANTENNA_GAIN_5G           0

//! Memory length for driver
#define GLOBAL_BUFF_LEN               15000  

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY        1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY      2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE      500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE    500

//! Max payload len for gain table
#define MAX_PAYLOAD_LEN               128

#define RSI_BW_20MHZ                  0

#define RSI_BW_40MHZ                  1

//! Transmit test channel
#define RSI_TX_TEST_CHANNEL           11     //! Ex: 1,6,11 for 2.4Ghz, 36,40,149 for 5Ghz 

//! Transmit test rate
#define RSI_TX_TEST_RATE              RSI_RATE_6 

#if (RSI_BAND == RSI_BAND_2P4GHZ)
#define BAND                          1 
#elif (RSI_BAND == RSI_BAND_5GHZ)
#define BAND                          2
#endif

#define RSI_BANDWIDTH                 RSI_BW_20MHZ

//! Transmission packets duration on AIR
#define TRANSMIT_TIME                 10000  //! Time in milliseconds


#define FCC     0
#define ETSI    1
#define TELEC   2
#define WORLDWIDE 3
#define KCC     4

//! Global Variables

#if (RSI_BAND == RSI_BAND_2P4GHZ)			
#if (RSI_BANDWIDTH == RSI_BW_20MHZ)
uint8_t gain_table_payload[] = {3,  0, 13,
                                1, 34, 20, 20,
                                2, 34, 28, 28,
                                3, 34, 32, 32,
                                4, 34, 36, 36,
                                5, 34, 38, 38,
                                6, 34, 40, 40,
                                7, 34, 38, 38,
                                8, 34, 36, 36,
                                9, 34, 32, 32,
                               10, 34, 32, 32,
                               11, 34, 24, 24,
                               12, 34, 16, 24,
                               13, 34, 12, 12,
                                2, 17,
                              255, 20, 16, 16,
                                4, 17,
                              255, 26, 20, 20};
#endif
#elif (RSI_BAND == RSI_BAND_5GHZ)		
#if (RSI_BANDWIDTH == RSI_BW_20MHZ)
uint8_t gain_table_payload[] =  {2, 0,  6,
                                 1, 9, 10,
                                 2, 8,  9,
                               100, 4,  4,
                                 3, 6,  8,
                               149, 3,  3,
                                 4, 6,  7,
                                 2, 4, 
                                 1, 9, 10,
                                 2, 8, 10,
                                 3, 6,  8,
                                 4, 6,  7};
#elif (RSI_BANDWIDTH == RSI_BW_40MHZ) 
uint8_t gain_table_payload[] = {2, 0,  8,    
                                1, 9, 10,
                               62, 8,  9,
                                2, 8,  9,
                              102, 4,  4,
                              134, 6,  8,
                                3, 6,  8,
                              151, 3,  3,
                                4, 6,  7,
                                2, 4,  
                                1, 9, 10,
                                2, 8, 10,
                                3, 6,  8,
                                4, 6,  7};
#endif
#endif



//M7DB - MARS ANTENNA
/******WLAN_20MHz*****/
uint8_t _RS9116_HP_MODE_REGION_BASED_RATE_VS_MAXPOWER_SINGLE_BAND_NONHPM_XF6_M7DB7_1P4_MARS_ANT[] = {//{{{
3,//NUM_OF_REGIONS
    FCC, 0xD,//NUM_OF_CHANNELS
//   rate,  11b, 11g, 11n
        1,  28,  32,  30,
        2,  28,  32,  30,
        3,  28,  32,  30,
        4,  30,  28,  34,
        5,  30,  28,  34,
        6,  30,  28,  34,
        7,  30,  28,  34,
        8,  30,  28,  34,
        9,  28,  30,  30,
        10, 28,  30,  30,
        11, 28,  30,  30,
        12, 28,  30,  30,
        13, 28,  30,  30,
    TELEC,0x11, //NA
    	 255, 20,  16, 16,
    KCC, 0x11,   //NA,
    	 255, 26,  20, 20
};//}}}


/******WLAN_40MHz*****/
uint8_t _RS9116_HP_MODE_REGION_BASED_RATE_VS_MAXPOWER_SINGLE_BAND_NONHPM_40MHZ_XF6_M7DB7_1P4_MARS_ANT[] = { //{{{
3,
    FCC, 0x3,
         3, 12, 11, 11,
         6, 12, 14, 14,
         9, 12, 13, 13,
    TELEC, 0x11, //NA
    	 255, 10, 8, 8,
    KCC, 0x11, //NA
    	 255, 13, 10, 10,
};//}}}


/******BT*****/
uint8_t _RS9116_BT_REGION_BASED_MAXPOWER_XF6_M7DB7_1P4_MARS_ANT[] = {//{{{
//EDR - Max Power Index,  BDR Max Power Index, BLE Max Power Index,
FCC,        17,
ETSI,       10, //NA
TELEC,      14, //NA
WORLDWIDE,  11, //NA
KCC,        12, //NA
};//}}}


uint8_t _RS9116_BT_REGION_BASED_MAXPOWER_VS_OFFSET_XF6_M7DB7_1P4_MARS_ANT[] = {//{{{
5,//NUM_OF_REGIONS
FCC,
4,//NUM_OF_CHANNELS
//channel_num tx_power_offset
		 255,			  3,
       0,       3,
      39,       2,
      78,       0,
ETSI, //NA
4,//NUM_OF_CHANNELS
//channel_num tx_power
			255,			0,
       0,       0,
      39,       1,
      78,       0,
TELEC, //NA
4,//NUM_OF_CHANNELS
//channel_num tx_power
			255,			3,
       12,       0,
       15,       5,
       26,       1,
WORLDWIDE, //NA
4,//NUM_OF_CHANNELS
//channel_num tx_power
			255,      0,
        0,      0,
       39,      0,
       78,      0,
KCC, //NA
4,//NUM_OF_CHANNELS
//channel_num tx_power
			255,			3,
       2,       2,
       5,       5,
       7,       1,
};//}}}


/******BLE*****/
uint8_t _RS9116_BLE_REGION_BASED_MAXPOWER_XF6_M7DB7_1P4_MARS_ANT[] = {//{{{
//EDR - Max Power Index,  BDR Max Power Index, BLE Max Power Index,
FCC,        15,
ETSI,       10, //NA
TELEC,      14, //NA
WORLDWIDE,  20, //NA
KCC,        12, //NA
};//}}}


uint8_t _RS9116_BLE_REGION_BASED_MAXPOWER_VS_OFFSET_XF6_M7DB7_1P4_MARS_ANT[] = {//{{{
5,//NUM_OF_REGIONS
FCC,
4,//NUM_OF_CHANNELS
//channel_num tx_power_offset
			255,			2,
       0,       2,
      20,       2,
      39,       0,
ETSI, //NA
4,//NUM_OF_CHANNELS
//channel_num tx_power
			255,			0,
       0,       0,
      20,       0,
      39,       0,
TELEC, //NA
4,//NUM_OF_CHANNELS
//channel_num tx_power
			255,			3,
       12,       0,
       15,       5,
       26,       1,
WORLDWIDE, //NA
4,//NUM_OF_CHANNELS
//channel_num tx_power
			255,			 0,
        0,       0,
       20,       0,
       39,       0,
KCC, //NA
4,//NUM_OF_CHANNELS
//channel_num tx_power
			255,			3,
       2,       2,
       5,       5,
       7,       1,
};//}}}




/******5GHz-20MHz*****/
uint8_t _RS9113_RS8111_5G_HP_MODE_REGION_BASED_RATE_VS_MAXPOWER_NONHPM_XF6_M7DB7_1P4_MARS_ANT[] = {//{{{
2,
FCC, 0x6,
    1, 12, 12, //band 1
    2, 11, 11, //band 2	
  100, 10, 12, //band 3	
    3, 13, 13, //band 3	
  140, 10, 11, //band 4	
    4, 13, 13, //band 4	
TELEC, 0x4, //NA
  1, 9, 10, //band 1
  2, 8, 10, //band 2	
  3, 6,  8, //band 3	
  4, 6,  7, //band 4 
};//}}}


/******5GHz-40MHz*****/
uint8_t _RS9113_RS8111_5G_HP_MODE_REGION_BASED_RATE_VS_MAXPOWER_NONHPM_40MHZ_XF6_M7DB7_1P4_MARS_ANT[] = {//{{{
2,
FCC, 0x8,	
    1,  9,  9, //band 1
   62,  8,  8, //band 2	
    2,  9,  9, //band 2	
  102,  9,  9, //band 3	
  134, 12, 12, //band 3	
    3, 10, 10, //band 3	
  151, 11, 11, //band 4	    
    4, 11, 11, //band 4	    
TELEC, 0x4, //NA   
   1, 9, 10, //band 1
   2, 8, 10, //band 2	
   3, 6,  8, //band 3	
   4, 6,  7, //band 4 
};//}}}




//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Application for user config gain table
int32_t rsi_user_config_gain_table_app()
{
  int32_t     status       = RSI_SUCCESS;

  //! WC initialization
  status = rsi_wireless_init(8, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if (RSI_BAND == RSI_BAND_5GHZ) 
  status = rsi_send_feature_frame();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif 

  //! Wlan radio init 
  status = rsi_wlan_radio_init();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
	
	//! To selct Internal antenna or uFL connector 
  status = rsi_wireless_antenna(RSI_ANTENNA, RSI_ANTENNA_GAIN_2G , RSI_ANTENNA_GAIN_5G);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
	
  //! Update the gain table values
  status = rsi_wlan_update_gain_table((uint8_t )BAND,(uint8_t )RSI_BANDWIDTH,gain_table_payload, MAX_PAYLOAD_LEN);
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
  rsi_delay_ms(TRANSMIT_TIME);

  //! Stop TX transmit
  status = rsi_transmit_test_stop();
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
  rsi_task_create(rsi_user_config_gain_table_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call Transmit test application
  status = rsi_user_config_gain_table_app();

  //! Application main loop
  main_loop();
#endif
  return status;
}
