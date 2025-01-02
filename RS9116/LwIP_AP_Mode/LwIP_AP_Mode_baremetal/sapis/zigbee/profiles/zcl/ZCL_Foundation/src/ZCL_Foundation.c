/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include "ZigBeeDataTypes.h"
#include "common_includes.h"
#include "ZCL_Common.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "stack_util.h"
#include "memory_utility.h"
#include "ZCL_Foundation_Reporting.h"

#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"
#endif /* #if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )*/

#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Pump_Config_And_Control_Cluster.h"
#endif /* #if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )*/

#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Thermostat_Cluster.h"
#endif /* #if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )*/


#include "ZCL_General_Alarms_Cluster.h"
#include "ZCL_General_Device_Temp_Config_Cluster.h"
#include "ZCL_General_Power_Config_Cluster.h"

#include "ZCL_General_Scenes_Cluster.h"
#include "ZCL_Interface.h"

#include "ZCL_General_Time_Cluster.h"
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#endif

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_SINGLE_BYTE_DATA_c                                    0x01
#define m_TWO_BYTE_DATA_c                                       0x02
#define m_THREE_BYTE_DATA_c                                     0x03
#define m_FOUR_BYTE_DATA_c                                      0x04
#define m_FIVE_BYTE_DATA_c                                      0x05
#define m_SIX_BYTE_DATA_c                                       0x06
#define m_SEVEN_BYTE_DATA_c                                     0x07
#define m_EIGHT_BYTE_DATA_c                                     0x08
#define m_INVALID_INDEX_c                                       0xFF
#define m_MASK_MSB_IN_INTEGER_c                                 0x00FF
#define m_IAS_ZONE_STATE_ATTRIBUTE_c                            0x0000
#define m_DISC_ATTRIBUTE_RESP_REC_LEN_c                         0x03
#define m_SET_DIR_AS_CLIENT_TO_SERVER_c                         0xF7
#define MASK_FOR_DIRECTION_BIT_IN_APP_ZCL_HEADER_INFO           0x80
#define NON_MSP_CLUSTER_DISCOVER_ATTRIBUTE_REQ_LENGTH           0x06
#define ATTRIBUTE_DATA_TYPE_LOCATION                            0x02
#define LENGTH_OF_ATTRIBUTE_ID_AND_DATA_TYPE                    0x03
#define INVALID_INTERVAL                                        0xFFFF
#define m_SIXTEEN_BYTE_DATA_c                                   0x10
#define m_MAX_BYTES_TO_BE_COPIED_c								0x00C8


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

ZCL_Cluster_Info_t *mp_cluster_info;
uint8_t * mp_attribute_data;

/*Look Up table to match Attribute data types with data length*/
const ZCL_Attrib_Len_Lookup_Table Attribute_Length_Look_Up_Table[] =
{
   { g_No_Data_c,                                   g_ZERO_DATA_LENGTH_c },
   { g_Eight_Bit_Data_c,                            m_SINGLE_BYTE_DATA_c },
   { g_Sixteen_Bit_Data_c,                          m_TWO_BYTE_DATA_c },
   { g_Twenty_Four_Bit_Data_c,                      m_THREE_BYTE_DATA_c },
   { g_Thirty_Two_Bit_Data_c,                       m_FOUR_BYTE_DATA_c },
   { g_Boolean_Data_c ,                             m_SINGLE_BYTE_DATA_c },
   { g_Eight_Bit_Bitmap_Data_c,                     m_SINGLE_BYTE_DATA_c },
   { g_Sixteen_Bit_Bitmap_Data_c,                   m_TWO_BYTE_DATA_c },
   { g_Twenty_Four_Bit_Bitmap_Data_c,               m_THREE_BYTE_DATA_c },
   { g_Thirty_Two_Bit_Bitmap_Data_c,                m_FOUR_BYTE_DATA_c },
   { g_Unsigned_Eight_Bit_Integer_Data_c ,          m_SINGLE_BYTE_DATA_c },
   { g_Unsigned_Sixteen_Bit_Integer_Data_c,         m_TWO_BYTE_DATA_c },
   { g_Unsigned_Twenty_Four_Bit_Integer_Data_c,     m_THREE_BYTE_DATA_c },
   { g_Unsigned_Thirty_Two_Bit_Integer_Data_c,      m_FOUR_BYTE_DATA_c },
   { g_unsigned_Fourty_Bit_Integer_Data_c,			m_FIVE_BYTE_DATA_c },
   { g_unsigned_Fourty_Eight_Bit_Integer_Data_c,	m_SIX_BYTE_DATA_c },
   { g_Unsigned_Fifty_Six_Bit_Integer_Data_c,		m_SEVEN_BYTE_DATA_c },
   { g_Unsigned_Sixty_Four_Bit_Integer_Data_c,		m_EIGHT_BYTE_DATA_c},
   { g_Signed_Eight_Bit_Integer_Data_c ,            m_SINGLE_BYTE_DATA_c },
   { g_Signed_Sixteen_Bit_Integer_Data_c,           m_TWO_BYTE_DATA_c },
   { g_Signed_Twenty_Four_Bit_Integer_Data_c,       m_THREE_BYTE_DATA_c },
   { g_Signed_Thirty_Two_Bit_Integer_Data_c,        m_FOUR_BYTE_DATA_c },
   { g_Signed_Fourty_Bit_Integer_Data_c,			m_FIVE_BYTE_DATA_c },
   { g_Signed_Fourty_Eight_Bit_Integer_Data_c,		m_SIX_BYTE_DATA_c },
   { g_Signed_Fifty_Six_Bit_Integer_Data_c,			m_SEVEN_BYTE_DATA_c },
   { g_Signed_Sixty_Four_Bit_Integer_Data_c,		m_EIGHT_BYTE_DATA_c },
   { g_Eight_Bit_Enumeration_Data_c ,               m_SINGLE_BYTE_DATA_c },
   { g_Sixteen_Bit_Enumeration_Data_c,              m_TWO_BYTE_DATA_c },
#if(g_HA_1_2_d == 1)
   { g_Thirty_Two_Bit_Enumeration_Data_c,           m_FOUR_BYTE_DATA_c},
#endif
   { g_Semi_Precision_Data_c ,                      m_TWO_BYTE_DATA_c },
   { g_Single_Precision_Data_c,                     m_FOUR_BYTE_DATA_c },
   { g_Double_Presicion_Data_c,                     m_EIGHT_BYTE_DATA_c },
   { g_Octet_String_Data_c,                         g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c },
   { g_Character_String_Data_c ,                    g_LENGTH_FOR_CHAR_STRING_DATA_TYPE_c },
   { g_Long_Octet_String_Data_c,					g_LENGTH_FOR_LONG_OCTET_STRING_DATA_TYPE_c },
   { g_Long_Character_String_Data_c,				g_LENGTH_FOR_LONG_CHAR_STRING_DATA_TYPE_c },
   { g_Time_Of_Day_Data_c ,                         m_FOUR_BYTE_DATA_c },
   { g_Date_Data_c,                                 m_FOUR_BYTE_DATA_c },
   { g_UTC_Time_Data_c,								m_FOUR_BYTE_DATA_c },
   { g_Cluster_Id_Data_c,                           m_TWO_BYTE_DATA_c },
   { g_Attribute_Id_Data_c,                         m_TWO_BYTE_DATA_c },
   { g_BACNet_OID_Data_c,                           m_FOUR_BYTE_DATA_c },
   { g_IEEE_Address_Data_c ,                        m_EIGHT_BYTE_DATA_c },
   { g_One_Twenty_Eight_Bit_Sec_Key_c,              m_SIXTEEN_BYTE_DATA_c},
   { g_Short_Octet_String_Data_c,                   m_SIX_BYTE_DATA_c },
   { g_Octet_Two_Byte_String_Data_c,                m_TWO_BYTE_DATA_c },
   { g_Unknown_Data_c ,                             g_ZERO_DATA_LENGTH_c }
};

