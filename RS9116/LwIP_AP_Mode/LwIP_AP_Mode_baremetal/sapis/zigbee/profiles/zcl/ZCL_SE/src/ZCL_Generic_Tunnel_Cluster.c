/*******************************************************************************
* ZCL_Generic_Tunnel_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Generic Tunnel cluster
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "memory_utility.h"
#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Generic_Tunnel_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Generic_Tunnel_Server_Attribute [] =
{
    {
	  	g_MAXIMUM_INCOMING_TRANSFER_SIZE_ATTRIBUTE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
	} ,		
	{
	  	g_MAXIMUM_OUTGOING_TRANSFER_SIZE_ATTRIBUTE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
	} , 		
	{
		g_PROTOCOL_ADDRESS_ATTRIBUTE_c,
        g_Octet_String_Data_c,
        g_Read_Write_Attribute_c
	}
		
};

 /* Cluster Info for Generic Tunnel Cluster */
const ZCL_Cluster_Info_t ga_Generic_Tunnel_Server_Cluster_Info =
{
    g_GENERIC_TUNNEL_CLUSTER_c,
    sizeof(ma_Generic_Tunnel_Server_Attribute)/sizeof(ZCL_Attribute_Info_t),
    ma_Generic_Tunnel_Server_Attribute
};
#endif				/* g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d */

#if ( g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d == 1 )
 /* Cluster Info for Generic TunnelCluster */
const ZCL_Cluster_Info_t ga_Generic_Tunnel_Client_Cluster_Info =
{
	g_GENERIC_TUNNEL_CLUSTER_c,
	g_NULL_c,
	g_NULL_c
};
#endif				/* g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Generic_Tunnel_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
	uint8_t responseAsduLength = g_ZERO_c;
  	pActionHandler->action = No_Response;
	pActionHandler->event.eventLength = asduLength - 0x03;

    if( ( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) ) {

#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
	  if( g_MatchProtocolAddressCmd_c == pReceivedAsdu[ g_ZCL_HEADER_COMMAND_OFFSET_c ] ){
			pActionHandler->event.eventId = g_MatchProtocolAddressEvent_c;
		}
#endif	
	}	
	else {
#if ( g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d == 1 )
	   if( g_MatchProtocolAddressResponseCmd_c == pReceivedAsdu[g_ZCL_HEADER_COMMAND_OFFSET_c] ){
		pActionHandler->event.eventId = g_MatchProtocolAddressResponseEvent_c;
		pActionHandler->action = Received_Response;
	   }
        else if( g_AdvertiseProtocolAddressCmd_c == pReceivedAsdu[g_ZCL_HEADER_COMMAND_OFFSET_c] ){
            /* Update the event with Advertise Protocol event */
            pActionHandler->event.eventId = g_AdvertiseProtocolAddressEvent_c;
        }
        else {
            /* since it is an error scenario, default response command should be
               returned with status as unsupported cluster command */
            pActionHandler->action = Send_Response;
        }
#endif      /* ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )   */
    }
   	 memUtils_memCopy ( pActionHandler->event.pEventData,
               &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
               asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c ) ;
	if ( Is_Default_Response_Required ( pReceivedAsdu ) ) {
	responseAsduLength =	ZCL_CreateDefaultResponseCommand (
							  pReceivedAsdu,
							  pResponseLocation,
							  g_ZCL_Success_c );
	 pActionHandler->action = Send_Response;
	
	}

    return responseAsduLength;
}

#endif
