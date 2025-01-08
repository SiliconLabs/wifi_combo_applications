/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include <stdio.h>
#include "stack_common.h"
#include "ZigBeeDataTypes.h"
#include "ZCL_Common.h"
//#include "sw_timer.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "AppTimerUtilities.h"
#if( g_ALARMS_CLUSTER_ENABLE_d == 1 )
#include "AppAlarmsUtilities.h"
#include "ApplicationThinLayer.h"
#include "ZCL_General_Alarms_Cluster.h"
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Power_Config_Cluster.h"
#endif
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Device_Temp_Config_Cluster.h"
#endif

int16_t rsi_zigb_send_broadcast_data(ZigBeeAPSDEDataRequest_t * pAPSDERequest);
uint8_t APS_APSDE_request(uint8_t messageIndex,APSDE_Data_Request_t *p_apsde_data_request );

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_INVALID_ATTRIBUTE_ID_c                          0xFFFF
#define m_INVALID_CLUSTER_ID_c                            0xFFFF
#define m_INVALID_TIMESTAMP_c                             0xFFFFFFFF
#define m_INDIRECT_ADDR_MODE_c                            0x00
#define m_ALARM_CMD_LENGTH_c                              0x06
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
const  AlarmConditionsTable_t alarmConditionTable[] =
{
#if( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
#if ( g_MAINS_VOLTAGE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_POWER_CONFIGURATION_CLUSTER_c,
      g_MAINS_VOLTAGE_ATTRIBUTE_c,
      g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_c,
      g_MAINS_ALARM_MASK_ATTRIBUTE_c,
      g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_c,
      g_Mains_Voltage_Too_Low_c,
      0x01,
      g_Min_Value_Compared_c
    },
    {
      g_POWER_CONFIGURATION_CLUSTER_c,
      g_MAINS_VOLTAGE_ATTRIBUTE_c,
      g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_c,
      g_MAINS_ALARM_MASK_ATTRIBUTE_c,
      g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_c,
      g_Mains_Voltage_Too_High_c,
      0x02,
      g_Max_Value_Compared_c
   },
#endif          /* ( g_MAINS_VOLTAGE_ATTRIBUTE_ENABLE_d == 1 ) */
#if ( g_BATTERY_VOLATGE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_POWER_CONFIGURATION_CLUSTER_c,
      g_BATTERY_VOLATGE_ATTRIBUTE_c,
      g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_c,
      g_BATTERY_ALARM_MASK_ATTRIBUTE_c,
      m_INVALID_ATTRIBUTE_ID_c,
      g_Battery_Voltage_Too_Low_c,
      0x01,
      g_Min_Value_Compared_c
   },
#endif          /* ( g_BATTERY_VOLATGE_ATTRIBUTE_ENABLE_d == 1 ) */
#endif          /* ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
   {
      g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
      g_CURRENT_TEMPERATURE_ATTRIBUTE_c,
      g_LOW_TEMP_THRESHOLD_ATTRIBUTE_c,
      g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_c,
      g_LOW_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_c,
      g_Temp_Too_Low_c,
      0x01,
      g_Min_Value_Compared_c
   },
   {
      g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
      g_CURRENT_TEMPERATURE_ATTRIBUTE_c,
      g_HIGH_TEMP_THRESHOLD_ATTRIBUTE_c,
      g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_c,
      g_HIGH_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_c,
      g_Temp_Too_High_c,
      0x02,
      g_Max_Value_Compared_c
   },
#endif          /* ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */
   { g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c,g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c }
};

/* This is alarm table is updated when alarm condition rised */
ALARM_AlarmTable_t AlarmEntries[ g_ALARMS_TABLE_SIZE_c ] =
{
    { Alarm_Clear,  g_ZCL_INVALID_DATA_c,  m_INVALID_CLUSTER_ID_c,  m_INVALID_TIMESTAMP_c },
    { Alarm_Clear,  g_ZCL_INVALID_DATA_c,  m_INVALID_CLUSTER_ID_c,  m_INVALID_TIMESTAMP_c }
};

App_DwellTripPointTime_t appPowerConfigMainsDwellTrip = { 0x00, 0x0000, 0x00000000 };
App_DwellTripPointTime_t appTempConfigMainsDwellTrip  = { 0x00, 0x0000, 0x00000000 };
uint8_t nextIndexToBeUpdated = g_ZERO_c;

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/


static void GenerateAlarm( uint16_t clusterId, uint8_t alarmCode );

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

void App_AlarmTableInit( void )
{
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
    nextIndexToBeUpdated = g_ZERO_c;
    memUtils_memSet( (uint8_t*)&appPowerConfigMainsDwellTrip, g_ZERO_c, sizeof(App_DwellTripPointTime_t));
    memUtils_memSet( (uint8_t*)&appTempConfigMainsDwellTrip, g_ZERO_c, sizeof(App_DwellTripPointTime_t));
    memUtils_memSet( (uint8_t*)AlarmEntries, g_ZCL_INVALID_DATA_c,
              sizeof(ALARM_AlarmTable_t) * g_ALARMS_TABLE_SIZE_c );
#endif
}

/*******************************************************************************************/

void App_AlarmTask( void )
{

#if ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
    if ( appPowerConfigMainsDwellTrip.DwellTripPoint == g_TRUE_c ) {
        if ( App_getDifferance( App_getcurrentTick(),
                               appPowerConfigMainsDwellTrip.DwellTripPointStartTime ) >=
             appPowerConfigMainsDwellTrip.DwellTripPointTimeOut ) {

                appPowerConfigMainsDwellTrip.DwellTripPoint = g_FALSE_c;
                ZCL_Callback_CheckAlarmCondition ( ZCL_Callback_Get_Device_EndPoint(),
                                                  g_MAINS_VOLTAGE_ATTRIBUTE_c,
                                                  &ga_Power_Configuration_Server_Cluster_Info,
                                                  g_After_Timer_Expiry_c );
        }
    }
#endif          /* ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
    if ( appTempConfigMainsDwellTrip.DwellTripPoint == g_TRUE_c ) {
        if ( App_getDifferance( App_getcurrentTick(),
                               appTempConfigMainsDwellTrip.DwellTripPointStartTime ) >=
               appTempConfigMainsDwellTrip.DwellTripPointTimeOut ) {

               appTempConfigMainsDwellTrip.DwellTripPoint = g_FALSE_c;
               ZCL_Callback_CheckAlarmCondition ( ZCL_Callback_Get_Device_EndPoint(),
                                   g_CURRENT_TEMPERATURE_ATTRIBUTE_c,
                                   &ga_Device_Temp_Config_Server_Cluster_Info,
                                   g_After_Timer_Expiry_c );
        }
    }
#endif        /* ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */
}

/*******************************************************************************************/

void Handle_Alarm_Cluster_Events( ZCL_ActionHandler_t pActionHandler )
{
    uint16_t clusterId;

    switch ( pActionHandler.event.eventId )
    {
        case g_Alarm_Occurred_Event_c:
            break;

        case g_Reset_Alarm_Command_Event_c:

            memUtils_memCopy( (uint8_t*)&clusterId, pActionHandler.event.pEventData + 1, 0x02 );
            UpdateAlarmTable ( clusterId,
                               pActionHandler.event.pEventData[0],
                               0xFFFFFFFF,
                               g_Reset_Alarm_Command_Event_c );
            break;

        case g_Reset_All_Alarms_Command_Event_c:
            UpdateAlarmTable ( g_TWO_BYTE_INVALID_VALUE_c,
                               g_ZCL_INVALID_DATA_c,
                               0xFFFFFFFF,
                               g_Reset_All_Alarms_Command_Event_c );
            break;
        case g_Get_Alarm_Command_Event_c:
            break;
        case g_Reset_Alarm_Log_Command_Event_c:
            UpdateAlarmTable ( g_TWO_BYTE_INVALID_VALUE_c,
                               g_ZCL_INVALID_DATA_c,
                               0xFFFFFFFF,
                               g_Reset_Alarm_Log_Command_Event_c );
            break;

        default :
            break;
    }

}

/*******************************************************************************/

