#ifndef _ZCL_LIGHTING_COLOR_CONTROL_CLUSTER_H_
#define _ZCL_LIGHTING_COLOR_CONTROL_CLUSTER_H_


#if(g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1)
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_COLOR_CONTROL_CLUSTER_c                                   0x0300

#define g_CURRENT_HUE_ATTRIBUTE_c                                   0x0000
#define g_CURRENT_SATURATION_ATTRIBUTE_c                            0x0001
#define g_REMAINING_TIME_ATTRIBUTE_c                                0x0002
#define g_COLOR_MODE_ATTRIBUTE_c                                    0x0008
#define g_CURRENT_X_ATTRIBUTE_c                                     0x0003
#define g_CURRENT_Y_ATTRIBUTE_c                                     0x0004
#define g_DRIFT_COMPENSATION_ATTRIBUTE_c                            0x0005
#define g_COMPENSATION_TEXT_ATTRIBUTE_c                             0x0006
#define g_COLOR_TEMPERATURE_ATTRIBUTE_c                             0x0007
#define g_NUMBER_OF PRIMARIES_ATTRIBUTE_c                           0x0010
#define g_PRIMARY_1X_ATTRIBUTE_c                                    0x0011
#define g_PRIMARY_1Y_ATTRIBUTE_c                                    0x0012
#define g_PRIMARY_1_INTENSITY_ATTRIBUTE_c                           0x0013
#define g_PRIMARY_2X_ATTRIBUTE_c                                    0x0015
#define g_PRIMARY_2Y_ATTRIBUTE_c                                    0x0016
#define g_PRIMARY_2_INTENSITY_ATTRIBUTE_c                           0x0017
#define g_PRIMARY_3X_ATTRIBUTE_c                                    0x0019
#define g_PRIMARY_3Y_ATTRIBUTE_c                                    0x001a
#define g_PRIMARY_3_INTENSITY_ATTRIBUTE_c                           0x001b
#define g_PRIMARY_4X_ATTRIBUTE_c                                    0x0020
#define g_PRIMARY_4Y_ATTRIBUTE_c                                    0x0021
#define g_PRIMARY_4_INTENSITY_ATTRIBUTE_c                           0x0022
#define g_PRIMARY_5X_ATTRIBUTE_c                                    0x0024
#define g_PRIMARY_5Y_ATTRIBUTE_c                                    0x0025
#define g_PRIMARY_5_INTENSITY_ATTRIBUTE_c                           0x0026
#define g_PRIMARY_6X_ATTRIBUTE_c                                    0x0028
#define g_PRIMARY_6Y_ATTRIBUTE_c                                    0x0029
#define g_PRIMARY_6_INTENSITY_ATTRIBUTE_c                           0x002a
#define g_WHITE_POINT_X_ATTRIBUTE_c                                 0x0030
#define g_WHITE_POINT_Y_ATTRIBUTE_c                                 0x0031
#define g_COLOR_POINT_RX_ATTRIBUTE_c                                0x0032
#define g_COLOR_POINT_RY_ATTRIBUTE_c                                0x0033
#define g_COLOR_POINT_R_INTENSITY_ATTRIBUTE_c                       0x0034
#define g_COLOR_POINT_GX_ATTRIBUTE_c                                0x0036
#define g_COLOR_POINT_GY_ATTRIBUTE_c                                0x0037
#define g_COLOR_POINT_G_INTENSITY_ATTRIBUTE_c                       0x0038
#define g_COLOR_POINT_BX_ATTRIBUTE_c                                0x003a
#define g_COLOR_POINT_BY_ATTRIBUTE_c                                0x003b
#define g_COLOR_POINT_B_INTENSITY_ATTRIBUTE_c                       0x003c
#if (g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c                          0x4000
#define g_ENHANCED_COLOR_MODE_ATTRIBUTE_c                           0x4001
#define g_COLOR_LOOP_ACTIVE_ATTRIBUTE_c                             0x4002
#define g_COLOR_LOOP_DIRECTION_ATTRIBUTE_c                          0x4003
#define g_COLOR_LOOP_TIME_ATTRIBUTE_c                               0x4004
#define g_COLOR_LOOP_START_ENHANCED_HUE_c                           0x4005
#define g_COLOR_LOOP_STORE_ENHANCED_HUE_c                          0x4006
#define g_CLOLR_CAPABILITIES_c                                      0x400a
#define g_COLOR_TEMP_PHYSICAL_MIN_c                                 0x400b
#define g_COLOR_TEMP_PHYSICAL_MAX_c                                 0x400c
#endif

