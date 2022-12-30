/***************************************************************************//**
 * @file
 * @brief Provide MicroSD SPI configuration parameters.
 * @version  1.00

 *******************************************************************************
 * @section License
 * <b>Copyright 2017 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef __MICROSDCONFIG_H
#define __MICROSDCONFIG_H

#include "bspconfig.h"

#define MICROSD_LO_SPI_FREQ     100000

#if defined(BSP_STK_BRD2204A)
#define MICROSD_USART           USART0
#define MICROSD_TXLOC           USART_ROUTELOC0_TXLOC_LOC0
#define MICROSD_RXLOC           USART_ROUTELOC0_RXLOC_LOC0
#define MICROSD_CLKLOC          USART_ROUTELOC0_CLKLOC_LOC0
#define MICROSD_CMUCLOCK        cmuClock_USART0
#define MICROSD_GPIOPORT        gpioPortE
#define MICROSD_MOSIPIN         10
#define MICROSD_MISOPIN         11
#define MICROSD_CSPIN           13
#define MICROSD_CLKPIN          12
#else
#define MICROSD_USART           USART2
#define MICROSD_TXLOC           USART_ROUTELOC0_TXLOC_LOC1
#define MICROSD_RXLOC           USART_ROUTELOC0_RXLOC_LOC1
#define MICROSD_CLKLOC          USART_ROUTELOC0_CLKLOC_LOC1
#define MICROSD_CMUCLOCK        cmuClock_USART2
#define MICROSD_GPIOPORT        gpioPortA
#define MICROSD_MOSIPIN         6
#define MICROSD_MISOPIN         7
#define MICROSD_CSPIN           9
#define MICROSD_CLKPIN          8
#endif

#endif /* __MICROSDCONFIG_H */

void MICROSD_Init(void);
