/******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include <stdbool.h>
#include "ZCL_Common.h"
#include "stack_common.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "zdo_interface.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "AppTimerUtilities.h"
#include "ApplicationThinLayer.h"
#include "AppDiscoveryUtilities.h"
#include "stack_functionality.h"

#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Time_Cluster.h"
#endif    /* g_TIME_CLUSTER_ENABLE_d*/
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Key_Establishment.h"
#include "AppCBKEUtilities.h"
#endif  /*g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d */

#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
#include "AppClientBootloader.h"
#include "ZCL_OTA_Upgrade_Cluster.h"
#endif  /* g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d*/


#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Generic_Tunnel_Cluster.h"
#include "AppGenericTunnelClusterUtilities.h"
#endif  /* g_GENERIC_TUNNEL_CLUSTER_ENABLE_d*/

#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
#include "AppPrepaymentUtilities.h"
#endif   /* g_PREPAYMENT_CLUSTER_ENABLE_d*/

#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
#include "ZCL_Simple_Metering_Cluster.h"
#endif
#endif   /*  g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d */

#if(g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1) && ( g_ZIGBEE_ROUTER_d == 1 )
#include "AppTrustCenterKeepAliveUtilities.h"
#endif /* (g_TRUST_CENTER_SWAP_OUT_ENABLE_d)	 */

/****************************************************************************************
* Global Constants
*******************************************************************************/
#define m_UNICAST_QUERY_BIT_c                     0x80
#define m_RESPONSE_MASK_BIT_c                     0x01
#define m_MASK_MAST_BIT_SET_c                     0x01
#define m_MASK_SYNC_BIT_SET_c                     0x02
#define m_SET_BIT_c( count )                      ( 0x01 < count )

#define m_SITE_ID_c					0x0307
#define m_METER_SERIAL_NUMBER_c		0x0308

/******************************************************************************
* Public Memory declarations
*****************************************************************************************/
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
extern uint8_t readMeterSerialNo[16];
extern uint8_t readMeterSiteId[16];
#endif

#if( g_SERVICE_DISCOVERY_ENABLE_d == 1 )
EsiRegStates_t currEsiRegState;
static uint16_t timeOutPeriod;
/****************************************************************************************
* Private Memory declarations
*****************************************************************************************/
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
static MultipleEsiRegtable_t esitable[g_MULTIPLE_ESI_TABLE_SIZE_c];
static uint8_t esiCount = 0;
static uint8_t esiIndex = 0;
static MultipleEsiTimeSource_t timesource;
#endif
static uint32_t lastTickTime = 0;
static bool unicastQuery = g_FALSE_c;
bool isDeviceJoindOrRejoined = g_FALSE_c;
static bool discoveryWaitState = g_FALSE_c;
static uint8_t discoveryTempBufferId = g_NO_AVAILABLE_BUFFER_c;
ServiceDiscoveryCallback *serviceDiscoveryCallback = g_NULL_c;
/****************************************************************************************
* Function Prototypes
****************************************************************************************/

static void App_serviceDicsoveryTick( void );
void App_setWaitTimePeriod(uint16_t timePeroid, ServiceDiscoveryCallback * callback );
void App_serviceDiscoveryRequest( ServiceDiscoveryCallback * callback );
void App_tranasitionAfterDelay( EsiRegStates_t state, uint16_t time );
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
static void App_searchForAuthoritativeTimeSource( void );
static void App_checkForTimeValidity( void );
static void App_getAppPartnerLinkKey( void );
#endif
static void App_regProcessComplete( void );
void App_setDescoveryData( uint16_t target,
        ServiceDiscoveryCallback *callBack,uint8_t zdoClusterRequest );

void CBKE_KeyInfoStoreDestEndpoint( uint8_t endpoint, uint16_t shortAddress );
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
void App_getMeterDetails( void );
#endif
#endif
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
void App_startOtaServiceDiscovery( void );
#endif
#if (DEBUG_TEST == 1)
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif
/******************************************************************************
* Public Functions
*******************************************************************************/
bool App_isUnicastQuery( void )
{
    return unicastQuery == g_TRUE_c;
}

/****************************************************************************************/
void App_setUnicastQuary( void )
{
    unicastQuery = g_TRUE_c;
}

