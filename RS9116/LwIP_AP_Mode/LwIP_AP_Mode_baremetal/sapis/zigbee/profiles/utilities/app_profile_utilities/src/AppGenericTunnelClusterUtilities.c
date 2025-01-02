/*******************************************************************************
* AppGenericTunnelClusterUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Generic Tunnel cluster
*
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ApplicationThinLayer.h"
#include "memory_utility.h"
#include "buffer_management.h"
#include "apsde_interface.h"
#include "stack_util.h"
#if( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
#include "AppGenericTunnelClusterUtilities.h"
#include "ZCL_Generic_Tunnel_Cluster.h"
#include "ApplicationThinLayer.h"

/*******************************************************************************
* Global Constants
********************************************************************************/
#define g_GENERIC_TUNNEL_PROTOCOL_ADDRESS_SIZE_d 8
/******************************************************************************
* Public Memory declarations
*******************************************************************************/
#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif
/******************************************************************************
* Private Memory declarations
*******************************************************************************/
/* The device protocol address . The first byte is the length and this address
is stored  by all endpoints*/
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
static uint8_t aGenericProtocolAddress[g_GENERIC_TUNNEL_PROTOCOL_ADDRESS_SIZE_d + 1];
#endif

/*****************************************************************************
* Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Public Functions
********************************************************************************/
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t App_GenericTunnelInit( void )
{

  	 uint8_t Status =g_ZCL_Failure_c;
	 uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
	 uint8_t MaxSize[2];

	 memUtils_memCopy( (uint8_t*)&MaxSize , (uint8_t*)g_MAX_PAYLOAD_c, 0x02);
	 Status = App_ReadWriteAttribute( EndPoint,
                            g_GENERIC_TUNNEL_CLUSTER_c,
                            g_MAXIMUM_INCOMING_TRANSFER_SIZE_ATTRIBUTE_c,
                            (uint8_t*)&MaxSize,
                            g_Write_Attribute_Data_c );
	 if(Status != g_ZCL_Success_c) {
		return Status;
	 }
	  Status = App_ReadWriteAttribute( EndPoint,
                            g_GENERIC_TUNNEL_CLUSTER_c,
                            g_MAXIMUM_OUTGOING_TRANSFER_SIZE_ATTRIBUTE_c,
                            (uint8_t*)&MaxSize,
                            g_Write_Attribute_Data_c );
	 if(Status != g_ZCL_Success_c) {
		return Status;
	 }
	 /* Set up the default address. */
	aGenericProtocolAddress[0] = 3;
	aGenericProtocolAddress[1] = '0';
	aGenericProtocolAddress[2] = '0';
	aGenericProtocolAddress[3] = '1';
	Status = App_ReadWriteAttribute( EndPoint,
                            g_GENERIC_TUNNEL_CLUSTER_c,
                            g_PROTOCOL_ADDRESS_ATTRIBUTE_c,
                            aGenericProtocolAddress,
                            g_Write_Attribute_Data_c );
	return Status;
}
#endif
/******************************************************************************/

void App_GenericTunnelEventsHandler( ZCL_Event_t *pZCLEvent,
                             APSDE_Data_Indication_t *pBuffer )
{

#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
    if(g_MatchProtocolAddressEvent_c == pZCLEvent->eventId ){

			App_GenericTunnelMatchProtocolAddressHandler(
					((MatchProtocolAddress_t*)pZCLEvent->pEventData)->ProtocolAddress,pBuffer);
    }

#endif

#if ( g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d == 1 )

        if( g_MatchProtocolAddressResponseEvent_c == pZCLEvent->eventId ||
           g_AdvertiseProtocolAddressEvent_c == pZCLEvent->eventId ){
            /* Application should handle */
           }

#endif

}
/******************************************************************************/
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )

void App_GenericTunnelMatchProtocolAddressHandler( uint8_t *ProtocolAddress,
												  APSDE_Data_Indication_t *pBuffer)
{
	uint8_t i ,asduLength;
	MatchProtocolAddressResponse_t *matchProtocolAddressRespCmdPayload;
	matchProtocolAddressRespCmdPayload
	  				= (MatchProtocolAddressResponse_t*)&pBuffer->a_asdu[3];
	/* Starting with the length byte, make sure the incoming address matches our
	 address.  If not, send a default response.  If everything matches, send a
	 Match Protocol Address response. */
	for (i = 1; i <= aGenericProtocolAddress[0]; i++) {
		if (ProtocolAddress[i] != aGenericProtocolAddress[i]) {
				App_SendDefaultResponse( g_ZCL_Not_Found_c, g_FALSE_c, pBuffer );
			return;
		}
	}
	/* create an payload for  match protocol address respone*/

	pBuffer->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c|
	  						g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
	pBuffer->a_asdu[1] = App_GetTransSeqNumber();
	pBuffer->a_asdu[2] = g_MatchProtocolAddressResponseCmd_c;

	/*IEEE address of the responding device */
    memUtils_memCopy( (uint8_t*)&matchProtocolAddressRespCmdPayload->DeviceIEEEAddress,
               (uint8_t*)ZCL_Callback_Get_Device_IEEEAddress(), g_EXTENDED_ADDRESS_LENGTH_c );

	 /*matched Protocol Address. */
	memUtils_memCopy( (uint8_t*)&matchProtocolAddressRespCmdPayload->ProtocolAddress,
								   (uint8_t*)&aGenericProtocolAddress,
								   g_GENERIC_TUNNEL_PROTOCOL_ADDRESS_SIZE_d+1);

	asduLength = 0x0b+ g_GENERIC_TUNNEL_PROTOCOL_ADDRESS_SIZE_d+1;
	App_SendData( pBuffer->src_addr_mode,
					pBuffer->src_address.IEEE_address,
					pBuffer->src_endpoint,
					g_GENERIC_TUNNEL_CLUSTER_c,
					asduLength,
					pBuffer->a_asdu );
  return ;
}

