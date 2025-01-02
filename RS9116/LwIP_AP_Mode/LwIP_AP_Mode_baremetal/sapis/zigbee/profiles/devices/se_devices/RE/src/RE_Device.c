/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "buffer_management.h"
#include "queue.h"
//#include "timer.h"
#include "ZCL_Common.h"
#include "stack_common.h"
#include "AppTestHarness.h"
#include "memory_utility.h"
#include "zdo_interface.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "ApplicationThinLayer.h"
#include "RE_Device.h"
#include "RE_Device_Application_Framework.h"
#include "ApptimerId.h"
#include "AppReportingUtilities.h"
#include "AppZclDefaultAttributesData.h"
#include "AppDiscoveryUtilities.h"
#include "stack_functionality.h"
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Basic_Cluster.h"
#endif
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Power_Config_Cluster.h"
#endif
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Alarms_Cluster.h"
    #include "AppAlarmsUtilities.h"
#endif
#if( g_IDENTIFY_CLUSTER_ENABLE_d ==1 )
    #include "AppIdentifyUtilities.h"
    #include "ZCL_General_Identify_Cluster.h"
#endif
#if ( g_MESSAGING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Message_Cluster.h"
    #include "AppMessageUtilities.h"
#endif
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Price_Cluster.h"
    #include "AppPriceClusterUtilities.h"
#endif
#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_DRLC_Cluster.h"
    #include "AppDRLCUtilities.h"
#endif
#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_General_Time_Cluster.h"
#endif
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Simple_Metering_Cluster.h"
    #include "AppSimpleMeterUtilities.h"
#endif
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Key_Establishment.h"
    #include "AppCBKEUtilities.h"
#endif
#if(g_COMMISSIONING_CLUSTER_ENABLE_d == 1)
    #include "ZCL_Commissioning_Cluster.h"
    #include "AppCommissioningUtilities.h"
#endif
#if(g_TUNNELING_CLUSTER_ENABLE_d == 1)
    #include "ZCL_Tunneling_Cluster.h"
    #include "AppTunnelingClusterUtilities.h"
#endif
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
    #include "aps_interpan_stub.h"
#endif

#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
	#include "ZCL_Generic_Tunnel_Cluster.h"
	#include "AppGenericTunnelClusterUtilities.h"
#endif
#if ( g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1 )
	#include "AppTrustCenterKeepAliveUtilities.h"
#endif
#if ( g_OTA_BOOTLOAD_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_OTA_Upgrade_Cluster.h"
#if( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
    #include "AppClientBootloader.h"
#endif
#if ( g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1 )
    #include "AppServerBootloader.h"
#endif
#endif
/*-----------------------------------------------------------------------------
 Public Memory declarations
----------------------------------------------------------------------------*/


/* App ZCL Cluster attributes data strorage and default values */

#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
Basic_Cluster_t basic_server_cluster_values ;
const Basic_Cluster_t basic_sever_cluster_default_values =
{
     g_ZCL_VERSION_ATTRIBUTE_DEFAULT_DATA_c,

#if ( g_APPLICATION_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_APPLICATION_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_STACK_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_STACK_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_HW_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_HW_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_MANUFACTURE_NAME_ATTRIBUTE_ENABLE_d == 1 )
    g_MANUFACTURE_NAME_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_MODEL_IDENTIFIER_ATTRIBUTE_ENABLE_d == 1 )
    g_MODEL_IDENTIFIER_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_DATE_CODE_ATTRIBUTE_ENABLE_d == 1 )
    g_DATE_CODE_ATTRIBUTE_DEFAULT_DATA_c,
#endif

    g_POWER_SOURCE_ATTRIBUTE_DEFAULT_DATA_c,

#if ( g_LOCATION_DESCRIPTION_ATTRIBUTE_ENABLE_d == 1 )
    g_LOCATION_DESCRIPTION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d == 1 )
    g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_DEVICE_ENABLED_ATTRIBUTE_ENABLE_d == 1 )
    g_DEVICE_ENABLED_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    g_ALARM_MASK_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_DISABLE_LOCAL_CONFIG_ATTRIBUTE_ENABLE_d == 1 )
    g_DISABLE_LOCAL_CONFIG_ATTRIBUTE_DEFAULT_DATA_c
#endif
};
#endif       /* ( g_BASIC_CLUSTER_ENABLE_d == 1 ) */

