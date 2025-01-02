/*******************************************************************************
* ZCL_General_Alarms_Cluster.c.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Alarms functinality
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "memory_utility.h"
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Alarms_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define ZCL_ALARM_RESET_ALARM_CMD_PAYLOAD_POSITION                      0x03
#define ZCL_ALARM_ALARM_CMD_PAYLOAD_POSITION                            0x03

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t m_Alarm_Count_Attribute =
{
   g_ALARM_COUNT_ATTRIBUTE_c,
   g_Unsigned_Sixteen_Bit_Integer_Data_c,
   g_Read_Only_Attribute_c
 };


/* Cluster Info for Alarms Cluster */
const ZCL_Cluster_Info_t ga_Alarms_Server_Cluster_Info =
{
   g_ALARMS_CLUSTER_c,
   sizeof(m_Alarm_Count_Attribute)/sizeof(ZCL_Attribute_Info_t),
   &m_Alarm_Count_Attribute
};
#endif          /* ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_ALARMS_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Alarms_Client_Cluster_Info =
{
   g_ALARMS_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* ( g_ALARMS_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Alarms_Cluster_Cmd
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
)
{
    uint8_t responseAsduLength = 0x00;
	
    pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;
	
    if ( ( *pReceivedAsdu & g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) )
    {
#if ( g_ALARMS_CLIENT_CLUSTER_ENABLE_d == 1 )
        switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {
        case g_Alarm_Command_c :
            /* Update the Event Details. This event is handled by the
            application.*/
            pActionHandler->event.eventId = g_Alarm_Occurred_Event_c ;
            memUtils_memCopy ( pActionHandler->event.pEventData,
                      pReceivedAsdu + ZCL_ALARM_ALARM_CMD_PAYLOAD_POSITION, sizeof( Alarm_Command_t ) );
            pActionHandler->action = Received_Response;
			/*If Received request not requires response,then no need to send
            response.So action is No_Response.*/

				
			if ( Is_Default_Response_Required(pReceivedAsdu)) {
				/* The default response frame's header consists of generic command
				   server to client, default response is disabled.Default response command
				   will not be generated in response to reception of another
				   default response command. */
				*( pResponseAsduStartLocation + responseAsduLength ) =
					( g_ZCL_FC_PROFILE_WIDE_COMMAND_c | g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
					g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c );
				
				responseAsduLength++;
				
				/* Update the Transaction Sequence number Field in the Response Frame
				which needs  to be transmitted*/
				*( pResponseAsduStartLocation + responseAsduLength ) = *( pReceivedAsdu + responseAsduLength );
				
				responseAsduLength++;
				
				/* Update the CommandId Field in the Response Frame which needs  to be
				   transmitted */
				*( pResponseAsduStartLocation + responseAsduLength ) = g_Default_Response_Command_c;
				
				responseAsduLength++;
				
				/* update the received command id,for which default response command is
				   created */
				*( pResponseAsduStartLocation + responseAsduLength ) =
					pReceivedAsdu[ ( GetZCLHeaderLength( pReceivedAsdu ) ) - 0x01 ];
				responseAsduLength++;
				/* update the status value in the default response command */
				*( pResponseAsduStartLocation + responseAsduLength ) = g_ZCL_Success_c;
				responseAsduLength++;
				
				pActionHandler->action = Send_Response;
				return responseAsduLength;
			}
//			else
//			   pActionHandler->action = No_Response;
            break;

        case g_Get_Alarm_Response_Command_c :
            /* Update the Event Details. This event is handled by the
            application.*/
            pActionHandler->action = Received_Response;
            break;
        default:
            break;

        }
#endif
    }
    else
    {
        switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ))
        {
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
        case g_Reset_Alarm_Command_c :
            /* Update the Event Details. This event is handled by the
            application.*/
            pActionHandler->event.eventId = g_Reset_Alarm_Command_Event_c ;
            memUtils_memCopy ( pActionHandler->event.pEventData,
                      pReceivedAsdu + ZCL_ALARM_RESET_ALARM_CMD_PAYLOAD_POSITION,
                      sizeof( Reset_Alarm_Command_t ) );

            /*If Received request not requires response,then no need to send
            response.So action is No_Response.*/
            if( Is_Default_Response_Required( pReceivedAsdu ) ) {
                responseAsduLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                                                                       pResponseAsduStartLocation,
                                                                       g_ZCL_Success_c );
            }
            else {
                pActionHandler->action = No_Response;
            }
            break;

        case g_Reset_All_Alarms_Command_c :
            /* Update the Event Details. This event is handled by the
            application.*/
            pActionHandler->event.eventId = g_Reset_All_Alarms_Command_Event_c ;
            /*If Received request not requires response,then no need to send
            response.So action is No_Response.*/
            if( Is_Default_Response_Required( pReceivedAsdu ) ) {
                responseAsduLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                                                                       pResponseAsduStartLocation,
                                                                       g_ZCL_Success_c );
            }
            else {
                pActionHandler->action = No_Response;

            }
            break;

        case g_Get_Alarm_Command_c :

            pActionHandler->event.eventId = g_Get_Alarm_Command_Event_c ;
            /*The command has event and response*/
			pActionHandler->action = Send_Response;
            *pResponseAsduStartLocation++ = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
                g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c ;

            *pResponseAsduStartLocation++ =  *( pReceivedAsdu + 1 );

            *pResponseAsduStartLocation++ = g_Get_Alarm_Response_Command_c;
            responseAsduLength = g_ZCL_HEADER_WITHOUT_MANUF_CODE_c;
            responseAsduLength += ZCL_CallBackCreateAlarmResponse( endPointId,
                                                           pResponseAsduStartLocation );
            break;

        case g_Reset_Alarm_Log_Command_c :
            /* Update the Event Details. This event is handled by the
            application.*/
            pActionHandler->event.eventId = g_Reset_Alarm_Log_Command_Event_c ;
            /*If Received request not requires response,then no need to send
            response.So action is No_Response.*/
            if( Is_Default_Response_Required( pReceivedAsdu ) ) {
                responseAsduLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                                                                       pResponseAsduStartLocation,
                                                                       g_ZCL_Success_c );
            }
            else {
                pActionHandler->action = No_Response;

            }
            break;

        default:
            break;

#endif
        }
    }
    return responseAsduLength;
}


#endif          /* ( g_ALARMS_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
