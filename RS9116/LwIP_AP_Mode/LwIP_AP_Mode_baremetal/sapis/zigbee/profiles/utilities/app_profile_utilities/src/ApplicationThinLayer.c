/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include"ZigBeeDataTypes.h"
//#include "stack_events.h"
#include <stdio.h>
//#include "stack_common.h"
#include "common_includes.h"
#include "buffer_management.h"
#include "queue.h"
#include "ZCL_Common.h"
#include "stack_functionality.h"
#include "zdo_interface.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#include "stack_util.h"
#include "Stack_Info.h"
#include "memory_utility.h"
#include "sw_timer.h"
#include "ZCL_Interface.h"
#include "AppTestHarness.h"
#include "ApplicationThinLayer.h"
#include "ZCL_Stack_Utility.h"
#include "AppTimerUtilities.h"
#include "AppDiscoveryUtilities.h"
#include "app_configuration_default.h"
#include "zdo_interface.h"
#include "ZLL_defines.h"
#include "ZLL_Network.h"
#ifdef g_ZLL_LIGHT_DEVICE_c
#include "ZLL_Light_Application_Framework.h"
#endif
#ifdef g_ZLL_LIGHT_CONTROLLER_c
#include "ZLL_Controller_Application_Framework.h"
#endif

#if ( g_REPORTING_ENABLE_d == 1 )
    #include "AppReportingUtilities.h"
#endif /*g_REPORTING_ENABLE_d*/
#if( g_IDENTIFY_CLUSTER_ENABLE_d ==1 )
    #include "AppIdentifyUtilities.h"
#endif /*g_IDENTIFY_CLUSTER_ENABLE_d*/
#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
    #include "AppScenesUtilities.h"
#endif /*g_SCENES_CLUSTER_ENABLE_d*/
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
    #include "AppAlarmsUtilities.h"
#endif /*g_ALARMS_CLUSTER_ENABLE_d*/
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
    #include "AppMessageUtilities.h"
#endif /*g_MESSAGING_CLUSTER_ENABLE_d*/
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Price_Cluster.h"
    #include "AppPriceClusterUtilities.h"
#endif /*g_PRICE_CLUSTER_ENABLE_d*/
#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_DRLC_Cluster.h"
    #include "AppDRLCUtilities.h"
#endif /*g_DRLC_CLUSTER_ENABLE_d*/
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Simple_Metering_Cluster.h"
    #include "AppSimpleMeterUtilities.h"
#endif /*g_SIMPLE_METERING_CLUSTER_ENABLE_d*/
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1)
	#include "AppZoneTableUtilities.h"
#endif /*g_IAS_ZONE_CLUSTER_ENABLE_d*/
#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Time_Cluster.h"
#endif /*g_TIME_CLUSTER_ENABLE_d*/
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
	#include "ZCL_Key_Establishment.h"
    #include "AppCBKEUtilities.h"
     #include "ZCL_Key_Establishment.h"
#endif /*g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d*/
#if ( g_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Commissioning_Cluster.h"
#endif /*g_COMMISSIONING_CLUSTER_ENABLE_d*/
 #if(g_TUNNELING_CLUSTER_ENABLE_d == 1)
    #include "ZCL_Tunneling_Cluster.h"
    #include "AppTunnelingClusterUtilities.h"
#endif /*g_TUNNELING_CLUSTER_ENABLE_d*/
#if(g_PREPAYMENT_CLUSTER_ENABLE_d == 1)
    #include "ZCL_Prepayment_Cluster.h"
    #include "AppPrepaymentUtilities.h"
#endif /*g_PREPAYMENT_CLUSTER_ENABLE_d*/
#if( g_INTERPAN_DATA_ENABLE_d == 1 )
    #include "aps_interpan_stub.h"
#endif /*g_INTERPAN_DATA_ENABLE_d*/
#if ( g_OTA_BOOTLOAD_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_OTA_Upgrade_Cluster.h"
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
    #include "AppClientBootloader.h"
#endif /*g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d*/
#if (g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1)
    #include "AppServerBootloader.h"
#endif /*g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d*/
#endif /*g_OTA_BOOTLOAD_CLUSTER_ENABLE_d*/
#if( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
	#include "AppGenericTunnelClusterUtilities.h"
	#include "ZCL_Generic_Tunnel_Cluster.h"
#endif /*g_GENERIC_TUNNEL_CLUSTER_ENABLE_d*/
#if(g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1) &&  ( g_ZIGBEE_ROUTER_d == 1 )
    #include "AppTrustCenterKeepAliveUtilities.h"
#endif /**/

#if(g_HA_1_2_d == 1)
#if(g_APPLIANCE_STATISTICS_CLUSTER_ENABLE_d == 1)
    #include "ZCL_HA_Appliance_Statistics_Cluster.h"
    #include "AppApplianceStatisticUtilities.h"
#endif

#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Appliance_Control_Cluster.h"
#include "AppApplianceControlUtilities.h"
#endif /* (g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)*/

#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1)
    #include "ZCL_HA_Appliance_Events_And_Alert_Cluster.h"
    #include "AppApplianceEventsAndAlertUtilities.h"

#endif /* (g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1)*/

#if(g_POWER_PROFILE_CLUSTER_ENABLE_d == 1)
    #include "ZCL_General_Power_Profile_Cluster.h"
    #include "AppPowerProfileUtilities.h"
#endif /* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1*/

#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Thermostat_Cluster.h"
#include "AppThermostatUtilities.h"
#endif  /* g_THERMOSTAT_CLUSTER_ENABLE_d == 1*/

#if( g_DIAGNOSTICS_CLUSTER_ENABLE_d ==1 )
#include "ZCL_HA_Diagnostics_Cluster.h"
#endif  /* g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1*/


#if(g_POLL_CONTROL_CLUSTER_ENABLE_d == 1)
    #include "ZCL_General_Poll_Control_Cluster.h"
    #include "AppPollControlUtilities.h"
#endif

#if(g_EN50523_APPLIANCE_CONTROL_CLUSTER_ENABLE_d == 1)
    #include "ZCL_General_Appliance_Control_Cluster.h"
    #include "AppApplianceControlUtilities.h"
#endif


#endif/*g_HA_1_2_d */

/*For ZLL Profile */
#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZLL_State_Machine.h"
#include "ZLL_NVM_Utilities.h"
#include "ZLL_Commissioning_Cluster.h"
#include "AppZllCommissioningClusterUtilities.h"
#endif /* #if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )*/
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_IAS_ZONE_CLUSTER_c                                    0x0500
#define m_SHIFT_TO_GET_HIGH_BYTE                                0x08
#define m_RESPONSE_FRAME_c                                      0x80
#define m_DRLC_CLUSTER_c										0x0701
#define g_Zone_IEEE_Addr_Response_c								0x8001

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if( ( g_APP_PROFILE_ID_c == 0x0109 ) || ( g_APP_PROFILE_ID_c == 0xc05e ))
clusterSpecificSecurity_t aclusterSpecificSecurityTable[] =
{
#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
    { g_TIME_CLUSTER_c, g_HIGH_SECURITY_c },
#endif      /* ( g_TIME_CLUSTER_ENABLE_d == 1 ) */
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
    { g_PRICE_CLUSTER_c, g_HIGH_SECURITY_c },
#endif
#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
    { g_DRLC_CLUSTER_c, g_HIGH_SECURITY_c },
#endif
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
    { g_SIMPLE_METERING_CLUSTER_c, g_HIGH_SECURITY_c },
#endif
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
    { g_MESSAGING_CLUSTER_c, g_HIGH_SECURITY_c },
#endif
#if ( g_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
    { g_COMMISSIONING_CLUSTER_c, g_HIGH_SECURITY_c } ,
#endif
#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
    { g_TUNNELING_CLUSTER_c, g_HIGH_SECURITY_c } ,
#endif
#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
    { g_PREPAYMENT_CLUSTER_c, g_HIGH_SECURITY_c } ,
#endif
#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
    { g_GENERIC_TUNNEL_CLUSTER_c, g_HIGH_SECURITY_c } ,
#endif
#if ( g_OTA_BOOTLOAD_CLUSTER_ENABLE_d == 1 )
    { g_OTA_UPGRADE_CLUSTER_c, g_HIGH_SECURITY_c } ,
#endif
#if (g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
    { g_ZLL_COMMISSIONING_CLUSTER_c, g_HIGH_SECURITY_c } ,
#endif /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/
    {0xFFFF, 0xFF}

};
#endif      /* ( g_APP_PROFILE_ID_c == 0x0104 ) || ( g_APP_PROFILE_ID_c == 0xc05e )) */

