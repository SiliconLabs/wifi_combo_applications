/****************************************************************************************
* AppTunnelingClusterUtilities.c
*****************************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Tunneling cluster
*
*****************************************************************************************/
/****************************************************************************************
* Includes
*****************************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "memory_utility.h"
#include "ApplicationThinLayer.h"
#include "AppTimerUtilities.h"
#include "zdo_interface.h"
#if( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
#include "AppTunnelingClusterUtilities.h"
#include "ZCL_Tunneling_Cluster.h"
#endif

#if( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Generic_Tunnel_Cluster.h"
#endif

/***************************************************************************************
* Global Constants
****************************************************************************************/

/***************************************************************************************
* Public Memory declarations
****************************************************************************************/
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
TunnelingServerTunnel_t servertunnels[ g_MAX_TUNNELING_SERVER_TUNNEL_LIMIT_c ];
#endif
#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
TunnelingClientTunnel_t clienttunnels[ g_MAX_TUNNELING_CLIENT_TUNNEL_LIMIT_c ];
#endif
#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
APSDE_Data_Indication_t *ptempIndBuffer;
uint8_t g_Tunnel_Max_Default_Value[2]={0x05,0xDC};
#endif
/***************************************************************************************
* Private Memory declarations
****************************************************************************************/
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
static uint8_t tunnelServerEndpoint = 0xFF;

#if ( g_SE_PROFILE_1_2_d == 1 )
/*Protocols supported by the tunneling cluster */
uint8_t protocolList[g_MAX_TUNNELING_SUPPORTED_PROTOCOLS_c] =
	{ g_DLMS_COSEM_IEC62056_c, g_IEC61107_c, g_ANSIC12_c, g_MBUS_c, g_SML_c, g_CLIMATETALK_c};	
#endif       /*g_SE_PROFILE_1_2_d */


#endif      /* ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
uint8_t protocolId;
uint16_t manufacturerCode;
bool flowControl;
#endif      /* ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/***************************************************************************************
* Function Prototypes
****************************************************************************************/
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
static void App_handleTunnelRequest( uint8_t protocolId,
               uint16_t manufacturerCode,  bool flowControlSupport, uint8_t seqNum );
static void App_handleCloseTunnelcommand( uint16_t tunnelId,
                                                   APSDE_Data_Indication_t *pBuffer );

#if ( g_SE_PROFILE_1_2_d == 1)
static void App_CreateAndSendSupportedTunnelProtocols( uint8_t offset,
											APSDE_Data_Indication_t *pBuffer );
void App_sendCloseTimeoutNotificationCommand(uint16_t nwkSrcAddress,
											 				uint16_t tuunelId);
#endif

#endif              /* ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 ) */


#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
static void App_handleTunnelResponseCommand( uint16_t tunnelId,
                           uint8_t tunnelStatus,   APSDE_Data_Indication_t *pBuffer );

#endif              /* ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
static void App_CreateAndSendTunnelDataAck( uint16_t tunnelId,
                    uint16_t numberOfBytesLeft, bool fromServer, uint8_t seqNum );

static void App_handleTunnelDataTransferError( uint16_t tunnelId,
       uint8_t DataStatus, bool fromServer, APSDE_Data_Indication_t *pBuffer );

static void App_handleTunnelDataAckandReady( uint16_t tunnelId,
        uint16_t numOfBytesLeft, bool fromServer,  APSDE_Data_Indication_t *pBuffer );

static void App_CreateAndSendTransferDataErrorCommand ( uint16_t tunnelId,
                                  uint8_t status, bool fromServer, uint8_t seqNum );
#endif


/***************************************************************************************
* Public Functions
****************************************************************************************/

#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
void App_TunnelTableIntit( void )
{
    uint8_t index;
#if( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
	uint8_t Status =g_ZCL_Failure_c;

	uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
#endif
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
    for ( index = 0; index < g_MAX_TUNNELING_SERVER_TUNNEL_LIMIT_c; index++ ) {
        servertunnels[index].inUse = g_FALSE_c;
    }
#endif
#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
     for ( index = 0; index < g_MAX_TUNNELING_CLIENT_TUNNEL_LIMIT_c; index++ ) {
         clienttunnels[index].inUse = g_FALSE_c;
    }
#endif
#if( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
	
	 Status = App_ReadWriteAttribute( EndPoint,
                            g_GENERIC_TUNNEL_CLUSTER_c,
                            g_MAXIMUM_INCOMING_TRANSFER_SIZE_ATTRIBUTE_c,
                            (uint8_t*)&g_Tunnel_Max_Default_Value,
                            g_Write_Attribute_Data_c );
	 if(Status != g_ZCL_Success_c) {
		return;
	 }
#endif	
}

