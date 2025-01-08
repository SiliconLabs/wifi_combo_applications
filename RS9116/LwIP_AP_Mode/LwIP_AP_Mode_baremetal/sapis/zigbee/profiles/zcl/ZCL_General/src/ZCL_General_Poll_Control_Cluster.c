/*******************************************************************************
* ZCL_General_Poll_Control_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Poll Control
* cluster
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if( g_POLL_CONTROL_CLUSTER_ENABLE_d ==  1)
#include "ZCL_General_Poll_Control_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of  g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d */
const ZCL_Attribute_Info_t ma_Poll_Control_Cluster_Server_Attribute_info[] =
{
#if (g_CHECK_IN_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
    { g_CHECK_IN_INTERVAL_ATTRIBUTE_ID_c,
    g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
    g_Read_Write_Attribute_c
    }
#endif

#if (g_LONG_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_LONG_POLL_INTERVAL_ATTRIBUTE_ID_c,
    g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c
    }
#endif

#if (g_SHORT_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_SHORT_POLL_INTERVAL_ATTRIBUTE_ID_c,
    g_Unsigned_Sixteen_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c
    }
#endif

#if (g_FAST_POLL_TIMEOUT_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_FAST_POLL_TIMEOUT_ATTRIBUTE_ID_c,
    g_Unsigned_Sixteen_Bit_Integer_Data_c,
    g_Read_Write_Attribute_c
    }
#endif

#if (g_CHECK_IN_INTERVAL_MIN_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_CHECK_IN_INTERVAL_MIN_ATTRIBUTE_ID_c,
    g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c
    }
#endif

#if (g_LONG_POLL_INTERVAL_MIN_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_LONG_POLL_INTERVAL_MIN_ATTRIBUTE_ID_c,
    g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c
    }
#endif

#if (g_FAST_POLL_TIMEOUT_MAX_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_FAST_POLL_TIMEOUT_MAX_ATTRIBUTE_ID_c,
    g_Unsigned_Sixteen_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c
    }
#endif
};

/* Cluster Info for Poll Control Cluster */
const ZCL_Cluster_Info_t ga_HA_Poll_Control_Server_Cluster_Info =
{
   g_POLL_CONTROL_CLUSTER_c,
   sizeof( ma_Poll_Control_Cluster_Server_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Poll_Control_Cluster_Server_Attribute_info
};

#endif                  /* g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )


/* Cluster Info for poll control Cluster */
const ZCL_Cluster_Info_t ga_HA_Poll_Control_Client_Cluster_Info =
{
    g_POLL_CONTROL_CLUSTER_c,
    /* No Client Attributes*/
    g_NULL_c,
    /* No Client Attributes*/
    g_NULL_c
};
#endif          /* g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_HA_Poll_Control_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{

    uint8_t responseLength = g_ZERO_DATA_LENGTH_c;

    pActionHandler->action = No_Response;
    pActionHandler->event.eventLength = asduLength - 0x03;

    if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {

#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )

        if( g_CheckinCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_CheckinEvent_c;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }

#endif /* g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1*/

    }
    else{
#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
        if( g_CheckinResponseCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_CheckinResponseEvent_c  ;
            pActionHandler->action = Received_Response;
        }
        else if( g_FastPollStopCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_FastPollStopEvent_c  ;
        }
        else if( g_SetLongPollIntervalCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_SetLongPollIntervalEvent_c  ;
        }
        else if( g_SetShortPollIntervalCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_SetShortPollIntervalEvent_c  ;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }


#endif /* g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d== 1*/
    }

    memUtils_memCopy( pActionHandler->event.pEventData,
              &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
              asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c );


   
    return responseLength;
}

/*******************************************************************************
* Private Functions
*******************************************************************************/

/* None */

/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/

/* None */

/*******************************************************************************
* End Of File
*******************************************************************************/


#endif                  /* g_POLL_CONTROL_CLUSTER_ENABLE_d */
