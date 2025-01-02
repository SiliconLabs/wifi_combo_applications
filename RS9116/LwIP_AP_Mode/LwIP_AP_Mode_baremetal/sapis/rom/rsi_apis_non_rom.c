#include "rsi_apis_rom.h"
#include "rsi_driver.h"
#include "rsi_socket.h"
#include "rsi_wlan_non_rom.h"
#include "rsi_apis_rom.h"

global_cb_t global_cb;
global_cb_t *global_cb_p;
rsi_wlan_cb_non_rom_t wlan_cb_non_rom;
struct rom_apis_s  rom_apis;
extern struct rom_apis_s  *rom_apis_p;
#ifndef ROM_WIRELESS
const struct ROM_WL_API_S api_wl_s={
 //! Socket APIs
  &ROM_WL_socket_async, 
  &ROM_WL_socket,  
  &ROM_WL_bind,  
  &ROM_WL_connect,  
  &ROM_WL_listen,  
  &ROM_WL_accept,  
  &ROM_WL_recvfrom,  
  &ROM_WL_recv,  
  &ROM_WL_sendto,  
  &ROM_WL_sendto_async,  
  &ROM_WL_send,  
  &ROM_WL_send_async,  
  &ROM_WL_shutdown,  
  &ROM_WL_rsi_get_application_socket_descriptor,  
  &ROM_WL_rsi_clear_sockets,
  &ROM_WL_calculate_buffers_required,  
	&ROM_WL_rsi_socket_create,
	&ROM_WL_calculate_length_to_send,

  //! Scheduler
  &ROM_WL_rsi_scheduler_init,  
  &ROM_WL_rsi_get_event,  
  &ROM_WL_rsi_scheduler,  

  //! Packet allocation management
  &ROM_WL_rsi_pkt_pool_init,  
  &ROM_WL_rsi_pkt_alloc,  
  &ROM_WL_rsi_pkt_free,  
  &ROM_WL_rsi_is_pkt_available,  

  //! Queue Management
  &ROM_WL_rsi_queues_init,  
  &ROM_WL_rsi_enqueue_pkt,  
  &ROM_WL_rsi_dequeue_pkt,
  &ROM_WL_rsi_check_queue_status,  
  &ROM_WL_rsi_block_queue,  
  &ROM_WL_rsi_unblock_queue,  


  &ROM_WL_rsi_driver_send_data,  
  &ROM_WL_rsi_driver_process_recv_data, 

  //! Events
  &ROM_WL_rsi_set_event,  
  &ROM_WL_rsi_clear_event,  
  &ROM_WL_rsi_mask_event,  
  &ROM_WL_rsi_unmask_event,  
  &ROM_WL_rsi_find_event,  
  &ROM_WL_rsi_register_event, 
	
	//! Utils
  &ROM_WL_rsi_uint16_to_2bytes,
	&ROM_WL_rsi_uint32_to_4bytes,
	&ROM_WL_rsi_bytes2R_to_uint16,
	&ROM_WL_rsi_bytes4R_to_uint32,

#ifdef RSI_M4_INTERFACE
	//! M4 HAL
	&ROM_WL_rsi_m4_interrupt_isr,
	&ROM_WL_mask_ta_interrupt,
	&ROM_WL_unmask_ta_interrupt,
  &ROM_WL_rsi_frame_read,
  &ROM_WL_rsi_frame_write,
	&ROM_WL_rsi_submit_rx_pkt,
#endif
  &ROM_WL_rsi_enqueue_pkt_from_isr,
  &ROM_WL_rsi_set_event_from_isr,
  &ROM_WL_rsi_unmask_event_from_isr
};

ROM_WL_API_T *api_wl  = NULL;
#endif

extern rsi_socket_info_t *rsi_socket_pool;

