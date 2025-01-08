/********************************************************************************************
* AppDRLCUtilities.c
*********************************************************************************************
* Program Description:
*   This module provides all the functionality for supporting DRLC cluster
*
********************************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "memory_utility.h"
#include "sw_timer.h"
#include "AppTimerUtilities.h"
#include "ApplicationThinLayer.h"
#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
#include "ZCL_DRLC_Cluster.h"
#include "AppDRLCUtilities.h"
#endif
#if( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
#include "AppCBKEUtilities.h"
#endif
/*-----------------------------------------------------------------------------------------
* Global Constants
*------------------------------------------------------------------------------------------*/
#define m_MAX_CRITICALITY_LEVEL_c 0x0F
#define m_MIN_CRITICALITY_LEVEL_c 0x01

#define m_DEVICE_CLASS_BIT_MAP_c  0x01

/*-----------------------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------------------*/
#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
ESPAddrInfo_t ESPAddrInfo;
LCEClientTableEntry_t clientLCETable[g_DRLC_MAX_EVENT_TABLE_SIZE_c];

const uint8_t controlValueToStatus[] =
{
  EVENT_COMPLETED_NO_USER_PARTICIPATION_OPT_OUT,
  EVENT_COMPLETED,
  EVENT_PARTIALLY_COMPLETED_OPT_OUT,
  EVENT_PARTIALLY_COMPLETED_OPT_IN
};
#endif          /*( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
LCEServerTableEntry_t serverLCETable[g_DRLC_MAX_EVENT_TABLE_SIZE_c];
LECcmdTxDetails_t LECcmdTxDetails;

/*-------------------------------------------------------------------------------------------
* Private Memory declarations
*-------------------------------------------------------------------------------------------*/

static uint8_t scheduledLCESendingIndex = g_LCE_INVALID_INDEX_c;
static uint32_t scheduledLCESendingTime = 0x00;
static uint8_t numScheduledLCELeftToSend;


/*--------------------------------------------------------------------------------------------
* Function Prototypes
*--------------------------------------------------------------------------------------------*/

void AppDRLCServerTick( void );

#endif          /*  ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
void AppDRLCClientTick( void );
static bool AppMatchUegAndDeviceClass( uint8_t utilityEnrollmentGroup,
                                       uint16_t deviceClass );
static void App_CopyESPAddressInfo(APSDE_Data_Indication_t *pBuffer);

#endif          /*( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/*------------------------------------------------------------------------------------------
* Public Functions
*--------------------------------------------------------------------------------------------*/

#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
void AppDRLCTableInit( void  )
{
  uint8_t index;

  for ( index = 0; index < g_DRLC_MAX_EVENT_TABLE_SIZE_c; index++ ) {
#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
      memUtils_memSet ( (uint8_t*)&serverLCETable[index],
                g_ZCL_INVALID_DATA_c, sizeof(LCEServerTableEntry_t) );
#endif

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
      memUtils_memSet ( (uint8_t*)&clientLCETable[index],
                g_ZCL_INVALID_DATA_c,  sizeof(LCEClientTableEntry_t) );
      clientLCETable[index].entryStatus = g_FREE_ENTRY_c;
#endif
  }
}

/**************************************************************************************/

void AppDRLCTick( void )
{

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
    AppDRLCClientTick();
#endif

#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
    AppDRLCServerTick();
#endif
}

/**************************************************************************************/

void App_DRLCEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
        if (( pZCLEvent->eventId )== g_DRLC_LoadControlEvent_c){
            App_CopyESPAddressInfo(pBuffer);
            AppLoadControlEventHandler( (LoadControlEvent_t*)pZCLEvent->pEventData );
		}
		else if (( pZCLEvent->eventId )== g_DRLC_CancelLoadControlEvent_c){
                cancelLoadControlEvent_t* cancelLCE = (cancelLoadControlEvent_t*)pZCLEvent->pEventData;
                App_CopyESPAddressInfo(pBuffer);
                AppCancelEvent( cancelLCE->eventId, cancelLCE->deviceClass,
                                cancelLCE->utilityEnrollmentGroup, cancelLCE->cancelControl,
                                cancelLCE->effectiveTime );
            }
		else if (( pZCLEvent->eventId )== g_DRLC_CancelAllLoadControlEvents_c){
            App_CopyESPAddressInfo(pBuffer);
            AppCancelAllEvents( *pZCLEvent->pEventData );
		}