enum
{
   m_Group_Address_Mode_c = 0x01
};

typedef enum
{
   g_Discovery_Not_Complete_c,
   g_Discovery_Complete_c
}Discovery;

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct ZCL_Header_And_Payload_Tag
{
   uint8_t frame_control;
   uint8_t a_manufacture_code[2];
   uint8_t transaction_sequence_number;
   ZCL_Command_t ZCL_Command;
}__attribute__((__packed__))ZCL_Header_And_Payload_t;


/*-----------------------------------------------------------------------------
Discover_Attribute_Tag
Field: attribute_id - gives the 16 bit attribute id
Field: attribute_data_type - gives the attribute data type corresponding to
               the attribute id
-----------------------------------------------------------------------------*/
typedef struct Discover_Attribute_Tag
{
   uint16_t attribute_id;
   uint8_t attribute_data_type;
}__attribute__((__packed__))Discover_Attribute_t;


/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
The following function pointer is used by ZCL to parse the Generic command
Handlers.
endPointId -This is the endpoint for which the generic command is received
asduLength - This gives the asdu length of the received ZCL frame.
pReceivedAsu - Pointer to the received ZCL header and payload.
pResponseAsduStartLocation - Pointer to where the Response ZCL header and
                             payload should be updated. It gives the starting
                             location.
pZCLClusterInfo - This is the pointer holding the cluster info pointer.
-----------------------------------------------------------------------------*/
typedef uint8_t ( *pGenericFuncPtr) ( uint8_t endPointId,
                                      uint8_t asduLength,
                                      uint8_t *pReceivedAsu,
                                      uint8_t *pResponseAsduStartLocation,
                                      ZCL_ActionHandler_t *pActionHandler,
                                      ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                      uint8_t * pAttrsDatastartLocation );

/*----------------------------------------------------------------------------
  This structure holds the commandId and Corresponding function call.
  commandId - This is 8-bit integer. This holds the generic command id.
  function  - This is the function to handle a particular command.
              This is specific to the command id.
-----------------------------------------------------------------------------*/
typedef struct GenericCommandFunctionPointer_Tag
{
    uint8_t commandId;
    pGenericFuncPtr function;
} __attribute__((__packed__))GenericCommand_FunctionPointer_t;

/******************************************************************************
        Variable Definations
******************************************************************************/
/* This look up table holds command id,corresponding handler */
const GenericCommand_FunctionPointer_t aGenericCmdHandlerLookUpTable[] =
{
    { g_Read_Attribute_Command_c, ZCL_HandleReadAttributeCommand },
    { g_Write_Attributes_Command_c, ZCL_HandleWriteAttributeCommand },
    { g_Write_Attributes_Undivided_Command_c, ZCL_HandleWriteAttributeCommand },
    { g_Write_Attributes_No_Response_Command_c, ZCL_HandleWriteAttributeCommand },
    { g_Discover_Attributes_Command_c, ZCL_HandleDiscoverAttributesCommand },
#if ( g_REPORTING_ENABLE_d == 1 )
    { g_Configure_Reporting_Command_c , ZCL_HandleConfigureReportingCommand},
    { g_Read_Reporting_Configuration_Command_c , ZCL_HandleReadReportingConfigurationCommand }
#endif
};

static uint8_t genericResponseCommandTable[] =
{
   g_Read_Attribute_Response_Command_c,
   g_Write_Attributes_Response_Command_c,
#if ( g_REPORTING_ENABLE_d == 1 )
   g_Configure_Reporting_Response_Command_c,
   g_Read_Reporting_Configuration_Response_Command_c,
   g_Report_Attributes_Command_c,
#endif
   g_Discover_Attributes_Response_Command_c,
   g_Default_Response_Command_c
};

static uint8_t ZCL_Attribute_Validation
(
   ZCL_Attribute_Info_t const *p_attribute_info,
   uint8_t attribute_data_type
);


/*-----------------------------------------------------------------------------
* Write_Attribute_Data
*------------------------------------------------------------------------------
*
* Return Value : Returns true or false
*	
* Input Parameters:
*  action_to_be_taken - Indicates whether the write function is called upon
*  reciept of a arite attribute command or a local write of a reportable
*  attribute is being done.
*  p_Attribute_Data - gives the dest pointer to where data is updated
*  p_cluster_info - cluster information
*  index - where the attribute id is present in the attribute info
*     list pointer
*  data_length - length of the data to be written
*  p_data - the actual data
*
*
* Output Parameters:
*  p_events - holds the list of events
*  p_number_of_events - gives the number of events
*
* Scope of the function:
*  This function is called within the module
*
* Purpose of the function:
*	This function updates the attribute data into the given location by
*  taking the offset into consideration
*   	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Write_Attribute_Data
(
   uint16_t cluster_id,
   uint8_t *p_Attribute_Data,
   ZCL_Attribute_Info_t const *p_Attribute_Info,
   uint8_t data_length,
   uint8_t *p_data,
   bool externalWrite
);

/*-----------------------------------------------------------------------------
* ZCL_Get_Data_length
*------------------------------------------------------------------------------
*
* Return Value : Returns the data length of the given attribute data type
*	
* Input Parameters:
*  attribute_data_type - attribute data type whose data length is to be
*     returned.
*
* Output Parameters:
*  None
*
* Scope of the function:
*  This function is called within the module
*
* Purpose of the function:
*	This function loops through the look up table held for attribute data type
*  and the corresponding data length.If the data type is found returns the
*  corresponding data length else returns zero.
*   	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
static uint8_t WriteAttributeRecord (   uint8_t index,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t* responseAsduLength,
                                        uint8_t* pAttrsDataBaseLoc,
                                        uint8_t OperationMode );


/***********************************************************************************************/

