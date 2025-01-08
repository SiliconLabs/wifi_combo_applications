#ifndef _ZCL_GENERAL_ON_OFF_SWITCH_CONFIG_H_
#define _ZCL_GENERAL_ON_OFF_SWITCH_CONFIG_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/*Attributes and commands for configuring On/Off
switching devices 0x0007*/
#if( g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1 )

#define g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_c                0x0007

/*Attributes for On Off Switch Configuration cluster*/
#define g_SWITCH_TYPE_ATTRIBUTE_c                              0x0000
#define g_SWITCH_ACTIONS_ATTRIBUTE_c                           0x0010

/*enumerations for Switch type*/
typedef enum Switch_Type
{
   g_Toggle_Switch_Type_c,
   g_Momentary_Switch_Type_c
#if(g_HA_1_2_d == 1) 	
   ,g_Multi_Function_Type_c
#endif	
}Switch_Type;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_ON_OFF_SWITCH_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_On_Off_Switch_Config_Server_Cluster_Info;
#endif

#if ( g_ON_OFF_SWITCH_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1)
extern const ZCL_Cluster_Info_t ga_On_Off_Switch_Config_Client_Cluster_Info;
#endif
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
On Off Switch Configuration Cluster
Field: switch_type -
Field: switch_action -
-----------------------------------------------------------------------------*/
typedef struct On_Off_Switch_Config_Cluster_Tag
{
   uint8_t switch_type;
   uint8_t switch_action;
}__attribute__((__packed__))On_Off_Switch_Config_Cluster_t;



/*-----------------------------------------------------------------------------
* Function Prototypes
--------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_OnOff_Switch_Config_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : Length
*		
* Input Parameters:
*	endPointId - Device End point
*   asduLength - Length of received asdu
*   pReceivedAsdu - Pointer to received asdu
*   pZCLClusterInfo - Pointer to On/Off Switch config cluster info
*   pAttrsDataBaseLocation - Pointer to On/Off Switch config cluster attributes storage
*
* Output Parameters:
*	pResponseAsduStartLocation - Pointer for response creation
*   pActionHandler - Pointer for Event Updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the On/Off Switch config cluster command received in the indication
*     	
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_OnOff_Switch_Config_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );


#endif                  /*( g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_GENERAL_ON_OFF_SWITCH_CONFIG_H_*/
