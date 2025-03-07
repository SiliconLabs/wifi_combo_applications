/***************************************************************************/ /**
 * @file sl_ulp_adc_example.c
 * @brief ADC example
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
#include "sl_si91x_adc.h"
#include "sl_ulp_adc_example.h"
#include "rsi_debug.h"
#include "rsi_chip.h"
#include "sl_adc_instances.h"
#include "sl_si91x_adc_common_config.h"

/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/
/* Macros */
#define CHANNEL_SAMPLE_LENGTH 1023       // Number of ADC sample collect for operation
#define ADC_MAX_OP_VALUE      4095       // Maximum output value get from adc data register
#define ADC_DATA_CLEAR        0xF7FF     // Clear the data if 12th bit is enabled
#define VREF_VALUE            3.3        // reference voltage
#define ADC_PING_BUFFER       0x24060800 // ADC buffer starting address

/*******************************************************************************
 *************************** LOCAL VARIABLES   *******************************
 ******************************************************************************/
static float vref_value              = (float)VREF_VALUE;
static boolean_t chnl0_complete_flag = false;
static int16_t adc_output[CHANNEL_SAMPLE_LENGTH];
/*******************************************************************************
 **********************  Local Function prototypes   ***************************
 ******************************************************************************/
static void callback_event(uint8_t channel_no, uint8_t event);

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/
void hardware_setup(void);
/*******************************************************************************
 * ADC example initialization function
 ******************************************************************************/
void adc_example_init(void)
{
  sl_adc_version_t version;
  sl_status_t status;

  sl_adc_channel_config.rx_buf[0]            = adc_output;
  sl_adc_channel_config.chnl_ping_address[0] = ADC_PING_BUFFER; /* ADC Ping address */
  sl_adc_channel_config.chnl_pong_address[0] =
    ADC_PING_BUFFER + (sl_adc_channel_config.num_of_samples[0]); /* ADC Pong address */
  do {
    // Version information of ADC driver
    version = sl_si91x_adc_get_version();
    DEBUGOUT("ADC version is fetched successfully \n");
    DEBUGOUT("API version is %d.%d.%d\n", version.release, version.major, version.minor);
    // Converting M4 to ULP mode.
    hardware_setup();
    status = sl_si91x_adc_init(sl_adc_channel_config, sl_adc_config, vref_value);
    /* Due to calling trim_efuse API on ADC init in driver it will change the clock frequency,
      if we are not initialize the debug again it will print the garbage data in console output. */
    DEBUGINIT();
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_init: Error Code : %lu \n", status);
      break;
    }
    DEBUGOUT("ADC Initialization Success\n");
    status = sl_si91x_adc_set_channel_configuration(sl_adc_channel_config, sl_adc_config);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_channel_set_configuration: Error Code : %lu \n", status);
      break;
    }
    DEBUGOUT("ADC Channel Configuration Successfully \n");
    // Register user callback function
    status = sl_si91x_adc_register_event_callback(callback_event);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_register_event_callback: Error Code : %lu \n", status);
      break;
    }
    DEBUGOUT("ADC user event callback registered successfully \n");
    status = sl_si91x_adc_start(sl_adc_config);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("sl_si91x_adc_start: Error Code : %lu \n", status);
      break;
    }
    DEBUGOUT("ADC started Successfully\n");
  } while (false);
}

/*******************************************************************************
 * Function will run continuously and will wait for trigger
 ******************************************************************************/
void adc_example_process_action(void)
{
  sl_status_t status;
  uint32_t sample_length;
  uint8_t chnl_num = 0;
  uint16_t adc_value;
  float vout = 0;
  if (chnl0_complete_flag) {
    // ADC operation mode if FIFO then it will execute, here it will give equivalent voltage of 12 bit adc output.
    if (!sl_adc_config.operation_mode) {
      for (chnl_num = 0; chnl_num < sl_adc_config.num_of_channel_enable; chnl_num++) {
        status = sl_si91x_adc_read_data(sl_adc_channel_config, chnl_num);
        if (status != SL_STATUS_OK) {
          DEBUGOUT("sl_si91x_adc_read_data: Error Code : %lu \n", status);
        }
        for (sample_length = 0; sample_length < sl_adc_channel_config.num_of_samples[chnl_num]; sample_length++) {
          /* In two’s complement format, the MSb (11th bit) of the conversion result determines the polarity,
             when the MSb = ‘0’, the result is positive, and when the MSb = ‘1’, the result is negative*/
          if (adc_output[sample_length] & SIGN_BIT) {
            // Full-scale would be represented by a hexadecimal value, full-scale range of ADC result values in two’s complement.
            adc_output[sample_length] = (int16_t)(adc_output[sample_length] & (ADC_DATA_CLEAR));
          } else { // set the MSb bit.
            adc_output[sample_length] = adc_output[sample_length] | SIGN_BIT;
          }
          vout = (((float)adc_output[sample_length] / (float)ADC_MAX_OP_VALUE) * vref_value);
          //For differential type it will give vout.
          if (sl_adc_channel_config.input_type[chnl_num]) {
            vout = vout - (vref_value / 2);
          }
          DEBUGOUT("ADC Measured input[%ld] :%0.2fV \n", sample_length, (double)vout);
        }
      }
    } else {
      chnl0_complete_flag = false;
      status              = sl_si91x_adc_read_data_static(sl_adc_channel_config, sl_adc_config, &adc_value);
      if (status != SL_STATUS_OK) {
        DEBUGOUT("sl_si91x_adc_read_data_static: Error Code : %lu \n", status);
      }
      // Read the data from register and store it in variable.
      adc_output[0] = (int16_t)adc_value;
      /* In two’s complement format, the MSb (11th bit) of the conversion result determines the polarity,
         when the MSb = ‘0’, the result is positive, and when the MSb = ‘1’, the result is negative*/
      if (adc_output[0] & SIGN_BIT) {
        // Full-scale would be represented by a hexadecimal value, full-scale range of ADC result values in two’s complement.
        adc_output[0] = (int16_t)(adc_output[0] & (ADC_DATA_CLEAR));
      } else { // set the MSb bit.
        adc_output[0] = adc_output[0] | SIGN_BIT;
      }
      vout = (((float)adc_output[0] / (float)ADC_MAX_OP_VALUE) * vref_value);
      //For differential type it will give vout.
      if (sl_adc_channel_config.input_type[0]) {
        vout = vout - (vref_value / 2);
        DEBUGOUT("Differential ended input  :%lf\n", (double)vout);
      } else {
        DEBUGOUT("Single ended input :%lf\n", (double)vout);
      }
    }
    if (!sl_adc_config.operation_mode) {
      status = sl_si91x_adc_deinit(sl_adc_config);
      if (status != SL_STATUS_OK) {
        DEBUGOUT("sl_si91x_adc_deinit: Error Code : %lu \n", status);
      }
      chnl0_complete_flag = false;
      DEBUGOUT("ADC deinit successfully \n");
    }
  }
}

/*******************************************************************************
 * Callback event function
 * It is responsible for the event which are triggered by ADC interface
 *
 * @param  event       : INTERNAL_DMA => Single channel data acquisition done.
 ******************************************************************************/
static void callback_event(uint8_t channel_no, uint8_t event)
{
  if (event == SL_INTERNAL_DMA) {
    if (channel_no == 0) {
      chnl0_complete_flag = true;
    }
  } else if (event == SL_ADC_STATIC_MODE_EVENT) {
    chnl0_complete_flag = true;
  }
}
