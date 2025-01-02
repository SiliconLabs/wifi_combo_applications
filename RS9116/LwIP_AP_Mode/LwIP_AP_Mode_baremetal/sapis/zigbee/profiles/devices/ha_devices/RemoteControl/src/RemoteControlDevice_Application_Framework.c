/*******************************************************************************
* RemoteControlDevice_Application_Framework.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for Remote Control
*  device event handling
*******************************************************************************/

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

#include "ZCL_Foundation.h"
#include "zdp_interface.h"
#include "RemoteControlDevice_Application_Framework.h"
#include "ApplicationThinLayer.h"

#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Basic_Cluster.h"
#endif                                      /* g_BASIC_CLUSTER_ENABLE_d */

#if(g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Power_Config_Cluster.h"
#endif                                      /* g_POWER_CONFIGURATION_CLUSTER_ENABLE_d */

#if(g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Device_Temp_Config_Cluster.h"
#endif                      /* g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d */

#if(g_ALARMS_CLUSTER_ENABLE_d ==1 )
#include "ZCL_General_Alarms_Cluster.h"
#endif                                      /* g_ALARMS_CLUSTER_ENABLE_d */

#if(g_IDENTIFY_CLUSTER_ENABLE_d ==1 )
#include "ZCL_General_Identify_Cluster.h"
#endif                                      /* g_IDENTIFY_CLUSTER_ENABLE_d */

#if(g_GROUPS_CLUSTER_ENABLE_d ==1 )
#include "ZCL_General_Groups_Cluster.h"
#endif                                      /* g_GROUPS_CLUSTER_ENABLE_d */

#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Scenes_Cluster.h"
#endif                                      /* g_SCENES_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#endif                                  /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */

#if( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Level_Control_Cluster.h"
#endif                                  /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_On_Off_Cluster.h"
#endif                                  /* g_ON_OFF_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
#endif                                  /* g_COLOR_CONTROL_CLUSTER_ENABLE_d */

#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Pump_Config_And_Control_Cluster.h"
#endif                      /* g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d */

#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Closures_Shade_Configuration_Cluster.h"
#endif                      /* g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d */

#if(g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Switch_Config_Cluster.h"
#endif                      /* g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Temp_Measurement_Cluster.h"
#endif                      /* g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Illuminance_Level_Sensing_Cluster.h"
#endif                      /* g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Illuminance_Measurement_Cluster.h"
#endif                      /* g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d */

#include "app_configuration_default.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_BROADCAST_END_POINT_c						    0xFF


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
const cluster_id_t a_In_Cluster_List_For_Remote_Control[] =
{
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif                                  /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
   g_IDENTIFY_CLUSTER_c,
#endif                                  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   g_POWER_CONFIGURATION_CLUSTER_c,
#endif                                  /* g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
#endif                                  /* g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
   g_ALARMS_CLUSTER_c,
#endif                                  /* g_ALARMS_SERVER_CLUSTER_ENABLE_d */

};


const cluster_id_t a_Out_Cluster_List_For_Remote_Control[] =
{
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif                                      /* g_BASIC_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
   g_IDENTIFY_CLUSTER_c,
#endif                                  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLUSTER_ENABLE_d == 1 )
    g_ON_OFF_CLUSTER_c,
#endif                                  /* g_ON_OFF_CLUSTER_ENABLE_d */

#if( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
    g_LEVEL_CONTROL_CLUSTER_c,
#endif                                  /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
    g_GROUPS_CLUSTER_c,
#endif                              /* g_GROUPS_CLUSTER_ENABLE_d */

#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
    g_SCENES_CLUSTER_c,
#endif      						 /* g_SCENES_CLUSTER_ENABLE_d */
#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
    g_COLOR_CONTROL_CLUSTER_c,
#endif                                  /* g_COLOR_CONTROL_CLUSTER_ENABLE_d */

#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
    g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_c,
#endif                      /* g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d */

#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    g_SHADE_CONFIGURATION_CLUSTER_c,
#endif                      /* g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d */

#if(g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1)
    g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_c,
#endif                      /* g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d */

#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    g_TEMPERATURE_MEASUREMENT_CLUSTER_c,
#endif                      /* g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )
    g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_c,
#endif                      /* g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    g_ILLUMINANCE_MEASUREMENT_CLUSTER_c,
#endif                      /* g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                              /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
};

/* simple descriptor for Remote Control Controller */
const Simple_Descriptor_t Remote_Control_Simple_Desc =
{	
	g_APP_PROFILE_ID_c,
	g_REMOTE_CONTROL_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Remote_Control) /	sizeof(cluster_id_t),	
	(cluster_id_t*)a_In_Cluster_List_For_Remote_Control,
	sizeof(a_Out_Cluster_List_For_Remote_Control) / sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Remote_Control
};

/*---------------------------------------------------------------------------*/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{ (Simple_Descriptor_t*)&Remote_Control_Simple_Desc,
        g_REMOTE_CONTROL_ENDPOINT_c },
};

Endpoint_Description_t  g_Broadcast_Endpoint_Descriptors =
{
	g_NULL_c, m_BROADCAST_END_POINT_c
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
