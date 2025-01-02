/******************************************************************************
* ZCL_General_Poll_Control_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Poll Control attribute and command
* information
******************************************************************************/

#ifndef _ZCL_GENERAL_POLL_CONTROL_CLUSTER_H_
#define _ZCL_GENERAL_POLL_CONTROL_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/******************************************************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Poll Control Cluster */
#define g_POLL_CONTROL_CLUSTER_c                                0x0020


/* Attribute Id  */
/* Server Attributes*/
/*The Poll Control server is responsible for checking in with the poll
control client periodically to see if the poll control client wants to
modify the poll rate of the poll control server*/
#define g_CHECK_IN_INTERVAL_ATTRIBUTE_ID_c                      0x0000

/* maximum amount of time in quarterseconds between MAC Data Polls from the
end device to its parent*/
#define g_LONG_POLL_INTERVAL_ATTRIBUTE_ID_c                     0x0001

/* The Short Poll Interval represents the number of quarterseconds that an
end device waits between MAC Data Polls to its parent*/
#define g_SHORT_POLL_INTERVAL_ATTRIBUTE_ID_c                    0x0002

/*The Fast Poll Timeout attribute represents the number of quarterseconds
that an end device will stay in fast poll mode by default.*/
#define g_FAST_POLL_TIMEOUT_ATTRIBUTE_ID_c                      0x0003

/* minimum value ,used in validation*/
#define g_CHECK_IN_INTERVAL_MIN_ATTRIBUTE_ID_c                  0x0004

/* minimum value ,used in validation*/
#define g_LONG_POLL_INTERVAL_MIN_ATTRIBUTE_ID_c                 0x0005

/* max value ,used in validation*/
#define g_FAST_POLL_TIMEOUT_MAX_ATTRIBUTE_ID_c                  0x0006

/* Commands received by the server */

enum pollControlClientCommands
{
    g_CheckinResponseCommand_c = 0x00,
    g_FastPollStopCommand_c,
    g_SetLongPollIntervalCommand_c,
    g_SetShortPollIntervalCommand_c
};

/* Commands received by the Client */
enum pollControlServerCommands
{
    g_CheckinCommand_c = 0x00
};

/* enum for Poll Control cluster event identifier */
enum pollControlClusterEvents
{
    g_CheckinResponseEvent_c = 0x70,
    g_FastPollStopEvent_c,
    g_SetLongPollIntervalEvent_c,
    g_SetShortPollIntervalEvent_c,
    g_CheckinEvent_c

};
#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
typedef struct PollControlCluster_Tag{
#if(g_CHECK_IN_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
    uint8_t checkInInterval[4];
#endif

#if(g_LONG_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
    uint8_t longPollInterval[4];
#endif
#if(g_SHORT_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
    uint8_t shortPollInterval[2];
#endif
#if(g_FAST_POLL_TIMEOUT_ATTRIBUTE_ENABLE_c == 1 )
    uint8_t fastPollTimeout[2];
#endif
#if(g_CHECK_IN_INTERVAL_MIN_ATTRIBUTE_ENABLE_c == 1)
    uint8_t checkInIntervalMin[4];
#endif
#if(g_LONG_POLL_INTERVAL_MIN_ATTRIBUTE_ENABLE_c == 1)
    uint8_t longPollIntervalMin[4];
#endif
#if( g_FAST_POLL_TIMEOUT_MAX_ATTRIBUTE_ENABLE_c == 1)
    uint8_t fastPollTimeoutMax[2];
#endif
}__attribute__((__packed__))PollControlCluster_t;

#endif

/******************************************************************************
* Memory Declarations
******************************************************************************/
#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HA_Poll_Control_Server_Cluster_Info;
#endif

#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Poll_Control_Client_Cluster_Info;
#endif




/******************************************************************************
* Type Definitions
******************************************************************************/
/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_HA_Poll_Control_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                          /*_ZCL_GENERAL_POLL_CONTROL_CLUSTER_H_*/
