#ifndef _ZCL_FUNCTIONALITY_H_
#define _ZCL_FUNCTIONALITY_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* Cluster functionality */
#define g_BASIC_CLUSTER_ENABLE_d                                        1
#define g_POWER_CONFIGURATION_CLUSTER_ENABLE_d                          1
#define g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d             1
#define g_IDENTIFY_CLUSTER_ENABLE_d                                     1
#define g_ALARMS_CLUSTER_ENABLE_d                                       1
#define g_LEVEL_CONTROL_CLUSTER_ENABLE_d                                0
#define g_GROUPS_CLUSTER_ENABLE_d                                       0
#define g_SCENES_CLUSTER_ENABLE_d                                       0
#define g_ON_OFF_CLUSTER_ENABLE_d                                       0
#define g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d                  0
#define g_RSSI_LOCATION_CLUSTER_ENABLE_d                                0
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
#define g_IAS_ACE_CLUSTER_ENABLE_d                                      1
#define g_IAS_ZONE_CLUSTER_ENABLE_d                                     1
#define g_IAS_WD_CLUSTER_ENABLE_d                                       0
#define g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d                          0
#define g_SIMPLE_METERING_CLUSTER_ENABLE_d                         		0
#define g_COMMISSIONING_CLUSTER_ENABLE_d              		            0
#define g_DIAGNOSTIC_CLUSTER_ENABLE_d									0




/* Server or Client enabling Macros */

#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
    #define g_BASIC_SERVER_CLUSTER_ENABLE_d                                 1
    #define g_BASIC_CLIENT_CLUSTER_ENABLE_d                                 0
#else
    #define g_BASIC_SERVER_CLUSTER_ENABLE_d                                 0
    #define g_BASIC_CLIENT_CLUSTER_ENABLE_d                                 0
#endif						/* g_BASIC_CLUSTER_ENABLE_d */

#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #define g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d                   1
    #define g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d                   0
#else
    #define g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d                   0
    #define g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d                   0
#endif						/* g_POWER_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #define g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d  	1
    #define g_DEVICE_TEMPERATURE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d    	0
#else
    #define g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d  	0
    #define g_DEVICE_TEMPERATURE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d   	0
#endif						/* g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
    #define g_IDENTIFY_SERVER_CLUSTER_ENABLE_d                           	1
    #define g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d                            	0
#else
    #define g_IDENTIFY_SERVER_CLUSTER_ENABLE_d                            	0
    #define g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d                            	0
#endif						/* g_IDENTIFY_CLUSTER_ENABLE_d */

#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
    #define g_ALARMS_SERVER_CLUSTER_ENABLE_d                                1
    #define g_ALARMS_CLIENT_CLUSTER_ENABLE_d                                0
#else
    #define g_ALARMS_SERVER_CLUSTER_ENABLE_d                                0
    #define g_ALARMS_CLIENT_CLUSTER_ENABLE_d                                0
#endif						/* g_ALARMS_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
    #define g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d                         1
    #define g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d                         0
#else
    #define g_LEVEL_CONTROL_CLUSTER_ENABLE_d                                0
    #define g_LEVEL_CONTROL_CLUSTER_ENABLE_d                                0
#endif						/* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */

#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
    #define g_GROUPS_SERVER_CLUSTER_ENABLE_d                                1
    #define g_GROUPS_CLIENT_CLUSTER_ENABLE_d                                0
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
    #define g_FAN_CONTROL_CLIENT_CLUSTER_ENABLE_d                         	1
#else
    #define g_FAN_CONTROL_SERVER_CLUSTER_ENABLE_d                          	0
    #define g_FAN_CONTROL_CLIENT_CLUSTER_ENABLE_d                          	0
#endif						/* g_FAN_CONTROL_CLUSTER_ENABLE_d */

#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
    #define g_PUMP_CONFIGURATION_AND_CONTROL_SERVER_CLUSTER_ENABLE_d       	1
    #define g_PUMP_CONFIGURATION_AND_CONTROL_CLIENT_CLUSTER_ENABLE_d      	0
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
    #define g_THERMOSTAT_USER_INTERFACE_CONFIG_SERVER_CLUSTER_ENABLE_d    	1
    #define g_THERMOSTAT_USER_INTERFACE_CONFIG_CLIENT_CLUSTER_ENABLE_d    	0
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
    #define g_FLOW_MEASUREMENT_SERVER_CLUSTER_ENABLE_d                    	1
    #define g_FLOW_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d                     	0
#else
    #define g_FLOW_MEASUREMENT_SERVER_CLUSTER_ENABLE_d                     	0
    #define g_FLOW_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d                    	0
#endif							/* g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )
    #define g_ILLUMINANCE_LEVEL_SENSING_SERVER_CLUSTER_ENABLE_d            	1
    #define g_ILLUMINANCE_LEVEL_SENSING_CLIENT_CLUSTER_ENABLE_d            	0
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
    #define g_OCCUPANCY_SENSING_SERVER_CLUSTER_ENABLE_d                    	1
    #define g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d                    	1
#else
    #define g_OCCUPANCY_SENSING_SERVER_CLUSTER_ENABLE_d                		0
    #define g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d                    	0
#endif								/* g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d */

#if ( g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #define g_PRESSURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d                	1
    #define g_PRESSURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d                 	0
#else
    #define g_PRESSURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d                 	0
    #define g_PRESSURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d                 	0
#endif				/* g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #define g_RELATIVE_HUMIDITY_MEASUREMENT_SERVER_CLUSTER_ENABLE_d      	1
    #define g_RELATIVE_HUMIDITY_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d        	1
#else
    #define g_RELATIVE_HUMIDITY_MEASUREMENT_SERVER_CLUSTER_ENABLE_d       	0
    #define g_RELATIVE_HUMIDITY_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d        	0
