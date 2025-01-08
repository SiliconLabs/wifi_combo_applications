/*******************************************************************************
* ZCL_Tunneling_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Tunneling cluster
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "memory_utility.h"
#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Tunneling_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Tunneling_Server_Attribute [] =
{
    {   g_CLOSE_TUNNEL_TIMEOUT_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c   }
};

 /* Cluster Info for Tunneling Cluster */
const ZCL_Cluster_Info_t ga_Tunneling_Server_Cluster_Info =
{
    g_TUNNELING_CLUSTER_c,
    sizeof(ma_Tunneling_Server_Attribute)/sizeof(ZCL_Attribute_Info_t),
    ma_Tunneling_Server_Attribute
};
#endif				/* g_TUNNELING_SERVER_CLUSTER_ENABLE_d */

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
 /* Cluster Info for Tunneling Cluster */
const ZCL_Cluster_Info_t ga_Tunneling_Client_Cluster_Info =
{
	g_TUNNELING_CLUSTER_c,
	g_NULL_c,
	g_NULL_c
};
#endif				/* g_TUNNELING_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_HandleTunnelingClusterCmd ( uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
    uint8_t responseLength = g_ZERO_c;
    pActionHandler->event.eventLength = asduLength - 0x03;
    pActionHandler->action = No_Response;
    uint8_t status = g_ZCL_Success_c;
    uint8_t tunnelDataLength;
#if(g_SE_PROFILE_1_2_d == 1)
     uint8_t i, offset = m_PAYLOAD_LOCATION_c;
#endif /*g_SE_PROFILE_1_2_d */	
	pActionHandler->event.eventLength = asduLength - m_PAYLOAD_LOCATION_c;
    if( ZCL_GetDirectionOfCommand(pReceivedAsdu) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
        switch( pReceivedAsdu[ g_ZCL_HEADER_COMMAND_OFFSET_c ] )
        {
            case g_RequestTunnelResponseCommand_c:
                pActionHandler->event.eventId = g_RequestTunnelResponse_Event_c;
                pActionHandler->action = Received_Response;
                break;

            case g_ServerTransferDataCommand_c:
                tunnelDataLength = pActionHandler->event.eventLength - 0x02;
                status = ZCL_callbackHandlerServerTunnelData(*((uint16_t*)&pReceivedAsdu[3]),
                         &tunnelDataLength, &pReceivedAsdu[5], pReceivedAsdu );
                pActionHandler->event.eventLength = tunnelDataLength + 0x02;
                if ( status == g_ZCL_Success_c ) {
                    pActionHandler->event.eventId = g_ServerTransferData_Event_c;
                    pActionHandler->action = No_Response;
                }
                break;

            case g_ServerTransferDataErrorCommand_c:
                pActionHandler->event.eventId = g_ServerTransferDataError_Event_c;
                pActionHandler->action = Received_Response;
                break;

            case g_ServerAckTransferDataCommand_c:
                pActionHandler->event.eventId = g_ServerAckTransferData_Event_c;
                pActionHandler->action = Received_Response;
                break;

            case g_ServerReadyDataCommand_c:
                pActionHandler->event.eventId = g_ServerReadyData_Event_c;
                pActionHandler->action = No_Response;
                break;
/***********************************************	
 Updates of SE 1.1 B
 **********************************************/
#if(g_SE_PROFILE_1_2_d == 1)
			case g_SupportedTunnelProtocolsResponse_c:
			  	pActionHandler->event.eventId = g_SupportedTunnelProtocolsResponse_Event_c;
                pActionHandler->action = Received_Response;
				 SuppTunnelProtocolsRespCommand_t *pFormatevent;
				  pFormatevent = (SuppTunnelProtocolsRespCommand_t*)pActionHandler->event.pEventData;
				
				pFormatevent->ProtocolCompleteList = *( pReceivedAsdu + offset );
				offset += 0x01;
				pFormatevent->ProtocolCount = *( pReceivedAsdu + offset );
				offset += 0x01;
				for (i=0; i< pFormatevent->ProtocolCount ; i++ ){
					memUtils_memCopy( (uint8_t*)&pFormatevent->ProtocolList[i].ManufactureCode,
												pReceivedAsdu + offset, 0x02 );
					offset += 0x02;
					pFormatevent->ProtocolList[i].ProtocolId = *( pReceivedAsdu + offset );
					offset += 0x01;
				} 				
 			   return g_ZERO_DATA_LENGTH_c;

  			case g_TunnelClosureNotification_c:
                pActionHandler->event.eventId = g_TunnelClosureNotification_Event_c;
                pActionHandler->action = Received_Response;
                break;    				
#endif 									/*g_SE_PROFILE_1_2_d  */						

            default:
                pActionHandler->action = Send_Response;
                return responseLength;
         }
         memUtils_memCopy( pActionHandler->event.pEventData,
                  &pReceivedAsdu[0x03],pActionHandler->event.eventLength );
#endif  /* g_TUNNELING_CLIENT_CLUSTER_ENABLE_d */
    }
    else {

#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
        switch( pReceivedAsdu[ g_ZCL_HEADER_COMMAND_OFFSET_c ] )
        {
            case g_RequestTunnelCommand_c:
                pActionHandler->event.eventId = g_RequestTunnel_Event_c;
                pActionHandler->action = No_Response;
                break;

            case g_ClientTransferDataCommand_c:
                tunnelDataLength = pActionHandler->event.eventLength - 0x02;
                status = ZCL_callbackHandlerClientTunnelData(*((uint16_t*)&pReceivedAsdu[3]),
                         &tunnelDataLength, &pReceivedAsdu[5], pReceivedAsdu );
                pActionHandler->event.eventLength = tunnelDataLength + 0x02;
               if ( status == g_ZCL_Success_c ) {
                    pActionHandler->action = No_Response;
                    pActionHandler->event.eventId = g_ClientTransferData_Event_c;
                }
                break;

            case g_CloseTunnelCommand_c:
                pActionHandler->event.eventId = g_CloseTunnel_Event_c;
                pActionHandler->action = No_Response;
                break;
            case g_ClientTransferDataErrorCommand_c:
                pActionHandler->event.eventId = g_ClientTransferDataError_Event_c;
                pActionHandler->action = Received_Response;
                break;
            case g_ClientAckTransferDataCommand_c:
                pActionHandler->event.eventId = g_ClientAckTransferData_Event_c;
                pActionHandler->action = Received_Response;
                break;
            case g_ClientReadyDataCommand_c:
                pActionHandler->event.eventId = g_ClientReadyData_Event_c;
                pActionHandler->action = No_Response;
                break;
/***********************************************	
 Updates of SE 1.1 B
 **********************************************/
#if(g_SE_PROFILE_1_2_d == 1)
			case g_GetSupportedTunnelProtocolsCommand_c:
                pActionHandler->event.eventId = g_GetSupportedTunnelProtocolsCommand_Event_c;
                pActionHandler->action = Received_Response;
                break;   				
#endif 									/*g_SE_PROFILE_1_2_d  */				
            default:
                pActionHandler->action = Send_Response;
                return responseLength;
        }
        memUtils_memCopy( pActionHandler->event.pEventData,
                  &pReceivedAsdu[0x03], pActionHandler->event.eventLength );
		
#endif  /* g_TUNNELING_SERVER_CLUSTER_ENABLE_d */
    }
    return responseLength;
}

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                           /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */


