/***************************************************************************/ /**
 * @file
 * @brief timer.c
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "rsi_common_apis.h"
#include "timer.h"

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/
extern uint8_t  periodic_timer_counter;

osStatus_t sl_status;
osTimerId_t retry_handle,motor_handle,auto_lock_handle;

void create_all_timers(void)
{
  retry_handle     = osTimerNew ((void *)periodic_retry_timer_callback, osTimerOnce, (int *)1, 0);
  motor_handle     = osTimerNew ((void *)periodic_retry_timer_callback, osTimerOnce, (int *)2, 0);
  auto_lock_handle = osTimerNew ((void *)periodic_retry_timer_callback, osTimerOnce, (int *)3, 0);
}

void start_periodic_timer(osTimerId_t handle,uint32_t time_ms)
{
  if(handle == NULL)
  {
      LOG_PRINT("handle does not exists\r\n");
  }

  sl_status = osTimerStart (handle, time_ms);
  if(sl_status != osOK)
  {
      LOG_PRINT("Failed to start timer, handle %d\r\n",(int)handle);
  }
}

void periodic_stop_timer(osTimerId_t handle)
{
  if(osTimerIsRunning(handle)){
//      LOG_PRINT("stop timer\r\n");
      osTimerStop(handle);
      periodic_timer_counter = 0;
   }
}
bool is_timer_running(osTimerId_t handle)
{
  if(osTimerIsRunning(handle) == true){
      return true;
  }
  return false;
}
