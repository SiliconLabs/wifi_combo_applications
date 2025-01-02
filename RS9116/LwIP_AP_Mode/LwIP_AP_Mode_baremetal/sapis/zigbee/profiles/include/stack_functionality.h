/*******************************************************************************
 stack_functionality.h
 ********************************************************************************
 * Program Description:
 *
 * This file is used to define MACROs for different NWk Layer functionalities
 * depending upon the compiler configuration.
 *
 *******************************************************************************/
#ifndef STACK_FUNCTIONALITY_H
#define STACK_FUNCTIONALITY_H
/*******************************************************************************
 * Includes
 *******************************************************************************/

/* None */

/*******************************************************************************
 * Global Constants
 *******************************************************************************/

#define m_COORDINATOR_c                                     0x00
#define m_ROUTER_c                                          0x01
#define m_END_DEVICE_c                                      0x02
#define m_UNIFIED_c                                         0xFF

#ifdef  APPLY_R_19_CHANGE
#define APPLY_R_18_CHANGE
#elif defined APPLY_R_20_CHANGE
#define APPLY_R_18_CHANGE
#define APPLY_R_19_CHANGE
#endif

#define RSI_ENABLE 							1
#define RSI_DISABLE							0

/******************************************************************************/

#if defined( ZIGBEE_COORDINATOR )
#define g_ZIGBEE_COORDINATOR_d                             RSI_ENABLE
#define g_ZIGBEE_ROUTER_d                                  RSI_DISABLE
#define g_ZIGBEE_END_DEVICE_d                              RSI_DISABLE
#define g_ZIGBEE_ACTIVE_END_DEVICE_d                       RSI_DISABLE

#elif defined( ZIGBEE_ROUTER )
#define g_ZIGBEE_COORDINATOR_d                             RSI_DISABLE
#define g_ZIGBEE_ROUTER_d                                  RSI_ENABLE
#define g_ZIGBEE_END_DEVICE_d                              RSI_DISABLE
#define g_ZIGBEE_ACTIVE_END_DEVICE_d                       RSI_DISABLE

#elif defined( ZIGBEE_END_DEVICE )
#define g_ZIGBEE_COORDINATOR_d                             RSI_DISABLE
#define g_ZIGBEE_ROUTER_d                                  RSI_DISABLE
#define g_ZIGBEE_END_DEVICE_d                              RSI_ENABLE
#define g_ZIGBEE_ACTIVE_END_DEVICE_d                       RSI_DISABLE

#elif defined( ZIGBEE_ACTIVE_END_DEVICE )
#define g_ZIGBEE_COORDINATOR_d                             RSI_DISABLE
#define g_ZIGBEE_ROUTER_d                                  RSI_DISABLE
#define g_ZIGBEE_ACTIVE_END_DEVICE_d                       RSI_ENABLE
#endif

#ifndef g_ZIGBEE_PRO_STACK_PROFILE_d
#define g_ZIGBEE_PRO_STACK_PROFILE_d                        RSI_DISABLE
#endif                                      /* g_ZIGBEE_PRO_STACK_PROFILE_d */

/*******************************************************************************
 Z I G B E E    C O O R D I N A T O R
 *******************************************************************************/

#if ( g_ZIGBEE_COORDINATOR_d == RSI_ENABLE )
#define g_NWK_FORMATION_d                                   RSI_ENABLE


#define g_NWK_MESH_ROUTING_d                                RSI_ENABLE


#define g_NWK_LEAVE_d                                      RSI_ENABLE
#define g_NWK_PERMIT_JOINING_d                             RSI_ENABLE
#define g_NWK_START_ROUTER_d                               RSI_DISABLE
#define g_NWK_ALLOW_JOIN_d                                 RSI_ENABLE
#define g_NWK_JOIN_NETWORK_d                               RSI_DISABLE
#define g_MULTIPLE_OUTSTANDING_BCAST_d                     RSI_ENABLE

#define g_NWK_BROADCAST_d                                  RSI_ENABLE
#define g_BROADCAST_RETRIES_d                              RSI_ENABLE
#define g_NWK_ROUTE_MAINTENANCE_d                          RSI_ENABLE
#define g_NLME_SYNC_d                                      RSI_DISABLE
#define g_EXTENDED_DEVICE_RESPONSE_d                       RSI_ENABLE
#define g_INTERVAL_TIMER_d                                 RSI_DISABLE
#define g_NWK_LAYER_VALIDATION_d                           RSI_DISABLE
#define g_NLME_ED_SCAN_REQUEST_d                           RSI_ENABLE
#define g_PAN_ID_CONFLICT_d                                RSI_ENABLE
#define g_NWK_USE_MULTICAST_d                              RSI_ENABLE
#define g_NWK_LINK_STATUS_d                                RSI_ENABLE
#define g_NWK_MANY_TO_ONE_ROUTING_d                        RSI_ENABLE

#if ( g_PAN_ID_CONFLICT_d == RSI_ENABLE )
#define g_PAN_ID_CONFLICT_DETECTION_d                      RSI_ENABLE
#define g_PAN_ID_CONFLICT_REPORT_d                         RSI_ENABLE
#define g_PAN_ID_RESOLUTION_d                              RSI_ENABLE
#else
#define g_PAN_ID_CONFLICT_DETECTION_d                      RSI_DISABLE
#define g_PAN_ID_CONFLICT_REPORT_d                         RSI_DISABLE
#define g_PAN_ID_RESOLUTION_d                              RSI_DISABLE
#endif                                          /*( g_PAN_ID_CONFLICT_d == RSI_ENABLE )*/
#define g_NWK_ADDRESS_CONFLICT_DETECTION_d                 RSI_ENABLE
#define g_NWK_ADDRESS_CONFLICT_RESOLUTION_d                RSI_ENABLE


#define g_PERSISTENT_DATA_STORAGE_d                        RSI_ENABLE


#define g_REJOIN_REQUEST_d                                 RSI_DISABLE
#define g_REJOIN_RESPONSE_d                                RSI_ENABLE
#define g_DEVICE_TYPE_c                                     m_COORDINATOR_c
#define g_APS_GROUP_DATA_RX_d                              RSI_ENABLE
#define g_APS_LAYER_VALIDATION_d                           RSI_ENABLE
#define g_APS_DUPLICATE_REJECTION_c                        RSI_ENABLE


//#ifndef APS_FRAGMENTATION_CAPABILITY
//#define APS_FRAGMENTATION_CAPABILITY                    	RSI_DISABLE
//#endif

//#ifndef g_FREQUENCY_AGILITY_SUPPORTED_d
//#define g_FREQUENCY_AGILITY_SUPPORTED_d                     RSI_DISABLE
//#endif


