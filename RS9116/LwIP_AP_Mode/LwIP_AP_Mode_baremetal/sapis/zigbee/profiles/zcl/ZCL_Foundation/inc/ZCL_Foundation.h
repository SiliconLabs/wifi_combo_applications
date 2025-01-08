
#ifndef _ZCL_FOUNDATION_H_
#define _ZCL_FOUNDATION_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Interface.h"
#include "ZCL_Common.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_ZCL_SERVER_TO_CLIENT_DIRECTION_c                              0x08
#define g_ZCL_CLIENT_TO_SERVER_DIRECTION_c                              0x00
#define g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c                           0x10
#define g_ZCL_CLUSTER_SPECIFIC_COMMAND_c                                0x01
#define g_ZCL_ATTRIBUTE_ID_LENGTH_c                                     0x02
#define g_ZCL_CLUSTER_ID_LENGTH_c                                       0x02
#define g_LENGTH_OF_MANUFACTURER_CODE_c                                 0x02
#define g_ZCL_HEADER_WITH_MANUF_CODE_c                                  0x05
#define g_ZCL_HEADER_WITHOUT_MANUF_CODE_c                               0x03
#define g_ZERO_DATA_LENGTH_c                                            0x00
#define g_LENGTH_FOR_CHAR_STRING_DATA_TYPE_c                            0x10
#define g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c                           0x10
#define g_LENGTH_FOR_LONG_CHAR_STRING_DATA_TYPE_c                       0x20
#define g_LENGTH_FOR_LONG_OCTET_STRING_DATA_TYPE_c                      0x20

#define g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c                        0x03
#define g_ZCL_HEADER_LENGTH_WITH_MANUF_CODE_c                           0x05
#define g_ZCL_DIRECTION_BIT_MASK_c                                      0x08
#define g_ZCL_FRAME_CONTROL_MANUFACTURE_SPECIFIC_c                      0x04
#define g_ZCL_FRAME_TYPE_MASK_c                                         0x03

#define g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c                             0x01
#define g_ZCL_FC_PROFILE_WIDE_COMMAND_c                                 0x00
#define g_SIZE_OF_MANUFACTURER_CODE_c                                   0x02
#define g_ZCL_HEADER_COMMAND_OFFSET_c                                   0x02
#define g_EVENT_LENGTH_THREE_BYTES_c									0x03


#define ZCL_GetDirectionOfCommand( param ) ( param[0] & g_ZCL_DIRECTION_BIT_MASK_c )

#define Is_Manufacture_Specific_Cmd( param ) (( param[0] & g_ZCL_FRAME_CONTROL_MANUFACTURE_SPECIFIC_c ) )

#define GetZCLHeaderLength( param ) ( ( param[0] & g_ZCL_FRAME_CONTROL_MANUFACTURE_SPECIFIC_c ) ?  \
                    g_ZCL_HEADER_LENGTH_WITH_MANUF_CODE_c : g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c )

#define Is_ClusterSpecificCommand( param ) ( ( param[0] & g_ZCL_FRAME_TYPE_MASK_c ) )

#define Is_Default_Response_Required( param )  ( !( param[0] & g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c) )

#define Is_AttributeAnalogDataType( datatype )  \
    ((( datatype >= 0x20 ) && ( datatype <= 0x2F  )) || \
    (( datatype >= 0x38 ) && ( datatype <= 0x3a )) || (( datatype >= 0xe0 )&&( datatype <= 0xe2 )))

#define IsSignedInteger( attr )   ( ( attr >= (0x28) && attr <= (0x2f) ) ? 0x01:0x00 )

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

extern ZCL_Cluster_Info_t *mp_cluster_info;
extern uint8_t * mp_attribute_data;

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/* enumeration that is used to indicate if the attribute id is
read only or read write. If the attribute is reportable the upper most bit is
set */
typedef enum Access_Permission
{
   g_Read_Only_Attribute_c  = 0x00,
   g_Read_Write_Attribute_c = 0x01,
   g_Reportable_Attribute_c = 0x02,
   g_Scenes_Ext_Attribute_c = 0x04
}Access_Permission;


typedef enum
{
    g_Ready_State_c,
    g_IEEE_Address_Request_State_c,
    g_IEEE_Address_Response_Processing_State_c
}states;


