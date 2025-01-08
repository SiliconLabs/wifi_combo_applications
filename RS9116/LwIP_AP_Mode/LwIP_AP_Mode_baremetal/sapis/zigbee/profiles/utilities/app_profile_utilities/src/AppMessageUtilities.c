/*******************************************************************************
* AppMessageUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting meassage cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Common.h"
#include "stack_common.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "memory_utility.h"
#include "zdo_interface.h"
#include "ApplicationThinLayer.h"
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
#include "AppMessageUtilities.h"
#include "ZCL_Message_Cluster.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_DISPLAY_MESSAGE_CMD_FIXED_LENGTH_c                0x0B
#define m_SART_TIME_LENGTH_c                                0x04
#define m_MESSAGE_ID_LENGTH_c                               0x04
#define m_MESSAGE_DURATION_c                                0x02
#define m_DISPLAY_MSG_CONFIRMATION_c                        0x02
#define m_CANCEL_MSG_CONFIRMATION_c                         0x03
#define m_MASK_FOR_MSG_TYPE_c                               0x01
#define m_MASK_FOR_MSG_CONFIRM_c                            0x02
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
static DisplayMessage_t serverDisplayMsgTable;
#endif

#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
static DisplayMessage_t clientDisplayMsgTable;
static MsgConfirmTxDetailes_t msgConfirmTxDetails = { g_FALSE_c };
#if (g_CLI_SUPPORT_d == 1)
uint8_t userConfirmed = g_FALSE_c;
#else
uint8_t userConfirmed = g_TRUE_c;
#endif
#endif

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

static void App_MsgInit( DisplayMessage_t *pMsgTable  );

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

void App_MessageTableInit( void )
{
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
    App_MsgInit( &serverDisplayMsgTable );
#endif

#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
    App_MsgInit( &clientDisplayMsgTable );
#endif
}

/**************************************************************************************/
static void App_MsgInit( DisplayMessage_t *pMsgTable  )
{
    memUtils_memSet( pMsgTable->startTime, g_ZCL_INVALID_DATA_c, m_SART_TIME_LENGTH_c );
    memUtils_memSet( pMsgTable->messageId, g_ZCL_INVALID_DATA_c, m_MESSAGE_ID_LENGTH_c);
    memUtils_memSet( pMsgTable->duration, g_ZERO_c, m_MESSAGE_DURATION_c );
    pMsgTable->messageControl = g_RESET_c;
}
/**************************************************************************************/

#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
void App_UpdateMessageTable( DisplayMessage_t *pDisplayMsg, uint8_t serverTable )
{
    DisplayMessage_t *pMessageTable;

    if ( serverTable == g_FALSE_c ) {
#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
        pMessageTable = &clientDisplayMsgTable;
#else
        return;
#endif
    }
    else {
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
         pMessageTable = &serverDisplayMsgTable;
#else
         return;
#endif
    }
    memUtils_memCopy ((uint8_t*)pMessageTable,
              (uint8_t*)pDisplayMsg, m_DISPLAY_MESSAGE_CMD_FIXED_LENGTH_c );

    if( ! ( *((uint32_t*)pDisplayMsg->startTime ) ) ) {
        *((uint32_t*)pMessageTable->startTime) = ZCL_CallBackGetCurrentTime();
    }
    memUtils_memCopy ( pMessageTable->aDisplayMessage,
               pDisplayMsg->aDisplayMessage, pDisplayMsg->aDisplayMessage[0] + 1 );
}
#endif
/**************************************************************************************/

DisplayMessage_t* App_GetMessageTablePtr(uint8_t serverTable)
{
    if ( serverTable == g_TRUE_c ) {
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
        return &serverDisplayMsgTable;
#endif
    }
    else {
#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
        return &clientDisplayMsgTable;
#endif
    }
    return g_NULL_c;
}

