/******************************************************************************
 * zdp_interface.h
 ******************************************************************************
 * Program Description:
 *     This file contains the interface functions and data structures to be used
 *     by the application for accessing ZDP management services.
 *
 ******************************************************************************/
#ifndef CLUSTER_LIST_H
#define CLUSTER_LIST_H

/******************************************************************************
 * Includes
 ******************************************************************************/

#if(g_BASIC_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Basic_Cluster.h"
#endif
#if(g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Power_Config_Cluster.h"
#endif
#if (g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Device_Temp_Config_Cluster.h"
#endif
#if(g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Level_Control_Cluster.h"
#endif
#if(g_ALARMS_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Alarms_Cluster.h"
#endif
#if(g_GROUPS_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Groups_Cluster.h"
#endif
#if(g_SCENES_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Scenes_Cluster.h"
#endif
#if(g_IDENTIFY_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Identify_Cluster.h"
#endif
#if(g_ON_OFF_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Cluster.h"
#endif
#if(g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Switch_Config_Cluster.h"
#endif
#if(g_RSSI_LOCATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_RSSI_Location_Cluster.h"
#endif
#if ( g_BINARY_INPUT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Binary_Input_Cluster.h"
#endif
#if ( g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Dehumidification_Control_Cluster.h"
#endif
#if ( g_FAN_CONTROL_CLUSTER_ENABLE_d ==  1 )
#include "ZCL_HVAC_Fan_Control_Cluster.h"
#endif
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Pump_Config_And_Control_Cluster.h"
#endif
#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Thermostat_Cluster.h"
#endif
#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Thermostat_User_Interface_Config_Cluster.h"
#endif
#if ( g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Flow_Measurement_Cluster.h"
#endif
#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Illuminance_Level_Sensing_Cluster.h"
#endif
#if ( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Illuminance_Measurement_Cluster.h"
#endif
#if ( g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Occupancy_Sensing_Cluster.h"
#endif
#if ( g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Pressure_Measurement_Cluster.h"
#endif
#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Relative_Humidity_Measurement_Cluster.h"
#endif
#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Temp_Measurement_Cluster.h"
#endif
#if ( g_IAS_ACE_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_ACE_Cluster.h"
#endif
#if ( g_IAS_WD_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_WD_Cluster.h"
#endif
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"
#endif
#if(g_DOOR_LOCK_CLUSTER_ENABLE_d == 1)
#include "ZCL_Closures_Door_Lock_Cluster.h"
#endif
#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Closures_Shade_Configuration_Cluster.h"
#endif
#if ( g_WINDOW_COVERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Windows_Covering_Cluster.h"
#endif
#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
#endif
#if ( g_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Commissioning_Cluster.h"
#endif
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#endif              /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Time_Cluster.h"
#endif
#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
#include "ZCL_DRLC_Cluster.h"
#endif
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Key_Establishment.h"
#endif
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Message_Cluster.h"
#endif
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Price_Cluster.h"
#endif
#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Generic_Tunnel_Cluster.h"
#endif
#if ( g_OTA_BOOTLOAD_CLUSTER_ENABLE_d == 1 )
#include "ZCL_OTA_Upgrade_Cluster.h"
#endif
#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Prepayment_Cluster.h"
#endif
#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Tunneling_Cluster.h"
#endif

#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Prepayment_Cluster.h"
#endif /* g_PREPAYMENT_CLUSTER_ENABLE_d*/

#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"
#endif

#if ( g_IAS_ACE_CLIENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_ACE_Cluster.h"
#endif

#if ( g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
#endif

#if(g_DOOR_LOCK_CLIENT_CLUSTER_ENABLE_d == 1)
#include "ZCL_Closures_Door_Lock_Cluster.h"
#endif


/******************************************************************************
 * Global Constants
 ******************************************************************************/

typedef struct  {
	cluster_id_t cluster_id;
	const char * name;
}__attribute__((__packed__))clusterIdToNames;

#define g_ZCL_NULL_CLUSTER_c 0xFFFF


