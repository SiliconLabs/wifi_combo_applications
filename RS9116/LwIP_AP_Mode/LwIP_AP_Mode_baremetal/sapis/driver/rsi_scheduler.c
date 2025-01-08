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
#include "rsi_driver.h"
/**
 * Global Variables
 */

/*====================================================*/
/**
 * @fn          uint16_t rsi_scheduler_init(rsi_scheduler_cb_t *scheduler_cb)
 * @brief       Initializes scheduler
 * @param[in]   rsi_scheduler_cb_t scheduler_cb, pointer to scheduler structure
 * @return      void
 * @section description 
 * This function is used initialize scheduler
 */
void rsi_scheduler_init(rsi_scheduler_cb_t *scheduler_cb)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_scheduler_init(global_cb_p, scheduler_cb);
#else
  api_wl->rsi_scheduler_init(global_cb_p, scheduler_cb);
#endif
  return;
}


uint32_t rsi_get_event(rsi_scheduler_cb_t *scheduler_cb)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_get_event(global_cb_p, scheduler_cb);
#else
  return api_wl->rsi_get_event(global_cb_p,scheduler_cb);
#endif
}
  

/*====================================================*/
/**
 * @fn          void rsi_scheduler(rsi_scheduler_cb_t *scheduler_cb)
 * @brief       scheduler to handle events
 * @param[in]   rsi_scheduler_cb_t *scheduler_cb, pointer to scheduler cb structure
 * @return      void
 * @section description 
 * This scheduler function handles events
 */
void rsi_scheduler(rsi_scheduler_cb_t *scheduler_cb)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_scheduler(global_cb_p, scheduler_cb);
#else
  api_wl->rsi_scheduler(global_cb_p, scheduler_cb);
#endif
}

