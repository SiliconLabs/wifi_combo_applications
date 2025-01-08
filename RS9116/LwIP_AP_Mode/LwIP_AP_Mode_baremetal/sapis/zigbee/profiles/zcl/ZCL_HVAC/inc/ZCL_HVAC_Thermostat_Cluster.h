#ifndef _ZCL_HVAC_THERMOSTAT_CLUSTER_H_
#define _ZCL_HVAC_THERMOSTAT_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )

/* Interface for configuring and controlling the functionality of a thermostat */
#define g_THERMOSTAT_CLUSTER_c                                       0x0201

/* Attributes of the Thermostat Information Attribute Set */
#define g_LOCAL_TEMPERATURE_ATTRIBUTE_c                              0x0000
#define g_OUTDOOR_TEMPERATURE_ATTRIBUTE_c                            0x0001
#define g_OCCUPANCY_ATTRIBUTE_c                                      0x0002
#define g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c                   0x0003
#define g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c                   0x0004
#define g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_c                   0x0005
#define g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_c                   0x0006
#define g_PI_COOLING_DEMAND_ATTRIBUTE_c                              0x0007
#define g_PI_HEATING_DEMAND_ATTRIBUTE_c                              0x0008

#if(g_HA_1_2_d == 1)
#define g_HVAC_SYSTEM_TYPE_CONFIGURATION_ATTRIBUTE_c				 0x0009
#endif

/* Attributes of the Thermostat Settings Attribute Set */
#define g_LOCAL_TEMPERATURE_CALIBRATION_ATTRIBUTE_c                  0x0010
#define g_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c                     0x0011
#define g_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c                     0x0012
#define g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c                  0x0013
#define g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c                  0x0014
#define g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c                       0x0015
#define g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c                       0x0016
#define g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_c                       0x0017
#define g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_c                       0x0018
#define g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_c                        0x0019
#define g_REMOTE_SENSING_ATTRIBUTE_c                                 0x001A
#define g_CONTROL_SEQUENCE_OF_OPERATION_ATTRIBUTE_c                  0x001B
#define g_SYSTEM_MODE_ATTRIBUTE_c                                    0x001C
#define g_THERMOSTAT_ALARM_MASK_ATTRIBUTE_c                          0x001D

#if(g_HA_1_2_d == 1)
#define g_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_c				 		 0x001E
#endif

/* Thermostat Schedule & HVAC Relay Attribute Set */
#if(g_HA_1_2_d == 1)
/* Schedule Attribute Set*/

#define g_START_OF_WEEK_ATTRIBUTE_c                  				0x0020
#define g_NUMBER_OF_WEEKLY_TRANSITIONS_ATTRIBUTE_c                  0x0021
#define g_NUMBER_OF_DAILY_TRANSITIONS_ATTRIBUTE_c                  	0x0022
#define g_TEMPERATURE_SETPOINT_HOLD_ATTRIBUTE_c	                  	0x0023
#define g_TEMPERATURE_SETPOINT_HOLD_DURATION_ATTRIBUTE_c            0x0024
#define g_THERMOSTAT_PROGRAMMING_OPERATIONS_MODE_ATTRIBUTE_c        0x0025

/* HVAC Relay Attribute Set */
#define g_THERMOSTAT_RUNNING_STATE_ATTRIBUTE_c                  	0x0029

/* Thermostat Setpoint Change Tracking Attribute Set */
#define g_SETPOINT_CHANGE_SOURCE_ATTRIBUTE_c                  	0x0030
#define g_SETPOINT_CHANGE_AMOUNT_ATTRIBUTE_c                  	0x0031
#define g_SETPOINT_CHANGE_SOURCE_TIMESTAMP_ATTRIBUTE_c          0x0032

/* AC Information Attribute Set*/

#define g_AC_TYPE_ATTRIBUTE_c									0x0040
#define g_AC_CAPACITY_ATTRIBUTE_c								0x0041
#define g_AC_REFRIGERANT_TYPE_ATTRIBUTE_c						0x0042
#define g_AC_COMPRESSOR_ATTRIBUTE_c								0x0043
#define g_AC_ERROR_CODE_ATTRIBUTE_c								0x0044
#define g_AC_LOUVER_POSITION_ATTRIBUTE_c						0x0045
#define g_AC_COIL_TEMPERATURE_ATTRIBUTE_c						0x0046


#endif