/******************************************************************************/

void ZCL_Callback_ProtocolAddressChanged (  uint8_t endpoint,
                                         uint16_t attributeId,
                                         const ZCL_Cluster_Info_t *pZCLClusterInfo)
{
    uint8_t mGenericProtocolAddress[g_GENERIC_TUNNEL_PROTOCOL_ADDRESS_SIZE_d + 1];
	if (pZCLClusterInfo->cluster_id == g_GENERIC_TUNNEL_CLUSTER_c &&
										attributeId == g_PROTOCOL_ADDRESS_ATTRIBUTE_c) {

		App_ReadWriteAttribute(  endpoint,
							   pZCLClusterInfo->cluster_id,
							   attributeId,
							   (uint8_t*)&mGenericProtocolAddress,
							   g_Read_Attribute_Data_c );



		if(  g_Memory_Matches_c != ( memUtils_memCompare( mGenericProtocolAddress,
								/* App_GenericTunnelGetProtocolAddress(endpoint)*/
                                aGenericProtocolAddress,
								 0x08 ) ) ) {
		      memUtils_memCopy( (uint8_t*)&aGenericProtocolAddress ,
                                (uint8_t*)&mGenericProtocolAddress, 0x08);
			App_SendChangedProtocolAddress (endpoint,pZCLClusterInfo->cluster_id);
		}
	}

}


/******************************************************************************
* Private Functions
******************************************************************************/

/*******************************************************************************************/
void App_SendChangedProtocolAddress(uint8_t endpoint, uint16_t clusterId )
{

    APSDE_Data_Request_t *p_apsde_data_request;
    uint8_t buffindex;

    buffindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

   if( g_NO_AVAILABLE_BUFFER_c != buffindex ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 1 */
        allocatedbuffer[buffindex]= 203;
        freebuffer[buffindex][0]= 203;
        freebuffer[buffindex][1]= 0;
#endif
        p_apsde_data_request = ( APSDE_Data_Request_t * )buffMgmt_getBufferPointer( buffindex );
        *((uint8_t*)p_apsde_data_request) = g_APSDE_Data_Request_c;
        p_apsde_data_request = (APSDE_Data_Request_t *)&(((APSDE_Request_t *)p_apsde_data_request)->
                                                       APSDE_Request_Primitive.apsde_data_request);
        memUtils_memSet( (uint8_t*)&p_apsde_data_request->dest_address.short_address,
                           0xff, g_SHORT_ADDRESS_LENGTH_c );
        p_apsde_data_request->dest_addr_mode = g_Short_Address_Mode_c;
		p_apsde_data_request->dest_endpoint   = 0x01;
		p_apsde_data_request->src_endpoint   = endpoint;
        p_apsde_data_request->profile_id     = g_APP_PROFILE_ID_c;
        p_apsde_data_request->cluster_id     = clusterId;
        p_apsde_data_request->asdulength     = 0x03+ g_GENERIC_TUNNEL_PROTOCOL_ADDRESS_SIZE_d+1;
        p_apsde_data_request->asdu[0]        = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
                                               g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                               g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        p_apsde_data_request->asdu[1]        = App_GetTransSeqNumber();
        p_apsde_data_request->asdu[2]        = g_AdvertiseProtocolAddressCmd_c;
        memUtils_memCopy( &p_apsde_data_request->asdu[3], (uint8_t*)&aGenericProtocolAddress,
				 					g_GENERIC_TUNNEL_PROTOCOL_ADDRESS_SIZE_d+1 );

        p_apsde_data_request->txoptions =
            m_GET_g_NWK_SecurityLevel_c ? g_APS_Tx_Opt_Use_NWK_Key_c:g_APS_Tx_Opt_Unsecured_c;
        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;

        //APS_APSDErequest( buffindex );
        APS_APSDE_request(buffindex, p_apsde_data_request );
   }
}
#endif
/*******************************************************************************
* Interrupt Service Routines
********************************************************************************/

/* None */
#endif
/*******************************************************************************
* End Of File
********************************************************************************/

