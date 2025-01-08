#ifndef _ZCL_FUNCTIONALITY_H_
#define _ZCL_FUNCTIONALITY_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

#include "stack_functionality.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define g_BASIC_CLUSTER_ENABLE_d                                        1
#define g_POWER_CONFIGURATION_CLUSTER_ENABLE_d                          1
#define g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d             0
#define g_IDENTIFY_CLUSTER_ENABLE_d                                     1
#define g_ALARMS_CLUSTER_ENABLE_d                                       1
#define g_GROUPS_CLUSTER_ENABLE_d                                       0
#define g_SCENES_CLUSTER_ENABLE_d										0
#define g_LEVEL_CONTROL_CLUSTER_ENABLE_d                                0
#define g_ON_OFF_CLUSTER_ENABLE_d                                       0
#define g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d                  0
#define g_RSSI_LOCATION_CLUSTER_ENABLE_d                                0
#define g_TIME_CLUSTER_ENABLE_d                                         1
#define g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d                     0
#define g_FAN_CONTROL_CLUSTER_ENABLE_d                                  0
#define g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d               0
#define g_THERMOSTAT_CLUSTER_ENABLE_d                                   0
#define g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d             0
#define g_COLOR_CONTROL_CLUSTER_ENABLE_d                                0
#define g_BALLAST_CONFIGURATION_CLUSTER_ENABLE_d                        0
#define g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d                             0
#define g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d                    0
#define g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d                      0
#define g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d                            0
#define g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d                         0
#define g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d                0
#define g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d                      0
#define g_IAS_ZONE_CLUSTER_ENABLE_d                                     0
#define g_IAS_ACE_CLUSTER_ENABLE_d                                      0
#define g_IAS_ZONE_CLUSTER_ENABLE_d                                     0
#define g_IAS_WD_CLUSTER_ENABLE_d                                       0
#define g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d                          0
#define g_SIMPLE_METERING_CLUSTER_ENABLE_d                              1
#define g_MESSAGING_CLUSTER_ENABLE_d                                    1
#define g_PRICE_CLUSTER_ENABLE_d                                        1
#define g_DRLC_CLUSTER_ENABLE_d                                         1
#define g_COMMISSIONING_CLUSTER_ENABLE_d                                1
#define g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d							0
#define g_OTA_BOOTLOAD_CLUSTER_ENABLE_d                                 0
#if(g_SE_PROFILE_1_1_d == 1)
#define g_PREPAYMENT_CLUSTER_ENABLE_d                                   1
#define g_TUNNELING_CLUSTER_ENABLE_d                                    1
#define g_MULTIPLE_ESI_DISCOVERY_ENABLE_d                               0
#define g_GENERIC_TUNNEL_CLUSTER_ENABLE_d								1
#endif     /* g_SE_PROFILE_1_1_d*/

#if(g_SE_PROFILE_1_2_d == 1)

#define g_TOU_CALENDER_CLUSTER_ENABLE_d                                 1

#endif    /* g_SE_PROFILE_1_2_d*/

#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
    #define g_BASIC_SERVER_CLUSTER_ENABLE_d                                 1
    #define g_BASIC_CLIENT_CLUSTER_ENABLE_d                                 1
#else
    #define g_BASIC_SERVER_CLUSTER_ENABLE_d                                 0
    #define g_BASIC_CLIENT_CLUSTER_ENABLE_d                                 0
#endif						/* g_BASIC_CLUSTER_ENABLE_d */

#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #define g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d                   1
    #define g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d                   1
#else
    #define g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d                   0
    #define g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d                   0
#endif						/* g_POWER_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #define g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d  	1
    #define g_DEVICE_TEMPERATURE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d    	1
#else
    #define g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d  	0
    #define g_DEVICE_TEMPERATURE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d   	0
#endif						/* g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
    #define g_IDENTIFY_SERVER_CLUSTER_ENABLE_d                           	1
    #define g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d                            	1
#else
    #define g_IDENTIFY_SERVER_CLUSTER_ENABLE_d                            	0
    #define g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d                            	0
#endif						/* g_IDENTIFY_CLUSTER_ENABLE_d */

#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
    #define g_ALARMS_SERVER_CLUSTER_ENABLE_d                                1
    #define g_ALARMS_CLIENT_CLUSTER_ENABLE_d                                1
#else
    #define g_ALARMS_SERVER_CLUSTER_ENABLE_d                                0
    #define g_ALARMS_CLIENT_CLUSTER_ENABLE_d                                0
#endif						/* g_ALARMS_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
    #define g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d                         1
    #define g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d                         0
#else
    #define g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d                         0
    #define g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d                         0
#endif						/* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */

#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
    #define g_GROUPS_SERVER_CLUSTER_ENABLE_d                                0
    #define g_GROUPS_CLIENT_CLUSTER_ENABLE_d                                1
#else
    #define g_GROUPS_SERVER_CLUSTER_ENABLE_d                                0
    #define g_GROUPS_CLIENT_CLUSTER_ENABLE_d                                0
#endif						/* g_GROUPS_CLUSTER_ENABLE_d */

#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
    #define g_SCENES_SERVER_CLUSTER_ENABLE_d                                1
    #define g_SCENES_CLIENT_CLUSTER_ENABLE_d                                0
#else
    #define g_SCENES_SERVER_CLUSTER_ENABLE_d                                0
    #define g_SCENES_CLIENT_CLUSTER_ENABLE_d                                0
#endif						/* g_SCENES_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLUSTER_ENABLE_d == 1 )
    #define g_ON_OFF_SERVER_CLUSTER_ENABLE_d                                1
    #define g_ON_OFF_CLIENT_CLUSTER_ENABLE_d                                0
#else
    #define g_ON_OFF_SERVER_CLUSTER_ENABLE_d                                0
    #define g_ON_OFF_CLIENT_CLUSTER_ENABLE_d                                0
#endif						/* g_ON_OFF_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #define g_ON_OFF_SWITCH_CONFIGURATION_SERVER_CLUSTER_ENABLE_d         	1
    #define g_ON_OFF_SWITCH_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d      		0
#else
    #define g_ON_OFF_SWITCH_CONFIGURATION_SERVER_CLUSTER_ENABLE_d          	0
    #define g_ON_OFF_SWITCH_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d        	0
#endif						/* g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
    #define g_TIME_SERVER_CLUSTER_ENABLE_d                       	        1
    #define g_TIME_CLIENT_CLUSTER_ENABLE_d                        	        0
#else
    #define g_TIME_SERVER_CLUSTER_ENABLE_d                      	        0
    #define g_TIME_CLIENT_CLUSTER_ENABLE_d                      	        0
#endif						/* g_RSSI_LOCATION_CLUSTER_ENABLE_d */


#if ( g_RSSI_LOCATION_CLUSTER_ENABLE_d == 1 )
    #define g_RSSI_LOCATION_SERVER_CLUSTER_ENABLE_d                       	1
    #define g_RSSI_LOCATION_CLIENT_CLUSTER_ENABLE_d                        	0
#else
    #define g_RSSI_LOCATION_SERVER_CLUSTER_ENABLE_d                      	0
    #define g_RSSI_LOCATION_CLIENT_CLUSTER_ENABLE_d                      	0
#endif						/* g_RSSI_LOCATION_CLUSTER_ENABLE_d */

#if ( g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d == 1 )
    #define g_DEHUMIDIFICATION_CONTROL_SERVER_CLUSTER_ENABLE_d            	1
    #define g_DEHUMIDIFICATION_CONTROL_CLIENT_CLUSTER_ENABLE_d             	0
