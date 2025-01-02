/*******************************************************************************
* ZLL_Controller_Application_Framework.c
********************************************************************************
* Program Description:
*   This file provides information about cluster list for every
*	controller device.
*	- Provides info about simple descriptor for each device.
*	- Has array of endpoints supported by the node along with their respective
*	  endpoint information.

*******************************************************************************/


/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZCL_Foundation.h"
#include "zdp_interface.h"
#include "ApplicationThinLayer.h"
#include "app_configuration_default.h"

#if(g_BASIC_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Basic_Cluster.h"
#endif /* g_BASIC_CLUSTER_ENABLE_d */


#if(g_IDENTIFY_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Identify_Cluster.h"
#endif /*  g_IDENTIFY_CLUSTER_ENABLE_d*/

#if(g_GROUPS_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Groups_Cluster.h"
#endif /* g_GROUPS_CLUSTER_ENABLE_d */

#if(g_SCENES_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Scenes_Cluster.h"
#endif /*  g_SCENES_CLUSTER_ENABLE_d*/


#if(g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Level_Control_Cluster.h"
#endif /*  g_LEVEL_CONTROL_CLUSTER_ENABLE_d*/

#if(g_ON_OFF_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Cluster.h"
#endif /* g_ON_OFF_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
#endif  /*g_COLOR_CONTROL_CLUSTER_ENABLE_d  */

#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1)
    #include "ZLL_Commissioning_Cluster.h"
    #include "AppZllCommissioningClusterUtilities.h"
#endif

/*******************************************************************************
* Global Constants
*******************************************************************************/
#define m_BROADCAST_END_POINT_c			        0xFF

/*******************************************************************************
* Public Memory declarations
******************************************************************************/

/* None */

/*******************************************************************************
* Private Memory declarations
*******************************************************************************/
const cluster_id_t a_In_Cluster_List_For_Color_Conroller[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/
};

const cluster_id_t a_Out_Cluster_List_For_Color_Controller[] =
{
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
	g_IDENTIFY_CLUSTER_c
#endif                                  /* g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d */
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif                              /* g_GROUPS_CLIENT_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif                              /* g_ON_OFF_CLIENT_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif                              /* g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
   , g_COLOR_CONTROL_CLUSTER_c
#endif                              /* g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)*/

};

	/* List of Clusters supported for Color Scene Controller as Server*/
const cluster_id_t a_In_Cluster_List_For_Color_Scene_Conroller[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */

#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/
};

const cluster_id_t a_Out_Cluster_List_For_Color_Scene_Conroller[] =
{
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
	g_IDENTIFY_CLUSTER_c
#endif                                  /* g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d */
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif                              /* g_GROUPS_CLIENT_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif                              /* g_ON_OFF_CLIENT_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif                              /* g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d */

#if ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif                              /* g_SCENES_CLIENT_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
   , g_COLOR_CONTROL_CLUSTER_c
#endif                              /* g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)*/

};


	/* List of Clusters supported for Non Color  Controller as Server*/
const cluster_id_t a_In_Cluster_List_For_Non_Color_Conroller[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/
};

const cluster_id_t a_Out_Cluster_List_For_Non_Color_Conroller[] =
{
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
	g_IDENTIFY_CLUSTER_c
#endif                                  /* g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d */
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif                              /* g_GROUPS_CLIENT_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif                              /* g_ON_OFF_CLIENT_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_CLEINT_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif                              /* g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)*/

};


	/* List of Clusters supported for Non Color Scene Controller as Server*/
const cluster_id_t a_In_Cluster_List_For_Non_Color_Scene_Conroller[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/
};

const cluster_id_t a_Out_Cluster_List_For_Non_Color_Scene_Conroller[] =
{
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
	g_IDENTIFY_CLUSTER_c
#endif                                  /* g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d */
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif                              /* g_GROUPS_CLIENT_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif                              /* g_ON_OFF_CLIENT_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif                              /* g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d */

#if ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif                              /* g_SCENES_CLIENT_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)*/
};


	/* List of Clusters supported for Control Bridge Controller as Server*/
const cluster_id_t a_In_Cluster_List_For_Control_Bridge_Conroller[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/
};

const cluster_id_t a_Out_Cluster_List_For_Control_Bridge_Conroller[] =
{
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
	g_IDENTIFY_CLUSTER_c
#endif                                  /* g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d */
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif                              /* g_GROUPS_CLIENT_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif                              /* g_ON_OFF_CLIENT_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif                              /* g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d */

#if ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif                              /* g_SCENES_CLIENT_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
   , g_COLOR_CONTROL_CLUSTER_c
#endif                              /* g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d */

#if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)*/
};


	/* List of Clusters supported for On Off Sensor Controller as Server*/
const cluster_id_t a_In_Cluster_List_For_On_Off_Sensor_Conroller[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/
};