#if ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
Power_Config_Cluster_t power_config_server_cluster_values;
const Power_Config_Cluster_t power_config_server_cluster_default_values =
{
#if ( g_MAINS_VOLTAGE_ATTRIBUTE_ENABLE_d == 1 )
    g_MAINS_VOLTAGE_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_MAINS_VOLTAGE_ATTRIBUTE_ENABLE_d*/
#if ( g_MAINS_FREQUENCY_ATTRIBUTE_ENABLE_d == 1 )
    g_MAINS_FREQUENCY_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_MAINS_FREQUENCY_ATTRIBUTE_ENABLE_d*/
#if ( g_MAINS_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    g_MAINS_ALARM_MASK_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_MAINS_ALARM_MASK_ATTRIBUTE_ENABLE_d*/
#if ( g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d*/
#if ( g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_ENABLE_d*/
#if ( g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d == 1 )
    g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d*/
#if ( g_BATTERY_VOLATGE_ATTRIBUTE_ENABLE_d == 1 )
    g_BATTERY_VOLATGE_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_BATTERY_VOLATGE_ATTRIBUTE_ENABLE_d*/
#if ( g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d == 1 )
    g_BATTERY_MANUFACTURER_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d*/
#if ( g_BATTERY_SIZE_ATTRIBUTE_ENABLE_d == 1 )
    g_BATTERY_SIZE_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_BATTERY_SIZE_ATTRIBUTE_ENABLE_d*/
#if ( g_BATTERY_AHR_RATING_ATTRIBUTE_ENABLE_d == 1 )
    g_BATTERY_AHR_RATING_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_BATTERY_AHR_RATING_ATTRIBUTE_ENABLE_d*/
#if ( g_BATTERY_QUANTITY_ATTRIBUTE_ENABLE_d == 1 )
    g_BATTERY_QUANTITY_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_BATTERY_QUANTITY_ATTRIBUTE_ENABLE_d*/
#if ( g_BATTERY_RATED_VOLATGE_ATTRIBUTE_ENABLE_d == 1 )
    g_BATTERY_RATED_VOLATGE_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_BATTERY_RATED_VOLATGE_ATTRIBUTE_ENABLE_d*/
#if ( g_BATTERY_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    g_BATTERY_ALARM_MASK_ATTRIBUTE_DEFAULT_DATA_c,
#endif          /*g_BATTERY_ALARM_MASK_ATTRIBUTE_ENABLE_d*/
#if ( g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_DEFAULT_DATA_c
#endif          /*g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d*/
};
#endif         /* ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if( g_ALARMS_SERVER_CLUSTER_ENABLE_d ==1 )
Alarm_Cluster_t alarm_server_cluster_values = {
		{0x00, 0x00}
};
const Alarm_Cluster_t alarm_server_cluster_default_values = {
		{0x00, 0x00}
};
#endif

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
Identify_Cluster_t identify_server_cluster_values;
const Identify_Cluster_t identify_server_cluster_default_values =
{
    g_IDENTIFY_TIME_ATTRIBUTE_DEFAULT_DATA_c
};
#endif /* ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 ) */


#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )

Time_Cluster_t time_server_cluster_values;

const Time_Cluster_t time_server_cluster_default_values =
{
    g_TIME_ATTRIBUTE_DEFAULT_DATA_c,
    g_TIME_STATUE_ATTRIBUTE_DEFAULT_DATA_c,
#if ( g_TIME_ZONE_ATTRIBUTE_ENABLE_d == 1) 	
   g_TIME_ZONE_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_TIME_DST_START_ATTRIBUTE_ENABLE_d == 1) 	
    g_TIME_DST_START_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_TIME_DST_END_ATTRIBUTE_ENABLE_d == 1) 	
    g_TIME_DST_END_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_TIME_DST_SHIFT_ATTRIBUTE_ENABLE_d == 1)
    g_TIME_DST_SHIFT_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_TIME_STANDARD_TIME_ATTRIBUTE_ENABLE_d == 1) 	
    g_TIME_STANDARD_TIME_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_TIME_LOCAL_TIME_ATTRIBUTE_ENABLE_d == 1) 	
    g_TIME_LOCAL_TIME_ATTRIBUTE_DEFAULT_DATA_c
#endif
};
#endif /* ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 ) */