typedef enum discoveryComplete {
	g_ZCL_DiscoveryInComplete_c = 0x00,
    g_ZCL_DiscoveryComplete_c =   0x01
}discoveryComplete;

enum
{
	m_Start_Reporting_Timer_c = 0x02
};

/*Structure for Alarm conditions*/
typedef struct Alarm_Conditions_Tag
{
   uint8_t alarm_code;
   uint16_t cluster_id;
   uint8_t a_attribute_id[2];
   uint8_t a_threshold_attribute_id[2];
   uint8_t attribute_data_length;
   uint8_t a_alarm_mask_attribute_id[2];
   uint8_t compare_for_min_or_max_threshold;
   uint8_t alarm_mask_data;
}__attribute__((__packed__))Alarm_Conditions_t;


/* structure that is used by the application for
Read Request Command structure
Field : Consists of list of attribute ids*/
typedef struct Read_Attribute_Command_Request_tag{
  uint8_t a_attrib_list[2];			
}__attribute__((__packed__))Read_Attribute_Command_Request_t;

/*structure used by the application for read attribute response
Field: num_of_attributes - number of attributes read
Field: attribute id - attribute identifier
Field: status - corresponding to the above attribute id
Field: attribute data type- gives the data type of the above attribute
Field: attribute data - for the given attribute id
The fields from attribute id till attribute data is repeated based
on the number of attributes*/
typedef struct Read_Attribute_Response_Command_Tag
{
//    uint8_t num_of_attributes;
    uint8_t a_attribute_id[2];
    uint8_t status;
    uint8_t attribute_data_type;
    uint8_t a_AttributData[1];
}__attribute__((__packed__))Read_Attribute_Response_Command_t;

/*structure for write attribute request command for the application
Field: attribute_id - gives attribute id
Field: attribute_data_type - data type of the attribute id
Field: a_attribute_data - actual data to be updated for a particular
         attribute.
*/
typedef struct Write_Attribute_Command_Tag{
   uint8_t a_attribute_id[2];
   uint8_t attribute_data_type;
   uint8_t a_attribute_data[1];
}__attribute__((__packed__))Write_Attribute_Command_t;

/*structure fro write attribute response command for the application
Field: Status - gives succes or error code
Field: Attribute id is not present if status is succes and is
present otherwise*/
typedef struct Write_Attribute_Response_Command_Tag{
   uint8_t status;
}__attribute__((__packed__))Write_Attribute_Response_Command_t;

/*structure for discover attribute command for the application
Field: start_attribute_identifier - gives the start attribute id which
      indicates from where the id should start while providing the list
      supported by the cluster. Above and equal to this start id is
      present in the discover attribute response command
Field: maximum_count - indicates how many attribute id list needs to
      be placed in the discover attribute response command
*/
typedef struct Discover_Attribute_Command_Tag
{
   uint8_t a_start_attribute_identifier[2];
   uint8_t maximum_count;
}__attribute__((__packed__))Discover_Attribute_Command_t;


/* structure for report attribute command for the application
Field: no_of_atributes_being_reported - number of attributes reported
Field: a_attribute_reports - list of attribute records */
typedef struct Report_Attributes_Command_Tag
{
   uint8_t a_attribute_identifier[2];
   uint8_t attribute_data_type;
   uint8_t attribute_data[0];
}__attribute__((__packed__))Report_Attributes_Command_t;


/* structure for discover attribute response command for the application
Field: attribute_identifier - gives the attribute id
Field: attribute_data_type - indicates data type of the attribute */
typedef struct Discover_Attribute_ResponseCommand_Tag
{
   uint8_t a_attribute_identifier[2];
   uint8_t attribute_data_type;
}__attribute__((__packed__))Discover_Attribute_ResponseCommand_t;


/*structure for default response command for the application
Field: command_identifier - gives the command id on recipt of which
         default response is created
Field: status - indicates if the command operation is successfull or
         indicates with an error code. */
typedef struct Default_ResponseCommand_Tag
{
    uint8_t command_identifier;
    uint8_t status;
}__attribute__((__packed__))Default_ResponseCommand_t;


