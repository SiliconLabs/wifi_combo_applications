/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

#include "stack_util.h"
#include "stack_common.h"
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "ZCL_Common.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "ApplicationThinLayer.h"
#include "ConfigurationToolDevice_Application_Framework.h"
#include "app_configuration_default.h"
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Basic_Cluster.h"
#endif
#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Identify_Cluster.h"
#endif
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Groups_Cluster.h"
#endif
#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Scenes_Cluster.h"
#endif
#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Closures_Shade_Configuration_Cluster.h"
#endif
#if ( g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Flow_Measurement_Cluster.h"
#endif
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Power_Config_Cluster.h"
#endif
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Device_Temp_Config_Cluster.h"
#endif
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Alarms_Cluster.h"
#endif
#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Illuminance_Level_Sensing_Cluster.h"
#endif
#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Temp_Measurement_Cluster.h"
#endif
#if ( g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Pressure_Measurement_Cluster.h"
#endif
#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_HVAC_Thermostat_User_Interface_Config_Cluster.h"
#endif
#if ( g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Occupancy_Sensing_Cluster.h"
#endif
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_HVAC_Pump_Config_And_Control_Cluster.h"
#endif
#if ( g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Flow_Measurement_Cluster.h"
#endif
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#endif                                  /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_BROADCAST_END_POINT_c						    0xFF


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
const cluster_id_t a_In_Cluster_List_For_Config_Tool[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif
#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==  1 )
   g_IDENTIFY_CLUSTER_c,
#endif
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_POWER_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_ALARMS_CLUSTER_c,
#endif
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                              /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
};

const cluster_id_t a_Out_Cluster_List_For_Config_Tool[] =
{
#if ( g_BASIC_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_IDENTIFY_CLUSTER_c,
#endif
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_GROUPS_CLUSTER_c,
#endif
#if ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_SCENES_CLUSTER_c,
#endif
#if ( g_ILLUMINANCE_LEVEL_SENSING_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_c,
#endif
#if ( g_TEMPERATURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_TEMPERATURE_MEASUREMENT_CLUSTER_c,
#endif
#if ( g_PRESSURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_PRESSURE_MEASUREMENT_CLUSTER_c,
#endif
#if ( g_FLOW_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_FLOW_MEASUREMENT_CLUSTER_c,
#endif
#if ( g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_OCCUPANCY_SENSING_CLUSTER_c,
#endif
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_c,
#endif
#if ( g_SHADE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_SHADE_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_c,
#endif
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                              /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
};

/* simple descriptor for Configuration tool */
const Simple_Descriptor_t Config_Tool_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_CONFIGURATION_TOOL_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Config_Tool) /	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_Config_Tool,
	sizeof(a_Out_Cluster_List_For_Config_Tool) / sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Config_Tool
};

/*---------------------------------------------------------------------------*/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{ (Simple_Descriptor_t*)&Config_Tool_Simple_Desc, g_CONFIG_TOOL_ENDPOINT_c },
};
Endpoint_Description_t *gp_Endpoint_Descriptors = ga_Endpoint_Descriptors;
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
