/******************************************************************************
* ZCL_Windows_Covering_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Windows Covering attribute and cmmnd information
******************************************************************************/
#ifndef _ZCL_WINDOWS_COVERING_CLUSTER_H_
#define _ZCL_WINDOWS_COVERING_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

#if ( g_WINDOW_COVERING_CLUSTER_ENABLE_d == 1 )

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/*Attributes and commands for putting a device into
Identification mode (e.g. flashing a light)*/
#define g_WINDOW_COVERING_CLUSTER_c                                   0x0102

#define g_WINDOW_COVERING_TYPE_ATTRIBUTE_c                            0x0000
#define g_PHYSICAL_CLOSED_LIMIT_LIFT_ATTRIBUTE_c                      0x0001
#define g_PHYSICAL_CLOSED_LIMIT_TILT_ATTRIBUTE_c                      0x0002
#define g_CURRENT_POSITION_LIFT_ATTRIBUTE_c                           0x0003
#define g_CURRENT_POSITION_TILT_ATTRIBUTE_c                           0x0004
#define g_NUMBER_OF_ACTUATIONS_LIFT_ATTRIBUTE_c                       0x0005
#define g_NUMBER_OF_ACTUATIONS_TILT_ATTRIBUTE_c                       0x0006
#define g_CONFIG_STATUS_ATTRIBUTE_c                                   0x0007
#define g_CURRENT_POSITION_LIFT_PERCENTAGE_ATTRIBUTE_c                0x0008
#define g_CURRENT_POSITION_TILT_PERCENTAGE_ATTRIBUTE_c                0x0009
#define g_INSTALLED_OPEN_LIMIT_LIFT_ATTRIBUTE_c                       0x0010
#define g_INSTALLED_CLOSED_LIMIT_LIFT_ATTRIBUTE_c                     0x0011
#define g_INSTALLED_OPEN_LIMIT_TILT_ATTRIBUTE_c                       0x0012
#define g_INSTALLED_CLOSED_LIMIT_TILT_ATTRIBUTE_c                     0x0013
#define g_VELOCITY_LIFT_ATTRIBUTE_c                                   0x0014
#define g_ACCELERATION_TIME_LIFT_ATTRIBUTE_c                          0x0015
#define g_DECELERATION_TIME_LIFT_ATTRIBUTE_c                          0x0016
#define g_MODE_ATTRIBUTE_c                                            0x0017
#define g_INTERMEDIATE_SET_POINTS_LIFT_ATTRIBUTE_c                    0x0018
#define g_INTERMEDIATE_SET_POINTS_TILT_ATTRIBUTE_c                    0x0019



enum Windows_Covering_Cluster
{
   g_Up_Open_Command_c,
   g_Down_Close_Command_c,
   g_Windows_Stop_Command_c,
   g_Go_To_Lift_Set_Point_Command_c,
   g_Go_To_Lift_Value_Command_c,
   g_Go_To_Lift_Percentage_Command_c,
   g_Go_To_Tilt_Set_Point_Command_c,
   g_Go_To_Tilt_Value_Command_c,
   g_Go_To_Tilt_Percentage_Command_c,
   g_Program_Set_Point_Command_c
};
/*---------------------------------------------------------------------------*/

/*enumerations for window covering events*/
enum Windows_Covering_Cluster_Events
{
   g_Up_Open_Command_Event_c = 0x90,
   g_Down_Close_Command_Event_c,
   g_Stop_Command_Event_c,
   g_Go_To_Lift_Set_Point_Command_Event_c,
   g_Go_To_Lift_Value_Command_Event_c,
   g_Go_To_Lift_Percentage_Command_Event_c,
   g_Go_To_Tilt_Set_Point_Command_Event_c,
   g_Go_To_Tilt_Value_Command_Event_c,
   g_Go_To_Tilt_Percentage_Command_Event_c,
   g_Program_Set_Point_Command_Version1_Event_c,
   g_Program_Set_Point_Command_Version2_Event_c
};


typedef struct Set_point_Version1
{
   uint8_t a_set_point_type;
    uint8_t a_set_point_index;
    uint16_t a_set_point_value;
}__attribute__((__packed__))Set_point_Version1;


typedef struct Set_point_Version2
{
    uint8_t a_set_point_index;
}__attribute__((__packed__))Set_point_Version2;



