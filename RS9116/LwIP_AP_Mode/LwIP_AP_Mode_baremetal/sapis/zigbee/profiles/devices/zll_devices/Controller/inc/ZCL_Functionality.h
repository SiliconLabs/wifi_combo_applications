#ifndef _ZCL_FUNCTIONALITY_H_
#define _ZCL_FUNCTIONALITY_H_

/******************************************************************************
* Includes
******************************************************************************/

#include "ZLL_Devices.h"
#include "ZLL_Configuration.h"

/******************************************************************************
* Global Constants
******************************************************************************/

/* Cluster functionality */
		/* Common for all device */
#define g_BASIC_CLUSTER_ENABLE_d                                        1

#define g_LEVEL_CONTROL_CLUSTER_ENABLE_d                                1
#define g_GROUPS_CLUSTER_ENABLE_d                                       1
#define g_SCENES_CLUSTER_ENABLE_d                                       1
#define g_IDENTIFY_CLUSTER_ENABLE_d                                     1
#define g_ON_OFF_CLUSTER_ENABLE_d                                       1
#define g_COLOR_CONTROL_CLUSTER_ENABLE_d                                1
#define g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d							1





/* ********************Controller Device - Enabling the Clusters**************/
/* Basic cluster is supported for all device */
#define g_BASIC_SERVER_CLUSTER_ENABLE_d                                 1
#define g_BASIC_CLIENT_CLUSTER_ENABLE_d                                 1
#define g_ZLL_BASIC_SERVER_CLUSTER_ENABLE_d								1
#define g_ZLL_SCENES_CLUSTER_ENABLE_d                           		 1
#define g_ZLL_COLOR_CONTROL_CLUSTER_ENABLE_d                            1
#define g_ZLL_IDENTIFY_CLUSTER_ENABLE_d                             1

#ifdef g_ZLL_LIGHT_CONTROLLER_c
/* ZLL commissioning cluster as server and client is enabled for all controller
device type*/
#define g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d						1
#define g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d						1



/* Based device cluster can be enabled or disabled */
#if(( g_COLOR_CONTROLLER_DEVICE_c == RSI_ENABLE) || (g_COLOR_SCENE_CONTROLLER_DEVICE_c == RSI_ENABLE)        \
|| ( g_NON_COLOR_CONTROLLER_DEVICE_c ==RSI_ENABLE ) || (g_NON_COLOR_SCENE_CONTROLLER_DEVICE_c ==RSI_ENABLE) \
||(g_CONTROL_BRIDGE_DEVICE_c == RSI_ENABLE) || (g_ON_OFF_SENSOR_DEVICE_c == RSI_ENABLE))
#define g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d                            	1
#define g_ZLL_IDENTIFY_CLIENT_CLUSTER_ENABLE_d							1
#endif /* End of Identify Cluster supported */

#if(( g_COLOR_CONTROLLER_DEVICE_c == RSI_ENABLE) || (g_COLOR_SCENE_CONTROLLER_DEVICE_c == RSI_ENABLE)        \
|| ( g_NON_COLOR_CONTROLLER_DEVICE_c ==RSI_ENABLE ) || (g_NON_COLOR_SCENE_CONTROLLER_DEVICE_c ==RSI_ENABLE) \
||(g_CONTROL_BRIDGE_DEVICE_c == RSI_ENABLE) || (g_ON_OFF_SENSOR_DEVICE_c == RSI_ENABLE))

#define g_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d                         1
#define g_ZLL_LEVEL_CONTROL_CLIENT_CLUSTER_ENABLE_d                     1
#endif /* End of Level Control Clsuter */