void rom_init(void)
{
#ifndef ROM_WIRELESS
	api_wl                  = &api_wl_s;
#endif	

  global_cb_p             = &global_cb;

  global_cb_p->rsi_driver_cb   = rsi_driver_cb;
#ifdef RSI_WLAN_ENABLE
  global_cb_p->rsi_socket_pool = rsi_socket_pool;
#endif
  global_cb_p->rom_apis_p      = &rom_apis;
  global_cb_p->no_of_sockets   = RSI_NUMBER_OF_SOCKETS;
#ifdef RSI_LITTLE_ENDIAN
  global_cb_p->endian          = IS_LITTLE_ENDIAN;
#else
  global_cb_p->endian          = IS_BIG_ENDIAN;
#endif

#ifdef RX_BUFFER_MEM_COPY  
  global_cb_p->rx_buffer_mem_copy = 1;
#else
  global_cb_p->rx_buffer_mem_copy = 0;
#endif

  global_cb_p->rsi_tcp_listen_min_backlog = RSI_TCP_LISTEN_MIN_BACKLOG;

#ifdef RSI_WITH_OS
  global_cb_p->os_enabled         = 1;
#else 
  global_cb_p->os_enabled         = 0;
#endif

  global_cb_p->rsi_max_num_events       = RSI_MAX_NUM_EVENTS;

#ifdef RSI_M4_INTERFACE
  global_cb_p->tx_desc                  = &tx_desc[0];
  global_cb_p->rx_desc                  = &rx_desc[0];
#endif


  global_cb_p->rom_apis_p->rsi_mutex_create          = rsi_mutex_create;
  global_cb_p->rom_apis_p->rsi_mutex_lock            = rsi_mutex_lock;
  global_cb_p->rom_apis_p->rsi_mutex_unlock          = rsi_mutex_unlock;
  global_cb_p->rom_apis_p->rsi_semaphore_create      = rsi_semaphore_create;
  global_cb_p->rom_apis_p->rsi_critical_section_entry = rsi_critical_section_entry;
  global_cb_p->rom_apis_p->rsi_critical_section_exit = rsi_critical_section_exit;
  global_cb_p->rom_apis_p->rsi_semaphore_wait        = rsi_semaphore_wait;
  global_cb_p->rom_apis_p->rsi_semaphore_post        = rsi_semaphore_post;
#ifdef RSI_M4_INTERFACE 
	global_cb_p->rom_apis_p->rsi_semaphore_post_from_isr 		= rsi_semaphore_post_from_isr;
	global_cb_p->rom_apis_p->rsi_mask_ta_interrupt	   			= rsi_mask_ta_interrupt;
	global_cb_p->rom_apis_p->rsi_unmask_ta_interrupt   			= rsi_unmask_ta_interrupt;
#endif
#ifdef RSI_WLAN_ENABLE
  global_cb_p->rom_apis_p->rsi_driver_wlan_send_cmd  = rsi_driver_wlan_send_cmd;
  global_cb_p->rom_apis_p->rsi_driver_send_data      = rsi_driver_send_data;
#endif
  global_cb_p->rom_apis_p->rsi_delay_ms              = rsi_delay_ms;
#ifdef RSI_M4_INTERFACE
  global_cb_p->rom_apis_p->rsi_assertion             = rsi_assertion;
#endif

#ifdef RSI_WLAN_ENABLE
#ifdef ROM_WIRELESS
  global_cb_p->rom_apis_p->ROM_WL_rsi_socket_create        = ROMAPI_WL->rsi_socket_create;
	global_cb_p->rom_apis_p->ROM_WL_calculate_length_to_send = ROMAPI_WL->calculate_length_to_send;
#else
  global_cb_p->rom_apis_p->ROM_WL_rsi_socket_create        = api_wl->rsi_socket_create;
	global_cb_p->rom_apis_p->ROM_WL_calculate_length_to_send = api_wl->calculate_length_to_send;
#endif
#ifndef RSI_M4_INTERFACE
	if((RSI_TCP_IP_FEATURE_BIT_MAP & TCP_IP_FEAT_EXTENSION_VALID) && (RSI_EXT_TCPIP_FEATURE_BITMAP & EXT_TCP_IP_CERT_BYPASS))
	{
		global_cb_p->rom_apis_p->ROM_WL_rsi_socket_create        = rsi_socket_create_async;
	}
#endif
#endif

}