uint8_t ZCL_Process_Message(    uint8_t endPointId,
                                uint8_t  asduLength,
                                uint8_t* pReceivedAsdu,
                                uint8_t* pResponseAsduStartLocation,
                                ZCL_ActionHandler_t* pActionHandler,
                                const App_Cluster_Def_t* pZCLClusterDefinition )
{
    uint8_t index;
    uint8_t commandId;
    uint8_t responseAsdulength = g_ZERO_c;
    uint8_t status = g_ZCL_Success_c;

    pActionHandler->action = Send_Response;

    if( Is_ClusterSpecificCommand( pReceivedAsdu ) ){

        if ( ! Is_Manufacture_Specific_Cmd( pReceivedAsdu ) ){
            responseAsdulength =    pZCLClusterDefinition->cluster_func ( endPointId,
                                                asduLength,
                                                pReceivedAsdu,
                                                pResponseAsduStartLocation,
                                                pActionHandler,
                                                pZCLClusterDefinition->p_cluster_info,
                                                pZCLClusterDefinition->p_attrib_data_list );
        }
        if( responseAsdulength == g_ZERO_c ) {
            status = g_ZCL_UnSup_Cluster_Command_c;
            if( Is_Manufacture_Specific_Cmd( pReceivedAsdu )  ) {
                status = g_ZCL_UnSup_Manuf_Cluster_Command_c;
            }
        }
    }
    else{
        commandId = pReceivedAsdu[ GetZCLHeaderLength(pReceivedAsdu) - 0x01 ];
        for( index = g_ZERO_c; index < sizeof( aGenericCmdHandlerLookUpTable ) /
            sizeof(GenericCommand_FunctionPointer_t); index++ ){

            if( commandId == aGenericCmdHandlerLookUpTable[index].commandId ) {

                responseAsdulength =    aGenericCmdHandlerLookUpTable[index].function (
                                            endPointId,
                                            asduLength,
                                            pReceivedAsdu,
                                            pResponseAsduStartLocation,
                                            pActionHandler,
                                            pZCLClusterDefinition->p_cluster_info,
                                            pZCLClusterDefinition->p_attrib_data_list );

                if( commandId == g_Write_Attributes_No_Response_Command_c ) {
                    pActionHandler->action = No_Response;

                    return g_ZERO_DATA_LENGTH_c;
                }
                if( g_Server_To_Client_c == ZCL_GetDirectionOfCommand( pReceivedAsdu ) ){
                    pResponseAsduStartLocation[0] &= 0xF7;
                }
                return responseAsdulength;
            }
        }
        if ( responseAsdulength == g_ZERO_c ){

            commandId = pReceivedAsdu[ ( GetZCLHeaderLength( pReceivedAsdu ) ) - 0x01 ];
            for ( index = 0; index < sizeof(genericResponseCommandTable); index++ ){
                if (commandId == genericResponseCommandTable[index] ) {
                    pActionHandler->action = Received_Response;
                    return responseAsdulength;
                }
            }
        }
        status = g_ZCL_UnSup_General_Command_c;
        if( Is_Manufacture_Specific_Cmd( pReceivedAsdu ) ) {
            status = g_ZCL_UnSup_Manuf_General_Command_c;
        }
    }
    if( ( g_ZCL_Success_c != status  ) && ( Send_Response == pActionHandler->action ) ){

        if ( Is_Default_Response_Required( pReceivedAsdu ) ) {
            responseAsdulength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                                pResponseAsduStartLocation, status );
        }
        else{
            pActionHandler->action = No_Response;
        }
    }
    return responseAsdulength;
}


/***********************************************************************************************/
uint8_t ZCL_HandleReadAttributeCommand (    uint8_t endPointId,
                                            uint8_t asduLength,
                                            uint8_t* pReceivedAsdu,
                                            uint8_t* pResponseAsduStartLocation,
                                            ZCL_ActionHandler_t *pActionHandler,
                                            ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                            uint8_t * pAttrsDataBaseLocation )
{
    uint8_t   responseAsduLength = g_ZERO_c;
    uint8_t   rspindex = g_ZERO_c;
    uint8_t   receivedCommandId;
    uint8_t   NumOfAttributesPresent;
    uint8_t   AttrIndexInReceivedFrame;
    uint8_t   attrDataTypeLength;
    uint16_t   attrindex;
    uint16_t  dataOffset;
    uint8_t   status;

    rspindex = CreateZCLHeaderForGenericResponseCommand ( g_Read_Attribute_Response_Command_c,
                                                pReceivedAsdu,
                                                pResponseAsduStartLocation,
                                                &receivedCommandId );

    /* Find out the Number of Attributes present in the received frame.
    It is known that AttributeId is always 2 bytes. index parameter
    gives the ZCL Header size. So, the Number of Attributes present in the
    frame can be calculated using asduLength and index, as shown below */
    NumOfAttributesPresent = ( asduLength - rspindex ) / g_ATTRIBUTE_ID_LENGTH_c ;

    /* Take a backup of start index of the Attribute List in the received frame.
    This is required when multiple attributes are present in the received frame.
    This is used as index to point to next AttributeId field in the recevied frame */
    AttrIndexInReceivedFrame = rspindex;

    while ( NumOfAttributesPresent > g_ZERO_DATA_LENGTH_c ){

        /*  Decrement the NumOfAttribtues. If this variable becomes zero, means
            that we have reached the last attribute in the received frame  */
        NumOfAttributesPresent--;

        status = ZCL_Get_AttributeInfo_Index( pZCLClusterInfo->num_attrib_info_present,
                                        *((uint16_t*)(pReceivedAsdu + AttrIndexInReceivedFrame)),
                                        &attrindex,
                                        pZCLClusterInfo->p_array_of_attrib_info );

        if ( status == g_ZCL_Success_c ){

            attrDataTypeLength = ZCL_Get_Data_length ( pZCLClusterInfo->
                                        p_array_of_attrib_info[attrindex].data_type );

            dataOffset = ZCL_Get_AttributeDataOffset( pZCLClusterInfo->num_attrib_info_present,
                           *((uint16_t*)(pReceivedAsdu + AttrIndexInReceivedFrame)),
                            pZCLClusterInfo->p_array_of_attrib_info );

            if ( ( pZCLClusterInfo->p_array_of_attrib_info[attrindex].data_type == g_Octet_String_Data_c ) ||
                ( pZCLClusterInfo->p_array_of_attrib_info[attrindex].data_type == g_Character_String_Data_c ) ) {

                    attrDataTypeLength = *( pAttrsDataBaseLocation + dataOffset ) + 0x01;
            }
            if ( ( rspindex + 0x04 + attrDataTypeLength ) < ZCL_Callback_GetMaxAPSPayloadLength() ){

                memUtils_memCopy( pResponseAsduStartLocation + rspindex, (uint8_t*)&pZCLClusterInfo->
                          p_array_of_attrib_info[attrindex].a_attribute_id, g_ATTRIBUTE_ID_LENGTH_c );

                *(pResponseAsduStartLocation + rspindex + 2) = status;

                *(pResponseAsduStartLocation + rspindex + 3 ) = pZCLClusterInfo->p_array_of_attrib_info[attrindex].data_type;

                rspindex += 0x04;

                memUtils_memCopy(   pResponseAsduStartLocation + rspindex,
                            pAttrsDataBaseLocation + dataOffset, attrDataTypeLength );

                rspindex += attrDataTypeLength;
            }
            else{
                /* Since we have reached the Maximum Payload that can be written,
                   we should stop further processing of the remaining attributes
                  in the received frame */
                break;
            }
        }
        else {
            /*  Make sure the asdu should not be more than the supported size.
                Max size that can be written is 3 bytes (AttributeId and Status).
                Check whether 3 bytes are available in the asdu */
            if ( ( rspindex + g_ATTRIBUTE_ID_DATA_TYPE_LENGTH_c ) <= ZCL_Callback_GetMaxAPSPayloadLength() ) {

                memUtils_memCopy( pResponseAsduStartLocation + rspindex,
                          pReceivedAsdu + AttrIndexInReceivedFrame, g_ATTRIBUTE_ID_LENGTH_c );
                *( pResponseAsduStartLocation + rspindex + g_ATTRIBUTE_ID_LENGTH_c ) = g_ZCL_Unsupported_Attribute_c;

                rspindex += g_ATTRIBUTE_ID_DATA_TYPE_LENGTH_c;
            }
            else {
                /* Since we have reached the Maximum Payload that can be written,
                   we should stop further processing of the remaining attributes
                  in the received frame */
                break;
            }
        }
        AttrIndexInReceivedFrame += g_ATTRIBUTE_ID_LENGTH_c;
    }
    responseAsduLength = rspindex;

    return responseAsduLength;
}