/* Enumerations for Thermostat Commands */
typedef enum Thermostat
{
   g_Set_Point_Raise_Lower_Command_c
#if(g_HA_1_2_d == 1)	
   ,g_Set_Weekly_Schedule_Command_c
   ,g_Get_Weekly_Schedule_Command_c
   ,g_Clear_Weekly_Schedule_Command_c
   ,g_Get_Relay_Status_Log_Command_c
   ,g_Current_Weekly_Schedule_Command_c
   ,g_Relay_Status_Log_Command_c
#endif
}Thermostat;

/* Mode field Values for the Setpoint Raise/Lower Command */
enum{
   g_Set_Point_Heat_c            = 0x00 ,
   g_Set_Point_Cool_c            = 0x01,
   g_Set_Point_Both_Heat_Cool_c  = 0x02
};


#if(g_HA_1_2_d == 1)
enum Cooling_System_Stage{
	g_Cool_Stage_One_c			=0x00,
	g_Cool_Stage_Two_c			=0x01,
	g_Cool_Stage_Three_c		=0x02
};
enum Heating_System_Stage{
	g_Heat_Stage_One_c			=0x00,
	g_Heat_Stage_Two_c			=0x01,
	g_Heat_Satge_Three_c		=0x02
};

enum Heating_System_Type{
	g_Conventional_c			=0x00,
	g_Heat_Pump_c			    =0x01
};

enum Heating_Fuel_Source{
	g_Electric_B_c			=0x00,
	g_Gas_O_c			    =0x01
};

enum Thermostat_Running_Mode{
	g_Off_c			     =0x00,
	g_Cool_c			 =0x03,
	g_Heat_c		     =0x04
};
#endif
enum System_Mode{
	g_System_Mode_Off_c			     =0x00,
	g_Auto_c			 			 =0x01,	
	g_System_Mode_Cool_c			 =0x02,
	g_System_Mode_Heat_c		     =0x03,
	g_Emergency_Heating_c  			 =0x04,
	g_Pre_Cooling_c		 			 =0x05,
	g_Fan_c			     			 =0x06,
	g_Dry_c				 			 =0x07,
	g_Sleep_c			 			 =0x08
};

#if(g_HA_1_2_d == 1)
enum Start_Of_week{
	g_Sunday_c			     =0x00,
	g_Monday_c			 	 =0x01,	
	g_Tuesday_c			     =0x02,
	g_Wednesday_c		     =0x03,
	g_Thursday_c  			 =0x04,
	g_Friday_c		         =0x05,
	g_Saturday_c			 =0x06

};

enum Set_Point_Hold{
	g_Set_Point_Hold_Off_c			=0x00,
	g_Set_Point_Hold_On_c			=0x01
};

enum Thermostat_Running_State {
	g_Heat_State_On_c			=0x00,
	g_Cool_State_On_c			=0x01,
	g_Fan_State_On_c			=0x02,
	g_Heat_2nd_Satge_State_On_c			=0x03,
	g_Cool_2nd_Satge_State_On_c			=0x04,
	g_Fan_2nd_Satge_State_On_c			=0x05,
	g_Fan_3rd_Satge_State_On_c			=0x06
	
};
#endif

/* events for Setpoint Raise/Lower Command */
enum HVAC_Cluster_Events
{
   g_Heat_Setpoint_Raised_Event_c = 0x30,
   g_Heat_Setpoint_Lowered_Event_c,
   g_Cool_Setpoint_Raised_Event_c,
   g_Cool_Setpoint_Lowered_Event_c
#if(g_HA_1_2_d == 1)	
   ,g_Set_Weekly_Schedule_Event_c=0x34
   ,g_Get_Weekly_Schedule_Event_c
   ,g_Clear_Weekly_Schedule_Event_c
   ,g_Get_Relay_Status_Log_Event_c
   ,g_Current_Weekly_Schedule_Event_c
   ,g_Relay_Status_Log_Event_c
   ,
#endif
};


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/


/* Format of the Setpoint Raise/Lower Command Payload */
typedef struct Setpoint_Raise_Lower_Command_Tag
{
   uint8_t mode;
   int8_t amount;
}__attribute__((__packed__))Setpoint_Raise_Lower_Command_t;

/* Format of the Set Weekly Schedule Command Payload*/

typedef struct Transition_Sequence_Tag
{
  	uint16_t transition_time;
	int16_t  heat_set_point;
	int16_t  cool_set_point;
}__attribute__((__packed__))Transition_Sequence_t;