//#define g_APS_BINDING_CAPACITY_c			                RSI_ENABLE

#define g_TRUST_CENTRE_d                                   RSI_ENABLE
#define g_USE_HASH_KEY                                     RSI_ENABLE
/* security macros */
#if( g_ZIGBEE_SECURITY_d == RSI_ENABLE )
#define g_APS_LAYER_SEC_PROCESSING_d                       RSI_ENABLE
#define g_NWK_LAYER_SEC_PROCESSING_d                       RSI_ENABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                    RSI_ENABLE
#define g_TRANSPORT_KEY_REQUEST_d                          RSI_ENABLE
#define g_TRANSPORT_KEY_INDICATION_d                       RSI_DISABLE
#define g_UPDATE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_UPDATE_DEVICE_INDICATION_d                       RSI_ENABLE
#define g_REMOVE_DEVICE_REQUEST_d                          RSI_ENABLE
#define g_REMOVE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_SWITCH_KEY_REQUEST_d                             RSI_ENABLE
#define g_SWITCH_KEY_INDICATION_d                          RSI_DISABLE
#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )
#define g_REQUEST_KEY_REQUEST_d                            RSI_DISABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_ENABLE
#else
#define g_REQUEST_KEY_REQUEST_d                            RSI_DISABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_DISABLE
#endif
#else
#define g_APS_LAYER_SEC_PROCESSING_d                       RSI_DISABLE
#define g_NWK_LAYER_SEC_PROCESSING_d                       RSI_DISABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                    RSI_DISABLE
#define g_TRANSPORT_KEY_REQUEST_d                          RSI_DISABLE
#define g_TRANSPORT_KEY_INDICATION_d                       RSI_DISABLE
#define g_UPDATE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_UPDATE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_REMOVE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_REMOVE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_SWITCH_KEY_REQUEST_d                             RSI_DISABLE
#define g_SWITCH_KEY_INDICATION_d                          RSI_DISABLE
#define g_REQUEST_KEY_REQUEST_d                            RSI_DISABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_DISABLE
#endif                                      /* ( g_ZIGBEE_SECURITY_d == RSI_ENABLE ) */

#define  g_Nwk_Addr_Response_d                             RSI_ENABLE
#define  g_IEEE_Addr_Response_d                            RSI_ENABLE
#define  g_Node_Desc_Response_d                            RSI_ENABLE
#define  g_Power_Desc_Response_d                           RSI_ENABLE
#define  g_Simple_Desc_Response_d                          RSI_ENABLE
#define  g_Active_Endpoint_Response_d                      RSI_ENABLE
#define  g_Match_Desc_Response_d                           RSI_ENABLE
#define  g_Device_Annce_Process_d                          RSI_ENABLE
#define  g_System_Server_Discovery_Response_d              RSI_ENABLE
#define  g_End_Device_Bind_Response_d                      RSI_ENABLE
#define  g_Unbind_Response_d                               RSI_ENABLE
#define  g_Bind_Response_d                                 RSI_ENABLE
#ifdef ZCP_15
#define  g_Complex_Desc_Response_d                         RSI_ENABLE
#define  g_User_Desc_Response_d                            RSI_ENABLE
#define  g_User_Desc_Conf_d                                RSI_ENABLE
#define  g_Discovery_Cache_Response_d                      RSI_ENABLE
#endif // #ifdef ZCP_15

/*******************Network Mgmt Server Service Primitives*********************/
#define g_Mgmt_Permit_Joining_Response_d                    RSI_ENABLE

//#if ( g_FREQUENCY_AGILITY_SUPPORTED_d == RSI_ENABLE )
//#define  g_ZDO_Mgmt_NWK_Update_req_d                       RSI_ENABLE
//#define  g_ZDO_Mgmt_NWK_Update_Notify_d                    RSI_ENABLE
//#define  g_Create_Mgmt_Nwk_Update_Req_d                    RSI_ENABLE
//
//#else
//#define  g_ZDO_Mgmt_NWK_Update_req_d                       RSI_DISABLE
//#define  g_ZDO_Mgmt_NWK_Update_Notify_d                    RSI_DISABLE
//#define  g_Create_Mgmt_Nwk_Update_Req_d                    RSI_DISABLE
//#endif                          /* ( g_FREQUENCY_AGILITY_SUPPORTED_d == RSI_ENABLE ) */

#ifndef g_COMMISSIONING_CAPABILITIES_d
#define g_COMMISSIONING_CAPABILITIES_d                      RSI_ENABLE
#endif

#define g_RX_ON_DEVICE_d                                    RSI_ENABLE

#if ( ( g_Node_Desc_Response_d == RSI_ENABLE ) || ( g_Power_Desc_Response_d ==   RSI_ENABLE ) || \
( g_Simple_Desc_Response_d ==   RSI_ENABLE ) || ( g_Active_Endpoint_Response_d ==    RSI_ENABLE ))
#define g_Device_Desc_Response_Supported_d               RSI_ENABLE
#else
#define g_Device_Desc_Response_Supported_d               RSI_DISABLE
#endif

#define  g_Create_Nwk_Addr_Req_d                           RSI_ENABLE
#define  g_Create_IEEE_Addr_Req_d                          RSI_ENABLE
#define  g_Create_Node_Desc_Req_d                          RSI_ENABLE
#define  g_Create_Power_Desc_Req_d                         RSI_ENABLE
#define  g_Create_Simple_Desc_Req_d                        RSI_ENABLE
#define  g_Create_Active_Endpoint_Req_d                    RSI_ENABLE
#define  g_Create_Match_Desc_Req_d                         RSI_ENABLE
#ifdef ZCP_15
#define  g_Create_Complex_Desc_Req_d                       RSI_ENABLE
#define  g_Create_User_Desc_Req_d                          RSI_ENABLE
#define  g_Create_User_Desc_Set_d                          RSI_ENABLE
#define  g_Create_Discovery_Cache_Req_d                    RSI_ENABLE
#endif // #ifdef ZCP_15
#define  g_Create_Device_Annce_Req_d                       RSI_ENABLE
#define  g_Create_Mgmt_Permit_Joining_Req_d                RSI_ENABLE
#define  g_Create_System_Server_Discovery_Req_d            RSI_DISABLE
#define  g_Create_Bind_Unbind_Req_d                        RSI_ENABLE
#define  g_Create_End_Device_Bind_Req_d                    RSI_ENABLE

#define g_APP_PAN_ID_CONFLICT_d                     1
#define g_PERSISTENT_STACK_TABLES_d                 1
#define g_POLL_d                                    0
#define g_APP_APS_GROUP_DATA_RX_d                   1

