/*******************************************************************************
* ZLL_Light_Device.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for any ZLL Light device
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include <stdbool.h>

#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#include "ApptimerId.h"
#include "buffer_management.h"
//#include "timer.h"
#include "memory_utility.h"
#include "stack_common.h"
#include "zdo_interface.h"
#include "ZCL_Interface.h"
#include "app_configuration_default.h"
#include "ZCL_Foundation_Reporting.h"
#include "ApplicationThinLayer.h"
#include "AppReportingUtilities.h"
#include "AppZclDefaultAttributesData.h"

#include "AppLevelControlUtilities.h"

#if(g_BASIC_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Basic_Cluster.h"
#endif                       /* g_BASIC_CLUSTER_ENABLE_d */

#if(g_IDENTIFY_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Identify_Cluster.h"
#include "AppIdentifyUtilities.h"
#endif                                /* g_IDENTIFY_CLUSTER_ENABLE_d */


#if(g_GROUPS_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Groups_Cluster.h"
#endif                                /* g_GROUPS_CLUSTER_ENABLE_d */
#if(g_SCENES_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Scenes_Cluster.h"
#endif
                                  /* g_SCENES_CLUSTER_ENABLE_d */

#if(g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Level_Control_Cluster.h"
#include "AppLevelControlUtilities.h"
#endif                          /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d */

#if(g_ON_OFF_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Cluster.h"
#endif                           /* g_ON_OFF_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
#include "AppColorControlUtilities.h"
#endif 				 /*g_COLOR_CONTROL_CLUSTER_ENABLE_d*/

#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1)
    #include "ZLL_Commissioning_Cluster.h"
    #include "AppZllCommissioningClusterUtilities.h"
#endif

#include "ZLL_Process.h"
/*******************************************************************************
* Public Memory declarations
*******************************************************************************/


extern uint8_t ga_Timer_Link_Array[];

extern uint8_t g_Dst_EndPoint;

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
extern uint16_t g_identifyTime;
#endif

#if (g_CLI_SUPPORT_d == 1)
uint8_t overrideApsRetry;
#endif
/*******************************************************************************/


#if (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)
 /* BASIC SERVER CLUSTER VALUES */
Basic_Cluster_t basic_server_cluster_values;

/* BASIC SERVER CLUSTER DEFAULT VALUES */
const Basic_Cluster_t basic_server_cluster_default_values =
{
     g_ZCL_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#if ( g_APPLICATION_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_APPLICATION_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_STACK_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_STACK_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_HW_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_HW_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_MANUFACTURE_NAME_ATTRIBUTE_ENABLE_d == 1 )
    g_MANUFACTURE_NAME_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_MODEL_IDENTIFIER_ATTRIBUTE_ENABLE_d == 1 )
    g_MODEL_IDENTIFIER_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_DATE_CODE_ATTRIBUTE_ENABLE_d == 1 )
    g_DATE_CODE_ATTRIBUTE_DEFAULT_DATA_c,
#endif
    g_POWER_SOURCE_ATTRIBUTE_DEFAULT_DATA_c,
#if ( g_LOCATION_DESCRIPTION_ATTRIBUTE_ENABLE_d == 1 )
    g_LOCATION_DESCRIPTION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d == 1 )
    g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_DEVICE_ENABLED_ATTRIBUTE_ENABLE_d == 1 )
    g_DEVICE_ENABLED_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    g_ALARM_MASK_ATTRIBUTE_DEFAULT_DATA_c
#endif
};
#endif


/******************************************************************/

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
/* ZCL Identify cluster attribute application data strorage  */
Identify_Cluster_t identify_server_cluster_values;
#endif                              /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
/* ZCL Identify cluster default attribute data */
const Identify_Cluster_t identify_server_cluster_default_values =
{
    g_IDENTIFY_TIME_ATTRIBUTE_DEFAULT_DATA_c
};
#endif                              /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */



/******************************************************************/

#if( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
/* LEVEL CONTROL SERVER CLUSTER VALUES*/
Level_Control_Cluster_t level_control_server_cluster_values;



const Level_Control_Cluster_t level_control_server_cluster_default_values =
{
 /* LEVEL CONTROL SERVER CLUSTER DEFAULT VALUES*/
 g_CURRENT_LEVEL_ATTRIBUTE_DEFAULT_DATA_c,
#if(g_LEVEL_REMAINING_TIME_ATTRIBUTE_ENABLE_d == 1)
 g_REMAINING_TIME_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if(g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d == 1)
 g_ON_OFF_TRANSITION_TIME_DEFAULT_DATA_c,
#endif
#if (g_ON_LEVEL_ATTRIBUTE_ENABLE_d == 1)
 g_ON_LEVEL_DEFAULT_DATA_c
#endif
};
#endif

/******************************************************************/

#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
/* ON_OFF_CLUSTER_VALUES */
On_Off_Cluster_t on_off_server_cluster_values;
/* ON_OFF_CLUSTER_DEFAULT_VALUES */
On_Off_Cluster_t on_off_cluster_default_values =
{
g_ON_OFF_ATTRIBUTE_DEFAULT_DATA_c
};

#endif

