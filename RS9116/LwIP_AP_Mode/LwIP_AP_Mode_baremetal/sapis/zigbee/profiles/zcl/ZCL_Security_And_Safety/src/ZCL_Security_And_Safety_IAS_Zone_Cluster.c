/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "stack_common.h"
#include "common_includes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "stack_util.h"
#include "ZCL_Stack_Utility.h"
#include "stack_common.h"
#include "memory_utility.h"

#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_ZONE_ENROLL_RESPONSE_CMD_PAYLOAD_LENGTH_c			   0x02
#define m_ZONE_STATUS_CHANGE_NOTIFICATION_CMD_PAYLOAD_LENGTH_c 0x03
#define m_ZONE_ENROLL_REQUEST_CMD_PAYLOAD_LENGTH_c			   0x04

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d == 1 )

/* Server Cluster Info for IAS Zone Cluster*/
const ZCL_Attribute_Info_t ma_IAS_Zone_Attribute_Info[] =
{
   /*attribute info for zone state*/
   {
      /*attribute id*/
      g_IAS_ZONE_INFO_ZONE_STATE_ATTRIBUTE_c,
      /*attribute data type*/
      g_Eight_Bit_Enumeration_Data_c,
      /*access permission*/
      g_Read_Only_Attribute_c,
   },
   /*attribute info for zone type*/
   {
      /*attribute id*/
      g_IAS_ZONE_INFO_ZONE_TYPE_ATTRIBUTE_c,
      /*attribute data type*/
      g_Sixteen_Bit_Enumeration_Data_c,
      /*access permission*/
      g_Read_Only_Attribute_c,

   },
   /*attribute info for zone status*/
   {
      /*attribute id*/
      g_IAS_ZONE_INFO_ZONE_STATUS_ATTRIBUTE_c,
      /*attribute data type*/
      g_Sixteen_Bit_Bitmap_Data_c,
      /*access permission*/
      g_Read_Only_Attribute_c,

   },
   /*attribute info for IAS CIE Address*/
   {
      /*attribute id*/
      g_IAS_ZONE_SETTINGS_IAS_CIE_ADDRESS_ATTRIBUTE_c,
      /*attribute data type*/
      g_IEEE_Address_Data_c,
      /*access permission*/
      g_Read_Write_Attribute_c,
   }
};

