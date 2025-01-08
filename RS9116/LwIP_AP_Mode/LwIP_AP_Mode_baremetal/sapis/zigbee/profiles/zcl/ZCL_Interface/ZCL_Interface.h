#ifndef _ZCL_INTERFACE_H_
#define _ZCL_INTERFACE_H_

#include "common_includes.h"
#include "ZCL_Functionality.h"
/*****************************************************************************
  Constants and Enumerations
*****************************************************************************/
typedef enum
{
    No_Response                                 = 0x00,
    Send_Response                               = 0x01,
    Received_Response                           = 0x02
}acion;

typedef enum CommandType
{
    g_GenericCommand_c                          = 0x00,
    g_ClusterSpecificCommand_c                  = 0x01
}CommandType;


typedef enum
{
   g_Min_Value_Compared_c,
   g_Max_Value_Compared_c
}Compare_Values;

typedef enum
{
     Alarm_Inactive   = 0x00,
     Alarm_Active     = 0x01,
     Alarm_Clear      = 0xFF
}Alarm_State;

typedef enum
{
    g_Write_Attribute_Data_c,
    g_Read_Attribute_Data_c,
    g_Validate_Write_Attribute_c
}Opaeration_Mode;

/*Enumerations for ZCL status*/
typedef enum ZCL_Status
{
   g_ZCL_Success_c,
   g_ZCL_Failure_c,
   g_ZCL_Not_Authorized_c = 0x7E,
   g_ZCL_Malformed_Command_c = 0x80,
   g_ZCL_UnSup_Cluster_Command_c,
   g_ZCL_UnSup_General_Command_c,
   g_ZCL_UnSup_Manuf_Cluster_Command_c,
   g_ZCL_UnSup_Manuf_General_Command_c,
   g_ZCL_Invalid_Field_c,
   g_ZCL_Unsupported_Attribute_c,
   g_ZCL_Invalid_Value_c,
   g_ZCL_Read_Only_c,
   g_ZCL_Insufficient_Space_c,
   g_ZCL_Duplicate_Exists_c,
   g_ZCL_Not_Found_c,
   g_ZCL_UnReportable_Attribute_c,
   g_ZCL_Invalid_Data_type_c,
   g_ZCL_Abort_c = 0x95,
   g_ZCL_Invalid_Image_c,
   g_ZCL_Wait_For_Data_c,
   g_ZCL_No_Image_Available_c,
   g_ZCL_Require_More_Image_c,
   g_ZCL_Hardware_Failure_c = 0xC0,
   g_ZCL_Software_Failure_c,
   g_ZCL_Calibration_Error_c
}ZCL_Status;

