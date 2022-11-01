/*******************************************************************************
* @file  web_page_with_real_time_data.c
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
 * @brief : This file contains example application for Web Page with Real Time
 * Data Project
 * @section Description :
 * This application demonstrates how to view gathered Real Time Data such as
 * Region based Time & Date, Temperature and Surrounding Wi-Fi Networks Scan
 * Results on a web page using any browser.
 =================================================================================*/

/**
 * Include files
 * */

//! Contains definitions for Input/Output functions
#include <stdlib.h>

//! Contains definitions for Memory Allocation/freeing functions
#include <stdio.h>

//! Driver include file to refer driver APIs
#include "rsi_driver.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

//! WLAN include file to refer wlan non rom APIs
#include "rsi_wlan_non_rom.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! SNTP Client include file to refer SNTP APIs
#include "rsi_sntp_client.h"

//! Error include files
#include "rsi_error.h"

//! Network include file to refer network related APIs
#include "rsi_nwk.h"

#ifdef RSI_WITH_OS
//! OS include file to refer OS specific functionality
#include "rsi_os.h"
#endif

//! Include file for Type conversions
#include "rsi_utils.h"

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//!Include files for temperature sensor when Host MCU used is EFR32
#ifdef EFR32MG21A020F1024IM32
#include "em_cmu.h"
#include "em_gpio.h"
#include "tempdrv.h"
#endif


//! Access point's SSID to connect
#define SSID                   "SILABS_AP"

//! Scan Channel number , 0 - to scan all channels
#define CHANNEL_NO                 0

//! Security type of Access Point
#define SECURITY_TYPE            RSI_WPA2

//! Pre-shared key of Access Point
#define PSK                     "123456789"

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE                  1

//! If DHCP mode is disabled, IP is assigned statically
#if !(DHCP_MODE)

//! IP address of the module
#define DEVICE_IP               "192.168.10.10"

//! IP address of Gateway
#define GATEWAY                 "192.168.10.1"

//! IP address of subnet mask
#define NETMASK                 "255.255.255.0"

#endif

//! Enable to print all the intermediate debug prints present in the application
#define DEBUG_PRINTS                 0

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY       1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY     2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE    4000

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE   500

//! Memory length for driver
#define GLOBAL_BUFF_LEN             15000

//! Memory length to store IPv4 Address of RS9116 NCP
#define IP_BUFF_LEN                  20

//! Memory length for HTML code
#define WEB_BUFF_LEN                18000

//! Memory length to store Temperature value
#define TEMPERATURE_BUFF_LEN          5

//! Buffer length to store time and date fetched from SNTP Server
#define RSI_SNTP_RSP_BUFFER_LENGTH   50

//! Connected sleep mode
#define PSP_MODE                RSI_SLEEP_MODE_2

//! RSI_FAST_PSP mode allows module to disable power save for any Tx / Rx packet for monitor interval of time
#define PSP_TYPE                  RSI_FAST_PSP

//! Power save listen interval-based
#define PS_LISTEN_INTERVAL           500

//! SNTP timeout value
#define TIME_OUT                      5

//! Buffer length to store Time
#define TIME_BUFFER                   5

//! Buffer length to store Month
#define MONTH_BUFFER                  6

//! Buffer length to store Year
#define YEAR_BUFFER                   7

//! Unicast mode - SNTP client(RS9116 NCP) contacts specific SNTP Server
#define SNTP_METHOD             RSI_SNTP_UNICAST_MODE

#define FLAGS                         0

#ifdef DHCP_DNS
//! Buffer to store gateway IP extracted during IP configuration
char gateway_ip[20] = "";
#else
//! DNS server's primary IP address
#define primary_server_address      "8.8.8.8"
#endif

//! Domain name1 for SNTP server
#define RSI_DNS_URL_NAME1         "time.nist.gov"

//! Domain name2 for SNTP server (Backup)
#define RSI_DNS_URL_NAME2         "0.pool.ntp.org"

//! Buffer length to store scanned networks Info obtained in background scan
#define SSID_LENGTH                   34

//! Buffer length to store Security Types of networks scanned obtained in background scan
#define NO_OF_SEC_TYPES               10

//! Buffer length to store Security Type obtained in background scan
#define SEC_TYPE                      20

//! Buffer length to store RSSI values obtained in background scan
#define RSSI_LENGTH                   20

#define POWER_MODE_DISABLE_TIME      10000

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! Memory to store HTTP Response/ Web Page
char temp_send[WEB_BUFF_LEN];

//! Buffer to store IP address
unsigned char BUFF[IP_BUFF_LEN];

//! Buffer to store temperature value
char temp_arr[TEMPERATURE_BUFF_LEN ]="";

//! Buffer to store time and date
char SNTP_tdrsp[RSI_SNTP_RSP_BUFFER_LENGTH];

//! Buffer to store time stamp
char SNTP_trsp[RSI_SNTP_RSP_BUFFER_LENGTH];

//! SNTP server asynchronous response
rsi_sntp_server_rsp_t sntp_server_async_rsp;

//! Buffer to store DNS query results
rsi_rsp_dns_query_t  dns_query_resp;

//! Buffer to store seconds
char sec[TIME_BUFFER ]="";

//! Buffer to store minutes
char min[TIME_BUFFER ]="";

//! Buffer to store hours
char hours[TIME_BUFFER ]="";

//! Buffer to store day
char day[TIME_BUFFER ]="";

//! Buffer to store month
char month[MONTH_BUFFER]="";

//! Buffer to store year
char year[YEAR_BUFFER]="";

