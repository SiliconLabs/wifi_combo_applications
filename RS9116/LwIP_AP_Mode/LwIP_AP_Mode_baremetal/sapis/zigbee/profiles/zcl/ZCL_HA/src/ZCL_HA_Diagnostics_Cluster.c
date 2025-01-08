                              /*******************************************************************************
* ZCL_General_Binary_Input_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Binary Input (Basic)
* cluster
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#include "memory_utility.h"
#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HA_Diagnostics_Cluster.h"



/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_PAYLOAD_LOCATION_c                            0x03
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_DIAGNOSTICS_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Diagnostics_Attribute []=
{
#if ( g_NO_OF_RESETS_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_NO_OF_RESETS_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /*  g_NO_OF_RESETS_ATTRIBUTE_ENABLE_d */

#if ( g_PERSISTENT_MEMORY_WRITES_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_PERSISTENT_MEMORY_WRITES_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                                  /* g_PERSISTENT_MEMORY_ATTRIBUTE_ENABLE_d */

#if ( g_MAC_RX_BCAST_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_MAC_RX_BCAST_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                                  /* g_MAC_RX_BCAST_ATTRIBUTE_ENABLE_d */
#if ( g_MAC_TX_BCAST_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_MAC_TX_BCAST_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                       			 /* g_MAC_TX_BCAST_ATTRIBUTE_ENABLE_d */
#if ( g_MAC_RX_UCAST_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_MAC_RX_UCAST_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                                  /* g_MAC_RX_UCAST_ATTRIBUTE_ENABLE_d */
#if ( g_MAC_TX_UCAST_ATTRIBUTE_ENABLE_d == 1 )

    ,{
        g_MAC_TX_UCAST_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                     /* g_MAC_TX_UCAST_ATTRIBUTE_ENABLE_d */
#if ( g_MAC_TX_UCAST_RETRY_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_MAC_TX_UCAST_RETRY_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                                  /* g_MAC_TX_UCAST_RETRY_ATTRIBUTE_ENABLE_d */
#if ( g_MAC_TX_UCAST_FAIL_ATTRIBUTE_ENABLE_d == 1 )
   , {
        g_MAC_TX_UCAST_FAIL_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                /* g_MAC_TX_UCAST_FAIL_ATTRIBUTE_ENABLE_d */
#if ( g_APS_RX_BCAST_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APS_RX_BCAST_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_RX_BCAST_ATTRIBUTE_ENABLE_d */
#if ( g_APS_TX_BCAST_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APS_TX_BCAST_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_TX_BCAST_ATTRIBUTE_ENABLE_d */
#if ( g_APS_RX_UCAST_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APS_RX_UCAST_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_RX_UCAST_ATTRIBUTE_ENABLE_d */	
#if ( g_APS_TX_UCAST_SUCCESS_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APS_TX_UCAST_SUCCESS_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_TX_UCAST_SUCCESS_ATTRIBUTE_ENABLE_d */	
#if ( g_APS_TX_UCAST_RETRY_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APS_TX_UCAST_RETRY_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_TX_UCAST_RETRY_ATTRIBUTE_ENABLE_d */	
#if ( g_APS_TX_UCAST_FAIL_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APS_TX_UCAST_FAIL_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_TX_UCAST_FAIL_ATTRIBUTE_ENABLE_d */	
#if ( g_ROUTE_DISC_INITIATED_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_ROUTE_DISC_INITIATED_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_ROUTE_DISC_INITIATED_ATTRIBUTE_ENABLE_d */	
#if ( g_NEIGHBOR_ADDED_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_NEIGHBOR_ADDED_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_NEIGHBOR_ADDED_ATTRIBUTE_ENABLE_d */	
#if ( g_NEIGHBOR_REMOVED_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_NEIGHBOR_REMOVED_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_NEIGHBOR_REMOVED_ATTRIBUTE_ENABLE_d */	
#if ( g_NEIGHBOR_STALE_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_NEIGHBOR_STALE_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_NEIGHBOR_STATE_ATTRIBUTE_ENABLE_d */	
#if ( g_JOIN_INDICATION_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_JOIN_INDICATION_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_JOIN_INDICATION_ATTRIBUTE_ENABLE_d */	
#if ( g_CHILD_MOVED_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_CHILD_MOVED_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_CHILD_MOVED_ATTRIBUTE_ENABLE_d */	
#if ( g_NWKFC_FAILURE_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_NWKFC_FAILURE_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_NWKFC_FAILURE_ATTRIBUTE_ENABLE_d */	
#if ( g_APSFC_FAILURE_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APSFC_FAILURE_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APSFC_FAILURE_ATTRIBUTE_ENABLE_d */	
#if ( g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d */	

#if ( g_NWK_DECRYPT_FAILURES_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_NWK_DECRYPT_FAILURES_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d */		

#if ( g_APS_DECRYPT_FAILURES_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_APS_DECRYPT_FAILURES_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d */		

#if ( g_PACKET_BUFFER_ALLOCATE_FAILURES_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_PACKET_BUFFER_ALLOCATE_FAILURES_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d */		

#if ( g_RELAYED_UCAST_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_RELAYED_UCAST_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d */		

#if ( g_PHY_TO_MAC_QUEUE_LIMIT_REACHED_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_PHY_TO_MAC_QUEUE_LIMIT_REACHED_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d */		

#if ( g_PACKET_VALIDATE_DROP_COUNT_ATTRIBUTE_ENABLE_d == 1 )
    ,{
        g_PACKET_VALIDATE_DROP_COUNT_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d */		
	
};

/* Cluster Info for Alarms Cluster */
const ZCL_Cluster_Info_t ga_Diagnostics_Server_Cluster_Info =
{
   g_DIAGNOSTICS_CLUSTER_c,
   sizeof(ma_Diagnostics_Attribute)/sizeof(ZCL_Attribute_Info_t),
   ma_Diagnostics_Attribute
};
#endif                              /* g_DIAGNOSTICS_SERVER_CLUSTER_ENABLE_d */

#if ( g_DIAGNOSTICS_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Diagnostics_Client_Cluster_Info =
{
   g_DIAGNOSTICS_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif                              /* g_DIAGNOSTICS_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/




/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Diagnostic_Cluster_Cmd
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
    uint8_t responseLength = g_ZERO_DATA_LENGTH_c;
	
	pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;
	if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c )
	{
#if ( g_DIAGNOSTICS_SERVER_CLUSTER_ENABLE_d == 1 )
		uint8_t offset = m_PAYLOAD_LOCATION_c;
		uint8_t index;
		if ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Clear_diagnostic_attributes_Command_c )
		{
			/* Update the Event Details. This event is handled by the
			application.*/
		
		    Clear_Diagnostic_Attribute_Command_t  *pFormattedRecData =
			  (Clear_Diagnostic_Attribute_Command_t*)pActionHandler->event.pEventData;
			
			pActionHandler->event.eventId = g_Clear_Diagnostics_Attributes_Events_c ;
			pActionHandler->action = Received_Response;
			
			pFormattedRecData->attribute_count = *( pReceivedAsdu + offset );
			offset += 0x01;
			
			for(index =0;index <pFormattedRecData->attribute_count;index++){
				memUtils_memCopy( (uint8_t*)&pFormattedRecData->list_of_attributes[index],
										pReceivedAsdu + offset, 0x02 );
				offset += 0x02;
			}
				
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
#endif                                  /* g_BASIC_INPUT_CLUSIER_ENABLE_d */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
