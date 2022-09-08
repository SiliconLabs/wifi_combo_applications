
/*******************************************************************************
* @file  rsi_low_power_wlan.c
* @brief
*******************************************************************************
* # License
* <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/

/*================================================================================
 * @brief : This file contains example application for Low power WLAN 
 * @section Description :
 * This application will demonstrate the low power functionality, for both the
 * EFR32 and the RS9116W.
 =================================================================================*/

/*******************************************************************************
 ***************************  INCLUDE FILES   ********************************
 ******************************************************************************/

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
#include "string.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#include "rsi_utils.h"

//! Power manager include files
#include "sl_power_manager.h"
#include "em_gpio.h"

/*******************************************************************************
 ***************************  DEFINES   ********************************
 ******************************************************************************/


#if CODE_CORRELATION_ENABLE
    #include "bsp_trace.h"
#endif

//! Access point SSID to connect
#define SSID "SILABS_AP"

//! Password
#define PSK "1234567890"

//! Security type
//! open mode: RSI_OPEN     WPA2: RSI_WPA2,
#define SECURITY_TYPE RSI_WPA2

//! defines related to Host MCU power save
#define PWSAVE_STANDBY  RSI_ENABLE
//#define PWSAVE_PING   RSI_ENABLE

//! Power Save Profile mode
#define PSP_MODE RSI_SLEEP_MODE_2

//! Power Save Profile type
#define PSP_TYPE RSI_FAST_PSP

//!Uncomment this macro to enable Code Correlation feature
//#define CODE_CORRELATION_ENABLE 1

#define HOST_PWSAVE_ENABLED RSI_ENABLE
//! Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO 0

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE 1

//! If DHCP mode is disabled given IP statically
#if !(DHCP_MODE)

//! IP address of the module
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP "192.168.10.101" //0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY "192.168.10.1" //0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK "255.255.255.0" //0x00FFFFFF

#endif


//! IP address to ping
//! AP Gateway address is selected as REMOTE_IP by default
//#define REMOTE_IP "192.168.0.1"

//! Memory length for driver
#define GLOBAL_BUFF_LEN 15000

//! Wlan task priority
#define RSI_APPLICATION_TASK_PRIORITY 1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY 2

//! Wlan task stack size
#define RSI_APPLICATION_TASK_STACK_SIZE 1500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE 500

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Buffer to store scan results
rsi_rsp_scan_t scan_results;

//!Buffer to store firmware version
uint8_t fw_version[20];

//! Buffer to store Security Type of scanned network
char sec_type[10][20]={ "Open", "WPA", "WPA2", "WEP", "WPA_EAP", "WPA2_EAP","","WPA3"};

#ifdef RSI_WITH_OS
  rsi_task_handle_t application_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;

#endif

// Enumeration for states in application
typedef enum rsi_wlan_app_state_e {
  RSI_WLAN_INITIAL_STATE                   = 0,
  RSI_WLAN_SCAN_STATE                      = 1,
  RSI_WLAN_UNCONNECTED_STATE               = 2,
  RSI_WLAN_CONNECTED_STATE                 = 3,
  RSI_WLAN_IPCONFIG_DONE_STATE             = 4,
  RSI_PWSAVE_INIT_STATE                    = 5,

} rsi_wlan_app_state_t;

/*******************************************************************************
 ***************************  FUNCTION PROTOTYPES   ********************************
 ******************************************************************************/

int32_t rsi_wlan_power_save_profile(uint8_t psp_mode, uint8_t psp_type);
extern int32_t rsi_wlan_filter_broadcast(uint16_t beacon_drop_threshold,
                                         uint8_t filter_bcast_in_tim,
                                         uint8_t filter_bcast_tim_till_next_cmd);


#ifdef HOST_PWSAVE_ENABLED
#ifdef PWSAVE_PING

  int32_t rsi_host_pwmode_timer_init();
  extern void rsi_delay_ms(uint32_t delay_ms);
  #ifdef REMOTE_IP

    extern uint64_t ip_to_reverse_hex(char *ip);

  #endif

#elif PWSAVE_STANDBY

  int32_t rsi_host_pwmode_init();

#endif

