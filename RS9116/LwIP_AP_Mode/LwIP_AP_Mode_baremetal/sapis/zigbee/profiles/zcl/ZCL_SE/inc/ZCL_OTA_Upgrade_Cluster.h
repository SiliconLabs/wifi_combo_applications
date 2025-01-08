/******************************************************************************
* ZCL_Tunneling_Cluster.h
*******************************************************************************
* Program Description:
* This file contains the ZCL_OTA_Upgrade_Cluster attribute and command
* information
******************************************************************************/
#ifndef _ZCL_OTA_UPGRADE_CLUSTER_H_
#define _ZCL_OTA_UPGRADE_CLUSTER_H_

/********************************************************************************
* Includes
*********************************************************************************/
#include "ZCL_Functionality.h"

/********************************************************************************
* Global Constants
*********************************************************************************/
#define g_OTA_UPGRADE_CLUSTER_c                         0x0019

#define g_UPGRADE_SERVER_ID_c                           0x0000
#define g_FILE_OFFSET_c                                 0x0001
#define g_CURRENT_FILE_VERSION_c                        0x0002
#define g_CURRENT_ZIGBEESTACK_VERSION_c                 0x0003
#define g_DOWNLOADED_FILE_VERSION_c                     0x0004
#define g_DOWNLOADED_ZIGBEESTACK_VERSION_c              0x0005
#define g_IMAGE_UPGRADE_STATUS_c                        0x0006
#define g_MANUFACTURER_ID_c                             0x0007
#define g_IMAGE_TYPE_ID_c                               0x0008


#define g_MFG_ID_WILD_CARD_c                            0xFFFF
#define g_IMAGE_TYPE_WILD_CARD_c                        0xFFFF
#define g_IMAGE_TYPE_SECURITY_c                         0xFFC0
#define g_IMAGE_TYPE_CONFIG_c                           0xFFC1
#define g_IMAGE_TYPE_LOG_c                              0xFFC2
#define g_FILE_VERSION_WILD_CARD_c                      0xFFFFFFFF
#define g_ZIGBEE_2006_STACK_VERSION_c                   0x0000
#define g_ZIGBEE_2007_STACK_VERSION_c                   0x0001
#define g_ZIGBEE_PRO_STACK_VERSION_c                    0x0002

// These apply to the field control of the Over-the-air messages
#define OTA_HW_VERSION_BIT_MASK                     0x01
#define OTA_NODE_EUI64_BIT_MASK                     0x01

#define g_SEND_IEEE_ADDR_WITH_BLK_REQ_c                         0x01u
/********************************************************************************
* Memory Declarations
*********************************************************************************/
#if ( g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ma_OTA_Upgrade_Server_Cluster_Info;
#endif				/* g_OTA_UPGRADE_SERVER_CLUSTER_ENABLE_d */

#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ma_OTA_Upgrade_Client_Cluster_Info;
#endif			   /* g_OTA_UPGRADE_CLIENT_CLUSTER_ENABLE_d */
/********************************************************************************
* Type Definitions
*********************************************************************************/

/********************************************************************************
   OTA cluster command and attributes structure
********************************************************************************/
typedef struct OTA_Upgrade_Cluster_Attribute_Tag
{
    uint8_t upgradeServerId[8];
    uint8_t fileOffset[4];
    uint8_t currentFileVersion[4];
    uint8_t currentZigBeeStackVersion[2];
    uint8_t downloadedFileVersion[4];
    uint8_t downloadedZigBeeStackVersion[2];
    uint8_t imageUpgradeStatus;
    uint8_t manufacturerId[2];
    uint8_t imageTypeId[2];
}__attribute__((__packed__))OTA_Upgrade_Cluster_Attribute_t;


typedef struct ImageNotify_Tag
{
	uint8_t payLoadType;
    uint8_t queryJitter;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t newFileVersion[4];
}__attribute__((__packed__))ImageNotify_t;

typedef struct QueryNextImageRequest_Tag
{
	uint8_t fieldControl;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t currentFileVersion[4];
    uint8_t hardwareVersion[2];
}__attribute__((__packed__))QueryNextImageRequest_t;

typedef struct QueryNextImageResponse_Tag
{
	uint8_t status;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t imageSize[4];
}__attribute__((__packed__))QueryNextImageResponse_t;

typedef struct ImageBlockRequest_Tag
{
	uint8_t fieldControl;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t fileOffset[4];
    uint8_t maxDataSize;
    uint8_t requestNodeAddr[8];
}__attribute__((__packed__))ImageBlockRequest_t;


