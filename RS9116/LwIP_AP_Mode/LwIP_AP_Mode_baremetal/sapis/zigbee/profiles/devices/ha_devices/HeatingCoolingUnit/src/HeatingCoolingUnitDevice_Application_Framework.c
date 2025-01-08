/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"
#include "zdp_interface.h"
#include "ApplicationThinLayer.h"
#include "HeatingCoolingUnitDevice_Application_Framework.h"
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Basic_Cluster.h"
#endif                      /* ( g_BASIC_CLUSTER_ENABLE_d == 1 ) */
#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Identify_Cluster.h"
#endif                      /* ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 ) */
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Power_Config_Cluster.h"
#endif                      /* ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 ) */
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Device_Temp_Config_Cluster.h"
#endif                      /* ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 ) */
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Alarms_Cluster.h"
#endif                      /* ( g_ALARMS_CLUSTER_ENABLE_d == 1 ) */
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Groups_Cluster.h"
#endif                      /* ( g_GROUPS_CLUSTER_ENABLE_d == 1 ) */
#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_HVAC_Thermostat_Cluster.h"
#endif                      /* ( g_FAN_CONTROL_CLUSTER_ENABLE_d == 1 ) */
#if ( g_FAN_CONTROL_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_HVAC_Fan_Control_Cluster.h"
#endif                      /* ( g_FAN_CONTROL_CLUSTER_ENABLE_d == 1 ) */
#if( g_ON_OFF_CLUSTER_ENABLE_d ==1 )
    #include "ZCL_General_On_Off_Cluster.h"
#endif                      /* ( g_ON_OFF_CLUSTER_ENABLE_d == 1 ) */
#if( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Level_Control_Cluster.h"
#endif                      /* ( g_ON_OFF_CLUSTER_ENABLE_d == 1 ) */

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#endif                      /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */

#include "app_configuration_default.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_BROADCAST_END_POINT_c						    0xFF


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
Complex Descriptor:
these are the default values of complex descriptor the fields are explained
in Application_Framework.h
-----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
const cluster_id_t a_In_Cluster_List_For_HeatingCoolingUnit[] =
{
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif                                  /* g_BASIC_CLUSTER_ENABLE_d */
#if ( g_IDENTIFY_CLUSTER_ENABLE_d ==  1 )
   g_IDENTIFY_CLUSTER_c,
#endif                                   /* g_IDENTIFY_CLUSTER_ENABLE_d */
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   g_POWER_CONFIGURATION_CLUSTER_c,
#endif                           /* g_POWER_CONFIGURATION_CLUSTER_ENABLE_d */
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
#endif                           /* g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d */
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
   g_ALARMS_CLUSTER_c,
#endif                               /* g_ALARMS_CLUSTER_ENABLE_d */
#if( g_ON_OFF_CLUSTER_ENABLE_d ==1 )
    g_ON_OFF_CLUSTER_c,
#endif                               /* g_ON_OFF_CLUSTER_ENABLE_d */
#if( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
    g_LEVEL_CONTROL_CLUSTER_c,
#endif                               /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */
#if ( g_FAN_CONTROL_CLUSTER_ENABLE_d == 1 )
    g_FAN_CONTROL_CLUSTER_c,
#endif                               /* g_FAN_CONTROL_CLUSTER_ENABLE_d */
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
   g_GROUPS_CLUSTER_c,
#endif                               /* g_GROUPS_CLUSTER_ENABLE_d */
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                              /* g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d */	
};

const cluster_id_t a_Out_Cluster_List_For_HeatingCoolingUnit[] =
{
#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
    g_THERMOSTAT_CLUSTER_c,
#endif                              /* g_THERMOSTAT_CLUSTER_ENABLE_d */
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                              /* g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d */
};

/* simple descriptor for Configuration tool */
const Simple_Descriptor_t HeatingCoolingUnit_Simple_Desc =
{	
	g_APP_PROFILE_ID_c,
	g_HEATINGCOOLINGUNIT_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_HeatingCoolingUnit) /	sizeof(cluster_id_t),	
	(cluster_id_t*)a_In_Cluster_List_For_HeatingCoolingUnit,
	sizeof(a_Out_Cluster_List_For_HeatingCoolingUnit) / sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_HeatingCoolingUnit
};

/*---------------------------------------------------------------------------*/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{(Simple_Descriptor_t*)&HeatingCoolingUnit_Simple_Desc, g_HEATINGCOOLINGUNIT_ENDPOINT_c }
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
