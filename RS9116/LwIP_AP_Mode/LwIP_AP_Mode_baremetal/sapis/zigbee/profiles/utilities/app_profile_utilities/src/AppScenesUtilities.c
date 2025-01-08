/*******************************************************************************
* AppScenesUtilities.c
********************************************************************************
* Program Description:
*   This module provides appliction level scenes cluster functinalities
*
*******************************************************************************/

/********************************************************************************
* Includes
*********************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "buffer_management.h"
#include "ZCL_Functionality.h"
#include "ZCL_Interface.h"
#include "stack_common.h"
#include "AppTimerUtilities.h"
#include "ApplicationThinLayer.h"
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
#include "AppScenesUtilities.h"
#include "ZCL_General_Scenes_Cluster.h"
#include "ZCL_Stack_Utility.h"
//#include "ApptimerId.h"
//#include "sw_timer.h"

/********************************************************************************
* Global Constants
*********************************************************************************/
#define m_NULL_GROUP_ADDRESS_c                              0x0000
#define m_SIZE_OF_GROUP_ID_c                                0x02
#define m_SIZE_OF_SCENE_ID_c                                0x01
#define m_SIZE_OF_STATUS_c                                  0x01
#define m_LENGTH_OF_TNS_TIME_c                              0x02
#define m_INVALID_GROUP_ID_c                                0xFFFF
#define m_INVALID_CLUSTER_ID_c                              0xFFFF
#define m_IS_NAMES_SUPPORTED_c                              0x01
#define m_INVALID_SCENE_ID_c                                0xFF



#define PutSceneTableEntry( x, y )  memUtils_memCopy ( (uint8_t *)x, (uint8_t *)y, sizeof(App_ScenesRecord_t))
#define GetSceneTableEntry( x, y )  memUtils_memCopy ( (uint8_t *)x, (uint8_t *)y, sizeof(App_ScenesRecord_t))

/********************************************************************************
* Public Memory declarations
*********************************************************************************/
//extern uint8_t g_Number_Of_End_Points_c;
//uint8_t g_Number_Of_End_Points_c;// = g_MAX_NO_OF_ENDPOINTS_c;
uint8_t g_Number_Of_End_Points_c = 0x4;

extern const ZCL_Device_Def_t AppDeviceListServerInfo[];
static uint32_t lastTimerTick = 0x00;
App_ScenesRecord_t App_ScenesTable[g_MAX_SCENES_c];
uint8_t sceneTableIndexArray[g_MAX_SCENES_c];
Scenes_RecallCmdRecord_t scenesRecallCmdRecord = {0x01};
Scene_ValidInfo_t sceneValidInfo = { 0x00 };

/********************************************************************************
* Private Memory declarations
*********************************************************************************/

/* None */

/********************************************************************************
* Function Prototypes
*********************************************************************************/

static void Scenes_UpdateSceneValidFields ( uint8_t sceneTableIndex,
                                            uint8_t endPointId,
 	                                        uint8_t isSceneValid );

static uint8_t Update_AndGetSceneCount ( uint8_t endPointId, uint8_t direction );

static uint8_t Get_SceneTableEntry ( uint16_t groupId, uint8_t index );

static void App_Handle_ValidScenesValuesChanges( void );

#endif  /* ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 ) */
/********************************************************************************
* Public Functions
*********************************************************************************/

void App_ScenesTableInit( void )
{
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    memUtils_memSet( (uint8_t*)&App_ScenesTable, g_INVALID_VALUE_c,
              sizeof( App_ScenesRecord_t)* g_MAX_SCENES_c );

    memUtils_memSet( (uint8_t*)&sceneValidInfo, g_ZERO_c, sizeof(Scene_ValidInfo_t) );
    memUtils_memSet( (uint8_t*)&scenesRecallCmdRecord, g_ZERO_c, sizeof(Scenes_RecallCmdRecord_t) );
    scenesRecallCmdRecord.txnCount = 0x01;
    memUtils_memSet( (uint8_t*)&sceneValidInfo, g_ZERO_c, sizeof(Scene_ValidInfo_t) );
    lastTimerTick = App_getcurrentTick();
#endif
}

/******************************************************************************************/
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
void AppScenesTask( void )
{
    if ( App_getDifferance( App_getcurrentTick(), lastTimerTick ) >= g_ONE_TICK_c ) {
        lastTimerTick = App_getcurrentTick();
        if ( scenesRecallCmdRecord.sceneTxnTime == g_TRUE_c ) {
            if ( --scenesRecallCmdRecord.sceneTxnTimeout ) {
                scenesRecallCmdRecord.txnCount++;
                Handle_StoreAndRecallSceneCmd ( scenesRecallCmdRecord.endPoint,
                                                scenesRecallCmdRecord.sceneTableIndex,
                                                RecallScene );
            }
            else {
                scenesRecallCmdRecord.sceneTxnTime = g_FALSE_c;
            }
        }
    }
    App_Handle_ValidScenesValuesChanges();
}
/******************************************************************************************/

