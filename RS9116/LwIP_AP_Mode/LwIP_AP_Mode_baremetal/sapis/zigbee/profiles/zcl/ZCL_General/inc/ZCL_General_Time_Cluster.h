#ifndef _ZCL_GENERAL_TIME_CLUSTER_H_
#define _ZCL_GENERAL_TIME_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"

#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* Attributes and commands that provide a basic interface
   to a real-time clock */
#define g_TIME_CLUSTER_c                                    0x000a

/*Attributes for Time Cluster*/
#define g_TIME_ATTRIBUTE_c                                  0x0000
#define g_TIME_STATUE_ATTRIBUTE_c                           0x0001
#define g_TIME_ZONE_ATTRIBUTE_c                             0x0002
#define g_TIME_DST_START_ATTRIBUTE_c                        0x0003
#define g_TIME_DST_END_ATTRIBUTE_c                          0x0004
#define g_TIME_DST_SHIFT_ATTRIBUTE_c                        0x0005
#define g_TIME_STANDARD_TIME_ATTRIBUTE_c                    0x0006
#define g_TIME_LOCAL_TIME_ATTRIBUTE_c                       0x0007
#define g_TIME_LASTSET_TIME_ATTRIBUTE_c                     0x0008
#define g_TIME_VALID_UNTIL_TIME_ATTRIBUTE_c                 0x0009

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )

extern const ZCL_Cluster_Info_t ga_Time_Server_Cluster_Info;

#endif

#if ( g_TIME_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Time_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct Time_Cluster_Tag
{
    uint8_t time[4];
    uint8_t timeStatus;
#if ( g_TIME_ZONE_ATTRIBUTE_ENABLE_d== 1) 	
    uint8_t timeZone[4];
#endif
#if ( g_TIME_DST_START_ATTRIBUTE_ENABLE_d== 1) 	
    uint8_t dstStart[4];
#endif
#if ( g_TIME_DST_END_ATTRIBUTE_ENABLE_d== 1) 	
    uint8_t dstEnd[4];
#endif
#if ( g_TIME_DST_SHIFT_ATTRIBUTE_ENABLE_d== 1)
    uint8_t dstShift[4];
#endif
#if ( g_TIME_STANDARD_TIME_ATTRIBUTE_ENABLE_d== 1) 	
    uint8_t standardTime[4];
#endif
#if ( g_TIME_LOCAL_TIME_ATTRIBUTE_ENABLE_d== 1) 	
    uint8_t localTime[4];
#endif
#if ( g_TIME_LASTSET_TIME_ATTRIBUTE_ENABLE_d== 1) 	
    uint8_t lastSetTime[4];
#endif
#if ( g_TIME_VALID_UNTIL_TIME_ATTRIBUTE_ENABLE_d== 1) 	
    uint8_t validUntilTime[4];
#endif
} __attribute__((__packed__))Time_Cluster_t;

/*-----------------------------------------------------------------------------
* Function Prototypes
--------------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Time_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataLocation );

#endif      /* ( g_TIME_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif      /*_ZCL_GENERAL_TIME_CLUSTER_H_*/