#if ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 )
KeyEstClusterAttributes_t KeyEst_server_cluster_values;
const KeyEstClusterAttributes_t KeyEst_server_cluster_default_values =
{
    g_KEY_ESTABLISHMENT_SUITE_ATTRIBUTE_DEFAULT_DATA_c
};
#endif /* ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
KeyEstClusterAttributes_t KeyEst_client_cluster_values;
const KeyEstClusterAttributes_t KeyEst_client_cluster_default_values =
{
    g_KEY_ESTABLISHMENT_SUITE_ATTRIBUTE_DEFAULT_DATA_c
};
#endif /* ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if (g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
Commissioning_Cluster_t commissioning_cluster_server_values;
const Commissioning_Cluster_t commissioning_cluster_server_default_values =
{
    g_COMMISSIONING_SHORT_ADDRESS_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_EXTENDED_PANID_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_PANID_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_CHANNEL_MASK_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_PROTOCOL_VERSION_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_STACK_PROFILE_ATTRIBUTE_DEFAULT_VALUE_c  ,
    g_COMMISSIONING_STARTUP_CONTROL_ATTRIBUTE_DEFAULT_VALUE_c ,
    g_COMMISSIONING_TRUST_CENTER_ADDRESS_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_TRUST_CENTER_MASTER_KEY_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_NETWORK_KEY_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_USE_INSECURE_JOIN_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_PRE_CONFIGURED_LINK_KEY_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_NETWORK_KEY_SEQ_NUM_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_NETWORK_KEY_TYPE_ATTRIBUTE_DEFAULT_VALUE_c ,
    g_COMMISSIONING_NETWORK_MANAGER_ADDRESS_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_SCAN_ATTEMPTS_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_TIME_BETWEEN_SCANS_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_REJOIN_INTERVAL_ATTRIBUTE_DEFAULT_VALUE_c ,
    g_COMMISSIONING_MAX_REJOIN_INTERVAL_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_INDIRECT_POLL_RATE_ATTRIBUTE_DEFAULT_VALUE_c ,
    g_COMMISSIONING_PARENT_RETRY_THRESHOLD_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_CONCENTRATOR_FLAG_ATTRIBUTE_DEFAULT_VALUE_c ,
    g_COMMISSIONING_CONCENTRATOR_RADIUS_ATTRIBUTE_DEFAULT_VALUE_c,
    g_COMMISSIONING_CONCENTRATOR_DISCOVERY_TIME_ATTRIBUTE_DEFAULT_VALUE_c
};
#endif /*g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
DRLCClusterAttribute_t drlc_client_cluster_values;

/* ZCL DRLC cluster default attribute data */
const DRLCClusterAttribute_t drlc_client_cluster_default_values =
{
    g_UTILITY_ENROLEMENT_GROUP_ATTRIBUTE_DEFAULT_DATA_c,
    g_START_RANDOMIZE_MINUTES_ATTRIBUTE_DEFAULT_DATA_c,
    g_STOP_RANDOMIZE_MINUTES_ATTRIBUTE_DEFAULT_DATA_c,
    g_DEVICE_CLASS_VALUE_ATTRIBUTE_DEFAULT_DATA_c
};
#endif /* ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
Tunneling_Cluster_t tunneling_cluster_server_values;
const Tunneling_Cluster_t tunneling_cluster_server_default_values =
{
    g_CLOSE_TUNNEL_TIMEOUT_DEFAULT_VALUE_c
};
#endif

#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
GenericTunnelClusterAttribute_t generic_tunnel_server_cluster_values;

/* ZCL DRLC cluster default attribute data */
const GenericTunnelClusterAttribute_t generic_tunnel_server_cluster_default_values =
{
	g_MAXIMUM_INCOMING_TRANSFER_SIZE_ATTRIBUTE_DEFAULT_DATA_c,
	g_MAXIMUM_OUTGOING_TRANSFER_SIZE_ATTRIBUTE_DEFAULT_DATA_c,
	g_PROTOCOL_ADDRESS_DEFAULT_DATA_c
};
#endif /* ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
OTA_Upgrade_Cluster_Attribute_t ota_upgrade_cluster_values;
/* ZCL Generic Tunnel cluster default attribute data */
const OTA_Upgrade_Cluster_Attribute_t ota_upgrade_cluster_default_values =
{
    g_UPGRADE_SERVER_ID_ATTRIBUTE_DEFAULT_DATA_c,

#if ( g_FILE_OFFSET_ATTRIBUTE_ENABLE_d == 1 )
    g_FILE_OFFSET_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_CURRENT_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_CURRENT_FILE_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_CURRENT_ZIGBEESTACK_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_CURRENT_ZIGBEESTACK_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_DOWNLOADED_FILE_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_DOWNLOADED_FILE_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_DOWNLOADED_ZIGBEESTACK_VERSION_ATTRIBUTE_ENABLE_d == 1 )
    g_DOWNLOADED_ZIGBEESTACK_VERSION_ATTRIBUTE_DEFAULT_DATA_c,
#endif
    g_IMAGE_UPGRADE_STATUS_ATTRIBUTE_DEFAULT_DATA_c,
#if ( g_MANUFACTURER_ID_ATTRIBUTE_ENABLE_d == 1 )
    g_MANUFACTURER_ID_ATTRIBUTE_DEFAULT_DATA_c,
#endif
#if ( g_IMAGE_TYPE_ID_ATTRIBUTE_ENABLE_d == 1 )
    g_IMAGE_TYPE_ID_ATTRIBUTE_DEFAULT_DATA_c
#endif
};
#endif /* ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 ) */


