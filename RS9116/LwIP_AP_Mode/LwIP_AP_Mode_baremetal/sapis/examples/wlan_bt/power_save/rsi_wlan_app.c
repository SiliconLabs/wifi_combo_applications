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

//! Driver Header file
#include "rsi_driver.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h" 
#include "rsi_wlan_non_rom.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

//! include the certificate 
#include "cacert.pem"

//! SEND event number used in the application
#define RSI_SEND_EVENT                  BIT(0)

//! Power Save Profile Mode
#define PSP_TYPE              RSI_MAX_PSP

//! Access point SSID to connect
#define SSID               "REDPINE_AP"

//! Security type
#define SECURITY_TYPE      0

//! Password
#define PSK                ""

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE          1

//! If DHCP mode is disabled give IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP          0x6500A8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY            0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK            0x00FFFFFF 

#endif

//! Device port number
#define DEVICE_PORT        5001

//! Remote port number
#define REMOTE_PORT        5001

//! Remote IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define REMOTE_IP_ADDRESS 0x020AA8C0




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

//! BT event: set when a packet from BT received 
#define RSI_BT_EVENT            BIT(0)

//! WLAN event: set when a  packet from WLAN device received
#define RSI_WLAN_EVENT           BIT(1)

//! Application buffers size
#define RSI_APP_BUF_SIZE        2000

//! Function prototypes

extern void rsi_wlan_get_mac_address(uint8_t *buffer, uint32_t length);
extern void prepare_udp_pkt_header(uint8_t *buffer, uint32_t length);
extern void rsi_udp_socket_create(uint16_t port_no);
extern int32_t rsi_wlan_check_mac_address(uint8_t *buffer, uint32_t length);
extern void rsi_wlan_set_ipaddress(uint32_t ip_addr,uint32_t network_mask, uint32_t gateway);
extern int32_t rsi_wlan_check_packet_type(uint8_t *buffer, uint32_t length);
extern void rsi_wlan_send_arp_response(uint8_t *buffer, uint32_t length);
extern void rsi_wlan_send_arp_request(uint8_t *buffer, uint32_t length);
extern void udp_socket_create(uint16_t port_no);
extern int32_t udp_send(uint16_t remote_port, uint8_t *buffer, uint32_t length);
extern void udp_recv(uint8_t *buffer, uint32_t length);
extern int32_t rsi_bt_app_send_to_wlan(uint8_t msg_type,uint8_t *buffer, uint32_t length);
extern void rsi_wlan_app_callbacks_init(void);
extern void rsi_packet_receive_notify_handler(uint32_t status, uint8_t *buffer, uint32_t length);
extern void rsi_prepare_udp_packet_header(uint8_t *buffer, uint16_t length);
extern uint16_t rsi_data_send_checksum(uint32_t csum, uint8_t * buff_ptr, uint32_t data_len);
extern uint32_t rsi_udp_start_checksum(uint8_t proto, uint16_t len, uint16_t port1, uint16_t port2,
    uint8_t *srcaddr, uint8_t *dstaddr);
extern uint16_t rsi_udp_end_checksum(uint32_t csum);
extern void rsi_wlan_app_task(void);
extern void rsi_wlan_async_data_recv(uint32_t sock_no, uint8_t *buffer, uint32_t length);
extern void rsi_join_fail_handler(uint16_t status,const uint8_t *buffer, const uint16_t length);
uint16_t convert_le_be(uint16_t num);


uint8_t null_mac_addr[6] ={0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t broadcast_mac_addr[6] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

//! client socket id
int32_t     client_socket;

//! server and client ip addresses
struct      rsi_sockaddr_in server_addr, client_addr;

#if !(DHCP_MODE)

//! device static ip address
uint32_t    ip_addr      = DEVICE_IP;

//! Network mask
uint32_t    network_mask = NETMASK;

//! Gateway
uint32_t    gateway      = GATEWAY;
#else
	uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif

typedef enum rsi_wlan_app_state_e
{
  RSI_WLAN_INITIAL_STATE          = 0,
  RSI_WLAN_UNCONNECTED_STATE      = 1,
  RSI_WLAN_CONNECTED_STATE        = 2,
  RSI_WLAN_IPCONFIG_DONE_STATE    = 3,
  RSI_WLAN_SOCKET_CONNECTED_STATE = 4
}rsi_wlan_app_state_t;

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
  uint32_t bt_pkt_length;

  //! application buffer
  uint8_t bt_buffer[RSI_APP_BUF_SIZE];

  //! to check application buffer availability
  uint8_t bt_buf_in_use;

  //! length of buffer to copy
  uint32_t wlan_pkt_length;

  //! application buffer
  uint8_t wlan_buffer[RSI_APP_BUF_SIZE];

  //! to check application buffer availability
  uint8_t wlan_buf_in_use;

  //! application events bit map 
  uint32_t event_map;

  //! wlan device details
  rsi_wlan_device_params_t device_params;

}rsi_wlan_app_cb_t;