/******************************************************************/
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
/* ZCL group cluster attribute application data strorage  */
Group_Cluster_t group_server_cluster_values;
#endif                              /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
/* ZCL group cluster default attribute application data strorage  */
const Group_Cluster_t group_server_cluster_default_values =
{
    g_GROUP_ATTRIBUTE_NAME_SUPPORT_ATTRIBUTE_DEFAULT_DATA_c
};
#endif                              /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */

/******************************************************************/

#if(g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
/* SCENES_SERVER_CLUSTER_VALUES */
Scenes_Cluster_t  Scenes_Cluster_server_values ;

/* SCENES_SERVER_CLUSTER_VALUES */
const Scenes_Cluster_t  Scenes_Cluster_server_default_values =
{
	g_SCENE_MGMT_INFO_SCENE_COUNT_ATTRIBUTE_DEFAULT_VALUE_c,
    g_SCENE_MGMT_INFO_CURRENT_SCENE_ATTRIBUTE_DEFAULT_VALUE_c,
    g_SCENE_MGMT_INFO_CURRENT_GROUP_ATTRIBUTE_DEFAULT_VALUE_c,
    g_SCENE_MGMT_INFO_SCENE_VALID_ATTRIBUTE_DEFAULT_VALUE_c,
    g_SCENE_MGMT_INFO_NAME_SUPPORT_ATTRIBUTE_DEFAULT_VALUE_c,
#if(g_SCENE_MGMT_INFO_LAST_CONFIGURED_BY_ATTRIBUTE_ENABLE_d == 1)
    g_SCENE_MGMT_INFO_LAST_CONFIGURED_BY_ATTRIBUTE_DEFAULT_VALUE_c
#endif
};
#endif


/******************************************************************/
#if( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )

color_control_attribute_data_t  Color_Control_Cluster_server_values ;

/* COLOR_CONTROL_SERVER_CLUSTER_DEFAULT_VALUES */
const color_control_attribute_data_t  Color_Control_Cluster_server_default_values ={
  #if (g_CURRENT_HUE_ATTRIBUTE_ENABLE_d == 1)
g_CURRENT_HUE_ATTRIBUTE_DEFAULT_DATA_c,
#endif           /*g_CURRENT_HUE_ATTRIBUTE_ENABLE_d*/
#if(g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d == 1)
g_CURRENT_SATURATION_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d*/
#if( g_REMAINING_TIME_ATTRIBUTE_ENABLE_d == 1)
g_REMAINING_TIME_ATTRIBUTE_DEFAULT_DATA_c,
#endif           /*g_REMAINING_TIME_ATTRIBUTE_ENABLE_d*/
g_CURRENT_X_ATTRIBUTE_DEFAULT_DATA_c,
g_CURRENT_Y_ATTRIBUTE_DEFAULT_DATA_c,
#if (g_DRIFT_COMPENSATION_ATTRIBUTE_ENABLE_d == 1)
g_DRIFT_COMPENSATION_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_DRIFT_COMPENSATION_ATTRIBUTE_ENABLE_d*/
#if (g_COMPENSATION_TEXT_ATTRIBUTE_ENABLE_d == 1)
g_COMPENSATION_TEXT_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_COMPENSATION_TEXT_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1)
g_COLOR_TEMPERATURE_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_COLOR_TEMPERATURE_ATTRIBUTE_ENABLE_d*/
#if( g_COLOR_MODE_ATTRIBUTE_ENABLE_d == 1)
g_COLOR_MODE_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_COLOR_MODE_ATTRIBUTE_ENABLE_d*/
#if ( g_NUMBER_OF_PRIMARIES_ATTRIBUTE_ENABLE_d == 1)
g_NUMBER_OF_PRIMARIES_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_NUMBER_OF_PRIMARIES_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_1X_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_1X_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_1X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_1Y_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_1Y_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_PRIMARY_1Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_1_INTENSITY_ATTRIBUTE_ENABLE_d == 1)
g_PRIMARY_1_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_PRIMARY_1_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_2X_ATTRIBUTE_ENABLE_d == 1)
g_PRIMARY_2X_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_2X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_2Y_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_2Y_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_2Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_2_INTENSITY_ATTRIBUTE_ENABLE_d == 1)
g_PRIMARY_2_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_2_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_3X_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_3X_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_3X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_3Y_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_3Y_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_PRIMARY_3Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_3_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_3_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_PRIMARY_3_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_4X_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_4X_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_4X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_4Y_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_4Y_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_PRIMARY_4Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_4_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_4_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_PRIMARY_4_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_5X_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_5X_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_PRIMARY_5X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_5Y_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_5Y_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_PRIMARY_5Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_5_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_5_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_5_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_6X_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_6X_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_6X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_6Y_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_6Y_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_6Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_6_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
g_PRIMARY_6_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_PRIMARY_6_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_WHITE_POINT_X_ATTRIBUTE_ENABLE_d == 1 )
g_WHITE_POINT_X_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_WHITE_POINT_X_ATTRIBUTE_ENABLE_d*/
#if ( g_WHITE_POINT_Y_ATTRIBUTE_ENABLE_d == 1 )
g_WHITE_POINT_Y_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_WHITE_POINT_Y_ATTRIBUTE_ENABLE_d*/
#if( g_COLOR_POINT_RX_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_RX_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_COLOR_POINT_RX_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_RY_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_RY_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_COLOR_POINT_RY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_R_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_R_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_COLOR_POINT_R_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_GX_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_GX_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_COLOR_POINT_GX_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_GY_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_GY_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_COLOR_POINT_GY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_G_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_G_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_COLOR_POINT_G_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_BX_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_BX_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_COLOR_POINT_BX_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_BY_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_BY_ATTRIBUTE_DEFAULT_DATA_c ,
#endif          /*g_COLOR_POINT_BY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_B_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
g_COLOR_POINT_B_INTENSITY_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_COLOR_POINT_B_INTENSITY_ATTRIBUTE_ENABLE_d*/

};
#endif          /* COLOR_CONTROL_SERVER_CLUSTER_VALUES */

