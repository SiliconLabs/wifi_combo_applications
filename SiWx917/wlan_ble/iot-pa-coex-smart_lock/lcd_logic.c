/*
 * lcd_logic.c
 *
 *  Created on: Aug 6, 2025
 *      Author: swpaladu
 */

#include "app.h"
#include "lcd_logic.h"
#include "sl_memlcd.h"
#include <glib.h>

// Forward declarations for static helper functions
static sl_status_t draw_silicon_labs_logo(void);
static sl_status_t draw_lock_status_icon(bool lock_status);
static sl_status_t draw_connection_icons(const SystemStatus_t* SystemStatus);
static sl_status_t draw_operation_indicators(const SystemStatus_t* SystemStatus);

//LCD related variables
GLIB_Context_t glibContext;
volatile int currentLine = 0;
static bool lcd_initialized = false;

// Helper functions for position calculations
static inline uint32_t get_light_x_position(void) {
    return lcd_initialized ? ((glibContext.pDisplayGeometry->xSize - 64) / 2) : 0;
}

static inline uint32_t get_light_y_position(void) {
    return lcd_initialized ? (glibContext.pDisplayGeometry->ySize - 64 - 5) : 0;
}

sl_status_t updateDisplay(void)
{
    sl_status_t status = SL_STATUS_OK;

    // In sleep, memlcd will not be retained so re-initialize MEMLCD interface after sleep wakeup
    sl_memlcd_post_wakeup_init();

    status = DMD_updateDisplay();
    if (status != DMD_OK)
        return SL_STATUS_FAIL;

    // MEMLCD is not a UULP component and not available in sleep so powering down before sleep and need to be re-initialized after
    // sleep-wakeup
    sl_memlcd_power_on(NULL, false);

    return SL_STATUS_OK;
}


void memlcd_app_init(void)
{
  sl_status_t status;

    // Enabling LCD display
    RSI_NPSSGPIO_InputBufferEn(SL_BOARD_ENABLE_DISPLAY_PIN, 1U);
    RSI_NPSSGPIO_SetPinMux(SL_BOARD_ENABLE_DISPLAY_PIN, 0);
    RSI_NPSSGPIO_SetDir(SL_BOARD_ENABLE_DISPLAY_PIN, 0);
    RSI_NPSSGPIO_SetPin(SL_BOARD_ENABLE_DISPLAY_PIN, 1U);

    /* Initialize the DMD support for memory lcd display */
    status = DMD_init(0);
    EFM_ASSERT(status == DMD_OK);

    /* Initialize the glib context */
    status = GLIB_contextInit(&glibContext);
    EFM_ASSERT(status == GLIB_OK);

    glibContext.backgroundColor = White;
    glibContext.foregroundColor = Black;

    /* Fill lcd with background color */
    GLIB_clear(&glibContext);

    /* Use Narrow font */
    GLIB_setFont(&glibContext, (GLIB_Font_t *)&GLIB_FontNarrow6x8);
  }

sl_status_t demoUIDisplayApp(SystemStatus_t SystemStatus)
{
  sl_status_t status = SL_STATUS_OK;

  // Clear display
  status = GLIB_clear(&glibContext);
  if (status != GLIB_OK) {
    LOG_PRINT("Failed to clear display\r\n");
    return SL_STATUS_FAIL;
  }

  // Draw Silicon Labs logo
  status = draw_silicon_labs_logo();
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Draw main title
  status = GLIB_drawStringOnLine(&glibContext, "Smartlock Demo", 5, GLIB_ALIGN_CENTER, 0, 0, true);
  if (status != GLIB_OK) {
    LOG_PRINT("Failed to draw title string\r\n");
    return SL_STATUS_FAIL;
  }

  // Draw lock status icon
  status = draw_lock_status_icon(SystemStatus.lock_status);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Draw connection status icons
  status = draw_connection_icons(&SystemStatus);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Draw operation indicators
  status = draw_operation_indicators(&SystemStatus);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Update the display
  updateDisplay();
  
  return SL_STATUS_OK;
}

