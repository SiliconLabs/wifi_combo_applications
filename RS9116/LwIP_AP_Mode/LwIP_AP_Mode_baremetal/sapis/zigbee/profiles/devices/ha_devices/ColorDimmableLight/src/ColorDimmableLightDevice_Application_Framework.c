
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Foundation.h"
#include "zdp_interface.h"
#include "ColorDimmableLightDevice.h"
#include "ColorDimmableLightDevice_Application_Framework.h"

#include "app_configuration_default.h"


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

#if(g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Level_Control_Cluster.h"
#endif                                    /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */

#if(g_ON_OFF_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Cluster.h"
#endif                                        /* g_ON_OFF_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
#include "AppColorControlUtilities.h"
#endif                                  /* g_COLOR_CONTROL_CLUSTER_ENABLE_d */

#if(g_GROUPS_CLUSTER_ENABLE_d ==1 )
#include "ZCL_General_Groups_Cluster.h"
#endif                                      /* g_GROUPS_CLUSTER_ENABLE_d */

#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Scenes_Cluster.h"
#endif                                      /* g_SCENES_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#endif                                  /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
#if(g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d == 1)
#include "ZCL_M_And_S_Occupancy_Sensing_Cluster.h"
#endif

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_BROADCAST_END_POINT_c			        0xFF


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
const cluster_id_t a_In_Cluster_List_For_Color_Dimmable_Light[] =
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
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    g_ON_OFF_CLUSTER_c,
#endif                              /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    g_GROUPS_CLUSTER_c,
#endif                              /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    g_SCENES_CLUSTER_c,
#endif                              /* g_SCENES_SERVER_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    g_LEVEL_CONTROL_CLUSTER_c,
#endif                              /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */
#if(g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
  g_COLOR_CONTROL_CLUSTER_c,
#endif                      /*  g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                              /* g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d */

};

const cluster_id_t a_Out_Cluster_List_For_Color_Dimmable_Light[] =
{
#if(g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d == 1)
  g_OCCUPANCY_SENSING_CLUSTER_c,
#endif                              /* g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d */
#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                              /* g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d */


};


/*---------------------------------------------------------------------------*/
/*simple descriptor for Color Dimmable Light*/
const Simple_Descriptor_t Color_Dimmable_Light_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_COLOR_DIMMABLE_LIGHT_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
        sizeof(a_In_Cluster_List_For_Color_Dimmable_Light)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Color_Dimmable_Light,
	sizeof(a_Out_Cluster_List_For_Color_Dimmable_Light)/ sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Color_Dimmable_Light
};

/*---------------------------------------------------------------------------*/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{ (Simple_Descriptor_t*)&Color_Dimmable_Light_Simple_Desc, g_COLOR_DIMMABLE_LIGHT_ENDPOINT_c },
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
