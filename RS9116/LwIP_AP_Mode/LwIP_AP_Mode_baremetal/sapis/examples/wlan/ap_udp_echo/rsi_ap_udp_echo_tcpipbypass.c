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

#include "rsi_utils.h"
//! Error include files
#include "rsi_error.h"

//! File for prototype of memcpy functions 
#include "string.h"
#include "rsi_bootup_config.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"


//! Configuration parameters 

//! Access point SSID to be created
#define SSID               "REDPINE_AP"

//! Channel Number
#define CHANNEL_NO             11 

//! Security Type
#define SECURITY_TYPE        RSI_WPA2

//! Encryption Type
#define ENCRYPTION_TYPE      RSI_CCMP

//! Password
#define PSK                "1234567890"

//! Beacon Interval
#define BEACON_INTERVAL        100

//! DTIM Count
#define DTIM_COUNT             4  

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP          0x010AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY            0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK            0x00FFFFFF 

//! Device port number
#define DEVICE_PORT        5001

//! Remote port number
#define REMOTE_PORT        5001



//! Standard Defines

//!   ethernet header offset
#define ETHERNET_HEADER_OFFSET    14

//! IP header offset
#define IP_HEADER_OFFSET         (14 + 20)

//! UDP header offset
#define UDP_HEADER_OFFSET        (14 + 20 + 8)

//!ARP REQUEST CODE
#define ARP_OPTION_REQUEST       0x0100

//! ARP RESPONSE CODE
#define ARP_OPTION_RESPONSE      0x0200

//! ARP Message size
#define ARP_MESSAGE_SIZE         28

//! MAC address size
#define MAC_ADDRESS_SIZE         6

//! IP header size 
#define IP_HEADER_SIZE           20

//! Protocol type of UDP
#define UDP_PROTOCOL             17

//!Time to live
#define TIME_TO_LIVE             64

//! ARP PACKET 
#define RSI_ARP_PACKET          0x0608

//! IP PACKET
#define RSI_IP_PACKET           0x0008

//! WLAN event: set when a  packet from WLAN device received
#define RSI_WLAN_EVENT           2

//! Application buffers size
#define RSI_APP_BUF_SIZE        2000

//! Memory length for driver
#define GLOBAL_BUFF_LEN         15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   2

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500


//! Wlan access point mode 
#define RSI_WLAN_AP_MODE    6

//! Parameter to run forever loop
#define RSI_FOREVER             1

//! Function prototypes

extern int32_t rsi_wlan_check_mac_address(const uint8_t *buffer, uint32_t length);
extern void rsi_wlan_set_ipaddress(uint32_t ip_addr,uint32_t network_mask, uint32_t gateway);
extern int32_t rsi_wlan_check_packet_type(uint8_t *buffer, uint16_t length);
extern void rsi_wlan_handle_arp_request(uint8_t *buffer, uint16_t length);
extern void udp_socket_create(uint16_t port_no);
void udp_send(uint16_t remote_port, uint8_t *buffer, uint32_t length);
extern void udp_recv(uint8_t *buffer, uint32_t length);
extern void rsi_wlan_ap_app_task(void);
extern void rsi_wlan_app_callbacks_init(void);
extern void rsi_stations_connect_notify_handler(uint16_t status,  uint8_t *buffer, const uint32_t length);
extern void rsi_stations_disconnect_notify_handler(uint16_t status,  uint8_t *buffer, const uint32_t length);
extern void rsi_packet_receive_notify_handler(uint16_t status,  uint8_t *buffer, const uint32_t length);
extern void rsi_prepare_udp_packet_header(uint8_t *buffer, uint16_t length);
extern uint16_t rsi_data_send_checksum(uint32_t csum, uint8_t * buff_ptr, uint32_t data_len);
extern uint32_t rsi_udp_start_checksum(uint8_t proto, uint16_t len, uint16_t port1, uint16_t port2,
                uint8_t *srcaddr, uint8_t *dstaddr);
extern uint16_t rsi_udp_end_checksum(uint32_t csum);
uint16_t convert_le_be(uint16_t num);
void rsi_udp_socket_create(uint16_t port_no);
void prepare_udp_pkt_header(uint8_t *buffer, uint32_t length);

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

//! stations count
uint8_t rsi_stations_count;

//! device static ip address
uint32_t    ip_addr      = DEVICE_IP;

//! Network mask
uint32_t    network_mask = NETMASK;

//! Gateway
uint32_t    gateway      = GATEWAY;


