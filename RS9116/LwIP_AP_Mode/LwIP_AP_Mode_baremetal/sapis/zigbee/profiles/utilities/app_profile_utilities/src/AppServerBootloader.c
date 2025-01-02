/*******************************************************************************
 * AppServerBootloader.c
 ********************************************************************************
 * Program Description:
 * This module provides all the functionality for supporting OTA cluster from
 * server side.
 *
 *******************************************************************************/
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "queue.h"
#include "stack_common.h"
#include "ApptimerId.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "timer_id.h"
#include "sw_timer.h"
#include "ApplicationThinLayer.h"
#include "apsde_interface.h"
#include "zdo_interface.h"
#include "ZCL_Functionality.h"
#if (g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1)
#include "ZCL_OTA_Upgrade_Cluster.h"
//RP: need to integrate external EEPROM
//#include "eeprom.h"
#include "AppServerBootloader.h"
#include "AppOTACommon.h"
#endif   /* g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d*/

#if (g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1)
/******************************************************************************
 * local Constants
 ******************************************************************************/

#define g_GetZCLPacketPayloadPointer_d(x)   (((uint8_t*)x) +  sizeof(ZCL_Header_t))

#define g_MIN_QUERY_NEXT_IMAGE_REQ_SIZE_c   (sizeof(QueryNextImageRequest_t) \
    - sizeof(uint16_t))

#define g_MIN_IMAGE_BLOCK_REQ_SIZE_c        (sizeof(ImageBlockRequest_t) -\
    g_EXTENDED_ADDRESS_LENGTH_c)

#define g_MIN_IMAGE_PAGE_REQ_SIZE_c         (sizeof(ImagePageRequest_t) -\
    g_EXTENDED_ADDRESS_LENGTH_c)\

#define g_MIN_UPGRADE_END_REQ_SIZE_c        (sizeof(ImageUpgradeEndRequest_t))

#define g_MIN_QUERY_FILE_SPEC_REQ_SIZE_c    (sizeof(QuerySpecificFileRequest_t))

#define g_SERVER_DELAYED_RESPONSE_QUEUE_SIZE_c          2u

#define g_SECS_IN_MSECS_c                               1000u


/* server requires max and min time*/
#define g_SERVER_DELAYED_RESPONSE_MIN_TIME_IN_SEC_c      0.5

#define g_SERVER_DELAYED_RESPONSE_MAX_TIME_IN_SEC_c     1.0

#if (g_OTA_BOOTLOADER_SERVER_CLI_SUPPORT_d == 0)

#define g_SERVER_DELAYED_RESPONSE_MIN_TIME_IN_MS_c     \
    ((uint32_t)(g_SERVER_DELAYED_RESPONSE_MIN_TIME_IN_SEC_c * g_SECS_IN_MSECS_c))

#define g_SERVER_DELAYED_RESPONSE_MAX_TIME_IN_MS_c     \
    ((uint32_t)(g_SERVER_DELAYED_RESPONSE_MAX_TIME_IN_SEC_c * g_SECS_IN_MSECS_c))

#endif   /* g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d */

#if (g_OTA_BOOTLOADER_SERVER_CLI_SUPPORT_d == 1)
static uint32_t g_SERVER_DELAYED_RESPONSE_MIN_TIME_IN_MS_c =  ((uint32_t)(0.5) * g_SECS_IN_MSECS_c);
static uint32_t g_SERVER_DELAYED_RESPONSE_MAX_TIME_IN_MS_c =  ((uint32_t)(1.0) * g_SECS_IN_MSECS_c);

#endif   /* g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d*/

/******************************************************************************
 * Type definitions, structures & unions
 ******************************************************************************/
typedef enum {
    APP_BTLDR_INIT = 0u, APP_BTLDR_READY = 1u, APP_BTLDR_UPGRADE = 2u
} srvBtldr_State_t;
#pragma pack(1)
typedef struct {
    uint16_t manufacturerCode;
    uint16_t imageType;
    uint32_t fileVersion;
    uint16_t stackVersion;
    uint32_t imageSize;
    uint32_t fileStartLocation;
    uint16_t minHardwareVersion;
    uint16_t maxHardwareVersion;
    uint8_t ieeeAddr[8];
} __attribute__((__packed__))srvBtldr_fileInfo_t;

typedef struct {
    uint16_t deviceAddress;
    uint8_t endPoint;
    uint32_t reqOffset;
    uint32_t finalOffset;
    uint8_t dataSize;
    uint16_t responseDelay;
    uint16_t minResponseDelay;
    uint8_t queryJitter;
    uint32_t currentTime;
    uint32_t upgradeTime;
} __attribute__((__packed__))srvBtldr_upgradeParameter_t;

typedef struct {
    queue_t otaToAppBtldrQueue;
    uint32_t upgradeTime;
    uint32_t currentTime;
    uint8_t upgradeParamBuffId;
    uint8_t stateMachine;
    srvBtldr_fileInfo_t currentFileInfo;
    uint8_t imageAvailable;
} __attribute__((__packed__))srvBtldr_config_t;

typedef struct {
    uint8_t queryJitter;
    uint8_t dataSize;
} __attribute__((__packed__))srvBtldr_OTAParameters_t;

typedef struct {
    uint8_t deviceExtAddr[8];
    uint32_t fileOffset;
    uint16_t pageSize;
    uint8_t dataSize;
    uint16_t responseDelay;
    uint16_t minResponseDelay;
} __attribute__((__packed__))srvBtldr_pageRequestTable_t;


typedef struct {
    uint32_t id;
    uint16_t headerVersion;
    uint16_t headerLength;
    uint16_t fieldControl;
    uint16_t manufacturerCode;
    uint16_t imageType;
    uint32_t fileVersion;
    uint16_t stackVersion;
    uint8_t headerString[32];
    uint8_t imageSize;
} __attribute__((__packed__))ota_header_t;
#pragma pack()

/******************************************************************************
 * Public Memory declarations
 ******************************************************************************/