/*******************************************************************************
 Z I G B E E    R O U T E R
 *******************************************************************************/

#elif defined(ZIGBEE_ROUTER)
#define g_NWK_FORMATION_d                                   RSI_DISABLE


#define g_NWK_MESH_ROUTING_d                                RSI_ENABLE


#define g_NWK_LEAVE_d                                      RSI_ENABLE
#define g_NWK_PERMIT_JOINING_d                             RSI_ENABLE
#define g_NWK_START_ROUTER_d                               RSI_ENABLE
#define g_NWK_ALLOW_JOIN_d                                 RSI_ENABLE
#define g_NWK_JOIN_NETWORK_d                               RSI_ENABLE
#define g_MULTIPLE_OUTSTANDING_BCAST_d                     RSI_ENABLE

#define g_NWK_BROADCAST_d                                  RSI_ENABLE
#define g_BROADCAST_RETRIES_d                              RSI_ENABLE
#define g_NWK_ROUTE_MAINTENANCE_d                          RSI_ENABLE
#define g_NLME_SYNC_d                                      RSI_DISABLE
#define g_EXTENDED_DEVICE_RESPONSE_d                       RSI_ENABLE
#define g_INTERVAL_TIMER_d                                 RSI_DISABLE
#define g_NWK_LAYER_VALIDATION_d                           RSI_DISABLE
#define g_NLME_ED_SCAN_REQUEST_d                           RSI_ENABLE
#define g_PAN_ID_CONFLICT_d                                RSI_ENABLE
#define g_NWK_USE_MULTICAST_d                              RSI_ENABLE
#define g_NWK_LINK_STATUS_d                                RSI_ENABLE
#define g_NWK_MANY_TO_ONE_ROUTING_d                        RSI_ENABLE

#if ( g_PAN_ID_CONFLICT_d == RSI_ENABLE )
#define g_PAN_ID_CONFLICT_DETECTION_d                      RSI_ENABLE
#define g_PAN_ID_CONFLICT_REPORT_d                         RSI_ENABLE
#define g_PAN_ID_RESOLUTION_d                              RSI_ENABLE
#else
#define g_PAN_ID_CONFLICT_DETECTION_d                      RSI_DISABLE
#define g_PAN_ID_CONFLICT_REPORT_d                         RSI_DISABLE
#define g_PAN_ID_RESOLUTION_d                              RSI_DISABLE
#endif                                          /*( g_PAN_ID_CONFLICT_d == RSI_ENABLE )*/
#define g_NWK_ADDRESS_CONFLICT_DETECTION_d                 RSI_ENABLE
#define g_NWK_ADDRESS_CONFLICT_RESOLUTION_d                RSI_ENABLE



#define g_PERSISTENT_DATA_STORAGE_d                        RSI_ENABLE


#define g_REJOIN_REQUEST_d                                 RSI_ENABLE
#define g_REJOIN_RESPONSE_d                                RSI_ENABLE
#define g_DEVICE_TYPE_c                                     m_ROUTER_c
#define g_APS_GROUP_DATA_RX_d                              RSI_ENABLE
#define g_APS_LAYER_VALIDATION_d                           RSI_ENABLE
#define g_APS_DUPLICATE_REJECTION_c                        RSI_ENABLE
//#ifndef APS_FRAGMENTATION_CAPABILITY
//#define APS_FRAGMENTATION_CAPABILITY                    	RSI_DISABLE
//#endif

//#ifndef g_FREQUENCY_AGILITY_SUPPORTED_d
//#define g_FREQUENCY_AGILITY_SUPPORTED_d                     RSI_DISABLE
//#endif
//#define g_APS_BINDING_CAPACITY_c			                RSI_ENABLE

#define g_TRUST_CENTRE_d                                   RSI_DISABLE
#define g_USE_HASH_KEY                                     RSI_DISABLE
/* security primitives */
#if( g_ZIGBEE_SECURITY_d == RSI_ENABLE )
#define g_APS_LAYER_SEC_PROCESSING_d                       RSI_ENABLE
#define g_NWK_LAYER_SEC_PROCESSING_d                       RSI_ENABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                    RSI_ENABLE
#define g_TRANSPORT_KEY_REQUEST_d                          RSI_DISABLE
#define g_TRANSPORT_KEY_INDICATION_d                       RSI_ENABLE
#define g_UPDATE_DEVICE_REQUEST_d                          RSI_ENABLE
#define g_UPDATE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_REMOVE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_REMOVE_DEVICE_INDICATION_d                       RSI_ENABLE
#define g_SWITCH_KEY_REQUEST_d                             RSI_DISABLE
#define g_SWITCH_KEY_INDICATION_d                          RSI_ENABLE
#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )
#define g_REQUEST_KEY_REQUEST_d                            RSI_ENABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_DISABLE
#else
#define g_REQUEST_KEY_REQUEST_d                            RSI_DISABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_DISABLE
#endif
#else
#define g_APS_LAYER_SEC_PROCESSING_d                       RSI_DISABLE
#define g_NWK_LAYER_SEC_PROCESSING_d                       RSI_DISABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                    RSI_DISABLE
#define g_TRANSPORT_KEY_REQUEST_d                          RSI_DISABLE
#define g_TRANSPORT_KEY_INDICATION_d                       RSI_DISABLE
#define g_UPDATE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_UPDATE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_REMOVE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_REMOVE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_SWITCH_KEY_REQUEST_d                             RSI_DISABLE
#define g_SWITCH_KEY_INDICATION_d                          RSI_DISABLE
#define g_REQUEST_KEY_REQUEST_d                            RSI_DISABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_DISABLE
#endif

#define  g_Nwk_Addr_Response_d                             RSI_ENABLE
#define  g_IEEE_Addr_Response_d                            RSI_ENABLE
#define  g_Node_Desc_Response_d                            RSI_ENABLE
#define  g_Power_Desc_Response_d                           RSI_ENABLE
#define  g_Simple_Desc_Response_d                          RSI_ENABLE
#define  g_Active_Endpoint_Response_d                      RSI_ENABLE
#define  g_Match_Desc_Response_d                           RSI_ENABLE
#define  g_Device_Annce_Process_d                          RSI_ENABLE
#define  g_System_Server_Discovery_Response_d				RSI_DISABLE
#define  g_End_Device_Bind_Response_d                      RSI_DISABLE
#define  g_Unbind_Response_d                               RSI_ENABLE
#define  g_Bind_Response_d                                 RSI_ENABLE
#ifdef ZCP_15
#define  g_Complex_Desc_Response_d                         RSI_ENABLE
#define  g_User_Desc_Response_d                            RSI_ENABLE
#define  g_User_Desc_Conf_d                                RSI_ENABLE
#define  g_Discovery_Cache_Response_d                      RSI_ENABLE
#endif // #ifdef ZCP_15