/******************************************************************/

	/************ On Off Light Device ********************/
const App_Cluster_Def_t a_Server_Cluster_List_For_On_Off_Light[]=
{
	/* Mandatory Cluster*/
#if (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)
   { &ga_Basic_Server_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd,
   		&basic_server_cluster_values, &basic_server_cluster_default_values }
#endif

#if(g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==1)
   ,{ &ga_Identify_Server_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd,
   &identify_server_cluster_values, &identify_server_cluster_default_values }
#endif

#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_On_Off_Server_Cluster_Info,ZCL_Handle_OnOff_Cluster_Cmd,
   &on_off_server_cluster_values, &on_off_cluster_default_values }
#endif

#if(g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Group_Server_Cluster_Info, ZCL_Handle_Groups_Cluster_Cmd,
   &group_server_cluster_values, &group_server_cluster_default_values }
#endif

#if(g_SCENES_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Scenes_Server_Cluster_Info, ZCL_Handle_Scenes_Cluster_Cmd,
   &Scenes_Cluster_server_values, &Scenes_Cluster_server_default_values }
#endif

#if(g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Level_Control_Server_Cluster_Info, ZCL_Handle_Level_Control_Cluster_Cmd,
   &level_control_server_cluster_values,
     &level_control_server_cluster_default_values }
#endif

#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,{&ga_ZLL_Commissioning_Cluster_Server_Cluster_Info,ZLL_Handle_Commissioning_Cluster_Cmd,
		g_NULL_c,g_NULL_c}
#endif	 /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/


	,{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};

	/* On Off Light - CLient side cluster Info*/
const App_Cluster_Def_t a_Client_Cluster_List_For_On_Off_Light[]=
{

	{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};



	/************ On Off Plug-In Unit Light Device ********************/
const App_Cluster_Def_t a_Server_Cluster_List_For_On_Off_Plug_In_Unit_Light[]=
{
	/* Mandatory Cluster*/
#if (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)
   { &ga_Basic_Server_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd,
   		&basic_server_cluster_values, &basic_server_cluster_default_values }
#endif

#if(g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==1)
   ,{ &ga_Identify_Server_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd,
   &identify_server_cluster_values, &identify_server_cluster_default_values }
#endif

#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_On_Off_Server_Cluster_Info,ZCL_Handle_OnOff_Cluster_Cmd,
   &on_off_server_cluster_values, &on_off_cluster_default_values }
#endif

#if(g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Group_Server_Cluster_Info, ZCL_Handle_Groups_Cluster_Cmd,
   &group_server_cluster_values, &group_server_cluster_default_values }
#endif

#if(g_SCENES_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Scenes_Server_Cluster_Info, ZCL_Handle_Scenes_Cluster_Cmd,
   &Scenes_Cluster_server_values, &Scenes_Cluster_server_default_values }
#endif

#if(g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Level_Control_Server_Cluster_Info, ZCL_Handle_Level_Control_Cluster_Cmd,
   &level_control_server_cluster_values,
     &level_control_server_cluster_default_values }
#endif
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,{&ga_ZLL_Commissioning_Cluster_Server_Cluster_Info,ZLL_Handle_Commissioning_Cluster_Cmd,
		g_NULL_c,g_NULL_c}
#endif	 /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/

	,{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};

	/* On Off Plug -in unit Light - CLient side cluster Info*/
const App_Cluster_Def_t a_Client_Cluster_List_For_On_Off_Plug_In_Unit_Light[]=
{

	{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};


	/************ Dimmable Light Device ********************/
const App_Cluster_Def_t a_Server_Cluster_List_For_Dimmable_Light[]=
{
	/* Mandatory Cluster*/
#if (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)
   { &ga_Basic_Server_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd,
   		&basic_server_cluster_values, &basic_server_cluster_default_values }
#endif

#if(g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==1)
   ,{ &ga_Identify_Server_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd,
   &identify_server_cluster_values, &identify_server_cluster_default_values }
#endif

#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_On_Off_Server_Cluster_Info,ZCL_Handle_OnOff_Cluster_Cmd,
   &on_off_server_cluster_values, &on_off_cluster_default_values }
#endif

#if(g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Group_Server_Cluster_Info, ZCL_Handle_Groups_Cluster_Cmd,
   &group_server_cluster_values, &group_server_cluster_default_values }
#endif

#if(g_SCENES_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Scenes_Server_Cluster_Info, ZCL_Handle_Scenes_Cluster_Cmd,
   &Scenes_Cluster_server_values, &Scenes_Cluster_server_default_values }
#endif
#if(g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Level_Control_Server_Cluster_Info, ZCL_Handle_Level_Control_Cluster_Cmd, &level_control_server_cluster_values,
     &level_control_server_cluster_default_values }