/* Application cluster definations for RE for Server */
const App_Cluster_Def_t a_Server_Cluster_List_For_RE[]=
{
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
   { &ga_Basic_Server_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd, &basic_server_cluster_values,
     &basic_sever_cluster_default_values },
#endif                      /* ( g_BASIC_CLUSTER_ENABLE_d == 1 ) */

#if ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
   { &ga_Power_Configuration_Server_Cluster_Info, ZCL_Handle_PowerConfig_Cluster_Cmd ,
     &power_config_server_cluster_values, &power_config_server_cluster_default_values },
#endif                      /* ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 ) */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
   { &ga_Identify_Server_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd,
     &identify_server_cluster_values, &identify_server_cluster_default_values },
#endif                      /* ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 ) */

#if ( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
   { &ga_Alarms_Server_Cluster_Info, ZCL_Handle_Alarms_Cluster_Cmd, &alarm_server_cluster_values,
     &alarm_server_cluster_default_values },
#endif                      /* ( g_ALARMS_CLUSTER_ENABLE_d == 1 ) */

#if( g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
     {&ga_Commissioning_Server_Cluster_Info,ZCL_Handle_Commissioning_Cluster_Cmd,
      &commissioning_cluster_server_values,&commissioning_cluster_server_default_values },
#endif /*g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/

#if ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 )
   { &ga_KeyEstablishment_Server_Cluster_Info, ZCL_Handle_KeyEstablishment_Cluster_Cmd,
     &KeyEst_server_cluster_values, &KeyEst_server_cluster_default_values },
#endif                      /* ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
   { &ga_Tunneling_Server_Cluster_Info, ZCL_HandleTunnelingClusterCmd,
     &tunneling_cluster_server_values, &tunneling_cluster_server_default_values },
#endif                      /* ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 ) */
	
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
   { &ga_Generic_Tunnel_Server_Cluster_Info, ZCL_Handle_Generic_Tunnel_Cluster_Cmd,
     &generic_tunnel_server_cluster_values, &generic_tunnel_server_cluster_default_values },
#endif                      /* ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 ) */	

#if ( g_OTA_BOOTLOAD_SERVER_CLUSTER_ENABLE_d == 1 )
   { &ma_OTA_Upgrade_Server_Cluster_Info, ZCL_HandleOTAUpgradeClusterCmd, g_NULL_c, g_NULL_c },
#endif
	{ g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c }
};


/* Application cluster definations for RE for Client */
const App_Cluster_Def_t a_Client_Cluster_List_For_RE[]=
{
#if ( g_BASIC_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ga_Basic_Client_Cluster_Info, ZCL_Handle_Basic_Cluster_Cmd, g_NULL_c, g_NULL_c },
#endif                  /* ( g_BASIC_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 )
	 { &ga_Power_Configuration_Client_Cluster_Info, ZCL_Handle_PowerConfig_Cluster_Cmd,
      g_NULL_c, g_NULL_c },	
#endif                  /* ( g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ga_Identify_Client_Cluster_Info, ZCL_Handle_Identify_Cluster_Cmd, g_NULL_c, g_NULL_c },
#endif                  /* ( g_IDENTIFY_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_ALARMS_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ga_Alarms_Client_Cluster_Info, ZCL_Handle_Alarms_Cluster_Cmd, g_NULL_c, g_NULL_c },
#endif                  /* ( g_ALARMS_CLIENT_CLUSTER_ENABLE_d == 1 )*/

#if ( g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1 )
    { &ga_Commissioning_Client_Cluster_Info, ZCL_Handle_Commissioning_Cluster_Cmd,
     g_NULL_c, g_NULL_c },
#endif

#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ga_KeyEstablishment_Client_Cluster_Info, ZCL_Handle_KeyEstablishment_Cluster_Cmd,
     &KeyEst_client_cluster_values,  &KeyEst_client_cluster_default_values },
#endif                      /* ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ga_Tunneling_Client_Cluster_Info, ZCL_HandleTunnelingClusterCmd, g_NULL_c, g_NULL_c },
#endif                      /* ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ga_DRLC_Client_Cluster_Info, ZCL_Handle_DRLC_Cluster_Cmd, &drlc_client_cluster_values,
     &drlc_client_cluster_default_values  },
#endif                      /* ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TIME_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ga_Time_Client_Cluster_Info, ZCL_Handle_Time_Cluster_Cmd, g_NULL_c, g_NULL_c  },
#endif              /* ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
    { &ga_Price_Client_Cluster_Info, ZCL_Handle_Price_Cluster_Cmd, g_NULL_c, g_NULL_c }  ,
#endif              /* ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ga_Generic_Tunnel_Client_Cluster_Info, ZCL_Handle_Generic_Tunnel_Cluster_Cmd,
   g_NULL_c, g_NULL_c  },
#endif                      /* ( g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_OTA_BOOTLOAD_CLIENT_CLUSTER_ENABLE_d == 1 )
   { &ma_OTA_Upgrade_Client_Cluster_Info, ZCL_HandleOTAUpgradeClusterCmd,
     &ota_upgrade_cluster_values, &ota_upgrade_cluster_default_values  },
#endif
    { g_NULL_c, g_NULL_c, g_NULL_c, g_NULL_c }

};