/****************************************************************************************/
void App_ChangeEsiRegState( EsiRegStates_t state )
{
    ChangeEsiRegState( state );
}

/****************************************************************************************/
void App_holdCurrentDiscovery( EsiRegStates_t newstate )
{
    DiscoveryState_t* pDiscoveryState;

    if ( discoveryTempBufferId != g_NO_AVAILABLE_BUFFER_c ) {
        return;
    }
    discoveryTempBufferId = buffMgmt_allocateBuffer( g_SMALL_BUFFER_c );
    if ( discoveryTempBufferId == g_NO_AVAILABLE_BUFFER_c ) {
        return;
    }
    pDiscoveryState = (DiscoveryState_t*)buffMgmt_getBufferPointer(discoveryTempBufferId);
#if (DEBUG_TEST == 1)
    /* Buffer ID --> 57 */
    allocatedbuffer[discoveryTempBufferId]= 220;
    freebuffer[discoveryTempBufferId][0]= 220;
    freebuffer[discoveryTempBufferId][1]= 0;
#endif
    pDiscoveryState->currState = currEsiRegState;
    pDiscoveryState->unicastQuery = unicastQuery;
    pDiscoveryState->discoveryWaitState = discoveryWaitState;
    pDiscoveryState->timeOutPeriod = timeOutPeriod;
    pDiscoveryState->lastTickTime = lastTickTime;
    pDiscoveryState->serviceDiscoveryCallback = serviceDiscoveryCallback;
    serviceDiscoveryCallback = g_NULL_c;
    discoveryWaitState = g_FALSE_c;
    ChangeEsiRegState( newstate );
}

/****************************************************************************************/
void App_ResumDiscovery( void )
{
    DiscoveryState_t* pDiscoveryState;

    if ( discoveryTempBufferId == g_NO_AVAILABLE_BUFFER_c ) {
        return;
    }
    pDiscoveryState = (DiscoveryState_t*)buffMgmt_getBufferPointer(discoveryTempBufferId);
    currEsiRegState = pDiscoveryState->currState;
    discoveryWaitState = pDiscoveryState->discoveryWaitState;
    unicastQuery = pDiscoveryState->unicastQuery ;
    timeOutPeriod = pDiscoveryState->timeOutPeriod;
    lastTickTime = pDiscoveryState->lastTickTime ;
    serviceDiscoveryCallback = pDiscoveryState->serviceDiscoveryCallback;
    buffMgmt_freeBuffer( discoveryTempBufferId, 144 );
    discoveryTempBufferId = g_NO_AVAILABLE_BUFFER_c;
}

/****************************************************************************************/
void App_serviceDiscoveryInit( void )
{
    currEsiRegState = IDEAL_STATE;
    lastTickTime = 0;
    unicastQuery = g_FALSE_c;
    serviceDiscoveryCallback = g_NULL_c;
    discoveryWaitState = g_FALSE_c;
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
    esiCount = esiIndex = 0;
    timesource.valid = g_FALSE_c;
    isDeviceJoindOrRejoined = g_FALSE_c;
#endif
}

/****************************************************************************************/
void App_serviceDicsoveryTask( void )
{
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
    static uint32_t esiDiscoveryTick = 0;
#endif
    if ( discoveryWaitState == g_TRUE_c ) {
        if ( (App_getcurrentTick() - lastTickTime) > timeOutPeriod ) {
            discoveryWaitState = g_FALSE_c;
            if ( serviceDiscoveryCallback != g_NULL_c )
            {
                servicediscoveryresult_t result = {0x00,0x00,0x0000};
                result.status = App_isUnicastQuery() ? UNICAST_SERVICE_DISCOVERY_TIMEOUT :
                    BROADCAST_SERVICE_DISCOVERY_COMPLETE;
                result.matchAddress = g_NULL_c;
                result.responseData = g_NULL_c;
                result.zdoRspId = 0xFF;
                App_serviceDiscoveryHandler( &result );
                serviceDiscoveryCallback = g_NULL_c;
            }
        }
    }
    else {
        App_serviceDicsoveryTick();
    }
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
    if ( ( App_getcurrentTick() - esiDiscoveryTick ) > g_ONE_HOUR_PERIOD_c ) {
       if ( serviceDiscoveryInProgress() ) {
            return;
       }
       esiDiscoveryTick = App_getcurrentTick();
       serviceDiscoveryCallback = g_NULL_c;
       discoveryWaitState = g_FALSE_c;
       ChangeEsiRegState( ESI_DISCOVERY_STATE );
    }
#endif
}

