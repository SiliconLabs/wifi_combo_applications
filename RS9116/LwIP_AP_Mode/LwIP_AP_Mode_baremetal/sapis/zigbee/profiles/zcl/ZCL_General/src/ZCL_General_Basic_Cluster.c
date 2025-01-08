/*******************************************************************************
* ZCL_General_Basic_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Basic cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Basic_Cluster.h"

#define m_BASIC_CMD_PAYLOAD_LENGTH_c                           0x00

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Basic_Server_Cluster_Attribute_Info [] =
{
    {
      g_ZCL_VERSION_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c | g_Reportable_Attribute_c
    },

#if ( g_APPLICATION_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_APPLICATION_VERSION_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_APPLICATION_VERSION_ATTRIBUTE_ENABLE_d */

#if ( g_STACK_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_STACK_VERSION_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_STACK_VERSION_ATTRIBUTE_ENABLE_d */

#if ( g_HW_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_HW_VERSION_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_HW_VERSION_ATTRIBUTE_ENABLE_d */

#if ( g_MANUFACTURE_NAME_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_MANUFACTURE_NAME_ATTRIBUTE_c,
      g_Character_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_MANUFACTURE_NAME_ATTRIBUTE_ENABLE_d */

#if ( g_MODEL_IDENTIFIER_ATTRIBUTE_ENABLE_d == 1 )
  {
      g_MODEL_IDENTIFIER_ATTRIBUTE_c,
      g_Character_String_Data_c,
      g_Read_Only_Attribute_c
  },
#endif                          /* g_MODEL_IDENTIFIER_ATTRIBUTE_ENABLE_d */

#if ( g_DATE_CODE_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_DATE_CODE_ATTRIBUTE_c,
      g_Character_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_DATE_CODE_ATTRIBUTE_ENABLE_d */

    {
      g_POWER_SOURCE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c
    },

#if ( g_LOCATION_DESCRIPTION_ATTRIBUTE_ENABLE_d == 1 )
 {
      g_LOCATION_DESCRIPTION_ATTRIBUTE_c,
      g_Character_String_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_LOCATION_DESCRIPTION_ATTRIBUTE_ENABLE_d */

#if ( g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d */

#if ( g_DEVICE_ENABLED_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_DEVICE_ENABLED_ATTRIBUTE_c,
      g_Boolean_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_DEVICE_ENABLED_ATTRIBUTE_ENABLE_d */

#if ( g_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_ALARM_MASK_ATTRIBUTE_c,
      g_Eight_Bit_Bitmap_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_ALARM_MASK_ATTRIBUTE_ENABLE_d */

#if ( g_DISABLE_LOCAL_CONFIG_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_DISABLE_LOCAL_CONFIG_ATTRIBUTE_c,
      g_Eight_Bit_Bitmap_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_DISABLE_LOCAL_CONFIG_ATTRIBUTE_ENABLE_d */

};

const ZCL_Cluster_Info_t ga_Basic_Server_Cluster_Info =
{
   g_BASIC_CLUSTER_c,
   sizeof(ma_Basic_Server_Cluster_Attribute_Info)/sizeof(ZCL_Attribute_Info_t),
   ma_Basic_Server_Cluster_Attribute_Info,
};

#endif          /*  ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_BASIC_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Basic_Client_Cluster_Info =
{
   g_BASIC_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /*  ( g_BASIC_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Basic_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
    uint8_t responseLength = g_ZERO_DATA_LENGTH_c;
	pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;
	if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c )
	{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
		if ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Reset_To_Factory_Defaults_Command_c )
		{
			/* Update the Event Details. This event is handled by the
			application.*/
		
			pActionHandler->event.eventId = g_Reset_To_Factory_Defaults_Events_c ;
			pActionHandler->action = Received_Response;
			if ( Is_Default_Response_Required(pReceivedAsdu)) {
					responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, g_ZCL_Success_c );
					pActionHandler->action = Send_Response;
			}   			
	
		}
#endif
	}
    return responseLength;
}

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                          /* g_BASIC_CLUSTER_ENABLE_d */
