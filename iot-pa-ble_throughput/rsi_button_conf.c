#include "rsi_board_configuration.h"
#include "stdio.h"
#include "stdbool.h"

#ifdef RSI_STM32_PLATFORM
#include "rsi_driver.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_tim_ex.h"
#include "rsi_driver.h"
#include "rsi_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "rsi_ble_config.h"
#endif
#ifdef RSI_EFR32_PLATFORM
#include "rsi_driver.h"
#include "em_gpio.h"
#endif

extern uint8_t device_connected;
#ifdef RSI_STM32_PLATFORM
typedef void (* UserIntCallBack_t)(void);
#endif


#ifdef RSI_EFR32_PLATFORM
typedef void (* UserIntCallBack_t)(void);
extern UserIntCallBack_t call_back;
#endif

#ifdef RSI_EFR32_PLATFORM
#define PACKET_PENDING_PB0  4
#endif
void rsi_button_config(void)
{
#ifdef RSI_STM32_PLATFORM
	GPIO_InitTypeDef GPIO_InitStruct={0};
	/*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,9, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif
#ifdef RSI_EFR32_PLATFORM
  //ports:PB00
   //! configure packet pending interrupt priority
  NVIC_SetPriority(GPIO_EVEN_IRQn,PACKET_PENDING_PB0);
  // Configure interrupt pin
  GPIO_PinModeSet(SLEEP_CONFIRM_PIN.port, SLEEP_CONFIRM_PIN.pin, gpioModeInput, 1);
  GPIO_ExtIntConfig(SLEEP_CONFIRM_PIN.port, SLEEP_CONFIRM_PIN.pin, SLEEP_CONFIRM_PIN.pin, false, true, true);
  /* Enable EVEN interrupt to catch button press that changes slew rate */
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  /* Configure pB00 as a push pull output for LED drive */
  //GPIO_PinModeSet(RESET_PIN.port, RESET_PIN.pin, gpioModePushPull, 0);

#endif
}
#ifdef RSI_STM32_PLATFORM
void EXTI15_10_IRQHandler(void)
{
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
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) 
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
	static uint8_t prev_val;
	if(GPIO_Pin == GPIO_PIN_13)
	{
		
	  if(device_connected == 0x01)
		{
			if(prev_val == 0x00)
			{
			   prev_val =1;
				transmission_on_notifications_handling(prev_val);
			}
     else
			{
				 prev_val =0;
				transmission_on_notifications_handling(prev_val);
			}			
		}

	}
	
}
#endif
#ifdef RSI_EFR32_PLATFORM
void GPIO_EVEN_IRQHandler(void)
{
  /* Get and clear all pending GPIO interrupts */
   if(call_back!=NULL)
    (*call_back)();
  GPIO_IntClear(0xFFFF);
  static uint8_t prev_val;
  if(device_connected == 0x01)
  {
    if(prev_val == 0x00)
    {
       prev_val =1;
       transmission_on_notifications_handling(prev_val);
    }
   else
    {
         prev_val =0;
         transmission_on_notifications_handling(prev_val);
    }
  }
}
#endif

