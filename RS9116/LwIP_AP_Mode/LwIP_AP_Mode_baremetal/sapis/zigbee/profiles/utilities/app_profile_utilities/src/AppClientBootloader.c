/**********************************************************************************
* Includes
***********************************************************************************/
#include "ZigBeeDataTypes.h"
#include <stdbool.h>
#include "ZCL_Common.h"
#include "stack_common.h"
#include "sw_timer.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "zdo_interface.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "AppTimerUtilities.h"
#include "ApplicationThinLayer.h"
#include "AppDiscoveryUtilities.h"
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
//#include "M24M02.h"

#include "micro_bootloader.h"

#include "AppOTACommon.h"
#include "AppClientBootloader.h"
#include "ZCL_OTA_Upgrade_Cluster.h"
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
#include "AppCBKEUtilities.h"
#endif   /*g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d */

/**************************************************************************************
* Global Constants
**************************************************************************************/
#define g_UNDEFINED_ENDPOINT_c                                  0xFF
#define g_UNKNOWN_NODE_ID_c                                     0xFFFF
#define g_RESET_ALL_ENTRIES_c                                   0xFFFFFFFF
#define g_BLOCK_SIZE_c                                          16
/* Minimum sizes micros*/
#define g_IMAGE_BLOCK_RESPONSE_SUCCESS_MIN_LENGTH_c             14
#define g_UPGRADE_END_RESPONSE_MIN_LENGTH_c                     16
#define g_QUERY_NEXT_IMAGE_SUCCESS_RESPONSE_MIN_LENGTH_c        13
#define g_IMAGE_BLOCK_ABORT_LENGTH_c                            1
#define g_IMAGE_BLOCK_RESPONSE_WAIT_FOR_DATA_LENGTH_c           8
#define g_QUERY_SPECIFIC_FILE_RESPONSE_SUCCESS_LENGTH_c         13
#define g_SEC_CRED_VER_c                                        0x00

/**************************************************************************************
* Public Memory declarations
***************************************************************************************/
uint8_t myEndpoint = g_UNDEFINED_ENDPOINT_c;
uint8_t serverEndpoint = g_UNDEFINED_ENDPOINT_c;
uint16_t serverNodeId = g_UNKNOWN_NODE_ID_c;

#define changeBootLoadState( state )        clientOtaState = state
#define getBootLoadState()                  clientOtaState

/**************************************************************************************
* Private Memory declarations
***************************************************************************************/
static uint8_t clientOtaState = APP_CLIENT_BTLDR_INIT;
static bool bootLoaderWaitstate = g_FALSE_c;
static bool waitingForResponse = g_FALSE_c;
static ClientPageRequestState_t pageRequestState = APP_OTA_NO_PAGE_REQUEST;
static uint32_t pageRequestOffset;
static uint8_t packetSize = 0;
static uint16_t currentPageSize;
static uint32_t lastTickTime = 0x00;
static uint32_t nextTimeOut = 0x00;
static uint32_t timeOutPeriod = 0x00;
static uint32_t totalImageSize = 0x00000000;
static uint16_t hardwareVersion = 0xFFFF;
static OtaImageInfo_t currentDownloadFile;
static uint8_t errors = 0;
static uint8_t pageErrors = 0x00;
static uint8_t PageBufferId = 0xFF;

static bool usePageRequest = g_FALSE_c;
#if (g_OTA_SERVICE_DISCOVERY_CMD_d == 1)
static uint8_t m_startUpgrade = g_FALSE_c;
#endif /* (g_OTA_SERVICE_DISCOVERY_CMD_d == 1) */
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
static authorizedSigner allowedSigners[g_MAX_SIGNERS_c] = { 0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11 };
static IssuerCAKeys_t issuerCAkeys[g_MAX_CA_KEYS_c] = {
 0x54,0x45,0x53, 0x54,0x53,0x45,0x43,0x41,
 0x02,0x00,0xfd,0xe8,0xa7,0xf3,0xd1,0x08,0x42,0x24,0x96,
 0x2a,0x4e,0x7c,0x54,0xe6,0x9a,0xc3,0xf0,0x4d,0xa6,0xb8 };
#endif
/* Payload and Jitter only, and MFG ID, and Image Type ID, and Version */
static const uint8_t imageNotifyPayloadLengths[] = { 2, 4, 6, 10 };

static const uint8_t otaStateToUpgradeStatus[] = { g_Normal_c, g_Normal_c,
    g_Normal_c, g_Normal_c, g_Normal_c, g_DownloadInprogress_c,
    g_DownloadComplete_c, g_WaitingToUpgrade_c, g_CountDown_c };

/* Image Notify, Query Next Image Request, Query Next Image Response
 Image Block Request, Image Page Request, Image Block Response, Upgrade End Request
 Upgrade End Response, Run Upgrade Request, Query Specific File Request
 Query Specific File Response */

static const uint8_t OtaMinMessageLengths[] = { 2, 8, 1, 13, 13, 1, 1, 1, 18, 1 };
pageDataMissedInfo_t *pPageMissInfo = 0;

#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

#if (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 1)
static uint32_t g_OTA_NEXT_IMAGE_QUERY_DELAY_c = 120u * g_MS_IN_SEC_c;

static uint16_t m_ZIGBEE_PRO_STACK_VERSION = g_ZIGBEE_PRO_STACK_VERSION_c;

static uint32_t g_REQUEST_TIME_DELAY_c =  ((uint32_t)(0.5 * g_MS_IN_SEC_c));
#endif /* (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 1) */





bool App_verfyImageSignature( uint32_t imageSize );
/***************************************************************************************
* Function Prototypes
***************************************************************************************/
static void App_otaUpgradeBtldrMain( void );

static void App_queryNextImage( bool startTimer );

static void App_startDownload( uint32_t newVersion );

void App_downLoadedVerifyAndFinish( App_otaStatus_t status );

void App_sendRequest( uint8_t cmdId, ZCL_Status upgrdEndStatus, uint32_t timer );

static pageDataMissedInfo_t *App_getPageMissedTable( void );

static void App_resetPageRequestVars( void );

static void App_resetMissedPacketTable( uint32_t offset );

static void App_otaUpgradeserviceDiscovery( void );
#if(g_ZIGBEE_SECURITY_d == 1)
static void App_otaUpgradeGetPartnerAppLinkKey( void );
#endif
static void App_continueImageDownload( void );

static void App_askServerToRunUpgrade( bool timer );

static void App_verifyDownLoadedImage( void );

static void App_runUpgrade( void );

static void App_updateUpgradeStatus( uint8_t state );

static void App_updateCurrentOffset( uint32_t offset );

static void App_updateDownloadFileVersion( uint32_t firmwareVersion );

static ZCL_Status App_handleImageNotify( ImageNotify_t *imageNotify,
                                         uint8_t length, bool broadcast );

static ZCL_Status App_handlQueryNextImageResponse(
                QueryNextImageResponse_t *imageNotify,  uint8_t length );

static ZCL_Status App_handleImageBlockResponse(
                     ImageBlockResponse_t *imageNotify, uint8_t length );

static ZCL_Status App_handleUpgradeEndResponse(
       ImageUpgradeEndResponse_t*imageUpEndRsp,uint8_t length, ZCL_Status status );

static ZCL_Status App_handleQuerySpecificFileResponse(
              QuerySpecificFileResponse_t *querySpecFileRsp, uint8_t length );

static bool App_calculateTimer( uint32_t currentTime, uint32_t targetTime,
                                uint32_t* returnTimeMs );

static bool App_otaClientVerifyImage (void);

static void App_otaUpadateImagestate (void);
void App_resetBootloaderFirmwareInfo (void);
/****************************************************************************************
* Public Functions
****************************************************************************************/
#if (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 1)
void App_setRequestTimeDelay (uint32_t delay)
{
  g_REQUEST_TIME_DELAY_c = delay;
}
#endif //(g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 1)
/**************************************************************************************/
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )

bool App_UpdateSignerIEEEAddress(uint8_t *ieeeAddress, uint8_t index)
{
     if( index < g_MAX_SIGNERS_c  ){
        memUtils_memCopy(allowedSigners[index],  ieeeAddress, g_MAX_ADDR_LENGTH_c );
        return g_TRUE_c;
    }
    return g_FALSE_c;

}
#endif
/**************************************************************************************/

