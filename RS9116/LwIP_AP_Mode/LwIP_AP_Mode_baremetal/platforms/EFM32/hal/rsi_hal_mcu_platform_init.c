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
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#include "sl_system_process_action.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "em_ldma.h"
#include "em_chip.h"
#include "spidrv.h"
#include "spidrv_config.h"
//#include "efm32gg11b_usart.h"


#define DEBUG_BREAK           __asm__("BKPT #0");
void app_init(void);


SPIDRV_HandleData_t handleDataMaster;
SPIDRV_Handle_t handleMaster = &handleDataMaster;

void initLdma(void)
{
  // First, initialize the LDMA unit itself
  LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
  LDMA_Init(&ldmaInit);
}


void app_init(void)
{
  spidrv_app_init();
}

void spidrv_app_init(void)
{
  // stdout is redirected to VCOM in project configuration
  printf("Welcome to the SPIDRV example application, master mode\r\n");
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

    sl_system_init();

          // SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);
    GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);
    GPIO_ExtIntConfig(gpioPortB, 9, 9, 1, 0, 1);

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
}