/****************************************************************************************/
void App_tunnelEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
    switch ( pZCLEvent->eventId )
    {
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
    case g_RequestTunnel_Event_c :
        tunnelServerEndpoint = pBuffer->dest_endpoint;
        App_handleTunnelRequest( *pZCLEvent->pEventData, *((uint16_t*)(pZCLEvent->pEventData+1)),
                                 *(pZCLEvent->pEventData+3), pBuffer->a_asdu[1] );
        break;
    case g_ClientTransferDataError_Event_c :
        App_handleTunnelDataTransferError(  *((uint16_t*)pZCLEvent->pEventData), g_FALSE_c,
                                            *(pZCLEvent->pEventData+2), pBuffer );
        break;
    case g_ClientAckTransferData_Event_c :
    case g_ClientReadyData_Event_c :
        App_handleTunnelDataAckandReady( *((uint16_t*)pZCLEvent->pEventData),
                        *((uint16_t*)(pZCLEvent->pEventData + 2)), g_FALSE_c, pBuffer );

        break;
    case g_CloseTunnel_Event_c :
        App_handleCloseTunnelcommand(*((uint16_t*)pZCLEvent->pEventData), pBuffer );
        break;
#if ( g_SE_PROFILE_1_2_d == 1 )		
	case  g_GetSupportedTunnelProtocolsCommand_c:
		  App_CreateAndSendSupportedTunnelProtocols (*((uint16_t*)pZCLEvent->pEventData),pBuffer);
		break;
#endif	

#endif

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
    case g_RequestTunnelResponse_Event_c :
        App_handleTunnelResponseCommand( *((uint16_t*)(pZCLEvent->pEventData )),
                                         *(pZCLEvent->pEventData + 2), pBuffer );
        break;
    case g_ServerTransferDataError_Event_c :
        App_handleTunnelDataTransferError( *((uint16_t*)pZCLEvent->pEventData), g_TRUE_c,
                                            *(pZCLEvent->pEventData+2), pBuffer );
        break;
    case g_ServerAckTransferData_Event_c :
    case g_ServerReadyData_Event_c :
        App_handleTunnelDataAckandReady(*((uint16_t*)pZCLEvent->pEventData),
                        *((uint16_t*)(pZCLEvent->pEventData + 2)), g_TRUE_c, pBuffer );
        break;
#endif
    default :
        break;
    }
}