#else
    #define g_DEHUMIDIFICATION_CONTROL_SERVER_CLUSTER_ENABLE_d             	0
    #define g_DEHUMIDIFICATION_CONTROL_CLIENT_CLUSTER_ENABLE_d            	0
#endif						/* g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d */

#if ( g_FAN_CONTROL_CLUSTER_ENABLE_d == 1 )
    #define g_FAN_CONTROL_SERVER_CLUSTER_ENABLE_d                         	1
    #define g_FAN_CONTROL_CLIENT_CLUSTER_ENABLE_d                         	0
#else
    #define g_FAN_CONTROL_SERVER_CLUSTER_ENABLE_d                          	0
    #define g_FAN_CONTROL_CLIENT_CLUSTER_ENABLE_d                          	0
#endif						/* g_FAN_CONTROL_CLUSTER_ENABLE_d */

#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
    #define g_PUMP_CONFIGURATION_AND_CONTROL_SERVER_CLUSTER_ENABLE_d       	0
    #define g_PUMP_CONFIGURATION_AND_CONTROL_CLIENT_CLUSTER_ENABLE_d      	1
#else
    #define g_PUMP_CONFIGURATION_AND_CONTROL_SERVER_CLUSTER_ENABLE_d       	0
    #define g_PUMP_CONFIGURATION_AND_CONTROL_CLIENT_CLUSTER_ENABLE_d       	0
#endif						/* g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d */

#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
    #define g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d                          	1
    #define g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d                         	0
#else
    #define g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d                          	0
    #define g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d                         	0
#endif						/* g_THERMOSTAT_CLUSTER_ENABLE_d */

#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d == 1 )
    #define g_THERMOSTAT_USER_INTERFACE_CONFIG_SERVER_CLUSTER_ENABLE_d    	0
    #define g_THERMOSTAT_USER_INTERFACE_CONFIG_CLIENT_CLUSTER_ENABLE_d    	1
#else
    #define g_THERMOSTAT_USER_INTERFACE_CONFIG_SERVER_CLUSTER_ENABLE_d   	0
    #define g_THERMOSTAT_USER_INTERFACE_CONFIG_CLIENT_CLUSTER_ENABLE_d    	0
#endif						/* g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
    #define g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d          				1
    #define g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d                      	0
#else
    #define g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d                      	0
    #define g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d                      	0
#endif						/* g_COLOR_CONTROL_CLUSTER_ENABLE_d */

#if ( g_BALLAST_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #define g_BALLAST_CONFIGURATION_SERVER_CLUSTER_ENABLE_d              	1
    #define g_BALLAST_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d               	0
#else
    #define g_BALLAST_CONFIGURATION_SERVER_CLUSTER_ENABLE_d               	0
    #define g_BALLAST_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d               	0
#endif						/* g_BALLAST_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #define g_FLOW_MEASUREMENT_SERVER_CLUSTER_ENABLE_d                    	0
    #define g_FLOW_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d                     	1
#else
    #define g_FLOW_MEASUREMENT_SERVER_CLUSTER_ENABLE_d                     	0
    #define g_FLOW_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d                    	0
#endif							/* g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )
    #define g_ILLUMINANCE_LEVEL_SENSING_SERVER_CLUSTER_ENABLE_d            	0
    #define g_ILLUMINANCE_LEVEL_SENSING_CLIENT_CLUSTER_ENABLE_d            	1
#else
    #define g_ILLUMINANCE_LEVEL_SENSING_SERVER_CLUSTER_ENABLE_d            	0
    #define g_ILLUMINANCE_LEVEL_SENSING_CLIENT_CLUSTER_ENABLE_d            	0
#endif						/* g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #define g_ILLUMINANCE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d              	1
    #define g_ILLUMINANCE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d              	0
#else
    #define g_ILLUMINANCE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d              	0
    #define g_ILLUMINANCE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d              	0
#endif					/* g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d == 1 )
    #define g_OCCUPANCY_SENSING_SERVER_CLUSTER_ENABLE_d                    	0
    #define g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d                    	1
#else
    #define g_OCCUPANCY_SENSING_SERVER_CLUSTER_ENABLE_d                		0
    #define g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d                    	0
#endif								/* g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d */

#if ( g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #define g_PRESSURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d                	0
    #define g_PRESSURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d                 	1
#else
    #define g_PRESSURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d                 	0
    #define g_PRESSURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d                 	0
#endif				/* g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #define g_RELATIVE_HUMIDITY_MEASUREMENT_SERVER_CLUSTER_ENABLE_d      	1
    #define g_RELATIVE_HUMIDITY_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d        	0
#else
    #define g_RELATIVE_HUMIDITY_MEASUREMENT_SERVER_CLUSTER_ENABLE_d       	0
    #define g_RELATIVE_HUMIDITY_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d        	0
#endif				/* g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #define g_TEMPERATURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d             	0
    #define g_TEMPERATURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d              	1
#else
    #define g_TEMPERATURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d              	0
    #define g_TEMPERATURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d             	0
#endif								/* g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
    #define g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d                            	1
    #define g_IAS_ZONE_CLIENT_CLUSTER_ENABLE_d                            	0
#else
    #define g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d                            	0
    #define g_IAS_ZONE_CLIENT_CLUSTER_ENABLE_d                            	0
#endif								/* g_IAS_ZONE_CLUSTER_ENABLE_d */

#if ( g_IAS_ACE_CLUSTER_ENABLE_d == 1 )
    #define g_IAS_ACE_SERVER_CLUSTER_ENABLE_d                              	1
    #define g_IAS_ACE_CLIENT_CLUSTER_ENABLE_d                             	0
#else
    #define g_IAS_ACE_SERVER_CLUSTER_ENABLE_d                              	0
    #define g_IAS_ACE_CLIENT_CLUSTER_ENABLE_d                              	0
#endif								/* g_IAS_ACE_CLUSTER_ENABLE_d */

#if ( g_IAS_WD_CLUSTER_ENABLE_d == 1 )
    #define g_IAS_WD_SERVER_CLUSTER_ENABLE_d                                1
    #define g_IAS_WD_CLIENT_CLUSTER_ENABLE_d                                0
#else
    #define g_IAS_WD_SERVER_CLUSTER_ENABLE_d                                0
    #define g_IAS_WD_CLIENT_CLUSTER_ENABLE_d                                0
#endif								/* g_IAS_WD_CLUSTER_ENABLE_d */

#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #define g_SHADE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d                 	0
    #define g_SHADE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d                  	1
#else
    #define g_SHADE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d                  	0
    #define g_SHADE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d                  	0
#endif								/* g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_BINARY_INPUT_CLUSTER_ENABLE_d == 1 )
    #define g_BINARY_INPUT_SERVER_CLUSTER_ENABLE_d                       	1
    #define g_BINARY_INPUT_CLIENT_CLUSTER_ENABLE_d                        	0
#else
    #define g_BINARY_INPUT_SERVER_CLUSTER_ENABLE_d                       	0
    #define g_BINARY_INPUT_CLIENT_CLUSTER_ENABLE_d                        	0
#endif								/* g_BINARY_INPUT_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
    #define g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d                     	1
    #define g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d                      	1
#else
    #define g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d                     	0
    #define g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d                      	0
#endif								/* g_SIMPLE_METERING_CLUSTER_ENABLE_d */

#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
    #define g_MESSAGING_SERVER_CLUSTER_ENABLE_d                              1
    #define g_MESSAGING_CLIENT_CLUSTER_ENABLE_d                              0
#else
    #define g_MESSAGING_SERVER_CLUSTER_ENABLE_d                              0
    #define g_MESSAGING_CLIENT_CLUSTER_ENABLE_d                              0