//! Buffer to store bg_scan results
rsi_rsp_scan_t bgscan_results;

//! Buffer to store bg scan results
char** bgscan_ssid;

//! Buffer to store bg_scan networks psk
uint8_t* bgscan_psk;

//! Buffer to store RSSI signal strengths of Wi-Fi networks
uint8_t* bgscan_rssi;

//! Buffer to store Security Type of scanned network
char sec_type[NO_OF_SEC_TYPES][SEC_TYPE]={ "Open", "WPA", "WPA2", "WEP", "WPA Enterprise", "WPA2 Enterprise","","WPA3"};

//! Global variable to store status of APIs
int32_t status             = RSI_SUCCESS;

//! Global variable to detect URL request
uint32_t status_urlreq     = 0;

//! Global variable to detect disconnection with AP
uint32_t status_disconnect = 0;

//! Global variable to store count of Wi-Fi networks
uint8_t  count             = 0;

//! Global variable to switch between two DNS servers
uint8_t  dns               = 1;

//! Global variable to detect SNTP server response
int32_t gettime_status     = 0;

//! Global variable to detect SNTP response
volatile uint32_t rsp_received = 0;

//! Global variable to record start time
volatile uint32_t start_time = 0;

//! Global variable to enable and disable power save
uint8_t power_mode           = 0;

//! Global variable to store temperature value
uint32_t temp                = 28;

void main_loop(void);

//! Handler for response from remote SNTP server
void rsi_sntp_client_create_response_handler(uint16_t status, const uint8_t cmd_type, const uint8_t *buffer)
{

  if(status == RSI_SUCCESS)
    {
      //! Set SUCCESS response
      rsp_received = 1;
      LOG_PRINT("\r\nReceived response from SNTP server\r\n");

    } else {

      LOG_PRINT("\r\nSNTP server failure  %X\r\n",status);
      //! Set FAILURE response
      rsp_received = 2;
    }

}


//! Handler for URL request call back
void rsi_webpage_request_handler(uint8_t type, uint8_t *url_name,uint8_t *post_content_buffer, uint32_t post_content_length, uint32_t status){

  if(status == RSI_SUCCESS)
  {
    LOG_PRINT("\r\nURL Request Received \r\n");
    status_urlreq=1;
  }

}


//! Handler for rejoin failure call back
void rsi_rejoin_fail_handler(uint16_t status, uint8_t *buffer, const uint32_t length){

      LOG_PRINT("\r\nConnection with AP is lost\r\n");
      status_disconnect=1;

}


void get_time_date(){

      uint16_t    length             = RSI_SNTP_RSP_BUFFER_LENGTH ;
      uint16_t    sntp_timeout       = TIME_OUT;
      uint64_t    server_address;
      uint32_t    SNTP_server_ip;
      uint8_t     j=1,k=0;

#ifdef DHCP_DNS
    server_address= ip_to_reverse_hex(gateway_ip);
#else
    server_address= ip_to_reverse_hex(primary_server_address);
#endif
      rsp_received=0;

      if (dns == 1) {
      //! Get SNTP server IP via DNS query
      status = rsi_dns_req(RSI_IP_VERSION_4,(uint8_t *)RSI_DNS_URL_NAME1, (uint8_t *)&server_address,NULL, &dns_query_resp,sizeof(rsi_rsp_dns_query_t));
             if (status == RSI_SUCCESS) {

                LOG_PRINT("\r\nDNS Request is successful\r\n");
                SNTP_server_ip = rsi_bytes4R_to_uint32(dns_query_resp.ip_address[0].ipv4_address);
                LOG_PRINT("\r\nSNTP Server's IP : %d.%d.%d.%d\n",(uint8_t)(SNTP_server_ip),(uint8_t)(SNTP_server_ip>>8),(uint8_t)(SNTP_server_ip>>16),(uint8_t)(SNTP_server_ip>>24));
             } else {

                LOG_PRINT("\r\nDNS Request has failed, Error Code : 0x%lX\r\n", status);
                dns=0;
                return;
             }
      }
      if (dns == 0) {
      status = rsi_dns_req(RSI_IP_VERSION_4,(uint8_t *)RSI_DNS_URL_NAME2, (uint8_t *)&server_address,NULL, &dns_query_resp,sizeof(rsi_rsp_dns_query_t));
              if (status == RSI_SUCCESS) {

                 LOG_PRINT("\r\nDNS Request is successful\r\n");
                 SNTP_server_ip = rsi_bytes4R_to_uint32(dns_query_resp.ip_address[0].ipv4_address);
                 LOG_PRINT("\r\nSNTP Server's IP : %d.%d.%d.%d\n",(uint8_t)(SNTP_server_ip),(uint8_t)(SNTP_server_ip>>8),(uint8_t)(SNTP_server_ip>>16),(uint8_t)(SNTP_server_ip>>24));
              } else {


                 LOG_PRINT("\r\nDNS Request has failed, Error Code : 0x%lX\r\n", status);
                 dns=1;
                 rsp_received = 2;
                 return;
              }
      }


      //! Create SNTP client
      status = rsi_sntp_client_create_async((uint8_t) FLAGS, (uint8_t *)&SNTP_server_ip, (uint8_t)SNTP_METHOD,sntp_timeout, rsi_sntp_client_create_response_handler);
      //! Check for valid status
      if (status != RSI_SUCCESS) {

        LOG_PRINT("\r\nCreate SNTP Client failed, Error Code : 0x%lX\r\n", status);
        rsp_received = 2;
        return ;
      } else {

#if DEBUG_PRINTS
        LOG_PRINT("\r\nCreated SNTP Client successfully\r\n");
#endif
      }

       //! wait for the response from remote SNTP server
       do
       {
          //! event loop
       #ifndef RSI_WITH_OS
          rsi_wireless_driver_task();
       #endif

       }while (rsp_received != 1);

       if (rsp_received==1) {

          //! Get time and date from SNTP server
          status = rsi_sntp_client_gettime_date(length, (uint8_t *)SNTP_tdrsp);
          //! Check for valid status
          if (status != RSI_SUCCESS) {

              rsp_received = 2;
              LOG_PRINT("\r\nGet Time and Date failed, Error Code : 0x%lX\r\n", status);
              return ;
          } else {

              LOG_PRINT("\r\nCurrent Time and Date  : %s\r\n",SNTP_tdrsp);


              while (j<4)
                 month[k++]=SNTP_tdrsp[j++];
              k=0;
#if DEBUG_PRINTS
              LOG_PRINT("\r\nMonth :%s\r\n",month);
#endif
              j++;

              while (j<7) {
                 if(SNTP_tdrsp[j]!=',')
                       day[k++]=SNTP_tdrsp[j++];
                 else{

                     j++;
                     break;
                 }
              }
#if DEBUG_PRINTS
              LOG_PRINT("\r\nDay   :%s\r\n",day);
#endif
              k=0;
              j++;

              while (j<13)
                 year[k++]=SNTP_tdrsp[j++];
#if DEBUG_PRINTS
              LOG_PRINT("\r\nYear  :%s\r\n",year);
#endif
              k=0;

              while (SNTP_tdrsp[j++] != ':')
                hours[k++]=SNTP_tdrsp[j-1];
#if DEBUG_PRINTS
              LOG_PRINT("\r\nHours :%s\r\n",hours);
#endif
              k=0;

              while (SNTP_tdrsp[j++] != ':')
                 min[k++]=SNTP_tdrsp[j-1];
#if DEBUG_PRINTS
              LOG_PRINT("\r\nMins  :%s\r\n",min);
#endif
              k=0;

              while (SNTP_tdrsp[j++] != '.')
                 sec[k++]=SNTP_tdrsp[j-1];
#if DEBUG_PRINTS
              LOG_PRINT("\r\nSec   :%s\r\n",sec);
#endif
            }

            //! SNTP client delete
            status = rsi_sntp_client_delete_async();
            if(status != RSI_SUCCESS) {

               LOG_PRINT("\r\nSNTP Client delete failed, Error Code : 0x%lX\r\n", status);
            } else {

#if DEBUG_PRINTS
               LOG_PRINT("\r\nDeleted SNTP Client\r\n");
#endif
            }

     }

}


