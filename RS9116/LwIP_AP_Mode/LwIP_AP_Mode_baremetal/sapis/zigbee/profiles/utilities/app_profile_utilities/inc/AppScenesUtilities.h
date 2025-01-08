#ifndef _APPLICATION_SCENES_
#define _APPLICATION_SCENES_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_MAX_SCENES_ATTR_LENGTH_c							        0x14
#define g_MAX_SIZE_OF_SCENE_NAME_c							        0x10
#define g_MAX_EXT_FIELD_RECORDS_c							        0x03
#define g_MAX_SCENES_c                                              0x03
#define g_COPY_ALL_SCENES_c                                         0x01

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct ExtensionFieldSets
{
    uint16_t clusterId;
    uint8_t attributesLength;
    uint8_t attributeData[g_MAX_SCENES_ATTR_LENGTH_c];	
}__attribute__((__packed__))ExtensionFieldSets;

typedef struct App_ScenesRecord_Tag
{
    uint16_t groupId;
    uint8_t sceneId;
    uint8_t sceneName[g_MAX_SIZE_OF_SCENE_NAME_c];
    uint16_t sceneTransitionTime;
    uint8_t sceneTransitionTime100ms;
    ExtensionFieldSets extFieldRecord[g_MAX_EXT_FIELD_RECORDS_c];
} __attribute__((__packed__))App_ScenesRecord_t;

typedef struct Scenes_RecallCmdRecord
{
    uint8_t txnCount;
    uint8_t endPoint;
    uint8_t sceneTableIndex;
    uint8_t sceneTxnTime;
    uint16_t sceneTxnTimeout;
} __attribute__((__packed__))Scenes_RecallCmdRecord_t;

typedef struct Scene_ValidInfo_Tag
{
    uint8_t endPoint;
    uint8_t sceneTableIndex;
    uint8_t sceneValid;
} __attribute__((__packed__))Scene_ValidInfo_t;

enum
{
     Get_SceneCount   			= 0x00,
     Increment_SceneCount     	= 0x01,
     Decrement_SceneCount      	= 0x02,
	 Reset_SceneCount			= 0x03
};

enum
{
     StoreScene   		        = 0x00,
     RecallScene     	        = 0x01,
     CheckSceneValidity         = 0x02
};

enum
{
     discrete   			    = 0x00,
     Analog     			    = 0x01
};


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
extern Scenes_RecallCmdRecord_t scenesRecallCmdRecord;
extern Scene_ValidInfo_t sceneValidInfo;
#endif

/*---------------------------------------------------------------------------
   Function Prototypes
---------------------------------------------------------------------------*/

App_ScenesRecord_t Handle_StoreAndRecallSceneCmd ( uint8_t endPointId,
                                                   uint8_t sceneTableIndex,
                                                    uint8_t direction );

void VerifySceneTableEntry ( uint16_t groupId, uint8_t sceneId,
	                         uint8_t* psceneTableIndex );

void SceneTableCopyIndexArray ( uint16_t groupId ,uint8_t *size) ;

uint8_t Get_EmptySceneTableEntry ( void );

void App_ScenesTableInit( void );

void AppScenesTask( void );

uint8_t ZCL_Callback_Handle_Store_Commands (uint16_t group_id, uint8_t scene_id,
															uint8_t endPointId);

App_ScenesRecord_t *App_GetSceneTableEntry (uint8_t index );



/*-----------------------------------------------------------------------------
      End Of File
*----------------------------------------------------------------------------*/
#endif                                      /* _APPLICATION_SCENES_ */
