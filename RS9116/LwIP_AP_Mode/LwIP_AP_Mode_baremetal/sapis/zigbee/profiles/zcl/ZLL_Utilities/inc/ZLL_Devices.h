/******************************************************************************
* ZLL_Devices.h
*******************************************************************************
* Program Description:
*
*
******************************************************************************/
#ifndef _ZLL_DEVICES_H_
#define _ZLL_DEVICES_H_

/*****************************************************************************
* Includes
*****************************************************************************/

#include "ZLL_defines.h"
#include "ZLL_Configuration.h"


/*****************************************************************************
* Global Constants
*****************************************************************************/

/* There are 2 types of devices in ZLL profile
	- Controller Devices (controls lighting devices)
	- Lighting Devices*/

	/* -------Controller Devices ----------- */

#define g_COLOR_CONTROLLER_c					0x0800
#define g_COLOR_SCENE_CONTROLLER_c				0x0810
#define g_NON_COLOR_CONTROLLER_c				0x0820
#define g_NON_COLOR_SCENE_CONTROLLER_c			0x0830
#define g_CONTROL_BRIDGE_c						0x0840
#define g_ON_OFF_SENSOR_c						0x0850


	/* ----------- Lighting Devices -----------*/
#define g_ON_OFF_LIGHT_c						0x0000
#define g_ON_OFF_PLUG_IN_UNIT_c					0x0010
#define g_ZLL_DIMMABLE_LIGHT_c					0x0100
#define g_DIMMABLE_PLUG_IN_UNIT_c				0x0110
#define g_COLOR_LIGHT_c							0x0200
#define g_EXTENDED_COLOR_LIGHT_c				0x0210
#define g_COLOR_TEMPERATURE_LIGHT_c				0x0220


/* Mapping end Points to Controller Device Type*/
#ifdef g_ZLL_LIGHT_CONTROLLER_c
/* Note :Endpoint 0 is reserved for ZDO endpoint*/
#define g_COLOR_CONTROLLER_ENDPOINT_ID_c				(0x01)
#define g_COLOR_SCENE_CONTROLLER_ENDPOINT_ID_c			(0x02)
#define g_NON_COLOR_CONTROLLER_ENDPOINT_ID_c			(0x03)
#define g_NON_COLOR_SCENE_CONTROLLER_ENDPOINT_ID_c		(0x04)
#define g_CONTROL_BRIDGE_ENDPOINT_ID_c 					(0x05)
#define g_ON_OFF_SENSOR_ENDPOINT_ID_c					(0x06)
#endif /* g_ZLL_LIGHT_CONTROLLER_c*/

/* Mapping end Points to Controller Device Type*/
#ifdef g_ZLL_LIGHT_DEVICE_c

/* Note :Endpoint 0 is reserved for ZDO endpoint*/
#define g_ON_OFF_LIGHT_ENDPOINT_ID_c						(0x01)
#define g_ON_OFF_PLUG_IN_UNIT_ENDPOINT_ID_c					(0x02)
#define g_DIMMABLE_LIGHT_ENDPOINT_ID_c						(0x03)
#define g_DIMMABLE_PLUG_IN_UNIT_ENDPOINT_ID_c				(0x04)
#define g_COLOR_LIGHT_ENDPOINT_ID_c 						(0x05)
#define g_EXTENDED_COLOR_LIGHT_ENDPOINT_ID_c				(0x06)
#define g_COLOR_TEMPERATURE_LIGHT_ENDPOINT_ID_c				(0x07)

#endif /* g_ZLL_LIGHT_DEVICE_c*/





/*****************************************************************************
* Function Prototypes
*****************************************************************************/


#endif /*_ZLL_DEVICES_H_*/
/*****************************************************************************
* End Of File                                      _ZLL_DEVICES_H_
*****************************************************************************/