#endif
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,{&ga_ZLL_Commissioning_Cluster_Server_Cluster_Info,ZLL_Handle_Commissioning_Cluster_Cmd,
		g_NULL_c,g_NULL_c}
#endif	 /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/

	,{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};

	/* Dimmable Light - CLient side cluster Info*/
const App_Cluster_Def_t a_Client_Cluster_List_For_Dimmable_Light[]=
{

	{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};


	/************ Dimmable Plug-In Unit Light Device ********************/
const App_Cluster_Def_t a_Server_Cluster_List_For_Dimmable_Plug_In_Unit_Light[]=
{
	/* Mandatory Cluster*/
#if (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)
   { &ga_Basic_Server_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd,
   		&basic_server_cluster_values, &basic_server_cluster_default_values }
#endif

#if(g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==1)
   ,{ &ga_Identify_Server_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd,
   &identify_server_cluster_values, &identify_server_cluster_default_values }
#endif

#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_On_Off_Server_Cluster_Info,ZCL_Handle_OnOff_Cluster_Cmd,
   &on_off_server_cluster_values, &on_off_cluster_default_values }
#endif

#if(g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Group_Server_Cluster_Info, ZCL_Handle_Groups_Cluster_Cmd,
   &group_server_cluster_values, &group_server_cluster_default_values }
#endif

#if(g_SCENES_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Scenes_Server_Cluster_Info, ZCL_Handle_Scenes_Cluster_Cmd,
   &Scenes_Cluster_server_values, &Scenes_Cluster_server_default_values }
#endif
#if(g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Level_Control_Server_Cluster_Info, ZCL_Handle_Level_Control_Cluster_Cmd,
   &level_control_server_cluster_values, &level_control_server_cluster_default_values }
#endif
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,{&ga_ZLL_Commissioning_Cluster_Server_Cluster_Info,ZLL_Handle_Commissioning_Cluster_Cmd,
		g_NULL_c,g_NULL_c}
#endif	 /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/

	,{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};
/* Dimmable Plug -in Unit Light - CLient side cluster Info*/
const App_Cluster_Def_t a_Client_Cluster_List_For_Dimmable_Plug_In_Unit_Light[]=
{

	{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};


	/************ Color Light Device ********************/
const App_Cluster_Def_t a_Server_Cluster_List_For_Color_Light[]=
{
	/* Mandatory Cluster*/
#if (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)
   { &ga_Basic_Server_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd,
   		&basic_server_cluster_values, &basic_server_cluster_default_values }
#endif

#if(g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==1)
   ,{ &ga_Identify_Server_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd,
   &identify_server_cluster_values, &identify_server_cluster_default_values }
#endif

#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_On_Off_Server_Cluster_Info,ZCL_Handle_OnOff_Cluster_Cmd,
   &on_off_server_cluster_values, &on_off_cluster_default_values }
#endif

#if(g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Group_Server_Cluster_Info, ZCL_Handle_Groups_Cluster_Cmd,
   &group_server_cluster_values, &group_server_cluster_default_values }
#endif

#if(g_SCENES_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Scenes_Server_Cluster_Info, ZCL_Handle_Scenes_Cluster_Cmd,
   &Scenes_Cluster_server_values, &Scenes_Cluster_server_default_values }
#endif
#if(g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Level_Control_Server_Cluster_Info, ZCL_Handle_Level_Control_Cluster_Cmd,
   &level_control_server_cluster_values, &level_control_server_cluster_default_values }
#endif

#if(g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Color_Control_Server_Cluster_Info, ZCL_Handle_Color_Ctrl_Cluster_Cmd,
   &Color_Control_Cluster_server_values,
   &Color_Control_Cluster_server_default_values}
#endif                      /*  g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,{&ga_ZLL_Commissioning_Cluster_Server_Cluster_Info,ZLL_Handle_Commissioning_Cluster_Cmd,
		g_NULL_c,g_NULL_c}
#endif	 /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/

	,{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};

	/* Color Light - CLient side cluster Info*/
const App_Cluster_Def_t a_Client_Cluster_List_For_Color_Light[]=
{

	{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};


	/************ Extended Color Light Device ********************/
const App_Cluster_Def_t a_Server_Cluster_List_For_Extended_Color_Light[]=
{
	/* Mandatory Cluster*/
#if (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)
   { &ga_Basic_Server_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd,
   		&basic_server_cluster_values, &basic_server_cluster_default_values }
#endif

#if(g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==1)
   ,{ &ga_Identify_Server_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd,
   &identify_server_cluster_values, &identify_server_cluster_default_values }
#endif

#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_On_Off_Server_Cluster_Info,ZCL_Handle_OnOff_Cluster_Cmd,
   &on_off_server_cluster_values, &on_off_cluster_default_values }
#endif

#if(g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Group_Server_Cluster_Info, ZCL_Handle_Groups_Cluster_Cmd,
   &group_server_cluster_values, &group_server_cluster_default_values }
#endif

#if(g_SCENES_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Scenes_Server_Cluster_Info, ZCL_Handle_Scenes_Cluster_Cmd,
   &Scenes_Cluster_server_values, &Scenes_Cluster_server_default_values }