#endif

#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
		if (( pZCLEvent->eventId )== g_DRLC_GetScheduledEvents_c){

            LECcmdTxDetails.dstAddrMode = pBuffer->src_addr_mode;
            memUtils_memCopy( LECcmdTxDetails.dstAddr, pBuffer->src_address.IEEE_address, 0x08 );
            LECcmdTxDetails.dstEndPoint = pBuffer->src_endpoint;
            LECcmdTxDetails.tranSeqNum = pBuffer->a_asdu[1];

            AppGetScheduledLCEHandler( ((GetScheduledEvents_t*)pZCLEvent->pEventData)->startTime,
                                       ((GetScheduledEvents_t*)pZCLEvent->pEventData)->numOfEvents );
		}
#endif
}
#endif          /* ( g_DRLC_CLUSTER_ENABLE_d == 1 ) */
/***********************************************************************************/

#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t AppgetScheduledLCECount( void )
{
    uint8_t index;
    uint8_t count = 0x00;

    for ( index = 0; index < g_DRLC_MAX_EVENT_TABLE_SIZE_c; index++ ) {
        if ( serverLCETable[index].startTime != g_LCE_INVALID_STARTTIME_c ) {
            count++;
        }
    }
    return count;
}

/**************************************************************************************/

void AppGetScheduledLCEHandler( uint32_t startTime, uint8_t numEvents )
{
    scheduledLCESendingIndex = 0;
    scheduledLCESendingTime = startTime;
    if ( numEvents == 0 ) {
        numScheduledLCELeftToSend = AppgetScheduledLCECount();
        scheduledLCESendingTime = 0;
    }
    else {
        if ( ! scheduledLCESendingTime ) {
            scheduledLCESendingTime = ZCL_CallBackGetCurrentTime();
        }
        numScheduledLCELeftToSend = numEvents;
    }
}

/***************************************************************************************/
LCEServerTableEntry_t * AppgetLCETableEntry ( uint8_t index )
{
    if ( index > g_DRLC_MAX_EVENT_TABLE_SIZE_c ){
        return g_NULL_c;
    }
    else {
        return &serverLCETable[index];
    }
}

/***************************************************************************************/
void AppDRLCServerTick( void )
{
    LCEServerTableEntry_t *pLCEInfo;

    if ( scheduledLCESendingIndex != g_LCE_INVALID_INDEX_c ) {
        if ( ( scheduledLCESendingIndex < g_DRLC_MAX_EVENT_TABLE_SIZE_c ) &&
             ( numScheduledLCELeftToSend > 0 ) ) {

                pLCEInfo = AppgetLCETableEntry( scheduledLCESendingIndex );

                while( ( ( pLCEInfo->startTime == g_LCE_INVALID_STARTTIME_c ) ||
                         ( scheduledLCESendingTime > pLCEInfo->startTime ) )  &&
                         ( scheduledLCESendingIndex < g_DRLC_MAX_EVENT_TABLE_SIZE_c ) ) {

                            scheduledLCESendingIndex++;
                            pLCEInfo = AppgetLCETableEntry( scheduledLCESendingIndex );
                        }
                if ( ( scheduledLCESendingIndex < g_DRLC_MAX_EVENT_TABLE_SIZE_c ) &&
                     ( scheduledLCESendingTime <= pLCEInfo->startTime ) ) {

                        AppsendLoadConrolEventCommand( (LoadControlEvent_t *)pLCEInfo );

                        scheduledLCESendingIndex++;
                        numScheduledLCELeftToSend--;
                }
        }
        else {
            scheduledLCESendingIndex = g_LCE_INVALID_INDEX_c;
            scheduledLCESendingTime = 0x00;
            numScheduledLCELeftToSend = 0x00;
        }
    }
}

/***************************************************************************************/