/*************** Network Mgmt Server Service Primitives ***********************/

#define g_Mgmt_Permit_Joining_Response_d                    RSI_ENABLE

//#if ( g_FREQUENCY_AGILITY_SUPPORTED_d == RSI_ENABLE )
//#define  g_ZDO_Mgmt_NWK_Update_req_d                       RSI_ENABLE
//#define  g_ZDO_Mgmt_NWK_Update_Notify_d                    RSI_ENABLE
//#define  g_Create_Mgmt_Nwk_Update_Req_d                    RSI_ENABLE
//#else
//#define  g_ZDO_Mgmt_NWK_Update_req_d                       RSI_DISABLE
//#define  g_ZDO_Mgmt_NWK_Update_Notify_d                    RSI_DISABLE
//#define  g_Create_Mgmt_Nwk_Update_Req_d                    RSI_DISABLE
//#endif                          /* ( g_FREQUENCY_AGILITY_SUPPORTED_d == RSI_ENABLE ) */
//
//#ifndef g_COMMISSIONING_CAPABILITIES_d
//#define g_COMMISSIONING_CAPABILITIES_d                      RSI_ENABLE
//#endif

#define g_RX_ON_DEVICE_d                                    RSI_ENABLE

#if ( ( g_Node_Desc_Response_d == RSI_ENABLE ) || ( g_Power_Desc_Response_d ==   RSI_ENABLE ) ||\
    (g_Simple_Desc_Response_d ==   RSI_ENABLE ) || (g_Active_Endpoint_Response_d == RSI_ENABLE) )
#define g_Device_Desc_Response_Supported_d                 RSI_ENABLE
#else
#define g_Device_Desc_Response_Supported_d                 RSI_DISABLE
#endif

#define  g_Create_Nwk_Addr_Req_d                           RSI_ENABLE
#define  g_Create_IEEE_Addr_Req_d                          RSI_ENABLE
#define  g_Create_Node_Desc_Req_d                          RSI_ENABLE
#define  g_Create_Power_Desc_Req_d                         RSI_ENABLE
#define  g_Create_Simple_Desc_Req_d                        RSI_ENABLE
#define  g_Create_Active_Endpoint_Req_d                    RSI_ENABLE
#define  g_Create_Match_Desc_Req_d                         RSI_ENABLE
#ifdef ZCP_15
#define  g_Create_Complex_Desc_Req_d                       RSI_ENABLE
#define  g_Create_User_Desc_Req_d                          RSI_ENABLE
#define  g_Create_User_Desc_Set_d                          RSI_ENABLE
#define  g_Create_Discovery_Cache_Req_d                    RSI_ENABLE
#endif // #ifdef ZCP_15
#define  g_Create_Device_Annce_Req_d                       RSI_ENABLE
#define  g_Create_Mgmt_Permit_Joining_Req_d                RSI_ENABLE
#define  g_Create_System_Server_Discovery_Req_d            RSI_ENABLE
#define  g_Create_Bind_Unbind_Req_d                        RSI_ENABLE
#define  g_Create_End_Device_Bind_Req_d                    RSI_ENABLE

#define g_APP_PAN_ID_CONFLICT_d                     1
#define g_PERSISTENT_STACK_TABLES_d                 1
#define g_POLL_d                                    0
#define g_APP_APS_GROUP_DATA_RX_d                   1

/*******************************************************************************
 Z I G B E E   E N D   D E V I C E
 *******************************************************************************/

#elif defined(ZIGBEE_END_DEVICE)
#define g_NWK_FORMATION_d                                  RSI_DISABLE
#define g_NWK_MESH_ROUTING_d                               RSI_DISABLE
#define g_NWK_LEAVE_d                                      RSI_ENABLE
#define g_NWK_PERMIT_JOINING_d                             RSI_DISABLE
#define g_NWK_START_ROUTER_d                               RSI_DISABLE
#define g_NWK_ALLOW_JOIN_d                                 RSI_DISABLE
#define g_NWK_JOIN_NETWORK_d                               RSI_ENABLE
#define g_MULTIPLE_OUTSTANDING_BCAST_d                     RSI_ENABLE

#define g_NWK_BROADCAST_d                                  RSI_DISABLE
#define g_BROADCAST_RETRIES_d                              RSI_DISABLE
#define g_NWK_ROUTE_MAINTENANCE_d                          RSI_DISABLE
#define g_NLME_SYNC_d                                      RSI_ENABLE
#define g_PERSISTENT_DATA_STORAGE_d                        RSI_ENABLE
#define g_REJOIN_REQUEST_d                                 RSI_ENABLE
#define g_REJOIN_RESPONSE_d                                RSI_DISABLE
#define g_EXTENDED_DEVICE_RESPONSE_d                       RSI_DISABLE
#define g_INTERVAL_TIMER_d                                 RSI_DISABLE
#define g_NWK_LAYER_VALIDATION_d                           RSI_DISABLE
#define g_NLME_ED_SCAN_REQUEST_d                           RSI_DISABLE
#define g_PAN_ID_CONFLICT_d                                RSI_DISABLE
#define g_NWK_USE_MULTICAST_d                              RSI_DISABLE
#define g_NWK_LINK_STATUS_d                                RSI_DISABLE

#if ( g_PAN_ID_CONFLICT_d == RSI_ENABLE )
#define g_PAN_ID_CONFLICT_DETECTION_d                      RSI_DISABLE
#define g_PAN_ID_CONFLICT_REPORT_d                         RSI_DISABLE
#define g_PAN_ID_RESOLUTION_d                              RSI_DISABLE
#else
#define g_PAN_ID_CONFLICT_DETECTION_d                      RSI_DISABLE
#define g_PAN_ID_CONFLICT_REPORT_d                         RSI_DISABLE
#define g_PAN_ID_RESOLUTION_d                              RSI_DISABLE
#endif                                          /*( g_PAN_ID_CONFLICT_d == RSI_ENABLE )*/

#define g_NWK_ADDRESS_CONFLICT_DETECTION_d                 RSI_DISABLE
#define g_NWK_ADDRESS_CONFLICT_RESOLUTION_d                RSI_ENABLE

#define g_DEVICE_TYPE_c                                     m_END_DEVICE_c
#define g_APS_GROUP_DATA_RX_d                               RSI_DISABLE