const cluster_id_t a_Out_Cluster_List_For_On_Off_Sensor_Conroller[] =
{
#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
	g_IDENTIFY_CLUSTER_c
#endif                                  /* g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d */
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif                              /* g_GROUPS_CLIENT_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif                              /* g_ON_OFF_CLIENT_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif                              /* g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d */

#if ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif                              /* g_SCENES_CLIENT_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
   , g_COLOR_CONTROL_CLUSTER_c
#endif                              /* g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)*/

};


/*simple descriptor for Color Controller*/
 Simple_Descriptor_t ZLL_Color_Controller_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_COLOR_CONTROLLER_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Color_Conroller)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_Color_Conroller,
	sizeof(a_Out_Cluster_List_For_Color_Controller)/ sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Color_Controller
};

/*simple descriptor for Color Scene Controller*/
 Simple_Descriptor_t ZLL_Color_Scene_Controller_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_COLOR_SCENE_CONTROLLER_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Color_Scene_Conroller)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_Color_Scene_Conroller,
	sizeof(a_Out_Cluster_List_For_Color_Scene_Conroller)/ sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Color_Scene_Conroller
};


/*simple descriptor for Non Color Controller*/
 Simple_Descriptor_t ZLL_Non_Color_Controller_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_NON_COLOR_CONTROLLER_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Non_Color_Conroller)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_Non_Color_Conroller,
	sizeof(a_Out_Cluster_List_For_Non_Color_Conroller)/ sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Non_Color_Conroller
};


/*simple descriptor for Non Color Scene Controller*/
 Simple_Descriptor_t ZLL_Non_Color_Scene_Controller_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_NON_COLOR_SCENE_CONTROLLER_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Non_Color_Scene_Conroller)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_Non_Color_Scene_Conroller,
	sizeof(a_Out_Cluster_List_For_Non_Color_Scene_Conroller)/ sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Non_Color_Scene_Conroller
};

/*simple descriptor for Control Bridge Controller*/
 Simple_Descriptor_t ZLL_Control_Bridge_Controller_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_CONTROL_BRIDGE_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Control_Bridge_Conroller)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_Control_Bridge_Conroller,
	sizeof(a_Out_Cluster_List_For_Control_Bridge_Conroller)/ sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Control_Bridge_Conroller
};


/*simple descriptor for On Off Sensor Controller*/
 Simple_Descriptor_t ZLL_On_Off_Sensor_Controller_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_ON_OFF_SENSOR_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_On_Off_Sensor_Conroller)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_On_Off_Sensor_Conroller,
	sizeof(a_Out_Cluster_List_For_On_Off_Sensor_Conroller)/ sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_On_Off_Sensor_Conroller
};



/******************************************************************************/
/* 2 dummy endpoints*/
 Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c+2] =
{
	/* Invalid Endpoint (device)*/
	{g_NULL_c,0xfe}
#if(g_COLOR_CONTROLLER_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for Color Controller */
	,{ (Simple_Descriptor_t*)&ZLL_Color_Controller_Simple_Desc,
	g_COLOR_CONTROLLER_ENDPOINT_ID_c }
#endif
#if(g_COLOR_SCENE_CONTROLLER_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for Color Scene Controller */
	,{ (Simple_Descriptor_t*)&ZLL_Color_Scene_Controller_Simple_Desc,
	g_COLOR_SCENE_CONTROLLER_ENDPOINT_ID_c }
#endif
#if(g_NON_COLOR_CONTROLLER_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for Non Color Controller */
	,{ (Simple_Descriptor_t*)&ZLL_Non_Color_Controller_Simple_Desc,
	g_NON_COLOR_CONTROLLER_ENDPOINT_ID_c }
#endif
#if(g_NON_COLOR_SCENE_CONTROLLER_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for Non Color Scene Controller */
	,{ (Simple_Descriptor_t*)&ZLL_Non_Color_Scene_Controller_Simple_Desc,
	g_NON_COLOR_SCENE_CONTROLLER_ENDPOINT_ID_c }
#endif
#if(g_CONTROL_BRIDGE_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for Control Bridge Controller */
	,{ (Simple_Descriptor_t*)&ZLL_Control_Bridge_Controller_Simple_Desc,
	g_CONTROL_BRIDGE_ENDPOINT_ID_c }
#endif

#if(g_ON_OFF_SENSOR_DEVICE_c == RSI_ENABLE)
	/*  Simple Descriptor for On Off Sensor Controller */
	,{ (Simple_Descriptor_t*)&ZLL_On_Off_Sensor_Controller_Simple_Desc,
	g_ON_OFF_SENSOR_ENDPOINT_ID_c }
#endif
,{g_NULL_c,0xfe}
,{g_NULL_c,0xfe}
,{g_NULL_c,0xfe}
,{g_NULL_c,0xfe}
,{g_NULL_c,0xfe}
};


const Endpoint_Description_t  g_Broadcast_Endpoint_Descriptors =
{
	g_NULL_c, m_BROADCAST_END_POINT_c
};

Endpoint_Description_t *gp_Endpoint_Descriptors = ga_Endpoint_Descriptors;
/*******************************************************************************
* Function Prototypes
******************************************************************************/

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
