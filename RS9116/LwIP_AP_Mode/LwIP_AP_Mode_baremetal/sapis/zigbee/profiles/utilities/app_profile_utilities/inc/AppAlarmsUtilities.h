#ifndef _APPALARMSUTILITIES_H_
#define _APPALARMSUTILITIES_H_
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Interface.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_ALARMS_TABLE_SIZE_c                               0x03
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct AlarmConditionsTable_Tag
{
   uint16_t clusterId;
   uint16_t attributeId;
   uint16_t thresholdAttributeId;
   uint16_t alarmMaskAttributeId;
   uint16_t DwellTripPointAttributeId;
   uint8_t alarmCode;
   uint8_t alarmMaskData;
   uint8_t compareToMaxOrMinThreshold;
}__attribute__((__packed__))AlarmConditionsTable_t;

typedef struct App_DwellTripPointTime_Tag
{
    uint8_t DwellTripPoint;
    uint16_t DwellTripPointTimeOut;
    uint32_t DwellTripPointStartTime;
} __attribute__((__packed__))App_DwellTripPointTime_t;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
extern App_DwellTripPointTime_t appPowerConfigMainsDwellTrip;
extern App_DwellTripPointTime_t appTempConfigMainsDwellTrip;
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
void UpdateAlarmTable ( int16_t clusterId,
                        uint8_t alarmCode, uint32_t timeStamp, uint8_t event );

void App_AlarmTableInit( void );
void App_AlarmTask( void );

#endif              /* ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
void Handle_Alarm_Cluster_Events( ZCL_ActionHandler_t pActionHandler );
#endif          /* ( g_ALARMS_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                      /* _APPALARMSUTILITIES_H_ */