#define g_TRUST_CENTRE_d                                   RSI_DISABLE
#define g_USE_HASH_KEY                                     RSI_DISABLE
/* security primitives*/
#if( g_ZIGBEE_SECURITY_d == RSI_ENABLE )
#define g_APS_LAYER_SEC_PROCESSING_d                       RSI_ENABLE
#define g_NWK_LAYER_SEC_PROCESSING_d                       RSI_ENABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                    RSI_DISABLE
#define g_TRANSPORT_KEY_REQUEST_d                          RSI_DISABLE
#define g_TRANSPORT_KEY_INDICATION_d                       RSI_ENABLE
#define g_UPDATE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_UPDATE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_REMOVE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_REMOVE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_SWITCH_KEY_REQUEST_d                             RSI_DISABLE
#define g_SWITCH_KEY_INDICATION_d                          RSI_ENABLE
#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )
#define g_REQUEST_KEY_REQUEST_d                             1
#define g_REQUEST_KEY_INDICATION_d                          0
#else
#define g_REQUEST_KEY_REQUEST_d                             0
#define g_REQUEST_KEY_INDICATION_d                          0
#endif

#else
#define g_APS_LAYER_SEC_PROCESSING_d                       RSI_DISABLE
#define g_NWK_LAYER_SEC_PROCESSING_d                       RSI_DISABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                    RSI_DISABLE
#define g_TRANSPORT_KEY_REQUEST_d                          RSI_DISABLE
#define g_TRANSPORT_KEY_INDICATION_d                       RSI_DISABLE
#define g_UPDATE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_UPDATE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_REMOVE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_REMOVE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_SWITCH_KEY_REQUEST_d                             RSI_DISABLE
#define g_SWITCH_KEY_INDICATION_d                          RSI_DISABLE
#define g_REQUEST_KEY_REQUEST_d                            RSI_DISABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_DISABLE
#endif

#define g_APS_LAYER_VALIDATION_d                           RSI_ENABLE
#define g_APS_DUPLICATE_REJECTION_c                        RSI_ENABLE

//#ifndef APS_FRAGMENTATION_CAPABILITY
//#define APS_FRAGMENTATION_CAPABILITY                       RSI_DISABLE
//#endif

//#ifndef g_FREQUENCY_AGILITY_SUPPORTED_d
//#define g_FREQUENCY_AGILITY_SUPPORTED_d                    RSI_DISABLE
//#endif
//#define g_APS_BINDING_CAPACITY_c						   	RSI_ENABLE

#define  g_Nwk_Addr_Response_d                             RSI_ENABLE
#define  g_IEEE_Addr_Response_d                            RSI_ENABLE
#define  g_Node_Desc_Response_d                            RSI_ENABLE
#define  g_Power_Desc_Response_d                           RSI_ENABLE
#define  g_Simple_Desc_Response_d                          RSI_ENABLE
#define  g_Active_Endpoint_Response_d                      RSI_ENABLE
#define  g_Match_Desc_Response_d                           RSI_ENABLE
#define  g_Device_Annce_Process_d                          RSI_ENABLE
#define  g_System_Server_Discovery_Response_d				RSI_DISABLE
#define  g_End_Device_Bind_Response_d                      RSI_DISABLE
#define  g_Unbind_Response_d                               RSI_ENABLE
#define  g_Bind_Response_d                                 RSI_ENABLE
#ifdef ZCP_15
#define  g_Complex_Desc_Response_d                         RSI_ENABLE
#define  g_User_Desc_Response_d                            RSI_ENABLE
#define  g_User_Desc_Conf_d                                RSI_ENABLE
#define  g_Discovery_Cache_Response_d                      RSI_ENABLE
#endif // #ifdef ZCP_15

/*----------------- Network Mgmt Server Service Primitives-----------*/

#define g_Mgmt_Permit_Joining_Response_d                    RSI_DISABLE

//#if ( g_FREQUENCY_AGILITY_SUPPORTED_d == RSI_ENABLE )
//#define  g_ZDO_Mgmt_NWK_Update_req_d                       RSI_DISABLE
//#define  g_ZDO_Mgmt_NWK_Update_Notify_d                    RSI_DISABLE
//#define  g_Create_Mgmt_Nwk_Update_Req_d                    RSI_DISABLE
//#else
//#define  g_ZDO_Mgmt_NWK_Update_req_d                       RSI_DISABLE
//#define  g_ZDO_Mgmt_NWK_Update_Notify_d                    RSI_DISABLE
//#define  g_Create_Mgmt_Nwk_Update_Req_d                    RSI_DISABLE
//#endif                              /* ( g_FREQUENCY_AGILITY_SUPPORTED_d == RSI_ENABLE )*/

#ifndef g_COMMISSIONING_CAPABILITIES_d
#define g_COMMISSIONING_CAPABILITIES_d                      RSI_ENABLE
#endif

#define g_RX_ON_DEVICE_d                                    RSI_DISABLE

#if ( ( g_Node_Desc_Response_d == RSI_ENABLE ) || ( g_Power_Desc_Response_d ==   RSI_ENABLE ) ||\
( g_Simple_Desc_Response_d ==   RSI_ENABLE ) || ( g_Active_Endpoint_Response_d == RSI_ENABLE ) )
#define g_Device_Desc_Response_Supported_d                  RSI_ENABLE
#else
#define g_Device_Desc_Response_Supported_d                  RSI_DISABLE
#endif

#define g_Create_Nwk_Addr_Req_d                            RSI_ENABLE
#define g_Create_IEEE_Addr_Req_d                           RSI_ENABLE
#define g_Create_Node_Desc_Req_d                           RSI_ENABLE
#define g_Create_Power_Desc_Req_d                          RSI_ENABLE
#define g_Create_Simple_Desc_Req_d                         RSI_ENABLE
#define g_Create_Active_Endpoint_Req_d                     RSI_ENABLE
#define g_Create_Match_Desc_Req_d                          RSI_ENABLE
#ifdef ZCP_15
#define  g_Create_Complex_Desc_Req_d                       RSI_ENABLE
#define  g_Create_User_Desc_Req_d                          RSI_ENABLE
#define  g_Create_User_Desc_Set_d                          RSI_ENABLE
#define  g_Create_Discovery_Cache_Req_d                    RSI_ENABLE
#endif // #ifdef ZCP_15
#define g_Create_Device_Annce_Req_d                        RSI_ENABLE
#define g_Create_Mgmt_Permit_Joining_Req_d                 RSI_ENABLE
#define g_Create_System_Server_Discovery_Req_d             RSI_ENABLE
#define g_Create_Bind_Unbind_Req_d                         RSI_ENABLE
#define g_Create_End_Device_Bind_Req_d                     RSI_ENABLE