enum Hue_Light_Control_Cluster
{
   g_Move_to_Hue_Command_c,
   g_Move_Hue_Command_c,
   g_Step_Hue_Command_c,
   g_Move_To_Saturation_Command_c,
   g_Move_Saturation_Command_c,
   g_Step_Saturation_Command_c,
   g_Move_To_Hue_And_Saturation_Command_c,
   g_Move_To_Color_Command_c,
   g_Move_Color_Command_c,
   g_Step_Color_Command_c,
   g_Move_To_Color_Temperature_Command_c,
#if (g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
   g_Enhanced_Move_To_Hue_Command_c = 0x40,
   g_Enhanced_Move_Hue_Command_c,
   g_Enhanced_Step_Hue_Command_c,
   g_Enhanced_Move_To_Hue_And_Saturation_Command_c,
   g_Color_Loop_Set_Command_c,
   g_Stop_Move_Step_Command_c = 0x47,
   g_Move_Color_Temperature_Command_c = 0x4b,
   g_Step_Color_Temperature_Command_c
#endif
};

/*---------------------------------------------------------------------------*/

/* enumerations for move hue modes */
enum Move_Hue_Modes
{
   g_Stop_c,
   g_Up_c,
   g_Down_c = 0x03
};

/*---------------------------------------------------------------------------*/

/* enumerations for move hue modes */
enum Color_Loop_Set_Direction
{
   g_Decrement_Color_c,
   g_Increment_Color_c

};


/*---------------------------------------------------------------------------*/

/* enumerations for move to hue modes */
enum Move_To_Hue_Modes
{
   g_Shortest_Distance_c,
   g_Longest_Distance_c,
   g_Move_To_Hue_Up_c,
   g_Move_To_Hue_Down_c
};

/*---------------------------------------------------------------------------*/

/* enumerations for move to hue modes */
enum Move_Step_Saturation_Modes
{
   g_Move_To_Saturation_Up_c = 0x01,
   g_Move_To_Saturation_Down_c = 0x03
};

/*---------------------------------------------------------------------------*/

/*enumerations for lighting events*/
enum Lighting_Cluster_Events
{
   g_Move_to_Hue_Command_Event_c = 0x20,
   g_Move_hue_Command_Event_c,
   g_Step_Hue_Command_Event_c,
   g_Move_To_Saturation_Command_Event_c,
   g_Move_Saturation_Command_Event_c,
   g_Step_Saturation_Command_Event_c,
   g_Move_To_Hue_And_Saturation_Command_Event_c,
   g_Move_To_Color_Command_Event_c,
   g_Move_Color_Command_Event_c,
   g_Step_Color_Command_Event_c,
   g_Move_To_Color_Temperature_Command_Event_c,
#if (g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
   g_Enhanced_Move_To_Hue_Command_Event_c,
   g_Enhanced_Move_Hue_Command_Event_c,
   g_Enhanced_Step_Hue_Command_Event_c,
   g_Enhanced_Move_To_Hue_And_Saturation_Command_Event_c,
   g_Color_Loop_Set_Event_c,
   g_Stop_Move_Step_Event_c,
   g_Move_Color_Temperature_Event_c,
   g_Step_Color_Temperature_Event_c
#endif

};