/* Device definations for RE Tool for Server */
const ZCL_Device_Def_t AppDeviceListServerInfo[] =
{
    { g_RE_ENDPOINT_c, (sizeof( a_Server_Cluster_List_For_RE )-1)/ sizeof( App_Cluster_Def_t ),
      a_Server_Cluster_List_For_RE },
};


/* Device definations for RE for Client */
const ZCL_Device_Def_t AppDeviceListClientInfo[] =
{
    { g_RE_ENDPOINT_c, (sizeof( a_Client_Cluster_List_For_RE )-1)/ sizeof( App_Cluster_Def_t ),
      a_Client_Cluster_List_For_RE }
};

#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
extern APSDE_Data_Indication_t *ptempIndBuffer;
#endif

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

void Handle_ZCL_Message( uint8_t buffer_index, APSDE_Data_Indication_t *pIndicationbuffer )
{

    uint8_t Status;
    App_Cluster_Def_t *p_cluster_definition;
    APSDE_Data_Request_t * pAppDataReq;
    ZCL_ActionHandler_t pActionHandler;
    uint8_t rspbufferindex, eventbufferindex;
    uint8_t* pLocationForResponse;
    uint8_t receivedsecurity = g_NO_SECURITY_c;
    uint8_t expectedsecurity ;
	
    receivedsecurity = pIndicationbuffer->security_status;

    expectedsecurity = App_getClusterSecurity( pIndicationbuffer->cluster_id );

		if ( receivedsecurity == g_Aps_Secured_Link_Key_c ) {
			receivedsecurity = g_HIGH_SECURITY_c ;
		}
		else if ( receivedsecurity == g_Aps_Secured_Nwk_Key_c ) {
			receivedsecurity = g_NETWORK_SECURITY_c ;
		}
    if ( ( (pIndicationbuffer->a_asdu[0] & g_ZCL_FRAME_TYPE_MASK_c) == g_ZCL_FC_PROFILE_WIDE_COMMAND_c )
        &&( pIndicationbuffer->a_asdu[2] == g_Default_Response_Command_c ) ) {
            Send_Response_To_Application( pIndicationbuffer, buffer_index );
            buffMgmt_freeBuffer( buffer_index, 224 );
            return;
    }
    if ( (pIndicationbuffer->dest_address.short_address != g_BROADCAST_ADDRESS_c ) &&
		(( receivedsecurity == g_NO_SECURITY_c ) ||
			 (( expectedsecurity == g_HIGH_SECURITY_c ) && ( receivedsecurity != g_HIGH_SECURITY_c )) )) {
			
			  App_SendDefaultResponse( g_ZCL_Failure_c, g_TRUE_c, pIndicationbuffer );
	}
	else if( (pIndicationbuffer->dest_address.short_address != g_BROADCAST_ADDRESS_c )&&
			(receivedsecurity !=  expectedsecurity )) {
		App_SendDefaultResponse( g_ZCL_Failure_c, g_FALSE_c, pIndicationbuffer );	
	}
    else {

#if ( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
        ptempIndBuffer = pIndicationbuffer;
#endif
        Status =  App_Get_Cluster_Definition( ZCL_GetDirectionOfCommand( pIndicationbuffer->a_asdu ),
                                             pIndicationbuffer->dest_endpoint,
                                             pIndicationbuffer->cluster_id,
                                             &p_cluster_definition );
        if( Status == g_ZCL_Success_c ) {

            rspbufferindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

            if( rspbufferindex != g_NO_AVAILABLE_BUFFER_c ) {

#if ( DEBUG_TEST == 1 )
                allocatedbuffer[rspbufferindex]= 91;
                freebuffer[rspbufferindex][0]= 91;
                freebuffer[rspbufferindex][1]= 0;
#endif
                eventbufferindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

                if ( eventbufferindex != g_NO_AVAILABLE_BUFFER_c ) {

#if ( DEBUG_TEST == 1 )
                    allocatedbuffer[eventbufferindex]= 91;
                    freebuffer[eventbufferindex][0]= 91;
                    freebuffer[eventbufferindex][1]= 0;
#endif
#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
                    /* Update time attribute of time cluster, with RTC UTC value so that
                       read response sent with current UTC time */
                    if ( ( pIndicationbuffer->cluster_id == g_TIME_CLUSTER_c ) &&
                         ( pIndicationbuffer->a_asdu[ 0x02 ] == g_Read_Attribute_Command_c ) ) {
                           uint8_t index;
                        for ( index = 0x03; index < pIndicationbuffer->asdulength; index+= 0x02 ) {
                            if (* ((uint16_t*)&pIndicationbuffer->a_asdu[index]) == g_TIME_ATTRIBUTE_c ) {
                                ZCL_SetTimeAttribute( p_cluster_definition->p_attrib_data_list,
                                                      ZCL_CallBackGetCurrentTime() );
                            }
                        }
                    }
#endif      /* ( g_TIME_SERVER_CLUSTER_ENABLE_c == 1 ) */

                    pLocationForResponse = buffMgmt_getBufferPointer( rspbufferindex );
                    *pLocationForResponse = g_APSDE_Data_Request_c;
                    pAppDataReq = &((APSDE_Request_t *)pLocationForResponse)->
                        APSDE_Request_Primitive.apsde_data_request;
                    pLocationForResponse = pAppDataReq->asdu;
                    pActionHandler.event.pEventData = buffMgmt_getBufferPointer( eventbufferindex );
                    pActionHandler.event.eventId = g_NULL_c;
                    pActionHandler.action = No_Response;

                    pAppDataReq->asdulength = ZCL_Process_Message( pIndicationbuffer->dest_endpoint,
                                                                  pIndicationbuffer->asdulength,
                                                                  pIndicationbuffer->a_asdu,
                                                                  pLocationForResponse,
                                                                  &pActionHandler,
                                                                  p_cluster_definition );

                    if( pActionHandler.action == Send_Response ) {
                        if ( ( * ( pLocationForResponse + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Default_Response_Command_c ) &&
                             ( ( pIndicationbuffer->dest_addr_mode == g_Group_Address_Mode_c ) ||
                               ( ( pIndicationbuffer->dest_addr_mode == g_Short_Address_Mode_c ) &&
                                 ( pIndicationbuffer->dest_address.short_address >= g_BROADCAST_ADDRESS_ACTIVE_DEVICES_c ) ) ) ) {

                            buffMgmt_freeBuffer( rspbufferindex, 224 );
                        }
                        else {
                            memUtils_memCopy( pAppDataReq->dest_address.IEEE_address, pIndicationbuffer->src_address.IEEE_address, 8 );
                            pAppDataReq->dest_addr_mode = pIndicationbuffer->src_addr_mode;
                            pAppDataReq->dest_endpoint = pIndicationbuffer->src_endpoint;
                            pAppDataReq->src_endpoint = pIndicationbuffer->dest_endpoint;
                            pAppDataReq->profile_id  = pIndicationbuffer->profile_id;
                            pAppDataReq->cluster_id = pIndicationbuffer->cluster_id;
                            pAppDataReq->txoptions = App_getClusterSecurity( pAppDataReq->cluster_id );
                            pAppDataReq->radius = g_DEFAULT_RADIUS_c;
                            APS_APSDErequest( rspbufferindex );
                        }
                    }
                    else if ( pActionHandler.action == Received_Response ) {
                        Send_Response_To_Application( pIndicationbuffer, buffer_index );
#if ( g_TIME_CLIENT_CLUSTER_ENABLE_d == 1 )
                        /* UTC value received from Time server */
                        if ( ( pIndicationbuffer->cluster_id == g_TIME_CLUSTER_c ) &&
                            ( pIndicationbuffer->a_asdu[ 0x02 ] == g_Read_Attribute_Response_Command_c ) )  {
#if ( g_MULTIPLE_ESI_DISCOVERY_ENABLE_d == 1 )
                                App_ProcessTimeReadAttrResponse(
                                &pIndicationbuffer->a_asdu[0x03],pIndicationbuffer->asdulength-0x03);
#else
                                uint8_t *seconds;
                                seconds = ZCL_GetTimeAttribueReadRsp(
                                          pIndicationbuffer->a_asdu,pIndicationbuffer->asdulength );
                                if ( seconds != g_NULL_c ) {
                                    ZCL_callbackSetRTCtime( *((uint32_t*)seconds) );
                                }
#endif   /* g_MULTIPLE_ESI_DISCOVERY_ENABLE_d */
                            }
#endif   /* g_TIME_CLIENT_CLUSTER_ENABLE_d */

#if ( ( g_ZIGBEE_ROUTER_d == 1 )&& (g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1 ))
                    /* Update time attribute of time cluster, with RTC UTC value so that
                       read response sent with current UTC time */
                     if ( ( pIndicationbuffer->cluster_id == g_KEY_ESTABLISHMENT_CLUSTER_c ) &&
						( ZCL_GetDirectionOfCommand( pIndicationbuffer->a_asdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) &&
						(  !Is_ClusterSpecificCommand( pIndicationbuffer->a_asdu ) )&&
                         ( pIndicationbuffer->a_asdu[ 0x02 ] == g_Read_Attribute_Response_Command_c ) ) {
                          uint16_t short_address;
						  short_address =
            			ZigBeeGetShortAddrForSpecifiedIEEEAddr(pIndicationbuffer->src_address.IEEE_address);
                        App_keepAliveCmdRsp(short_address);
                    }
#endif      /* ( g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1 ) */	 	
                        buffMgmt_freeBuffer( rspbufferindex, 224 );
                    }
                    else {
                        buffMgmt_freeBuffer( rspbufferindex, 224 );
                    }
                    switch ( pActionHandler.event.eventId )
                    {
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_Reset_To_Factory_Defaults_Events_c :
                        App_ZCL_Data_Init();
                        break;
#endif                    /* ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if( g_ALARMS_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_Reset_Alarm_Command_Event_c:
                    case g_Reset_All_Alarms_Command_Event_c:
                    case g_Get_Alarm_Command_Event_c:
                    case g_Reset_Alarm_Log_Command_Event_c:
                        Handle_Alarm_Cluster_Events( pActionHandler );
                        break;
#endif              /* g_ALARMS_SERVER_CLUSTER_ENABLE_d */

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_Identify_Event_c :
                    case g_Identify_Query_Event_c :
                        App_IdentifyEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
                    case g_DRLC_LoadControlEvent_c :
                    case g_DRLC_CancelLoadControlEvent_c:
                    case g_DRLC_CancelAllLoadControlEvents_c:
                        App_DRLCEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /* g_PRICE_CLIENT_CLUSTER_ENABLE_d */

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
                    case g_PublishPriceEvent_c :
                        App_PriceEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /* g_PRICE_CLIENT_CLUSTER_ENABLE_d */

#if ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_KeyEstInitiateKeyEstReqEvent_c :
                    case g_KeyEstEphemeralDataReqEvent_c:
                    case g_KeyEstConfirmKeyDataReqEvent_c:
                    case g_KeyEstTerminateKeyEstEvent_c:
                        App_KeyEstablishmentEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /* g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d */

#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
                    case g_KeyEstInitiateKeyEstRespEvent_c :
                    case g_KeyEstEphemeralDataRespEvent_c:
                    case g_KeyEstConfirmKeyDataRespEvent_c:
                        App_KeyEstablishmentEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /* g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d */
						
#if( g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_CommissioningRestartDeviceReqEvent_c:
                    case g_CommissioningSaveStartupParametersReqEvent_c:
                    case g_CommissioningRestoreStartupParametersReqEvent_c:
                    case g_CommissioningResetStartupParametersReqEvent_c:
						App_CommissioningEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /* g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d */
#if( g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d ==1 )

                    case g_CommissioningRestartDeviceRespEvent_c:
                    case g_CommissioningSaveStartupParametersRespEvent_c:
                    case g_CommissioningRestoreStartupParametersRespEvent_c:
                    case g_CommissioningResetStartupParametersRespEvent_c:
						App_CommissioningEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /*g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d*/		
#if ( g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d == 1 )
                    case g_MatchProtocolAddressResponseEvent_c:
					case g_AdvertiseProtocolAddressEvent_c:
					  	  App_GenericTunnelEventsHandler( &pActionHandler.event,
														 pIndicationbuffer );
                         break;
#endif /*g_GENERIC_TUNNEL_CLIENT_CLUSTER_ENABLE_d */	
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )
                    case g_MatchProtocolAddressEvent_c:
					  	  App_GenericTunnelEventsHandler( &pActionHandler.event,
														 pIndicationbuffer );
                         break;
#endif /*g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d */
#if( g_TUNNELING_SERVER_CLUSTER_ENABLE_d ==1 )
                    case g_RequestTunnel_Event_c:
                    case g_CloseTunnel_Event_c:
                    case g_ClientTransferData_Event_c:
                    case g_ClientAckTransferData_Event_c:
                    case g_ClientReadyData_Event_c:
						App_tunnelEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /*g_TUNNELING_SERVER_CLUSTER_ENABLE_d*/
#if( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d ==1 )
                    case g_RequestTunnelResponse_Event_c:
                    case g_ServerTransferData_Event_c:
                    case g_ServerTransferDataError_Event_c:
                    case g_ServerAckTransferData_Event_c:
                    case g_ServerReadyData_Event_c:
						App_tunnelEventsHandler( &pActionHandler.event, pIndicationbuffer );
                        break;
#endif                  /*g_TUNNELING_CLIENT_CLUSTER_ENABLE_d*/							

                    default :
                        break;
                    }
                    buffMgmt_freeBuffer( eventbufferindex, 224 );
                }
                else {
                    buffMgmt_freeBuffer( rspbufferindex, 224 );
                }
            }
        }
        else {
            App_SendDefaultResponse( g_ZCL_Failure_c, g_FALSE_c, pIndicationbuffer );
        }
    }
    buffMgmt_freeBuffer( buffer_index, 224 );
    return;
}

/***********************************************************************************/
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void Handle_ZCL_InterPan_Message( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
    uint8_t status;
    APSDE_InterPan_Data_Request_t* pRspLocation;
    App_Cluster_Def_t *p_cluster_definition;
    ZCL_ActionHandler_t pActionHandler;
    uint8_t rspbufferindex, eventbufferindex;

    if ( ( (pApsdeDataInd->a_asdu[0] & g_ZCL_FRAME_TYPE_MASK_c) == g_ZCL_FC_PROFILE_WIDE_COMMAND_c )
        &&( pApsdeDataInd->a_asdu[2] == g_Default_Response_Command_c ) ) {
            Send_InterPan_Response_To_Application( pApsdeDataInd, buffer_index );
            buffMgmt_freeBuffer( buffer_index, 224 );
            return;
    }
    status =  App_Get_Cluster_Definition( ZCL_GetDirectionOfCommand( pApsdeDataInd->a_asdu ),
                                          g_RE_ENDPOINT_c,
                                          pApsdeDataInd->clusterId,
                                          &p_cluster_definition );
    if ( status == g_ZCL_Success_c ) {
        rspbufferindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
        if( rspbufferindex != g_NO_AVAILABLE_BUFFER_c ) {
#if ( DEBUG_TEST == 1 )
            allocatedbuffer[rspbufferindex]= 91;
            freebuffer[rspbufferindex][0]= 91;
            freebuffer[rspbufferindex][1]= 0;
#endif
            eventbufferindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

            if ( eventbufferindex != g_NO_AVAILABLE_BUFFER_c ) {
#if ( DEBUG_TEST == 1 )
                allocatedbuffer[eventbufferindex]= 91;
                freebuffer[eventbufferindex][0]= 91;
                freebuffer[eventbufferindex][1]= 0;
#endif
                pRspLocation = (APSDE_InterPan_Data_Request_t*)buffMgmt_getBufferPointer( rspbufferindex );
                pActionHandler.event.pEventData = buffMgmt_getBufferPointer( eventbufferindex );
                pActionHandler.event.eventId = g_NULL_c;
                pActionHandler.action = No_Response;

                pRspLocation->asdulength = ZCL_Process_Message( g_RE_ENDPOINT_c,
                                                               pApsdeDataInd->asdulength,
                                                               pApsdeDataInd->a_asdu,
                                                               pRspLocation->a_asdu,
                                                               &pActionHandler,
                                                               p_cluster_definition );
                  if( pActionHandler.action == Send_Response ) {
                    if ( (*( pRspLocation->a_asdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Default_Response_Command_c ) &&
                         ( ( pApsdeDataInd->dstaddrmode == g_Short_Address_Mode_c ) &&
                           ( pApsdeDataInd->dstAddress.short_address >= g_BROADCAST_ADDRESS_c ) ) ) {
                            buffMgmt_freeBuffer( rspbufferindex, 224 );
                    }
                    else {
                        pRspLocation->srcaddrmode = g_INTRP_64BIT_ADDR_MODE_c;
                        pRspLocation->dstaddrmode = pApsdeDataInd->srcaddrmode;
                        pRspLocation->dstpanid = pApsdeDataInd->srcpanid;
                        memUtils_memCopy( pRspLocation->dstaddress.IEEE_address,
                               pApsdeDataInd->srcaddress.IEEE_address, g_EXTENDED_ADDRESS_LENGTH_c);
                        pRspLocation->profileid = pApsdeDataInd->profileId;
                        pRspLocation->clusterid = pApsdeDataInd->clusterId;
                        pRspLocation->asduhandle = App_GetTransSeqNumber();
                        APS_interpan_data_request( rspbufferindex, pRspLocation);
                    }
                }
                else if ( pActionHandler.action == Received_Response ) {
                    Send_InterPan_Response_To_Application( pApsdeDataInd, buffer_index );
                    buffMgmt_freeBuffer( rspbufferindex, 224 );
                }
                else {
                    buffMgmt_freeBuffer( rspbufferindex, 224 );
                }
                switch ( pActionHandler.event.eventId )
                {
#if ( g_MESSAGING_CLIENT_CLUSTER_ENABLE_d == 1 )
                    case g_Display_Message_Events_c :
                    case g_Cancel_Message_Events_c :
                        App_InterPanMessageEventsHandler( &pActionHandler.event, pApsdeDataInd );
                        break;
#endif                  /* g_MESSAGING_SERVER_CLUSTER_ENABLE_d */

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
                    case g_PublishPriceEvent_c :
                        App_InterPanPriceEventsHandler( &pActionHandler.event, pApsdeDataInd );
                        break;
#endif                 /* g_PRICE_CLIENT_CLUSTER_ENABLE_d */
                default :
                    break;
                }
				buffMgmt_freeBuffer( eventbufferindex, 224 );
            }
        }
    }
    else {
        uint8_t asdulength;
        uint8_t asdu[10];
        asdulength = ZCL_CreateDefaultResponseCommand ( pApsdeDataInd->a_asdu,
                                                        asdu,
                                                        g_ZCL_Failure_c );
         App_SendInterPanData( pApsdeDataInd->srcaddrmode,
                      pApsdeDataInd->srcaddress.IEEE_address,
                      pApsdeDataInd->srcpanid,
                      pApsdeDataInd->clusterId,
                      asdulength,
                      asdu );
    }
    buffMgmt_freeBuffer( buffer_index, 224 );
    return;
}
#endif

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
