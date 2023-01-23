/**
 * @file    rsi_fw_update_via_bootloader.c
 * @version 0.1
 * @date    13-Nov-2018
 *
 *  Copyright(C) Redpine Signals 2018
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 */
/**
 * Include files
 * */


#include "rsi_wlan_config.h"
//!  Standard library that contains definitions for common types, variables, and functions
#include "stdlib.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_driver.h"

//! Include file to refer data types
#include "rsi_data_types.h"

#ifdef SD_CARD_FW
#include "ff.h"
#include "diskio.h"
#include "microsdconfig.h"
#endif

#ifdef INTERNAL_FLASH_FW
#include "fw_file.h"
#endif

//! Memory length for driver
#define GLOBAL_BUFF_LEN    15000

#define RSI_APP_BUF_SIZE    1600

//! Each chunk's size
#define RSI_CHUNK_SIZE     4096UL

//! First chunk
#define RSI_START_OF_FILE   1UL

//! Intermediate chunks
#define RSI_IN_BETWEEN_FILE 0UL

//! Last chunk
#define RSI_END_OF_FILE     2UL

//! Buffer length to store the firmware version
#define RSI_FW_VER_SIZE     20UL

// Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY 2

// Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE (512 * 2)

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN] = {0};

//! Buffer to store the firmware version
uint8_t fw_version[RSI_FW_VER_SIZE]={0};

uint32_t t_start = 0;
uint32_t t_end = 0;
uint32_t xfer_time = 0;
float secs = 0;

//! Enumeration for states in application
typedef enum rsi_wlan_app_state_e
{
  RSI_WLAN_INITIAL_STATE          = 0,
  RSI_WLAN_RADIO_INIT_STATE	      = 1,
  RSI_WLAN_FW_UPDATE		          = 2,
  RSI_WLAN_FW_UPDATE_DONE         = 3
}rsi_wlan_app_state_t;

//! wlan application control block
typedef struct rsi_wlan_app_cb_s
{
  //! wlan application state
  rsi_wlan_app_state_t state;

  //! length of buffer to copy
  uint32_t length;

  //! application buffer
  uint8_t buffer[RSI_APP_BUF_SIZE];

  //! to check application buffer availability
  uint8_t buf_in_use;

  //! application events bit map
  uint32_t event_map;

}rsi_wlan_app_cb_t;

//! Declaration of application control block structure
rsi_wlan_app_cb_t rsi_wlan_app_cb;

//! firmware burn variables and buffer
uint32_t chunk_cnt = 1u, chunk_check = 0u, offset = 0u, fw_image_size = 0u;
extern int32_t  status;
uint8_t recv_buffer[RSI_CHUNK_SIZE] = {0};// fw_version[RSI_FW_VER_SIZE]={0};


//! Firmware update request structure
typedef struct fwupeq_s{
  uint16_t control_flags;
  uint16_t sha_type;
  uint32_t magic_no;
  uint32_t image_size;
  uint32_t fw_version;
  uint32_t flash_loc;
  uint32_t crc;
}fwreq_t;

//! This function gets the size of the firmware
uint32_t get_fw_size(char *buffer)
{
  fwreq_t *fw = (fwreq_t *)buffer;
  return fw->image_size;
}

#ifdef SD_CARD_FW
#define FW_FILE_NAME1        "RS9116W.2.6.0.0.34.rps"

//! SD Card parameters
UINT bytesRead = 0u;
FIL g_fileObject_n = {0};
FATFS Fatfs = {0};
FRESULT     error = FR_OK;

FRESULT file_open(void)
{
  DIR dj = {0};
  FILINFO fno = {0};
  FRESULT res = FR_OK;

  //! Find the .rps fw file in the root directory of SD card
  res = f_findfirst(&dj, &fno, "/", "*.rps");
  if (!fno.fname[0]) {
      LOG_PRINT("\r\nNo file in uSD card\r\n");
      while(1);
  }

  //! Open the firmware image file (.rps)
  res = f_open(&g_fileObject_n, fno.fname, FA_READ);
  if (res != FR_OK) {
      LOG_PRINT("\r\nfile open error\r\n");
      while(1);
  }

  return res;
}

