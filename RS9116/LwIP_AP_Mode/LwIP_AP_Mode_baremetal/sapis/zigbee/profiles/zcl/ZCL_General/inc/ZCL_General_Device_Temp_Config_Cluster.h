/******************************************************************************
* ZCL_General_Device_Temp_Config_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Device temp cluster attribute and cmmnd information
******************************************************************************/

#ifndef _ZCL_GENERAL_DEVICE_TEMP_CONFIG_CLUSTER_H_
#define _ZCL_GENERAL_DEVICE_TEMP_CONFIG_CLUSTER_H_

#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*Attributes for determining information about a device�s internal temperature,
and for configuring under/over temperature alarms.0x0002*/
#define g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c                0x0002

/* Attributes for Device Temperature Cluster */
#define g_CURRENT_TEMPERATURE_ATTRIBUTE_c                           0x0000
#define g_MIN_TEMP_EXPERIENCED_ATTRIBUTE_c                          0x0001
#define g_MAX_TEMP_EXPERIENCED_ATTRIBUTE_c                          0x0002
#define g_OVER_TO_TOTAL_DWELL_ATTRIBUTE_c                           0x0003
#define g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_c                        0x0010
#define g_LOW_TEMP_THRESHOLD_ATTRIBUTE_c                            0x0011
#define g_HIGH_TEMP_THRESHOLD_ATTRIBUTE_c                           0x0012
#define g_LOW_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_c                     0x0013
#define g_HIGH_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_c                    0x0014


typedef enum Temp_Alarm_Codes
{
   g_Temp_Too_Low_c,
   g_Temp_Too_High_c
}Temp_Alarm;
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Device_Temp_Config_Server_Cluster_Info;
#endif

#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Device_Temp_Config_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
Device Temperature Configuration cluster structure - attributes
Field: current_temperature - specifies the current internal temperature
Field: min_temp_experienced - specifies the minimum internal temperature
Filed: max_temp_experienced - specifies the maximum internal temperature
Field: over_to_total_dewll - specifies the length of time, in hours,
         the device has spent above the temperature specified by the
         HighTempThreshold attribute.
Field: device_temp_alarm_mask - specifies which alarms may be generated
Field: low_temp_threshold - specifies the lower alarm threshold
Field: high_temp_threshold - specifies the higher alarm threshold
Field: low_temp_dwell_trip_point - specifies the length
         of time, in seconds, that the value of CurrentTemperature may
         exist below LowTempThreshold before an alarm is generated.
Field: high_temp_dwell_trip_point - specifies the
         length of time, in seconds, that the value of CurrentTemperature
         may exist above HighTempThreshold before an alarm is generated.
-----------------------------------------------------------------------------*/
typedef struct Device_Temp_Config_Cluster_Tag
{
    int8_t a_current_temperature[2];
#if ( g_MIN_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d == 1 )
    int8_t a_min_temp_experienced[2];
#endif
#if ( g_MAX_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d == 1 )
    int8_t a_max_temp_experienced[2];
#endif
#if ( g_OVER_TO_TOTAL_DWELL_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_over_to_total_dewll[2];
#endif
#if ( g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t device_temp_alarm_mask;
#endif
#if ( g_LOW_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    int8_t a_low_temp_threshold[2];
#endif
#if ( g_HIGH_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    int8_t a_high_temp_threshold[2];
#endif
#if ( g_LOW_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t low_temp_dwell_trip_point[3];
#endif
#if ( g_HIGH_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t high_temp_dwell_trip_point[3];
#endif
}__attribute__((__packed__))Device_Temp_Config_Cluster_t;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/

uint8_t ZCL_Handle_TempConfig_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif              /* g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d */

#endif                          /*  _ZCL_GENERAL_DEVICE_TEMP_CONFIG_CLUSTER_H_ */