App_ScenesRecord_t Handle_StoreAndRecallSceneCmd ( uint8_t endPointId, uint8_t sceneTableIndex,
                                        uint8_t direction  )
{
	App_Cluster_Def_t* pZCLClusterDefinition;
	uint8_t attrLength = g_ZERO_c;
	uint8_t isAnalog = g_ZERO_c;
	uint8_t extFieldLength = g_ZERO_c;
	uint16_t attributeId;
	uint32_t attrData = g_ZERO_c;
	App_ScenesRecord_t appSceneRecord;
	uint8_t i;
	uint8_t j;
    uint8_t status;

	/* Get the scene entry from the scenes table */
	GetSceneTableEntry	( &appSceneRecord,
		App_ScenesTable + (uint16_t)(sceneTableIndex) * (uint16_t)sizeof(App_ScenesRecord_t));

	/* Loop through all cluster which has extension fields */
	for ( i = 0; i < g_MAX_EXT_FIELD_RECORDS_c; i++ ){
        status = App_Get_Cluster_Definition( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
                                             endPointId,
                                             appSceneRecord.extFieldRecord[i].clusterId,
                                             &pZCLClusterDefinition );
		/* check if it is a valid cluster id or not */
		if ( status == g_ZCL_Failure_c ){
			break;
		}
	    /* If the ClusterId is not supported by the device, then no operation should
        be performed for this cluster. But we should continue checking to next
        extension fields, if any */
        /* Make the ext fiel to zero before looping through the attribute table */
        extFieldLength = g_ZERO_c;

        /* Loop through all the attributes in the attribute table for this cluster id */
        for ( j = 0; ( ( j < pZCLClusterDefinition->p_cluster_info->num_attrib_info_present) &&
             (extFieldLength < (appSceneRecord.extFieldRecord[i].attributesLength ) ) ); j++ ) {

            attributeId = pZCLClusterDefinition->p_cluster_info->p_array_of_attrib_info[j].a_attribute_id;

            /* Check if the attribute has scenes extension */
            if ( ( pZCLClusterDefinition->p_cluster_info->
                  p_array_of_attrib_info[j].access_permission) & g_Scenes_Ext_Attribute_c ) {

                /* Get the attribute type ( analog or digital )*/
                isAnalog = Is_AttributeAnalogDataType( pZCLClusterDefinition->
                                  p_cluster_info->p_array_of_attrib_info[j].data_type );

                attrLength =  ZCL_Get_Data_length( pZCLClusterDefinition->p_cluster_info->
                                                   p_array_of_attrib_info[j].data_type );

                /* Check if direction is recall scene command */
                if ( RecallScene == direction ) {
                    /* We are making assumption that any attribute which supports scnenes extension,
                       the maximum value will be 4 byte only */
                    attrData = 0x00000000;

                    memUtils_memCopy ( (uint8_t*)&attrData, &(appSceneRecord.extFieldRecord[i].
                                               attributeData[extFieldLength] ), attrLength );

                    /* Check if attribute is analog or discrete. If discrete, put the given vlaue immidiately,
                        otherwise store the total value in transition time */
                    if ( isAnalog ) {
                        /* Check if timer is expired. If so, store the value immidiately.
                           Otherwise store in parts */
                        if ( (scenesRecallCmdRecord.sceneTxnTime) &&
                             (appSceneRecord.sceneTransitionTime) ) {
                            /*  check if the variable is signed integer and its value is negative. If so, store only
                                the negative offset in transition time */
                            if ( (IsSignedInteger( pZCLClusterDefinition->p_cluster_info->
                                                   p_array_of_attrib_info[j].data_type ) )&&
                                ( attrData & (uint16_t)1 << ( (attrLength * 8 )-1) ) ) {

                                /* Calculate the offset data to be stored */
                                attrData =  ((attrData ^ ( uint16_t )1 << (attrLength * 8 )-1) *
                                    scenesRecallCmdRecord.txnCount ) / appSceneRecord.sceneTransitionTime ;

                                attrData |= ( uint16_t )1 << ( (attrLength * 8 )-1) ;

                            }
                            else {
                                /* Calculate the offset data to be stored */
                                attrData = ( attrData * scenesRecallCmdRecord.txnCount ) /
                                                appSceneRecord.sceneTransitionTime;
                            }
                        }
                    }
                    /* Write the digital attributes only first time ( when count is zero)
                       but analog attributes will be written whenever this function will be
                       called with recall scene direction*/
                    if ( ( scenesRecallCmdRecord.txnCount == 0x01 ) || (isAnalog ) ) {
                        App_ReadWriteAttribute ( endPointId,
                                                pZCLClusterDefinition->p_cluster_info->cluster_id,
                                                attributeId,
                                                (uint8_t*)&attrData,
                                                g_Write_Attribute_Data_c );
                    }
                    /* Update the ext field length also */
                    extFieldLength += attrLength;
                }
                else {
                    /* Read the attribute in the local variable */
                     App_ReadWriteAttribute ( endPointId,
                                            pZCLClusterDefinition->p_cluster_info->cluster_id,
                                            attributeId,
                                            (uint8_t*)&attrData,
                                            g_Read_Attribute_Data_c );

                    /* check if the command is store scene. If so, write the attribute value in the scenes table */
                    if ( StoreScene == direction ) {

                        memUtils_memCopy (
                            &( appSceneRecord.extFieldRecord[i].attributeData[extFieldLength] ),
                            ( uint8_t*)&attrData, attrLength  );

                        extFieldLength += attrLength;
                        appSceneRecord.extFieldRecord[i].attributesLength = extFieldLength;
                    }
                    /* Check if any attribute value is changed or not after the successful store
                       or recall scene command */
                    else if ( CheckSceneValidity == direction ) {
                        /* Check if the attribute is analog and the transition timer is not expired yet.
                           If attribute is analog, dont comare the attribute,because attribute will updated
                           fully when the transition timer will expire*/
                        if ( !( isAnalog && scenesRecallCmdRecord.sceneTxnTime ) ) {
                            /* Compare the attribute value. If it is changed, then make the scene valid attribute
                               and flag to false */
                            if ( g_MEMORY_NO_MATCH_c ==( memUtils_memCompare(
                                 &( appSceneRecord.extFieldRecord[i].attributeData[extFieldLength] ),
                                  ( uint8_t*)&attrData, attrLength ) ) ) {
                                /* Make the scene valid flag to false */
                                Scenes_UpdateSceneValidFields ( sceneTableIndex,
                                                                endPointId, g_FALSE_c );
                            }
                        }
                        extFieldLength += attrLength;
                    }
                }
            }
        }
	}
	return 	appSceneRecord;
}

/******************************************************************************************/

static void Scenes_UpdateSceneValidFields ( uint8_t sceneTableIndex, uint8_t endPointId,
 	                                uint8_t isSceneValid )
{

	App_ScenesRecord_t appSceneRecord;

	/* Get the scene entry from the scenes table */
	GetSceneTableEntry( &appSceneRecord,
		App_ScenesTable + (uint16_t)(sceneTableIndex) * (uint16_t)sizeof(App_ScenesRecord_t) );

	/* Fill the scene vaild info structure fields */
	sceneValidInfo.sceneValid = isSceneValid;
	sceneValidInfo.endPoint = endPointId;
	sceneValidInfo.sceneTableIndex = sceneTableIndex;

	/* Update the current scene and current group only if scne is valid */
	if ( isSceneValid){

        App_ReadWriteAttribute (  endPointId,
                                        g_SCENES_CLUSTER_c,
                                        g_SCENE_MGMT_INFO_CURRENT_GROUP_ATTRIBUTE_c,
                                        (uint8_t*)&(appSceneRecord.groupId),
                                        g_Write_Attribute_Data_c );

        App_ReadWriteAttribute (  endPointId,
                                        g_SCENES_CLUSTER_c,
                                        g_SCENE_MGMT_INFO_CURRENT_SCENE_ATTRIBUTE_c,
                                        (uint8_t*)&(appSceneRecord.sceneId),
                                        g_Write_Attribute_Data_c );

    }
	/* Make the scene valid attribute to true */
    App_ReadWriteAttribute (  endPointId,
                                    g_SCENES_CLUSTER_c,
                                    g_SCENE_MGMT_INFO_SCENE_VALID_ATTRIBUTE_c,
                                    &(sceneValidInfo.sceneValid),
                                    g_Write_Attribute_Data_c );
}

/******************************************************************************************/

void ZCL_Callback_DeleteScenesTableEntry ( uint8_t endPointId, uint16_t groupId )
{
	uint8_t i;
	uint8_t sceneId;
	App_ScenesRecord_t appSceneRecord;

	/* Set the scene table variable with all FF's */
	memUtils_memSet ( (uint8_t*)&appSceneRecord, 0xFF, sizeof(App_ScenesRecord_t) );

	/* Delete all the entries in the scenes table for this group id, if any */
	for ( i = 0 ; i < g_MAX_SCENES_c; i++ ){

		sceneId = Get_SceneTableEntry ( groupId, i );

		if ( 0xFF != sceneId ){

			/* Check if the scene is valid for the entry whch has to be removed.
	    				   If so, make the scene invalid */
 			if ( (sceneValidInfo.sceneValid) && (sceneValidInfo.sceneTableIndex == i) )	{
 				Scenes_UpdateSceneValidFields ( i, endPointId, g_FALSE_c );
 			}
			/* update the scene table */
			PutSceneTableEntry (
				App_ScenesTable + (uint16_t)(i) * (uint16_t)sizeof(App_ScenesRecord_t),
				&appSceneRecord );

			/* Decrement the scenes count also */
			Update_AndGetSceneCount( endPointId, Decrement_SceneCount );
		}
	}
}



/******************************************************************************************/