/****************************************************************************************/
static void App_handleTunnelDataAckandReady( uint16_t tunnelId,
            uint16_t numOfBytesLeft, bool fromServer,  APSDE_Data_Indication_t *pBuffer )
{
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
    TunnelingServerTunnel_t *stunnel;
#endif
#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
    TunnelingClientTunnel_t *ctunnel;
#endif
    uint16_t nwkSrcAddress = ZCL_Callback_Get_Device_ShortAddress();
    uint8_t status = g_ZCL_Failure_c;

    if ( fromServer == g_TRUE_c ) {
#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
         status = App_tunnelingClientFindTunnel( tunnelId,
                                            nwkSrcAddress,
                                            pBuffer->src_endpoint,
                                            &ctunnel );
         if (( status == g_ZCL_Success_c ) && (!ctunnel->flowControlSupport)) {
             status = g_ZCL_Failure_c;
         }
#endif
    }
    else {
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
        status = App_tunnelingServerFindTunnel( tunnelId,
                                                nwkSrcAddress,
                                                pBuffer->src_endpoint,
                                                &stunnel );
        if ( ( status == g_ZCL_Success_c ) && (!stunnel->flowControlSupport) ) {
            status = g_ZCL_Failure_c;
        }
#endif
    }
    if ( status != g_ZCL_Success_c ) {
        status = g_ZCL_Failure_c;
    }
    if ( Is_Default_Response_Required ( pBuffer->a_asdu ) ) {
        App_SendDefaultResponse( status, g_FALSE_c, pBuffer );
    }
}
/****************************************************************************************/
static void App_CreateAndSendTunnelDataAck( uint16_t tunnelId,
                    uint16_t numberOfBytesLeft, bool serverToClient, uint8_t seqNum )
{
    uint8_t asdu[7];
    uint16_t nwkSrcAddress = ZCL_Callback_Get_Device_ShortAddress();

    if ( serverToClient == g_TRUE_c ) {
        asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c |
                  g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ;
        asdu[2] = g_ServerAckTransferDataCommand_c;
    }
    else {
        asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c |
                  g_ZCL_CLIENT_TO_SERVER_DIRECTION_c;
        asdu[2] = g_ClientAckTransferDataCommand_c;
    }
    asdu[1] = seqNum;
    *((uint16_t*)&asdu[3]) = tunnelId;
    *((uint16_t*)&asdu[5]) = numberOfBytesLeft;

    App_SendData ( g_Short_Address_Mode_c,
             (uint8_t*)&nwkSrcAddress,
             ZCL_Callback_Get_Device_Src_EndPoint(),
             g_TUNNELING_CLUSTER_c,
             0x07,
             asdu );
}
/****************************************************************************************/
static void App_CreateAndSendTransferDataErrorCommand ( uint16_t tunnelId,
                                  uint8_t status, bool sendToClient, uint8_t seqNum )
{
    uint8_t asdu[6];
    uint16_t nwkSrcAddress = ZCL_Callback_Get_Device_ShortAddress();

    if ( sendToClient == g_TRUE_c ) {
        asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c |
                  g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ;
        asdu[2] = g_ServerTransferDataErrorCommand_c;
    }
    else {
        asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c |
                  g_ZCL_CLIENT_TO_SERVER_DIRECTION_c;
        asdu[2] = g_ClientTransferDataErrorCommand_c;
    }
    asdu[1] = seqNum;
    *((uint16_t*)&asdu[3]) = tunnelId;
    asdu[5] = status;

    App_SendData ( g_Short_Address_Mode_c,
                 (uint8_t*)&nwkSrcAddress,
                 ZCL_Callback_Get_Device_Src_EndPoint(),
                 g_TUNNELING_CLUSTER_c,
                 0x06,
                 asdu );
}

/****************************************************************************************/
static void App_handleTunnelDataTransferError( uint16_t tunnelId,
              uint8_t DataStatus, bool fromServer, APSDE_Data_Indication_t *pBuffer )
{
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
    TunnelingServerTunnel_t *stunnel;
#endif
#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
    TunnelingClientTunnel_t *ctunnel;
#endif
    uint8_t status = g_ZCL_Failure_c;
    uint16_t nwkSrcAddress = ZCL_Callback_Get_Device_ShortAddress();

    if ( fromServer == g_TRUE_c ) {
#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
        status = App_tunnelingClientFindTunnel( tunnelId,
                                            nwkSrcAddress,
                                            ZCL_Callback_Get_Device_Src_EndPoint(),
                                            &ctunnel );
        if ( status == g_ZCL_Success_c ) {
            ctunnel->inUse = g_FALSE_c;
        }
        else {
            status = g_ZCL_Failure_c;
        }
#endif
    }
    else {
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
         status = App_tunnelingServerFindTunnel( tunnelId, nwkSrcAddress,
                            ZCL_Callback_Get_Device_Src_EndPoint(), &stunnel );
        if ( status == g_ZCL_Success_c ) {
            stunnel->inUse = g_FALSE_c;
        }
        else {
            status = g_ZCL_Failure_c;
        }
#endif
    }
    if ( Is_Default_Response_Required ( pBuffer->a_asdu ) ) {
        App_SendDefaultResponse( status, g_FALSE_c, pBuffer );
    }
}

#endif  /* ( g_TUNNELING_CLUSTER_ENABLE_d == 1 ) */
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )

