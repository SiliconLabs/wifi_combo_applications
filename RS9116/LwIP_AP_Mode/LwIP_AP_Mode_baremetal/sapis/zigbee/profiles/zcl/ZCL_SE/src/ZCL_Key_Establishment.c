/*******************************************************************************
* ZCL_Key_Establishment.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Key Establishment
*   cluster
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Key_Establishment.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/


/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_KeyEstablishment_Client_Cluster_Attribute_Info [] =
{
    { g_KEY_ESTABLISHMENT_SUITE_ATTRIBUTE_c,
      g_Sixteen_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c }

};

const ZCL_Cluster_Info_t ga_KeyEstablishment_Client_Cluster_Info =
{
   g_KEY_ESTABLISHMENT_CLUSTER_c,
   sizeof( ma_KeyEstablishment_Client_Cluster_Attribute_Info )/ sizeof(ZCL_Attribute_Info_t),
   ma_KeyEstablishment_Client_Cluster_Attribute_Info
};
#endif          /*  ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_KeyEstablishment_Server_Cluster_Attribute_Info [] =
{
    { g_KEY_ESTABLISHMENT_SUITE_ATTRIBUTE_c,
      g_Sixteen_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c }
};
const ZCL_Cluster_Info_t ga_KeyEstablishment_Server_Cluster_Info =
{
   g_KEY_ESTABLISHMENT_CLUSTER_c,
   sizeof( ma_KeyEstablishment_Server_Cluster_Attribute_Info )/ sizeof(ZCL_Attribute_Info_t),
   ma_KeyEstablishment_Server_Cluster_Attribute_Info
};

#endif   /*  ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 ) */


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

uint8_t ZCL_Handle_KeyEstablishment_Cluster_Cmd ( uint8_t endPointId,
                                      uint8_t asduLength,
                                      uint8_t* pReceivedAsdu,
                                      uint8_t* pResponseLocation,
                                      ZCL_ActionHandler_t *pActionHandler,
                                      ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                      uint8_t * pAttrsDataLocation )
{
    pActionHandler->action = No_Response;
    pActionHandler->event.eventLength = asduLength - 0x03;

    if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {

        switch( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
            case g_KeyEstInitiateKeyEstResp_c:
			  	pActionHandler->action = Received_Response;
                pActionHandler->event.eventId = g_KeyEstInitiateKeyEstRespEvent_c;
                break;

            case g_KeyEstEphemeralDataResp_c:
			  	pActionHandler->action = Received_Response;
                pActionHandler->event.eventId = g_KeyEstEphemeralDataRespEvent_c;
                break;

            case g_KeyEstConfirmKeyDataResp_c:
			  	pActionHandler->action = Received_Response;
                pActionHandler->event.eventId = g_KeyEstConfirmKeyDataRespEvent_c;
                break;

            case g_KeyEstTerminateKeyEst_c:
			  	pActionHandler->action = Received_Response;
                pActionHandler->event.eventId = g_KeyEstTerminateKeyEstEvent_c;
                break;
#endif
            default:
                pActionHandler->action = Send_Response;
                return g_ZERO_c;
        }
    }
    else {

        switch( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {
#if ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 )
            case g_KeyEstInitiateKeyEstReq_c:
			  	pActionHandler->action = Received_Response;
                pActionHandler->event.eventId = g_KeyEstInitiateKeyEstReqEvent_c;
                break;

            case g_KeyEstEphemeralDataReq_c:
			  	pActionHandler->action = Received_Response;
                pActionHandler->event.eventId = g_KeyEstEphemeralDataReqEvent_c;
                break;

            case g_KeyEstConfirmKeyDataReq_c:
			  	pActionHandler->action = Received_Response;
                pActionHandler->event.eventId = g_KeyEstConfirmKeyDataReqEvent_c;
                break;

            case g_KeyEstTerminateKeyEst_c:
			  	pActionHandler->action = Received_Response;
                pActionHandler->event.eventId = g_KeyEstTerminateKeyEstEvent_c;
                break;
#endif
            default:
                pActionHandler->action = Send_Response;
                return g_ZERO_c;
        }
    }
    memUtils_memCopy( pActionHandler->event.pEventData,
              &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
              asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c );

    return g_ZERO_c;
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
#endif                        /* g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d */
