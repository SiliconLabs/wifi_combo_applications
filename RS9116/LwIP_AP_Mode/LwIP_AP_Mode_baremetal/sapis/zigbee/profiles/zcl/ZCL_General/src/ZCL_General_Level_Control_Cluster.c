/*******************************************************************************
* ZCL_General_Level_Control_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Level control
* functinality
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#if( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Level_Control_Cluster.h"
#if ( g_ZLL_ON_OFF_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_On_Off_Cluster.h"
#endif

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_MOVETOLEVEL_CMD_PAYLOAD_LENGTH_c              0x03
#define m_MOVE_CMD_PAYLOAD_LENGTH_c                     0x02
#define m_STEP_CMD_PAYLOAD_LENGTH_c                     0x04

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Level_Control_Attributes[] =
{
    {
        g_CURRENT_LEVEL_ATTRIBUTE_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c | g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c
    }
#if( g_LEVEL_REMAINING_TIME_ATTRIBUTE_ENABLE_d == 1)
   ,
    {
        g_LEVEL_REMAINING_TIME_ATTRIBUTE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif              /*g_LEVEL_REMAINING_TIME_ATTRIBUTE_c*/

#if(g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
   ,
    {
        g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    }
#endif      /*g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d*/

#if(g_ON_LEVEL_ATTRIBUTE_ENABLE_d == 1)
   ,
    {
        g_ON_LEVEL_ATTRIBUTE_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    }
#endif /*g_ON_LEVEL_ATTRIBUTE_ENABLE_d*/
	
#if(g_HA_1_2_d == 1)	

#if(g_ON_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
   ,
    {
        g_ON_TRANSITION_TIME_ATTRIBUTE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    }
#endif /*g_ON_TRANSITION_TIME_ATTRIBUTE_ENABLE_d*/
	
#if(g_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
   ,
    {
        g_OFF_TRANSITION_TIME_ATTRIBUTE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    }
#endif /*g_ON_TRANSITION_TIME_ATTRIBUTE_ENABLE_d*/

#if(g_DEFAULT_MOVE_RATE_ATTRIBUTE_ENABLE_d == 1)
   ,
    {
        g_DEFAULT_MOVE_RATE_ATTRIBUTE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    }
#endif /*g_ON_TRANSITION_TIME_ATTRIBUTE_ENABLE_d*/	
#endif	
};

/*cluster info for level server control cluster*/
const ZCL_Cluster_Info_t ga_Level_Control_Server_Cluster_Info =
{
    g_LEVEL_CONTROL_CLUSTER_c,
    sizeof(ma_Level_Control_Attributes)/sizeof(ZCL_Attribute_Info_t),
    ma_Level_Control_Attributes
};
#endif          /* ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 ) */

/*cluster info for level client control cluster*/
#if ( g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Level_Control_Client_Cluster_Info =
{
    g_LEVEL_CONTROL_CLUSTER_c,
    g_NULL_c,
    g_NULL_c
};
#endif              /* ( g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Level_Control_Cluster_Cmd (uint8_t endPointId,
                                    uint8_t asduLength,uint8_t* pReceivedAsdu,
                                    uint8_t* pResponseAsduStartLocation,
                                    ZCL_ActionHandler_t *pActionHandler,
                                    ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                    uint8_t * pAttrsDataBaseLocation )
{
    uint8_t responseAsduLength = g_ZERO_c;
#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t eventDataLength = g_ZERO_c;
#if ( g_ZLL_ON_OFF_CLUSTER_ENABLE_d == 1 )
    uint8_t On_Off_Value;
#endif /* g_ZLL_ON_OFF_CLUSTER_ENABLE_d */
#endif /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */

    pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;
    if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) {
#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
        switch(  pReceivedAsdu[ GetZCLHeaderLength( pReceivedAsdu )-1] )
        {
            case g_Move_To_Level_Command_c:
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
                ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,
                                 pAttrsDataBaseLocation,
                                 pZCLClusterInfo,
                                 g_ON_OFF_ATTRIBUTE_c,
                                 (uint8_t*)&On_Off_Value  );

                if ( On_Off_Value == g_ZERO_c)
                pActionHandler->event.eventId = g_ZERO_c; 
                else
#endif/* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 */
                pActionHandler->event.eventId = g_Move_To_Level_Event_c;
                eventDataLength = m_MOVETOLEVEL_CMD_PAYLOAD_LENGTH_c;
            break;

            case g_Move_To_Level_With_On_Off_Command_c:
                pActionHandler->event.eventId = g_Move_To_Level_With_On_Off_Event_c;
                eventDataLength = m_MOVETOLEVEL_CMD_PAYLOAD_LENGTH_c;
             break;

            case g_Move_Command_c:
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
                ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,
                                 pAttrsDataBaseLocation,
                                 pZCLClusterInfo,
                                 g_ON_OFF_ATTRIBUTE_c,
                                 (uint8_t*)&On_Off_Value  );

                if ( On_Off_Value == g_ZERO_c)
                pActionHandler->event.eventId = g_ZERO_c; 
                else
#endif /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 */
                pActionHandler->event.eventId = g_Move_Event_c;
                eventDataLength = m_MOVE_CMD_PAYLOAD_LENGTH_c;
                break;

            case g_Move_With_On_Off_Command_c:
                pActionHandler->event.eventId = g_Move_With_On_Off_Event_c;
                eventDataLength = m_MOVE_CMD_PAYLOAD_LENGTH_c;
             break;

            case g_Step_With_On_Off_Command_c:
                pActionHandler->event.eventId = g_Step_With_On_Off_Event_c;
                eventDataLength = m_STEP_CMD_PAYLOAD_LENGTH_c;
                break;

            case g_Step_Command_c:
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
                ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,
                                 pAttrsDataBaseLocation,
                                 pZCLClusterInfo,
                                 g_ON_OFF_ATTRIBUTE_c,
                                 (uint8_t*)&On_Off_Value  );

                if ( On_Off_Value == g_ZERO_c)
                pActionHandler->event.eventId = g_ZERO_c; 
                else
#endif/* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
                pActionHandler->event.eventId = g_Step_Event_c;
                eventDataLength = m_STEP_CMD_PAYLOAD_LENGTH_c;
                break;

            case g_Stop_Command_c:
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
                ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,
                                 pAttrsDataBaseLocation,
                                 pZCLClusterInfo,
                                 g_ON_OFF_ATTRIBUTE_c,
                                 (uint8_t*)&On_Off_Value  );

                if ( On_Off_Value == g_ZERO_c)
                pActionHandler->event.eventId = g_ZERO_c; 
                else
#endif/* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
                pActionHandler->event.eventId = g_Stop_Event_c;
                break;

            case g_Stop_With_On_Off_Command_c:
                pActionHandler->event.eventId = g_Stop_With_On_Off_Event_c;
                break;

            default:
                return responseAsduLength;
        }
        memUtils_memCopy( pActionHandler->event.pEventData,
                  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
                  eventDataLength );

       if ( Is_Default_Response_Required ( pReceivedAsdu ) ) {
            responseAsduLength = ZCL_CreateDefaultResponseCommand (
                                  pReceivedAsdu,
                                  pResponseAsduStartLocation,
                                  g_ZCL_Success_c );
		 pActionHandler->action = Send_Response;

       }
       else{
         pActionHandler->action = No_Response;
       }
#endif 					/* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */
	   }
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

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif  /*( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)*/