//! application control block
rsi_wlan_app_cb_t rsi_wlan_app_cb;



void rsi_wlan_app_callbacks_init(void)
{
  //! Initialze join fail call back
  rsi_wlan_register_callbacks(RSI_JOIN_FAIL_CB, rsi_join_fail_handler);
  
  //! Initialize packet receive notify call back
  rsi_wlan_register_callbacks(RSI_WLAN_DATA_RECEIVE_NOTIFY_CB, rsi_packet_receive_notify_handler);
}


void  rsi_wlan_app_task(void)
{
  int32_t     status = RSI_SUCCESS;
  int32_t pkt_type = 0; 


  switch(rsi_wlan_app_cb.state)
  {
    case RSI_WLAN_INITIAL_STATE:
      {
        //! register call backs
        rsi_wlan_app_callbacks_init();

        //! update wlan application state
        rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE; 
      }
    case RSI_WLAN_UNCONNECTED_STATE:
      {  
        //! Connect to an Access point
        status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
        if(status != RSI_SUCCESS)
        {
          break;
        }
        else
        {
          //! update wlan application state
          rsi_wlan_app_cb.state = RSI_WLAN_CONNECTED_STATE; 

          //! Get MAC address of the Access point
          status = rsi_wlan_get(RSI_MAC_ADDRESS, rsi_wlan_app_cb.device_params.module_mac_addr, MAC_ADDRESS_SIZE);
          if(status != RSI_SUCCESS)
          {
            break;
          }

        }
      }
    case RSI_WLAN_CONNECTED_STATE:
      {
#if DHCP_MODE
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0,0);
#else
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0,0);
#endif
          
        status = rsi_wlan_power_save_profile(RSI_SLEEP_MODE_2, PSP_TYPE);
        if(status != RSI_SUCCESS)
        {
          break;
        }


        //! update wlan application state
        rsi_wlan_app_cb.state = RSI_WLAN_IPCONFIG_DONE_STATE; 

      }
    case RSI_WLAN_IPCONFIG_DONE_STATE:
      { 
        //! create UDP socket at the Station
        rsi_udp_socket_create((uint16_t)DEVICE_PORT);

        //! update wlan application state
        rsi_wlan_app_cb.state = RSI_WLAN_SOCKET_CONNECTED_STATE; 

      }
    case RSI_WLAN_SOCKET_CONNECTED_STATE:
      {
        if( rsi_wlan_app_cb.event_map & RSI_WLAN_EVENT)
        {
          //! To check the type of packet: ARP or IP packet
          pkt_type = rsi_wlan_check_packet_type(rsi_wlan_app_cb.wlan_buffer, rsi_wlan_app_cb.wlan_pkt_length);

          switch(pkt_type)
          {
            case ARP_OPTION_REQUEST:
              {
                //! Send ARP response for the received arp request 
                rsi_wlan_send_arp_response(rsi_wlan_app_cb.wlan_buffer, rsi_wlan_app_cb.wlan_pkt_length);
              }
              break;
            case ARP_OPTION_RESPONSE:
              {
                //! Parse packet to get mac address 
                rsi_wlan_get_mac_address(rsi_wlan_app_cb.wlan_buffer, rsi_wlan_app_cb.wlan_pkt_length);
              }
              break;
            case RSI_IP_PACKET:
              {
                //! Copy received packet to bt buffer
                udp_recv(&rsi_wlan_app_cb.wlan_buffer[UDP_HEADER_OFFSET], (rsi_wlan_app_cb.wlan_pkt_length - UDP_HEADER_OFFSET));

              }
              break;
            default:
              {

              }
              break;

          }
          //! make buffer in use
          rsi_wlan_app_cb.wlan_buf_in_use = 0;

          //! clear send event after data send is successful
          rsi_wlan_app_cb.event_map &= ~(RSI_WLAN_EVENT);

        }
        if( rsi_wlan_app_cb.event_map & RSI_BT_EVENT)
        {
          //! send data on socket
          status = udp_send(REMOTE_PORT, rsi_wlan_app_cb.bt_buffer, rsi_wlan_app_cb.bt_pkt_length);

          if(status)
          {
            //! make buffer in use
            rsi_wlan_app_cb.bt_buf_in_use = 0;

            //! clear send event after data send is successful
            rsi_wlan_app_cb.event_map &= ~(RSI_BT_EVENT);
          }
        }

      }
    default:
      break;
  }

}