void AppsendLoadConrolEventCommand( LoadControlEvent_t *pLCETable )
{
    uint8_t asdu[40];
    uint8_t asduLength;

    asduLength = ZCL_DRLCcreateLCECommand ( LECcmdTxDetails.tranSeqNum,
                                     asdu,
                                     pLCETable );

    App_SendData ( LECcmdTxDetails.dstAddrMode,
             LECcmdTxDetails.dstAddr,
             LECcmdTxDetails.dstEndPoint,
             g_DRLC_CLUSTER_c,
             asduLength,
             asdu );

}
#endif          /* ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 ) */

/****************************************************************************************/

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )

/***************************************************************************************/

LCEClientTableEntry_t * AppGetClientLCETableEntry ( uint8_t index )
{
    if ( index > g_DRLC_MAX_EVENT_TABLE_SIZE_c ){
        return g_NULL_c;
    }
    else {
        return &clientLCETable[index];
    }
}

/****************************************************************************************/

void AppDRLCClientTick( void )
{
  uint32_t ctime;
  LCEClientTableEntry_t *e;
  LCEClientTableEntry_t *f;
  uint8_t i;
  uint8_t j;

  ctime = ZCL_CallBackGetCurrentTime();

  for( i = 0; i < g_DRLC_MAX_EVENT_TABLE_SIZE_c; i++ ) {

      e = &clientLCETable[i];

      if ( ( e->entryStatus == EVENT_COMMAND_RECEIVED ) &&
           ( e->event.startTime+e->event.startRand <= ctime ) ) {
          for ( j = i; j < g_DRLC_MAX_EVENT_TABLE_SIZE_c; j++ ) {
              f = &clientLCETable[j];
              if ( ( f->entryStatus == EVENT_COMMAND_RECEIVED )&&( e->event.eventId == f->event.eventId ) )
              {
                  f->entryStatus = EVENT_STARTED;
              }
          }
          if ( e->event.optionControl & EVENT_OPT_FLAG_OPT_IN ) {
              AppsendReportStatusEvent( EVENT_STARTED, (LoadControlEvent_t*)&e->event );
          }
          else {
              AppsendReportStatusEvent( EVENT_OPT_OUT, (LoadControlEvent_t*)&e->event );
          }
          //RP:return;
      }
      else if ( ( e->entryStatus == EVENT_STARTED ) &&
              ( ( e->event.startTime + ((uint32_t)e->event.duration*60) + e->event.endRand ) <= ctime ) )
      {
                   for ( j = i; j < g_DRLC_MAX_EVENT_TABLE_SIZE_c; j++ ) {
                       f = &clientLCETable[j];
                       if ( ( f->entryStatus == EVENT_STARTED )&&( e->event.eventId == f->event.eventId ) )
                       {
                           f->entryStatus = g_FREE_ENTRY_c;
                       }
                   }
                   AppsendReportStatusEvent( controlValueToStatus[e->event.optionControl],
                                             (LoadControlEvent_t*)& e->event );
                   //RP:return;
      }
      else if ( (e->entryStatus == EVENT_SUPERSEDED ) && ( e->event.startTime <= ctime) )
      {
          for ( j = i; j < g_DRLC_MAX_EVENT_TABLE_SIZE_c; j++ ) {
              f = &clientLCETable[j];
              if ( ( f->entryStatus == EVENT_SUPERSEDED )&&( e->event.eventId == f->event.eventId ) )
              {
                  f->entryStatus = g_FREE_ENTRY_c;
              }
          }
          AppsendReportStatusEvent( EVENT_SUPERSEDED, (LoadControlEvent_t*)&e->event );
          //RP:return;
      }
      else if ( ( e->entryStatus == EVENT_CANCELLED ) && ( e->event.startTime <= ctime ) )
      {
          for ( j = i; j < g_DRLC_MAX_EVENT_TABLE_SIZE_c; j++ ) {
              f = &clientLCETable[j];
              if ( ( f->entryStatus == EVENT_CANCELLED )&&( e->event.eventId == f->event.eventId ) )
              {
                  f->entryStatus = g_FREE_ENTRY_c;
              }
          }
          AppsendReportStatusEvent( EVENT_CANCELLED, (LoadControlEvent_t*)&e->event );
          //RP:return;
      }
      /*If the status is changed to FREE ENTRY, reset the whole load control event command table to default data*/
      if( (e->entryStatus == g_FREE_ENTRY_c) && (e->event.eventId != g_LCE_INVALID_EVENT_ID_c) )
      {
         memUtils_memSet ( (uint8_t*)&e->event, g_ZCL_INVALID_DATA_c, sizeof(LoadControlEvent_t) );
      }
  }
}