static void App_resumeOtaUpgradeAfterDelay( uint32_t timeOut )
{
    bootLoaderWaitstate = g_TRUE_c;
    lastTickTime = halCommonGetInt32uMillisecondTick();
    if ( waitingForResponse == g_TRUE_c ) {
        timeOutPeriod = timeOut;
        nextTimeOut = 0;
    }
    else {
        timeOutPeriod = timeOut;
        nextTimeOut = 0;
    }
}
/**************************************************************************************/
void App_otaUpgradeClientBtldrInit( void )
{
    serverEndpoint= g_UNDEFINED_ENDPOINT_c;
    myEndpoint = ZCL_Callback_Get_Device_EndPoint();
    serverNodeId = g_UNKNOWN_NODE_ID_c;
    lastTickTime = timeOutPeriod = 0x00;
    clientOtaState = APP_CLIENT_BTLDR_INIT;
    bootLoaderWaitstate = g_FALSE_c;
    App_resetPageRequestVars();
    pageErrors = errors = 0x00;
#ifndef	STM32L1XX_MD_PLUS
    eeprom_init();
#else
    sEE_Init();
#endif
#if (g_OTA_SERVICE_DISCOVERY_CMD_d == 1)
    m_startUpgrade = g_FALSE_c;
#endif /* (g_OTA_SERVICE_DISCOVERY_CMD_d == 1) */
}
/**************************************************************************************/
void App_otaUpgradeClientBtldrTask( void )
{
    volatile uint32_t currentTime = halCommonGetInt32uMillisecondTick();
    if ( bootLoaderWaitstate == g_TRUE_c ) {
        if ( ( currentTime - lastTickTime ) >= timeOutPeriod ) {
            bootLoaderWaitstate = g_FALSE_c;
            if ( waitingForResponse == g_TRUE_c ) {
                errors++;
                waitingForResponse = g_FALSE_c;
            }
            if ( nextTimeOut && (nextTimeOut > timeOutPeriod) ) {
                App_resumeOtaUpgradeAfterDelay( nextTimeOut - timeOutPeriod );
            }
        }
    }
    else {
        App_otaUpgradeBtldrMain();
    }
}
/**************************************************************************************/
void App_startOtaServiceDiscovery( void )
{
    if ( getBootLoadState() == APP_CLIENT_BTLDR_INIT ) {
        clientOtaState = APP_CLIENT_BTLDR_DISCOVER_SERVER;
    }
}
/**************************************************************************************/
static void App_otaUpgradeBtldrMain ( void )
{

    switch( getBootLoadState() ) {
    case APP_CLIENT_BTLDR_INIT:
        break;
    case APP_CLIENT_BTLDR_DISCOVER_SERVER :
        App_otaUpgradeserviceDiscovery();
        break;
    case APP_CLIENT_BTLDR_GET_SERVER_EUI :
        App_otaUpgradeserviceDiscovery();
        break;
    case APP_CLIENT_BTLDR_APP_LINK_KEY :
#if(g_ZIGBEE_SECURITY_d == 1)
        App_otaUpgradeGetPartnerAppLinkKey();
#endif
        break;
    case APP_CLIENT_BTLDR_QUERY_NEXT_IMAGE :
#if (g_OTA_SERVICE_DISCOVERY_CMD_d == 1u)
        if (m_startUpgrade == g_TRUE_c) {
#endif /* (g_OTA_SERVICE_DISCOVERY_CMD_d == 1u) */
            App_queryNextImage( g_TRUE_c );

#if (g_OTA_SERVICE_DISCOVERY_CMD_d == 1u)
        }
#endif /* (g_OTA_SERVICE_DISCOVERY_CMD_d == 1u) */
        break;
    case APP_CLIENT_BTLDR_DOWNLOAD :
        App_continueImageDownload();
        break;
    case APP_CLIENT_BTLDR_VERIFY_IMAGE :
        App_verifyDownLoadedImage();
        break;
    case APP_CLIENT_BTLDR_WAITING_FOR_UPGRADE_MESSAGE :
        App_askServerToRunUpgrade( g_TRUE_c );
        break;
    case APP_CLIENT_BTLDR_COUNTDOWN_TO_UPGRADE :
        App_runUpgrade();
        break;
    default :
        break;
    }
}

/**************************************************************************************/
void App_otaUpgradeClientEventHandler( ZCL_Event_t *pZCLEvent,
                                       APSDE_Data_Indication_t *pBuffer )
{
    ZCL_Status status;
    /* If recevied command atleast min length of the and from
       authenticated device then process else send default response */
    if ( pZCLEvent->eventLength < OtaMinMessageLengths[pBuffer->a_asdu[2]] ) {
        status = g_ZCL_Malformed_Command_c;
    }
    else if ( ZCL_Callback_Get_Device_ShortAddress() != serverNodeId ) {
        status = g_ZCL_Not_Authorized_c;
    }
    else {
        /* Response is received so make the flage to Reset so that error counter
        will not incremented and call the handling functions */
        waitingForResponse = g_FALSE_c;
        switch ( pZCLEvent->eventId ) {

        case g_ImageNotify_Event_c:
            status = App_handleImageNotify( (ImageNotify_t *)pZCLEvent->pEventData,
                     pZCLEvent->eventLength, pBuffer->dest_address.short_address > 0xFFFB );
            break;

        case g_QueryNextImageResponse_Event_c:
            status = App_handlQueryNextImageResponse(
            (QueryNextImageResponse_t*)pZCLEvent->pEventData, pZCLEvent->eventLength );
             break;

        case g_ImageBlockResponse_Event_c :
            status = App_handleImageBlockResponse(
            (ImageBlockResponse_t*)pZCLEvent->pEventData, pZCLEvent->eventLength );
            break;

        case g_UpgradeEndResponse_Event_c:
            status = App_handleUpgradeEndResponse(
            (ImageUpgradeEndResponse_t*)pZCLEvent->pEventData, pZCLEvent->eventLength,
                g_ZCL_Success_c );
            break;

        case g_QuerySpecificFileResponse_Event_c:
            App_handleQuerySpecificFileResponse(
              (QuerySpecificFileResponse_t*)pZCLEvent->pEventData, pZCLEvent->eventLength );
            break;

        default :
            status = g_ZCL_UnSup_Cluster_Command_c;
            break;
        }
    }
    if ( status != g_ZCL_Success_c ) {
        App_SendDefaultResponse( status, g_FALSE_c, pBuffer );
    }
}

/*****************************************************************************************/
void App_getOtaUpgradeImageInformation( OtaImageInfo_t *ImageInfo )
{
    ImageInfo->manufacturerId = currentDownloadFile.manufacturerId;
    ImageInfo->imageTypeId = currentDownloadFile.imageTypeId;
    ImageInfo->firmwareVersion = currentDownloadFile.firmwareVersion;
}

void App_setOtaUpgradeImageInformation( OtaImageInfo_t *ImageInfo )
{
    currentDownloadFile.manufacturerId = ImageInfo->manufacturerId;
    currentDownloadFile.imageTypeId = ImageInfo->imageTypeId;
    currentDownloadFile.firmwareVersion = ImageInfo->firmwareVersion;
}

/*****************************************************************************************/
static void App_parseImageInfoFromMessage(uint8_t *message, OtaImageInfo_t *OtaImageInfo )
{
    OtaImageInfo->manufacturerId = *((uint32_t*)message);
    OtaImageInfo->imageTypeId = *((uint32_t*)(message+2));
    OtaImageInfo->firmwareVersion = *((uint32_t*)(message+4));
}

/*****************************************************************************************/
static void App_updateUpgradeStatus( uint8_t state )
{
    uint8_t upgradeStatus = otaStateToUpgradeStatus[state];

    App_ReadWriteClientAttribute( myEndpoint,  g_OTA_UPGRADE_CLUSTER_c,
    g_IMAGE_UPGRADE_STATUS_c, (uint8_t*)&upgradeStatus, g_Write_Attribute_Data_c );
    clientOtaState = state;
    bootLoaderWaitstate = g_FALSE_c;

}

/**************************************************************************************/
static void App_updateDownloadFileVersion( uint32_t firmwareVersion )
{
#if ( g_DOWNLOADED_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
     g_DOWNLOADED_FILE_VERSION_c,(uint8_t*)&firmwareVersion, g_Write_Attribute_Data_c );
#endif
  currentDownloadFile.firmwareVersion = firmwareVersion;
}

/**************************************************************************************/
static void App_updateCurrentImageAttributes( OtaImageInfo_t* imageId )
{
#if ( g_CURRENT_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint,  g_OTA_UPGRADE_CLUSTER_c,
       g_MANUFACTURER_ID_c, (uint8_t*)&imageId->manufacturerId, g_Write_Attribute_Data_c );
#endif
#if ( g_IMAGE_TYPE_ID_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
        g_IMAGE_TYPE_ID_c, (uint8_t*)&imageId->imageTypeId, g_Write_Attribute_Data_c );
#endif
#if ( g_CURRENT_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
      g_CURRENT_FILE_VERSION_c,(uint8_t*)&imageId->firmwareVersion, g_Write_Attribute_Data_c );
#endif
}

