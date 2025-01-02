/******************************************************************************
* ZCL_HA_Appliance_Statistics_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Appliance Identificatio attribute and command
* information
******************************************************************************/

#ifndef _ZCL_HA_APPLIANCE_STATISTICS_CLUSTER_H_
#define _ZCL_HA_APPLIANCE_STATISTICS_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/******************************************************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Appliance Statistics Cluster */
#define g_APPLIANCE_STATISTICS_CLUSTER_c                        0x0B03

/* Appliance Statistics Attribute information */

/* Attibute Set Zero*/
        /* log size of the appliance log data */
#define g_LOG_MAX_SIZE_c                                        0x0000
        /* Maximum logs in queue */
#define g_LOG_QUEUE_MAX_SIZE_c                                  0x0001


/* enum for Appliance Statistics Cluster Server Commands */
enum applianceStatisticsServerCommands {
    g_LogNotificationCommand_c  = 0x00,
    g_LogResponseCommand_c,
    g_LogQueueResponseCommad_c,
    g_StatisticsAvailableCommand_c
};

/* enum for Appliance Statistics Cluster Client Commands */
enum applianceStatisticsClientCommands {
    g_LogRequest_c  = 0x00,
    g_LogQueueRequest_c
};

/* enum for Appliance Statistics Cluster Events Identifiers*/
enum applianceStatisticsCluserEvents {
    g_ApplianceStatisticsLogNotificationEvent_c = 0xa0,
    g_ApplianceStatisticsLogResponseEvent_c,
    g_ApplianceStatisticsLogQueueEvent_c,
    g_ApplianceStatisticsStatisticsAvailableEvent_c,
    g_ApplianceStatisticsLogRequestEvent_c,
    g_ApplianceStatisticsLogQueueRequestEvent_c

};

/* Details of Log Notification Command Payload */
typedef struct LogNotificationCommandPayload_Tag {
    uint8_t timeStamp[4]; /* UTC time*/
    uint8_t logId[4];
    uint8_t logLength[4];
    uint8_t logPayload[1]; /* variable payload length*/
}__attribute__((__packed__))LogNotificationCommandPayload_t;

/* Details of Log Response Command Payload */
typedef struct LogResponseCommandPayload_Tag {
    uint8_t timeStamp[4]; /* UTC time*/
    uint8_t logId[4];
    uint8_t logLength[4];
    uint8_t logPayload[1]; /*Note: variable payload length ,for our reference
                                declared size as 10 */
}__attribute__((__packed__))LogResponseCommandPayload_t;


/* Details of Log Queuse Response Command Payload */
typedef struct LogQueueResponseCommandPayload_Tag {
    uint8_t logQueueSize;
    uint8_t logId[1]; /* value of log Id depends on how many log Id's
    mentioned in Queue Size, Note : there could be no log Id's also. */
}__attribute__((__packed__))LogQueueResponseCommandPayload_t;


/* Details of Statistics Available Notification PAYLOAD */
typedef struct StatisticsAvailableNotificationCommandPayload_Tag {
    uint8_t logQueueSize;
    uint8_t logId[1]; /* value of log Id depends on how many log Id's
    mentioned in Queue Size, Note : there could be no log Id's also. */
}__attribute__((__packed__))StatisticsAvailableNotificationCommandPayload_t;

/* Details of Log Request command payload */
typedef struct LogRequestPayload_Tag {
    uint8_t logId[4];
}__attribute__((__packed__))LogRequestPayload_t;






/******************************************************************************
* Memory Declarations
******************************************************************************/
#if ( g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Appliance_Statistics_Server_Cluster_Info;
#endif

#if ( g_APPLIANCE_STATISTICS_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Appliance_Statistics_Client_Cluster_Info;
#endif


/******************************************************************************
* Type Definitions
******************************************************************************/
/* Appliance Statistics Cluster Attributes structure */
typedef struct Appliance_Statistics_Cluster_Tag
{
    uint8_t logMaxSize[4];
    uint8_t logQueueMaxSize;
} __attribute__((__packed__))Appliance_Statistics_Cluster_t;

/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_HA_Appliance_Statistics_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                          /*_ZCL_HA_APPLIANCE_STATISTICS_CLUSTER_H_*/
