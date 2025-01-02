#ifndef _APPBOOTLOADERCLIENT_H_
#define _APPBOOTLOADERCLIENT_H_
/*******************************************************************************
* Includes
********************************************************************************/
/******************************************************************************
* Global Constants
********************************************************************************/
#define g_OTA_SERVICE_DISCOVERY_CMD_d                   1u

#define g_MAX_SIGNERS_c                                 0x01
#define g_MAX_CA_KEYS_c                                 0x01

#define g_INVALID_HARDWARE_VERSION_c                    0xFFFF
#define g_WAIT_FOREVER_UPGRADE_MESSAGE_c                0xFFFFFFFF

#define g_START_DOWNLOAD_OFFSET_c                       (g_OTA_FILE_LOCATION_IN_EEPROM_c)

#define g_MS_IN_SEC_c                                   1000 //1024u
#define g_MIN_IN_SEC_c						            (60 * g_MS_IN_SEC_c)
#define g_HOURS_IN_SEC_c					            (g_MIN_IN_SEC_c * 60)

/* This defines how long to wait for a message response before considering
   it is an error */
#define g_MESSAGE_RESPONSE_TIMEOUT_SECONDS_c            ((uint32_t)(0.8 * g_MS_IN_SEC_c ))

/* This defines size in bytes max data server can send in a image block
   response */
#define g_MAX_CLIENT_DATA_SIZE_c                        40

/* This defines Page size for Image Page request */
#define g_OTA_CLIENT_PAGE_REQUEST_SIZE_c                1024

/* This defines min spacing in ms each image block response from server
   due to image page request */
#define g_OTA_CLIENT_PAGE_REQUEST_SPACING_c             (3 * g_MS_IN_SEC_c)


/* This defines max timeout( start of current page to end of current page )
  the ota client wait for last packet of current page.if not received within
  time out, re request the page */

  /* ( Ex: page size / 24 bytes * 1 second each packet ) + 5 packet miss delay */

#define g_OTA_CLIENT_PAGE_REQUEST_ALL_RESPONSE_DELAY        (10 * g_MS_IN_SEC_c)

#define g_OTA_CLIENT_PAGE_RESPONSE_MAX_INTER_FRAME_DELAY    (10 * g_MS_IN_SEC_c )

#define g_OTA_CLIENT_PAGE_REQUEST_INIT_ERROR_DELAY     (15 * g_MS_IN_SEC_c)



/* This defines min spacing in ms each missed image block response from server
   due to image page request */
#define g_OTA_PAGE_REQUEST_MISSED_PACKET_DELAY_c        ( 2 * g_MS_IN_SEC_c )

/* This defines number of page request retries, before client abort current
   download process */
#define g_OTA_PAGE_REQUEST_RETRIES_THRESHOLD_c          2

/* This defines number of image block response missed due to page request
   client re request page */
#define g_OTA_PAGE_REQ_BLOCK_MISS_THRESHOLD_c   (g_SMALL_BUFFER_c/sizeof (pageDataMissedInfo_t))

/* This defines max time required to get app link key from Trust Center */
#define g_OTA_APP_LINK_KEY_WAIT_SETUP_TIME_c            ( 10 * g_MS_IN_SEC_c )

/* How often the client will ask an upgrade image being downloaded */
#define g_OTA_CLIENT_DOWNLOAD_DELAY_c		            ((uint32_t)(2.0 * g_MS_IN_SEC_c))

/* This defines the maximum jitter time for every block request */
#define g_OTA_CLIENT_BLOCK_REQ_MAX_JITTER_c             ((uint32_t)(1.5 * g_MS_IN_SEC_c))

/* How often the OTA client looks for an OTA server when there is NOT
  one present in the network */
#define g_OTA_SERVER_DISCOVERY_DELAY_c		            (120 * g_MS_IN_SEC_c)

/* How often the OTA client looks updated image query to OTA server when it
  it is not downloading */
#if (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 0)
#define g_OTA_NEXT_IMAGE_QUERY_DELAY_c			        ( 30 * g_MS_IN_SEC_c )
#endif /* (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 0) */

/* The maximum number of sequential errors when downloading that will trigger
   the OTA client to abort the download.*/
#define g_OTA_CLIENT_DOWNLOAD_ERROR_THRESHOLD_c  		10

/* The maximum number of sequential errors when asking the OTA server when to
   upgrade that will cause the OTA client to apply the upgrade without the
   server telling it to do so */
#define g_OTA_CLIENT_UPGRADE_WAIT_THRESHOLD_c           5

/* Sever told the client upgrade immediately, timeout values before we
   start install */
#define g_OTA_IMMEDIATE_UPGRADE_DELAY_c                  2

/* Maximum wait time that the client would wait to retrive data or to be
   upgraded. If the server is not ready for the operation */
#define g_TIMEOUT_MAX_WAIT_TIME_c		                        ( 60 * g_MIN_IN_SEC_c )

/* If the server sent us an invalid delay time for the an image block response,
   this is how long we will delay before getting the next block */
#define g_CALCULATE_TIME_ERROR_IMAGE_BLOCK_DELAY_MIN_c          ( 5 * g_MIN_IN_SEC_c )

/* If the server sent us an invalid delay time for the an upgrade end response,
   this is how long we will delay before getting the next block */
#define g_CALCULATE_TIME_ERROR_UPGRADE_END_RESPONSE_DELAY_HR_c  ( 1 * g_HOURS_IN_SEC_c )