void get_temp(){

      LOG_PRINT("\r\nFetching Temperature....\r\n");

#ifdef EFR32MG21A020F1024IM32
       //! Get the current temperature in degrees celsius
      temp =TEMPDRV_GetTemp();
#endif
      sprintf(temp_arr,"%lu",temp);
      LOG_PRINT("\r\nTemperature (in Celsius) : %s\r\n",temp_arr);

}


void get_scan(){

       LOG_PRINT("\r\nPerforming background scan....\r\n");
       //! Perform bg scan
       status = rsi_wlan_bgscan_profile(RSI_INSTANT_BGSCAN,&bgscan_results,sizeof(rsi_rsp_scan_t));
       if (status != RSI_SUCCESS) {

               LOG_PRINT("\r\nBackground scan has failed, Error Code : 0x%lX\r\n", status);
       } else {

               LOG_PRINT("\r\nBackground scan is successful\r\n");
                      uint8_t i=0;
                      count=bgscan_results.scan_count[0];
                      LOG_PRINT("\r\nTotal networks scanned : %d\r\n",count);

                      bgscan_rssi=malloc(sizeof(uint8_t)*count);
                      bgscan_psk=malloc(sizeof(uint8_t)*count);
                      bgscan_ssid=malloc(count * sizeof(char*));

                      for (i=0; i<count; i++){
                             bgscan_psk[i]=bgscan_results.scan_info[i].security_mode;
                             bgscan_rssi[i]=bgscan_results.scan_info[i].rssi_val;
                             bgscan_ssid[i]=(char*)malloc(SSID_LENGTH*sizeof(char));
                             bgscan_ssid[i]=(char*)bgscan_results.scan_info[i].ssid;
#if DEBUG_PRINTS
    LOG_PRINT("\r\nSecurity type and RSSI value for  %s :%d, %d\r\n",bgscan_ssid[i],bgscan_psk[i],bgscan_rssi[i]);
#endif
                      }

        }

}


