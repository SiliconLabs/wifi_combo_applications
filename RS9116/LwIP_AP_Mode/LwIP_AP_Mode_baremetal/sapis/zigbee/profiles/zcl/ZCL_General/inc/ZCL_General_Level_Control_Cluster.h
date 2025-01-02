/******************************************************************************
* ZCL_General_Level_Control_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Level Control cluster attribute and cmmnd information
******************************************************************************/
#ifndef _ZCL_GENERAL_LEVEL_CONTROL_CLUSTER_H_
#define _ZCL_GENERAL_LEVEL_CONTROL_CLUSTER_H_

#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"

#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*Attributes and commands for controlling devices that
can be set to a level between fully �On� and fully �Off�.0x0008*/
#define g_LEVEL_CONTROL_CLUSTER_c                              0x0008

/*Attributes for Level Control Cluster*/
#define g_CURRENT_LEVEL_ATTRIBUTE_c                            0x0000
#define g_LEVEL_REMAINING_TIME_ATTRIBUTE_c                     0x0001
#define g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_c                   0x0010
#define g_ON_LEVEL_ATTRIBUTE_c                                 0x0011

#if(g_HA_1_2_d == 1) 	
#define g_ON_TRANSITION_TIME_ATTRIBUTE_c                       0x0012
#define g_OFF_TRANSITION_TIME_ATTRIBUTE_c                      0x0013
#define g_DEFAULT_MOVE_RATE_ATTRIBUTE_c                        0x0014
#endif

/*enumerations for Level control cluster for moving up or down*/
enum
{
   g_Move_Up_c,
   g_Move_Down_c
};

// uint8_t offset = 3;
/*enumerations for General Domain - cluster specific command events*/
typedef enum General_Domain_Level_Control_Events
{
   g_Move_To_Level_Event_c = 0x07,
   g_Move_Event_c,
   g_Step_Event_c,
   g_Stop_Event_c,
   g_Move_To_Level_With_On_Off_Event_c,
   g_Move_With_On_Off_Event_c,
   g_Step_With_On_Off_Event_c,
   g_Stop_With_On_Off_Event_c
}General_Domain_Level_Control_Events;    /*Level_Control_Events*/

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Level_Control_Server_Cluster_Info;
#endif

#if ( g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Level_Control_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef enum Level_Control_Cluster
{
   g_Move_To_Level_Command_c,
   g_Move_Command_c,
   g_Step_Command_c,
   g_Stop_Command_c,
   g_Move_To_Level_With_On_Off_Command_c,
   g_Move_With_On_Off_Command_c,
   g_Step_With_On_Off_Command_c,
   g_Stop_With_On_Off_Command_c
}Level_Control_Cluster;


/*-----------------------------------------------------------------------------
Level Control Cluster structure - attributes
Field: current_level - current level of this device.
Field: remaining_time - the time remaining until the current command is
         complete
Filed: on_off_transition_time - the time taken to move to or from
         the target level when On of Off commands are received by an On/Off
         cluster on the same endpoint.
Field: on_level - the value that the CurrentLevel attribute is set to
         when the OnOff attribute of an On/Off cluster on the same endpoint
         is set to On.
-----------------------------------------------------------------------------*/
typedef struct Level_Control_Cluster_Attributes_Tag
{
    uint8_t current_level;
#if( g_LEVEL_REMAINING_TIME_ATTRIBUTE_ENABLE_d == 1)
    uint8_t a_remaining_time[2];
#endif              /*g_LEVEL_REMAINING_TIME_ATTRIBUTE_c*/
#if(g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
    uint8_t a_on_off_transition_time[2];
#endif      /*g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d*/

#if(g_ON_LEVEL_ATTRIBUTE_ENABLE_d == 1)
    uint8_t on_level;
#endif /*g_ON_LEVEL_ATTRIBUTE_ENABLE_d*/
	
#if(g_HA_1_2_d == 1)	
#if(g_ON_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
    uint8_t a_on_transition_time[2];
#endif      /*g_ON_TRANSITION_TIME_ATTRIBUTE_ENABLE_d*/

#if(g_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
    uint8_t a_off_transition_time[2];
#endif      /*g_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d*/

#if(g_DEFAULT_MOVE_RATE_ATTRIBUTE_ENABLE_d == 1)
    uint8_t a_default_move_rate[2];
#endif      /*g_DEFAULT_MOVE_RATE_ATTRIBUTE_ENABLE_d*/

#endif	
	
}__attribute__((__packed__))Level_Control_Cluster_Attributes_t;

typedef struct Level_Control_Move_Command_Tag
{
   uint8_t move_mode;
   uint8_t move_rate;
}__attribute__((__packed__))Level_Control_Move_Command_t;

/* Format of the Step Command Payload */
typedef struct Level_Control_Step_Command_Tag
{
   uint8_t step_mode;
   uint8_t step_size;
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Level_Control_Step_Command_t;

/* Format of the Move to Level Command Payload */
typedef struct Level_Control_Move_To_Level_Command_Tag
{
   uint8_t level;
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Level_Control_Move_To_Level_Command;


typedef struct General_Domain_Level_Control_Request
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union Level_Control_Commands
   {
      Level_Control_Move_Command_t  level_control_move_command;
      Level_Control_Step_Command_t  level_control_step_command;
      Level_Control_Move_To_Level_Command level_control_move_to_level_command;
   }Level_Control_Commands;
}__attribute__((__packed__))General_Domain_Level_Control_Request;

typedef struct General_Domain_Level_Control_event_tag
{
   uint8_t event_id;
   uint8_t event_length;
   union Level_Control_Commands_event
   {
      Level_Control_Move_Command_t  level_control_move_command;
      Level_Control_Step_Command_t  level_control_step_command;
      Level_Control_Move_To_Level_Command level_control_move_to_level_command;
   }Level_Control_Commands;
}__attribute__((__packed__))General_Domain_Level_Control_event_t;


typedef struct Level_Control_Cluster_Tag
{
  uint8_t current_level;
#if(g_LEVEL_REMAINING_TIME_ATTRIBUTE_ENABLE_d == 1)
  uint8_t a_remaining_time[2];
#endif
#if(g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
  uint8_t a_on_off_transition_time[2];
#endif
#if (g_ON_LEVEL_ATTRIBUTE_ENABLE_d == 1)
  uint8_t on_level;
#endif
#if(g_HA_1_2_d == 1)
#if (g_ON_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
  uint8_t a_on_transition_time[2];

#endif
#if (g_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)

  uint8_t a_off_transition_time[2];
#endif
#if (g_DEFAULT_MOVE_RATE_ATTRIBUTE_ENABLE_d == 1 )
  uint8_t a_default_move_rate[2];
#endif
#endif/*g_HA_1_2_d*/
}__attribute__((__packed__))Level_Control_Cluster_t;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_Level_Control_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : Length
*		
* Input Parameters:
*	endPointId - Device End point
*   asduLength - Length of received asdu
*   pReceivedAsdu - Pointer to received asdu
*   pZCLClusterInfo - Pointer to Level control cluster info
*   pAttrsDataBaseLocation - Pointer to Level Control cluster attributes storage
*
* Output Parameters:
*	pResponseAsduStartLocation - Pointer for response creation
*   pActionHandler - Pointer for Event Updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the Level Control cluster command received in the indication
*     	
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Level_Control_Cluster_Cmd
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);
#endif          /* ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_GENERAL_LEVEL_CONTROL_CLUSTER_H_*/