#endif						/* g_MESSAGING_CLUSTER_ENABLE_d */

#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
    #define g_PRICE_SERVER_CLUSTER_ENABLE_d                                  1
    #define g_PRICE_CLIENT_CLUSTER_ENABLE_d                                  1
#else
    #define g_PRICE_SERVER_CLUSTER_ENABLE_d                                  0
    #define g_PRICE_CLIENT_CLUSTER_ENABLE_d                                  0
#endif						/* g_PRICE_CLUSTER_ENABLE_d */

#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
    #define g_DRLC_SERVER_CLUSTER_ENABLE_d                                   1
    #define g_DRLC_CLIENT_CLUSTER_ENABLE_d                                   0
#else
    #define g_DRLC_SERVER_CLUSTER_ENABLE_d                                   0
    #define g_DRLC_CLIENT_CLUSTER_ENABLE_d                                   0
#endif						/* g_DRLC_CLUSTER_ENABLE_d */

#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    #define g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d                      1
    #define g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d                      0
#else
    #define g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d                      0
    #define g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d                      0
#endif						/* g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d */

#if(g_COMMISSIONING_CLUSTER_ENABLE_d == 1)
    #define g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d                          1
    #define g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d                          1
#else
    #define g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d                          0
    #define g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d                          0
#endif              /* g_COMMISSIONING_CLUSTER_ENABLE_d*/

#if( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
    #define g_TUNNELING_SERVER_CLUSTER_ENABLE_d                             1
    #define g_TUNNELING_CLIENT_CLUSTER_ENABLE_d                             1
#else
    #define g_TUNNELING_SERVER_CLUSTER_ENABLE_d                             0
    #define g_TUNNELING_CLIENT_CLUSTER_ENABLE_d                             0
#endif              /* g_TUNNELING_CLUSTER_ENABLE_d*/

#if( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
    #define g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d                            1
    #define g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d                            1
#else
    #define g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d                            0
    #define g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d                            0
#endif              /* g_PREPAYMENT_CLUSTER_ENABLE_d*/

#if( g_OTA_BOOTLOAD_CLUSTER_ENABLE_d == 1 )
    #define g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d                          1
    #define g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d                          0
#else
    #define g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d                          0
    #define g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d                          0
#endif              /* g_OTA_BOOTLOAD_CLUSTER_ENABLE_d*/

#if( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
    #define g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d                       1
    #define g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d                       1
#else
    #define g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d                       0
    #define g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d                       0
#endif              /* g_GENERIC_TUNNEL_CLUSTER_ENABLE_d*/

#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )

#define g_APPLICATION_VERSION_ATTRIBUTE_ENABLE_d                        1
#define g_STACK_VERSION_ATTRIBUTE_ENABLE_d                              1
#define g_HW_VERSION_ATTRIBUTE_ENABLE_d                                 1
#define g_MANUFACTURE_NAME_ATTRIBUTE_ENABLE_d                           1
#define g_MODEL_IDENTIFIER_ATTRIBUTE_ENABLE_d                           1
#define g_DATE_CODE_ATTRIBUTE_ENABLE_d                                  1
#define g_LOCATION_DESCRIPTION_ATTRIBUTE_ENABLE_d                       1
#define g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d                       1
#define g_DEVICE_ENABLED_ATTRIBUTE_ENABLE_d                             1
#define g_ALARM_MASK_ATTRIBUTE_ENABLE_d                                 1
#define g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d                       1
#endif


#if(g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_MAINS_VOLTAGE_ATTRIBUTE_ENABLE_d                              1
#define g_MAINS_FREQUENCY_ATTRIBUTE_ENABLE_d                            1
#define g_MAINS_ALARM_MASK_ATTRIBUTE_ENABLE_d                           1
#define g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d                1
#define g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_ENABLE_d                1
#define g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d             1
#define g_BATTERY_VOLATGE_ATTRIBUTE_ENABLE_d                            1
#define g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d                       1
#define g_BATTERY_SIZE_ATTRIBUTE_ENABLE_d                               1
#define g_BATTERY_AHR_RATING_ATTRIBUTE_ENABLE_d                         1
#define g_BATTERY_QUANTITY_ATTRIBUTE_ENABLE_d                           1
#define g_BATTERY_RATED_VOLATGE_ATTRIBUTE_ENABLE_d                      1
#define g_BATTERY_ALARM_MASK_ATTRIBUTE_ENABLE_d                         1
#define g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d              1
#endif


#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_MIN_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d                       1
#define g_MAX_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d                       1
#define g_OVER_TO_TOTAL_DWELL_ATTRIBUTE_ENABLE_d                        1
#define g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_ENABLE_d                     1
#define g_LOW_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d                         1
#define g_HIGH_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_LOW_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d                  1
#define g_HIGH_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d                 1
#endif


#if( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_LEVEL_REMAINING_TIME_ATTRIBUTE_ENABLE_d                       1
#define g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d                     1
#define g_ON_LEVEL_ATTRIBUTE_ENABLE_d                                   1
#endif


#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d ==1 )
#define g_ALARM_COUNT_ATTRIBUTE_ENABLE_d                                1
#endif

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d==1 )
#define g_SCENE_MGMT_INFO_LAST_CONFIGURED_BY_ATTRIBUTE_ENABLE_d         1
#endif

#if ( g_RSSI_LOCATION_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_LOCATION_AGE_ATTRIBUTE_ENABLE_d                               1
#define g_QUALITY_MEASURE_ATTRIBUTE_ENABLE_d                            1
#define g_NUMBER_OF_DEVICES_ATTRIBUTE_ENABLE_d                          1
#define g_COORDINATE_3_ATTRIBUTE_ENABLE_d                               1
#define g_REPORTING_PERIOD_ATTRIBUTE_ENABLE_d                           1
#define g_CALCULATION_PERIOD_ATTRIBUTE_ENABLE_d                         1
#endif

#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_TIME_ZONE_ATTRIBUTE_ENABLE_d                                 1
#define g_TIME_DST_START_ATTRIBUTE_ENABLE_d                            1
#define g_TIME_DST_END_ATTRIBUTE_ENABLE_d                              1
#define g_TIME_DST_SHIFT_ATTRIBUTE_ENABLE_d                            1
#define g_TIME_STANDARD_TIME_ATTRIBUTE_ENABLE_d                        1
#define g_TIME_LOCAL_TIME_ATTRIBUTE_ENABLE_d                           1
#define g_TIME_LASTSET_TIME_ATTRIBUTE_ENABLE_d                         1
#define g_TIME_VALID_UNTIL_TIME_ATTRIBUTE_ENABLE_d                     1
#endif

#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
/* Attributes of the Thermostat Information Attribute Set */
#define g_OUTDOOR_TEMPERATURE_ATTRIBUTE_ENABLE_d                        1
#define g_OCCUPANCY_ATTRIBUTE_ENABLE_d                                  1
#define g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d               1
#define g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d               1
#define g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d               1
#define g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d               1
#define g_PI_COOLING_DEMAND_ATTRIBUTE_ENABLE_d                          1
#define g_PI_HEATING_DEMAND_ATTRIBUTE_ENABLE_d                          1
/* Attributes of the Thermostat Settings Attribute Set */
#define g_LOCAL_TEMPERATURE_CALIBRATION_ATTRIBUTE_ENABLE_d              1
#define g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d              1
#define g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d              1
#define g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d                   1
#define g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d                   1
#define g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d                   1
#define g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d                   1
#define g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_ENABLE_d                    1
#define g_REMOTE_SENSING_ATTRIBUTE_ENABLE_d                             1
#define g_THERMOSTAT_ALARM_MASK_ATTRIBUTE_ENABLE_d                      1
#endif


