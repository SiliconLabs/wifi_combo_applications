#ifndef _ZCL_SECURITY_AND_SAFETY_IAS_ACE_CLUSTER_H_
#define _ZCL_SECURITY_AND_SAFETY_IAS_ACE_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/*Attributes and commands for IAS Ancillary Control
Equipment.*/
#define g_IAS_ACE_CLUSTER_c                                         0x0501

typedef enum IAS_ACE_Request_Commands
{
   g_Arm_Command_c,
   g_Bypass_Command_c,
   g_Emergency_Command_c,
   g_Fire_Command_c,
   g_Panic_Command_c,
   g_Get_Zone_Id_Map_Command_c,
   g_Get_Zone_Information_Command_c
}IAS_ACE_Request_Commands;


typedef enum IAS_ACE_Response_Commands
{
   g_Arm_Response_Command_c,
   g_Get_Zone_Id_Map_Response_Command_c,
   g_Get_Zone_Information_Response_Command_c
}IAS_ACE_Response_Commands;


/*Events for IAS ACE Cluster*/
typedef enum
{
   g_Arm_Command_Event_c = 0x60,
   g_ByPass_Command_Event_c = 0x61,
   g_Emergency_Command_Event_c = 0x62,
   g_Fire_Command_Event_c = 0x63,
   g_Panic_Command_Event_c = 0x64,
   g_Get_Zone_Id_Map_Command_Event_c = 0x65,
   g_Get_Zone_Information_Command_Event_c = 0x66,
   g_Arm_Response_Event_c = 0x67,
   g_Get_Zone_Id_Map_Response_Event_c = 0x68,
   g_Get_Zone_Information_Response_Event_c = 0x69
}Command_Event;

/*Arm Mode field values */
enum Arm_Mode_Field_Values
{
   g_Arm_Mode_Disarm_c,
   g_Arm_Mode_Arm_Day_Home_Zones_Only_c,
   g_Arm_Mode_Arm_Night_Sleep_Zones_Only_c,
   g_Arm_Mode_Arm_All_Zones_c
};


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct Arm_Command_Tag
{
   uint8_t arm_mode;
}__attribute__((__packed__))Arm_Command_t;

typedef struct By_Pass_Command_Tag
{
   uint8_t number_of_zones;
   uint8_t a_zone_id_list[1];
}__attribute__((__packed__))By_Pass_Command_t;

typedef struct Get_Id_Zone_Information_Command_Tag
{
   uint8_t zone_id;
}__attribute__((__packed__))Get_Id_Zone_Information_Command_t;



typedef struct IAS_Domain_IAS_ACE_Request_Tag
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union
   {
      Arm_Command_t  arm_command;
      By_Pass_Command_t by_pass_command;
      Get_Id_Zone_Information_Command_t get_zone_id_information_command;
   }IAS_ACE_Commands;
}__attribute__((__packed__))IAS_Domain_IAS_ACE_Request_t;


typedef struct IAS_ACE_Event
{
   uint8_t event_id;
   uint8_t event_length;
   union IAS_ACE_event_data
   {
      Arm_Command_t  arm_command_event_data;
      By_Pass_Command_t by_pass_command_event_data_event_data;
      Get_Id_Zone_Information_Command_t get_zone_id_information_command_event_data;
   }IAS_ACE_event_data;
}__attribute__((__packed__))IAS_ACE_Event;

typedef struct Arm_Response_Command_Tag
{
   uint8_t arm_notification;
}__attribute__((__packed__))Arm_Response_Command_t;

typedef struct Get_Zone_Id_Map_Response_Command_Tag
{
   uint8_t zone_id_map_section[32];
}__attribute__((__packed__))Get_Zone_Id_Map_Response_Command_t;

typedef struct Get_Zone_Information_Response_Command_Tag
{
   uint8_t zone_id;
   uint8_t a_zone_type[2];
   uint8_t a_zone_address[8];
}__attribute__((__packed__))Get_Zone_Information_Response_Command_t;


/*stucture for ZCL response commands
Field: Command Identifier - gives the command id
Field: Command type - indicates the generic command or cluster specific
   command
Field: manufacture specific - indicates if manufacture specific is set
Field: manufacture code - manufacture code
Field: Actual payload   */
typedef struct ZCL_IAS_ACE_Response_Command_Tag
{
   uint8_t command_identifier;
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacture_code[2];
   union
   {
      Arm_Response_Command_t arm_response;
      Get_Zone_Id_Map_Response_Command_t get_zone_id_map_response;
      Get_Zone_Information_Response_Command_t get_zone_info_response;
   }ZCL_Response;
}__attribute__((__packed__))ZCL_IAS_ACE_Response_Command_t;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
extern const ZCL_Cluster_Info_t ga_IAS_ACE_Server_Cluster_Info;
extern const ZCL_Cluster_Info_t ga_IAS_ACE_Client_Cluster_Info;

///*module variable to keep the information about which Zones are allocated
//for Zone id map response*/
//extern uint8_t ga_Zone_Id_Map_Section[32];

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_IAS_ACE_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value :
*	uint8_t
* Input Parameters:
*	endPointId -
*	asduLength -
*	pReceivedAsdu -
*	pResponseAsduStartLocation
*	pActionHandler
*	pZCLClusterInfo
*	pAttrsDataBaseLocation
* Output Parameters:
*  None
* Scope of the function:
*  This function is called within ZCL_Handle_Cluster_Specific_Cmd() function,
*  when a Basic cluster command is received in the APSDE-Data indication.
*
* Purpose of the function:
*	This function handles the IAS ACE cluster command received in the
*  APSDE indication
*     	
*Before the function is called:
*
*After the function is called:
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_IAS_ACE_Cluster_Cmd (  	
								uint8_t endPointId,
								uint8_t asduLength,
								uint8_t* pReceivedAsdu,
								uint8_t* pResponseAsduStartLocation,
								ZCL_ActionHandler_t *pActionHandler,
								ZCL_Cluster_Info_t const *pZCLClusterInfo,
								uint8_t * pAttrsDataBaseLocation );


#endif  /*_ZCL_SECURITY_AND_SAFETY_IAS_ACE_CLUSTER_H_*/
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
