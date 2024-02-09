/***************************************************************************/ /**
 * @file si70xx_example.c
 * @brief si70xx example APIs
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
#include "rsi_debug.h"
#include "sl_si91x_si70xx.h"
#include "sl_si91x_i2c.h"
#include "i2c_leader_example.h"
#include "cmsis_os2.h"

//#include "sl_sleeptimer.h"
/*******************************************************************************
 ***************************  Defines / Macros  ********************************
 ******************************************************************************/
#define TX_THRESHOLD       0       // tx threshold value
#define RX_THRESHOLD       0       // rx threshold value
#define USER_REG_1         0xBA    // writing data into user register
#define DELAY_PERIODIC_MS1 2000    //sleeptimer1 periodic timeout in ms
/*******************************************************************************
 ******************************  Data Types  ***********************************
 ******************************************************************************/
/*******************************************************************************
 *************************** LOCAL VARIABLES   *******************************
 ******************************************************************************/
typedef sl_i2c_config_t sl_i2c_configuration_t;
//sl_sleeptimer_timer_handle_t timer1; //sleeptimer1 handle
boolean_t delay_timeout = false;     //Indicates sleeptimer1 timeout
/*******************************************************************************
 **********************  Local Function prototypes   ***************************
 ******************************************************************************/
static void i2c_leader_callback(sl_i2c_instance_t i2c_instance, uint32_t status);
void si70xx_example_init(void);
void si70xx_example_process_action(void);

//Sleeptimer timeout callbacks
//static void on_timeout_timer1(sl_sleeptimer_timer_handle_t *handle, void *data);
/*******************************************************************************
 **************************   GLOBAL VARIABLES   *******************************
 ******************************************************************************/
float sensor_data = 0.0;
/*******************************************************************************
 * RHT example initialization function
 ******************************************************************************/
void si70xx_example_init(void)
{
  sl_status_t status;
  uint8_t firm_rev;
  sl_i2c_configuration_t i2c_config;
  uint32_t humidity;
  int32_t temperature;
  uint8_t value;
  i2c_config.mode           = SL_I2C_LEADER_MODE;
  i2c_config.transfer_type  = SL_I2C_USING_INTERRUPT;
  i2c_config.operating_mode = SL_I2C_STANDARD_MODE;
  i2c_config.i2c_callback   = i2c_leader_callback;
  do {
      // Initialize I2C bus
    status = sl_i2c_driver_init(I2C, &i2c_config);
    if (status != SL_I2C_SUCCESS) {
      DEBUGOUT("sl_i2c_driver_init : Invalid Parameters, Error Code: 0x%ld \n", status);
      break;
    }
    status = sl_i2c_driver_configure_fifo_threshold(I2C, TX_THRESHOLD, RX_THRESHOLD);
    if (status != SL_I2C_SUCCESS) {
      DEBUGOUT("sl_i2c_driver_configure_fifo_threshold : Invalid Parameters, Error Code: 0x%ld \n", status);
      break;
    }
    // reset the sensor
    status = sl_si91x_si70xx_reset(I2C, SI7021_ADDR);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor reset un-successful, Error Code: 0x%ld \n", status);
      break;
    } else {
        osDelay(100);
    }
    // Initializes sensor and reads electronic ID 1st byte
    status = sl_si91x_si70xx_init(I2C, SI7021_ADDR, SL_EID_FIRST_BYTE);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor initialization un-successful, Error Code: 0x%ld \n", status);
      break;
    }
    // Initializes sensor and reads electronic ID 2nd byte
    status = sl_si91x_si70xx_init(I2C, SI7021_ADDR, SL_EID_SECOND_BYTE);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor initialization un-successful, Error Code: 0x%ld \n", status);
      break;
    }
    // Get sensor internal firmware version of sensor
    status = sl_si91x_si70xx_get_firmware_revision(I2C, SI7021_ADDR, &firm_rev);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor firmware version un-successful, Error Code: 0x%ld \n", status);
      break;
    }
    // write register data into sensor
    status = sl_si91x_si70xx_write_control_register(I2C, SI7021_ADDR, SL_RH_T_USER_REG, USER_REG_1);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor user register 1 write data failed, Error Code: 0x%ld \n", status);
      break;
    }
    // Reads register data from sensor
    status = sl_si91x_si70xx_read_control_register(I2C, SI7021_ADDR, SL_RH_T_USER_REG, &value);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor user register 1 read failed, Error Code: 0x%ld \n", status);
      break;
    }
    // Reads temperature from humidity from sensor
    status = sl_si91x_si70xx_read_temp_from_rh(I2C, SI7021_ADDR, &humidity, &temperature);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor temperature read failed, Error Code: 0x%ld \n", status);
      break;
    }
    // measure humidity data from sensor
    status = sl_si91x_si70xx_measure_humidity(I2C, SI7021_ADDR, &humidity);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor humidity read failed, Error Code: 0x%ld \n", status);
      break;
    }
    // measure temperature data from sensor
    status = sl_si91x_si70xx_measure_temperature(I2C, SI7021_ADDR, &temperature);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor temperature read failed, Error Code: 0x%ld \n", status);
      break;
    }
  } while (false);
}

/*******************************************************************************
 * Function will run continuously in while loop and reads relative humidity and
 * temperature from sensor
 ******************************************************************************/
void si70xx_example_process_action(void)
{
  sl_status_t status;
  uint32_t humidity   = 0;
  int32_t temperature = 0;
    delay_timeout = false;
    // Reads humidity(hold master mode) measurement from sensor
    status = sl_si91x_si70xx_measure_rh_and_temp(I2C, SI7021_ADDR, &humidity, &temperature);
    if (status != SL_STATUS_OK) {
      DEBUGOUT("Sensor temperature read failed, Error Code: 0x%ld \n", status);
    } else {
      DEBUGOUT("\r\nSensor temperature read is successful\r\n");
    }
    sensor_data = temperature;
    DEBUGOUT("\r\nSensor temperature  in celsius: %ld\r\n", temperature);
}

/*******************************************************************************
 * Callback Function
 ******************************************************************************/
void i2c_leader_callback(sl_i2c_instance_t i2c_instance, uint32_t status)
{
  (void)i2c_instance;
  switch (status) {
    case SL_I2C_DATA_TRANSFER_COMPLETE:
      break;
    default:
      break;
  }
}