queue_t ZDP_APP_Queue;
uint8_t g_Device_EndPoint = 0x01;
uint8_t g_Device_IEEE_Address[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t g_requestTransSeqNum = 0x00;
uint8_t g_Device_Address_Mode = 0x00;
uint16_t g_Device_Short_Address = 0xFFFF;
uint8_t g_Src_EndPoint = 0xFF;

uint8_t target_address[2];

//#ifndef HA_SA
#if 1
extern  const ZCL_Device_Def_t AppDeviceListServerInfo[];
extern  const ZCL_Device_Def_t AppDeviceListClientInfo[];
#endif

extern const uint8_t g_Small_Buffer_Size_c;
extern const uint8_t g_Large_Buffer_Size_c;

//extern uint8_t g_Number_Of_End_Points_c;

/* Extern declaration of pointers for ZDO Information Base */
ZDO_Information_Base_t *gp_ZDO_Information_Base;
#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

//#if(APS_FRAGMENTATION_CAPABILITY == RSI_ENABLE)
//    const uint8_t fragmentationID= 0xFA;
//    uint8_t *startBufferID;
//#endif 			/*APS_FRAGMENTATION_CAPABILITY*/


#if(g_COMMISSIONING_CLUSTER_ENABLE_d == 1)

extern bool LeaveConfFlagForCommission;
#endif

#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
extern bool isDeviceJoindOrRejoined ;
#endif

#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
extern bool isDeviceJoindOrRejoined;
#endif
#if ((g_CLI_SUPPORT_d == 1 ) && ( g_APP_PROFILE_ID_c ==0x0109))
bool zclApsSecurityOff = g_TRUE_c ;
#elif(g_APP_PROFILE_ID_c ==0xc05e)
bool zclApsSecurityOff = g_TRUE_c ;
#else
bool zclApsSecurityOff = g_FALSE_c;
#endif

#if (g_CLI_SUPPORT_d == 1)
bool prtRecMsg = g_FALSE_c;
#endif

#if(g_HA_1_2_d == 1)

#if( g_DIAGNOSTICS_CLUSTER_ENABLE_d ==1 )

uint8_t m_nwk_decrypt_failure_count =0x00;
uint8_t m_aps_decrypt_failure_count  =0x00;
uint8_t m_packet_buffer_allocate_failure_count  =0x00;
uint8_t m_relayed_ucast_count  =0x00;
uint8_t m_phy_to_mac_queue_lmit_count =0x00;
uint8_t m_packet_validate_drop_count  =0x00;
uint8_t m_unauthorised_key_count =0x00;
uint8_t m_apsfc_failure_count   =0x00;
uint8_t m_nwkfc_failure_count =0x00;
uint8_t m_child_move_count  =0x00;
uint8_t m_neighbor_removed_count  =0x00;
uint8_t m_neighbor_stale_count   =0x00;
uint8_t m_child_join_indication_count   =0x00;
uint8_t m_neighbor_added_count =0x00;
uint8_t m_route_disc_initiated_count  =0x00;
uint8_t m_aps_trx_unicast_failure_count =0x00;
uint8_t m_aps_trx_unicast_success_count  =0x00;
uint8_t m_aps_trx_unicast_retry_count =0x00;
uint8_t m_no_of_resets_count  =0x00;
uint8_t m_persistent_memory_writes_count=0x00;
uint8_t m_mac_rec_broadcast_count =0x00;
uint8_t m_mac_trx_broadcast_count =0x00;
uint8_t m_mac_rec_unicast_count =0x00;
uint8_t m_mac_trx_unicast_count =0x00;
uint8_t m_mac_trx_unicast_retry_count =0x00;
uint8_t m_mac_trx_unicast_failure_count =0x00;
uint8_t m_aps_rec_broadcast_count =0x00;
uint8_t m_aps_rec_unicast_count =0x00;
uint8_t m_aps_trx_broadcast_count=0x00;


void app_writeDiagnosticAtrribute(uint8_t attributeValue, uint16_t attributeID);


#endif
#endif

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
uint8_t g_IAS_Zone_State = g_Ready_State_c;

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
 static void Create_Enroll_Response_Command ( void );
#endif
uint8_t App_getClusterSecurity( uint16_t clusterId );

#ifdef HA_SA
extern void HA_SampleApp_Init(void);
#endif

uint8_t APS_APSDE_request(uint8_t messageIndex,APSDE_Data_Request_t *p_apsde_data_request );
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Callback_Get_Device_EndPoint( void )
{
    return g_Device_EndPoint;
}

/**************************************************************************************/
uint8_t App_GetTransSeqNumber( void )
{
    return g_requestTransSeqNum++;
}

/**************************************************************************************/
uint8_t* ZCL_Callback_Get_Device_IEEEAddress( void )
{
    return g_Device_IEEE_Address;
}
/**************************************************************************************/
uint16_t ZCL_Callback_Get_Device_ShortAddress( void )
{
    return g_Device_Short_Address;
}
/**************************************************************************************/
uint8_t ZCL_Callback_Get_Device_AddressMode( void )
{
    return g_Device_Address_Mode;
}
/**************************************************************************************/
uint8_t ZCL_Callback_Get_Device_Src_EndPoint( void )
{
    return g_Src_EndPoint;
}
/**************************************************************************************/

void App_SendDefaultResponse (  uint8_t status,
                                uint8_t NwkSecurityUse,
                                APSDE_Data_Indication_t* p_dataInd )
{
    APSDE_Request_t* pDataReq;
    APSDE_Data_Request_t * pAppDataReq;
    uint8_t buffindex;
    /*dont return default response if the message is a broadcast packet*/
    if(( p_dataInd->dest_addr_mode == g_SHORT_ADDR_MODE_c ) && ( p_dataInd->dest_address.short_address == g_BROADCAST_ADDRESS_c ))
    {
      return;
    }

    buffindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

    if( buffindex != g_INVALID_INDEX_c ) {

#if ( DEBUG_TEST == 1 )
	  /* Buffer ID --> 91 */
        allocatedbuffer[buffindex]= 91;
        freebuffer[buffindex][0]= 91;
        freebuffer[buffindex][1]= 0;
#endif

        pDataReq = (APSDE_Request_t *)buffMgmt_getBufferPointer( buffindex );
        pDataReq->msgType = g_APSDE_Data_Request_c;
        pAppDataReq = (APSDE_Data_Request_t *)&((pDataReq)->APSDE_Request_Primitive.apsde_data_request);

        pAppDataReq->asdulength =   ZCL_CreateDefaultResponseCommand ( p_dataInd->a_asdu,
                                                        pAppDataReq->asdu,
                                                        status );

        pAppDataReq->dest_addr_mode = p_dataInd->src_addr_mode;

        if( pAppDataReq->dest_addr_mode == g_Short_Address_Mode_c ) {
            memUtils_memCopy ( (uint8_t *)&(pAppDataReq->dest_address.short_address),
                      ( uint8_t *)&(p_dataInd->src_address.short_address), g_SHORT_ADDRESS_LENGTH_c );
        }
        if( pAppDataReq->dest_addr_mode == g_IEEE_Address_Mode_c ) {
            memUtils_memCopy ( pAppDataReq->dest_address.IEEE_address,
                      p_dataInd->src_address.IEEE_address, g_EXTENDED_ADDRESS_LENGTH_c );
        }
        pAppDataReq->dest_endpoint = p_dataInd->src_endpoint;
        pAppDataReq->profile_id    = p_dataInd->profile_id;
        pAppDataReq->cluster_id    = p_dataInd->cluster_id;
        pAppDataReq->src_endpoint  = p_dataInd->dest_endpoint;
        /* For cluster specific secutiry failure, Default response sent with
           Nwk Security and other cases cluster specific sucurity */
        if ( NwkSecurityUse == g_TRUE_c ) {
            pAppDataReq->txoptions = g_NETWORK_SECURITY_c;
        }
        else {
            pAppDataReq->txoptions = App_getClusterSecurity( pAppDataReq->cluster_id );
        }
        pAppDataReq->radius = g_DEFAULT_RADIUS_c;

        //APS_APSDErequest( buffindex );
        APS_APSDE_request(buffindex,pAppDataReq );
    }
}

/*****************************************************************************/

uint8_t App_getAPSMaxPayloadForDevice (uint16_t clusterId, Address *pAddress,
                                       uint8_t addr_mode)
{
    uint8_t length = 0;
#if ( ( g_ZIGBEE_COORDINATOR_d == 1 ) || ( g_ZIGBEE_ROUTER_d == 1 ) )
    Route_Record_Table_t *pTableEntry;
#endif
    uint16_t shortAddress = pAddress->short_address;
    /* get the max APS payload length */
    //length = ZigBeeGetMaxAPSPayloadLength();
    length = rsi_zigb_get_max_aps_payload_length();
    /* if addressing mode is extended */
    if (addr_mode == g_EXTENDED_ADDR_MODE_c) {
        /* get the short address for the extended address */
       //shortAddress = ZigBeeGetShortAddrForSpecifiedIEEEAddr(pAddress->IEEE_address);
        shortAddress = rsi_zigb_get_short_addr_for_specified_ieee_addr(pAddress->IEEE_address);
    }
#if ( ( g_ZIGBEE_COORDINATOR_d == 1 ) || ( g_ZIGBEE_ROUTER_d == 1 ) )
#if(g_NWK_MANY_TO_ONE_ROUTING_d == 1 )
#if (g_NWK_ROUTE_RECORD_TABLE_d == 1)

    /* get the route record table entry for the device */
    pTableEntry = (Route_Record_Table_t *)ZigBeeGetRouteRecordForDevice(shortAddress);
    /* route record table entry is available */
    if (pTableEntry) {
        /* then calculate the length for the based on the relay count */
        //length += ((m_GET_g_NWK_MaxDepth_c - pTableEntry->relayCount) * sizeof(shortAddress));
        length += ((Get_Nwk_MaxDepth() - pTableEntry->relayCount) * sizeof(shortAddress));
    }
#endif //#if (g_NWK_ROUTE_RECORD_TABLE_d == 1)
#endif
#endif

    /* return the max payload length */
    return length;
}


/*****************************************************************************/

uint8_t App_getClusterSecurity( uint16_t clusterId )
{
#if ( g_APP_PROFILE_ID_c == 0x0109 ||  g_APP_PROFILE_ID_c == 0xc05e)
    uint8_t i;
    for (i = 0; i < sizeof( aclusterSpecificSecurityTable )/sizeof( clusterSpecificSecurity_t )
          ; i++ ) {
        if( aclusterSpecificSecurityTable[i].clusterID == clusterId ) {
#if (g_CLI_SUPPORT_d == 1)
			if(zclApsSecurityOff == g_FALSE_c){
			return g_NO_SECURITY_c;
			}
#endif
            return aclusterSpecificSecurityTable[i].security;
        }
    }
    return g_NETWORK_SECURITY_c;
#else
    return g_NETWORK_SECURITY_c;//TODO
    //return m_GET_g_NWK_SecurityLevel_c ? g_NETWORK_SECURITY_c : g_NO_SECURITY_c;
    return Get_Nwk_SecurityLevel() ? g_NETWORK_SECURITY_c : g_NO_SECURITY_c;
#endif    /* ( g_APP_PROFILE_ID_c == 0x0109 ) */

}

/**************************************************************************************/
uint8_t App_Get_Cluster_Definition (  uint8_t direction,
                                      uint8_t endpointId,
                                      uint16_t clusterId,
                                      App_Cluster_Def_t ** pZCLClusterDefinition )
{
    uint8_t epindex, clstrindex;
    ZCL_Device_Def_t const *pDeviceInfo;

    for ( epindex = 0; epindex < g_MAX_NO_OF_ENDPOINTS_c ; epindex++ ){
        if ( direction == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) {
            pDeviceInfo = &AppDeviceListServerInfo[epindex];
        }
        else {
            pDeviceInfo = &AppDeviceListClientInfo[epindex];
        }
        if ( endpointId == pDeviceInfo->endpoint  ){
            for ( clstrindex = 0; clstrindex < pDeviceInfo->num_clust_defs_present; clstrindex++ ){
                if ( clusterId == pDeviceInfo->p_list_of_cluster_def_ptrs[clstrindex].p_cluster_info->cluster_id ) {
                    *pZCLClusterDefinition = (App_Cluster_Def_t *)pDeviceInfo->p_list_of_cluster_def_ptrs + clstrindex;
                    return g_ZCL_Success_c;
                }
            }
            break;
        }
    }
    return g_ZCL_Failure_c;
}


/**************************************************************************************/

uint8_t* App_Get_AttrDataLocation ( uint8_t frame_direction,
                                    uint8_t endpoint_id,
                                    uint16_t cluster_id,
                                    App_Cluster_Def_t ** pZCLClusterDefinition )
{

   if ( g_ZCL_Success_c == ( App_Get_Cluster_Definition ( frame_direction, endpoint_id, cluster_id,
        pZCLClusterDefinition ) ) ) {
        return  (uint8_t*)(*pZCLClusterDefinition)->p_attrib_data_list;

   }
   else{
       return g_NULL_c;
   }
}

/**************************************************************************************/
uint8_t App_ReadWriteAttribute(  uint8_t endPointId,
                                 uint16_t clusterId,
                                 uint16_t attributeId,
                                 uint8_t* pData,
                                 uint8_t operation )
{
    App_Cluster_Def_t *pZCLClusterDefinition;
    uint8_t status = g_ZCL_Failure_c;
    uint8_t direction ;


    if (pData == g_NULL_c)
    {

    	return status;
    }

    direction = ( clusterId == m_DRLC_CLUSTER_c ? g_ZCL_SERVER_TO_CLIENT_DIRECTION_c :
                               g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) ;

    status = App_Get_Cluster_Definition(direction, endPointId, clusterId, &pZCLClusterDefinition);

    if ( status == g_ZCL_Success_c ) {
        status = ZCL_Read_Write_Attribute_Data ( operation,
                                                 pZCLClusterDefinition->p_attrib_data_list,
                                                 pZCLClusterDefinition->p_cluster_info,
                                                 attributeId,
                                                 pData );
    }
    return status;
}

/**************************************************************************************/
uint8_t App_ReadWriteClientAttribute(  uint8_t endPointId,
                                       uint16_t clusterId,
                                       uint16_t attributeId,
                                       uint8_t* pData,
                                       uint8_t operation )
{
    App_Cluster_Def_t *pZCLClusterDefinition;
    uint8_t status;

    status = App_Get_Cluster_Definition( g_ZCL_SERVER_TO_CLIENT_DIRECTION_c,
                                         endPointId, clusterId, &pZCLClusterDefinition );

    if ( status == g_ZCL_Success_c ) {
        status = ZCL_Read_Write_Attribute_Data ( operation,
                                                 pZCLClusterDefinition->p_attrib_data_list,
                                                 pZCLClusterDefinition->p_cluster_info,
                                                 attributeId,
                                                 pData );
    }
    return status;
}

/**************************************************************************************/

void APSDE_Data_Request ( uint8_t *p_ZCL_Data,
                          uint8_t ZCL_Data_Length,
                          uint8_t dest_addr_mode,
                          Address dest_address,
                          uint16_t cluster_id,
                          uint8_t dest_endpoint,
                          uint8_t src_endpoint )
{
   uint8_t buffer_index;
   uint8_t *p_buffer;
   APSDE_Data_Request_t *p_apsde_data_request;
   uint8_t data_length = 0x00;

   buffer_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

   if( g_NO_AVAILABLE_BUFFER_c != buffer_index ){
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 201 */
        allocatedbuffer[buffer_index]= 201;
        freebuffer[buffer_index][0]= 201;
        freebuffer[buffer_index][1]= 0;
#endif
        p_buffer = buffMgmt_getBufferPointer( buffer_index );
        *p_buffer = g_APSDE_Data_Request_c;
        p_apsde_data_request = (APSDE_Data_Request_t *)&(((APSDE_Request_t *)p_buffer)->
                                                       APSDE_Request_Primitive.apsde_data_request);

        data_length =  ZCL_Create_Command ( g_Server_To_Client_c,
                                            p_apsde_data_request->asdu,
                                            (App_ZCL_Request_t*)p_ZCL_Data,
                                            ZCL_Data_Length,
                                            App_GetTransSeqNumber() );

        p_apsde_data_request->dest_addr_mode = dest_addr_mode;
        if( g_Indirect_Address_Mode_c != dest_addr_mode ){
             p_apsde_data_request->dest_endpoint = dest_endpoint;
             if ( dest_addr_mode == g_EXTENDED_ADDR_MODE_c ){
                 memUtils_memCopy( (uint8_t*)&p_apsde_data_request->dest_address.short_address,
                           (uint8_t*)&dest_address.short_address, g_EXTENDED_ADDRESS_LENGTH_c );
             }
             else{
                 memUtils_memCopy( p_apsde_data_request->dest_address.IEEE_address,
                           dest_address.IEEE_address, g_SHORT_ADDRESS_LENGTH_c );
             }
        }
        p_apsde_data_request->src_endpoint = src_endpoint;
        p_apsde_data_request->cluster_id = cluster_id;
        p_apsde_data_request->asdulength = data_length;
        p_apsde_data_request->profile_id = g_APP_PROFILE_ID_c;
        p_apsde_data_request->txoptions =  App_getClusterSecurity( cluster_id );

        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;

        //APS_APSDErequest ( buffer_index );
        APS_APSDE_request(buffer_index,p_apsde_data_request );
   }
}

/**************************************************************************************/

void App_SendData(  uint8_t dstAddrMode, uint8_t* dstAddress, uint8_t dstendpoint,
                    uint16_t clusterId, uint8_t asduLength,  uint8_t* asdu )
{
   uint8_t buffer_index;
   uint8_t *pbuffer;
   APSDE_Data_Request_t *p_apsde_data_request;

   if (asdu == g_NULL_c)
   {

   	return ;
   }

   buffer_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

   if( g_NO_AVAILABLE_BUFFER_c != buffer_index ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 205 */
        allocatedbuffer[buffer_index]= 205;
        freebuffer[buffer_index][0]= 205;
        freebuffer[buffer_index][1]= 0;
#endif
        pbuffer = buffMgmt_getBufferPointer( buffer_index );
        *pbuffer = g_APSDE_Data_Request_c;
        p_apsde_data_request = (APSDE_Data_Request_t *)&(((APSDE_Request_t *)pbuffer)->
                                                       APSDE_Request_Primitive.apsde_data_request);

        p_apsde_data_request->dest_addr_mode = dstAddrMode;
        if( g_Indirect_Address_Mode_c != dstAddrMode ) {
             p_apsde_data_request->dest_endpoint = dstendpoint;
             if ( dstAddrMode == g_EXTENDED_ADDR_MODE_c ) {
                 memUtils_memCopy( (uint8_t*)&p_apsde_data_request->dest_address.short_address,
                            dstAddress, g_EXTENDED_ADDRESS_LENGTH_c );
             }
             else{
                 memUtils_memCopy( p_apsde_data_request->dest_address.IEEE_address, dstAddress,
                           g_SHORT_ADDRESS_LENGTH_c );
             }
        }
        p_apsde_data_request->src_endpoint = ZCL_Callback_Get_Device_EndPoint();
        p_apsde_data_request->cluster_id = clusterId;
        p_apsde_data_request->asdulength = asduLength;
        memUtils_memCopy( p_apsde_data_request->asdu, asdu, asduLength );
        p_apsde_data_request->profile_id = g_APP_PROFILE_ID_c;
        p_apsde_data_request->txoptions = App_getClusterSecurity( clusterId );

#if( g_DRLC_CLUSTER_ENABLE_d == 1 )
		if( p_apsde_data_request->cluster_id == g_DRLC_CLUSTER_c) {
			  if ( p_apsde_data_request->asdu[2] == g_LoadControlEventCmd_c ) {
					p_apsde_data_request->txoptions |= g_APS_ACK_REQUIRED_c;
			  }
		}
#endif

#if( g_PRICE_CLUSTER_ENABLE_d == 1 )
		if( p_apsde_data_request->cluster_id == g_PRICE_CLUSTER_c) {
			if ( p_apsde_data_request->asdu[2] == g_PublishPriceCmd_c ) {
				p_apsde_data_request->txoptions |= g_APS_ACK_REQUIRED_c;
		  	}
		}
#endif


#if( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
		if( p_apsde_data_request->cluster_id == g_KEY_ESTABLISHMENT_CLUSTER_c)	{
			p_apsde_data_request->txoptions |= g_APS_ACK_REQUIRED_c;
		}
#endif

#if( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
        if( p_apsde_data_request->cluster_id == g_OTA_UPGRADE_CLUSTER_c)	{
			p_apsde_data_request->txoptions |= g_APS_ACK_REQUIRED_c;
		}
#endif	/* g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d */

        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;
        //APS_APSDErequest ( buffer_index );
        APS_APSDE_request(buffer_index,p_apsde_data_request );
   }
}

/**************************************************************************************/
void App_sendAPSData(uint8_t dstAddrMode, uint8_t* dstAddress, uint8_t dstendpoint,
                     uint16_t clusterId, uint8_t asduLength,  uint8_t* asdu,
                     uint8_t withACK)
{
   uint8_t buffer_index;
   uint8_t *pbuffer;
   APSDE_Data_Request_t *p_apsde_data_request;

   buffer_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

   if( g_NO_AVAILABLE_BUFFER_c != buffer_index ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 100 */
        allocatedbuffer[buffer_index]= 100;
        freebuffer[buffer_index][0]= 100;
        freebuffer[buffer_index][1]= 0;
#endif
        pbuffer = buffMgmt_getBufferPointer( buffer_index );
        *pbuffer = g_APSDE_Data_Request_c;
        p_apsde_data_request = (APSDE_Data_Request_t *)&(((APSDE_Request_t *)pbuffer)->
                                                       APSDE_Request_Primitive.apsde_data_request);

        p_apsde_data_request->txoptions = App_getClusterSecurity( clusterId );
        p_apsde_data_request->dest_addr_mode = dstAddrMode;
        if( g_Indirect_Address_Mode_c != dstAddrMode ) {
             p_apsde_data_request->dest_endpoint = dstendpoint;
             if ( dstAddrMode == g_EXTENDED_ADDR_MODE_c ) {
                 memUtils_memCopy( p_apsde_data_request->dest_address.IEEE_address,
                            dstAddress, g_EXTENDED_ADDRESS_LENGTH_c );
             }
             else{
                 memUtils_memCopy((uint8_t*)&p_apsde_data_request->dest_address.short_address, dstAddress,
                           g_SHORT_ADDRESS_LENGTH_c );
                 if (p_apsde_data_request->dest_address.short_address ==
                     (uint16_t)g_BROADCAST_ADDRESS_c) {
                   p_apsde_data_request->txoptions = g_NETWORK_SECURITY_c;
                 }
             }
        }
        p_apsde_data_request->src_endpoint = ZCL_Callback_Get_Device_EndPoint();
        p_apsde_data_request->cluster_id = clusterId;
        p_apsde_data_request->asdulength = asduLength;
        memUtils_memCopy( p_apsde_data_request->asdu, asdu, asduLength );
        p_apsde_data_request->profile_id = g_APP_PROFILE_ID_c;

        p_apsde_data_request->txoptions |= (withACK == g_APS_ACK_REQUIRED_c) ?\
            (g_APS_ACK_REQUIRED_c) : (0x00u);

        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;
        //APS_APSDErequest ( buffer_index );
        APS_APSDE_request(buffer_index,p_apsde_data_request );
   }
}

/**************************************************************************************/
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void App_SendInterPanData(  uint8_t dstAddrMode, uint8_t* dstAddress, uint16_t dstPanId,
                            uint16_t clusterId, uint8_t asduLength, uint8_t* asdu )
{
   uint8_t buffer_index;
   APSDE_InterPan_Data_Request_t *pApsdatarequest;

   buffer_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

   if( g_NO_AVAILABLE_BUFFER_c != buffer_index ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 53 */
        allocatedbuffer[buffer_index]= 53;
        freebuffer[buffer_index][0]= 53;
        freebuffer[buffer_index][1]= 0;
#endif
        pApsdatarequest = (APSDE_InterPan_Data_Request_t*)
                                             buffMgmt_getBufferPointer( buffer_index );
        pApsdatarequest->dstaddrmode = dstAddrMode;
        pApsdatarequest->srcaddrmode = g_INTRP_64BIT_ADDR_MODE_c;
        memUtils_memCopy( pApsdatarequest->dstaddress.IEEE_address, dstAddress,
                  g_EXTENDED_ADDRESS_LENGTH_c );
        pApsdatarequest->dstpanid = dstPanId;
        pApsdatarequest->profileid = g_APP_PROFILE_ID_c;
        pApsdatarequest->clusterid = clusterId;
        pApsdatarequest->asdulength = asduLength;
        pApsdatarequest->asduhandle = App_GetTransSeqNumber();
        memUtils_memCopy( pApsdatarequest->a_asdu, asdu, asduLength );

       // APS_interpan_data_request ( buffer_index, pApsdatarequest );
   }
}
#endif
/**************************************************************************************/
#if 1
void Create_ZDP_Request ( uint8_t ZDP_request_id, uint8_t msg_index )
{
    uint8_t buffer_index;
    ZDP_Request_t *p_ZDP_request_buffer;
	uint8_t *p_buffer_rx;
	p_buffer_rx = buffMgmt_getBufferPointer(msg_index);

    buffer_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

   if( buffer_index != g_NO_AVAILABLE_BUFFER_c )
   {
#if ( DEBUG_TEST == 1 )
        /* Buffer ID --> 77 */
        allocatedbuffer[buffer_index]= 77;
        freebuffer[buffer_index][0]= 77;
        freebuffer[buffer_index][1]= 0;
#endif
      p_ZDP_request_buffer =  (ZDP_Request_t *)buffMgmt_getBufferPointer(buffer_index);
      p_ZDP_request_buffer->msg_type = ZDP_request_id;
      p_ZDP_request_buffer->a_dest_addr[0] = target_address[0] ;
      p_ZDP_request_buffer->a_dest_addr[1] = target_address[1] ;
      memUtils_memCopy ( (uint8_t*)&p_ZDP_request_buffer->ZDP_Request_Command, p_buffer_rx + 1,
                 g_MAX_ZDP_LENGTH_c );

      ZDO_AppZDPrequest ( buffer_index );

      buffMgmt_freeBuffer(msg_index,236);
   }
}
/**************************************************************************************/
void App_Stack_Information ( uint8_t msg_type, uint8_t msg_index )
{
	uint8_t *p_msg_buffer;
	Get_Stack_Info_Confirm_t *stack_info_conf;
	uint8_t attribute_id;
	uint8_t attribute_length = 0x01;
    p_msg_buffer = buffMgmt_getBufferPointer(msg_index);

	stack_info_conf = ( Get_Stack_Info_Confirm_t*)p_msg_buffer;

	msg_type = p_msg_buffer[0];
	attribute_id = p_msg_buffer[1];

    if ( attribute_id == g_Get_MAC_Extended_Address_c ) {
       memUtils_memCopy( (p_msg_buffer + 2),
                (uint8_t*)gp_ZDO_Information_Base->a_extended_address, 0x08 );
       attribute_length = g_EXTENDED_ADDRESS_LENGTH_c;
    }
    else if ( attribute_id == g_Number_Of_freeBuffers_Available_c ) {
		stack_info_conf->attribute_value[0] = buffMgmt_getNumberofFreeBuffers(g_Small_Buffer_Size_c)+
                    buffMgmt_getNumberofFreeBuffers(g_Large_Buffer_Size_c);
        attribute_length = 0x01;
    }
	stack_info_conf->msg_type 			= msg_type + 1;
	stack_info_conf->attribute_id 		= attribute_id;
	testHrns_passMessageUp( msg_index,attribute_length + 2 );
}

#endif
/**************************************************************************************/
uint8_t stack_is_up;
#if (g_CLI_SUPPORT_d == 0)
void AppZigBeeStackStatusHandler( ZigBeeNWKStatusInfo *statusInfo )
{
	stack_is_up = rsi_zigb_stack_is_up();
#if ((defined HA_SA) || (defined SE_SA) || (defined ZLL_SA))
    	sample_app_change_state(*statusInfo);
#endif

    if ( (*statusInfo == ZigBeeNWKIsUp) || (*statusInfo == ZigBeeChangedNodeID) ) {
#if ( g_ZIGBEE_COORDINATOR_d == 0 ) && ( g_SERVICE_DISCOVERY_ENABLE_d == 1 )
            App_ChangeEsiRegState( DISCOVERY_INTIAL_STATE );
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
            isDeviceJoindOrRejoined = g_TRUE_c;
#endif
#endif
#if ( g_APP_PROFILE_ID_c == 0xc05e )
#ifdef g_ZLL_LIGHT_CONTROLLER_c
          if( ZLL_IsInitiator() == g_TRUE_c )
          {
              ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);
              ZLL_aplcMinStartupDelaytime = Get_Current_Time();
          }
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c
#ifdef g_ZLL_LIGHT_DEVICE_c
            //ZDPSendDeviceAnnouncement();
            rsi_zigb_zdp_send_device_announcement();
            ZLL_Change_LightDevice_State(g_T_DEVICE_READY_STATE_c);
#endif /* #ifdef g_ZLL_LIGHT_DEVICE_c*/
#endif //#if ( g_APP_PROFILE_ID_c == 0xc05e )
    }
#if ( g_APP_PROFILE_ID_c == 0xc05e )
    else if  (*statusInfo == ZigBeeNWKIsDown)
    {
      ZLL_Init();
    }
#endif //#if ( g_APP_PROFILE_ID_c == 0xc05e )
}
#endif /* (g_CLI_SUPPORT_d == 0) */