void ZCL_Callback_CheckAlarmCondition (  uint8_t endpoint,
                                         uint16_t attributeId,
                                         const ZCL_Cluster_Info_t *pZCLClusterInfo,
                                         uint8_t callFrom )
{
    uint8_t i;
    uint16_t index;
    int16_t receivedAttributeDataValue=0x0000;
    int16_t thresholdAttributeDataValue=0x0000;
    uint8_t attributeDataTypeLength;
    uint8_t maskValue;
    uint32_t DwellTripPointTimeOut;

    for ( i= 0x00; i < sizeof( alarmConditionTable)/ sizeof( AlarmConditionsTable_t ); i++ ) {
        /* It checks the given attributeId and ClusterID with its alarmConditionTable */
        if( ( alarmConditionTable[i].clusterId == pZCLClusterInfo->cluster_id ) &&
            ( alarmConditionTable[i].attributeId == attributeId ) ) {

            if ( ( g_ZCL_Success_c == ZCL_Get_AttributeInfo_Index (
                                              pZCLClusterInfo->num_attrib_info_present,
                                              attributeId,
                                              &index,
                                              pZCLClusterInfo->p_array_of_attrib_info ) ) ) {

                App_ReadWriteAttribute(  endpoint,
                                               pZCLClusterInfo->cluster_id,
                                               attributeId,
                                               (uint8_t*)&receivedAttributeDataValue,
                                               g_Read_Attribute_Data_c );

                App_ReadWriteAttribute(  endpoint,
                                               pZCLClusterInfo->cluster_id,
                                               alarmConditionTable[i].thresholdAttributeId,
                                               (uint8_t*)&thresholdAttributeDataValue,
                                               g_Read_Attribute_Data_c );

				if( pZCLClusterInfo->p_array_of_attrib_info->data_type ==g_Unsigned_Sixteen_Bit_Integer_Data_c)
				{
				  	thresholdAttributeDataValue = (uint16_t)thresholdAttributeDataValue;
					receivedAttributeDataValue  = (uint16_t)receivedAttributeDataValue;
				}


                /* It checks the received attribute data with its threshold value */
                if( ( ( receivedAttributeDataValue < thresholdAttributeDataValue ) &&
                      ( alarmConditionTable[i].compareToMaxOrMinThreshold == g_Min_Value_Compared_c ) )||
                    ( ( receivedAttributeDataValue > thresholdAttributeDataValue ) &&
                      ( alarmConditionTable[i].compareToMaxOrMinThreshold == g_Max_Value_Compared_c ) ) ) {

                    App_ReadWriteAttribute(  endpoint,
                                                   pZCLClusterInfo->cluster_id,
                                                   alarmConditionTable[i].alarmMaskAttributeId,
                                                   &maskValue,
                                                   g_Read_Attribute_Data_c );

                    /* If read maskValue matches with alarmConditionTableEntry alarmMask value,
                       then we need to add the alarm entry in its alarm table and also we need
                       to generate an alarm */
                    if( ( maskValue & alarmConditionTable[i].alarmMaskData ) ==
                                                        alarmConditionTable[i].alarmMaskData ) {

                        if ( callFrom == g_After_Attribute_Change_c ) {
                                DwellTripPointTimeOut = 0x00000000;
                                uint32_t tempDwellTripPointTimeOut = 0xFFFFFFFF;

                                ZCL_Get_AttributeInfo_Index ( pZCLClusterInfo->num_attrib_info_present,
                                                              attributeId,
                                                              &index,
                                                              pZCLClusterInfo->p_array_of_attrib_info );

                                attributeDataTypeLength = ZCL_Get_Data_length(
                                       ( pZCLClusterInfo->p_array_of_attrib_info + index )->data_type );

                                App_ReadWriteAttribute(  endpoint,
                                                               pZCLClusterInfo->cluster_id,
                                                               alarmConditionTable[i].DwellTripPointAttributeId,
                                                               (uint8_t*)&DwellTripPointTimeOut,
                                                               g_Read_Attribute_Data_c );

                                if ( g_MEMORY_MATCH_c == ( memUtils_memCompare( (uint8_t*)&DwellTripPointTimeOut,
                                                                        (uint8_t*)&tempDwellTripPointTimeOut,
                                                                         attributeDataTypeLength ) ) ) {
                                    break;
                                }
                                else if ( DwellTripPointTimeOut != 0x00000000 ) {
#if ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
                                    if ( pZCLClusterInfo->cluster_id == g_POWER_CONFIGURATION_CLUSTER_c ) {
                                        appPowerConfigMainsDwellTrip.DwellTripPoint = 0x01;
                                        appPowerConfigMainsDwellTrip.DwellTripPointTimeOut = DwellTripPointTimeOut;
                                        appPowerConfigMainsDwellTrip.DwellTripPointStartTime = App_getcurrentTick();
                                    }
#endif   /* ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */
#if( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
                                    if ( pZCLClusterInfo->cluster_id == g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c ) {
                                        appTempConfigMainsDwellTrip.DwellTripPoint = 0x01;
                                        appTempConfigMainsDwellTrip.DwellTripPointTimeOut = DwellTripPointTimeOut;
                                        appTempConfigMainsDwellTrip.DwellTripPointStartTime = App_getcurrentTick();

                                    }
#endif   /* ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */
                                    return;
                                }
                        }
                        UpdateAlarmTable ( alarmConditionTable[i].clusterId,
                                           alarmConditionTable[i].alarmCode,
                                           m_INVALID_TIMESTAMP_c,
                                           g_Alarm_Occurred_Event_c );

                        GenerateAlarm( alarmConditionTable[i].clusterId,
                                       alarmConditionTable[i].alarmCode );
                    }
                    return;
               }
            }
        }
    }
}

