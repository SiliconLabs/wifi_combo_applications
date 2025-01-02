/*******************************************************************************
 * nlme_interface.h
 ********************************************************************************
 * Program Description:
 *
 *     This file contains the interface functions and data structures to be used
 *     by the NHLE for accessing management services.
 *
 *******************************************************************************/

#ifndef NLME_INTERFACE_H
#define NLME_INTERFACE_H

/*******************************************************************************
 * Includes
 *******************************************************************************/

#include "ZigBeeDataTypes.h"
#include <stdbool.h>
#include "stack_functionality.h"

/*******************************************************************************
 * Global Constants
 *******************************************************************************/


/*******************************************************************************
 g_NWK_SYM_LINK_c

 - This macro defines the current route symmetry setting as follows:

 TRUE  - Indicates that routes are considered to be comprised of
 symmetric links. Backward and forward routes are created
 during one-route discovery and they are identical

 FALSE - Indicates that routes are not considered to be comprised
 of symmetric links.Only the forward route is stored
 during route discovery. This value needs to be constant
 across all devices in the network
******************************************************************************/
#define g_NWK_SYM_LINK_c                            g_TRUE_c

/*******************************************************************************

 g_BROADCAST_PASSIVE_ACK_TIME_OUT_c

 - This macro defines the passive acknowledgement time out for
 broadcast packets and is 0.5 seconds as per ZigBee Stack Profile.


******************************************************************************/
#define g_BROADCAST_PASSIVE_ACK_TIME_OUT_c                  0.5

/*******************************************************************************

 The following macro has been defined to hold 4 times the value of
 the passive ack time out value(in secs). This value will be stored
 in the NWK Layer info base. Broadcast module when using this value
 from the NWK layer info base will multiply the value by 250 to get
 the value in milliseconds.

******************************************************************************/
#define g_FOUR_TIMES_OF_BROADCAST_PASSIVE_ACK_TIME_OUT_c            \
                              g_BROADCAST_PASSIVE_ACK_TIME_OUT_c * 4


/*******************************************************************************

 g_BROADCAST_DELIVERY_TIME_c

 - This macro defines the time for the broadcast packet delivery. It
 is nine seconds as per ZigBee Stack Profile.

******************************************************************************/
#define g_BROADCAST_DELIVERY_TIME_c                         0x09

/*******************************************************************************

 g_MAX_BROADCAST_RETRIES_c

 - This macro defines the maximum number of retries allowed after
 a broadcast transmission failure.


******************************************************************************/
#define g_MAX_BROADCAST_RETRIES_c                       0x02



#define g_NWK_IS_CONCENTRATOR_DEF_VALUE_c               0x00



#define g_NWK_CONCENTRATOR_RADIUS_DEF_VALUE_c           0x001e



/*******************************************************************************

 g_PROTOCOL_VERSION_c

 - This macro defines the current protocol version in use.

******************************************************************************/
#define g_PROTOCOL_VERSION_c                 0x02


/*******************************************************************************
	g_STACK_PROFILE_c

 - This macro defines the value for stack profile in the ZigBee
 Specification 2006.

 - Contains the current Stack Profile in use

 0x01 is used for ZigBee Stack Profile

 0x02 is used for ZigBee Pro Stack Profile

******************************************************************************/

#define g_STACK_PROFILE_c                                   0x02

/*******************************************************************************
 g_NETWORK_KEY_DEF_SEQUENCE_NUMBER_c

 - default active sequence number
******************************************************************************/

#define g_NETWORK_KEY_DEF_SEQUENCE_NUMBER_c         0x00



/*******************************************************************************
 default values of properatory NIB attributes that  require
 non persistent storage
******************************************************************************/
#define g_NWK_NBT_CURRENT_INDEX_DEF_VALUE_c         0x00

#define g_NWK_RT_CURRENT_INDEX_DEF_VALUE_c          0x00

#define g_NWK_SEC_MAT_DESCRIP_INDEX_DEF_VALUE_C     0x00

#define g_NWK_LOGICAL_CHANNEL_DEF_VALUE_c           0xFF

#define g_NWK_LOGICAL_CHANNEL_DEF_VALUE_c           0xFF

#define g_NWK_ENERGY_THRESHOLD_DEF_VALUE_c          0xFF

#define g_NWK_REPORT_CONSTANT_COST_DEF_VALUE_c      0x00

#define g_NWK_CAPABILITY_INFORMATION_DEF_VALUE_c    0x00
#define g_NWK_USE_TREE_ADDRESS_ALLOC_DEF_VALUE_c    g_TRUE_c


#define g_NWK_ROUTE_DISCOVERY_RETRIES_DEF_VALUE_c   0x03



/*******************************************************************************

 g_NWK_TRANSACTION_PERSISTENCE_TIME_c

 - This macro defines the maximum time (in superframe periods)
 required  by the coordinator or router to store a transaction. The
 time is indicated in its beacon. This attribute reflects the value
 of the MAC PIB attribute and MAC Transaction Persistence Time. Any
 changes made by the higher layer is reflected in the MAC PIB
 attribute value as well.

******************************************************************************/
#define g_NWK_TRANSACTION_PERSISTENCE_TIME_c                0x01F4


/*******************************************************************************
g_NWK_UNIQUE_ADDRESS_c

 -This macro indicates the whether unique address is sending or not
******************************************************************************/
#define g_NWK_UNIQUE_ADDRESS_c                              g_FALSE_c

/*******************************************************************************
g_NWK_LEAVE_REQUEST_ALLOWED_c

 -This macro indicates the whether unique address is sending or not
******************************************************************************/

#ifdef APPLY_R_20_CHANGE
#define g_NWK_LEAVE_REQUEST_ALLOWED_c                   g_TRUE_c
#endif

/*******************************************************************************

 g_NWK_SECURE_ALL_FRAMES_c

 - This macro defines whether security is applied for incoming and
 outgoing network data frames or not.

******************************************************************************/
#define g_NWK_SECURE_ALL_FRAMES_c                  0x01

/*******************************************************************************
 g_NWK_ALT_PROTOCOL_VERSION_c

 - Sets the list of protocol version numbers,other than the current
 protocol version number, that the device may choose to employ in a
 PAN that it joins; This attribute is applicable only to ZigBee
 routers or end devices .The protocol version numbers in the list
 must refer to older versions of the ZigBee Specification.

******************************************************************************/
#define g_NWK_ALT_PROTOCOL_VERSION_c               0x00

/*******************************************************************************

 m_NWK_MAX_DEPTH_c

 - This macro defines the allowed depth of a device. This value is
 determined  for all devices in the network as per stack profile
 settings. This has to be constant across all devices in the network.
  this macro is different from application configured value
******************************************************************************/
#define m_NWK_MAX_DEPTH_c                                   15


/* default values of NIB attributes that does not require persistent storage */
#define g_NWK_SEQUENCE_NUMBER_DEF_VALUE_c           0xFF




/*
 g_USE_TREE_ROUTING_c

 - This macro defines the flag that determines the ability of the
 network layer to use hierarchical routing:

 TRUE  - Assumes the ability to use hierarchical routing

 FALSE - Never uses hierarchical routing

 */
#define g_USE_TREE_ROUTING_c                                g_FALSE_c


/*******************************************************************************
 This enumeration Specifies the Message that is exchanged between
 NWK layer and NHLE.
 *******************************************************************************/
enum NLME_primitive_id {
    g_NLME_Start_Router_Req_c = 0x73,
    g_NLME_Start_Router_Conf_c = 0x74,
    g_NLME_Permit_Joining_Req_c = 0x75,
    g_NLME_Permit_Joining_Conf_c = 0x76,
    g_NLME_Join_Req_c = 0x77,
    g_NLME_Join_Conf_c = 0x78,
    g_NLME_Join_Ind_c = 0x79,
    g_NLME_Leave_Req_c = 0x7A,
    g_NLME_Leave_Conf_c = 0x7B,
    g_NLME_Leave_Ind_c = 0x7C,
    g_NLME_Get_Req_c = 0x82,
    g_NLME_Set_Req_c = 0x84,
    g_NLME_Network_Discovery_Req_c = 0x86,
    g_NLME_Network_Discovery_Conf_c = 0x87,
    g_NLME_Network_Formation_Req_c = 0x88,
    g_NLME_Network_Formation_Conf_c = 0x89,
    g_NLME_Sync_Req_c = 0x8a,
    g_NLME_Sync_Conf_c = 0x8b,
    g_NLME_Sync_Ind_c = 0x8c,
    g_NLME_Route_Discovery_Req_c = 0x90,
    g_NLME_Route_Discovery_Conf_c = 0x91,
    g_NLME_Network_Status_Ind_c = 0x92,
    g_NLME_ED_Scan_Req_c = 0x8d,
    g_NLME_ED_Scan_Conf_c = 0x8e,
    g_Unsolicited_Rejoin_Indication_c = 0x8F

};

