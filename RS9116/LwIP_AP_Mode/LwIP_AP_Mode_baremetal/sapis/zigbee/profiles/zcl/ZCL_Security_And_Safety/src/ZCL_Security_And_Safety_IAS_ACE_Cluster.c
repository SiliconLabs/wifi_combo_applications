/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "common_includes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "stack_util.h"
#include "memory_utility.h"

#if ( g_IAS_ACE_CLUSTER_ENABLE_d == 1 )

#include "ZCL_Security_And_Safety_IAS_ACE_Cluster.h"
#include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_ACE_ARM_CMD_PAYLOAD_LENGTH_c 0x01
#define m_ACE_GET_ZONEID_MAP_CMD_PAYLOAD_LENGTH_c 0x00
#define m_ACE_GET_ZONE_INFORMATION_CMD_PAYLOAD_LENGTH_c 0x01
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
/*Cluster Info for IAS Zone Cluster*/
const ZCL_Cluster_Info_t ga_IAS_ACE_Server_Cluster_Info =
{
   g_IAS_ACE_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};

#if ( g_IAS_ACE_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_IAS_ACE_Client_Cluster_Info =
{
   g_IAS_ACE_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_IAS_ACE_Cluster_Cmd (  	
								uint8_t endPointId,
								uint8_t asduLength,
								uint8_t* pReceivedAsdu,
								uint8_t* pResponseAsduStartLocation,
								ZCL_ActionHandler_t *pActionHandler,
								ZCL_Cluster_Info_t const *pZCLClusterInfo,
								uint8_t * pAttrsDataBaseLocation ){

	uint8_t responseLength = g_NULL_c;
#if ( g_IAS_ACE_SERVER_CLUSTER_ENABLE_d == 1 )
	uint8_t zoneLength = g_NULL_c;
#endif	
	
	if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) {
#if ( g_IAS_ACE_SERVER_CLUSTER_ENABLE_d == 1 )
	pActionHandler->event.eventLength = 0x00;
	switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
	{
		case g_Arm_Command_c:
			/* Update the Event Details. This event is handled by the
			application.*/
			/*The command has event and response*/
			pActionHandler->action = Send_Response;
			/*Arm command received event*/
			pActionHandler->event.eventId = g_Arm_Command_Event_c;
			pActionHandler->event.eventLength = m_ACE_ARM_CMD_PAYLOAD_LENGTH_c;
			/*arm mode*/
			/*copy the event data, for this command only arm mode*/
			memUtils_memCopy( pActionHandler->event.pEventData,
							  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							  m_ACE_ARM_CMD_PAYLOAD_LENGTH_c );
			
			/* Create the ZCL Header for Enroll  response command*/
			/* Update the Frame Control Field in the scenes response
			Frame which needs to be transmitted*/
			*( pResponseAsduStartLocation + responseLength ) =
							g_Cluster_Specific_Cmd_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
												g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
			
			responseLength++;
			/* Update the TSN field in the ZCL Header. TSN field should be same as in
			   the received frame */
			*( pResponseAsduStartLocation + responseLength ) =
											*( pReceivedAsdu + responseLength ); 		
			responseLength++;	
			*( pResponseAsduStartLocation + responseLength ) =
														g_Arm_Response_Command_c;
			responseLength++;
			      /*Updating the commandId field in the ZCL Header*/
			
			*(pResponseAsduStartLocation + responseLength)= *( pReceivedAsdu + responseLength ); 			
			responseLength ++;
			return responseLength;	
		case g_Bypass_Command_c:
			/*update the event for by pass command*/
			pActionHandler->event.eventId = g_ByPass_Command_Event_c;
			pActionHandler->action = Send_Response;
			if(pReceivedAsdu[3] == 0)
			{
				memUtils_memCopy( pActionHandler->event.pEventData,
							  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							  0x01 );	
			}
			else
			{
				pActionHandler->event.eventLength =0x01 + *(pReceivedAsdu+3);
				memUtils_memCopy( pActionHandler->event.pEventData,
							  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							  pActionHandler->event.eventLength );	
			}
			
			if ( Is_Default_Response_Required(pReceivedAsdu)) {
					responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, g_ZCL_Success_c );
					
				}
			return responseLength;		
		case g_Emergency_Command_c:
			/*update the event to emergency command event*/		
			pActionHandler->event.eventId = g_Emergency_Command_Event_c;
			pActionHandler->action = Received_Response;
			if ( Is_Default_Response_Required(pReceivedAsdu)) {
					responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, g_ZCL_Success_c );
					pActionHandler->action = Send_Response;
				}
			break;
		case g_Fire_Command_c:
			/*update the event to fire command event*/
			pActionHandler->event.eventId = g_Fire_Command_Event_c;
			pActionHandler->action = Received_Response;
			if ( Is_Default_Response_Required(pReceivedAsdu)) {
					responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, g_ZCL_Success_c );
					pActionHandler->action = Send_Response;
				}
			break;		
		case g_Panic_Command_c:
			/*update the event to panic command event*/
			pActionHandler->event.eventId = g_Panic_Command_Event_c;
			pActionHandler->action = Received_Response;
			if ( Is_Default_Response_Required(pReceivedAsdu)) {
					responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, g_ZCL_Success_c );
					pActionHandler->action = Send_Response;
				}
			break;		
		case g_Get_Zone_Id_Map_Command_c:
			/*event and response is sent*/
			pActionHandler->action = Send_Response;
			/*Arm command received event*/
			pActionHandler->event.eventId = g_Get_Zone_Id_Map_Command_Event_c;
			pActionHandler->event.eventLength = m_ACE_GET_ZONEID_MAP_CMD_PAYLOAD_LENGTH_c;
			/*arm mode*/
			/*copy the event data, for this command only arm mode*/
			memUtils_memCopy( pActionHandler->event.pEventData,
							  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							  m_ACE_GET_ZONEID_MAP_CMD_PAYLOAD_LENGTH_c );
			
			/* Create the ZCL Header for Enroll  response command*/
			/* Update the Frame Control Field in the scenes response
			Frame which needs to be transmitted*/
			*( pResponseAsduStartLocation + responseLength ) =
					g_Cluster_Specific_Cmd_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
										g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c; 		
			
			responseLength++;
			
			/* Update the TSN field in the ZCL Header. TSN field should be same as in
			   the received frame */
			*( pResponseAsduStartLocation + responseLength ) =
								*( pReceivedAsdu + responseLength );
			
			responseLength++;
			
			/*Updating the commandId field in the ZCL Header*/
			*( pResponseAsduStartLocation + responseLength ) =
												g_Get_Zone_Id_Map_Response_Command_c;
			responseLength++;
			
			
			 /*should consider all 256 bits hence forth, it is 32 bytes and
			 the command id gives 33 bytes - 0x21*/
			 asduLength =  0x21;
			 ZCL_Callback_Get_ZoneId_Map_Information(pReceivedAsdu,pResponseAsduStartLocation+responseLength,asduLength );			
			 responseLength = responseLength +0x20;
			 return responseLength;	
		
		case g_Get_Zone_Information_Command_c:
		
			pActionHandler->action = Send_Response;
	        pActionHandler->event.eventLength = m_ACE_GET_ZONE_INFORMATION_CMD_PAYLOAD_LENGTH_c;
			/*Arm command received event*/
			pActionHandler->event.eventId = g_Get_Zone_Information_Command_Event_c;
			/*arm mode*/
			/*copy the event data, for this command only arm mode*/
			memUtils_memCopy( pActionHandler->event.pEventData,
							  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							  m_ACE_GET_ZONE_INFORMATION_CMD_PAYLOAD_LENGTH_c );
			
			/* Create the ZCL Header for Enroll  response command*/
			/* Update the Frame Control Field in the scenes response
			Frame which needs to be transmitted*/
			*( pResponseAsduStartLocation + responseLength ) =
					g_Cluster_Specific_Cmd_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
										g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
			
			responseLength++;
			
			/* Update the TSN field in the ZCL Header. TSN field should be same as in
			   the received frame */
			*( pResponseAsduStartLocation + responseLength ) =
								*( pReceivedAsdu + responseLength );
			
			responseLength++;
			
			/*Updating the commandId field in the ZCL Header*/
			*( pResponseAsduStartLocation + responseLength ) =
										g_Get_Zone_Information_Response_Command_c; 			
			responseLength++;
			
			zoneLength = ZCL_Callback_Get_Zone_Information (pReceivedAsdu, pResponseAsduStartLocation + responseLength, g_Get_Zone_Information_c);
			responseLength = responseLength + zoneLength;
			return responseLength;	
	}
	memUtils_memCopy( pActionHandler->event.pEventData,
							  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							  0x01 );	