/*******************************************************************************************/
static void GenerateAlarm( uint16_t clusterId, uint8_t alarmCode )
{

    APSDE_Data_Request_t *p_apsde_data_request;
    uint8_t buffindex;

    buffindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

   if( g_NO_AVAILABLE_BUFFER_c != buffindex ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 73 */
        allocatedbuffer[buffindex]= 73;
        freebuffer[buffindex][0]= 73;
        freebuffer[buffindex][1]= 0;
#endif
        p_apsde_data_request = ( APSDE_Data_Request_t * )buffMgmt_getBufferPointer( buffindex );
        *((uint8_t*)p_apsde_data_request) = g_APSDE_Data_Request_c;
        p_apsde_data_request = (APSDE_Data_Request_t *)&(((APSDE_Request_t *)p_apsde_data_request)->
                                                       APSDE_Request_Primitive.apsde_data_request);

        p_apsde_data_request->dest_addr_mode = m_INDIRECT_ADDR_MODE_c;
        p_apsde_data_request->profile_id     = g_APP_PROFILE_ID_c;
        p_apsde_data_request->src_endpoint   = ZCL_Callback_Get_Device_EndPoint();
        p_apsde_data_request->cluster_id     = g_ALARMS_CLUSTER_c;
        p_apsde_data_request->asdulength     = m_ALARM_CMD_LENGTH_c;
        p_apsde_data_request->asdu[0]        = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
                                               g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                               g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        p_apsde_data_request->asdu[1]        = App_GetTransSeqNumber();
        p_apsde_data_request->asdu[2]        = g_Alarm_Command_c;
        p_apsde_data_request->asdu[3]        = alarmCode;
        memUtils_memCopy( &p_apsde_data_request->asdu[4], (uint8_t*)&clusterId, g_CLUSTER_ID_LENGTH_c );
        p_apsde_data_request->txoptions      =  App_getClusterSecurity( g_ALARMS_CLUSTER_c );
        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;

       // APS_APSDErequest( buffindex );
          APS_APSDE_request(buffindex,p_apsde_data_request );
   }
}
/*******************************************************************************************/

