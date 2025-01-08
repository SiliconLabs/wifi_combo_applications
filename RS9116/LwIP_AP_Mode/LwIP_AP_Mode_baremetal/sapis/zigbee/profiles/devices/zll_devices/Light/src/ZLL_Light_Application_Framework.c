/*******************************************************************************
* ZLL_Light_Application_Framework.c
******************************************************************************
* Program Description:
*   This module provides all the functionality for any device in the
*	ZLL lIGHT device type
*******************************************************************************/



/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZCL_Foundation.h"
#include "zdp_interface.h"
#include "ZLL_Configuration.h"
#include "ApplicationThinLayer.h"
#include "app_configuration_default.h"
#include "ZLL_Light_Application_Framework.h"

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
/* In Cluster list and Out Cluster list of On Off Light Device */
const cluster_id_t a_In_Cluster_List_For_On_Off_Light[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
	,g_IDENTIFY_CLUSTER_c
#endif  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif  /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif /* g_SCENES_SERVER_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif  /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/

};

	/* List of Clusters supported for On Off Plug-in Unit as Server*/
const cluster_id_t a_In_Cluster_List_For_On_Off_Plug_In_Unit_Light[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
	,g_IDENTIFY_CLUSTER_c
#endif  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif  /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif /* g_SCENES_SERVER_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif  /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/
};

	/* List of Clusters supported for ZLL dimmable Light as Server*/
const cluster_id_t a_In_Cluster_List_For_ZLL_Dimmable_Light[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
	,g_IDENTIFY_CLUSTER_c
#endif  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif  /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif /* g_SCENES_SERVER_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif  /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/

};

/* List of Clusters supported for ZLL dimmable Plug In Unit Light as Server*/
const cluster_id_t a_In_Cluster_List_For_ZLL_Dimmable_Plug_In_Unit_Light[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
	,g_IDENTIFY_CLUSTER_c
#endif  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif  /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif /* g_SCENES_SERVER_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif  /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/

};


/* List of Clusters supported for ZLL Color Light as Server*/
const cluster_id_t a_In_Cluster_List_For_ZLL_Color_Light[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
	,g_IDENTIFY_CLUSTER_c
#endif  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif  /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif /* g_SCENES_SERVER_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif  /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
   , g_COLOR_CONTROL_CLUSTER_c
#endif                              /* g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/


};



/* List of Clusters supported for ZLL Extended Color Light as Server*/
const cluster_id_t a_In_Cluster_List_For_ZLL_Extended_Color_Light[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
	,g_IDENTIFY_CLUSTER_c
#endif  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif  /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif /* g_SCENES_SERVER_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif  /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
   , g_COLOR_CONTROL_CLUSTER_c
#endif  /* g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/


};



/* List of Clusters supported for ZLL Color temperature as Server*/
const cluster_id_t a_In_Cluster_List_For_ZLL_Color_Temperature_Light[] =
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c
#endif /* g_BASIC_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
	,g_IDENTIFY_CLUSTER_c
#endif  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_GROUPS_CLUSTER_c
#endif  /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_ON_OFF_CLUSTER_c
#endif /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_SCENES_CLUSTER_c
#endif /* g_SCENES_SERVER_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    ,g_LEVEL_CONTROL_CLUSTER_c
#endif  /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
   , g_COLOR_CONTROL_CLUSTER_c
#endif  /* g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,g_ZLL_COMMISSIONING_CLUSTER_c
#endif /* #if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)*/
};


/*simple descriptor for On Off Light Device */
 Simple_Descriptor_t ZLL_On_Off_Light_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_ON_OFF_LIGHT_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_On_Off_Light)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_On_Off_Light,
	0x00,g_NULL_c
};


/*simple descriptor for On Off Plug In Unit Light Device */
 Simple_Descriptor_t ZLL_On_Off_Plug_In_Unit_Light_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_ON_OFF_PLUG_IN_UNIT_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_On_Off_Plug_In_Unit_Light)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_On_Off_Plug_In_Unit_Light,
	0x00,g_NULL_c
};

/*simple descriptor for Dimmable Light Device */
 Simple_Descriptor_t ZLL_Dimmable_Light_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_ZLL_DIMMABLE_LIGHT_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_ZLL_Dimmable_Light)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_ZLL_Dimmable_Light,
	0x00,g_NULL_c
};