/****************************************************************************************/

static void App_serviceDicsoveryTick( void )
{
    switch( GetEsiRegState() ) {

    case DISCOVERY_INTIAL_STATE :
    {
        EsiRegStates_t state;
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
        state = CBKE_DISCOVERY_STATE;
#elif ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
        state = ESI_DISCOVERY_STATE;
#else
        state = REGISTRATION_COMPLETE_STATE;
#endif
        App_tranasitionAfterDelay( state, g_BROADCAST_TIMEOUT_PERIOD_c );
        break;
    }
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
    case CBKE_DISCOVERY_STATE:
        App_serviceDiscoveryRequest( App_serviceDiscoveryHandler );
        break;
#endif
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
    case ESI_DISCOVERY_STATE :
        App_serviceDiscoveryRequest( App_serviceDiscoveryHandler );
        break;

    case DISCOVER_IEEE_ADDRESSES :
        App_serviceDiscoveryRequest( App_serviceDiscoveryHandler );
        break;

    case PERFORM_PARTNER_LINK_KEY_EXCHANGE:
        App_getAppPartnerLinkKey();
        break;

    case FIND_AUTHORISED_TIME_SOURE :
        App_searchForAuthoritativeTimeSource();
        break;

    case CHECK_FOR_TIME_VALIDITY :
        App_checkForTimeValidity();
        break;
#endif

#if( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
    case g_PREPAYMENT_DISCOVERY_STATE_c :
        App_serviceDiscoveryRequest(App_serviceDiscoveryHandler );
        break;

    case g_PREPAYMENT_METER_READ_STATE_c :
        App_getMeterDetails();
        break;

    case g_PREPAYMENT_VERIFICATION_STATE_c :
        App_processSupplyChangeEvent();
        break;
#endif
#endif
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
    case BOOTLOAD_DISCOVERY_STATE :
        App_serviceDiscoveryRequest( App_otaUpgradeServiceDiscoveryCallback );
        break;

    case BOOTLOAD_DISCOVERY_SERVER_EUI :
        App_serviceDiscoveryRequest( App_otaUpgradeServiceDiscoveryCallback );
        break;
#endif
    case REGISTRATION_COMPLETE_STATE :
        if ( App_getTCShortAddress() == rsi_zigb_get_self_short_address() ) {
            ChangeEsiRegState( IDEAL_STATE );
        }
        else {
             App_regProcessComplete();
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
            ChangeEsiRegState( CHECK_FOR_TIME_VALIDITY );
#else
            ChangeEsiRegState( IDEAL_STATE );
#endif
        }
        break;

    case IDEAL_STATE :
        break;

    default :
        break;
    }
}

/**********************************************************************************/
static void App_regProcessComplete( void )
{
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
//    App_otaUpgradeClientBtldrInit();
#if( g_OTA_BOOTLOADER_CLIENT_CLI_SUPPORT_d == 0 )
    App_startOtaServiceDiscovery();
#endif
#endif
#if(g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1) && ( g_ZIGBEE_ROUTER_d == 1 )
	App_trustCenterKeepaliveUpdate();
#endif
//#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
//	App_SendChangedProtocolAddress (ZCL_Callback_Get_Device_EndPoint(),
//												g_GENERIC_TUNNEL_CLUSTER_c);
//#endif

}
/**********************************************************************************/
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
static void App_checkForTimeValidity( void )
{
    if ( timesource.valid ) {
        if ( ZCL_CallBackGetCurrentTime()  > timesource.validUntilTime ) {
            ChangeEsiRegState( FIND_AUTHORISED_TIME_SOURE );
            timesource.valid = g_FALSE_c;
            esiIndex = 0;
        }
    }
    else {
        esiIndex = 0;
        ChangeEsiRegState ( FIND_AUTHORISED_TIME_SOURE );
        App_setWaitTimePeriod( g_TIME_SYNC_FAILURE_DELAY_c, g_NULL_c );
    }
}

