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
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_ldma.h"
#include "hal.h"
#include "bsp.h"
#include "retargetserial.h"

#define DEBUG_BREAK           __asm__("BKPT #0");

#define ITM_Port32(n) (*((volatile unsigned int *)(0xE0000000+4*n)))

#ifndef WEAK
#define WEAK  __attribute__((weak))
#endif

#ifdef RSI_CALIB_MODE_EN
void rsi_calib_uart_recv_isr(uint8_t cmd_char);
#endif

//! systick interrupt priority
#define SYSTICK_INTR_PRI  ((1<<__NVIC_PRIO_BITS)-1)

//! packet pending interrupt priority
#define PACKET_PENDING_INT_PRI  3

uint32_t sys_clck =0;

void SPI_setup (void)
{

  CMU_ClockEnable(cmuClock_USART0, true);
  sys_clck = CMU_ClockFreqGet(cmuClock_USART0);
  GPIO_PinModeSet(gpioPortE, PE12_CLK, gpioModePushPull, LOW);  // US0_CLK is push pull
  GPIO_PinModeSet(gpioPortE, PE13_CS, gpioModePushPull, HIGH);  // US0_CS is push pull
  GPIO_PinModeSet(gpioPortE, PE10_MOSI, gpioModePushPull, HIGH);  // US0_TX (MOSI) is push pull
  GPIO_PinModeSet(gpioPortE, PE11_MISO, gpioModeInput, HIGH);   // US0_RX (MISO) is input

  // Start with default config, then modify as necessary
  USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
  config.master = true;                 // master mode
  config.baudrate =  10000000;                // CLK freq is 5 MHz , this needs to be updated as per RS9116.
  config.autoCsEnable = true;               // CS pin controlled by hardware, not firmware
  config.clockMode = usartClockMode0;           // clock idle low, sample on rising/first edge
  config.msbf = true;                   // send MSB first
  config.enable = usartDisable;             // Make sure to keep USART disabled until it's all set up
  USART_InitSync(USART0, &config);

  // Set USART pin locations
  USART0->ROUTELOC0 = (USART_ROUTELOC0_CLKLOC_LOC0) |   // US0_CLK on location 0 = PE12 per datasheet section 6.4 = EXP Header pin 8
      (USART_ROUTELOC0_CSLOC_LOC0) |          // US0_CS on location 0 = PE13 per datasheet section 6.4 = EXP Header pin 10
      (USART_ROUTELOC0_TXLOC_LOC0) |          // US0_TX (MOSI) on location 0 = PE10 per datasheet section 6.4 = EXP Header pin 4
      (USART_ROUTELOC0_RXLOC_LOC0);         // US0_RX (MISO) on location 0 = PE11 per datasheet section 6.4 = EXP Header pin 6

  // Enable USART pins
  USART0->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_CSPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;

  // Enable USART0
  USART_Enable(USART0, usartEnable);
}




/*==============================================*/
/**
 * @fn           void rsi_hal_board_init()
 * @brief        This function Initializes the platform
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function initializes the platform
 *
 */

void rsi_hal_board_init(void)
{
  //! Initializes the platform
  CHIP_Init();

  CMU_OscillatorEnable(cmuOsc_HFXO,true,true);
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_ClockEnable(cmuClock_CORE, true);
#ifdef RSI_WITH_OS
  // Set Systick Intr priority
  NVIC_SetPriority(SysTick_IRQn, SYSTICK_INTR_PRI);
#endif


  //! GPIO initialization, Configures PB9 pin (in expansion header) as an interrupt.
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(gpioPortB, PB9_INT, gpioModeInput, LOW);
  GPIO_ExtIntConfig(gpioPortB, PB9_INT, INTR_NO, RISING_EDGE, FALLING_EDGE, HIGH);

  //! Setting up USART0 as SPI
  SPI_setup();

#ifdef RSI_DEBUG_PRINTS
  RETARGET_SerialInit();
  RETARGET_SerialCrLf(1);
  RETARGET_ReadChar();
#endif

  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000))
    {
      DEBUG_BREAK;
    }



}


/*==============================================*/
/**
 * @fn           void rsi_switch_to_high_clk_freq()
 * @brief        This function intializes SPI to high clock
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function intializes SPI to high clock
 *
 *
 */

void rsi_switch_to_high_clk_freq(void)
{
  //! Initializes the high clock

  CMU_OscillatorEnable(cmuOsc_HFXO,true,true);
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_ClockEnable(cmuClock_CORE, true);


  sys_clck = CMU_ClockFreqGet(cmuClock_CORE);
  sys_clck = CMU_ClockFreqGet(cmuClock_USART0);

}

WEAK void uart_rx_handler(uint8_t character)
{
#ifdef RSI_CALIB_MODE_EN
  rsi_calib_uart_recv_isr(character);
#endif
}

// Com Port RX interrupt
void USART4_RX_IRQHandler(void)
{
    uart_rx_handler(COM_PORT_PERIPHERAL->RXDATA);
}


