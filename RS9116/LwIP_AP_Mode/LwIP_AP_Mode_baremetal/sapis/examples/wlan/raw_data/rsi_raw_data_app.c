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

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_driver.h"

//! Access point SSID to be created.
#define SSID              "REDPINE_AP"

//! Channel number
#define CHANNEL_NO              11

//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Encryption type
#define ENCRYPTION_TYPE   RSI_NONE

//! Password
#define PSK               "1234567890"

//! Beacon interval
#define  BEACON_INTERVAL        100

//! DTIM interval 
#define DTIM_INTERVAL           3

//! IP address of the module 
//! E.g: 0x010AA8C0 == 192.168.10.1
#define DEVICE_IP         0x010AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 

//! Memory length for driver
#define GLOBAL_BUFF_LEN   15000

//! Memory length for driver
#define APP_BUFF_LEN   300

//!standard macros for the Application

#define  PING_REPLY_PKT_SIZE 74

#define  ARP_REPLY_PKT_SIZE 45

#define  IP_ADDRESS_SIZE    4

#define  MAC_ADDRESS_SIZE   6

#define  ARP_PKT            0x806

#define  IP_PKT             0x800

#define  PROTO_ICMP         0x1

#define  ICMP_PING_REPLY    0

#define  ARP_REPLY          0x0200

#define  ICMP_PING_REQUEST  8

#define  CHECKSUM_LENGTH    2


//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Memory to initialize driver
uint8_t     global_buf[GLOBAL_BUFF_LEN];
int8_t      send_buffer[APP_BUFF_LEN] = {0};
uint8_t     glbl_mac[MAC_ADDRESS_SIZE] = {0};
uint8_t     send_raw_pkt = 0;
uint16_t    framelen = 0; 


typedef struct rsi_ip_header_s
{
  uint8_t       ip_header_word_0[4];
  uint8_t       ip_header_word_1[4];
  uint8_t       ip_header_word_2[4];
  /* Define the source IP address.  */
  uint8_t       ip_header_source_ip[4];
  /* Define the destination IP address.  */
  uint8_t       ip_header_destination_ip[4];
}rsi_ip_header_t;

typedef struct rsi_eth_header_s
 {
   uint8_t  src_mac_addr[6];
   uint8_t  dst_mac_addr[6];
   uint8_t  pkt_type[2];
 }rsi_eth_header_t;


typedef struct rsi_req_arp_s
{
  uint8_t    hw_type[2];
  uint8_t    proto_type[2];
  uint8_t    hw_size;
  uint8_t    proto_size;
  uint8_t    opcode[2];
  uint8_t    sender_mac_addr[6];
  uint8_t    sender_ip_addr[4]; 
  uint8_t    dst_mac_addr[6];
  uint8_t    dst_ip_addr[4];
}rsi_req_arp_t;

static void main_loop(void);


