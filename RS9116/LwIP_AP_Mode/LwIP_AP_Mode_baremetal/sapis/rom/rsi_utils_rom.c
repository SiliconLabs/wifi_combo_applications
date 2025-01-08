/**
 * @file       rsi_utils.c
 * @version    0.1
 * @date       15 Aug 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief UTIL: Generic function utilities
 *
 * @section Description
 * This file implements misc utilities/functions.
 *
 */


/**
 * Include files
 */
#ifndef ROM_WIRELESS 
#include "rsi_driver.h"


/**
 * Global defines
 */

/*=============================================================================*/
/**
 * @fn              void ROM_WL_rsi_uint16_to_2bytes(global_cb_t *global_cb_p, uint8_t *dBuf, uint16_t val)
 * @brief           Convert uint16 to two byte array
 * @param[in]       global_cb_t *global_cb_p,pointer to the common buffer
 * @param[in]       uint8_t *dBuf,pointer to buffer to put the data in
 * @param[in]       uint16_t data to convert
 * @param[out]      none
 * @return          none
 */
void ROM_WL_rsi_uint16_to_2bytes(global_cb_t *global_cb_p, uint8_t *dBuf, uint16_t val)
{
  if(global_cb_p->endian == IS_LITTLE_ENDIAN)
  {
    dBuf[0] = val & 0x00ff;
    dBuf[1] = (val >> 8) & 0x00ff;
  }
  else
  {
    dBuf[1] = val & 0x00ff;
    dBuf[0] = (val >> 8) & 0x00ff;
  }
}

/*=============================================================================*/
/**
 * @fn              void ROM_WL_rsi_uint32_to_4bytes(global_cb_t *global_cb_p,  uint8_t *dBuf, uint32_t val)
 * @brief           Convert uint32 to four byte array
 * @param[in]       global_cb_t *global_cb_p,pointer to the common buffer
 * @param[in]       uint8_t *dBuf,pointer to the buffer to put the data in
 * @param[in]       uint32_t data to convert
 * @param[out]      none
 * @return          none
 */
void ROM_WL_rsi_uint32_to_4bytes(global_cb_t *global_cb_p,  uint8_t *dBuf, uint32_t val)
{
  if(global_cb_p->endian == IS_LITTLE_ENDIAN)
  {
    dBuf[0] = val & 0x000000ff;
    dBuf[1] = (val >> 8) & 0x000000ff;
    dBuf[2] = (val >> 16) & 0x000000ff;
    dBuf[3] = (val >> 24) & 0x000000ff;
  }
  else
  {
    dBuf[3] = val & 0x000000ff;
    dBuf[2] = (val >> 8) & 0x000000ff;
    dBuf[1] = (val >> 16) & 0x000000ff;
    dBuf[0] = (val >> 24) & 0x000000ff;
  }
}

/*=============================================================================*/
/**
 * @fn              uint16_t ROM_WL_rsi_bytes2R_to_uint16(global_cb_t *global_cb_p, uint8_t *dBuf)
 * @brief           Convert a 2 byte array to uint16, first byte in array is LSB
 * @param[in]       global_cb_t *global_cb_p,pointer to the common buffer
 * @param[in]       uint8_t *dBuf,pointer to a buffer to get the data from
 * @param[out]      none
 * @return          uint16_t, converted data
 */
uint16_t ROM_WL_rsi_bytes2R_to_uint16(global_cb_t *global_cb_p, uint8_t *dBuf)
{
  uint16_t        val;    
  if(global_cb_p->endian == IS_LITTLE_ENDIAN)
  {
    val = dBuf[1];
    val <<= 8;
    val |= dBuf[0] & 0x000000ff;
  }
  else
  {
    val = dBuf[0];
    val <<= 8;
    val |= dBuf[1] & 0x000000ff;
  }
  return val;
}

/*=============================================================================*/
/**
 * @fn           uint32_t ROM_WL_rsi_bytes4R_to_uint32(global_cb_t *global_cb_p,  uint8_t *dBuf)
 * @brief        Convert a 4 byte array to uint32, first byte in array is LSB
 * @param[in]    global_cb_t *global_cb_p,pointer to the common buffer
 * @param[in]    uint8_t *dBuf,pointer to buffer to get the data from
 * @param[out]   none
 * @return       uint32, converted data
 */
uint32_t ROM_WL_rsi_bytes4R_to_uint32(global_cb_t *global_cb_p,  uint8_t *dBuf)
{
  //! the 32-bit value to return
  uint32_t            val;            

  if(global_cb_p->endian == IS_LITTLE_ENDIAN)
  {
    val = dBuf[3];
    val <<= 8;
    val |= dBuf[2] & 0x000000ff;
    val <<= 8;
    val |= dBuf[1] & 0x000000ff;
    val <<= 8;
    val |= dBuf[0] & 0x000000ff;
  }
  else
  {
    val = dBuf[0];
    val <<= 8;
    val |= dBuf[1] & 0x000000ff;
    val <<= 8;
    val |= dBuf[2] & 0x000000ff;
    val <<= 8;
    val |= dBuf[3] & 0x000000ff;
  }

  return val;
}
#endif
