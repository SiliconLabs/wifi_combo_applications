#ifndef __APP_SERVER_BOOTLOAD_H_
/*******************************************************************************
* AppServerBootloader.h
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting OTA cluster from
* server side.
*
*******************************************************************************/
/******************************************************************************
* Includes
******************************************************************************/

/* None */

/******************************************************************************
* Type definitions & structures
******************************************************************************/

typedef enum {
    g_OTA_SUCCESS_c                                         = 0x00u,
    g_OTA_ABORT_c                                           = 0x95u,
    g_OTA_NOT_AUTHORIZED_c                                  = 0x7eu,
    g_OTA_INVALID_IMAGE_c                                   = 0x96u,
    g_OTA_WAIT_FOR_DATA_c                                   = 0x97u,
    g_OTA_NO_IMAGE_AVAILABLE_c                              = 0x98u,
    g_OTA_MALFORMED_COMMAND_c                               = 0x80u,
    g_OTA_UNSUP_CLUSTER_COMMAND_c                           = 0x81u,
    g_OTA_REQUIRE_MORE_IMAGE_c                              = 0x99u
} ota_statusCode_t;

typedef enum {
    g_OTA_APP_SUCCESS_c                                     = 0u,
    g_OTA_APP_NO_IMAGE_AVAILABLE_c                          = 1u,
    g_OTA_APP_MALFORMED_COMMAND_c                           = 2u,
    g_OTA_APP_INVALID_PARAMS_c                              = 3u,
    g_OTA_APP_UNSUP_CLUSTER_COMMAND_c                       = 4u,
    g_OTA_APP_OUT_OF_RESOURCE_c                             = 5u,
    g_OTA_APP_INVALID_BUFFER_c                              = 6u,
    g_OTA_APP_INVALID_ADDRESS_c                             = 7u,
    g_OTA_APP_FAILURE_c                                     = 8u,
    g_OTA_APP_INVALID_STATE_c                               = 9u,
    g_OTA_APP_ERROR_c
} App_Status_t;

typedef enum {
    g_IMAGE_NOTIFY_CMD_c                                    = 0x00u,
    g_QUERY_NEX_IMAGE_REQ_CMD_c                             = 0x01u,
    g_QUERY_NEX_IMAGE_RESP_CMD_c                            = 0x02u,
    g_IMAGE_BLOCK_REQ_CMD_c                                 = 0x03u,
    g_IMAGE_PAGE_REQ_CMD_c                                  = 0x04u,
    g_IMAGE_BLOCK_RESP_CMD_c                                = 0x05u,
    g_UPGRADE_END_REQ_CMD_c                                 = 0x06u,
    g_UPGRADE_END_RESP_CMD_c                                = 0x07u,
    g_QUERY_SPECIFIC_FILE_REQ_CMD_c                         = 0x08u,
    g_QUERY_SPECIFIC_FILE_RESP_CMD_c                        = 0x09u
} ota_commandId_t;

typedef enum {
    g_NOTIF_TYPE_JITTER_c                                   = 0x00u,
    g_NOTIF_TYPE_MANF_CODE_c                                = 0x01u,
    g_NOTIF_TYPE_IMG_TYPE_c                                 = 0x02u,
    /* the rest of the values are reserved */
} ota_imageNotifyPayloadType_t;

typedef struct {
    uint8_t srcAddrMode;
    uint8_t srcAddr[8u];
    uint8_t srcEndPoint;
    uint8_t clusterId[2u];
    uint8_t payloadSize;
    uint8_t ackRequired;
    uint8_t pPayload[];
} __attribute__((__packed__))ota_imageBlockRespToAPS_t;
/******************************************************************************
* Global Constants
******************************************************************************/

#define g_IMAGE_NOTIFY_SUPPORTED_d RSI_ENABLE

#define g_OTA_APP_BOOTLOADER_MAX_SIMULTANEOUS_UPGRADE_c     4u

#define g_OTA_APP_BOOTLOADER_QUEUE_SIZE_c                   4u

#define g_OTA_APP_BOOTLOADER_MAX_PAGE_REQ_TABLE_ENTRIES_c   \
    ((g_LARGE_BUFFER_c)/(sizeof(srvBtldr_upgradeParameter_t)))

#define g_OTA_APP_PAGE_RESP_TMER_ID_c          (g_APP_SYSTEM_UTC_TIMER_c + 7)

#define g_OTA_APP_RESPONE_TIME_c                            100u

