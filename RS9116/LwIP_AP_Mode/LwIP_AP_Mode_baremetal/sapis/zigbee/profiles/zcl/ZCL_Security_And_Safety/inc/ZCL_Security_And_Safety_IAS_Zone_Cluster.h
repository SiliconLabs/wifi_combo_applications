#ifndef _ZCL_SECURITY_AND_SAFETY_IAS_ZONE_CLUSTER_H_
#define _ZCL_SECURITY_AND_SAFETY_IAS_ZONE_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_INVALID_ZONE_INDEX_c                                 0xFF

#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )

#define ZCL_IAS_Zone_Create_IEEE_Addr_Request(short_address)   \
         Create_IEEE_Address_Request   \
         (                             \
            short_address              \
         );
#elif ( g_IAS_ZONE_CLUSTER_ENABLE_d == 0 )

#define ZCL_IAS_Zone_Create_IEEE_Addr_Request(short_address)

#endif


/*Cluster for IAS security zone devices.*/
#define g_IAS_ZONE_CLUSTER_c                                   0x0500

/*Attributes supported by IAS Zone ClusterS*/
/*Attribute id defined for Zone state*/
#define g_IAS_ZONE_INFO_ZONE_STATE_ATTRIBUTE_c                 0x0000
/*Attribute id defined for Zone type*/
#define g_IAS_ZONE_INFO_ZONE_TYPE_ATTRIBUTE_c                  0x0001
/*Attribute id defined for Zone status*/
#define g_IAS_ZONE_INFO_ZONE_STATUS_ATTRIBUTE_c                0x0002
/*Attribute id defined for IAS CIE address*/
#define g_IAS_ZONE_SETTINGS_IAS_CIE_ADDRESS_ATTRIBUTE_c        0x0010

/*Macros for different zone types*/
#define g_ZONE_TYPE_CIE_c                                      0x0000
#define g_ZONE_TYPE_MOTION_SENSOR_c                            0x000d
#define g_ZONE_TYPE_CONTACT_SWITCH_c                           0x0015
#define g_ZONE_TYPE_FIRE_SENSOR_c                              0x0028
#define g_ZONE_TYPE_WATER_SENSOR_c                             0x002A
#define g_ZONE_TYPE_GAS_SENSOR_c                               0x002B
#define g_ZONE_TYPE_PERSONAL_EMERGENCY_DEVICE_c                0x002C
#define g_ZONE_TYPE_VIBRATION_MOVEMENT_SENSOR_c                0x002D
#define g_ZONE_TYPE_REMOTE_CONTROL_c                           0x010f
#define g_ZONE_TYPE_KEY_FOB_c                                  0x0115
#define g_ZONE_TYPE_KEP_PAD_c                                  0x021d
#define g_ZONE_TYPE_STANDARD_WD_c                              0x0225
#define g_ZONE_TYPE_INVALID_ZIGBEE_ZONE_TYPE_c                 0x0226
#define g_ZONE_TYPE_INVALID_ZONE_TYPE_c                        0xFFFF


/* Enumerations for Security and Safety Commands */
typedef enum IAS_Zone_Request
{
   g_Zone_Status_Change_Notification_Command_c,
   g_Zone_Enroll_Request_Command_c
}IAS_Zone_Request;

/* Enumerations for Security and Safety Commands */
typedef enum IAS_Zone_Response
{
   g_Zone_Enroll_Response_Command_c
}IAS_Zone_Response;

typedef enum Enroll_Response_Command_Status
{
   g_Enroll_Response_Success_c,
   g_Enroll_Not_Supported_c,
   g_No_Enroll_Permit_c,
   g_Too_Many_Zones_c

}Enroll_Response_Command_Status;


/*events for IAS Zone Cluster*/
typedef enum
{
   g_Zone_Status_Change_Notification_Command_Event_c = 0x50,
   g_Zone_Enroll_Request_Command_Event_c = 0x51,
   g_Zone_Enroll_Response_Event_c   = 0x52
}Command_Events;


typedef enum
{
   g_Get_Zone_Id_Map_c,
   g_Get_Zone_Information_c
}Get_Zone;


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct IAS_Zone_Cluster_Tag
{
   uint8_t zone_state;
   uint8_t a_zone_type[2];
   uint8_t a_zone_status[2];
   uint8_t IAS_CIE_address[8];
}__attribute__((__packed__))IAS_Zone_Cluster_t;


typedef struct Enroll_Zone_Request_Command_Tag
{
   uint8_t a_zone_type[2];
   uint8_t a_manufacturer_code[2];
}__attribute__((__packed__))Enroll_Zone_Request_Command_t;

typedef struct Status_Change_Notification_Command_Tag
{
   uint8_t a_zone_status[2];
   uint8_t extended_status;
}__attribute__((__packed__))Status_Change_Notification_Command_t;


typedef struct IAS_Domain_IAS_Zone_Request_Tag
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union IAS_Zone_Commands_Tag
   {
      Enroll_Zone_Request_Command_t Enroll_Zone_Request_Command;
      Status_Change_Notification_Command_t Status_Change_Notification_Command;
   }IAS_Zone_Commands_t;
}__attribute__((__packed__))IAS_Domain_IAS_Zone_Request_t;



typedef struct IAS_Zone_Event
{
   uint8_t event_id;
   uint8_t event_length;
   union IAS_Zone_event_data
   {
      Enroll_Zone_Request_Command_t Enroll_Zone_Req_Cmd_Event_Data;
      Status_Change_Notification_Command_t Status_Change_Notfn_Cmd_Event_Data;
   }IAS_Zone_event_data;
}__attribute__((__packed__))IAS_Zone_Event;

typedef struct Enroll_Response_Tag
{
   uint8_t enroll_response_code;
   uint8_t zone_id;
}__attribute__((__packed__))Enroll_Response_t;

/*stucture for ZCL response commands
Field: Command Identifier - gives the command id
Field: Command type - indicates the generic command or cluster specific
   command
Field: manufacture specific - indicates if manufacture specific is set
Field: manufacture code - manufacture code
Field: Actual payload   */
typedef struct ZCL_Zone_Cluster_Response_Command_Tag
{
   uint8_t command_identifier;
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacture_code[2];
   union
   {
      Enroll_Response_t enroll_response;
   }ZCL_Response;
}__attribute__((__packed__))ZCL_Zone_Cluster_Response_Command_t;


/*Structure for IAS Zone Table*/
typedef struct IAS_Zone_Table_Tag
{
   uint8_t a_zone_type[2];
   uint8_t a_zone_address[8];
}__attribute__((__packed__))IAS_Zone_Table_t;


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*Cluster Info for IAS Zone Cluster*/
extern const ZCL_Cluster_Info_t ga_IAS_Zone_Server_Cluster_Info;
extern const ZCL_Cluster_Info_t ga_IAS_Zone_Client_Cluster_Info;

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_IAS_Zone_Cluster_Cmd
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
*	This function handles the IAS zone cluster command received in the
*  APSDE indication
*     	
*Before the function is called:
*
*After the function is called:
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_IAS_Zone_Cluster_Cmd (  	
								uint8_t endPointId,
								uint8_t asduLength,
								uint8_t* pReceivedAsdu,
								uint8_t* pResponseAsduStartLocation,
								ZCL_ActionHandler_t *pActionHandler,
								ZCL_Cluster_Info_t const *pZCLClusterInfo,
								uint8_t * pAttrsDataBaseLocation );



#endif                                 /*_ZCL_SECURITY_AND_SAFETY_IAS_ZONE_CLUSTER_H_*/
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
