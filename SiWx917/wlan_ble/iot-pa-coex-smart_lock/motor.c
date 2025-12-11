/*******************************************************************************
 * @file  motor.c
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
/*************************************************************************
 *
 */
#include <motor.h>
#include <rsi_debug.h>
#include "cmsis_os2.h"
#include "timer.h"
#include <app.h>


uint8_t pad_sel = 1;
extern osTimerId_t motor_handle;
extern volatile bool timer_stop_received;
extern bool device_in_sleep;
extern SystemStatus_t SystemStatus;
/*******************************************************************************
 * GPIO initialization function
 ******************************************************************************/
void sl_gpio_initialization(void)
{
  sl_si91x_gpio_enable_clock((sl_si91x_gpio_select_clock_t)M4CLK_GPIO); // Enable GPIO M4_CLK

  // Enable pad selection for GPIO pins
//   PadSelection-9 related GPIOS are pre-configured for Other Functions
  for (pad_sel = 1; pad_sel < MAX_PAD_SELECT; pad_sel++) {
    if (pad_sel != PAD_SELECT_9) {
      sl_si91x_gpio_enable_pad_selection(pad_sel);
    }
  }

  // Enable pad receiver for GPIO pins
  sl_si91x_gpio_enable_pad_receiver(PIN10);
  sl_si91x_gpio_enable_pad_receiver(PIN8);
  sl_si91x_gpio_enable_pad_receiver(PIN7);
  sl_si91x_gpio_enable_pad_receiver(PIN6);
  sl_si91x_gpio_enable_pad_receiver(GPIO_28);
  sl_si91x_gpio_enable_pad_receiver(GPIO_29);
  sl_si91x_gpio_enable_pad_receiver(GPIO_46);
  sl_si91x_gpio_enable_pad_receiver(GPIO_49);

  // Select pad driver strength for GPIO pins
  sl_si91x_gpio_select_pad_driver_strength(PIN8, (sl_si91x_gpio_driver_strength_select_t)GPIO_TWO_MILLI_AMPS);

  // Select pad driver disable state for GPIO pins
  sl_si91x_gpio_select_pad_driver_disable_state(PIN8, (sl_si91x_gpio_driver_disable_state_t)GPIO_HZ);

  // Set the pin mode for GPIO pins.
  sl_gpio_set_pin_mode(PORT0, PIN10, _MODE0, OUTPUT_VALUE);
  sl_gpio_set_pin_mode(PORT0, PIN8, _MODE0, OUTPUT_VALUE);
  sl_gpio_set_pin_mode(PORT0, PIN7, _MODE0, OUTPUT_VALUE);
  sl_gpio_set_pin_mode(PORT0, PIN6, _MODE0, OUTPUT_VALUE);
  sl_gpio_set_pin_mode(PORT0, GPIO_46, _MODE0, OUTPUT_VALUE);
  sl_gpio_set_pin_mode(PORT0, GPIO_49, _MODE0, 0);
  sl_gpio_set_pin_mode(PORT0, GPIO_29, _MODE0, OUTPUT_VALUE);
  sl_gpio_set_pin_mode(PORT0, GPIO_28, _MODE0, OUTPUT_VALUE);


  // Select the direction of GPIO pin whether Input/ Output
  sl_si91x_gpio_set_pin_direction(PORT0, PIN10, (sl_si91x_gpio_direction_t)GPIO_OUTPUT);
  sl_si91x_gpio_set_pin_direction(PORT0, PIN6, (sl_si91x_gpio_direction_t)GPIO_OUTPUT);
  sl_si91x_gpio_set_pin_direction(PORT0, PIN8, (sl_si91x_gpio_direction_t)GPIO_INPUT);
  sl_si91x_gpio_set_pin_direction(PORT0, PIN7, (sl_si91x_gpio_direction_t)GPIO_INPUT);
  sl_si91x_gpio_set_pin_direction(PORT0, GPIO_28, (sl_si91x_gpio_direction_t)GPIO_INPUT);
  sl_si91x_gpio_set_pin_direction(PORT0, GPIO_29, (sl_si91x_gpio_direction_t)GPIO_INPUT);
  sl_si91x_gpio_set_pin_direction(PORT0, GPIO_46, (sl_si91x_gpio_direction_t)GPIO_OUTPUT);
  sl_si91x_gpio_set_pin_direction(PORT0, GPIO_49, (sl_si91x_gpio_direction_t)GPIO_OUTPUT);
}

/*******************************************************************************
 * ULP GPIO initialization function
 ******************************************************************************/