typedef enum  Attribute_Data_Type
{
   g_No_Data_c,
   g_Eight_Bit_Data_c = 0x08,
   g_Sixteen_Bit_Data_c,
   g_Twenty_Four_Bit_Data_c,
   g_Thirty_Two_Bit_Data_c,
   g_Boolean_Data_c = 0x10,
   g_Eight_Bit_Bitmap_Data_c = 0x18,
   g_Sixteen_Bit_Bitmap_Data_c,
   g_Twenty_Four_Bit_Bitmap_Data_c,
   g_Thirty_Two_Bit_Bitmap_Data_c,
   g_Forty_Bit_Bitmap_Data_c,
   g_Forty_Eight_Bit_Bitmap_Data_c,
   g_Fifty_Six_Bit_Bitmap_Data_c,
   g_Sixty_Four_Bit_Bitmap_Data_c,
   g_Unsigned_Eight_Bit_Integer_Data_c = 0x20,
   g_Unsigned_Sixteen_Bit_Integer_Data_c,
   g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
   g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
   g_unsigned_Fourty_Bit_Integer_Data_c,
   g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
   g_Unsigned_Fifty_Six_Bit_Integer_Data_c,
   g_Unsigned_Sixty_Four_Bit_Integer_Data_c,
   g_Signed_Eight_Bit_Integer_Data_c = 0x28,
   g_Signed_Sixteen_Bit_Integer_Data_c,
   g_Signed_Twenty_Four_Bit_Integer_Data_c,
   g_Signed_Thirty_Two_Bit_Integer_Data_c,
   g_Signed_Fourty_Bit_Integer_Data_c,
   g_Signed_Fourty_Eight_Bit_Integer_Data_c,
   g_Signed_Fifty_Six_Bit_Integer_Data_c,
   g_Signed_Sixty_Four_Bit_Integer_Data_c,
   g_Eight_Bit_Enumeration_Data_c = 0x30,
   g_Sixteen_Bit_Enumeration_Data_c,
#if(g_HA_1_2_d == 1)
   g_Thirty_Two_Bit_Enumeration_Data_c,
#endif
   g_Semi_Precision_Data_c = 0x38,
   g_Single_Precision_Data_c,
   g_Double_Presicion_Data_c,
   g_Octet_String_Data_c = 0x41,
   g_Character_String_Data_c,
   g_Long_Octet_String_Data_c,
   g_Long_Character_String_Data_c,
   g_Time_Of_Day_Data_c = 0xE0,
   g_Date_Data_c,
   g_UTC_Time_Data_c,
   g_Cluster_Id_Data_c = 0xE8,
   g_Attribute_Id_Data_c,
   g_BACNet_OID_Data_c,
   g_IEEE_Address_Data_c = 0xF0,
   g_One_Twenty_Eight_Bit_Sec_Key_c = 0xF1,
   g_Short_Octet_String_Data_c = 0xF2,
   g_Octet_Two_Byte_String_Data_c = 0xF3,
   g_Unknown_Data_c = 0xFF
}Attribute_Data_Type;

typedef enum Generic_Commands
{
   g_Read_Attribute_Command_c,
   g_Read_Attribute_Response_Command_c,
   g_Write_Attributes_Command_c,
   g_Write_Attributes_Undivided_Command_c,
   g_Write_Attributes_Response_Command_c,
   g_Write_Attributes_No_Response_Command_c,
   g_Configure_Reporting_Command_c,
   g_Configure_Reporting_Response_Command_c,
   g_Read_Reporting_Configuration_Command_c,
   g_Read_Reporting_Configuration_Response_Command_c,
   g_Report_Attributes_Command_c,
   g_Default_Response_Command_c,
   g_Discover_Attributes_Command_c,
   g_Discover_Attributes_Response_Command_c
}Generic_Commands;


/* enums for frame ctrl fields */
typedef enum Frame_Control
{
  g_Generic_Cmd_c,
  g_Cluster_Specific_Cmd_c,
  g_Manufacture_Specific_Bit_c   = 0x04,
  g_Disable_Default_Response_c   = 0x10
}Frame_Control;

typedef enum Frame_Control_Direction
{
   g_Client_To_Server_c = 0x00,
   g_Server_To_Client_c = 0x08
}Frame_Control_Direction;


typedef struct ZCL_Event_Tag
{
   uint8_t eventId;
   uint8_t eventLength;
   uint8_t *pEventData;
} __attribute__((__packed__))ZCL_Event_t;

/*************************************************************
 Overview: This Structure is used to define the Action.
**************************************************************/
/*-----------------------------------------------------------------------------
Address
	union holding both short address and extended address
Field: short_address - two byte address of device
Field: IEEE_address -
		64 bit address of device
-----------------------------------------------------------------------------*/
typedef union address_tag
{
   uint16_t short_address;
   uint8_t IEEE_address[8];
} address_t;

/*-----------------------------------------------------------------------------
ZCL_Attrib_Info - Each attribute supported by the cluster Ids are stored in
the code in the below format.
Field a_attribute_id - 16 bit unique Identifier of the attribute - specified as
                        array of 2
Field data_type - length of the Attribute value corresponding to the
                  attribute id
Field access_permission - Indicates if the attribute is read only or writeable
                  and reportable
Field data_offset - offset of the attribute id where the data is stored in the
                  Application space
Field a_default_value - default value of the attribute id.
-----------------------------------------------------------------------------*/
typedef struct  ZCL_Attribute_Info_Tag
{
   uint16_t a_attribute_id;
   uint8_t data_type;
   uint8_t access_permission;
} __attribute__((__packed__))ZCL_Attribute_Info_t;