typedef struct Set_weekly_Schedule_Tag
{
	uint8_t number_of_transitions_for_sequence;
	uint8_t day_of_week_for_sequence;
	uint8_t mode_for_sequence;
	Transition_Sequence_t transition_sequence[10];	
}__attribute__((__packed__))Set_Weekly_Schedule_t;

/* Format of the Get Weekly Schedule Command Payload*/

typedef struct Get_weekly_Schedule_Tag
{
  	uint8_t days_to_return;
	uint8_t mode_to_return;
}__attribute__((__packed__))Get_weekly_Schedule_t;

/* Format of the current Weekly Schedule Command Payload*/

typedef struct Current_weekly_Schedule_Tag
{
	uint8_t number_of_transitions_for_sequence;
	uint8_t day_of_week_for_sequence;
	uint8_t mode_for_sequence;
	Transition_Sequence_t transition_sequence[10];	
}__attribute__((__packed__))Current_Weekly_Schedule_t;

/* Format of the relay status log Command Payload*/

typedef struct relay_status_log_Tag {
	uint16_t time_of_day;
	uint16_t relay_status;
	int16_t local_temperature;
	uint8_t humidity_in_percenatge;
	int16_t set_point;
	uint16_t unread_entries;
} __attribute__((__packed__))relay_status_log_t;

/*-----------------------------------------------------------------------------
* Type definitions
*----------------------------------------------------------------------------*/
typedef struct Thermostat_Boundary_Values_Tag
{
   uint16_t attribute_id;
   int16_t lower_boundary_value;
   int16_t  upper_boundary_value;
}__attribute__((__packed__))Thermostat_Boundary_Values_t;



typedef struct HVAC_Thermostat_Cluster_Attributes_Tag
{
   uint8_t a_local_temperature[2];
#if ( g_OUTDOOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_outdoor_temperature[2];
#endif

#if ( g_OCCUPANCY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t ocupancy;
#endif

#if ( g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_abs_min_heat_setpoint_limit[2];
#endif

#if ( g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_abs_max_heat_setpoint_limit[2];
#endif

#if ( g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_abs_min_cool_setpoint_limit[2];
#endif

#if ( g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_abs_max_cool_setpoint_limit[2];
#endif
#if ( g_PI_HEATING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t PI_cooling_demand;
#endif
#if(g_HA_1_2_d == 1)
#if ( g_HVAC_SYSTEM_TYPE_CONFIGURATION_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t a_hvac_system_type_configuration;
#endif
#endif

#if ( g_OUTDOOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t PI_heating_demand;
#endif

#if ( g_LOCAL_TEMPERATURE_CALIBRATION_ATTRIBUTE_ENABLE_d == 1 )
   int8_t local_temperature_calibration;
#endif

   uint8_t a_occupied_cooling_setpoint[2];
   uint8_t a_occupied_heating_setpoint[2];

#if ( g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_unoccupied_cooling_setpoint[2];
#endif

#if ( g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_unoccupied_heating_setpoint[2];
#endif

#if ( g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_min_heat_setpoint_limit[2];
#endif

#if ( g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_max_heat_setpoint_limit[2];
#endif

#if ( g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_min_cool_setpoint_limit[2];
#endif

#if ( g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_max_cool_setpoint_limit[2];
#endif

#if ( g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_ENABLE_d == 1 )
   int8_t min_setpoint_dead_band;
#endif

#if ( g_REMOTE_SENSING_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t remote_sensing;
#endif

   uint8_t control_sequence_of_operation;
   uint8_t system_mode;

#if ( g_THERMOSTAT_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t alarm_mask;
#endif
#if(g_HA_1_2_d == 1)

#if (g_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_ENABLE_d == 1)
	uint8_t thermostat_running_mode;
#endif

#if ( g_START_OF_WEEK_ATTRIBUTE_ENABLE_d == 1)
	uint8_t start_of_week;
#endif
#if ( g_NUMBER_OF_WEEKLY_TRANSITIONS_ATTRIBUTE_ENABLE_d == 1)
	uint8_t number_of_weekly_transitions;
#endif
#if ( g_NUMBER_OF_DAILY_TRANSITIONS_ATTRIBUTE_ENABLE_d == 1)
	uint8_t number_of_daily_transitions;
#endif
#if ( g_TEMPERATURE_SETPOINT_HOLD_ATTRIBUTE_ENABLE_d == 1)
	uint8_t setpoint_hold;
#endif
#if ( g_TEMPERATURE_SETPOINT_HOLD_DURATION_ATTRIBUTE_ENABLE_d == 1)
	uint8_t setpoint_hold_duration;
#endif
#if ( g_THERMOSTAT_PROGRAMMING_OPERATIONS_MODE_ATTRIBUTE_ENABLE_d == 1)
	uint8_t programming_operations_mode;
#endif

#if ( g_THERMOSTAT_RUNNING_STATE_ATTRIBUTE_ENABLE_d	 == 1)
	uint8_t thermostat_running_state[2];
#endif

/* Thermostat Setpoint Change Tracking Attribute Set */
	
#if ( g_SETPOINT_CHANGE_SOURCE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t setpoint_change_source;
#endif	
#if ( g_SETPOINT_CHANGE_AMOUNT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t setpoint_change_amount[2];	
#endif	
#if ( g_SETPOINT_CHANGE_SOURCE_TIMESTAMP_ATTRIBUTE_ENABLE_d == 1 )	
	uint8_t setpoint_change_source_time_stamp[4];
#endif	
/* AC Information Attribute Set*/

#if ( g_AC_TYPE_ATTRIBUTE_ENABLE_d == 1 )	
	uint8_t ac_type;
#endif	
#if ( g_AC_CAPACITY_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t ac_capacity;
#endif	
#if ( g_AC_REFRIGERANT_TYPE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t ac_refrigerant;
#endif	
#if ( g_AC_COMPRESSOR_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t ac_compressor;
#endif	
#if ( g_AC_ERROR_CODE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t ac_error_code[4];
#endif	
#if ( g_AC_LOUVER_POSITION_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t ac_louver_position;
#endif
#if ( g_AC_COIL_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )	
	uint8_t ac_coil_temperature[2];
#endif
#endif		
}__attribute__((__packed__))HVAC_Thermostat_Cluster_Attributes_t;