typedef struct Windows_Covering_Event
{
    uint8_t a_index_of_lift_set_point;
    uint8_t a_lift_value[2];
    uint8_t a_percentage_lift_value;
    uint8_t a_index_of_tilt_set_point;
    uint8_t a_tilt_value[2];
    uint8_t a_percentage_tilt_value;
    Set_point_Version1 set_point_version1[3];
}__attribute__((__packed__))Windows_Covering_Event;
/*---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_WINDOW_COVERING_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Windows_Covering_Server_Cluster_Info;
#endif /* g_WINDOW_COVERING_SERVER_CLUSTER_ENABLE_d*/
#if ( g_WINDOW_COVERING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Windows_Covering_Client_Cluster_Info;
#endif /* g_WINDOW_COVERING_CLIENT_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct Windows_Covering_Cluster_tag
{
   uint8_t window_covering_type;
#if ( g_PHYSICAL_CLOSED_LIMIT_LIFT_ATTRIBUTE_ENABLE_d == 1)
   uint8_t physical_closed_limit_lift[2];
#endif /* g_PHYSICAL_CLOSED_LIMIT_LIFT_ATTRIBUTE_ENABLE_d*/
#if ( g_PHYSICAL_CLOSED_LIMIT_TILT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t physical_closed_limit_tilt[2];
#endif /* g_PHYSICAL_CLOSED_LIMIT_TILT_ATTRIBUTE_ENABLE_d*/
#if ( g_CURRENT_POSITION_LIFT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t current_position_lift[2];
#endif /* g_CURRENT_POSITION_LIFT_ATTRIBUTE_ENABLE_d*/
#if ( g_CURRENT_POSITION_TILT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t current_position_tilt[2];
#endif /* g_CURRENT_POSITION_TILT_ATTRIBUTE_ENABLE_d*/
#if ( g_NUMBER_OF_ACTUATIONS_LIFT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t number_of_actuations_lift[2];
#endif /* g_NUMBER_OF_ACTUATIONS_LIFT_ATTRIBUTE_ENABLE_d*/
#if ( g_NUMBER_OF_ACTUATIONS_TILT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t number_of_actuations_tilt[2];
#endif /* g_NUMBER_OF_ACTUATIONS_TILT_ATTRIBUTE_ENABLE_d*/
   uint8_t config_status;
#if ( g_CURRENT_POSITION_LIFT_PERCENTAGE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t current_position_lift_percentage;
#endif /* g_CURRENT_POSITION_LIFT_PERCENTAGE_ATTRIBUTE_ENABLE_d*/
#if ( g_CURRENT_POSITION_TILT_PERCENTAGE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t current_position_tilt_percentage;
#endif /* g_CURRENT_POSITION_TILT_PERCENTAGE_ATTRIBUTE_ENABLE_d*/
   uint8_t installed_open_limit_lift[2];
   uint8_t installed_closed_limit_lift[2];
   uint8_t installed_open_limit_tilt[2];
   uint8_t installed_closed_limit_tilt[2];
#if ( g_VELOCITY_LIFT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t velocity_lift[2];
#endif /* g_VELOCITY_LIFT_ATTRIBUTE_ENABLE_d*/
#if ( g_ACCELERATION_TIME_LIFT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t acceleration_time_lift[2];
#endif /* g_ACCELERATION_TIME_LIFT_ATTRIBUTE_ENABLE_d*/
#if ( g_DECELERATION_TIME_LIFT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t deceleration_time_lift[2];
#endif /* g_DECELERATION_TIME_LIFT_ATTRIBUTE_ENABLE_d*/
   uint8_t mode;
#if ( g_INTERMEDIATE_SET_POINTS_LIFT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t intermediate_set_points_lift[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif /* g_INTERMEDIATE_SET_POINTS_LIFT_ATTRIBUTE_ENABLE_d*/
#if ( g_INTERMEDIATE_SET_POINTS_TILT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t intermediate_set_points_tilt[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif /* g_INTERMEDIATE_SET_POINTS_TILT_ATTRIBUTE_ENABLE_d*/


}__attribute__((__packed__))Windows_Covering_Cluster_t;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* ZCL_Handle_Window_Covering_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value :
*		unsigned char - returns success or error code
*
* Input Parameters:
*	p_cluster_data - pointer to cluster data
*	
* Output Parameters:
*	p_cluster_data - pointer to cluster data	
*
* Scope of the function:
*  This function is called within ZCL_Handle_Cluster_Specific_Cmd() function,
*  when a identify cluster command is received in the Data indication.
*
* Purpose of the function:
*	This function handles the identify cluster command received in the data
*  indication
*     	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Window_Covering_Cluster_Cmd
(
     uint8_t endPointId,
     uint8_t asduLength,
     uint8_t* pReceivedAsdu,
     uint8_t* pResponseAsduStartLocation,
     ZCL_ActionHandler_t *pActionHandler,
     ZCL_Cluster_Info_t const *pZCLClusterInfo,
     uint8_t * pAttrsDataBaseLocation
);
uint8_t ZCL_CallBack_Windows_Covering( ZCL_Event_t *pEvents );

#endif                                  /* g_WINDOW_COVERING_CLUSTER_ENABLE_d  */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_WINDOWS_COVERING_CLUSTER_H_*/