int8_t openFile_sd()
{
  FRESULT error = FR_OK;

  // Initialize file system for SD card
  MICROSD_Init();

  error = f_mount(&Fatfs, "", 0);
  if (error) {

      LOG_PRINT("\r\nNo FAT file system in uSD card\r\n");
      while(1);
  }

  //! Open a file on SD card
  error =  f_open(&g_fileObject_n, FW_FILE_NAME1, FA_READ);
  if (error) {
      LOG_PRINT("\r\nOpen file failed\r\n");
      return -1;
  }
  return 0;
}



FRESULT file_read( void )
{
  uint8_t i = 0u;
  FRESULT res = FR_OK;

  uint8_t *f_ptr = &recv_buffer[0];

    for(i = 0u; i < 8u ; i++)
    {
        //! Read 512 bytes at a time from the file
        res = f_read(&g_fileObject_n, f_ptr, 512U, &bytesRead);
        if(res != FR_OK)
        {
        	LOG_PRINT("\r\nfile read error\r\n");
            return res;
        }
        //! Increment the pointer by 512 bytes
        f_ptr = f_ptr + 512;
    }

    return res;
}
#elif INTERNAL_FLASH_FW

int file_read_flash( uint32_t offset )
{
  memcpy(recv_buffer, &fw_array[offset], sizeof(recv_buffer) );

  return 0;
}
#endif