void raw_data_receive_handler(uint16_t status, uint8_t *data, const uint32_t length)
{
  rsi_ip_header_t    ip_header;
  rsi_eth_header_t   eth_header;
  rsi_req_arp_t      arp_req;
  uint16_t           pkt_type = 0;
  uint16_t           checksum = 0;

  //!pkt type
  pkt_type = ((data[12] << 8) | (data[13]));

  if(pkt_type == IP_PKT)
  {
    //! Extract IP Header
    memcpy((uint8_t *)&ip_header ,&data[14], sizeof(rsi_ip_header_t));
  }
  else if(pkt_type == ARP_PKT)
  {
    //! Extract Arp request pkt
    memcpy((uint8_t *) &arp_req,&data[14], sizeof(rsi_req_arp_t));
  }


  //! ARP Packet
  //! Compare ARP request destination IP address with our Module Ip address
  if(pkt_type == ARP_PKT)
  {
    //! Extract Arp request pkt
    memcpy((uint8_t *)&arp_req,&data[14], sizeof(rsi_req_arp_t));

    //!clearing buffer
    memset(&send_buffer[0], 0, sizeof(send_buffer));

    //! Copy the Received Packet
    memcpy((uint8_t *)&send_buffer[0], (uint8_t *) &data[0], length);

    //! Extract Ethernet Header
    memcpy((uint8_t *)&eth_header,(uint8_t *)&data[0], sizeof(rsi_eth_header_t));

    //! Copy Destinatio Macaddress
    memcpy(&send_buffer[0],(uint8_t *)&arp_req.sender_mac_addr[0], MAC_ADDRESS_SIZE);

    //! Copy Sender/Source Macaddress
    memcpy(&send_buffer[6],&glbl_mac[0], MAC_ADDRESS_SIZE);

    *(uint16_t *)&send_buffer[20] = ARP_REPLY;

    //! Copy Sender/Source Macaddress
    memcpy(&send_buffer[22],&glbl_mac[0], MAC_ADDRESS_SIZE);

    //! Copy Sender/Source IP address
    memcpy(&send_buffer[28],(uint8_t *)arp_req.dst_ip_addr, IP_ADDRESS_SIZE);

    //! Copy Destinatio Macaddress
    memcpy(&send_buffer[32],(uint8_t *)&arp_req.sender_mac_addr[0], MAC_ADDRESS_SIZE);

    //! Copy Destination IP address
    memcpy(&send_buffer[38],(uint8_t *)arp_req.sender_ip_addr, IP_ADDRESS_SIZE);

    if(arp_req.sender_ip_addr[0] || arp_req.sender_ip_addr[1] || arp_req.sender_ip_addr[2] || arp_req.sender_ip_addr[3])
    {
      send_raw_pkt = 1;
    }

    framelen = ARP_REPLY_PKT_SIZE;
  }

  //! Check for IP Packet
  //! Check for ICMP Protocol 
  else if((pkt_type == IP_PKT) && (ip_header.ip_header_word_2[1] == PROTO_ICMP))
  {
    //!Extracting ip header
    memcpy((uint8_t *)&ip_header ,&data[14], sizeof(rsi_ip_header_t));

    //!clearing buffer
    memset(&send_buffer[0], 0, sizeof(send_buffer));

    //! Copy the Received Packet
    memcpy((uint8_t *) &send_buffer[0], (uint8_t *)&data[0], length);

    //! Extract ICMP Header Checksum
    checksum = *(uint16_t *)&send_buffer[36];

    //! Recalculate ICMP Header checksum to send ICMP REPLY PKT
    checksum = checksum + ICMP_PING_REQUEST; 

    //! Extract Ethernet Header
    memcpy((uint8_t *)&eth_header,(uint8_t *)&send_buffer[0], sizeof(rsi_eth_header_t) );

    //! Extract IP Header
    memcpy((uint8_t *)&ip_header ,&send_buffer[14], sizeof(rsi_ip_header_t));

    //! Copy Destination Macaddress 
    memcpy(&send_buffer[0], &eth_header.dst_mac_addr[0], MAC_ADDRESS_SIZE);

    //! Copy Sender/source Macaddress
    memcpy(&send_buffer[6], &eth_header.src_mac_addr[0], MAC_ADDRESS_SIZE);

    //! Copy Sender/Source IP address 
    memcpy(&send_buffer[26], &ip_header.ip_header_destination_ip[0], IP_ADDRESS_SIZE);

    //! Copy Destinatio IP address
    memcpy(&send_buffer[30], &ip_header.ip_header_source_ip[0], IP_ADDRESS_SIZE);

    //! Keep ICMP message type as a ping reply
    send_buffer[34] = ICMP_PING_REPLY;

    //! Copy Checksum value
    memcpy(&send_buffer[36],(uint8_t *)& checksum, CHECKSUM_LENGTH);

    *(send_buffer + sizeof(send_buffer)-1) = '\0';

    send_raw_pkt = 1;

    framelen = PING_REPLY_PKT_SIZE;     
  }

}

int32_t rsi_ap_start()
{
  int32_t     status       = RSI_SUCCESS;
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;


  //! WC initialization
  status = rsi_wireless_init(6, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Configure IP 
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  //!registering call back
  status = rsi_wlan_register_callbacks(RSI_WLAN_RAW_DATA_RECEIVE_HANDLER,raw_data_receive_handler);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Start Access point
  status =  rsi_wlan_ap_start((int8_t *)SSID, CHANNEL_NO, SECURITY_TYPE, ENCRYPTION_TYPE, PSK, BEACON_INTERVAL, DTIM_INTERVAL);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //!mac address
  status = rsi_wlan_get(RSI_MAC_ADDRESS, &glbl_mac[0], 6);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  while(1)
   {
#ifndef RSI_WITH_OS
     main_loop();
#else
    if(send_raw_pkt)
#endif
    {
      status = rsi_send_raw_data((uint8_t *)&send_buffer[0],framelen);
      if(status != RSI_SUCCESS)
      {
        return status;
      }

     framelen = 0;
    }
    send_raw_pkt=0;
   }

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
   
    if(send_raw_pkt)
    {
      send_raw_pkt = 0;
      break;
    }

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
  rsi_task_create(rsi_ap_start, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call TCP server application in AP mode
  status = rsi_ap_start();

  //! Application main loop

#endif
  return status;

}

