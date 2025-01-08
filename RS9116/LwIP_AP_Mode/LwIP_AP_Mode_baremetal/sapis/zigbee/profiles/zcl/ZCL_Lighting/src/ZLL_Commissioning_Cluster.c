/*******************************************************************************
* ZLL_Commissioning_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting ZLL profile's
*   Commissioning Cluster.
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "memory_utility.h"
#if ( g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZLL_Commissioning_Cluster.h"


/*******************************************************************************
* Global Constants
*******************************************************************************/

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

Get_group_identifier_record_list_field   get_group_identifier_request;
Get_end_point_record_list_field   get_end_point_request;

End_point_information_response_field   end_point_information_response_field;
Get_group_identifier_response_field   get_group_identifier_response_field;



#if( g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for ZLL cOMMISSIONING cLUSTER  */
const ZCL_Cluster_Info_t ga_ZLL_Commissioning_Cluster_Server_Cluster_Info =
{
   g_ZLL_COMMISSIONING_CLUSTER_c,
   /* No server Attributes*/
    g_NULL_c,
    /* No server Attributes*/
    g_NULL_c
};

#endif                  /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d */

#if ( g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for ZLL cOMMISSIONING cLUSTER  */
const ZCL_Cluster_Info_t ga_ZLL_Commissioning_Cluster_Client_Cluster_Info =
{
    g_ZLL_COMMISSIONING_CLUSTER_c,
    /* No CLient Attributes*/
    g_NULL_c,
    /* No CLient Attributes*/
    g_NULL_c
};
#endif          /* g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZLL_Handle_Commissioning_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
    uint8_t responseLength = g_ZERO_DATA_LENGTH_c;
#if ( g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t status;
#endif
    pActionHandler->action = No_Response;

    pActionHandler->event.eventLength = asduLength - 0x03;

    if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {

#if ( g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1 )

        if(g_ZLL_Endpoint_Information_Response_Command_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){

              /* Note : no response is required, its an notification*/
              pActionHandler->event.eventId =
                g_ZLL_Endpoint_Information_Response_Event_c;


        }else if(g_ZLL_Get_Group_Identifiers_Response_Command_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ZLL_Get_Group_Identifiers_Response_Event_c;
        }
        else if(g_ZLL_Get_Endpoint_List_Response_Command_c ==
            *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ZLL_Get_Endpoint_List_Response_Event_c;
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }

#endif /* g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1*/

    }
    else{
#if ( g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )

        if (g_ZLL_Get_Group_Identifier_Req_Command_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_ZLL_Get_Group_Identifier_Req_Event_c;

            /* Need to send group Identifier response*/
            pActionHandler->action = Send_Response;

            if(pActionHandler->event.eventLength == sizeof(Get_group_identifier_record_list_field)){
                /* start index*/
                pActionHandler->event.pEventData = &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ];
            }
            else{
                status =  g_ZCL_Invalid_Field_c ;
            }
        }
        else if (g_ZLL_Get_Endpoint_List_Req_Command_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){

             /*Event Id*/
             pActionHandler->event.eventId =
                g_ZLL_Get_Endpoint_List_Req_Event_c;
             /* Send the response*/
             pActionHandler->action = Send_Response;

            if(pActionHandler->event.eventLength == sizeof(Get_end_point_record_list_field)){
                /* start index*/
                pActionHandler->event.pEventData = &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ];
            }
        }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }
#endif /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d== 1*/
    }

    memUtils_memCopy( pActionHandler->event.pEventData,
              &pReceivedAsdu[m_PAYLOAD_LOCATION_c],
              asduLength - m_PAYLOAD_LOCATION_c );



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


#endif                  /* g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d */
