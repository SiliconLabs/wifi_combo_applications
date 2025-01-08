/******************************************************************************
* ZCL_Commissioning_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Commissioning cluster attribute and cmmnd
information
******************************************************************************/

#ifndef _ZCL_COMMISSIONING_CLUSTER_H_
#define _ZCL_COMMISSIONING_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/********************************93**********************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Commissioning Cluster */
#define g_COMMISSIONING_CLUSTER_c                           0x0015

/* Commisioning Cluster Attribute Set information */
#define g_COMMISSIONING_SHORT_ADDRESS_c                     0x0000
#define g_COMMISSIONING_EXTENDED_PANID_c                    0x0001
#define g_COMMISSIONING_PANID_c                             0x0002
#define g_COMMISSIONING_CHANNEL_MASK_c                      0x0003
#define g_COMMISSIONING_PROTOCOL_VERSION_c                  0x0004
#define g_COMMISSIONING_STACK_PROFILE_c                     0x0005
#define g_COMMISSIONING_STARTUP_CONTROL_c                   0x0006

#define g_COMMISSIONING_TRUST_CENTER_ADDRESS_c              0x0010
#define g_COMMISSIONING_TRUST_CENTER_MASTER_KEY_c           0x0011
#define g_COMMISSIONING_NETWORK_KEY_c                       0x0012
#define g_COMMISSIONING_USE_INSECURE_JOIN_c                 0x0013
#define g_COMMISSIONING_PRE_CONFIGURED_LINK_KEY_c           0x0014
#define g_COMMISSIONING_NETWORK_KEY_SEQ_NUM_c               0x0015
#define g_COMMISSIONING_NETWORK_KEY_TYPE_c                  0x0016
#define g_COMMISSIONING_NETWORK_MANAGER_ADDRESS_c           0x0017

#define g_COMMISSIONING_SCAN_ATTEMPTS_c                     0x0020
#define g_COMMISSIONING_TIME_BETWEEN_SCANS_c                0x0021
#define g_COMMISSIONING_REJOIN_INTERVAL_c                   0x0022
#define g_COMMISSIONING_MAX_REJOIN_INTERVAL_c               0x0023

#define g_COMMISSIONING_INDIRECT_POLL_RATE_c                0x0030
#define g_COMMISSIONING_PARENT_RETRY_THRESHOLD_c            0x0031

#define g_COMMISSIONING_CONCENTRATOR_FLAG_c                 0x0040
#define g_COMMISSIONING_CONCENTRATOR_RADIUS_c               0x0041
#define g_COMMISSIONING_CONCENTRATOR_DISCOVERY_TIME_c       0x0042

#define m_MAX_SAS_ATTRIBUTE_SET_c                           0x05
/* enumeration for Commissioning Cluster Client Side Commands*/
enum commissioningClusterClientCommands
{
    g_CommissioningRestartDeviceReq_c                       = 0x00,
    g_CommissioningSaveStartupParametersReq_c               = 0x01,
    g_CommissioningRestoreStartupParametersReq_c            = 0x02,
    g_CommissioningResetStartupParametersReq_c              = 0x03
};
/* enumeration for Commissioning Cluster Server Side Commands*/
enum commissioningClusterServerCommands
{
    g_CommissioningRestartDeviceResp_c                      = 0x00,
    g_CommissioningSaveStartupParametersResp_c              = 0x01,
    g_CommissioningRestoreStartupParametersResp_c           = 0x02,
    g_CommissioningResetStartupParametersResp_c             = 0x03
};
/* enumeration for Commissioning Cluster Events */
enum commissioningEvents
{
    g_CommissioningRestartDeviceReqEvent_c                  = 0x10,
    g_CommissioningSaveStartupParametersReqEvent_c          ,
    g_CommissioningRestoreStartupParametersReqEvent_c       ,
    g_CommissioningResetStartupParametersReqEvent_c         ,
    g_CommissioningRestartDeviceRespEvent_c                 ,
    g_CommissioningSaveStartupParametersRespEvent_c         ,
    g_CommissioningRestoreStartupParametersRespEvent_c      ,
    g_CommissioningResetStartupParametersRespEvent_c

};

/*enumeration for Reset Req Command Types */
enum commissioningResetTypes
{
    g_Reset_Current_c                                       = 0x01,
    g_Reset_All_c                                           = 0x02,
    g_Erase_Index_c                                         = 0x04
};


typedef struct CommissioningRestartDeviceCmdPayload_Tag
{
    uint8_t Options;
    uint8_t Delay;
    uint8_t Jitter;
}__attribute__((__packed__))commissioningRestartDeviceCmdPayload_t;

typedef struct CommissioningSaveStartupParamsCmdPayload_Tag
{
    uint8_t Options;
    uint8_t Index;
} __attribute__((__packed__))CommissioningSaveStartupParamsCmdPayload_t;

typedef struct CommissioningRestoreStartupParamsCmdPayload_Tag
{
    uint8_t Options;
    uint8_t Index;
} __attribute__((__packed__))CommissioningRestoreStartupParamsCmdPayload_t;

typedef struct CommissioningResetStartupParamsCmdPayload_Tag
{
    uint8_t Options;
    uint8_t Index;
} __attribute__((__packed__))CommissioningResetStartupParamsCmdPayload_t;

typedef struct CommissioningResponseCmdPayload_Tag
{
    uint8_t Status;
} __attribute__((__packed__))CommissioningResponseCmdPayload_t;

/******************************************************************************
* Memory Declarations
******************************************************************************/

#if ( g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Commissioning_Client_Cluster_Info;
#endif

#if ( g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Commissioning_Server_Cluster_Info;
#endif

/******************************************************************************
* Type Definitions
******************************************************************************/
/* Commissioning Cluster Attributes structure */
typedef struct Commissioning_Cluster_Tag
{
    uint8_t     shortAddr[2];
    uint8_t     extendedPANId[8];
    uint8_t     PANId[2];
    uint8_t     channelMask[4];
    uint8_t     protocolVersion;
    uint8_t     stackProfile;
    uint8_t     startupControl;

    uint8_t     trustCenterAddress[8];
    uint8_t     trustCenterMasterKey[16];
    uint8_t     networkKey[16];
    uint8_t     useInsecureJoin;
    uint8_t     preconfiguredLinkKey[16];
    uint8_t     networkKeySeqNum;
    uint8_t     networkKeyType;
    uint8_t     networkManagerAddress[2];

    uint8_t     scanAttempts;
    uint8_t     timeBetweenScans[2];
    uint8_t     rejoinInterval[2];
    uint8_t     maxRejoinInterval[2];

    uint8_t     indirectPollRate[2];
    uint8_t     parentRetryThreshold;

    uint8_t     concentratorFlag;
    uint8_t     concentratorRadius;
    uint8_t     concentratorDiscoveryTime[2];

} Commissioning_Cluster_t;

/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_Commissioning_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                                      /*_ZCL_COMMISSIONING_CLUSTER_H_*/