void get_htmlcode(){

   //! Perform bg_scan
   get_scan();
   int8_t rssi_size=5;

   //! Character buffer to store RSSI values
   char rssi[count][rssi_size];

   for (int8_t j=0;j<count;j++) {

       //! Conversion from integer to character array
       sprintf(rssi[j],"%d",bgscan_rssi[j]);
    }

    //! Buffer to store HTML code
    char code[4000] = "<html><head><title>Display Real Time Data</title><link rel = \"icon\" href =\"https://upload.wikimedia.org/wikipedia/commons/thumb/1/1a/Silicon_Labs_2015.svg/1200px-Silicon_Labs_2015.svg.png\" type = \"image/x-icon\"> <style type=text/css> h1 {font-size:47px; font-style:normal;color: #000000;} body{text-align:center; font-size:35px; font-style:normal; background-repeat: no-repeat;background-position: 0px 200px; background-size: 100% 100%;} .button { background-color:#1589FF;border-radius:8px; border:2px solid #000000; color:#000000; text-align: center;display: inline-block; font-size: 26px; margin: 4px 2px; cursor: pointer; width: 311px; height: 50px; margin-right:4px; margin-left:4px;}.button:hover{color:#ffffff;} #btn{ padding-top: 10px; height:170px; padding-bottom:5px; background-image: url(\"https://upload.wikimedia.org/wikipedia/commons/thumb/1/1a/Silicon_Labs_2015.svg/1200px-Silicon_Labs_2015.svg.png\"); background-size: 110px 80px; background-repeat:no-repeat; background-position-x: 20px; background-position-y: 10px;outline-style: solid;outline-color: red;outline-offset:5px;font-size:35px; }#sub{height:30px;width: 150px;font-size: 20px;border-radius:5px;transition: background-color 1s ease-out;}#sub:hover{background-color:#4CC417;}#country{font-size: 25px;font-style: normal;} #res{ font-size:35px; color:black; text-align: center; }#time{font-size:40px;color:black;text-align: center;} table.scan{border:1px solid black;text-align:left;margin-left:auto;margin-right: auto;color: black;table-layout: auto; border-collapse:collapse; }  table.scan th,table.scan tr { border:1px solid black; text-align:left; margin-left:auto; margin-right: auto;border-collapse: collapse;height : 90px;font-size:35px;}table.tem th,table.tem tr{text-align:left;margin-left:auto;margin-right: auto;height : 70px;font-size:35px;}  .dot { height: 15px; width: 15px; background-color: green; border-radius: 50%; display:inline-block; color:green;   } #c{ text-align:center; font-size:25px; color:green;  } #c{text-align:center;font-size:25px;color:green;} table.tem { border:white;text-align:left;margin-left:auto;margin-right: auto;} .temid {color:black;} .wifi-symbol, .wifi-symbol.strong{  position: absolute;  display: inline-flex; width: 150px; height: 150px;  }  .wifi-symbol .wifi-circle, .wifi-symbol.strong.wifi-circle { box-sizing: border-box; -moz-box-sizing: border-box; display: inline-flex; width: 100%; height: 100%; font-size: 10px; position: absolute; bottom: 76%; left: 0; border-color: #000000; border-style: solid; border-width: 0.77em 0.77em 0 0; -webkit-border-radius: 0 100% 0 0; border-radius: 0 100% 0 0; opacity: 0;    } .wifi-symbol .wifi-circle.first { width: 4em; height: 4em; opacity: 1;  } .wifi-symbol .wifi-circle.second{ width: 2.85em; height: 2.85em; opacity: 1;    } .wifi-symbol .wifi-circle.third{ width: 1.8em; height: 1.8em; opacity: 1;   } .wifi-symbol .wifi-circle.fourth { width: 0.1em; height: 0.1em; opacity: 1;  }  .wifi-symbol.strong .wifi-circle.first, .wifi-symbol.medium .wifi-circle.first, .wifi-symbol.medium .wifi-circle.second, .wifi-symbol.weak .wifi-circle.first, .wifi-symbol.weak .wifi-circle.second, .wifi-symbol.weak .wifi-circle.third {  opacity: 0.3;  }</style></head><body id=\"resp\" onload=\"time_run()\" ><div id=\"btn\"><h1>REAL TIME DATA</h1><button class=\"button\"  onclick=\"time_get()\" id=\"time_btn\">Get Time & Date</button><button class=\"button\"  onclick=\"temp_get()\" id=\"temp_btn\">Get Temperature</button><button class=\"button\"  onclick=\"scan_get()\" id=\"scan_btn\">Display Wi-Fi Networks</button></div> <p id=\"res\"><br><br>Click on any of the buttons above, the asserted button appears in green</p><p id=\"time\"></p> <script type=\"text/javascript\"> var hours=\"";

    strncat(temp_send, code, 4000);

    //! Get SNTP server IP address via DNS request and fetch UTC time and date from that SNTP server
    LOG_PRINT("\r\nFetching Time and Date from SNTP Server....\r\n");
    get_time_date();
    if (rsp_received==2) {

        LOG_PRINT("\r\nTrying to fetch Response from other SNTP Server..............\r\n");

        //! Get UTC time and date from another SNTP server
        get_time_date();
    }

    //! Concatenate the time and date values to HTML code buffer
    strncat(temp_send,hours,TIME_BUFFER);
    strncat(temp_send,"\"; var mins=\"",15);
    strncat(temp_send,min,TIME_BUFFER);
    strncat(temp_send,"\"; var sec=\"",15);
    strncat(temp_send,sec,TIME_BUFFER);
    strncat(temp_send,"\"; var d=\"",15);
    strncat(temp_send,day,TIME_BUFFER);
    strncat(temp_send,"\"; var mnth=\"",16);
    strncat(temp_send,month,MONTH_BUFFER);
    strncat(temp_send,"\"; var yr=\"",15);
    strncat(temp_send,year,YEAR_BUFFER);

    char temp_re[7000]="\";var day=parseInt(d);var year=parseInt(yr); var ap=\"AM\"; var mer=0;var gt_twelve=0;var hr=parseInt(hours); var mn=parseInt(mins); var se=parseInt(sec)+3;var h=hr,hrs=hr,m=mn,mints=mn,s=se,secs=se;var dd=0;var mm=mnth;var yy=0;var mer=0;var timerun=0;var tem=0; var x=0;var prev=0;const month=new Map([[\"Jan\",1],[\"Feb\",2],[\"Mar\",3],[\"Apr\",4],[\"May\",5],[\"Jun\",6],[\"Jul\",7],[\"Aug\",8],[\"Sep\",9],[\"Oct\",10],[\"Nov\",11],[\"Dec\",12]]);const month_rev=new Map([[1,\"Jan\"],[2,\"Feb\"],[3,\"Mar\"],[4,\"Apr\"],[5,\"May\"],[6,\"Jun\"],[7,\"Jul\"],[8,\"Aug\"],[9,\"Sep\"],[10,\"Oct\"],[11,\"Nov\"],[12,\"Dec\"]]);function set_date(){ if(mm==\"Jan\"|mm==\"Mar\"|mm==\"May\"|mm==\"Jul\"|mm==\"Aug\"|mm==\"Oct\"|mm==\"Dec\"){if(prev==1){if(dd>1&&dd<32){dd=dd-1;}else if(dd==1){var p=month.get(mm);if(mm==\"Jan\"){mm=\"Dec\";dd=31;yy-=1;}else{mm=month_rev.get(p-1);if(mm==\"Jul\")dd=31;else dd=30;}}}else if(prev==2){  if(dd>=1&&dd<30){ dd=dd+1; } else if(dd==31){ var p=month.get(mm); if(mm==\"Dec\"){ mm=\"Jan\"; dd=1; yy+=1; } else{ mm=month_rev.get(p+1);   dd=1;  } }  } }else if(mm==\"Apr\"||mm==\"Jun\"||mm==\"Sep\"||mm==\"Nov\"){  if(prev==1){  if(dd>1&dd<32) { dd-=1; } else if(dd==1){ var p=month.get(mm);  mm=month_rev.get(p-1); dd=31;  } } else if(prev==2){  if(dd>=1&dd<29) { dd=dd+1; } else if(dd==30){ var p=month.get(mm);  mm=month_rev.get(p+1); dd=1;  } }   }  else if(mm==\"Feb\") { if(prev==2){ if(yy%4==0 & dd==28){ dd=29;  } else if((yy%4==0 & dd==29)|(yy%4!=0 & dd==28)){ dd=1; mm=\"Mar\";  } else dd=dd+1; }else if(prev==1){if(yy%4==0 & dd==1 & mm==\"Mar\"){ dd=29;}if(yy%4!=0 & dd==1 & mm==\"Mar\"){dd=28;}}} }function get_Temperature() { var tempfloat=parseFloat(document.getElementById(\"tempid\").innerHTML); var fahrenheit=((tempfloat*1.8)+32).toFixed(2); var kelvin=(tempfloat+273.15).toFixed(2); if(tempfloat>28&&tempfloat<37) { document.getElementById(\"resp\").style.backgroundImage=`url(\'https://wallpaperaccess.com/full/286494.jpg\')`; document.getElementById(\"temid\").style.color=\'#000000\';  } else if(tempfloat>26&&tempfloat<29) { document.getElementById(\"resp\").style.backgroundImage=`url(\'https://i.pinimg.com/originals/b0/a1/f9/b0a1f9e296c0349d491a770b82bac026.jpg\')`; document.getElementById(\"temid\").style.color=\'#FFFFFF\';  } else if(tempfloat>0&&tempfloat<23) { document.getElementById(\"resp\").style.backgroundImage=`url(\'https://www.sustrana.com/wp-content/uploads/2018/08/6cy5k8eqi.jpg\')`; document.getElementById(\"temid\").style.color=\'#B6100D\'; } else if(tempfloat>36&&tempfloat<50) { document.getElementById(\"resp\").style.backgroundImage=`url(\'https://c4.wallpaperflare.com/wallpaper/178/501/81/summer-desktop-backgrounds-wallpaper-preview.jpg\')`; document.getElementById(\"temid\").style.color=\'#000000\';  } else if(tempfloat>22&&tempfloat<27) { document.getElementById(\"resp\").style.backgroundImage=`url(\'https://wallpaperaccess.com/full/5465380.jpg\')`; document.getElementById(\"temid\").style.color=\'#FF0000\';  }  document.getElementById(\"fah\").innerHTML=fahrenheit; document.getElementById(\"kel\").innerHTML=kelvin;   }  function time_run() {s+=1; se+=1; if(s>=60){ s-=60; m+=1; } if(se>=60){ se-=60; mn+=1; } if(m>=60){ m-=60; h+=1; } if(mn>=60){ mn-=60; hr+=1; } if(gt_twelve==0){if(h==12 & ap==\"AM\"){ap=\"PM\";mer=1; }else if(h==12 & ap==\"PM\" &mer==0){ h=0;prev=2;set_date();ap=\"AM\";}}  if(h>12)h=h-12; if(h<10) hours=\"0\"+parseFloat(h); else hours=parseFloat(h); if(m<10) min=\"0\"+parseFloat(m); else min=parseFloat(m); if(s<10) sec=\"0\"+parseFloat(s); else sec=parseFloat(s); if(x==1&timerun==1){  document.getElementById(\"time\").innerHTML=\"<font size=5px>\"+parseFloat(dd)+\" \"+mm+\" \"+parseFloat(yy)+\"</font>\"+\"<br><b>\"+hours+\" : \"+min+\" : \"+sec+\"  \"+ap+\" </b>\";  }  setTimeout(time_run,1000);} function time_set(){  h=hr; m=mn; s=se; dd=day; yy=year; mm=mnth; count=document.getElementById(\"country\").value; if(count==\"California, USA\") { if(h==7){ tem=0; h=0; } else if(h<7){ h=h+12-7; tem=1; prev=1; set_date(); } else if(h>7&h<19){ h=h-7; tem=0; }else if(h==19){h=12;} else{ h=h-19; tem=1; }   } else if(count==\"Austin, USA\") { if(h==5){ tem=0; h=0; } else if(h<5){ h=h+12-5; tem=1; prev=1; set_date(); } else if(h>5&h<17){ h=h-5; tem=0; }else if(h==17){h=12;} else{ h=h-17; tem=1; }   } else if(count==\"UK\") { if(h<12){ tem=0;} else if(h==12){ tem=1;} else{ h=h-12; tem=1; }   } else if(count==\"Hungary\") { if(h>=23){ h=h-24+1; tem=0; prev=2; set_date(); } else if(h<11){ h=h+1; tem=0; }else if(h==11){h=12;} else{ h=h+1-12; tem=1; }  } else if(count==\"Finland\") { if(h>=21){ h=h-24+3; tem=0; prev=2; set_date(); } else if(h<9){ h=h+3; tem=0; } else if(h==9){h=12;}else{ h=h+3-12; tem=1; }  } else if(count==\"India\") { if(h==18&&m>=30){ h=h-24+6; m=m+30; tem=0; prev=2; set_date();  } else if(h>18){ h=h-24+5; m=m+30; tem=0; prev=2; set_date(); } else if(h==6&m<30){ h=h+5; m=m+30; tem=0;  } else if(h==6&m==30){h=12;}else if(h<6){ h=h+5; m=m+30; tem=0;  }else if(h==6&m>30){h=h+5;m=m+30;tem=0;} else{ h=h+5; m=m+30; tem=1; }   } else if(count==\"China\" || count==\"Taiwan\" || count==\"Singapore\" ) { if(h>=16){ h=h-24+8; tem=0; prev=2; set_date(); } else if(h<4){ h=h+8; tem=0; }else if(h==4)h=12; else{ h=h+8-12; tem=1;  }  }  else if(count==\"Japan\") { if(h>=15){ h=h-24+9; tem=0; prev=2; set_date(); } else if(h<3){ h=h+9; tem=0; } else if(h==3)h=h-12;else{ h=h+9-12; tem=1; }  } if(tem==0){ ap=\"AM\"; } else if(tem==1)ap=\"PM\";if(h==12){gt_twelve=1;ap=\"PM\";} else if(h==0){gt_twelve=1;ap=\"AM\";}   timerun=1;}function time_get() { document.getElementById(\"time_btn\").setAttribute(\"style\", \"background-color: #4CC417;\"); document.getElementById(\"resp\").style.removeProperty(\"background-image\"); document.getElementById(\"temp_btn\").setAttribute(\"style\", \"background-color:#C0C0C0;\"); document.getElementById(\"scan_btn\").setAttribute(\"style\", \"background-color:#C0C0C0;\");";
    strncat(temp_send,temp_re, 7000);
    if (rsp_received==2) {

          strncat(temp_send,"x=2;",5);
          rsp_received=0;
    } else if (rsp_received==1) {

          strncat(temp_send,"x=1;",5);
          rsp_received=0;
    }

    char temp_remaining[1000]="if(x==2){ document.getElementById(\"res\").innerHTML=\"<br>\";document.getElementById(\"time\").innerHTML=\"Failed to get response from SNTP Server\";}else document.getElementById(\"res\").innerHTML=\"<br><p style=\\\"color:black\\\">Select Region : <select id=\\\"country\\\"><option selected>California, USA</option><option>Austin, USA</option><option>UK</option><option>Hungary</option><option>Finland</option><option>India</option><option>China</option><option>Taiwan</option><option>Singapore</option><option>Japan</option></select></p><span><button id=\\\"sub\\\" onclick=\\\"time_set()\\\">Submit</button></span>\";}";
    strncat(temp_send,temp_remaining,1000);

    char temp_r[850]=" function temp_get() { x=0; document.getElementById(\"temp_btn\").setAttribute(\"style\", \"background-color: #4CC417;\"); document.getElementById(\"time_btn\").setAttribute(\"style\", \"background-color:#C0C0C0;\"); document.getElementById(\"scan_btn\").setAttribute(\"style\", \"background-color:#C0C0C0;\"); document.getElementById(\"res\").innerHTML=\"<b><br><br><table class=\\\"tem\\\" id=\\\"temid\\\"><tr><th> Temperature (in <sup>o</sup>C)&nbsp:&nbsp<span id=\\\"tempid\\\">";
    strncat(temp_send,temp_r,850);
    strncat(temp_send,temp_arr,TEMPERATURE_BUFF_LEN);

    char temp_rem[850]="</span></th></tr><tr><th>Temperature (in <sup>o</sup>F)&nbsp:&nbsp<span id=\\\"fah\\\"></span></th></tr><tr><th> Temperature (in &nbspK)&nbsp:&nbsp<span id=\\\"kel\\\">  </span></th></tr></b>\";document.getElementById(\"time\").innerHTML=\"<br>\"; get_Temperature();   }  function scan_get() { x=0; document.getElementById(\"scan_btn\").setAttribute(\"style\", \"background-color: #4CC417;\"); document.getElementById(\"time_btn\").setAttribute(\"style\", \"background-color: #C0C0C0;\"); document.getElementById(\"temp_btn\").setAttribute(\"style\", \"background-color: #C0C0C0;\"); document.getElementById(\"resp\").style.removeProperty(\"background-image\"); document.getElementById(\"res\").innerHTML=\"<br><table class=\\\"scan\\\"><tr><th>SSID </th><th>Security Mode &nbsp&nbsp</th><th>RSSI(dBm) &nbsp&nbsp</th></tr>";
    strncat(temp_send,temp_rem, 850);

    for (int8_t i=0;i<count;i++) {

      strncat(temp_send,"<tr><th><p style=\\\"font-weight:normal\\\">",80);
      char rssi_add[25]="&nbsp<span class=\\\"";
      strncat(temp_send,rssi_add,25);

      char rssi_exe[RSSI_LENGTH] = "wifi-symbol";
      char rssi_good[RSSI_LENGTH] = "wifi-symbol strong";
      char rssi_medium[RSSI_LENGTH] = "wifi-symbol medium";
      char rssi_weak[RSSI_LENGTH] = "wifi-symbol weak";

      if (bgscan_rssi[i]<50)
          strncat(temp_send,rssi_exe,RSSI_LENGTH);
      else if (bgscan_rssi[i]>49 && bgscan_rssi[i]<60)
          strncat(temp_send,rssi_good,RSSI_LENGTH);
      else if (bgscan_rssi[i]>59 && bgscan_rssi[i]<70)
          strncat(temp_send,rssi_medium,RSSI_LENGTH);
      else if (bgscan_rssi[i]>69)
          strncat(temp_send,rssi_weak,RSSI_LENGTH);

      char rssi_rem[300]="\\\"> <span class=\\\"wifi-circle first\\\"></span> <span class=\\\"wifi-circle second\\\"></span> <span class=\\\"wifi-circle third\\\"></span> <span class=\\\"wifi-circle fourth\\\"></span> </span>&nbsp&nbsp&nbsp&nbsp&nbsp <span style=\\\"font-weight:normal\\\">";
      strncat(temp_send,rssi_rem,300);
      strncat(temp_send,bgscan_ssid[i],SSID_LENGTH+1);

      if (strcmp((char*)SSID,bgscan_ssid[i])==0) {
        strncat(temp_send,"</span>&nbsp <span class=\\\"dot\\\"> </span><span id=\\\"c\\\"> connected&nbsp&nbsp&nbsp</span>",100);
      }

      strncat(temp_send,"</p></th><th><p style=\\\"font-weight:normal\\\">",80);
      strncat(temp_send,sec_type[bgscan_psk[i]],SEC_TYPE);
      strncat(temp_send,"</p></th><th><p style=\\\"font-weight:normal\\\">-",80);
      strncat(temp_send,rssi[i],rssi_size);
      strncat(temp_send,"</p></th></tr>",30);
    }
    char temp_final[100]="</table><br>\"; document.getElementById(\"time\").innerHTML=\"<br>\";} </script></body></html>";
    strncat(temp_send,temp_final,100);

    memset(temp_arr,0,strlen(temp_arr));
    memset(hours,0,strlen(hours));
    memset(min,0,strlen(min));
    memset(sec,0,strlen(sec));
    memset(day,0,strlen(day));
    memset(month,0,strlen(month));
    memset(year,0,strlen(year));

    for (int8_t i=0;i<count;i++) {

        memset(rssi[i],0,strlen(rssi[i]));
        memset(bgscan_ssid[i],0,strlen(bgscan_ssid[i]));
    }

    free(bgscan_psk);
    free(bgscan_rssi);

}