#if(g_DEHUMIDIFICATION_SERVER_CONTROL_CLUSTER_ENABLE_d==1)
#define g_RELATIVE_HUMIDITY_ATTRIBUTE_ENABLE_d                          0
#define g_RELATIVE_HUMIDITY_MODE_ATTRIBUTE_ENABLE_d                     0
#define g_DEHUMIDIFICATION_LOCK_OUT_ATTRIBUTE_ENABLE_d                  0
#define g_RELATIVE_HUMIDITY__DISPLAY_ATTRIBUTE_ENABLE_d                 0
#endif


#if(g_PUMP_CONFIGURATION_AND_CONTROL_SERVER_CLUSTER_ENABLE_d==1)
/* Attributes of the Pump Information Attribute Set */
#define g_MIN_CONST_PRESSURE_ATTRIBUTE_ENABLE_d                          1
#define g_MAX_CONST_PRESSURE_ATTRIBUTE_ENABLE_d                          1
#define g_MIN_COMP_PRESSURE_ATTRIBUTE_ENABLE_d                           1
#define g_MAX_COMP_PRESSURE_ATTRIBUTE_ENABLE_d                           1
#define g_MIN_CONST_SPEED_ATTRIBUTE_ENABLE_d                             1
#define g_MAX_CONST_SPEED_ATTRIBUTE_ENABLE_d                             1
#define g_MIN_CONST_FLOW_ATTRIBUTE_ENABLE_d                              1
#define g_MAX_CONST_FLOW_ATTRIBUTE_ENABLE_d                              1
#define g_MIN_CONST_TEMP_ATTRIBUTE_ENABLE_d                              1
#define g_MAX_CONST_TEMP_ATTRIBUTE_ENABLE_d                              1
/* Attributes of the Pump Dynamic Information Attribute Set */
#define g_PUMP_STATUS_ATTRIBUTE_ENABLE_d                                 1
#define g_SPEED_ATTRIBUTE_ENABLE_d                                       1
#define g_LIFETIME_RUNNING_HOURS_ATTRIBUTE_ENABLE_d                      1
#define g_POWER_ATTRIBUTE_ENABLE_d                                       1
#define g_LIFETIME_ENERGY_CONSUMED_ATTRIBUTE_ENABLE_d                    1
/* Attributes of the Pump Settings Attribute Set */
#define g_CONTROL_MODE_ATTRIBUTE_ENABLE_d                                1
#define g_ALARM_MASK_ATTRIBUTE_ENABLE_d                                  1
#endif


#if(g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
#define g_CURRENT_HUE_ATTRIBUTE_ENABLE_d                               1
#define g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d                        1
#define g_REMAINING_TIME_ATTRIBUTE_ENABLE_d                            1
#define g_DRIFT_COMPENSATION_ATTRIBUTE_ENABLE_d                         1
#define g_COMPENSATION_TEXT_ATTRIBUTE_ENABLE_d                          1
#define g_COLOR_TEMPERATURE_ATTRIBUTE_ENABLE_d                          1
#define g_COLOR_MODE_ATTRIBUTE_ENABLE_d                                 1
#define g_NUMBER_OF_PRIMARIES_ATTRIBUTE_ENABLE_d                        0
#define g_PRIMARY_1_X_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_1_Y_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_1_INTENSITY_ATTRIBUTE_ENABLE_d                        0
#define g_PRIMARY_2_X_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_2_Y_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_2_INTENSITY_ATTRIBUTE_ENABLE_d                        0
#define g_PRIMARY_3_X_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_3_Y_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_3_INTENSITY_ATTRIBUTE_ENABLE_d                        0
#define g_PRIMARY_4_X_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_4_Y_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_4_INTENSITY_ATTRIBUTE_ENABLE_d                        0
#define g_PRIMARY_5_X_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_5_Y_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_5_INTENSITY_ATTRIBUTE_ENABLE_d                        0
#define g_PRIMARY_6_X_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_6_Y_ATTRIBUTE_ENABLE_d                                0
#define g_PRIMARY_6_INTENSITY_ATTRIBUTE_ENABLE_d                        0
#define g_WHITE_POINT_X_ATTRIBUTE_ENABLE_d                              0
#define g_WHITE_POINT_Y_ATTRIBUTE_ENABLE_d                              0
#define g_COLOR_POINT_RX_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_RY_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_R_INTENSITY_ATTRIBUTE_ENABLE_d                    0
#define g_COLOR_POINT_GX_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_GY_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_G_INTENSITY_ATTRIBUTE_ENABLE_d                    0
#define g_COLOR_POINT_BX_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_BY_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_B_INTENSITY_ATTRIBUTE_ENABLE_d                    0
#endif


#if (g_BALLAST_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#define g_PHYSICAL_MIN_LEVEL_ATTRIBUTE_ENABLE_d                         0
#define g_PHYSICAL_MAX_LEVEL_ATTRIBUTE_ENABLE_d                         0
#define g_MIN_LEVEL_ATTRIBUTE_ENABLE_d                                  0
#define g_MAX_LEVEL_ATTRIBUTE_ENABLE_d                                  0
#define g_POWER_ON_LEVEL_ATTRIBUTE_ENABLE_d                             0
#define g_POWER_ON_FADE_TIME_ATTRIBUTE_ENABLE_d                         0
#define g_INTRINSIC_BALLAST_FACTOR_ATTRIBUTE_ENABLE_d                   0
#define g_BALLAST_FACTOR_ADJUSTMENT_ATTRIBUTE_ENABLE_d                  0
#define g_LAMP_TYPE_ATTRIBUTE_ENABLE_d                                  0
#define g_PHYSICAL_MIN_LEVEL_ATTRIBUTE_ENABLE_d                         0
#define g_LAMP_MANUFACTURER_ATTRIBUTE_ENABLE_d                          0
#define g_LAMP_RATED_HOURS_ATTRIBUTE_ENABLE_d                           0
#define g_LAMP_BURN_HOURS_ATTRIBUTE_ENABLE_d                            0
#define g_LAMP_ALARM_MODE_ATTRIBUTE_ENABLE_d                            0
#define g_LAMP_BURN_HOURS_TRIP_POINT_ATTRIBUTE_ENABLE_d                 0
#endif


#if( g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d == 1)
#define g_MS_ATTRIB_FLOW_TOLERANCE_ATTRIBUTE_ENABLE_d                   1
#endif


#if( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1)
#define g_MS_ATTRIB_LIGHT_SENSOR_TYPE_ATTRIBUTE_ENABLE_d                1
#endif


#if( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1)
#define g_MS_ATTRIB_ILLUMINANCE_TOLERANCE_ATTRIBUTE_ENABLE_d            1
#define g_MS_ATTRIB_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_ATTRIBUTE_ENABLE_d         1
#endif


#if ( g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d == 1)
#define g_MS_ATTRIB_PIR_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d     1
#define g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_DELAY_ATTRIBUTE_ENABLE_d     1
#define g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d      1
#define g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_DELAY_ATTRIBUTE_ENABLE_d      1
#endif


#if( g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d == 1)
#define g_MS_ATTRIB_PRESSURE_TOLERANCE_ATTRIBUTE_ENABLE_d                   0
#define g_MS_ATTRIB_SCALED_VALUE_ATTRIBUTE_ENABLE_d                         0
#define g_MS_ATTRIB_MIN_SCALED_VALUE_ATTRIBUTE_ENABLE_d                     0
#define g_MS_ATTRIB_MAX_SCALED_VALUE_ATTRIBUTE_ENABLE_d                     0
#define g_MS_ATTRIB_SCALED_TOLERANCE_ATTRIBUTE_ENABLE_d                     0
#define g_MS_ATTRIB_SCALE_ATTRIBUTE_ENABLE_d                                0
#endif


