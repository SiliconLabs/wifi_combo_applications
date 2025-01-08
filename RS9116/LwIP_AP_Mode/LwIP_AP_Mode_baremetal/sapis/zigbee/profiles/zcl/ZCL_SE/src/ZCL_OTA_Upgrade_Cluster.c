/*******************************************************************************
* ZCL_OTA_Upgrade_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting OTA cluster
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "memory_utility.h"
#if ( g_OTA_BOOTLOAD_CLUSTER_ENABLE_d == 1 )
#include "ZCL_OTA_Upgrade_Cluster.h"

/********************************************************************************
* Global Constants
*********************************************************************************/


/********************************************************************************
* Public Memory declarations
*********************************************************************************/
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_OTA_Upgrade_Client_Attribute [] =
{
    {  g_UPGRADE_SERVER_ID_c,
       g_IEEE_Address_Data_c,
       g_Read_Only_Attribute_c  },

#if ( g_FILE_OFFSET_ATTRIBUTE_ENABLE_d == 1 )
    {  g_FILE_OFFSET_c,
       g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c  },
#endif

#if ( g_CURRENT_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    {  g_CURRENT_FILE_VERSION_c,
       g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c  },
#endif

#if ( g_CURRENT_ZIGBEESTACK_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    {  g_CURRENT_ZIGBEESTACK_VERSION_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c  },
#endif

#if ( g_DOWNLOADED_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    {  g_DOWNLOADED_FILE_VERSION_c,
       g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c  },
#endif

#if ( g_DOWNLOADED_ZIGBEESTACK_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    {  g_DOWNLOADED_ZIGBEESTACK_VERSION_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c  },
#endif

    {  g_IMAGE_UPGRADE_STATUS_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Only_Attribute_c  },

#if ( g_MANUFACTURER_ID_ATTRIBUTE_ENABLE_d == 1 )
    {  g_MANUFACTURER_ID_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c  },
#endif

#if ( g_IMAGE_TYPE_ID_ATTRIBUTE_ENABLE_d == 1 )
    {  g_IMAGE_TYPE_ID_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c  }
#endif

};
#endif				/* g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d */

#if ( g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1 )
 /* Cluster Info for Server Cluster */
const ZCL_Cluster_Info_t ma_OTA_Upgrade_Server_Cluster_Info =
{
	g_OTA_UPGRADE_CLUSTER_c,
	g_NULL_c,
	g_NULL_c
};
#endif				/* g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d */

#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
 /* Cluster Info for OTA Client Cluster */
const ZCL_Cluster_Info_t ma_OTA_Upgrade_Client_Cluster_Info =
{
	g_OTA_UPGRADE_CLUSTER_c,
	sizeof(ma_OTA_Upgrade_Client_Attribute)/sizeof(ZCL_Attribute_Info_t),
	ma_OTA_Upgrade_Client_Attribute
};
#endif				/* g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d */

/********************************************************************************
* Public Functions
*********************************************************************************/

uint8_t ZCL_HandleOTAUpgradeClusterCmd ( uint8_t endPointId,
                                         uint8_t asduLength,
                                         uint8_t* pReceivedAsdu,
                                         uint8_t* pResponseLocation,
                                         ZCL_ActionHandler_t *pActionHandler,
                                         ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                         uint8_t * pAttrsDataBaseLocation )
{
    pActionHandler->event.eventLength = asduLength - 0x03;
    uint8_t cmdId = pReceivedAsdu[ g_ZCL_HEADER_COMMAND_OFFSET_c ];
    pActionHandler->action = No_Response;

    if( ZCL_GetDirectionOfCommand(pReceivedAsdu) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) {

#if ( g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1 )
        switch( cmdId )
        {
        case g_QueryNextImageRequest_c:
        case g_ImageBlockRequest_c:
        case g_ImagePageRequest_c:
        case g_UpgradeEndRequest_c:
        case g_QuerySpecificFileRequest_c:
            pActionHandler->event.eventId = g_ImageNotify_Event_c + cmdId;
            break;
        default:
            pActionHandler->action = Send_Response;
            return g_ZERO_DATA_LENGTH_c;
        }
        memUtils_memCopy( pActionHandler->event.pEventData, &pReceivedAsdu[0x03],
                  pActionHandler->event.eventLength );

#endif  /* g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d */
    }
    else {

#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
        switch( cmdId )
        {
            case g_ImageNotify_c:
            case g_QueryNextImageResponse_c:
            case g_ImageBlockResponse_c:
            case g_UpgradeEndResponse_c:
            case g_QuerySpecificFileResponse_c:
                pActionHandler->event.eventId = g_ImageNotify_Event_c + cmdId;
                break;
            default:
                pActionHandler->action = Send_Response;
                return g_ZERO_DATA_LENGTH_c;
        }
        memUtils_memCopy( pActionHandler->event.pEventData, &pReceivedAsdu[0x03],
                  pActionHandler->event.eventLength );
#endif  /* g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d */
    }
    return g_ZERO_DATA_LENGTH_c;
}

/********************************************************************************
* Interrupt Service Routines
*********************************************************************************/

/* None */

/********************************************************************************
* End Of File
*********************************************************************************/
#endif                           /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */


