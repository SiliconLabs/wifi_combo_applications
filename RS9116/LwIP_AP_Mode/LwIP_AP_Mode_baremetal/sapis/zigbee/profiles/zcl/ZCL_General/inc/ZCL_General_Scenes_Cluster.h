#ifndef _ZCL_GENERAL_SCENES_CLUSTER_H_
#define _ZCL_GENERAL_SCENES_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define g_SCENES_CLUSTER_c                                     0x0005

#define g_SCENE_MGMT_INFO_SCENE_COUNT_ATTRIBUTE_c              0x0000
#define g_SCENE_MGMT_INFO_CURRENT_SCENE_ATTRIBUTE_c            0x0001
#define g_SCENE_MGMT_INFO_CURRENT_GROUP_ATTRIBUTE_c            0x0002
#define g_SCENE_MGMT_INFO_SCENE_VALID_ATTRIBUTE_c              0x0003
#define g_SCENE_MGMT_INFO_NAME_SUPPORT_ATTRIBUTE_c             0x0004
#define g_SCENE_MGMT_INFO_LAST_CONFIGURED_BY_ATTRIBUTE_c       0x0005

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Scenes_Server_Cluster_Info;
#endif

#if ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Scenes_Client_Cluster_Info;
#endif
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef enum Scenes_Cluster
{
   g_Add_Scene_Command_c,
   g_View_Scene_Command_c,
   g_Remove_Scene_Command_c,
   g_Remove_All_Scenes_Command_c,
   g_Store_Scene_Command_c,
   g_Recall_Scene_Command_c,
   g_Scene_Membership_Command_c,
   g_Enhanded_Add_Scene_Command_c = 0x40,
   g_Enhanced_View_Scene_Command_c,
   g_Copy_Scene_Command_c
   
}Scenes_Cluster;

typedef enum Scenes_Cluster_Response_Command
{
   g_Add_Scene_Response_Command_c,
   g_View_Scene_Response_Command_c,
   g_Remove_Scene_Response_Command_c,
   g_Remove_All_Scenes_Response_Command_c,
   g_Store_Scene_Response_Command_c,
   g_Scene_Membership_Response_Command_c = 0x06,
   g_Enhanced_Add_Scene_Response_Command_c = 0x40,
   g_Enhanced_View_Scene_Response_Command_c,
   g_Copy_Scene_Response_Command_c
}Scenes_Cluster_Response_Command;


/*Event Ids for scene commands*/
typedef enum Scenes_Events_Command
{
   g_Add_Scene_Command_Event_c = 0x15,
   g_View_Scene_Command_Event_c,
   g_Remove_Scene_Command_Event_c,
   g_Remove_All_Scenes_Command_Event_c,
   g_Store_Scene_Command_Event_c,
   g_Recall_Scene_Command_Event_c,
   g_Scene_Membership_Command_Event_c,
   g_Enhanced_Add_Scene_Command_Event_c = 0x40,
   g_Enhanced_View_Scene_Command_Event_c,
   g_Copy_Scene_Command_Event_c
}Scenes_Events_Command;

/*-----------------------------------------------------------------------------
Scenes Cluster structure - attributes
Field: scene_count - specifies the number of scenes currently in the device’s
         scene table.
Field: current_scene - The CurrentScene attribute holds the Scene ID of the
         scene last invoked.
Filed: current_group - The CurrentGroup attribute holds the Group ID of the
         scene last invoked, or 0x0000 if the scene last invoked is not
         associated with a group.
Field: scene_valid - The SceneValid attribute indicates whether the state of
         the device corresponds to the values of the CurrentScene and
         CurrentGroup attributes.
Field: name_support - The most significant bit of the NameSupport attribute
         indicates whether or not group names are supported.
Field: last_configured_by - specifies the IEEE address of the device that last
         configured the scene table.
-----------------------------------------------------------------------------*/
typedef struct Scenes_Cluster_Tag
{
   uint8_t scene_count;
   uint8_t current_scene;
   uint8_t a_current_group[2];
   uint8_t scene_valid;
   uint8_t name_support;
   uint8_t last_configured_by[8];
}__attribute__((__packed__))Scenes_Cluster_t;

/*-----------------------------------------------------------------------------
Copy Scene Command Payload structure -attributes
Field : mode-contais information of how the scene copy is to be done
Field : groupId_from -specifies the identifier of the group from which the scene ids to be copied
Field : sceneid_from - specifies the identifier of the scene from which the scene is to be copied
Field : groupId_to - specifies the identifier of the group to which the scene ids to be copied
Field : sceneid_to- specifies the identifier of the scene to which the scene is to be copied

---------------------------------------------------------------------------------------------------*/

typedef struct CopySceneCmdPayload_Tag{
	uint8_t mode;
	uint16_t groupId_from;
	uint8_t sceneid_from;
	uint16_t groupId_to;
	uint8_t sceneid_to;
}__attribute__((__packed__))CopySceneCmdPayload_t;


/*-------------------------------------------------------------------------------
* Function Prototypes
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
* ZCL_Handle_Scenes_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value :
*		unsigned char - returns length of response
*
* Input Parameters:
*	endPointId - pointer to cluster data	
*   asduLength - Data indication asdu length
*   pReceivedAsdu - Received asdu
*   pZCLClusterInfo - Received cluster info
*   pAttrsDataBaseLocation - cluster attributes data base location
*
* Output Parameters:
*	pResponseAsduStartLocation - ZCL Response start location
*   pActionHandler - action updated by cluster handing function
*
* Scope of the function:
*  This function is called within ZCL_Handle_Cluster_Specific_Cmd function,
*  when a Scenes cluster command is received in the Data indication.
*
* Purpose of the function:
*	This function handles the scenes cluster command received in the data
*  indication
*     	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Scenes_Cluster_Cmd
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
#endif                                 /*_ZCL_GENERAL_SCENES_CLUSTER_H_*/