/**************************************************************************************/
void ZDP_APPresponse(uint8_t bufferIndex)
{
    ZigBeeNWKStatusInfo networkStatus ;
    uint8_t *p_buffer = buffMgmt_getBufferPointer(bufferIndex);
#if( g_SERVICE_DISCOVERY_ENABLE_d == 1 )
    if( *p_buffer == g_ZDP_Match_Desc_Response_c ) {

        servicediscoveryresult_t result;
        EndpointList_t endpointlist;

        endpointlist.count = *(p_buffer+ 6 );
        endpointlist.list = p_buffer+ 7;


        result.status = (App_isUnicastQuery() ? UNICAST_SERVICE_DISCOVERY_COMPLETE_WITH_RESPONSE
                 : BROADCAST_SERVICE_DISCOVERY_RESPONSE_RECEIVED);

        result.zdoRspId = g_ZDP_Match_Desc_Response_c;

        memUtils_memCopy(
                (uint8_t*)&(result.matchAddress) ,
                (uint8_t*) (p_buffer+ 4),
                g_SHORT_ADDRESS_LENGTH_c);

        result.responseData = &endpointlist;

        if ( serviceDiscoveryCallback && serviceDiscoveryCallback ( &result ) ) {
//             buffMgmt_freeBuffer( bufferIndex, 245 );
//             return;
        }
    }
    if( *p_buffer == g_ZDP_IEEE_Addr_Response_c ) {
        if ( *(p_buffer + 2 )== g_SUCCESS_c ) {
            servicediscoveryresult_t result;
            result.status = ( App_isUnicastQuery() ?
            UNICAST_SERVICE_DISCOVERY_COMPLETE_WITH_RESPONSE :
                                 BROADCAST_SERVICE_DISCOVERY_RESPONSE_RECEIVED );
            result.zdoRspId = g_ZDP_IEEE_Addr_Response_c;
            result.responseData = (p_buffer + 3 );
            if ( serviceDiscoveryCallback && serviceDiscoveryCallback ( &result ) ) {
                 buffMgmt_freeBuffer( bufferIndex, 245 );
                 return;
            }
        }
    }
#endif

    if( *p_buffer == g_NLME_Leave_Confirmation_c ) {
#if( g_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
        LeaveConfFlagForCommission = g_TRUE_c;
#endif /* ( g_COMMISSIONING_CLUSTER_ENABLE_d == 1 ) */

        uint8_t status;
        status = ((NLME_Response_t *)
                  p_buffer)->NLME_Response_Primitive.leave_conf.status;
#if (g_CLI_SUPPORT_d == 1)
        printf("  Device performs self leave from the network \n\r ");
#endif //#if (g_CLI_SUPPORT_d == 1)
        if(status == g_Success_c) {
            networkStatus = ZigBeeNWKIsDown;
            AppZigBeeStackStatusHandler(&networkStatus);
        }
#if (g_CLI_SUPPORT_d == 1)
        else{
            printf(" Invalid Leave Request \n\r");
        }
#endif //#if (g_CLI_SUPPORT_d == 1)

    }
    else if(*p_buffer == g_NLME_Leave_Indication_c){
        uint8_t extendedAddr[8];

        memUtils_memCopy( extendedAddr ,((NLME_Response_t *) p_buffer)
                         ->NLME_Response_Primitive.leave_ind.a_device_address,
                    m_LENGTH_EIGHT_BYTES_c );
#if (g_CLI_SUPPORT_d == 1)
        /* Print Extended Address in Reverse order*/
        printf(" Device with Extended Address %x %x %x %x %x %x %x %x Left Network \n\r",
               extendedAddr[7],extendedAddr[6],extendedAddr[5],
                   extendedAddr[4],extendedAddr[3],extendedAddr[2],extendedAddr[1],
                   extendedAddr[0]);
#endif //#if (g_CLI_SUPPORT_d == 1)

//        networkStatus = ZigBeeNWKIsDown;
//        AppZigBeeStackStatusHandler(&networkStatus);
    }

#if (g_CLI_SUPPORT_d == 0)
    if ( g_Buffer_Overflow_c == Enqueue( ZDP_APP_Queue, bufferIndex ) ) {
        buffMgmt_freeBuffer( bufferIndex,245 );
    }
#endif
#if (g_CLI_SUPPORT_d == 1)
	 buffMgmt_freeBuffer( bufferIndex,245 );
#endif
}
/******************************************************************************/

