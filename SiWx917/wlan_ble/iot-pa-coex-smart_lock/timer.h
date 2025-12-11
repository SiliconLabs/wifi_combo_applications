/***************************************************************************/ /**
 * @file
 * @brief timer.h
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

#ifndef TIMER_H
#define TIMER_H

#include "cmsis_os2.h"
#include "rsi_debug.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "FreeRTOS.h"
#include "timers.h"

void create_all_timers(void);
void start_periodic_timer(osTimerId_t retry_handle,uint32_t time_ms);
void periodic_retry_timer_callback(int arg);
void periodic_stop_timer(osTimerId_t handle);
bool is_timer_running(osTimerId_t handle);


#endif /* TIMER_H */
