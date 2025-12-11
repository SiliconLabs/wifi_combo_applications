/***************************************************************************/ /**
 * @file app.h
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "sl_si91x_clock_manager.h"
#include "sl_si91x_power_manager.h"
#include <rsi_common_apis.h>
#include "lcd_logic.h"
#include "led.h"

#define SILABS_SMARTLOCK_HARDWARE   0   // Enable this macro only if you have a reference PCB with Motor
#define DISPLAY                     1   // Enable this macro only if BRD4002A Mainboard and BRD4338A radio board is used

#define LEFT_HANDED_DOOR          0       //Disable - if the door is a right handed door, Enable - For left handed door

#define AWS                       1
#define ADAFRUIT                  0

#define IN_UNCONNECTED_STATE      0       //indicates no connection with AP
#define IN_ASSOCIATED_STATE       1       //indicates connection with AP
#define PERIODIC_TIMEOUT_MS      15000    //timeout for 15sec
#define PERIODIC_INTERVALS        4       //intervals, i.e one timeout interval is 15sec, so periodic timer stops after 4*15=60sec
#define WAKEUP_TIME_MS          10000     //timeout for 60sec
#define LOCK_ROTATION_MS        1800     //motor to rotate for 2800msec
#define AUTO_LOCK_TIMEOUT_MS    15000    //motor to unlock after 15sec
#define KEEP_ALIVE_TIME_SEC     (WAKEUP_TIME_MS/1000)+60

#define AUTO_LOCK_RECEIVED              0x1
#define LOCK_CMD_RECEIVED_STATE         0x2
#define UNLOCK_CMD_RECEIVED_STATE       0x3
#define FLASH_ERASE_RECEIVED_STATE      0x4
#define BUTTON_INTERRUPT_RCVD           0x5

typedef struct
{
char source[10];
char command[10];
}Source_operation_t;

#define AWS_SOURCE  1
#define BLE_SOURCE  2

#define LOCK        1
#define UNLOCK      2

/***************************************************************************/ /**
 * Initialize application.
 ******************************************************************************/
void app_init(void);
void default_clock_configuration(void);

/***************************************************************************/ /**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void);
void rsi_app_set_event(uint32_t event_num);
void rsi_app_clear_event(uint32_t event_num);
int32_t rsi_app_get_event(void);
#endif // APP_H