#define g_OTA_APP_RESPONSE_TIME_IN_MILLISECONDS_c           \
    (g_OTA_APP_RESPONE_TIME_c * m_MILLISECONDS_c)

#define g_OTA_APP_MAX_SUPPORTED_FILE_TYPE_c                 0xffbfu

#define g_OTA_MAX_JITTER_VALUE_SUPPORTED_c                  0x64u
#define g_OTA_APP_QUERY_JITTER_VALUE_c                      0x04u

#define g_OTA_APP_DEFAULT_UPGRADE_TIME_IN_SECS_c            90u

#define g_OTA_CLUSTER_END_POINT_c                           0x01u

#define g_OTA_CLUSTER_ID_c                                  0x0019u

#define g_OTA_INVALID_FILE_OFFSET_c                         0xffffffffu

#define g_OTA_INVALID_TIME_c                                0xffffffffu

#define g_OTA_UPGRADE_FILE_ID_c                             0x0BEEF11Eu

#define g_OTA_DEFAULT_MANF_CODE_c                           0x01u

#define g_OTA_DEFAULT_IMG_TYPE_c                            0x02u

#define g_OTA_DEFAULT_FILE_VERSION_c                        0x03u

#define g_OTA_DEFAULT_STACK_VERSION_c                       0x04u

#define g_OTA_DEFAULT_IMG_SIZE_c                            0x00u
#define g_OTA_SEC_IN_MSECS_c                                ((uint32_t)1000u)

extern const uint8_t g_OTA_APP_INVALID_IEEE_ADDR_c[8];

/******************************************************************************
* Public Memory declarations
******************************************************************************/

/* None */

/******************************************************************************
* Private Memory declarations
******************************************************************************/

/* None */

/******************************************************************************
* Function Prototypes
******************************************************************************/
void App_otaUpgradeServerEventHandler(ZCL_Event_t *pEvent,
                                      APSDE_Data_Indication_t *pIndicationbuffer);

/******************************************************************************
* Public Functions
******************************************************************************/

/* None */

/******************************************************************************
* Private Functions
******************************************************************************/

void srvBtldr_init (void);

/******************************************************************************/

void srvBtldr_main (void);

/******************************************************************************/

void srvBtldr_setImageUpgradeTime (uint8_t upgradeTime);

/******************************************************************************/

void srvBtldr_enqueueBuffer (uint8_t bufferId);


/******************************************************************************/

void srvBtldr_dequeueBuffer (uint8_t bufferId);

/******************************************************************************/

App_Status_t srvBtldr_isQueueFree (void);

/******************************************************************************/

void srvBtldr_convertToZigBee (void);

/******************************************************************************/

App_Status_t srvBtldr_handleImageBlockRequest (APSDE_Data_Indication_t *pRequest);

/******************************************************************************/

App_Status_t srvBtldr_handleImagePageRequest (APSDE_Data_Indication_t *pRequest);

/******************************************************************************/

App_Status_t srvBtldr_handleQueryNextImageRequest (APSDE_Data_Indication_t *pRequest);

/******************************************************************************/

App_Status_t srvBtldr_handleQuerySpecificFileRequest (APSDE_Data_Indication_t *pRequest);

/******************************************************************************/

App_Status_t srvBtldr_handleUpgradeEndRequest (APSDE_Data_Indication_t *pRequest);

/******************************************************************************/

void srvBtldr_pageResponseTimer (uint8_t timerId);

/******************************************************************************/

App_Status_t srvBtldr_sendImageNotify (uint8_t jitter, uint8_t *pDeviceAddress);

/******************************************************************************/

void srvBtldr_abortUpgrade (void);

/******************************************************************************/

void srvBtldr_imageReady (uint32_t imageAddress);

/******************************************************************************/

void srvBtldr_setUpgradeAndCurrentTime (uint32_t upgradeTime,
                                        uint32_t currentTime);
/******************************************************************************/
#if (g_OTA_BOOTLOADER_SERVER_CLI_SUPPORT_d == 1)

void App_otaConfigServerDelayedResponseTime(uint32_t minTime,uint32_t maxTime );
#endif
/******************************************************************************
* Interrupt Service Routines
******************************************************************************/

/* None */

/******************************************************************************
* End Of File
******************************************************************************/
#define __APP_SERVER_BOOTLOAD_H_
#endif /* __APP_SERVER_BOOTLOAD_H_ */