/***********************************************************************************************/

uint8_t ZCL_HandleDiscoverAttributesCommand ( uint8_t endPointId,
                                uint8_t asduLength,
                                uint8_t* pReceivedAsdu,
                                uint8_t* pResponseAsduStartLocation,
                                ZCL_ActionHandler_t *pActionHandler,
                                const ZCL_Cluster_Info_t *pZCLClusterInfo,
                                uint8_t * pAttrsDataBaseLocation )
{
    /* dataLength variable is used to create the ZCL header.The variable is
    incremented based on the discovery response frame */
    uint8_t dataLength = g_ZERO_c;
    /*variable i to loop through the number of attributes supported within
    a cluster.
    This variable is also resued to copy the manufacturer code,
    transaction sequence number from the received discovery request frame.
    It is initialized to 0x01 to point to sequence number if the command received
    is non-manufacture specific.if received frame is manufacture specific, it points
    to the manufacture code.*/
    uint16_t i = 0x01;
    /*count the number of discovered attributes , initialize it to zero and
    increment as and when the attributes are discovered*/
    uint8_t attributesDiscovered = g_ZERO_c;
    /*copy the maximum count received in the discovery request command frame*/
    uint8_t maximumCount = pReceivedAsdu[asduLength-1];
    /*hold the start attribute id received in the discover attribute request
    command*/
    uint16_t startattributeId ;

    /* create the discover attribute response command in the output pointer */
    pResponseAsduStartLocation[dataLength++] =
            g_ZCL_FC_PROFILE_WIDE_COMMAND_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
            g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c ;

    /* if length is greater than 0x06, then it indicates the discover
    attribute request command is manufacture specific, since discover attribute
    request has fixed length */
    if( asduLength > NON_MSP_CLUSTER_DISCOVER_ATTRIBUTE_REQ_LENGTH ) {
        /*copy the manufacturer code from the received request frame*/
        pResponseAsduStartLocation[dataLength++] = pReceivedAsdu[i++];
        pResponseAsduStartLocation[dataLength++] = pReceivedAsdu[i++];
    }

    /* copy the transaction sequence number from the request frame */
    pResponseAsduStartLocation[dataLength++] = pReceivedAsdu[i];

    /*write command id in the appropriate location*/
     pResponseAsduStartLocation[dataLength++] = g_Discover_Attributes_Response_Command_c;

    /* copy the start attribute id from the request frame. The 4th byte(third location)
    gives the start attribute id */

     memUtils_memCopy ( (uint8_t*)&startattributeId, (uint8_t*)&pReceivedAsdu[0x03],
                g_ZCL_ATTRIBUTE_ID_LENGTH_c );

    /* Default keep the discovery comlpete field with discovery completed
    Check if some more attributes are pending, then
    change this field to discovery incomplete */
     pResponseAsduStartLocation[ dataLength++ ] =  g_ZCL_DiscoveryInComplete_c;

     for( i = g_ZERO_c ; i < pZCLClusterInfo->num_attrib_info_present; i++ ) {
        uint16_t attributeId;

        memUtils_memCopy( (uint8_t*)&attributeId,
                  (uint8_t*)&pZCLClusterInfo->p_array_of_attrib_info[i].a_attribute_id,
                  g_ZCL_ATTRIBUTE_ID_LENGTH_c );

        /* Check if the attribute is greater than or equal to start
        attribute id, if so add into the attribute id list,
        0x03 is added for attribute id and attribute data type */
        if( attributeId >= startattributeId ) {

            memUtils_memCopy ( (uint8_t*)&pResponseAsduStartLocation[dataLength],
                    (uint8_t*)&( pZCLClusterInfo->p_array_of_attrib_info[i].a_attribute_id ),
                    g_ZCL_ATTRIBUTE_ID_LENGTH_c );

            attributesDiscovered++;
            dataLength += g_ZCL_ATTRIBUTE_ID_LENGTH_c;
            pResponseAsduStartLocation[dataLength++] =
                pZCLClusterInfo->p_array_of_attrib_info[i].data_type;
        }

        /*the last byte in the request gives the Maximum number of identifiers
        to be discovered. If the maximum attribute ids is reached , then
        stop discovering and update the discovery complete field*/
        if( attributesDiscovered == maximumCount ){
            break;
        }
        if( ( dataLength + 0x03 ) >  60 ) {
            return ( dataLength );
        }
    }
    if( attributesDiscovered > maximumCount ){
        return ( 0x04 );
    }
    /* 1. check if all attributes are discovered and the maximum count has reached
       2. maximum count, attributes discovered is zero and all the attributes are
          looped through */
    if( ( ( attributesDiscovered == maximumCount ) &&
        ( ( i == pZCLClusterInfo->num_attrib_info_present ) ||
        ( ( i + 1 ) == pZCLClusterInfo->num_attrib_info_present ) ) ) ||
        ( ( i == pZCLClusterInfo->num_attrib_info_present ) ||
        ( ( i + 1 ) == pZCLClusterInfo->num_attrib_info_present ) ) ||
        ( ( attributesDiscovered == g_ZERO_c ) && ( maximumCount == g_ZERO_c ) ) ) {

        pResponseAsduStartLocation[dataLength - (attributesDiscovered * 3) - 0x01] =  g_ZCL_DiscoveryComplete_c;
    }
    return dataLength;

}