int32_t rsi_bt_app_send_to_wlan(uint8_t msg_type, uint8_t *buffer, uint32_t length)
{
  switch(msg_type)
  {
    case RSI_DATA:
      {
        //! buffer is in use or not
        if(!rsi_wlan_app_cb.bt_buf_in_use)
        {
          //! if not in use

          //! copy the buffer to wlan app cb bt buffer
          memcpy(rsi_wlan_app_cb.bt_buffer, buffer, length); 

          //! hold length information
          rsi_wlan_app_cb.bt_pkt_length = length;

          //! make buffer in use
          rsi_wlan_app_cb.bt_buf_in_use = 1;

          //! raise event to wlan app task
          rsi_wlan_app_cb.event_map |= RSI_BT_EVENT;


        }
        else
          //!if buffer is in use
        {
          return -1;
          //! return error 
        }
      }
  }
  return 0;
}

void rsi_wlan_async_data_recv(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
  //! send packet to bt
  rsi_wlan_app_send_to_bt(RSI_DATA, buffer, length);
}


//! callback functions

//! rejoin failure call back handler in station mode
void rsi_join_fail_handler(uint16_t status, const uint8_t *buffer, const uint16_t length)
{
  //! update wlan application state
  rsi_wlan_app_cb.state = RSI_WLAN_UNCONNECTED_STATE;
}