void Update_sceneExtnWithCurrentAttrb ( uint8_t endPointId,
	                            App_ScenesRecord_t* pAppSceneRecord )
{
//#if 0
	App_Cluster_Def_t* pZCLClusterDefinition;
	uint8_t extFieldLength = g_ZERO_c;
	uint8_t attrLength = g_ZERO_c;
	uint8_t extFieldRecordIndex = g_ZERO_c;
	ZCL_Device_Def_t *pDeviceInfo = g_NULL_c;
	uint16_t attributeId;
	uint32_t attrData = g_ZERO_c;
	uint8_t i;
	uint8_t j;
	uint8_t updateExtFieldIndex;

	/* Get the device server info for the received endpoint */
	for ( i = 0; i < g_Number_Of_End_Points_c; i++ ){
		pDeviceInfo = (ZCL_Device_Def_t *)&AppDeviceListServerInfo[ i ];
		/* Come out of loop if end point matched with the requested endpoint */
		if ( pDeviceInfo->endpoint == endPointId ){
			break;
		}
	}
	/* Loop through all cluster which has extension fields */
	for ( i = 0; i < pDeviceInfo->num_clust_defs_present; i++ ) {

		/* Check if extension field index is less than three. Bacause currently we
           are supporting extension fields for max three clusters in scene table */
		if ( extFieldRecordIndex >= g_MAX_EXT_FIELD_RECORDS_c ){
			break;
		}

		/* Get the cluster defination stored in ROM */
        App_Get_Cluster_Definition ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
                             endPointId,
                             pDeviceInfo->p_list_of_cluster_def_ptrs[i].p_cluster_info->cluster_id,
                             &pZCLClusterDefinition );

		/* Make the ext field to zero before looping through the attribute table */
		extFieldLength = g_ZERO_c;

		updateExtFieldIndex = g_FALSE_c;

		/* Loop through all the attributes in the attribute table for this cluster id */
		for ( j = 0; ( ( j < pZCLClusterDefinition->p_cluster_info->num_attrib_info_present) &&
			 (extFieldLength < g_MAX_SCENES_ATTR_LENGTH_c ) ); j++ ) {
			attributeId = pZCLClusterDefinition->p_cluster_info->p_array_of_attrib_info[j].a_attribute_id;

            attrLength =  ZCL_Get_Data_length( pZCLClusterDefinition->p_cluster_info->
                                                   p_array_of_attrib_info[j].data_type );

			/* Check if the attribute has scenes extension */
			if ( pZCLClusterDefinition->p_cluster_info->
                p_array_of_attrib_info[j].access_permission & g_Scenes_Ext_Attribute_c ) {
				updateExtFieldIndex = g_TRUE_c;

				/* Read the attribute in the local variable */
                App_ReadWriteAttribute (
                                    endPointId,
                                    pDeviceInfo->p_list_of_cluster_def_ptrs[i].p_cluster_info->cluster_id,
                                    attributeId,
                                    (uint8_t*) &attrData,
                                    g_Read_Attribute_Data_c );

				/* Copy the current attribute value in the extension field */
				memUtils_memCopy( &( pAppSceneRecord->
                            extFieldRecord[extFieldRecordIndex].attributeData[extFieldLength] ),
					        ( uint8_t*)&attrData, attrLength );

				extFieldLength += attrLength;

				/* Update the lengh of extension fields in scenes table */
				pAppSceneRecord->extFieldRecord[extFieldRecordIndex].attributesLength = extFieldLength;
			}
		}
		/* Update ext field record only if any attribute with extension field found */
		if ( updateExtFieldIndex ) {

			/* Copy the cluster id in the extension field */
			 pAppSceneRecord->extFieldRecord[extFieldRecordIndex].clusterId =
			                  pDeviceInfo->p_list_of_cluster_def_ptrs[i].p_cluster_info->cluster_id;
			/* Increment the extension field index */
			extFieldRecordIndex++;
		}
	}
//#endif
}


/*******************************************************************************************/

void Update_LastConfiguredByAttr ( uint8_t endPointId )
{
    uint8_t * pIeeeaddr;

	pIeeeaddr = ZCL_Callback_Get_Device_IEEEAddress();

    App_ReadWriteAttribute (  endPointId,
                                    g_SCENES_CLUSTER_c,
                                    g_SCENE_MGMT_INFO_LAST_CONFIGURED_BY_ATTRIBUTE_c,
                                    pIeeeaddr,
                                    g_Write_Attribute_Data_c );
}

/*******************************************************************************************/

