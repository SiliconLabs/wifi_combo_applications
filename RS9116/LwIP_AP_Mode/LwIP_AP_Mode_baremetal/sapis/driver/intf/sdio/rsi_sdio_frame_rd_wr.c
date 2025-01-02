/**
 * @file     rsi_sdio_frame_rd_wr.c
 * @version  0.1
 * @date     15 Aug 2015
 *
 * Copyright(C) Redpine Signals 2015
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief SDIO Control: Functions used to control SDIO frame read/write functions
 *
 * @section Description
 * SDIO function to read/write management descriptors frames to/from the Wi-Fi module
 *
 * @section Improvements
 * Function header is modified for frameDscRd() function
 */



/*
 * Includes
 */
#include "rsi_driver.h"
#ifdef RSI_SDIO_INTERFACE
/**
 * Global Variables
 */
/*====================================================*/
/**
 * @fn          int16_t rsi_frame_read(uint8_t *pkt_buffer)
 * @brief This  function is used to read the response from module.
 * @param[in]   uint8_t *pkt_buffer, pointer to the buffer to which packet has to read
 *              which is used to store the response from the module
 * @param[out]  none
 * @return      errCode
 *              -1 = SDIO busy / Timeout
 *              -2 = SDIO Failure
 *              0  = SUCCESS
 * @section description 
 * This is a common function to read response for all the command and data from Wi-Fi module.
 */
uint8_t *rsi_frame_read(uint8_t *read_buff)
{
  int16_t   retval = RSI_SUCCESS;             
  uint8_t response            = 0;
  uint16_t no_of_blocks;

  //! Read number of blocks
  retval = rsi_reg_rd(0xf1, &response);

  if (retval != RSI_SUCCESS)
    return NULL;

  no_of_blocks = (response & 0x1F);


  //! Frame read
  retval = rsi_sdio_read_multiple(read_buff, no_of_blocks);
  return read_buff;
}


/*====================================================*/
/**
 * @fn          int16_t rsi_frame_write(rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param)
 * @brief       Common function for all the commands.
 * @param[in]   uFrameDsc uFrameDscFrame, frame descriptor
 * @param[in]   uint8_t *payloadparam, pointer to the command payload parameter structure
 * @param[in]   uint16_t size_param, size of the payload for the command
 * @return      errCode
 *              -1 = SDIO busy / Timeout
 *              -2 = SDIO Failure
 *              0  = SUCCESS
 * @section description 
 * This is a common function used to process a command to the Wi-Fi module.
 */
int16_t rsi_frame_write(rsi_frame_desc_t *uFrameDscFrame,uint8_t *payloadparam,uint16_t size_param)
{
	int16_t              retval = RSI_SUCCESS;
	uint16_t Addr 								= 0x0000;
	uint16_t no_of_blocks         = 0;
	uint16_t queue_type=0;
	
  //! Calculate number of blocks
	no_of_blocks = ((size_param +16)/256);
	if((size_param + 16) % 256)
	{
		no_of_blocks = no_of_blocks + 1;
	}
	
	Addr = (no_of_blocks * 256);
	
	queue_type = (uFrameDscFrame->frame_len_queue_no[1] >> 4);
  Addr  = (Addr | (queue_type << 12));


  //! Transfer packet 
  retval = rsi_sdio_write_multiple((uint8_t *)uFrameDscFrame, Addr, no_of_blocks);
  
  return retval;
}
#endif