/*******************************************************************************

 These enumerations are used to Get/Set NIB Attributes.

 g_NWK_SequenceNumber_c
 -A sequence number used to identify outgoing frames.

 g_NWK_PassiveAckTimeout_c
 -The maximum time duration in seconds allowed for the parent and
 all child devices to retransmit a broadcast message.

 g_NWK_MaxBroadcastRetries_c
 -The maximum number of retries allowed after a broadcast
 transmission failure.

 g_NWK_MaxChildren_c
 -The number of children a device is allowed to have on its current
 network. This value is determined by stack profile for all devices
 in the network.This has to be constant across all devices in the
 network.

 g_NWK_MaxDepth_c
 -The depth a device can have. This value is determined  for all
 devices in the network as per stack profile settings.This has to
 be constant across all devices in the network.

 g_NWK_MaxRouter_c
 -The number of routers any one device is allowed to have as
 children.This value is determined for all devices in the network as
 per stack profile settings. This has to be constant across all
 devices in the network.

 g_NWK_NeighborTable_c
 -The current set of neighbor table entries in the device.

 g_NWK_BroadcastDeliveryTime_c
 -Time duration in seconds that a broadcast message needs to
 encompass the entire network.

 g_NWK_ReportConstantCosts_c
 -If this is set to 0, the NWK layer shall calculate link cost from
 all neighbor nodes using the LQI values reported by the MAC layer;
 Otherwise,it shall report a constant value.

 g_NWK_RouteDiscoveryRetriesPermitted_c
 -The number of retries allowed after an unsuccessful route request.

 g_NWK_RouteTable_c
 -The current set of routing table entries in the device.

 g_NWK_SymLink_c
 -The current route symmetry setting:

 TRUE means that routes are considers to be comprised of
 symmetric links. Backward and forward routes are created during
 one-route discovery and they are identical.

 FALSE indicates that routes are not consider to be comprised of
 symmetric links.Only the forward route is stored during route
 discovery.This value has to be constant across all devices in
 the network.

 g_NWK_CapabilityInformation_c
 -This field shall contain the device capability Information
 established at network joining time.

 g_NWK_UseTreeAddressAlloc_c
 -A value that determines the method used to assign addresses:

 0x00 - Use distributed address allocation

 0x01 - Reserved

 0x02 - Use stochastic address allocation

 g_NWK_UseTreeRouting_c
 -A flag that determines whether the NWK layer should assume the
 ability to use hierarchical routing.

 TRUE  - assume the ability to use hierarchical routing

 FALSE - never use hierarchical routing

 g_NWK_TransactionPersistenceTime_c
 -The maximum time (in superframe periods) that a transaction is
 stored by a coordinator/router and indicated in its beacon. This
 attribute reflects the value of the MAC PIB attribute
 macTransaction PersistenceTime and any changes made by the higher
 layer will be reflected in the MAC PIB attribute value as well.

 g_NWK_ShortAddress_c
 -The 16-bit address that the device uses to communicate with the
 PAN.This attribute reflects the value of the MAC PIB attribute
 macShortAddress and any changes made by the higher layer will be
 reflected in the MAC PIB attribute value as well.

 g_NWK_StackProfile_c
 -The identifier of the ZigBee stack profile in use for this device.

 g_NWK_ExtendedPan_ID_c
 -The Extended PAN Identifier for the PAN of which the device is a
 member. The value 0x0000000000000000 means the Extended PAN
 Identifier will be the extended address of coordinator.

 g_NWK_SecurityLevel_c
 -The security level for outgoing and incoming NWK frames.

 g_NWK_SecurityMaterialSet_c
 -Set of network security material descriptors capable of
 maintaining an active and alternate Network key.

 g_NWK_ActiveKeySeqNumber_c
 -The sequence number of the active Network key in
 nwkSecurityMaterialSet.

 g_NWK_AllFresh_c
 -Indicates whether incoming NWK frames must be all checked for
 freshness when the memory for incoming frame counts is exceeded.

 g_NWK_SecureAllFrames_c
 -This indicates if security shall be applied to incoming and
 outgoing NWK data frames;if set to 0x01, security processing shall
 be applied to all incoming and outgoing frames except data frames
 destined for the current device that have the security sub-field of
 the frame control field set to 0; if this attribute has a value of
 0x01 the NWK layer shall not relay frames that have the security
 sub-field of the frame control field set to 0; The SecurityEnable
 parameter of the NLDE-DATA.request primitive shall override the
 setting of this attribute.

 g_NWK_KeySeqNumber_c
 -A sequence number assigned to a Network key by the trust center
 and used to distinguish Network keys for purposes of key updates,
 and incoming frame security operations.

 g_NWK_OutgoingFrameCounter_c
 -Outgoing frame counter used for outgoing frames.

 g_NWK_IncomingFrameCounterSet_c
 -Set of incoming frame counter values and corresponding device
 addresses.

 g_NWK_Key_c
 -The actual value of the key.

 g_NWK_security_material_descriptor_index_c
 -Gives the index of the security material set structure to which
 the data has to be written.

 g_NWK_Logical_Channel_c
 -Channel of PAN to which device has joined.

 *******************************************************************************/

/*******************************************************************************
 Network Information Base Attributes
 *******************************************************************************/
enum {
    g_NWK_SequenceNumber_c = 0x81,
    g_NWK_PassiveAckTimeout_c = 0x82,
    g_NWK_MaxBroadcastRetries_c = 0x83,
    g_NWK_MaxChildren_c = 0x84,
    g_NWK_MaxDepth_c = 0x85,
    g_NWK_MaxRouter_c = 0x86,
    g_NWK_NeighborTable_c = 0x87,
    g_NWK_BroadcastDeliveryTime_c = 0x88,
    g_NWK_ReportConstantCosts_c = 0x89,
    g_NWK_RouteDiscoveryRetriesPermitted_c = 0x8A,
    g_NWK_RouteTable_c = 0x8B,

    /* Attribute required for ZigBee 2007 frequency Agility feature*/
    g_NWK_Tx_Total_c = 0x8D,

    g_NWK_SymLink_c = 0x8E,
    g_NWK_CapabilityInformation_c = 0x8F,
    g_NWK_UseTreeAddressAlloc_c = 0x90,
    g_NWK_UseTreeRouting_c = 0x91,

    /*This is required for ZigBee 2007 - nwkManagerAddr*/
    g_NWK_Manager_Address_c = 0x92,

    /* Attribute required for ZigBee 2007*/
    g_NWK_Update_Id_c = 0x94,

    g_NWK_TransactionPersistenceTime_c = 0x95,
    g_NWK_ShortAddress_c = 0x96,
    g_NWK_StackProfile_c = 0x97,
    g_Reserved_c = 0x98,
    g_NWK_ExtendedPan_ID_c = 0x9A,
    g_NWK_RouteRecordTable_c = 0x9C,
    g_NWK_Is_Concentrator_c = 0x9D,
    g_NWK_Concentrator_Radius_c = 0x9E,
    g_NWK_Concentrator_DiscoveryTime_c = 0x9F,

    /* security related Attributes */
    g_NWK_SecurityLevel_c = 0xA0,
    g_NWK_SecurityMaterialSet_c = 0xA1,
    g_NWK_ActiveKeySeqNumber_c = 0xA2,
    g_NWK_AllFresh_c = 0xA3,
    g_NWK_SecureAllFrames_c = 0xA5,
#ifdef APPLY_R_20_CHANGE
    g_NWK_LeaveRequestAllowed_c = 0xAA,
#endif
    /* network properatory attributes     */
    g_NWK_current_neighbor_index_c = 0xB0,
    g_NWK_current_route_index_c = 0xB1,
    g_NWK_security_material_descriptor_index_c = 0xB2,
    g_NWK_current_device_type_c = 0xB3,
    g_NWK_number_of_active_children_joined_c = 0xB4,
    g_NWK_number_of_routers_joined_c = 0xB5,
    g_NWK_number_of_end_devices_joined_c = 0xB6,
    g_NWK_panid_c = 0xB8,
    g_NWK_current_device_depth_c = 0xB9,
    g_NWK_Logical_Channel_c = 0xBA,
    g_NWK_passive_ack_neighbor_table_index_c = 0xBB,
    g_NWK_Energy_Threshold_Value_c = 0xBC

};

/* Network Status indication- status codes */
enum Network_Status_Codes {
    g_Network_Status_No_Route_Available_c,
    g_Network_Status_Tree_Link_Failure_c,
    g_Network_Status_Non_Tree_Failure_c,
    g_Network_Status_Low_Battery_Level_c,
    g_Network_Status_No_Routing_Capacity_c,
    g_Network_Status_No_Indirect_Capacity_c,
    g_Network_Status_Indirect_Transaction_Expiry_c,
    g_Network_Status_Target_Device_Unavailable_c,
    g_Network_Status_Target_Address_Unallocated_c,
    g_Network_Status_Parent_Link_Failure_c,
    g_Network_Status_Validate_Route_c,
    g_Network_Status_Source_Route_Failure_c,
    g_Network_Status_Many_To_One_Route_Failure_c,
    g_Network_Status_Address_Conflict_c,
    g_Network_Status_Verify_Addresses_c,
    g_Network_Status_PAN_Identifier_Update_c,
    g_Network_Status_Address_Update_c,
    g_Network_Status_Bad_Frame_Counter_c,
    g_Network_Status_Bad_Key_Sequence_Number_c
};
/*****************************************************************************
 ZigBeeDeviceType
 ******************************************************************************
 ZigBeeUnknownDevice
 -Device is not joined
 ZigBeeCoordinator
 - Will relay messages and can act as a parent to other nodes
 ZigBeeRouter
 - Will relay messages and can act as a parent to other nodes
 ZigBeeEndDevice
 - Communicates only with its parent and will not relay messages
 ZigBeeSleepingEndDevice
 - An end device whose radio can be turned off to save power.
 The application must ZigBeePollForData() to receive messages
 ZigBeeMobileEndDevice
 -A sleepy end device that can move through the network

 ******************************************************************************/

typedef enum {
    ZigBeeCoordinator,
    ZigBeeRouter,
    ZigBeeEndDevice,
   // ZigBeeSleepingEndDevice,
    ZigBeeActiveEndDevice,
    ZigBeeMobileEndDevice,
    ZigBeeUnknownDevice,
} ZigBeeDeviceType;


/*****************************************************************************
This macro is used in application to decide about the concentrator as high or low
concentrator.If it is High concentrator then Concentrator is capable of storing
the route to destination device otherwise no.

******************************************************************************/

typedef enum {
    ZigBeeLowRAMConcentrator = 0x00,
    ZigBeeHighRAMConcentrator = 0x01
}ZigBeeConcentratorType;

/******************************************************************************
    ZigBeeNetworkDetails : The parameters of a ZigBee network.
 ******************************************************************************/

typedef struct{
    uint16_t shortPanId; /* The network's PAN identifier.*/
    uint8_t channel; /* The 802.15.4 channel associated with the network.*/
    uint8_t extendedPanId[8]; /*The network's extended PAN identifier. */
    uint8_t stackProfile; /* The Stack Profile associated with the network. */
    uint8_t nwkUpdateId; /* The instance of the Network.*/
    bool allowingJoining; /* Whether the network is allowing MAC associations.*/
}__attribute__((__packed__))ZigBeeNetworkDetails;



extern uint8_t gConcentratorType ;
/*******************************************************************************
 *  MACRO for accessing network layer attributes.
 *******************************************************************************/

/* The following macro can be used to get( read ) network attributes.
 attribute_id  is enum defined for that particular attribute */
#define g_GET_NIB(attribute_id)   m_GET_##attribute_id

/* MACROS for getting nib attributes  */

#define m_GET_g_NWK_SequenceNumber_c   g_NWK_Info_Base.NWK_sequence_number

#define m_GET_g_NWK_PassiveAckTimeout_c g_NWK_Info_Base.NWK_passive_ack_timeout

#define m_GET_g_NWK_MaxBroadcastRetries_c \
            g_NWK_Info_Base.NWK_max_broadcast_retries
/* No restriction for max CHILDREN ZigBee PRO*/
/*
#define m_GET_g_NWK_MaxChildren_c g_NWK_Info_Base.NWK_max_children
*/
#define m_GET_g_NWK_MaxDepth_c g_NWK_Info_Base.NWK_max_depth
/*
#define m_GET_g_NWK_MaxRouter_c g_NWK_Info_Base.NWK_max_router
*/
#define m_GET_g_NWK_NeighborTable_c gp_Neighbor_Table

#if ( g_NWK_MANY_TO_ONE_ROUTING_d == 1 )
#if (g_NWK_ROUTE_RECORD_TABLE_d == 1)
#define m_GET_g_NWK_RouteRecordTable_c gp_route_record_table
#endif //g_NWK_ROUTE_RECORD_TABLE_d
#endif

#define m_GET_g_NWK_BroadcastDeliveryTime_c \
        g_NWK_Info_Base.NWK_network_broadcast_delivery_time

