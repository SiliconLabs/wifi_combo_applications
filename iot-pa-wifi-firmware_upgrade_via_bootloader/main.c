// Header files
#include "rsi_driver.h"
#include "integer.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

// Wlan task priority
#define RSI_FW_UPDATE_TASK_PRIORITY 1

// Wlan task stack size
#define RSI_FW_UPDATE_TASK_STACK_SIZE (512 * 2)

int32_t status = RSI_SUCCESS;

int32_t rsi_app_task_fw_update_via_bootloader(void);


void main_loop(void)
{

    while(1)
    {
#ifndef RSI_WITH_OS
        //! event loop
        rsi_wireless_driver_task();
#endif


    }

}

DWORD get_fattime(void)
{
  return (28 << 25) | (2 << 21) | (1 << 16);
}
/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/

int main(void)
{
#ifdef RSI_WITH_OS
    rsi_task_handle_t wlan_task_handle = NULL;

    //! OS case
    //! Firmware update task created
    rsi_task_create((rsi_task_function_t)(int32_t)rsi_app_task_fw_update_via_bootloader,
                    (uint8_t *)"wlan_task",
                    RSI_FW_UPDATE_TASK_STACK_SIZE,
                    NULL,
                    RSI_FW_UPDATE_TASK_PRIORITY,
                    &wlan_task_handle);

    //! OS TAsk Start the scheduler
    rsi_start_os_scheduler();


#else
    //! Firmware update function
    rsi_app_task_fw_update_via_bootloader();

    main_loop();

#endif

}