#if( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1)
#define g_MS_ATTRIB_RELATIVE_TOLERANCE_ATTRIBUTE_ENABLE_d                   1
#endif


#if( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1)
#define g_MS_ATTRIB_TEMPERATURE_TOLERANCE_ATTRIBUTE_ENABLE_d                1
#endif



/* Smart Energy Price cluster attribute Optional Attributes */
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_PRICE_TIER1_ATTRIBUTE_ENABLE_d                                   1
#define g_PRICE_TIER2_ATTRIBUTE_ENABLE_d                                   1
#define g_PRICE_TIER3_ATTRIBUTE_ENABLE_d                                   1
#define g_PRICE_TIER4_ATTRIBUTE_ENABLE_d                                   1
#define g_PRICE_TIER5_ATTRIBUTE_ENABLE_d                                   1
#define g_PRICE_TIER6_ATTRIBUTE_ENABLE_d                                   1
#if(g_SE_PROFILE_1_1_d == 1)
/* updates to the SE 1.1 for Tier Label set*/	
#define g_PRICE_TIER7_ATTRIBUTE_ENABLE_d                                   1
#define g_PRICE_TIER8_ATTRIBUTE_ENABLE_d                                   1
#define g_PRICE_TIER9_ATTRIBUTE_ENABLE_d                                   1
#define g_PRICE_TIER10_ATTRIBUTE_ENABLE_d                                  1
#define g_PRICE_TIER11_ATTRIBUTE_ENABLE_d                                  1
#define g_PRICE_TIER12_ATTRIBUTE_ENABLE_d                                  1
#define g_PRICE_TIER13_ATTRIBUTE_ENABLE_d                                  1
#define g_PRICE_TIER14_ATTRIBUTE_ENABLE_d                                  1
#define g_PRICE_TIER15_ATTRIBUTE_ENABLE_d                                  1
#endif    /* g_SE_PROFILE_1_1_d*/

#if(g_SE_PROFILE_1_1_d == 1)
/* updates to the SE 1.1 for Block Threshold set*/	
#define g_BLOCK1_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK2_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK3_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK4_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK5_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK6_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK7_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK8_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK9_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_BLOCK10_THRESHOLD_ATTRIBUTE_ENABLE_d                       1
#define g_BLOCK11_THRESHOLD_ATTRIBUTE_ENABLE_d                       1
#define g_BLOCK12_THRESHOLD_ATTRIBUTE_ENABLE_d                       1
#define g_BLOCK13_THRESHOLD_ATTRIBUTE_ENABLE_d                       1
#define g_BLOCK14_THRESHOLD_ATTRIBUTE_ENABLE_d                       1
#define g_BLOCK15_THRESHOLD_ATTRIBUTE_ENABLE_d                       1
#endif         /* g_SE_PROFILE_1_1_d*/


#if(g_SE_PROFILE_1_1_d == 1)
/* updates to the SE 1.1 for Block Period set*/	
#define g_START_OF_BLOCK_PERIOD_ATTRIBUTE_ENABLE_d                   1
#define g_BLOCK_PERIOD_DURATION_ATTRIBUTE_ENABLE_d                   1
#define g_THRESHOLD_MULTIPLIER_ATTRIBUTE_ENABLE_d                    1
#define g_THRESHOLD_DEVISOR_ATTRIBUTE_ENABLE_d                       1
#endif      /* g_SE_PROFILE_1_1_d*/

#if(g_SE_PROFILE_1_1_d == 1)
/* updates to the SE 1.1 for commodity set*/	
#define g_SERVER_COMMODITY_TYPE_ATTRIBUTE_ENABLE_d                 1
#define g_STANDING_CHARGE_ATTRIBUTE_ENABLE_d                       1
#endif        /* g_SE_PROFILE_1_1_d*/


#if(g_SE_PROFILE_1_2_d == 1)
/* SE 1.1 A update**/
#define g_CONVERSION_FACTOR_ATTRIBUTE_ENABLE_d					1
#define g_CONVERSION_FACTOR_TRAILING_DIGIT_ATTRIBUTE_ENABLE_d	1
#define g_CALORIFIC_VALUE_ATTRIBUTE_ENABLE_d					1
#define g_CALORIFIC_VALUE_UNIT_ATTRIBUTE_ENABLE_d 				1
#define g_CALORIFIC_VALUE_TRAILING_DIGIT_ATTRIBUTE_ENABLE_d		1
/* end of SE 1.1 A update*/
#endif       /* g_SE_PROFILE_1_2_d*/


#if(g_SE_PROFILE_1_1_d == 1)
/* updates to the SE 1.1 for Block Price Information set*/	

#define g_PRICE_NO_TIER_BLOCK1_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK2_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK3_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK4_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK5_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK6_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK7_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK8_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK9_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_NO_TIER_BLOCK10_ATTRIBUTE_ENABLE_d                      1
#define g_PRICE_NO_TIER_BLOCK11_ATTRIBUTE_ENABLE_d                      1
#define g_PRICE_NO_TIER_BLOCK12_ATTRIBUTE_ENABLE_d                      1
#define g_PRICE_NO_TIER_BLOCK13_ATTRIBUTE_ENABLE_d                      1
#define g_PRICE_NO_TIER_BLOCK14_ATTRIBUTE_ENABLE_d                      1
#define g_PRICE_NO_TIER_BLOCK15_ATTRIBUTE_ENABLE_d                      1
#define g_PRICE_NO_TIER_BLOCK16_ATTRIBUTE_ENABLE_d                      1

#define g_PRICE_TIER1_BLOCK1_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER1_BLOCK2_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER1_BLOCK3_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER1_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER1_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER1_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER1_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER1_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER1_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER1_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER1_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER1_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER1_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER1_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER1_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER1_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER2_BLOCK1_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER2_BLOCK2_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER2_BLOCK3_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER2_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER2_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER2_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER2_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER2_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER2_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER2_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER2_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER2_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER2_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER2_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER2_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER2_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER3_BLOCK1_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER3_BLOCK2_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER3_BLOCK3_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER3_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER3_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER3_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER3_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER3_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER3_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER3_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER3_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER3_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER3_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER3_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER3_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER3_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER4_BLOCK1_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER4_BLOCK2_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER4_BLOCK3_ATTRIBUTE_ENABLE_d                       1
#define g_PRICE_TIER4_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER4_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER4_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER4_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER4_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER4_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER4_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER4_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER4_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER4_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER4_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER4_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER4_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER5_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER5_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER5_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER5_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER5_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER5_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER5_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER5_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER6_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER6_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER6_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER6_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER6_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER6_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER6_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER6_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER7_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER7_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER7_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER7_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER7_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER7_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER7_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER7_BLOCK16_ATTRIBUTE_ENABLE_d                      0


#define g_PRICE_TIER8_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER8_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER8_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER8_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER8_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER8_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER8_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER8_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER9_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER9_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER9_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER9_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER9_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER9_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER9_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER9_BLOCK16_ATTRIBUTE_ENABLE_d                      0