uint8_t ZCL_Callback_HandleSceneCmds (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponsePayloadStartLocation )
{
	uint8_t status;
	uint8_t sceneTableIndex;
	App_ScenesRecord_t appSceneRecord;
	uint16_t groupAddr;
	uint16_t groupAddr_to ;
	uint8_t sceneId_to=g_ZERO_c ;
	uint8_t copyMode=g_ZERO_c ;
	uint8_t sceneId;
	uint8_t responsePayloadLangth = g_ZERO_c;
	/* Intialize the scene count with zero */
	uint8_t sceneCount = g_ZERO_c;

	/*Loop variable*/
	uint8_t i = g_ZERO_c;

	/* Initialize the create resp variable to true by default.If creation of response is not required
	   for any command, then set it to false for that command */
	uint8_t createResp = g_TRUE_c;
	/* Get the pointer to the actual payload of the command received */
	uint8_t* pReceivedCmdPayload = ( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c + 1 );


	/* command id is retrieved from the payload*/
	uint8_t command_id = *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )	;



	if(command_id ==  g_Copy_Scene_Command_c )
	{

		/*Get copy mode */
		copyMode = *pReceivedCmdPayload;
		/*Get grouop Id from field */
		memUtils_memCopy( (uint8_t*)&groupAddr, (uint8_t*)&pReceivedCmdPayload[1], 0x02 ) ;
		/*Get scene Id from field */
		sceneId =  *(pReceivedCmdPayload+3);
		/*Get group Id to field */
		memUtils_memCopy( (uint8_t*)&groupAddr_to,(uint8_t*)&pReceivedCmdPayload[4], 0x02 )  ;
		/*Get scene id to field */
	    sceneId_to=*(pReceivedCmdPayload+6) ;

	}




	else{

	memUtils_memCopy( (uint8_t*)&groupAddr, pReceivedCmdPayload, 0x02 );
		/* Get the scene id */
	sceneId = *( pReceivedCmdPayload + 2 );
	}





	memUtils_memSet ( (uint8_t*)&appSceneRecord, 0xFF, sizeof(App_ScenesRecord_t) );

	/* Check if the received group address is null. If so, set the status as success,
       otherwise check in the group table */
	if ( m_NULL_GROUP_ADDRESS_c == groupAddr ){
		status = g_ZCL_Success_c;
	}
	/* Check if the group entry is exist for the endpoint which has received the scene command */
	else{
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
		status = ZCL_Verify_GroupAddrAndEndPoint ( groupAddr, endPointId );
#else
        status = g_ENTRY_NOT_FOUND_c;
#endif
		/* If no group entry exists for the end point, set the status as invalid field */
		if ( g_ENTRY_NOT_FOUND_c == status ) {
			status = g_ZCL_Invalid_Field_c;
		}
	}
	/* Check if any scene entry exist for the received group id and scene id.
	   If so, get the index in the output parameter */
	VerifySceneTableEntry ( groupAddr, sceneId, &sceneTableIndex );

    switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
    {

        case g_Add_Scene_Command_c:
        {
            /* check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Check the scene table index. If entry exists, overwrite it */
                if ( g_MAX_SCENES_c != sceneTableIndex ){
                    /* Remove all the information in this entry
                      (i.e. fill the scene table entry with all FF's )*/
                    PutSceneTableEntry(
                        App_ScenesTable + (uint16_t)(sceneTableIndex) * (uint16_t)sizeof(App_ScenesRecord_t),
                        &appSceneRecord );

                    /* Make the scene count to one. So that we will not increment the scene count
                       attribute for this add request */
                    sceneCount = 0x01;
                }
                /* Search empty scene entry from the scenes table, if any */
                else {
                    sceneTableIndex = Get_EmptySceneTableEntry ( );

                }

                /* Check if valid scene entry is there. If not, update the status as insufficient space */
                if ( g_MAX_SCENES_c == sceneTableIndex ) {
                    status = g_ZCL_Insufficient_Space_c;
                }
                else {
                    uint8_t SceneNameSupported = g_ZERO_c;
                    uint8_t length = g_ZERO_c;

                    /* Set the status as success */
                    status = g_ZCL_Success_c;

                    /* Update the scene table variable with the information received from add scene command */
                    /* Copy the group id */
                    memUtils_memCopy ( (uint8_t*)&appSceneRecord.groupId, pReceivedCmdPayload,
                        m_SIZE_OF_GROUP_ID_c );
                    /* Increment the received command payload pointer also */
                    pReceivedCmdPayload += m_SIZE_OF_GROUP_ID_c;

                    /* copy the scene id */
                    appSceneRecord.sceneId = *( pReceivedCmdPayload );
                    pReceivedCmdPayload++;

                    /* Copy the scene transition time */
                    memUtils_memCopy ( (uint8_t*)&(appSceneRecord.sceneTransitionTime),
                        ( pReceivedCmdPayload ), 2 );

					/*set transition time 100ms field  to zero*/
					appSceneRecord.sceneTransitionTime100ms=0x00 ;


                    pReceivedCmdPayload += m_LENGTH_OF_TNS_TIME_c;

                    /* Get the name support attribute value. If name is supported then copy the scene name,
                       Otherwise copy the null string */
                    App_ReadWriteAttribute( endPointId,
                                                  g_SCENES_CLUSTER_c,
                                                  g_SCENE_MGMT_INFO_NAME_SUPPORT_ATTRIBUTE_c,
                                                  &SceneNameSupported,
                                                  g_Read_Attribute_Data_c );

                    if ( m_IS_NAMES_SUPPORTED_c & SceneNameSupported ){
                        /* Copy the Group Name. Total bytes to be copied are string
                           lengh plus one byte which hold the lengh of the string */
                        memUtils_memCopy ( appSceneRecord.sceneName, pReceivedCmdPayload,
                            *pReceivedCmdPayload + 1 );
                    }
                    else {
                        /* Copy the null string if scene names are not supported */
                        appSceneRecord.sceneName[0] = 0x00;
                    }

                    /* Decrement the asdu length also */
                    asduLength = asduLength - ( m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c +
                                    m_LENGTH_OF_TNS_TIME_c + ( *pReceivedCmdPayload + 1 ) );

                    /* Increment the received command payload by scene name lengh plus byte
                       to hald the scene name length */
                    pReceivedCmdPayload += ( *pReceivedCmdPayload + 1 );

                    /* Copy the extension field sets */
                    while ( asduLength > 0x00 ) {
                        /* Copy the cluster id */
                        memUtils_memCopy (
                            (uint8_t*)&(appSceneRecord.extFieldRecord[i].clusterId),
                            ( pReceivedCmdPayload ),
                            g_CLUSTER_ID_LENGTH_c );
                        pReceivedCmdPayload += 2;

                        /* Get the extn field length */
                        length = *pReceivedCmdPayload;

                        /* Copy the extn field length */
                        appSceneRecord.extFieldRecord[i].attributesLength = length;
                        pReceivedCmdPayload++;
                        memUtils_memCopy (
                            ( appSceneRecord.extFieldRecord[i].attributeData ),
                            ( pReceivedCmdPayload ),
                            length );

                        /* Increment the received command payload pointer also */
                        pReceivedCmdPayload += length;
                        i++;
                        /* Decrement the received asdu length */
                        asduLength = asduLength - ( g_CLUSTER_ID_LENGTH_c + length + 1 );
                    }

                    /* Save the information in the AppSceneTable */
                    PutSceneTableEntry (
                        App_ScenesTable + (uint16_t)(sceneTableIndex) * (uint16_t)sizeof(App_ScenesRecord_t),
                        &appSceneRecord );
                    /* Update the scene count only if it is null. If not null then scene entry is
                       overwritten so there is no need to update the scene count */
                    if ( !sceneCount ) {
                        Update_AndGetSceneCount( endPointId, Increment_SceneCount );
                    }

                    /* Update the last configured by attribute also */
                    Update_LastConfiguredByAttr ( endPointId );
                }
            }
            /* update the total response payload length */
            responsePayloadLangth = m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c;
        }
        break;


        case g_View_Scene_Command_c:
        {
            /* check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Check if requested scene entry exists in the scene table. Entry will be
                   existed if scene index is not MAX_SCENES */
                if ( g_MAX_SCENES_c == sceneTableIndex ) {
                    status = g_ZCL_Not_Found_c;
                }
                else {
                    uint8_t* pLocal;

                    /* Put the status as success */
                    status = g_ZCL_Success_c;

                    /* Get the scene table entry from the scane table */
                    GetSceneTableEntry (
                        &appSceneRecord,
                        App_ScenesTable + (uint16_t)(sceneTableIndex) *
                            (uint16_t)sizeof(App_ScenesRecord_t) );

                    /* Make the local pointer to point to the start of the transition time field
                       in the response payload */
                    pLocal = ( pResponsePayloadStartLocation + m_SIZE_OF_STATUS_c +
                               m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c );

                    /* Copy the scene transition time */
                    memUtils_memCopy ( pLocal, (uint8_t*)&(appSceneRecord.sceneTransitionTime),
                        m_LENGTH_OF_TNS_TIME_c );

                    pLocal += m_LENGTH_OF_TNS_TIME_c;

                    /* Copy the Group Name. Total bytes to be copied are string
                       lengh plus one byte which hold the lengh of the string */
                    memUtils_memCopy ( pLocal, appSceneRecord.sceneName, appSceneRecord.sceneName[0] + 1 );

                    /* Increment the pointer by scene name lengh plus byte to hald the scene name length */
                    pLocal += ( appSceneRecord.sceneName[0] + 1 );

                    /* Copy the extension field sets */
                    for ( i=0; i < g_MAX_EXT_FIELD_RECORDS_c; i++ ) {
                        /* Check if any extension filed recor exist in this entry.If exists, copy it
                           in the response payload, otherwise come out of the loop */
                        if ( m_INVALID_CLUSTER_ID_c != appSceneRecord.extFieldRecord[i].clusterId ){
                            /* Copy the cluster id */
                            memUtils_memCopy ( pLocal,
                                (uint8_t*)&(appSceneRecord.extFieldRecord[i].clusterId),
                                g_CLUSTER_ID_LENGTH_c );

                            pLocal += g_CLUSTER_ID_LENGTH_c;

                            /* Copy the attributes length */
                            *pLocal = appSceneRecord.extFieldRecord[i].attributesLength;
                            pLocal++;

                            /* Copy the attrubute data */
                            memUtils_memCopy ( pLocal,
                                (appSceneRecord.extFieldRecord[i].attributeData),
                                appSceneRecord.extFieldRecord[i].attributesLength );

                            /* Update the pointer by the attribute length */
                            pLocal += appSceneRecord.extFieldRecord[i].attributesLength;

                        }
                    }
                    /* Get the response payload length of transition time, scene name and
                       extension fields */
                    responsePayloadLangth = pLocal -
                                            ( pResponsePayloadStartLocation + m_SIZE_OF_STATUS_c +
                                              m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c );
                }
            }

            /* Update the total response payload length */
            responsePayloadLangth += m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c;
        }
        break;

        case g_Remove_Scene_Command_c:
        {
            /* check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Check if requested scene entry exists in the scene table. Entry will be
                   existed if scene index is not MAX_SCENES */
                if ( g_MAX_SCENES_c == sceneTableIndex ) {
                    status = g_ZCL_Not_Found_c;
                }
                else {
                    /* Check if the scene is valid for the entry whch has to be removed.
                       If so, make the scene invalid */
                    if (sceneValidInfo.sceneValid && sceneValidInfo.sceneTableIndex == sceneTableIndex ) {
                            Scenes_UpdateSceneValidFields ( sceneTableIndex, endPointId,
                                            g_FALSE_c );
                    }

                    /* Put the status as success */
                    status = g_ZCL_Success_c;

                    /* Decrement the scene count */
                    Update_AndGetSceneCount ( endPointId, Decrement_SceneCount );

                    /* delete the entry in the scenes table */
                    PutSceneTableEntry (
                        App_ScenesTable + (uint16_t)(sceneTableIndex) * (uint16_t)sizeof(App_ScenesRecord_t),
                        &appSceneRecord );
                }
            }
            /* Update the total response payload length */
            responsePayloadLangth += m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c;
        }
        break;

        case g_Remove_All_Scenes_Command_c:
        {
            /* check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Make the status as invalid field. If any scene entry will found for this group id,
                   then change the status to success*/
                status = g_ZCL_Invalid_Field_c;

                /* Remove all the scene entries associated with this group */
                for( i = 0; i < g_MAX_SCENES_c; i++ ) {
                    /* Check if any scene exists for the group id. Remove all the scenes associated with this group */
                    sceneId = Get_SceneTableEntry ( groupAddr, i );

                    if ( m_INVALID_SCENE_ID_c != sceneId ) {
                        /* Check if the scene is valid for the entry whch has to be removed.
                           If so, make the scene invalid */
                        if (sceneValidInfo.sceneValid && sceneValidInfo.sceneTableIndex == i ) {
                                Scenes_UpdateSceneValidFields ( i, endPointId, g_FALSE_c );
                        }
                        /* Remove the scene table entry */
                        PutSceneTableEntry (
                            App_ScenesTable + (uint16_t)(i) * (uint16_t)sizeof(App_ScenesRecord_t),
                            &appSceneRecord );

                        /* Decrement the scene count */
                        Update_AndGetSceneCount ( endPointId, Decrement_SceneCount );
                        /*Set the status as success*/
                        status = g_ZCL_Success_c;
                    }
                }

            }
            /* Update the total response payload length */
            responsePayloadLangth += m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c;
        }
        break;

        case g_Store_Scene_Command_c:
        {
            /* check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Set the status as success */
                status = g_ZCL_Success_c;

                /* Check if scene index has valid value */
                if ( g_MAX_SCENES_c != sceneTableIndex ) {
                    /* Entry exists. Now modify it */
                    appSceneRecord = Handle_StoreAndRecallSceneCmd ( endPointId,
                                                            sceneTableIndex, StoreScene );
                }
                else {
                    /* Get free entry from the scene table, if any */
                    sceneTableIndex = Get_EmptySceneTableEntry();

                    /* Check if scene index has valid value */
                    if ( g_MAX_SCENES_c == sceneTableIndex ) {
                        status = g_ZCL_Insufficient_Space_c;
                    }
                    else {
                        /* Update the scene table variable with the information received
                           from add scene command */
                        /* Copy the group id */
                        memUtils_memCopy ( (uint8_t*)&(appSceneRecord.groupId), pReceivedCmdPayload,
                            m_SIZE_OF_GROUP_ID_c );
                        pReceivedCmdPayload += m_SIZE_OF_GROUP_ID_c;

                        /* copy the scene id */
                        appSceneRecord.sceneId = *( pReceivedCmdPayload );

                        /* Copy the scenes transition time to null */
                        appSceneRecord.sceneTransitionTime = 0x0000;

                        /* Copy the null string */
                        appSceneRecord.sceneName[0] = 0x00;

                        /* Update the scenes extension fields */
                        Update_sceneExtnWithCurrentAttrb ( endPointId, &appSceneRecord );

                        /* Increment the scene count also */
                        Update_AndGetSceneCount( endPointId, Increment_SceneCount );
                    }
                }
                /* Store the scene entry back in the scenes table if scene index has a valid value */
                if ( g_MAX_SCENES_c != sceneTableIndex ) {

                    PutSceneTableEntry (
                        App_ScenesTable + (uint16_t)(sceneTableIndex) * (uint16_t)sizeof(App_ScenesRecord_t),
                        &appSceneRecord );

                    /* Update the scene valid fields i.e. set the attributes
                       ( scene valid, current scene and current group) and scene valid flag */
                    Scenes_UpdateSceneValidFields ( sceneTableIndex,
                                                    endPointId,
                                                    g_TRUE_c );

                    /* Update the last configured by attribute also */
                    Update_LastConfiguredByAttr ( endPointId );
                }
            }

            /* Update the total response payload length */
            responsePayloadLangth += m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c;
        }
        break;

        case g_Recall_Scene_Command_c:
        {
            /* Check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Verify the scene table entry for the received scene id and group id.
                   If entry exists, overwrite extension fields with the current values */
                if ( g_MAX_SCENES_c != sceneTableIndex ) {
                    status = g_ZCL_Success_c;

                    scenesRecallCmdRecord.txnCount = 0x01;

                    /* Make the scenes transition time flag to true.*/
                    scenesRecallCmdRecord.sceneTxnTime = g_TRUE_c;

                    /* Copy the endpoint id and scene table index in the recall command record.
                       So when we call this function at timer expiry, it will get the info from
                       the command record variable */
                    scenesRecallCmdRecord.endPoint = endPointId;

                    scenesRecallCmdRecord.sceneTableIndex = sceneTableIndex;

                    /* Store the scene entry in the scene table variable */
                    appSceneRecord = Handle_StoreAndRecallSceneCmd ( endPointId,
                                             sceneTableIndex, RecallScene );

                    if (! ( appSceneRecord.sceneTransitionTime) ) {
                        /* Transition Time is set to Zero. So, we dont need to do background
                        check. All the attributes are already set to specified value in the received
                        frame*/
                        scenesRecallCmdRecord.sceneTxnTime = g_FALSE_c;
                    }
                    else {
                        /*Transition Time is valid. We need update the attributes gradually based on the
                        transition time. So, update the timers.*/

                        /* Get the scenes transition time */
                        scenesRecallCmdRecord.sceneTxnTimeout = appSceneRecord.sceneTransitionTime;
                    }
                    /* Update the scene valid fields i.e. set the attributes
                       ( scene valid, current scene and current group) and scene valid flag */
                    Scenes_UpdateSceneValidFields ( sceneTableIndex, endPointId,g_TRUE_c );
                }
                else {
                    status = g_ZCL_Not_Found_c;
                }
            }
            /* Copy the status */
            *pResponsePayloadStartLocation = status;

            /* No response is required for this command */
            createResp = g_FALSE_c;
        }
        break;

        case g_Scene_Membership_Command_c:
        {
            uint8_t remaingCapacity = 0x00;
            uint8_t respSceneCount  = 0x00;
            /* Initialize the pointer to the starting location where the scene list has to
               be copied the response payload */
            uint8_t* prespSceneList = ( pResponsePayloadStartLocation + m_SIZE_OF_STATUS_c +
                                     m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c + sizeof ( respSceneCount ));

            /* Get the scene count */
            sceneCount = Update_AndGetSceneCount (endPointId, Get_SceneCount);

            /* Calculate the remaining capacity of the scenes table */
            remaingCapacity = g_MAX_SCENES_c - sceneCount;

            /* check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Set the status as success */
                status = g_ZCL_Success_c;

                /* Loop through the scenes table and find out the scene entries for
                   the requested group id */
                for ( i = 0; i < g_MAX_SCENES_c; i++ ) {
                    /* Get the scene id from the scene table */
                    sceneId = Get_SceneTableEntry ( groupAddr, i );

                    /* Check if this is a valid scene id. If so, update the scene count
                       and scene list */
                    if ( m_INVALID_SCENE_ID_c != sceneId ) {
                        respSceneCount++;
                        *prespSceneList++ = sceneId;
                    }
                }
                /* Put the scene count in the response payload */
                *( pResponsePayloadStartLocation + m_SIZE_OF_STATUS_c +
                   m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c ) = respSceneCount;

                /* Update the response payload length by scene count plus a byte whch holds
                   the scene count */
                responsePayloadLangth += ( 1 + respSceneCount );
            }

            /* Update the total response payload length */
            responsePayloadLangth += m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c;

            /* Copy the status */
            *pResponsePayloadStartLocation = status;

            /* Copy the remaining capacity */
            *( pResponsePayloadStartLocation + m_SIZE_OF_STATUS_c )  = remaingCapacity;

            /* Copy the group id in the response payload */
            memUtils_memCopy ( pResponsePayloadStartLocation+2, (uint8_t*)&groupAddr, m_SIZE_OF_GROUP_ID_c );

            /* Response is already created for this command. So no need to create it again */
            createResp = g_FALSE_c;
        }
        break;
