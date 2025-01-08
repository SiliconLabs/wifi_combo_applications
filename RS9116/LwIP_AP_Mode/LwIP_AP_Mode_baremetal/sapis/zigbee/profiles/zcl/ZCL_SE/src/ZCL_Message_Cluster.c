/*******************************************************************************
* ZCL_Message_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Message cluster
*
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Message_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_MESSAGE_CONFIRMATION_LENGTH_c                 0x08
#define m_DISPLAY_MESSAGE_FIXED_LENGTH_c                0x0b
#define m_MESSAGE_ID_LENGTH_c                           0x04
#define m_TIME_LENGTH_OF_TIME_ATTRIBUTE_DATA_c          0x04
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_message_Server_Cluster_Info =
{
   g_MESSAGING_CLUSTER_c,
   g_NULL_c,
   g_NULL_c,
};

#endif          /*  ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_message_Client_Cluster_Info =
{
   g_MESSAGING_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /*  ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 ) */

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

uint8_t ZCL_Handle_Message_Cluster_Cmd (  uint8_t endPointId,
                                          uint8_t asduLength,
                                          uint8_t* pReceivedAsdu,
                                          uint8_t* pResponseLocation,
                                          ZCL_ActionHandler_t *pActionHandler,
                                          ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                          uint8_t * pAttrsDataBaseLocation )
{

    if( ( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) ) {
        /* Initiate the action to received response, since the commands
           are received */
#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )

        pActionHandler->action = Received_Response;

        if( g_Display_Message_Command_c == pReceivedAsdu[g_ZCL_HEADER_COMMAND_OFFSET_c] ) {

            /* update the event to display message command event */
            pActionHandler->event.eventId = g_Display_Message_Events_c;
        }
        else if( g_Cancel_Message_Command_c == pReceivedAsdu[g_ZCL_HEADER_COMMAND_OFFSET_c] ){
            /* Update event as Cancel message event */
            pActionHandler->event.eventId = g_Cancel_Message_Events_c;
        }
        else {
            /*  since it is an error scenario, default response command should be
                returned with status as unsupported cluster command */
            pActionHandler->action = Send_Response;
        }
#endif    /* ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 ) */
    }
    else{
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
        if( g_GetLast_Message_Command_c == pReceivedAsdu[g_ZCL_HEADER_COMMAND_OFFSET_c] ){
           /* Update the event with Get last message event */
           pActionHandler->event.eventId = g_GetLast_Message_Events_c;
           /* Application should send response.Hence initiate the action
             to No response */
            pActionHandler->action = No_Response;
        }
        else if( g_Message_Confirmation_Command_c == pReceivedAsdu[g_ZCL_HEADER_COMMAND_OFFSET_c] ){
            pActionHandler->action = Received_Response;
            /* Update the event with Message confirmation event */
            pActionHandler->event.eventId = g_Message_Confirmation_Events_c;
        }
        else {
            /* since it is an error scenario, default response command should be
               returned with status as unsupported cluster command */
            pActionHandler->action = Send_Response;
        }
#endif      /* ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )   */
    }
    memUtils_memCopy ( pActionHandler->event.pEventData,
               &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
               asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c ) ;

    return g_ZERO_DATA_LENGTH_c;
}


/******************************************************************************/
#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )

uint8_t zclCreateMessageConfirmation ( uint8_t *pResponseAsdu,
                                uint8_t transactionSeqNum, uint8_t* pMessageId )
{
    uint32_t currentTime;
    *pResponseAsdu++ = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    *pResponseAsdu++ = transactionSeqNum;
    *pResponseAsdu++ = g_Message_Confirmation_Command_c;
    memUtils_memCopy ( pResponseAsdu, pMessageId, m_MESSAGE_ID_LENGTH_c );
    pResponseAsdu += m_MESSAGE_ID_LENGTH_c;
    currentTime = ZCL_CallBackGetCurrentTime();
    memUtils_memCopy (  pResponseAsdu, (uint8_t*)&currentTime, m_TIME_LENGTH_OF_TIME_ATTRIBUTE_DATA_c );

    return m_MESSAGE_CONFIRMATION_LENGTH_c + g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c;
}
#endif                  /* ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/******************************************************************************/
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t zclCreateDisplayMessageCommand ( uint8_t *pResponseAsdu,
           DisplayMessage_t* pDisplayMessage, uint8_t transactionSeqNum )
{
    *pResponseAsdu++ = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c;
    *pResponseAsdu++ = transactionSeqNum;
    *pResponseAsdu++ = g_Display_Message_Command_c;

    memUtils_memCopy ( pResponseAsdu, (uint8_t*)pDisplayMessage, m_DISPLAY_MESSAGE_FIXED_LENGTH_c );
    pResponseAsdu += m_DISPLAY_MESSAGE_FIXED_LENGTH_c;
    *pResponseAsdu++ =  pDisplayMessage->aDisplayMessage[0];
    memUtils_memCopy (  pResponseAsdu, &pDisplayMessage->aDisplayMessage[1],
                pDisplayMessage->aDisplayMessage[0] );

    return pDisplayMessage->aDisplayMessage[0] + g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c +
           m_DISPLAY_MESSAGE_FIXED_LENGTH_c + 1 ;
}
#endif              /* ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 ) */
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
#endif                                /* g_MESSAGING_CLUSTER_ENABLE_d */