/**************************************************************************************/
void App_MessageEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{

#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )

    if(g_GetLast_Message_Events_c == pZCLEvent->eventId) {
        uint8_t asduLength;
        if( *(( uint32_t *)serverDisplayMsgTable.startTime) != g_MESSAGING_INVALID_START_TIME_c ) {

            asduLength = zclCreateDisplayMessageCommand ( pBuffer->a_asdu,
                                                          &serverDisplayMsgTable,
                                                          pBuffer->a_asdu[1] );

            App_SendData ( pBuffer->src_addr_mode,
                             (uint8_t*)&pBuffer->src_address.short_address,
                             pBuffer->src_endpoint,
                             g_MESSAGING_CLUSTER_c,
                             asduLength,
                             pBuffer->a_asdu );
        }
        else {
            App_SendDefaultResponse( g_ZCL_Not_Found_c, g_FALSE_c, pBuffer );
        }
    }

    else if(g_Message_Confirmation_Events_c == pZCLEvent->eventId){
        /* Application need to handle*/

    }

#endif

#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )

    if(g_Display_Message_Events_c == pZCLEvent->eventId )
    {
        DisplayMessage_t *pDisplayMessge = (DisplayMessage_t*)pZCLEvent->pEventData;

        if (*((uint32_t*)clientDisplayMsgTable.messageId) ==
                                     *((uint32_t*)pDisplayMessge->messageId) ) {
            App_SendDefaultResponse( g_ZCL_Duplicate_Exists_c,
                                     g_FALSE_c, pBuffer );
            return;
        }
        msgConfirmTxDetails.dstaddrmode = pBuffer->src_addr_mode;
        msgConfirmTxDetails.InterPANMode = g_FALSE_c;
        memUtils_memCopy( msgConfirmTxDetails.destaddress, pBuffer->src_address.IEEE_address, 0x08 );
        msgConfirmTxDetails.destEndPoint = pBuffer->src_endpoint;
        msgConfirmTxDetails.sequenceNumber = pBuffer->a_asdu[1];

        /* Check if Message confirmation required and update the table */
        if( pDisplayMessge->messageControl & g_MESSAGE_CONFIRMATION_REQUIRED_c ) {
            msgConfirmTxDetails.msgTypeConfReq = m_DISPLAY_MSG_CONFIRMATION_c;
#if (g_CLI_SUPPORT_d == 0)
            userConfirmed = g_TRUE_c;
#endif //for test harness
        }
        App_UpdateMessageTable( pDisplayMessge, g_FALSE_c );
    }

    else if(g_Cancel_Message_Events_c == pZCLEvent->eventId )
    {
        CancelMessageCmdPayload_t*pCancelMessge = (CancelMessageCmdPayload_t*)pZCLEvent->pEventData;

        if ( *((uint32_t*)clientDisplayMsgTable.messageId ) == *((uint32_t*)pZCLEvent->pEventData)) {
            if( pCancelMessge->cancelControl & g_MESSAGE_CONFIRMATION_REQUIRED_c ) {
                msgConfirmTxDetails.msgTypeConfReq = m_CANCEL_MSG_CONFIRMATION_c;
            }
            else {
                msgConfirmTxDetails.msgTypeConfReq = g_FALSE_c;
                App_MsgInit( &clientDisplayMsgTable );
            }
        }
    }


#endif

}

/**************************************************************************************/
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void App_InterPanMessageEventsHandler( ZCL_Event_t *pZCLEvent,
                                       APSDE_InterPan_Data_Indication_t *pBuffer )
{


    switch ( pZCLEvent->eventId )
    {
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
    case g_GetLast_Message_Events_c:
    {
        uint16_t tempClusterId = 0xffff;
        uint8_t *ptr = g_NULL_c;
        uint8_t asduLength = 0;
		uint8_t asdu[10];


        if( *(( uint32_t *)serverDisplayMsgTable.startTime) != g_MESSAGING_INVALID_START_TIME_c ) {

            asduLength = zclCreateDisplayMessageCommand ( pBuffer->a_asdu,
                                                          &serverDisplayMsgTable,
                                                          pBuffer->a_asdu[1] );


            tempClusterId = g_MESSAGING_CLUSTER_c;
            ptr = pBuffer->a_asdu ;
        }
        else {

            asduLength = ZCL_CreateDefaultResponseCommand ( pBuffer->a_asdu,
                                                            asdu,
                                                            g_ZCL_Not_Found_c );


            tempClusterId = pBuffer->clusterId;
            ptr = asdu;

        }
        App_SendInterPanData( pBuffer->srcaddrmode,
                          pBuffer->srcaddress.IEEE_address,
                          pBuffer->srcpanid,
                          tempClusterId,
                          asduLength,
                          ptr );

    }
    break;

    case g_Message_Confirmation_Events_c:
        break;
#endif

#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
    case g_Display_Message_Events_c:
    {
        DisplayMessage_t *pDisplayMessge = (DisplayMessage_t*)pZCLEvent->pEventData;

        msgConfirmTxDetails.dstaddrmode = pBuffer->srcaddrmode;
        msgConfirmTxDetails.InterPANMode = g_TRUE_c;
        msgConfirmTxDetails.dstInterPANId = pBuffer->srcpanid;
        memUtils_memCopy( msgConfirmTxDetails.destaddress, pBuffer->srcaddress.IEEE_address, 0x08 );
        msgConfirmTxDetails.sequenceNumber = pBuffer->a_asdu[1];

        /* Check if Message confirmation required and update the table */
        if( pDisplayMessge->messageControl & g_MESSAGE_CONFIRMATION_REQUIRED_c ) {
            msgConfirmTxDetails.msgTypeConfReq = m_DISPLAY_MSG_CONFIRMATION_c;
        }
        App_UpdateMessageTable( pDisplayMessge, g_FALSE_c );
    }
    break;
    case g_Cancel_Message_Events_c :
    {
        CancelMessageCmdPayload_t*pCancelMessge = (CancelMessageCmdPayload_t*)pZCLEvent->pEventData;
        msgConfirmTxDetails.InterPANMode = g_TRUE_c;
        if ( *((uint32_t*)clientDisplayMsgTable.messageId ) == *((uint32_t*)pZCLEvent->pEventData)) {
            if( pCancelMessge->cancelControl & g_MESSAGE_CONFIRMATION_REQUIRED_c ) {
                msgConfirmTxDetails.msgTypeConfReq = m_CANCEL_MSG_CONFIRMATION_c;
            }
            else {
                msgConfirmTxDetails.msgTypeConfReq = g_FALSE_c;
                App_MsgInit( &clientDisplayMsgTable );
            }
        }
    }
    break;

#endif
    default :
        break;
    }
}
#endif
/**************************************************************************************/
static void AppCheckMsgValidity( DisplayMessage_t *pMsgTable )
{
    if (*((uint32_t*)pMsgTable->startTime) != g_MESSAGING_INVALID_START_TIME_c ) {
        if ( *( ( uint16_t *)pMsgTable->duration ) !=  g_SPECIAL_DURATION_UNTIL_CHANGED_c ) {
            if ( ZCL_CallBackGetCurrentTime() > (*((uint32_t*)pMsgTable->startTime ) +
                                                  *((uint16_t*)pMsgTable->duration ) * 60 ) ) {
                App_MsgInit( pMsgTable );
#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
                msgConfirmTxDetails.msgTypeConfReq = g_FALSE_c;
                userConfirmed = g_FALSE_c;
#endif
            }
        }
    }
}

