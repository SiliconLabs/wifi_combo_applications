/******************************************************************************
* AppApplianceStatisticUtilities..h
******************************************************************************
* Program Description:
* This file contains the Appliance Statistic Cluster App type definations
******************************************************************************/
#ifndef _APP_APPLIANCE_STATISTIC_UTILITIES_H_
#define _APP_APPLIANCE_STATISTIC_UTILITIES_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_HA_Appliance_Statistics_Cluster.h"

/******************************************************************************
* Global Constants
******************************************************************************/

/******************************************************************************
* Memory Declarations
******************************************************************************/
/* This struct is used as an template for storing default log entries ,which is
actually application dependent.*/
typedef struct DefaultLogResponse_Tag {
    uint8_t defaultTimeStamp[4]; /* UTC time*/
    uint8_t defaultLogId[4];
    uint8_t defaultLogLength[4];
    uint8_t defaultLogPayload[1];
}__attribute__((__packed__))DefaultLogResponse_t;


/******************************************************************************
* Type Definitions
******************************************************************************/

/******************************************************************************
   Function Prototypes
******************************************************************************/

void App_ApplianceStatisticEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer );
void InvokeLogNotificationCommand(void);
void InvokeStatisticsAvailableCommand(void);
void App_ApplianceStatisticReset(void);
/******************************************************************************
* End Of File
******************************************************************************/
#endif          /* _APP_APPLIANCE_STATISTIC_UTILITIES_H_ */