typedef struct rsi_ether_header_s 
{
  //! destination mac address
  uint8_t ether_dest[6];

  //! source mac address
  uint8_t ether_source[6];

  //! ethernet type
  uint16_t ether_type;

} rsi_ether_header_t;

//! Enumeration for commands used in application
typedef enum rsi_app_cmd_e
{
  RSI_DATA = 0
}rsi_app_cmd_t;

//! IP header structure
typedef struct rsi_ip_header_s 
{
  //! version 
  uint8_t ver_ihl;

  //! type of service
  uint8_t tos;

  //! length higher byte
  uint8_t len_hb;

  //! length lower byte
  uint8_t len_lb;

  //! id higher byte
  uint8_t id_hb;

  //! id lower byte
  uint8_t id_lb;

  //! fragmentation higher byte
  uint8_t frag_hb;

  //! fragmentation lower byte
  uint8_t frag_lb;

  //! time to live
  uint8_t ttl;

  //! protocol
  uint8_t proto;

  //! check sum higher byte
  uint8_t csum_hb;

  //! check sum lower byte
  uint8_t csum_lb;

  //! source ip address
  uint8_t srcaddr[4];

  //! destination ip address
  uint8_t dstaddr[4];

} rsi_ip_header_t;

//! Enumeration for states in applcation 
typedef enum rsi_wlan_app_state_e
{
  RSI_WLAN_INITIAL_STATE              = 0,
  RSI_WLAN_AP_UP_STATE                = 1,
  RSI_WLAN_SOCKET_CONNECTED_STATE     = 2

}rsi_wlan_app_state_t;

//! UDP header structure
typedef struct rsi_udp_header_s 
{
  //! source port
  uint16_t src_port;

  //! destination port
  uint16_t dest_port;

  //! payload length
  uint16_t length;

  //! check sum
  uint16_t check_sum;

}rsi_udp_header_t;

//! UDP pkt total header, excluding payload 
typedef struct rsi_udp_pkt_header_s
{
  //! ethernet header
  rsi_ether_header_t ether_header;

  //! ip header
  rsi_ip_header_t  ip_header;

  //! udp header
  rsi_udp_header_t udp_header;

}rsi_udp_pkt_header_t;

//! APR packet structure
typedef struct rsi_arp_packet_s
{
  //! hardware type
  uint16_t hardware_type;

  //! protocol type
  uint16_t protocol_type;

  //! hardware size
  uint8_t  hardware_size;

  //! protocol size
  uint8_t  protocol_size;

  //! protocol opcode
  uint16_t opcode;

  //!sender physical address
  uint8_t  sender_mac_addr[6];

  //! sender ip address
  uint8_t  sender_ip_addr[4];

  //! target physical address
  uint8_t  target_mac_addr[6];

  //! target ip address
  uint8_t  target_ip_addr[4];

}rsi_arp_packet_t;

//! wlan application control block

typedef struct rsi_wlan_device_params_s
{
  //! device static ip address
  uint32_t    module_ip_addr;

  //! udp source port no
  uint16_t source_port;

  //! remote device ip address
  uint32_t    sender_ip_addr;

  //! udp remote port no
  uint16_t remote_port;

  //! module physical address
  uint8_t module_mac_addr[6];

  //! target physical address
  uint8_t sender_mac_addr[6];

}rsi_wlan_device_params_t;

//! wlan application control block
typedef struct rsi_wlan_app_cb_s
{
  //! wlan application state 
  rsi_wlan_app_state_t state;

  //! length of buffer to copy
  uint32_t tx_pkt_length;

  //! application buffer
  uint8_t tx_buffer[RSI_APP_BUF_SIZE];

  //! to check application buffer availability
  uint8_t tx_buf_in_use;

  //! length of buffer to copy
  uint32_t rx_pkt_length;

  //! application buffer
  uint8_t rx_buffer[RSI_APP_BUF_SIZE];

  //! to check application buffer availability
  uint8_t rx_buf_in_use;

  //! application events bit map 
  uint32_t event_map;

  //! wlan device details
  rsi_wlan_device_params_t device_params;

}rsi_wlan_app_cb_t;

//! application control block
rsi_wlan_app_cb_t rsi_wlan_app_cb;



