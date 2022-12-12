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
#include <stdio.h>
//#include "rsi_driver.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_chip.h"
#include "efr32mg21_usart.h"

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"

#include "em_ldma.h"
#include "system_efr32mg21.h"

#include "em_device.h"
//#include "em_chip.h"
#include "em_emu.h"
#include "em_gpio.h"
//#include "em_usart.h"
//#include "em_cmu.h"
//#include "sl_device_init_hfxo_config.h"
//#include "sl_device_init_emu_config.h"
//#include "uart_init.c"

#define US0CLK_PORT  gpioPortC
#define US0MOSI_PORT gpioPortC
#define US0MISO_PORT gpioPortC
#define US0MISO_PIN  1
#define US0MOSI_PIN  0
#define US0CLK_PIN   2
#define US0CS_PORT   gpioPortC
#define US0CS_PIN    3

// LDMA channel for receive and transmit servicing
#define RX_LDMA_CHANNEL 0
#define TX_LDMA_CHANNEL 1

// Size of the data buffers
#define BUFLEN 4

// Outgoing data
uint8_t outbuf[BUFLEN];

// Incoming data
uint8_t inbuf[BUFLEN];

// Data reception complete
bool rx_done;
//extern volatile uint8_t rx_done;

#define DEBUG_BREAK __asm__("BKPT #0");

#define ITM_Port32(n) (*((volatile unsigned int *)(0xE0000000 + 4 * n)))
/* Need to implement the  two Retarget IO functions with the read/write functions we want to use. */
int RETARGET_WriteChar(char c)
{
  return ITM_SendChar(c);
}
int RETARGET_ReadChar(void)
{
  return 0;
}

void uart_init(void);
void ABRD(void);
void init_gpio_reset(void);
void reset_RS9116_module(void);
/**************************************************************************/ /**
 * @brief
 *    GPIO initialization
 *****************************************************************************/
void initGpio(void)
{
  // Enable clock (not needed on xG21)
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Configure RX pin as an input
  GPIO_PinModeSet(US0MISO_PORT, US0MISO_PIN, gpioModeInput, 0);

  // Configure TX pin as an output
  GPIO_PinModeSet(US0MOSI_PORT, US0MOSI_PIN, gpioModePushPull, 0);

  // Configure CLK pin as an output low (CPOL = 0)
  GPIO_PinModeSet(US0CLK_PORT, US0CLK_PIN, gpioModePushPull, 0);

  // Configure CS pin as an output and drive inactive high
  GPIO_PinModeSet(US0CS_PORT, US0CS_PIN, gpioModePushPull, 1);
}
/**************************************************************************/ /**
 * @brief
 *    USART0 initialization
 *****************************************************************************/
