/******************************************************************************
* OnOffLightDevice.h
******************************************************************************
* Program Description:
* This file contains the on off device
******************************************************************************/
#ifndef _ONOFF_LIGHT_DEVICE_H_
#define _ONOFF_LIGHT_DEVICE_H_
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Interface.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
void Handle_ZCL_Message( uint8_t buffer_index, APSDE_Data_Indication_t *p_buffer );
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                          /* _ONOFF_LIGHT_DEVICE_H_ */