#if(( g_COLOR_CONTROLLER_DEVICE_c == RSI_ENABLE) || (g_COLOR_SCENE_CONTROLLER_DEVICE_c == RSI_ENABLE)        \
|| ( g_NON_COLOR_CONTROLLER_DEVICE_c ==RSI_ENABLE ) || (g_NON_COLOR_SCENE_CONTROLLER_DEVICE_c ==RSI_ENABLE) \
||(g_CONTROL_BRIDGE_DEVICE_c == RSI_ENABLE) || (g_ON_OFF_SENSOR_DEVICE_c == RSI_ENABLE))

#define g_GROUPS_CLIENT_CLUSTER_ENABLE_d                                1

#endif /* End of Group cluster */



#if( (g_COLOR_SCENE_CONTROLLER_DEVICE_c == RSI_ENABLE)        \
||  (g_NON_COLOR_SCENE_CONTROLLER_DEVICE_c ==RSI_ENABLE) \
||(g_CONTROL_BRIDGE_DEVICE_c == RSI_ENABLE) || (g_ON_OFF_SENSOR_DEVICE_c == RSI_ENABLE))

#define g_SCENES_CLIENT_CLUSTER_ENABLE_d                                1
#define g_ZLL_SCENES_CLIENT_CLUSTER_ENABLE_d							1
#endif /* End of Scenes cluster */


#if(( g_COLOR_CONTROLLER_DEVICE_c == RSI_ENABLE) || (g_COLOR_SCENE_CONTROLLER_DEVICE_c == RSI_ENABLE)        \
|| ( g_NON_COLOR_CONTROLLER_DEVICE_c == RSI_ENABLE ) || (g_NON_COLOR_SCENE_CONTROLLER_DEVICE_c ==RSI_ENABLE) \
||(g_CONTROL_BRIDGE_DEVICE_c == RSI_ENABLE) || (g_ON_OFF_SENSOR_DEVICE_c == RSI_ENABLE))
#define g_ON_OFF_CLIENT_CLUSTER_ENABLE_d                                1
#define g_ZLL_ON_OFF_CLIENT_CLUSTER_ENABLE_d							1
#endif /* End of On Off Cluster */


#if(( g_COLOR_CONTROLLER_DEVICE_c == RSI_ENABLE) || (g_COLOR_SCENE_CONTROLLER_DEVICE_c == RSI_ENABLE)        \
||  (g_CONTROL_BRIDGE_DEVICE_c == RSI_ENABLE) || (g_ON_OFF_SENSOR_DEVICE_c == RSI_ENABLE))

#define g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d                      	1
#define g_ZLL_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d						1
#endif /* Color Control */

/* ZLL Commissioning Cluser as Server and Client is common to all controller
 device type*/
#define ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d						1
#define ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d						1

#endif /* g_ZLL_LIGHT_CONTROLLER_c*/

	/* **************** End of Controller Device - Enabling the Clusters*******/


/*********************Lighting Device - Enabling the Clusters**************/
#ifdef g_ZLL_LIGHT_DEVICE_c

#if((g_ON_OFF_LIGHT_DEVICE_c== RSI_ENABLE) || (g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE) \
||(g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE )||(g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)\
	||(g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)||(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)\
		||(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c ==RSI_ENABLE))
#define g_IDENTIFY_SERVER_CLUSTER_ENABLE_d                           	1
#define g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d                          1
#endif  /* eND of identify cluster*/

#if((g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE )||(g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)\
	||(g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)||(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)\
		||(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c ==RSI_ENABLE))
#define g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d                         1
#define g_ZLL_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d                     1
#endif /* end of level control cluster */

#if((g_ON_OFF_LIGHT_DEVICE_c== RSI_ENABLE) || (g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE) \
||(g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE )||(g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)\
	||(g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)||(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)\
		||(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c ==RSI_ENABLE))

#define g_GROUPS_SERVER_CLUSTER_ENABLE_d                                1
#endif  /* end of groups*/

#if((g_ON_OFF_LIGHT_DEVICE_c== RSI_ENABLE) || (g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE) \
||(g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE )||(g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)\
	||(g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)||(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)\
		||(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c ==RSI_ENABLE))