#endif				/* g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #define g_TEMPERATURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d             	1
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
    #define g_IAS_ACE_SERVER_CLUSTER_ENABLE_d                              	0
    #define g_IAS_ACE_CLIENT_CLUSTER_ENABLE_d                             	1
#else
    #define g_IAS_ACE_SERVER_CLUSTER_ENABLE_d                              	0
    #define g_IAS_ACE_CLIENT_CLUSTER_ENABLE_d                              	0
#endif								/* g_IAS_ACE_CLUSTER_ENABLE_d */

#if ( g_IAS_WD_CLUSTER_ENABLE_d == 1 )
    #define g_IAS_WD_SERVER_CLUSTER_ENABLE_d                                0
    #define g_IAS_WD_CLIENT_CLUSTER_ENABLE_d                                0
#else
    #define g_IAS_WD_SERVER_CLUSTER_ENABLE_d                                0
    #define g_IAS_WD_CLIENT_CLUSTER_ENABLE_d                                0
#endif								/* g_IAS_WD_CLUSTER_ENABLE_d */

#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #define g_SHADE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d                 	1
    #define g_SHADE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d                  	0
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

#if ( g_DOOR_LOCK_CLUSTER_ENABLE_d == 1 )
    #define g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d                     	    1
    #define g_DOOR_LOCK_CLIENT_CLUSTER_ENABLE_d                     	    0
#else
    #define g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d                     	    0
    #define g_DOOR_LOCK_CLIENT_CLUSTER_ENABLE_d                     	    0
#endif								/* g_DOOR_LOCK_CLUSTER_ENABLE_d */


#if(g_COMMISSIONING_CLUSTER_ENABLE_d == 1)
    #define g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d                          1
    #define g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d                          1
#else
    #define g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d                          0
    #define g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d                          0
#endif              /* g_COMMISSIONING_CLUSTER_ENABLE_d*/

#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )

#define g_APPLICATION_VERSION_ATTRIBUTE_ENABLE_d                        1
#define g_STACK_VERSION_ATTRIBUTE_ENABLE_d                              1
#define g_HW_VERSION_ATTRIBUTE_ENABLE_d                                 1
#define g_MODEL_IDENTIFIER_ATTRIBUTE_ENABLE_d                           1
#define g_DATE_CODE_ATTRIBUTE_ENABLE_d                                  1
#define g_LOCATION_DESCRIPTION_ATTRIBUTE_ENABLE_d                       1
#define g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d                       1
#define g_DEVICE_ENABLED_ATTRIBUTE_ENABLE_d                             1
#define g_ALARM_MASK_ATTRIBUTE_ENABLE_d                                 1
#define g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d                       1
#endif


#if(g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
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


#if(g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#define g_MIN_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d                       1
#define g_MAX_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d                       1
#define g_OVER_TO_TOTAL_DWELL_ATTRIBUTE_ENABLE_d                        1
#define g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_ENABLE_d                     1
#define g_LOW_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d                         1
#define g_HIGH_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d                        1
#define g_LOW_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d                  1
#define g_HIGH_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d                 1
#endif


#if(g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#define g_LEVEL_REMAINING_TIME_ATTRIBUTE_ENABLE_d                       1
#define g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d                     1
#define g_ON_LEVEL_ATTRIBUTE_ENABLE_d                                   1
#endif


#if(g_ALARMS_CLUSTER_ENABLE_d ==1 )
#define g_ALARM_COUNT_ATTRIBUTE_ENABLE_d                                1
#endif


#if(g_SCENES_CLUSTER_ENABLE_d==1)
#define g_SCENE_MGMT_INFO_LAST_CONFIGURED_BY_ATTRIBUTE_ENABLE_d         1
#endif


#if ( g_RSSI_LOCATION_CLUSTER_ENABLE_d == 1 )
#define g_LOCATION_AGE_ATTRIBUTE_ENABLE_d                               1
#define g_QUALITY_MEASURE_ATTRIBUTE_ENABLE_d                            1
#define g_NUMBER_OF_DEVICES_ATTRIBUTE_ENABLE_d                          1
#define g_COORDINATE_3_ATTRIBUTE_ENABLE_d                               1
#define g_REPORTING_PERIOD_ATTRIBUTE_ENABLE_d                           1
#define g_CALCULATION_PERIOD_ATTRIBUTE_ENABLE_d                         1
#endif


#if(g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
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


#if(g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d==1)
#define g_RELATIVE_HUMIDITY_ATTRIBUTE_ENABLE_d                          0
#define g_RELATIVE_HUMIDITY_MODE_ATTRIBUTE_ENABLE_d                     0
#define g_DEHUMIDIFICATION_LOCK_OUT_ATTRIBUTE_ENABLE_d                  0
#define g_RELATIVE_HUMIDITY__DISPLAY_ATTRIBUTE_ENABLE_d                 0
#endif


#if(g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d==1)
/* Attributes of the Pump Information Attribute Set */
#define g_MIN_CONST_PRESSURE_ATTRIBUTE_ENABLE_d                         1
#define g_MAX_CONST_PRESSURE_ATTRIBUTE_ENABLE_d                         1
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


#if(g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1)
#define g_CURRENT_HUE_ATTRIBUTE_ENABLE_d                                       1
#define g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d                                1
#define g_REMAINING_TIME_ATTRIBUTE_ENABLE_d                                    1
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
#define	g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ATTRIBUTE_ENABLE_d		1
#define g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d      1
#define g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_DELAY_ATTRIBUTE_ENABLE_d      1
#define g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ATTRIBUTE_ENABLE_d	1
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

#define g_REPORTING_ENABLE_d                                                 1

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif 								   /* _ZCL_FUNCTIONALITY_H_ */
