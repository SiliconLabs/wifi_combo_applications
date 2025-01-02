#ifndef _ZCL_GENERAL_ALARMS_CLUSTER_H_
#define _ZCL_GENERAL_ALARMS_CLUSTER_H_

#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*Attributes and commands for sending notifications and
configuring alarm functionality.0x0009*/
#define g_ALARMS_CLUSTER_c                                          0x0009

/*Attributes for Alarms Cluster*/
#define g_ALARM_COUNT_ATTRIBUTE_c                                   0x0000


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t  ga_Alarms_Server_Cluster_Info;
#endif

#if ( g_ALARMS_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t  ga_Alarms_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

enum
{	
	g_After_Attribute_Change_c,
	g_After_Timer_Expiry_c
};

typedef enum Alarms_Cluster
{
   g_Reset_Alarm_Command_c,
   g_Reset_All_Alarms_Command_c,
   g_Get_Alarm_Command_c,
   g_Reset_Alarm_Log_Command_c
}Alarms_Cluster;


typedef enum Alarm_Response_Commands
{
   g_Alarm_Command_c,
   g_Get_Alarm_Response_Command_c
}Alarm_command;

typedef enum
{
   g_Alarm_Occurred_Event_c = 0x80,
   g_Reset_Alarm_Command_Event_c,
   g_Reset_All_Alarms_Command_Event_c,
   g_Get_Alarm_Command_Event_c,
   g_Reset_Alarm_Log_Command_Event_c
}Alarms_Events;


/*alarm count for alarms cluster*/
typedef struct Alarm_Cluster_Tag
{
   uint8_t alarm_count[2];
}__attribute__((__packed__))Alarm_Cluster_t;


typedef struct Alarm_Table_Tag
{
   uint8_t alarm_code;
   uint16_t cluster_id;
   uint8_t a_time_stamp[4];
}__attribute__((__packed__))Alarm_Table_t;

typedef struct ALARM_AlarmTable_Tag
{
    uint8_t status;
    uint8_t alarmCode;
    uint16_t clusterId;
    uint32_t timeStamp;
} __attribute__((__packed__))ALARM_AlarmTable_t;

typedef struct Alarm_Command_Tag
{
   uint8_t alarm_code;
   uint16_t cluster_id;
}__attribute__((__packed__))Alarm_Command_t;

typedef struct Reset_Alarm_Command_Tag
{
   uint8_t alarm_code;
   uint16_t cluster_id;
}__attribute__((__packed__))Reset_Alarm_Command_t;

typedef struct General_Domain_Alarms_Request_Tag
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union Alarms_Commands_Tag
   {
      Alarm_Command_t alarm_command;
      Reset_Alarm_Command_t reset_alarm_command;
   }Alarms_Commands_t;
}__attribute__((__packed__))General_Domain_Alarms_Request_t;


typedef struct Get_Alarm_Command_Response_Tag
{
   uint8_t status;
   uint8_t alarm_code;
   uint8_t a_cluster_id[2];
   uint8_t a_time_stamp[4];
}__attribute__((__packed__))Get_Alarm_Command_Response_t;

/* stucture for ZCL response commands
Field: Command Identifier - gives the command id
Field: Command type - indicates the generic command or cluster specific
   command
Field: manufacture specific - indicates if manufacture specific is set
Field: manufacture code - manufacture code
Field: Actual payload   */
typedef struct ZCL_Alarms_Cluster_Response_Command_Tag
{
   uint8_t command_identifier;
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacture_code[2];
   union
   {
      Get_Alarm_Command_Response_t get_alarm_command_response;
   }ZCL_Response;
}__attribute__((__packed__))ZCL_Alarms_Cluster_Response_Command_t;


typedef struct Alarms_Event
{
   uint8_t event_id;
   uint8_t event_length;
   union alarms_event_data
   {
      Alarm_Command_t alarm_event_data;
   }alarms_event_data;
}__attribute__((__packed__))Alarms_Event;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_Alarms_Cluster_Cmd
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
*  when a Alarms cluster command is received in the Data indication.
*
* Purpose of the function:
*	This function handles the Alarms cluster command received in the data
*  indication
*     	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Alarms_Cluster_Cmd
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);

void Reset_Get_Update_Alarm_Table
(
   uint8_t command_id,
   uint8_t *p_Alarm_Command
);

#endif          /* ( g_ALARMS_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_GENERAL_ALARMS_CLUSTER_H_*/