/***********************************************************************************************/

uint8_t ZCL_HandleWriteAttributeCommand (   uint8_t endPointId,
                                            uint8_t asduLength,
                                            uint8_t* pReceivedAsdu,
                                            uint8_t* pResponseAsduStartLocation,
                                            ZCL_ActionHandler_t *pActionHandler,
                                            ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                            uint8_t * pAttrsDataBaseLocation )
{
	uint8_t responseAsduLength = g_ZERO_c;
	uint8_t index = g_ZERO_c;
	uint8_t receivedCommandId;
	uint8_t allAttributesSuccess;

	index = CreateZCLHeaderForGenericResponseCommand ( g_Write_Attributes_Response_Command_c,
                                        pReceivedAsdu,
                                        pResponseAsduStartLocation,
                                        &receivedCommandId );

    responseAsduLength = index;
	
	/*Validate the Attribute Records in the received frame. */
	allAttributesSuccess = WriteAttributeRecord ( index,
                                                  asduLength,
                                                  pReceivedAsdu,
                                                  pResponseAsduStartLocation,
                                                  pZCLClusterInfo,
                                                  &responseAsduLength,
                                                  pAttrsDataBaseLocation,
                                                  g_Validate_Write_Attribute_c );

	/* allAttributeSuccess variable will hold the status of validation.
	  If all the Attribute Records are valid, then we need update the
	  Attribute Value stored with the value specified in the frame. */
	if ( allAttributesSuccess ){
	    allAttributesSuccess = WriteAttributeRecord ( index,
                                                  asduLength,
                                                  pReceivedAsdu,
                                                  pResponseAsduStartLocation,
                                                  pZCLClusterInfo,
                                                  &responseAsduLength,
                                                  pAttrsDataBaseLocation,
                                                  g_Write_Attribute_Data_c );
		/* If all the attributes have written successfully, then only Status field shall
		   be included in the response frame */
		*( pResponseAsduStartLocation + index ) = g_ZCL_Success_c;
		index++;
		responseAsduLength = index;
	}
	else {
		/* If the one or more Attribute Records are invalid, then we should not
		 update the Attribute Value, if the received frame is Write Attribute
		 Undivided command. Otherwire, we have to update the Attribute Value
		 for all the valid Attribute Records present in the frame.*/
		if ( receivedCommandId != g_Write_Attributes_Undivided_Command_c ) {
            responseAsduLength = index;
	        allAttributesSuccess = WriteAttributeRecord ( index,
                                                  asduLength,
                                                  pReceivedAsdu,
                                                  pResponseAsduStartLocation,
                                                  pZCLClusterInfo,
                                                  &responseAsduLength,
                                                  pAttrsDataBaseLocation,
                                                  g_Write_Attribute_Data_c );
		}
	}
	return responseAsduLength;
}

/***********************************************************************************************/

static uint8_t WriteAttributeRecord (  uint8_t index, uint8_t asduLength,
                            uint8_t* pReceivedAsdu,
                            uint8_t* pResponseAsduStartLocation,
                            ZCL_Cluster_Info_t const *pZCLClusterInfo,
                            uint8_t* responseAsduLength,
                            uint8_t* pAttrsDataBaseLoc,
                            uint8_t OperationMode )
{	
	uint8_t NumOfAttributesPresent;
	uint8_t allAttributesSuccess = g_TRUE_c;
	uint8_t attribDataTypeLen;
	uint16_t attrindex;
	uint8_t Status = g_ZCL_Success_c;
    uint16_t dataOffset = 0x0000;

	NumOfAttributesPresent = index;

	while ( NumOfAttributesPresent < asduLength ) {		

        Status = ZCL_Get_AttributeInfo_Index( pZCLClusterInfo->num_attrib_info_present,
                                              *((uint16_t*)(pReceivedAsdu + index )),
                                              &attrindex,
                                              pZCLClusterInfo->p_array_of_attrib_info );

        if ( Status == g_ZCL_Success_c ) {

            Status = ZCL_Attribute_Validation ( pZCLClusterInfo->p_array_of_attrib_info + attrindex,
                                        *(pReceivedAsdu + index + ATTRIBUTE_DATA_TYPE_LOCATION) );
        }
        if ( ( *( pReceivedAsdu + index + ATTRIBUTE_DATA_TYPE_LOCATION ) == g_Octet_String_Data_c ) ||
             ( *( pReceivedAsdu + index + ATTRIBUTE_DATA_TYPE_LOCATION ) == g_Character_String_Data_c ) ) {

             attribDataTypeLen = *( pReceivedAsdu + index + LENGTH_OF_ATTRIBUTE_ID_AND_DATA_TYPE ) + 0x01;
        }
        else{
            attribDataTypeLen = ZCL_Get_Data_length (
                                      *( pReceivedAsdu + index + ATTRIBUTE_DATA_TYPE_LOCATION ) );
        }
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
        if ( Status == g_ZCL_Success_c ) {
            if ( g_THERMOSTAT_CLUSTER_c == pZCLClusterInfo->cluster_id ) {

                mp_cluster_info = (ZCL_Cluster_Info_t *)pZCLClusterInfo;
                mp_attribute_data = pAttrsDataBaseLoc;

                Status = ZCL_Check_Boundary_Values ( (uint8_t*)&(pZCLClusterInfo->
                                  p_array_of_attrib_info[attrindex].a_attribute_id),
                                  pReceivedAsdu + index + LENGTH_OF_ATTRIBUTE_ID_AND_DATA_TYPE,
                                  attribDataTypeLen );
            }
        }
#endif
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )		
        if ( Status == g_ZCL_Success_c ) {
            if ( g_SIMPLE_METERING_CLUSTER_c == pZCLClusterInfo->cluster_id ) {

                mp_cluster_info = (ZCL_Cluster_Info_t *)pZCLClusterInfo;
                mp_attribute_data = pAttrsDataBaseLoc;

                Status = ZCL_Check_Metering_Boundary_Values ( (uint8_t*)&(pZCLClusterInfo->
                                  p_array_of_attrib_info[attrindex].a_attribute_id),
                                  pReceivedAsdu + index + LENGTH_OF_ATTRIBUTE_ID_AND_DATA_TYPE,
                                  attribDataTypeLen );
            }
        }
#endif  /* #if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )*/

#if( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
        if ( Status == g_ZCL_Success_c ) {
            ZCL_Attribute_Info_t *p_attrib_info =
                (ZCL_Attribute_Info_t*)pZCLClusterInfo->p_array_of_attrib_info + attrindex;

            if ( pZCLClusterInfo-> cluster_id == g_TIME_CLUSTER_c ) {
                if ( ( p_attrib_info->a_attribute_id == g_TIME_ATTRIBUTE_c ) ||
                     ( p_attrib_info->a_attribute_id == g_TIME_STATUE_ATTRIBUTE_c ) ) {
                    dataOffset = ZCL_Get_AttributeDataOffset( pZCLClusterInfo->num_attrib_info_present,
                                                             g_TIME_STATUE_ATTRIBUTE_c,
                                                             pZCLClusterInfo->p_array_of_attrib_info );
                    if( *(pAttrsDataBaseLoc + dataOffset) & 0x01 ) {
                        Status = g_ZCL_Not_Authorized_c;
                    }
                }
                else if ( ( p_attrib_info->a_attribute_id == g_TIME_DST_START_ATTRIBUTE_c ) ||
                         ( p_attrib_info->a_attribute_id == g_TIME_DST_SHIFT_ATTRIBUTE_c ) ||
                             ( p_attrib_info->a_attribute_id == g_TIME_DST_END_ATTRIBUTE_c )   ||
                                 ( p_attrib_info->a_attribute_id == g_TIME_ZONE_ATTRIBUTE_c ) ) {

                                     if( *(pAttrsDataBaseLoc + dataOffset) & 0x04 ) {
                                         Status = g_ZCL_Not_Authorized_c;
                                     }

                                 }
            }
        }

#endif     /* ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 ) */
        if ( Status != g_ZCL_Success_c ) {

             allAttributesSuccess = g_FALSE_c;

            *( pResponseAsduStartLocation + *responseAsduLength ) = Status;
             ( *responseAsduLength )++;

            memUtils_memCopy( ( pResponseAsduStartLocation + *responseAsduLength ),
                        pReceivedAsdu + index, g_ATTRIBUTE_ID_LENGTH_c );

            *responseAsduLength = *responseAsduLength + g_ATTRIBUTE_ID_LENGTH_c;
        }
        else {
            if ( OperationMode == g_Write_Attribute_Data_c ) {

                dataOffset = ZCL_Get_AttributeDataOffset( pZCLClusterInfo->num_attrib_info_present,
                                                         *((uint16_t*)( pReceivedAsdu + index ) ),
                                                         pZCLClusterInfo->p_array_of_attrib_info );

                mp_cluster_info = ( ZCL_Cluster_Info_t *)pZCLClusterInfo;
                mp_attribute_data = pAttrsDataBaseLoc;

                Status = ZCL_Write_Attribute_Data( pZCLClusterInfo->cluster_id,
                                                  pAttrsDataBaseLoc + dataOffset,
                                                  pZCLClusterInfo->p_array_of_attrib_info + attrindex,
                                                  attribDataTypeLen,
                                                  pReceivedAsdu + index + LENGTH_OF_ATTRIBUTE_ID_AND_DATA_TYPE,
                                                  g_FALSE_c );
            }
        }
        NumOfAttributesPresent += attribDataTypeLen + LENGTH_OF_ATTRIBUTE_ID_AND_DATA_TYPE;
        index += attribDataTypeLen + LENGTH_OF_ATTRIBUTE_ID_AND_DATA_TYPE;
    }
	return allAttributesSuccess;
}