void sl_gpio_ulp_initialization(void)
{
  sl_si91x_gpio_enable_clock((sl_si91x_gpio_select_clock_t)ULPCLK_GPIO); // Enable GPIO ULP_CLK

  // Enable pad receiver for ULP GPIO pins
  sl_si91x_gpio_enable_ulp_pad_receiver(ULP_PIN_1);
  sl_si91x_gpio_enable_ulp_pad_receiver(ULP_PIN_2);
  sl_si91x_gpio_enable_ulp_pad_receiver(ULP_PIN_4);

  // Select pad driver strength for ULP GPIO pins
  sl_si91x_gpio_select_ulp_pad_driver_strength(ULP_PIN_1, (sl_si91x_gpio_driver_strength_select_t)GPIO_TWO_MILLI_AMPS);

  // Select pad driver disable state for ULP GPIO pins
  sl_si91x_gpio_select_ulp_pad_driver_disable_state(ULP_PIN_1, (sl_si91x_gpio_driver_disable_state_t)GPIO_HZ);

  // Set the pin mode for ULP GPIO pins.
  sl_gpio_set_pin_mode(SL_ULP_GPIO_PORT, ULP_PIN_2, _MODE0, 0);
  sl_gpio_set_pin_mode(SL_ULP_GPIO_PORT, ULP_PIN_1, _MODE0, OUTPUT_VALUE);
  sl_gpio_set_pin_mode(SL_ULP_GPIO_PORT, ULP_PIN_4, _MODE0, OUTPUT_VALUE);
  // Set the pin direction for ULP GPIO pins.
  sl_si91x_gpio_set_pin_direction(SL_ULP_GPIO_PORT, ULP_PIN_2, (sl_si91x_gpio_direction_t)GPIO_OUTPUT);
  sl_si91x_gpio_set_pin_direction(SL_ULP_GPIO_PORT, ULP_PIN_1, (sl_si91x_gpio_direction_t)GPIO_INPUT);
  sl_si91x_gpio_set_pin_direction(SL_ULP_GPIO_PORT, ULP_PIN_4, (sl_si91x_gpio_direction_t)GPIO_INPUT);
}
/*==============================================*/
uint8_t check_lock_state(void)
{
#if SL_SI91X_TICKLESS_MODE
  if(device_in_sleep || AWS)
  {
    DEBUGINIT();

    sl_gpio_driver_deinit();
    sl_gpio_driver_init();

    sl_gpio_initialization();
  }
#endif
  // read hall sensor status to check if it is already locked
  if(sl_gpio_get_pin_output(PORT0,GPIO_28) == 1)
  {
      SystemStatus.lock_status = 1;
      send_initial_lock_state_to_ble();
      return 1;
  }
  else
  {
      SystemStatus.lock_status = 0;
      send_initial_lock_state_to_ble();
      return 0;
  }
}

void lock()
{
  led_init(&LED0);
  sl_si91x_simple_rgb_led_off(&led_led0);
  blue_led();
#if SILABS_SMARTLOCK_HARDWARE
  #if SL_SI91X_TICKLESS_MODE
    if(device_in_sleep || AWS)
    {
      DEBUGINIT();

      sl_gpio_driver_deinit();
      sl_gpio_driver_init();

      sl_gpio_initialization();
    }
    sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
  #endif
    if(sl_gpio_get_pin_output(PORT0,GPIO_28) == 0) //Hall
    {
      sl_gpio_set_pin_output (PORT0, GPIO_49);               // Motor enable pin
  #if LEFT_HANDED_DOOR
      sl_gpio_set_pin_output (PORT0, GPIO_46);               // Motor Phase anti-clockwise
  #else //RIGHT_HANDED_DOOR
      sl_gpio_clear_pin_output (PORT0, GPIO_46);             //Motor Phase clockwise
  #endif
      start_periodic_timer(motor_handle,LOCK_ROTATION_MS);
      while(!timer_stop_received);
      timer_stop_received = false;
      stop_motor();
    }
    else
    {
        LOG_PRINT("\r\nAlready locked\r\n");
    }
  #if SL_SI91X_TICKLESS_MODE
    sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
  #endif
#endif
}

void unlock()
{
  led_init(&LED0);
  sl_si91x_simple_rgb_led_off(&led_led0);
  red_led();
#if SILABS_SMARTLOCK_HARDWARE
  #if SL_SI91X_TICKLESS_MODE
    if(device_in_sleep || AWS)
    {
      DEBUGINIT();

      sl_gpio_driver_deinit();
      sl_gpio_driver_init();

      sl_gpio_initialization();
    }
    sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
  #endif
    if(sl_gpio_get_pin_output(PORT0,GPIO_28) == 1)  //HAll
    {
      sl_gpio_set_pin_output (PORT0, GPIO_49);               //Motor enable pin
  #if LEFT_HANDED_DOOR
      //keep the device in unlock position on power up by rotating motor clock wise
      sl_gpio_clear_pin_output (PORT0, GPIO_46);             //Motor Phase clockwise
  #else //RIGHT_HANDED_DOOR
      //keep the device in unlock position on power up by rotating motor anti-clock wise
      sl_gpio_set_pin_output (PORT0, GPIO_46);               // Motor Phase anti-clockwise
  #endif
      start_periodic_timer(motor_handle,LOCK_ROTATION_MS);
      while(!timer_stop_received);
      timer_stop_received = false;
      stop_motor();
      rsi_app_set_event(AUTO_LOCK_RECEIVED);
    }
    else
    {
      LOG_PRINT("\r\nAlready unlocked\r\n");
    }
  #if SL_SI91X_TICKLESS_MODE
    sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
  #endif
#endif
}

void stop_motor()
{
  sl_gpio_clear_pin_output (PORT0, GPIO_49);            //Motor enable pin
}