/**************************************************************************************/
static uint32_t App_getCurrentOffset( void )
{
    uint32_t offset = 0;
    App_ReadWriteClientAttribute( myEndpoint,  g_OTA_UPGRADE_CLUSTER_c,
                g_FILE_OFFSET_c, (uint8_t*)&offset, g_Read_Attribute_Data_c );
   return offset;
}

/**************************************************************************************/
static void App_updateCurrentOffset( uint32_t offset )
{
    App_ReadWriteClientAttribute( myEndpoint,
      g_OTA_UPGRADE_CLUSTER_c, g_FILE_OFFSET_c,(uint8_t*)&offset,
      g_Write_Attribute_Data_c );
}
/**************************************************************************************/
bool App_WriteImagetoEEprom( uint32_t offset, uint8_t datasize, uint8_t* data )
{
    uint8_t status;

    status = eeprom_writeBuffer( g_START_DOWNLOAD_OFFSET_c+offset, data, datasize );

    if (status != g_SUCCESS_c ) {
        return g_FALSE_c;
    }
    return g_TRUE_c;

}

/**************************************************************************************/
static bool App_calculateTimer( uint32_t currentTime, uint32_t targetTime,
                                uint32_t* returnTime )
{
    if ( targetTime < currentTime ) {
        return g_FALSE_c;
    }
    else {
        *returnTime = targetTime - currentTime;
    }
    if ( *returnTime > g_TIMEOUT_MAX_WAIT_TIME_c ) {
        *returnTime = g_TIMEOUT_MAX_WAIT_TIME_c;
    }
    return g_TRUE_c;
}
/*****************************************************************************************/
static ZCL_Status App_handleImageNotify( ImageNotify_t *imageNotify,
                                      uint8_t length, bool broadcast )
{
    OtaImageInfo_t myInfo;
    uint8_t random = 0;

    if ( getBootLoadState() != APP_CLIENT_BTLDR_QUERY_NEXT_IMAGE ) {
        return g_ZCL_Failure_c;
    }
    else if ( ! broadcast ) {
        App_getOtaUpgradeImageInformation( &myInfo );
        if ( imageNotify->payLoadType > g_QueryJitterManufCodeImagetypeAndNewFileVersion_c ) {
            return g_ZCL_Success_c;
        }
        if ( imageNotify->queryJitter < 1 || imageNotify->queryJitter > 100 ) {
            return g_ZCL_Success_c;
        }
        if ( length != imageNotifyPayloadLengths[imageNotify->payLoadType] ) {
            return g_ZCL_Success_c;
        }
        if ( imageNotify->payLoadType >= g_QueryJitterAndManufCode_c ) {
            if ( *((uint16_t*)imageNotify->manufacturerCode) != myInfo.manufacturerId ) {
                return g_ZCL_Success_c;
            }
        }
        if ( imageNotify->payLoadType >= g_QueryJitterManufCodeAndImagetype ) {
            if (*((uint16_t*)imageNotify->imageType) != myInfo.imageTypeId) {
                return g_ZCL_Success_c;
            }
        }
        if ( imageNotify->payLoadType >= g_QueryJitterManufCodeImagetypeAndNewFileVersion_c  ) {
            if ( *((uint32_t*)imageNotify->newFileVersion) == myInfo.firmwareVersion ) {
                return g_ZCL_Success_c;
            }
        }
        random = ( randomT()%100 ) + 1;
        if( random > imageNotify->queryJitter ) {
            return g_ZCL_Success_c;
        }
    }
    App_queryNextImage( g_TRUE_c );
    return g_ZCL_Success_c;
}

/**************************************************************************************/
static ZCL_Status App_handlQueryNextImageResponse(
                    QueryNextImageResponse_t* queryNextImageRsp, uint8_t length )
{
    ZCL_Status status;
    OtaImageInfo_t imageinfo;

    /* If u are not in Next query image state or Response is not success
       then just ingore the packet */
    if ( getBootLoadState() != APP_CLIENT_BTLDR_QUERY_NEXT_IMAGE ) {
        return g_ZCL_Success_c;
    }
    else if ( queryNextImageRsp->status != g_ZCL_Success_c ) {
        status = g_ZCL_Success_c;
    }
    else if ( length < g_QUERY_NEXT_IMAGE_SUCCESS_RESPONSE_MIN_LENGTH_c ) {
        /* Packet len is lesser then expected */
        status = g_ZCL_Malformed_Command_c;
    }
    else {
        /* Check received image firmware version is not equal to current
        version then start down load else ignore */
        App_parseImageInfoFromMessage( queryNextImageRsp->manufacturerCode,
                                      &imageinfo );
        if ( ( imageinfo.manufacturerId != currentDownloadFile.manufacturerId ) ||
             ( imageinfo.imageTypeId != currentDownloadFile.imageTypeId )  ||
             ( imageinfo.firmwareVersion == currentDownloadFile.firmwareVersion ) ||
               (*((uint32_t*)queryNextImageRsp->imageSize)) == 0 ) {
            status = g_ZCL_Malformed_Command_c;
        }
        else {
            totalImageSize = *((uint32_t*)queryNextImageRsp->imageSize);
            App_updateDownloadFileVersion( imageinfo.firmwareVersion );
            App_startDownload( imageinfo.firmwareVersion );
            return g_ZCL_Success_c;
        }
    }
    /* If u are not moving to image down load state then
       send Next image query after configured time */
    App_queryNextImage( g_FALSE_c );
    return status;
}

/*******************************************************************************************/
static void App_resetPageRequestVars( void )
{
    pageRequestState = APP_OTA_NO_PAGE_REQUEST;
    bootLoaderWaitstate = g_FALSE_c;
    if ( PageBufferId != g_NO_AVAILABLE_BUFFER_c ) {
        buffMgmt_freeBuffer( PageBufferId, 144 );
        pPageMissInfo = 0;
    }
}