#define g_PRICE_TIER10_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER10_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER10_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER10_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER10_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER10_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER10_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER10_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER11_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER11_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER11_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER11_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER11_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER11_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER11_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER11_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER12_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER12_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER12_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER12_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER12_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER12_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER12_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER12_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER13_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER13_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER13_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER13_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER13_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER13_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER13_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER13_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER14_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER14_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER14_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER14_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER14_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER14_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER14_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER14_BLOCK16_ATTRIBUTE_ENABLE_d                      0

#define g_PRICE_TIER15_BLOCK1_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK2_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK3_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK4_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK5_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK6_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK7_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK8_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK9_ATTRIBUTE_ENABLE_d                       0
#define g_PRICE_TIER15_BLOCK10_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER15_BLOCK11_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER15_BLOCK12_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER15_BLOCK13_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER15_BLOCK14_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER15_BLOCK15_ATTRIBUTE_ENABLE_d                      0
#define g_PRICE_TIER15_BLOCK16_ATTRIBUTE_ENABLE_d                      0
#endif  /* g_SE_PROFILE_1_1_d*/
#endif   /* g_PRICE_SERVER_CLUSTER_ENABLE_d*/

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
#if(g_SE_PROFILE_1_1_d == 1)	
#define g_PRICE_INCREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d			1
#define g_PRICE_DECREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d			1
#define g_CLIENT_COMMODITY_TYPE_ATTRIBUTE_ENABLE_d						1
#endif /* g_SE_PROFILE_1_1_d*/
#endif

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
/* Reading Information Attribute Set */
#define g_CURRENT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d					1
#define g_CURRENT_MAX_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d				1
#define g_CURRENT_MAX_DEMAND_RECIEVED_ATTRIBUTE_ENABLE_d				1
#define g_DFT_SUMMATION_ATTRIBUTE_ENABLE_d								1
#define g_DAILY_FREEZE_TIME_ATTRIBUTE_ENABLE_d							1
#define g_POWER_FACTOR_ATTRIBUTE_ENABLE_d								1
#define g_READING_SNAP_SHOT_TIME_ATTRIBUTE_ENABLE_d						1
#define g_CURRENT_MAX_DEMAND_DELIVERED_TIME_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_MAX_DEMAND_RECIEVED_TIME_ATTRIBUTE_ENABLE_d	        1	
#if(g_SE_PROFILE_1_1_d == 1)	
#define g_DEFAULT_UPDATE_PERIOD_ATTRIBUTE_ENABLED_d	       				1
#define g_FAST_POLL_UPDATE_PERIOD_ATTRIBUTE_ENABLED_d	       			1	
#define g_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLED_d  1	
#define g_DAILY_CONSUMPTION_TARGET_ATTRIBUTE_ENABLED_d	       			1	
#define g_CURRENT_BLOCK_ATTRIBUTE_ENABLED_d	       			            1
#define g_PROFILE_INTERVAL_PERIOD_ATTRIBUTE_ENABLED_d	        		1	
#define g_INTERVAL_READ_REPORTING_PERIOD_ATTRIBUTE_ENABLED_d	        1	
#define g_PRESET_READING_TIME_ATTRIBUTE_ENABLED_d	        			1	
#define g_VOLUME_PER_REPORT_ATTRIBUTE_ENABLED_d	        		     	1	
#define g_FLOW_RESTRICTION_ATTRIBUTE_ENABLED_d	        			    1	
#define g_SUPPLY_STATUS_ATTRIBUTE_ENABLED_d	        			        1	
#define g_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_ATTRIBUTE_ENABLED_d    1	
#define g_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_ATTRIBUTE_ENABLED_d   1	
#define g_INLET_TEMPERATURE_ATTRIBUTE_ENABLED_d	        			    1	
#define g_OUTLET_TEMPERATURE_ATTRIBUTE_ENABLED_d	        			1	
#define g_CONTROL_TEMPERATURE_ATTRIBUTE_ENABLED_d	        			1
#define g_CURRENT_INLET_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLED_d       1	
#define g_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLED_d      1
#endif /* End of g_SE_PROFILE_1_1_d */
#if(g_SE_PROFILE_1_2_d == 1)
#define g_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d    1
#define g_CURRENT_BLOCK_PERIOD_CONSUMPTION_RECIEVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_BLOCK_RECIEVED_ATTRIBUTE_ENABLE_d                         1
#define g_DFT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d                         1
#define g_ACTIVE_REGISTER_TIER_DELIVERED_ATTRIBUTE_ENABLE_d                 1
#define g_ACTIVE_REGISTER_TIER_RECIEVED_ATTRIBUTE_ENABLE_d                  1
#endif /* End of g_SE_PROFILE_1_2_d */
/* TOU Information Set */
#define g_CURRENT_TIER1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER1_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER2_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER3_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER4_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER5_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER6_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#if(g_SE_PROFILE_1_1_d == 1)	
#define g_CURRENT_TIER7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER7_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER8_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER9_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER10_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER11_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER12_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER13_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER14_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER15_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#endif  /* g_SE_PROFILE_1_1_d*/
#if(g_SE_PROFILE_1_2_d == 1)
#define g_CURRENT_TIER16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER16_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER17_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER17_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER18_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER18_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER19_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER19_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER20_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER20_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER21_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER21_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER22_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER22_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER23_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER23_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER24_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER24_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER25_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER25_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER26_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER26_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER27_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER27_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER28_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER28_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER29_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER29_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER30_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			0
#define g_CURRENT_TIER30_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER31_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER31_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER32_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER32_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER33_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER33_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER34_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER34_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER35_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER35_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER36_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER36_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER37_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER37_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER38_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER38_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER39_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER39_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER40_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			0
#define g_CURRENT_TIER40_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER41_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER41_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER42_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER42_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER43_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER43_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER44_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER44_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER45_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER45_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER46_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER46_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER47_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER47_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER48_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_TIER48_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d			1
#endif  /* g_SE_PROFILE_1_2_d*/

#if(g_SE_PROFILE_1_1_d == 1)	
#define g_REMAINING_BATTERY_LIFE_ATTRIBUTE_ENABLE_d						1
#define g_HOURS_IN_OPERATION_ATTRIBUTE_ENABLE_d							1
#define g_HOURS_IN_FAULT_ATTRIBUTE_ENABLE_d								1
#endif  /* g_SE_PROFILE_1_1_d*/
#if(g_SE_PROFILE_1_2_d == 1)
#define g_EXTENDED_STATUS_ATTRIBUTE_ENABLE_d                            1
#define g_REMAINING_BATTERY_LIFE_IN_DAYS_ATTRIBUTE_ENABLE_d             1
#define g_CURRENT_METER_ID_ATTRIBUTE_ENABLE_d                           1
#define g_LOW_MEDIUM_HIGH_STATUS_ATTRIBUTE_ENABLE_d                     1
#endif  /* g_SE_PROFILE_1_2_d*/

#define g_MULTIPLIER_ATTRIBUTE_ENABLE_d									1
#define g_DIVISOR_ATTRIBUTE_ENABLE_d									1
#define g_DEMAND_FORMATTING_ATTRIBUTE_ENABLE_d							1
#define g_HISTORICAL_CONSUMPTION_FORMATTING_ATTRIBUTE_ENABLE_d			1