#define m_GET_g_NWK_ReportConstantCosts_c \
        g_NWK_Info_Base.NWK_report_constant_cost

#define m_GET_g_NWK_RouteDiscoveryRetriesPermitted_c \
        g_NWK_Info_Base.NWK_route_discovery_retries_permitted

#define m_GET_g_NWK_RouteTable_c gp_RouterTable

#define m_GET_g_NWK_SymLink_c g_NWK_Info_Base.NWK_sym_link

#define m_GET_g_NWK_unique_address g_NWK_Info_Base.NWK_unique_address

#define m_GET_g_NWK_CapabilityInformation_c \
        g_NWK_Info_Base.NWK_capability_information

#define m_GET_g_NWK_UseTreeAddressAlloc_c \
        g_NWK_Info_Base.NWK_use_tree_addr_alloc

#define m_GET_g_NWK_UseTreeRouting_c g_NWK_Info_Base.NWK_use_tree_routing

#define m_GET_g_NWK_TransactionPersistenceTime_c \
        g_NWK_Info_Base.NWK_transaction_persistence_time

#define m_GET_g_NWK_Energy_Threshold_Value_c g_NWK_Info_Base.NWK_energy_threshold

#define m_GET_g_NWK_ShortAddress_c  gp_Persistent_NIB->NWK_short_address
#define m_GET_g_NWK_StackProfile_c g_NWK_Info_Base.NWK_stack_profile

#define m_GET_g_NWK_Is_Concentrator_c \
        g_NWK_Info_Base.NWK_Is_Concentrator

#define m_GET_g_NWK_Concentrator_Radius_c \
        g_NWK_Info_Base.NWK_Concentrator_Radius

#define m_GET_g_NWK_Concentrator_DiscoveryTime_c \
        g_NWK_Info_Base.NWK_Concentrator_DiscoveryTime

#ifdef APPLY_R_20_CHANGE
#define m_GET_g_NWK_LeaveRequestAllowed_c        \
        g_NWK_Info_Base.NWK_leave_request_allowed
#endif

#define m_GET_g_NWK_ExtendedPan_ID_c \
        gp_Persistent_NIB->NWK_extended_panid

/* This macro is used for getting the nwkUpdateID attribute which
 has been added for ZigBee 2007*/
#define m_GET_g_NWK_Update_ID_c gp_Persistent_NIB->NWK_update_id

/* This macro is used for getting the nwk_tx_total attribute which
 has been added for ZigBee 2007*/
#define m_GET_g_NWK_Tx_Total_c  gp_Persistent_NIB->NWK_tx_total

#define m_GET_g_NWK_tx_failure_count_c  gp_Persistent_NIB->NWK_tx_failure_count

/*properatory attributes */

#define m_GET_g_NWK_current_device_type_c  \
        gp_Persistent_NIB->NWK_current_device_type

#define m_GET_g_NWK_number_of_active_children_joined_c  \
        gp_Persistent_NIB->NWK_number_of_active_children_joined

#define m_GET_g_NWK_number_of_routers_joined_c \
        gp_Persistent_NIB->NWK_number_of_routers_joined

#define m_GET_g_NWK_number_of_end_devices_joined_c \
        gp_Persistent_NIB->NWK_number_of_end_devices_joined

#define m_GET_g_NWK_panid_c gp_Persistent_NIB->NWK_pan_id

#define m_GET_g_NWK_passive_ack_neighbor_table_index_c \
        gp_Persistent_NIB->NWK_passive_ack_neighbor_table_index

#define m_GET_g_NWK_current_device_depth_c \
        gp_Persistent_NIB->NWK_current_device_depth

#define m_GET_g_NWK_Logical_Channel_c   g_NWK_Info_Base.NWK_logical_channel

/* Security Attributes */
//#define m_GET_g_NWK_SecurityLevel_c gp_Persistent_NIB->NWK_security_level
#define m_GET_g_NWK_SecurityLevel_c Get_Nwk_SecurityLevel()

#define m_GET_g_NWK_ActiveKeySeqNumber_c \
        gp_Persistent_NIB->NWK_active_key_seq_number

#define m_GET_g_NWK_AllFresh_c gp_Persistent_NIB->NWK_all_fresh

#define m_GET_g_NWK_SecureAllFrames_c gp_Persistent_NIB->NWK_secure_all_frames

/* security material */
#define m_GET_g_NWK_SecurityMaterialSet_c gp_Security_Material_Descriptor

/*Network Manager Address*/
#define m_GET_g_NWK_Manager_Address_c  gp_Persistent_NIB->NWK_manager_address

/* Returns the size of the route table */
#define ZigBeeGetRouteTableSize()        (sizeof(Route_Discovery_Table_t))
/******************** MACROS for Setting nib attributes  *********************/
#define g_SET_NIB(attribute_id, value)   m_SET_##attribute_id( value )

#define m_SET_g_NWK_SequenceNumber_c( value )                    \
        g_NWK_Info_Base.NWK_sequence_number = value

#define m_SET_g_NWK_PassiveAckTimeout_c( value )                 \
        g_NWK_Info_Base.NWK_passive_ack_timeout= value

#define m_SET_g_NWK_MaxBroadcastRetries_c( value )               \
        g_NWK_Info_Base.NWK_max_broadcast_retries= value

/* No restriction for max CHILDREN ZigBee PRO*/
/*
#define m_SET_g_NWK_MaxChildren_c( value )                       \
        g_NWK_Info_Base.NWK_max_children= value
*/
#define m_SET_g_NWK_MaxDepth_c( value )                          \
        g_NWK_Info_Base.NWK_max_depth= value

/*
#define m_SET_g_NWK_MaxRouter_c( value )                         \
        g_NWK_Info_Base.NWK_max_router= value
*/
#define m_SET_g_NWK_NeighborTable_c( value )                     \
        memUtils_memCopy(gp_Neighbor_Table,value,sizeof(NeighborTable_t))

#define m_SET_g_NWK_BroadcastDeliveryTime_c( value )             \
        g_NWK_Info_Base.NWK_network_broadcast_delivery_time = value

#define m_SET_g_NWK_UNIQUE_ADDRESS_c( value )                   \
        g_NWK_Info_Base.NWK_unique_address = value

#define m_SET_g_NWK_ReportConstantCosts_c( value )               \
        g_NWK_Info_Base.NWK_report_constant_cost = value

#define m_SET_g_NWK_RouteDiscoveryRetriesPermitted_c( value )    \
        g_NWK_Info_Base.NWK_route_discovery_retries_permitted = value

#define m_SET_g_NWK_RouteTable_c( value )                        \
        memUtils_memCopy(gp_Router_Table,value,sizeof(Router_Table_t))

#define m_SET_g_NWK_SymLink_c( value )                           \
        g_NWK_Info_Base.NWK_sym_link = value

#define m_SET_g_NWK_CapabilityInformation_c( value )             \
        g_NWK_Info_Base.NWK_capability_information = value

#define m_SET_g_NWK_UseTreeAddressAlloc_c( value )               \
        g_NWK_Info_Base.NWK_use_tree_addr_alloc = value

#define m_SET_g_NWK_UseTreeRouting_c( value )                    \
        g_NWK_Info_Base.NWK_use_tree_routing = value

#define m_SET_g_NWK_TransactionPersistenceTime_c( value )        \
        g_NWK_Info_Base.NWK_transaction_persistence_time = value

#define m_SET_g_NWK_Energy_Threshold_Value_c( value )            \
        g_NWK_Info_Base.NWK_energy_threshold = value

#define m_SET_g_NWK_ShortAddress_c( value )                      \
        gp_Persistent_NIB->NWK_short_address = value

#define m_SET_g_NWK_StackProfile_c( value )                      \
         g_NWK_Info_Base.NWK_stack_profile = value

#define m_SET_g_NWK_ExtendedPan_ID_c( value )                    \
        memUtils_memCopy (gp_Persistent_NIB->NWK_extended_panid, value,  \
        g_EXTENDED_PANID_LENGTH_c );

#define m_SET_g_NWK_Is_Concentrator_c( value )                   \
         g_NWK_Info_Base.NWK_Is_Concentrator = value

#define m_SET_g_NWK_Concentrator_Radius_c( value )               \
         g_NWK_Info_Base.NWK_Concentrator_Radius = value

#define m_SET_g_NWK_Concentrator_DiscoveryTime_c( value )        \
         g_NWK_Info_Base.NWK_Concentrator_DiscoveryTime = value

#ifdef APPLY_R_20_CHANGE
#define m_SET_g_NWK_leave_request_allowed_c( value )        \
         g_NWK_Info_Base.NWK_leave_request_allowed = value
#endif



/*properatory attributes */

#define m_SET_g_NWK_current_device_type_c( value )               \
        gp_Persistent_NIB->NWK_current_device_type = value

#define m_SET_g_NWK_number_of_active_children_joined_c( value )  \
        gp_Persistent_NIB->NWK_number_of_active_children_joined = value

#define m_SET_g_NWK_number_of_routers_joined_c( value )          \
        gp_Persistent_NIB->NWK_number_of_routers_joined = value

#define m_SET_g_NWK_number_of_end_devices_joined_c( value )      \
        gp_Persistent_NIB->NWK_number_of_end_devices_joined  = value

#define m_SET_g_NWK_panid_c( value )                             \
        gp_Persistent_NIB->NWK_pan_id = value

#define m_SET_g_NWK_passive_ack_neighbor_table_index_c( value )  \
        gp_Persistent_NIB->NWK_passive_ack_neighbor_table_index = value

#define m_SET_g_NWK_current_device_depth_c( value )              \
        gp_Persistent_NIB->NWK_current_device_depth = value

#define m_SET_g_NWK_Logical_Channel_c( value )                   \
        g_NWK_Info_Base.NWK_logical_channel  = value

/* Security Attributes */
#define m_SET_g_NWK_SecurityLevel_c( value )                     \
        gp_Persistent_NIB->NWK_security_level = value

#define m_SET_g_NWK_ActiveKeySeqNumber_c( value )                \
        gp_Persistent_NIB->NWK_active_key_seq_number = value

#define m_SET_g_NWK_AllFresh_c( value )                          \
        gp_Persistent_NIB->NWK_all_fresh = value

#define m_SET_g_NWK_SecureAllFrames_c( value )                   \
        gp_Persistent_NIB->NWK_secure_all_frames = value

#define m_SET_g_NWK_security_material_descriptor_index_c( value )\
        g_NWK_Info_Base.NWK_security_material_descriptor_index = value

/* This macro is used for setting the NWK_Manager_Address attribute which
 has been added for ZigBee 2007 */

#define m_SET_g_NWK_Manager_Address_c( value )                   \
        gp_Persistent_NIB->NWK_manager_address = value

/* This macro is used for setting the nwkUpdateID attribute which
 has been added for ZigBee 2007 */
#define m_SET_g_NWK_Update_ID_c( value )                         \
        gp_Persistent_NIB->NWK_update_id = value

/* This macro is used for setting the nwk_tx_total attribute which
 has been added for ZigBee 2007*/