typedef struct HVAC_Domain_Thermostat_Request_Tag
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
	Setpoint_Raise_Lower_Command_t setpoint_raise_lower_command;
}__attribute__((__packed__))HVAC_Domain_Thermostat_Request_t;

/* Format of the Setpoint Raise/Lower Command Payload */
typedef struct Set_Point_Raise_Lower_Cmd_Tag
{
   uint8_t mode;
   int8_t amount;
}__attribute__((__packed__))Set_Point_Raise_Lower_Cmd_t;

typedef struct HVAC_Event
{
   uint8_t event_id;
   uint8_t event_length;
   union HVAC_event_data
   {
      Set_Point_Raise_Lower_Cmd_t set_point_raise_lower_cmd_event_data;
   }HVAC_event_data;
}__attribute__((__packed__))HVAC_Event;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/* Thermostat cluster info */

#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Thermostat_Server_Cluster_Info;
#endif

#if ( g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Thermostat_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* ZCL_Handle_Thermostat_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : uint8_t
*
* Input Parameters:
*    endPointId -> On which end point this command is received
*    asduLength -> Length of command payload
*    pReceivedAsdu -> pointer to command payload
*    pResponseAsduStartLocation -> pointer response to be created
*    pActionHandler -> pointer to action to be done by the application
*    pZCLClusterInfo -> pointer to Cluster Information
*    pAttrsDataBaseLocation -> Attribute data base pointer for the cluster
*
* Output Parameters: None
*
* Scope of the function:
*  This function is called within ZCL_Handle_Cluster_Specific_Cmd() function,
*  when a HVAC Thermostate cluster command is received in the APSDE-Data indication.
*
* Purpose of the function:
*	This function handles the Thermostat cluster command received in the
*  APSDE indication 	

*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Thermostat_Cluster_Cmd
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);

#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t ZCL_Check_Boundary_Values ( uint8_t *p_attribute_id,
                                    uint8_t *p_attribute_data,
                                    uint8_t data_length   );
#endif          /* ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if(g_HA_1_2_d == 1)
uint8_t ZCL_CreateWeeklyScheduleCommand (uint8_t tranSeqNum,
				uint8_t* pRspLocation, Set_Weekly_Schedule_t *setWeeklySchedule );

#endif

#endif              /*  ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_HVAC_THERMOSTAT_CLUSTER_H_*/