/**************************************************************************************/
void AppsendReportStatusEvent( uint8_t status, LoadControlEvent_t * reportEvent )
{
    ReportEventStatus_t Report;
    uint8_t asdu[70];
    uint8_t asduLength;

    if (reportEvent == g_NULL_c)
    {

    	return;
    }

    Report.eventId = reportEvent->eventId;
    Report.eventStatus = status;
    Report.startTime = ZCL_CallBackGetCurrentTime();
    Report.criticalityLevel = reportEvent->criticalityLevel;
    Report.coolingTempSetPoint = reportEvent->coolingTempSetPoint;
    Report.heatingTempSetPoint = reportEvent->heatingTempSetPoint;
    Report.avgLoadPercentage = reportEvent->avgLoadPercentage;
    Report.dutyCycle = reportEvent->dutyCycle;
    Report.eventControl = reportEvent->eventControl;
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    Report.signatureType = ECDSA;

	if ( MCE_SUCCESS != App_CBKEapplyOrVerifyDSAsignature( g_TRUE_c,
         (sizeof(ReportEventStatus_t) - g_SIGNATURE_SIZE_c ), (uint8_t*)&Report ) ) {
        return;
    };	
#else
    Report.signatureType = g_INVALID_SIGNATURE_c;
    memUtils_memSet( Report.signature, 0xFF, g_SIGNATURE_SIZE_c );

#endif  /* ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 ) */

    asduLength = ZCL_CreateReportEventStatusCommand ( App_GetTransSeqNumber(),
                                                      &Report, asdu );

    App_SendData ( ESPAddrInfo.addrMode,
                   ESPAddrInfo.dstAddress,
                   ESPAddrInfo.endpoint,
                   g_DRLC_CLUSTER_c,
                   asduLength,
                   asdu );

}