#if ( g_ZLL_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )

		case g_Enhanded_Add_Scene_Command_c :
		{
			/*check if grp id and scene id is zero set status as invalid field */
			if((0x00==sceneId)&&(0x0000==groupAddr))
			{
			 status=g_ZCL_Invalid_Field_c ;
			 }
            /* check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Check the scene table index. If entry exists, overwrite it */
                if ( g_MAX_SCENES_c != sceneTableIndex ){
                    /* Remove all the information in this entry
                      (i.e. fill the scene table entry with all FF's )*/
                    PutSceneTableEntry(
                        App_ScenesTable + (uint16_t)(sceneTableIndex) * (uint16_t)sizeof(App_ScenesRecord_t),
                        &appSceneRecord );

                    /* Make the scene count to one. So that we will not increment the scene count
                       attribute for this add request */
                    sceneCount = 0x01;
                }
                /* Search empty scene entry from the scenes table, if any */
                else {
                    sceneTableIndex = Get_EmptySceneTableEntry ( );

                }

                /* Check if valid scene entry is there. If not, update the status as insufficient space */
                if ( g_MAX_SCENES_c == sceneTableIndex ) {
                    status = g_ZCL_Insufficient_Space_c;
                }
                else {
                    uint8_t SceneNameSupported = g_ZERO_c;
                    uint8_t length = g_ZERO_c;
                    uint16_t transtion100ms ;

                    /* Set the status as success */
                    status = g_ZCL_Success_c;

                    /* Update the scene table variable with the information received from add scene command */
                    /* Copy the group id */
                    memUtils_memCopy ( (uint8_t*)&appSceneRecord.groupId, pReceivedCmdPayload,
                        m_SIZE_OF_GROUP_ID_c );
                    /* Increment the received command payload pointer also */
                    pReceivedCmdPayload += m_SIZE_OF_GROUP_ID_c;

                    /* copy the scene id */
                    appSceneRecord.sceneId = *( pReceivedCmdPayload );
                    pReceivedCmdPayload++;

                    /* Copy the scene transition time */
                    memUtils_memCopy ( (uint8_t*)&(appSceneRecord.sceneTransitionTime),
                        ( pReceivedCmdPayload ), 2 );
					/*transition time(in tenth of seconds) convert in to seconds */
                    transtion100ms=appSceneRecord.sceneTransitionTime ;

                    transtion100ms/=10 ;

					/*Copy the scenetransitiontime100ms  */

					memUtils_memCopy ( (uint8_t*)&(appSceneRecord.sceneTransitionTime100ms),
                        ( (uint8_t*)&(transtion100ms) ), 2 );



					pReceivedCmdPayload += m_LENGTH_OF_TNS_TIME_c;










                    /* Get the name support attribute value. If name is supported then copy the scene name,
                       Otherwise copy the null string */
                    App_ReadWriteAttribute( endPointId,
                                                  g_SCENES_CLUSTER_c,
                                                  g_SCENE_MGMT_INFO_NAME_SUPPORT_ATTRIBUTE_c,
                                                  &SceneNameSupported,
                                                  g_Read_Attribute_Data_c );

                    if ( m_IS_NAMES_SUPPORTED_c & SceneNameSupported ){
                        /* Copy the Group Name. Total bytes to be copied are string
                           lengh plus one byte which hold the lengh of the string */
                        memUtils_memCopy ( appSceneRecord.sceneName, pReceivedCmdPayload,
                            *pReceivedCmdPayload + 1 );
                    }
                    else {
                        /* Copy the null string if scene names are not supported */
                        appSceneRecord.sceneName[0] = 0x00;
                    }

                    /* Decrement the asdu length also */
                    asduLength = asduLength - ( m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c +
                                    m_LENGTH_OF_TNS_TIME_c + ( *pReceivedCmdPayload + 1 ) );

                    /* Increment the received command payload by scene name lengh plus byte
                       to hald the scene name length */
                    pReceivedCmdPayload += ( *pReceivedCmdPayload + 1 );

                    /* Copy the extension field sets */
                    while ( asduLength > 0x00 ) {
                        /* Copy the cluster id */
                        memUtils_memCopy (
                            (uint8_t*)&(appSceneRecord.extFieldRecord[i].clusterId),
                            ( pReceivedCmdPayload ),
                            g_CLUSTER_ID_LENGTH_c );
                        pReceivedCmdPayload += 2;

                        /* Get the extn field length */
                        length = *pReceivedCmdPayload;

                        /* Copy the extn field length */
                        appSceneRecord.extFieldRecord[i].attributesLength = length;
                        pReceivedCmdPayload++;
                        memUtils_memCopy (
                            ( appSceneRecord.extFieldRecord[i].attributeData ),
                            ( pReceivedCmdPayload ),
                            length );

                        /* Increment the received command payload pointer also */
                        pReceivedCmdPayload += length;
                        i++;
                        /* Decrement the received asdu length */
                        asduLength = asduLength - ( g_CLUSTER_ID_LENGTH_c + length + 1 );
                    }

                    /* Save the information in the AppSceneTable */
                    PutSceneTableEntry (
                        App_ScenesTable + (uint16_t)(sceneTableIndex) * (uint16_t)sizeof(App_ScenesRecord_t),
                        &appSceneRecord );
                    /* Update the scene count only if it is null. If not null then scene entry is
                       overwritten so there is no need to update the scene count */
                    if ( !sceneCount ) {
                        Update_AndGetSceneCount( endPointId, Increment_SceneCount );
                    }

                    /* Update the last configured by attribute also */
                    Update_LastConfiguredByAttr ( endPointId );
                }
            }
            /* update the total response payload length */
            responsePayloadLangth = m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c;
        }
        break;

		case g_Enhanced_View_Scene_Command_c :

		{
            /* check if the status is not invalid field */
            if ( g_ZCL_Invalid_Field_c != status ) {
                /* Check if requested scene entry exists in the scene table. Entry will be
                   existed if scene index is not MAX_SCENES */
                if ( g_MAX_SCENES_c == sceneTableIndex ) {
                    status = g_ZCL_Not_Found_c;
                }
                else {
                    uint8_t* pLocal;

                    /* Put the status as success */
                    status = g_ZCL_Success_c;

                    /* Get the scene table entry from the scane table */
                    GetSceneTableEntry (
                        &appSceneRecord,
                        App_ScenesTable + (uint16_t)(sceneTableIndex) *
                            (uint16_t)sizeof(App_ScenesRecord_t) );

                    /* Make the local pointer to point to the start of the transition time field
                       in the response payload */
                    pLocal = ( pResponsePayloadStartLocation + m_SIZE_OF_STATUS_c +
                               m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c );

                    /* Copy the scene transition time */
                    memUtils_memCopy ( pLocal, (uint8_t*)&(appSceneRecord.sceneTransitionTime),
                        m_LENGTH_OF_TNS_TIME_c );

                    pLocal += m_LENGTH_OF_TNS_TIME_c;

                    /* Copy the Group Name. Total bytes to be copied are string
                       lengh plus one byte which hold the lengh of the string */
                    memUtils_memCopy ( pLocal, appSceneRecord.sceneName, appSceneRecord.sceneName[0] + 1 );

                    /* Increment the pointer by scene name lengh plus byte to hald the scene name length */
                    pLocal += ( appSceneRecord.sceneName[0] + 1 );

                    /* Copy the extension field sets */
                    for ( i=0; i < g_MAX_EXT_FIELD_RECORDS_c; i++ ) {
                        /* Check if any extension filed recor exist in this entry.If exists, copy it
                           in the response payload, otherwise come out of the loop */
                        if ( m_INVALID_CLUSTER_ID_c != appSceneRecord.extFieldRecord[i].clusterId ){
                            /* Copy the cluster id */
                            memUtils_memCopy ( pLocal,
                                (uint8_t*)&(appSceneRecord.extFieldRecord[i].clusterId),
                                g_CLUSTER_ID_LENGTH_c );

                            pLocal += g_CLUSTER_ID_LENGTH_c;

                            /* Copy the attributes length */
                            *pLocal = appSceneRecord.extFieldRecord[i].attributesLength;
                            pLocal++;

                            /* Copy the attrubute data */
                            memUtils_memCopy ( pLocal,
                                (appSceneRecord.extFieldRecord[i].attributeData),
                                appSceneRecord.extFieldRecord[i].attributesLength );

                            /* Update the pointer by the attribute length */
                            pLocal += appSceneRecord.extFieldRecord[i].attributesLength;

                        }
                    }
                    /* Get the response payload length of transition time, scene name and
                       extension fields */
                    responsePayloadLangth = pLocal -
                                            ( pResponsePayloadStartLocation + m_SIZE_OF_STATUS_c +
                                              m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c );
                }
            }

            /* Update the total response payload length */
            responsePayloadLangth += m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c;
        }
        break;

		case g_Copy_Scene_Command_c :
		{

			/* Check if any scene entry exist for the received group id and scene id.
							If so, get the index in the output parameter */
			VerifySceneTableEntry ( groupAddr, sceneId, &sceneTableIndex );

			uint8_t index = sceneTableIndex ;

			VerifySceneTableEntry ( groupAddr_to, sceneId_to, &sceneTableIndex );

			/* Check if all scenes to be copied or not using bit0 of mode field */
			if(copyMode&&g_COPY_ALL_SCENES_c)
			{

				if(0==groupAddr)
				{
				/*set status as invalid field in response */
				status=g_ZCL_Invalid_Field_c ;
				}
				else  /*else of grp addrs equal zero */
				{
					/*Declare pointers to array of scene table index  */
					uint8_t *psceneTableFromIndex=g_NULL_c;

					/*Declare  size of arrays contain Group Id from and Group Id to field indices respectively */
					uint8_t fromFieldSize ;

					/*define fist elements of 'Goroup Id from' and 'Group Id to' field array */
					SceneTableCopyIndexArray ( groupAddr ,&fromFieldSize) ;


					/*Copy scene from 'groupId from' field to 'grpId to' field */
					uint8_t i=0 ;

					/*check grp id to field size */
					while(0!=fromFieldSize)
					{
						uint8_t sceneIdFrom ;
						GetSceneTableEntry ( &appSceneRecord,
												&App_ScenesTable[sceneTableIndexArray[i]])  ;

						/*get scene id */
						sceneIdFrom=appSceneRecord.sceneId ;

						/*Check if any scene entry exist for the received group id and scene id.
							If so, get the index in the output parameter */

						VerifySceneTableEntry ( groupAddr_to, sceneIdFrom, &sceneTableIndex );

						/* Check the scene table index. If entry exists, overwrite it */
						if ( g_MAX_SCENES_c != sceneTableIndex ){

						memUtils_memSet ( (uint8_t*)&appSceneRecord, 0xFF, sizeof(App_ScenesRecord_t) );

						/* Remove all the information in this entry
							(i.e. fill the scene table entry with all FF's )*/
							PutSceneTableEntry(
									&App_ScenesTable[sceneTableIndex],&appSceneRecord );


                    /* Make the scene count to one. So that we will not increment the scene count
                       attribute for this add request */
                    sceneCount = 0x01;
						}
					 /* Search empty scene entry from the scenes table, if any */
						else {
								sceneTableIndex = Get_EmptySceneTableEntry ( );

								}

						/* Check if valid scene entry is there. If not, update the status as insufficient space */
						if ( g_MAX_SCENES_c == sceneTableIndex ) {
									status = g_ZCL_Insufficient_Space_c;
						}

						else {

							/*group id */

							App_ScenesTable[sceneTableIndex].groupId= groupAddr_to ;

							/*scene Id */
							App_ScenesTable[sceneTableIndex].sceneId= App_ScenesTable[sceneTableIndexArray[i]].sceneId ;


							/*copy scene transition time */

							App_ScenesTable[sceneTableIndex].sceneTransitionTime= App_ScenesTable[*(psceneTableFromIndex+i)].sceneTransitionTime ;

							/*scene transition time 100ms */
							App_ScenesTable[sceneTableIndex].sceneTransitionTime100ms= App_ScenesTable[*(psceneTableFromIndex+i)].sceneTransitionTime100ms ;

							/*extension field */
							memUtils_memCopy((uint8_t*)App_ScenesTable[sceneTableIndex].extFieldRecord,(uint8_t*)App_ScenesTable[*(psceneTableFromIndex+i)].														extFieldRecord,sizeof(App_ScenesTable[*(psceneTableFromIndex+i)].extFieldRecord)) ;

						}

							fromFieldSize-- ;
							i++ ;

					}

				}

			}
			else  /*else of check copy mode */
			{
				if(0==groupAddr) {
				/*set status as invalid field in response */

					status=g_ZCL_Invalid_Field_c ;
				}
				else /*else of grp addrs equals zero */
				{

				/* Copy 'GroupId/Scene Id from' field to 'Group Id/Scene Id to' field */

					App_ScenesTable[sceneTableIndex].sceneTransitionTime= App_ScenesTable[index].sceneTransitionTime ;

					App_ScenesTable[sceneTableIndex].sceneTransitionTime100ms= App_ScenesTable[index].sceneTransitionTime100ms ;

					 memUtils_memCopy((uint8_t*)App_ScenesTable[sceneTableIndex].extFieldRecord,(uint8_t*)App_ScenesTable[index].extFieldRecord,sizeof(App_ScenesTable[index].extFieldRecord)) ;
				}
			}
                        
                  /* update the total response payload length */
                  responsePayloadLangth = m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c + m_SIZE_OF_SCENE_ID_c;
		}
		break ;
