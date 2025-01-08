/*******************************************************************************
* Meter Interface _Application_Framework.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for Meter Interface device events
*   handling
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/

#include "ZigBeeDataTypes.h"
#include "stack_common.h"
#include "zdp_interface.h"
#include "ZCL_Common.h"
#include "ZCL_Foundation.h"
#include "apsde_interface.h"

#include "MeterInterface_Application_Framework.h"
#include "app_configuration_default.h"

#if(g_BASIC_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Basic_Cluster.h"
#endif                             /* g_BASIC_CLUSTER_ENABLE_d */

#if(g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Power_Config_Cluster.h"
#endif                             /* g_POWER_CONFIGURATION_CLUSTER_ENABLE_d */

#if(g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Device_Temp_Config_Cluster.h"
#endif                 /* g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d */

#if(g_IDENTIFY_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Identify_Cluster.h"
#endif                            /* g_IDENTIFY_CLUSTER_ENABLE_d */

#if(g_ALARMS_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Alarms_Cluster.h"
#endif                            /* g_ALARMS_CLUSTER_ENABLE_d */

#if (g_ON_OFF_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Cluster.h"
#endif                            /* g_ON_OFF_CLUSTER_ENABLE_d */

#if(g_GROUPS_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Groups_Cluster.h"
#endif                           /* g_GROUPS_CLUSTER_ENABLE_d */

#if(g_SCENES_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Scenes_Cluster.h"
#endif                          /* g_SCENES_CLUSTER_ENABLE_d */

#if(g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Switch_Config_Cluster.h"
#endif                      /* g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d */

#if(g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Level_Control_Cluster.h"
#endif                         /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#endif                                  /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
#endif                                  /* g_COLOR_CONTROL_CLUSTER_ENABLE_d */

#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Price_Cluster.h"
    #include "AppPriceClusterUtilities.h"
#endif              /* g_PRICE_CLUSTER_ENABLE_d */

#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_DRLC_Cluster.h"
    #include "AppDRLCUtilities.h"
#endif              /* g_DRLC_CLUSTER_ENABLE_d */

#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Time_Cluster.h"
#endif             /* g_TIME_CLUSTER_ENABLE_d */

#if(g_ON_OFF_CLUSTER_ENABLE_d ==1 )
#include "ZCL_General_On_Off_Cluster.h"
#endif                                      /* g_ON_OFF_CLUSTER_ENABLE_d */

#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Message_Cluster.h"
#endif

#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Prepayment_Cluster.h"
#endif

#if(g_METER_IDENTIFICATION_CLUSTER_ENABLE_d == 1)
    #include "ZCL_HA_Meter_Identification_Cluster.h"
#endif
/*******************************************************************************
* Global Constants
*******************************************************************************/

#define m_BROADCAST_END_POINT_c			        0xFF



/*******************************************************************************
* Public Memory declarations
*******************************************************************************/
/*******************************************************************************
Complex Descriptor:
these are the default values of complex descriptor the fields are explained
in Application_Framework.h
*******************************************************************************/
/* None */

/*******************************************************************************
* Private Memory declarations
*******************************************************************************/
const cluster_id_t a_In_Cluster_List_For_Meter_Interface[] =
{
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif                           /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
   ,g_IDENTIFY_CLUSTER_c
#endif                          /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   ,g_POWER_CONFIGURATION_CLUSTER_c
#endif                         /* g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   ,g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c
#endif                        /* g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
   ,g_ALARMS_CLUSTER_c
#endif                       /* g_ALARMS_SERVER_CLUSTER_ENABLE_d */
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_PRICE_CLUSTER_c        /* g_PRICE_CLIENT_CLUSTER_ENABLE_d */
#endif
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
   ,g_SIMPLE_METERING_CLUSTER_c
#endif                          /* g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d */
#if( g_METER_IDENTIFICATION_SERVER_CLUSTER_ENABLE_d == 1 )
   ,g_METER_IDENTIFICATION_CLUSTER_c
#endif

};

const cluster_id_t a_Out_Cluster_List_For_Meter_Interface[] =
{

#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_IDENTIFY_CLUSTER_c
#endif                             /* g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
   ,g_SIMPLE_METERING_CLUSTER_c
#endif                          /* g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d */
#if ( g_TIME_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_TIME_CLUSTER_c
#endif                         /* g_TIME_CLIENT_CLUSTER_ENABLE_d */
#if ( g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_PREPAYMENT_CLUSTER_c
#endif
#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_PRICE_CLUSTER_c        /* g_PRICE_CLIENT_CLUSTER_ENABLE_d */
#endif
#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d ==  1 )
    ,g_MESSAGING_CLUSTER_c
#endif
};

/*simple descriptor for Meter Interface*/
const Simple_Descriptor_t Meter_Interface_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_METER_INTERFACE_DEVICE_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Meter_Interface)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_Meter_Interface,
	sizeof(a_Out_Cluster_List_For_Meter_Interface)/ sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Meter_Interface
};

/******************************************************************************/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{ (Simple_Descriptor_t*)&Meter_Interface_Simple_Desc,
    g_METER_INTERFACE_DEVICE_ENDPOINT_c },
};

Endpoint_Description_t  g_Broadcast_Endpoint_Descriptors =
{
	NULL, m_BROADCAST_END_POINT_c
};

Endpoint_Description_t *gp_Endpoint_Descriptors = ga_Endpoint_Descriptors;
/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* None */

/*******************************************************************************
* Public Functions
*******************************************************************************/

/* None */

/*******************************************************************************
* Private Functions
*******************************************************************************/

/* None */

/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/

/* None */

/*******************************************************************************
* End Of File
*******************************************************************************/