/***********************************************************************************/
void AppScheduleEvent( LoadControlEvent_t *newEvent, uint16_t deviceClass )
{
    uint8_t i, j;
    uint32_t ctime;
    uint16_t deviceClassBitMap = m_DEVICE_CLASS_BIT_MAP_c ;
    LCEClientTableEntry_t *e;
    uint8_t EventEntryCreated = g_FALSE_c;
    ctime = ZCL_CallBackGetCurrentTime();

    if (newEvent == g_NULL_c)
    {

    	return;
    }

    if ( ( newEvent->criticalityLevel > m_MAX_CRITICALITY_LEVEL_c ) || ( newEvent->criticalityLevel < m_MIN_CRITICALITY_LEVEL_c ) ) {
        return;
    }
    if ( ( newEvent->startTime == g_LCE_INVALID_STARTTIME_c ) ||
         ( newEvent->duration > g_MINUTES_PER_DAY_c ) ) {
            AppsendReportStatusEvent( EVENT_REJECTED, newEvent );
            return ;
    }
    if ( ctime > ( newEvent->startTime + ( ((uint32_t)newEvent->duration) * 60 ) ) ) {
        AppsendReportStatusEvent( EVENT_REJECTED_EXPIRED, newEvent );
        return ;
    }
    for ( i = 0;i < g_DRLC_MAX_EVENT_TABLE_SIZE_c; i++ ) {
        e = &clientLCETable[i];
		if ( (e->entryStatus == EVENT_COMMAND_RECEIVED || e->entryStatus  == EVENT_STARTED) &&
			(e->event.eventId == newEvent->eventId ) ) {
         uint8_t asdu[10];
         asdu[0] =  g_ZCL_FC_PROFILE_WIDE_COMMAND_c | g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
                    g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
         asdu[1] = App_GetTransSeqNumber();
         asdu[2] = g_Default_Response_Command_c;
         asdu[3] = g_LoadControlEventCmd_c;
         asdu[4] = g_ZCL_Duplicate_Exists_c;

         App_SendData(  ESPAddrInfo.addrMode,
                        ESPAddrInfo.dstAddress,
                        ESPAddrInfo.endpoint,
                        g_DRLC_CLUSTER_c,
                        0x05,
                        asdu );

         return ;
        }
    }
    for ( j = 0; j < g_DRLC_MAX_DEVICE_CLASS_SUPPORTED_c ; j++ ) {
        if ( (deviceClassBitMap & newEvent->deviceClass) && ( deviceClassBitMap & deviceClass ) ) {
            for ( i= 0; i < g_DRLC_MAX_EVENT_TABLE_SIZE_c; i++ ) {
                LCEClientTableEntry_t *currentEntry = &clientLCETable[i];
                if ( ( ( currentEntry->entryStatus == EVENT_COMMAND_RECEIVED ) ||
                       ( currentEntry->entryStatus == EVENT_STARTED ) ) &&
                     ( (deviceClassBitMap & newEvent->deviceClass )== currentEntry->event.deviceClass ) ) {

					if ( ( newEvent->startTime < ( currentEntry->event.startTime +
							(uint32_t)currentEntry->event.duration * 60 ) ) &&
							( currentEntry->event.startTime < ( newEvent->startTime +
							(int32_t)newEvent->duration * 60 ) ) ) {
							
                        if ( currentEntry->entryStatus == EVENT_COMMAND_RECEIVED ) {
                             currentEntry->event.startTime = ctime;
                        }
                        else {
                            currentEntry->event.startTime = ( newEvent->startTime + newEvent->startRand - 1 );
                        }
                        currentEntry->entryStatus = EVENT_SUPERSEDED;					
                    }
                }
            }
            for ( i = 0; i < g_DRLC_MAX_EVENT_TABLE_SIZE_c; i++ ) {
                e = &clientLCETable[i];
                if ( e->entryStatus == g_FREE_ENTRY_c ) {
                    memUtils_memCopy((uint8_t*)&(e->event), (uint8_t*)newEvent, sizeof(LoadControlEvent_t) );
                    e->event.deviceClass = deviceClassBitMap;
                    e->entryStatus = EVENT_COMMAND_RECEIVED;
                    EventEntryCreated = g_TRUE_c;
                    break;
                }
            }
        }
        deviceClassBitMap = deviceClassBitMap << 1;
    }
    if ( EventEntryCreated ) {
       AppsendReportStatusEvent( EVENT_COMMAND_RECEIVED, newEvent );
    }
    else {
       AppsendReportStatusEvent( EVENT_REJECTED, newEvent );
    }
    return;
}

/**************************************************************************************/
void AppCancelEvent( uint32_t eventId, uint16_t deviceclass, uint16_t UEGroup,
                     uint8_t cancelControl, uint32_t effectiveTime )
{
  uint8_t i, j;
  LCEClientTableEntry_t *e;
  LoadControlEvent_t undefEvent;
  uint32_t cancelTime = 0;
  uint16_t devClaassBitMap = m_DEVICE_CLASS_BIT_MAP_c;
  uint32_t currentTime = ZCL_CallBackGetCurrentTime();

  if ( ! AppMatchUegAndDeviceClass( UEGroup, deviceclass ) ) {
      return ;
  }
  for ( i = 0; i < g_DRLC_MAX_EVENT_TABLE_SIZE_c; i++ ) {
      e = &clientLCETable[i];
      if ( ( e->event.eventId != eventId ) || ( e->entryStatus == g_FREE_ENTRY_c ) ||
           ( e->entryStatus == EVENT_CANCELLED ) ) {
               continue ;
      }
      if ( ( effectiveTime == g_LCE_INVALID_STARTTIME_c ) ||
           ( effectiveTime > ( e->event.startTime +(((uint32_t) e->event.duration) * 60) ) ) ) {
            AppsendReportStatusEvent( EVENT_REJECTED_INVALID_CANCEL_INVALID_EFFECTIVE_TIME,
                                      &e->event );
            return;
      }
      devClaassBitMap = 0x01;
      for ( j = 0; j < g_DRLC_MAX_DEVICE_CLASS_SUPPORTED_c ; j++ ) {
          if ( ( devClaassBitMap & e->event.deviceClass ) ==  ( devClaassBitMap & deviceclass ) ) {
              if ( cancelControl & g_CANCEL_WITH_RANDOMIZATION_c ) {
                  if ( effectiveTime == 0 ) {
                      cancelTime = currentTime ;
                  }
              }
              else {
                  if ( effectiveTime > ( e->event.startTime + e->event.duration * 60 ) ) {
                      AppsendReportStatusEvent( EVENT_REJECTED_INVALID_CANCEL_INVALID_EFFECTIVE_TIME,
                                                &e->event );
                      return;
                  }
                  cancelTime = effectiveTime;
              }
              cancelTime += e->event.endRand;
              e->entryStatus = EVENT_CANCELLED;
              e->event.startTime = cancelTime;
          }
          devClaassBitMap <<= 0x01;
      }
      return;
  }
  memUtils_memSet( (uint8_t*)&undefEvent, 0x00, sizeof( LoadControlEvent_t ) );
  undefEvent.eventId = eventId;
  AppsendReportStatusEvent( EVENT_REJECTED_INVALID_CANCEL_UNDEFINED_EVENT, &undefEvent );

}

