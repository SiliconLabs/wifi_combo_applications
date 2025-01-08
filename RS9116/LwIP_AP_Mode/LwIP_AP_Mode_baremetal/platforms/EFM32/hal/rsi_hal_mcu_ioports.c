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
#include "rsi_driver.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "hal.h"

/**
 * Global Variales
 */



/*===========================================================*/
/**
 * @fn            void rsi_hal_config_gpio(uint8_t gpio_number,uint8_t mode,uint8_t value)
 * @brief         Configures gpio pin in output mode,with a value
 * @param[in]     uint8_t gpio_number, gpio pin number to be configured
 * @param[in]     uint8_t mode , input/output mode of the gpio pin to configure
 *                0 - input mode
 *                1 - output mode
 * @param[in]     uint8_t value, default value to be driven if gpio is configured in output mode
 *                0 - low
 *                1 - high
 * @param[out]    none
 * @return        none
 * @description This API is used to configure host gpio pin in output mode.
 */
void rsi_hal_config_gpio(uint8_t gpio_number,uint8_t mode,uint8_t value)
{

  CMU_ClockEnable(cmuClock_GPIO, true);
  //! Initialise the gpio pins in input/output mode

  GPIO_PinModeSet(gpioPortA,PA12_PSP,gpioModePushPull,LOW);
  //value = GPIO_PinModeGet(gpioPortA,12);

  GPIO_PinModeSet(gpioPortA,PA13_PSP,gpioModeInput,LOW);
  //value = GPIO_PinModeGet(gpioPortA,13);

  GPIO_PinModeSet(gpioPortB,PB11_RST,gpioModePushPull,HIGH);//Reset PB11
  //value = GPIO_PinModeGet(gpioPortB,11);

  return;

}



/*===========================================================*/
/**
 * @fn            void rsi_hal_set_gpio(uint8_t gpio_number)
 * @brief         Makes/drives the gpio  value high
 * @param[in]     uint8_t gpio_number, gpio pin number
 * @param[out]    none
 * @return        none
 * @description   This API is used to drives or makes the host gpio value high.
 */
void rsi_hal_set_gpio(uint8_t gpio_number)
{

  //! drives a high value on GPIO
  if(gpio_number ==  RSI_HAL_SLEEP_CONFIRM_PIN)
    {
      //GPIO_PinModeSet(gpioPortA,PA13_PSP,gpioModeWiredOrPullDown,HIGH);
      GPIO_PinModeSet(gpioPortA,PA12_PSP,gpioModePushPull,HIGH);  // mapped to UULP_2

    }

  if(gpio_number ==  RSI_HAL_WAKEUP_INDICATION_PIN)
    {
      //GPIO_PinModeSet(gpioPortA,PA12_PSP,gpioModeWiredOrPullDown,HIGH);
      GPIO_PinModeSet(gpioPortA,PA13_PSP,gpioModeInput,HIGH); // mapped to UULP_3
    }

  if(gpio_number ==  RSI_HAL_LP_SLEEP_CONFIRM_PIN)
    {
      //Not used now.
    }

  if(gpio_number ==  RSI_HAL_RESET_PIN)
    {
      GPIO_PinModeSet(gpioPortB,PB11_RST,gpioModeWiredOrPullDown,HIGH);
    }

  return;
}




/*===========================================================*/
/**
 * @fn          uint8_t rsi_hal_get_gpio(void)
 * @brief       get the gpio pin value
 * @param[in]   uint8_t gpio_number, gpio pin number
 * @param[out]  none
 * @return      gpio pin value
 * @description This API is used to configure get the gpio pin value.
 */
uint8_t rsi_hal_get_gpio(uint8_t gpio_number)
{
  uint8_t gpio_value = 0;

  //! Get the gpio value
  //gpio_value = GPIO_PinInGet(gpioPortB, 9);

  if(gpio_number == RSI_HAL_SLEEP_CONFIRM_PIN)
    {
      gpio_value = GPIO_PinInGet(gpioPortA,PA12_PSP);
    }

  if(gpio_number == RSI_HAL_WAKEUP_INDICATION_PIN)
    {
      gpio_value =  GPIO_PinInGet(gpioPortA,PA13_PSP);
    }

  if(gpio_number ==  RSI_HAL_LP_SLEEP_CONFIRM_PIN)
    {
      //Not used now.
    }

  //! Status of External Interrupt GPIO Pin
  if(gpio_number ==  RSI_HAL_MODULE_INTERRUPT_PIN)
    {
      gpio_value =  GPIO_PinInGet(gpioPortB, PB9_INT);
    }

  return gpio_value;

}




/*===========================================================*/
/**
 * @fn            void rsi_hal_set_gpio(uint8_t gpio_number)
 * @brief         Makes/drives the gpio value to low
 * @param[in]     uint8_t gpio_number, gpio pin number
 * @param[out]    none
 * @return        none
 * @description   This API is used to drives or makes the host gpio value low.
 */
void rsi_hal_clear_gpio(uint8_t gpio_number)
{

  //! drives a low value on GPIO
  if(gpio_number ==  RSI_HAL_SLEEP_CONFIRM_PIN)
    {
      GPIO_PinOutClear(gpioPortA,PA12_PSP);
    }

  if(gpio_number ==  RSI_HAL_WAKEUP_INDICATION_PIN)
    {
      GPIO_PinOutClear(gpioPortA,PA13_PSP);
    }

  if(gpio_number ==  RSI_HAL_LP_SLEEP_CONFIRM_PIN)
    {
      GPIO_PinOutClear(gpioPortA,PA12_PSP);
    }

  if(gpio_number ==  RSI_HAL_RESET_PIN)
    {
      GPIO_PinOutClear(gpioPortB,PB11_RST);
    }


  return;
}


