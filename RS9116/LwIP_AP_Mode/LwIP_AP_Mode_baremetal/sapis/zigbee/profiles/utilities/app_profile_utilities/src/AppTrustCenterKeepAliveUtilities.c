/*******************************************************************************
* AppTrustCenterKeepAliveUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Trust Center
*   SwapOut Feature
*
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ApplicationThinLayer.h"
#include "memory_utility.h"
#include "buffer_management.h"
#include "zdo_interface.h"
#include "apsde_interface.h"
#include "nlme_interface.h"
#include "ZCL_Common.h"
#include "stack_functionality.h"
#include "AppDiscoveryUtilities.h"

#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Key_Establishment.h"
    #include "AppCBKEUtilities.h"
#endif

#if ( g_ZIGBEE_ROUTER_d == 1 )
extern	uint8_t g_trust_center_swapout_pan_id_c[2];
#endif

#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

#if(g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1) && ( g_ZIGBEE_ROUTER_d == 1 )
#include "AppTrustCenterKeepAliveUtilities.h"
#include "AppTimerUtilities.h"
/*******************************************************************************
* Global Constants
********************************************************************************/
/******************************************************************************
* Public Memory declarations
*******************************************************************************/
#define m_KEEP_ALIVE_CMD_LENGTH_c                 0x05
/******************************************************************************
* Private Memory declarations
*******************************************************************************/
/* The device protocol address . The first byte is the length and this address
is stored  by all endpoints*/

static TrustCenterKeepaliveState state = KEEP_ALIVE_INITIAL_STATE;
static uint8_t failures = 0;
static bool waitForResponse;
/*****************************************************************************
* Function Prototypes
*******************************************************************************/
static void App_sendTrustCenterKeepaliveSignal(void);
static void App_trustCenterKeepaliveStart(void);
static void App_initiateTrustCenterSearch(void);
static void App_messageTimeout(void);

/*******************************************************************************
* Public Functions
********************************************************************************/
void App_trustCenterKeepaliveStop(void)
{
	state = KEEP_ALIVE_INITIAL_STATE;
	waitForResponse = g_FALSE_c;
	memUtils_memSet( g_trust_center_swapout_pan_id_c, 0xFF, 2 );
}
/******************************************************************************/

void App_trustCenterKeepaliveUpdate(void)
{
	if (state == KEEP_ALIVE_INITIAL_STATE) {
        /*call for keep alive start */
		App_trustCenterKeepaliveStart();		
		return;
	}
    else if (state == INITIATE_TRUST_CENTER_SEARCH_STATE) {
		if ( ZigBeeNetworkState() == g_ZigBeeJoinedNWK_c) {
			state = KEEP_ALIVE_SIGNAL_SEND_STATE;
		}
        else {

            if(ZigBeeNetworkRestore() == g_SUCCESS_c) {
                state = KEEP_ALIVE_SIGNAL_SEND_STATE;
            }
            else {
                App_trustCenterKeepaliveStop();
            }
        }
	}
}

/******************************************************************************/
void App_trustCenterKeepaliveTask(void)
{
    static uint32_t lastTickTime = 0;
    static uint16_t count =0;

    if ( ( App_getcurrentTick() - lastTickTime ) > g_ONE_MINUTES_IN_SECONDS_c  ) {
        lastTickTime = App_getcurrentTick();
        count++;
        switch ( state ) {
        case KEEP_ALIVE_SIGNAL_SEND_STATE:
            if( count >= g_KEEP_ALIVE_SEND_SIGNAL_RATE_IN_MINUTES_c ) {
                if (waitForResponse) {
                    App_messageTimeout();
                    return;
                }
                App_sendTrustCenterKeepaliveSignal();
                count =0 ;
            }
            break;
        case INITIATE_TRUST_CENTER_SEARCH_STATE:
            App_trustCenterKeepaliveUpdate();

        default:
            break;
        }
    }
}

