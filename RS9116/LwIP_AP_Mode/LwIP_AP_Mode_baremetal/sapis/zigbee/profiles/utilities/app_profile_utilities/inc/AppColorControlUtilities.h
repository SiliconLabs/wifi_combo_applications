#ifndef _APPCOLORCONTROLUTILITIES_H_
#define _APPCOLORCONTROLUTILITIES_H_
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "buffer_management.h"
#include "memory_utility.h"
//#include "sw_timer.h"
#include "ZCL_Functionality.h"
#include "ZCL_Interface.h"
#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
#include "AppColorControlUtilities.h"

#include "ZCL_General_On_Off_Cluster.h"
#include "ZCL_Foundation.h"
#include "ZCL_Common.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_MIN_VALUE_c                                           0x00
#define m_MAXIMUM_VALUE_c                                       0xFFFF
#define m_MAX_VALUE_c                                           0xFF
#define m_MINIMUM_VALUE_c                                       0x0000
#define m_CHANGE_UP_BY_ONE_c                                    1
#define m_CHANGE_DOWN_BY_ONE_c                                  -1
#define m_IGNORE_DATA_c                                         0xFF
#define m_TRANSITION_RATE_c                                     0x0A
#define g_COLOR_CONTROL_TIME_OUT_DURATION_c                     0x03E8
#define UPDATE_ACTION 											0x01
#define UPDATE_DIRECTION 										0x02
#define UPDATE_TIME 											0x04
#define UPDATE_START_HUE 										0x08
#define ACTIVE_MODE_1											0x01
#define ACTIVE_MODE_2											0x02


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

void App_Handle_Color_Control_Cluster_Events
(
	ZCL_Event_t *pActionHandler,
	uint8_t endpoint
);

void App_Move_Hue_Call_Back
(
   uint8_t timer_id
);


void App_Move_Saturation_Call_Back
(
   uint8_t timer_id
);

void App_Move_To_Hue_Call_Back
(
   uint8_t timer_id
);

void App_Move_Hue_And_Saturation_Call_Back
(
  uint8_t timer_id
);
void App_Move_To_Color_Call_Back
(
  uint8_t timer_id
);
void App_Move_Color_Call_Back
(
  uint8_t timer_id
);
void App_Step_Color_Call_Back
(
  uint8_t timer_id
);
void App_Move_To_Color_temperature_Call_Back
(
  uint8_t timer_id
);
void App_Step_Saturation_Call_Back
(
  uint8_t timer_id
);
void App_Step_Hue_Call_Back
(
  uint8_t timer_id
);
void App_Move_To_Saturation_Call_Back
(
  uint8_t timer_id
);
#if ( g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
void App_Enhanced_Move_To_Hue_Call_Back
(
   uint8_t timer_id
);
void App_Enhanced_Move_Hue_Call_Back
(
   uint8_t timer_id
);
void App_Enhanced_Step_Hue_Call_Back
(
   uint8_t timer_id
);
void App_Enhanced_Move_To_Hue_And_Saturation_Call_Back
(
   uint8_t timer_id
);
void App_Color_Loop_Set_Call_Back
(
   uint8_t timer_id
);
void App_Move_Color_Temperature_Call_Back
(
   uint8_t timer_id
);
void App_Step_Color_Temperature_Call_Back
(
   uint8_t timer_id
);
#endif  /*g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d*/

#endif                     /* ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif    /* _APPCOLORCONTROLUTILITIES_H_ */
