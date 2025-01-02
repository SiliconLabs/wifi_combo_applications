/******************************************************************************
* ZCL_General_Appliance_Control_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Appliance Conrol attribute and command
* information
******************************************************************************/

#ifndef _ZCL_GENERAL_APPLIANCE_CONTROL_CLUSTER_H_
#define _ZCL_GENERAL_APPLIANCE_CONTROL_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/******************************************************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Appliance Control Cluster */
#define g_APPLIANCE_CONTROL_CLUSTER_c                           0x001b

/* Appliance Control Attribute information */

///* Appliance Status Attibute Set */
//    /* Determines current status of appliance*/
//#define g_APPLIANCE_STATUS_ATTRIBUTE_c                          0x0000
//    /* appliance state correlated to remote device */
//#define g_REMOTE_ENABLE_FLAGS_ATTRIBUTE_c                       0x0001
//
//#define g_APPLIANCE_STATUS_2_ATTRIBUTE_c                        0x0002
//

/* Appliance Funtions Attibute Set */
        /* Determines start time of the machine activity*/
#define g_START_TIME_ATTRIBUTE_c                                0x0000
        /* Determines Finish time of the machine activity*/
#define g_FINISH_TIME_ATTRIBUTE_c                               0x0001
        /* Time remaining for finishing the machine cycle*/
#define g_REMAINING_TIME_ATTRIBUTE_c                            0x0002




/* enum for Appliance Control Cluster Client Commands */
enum applianceControlClientCommands {
    g_ExecutionOfACommand_c  = 0x00,
    g_SignalStateCommand_c,
    g_WriteFunctionCommad_c,
    g_OverloadPauseResumeCommand_c,
    g_OverloadPauseCommand_c,
    g_OverloadWarningCommand_c
};

/* enum for Appliance Control Cluster Server Commands */
enum applianceControlServerCommands {
    g_SignalStateResponseCommand_c  = 0x00,
    g_SignalStateNotificationCommand_c
};

/* enum for Appliance Control Cluster Events Identifiers*/
enum applianceControlCluserEvents {
    g_ApplianceControlExecutionEvent_c = 0xb0,
    g_ApplianceControlSignalStateEvent_c,
    g_ApplianceControlWriteFunctionEvent_c,
    g_ApplianceControlOverloadPauseResumeEvent_c,
    g_ApplianceControlOverloadPauseEvent_c,
    g_ApplianceControlOverloadWarningEvent_c,
    g_ApplianceControlStateResponseEvent_c,
    g_ApplianceControlStateNotificationEvent_c
};


/* Details of Execution of Command */
typedef struct ExecutionOfACommandPayload_Tag {
    uint8_t commandIdentification;
}__attribute__((__packed__))ExecutionOfACommandPayload_t;

/* Details of Write Function Command */
typedef struct WriteFunctionCommandPayload_Tag {
    uint8_t functionIdentifier[2];
    uint8_t functionDatatype;
    /* function data is variable in length ,dependent on functiondatatype*/
    uint8_t functionData[1];
}__attribute__((__packed__))WriteFunctionCommandPayload_t;


/* Details of Overload Warning Command */
typedef struct OverloadWarningCommandPayload_Tag {
    uint8_t warningEvent;
}__attribute__((__packed__))OverloadWarningCommandPayload_t;

/* Details of Signal State Response Command */
typedef struct SignalStateResponseCommandPayload_Tag {
    uint8_t applianceStatus;
    uint8_t remoteEnableFlagsAndDeviceStatus2;
    /* appliance Status 2 is optional */
    uint8_t applianceStatus2[3];
}__attribute__((__packed__))SignalStateResponseCommandPayload_t;

/* Details of Signal State Notification Command */
typedef struct SignalStateNotificationCommandPayload_Tag {
    uint8_t applianceStatus;
    uint8_t remoteEnableFlagsAndDeviceStatus2;
    /* appliance Status is optional */
    uint8_t applianceStatus2[3];
}__attribute__((__packed__))SignalStateNotificationCommandPayload_t;




/******************************************************************************
* Memory Declarations
******************************************************************************/
#if ( g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Appliance_Control_Server_Cluster_Info;
#endif

#if ( g_EN50523_APPLIANCE_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Appliance_Control_Client_Cluster_Info;
#endif


/******************************************************************************
* Type Definitions
******************************************************************************/
/* Appliance Control Cluster Attributes structure */
typedef struct Appliance_Control_Cluster_Tag
{
    uint8_t startTime[2];
    uint8_t finishTime[2];
    uint8_t remainingTime[2];
} __attribute__((__packed__))Appliance_Control_Cluster_t;

/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_HA_Appliance_Control_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                          /*_ZCL_GENERAL_APPLIANCE_CONTROL_CLUSTER_H_*/