#define g_SCENES_SERVER_CLUSTER_ENABLE_d                                1
#define g_ZLL_SCENES_SERVER_CLUSTER_ENABLE_d							1
#endif /* end of scenes*/

#if((g_ON_OFF_LIGHT_DEVICE_c== RSI_ENABLE) || (g_ON_OFF_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE) \
||(g_DIMMABLE_LIGHT_DEVICE_c == RSI_ENABLE )||(g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c == RSI_ENABLE)\
	||(g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)||(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)\
		||(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c ==RSI_ENABLE))

#define g_ON_OFF_SERVER_CLUSTER_ENABLE_d                                1
#define g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d							1
#endif /* end of On Off*/

#if( (g_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)||(g_EXTENDED_COLOR_LIGHT_DEVICE_c == RSI_ENABLE)\
		||(g_COLOR_TEMPERATURE_LIGHT_DEVICE_c ==RSI_ENABLE))

#define g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d          				1
#define g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d						1

#endif  /* end of color control*/

#endif /* g_ZLL_LIGHT_DEVICE_c*/



/***************** End of Lighting Device - Enabling the Clusters*******/

/* Enabling Attributes*/

#if ( g_BASIC_CLUSTER_ENABLE_d == 1 ) && (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)

#define g_APPLICATION_VERSION_ATTRIBUTE_ENABLE_d                        1
#define g_STACK_VERSION_ATTRIBUTE_ENABLE_d                              1
#define g_HW_VERSION_ATTRIBUTE_ENABLE_d                                 1
#define g_MODEL_IDENTIFIER_ATTRIBUTE_ENABLE_d                           1
#define g_DATE_CODE_ATTRIBUTE_ENABLE_d                                  1
#define g_LOCATION_DESCRIPTION_ATTRIBUTE_ENABLE_d                       0
#define g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d                       0
#define g_DEVICE_ENABLED_ATTRIBUTE_ENABLE_d                             0
#define g_ALARM_MASK_ATTRIBUTE_ENABLE_d                                 0
#define g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d                       0
#define g_MANUFACTURE_NAME_ATTRIBUTE_ENABLE_d  							1
#define g_SWBUILDID_ATTRIBUTE_ENABLE_d									1
#endif /*( g_BASIC_CLUSTER_ENABLE_d == 1 ) && (g_BASIC_SERVER_CLUSTER_ENABLE_d == 1)*/


/* IdentifyTime Attribute in Identify Cluster is a mandatory attribute and is
common even for ZLL*/

/* g_GROUP_ATTRIBUTE_NAME_SUPPORT_ATTRIBUTE_c is supported by default */



#if(g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
#define g_LEVEL_REMAINING_TIME_ATTRIBUTE_ENABLE_d                       1
#define g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ENABLE_d                     1
#define g_ON_LEVEL_ATTRIBUTE_ENABLE_d                                   1
#endif


#if(g_SCENES_CLUSTER_ENABLE_d==1) &&(g_SCENES_SERVER_CLUSTER_ENABLE_d==1)
#define g_LAST_CONFIGURED_BY_ATTRIBUTE_ENABLE_d							0
#define g_SCENE_MGMT_INFO_LAST_CONFIGURED_BY_ATTRIBUTE_ENABLE_d         0
#define g_TRANSITION_TIME_IN_MSEC_ATTRIBUTE_ENABLE_d					1
#endif/*(g_SCENES_CLUSTER_ENABLE_d==1) &&(g_SCENES_SERVER_CLUSTER_ENABLE_d==1)*/


#if(g_ON_OFF_SERVER_CLUSTER_ENABLE_d ==1)&&(g_ON_OFF_CLUSTER_ENABLE_d ==1)
#define g_GLOBAL_SCENE_CONTROL_ATTRIBUTE_ENABLE_d						1
#define g_ON_TIME_ATTRIBUTE_ENABLE_d									1
#define g_OFF_WAIT_TIME_ATTRIBUTE_ENABLE_d								1
#endif /*(g_ON_OFF_SERVER_CLUSTER_ENABLE_d ==1)&&
	(g_ON_OFF_CLUSTER_ENABLE_d ==1) */


