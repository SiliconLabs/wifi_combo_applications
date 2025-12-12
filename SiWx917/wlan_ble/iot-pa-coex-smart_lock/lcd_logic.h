/*
 * lcd_logic.h
 *
 *  Created on: Aug 6, 2025
 *      Author: swpaladu
 */

#ifndef LCD_LOGIC_H_
#define LCD_LOGIC_H_

#include <stdbool.h>
#include <stdint.h>
#include "glib.h"
#include "logo_bitmaps.h"
#include "rsi_retention.h"
#include "ble_app.h"
#include "sl_status.h"

// Function declarations
void memlcd_app_init(void);
sl_status_t demoUIDisplayApp(SystemStatus_t SystemStatus);
bool is_lcd_initialized(void);
sl_status_t get_display_dimensions(uint32_t* width, uint32_t* height);
sl_status_t updateDisplay(void);

// Note: Helper functions are static and defined in lcd_logic.c

// LCD related defines
#define SL_BOARD_ENABLE_DISPLAY_PIN  0
#define SL_BOARD_ENABLE_DISPLAY_PORT 0

// Position defines for drawing elements
#define BLE_START_X  23
#define BLE_BASE_Y   104
#define BLE_END_X    35
#define WIFI_START_X 90
#define WIFI_BASE_Y  104
#define WIFI_END_X   102

// Bitmap size defines
#define APP_BITMAP_WIDTH 64
#define APP_BITMAP_HEIGHT 64

#define SILICONLABS_BITMAP_WIDTH        128
#define SILICONLABS_BITMAP_HEIGHT       45
#define SILICONLABS_Y_POSITION          0
#define SILICONLABS_X_POSITION          ((glibContext.pDisplayGeometry->xSize - SILICONLABS_BITMAP_WIDTH) / 2)

// Note: Position calculations moved to functions to avoid compile-time dependencies
// Use get_display_dimensions() and position calculation functions instead

#endif /* LCD_LOGIC_H_ */
