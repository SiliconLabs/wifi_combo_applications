/******************************************************************************
* ZCL_Message_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Message cluster attribute and cmmnd information
******************************************************************************/

#ifndef _ZCL_MESSAGE_CLUSTER_H_
#define _ZCL_MESSAGE_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Functionality.h"
#include "ZCL_Interface.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_MESSAGING_CLUSTER_c                                   0x0703

/* Message Control Fields */
#define g_NORMAL_TRANSMISSION_ONLY_c                            0x00
#define g_NORMAL_AND_ANONYMOUS_INTERPAN_c                       0x01
#define g_ANONYMOUS_INTERPAN_ONLY_c                             0x02
/* Level of Importance of the Message in Message Control Field */
#define g_LOW_LEVEL_IMPORTANCE_c                                0x00
#define g_MEDIUM_LEVEL_IMPORTANCE_c                             0x04
#define g_HIGH_LEVEL_IMPORTANCE_c                               0x08
#define g_CRITICAL_LEVEL_IMPORTANCE_c                           0x0C
/* Message Confirmation Bits in Message Control Field */
#define g_MESSAGE_CONFIRMATION_NOT_REQUIRED_c                   0x00
#define g_MESSAGE_CONFIRMATION_REQUIRED_c                       0x80

enum
{
   g_Display_Message_Events_c = 0x90,
   g_Cancel_Message_Events_c,
   g_GetLast_Message_Events_c,
   g_Message_Confirmation_Events_c
};

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_message_Server_Cluster_Info;
#endif

#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_message_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
/* enumerations for commands specific for general clusters */
typedef enum Message_Cluster_Server
{
   g_Display_Message_Command_c,
   g_Cancel_Message_Command_c
}Message_Cluster_Server;


typedef enum Message_Cluster_Client
{
   g_GetLast_Message_Command_c,
   g_Message_Confirmation_Command_c
}Message_Cluster_Client;


typedef struct MessageConfirmationCmdPayload_Tag
{
    uint8_t messageId[4];
    uint8_t  messageControl;
}__attribute__((__packed__))MessageConfirmationCmdPayload_t;

typedef struct CancelMessageCmdPayload_Tag
{
    uint8_t messageId[4];
    uint8_t cancelControl;
}__attribute__((__packed__))CancelMessageCmdPayload_t;

typedef struct DisplayMessage_Tag
{
    uint8_t messageId[4];
    uint8_t messageControl;
    uint8_t startTime[4];
    uint8_t duration[2];
    uint8_t aDisplayMessage[80];
}__attribute__((__packed__))DisplayMessage_t;


/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/
uint8_t zclCreateDisplayMessageCommand ( uint8_t *pResponseAsdu,
               DisplayMessage_t* pDisplayMessage, uint8_t transactionSeqNum );

uint8_t zclCreateMessageConfirmation ( uint8_t *pResponseAsdu,
                             uint8_t transactionSeqNum, uint8_t* pMessageId );
/*-----------------------------------------------------------------------------
* ZCL_Handle_Message_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : Length
*		
* Input Parameters:
*	endPointId - Device End point
*   asduLength - Length of received asdu
*   pReceivedAsdu - Pointer to received asdu
*   pZCLClusterInfo - Pointer to Level control cluster info
*   pAttrsDataBaseLocation - Pointer to Basic cluster attributes storage
*
* Output Parameters:
*	pResponseAsduStartLocation - Pointer for response creation
*   pActionHandler - Pointer for Event Updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the message cluster command received in the indication
*     	
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Message_Cluster_Cmd ( uint8_t endPointId,
                                         uint8_t asduLength,
                                         uint8_t* pReceivedAsdu,
                                         uint8_t* pResponseAsduStartLocation,
                                         ZCL_ActionHandler_t *pActionHandler,
                                         ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                         uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                      /* g_MESSAGING_CLUSTER_ENABLE_d == 1 */

#endif                                      /*_ZCL_MESSAGE_CLUSTER_H_*/