const uint8_t g_OTA_APP_INVALID_IEEE_ADDR_c[8] =
{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

/******************************************************************************
 * Private Memory declarations
 ******************************************************************************/

static uint32_t delayExpiryTime;

static uint32_t lastTimeTickAtExpiry;

static srvBtldr_config_t m_btldrConfig;

static queue_t m_delayedRespQueue;



uint8_t serverDelayedResponseMinTime;
uint8_t serverDelayedResponseMaxTime;


/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

static App_Status_t srvBtldr_startPageResp ( APSDE_Data_Indication_t *pRequest );

static void srvBtldr_pageResponseTimer ( uint8_t timerId );

static void srvBtldr_sendBlockRespForPageReq ( uint8_t indexInEntry,
        srvBtldr_upgradeParameter_t *pTable );

static void srvBtldr_updateCurrentFileInfo ( void );

static void srvBtldr_prepareImageInfo ( uint8_t *pDest, uint8_t *pSrc );

static App_Status_t srvBtldr_verifyImageInfo ( uint8_t *pReceived );

static void srvBtldr_sendResponse ( APSDE_Data_Indication_t *pRequest,
                                   uint8_t *pResponse, uint8_t commandId,
                                   uint8_t payloadSize,
                                   uint8_t ackRequired );

static void srvBtldr_stopDevicePageResp (uint8_t *pDeviceAddress,
                                         uint8_t addrMode);

static void srvBtldr_sendWithDelay  ( APSDE_Data_Indication_t *pRequest,
        uint8_t *pResponse, uint8_t commandId, uint8_t payloadSize,
        uint8_t ackRequired );

static void srvBtldr_sendData (uint8_t bufferId);

static void srvBtldr_setDelayedRespExpiryTime (void);

static void srvBtldr_sendNegativeResp (uint8_t status,
                                       APSDE_Data_Indication_t *pRequest,
                                       uint8_t cmdId);

/******************************************************************************
 * Public Functions
 ******************************************************************************/
#if (g_OTA_BOOTLOADER_SERVER_CLI_SUPPORT_d == 1)

void App_otaConfigServerDelayedResponseTime(uint32_t minTime,uint32_t maxTime )
{
	g_SERVER_DELAYED_RESPONSE_MIN_TIME_IN_MS_c = minTime;
	g_SERVER_DELAYED_RESPONSE_MAX_TIME_IN_MS_c = maxTime;
}
#endif
/******************************************************************************/

void App_otaUpgradeServerEventHandler (ZCL_Event_t *pEvent,
        APSDE_Data_Indication_t *pIndicationbuffer)
{
    /* check whether the device is in valid state to respond for the request */
    if (m_btldrConfig.stateMachine != APP_BTLDR_UPGRADE) {
        /* device not in a state to respond */
        return ;
    }
    if (pEvent == g_NULL_c) {
        return;
    }
    App_Status_t status = g_OTA_APP_SUCCESS_c;
    switch( (pEvent->eventId - (uint8_t) g_ImageNotify_Event_c) ) {
    case g_QueryNextImageRequest_c:
        if (pIndicationbuffer->asdulength < g_MIN_QUERY_NEXT_IMAGE_REQ_SIZE_c) {
            status = g_OTA_APP_MALFORMED_COMMAND_c;
            break;
        }
        status = srvBtldr_handleQueryNextImageRequest(pIndicationbuffer);
        break;
    case g_ImageBlockRequest_c:
        if (pIndicationbuffer->asdulength < g_MIN_IMAGE_BLOCK_REQ_SIZE_c) {
            status = g_OTA_APP_MALFORMED_COMMAND_c;
            break;
        }
        status = srvBtldr_handleImageBlockRequest(pIndicationbuffer);
        break;
    case g_ImagePageRequest_c:
        if (pIndicationbuffer->asdulength < g_MIN_IMAGE_PAGE_REQ_SIZE_c) {
            status = g_OTA_APP_MALFORMED_COMMAND_c;
            break;
        }
        status = srvBtldr_handleImagePageRequest(pIndicationbuffer);
        break;
    case g_UpgradeEndRequest_c:
        if (pIndicationbuffer->asdulength < g_MIN_UPGRADE_END_REQ_SIZE_c) {
            status = g_OTA_APP_MALFORMED_COMMAND_c;
            break;
        }
        status = srvBtldr_handleUpgradeEndRequest(pIndicationbuffer);
        break;
    case g_QuerySpecificFileRequest_c:
        if (pIndicationbuffer->asdulength < g_MIN_QUERY_FILE_SPEC_REQ_SIZE_c) {
            status = g_OTA_APP_MALFORMED_COMMAND_c;
            break;
        }
        status = srvBtldr_handleQuerySpecificFileRequest(pIndicationbuffer);
        break;
    default:
        break;
    }
    /* if status is malformed image */
    if (g_OTA_APP_MALFORMED_COMMAND_c == status) {
        /* then send default response as malformed command */
        App_SendDefaultResponse(g_ZCL_Malformed_Command_c, g_TRUE_c,
                                pIndicationbuffer);
    }
}

/******************************************************************************/

void srvBtldr_init ( void )
{
    /* free the 'page request table' */
    if (m_btldrConfig.upgradeParamBuffId != 0x00
            && m_btldrConfig.upgradeParamBuffId != 0xff) {
        buffMgmt_freeBuffer(m_btldrConfig.upgradeParamBuffId, 189);
    }
    m_btldrConfig.upgradeParamBuffId = 0xff;

    /* TBD - init EEPROM */
#ifndef	STM32L1XX_MD_PLUS
    eeprom_init();
#else
	sEE_Init ();
#endif

    /* initialize the ota parameters */
    m_btldrConfig.currentFileInfo.manufacturerCode = g_OTA_DEFAULT_MANF_CODE_c;
    m_btldrConfig.currentFileInfo.imageType = g_OTA_DEFAULT_IMG_TYPE_c;
    m_btldrConfig.currentFileInfo.fileVersion = g_OTA_DEFAULT_FILE_VERSION_c;
    m_btldrConfig.currentFileInfo.stackVersion = g_OTA_DEFAULT_STACK_VERSION_c;
    m_btldrConfig.currentFileInfo.imageSize = g_OTA_DEFAULT_IMG_SIZE_c;

    /* image available */
    m_btldrConfig.imageAvailable = g_FALSE_c;

    /* upgrade time & current time */
    m_btldrConfig.upgradeTime = g_OTA_APP_DEFAULT_UPGRADE_TIME_IN_SECS_c;
    m_btldrConfig.currentTime = 0;

    /* TBD - change the AppBtldr mode to init mode */
    m_btldrConfig.stateMachine = APP_BTLDR_INIT;

    queueMgmt_Init(&m_delayedRespQueue, g_SERVER_DELAYED_RESPONSE_QUEUE_SIZE_c);

    lastTimeTickAtExpiry = 0;
    delayExpiryTime = 0;
}

/******************************************************************************/

void srvBtldr_main ( void )
{
    uint32_t currentTickTime = halCommonGetInt32uMillisecondTick();
    uint32_t timeElapsedSinceLastExpiry = 0;
    uint8_t bufferId;
    /* Application Bootloader main state machine */
    switch( m_btldrConfig.stateMachine ) {
    case APP_BTLDR_INIT:
        /* wait for the application to send the image */
        break;
    case APP_BTLDR_READY:
//        /* convert the received image to *.zigbee file */
//        srvBtldr_convertToZigBee();
        /* update current file information */
        srvBtldr_updateCurrentFileInfo();
        m_btldrConfig.stateMachine = APP_BTLDR_UPGRADE;
        break;
    case APP_BTLDR_UPGRADE:
        if (Is_Msg_Available(m_delayedRespQueue)) {
            if (lastTimeTickAtExpiry > currentTickTime) {
                timeElapsedSinceLastExpiry = currentTickTime +
                    (0xffffffffu - lastTimeTickAtExpiry);
            }
            else {
                timeElapsedSinceLastExpiry = currentTickTime - lastTimeTickAtExpiry;
            }
            if (delayExpiryTime <= timeElapsedSinceLastExpiry) {
                bufferId = Dequeue(m_delayedRespQueue);
                srvBtldr_sendData(bufferId);
                if (!Is_Msg_Available(m_delayedRespQueue)) {
                    lastTimeTickAtExpiry = 0;
                }
                else {
                    srvBtldr_setDelayedRespExpiryTime();
                }
            }
        }
        break;
    default:
        srvBtldr_init();
        break;
    }
}

/******************************************************************************/

#if (g_IMAGE_NOTIFY_SUPPORTED_d == RSI_ENABLE)
App_Status_t srvBtldr_sendImageNotify (uint8_t jitter, uint8_t *pDeviceAddress)
{
    /* if the application is not in state to send the image notify command */
    if (m_btldrConfig.stateMachine != APP_BTLDR_UPGRADE) {
        /* then return state as invalid */
        return g_OTA_APP_INVALID_STATE_c;
    }
    /* create a buffer for the response */
    uint8_t bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
    /* if no buffer is available */
    if (bufferId == g_NO_AVAILABLE_BUFFER_c) {
        /* then return status as failure */
        return g_OTA_APP_OUT_OF_RESOURCE_c;
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 90 */
        allocatedbuffer[bufferId]= 230;
        freebuffer[bufferId][0]= 230;
        freebuffer[bufferId][1]= 0;
#endif
    /* get the allocated buffer pointer */
    uint8_t *pResponse = buffMgmt_getBufferPointer(bufferId);
    /* if the allocated buffer pointer is invalid */
    if (pResponse == g_NULL_c) {
        /* then return status as failure */
        return g_OTA_APP_OUT_OF_RESOURCE_c;
    }

    ImageNotify_t *pResp =
        (ImageNotify_t*) g_GetZCLPacketPayloadPointer_d(pResponse);

    /* append ZCL Header */
    ZCL_Header_t *pZCLHeader = (ZCL_Header_t*)pResponse;
    pZCLHeader->frameControl = (g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                    g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c);
    pZCLHeader->transactionSeq = App_GetTransSeqNumber();
    pZCLHeader->commandId = g_IMAGE_NOTIFY_CMD_c;

    /* prepare image notify command */
    pResp->payLoadType = (g_NOTIF_TYPE_JITTER_c | g_NOTIF_TYPE_MANF_CODE_c |
                          g_NOTIF_TYPE_IMG_TYPE_c);
    pResp->queryJitter = (jitter > g_OTA_MAX_JITTER_VALUE_SUPPORTED_c) ?\
        g_OTA_APP_QUERY_JITTER_VALUE_c : jitter;
    srvBtldr_prepareImageInfo(pResp->manufacturerCode,\
        (uint8_t*)&m_btldrConfig.currentFileInfo.manufacturerCode);

    /* if device address pointer is null */
    if (pDeviceAddress == g_NULL_c) {
        /* then broadcast the message to all the devices */
        uint16_t address = g_BROADCAST_ADDRESS_c;
        App_sendAPSData(g_SHORT_ADDR_MODE_c, (uint8_t*) &address,
                        g_OTA_CLUSTER_END_POINT_c, g_OTA_CLUSTER_ID_c,
                        (sizeof(ImageNotify_t) + sizeof(ZCL_Header_t)), pResponse, 0);
        buffMgmt_freeBuffer(bufferId, 182);
        return g_OTA_APP_SUCCESS_c;
    }
    uint8_t aInvalidExtAddress[g_EXTENDED_ADDRESS_LENGTH_c];
    memUtils_memSet(aInvalidExtAddress, 0x00, g_EXTENDED_ADDRESS_LENGTH_c);
    /* if device address is invalid */
    if ((!memUtils_memCompare(aInvalidExtAddress, pDeviceAddress,
                              g_EXTENDED_ADDRESS_LENGTH_c))
        || (!memUtils_memCompare((uint8_t*) g_OTA_APP_INVALID_IEEE_ADDR_c,
                                 pDeviceAddress,
                                 g_EXTENDED_ADDRESS_LENGTH_c))) {
         /* then display message as invalid destination address */
         buffMgmt_freeBuffer(bufferId, 221);
         return g_OTA_APP_INVALID_ADDRESS_c;
     }
    /* send the response */
    App_sendAPSData(g_EXTENDED_ADDR_MODE_c, pDeviceAddress,
                    g_OTA_CLUSTER_END_POINT_c, g_OTA_CLUSTER_ID_c,
                    (sizeof(ImageNotify_t) + sizeof(ZCL_Header_t)), pResponse,
                    g_APS_ACK_REQUIRED_c);
    buffMgmt_freeBuffer(bufferId, 183);
    return g_OTA_APP_SUCCESS_c;
}
#else
#warning Image Notify command is not supported, you can enable this by \
setting g_IMAGE_NOTIFY_SUPPORTED_d to RSI_ENABLE in ZCL_Functionality.h
#endif /* (g_IMAGE_NOTIFY_SUPPORTED_d == RSI_ENABLE) */

/******************************************************************************/

void srvBtldr_abortUpgrade (void)
{
    /* if the application is not in upgrade state */
    if (m_btldrConfig.stateMachine != APP_BTLDR_UPGRADE) {
        /* then return */
        return;
    }
    /* change the state to INIT mode */
    m_btldrConfig.stateMachine = APP_BTLDR_INIT;

    /* stop the page response timer */
    tmr_stopTimer(g_OTA_APP_PAGE_RESP_TMER_ID_c);

    /* remove the page request table */
    if (m_btldrConfig.upgradeParamBuffId != g_NO_AVAILABLE_BUFFER_c) {
        buffMgmt_freeBuffer(m_btldrConfig.upgradeParamBuffId, 177);
        m_btldrConfig.upgradeParamBuffId = g_NO_AVAILABLE_BUFFER_c;
    }
    return;
}

/******************************************************************************/

void srvBtldr_imageReady (uint32_t imageAddress)
{
    m_btldrConfig.stateMachine = APP_BTLDR_READY;
    m_btldrConfig.currentFileInfo.fileStartLocation = imageAddress;
}

/******************************************************************************
 * Private Functions
 ******************************************************************************/
static App_Status_t srvBtldr_handleQueryNextImageRequest
                                    (APSDE_Data_Indication_t *pRequest)
{
    /* check if the buffer pointer is valid */
    if (pRequest == g_NULL_c) {
        /* if not return failure */
        return g_OTA_APP_FAILURE_c;
    }

    /* create a buffer for the response */
    uint8_t bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
    /* if no buffer is available */
    if (bufferId == g_NO_AVAILABLE_BUFFER_c) {
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 90 */
        allocatedbuffer[bufferId]= 231;
        freebuffer[bufferId][0]= 231;
        freebuffer[bufferId][1]= 0;
#endif
    /* get the allocated buffer pointer */
    uint8_t *pResponse = buffMgmt_getBufferPointer(bufferId);
    /* if the allocated buffer pointer is invalid */
    if (pResponse == g_NULL_c) {
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }

    QueryNextImageResponse_t *pResp =
        (QueryNextImageResponse_t*)\
            g_GetZCLPacketPayloadPointer_d(pResponse);

    QueryNextImageRequest_t *pQuery =
        (QueryNextImageRequest_t*)&pRequest->a_asdu[3];

    /* if image information verification is not success */
    if ((m_btldrConfig.imageAvailable == g_FALSE_c) ||
        (*(uint16_t*) pQuery->manufacturerCode
        != m_btldrConfig.currentFileInfo.manufacturerCode) ||
        (*(uint16_t*) pQuery->imageType
         != m_btldrConfig.currentFileInfo.imageType) ||
            (*(uint32_t*) pQuery->currentFileVersion
             >= m_btldrConfig.currentFileInfo.fileVersion)) {
         /* then send the response as no image */
         pResp->status = g_OTA_NO_IMAGE_AVAILABLE_c;
         srvBtldr_sendResponse(pRequest, pResponse, g_QueryNextImageResponse_c,
                               sizeof(pResp->status), g_APS_ACK_REQUIRED_c);
         buffMgmt_freeBuffer(bufferId, 196);
         return g_OTA_APP_SUCCESS_c;
    }

    /* if there is a hardware version field in the query */
    if (pQuery->fieldControl & 1u) {
        /* then if the hardware version in the query is not matching with the
        * images supported hardware version */
        uint16_t hardwareVersion = (*(uint16_t*) pQuery->hardwareVersion);
        if ((hardwareVersion < m_btldrConfig.currentFileInfo.minHardwareVersion) ||
            (hardwareVersion > m_btldrConfig.currentFileInfo.maxHardwareVersion)){
            /* then send the response as no image */
            pResp->status = g_OTA_NO_IMAGE_AVAILABLE_c;
            srvBtldr_sendResponse(pRequest, pResponse, g_QueryNextImageResponse_c,
                                  sizeof(pResp->status), g_APS_ACK_REQUIRED_c);
            buffMgmt_freeBuffer(bufferId, 200);
            return g_OTA_APP_SUCCESS_c;
        }
    }
    /* create response with the success */
    pResp->status = g_OTA_SUCCESS_c;
    *(uint16_t*) pResp->manufacturerCode
        = *(uint16_t*) pQuery->manufacturerCode;
    *(uint16_t*) pResp->imageType = *(uint16_t*) pQuery->imageType;
    *(uint32_t*) pResp->fileVersion
        = m_btldrConfig.currentFileInfo.fileVersion;
    *(uint32_t*) pResp->imageSize
        = m_btldrConfig.currentFileInfo.imageSize;

    /* send the response */
    srvBtldr_sendResponse(pRequest, pResponse, g_QueryNextImageResponse_c,
                          sizeof(QueryNextImageResponse_t), g_APS_ACK_REQUIRED_c);

    buffMgmt_freeBuffer(bufferId, 199);
    return g_OTA_APP_SUCCESS_c;
}

/******************************************************************************/

static App_Status_t srvBtldr_handleImageBlockRequest
                                        (APSDE_Data_Indication_t *pRequest)
{
    uint8_t ieeeAddress[8];
    /* check if the buffer pointer is valid */
    if (pRequest == g_NULL_c) {
        /* if not return failure */
        return g_OTA_APP_FAILURE_c;
    }

    ImageBlockRequest_t *pQuery =
            (ImageBlockRequest_t*)&pRequest->a_asdu[3];

    if (pQuery->fieldControl & g_SEND_IEEE_ADDR_WITH_BLK_REQ_c) {
        memUtils_memSet(ieeeAddress, 0u, 8);
        if (memUtils_memCompare(pQuery->requestNodeAddr, ieeeAddress, 8)) {
            /* send default response */
        }
        memUtils_memSet(ieeeAddress, 0xff, 8);
        if (memUtils_memCompare(pQuery->requestNodeAddr, ieeeAddress, 8)) {
            /* send default response */
        }
    }

    if (srvBtldr_verifyImageInfo((uint8_t*) &pQuery->manufacturerCode)
            != g_OTA_APP_SUCCESS_c) {
        /* then send the response as no image */
        srvBtldr_sendNegativeResp(g_OTA_APP_NO_IMAGE_AVAILABLE_c,
                                  pRequest,
                                  g_IMAGE_BLOCK_RESP_CMD_c);
        return g_OTA_APP_SUCCESS_c;
    }
    if  ((Is_Queue_Full(m_delayedRespQueue) == g_TRUE_c) ||
         (m_btldrConfig.currentFileInfo.imageSize <= g_OTA_HDR_MAX_SIZE_IN_BYTES_c)) {
        /* send response with the WAIT_FOR_DATA */
        srvBtldr_sendNegativeResp(g_OTA_WAIT_FOR_DATA_c, pRequest,
                                  g_IMAGE_BLOCK_RESP_CMD_c);
        return g_OTA_APP_SUCCESS_c;
    }
    /* if the query file offset is invalid */
    if (*(uint32_t*)pQuery->fileOffset >
        (m_btldrConfig.currentFileInfo.fileStartLocation
            + m_btldrConfig.currentFileInfo.imageSize)) {
        /* then return error as malformed packet */
        return g_OTA_APP_MALFORMED_COMMAND_c;
    }

    /* create a buffer for the response */
    uint8_t bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
    /* if no buffer is available */
    if (bufferId == g_NO_AVAILABLE_BUFFER_c) {
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 90 */
        allocatedbuffer[bufferId]= 232;
        freebuffer[bufferId][0]= 232;
        freebuffer[bufferId][1]= 0;
#endif
    /* get the allocated buffer pointer */
    uint8_t *pResponse = buffMgmt_getBufferPointer(bufferId);
    /* if the allocated buffer pointer is invalid */
    if (pResponse == g_NULL_c) {
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }

    ImageBlockResponse_t *pResp =
            (ImageBlockResponse_t*)\
            g_GetZCLPacketPayloadPointer_d(pResponse);

    /* create response with the success */
    pResp->status = g_OTA_SUCCESS_c;
    ImageBlockRspSuccess_t *pSuccessResp =
        (ImageBlockRspSuccess_t*)&pResp->ImageBlockRspPayload.ImageBlockRspSuccess;

    srvBtldr_prepareImageInfo(pSuccessResp->manufacturerCode,
                              pQuery->manufacturerCode);
    *(uint32_t*)pSuccessResp->fileOffset = *(uint32_t*)pQuery->fileOffset;
    /* TBD - The data size has to be calculated by considering the maximum data
     * fit's in a packet. However this value should be less than the
     * data size mentioned in the request packet */
    uint8_t headerSize = (sizeof(ImageBlockResponse_t) + sizeof(pResp->status));
    pSuccessResp->dataSize = App_getAPSMaxPayloadForDevice(g_OTA_CLUSTER_ID_c, &pRequest->src_address,
                                      pRequest->src_addr_mode) - headerSize;
    if (pSuccessResp->dataSize > pQuery->maxDataSize) {
        pSuccessResp->dataSize = pQuery->maxDataSize;
    }
    if (pSuccessResp->dataSize >
        (m_btldrConfig.currentFileInfo.imageSize -
         *(uint32_t*)(pSuccessResp->fileOffset))) {
        pSuccessResp->dataSize = (m_btldrConfig.currentFileInfo.imageSize -\
            *(uint32_t*)(pSuccessResp->fileOffset));
    }

    uint8_t status;
    /* read the data from the eeprom */
    status = eeprom_readBuffer(*(uint32_t*)(pSuccessResp->fileOffset),
                               pSuccessResp->imageData, pSuccessResp->dataSize);
    /* if status is not success */
    if (status != g_SUCCESS_c) {
        /* then change the data size in the response as zero */
        pSuccessResp->dataSize = 0;
    }
#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
//    printf("\n\rDEV:%x->S:%x, MC:%x, IT:%x, FV:%x, FO:%x, DS:%x, FB:%u\n\r",
//           (*(uint16_t*)&pRequest->src_address), pResp->status,
//           *(uint16_t*)pSuccessResp->manufacturerCode,
//           *(uint16_t*)pSuccessResp->imageType,
//           *(uint32_t*)pSuccessResp->fileVersion,
//           *(uint32_t*)pSuccessResp->fileOffset,
//           pSuccessResp->dataSize,
//           buffMgmt_getNumberofFreeBuffers(g_LARGE_BUFFER_c));
#endif /* (g_CLI_SUPPORT_d == 1) */
    /* send the response */
    srvBtldr_sendWithDelay(pRequest, pResponse, g_IMAGE_BLOCK_RESP_CMD_c,
                          sizeof(ImageBlockResponse_t) +
                              (pSuccessResp->dataSize) - 1,
                          g_APS_ACK_REQUIRED_c);

    buffMgmt_freeBuffer(bufferId, 195);
    return g_OTA_APP_SUCCESS_c;
}

/******************************************************************************/

static void srvBtldr_sendNegativeResp (uint8_t status,
                                       APSDE_Data_Indication_t *pRequest,
                                       uint8_t cmdId)
{
    uint8_t bufferId = 0;
    uint8_t responseLen = 0;
    uint8_t *pResponse = g_NULL_c;
    bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
    if (bufferId == g_NO_AVAILABLE_BUFFER_c ||
        (!(pResponse = buffMgmt_getBufferPointer(bufferId)))) {
            return;
    }
    ImageBlockResponse_t *pResp =
            (ImageBlockResponse_t*)\
            g_GetZCLPacketPayloadPointer_d(pResponse);
    pResp->status = status;
    switch (status) {
    case g_OTA_WAIT_FOR_DATA_c:
        {
            /* then send response as WAIT_FOR_DATA */
            ImageBlockRspWaitForData_t *pWaitResp =
                (ImageBlockRspWaitForData_t*)&(pResp->ImageBlockRspPayload.ImageBlockRspWaitForData);
            if (m_btldrConfig.currentTime == 0) {
                (*(uint32_t*)pWaitResp->currentTime) = m_btldrConfig.currentTime;
            }
            else {
                *(uint32_t*)pWaitResp->currentTime =
                    (halCommonGetInt32uMillisecondTick() / 1000u);
            }
            (*(uint32_t*)pWaitResp->requestTime) =
                g_OTA_APP_DEFAULT_UPGRADE_TIME_IN_SECS_c;
            responseLen = sizeof(ImageBlockRspWaitForData_t) +
                sizeof(pResp->status);
        }
        break;
    case g_OTA_APP_NO_IMAGE_AVAILABLE_c:
    case g_OTA_APP_UNSUP_CLUSTER_COMMAND_c:
        responseLen = sizeof(pResp->status);
        break;
    default:
        return;
    }
    /* send response */
    srvBtldr_sendResponse(pRequest, pResponse, cmdId, responseLen,
                          g_APS_ACK_REQUIRED_c);
    /* clear the response buffer */
    buffMgmt_freeBuffer(bufferId, 196);
    return;
}

/******************************************************************************/

static App_Status_t srvBtldr_handleImagePageRequest
                                (APSDE_Data_Indication_t *pRequest)
{
    /* check if the buffer pointer is valid */
    if (pRequest == g_NULL_c) {
        /* if not return failure */
        return g_OTA_APP_FAILURE_c;
    }

    ImagePageRequest_t *pQuery =
        (ImagePageRequest_t*)&pRequest->a_asdu[3];

    if (srvBtldr_verifyImageInfo((uint8_t*) &pQuery->manufacturerCode)
        != g_OTA_APP_SUCCESS_c) {
            /* then send the response as no image */
        srvBtldr_sendNegativeResp(g_OTA_APP_NO_IMAGE_AVAILABLE_c, pRequest,
                                  g_IMAGE_BLOCK_RESP_CMD_c);
            return g_OTA_APP_SUCCESS_c;
        }

    if (m_btldrConfig.currentFileInfo.imageSize <= g_OTA_HDR_MAX_SIZE_IN_BYTES_c) {
        /* create response with the WAIT_FOR_DATA */
        srvBtldr_sendNegativeResp(g_OTA_WAIT_FOR_DATA_c, pRequest,
                                  g_IMAGE_BLOCK_RESP_CMD_c);
        return g_OTA_APP_SUCCESS_c;
    }

    /* if the query file offset is invalid */
    if (*(uint32_t*)pQuery->fileOffset >
        (m_btldrConfig.currentFileInfo.fileStartLocation
         + m_btldrConfig.currentFileInfo.imageSize)) {
             /* then return error as malformed packet */
             return g_OTA_APP_MALFORMED_COMMAND_c;
         }

    /* if there is a ieee address field in the query */
    if (*(uint8_t*)pQuery->fieldControl & 1u) {
        /* TBD - Validation of ieee address of the client is currently not
        * supported */
        /* then send the response as no image */
        srvBtldr_sendNegativeResp(g_OTA_APP_UNSUP_CLUSTER_COMMAND_c, pRequest,
                                  g_IMAGE_BLOCK_RESP_CMD_c);
        return g_OTA_APP_SUCCESS_c;
    }
    /* start the image block responses for page request by the device */
    App_Status_t status = srvBtldr_startPageResp(pRequest);
    /* if status is g_OTA_APP_OUT_OF_RESOURCE_c */
    if (status == g_OTA_APP_OUT_OF_RESOURCE_c) {
        /* then send response as WAIT_FOR_DATA */
        srvBtldr_sendNegativeResp(g_OTA_WAIT_FOR_DATA_c, pRequest,
                                  g_IMAGE_BLOCK_RESP_CMD_c);
        return g_OTA_APP_SUCCESS_c;
    }
    return status;
}

/******************************************************************************/

static App_Status_t srvBtldr_handleUpgradeEndRequest
                                    (APSDE_Data_Indication_t *pRequest)
{
    /* check if the buffer pointer is valid */
    if (pRequest == g_NULL_c) {
        /* if not return failure */
        return g_OTA_APP_FAILURE_c;
    }

    ImageUpgradeEndRequest_t *pQuery =
        (ImageUpgradeEndRequest_t*)&pRequest->a_asdu[3];

    /* if the status in the upgrade end request is not success */
    if (pQuery->status != g_OTA_SUCCESS_c) {
        /* if status is abort */
        if (pQuery->status == g_OTA_ABORT_c) {
            /* then stop the page response of the device */
            srvBtldr_stopDevicePageResp((uint8_t*)&pRequest->src_address,
                                        pRequest->src_addr_mode);
        }
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }

    /* check the image file info with the upgrade file info */
    if (srvBtldr_verifyImageInfo(pQuery->manufacturerCode)
        != g_OTA_APP_SUCCESS_c) {
        /* then return status as malformed command  */
        return g_OTA_APP_MALFORMED_COMMAND_c;
    }

    /* create a buffer for the response */
    uint8_t bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
    /* if no buffer is available */
    if (bufferId == g_NO_AVAILABLE_BUFFER_c) {
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 90 */
        allocatedbuffer[bufferId]= 234;
        freebuffer[bufferId][0]= 234;
        freebuffer[bufferId][1]= 0;
#endif
    /* get the allocated buffer pointer */
    uint8_t *pResponse = buffMgmt_getBufferPointer(bufferId);
    /* if the allocated buffer pointer is invalid */
    if (pResponse == g_NULL_c) {
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }

    ImageUpgradeEndResponse_t *pResp =
        (ImageUpgradeEndResponse_t*)\
            g_GetZCLPacketPayloadPointer_d (pResponse);

    /* send Upgrade End Response */
    srvBtldr_prepareImageInfo(pResp->manufacturerCode,
                              pQuery->manufacturerCode);

    if (m_btldrConfig.currentTime == 0) {
        *(uint32_t*)pResp->currentTime = m_btldrConfig.currentTime;
    }
    else {
        *(uint32_t*)pResp->currentTime = halCommonGetInt32uMillisecondTick();
    }
    *(uint32_t*)pResp->upgradeTime = m_btldrConfig.upgradeTime;

    srvBtldr_sendResponse(pRequest, pResponse, g_UPGRADE_END_RESP_CMD_c,
                          sizeof(ImageUpgradeEndResponse_t),
                          g_APS_ACK_REQUIRED_c);

    buffMgmt_freeBuffer(bufferId, 184);
    return g_OTA_APP_SUCCESS_c;
}

/******************************************************************************/

void srvBtldr_setUpgradeAndCurrentTime (uint32_t upgradeTime,
                                        uint32_t currentTime)
{
    m_btldrConfig.upgradeTime = upgradeTime;
    if (currentTime != g_OTA_INVALID_TIME_c) {
        m_btldrConfig.currentTime = currentTime;
    }
}

/******************************************************************************/

static void srvBtldr_stopDevicePageResp (uint8_t *pDeviceAddress,
                                         uint8_t addrMode)
{
    uint16_t deviceAddress;
    /* if address pointer is invalid or upgrade parameter table is not
     * created*/
    if ((pDeviceAddress == g_NULL_c) ||
        (m_btldrConfig.upgradeParamBuffId == g_NO_AVAILABLE_BUFFER_c)) {
        /* then return */
        return;
    }
    /* if the addressing mode is short addressing mode */
    if (addrMode == g_SHORT_ADDR_MODE_c) {
        /* then copy the short address */
        deviceAddress = *(uint16_t*)pDeviceAddress;
    }
    else {
        /* else get the short address of the device */
        deviceAddress =
            ZigBeeGetShortAddrForSpecifiedIEEEAddr(pDeviceAddress);
    }
    uint8_t i = 0;
    srvBtldr_upgradeParameter_t *pTable = (srvBtldr_upgradeParameter_t*)
        buffMgmt_getBufferPointer(m_btldrConfig.upgradeParamBuffId);
    /* if the pointer to the buffer is invalid */
    if (pTable == g_NULL_c) {
        /* then return */
        return;
    }
    /* look for an entry in the upgrade parameter table for the device */
    for (i = 0; i < g_OTA_APP_BOOTLOADER_MAX_PAGE_REQ_TABLE_ENTRIES_c; i++) {
        if (pTable[i].deviceAddress == deviceAddress) {
            /* if found then clear the entry from the table */
            memUtils_memSet((uint8_t*) &pTable[i], g_NO_AVAILABLE_BUFFER_c,
                            sizeof(srvBtldr_upgradeParameter_t));
            break;
        }
    }
    return;
}

/******************************************************************************/
static App_Status_t srvBtldr_handleQuerySpecificFileRequest
                                    (APSDE_Data_Indication_t *pRequest)
{
    /* check if the buffer pointer is valid */
    if (pRequest == g_NULL_c) {
        /* if not return failure */
        return g_OTA_APP_FAILURE_c;
    }

    /* create a buffer for the response */
    uint8_t bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
    /* if no buffer is available */
    if (bufferId == g_NO_AVAILABLE_BUFFER_c) {
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 90 */
        allocatedbuffer[bufferId]= 235;
        freebuffer[bufferId][0]= 235;
        freebuffer[bufferId][1]= 0;
#endif
    /* get the allocated buffer pointer */
    uint8_t *pResponse = buffMgmt_getBufferPointer(bufferId);
    /* if the allocated buffer pointer is invalid */
    if (pResponse == g_NULL_c) {
        /* then return status as failure */
        return g_OTA_APP_FAILURE_c;
    }

    QuerySpecificFileResponse_t *pResp =
        (QuerySpecificFileResponse_t*)\
            g_GetZCLPacketPayloadPointer_d(pResponse);

    QuerySpecificFileRequest_t *pQuery =
        (QuerySpecificFileRequest_t*)&pRequest->a_asdu[3];

    /* if the upgrade image is intended for a particular device  */
    if (!memUtils_memCompare(m_btldrConfig.currentFileInfo.ieeeAddr,
                             (uint8_t*) g_OTA_APP_INVALID_IEEE_ADDR_c,
                             sizeof(g_OTA_APP_INVALID_IEEE_ADDR_c))) {
        /* then if the image is not for the requested device */
        if (!memUtils_memCompare(pQuery->requestNodeAddr,
                                 (uint8_t*) g_OTA_APP_INVALID_IEEE_ADDR_c,
                                 sizeof(g_OTA_APP_INVALID_IEEE_ADDR_c))) {
            /* then send response as no image available */
            pResp->status = g_OTA_NO_IMAGE_AVAILABLE_c;
            srvBtldr_sendResponse(pRequest, pResponse,
                                  g_QUERY_SPECIFIC_FILE_RESP_CMD_c,
                                  sizeof(pResp->status), g_APS_ACK_REQUIRED_c);;
            buffMgmt_freeBuffer(bufferId, 188);
            return g_OTA_APP_SUCCESS_c;
        }
    }

    if ((*(uint16_t*)pQuery->manufacturerCode !=
         m_btldrConfig.currentFileInfo.manufacturerCode) ||
        (*(uint16_t*)pQuery->imageType > g_OTA_APP_MAX_SUPPORTED_FILE_TYPE_c) ||
            (*(uint16_t*)pQuery->fileVersion !=
             m_btldrConfig.currentFileInfo.fileVersion)) {
            /* then send the response as no image */
            pResp->status = g_OTA_APP_NO_IMAGE_AVAILABLE_c;
            srvBtldr_sendResponse(pRequest, pResponse,
                                  g_QUERY_SPECIFIC_FILE_RESP_CMD_c,
                                  sizeof(pResp->status), g_APS_ACK_REQUIRED_c);
            buffMgmt_freeBuffer(bufferId, 196);
            return g_OTA_APP_SUCCESS_c;
    }

    /* if the stack version in the request is not the stack version of the
    *  image */
    if (*(uint16_t*)pQuery->zigbeeStackVersion !=
        m_btldrConfig.currentFileInfo.stackVersion) {
            /* then send the response as no image */
            pResp->status = g_OTA_NO_IMAGE_AVAILABLE_c;
            srvBtldr_sendResponse(pRequest, pResponse,
                                  g_QUERY_SPECIFIC_FILE_RESP_CMD_c,
                                  sizeof(pResp->status), g_APS_ACK_REQUIRED_c);
            buffMgmt_freeBuffer(bufferId, 188);
            return g_OTA_APP_SUCCESS_c;
        }

    /* send response */
    pResp->status = g_OTA_SUCCESS_c;
    *(uint16_t*)pResp->manufacturerCode = *(uint16_t*)pQuery->manufacturerCode;
    *(uint16_t*)pResp->imageType = *(uint16_t*)pQuery->imageType;
    *(uint32_t*)pResp->fileVersion = *(uint32_t*)pQuery->fileVersion;
    *(uint32_t*)pResp->imageSize =
       *(uint32_t*)&(m_btldrConfig.currentFileInfo.imageSize);

    srvBtldr_sendResponse(pRequest, pResponse,
                          g_QUERY_SPECIFIC_FILE_RESP_CMD_c,
                          sizeof(QuerySpecificFileResponse_t),
                          g_APS_ACK_REQUIRED_c);

    buffMgmt_freeBuffer(bufferId, 188);
    return g_OTA_APP_SUCCESS_c;
}

/******************************************************************************/

static void srvBtldr_updateCurrentFileInfo (void)
{
    /* if the bootloader is not in the ready state */
    if (m_btldrConfig.stateMachine != APP_BTLDR_READY) {
        /* then return */
        return;
    }
    uint8_t bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
    /* if allocated buffer id is invalid */
    if (bufferId == g_NO_AVAILABLE_BUFFER_c) {
        /* then return */
        return;
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 90 */
        allocatedbuffer[bufferId]= 236;
        freebuffer[bufferId][0]= 236;
        freebuffer[bufferId][1]= 0;
#endif
    uint8_t *pHeader = buffMgmt_getBufferPointer(bufferId);
    /* if pointer to the allocated buffer is invalid */
    if (pHeader == g_NULL_c) {
        /* then return */
        return;
    }
    memset(pHeader, 0xff, g_OTA_HDR_MAX_SIZE_IN_BYTES_c);
    uint8_t status;
    /* read the ota header from the EEPROM */
    status = eeprom_readBuffer(m_btldrConfig.currentFileInfo.fileStartLocation,
                               (uint8_t*)pHeader,
                               g_OTA_HDR_MAX_SIZE_IN_BYTES_c);
    /* if status in not success */
    if (status != g_SUCCESS_c) {
        /* then change the application state to init and return */
        m_btldrConfig.stateMachine = APP_BTLDR_INIT;
        return;
    }
    if (g_OTA_HDR_FILE_ID_IN(pHeader) != g_OTA_UPGRADE_FILE_ID_c) {
       /* image not available */
        return ;
    }
    /* update the file information to the ram */
    m_btldrConfig.currentFileInfo.manufacturerCode =
        g_OTA_HDR_MANF_CODE_IN(pHeader);
    m_btldrConfig.currentFileInfo.imageType =
        g_OTA_HDR_IMAGE_TYPE_IN(pHeader);
    m_btldrConfig.currentFileInfo.fileVersion =
        g_OTA_HDR_FILE_VER_IN(pHeader);
    m_btldrConfig.currentFileInfo.stackVersion =
        g_OTA_HDR_ZIGBEE_STACK_VER_IN(pHeader);
    m_btldrConfig.currentFileInfo.imageSize =
        g_OTA_HDR_IMAGE_SIZE_IN(pHeader);
    /* if the ieee address is present in the header */
    if (g_OTA_HDR_FIELD_CONTROL_IN(pHeader) & g_FIELD_CTRL_DEV_SPEC_BIT_MASK_c) {
        /* then read the ieee address */
        memUtils_memCopy(m_btldrConfig.currentFileInfo.ieeeAddr,
                         g_OTA_HDR_FILE_DEST_LOC(pHeader),
                         g_EXTENDED_ADDRESS_LENGTH_c);
    }
    /* if the hardware version is present in the header */
    if (g_OTA_HDR_FIELD_CONTROL_IN(pHeader) & g_FIELD_CTRL_HRD_VER_BIT_MASK_c) {
        /* then read the hardware version */
        m_btldrConfig.currentFileInfo.minHardwareVersion =
            *(uint16_t*)g_OTA_HDR_MIN_HARDWARE_VER_LOC(pHeader);
        m_btldrConfig.currentFileInfo.maxHardwareVersion =
            *(uint16_t*)g_OTA_HDR_MAX_HARDWARE_VER_LOC(pHeader);
    }
    /* image is available */
    m_btldrConfig.imageAvailable = g_TRUE_c;
    buffMgmt_freeBuffer(bufferId, 236);
}

/******************************************************************************/

static App_Status_t srvBtldr_startPageResp (APSDE_Data_Indication_t *pRequest)
{
    srvBtldr_upgradeParameter_t *pTable = g_NULL_c;
    uint8_t i = 0;
    /* if upgrade parameter buffer is not available */
    if (m_btldrConfig.upgradeParamBuffId == g_NO_AVAILABLE_BUFFER_c) {
        /* then allocate a buffer for the table */
        m_btldrConfig.upgradeParamBuffId
                = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
        /* if buffer allocation failed */
        if (m_btldrConfig.upgradeParamBuffId == g_NO_AVAILABLE_BUFFER_c) {
            /* then return status as out of resource */
            return g_OTA_APP_OUT_OF_RESOURCE_c;
        }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 90 */
        allocatedbuffer[m_btldrConfig.upgradeParamBuffId]= 237;
        freebuffer[m_btldrConfig.upgradeParamBuffId][0]= 237;
        freebuffer[m_btldrConfig.upgradeParamBuffId][1]= 0;
#endif
        pTable = (srvBtldr_upgradeParameter_t*)\
            buffMgmt_getBufferPointer(m_btldrConfig.upgradeParamBuffId);
        /* if the pointer to the allocated buffer is invalid */
        if (pTable == g_NULL_c) {
            /* then return status as error */
            return g_OTA_APP_ERROR_c;
        }
        memUtils_memSet((uint8_t*) pTable, g_NO_AVAILABLE_BUFFER_c,
                        g_LARGE_BUFFER_c);
    }
    /* if the pointer to the table is invalid */
    if (pTable == g_NULL_c) {
        /* then get the buffer pointer */
        pTable = (srvBtldr_upgradeParameter_t*)\
            buffMgmt_getBufferPointer(m_btldrConfig.upgradeParamBuffId);
        /* if the pointer to the allocated buffer is invalid */
        if (pTable == g_NULL_c) {
            /* then return status as error */
            return g_OTA_APP_ERROR_c;
        }
    }
    uint16_t requestDeviceAddr;
    if (pRequest->src_addr_mode == g_EXTENDED_ADDR_MODE_c) {
        requestDeviceAddr =
            ZigBeeGetShortAddrForSpecifiedIEEEAddr(pRequest->src_address.IEEE_address);
    }
    else {
        requestDeviceAddr = (*(uint16_t*)&pRequest->src_address);
    }
    ImagePageRequest_t *pQuery =\
        (ImagePageRequest_t*)&pRequest->a_asdu[3];
    /* look for a free location in the table */
    for ( i = 0; i < g_OTA_APP_BOOTLOADER_MAX_PAGE_REQ_TABLE_ENTRIES_c; i++ ) {
        /* if the fileOffset in the entry is invalid */
        if (pTable[i].reqOffset == g_OTA_INVALID_FILE_OFFSET_c) {
            /* then consider the entry as free break */
            break;
        }
        /* if entry is already available */
        if (pTable[i].deviceAddress == requestDeviceAddr) {
            /* then delete the previous entry */
            memUtils_memSet((uint8_t*) &pTable[i], g_NO_AVAILABLE_BUFFER_c,
                            sizeof(srvBtldr_upgradeParameter_t));
            break;
        }
    }

    /* if the index i is equal to max page entries */
    if (i == g_OTA_APP_BOOTLOADER_MAX_PAGE_REQ_TABLE_ENTRIES_c) {
        /* then there is no free space in the table to create an entry return
         * status as out of resource */
        return g_OTA_APP_OUT_OF_RESOURCE_c;
    }
    /* update the values to the entry */
    pTable[i].deviceAddress = requestDeviceAddr;
    pTable[i].endPoint = pRequest->src_endpoint;
    pTable[i].reqOffset = *(uint32_t*)pQuery->fileOffset;
    pTable[i].finalOffset = (pTable[i].reqOffset + *(uint16_t*)pQuery->pageSize);
    if (pTable[i].finalOffset > m_btldrConfig.currentFileInfo.imageSize) {
        /* then clear the entry and return status as out of resource */
        memUtils_memSet((uint8_t*) &pTable[i], g_NO_AVAILABLE_BUFFER_c,
                        sizeof(srvBtldr_upgradeParameter_t));
        return g_OTA_APP_OUT_OF_RESOURCE_c;
    }
    pTable[i].dataSize = pQuery->maxDataSize;
    pTable[i].responseDelay = ((*(uint16_t*)pQuery->responseSpacing) +
                               (randomT() & 0x3ffu));
    pTable[i].minResponseDelay = pTable[i].responseDelay;
    pTable[i].queryJitter = g_OTA_APP_QUERY_JITTER_VALUE_c;
    /* TBD - currentTime value is to set from time cluster */
    if (m_btldrConfig.currentTime == 0) {
        pTable[i].currentTime  = m_btldrConfig.currentTime;
    }
    else {
        pTable[i].currentTime = halCommonGetInt32uMillisecondTick();
    }
    pTable[i].upgradeTime = m_btldrConfig.upgradeTime;

    /* if the page response timer is not running */
    //if (!Is_Timer_Running(g_OTA_APP_PAGE_RESP_TMER_ID_c)) {
    if (0) {
        /* then start the timer */
        uint8_t status = 0;
        status = tmr_startRelativeTimer(g_OTA_APP_PAGE_RESP_TMER_ID_c,
                                        g_OTA_APP_RESPONSE_TIME_IN_MILLISECONDS_c,
                                        srvBtldr_pageResponseTimer);
        /* if timer is not started */
        if (status != g_TIMER_SUCCESS_c) {
            /* then clear the entry and return status as out of resource */
            memUtils_memSet((uint8_t*) &pTable[i], g_NO_AVAILABLE_BUFFER_c,
                            sizeof(srvBtldr_upgradeParameter_t));
            return g_OTA_APP_OUT_OF_RESOURCE_c;
        }
    }
    return g_OTA_APP_SUCCESS_c;
}

/******************************************************************************/

static void srvBtldr_sendBlockRespForPageReq (uint8_t indexInEntry,
                                              srvBtldr_upgradeParameter_t *pTable)
{

    /* if parameters are invalid */
    if ((indexInEntry >= g_OTA_APP_BOOTLOADER_MAX_PAGE_REQ_TABLE_ENTRIES_c)
            || (pTable == g_NULL_c)) {
        /* then return */
        return;
    }
    uint8_t bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
    /* if unable to allocate a buffer */
    if (bufferId == g_NO_AVAILABLE_BUFFER_c) {
        /* then return */
        return;
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 90 */
        allocatedbuffer[bufferId]= 238;
        freebuffer[bufferId][0]= 238;
        freebuffer[bufferId][1]= 0;
#endif
    uint8_t *pResponse = buffMgmt_getBufferPointer(bufferId);
    /* if unable to get a valid pointer to the allocated buffer*/
    if (pResponse == g_NULL_c) {
        /* then free the buffer and return */
        buffMgmt_freeBuffer(bufferId, 238);
        return;
    }
    ImageBlockRspSuccess_t *pResp =
        (ImageBlockRspSuccess_t*)g_GetZCLPacketPayloadPointer_d(pResponse + 1);

    /* append ZCL Header */
    ZCL_Header_t *pZCLHeader = (ZCL_Header_t*)pResponse;
    pZCLHeader->frameControl = (g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                    g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c);
    pZCLHeader->transactionSeq = App_GetTransSeqNumber();
    pZCLHeader->commandId = g_IMAGE_BLOCK_RESP_CMD_c;

    *(uint8_t*)g_GetZCLPacketPayloadPointer_d(pResponse) = g_OTA_SUCCESS_c;
    srvBtldr_prepareImageInfo(pResp->manufacturerCode,
                              (uint8_t*)&m_btldrConfig.currentFileInfo.manufacturerCode);
    *(uint32_t*)pResp->fileOffset = pTable[indexInEntry].reqOffset;

    uint8_t headerSize = (sizeof(ImageBlockResponse_t) + sizeof(uint8_t));
    pResp->dataSize =
        App_getAPSMaxPayloadForDevice(g_OTA_CLUSTER_ID_c,
                                      (Address*)&pTable[indexInEntry].deviceAddress,
                                      g_SHORT_ADDR_MODE_c) -
        headerSize;

    if (pResp->dataSize > pTable[indexInEntry].dataSize) {
        pResp->dataSize = pTable[indexInEntry].dataSize;
    }
    if ((pTable[indexInEntry].reqOffset + pResp->dataSize) >
        pTable[indexInEntry].finalOffset) {
       pResp->dataSize = pTable[indexInEntry].finalOffset -
           pTable[indexInEntry].reqOffset;
    }
    uint8_t status;
    /* read the data from the EEPROM to the packet */
    status = eeprom_readBuffer(pTable[indexInEntry].reqOffset,
                               pResp->imageData, pResp->dataSize);
    /* if status is not a success */
    if (status != g_SUCCESS_c) {
        /* then set reqOffset as finalOffset, clear the buffer and return */
        /* NOTE: This assignment is done so that the response timer will clear
         * the entry from the table */
        pTable[indexInEntry].reqOffset = pTable[indexInEntry].finalOffset;
        buffMgmt_freeBuffer(bufferId, 199);
        return;
    }
#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("\n\rDEV:%x->S:%x, MC:%x, IT:%x, FV:%x, FO:%x, DS:%x\n\r",
           pTable[indexInEntry].deviceAddress, 0,
           m_btldrConfig.currentFileInfo.manufacturerCode,
           m_btldrConfig.currentFileInfo.imageType,
           m_btldrConfig.currentFileInfo.fileVersion,
           pTable[indexInEntry].reqOffset,
           pTable[indexInEntry].dataSize);
    if (pResp->dataSize == 0) {
        printf("\n\rDS:0, pageTableId = %x\n\r", m_btldrConfig.upgradeParamBuffId);
    }
#endif /* (g_CLI_SUPPORT_d == 1) */
    /* send the response */
    App_sendAPSData(g_SHORT_ADDR_MODE_c,
                    (uint8_t*) &pTable[indexInEntry].deviceAddress,
                    pTable[indexInEntry].endPoint, g_OTA_CLUSTER_ID_c,
                    (sizeof(ImageBlockRspSuccess_t) + pResp->dataSize + sizeof(ZCL_Header_t)),
                    pResponse, 0);

    /* change the response parameters in the upgrade parameter table */
    pTable[indexInEntry].reqOffset += pResp->dataSize;

    /* free the buffer */
    buffMgmt_freeBuffer(bufferId, 199);
    return;
}

/******************************************************************************/

static void srvBtldr_pageResponseTimer (uint8_t timerId)
{
    uint8_t entryCount = 0;
    srvBtldr_upgradeParameter_t *pTable = g_NULL_c;
    uint8_t i = 0;
    /* check if the buffer was allocated */
    if (m_btldrConfig.upgradeParamBuffId == g_NO_AVAILABLE_BUFFER_c) {
        /*TBD - if not return */
        return;
    }
    /* get the pointer to the buffer allocated to the page request table */
    pTable =
        (srvBtldr_upgradeParameter_t *)\
            buffMgmt_getBufferPointer(m_btldrConfig.upgradeParamBuffId);
    /* check if the buffer pointer is not equal to NULL */
    if (pTable == g_NULL_c) {
        return;
    }

    /* look for a free location in the table */
    for (i = 0; i < g_OTA_APP_BOOTLOADER_MAX_PAGE_REQ_TABLE_ENTRIES_c; i++) {
        /* if the fileOffset in the entry is invalid */
        if (pTable[i].reqOffset != g_OTA_INVALID_FILE_OFFSET_c) {
            /* then consider for entry */
            entryCount++;
            if (pTable[i].responseDelay > g_OTA_APP_RESPONE_TIME_c) {
            pTable[i].responseDelay -= g_OTA_APP_RESPONE_TIME_c;
            }
            else {
                pTable[i].responseDelay = 0;
            }
            /* check if response time is complete */
            if (pTable[i].responseDelay == 0) {
                /* send the response block */
                srvBtldr_sendBlockRespForPageReq(i, pTable);
                /* if the page size is zero */
                if (pTable[i].reqOffset == pTable[i].finalOffset) {
                    /* then remove the entry from the table */
                    memUtils_memSet((uint8_t*) &pTable[i], 0xff,
                                    sizeof(srvBtldr_upgradeParameter_t));
                }
                else {
                    pTable[i].responseDelay = pTable[i].minResponseDelay;
                }
            }
        }
    }
    if (entryCount != 0x0) {
        /* start the page response timer */
        tmr_startRelativeTimer(g_OTA_APP_PAGE_RESP_TMER_ID_c,
                               g_OTA_APP_RESPONSE_TIME_IN_MILLISECONDS_c,
                               srvBtldr_pageResponseTimer);
        return;
    }
    /* free the upgradeParam Buffer */
    buffMgmt_freeBuffer(m_btldrConfig.upgradeParamBuffId, 211);
    m_btldrConfig.upgradeParamBuffId = g_NO_AVAILABLE_BUFFER_c;
    return;
}

/******************************************************************************/

static void srvBtldr_prepareImageInfo ( uint8_t *pDest, uint8_t *pSrc )
{
    memUtils_memCopy(pDest, pSrc, 8);
}

/******************************************************************************/

static App_Status_t srvBtldr_verifyImageInfo ( uint8_t *pReceived )
{
    if (*(uint16_t*) pReceived
            != m_btldrConfig.currentFileInfo.manufacturerCode) {
        return g_OTA_APP_ERROR_c;
    }
    pReceived += sizeof(uint16_t);
    if (*(uint16_t*) pReceived
            != m_btldrConfig.currentFileInfo.imageType) {
        return g_OTA_APP_ERROR_c;
    }
    pReceived += sizeof(uint16_t);
    if (*(uint32_t*) pReceived
            != m_btldrConfig.currentFileInfo.fileVersion) {
        return g_OTA_APP_ERROR_c;
    }
    return g_OTA_APP_SUCCESS_c;
}

/******************************************************************************/

static void srvBtldr_sendResponse ( APSDE_Data_Indication_t *pRequest,
        uint8_t *pResponse, uint8_t commandId, uint8_t payloadSize,
        uint8_t ackRequired )
{
    /* get destionation address */
    uint8_t *pAddress =
        (pRequest->src_addr_mode == g_EXTENDED_ADDR_MODE_c) ?\
            (pRequest->src_address.IEEE_address) :\
                ((uint8_t*) &pRequest->src_address.short_address);
    /* Append ZCL header */
    ZCL_Header_t *pZCLHeader = (ZCL_Header_t*) pResponse;
    pZCLHeader->frameControl =
        (g_ZCL_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c\
            | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c);
    pZCLHeader->transactionSeq = App_GetTransSeqNumber();
    pZCLHeader->commandId = commandId;
    payloadSize += sizeof(ZCL_Header_t);

    /* send the packet */
    App_sendAPSData(pRequest->src_addr_mode, pAddress, pRequest->src_endpoint,
                    g_OTA_CLUSTER_ID_c, payloadSize, (uint8_t*) pResponse,
                    ackRequired);
}

static void srvBtldr_sendWithDelay  ( APSDE_Data_Indication_t *pRequest,
        uint8_t *pResponse, uint8_t commandId, uint8_t payloadSize,
        uint8_t ackRequired )
{
    uint8_t bufferId;
    uint8_t addressLen;
    /* allocate buffer */
    if (g_NO_AVAILABLE_BUFFER_c ==
//        (bufferId = buffMgmt_allocateBuffer(g_SMALL_BUFFER_c))) {
//        if (g_NO_AVAILABLE_BUFFER_c ==
            (bufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c))) {
                return;
//        }
    }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 166 */
        allocatedbuffer[bufferId]= 166;
        freebuffer[bufferId][0]= 166;
        freebuffer[bufferId][1]= 0;
#endif
    ota_imageBlockRespToAPS_t *pDelayedResp =
        (ota_imageBlockRespToAPS_t*)buffMgmt_getBufferPointer(bufferId);
    /* set the delayed response parameters */
    pDelayedResp->srcAddrMode = pRequest->src_addr_mode;
    addressLen = (pDelayedResp->srcAddrMode == g_EXTENDED_ADDR_MODE_c) ? 8u: \
        2u;
    memUtils_memCopy(pDelayedResp->srcAddr, (uint8_t*)&pRequest->src_address,
                     addressLen);

    /* Append ZCL header */
    ZCL_Header_t *pZCLHeader = (ZCL_Header_t*) pDelayedResp->pPayload;

    pZCLHeader->frameControl =
        (g_ZCL_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c\
            | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c);
    pZCLHeader->transactionSeq = App_GetTransSeqNumber();
    pZCLHeader->commandId = commandId;
    pDelayedResp->payloadSize = (payloadSize + sizeof(ZCL_Header_t));

    pDelayedResp->srcEndPoint = pRequest->src_endpoint;

    *(uint16_t*)pDelayedResp->clusterId = g_OTA_CLUSTER_ID_c;
    pDelayedResp->ackRequired = ackRequired;
    memUtils_memCopy((pDelayedResp->pPayload + sizeof(ZCL_Header_t)),
                     (pResponse + sizeof(ZCL_Header_t)), payloadSize);

    /* enqueue the packet for delayed response */
    Enqueue(m_delayedRespQueue, bufferId);

    if (lastTimeTickAtExpiry == 0) {
        srvBtldr_setDelayedRespExpiryTime();
    }
}

/******************************************************************************/

static void srvBtldr_sendData (uint8_t bufferId)
{
    ota_imageBlockRespToAPS_t *pDelayedResp =
        (ota_imageBlockRespToAPS_t*)buffMgmt_getBufferPointer(bufferId);
    if ((uint8_t*)pDelayedResp == g_NULL_c) {
        return;
    }
    /* send the packet */
    App_sendAPSData(pDelayedResp->srcAddrMode, pDelayedResp->srcAddr,
                    pDelayedResp->srcEndPoint, *(uint16_t*)pDelayedResp->clusterId,
                    pDelayedResp->payloadSize, pDelayedResp->pPayload,
                    pDelayedResp->ackRequired);
    buffMgmt_freeBuffer(bufferId, 185);
}

/******************************************************************************/

static void srvBtldr_setDelayedRespExpiryTime (void)
{
    delayExpiryTime = randomT();
    if (delayExpiryTime > g_SERVER_DELAYED_RESPONSE_MAX_TIME_IN_MS_c) {
        delayExpiryTime %= (g_SERVER_DELAYED_RESPONSE_MAX_TIME_IN_MS_c);
        if (delayExpiryTime < g_SERVER_DELAYED_RESPONSE_MIN_TIME_IN_MS_c) {
            delayExpiryTime += g_SERVER_DELAYED_RESPONSE_MIN_TIME_IN_MS_c;
        }
    }
    lastTimeTickAtExpiry = halCommonGetInt32uMillisecondTick();
}

#endif /* (g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1) */
/******************************************************************************
 * Interrupt Service Routines
 ******************************************************************************/

/* None */

/******************************************************************************
 * End Of File
 ******************************************************************************/