// Helper function implementations
static sl_status_t draw_silicon_labs_logo(void)
{
  sl_status_t status = GLIB_drawBitmap(&glibContext,
                                      SILICONLABS_X_POSITION,
                                      SILICONLABS_Y_POSITION,
                                      SILICONLABS_BITMAP_WIDTH,
                                      SILICONLABS_BITMAP_HEIGHT,
                                      siliconlabsBitmap);
  if (status != GLIB_OK) {
    LOG_PRINT("Failed to draw Silicon Labs logo\r\n");
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

static sl_status_t draw_lock_status_icon(bool lock_status)
{
  const uint32_t x_pos = (glibContext.pDisplayGeometry->xSize - APP_BITMAP_WIDTH) / 2;
  const uint32_t y_pos = glibContext.pDisplayGeometry->ySize - APP_BITMAP_HEIGHT - 5;
  
  sl_status_t status = GLIB_drawBitmap(&glibContext, 
                                      x_pos, y_pos, 
                                      APP_BITMAP_WIDTH, APP_BITMAP_HEIGHT,
                                      (lock_status ? OnStateBitMap : OffStateBitMap));
  if (status != GLIB_OK) {
    LOG_PRINT("Failed to draw lock status icon\r\n");
    return SL_STATUS_FAIL;
  }
  return SL_STATUS_OK;
}

static sl_status_t draw_connection_icons(const SystemStatus_t* SystemStatus)
{
  sl_status_t status = SL_STATUS_OK;

  if (SystemStatus->ble_connect) {
    status = GLIB_drawBitmap(&glibContext, 
                            BLE_ICON_POSITION_X, BLE_ICON_POSITION_Y, 
                            BLUETOOTH_ICON_WIDTH, BLUETOOTH_ICON_HEIGHT,  // Fixed typo
                            bleLogo);
    if (status != GLIB_OK) {
      LOG_PRINT("Failed to draw BLE icon\r\n");
      return SL_STATUS_FAIL;
    }
  }

  if (SystemStatus->wifi_connect) {
    status = GLIB_drawBitmap(&glibContext, 
                            WIFI_ICON_POSITION_X, WIFI_ICON_POSITION_Y, 
                            WIFI_BITMAP_WIDTH, WIFI_BITMAP_HEIGHT,
                            wifiLogo);
    if (status != GLIB_OK) {
      LOG_PRINT("Failed to draw WiFi icon\r\n");
      return SL_STATUS_FAIL;
    }
  }

  return SL_STATUS_OK;
}

static sl_status_t draw_operation_indicators(const SystemStatus_t* SystemStatus)
{
  sl_status_t status = SL_STATUS_OK;

  if (SystemStatus->ble_ops) {
    // Draw BLE operation arrow
    status |= GLIB_drawLine(&glibContext, BLE_START_X, BLE_BASE_Y, BLE_END_X, BLE_BASE_Y);
    status |= GLIB_drawLine(&glibContext, BLE_START_X + 7, BLE_BASE_Y - 3, BLE_END_X, BLE_BASE_Y);
    status |= GLIB_drawLine(&glibContext, BLE_START_X + 7, BLE_BASE_Y + 3, BLE_END_X, BLE_BASE_Y);
    
    if (status != GLIB_OK) {
      LOG_PRINT("Failed to draw BLE operation indicator\r\n");
      return SL_STATUS_FAIL;
    }
  }

  if (SystemStatus->wifi_ops) {
    // Draw WiFi operation arrow
    status |= GLIB_drawLine(&glibContext, WIFI_START_X, WIFI_BASE_Y, WIFI_END_X, WIFI_BASE_Y);
    status |= GLIB_drawLine(&glibContext, WIFI_START_X + 5, WIFI_BASE_Y - 3, WIFI_START_X, WIFI_BASE_Y);
    status |= GLIB_drawLine(&glibContext, WIFI_START_X + 5, WIFI_BASE_Y + 4, WIFI_START_X, WIFI_BASE_Y);
    
    if (status != GLIB_OK) {
      LOG_PRINT("Failed to draw WiFi operation indicator\r\n");
      return SL_STATUS_FAIL;
    }
  }

  return SL_STATUS_OK;
}

// Utility function to check LCD status
bool is_lcd_initialized(void)
{
  return lcd_initialized;
}

// Function to get display dimensions (safer than direct access)
sl_status_t get_display_dimensions(uint32_t* width, uint32_t* height)
{
  if (!lcd_initialized || !glibContext.pDisplayGeometry || !width || !height) {
    return SL_STATUS_INVALID_PARAMETER;
  }

  *width = glibContext.pDisplayGeometry->xSize;
  *height = glibContext.pDisplayGeometry->ySize;
  
  return SL_STATUS_OK;
}


