 /******************************************************************************
* AppPollControlUtilities..h
******************************************************************************
* Program Description:
* This file contains the pOLL Control Cluster App type definitions
******************************************************************************/
#ifndef _APP_POLL_CONTROL_UTILITIES_H_
#define _APP_POLL_CONTROL_UTILITIES_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_General_Poll_Control_Cluster.h"

/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Global Constants
******************************************************************************/


/******************************************************************************
* Memory Declarations
******************************************************************************/
/* Set Long Poll INterval Command Payload */
typedef struct LongPollInterval_Tag{
    uint8_t newLongPollInterval[4];
}__attribute__((__packed__))LongPollInterval_t;

/* Set Short Poll Interval Command Payload */
typedef struct ShortPollInterval_Tag{
    uint8_t newShortPollInterval[2];
}__attribute__((__packed__))ShortPollInterval_t;

/* Check in response payload*/
typedef struct CheckInResponsePayload_Tag{
    /* boolean value :
    g_TRUE_c indicates start fast polling,
    g_FALSE_c indicates stop fast polling*/
    uint8_t startFastPolling;
    /* fast poll timeout period*/
    uint8_t fastPollTimeOut[2];
}__attribute__((__packed__))CheckInResponsePayload_t;

/******************************************************************************
* Type Definitions
******************************************************************************/

/******************************************************************************
   Function Prototypes
******************************************************************************/

void App_PollControlEventHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer );

#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
void App_UpdateUserSetValue(bool startFastPoll,uint16_t fastPollTimeout);
#endif

#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
void CheckTheCheckIntervalAndSendCheckInCommand(void);

#if ( g_ZIGBEE_END_DEVICE_d == 1 )
extern void TH_setPollRate (uint16_t poll_time);
#endif                                      /* ( g_ZIGBEE_END_DEVICE_d == 1 ) */

void SetTheMACPollRateAccordingly(void);
#endif

/******************************************************************************
* End Of File
******************************************************************************/
#endif          /* _APP_POLL_CONTROL_UTILITIES_H_ */
