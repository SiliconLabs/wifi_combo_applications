/*******************************************************************************
* ZCL_General_Power_Profile_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Power Profile
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
#if( g_POWER_PROFILE_CLUSTER_ENABLE_d ==  1)
#include "ZCL_General_Power_Profile_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of  g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d */
const ZCL_Attribute_Info_t ma_Power_Profile_Cluster_Attribute_info[] =
{

#if( g_TOTAL_PROFILE_NUMBER_ATTRIBUTE_ENABLE_c == 1)
    { g_TOTAL_PROFILE_NUMBER_ATTRIBUTE_c,
    g_Unsigned_Eight_Bit_Integer_Data_c,
    g_Read_Only_Attribute_c }
#endif

#if( g_MULTIPLE_SCHEDULE_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_MULTIPLE_SCHEDULE_ATTRIBUTE_c,
    g_Boolean_Data_c,
    g_Read_Only_Attribute_c }

#endif

#if( g_ENERGY_FORMATTING_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_ENERGY_FORMATTING_ATTRIBUTE_c,
    g_Eight_Bit_Bitmap_Data_c,
    g_Read_Only_Attribute_c }

#endif

#if( g_ENERGY_REMOTE_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_ENERGY_REMOTE_ATTRIBUTE_c,
    g_Boolean_Data_c,
    g_Read_Only_Attribute_c | g_Reportable_Attribute_c}

#endif

#if( g_SCHEDULE_MODE_ATTRIBUTE_ENABLE_c == 1)
    ,{ g_SCHEDULE_MODE_ATTRIBUTE_c,
    g_Eight_Bit_Bitmap_Data_c,
    g_Read_Write_Attribute_c | g_Reportable_Attribute_c}

#endif
};

/* Cluster Info for Power Profile Cluster */
const ZCL_Cluster_Info_t ga_HA_Power_Profile_Server_Cluster_Info =
{
   g_POWER_PROFILE_CLUSTER_c,
   sizeof( ma_Power_Profile_Cluster_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Power_Profile_Cluster_Attribute_info
};

#endif                  /* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d */

#if ( g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Power Profile Cluster */
const ZCL_Cluster_Info_t ga_HA_Power_Proile_Client_Cluster_Info =
{
    g_POWER_PROFILE_CLUSTER_c,
    /* No CLient Attributes*/
    g_NULL_c,
    /* No CLient Attributes*/
    g_NULL_c
};
#endif          /* g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_HA_Power_Profile_Cluster_Cmd (  uint8_t endPointId,
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

#if ( g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1 )

        if( g_PowerProfileNotificationCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfileNotificationEvent_c;
            pActionHandler->action = No_Response;
        }
        else if( g_PowerProfileResponseCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfileResponseEvent_c;
            pActionHandler->action = Received_Response;
        }
        else if( g_PowerProfileStateResponseCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfileStateResponseEvent_c;
            pActionHandler->action = Received_Response;
        }
        else if( g_PowerProfilePriceCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfilePriceEvent_c;
            pActionHandler->action = Send_Response;
        }
        else if( g_PowerProfilesStateNotificationCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfilesStateNotificationEvent_c;
            pActionHandler->action = No_Response;
        }
        else if( g_GetOverallSchedulePriceCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_GetOverallSchedulePriceEvent_c;
            pActionHandler->action = Send_Response;
        }
        else if( g_EnergyPhasesScheduleRequestCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_EnergyPhasesScheduleRequestEvent_c;
            pActionHandler->action = Send_Response;
        }
        else if( g_EnergyPhasesScheduleStateResponseCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_EnergyPhasesScheduleStateResponseEvent_c;
            pActionHandler->action = Received_Response;
        }
        else if( g_EnergyPhasesScheduleStateNotificationCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_EnergyPhasesScheduleStateNotificationEvent_c;
            pActionHandler->action = No_Response;
        }
        else if( g_PowerProfileScheduleConstraintsNotificationCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfileScheduleConstraintsNotificationEvent_c;
            pActionHandler->action = No_Response;
        }
        else if( g_PowerProfileScheduleConstraintsResponseCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfileScheduleConstraintsResponseEvent_c;
            pActionHandler->action = Received_Response;
        }
        else if( g_PowerProfilePriceExtendedCommand_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfilePriceExtendedEvent_c;
            pActionHandler->action = Send_Response;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }

#endif /* g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1*/

    }
    else{
#if ( g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1 )
        if( g_PowerProfileRequestCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfileRequestEvent_c  ;
            pActionHandler->action = Send_Response;
        }
        else if( g_PowerProfileStateRequestCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfileStateRequestEvent_c  ;
            pActionHandler->action = Send_Response;
        }
        else if( g_GetPowerProfilePriceResponseCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_GetPowerProfilePriceResponseEvent_c  ;
            pActionHandler->action = Received_Response;
        }
        else if( g_GetOverallSchedulePriceResponseCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_GetOverallSchedulePriceResponseEvent_c  ;
            pActionHandler->action = Received_Response;
        }
        else if( g_EnergyPhasesScheduleNotificationCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_EnergyPhasesScheduleNotificationEvent_c  ;
            pActionHandler->action = No_Response;
        }
        else if( g_EnergyPhasesScheduleResponseCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_EnergyPhasesScheduleResponseEvent_c  ;
            pActionHandler->action = Received_Response;
        }
        else if( g_PowerProfileScheduleConstraintsRequestCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_PowerProfileScheduleConstraintsRequestEvent_c  ;
            pActionHandler->action = Send_Response;
        }
        else if( g_EnergyPhasesScheduleStateRequestCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_EnergyPhasesScheduleStateRequestEvent_c  ;
            pActionHandler->action = Send_Response;
        }
        else if( g_GetPowerProfilePriceExtendedResponseCommand_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId = g_GetPowerProfilePriceExtendedResponseEvent_c  ;
            pActionHandler->action = Received_Response;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }


#endif /* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d== 1*/
    }

    memUtils_memCopy( pActionHandler->event.pEventData,
              &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
              asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c );

    if(pActionHandler->action != No_Response){
        if ( Is_Default_Response_Required ( pReceivedAsdu )) {
            responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                    pResponseAsduStartLocation, g_ZCL_Success_c );
            pActionHandler->action = Send_Response;
    
        }
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


#endif                  /* _ZCL_GENERAL_POWER_PROFILE_CLUSTER_c_ */
