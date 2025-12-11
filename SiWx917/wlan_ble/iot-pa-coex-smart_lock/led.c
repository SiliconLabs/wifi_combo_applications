/*******************************************************************************
 * @file  led.c
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

#include <led.h>
#include "sl_si91x_driver_gpio.h"
#include <stdio.h>

sl_gpio_t gpio_port_pin_1;

void led_init(const sl_rgb_led_t *handle){
sl_si91x_gpio_driver_enable_clock((sl_si91x_gpio_select_clock_t)M4CLK_GPIO);

  sl_si91x_gpio_pin_config_t sl_gpio_red_pin_config = { { handle->red->port, handle->red->pin }, GPIO_OUTPUT };
  sl_gpio_set_configuration(sl_gpio_red_pin_config);

  sl_si91x_gpio_pin_config_t sl_gpio_green_pin_config = { { handle->green->port, handle->green->pin }, GPIO_OUTPUT };
  sl_gpio_set_configuration(sl_gpio_green_pin_config);

  sl_si91x_gpio_pin_config_t sl_gpio_blue_pin_config = { { handle->blue->port, handle->blue->pin }, GPIO_OUTPUT };
  sl_gpio_set_configuration(sl_gpio_blue_pin_config);
}

void blue_led(void)
{
  sl_si91x_simple_rgb_led_off(&led_led0);
  gpio_port_pin_1.pin = LED0.blue->pin;
  gpio_port_pin_1.port = LED0.blue->port;
  sl_gpio_driver_clear_pin(&gpio_port_pin_1);
}

void red_led(void)
{
  sl_si91x_simple_rgb_led_off(&led_led0);
  gpio_port_pin_1.pin = LED0.red->pin;
  gpio_port_pin_1.port = LED0.red->port;
  sl_gpio_driver_clear_pin(&gpio_port_pin_1);
}

void green_led(void)
{
  sl_si91x_simple_rgb_led_off(&led_led0);
  gpio_port_pin_1.pin = LED0.green->pin;
  gpio_port_pin_1.port = LED0.green->port;
  sl_gpio_driver_clear_pin(&gpio_port_pin_1);
}