#endif	
	}
	else {
#if ( g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d == 1 )
		if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ==
											g_Arm_Response_Command_c ){
			pActionHandler->event.eventId = g_Arm_Response_Event_c;
			pActionHandler->event.eventLength = 0x01;
			/*copy the data into the event data*/
			memUtils_memCopy( pActionHandler->event.pEventData,
						  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
						  0x01 );
			pActionHandler->action = Received_Response;
	
		}
		else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ==
											g_Get_Zone_Id_Map_Response_Command_c ) {
			pActionHandler->event.eventId = g_Get_Zone_Id_Map_Response_Event_c;
			 pActionHandler->event.eventLength = 0x21;
			/*copy the data into the event data*/
			memUtils_memCopy( pActionHandler->event.pEventData,
						  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
						  0x21 );
			pActionHandler->action = Received_Response;
		}
		else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ==
											g_Get_Zone_Information_Response_Command_c ) {
			pActionHandler->event.eventId = g_Get_Zone_Information_Response_Event_c;
			pActionHandler->event.eventLength = 0x07;
			/*copy the data into the event data*/
			memUtils_memCopy( pActionHandler->event.pEventData,
						  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
						  0x07 );
			pActionHandler->action = Received_Response;
		}
#endif	/*g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d */
		return g_ZCL_Success_c;
	}
	return responseLength;								
}								


/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */
#endif
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