#if(g_SE_PROFILE_1_1_d == 1)	
#define g_SITE_ID_ATTRIBUTE_ENABLE_d									1
#define g_METER_SERIAL_NUMBER_ATTRIBUTE_ENABLE_d						1
#define g_ENERGY_CARRIER_UNIT_OF_MEASURE_ATTRIBUTE_ENABLE_d				1
#define g_ENERGY_CARRIER_SUMMATION_FORMATTING_ATTRIBUTE_ENABLE_d		1
#define g_ENERGY_CARRIER_DEMAND_FORMATTING_ATTRIBUTE_ENABLE_d			1
#define g_TEMPERATURE_UNIT_OF_MEASURE_ATTRIBUTE_ENABLE_d				1
#define g_TEMPERATURE_FORMATTING_ATTRIBUTE_ENABLE_d						1
#endif  /* g_SE_PROFILE_1_1_d*/
#if(g_SE_PROFILE_1_2_d==1)
#define g_MODULE_SERIAL_NUMBER_ATTRIBUTE_ENABLE_d                       1
#define g_OPERATING_TARIFF_LABEL_DELIVERED_ATTRIBUTE_ENABLE_d           1
#define g_OPERATING_TARIFF_LABEL_RECEIVED_ATTRIBUTE_ENABLE_d            1
#endif  /*  ( g_SE_PROFILE_1_2_d == 1 ) */

#define g_INSTANTANEOUS_DEMAND_ATTRIBUTE_ENABLE_d						1
#define g_CURRENT_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d			1
#define g_PREVIOUS_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d			1
#define g_PREVIOUS_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d			1
#define g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_RECIEVED_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_RECIEVED_ATTRIBUTE_ENABLE_d		1

#if(g_SE_PROFILE_1_1_d == 1)	
#define g_CURRENT_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d					1
#define g_CURRENT_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d					1
#define g_PREVIOUS_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d					1
#define g_PREVIOUS_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d					1
#define g_CURRENT_DAY_MAX_DEMAND_ATTRIBUTE_ENABLE_d						1
#define g_PREVIOUS_DAY_MAX_DEMAND_ATTRIBUTE_ENABLE_d					1
#define g_CURRENT_MONTH_MAX_DEMAND_ATTRIBUTE_ENABLE_d					1
#define g_CURRENT_YEAR_MAX_DEMAND_ATTRIBUTE_ENABLE_d					1
#define g_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d		1
#define g_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d		1
#endif    /* g_SE_PROFILE_1_1_d*/
#if(g_SE_PROFILE_1_2_d == 1)
#define g_PREVIOUS_DAY2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_DAY2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_PREVIOUS_DAY3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_DAY3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_PREVIOUS_DAY4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_DAY4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_PREVIOUS_DAY5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_DAY5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_PREVIOUS_DAY6_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_DAY6_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_PREVIOUS_DAY7_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_DAY7_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_PREVIOUS_DAY8_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_DAY8_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_CURRENT_WEEK_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d         1
#define g_CURRENT_WEEK_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d          1
#define g_PREVIOUS_WEEK_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_WEEK_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_PREVIOUS_WEEK2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_WEEK2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_WEEK3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_WEEK3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_WEEK4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_WEEK4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_WEEK5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_WEEK5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_MONTH_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_MONTH_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d         1
#define g_PREVIOUS_MONTH_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_PREVIOUS_MONTH2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH6_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH6_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH7_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH7_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH8_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH8_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH9_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH9_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_PREVIOUS_MONTH10_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d     1
#define g_PREVIOUS_MONTH10_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH11_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d     1
#define g_PREVIOUS_MONTH11_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH12_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d     1
#define g_PREVIOUS_MONTH12_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_PREVIOUS_MONTH13_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d     1
#define g_PREVIOUS_MONTH13_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */

#define g_MAX_NUMBER_OF_PERIODS_DELIVERED_ATTRIBUTE_ENABLE_d			    1
#define g_CURRENT_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d					    1
#define g_DEMAND_LIMIT_ATTRIBUTE_ENABLE_d								    1
#define g_DEMAND_INTEGRATION_PERIOD_ATTRIBUTE_ENABLE_d					    1
#define g_NUMBER_OF_DEMAND_SUBINTERVALS_ATTRIBUTE_ENABLE_d				    1
#if(g_SE_PROFILE_1_2_d == 1)
#define g_DEMAND_LIMIT_ARM_DURATION_IN_MINUTES_ATTRIBUTE_ENABLE_d           1
#define g_LOAD_LIMIT_SUPPLY_STATE_ATTRIBUTE_ENABLE_d                        1
#define g_LOAD_LIMIT_COUNTER_ATTRIBUTE_ENABLE_d                             1
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */
#if(g_SE_PROFILE_1_1_d == 1)	
#define g_CURRENT_NO_TIER_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_NO_TIER_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_NO_TIER_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_NO_TIER_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_NO_TIER_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_NO_TIER_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	1
#define g_CURRENT_NO_TIER_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	1

#define g_CURRENT_TIER1_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER1_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER2_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER2_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER3_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER4_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER4_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER5_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER5_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER6_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER6_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER7_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER7_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER8_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER8_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER9_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER9_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER10_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	    1
#define g_CURRENT_TIER10_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER10_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER11_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d	    1
#define g_CURRENT_TIER11_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER11_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER12_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER12_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER13_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		0
#define g_CURRENT_TIER13_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER13_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER14_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER14_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1

#define g_CURRENT_TIER15_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#define g_CURRENT_TIER15_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d		1
#endif  /* g_SE_PROFILE_1_1_d*/
#if(g_SE_PROFILE_1_2_d == 1)	
/* Block Information Set (Received) */
#define g_CURRENT_NO_TIER_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_NO_TIER_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d     1
#define g_CURRENT_NO_TIER_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d     1
#define g_CURRENT_NO_TIER_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d     1
#define g_CURRENT_NO_TIER_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d     1
#define g_CURRENT_NO_TIER_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d     1
#define g_CURRENT_NO_TIER_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d     1
#define g_CURRENT_NO_TIER_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d     1

#define g_CURRENT_TIER1_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER1_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER1_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER1_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER1_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER1_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER1_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER1_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER2_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER2_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER2_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER2_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER2_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER2_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER2_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER2_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER3_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER3_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER3_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER3_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER3_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER3_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER3_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER3_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER4_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER4_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER4_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER4_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER4_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER4_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER4_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER4_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER5_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER5_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER5_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER5_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER5_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER5_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER5_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER5_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER6_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER6_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER6_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER6_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER6_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER6_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER6_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER6_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER7_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER7_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER7_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER7_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER7_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER7_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER7_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER7_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER8_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER8_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER8_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER8_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER8_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER8_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER8_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER8_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER9_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d        1
#define g_CURRENT_TIER9_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER9_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER9_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER9_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER9_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER9_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER9_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1

#define g_CURRENT_TIER10_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER10_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER10_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER10_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER10_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER10_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER10_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER10_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1

#define g_CURRENT_TIER11_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER11_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER11_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER11_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER11_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER11_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER11_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER11_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1

#define g_CURRENT_TIER12_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER12_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER12_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER12_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER12_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER12_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER12_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER12_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1

#define g_CURRENT_TIER13_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER13_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER13_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER13_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER13_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER13_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER13_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER13_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1

#define g_CURRENT_TIER14_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER14_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER14_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER14_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER14_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER14_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER14_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER14_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1

