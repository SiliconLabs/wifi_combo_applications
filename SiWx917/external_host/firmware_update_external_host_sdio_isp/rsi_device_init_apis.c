/*******************************************************************************
* @file  rsi_device_init_apis.c
* @brief
*******************************************************************************
* # License
* <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

/*
  Includes
 */
#include "rsi_driver.h"
#include "../../firmware_update/common.h"
#ifdef SIM_9118
#include "rsi_board.h"
#endif
#ifdef RSI_M4_INTERFACE
#ifdef ROM_WIRELESS
#include "rsi_chip.h"
#endif
#include "core_cm4.h"
#include "rsi_board.h"
#endif
#ifdef LINUX_PLATFORM
#include <stdint.h>
#include <sys/ioctl.h>
#endif
#include "rsi_hal.h"
#include "rsi_wlan_non_rom.h"
#include "rsi_wlan_config.h"
#include "../../firmware_update/ta_fw_file.h"
#include "../../firmware_update/m4_fw_file.h"
#include "../../firmware_update/m4_sfw_file.h"
#include "../../firmware_update/ta_sfw_file.h"

typedef uint32_t sl_status_t;
extern rsi_socket_info_non_rom_t *rsi_socket_pool_non_rom;
uint32_t sl_si91x_bl_upgrade_firmware(uint8_t *firmware_image, uint32_t fw_image_size, uint8_t flags);
int32_t sl_si91x_app_task_fw_update();

//extern const unsigned char fw_buffer[SIZE_OF_FW];
/** @addtogroup COMMON
* @{
*/
/*==============================================*/
/**
 * @brief      Power cycle the module and set the firmware image type to be loaded. This is a blocking API.
 * @param[in]   select_option - \ref LOAD_NWP_FW                    : To load Firmware image \n
 *                              \ref LOAD_DEFAULT_NWP_FW_ACTIVE_LOW : To load active low Firmware image. \n
 *                              Active low firmware will generate active low interrupts to indicate that packets are pending on the \n
 *                              module, instead of the default active high. \n
 * @return      Non-Zero Value - Failure
 * @note        **Precondition** - \ref rsi_driver_init() must be called before this API.
 * @note        Add the ENABLE_POC_IN_TOGGLE macro in the preprocessor to enable toggling of the POC_IN pin if it is controlled by the host to power cycle the module. For STM32 and EFM32 hosts, pins have been configured to drive the POC_IN pin. For EFR32, a pin has to be configured by the user to drive the POC_IN pin.  
 * @return      **Success**  - RSI_SUCCESS \n
 * @return      **Failure**  - Non-Zero Value
 * 
 */

