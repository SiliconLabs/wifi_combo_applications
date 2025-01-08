#ifndef _APPLEVELCONTROLUTILITIES_H_
#define _APPLEVELCONTROLUTILITIES_H_
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "sw_timer.h"
#include "ZCL_Functionality.h"
#include "ZCL_Interface.h"
#if( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Level_Control_Cluster.h"
#include "AppLevelControlUtilities.h"
#include "ApplicationThinLayer.h"
#include "ZCL_General_On_Off_Cluster.h"
#include "ApptimerId.h"
#include "ZCL_Foundation.h"
#include "ZCL_Common.h"

/*------------------------------------------------------------------------------------------------------------------
* Global Constants
*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------
* Type Definitions
*-------------------------------------------------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------------------------------------------
* Memory Declarations
*------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------------------------------------------*/

void Handle_Level_Control_Cluster_Events( ZCL_Event_t *pEvent, uint8_t endpoint );

#if(g_HA_1_2_d == 1) 	
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )

void App_Move_Current_Level_with_OnOff_command ( uint8_t events, uint8_t endpoint);
#endif
#endif
/* Call back declaration for fade to off in 0.8 seconds */
void App_ZLL_Level_Control_Fade_To_Off_In_0_8_Sec_Command_Call_Back ( uint8_t timer_id );
/* Call back declaration for 50 percent dim down in 08 seconds and fade to off in 12 seconds */
void App_ZLL_Level_Control_50_per_Dim_Fade_To_Off_In_12_Sec_Command_Call_Back ( uint8_t timer_id );
/* Call back declaration for 20 percent dim down  and fade to off in 1 second */
void App_ZLL_Level_Control_20per_Fade_To_Off_In_1_Sec_Command_Call_Back ( uint8_t timer_id );

#endif                     /* ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                      /* _APPLEVELCONTROLUTILITIES_H_ */