#ifdef PWSAVE_PING

  uint8_t ping_rsp_received = 0;
  //! ping response notify call back handler
    void rsi_ping_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length)
    {

      ping_rsp_received = 1;

    }

#endif
#endif

int32_t application()
{
  uint8_t ip_buff[20];
  int32_t status = RSI_SUCCESS;
  #if !(DHCP_MODE)
    uint32_t ip_addr      = ip_to_reverse_hex(DEVICE_IP);
    uint32_t network_mask = ip_to_reverse_hex(NETMASK);
    uint32_t gateway      = ip_to_reverse_hex(GATEWAY);
  #else
    uint8_t dhcp_mode = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
  #endif

  //! Driver initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if ((status < 0) || (status > GLOBAL_BUFF_LEN)) {
    return status;
  }

  //! Silicon Labs module initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nDevice Initialization Success\r\n");

  #if CODE_CORRELATION_ENABLE
    void BSP_TraceSwoSetup(void);

    BSP_TraceSwoSetup();
    BSP_TraceProfilerSetup();
  #endif

  #ifdef RSI_WITH_OS

    //! Task created for Driver task
    rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                    (uint8_t *)"driver_task",
                    RSI_DRIVER_TASK_STACK_SIZE,
                    NULL,
                    RSI_DRIVER_TASK_PRIORITY,
                    &driver_task_handle);
  #endif

  //! WC initialization
  status = rsi_wireless_init(0, 0);
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%lX\r\n", status);
    return status;
  }
  LOG_PRINT("\r\nWireless Initialization Success\r\n");

  //! Get firmware version of RS9116
  status = rsi_wlan_get(RSI_FW_VERSION, fw_version, sizeof(fw_version));
  LOG_PRINT("\r\nFirmware version: %s\r\n", fw_version);

  //! Send feature frame
  status = rsi_send_feature_frame();
  if (status != RSI_SUCCESS) {
    LOG_PRINT("\n Feature Frame Failed, Error Code :0x%lX \r\n", status);
    return status;
  }
  uint8_t state=RSI_WLAN_INITIAL_STATE;

  while(1){

    switch (state) {

    case RSI_WLAN_INITIAL_STATE: {
      state = RSI_WLAN_SCAN_STATE;

    } break;

    case RSI_WLAN_SCAN_STATE: {
      //! Scan for AP with given SSID
      status = rsi_wlan_scan(NULL,(uint8_t)CHANNEL_NO, &scan_results,sizeof(rsi_rsp_scan_t));
      if (status != RSI_SUCCESS) {
        LOG_PRINT("wLAN AP Scan Failure, Error Code : 0x%lX\r\n", status);
        state = RSI_WLAN_SCAN_STATE;
      }
      else {
        state = RSI_WLAN_UNCONNECTED_STATE;
        LOG_PRINT("\r\nWLAN AP Scan success\r\n");
        uint8_t i=0;
        uint8_t count=scan_results.scan_count[0];
        LOG_PRINT("\r\nTotal networks scanned : %d\r\n",count);
        LOG_PRINT("\r\nChannel   Security Mode   RSSI    SSID\r\n");

        for (i=0;i<count;i++){
           LOG_PRINT("\r\n%-10d",scan_results.scan_info[i].rf_channel);
           LOG_PRINT("%-16s",sec_type[scan_results.scan_info[i].security_mode]);
           LOG_PRINT("-%-7d",scan_results.scan_info[i].rssi_val);
           LOG_PRINT("%-s\r\n",scan_results.scan_info[i].ssid);

        }
      }

    } break;

    case RSI_WLAN_UNCONNECTED_STATE: {
      //! Connect to an Access point
      status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nWLAN AP Connect Failure, Error Code : 0x%lX\r\n", status);
        state = RSI_WLAN_INITIAL_STATE;
      }
      else {
        LOG_PRINT("\r\nWLAN AP Connect Success\r\n");
        state = RSI_WLAN_CONNECTED_STATE;
      }

    } break;

    case RSI_WLAN_CONNECTED_STATE: {
      //! Configure IP Address
      #if DHCP_MODE
        status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, ip_buff, sizeof(ip_buff), 0);
      #else
        status            = rsi_config_ipaddress(RSI_IP_VERSION_4,
                                          RSI_STATIC,
                                          (uint8_t *)&ip_addr,
                                          (uint8_t *)&network_mask,
                                          (uint8_t *)&gateway,
                                          NULL,
                                          0,
                                          0);
      #endif
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%lX\r\n", status);
        state = RSI_WLAN_CONNECTED_STATE;
      }
      else {
        LOG_PRINT("\r\nIP Config Success\r\n");
        LOG_PRINT("RSI_STA IP ADDR: %d.%d.%d.%d \r\n", ip_buff[6], ip_buff[7], ip_buff[8], ip_buff[9]);
        LOG_PRINT("Gateway IP ADDR: %d.%d.%d.%d \r\n", ip_buff[14], ip_buff[15], ip_buff[16], ip_buff[17]);
        state = RSI_WLAN_IPCONFIG_DONE_STATE;
      }
      //! Enable Broadcast data filter
      status = rsi_wlan_filter_broadcast(5000, 1, 1);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nBroadcast Data Filtering Failed with Error Code : 0x%lX\r\n", status);
        return status;
      }

    } break;

    case RSI_WLAN_IPCONFIG_DONE_STATE: {
      //! Apply power save profile with connected sleep
      status = rsi_wlan_power_save_profile(PSP_MODE, PSP_TYPE);
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nPowersave Config Failed, Error Code : 0x%lX\r\n", status);
        return status;
      }
      LOG_PRINT("\r\nPowersave Config Success \r\n");
	  LOG_PRINT("  ");
      state = RSI_PWSAVE_INIT_STATE;

    } break;

    default:
      break;
    }
    if (state==RSI_PWSAVE_INIT_STATE){
      break;
    }
  }
