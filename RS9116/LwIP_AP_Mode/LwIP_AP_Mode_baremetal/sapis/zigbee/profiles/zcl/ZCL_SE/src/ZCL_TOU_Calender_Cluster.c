/*******************************************************************************
* ZCL_TOU_Calender_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting DRLC cluster
*
*******************************************************************************/

#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#if ( g_TOU_CALENDER_CLUSTER_ENABLE_d == 1 )
#include "memory_utility.h"
#include "ZCL_Foundation.h"
#include "ZCL_TOU_Calender_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/
#define m_GET_CURRENT_PRICE_CMD_LENGTH_c		1
#define m_GET_SCHEDULED_PRICE_CMD_LENGTH_c		1

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_TOU_CALENDER_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_TOU_Calender_Server_Cluster_Attribute_Info [] =
{

#if ( g_AUX_SWITCH_1_ATTRIBUTE_ENABLE_d == 1 )
    { g_AUX_SWITCH_1_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c },
#endif         /* g_AUX_SWITCH_1_ATTRIBUTE_ENABLE_d*/	
#if ( g_AUX_SWITCH_2_ATTRIBUTE_ENABLE_d == 1 )
    { g_AUX_SWITCH_2_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c },
#endif      /* g_AUX_SWITCH_2_ATTRIBUTE_ENABLE_d*/	
#if ( g_AUX_SWITCH_3_ATTRIBUTE_ENABLE_d == 1 )	

    { g_AUX_SWITCH_3_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c },
#endif      /* g_AUX_SWITCH_3_ATTRIBUTE_ENABLE_d*/	
#if ( g_AUX_SWITCH_4_ATTRIBUTE_ENABLE_d == 1 )	

    { g_AUX_SWITCH_4_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c },
#endif      /*g_AUX_SWITCH_4_ATTRIBUTE_ENABLE_d */	
#if ( g_AUX_SWITCH_5_ATTRIBUTE_ENABLE_d == 1 )	
	
    { g_AUX_SWITCH_5_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c },
#endif      /*g_AUX_SWITCH_5_ATTRIBUTE_ENABLE_d */	
#if ( g_AUX_SWITCH_6_ATTRIBUTE_ENABLE_d == 1 )	

    { g_AUX_SWITCH_6_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c },
#endif      /* g_AUX_SWITCH_6_ATTRIBUTE_ENABLE_d*/	
#if ( g_AUX_SWITCH_7_ATTRIBUTE_ENABLE_d == 1 )	

    { g_AUX_SWITCH_7_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c },
#endif      /* g_AUX_SWITCH_7_ATTRIBUTE_ENABLE_d*/	
#if ( g_AUX_SWITCH_8_ATTRIBUTE_ENABLE_d == 1 )	

    { g_AUX_SWITCH_8_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c },
#endif      /* g_AUX_SWITCH_8_ATTRIBUTE_ENABLE_d*/	
	{g_NULL_c, g_NULL_c, g_NULL_c}
};

const ZCL_Cluster_Info_t  ga_TOU_Calender_Server_Cluster_Info=
{
   g_TOU_CALENDER_CLUSTER_c,
   (sizeof( ma_TOU_Calender_Server_Cluster_Attribute_Info )-1)/ sizeof(ZCL_Attribute_Info_t),
   ma_TOU_Calender_Server_Cluster_Attribute_Info
};
#endif          /* g_TOU_CALENDER_SERVER_CLUSTER_ENABLE_d */


#if ( g_TOU_CALENDER_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_TOU_Calender_Client_Cluster_Info =
{
   g_TOU_CALENDER_CLUSTER_c,
   g_NULL_c,
   g_NULL_c,
};

#endif          /* g_TOU_CALENDER_CLIENT_CLUSTER_ENABLE_d */



/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/* None */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* None */

/*******************************************************************************
* Public Functions
*******************************************************************************/
uint8_t ZCL_Handle_TOU_Calender_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
	pActionHandler->action = No_Response;
	pActionHandler->event.eventLength = asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c;
	if( ( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) ) {
	
#if ( g_TOU_CALENDER_CLIENT_CLUSTER_ENABLE_d == 1 )
		pActionHandler->action = Received_Response;
	
		if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_PublishCalenderCmd_c)
		{
		
		}
		else if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_PublishDayProfileCmd_c){
		
		}
		else if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_PublishWeekProfileCmd_c){
		
		}
		else if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_PublishSeasonsCmd_c){
		
		}
		else if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_PublishSpecialDaysCmd_c){
		
		}
		
#endif        /*g_TOU_CALENDER_CLIENT_CLUSTER_ENABLE_d */		
	}
#if ( g_TOU_CALENDER_SERVER_CLUSTER_ENABLE_d == 1 )
	
	else {
	
	    switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {
			case g_GetCalenderCmd_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetCalenderEvent_c ;
				memUtils_memCopy ( pActionHandler->event.pEventData,
						  				&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
										   m_GET_CURRENT_PRICE_CMD_LENGTH_c );
				break;
	
			case g_GetDayProfilesCmd_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetDayProfilesEvent_c;
				memUtils_memCopy ( pActionHandler->event.pEventData,
						  				&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
									   m_GET_SCHEDULED_PRICE_CMD_LENGTH_c );
				break;
	
			case g_GetWeekProfilesCmd_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetWeekProfileEvent_c;
				memUtils_memCopy ( pActionHandler->event.pEventData,
						  				&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
									   m_GET_SCHEDULED_PRICE_CMD_LENGTH_c );
				break;
	
			case g_GetSeasonsCmd_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetSeasonsEvent_c;
				memUtils_memCopy ( pActionHandler->event.pEventData,
						  				&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
									   m_GET_SCHEDULED_PRICE_CMD_LENGTH_c );
				break;
	
			case g_GetSpecialDays_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetSpecialDaysEvent_c;
				memUtils_memCopy ( pActionHandler->event.pEventData,
						  				&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
									   m_GET_SCHEDULED_PRICE_CMD_LENGTH_c );
				break;
		}
	}
#endif     /* g_TOU_CALENDER_SERVER_CLUSTER_ENABLE_d*/	
	return g_ZERO_DATA_LENGTH_c;

}

/*******************************************************************************/



/*******************************************************************************
* Private Functions
*******************************************************************************/

/* None */

#endif                                /* g_TOU_CALENDER_CLUSTER_ENABLE_d */
/*******************************************************************************
* End Of File
*******************************************************************************/