/****************************************************************************************/
static void App_handleTunnelRequest( uint8_t protocolId,
                     uint16_t manufacturerCode, bool flowControlSupport, uint8_t seqNum )
{
    uint16_t tunnelId = g_TUNNELING_CLUSTER_INVALID_TUNNEL_ID_c;
    uint8_t status = g_Tunnel_NoMoreTunnelIDs_c;
    uint8_t index;
    uint8_t asdu[6];
    uint16_t nwkAddress = ZCL_Callback_Get_Device_ShortAddress();
	


#if( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t Status =g_ZCL_Failure_c;
	uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();

	
	 Status = App_ReadWriteAttribute( EndPoint,
                            g_GENERIC_TUNNEL_CLUSTER_c,
                            g_MAXIMUM_OUTGOING_TRANSFER_SIZE_ATTRIBUTE_c,
                            (uint8_t*)&g_Tunnel_Max_Default_Value,
                            g_Read_Attribute_Data_c );
	 if(Status != g_ZCL_Success_c) {
		return;
	 }
#endif	

    if (!App_tunnelingServerIsProtocolSupportedCallBack( protocolId, manufacturerCode ) ) {
        status = g_Tunnel_ProtocolNotSupported_c;
    }
    else if ( flowControlSupport && !App_tunnelingServerIsFlowControlSupportedCallBack() ) {
        status = g_Tunnel_FlowControlNotSupported_c;
    }
    else {
        for ( index = 0; index < g_MAX_TUNNELING_SERVER_TUNNEL_LIMIT_c; index++ ) {
            if (! servertunnels[index].inUse ) {
                servertunnels[index].inUse = g_TRUE_c;
                servertunnels[index].client = nwkAddress;
                servertunnels[index].clientEndpoint = ZCL_Callback_Get_Device_Src_EndPoint();
                servertunnels[index].serverEndpoint = ZCL_Callback_Get_Device_EndPoint();
                servertunnels[index].tunnelId = tunnelId = index;
                servertunnels[index].protocolId = protocolId;
                servertunnels[index].manufacturerCode = manufacturerCode;
                servertunnels[index].flowControlSupport = flowControlSupport;
                servertunnels[index].lastActive = App_getcurrentTick();
//                servertunnels[index].numberOfBytesLeft = g_Tunnel_Max_Default_Value;
				memUtils_memCopy((uint8_t*)&servertunnels[index].numberOfBytesLeft ,
								 (uint8_t*)&g_Tunnel_Max_Default_Value,  2 );
                status = g_Tunnel_Success_c;
                break;
            }
        }
    }
    asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
              g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c ;
    asdu[1] = seqNum;
    asdu[2] = g_RequestTunnelResponseCommand_c;
    *((uint16_t*)&asdu[3]) = tunnelId;
    asdu[5] = status;

    App_SendData ( g_Short_Address_Mode_c,
                  (uint8_t*)&nwkAddress,
                   ZCL_Callback_Get_Device_Src_EndPoint(),
                   g_TUNNELING_CLUSTER_c,
                   0x06,
                   asdu );
}

/****************************************************************************************/
static void App_handleCloseTunnelcommand( uint16_t tunnelId,
                                          APSDE_Data_Indication_t *pBuffer )
{
    TunnelingServerTunnel_t *tunnel;
    uint8_t status;
	
	uint16_t short_address =
	   rsi_zigb_get_short_addr_for_specified_ieee_addr(pBuffer->src_address.IEEE_address);

    status = App_tunnelingServerFindTunnel( tunnelId,
                                            short_address,
                                            pBuffer->src_endpoint,
                                            &tunnel );
    if ( status == g_ZCL_Success_c ) {
        tunnel->inUse = g_FALSE_c;
    }
    if ( Is_Default_Response_Required ( pBuffer->a_asdu ) ) {
        App_SendDefaultResponse( status, g_FALSE_c, pBuffer );
    }
}

