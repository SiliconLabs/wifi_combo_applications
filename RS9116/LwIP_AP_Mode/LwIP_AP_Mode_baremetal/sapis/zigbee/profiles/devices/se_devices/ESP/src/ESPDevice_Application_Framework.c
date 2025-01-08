/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "app_configuration_default.h"
#include "stack_util.h"
#include "stack_common.h"
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "ZCL_Common.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "ApplicationThinLayer.h"
#include "ESPDevice_Application_Framework.h"
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Basic_Cluster.h"
#endif
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Power_Config_Cluster.h"
#endif
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Alarms_Cluster.h"
#endif
#if( g_IDENTIFY_CLUSTER_ENABLE_d ==1 )
    #include "ZCL_General_Identify_Cluster.h"
#endif
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Message_Cluster.h"
#endif
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Price_Cluster.h"
#endif
#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_DRLC_Cluster.h"
#endif
#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Time_Cluster.h"
#endif
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Simple_Metering_Cluster.h"
#endif
#if(g_COMMISSIONING_CLUSTER_ENABLE_d == 1)
    #include "ZCL_Commissioning_Cluster.h"
#endif
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Key_Establishment.h"
#endif
#if(g_TUNNELING_CLUSTER_ENABLE_d == 1)
    #include "ZCL_Tunneling_Cluster.h"
#endif
#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Prepayment_Cluster.h"
#endif
#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Generic_Tunnel_Cluster.h"
#endif
#if ( g_OTA_BOOTLOAD_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_OTA_Upgrade_Cluster.h"
#endif
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

const cluster_id_t a_In_Cluster_List_For_ESP[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif
#if ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
     g_POWER_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==  1 )
    g_IDENTIFY_CLUSTER_c,
#endif
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
     g_ALARMS_CLUSTER_c,
#endif
#if ( g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d ==  1 )
    g_COMMISSIONING_CLUSTER_c,
#endif
#if ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 )
    g_KEY_ESTABLISHMENT_CLUSTER_c,
#endif
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d ==  1 )
    g_MESSAGING_CLUSTER_c,
#endif
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
    g_PRICE_CLUSTER_c,
#endif
#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
    g_DRLC_CLUSTER_c,
#endif
#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
    g_TIME_CLUSTER_c,
#endif
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
    g_SIMPLE_METERING_CLUSTER_c,
#endif
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
      g_TUNNELING_CLUSTER_c,
#endif
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
      g_PREPAYMENT_CLUSTER_c,
#endif
#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
	 g_GENERIC_TUNNEL_CLUSTER_c,
#endif
#if ( g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1 )
	 g_OTA_UPGRADE_CLUSTER_c
#endif
};

const cluster_id_t a_Out_Cluster_List_For_ESP[] =
{
#if ( g_BASIC_CLIENT_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif
#if ( g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 )
     g_POWER_CONFIGURATION_CLUSTER_c,
#endif
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d ==  1 )
    g_IDENTIFY_CLUSTER_c,
#endif
#if ( g_ALARMS_CLIENT_CLUSTER_ENABLE_d == 1 )
     g_ALARMS_CLUSTER_c,
#endif
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
    g_KEY_ESTABLISHMENT_CLUSTER_c,
#endif
#if ( g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d ==  1 )
    g_COMMISSIONING_CLUSTER_c,
#endif
#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
    g_PRICE_CLUSTER_c,
#endif
#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
    g_SIMPLE_METERING_CLUSTER_c,
#endif
#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
     g_TUNNELING_CLUSTER_c,
#endif
#if ( g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
    g_PREPAYMENT_CLUSTER_c,
#endif
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
	 g_OTA_UPGRADE_CLUSTER_c,
#endif
#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
	 g_GENERIC_TUNNEL_CLUSTER_c
#endif

};

/* simple descriptor for ESP tool */
const Simple_Descriptor_t ESP_Simple_Desc =
{	
	g_APP_PROFILE_ID_c,
	g_ESP_DEVICE_ID_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_ESP) /	sizeof(cluster_id_t),	
	(cluster_id_t*)a_In_Cluster_List_For_ESP,
	sizeof(a_Out_Cluster_List_For_ESP) / sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_ESP
};

/*---------------------------------------------------------------------------*/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{ (Simple_Descriptor_t*)&ESP_Simple_Desc, g_ESP_ENDPOINT_c },
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
