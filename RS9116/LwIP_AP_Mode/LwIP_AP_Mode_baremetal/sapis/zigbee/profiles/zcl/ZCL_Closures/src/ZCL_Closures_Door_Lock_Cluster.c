/*******************************************************************************
* ZCL_Closures_Door_Lock_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Door Lock cluster
*
*******************************************************************************/

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "memory_utility.h"
#if(g_DOOR_LOCK_CLUSTER_ENABLE_d == 1)
#include "ZCL_Closures_Door_Lock_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/



#if ( g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d == 1 )

const ZCL_Attribute_Info_t ma_Door_Lock_Attribute_Info [] =
{

   {
      g_LOCK_STATE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c| g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c,

   },

   {
      g_LOCK_TYPE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c,
   },

   {
      g_ACTUATOR_ENABLED_ATTRIBUTE_c,
      g_Boolean_Data_c,
      g_Read_Only_Attribute_c,

   },

#if ( g_DOOR_STATE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_DOOR_STATE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c| g_Reportable_Attribute_c,
   },
#endif          /*g_DOOR_STATE_ATTRIBUTE_ENABLE_d*/

#if ( g_DOOR_OPEN_EVENTS_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_DOOR_OPEN_EVENTS_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif         /*g_DOOR_OPEN_EVENTS_ATTRIBUTE_ENABLE_d*/

#if ( g_DOOR_CLOSED_EVEVTS_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_DOOR_CLOSED_EVEVTS_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif      /*g_DOOR_CLOSED_EVEVTS_ATTRIBUTE_ENABLE_d*/

#if ( g_DOOR_OPEN_PERIOD_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_DOOR_OPEN_PERIOD_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c,

   }
#endif      /*g_DOOR_OPEN_PERIOD_ATTRIBUTE_ENABLE_d*/

};

const ZCL_Cluster_Info_t ga_Door_Lock_Server_Cluster_Info =
{
   g_DOOR_LOCK_CLUSTER_c,
   sizeof(ma_Door_Lock_Attribute_Info)/sizeof(ZCL_Attribute_Info_t),
   ma_Door_Lock_Attribute_Info,
};

#endif          /*  ( g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_DOOR_LOCK_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Door_Lock_Client_Cluster_Info =
{
   g_DOOR_LOCK_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /*  ( g_DOOR_LOCK_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Closure_Door_Lock_Cmd ( uint8_t endPointId,
                             uint8_t asduLength,
                             uint8_t* pReceivedAsdu,
                             uint8_t* pResponseAsduStartLocation,
                             ZCL_ActionHandler_t *pActionHandler,
                             ZCL_Cluster_Info_t const *pZCLClusterInfo,
                             uint8_t * pAttrsDataBaseLocation )
{
    uint8_t responseLength = 0x00;
    pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;
    if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) {
	
	 pActionHandler->action = Send_Response;
#if ( g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d == 1 )
        switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {
            case g_Lock_Door_Command_c:
                pActionHandler->event.eventId = g_Lock_Door_Event_c;
				
                break;

               case g_Unlock_Door_Command_c:
                  pActionHandler->event.eventId = g_Unlock_Door_Event_c;
                  break;

            default:
                return responseLength;
        }
         memUtils_memCopy ( pActionHandler->event.pEventData,
               &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
               asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c ) ;
#endif             /* ( g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d == 1 ) */
    }
    else {
	    pActionHandler->action = Received_Response;

#if ( g_DOOR_LOCK_CLIENT_CLUSTER_ENABLE_d == 1 )
        switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {
            case g_Lock_Door_Response_c:
                pActionHandler->event.eventId = g_Lock_Door_Response_c;
                break;

            case g_Unlock_Door_Response_c:
                pActionHandler->event.eventId = g_Unlock_Door_Response_c;
                break;

            default:
                break;
        }
#endif            /* ( g_DOOR_LOCK_CLIENT_CLUSTER_ENABLE_d == 1 ) */
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

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*g_DOOR_LOCK_CLUSTER_ENABLE_d*/
