/*******************************************************************************
 * @file  motor.h
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
#ifndef MOTOR_H_
#define MOTOR_H_

#include "sl_si91x_peripheral_gpio.h"
#include "sl_driver_gpio.h"

#define PORT0 0 // GPIO Port 0

#define PAD_SELECT_9   9  // GPIO PAD selection number
#define MAX_PAD_SELECT 34 // Maximum GPIO PAD selection number

#define PIN6  6   // Pin number 6 port 0
#define PIN7  7   // Pin number 7 port 0 for button
#define PIN8  8   // Pin number 8 port 0 for pin interrupt
#define PIN10 10  // Pin number 10 port 0 for led
#define PIN25 28  // Pin number 10 port 0 for led
#define PIN28 25  // Pin number 10 port 0 for led

// SmartLock Motor Control GPIOs
#define GPIO_25   25  // Pin number 25 for interrupt
#define GPIO_28   12//28  // Pin number 28 for HALL Sense
#define GPIO_49   6//49  // GPIO 49 for Motor enable
#define GPIO_46   10//46  // GPIO 46 for Motor Phase
#define ULP_PIN_4  4  // ULP GPIO4 OPT Sense2
#define ULP_PIN_2  2  // ULP GPIO2 OPT Enable


#define GPIO_29   29 // GPIO 29 for OPT Sense1

#define ULP_PIN_1  1  // ULP GPIO Pin number 1

#define ULP_PIN_10 10 // ULP GPIO Pin number 10


#define UULP_PIN0 0 // UULP Pin number 0
#define UULP_PIN1 1 // UULP Pin number 1
#define UULP_PIN2 2 // UULP Pin number 2

#define POLARITY  0 // Polarity for GPIO pin
#define PIN_COUNT 2 // Number of interrupts needed

#define AND_EVENT   0 // AND for group interrupt
#define OR_EVENT    1 // OR for group interrupt
#define LEVEL_EVENT 0 // level for group interrupt
#define EDGE_EVENT  1 // Edge for group interrupt
#define GRP_COUNT   2 // Count of group interrupt pins

#define PRIORITY_7 7      // Priority 7 for interrupt
#define PINS       0x0048 // Pins in a port to mask

#define INT_CH       0 // GPIO Pin interrupt 0
#define ULP_INT_CH   0 // ULP GPIO Pin interrupt 0
#define OUTPUT_VALUE 1 // GPIO output value
#define NPSS_INTR    4 // NPSS GPIO interrupt number

#define MS_DELAY_COUNTER 4600 // Delay count
#define DELAY            1000 // Delay for 1sec
#define FALL_EDGE        8    // Fall edge event

#define UULP_MASK        0x00
#define UULP_GPIO_INTR_2 2    // UULP GPIO pin interrupt 2

void sl_gpio_initialization(void);
void sl_gpio_ulp_initialization(void);
void unlock(void);
void lock(void);
void stop_motor();
uint8_t check_lock_state(void);

#endif /* MOTOR_H_ */