uint32_t power_save_feature() {


   //! Set the power save profile with listen interval-based wake up
   status = rsi_wlan_power_save_with_listen_interval(PSP_MODE, PSP_TYPE, PS_LISTEN_INTERVAL);
   if (status != RSI_SUCCESS) {

         LOG_PRINT("\r\nFailed to enter power save mode, Error Code : 0x%lX\r\n",status);
         return status;
    } else {

         LOG_PRINT("\r\nEntered listen interval based power save mode\r\n");
         power_mode=1;
    }
   return 0;
}


int32_t config_host()
{

  #if !(DHCP_MODE)
  uint32_t ip_addr      = ip_to_reverse_hex(DEVICE_IP);
  uint32_t network_mask = ip_to_reverse_hex(NETMASK);
  uint32_t gateway      = ip_to_reverse_hex(GATEWAY);
  #else
  uint8_t dhcp_mode     = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
  #endif

    //! Driver initialization
    status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
    if ((status < 0) || (status > GLOBAL_BUFF_LEN)) {
          return status;
    }

    //! Module initialization
    status = rsi_device_init(LOAD_NWP_FW);
    if (status != RSI_SUCCESS) {

          LOG_PRINT("\r\nDevice initialization has failed, Error Code : 0x%lX\r\n", status);
          return status;
    } else {

          LOG_PRINT("\r\nDevice initialization is successful\r\n");
    }
#ifdef RSI_WITH_OS
     rsi_task_handle_t driver_task_handle = NULL;

      //! Task created for Driver task
      rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                      (uint8_t *)"driver_task",
                      RSI_DRIVER_TASK_STACK_SIZE,
                      NULL,
                      RSI_DRIVER_TASK_PRIORITY,
                      &driver_task_handle);