/**************************************************************************************/
void App_ZCL_Data_Init ( void )
{
   uint8_t index;
   for( index = g_ZERO_c; index < g_MAX_NO_OF_ENDPOINTS_c; index++ ) {
	   if(AppDeviceListServerInfo[ index ].endpoint != g_DUMMY_ENDPOINT_ID_c){
		   ZCL_Reset ( &AppDeviceListServerInfo[ index ] );

	   }
	   if(AppDeviceListClientInfo[ index ].endpoint != g_DUMMY_ENDPOINT_ID_c){
		   ZCL_Reset ( &AppDeviceListClientInfo[ index ] );
	   }
   }
}

/**************************************************************************************/
void App_Reset( void )
{
//   ZDOinitSTK();

    App_ZCL_Data_Init();
    App_TimerInit();

#if ( g_REPORTING_ENABLE_d == 1 )
     App_ReportingTableInit();
#endif    /* ( g_REPORTING_ENABLE_d == 1 ) */

#if ( g_SCENES_CLUSTER_ENABLE_d == 1 )
    App_ScenesTableInit();
#endif    /* ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
    App_AlarmTableInit();
#endif    /* ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
      App_MessageTableInit();
#endif    /* ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
    App_PriceTableInit();
#if ( g_SE_PROFILE_1_1_d == 1 )
	App_BlockPeriodTableInit();
#endif /* ( g_SE_PROFILE_1_1_d == 1 ) */
#endif    /* ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
    AppDRLCTableInit();
#endif    /* ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
    App_MeterClusterInit();
#endif    /* ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 ) */

#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1)
    App_Reset_Zone_Table();
#endif    /* ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 ) */

#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    App_CBKE_Reset();
#endif    /* ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
    App_TunnelTableIntit();
#endif    /* ( g_TUNNELING_CLUSTER_ENABLE_d == 1 ) */
#if ( g_SERVICE_DISCOVERY_ENABLE_d == 1 )
    App_serviceDiscoveryInit();
#endif

#if (g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1)
    srvBtldr_init();
#endif /* (g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1) */

#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
    App_otaUpgradeClientBtldrInit();
#endif
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
    App_GenericTunnelInit();
#endif
#if (g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1) &&( g_ZIGBEE_ROUTER_d == 1 )
	App_trustCenterKeepaliveStop();
#endif
#if(g_HA_1_2_d == 1)
#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)
    App_ApplianceStatisticReset();
#endif

#if(g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1)
    App_WeeklyScheduleInit();
#endif

#if( g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)

    /* reset */
    App_ApplianceControlReset();
#endif /* g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 */

#if(g_POWER_PROFILE_CLUSTER_ENABLE_d == 1)
    /* reset */
    App_PowerProfileReset();
#endif
#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1)
    App_ResetEventsAndAlert();
#endif /* #if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1)*/


#endif /* g_HA_1_2_d == 1*/

#if ( g_APP_PROFILE_ID_c == 0xc05e )
    ZLL_Reset();
#endif //#if ( g_APP_PROFILE_ID_c == 0xc05e )

    g_requestTransSeqNum = 0x00;
    queueMgmt_Init( &ZDP_APP_Queue, m_QUEUE_SIZE_c );
}

