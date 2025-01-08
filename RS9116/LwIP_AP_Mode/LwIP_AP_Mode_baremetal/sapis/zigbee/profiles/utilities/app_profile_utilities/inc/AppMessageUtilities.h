/******************************************************************************
* AppMessageUtilities..h
******************************************************************************
* Program Description:
* This file contains the Message cluster App type definations
******************************************************************************/
#ifndef _APP_MESSAGE_UTILITIES_H_
#define _APP_MESSAGE_UTILITIES_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Message_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_MESSAGING_INVALID_MESSAGE_ID_c                      0xFFFFFFFF
#define g_MESSAGING_INVALID_MESSAGE_CONTROL_c                 0x00
#define g_MESSAGING_INVALID_START_TIME_c                      0xFFFFFFFF
#define g_MESSAGING_INVALID_DURATION_c                        0x0000
#define g_SPECIAL_DURATION_UNTIL_CHANGED_c                    0xFFFF
#define g_MESSAGE_DISPLAY_TIME_START_NOW_c                    0x00000000

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct MsgConfirmTxDetailes_Tag
{
    uint8_t msgTypeConfReq;
    uint8_t InterPANMode;
    uint8_t dstaddrmode;
    uint8_t destaddress[8];
    uint16_t dstInterPANId;
    uint8_t destEndPoint;
    uint8_t sequenceNumber;
}__attribute__((__packed__))MsgConfirmTxDetailes_t;

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
void App_MessageEventsHandler( ZCL_Event_t *pZCLEvent,
                               APSDE_Data_Indication_t *pBuffer );

#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void App_InterPanMessageEventsHandler( ZCL_Event_t *pZCLEvent,
                                   APSDE_InterPan_Data_Indication_t *pBuffer );
#endif
void App_MessagingTick ( void );

void App_MessageTableInit( void );

void App_UpdateMessageTable( DisplayMessage_t *pDisplayMsg, uint8_t serverTable );
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
DisplayMessage_t* App_GetMessageTablePtr(uint8_t serverTable);
#endif

DisplayMessage_t* App_GetMessageTablePtr(uint8_t serverTable);
#endif

uint8_t App_AddEntryToMessageTable(DisplayMessage_t *pDisplayMsg);
uint8_t App_DeleteMessage(void);
uint32_t App_GetMessageID(void);

#endif          /* _APP_MESSAGE_UTILITIES_H_ */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