/*simple descriptor for Dimmable Plug In Unit Light Device */
 Simple_Descriptor_t ZLL_Dimmable_Plug_In_Unit_Light_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_DIMMABLE_PLUG_IN_UNIT_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_ZLL_Dimmable_Plug_In_Unit_Light)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_ZLL_Dimmable_Plug_In_Unit_Light,
	0x00,g_NULL_c
};


/*simple descriptor for Color Light Device */
 Simple_Descriptor_t ZLL_Color_Light_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_COLOR_LIGHT_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_ZLL_Color_Light)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_ZLL_Color_Light,
	0x00,g_NULL_c
};

/*simple descriptor for Extended Color Light Device */
 Simple_Descriptor_t ZLL_Extended_Color_Light_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_EXTENDED_COLOR_LIGHT_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_ZLL_Extended_Color_Light)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_ZLL_Extended_Color_Light,
	0x00,g_NULL_c
};

/*simple descriptor for Color Temperature Light Device */
 Simple_Descriptor_t ZLL_Color_Temperature_Simple_Desc =
{
	g_APP_PROFILE_ID_c,
	g_COLOR_TEMPERATURE_LIGHT_c,
	g_APP_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_ZLL_Color_Temperature_Light)/	sizeof(cluster_id_t),
	(cluster_id_t*)a_In_Cluster_List_For_ZLL_Color_Temperature_Light,
	0x00,g_NULL_c
};


/******************************************************************************/
/* plus 2 dummy endpoints*/
 Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c+2] =
{
	/* Invalid Endpoint (device)*/
	{g_NULL_c,0xfe}
#if(g_ON_OFF_LIGHT_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for ZLL_On_Off_Light_Simple_Desc*/
	,{ (Simple_Descriptor_t*)&ZLL_On_Off_Light_Simple_Desc,
	g_ON_OFF_LIGHT_ENDPOINT_ID_c }
#endif /* g_ON_OFF_LIGHT_DEVICE_c == RSI_ENABLE*/

#if(g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for ZLL_On_Off_Plug_In_Unit_Light_Simple_Desc */
	,{ (Simple_Descriptor_t*)&ZLL_On_Off_Plug_In_Unit_Light_Simple_Desc,
	g_ON_OFF_PLUG_IN_UNIT_ENDPOINT_ID_c }
#endif /* g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE*/

#if(g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for ZLL_Dimmable_Light_Simple_Desc */
	,{ (Simple_Descriptor_t*)&ZLL_Dimmable_Light_Simple_Desc,
	g_DIMMABLE_LIGHT_ENDPOINT_ID_c }
#endif /* g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE*/
#if(g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor forZLL_Dimmable_Plug_In_Unit_Light_Simple_Desc */
	,{ (Simple_Descriptor_t*)&ZLL_Dimmable_Plug_In_Unit_Light_Simple_Desc,
	g_DIMMABLE_PLUG_IN_UNIT_ENDPOINT_ID_c }
#endif /*g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE */

#if(g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for ZLL_Color_Light_Simple_Desc*/
	,{ (Simple_Descriptor_t*)&ZLL_Color_Light_Simple_Desc,
	g_COLOR_LIGHT_ENDPOINT_ID_c }
#endif /* g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE*/


#if(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)
	/* Simple Descriptor for ZLL_Extended_Color_Light_Simple_Desc */
	,{ (Simple_Descriptor_t*)&ZLL_Extended_Color_Light_Simple_Desc,
	g_EXTENDED_COLOR_LIGHT_ENDPOINT_ID_c }
#endif  /* g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE*/


#if(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c == RSI_ENABLE)
	/*  Simple Descriptor for ZLL_Color_Temperature_Simple_Desc*/
	,{ (Simple_Descriptor_t*)&ZLL_Color_Temperature_Simple_Desc,
	g_COLOR_TEMPERATURE_LIGHT_ENDPOINT_ID_c }
#endif /* g_COLOR_TEMPERATURE_LIGHT_DEVICE_c == RSI_ENABLE*/

};
Endpoint_Description_t *gp_Endpoint_Descriptors = ga_Endpoint_Descriptors;

const Endpoint_Description_t  g_Broadcast_Endpoint_Descriptors =
{
	g_NULL_c, m_BROADCAST_END_POINT_c
};


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