/**************************************************************************************/
void App_Init( void )
{
    App_Reset();
#ifdef HA_SA
    HA_SampleApp_Init();
#endif /* HA_SA */
#ifdef SE_SA
    SE_SampleApp_Init();
#endif /* HA_SA */
#ifdef ZLL_SA
   
    ZLL_SampleApp_Init();
#endif /* ZLL_SA */


}

/**************************************************************************************/
void App_Main( void )
{

#if ( g_REPORTING_ENABLE_d == 1 )
      App_reportingTask();
#endif

#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
    App_AlarmTask();
#endif

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
    App_IdentifyTask();
#endif

#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
      AppScenesTask();
#endif

#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
      App_MessagingTick();
#endif
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
      App_PriceTick();
#endif    /* ( g_PRICE_CLUSTER_ENABLE_d == 1 ) */

#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
      AppDRLCTick();
#endif    /* ( g_DRLC_CLUSTER_ENABLE_d == 1 ) */

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
    App_MeterClusterTask();
#endif    /* ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 ) */

#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    App_CBKE_StateMachineHandler();
#endif    /* ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
    App_ServerTunnelTask();
#endif    /* ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 ) */
#if ( g_SERVICE_DISCOVERY_ENABLE_d == 1 )
    App_serviceDicsoveryTask();
#endif
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
    App_prepaymentTask();
#endif
#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
    App_otaUpgradeClientBtldrTask();
#endif

#if (g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1) &&( g_ZIGBEE_ROUTER_d == 1 )
    App_trustCenterKeepaliveTask();
#endif
#if (g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1)
    srvBtldr_main();
#endif /* (g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1) */
    //App_checkFreeRunTimer();

#if ( g_HA_1_2_d == 1 )
#if( g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
    App_ApplianceActivitySimulated();
#endif /* g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1*/
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
    App_UpdateServerValues();
#endif /* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1*/
#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
    App_UpdateClientValues();
#endif /* g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1*/


#if(g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1)
    App_Thermostat_Task();
#endif /*g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 */



#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
    CheckTheCheckIntervalAndSendCheckInCommand();
    SetTheMACPollRateAccordingly();
#endif /*  g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 */



#endif	/*g_HA_1_2_d == 1 */

#ifdef HA_SA
    HA_SampleApp_StateMachine();
#endif /* HA_SA */

#ifdef SE_SA
    SE_SampleApp_StateMachine();
#endif /* SE_SA */

#ifdef ZLL_SA
    ZLL_SampleApp_Statemachine();
#endif /* ZLL_SA */

}

