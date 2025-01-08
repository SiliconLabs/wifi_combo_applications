/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
//#include "stack_events.h"
//#include "stack_common.h"
#include "common_includes.h"
#include "buffer_management.h"
#include "AppTestHarness.h"
#include "stack_util.h"
#include "memory_utility.h"
#if ( g_APP_PROFILE_ID_c == 0xc05e )
#include "ZLL_Interpan.h"
#include "ZLL_Network.h"
#include "ZLL_Main.h"
#include "ZLL_State_Machine.h"
#endif

//#include "uart_driver.h"

//#include "uart.h"
//#include "uart_driver.h"
#include "queue.h"
#include "stack_functionality.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "zdp_interface.h"
#include "zdo_interface.h"
#include "apsde_interface.h"
#include "ZCL_Message_Cluster.h"
#include "stack_functionality.h"
#include "AppTimerUtilities.h"
#include "ApplicationThinLayer.h"
#include "commissioning_support.h"
//#include "zb_hil_intf.h"

#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
#include "AppDRLCUtilities.h"
#endif
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Price_Cluster.h"
#include "AppPriceClusterUtilities.h"
#endif
#if( g_MESSAGING_CLUSTER_ENABLE_d == 1)
#include "ZCL_Message_Cluster.h"
#include "AppMessageUtilities.h"
#endif
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
#include "AppZoneTableUtilities.h"
#endif
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1)
#include "AppCBKECertificate.h"
#include "AppCBKEUtilities.h"
#endif
#if(g_TUNNELING_CLUSTER_ENABLE_d == 1)
#include "ZCL_Tunneling_Cluster.h"
#include "AppTunnelingClusterUtilities.h"
#endif

#if(g_APPLIANCE_STATISTICS_CLUSTER_ENABLE_d == 1)
    #include "ZCL_HA_Appliance_Statistics_Cluster.h"
    #include "AppApplianceStatisticUtilities.h"
#endif

#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1)
    #include "ZCL_HA_Appliance_Events_And_Alert_Cluster.h"
    #include "AppApplianceEventsAndAlertUtilities.h"
#endif /* (g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1)*/

#if(g_POWER_PROFILE_CLUSTER_ENABLE_d == 1)
    #include "ZCL_General_Power_Profile_Cluster.h"
    #include "AppPowerProfileUtilities.h"
#endif /* g_POWER_PROFILE_CLUSTER_ENABLE_d == 1 */

#if(g_POLL_CONTROL_CLUSTER_ENABLE_d == 1)
    #include "ZCL_General_Poll_Control_Cluster.h"
    #include "AppPollControlUtilities.h"
#endif /* (g_POLL_CONTROL_CLUSTER_ENABLE_d == 1)*/

#if ( g_APP_PROFILE_ID_c == 0xc05e )
#include "ZLL_Process.h"
#include "ZLL_State_Machine.h"
#include "AppZllCommissioningClusterUtilities.h"
#endif //#if ( g_APP_PROFILE_ID_c == 0xc05e )

#include "app_configuration_default.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_SUCCESS_c                       0x00
#define m_INDEX_PID_c                     0x01
#define m_SYS_GET_REQUEST_c               0x14
#define m_EVENT_c						  0xEE
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
uint8_t g_Length_Of_Primitive = 0x00;
pf_Test_Harness_To_Application Test_Harness_Data_Path = g_NULL_c;
pf_Test_Harness_To_Application Test_Harness_Management_Path = g_NULL_c;
pf_Test_Harness_To_Application Test_Harness_To_Test_Application = g_NULL_c;
pf_Test_Harness_To_Application Test_Harness_To_Test_Utility = g_NULL_c;
uint8_t m_index = 0x00;
Security_Material_Descriptor_t *gp_Security_Material_Descriptor;
uint8_t certificate_location;


#if ( DEBUG_TEST == 1 )
uint8_t allocatedbuffer[24];
uint8_t freebuffer[24][2];
uint8_t skipNeighborTable;
uint8_t g_RouteTableAgeing = 0x01;
uint8_t g_NeighborTableAgeing = 0x01;
uint8_t Additional_NBT[156];
#endif
extern uint16_t RejectionAddrList[10]={0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
								0xFFFF,0xFFFF,0xFFFF,0xFFFF};

extern uint8_t target_address[];

#if (( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 ) || ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ))
uint8_t g_DstEndPoint_c = 0xFF;
#endif
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
uint8_t InterPanData[3];
#endif

extern queue_t ZDP_APP_Queue;
extern void NWK_resetNeighborTable(uint8_t actionToBePerformed);
extern void STK_updateTableBitMap( uint16_t);

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
typedef struct NTS_Msg_Ids {
    uint8_t NTSId;
    uint8_t msgId;
} __attribute__((__packed__))NTS_Msg_Ids;


enum NTS_Message_Ids {
    g_Nwk_Addr_Response_c = 0xC1,
    g_IEEE_addr_Response_c = 0xC3,
    g_Node_Desc_Response_c = 0xC5,
    g_Power_Desc_Response_c = 0xC7,
    g_Simple_Desc_Response_c = 0xC9,
    g_Active_Endpoint_Response_c = 0xCB,
    g_Match_Desc_Response_c = 0xCD,
    g_End_Device_Bind_Response_c = 0xD7,
    g_Bind_Response_c = 0xD9,
    g_Unbind_Response_c = 0xDB,
    g_Mgmt_Permit_Joining_Response_c = 0xEB,
    g_Add_Group_Confirm_c = 0xAC,
    g_Remove_Group_Confirm_c = 0xAE,
    g_Remove_All_Groups_Confirm_c = 0xBE,
    g_Transport_Key_Indication_c = 0xB5,
    g_Update_Device_Indication_c = 0xB7,
    g_Remove_Device_Indication_c = 0xB9,
    g_Switch_Key_Indication_c = 0xBD,
    g_Start_Response_c = 0x17,
    g_Get_Confirm_c = 0xA8,
    g_Set_Confirm_c = 0xAA,
    g_Data_Confirmation_c = 0xA1,
    g_Data_Indication_c = 0xA2,
    g_Permit_Join_Confirm_c = 0x8A,
    g_Application_Poll_Response = 0x58,
    g_NTS_Set_Route_Table_c = 0x78,
    g_NLME_Route_Discovery_Confirm = 0x91,
    g_ReStart_Response_c = 0x13,
    g_Mgmt_Nwk_Update_Notify_c = 0x19,
    g_NLME_Sync_Confirm_c      = 0x58
};

const NTS_Msg_Ids g_Look_Table_For_ZDP_Response[] = {
    { 0xC1, g_ZDP_NWK_Addr_Response_c },
    { 0xC3, g_ZDP_IEEE_Addr_Response_c },
    { 0xC5, g_ZDP_Node_Desc_Response_c },
    { 0xC7, g_ZDP_Power_Desc_Response_c },
    { 0xC9, g_ZDP_Simple_Desc_Response_c },
    { 0xCB, g_ZDP_Active_Endpoint_Response_c },
    { 0xCD, g_ZDP_Match_Desc_Response_c },
    { 0xD7, g_ZDP_End_Device_Bind_Response_c },
    { 0xD9, g_ZDP_Bind_Response_c },
    { 0xDB, g_ZDP_Unbind_Response_c },
    { 0xEB, g_ZDP_Mgmt_Permit_Joining_Response_c },
    { 0xAC, g_APSME_Add_Group_Confirm_c },
    { 0xAE, g_APSME_Remove_Group_Confirm_c },
    { 0xBE, g_APSME_Remove_All_Groups_Confirm_c },
    { 0xB5, g_APSME_Transport_Key_Indication_c },
    { 0xB9, g_APSME_Remove_Device_Indication_c },
    { 0xBD, g_APSME_Switch_Key_Indication_c },
    { 0x17, g_Start_Device_Response_c },
    { 0xA8, g_APSME_Get_Confirm_c },
    { 0xAA, g_APSME_Set_Confirm_c },
    { 0xA1, g_APSDE_Data_Confirmation_c },
    { 0xA2, g_APSDE_Data_Indication_c },
    { 0x8A, g_NLME_Permit_Joining_Conf_c },
    { 0x58, g_NLME_Sync_Conf_c },
    { 0x87, g_NLME_Network_Discovery_Confirmation_c },
    { 0x7B, g_NLME_Leave_Indication_c },
    { 0x7A, g_NLME_Leave_Confirmation_c },
    { 0x91, g_NTS_Route_Discovery_Confirm_c },
    { 0x19, g_ZDP_Mgmt_Nwk_Update_Notify_c },
    { 0x21, g_ZDP_System_Server_Discovery_Response_c }
};

#define m_NUMBER_OF_ZDP_PRIMITIVES_c    sizeof( g_Look_Table_For_ZDP_Response )
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
void App_updatePriceEvents ( uint8_t* pPriceData );
#if ( g_SE_PROFILE_1_1_d == 1 )
void App_updateBlockPeriodEvents ( uint8_t* pBlockData );
#endif /* ( g_SE_PROFILE_1_1_d == 1 ) */
#endif

#if( g_MESSAGING_CLUSTER_ENABLE_d == 1)
void App_UpdateMessageTable( DisplayMessage_t *pDisplayMsg, uint8_t serverTable );
#endif
void Create_ZDP_Request ( uint8_t ZDP_request_id, uint8_t msg_index );
#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
void AppsetLoadControlEvent( uint8_t index, uint8_t * LCEData );
#endif
//#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
//void AppOptInOrOut( uint32_t eventId, bool optIn );
//#endif

uint8_t App_GetTransSeqNumber( void );
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void APS_interpan_data_request( uint8_t bufferid,
                                APSDE_InterPan_Data_Request_t* pApsDataReq );
#endif

#if ( g_REPORTING_ENABLE_d == 1 )
void App_setReportingInformation ( uint8_t index, uint8_t addrMode,
                                   uint8_t dstEndPoint, uint8_t* dstAddr );
#endif

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
#if 1
uint8_t TH_getNTSid(uint8_t stackId)
{
    uint8_t i = 0x00;
    for (; i < m_NUMBER_OF_ZDP_PRIMITIVES_c; i++ ) {
        if (g_Look_Table_For_ZDP_Response[i].msgId == stackId) {
            return g_Look_Table_For_ZDP_Response[i].NTSId;
        }
    }
    return stackId;
}