typedef struct ImagePageRequest_Tag
{
	uint8_t fieldControl;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t fileOffset[4];
    uint8_t maxDataSize;
    uint8_t pageSize[2];
    uint8_t responseSpacing[2];
    uint8_t requestNodeAddr[8];
}__attribute__((__packed__))ImagePageRequest_t;

typedef struct ImageBlockRspSuccess_Tag
{	
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t fileOffset[4];
    uint8_t dataSize;
    uint8_t imageData[1];
}__attribute__((__packed__))ImageBlockRspSuccess_t;

typedef struct ImageBlockRspWaitForData_Tag
{	
    uint8_t currentTime[4];
    uint8_t requestTime[4];
}__attribute__((__packed__))ImageBlockRspWaitForData_t;


typedef struct ImageBlockResponse_Tag
{
	uint8_t status;
    union ImageBlockRspPayload
    {
        ImageBlockRspWaitForData_t ImageBlockRspWaitForData;
        ImageBlockRspSuccess_t ImageBlockRspSuccess;
    } ImageBlockRspPayload;
}__attribute__((__packed__))ImageBlockResponse_t;

typedef struct ImageUpgradeEndRequest_Tag
{
	uint8_t status;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
}__attribute__((__packed__))ImageUpgradeEndRequest_t;


typedef struct ImageUpgradeEndResponse_Tag
{
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t currentTime[4];
    uint8_t upgradeTime[4];
}__attribute__((__packed__))ImageUpgradeEndResponse_t;


typedef struct QuerySpecificFileRequest_Tag
{
    uint8_t requestNodeAddr[8];
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t zigbeeStackVersion[2];
}__attribute__((__packed__))QuerySpecificFileRequest_t;


typedef struct QuerySpecificFileResponse_Tag
{
    uint8_t status;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t imageSize[4];
}__attribute__((__packed__))QuerySpecificFileResponse_t;

/********************************************************************************
* Type Definitions
*********************************************************************************/

typedef enum OTAUpgrade_Cluster_Command_Tag
{
	g_ImageNotify_c = 0x00,
	g_QueryNextImageRequest_c,
	g_QueryNextImageResponse_c,
    g_ImageBlockRequest_c,
    g_ImagePageRequest_c,
    g_ImageBlockResponse_c,
	g_UpgradeEndRequest_c,
	g_UpgradeEndResponse_c,
    g_QuerySpecificFileRequest_c,
    g_QuerySpecificFileResponse_c
}OTAUpgrade_Cluster_Command_t;


typedef enum OTAUpgrade_Cluster_Event_Tag
{
    g_ImageNotify_Event_c = 0xD5,
	g_QueryNextImageRequest_Event_c,
	g_QueryNextImageResponse_Event_c,
    g_ImageBlockRequest_Event_c,
    g_ImagePageRequest_Event_c,
    g_ImageBlockResponse_Event_c,
	g_UpgradeEndRequest_Event_c,
	g_UpgradeEndResponse_Event_c,
    g_QuerySpecificFileRequest_Event_c,
    g_QuerySpecificFileResponse_Event_c
}OTA_Upgrade_Cluster_Event_t;


typedef enum ImageUpgrade_Status_Tag
{
    g_Normal_c              = 0x00,
    g_DownloadInprogress_c  = 0x01,
    g_DownloadComplete_c    = 0x02,
    g_WaitingToUpgrade_c    = 0x03,
    g_CountDown_c           = 0x04,
    g_WaitForMore           = 0x05
}ImageUpgrade_Status_t;

typedef enum ImageNotify_Payloadtype_Tag
{
    g_QueryJitter_c             = 0x00,
    g_QueryJitterAndManufCode_c = 0x01,
    g_QueryJitterManufCodeAndImagetype = 0x02,
    g_QueryJitterManufCodeImagetypeAndNewFileVersion_c= 0x03
}ImageNotify_Payloadtype_t;

typedef enum ImageBlockReq_FieldControl_Tag
{
    g_blockIeeeAddrPresent_c        = 0x01,
}ImageBlockReq_FieldControl_t;

typedef enum ImageBlockRsp_FieldControl_Tag
{
    g_pageIeeeAddrPresent_c        = 0x01,
}ImageBlockRsp_FieldControl_Tag;
/*********************************************************************************/
/* Function Prototypes                                                           */
/*********************************************************************************/

uint8_t ZCL_HandleOTAUpgradeClusterCmd ( uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );


#endif                                 /* _ZCL_OTA_UPGRADE_CLUSTER_H_ */
/********************************************************************************
* End Of File
*********************************************************************************/