/*******************************************************************************************/
uint16_t App_initPageRequest( uint32_t currentOffset )
{
    /* Allocate buffer for keep track of missed packet and
     Intit the all page request variable to correct valus */
    PageBufferId = buffMgmt_allocateBuffer( g_LARGE_BUFFER_c );
    pPageMissInfo = App_getPageMissedTable();
    if ( PageBufferId == g_NO_AVAILABLE_BUFFER_c ) {
        return g_OTA_CLIENT_PAGE_REQUEST_INIT_ERROR_DELAY;
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 57 */
        allocatedbuffer[PageBufferId]= 57;
        freebuffer[PageBufferId][0]= 57;
        freebuffer[PageBufferId][1]= 0;
#endif
    App_resetMissedPacketTable( g_RESET_ALL_ENTRIES_c );
    pageRequestState = APP_OTA_WAITING_PAGE_REQUEST_REPLIES;
    pageRequestOffset = currentOffset;
    currentPageSize = ((totalImageSize - currentOffset) > g_OTA_CLIENT_PAGE_REQUEST_SIZE_c) ? g_OTA_CLIENT_PAGE_REQUEST_SIZE_c  :( totalImageSize - currentOffset );
    packetSize = 0;
    return g_OTA_CLIENT_PAGE_RESPONSE_MAX_INTER_FRAME_DELAY;
}

/*******************************************************************************************/
static bool App_addOrGetMissedBlockOffset( uint32_t *offset, bool retrieve )
{
    pageDataMissedInfo_t *pPageMissInfo = App_getPageMissedTable();
    uint8_t index;
    /* Loop through to add or get the missed packet info till all table entries */
    for ( index = 0; index < g_OTA_PAGE_REQ_BLOCK_MISS_THRESHOLD_c; index++ ) {
        if ( pPageMissInfo->inUse == retrieve ) {
            if ( retrieve ) {
                *offset = pPageMissInfo->fileOffeset;
            }
            else {
                pPageMissInfo->inUse = g_TRUE_c;
                pPageMissInfo->fileOffeset = *offset;
            }
            return g_TRUE_c;
        }
        pPageMissInfo++;
    }
    return g_FALSE_c;
}

/*******************************************************************************************/
static pageDataMissedInfo_t *App_getPageMissedTable( void )
{
    if ( PageBufferId != g_NO_AVAILABLE_BUFFER_c ) {
         return (pageDataMissedInfo_t*)buffMgmt_getBufferPointer( PageBufferId );
    }
    else {
        return g_NULL_c;
    }
}
/*******************************************************************************************/
static bool App_checkPageRequestAllPacketReceived( void )
{
     uint8_t index;
     pageDataMissedInfo_t *pPageMissInfo = App_getPageMissedTable();
     for ( index = 0; index < g_OTA_PAGE_REQ_BLOCK_MISS_THRESHOLD_c; index++ ) {
         if ( pPageMissInfo->inUse == g_TRUE_c ) {
             return g_FALSE_c;
         }
         pPageMissInfo++;
     }
     return g_TRUE_c;
}

/*******************************************************************************************/
static void App_resetMissedPacketTable( uint32_t offset )
{
    uint8_t index;
    pageDataMissedInfo_t *pPageMissInfo = App_getPageMissedTable();
    for ( index = 0; index < g_OTA_PAGE_REQ_BLOCK_MISS_THRESHOLD_c; index++ ) {
        if ( offset == g_RESET_ALL_ENTRIES_c ) {
            pPageMissInfo->inUse = g_FALSE_c;
        }
        else if ( pPageMissInfo->inUse && pPageMissInfo->fileOffeset == offset ) {
            pPageMissInfo->inUse = g_FALSE_c;
            break;
        }
        pPageMissInfo++;
    }
}
/*******************************************************************************************/
ClientPageRequestState_t App_receiveBlockRspForPageRequest( uint32_t currentOffset,
                                       uint32_t receivedOffset, uint8_t size )
{
    ClientPageRequestState_t status = pageRequestState;

    if ( receivedOffset > currentOffset ) {
        if ( !App_addOrGetMissedBlockOffset( &currentOffset, g_FALSE_c ) ) {
            if ( pageErrors < g_OTA_PAGE_REQUEST_RETRIES_THRESHOLD_c ) {
                status = APP_OTA_RETRY_PAGE_REQUEST;
                pageErrors++;
            }
            else {
                 status = APP_OTA_PAGE_REQUEST_ERROR;
            }
        }
    }
    else if ( receivedOffset + size >= pageRequestOffset + currentPageSize ) {
        if ( App_checkPageRequestAllPacketReceived() ) {
            status = APP_OTA_PAGE_REQUEST_COMPLETE;
        }
        else {
            pageRequestState = status = APP_OTA_RETRY_MISSED_PACKETS;
            return status;
        }
    }
    if ( pageRequestState == APP_OTA_RETRY_MISSED_PACKETS ) {
        App_resetMissedPacketTable( receivedOffset );
        if ( App_checkPageRequestAllPacketReceived() ) {
            status = APP_OTA_PAGE_REQUEST_COMPLETE;
        }
    }
    return status;
}
/*******************************************************************************************/
static ZCL_Status App_handleImageBlockResponse( ImageBlockResponse_t *pbuffer,
                                                uint8_t length )
{
    OtaImageInfo_t imageInfo;
    uint32_t currentOffset;
    uint8_t status = pbuffer->status;
    uint32_t nextOffset;
    uint32_t offset;
    ImageBlockRspSuccess_t * ImageBlockRsp;

    if ( status == g_ZCL_Wait_For_Data_c ) {
        uint32_t calculatedTimer;
        ImageBlockRspWaitForData_t * RspWaitForData;

        RspWaitForData = &pbuffer->ImageBlockRspPayload.ImageBlockRspWaitForData;

        if ( length < g_IMAGE_BLOCK_RESPONSE_WAIT_FOR_DATA_LENGTH_c ) {
            return g_ZCL_Malformed_Command_c;
        }
        if (!App_calculateTimer( *((uint32_t*)RspWaitForData->currentTime),
                        *((uint32_t*)RspWaitForData->requestTime), &calculatedTimer ) ) {
            calculatedTimer = g_CALCULATE_TIME_ERROR_IMAGE_BLOCK_DELAY_MIN_c;
        }
        if (calculatedTimer != g_CALCULATE_TIME_ERROR_IMAGE_BLOCK_DELAY_MIN_c) {
            calculatedTimer *= g_MS_IN_SEC_c;
        }
        App_resumeOtaUpgradeAfterDelay( calculatedTimer );
        return g_ZCL_Success_c;
    }
    else if ( status == g_ZCL_Abort_c || status == g_ZCL_No_Image_Available_c ) {
        App_downLoadedVerifyAndFinish( APP_OTA_SERVER_ABORTED );
        return g_ZCL_Success_c;
    }
    else if ( status == g_ZCL_UnSup_Cluster_Command_c ) {
        App_setOrResetPageRequest( g_FALSE_c );
        App_queryNextImage( g_TRUE_c );
        return g_ZCL_Success_c;
    }
    else if ( status != g_ZCL_Success_c ) {
        return g_ZCL_Invalid_Value_c;
    }
    ImageBlockRsp = &pbuffer->ImageBlockRspPayload.ImageBlockRspSuccess;
    offset = *((uint32_t*)ImageBlockRsp->fileOffset);
    nextOffset = offset + ImageBlockRsp->dataSize;
    if ( ( length < g_IMAGE_BLOCK_RESPONSE_SUCCESS_MIN_LENGTH_c ) ||
         ( (length - sizeof(ImageBlockRspSuccess_t)) < ImageBlockRsp->dataSize ) ||
         ( nextOffset > totalImageSize ) ) {
        return g_ZCL_Malformed_Command_c;
    }
    currentOffset = App_getCurrentOffset();
    App_parseImageInfoFromMessage( ImageBlockRsp->manufacturerCode,
                                   &imageInfo );
    if ( !usePageRequest  || ( pageRequestState == APP_OTA_RETRY_MISSED_PACKETS ) ) {
        if ( offset != currentOffset ) {
            return g_ZCL_Success_c;
        }
    }
    if ( ( currentDownloadFile.manufacturerId != imageInfo.manufacturerId ) ||
         ( currentDownloadFile.imageTypeId != imageInfo.imageTypeId ) ||
         ( currentDownloadFile.firmwareVersion != imageInfo.firmwareVersion ) ) {
            return g_ZCL_Invalid_Value_c;
    }
    if (!App_WriteImagetoEEprom( currentOffset,ImageBlockRsp->dataSize,ImageBlockRsp->imageData ) ) {
        /* If not able write eeprom then abort Ota */
        return g_ZCL_Abort_c;
    }
    if ( usePageRequest ) {
        packetSize = packetSize == 0 ? ImageBlockRsp->dataSize : packetSize;
        status = App_receiveBlockRspForPageRequest( currentOffset, offset,
                        ImageBlockRsp->dataSize );
        if ( status == APP_OTA_PAGE_REQUEST_ERROR ) {
            App_downLoadedVerifyAndFinish( APP_OTA_CLIENT_ABORTED );
        }
        else if ( status == APP_OTA_RETRY_PAGE_REQUEST ) {
             App_resetPageRequestVars();
             App_updateCurrentOffset( pageRequestOffset );
             return g_ZCL_Success_c;
        }
        else if ( status == APP_OTA_WAITING_PAGE_REQUEST_REPLIES ) {
            App_updateCurrentOffset( nextOffset );
            App_resumeOtaUpgradeAfterDelay( g_OTA_CLIENT_PAGE_RESPONSE_MAX_INTER_FRAME_DELAY );
            return g_ZCL_Success_c;
        }
        else if ( status == APP_OTA_PAGE_REQUEST_COMPLETE ) {
               pageErrors = 0x00;
               nextOffset = pageRequestOffset + currentPageSize;
               App_resetPageRequestVars();
               App_resumeOtaUpgradeAfterDelay( g_MESSAGE_RESPONSE_TIMEOUT_SECONDS_c );
        }
        else {
            App_resumeOtaUpgradeAfterDelay( g_OTA_PAGE_REQUEST_MISSED_PACKET_DELAY_c );
            return g_ZCL_Success_c;
        }
    }
    errors = 0;
    App_updateCurrentOffset( nextOffset );
    App_resumeOtaUpgradeAfterDelay( g_REQUEST_TIME_DELAY_c );
    if ( nextOffset >= totalImageSize ) {
        changeBootLoadState ( APP_CLIENT_BTLDR_VERIFY_IMAGE );
        App_resumeOtaUpgradeAfterDelay( g_REQUEST_TIME_DELAY_c );
    }
    return g_ZCL_Success_c;
}

/*******************************************************************************************/
void App_downLoadedVerifyAndFinish( App_otaStatus_t status )
{
    ZCL_Status zclStatusOtaStatus[] = {
    g_ZCL_Success_c, g_ZCL_Abort_c, g_ZCL_Invalid_Image_c, g_ZCL_Abort_c, g_ZCL_Abort_c};
    ZCL_Status zclStatus = zclStatusOtaStatus[status];

    if ( zclStatus == g_ZCL_Success_c ) {
        App_askServerToRunUpgrade( g_FALSE_c );
    }
    else if ( status != APP_OTA_SERVER_ABORTED ) {
         App_sendRequest( g_UpgradeEndRequest_c,
                     zclStatus, g_RUN_UPGRADE_REQUEST_DELAY_MIN_c );
         App_resetBootloaderFirmwareInfo();
         App_queryNextImage( g_FALSE_c );
    }
    else {
        App_resetBootloaderFirmwareInfo();
    }
    pageErrors = 0x00;
    App_resetPageRequestVars();
}

/*******************************************************************************************/
static ZCL_Status App_handleUpgradeEndResponse( ImageUpgradeEndResponse_t *imageUpEndRsp,
                                     uint8_t length, ZCL_Status status )
{
  OtaImageInfo_t OtaImageInfo;
  uint32_t waitTime = 0;
  uint32_t currentTime, upgradeTime;

    if ( status ) {
        App_downLoadedVerifyAndFinish( APP_OTA_SERVER_ABORTED );
        return g_ZCL_Success_c;
    }
    if ( length < g_UPGRADE_END_RESPONSE_MIN_LENGTH_c ) {
        return g_ZCL_Malformed_Command_c;
    }
    App_parseImageInfoFromMessage( imageUpEndRsp->manufacturerCode,
                                   &OtaImageInfo );

    if ( ( OtaImageInfo.manufacturerId != currentDownloadFile.manufacturerId ) &&
         ( OtaImageInfo.manufacturerId != g_MFG_ID_WILD_CARD_c ) ) {
        return g_ZCL_Invalid_Value_c;
    }
    if ( ( OtaImageInfo.imageTypeId != currentDownloadFile.imageTypeId ) &&
         ( OtaImageInfo.imageTypeId != g_IMAGE_TYPE_WILD_CARD_c ) ) {
        return g_ZCL_Invalid_Value_c;
    }
    if ( ( OtaImageInfo.firmwareVersion != currentDownloadFile.firmwareVersion ) &&
         ( OtaImageInfo.firmwareVersion != g_FILE_VERSION_WILD_CARD_c ) ) {
        return g_ZCL_Invalid_Value_c;
    }
    currentTime = *((uint32_t*)imageUpEndRsp->currentTime);
    upgradeTime = *((uint32_t*)imageUpEndRsp->upgradeTime);
    if ( g_WAIT_FOREVER_UPGRADE_MESSAGE_c == upgradeTime ) {
        /* If server told to wait forever. client wake up and ask for server
           when to upgrade */
        App_updateUpgradeStatus( APP_CLIENT_BTLDR_WAITING_FOR_UPGRADE_MESSAGE );
        App_resumeOtaUpgradeAfterDelay( g_WAIT_FOR_UPGRADE_DELAY_HR_c );
        return g_ZCL_Success_c;
    }
    if (!App_calculateTimer( currentTime, upgradeTime, &waitTime ) ) {
        waitTime = g_CALCULATE_TIME_ERROR_UPGRADE_END_RESPONSE_DELAY_HR_c;
    }
    else {
        /* Upgrade Immediataly with small delay any to avoid packet retries
           over the air */
        if ( waitTime < g_OTA_IMMEDIATE_UPGRADE_DELAY_c ) {
            waitTime = g_OTA_IMMEDIATE_UPGRADE_DELAY_c;
        }
        waitTime *= g_MS_IN_SEC_c ;
    }
    App_updateUpgradeStatus( APP_CLIENT_BTLDR_COUNTDOWN_TO_UPGRADE );
    App_resumeOtaUpgradeAfterDelay( waitTime );
    return g_ZCL_Success_c;
}

/*******************************************************************************************/
static ZCL_Status App_handleQuerySpecificFileResponse(
                        QuerySpecificFileResponse_t *querySpecFileRsp, uint8_t length )
{
    ZCL_Status status;
    OtaImageInfo_t imageinfo;

    if ( querySpecFileRsp->status != g_ZCL_Success_c ) {
        status = g_ZCL_Success_c;
    }
    else if ( length < g_QUERY_SPECIFIC_FILE_RESPONSE_SUCCESS_LENGTH_c ) {
        status = g_ZCL_Malformed_Command_c;
    }
    else {
        totalImageSize = *((uint32_t*)querySpecFileRsp->imageSize);
        App_parseImageInfoFromMessage( querySpecFileRsp->manufacturerCode,
                                       &imageinfo );
        if ( ( imageinfo.manufacturerId != currentDownloadFile.manufacturerId ) ||
             ( imageinfo.imageTypeId != currentDownloadFile.imageTypeId )  ||
             ( imageinfo.firmwareVersion == currentDownloadFile.firmwareVersion ) ||
               totalImageSize == 0 ) {
              status = g_ZCL_Malformed_Command_c;
        }
        else {
            App_updateDownloadFileVersion( imageinfo.firmwareVersion );
            App_startDownload( imageinfo.firmwareVersion );
            App_resumeOtaUpgradeAfterDelay( g_REQUEST_TIME_DELAY_c );
            status = g_ZCL_Success_c;
        }
    }
    return status;
}

/*******************************************************************************************/
static void App_askServerToRunUpgrade( bool timer )
{
    if ( ! timer ) {
        errors = 0;
        App_otaUpadateImagestate();
    }
    if ( errors >= g_OTA_CLIENT_UPGRADE_WAIT_THRESHOLD_c ) {
        App_runUpgrade();
        return;
    }
    App_updateUpgradeStatus( APP_CLIENT_BTLDR_WAITING_FOR_UPGRADE_MESSAGE );
    App_sendRequest( g_UpgradeEndRequest_c,
                     g_ZCL_Success_c, g_RUN_UPGRADE_REQUEST_DELAY_MIN_c );
}

/*******************************************************************************************/
static void App_runUpgrade( void )
{
//    if ( ZigBeeNetworkState() == g_ZigBeeJoinedNWK_c ) {
//        ZigBeeLeaveNetwork();
//        App_resumeOtaUpgradeAfterDelay(10000);
		#ifndef	STM32L1XX_MD_PLUS
			halReboot();
		#else
			NVIC_SystemReset();
		#endif
//    }
//    else {
//        halReboot();
//        App_queryNextImage( g_FALSE_c );
//    }
}

/*******************************************************************************************/
static void App_verifyDownLoadedImage( void )
{
    App_otaStatus_t status;
    App_updateUpgradeStatus( APP_CLIENT_BTLDR_VERIFY_IMAGE );
    status = (App_otaClientVerifyImage() == g_TRUE_c) ?\
              APP_OTA_IMAGE_VERIFY_SUCCESS : APP_OTA_IMAGE_VERIFY_FAILED;
    /* TBD Vefify the image is send to next Function */
    App_downLoadedVerifyAndFinish( status );
}

/*******************************************************************************************/
static void App_continueImageDownload( void )
{
    uint8_t cmdId = g_ImageBlockRequest_c;
    uint32_t timer = g_REQUEST_TIME_DELAY_c;
    bool send = g_TRUE_c;
    uint32_t offset;
    App_updateUpgradeStatus( APP_CLIENT_BTLDR_DOWNLOAD );

//    timer += randomT();
//    if (timer >
//        (g_OTA_CLIENT_BLOCK_REQ_MAX_JITTER_c + g_OTA_CLIENT_DOWNLOAD_DELAY_c)) {
//        timer %= (uint32_t)g_OTA_CLIENT_BLOCK_REQ_MAX_JITTER_c;
//        if (timer < g_OTA_CLIENT_DOWNLOAD_DELAY_c) {
//            timer += g_OTA_CLIENT_DOWNLOAD_DELAY_c;
//        }
//    }

    if ( errors >= g_OTA_CLIENT_DOWNLOAD_ERROR_THRESHOLD_c ) {
        App_downLoadedVerifyAndFinish( APP_OTA_CLIENT_ABORTED );
        return;
    }
    if ( usePageRequest ) {

        if ( pageRequestState == APP_OTA_NO_PAGE_REQUEST ) {
            timer = App_initPageRequest( App_getCurrentOffset() );
            cmdId = g_ImagePageRequest_c;
            send = timer == g_OTA_CLIENT_PAGE_REQUEST_INIT_ERROR_DELAY ? g_FALSE_c : g_TRUE_c;
        }
        else if ( pageRequestState == APP_OTA_WAITING_PAGE_REQUEST_REPLIES ) {
            offset = App_getCurrentOffset();
            if ( App_addOrGetMissedBlockOffset( &offset, g_FALSE_c ) ) {
                if( (offset + packetSize) < (pageRequestOffset+currentPageSize) ) {
                    App_updateCurrentOffset ( offset + packetSize );
                    App_resumeOtaUpgradeAfterDelay( g_OTA_CLIENT_PAGE_RESPONSE_MAX_INTER_FRAME_DELAY );
                }
                else {
                    pageRequestState = APP_OTA_RETRY_MISSED_PACKETS;
                }
                return;
            }
            else {
                pageErrors++;
                if ( pageErrors < g_OTA_PAGE_REQUEST_RETRIES_THRESHOLD_c ) {
                    App_resetPageRequestVars();
                    App_updateCurrentOffset( pageRequestOffset );
                }
                else {
                    App_downLoadedVerifyAndFinish( APP_OTA_CLIENT_ABORTED );
                }
                return;
            }
        }
        else {
            if ( App_addOrGetMissedBlockOffset( &offset, g_TRUE_c ) ) {
                App_updateCurrentOffset( offset );
                timer = g_OTA_PAGE_REQUEST_MISSED_PACKET_DELAY_c;
            }
            else {
                send = g_FALSE_c;
            }
        }
    }
    if ( send ) {
        App_sendRequest( cmdId, g_ZCL_Success_c, timer );
    }
    else {
        App_resumeOtaUpgradeAfterDelay( timer );
    }
    return;
}
/*******************************************************************************************/
static void App_startDownload( uint32_t newVersion )
{
    App_updateDownloadFileVersion( newVersion );
    App_updateCurrentOffset(0);
    errors = pageErrors = 0;
    App_updateUpgradeStatus( APP_CLIENT_BTLDR_DOWNLOAD );
//    App_continueImageDownload();
}

/*******************************************************************************************/
static void App_queryNextImage( bool startTimer )
{
   App_updateUpgradeStatus( APP_CLIENT_BTLDR_QUERY_NEXT_IMAGE );

   if ( startTimer ) {
       errors = 0;
       App_getOtaUpgradeImageInformation( &currentDownloadFile );
       App_updateCurrentImageAttributes( &currentDownloadFile );
       App_sendRequest( g_QueryNextImageRequest_c,
                g_ZCL_Success_c, g_OTA_NEXT_IMAGE_QUERY_DELAY_c );
   }
//   else {
//       App_resumeOtaUpgradeAfterDelay( g_OTA_NEXT_IMAGE_QUERY_DELAY_c );
//   }
}
/***********************************************************************************************/
static void App_otaUpgradeserviceDiscovery( void )
{
  #if( g_SERVICE_DISCOVERY_ENABLE_d == 1 )
    if ( serviceDiscoveryInProgress() ) {
        return;
    }
  #endif //#if( g_SERVICE_DISCOVERY_ENABLE_d == 1 )
    if ( getBootLoadState() == APP_CLIENT_BTLDR_DISCOVER_SERVER ) {
        serverEndpoint = g_UNDEFINED_ENDPOINT_c;
        App_holdCurrentDiscovery( BOOTLOAD_DISCOVERY_STATE );
    }
    else if ( getBootLoadState() == APP_CLIENT_BTLDR_GET_SERVER_EUI ) {
        App_holdCurrentDiscovery( BOOTLOAD_DISCOVERY_SERVER_EUI );
    }
}

/******************************************************************************/
#if(g_ZIGBEE_SECURITY_d == 1)
static void App_otaUpgradeGetPartnerAppLinkKey( void )
{
    uint8_t tcEUI[8];
    uint8_t serverEui[8];
    uint8_t status;
    uint8_t nullEUI[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    if ( serverNodeId != App_getTCShortAddress() ) {
        App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
               g_UPGRADE_SERVER_ID_c, serverEui, g_Read_Attribute_Data_c );
        if ( ! ZigBeeHaveLinkKey ( serverEui  ) ) {
            status = ZigBeeGetIEEEAddrForSpecifiedShortAddr( App_getTCShortAddress(), tcEUI );
            if ( status && memUtils_memCompare( tcEUI, nullEUI, 0x08 ) ) {
                ZigBeeRequestLinkKey( tcEUI, serverEui );
            }
        }
    }
    changeBootLoadState( APP_CLIENT_BTLDR_QUERY_NEXT_IMAGE );
    App_resumeOtaUpgradeAfterDelay( g_OTA_APP_LINK_KEY_WAIT_SETUP_TIME_c );
}

#endif
/*****************************************************************************/
#if (g_OTA_SERVICE_DISCOVERY_CMD_d == 1u)
void App_otaUpgradeStart (void)
{
    if (APP_CLIENT_BTLDR_QUERY_NEXT_IMAGE == getBootLoadState()) {
        m_startUpgrade = g_TRUE_c;
    }
}
#endif /* (g_OTA_SERVICE_DISCOVERY_CMD_d == 1u) */

/*******************************************************************************************/
bool App_otaUpgradeServiceDiscoveryCallback( servicediscoveryresult_t *result )
{
    EndpointList_t* epList = (EndpointList_t*)result->responseData;
    if ( result->status == BROADCAST_SERVICE_DISCOVERY_COMPLETE ) {
        if ( serverEndpoint == g_UNDEFINED_ENDPOINT_c ) {
            App_resumeOtaUpgradeAfterDelay( g_OTA_SERVER_DISCOVERY_DELAY_c );
        }
    }
    else if ( result->status == BROADCAST_SERVICE_DISCOVERY_RESPONSE_RECEIVED ) {
        if ( epList->count > 0 ) {
            serverEndpoint = epList->list[0];
            serverNodeId = result->matchAddress;
            changeBootLoadState( APP_CLIENT_BTLDR_GET_SERVER_EUI );
        }
    }
    else if ( result->status == UNICAST_SERVICE_DISCOVERY_COMPLETE_WITH_RESPONSE ) {
         App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
               g_UPGRADE_SERVER_ID_c, result->responseData, g_Write_Attribute_Data_c );
         changeBootLoadState( APP_CLIENT_BTLDR_APP_LINK_KEY );
    }
    else {
        App_resumeOtaUpgradeAfterDelay( g_OTA_SERVER_DISCOVERY_DELAY_c );
    }
    App_ResumDiscovery();
    return g_TRUE_c;
}