#ifdef HOST_PWSAVE_ENABLED

  //! Initialize power manager
  #ifdef PWSAVE_PING
    status = sl_power_manager_init();
    if (status != RSI_SUCCESS) {
      LOG_PRINT("\r\nPower Manager Initialization Failed, Error Code : 0x%lX\r\n", status);
      return status;
    }
  #endif

  #ifdef REMOTE_IP
    uint32_t ping_ip=ip_to_reverse_hex((char *)REMOTE_IP);
  #else
    uint32_t ping_ip=0;
    ping_ip = (uint64_t)ip_buff[14];
    ping_ip |= (uint64_t)(ip_buff[15] << 8);
    ping_ip |= (uint64_t)(ip_buff[16] << 16);
    ping_ip |= (uint64_t)(ip_buff[17] << 24);

  #endif

  while(1)
  {

    #if PWSAVE_PING
      status = rsi_host_pwmode_timer_init();
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nPower Mode Initialization Failed, Error Code : 0x%lX\r\n", status);
        return status;
      }
    #elif PWSAVE_STANDBY
      status = rsi_host_pwmode_init();
      if (status != RSI_SUCCESS) {
        LOG_PRINT("\r\nPower Mode Initialization Failed, Error Code : 0x%lX\r\n", status);
        return status;
      }
    #endif

      sl_power_manager_sleep();

      #if PWSAVE_PING
        status= rsi_wlan_ping_async(0, (uint8_t *)&ping_ip, 100, rsi_ping_response_handler);
        if (status==RSI_SUCCESS){

          LOG_PRINT("\r\nPing Request\r\n");

        }
        while (!ping_rsp_received)
        {
          #ifdef RSI_WITH_OS
              rsi_delay_ms(2);
          #else
              rsi_wireless_driver_task();
          #endif
        }
        ping_rsp_received = 0;

        LOG_PRINT("Ping Reply\r\n");

    #endif

  }
#endif
  return status;

}

//! main function definition
int main(void)
{
  #ifdef RSI_WITH_OS

    //! Create application task
    rsi_task_create((rsi_task_function_t)application,
                    (uint8_t *)"application_task",
                    RSI_APPLICATION_TASK_STACK_SIZE,
                    NULL,
                    RSI_APPLICATION_TASK_PRIORITY,
                    &application_handle);


    //! Start the scheduler
    rsi_start_os_scheduler();

  #else
    application();
  #endif
}