#define g_APP_PAN_ID_CONFLICT_d                     0
#define g_PERSISTENT_STACK_TABLES_d                 1
#define g_POLL_d                                    1
#define g_APP_APS_GROUP_DATA_RX_d                   0

/*******************************************************************************
 Z I G B E E   A C T I V E   E N D   D E V I C E
 *******************************************************************************/

#elif defined(ZIGBEE_ACTIVE_END_DEVICE)

#define g_ZIGBEE_END_DEVICE_d                              RSI_ENABLE
#define g_NWK_FORMATION_d                                  RSI_DISABLE
#define g_NWK_MESH_ROUTING_d                               RSI_DISABLE
#define g_NWK_LEAVE_d                                      RSI_ENABLE
#define g_NWK_PERMIT_JOINING_d                             RSI_DISABLE
#define g_NWK_START_ROUTER_d                               RSI_DISABLE
#define g_NWK_ALLOW_JOIN_d                                 RSI_DISABLE
#define g_NWK_JOIN_NETWORK_d                               RSI_ENABLE
#define g_MULTIPLE_OUTSTANDING_BCAST_d                     RSI_ENABLE

#define g_NWK_BROADCAST_d                                  RSI_ENABLE
#define g_BROADCAST_RETRIES_d                              RSI_DISABLE
#define g_NWK_ROUTE_MAINTENANCE_d                          RSI_DISABLE
#define g_NLME_SYNC_d                                      RSI_DISABLE
#define g_PERSISTENT_DATA_STORAGE_d                        RSI_ENABLE
#define g_REJOIN_REQUEST_d                                 RSI_ENABLE
#define g_REJOIN_RESPONSE_d                                RSI_DISABLE
#define g_EXTENDED_DEVICE_RESPONSE_d                       RSI_DISABLE
#define g_INTERVAL_TIMER_d                                 RSI_DISABLE
#define g_NWK_LAYER_VALIDATION_d                           RSI_DISABLE
#define g_NLME_ED_SCAN_REQUEST_d                           RSI_ENABLE
#define g_PAN_ID_CONFLICT_d                                RSI_DISABLE
#define g_NWK_USE_MULTICAST_d                              RSI_DISABLE

#if ( g_PAN_ID_CONFLICT_d == RSI_ENABLE )
#define g_PAN_ID_CONFLICT_DETECTION_d                      RSI_DISABLE
#define g_PAN_ID_CONFLICT_REPORT_d                         RSI_DISABLE
#define g_PAN_ID_RESOLUTION_d                              RSI_DISABLE
#else
#define g_PAN_ID_CONFLICT_DETECTION_d                      RSI_DISABLE
#define g_PAN_ID_CONFLICT_REPORT_d                         RSI_DISABLE
#define g_PAN_ID_RESOLUTION_d                              RSI_DISABLE
#endif                                          /*( g_PAN_ID_CONFLICT_d == RSI_ENABLE )*/

#define g_NWK_ADDRESS_CONFLICT_DETECTION_d                  RSI_DISABLE
#define g_NWK_ADDRESS_CONFLICT_RESOLUTION_d                 RSI_ENABLE

#define g_DEVICE_TYPE_c                                     m_END_DEVICE_c
#define g_APS_GROUP_DATA_RX_d                              RSI_ENABLE

#define g_TRUST_CENTRE_d                                   RSI_DISABLE
#define g_USE_HASH_KEY                                     RSI_DISABLE
/* security primitives*/
#if( g_ZIGBEE_SECURITY_d == RSI_ENABLE )
#define g_NWK_LAYER_SEC_PROCESSING_d                       RSI_ENABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                    RSI_DISABLE
#define g_TRANSPORT_KEY_REQUEST_d                          RSI_DISABLE
#define g_TRANSPORT_KEY_INDICATION_d                       RSI_ENABLE
#define g_UPDATE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_UPDATE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_REMOVE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_REMOVE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_SWITCH_KEY_REQUEST_d                             RSI_DISABLE
#define g_SWITCH_KEY_INDICATION_d                          RSI_ENABLE
#define g_REQUEST_KEY_REQUEST_d                            RSI_ENABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_DISABLE
#else
#define g_NWK_LAYER_SEC_PROCESSING_d                       RSI_DISABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                    RSI_DISABLE
#define g_TRANSPORT_KEY_REQUEST_d                          RSI_DISABLE
#define g_TRANSPORT_KEY_INDICATION_d                       RSI_DISABLE
#define g_UPDATE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_UPDATE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_REMOVE_DEVICE_REQUEST_d                          RSI_DISABLE
#define g_REMOVE_DEVICE_INDICATION_d                       RSI_DISABLE
#define g_SWITCH_KEY_REQUEST_d                             RSI_DISABLE
#define g_SWITCH_KEY_INDICATION_d                          RSI_DISABLE
#define g_REQUEST_KEY_REQUEST_d                            RSI_DISABLE
#define g_REQUEST_KEY_INDICATION_d                         RSI_DISABLE
#endif

#define g_APS_LAYER_SEC_PROCESSING_d                       RSI_ENABLE
#define g_APS_LAYER_VALIDATION_d                           RSI_ENABLE
#define g_APS_DUPLICATE_REJECTION_c                        RSI_ENABLE

//#ifndef APS_FRAGMENTATION_CAPABILITY
//#define APS_FRAGMENTATION_CAPABILITY                        RSI_DISABLE
//#endif
//#ifndef g_FREQUENCY_AGILITY_SUPPORTED_d
//#define g_FREQUENCY_AGILITY_SUPPORTED_d                     RSI_DISABLE
//#endif
//#define g_APS_BINDING_CAPACITY_c						   	RSI_ENABLE

#define  g_Nwk_Addr_Response_d                              RSI_ENABLE
#define  g_IEEE_Addr_Response_d                             RSI_ENABLE
#define  g_Node_Desc_Response_d                             RSI_ENABLE
#define  g_Power_Desc_Response_d                            RSI_ENABLE
#define  g_Simple_Desc_Response_d                           RSI_ENABLE
#define  g_Active_Endpoint_Response_d                       RSI_ENABLE
#define  g_Match_Desc_Response_d                            RSI_ENABLE
#define  g_Device_Annce_Process_d                           RSI_ENABLE
#define  g_System_Server_Discovery_Response_d				RSI_DISABLE
#define  g_End_Device_Bind_Response_d                       RSI_DISABLE
#define  g_Unbind_Response_d                                RSI_ENABLE
#define  g_Bind_Response_d                                  RSI_ENABLE
#ifdef ZCP_15
#define  g_Complex_Desc_Response_d                          RSI_ENABLE
#define  g_User_Desc_Response_d                             RSI_ENABLE
#define  g_User_Desc_Conf_d                                 RSI_ENABLE
#define  g_Discovery_Cache_Response_d                       RSI_ENABLE
#endif // #ifdef ZCP_15