/*******************************************************************************************/
static void App_addImageInfo( uint8_t* imageInfo )
{
    memUtils_memCopy( imageInfo, (uint8_t*)&currentDownloadFile.manufacturerId, 0x02 );
    memUtils_memCopy( imageInfo + 2, (uint8_t*)&currentDownloadFile.imageTypeId, 0x02 );
    memUtils_memCopy( imageInfo + 4, (uint8_t*)&currentDownloadFile.firmwareVersion, 0x04 );
}

/*******************************************************************************************/
void App_sendRequest( uint8_t cmdId, ZCL_Status upgrdEndStatus, uint32_t timer )
{
    uint8_t asdu[25];
    uint8_t fieldControl = 0;
    uint8_t asduLength = 0x03;

    asdu[0] =  g_ZCL_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
                    g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    asdu[1] = App_GetTransSeqNumber();
    asdu[2] = cmdId;
    waitingForResponse = g_TRUE_c;
    uint32_t expiryTime = g_RESPONSE_TIME_OUT_c;

    switch( cmdId )
    {
    case g_QueryNextImageRequest_c :
        if ( hardwareVersion != g_INVALID_HARDWARE_VERSION_c ) {
             fieldControl |= 0x01;
        }
        asdu[asduLength++] = fieldControl;
        App_addImageInfo( &asdu[asduLength] );
        asduLength += 0x08;
        if ( hardwareVersion != g_INVALID_HARDWARE_VERSION_c ) {
            *((uint16_t*)asdu[asduLength]) = hardwareVersion;
            asduLength += 0x02;
        }
        expiryTime = g_OTA_NEXT_IMAGE_QUERY_DELAY_c;
        break;

    case g_ImageBlockRequest_c :
    case g_ImagePageRequest_c :
#if (g_SEND_IEEE_ADDR_WITH_BLK_REQ_c == 1)
        fieldControl = 1u;
#else
        fieldControl = 0u;
#endif /* (g_SEND_IEEE_ADDR_WITH_BLK_REQ_d == 1) */
        asdu[asduLength++] = fieldControl;
        App_addImageInfo( &asdu[asduLength] );
        asduLength += 0x08;
        *((uint32_t*)&asdu[asduLength]) = App_getCurrentOffset();
        asduLength += 0x04;
        asdu[asduLength++] = g_MAX_CLIENT_DATA_SIZE_c;

        if ( cmdId == g_ImagePageRequest_c ) {
            *((uint16_t*)&asdu[asduLength]) = currentPageSize;
            asduLength += 0x02;
            *((uint16_t*)&asdu[asduLength]) = g_OTA_CLIENT_PAGE_REQUEST_SPACING_c;
            asduLength += 0x02;
        }

        if (fieldControl & g_SEND_IEEE_ADDR_WITH_BLK_REQ_c) {
            memUtils_memCopy(&asdu[asduLength], ZigBeeGetSelfIEEEAddress(),
                             0x08);
            asduLength += 0x08;
        }
        break;

    case g_UpgradeEndRequest_c:
        asdu[asduLength++] = upgrdEndStatus;
        App_addImageInfo( &asdu[asduLength] );
        asduLength += 0x08;
        if ( upgrdEndStatus != g_ZCL_Success_c ) {
            waitingForResponse = g_FALSE_c;
        }
        break;

    case g_QuerySpecificFileRequest_c:
        memUtils_memCopy(&asdu[asduLength], ZigBeeGetSelfIEEEAddress(), 0x08 );
        asduLength += 0x08;
        App_addImageInfo( &asdu[asduLength] );
        asduLength += 0x08;
#if (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 0)
        *((uint16_t*)&asdu[asduLength]) = g_ZIGBEE_PRO_STACK_VERSION_c;
#else
        *((uint16_t*)&asdu[asduLength]) = m_ZIGBEE_PRO_STACK_VERSION;
#endif /* (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 0) */
		asduLength= asduLength +2;
        break;

    default:
        return;
    }
    App_resumeOtaUpgradeAfterDelay( expiryTime );
    App_SendData ( g_Short_Address_Mode_c,(uint8_t*)&serverNodeId,
                   serverEndpoint, g_OTA_UPGRADE_CLUSTER_c, asduLength, asdu );
}