/**************************************************************************************/
void App_CreateAndSendLeaveRequest( uint8_t *pDestAddress )
{
    uint8_t leaveBufferIndex;
    NLME_Request_t tempNLMELeaveReq;
    uint8_t *pBufferRx;

    /*Allocate a buffer to send leave request*/
    leaveBufferIndex = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
#if (DEBUG_TEST == 1)
    /* Buffer ID --> 63 */
    allocatedbuffer[leaveBufferIndex]= 63;
    freebuffer[leaveBufferIndex][0]= 63;
    freebuffer[leaveBufferIndex][1]= 0;
#endif
    /*This Buffer is used for to issue leave request*/
    if(g_NO_AVAILABLE_BUFFER_c != leaveBufferIndex) {

		pBufferRx = buffMgmt_getBufferPointer(leaveBufferIndex);

		tempNLMELeaveReq.primitive = g_NLME_Leave_Request_c;
		if(pDestAddress != g_NULL_c) {
            memUtils_memCopy(tempNLMELeaveReq.NLME_Request_Primitive.leave.a_device_address,pDestAddress,
                    g_EXTENDED_ADDRESS_LENGTH_c);
        }
        else
        {
            memUtils_memSet(tempNLMELeaveReq.NLME_Request_Primitive.leave.a_device_address,0x00,
                    g_EXTENDED_ADDRESS_LENGTH_c);
        }
		tempNLMELeaveReq.NLME_Request_Primitive.leave.remove_children = g_TRUE_c;
		tempNLMELeaveReq.NLME_Request_Primitive.leave.rejoin = g_FALSE_c;
		memUtils_memCopy(pBufferRx, (uint8_t *)&tempNLMELeaveReq, sizeof(NLME_Request_t));

//        ZDO_AppZDPrequest( leaveBufferIndex);
    }
}



/**************************************************************************************/
void App_Update_EndPoint_And_Addr( APSDE_Data_Indication_t * pDataIndication )
{
    g_Device_EndPoint = pDataIndication->dest_endpoint;
	g_Src_EndPoint = pDataIndication->src_endpoint;
	g_Device_Address_Mode = pDataIndication->src_addr_mode;

    if ( pDataIndication->src_addr_mode == g_IEEE_Address_Mode_c ) {
        memUtils_memCopy( g_Device_IEEE_Address,
                  pDataIndication->src_address.IEEE_address,g_EXTENDED_ADDRESS_LENGTH_c );

        APS_APSMEsearchAddress( &g_Device_Short_Address,
                                 g_Device_IEEE_Address, g_SEARCH_BY_EXTENDED_ADDRESS_c );
    }
    else {
	    memUtils_memSet( g_Device_IEEE_Address, g_INVALID_VALUE_c, g_EXTENDED_ADDRESS_LENGTH_c );
		memUtils_memCopy( (uint8_t *)&g_Device_Short_Address,
                 (uint8_t *)& pDataIndication->src_address.short_address, g_SHORT_ADDRESS_LENGTH_c );
    }
}

/**************************************************************************************/

void AppHandleDataIndication(uint8_t bufferIndex, APSDE_Data_Indication_t * pDataIndication )
{
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
	APSDE_Data_Indication_t* pAppDataIndication;
	uint8_t msgType ;
	uint16_t aClusterId;
	uint8_t* bufPtr;
#endif

	if( pDataIndication->dest_endpoint == g_ZDO_ENDPOINT_ID_c ) {
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
		bufPtr = buffMgmt_getBufferPointer( bufferIndex);
		pAppDataIndication = &(((APSDE_Response_t *)bufPtr)->APSDE_Response_Primitive.data_Indication);
		/* High byte of the cluster id is taken to see if the received frame is
		a request frame or a response frame */
		msgType =  (pAppDataIndication->cluster_id >> m_SHIFT_TO_GET_HIGH_BYTE);
		/* The actual primitive is taken here */
		if( ( msgType & m_RESPONSE_FRAME_c ) == m_RESPONSE_FRAME_c ) {
			aClusterId = g_Zone_IEEE_Addr_Response_c;
			if( g_Memory_Matches_c == memUtils_memCompare( (uint8_t*)&pAppDataIndication->cluster_id,
			(uint8_t*)&aClusterId, sizeof(uint16_t ) ) ) {
				if( (g_ZDP_IEEE_Addr_Response_c == (msgType | g_ZDP_IEEE_Addr_Request_c)) &&
					g_Ieee_Address_Request_For_Zone_cluster) {
					g_Ieee_Address_Request_For_Zone_cluster = g_FALSE_c;
					Handle_IEEE_Address_Response ( (APSDE_Data_Indication_t *)( bufPtr + 1 ));
					if( g_IEEE_Address_Response_Processing_State_c ==  g_IAS_Zone_State ){
						Create_Enroll_Response_Command();
					}
				}
			}
			return;
		}
#endif
		return;
	}
    if ( pDataIndication->profile_id == g_APP_PROFILE_ID_c ) {
        App_Update_EndPoint_And_Addr( pDataIndication );
#if (g_CLI_SUPPORT_d == 1)
		if(prtRecMsg == g_TRUE_c){
			Print_ZCL_Incoming_Msg( pDataIndication, bufferIndex );
		}
#endif
    Handle_ZCL_Message ( bufferIndex, pDataIndication );
    }
    else {
        buffMgmt_freeBuffer( bufferIndex,161 );
    }
}
/**************************************************************************************/
void AppAddressConflictHandler(uint8_t bufferIndex ,uint16_t oldShortAddress){

    return;
}


#if ( g_INTERPAN_DATA_ENABLE_d== 1 )
/**************************************************************************************/
void App_InterPanDataindication( uint8_t bufferid,
                                 APSDE_InterPan_Data_Indication_t *pApsdeDataInd )
{
    Handle_ZCL_InterPan_Message( bufferid, pApsdeDataInd );

}

/**************************************************************************************/
void App_InterPanDataConfirmation( uint8_t bufferid,
                                   APSDE_InterPan_Data_Confirmation_t *pInterPanDataConf )
{
    buffMgmt_freeBuffer( bufferid, 161 ) ;
}
#endif    /* ( g_INTERPAN_DATA_ENABLE_d == 1 ) */

/**************************************************************************************/
void AppHandleDataConfirmation ( uint8_t bufferIndex,
                                 APSDE_Data_Confirmation_t* pDataConfirmation )
{

#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    if( ( pDataConfirmation->status == g_SUCCESS_c ) &&
        ( CBKE_GetCurrentState() == INTERMEDIATE_STATE ) &&
        ( CBKE_GetPreviousState()== INITIATE_TERMINATE_KEY_REQ_STATE ) ) {
         CBKE_ChangeState( TERMINATE_KEY_STATE );
    }