/*************** Network Mgmt Server Service Primitives ***********************/
#define g_Mgmt_Permit_Joining_Response_d                    RSI_DISABLE

//#if ( g_FREQUENCY_AGILITY_SUPPORTED_d == RSI_ENABLE )
//#define  g_ZDO_Mgmt_NWK_Update_req_d                        RSI_DISABLE
//#define  g_ZDO_Mgmt_NWK_Update_Notify_d                     RSI_DISABLE
//#define  g_Create_Mgmt_Nwk_Update_Req_d                     RSI_DISABLE
//#else
//#define  g_ZDO_Mgmt_NWK_Update_req_d                        RSI_DISABLE
//#define  g_ZDO_Mgmt_NWK_Update_Notify_d                     RSI_DISABLE
//#define  g_Create_Mgmt_Nwk_Update_Req_d                     RSI_DISABLE
//#endif                          /* ( g_FREQUENCY_AGILITY_SUPPORTED_d == RSI_ENABLE ) */

#ifndef g_COMMISSIONING_CAPABILITIES_d
#define g_COMMISSIONING_CAPABILITIES_d                      RSI_ENABLE
#endif
#define g_RX_ON_DEVICE_d                                    RSI_ENABLE

#if ( ( g_Node_Desc_Response_d == RSI_ENABLE ) || ( g_Power_Desc_Response_d ==   RSI_ENABLE ) ||\
( g_Simple_Desc_Response_d ==   RSI_ENABLE ) || ( g_Active_Endpoint_Response_d ==RSI_ENABLE ) )
#define g_Device_Desc_Response_Supported_d                  RSI_ENABLE
#else
#define g_Device_Desc_Response_Supported_d                  RSI_DISABLE
#endif

#define g_Create_Nwk_Addr_Req_d                             RSI_ENABLE
#define g_Create_IEEE_Addr_Req_d                            RSI_ENABLE
#define g_Create_Node_Desc_Req_d                            RSI_ENABLE
#define g_Create_Power_Desc_Req_d                           RSI_ENABLE
#define g_Create_Simple_Desc_Req_d                          RSI_ENABLE
#define g_Create_Active_Endpoint_Req_d                      RSI_ENABLE
#define g_Create_Match_Desc_Req_d                           RSI_ENABLE
#ifdef ZCP_15
#define  g_Create_Complex_Desc_Req_d                        RSI_ENABLE
#define  g_Create_User_Desc_Req_d                           RSI_ENABLE
#define  g_Create_User_Desc_Set_d                           RSI_ENABLE
#define  g_Create_Discovery_Cache_Req_d                     RSI_ENABLE
#endif // #ifdef ZCP_15
#define g_Create_Device_Annce_Req_d                         RSI_ENABLE
#define g_Create_Mgmt_Permit_Joining_Req_d                  RSI_ENABLE
#define g_Create_System_Server_Discovery_Req_d              RSI_ENABLE
#define  g_Create_Bind_Unbind_Req_d                         RSI_ENABLE
#define  g_Create_End_Device_Bind_Req_d                     RSI_ENABLE

#ifdef g_ZIGBEE_END_DEVICE_d
#undef g_ZIGBEE_END_DEVICE_d
#undef g_POLL_d
#define g_ZIGBEE_END_DEVICE_d                       1
#endif                                              /*g_ZIGBEE_END_DEVICE_d*/
#define g_APP_PAN_ID_CONFLICT_d                     0
//#define g_FREQUENCY_AGILITY_SUPPORTED_d             0
#define g_PERSISTENT_STACK_TABLES_d                 1
#define g_POLL_d                                    0
#define g_APP_APS_GROUP_DATA_RX_d                   0

/*******************************************************************************
 U N I F I E D    D E V I C E
 *******************************************************************************/
#else
#define g_NWK_FORMATION_d                                   RSI_ENABLE



#define g_NWK_MESH_ROUTING_d                                RSI_ENABLE


#define g_NWK_LEAVE_d                                       RSI_ENABLE
#define g_NWK_PERMIT_JOINING_d                              RSI_ENABLE
#define g_NWK_START_ROUTER_d                                RSI_ENABLE
#define g_NWK_ALLOW_JOIN_d                                  RSI_ENABLE
#define g_CHILD_DEVICE_AUTHENTICATION_c                     RSI_ENABLE
#define g_NWK_JOIN_NETWORK_d                                RSI_ENABLE
#define g_MULTIPLE_OUTSTANDING_BCAST_d                      RSI_ENABLE

#define g_NWK_BROADCAST_d                                   RSI_ENABLE
#define g_NWK_ROUTE_MAINTENANCE_d                           RSI_ENABLE
#define g_NLME_SYNC_d                                       RSI_ENABLE
#define g_INTERVAL_TIMER_d                                  RSI_DISABLE
#define g_NWK_LAYER_VALIDATION_d                            RSI_DISABLE
#define g_NLME_ED_SCAN_REQUEST_d                            RSI_ENABLE
#define g_NWK_USE_MULTICAST_d                               RSI_DISABLE


#define g_PERSISTENT_DATA_STORAGE_d                         RSI_DISABLE


#define g_REJOIN_REQUEST_d                                  RSI_ENABLE
#define g_REJOIN_RESPONSE_d                                 RSI_ENABLE
#define g_DEVICE_TYPE_c                                     m_UNIFIED_c
#define g_APS_GROUP_DATA_RX_d                               RSI_ENABLE
#define g_APS_LAYER_SEC_PROCESSING_d                        RSI_DISABLE
#define g_APS_LAYER_VALIDATION_d                            RSI_ENABLE
#define g_APS_DUPLICATE_REJECTION_c                         RSI_ENABLE

//#ifndef APS_FRAGMENTATION_CAPABILITY
//#define APS_FRAGMENTATION_CAPABILITY                        RSI_DISABLE
//#endif

//#define g_APS_BINDING_CAPACITY_c						   	RSI_ENABLE
//#ifndef g_FREQUENCY_AGILITY_SUPPORTED_d
//#define g_FREQUENCY_AGILITY_SUPPORTED_d                     RSI_DISABLE
//#endif
#define g_ZIGBEE_SECURITY_d                                 RSI_ENABLE
#define g_TRUST_CENTRE_d                                    RSI_ENABLE
#define g_USE_HASH_KEY                                      RSI_DISABLE