/*---------------------------------------------------------------------------*/
/*enumeration for color loop mode */
enum Color_Loop_Move_Mode
{
    g_Move_Hue_Down_c ,
	g_Move_Hue_Up_c
};

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct color_control_attribute_data_tag
{
#if ( g_CURRENT_HUE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t current_hue;
#endif       /*g_CURRENT_HUE_ATTRIBUTE_ENABLE_d*/
#if ( g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t current_saturation;
#endif       /*g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d*/
#if ( g_REMAINING_TIME_ATTRIBUTE_ENABLE_d == 1)
   uint8_t a_remaining_time[2];
#endif         /*g_REMAINING_TIME_ATTRIBUTE_ENABLE_d*/
   uint8_t a_current_x[2];
   uint8_t a_current_y[2];
#if ( g_DRIFT_COMPENSATION_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t drift_compensation;
#endif       /*g_DRIFT_COMPENSATION_ATTRIBUTE_ENABLE_d*/
#if ( g_COMPENSATION_TEXT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t compensation_text[16];
#endif        /*g_COMPENSATION_TEXT_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_temperature[2];
#endif        /*g_COLOR_TEMPERATURE_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_MODE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_mode;
#endif         /*g_COLOR_MODE_ATTRIBUTE_ENABLE_d*/
#if ( g_NUMBER_OF_PRIMARIES_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t no_of_primaries;
#endif        /*g_NUMBER_OF_PRIMARIES_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_1X_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_1x;
#endif        /*g_PRIMARY_1X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_1Y_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_1y;
#endif       /*g_PRIMARY_1Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_1_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_1_intensity;
#endif        /*g_PRIMARY_1_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_2X_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_2x;
#endif       /*g_PRIMARY_2X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_2Y_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_2y;
#endif       /*g_PRIMARY_2Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_2_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_2_intensity;
#endif       /*g_PRIMARY_2_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_3X_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_3x;
#endif       /*g_PRIMARY_3X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_3Y_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_3y;
#endif       /*g_PRIMARY_3Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_3_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_3_intensity;
#endif       /*g_PRIMARY_3_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_4X_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_4x;
#endif       /*g_PRIMARY_4X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_4Y_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_4y;
#endif       /*g_PRIMARY_4Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_4_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_4_intensity;
#endif        /*g_PRIMARY_4_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_5X_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_5x;
#endif       /*g_PRIMARY_5X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_5Y_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_5y;
#endif       /*g_PRIMARY_5Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_5_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_5_intensity;
#endif       /*g_PRIMARY_5_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_6X_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_6x;
#endif       /*g_PRIMARY_6X_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_6Y_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_6y;
#endif       /*g_PRIMARY_6Y_ATTRIBUTE_ENABLE_d*/
#if ( g_PRIMARY_6_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t primary_6_intensity;
#endif       /*g_PRIMARY_6_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_WHITE_POINT_X_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t white_point_x;
#endif       /*g_WHITE_POINT_X_ATTRIBUTE_ENABLE_d*/
#if ( g_WHITE_POINT_Y_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t white_point_y;
#endif       /*g_WHITE_POINT_Y_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_RX_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_point_rx;
#endif       /*g_COLOR_POINT_RX_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_RY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_point_ry;
#endif       /*g_COLOR_POINT_RY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_R_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_point_r_intensity;
#endif       /*g_COLOR_POINT_R_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_GX_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_point_gx;
#endif       /*g_COLOR_POINT_GX_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_GY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_point_gy;
#endif       /*g_COLOR_POINT_GY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_G_INTENSITY_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_point_g_intensity;
#endif       /*g_COLOR_POINT_G_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_BX_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_point_bx;
#endif       /*g_COLOR_POINT_BX_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_BY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_point_by;
#endif       /*g_COLOR_POINT_BY_ATTRIBUTE_ENABLE_d*/
#if ( g_COLOR_POINT_B_INTENSITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t color_point_b_intensity;
#endif       /*g_COLOR_POINT_B_INTENSITY_ATTRIBUTE_ENABLE_d*/
#if (g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
#if (g_ENHANCED_CURRENT_HUE_ATTRIBUTE_ENABLE_d == 1)
   uint8_t enhanced_current_hue[2];
#endif          /* g_ENHANCED_CURRENT_HUE_ATTRIBUTE_ENABLE_d */
#if (g_ENHANCED_COLOR_MODE_ATTRIBUTE_ENABLE_d == 1)
   uint8_t enhanced_color_mode;
#endif         /*g_ENHANCED_COLOR_MODE_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_LOOP_ACTIVE_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_loop_active;
#endif        /*g_COLOR_LOOP_ACTIVE_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_LOOP_DIRECTION_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_loop_direction;
#endif        /*g_COLOR_LOOP_DIRECTION_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_LOOP_TIME_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_loop_time[2];
#endif        /*g_COLOR_LOOP_TIME_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_LOOP_START_ENHANCED_HUE_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_loop_start_enhanced_hue[2];
#endif       /*g_COLOR_LOOP_START_ENHANCED_HUE_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_LOOP_STORED_ENHANCED_HUE_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_loop_stored_enhanced_hue[2];
#endif       /*g_COLOR_LOOP_STORED_ENHANCED_HUE_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_CAPABILITIES_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_capabilities[2];
#endif      /*g_COLOR_CAPABILITIES_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_TEMP_PHYSICAL_MIN_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_temp_physical_min[2];
#endif      /*g_COLOR_TEMP_PHYSICAL_MIN_ATTRIBUTE_ENABLE_d*/
#if (g_COLOR_TEMP_PHYSICAL_MAX_ATTRIBUTE_ENABLE_d == 1)
   uint8_t color_temp_physical_max[2];
#endif      /*g_COLOR_TEMP_PHYSICAL_MAX_ATTRIBUTE_ENABLE_d*/
#endif /*g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
}__attribute__((__packed__)) color_control_attribute_data_t;
/*---------------------------------------------------------------------------*/

