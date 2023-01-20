/*******************************************************************************
 * @file rsi_hal_mcu_random.c
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com
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
#include "rsi_board_configuration.h"
#include "rsi_driver.h"
#include "stdbool.h"
#include "stdio.h"

#ifdef RSI_STM32_PLATFORM
#include "rsi_ble_config.h"
#include "rsi_driver.h"
#include "rsi_hal.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_tim_ex.h"
#include "stm32f4xx_hal_uart.h"
#endif

#ifdef RSI_EFR32_PLATFORM
#include "em_gpio.h"
#include "rsi_driver.h"
#endif

#ifdef RSI_STM32_PLATFORM
typedef void (*UserIntCallBack_t)(void);
#endif

#ifdef RSI_EFR32_PLATFORM
typedef void (*UserIntCallBack_t)(void);
extern UserIntCallBack_t call_back;
#endif

#ifdef RSI_EFR32_PLATFORM
#define PACKET_PENDING_PB0 4
#endif

extern uint8_t device_connected;
/*============================================================================*/
/**
 * @fn         rsi_button_config
 * @brief      This function is used to configure the host MCU push button.
 *              -For EFR32, the PB00 button is configured.
 *              -For STM32, the "B1" button
 * @param[in]  none.
 * @return     none
 */
void rsi_button_config(void) {
#ifdef RSI_STM32_PLATFORM
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 9, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif
#ifdef RSI_EFR32_PLATFORM
  // ports:PB00
  //! configure packet pending interrupt priority
  NVIC_SetPriority(GPIO_EVEN_IRQn, PACKET_PENDING_PB0);
  // Configure interrupt pin
  GPIO_PinModeSet(SLEEP_CONFIRM_PIN.port, SLEEP_CONFIRM_PIN.pin, gpioModeInput,
                  1);
  GPIO_ExtIntConfig(SLEEP_CONFIRM_PIN.port, SLEEP_CONFIRM_PIN.pin,
                    SLEEP_CONFIRM_PIN.pin, false, true, true);
  /* Enable EVEN interrupt to catch button press that changes slew rate */
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
#endif
}

#ifdef RSI_STM32_PLATFORM
/*============================================================================*/
/**
 * @fn         EXTI15_10_IRQHandler
 * @brief      This function is used to register the GPIO callback for STM32 MCU
 * @param[in]  none.
 * @return     none
 */
void EXTI15_10_IRQHandler(void) {
  UserIntCallBack_t call_back, gpio_callback;
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
  //! Mask/Disable the interrupt
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  //! Unmask/Enable the interrupt
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  /* USER CODE END EXTI15_10_IRQn 1 */
}
/*============================================================================*/
/**
 * @fn         EXTI15_10_IRQHandler
 * @brief      This function is used to process the button functionality for
 * STM32 MCU.
 * @param[in]  GPIO_Pin, Pin number.
 * @return     none
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  static uint8_t prev_val;
  if (GPIO_Pin == GPIO_PIN_13) {
    if (device_connected == 0x01) {
      if (prev_val == 0x00) {
        prev_val = 1;
      } else {
        prev_val = 0;
      }
      data_transfer(prev_val);
    }
  }
}
#endif
#ifdef RSI_EFR32_PLATFORM
/*============================================================================*/
/**
 * @fn         GPIO_EVEN_IRQHandler
 * @brief      This function is used to process the button functionality for
 * EFR32 MCU.
 * @param[in]  GPIO_Pin, Pin number.
 * @return     none
 */ //=>not functionality
void GPIO_EVEN_IRQHandler(void) {
  static uint8_t prev_val;
  /* Get and clear all pending GPIO interrupts */
  if (call_back != NULL)
    (*call_back)();
  GPIO_IntClear(0xFFFF);
  if (device_connected == 0x01) {
    if (prev_val == 0x00) {
      prev_val = 1;
    } else {
      prev_val = 0;
    }
    data_transfer(prev_val);
  }
}
#endif