//! packet receive notify call back handler in AP mode
void rsi_packet_receive_notify_handler(uint32_t status, uint8_t *buffer, uint32_t length)
{

  //! check destination mac address of the received packet 
  status = rsi_wlan_check_mac_address(buffer, length);

  if(status != RSI_SUCCESS)
  {
    return;
  }
  else
  {
    if(!rsi_wlan_app_cb.wlan_buf_in_use)
    {
      //! if not in use

      //! copy the buffer to wlan app cb wlan buffer
      memcpy(rsi_wlan_app_cb.wlan_buffer, buffer, length); 

      //! hold length information
      rsi_wlan_app_cb.wlan_pkt_length = length;

      //! make buffer in use
      rsi_wlan_app_cb.wlan_buf_in_use = 1;

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
int32_t rsi_wlan_check_mac_address(uint8_t *buffer, uint32_t length)
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


int32_t rsi_wlan_check_packet_type(uint8_t *buffer, uint32_t length)
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
    if (arp_packet->opcode == ARP_OPTION_REQUEST)
    {
      return ARP_OPTION_REQUEST; 
    }
    else if (arp_packet->opcode == ARP_OPTION_RESPONSE)
    {
      return ARP_OPTION_RESPONSE; 
    }
    else
    {
      return RSI_FAILURE; 
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

//! Function to get mac address from arp response
void rsi_wlan_get_mac_address(uint8_t *buffer, uint32_t length)
{
  rsi_arp_packet_t *arp_packet;
  rsi_ether_header_t *ether_header;


  ether_header = (rsi_ether_header_t *)buffer;

  //! Setup a pointer to the ARP message.  
  arp_packet = (rsi_arp_packet_t *)(buffer  + ETHERNET_HEADER_OFFSET);

  //! copy Access point mac address to source address of ARP response packet
  if(memcmp(ether_header->ether_dest, rsi_wlan_app_cb.device_params.module_mac_addr, MAC_ADDRESS_SIZE)== 0)
  { 
    //! Pick up the sender's physical address from the message.  
    memcpy(rsi_wlan_app_cb.device_params.sender_mac_addr,(arp_packet->sender_mac_addr),MAC_ADDRESS_SIZE); 
  }

}


//! Function to handle ARP request packet by sending arp response
void rsi_wlan_send_arp_request(uint8_t *buffer, uint32_t length)
{
  rsi_arp_packet_t *arp_packet;
  rsi_ether_header_t *ether_header;
  rsi_ip_header_t   *ip_header;
  rsi_udp_header_t   *udp_header;


  ether_header = (rsi_ether_header_t *)buffer;

  //! Setup a pointer to the ARP message.  
  arp_packet = (rsi_arp_packet_t *)(buffer  + ETHERNET_HEADER_OFFSET);

  //! Pick up sender ip address
  rsi_wlan_app_cb.device_params.sender_ip_addr = REMOTE_IP_ADDRESS; 

  //! Set the ARP message type to ARP response.  
  arp_packet->hardware_type = ARP_OPTION_REQUEST;
  arp_packet->protocol_type = RSI_IP_PACKET;
  arp_packet->hardware_size = 0x06; 
  arp_packet->protocol_size = 0x04; 
  arp_packet->opcode = ARP_OPTION_REQUEST;


  //! Now fill in the new source and destination information for the ARP response.  


  //! copy Access point mac address to source address of ARP response packet
  memcpy(ether_header->ether_source, rsi_wlan_app_cb.device_params.module_mac_addr, MAC_ADDRESS_SIZE);

  //! copy Access point mac address to sender mac address of ARP response packet
  memcpy(arp_packet->sender_mac_addr, rsi_wlan_app_cb.device_params.module_mac_addr,MAC_ADDRESS_SIZE);

  //! copy Access point ip address to source ip address of ARP response packet
  rsi_uint32_to_4bytes(arp_packet->sender_ip_addr, rsi_wlan_app_cb.device_params.module_ip_addr); 

  //! copy remote device mac address to destination address of ARP response packet
  memcpy(ether_header->ether_dest, broadcast_mac_addr, MAC_ADDRESS_SIZE); 

  //! copy remote device mac address to destination address of ARP response packet
  ether_header->ether_type = RSI_ARP_PACKET ; 
  
  //! copy remote device mac address to target address of ARP response packet
  memcpy(arp_packet->target_mac_addr, null_mac_addr, MAC_ADDRESS_SIZE); 

  //! copy remote device ip address to target ip address of ARP response packet
  rsi_uint32_to_4bytes(arp_packet->target_ip_addr, REMOTE_IP_ADDRESS); 

  //! Make sure the packet length is set properly.  
  length =  ARP_MESSAGE_SIZE + ETHERNET_HEADER_OFFSET;

  rsi_wlan_send_data(buffer, length);
}


//! Function to handle ARP request packet by sending arp response
void rsi_wlan_send_arp_response(uint8_t *buffer, uint32_t length)
{
  rsi_arp_packet_t *arp_packet;
  rsi_ether_header_t *ether_header;
  rsi_ip_header_t   *ip_header;
  rsi_udp_header_t   *udp_header;


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

//! To send packet received from bt to wlan staion connected
int32_t udp_send(uint16_t remote_port, uint8_t *buffer, uint32_t length)
{
  rsi_udp_pkt_header_t *udp_pkt;

  udp_pkt = (rsi_udp_pkt_header_t *)rsi_wlan_app_cb.wlan_buffer;

  memcpy ((uint8_t *)(udp_pkt + 1), buffer, length);

  //! prepare udp header for the given udp data packet
  prepare_udp_pkt_header((uint8_t *)udp_pkt, (length + sizeof(rsi_udp_pkt_header_t)));

  if(!memcmp(rsi_wlan_app_cb.device_params.sender_mac_addr,null_mac_addr,6))
  {
    //! prepare arp packet and send
    rsi_wlan_send_arp_request(rsi_wlan_app_cb.wlan_buffer, rsi_wlan_app_cb.wlan_pkt_length);
    return 0;
  }
  else
  {
    //! send this udp data packet to remote wlan client 
    rsi_wlan_send_data(rsi_wlan_app_cb.wlan_buffer,  length + sizeof(rsi_udp_pkt_header_t));

    return 1;
  }

}

//! To receive packet from Wlan station and forward it to BT device 
void udp_recv(uint8_t *buffer, uint32_t length)
{

  //! send packet to bt
  rsi_wlan_app_send_to_bt(RSI_DATA, buffer, length);
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

//! Prepare UDP packet to payload recieved from BT device
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
