/*******************************************************************************
* PumpControllerDevice_Application_Framework.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for PumpController Device event
*   handling
*******************************************************************************/

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"
#include "zdp_interface.h"
#include "ApplicationThinLayer.h"
#include "PumpControllerDevice_Application_Framework.h"
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Basic_Cluster.h"
#endif
#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Identify_Cluster.h"
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
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_HVAC_Pump_Config_And_Control_Cluster.h"
#endif
#if( g_ON_OFF_CLUSTER_ENABLE_d ==1 )
    #include "ZCL_General_On_Off_Cluster.h"
#endif
#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Scenes_Cluster.h"
#endif
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Groups_Cluster.h"
#endif
#if( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Level_Control_Cluster.h"
#endif
#if ( g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Pressure_Measurement_Cluster.h"
#endif
#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Temp_Measurement_Cluster.h"
#endif
#if ( g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_M_And_S_Flow_Measurement_Cluster.h"
#endif
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#endif                                  /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */

#include "app_configuration_default.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
const static cluster_id_t a_In_Cluster_List_For_PumpController[] =
{
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif
#if ( g_IDENTIFY_CLUSTER_ENABLE_d ==  1 )
   g_IDENTIFY_CLUSTER_c,
#endif
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   g_POWER_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
   g_ALARMS_CLUSTER_c
#endif
};

const static cluster_id_t a_Out_Cluster_List_For_PumpController[] =
{

#if( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d ==1 )
    g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_c,
#endif
#if( g_ON_OFF_CLUSTER_ENABLE_d ==1 )
    g_ON_OFF_CLUSTER_c,
#endif
#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
   g_SCENES_CLUSTER_c,
#endif
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
   g_GROUPS_CLUSTER_c,
#endif
#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
    g_LEVEL_CONTROL_CLUSTER_c,
#endif
#if ( g_PRESSURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    g_PRESSURE_MEASUREMENT_CLUSTER_c,
#endif
#if ( g_TEMPERATURE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    g_TEMPERATURE_MEASUREMENT_CLUSTER_c,
#endif
#if ( g_FLOW_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
    g_FLOW_MEASUREMENT_CLUSTER_c,
#endif

#if ( g_IDENTIFY_CLUSTER_ENABLE_d ==  1 )
   g_IDENTIFY_CLUSTER_c,
#endif
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                              /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
};

/* simple descriptor for PumpController tool */
const static Simple_Descriptor_t PumpController_Simple_Desc =
{	
	g_APP_PROFILE_ID_c,
	g_PUMPCONTROLLER_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_PumpController) /sizeof(cluster_id_t),	
	(cluster_id_t*)a_In_Cluster_List_For_PumpController,
	sizeof(a_Out_Cluster_List_For_PumpController) / sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_PumpController
};

/*---------------------------------------------------------------------------*/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{ (Simple_Descriptor_t*)&PumpController_Simple_Desc, g_PUMPCONTROLLER_ENDPOINT_c }
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