/**************************************************************************************/
static uint8_t TH_convertNTSidsToZDPrequestIds(uint8_t msgType)
{
    uint8_t convertedMsgtype = 0xFF;
    if( msgType > m_DEVICE_DISCOVERY_START_ID_c )
    {
    	convertedMsgtype = ((msgType - m_DEVICE_DISCOVERY_START_ID_c) / 2);
    }

    if (msgType == 0x16) {
        convertedMsgtype = 0xbd;
    }
    else if (msgType > 0xcc && msgType <= 0xd4) {
        convertedMsgtype += 0x09;
    }
    else if (msgType > 0xd5 && msgType <= 0xda) {
        convertedMsgtype += 0x15;
    }
    else if (msgType > 0xdb && msgType <= 0xe6) {
        convertedMsgtype += 0x22;
    }
    else if (msgType > 0x1f && msgType <= 0x2e) {
        convertedMsgtype = ((msgType - 0x0b) >> 1) + 0x0b;
    }
    else if (msgType > 0x2f && msgType <= 0x3e) {
        convertedMsgtype = (msgType >> 1) + 0x0b;
    }
    else if (msgType > 0xA2 && msgType <= 0xbc) {
        convertedMsgtype = msgType;
    }
    else if (msgType == 0xEA) {
        convertedMsgtype = g_ZDP_Mgmt_Permit_Joining_Request_c;
    }
    if (msgType == 0x18) {
        convertedMsgtype = g_ZDP_Mgmt_Nwk_Update_Request_c;
    }
    return convertedMsgtype;
}
/**************************************************************************************/
void testHrns_passMessageDown ( uint8_t buffer_index )
{
    uint8_t *buffer_rx;
    uint8_t type_data;

    buffer_rx= buffMgmt_getBufferPointer(buffer_index);

    g_Length_Of_Primitive = *( buffer_rx);

    type_data = *( buffer_rx + 1 );
    printf("Type of Data is %X[%X]\n",type_data,Test_Harness_To_Test_Application);

    memUtils_memCopy ( buffer_rx, buffer_rx + 1, g_Length_Of_Primitive );
    if( Test_Harness_To_Test_Application != g_NULL_c ){

        Test_Harness_To_Test_Application ( type_data, buffer_index );
    }
    else if ( type_data == m_SYS_GET_REQUEST_c ) {
        Test_Harness_To_Test_Utility ( type_data, buffer_index );
    }
    /*Check if the type of message ids for data and forward it to the data path*/
    else if ( ( type_data == g_Data_Request_c ) || ( type_data == g_Indirect_Request_c ) ||
              ( type_data == g_Direct_Request_c ) || ( type_data == g_Group_Request_c ) ) {

            Test_Harness_Data_Path ( type_data, buffer_index );
    }
   	/* If the type is neither system set request, systen get request nor
    ASPDE_Data_request or Application data requests,then the below API is
    invoked to handle the management messages.
    The type of message ids for management and forward it to the
	management path. */
    else {
        Test_Harness_Management_Path ( type_data, buffer_index );
    }
}

/**************************************************************************************/
void App_Handle_Data_Request ( uint8_t msgId, uint8_t msg_index )
{
	uint8_t i=0,frame_control = g_Client_To_Server_c;
	APSDE_Request_t * pBuffer;
	APSDE_Data_Request_t* pApsdeData;
	uint8_t status;
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
	APSDE_InterPan_Data_Request_t* pApsDataReq;
#endif
    uint8_t bufferIndex;
    uint8_t *pMessageData = buffMgmt_getBufferPointer( msg_index );

    pMessageData++;

    bufferIndex = buffMgmt_allocateBuffer( g_LARGE_BUFFER_c );

    if (g_NO_AVAILABLE_BUFFER_c != bufferIndex) {

#if ( DEBUG_TEST == 1 )
	  /* Buffer ID --> 95 */
        allocatedbuffer[bufferIndex]= 95;
        freebuffer[bufferIndex][0]= 95;
        freebuffer[bufferIndex][1]= 0;
#endif
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
	if ( InterPanData[0] == g_TRUE_c ) {
        pApsDataReq = (APSDE_InterPan_Data_Request_t *) buffMgmt_getBufferPointer( bufferIndex );
		pApsDataReq->srcaddrmode =  g_IEEE_Address_Mode_c;
		pApsDataReq->dstaddrmode =  *pMessageData++;
		pApsDataReq->dstpanid = *((uint16_t*)&InterPanData[1]);

		if ( pApsDataReq->dstaddrmode == g_IEEE_Address_Mode_c ) {
			memUtils_memCopy( pApsDataReq->dstaddress.IEEE_address, pMessageData, 0x08 );
			pMessageData += 9;
		}
		else {
			memUtils_memCopy( pApsDataReq->dstaddress.IEEE_address, pMessageData, 0x02 );
			pMessageData += 3;
		}
		pApsDataReq->profileid = *((uint16_t*)pMessageData);
		pMessageData += 2;
		pApsDataReq->clusterid = *((uint16_t*)pMessageData);
		pMessageData += 3;
		pApsDataReq->asdulength = *pMessageData++;
		memUtils_memCopy( pApsDataReq->a_asdu, pMessageData, pApsDataReq->asdulength );
		if( ( pApsDataReq->a_asdu[0] & 0x80 ) == 0x80 ) {
			frame_control = g_Server_To_Client_c;
		}
		pApsDataReq->asdulength =  ZCL_Create_Command ( frame_control,
											pApsDataReq->a_asdu,
											(App_ZCL_Request_t*)pApsDataReq->a_asdu,
											pApsDataReq->asdulength,
									        App_GetTransSeqNumber());
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
        if ( pApsDataReq->clusterid == g_MESSAGING_CLUSTER_c ) {
            if ( ( pApsDataReq->a_asdu[0x02] == g_Display_Message_Command_c ) &&
                 ( frame_control == g_Server_To_Client_c ) ) {
                 App_UpdateMessageTable( (DisplayMessage_t*)&pApsDataReq->a_asdu[0x03], g_TRUE_c );
            }
        }
#endif
//#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
//        if ( pApsDataReq->clusterid == g_PRICE_CLUSTER_c ) {
//            if ( ( pApsDataReq->a_asdu[0x02] == g_PublishPriceCmd_c ) &&
//                 ( frame_control == g_Server_To_Client_c ) ) {
//                    App_updatePriceEvents( &pApsDataReq->a_asdu[0x03] );
//            }
//        }
//#endif
	    APS_interpan_data_request ( bufferIndex,  pApsDataReq );
	}
	else {
#endif
		pBuffer = (APSDE_Request_t *) buffMgmt_getBufferPointer( bufferIndex );
		pBuffer->msgType = g_APSDE_Data_Request_c;
		pApsdeData = &(pBuffer->APSDE_Request_Primitive.apsde_data_request);
		pApsdeData->dest_addr_mode = *pMessageData;

		if ( (g_Short_Address_Mode_c == pApsdeData->dest_addr_mode) ||
			 (g_Group_Address_Mode_c == pApsdeData->dest_addr_mode) ) {
			memUtils_memCopy((uint8_t*) &(pApsdeData->dest_address.short_address),++pMessageData, g_SHORT_ADDRESS_LENGTH_c);
			pMessageData += g_SHORT_ADDRESS_LENGTH_c;
		}
		else if (g_IEEE_Address_Mode_c == pApsdeData->dest_addr_mode) {
			memUtils_memCopy((uint8_t *)&(pApsdeData->dest_address.IEEE_address),++pMessageData, g_EXTENDED_ADDRESS_LENGTH_c);
			pMessageData += g_EXTENDED_ADDRESS_LENGTH_c;
		}
		else {
			memUtils_memSet((uint8_t*) &(pApsdeData->dest_address), 0x00, g_EXTENDED_ADDRESS_LENGTH_c);
			pMessageData++;
		}
		pApsdeData->dest_endpoint = 0x00;
		if ( (g_IEEE_Address_Mode_c == pApsdeData->dest_addr_mode) ||
			 ( g_Short_Address_Mode_c == pApsdeData->dest_addr_mode) ) {
			pApsdeData->dest_endpoint = *pMessageData++;
		}
		memUtils_memCopy((uint8_t*) &(pApsdeData->profile_id), pMessageData, 0x02 );
		pMessageData += 0x02;
		memUtils_memCopy((uint8_t*) &(pApsdeData->cluster_id), pMessageData, 0x02 );
		pMessageData += 0x02;
		pApsdeData->src_endpoint = *pMessageData++;
		pApsdeData->asdulength = *pMessageData++;
		memUtils_memCopy(&(pApsdeData->asdu[0]), pMessageData, pApsdeData->asdulength );
		pMessageData += pApsdeData->asdulength;
		pApsdeData->txoptions = *pMessageData++;
		pApsdeData->radius = *pMessageData;
		if( ( pApsdeData->asdu[0] & 0x80 ) == 0x80 ) {
			frame_control = g_Server_To_Client_c;
		}
		pApsdeData->asdulength =  ZCL_Create_Command ( frame_control,
											pApsdeData->asdu,
											(App_ZCL_Request_t*)pApsdeData->asdu,
											pApsdeData->asdulength,
											App_GetTransSeqNumber());
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
        if ( pApsdeData->cluster_id == g_MESSAGING_CLUSTER_c ) {
            if ( ( pApsdeData->asdu[0x02] == g_Display_Message_Command_c ) &&
                 ( frame_control == g_Server_To_Client_c ) ) {
                 App_UpdateMessageTable( (DisplayMessage_t*)&pApsdeData->asdu[0x03], g_TRUE_c );
            }
        }
#endif
//#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
//        if ( pApsdeData->cluster_id == g_PRICE_CLUSTER_c ) {
//            if ( ( pApsdeData->asdu[0x02] == g_PublishPriceCmd_c ) &&
//                 ( frame_control == g_Server_To_Client_c ) ) {
//                   App_updatePriceEvents( &pApsdeData->asdu[0x03] );
//            }
//        }
//#endif

#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
        if ( pApsdeData->cluster_id == g_TUNNELING_CLUSTER_c ) {
            if ( pApsdeData->asdu[0x02] == g_ServerReadyDataCommand_c ) {
                   TunnelingServerTunnel_t* tunnel;
                   uint8_t status;
                   status = App_tunnelingServerFindTunnel( *((uint16_t*)&pApsdeData->asdu[0x03]),
                                                   pApsdeData->dest_address.short_address,
                                                   pApsdeData->dest_endpoint, &tunnel );
                   if ( status == g_ZCL_Success_c ) {
                       tunnel->numberOfBytesLeft = *((uint16_t*)&pApsdeData->asdu[0x05]);
                   }
            }
        }
#endif

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
        if ( pApsdeData->cluster_id == g_TUNNELING_CLUSTER_c ) {
            if ( pApsdeData->asdu[0x02] == g_ClientReadyDataCommand_c ) {
                   TunnelingClientTunnel_t *tunnel;
                   uint8_t status;
                   status = App_tunnelingClientFindTunnel( *((uint16_t*)&pApsdeData->asdu[0x03]),
                                            pApsdeData->dest_address.short_address,
                                            pApsdeData->dest_endpoint, &tunnel );
                   if ( status == g_ZCL_Success_c ) {
                       tunnel->numberOfBytesLeft = *((uint16_t*)&pApsdeData->asdu[0x05]);
                   }
            }
            else if( pApsdeData->asdu[0x02] == g_RequestTunnelCommand_c ) {
                protocolId = pApsdeData->asdu[0x03];
                manufacturerCode = *((uint16_t*)&pApsdeData->asdu[0x04]);
                flowControl = pApsdeData->asdu[0x06];

            }
            else if ( pApsdeData->asdu[0x02] == g_CloseTunnelCommand_c ) {
                   TunnelingClientTunnel_t *tunnel;
                   uint8_t status;
                   status = App_tunnelingClientFindTunnel( *((uint16_t*)&pApsdeData->asdu[0x03]),
                                            pApsdeData->dest_address.short_address,
                                            pApsdeData->dest_endpoint, &tunnel );
                   if ( status == g_ZCL_Success_c ) {
                       tunnel->inUse = g_FALSE_c;
                   }
            }
        }
#endif

      APS_APSDErequest(bufferIndex);
      //APS_APSDE_request ( bufferIndex,pApsdeData);

#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
		}
#endif
    }
    buffMgmt_freeBuffer ( msg_index, 224 );
    return;
}

