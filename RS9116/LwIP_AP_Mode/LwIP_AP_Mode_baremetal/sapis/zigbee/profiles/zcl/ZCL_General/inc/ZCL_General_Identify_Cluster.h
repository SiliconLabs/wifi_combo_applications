/******************************************************************************
* ZCL_General_Identify_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Identify_Cluster attribute and cmmnd information
******************************************************************************/
#ifndef _ZCL_GENERAL_IDENTIFY_CLUSTER_H_
#define _ZCL_GENERAL_IDENTIFY_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/*Attributes and commands for putting a device into
Identification mode (e.g. flashing a light)*/
#define g_IDENTIFY_CLUSTER_c                                   0x0003

/*Attributes for Identify Cluster*/
#define g_IDENTIFY_TIME_ATTRIBUTE_c                            0x0000

/*enumerations for General Domain - cluster specific command events*/
typedef enum General_Domain_Events
{
   g_Identify_Event_c = 0x01,
   g_Identify_Query_Event_c,
   g_Trigger_Effect_Event_c = 0x51
}General_Domain_Events;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Identify_Server_Cluster_Info;
#endif

#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Identify_Client_Cluster_Info;
#endif
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/*enumerations for identify cluster commands*/
typedef enum Identify_Cluster_Command
{
   g_Identify_Command_c,
   g_Identify_Query_Command_c,
   g_Trigger_Effect_Command_c = 0x40
}Identify_Cluster_Command_t;

typedef enum Identify_Cluster_Response_Command
{
   g_Identify_Query_Response_Command_c
}Identify_Cluster_Response_Command;



typedef struct Identify_Event
{
    uint8_t a_identify_time[2];
}__attribute__((__packed__))Identify_Event;


typedef struct Identify_Cluster_tag
{
   uint8_t a_identify_time[2];
}__attribute__((__packed__))Identify_Cluster_t;


typedef struct General_Domain_Identify_Request
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union Indentify_Commands
   {
        Identify_Cluster_t identify_command;
   }Indentify_Commands;
}__attribute__((__packed__))General_Domain_Identify_Request;

/*Structures to be used by the Application
Field: identify_time - indicates the Time duration for which the device is
                        identifying*/
typedef struct Identify_Query_Response_Tag
{
   uint8_t a_identify_time[2];
}__attribute__((__packed__))Identify_Query_Response_t;
/*stucture for ZCL response commands
Field: Command Identifier - gives the command id
Field: Command type - indicates the generic command or cluster specific
                        command
Field: manufacture specific - indicates if manufacture specific is set
Field: manufacture code - manufacture code
Field: Actual payload   */
typedef struct ZCL_Identify_Cluster_Response_Command_Tag
{
   uint8_t command_identifier;
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacture_code[2];
   union
   {
      Identify_Query_Response_t identify_query_response;
   }ZCL_Response;
}__attribute__((__packed__))ZCL_Identify_Cluster_Response_Command_t;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* ZCL_Handle_Identify_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value :
*		unsigned char - returns success or error code
*
* Input Parameters:
*	p_cluster_data - pointer to cluster data
*	
* Output Parameters:
*	p_cluster_data - pointer to cluster data	
*
* Scope of the function:
*  This function is called within ZCL_Handle_Cluster_Specific_Cmd() function,
*  when a identify cluster command is received in the Data indication.
*
* Purpose of the function:
*	This function handles the identify cluster command received in the data
*  indication
*     	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Identify_Cluster_Cmd
(
     uint8_t endPointId,
     uint8_t asduLength,
     uint8_t* pReceivedAsdu,
     uint8_t* pResponseAsduStartLocation,
     ZCL_ActionHandler_t *pActionHandler,
     ZCL_Cluster_Info_t const *pZCLClusterInfo,
     uint8_t * pAttrsDataBaseLocation
);

#if ( g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
void ZCL_Callback_Identifier_Trigger_Effect
(
     ZCL_Event_t *pEvents
);
#endif /* #if ( g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )*/

#endif                                  /* g_IDENTIFY_CLUSTER_ENABLE_d == 1 */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_GENERAL_IDENTIFY_CLUSTER_H_*/
