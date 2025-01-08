/**
 * @file       rsi_wisemcu_hardware_setup.h
 * @version    0.9.3
 * @date       22 Feb 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains hardware configuration functuions for WiSeMCU mode products
 *
 * @section Description
 * This files contains hardware configuration functuions for WiSeMCU mode products
 *
 *
 */
/*Includes*/

#include "rsi_chip.h"

/*Configure the PMU and XTAL good times */
#define PMU_GOOD_TIME        31    /*Duration in us*/
#define XTAL_GOOD_TIME       31    /*Duration in us*/

/*Macro to be used for configuring the 'rams_in_use' parameter in 'RSI_WISEMCU_ConfigRamRetention()' function*/
#define WISEMCU_0KB_RAM_IN_USE   ( RAM_BANK_0  | RAM_BANK_1  | RAM_BANK_2                \
                                 | RAM_BANK_3  | RAM_BANK_4  | RAM_BANK_5  | RAM_BANK_6  \
                                 | RAM_BANK_7  | RAM_BANK_8  | RAM_BANK_9                \
                                 | RAM_BANK_10 | RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13 )
#define WISEMCU_16KB_RAM_IN_USE  ( RAM_BANK_0  | RAM_BANK_1  | RAM_BANK_2                \
                                 | RAM_BANK_7  | RAM_BANK_8  | RAM_BANK_9                \
                                 | RAM_BANK_10 | RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13 )
#define WISEMCU_48KB_RAM_IN_USE  ( RAM_BANK_0  | RAM_BANK_1  | RAM_BANK_2                \
                                 | RAM_BANK_8  | RAM_BANK_9                              \
                                 | RAM_BANK_10 | RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13 )
#define WISEMCU_112KB_RAM_IN_USE (  RAM_BANK_0  | RAM_BANK_1  | RAM_BANK_2               \
                                 | RAM_BANK_9                                            \
                                 | RAM_BANK_10 | RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13 )
#define WISEMCU_128KB_RAM_IN_USE ( RAM_BANK_0  | RAM_BANK_1  | RAM_BANK_2                \
                                 | RAM_BANK_10 | RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13 )
#define WISEMCU_144KB_RAM_IN_USE ( RAM_BANK_1  | RAM_BANK_2                              \
                                 | RAM_BANK_10 | RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13 )                                 
#define WISEMCU_176KB_RAM_IN_USE ( RAM_BANK_2                                            \
                                 | RAM_BANK_10 | RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13 )                                 
#define WISEMCU_192KB_RAM_IN_USE ( RAM_BANK_10 | RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13 )
#define WISEMCU_208KB_RAM_IN_USE ( RAM_BANK_11 | RAM_BANK_12 | RAM_BANK_13               )
#define WISEMCU_240KB_RAM_IN_USE ( RAM_BANK_12 | RAM_BANK_13                             )
#define WISEMCU_320KB_RAM_IN_USE ( RAM_BANK_13 )
#define WISEMCU_384KB_RAM_IN_USE ( 0                                                     )

/*Macro to be used for configuring the 'rams_retention_during_sleep' parameter in 'RSI_WISEMCU_ConfigRamRetention()' function*/
#define WISEMCU_RETAIN_DEFAULT_RAM_DURING_SLEEP ( 1 )
#define WISEMCU_RETAIN_16K_RAM_DURING_SLEEP     ( M4ULP_RAM16K_RETENTION_MODE_EN )
#define WISEMCU_RETAIN_128K_RAM_DURING_SLEEP    ( M4ULP_RAM16K_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN )
#define WISEMCU_RETAIN_192K_RAM_DURING_SLEEP    ( M4ULP_RAM16K_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN | M4SS_RAM_RETENTION_MODE_EN )
#define WISEMCU_RETAIN_384K_RAM_DURING_SLEEP    ( M4ULP_RAM16K_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN | M4SS_RAM_RETENTION_MODE_EN | TA_RAM_RETENTION_MODE_EN )
#define WISEMCU_RETAIN_M4SS_RAM_DURING_SLEEP    ( M4SS_RAM_RETENTION_MODE_EN )
#define WISEMCU_RETAIN_ULPSS_RAM_DURING_SLEEP   ( ULPSS_RAM_RETENTION_MODE_EN )
#define WISEMCU_RETAIN_TASS_RAM_DURING_SLEEP    ( TA_RAM_RETENTION_MODE_EN )
#define WISEMCU_RETAIN_M4ULP_RAM_DURING_SLEEP   ( M4ULP_RAM_RETENTION_MODE_EN )

/*Function proto-types*/
void RSI_WISEMCU_HardwareSetup(void);
void RSI_WISEMCU_TriggerSleep(SLEEP_TYPE_T sleepType , uint8_t  lf_clk_mode    , 
                              uint32_t stack_address , uint32_t jump_cb_address, 
                              uint32_t vector_offset , uint32_t mode);
void RSI_WISEMCU_ConfigRamRetention(uint32_t rams_in_use, uint32_t rams_retention_during_sleep);  

/*End of file not truncated*/
