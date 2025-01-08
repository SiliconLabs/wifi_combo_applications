#ifndef _APPDISCOVERYUTILITIES_H_
#define _APPDISCOVERYUTILITIES_H_
/*******************************************************************************
* Includes
********************************************************************************/

/*******************************************************************************
* Global Constants
********************************************************************************/
#define g_MULTIPLE_ESI_TABLE_SIZE_c                   0x02
#define g_BROADCAST_TIMEOUT_PERIOD_c                  0x0A
#define g_UNICAST_TIMEOUT_PERIOD_c                    0x05
#define g_TIME_ATTRIBUTES_REQUEST_DELAY_c             0x02
#define g_SERVICE_DISCOVERY_FAILURE_DELAY_c           0x12C
#define g_KEY_ESTABLISHMENT_CONCLUDE_DELAY_c          0x20
#define g_TIME_SYNC_FAILURE_DELAY_c                   0x20
#define g_ONE_HOUR_PERIOD_c                           0x0E10

#define GetEsiRegState()                currEsiRegState
#define ChangeEsiRegState( state )      currEsiRegState = state
#define serviceDiscoveryInProgress()    serviceDiscoveryCallback != g_NULL_c

typedef enum {
    DISCOVERY_INTIAL_STATE,
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
    CBKE_DISCOVERY_STATE,
#endif
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
    ESI_DISCOVERY_STATE,
    DISCOVER_IEEE_ADDRESSES,
    PERFORM_PARTNER_LINK_KEY_EXCHANGE,
    FIND_AUTHORISED_TIME_SOURE,
    CHECK_FOR_TIME_VALIDITY,
#endif
#if( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
    g_PREPAYMENT_DISCOVERY_STATE_c,
    g_PREPAYMENT_METER_READ_STATE_c,
    g_PREPAYMENT_VERIFICATION_STATE_c,
#endif
#endif
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
    BOOTLOAD_DISCOVERY_STATE,
    BOOTLOAD_DISCOVERY_SERVER_EUI,
#endif
    REGISTRATION_COMPLETE_STATE,
    WAIT_FOR_TIMEOUT_STATE,
    IDEAL_STATE,
}EsiRegStates_t;

typedef enum {
  BROADCAST_SERVICE_DISCOVERY_COMPLETE             = 0x00,
  BROADCAST_SERVICE_DISCOVERY_RESPONSE_RECEIVED    = 0x01,
  UNICAST_SERVICE_DISCOVERY_TIMEOUT                = 0x02,
  UNICAST_SERVICE_DISCOVERY_COMPLETE_WITH_RESPONSE = 0x03,
} serviceDiscoveryStatus_t;

/*******************************************************************************
* Type Definitions
********************************************************************************/

typedef struct MultipleEsiTimeSource_tag
{
  bool valid;
  uint32_t time;
  uint8_t timeStatus;
  uint32_t lastSetTime;
  uint32_t validUntilTime;
} __attribute__((__packed__))MultipleEsiTimeSource_t;

typedef struct MultipleEsiRegtable_tag
{
  uint16_t nodeId;
  uint8_t endpoint;
  uint8_t nodeEUI[8];
} __attribute__((__packed__))MultipleEsiRegtable_t;


typedef struct servicediscoveryresult_tag
{
  uint8_t status;
  uint8_t zdoRspId;
  uint16_t matchAddress;
  void* responseData;
} __attribute__((__packed__))servicediscoveryresult_t;

typedef struct EndpointList_tag
{
  uint8_t count;
  uint8_t* list;
} __attribute__((__packed__))EndpointList_t;

typedef bool (ServiceDiscoveryCallback)(servicediscoveryresult_t* result);

typedef struct DiscoveryCurrentState_Tag
{
  EsiRegStates_t currState;
  bool unicastQuery;
  bool discoveryWaitState;
  uint16_t timeOutPeriod;
  uint32_t lastTickTime;
  ServiceDiscoveryCallback *serviceDiscoveryCallback;
} __attribute__((__packed__))DiscoveryState_t;


typedef struct ZDP_MatchDescriptorRequest_tag
{
    uint8_t nwkAddrOfInterest[2];
    uint8_t profileId[2];
    uint8_t numInClusters;
    uint8_t inclusterList[2];
    uint8_t numOutClusters;
    uint8_t outclusterList[2];
}__attribute__((__packed__))ZDP_MatchDescriptorRequest_t;

/*******************************************************************************
* Memory Declarations
********************************************************************************/

/*******************************************************************************
* Function Prototypes
********************************************************************************/
#if( g_SERVICE_DISCOVERY_ENABLE_d == 1 )
void App_ChangeEsiRegState( EsiRegStates_t state );
bool App_isUnicastQuery( void );
bool App_serviceDiscoveryHandler( servicediscoveryresult_t * result );
void App_serviceDiscoveryInit( void );
void App_serviceDicsoveryTask( void );
void App_holdCurrentDiscovery( EsiRegStates_t newstate );
void App_ResumDiscovery( void );
extern ServiceDiscoveryCallback *serviceDiscoveryCallback;
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
bool App_updateMeterDeviceDetails( uint8_t*readAttrRsp, uint8_t rsplength );
#endif   /* ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 ) */
#endif  /* ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 ) */
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
bool App_ProcessTimeReadAttrResponse(uint8_t *readAttibuteRsp, uint8_t rsplength);
#endif  /* ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 ) */
#endif  /* ( g_SERVICE_DISCOVERY_ENABLE_d == 1 ) */

bool App_serviceDiscoveryHandler( servicediscoveryresult_t * result );
void App_serviceDicsoveryTask( void );
/*******************************************************************************
* End Of File
********************************************************************************/
#endif                      /* _APPDISCOVERYUTILITIES_H_ */
