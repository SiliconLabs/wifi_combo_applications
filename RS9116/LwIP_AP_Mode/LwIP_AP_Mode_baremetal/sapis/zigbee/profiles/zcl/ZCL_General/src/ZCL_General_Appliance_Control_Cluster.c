/*******************************************************************************
* ZCL_General_Appliance_Control_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Appliance
* Control cluster
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_EN50523_APPLIANCE_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Appliance_Control_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of
g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d */
const ZCL_Attribute_Info_t ma_Appliance_Control_Cluster_Attribute_info[] =
{
    /* Note :Commented attributes are required or not it is not clear*/
#if( g_START_TIME_ATTRIBUTE_ENABLE_c == 1 )
    { g_START_TIME_ATTRIBUTE_c,
    g_Unsigned_Sixteen_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c | g_Reportable_Attribute_c}
#endif

#if( g_FINISH_TIME_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_FINISH_TIME_ATTRIBUTE_c,
    g_Unsigned_Sixteen_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c | g_Reportable_Attribute_c}
#endif

#if(g_REMAINING_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_REMAINING_TIME_ATTRIBUTE_c,
    g_Unsigned_Sixteen_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c | g_Reportable_Attribute_c}
#endif
};

/* Cluster Info for Appliance Control Cluster */
const ZCL_Cluster_Info_t ga_HA_Appliance_Control_Server_Cluster_Info =
{
   g_APPLIANCE_CONTROL_CLUSTER_c,
   sizeof( ma_Appliance_Control_Cluster_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Appliance_Control_Cluster_Attribute_info
};

#endif                  /* g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_EN50523_APPLIANCE_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Appliance Control Cluster */
const ZCL_Cluster_Info_t ga_HA_Appliance_Control_Client_Cluster_Info =
{
    g_APPLIANCE_CONTROL_CLUSTER_c,
    /* No CLient Attributes*/
    g_NULL_c,
    /* No CLient Attributes*/
    g_NULL_c
};
#endif          /* g_EN50523_APPLIANCE_CONTROL_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_HA_Appliance_Control_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{

    pActionHandler->action = No_Response;
    pActionHandler->event.eventLength = asduLength - 0x03;

    if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {

#if ( g_EN50523_APPLIANCE_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )

        if(g_SignalStateResponseCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceControlStateResponseEvent_c;
            pActionHandler->action = Received_Response;
        }
        else if (g_SignalStateNotificationCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceControlStateNotificationEvent_c;
            pActionHandler->action = Received_Response;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }


#endif /* g_EN50523_APPLIANCE_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1*/

    }
    else{
#if ( g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )

        if( g_ExecutionOfACommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceControlExecutionEvent_c;

        }else if(g_SignalStateCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceControlSignalStateEvent_c;

        }else if(g_WriteFunctionCommad_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceControlWriteFunctionEvent_c;

        } else if(g_OverloadPauseResumeCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ApplianceControlOverloadPauseResumeEvent_c;

        } else if(g_OverloadPauseCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c)) {
            pActionHandler->event.eventId =
                g_ApplianceControlOverloadPauseEvent_c;

        } else if(g_OverloadWarningCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c)) {
            pActionHandler->event.eventId =
                g_ApplianceControlOverloadWarningEvent_c;
        } else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }


#endif /* g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1*/
    }

    memUtils_memCopy( pActionHandler->event.pEventData,
              &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
              asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c );



    return g_ZERO_DATA_LENGTH_c;
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


#endif                  /* _ZCL_GENERAL_APPLIANCE_CONTROL_CLUSTER_H_ */