/***********************************************************************************************/

uint8_t CreateZCLHeaderForGenericResponseCommand ( uint8_t commandId,
                                    uint8_t* pReceivedAsdu,
                                    uint8_t* pResponseAsduStartLocation,
                                    uint8_t* preceivedCommandId )
{
    uint8_t    index = g_ZERO_c;

    /* For Generic Command Response, the following fields will have a fixed value.
    FrameType               = 0x00 --> Generic Profile wide command
    Direction               = 0x08 --> Direction is from Server to Client
    DefaultResponse  = 0x10 --> Default Response not requrired for Response command*/
    uint8_t    frameControl = 0x18;

    /*Update the Frame Control Field in the Response Frame which needs
    to be transmitted*/
    *( pResponseAsduStartLocation + index ) = frameControl;
    index++;

    /*Check whether Received Command is for the cluster defined in ZCL
    or Manufacturer Specific cluster.
    Is_Manufacture_Specific_Cmd - returns 0 if manufacture specific and
                                             1 if not manufacture specific */
    if( ( Is_Manufacture_Specific_Cmd( pReceivedAsdu ) ) )
    {
        /*Update the Manufacturer Specific field in Frame Control Field,
        always the frame control is the first byte(0th byte)*/
        pResponseAsduStartLocation[0] = frameControl | g_ZCL_FRAME_CONTROL_MANUFACTURE_SPECIFIC_c;

       memUtils_memCopy( pResponseAsduStartLocation + index, pReceivedAsdu + index,
                 g_SIZE_OF_MANUFACTURER_CODE_c );

        index += g_SIZE_OF_MANUFACTURER_CODE_c;
    }
    *( pResponseAsduStartLocation + index ) =  *( pReceivedAsdu + index ) ;

    index++;

    *preceivedCommandId = *( pReceivedAsdu + index );

    *( pResponseAsduStartLocation + index ) = commandId;

    index++;

    return index;
}

/***********************************************************************************************/

uint8_t ZCL_CreateDefaultResponseCommand ( uint8_t* pReceivedAsdu,
                                uint8_t* pResponseAsduStartLocation,
                                uint8_t status )
{
    uint8_t resp_index = g_ZERO_c;
    /* The default response frame's header consists of generic command
       server to client, default response is disabled.Default response command
       will not be generated in response to reception of another
       default response command. */
    if ( ZCL_GetDirectionOfCommand ( pReceivedAsdu ) ) {
        *( pResponseAsduStartLocation + resp_index ) =
        ( g_ZCL_FC_PROFILE_WIDE_COMMAND_c | g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
        g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c );
    }
    else {
        *( pResponseAsduStartLocation + resp_index ) =
        ( g_ZCL_FC_PROFILE_WIDE_COMMAND_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
        g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c );
    }
    resp_index++;
    if ( Is_Manufacture_Specific_Cmd( pReceivedAsdu ) ) {
       pResponseAsduStartLocation[0] |= g_ZCL_FRAME_CONTROL_MANUFACTURE_SPECIFIC_c;

       memUtils_memCopy( pResponseAsduStartLocation + resp_index, pReceivedAsdu + resp_index,
                 g_SIZE_OF_MANUFACTURER_CODE_c );

       resp_index += g_SIZE_OF_MANUFACTURER_CODE_c;
    }
    /* Update the Transaction Sequence number Field in the Response Frame
    which needs  to be transmitted*/
    *( pResponseAsduStartLocation + resp_index ) = *( pReceivedAsdu + resp_index );

    resp_index++;

    /* Update the CommandId Field in the Response Frame which needs  to be
       transmitted */
    *( pResponseAsduStartLocation + resp_index ) = g_Default_Response_Command_c;

    resp_index++;

    /* update the received command id,for which default response command is
       created */
    *( pResponseAsduStartLocation + resp_index ) =
        pReceivedAsdu[ ( GetZCLHeaderLength( pReceivedAsdu ) ) - 0x01 ];
    resp_index++;
    /* update the status value in the default response command */
    *( pResponseAsduStartLocation + resp_index ) = status;
    resp_index++;

    return resp_index;
}