#define m_SET_g_NWK_Tx_Total_c( value )                          \
        gp_Persistent_NIB->NWK_tx_total = value

#define m_SET_NWK_tx_failure_count_c( value )                    \
        gp_Persistent_NIB->NWK_tx_failure_count = value

/*******************************************************************************
 * Type Definitions
 *******************************************************************************/

/*******************************************************************************

 NLME_Network_Discovery_Request_t

 This structure allows the next higher layer to request that the
 NWK layer discover networks currently operating within the POS,
 Before forming the network or joining an existing the network.

 scan_channels
 -This field indicates the list of channels that need to be
 discovered.

 scan_duration
 -This field indicates the duration of scan for each channel.

 *******************************************************************************/

typedef struct NLME_Network_Discovery_Request_Tag {
    uint32_t scan_channels;
    uint8_t scan_duration;
}__attribute__((__packed__)) NLME_Network_Discovery_Request_t;

/*******************************************************************************
 NLME_ED_Scan_Request_t

 This structure is used to issue NLME ED scan request from the
 Application.

 scan_channels
 - This field indicates the list of channels that need to be
 discovered.

 scan_duration
 - This field indicates the duration of scan for each channel.

 *******************************************************************************/

typedef struct NLME_ED_Scan_Request_Tag {
    uint32_t scan_channels;
    uint8_t scan_duration;
}__attribute__((__packed__)) NLME_ED_Scan_Request_t;

/*******************************************************************************
 NLME_Network_Formation_Request_t

 This structure allows the next higher layer to request that the
 device start a new ZigBee network with itself as the coordinator
 and subsequently make changes to its superframe configuration.

 scan_channels
 -Indicate which channels are to be scanned.

 scan_duration
 -A value used to calculate the length of time to spend scanning
 each channel;The time spent scanning each channel is
 (aBaseSuperframeDuration * (2n + 1)) symbols, where n is the value
 of the ScanDuration parameter.

 beacon_order
 -The value of the network that the higher layers wishes to form.The
 Beaconorder of 0x0F indicates a BeaconLess Network.

 superframe_order
 -The value of the network that the higher layers wishes to form. If
 the BeaconOrder is 0x0F this parameter is ignored.

 battery_life_extension
 TRUE    - use battery life extension

 FALSE   - Do Not use battery life extension

 *******************************************************************************/

typedef struct NLME_Network_Formation_Request_Tag {
    uint32_t scan_channels;
    uint8_t scan_duration;
    uint8_t beacon_order;
    uint8_t superframe_order;
    uint8_t battery_life_extension;
}__attribute__((__packed__)) NLME_Network_Formation_Request_t;

/*******************************************************************************
 NLME_Permit_Join_Request_t

 The Permit Join Request allows ZC or ZR to set its MAC sub-layer
 association permit flag for a fixed period, while it accepts
 devices onto its network.

 permit_duration
 -The length of time in seconds during which the ZC or ZR allow
 associations.The values without a specified time limit are as
 follows:

 0x00 - Permission is disabled

 0xff - Permission is enabled

 *******************************************************************************/

typedef struct NLME_Permit_Join_Request_Tag {
    uint8_t permit_duration;
}__attribute__((__packed__)) NLME_Permit_Join_Request_t;

/*******************************************************************************
 NLME_Start_Router_Request_t

 This structure allows a ZigBee router that is newly joined to a
 network to begin engaging in the activities expected of a ZigBee
 router including the routing of data frames, route discovery, route
 repair and the accepting of requests to join the network from other
 devices.It also allows a ZigBee router to set up its superframe
 configuration.It may also be used by a ZigBee router to reconfigure
 its superframe.

 beacon_order
 -The value of the network that the higher layers wishes to start as
 a Router.The Beaconorder of 0x0F indicates a BeaconLess Network.

 superframe_order
 -The value of the network that the higher layers wishes to start as
 a Router. If the BeaconOrder is 0x0F this parameter is ignored.

 battery_life_extension
 TRUE    - use battery life extension

 FALSE   - Do Not use battery life extension

 *******************************************************************************/
typedef struct NLME_Start_Router_Request_Tag {
    uint8_t beacon_order;
    uint8_t superframe_order;
    uint8_t battery_life_extension;
}__attribute__((__packed__)) NLME_Start_Router_Request_t;

/*******************************************************************************
 NLME_Join_Request_t

 This structure allows the next higher layer to request to join a
 network either through association or directly or to re-join a
 network if orphaned.

 a_extended_pan_id[8]
 -The 64-bit PAN identifier of the network to join.

 join_as_router
 TRUE  - The device joins as a router

 FALSE - The device joins as an end-device

 rejoin_network
 -This parameter controls the method of joining the network.If the
 parameter is

 0x00 - device is requesting to join a network through
 association

 0x02 - device is joining the network using the NWK rejoining
 procedure

 scan_channels
 -Indicates which channels are to be scanned.

 scan_duration
 -A value used to calculate the length of time to spend scanning
 each channel;The time spent scanning each channel is
 (aBaseSuperframeDuration * (2n + 1)) symbols, where n is the value
 of the ScanDuration parameter.
 power_source
 -The parameters value determine the source of power.

 0x00 - Battery powered

 0x01 - Mains powered

 rx_on_when_idle
 TRUE  - The receiver is enabled when the device is idle

 FALSE - The receiver may be disabled when the device is idle

 security_enable
 0x01 - MAC security is used

 0x00 - MAC security disabled

 *******************************************************************************/
typedef struct NLME_Join_Request_Tag {
    uint8_t a_extended_pan_id[8];
    uint8_t rejoin_network;
    uint32_t scan_channels;
    uint8_t scan_duration;
    uint8_t capability_information;
    uint8_t security_enable;
}__attribute__((__packed__)) NLME_Join_Request_t;

/*******************************************************************************
 NLME_Leave_Request_t

 This structure allows the next higher layer to enable the current
 device to leave the network or to request the child device to leaves
 the network.

 a_device_address[8]
 -The 64-Bit IEEE address of the device being asked to leave the
 network or NULL in case it is the current device is leaving the
 network.

 remove_children

 TRUE  - if the device being asked to leave the network is also
 being asked to remove its child devices

 FALSE - if the device being asked to leave the network is NOT being
 asked to remove its child devices.

 rejoin
 -This is a bit field. Bit 0 is used to indicate rejoin.

 If Bit0 is
 1  -  indicates rejoin is true
 0  -  0 indicates rejoin is false.

 If Bit1 is
 0  -  reuse of address is set to false
 1  -  reuse of address is set to true

 If Bit2
 0  -  indicates it is silent
 1  -  indicates it is not silent

 *******************************************************************************/

typedef struct NLME_Leave_Request_Tag {
    uint8_t a_device_address[8];
    uint8_t remove_children;
#ifndef APPLY_R_19_CHANGE
    uint8_t rejoin;
#else
    bool rejoin;
#endif
}__attribute__((__packed__)) NLME_Leave_Request_t;

/*******************************************************************************
 NLME_Route_Discovery_Request_t

 This structure allows the next higher layer to initiate route
 discovery.

 dest_addr_mode
 -This field indicates the destination address mode of the route
 discovery. The values are as follows:

 g_Dest_Addr_Mode_No_Addr (0x00)         - No destination address

 g_Dest_Addr_Mode_Multicast_Addr  (0x01) - 16-bit network address
 of a multicast group

 g_Dest_Addr_Mode_Individual_Addr (0x02) - 16-bit network address
 of an individual device

 dest_addr
 -This field indicates the destination of the route discovery. The
 values are as follows:

 0x00 - No destination address supplied

 0x01 - Multicast route discovery

 0x02 - Unicast route discovery

 radius
 -This field describes the number of hops that the route request
 travels through the network.
 no_route_cache
 - In the case where DstAddrMode has a value of zero, indicating
 many-to-one route discovery, this flag determines whether the NWK should
 establish a route record table.
 TRUE = no route record table should be
 established
 FALSE = establish a route record table

 *******************************************************************************/
typedef struct NLME_Route_Discovery_Request_Tag {
    uint8_t dest_addr_mode;
    uint16_t dest_addr;
    uint8_t radius;
    uint8_t no_route_cache;
}__attribute__((__packed__)) NLME_Route_Discovery_Request_t;

/*******************************************************************************
 NLME_Sync_Request_t

 This structure is used when the device wants to synchronize or
 extract data from its ZigBee coordinator or router.

 track
 TRUE  - synchronization should be maintained for future beacons

 FALSE - synchronization should NOT be maintained for future beacons

 *******************************************************************************/
typedef struct NLME_Sync_Request_Tag {
    uint8_t track;
}__attribute__((__packed__)) NLME_Sync_Request_t;

/*******************************************************************************
 NLME Indication Primitives

 The NLME Indication primitives are declared as structures with one member for
 each management indication primitive.
 The first byte is reserved for the Primitive enumration. The structure name
 is only to be used by the Indication buffer union.
 *******************************************************************************/

/*******************************************************************************
 NLME_Join_Indication_t

 This structure allows the next higher layer of a ZigBee coordinator
 or ZigBee router to be notified when a new device has successfully
 joined its network by association or rejoined using the NWK rejoin
 procedure.

 short_address
 -The network address of an entity that has been added to the
 network.

 a_extended_address[8]
 -The 64-bit IEEE address of an entity that has been added to the
 network.

 capability_Information
 -The operating capabilities of the device being directly joined

 BIT No.

 0      - alternate Pan coordinator( always has a
 value 0 for this implementation)

 1      - device type (0->if join_as_router is FALSE
 else it is TRUE)

 2      - power source (1->if mains powered,0->if
 battery powered)

 3      - rx_on_when_idle( 1-> receiver is enabled
 when idle,0-> receiver is disabled when
 idle)

 4-5     - reserved

 6      - reserved

 7      - allocate address(This field will always have
 a value of 1 in implementations of this \
                             specification,indicating that the joining
 device must be issued a 16-bit short
 address)
 rejoining_network
 - The RejoinNetwork parameter indicating the method used to join the
 network.
 The parameter is 0x00 if the device joined through association.
 The parameter is 0x01 if the device joined directly or rejoined using
 orphaning.
 The parameter is 0x02 if the device used  NWK rejoin

 secure_join
 - If this parameter is
 TRUE  - the association was performed in a secure manner

 FALSE - the association was performed in a unsecure manner

 *******************************************************************************/

typedef struct NLME_Join_Indication_Tag {
    uint16_t short_address;
    uint8_t a_extended_address[8];
    uint8_t capability_Information;
    uint8_t rejoining_network;
    uint8_t secure_join;
} __attribute__((__packed__)) NLME_Join_Indication_t;

/*******************************************************************************
 NLME_Leave_Indication_t

 This structure is used by the device to enable the next higher
 layer to be notified if that device has left the network or if a
 neighboring device has left the network.

 a_device_address[8]
 -The 64-Bit IEEE address of the device leaving the network.

 rejoin
 TRUE  - if the device being asked to leave from the current parent
 is requested to rejoin the network

 FALSE - if the device being asked to leave from the current parent
 is NOT requested to rejoin the network.

 is_child
 TRUE  - if the device is being left is immediate child

 FALSE - if the device is being left is not immediate child

 *******************************************************************************/
