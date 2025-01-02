/******************************************************************************
* ZCL_General_On_Off_Cluster.h
******************************************************************************
* Program Description:
* This file contains the On-Off cluster attribute and cmmnd information
******************************************************************************/

#ifndef _ZCL_GENERAL_ON_OFF_CLUSTER_H_
#define _ZCL_GENERAL_ON_OFF_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Interface.h"
#if ( g_ON_OFF_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*On/off Attributes and commands for switching devices between
‘On’ and ‘Off’ states.0x0006*/
#define g_ON_OFF_CLUSTER_c                                     0x0006

/*Attributes for On Off cluster*/
#define g_ON_OFF_ATTRIBUTE_c                                   0x0000

#if(g_HA_1_2_d == 1)
#define g_MAX_DURATION_ATTRIBUTE_c							   0x0001
#define g_ON_DURATION_ATTRIBUTE_c							   0x0002
#endif


#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_GLOBAL_SCENE_CONTROL_ATTRIBUTE_c                     0x4000
#define g_ON_TIME_ATTRIBUTE_c                                  0x4001
#define g_OFF_WAIT_TIME_ATTRIBUTE_c                            0x4002
#endif /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 */


/*enumerations for General Domain - cluster specific command events*/
typedef enum General_Domain_Scenes_Events
{
   g_On_Off_Cluster_On_Event_c = 0x04
   ,g_On_Off_Cluster_Off_Event_c
   ,g_On_Off_Cluster_Toggle_Event_c
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
   ,g_Off_With_Effect_Event_c = 0x81
   ,g_On_With_Recall_Global_Scene_Event_c
   ,g_On_With_Timed_Off_Event_c
#endif  /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
#if(g_HA_1_2_d == 1)
   ,g_On_Off_Cluster_On_With_Duration_Event_c=0x1d
#endif
}General_Domain_Scenes_Events;

enum
{
   g_OFF_c,
   g_ON_c
};
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_On_Off_Server_Cluster_Info;
#endif

#if ( g_ON_OFF_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_On_Off_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef enum OnOff_Cluster
{
   g_Off_Command_c =0x00
   ,g_On_Command_c
   ,g_Toggle_Command_c
#if(g_HA_1_2_d == 1)
   ,g_On_With_Duration_command_c
#endif
#if ( g_ZLL_ON_OFF_CLUSTER_ENABLE_d == 1 )
   ,g_Off_With_Effect_Command_c = 0x40
   ,g_On_With_Recall_Global_Scene_Command_c
   ,g_On_With_Timed_Off_Command_c
#endif    /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/

}OnOff_Cluster;

/*attributes for on off cluster*/
typedef struct On_Off_Cluster_Tag
{
    uint8_t On_Off_Attribute;
#if(g_HA_1_2_d == 1)
    uint8_t max_duration[2];
	uint8_t on_duration[2];
#endif
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t Global_Scene_Control_Attribute;
    uint8_t On_Time_Attribute[2];
    uint8_t Off_Wait_Time_Attribute[2];
#endif   /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
}__attribute__((__packed__))On_Off_Cluster_t;

/* Structure to send basic commands */
typedef struct General_Domain_On_Off_Request
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
}__attribute__((__packed__))General_Domain_On_Off_Request;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* ZCL_Handle_OnOff_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : Length
*		
* Input Parameters:
*	endPointId - Device End point
*   asduLength - Length of received asdu
*   pReceivedAsdu - Pointer to received asdu
*   pZCLClusterInfo - Pointer to Level control cluster info
*   pAttrsDataBaseLocation - Pointer to On/Off cluster attributes storage
*
* Output Parameters:
*	pResponseAsduStartLocation - Pointer for response creation
*   pActionHandler - Pointer for Event Updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the On/Off cluster command received in the indication
*     	
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_OnOff_Cluster_Cmd
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);

void ZLLUtilOnOffHandler
(
    ZCL_Event_t *pEvents
);

#endif                                          /* g_ON_OFF_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                          /*_ZCL_GENERAL_ON_OFF_CLUSTER_H_*/