#endif
#if( g_APP_PROFILE_ID_c == 0xc05e )
#ifdef g_ZLL_LIGHT_CONTROLLER_c
    if(SentEndpointInfo)
      SentEndpointInfo = g_FALSE_c;
#endif /* #ifdef g_ZLL_LIGHT_CONTROLLER_c*/
#endif /* ( g_APP_PROFILE_ID_c == 0xc05e )*/

#ifdef HA_SA
    if( pDataConfirmation->status == g_SUCCESS_c )
    {
    	if(stateMachine.data_event == g_WAIT_FOR_ON_CMD_CONF_c)
		{
    		stateMachine.data_event = g_READ_ONOFF_ATTRIBUTE_c;
		}
    	else if(stateMachine.data_event == g_WAIT_FOR_OFF_CMD_CONF_c)
    	{
    		stateMachine.data_event = g_READ_ONOFF_ATTRIBUTE_c;
    	}
    	else if(stateMachine.data_event == g_WAIT_FOR_TOGGLE_CMD_CONF_c)
    	{
    		stateMachine.data_event = g_INITATE_SLEEP_STATE_c;
    	}
    }

	//FIXME for Demo
	if (pDataConfirmation->status == g_FAILURE_c)
	{
    	stateMachine.data_event = g_INITATE_SLEEP_STATE_c;
	}

#endif //HA_SA

    buffMgmt_freeBuffer( bufferIndex, 161 );
}

/**************************************************************************************/
uint16_t App_getTCShortAddress( void )
{
    //return gp_ZDO_Configuration->a_config_trust_center_short_address;
    return Zigb_Get_TCShortAddr();
}
#if(g_HA_1_2_d == 1)

#if( g_DIAGNOSTICS_CLUSTER_ENABLE_d ==1 )

/* An attribute that is incremented each time the device resets.*/
void app_No_OF_ResetsHandler(void)
{
  	m_no_of_resets_count++;
	app_writeDiagnosticAtrribute(m_no_of_resets_count,g_NO_OF_RESETS_ATTRIBUTE_c);
}
/**************************************************************************************/