/****************************************************************************************/
uint8_t ZCL_callbackHandlerClientTunnelData( uint16_t tunnelId,
                uint8_t* tunneldataLength,uint8_t *tunneldata, uint8_t* zclHeader )
{
    uint8_t status;
    TunnelingServerTunnel_t *tunnel;
    uint8_t srcEndpoint = ZCL_Callback_Get_Device_Src_EndPoint();
    uint16_t nwkSrcAddress = ZCL_Callback_Get_Device_ShortAddress();

    status = App_tunnelingServerFindTunnel( tunnelId, nwkSrcAddress, srcEndpoint,
                                            &tunnel );
    if ( status == g_ZCL_Success_c ) {
        tunnel->lastActive = App_getcurrentTick();
        if ( tunnel->numberOfBytesLeft > *tunneldataLength ) {
            tunnel->numberOfBytesLeft = tunnel->numberOfBytesLeft - *tunneldataLength;
        }
        else {
            if ( ! tunnel->numberOfBytesLeft ) {
                status = g_ZCL_Insufficient_Space_c;
            }
            *tunneldataLength = tunnel->numberOfBytesLeft;
            tunnel->numberOfBytesLeft = g_ZERO_c;
            ptempIndBuffer->asdulength = *tunneldataLength + 0x05;
        }
        if ( tunnel->flowControlSupport == g_TRUE_c ) {
            App_CreateAndSendTunnelDataAck( tunnelId, tunnel->numberOfBytesLeft,
                                            g_TRUE_c, zclHeader[1] );
        }
    }
    else {
        status =( (status ==g_ZCL_Not_Authorized_c) ? g_WrongDevice_c : g_NoSuchTunnel_c);
        App_CreateAndSendTransferDataErrorCommand ( tunnelId, status, g_TRUE_c, zclHeader[1] );
        return status;
    }
    if ( Is_Default_Response_Required ( zclHeader ) ) {
        uint8_t asdu[5];
        uint8_t asduLength;
        uint16_t nwkSrcAddress = ZCL_Callback_Get_Device_ShortAddress();
        asduLength = ZCL_CreateDefaultResponseCommand ( zclHeader, asdu, status );

        App_SendData ( g_Short_Address_Mode_c,
             (uint8_t*)&nwkSrcAddress,
             ZCL_Callback_Get_Device_Src_EndPoint(),
             g_TUNNELING_CLUSTER_c,
             asduLength,
             asdu );
    }
    return status;
}

/****************************************************************************************/
uint8_t App_tunnelingServerFindTunnel( uint16_t tunnelId,
                                       uint16_t nwkAddress,
                                       uint8_t clientEndpoint,
                                       TunnelingServerTunnel_t** tunnel )

{
    uint8_t i;
    for ( i = 0; i < g_MAX_TUNNELING_SERVER_TUNNEL_LIMIT_c; i++ ) {
        if ( servertunnels[i].inUse && ( servertunnels[i].tunnelId == tunnelId ) ) {
            if( ( servertunnels[i].clientEndpoint == clientEndpoint ) &&
                ( nwkAddress == servertunnels[i].client ) ) {
                *tunnel = &servertunnels[i];
                return g_ZCL_Success_c;
            }
            else {
                return g_ZCL_Not_Authorized_c;
            }
        }
    }
    return g_ZCL_Not_Found_c;
}

/****************************************************************************************/
bool App_tunnelingServerIsProtocolSupportedCallBack( uint8_t protocolId,
                                             uint16_t manufacturerCode )
{
    return protocolId <= g_CLIMATETALK_c;
}

/****************************************************************************************/
bool App_tunnelingServerIsFlowControlSupportedCallBack( void )
{
    return g_TRUE_c;
}

/****************************************************************************************/
void App_ServerTunnelTask( void )
{
    uint8_t index;
    uint16_t tunnelCloseTimeOut = 0xFFFF;
    uint8_t status;

    status = App_ReadWriteAttribute( tunnelServerEndpoint, g_TUNNELING_CLUSTER_c,
                            g_CLOSE_TUNNEL_TIMEOUT_c, (uint8_t*)&tunnelCloseTimeOut,
                            g_Read_Attribute_Data_c );

    if ( status != g_ZCL_Success_c ) {
        return;
    }
    for ( index = 0; index < g_MAX_TUNNELING_SERVER_TUNNEL_LIMIT_c; index++ ) {
        if ( servertunnels[index].inUse ) {
            if (( App_getcurrentTick() - servertunnels[index].lastActive) > tunnelCloseTimeOut) {
                 servertunnels[index].inUse = g_FALSE_c;
#if(g_SE_PROFILE_1_2_d == 1)
	/*send Tunnel closure notification command after expiration of closetTunnelTimeout
	period*/
		App_sendCloseTimeoutNotificationCommand(servertunnels[index].client,
												servertunnels[index].tunnelId);
#endif     /* g_SE_PROFILE_1_2_d*/				
            }
        }
    }

}
/****************************************************************************************/
#if(g_SE_PROFILE_1_2_d == 1)
	/*send Tunnel closure notification command after expiration of closetTunnelTimeout
	period*/
