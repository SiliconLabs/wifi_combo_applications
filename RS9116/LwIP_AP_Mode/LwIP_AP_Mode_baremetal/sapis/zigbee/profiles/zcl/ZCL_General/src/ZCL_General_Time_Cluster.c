/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "memory_utility.h"
#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Time_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_INVALID_VALUE_c                           0xFF

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/*Attribute info for Time Cluster - Time Attribute and Time Status attribute */
#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Time_Attribute_Info_List[] =
{
    {   g_TIME_ATTRIBUTE_c,
        g_UTC_Time_Data_c,
        g_Read_Write_Attribute_c
    },
    {
        g_TIME_STATUE_ATTRIBUTE_c,
        g_Eight_Bit_Bitmap_Data_c,
        g_Read_Write_Attribute_c
    },
#if ( g_TIME_ZONE_ATTRIBUTE_ENABLE_d == 1) 	
    {
        g_TIME_ZONE_ATTRIBUTE_c,
        g_Signed_Thirty_Two_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    },
#endif
#if ( g_TIME_DST_START_ATTRIBUTE_ENABLE_d == 1) 	
    {
        g_TIME_DST_START_ATTRIBUTE_c,
        g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    },
#endif
#if ( g_TIME_DST_END_ATTRIBUTE_ENABLE_d== 1) 	
    {
        g_TIME_DST_END_ATTRIBUTE_c,
        g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    },
#endif
#if ( g_TIME_DST_SHIFT_ATTRIBUTE_ENABLE_d == 1)
    {
        g_TIME_DST_SHIFT_ATTRIBUTE_c,
        g_Signed_Thirty_Two_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    },
#endif
#if ( g_TIME_STANDARD_TIME_ATTRIBUTE_ENABLE_d== 1) 	
    {
        g_TIME_STANDARD_TIME_ATTRIBUTE_c,
        g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    },
#endif	
#if ( g_TIME_LOCAL_TIME_ATTRIBUTE_ENABLE_d== 1) 	
    {
        g_TIME_LOCAL_TIME_ATTRIBUTE_c,
        g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    },
#endif
#if ( g_TIME_LASTSET_TIME_ATTRIBUTE_ENABLE_d== 1)
    {
        g_TIME_LASTSET_TIME_ATTRIBUTE_c,
        g_UTC_Time_Data_c,
        g_Read_Only_Attribute_c
    },
#endif
#if ( g_TIME_VALID_UNTIL_TIME_ATTRIBUTE_ENABLE_d== 1)
    {
        g_TIME_VALID_UNTIL_TIME_ATTRIBUTE_c,
        g_UTC_Time_Data_c,
        g_Read_Write_Attribute_c
    }
#endif
};
/* Server cluster info for Time cluster */
const ZCL_Cluster_Info_t ga_Time_Server_Cluster_Info =
{
   g_TIME_CLUSTER_c,
   sizeof(ma_Time_Attribute_Info_List)/sizeof(ZCL_Attribute_Info_t),
   ma_Time_Attribute_Info_List
};
#endif

/* Client cluster info for Time cluster */
#if ( g_TIME_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Time_Client_Cluster_Info =
{
   g_TIME_CLUSTER_c,
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

uint8_t ZCL_Handle_Time_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
    pActionHandler->action = Send_Response;
    return g_ZERO_DATA_LENGTH_c;
}



#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
/*******************************************************************************************/
void ZCL_SetTimeAttribute( uint8_t* timeclusterAttrsData, uint32_t time )
{
    uint16_t index;
    uint16_t offset;

    ZCL_Get_AttributeInfo_Index ( ga_Time_Server_Cluster_Info.num_attrib_info_present,
           g_TIME_ATTRIBUTE_c, &index, ga_Time_Server_Cluster_Info.p_array_of_attrib_info );

    if ( index == m_INVALID_VALUE_c ) {
        return;
    }
    offset = ZCL_Get_AttributeDataOffset( ga_Time_Server_Cluster_Info.num_attrib_info_present,
                                 g_TIME_ATTRIBUTE_c,
                                 ga_Time_Server_Cluster_Info.p_array_of_attrib_info );
    memUtils_memCopy( timeclusterAttrsData + offset, (uint8_t*)&time, 0x04 );
}
#endif


#if ( g_TIME_CLIENT_CLUSTER_ENABLE_d == 1 )
/******************************************************************************************/
uint8_t* ZCL_GetTimeAttribueReadRsp( uint8_t*ReceivedZCLData, uint8_t ZCLDataLength )
{
    uint8_t dataLength = GetZCLHeaderLength( ReceivedZCLData );
    uint16_t attributeId = 0x00;
    uint8_t Length = 0x00;

    while( dataLength < ZCLDataLength ) {

        memUtils_memCopy( (uint8_t *)&attributeId, ReceivedZCLData + dataLength ,
                  g_ZCL_ATTRIBUTE_ID_LENGTH_c );
        dataLength += g_ZCL_ATTRIBUTE_ID_LENGTH_c + 0x01;
        if( g_ZCL_Success_c == *( ReceivedZCLData + dataLength - 0x01 ) ) {
           Length = ZCL_Get_Data_length( *( ReceivedZCLData + dataLength ) );
           dataLength += 0x01;

           if( g_TIME_ATTRIBUTE_c == attributeId  ) {
               return ( ReceivedZCLData + dataLength );
           }
           dataLength += Length;
        }
    }
    return g_NULL_c;
}
#endif
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

#endif
