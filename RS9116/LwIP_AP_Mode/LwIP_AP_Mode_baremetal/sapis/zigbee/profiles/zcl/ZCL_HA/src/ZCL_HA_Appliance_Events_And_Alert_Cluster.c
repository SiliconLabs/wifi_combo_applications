/*******************************************************************************
* ZCL_HA_Appliance_Events_And_Alert_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Appliance
* Events and Alert Cluster
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HA_Appliance_Events_And_Alert_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Appliance events and alert Cluster */
const ZCL_Cluster_Info_t ga_HA_Appliance_Events_And_Alert_Server_Cluster_Info =
{
    g_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_c,
    /* No CLient Attributes*/
    g_NULL_c,
    /* No CLient Attributes*/
    g_NULL_c
};

#endif      /* g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d */

#if ( g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Appliance Identification Cluster */
const ZCL_Cluster_Info_t ga_HA_Appliance_Events_And_Alert_Client_Cluster_Info =
{
    g_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_c,
    /* No CLient Attributes*/
    g_NULL_c,
    /* No CLient Attributes*/
    g_NULL_c
};
#endif      /* g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_HA_Appliance_Events_And_Alert_Cluster_Cmd (  uint8_t endPointId,
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

#if ( g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLIENT_CLUSTER_ENABLE_d == 1 )

        if(g_GetAlertsResponse_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_GetAlertsResponseEvent_c;
            pActionHandler->action = Received_Response;
        }
        else if(g_AlertsNotificationCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_AlertsNotificationEvent_c;
        }
        else if(g_EventNotificationCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_EventNotification_c;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }

#endif /* g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLIENT_CLUSTER_ENABLE_d == 1*/

    }
    else{
#if ( g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1 )
        if(g_GetAlertsCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_GetAlertsEvent_c;
            pActionHandler->action = Send_Response;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }


#endif /* g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d== 1*/
    }

    memUtils_memCopy( pActionHandler->event.pEventData,
              &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
              asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c );



    if ( Is_Default_Response_Required ( pReceivedAsdu )) {
        responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                pResponseAsduStartLocation, g_ZCL_Success_c );
        pActionHandler->action = Send_Response;

    }
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


#endif          /* g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1*/
