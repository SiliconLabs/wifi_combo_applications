/**
 * @file       rsi_event.c
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
 * @brief This file contains events handling
 *
 * @section Description
 * This file contains events handling
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
 * @fn          void ROM_WL_rsi_set_event(global_cb_t *global_cb_p, uint32_t event_num)
 * @brief       Sets an event
 * @param[in]   uint32_t event_num, event number to be set
 * @param[in]   global_cb_t *global_cb_p,pointer to the common buffer
 * @return      void
 * @section description 
 * This function sets an event
 */
void ROM_WL_rsi_set_event(global_cb_t *global_cb_p, uint32_t event_num)
{
  rsi_driver_cb_t *rsi_driver_cb = global_cb_p->rsi_driver_cb;
  
#ifdef RSI_M4_INTERFACE
  //! mask P2P interrupt
  RSI_MASK_TA_INTERRUPT();
#endif

  //! Set the event bit in bitmap
  rsi_driver_cb->scheduler_cb.event_map |= BIT(event_num);
  
#ifdef RSI_M4_INTERFACE
  //! unmask P2P interrupt
  RSI_UNMASK_TA_INTERRUPT();
#endif  
  
  if(global_cb_p->os_enabled == 1)
  {
    //! Post the semaphore
    RSI_SEMAPHORE_POST(&rsi_driver_cb->scheduler_cb.scheduler_sem);
  }
}

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_set_event_from_isr(global_cb_t *global_cb_p, uint32_t event_num)
 * @brief       Sets an event from ISR
 * @param[in]   uint32_t event_num, event number to be set
 * @param[in]   global_cb_t *global_cb_p,pointer to the common buffer
 * @return      void
 * @section description
 * This function sets an event from ISR
 */
void ROM_WL_rsi_set_event_from_isr(global_cb_t *global_cb_p, uint32_t event_num)
{
	rsi_driver_cb_t *rsi_driver_cb = global_cb_p->rsi_driver_cb;

	//! Set the event bit in scheduler bitmap
	rsi_driver_cb->scheduler_cb.event_map |= BIT(event_num);

	if(global_cb_p->os_enabled == 1)
	{
		//! Post the semaphore
		RSI_SEMAPHORE_POST_FROM_ISR(&rsi_driver_cb->scheduler_cb.scheduler_sem);
	}
	else
	{
		//! Post the semaphore
		RSI_SEMAPHORE_POST(&rsi_driver_cb->scheduler_cb.scheduler_sem);
	}
}

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_clear_event(global_cb_t *global_cb_p,uint32_t event_num)
 * @brief       Clears an event
 * @param[in]   uint32_t event_num, event number to clear
 * @param[in]   global_cb_t *global_cb_p,pointer to the common buffer
 * @return      void
 * @section description 
 * This function clears an event
 */

void ROM_WL_rsi_clear_event(global_cb_t *global_cb_p,uint32_t event_num)
{

  rsi_reg_flags_t flags;

  //! Disable all the interrupts 
  flags = RSI_CRITICAL_SECTION_ENTRY();

  //! Reset the event bit in bitmap
  global_cb_p->rsi_driver_cb->scheduler_cb.event_map &= ~BIT(event_num);

  //! Enable all the interrupts
  RSI_CRITICAL_SECTION_EXIT(flags);

  return;
}

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_mask_event(global_cb_t *global_cb_p,uint32_t event_num)
 * @brief       Masks an event
 * @param[in]   uint32_t event_num, event number to mask
 * @param[in]   global_cb_t *global_cb_p,pointer to the common buffer
 * @return      void
 * @section description 
 * This function masks an event
 */
void ROM_WL_rsi_mask_event(global_cb_t *global_cb_p,uint32_t event_num)
{
  rsi_reg_flags_t flags;

  //! Disable all the interrupts 
  flags = RSI_CRITICAL_SECTION_ENTRY();
  
  //! Reset event bit in mask bitmap
  global_cb_p->rsi_driver_cb->scheduler_cb.mask_map &= ~BIT(event_num);

  //! Enable all the interrupts
  RSI_CRITICAL_SECTION_EXIT(flags);

  return;
}

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_unmask_event_from_isr(global_cb_t *global_cb_p, uint32_t event_num)
 * @brief       Unmasks an event from ISR
 * @param[in]   uint32_t event_num, event number to unmask
 * @param[in]   global_cb_t *global_cb_p,pointer to the common buffer
 * @return      void
 * @section description
 * This function is used to unmask an event from ISR
 */

