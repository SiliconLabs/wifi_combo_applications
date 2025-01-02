/*******************************************************************************
* ThermostatDevice_Application_Framework.c
********************************************************************************
* Program Description:
*   This module provides all the functionality forThermostat Device event
*   handling
*******************************************************************************/

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "app_configuration_default.h"
#include "stack_util.h"
#include "stack_common.h"
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "ZCL_Common.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "ApplicationThinLayer.h"

#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Basic_Cluster.h"
#endif
#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Identify_Cluster.h"
#endif
#if ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Power_Config_Cluster.h"
#endif
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Device_Temp_Config_Cluster.h"
#endif
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Alarms_Cluster.h"
#endif
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_HVAC_Thermostat_Cluster.h"
#endif
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Groups_Cluster.h"
#endif
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Scenes_Cluster.h"
#endif

#if ( g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"
#endif

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Simple_Metering_Cluster.h"
#endif

#if (g_IAS_WD_SERVER_CLUSTER_ENABLE_d == 1)
	#include "ZCL_Security_And_Safety_IAS_WD_Cluster.h"
	#include "IAS_WDDevice_Application_Framework.h"

#endif

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
const cluster_id_t a_In_Cluster_List_For_IAS_WD[] = {
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif
#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==  1 )
   g_IDENTIFY_CLUSTER_c,
#endif
#if ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
   g_POWER_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
   g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
   g_ALARMS_CLUSTER_c,
#endif
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
   g_GROUPS_CLUSTER_c,
#endif
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
     g_SCENES_CLUSTER_c,
#endif

#if ( g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d == 1)
   g_IAS_ZONE_CLUSTER_c,
#endif
	
#if ( g_IAS_WD_SERVER_CLUSTER_ENABLE_d == 1)
   g_IAS_WD_CLUSTER_c,
#endif	
	
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
    g_SIMPLE_METERING_CLUSTER_c    /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
#endif	
};

const cluster_id_t a_Out_Cluster_List_For_IAS_WD[] = {

#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
    g_SIMPLE_METERING_CLUSTER_c
#endif
};

/* simple descriptor for Configuration tool */
const Simple_Descriptor_t IAS_WD_Simple_Desc =
{	
	g_APP_PROFILE_ID_c,
	g_IAS_WD_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_IAS_WD) / sizeof(cluster_id_t),	
	(cluster_id_t*)a_In_Cluster_List_For_IAS_WD,
	sizeof(a_Out_Cluster_List_For_IAS_WD) / sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_IAS_WD
};

/*---------------------------------------------------------------------------*/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{(Simple_Descriptor_t*)&IAS_WD_Simple_Desc, g_IAS_WD_ENDPOINT_c },
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