/*This attribute keeps track of the number of writes to persistent memory.
Each time that the device stores a token in persistent memory it will increment this value*/
void app_PersistentMemoryWritesHandler(void)
{
  	m_persistent_memory_writes_count++;
	app_writeDiagnosticAtrribute(m_persistent_memory_writes_count,
								 g_PERSISTENT_MEMORY_WRITES_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the MAC layer receives a broadcast*/
void app_MACRecBroadcastHandler(void)
{
  	m_mac_rec_broadcast_count++;
	app_writeDiagnosticAtrribute(m_mac_rec_broadcast_count,g_MAC_RX_BCAST_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the MAC layer transmits a broadcast*/
void app_MACTrxBroadcastHandler(void)
{
  	m_mac_trx_broadcast_count++;
	app_writeDiagnosticAtrribute(m_mac_trx_broadcast_count,g_MAC_TX_BCAST_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the MAC layer receives a unicast.*/
void app_MACRecUnicastHandler(void)
{
  	m_mac_rec_unicast_count++;
	app_writeDiagnosticAtrribute(m_mac_rec_unicast_count,g_MAC_RX_UCAST_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the MAC layer transmits a unicast*/
void app_MACTrxUnicastHandler(void)
{
  	m_mac_trx_unicast_count++;
	app_writeDiagnosticAtrribute(m_mac_trx_unicast_count,g_MAC_TX_UCAST_ATTRIBUTE_c);
}
/**************************************************************************************/

/*A counter that is incremented each time the MAC layer retries a unicast. */
void app_MACTrxUnicastRetryHandler(void)
{
  	m_mac_trx_unicast_retry_count++;
	app_writeDiagnosticAtrribute(m_mac_trx_unicast_retry_count,g_MAC_TX_UCAST_RETRY_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the MAC layer fails to send a unicast.*/
void app_MACTrxUnicastFilureHandler(void)
{
  	m_mac_trx_unicast_failure_count++;
	app_writeDiagnosticAtrribute(m_mac_trx_unicast_failure_count,g_MAC_TX_UCAST_FAIL_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the APS layer receives a broadcast*/
void app_APSRecBroadcastHandler(void)
{
  	m_aps_rec_broadcast_count++;
	app_writeDiagnosticAtrribute(m_aps_rec_broadcast_count,g_APS_RX_BCAST_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the APS layer transmits a broadcast.*/
void app_APSTrxBroadcastHandler(void)
{
  	m_aps_trx_broadcast_count++;
	app_writeDiagnosticAtrribute(m_aps_trx_broadcast_count,g_APS_TX_BCAST_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the APS layer receives a unicast */
void app_APSRecUnicastHandler(void)
{
  	m_aps_rec_unicast_count++;
	app_writeDiagnosticAtrribute(m_aps_rec_unicast_count,g_APS_RX_UCAST_ATTRIBUTE_c);
}

/**************************************************************************************/

/* A counter that is incremented each time the APS layer successfully transmits a
 unicast.*/
void app_APSTrxUnicastSuccessHandler(void)
{
  	m_aps_trx_unicast_success_count++;
	app_writeDiagnosticAtrribute(m_aps_trx_unicast_success_count,g_APS_TX_UCAST_SUCCESS_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time the APS layer 46 retries the sending of a unicast.*/
void app_APSTrxUnicastRetryHandler(void)
{
  	m_aps_trx_unicast_retry_count++;
	app_writeDiagnosticAtrribute(m_aps_trx_unicast_retry_count,g_APS_TX_UCAST_RETRY_ATTRIBUTE_c);
}
/**************************************************************************************/

/*A counter that is incremented each time the APS layer fails to send a unicast. */
void app_APSTrxUnicastFailureHandler(void)
{
  	m_aps_trx_unicast_failure_count++;
	app_writeDiagnosticAtrribute(m_aps_trx_unicast_failure_count,g_APS_TX_UCAST_FAIL_ATTRIBUTE_c);
}
/**************************************************************************************/

/*A counter that is incremented each time the network layer submits a route
discovery message to the MAC. */
void app_RouteDiscInitiatedHandler(void)
{
  	m_route_disc_initiated_count++;
	app_writeDiagnosticAtrribute(m_route_disc_initiated_count,g_ROUTE_DISC_INITIATED_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time an entry is added to the neighbor table*/
void app_NeighborAddedHandler(void)
{
 	 m_neighbor_added_count++;
	app_writeDiagnosticAtrribute(m_neighbor_added_count,g_NEIGHBOR_ADDED_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time an entry is removed from the neighbor
 table.*/
void app_NeighborRemovedHandler(void)
{
 	 m_neighbor_removed_count++;
	app_writeDiagnosticAtrribute(m_neighbor_removed_count,g_NEIGHBOR_REMOVED_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time a neighbor table entry becomes stale
 because the neighbor has not been heard from.*/
void app_NeighborStaleHandler(void)
{
 	 m_neighbor_stale_count++;
	app_writeDiagnosticAtrribute(m_neighbor_stale_count,g_NEIGHBOR_STALE_ATTRIBUTE_c);
}

/**************************************************************************************/

/* A counter that is incremented each time a node joins or rejoins the network via this
 node*/
/**************************************************************************************/

void app_JoinIndicationHandler(void)
{
 	 m_child_join_indication_count++;
	app_writeDiagnosticAtrribute(m_child_join_indication_count,
								 g_JOIN_INDICATION_ATTRIBUTE_c);
}
/**************************************************************************************/
/*A counter that is incremented each time an entry is removed from the child table */
/**************************************************************************************/

void app_ChildMovedHandler(void)
{
  	m_child_move_count++;
	app_writeDiagnosticAtrribute(m_child_move_count,
								 g_CHILD_MOVED_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time a message is dropped at the network layer
 because the APS frame counter was not higher than the last message seen from that
 source*/
/**************************************************************************************/

void app_NWKFCFailureHandler(void)
{
  	m_nwkfc_failure_count++;
	app_writeDiagnosticAtrribute(m_nwkfc_failure_count,
								 g_NWKFC_FAILURE_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time a message is dropped at the APS layer
because the APS frame counter was not higher than the last message seen from that
 source.*/
void app_APSFC_FailureHandler(void)
{
  	m_apsfc_failure_count++;
	app_writeDiagnosticAtrribute(m_apsfc_failure_count,
								 g_APSFC_FAILURE_ATTRIBUTE_c);
}
 /**************************************************************************************/

/*A counter that is incremented each time a message is dropped  at the APS layer
 because it had APS encryption but the key associated with the sender has not been authenticated,
 and thus the key is not authorized for use in APS data messages. */

void app_APSUnauthorizedKeyHandler(void)
{
	m_unauthorised_key_count++;
	app_writeDiagnosticAtrribute(m_unauthorised_key_count,
								 g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_c);
}
/**************************************************************************************/

/* A counter that is incremented each time a NWK encrypted message was received
 but dropped because decryption failed*/
void app_NWKDecryptFailuresHandler(void)
{
	m_nwk_decrypt_failure_count++;
	app_writeDiagnosticAtrribute(m_nwk_decrypt_failure_count,
								 g_NWK_DECRYPT_FAILURES_ATTRIBUTE_c);
}
/**************************************************************************************/

/*A counter that is incremented each time an APS encrypted message was received
 but dropped because decryption failed. */
void app_APSDecryptFailuresHandler(void)
{
	m_aps_decrypt_failure_count++;
	app_writeDiagnosticAtrribute(m_aps_decrypt_failure_count,
								 g_APS_DECRYPT_FAILURES_ATTRIBUTE_c);
}
/**************************************************************************************/

/*A counter that is incremented each time the stack failed to allocate a packet
buffers. This doesn't necessarily mean that the packet buffer count was 0 at the
 time, but that the number requested was greater than the number free. */

void app_PacketBufferAllocateFailuresHandler(void)
{
	m_packet_buffer_allocate_failure_count++;
	app_writeDiagnosticAtrribute(m_packet_buffer_allocate_failure_count,
								 g_PACKET_BUFFER_ALLOCATE_FAILURES_ATTRIBUTE_c);
}

/**************************************************************************************/


/* A counter that is incremented each time a unicast packet is relayed.*/
void app_RelayedUcastHandler(void)
{
  	m_relayed_ucast_count++;
	app_writeDiagnosticAtrribute(m_relayed_ucast_count,
								 	g_RELAYED_UCAST_ATTRIBUTE_c);

}
/**************************************************************************************/

/* */
void app_PhyToMACQueueLimitReachedHandler(void)
{
  	m_phy_to_mac_queue_lmit_count++;
	app_writeDiagnosticAtrribute(m_phy_to_mac_queue_lmit_count,
								 	g_PHY_TO_MAC_QUEUE_LIMIT_REACHED_ATTRIBUTE_c);

}
/**************************************************************************************/

/* A counter that is incremented each time a packet was dropped due to a packet validation error.
 This could be due to length or other formatting problems in the
packet*/
void app_PacketValidateDropHandler(void)
{
  	m_packet_validate_drop_count++;
	app_writeDiagnosticAtrribute(m_packet_validate_drop_count,
								 	g_PACKET_VALIDATE_DROP_COUNT_ATTRIBUTE_c);

}
/**************************************************************************************/

void app_writeDiagnosticAtrribute(uint8_t attributeValue, uint16_t attributeID)
{
	uint8_t status;
	status = App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
									g_DIAGNOSTICS_CLUSTER_c,
									attributeID,
									&attributeValue,g_Write_Attribute_Data_c );
}

void app_reset_diagnostic_cluster_variables(void)
{
	 m_nwk_decrypt_failure_count =0x00;
	 m_aps_decrypt_failure_count  =0x00;
	 m_packet_buffer_allocate_failure_count  =0x00;
	 m_relayed_ucast_count  =0x00;
	 m_phy_to_mac_queue_lmit_count =0x00;
	 m_packet_validate_drop_count  =0x00;
	 m_unauthorised_key_count =0x00;
	 m_apsfc_failure_count   =0x00;
	 m_nwkfc_failure_count =0x00;
	 m_child_move_count  =0x00;
	 m_neighbor_removed_count  =0x00;
	 m_neighbor_stale_count   =0x00;
	 m_child_join_indication_count   =0x00;
	 m_neighbor_added_count =0x00;
	 m_route_disc_initiated_count  =0x00;
	 m_aps_trx_unicast_failure_count =0x00;
	 m_aps_trx_unicast_success_count  =0x00;
	 m_aps_trx_unicast_retry_count =0x00;
	 m_no_of_resets_count  =0x00;
	 m_persistent_memory_writes_count=0x00;
	 m_mac_rec_broadcast_count =0x00;
	 m_mac_trx_broadcast_count =0x00;
	 m_mac_rec_unicast_count =0x00;
	 m_mac_trx_unicast_count =0x00;
	 m_mac_trx_unicast_retry_count =0x00;
	 m_mac_trx_unicast_failure_count =0x00;
	 m_aps_rec_broadcast_count =0x00;
	 m_aps_rec_unicast_count =0x00;
	 m_aps_trx_broadcast_count=0x00;
}
#endif
#endif /* #if(g_HA_1_2_d == 1)*/
/**************************************************************************************/

/***************************************************************************************
* Private Functions
****************************************************************************************/
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
static void Create_Enroll_Response_Command ( void )
{
   uint8_t src_endpoint;
   uint8_t dest_endpoint;
   uint16_t dest_addr;
   uint8_t message_index;
   APSDE_Request_t *p_apsde_data_request;
   message_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
    if( message_index != g_INVALID_INDEX_c ){
#if ( DEBUG_TEST == 1 )
	  /* Buffer ID --> 93 */
        allocatedbuffer[message_index]= 93;
        freebuffer[message_index][0]= 93;
        freebuffer[message_index][1]= 0;
#endif
	  p_apsde_data_request = (APSDE_Request_t *)buffMgmt_getBufferPointer( message_index );
      g_IAS_Zone_State = g_Ready_State_c;
      p_apsde_data_request->msgType = g_APSDE_Data_Request_c;
      p_apsde_data_request->APSDE_Request_Primitive.
            apsde_data_request.dest_addr_mode = g_Short_Address_Mode_c;
      p_apsde_data_request->APSDE_Request_Primitive.
            apsde_data_request.profile_id = g_APP_PROFILE_ID_c;
      p_apsde_data_request->APSDE_Request_Primitive.
            apsde_data_request.cluster_id =g_IAS_ZONE_CLUSTER_c;
      p_apsde_data_request->APSDE_Request_Primitive.
            apsde_data_request.txoptions = App_getClusterSecurity( g_IAS_ZONE_CLUSTER_c );

      p_apsde_data_request->APSDE_Request_Primitive.
            apsde_data_request.radius = 0x0A;

      App_Create_Enroll_Response_Command_Payload ( (uint8_t*)&dest_addr,
         &(p_apsde_data_request->APSDE_Request_Primitive. apsde_data_request.asdu[0]) ,
         &src_endpoint, &dest_endpoint );
      p_apsde_data_request->APSDE_Request_Primitive.
         apsde_data_request.asdulength = 0x05;
      p_apsde_data_request->APSDE_Request_Primitive.
            apsde_data_request.dest_address.short_address = dest_addr;
      p_apsde_data_request->APSDE_Request_Primitive.
            apsde_data_request.dest_endpoint = dest_endpoint;
      p_apsde_data_request->APSDE_Request_Primitive.
            apsde_data_request.src_endpoint = src_endpoint;

   //  APS_APSDErequest( message_index );
       APS_APSDE_request(message_index,p_apsde_data_request );
   }
}
#endif

ZigBeeAPSDEDataRequest_t APSDEDataReq_temp;
Address dest_address_temp;
uint8_t APS_APSDE_request(uint8_t messageIndex,APSDE_Data_Request_t *p_apsde_data_request )
{
  APS_APSDErequest(messageIndex);
  return 0;
	uint8_t status = 0,i=0;
  APSDEDataReq_temp.DestEndpoint = p_apsde_data_request->dest_endpoint;
  APSDEDataReq_temp.SrcEndpoint  = p_apsde_data_request->src_endpoint;
  APSDEDataReq_temp.ProfileId    = p_apsde_data_request->profile_id;
  APSDEDataReq_temp.ClusterId    = p_apsde_data_request->cluster_id;
  APSDEDataReq_temp.AsduLength   = p_apsde_data_request->asdulength;
  APSDEDataReq_temp.TxOptions    = p_apsde_data_request->txoptions;
  APSDEDataReq_temp.Radius       = p_apsde_data_request->radius;
  for(i = 0; i < APSDEDataReq_temp.AsduLength; i++)
    APSDEDataReq_temp.aPayload[i] = p_apsde_data_request->asdu[i];

  if(p_apsde_data_request->dest_addr_mode != g_Indirect_Address_Mode_c)
  {
	  if(p_apsde_data_request->dest_addr_mode == g_Short_Address_Mode_c) {
      dest_address_temp.short_address = (uint16_t*)p_apsde_data_request->dest_address.short_address;
    }
    else if(p_apsde_data_request->dest_addr_mode == g_IEEE_Address_Mode_c) {
            memUtils_memCopy ( dest_address_temp.IEEE_address,
                             p_apsde_data_request->dest_address.IEEE_address, g_EXTENDED_ADDRESS_LENGTH_c );
    }
   status = rsi_zigb_send_unicast_data(p_apsde_data_request->dest_addr_mode,dest_address_temp,&APSDEDataReq_temp);
  }
  else
  {
   status = rsi_zigb_send_broadcast_data(&APSDEDataReq_temp);
  }
   buffMgmt_freeBuffer(messageIndex,73);
  return status;
}
uint16_t randomT ( void )
{
  uint16_t random;
  uint32 i, r;
  uint8 seed[16];          //seed for DRNG
  for(i = 0; i < 16; i++) 
    seed[i] = 0x0;
  for(i = 0; i < 8; i++)
  {
    //r = rand();
    r = rsi_get_random_number();
    seed[2*i] = (uint8)r;
    seed[2*i + 1] = (uint8)(r >> 8);
  }
  //Initialize the DRNG
  ctr_init(seed);
  //	random = rsi_802154_get_random_num();
  random = ctr_generate(1,&random);
  if ( (random > 0xFFF7) || (random == 0x0000) ) {
    random -= 10;
  }

  return (random );
}

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