/*-----------------------------------------------------------------------------
ZCL_Attrib_Len_Lookup_Table
This data structure is used for managing a lookup table to map the data type
ID with the corresponding data length.
Field: attribute_datatype_id will hold the data type defined by ZCL Foundation
Field: data_length will hold the length of the data corresponding to the
         attribute data type
-----------------------------------------------------------------------------*/
typedef struct ZCL_Attrib_Len_Lookup_Table_Tag
{
   uint8_t attribute_datatype_id;
   uint8_t data_length;
}__attribute__((__packed__))ZCL_Attrib_Len_Lookup_Table;

/*-----------------------------------------------------------------------------
ZCL_Cluster_Info_t
This structure is used by ZCL to define a cluster.

Field: cluster_id will hold Identifier of the particular cluster.
Field: num_attrib_info_present will hold information about the number of
       attributes available for the cluster.
Field: p_array_of_attrib_info will point to an array of elements of type
       ZCL_Attrib_Info.
-----------------------------------------------------------------------------*/
typedef  struct  ZCL_Cluster_Info_Tag
{
   uint16_t cluster_id;
   uint16_t num_attrib_info_present;
   ZCL_Attribute_Info_t const* p_array_of_attrib_info;
}__attribute__((__packed__)) ZCL_Cluster_Info_t;

typedef struct ZCL_ActionHandler_Tag
{
    uint8_t action;
    ZCL_Event_t event;
}__attribute__((__packed__))ZCL_ActionHandler_t;

/*-----------------------------------------------------------------------------
This is a type definition for the function pointer that gets called when the
 ZCL receives a cluster specific ZCL command. These handlers are defined by ZCL
-----------------------------------------------------------------------------*/