/*----------------------------------------------------------------------------*/
const clusterIdToNames Cluster_List[] =
{
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
	{  g_BASIC_CLUSTER_c,  "Basic " },
#endif                                /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
	{ g_POWER_CONFIGURATION_CLUSTER_c, "PowerConfig "},
#endif                                  /* g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
	{g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c, "TempConfig "},
#endif                                  /* g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */
#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
	{ g_LEVEL_CONTROL_CLUSTER_c, "levelControl "},
#endif                              /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
	{g_ALARMS_CLUSTER_c, "Alarms "},
#endif                                  /* g_ALARMS_CLUSTER_ENABLE_d */	
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
	{g_GROUPS_CLUSTER_c, "Groups "},
#endif                              /* g_GROUPS_CLUSTER_ENABLE_d */
#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
	{g_SCENES_CLUSTER_c, "Scenes "},
#endif                              /* g_SCENES_CLUSTER_ENABLE_d */
#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
	{ g_IDENTIFY_CLUSTER_c, "Identify "},
#endif                /* g_IDENTIFY_CLUSTER_ENABLE_d */
#if ( g_ON_OFF_CLUSTER_ENABLE_d == 1 )
	{ g_ON_OFF_CLUSTER_c, "Onoff "},
#endif                              /* g_ON_OFF_CLUSTER_ENABLE_d */
#if( g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
	{g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_c ,"OnOff Switch "},
#endif	               /* g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d */
#if(g_RSSI_LOCATION_CLUSTER_ENABLE_d == 1)
	{g_RSSI_LOCATION_CLUSTER_c,  "RSSI "},
#endif                        /* g_RSSI_LOCATION_CLUSTER_ENABLE_d */
#if ( g_BINARY_INPUT_CLUSTER_ENABLE_d == 1 )
	{g_BINARY_INPUT_CLUSTER_c, "Binary Cluster"},
#endif                         /* g_BINARY_INPUT_CLUSTER_ENABLE_d */
#if ( g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d == 1 )
	{g_DEHUMIDIFICATION_CONTROL_CLUSTER_c, "Dehumidication Control "},
#endif                         /* g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d */
#if ( g_FAN_CONTROL_CLUSTER_ENABLE_d ==  1 )
	{g_FAN_CONTROL_CLUSTER_c,"Fan Control "},
#endif                          /* g_FAN_CONTROL_CLUSTER_ENABLE_d */
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
	{g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_c, "Pump Config & control "},
#endif                            /* g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d */
#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
	{g_THERMOSTAT_CLUSTER_c, "Thermostat "},
#endif                             /* g_THERMOSTAT_CLUSTER_ENABLE_d */
#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d == 1 )
	{g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_c, "Thermostat User Interface "},
#endif                            /* g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d */
#if ( g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
	{g_FLOW_MEASUREMENT_CLUSTER_c, "Flow Measurement "},
#endif                          /* g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d */
#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )
	{g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_c, "Illuminance Level Sensing "},
#endif                           /* g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d */
#if ( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
	{g_ILLUMINANCE_MEASUREMENT_CLUSTER_c, "Illuminance Measurement "},
#endif                          /* g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d */
#if ( g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d == 1 )
	{g_OCCUPANCY_SENSING_CLUSTER_c, "Occupancy Sensing "},
#endif                      /* g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d */
#if ( g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
	{g_PRESSURE_MEASUREMENT_CLUSTER_c, "Pressure Measurement "},
#endif                       /* g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d */
#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
	{g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_c, "Relative Humidity Measurement "},
#endif                         /* g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d */
#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
	{g_TEMPERATURE_MEASUREMENT_CLUSTER_c, "Temperature Measurement "},
#endif                       /* g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d */
#if ( g_IAS_ACE_CLUSTER_ENABLE_d == 1 )
	{g_IAS_ACE_CLUSTER_c, "IAS ACE "},
#endif                     /* g_IAS_ACE_CLUSTER_ENABLE_d */
#if ( g_IAS_WD_CLUSTER_ENABLE_d == 1 )
	{g_IAS_WD_CLUSTER_c , "IAS WD "},
#endif                  /* g_IAS_WD_CLUSTER_ENABLE_d */
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
	{g_IAS_ZONE_CLUSTER_c, "IAS Zone "},
#endif                 /* g_IAS_ZONE_CLUSTER_ENABLE_d */
#if(g_DOOR_LOCK_CLUSTER_ENABLE_d == 1)
	{g_DOOR_LOCK_CLUSTER_c, "Door Lock "} ,
#endif                   /* g_DOOR_LOCK_CLUSTER_ENABLE_d */
#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
	{g_SHADE_CONFIGURATION_CLUSTER_c, "Shade Configuration "},
#endif                     /* g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d */
#if ( g_WINDOW_COVERING_CLUSTER_ENABLE_d == 1 )
	{g_WINDOW_COVERING_CLUSTER_c, "Windows Covering"},
#endif             			/* g_WINDOW_COVERING_CLUSTER_ENABLE_d */
#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
	{g_COLOR_CONTROL_CLUSTER_c, "Color Control "},
#endif                  	/* g_COLOR_CONTROL_CLUSTER_ENABLE_d */
#if ( g_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
	{g_COMMISSIONING_CLUSTER_c, "Commissioning "},
#endif                  /* g_COMMISSIONING_CLUSTER_ENABLE_d */
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
	{g_SIMPLE_METERING_CLUSTER_c , "Simple Metering "},
#endif                              /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
	{g_TIME_CLUSTER_c, "Time "},
#endif                             /* g_TIME_CLUSTER_ENABLE_d */
#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
	{g_DRLC_CLUSTER_c, "DRLC "},
#endif                   		 /* g_DRLC_CLUSTER_ENABLE_d */
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
	{g_KEY_ESTABLISHMENT_CLUSTER_c, "Key Establishment "},
#endif	               			/* g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d */
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
	{g_MESSAGING_CLUSTER_c, "Messaging "},
#endif	            			 /* g_MESSAGING_CLUSTER_ENABLE_d */
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
	{g_PRICE_CLUSTER_c , "Price "},
#endif	 						/* g_PRICE_CLUSTER_ENABLE_d */
#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
	{g_GENERIC_TUNNEL_CLUSTER_c, "Generic "},
#endif                       	/* g_GENERIC_TUNNEL_CLUSTER_ENABLE_d */
#if ( g_OTA_BOOTLOAD_CLUSTER_ENABLE_d == 1 )
	{g_OTA_UPGRADE_CLUSTER_c, "OTA Upgrade "},
#endif                     		 /* g_OTA_BOOTLOAD_CLUSTER_ENABLE_d */
#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
	{g_PREPAYMENT_CLUSTER_c, "Prepayment "},
#endif	   						/* g_PREPAYMENT_CLUSTER_ENABLE_d */
#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
	{g_TUNNELING_CLUSTER_c, "Tunnelling "},
#endif        					/* g_TUNNELING_CLUSTER_ENABLE_d */
	{g_ZCL_NULL_CLUSTER_c, "Null Cluster"}
};

#endif