void UpdateAlarmTable ( int16_t clusterId, uint8_t alarmCode,
                        uint32_t timeStamp, uint8_t event )
{
    uint16_t alarmCount = 0x0000;
    uint8_t index;

    App_ReadWriteAttribute(  ZCL_Callback_Get_Device_EndPoint(),
                                   g_ALARMS_CLUSTER_c,
                                   g_ALARM_COUNT_ATTRIBUTE_c,
                                   (uint8_t*)&alarmCount,
                                   g_Read_Attribute_Data_c );

    /* This is for checking duplicate alarmEntry.If exists it updates with input
       values and returns from the function */
    for ( index = 0x00; index < g_ALARMS_TABLE_SIZE_c ; index++ ) {
        if( ( AlarmEntries[index].clusterId == clusterId ) &&
            ( AlarmEntries[index].alarmCode == alarmCode ) &&
            ( event == g_Alarm_Occurred_Event_c ) ) {

            AlarmEntries[index].timeStamp = timeStamp;
            AlarmEntries[index].status = Alarm_Active;

            return;
        }
    }
    /* This is for adding alarmEntry in alarmTable */
    for ( index = 0x00; index < g_ALARMS_TABLE_SIZE_c; index++ ) {
        /* For adding alarmEntry.It increments alarm count by one */
        if((AlarmEntries[index].status == Alarm_Clear) && (event == g_Alarm_Occurred_Event_c)) {

            AlarmEntries[index].clusterId = clusterId;
            AlarmEntries[index].alarmCode = alarmCode;
            AlarmEntries[index].timeStamp = timeStamp;
            AlarmEntries[index].status = Alarm_Active;
            alarmCount++;

            break;
        }
        /* If alarmCount reaches max value,then we need to replace earliest time stamp value with
          the given input values */
        if( ( alarmCount == g_ALARMS_TABLE_SIZE_c ) && ( g_Alarm_Occurred_Event_c == event) ) {
           uint32_t timeStamp = 0xFFFFFFFF;
           uint8_t i;
           for ( i = 0x00; i < g_ALARMS_TABLE_SIZE_c; i++ ) {
                if( AlarmEntries[i].timeStamp < timeStamp ) {
                    timeStamp = AlarmEntries[i].timeStamp;
                    index = i;
                }
            }
            AlarmEntries[index].clusterId = clusterId;
            AlarmEntries[index].alarmCode = alarmCode;
            AlarmEntries[index].timeStamp = timeStamp;
            AlarmEntries[index].status = Alarm_Active;
            return;
        }
        /* It makes the alarmEntry Status with Inactive, if entry exists in alarmTable */
        if( ( event == g_Reset_Alarm_Command_Event_c ) &&
            ( AlarmEntries[index].clusterId == clusterId )&&
            ( AlarmEntries[index].alarmCode == alarmCode ) ) {

            AlarmEntries[index].status = Alarm_Inactive;

            return;
        }
        /* It makes the all alarmEntries Status with Inactive */
        if( event == g_Reset_All_Alarms_Command_Event_c ) {
            AlarmEntries[index].status = Alarm_Inactive;
        }
        /*It Clears the all alarm entries*/
        if( event == g_Reset_Alarm_Log_Command_Event_c ) {
            AlarmEntries[index].clusterId = 0xFFFF;
            AlarmEntries[index].alarmCode = 0xFF;
            AlarmEntries[index].timeStamp = 0xFFFFFFFF;
            AlarmEntries[index].status = Alarm_Clear;
            alarmCount = 0x0000;
        }
    }
    App_ReadWriteAttribute(  ZCL_Callback_Get_Device_EndPoint(),
                                   g_ALARMS_CLUSTER_c,
                                   g_ALARM_COUNT_ATTRIBUTE_c,
                                   (uint8_t*)&alarmCount,
                                   g_Write_Attribute_Data_c );

}

/*******************************************************************************************/

uint8_t ZCL_CallBackCreateAlarmResponse( uint8_t endPoint, uint8_t *pResponseAsdu )
{

    uint16_t alarmCount = 0x0000;

    App_ReadWriteAttribute(  endPoint,
						     g_ALARMS_CLUSTER_c,
						     g_ALARM_COUNT_ATTRIBUTE_c,
						     (uint8_t*)&alarmCount,
						    g_Read_Attribute_Data_c );
    if( alarmCount == 0x0000 ) {
        *pResponseAsdu = g_ZCL_Not_Found_c;
        return 0x01;
    }
    else{
        uint32_t timeStamp = 0xFFFFFFFF;
        uint8_t index = 0xFF;
        uint8_t i;
        /* Find out entry Least time stamp Alarm index */
        for ( i = 0x00; i < sizeof(AlarmEntries)/sizeof(ALARM_AlarmTable_t); i++ ) {
            if( AlarmEntries[i].timeStamp <= timeStamp && AlarmEntries[i].clusterId != 0XFFFF) {
                timeStamp = AlarmEntries[i].timeStamp;
                index = i;
            }
        }
        *pResponseAsdu++ = g_ZCL_Success_c;

         memUtils_memCopy ( pResponseAsdu,  (uint8_t*)&AlarmEntries[index].alarmCode,
                    sizeof(ALARM_AlarmTable_t) - 0x01 );

         memUtils_memSet ( (uint8_t*)&AlarmEntries[index], 0xFF, sizeof(ALARM_AlarmTable_t) );

        alarmCount -= 0x01;
        App_ReadWriteAttribute(  endPoint,
                                       g_ALARMS_CLUSTER_c,
                                       g_ALARM_COUNT_ATTRIBUTE_c,
                                       (uint8_t*)&alarmCount,
                                       g_Write_Attribute_Data_c );

        return sizeof( ALARM_AlarmTable_t );
    }
}

#endif          /* g_ALARMS_SERVER_CLUSTER_ENABLE_d */

#endif          /* g_ALARMS_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