typedef uint8_t (*ClusterSpecificFuncptr )(
                         uint8_t endPointId,
                         uint8_t asduLength,
                         uint8_t* pReceivedAsdu,
                         uint8_t* pResponseAsduStartLocation,
                         ZCL_ActionHandler_t *pActionHandler,
                         ZCL_Cluster_Info_t const *pZCLClusterInfo,
                         uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
App_Cluster_Def_t
This structure is used to store the information about a particular cluster
required by the application.
Field: p_cluster_info holds the address of a ZCL_Cluster_Info_t defined in the
         respective function domains of ZCL.
Field: cluster_func will hold the address of the function defined for handling
         the cluster.
         This parameter is expected to be provided by the application developed
         because ,
         1. A manufacturer can specify his cluster definition and provide his
            function pointer.
         2.If a ZCL domain is NOT used by the application, linker will exclude
           the respective code resulting in a smaller binary.
Field: p_attrib_data_list points to the memory location where cluster specific
       attribute data is held.
-----------------------------------------------------------------------------*/
typedef struct  App_Cluster_Def_Tag
{
   ZCL_Cluster_Info_t const *p_cluster_info;
   ClusterSpecificFuncptr cluster_func;
   void * p_attrib_data_list;
   void const * p_attrib_default_data_list;
} __attribute__((__packed__))App_Cluster_Def_t;

/*-----------------------------------------------------------------------------
ZCL_Header_t
   This structure Denotes the ZCL frame header.
Field: frameControl - frame control of the header.
Field: transactionSeq - gives the transaction sequence number.
Field: commandId - command id.
-----------------------------------------------------------------------------*/
typedef struct ZCL_Header_Def_Tag
{
    uint8_t frameControl;
    uint8_t transactionSeq;
    uint8_t commandId;
}__attribute__((__packed__))ZCL_Header_t;

/*-----------------------------------------------------------------------------
ZCL_Device_Def_t_t
   This structure is used by an application to define its functionality.
   This includes the definition of all the clusters supported by the
   application in the form of App_Cluster_Def_t structure.
Field: device_id - gives the device identifier for which the device definition
         is written.
Field: num_clust_defs_present gives the number of clusters defined by the
            Application.
Field: p_list_of_cluster_def_ptrs holds the address of the list of pointers
         of type App_Cluster_Def_t.
-----------------------------------------------------------------------------*/
typedef struct ZCL_Device_Def_Tag
{
   uint8_t endpoint;
   uint8_t num_clust_defs_present;
   App_Cluster_Def_t const *p_list_of_cluster_def_ptrs;
}__attribute__((__packed__))ZCL_Device_Def_t;

/*-----------------------------------------------------------------------------*/

uint8_t ZCL_Read_Write_Attribute_Data
(
   uint8_t mode,
   uint8_t *p_attribute_data,
   ZCL_Cluster_Info_t const *p_cluster_info,
   uint16_t attribute_id,
   uint8_t *p_data
);

#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
/*******************************************************************************
* ZCL_Callback_Is_Device_In_Identifing
********************************************************************************
*   Return Value:
*   bool
*
*   Parameters:
*   Input parameter:
*   None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function has been implemented to find the device is in
*	identify mode or not
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/

uint8_t ZCL_Callback_Is_Device_In_Identifing(void);
#endif


/*******************************************************************************
* ZCL_Callback_Get_Device_EndPoint
********************************************************************************
*   Return Value:
*   end point value
*
*   Parameters:
*   Input parameter:
*   None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function has been implemented to find the device's
*	end point id
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint8_t ZCL_Callback_Get_Device_EndPoint( void );


/*******************************************************************************
* ZCL_Callback_GetMaxAPSPayloadLength
********************************************************************************
*   Return Value:
*   Max payload len
*
*   Parameters:
*   Input parameter:
*   None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function has been implemented to find the device's
*	end point id
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint8_t ZCL_Callback_GetMaxAPSPayloadLength( void );


/*******************************************************************************
* ZCL_Callback_Add_Reporting_Table_Entry
********************************************************************************
*   Return Value: status
*
*   Parameters:
*   Input parameter:
*   direction -> Reporter of Receiver of the report
*   attributeId -> Attribute Id
*   attrDataType -> Data type of attribute
*   minReportingInterval -> Maximum Reporting Interval
*   maxReportingInterval -> Manimum Reporting Interval
*   timeOut -> Reporting Interval timeout value
*   ReportableChange -> how much value of attribute change report to be sent
*   clusterId -> Report cluster Id
*   pattrDataLocation -> Attribute data location
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function has been implemented to Adding report Entry
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/

uint8_t ZCL_Callback_Add_Reporting_Table_Entry
(
    uint8_t direction,
    uint16_t attributeId,
    uint8_t attrDataType,
    uint16_t minReportingInterval,
    uint16_t maxReportingInterval,
    uint16_t timeOut,
    uint8_t* ReportableChange,
    uint16_t clusterId,
    uint8_t * pattrDataLocation
);

/*******************************************************************************
* ZCL_Callback_Add_Reporting_Table_Entry
********************************************************************************
*   Return Value: status
*
*   Parameters:
*   Input parameter:
*   direction -> Reporter of Receiver of the report
*   clusterId -> Report cluster Id
*   pattrDataLocation -> Attribute data location
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function has been implemented to Adding report Entry
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/

void ZCL_Callback_ReadReportRecord( uint8_t direction,
                                    uint16_t clusterId,
                                    uint16_t attributeId,
                                    uint8_t *pResponseAsdu,
                                    uint8_t* pResponseDataLength,
                                    ZCL_Cluster_Info_t const *pZCLClusterInfo );


/*******************************************************************************
* ZCL_Callback_Check_Reportable_Attributes
********************************************************************************
*   Return Value: status
*
*   Parameters:
*   Input parameter:
*   direction -> Reporter of Receiver of the report
*   clusterId -> Report cluster Id
*   pattrDataLocation -> Attribute data location
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function has been implemented to Adding report Entry
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/

uint8_t ZCL_Callback_Check_Reportable_Attributes
(
	uint8_t report_after_timer_expiry,	
	uint16_t cluster_id,
	uint8_t *p_attribute_id,
	uint8_t *p_data
);

/*******************************************************************************
* ZCL_Callback_HandleSceneCmds
********************************************************************************
*   Return Value: status
*
*   Parameters:
*   Input parameter:
*   direction -> Reporter of Receiver of the report
*   clusterId -> Report cluster Id
*   pattrDataLocation -> Attribute data location
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function has been implemented to Adding report Entry
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint8_t ZCL_Callback_HandleSceneCmds
(
    uint8_t endPointId,
	uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponsePayloadStartLocation
);

/*******************************************************************************
* ZCL_Callback_Get_Device_IEEEAddress
********************************************************************************
*   Return Value: pointer to IEEE Address
*
*   Parameters: None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function get the Ieee address of device that sent command
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/

uint8_t* ZCL_Callback_Get_Device_IEEEAddress( void );

/*******************************************************************************
* ZCL_Callback_DeleteScenesTableEntry
********************************************************************************
*   Return Value: None
*
*   Input Parameters:
*   endPointId - Endpoint
*   groupId - Group scene entry to remove
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function deletes scenes of this group
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/

void ZCL_Callback_DeleteScenesTableEntry ( uint8_t endPointId, uint16_t groupId );


/*******************************************************************************
* ZCL_CallBackCreateAlarmResponse
********************************************************************************
*   Return Value: None
*
*   Input Parameters:
*   endPointId - Endpoint
*   pResponseAsdu - Pointer to where response to create
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function creates the Alarm response
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/

uint8_t ZCL_CallBackCreateAlarmResponse( uint8_t endPoint, uint8_t *pResponseAsdu );



/*******************************************************************************
* ZCL_CallBackCreateAlarmResponse
********************************************************************************
*   Return Value: None
*
*   Input Parameters:
*   endPointId - Endpoint
*   attributeId - Attribute for Alarm condition
*   pZCLClusterInfo -  pointer to Cluster Info
*   callFrom - function called from
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function creates the Alarm response
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
void ZCL_Callback_CheckAlarmCondition (  uint8_t endpoint,
                                         uint16_t attributeId,
                                         const ZCL_Cluster_Info_t *pZCLClusterInfo,
                                         uint8_t callFrom );
#endif
/*******************************************************************************
* ZCL_Callback_Get_Device_ShortAddress
********************************************************************************
*   Return Value: Short Address
*
*   Parameters: None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function get the Short address of device that sent command
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint16_t ZCL_Callback_Get_Device_ShortAddress( void );

/*******************************************************************************
* ZCL_Callback_Get_Device_AddressMode
********************************************************************************
*   Return Value:  Address Mode
*
*   Parameters: None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function get the address Mode of device that sent command
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint8_t ZCL_Callback_Get_Device_AddressMode( void );

/*******************************************************************************
* ZCL_Callback_Get_Device_ShortAddress
********************************************************************************
*   Return Value: Short Address
*
*   Parameters: None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function get the Short address of device that sent command
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint16_t ZCL_Callback_Get_Device_ShortAddress( void );

/*******************************************************************************
* ZCL_Callback_Get_Device_AddressMode
********************************************************************************
*   Return Value:  Address Mode
*
*   Parameters: None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function get the address Mode of device that sent command
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint8_t ZCL_Callback_Get_Device_AddressMode( void );

/*-----------------------------------------------------------------------------
* ZCL_Callback_Zone_Validate_IEEE_Address
*------------------------------------------------------------------------------
*
* Return Value : Returns success or no enroll permit
*                 failure
*	
* Input Parameters:
*  p_IEEE_address - 64 - bit IEEE address
*
* Output Parameters:
*  p_zone_index - index of the zone table
*
* Scope of the function:
*  This function is called within the module
*
* Purpose of the function:
*	This function loops through the table maintained for zone table. If the
*  IEEE address received as the input parameter is configured in the
*  zone table, it returns success else returns no enroll permit. If the
*  validation is successful, the give zone type is updated in the entry
*  corresponding to the IEEE address in the zone table.
*   	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Callback_Zone_Validate_IEEE_Address (  uint8_t *p_IEEE_address,
									  		uint8_t *p_zone_index,
									    		uint8_t *p_zone_type );
/*-----------------------------------------------------------------------------
* ZCL_Callback_Zone_Check_For_Valid_Zone_Type
*------------------------------------------------------------------------------
*
* Return Value : Returns the if zone type is found as success else
*                 failure
*	
* Input Parameters:
*  p_zone_type - 16 bit zone type for the zone devices
*
* Output Parameters:
*  None
*
* Scope of the function:
*  This function is called within the module
*
* Purpose of the function:
*	This function loops through the table maintained for the zone types to
*  see if there are any zone types that are matching, if matches then
*  returns successful else returns zcl failure.
*   	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Callback_Zone_Check_For_Valid_Zone_Type ( uint8_t *p_zone_type);

/*-----------------------------------------------------------------------------
* ZCL_Callback_Get_Zone_Information
*------------------------------------------------------------------------------
*
* Return Value :
*		returns the length of the payload for Get zone information command(mode)
* Input Parameters:
*	None
* Output Parameters:
*  p_data - where the zone id map response, zone information response payload
*           is written
*
* Scope of the function:
*  This is called within the module
*
* Purpose of the function:
*	This function creates updates the zone id map or zone information response
*  payload based on the mode.if the command is zone map, the return value is
*  ignored
*     	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Callback_Get_Zone_Information (  uint8_t *p_data,uint8_t *p_respData ,uint8_t mode );
/*******************************************************************************
* ZCL_Callback_Backup_Zone_Enroll_Resp_Data
********************************************************************************
*   Return Value:  None
*
*   Parameters: None
*
*   Output parameter:
*   p_payload
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function to update the zone enroll response data
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
void ZCL_Callback_Backup_Zone_Enroll_Resp_Data( uint8_t *p_payload);

/*******************************************************************************
* ZCL_Callback_Get_Device_Src_EndPoint
********************************************************************************
*   Return Value:  None
*
*   Parameters: None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function get the device source endpoint
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint8_t ZCL_Callback_Get_Device_Src_EndPoint( void );

/*******************************************************************************
* ZCL_Callback_Get_ZoneId_Map_Information
********************************************************************************
*   Return Value:  None
*
*   Parameters: None
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*   This function get the Zone information
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
void ZCL_Callback_Get_ZoneId_Map_Information( uint8_t *p_zone_Id_Map_Section ,
										      uint8_t *p_respData,uint8_t length);

uint8_t ZCL_IAS_Zone_Validate_IEEE_Address ( uint8_t *p_IEEE_address,
									         uint8_t *p_zone_index,
									         uint8_t *p_zone_type );

uint32_t ZCL_CallBackGetCurrentTime( void );


uint8_t* ZCL_GetTimeAttribueReadRsp( uint8_t*ReceivedZCLData, uint8_t ZCLDataLength );

void ZCL_SetTimeAttribute( uint8_t* apptimeclusterData, uint32_t time );

uint8_t ZCL_callbackHandlerServerTunnelData( uint16_t tunnelId,
          uint8_t *tunneldataLength, uint8_t *tunneldata, uint8_t* zclHeader );
uint8_t ZCL_callbackHandlerClientTunnelData( uint16_t tunnelId,
          uint8_t* tunneldataLength,uint8_t *tunneldata, uint8_t* zclHeader );

void ZCL_callbackSetRTCtime( uint32_t utctime );

#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )

void ZCL_Callback_ProtocolAddressChanged (  uint8_t endpoint,
                                         uint16_t attributeId,
                                         const ZCL_Cluster_Info_t *pZCLClusterInfo
                                           );
#endif
#endif

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                             /* _ZCL_INTERFACE_H_ */

