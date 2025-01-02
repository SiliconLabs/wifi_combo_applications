/******************************************************************************
* ZCL_DRLC_Cluster.h
******************************************************************************
* Program Description:
* This file contains the DRLC cluster attribute and cmmnd information
******************************************************************************/

#ifndef _ZCL_DRLC_CLUSTER_H_
#define _ZCL_DRLC_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"
#include "ZCL_Interface.h"

#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_DRLC_CLUSTER_c                                0x0701

#define g_UTILITY_ENROLEMENT_GROUP_ATTRIBUTE_c          0x0000
#define g_START_RANDOMIZE_MINUTES_ATTRIBUTE_c           0x0001
#define g_STOP_RANDOMIZE_MINUTES_ATTRIBUTE_c            0x0002
#define g_DEVICE_CLASS_VALUE_ATTRIBUTE_c                0x0003
#define g_SIGNATURE_SIZE_c                              0x2A
#define g_INVALID_SIGNATURE_c                           0xFF

enum
{
    g_DRLC_LoadControlEvent_c         = 0xA0,
    g_DRLC_CancelLoadControlEvent_c,
    g_DRLC_CancelAllLoadControlEvents_c,
    g_DRLC_GetScheduledEvents_c,
    g_DRLC_ReportEventStatusEvent_c
};

enum
{
    g_LoadControlEventCmd_c                         = 0x00,
    g_CancelLoadControlEventCmd_c                   = 0x01,
    g_CancelAllLoadControlEventCmds_c               = 0x02
};


enum
{
    g_ReportEventStatusCmd_c                        = 0x00,
    g_GetScheduledEventCmd_c                        = 0x01
};
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_DRLC_Server_Cluster_Info;
#endif

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_DRLC_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct DRLCClusterAttribute_Tag
{
    uint8_t utilityEnrolementGroup;
    uint8_t startRandomizeEvents;
    uint8_t stopRandomizeEvents;
    uint8_t deviceClassValue[2];
} __attribute__((__packed__))DRLCClusterAttribute_t;

typedef struct LoadControlEvent_tag
{
    uint32_t eventId;
    uint16_t deviceClass;
    uint8_t  utilityEnrollmentGroup;
    uint32_t startTime;
    uint16_t duration;
    uint8_t  criticalityLevel;
    uint8_t  coolingTempOffset;
    uint8_t  heatingTempOffset;
    uint16_t coolingTempSetPoint;
    uint16_t heatingTempSetPoint;
    uint8_t  avgLoadPercentage;
    uint8_t  dutyCycle;
    uint8_t  eventControl;
    uint16_t startRand;
    uint16_t endRand;
    uint8_t  optionControl;
} __attribute__((__packed__))LoadControlEvent_t;


typedef struct cancelLoadControlEvent_tag
{
    uint8_t  eventId;
    uint16_t deviceClass;
    uint8_t  utilityEnrollmentGroup;
    uint8_t  cancelControl;
    uint32_t effectiveTime;
} __attribute__((__packed__))cancelLoadControlEvent_t;


typedef struct cancelAllLoadControlEvents_Tag
{
   uint8_t cancelControl;
} __attribute__((__packed__))cancelAllLoadControlEvents_t;

typedef struct  ReportEventStatus_tag
{
    uint32_t eventId;
    uint8_t  eventStatus;
    uint32_t startTime;
    uint8_t  criticalityLevel;
    int16_t  coolingTempSetPoint;
    int16_t  heatingTempSetPoint;
    int8_t   avgLoadPercentage;
    uint8_t  dutyCycle;
    uint8_t  eventControl;
    uint8_t  signatureType;
    uint8_t  signature[42];
}__attribute__((__packed__))ReportEventStatus_t;


 typedef struct GetScheduledEvents_tag
{
    uint32_t startTime;
    uint8_t  numOfEvents;
} __attribute__((__packed__))GetScheduledEvents_t;

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_DRLC_Cluster_Cmd
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
*	pResponseLocation - Pointer for response creation
*   pActionHandler - Pointer for event updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the DRLC cluster command received in the indication
*     	
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_DRLC_Cluster_Cmd ( uint8_t endPointId,
                                      uint8_t asduLength,
                                      uint8_t* pReceivedAsdu,
                                      uint8_t* pResponseLocation,
                                      ZCL_ActionHandler_t *pActionHandler,
                                      ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                      uint8_t * pAttrsDataBaseLocation );

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )

uint8_t ZCL_CreateReportEventStatusCommand ( uint8_t transeqnum,
            ReportEventStatus_t* pReportEvent, uint8_t* pRespLocation );
#endif

#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t ZCL_DRLCcreateLCECommand ( uint8_t transeqnum, uint8_t* pRespLocation,
    LoadControlEvent_t * pLCETable );

#endif
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                      /* g_DRLC_CLUSTER_ENABLE_d == 1 */

#endif                                      /*_ZCL_DRLC_CLUSTER_H_*/