#endif /*    g_ZLL_SCENES_CLUSTER_ENABLE_d */

        default:
            createResp = g_FALSE_c;
            break;
    }


	/* Check if the response is to be created */
	if ( createResp ) {
		/* Copy the status */
		*pResponsePayloadStartLocation = status;

            /* Copy the group id in the response payload */
            memUtils_memCopy ( pResponsePayloadStartLocation + m_SIZE_OF_STATUS_c, (uint8_t*)&groupAddr,
                m_SIZE_OF_GROUP_ID_c );

		/* Copy the scene id in the response payload */
		*( pResponsePayloadStartLocation +  m_SIZE_OF_STATUS_c + m_SIZE_OF_GROUP_ID_c ) = sceneId;
	}

	return responsePayloadLangth;
}

/*******************************************************************************************/

static uint8_t Update_AndGetSceneCount ( uint8_t endPointId, uint8_t direction )
{
	uint8_t sceneCount = 0x00;

	/* Check if direction is not reset count.If direction is reset count,then put
	   scene count as zero in the attribute table */
	if ( Reset_SceneCount != direction) {
		App_ReadWriteAttribute (  endPointId,
                                        g_SCENES_CLUSTER_c,
                                        g_SCENE_MGMT_INFO_SCENE_COUNT_ATTRIBUTE_c,
                                        &sceneCount,
                                        g_Read_Attribute_Data_c	);
		/* Check If direction is get scenes count. If so, returns the scene count */
		if ( Get_SceneCount == direction ){
			return sceneCount;
		}
		/* If direction is increment scenes count, increment the scne count value */
		if ( Increment_SceneCount == direction ){
			sceneCount++;
		}
		/* If direction is decrement scenes count, decrement the scne count value */
		else{
			sceneCount--;
		}
	}
	/* Put the value of the scene count attribute */
	App_ReadWriteAttribute (  endPointId,
                                    g_SCENES_CLUSTER_c,
                                    g_SCENE_MGMT_INFO_SCENE_COUNT_ATTRIBUTE_c,
                                    &sceneCount,
                                    g_Write_Attribute_Data_c );

	/* Returns the scene count */
	return sceneCount;

}