void initUsart0(void)
{
  // Enable clock (not needed on xG21)
  CMU_ClockEnable(cmuClock_USART0, true);

  // Default asynchronous initializer (master mode, 1 Mbps, 8-bit data)
  USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;

  init.msbf         = true;    // MSB first transmission for SPI compatibility
  init.autoCsEnable = true;    // Allow the USART to assert CS
  init.autoCsSetup  = 4;       // Insert 7 bit times of CS setup delay
  init.autoCsHold   = 4;       // Insert 7 bit times of CS hold delay
  init.baudrate     = 8330000; //5000000;
  /*
   * Route USART0 RX, TX, and CLK to the specified pins.  Note that CS is
   * not controlled by USART0 so there is no write to the corresponding
   * USARTROUTE register to do this.
   */
  GPIO->USARTROUTE[0].RXROUTE = (US0MISO_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
                                | (US0MISO_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].TXROUTE = (US0MOSI_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
                                | (US0MOSI_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].CLKROUTE = (US0CLK_PORT << _GPIO_USART_CLKROUTE_PORT_SHIFT)
                                 | (US0CLK_PIN << _GPIO_USART_CLKROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[0].CSROUTE = (US0CS_PORT << _GPIO_USART_CSROUTE_PORT_SHIFT)
                                | (US0CS_PIN << _GPIO_USART_CSROUTE_PIN_SHIFT);

  // Enable USART interface pins
  GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | // MISO
                                GPIO_USART_ROUTEEN_TXPEN | // MOSI
                                GPIO_USART_ROUTEEN_CLKPEN | GPIO_USART_ROUTEEN_CSPEN;

  // Configure and enable USART0
  USART_InitSync(USART0, &init);
}

/**************************************************************************/ /**
 * @brief
 *    LDMA initialization
 *****************************************************************************/
void initLdma(void)
{
  // First, initialize the LDMA unit itself
  LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
  LDMA_Init(&ldmaInit);
}

/**************************************************************************/ /**
 * @brief LDMA IRQHandler
 *****************************************************************************/
/*void LDMA_IRQHandler()
{
  uint32_t flags = LDMA_IntGet();

  // Clear the transmit channel's done flag if set
  if (flags & (1 << TX_LDMA_CHANNEL))
    LDMA_IntClear(1 << TX_LDMA_CHANNEL);

  if (flags & (1 << RX_LDMA_CHANNEL))
  {
    LDMA_IntClear(1 << RX_LDMA_CHANNEL);
    rx_done = true;
  }

  // Stop in case there was an error
  if (flags & LDMA_IF_ERROR)
   {
      __BKPT(0) ;
   }
}*/

/***************************************************************************/ /**
 * Configure SWO - serial wire output
 ******************************************************************************/
static void swo_setup(void)
{
  uint32_t tpiu_prescaler_val;

#if defined(_CMU_HFBUSCLKEN0_GPIO_MASK)
  CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_GPIO;
#endif
#if defined(_CMU_HFPERCLKEN0_GPIO_MASK)
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
#endif

  // Enable Serial wire output pin
#if defined(_GPIO_ROUTE_MASK)
  GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;
#endif
#if defined(_GPIO_ROUTEPEN_MASK)
  GPIO->ROUTEPEN |= GPIO_ROUTEPEN_SWVPEN;
#endif

#if defined(_EFM32_GECKO_FAMILY) || defined(_EFM32_TINY_FAMILY)
  // Set location 1
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC1;
  // Enable output on pin
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
  // Set TPIU prescaler to 16 (14 MHz / 16 = 875 kHz SWO speed)
  tpiu_prescaler_val = 15;
#elif defined(_SILICON_LABS_32B_SERIES_0) && (defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY))
  // Set location 0
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC0;
  // Enable output on pin
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
  // Set TPIU prescaler to 16 (14 MHz / 16 = 875 kHz SWO speed)
  tpiu_prescaler_val = 16 - 1;
#elif defined(_SILICON_LABS_32B_SERIES_1)
  // Set location 0
  GPIO->ROUTELOC0 = (GPIO->ROUTELOC0 & ~(_GPIO_ROUTELOC0_SWVLOC_MASK)) | GPIO_ROUTELOC0_SWVLOC_LOC0;
  // Enable output on pin
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= 4 << 8;
#if defined(EMDK1001_FPGA)
  // Set TPIU prescaler to get a 875 kHz swo speed
  tpiu_prescaler_val = SystemCoreClockGet() / 875000 - 1;
#else
  // Set TPIU prescaler to 22 (19 MHz / 22 = 863.63 kHz SWO speed)
  tpiu_prescaler_val = 22 - 1;
#endif

#elif (_SILICON_LABS_32B_SERIES == 2)

  // Enable output on pin
#if defined(_CMU_CLKEN0_MASK)
  CMU->CLKEN0 |= CMU_CLKEN0_GPIO;
#endif
  GPIO->P[GPIO_SWV_PORT].MODEL &= ~(_GPIO_P_MODEL_MODE0_MASK << (GPIO_SWV_PIN * 4));
  GPIO->P[GPIO_SWV_PORT].MODEL |= _GPIO_P_MODEL_MODE0_PUSHPULL << (GPIO_SWV_PIN * 4);
  GPIO->TRACEROUTEPEN |= GPIO_TRACEROUTEPEN_SWVPEN;
#if defined(HFRCOEM23_PRESENT)
  // Select HFRCOEM23 as source for TRACECLK
  CMU_ClockSelectSet(cmuClock_TRACECLK, cmuSelect_HFRCOEM23);
#endif
  // Set TPIU prescaler to get a 863.63 kHz SWO speed
  tpiu_prescaler_val = CMU_ClockFreqGet(cmuClock_TRACECLK) / 863630 - 1;

#else
#error Unknown device family!
#endif

#if (_SILICON_LABS_32B_SERIES != 2)
  // Enable debug clock AUXHFRCO
  CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

  while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY))
    ;
#endif

  // Enable trace in core debug
  CoreDebug->DHCSR |= 1;
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  // Enable Cycle Counter
  DWT->CTRL = 0x3FF;

  // Set TPIU Prescaler
  TPI->ACPR = tpiu_prescaler_val;

  // Set protocol to NRZ
  TPI->SPPR = 2;

  // Disable continuous formatting
  TPI->FFCR = 0x100;

  // Unlock ITM and output data
  ITM->LAR = 0xC5ACCE55;
  ITM->TCR = 0x10009;

  // ITM Channel 0 is used for print-style output
  ITM->TER |= (1UL << 0);
}

// Fetch CTUNE value from USERDATA page as a manufacturing token
#define MFG_CTUNE_ADDR 0x0FE00100UL
#define MFG_CTUNE_VAL  (*((uint16_t *)(MFG_CTUNE_ADDR)))

void efr32_init(void)
{

#if 1
  //sl_board_preinit
  CMU_ClockEnable(cmuClock_GPIO, true);
#endif

  //nvic
#if 0
    for (IRQn_Type i = SVCall_IRQn; i < EXT_IRQ_COUNT; i++)
    {
        NVIC_SetPriority(i, CORE_INTERRUPT_DEFAULT_PRIORITY);
    }
#endif

#if 1
  //sl_device_init_hfrco
  CMU_HFRCODPLLBandSet(cmuHFRCODPLLFreq_80M0Hz);
#endif

#if 1
  //sl_device_init_hfxo
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
  hfxoInit.mode                 = cmuHfxoOscMode_Crystal;

  int ctune = -1;

#if defined(_DEVINFO_MODXOCAL_HFXOCTUNE_MASK)
  // Use HFXO tuning value from DEVINFO if available (PCB modules)
  if ((DEVINFO->MODULEINFO & _DEVINFO_MODULEINFO_HFXOCALVAL_MASK) == 0) {
    ctune = DEVINFO->MODXOCAL & _DEVINFO_MODXOCAL_HFXOCTUNEXIANA_MASK;
  }
#endif

  // Use HFXO tuning value from MFG token in UD page if not already set
  if ((ctune == -1) && (MFG_CTUNE_VAL != 0xFFFF)) {
    ctune = MFG_CTUNE_VAL;
  }

  // Use HFXO tuning value from configuration header as fallback
  if (ctune == -1) {
    ctune = 133; //SL_DEVICE_INIT_HFXO_CTUNE;
  }

  if (ctune != -1) {
    hfxoInit.ctuneXoAna = ctune;
    hfxoInit.ctuneXiAna = ctune;
  }

  SystemHFXOClockSet(38400000);
  CMU_HFXOInit(&hfxoInit);
#endif

#if 1
  //sl_device_init_lfxo
  CMU_LFXOInit_TypeDef lfxoInit = CMU_LFXOINIT_DEFAULT;

  lfxoInit.mode    = cmuLfxoOscMode_Crystal;
  lfxoInit.capTune = 37;

  CMU_LFXOInit(&lfxoInit);
  CMU_LFXOPrecisionSet(100);
#endif

#if 1
  //sl_device_init_clocks
  CMU_ClockSelectSet(cmuClock_SYSCLK, cmuSelect_HFRCODPLL);
#if defined(CMU_EM01GRPACLKCTRL_MASK)
  CMU_ClockSelectSet(cmuClock_EM01GRPACLK, cmuSelect_HFRCODPLL);
#endif
#if defined(CMU_EM01GRPBCLKCTRL_MASK)
  CMU_ClockSelectSet(cmuClock_EM01GRPBCLK, cmuSelect_HFRCODPLL);
#endif
  CMU_ClockSelectSet(cmuClock_EM23GRPACLK, cmuSelect_LFXO);
  CMU_ClockSelectSet(cmuClock_EM4GRPACLK, cmuSelect_LFXO);
#if defined(RTCC_PRESENT)
  CMU_ClockSelectSet(cmuClock_RTCC, cmuSelect_LFXO);
#endif
  CMU_ClockSelectSet(cmuClock_WDOG0, cmuSelect_LFXO);
#if WDOG_COUNT > 1
  CMU_ClockSelectSet(cmuClock_WDOG1, cmuSelect_LFXO);
#endif

#endif

#if 0
  // EM2 set debug enable
  EMU->CTRL = (EMU->CTRL & ~_EMU_CTRL_EM2DBGEN_MASK)
              | (SL_DEVICE_INIT_EMU_EM2_DEBUG_ENABLE << _EMU_CTRL_EM2DBGEN_SHIFT);

  // EM4 Init
  EMU_EM4Init_TypeDef em4_init = EMU_EM4INIT_DEFAULT;

  em4_init.pinRetentionMode = SL_DEVICE_INIT_EMU_EM4_PIN_RETENTION_MODE;
  EMU_EM4Init(&em4_init);
#endif

#if 0
  osKernelInitialize();
#endif
}

void rsi_hal_board_init(void)
{
  //! Initializes the platform

  // Chip errata
#ifndef RSI_WITH_OS
  CHIP_Init();
#else
  CHIP_Init();
  //efr32_init();
#endif
  swo_setup();
  // Initialize GPIO and USART0
#if RSI_SPI_INTERFACE
  initGpio();
  initUsart0();
  initLdma();
#endif
  // timer
  SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);

  GPIO_PinModeSet(gpioPortD, 03, gpioModeInput, 0); //Made PB01 as Interrupt pin
  GPIO_ExtIntConfig(gpioPortD, 03, 03, 1, 0, 1);

  //  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  //    NVIC_EnableIRQ(GPIO_ODD_IRQn);

  /* Enable ODD interrupt to catch button press that changes slew rate */

#if RSI_UART_INTERFACE
  uart_init();
  init_gpio_reset();
  reset_RS9116_module();

  ABRD();
#endif
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
}