/*Cluster Info for IAS Zone Cluster*/
const ZCL_Cluster_Info_t ga_IAS_Zone_Server_Cluster_Info =
{
   /*cluster id*/
   g_IAS_ZONE_CLUSTER_c,
   /*number of attributes*/
   sizeof(ma_IAS_Zone_Attribute_Info)/sizeof(ZCL_Attribute_Info_t),
   /*pointer to list of attribute info*/
   ma_IAS_Zone_Attribute_Info
};
#endif
#if ( g_IAS_ZONE_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_IAS_Zone_Client_Cluster_Info =
{
   g_IAS_ZONE_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_IAS_Zone_Cluster_Cmd (
								uint8_t endPointId,
								uint8_t asduLength,
								uint8_t* pReceivedAsdu,
								uint8_t* pResponseAsduStartLocation,
								ZCL_ActionHandler_t *pActionHandler,
								ZCL_Cluster_Info_t const *pZCLClusterInfo,
								uint8_t * pAttrsDataBaseLocation ){

	uint8_t responseAsduLength = g_ZERO_c;
#if ( g_IAS_ZONE_CLIENT_CLUSTER_ENABLE_d == 1 )

	uint8_t status = g_ZCL_Success_c;
	uint8_t * pIeeeaddr;
    uint8_t index = m_INVALID_ZONE_INDEX_c;
#endif
	/* Get the command id from the received frame */

	if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {
#if ( g_IAS_ZONE_CLIENT_CLUSTER_ENABLE_d == 1 )

		switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
		{
			case g_Zone_Status_Change_Notification_Command_c:
				pActionHandler->event.eventId = g_Zone_Status_Change_Notification_Command_Event_c;
				pActionHandler->action = Received_Response;
				pActionHandler->event.eventLength = m_ZONE_STATUS_CHANGE_NOTIFICATION_CMD_PAYLOAD_LENGTH_c;
				/*copy the data into the event data*/
				memUtils_memCopy( pActionHandler->event.pEventData,
							  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							  m_ZONE_STATUS_CHANGE_NOTIFICATION_CMD_PAYLOAD_LENGTH_c );

				if ( Is_Default_Response_Required(pReceivedAsdu)) {
//					responseAsduLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
//															   pResponseAsduStartLocation, status );
					/* The default response frame's header consists of generic command
					   server to client, default response is disabled.Default response command
					   will not be generated in response to reception of another
					   default response command. */
					*( pResponseAsduStartLocation + responseAsduLength ) =
						( g_ZCL_FC_PROFILE_WIDE_COMMAND_c | g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
						g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c );

					responseAsduLength++;

					/* Update the Transaction Sequence number Field in the Response Frame
					which needs  to be transmitted*/
					*( pResponseAsduStartLocation + responseAsduLength ) = *( pReceivedAsdu + responseAsduLength );

					responseAsduLength++;

					/* Update the CommandId Field in the Response Frame which needs  to be
					   transmitted */
					*( pResponseAsduStartLocation + responseAsduLength ) = g_Default_Response_Command_c;

					responseAsduLength++;

					/* update the received command id,for which default response command is
					   created */
					*( pResponseAsduStartLocation + responseAsduLength ) =
						pReceivedAsdu[ ( GetZCLHeaderLength( pReceivedAsdu ) ) - 0x01 ];
					responseAsduLength++;
					/* update the status value in the default response command */
					*( pResponseAsduStartLocation + responseAsduLength ) = status;
					responseAsduLength++;

					pActionHandler->action = Send_Response;
					return responseAsduLength;
				}
				break;

			case g_Zone_Enroll_Request_Command_c:
				pActionHandler->event.eventId = g_Zone_Enroll_Request_Command_Event_c;
				pActionHandler->event.eventLength = m_ZONE_ENROLL_REQUEST_CMD_PAYLOAD_LENGTH_c;
				pActionHandler->action = Send_Response;
				/*copy the data into the event data*/
				memUtils_memCopy( pActionHandler->event.pEventData,
						  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
						  m_ZONE_ENROLL_REQUEST_CMD_PAYLOAD_LENGTH_c );

				/*check for validation of Zone type received in the enroll
				request command
				pass the zone type as the parameter*/
				status = ZCL_Callback_Zone_Check_For_Valid_Zone_Type( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c+1);

				/*if the status is success, check if Zone address is configured
				 with the zone table*/
				 if( g_Enroll_Response_Success_c == status ) {

					if ( 0x03 != ZCL_Callback_Get_Device_AddressMode()) {

					  ZCL_Callback_Backup_Zone_Enroll_Resp_Data(pReceivedAsdu);

					   /*generate the IEEE address, If the device is other
					   than CIE, then this function is replaced with a dummy
					   function*/
					   ZCL_IAS_Zone_Create_IEEE_Addr_Request (ZCL_Callback_Get_Device_ShortAddress());
					   return g_ZCL_Success_c;
					}
					else {
						pIeeeaddr = ZCL_Callback_Get_Device_IEEEAddress();
						status = ZCL_Callback_Zone_Validate_IEEE_Address (pIeeeaddr , &index,
							(pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c+1));
					}
				 }


				/* Create the ZCL Header for Enroll  response command*/
				*( pResponseAsduStartLocation + responseAsduLength ) =
						g_Cluster_Specific_Cmd_c | g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
											g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

				responseAsduLength++;
				/* Update the TSN field in the ZCL Header. TSN field should be same as in
				   the received frame */
				*( pResponseAsduStartLocation + responseAsduLength ) =
									*( pReceivedAsdu + responseAsduLength );

				responseAsduLength++;

				*( pResponseAsduStartLocation + responseAsduLength ) =
										g_Zone_Enroll_Response_Command_c;
				responseAsduLength++;
				*( pResponseAsduStartLocation + responseAsduLength ) = status;
				responseAsduLength ++;
				*( pResponseAsduStartLocation + responseAsduLength ) = index;
				responseAsduLength ++;
				break;
			default:
				return responseAsduLength;
		}
#endif  /*g_IAS_ZONE_CLIENT_CLUSTER_ENABLE_d */
	}
	else {
#if ( g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d == 1 )
	if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ==
										g_Zone_Enroll_Response_Command_c ){
		pActionHandler->event.eventId = g_Zone_Enroll_Response_Event_c;
		pActionHandler->event.eventLength = m_ZONE_ENROLL_RESPONSE_CMD_PAYLOAD_LENGTH_c;
		/*copy the data into the event data*/
		memUtils_memCopy( pActionHandler->event.pEventData,
					  pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
					  m_ZONE_ENROLL_RESPONSE_CMD_PAYLOAD_LENGTH_c );
		pActionHandler->action = Received_Response;

	}
#endif	/*g_IAS_ZONE_SERVER_CLUSTER_ENABLE_d */
	return g_ZCL_Success_c;
	}
	return responseAsduLength;
}

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

#endif

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