/****************************************************************************************/
void AppCancelAllEvents( uint8_t cancelControl )
{
    uint8_t i;
    LCEClientTableEntry_t *e;
    uint32_t currentTime = ZCL_CallBackGetCurrentTime();

    for (i = 0;i < g_DRLC_MAX_EVENT_TABLE_SIZE_c; i++ ) {
        e = &clientLCETable[i];
        if ( ( e->entryStatus != g_FREE_ENTRY_c ) && ( e->entryStatus != EVENT_CANCELLED ) ) {
            e->entryStatus = EVENT_CANCELLED;
            e->event.startTime = currentTime + e->event.endRand;
        }
    }

}

/**************************************************************************************/
static bool AppMatchUegAndDeviceClass( uint8_t utilityEnrollmentGroup,
                                        uint16_t deviceClass )
{
    uint8_t ueg = 0;
    uint16_t dc = 0;
    uint8_t status= g_ZCL_Success_c;

    if ((status = App_ReadWriteAttribute ( ZCL_Callback_Get_Device_EndPoint(),
                          g_DRLC_CLUSTER_c,
                          g_UTILITY_ENROLEMENT_GROUP_ATTRIBUTE_c,
                          (uint8_t*)&ueg,
                          g_Read_Attribute_Data_c )) !=g_ZCL_Success_c )
	{
		return g_FALSE_c;
	}

    else if ( (status = App_ReadWriteAttribute ( ZCL_Callback_Get_Device_EndPoint(),
                          g_DRLC_CLUSTER_c,
                          g_DEVICE_CLASS_VALUE_ATTRIBUTE_c,
                          (uint8_t*)&dc,
                          g_Read_Attribute_Data_c )) != g_ZCL_Success_c)
	{
		return g_FALSE_c;

	}
    if (!( dc & deviceClass ) ) {
        return g_FALSE_c;
    }
    if ( utilityEnrollmentGroup == 0 || ( utilityEnrollmentGroup == ueg ) ) {
        return g_TRUE_c;
    }
    return g_FALSE_c;
}