#define g_CURRENT_TIER15_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER15_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER15_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER15_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER15_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER15_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER15_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER15_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1

#define g_CURRENT_TIER16_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d       1
#define g_CURRENT_TIER16_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER16_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER16_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER16_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER16_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER16_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#define g_CURRENT_TIER16_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d      1
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */

#if(g_SE_PROFILE_1_1_d == 1)
#define g_GENERIC_ALARM_MASK_ATTRIBUTE_ENABLE_d							1
#define g_ELECTRICITY_ALARM_MASK_ATTRIBUTE_ENABLE_d						1
#define g_GENERIC_FLOW_MEASURE_ALARM_MASK_ATTRIBUTE_ENABLE_d			1
#define g_WATER_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d					1
#define g_HEAT_AND_COOLING_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d		1
#define g_GAS_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d					1
#endif
#if(g_SE_PROFILE_1_2_d == 1)
#define g_EXTENDED_GENERIC_ALARM_MASK_ATTRIBUTE_ENABLE_d                1
#define g_MANUFACTURER_ALARM_MASK_ATTRIBUTE_ENABLE_d                    1
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */
#if(g_SE_PROFILE_1_2_d == 1)
#define g_BILL_TO_DATE_IMPORT_ATTRIBUTE_ENABLE_d                        1
#define g_BILL_TO_DATE_TIME_STAMP_IMPORT_ATTRIBUTE_ENABLE_d             1
#define g_PROJECTED_BILL_IMPORT_ATTRIBUTE_ENABLE_d                      1
#define g_PROJECTED_BILL_TIME_STAMP_IMPORT_ATTRIBUTE_ENABLE_d           1
#define g_BILL_TO_DATE_EXPORT_ATTRIBUTE_ENABLE_d                        1
#define g_BILL_TO_DATE_TIME_STAMP_EXPORT_ATTRIBUTE_ENABLE_d             1
#define g_PROJECTED_BILL_EXPORT_ATTRIBUTE_ENABLE_d                      1
#define g_PROJECTED_BILL_TIME_STAMP_EXPORT_ATTRIBUTE_ENABLE_d           1
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */

#endif			          /* g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d */

#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
#define g_FILE_OFFSET_ATTRIBUTE_ENABLE_d					            1
#define g_CURRENT_FILE_VERSION_ATTRIBUTE_ENABLE_d			            1
#define g_CURRENT_ZIGBEESTACK_VERSION_ATTRIBUTE_ENABLE_d	            1
#define g_DOWNLOADED_FILE_VERSION_ATTRIBUTE_ENABLE_d		            1
#define g_DOWNLOADED_ZIGBEESTACK_VERSION_ATTRIBUTE_ENABLE_d	            1
#define g_MANUFACTURER_ID_ATTRIBUTE_ENABLE_d				            1
#define g_IMAGE_TYPE_ID_ATTRIBUTE_ENABLE_d					            1
#endif

#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_PAYMENT_CONTROL_ATTRIBUTE_ENABLE_d   						1
#define g_CREDIT_REMAINING_ATTRIBUTE_ENABLE_d   						1
#define g_EMERGENCY_CREDIT_REMAINING_ATTRIBUTE_ENABLE_d   			1
#define g_CREDIT_STATUS_ATTRIBUTE_ENABLE_d   						1
#define g_TOP_UP_DATE_TIME_1_ATTRIBUTE_ENABLE_d   					1
#define g_TOP_UP_AMOUNT_1_ATTRIBUTE_ENABLE_d   						1
#define g_ORIGINATING_DEVICE_1_ATTRIBUTE_ENABLE_d   					1
#define g_TOP_UP_DATE_TIME_2_ATTRIBUTE_ENABLE_d   					1
#define g_TOP_UP_AMOUNT_2_ATTRIBUTE_ENABLE_d   						1
#define g_ORIGINATING_DEVICE_2_ATTRIBUTE_ENABLE_d   					1
#define g_TOP_UP_DATE_TIME_3_ATTRIBUTE_ENABLE_d   						1
#define g_TOP_UP_AMOUNT_3_ATTRIBUTE_ENABLE_d   						1
#define g_ORIGINATING_DEVICE_3_ATTRIBUTE_ENABLE_d   						1
#define g_TOP_UP_DATE_TIME_4_ATTRIBUTE_ENABLE_d   						1
#define g_TOP_UP_AMOUNT_4_ATTRIBUTE_ENABLE_d   						1
#define g_ORIGINATING_DEVICE_4_ATTRIBUTE_ENABLE_d   						1
#define g_TOP_UP_DATE_TIME_5_ATTRIBUTE_ENABLE_d   						1
#define g_TOP_UP_AMOUNT_5_ATTRIBUTE_ENABLE_d   						1
#define g_ORIGINATING_DEVICE_5_ATTRIBUTE_ENABLE_d   						1
#define g_FUEL_DEBT_REMAINING_ATTRIBUTE_ENABLE_d   						1
#define g_FUEL_DEBT_RECOVERY_RATE_ATTRIBUTE_ENABLE_d   					1
#define g_FUEL_DEBT_RECOVERY_PERIOD_ATTRIBUTE_ENABLE_d   				1
#define g_NON_FUEL_DEBT_REMAINING_ATTRIBUTE_ENABLE_d   					1
#define g_NON_FUEL_DEBT_RECOVERY_RATE_ATTRIBUTE_ENABLE_d   				1
#define g_NON_FUEL_DEBT_RECOVERY_PERIOD_ATTRIBUTE_ENABLE_d   			1
#define g_PROPOSED_CHANGE_PROVIDER_ID_ATTRIBUTE_ENABLE_d   				1
#define g_PROPOSED_CHANGE_IMPLEMENTATION_TIME_ATTRIBUTE_ENABLE_d   		1
#define g_PROPOSED_CHANGE_SUPPLY_STATUS_ATTRIBUTE_ENABLE_d   			1
#define g_DELAYED_SUPPLY_INTERRUPT_VALUE_REMAINING_ATTRIBUTE_ENABLE_d   1
#define g_DELAYED_SUPPLY_INTERRUPT_VALUE_TYPE_ATTRIBUTE_ENABLE_d   	   1

#endif   /* g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d*/
#endif  /* g_SE_PROFILE_1_1_d*/

#if(g_SE_PROFILE_1_2_d == 1)	
#if ( g_TOU_CALENDER_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_AUX_SWITCH_1_ATTRIBUTE_ENABLE_d		1
#define g_AUX_SWITCH_2_ATTRIBUTE_ENABLE_d		1
#define g_AUX_SWITCH_3_ATTRIBUTE_ENABLE_d		1
#define g_AUX_SWITCH_4_ATTRIBUTE_ENABLE_d		1
#define g_AUX_SWITCH_5_ATTRIBUTE_ENABLE_d		1
#define g_AUX_SWITCH_6_ATTRIBUTE_ENABLE_d		1
#define g_AUX_SWITCH_7_ATTRIBUTE_ENABLE_d		1

#endif      /* g_TOU_CALENDER_SERVER_CLUSTER_ENABLE_d*/
#endif     /* g_SE_PROFILE_1_2_d*/


#define g_REPORTING_ENABLE_d                                            1
#define g_SERVICE_DISCOVERY_ENABLE_d                                    1
#if( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1)
#if(g_SE_PROFILE_1_1_d == 1)	
	#define g_TRUST_CENTER_SWAP_OUT_ENABLE_d							1
#endif
#else
	#define g_TRUST_CENTER_SWAP_OUT_ENABLE_d							0
#endif


/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif 								   /* _ZCL_FUNCTIONALITY_H_ */