/**************************************************************************************/
void App_MessagingTick ( void )
{
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
    if( ( msgConfirmTxDetails.msgTypeConfReq & m_MASK_FOR_MSG_CONFIRM_c ) &&
        ( userConfirmed == g_TRUE_c ) ) {
        uint8_t asduLength;
        uint8_t asdu[0x10];

        if( *(( uint32_t *)&clientDisplayMsgTable.startTime)!= g_MESSAGING_INVALID_START_TIME_c ) {

            asduLength = zclCreateMessageConfirmation ( asdu,
                                                       msgConfirmTxDetails.sequenceNumber,
                                                       clientDisplayMsgTable.messageId );

            if ( msgConfirmTxDetails.InterPANMode == g_FALSE_c ) {
                App_SendData( msgConfirmTxDetails.dstaddrmode,
                              msgConfirmTxDetails.destaddress,
                              msgConfirmTxDetails.destEndPoint,
                              g_MESSAGING_CLUSTER_c,
                              asduLength,
                              asdu );
            }
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
            else {
                App_SendInterPanData( msgConfirmTxDetails.dstaddrmode,
                      msgConfirmTxDetails.destaddress,
                      msgConfirmTxDetails.dstInterPANId,
                      g_MESSAGING_CLUSTER_c,
                      asduLength,
                      asdu );
            }
#endif
            if ( msgConfirmTxDetails.msgTypeConfReq & m_MASK_FOR_MSG_TYPE_c ) {
                App_MsgInit( &clientDisplayMsgTable );
            }
            msgConfirmTxDetails.msgTypeConfReq = g_FALSE_c;
#if (g_CLI_SUPPORT_d == 1)
            userConfirmed = g_FALSE_c;
#endif
        }
    }
    AppCheckMsgValidity( &clientDisplayMsgTable );
#endif

#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
    AppCheckMsgValidity( &serverDisplayMsgTable );
#endif
#endif

}
/**************************************************************************************/
uint8_t App_AddEntryToMessageTable(DisplayMessage_t *pDisplayMsg)
{
	DisplayMessage_t dispMsg;
	uint8_t status = g_ZCL_Failure_c;
	bool type;
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
	type = g_TRUE_c;
#else
	type = g_FALSE_c;
#endif
	if (pDisplayMsg!= g_NULL_c)
	{
		memUtils_memCopy( (uint8_t *)&dispMsg, (uint8_t *)pDisplayMsg, sizeof(DisplayMessage_t ));

		App_UpdateMessageTable( &dispMsg, type);
		status = g_ZCL_Success_c;

	}
	return status;
}
/**************************************************************************************/
uint32_t App_GetMessageID(void)
{

	DisplayMessage_t *dispMsg = g_NULL_c;
	uint32_t msgID = g_MESSAGING_INVALID_MESSAGE_ID_c;
	bool type;
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
	type = g_TRUE_c;
#else
	type = g_FALSE_c;
#endif
	dispMsg = App_GetMessageTablePtr(type);
	if(dispMsg != g_NULL_c)
	{
		memUtils_memCopy( (uint8_t *)&msgID, dispMsg->messageId, sizeof(uint32_t));
	}

	return msgID;

}
/**************************************************************************************/
uint8_t App_DeleteMessage(void)
{

	DisplayMessage_t *dispMsg = g_NULL_c;
	uint8_t status = g_ZCL_Failure_c;
	bool type;

#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
	type = g_TRUE_c;
#else
	type = g_FALSE_c;
#endif
	dispMsg = App_GetMessageTablePtr(type);
	if(dispMsg != g_NULL_c)
	{
		App_MsgInit(dispMsg);
		status = g_ZCL_Success_c;
	}
	return status;
}

/**************************************************************************************/
/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

#endif                      /* ( g_MESSAGING_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