#endif

  //! Register the URL Request event call back
  rsi_wlan_nwk_register_webpage_req_cb(RSI_WLAN_NWK_URL_REQ_CB, rsi_webpage_request_handler);

  //! Register the Rejoin failure event call
  rsi_wlan_register_callbacks (RSI_JOIN_FAIL_CB, rsi_rejoin_fail_handler);


  //! Module's features initialization
  status = rsi_wireless_init(0, 0);

  if (status != RSI_SUCCESS) {

       LOG_PRINT("\r\nWireless Initialization has failed, Error Code : 0x%lX\r\n", status);
       return status;
  } else {

       LOG_PRINT("\r\nWireless Initialization is successful\r\n");
  }

  //! Erase all web pages that are already present in module's flash
  status = rsi_webpage_erase(NULL);
  if (status != RSI_SUCCESS) {

        LOG_PRINT("\r\nFailed to erase previous web pages in the module's flash, Error Code : 0x%lX\r\n", status);
        return status;
  } else {

        LOG_PRINT("\r\nErased all the previous web pages in module's flash\r\n");
  }

  //! Scan for AP with given SSID
  status = rsi_wlan_scan((int8_t*)SSID, (uint8_t)CHANNEL_NO, NULL, 0);
  if (status != RSI_SUCCESS) {

     LOG_PRINT("\r\nWLAN AP Scan has failed, Error Code : 0x%lX\r\n", status);
     return status;
  } else {

     LOG_PRINT("\r\nWLAN AP Scan is successful\r\n");
  }

  //! Connect to AP
  status = rsi_wlan_connect((int8_t *)SSID, (rsi_security_mode_t)SECURITY_TYPE, PSK);
  if (status != RSI_SUCCESS) {

      LOG_PRINT("\r\nFailed to connect to AP, Error Code : 0x%lX\r\n", status);
      return status;
  } else {

      LOG_PRINT("\r\nConnected to AP\r\n");
  }

  //! Get IP address
  #if DHCP_MODE
    status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0,(uint8_t*)BUFF, sizeof(BUFF), 0);

  #else
    status            = rsi_config_ipaddress(RSI_IP_VERSION_4,
                                             RSI_STATIC,
                                             (uint8_t *)&ip_addr,
                                             (uint8_t *)&network_mask,
                                             (uint8_t *)&gateway,
                                             (uint8_t*)BUFF,
                                             sizeof(BUFF),
                                             0);
  #endif

   if (status != RSI_SUCCESS) {

       LOG_PRINT("\r\nIP Configuration has failed, Error Code : 0x%lX\r\n", status);
       return status;
   } else {

       LOG_PRINT("\r\nIP Address of the module : %d.%d.%d.%d\r\n",BUFF[6], BUFF[7], BUFF[8], BUFF[9]);
       LOG_PRINT("\r\nIP Configuration is successful\r\n");
#ifdef DHCP_DNS
       sprintf(gateway_ip,"%d.%d.%d.%d",BUFF[14],BUFF[15],BUFF[16],BUFF[17]);
       LOG_PRINT("\r\ngateway IP address: %s\r\n",gateway_ip);
#endif
   }

  //! Set module in power save mode
  status = power_save_feature();
  if(status != RSI_SUCCESS)
       return status;

  //! Application main loop
  main_loop();

  return 0;

}