void ROM_WL_rsi_unmask_event_from_isr(global_cb_t *global_cb_p, uint32_t event_num)
{

	//! set event bit in mask bitmap
	global_cb_p->rsi_driver_cb->scheduler_cb.mask_map |= BIT(event_num);

	if(global_cb_p->os_enabled == 1)
	{
		//! Post the semaphore
		RSI_SEMAPHORE_POST_FROM_ISR(&global_cb_p->rsi_driver_cb->scheduler_cb.scheduler_sem);
	}
	else
	{
		//! Post the semaphore
		RSI_SEMAPHORE_POST(&global_cb_p->rsi_driver_cb->scheduler_cb.scheduler_sem);
	}
}

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_unmask_event(global_cb_t *global_cb_p, uint32_t event_num)
 * @brief       Unmasks an event
 * @param[in]   uint32_t event_num, event number to unmask
 * @param[in]   global_cb_t *global_cb_p,pointer to the common buffer
 * @return      void
 * @section description 
 * This function is used to unmask an event
 */
void ROM_WL_rsi_unmask_event(global_cb_t *global_cb_p, uint32_t event_num)
{
#ifdef RSI_M4_INTERFACE
  //! mask P2P interrupt
  RSI_MASK_TA_INTERRUPT();
#endif
  
  //! set event bit in mask bitmap
  global_cb_p->rsi_driver_cb->scheduler_cb.mask_map |= BIT(event_num);
  
#ifdef RSI_M4_INTERFACE
  //! unmask P2P interrupt
  RSI_UNMASK_TA_INTERRUPT();
#endif

  if(global_cb_p->os_enabled == 1)
  {
	//! Post the semaphore
	RSI_SEMAPHORE_POST(&global_cb_p->rsi_driver_cb->scheduler_cb.scheduler_sem);
  }
}

/*====================================================*/
/**
 * @fn          uint32_t ROM_WL_rsi_find_event(global_cb_t *global_cb_p, uint32_t event_map)
 * @brief       Returns the event set from the event map
 * @param[in]   uint32_t event_map, event map
 * @param[in]   global_cb_t *global_cb_p,pointer to the common buffer
 * @return      void
 * @section description 
 * This function is used find the event which is set from the map
 */
uint32_t ROM_WL_rsi_find_event(global_cb_t *global_cb_p, uint32_t event_map)
{
  uint8_t i;

  for(i=0 ; i < global_cb_p->rsi_max_num_events ; i++)
  {
    if(event_map & BIT(i))
    {
      break;
    }
  }
  return i;
}

/*====================================================*/
/**
 * @fn          uint16_t ROM_WL_rsi_register_event(global_cb_t *global_cb_p, uint32_t event_id, void (*event_handler_ptr)(void))
 * @brief       Registers the event
 * @param[in]   uint32_t event_id, event number which needs to be registered
 * @param[in]   global_cb_t *global_cb_p,pointer to the common buffer
 * @param[in]   void (*event_handler_ptr)(void), event handler which needs to be registered for a given event
 * @return      
 *                1 -  error
 *                0 - Success
 * @section description 
 * This function is used to register the event
 */
uint16_t ROM_WL_rsi_register_event(global_cb_t *global_cb_p, uint32_t event_id, void (*event_handler_ptr)(void))
{
  if((event_id > global_cb_p->rsi_max_num_events) || (global_cb_p->rsi_driver_cb->event_list[event_id].event_handler != NULL))
  {
    /*
     *Return , if the event number exceeds the RSI_MAX_NUM_EVENTS ,or
     * the event is already registered
     */
    return 1;
  }

  //! Register event handler
  global_cb_p->rsi_driver_cb->event_list[event_id].event_handler = event_handler_ptr;

  return 0;
}

#endif