/*Structure for Attribute_reporting_Configuration_Record_tag
Field: direction - indicates whether the reports are received or
               the reports are being sent
Field: a_attrbute_id - attribute id that is to be reported or receive reports
Field: data_type - data type of the attribute id
Field: a_minimum_reporting_interval - 16 bit minm report interval
Field: a_maximum_reporting_interval - 16 bit maxm report interval
Field: a_reportable_change - 16 bit reportable change */
typedef struct Attribute_reporting_Configuration_Record_Tag
{
	uint8_t direction;
	uint8_t a_attrbute_id[2];             /* in little endian order */
    uint8_t data_type;
    uint8_t a_minimum_reporting_interval[2];
    uint8_t a_maximum_reporting_interval[2];
    uint8_t a_reportable_change[2];
}__attribute__((__packed__))Attribute_reporting_Configuration_Record;

/*Structure for Configure_Report_Attributes_Command
Field: reporting_Configuration_Record - gives the fields of reporting
         configuration command
*/
typedef struct Configure_Report_Attributes_Command_Tag
{
   Attribute_reporting_Configuration_Record reporting_Configuration_Record[2];
}__attribute__((__packed__))Configure_Report_Attributes_Command_t;

typedef struct Read_Reporting_Configurartion_Command_Tag
{
   uint8_t direction;
   uint8_t a_attribute_id[2];
}__attribute__((__packed__))Read_Reporting_Configurartion_Command_t;

typedef struct Reporter_Device_Record_Tag
{
   uint8_t status;
   uint8_t direction;
   uint8_t a_attribute_id[2];
   uint8_t attribute_data_type;
   uint8_t a_min_reporting_interval[2];
   uint8_t a_max_reporting_interval[2];
   uint8_t reportable_change[1];
}__attribute__((__packed__))Reporter_Device_Record_t;

typedef struct Sink_Device_Record_Tag
{
   uint8_t status;
   uint8_t direction;
   uint8_t a_attribute_id[2];
   uint8_t a_time_out_period[2];
}__attribute__((__packed__))Sink_Device_Record_t;

typedef struct Read_Reporting_Config_Resp_Command_Tag
{
   uint8_t num_of_attrib_config_records;
   union Read_Report_Config_Record
   {
      Reporter_Device_Record_t   reporter_device_record;
      Sink_Device_Record_t       sink_device_record;
   }Read_Report_Config_Record;
}__attribute__((__packed__))Read_Reporting_Config_Resp_Command_t;

/* ZCl general command  structure
The below structure gives the general commands as defined by the ZCL Foundation
specification
Field: command_identifier - gives the command id supported by ZCL foundation
Field: read_attribute_command - gives the fields of read attribute command
Field: write_attribute_command - gives the fields of write attribute command
Field: discover_attribute_command -
            gives the fields of discover attribute command
Field: report_attributes_command -
            gives the fields of report attributes command
Field: configure_report_attributes_command -
            gives the fields of configure report attribute command
*/

typedef struct ZCL_Command_Tag
{
   uint8_t command_identifier;
   union Foundation_Commands
   {
      Read_Attribute_Command_Request_t   read_attribute_command;
      Write_Attribute_Command_t          write_attribute_command;
      Discover_Attribute_Command_t       discover_attribute_command;
      Report_Attributes_Command_t        report_attributes_command;
      Configure_Report_Attributes_Command_t configure_report_attributes_command;
      Read_Reporting_Configurartion_Command_t  a_attribute_records[1];
   }Foundation_Commands;
}__attribute__((__packed__))ZCL_Command_t;


/* stucture for ZCL response commands
Field: Command Identifier - gives the command id
Field: Command type - indicates the generic command or cluster specific
   command
Field: manufacture specific - indicates if manufacture specific is set
Field: manufacture code - manufacture code
Field: Actual payload */
typedef struct ZCL_Response_Command_Tag
{
   uint8_t command_identifier;
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacture_code[2];
   union
   {
      Read_Attribute_Response_Command_t  read_attribute_response_command;
      Write_Attribute_Response_Command_t  write_attribute_response_command;
      Discover_Attribute_ResponseCommand_t discover_attribute_response_command;
      Default_ResponseCommand_t              default_response_command;
      Read_Reporting_Config_Resp_Command_t read_report_response_cmd;
   }ZCL_Response;
}__attribute__((__packed__))ZCL_Response_Command_t;