/* Upgrade end request retry timeout value, if Upgrade end respone is not sent
   from server after sending Upgrade end request */
#define g_RUN_UPGRADE_REQUEST_DELAY_MIN_c                       ( 10 * g_MIN_IN_SEC_c )

/* When the server asks us to wait indefinitely to apply an upgrade,
   we will ask them again in this much time */
#define g_WAIT_FOR_UPGRADE_DELAY_HR_c                           ( 1 * g_HOURS_IN_SEC_c )



/******************************************************************************/

/* Time delay between OTA response & next OTA request */
#if (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 0)

#define g_REQUEST_TIME_DELAY_c        		  ((uint32_t)(0.1 * g_MS_IN_SEC_c))
#endif   /*g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d */


/* Timeout period for each OTA request */
#define g_RESPONSE_TIME_OUT_c                                   ((uint32_t)(10u * g_MS_IN_SEC_c))

/******************************************************************************/
typedef enum {
    APP_CLIENT_BTLDR_INIT,
    APP_CLIENT_BTLDR_DISCOVER_SERVER,
    APP_CLIENT_BTLDR_GET_SERVER_EUI,
    APP_CLIENT_BTLDR_APP_LINK_KEY,
    APP_CLIENT_BTLDR_QUERY_NEXT_IMAGE,
    APP_CLIENT_BTLDR_DOWNLOAD,
    APP_CLIENT_BTLDR_VERIFY_IMAGE,
    APP_CLIENT_BTLDR_WAITING_FOR_UPGRADE_MESSAGE,
    APP_CLIENT_BTLDR_COUNTDOWN_TO_UPGRADE
}__attribute__((__packed__))clientOtaState_t;

typedef enum {
  APP_OTA_NO_PAGE_REQUEST              = 0,
  APP_OTA_WAITING_PAGE_REQUEST_REPLIES = 1,
  APP_OTA_RETRY_MISSED_PACKETS         = 2,
  APP_OTA_PAGE_REQUEST_COMPLETE        = 3,
  APP_OTA_RETRY_PAGE_REQUEST           = 4,
  APP_OTA_PAGE_REQUEST_ERROR           = 5
}ClientPageRequestState_t;


typedef enum {
  APP_OTA_IMAGE_VERIFY_SUCCESS =        0,
  APP_OTA_DOWNLOAD_TIME_OUT           = 1,
  APP_OTA_IMAGE_VERIFY_FAILED         = 2,
  APP_OTA_SERVER_ABORTED              = 3,
  APP_OTA_CLIENT_ABORTED              = 4,
  APP_OTA_OUT_OF_RESOURCE             = 5,
  APP_OTA_STORAGE_ERROR               = 6
}App_otaStatus_t;

/*******************************************************************************
* Type Definitions
********************************************************************************/

typedef struct OtaImageInfo_Tag {
  uint16_t manufacturerId;
  uint16_t imageTypeId;
  uint32_t firmwareVersion;
} __attribute__((__packed__))OtaImageInfo_t;

typedef struct pageDataMissedInfo_Tag {
  uint32_t fileOffeset;
  bool inUse;
} __attribute__((__packed__))pageDataMissedInfo_t;

typedef struct IssuerCAKeys_Tag {
  uint8_t issuer[8];
  uint8_t capublickey[22];
} __attribute__((__packed__))IssuerCAKeys_t;

typedef uint8_t authorizedSigner[8];
/*******************************************************************************
* Memory Declarations
********************************************************************************/
extern uint16_t serverNodeId;
/*******************************************************************************
* Function Prototypes
********************************************************************************/
void App_otaUpgradeClientBtldrTask( void );
void App_otaUpgradeClientBtldrInit( void );
#if (g_SERVICE_DISCOVERY_ENABLE_d == 1)
bool App_otaUpgradeServiceDiscoveryCallback( servicediscoveryresult_t *result );
#endif //#if (g_SERVICE_DISCOVERY_ENABLE_d == 1)
void App_otaUpgradeClientEventHandler( ZCL_Event_t *pZCLEvent,
                                       APSDE_Data_Indication_t *pBuffer );
void App_getOtaUpgradeImageInformation( OtaImageInfo_t *ImageInfo );

void App_downLoadedVerifyAndFinish( App_otaStatus_t status );
void App_setOtaUpgradeImageInformation( OtaImageInfo_t *ImageInfo );
void App_sendRequest( uint8_t cmdId, ZCL_Status upgrdEndStatus, uint32_t timer );

void App_startOtaServiceDiscovery( void );
void App_setOrResetPageRequest( bool edpagerequest );

#if (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 1)

void App_clientBootloaderSetImageType (uint16_t value);
void App_clientBootloaderSetManufatureCode (uint16_t value);
void App_clientBootloaderSetFileVersion (uint32_t value);
void App_clientBootloaderSetPollTime (uint32_t value);
void App_clientBootloaderSetStackVersion (uint16_t value);

bool App_UpdateSignerIEEEAddress(uint8_t *ieeeAddress, uint8_t index);
void App_setRequestTimeDelay (uint32_t delay);


#if (g_OTA_SERVICE_DISCOVERY_CMD_d == 1u)
void App_otaUpgradeStart (void);
#endif /* (g_OTA_SERVICE_DISCOVERY_CMD_d == 1u) */

#endif /* (g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 1) */

/*******************************************************************************
* End Of File
********************************************************************************/
#endif                      /* _APPBOOTLOADERCLIENT_H_ */