/***************************************************************************************/
void AppLoadControlEventHandler( LoadControlEvent_t *Event )
{
    uint16_t deviceclass;
	uint8_t maxRandMin = 0;

    if (Event == g_NULL_c)
    {

    	return;
    }

    if (!AppMatchUegAndDeviceClass(Event->utilityEnrollmentGroup, Event->deviceClass) ) {
        return ;
    }
    Event->startRand = 0x00;
    Event->endRand = 0x00;
    Event->optionControl = g_EVENT_OPT_IN_DEFAULT_c;

    if ( Event->eventControl & g_RANDOMIZE_START_TIME_FLAG_c ) {
	       Event->startRand = App_ReadWriteAttribute ( ZCL_Callback_Get_Device_EndPoint(),
                          g_DRLC_CLUSTER_c,
                          g_START_RANDOMIZE_MINUTES_ATTRIBUTE_c,
                          &maxRandMin,
                          g_Read_Attribute_Data_c );
			if ( ( Event->startRand  != g_ZCL_Success_c ) || ( maxRandMin == 0 ) ) {
				Event->startRand = g_NULL_c ;
			}
			else {
				Event->startRand = ( randomT() ) % ( maxRandMin * 60 );
			}
    }
    if ( Event->eventControl & g_RANDOMIZE_END_TIME_FLAG_c ) {
//        Event->endRand = AppGetRandomizationTime( g_FALSE_c );
	       Event->endRand = App_ReadWriteAttribute ( ZCL_Callback_Get_Device_EndPoint(),
                          g_DRLC_CLUSTER_c,
                          g_STOP_RANDOMIZE_MINUTES_ATTRIBUTE_c ,
                          &maxRandMin,
                          g_Read_Attribute_Data_c );
			if ( ( Event->endRand  != g_ZCL_Success_c ) || ( maxRandMin == 0 ) ) {
			 Event->endRand = g_NULL_c ;
			}
			else {
			 Event->endRand  = ( randomT() ) % ( maxRandMin * 60 );
			}
		
    }
    if ( Event->startTime == 0x00000000 ) {
        Event->startTime = ZCL_CallBackGetCurrentTime();
    }
    App_ReadWriteAttribute ( ZCL_Callback_Get_Device_EndPoint(),
                      g_DRLC_CLUSTER_c,
                      g_DEVICE_CLASS_VALUE_ATTRIBUTE_c,
                      (uint8_t*)&deviceclass,
                      g_Read_Attribute_Data_c );

    AppScheduleEvent( Event, deviceclass );

}

/**************************************************************************************/
void AppOptInOrOut( uint32_t eventId, bool optIn )
{
  uint8_t i;
  LCEClientTableEntry_t *e;
  LCEClientTableEntry_t *ec = g_NULL_c;

  for ( i = 0; i < g_DRLC_MAX_EVENT_TABLE_SIZE_c; i++ ) {

    e = &clientLCETable[i];

	  if ( ( e->entryStatus  == EVENT_COMMAND_RECEIVED || e->entryStatus  == EVENT_STARTED ) &&
											( e->event.eventId == eventId ) ) {
        bool preEventOption = e->event.optionControl & EVENT_OPT_FLAG_OPT_IN ;
        e->event.optionControl =( optIn ? ( e->event.optionControl | EVENT_OPT_FLAG_OPT_IN  ):
                                         ( e->event.optionControl & ~EVENT_OPT_FLAG_OPT_IN ));
        if ( ( preEventOption != optIn ) && ( e->entryStatus == EVENT_STARTED ) ) {
               e->event.optionControl |= EVENT_OPT_FLAG_PARTIAL ;
               ec = e;
        }
    }
  }
  if ( ec != g_NULL_c ) {
    AppsendReportStatusEvent( optIn ? EVENT_OPT_IN : EVENT_OPT_OUT, &ec->event );
  }
  return;
}

/***********************************************************************************/

static void App_CopyESPAddressInfo(APSDE_Data_Indication_t *pBuffer)
{

    if (pBuffer == g_NULL_c)
    {

    	return;
    }

	ESPAddrInfo.addrMode = pBuffer->src_addr_mode;
	if( pBuffer->src_addr_mode == g_Short_Address_Mode_c )
	{
		memUtils_memCopy( ESPAddrInfo.dstAddress, pBuffer->src_address.IEEE_address,
				g_SHORT_ADDRESS_LENGTH_c );
	}
	else
	{
		memUtils_memCopy( ESPAddrInfo.dstAddress, pBuffer->src_address.IEEE_address,
					 							g_EXTENDED_ADDRESS_LENGTH_c );
	}
	ESPAddrInfo.endpoint = pBuffer->src_endpoint;
}

