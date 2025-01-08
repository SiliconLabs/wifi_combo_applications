/*******************************************************************************
* ZCL_General_Identify_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Identify cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "stack_common.h"
#include "ZCL_Common.h"
#include "ZCL_Foundation.h"
#include "ZCL_Stack_Utility.h"
#include "ZCL_Interface.h"
#include "memory_utility.h"
#include "common_includes.h"
#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Identify_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )

const ZCL_Attribute_Info_t ma_Identify_Time_Server_Attribute =
{
   g_IDENTIFY_TIME_ATTRIBUTE_c,
   g_Unsigned_Sixteen_Bit_Integer_Data_c,
   g_Read_Write_Attribute_c
};

/* Cluster Info for Identify Cluster */
const ZCL_Cluster_Info_t ga_Identify_Server_Cluster_Info =
{
   g_IDENTIFY_CLUSTER_c,
   sizeof(ma_Identify_Time_Server_Attribute)/sizeof(ZCL_Attribute_Info_t),
   &ma_Identify_Time_Server_Attribute
};

#endif              /*  ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Identify_Client_Cluster_Info =
{
   g_IDENTIFY_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif              /*  ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/


/******************************************************************************/
uint8_t ZCL_Handle_Identify_Cluster_Cmd
(
     uint8_t endPointId,
     uint8_t asduLength,
     uint8_t* pReceivedAsdu,
     uint8_t* pResponseAsduStartLocation,
     ZCL_ActionHandler_t *pActionHandler,
     ZCL_Cluster_Info_t const *pZCLClusterInfo,
     uint8_t * pAttrsDataBaseLocation
)
{
    uint8_t responseLength= g_ZERO_c;   /* return value */
    pActionHandler->event.eventLength = asduLength - GetZCLHeaderLength(pReceivedAsdu);

    /* Checking the cases for ressponse cmmnd */
    if( ( pReceivedAsdu[0] & g_ZCL_SERVER_TO_CLIENT_DIRECTION_c) ) {
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
        switch(pReceivedAsdu[ GetZCLHeaderLength(pReceivedAsdu) - 0x01]) {
            case g_Identify_Query_Response_Command_c:
                /* updating as a received response */
                pActionHandler->action = Received_Response ;
                break;

            default :
                break;
        }
#endif    /* ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 ) */
    }
    else {
#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
        switch( pReceivedAsdu[ GetZCLHeaderLength(pReceivedAsdu) - 0x01] ) {
            /* Command - g_Identify_Command_c */
            case g_Identify_Command_c:

	  		    pActionHandler->action = Send_Response ;

                pActionHandler->event.eventId = g_Identify_Event_c;

                memUtils_memCopy(pActionHandler->event.pEventData,
                        &pReceivedAsdu[ GetZCLHeaderLength(pReceivedAsdu)], 0x02 );

                /* Writing identify time */
                ZCL_Read_Write_Attribute_Data( g_Write_Attribute_Data_c,
                                          pAttrsDataBaseLocation,
                                          pZCLClusterInfo,
                                          g_IDENTIFY_TIME_ATTRIBUTE_c,
                                          &pReceivedAsdu[ GetZCLHeaderLength(pReceivedAsdu)] );
                /* create if default reaponse required */
                if ( Is_Default_Response_Required( pReceivedAsdu ) ) {
                    responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                                            pResponseAsduStartLocation, g_ZCL_Success_c);
                }
                else {
                        pActionHandler->action = No_Response;
                }

                break;
            case g_Identify_Query_Command_c:
                pActionHandler->event.eventId = g_Identify_Query_Event_c;

                if( g_TRUE_c == ZCL_Callback_Is_Device_In_Identifing() ) {

	  				 pActionHandler->action = Send_Response ;

                     *( pResponseAsduStartLocation ) =
			        ( *pReceivedAsdu )| g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
			                            g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
                     responseLength++;

                    /* Transaction sequence no */
                    *( pResponseAsduStartLocation + responseLength ) = *(pReceivedAsdu +
                                                             responseLength);
                     responseLength++;
                    pResponseAsduStartLocation[responseLength] =
                                        g_Identify_Query_Response_Command_c;
                    responseLength++;

                    memUtils_memCopy((uint8_t*)&(pResponseAsduStartLocation[responseLength]),
                             pAttrsDataBaseLocation,2);
                    responseLength += 2;
                }
                else{
                    pActionHandler->action = No_Response;
                }
                break;

#if ( g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
            case g_Trigger_Effect_Command_c:

                 pActionHandler->action = Send_Response ;
				 /*load event id */
                 pActionHandler->event.eventId = g_Trigger_Effect_Event_c;
				/*get effect identifier and effect variant field */
                 *pActionHandler->event.pEventData = pReceivedAsdu[3];
                 *( pActionHandler->event.pEventData + 1 ) = pReceivedAsdu[4];
                 ZCL_Callback_Identifier_Trigger_Effect(&(pActionHandler->event));

                /* create if default reaponse required */
                if ( Is_Default_Response_Required( pReceivedAsdu ) ) {
                    responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                                            pResponseAsduStartLocation, g_ZCL_Success_c);
                }
                else {
                        pActionHandler->action = No_Response;
                }
                break;
#endif

            default :
                break;
        }
#endif          /* ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 ) */
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

#endif                                   /* g_IDENTIFY_CLUSTER_ENABLE_d == 1 */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