/**********************************************************************************/
static void App_searchForAuthoritativeTimeSource( void )
{
    uint16_t attributeIds[] = { g_TIME_ATTRIBUTE_c,
                              g_TIME_STATUE_ATTRIBUTE_c,
                              g_TIME_LASTSET_TIME_ATTRIBUTE_c,
                              g_TIME_VALID_UNTIL_TIME_ATTRIBUTE_c };
    uint8_t asdu[10];

    while ( esiIndex < esiCount ) {

        asdu[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        asdu[1] = App_GetTransSeqNumber();
        asdu[2] = g_Read_Attribute_Command_c;
        memUtils_memCopy( &asdu[3], (uint8_t*)attributeIds, sizeof( attributeIds ) );

        App_SendData ( 0x02,
                     (uint8_t*)&esitable[esiIndex].nodeId,
                     esitable[esiIndex].endpoint,
                     g_TIME_CLUSTER_c,
                     sizeof(attributeIds) + 0x03,
                     asdu );
        App_setWaitTimePeriod( g_TIME_ATTRIBUTES_REQUEST_DELAY_c, g_NULL_c );
        esiIndex++;
        return ;
    }
    if ( isDeviceJoindOrRejoined == g_TRUE_c ) {
        isDeviceJoindOrRejoined = g_FALSE_c;
        ChangeEsiRegState( REGISTRATION_COMPLETE_STATE );
    }
    else {
        ChangeEsiRegState( CHECK_FOR_TIME_VALIDITY );
    }
}

/**********************************************************************************/
static void App_getAppPartnerLinkKey( void )
{
    uint8_t tcEUI[8];
    uint8_t nullEUI[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t status = 0x00;
    while ( esiIndex <  esiCount ) {
        if ( esitable[esiIndex].nodeId != App_getTCShortAddress() ) {
            status = rsi_zigb_get_ieee_addr_for_specified_short_addr( App_getTCShortAddress(), tcEUI );
            if ( status && memUtils_memCompare( esitable[esiIndex].nodeEUI,nullEUI, 0x08 ) ) {
                rsi_zigb_request_link_key( tcEUI, esitable[esiIndex].nodeEUI );
                App_setWaitTimePeriod( g_UNICAST_TIMEOUT_PERIOD_c, g_NULL_c );
            }
        }
        esiIndex++;
        return;
    }
    esiIndex = 0x00;
    ChangeEsiRegState( FIND_AUTHORISED_TIME_SOURE );
}
#endif

/**********************************************************************************/
#if( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
void App_getMeterDetails( void )
{
    uint8_t payload[10];

    payload[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    payload[1] = App_GetTransSeqNumber();
    payload[2] = 0x00;
    *((uint16_t*)&payload[3]) = 0x0307;
    *((uint16_t*)&payload[5]) = 0x0308;

    /* Sending Read Request for attributes meter serial no and site id
       to the device from which has got metering cluster as server */
    App_SendData ( g_Short_Address_Mode_c,
                  (uint8_t*)&storedMatchedAddress,
                  storedMatchedEndpoint,
                  g_SIMPLE_METERING_CLUSTER_c,
                  0x07,
                  payload );
    ChangeEsiRegState( g_PREPAYMENT_VERIFICATION_STATE_c );
    App_setWaitTimePeriod( g_UNICAST_TIMEOUT_PERIOD_c, g_NULL_c );
}
#endif /*(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0) */
#endif /*(g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 0) */
/**********************************************************************************/
bool App_discoveryResponseStatus ( uint8_t status )
{
    return ( status & m_RESPONSE_MASK_BIT_c )== m_RESPONSE_MASK_BIT_c;
}

/**********************************************************************************/
bool App_serviceDiscoveryHandler( servicediscoveryresult_t * result )
{
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
    uint8_t keyEstEndPoint = g_INVALID_VALUE_c;
#endif
    uint8_t index;
    if ( ! serviceDiscoveryInProgress() ) {
        return g_FALSE_c;
    }
    if ( App_discoveryResponseStatus( result->status  ) ) {
        if (g_ZDP_Match_Desc_Response_c == (uint8_t)(result->zdoRspId)  ) {
            EndpointList_t* endpointList = ( EndpointList_t*)result->responseData;
            for( index = 0; index < endpointList->count; index++ ) {
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
                if ( GetEsiRegState() == CBKE_DISCOVERY_STATE ) {
                    if ( keyEstEndPoint == g_INVALID_VALUE_c ) {
                        keyEstEndPoint = endpointList->list[index];
                        CBKE_KeyInfoStoreDestEndpoint( keyEstEndPoint, result->matchAddress );
                    }
                }
#endif
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
                if ( GetEsiRegState() == ESI_DISCOVERY_STATE ) {
                    if ( esiCount < g_MULTIPLE_ESI_TABLE_SIZE_c ) {
                        esitable[esiCount].nodeId = result->matchAddress;
                        esitable[esiCount].endpoint = endpointList->list[index];
                        esiCount++;
                    }
                }
#endif
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
                if ( GetEsiRegState() == g_PREPAYMENT_DISCOVERY_STATE_c ) {
                    storedMatchedEndpoint = endpointList->list[index];
                    storedMatchedAddress = result->matchAddress;
                    ChangeEsiRegState( g_PREPAYMENT_METER_READ_STATE_c );
                }
#endif
#endif
            }
        }
    }
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
    if ( GetEsiRegState() == CBKE_DISCOVERY_STATE ) {
        if ( keyEstEndPoint == g_INVALID_VALUE_c ) {
            uint8_t selfLeaveReqAddr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
            App_serviceDiscoveryInit();
            //App_CreateAndSendLeaveRequest( selfLeaveReqAddr );
            rsi_zigb_leave_network();
        }
        else {
            CBKE_ChangeState( INITIATE_KEY_EST_REQ_STATE );
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
            App_tranasitionAfterDelay( ESI_DISCOVERY_STATE,
                                       g_KEY_ESTABLISHMENT_CONCLUDE_DELAY_c );
#else
            ChangeEsiRegState( REGISTRATION_COMPLETE_STATE );
#endif
        }
    }
#endif
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
    if ( GetEsiRegState() == ESI_DISCOVERY_STATE ) {
        if ( result->status == BROADCAST_SERVICE_DISCOVERY_COMPLETE ) {
            if ( esiCount == 0 ) {
                App_tranasitionAfterDelay( ESI_DISCOVERY_STATE,
                                           g_SERVICE_DISCOVERY_FAILURE_DELAY_c );
            }
            else {
                esiIndex = 0;
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
                ChangeEsiRegState( DISCOVER_IEEE_ADDRESSES );
#else
                ChangeEsiRegState( FIND_AUTHORISED_TIME_SOURE );
#endif
            }
        }
    }
    if( GetEsiRegState() == DISCOVER_IEEE_ADDRESSES ) {
        if ( result->status == UNICAST_SERVICE_DISCOVERY_COMPLETE_WITH_RESPONSE ) {
            memUtils_memCopy( esitable[esiIndex].nodeEUI, result->responseData,
                      g_EXTENDED_ADDRESS_LENGTH_c );
        }
        else {
            memUtils_memSet( esitable[esiIndex].nodeEUI, 0x00, g_EXTENDED_ADDRESS_LENGTH_c );
        }
        esiIndex++;
        if ( esiIndex == esiCount ) {
            esiIndex = 0x00;
            ChangeEsiRegState( PERFORM_PARTNER_LINK_KEY_EXCHANGE );
        }
        serviceDiscoveryCallback = g_NULL_c;
    }
#endif
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
    if ( GetEsiRegState() == g_PREPAYMENT_DISCOVERY_STATE_c ) {
        if ( result->status == BROADCAST_SERVICE_DISCOVERY_COMPLETE ) {
            ChangeEsiRegState( g_PREPAYMENT_VERIFICATION_STATE_c );
        }
    }
#endif
#endif
    return g_TRUE_c;
}
/*************************************************************************************/
void App_serviceDiscoveryRequest( ServiceDiscoveryCallback * callback )
{
    uint16_t targetAddr = 0xFFFF;
    uint16_t clusterid = 0xFFFF;

    if ( serviceDiscoveryInProgress() ) {
        return;
    }
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
    if ( GetEsiRegState() == CBKE_DISCOVERY_STATE ) {
        targetAddr = App_getTCShortAddress();
        clusterid = g_KEY_ESTABLISHMENT_CLUSTER_c;
    }
#endif
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
    if ( GetEsiRegState() == ESI_DISCOVERY_STATE ) {
        targetAddr = 0xfffd;
        clusterid = g_TIME_CLUSTER_c;
        esiCount = esiIndex = 0;
    }
    if ( GetEsiRegState() == DISCOVER_IEEE_ADDRESSES ) {
        while( esiIndex < esiCount ) {
           if (!rsi_zigb_get_ieee_addr_for_specified_short_addr( esitable[esiIndex].nodeId,
                              esitable[esiIndex].nodeEUI ) ) {
                memUtils_memSet( esitable[esiIndex].nodeEUI, 0x00, 0x08 );
                rsi_zigb_zdp_send_ieee_addr_request( esitable[esiIndex].nodeId, g_FALSE_c, 0x00, g_FALSE_c );
                App_setDescoveryData( esitable[esiIndex].nodeId, App_serviceDiscoveryHandler,
                                  g_ZDP_IEEE_Addr_Request_c );
                return;
            }
           esiIndex++;
        }
        esiIndex = 0x00;
        serviceDiscoveryCallback = g_NULL_c;
        ChangeEsiRegState( PERFORM_PARTNER_LINK_KEY_EXCHANGE );
        return;
    }
#endif
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
     targetAddr = 0xfffd;
     clusterid = g_SIMPLE_METERING_CLUSTER_c;
#endif
#endif
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
    if ( GetEsiRegState() == BOOTLOAD_DISCOVERY_STATE ) {
        targetAddr = 0xfffd;
        clusterid = g_OTA_UPGRADE_CLUSTER_c;
    }
    else if ( GetEsiRegState() == BOOTLOAD_DISCOVERY_SERVER_EUI ) {
       rsi_zigb_zdp_send_ieee_addr_request( serverNodeId, g_FALSE_c, 0x00, g_FALSE_c );
       App_setDescoveryData( serverNodeId, callback, g_ZDP_IEEE_Addr_Request_c );
       return;
    }
#endif
    //ZDPSendMatchDescriptorsRequest( targetAddr, g_APP_PROFILE_ID_c,
    //                     (uint8_t*)&clusterid, 0x01, g_NULL_c, 0x00, g_TRUE_c, targetAddr );
    rsi_zigb_send_match_descriptors_request( targetAddr, g_APP_PROFILE_ID_c,
                         (uint8_t*)&clusterid, 0x01, g_NULL_c, 0x00, g_TRUE_c, targetAddr );

    App_setDescoveryData( targetAddr, callback, g_ZDP_Match_Desc_Request_c );
}

/**********************************************************************************/
void App_setDescoveryData( uint16_t target, ServiceDiscoveryCallback *callBack,
                           uint8_t zdoClusterRequest )
{
    uint16_t timeout = g_BROADCAST_TIMEOUT_PERIOD_c;
    unicastQuery = g_FALSE_c;
    if ( target < g_BROADCAST_ADDRESS_FOR_ZC_AND_ZR_c ) {
        App_setUnicastQuary();
        timeout = g_UNICAST_TIMEOUT_PERIOD_c;
    }
    App_setWaitTimePeriod( timeout,  callBack );

}

/**********************************************************************************/
void App_tranasitionAfterDelay( EsiRegStates_t state, uint16_t time )
{
    ChangeEsiRegState( state );
    App_setWaitTimePeriod( time, g_NULL_c );
}
/**********************************************************************************/
void App_setWaitTimePeriod( uint16_t timePeroid, ServiceDiscoveryCallback * callback )
{
    discoveryWaitState = g_TRUE_c;
    lastTickTime = App_getcurrentTick();
    timeOutPeriod = timePeroid;
    serviceDiscoveryCallback = callback;
}

/**********************************************************************************/
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
static bool App_isLstSetTimelesser( uint32_t recvdLstTime, uint32_t currentLstTime )
{
    return recvdLstTime > currentLstTime;
}

/**********************************************************************************/
bool App_ProcessTimeReadAttrResponse( uint8_t *readAttibuteRsp, uint8_t rsplength )
{
    uint8_t buffindex = 0x00;
    uint32_t time = 0x00000000;
    uint8_t timeStatus = 0x00;
    uint32_t lastSetTime = 0x00000000;
    uint32_t validUntilTime = 0xFFFFFFFF;

    if ( GetEsiRegState() != FIND_AUTHORISED_TIME_SOURE ) {
        return g_FALSE_c;
    }
    while ( buffindex + 3 < rsplength ) {
        uint16_t attrid;
        uint8_t status;
        memUtils_memCopy( (uint8_t*)&attrid,  readAttibuteRsp + buffindex, 0x02 );
        buffindex += 2;
        status = *( readAttibuteRsp + buffindex );
        buffindex += 1;
        if ( status == g_ZCL_Success_c ) {

            uint8_t attrDataType = *( readAttibuteRsp + buffindex );
            buffindex += 1;

            switch ( attrid )
            {
            case g_TIME_ATTRIBUTE_c :
                memUtils_memCopy( (uint8_t*)&time,  readAttibuteRsp + buffindex, 0x04 );
                break;
            case g_TIME_STATUE_ATTRIBUTE_c :
                timeStatus = *( readAttibuteRsp + buffindex );
                break;
            case g_TIME_LASTSET_TIME_ATTRIBUTE_c :
                memUtils_memCopy( (uint8_t*)&lastSetTime,  readAttibuteRsp + buffindex, 0x04 );
                break;
            case g_TIME_VALID_UNTIL_TIME_ATTRIBUTE_c :
                memUtils_memCopy( (uint8_t*)&validUntilTime,  readAttibuteRsp + buffindex, 0x04 );
                break;

            default :
                break;
            }
            buffindex += ZCL_Get_Data_length( attrDataType );
        }
    }
    if ( ( timeStatus & 0x01 || timeStatus & 0x02 )
          && lastSetTime <= time && time < validUntilTime
          &&(!timesource.valid  || App_isLstSetTimelesser(lastSetTime,timesource.lastSetTime))) {
              timesource.valid          = g_TRUE_c;
              timesource.time           = time;
              timesource.timeStatus     = timeStatus;
              timesource.lastSetTime    = lastSetTime;
              timesource.validUntilTime = validUntilTime;
              ZCL_callbackSetRTCtime( time );
              return g_TRUE_c;

    }
    return g_FALSE_c;
}
#endif

/**********************************************************************************/
#if(g_SE_PROFILE_1_1_d==1)
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if((g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1))
bool App_updateMeterDeviceDetails( uint8_t*readAttrRsp, uint8_t rsplength )
{
    uint8_t buffindex = 0x00;

    if ( GetEsiRegState() != g_PREPAYMENT_VERIFICATION_STATE_c ) {
        return g_FALSE_c;
    }
    while ( buffindex + 3 < rsplength ) {
        uint16_t attrid;
        uint8_t status;
        memUtils_memCopy( (uint8_t*)&attrid,  readAttrRsp + buffindex, 0x02 );
        buffindex += 2;
        status = *( readAttrRsp + buffindex );
        buffindex += 1;
        if ( status == g_ZCL_Success_c ) {
             buffindex += 1;
             switch ( attrid )
             {
                case m_SITE_ID_c :
                    memUtils_memCopy( readMeterSiteId, readAttrRsp+buffindex,
                             *(readAttrRsp+buffindex)+1 );
                    break;
                case m_METER_SERIAL_NUMBER_c :
                    memUtils_memCopy( readMeterSerialNo,readAttrRsp+buffindex,
                             *(readAttrRsp+buffindex)+1 );
                     break;
                default :
                    break;
             }
             buffindex += *(readAttrRsp + buffindex) + 1;
        }
    }
    App_ChangeEsiRegState( g_PREPAYMENT_VERIFICATION_STATE_c );
    return g_TRUE_c;
}
#endif      /*(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0) */
#endif      /* ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 ) */
#endif /*(g_SE_PROFILE_1_1_d==1)	 */
#endif      /* ( g_SERVICE_DISCOVERY_ENABLE_d == 1 ) */
/******************************************************************************
* End Of File
*******************************************************************************/