int32_t rsi_device_init(uint8_t select_option)
{
  int32_t status = RSI_SUCCESS;
  SL_PRINTF(SL_DEVICE_INIT_ENTRY, COMMON, LOG_INFO);
#if (defined(RSI_SPI_INTERFACE) || defined(RSI_SDIO_INTERFACE))
  rsi_timer_instance_t timer_instance;
#endif

  if ((rsi_driver_cb_non_rom->device_state != RSI_DRIVER_INIT_DONE)) {
    // Command given in wrong state
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }

#if defined(RSI_SPI_INTERFACE) || defined(RSI_M4_INTERFACE) || defined(RSI_UART_INTERFACE) \
  || defined(RSI_SDIO_INTERFACE)
  // Board Initialization
  rsi_hal_board_init();
#endif

#ifdef RSI_M4_INTERFACE

  SysTick_Config(SystemCoreClock / 1000);
#ifdef RSI_WITH_OS
  // Set P2P Intr priority
  NVIC_SetPriority(SysTick_IRQn, SYSTICK_INTR_PRI);

  //Configure NVIC interrupt priorities
  nvic_priorities_config();
#endif
  if (!(P2P_STATUS_REG & TA_is_active)) {
#ifdef DEBUG_UART
    DEBUGOUT("\r\nM4 Indicate TA To Wakeup From Sleep\r\n");
#endif
    P2P_STATUS_REG |= M4_wakeup_TA;
    skip_bootload_sequence = 1;
  }
#ifdef DEBUG_UART
  DEBUGOUT("\r\n Wait Until TA is Active  \r\n");
#endif
  while (!(P2P_STATUS_REG & TA_is_active))
    ;
#ifdef DEBUG_UART
  DEBUGOUT("\r\n TA is in Active Mode Now \r\n");
#endif

  if (!skip_bootload_sequence) {
    do {
      retval = rsi_waitfor_boardready();
      if (retval == RSI_ERROR_IN_OS_OPERATION) {
#ifdef DEBUG_UART
        DEBUGOUT("\r\n Wait Until TA is Active \r\n");
#endif
      }
      if ((retval < 0) && (retval != RSI_ERROR_WAITING_FOR_BOARD_READY) && (retval != RSI_ERROR_IN_OS_OPERATION)) {
        SL_PRINTF(SL_DEVICE_INIT_EXIT_1, COMMON, LOG_ERROR, "retval:%d", retval);
        return retval;
      }
    } while ((retval == RSI_ERROR_WAITING_FOR_BOARD_READY) && (retval == RSI_ERROR_IN_OS_OPERATION));
    retval = rsi_select_option(select_option);
    if (retval < 0) {
      SL_PRINTF(SL_DEVICE_INIT_EXIT_2, COMMON, LOG_ERROR, "retval:%d", retval);
      return retval;
    }
  }
  // Update TX & RX DMA descriptor address
  rsi_update_tx_dma_desc(skip_bootload_sequence);
  rsi_update_rx_dma_desc();
#endif


#ifndef RSI_M4_INTERFACE
  // power cycle the module
  status = rsi_bl_module_power_cycle();
  if (status != RSI_SUCCESS) {
    SL_PRINTF(SL_DEVICE_INIT_MODULE_POWER_CYCLE_FAILURE, COMMON, LOG_ERROR, "status: %4x", status);
    return status;
  }
#endif
#if defined(RSI_SDIO_INTERFACE)
  // SDIO interface initialization
  status = rsi_sdio_iface_init();
#elif defined(RSI_SPI_INTERFACE)
  // SPI interface initialization
  status = rsi_spi_iface_init();
#elif defined(RSI_UART_INTERFACE)
  // UART interface initialization
  status = rsi_uart_iface_int();
#endif
  if (status != RSI_SUCCESS) {
    SL_PRINTF(SL_DEVICE_INIT_SPI_INIT_FAILURE, COMMON, LOG_ERROR, "status: %4x", status);
    return status;
  }

#if (defined(RSI_SPI_INTERFACE) || defined(RSI_SDIO_INTERFACE))
  rsi_init_timer(&timer_instance, RSI_BOARD_READY_WAIT_TIME); //40000
  do {
    status = rsi_bl_waitfor_boardready();

    if ((status < 0) && (status != RSI_ERROR_WAITING_FOR_BOARD_READY)) {
      SL_PRINTF(SL_DEVICE_INIT_BL_WAIT_FOR_BOARD_READY_ERROR, COMMON, LOG_ERROR, "status: %4x", status);
      return status;
    }

    if (rsi_timer_expired(&timer_instance)) {
      SL_PRINTF(SL_DEVICE_INIT_BOARD_READY_TIMEOUT, COMMON, LOG_ERROR);
      return RSI_ERROR_BOARD_READY_TIMEOUT;
    }

  } while (status == RSI_ERROR_WAITING_FOR_BOARD_READY);
#ifdef RSI_SPI_INTERFACE
  if (select_option == LOAD_NWP_FW) {
    rsi_set_intr_type(RSI_ACTIVE_HIGH_INTR);
  } else if (select_option == LOAD_DEFAULT_NWP_FW_ACTIVE_LOW) {

    status = rsi_set_intr_type(RSI_ACTIVE_LOW_INTR);
    if (status < 0) {
      SL_PRINTF(SL_DEVICE_INIT_SET_INTR_TYPE_ERROR, COMMON, LOG_ERROR, "status: %4x", status);
      return status;
    }
  }
#endif
#if (defined(RSI_SPI_INTERFACE) || defined(RSI_SDIO_INTERFACE))

//  sl_si91x_set_fast_fw_up();

  status = rsi_bl_select_option(select_option);
  if (status < 0) {
    SL_PRINTF(SL_DEVICE_INIT_BL_SELECT_OPTION_ERROR, COMMON, LOG_ERROR, "status: %4x", status);
    return status;
  }
  sl_si91x_app_task_fw_update();
#endif
#endif

#ifdef RSI_M4_INTERFACE
  rsi_m4_ta_interrupt_init();
  if (!(M4SS_P2P_INTR_SET_REG & RX_BUFFER_VALID)) {
    rsi_submit_rx_pkt();
  }
#else
#if ((defined RSI_SPI_INTERFACE) && defined(RSI_SPI_HIGH_SPEED_ENABLE))
  // Enable high speed SPI
  status = rsi_spi_high_speed_enable();
  if (status < 0) {
    SL_PRINTF(SL_DEVICE_INIT_HIGH_SPEED_ENABLE_ERROR, COMMON, LOG_ERROR, "status: %4x", status);
    return status;
  }
  // Switch host clock to high frequency
  rsi_switch_to_high_clk_freq();
#endif
#ifdef RSI_SDIO_INTERFACE
  // Switch host clock to high frequency
  rsi_switch_to_high_clk_freq();
#endif
#if (defined(RSI_SPI_INTERFACE) || defined(RSI_SDIO_INTERFACE))
  // Configure interrupt
  rsi_hal_intr_config(rsi_interrupt_handler);

  // Unmask interrupts
  rsi_hal_intr_unmask();
#endif
#endif
  // Updating state
  rsi_driver_cb_non_rom->device_state = RSI_DEVICE_INIT_DONE;
  SL_PRINTF(SL_DEVICE_INIT_EXIT_5, COMMON, LOG_INFO, "status: %4x", status);
  return status;
}