typedef struct NLME_Leave_Indication_Tag {
    uint8_t a_device_address[8];
    uint8_t rejoin;
    uint8_t is_child;
}__attribute__((__packed__)) NLME_Leave_Indication_t;

/*******************************************************************************
 NLME_Network_Status_Indication_t

 This structure  allows the next higher layer to be notified of the
 failure of a communication path across the network.


 Status
 -The error code associated with the routing failure.

 short_address
 -The 16-bit network address of the destination device associated
 with the routing failure.

 *******************************************************************************/

typedef struct NLME_Network_Status_Indication_Tag {
    uint16_t short_address;
    uint8_t status;
}__attribute__((__packed__)) NLME_Network_Status_Indication_t;

/*******************************************************************************
 NLME Confirm Primitives

 The NLME Confirm primitives are declared as structures with one member for
 each management confirm primitive.
 The first byte is reserved for the Primitive enumration.
 *******************************************************************************/

/*******************************************************************************
 Network_Descriptor_t

 Network decriptor structure is updated when PAN Descriptors are
 received for the Active Scan performed as a Part of
 NLME-Network.Discovery operation.Network Descriptors gives details
 about the Neighboring Routers and Coordinators.

 logical_channel
 -The current logical channel occupied by the network.

 pan_id
 -The 16-bit PAN identifier of the discovered network;the 2
 highestorder bits of this parameter are reserved and shall be set
 to 0.

 a_extended_pan_id[8]
 -The 64-bit PAN identifier of the network.

 stack_profile
 -The ZigBee stack profile identifier indicating the stack profile
 in use in the discovered network.This value is set to 0x01.

 zigbee_version
 -The version of the ZigBee protocol in use in the discovered
 network.

 beacon_order
 -The beacon order of the beacon found during active scan.

 superframe_order
 -The superframe order of the beacon found during active scan.

 permit_joining
 TRUE  -   device is allowing association

 FALSE -   device is NOT allowing association
 router_capacity
 - This value is set to true if the device is capable of accepting
 join requests from router-capable devices and set to FALSE otherwise.
 end_device_capacity
 -This value is set to true if the device is capable of accepting
 join requests from end devices and set to FALSE otherwise.

 *******************************************************************************/
typedef struct Network_Descriptor_Tag {
    uint16_t pan_id;
    uint8_t logical_channel;
    uint8_t a_extended_pan_id[8];
    uint8_t stack_profile;
    uint8_t zigbee_version;
    uint8_t beacon_order;
    uint8_t superframe_order;
    uint8_t permit_joining;
    uint8_t router_capacity;
    uint8_t end_device_capacity;
}__attribute__((__packed__)) Network_Descriptor_t;

/*******************************************************************************
 ZigBee_Network_Descriptor_t
    pan_id
     -The 16-bit PAN identifier of the discovered network;the 2
     highestorder bits of this parameter are reserved and shall be set
     to 0.

     a_extended_pan_id[8]
     -The 64-bit PAN identifier of the network.

    radio_tx_power
    -Radio transmit power of te device

    logical_channel
     -The current logical channel occupied by the network.
*******************************************************************************/
typedef struct{
    uint8_t a_extended_pan_id[8];
    uint16_t pan_id;
    int8_t radio_tx_power;
    uint8_t logical_channel;
}__attribute__((__packed__))ZigBee_Network_Descriptor_t;

/*******************************************************************************
 NLME_Network_Discovery_Confirm_t

 This structure reports the results of a network discovery operation

 Status
 -This field indicates the status of Discover Network Request as
 follows:

 SUCCESS     - Discovers network successfully

 NO_NETWORKS - Network is not available

 network_count
 -This field indicates the count of discovered networks.

 a_network_descriptor[1]
 -This field indicates the network descriptor list based on the
 network count.

 *******************************************************************************/

typedef struct NLME_Network_Discovery_Confirm_Tag {
    uint8_t status;
    uint8_t network_count;
    Network_Descriptor_t a_network_descriptor[1];
}__attribute__((__packed__)) NLME_Network_Discovery_Confirm_t;

/*******************************************************************************
 NLME_Network_Formation_Confirm_t

 This structure reports the results of the request to initialize a
 ZigBee coordinator in a network.

 Status
 -The result of the attempt to initialize a ZigBee coordinator or
 request a change to the superframe configuration.The status value
 returned are
 g_Invalid_Request_c

 g_Startup_Failure_c

 *******************************************************************************/

typedef struct NLME_Network_Formation_Confirm_Tag {
    uint8_t status;
}__attribute__((__packed__)) NLME_Network_Formation_Confirm_t;

/*******************************************************************************
 NLME_Permit_Joining_Confirmation_t

 This structure allows the next higher layer of a ZigBee
 coordinator or router to be notified of the results of its request
 to permit the acceptance of devices onto the network.

 Status
 -This field indicates the status of Permit Join Request as follows:

 SUCCESS  (0x00)         - Successfully updates permit join
 duration

 INVALID_REQUEST (0xFF)  - Device is not capable of processing
 permit join request

 *******************************************************************************/

typedef struct NLME_Permit_Joining_Confirmation_Tag {
    uint8_t status;
} __attribute__((__packed__))NLME_Permit_Joining_Confirmation_t;

/*******************************************************************************
 NLME_Start_Router_Confirm_t

 This structure reports the results of the request to initialize or
 change the superframe configuration of a ZigBee router.

 Status
 -The result of the attempt to initialize a ZigBee router.
 g_Invalid_Request_c

 *******************************************************************************/

typedef struct NLME_Start_Router_Confirm_Tag {
    uint8_t status;
} __attribute__((__packed__)) NLME_Start_Router_Confirm_t;

/*******************************************************************************
 NLME_Join_Confirm_t

 This structure allows the next higher layer to be notified of the
 results of its request to join a network.

 Status
 - The status of the corresponding NLME-Join.Request
 g_Invalid_Request_c
 g_Not_Permitted_c
 g_No_Networks_c

 shortaddress
 - The 16-bit short address that was allocated to this device.This
 parameter will be equal to 0xffff if the join attempt was
 unsuccessful.

 a_extended_pand_id[8]
 - The PAN identifier for the network of which the device is now a
 member.

 active_channel
 - The value of phyCurrentChannel attribute of the PHY PIB, which is
 equal to the current channel of the network that has been joined.

 *******************************************************************************/
typedef struct NLME_Join_Confirm_Tag {
    uint8_t status;
    uint16_t shortaddress;
    uint8_t a_extended_pand_id[8];
    uint8_t active_channel;
} __attribute__((__packed__)) NLME_Join_Confirm_t;

/*******************************************************************************
 NLME_Leave_Confirm_t

 This structure allows the next higher layer to be notified of the
 results of its request for itself or another device to leave the
 network.

 Status
 - The status of the corresponding NLME-Leave.request are
 g_Success_c
 g_Unknown_Device_c
 g_Invalid_Request_c

 a_device_address[8]
 -The 64-bit IEEE address in the request to which this is a
 confirmation  or null if the device requested to remove itself from
 the network.

 *******************************************************************************/

typedef struct NLME_Leave_Confirm_Tag {
    uint8_t status;
    uint8_t a_device_address[8];
}__attribute__((__packed__)) NLME_Leave_Confirm_t;

/******************************************************************************
 NLME_Route_Discovery_Confirm_t

 This structure is called when the device wants to report about the
 results of a Route Discovery operation.

 Status
 - The status of an attempt to initiate route discovery
 g_Invalid_Request_c

 g_No_Routing_Capacity_c

 g_Route_Discovery_Failed_c

 nwk_status_code
 - In the case where the Status parameter has a value of ROUTE_ERROR,
 this code gives further information about the kind of error that
 occurred. Otherwise, it should be ignored
 *******************************************************************************/

typedef struct NLME_Route_Discovery_Confirm_Tag {
    uint8_t status;
    uint8_t nwk_status_code;
}__attribute__((__packed__)) NLME_Route_Discovery_Confirm_t;

/*******************************************************************************
 NLME_Sync_Confirm_t

 This structure is called when the device wants to report about the
 results of a sync request operation.

 Status
 -The result of the request to synchronize with the ZigBee
 coordinator or router are
 g_Success_c

 g_Sync_Failure_c

 g_Invalid_Parameter_c

 *******************************************************************************/
typedef struct NLME_Sync_Confirm_Tag {
    uint8_t status;
}__attribute__((__packed__)) NLME_Sync_Confirm_t;

/*******************************************************************************
 NLME_ED_Scan_Confirm_Tag

 This structure is called when the device wants to report about the
 results of a Route Discovery operation.

 Status
 - The status of the request.

 energy_Detect_list[16]
 -The list of energy measurements , one for each channel.
 *******************************************************************************/
typedef struct NLME_ED_Scan_Confirm_Tag {
    uint8_t status;
#ifndef APPLY_R_18_CHANGE
    uint8_t unscanned_channels[4];
#endif
    uint8_t energy_Detect_list[16];
}__attribute__((__packed__)) NLME_ED_Scan_Confirm_t;

/******************************************************************************/
typedef struct NWK_Info_Base_Tag {
    uint8_t NWK_sequence_number;
    uint8_t NWK_passive_ack_timeout; /* in milli seconds*/
    uint8_t NWK_max_broadcast_retries;
 //   uint8_t NWK_max_children;
    uint8_t NWK_max_depth;
  //  uint8_t NWK_max_router;
    uint8_t NWK_network_broadcast_delivery_time;
    uint8_t NWK_report_constant_cost;
    uint8_t NWK_route_discovery_retries_permitted;
    uint8_t NWK_sym_link;
    uint8_t NWK_capability_information;
    uint8_t NWK_use_tree_addr_alloc;
    uint8_t NWK_use_tree_routing;
    uint16_t NWK_transaction_persistence_time;
    uint8_t NWK_stack_profile;
    uint8_t NWK_current_neighbor_table_index;
    uint8_t NWK_current_route_table_index;
    uint8_t NWK_security_material_descriptor_index;
    uint8_t NWK_logical_channel;
    uint8_t NWK_energy_threshold;
    uint8_t NWK_Is_Concentrator;
    uint16_t NWK_Concentrator_Radius;
    uint16_t NWK_Concentrator_DiscoveryTime;
    uint8_t NWK_unique_address;
#ifdef APPLY_R_20_CHANGE
    uint8_t NWK_leave_request_allowed;
#endif
} __attribute__((__packed__)) NWK_Info_Base;
/******************************************************************************/

/*******************************************************************************
 NLME SAP
 *******************************************************************************/