#if(g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1)
#define g_CURRENT_HUE_ATTRIBUTE_ENABLE_d								1
#define g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d                         1
#define g_REMAINING_TIME_ATTRIBUTE_ENABLE_d                             1
/* CurrentX currentY it is by default enabled*/


#define g_DRIFT_COMPENSATION_ATTRIBUTE_ENABLE_d                         1
#define g_COMPENSATION_TEXT_ATTRIBUTE_ENABLE_d                          1
#define g_COLOR_TEMPERATURE_ATTRIBUTE_ENABLE_d                          1
#define g_COLOR_MODE_ATTRIBUTE_ENABLE_d                                 1


#define g_NUMBER_OF_PRIMARIES_ATTRIBUTE_ENABLE_d                        1
#define g_PRIMARY_1_X_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_1_Y_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_1_INTENSITY_ATTRIBUTE_ENABLE_d                        1
#define g_PRIMARY_2_X_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_2_Y_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_2_INTENSITY_ATTRIBUTE_ENABLE_d                        1
#define g_PRIMARY_3_X_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_3_Y_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_3_INTENSITY_ATTRIBUTE_ENABLE_d                        1
#define g_PRIMARY_4_X_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_4_Y_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_4_INTENSITY_ATTRIBUTE_ENABLE_d                        1
#define g_PRIMARY_5_X_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_5_Y_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_5_INTENSITY_ATTRIBUTE_ENABLE_d                        1
#define g_PRIMARY_6_X_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_6_Y_ATTRIBUTE_ENABLE_d                                1
#define g_PRIMARY_6_INTENSITY_ATTRIBUTE_ENABLE_d                        1
#define g_WHITE_POINT_X_ATTRIBUTE_ENABLE_d                              0
#define g_WHITE_POINT_Y_ATTRIBUTE_ENABLE_d                              0
#define g_COLOR_POINT_RX_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_RY_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_R_INTENSITY_ATTRIBUTE_ENABLE_d                    0
#define g_COLOR_POINT_GX_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_GY_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_G_INTENSITY_ATTRIBUTE_ENABLE_d                    0
#define g_COLOR_POINT_BX_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_BY_ATTRIBUTE_ENABLE_d                             0
#define g_COLOR_POINT_B_INTENSITY_ATTRIBUTE_ENABLE_d                    0
#define g_ENHANCED_CURRENT_HUE_ATTRIBUTE_ENABLE_d						1
#define	g_ENHANCED_COLOR_MODE_ATTRIBUTE_ENABLE_d						1
#define	g_COLOR_LOOP_ACTIVE_ATTRIBUTE_ENABLE_d							1
#define g_COLOR_LOOP_DIRECTION_ATTRIBUTE_ENABLE_d						1
#define g_COLOR_LOOP_TIME_ATTRIBUTE_ENABLE_d							1
#define g_COLOR_LOOP_START_ENCHANCED_ATTRIBUTE_ENABLE_d					1
#define g_COLOR_LOOP_STORED_ENHANCED_HUE_ENABLE_d						1
#define g_COLOR_CAPABILITIES_ATTRIBUTE_ENABLE_d							1
#define g_COLOR_TEMP_PHYSICAL_MIN_ATTRIBUTE_ENABLE_d					1
#define g_COLOR_TEMP_PHYSICAL_MAX_ATTRIBUTE_ENABLE_d					1
#endif

#define g_CLI_SUPPORT_d                                                 	0
#define g_SERVICE_DISCOVERY_ENABLE_d                                        0


/******************************************************************************
* End Of File
******************************************************************************/
#endif 								   /* _ZCL_FUNCTIONALITY_H_ */
