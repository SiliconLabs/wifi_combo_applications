/******************************************************************************
* AppCommissioningUtilities..h
******************************************************************************
* Program Description:
* This file contains the Message cluster App type definations
******************************************************************************/
#ifndef _APP_COMMISSIONING_UTILITIES_H_
#define _APP_COMMISSIONING_UTILITIES_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Commissioning_Cluster.h"

/******************************************************************************
* Global Constants
******************************************************************************/
#define g_EXTENDED_PANID_LENGTH_c                      (0x08)
#define g_SILENT_START_NETWORK_c                       (0x00)
#define g_FORM_NETWORK_c                               (0x01)
#define g_REJOIN_NETWORK_c                             (0x02)
#define g_ASSOC_JOIN_NETWORK_c                         (0x03)

/******************************************************************************
* Memory Declarations
******************************************************************************/


/******************************************************************************
* Type Definitions
******************************************************************************/
typedef enum RestartCmdReturnValue_Tag
{
    g_SUCCESS_ZCL_c,
    g_INCONSISTENT_STARTUP_STATE_c,
    g_ZCL_INSUFFICIENTSPACE_c,
    g_INVALID_FIELD_c
}RestartCmdReturnValue_t;

/******************************************************************************
   Function Prototypes
******************************************************************************/
void App_CommissioningEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer );

RestartCmdReturnValue_t validateSAS();
void SendCommissioningClusterResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status);
void RestoreCurrentStartUpAttribute();
/******************************************************************************
* End Of File
******************************************************************************/
#endif          /* _APP_COMMISSIONING_UTILITIES_H_ */