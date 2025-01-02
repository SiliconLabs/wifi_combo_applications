#ifndef _ZCL_SECURITY_AND_SAFETY_IAS_WD_CLUSTER_H_
#define _ZCL_SECURITY_AND_SAFETY_IAS_WD_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*Attributes and commands for IAS Warning Devices.*/
#define g_IAS_WD_CLUSTER_c                                       0x0502

/*Attributes supported by IAS Warning device cluster*/
#define g_IAS_WD_MAX_DURATION_ATTRIBUTE_c                        0x0000

/* Enumerations for Security and Safety Commands */
typedef enum IAS_WD
{
   g_Start_Warning_Command_c,
   g_Squawk_Command_c
}IAS_WD;

/*Events for IAS WD cluster Commands*/
typedef enum
{
   g_Start_Warning_Command_Event_c = 0x70,
   g_Squawk_Command_Event_c = 0x71
}Warning_Command_Event;

/*Warning modes*/
enum
{
   g_Warning_Mode_Stop_c,
   g_Warning_Mode_Burglar_c,
   g_Warning_Mode_Fire_c,
   g_Warning_Mode_Emergency_c
};

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct IAS_WD_Cluster_Tag
{
   uint8_t wd_max_duration[2];
}__attribute__((__packed__))IAS_WD_Cluster_t;


typedef struct Start_Warning_Command_Tag
{
   uint8_t warning_strobe_mode;
   uint8_t a_max_duration[2];
}__attribute__((__packed__))Start_Warning_Command_t;


typedef struct Squawk_Command_Tag
{
   uint8_t squawk_mode;
}__attribute__((__packed__))Squawk_Command_t;

typedef struct IAS_Domain_IAS_WD_Request_Tag
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union
   {
      Start_Warning_Command_t start_warning_command;
      Squawk_Command_t squawk_command;

   }IAS_WD_Commands;
}__attribute__((__packed__))IAS_Domain_IAS_WD_Request_t;


typedef struct IAS_WD_Event
{
   uint8_t event_id;
   uint8_t event_length;
   union IAS_WD_event_data
   {
      Start_Warning_Command_t start_warning_command_event_data;
      Squawk_Command_t squawk_command_event_data;
   }IAS_WD_event_data;
}__attribute__((__packed__))IAS_WD_Event;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*Cluster Info for IAS Zone Cluster*/
extern const ZCL_Cluster_Info_t ga_IAS_WD_Server_Cluster_Info;
extern const ZCL_Cluster_Info_t ga_IAS_WD_Client_Cluster_Info;
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_IAS_WD_Cluster_Cmd
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
*Purpose of the function:
*	This function handles the IAS WD cluster command received in the
*  APSDE indication
*     	
*Before the function is called:
*
*After the function is called:
*
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_IAS_WD_Cluster_Cmd (  	
								uint8_t endPointId,
								uint8_t asduLength,
								uint8_t* pReceivedAsdu,
								uint8_t* pResponseAsduStartLocation,
								ZCL_ActionHandler_t *pActionHandler,
								ZCL_Cluster_Info_t const *pZCLClusterInfo,
								uint8_t * pAttrsDataBaseLocation );

#endif                                 /*_ZCL_SECURITY_AND_SAFETY_IAS_WD_CLUSTER_H_*/
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