/***********************************************************************************************/

uint16_t ZCL_Get_Attributes_length ( ZCL_Cluster_Info_t const *p_cluster_info )
{
	uint16_t index;
    uint16_t attributeslength = g_ZERO_c;

    for ( index = 0; index < p_cluster_info->num_attrib_info_present; index++ ){
          attributeslength += ZCL_Get_Data_length ( p_cluster_info->
                                                     p_array_of_attrib_info[index].data_type );
    }
    return attributeslength;

}

/***********************************************************************************************/

void ZCL_Reset ( ZCL_Device_Def_t const *p_device_def )
{
    uint8_t clusters_count = g_ZERO_c;
    uint16_t attributeslength = 0x0000;
	uint16_t offset= 0x0000;
	uint16_t max_bytes_to_be_copied = m_MAX_BYTES_TO_BE_COPIED_c;
    App_Cluster_Def_t const *p_appcluster_def = p_device_def->p_list_of_cluster_def_ptrs;

    for ( ; clusters_count < p_device_def->num_clust_defs_present; clusters_count++ ) {
            p_appcluster_def = p_device_def->p_list_of_cluster_def_ptrs + clusters_count;

        if ( ( p_appcluster_def->p_attrib_data_list != g_NULL_c ) &&
             ( p_appcluster_def->p_attrib_default_data_list != g_NULL_c ) ){

            attributeslength = ZCL_Get_Attributes_length( p_appcluster_def->p_cluster_info );
            if(attributeslength <255) {
				memUtils_memCopy( (uint8_t*)p_appcluster_def->p_attrib_data_list,
						  (uint8_t*)p_appcluster_def->p_attrib_default_data_list, attributeslength );
			}
			else{
				while(attributeslength  > g_ZERO_c) {
					memUtils_memCopy( (uint8_t*)p_appcluster_def->p_attrib_data_list+offset,
					(uint8_t*)p_appcluster_def->p_attrib_default_data_list+offset, max_bytes_to_be_copied);
					offset = offset + max_bytes_to_be_copied;
                    attributeslength = attributeslength -  max_bytes_to_be_copied;
					if(attributeslength < max_bytes_to_be_copied) {
					   max_bytes_to_be_copied = attributeslength;
					}
				}
				 offset= 0x0000;
				 max_bytes_to_be_copied = m_MAX_BYTES_TO_BE_COPIED_c;
			}
        }
    }
}

/***********************************************************************************************/
uint8_t ZCL_Create_Command
(
   uint8_t direction,
   uint8_t *p_asdu,
   void* p_ZCL_Cmd_Data,
   uint8_t ZCL_Cmd_Data_Length,
   uint8_t trans_seq_num
)
{
   App_ZCL_Request_t *p_received_data = ( App_ZCL_Request_t *)p_ZCL_Cmd_Data;
   uint8_t data_length = g_ZERO_DATA_LENGTH_c;
   bool manufacture_specific = p_received_data->manufacture_specific;
   bool disable_default_response = p_received_data->disable_default_response;
   uint8_t *p_ZCL_Header_Payload = p_asdu;

   if ( !( p_received_data->command_type & 0x01 )) {
      ((ZCL_Header_And_Payload_t*)p_ZCL_Header_Payload)->frame_control = g_Generic_Cmd_c ;
   }
   else {
      ((ZCL_Header_And_Payload_t*)p_ZCL_Header_Payload)->frame_control = g_Cluster_Specific_Cmd_c;
   }
   ((ZCL_Header_And_Payload_t*)p_ZCL_Header_Payload)->frame_control |= direction;

   data_length++;

   if ( disable_default_response ) {
      ((ZCL_Header_And_Payload_t*)p_ZCL_Header_Payload)->frame_control |= g_Disable_Default_Response_c;
   }
   if ( manufacture_specific ) {
      ((ZCL_Header_And_Payload_t*)p_ZCL_Header_Payload)->frame_control |= g_Manufacture_Specific_Bit_c ;

      memUtils_memCopy( (uint8_t*)((ZCL_Header_And_Payload_t*)p_ZCL_Header_Payload)->a_manufacture_code,
                (uint8_t*)p_received_data->a_manufacturer_code, 2 );

      data_length += sizeof(uint16_t);

      ZCL_Cmd_Data_Length -= g_ZCL_HEADER_WITHOUT_MANUF_CODE_c;
   }
   else{
       ZCL_Cmd_Data_Length -= g_ZCL_HEADER_WITH_MANUF_CODE_c;
   }
   *( p_ZCL_Header_Payload + data_length ) = trans_seq_num;

   data_length++;

   *( p_ZCL_Header_Payload +  data_length ) = p_received_data->ZCL_command_received.command_identifier;

   ZCL_Cmd_Data_Length--;
   data_length++;

   memUtils_memCopy
    (
        p_ZCL_Header_Payload + data_length,
        (uint8_t*)&( p_received_data->ZCL_command_received.Foundation_Commands ),
        ZCL_Cmd_Data_Length
    );

	data_length += ZCL_Cmd_Data_Length;

   return data_length;
}

/***********************************************************************************************/
uint8_t ZCL_Get_Data_length(   uint8_t attribute_data_type)
{
	uint8_t count = 0;
   for( ; count < sizeof(Attribute_Length_Look_Up_Table)/sizeof(ZCL_Attrib_Len_Lookup_Table); count++ )
   {
      if( attribute_data_type == Attribute_Length_Look_Up_Table[count].attribute_datatype_id )
      {
         return Attribute_Length_Look_Up_Table[count].data_length;
      }
   }
   return g_ZERO_DATA_LENGTH_c;
}

/***********************************************************************************************/

uint8_t ZCL_Get_AttributeInfo_Index ( uint16_t numberOfattributes,
                               uint16_t attributeId,
                               uint16_t *p_index,
                               ZCL_Attribute_Info_t const *p_Attribute_Info )
{

   uint16_t count;
   *p_index = m_INVALID_INDEX_c;
   for( count = g_ZERO_c ; count < numberOfattributes; count++ ){
       if ( ( g_Memory_Matches_c == memUtils_memCompare( (uint8_t*)&p_Attribute_Info->a_attribute_id,
                                                (uint8_t*)&attributeId, 2 ) ) ){
            *p_index = count;
            return g_ZCL_Success_c;
       }
       p_Attribute_Info++;
   }
   return g_ZCL_Unsupported_Attribute_c;
}

/***********************************************************************************************/

uint16_t ZCL_Get_AttributeDataOffset ( uint16_t numberOfattributes, uint16_t attributeId,
   ZCL_Attribute_Info_t const *p_Attribute_Info )
{
   uint16_t dataOffset = g_ZERO_c;
   uint16_t index;
   uint8_t attrDataTypeLength;

   for( index = g_ZERO_c ; index < numberOfattributes; index++ ){
       if ( ( g_Memory_Matches_c == memUtils_memCompare( (uint8_t*)&p_Attribute_Info->a_attribute_id,
                                                 (uint8_t*)&attributeId, 2 ) ) ){
            return dataOffset;
       }
       else{
           attrDataTypeLength = ZCL_Get_Data_length ( p_Attribute_Info->data_type );
           dataOffset += attrDataTypeLength;
       }
       p_Attribute_Info++;
   }
   return g_ZERO_c;
}

