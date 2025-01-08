/*******************************************************************************
* ZCL_HA_Appliance_Statistics_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Appliance
* Statistics cluster
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_APPLIANCE_STATISTICS_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HA_Appliance_Statistics_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of
g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d */
const ZCL_Attribute_Info_t ma_Appliance_Statistics_Cluster_Attribute_info[] =
{
#if( g_LOG_MAX_SIZE_ATTRIBUTE_ENABLE_c == 1 )
    { g_LOG_MAX_SIZE_c,
    g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c}
#endif

#if( g_LOG_QUEUE_MAX_SIZE_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_LOG_QUEUE_MAX_SIZE_c,
    g_Unsigned_Eight_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c}
#endif
};

/* Cluster Info for Appliance Statistics Cluster */
const ZCL_Cluster_Info_t ga_HA_Appliance_Statistics_Server_Cluster_Info =
{
   g_APPLIANCE_STATISTICS_CLUSTER_c,
   sizeof( ma_Appliance_Statistics_Cluster_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Appliance_Statistics_Cluster_Attribute_info
};

#endif                  /* g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d */

#if ( g_APPLIANCE_STATISTICS_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Appliance Statistics Cluster */
const ZCL_Cluster_Info_t ga_HA_Appliance_Statistics_Client_Cluster_Info =
{
    g_APPLIANCE_STATISTICS_CLUSTER_c,
    /* No CLient Attributes*/
    g_NULL_c,
    /* No CLient Attributes*/
    g_NULL_c
};
#endif          /* g_APPLIANCE_STATISTICS_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_HA_Appliance_Statistics_Cluster_Cmd (  uint8_t endPointId,
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

#if ( g_APPLIANCE_STATISTICS_CLIENT_CLUSTER_ENABLE_d == 1 )

        if(g_LogNotificationCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceStatisticsLogNotificationEvent_c;
        }
        else if(g_LogResponseCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
              pActionHandler->action = Received_Response;
            pActionHandler->event.eventId =
                g_ApplianceStatisticsLogResponseEvent_c;
        }
        else if(g_LogQueueResponseCommad_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
              pActionHandler->action = Received_Response;
            pActionHandler->event.eventId =
                g_ApplianceStatisticsLogQueueEvent_c;

        }else if(g_StatisticsAvailableCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceStatisticsStatisticsAvailableEvent_c;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }

#endif /* g_APPLIANCE_STATISTICS_CLIENT_CLUSTER_ENABLE_d == 1*/

    }
    else{
#if ( g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1 )
        if(g_LogRequest_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceStatisticsLogRequestEvent_c;
        }
        else if (g_LogQueueRequest_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceStatisticsLogQueueRequestEvent_c;
        }

        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }


#endif /* g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d== 1*/
    }

    memUtils_memCopy( pActionHandler->event.pEventData,
              &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
              asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c );


//    if ( Is_Default_Response_Required ( pReceivedAsdu )) {
//        responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
//                pResponseAsduStartLocation, g_ZCL_Success_c );
//        pActionHandler->action = Send_Response;
//
//    }
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


#endif                  /* g_APPLIANCE_STATISTICS_CLUSTER_ENABLE_d */
