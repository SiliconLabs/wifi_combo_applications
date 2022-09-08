/***************************************************************************//**
 * @file rsi_power_manager_app.c
 * @brief 
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

/*================================================================================
 * @brief : This file contains functions to configure EFR32 into various power modes 
 * @section Description :
 * This application will demonstrate the low power functionality, for both the
 * EFR32 and the RS9116W.
 =================================================================================*/


#include "sl_power_manager.h"
#include "sl_power_manager_debug.h"
#include "sl_sleeptimer.h"
#include "em_emu.h"


/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define EM_EVENT_MASK_ALL  (SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM0   \
                            | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM0  \
                            | SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM1 \
                            | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM1  \
                            | SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM2 \
                            | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM2  \
                            | SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM3 \
                            | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM3)

#define SLEEP_TIME 5000      //Host MCU sleep time in ms

/*******************************************************************************
 *******************  LOCAL FUNCTION DECLARATIONS   ****************************
 ******************************************************************************/

//! sleeptimer callback
static void timer_callback(sl_sleeptimer_timer_handle_t *handle,
                           void *data);

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/
//! Energy mode enumerations
typedef enum {
  EMODE_0,
  EMODE_1,
  EMODE_2,
  EMODE_3,
  EMODE_4,
  NUM_EMODES
} energy_mode_enum_t;



static sl_sleeptimer_timer_handle_t my_timer;

// Flag to indicate to power manager if application can sleep
// The application will start in EM0
static bool ok_to_sleep = true;
// Flag to indicate to power manager if the application should return to sleep
// after an interrupt
static sl_power_manager_on_isr_exit_t isr_ok_to_sleep = SL_POWER_MANAGER_IGNORE;
// Target energy mode
//static sl_power_manager_em_t em_mode = SL_POWER_MANAGER_EM2;
static sl_power_manager_em_t em_mode = SL_POWER_MANAGER_EM2;
// Start selected energy mode test.
static volatile int start_test=true;
static volatile int is_efr_wakeup=false;
/*******************************************************************************
 *********************   LOCAL FUNCTION PROTOTYPES   ***************************
 ******************************************************************************/



/***************************************************************************//**
 * Function called when sleep timer expires.
 ******************************************************************************/

static void timer_callback(sl_sleeptimer_timer_handle_t *handle,
                           void *data)
{
  (void)handle;
  (void)data;
  // when the timer expires, prevent return to sleep
  ok_to_sleep = false;
  isr_ok_to_sleep = SL_POWER_MANAGER_WAKEUP;
  // Clear requirements
  if (em_mode == SL_POWER_MANAGER_EM1
      || em_mode == SL_POWER_MANAGER_EM2) {
    sl_power_manager_remove_em_requirement(em_mode);

  }
  start_test=true;
  is_efr_wakeup=true;
}



/***************************************************************************//**
 * Hook for power manager
 ******************************************************************************/
bool app_is_ok_to_sleep(void)
{
  // return false to prevent sleep mode and force EM0
  // return true to indicate to power manager that application can sleep
  return ok_to_sleep;
}

/***************************************************************************//**
 * Hook for power manager
 ******************************************************************************/
sl_power_manager_on_isr_exit_t app_sleep_on_isr_exit(void)
{
  // flag used by power manager to determine if device can return to sleep
  // following interrupt
  //return isr_ok_to_sleep;
  return 4;
}

//! Function to select power save mode for Host MCU
//! Initializes sleep timer for timer based wake up.
int rsi_host_pwmode_timer_init()
{
  int status=0;
  if(start_test){
      start_test=false;
      em_mode=EMODE_0;
      // Set flag returned by app_is_ok_to_sleep() to indicate
      // to power manager that application can sleep
      ok_to_sleep = true;
      // Set flag returned by app_sleep_on_isr_exit() to indicate
      // to power manager that application can return to sleep after
      // interrupt
      isr_ok_to_sleep = SL_POWER_MANAGER_SLEEP;
      em_mode = SL_POWER_MANAGER_EM2;
      // Ensure app stays in selected energy mode

      //Sleep timer functions in EM2
      status=sl_sleeptimer_start_timer_ms(&my_timer,
                                   SLEEP_TIME,
                                   timer_callback,
                                   (void *)NULL,
                                   0,
                                   SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG);


  }
  return status;

}

//! Function to select power save mode for Host MCU
int rsi_host_pwmode_init()
{
  if(start_test){
      start_test=false;
      em_mode=EMODE_0;
      // Set flag returned by app_is_ok_to_sleep() to indicate
      // to power manager that application can sleep
      ok_to_sleep = true;
      // Set flag returned by app_sleep_on_isr_exit() to indicate
      // to power manager that application can return to sleep after
      // interrupt
      isr_ok_to_sleep = SL_POWER_MANAGER_SLEEP;
      em_mode = SL_POWER_MANAGER_EM3;

  }
  return 0;
}