#endif
#if(g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Level_Control_Server_Cluster_Info, ZCL_Handle_Level_Control_Cluster_Cmd,
   &level_control_server_cluster_values, &level_control_server_cluster_default_values }
#endif

#if(g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Color_Control_Server_Cluster_Info, ZCL_Handle_Color_Ctrl_Cluster_Cmd,
   &Color_Control_Cluster_server_values,
   &Color_Control_Cluster_server_default_values}
#endif                      /*  g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,{&ga_ZLL_Commissioning_Cluster_Server_Cluster_Info,ZLL_Handle_Commissioning_Cluster_Cmd,
		g_NULL_c,g_NULL_c}
#endif	 /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/


	,{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};

	/* Color Light - CLient side cluster Info*/
const App_Cluster_Def_t a_Client_Cluster_List_For_Extended_Color_Light[]=
{

	{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};



	/************ Color Temperature Light Device ********************/
const App_Cluster_Def_t a_Server_Cluster_List_For_Color_Temperature_Light[]=
{
	/* Mandatory Cluster*/
#if (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)
   { &ga_Basic_Server_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd,
   		&basic_server_cluster_values, &basic_server_cluster_default_values }
#endif

#if(g_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==1)
   ,{ &ga_Identify_Server_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd,
   &identify_server_cluster_values, &identify_server_cluster_default_values }
#endif

#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_On_Off_Server_Cluster_Info,ZCL_Handle_OnOff_Cluster_Cmd,
   &on_off_server_cluster_values, &on_off_cluster_default_values }
#endif

#if(g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Group_Server_Cluster_Info, ZCL_Handle_Groups_Cluster_Cmd,
   &group_server_cluster_values, &group_server_cluster_default_values }
#endif

#if(g_SCENES_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Scenes_Server_Cluster_Info, ZCL_Handle_Scenes_Cluster_Cmd,
   &Scenes_Cluster_server_values, &Scenes_Cluster_server_default_values }
#endif
#if(g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Level_Control_Server_Cluster_Info, ZCL_Handle_Level_Control_Cluster_Cmd,
   &level_control_server_cluster_values, &level_control_server_cluster_default_values }
#endif

#if(g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
   ,{ &ga_Color_Control_Server_Cluster_Info, ZCL_Handle_Color_Ctrl_Cluster_Cmd,
   &Color_Control_Cluster_server_values,
   &Color_Control_Cluster_server_default_values}
#endif                      /*  g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
	,{&ga_ZLL_Commissioning_Cluster_Server_Cluster_Info,ZLL_Handle_Commissioning_Cluster_Cmd,
		g_NULL_c,g_NULL_c}
#endif	 /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/


	,{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};

	/* ColorTemperature Light - CLient side cluster Info*/
const App_Cluster_Def_t a_Client_Cluster_List_For_Color_Temperature_Light[]=
{

	{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c}
};



