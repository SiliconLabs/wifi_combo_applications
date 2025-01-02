/******************************************************************************
* AppDRLCUtilities..h
******************************************************************************
* Program Description:
* This file contains the drlc cluster App type definations
******************************************************************************/
#ifndef _APP_DRLC_UTILITIES_H_
#define _APP_DRLC_UTILITIES_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_DRLC_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_DRLC_MAX_EVENT_TABLE_SIZE_c                           0x03
#define g_DRLC_MAX_DEVICE_CLASS_SUPPORTED_c                     0x0F
#define g_RANDOMIZE_START_TIME_FLAG_c     						0x01
#define g_RANDOMIZE_END_TIME_FLAG_c       						0x02
#define g_CANCEL_WITH_RANDOMIZATION_c                           0x01
#define g_CANCEL_WITH_RANDOMIZATION_c        					0x01
#define g_EVENT_OPT_IN_DEFAULT_c                                0x01
#define g_LCE_INVALID_INDEX_c                                   0xFF
#define g_LCE_INVALID_STARTTIME_c                               0xFFFFFFFF
#define g_MINUTES_PER_DAY_c                                     0x05A0
#define g_FREE_ENTRY_c                                          0x00
#define g_LCE_SUCCESS_c                                         0x00
#define g_LCE_FAILURE_c                                         0xFF
#define g_LCE_INVALID_EVENT_ID_c                                0xFFFFFFFF
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

enum
{
    EVENT_OPT_FLAG_OPT_IN                                = 0x01,
    EVENT_OPT_FLAG_PARTIAL                               = 0x02
};

enum
{
    EVENT_COMMAND_RECEIVED                               = 0x01,
    EVENT_STARTED                                        = 0x02,
    EVENT_COMPLETED                                      = 0x03,
    EVENT_OPT_OUT                                        = 0x04,
    EVENT_OPT_IN                                         = 0x05,
    EVENT_CANCELLED                                      = 0x06,
    EVENT_SUPERSEDED                                     = 0x07,
    EVENT_PARTIALLY_COMPLETED_OPT_OUT                    = 0x08,
    EVENT_PARTIALLY_COMPLETED_OPT_IN                     = 0x09,
    EVENT_COMPLETED_NO_USER_PARTICIPATION_OPT_OUT        = 0x0A,
    EVENT_REJECTED_INVALID_CANCEL_COMMAND                = 0xF8,
    EVENT_REJECTED_INVALID_CANCEL_INVALID_EFFECTIVE_TIME = 0xF9,
    EVENT_REJECTED_EXPIRED                               = 0xFB,
    EVENT_REJECTED_INVALID_CANCEL_UNDEFINED_EVENT        = 0xFD,
    EVENT_REJECTED                                       = 0xFE
};

enum SignatureType
{
    SigReserved1                                    = 0x00,
    ECDSA                                           = 0x01,
    SigReserved2                                    = 0x02
};

typedef struct LCEServerTableEntry_tag
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
} __attribute__((__packed__))LCEServerTableEntry_t;

typedef struct DRLCEventTableEntry_tag
{
  LoadControlEvent_t event;
  uint8_t entryStatus;
} __attribute__((__packed__))LCEClientTableEntry_t;

typedef struct ESPAddrInfo_tag
{
    uint8_t addrMode;
    uint8_t dstAddress[8];
    uint8_t endpoint;
} __attribute__((__packed__))ESPAddrInfo_t;

typedef struct LECcmdTxDetails_tag
{
    uint8_t dstAddrMode;
    uint8_t dstAddr[8];
    uint8_t dstEndPoint;
    uint8_t tranSeqNum;
}__attribute__((__packed__))LECcmdTxDetails_t;

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

void AppDRLCTableInit( void  );

#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
LCEServerTableEntry_t * AppgetLCETableEntry ( uint8_t index );
void AppsendLoadConrolEventCommand( LoadControlEvent_t *pLCETable );
void AppGetScheduledLCEHandler( uint32_t startTime, uint8_t numEvents );


uint8_t App_addEntryLCETable(LoadControlEvent_t *pLCETable, uint8_t index);
void App_removeAllLCETableEntires(void);
uint8_t App_removeAnEntryFrmLCETable(uint32_t evtID);
uint8_t App_drlcCount(void);

#endif          /*  g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
void AppLoadControlEventHandler( LoadControlEvent_t *Event );
void AppsendReportStatusEvent( uint8_t status, LoadControlEvent_t * reportEvent );
void AppCancelEvent( uint32_t eventId, uint16_t deviceclass, uint16_t UEGroup,
                     uint8_t cancelControl, uint32_t effectiveTime );
void AppCancelAllEvents( uint8_t cancelControl );
LCEClientTableEntry_t * AppGetClientLCETableEntry ( uint8_t index );
void AppScheduleEvent( LoadControlEvent_t *newEvent, uint16_t deviceClass );
void AppOptInOrOut( uint32_t eventId, bool optIn );



#endif          /*  g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */

void AppDRLCTick( void );
void App_DRLCEventsHandler( ZCL_Event_t *pZCLEvent,
                            APSDE_Data_Indication_t *pBuffer );
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif          /* _APP_DRLC_UTILITIES_H_ */
