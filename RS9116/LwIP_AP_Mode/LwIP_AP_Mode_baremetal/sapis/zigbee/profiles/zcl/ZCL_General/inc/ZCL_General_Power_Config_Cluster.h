/******************************************************************************
* ZCL_General_Power_Config_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Power configuration cluster attribute and cmmnd information
******************************************************************************/
#ifndef _ZCL_GENERAL_POWER_CONFIG_CLUSTER_H_
#define _ZCL_GENERAL_POWER_CONFIG_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/*Attributes for determining more detailed information
about a device�s power source(s), and for configuring
under/over voltage alarms.0x0001*/
#define g_POWER_CONFIGURATION_CLUSTER_c                        0x0001

/*Attributes for Power Configuration Cluster*/
#define g_MAINS_VOLTAGE_ATTRIBUTE_c                            0x0000
#define g_MAINS_FREQUENCY_ATTRIBUTE_c                          0x0001
#define g_MAINS_ALARM_MASK_ATTRIBUTE_c                         0x0010
#define g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_c              0x0011
#define g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_c              0x0012
#define g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_c           0x0013
#define g_BATTERY_VOLATGE_ATTRIBUTE_c                          0x0020
#define g_BATTERY_MANUFACTURER_ATTRIBUTE_c                     0x0030
#define g_BATTERY_SIZE_ATTRIBUTE_c                             0x0031
#define g_BATTERY_AHR_RATING_ATTRIBUTE_c                       0x0032
#define g_BATTERY_QUANTITY_ATTRIBUTE_c                         0x0033
#define g_BATTERY_RATED_VOLATGE_ATTRIBUTE_c                    0x0034
#define g_BATTERY_ALARM_MASK_ATTRIBUTE_c                       0x0035
#define g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_c            0x0036



typedef enum Power_Alarm_Codes
{
   g_Mains_Voltage_Too_Low_c,
   g_Mains_Voltage_Too_High_c ,
   g_Battery_Voltage_Too_Low_c = 0x10
}Power_Alarm_Codes;
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

extern const ZCL_Cluster_Info_t ga_Power_Configuration_Server_Cluster_Info;
extern const ZCL_Cluster_Info_t ga_Power_Configuration_Client_Cluster_Info;

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
Power Configuration Cluster structure - attributes
Field: mains_voltage - Actual RMS voltage currently applied to the device
Field: mains_frequency - frequency of mains  currently applied to the device
Filed: mains_alarm_mask - specifices which alarms may be generated during
               certain situations
Field: mains_voltage_min_threshold - gives lower alarm threshold
Field: mains_voltage_max_threshold - gives higher alarm thershold
Field: mains_voltage_dwell_trip_point - specifies the
               length of time, in seconds that the value of MainsVoltage
               may exist beyond either of its thresholds before an alarm
               is generated.
Field: battery_voltage - gives the actual battery voltage
Field: battery_size - specifies the type of battery
                        being used by the device.
Field: battery_ahr_rating - specifies the Ampere-hour rating of the battery
Field: battery_quantity - specifies the number of battery cells used to
                             power the device.
Field: battery_rated_volatge - specifies the rated voltage of the battery
                              being used in the device.
Field: battery_alarm_mask - specifies which mains alarms may be generated.
Field: battery_voltage_min_threshold - specifies the low voltage alarm
                  threshold
Field: battery_manufacturer - maximum of 16 bytes in length and specifies
               the name of the battery manufacturer as a ZigBee character
               string.
-----------------------------------------------------------------------------*/
typedef struct Power_Config_Cluster_Tag
{
#if ( g_MAINS_VOLTAGE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_mains_voltage[2];
#endif
#if ( g_MAINS_FREQUENCY_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t mains_frequency;
#endif
#if ( g_MAINS_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t mains_alarm_mask;
#endif
#if ( g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_mains_voltage_min_threshold[2];
#endif
#if ( g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_mains_voltage_max_threshold[2];
#endif
#if ( g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_mains_voltage_dwell_trip_point[2];
#endif
#if ( g_BATTERY_VOLATGE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t battery_voltage;
#endif
#if ( g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_battery_manufacturer[16];
#endif
#if ( g_BATTERY_SIZE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t battery_size;
#endif
#if ( g_BATTERY_AHR_RATING_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_battery_ahr_rating[2];
#endif
#if ( g_BATTERY_QUANTITY_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t battery_quantity;
#endif
#if ( g_BATTERY_RATED_VOLATGE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t battery_rated_volatge;
#endif
#if ( g_BATTERY_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t battery_alarm_mask;
#endif
#if ( g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t battery_voltage_min_threshold;
#endif
}__attribute__((__packed__))Power_Config_Cluster_t;

#endif                              /* g_POWER_CONFIGURATION_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
     Function Prototypes
-------------------------------------------------------------------------------*/

uint8_t ZCL_Handle_PowerConfig_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*-------------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_GENERAL_POWER_CONFIG_CLUSTER_H_*/