typedef struct App_ZCL_Request_Tag
{
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   ZCL_Command_t ZCL_command_received;
}__attribute__((__packed__))App_ZCL_Request_t;



/* Foundation_Commands_Request_Tag
Command type - indicates if the command is generic command or cluster specific
               command.
manufacture_specific - indicates if the command is manufacture specific or not
                        true - indicates the command is manufacture specific
                        false - indicates the command is not manufacture specific
a_manufacturer_code - gives the appropriate manufacture code if
                           manufacture_specific byte is TRUE
                        dummy manufacture code
                           if manufacture_specific byte is false
disable_default_response - indicates if the default response is enabled or disabled
                           TRUE - disabled
                           FALSE - enabled
command_identifier - consists of the command to be created
Foundation_Commands_t - consists of the union of Foundation commands
*/
typedef struct Foundation_Commands_Request_Tag
{
   uint8_t command_type;
   uint8_t manufacture_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
   union Foundation_Commands_Tag
   {
      Read_Attribute_Command_Request_t   read_attribute_command;
      Write_Attribute_Command_t          write_attribute_command;
      Discover_Attribute_Command_t       discover_attribute_command;
      Report_Attributes_Command_t        report_attributes_command;
      Configure_Report_Attributes_Command_t configure_report_attributes_command;
      Read_Reporting_Configurartion_Command_t  a_attribute_records[1];
   }Foundation_Commands_t;
}__attribute__((__packed__))Foundation_Commands_Request_Tag;


uint8_t ZCL_Create_Command
(
   uint8_t direction,
   uint8_t *p_asdu,
   void* p_ZCL_Cmd_Data,
   uint8_t ZCL_Cmd_Data_Length,
   uint8_t trans_seq_num
);

void ZCL_Reset ( ZCL_Device_Def_t const *p_device_def );

uint8_t ZCL_Get_Data_length ( uint8_t attribute_data_type );

uint8_t ZCL_HandleReadAttributeCommand
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);

uint8_t ZCL_HandleDiscoverAttributesCommand
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    const ZCL_Cluster_Info_t *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);

uint8_t ZCL_HandleWriteAttributeCommand
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);

uint8_t ZCL_CreateDefaultResponseCommand
(
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    uint8_t status
);

uint8_t CreateZCLHeaderForGenericResponseCommand
(
    uint8_t   commandId,
    uint8_t*  pReceivedAsdu,
    uint8_t*  pResponseAsduStartLocation,
    uint8_t*  preceivedCommandId
);

uint16_t ZCL_Get_AttributeDataOffset
(
   uint16_t numberOfattributes,
   uint16_t attributeId,
   ZCL_Attribute_Info_t const *p_Attribute_Info
);

/*-----------------------------------------------------------------------------
* ZCL_Get_AttributeInfo_Index
*------------------------------------------------------------------------------
*
* Return Value : Returns success or unsupported attribute id
*
* Input Parameters:
*  count_of_attribute_ids - gives the count of attribute ids supported
*  received_attribute_id - attribute id received over the air
*
*  p_data - gives the attributes that are to be read - in the asdu.
*     The received indication is held in this pointer
*  p_Attribute_Info - holds the attribute info
* Output Parameters:
*  p_index - index to which the attribute id belongs
*
* Scope of the function:
*  This function is called within the module
*
* Purpose of the function:
*	This function checks if the attribute ids match and returns success
*  if found by updating the index else returns unsupported attribute
*  and the index is invalid
*
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Get_AttributeInfo_Index
(
   uint16_t numberOfattributes,
   uint16_t attributeId,
   uint16_t *p_index,
   ZCL_Attribute_Info_t const *p_Attribute_Info
);

uint8_t ZCL_Process_Message(    uint8_t endPointId,
                                uint8_t  asduLength,
                                uint8_t* pReceivedAsdu,
                                uint8_t* pResponseAsduStartLocation,
                                ZCL_ActionHandler_t* pActionHandler,
                                const App_Cluster_Def_t* pZCLClusterDefinition );
#endif                                 /*_ZCL_FOUNDATION_H_*/
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