/* Structures for lighting Commands */

/*
Purpose  : Structure for Move to Hue Command
Field    : hue - holds the hue value
Field    : direction - gives the direction in which the hue has to change
Field    : transition_time - gives the time within which the hue has to be
                             updated to the new value
*/
typedef struct Move_To_Hue_Command
{
   uint8_t hue;
   uint8_t direction;
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Move_To_Hue_Command;

/*---------------------------------------------------------------------------*/


/*
Purpose  : Structure for Move Hue Command
Field    : move_mode - gives the mode in which the hue has to change
Field    : rate - gives the value by which hue has to change
*/

typedef struct Move_Hue_Command
{
   uint8_t move_mode;
   uint8_t rate;
}__attribute__((__packed__))Move_Hue_Command;



/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Step Hue Command
Field    : step_mode - gives information if the hue has to step up or down
Field    : transition_time - gives the time by which hue has to change
*/
typedef struct Step_Hue_Command
{
   uint8_t step_mode;
   uint8_t step_size;
   uint8_t transition_time;
}__attribute__((__packed__))Step_Hue_Command;




/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Move to Saturation Command
Field    : saturation - holds the saturation value
Field    : direction - gives the direction in which the saturation has to
                       change
Field    : transition_time - gives the time within which the saturation has
                       to be updated to the new value
*/
typedef struct Move_To_Saturation_Command
{
   uint8_t saturation;
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Move_To_Saturation_Command;


/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Move Saturation Command
Field    : move_mode - gives the mode in which the saturation has to change
Field    : rate - gives the value by which saturation has to change
*/
typedef struct Move_Saturation_Command
{
   uint8_t move_mode;
   uint8_t rate;
}__attribute__((__packed__))Move_Saturation_Command;


/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Step Saturation Command
Field    : step_mode - gives information if the saturation has to step up or
                       down
Field    : transition_time - gives the time by which saturation has to change
*/
typedef struct Step_Saturation_Command
{
   uint8_t step_mode;
   uint8_t step_size;
   uint8_t transition_time;
}__attribute__((__packed__))Step_Saturation_Command;


/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Move to Hue and saturation Command
Field    : hue - holds the hue value
Field    : saturation - holds the saturation value
Field    : transition_time - gives the time within which the hue and saturation
                  have to be updated to the new value
*/
typedef struct Move_To_Hue_And_Saturation_Command
{
   uint8_t hue;
   uint8_t saturation;
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Move_To_Hue_And_Saturation_Command;





/*---------------------------------------------------------------------------*/
/*
Purpose  : Structure for Move to Color Command
Field    : color_x - holds the color x value
Field    : color_y - holds the color y value
Field    : transition_time - gives the time within which the color x and color y
                  have to be updated to the new value
*/

typedef struct Move_To_Color_Command
{
   uint8_t color_x[2];
   uint8_t color_y[2];
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Move_To_Color_Command;


/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Move Color Command
Field    : rate_x - holds the rate x value that specifies the rate of movement
                 in steps per second
Field    : rate_y - holds the rate y value that specifies the rate of movement
                 in steps per second
*/

typedef struct Move_Color_Command
{
   uint8_t rate_x[2];
   uint8_t rate_y[2];
}__attribute__((__packed__))Move_Color_Command;


/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Step Color Command
Field    : step_x - holds the rate x value that specifies the change to be added
                  to the device current x attribute
Field    : step_y - holds the rate y value that specifies the change to be added
                  to the device current y attribute
Field    : transition_time - gives the time within which the step x and step y
                  have to be updated to the new value
*/

typedef struct Step_Color_Command
{
   uint8_t step_x[2];
   uint8_t step_y[2];
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Step_Color_Command;


/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Move To Color Temperature Command
Field    : step_x - holds the rate x value that specifies the change to be added
                  to the device current x attribute
Field    : step_y - holds the rate y value that specifies the change to be added
                  to the device current y attribute
Field    : transition_time - gives the time within which the step x and step y
                  have to be updated to the new value
*/

typedef struct Move_To_Color_Temperature_Command
{
   uint8_t color_temperature[2];
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Move_To_Color_Temperature_Command;


/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Enhanced Move to Hue Command
Field    : hue - holds the 16 bit hue value
Field    : direction - gives the direction in which the hue has to change
Field    : transition_time - gives the time within which the hue has to be
                             updated to the new value
*/
#if (g_ZLL_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
typedef struct Enhanced_Move_To_Hue_Command
{
   uint8_t Enhanced_hue[2];
   uint8_t direction;
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Enhanced_Move_To_Hue_Command;

/*---------------------------------------------------------------------------*/
/*
Purpose  : Structure for Enhanced Move Hue Command
Field    : move_mode - gives the mode in which the hue has to change
Field    : rate - gives the 16 bit rate value by which hue has to change
*/

typedef struct Enhanced_Move_Hue_Command
{
   uint8_t move_mode;
   uint8_t rate[2];
}__attribute__((__packed__))Enhanced_Move_Hue_Command;

/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Step Hue Command
Field    : step_mode - gives information if the hue has to step up or down
Field    : transition_time - gives the time by which hue has to change
*/
typedef struct Enhanced_Step_Hue_Command
{
   uint8_t step_mode;
   uint8_t step_size[2];
   uint8_t transition_time[2];
}__attribute__((__packed__))Enhanced_Step_Hue_Command;

/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Enhanced Move to Hue and saturation Command
Field    : hue - holds the hue value
Field    : saturation - holds the saturation value
Field    : transition_time - gives the time within which the hue and saturation
                  have to be updated to the new value
*/
typedef struct Enhanced_Move_To_Hue_And_Saturation_Command
{
   uint8_t Enhanced_hue[2];
   uint8_t saturation;
   uint8_t a_transition_time[2];
}__attribute__((__packed__))Enhanced_Move_To_Hue_And_Saturation_Command;

/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Color Loop Set Command
Field    : step_x - holds the rate x value that specifies the change to be added
                  to the device current x attribute
Field    : step_y - holds the rate y value that specifies the change to be added
                  to the device current y attribute
Field    : transition_time - gives the time within which the step x and step y
                  have to be updated to the new value
*/

typedef struct Color_Loop_Set
{
   uint8_t update_flags;
   uint8_t action;
   uint8_t direction;
   uint8_t a_transition_time[2];
   uint8_t start_hue[2];
}__attribute__((__packed__))Color_Loop_Set;
/*---------------------------------------------------------------------------*/
/*
Purpose  : Structure for Move Color Temperature Command
Field    : move_mode - gives the mode in which the Color Temperature has to change
Field    : rate - gives the 16 bit rate value by which Color Temperature has to change
*/

typedef struct Move_Color_Temperature
{
   uint8_t move_mode;
   uint8_t rate[2];
   uint8_t color_temp_min[2];
   uint8_t color_temp_max[2];
}__attribute__((__packed__))Move_Color_Temperature;

/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Step Color Temperature Command
Field    : step_mode - gives information if the Color Temperature has to step up or down
Field    : transition_time - gives the time by which Color Temperature has to change
*/
typedef struct Step_Color_Temperature
{
   uint8_t step_mode;
   uint8_t step_size[2];
   uint8_t transition_time[2];
   uint8_t color_temp_min[2];
   uint8_t color_temp_max[2];
}__attribute__((__packed__))Step_Color_Temperature;

/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Lighting Events
Field    : Event Id - gives the Identifier for a particular event
Field    : Event Length - gives the length of the event
Field    : Event_data - gives the event data depending on the command received.
                   Each command will be sent with an event data as defined in
                   the structures
*/
#endif

typedef struct Lighting_Event
{
   union event_data
   {
      Move_To_Hue_Command move_to_hue_command_data;
      Move_Hue_Command  move_hue_command_data;
      Step_Hue_Command  step_hue_command_data;
      Move_To_Saturation_Command move_to_saturation_command_data;
      Move_Saturation_Command move_saturation_command_data;
      Step_Saturation_Command step_saturation_command_data;
      Move_To_Hue_And_Saturation_Command  move_to_hue_and_saturation_command_data;
      Move_To_Color_Command move_to_color_command_data;
      Move_Color_Command move_color_command_data;
      Step_Color_Command step_color_command_data;
      Move_To_Color_Temperature_Command move_to_color_temperature_command_data;
#if (g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
      Enhanced_Move_To_Hue_Command enhanced_move_to_hue_command_data;
      Enhanced_Move_Hue_Command enhanced_move_hue_command_data;
      Enhanced_Step_Hue_Command enhanced_step_hue_command;
      Enhanced_Move_To_Hue_And_Saturation_Command enhanced_move_to_hue_and_saturation_command;
      Color_Loop_Set color_loop_set;
      Move_Color_Temperature move_color_temperature;
      Step_Color_Temperature step_color_temperature;
#endif
   }event_data;
}__attribute__((__packed__))Lighting_Event;

/*---------------------------------------------------------------------------*/

/*
Purpose  : Structure for Lighting Commands
Field    : command_type - This gives information if the command is Generic or
                  cluster specific
Field    : manufacturer_specific - This byte gives information if manufacturer
                  specific code is present in the payload
Field    : manufacturer_code - These 2 bytes depends on the manufavturer
Field    : disable_default_response - This byte gives the information if
                  default response is enabled or disabled
Field    : command_identifier -  This gives the command id for the specific
                  lighting command.
Field    : Lighting_Commands - Contains an union of all lighting commands
*/

typedef struct Lighting_Command_Request
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union Lighting_Commands
   {
      Move_To_Hue_Command move_to_hue_command_data;
      Move_Hue_Command move_hue_command_data;
      Step_Hue_Command step_hue_command_data;
      Move_To_Saturation_Command move_to_saturation_command_data;
      Move_Saturation_Command move_saturation_command_data;
      Step_Saturation_Command step_saturation_command_data;
      Move_To_Hue_And_Saturation_Command move_to_hue_and_saturation_command_data;
      Move_To_Color_Command move_to_color_command_data;
      Move_Color_Command move_color_command_data;
      Step_Color_Command step_color_command_data;
      Move_To_Color_Temperature_Command move_to_color_temperature_command_data;
#if (g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
      Enhanced_Move_To_Hue_Command enhanced_move_to_hue_command_data;
      Enhanced_Move_Hue_Command enhanced_move_hue_command_data;
      Enhanced_Step_Hue_Command enhanced_step_hue_command;
      Enhanced_Move_To_Hue_And_Saturation_Command enhanced_move_to_hue_and_saturation_command;
      Color_Loop_Set color_loop_set;
      Move_Color_Temperature move_color_temperature;
      Step_Color_Temperature step_color_temperature;
#endif
   }Lighting_Commands;
}__attribute__((__packed__))Lighting_Command_Request;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Color_Control_Server_Cluster_Info;
#endif                              /* g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Color_Control_Client_Cluster_Info;
#endif                              /* g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_Color_Ctrl_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value :
*		uint8_t - Gives the status of the command operation
* Input Parameters:
*	p_cluster_data - pointer to information about the cluster and attributes
*
* Output Parameters:
*
* Scope of the function:
*  This function is called within ZCL_Handle_Cluster_Specific_Cmd() function,
*  when a Cluster Command is received in the Data indication.
*
* Purpose of the function:
*	This function handles the Color control cluster command received in the
*  Data indication
*
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Color_Ctrl_Cluster_Cmd (uint8_t endPointId,
                                uint8_t asduLength, uint8_t* pReceivedAsdu,
                                uint8_t* pResponseAsduStartLocation,
                                ZCL_ActionHandler_t *pActionHandler,
                                ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                uint8_t * pAttrsDataBaseLocation );

/*----------------------------------------------------------------------------
* Read_Write_Color_Control_Attributes
*------------------------------------------------------------------------------
*
* Return Value :
*			None	
* Input Parameters:
*	read_or_write - This parameter indicates whether attribute has to be read or
*  write.
*	attrib_value - The value of attribute that has to be written.
*	attrib_id - The cluster id whose attribute has to be modified.
*	
* Output Parameters:
*	   attrib_value - The attribute value that is read or to be written.
*
* Scope of the function:
*    Exposed	
*
* Purpose of the function:
*    This function reads or writes the color control attributes
*	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/	

void Read_Write_Color_Control_Attributes
(
   uint8_t read_or_write,
   uint8_t *attrib_value,
   uint16_t attrib_id
);
#endif                                 /*g_COLOR_CONTROL_CLUSTER_ENABLE_d*/
#endif                                 /*_ZCL_LIGHTING_COLOR_CONTROL_CLUSTER_H_*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
