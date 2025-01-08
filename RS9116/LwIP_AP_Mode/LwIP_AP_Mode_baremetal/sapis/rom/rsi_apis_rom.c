#include "rsi_apis_rom.h"
#include "rsi_driver.h"
#include "rsi_socket.h"


#ifndef ROM_WIRELESS


const struct ROM_WL_API_S wl_api={
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
};
#endif