/*******************************************************************************
* Public Functions
*******************************************************************************/
/*Initially start trust center keep alive signal */
static void App_trustCenterKeepaliveStart(void)
{
  /* Check whether the node is an TRUST CENTER */
    if ( ZigBeeGetSelfShortAddress( )  == App_getTCShortAddress() ) {
        return;
    }
    failures = 0;
    state = KEEP_ALIVE_SIGNAL_SEND_STATE;
    App_sendTrustCenterKeepaliveSignal();
}
/******************************************************************************/
static void App_sendTrustCenterKeepaliveSignal( void )
{
	ZigBeeAPSDEDataRequest_t *p_apsde_data_request;
	uint8_t buffindex;
	Address destination;
	buffindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
	uint16_t AttributeId = 0x0000;
	
	if( g_NO_AVAILABLE_BUFFER_c != buffindex ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 1 */
		allocatedbuffer[buffindex]= 203;
		freebuffer[buffindex][0]= 203;
		freebuffer[buffindex][1]= 0;
#endif
        p_apsde_data_request = ( ZigBeeAPSDEDataRequest_t * )buffMgmt_getBufferPointer( buffindex );

		p_apsde_data_request->DestEndpoint   = CBKE_KeyInfoGetDestEndpoint();
		p_apsde_data_request->SrcEndpoint    =  ZCL_Callback_Get_Device_EndPoint();
        p_apsde_data_request->ProfileId      =  g_APP_PROFILE_ID_c;
		
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )     		
        p_apsde_data_request->ClusterId      =  g_KEY_ESTABLISHMENT_CLUSTER_c;
#endif	
        p_apsde_data_request->AsduLength     =  m_KEEP_ALIVE_CMD_LENGTH_c;
        p_apsde_data_request->aPayload[0]    =  g_ZCL_FC_PROFILE_WIDE_COMMAND_c |
            g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        p_apsde_data_request->aPayload[1]    =  App_GetTransSeqNumber();
        p_apsde_data_request->aPayload[2]    = g_Read_Attribute_Command_c;
		
        memUtils_memCopy( &p_apsde_data_request->aPayload[3],(uint8_t*)&AttributeId,2);
		
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )  	
        p_apsde_data_request->TxOptions   =
            App_getClusterSecurity( g_KEY_ESTABLISHMENT_CLUSTER_c );
#endif		
		
        p_apsde_data_request->Radius = g_DEFAULT_RADIUS_c;
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )  			
        memUtils_memSet( (uint8_t*)&destination.short_address,g_TRUST_CENTER_c ,
                g_SHORT_ADDRESS_LENGTH_c );
#endif
        ZigBeeSendUnicastData(ZigBee_Outgoing_Direct,
                              destination,  p_apsde_data_request);
		waitForResponse = g_TRUE_c;
    }
    buffMgmt_freeBuffer(buffindex, 203);
}

/******************************************************************************/

static void App_initiateTrustCenterSearch(void)
{
  	uint8_t  status;
#if(g_ZIGBEE_ROUTER_d == 1)
	status = ZigBeeRejoinNetwork(g_FALSE_c); 	
#endif	
	if (status == g_SUCCESS_c) {
		 state = INITIATE_TRUST_CENTER_SEARCH_STATE;
	 }
}
/******************************************************************************/
static void App_messageTimeout(void)
{
    waitForResponse = g_FALSE_c;
    if (failures != 255) {
        failures++;
    }
    if ( failures >= g_TRUST_CENTER_KEEPALIVE_FAILURE_LIMIT_c ) {
        memUtils_memCopy( g_trust_center_swapout_pan_id_c, (uint8_t*)m_GET_g_NWK_panid_c,2 );
        App_initiateTrustCenterSearch();
    }
}
/******************************************************************************/
void App_keepAliveCmdRsp(uint16_t ShortAddress )
{
  /* */
	if( ShortAddress == App_getTCShortAddress() && state == KEEP_ALIVE_SIGNAL_SEND_STATE ) {
		waitForResponse = g_FALSE_c;
		failures = 0;
	}
	else {
	   waitForResponse = g_TRUE_c;
    }
    return ;
}
/******************************************************************************/

#endif
/******************************************************************************/
	
