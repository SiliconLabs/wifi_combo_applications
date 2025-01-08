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


typedef void (* UserIntCallBack_t)(void);

UserIntCallBack_t call_back;
#if 1
void GPIO_ODD_IRQHandler(void)
{
  if(call_back!=NULL)
    (*call_back)();

  GPIO_IntClear(0xAAAA); //0xffff
}
#endif
/*===================================================*/
/**
 * @fn           void rsi_hal_intr_config(void (* rsi_interrupt_handler)())
 * @brief        Starts and enables the SPI interrupt
 * @param[in]    rsi_interrupt_handler() ,call back function to handle interrupt
 * @param[out]   none
 * @return       none
 * @description  This HAL API should contain the code to initialize the register/pins
 *               related to interrupts and enable the interrupts.
 */
void rsi_hal_intr_config(void (* rsi_interrupt_handler)(void))
{
  call_back = rsi_interrupt_handler;
  return;
}


/*===================================================*/
/**
 * @fn           void rsi_hal_intr_mask(void)
 * @brief        Disables the SPI Interrupt
 * @param[in]    none
 * @param[out]   none
 * @return       none
 * @description  This HAL API should contain the code to mask/disable interrupts.
 */
void rsi_hal_intr_mask(void)
{
  //! Mask/Disable the interrupt
  NVIC_DisableIRQ(GPIO_ODD_IRQn);
  return;
}


/*===================================================*/
/**
 * @fn           void rsi_hal_intr_unmask(void)
 * @brief        Enables the SPI interrupt
 * @param[in]    none
 * @param[out]   none
 * @return       none
 * @description  This HAL API should contain the code to enable interrupts.
 */
void rsi_hal_intr_unmask(void)
{
  //! Unmask/Enable the interrupt
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  return;
}



/*===================================================*/
/**
 * @fn           void rsi_hal_intr_clear(void)
 * @brief        Clears the pending interrupt
 * @param[in]    none
 * @param[out]   none
 * @return       none
 * @description  This HAL API should contain the code to clear the handled interrupts.
 */
void rsi_hal_intr_clear(void)
{
  //! Clear the interrupt

  return;

}


/*===================================================*/
/**
 * @fn          void rsi_hal_intr_pin_status(void)
 * @brief       Checks the SPI interrupt at pin level
 * @param[in]   none
 * @param[out]  uint8_t, interrupt status
 * @return      none
 * @description This API is used to check interrupt pin status(pin level whether it is high/low).
 */
uint8_t rsi_hal_intr_pin_status(void)
{

  volatile uint8_t status = 0;

  //! Return interrupt pin  status(high(1) /low (0))
  status = rsi_hal_get_gpio(RSI_HAL_MODULE_INTERRUPT_PIN);
  return status;
}