#endif      /* ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/************************************************************************************/
#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
uint8_t App_addEntryLCETable(LoadControlEvent_t *pLCETable, uint8_t index)
{
    LCEServerTableEntry_t *pServerLCETable = g_NULL_c; ;
    uint8_t status = g_ZCL_Failure_c;

    if(index >= g_DRLC_MAX_EVENT_TABLE_SIZE_c )
    {
		status = g_ZCL_Insufficient_Space_c;
    }
    else
    {
    	pServerLCETable = AppgetLCETableEntry( index );
     /* if the pointer is not null, then Add entry to the Load Control Event Table*/
		if ((pLCETable != g_NULL_c) && (pServerLCETable != g_NULL_c))
		{
	//		memUtils_memCopy( (uint8_t*)pserverLCETable, (uint8_t*)pLCETable, sizeof(LCEServerTableEntry_t) );

				memUtils_memCopy( (uint8_t*)&pServerLCETable->eventId, (uint8_t*)&pLCETable->eventId, 0x04 );

				memUtils_memCopy( (uint8_t*)&pServerLCETable->deviceClass, (uint8_t*)&pLCETable->deviceClass, 0x02 );

				pServerLCETable->utilityEnrollmentGroup = pLCETable->utilityEnrollmentGroup;

				memUtils_memCopy( (uint8_t*)&pServerLCETable->startTime,
						(uint8_t*)&pLCETable->startTime, 0x04 );

				memUtils_memCopy( (uint8_t*)&pServerLCETable->duration,
								(uint8_t*)&pLCETable->duration, 0x02 );

				pServerLCETable->criticalityLevel = pLCETable->criticalityLevel;

				pServerLCETable->coolingTempOffset = pLCETable->coolingTempOffset;

				pServerLCETable->heatingTempOffset = pLCETable->heatingTempOffset;

				memUtils_memCopy( (uint8_t*)&pServerLCETable->coolingTempSetPoint,
											(uint8_t*)&pLCETable->coolingTempSetPoint, 0x02 );

				memUtils_memCopy( (uint8_t*)&pServerLCETable->heatingTempSetPoint,
										(uint8_t*)&pLCETable->heatingTempSetPoint, 0x02 );

				pServerLCETable->avgLoadPercentage = pLCETable->avgLoadPercentage;

				pServerLCETable->dutyCycle = pLCETable->dutyCycle;

				pServerLCETable->eventControl = pLCETable->eventControl;

				if ( ! pServerLCETable->startTime ) {
					pServerLCETable->startTime = ZCL_CallBackGetCurrentTime();
				}
			status = g_ZCL_Success_c;

		}
    }
   	return status;

}
/************************************************************************************/
void App_removeAllLCETableEntires(void)
{
	uint8_t index;
	  for ( index = 0; index < g_DRLC_MAX_EVENT_TABLE_SIZE_c; index++ )
	  {

	      memUtils_memSet ( (uint8_t*)&serverLCETable[index],
	                g_ZCL_INVALID_DATA_c, sizeof(LCEServerTableEntry_t) );
	  }
	  return;
}

/************************************************************************************/
uint8_t  App_removeAnEntryFrmLCETable(uint32_t evtID)
{
	LCEServerTableEntry_t *pserverLCETable = g_NULL_c;
	uint8_t index;
	uint8_t status = g_ZCL_Failure_c;
	for ( index = 0; index < g_DRLC_MAX_EVENT_TABLE_SIZE_c; index++ )
	{
		pserverLCETable = AppgetLCETableEntry( index );
		if ((pserverLCETable != g_NULL_c)&& ( pserverLCETable->eventId == evtID ) )
		{

			memUtils_memSet ( (uint8_t*)&serverLCETable[index],
					g_ZCL_INVALID_DATA_c, sizeof(LCEServerTableEntry_t) );
			status = g_ZCL_Success_c;
			break;
		}
	}
	if(index >= g_DRLC_MAX_EVENT_TABLE_SIZE_c)
	{
		status = g_ZCL_Not_Found_c;
	}
	return status;
}
/************************************************************************************/
uint8_t App_drlcCount(void)
{
	uint8_t count = 0x00;
	count = AppgetScheduledLCECount();
	return count;
}
#endif
/************************************************************************************/
