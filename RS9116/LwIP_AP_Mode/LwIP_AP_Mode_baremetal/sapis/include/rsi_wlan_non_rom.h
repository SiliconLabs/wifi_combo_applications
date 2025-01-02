/**
 * @file    rsi_wlan_non_rom.h
 * @version 0.1
 * @date    15 July,2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contain declarations for non rom structures for wlan
 *
 *  @section Description  This file contains declarations for non rom structures for wlan
 *  implementation.
 *
 *
 */

#ifndef RSI_WLAN_NON_ROM_H
#define RSI_WLAN_NON_ROM_H

#include "rsi_apis_rom.h"
//! Max chunk size 
#define CHUNK_SIZE 1472
#define MIN(x,y) ((x)<(y)?(x):(y))
//! driver WLAN control block
typedef struct rsi_wlan_cb_non_rom_s 
{
	uint32_t tls_version;
  uint8_t join_bssid_non_rom[6];
  //! to identify on which socket data is present
	uint32_t socket_bitmap;
#ifdef RSI_UART_INTERFACE
  uint8_t rsi_uart_data_ack_check;
#endif
  uint32_t *fd_set_ptr; 
  
  void (* sock_select_callback)(uint32_t fd_read);
}rsi_wlan_cb_non_rom_t;

typedef struct rsi_socket_info_non_rom_s
{
  uint8_t max_tcp_retries;

  uint8_t vap_id;

  //! TCP keepalive initial timeout
  uint8_t     tcp_keepalive_initial_time[2];
	
	//! Asynchronous accpet respone handler
	void   (* accept_call_back_handler)(int32_t sock_id, int16_t dest_port, uint8_t *ip_addr, int16_t ip_version);
	//! Asynchronous data tx done call back handler
	void (*rsi_sock_data_tx_done_cb)(uint8_t sockID,int16_t status,uint16_t total_data_sent);
	

  //! total data to be send
	int8_t	*buffer;
  //! total data sent
	int32_t offset;
  //! remaining data to be send
	int32_t rem_len;
  //! flag which indicates more data is present 
	uint32_t more_data;
  //! user flags 
	int32_t flags;
  
  uint8_t    ssl_bitmap;
  
  uint8_t    high_performance_socket;
	
}rsi_socket_info_non_rom_t;

/*==================================================*/
//! Asynchronous message for connection state notification
typedef struct rsi_state_notification_s
{
	uint8_t TimeStamp[4];
	uint8_t StateCode;
	uint8_t reason_code;
	uint8_t rsi_channel;
	uint8_t rsi_rssi;
	uint8_t rsi_bssid[6];
} rsi_state_notification_t;

/*==================================================*/
#define WLAN_MODULE_STATES		11
#define WLAN_REASON_CODES      6
struct rsi_bit_2_string
{
	uint8_t bit;
	char *string;
};
static const struct rsi_bit_2_string STATE[WLAN_MODULE_STATES] =
{
		{
				0x10,
				"Beacon Loss (Failover Roam)"
		},
		{
				0x20,
				"De-authentication (AP induced Roam / Disconnect from supplicant"
		},
		{
				0x50,
				"Current AP is best"
		},
		{
				0x60,
				"Better AP found"
		},
		{
				0x70,
				"No AP found"
		},
		{
				0x80,
				"Associated"
		},
		{
				0x90,
				"Unassociated"
		},
		{
				0x01,
				"Authentication denial"
		},
		{
				0x02,
				"Association denial"
		},
		{
				0x03,
				"AP not present"
		},
		{
				0x05,
				"WPA2 key exchange failed"
		}

};


static const struct rsi_bit_2_string REASONCODE[WLAN_REASON_CODES] =
{
		{
				0x01,
				"Authentication denial"
		},
		{
				0x02,
				"Association denial"
		},
		{
				0x10,
				"Beacon Loss (Failover Roam)"
		},
		{
				0x20,
				"De-authentication (AP induced Roam/Deauth from supplicant)"
		},
		{
				0x07,
				"PSK not configured"
		},
		{
				0x05,
				"Roaming not enabled"
		},

};

#ifndef RSI_M4_INTERFACE
int32_t rsi_socket_create_async(global_cb_t *global_cb_p, int32_t sockID, int32_t type, int32_t backlog);
#endif
int32_t  rsi_accept_non_rom(int32_t sockID, struct rsi_sockaddr *ClientAddress, int32_t *addressLength);
int rsi_fd_isset(uint32_t fd, struct rsi_fd_set_s *fds_p);
void rsi_set_fd(uint32_t fd, struct rsi_fd_set_s *fds_p);
void rsi_fd_clr(uint32_t fd, struct rsi_fd_set_s *fds_p);
#ifdef RSI_IPV6_ENABLE
int32_t  rsi_socket_async_non_rom(global_cb_t *global_cb_p, int32_t protocolFamily, int32_t type, int32_t protocol, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length));
int32_t rsi_driver_send_data_non_rom(global_cb_t *global_cb_p, uint32_t sockID, uint8_t* buffer, uint32_t length, struct rsi_sockaddr *destAddr);
#endif
#endif
