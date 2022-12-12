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
 * Includes
 */
#ifdef LINUX_PLATFORM
#ifdef RSI_ZB_ENABLE
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

void (*timer_func_handler_pntr)(void);

#ifdef __linux__

void timer_handler(int);

struct itimerval timervalue;
struct itimerval timervalue_read;

struct sigaction new_handler, old_handler;

void timer_sig_handler(int);

#define CLOCKID CLOCK_REALTIME

int start_timer(int uSec, void (*timer_func_handler)(void))
{
  timer_func_handler_pntr = timer_func_handler;

  //! Single Shot
  timervalue.it_interval.tv_sec  = 0;
  timervalue.it_interval.tv_usec = 0;

  timervalue.it_value.tv_sec  = uSec / 1000000;
  timervalue.it_value.tv_usec = 0;
  if (setitimer(ITIMER_REAL, &timervalue, NULL)) {
    printf("\nsetitimer() error\n");
    return (1);
  }

  new_handler.sa_handler = &timer_sig_handler;
  new_handler.sa_flags   = SA_NOMASK;
  if (sigaction(SIGALRM, &new_handler, &old_handler)) {
    printf("\nsigaction() error\n");
    return (1);
  }

  return (0);
}

int read_time(void)
{
  getitimer(ITIMER_REAL, &timervalue_read);
  return timervalue_read.it_value.tv_usec;
}

void timer_sig_handler(int arg)
{
  timer_func_handler_pntr();
}

void stop_timer(void)
{
  timervalue.it_interval.tv_sec  = 0;
  timervalue.it_interval.tv_usec = 0;
  timervalue.it_value.tv_sec     = 0;
  timervalue.it_value.tv_usec    = 0;
  setitimer(ITIMER_REAL, &timervalue, NULL);

  sigaction(SIGALRM, &old_handler, NULL);
}

/*===================================================*/
/**
 * @fn           void rsi_zigb_delay_ms(uint32_t delay)
 * @brief        create delay in micro seconds
 * @param[in]    delay, timer delay in micro seconds
 * @param[out]   none
 * @return       none
 * @description  This HAL API should contain the code to create delay in micro seconds
 *
 */
void rsi_zigb_delay_ms(int delay)
{
  usleep(delay * 1000);
}

#endif
#endif
#endif