int32_t rsi_app_task_fw_update_via_bootloader(void)
{
  //! Driver initialization
  status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
  if((status < 0) || (status > GLOBAL_BUFF_LEN)) {
      return status;
  }
  //! Device initialization
  status = rsi_device_init(BURN_NWP_FW);
  if(status == RSI_SUCCESS) {
      LOG_PRINT("\r\nDevice initialization is successful\r\n");
  }
  else {
      LOG_PRINT("\r\nDevice initialization failed, Error code: %lX\r\n",status);
      return status;
  }

  t_start = rsi_hal_gettickcount();

  while(1){

    switch(rsi_wlan_app_cb.state)
    {
        case RSI_WLAN_INITIAL_STATE:
        {
#ifdef SD_CARD_FW

            LOG_PRINT("\r\nStarted reading the firmware file from the microSD card... \r\n");

            //! Open file on SD card
            error = openFile_sd();
            if (error)
            {
              LOG_PRINT("\r\nOpen file failed, Error code: %d\r\n",error);
              return -1;
            }
#endif
            //! Update wlan application state
            rsi_wlan_app_cb.state = RSI_WLAN_FW_UPDATE;
        }
      //no break
      case RSI_WLAN_FW_UPDATE:
        {
#ifdef SD_CARD_FW
          //! Read the SD card file contents chunk by chunk
          error = file_read();
#elif INTERNAL_FLASH_FW

          LOG_PRINT("\r\nStarted reading firmware file from the EFM32's Internal flash... \r\n");

          if( file_read_flash(offset) != 0)
          {
              LOG_PRINT("Flash read failed\n");
              while(1);
          }
#endif

          //! Get the total size of firmware file
          fw_image_size = get_fw_size((char *)recv_buffer);
          LOG_PRINT("\r\nFirmware file size: %ld bytes\r\n",fw_image_size);

          //! Calculate the total number of chunks
          chunk_check = (fw_image_size/RSI_CHUNK_SIZE);
          if(fw_image_size%RSI_CHUNK_SIZE)
          {
              chunk_check += 1;
          }
          LOG_PRINT("\r\nTotal chunks: %ld\r\n",chunk_check);

          //! Loop until all the chunks are burnt to the flash of RS9116
          while(offset <= fw_image_size)
          {
            if(chunk_cnt == chunk_check+1)
            {
              break;
            }
            if(chunk_cnt > 1)
            {
#ifdef SD_CARD_FW
              //! Read the SD card file contents chunk by chunk
              error = file_read();
#elif INTERNAL_FLASH_FW
                if( file_read_flash(offset) != 0)
                {
                    LOG_PRINT("Flash read failed\n");
                    while(1);
                }
#endif
                LOG_PRINT("\r\nChunk count: %ld\r\n",chunk_cnt);

            }
            if(chunk_cnt == 1)
            {
              LOG_PRINT("\r\nFimrware update started..\r\n");
              status = rsi_bl_upgrade_firmware((uint8_t *)recv_buffer,RSI_CHUNK_SIZE,RSI_START_OF_FILE);           
              if(status != RSI_SUCCESS)
              {
                  LOG_PRINT("\r\nBurning first chunk to RS9116 has failed, Error code:%lX\r\n",status);
                  break;
              }
              LOG_PRINT("\r\nChunk count: %ld\r\n",chunk_cnt);

            }
            else if(chunk_cnt == chunk_check)
            {
              status = rsi_bl_upgrade_firmware((uint8_t *)recv_buffer,RSI_CHUNK_SIZE,RSI_END_OF_FILE);
              if(status != RSI_SUCCESS)
              {
                LOG_PRINT("\r\nBurning last chunk to RS9116 has failed, Error code:%lX\r\n",status);
                break;
              }
              LOG_PRINT("\r\nFirmware update is successful\r\n");
              t_end = rsi_hal_gettickcount();
              xfer_time = t_end-t_start;
              secs  = xfer_time/1000;
#ifdef SD_CARD_FW

              LOG_PRINT("\r\nTime taken for firmware update using microSD card:%3.0lf secs\r\n",secs);
#elif INTERNAL_FLASH_FW

              LOG_PRINT("\r\nTime taken for firmware update using EFM32's internal flash:%3.0lf secs\r\n",secs);
#endif
              rsi_wlan_app_cb.state = RSI_WLAN_FW_UPDATE_DONE;
            }
            else
            {
              status = rsi_bl_upgrade_firmware((uint8_t *)recv_buffer,RSI_CHUNK_SIZE,RSI_IN_BETWEEN_FILE);
              if(status != RSI_SUCCESS)
              {
                LOG_PRINT("\r\nBurning intermediate chunk: %ld to RS9116 has failed, Error code:%lX\r\n",chunk_cnt, status);
                break;
              }

            }
            offset += RSI_CHUNK_SIZE;
            memset(recv_buffer, 0, sizeof(recv_buffer));
            chunk_cnt++;
          }
        }
        break;
      case RSI_WLAN_FW_UPDATE_DONE :
        {
        status = rsi_wireless_deinit();
        if (status != RSI_SUCCESS) {
          LOG_PRINT("\r\nWireless de-initialization has failed, Error Code : 0x%lX\r\n", status);
          break;
        }
        else {
            LOG_PRINT("\r\nWireless de-initialization is successful\r\n");
        }

#ifdef RSI_WITH_OS
        rsi_task_handle_t driver_task_handle = NULL;
        //! Task created for Driver task
        rsi_task_create((rsi_task_function_t)rsi_wireless_driver_task,
                        (uint8_t *)"driver_task",
                        RSI_DRIVER_TASK_STACK_SIZE,
                        NULL,
                        RSI_DRIVER_TASK_PRIORITY,
                        &driver_task_handle);
#endif

          //! WiSeConnect initialization
          status = rsi_wireless_init(0, 0);
          if(status != RSI_SUCCESS)
          {
             LOG_PRINT("\r\nWireless initialization failed, Error Code : 0x%lX\r\n", status);
             return status;
          }
          else {
              LOG_PRINT("\r\nWireless initialization is successful\r\n");
          }

          memset(fw_version,0,sizeof(fw_version));
          status = rsi_wlan_get(RSI_FW_VERSION,fw_version,sizeof(fw_version));
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT("Reading firmware version failed\n");
            break;
          }
          LOG_PRINT("\r\nFirmware version after update is: %s\r\n",fw_version);
          while(1);
        }
        break;
      default:
        break;
    }
  }

  return status;
}