/*==============================================*/
/**
 * @brief      De-Initialize the module and reset the module. \n
 *             Reset is driven to the module by asserting the reset pin for some duration and releasing it. \n
 *             This is a non-blocking API.
 * @param[in]  Void
 * @return     0              - Success  \n
 * @return     Non-Zero Value - Failure
 */

int32_t rsi_device_deinit(void)
{
#if (RSI_WITH_OS && (defined RSI_WLAN_ENABLE))
  int32_t status = RSI_SUCCESS;
#endif
  SL_PRINTF(SL_DEVICE_DEINIT_ENTRY, COMMON, LOG_INFO);
#ifdef RSI_WLAN_ENABLE
  uint8_t i;
#endif
  if ((rsi_driver_cb_non_rom->device_state < RSI_DEVICE_INIT_DONE)) {
    // Command given in wrong state
    SL_PRINTF(SL_DEVICE_DEINIT_COMMAND_GIVEN_IN_WRONG_STATE,
              COMMON,
              LOG_ERROR,
              "state: %4x",
              rsi_driver_cb_non_rom->device_state);
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  if (rsi_driver_cb != NULL) {
#ifndef RSI_M4_INTERFACE
    // Mask the interrupt
    rsi_hal_intr_mask();
#else
    mask_ta_interrupt(RX_PKT_TRANSFER_DONE_INTERRUPT | TX_PKT_TRANSFER_DONE_INTERRUPT);
#endif
  }
#ifdef RSI_SPI_INTERFACE
  // Power cycle the module
  rsi_bl_module_power_cycle();
#endif
  if (rsi_driver_cb != NULL) {
    rsi_driver_cb->common_cb->state = RSI_COMMON_STATE_NONE;
#ifdef RSI_WLAN_ENABLE
    rsi_driver_cb->wlan_cb->state = RSI_WLAN_STATE_NONE;
#endif
  }

  rsi_driver_cb_non_rom->device_state = RSI_DRIVER_INIT_DONE;
  SL_PRINTF(SL_DEVICE_INIT_EXIT, COMMON, LOG_INFO);
  return RSI_SUCCESS;
}
#if ((defined RSI_SPI_INTERFACE) || (defined RSI_SDIO_INTERFACE))
/*==================================================*/
/**
 * @brief       Get the interrupt status.
 * @param[in]   NA
 * @return      1 - when there is an interrupt \n
 * @return      0 - when there is no interrupt
*/

uint8_t rsi_get_intr_status()
{
#if ((defined RSI_SPI_INTERFACE) && (!(defined RSI_ACTIVE_LOW)))
  return rsi_hal_intr_pin_status();
#else
  return !rsi_hal_intr_pin_status();
#endif
}


//! Enumeration for states in application
typedef enum si91x_wlan_app_state_e {
  SI91X_WLAN_INITIAL_STATE    = 0,
  SI91X_WLAN_RADIO_INIT_STATE = 1,
  SI91X_WLAN_FW_UPGRADE       = 2,
  SI91X_WLAN_FW_UPGRADE_DONE  = 3
} si91x_wlan_app_state_t;

#define SI91X_IN_BETWEEN_FILE 0UL
#define SI91X_START_OF_FILE   1UL
#define SI91X_END_OF_FILE     2UL
#define SI91X_FW_VER_SIZE     20UL
#define FW_HEADER_SIZE        64UL
#define XMODEM_CHUNK_SIZE     128UL
#define FIRST_PKT_XMODEM_CNT  32UL

#define SL_STATUS_OK    ((sl_status_t)0x0000)  ///< No error.
#define SL_STATUS_FAIL  ((sl_status_t)0x0001)  ///< Generic error.


int32_t sl_si91x_app_task_fw_update()
{
  si91x_wlan_app_state_t state = SI91X_WLAN_INITIAL_STATE;
  unsigned int remaining_bytes = 0, fw_image_size = 0, xmodem_chunk_rem = 0, xmodem_chunk_cnt = 0, chunk_check = 0, offset = 0, chunk_cnt = 0;
  int status = 0;
  printf("DEMO STARTED\n");
  //! Send the first chunk to extract header
  fw_image_size = SIZE_OF_FW;

  remaining_bytes = fw_image_size;

  //!Calculate the total number of chunks
  chunk_check = (fw_image_size / SI91X_CHUNK_SIZE);
  if (fw_image_size % SI91X_CHUNK_SIZE) {
    chunk_check += 1;
  }
  rsi_delay_ms(300);

  while(1) {
  switch (state) {
    case SI91X_WLAN_INITIAL_STATE:
    case SI91X_WLAN_FW_UPGRADE: {
      if (offset <= fw_image_size) {
        if (chunk_cnt == chunk_check) {
          printf("fw_upgrade Start\n");
          break;
        }
        if (chunk_cnt != 0) {
          printf("chunk_cnt: %d\n", chunk_cnt);
        }
        if (chunk_cnt == 0) {
          printf("fw_upgrade Start\n");
          status = sl_si91x_bl_upgrade_firmware(&fw_buffer[offset], SI91X_CHUNK_SIZE, SI91X_START_OF_FILE);
          if (status != SL_STATUS_OK) {
            printf("ERROR: 0x%lx\n", status);
            break;
          }
        } else if (chunk_cnt == (chunk_check - 1)) {
          status = sl_si91x_bl_upgrade_firmware(&fw_buffer[offset], SI91X_CHUNK_SIZE, SI91X_END_OF_FILE);
          if (status != SL_STATUS_OK) {
            printf("ERROR: 0x%lx\n", status);
            break;
          }
          printf("\r\nfw_upgrade Success\n");
          state = SI91X_WLAN_FW_UPGRADE_DONE;
        } else {
          status = sl_si91x_bl_upgrade_firmware(&fw_buffer[offset], SI91X_CHUNK_SIZE, SI91X_IN_BETWEEN_FILE);
          if (status != SL_STATUS_OK) {
            printf("ERROR: 0x%lx\n", status);
            break;
          }
        }
        offset += SI91X_CHUNK_SIZE;

        chunk_cnt++;
        rsi_delay_ms(100);
      }
    } break;
    case SI91X_WLAN_FW_UPGRADE_DONE: {

      printf("\r\nDEMO DONE\r\n");
      while (1)
        ;
    } break;
    default:
      break;
  }
  }
  return status;
}

#define SL_SI91X_SAFE_UPGRADE_ADDR        0x55408
#define SL_SI91X_SAFE_UPGRADE             BIT(12)
#define SL_SI91X_FW_START_OF_FILE         BIT(0)
#define SL_SI91X_FW_END_OF_FILE           BIT(1)

#ifndef SECURE_UPDATE
#define SL_SI91X_MIN_CHUNK_SIZE           4096
#else
#define SL_SI91X_MIN_CHUNK_SIZE     SI91X_CHUNK_SIZE
#endif
#define VERIFY_STATUS_AND_RETURN(status) \
  do {                                   \
    if (status != SL_STATUS_OK) {        \
      return status;                     \
    }                                    \
  } while (0);

sl_status_t sl_si91x_bl_upgrade_firmware(uint8_t *firmware_image, uint32_t fw_image_size, uint8_t flags)
{
  static uint16_t boot_cmd = 0;
  uint16_t read_value      = 0;
  uint32_t offset          = 0;
  uint32_t retval          = 0;
  uint32_t boot_insn = 0, poll_resp = 0;

  //! If it is a start of file set the boot cmd to pong valid
  if (flags & SL_SI91X_FW_START_OF_FILE) {
    boot_cmd = RSI_HOST_INTERACT_REG_VALID | RSI_PONG_VALID;
  }



  //! loop to execute multiple of 4K chunks
  while (offset < fw_image_size) {
    switch (boot_cmd) {
      case (RSI_HOST_INTERACT_REG_VALID | RSI_PING_VALID):
        boot_insn = RSI_PONG_WRITE;
        poll_resp = RSI_PING_AVAIL;
        boot_cmd  = RSI_HOST_INTERACT_REG_VALID | RSI_PONG_VALID;
        break;

      case (RSI_HOST_INTERACT_REG_VALID | RSI_PONG_VALID):
        boot_insn = RSI_PING_WRITE;
        poll_resp = RSI_PONG_AVAIL;
        boot_cmd  = RSI_HOST_INTERACT_REG_VALID | RSI_PING_VALID;
        break;
    }

    retval = rsi_bootloader_instructions(boot_insn, (uint16_t *)((uint8_t *)firmware_image + offset));
    VERIFY_STATUS_AND_RETURN(retval);

    while (1) {
      retval = rsi_bootloader_instructions(RSI_REG_READ, &read_value);
      VERIFY_STATUS_AND_RETURN(retval);

      if (read_value == (RSI_HOST_INTERACT_REG_VALID | poll_resp)) {
        break;
      }
    }
    offset += SL_SI91X_MIN_CHUNK_SIZE;
  }

  //! For last chunk set boot cmd as End of file reached
  if (flags & SL_SI91X_FW_END_OF_FILE) {
    boot_cmd = RSI_HOST_INTERACT_REG_VALID | RSI_EOF_REACHED;

    retval = rsi_bootloader_instructions(RSI_REG_WRITE, &boot_cmd);
    VERIFY_STATUS_AND_RETURN(retval);

    //! check for successful firmware upgrade
    do {
      retval = rsi_bootloader_instructions(RSI_REG_READ, &read_value);
      VERIFY_STATUS_AND_RETURN(retval);

      if((read_value & 0xFF) == 0xCC){
          printf("\r\n Firmware Update failed!!!\r\n");
          while(1);
      }

    } while (read_value != (RSI_HOST_INTERACT_REG_VALID | RSI_FWUP_SUCCESSFUL));
  }
  return retval;
}

#define SL_SI91X_SAFE_UPGRADE_ADDR        0x55408

#endif
/** @} */
