/**
 * @file       rsi_scheduler.c
 * @version    0.1
 * @date       2015-Aug-15
 *
 * Copyright(C) Redpine Signals 2012
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contain function for scheduler implementation
 *
 * @section Description
 * This file contains function related to scheduler implementation
 *
 *
 */

/**
 * Includes
 */
 #ifndef ROM_WIRELESS
#include "rsi_driver.h"
/**
 * Global Variables
 */

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_scheduler_init(global_cb_t *global_cb_p, rsi_scheduler_cb_t *scheduler_cb)
 * @brief       Initializes scheduler
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   rsi_scheduler_cb_t scheduler_cb, pointer to scheduler structure
 * @return      void
 * @section description 
 * This function is used initialize scheduler
 */
void ROM_WL_rsi_scheduler_init(global_cb_t *global_cb_p, rsi_scheduler_cb_t *scheduler_cb)
{
  //! Set no event
  scheduler_cb->event_map = 0x0;

  //! Set no mask
  scheduler_cb->mask_map = 0xFFFFFFFF;
 
  if(global_cb_p->os_enabled == 1)
  {
    //! create semaphore
    RSI_SEMAPHORE_CREATE(&scheduler_cb->scheduler_sem, 0);
  }

  return;
}


uint32_t ROM_WL_rsi_get_event(global_cb_t *global_cb_p, rsi_scheduler_cb_t *scheduler_cb)
{
  uint32_t       active_int_event_map;
  
  rsi_reg_flags_t flags;
  
  //! Disable all the interrupts 
  flags = RSI_CRITICAL_SECTION_ENTRY();
 
  //! Get current event map after applying mask
  active_int_event_map = (scheduler_cb->event_map & scheduler_cb->mask_map);
  
  //! Enable all the interrupts
  RSI_CRITICAL_SECTION_EXIT(flags);
  
  return active_int_event_map;
  
}
  

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_scheduler(global_cb_t *global_cb_p, rsi_scheduler_cb_t *scheduler_cb)
 * @brief       scheduler to handle events
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   rsi_scheduler_cb_t *scheduler_cb, pointer to scheduler cb structure
 * @return      void
 * @section description 
 * This scheduler function handles events
 */
void ROM_WL_rsi_scheduler(global_cb_t *global_cb_p, rsi_scheduler_cb_t *scheduler_cb)
{
  uint32_t       event_no;
  rsi_event_cb_t *temp_event;

  rsi_driver_cb_t *rsi_driver_cb = global_cb_p->rsi_driver_cb;
 
  while(ROM_WL_rsi_get_event(global_cb_p, scheduler_cb))
  {
    //! Find event event 
    event_no = ROM_WL_rsi_find_event(global_cb_p, (scheduler_cb->event_map & scheduler_cb->mask_map));

    //! Get event handler
    temp_event = &rsi_driver_cb->event_list[event_no];

    if(temp_event->event_handler)
    {
      //! Call event handler
      temp_event->event_handler();
    }
    else 
    {
      while(1); //If Unregistered Events are raised, event handler is NULL
    }

    if(global_cb_p->os_enabled != 1)
    {
      break;
    }
  }
  
  if(global_cb_p->os_enabled == 1)
  {
    RSI_SEMAPHORE_WAIT(&scheduler_cb->scheduler_sem, 0);
  }
}
#endif