/*******************************************************************************
* Private Memory declarations
*******************************************************************************/
/* No of devices supported in a node */
const ZCL_Device_Def_t AppDeviceListServerInfo[g_MAX_NO_OF_ENDPOINTS_c] =
{
    {0xFe,0x00,g_NULL_c}
	/* Atleast one device should be supported*/
#if(g_ON_OFF_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_ON_OFF_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Server_Cluster_List_For_On_Off_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Server_Cluster_List_For_On_Off_Light }
#endif /* g_ON_OFF_LIGHT_DEVICE_c == RSI_ENABLE*/
#if(g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)
    ,{ g_ON_OFF_PLUG_IN_UNIT_ENDPOINT_ID_c,
		sizeof( a_Server_Cluster_List_For_On_Off_Plug_In_Unit_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Server_Cluster_List_For_On_Off_Plug_In_Unit_Light }
#endif /* g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE*/

#if(g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_DIMMABLE_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Server_Cluster_List_For_Dimmable_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Server_Cluster_List_For_Dimmable_Light }
#endif /* g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE*/

#if(g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)
    ,{ g_DIMMABLE_PLUG_IN_UNIT_ENDPOINT_ID_c,
		sizeof( a_Server_Cluster_List_For_Dimmable_Plug_In_Unit_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Server_Cluster_List_For_Dimmable_Plug_In_Unit_Light }
#endif /* g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE*/

#if(g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_COLOR_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Server_Cluster_List_For_Color_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Server_Cluster_List_For_Color_Light }
#endif /* g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE*/

#if(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_EXTENDED_COLOR_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Server_Cluster_List_For_Extended_Color_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Server_Cluster_List_For_Extended_Color_Light }
#endif /* g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE*/

#if(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_COLOR_TEMPERATURE_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Server_Cluster_List_For_Color_Temperature_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Server_Cluster_List_For_Color_Temperature_Light }
#endif /* g_COLOR_TEMPERATURE_LIGHT_DEVICE_c == RSI_ENABLE*/

	/* No more devices*/
	,{0xfe,0x00,g_NULL_c}
  ,{0xfe,0x00,g_NULL_c}

};
/* No of devices supported in a node */
const ZCL_Device_Def_t AppDeviceListClientInfo[g_MAX_NO_OF_ENDPOINTS_c] =
{
    {0xFe,0x00,g_NULL_c}
	/* Atleast one device should be supported*/
#if(g_ON_OFF_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_ON_OFF_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Client_Cluster_List_For_On_Off_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Client_Cluster_List_For_On_Off_Light }
#endif /* g_ON_OFF_LIGHT_DEVICE_c == RSI_ENABLE*/
#if(g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)
    ,{ g_ON_OFF_PLUG_IN_UNIT_ENDPOINT_ID_c,
		sizeof( a_Client_Cluster_List_For_On_Off_Plug_In_Unit_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Client_Cluster_List_For_On_Off_Plug_In_Unit_Light }
#endif /* g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE*/

#if(g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_DIMMABLE_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Client_Cluster_List_For_Dimmable_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Client_Cluster_List_For_Dimmable_Light }
#endif /* g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE*/

#if(g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)
    ,{ g_DIMMABLE_PLUG_IN_UNIT_ENDPOINT_ID_c,
		sizeof( a_Client_Cluster_List_For_Dimmable_Plug_In_Unit_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Client_Cluster_List_For_Dimmable_Plug_In_Unit_Light }
#endif /* g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE*/

#if(g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_COLOR_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Client_Cluster_List_For_Color_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Client_Cluster_List_For_Color_Light }
#endif /* g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE*/

#if(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_EXTENDED_COLOR_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Client_Cluster_List_For_Extended_Color_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Client_Cluster_List_For_Extended_Color_Light }
#endif /* g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE*/

#if(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c == RSI_ENABLE)
    ,{ g_COLOR_TEMPERATURE_LIGHT_ENDPOINT_ID_c,
		sizeof( a_Client_Cluster_List_For_Color_Temperature_Light )/
               sizeof( App_Cluster_Def_t ),
			   a_Client_Cluster_List_For_Color_Temperature_Light }
#endif /* g_COLOR_TEMPERATURE_LIGHT_DEVICE_c == RSI_ENABLE*/

	/* No more devices*/
	,{0xfe,0x00,g_NULL_c}
  ,{0xfe,0x00,g_NULL_c}

};


/******************************************************************************/
void Handle_ZCL_Message( uint8_t buffer_index, APSDE_Data_Indication_t *p_buffer )
{
	uint8_t Status;
    App_Cluster_Def_t *p_cluster_definition;
    APSDE_Data_Request_t * pAppDataReq;
    ZCL_ActionHandler_t pActionHandler;
    uint8_t rspbufferindex, eventbufferindex;
    uint8_t* pLocationForResponse;

	/* To check whether it is generic command or cluster specific command*/
    if ( ( ( p_buffer->a_asdu[0] & g_ZCL_FRAME_TYPE_MASK_c) == g_ZCL_FC_PROFILE_WIDE_COMMAND_c )
           &&( p_buffer->a_asdu[2] == g_Default_Response_Command_c ) ) {
            Send_Response_To_Application( p_buffer, buffer_index );
            buffMgmt_freeBuffer( buffer_index, 224 );
            return;
	}

	/* Get the cluster details*/
    Status =  App_Get_Cluster_Definition( ZCL_GetDirectionOfCommand( p_buffer->a_asdu ),
                                          p_buffer->dest_endpoint,
                                          p_buffer->cluster_id,
                                          &p_cluster_definition );

	/* Check whether retrieving cluster definition is success or failure*/

	if( Status == g_ZCL_Success_c )
    {
        rspbufferindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
        if( rspbufferindex != g_NO_AVAILABLE_BUFFER_c ){

#if ( DEBUG_TEST == 1 )
            allocatedbuffer[rspbufferindex]= 91;
            freebuffer[rspbufferindex][0]= 91;
            freebuffer[rspbufferindex][1]= 0;
#endif
            eventbufferindex = buffMgmt_allocateBuffer ( g_SMALL_BUFFER_c );

            if ( eventbufferindex != g_NO_AVAILABLE_BUFFER_c ){

#if ( DEBUG_TEST == 1 )
                allocatedbuffer[eventbufferindex]= 91;
                freebuffer[eventbufferindex][0]= 91;
                freebuffer[eventbufferindex][1]= 0;
#endif

                pLocationForResponse = buffMgmt_getBufferPointer( rspbufferindex );

                *pLocationForResponse = g_APSDE_Data_Request_c;

                pAppDataReq = &((APSDE_Request_t *)pLocationForResponse)->
                                APSDE_Request_Primitive.apsde_data_request;

                pLocationForResponse = pAppDataReq->asdu;

                pActionHandler.event.pEventData = buffMgmt_getBufferPointer( eventbufferindex );

                pActionHandler.event.eventId = g_NULL_c;
                pActionHandler.action = No_Response;

                pAppDataReq->asdulength = ZCL_Process_Message( p_buffer->dest_endpoint,
                                                p_buffer->asdulength,
                                                p_buffer->a_asdu,
                                                pLocationForResponse,
                                                &pActionHandler,
                                                p_cluster_definition );

                if( pActionHandler.action == Send_Response ){
                    if ( ( ( * ( pLocationForResponse + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Default_Response_Command_c )
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
                       ||
                       ( * ( pLocationForResponse + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Remove_Scene_Response_Command_c )  ||
                       ( * ( pLocationForResponse + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Remove_All_Scenes_Response_Command_c )  ||
                       ( * ( pLocationForResponse + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Store_Scene_Response_Command_c )
#endif
                     ) && ( ( p_buffer->dest_addr_mode == g_Group_Address_Mode_c ) ||
                            ( ( p_buffer->dest_addr_mode == g_Short_Address_Mode_c ) &&
                              (( p_buffer->dest_address.short_address == g_BROADCAST_ADDRESS_c ) ||
                              ( p_buffer->dest_address.short_address == g_BROADCAST_ADDRESS_ACTIVE_DEVICES_c ) ||
                              ( p_buffer->dest_address.short_address == g_BROADCAST_ADDRESS_FOR_ZC_AND_ZR_c ) ) ) )){

                         buffMgmt_freeBuffer( rspbufferindex, 224 );
                    }
                    else{
                        memUtils_memCopy( pAppDataReq->dest_address.IEEE_address, p_buffer->src_address.IEEE_address, 8 );
                        pAppDataReq->dest_addr_mode = p_buffer->src_addr_mode;
                        pAppDataReq->dest_endpoint = p_buffer->src_endpoint;
                        pAppDataReq->src_endpoint = p_buffer->dest_endpoint;
                        pAppDataReq->profile_id  = p_buffer->profile_id;
                        pAppDataReq->cluster_id = p_buffer->cluster_id;
                        pAppDataReq->txoptions = App_getClusterSecurity( pAppDataReq->cluster_id );
#if (g_CLI_SUPPORT_d == 1)
							if( overrideApsRetry ==1){
								pAppDataReq->txoptions = pAppDataReq->txoptions;
							}
							else if( overrideApsRetry ==2) {
							    pAppDataReq->txoptions = pAppDataReq->txoptions|g_APS_ACK_REQUIRED_c;
							}
#endif

                        pAppDataReq->radius = g_DEFAULT_RADIUS_c;
                        APS_APSDErequest( rspbufferindex );
                  }
               }
               else if ( pActionHandler.action == Received_Response ){
                   Send_Response_To_Application( p_buffer, buffer_index );
				   buffMgmt_freeBuffer( rspbufferindex, 224 );
               }
               else{
                   buffMgmt_freeBuffer( rspbufferindex, 224 );
               }

			   /* Note :Application Should handle the event for
						specific device , for now  event is not handled
						seperately for each individual device in a node
						ex: Controller might support so many devices , such as
						    on of light , dimmable light etc,
						    so reset to factory event is for which sub device
						    application should write a code to handle this */

               switch ( pActionHandler.event.eventId )
               {
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_Reset_To_Factory_Defaults_Events_c :
                        App_ZCL_Data_Init();
				   		Send_Event_To_Application ( &pActionHandler.event );
                        break;
#endif                              /* ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 ) */


#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_Identify_Event_c :
                    case g_Identify_Query_Event_c :
#if (g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d ==1)
			   		case g_Trigger_Effect_Event_c :
#endif /*g_ZLL_IDENTIFY_CLUSTER_ENABLE_d == 1*/
                        App_IdentifyEventsHandler( &pActionHandler.event, p_buffer );
                        Send_Event_To_Application ( &pActionHandler.event );
                        break;
#endif                   /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_Add_Group_Command_Events_c:
                    case g_View_Group_Command_Events_c:
                    case g_Remove_Group_Command_Events_c:
                    case g_Remove_All_Groups_Command_Events_c:
                    case g_Get_Group_Membership_Command_Events_c:
                    case g_Add_Group_If_Identifying_Command_Events_c :
				   		Send_Event_To_Application ( &pActionHandler.event );
                        break;
#endif                                            /* g_GROUPS_CLUSTER_ENABLE_d */

#if( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_Add_Scene_Command_Event_c:
                    case g_View_Scene_Command_Event_c:
                    case g_Remove_Scene_Command_Event_c:
                    case g_Remove_All_Scenes_Command_Event_c:
                    case g_Store_Scene_Command_Event_c:
                    case g_Scene_Membership_Command_Event_c :

#if(g_ZLL_SCENES_SERVER_CLUSTER_ENABLE_d==1)
				case g_Enhanced_Add_Scene_Command_Event_c:
				case g_Enhanced_View_Scene_Command_Event_c:
				case g_Copy_Scene_Command_Event_c :
#endif /*g_ZLL_SCENES_SERVER_CLUSTER_ENABLE_d */

				   		Send_Event_To_Application ( &pActionHandler.event );
                        break;
#endif                       /* g_SCENES_CLUSTER_ENABLE_d */

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
                        /* On off cluster event handling */
                    case g_On_Off_Cluster_On_Event_c :
                    case g_On_Off_Cluster_Off_Event_c :
                    case g_On_Off_Cluster_Toggle_Event_c :
#if(g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d==1)
			   case g_Off_With_Effect_Event_c:
			   case g_On_With_Recall_Global_Scene_Event_c:
			   case g_On_With_Timed_Off_Event_c :
#endif /*	g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d */
				   		Send_Event_To_Application ( &pActionHandler.event );
                        break;
#endif                                            /* g_ON_OFF_CLUSTER_ENABLE_d */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_Move_To_Level_Event_c:
                    case g_Move_To_Level_With_On_Off_Event_c:
                    case g_Move_Event_c:
                    case g_Move_With_On_Off_Event_c:
                    case g_Step_With_On_Off_Event_c:
                    case g_Step_Event_c :
                    case g_Stop_Event_c:
                    case g_Stop_With_On_Off_Event_c:
                        Handle_Level_Control_Cluster_Events( &pActionHandler.event, p_buffer->dest_endpoint );
				   		Send_Event_To_Application ( &pActionHandler.event );
                        break;
#endif                              /* g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )

                    /* setting the move to hue mode */
                case g_Move_to_Hue_Command_Event_c:
                    /* setting the move hue mode */
                case g_Move_hue_Command_Event_c:
                    /* setting the step hue mode */
                case g_Step_Hue_Command_Event_c:
                    /* setting the move to saturation mode */
                case g_Move_To_Saturation_Command_Event_c:
                    /* setting the move saturation mode */
                case g_Move_Saturation_Command_Event_c:
                    /* setting the step saturation mode */
                case g_Step_Saturation_Command_Event_c:
                    /* setting the move to hue and saturation mode */
                case g_Move_To_Hue_And_Saturation_Command_Event_c:
                    /* setting the move to color tepmerature mode */
                case g_Move_To_Color_Temperature_Command_Event_c:
                    /* setting the move to color mode */
                case g_Move_To_Color_Command_Event_c:
                    /* setting the move color mode */
                case g_Move_Color_Command_Event_c:
                    /* setting the step color mode */
                case g_Step_Color_Command_Event_c:
                     /* setting the enhanced move to hue mode */
#if( g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d==1)     /*    g_ZLL_COLOR_CONTROL_CLUSTER_ENABLE_d */
				  /*setting enhanced move to hue mode */
			   case g_Enhanced_Move_To_Hue_Command_Event_c:
				 /*setting enhanced move hue mode */
			   case g_Enhanced_Move_Hue_Command_Event_c:
				 /*setting enhanced step hue mode */
			   case g_Enhanced_Step_Hue_Command_Event_c:
				 /*setting enhanced move to hue and saturation mode */
			   case g_Enhanced_Move_To_Hue_And_Saturation_Command_Event_c:
				 /*  setting color loop set mode */
			   case g_Color_Loop_Set_Event_c:
				 /*setting stop move step mode */
			   case g_Stop_Move_Step_Event_c:
				 /*setting move color temperature mode */
			   case g_Move_Color_Temperature_Event_c:
				 /*setting step color temperature mode */
			   case g_Step_Color_Temperature_Event_c:
#endif     /* g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */


                   App_Handle_Color_Control_Cluster_Events( &pActionHandler.event,
														   p_buffer->dest_endpoint );
				   Send_Event_To_Application ( &pActionHandler.event );
                      break;
#endif              /* g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */

                     default :
                        break;
               }
			   buffMgmt_freeBuffer( eventbufferindex, 224 );
            }
            else{
                buffMgmt_freeBuffer( rspbufferindex, 224 );
            }
        }
    }
    else{
        /* failed to retrieve the cluster details */
        App_SendDefaultResponse( g_ZCL_Failure_c,
                                 m_GET_g_NWK_SecurityLevel_c ?
                                 g_APS_Tx_Opt_Use_NWK_Key_c : g_APS_Tx_Opt_Unsecured_c,
                                 p_buffer );
    }
    buffMgmt_freeBuffer( buffer_index, 224 );
    return;

}

/***********************************************************************************/
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void Handle_ZCL_InterPan_Message( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
    /*Check if Commissioning cluster*/
    if( pApsdeDataInd->clusterId == g_ZLL_COMMISSIONING_CLUSTER_c )
    {
      /*parse the commands*/
      switch( pApsdeDataInd->a_asdu[2])
      {
        case g_ZLL_Scan_Req_Command_c:
        case g_ZLL_Scan_Response_Command_c:
          ZLL_Process_ScanCommands( buffer_index, pApsdeDataInd);
          break;

        case g_ZLL_Device_Information_Req_Command_c:
        case g_ZLL_Device_Information_Response_Command_c:
          ZLL_Process_DeviceInformationCommands(  buffer_index,  pApsdeDataInd);
          break;

        case g_ZLL_Identify_Req_Command_c:
          ZLL_Process_IdentifyRequestCommands(  buffer_index, pApsdeDataInd);
          break;

        case g_ZLL_Reset_To_Factory_New_Req_Command_c:
          ZLL_Process_ResetToFactory( buffer_index, pApsdeDataInd);
          break;

        case g_ZLL_Network_Start_Req_Command_c:
        case g_ZLL_Nwk_Start_Response_Command_c:
          ZLL_Process_NetworkStartCommands(  buffer_index, pApsdeDataInd);
          break;

        case g_ZLL_Network_Join_Router_Req_Command_c:
        case g_ZLL_Network_Join_Router_Response_Command_c:
        case g_ZLL_Network_Join_End_Device_Response_Command_c:
        case g_ZLL_Network_Join_End_Device_Req_Command_c:
          ZLL_Process_NetworkJoinCommands( buffer_index, pApsdeDataInd);
          break;


        case g_ZLL_Network_Update_Req_Command_c:
          ZLL_Process_NetworkUpdateCommands(  buffer_index,  pApsdeDataInd);
          break;

      }  /* end switch */

    } /* end if */
    buffMgmt_freeBuffer( buffer_index, 224 );
    return;
}
#endif /* g_INTERPAN_DATA_ENABLE_d == 1*/
/******************************************************************************
* End Of File
*******************************************************************************/