void main_loop(void)
{
#ifdef EFR32MG21A020F1024IM32
  //! Initialize temperature Driver
  TEMPDRV_Init();

  //! Enable the temperature Driver
  TEMPDRV_Enable(1);
#endif

  while (1) {
      ////////////////////////
      //! Application code ///
      ////////////////////////

      //! event loop

      //! Check for URL request
      if (status_urlreq == 1 ){

             //! Disable the power save mode
             rsi_wlan_power_save_profile(RSI_ACTIVE,PSP_TYPE);
             LOG_PRINT("\r\nDisabled power save mode\r\n");
             power_mode=0;

             status_urlreq=0;

             //! Function call to get the temperature from Si7021 sensor integrated on Host MCU EFR32
             get_temp();

             //! Function call to prepare HTTP Response
             get_htmlcode();

             if (status_disconnect == 0) {

                 //! Send web page to Host MCU
                 status = rsi_webpage_send(0, (uint8_t*)temp_send, strlen(temp_send));
                 if(status == RSI_SUCCESS){

                      LOG_PRINT("\r\nResponded with the requested web page\r\n");
                      start_time=rsi_hal_gettickcount();
#if DEBUG_PRINTS
                      LOG_PRINT("\r\nStart time: %ld\r\n",start_time);

#endif

                 } else {

                      LOG_PRINT("\r\nFailed to send web page, Error Code : 0x%lX\r\n", status);
                 }
             }

             memset(temp_send,0,strlen(temp_send));
      }

      //! Enter power save mode again if no URL request is received for MAX_POWER_MODE_DISABLE_TIME (10 sec)
      if(((rsi_hal_gettickcount())-start_time) > POWER_MODE_DISABLE_TIME && power_mode == 0 && status == 0)
      {
#if DEBUG_PRINTS
            LOG_PRINT("\r\nEnd time: %ld\r\n",rsi_hal_gettickcount());
#endif
            power_save_feature();
            power_mode=1;
      }
#ifndef RSI_WITH_OS
      //!Handles driver events
      rsi_wireless_driver_task();
#endif

   }

}


int main()
{
  int32_t status=0;

#ifdef RSI_WITH_OS

 rsi_task_handle_t wlan_task_handle = NULL;

  //! Task created for WLAN task
  rsi_task_create((rsi_task_function_t)config_host,
                    (uint8_t *)"wlan_task",
                    RSI_WLAN_TASK_STACK_SIZE,
                    NULL,
                    RSI_WLAN_TASK_PRIORITY,
                    &wlan_task_handle);

  // Start the scheduler
  rsi_start_os_scheduler();
#else

  //! Call Web Page with Real Time Data application
  status = config_host();

  //! Application main loop
  main_loop();
#endif

  return status;
}