void rsi_wlan_app_callbacks_init(void)
{

  //! Initialze station connect notify  call back
  rsi_wlan_register_callbacks(RSI_STATIONS_CONNECT_NOTIFY_CB, rsi_stations_connect_notify_handler);

  //! Initialze station disconnect notify call back
  rsi_wlan_register_callbacks(RSI_STATIONS_DISCONNECT_NOTIFY_CB, rsi_stations_disconnect_notify_handler);

  //! Initialize packet receive notify call back
  rsi_wlan_register_callbacks(RSI_WLAN_DATA_RECEIVE_NOTIFY_CB, rsi_packet_receive_notify_handler);
}

int32_t rsi_wlan_ap_app(void)
{
  int32_t     status = RSI_SUCCESS;

  //! WiSeConnect initialization
  status = rsi_wireless_init(RSI_WLAN_AP_MODE, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  while(RSI_FOREVER)
  {
    //! WLAN application tasks
    rsi_wlan_ap_app_task();
#ifndef RSI_WITH_OS
    //! wireless driver tasks
    rsi_wireless_driver_task();
#endif

  }
}

void  rsi_wlan_ap_app_task(void)
{
  int32_t     status = RSI_SUCCESS;
  uint32_t pkt_type = 0; 

  switch(rsi_wlan_app_cb.state)
  {
    case RSI_WLAN_INITIAL_STATE:
      {
        //! register call backs
        rsi_wlan_app_callbacks_init();

        //! Configure IP 
        rsi_wlan_set_ipaddress(ip_addr, network_mask, gateway);

        //! Start Access point
        status =  rsi_wlan_ap_start((int8_t *)SSID, CHANNEL_NO, SECURITY_TYPE, ENCRYPTION_TYPE, PSK, BEACON_INTERVAL, DTIM_COUNT);
        if(status != RSI_SUCCESS)
        {
          break;
        }

        //! Get MAC address of the Access point
        status = rsi_wlan_get(RSI_MAC_ADDRESS, rsi_wlan_app_cb.device_params.module_mac_addr, MAC_ADDRESS_SIZE);

        if(status != RSI_SUCCESS)
        {
          break;
        }
        else
        {
          //! update wlan application state
          rsi_wlan_app_cb.state = RSI_WLAN_AP_UP_STATE; 
        }
      }
    case RSI_WLAN_AP_UP_STATE:
      { 
        //! create UDP socket at the Access point
        rsi_udp_socket_create((uint16_t)DEVICE_PORT);

        //! update wlan application state
        rsi_wlan_app_cb.state = RSI_WLAN_SOCKET_CONNECTED_STATE; 

      }
    case RSI_WLAN_SOCKET_CONNECTED_STATE:
      {
        if( rsi_wlan_app_cb.event_map & RSI_WLAN_EVENT)
        {
          //! To check the type of packet: ARP or IP packet
          pkt_type = rsi_wlan_check_packet_type(rsi_wlan_app_cb.rx_buffer, rsi_wlan_app_cb.rx_pkt_length);

          switch(pkt_type)
          {
            case RSI_ARP_PACKET:
              {
                //! Send ARP response for the received arp request 
                rsi_wlan_handle_arp_request(rsi_wlan_app_cb.rx_buffer, rsi_wlan_app_cb.rx_pkt_length);
              }
              break;
            case RSI_IP_PACKET:
              {
                //! Copy received packet to rx buffer
                udp_recv(&rsi_wlan_app_cb.rx_buffer[UDP_HEADER_OFFSET], (rsi_wlan_app_cb.rx_pkt_length - UDP_HEADER_OFFSET));

              }
              break;
            default:
              {

              }
              break;

          }
          //! make buffer in use
          rsi_wlan_app_cb.rx_buf_in_use = 0;

          //! clear send event after data send is successful
          rsi_wlan_app_cb.event_map &= ~(RSI_WLAN_EVENT);
          
          //! make buffer in use
          rsi_wlan_app_cb.tx_buf_in_use = 0;

        }

      }
    default:
      break;
  }

}

//! callback functions

//! stations connect notify call back handler in AP mode
void rsi_stations_connect_notify_handler(uint16_t status,uint8_t *buffer, const uint32_t length)
{

  //! increment connected stations count
  rsi_stations_count++;

}

//! stations disconnect notify call back handler in AP mode
void rsi_stations_disconnect_notify_handler(uint16_t status,  uint8_t *buffer, const uint32_t length)
{
  //! decrement connected stations count
  rsi_stations_count--;
}

//! packet receive notify call back handler in AP mode
void rsi_packet_receive_notify_handler(uint16_t status,  uint8_t *buffer, const uint32_t length)
{

  //! check destination mac address of the received packet 
  status = rsi_wlan_check_mac_address(buffer, length);

  if(status != RSI_SUCCESS)
  {
    return;
  }
  else
  {
    if(!rsi_wlan_app_cb.rx_buf_in_use)
    {
      //! if not in use

      //! copy the buffer to wlan app cb wlan buffer
      memcpy(rsi_wlan_app_cb.rx_buffer, buffer, length); 

      //! hold length information
      rsi_wlan_app_cb.rx_pkt_length = length;

      //! make buffer in use
      rsi_wlan_app_cb.rx_buf_in_use = 1;

      //! raise event to wlan app task
      rsi_wlan_app_cb.event_map |= RSI_WLAN_EVENT;


    }
    else
      //!if buffer is in use
    {
      return;
    }


  }
}


//! Function to check destination mac address of the recieved packet
int32_t rsi_wlan_check_mac_address(const uint8_t *buffer, uint32_t length)
{
  rsi_ether_header_t *ether_header;

  ether_header = (rsi_ether_header_t *)buffer;

  if ((memcmp(ether_header->ether_dest, rsi_wlan_app_cb.device_params.module_mac_addr,MAC_ADDRESS_SIZE) == 0) ||
      (ether_header->ether_dest[0] == 0xFF))
  {
    return RSI_SUCCESS;
  }
  return RSI_FAILURE;

}

void rsi_wlan_set_ipaddress(uint32_t ip_addr, uint32_t network_mask,uint32_t gateway)
{
  //! hold ip address of the Access point in a variable
  rsi_wlan_app_cb.device_params.module_ip_addr = ip_addr;
}


int32_t rsi_wlan_check_packet_type(uint8_t *buffer, uint16_t length)
{
  rsi_arp_packet_t *arp_packet;
  rsi_ether_header_t *ether_header;
  rsi_ip_header_t   *ip_header;
  rsi_udp_header_t   *udp_header;

  ether_header = (rsi_ether_header_t *)buffer;

  if (ether_header->ether_type == RSI_ARP_PACKET)
  {
    /* Setup a pointer to the ARP message.  */
    arp_packet = (rsi_arp_packet_t *)(buffer  + ETHERNET_HEADER_OFFSET);

    /* Determine if the ARP message type is valid.  */
    if (arp_packet->opcode != ARP_OPTION_REQUEST)
    {
      return RSI_FAILURE; 
    }
    else
    {
      return RSI_ARP_PACKET; 
    }
  }
  else if (ether_header->ether_type == RSI_IP_PACKET) 
  {
    /* Setup a pointer to the IP message.  */
    ip_header = (rsi_ip_header_t *)(buffer  + ETHERNET_HEADER_OFFSET);

    /* Determine what protocol the current IP datagram is.  */

    if((rsi_bytes4R_to_uint32(ip_header->dstaddr) == rsi_wlan_app_cb.device_params.module_ip_addr)&&(ip_header->proto == UDP_PROTOCOL))
    {

      udp_header = (rsi_udp_header_t *)(buffer  + IP_HEADER_OFFSET);

      if(rsi_wlan_app_cb.device_params.source_port == convert_le_be(udp_header->dest_port))
      {
        //! copy sender ip address in a global variable
        rsi_wlan_app_cb.device_params.sender_ip_addr = rsi_bytes4R_to_uint32(ip_header->srcaddr); 
        
        //! copy remote port in a global variable
        rsi_wlan_app_cb.device_params.remote_port = convert_le_be(udp_header->src_port); 
      }
      return RSI_IP_PACKET; 

    }
  }

  return RSI_FAILURE; 
}

//! Function to handle ARP request packet by sending arp response
void rsi_wlan_handle_arp_request(uint8_t *buffer, uint16_t length)
{
  rsi_arp_packet_t *arp_packet;
  rsi_ether_header_t *ether_header;


  ether_header = (rsi_ether_header_t *)buffer;

  //! Setup a pointer to the ARP message.  
  arp_packet = (rsi_arp_packet_t *)(buffer  + ETHERNET_HEADER_OFFSET);

  //! Pick up the sender's physical address from the message.  
  memcpy(rsi_wlan_app_cb.device_params.sender_mac_addr,(arp_packet->sender_mac_addr),MAC_ADDRESS_SIZE); 

  //! Pick up sender ip address
  rsi_wlan_app_cb.device_params.sender_ip_addr = rsi_bytes4R_to_uint32(arp_packet->sender_ip_addr); 

  //! Set the ARP message type to ARP response.  
  arp_packet->hardware_size = 0x06; 
  arp_packet->protocol_size = 0x04; 
  arp_packet->opcode = ARP_OPTION_RESPONSE;


  //! Now fill in the new source and destination information for the ARP response.  
  

  //! copy Access point mac address to source address of ARP response packet
  memcpy(ether_header->ether_source, rsi_wlan_app_cb.device_params.module_mac_addr, MAC_ADDRESS_SIZE);
  
  //! copy Access point mac address to sender mac address of ARP response packet
  memcpy(arp_packet->sender_mac_addr, rsi_wlan_app_cb.device_params.module_mac_addr,MAC_ADDRESS_SIZE);
  
  //! copy Access point ip address to source ip address of ARP response packet
  rsi_uint32_to_4bytes(arp_packet->sender_ip_addr, rsi_wlan_app_cb.device_params.module_ip_addr); 
  
  //! copy remote device mac address to destination address of ARP response packet
  memcpy(ether_header->ether_dest, rsi_wlan_app_cb.device_params.sender_mac_addr,MAC_ADDRESS_SIZE); 
  
  //! copy remote device mac address to target address of ARP response packet
  memcpy(arp_packet->target_mac_addr, rsi_wlan_app_cb.device_params.sender_mac_addr,MAC_ADDRESS_SIZE); 
  
  //! copy remote device ip address to target ip address of ARP response packet
  rsi_uint32_to_4bytes(arp_packet->target_ip_addr, rsi_wlan_app_cb.device_params.sender_ip_addr); 

  //! Make sure the packet length is set properly.  
  length =  ARP_MESSAGE_SIZE + ETHERNET_HEADER_OFFSET;

  rsi_wlan_send_data(buffer, length);
}

//! Funtion to create udp socket
void rsi_udp_socket_create(uint16_t port_no)
{
  rsi_wlan_app_cb.device_params.source_port = port_no;
}

//! To send packet received from wlan station connected back to it
void udp_send(uint16_t remote_port, uint8_t *buffer, uint32_t length)
{
  rsi_udp_pkt_header_t *udp_pkt;

  udp_pkt = (rsi_udp_pkt_header_t *)rsi_wlan_app_cb.rx_buffer;

  memcpy ((uint8_t *)(udp_pkt + 1), buffer, length);
  
  //! prepare udp header for the given udp data packet
  prepare_udp_pkt_header((uint8_t *)udp_pkt, length + sizeof(rsi_udp_pkt_header_t));
  
  //! send this udp data packet to remote wlan client 
  rsi_wlan_send_data(rsi_wlan_app_cb.rx_buffer,  length + sizeof(rsi_udp_pkt_header_t));

}

//! To receive packet from Wlan station and forward it to wlan device again
void udp_recv(uint8_t *buffer, uint32_t length)
{

  //! send data on socket
  udp_send(REMOTE_PORT, buffer, length);
}

//! convert little Endian to Big Endian
uint16_t convert_le_be(uint16_t num)
{
  return ((num >> 8) & 0xFF) | ((num & 0xFF) << 8);
}
#define CSUM_INT16(H,L) (((((uint16_t)H) <<8)) | L)

//! calculate checksum for pseudo header
uint32_t rsi_udp_start_checksum(uint8_t proto, uint16_t len, uint16_t port1, uint16_t port2,
    uint8_t *srcaddr, uint8_t *dstaddr)
{
  uint32_t csum;
  csum = CSUM_INT16(srcaddr[0],srcaddr[1]);
  csum += CSUM_INT16(srcaddr[2],srcaddr[3]);
  csum += CSUM_INT16(dstaddr[0],dstaddr[1]);
  csum += CSUM_INT16(dstaddr[2],dstaddr[3]);
  csum = csum + proto;

  /* finish UDP header checksum */
  if (proto == UDP_PROTOCOL)
    csum += len;
  csum += len;
  csum += port1;
  csum += port2;

  return (csum);
}

//! Prepare UDP packet to payload recieved from connected device
void prepare_udp_pkt_header(uint8_t *buffer, uint32_t length)
{
  uint16_t check_sum;
  uint32_t csum = 0;
  rsi_udp_pkt_header_t *udp_pkt;
  uint16_t ip_len = length - ETHERNET_HEADER_OFFSET;
  static uint16_t ip_id =0;
  ip_id++;
  udp_pkt = (rsi_udp_pkt_header_t *)buffer;
  

  //! fill UDP header
  udp_pkt->udp_header.src_port      = convert_le_be(rsi_wlan_app_cb.device_params.source_port);
  udp_pkt->udp_header.dest_port     = convert_le_be(REMOTE_PORT);
  udp_pkt->udp_header.length        = convert_le_be((uint16_t)(length - IP_HEADER_OFFSET));
  udp_pkt->udp_header.check_sum     = 0;


  //! fill IP header
  udp_pkt->ip_header.ver_ihl  =    0x45;
  udp_pkt->ip_header.tos      =    0;
  udp_pkt->ip_header.len_hb   =    ((ip_len >>8) & 0xFF);
  udp_pkt->ip_header.len_lb   =    ip_len & 0xFF;
  udp_pkt->ip_header.id_hb    =    ((ip_id >> 8) & 0xFF);
  udp_pkt->ip_header.id_lb    =    ip_id & 0xFF;
  udp_pkt->ip_header.frag_hb  =    0;
  udp_pkt->ip_header.frag_lb  =    0;
  udp_pkt->ip_header.ttl      =    TIME_TO_LIVE; 
  udp_pkt->ip_header.proto    =    UDP_PROTOCOL ;
  udp_pkt->ip_header.csum_hb  =    0;
  udp_pkt->ip_header.csum_lb  =    0;
  rsi_uint32_to_4bytes(udp_pkt->ip_header.srcaddr, rsi_wlan_app_cb.device_params.module_ip_addr);
  rsi_uint32_to_4bytes(udp_pkt->ip_header.dstaddr, rsi_wlan_app_cb.device_params.sender_ip_addr);

  csum = rsi_udp_start_checksum(UDP_PROTOCOL,(uint16_t)(length - IP_HEADER_OFFSET), rsi_wlan_app_cb.device_params.source_port, REMOTE_PORT,
      udp_pkt->ip_header.srcaddr, udp_pkt->ip_header.dstaddr);
  check_sum = rsi_data_send_checksum(csum, buffer + UDP_HEADER_OFFSET, (length - UDP_HEADER_OFFSET));
  udp_pkt->udp_header.check_sum = convert_le_be(check_sum);

  check_sum = rsi_data_send_checksum(0, buffer + ETHERNET_HEADER_OFFSET, IP_HEADER_SIZE);

  udp_pkt->ip_header.csum_hb = ((check_sum >>8) & 0xFF);
  udp_pkt->ip_header.csum_lb =  check_sum & 0xFF ;
 
  //! fill ethernet header
  memcpy(udp_pkt->ether_header.ether_dest, rsi_wlan_app_cb.device_params.sender_mac_addr, MAC_ADDRESS_SIZE);
  memcpy(udp_pkt->ether_header.ether_source, rsi_wlan_app_cb.device_params.module_mac_addr, MAC_ADDRESS_SIZE);
  udp_pkt->ether_header.ether_type = RSI_IP_PACKET;
}


//! update the checksum from the data to be sent
uint16_t rsi_data_send_checksum(uint32_t csum, uint8_t * buff_ptr, uint32_t data_len)
{
  uint16_t check_sum;
  uint16_t temp_data;
  uint8_t  odd_byte;

  if (data_len)
  {
    odd_byte = (uint8_t)(data_len & 0x01);
    data_len = data_len >> 1;    

    while (data_len--)           
    {

      temp_data = ((uint16_t)((uint16_t)(*buff_ptr++)<<8));
      temp_data |= (uint16_t)(*buff_ptr++);
      csum += temp_data;
    }

    if (odd_byte)
    {
      temp_data = ((uint16_t)((uint16_t)(*buff_ptr)<<8));
      temp_data &= 0xFF00;
      csum += temp_data;
    }
  }

  //! udp end check sum 
  check_sum = rsi_udp_end_checksum(csum);

  if(check_sum == 0)
  {
    return 0xFFFF;
  }

  return check_sum;
}


//! udp end check sum 
uint16_t rsi_udp_end_checksum(uint32_t csum)
{
  while (csum >> 16)
    csum = (csum & 0xFFFF) + (csum >> 16);

  return ((uint16_t)(~csum));
}

//! main funtion definition
int main(void)
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
  rsi_task_create(rsi_wlan_ap_app, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
  //! NON - OS case
  //! Call  WLAN  application
  status = rsi_wlan_ap_app();

#endif
  return status;

}