/*******************************************************************************
 NLME_Request_t

 This structure is used when NHLE wants to send a Management
 primitive to the network Layer.

 primitive
 -The Network Layer management request primitive.

 network_discovery
 -A request to discover Networks issued by the NHLE to the Network
 Layer.

 network_formation
 -A request to Establish  a new Networks issued by the NHLE to the
 Network Layer.

 permit_joining
 -A request to set the association permit issued by the NHLE to the
 Network Layer.

 start_router
 -A request to start the device as a router issued by the NHLE to
 the Network Layer.

 permit_joining
 -A request by the device to Join or Rejoin a Networks issued by the
 NHLE to the Network Layer.
 leave
 -A request by the device to leave or ask other device to leave
 issued by the NHLE to the NetworkLayer.

 route_discovery
 -A request by the device to perform route discovery issued by the
 NHLE to the Network Layer.

 sync
 -A request by the device to perform a poll in a beacon less network
 or to sync with the beacons of the parent in a beacon enabled
 network issued by the NHLE to the Network Layer.

 *******************************************************************************/

typedef struct NLME_Request_Tag {
    uint8_t primitive;
    union NLME_Request_Primitive {
        NLME_Network_Discovery_Request_t network_discovery;
        NLME_Network_Formation_Request_t network_formation;
        NLME_Permit_Join_Request_t permit_joining;
        NLME_Start_Router_Request_t start_router;
        NLME_Join_Request_t join;
        NLME_Leave_Request_t leave;
        NLME_Route_Discovery_Request_t route_discovery;
        NLME_Sync_Request_t sync;
        NLME_ED_Scan_Request_t ED_scan_request;
    } NLME_Request_Primitive;
} __attribute__((__packed__)) NLME_Request_t;

/*******************************************************************************
 NLME_Response_t

 This structure is used when Network Layer wants to send a
 Management confirmation or a Management Indication to the NHLE
 layer.

 primitive
 -The Network Layer management confirmation primitive.

 network_discovery_conf
 -Confirmation for the request performed by network discovery.

 network_formation_conf
 -Confirmation for the request performed by network formation.

 permit_joining_conf
 -Confirmation for the request to allow/disallow device joining.

 start_router_conf
 -Confirmation for the request to start as a router.

 join_conf
 -Confirmation for the request to Join/Rejoin a network.

 leave_conf
 -Confirmation for the request to perform Leave operation.

 route_discovery_conf
 -Confirmation for the request to perform route discovery.

 sync_conf
 -Confirmation for the request to perform sync with its parent.

 join_ind
 -Indication of the association performed.

 leave_ind
 -Indication of the leave performed.

 route_error_ind
 -Indication of the route discovery error.


 *******************************************************************************/

/*******************************************************************************
 NLME_Response_t - NLME Response structure
 *******************************************************************************/
typedef struct NLME_Response_Tag {
    uint8_t primitive;
    union NLME_Response_Primitive {
        NLME_Network_Discovery_Confirm_t network_discovery_conf;
        NLME_Network_Formation_Confirm_t network_formation_conf;
        NLME_Permit_Joining_Confirmation_t permit_joining_conf;
        NLME_Start_Router_Confirm_t start_router_conf;
        NLME_Join_Confirm_t join_conf;
        NLME_Leave_Confirm_t leave_conf;
        NLME_Route_Discovery_Confirm_t route_discovery_conf;
        NLME_Sync_Confirm_t sync_conf;
        NLME_Join_Indication_t join_ind;
        NLME_Leave_Indication_t leave_ind;
        NLME_Network_Status_Indication_t nwk_status_ind;
        NLME_ED_Scan_Confirm_t ed_scan_conf;
    } NLME_Response_Primitive;
} __attribute__((__packed__)) NLME_Response_t;

/*******************************************************************************
 Route_Table_t

 This structure carries all the info required by the routing module,
 when a route is discovered.

 dest_address
 -Stores the short address of the device to which the route is
 discovered or for which discovery is in progress.

 status_info

 -The status of the route are 1st 3 bits
 g_Active_c
 g_Discovery_Underway_c
 g_Discovery_Failed_c
 g_Inactive_c
 g_Validation_Underway_c

 - 4th bit No route cache 1 bit: A flag indicating that the destination
 indicated by this address does not store source routes.
 - 5th bit Many-to-one 1 bit : A flag indicating that the destination is a
 concentrator that issued a many-to-one route request.
 - 6th bit Route record required 1 bit:  A flag indicating that a
 route record command frame should be sent to the destination prior to the
 next data packet.
 - 7th bit GroupID flag 1 bit A flag indicating that the destination
 address is a Group ID.
 ageingIndex
 -The aging index keep track of routing table entries in order to reclaim
 table space from entries that are no longer in use.
 next_hop_address
 -Holds the next hop address to reach the particular destination
 address.

 *******************************************************************************/
typedef struct Route_Table_Tag {
    uint16_t dest_address;
    uint8_t status_info;
    uint8_t ageingIndex;
    uint16_t next_hop_address;
} __attribute__((__packed__)) Route_Table_t;

/*******************************************************************************
 Route_Record_Table_t

 This structure carries all the info required by the concentrator,
 to reach the destination with source routed frame.

 relayCount
 -Number of intermediate devices to reach the dest.

 relayList
 -List of device addresses.

 *******************************************************************************/
typedef struct Route_Record_Table_Tag {
    uint16_t dest_address;
    uint8_t relayIndex;
    uint8_t relayCount;
    uint16_t relayList[m_NWK_MAX_DEPTH_c];
}Route_Record_Table_t,rsi_GetRouteRecordforDev;

/*******************************************************************************
 NeighborTable_t

 The neighbor table of a device shall contain Information on every
 device within transmission range,up to
 g_MAX_NEIGHBOR_TABLE_ENTRIES_c.

 The neighbor table is updated during network discovery and during
 network join to store Information about routers/coordinators within
 RF reception range that can be potential parents.The neighbor table
 is  also updated every time a child or a parent device Leaves the
 Network.

 A table entry shall be updated every time a device receives any NWK
 Command frame or Beacons with payload from the corresponding
 neighbor.

 a_extended_address[8]
 -This 64-bit IEEE address is unique to every device;This field is
 updated only if the neighbor is an immediate child of the current
 device.

 network_address
 -The 16-bit network address of the neighboring device.

 logical_channel
 -This is a one nyte logicla channel.

 nwk_pan_id
 -The 16-bit PAN ID of a network.

 device_info

 Bit

 0 - 1  -    devicetype

 2      -    rxOnWhenidle

 3-5    -    relationship

 6      -    address_in_use.

 The bit 6  will be set to 0 if device leaves the network with reuse
 bit set to one.

 transmit_failure
 -This value indicates if previous transmissions to the device were
 successful or not; Higher values indicate more failures.

 LQI
 -The estimated link quality for RF transmissions from this device.
 This field must be present in every neighbor table entry.

 depth_permit_join
 -This value indicates permit joining capability of device and depth
 of device.

 Bit 0    -  indicates permit join capability of device.If the bit
 is set to
 0  -   indicates device does not permit join
 1  -   indicates device allows joining.

 Bit 1 to 6 - gives the depth of device.

 Bit 7    -   indicate whether the child is authenticated.
 1  -   indicated device not authenticated
 0  -   indicates device authenticated

 incoming_frame_counter[2]
 -Set of incoming frame counter values and corresponding device
 addresses.

 *******************************************************************************/

typedef struct NeighborTable_Tag {
    uint8_t a_extended_address[8];
    uint16_t network_address;
    uint16_t nwk_pan_id;
    uint8_t logical_channel;
    uint8_t device_info;
    uint8_t transmit_failure;
    uint8_t LQI;
    uint8_t depth_permit_join;
    uint32_t incoming_frame_counter[2];
    uint8_t incoming_cost;
    uint8_t outgoing_cost;
    uint8_t ageing;
} __attribute__((__packed__)) NeighborTable_t;

/*******************************************************************************
 Persistent_NIB_t

 This structure contains network Information Base parameters which
 need to be stored in persistence storage.

 nwk_security_level
 -The security level for outgoing and incoming NWK frames.

 nwk_short_address
 -The 16-bit address that the device uses to communicate with the
 PAN.

 nwk_active_key_seq_number
 -The sequence number of the active Network key in
 nwkSecurityMaterialSet.

 nwk_all_fresh
 -Indicates whether incoming NWK frames must be all checked for
 freshness when the memory for incoming frame counts is exceeded.

 nwk_secure_all_frames
 -This indicates if security shall be applied to incoming and
 outgoing NWK data frames; if set to

 0x01   -  security processing shall be applied to all incoming and
 outgoing frames except data frames destined for the
 current device that have the security sub-field of the
 frame control field set to 0;

 0x01   -  the NWK layer shall not relay frames that have the
 security sub-field of the frame control field set to 0;

 The SecurityEnable parameter of the NLDE-DATA.request primitive shall
 override the setting of this attribute.

 NWK_pan_id
 -This is a custom element added to NIB.This indicates pan id of
 network in which device has joined.

 nwk_extended_panid[8]
 -This is a custom element added to NIB.This indicates pan id of
 network in which device has joined.

 NWK_number_of_active_children_joined
 -This is a custom element added to NIB.This indicates how many non
 sleeping devices joined to device.

 NWK_number_of_routers_joined
 -This is a custom element added to NIB.This indicates how many
 routers joined to device.

 NWK_number_of_end_devices_joined
 -This is a custom element added to NIB.This indicates how many end
 devices joined to device.

 NWK_passive_ack_neighbor_table_index
 -This is a custom element added to NIB.This parameter maintains the
 indices to the neighbor table entries holding info of joined routes
 and coordinator.Every time a new device (ZR) joins or leaves
 PASSIVE_ACK_NBT_INDEX shall be updated accordingly by setting or
 resetting the nth bit, as it is in the nth entry in the Neighbor
 Table.

 *******************************************************************************/
typedef struct Persistent_NIB_Tag {
    uint8_t NWK_security_level;
    uint16_t NWK_short_address;
    uint8_t NWK_active_key_seq_number;
    uint8_t NWK_all_fresh;
    uint8_t NWK_secure_all_frames;
    /* pan id is proprietary */
    uint16_t NWK_pan_id;
    uint8_t NWK_extended_panid[8];

    /* The following members are proprietary */
    uint8_t NWK_current_device_type;
    uint8_t NWK_number_of_active_children_joined;
    uint8_t NWK_number_of_routers_joined;
    uint8_t NWK_number_of_end_devices_joined;
    uint8_t NWK_current_device_depth;
    uint32_t NWK_passive_ack_neighbor_table_index;
    /*This attribute is required for ZigBee 2007*/
    uint8_t NWK_update_id;
    /*Network Manager Address is required for Zigbee 2007*/
    uint16_t NWK_manager_address;
    /*Device's parent address is stored here*/
    uint16_t NWK_parent_address;
    /*Nwk_Tx_Total is required for Zigbee 2007 Frequency Agility feature*/
    uint16_t NWK_tx_total;
    uint16_t NWK_tx_failure_count;
} __attribute__((__packed__)) Persistent_NIB_t;