/*******************************************************************************************/
static uint8_t Get_SceneTableEntry ( uint16_t groupId, uint8_t index )
{
	App_ScenesRecord_t appSceneRecord;

	/* Get the scene entry from the scenes table */
	GetSceneTableEntry (
		&appSceneRecord,
		App_ScenesTable + (uint16_t)(index) * (uint16_t)sizeof(App_ScenesRecord_t) );
	/* Returns the scene id, if group id is matched */
	if ( ( groupId == appSceneRecord.groupId ) ) {
		return appSceneRecord.sceneId;
	}
	/* Retrun FF if group id is not matched */
	return 0xFF;
}
/*******************************************************************************************/
App_ScenesRecord_t *App_GetSceneTableEntry (uint8_t index )
{
  	if ( index > g_MAX_SCENES_c ){
        return g_NULL_c;
    }
	else {
	   return &App_ScenesTable[index];
	}

}
/*******************************************************************************************/
void VerifySceneTableEntry ( uint16_t groupId, uint8_t sceneId, uint8_t* psceneTableIndex )
{
	uint8_t index;

	App_ScenesRecord_t appSceneRecord;
	for ( index = 0; index < g_MAX_SCENES_c; index++ ) {
		/* Get the scene entry from the scenes table */
		GetSceneTableEntry (
			&appSceneRecord,
			&(App_ScenesTable[index]) );
		/* If the requested group id and scene id found, then update the scene table
		   index to this scne table entry */
		if ( ( appSceneRecord.groupId == groupId ) && ( appSceneRecord.sceneId == sceneId ) ) {
			*psceneTableIndex = index;
			return;
		}
	}
	/* Scene id and group id are not found, update scnee table with max scenes */
	*psceneTableIndex = g_MAX_SCENES_c;
}