/******************************************************************************/
#if (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 1)

void App_clientBootloaderSetImageType (uint16_t value)
{
    currentDownloadFile.imageTypeId = value;
#if ( g_IMAGE_TYPE_ID_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
                                 g_IMAGE_TYPE_ID_c,
                                 (uint8_t*)&currentDownloadFile.imageTypeId,
                                 g_Write_Attribute_Data_c );
#endif
}

/******************************************************************************/

void App_clientBootloaderSetManufatureCode (uint16_t value)
{
    currentDownloadFile.manufacturerId = value;
#if ( g_MANUFACTURER_ID_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint,  g_OTA_UPGRADE_CLUSTER_c,
                                 g_MANUFACTURER_ID_c,
                                 (uint8_t*)&currentDownloadFile.manufacturerId,
                                 g_Write_Attribute_Data_c );
#endif
}

/******************************************************************************/

void App_clientBootloaderSetFileVersion (uint32_t value)
{
    currentDownloadFile.firmwareVersion = value;
#if ( g_CURRENT_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
                                 g_CURRENT_FILE_VERSION_c,
                                 (uint8_t*)&currentDownloadFile.firmwareVersion,
                                 g_Write_Attribute_Data_c );
#endif
}

/******************************************************************************/

void App_clientBootloaderSetPollTime (uint32_t value)
{
    g_OTA_NEXT_IMAGE_QUERY_DELAY_c = (value * g_MS_IN_SEC_c);
}

