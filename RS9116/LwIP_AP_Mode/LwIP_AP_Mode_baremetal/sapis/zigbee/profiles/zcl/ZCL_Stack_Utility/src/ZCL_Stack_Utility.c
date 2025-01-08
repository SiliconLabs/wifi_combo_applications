/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "stack_functionality.h"
#include "stack_util.h"
#include "stack_common.h"
#include "zdo_interface.h"
#include "ZCL_Common.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "ZCL_Foundation_Reporting.h"
#include "buffer_management.h"
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"
#endif /* #if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )*/
#include "ZCL_Stack_Utility.h"
#include "memory_utility.h"
#include "apsde_interface.h"

uint8_t a_IEEE_Address_Found[8];
uint8_t g_Num_Of_Group_Table_Entries_c;//ADDED
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )

extern uint8_t g_Num_Of_Group_Table_Entries_c;
#endif

/* State maintained while performing IEEE address request. This is for
validation when Enroll request command is received */
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
uint8_t g_Ieee_Address_Request_For_Zone_cluster;
uint8_t g_ZDP_Transcation_Seq_Num = 0x00;
extern uint8_t g_IAS_Zone_State;
#endif

#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
	#if ( g_GROUP_TABLE_NVM_STORAGE_d == RSI_DISABLE )
		extern Group_Table_t *gp_Group_Table;
	#endif

//extern uint8_t  g_Num_Of_Group_Table_Entries_c;

uint8_t ZCL_Stack_View_Group_Request ( uint8_t *p_group_data );
#endif                                 /*g_GROUPS_SERVER_CLUSTER_ENABLE_d*/

#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
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

#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
uint8_t ZCL_Stack_Add_Group_Request ( uint8_t *p_group_data, uint8_t end_point )
{
   Add_Group_Request_t add_group_req;

   memUtils_memCopy( (uint8_t  *)&(add_group_req.group_address), (uint8_t *)p_group_data,
      sizeof(uint16_t));

   add_group_req.endpoint = end_point;

   if( g_ZCL_Not_Found_c == (ZCL_Stack_View_Group_Request ((uint8_t *)p_group_data ) ) ){
      return APS_APSMEaddGroup ( &add_group_req );
   }
   return g_ZCL_Duplicate_Exists_c;
}

/*---------------------------------------------------------------------------*/

uint8_t ZCL_Stack_Remove_Group_Request
(
   uint8_t *p_group_data,
   uint8_t end_point
)
{
   Remove_Group_Request_t remove_group_req;

   memUtils_memCopy
   ((uint8_t *)&(remove_group_req.group_address),
      (uint8_t *)p_group_data,
      sizeof(uint16_t)
   );

   remove_group_req.endpoint = end_point;

   return APS_APSMEremoveGroup
          (
             &remove_group_req
          );
}

/*---------------------------------------------------------------------------*/

uint8_t ZCL_Stack_Remove_All_Groups_Request ( uint8_t end_point )
{
   return APS_APSMEremoveAllGroups ( end_point );
}


uint8_t ZCL_Stack_View_Group_Request ( uint8_t *p_group_data )
{
#if( g_APS_GROUP_DATA_RX_d == 1 )
   uint8_t count;
   for ( count = 0x00; count < g_Num_Of_Group_Table_Entries_c ; count++ )
   {
      if( g_ENTRY_NOT_FOUND_c != APS_getGroupTableIndex( *((uint16_t*)p_group_data) ) ) {
         return g_ZCL_Success_c;
      }
   }
#endif      /* ( g_APS_GROUP_DATA_RX_d == 1 ) */
   return g_ZCL_Not_Found_c;
}
#endif                                 /*g_GROUPS_SERVER_CLUSTER_ENABLE_d*/


/*---------------------------------------------------------------------------*/
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
uint8_t Create_IEEE_Address_Request ( uint16_t dest_addr )
{
   uint8_t message_index;
   APSDE_Request_t *p_apsde_data_request;

    message_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
    if( message_index != g_INVALID_INDEX_c ){
#if ( DEBUG_TEST == 1 )
        allocatedbuffer[message_index]= 91;
        freebuffer[message_index][0]= 91;
        freebuffer[message_index][1]= 0;
#endif
		p_apsde_data_request = (APSDE_Request_t *)buffMgmt_getBufferPointer( message_index );
		g_IAS_Zone_State = g_IEEE_Address_Request_State_c;
		p_apsde_data_request->msgType = g_APSDE_Data_Request_c;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.dest_addr_mode = g_Short_Address_Mode_c;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.dest_address.short_address = dest_addr;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.dest_endpoint =g_ZDO_ENDPOINT_ID_c;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.src_endpoint = g_ZDO_ENDPOINT_ID_c;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.profile_id = 0x0000;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.cluster_id = 0x0001;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.asdulength =
			   sizeof(ZDP_IEEE_Addr_Request_t) + 0x01;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.txoptions = App_getClusterSecurity( 0x0001 );

		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.radius = 0x0A;
		(p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.asdu[0]) = g_ZDP_Transcation_Seq_Num;
		memUtils_memCopy
		(&(p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.asdu[1]),
		 (uint8_t *)&dest_addr,
		 sizeof(uint16_t)
		);
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.asdu[3] = 0x00;
		p_apsde_data_request->APSDE_Request_Primitive.
			apsde_data_request.asdu[4] = 0x00;
		g_Ieee_Address_Request_For_Zone_cluster = g_TRUE_c;
		//return APS_APSDErequest( message_index );
		return APS_APSDE_request(message_index,p_apsde_data_request );;
   }
   return g_ZCL_Failure_c;
}

/*---------------------------------------------------------------------------*/
void Handle_IEEE_Address_Response ( APSDE_Data_Indication_t *p_buffer )
{
   g_IAS_Zone_State = g_IEEE_Address_Response_Processing_State_c;
   memUtils_memCopy ((uint8_t*)a_IEEE_Address_Found, &(p_buffer->a_asdu[3]),0x08 );
}
#endif          /*g_IAS_ZONE_CLUSTER_ENABLE_d*/

#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )
/*---------------------------------------------------------------------------*/
uint16_t ZCL_Stack_Group_Id(uint8_t index)
{
#if ( g_APS_GROUP_DATA_RX_d == 1 )
    return(APS_getGroupId(index));
#endif
    return (0xffff);
}
/*----------------------------------------------------------------------------*/
uint8_t ZCL_Verify_GroupAddrAndEndPoint(uint16_t groupId,uint8_t dstEndPoint)
{
#if ( g_APS_GROUP_DATA_RX_d == 1 )
    return(APS_Verify_GroupAddrAndEndPoint(groupId,dstEndPoint));
#endif
    return g_ENTRY_NOT_FOUND_c;
}

/*----------------------------------------------------------------------------*/
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
uint8_t ZCL_Group_Table_Max_Value()
{
//    return (g_MAX_GROUP_TABLE_ENTRIES_c);

  return g_Num_Of_Group_Table_Entries_c;
}
#endif /*#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )*/
/*---------------------------------------------------------------------------*/
#endif                                  /* ( g_GROUPS_CLUSTER_ENABLE_d == 1 ) */


uint8_t ZCL_Callback_GetMaxAPSPayloadLength( void )
{
#if 0
#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )
    return gp_ZDO_Configuration->node_desc_info.max_buffer_size - 0x12;
#else
    return gp_ZDO_Configuration->node_desc_info.max_buffer_size;
#endif
#endif
    return Zigb_GetMaxAPSPayloadLength();
}

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