void App_sendCloseTimeoutNotificationCommand(uint16_t nwkSrcAddress, uint16_t tunnelId)
{
	uint8_t asduLength = 0x02;
	uint8_t asdu[4];
	/* ZCL Header Creation */
	asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c;
	asdu[1] = App_GetTransSeqNumber();
	asdu[2] = g_TunnelClosureNotification_c;
	*((uint16_t*)&asdu[3]) = tunnelId;
	
	App_SendData ( g_Short_Address_Mode_c,
					(uint8_t*)&nwkSrcAddress,
					ZCL_Callback_Get_Device_Src_EndPoint(),
					g_TUNNELING_CLUSTER_c,
					asduLength,
					asdu );	
}

/****************************************************************************************/
static void App_CreateAndSendSupportedTunnelProtocols(uint8_t offset,
													  APSDE_Data_Indication_t *pBuffer )
{
    uint8_t protocolCompleteList = g_TRUE_c, i,m=5, asdulength = 0x00;
	uint8_t protocolCount = g_MAX_TUNNELING_SUPPORTED_PROTOCOLS_c, asdu[20];
    uint16_t nwkAddress = ZCL_Callback_Get_Device_ShortAddress();     	
	if(offset == g_NULL_c)
	{
		/* Initially offest will be null*/
		
		asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
				  g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
		asdu[1] = App_GetTransSeqNumber();
		asdu[2] = g_SupportedTunnelProtocolsResponse_c;
		asdu[3] = protocolCompleteList;
		asdu[4] = protocolCount;
		for (i= 0; i< g_MAX_TUNNELING_SUPPORTED_PROTOCOLS_c; i++)
		{
				*((uint16_t*)&asdu[m]) = 0xFFFF;
				asdu[m+2] = protocolList [i];
				m=m+3;
		}
		asdulength = 5 + g_MAX_TUNNELING_SUPPORTED_PROTOCOLS_c*3;
		App_SendData ( g_Short_Address_Mode_c,
						(uint8_t*)&nwkAddress,
						ZCL_Callback_Get_Device_Src_EndPoint(),
						g_TUNNELING_CLUSTER_c,
						asdulength,
						asdu );
	}
}

#endif     /* g_SE_PROFILE_1_2_d*/	

#endif  /* ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
/****************************************************************************************/
static void App_handleTunnelResponseCommand( uint16_t tunnelId,
                              uint8_t tunnelStatus, APSDE_Data_Indication_t *pBuffer )
{
    uint8_t index;
    uint8_t status = g_ZCL_Insufficient_Space_c;
    uint16_t nwkAddress = ZCL_Callback_Get_Device_ShortAddress();

#if( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
	 uint8_t Status =g_ZCL_Failure_c;
	 uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
	 Status = App_ReadWriteAttribute( EndPoint,
                            g_GENERIC_TUNNEL_CLUSTER_c,
                            g_MAXIMUM_OUTGOING_TRANSFER_SIZE_ATTRIBUTE_c,
                            (uint8_t*)&g_Tunnel_Max_Default_Value,
                            g_Read_Attribute_Data_c );
	 if(Status != g_ZCL_Success_c) {
		return;
	 }
#endif	
    if ( tunnelStatus == g_Tunnel_Success_c ) {
        for ( index = 0; index < g_MAX_TUNNELING_CLIENT_TUNNEL_LIMIT_c; index++ ) {
            if ( !clienttunnels[index].inUse ) {
                clienttunnels[index].inUse = g_TRUE_c;
                clienttunnels[index].server = nwkAddress;
                clienttunnels[index].clientEndpoint = pBuffer->dest_endpoint;
                clienttunnels[index].serverEndpoint = pBuffer->src_endpoint;
                clienttunnels[index].tunnelId = tunnelId;
                clienttunnels[index].protocolId = protocolId;
                clienttunnels[index].manufacturerCode = manufacturerCode;
                clienttunnels[index].flowControlSupport = flowControl;
//                clienttunnels[index].numberOfBytesLeft = g_Tunnel_Max_Default_Value;
				memUtils_memCopy((uint8_t*)&clienttunnels[index].numberOfBytesLeft ,
								 (uint8_t*)&g_Tunnel_Max_Default_Value,  2 );
                status = g_ZCL_Success_c;
                break;
            }
        }
    }
    else {
        status = g_ZCL_Success_c;
    }
    if ( Is_Default_Response_Required ( pBuffer->a_asdu ) ) {
        App_SendDefaultResponse( status, g_FALSE_c, pBuffer );
    }
}

/****************************************************************************************/
uint8_t ZCL_callbackHandlerServerTunnelData( uint16_t tunnelId,
                    uint8_t *tunneldataLength, uint8_t *tunneldata, uint8_t* zclHeader )
{
    uint8_t status;
    TunnelingClientTunnel_t *tunnel;
    uint8_t srcEndpoint = ZCL_Callback_Get_Device_Src_EndPoint();
    uint16_t nwkSrcAddress = ZCL_Callback_Get_Device_ShortAddress();

    status = App_tunnelingClientFindTunnel( tunnelId, nwkSrcAddress, srcEndpoint,
                                            &tunnel );
    if ( status == g_ZCL_Success_c ) {
        if ( tunnel->numberOfBytesLeft > *tunneldataLength ) {
            tunnel->numberOfBytesLeft = tunnel->numberOfBytesLeft - *tunneldataLength;
        }
        else {
            if ( ! tunnel->numberOfBytesLeft ) {
                status = g_ZCL_Insufficient_Space_c;
            }
            *tunneldataLength = tunnel->numberOfBytesLeft;
            tunnel->numberOfBytesLeft = g_ZERO_c;
            ptempIndBuffer->asdulength = *tunneldataLength + 0x05;
        }
        if ( tunnel->flowControlSupport == g_TRUE_c ) {
            App_CreateAndSendTunnelDataAck( tunnelId, tunnel->numberOfBytesLeft,
                                            g_FALSE_c, zclHeader[1] );
            return status;
        }
    }
    else {
        status = status == g_ZCL_Not_Authorized_c ? g_WrongDevice_c : g_NoSuchTunnel_c;
        App_CreateAndSendTransferDataErrorCommand ( tunnelId, status, g_FALSE_c, zclHeader[1] );
        return status;
    }
    if ( Is_Default_Response_Required ( zclHeader ) ) {
        uint8_t asdu[5];
        uint8_t asduLength;
        uint16_t nwkSrcAddress = ZCL_Callback_Get_Device_ShortAddress();
        asduLength = ZCL_CreateDefaultResponseCommand ( zclHeader, asdu, status );

        App_SendData ( g_Short_Address_Mode_c,
             (uint8_t*)&nwkSrcAddress,
             ZCL_Callback_Get_Device_Src_EndPoint(),
             g_TUNNELING_CLUSTER_c,
             asduLength,
             asdu );
    }
    return status;
}

/****************************************************************************************/
uint8_t App_tunnelingClientFindTunnel( uint16_t tunnelId,
                                       uint16_t nwkAddress,
                                       uint8_t serverEndpoint,
                                       TunnelingClientTunnel_t** tunnel )

{
    uint8_t i;
    for ( i = 0; i < g_MAX_TUNNELING_CLIENT_TUNNEL_LIMIT_c; i++ ) {
        if ( clienttunnels[i].inUse && ( clienttunnels[i].tunnelId == tunnelId ) ) {
            if( ( clienttunnels[i].serverEndpoint == serverEndpoint ) &&
                ( nwkAddress == clienttunnels[i].server ) ) {
                *tunnel = &clienttunnels[i];
                return g_ZCL_Success_c;
            }
            else {
                return g_ZCL_Not_Authorized_c;
            }
        }
    }
    return g_ZCL_Not_Found_c;
}

#endif      /* ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/***************************************************************************************
* Private Functions
****************************************************************************************/

/* None */

/***************************************************************************************
* Interrupt Service Routines
****************************************************************************************/

/* None */

/***************************************************************************************
* End Of File
****************************************************************************************/