/* ZDP primitives */
#define g_Nwk_Addr_Response_d                               RSI_ENABLE
#define g_IEEE_Addr_Response_d                              RSI_ENABLE
#define g_Node_Desc_Response_d                              RSI_ENABLE
#define g_Power_Desc_Response_d                             RSI_ENABLE
#define g_Simple_Desc_Response_d                            RSI_ENABLE
#define g_Active_Endpoint_Response_d                        RSI_ENABLE
#define g_Match_Desc_Response_d                             RSI_ENABLE
#define g_Device_Annce_Process_d                            RSI_ENABLE
#define  g_Unbind_Response_d                                RSI_ENABLE
#define  g_Bind_Response_d                                  RSI_ENABLE
#ifdef ZCP_15
#define  g_Complex_Desc_Response_d                          RSI_ENABLE
#define  g_User_Desc_Response_d                             RSI_ENABLE
#define  g_User_Desc_Conf_d                                 RSI_ENABLE
#define  g_Discovery_Cache_Response_d                       RSI_ENABLE
#endif // #ifdef ZCP_15

/**************** Network Mgmt Server Service Primitives **********************/
//#define g_ZDO_Mgmt_NWK_Update_req_d                         RSI_ENABLE
//#define g_ZDO_Mgmt_NWK_Update_Notify_d                      RSI_ENABLE

#ifndef g_COMMISSIONING_CAPABILITIES_d
#define g_COMMISSIONING_CAPABILITIES_d                      RSI_ENABLE
#endif

#define g_RX_ON_DEVICE_d                                    RSI_ENABLE

#if ( ( g_Node_Desc_Response_d == RSI_ENABLE ) || ( g_Power_Desc_Response_d ==   RSI_ENABLE ) ||\
( g_Simple_Desc_Response_d == RSI_ENABLE ) || ( g_Active_Endpoint_Response_d ==RSI_ENABLE ) ||\
(g_Match_Desc_Response_d == RSI_ENABLE ))

#define g_Device_Desc_Response_Supported_d                  RSI_ENABLE
#else
#define g_Device_Desc_Response_Supported_d                  RSI_DISABLE
#endif

#define g_Create_Nwk_Addr_Req_d                             RSI_ENABLE
#define g_Create_IEEE_Addr_Req_d                            RSI_ENABLE
#define g_Create_Node_Desc_Req_d                            RSI_ENABLE
#define g_Create_Power_Desc_Req_d                           RSI_ENABLE
#define g_Create_Simple_Desc_Req_d                          RSI_ENABLE
#define g_Create_Active_Endpoint_Req_d                      RSI_ENABLE
#define g_Create_Match_Desc_Req_d                           RSI_ENABLE
#ifdef ZCP_15
#define  g_Create_Complex_Desc_Req_d                        RSI_ENABLE
#define  g_Create_User_Desc_Req_d                           RSI_ENABLE
#define  g_Create_User_Desc_Set_d                           RSI_ENABLE
#define  g_Create_Discovery_Cache_Req_d                     RSI_ENABLE
#endif // #ifdef ZCP_15
#define g_Create_Device_Annce_Req_d                         RSI_ENABLE
#define g_Create_Mgmt_Permit_Joining_Req_d                  RSI_ENABLE
//#define g_Create_Mgmt_Nwk_Update_Req_d                      RSI_DISABLE
#define  g_Create_Bind_Unbind_Req_d                         RSI_ENABLE
#define  g_Create_End_Device_Bind_Req_d                     RSI_ENABLE

#define g_APP_PAN_ID_CONFLICT_d                     1
#define g_PERSISTENT_STACK_TABLES_d                 1
#define g_POLL_d                                    0
#define g_APP_APS_GROUP_DATA_RX_d                   1

#endif


#ifndef ZIGBEE_LIB_MEMORY_OPTIMIZATION
//#define ZIGBEE_LIB_MEMORY_OPTIMIZATION               RSI_ENABLE
#define ZIGBEE_LIB_MEMORY_OPTIMIZATION               RSI_DISABLE
#endif
/* Note: This macro either enables writing tables to NVM or disables writing of
 * tables to NVM. The macro's below should not be disables or enabled
 * individually. */

#if (ZIGBEE_LIB_MEMORY_OPTIMIZATION == RSI_ENABLE)
#define g_BIND_TABLE_NVM_STORAGE_d                  RSI_ENABLE
#define g_ADDRESS_MAP_NVM_STORAGE_d                 RSI_ENABLE
#define g_GROUP_TABLE_NVM_STORAGE_d                 RSI_ENABLE
#define g_APP_LINK_KEY_TABLE_NVM_STORAGE_d          RSI_ENABLE
#define g_UPDATE_TABLES_TO_PERSISETENT_STORAGE      RSI_ENABLE
#define g_CBKE_INFO_TABLE_NVM_STORAGE_d             RSI_ENABLE
#else
#define g_BIND_TABLE_NVM_STORAGE_d                  RSI_DISABLE
#define g_ADDRESS_MAP_NVM_STORAGE_d                 RSI_DISABLE
#define g_GROUP_TABLE_NVM_STORAGE_d                 RSI_DISABLE
#define g_APP_LINK_KEY_TABLE_NVM_STORAGE_d          RSI_DISABLE
#define g_UPDATE_TABLES_TO_PERSISETENT_STORAGE      RSI_DISABLE
#define g_CBKE_INFO_TABLE_NVM_STORAGE_d             RSI_DISABLE
#endif /*ZIGBEE_LIB_MEMORY_OPTIMIZATION*/



/* ~~~~~~~~~~~~ MTORR and source routing feature related macro ~~~~~~~~~~~  */
/******THIS MACRO SHOULD NOT BE DISABLED. THIS IS MANDATORY
**********************************************************/


/* MANDATORY MACROS*/

#define g_NWK_CREATE_ROUTE_RECORD_FRAME_d           RSI_ENABLE
#define g_NWK_CREATE_MTORR_FRAME_d                  RSI_ENABLE

#define g_NWK_ROUTE_RECORD_TABLE_d                  RSI_ENABLE
#define g_NWK_PARSE_SOURCE_ROUTE_FRAME_d            RSI_ENABLE


/*OPTIONAL MACROS*/
#define g_NWK_CREATE_SOURCE_ROUTE_FRAME_d           RSI_ENABLE

/* ~~~~~~~~~~~~MTORR and source routing feature related macro ~~~~~~~~ */

/*******************************************************************************
 * Type Definitions
 *******************************************************************************/

/* None */

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/

/* None */

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/* None */

/*******************************************************************************
 * End Of File
 *******************************************************************************/
#endif                                                  /*STACK_FUNCTIONALITY_H*/