/*******************************************************************************************/
void  SceneTableCopyIndexArray ( uint16_t groupId ,uint8_t *noOfSceneIds)
{
	uint8_t index;
	uint8_t i=0 ;



	App_ScenesRecord_t appSceneRecord;
	for ( index = 0; index < g_MAX_SCENES_c; index++ ) {
		/* Get the scene entry from the scenes table */
		GetSceneTableEntry (
			(uint8_t*)&appSceneRecord,
			App_ScenesTable + (uint16_t)(index) * (uint16_t)sizeof(App_ScenesRecord_t) );
		/* If the requested group id found, then update the scene table
		   index to this scne table entry */
		if ( appSceneRecord.groupId == groupId  ) {

			sceneTableIndexArray[i] = index;
			i++ ;

			}
			}

	/*Size of the array contains scene table index */
			*noOfSceneIds=i ;
}
/*********************************************************************************************/

uint8_t Get_EmptySceneTableEntry ( void )
{
	uint8_t index;

	App_ScenesRecord_t appSceneRecord;

	for ( index = 0; index < g_MAX_SCENES_c; index++ ) {
		/* Get the scene entry from the scenes table */
		GetSceneTableEntry (
			&appSceneRecord,
			App_ScenesTable + (uint16_t)(index) * (uint16_t)sizeof(App_ScenesRecord_t) );

		/* If entry is free, then returns the index of the free entry */
		if ( ( m_INVALID_GROUP_ID_c == appSceneRecord.groupId ) ) {
			return index;
		}
	}
	/* Free entry is not found, returns max scenes */
	return g_MAX_SCENES_c;
}

/*******************************************************************************************/

static void App_Handle_ValidScenesValuesChanges( void )
{
    if ( sceneValidInfo.sceneValid ){
        Handle_StoreAndRecallSceneCmd ( sceneValidInfo.endPoint,
			                        sceneValidInfo.sceneTableIndex, CheckSceneValidity  );
    }
}


#endif                  /*  ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 ) */



/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