void App_clientBootloaderSetStackVersion (uint16_t value)
{
    m_ZIGBEE_PRO_STACK_VERSION = value;
}
#endif /* (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 1) */

/******************************************************************************/

void App_resetBootloaderFirmwareInfo (void)
{
#if ( g_CURRENT_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
                                 g_CURRENT_FILE_VERSION_c,
                                 (uint8_t*)&currentDownloadFile.firmwareVersion,
                                 g_Read_Attribute_Data_c );
#endif
#if ( g_MANUFACTURER_ID_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint,  g_OTA_UPGRADE_CLUSTER_c,
                                 g_MANUFACTURER_ID_c,
                                 (uint8_t*)&currentDownloadFile.manufacturerId,
                                 g_Read_Attribute_Data_c );
#endif
#if ( g_IMAGE_TYPE_ID_ATTRIBUTE_ENABLE_d == 1 )
    App_ReadWriteClientAttribute( myEndpoint, g_OTA_UPGRADE_CLUSTER_c,
                                 g_IMAGE_TYPE_ID_c,
                                 (uint8_t*)&currentDownloadFile.imageTypeId,
                                 g_Read_Attribute_Data_c );
#endif
}
/******************************************************************************/

static bool App_otaClientVerifyImage (void)
{
    uint8_t bufferId;
    uint8_t *pHeader;
    uint8_t status;
    /* allocate a buffer to store the OTA file header */
    bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);

    /* if buffer index is invalid */
    if (bufferId == g_NO_AVAILABLE_BUFFER_c) {
        /* then return status as out of resource */
        return g_FALSE_c;
    }
    /* get the allocated buffer pointer */
    pHeader = buffMgmt_getBufferPointer(bufferId);
    /* read the OTA header in the EEPROM to the allocated buffer */
    status = eeprom_readBuffer(g_START_DOWNLOAD_OFFSET_c,
                               pHeader, g_OTA_HDR_MAX_SIZE_IN_BYTES_c);
    /* if the EEPROM read fails */
    if (status != g_SUCCESS_c) {
        /* then return status as generic error */
        buffMgmt_freeBuffer( bufferId, 144 );
        return g_FALSE_c;
    }
    /* if file id is not OTA file id */
    if (g_OTA_HDR_FILE_ID_IN(pHeader) != g_OTA_FILE_ID_c) {
        /* then return status as failure */
        buffMgmt_freeBuffer( bufferId, 144 );
        return g_FALSE_c;
    }
    /* if header length is less invalid */
    if ((g_OTA_HDR_LEN_IN(pHeader) < g_OTA_HDR_MANDATORY_FIELDS_SIZE_c) ||
        (g_OTA_HDR_LEN_IN(pHeader) > g_OTA_HDR_MAX_SIZE_IN_BYTES_c)) {
        /* then return status as failure */
        buffMgmt_freeBuffer( bufferId, 144 );
        return g_FALSE_c;
    }
    /* if header manufaturer code is not matching with device manufacturer code */
    if (g_OTA_HDR_MANF_CODE_IN(pHeader) != currentDownloadFile.manufacturerId) {
        /* then return status as failure */
        buffMgmt_freeBuffer( bufferId, 144 );
        return g_FALSE_c;
    }
    /* if image type is not matching the download image type */
    if (g_OTA_HDR_IMAGE_TYPE_IN(pHeader) != currentDownloadFile.imageTypeId) {
        /* then return status as failure */
        buffMgmt_freeBuffer( bufferId, 144 );
        return g_FALSE_c;
    }
    /* if file version is not matching with the download file version */
    if (g_OTA_HDR_FILE_VER_IN(pHeader) != currentDownloadFile.firmwareVersion) {
        /* then return status as failure */
        buffMgmt_freeBuffer( bufferId, 144 );
        return g_FALSE_c;
    }
    /* if security credential is available */
    if (g_OTA_HDR_FIELD_CONTROL_IN(pHeader) & g_FIELD_CTRL_SEC_CRED_BIT_MASK_c) {
        /* if security credential version is not matching with the device */
        if (g_OTA_HDR_SEC_CRED_VER_IN(pHeader) != g_SEC_CRED_VER_c) {
            /* then return status as failure */
            buffMgmt_freeBuffer( bufferId, 144 );
            return g_FALSE_c;
        }
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
        if ( !App_verfyImageSignature( g_OTA_HDR_IMAGE_SIZE_IN(pHeader ) ) ) {
            buffMgmt_freeBuffer( bufferId, 144 );
            return g_FALSE_c;
        }
#else
        buffMgmt_freeBuffer( bufferId, 144 );
        return g_FALSE_c;
#endif
    }
    /* if file is device specific */
    if (g_OTA_HDR_FIELD_CONTROL_IN(pHeader) & g_FIELD_CTRL_DEV_SPEC_BIT_MASK_c) {
        /* if the device address in the image is not matching with the device */
        if (memUtils_memCompare(g_OTA_HDR_FILE_DEST_LOC(pHeader),
                                ZigBeeGetSelfIEEEAddress(),
                                g_EXTENDED_ADDRESS_LENGTH_c)) {
            /* then return status as failure */
            buffMgmt_freeBuffer( bufferId, 144 );
            return g_FALSE_c;
        }
    }
    /* if hardware version present */
    if (g_OTA_HDR_FIELD_CONTROL_IN(pHeader) & g_FIELD_CTRL_HRD_VER_BIT_MASK_c) {
        /* if the image hardware version is not matching with the device
         * hardware version */
        if ((g_OTA_HDR_MIN_HARDWARE_VER_IN(pHeader) < hardwareVersion) ||
            (g_OTA_HDR_MAX_HARDWARE_VER_IN(pHeader) > hardwareVersion)) {
            /* then return status as failure */
           buffMgmt_freeBuffer( bufferId, 144 );
           return g_FALSE_c;
        }
    }
    buffMgmt_freeBuffer( bufferId, 144 );
    return g_TRUE_c;
}

/******************************************************************************/
static void App_otaUpadateImagestate( void )
{
    bootloader_fileInfo_t file;
    /* update the image info from the ota header */
    file.state = g_IMAGE_AVAILABLE_c;

    if (!App_getImageTagLocation(g_OTA_UPGRADE_IMAGE_TAG_ID_c,&file.address, &file.length)) {
        /* return due to image not found */
        return;
    }
    /* write the file info into the EEPROM */
    if ( (eeprom_writeBuffer(g_IMAGE_STATE_LOCATION_IN_EEPROM_c,
                           (uint8_t*)&file, sizeof(bootloader_fileInfo_t))) != g_SUCCESS_c) {
        /* return if eeprom write fails */
        return;
    }
//    /* reboot the device */
//    halReboot();
}