/*******************************************************************************
 Security_Material_Descriptor_t

 This structure  is used to securely transmit outgoing frames and
 securely receive incoming frames.

 Upper layers control the security processing operations, by setting
 up the appropriate keys and frame counters and establishing which
 security level to use.

 keysequencenumber
 -A sequence number assigned to a Network key by the trust center
 and used to distinguish Network keys for purposes of key updates,
 and incoming frame security operation.

 outgoing_frame_counter
 -A sequence number assigned to a Network key by the trust center
 and used to distinguish Network keys for purposes of key updates,
 and incoming frame security operation.

 key[16]
 -The actual value of the key to be used for Encryption and
 Decryption.

 *******************************************************************************/
//#pragma pack(1)
typedef struct Security_Material_Descriptor_Tag {
    uint8_t keysequencenumber;
    uint32_t outgoing_frame_counter;
    uint8_t key[16];
} __attribute__((__packed__)) Security_Material_Descriptor_t;
//#pragma pack()

/******************************************************************************
 *ZigBeeNeighborTableEntry_t
 shortId
 The neighbor's two byte short address.
 averageLqi
 An exponentially weighted moving average of the link quality values of
 incoming packets from this neighbor as reported by the PHY.
 incomingCost
 The incoming cost for this neighbor, computed from the average LQI.
 Values range from 1 for a good link to 7 for a bad link.
 outgoingCost
 The outgoing cost for this neighbor, obtained from the most recently
 received neighbor exchange message from the neighbor. A value of zero
 means that a neighbor exchange message from the neighbor has not been
 received recently enough, or that our id was not present in the most
 recently received one.
 age
 he number of aging periods elapsed since a neighbor exchange message was
 last received from this neighbor. In stack profile 1, the number of
 aging periods since any packet was received. An entry with an age
 greater than 3 is considered stale and may be reclaimed. The aging
 period is 16 seconds.
 aIEEEAddress
 The 8 byte IEEE address of the neighbor.
 ******************************************************************************/
typedef struct ZigBeeNeighborTableEntry_Tag {
    uint16_t shortId;
    uint8_t averageLqi;
    uint8_t incomingCost;
    uint8_t outgoingCost;
    uint8_t age;
    uint8_t aIEEEAddress[8];
}__attribute__((__packed__)) ZigBeeNeighborTableEntry_t;

/******************************************************************************
 * ZigBeeRoutingTableEntry_t
 destAddr
 The short id of the destination.
 nextHop
 The short address of the next hop to this destination.
 status
 Indicates whether this entry is active (0), being discovered (1),
 or unused (3).
 age
 The number of seconds since this route entry was last used to send a
 packet.
 concentratorType
 Indicates whether this destination is a High RAM Concentrator (2), a Low RAM
 Concentrator (1), or not a concentrator (0).
 routeRecordState
 For a High RAM Concentrator, indicates whether a route record is
 needed (2), has been sent (1), or is no long needed (0) because a source
 routed message from the concentrator has been received.
 ******************************************************************************/
typedef struct ZigBeeRoutingTableEntry_Tag {
    uint16_t destAddr;
    uint16_t nextHop;
    uint8_t status;
    uint8_t age;
    uint8_t concentratorType;
    uint8_t routeRecordState;
}__attribute__((__packed__)) ZigBeeRoutingTableEntry_t;

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/

/* Holds the neighbor table contents */
extern NeighborTable_t *gp_Neighbor_Table;

#if (g_NWK_ROUTE_RECORD_TABLE_d == 1)
/* Holds the Route Record table contents */
extern Route_Record_Table_t *gp_route_record_table;
#endif //g_NWK_ROUTE_RECORD_TABLE_d

/* Holds the route table contents */
extern Route_Table_t *gp_Route_Table;

/* Holds the persistent nib contents */
extern Persistent_NIB_t *gp_Persistent_NIB;

/* memory for network information base attributes that does not need to be
 stored in persistent storage */
extern NWK_Info_Base g_NWK_Info_Base;

#if ( g_ZIGBEE_ROUTER_d == 1 )
extern uint8_t g_trust_center_swapout_pan_id_c[2];
#endif



extern uint8_t Source_Route_Supported ;

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

extern void NWK_handleLeaveRequest(uint8_t leaveBufferIndex, uint8_t* pMessage);

/*******************************************************************************
 *
 * Input Parameters:
 *           NONE.
 *
 * Output Parameters:
 *           NONE.
 *
 * Return Value :
 *           NONE.
 *
 * Scope of the function:
 *           This function should be called by the upperlayer before sending any
 *           message to the Network Layer.
 *
 * Detailed Description:
 *           This function Initializes all the  Network Layer parameters and
 *           also initializes the Queues used for exchanging messages between
 *           the Upperlayer NHLE and the Network layer.
 *
 * Before the function is called:
 *           Hardware should be initialized.
 *
 * After the function is called:
 *           Messages sent from the Upperlayer can be processed by the Network
 *           layer.
 *
 * Error Handling:
 *           NONE.
 *
 *******************************************************************************/
void NWK_Init(uint8_t warm_start);

/*******************************************************************************
 *
 * Input Parameters:
 *           NONE.
 *
 * Output Parameters:
 *           NONE.
 *
 * Return Value :
 *           NONE.
 *
 * Scope of the function:
 *           This function is global and will be called by the main function of
 *           the NHLE continuously.
 *
 * Detailed Description:
 *           This function enables the Network Layer to receive data from its
 *           NHLEs and NLLE.
 *
 * Before the function is called:
 *           NWK_Init should be called by the NHLE.
 *
 * After the function is called:
 *           NONE.
 *
 * Error Handling:
 *           NONE.
 *
 *******************************************************************************/
void NWK_Main(void);

/*******************************************************************************
 *
 * Input Parameters:
 *
 *     message_index
 *           -index of the buffer holding the data to be sent from NHLE to NWK
 *           layer.
 *
 * Output Parameters:
 *           NONE.
 *
 * Return Value :
 *           SUCCESS(0x00) or BUFFER_OVERFLOW(0xFF)
 *
 * Scope of the function:
 *           This function is called by the NHLE when it wants to  pass NLME
 *           Message to the underlying Network layer.
 *
 * Detailed Description:
 *           This function enables the NHLE to send NLME request to the NWK
 *           Layer.This function returns SUCCESS(0x00) if the message is
 *           successfully posted to the NWK layer else returns
 *           BUFFER_OVERFLOW(0xFF) indicating that the network layer is
 *           receiving more than the specified number of NLME messages
 *           simultaneously into its Queue.
 *
 * Before the function is called:
 *           The following steps should be followed to allocate a memory for the
 *           message sent using the buffer_index as the reference to the
 *           function.
 *
 *              Send_ZDO_NWK_Message():-
 *                 Call the function allocate_buffer() to allocate a buffer and
 *                 get the index.Eg: buffer_index = allocate_buffer().
 *
 *                 Get the starting address of the buffer_index returned by the
 *                 function allocate_buffer() by calling the function
 *                 buffMgmt_getBufferPointer().Eg: myPointerToTheAllocatedBuffer =
 buffMgmt_getBufferPointer(buffer_index)
 *
 *     NOTE:-myPointerToTheAllocatedBuffer is a pointer of type unsigned
 *           character  pointer.Using myPointerToTheAllocatedBuffer  create NLME.
 *           Request message to be sent to the Network layer.Put the message
 *           pointed to by myPointerToTheAllocatedBuffer into the ZDO_NLME
 *           message queue by calling the function NWK_NLMErequest().
 *
 *
 * After the function is called:
 *           Upperlayer(NHLE) should relinquish its control so that other layers
 *           get a chance to process the request.NHLE should also wait for a
 *           confirmation for the issued request.
 *
 * Error Handling:
 *           If the NWK_NLDErequest returns 0xff(BUFFER_OVERFLOW) then the NHLE
 *           should free the message allocated for Issuing the corresponding
 *           management primitive.
 *
 *******************************************************************************/
uint8_t NWK_NLMErequest(uint8_t message_index);

/*******************************************************************************
 *
 * Input Parameters:
 *     message_index
 *           -index of the buffer holding the NLME message sent from NWK layer
 *           to NHLE.
 *
 * Output Parameters:
 *           NONE.
 *
 * Return Value :
 *           NONE.
 *
 * Scope of the function:
 *           This function is global and is a callback function to the NHLE from
 *           the Network Layer.
 *
 * Detailed Description:
 *           This function gets the message_index of the NLME confirmation and
 *           indication message sent by the Network layer to the NHLE .
 *
 * Before the function is called:
 *           NONE.
 *
 * After the function is called:
 *           if Handle_NLME_Message() gets a message_index other than 0xFF
 *           (BUFFER_EMPTY).The NHLE has to perform the below steps to retrive
 *           the message sent by the Network layer:-
 *
 *              Call buffMgmt_getBufferPointer() to get the pointer to the message
 *              refered to by the Message_Index passed as a parameter to
 *              Handle_NLME_Message().
 *
 *              process the Message received .
 *
 *              After the data in the message is consumed by the NHLE.NHLE
 *              should free the buffer by calling the function buffMgmt_freeBuffer
 *              (Message_Index).
 *
 * Error Handling:
 *           NONE.
 *******************************************************************************/
void Handle_NLME_Message(uint8_t message_index);

/*******************************************************************************
 *
 *
 * The input parameters of this function are as follows:-
 * Input Parameters:
 *           NONE.
 *
 * Output Parameters:
 *           NONE.
 *
 * Return Value :
 *           status of reset
 *
 * Scope of the function:
 *           This is a global function and is called by the NHLE when it wants
 *           to perform software reset on the Network layer.
 *
 * Detailed Description:
 *           NWK_Reset() is a synchronous function call and NHLE calls this
 *           function to reset the network layer to its initial condition.Upon
 *           issuing this command the network layer performs a MAC layer reset
 *           and the NWK layer resets itself by clearing all internal variables,
 *           routing table and route discovery table entries and by setting all
 *           NIB Attributes to their default values.
 *
 * Before the function is called:
 *           Before calling this function all buffer allocated previously should
 *           be deallocated as this function in turn resets buffers also.If the
 *           device is already a part of the network then NLME-Leave.Request
 *           should be issued to leave the network and then NWK_Reset() should
 *           be called.
 *
 * After the function is called:
 *           NONE.
 *
 * Error Handling:
 *           NONE.
 *******************************************************************************/
uint8_t NWK_Reset(uint8_t warm_start);

/*******************************************************************************
 *
 * The input parameters of this function are as follows:-
 * Input Parameters:
 *           dest_address->next hop address in the default routing table entry.
 *
 * Output Parameters:
 *           NONE.
 *
 * Return Value :
 *           NONE.
 *
 * Scope of the function:
 *           This is a global function and is called by the NHLE when it wants
 *           to create a default routing table entry.
 *
 * Before the function is called:
 *           Before calling this function device should be in joined state.
 *
 * After the function is called:
 *           NONE.
 *
 * Error Handling:
 *           NONE.
 *******************************************************************************/
