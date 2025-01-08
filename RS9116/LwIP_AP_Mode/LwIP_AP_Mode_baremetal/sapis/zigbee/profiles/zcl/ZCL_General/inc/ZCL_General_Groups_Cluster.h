/******************************************************************************
* ZCL_General_Groups_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Groups_Cluster attribute and cmmnd information
******************************************************************************/
#ifndef _ZCL_GENERAL_GROUPS_CLUSTER_H_
#define _ZCL_GENERAL_GROUPS_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#if(g_GROUPS_CLUSTER_ENABLE_d == 1)


/*Attributes and commands for group configuration and
manipulation.0x0004*/
#define g_GROUPS_CLUSTER_c                                          0x0004

/*Attributes for group cluster*/
#define g_GROUP_ATTRIBUTE_NAME_SUPPORT_ATTRIBUTE_c                  0x0000

/*attribute for groups cluster*/
typedef struct Group_Cluster_Tag
{
    uint8_t Name_Support;
}__attribute__((__packed__))Group_Cluster_t;
/*enumerations for General Domain - cluster specific command events*/
typedef enum General_Domain_Groups_Events
{
   g_Add_Group_Command_Events_c = 0x0F,
   g_View_Group_Command_Events_c,
   g_Get_Group_Membership_Command_Events_c,
   g_Remove_Group_Command_Events_c,
   g_Remove_All_Groups_Command_Events_c,
   g_Add_Group_If_Identifying_Command_Events_c
}General_Domain_Groups_Events;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Group_Server_Cluster_Info;
#endif

#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Group_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef enum Group_Cluster
{
   g_Add_Group_Command_c,
   g_View_Group_Command_c,
   g_Get_Group_Membership_Command_c,
   g_Remove_Group_Command_c,
   g_Remove_All_Groups_Command_c,
   g_Add_Group_If_Identifying_Command_c
}Group_Cluster;

typedef enum Group_Cluster_Response
{
   g_Add_Group_Response_c,
   g_View_Group_Response_c,
   g_Get_Group_Membership_Reponse_c,
   g_Remove_Group_Response_c,
   g_Remove_All_Groups_Response_c
}Group_Cluster_Response;


/* Structures for group commands */
typedef struct Add_Group_Command
{
   uint8_t a_group_id[2];
   uint8_t group_name;
}__attribute__((__packed__))Add_Group_Command;

typedef struct View_Group_Command
{
   uint8_t a_group_id[2];
}__attribute__((__packed__))View_Group_Command;

typedef struct Get_Group_Membership_Command
{
   uint8_t group_count;
   uint8_t a_group_list[32];
}__attribute__((__packed__))Get_Group_Membership_Command;

typedef struct Remove_Group_Command
{
   uint8_t a_group_id[2];
}__attribute__((__packed__))Remove_Group_Command;

typedef struct Add_Group_If_Identifying_Command
{
   uint8_t a_group_id[2];
   uint8_t a_group_name[16];
}__attribute__((__packed__))Add_Group_If_Identifying_Command;

typedef struct Groups_Event
{
   uint8_t event_id;
   uint8_t event_length;
   union groups_event_data
   {
      Add_Group_Command add_group_command_data;
      View_Group_Command view_group_commmand_data;
      Get_Group_Membership_Command get_group_command_data;
      Remove_Group_Command remove_group_command_data;
      Add_Group_If_Identifying_Command add_group_if_identifying_command_data;
   }groups_event_data;
}__attribute__((__packed__))Groups_Event;

typedef struct General_Domain_Groups_Request
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union Groups_Commands
   {
      Add_Group_Command add_group_command;
      View_Group_Command view_group_commmand;
      Get_Group_Membership_Command get_group_command;
      Remove_Group_Command remove_group_command;
      Add_Group_If_Identifying_Command add_group_if_identifying_command;
   }Groups_Commands;
}__attribute__((__packed__))General_Domain_Groups_Request;



/* Structures for group commands */
typedef struct Add_Group_Response_Tag
{
   uint8_t status;
   uint8_t a_group_id[2];
}__attribute__((__packed__))Add_Group_Response_t;

typedef struct View_Group_Response_Tag
{
   uint8_t status;
   uint8_t a_group_id[2];
   uint8_t group_name[16];
}__attribute__((__packed__))View_Group_Response_t;

typedef struct Get_Group_Membership_Response_Tag
{
   uint8_t capacity;
   uint8_t group_count;
   uint8_t a_group_list[32];
}__attribute__((__packed__))Get_Group_Membership_Response_t;

typedef struct Remove_Group_Response_Tag
{
   uint8_t status;
   uint8_t a_group_id[2];
}__attribute__((__packed__))Remove_Group_Response_t;


/*stucture for ZCL response commands
Field: Command Identifier - gives the command id
Field: Command type - indicates the generic command or cluster specific
   command
Field: manufacture specific - indicates if manufacture specific is set
Field: manufacture code - manufacture code
Field: Actual payload   */
typedef struct ZCL_Groups_Cluster_Response_Command_Tag
{
   uint8_t command_identifier;
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacture_code[2];
   union
   {
      Add_Group_Response_t add_group_response;
      View_Group_Response_t view_group_response;
      Get_Group_Membership_Response_t get_group_membership_response;
      Remove_Group_Response_t remove_group_response;
   }ZCL_Response;
}__attribute__((__packed__))ZCL_Groups_Cluster_Response_Command_t;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_Groups_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : Length
*		
* Input Parameters:
*	endPointId - Device End point
*   asduLength - Length of received asdu
*   pReceivedAsdu - Pointer to received asdu
*   pZCLClusterInfo - Pointer to Level control cluster info
*   pAttrsDataBaseLocation - Pointer to Groups cluster attributes storage
*
* Output Parameters:
*	pResponseAsduStartLocation - Pointer for response creation
*   pActionHandler - Pointer for event Updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the Groups cluster command received in the indication
*     	
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Groups_Cluster_Cmd
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_GENERAL_GROUPS_CLUSTER_H_*/

#endif                                  /* _ZCL_GENERAL_GROUPS_CLUSTER_H_ */