/**************************************************************************************/
bool App_getImageTagLocation( uint16_t tagId,
                                     uint32_t* tagOffset, uint32_t* tagSize )
{
    uint8_t header[ g_OTA_HDR_MAX_SIZE_IN_BYTES_c + 6 ];
    uint8_t tagHeader[ g_OTA_SUB_ELEMENT_HDR_SIZE_c ];
    uint32_t offset;
    uint32_t imageSize;
    bool found = g_FALSE_c;

    uint8_t status = 0xff;

    /* Read the Ota file header */
    status = eeprom_readBuffer(g_START_DOWNLOAD_OFFSET_c, header, (g_OTA_HDR_MAX_SIZE_IN_BYTES_c + 6) );
      if(status != g_SUCCESS_c) {
      return g_FALSE_c;
    }
    offset = g_START_DOWNLOAD_OFFSET_c + g_OTA_HDR_LEN_IN( header );
    imageSize = g_OTA_HDR_IMAGE_SIZE_IN( header );
    /* Loop through the imge file till u reach the required tag or reach to
    end of the image file */
    while ( ( offset < imageSize ) && ! found ) {
    /* Read tag header and check with tag Id, if match break else go to next tag */
    status = eeprom_readBuffer( offset, tagHeader, g_OTA_SUB_ELEMENT_HDR_SIZE_c ) ;
    if(status != g_SUCCESS_c) {
      return g_FALSE_c;
    }

        offset += g_OTA_SUB_ELEMENT_HDR_SIZE_c;
        if ( g_OTA_SUB_ELEMENT_TAG_ID_IN( tagHeader ) == tagId ) {
            found = g_TRUE_c;
        }
        else {
            offset += g_OTA_SUB_ELEMENT_TAG_LEN_IN( tagHeader );
        }
    }
    if ( !found ) {
        return g_FALSE_c;
    }
    *tagOffset = offset;
    *tagSize = g_OTA_SUB_ELEMENT_TAG_LEN_IN( tagHeader );
    return g_TRUE_c;
}

/******************************************************************************/
void App_setOrResetPageRequest( bool edpagerequest )
{
    if (APP_CLIENT_BTLDR_DOWNLOAD != getBootLoadState()) {
        usePageRequest = edpagerequest;
    }
}

#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
/******************************************************************************/
uint8_t App_VerifyDSAsignature(uint8_t*signature, uint8_t* digest, uint8_t*publickey )
{
    uint8_t status;

    uint8_t rComponent[SECT163K1_POINT_ORDER_SIZE];
    uint8_t sComponent[SECT163K1_POINT_ORDER_SIZE];

    memUtils_memCopy ( rComponent, signature, SECT163K1_POINT_ORDER_SIZE );
    memUtils_memCopy ( sComponent, signature + SECT163K1_POINT_ORDER_SIZE,
                       SECT163K1_POINT_ORDER_SIZE );

    status = ZSE_ECDSAVerify ( publickey,
                    digest,
                    rComponent,
                    sComponent,
                    AppMCHPYieldFunc,
                    g_YIELD_LEVEL_c );

    return status == MCE_SUCCESS ? g_TRUE_c : g_FALSE_c;
}

/*****************************************************************************/
bool App_genarateDigest(uint8_t *message, uint32_t length, uint8_t *output)
{
    uint16_t i;
    uint16_t j;
    uint16_t NoOfBlocks;
    uint8_t messagePending;
    uint16_t lengthInBits;
    uint8_t blockData[16];
    uint8_t plain_block[16];
    NoOfBlocks = length / g_BLOCK_SIZE_c;
    uint8_t status = 0xff;

    lengthInBits = 0x0000;
    memUtils_memSet( output, 0x00, g_BLOCK_SIZE_c );
    for ( i = 0; i < NoOfBlocks; i++ ) {
      status= eeprom_readBuffer( (uint32_t)(message+ i * g_BLOCK_SIZE_c), blockData, 16 );
      if( status != g_SUCCESS_c) {
        return g_FALSE_c;
      }
        memUtils_memCopy( plain_block, blockData, g_BLOCK_SIZE_c );
        ST_AesSetKey(output);
        ST_AesEncrypt(blockData);
        for (j = 0; j < g_BLOCK_SIZE_c; j++) {
            output[j] = plain_block[j] ^ blockData[j];
        }
    }
    messagePending = length - (NoOfBlocks * g_BLOCK_SIZE_c);
    if ( messagePending ) {
      status= eeprom_readBuffer( (uint32_t)(message+ NoOfBlocks*g_BLOCK_SIZE_c),
                                blockData, messagePending );
      if( status != g_SUCCESS_c) {
      return g_FALSE_c;
      }
        for (i = 0; i < (g_BLOCK_SIZE_c - messagePending); i++) {
            blockData[messagePending + i] = 0x00;
        }
    }
    else {
        for (i = 0; i < g_BLOCK_SIZE_c; i++) {
            blockData[i] = 0x00;
        }
    }
    blockData[messagePending] = 0x80;
    lengthInBits = length * 8;
    blockData[14] = (uint8_t)((lengthInBits & 0xFF00) >> 8);
    blockData[15] = (uint8_t) lengthInBits;
    memUtils_memCopy(plain_block, blockData, g_BLOCK_SIZE_c);
    ST_AesSetKey(output);
    ST_AesEncrypt(blockData);
    for (j = 0; j < g_BLOCK_SIZE_c; j++) {
        output[j] = plain_block[j] ^ blockData[j];
    }
    ST_AesSetKey(output);
    return g_TRUE_c;
}

/**************************************************************************************/
bool App_CheckAuthorizedSigner( uint8_t* signer )
{
    uint8_t index;
    for ( index =0; index < g_MAX_SIGNERS_c; index++ ) {
        if ( g_MEMORY_MATCH_c == memUtils_memCompare(
                      signer, allowedSigners[index], g_MAX_ADDR_LENGTH_c ) ) {
            return g_TRUE_c;
        }
    }
    return g_FALSE_c;
}

/******************************************************************************/
bool App_getSignerPublicKey( uint8_t* certificate, uint8_t* signerpubliKey )
{
    uint8_t caPublicKey[SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE];
    uint8_t index;
    for ( index = 0; index < g_MAX_CA_KEYS_c; index++ ) {
        if ( g_MEMORY_MATCH_c == memUtils_memCompare(
             &certificate[g_ISSUER_ADDRESS_LOCATION_c], issuerCAkeys[index].issuer, 0x08) ) {
             memUtils_memCopy( caPublicKey,
                   issuerCAkeys[index].capublickey, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
             ZSE_ECQVReconstructPublicKey( certificate, caPublicKey,
                signerpubliKey, AppMCHPHashFunc,AppMCHPYieldFunc, g_YIELD_LEVEL_c );
             return g_TRUE_c;
        }
    }
    return g_FALSE_c;
}

/**************************************************************************************/
bool App_verfyImageSignature( uint32_t imageSize )
{
    uint8_t certificate[SECT163K1_CERTIFICATE_SIZE];
    uint8_t signature[SECT163K1_POINT_ORDER_SIZE*2+g_MAX_ADDR_LENGTH_c];
    uint32_t offset;
    uint32_t tagsize;
    uint8_t digest[16];
    uint8_t signerPublicKey[SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE];

    if ( !( App_getImageTagLocation( g_OTA_ECDSA_SIGNATURE_TAG_ID_c, &offset, &tagsize) &&
            (eeprom_readBuffer( offset, signature, tagsize )!= g_SUCCESS_c)    ) ) {
        return g_FALSE_c;
    }
    if ( ! App_CheckAuthorizedSigner( signature ) ) {
        return g_FALSE_c;
    }
    if ( !( App_getImageTagLocation(g_OTA_ECDSA_CERTIFICATE_TAG_ID_c, &offset, &tagsize ) &&
            (eeprom_readBuffer(offset, certificate, tagsize)!= g_SUCCESS_c) ) ) {
           return g_FALSE_c;
    }
    if ( g_MEMORY_NO_MATCH_c == memUtils_memReverseCmp(
         signature, &certificate[g_DEVICE_ADDRESS_LOCATION_c],g_MAX_ADDR_LENGTH_c) ) {
        return g_FALSE_c;
    }
    if ( !App_getSignerPublicKey( certificate, signerPublicKey ) ) {
        return g_FALSE_c;
    }
    if ( !App_genarateDigest((uint8_t*)g_START_DOWNLOAD_OFFSET_c, imageSize-42, digest) ) {
         return g_FALSE_c;
    }
    if ( !App_VerifyDSAsignature(&signature[8], digest, signerPublicKey ) ) {
        return g_FALSE_c;
    }
    return g_TRUE_c;
}
#endif  /* ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 ) */
/*********************************************************************************************
* End Of File
*********************************************************************************************/
#endif /* ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 ) */
