/*******************************************************************************
 * @file  led.h
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

#ifndef LED_H_
#define LED_H_

#include "app.h"

#include "sl_si91x_rgb_led_instances.h"
#include "sl_driver_gpio.h"

#define LED0 led_led0

void led_init(const sl_rgb_led_t *handle);
void blue_led(void);
void red_led(void);
void green_led(void);

#endif /* LED_H_ */
