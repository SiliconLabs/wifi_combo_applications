 /*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Scenes_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_FIRST_SCENE_EVENT_ID_c   								0x15
#define m_FIRST_SCENE_CMD_RESP_EVENT_ID_c   					0x1C
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
/* Attribute Info list for the attributes supported by the Scenes Cluster */
const ZCL_Attribute_Info_t ma_Scenes_Cluster_Server_Attribute_Info [] =
{
   {
      g_SCENE_MGMT_INFO_SCENE_COUNT_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
   },
   {
      g_SCENE_MGMT_INFO_CURRENT_SCENE_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c

   },
   {
      g_SCENE_MGMT_INFO_CURRENT_GROUP_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c

   },
   {
      g_SCENE_MGMT_INFO_SCENE_VALID_ATTRIBUTE_c,
      g_Boolean_Data_c,
      g_Read_Only_Attribute_c
   },
   {
      g_SCENE_MGMT_INFO_NAME_SUPPORT_ATTRIBUTE_c,
      g_Eight_Bit_Bitmap_Data_c,
      g_Read_Only_Attribute_c

   },
#if(g_LAST_CONFIGURED_BY_ATTRIBUTE_ENABLE_d == 1)
   {
      g_SCENE_MGMT_INFO_LAST_CONFIGURED_BY_ATTRIBUTE_c,
      g_IEEE_Address_Data_c,
      g_Read_Only_Attribute_c
   }
#endif
};


const ZCL_Cluster_Info_t ga_Scenes_Server_Cluster_Info =
{
   g_SCENES_CLUSTER_c,
   sizeof(ma_Scenes_Cluster_Server_Attribute_Info)/sizeof(ZCL_Attribute_Info_t) ,
   ma_Scenes_Cluster_Server_Attribute_Info
};
#endif          /* ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Scenes_Client_Cluster_Info =
{
   g_SCENES_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Scenes_Cluster_Cmd
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
)
{
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
        uint8_t respPayloadLength = 0x00;
#endif		
    uint8_t cmdDataOffset;
	uint8_t responseAsduLength = 0x00;
	uint8_t commandId;
    /* For Most of the scenes cluster commands needs to send response.
    In this case  action is Initialized with Send_Response */
    pActionHandler->action = Send_Response;
    /* responseAsduLength is initialiaed to Zero because this value will be varied
    for different commands of an scenes cluster */

    cmdDataOffset = GetZCLHeaderLength( pReceivedAsdu );

	/* Get the command id from the received frame */
	commandId =pReceivedAsdu[cmdDataOffset-0x01];
    if ( ZCL_GetDirectionOfCommand ( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c )
    {
#if ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 )

#if ( g_ZLL_SCENES_CLIENT_CLUSTER_ENABLE_d  == 1)
        if ( ( commandId <= g_Store_Scene_Response_Command_c ) ||
             ( commandId == g_Scene_Membership_Response_Command_c) ||
             ((commandId >= g_Enhanced_Add_Scene_Response_Command_c) ||
              (commandId <= g_Copy_Scene_Response_Command_c))) {
#else
        if ( ( commandId <= g_Store_Scene_Response_Command_c ) ||
             ( commandId == g_Scene_Membership_Response_Command_c ) ) {

#endif/*g_ZLL_SCENES_CLIENT_CLUSTER_ENABLE_d*/
            pActionHandler->action = Received_Response;
        }

#endif         /* ( g_SCENES_CLIENT_CLUSTER_ENABLE_d == 1 ) */
	}
    else {

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )

   		 pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;


		/* Check if the valid scene command is received. If not, do nothing */
		if ( ( commandId <= g_Scene_Membership_Command_c )
#if(g_ZLL_SCENES_SERVER_CLUSTER_ENABLE_d == 1)
		|| ( commandId == g_Enhanded_Add_Scene_Command_c )
            || ( commandId == g_Enhanced_View_Scene_Command_c ) || ( commandId == g_Copy_Scene_Command_c )
#endif /* g_ZLL_SCENES_SERVER_CLUSTER_ENABLE_d*/

			)
		{
			/*Update the Event Details. This event is handled by the application.*/
			pActionHandler->event.eventId = m_FIRST_SCENE_EVENT_ID_c + commandId ;

            memUtils_memCopy( pActionHandler->event.pEventData, &pReceivedAsdu[ cmdDataOffset ],
                      asduLength - cmdDataOffset );

			/* Create the ZCL Header for scenes response command*/
			/* Update the Frame Control Field in the scenes response
			   Frame which needs to be transmitted*/
			*( pResponseAsduStartLocation + responseAsduLength ) =
			        *pReceivedAsdu | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
			                            g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

			responseAsduLength++;

			/* Update the TSN field in the ZCL Header. TSN field should be same as in
               the received frame */
			*( pResponseAsduStartLocation + responseAsduLength ) =
			                    *( pReceivedAsdu + responseAsduLength );

			responseAsduLength++;

			/*Updating the commandId field in the ZCL Header*/
			*( pResponseAsduStartLocation + responseAsduLength ) = commandId;
			responseAsduLength++;

	        /* Handle the received gruop commands on the server side. subtract the header
			   length from the asdu lengh and pass only the payload length to the handle
			   scene command */
			respPayloadLength = ZCL_Callback_HandleSceneCmds
								(
									endPointId,
									asduLength - 3,
									pReceivedAsdu,
									pResponseAsduStartLocation + responseAsduLength
								);

			/* Check if the response is required for particular command or not */
			if ( g_ZERO_c == respPayloadLength )
			{
				pActionHandler->action = No_Response;
			}
			if( Is_Default_Response_Required( pReceivedAsdu ) )
            {
				/* Check if the command received is Recall Scene. If so, send the default response */
				if ( g_Recall_Scene_Command_c == commandId )
            	{
	                responseAsduLength = ZCL_CreateDefaultResponseCommand
	                                     (
	                                        pReceivedAsdu,
	                                        pResponseAsduStartLocation,
	                                        *( pResponseAsduStartLocation + responseAsduLength )
	                                     );
					pActionHandler->action = Send_Response;
            	}
			}
			responseAsduLength += respPayloadLength;
		}
#endif      /* ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 ) */
    }
    return responseAsduLength;
}


#endif          /* ( g_SCENES_CLUSTER_ENABLE_d == 1 ) */

/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

