/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "stack_util.h"
#include "memory_utility.h"
#include "common_includes.h"

#if ( g_IAS_WD_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_WD_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_WD_START_WARNING_CMD_PAYLOAD_LENGTH_c		0x03
#define m_WD_SQUAK_WARNING_CMD_PAYLOAD_LENGTH_c		0x01

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_IAS_WD_SERVER_CLUSTER_ENABLE_d == 1 )

/* Server Cluster Info for IAS WD Cluster*/
const ZCL_Attribute_Info_t m_IAS_WD_Server_Attribute_Info =
{

   g_IAS_WD_MAX_DURATION_ATTRIBUTE_c,
   g_Unsigned_Sixteen_Bit_Integer_Data_c,
   g_Read_Write_Attribute_c

};


/*Server Cluster Info for IAS Zone Cluster*/
const ZCL_Cluster_Info_t ga_IAS_WD_Server_Cluster_Info =
{
   g_IAS_WD_CLUSTER_c,
   sizeof(m_IAS_WD_Server_Attribute_Info)/sizeof(ZCL_Attribute_Info_t),
   &m_IAS_WD_Server_Attribute_Info
};
#endif

#if ( g_IAS_WD_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Client Cluster Info for IAS Zone Cluster*/
const ZCL_Cluster_Info_t ga_IAS_WD_Client_Cluster_Info =
{
	g_IAS_WD_CLUSTER_c,
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

uint8_t ZCL_Handle_IAS_WD_Cluster_Cmd (  	
								uint8_t endPointId,
								uint8_t asduLength,
								uint8_t* pReceivedAsdu,
								uint8_t* pResponseAsduStartLocation,
								ZCL_ActionHandler_t *pActionHandler,
								ZCL_Cluster_Info_t const *pZCLClusterInfo,
								uint8_t * pAttrsDataBaseLocation ){
								
	uint8_t responseAsduLength = g_ZERO_c;								
#if ( g_IAS_WD_SERVER_CLUSTER_ENABLE_d == 1 ) 								
		uint8_t eventLength = g_NULL_c;
#endif
	if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) {
#if ( g_IAS_WD_SERVER_CLUSTER_ENABLE_d == 1 ) 	
		switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
		{
			case g_Start_Warning_Command_c:
				/* Update the Event Details. This event is handled by the
				application.*/
				pActionHandler->action = Received_Response;
				/*Start Warning command received event*/
				pActionHandler->event.eventId = g_Start_Warning_Command_Event_c; 				
				eventLength = m_WD_START_WARNING_CMD_PAYLOAD_LENGTH_c; 	
                pActionHandler->event.eventLength = m_WD_START_WARNING_CMD_PAYLOAD_LENGTH_c;
				
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
					responseAsduLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, g_ZCL_Success_c );
					pActionHandler->action = Send_Response;
				}
			break;		
			case g_Squawk_Command_c:
				/* Update the Event Details. This event is handled by the
				application.*/
				pActionHandler->action = Received_Response;
				/*Squak command received event*/
				pActionHandler->event.eventId = g_Squawk_Command_Event_c; 				
				eventLength = m_WD_SQUAK_WARNING_CMD_PAYLOAD_LENGTH_c;
				pActionHandler->event.eventLength = m_WD_SQUAK_WARNING_CMD_PAYLOAD_LENGTH_c;
				
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
					responseAsduLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, g_ZCL_Success_c );
					pActionHandler->action = Send_Response;
				}
			break;
			default:
				return g_ZCL_UnSup_Cluster_Command_c;
		}
											
		/*copy the event data*/
		memUtils_memCopy( pActionHandler->event.pEventData,
							pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							eventLength );
			
#endif  /*g_IAS_WD_SERVER_CLUSTER_ENABLE_d*/
	}	/* g_ZCL_CLIENT_TO_SERVER_DIRECTION_c */
	return responseAsduLength;								
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