#if(g_NWK_MANY_TO_ONE_ROUTING_d == 1 )
void NWK_createDefaultRoutingTableEntry( uint8_t dest_address );
#endif
/*******************************************************************************
 * The input parameters of this function are as follows:-
 * Input Parameters:
 *           None.
 * Output Parameters:
 *           NONE.
 *
 * Return Value :
 *           NONE.
 *
 * Scope of the function:
 *           This Function is used for restoring the NWK to back to active State
 * Before the function is called:
 *           NONE
 * After the function is called:
 *           NONE.
 * Error Handling:
 *           NONE.
 *******************************************************************************/
void NWK_changeToReadyState(void);

#if(g_NWK_LINK_STATUS_d == 1 )
void NWK_sendRemaningLinkStatusCommandFrame(void);
#endif

void getSetUpdateId(bool opType, uint8_t* updateId);
bool setNeibhourTable(NeighborTable_t pNeighborTable);

/******************************************************************************
 * ZigBeeSendManyToOneRouteRequest
 ******************************************************************************
 *
 * Return Value :
 *   returns the status of the function call SUCCESS or error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *   ConcentratorType - Must be either ZIGBEE_HIGH_RAM_CONCENTRATOR or
 *   ZigBeeLowRAMConcentrator. The former is used when the caller has enough
 *   memory to store source routes for the whole network. In that case, remote
 *   nodes stop sending route records once the concentrator has successfully
 *   received one. The latter is used when the concentrator has insufficient
 *   RAM to store all outbound source routes. In that case, route records are sent
 *   to the concentrator prior to every inbound APS unicast.
 *
 *   Radius - The maximum number of hops the route request will be relayed.
 *   A radius of zero is converted to ZIGBEE_MAX_HOPS
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   global function.
 *
 * Purpose of the function:
 *   This function allows the Application to initiate a Many to One Route
 *   Request
 *
 * Before the function is called:
 *   The device should be part of the network to initiate data transmission
 *   successfully.
 *
 * After the function is called:
 *   None
 ******************************************************************************/
#if (g_NWK_CREATE_MTORR_FRAME_d == 1)
uint8_t ZigBeeSendManyToOneRouteRequest(ZigBeeConcentratorType ConcentratorType,
        uint8_t Radius);
#endif //#if (g_NWK_CREATE_MTORR_FRAME_d == 1)
/******************************************************************************
 * AppIncomingManyToOneRouteRequestHandler
 ******************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   SourceAddr - The short id of the concentrator that initiated the
 *                many-to-one route request
 *   pSrcIEEEAddr - The 64-bit address of the Concentrator
 *   PathCost - The path cost to the concentrator
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   global function.
 *
 * Purpose of the function:
 *   This function allows the Application to handle Many to One Route
 *   Request
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void AppIncomingManyToOneRouteRequestHandler(uint16_t SourceAddr,
        uint8_t * pSrcIEEEAddr, uint8_t PathCost);

/******************************************************************************
 * GetNetworkKey
 ******************************************************************************
 *
 * Return Value :
 *   returns g_FAILURE_c / g_No_Key_c / g_SUCCESS_c
 *
 * Parameters   :
 *
 * Input Parameters:
 *    None
 *
 * Output Parameters:
 *   ptrNetworkKey :
 *       network key which is used currently
 *
 * Scope of the function:
 *   global function.
 *
 * Purpose of the function:
 *
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/

uint8_t GetNetworkKey(uint8_t * ptrNetworkKey);


/******************************************************************************
 * SetNetworkKey
 ******************************************************************************
 *
 * Return Value :
 *   returns g_FAILURE_c / g_No_Key_c / g_SUCCESS_c
 *
 * Parameters   :
 *
 * Input Parameters:
 *    None
 *
 * Output Parameters:
 *   ptrNetworkKey :
 *       network key which is used currently
 *
 * Scope of the function:
 *   global function.
 *
 * Purpose of the function:
 * This function sets the network key in security material descriptor
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t SetNetworkKey(uint8_t * ptrNetworkKey);

/******************************************************************************
 * ZigBeeGetRouteTableEntry
 ******************************************************************************
 * Return Value : returns SUCCESS or FAILURE
 *
 * Parameters   :
 *
 * Input Parameters:
 *   index indicates index from where the routing table entry is to be retrieved.
 *
 * Output Parameters:
 *   pRouteTableEntry pointer location to which the routing table entry need
 *   to be copied
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *   This function allows the Application to read the Routing table entry in
 *   the specified index.

 *
 * Before the function is called:
 *   Note : The index should be less than the total number of routing table
 *   active entries. This value is got by calling the function
 *   ZigBeeGetRoutingTableEntryCount
 *
 * After the function is called:
 *   None
 ******************************************************************************/

uint8_t ZigBeeGetRouteTableEntry(uint8_t index,
        ZigBeeRoutingTableEntry_t * pRouteTableEntry);

/******************************************************************************
 * ZigBeeGetExtendedPanId
 ******************************************************************************
 * Return Value : None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   pExtendedPANId pointer to the location where the 64-bit extended PAN id
 *   need to be copied
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *   This function allows the Application to get the 64-bit Extended PAN id of the
 *   operating Network
 *
 * Before the function is called:
 *   The input pointer should be statically allocated to store the IEEE address
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void ZigBeeGetExtendedPanId(uint8_t * pExtendedPANId);

/******************************************************************************
 * ZigBeeGetNeighborTableEntryCount
 ******************************************************************************
 * Return Value : returns the total count of active neighbor table entries in
 *               the neighbor table
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *   This function allows the Application to know the count of active neighbor
 *   table entries.
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t ZigBeeGetNeighborTableEntryCount(void);

/******************************************************************************
 * ZigBeeReadNeighborTableEntry
 ******************************************************************************
 * Return Value : returns SUCCESS or FAILURE
 *
 * Parameters   :
 *
 * Input Parameters:
 *   index indicates index from where the neighbortable entry is to be retrieved
 *
 * Output Parameters:
 *   pNeighborTableEntry pointer location to which the NeighbortableEntry
 *   need to be copied
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *   This function allows the Application to read the Neighbor table entry in the
 *   specified index.
 *
 * Before the function is called:
 *   The index should be less than the total number of neighbor table active
 *   entries. This value is got by calling the function
 *   ZigBeeGetNeighborTableEntryCount
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t ZigBeeReadNeighborTableEntry(uint8_t index,
        ZigBeeNeighborTableEntry_t * pNeighborTableEntry);

/******************************************************************************
 * ZigBeeGetRoutingTableEntryCount
 ******************************************************************************
 * Return Value : returns the total count of active routing table entries in
 *   the routing table
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *   This function allows the Application to know the count of active routing
 *   table entries
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t ZigBeeGetRoutingTableEntryCount(void);

/*******************************************************************************
 * ZigBeeSetRadioPower
 ********************************************************************************
 *
 * Return Value :
 *  returns the SUCCESS or failure
 *
 * Parameters   :
 *
 * Input Parameters:
 *       TxPower inidcates the Transmit power to be set
 *
 * Output Parameters:
 *       None
 *
 *
 * Scope of the function:
 *       This function can be called from application to set Transmit power
 *
 * Purpose of the function:
 *       This function allows the Application to set the radio power for the
 *       device
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeSetRadioPower(int8_t TxPower);

/*******************************************************************************
 * ZigBeeGetRadioPower
 ********************************************************************************
 *
 * Return Value :
 *   returns the radio Transmit power
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   None
 *
 *
 * Scope of the function:
 *       This function can be called from application to get Transmit power
 *
 * Purpose of the function:
 *       This function allows the Application to gets the radio transmit power
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeGetRadioPower(void);

/*******************************************************************************
 * ZigBeeGetLastHopLqi
 ********************************************************************************
 *
 * Return Value :
 *   This function always returns SUCCESS. It is not necessary to check this
 *   return value.
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   lastHopLqi  The link quality for the last incoming message processed.
 *
 * Scope of the function:
 *
 *
 * Purpose of the function:
 *       This function allows the Application to know the lqi value of the last
 *   processed incoming message
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeGetLastHopLqi(uint8_t * lastHopLqi);

/*******************************************************************************
 * ZigBeeCreateRequest
 ********************************************************************************
 *
 * Return Value :
 *   This function always returns SUCCESS or FAILURE
 *
 * Parameters   :
 *
 * Input Parameters:
 *  uint8_t primitiveId
 *  uint16_t shortAddress : short address of the destination device to which the
 *      the request is sent
 *  bool APSAckRequired : This flag indicates source device expects the acknowledgement
 *      for the reception of the request
 *  uint8_t *Payload : pointer to the array
 *  uint8_t dataLength :Indicates the length of the payload(actual data)
 *
 * Output Parameters:
 *
 *
 * Scope of the function:
 *  This function is called from the application
 *
 * Purpose of the function:
 *   This function creates the ZDP primitive requests
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/

uint8_t ZigBeeCreateRequest(uint8_t primitiveId, uint16_t shortAddress,
        bool APSAckRequired, uint8_t *Payload, uint8_t dataLength);

/*******************************************************************************
 * ZigBeeGetNWKParameters
 ********************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   ZigBee_Network_Descriptor_t *pDescriptor
*      uint8_t a_extended_pan_id[8] :
*      uint16_t pan_id :
*      int8_t radio_tx_power :
*      uint8_t logical_channel :
 *
 * Scope of the function:
 *   This function is called from application.
 *
 * Purpose of the function:
 *       This function allows the Application to get the network details such as
 *  devices extended pan id ,short pan id , radio transmit power ,and current
 *  working channel
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
void ZigBeeGetNWKParameters(ZigBee_Network_Descriptor_t *pDescriptor);

/*******************************************************************************
 * AppNetworkFoundHandler
 ********************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   ZigBeeNetworkDetails: Holds networ beacons information
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is a callback function to the application
 *
 * Purpose of the function:
 *       This function allows the Application to get the other network details
 *  such as devices extended pan id ,short pan id , current working channel
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
void AppNetworkFoundHandler(ZigBeeNetworkDetails networkInformation);


#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
void app_NeighborAddedHandler(void) ;
void app_NeighborRemovedHandler(void);
void app_RelayedUcastHandler(void);
void app_NeighborStaleHandler(void);
void app_RouteDiscInitiatedHandler(void);
void app_NWKFCFailureHandler(void);
void app_NWKDecryptFailuresHandler(void);
void app_JoinIndicationHandler(void);

#endif /*g_DIAGNOSTICS_CLUSTER_ENABLE_d */
#if (g_NWK_ROUTE_RECORD_TABLE_d == 1)
void Set_RouteRecordTableEntry(Route_Record_Table_t *pRecdRouteRecord , uint8_t routeRecordIndex);
#endif //#if (g_NWK_ROUTE_RECORD_TABLE_d == 1)

/*******************************************************************************
 * End Of File
 *******************************************************************************/
#endif                                                  /* NLME_INTERFACE_H */