/**************************************************************************************/

#if 1
void Application_to_ZDO ( uint8_t msg_type, uint8_t msg_index )
{
	uint8_t device_type;
  uint16_t clusterId=0;
  uint16_t attributeId=0;
  uint8_t dataType;
  uint8_t endpoint;
  uint8_t *p_buffer_rx;
    uint8_t status;
  uint8_t aDestAddr[2] = { 0x00, 0x00 };
  uint8_t convertedMsgtype;
  uint8_t ext_panid[8]={0};
//#if (g_APP_APS_GROUP_DATA_RX_d == 1)
    uint8_t array[8],a[8];
//#endif

  p_buffer_rx = buffMgmt_getBufferPointer(msg_index);

  if(*p_buffer_rx == m_SYS_SET_REQUEST_c ){
    msg_type = (*(p_buffer_rx+1));
  }
  switch(msg_type)
  {
    case g_MAC_Extended_Address_c:
      {
        ZDO_setExtendedAddress(p_buffer_rx + 3);
        rsi_zigb_get_self_ieee_address(a);

#if(g_APP_PROFILE_ID_c == 0xc05e)
        UpdateSubDeviceExtendedAddress(p_buffer_rx + 3);
#endif/*#if(g_APP_PROFILE_ID_c == g_ZLL_PROFILE_ID_c)*/

        *p_buffer_rx = m_SET_CONFIRM_c;
        *(p_buffer_rx + 1) = g_ZCL_Success_c;
        *(p_buffer_rx + 2) = g_MAC_Extended_Address_c;
        testHrns_passMessageUp(msg_index, 3);
      }
      return;
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
    case g_set_InterPanData_c:
      {
        memUtils_memCopy( InterPanData, p_buffer_rx + 3, 0x03 );
        *p_buffer_rx = m_SET_CONFIRM_c;
        *(p_buffer_rx + 1) = g_ZCL_Success_c;
        *(p_buffer_rx + 2) = g_set_InterPanData_c;
        testHrns_passMessageUp(msg_index, 3 );
      }
      return;
#if (g_APP_PROFILE_ID_c == 0xc05e)
#ifdef g_ZLL_LIGHT_CONTROLLER_c
    case g_ZLL_Trigger_Touch_link_Procedure:
      {
        ZLL_TriggerTouchLinkCommisioning();
        *p_buffer_rx = m_SET_CONFIRM_c;
        *(p_buffer_rx + 1) = g_ZCL_Success_c;
        *(p_buffer_rx + 2) = g_ZLL_Trigger_Touch_link_Procedure;
        testHrns_passMessageUp(msg_index, 3 );
      }
      return;

    case g_ZLL_Reset_Target_Device:
      {
        ZLL_ResetTargetDevice( *(p_buffer_rx + 3));
        *p_buffer_rx = m_SET_CONFIRM_c;
        *(p_buffer_rx + 1) = g_ZCL_Success_c;
        *(p_buffer_rx + 2) = g_ZLL_Reset_Target_Device;
        testHrns_passMessageUp(msg_index, 3 );
      }
      return;
    case g_ZLL_Set_Identify_Duration:
      {
        uint16_t identify_duration = 0x0000;
        memUtils_memReverseCopy( (uint8_t*)&identify_duration,   p_buffer_rx + 3, 0x02 );
        ZLL_SetIdentifyDuration(identify_duration);
        *p_buffer_rx = m_SET_CONFIRM_c;
        *(p_buffer_rx + 1) = g_ZCL_Success_c;
        *(p_buffer_rx + 2) = g_ZLL_Set_Identify_Duration;
        testHrns_passMessageUp(msg_index, 3 );

      }
      return;
    case g_ZLL_Stop_Identify_Mode:
      {
        /* duration of zero indicates exit identify mode*/
        uint16_t identify_duration = 0x0000;
        ZLL_SetIdentifyDuration(identify_duration);
        *p_buffer_rx = m_SET_CONFIRM_c;
        *(p_buffer_rx + 1) = g_ZCL_Success_c;
        *(p_buffer_rx + 2) = g_ZLL_Stop_Identify_Mode;
        testHrns_passMessageUp(msg_index, 3 );
      }
      return;
    case g_ZLL_Trigger_Utility_Notification_c:
      {
        /* This interface is used to send the ,endpoint information command*/
        uint16_t dest_addr = 0xffff;
        if(p_buffer_rx + 3 != g_NULL_c)
          memUtils_memReverseCopy( (uint8_t*)&dest_addr, p_buffer_rx + 3, 0x02 );

        ZLL_InitiateEndpointNotification(dest_addr);
        *p_buffer_rx = m_SET_CONFIRM_c;
        *(p_buffer_rx + 1) = g_ZCL_Success_c;
        *(p_buffer_rx + 2) = g_ZLL_Trigger_Utility_Notification_c;
        testHrns_passMessageUp(msg_index, 3 );
      }
      return;

#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c

#ifdef g_ZLL_LIGHT_DEVICE_c
    case g_ZLL_Target_Set_Priority:
      ZLL_SetTargetPriority((bool)*(p_buffer_rx + 3));
      *p_buffer_rx = m_SET_CONFIRM_c;
      *(p_buffer_rx + 1) = g_ZCL_Success_c;
      *(p_buffer_rx + 2) = g_ZLL_Target_Set_Priority;
      testHrns_passMessageUp(msg_index, 3 );
      return;
      break;
#endif //ifdef g_ZLL_LIGHT_DEVICE_c


    case g_ZLL_Security_Set_KeyIndex:
        ZLL_SetKeyIndex((bool)*(p_buffer_rx + 3));
      *p_buffer_rx = m_SET_CONFIRM_c;
      *(p_buffer_rx + 1) = g_ZCL_Success_c;
      *(p_buffer_rx + 2) = g_ZLL_Security_Set_KeyIndex;
      testHrns_passMessageUp(msg_index, 3 );
      return;
      break;
#endif //(g_APP_PROFILE_ID_c == g_ZLL_PROFILE_ID_c)
#endif //#if ( g_INTERPAN_DATA_ENABLE_d == 1 )

#if(g_HA_1_2_d == 1)
#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)
      /* This interface is used to send log information of the server to the
         client without client requesting for the same. */

    case g_ApplianceStaticClusterEvent_c:
      {
        uint8_t logNotificationOrStatisticAvailable = 0x00;

        logNotificationOrStatisticAvailable = *(p_buffer_rx+3);
        if(0x01 == logNotificationOrStatisticAvailable ){
          /* Call LogNotification Event */
          InvokeLogNotificationCommand();
        }
        else if(0x02 == logNotificationOrStatisticAvailable ){
          /* Call StatisticsAvailableEvent*/
          InvokeStatisticsAvailableCommand();
        }
        *p_buffer_rx = m_SET_CONFIRM_c;
        if((0x01 == logNotificationOrStatisticAvailable )||
            (0x02 == logNotificationOrStatisticAvailable )){
          *(p_buffer_rx + 1) = g_ZCL_Success_c;
        }
        else {
          *(p_buffer_rx + 1) = g_ZCL_Invalid_Value_c;
        }
        *(p_buffer_rx + 2) = g_ApplianceStaticClusterEvent_c;
        testHrns_passMessageUp(msg_index, 3);
      }
      return;
#endif /* g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d */
#if( g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1)

    case g_ApplianceEventsAndAlertEvent_c :{
                                             uint8_t typeofAction = 0x00;

                                             typeofAction = *(p_buffer_rx+3);

                                             *p_buffer_rx = m_SET_CONFIRM_c;
                                             *(p_buffer_rx + 1) = g_ZCL_Success_c;
                                             *(p_buffer_rx + 2) = g_ApplianceEventsAndAlertEvent_c;

                                             /* typeofAction = 0x01 used to store alerts into warning table.
                                                typeofAction = 0x02 used to send the alerts which are in alerts table
                                                i.e., alert notification command
                                                typeofAction = 0x03 used to send the random event to the client */
                                             if( 0x01 == typeofAction){
                                               StoreAlertIntoAlertsTable();
                                             }
                                             else if( 0x02 == typeofAction){
                                               InvokeAlertNotificationCommand();
                                             }
                                             else if(0x03 == typeofAction){
                                               InvokeEventNotificationCommand();
                                             }
                                             else{
                                               *(p_buffer_rx + 1) = g_ZCL_Invalid_Value_c;
                                             }
                                             *p_buffer_rx = m_SET_CONFIRM_c;
                                             *(p_buffer_rx + 1) = g_ZCL_Success_c;
                                             *(p_buffer_rx + 2) = g_ApplianceEventsAndAlertEvent_c;

                                             testHrns_passMessageUp(msg_index, 3);
                                           }
                                           return;
#endif /*g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1*/
#if(g_POWER_PROFILE_CLUSTER_ENABLE_d == 1)
    case g_PowerProfileClusterEvent_c : {
                                          uint8_t status = g_ZCL_Success_c;
                                          uint8_t typeOfNotification = 0x00,length = 0x00;
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
                                          uint8_t tempData[10] ={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#endif /* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1*/

                                          *p_buffer_rx = m_SET_CONFIRM_c;
                                          *(p_buffer_rx + 1) = status;
                                          *(p_buffer_rx + 2) = g_PowerProfileClusterEvent_c;
                                          typeOfNotification = *(p_buffer_rx+3);
                                          length = *(p_buffer_rx+2);


#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)

                                          /* TypeOf Server Notification
                                             Power Profile Notification = 0x00
                                             Power Profile State Notification = 0x04
                                             energy phases schedule state notification = 0x08
                                             power profile schedule constraints notification =0x09
                                             */
                                          if(0x00 == typeOfNotification ){
                                            /* Transmits Profile Notification command */
                                            InvokeTransmistOfProfileNotification();
                                          }
                                          else if(0x04 == typeOfNotification ){
                                            /* Transmit Profile State Notification*/
                                            InvokeTransmitOfProfileStateNotification();
                                          }
                                          else if(0x08 == typeOfNotification){
                                            /* Transmit Energy Phases Schedule State Notification */
                                            InvokeTransmitOfPhasesScheduleStateNotification();
                                          }
                                          else if(0x09 == typeOfNotification ){
                                            /* Transmit Power Profile Schedule Constraints Notification */
                                            /* Checking atleast it has start or stop parameter to change*/
                                            /* notification will be sent only if value is updated */
                                            length = *(p_buffer_rx+4);
                                            uint8_t profileId = *(p_buffer_rx+5);
                                            memUtils_memCopy(tempData,(p_buffer_rx+6),length -1 );
                                            if((length -1 )> 0x02){
                                              InvokeTransmitScheduleConstraintsNotification(profileId
                                                  ,tempData,length-1);
                                            }
                                          }
                                          else{
                                            *(p_buffer_rx + 1) = g_ZCL_Invalid_Value_c;
                                          }
#endif /* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1*/

#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
                                          /* Client Notification Commands
                                             Energy PhasesSchedule Notification = 0x04
                                             0x0a is used to set cost of the profile.
                                             */

                                          if(0x04 == typeOfNotification ){
                                            /* Transmits Power Profile's Energy Phases Schedule Notification
                                               command */
                                            InvokeTransmistEnergyPhasesSchNotification();
                                          }
                                          else if(0x0a == typeOfNotification){
                                            uint8_t tempData[10] =
                                            {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
                                            /* 0x0a command is not present ,*/
                                            length = *(p_buffer_rx+4);
                                            /* length shd be equal to 8 which includes
                                               profileId(1) ,currency(2),price(4), priceTrailingDigit(1)*/
                                            if(length == 0x08){
                                              memUtils_memCopy(tempData,(p_buffer_rx+5),length );
                                              SetCostOfProfile(tempData,length);
                                            }
                                            else{
                                              status = g_ZCL_Failure_c;
                                            }
                                          }

#endif
                                          *p_buffer_rx = m_SET_CONFIRM_c;


                                          *(p_buffer_rx + 1) = status;
                                          *(p_buffer_rx + 2) = g_PowerProfileClusterEvent_c;

                                          testHrns_passMessageUp(msg_index, 3);
                                        }
                                        return;
#endif  /*g_POWER_PROFILE_CLUSTER_ENABLE_d */
#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
    case g_PollControlSetPollRateEvent_c:{
                                           /* expected parameter to be set by client
                                              startFastPolling(1byte) T/F
                                              FastTimeoutPolling(2byte)*/

                                           uint8_t status = g_ZCL_Success_c,expectedlength = 0x03;

                                           *p_buffer_rx = m_SET_CONFIRM_c;
                                           /* This interface is used to set values for check in response
                                              parameter*/
                                           if(expectedlength == *(p_buffer_rx + 2)){
                                             bool startFastPoll = g_FALSE_c;
                                             uint16_t FastPollTimeout = 0xffff;

                                             if(*(p_buffer_rx + 4)){
                                               startFastPoll = g_TRUE_c;
                                             }
                                             else{
                                               startFastPoll = g_FALSE_c;
                                             }
                                             memUtils_memCopy((uint8_t*)&FastPollTimeout,(uint8_t*)(p_buffer_rx + 5),
                                                 0x02);
                                             App_UpdateUserSetValue(startFastPoll,FastPollTimeout);
                                             status = g_ZCL_Success_c;

                                           }
                                           else{
                                             status = g_ZCL_Invalid_Value_c;

                                           }

                                           *(p_buffer_rx + 1) = status;
                                           *(p_buffer_rx + 2) = g_PollControlSetPollRateEvent_c;

                                           testHrns_passMessageUp(msg_index, 3);

                                         }
                                         return;

#endif /* g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 */
#endif /* g_HA_1_2_d == 1 */
#if ( g_REPORTING_ENABLE_d == 1 )
    case m_SET_REPORTING_TARGET_INFO_c:
                                         {
                                           App_setReportingInformation( p_buffer_rx[3],
                                               p_buffer_rx[4], p_buffer_rx[5], &p_buffer_rx[6] );

                                           p_buffer_rx[0] = m_SET_CONFIRM_c;
                                           p_buffer_rx[1] = g_ZCL_Success_c;
                                           p_buffer_rx[2] = m_SET_REPORTING_TARGET_INFO_c;
                                           testHrns_passMessageUp(msg_index, 3);
                                           return;
                                         }
#endif
    case m_SET_TARGET_ADDRESS_c:
                                         {
                                           target_address[0] = p_buffer_rx[3];
                                           target_address[1] = p_buffer_rx[4];

                                           p_buffer_rx[0] = m_SET_CONFIRM_c;
                                           p_buffer_rx[1] = g_ZCL_Success_c;
                                           p_buffer_rx[2] = m_SET_TARGET_ADDRESS_c;
                                           testHrns_passMessageUp(msg_index, 3);
                                         }
                                         return;
                                         //#if ( g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1 )  && ( g_APS_LAYER_SEC_PROCESSING_d == 1 )

    case g_set_NWK_Extended_Pan_Id_c:
                                         memUtils_memCopy( gp_Active_Startup_Attrib_Set->a_extended_pan_id,
                                             p_buffer_rx + 3, 8);
#if( g_COMMISSIONING_CAPABILITIES_d == 1 )
                                         ZDOupdateSAS( (uint8_t *)gp_Active_Startup_Attrib_Set, m_index );
#endif
                                         *p_buffer_rx= m_SET_CONFIRM_c;
                                         *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                         *(p_buffer_rx + 2 ) = g_set_NWK_Extended_Pan_Id_c;
                                         testHrns_passMessageUp( msg_index, 3 );
                                         return;
    case g_get_NWK_Extended_Pan_Id_c:
                                         rsi_zigb_get_extended_panid(ext_panid);
                                         *p_buffer_rx= m_SET_CONFIRM_c;
                                         *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                         memUtils_memCopy( p_buffer_rx+2,
                                             ext_panid, 8);
                                         *(p_buffer_rx + 10 ) = g_get_NWK_Extended_Pan_Id_c;
                                         testHrns_passMessageUp( msg_index, 11 );
                                         return;
    case g_get_trust_center_link_keys_c:
                                         {
                                           TrustCenterLinkKeyTable_t tcLinkKey;
                                           *p_buffer_rx = m_SET_CONFIRM_c;
                                           m_index     =  *( p_buffer_rx +3);
                                           *(p_buffer_rx + 1 ) =GetTrustCenterLinkKey(&tcLinkKey, m_index );

                                           memUtils_memCopy( p_buffer_rx + 2,tcLinkKey.LinkKeyDevicePair.deviceAddress,8 );

                                           memUtils_memCopy(p_buffer_rx + 10, tcLinkKey.LinkKeyDevicePair.linkKey,16 );
                                           memUtils_memCopy(p_buffer_rx + 26, tcLinkKey.LinkKeyFrameCounter.outgoingFrameCounter,
                                               4 );
                                           memUtils_memCopy(p_buffer_rx + 30, tcLinkKey.LinkKeyFrameCounter.incomingFrameCounter,
                                               4 );
                                           *(p_buffer_rx +34)= g_get_trust_center_link_keys_c;
                                           testHrns_passMessageUp( msg_index, 35 );
                                         }
                                         return;
    case g_set_trust_center_link_keys_c:
                                         {
                                           TrustCenterLinkKeyTable_t tcLinkKey;
                                           m_index     =  *( p_buffer_rx +3);
                                           memUtils_memCopy(tcLinkKey.LinkKeyDevicePair.deviceAddress,
                                               p_buffer_rx + 4, 8 );
                                           memUtils_memCopy(tcLinkKey.LinkKeyDevicePair.linkKey,
                                               p_buffer_rx + 12, 16 );
                                           memUtils_memCopy( tcLinkKey.LinkKeyFrameCounter.outgoingFrameCounter,
                                               p_buffer_rx + 28, 4 );
                                           memUtils_memCopy( tcLinkKey.LinkKeyFrameCounter.incomingFrameCounter,
                                               p_buffer_rx + 32,4 );

                                           *p_buffer_rx = m_SET_CONFIRM_c;
                                           *(p_buffer_rx + 1 ) =SetTrustCenterLinkKey(tcLinkKey,m_index);
                                           *(p_buffer_rx +2)= g_set_trust_center_link_keys_c;
                                           testHrns_passMessageUp( msg_index, 3 );
                                         }
                                         return;
                                         ///#endif	/* ( g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1 )  && ( g_APS_LAYER_SEC_PROCESSING_d == 1 )	*/

    case g_set_nwk_key_c: 	  {
                                gp_Security_Material_Descriptor[m_index].keysequencenumber=*( p_buffer_rx + 3);
                                memUtils_memCopy( (uint8_t *)&gp_Security_Material_Descriptor[m_index].outgoing_frame_counter,
                                    p_buffer_rx + 4, 4);
                                memUtils_memCopy( gp_Security_Material_Descriptor[m_index].key,
                                    p_buffer_rx + 8, 16);

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_set_nwk_key_c;
                                testHrns_passMessageUp( msg_index, 3 );

                              }
#if ( g_COMMISSIONING_CAPABILITIES_d == 1 )
    case m_UPDATE_SAS_REQUEST_c :
                              {
                                memUtils_memCopy( gp_Active_Startup_Attrib_Set->a_extended_pan_id,
                                    p_buffer_rx + 3, 8);
                                memUtils_memCopy( (uint8_t *)&gp_Active_Startup_Attrib_Set->channel_mask,
                                    p_buffer_rx + 11, 4);
                                memUtils_memCopy( (uint8_t *)&gp_Active_Startup_Attrib_Set->protocol_version,
                                    p_buffer_rx + 15, 30);
                                memUtils_memCopy( (uint8_t *)&gp_Active_Startup_Attrib_Set->network_manager_address,
                                    p_buffer_rx + 45, 3);
                                memUtils_memCopy( (uint8_t *)&gp_Active_Startup_Attrib_Set->time_between_scans,
                                    p_buffer_rx + 48, 9);
                                memUtils_memCopy( (uint8_t *)&gp_Active_Startup_Attrib_Set->security_timeout_period,
                                    p_buffer_rx + 57, 41);
                                memUtils_memCopy( (uint8_t *)&gp_Active_Startup_Attrib_Set->concentrator_radius,
                                    p_buffer_rx + 98, 6 );
                                ZDOupdateSAS( (uint8_t *)gp_Active_Startup_Attrib_Set, m_index );
                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = m_UPDATE_SAS_REQUEST_c;
                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
    case m_NVM_SAVE_ZDO_REQUEST_c :
                              {
                                memUtils_memCopy( (uint8_t *)gp_ZDO_Configuration, p_buffer_rx + 3,8);
                                memUtils_memCopy( (uint8_t *)&(gp_ZDO_Configuration->config_no_of_data_txns_before_NVM_save),
                                    p_buffer_rx + 11, 32);
                                //ZDOupdateConfiguration ( (uint8_t *)gp_ZDO_Configuration, g_FALSE_c );
                                //rsi_zigb_update_zdo_configuration( (uint8_t *)gp_ZDO_Configuration);
                                rsi_zigb_update_all_zdo_configuration((uint8_t *)gp_ZDO_Configuration);
                                *p_buffer_rx = m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = m_NVM_SAVE_ZDO_REQUEST_c;
                                testHrns_passMessageUp( msg_index, 3);
                              }
                              return;
    case m_NVM_RESTORE_DEFAULT_SAS_REQUEST_c :
                              {
                                ZDOreadNVMdefaultSAS ( (p_buffer_rx + 3));
                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = m_NVM_RESTORE_DEFAULT_SAS_REQUEST_c;
                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
    case g_ACTIVATE_SAS_INDEX_c :
                              {
                                ZDOactivateSASindex(*(p_buffer_rx + 3 ));
                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_ACTIVATE_SAS_INDEX_c;
                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
#endif    /* ( g_COMMISSIONING_CAPABILITIES_d == 1 )  */
    case m_ZCL_BIND_REQUEST_c:

                              Create_ZDP_Request ( g_ZDP_Bind_Request_c,  msg_index );
                              return;

    case m_RESET_REQUEST_c:
#if( g_DIAGNOSTICS_CLUSTER_ENABLE_d ==1 )
                              app_reset_diagnostic_cluster_variables();
#endif
                              rsi_zigb_reset_stack();
                              App_Reset();


#if ( DEBUG_TEST == 1 )
                              memUtils_memSet( (uint8_t*)RejectionAddrList, 0xFF, 20);
#endif
                              *p_buffer_rx = msg_type + 1;
                              *(p_buffer_rx+1) = 0x00;
                              testHrns_passMessageUp ( msg_index, 2 );
                              return;

    case m_LOCAL_WRITE_ATTRIBUTE_REQUEST_c:
                              {
                                uint8_t attributeDataLen = 0;
                                uint8_t mAttributeData[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00};

                                endpoint = *( p_buffer_rx + 3 );
                                memUtils_memReverseCopy( (uint8_t*)&clusterId,   p_buffer_rx + 4, 0x02 );
                                memUtils_memReverseCopy( (uint8_t*)&attributeId, p_buffer_rx + 6, 0x02 );
                                dataType = *(p_buffer_rx + 8);

                                if( dataType < g_Octet_String_Data_c || dataType > g_Long_Character_String_Data_c){
                                  attributeDataLen = ZCL_Get_Data_length(dataType);
                                  memUtils_memReverseCopy((uint8_t*)&mAttributeData, (uint8_t*)( p_buffer_rx + 9), attributeDataLen);

                                }
                                else {
                                  attributeDataLen = *(p_buffer_rx + 9);
                                  memUtils_memCopy((uint8_t*)&mAttributeData, (uint8_t*)( p_buffer_rx + 9), attributeDataLen+1);
                                }

                                *(p_buffer_rx +1) = App_ReadWriteAttribute( endpoint,
                                    clusterId, attributeId, mAttributeData, g_Write_Attribute_Data_c );

                                *p_buffer_rx = m_SET_CONFIRM_c;
                                *( p_buffer_rx +2 ) = m_LOCAL_WRITE_ATTRIBUTE_CONFIRM_c;
                                testHrns_passMessageUp ( msg_index, 3 );
                              }
                              return;

#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
    case m_ADD_ZONE_ADDRESS_REQUEST_c:
                              *(p_buffer_rx + 1) =  App_Add_Zone_Address ( p_buffer_rx + 0x03 );
                              *p_buffer_rx = m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                              *(p_buffer_rx + 2) = m_ADD_ZONE_ADDRESS_CONFIRM_c;
                              testHrns_passMessageUp ( msg_index, 3 );
                              return;
#endif

    case g_Start_Dev_c:
                              *p_buffer_rx = g_Start_Device_Request_c;
                              break;

    case g_Permit_Join_Request_c:
                              {
                                ((NLME_Request_t*)p_buffer_rx)->primitive = g_NLME_Permit_Joining_Req_c;
                                ((NLME_Request_t*)p_buffer_rx)->NLME_Request_Primitive.permit_joining.permit_duration = *(p_buffer_rx + 1);
                              }
                              break;

    case g_Set_Index_For_Table_c :
                              m_index = *(p_buffer_rx + 3 );
                              *p_buffer_rx= m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                              *(p_buffer_rx + 2 ) = g_Set_Index_For_Table_Confirm_c;
                              testHrns_passMessageUp (msg_index, 3 );
                              return;

    case g_APSME_Add_Group_Request_c:
                              {
#if (g_APP_APS_GROUP_DATA_RX_d == 1)
                                uint8_t status;
                                memUtils_memCopy((uint8_t*) array, (p_buffer_rx + 1), 0x03);
                                status = APS_APSMEaddGroup((Add_Group_Request_t*) (p_buffer_rx + 1));
                                *p_buffer_rx = g_APSME_Add_Group_Confirm_c;
                                *(p_buffer_rx + 1) = status;
                                memUtils_memCopy((p_buffer_rx + 2), (uint8_t*) array, 3);
                                testHrns_passMessageUp(msg_index, 5);
#else
                                buffMgmt_freeBuffer(msg_index,241);
#endif                                           /* g_APP_APS_GROUP_DATA_RX_d */
                              }
                              return;
    case g_APSME_Remove_All_Groups_Request_c:
                              {
#if (g_APP_APS_GROUP_DATA_RX_d == 1)
                                uint8_t status;
                                array[0] = *(p_buffer_rx + 1);
                                status = APS_APSMEremoveAllGroups(*(p_buffer_rx + 1));
                                *p_buffer_rx = g_APSME_Remove_All_Groups_Confirm_c;
                                *(p_buffer_rx + 1) = status;
                                *(p_buffer_rx + 2) = array[0];
                                testHrns_passMessageUp(msg_index, 3);
#else
                                buffMgmt_freeBuffer(msg_index,243);
#endif          /* g_APP_APS_GROUP_DATA_RX_d */

                                return;
                              }
    case g_APSME_Remove_Group_Request_c:
                              {
#if (g_APP_APS_GROUP_DATA_RX_d == 1)
                                uint8_t status;
                                memUtils_memCopy((uint8_t*) array, (p_buffer_rx + 1), 0x03);
                                status = APS_APSMEremoveGroup(
                                    (Remove_Group_Request_t*) (p_buffer_rx + 1));
                                *p_buffer_rx = g_APSME_Remove_Group_Confirm_c;
                                *(p_buffer_rx + 1) = status;
                                memUtils_memCopy((p_buffer_rx + 2), (uint8_t*) array, 3);
                                testHrns_passMessageUp(msg_index, 5);
#else
                                buffMgmt_freeBuffer(msg_index,242);
#endif                                           /* g_APP_APS_GROUP_DATA_RX_d */
                                return;
                              }
    case g_Application_Poll_Request_c :
                              *p_buffer_rx = g_NLME_Sync_Req_c;
                              break;
                              /*************************************************/
#if ( DEBUG_TEST == 1 )
    case g_Disable_NT_Ageing_c:
                              {
                                g_NeighborTableAgeing = *( p_buffer_rx + 3 );
                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_Disable_NT_Ageing_c;
                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
    case g_Disable_RT_Ageing_c:
                              {
                                g_RouteTableAgeing = *( p_buffer_rx + 3 );
                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_Disable_RT_Ageing_c;
                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
    case g_setRejectDevice:
                              {
                                memUtils_memCopy( (uint8_t*)&RejectionAddrList[*(p_buffer_rx + 3)],
                                    (p_buffer_rx + 4), 2 );
                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                testHrns_passMessageUp(msg_index, 2);
                              }
                              return;
#endif
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
    case g_setPriceEvent_c:
                              {
                                App_updatePriceEvents( (p_buffer_rx +3) );

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_setPriceEvent_c;

                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
#if ( g_SE_PROFILE_1_1_d == 1 )
    case g_setBlockPeriodEvent_c:
                              {
                                App_updateBlockPeriodEvents( (p_buffer_rx +3) );

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_setBlockPeriodEvent_c;

                                testHrns_passMessageUp( msg_index, 3 );

                              }
                              return;
#endif /* ( g_SE_PROFILE_1_1_d == 1 ) */
#endif
#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
    case g_setDRLCEvent_c:
                              {
                                AppsetLoadControlEvent( *(p_buffer_rx +3), p_buffer_rx +4 );

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_setDRLCEvent_c;

                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
#endif
#if ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 )
    case g_set_MessageEvent_c:
                              {
                                //        uint32_t startTime;
                                //        uint8_t asdu[80];
                                //        uint8_t asduLength;

                                //        startTime = APP_ConvertYearToSeconds( (elapsedTimeInfo_t*)(p_buffer_rx +8) );
                                //        memUtils_memCopy( p_buffer_rx+ 8, (uint8_t*)&startTime, 0x04 );
                                //        memUtils_memCopy( p_buffer_rx+ 12, p_buffer_rx+14, *(p_buffer_rx+16) + 3 );

                                //        asduLength = zclCreateDisplayMessageCommand ( asdu,
                                //                                          (DisplayMessage_t*)(p_buffer_rx + 3),
                                //                                          App_GetTransSeqNumber() );

                                App_UpdateMessageTable( (DisplayMessage_t*)(p_buffer_rx +3), g_TRUE_c );

                                //        App_SendData ( g_Short_Address_Mode_c,
                                //                   0xFFFF,
                                //                   m_BROADCAST_END_POINT_c,
                                //                   g_MESSAGING_CLUSTER_c,
                                //                   asduLength,
                                //                   asdu );

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_set_MessageEvent_c;
                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
#endif
    case g_setSystemTime_c:
                              {
                                App_setRTCtime( (elapsedTimeInfo_t*)(p_buffer_rx +3) );

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_setSystemTime_c;

                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
    case g_setSystemUTCTime_c:
                              {
                                ZCL_callbackSetRTCtime( *((uint32_t*)(p_buffer_rx +3)) );

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_setSystemUTCTime_c;

                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
    case g_getSystemUTCTime_c:
                              {
                                *p_buffer_rx = m_SET_CONFIRM_c + 2;
                                *(p_buffer_rx + 1 ) = g_getSystemUTCTime_c;
                                *((uint32_t*)(p_buffer_rx+2)) = ZCL_CallBackGetCurrentTime();
                                testHrns_passMessageUp( msg_index, 6 );
                              }
                              return;
#if (( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 ) || ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ))
    case g_Set_EndPoint_c:
                              {
                                g_DstEndPoint_c = *(p_buffer_rx +3);

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_Set_EndPoint_c;

                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
#endif

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
    case g_DRLCOptInOrOptOut_c:
                              {
                                AppOptInOrOut( *((uint32_t*)(p_buffer_rx +3)), *(p_buffer_rx +7) );

                                *p_buffer_rx= m_SET_CONFIRM_c;
                                *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                                *(p_buffer_rx + 2 ) = g_DRLCOptInOrOptOut_c;

                                testHrns_passMessageUp( msg_index, 3 );
                              }
                              return;
#endif
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    case g_set_certificate_c :

                              App_SetCertificate( p_buffer_rx + 3 );
                              *p_buffer_rx= m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                              *(p_buffer_rx + 2 ) = g_SET_CERTIFICATE_c;

                              testHrns_passMessageUp( msg_index, 3 );
                              return;

    case g_set_ca_public_key_c :
                              App_SetCAPublicKey( p_buffer_rx + 3 );
                              *p_buffer_rx= m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                              *(p_buffer_rx + 2 ) = g_SET_CA_PUBLIC_KEY_c;
                              testHrns_passMessageUp( msg_index, 3 );
                              return;

    case g_set_static_private_key_c :
                              App_SetStaticPrivateKey( p_buffer_rx + 3 );
                              *p_buffer_rx= m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                              *(p_buffer_rx + 2 ) = g_SET_STATIC_PRIVATE_KEY_c;
                              testHrns_passMessageUp( msg_index, 3 );
                              return;

    case g_get_certificate_c :
                              *p_buffer_rx= m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                              memUtils_memCopy(p_buffer_rx+2, App_GetCertificate(),
                                  SECT163K1_CERTIFICATE_SIZE );
                              testHrns_passMessageUp( msg_index, SECT163K1_CERTIFICATE_SIZE+2 );
                              return;

    case g_get_ca_public_key_c :
                              *p_buffer_rx= m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                              memUtils_memCopy(p_buffer_rx+2,  App_GetCAPublicKey(),
                                  SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
                              testHrns_passMessageUp( msg_index, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE+2 );
                              return;

    case g_get_static_private_key_c :
                              *p_buffer_rx= m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1 ) = m_SUCCESS_c;
                              memUtils_memCopy(p_buffer_rx+2,  App_GetStaticPrivateKey(),
                                  SECT163K1_PRIVATE_KEY_SIZE );
                              testHrns_passMessageUp( msg_index, SECT163K1_PRIVATE_KEY_SIZE+2 );
                              return;
    case g_Issuer_Address_c:
                              memUtils_memCopy( &( App_GetCertificate()[ g_ISSUER_ADDRESS_LOCATION_c ] ),
                                  p_buffer_rx + 3, *( p_buffer_rx + 2) );
                              *p_buffer_rx = m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1) = g_ZCL_Success_c;
                              *(p_buffer_rx + 2) = g_Issuer_Address_c;
                              testHrns_passMessageUp( msg_index, 3 );
                              return;
    case g_read_Single_Byte_Of_Certificate_c:
                              m_index= *( p_buffer_rx +3);
                              memUtils_memCopy( &certificate_location, &( App_GetCertificate()[ m_index ] ),
                                  *( p_buffer_rx + 2) );
                              *p_buffer_rx = m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1) = g_ZCL_Success_c;
                              *(p_buffer_rx + 2) = g_read_Single_Byte_Of_Certificate_c;
                              testHrns_passMessageUp( msg_index, 3 );

                              return;
    case g_Write_Single_Byte_Of_Certificate_c:
                              m_index= *( p_buffer_rx +3);
                              memUtils_memCopy(&( App_GetCertificate()[ m_index ] ),&certificate_location,
                                  *( p_buffer_rx + 2) );
                              *p_buffer_rx = m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1) = g_ZCL_Success_c;
                              *(p_buffer_rx + 2) = g_Write_Single_Byte_Of_Certificate_c;
                              testHrns_passMessageUp( msg_index, 3 );

                              return;
    case g_Corrupt_Certificate_c:
                              memUtils_memCopy( &( App_GetCertificate()[ m_index ] ),
                                  p_buffer_rx + 3, *( p_buffer_rx + 2) );
                              *p_buffer_rx = m_SET_CONFIRM_c;
                              *(p_buffer_rx + 1) = g_ZCL_Success_c;
                              *(p_buffer_rx + 2) = g_Issuer_Address_c;
                              testHrns_passMessageUp( msg_index, 3 );
                              return;
#endif
#if( g_ZIGBEE_SECURITY_d == 1 )
    case g_APSME_Transport_Key_Request_c:
                              {
                                Transport_Key_Request_t tempTransReq;
                                memUtils_memCopy(tempTransReq.a_dest_address, p_buffer_rx + 1, 8);
                                tempTransReq.key_type = *(p_buffer_rx + 9);
                                tempTransReq.APSME_Transport_Key_Data.Network_Key_Data.key_seq_number
                                  = *(p_buffer_rx + 10);
                                memUtils_memCopy(
                                    tempTransReq.APSME_Transport_Key_Data.Network_Key_Data.a_network_key,
                                    p_buffer_rx + 11, 16);
                                tempTransReq.APSME_Transport_Key_Data.Network_Key_Data.use_parent
                                  = *(p_buffer_rx + 27);
                                memUtils_memCopy(p_buffer_rx + 1, (uint8_t *) &tempTransReq,
                                    sizeof(Transport_Key_Request_t));
                              }
                              break;
    case g_APSME_Remove_Device_Request_c:
                              {
                                Remove_Device_Request_t tempRemoveReq;
                                memUtils_memCopy(tempRemoveReq.a_parent_address, p_buffer_rx + 1, 8);
                                memUtils_memCopy(tempRemoveReq.a_target_address, p_buffer_rx + 9, 8);
                                memUtils_memCopy(p_buffer_rx + 1, (uint8_t *) &tempRemoveReq,
                                    sizeof(Remove_Device_Request_t));
                              }
                              break;
    case g_APSME_Switch_Key_Request_c:
                              {
                                Switch_Key_Request_t tempSwitchReq;
                                memUtils_memCopy(tempSwitchReq.a_dest_address, p_buffer_rx + 1, 8);
                                tempSwitchReq.key_seq_number = *(p_buffer_rx + 9);
                                memUtils_memCopy(p_buffer_rx + 1, (uint8_t *) &tempSwitchReq,
                                    sizeof(Switch_Key_Request_t));
                              }
                              break;
#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )

    case g_APSME_Request_Key_Request_c:
                              {
                                requestKeyRequest_t requestKeyRequest;
                                memUtils_memCopy(requestKeyRequest.aDestAddress, p_buffer_rx + 1, 8);
                                requestKeyRequest.keyType = *(p_buffer_rx + 9);
                                memUtils_memCopy(requestKeyRequest.aPartnerAddress, p_buffer_rx + 10, 8);
                                memUtils_memCopy(p_buffer_rx + 1, (uint8_t *) &requestKeyRequest,
                                    sizeof(requestKeyRequest_t));

                              }
                              break;
#endif                                        /* (g_APS_LAYER_SEC_PROCESSING_d == 1) */
#endif                                        /* ( g_ZIGBEE_SECURITY_d == 1 ) */
    case g_Restart_Dev_c:
                              {
                                *p_buffer_rx = g_ZDO_Restart_Request_c;
                              }
                              break;
    default:
                              {
                                convertedMsgtype = TH_convertNTSidsToZDPrequestIds(msg_type);
                                if (convertedMsgtype != 0xFF)
                                {
                                  if ((target_address[0] != 0xFA)) {
                                    memUtils_memRightShift((p_buffer_rx + 1),
                                        (g_Length_Of_Primitive - 1), 0x03);
                                    p_buffer_rx[2] = target_address[0];
                                    p_buffer_rx[3] = target_address[1];
                                    target_address[0] = 0xFA;
                                  }
                                  else {
                                    aDestAddr[0] = p_buffer_rx[1];
                                    aDestAddr[1] = p_buffer_rx[2];
                                    memUtils_memRightShift((p_buffer_rx + 1),(g_Length_Of_Primitive - 1), 0x01 );
                                    *((uint16_t*) (p_buffer_rx + 2)) = *((uint16_t*) aDestAddr);
                                  }
                                  *p_buffer_rx = convertedMsgtype;
                                  *(p_buffer_rx + 1) = 0x00;
                                }
                                else
                                {
                                  return;
                                }
                              }
                              break;
  }
  ZDO_AppZDPrequest(msg_index);
}
#endif

/*********************************************************************************************/
void Test_Main( void )
{
   uint8_t length = 0x02;
   uint8_t buffer_index = g_NO_AVAILABLE_BUFFER_c;
   uint8_t *p_buffer;

   if ( Is_Msg_Available(ZDP_APP_Queue) )
   {
      buffer_index = Dequeue ( ZDP_APP_Queue );

      if ( !Is_Msg_Available(ZDP_APP_Queue) ) {
      	//Stack_Events_ClearEvent(g_ZDP_APP_QUEUE_PENDING_c);
      }

      p_buffer = buffMgmt_getBufferPointer(buffer_index);
      *p_buffer = TH_getNTSid( *p_buffer );

      if( *p_buffer == g_Permit_Join_Confirm_c ) {
         length = 0x02;
         *( p_buffer + 1 ) = ((NLME_Response_t *)p_buffer)->
                             NLME_Response_Primitive.permit_joining_conf.status;
      }
      else if( *p_buffer == g_Start_Response_c )  {
        ZDP_Response_t *pStartDeviceResponse;
        uint8_t * p_buffer;

        pStartDeviceResponse = (ZDP_Response_t *)buffMgmt_getBufferPointer( buffer_index );
        p_buffer =  (uint8_t *)pStartDeviceResponse;
        *( p_buffer+ 1) = pStartDeviceResponse->ZDP_Response_Command.start_device_response.status;

         memUtils_memCopy( p_buffer+ 2,  (uint8_t*)&pStartDeviceResponse->ZDP_Response_Command.
                  start_device_response.Nwk_Pan_Id, 2 );

         memUtils_memCopy( p_buffer+ 4,  (uint8_t*)&pStartDeviceResponse->ZDP_Response_Command.
                  start_device_response.Nwk_Short_Address, 2 );

        *(p_buffer + 6) = pStartDeviceResponse->ZDP_Response_Command.start_device_response.
                                    logical_channel;
         length = 0x07;
      }
      else if( *p_buffer == g_NLME_Sync_Confirm_c  ) {
          length = 0x03;
      }
      else if( *p_buffer == m_ZCL_BIND_RESPONSE_c ) { //g_ZDP_Bind_Response_c ) {
		  ZDP_Response_t *pBindResponse;
          uint8_t *p_buffer;

		  pBindResponse = (ZDP_Response_t *)buffMgmt_getBufferPointer( buffer_index );
       	  p_buffer =  (uint8_t *)pBindResponse;
        *p_buffer = m_ZCL_BIND_RESPONSE_c;
		*( p_buffer+ 1) = pBindResponse->ZDP_Response_Command.bind_response.status;
		length = 2;
	  }
      else {
		 APSDE_Data_Indication_t *p_APSDE_Response;
			p_APSDE_Response
					= &(((APSDE_Response_t *) (p_buffer))-> APSDE_Response_Primitive.data_Indication);
			/*memUtils_memCopy(pDataResponse + 1, pDataResponse + 2,
					zdp_primitive_v.dataIndicationAsduLength + 1);*/
		  length =  p_APSDE_Response->asdulength;
		  if (g_Match_Desc_Response_c == *p_buffer) {
				length += 0x02;
		  }
		  else if( g_Simple_Desc_Response_c == *p_buffer) {

				length = *(p_buffer+5)+5;
		  }

//		  memUtils_memCopy(p_buffer + 1, p_buffer + 2,
//				   length + 1  );


		  memUtils_memCopy(p_buffer, p_buffer +1,
				   length + 1  );
      }
      testHrns_passMessageUp ( buffer_index, length );
    }
}

extern void send_response_to_client(unsigned char *bufferTx,int length);
/*********************************************************************************************/
void testHrns_passMessageUp (uint8_t bufferIndex, uint8_t length)
{
    uint8_t *bufferTx;

    bufferTx = buffMgmt_getBufferPointer (bufferIndex);

    memUtils_memRightShift (bufferTx, length, 1);

//    memUtils_memRightShift (bufferTx, length, 2);
//    *bufferTx = g_UART_TRANSMIT_c;
//    queueMgmt_enqueue(uart_getUartQueuePtr(), bufferIndex);
    *(bufferTx) = length;
#ifdef TEST_HARNESS
	  send_response_to_client(bufferTx,length+1);
#endif
    buffMgmt_freeBuffer(bufferIndex, 120);
}

/********************************************************************************************/

void Send_Response_To_Application ( APSDE_Data_Indication_t *p_buffer, uint8_t buffer_index )
{
     uint8_t * ptxbuffer;
     uint8_t buffindex;
     uint8_t indicationlength = 17;
     buffindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

     if ( buffindex != g_NO_AVAILABLE_BUFFER_c ) {

#if ( DEBUG_TEST == 1 )
	   /* Buffer ID --> 97 */
         allocatedbuffer[buffindex]= 97;
         freebuffer[buffindex][0]= 97;
         freebuffer[buffindex][1]= 0;
#endif
        ptxbuffer = buffMgmt_getBufferPointer( buffindex );
        *ptxbuffer = g_Data_Indication_c;
        *(ptxbuffer+1) = p_buffer->dest_addr_mode;
        memUtils_memCopy(ptxbuffer+2,(uint8_t*)&p_buffer->dest_address.short_address, 2 );
        *(ptxbuffer+4) = p_buffer->dest_endpoint;

         *(ptxbuffer+5) = p_buffer->src_addr_mode;

         if( p_buffer->src_addr_mode == g_SHORT_ADDR_MODE_c ) {
             memUtils_memCopy(ptxbuffer+6,(uint8_t *)&p_buffer->src_address.short_address,2);
             ptxbuffer = (ptxbuffer+6 ) + 2;
        }
        else if( p_buffer->src_addr_mode == g_EXTENDED_ADDR_MODE_c ) {
             memUtils_memCopy(ptxbuffer+6,(uint8_t*)p_buffer->src_address.IEEE_address,8);
             ptxbuffer = (ptxbuffer+6 ) + 8;
             indicationlength += 6;
        }
        *ptxbuffer = p_buffer->src_endpoint;
         ptxbuffer++;
         memUtils_memCopy( ptxbuffer,(uint8_t*)&(p_buffer->profile_id),2 );
         ptxbuffer +=2;
         memUtils_memCopy( ptxbuffer,(uint8_t*)&(p_buffer->cluster_id),2 );
         ptxbuffer +=2;
         *ptxbuffer = p_buffer->asdulength;
         memUtils_memCopy(ptxbuffer + 1, p_buffer->a_asdu,p_buffer->asdulength);
         ptxbuffer += *ptxbuffer + 1;
         *ptxbuffer = p_buffer->was_broadcast;
         *( ptxbuffer + 1 ) = p_buffer->security_status;
         *( ptxbuffer + 2 ) = p_buffer->link_quality;
         testHrns_passMessageUp ( buffindex, indicationlength + p_buffer->asdulength );
     }
}

/********************************************************************************************/
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void Send_InterPan_Response_To_Application ( APSDE_InterPan_Data_Indication_t* p_buffer,
                                             uint8_t buffer_index )
{
     uint8_t * ptxbuffer;
     uint8_t buffindex;
     uint8_t index = 0;

     buffindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
     if ( buffindex != g_NO_AVAILABLE_BUFFER_c ) {

#if ( DEBUG_TEST == 1 )
	   /* Buffer ID --> 96 */
         allocatedbuffer[buffindex]= 96;
         freebuffer[buffindex][0]= 96;
         freebuffer[buffindex][1]= 0;
#endif
         ptxbuffer = buffMgmt_getBufferPointer( buffindex );
         *(ptxbuffer + index) = g_InterPan_Data_Indication_c;
		 index++;
		 *(ptxbuffer + index) = p_buffer->srcaddrmode;
		 index++;
         memUtils_memCopy( ptxbuffer+index,(uint8_t*)&p_buffer->srcpanid, 2 );
		 index += 2;
         memUtils_memCopy(ptxbuffer+index,(uint8_t*)p_buffer->srcaddress.IEEE_address, 8 );
		 index += 8;
		 *(ptxbuffer + index) = p_buffer->dstaddrmode;
		 index++;
		 memUtils_memCopy( ptxbuffer+index,(uint8_t*)&p_buffer->dstpanid, 2 );
		 index += 2;
         if( p_buffer->dstaddrmode == g_EXTENDED_ADDR_MODE_c ) {
            memUtils_memCopy(ptxbuffer+index,(uint8_t*)p_buffer->dstAddress.IEEE_address,8);
			index += 8;
         }
		 else {
			memUtils_memCopy(ptxbuffer+index,(uint8_t*)p_buffer->dstAddress.IEEE_address,2);
			index += 2;
		 }
		 memUtils_memCopy( ptxbuffer+index,(uint8_t*)&p_buffer->profileId, 2 );
		 index += 2;
		 memUtils_memCopy( ptxbuffer+index,(uint8_t*)&p_buffer->clusterId, 2 );
		 index += 2;
		 memUtils_memCopy( ptxbuffer+index, &p_buffer->asdulength, 2 );
		 index += 2;
		 memUtils_memCopy(ptxbuffer+index, p_buffer->a_asdu,p_buffer->asdulength);
         index += p_buffer->asdulength;
         testHrns_passMessageUp ( buffindex, index);
     }
}
#endif
/********************************************************************************************/
void Send_Event_To_Application ( ZCL_Event_t * event )
{
     uint8_t * ptxbuffer;
     uint8_t buffindex;

     buffindex = buffMgmt_allocateBuffer ( g_SMALL_BUFFER_c );

     if ( buffindex != g_NO_AVAILABLE_BUFFER_c ) {

#if ( DEBUG_TEST == 1 )
	   /* Buffer ID --> 98 */
         allocatedbuffer[buffindex]= 98;
         freebuffer[buffindex][0]= 98;
         freebuffer[buffindex][1]= 0;
#endif
         ptxbuffer = buffMgmt_getBufferPointer( buffindex );
         *ptxbuffer = m_EVENT_c;
		 *(ptxbuffer + 1 ) = event->eventId;
		 *(ptxbuffer + 2 ) = event->eventLength;
		 memUtils_memCopy( ptxbuffer + 3, event->pEventData, event->eventLength );
         testHrns_passMessageUp ( buffindex, event->eventLength + 3 );
     }
}
/*---------------------------------------------------------------------------*/

#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )

void AppsetLoadControlEvent( uint8_t index, uint8_t * LCEData )
{
   // uint32_t startTime;
    LCEServerTableEntry_t * serverLCETable = AppgetLCETableEntry( index );

    if ( serverLCETable == g_NULL_c ) {
        return;
    }
    memUtils_memCopy( (uint8_t*)&serverLCETable->eventId, LCEData, 0x04 );
    LCEData += 4;
    memUtils_memCopy( (uint8_t*)&serverLCETable->deviceClass, LCEData, 0x02 );
    LCEData += 2;
    serverLCETable->utilityEnrollmentGroup = *LCEData;
    LCEData += 1;
//    startTime = APP_ConvertYearToSeconds( (elapsedTimeInfo_t*)LCEData );
    //startTime = ((uint32_t*)LCEData)
    memUtils_memCopy( (uint8_t*)&serverLCETable->startTime, (uint8_t*)LCEData, 0x04 );
    LCEData += 4;
    memUtils_memCopy( (uint8_t*)&serverLCETable->duration, LCEData, 0x02 );
    LCEData += 2;
    serverLCETable->criticalityLevel = *LCEData;
    LCEData += 1;
    serverLCETable->coolingTempOffset = *LCEData;
    LCEData += 1;
    serverLCETable->heatingTempOffset = *LCEData;
    LCEData += 1;
    memUtils_memCopy( (uint8_t*)&serverLCETable->coolingTempSetPoint, LCEData, 0x02 );
    LCEData += 2;
    memUtils_memCopy( (uint8_t*)&serverLCETable->heatingTempSetPoint, LCEData, 0x02 );
    LCEData += 2;
    serverLCETable->avgLoadPercentage = *LCEData;
    LCEData += 1;
    serverLCETable->dutyCycle = *LCEData;
    LCEData += 1;
    serverLCETable->eventControl = *LCEData;
    LCEData += 1;
    if ( ! serverLCETable->startTime ) {
        serverLCETable->startTime = ZCL_CallBackGetCurrentTime();
    }
   // memUtils_memCopy( (uint8_t*)&serverLCETable->startTime, (uint8_t*)&startTime, 0x04 );
}
#endif          /* ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 ) */

/**************************************************************************************/
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )

void App_SetPriceTable ( uint8_t* pPriceData )
{
	publishPriceInfo_t  priceTable;
	memUtils_memSet((uint8_t*) &priceTable, g_ZERO_c,
                            sizeof(publishPriceInfo_t));

    memUtils_memReverseCopy( (uint8_t*)&priceTable.providerId, pPriceData, 0x04 );
    pPriceData += 0x04;
    memUtils_memCopy( priceTable.rateLabel, pPriceData, *pPriceData + 1  );
    pPriceData += *pPriceData + 1;
    memUtils_memReverseCopy( (uint8_t*)&priceTable.issuerEventId, pPriceData, 0x04 );
    pPriceData += 0x04;
    memUtils_memReverseCopy( (uint8_t*)&priceTable.currentTime, pPriceData, 0x04 );
    pPriceData += 0x04;
    priceTable.unitOfMeasure = *(pPriceData);
    pPriceData += 0x01;
    memUtils_memCopy( (uint8_t*)&priceTable.currency, pPriceData, 0x02 );
    pPriceData += 0x02;
    priceTable.priceTrailingDigitAndPriceTier = *(pPriceData);
    pPriceData += 0x01;
    priceTable.numOfPriceTiersAndRegisterTier = *(pPriceData);
    pPriceData += 0x01;
    memUtils_memReverseCopy( (uint8_t*)&priceTable.startTime, (uint8_t*)pPriceData, 0x04 );
    pPriceData += 0x04;
    memUtils_memReverseCopy( (uint8_t*)&priceTable.duration, pPriceData, 0x02 );
    pPriceData += 0x02;
    memUtils_memReverseCopy( (uint8_t*)&priceTable.price, pPriceData, 0x04 );
    pPriceData += 0x04;
    priceTable.priceRatio = *pPriceData;
    pPriceData += 0x01;
    memUtils_memReverseCopy( (uint8_t*)&priceTable.generationPrice, pPriceData, 0x04 );
    pPriceData += 0x04;
    priceTable.generationPriceRatio = *(pPriceData);
    pPriceData += 0x01;
    memUtils_memReverseCopy( (uint8_t*)&priceTable.alternateCostDelivered, pPriceData, 0x04 );
    pPriceData += 0x04;
    priceTable.alternateCostUnit= *(pPriceData);
    pPriceData += 0x01;
    priceTable.alternateCostTrailingDigit= *(pPriceData);
	pPriceData += 0x01;
#if ( g_SE_PROFILE_1_1_d == 1 )
	priceTable.numberOfBlockThresholds= *(pPriceData);
	pPriceData += 0x01;

	if(*(pPriceData)==0xFF) {
		priceTable.priceControl= 0x00;
		pPriceData += 0x01;
	}else {
		priceTable.priceControl= *(pPriceData);
		pPriceData += 0x01;
	}
#endif

    if ( ! priceTable.startTime ) {
    	priceTable.startTime = ZCL_CallBackGetCurrentTime();
    }
    App_AddEntryToPriceTable(&priceTable);

}
/**************************************************************************************/
void App_updatePriceEvents ( uint8_t* pPriceData )
{
    scheduledPublishPriceInfo_t  pPriceTable;
	memUtils_memSet((uint8_t*) &pPriceTable, g_ZERO_c,
                            sizeof(scheduledPublishPriceInfo_t));
    memUtils_memReverseCopy( (uint8_t*)&pPriceTable.providerId, pPriceData, 0x04 );
    pPriceData += 0x04;
    memUtils_memCopy( pPriceTable.rateLabel, pPriceData, *pPriceData + 1  );
    pPriceData += *pPriceData + 1;
    memUtils_memReverseCopy( (uint8_t*)&pPriceTable.issuerEventId, pPriceData, 0x04 );
    pPriceData += 0x04;
    memUtils_memReverseCopy( (uint8_t*)&pPriceTable.currentTime, pPriceData, 0x04 );
    pPriceData += 0x04;
    pPriceTable.unitOfMeasure = *(pPriceData);
    pPriceData += 0x01;
    memUtils_memCopy( (uint8_t*)&pPriceTable.currency, pPriceData, 0x02 );
    pPriceData += 0x02;
    pPriceTable.priceTrailingDigitAndPriceTier = *(pPriceData);
    pPriceData += 0x01;
    pPriceTable.numOfPriceTiersAndRegisterTier = *(pPriceData);
    pPriceData += 0x01;
    memUtils_memReverseCopy( (uint8_t*)&pPriceTable.startTime, (uint8_t*)pPriceData, 0x04 );
    pPriceData += 0x04;
    memUtils_memReverseCopy( (uint8_t*)&pPriceTable.duration, pPriceData, 0x02 );
    pPriceData += 0x02;
    memUtils_memReverseCopy( (uint8_t*)&pPriceTable.price, pPriceData, 0x04 );
    pPriceData += 0x04;
    pPriceTable.priceRatio = *pPriceData;
    pPriceData += 0x01;
    memUtils_memReverseCopy( (uint8_t*)&pPriceTable.generationPrice, pPriceData, 0x04 );
    pPriceData += 0x04;
    pPriceTable.generationPriceRatio = *(pPriceData);
    pPriceData += 0x01;
    memUtils_memReverseCopy( (uint8_t*)&pPriceTable.alternateCostDelivered, pPriceData, 0x04 );
    pPriceData += 0x04;
    pPriceTable.alternateCostUnit= *(pPriceData);
    pPriceData += 0x01;
    pPriceTable.alternateCostTrailingDigit= *(pPriceData);
	pPriceData += 0x01;
#if ( g_SE_PROFILE_1_1_d == 1 )
    pPriceTable.numberOfBlockThresholds= *(pPriceData);
	pPriceData += 0x01;

	if(*(pPriceData)==0xFF) {
		pPriceTable.priceControl= 0x00;
		pPriceData += 0x01;
	}else {
		pPriceTable.priceControl= *(pPriceData);
		pPriceData += 0x01;
	}
	memUtils_memCopy( (uint8_t*)&pPriceTable.endTime, 0x00, 0x04 );
    pPriceData += 0x04;
    pPriceTable.valid= g_TRUE_c;
    pPriceTable.active= g_FALSE_c;
#endif

    if ( ! pPriceTable.startTime ) {
        pPriceTable.startTime = ZCL_CallBackGetCurrentTime();
    }
	App_Update_PriceEventList(&pPriceTable);

}

/**************************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )
void App_updateBlockPeriodEvents ( uint8_t* pBlockData )
{
	scheduledPublishBlockPeriodInfo_t pBlockTable;

	memUtils_memReverseCopy( (uint8_t*)&pBlockTable.providerId, pBlockData, 0x04 );
    pBlockData += 0x04;
    memUtils_memReverseCopy( (uint8_t*)&pBlockTable.issuerEventId, pBlockData, 0x04 );
    pBlockData += 0x04;
    memUtils_memReverseCopy( (uint8_t*)&pBlockTable.blockPeriodStartTime, pBlockData, 0x04 );
    pBlockData += 0x04;
    memUtils_memReverseCopy( (uint8_t*)&pBlockTable.blockPeriodDurationInMinutes, pBlockData, 0x03 );
    pBlockData += 0x03;
    pBlockTable.numberOfPriceTiersAndBlockThresholds= *(pBlockData);
	pBlockData += 0x01;
    pBlockTable.blockPeriodControl= *(pBlockData);
	pBlockData += 0x01;
	memUtils_memReverseCopy( (uint8_t*)&pBlockTable.endTime, 0x00, 0x04 );
    pBlockData += 0x04;
    pBlockTable.valid= g_TRUE_c;
    pBlockTable.active= g_FALSE_c;
	App_Init_BlockEventList (m_index, &pBlockTable);
}
#endif  /* ( g_SE_PROFILE_1_1_d == 1 ) */

#endif          /* ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ) */

/**************************************************************************************/

void AppIncomingManyToOneRouteRequestHandler( uint16_t SourceAddr,
     uint8_t * pSrcIEEEAddr,uint8_t PathCost )
{
    return;
}

/**************************************************************************************/
bool ZigBeeSwitchNetworkKeyHandler( uint8_t sequenceNumber )
{
    return ((g_ENTRY_NOT_FOUND_c == sequenceNumber)? g_FALSE_c:g_TRUE_c);
}

/**************************************************************************************/
void AppEnergyScanResultHandler( uint32_t channel, uint8_t *maxRSSIValue)
{

}

/**************************************************************************************/
void AppScanCompleteHandler ( uint32_t channel, uint8_t status )
{
    /* This function will be handled in application layer*/
#if(g_APP_PROFILE_ID_c == 0xc05e)
  if( status == g_MAC_No_Beacon_c)
  {
    ZigBeeNetworkDetails dummyNetworkInformation;
    ZLL_Update_Scan_ConfirmationDetails( dummyNetworkInformation,g_MAC_No_Beacon_c  );
  }
#endif //#if #if(g_APP_PROFILE_ID_c == g_ZLL_PROFILE_ID_c)
}

/***************************************************************************/
void AppChildJoinHandler( uint16_t shortAddress, bool joining)
{

}

/***************************************************************************/
void AppNetworkFoundHandler(ZigBeeNetworkDetails networkInformation)
{
    /* This function will be handled in application layer*/
#if(g_APP_PROFILE_ID_c == 0xc05e)
  ZLL_Update_Scan_ConfirmationDetails( networkInformation,g_MAC_Success_c  );
#endif //#if(g_APP_PROFILE_ID_c == g_ZLL_PROFILE_ID_c)

}
/******************************************************************************/

void pollDataRequestHandler(uint8_t* ptr)
{

    /*This function will be handled in application layer*/
}
#endif
/***************************************************************************/
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
