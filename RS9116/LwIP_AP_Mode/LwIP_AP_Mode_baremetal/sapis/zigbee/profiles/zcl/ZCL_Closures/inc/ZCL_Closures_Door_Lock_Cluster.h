/******************************************************************************
* ZCL_Closures_Door_Lock_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Door Lock Cluster attribute and cmmnd information
******************************************************************************/
#ifndef _ZCL_CLOSURE_DOOR_LOCK_CLUSTER_H_
#define _ZCL_CLOSURE_DOOR_LOCK_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#if(g_DOOR_LOCK_CLUSTER_ENABLE_d == 1)
#define g_DOOR_LOCK_CLUSTER_c                                  0x0101
/* Door Lock Cluster Attributes */

#if(g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d == 1)
#define g_LOCK_STATE_ATTRIBUTE_c                               0x0000
#define g_LOCK_TYPE_ATTRIBUTE_c                                0x0001
#define g_ACTUATOR_ENABLED_ATTRIBUTE_c                         0x0002
#define g_DOOR_STATE_ATTRIBUTE_c                               0x0003
#define g_DOOR_OPEN_EVENTS_ATTRIBUTE_c                         0x0004
#define g_DOOR_CLOSED_EVEVTS_ATTRIBUTE_c                       0x0005
#define g_DOOR_OPEN_PERIOD_ATTRIBUTE_c                         0x0006

#endif

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
extern const ZCL_Cluster_Info_t ga_Door_Lock_Server_Cluster_Info;
extern const ZCL_Cluster_Info_t ga_Door_Lock_Client_Cluster_Info;
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
/*enumerations for General Domain - cluster specific command events*/
#if(g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d == 1)
typedef enum Closures_Door_Lock_Cluster
{
   g_Lock_Door_Event_c = 0x8a,
   g_Unlock_Door_Event_c

}Closures_Door_Lock_Cluster;

typedef enum Lock_State
{
  Not_Fully_Locked = 0x00,
  Locked,
  Unlocked
}Lock_State;

typedef enum Lock_Type
{
  Dead_Bolt = 0x00,
  Magnetic,
  Other
}Lock_Type;

typedef enum Actuator_Enabled
{
  Disabled = 0x00,
  Enabled
}Actuator_Enabled;

typedef enum Door_State
{
  Open = 0x00,
  Closed,
  Error
}Door_State;

#endif


/*enumerations for Door Lock cluster commands*/
typedef enum Door_Lock_Cluster
{
   g_Lock_Door_Command_c,
   g_Unlock_Door_Command_c

}Door_Lock_Cluster;



typedef enum Door_Lock_Cluster_Response
{
   g_Lock_Door_Response_c,
   g_Unlock_Door_Response_c

}Door_Lock_Cluster_Response;

typedef enum Closures_Door_Lock_Cluster_Response
{
   g_Lock_Door_Response_Event_c = 0x03,
   g_Unlock_Door_Event_Response_c

}Closures_Door_Lock_Cluster_Response;

/*typedef struct Closure_Door_Lock_Request
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union Door_Lock_Commands
   {
        uint8_t Lock_Door;
        uint8_t Unlock_Door;
   }Door_Lock_Commands;
}__attribute__((__packed__))Closure_Door_Lock_Request;



typedef struct Lock_Door_Response_Tag
{
   uint8_t lock_status;
}__attribute__((__packed__))Lock_Door_Response_t;

typedef struct Unlock_Door_Response_Tag
{
   uint8_t unock_status;
}__attribute__((__packed__))Unlock_Door_Response_t;
*/
/*stucture for ZCL response commands
Field: Command Identifier - gives the command id
Field: Command type - indicates the generic command or cluster specific
                        command
Field: manufacture specific - indicates if manufacture specific is set
Field: manufacture code - manufacture code
Field: Actual payload   */
/*typedef struct ZCL_Door_Lock_Cluster_Response_Command_Tag
{
   uint8_t command_identifier;
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacture_code[2];
   union
   {
      Lock_Door_Response_t lock_door_response;
      Unlock_Door_Response_t unlock_door_response;
   }ZCL_Response;
}__attribute__((__packed__))ZCL_Door_Lock_Cluster_Response_Command_t;

*/
/*---------------------------------------------------------------------------*/

typedef struct Door_Lock_Cluster_Tag
{

   uint8_t lock_state;
   uint8_t lock_type;
   uint8_t actuator_enabled;
#if ( g_DOOR_STATE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t door_state;
#endif
#if ( g_DOOR_OPEN_EVENTS_ATTRIBUTE_ENABLE_d == 1 )
   uint32_t door_open_events;
#endif
#if ( g_DOOR_CLOSED_EVEVTS_ATTRIBUTE_ENABLE_d == 1 )
   uint32_t door_closed_events;
#endif
#if ( g_DOOR_OPEN_PERIOD_ATTRIBUTE_ENABLE_d == 1 )
   uint16_t door_open_peroid;
#endif
}__attribute__((__packed__))Door_Lock_Cluster_t;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Closure_Door_Lock_Cmd
(
     uint8_t endPointId,
     uint8_t asduLength,
     uint8_t* pReceivedAsdu,
     uint8_t* pResponseAsduStartLocation,
     ZCL_ActionHandler_t *pActionHandler,
     ZCL_Cluster_Info_t const *pZCLClusterInfo,
     uint8_t * pAttrsDataBaseLocation
);

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*g_DOOR_LOCK_CLUSTER_ENABLE_d*/

#endif                                  /* _ZCL_CLOSURE_DOOR_LOCK_CLUSTER_H_ */