/***********************************************************************************************/

uint8_t ZCL_Write_Attribute_Data (  uint16_t cluster_id,
                                    uint8_t *p_Attribute_Data,
                                    ZCL_Attribute_Info_t const *p_Attribute_Info,
                                    uint8_t data_length,
                                    uint8_t *p_data,
                                    bool internalWrite )
{

    uint8_t status = g_ZCL_Success_c;

#if ( g_REPORTING_ENABLE_d ==  1 )
	if ((p_Attribute_Info->access_permission & g_Reportable_Attribute_c) == g_Reportable_Attribute_c) {

		ZCL_Callback_Check_Reportable_Attributes (  g_Report_After_Attribute_Change_c,            			
            			                   cluster_id,
            			                  (uint8_t*)&p_Attribute_Info->a_attribute_id,
            			                   p_data );
	}
#endif          /* ( g_REPORTING_ENABLE_d == 1 ) */
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
     if ( g_THERMOSTAT_CLUSTER_c == cluster_id ) {
         status = ZCL_Check_Boundary_Values ( (uint8_t*)&(p_Attribute_Info->a_attribute_id),
                                              p_data,
                                              data_length );
    }
#endif          /* ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
    if ( g_TIME_CLUSTER_c == cluster_id ) {
        uint16_t dataoffset = 0x0000;
        if ( p_Attribute_Info->a_attribute_id == g_TIME_ATTRIBUTE_c ) {
            if ( internalWrite == g_TRUE_c ) {
                dataoffset = ZCL_Get_AttributeDataOffset( mp_cluster_info->num_attrib_info_present,
                                                         g_TIME_STATUE_ATTRIBUTE_c,
                                                         mp_cluster_info->p_array_of_attrib_info );
                *(mp_attribute_data + dataoffset) = (*(mp_attribute_data + dataoffset)&0xFE ) | 0x01;
            }
           dataoffset = ZCL_Get_AttributeDataOffset( mp_cluster_info->num_attrib_info_present,
                                                 g_TIME_LASTSET_TIME_ATTRIBUTE_c,
                                                 mp_cluster_info->p_array_of_attrib_info );
           /* If we can write Time Attribute update the LastSetTime set same value */
           memUtils_memCopy( mp_attribute_data + dataoffset, p_data, data_length );
           ZCL_callbackSetRTCtime( *(uint32_t*)p_data );
        }
        else if ( ( p_Attribute_Info->a_attribute_id == g_TIME_DST_START_ATTRIBUTE_c ) ||
                 ( p_Attribute_Info->a_attribute_id == g_TIME_DST_SHIFT_ATTRIBUTE_c ) ||
                     ( p_Attribute_Info->a_attribute_id == g_TIME_DST_END_ATTRIBUTE_c )   ||
                         ( p_Attribute_Info->a_attribute_id == g_TIME_ZONE_ATTRIBUTE_c ) ) {
                             if ( internalWrite == g_TRUE_c ) {
                                 *(mp_attribute_data + dataoffset) |= 0x04;
                             }
                         }
        else if((p_Attribute_Info->a_attribute_id == g_TIME_STATUE_ATTRIBUTE_c) && (!internalWrite) ) {
          if ( *p_Attribute_Data & 0x01 ) {
              *p_data &= 0xFD;
          }
           *p_data = (*p_data & 0xFA ) | (*p_Attribute_Data & 0x05);
        }
    }
#endif          /* ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 ) */
    if ( g_ZCL_Invalid_Value_c != status ) {
        memUtils_memCopy ( p_Attribute_Data, p_data, data_length );
#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
        ZCL_Callback_CheckAlarmCondition ( ZCL_Callback_Get_Device_EndPoint(),
                                           p_Attribute_Info->a_attribute_id,
                                           mp_cluster_info,
                                           g_After_Attribute_Change_c );
#endif          /* ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if(g_SE_PROFILE_1_1_d == 1)		
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
        ZCL_Callback_ProtocolAddressChanged( ZCL_Callback_Get_Device_EndPoint(),
                                           p_Attribute_Info->a_attribute_id,
                                           mp_cluster_info);
#endif          /* ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 ) */		
#endif		
    }
    return status;
}

/***********************************************************************************************/
static uint8_t ZCL_Attribute_Validation
(
   ZCL_Attribute_Info_t const *p_attribute_info,
   uint8_t attribute_data_type
)
{
    if ( !( p_attribute_info->access_permission & g_Read_Write_Attribute_c ) ) {
      return g_ZCL_Read_Only_c;
    }
    else if ( attribute_data_type != p_attribute_info->data_type ) {
      return g_ZCL_Invalid_Data_type_c;
    }

   return g_ZCL_Success_c;
}

/***********************************************************************************************/

uint8_t ZCL_Read_Write_Attribute_Data (    uint8_t mode,   uint8_t *p_attribute_data,
   ZCL_Cluster_Info_t const *p_cluster_info,    uint16_t attribute_id,    uint8_t *p_data )
{
    uint16_t index;
    uint8_t status;
    uint16_t dataOffset;
    ZCL_Attribute_Info_t const *p_Attribute_Info = p_cluster_info->p_array_of_attrib_info;

	if( (p_attribute_data == g_NULL_c) || (p_data == g_NULL_c )|| ( p_cluster_info == g_NULL_c))
	{
		//status = g_ZCL_Invalid_Field_c;
		return g_ZCL_Invalid_Field_c;
	}

    status = ZCL_Get_AttributeInfo_Index (  p_cluster_info->num_attrib_info_present,
                                            attribute_id,
                                            &index,
                                            p_Attribute_Info );

    if ( m_INVALID_INDEX_c != index ) {

        dataOffset = ZCL_Get_AttributeDataOffset( p_cluster_info->num_attrib_info_present,
                           attribute_id, p_cluster_info->p_array_of_attrib_info );

        p_Attribute_Info += index;

        if ( g_Read_Attribute_Data_c == mode ) {
            memUtils_memCopy  ( p_data, p_attribute_data + dataOffset,
                        ZCL_Get_Data_length ( p_Attribute_Info->data_type ) );
        }
        else{

            mp_cluster_info = ( ZCL_Cluster_Info_t *)p_cluster_info;
            mp_attribute_data = p_attribute_data;

            status =  ZCL_Write_Attribute_Data ( p_cluster_info->cluster_id,
                                        p_attribute_data + dataOffset,
                                        p_Attribute_Info,
                                        ZCL_Get_Data_length ( p_Attribute_Info->data_type ),
                                        p_data, g_TRUE_c  );
       }
   }
   return status;
}
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
