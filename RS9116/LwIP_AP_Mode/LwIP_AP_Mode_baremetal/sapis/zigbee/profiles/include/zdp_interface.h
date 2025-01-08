/******************************************************************************
 * zdp_interface.h
 ******************************************************************************
 * Program Description:
 *     This file contains the interface functions and data structures to be used
 *     by the application for accessing ZDP management services.
 *
 ******************************************************************************/
#ifndef ZDP_INTERFACE_H
#define ZDP_INTERFACE_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "nlde_interface.h"
#include "nlme_interface.h"

/******************************************************************************
 * Global Constants
 ******************************************************************************/
/* These two macros is not configurable by the user */
#define g_APSME_MAX_CLUSTERIDS_c               0x05
#define g_MAX_ENDPOINTS_IN_A_GROUP_c           0x04
/*Sizeof structure adds padding.Hence the size of bind request is used as
 a macro*/
#define m_LENGTH_OF_ZDP_BIND_UNBIND_REQUEST_c                   0x15

/* These moved from AP Frame work, now moved to app_configuration_default */
///* g_MAX_NO_OF_ENDPOINTS_c
// - This is the max number of endpoints supported. */
//#ifndef g_MAX_NO_OF_ENDPOINTS_c
//#ifdef  APPLY_R_20_CHANGE
//#define g_MAX_NO_OF_ENDPOINTS_c                             0x0A
//#else
//#define g_MAX_NO_OF_ENDPOINTS_c                             0x04
//#endif
//#endif

/*Defines the type of clusters */
#define g_INCLUSTER_c				0x00
#define g_OUTCLUSTER_c				0x01

/*******************************************************************************
 The below section gives the default values of endpoint descriptor
 *******************************************************************************/

/* Endpoint 2*/
#define g_ENDPOINT_TWO_c                                0x02
/* Endpoint 3*/
#define g_ENDPOINT_THREE_c                              0x03

/* Test Profile Identifier.This value is used in the simple descriptor of
 endpoints */
#ifndef ZCP_15
#define g_TEST_PROFILE_ID_c                             0x7F01
#else
#define g_TEST_PROFILE_ID_c                             0x0103
#endif

#define g_DATA_PROFILE_ID_c                             0xCAFE

#define g_WILDCARD_PROFILE_ID_c                         0xFFFF

#define g_ZDO_PROFILE_ID_c                              0x0000

/*Number if clusters(in/out) supported for endpoints 0x02 & 0x03*/
#define g_NUM_OF_CLUSTERS_c                             0x02

/* These moved from AP Frame work */
/*-----------------------------------------------------------------------------
 The below section gives the default values of node descriptor
 -----------------------------------------------------------------------------*/
/*macro for APS Device version and flags*/
#define g_APS_DEVICE_VERSION_FLAGS_c	0x00

/*2400-2483.5MHz frequency band*/
#define g_FREQ_BAND_c				0x40

#if ( g_ZIGBEE_COORDINATOR_d == 1 )

/*logical type in node descritpor for ZigBee Coordinator*/
#define g_LOGICAL_TYPE_ZC_c			0x18

/*MAC Capability flags for ZigBee Coordinator*/
#define g_MAC_CAP_FLAGS_c		0x8E
#endif								   /*g_ZIGBEE_COORDINATOR_d */

#if ( g_ZIGBEE_ROUTER_d == 1 )

/*logical type in node descritpor for ZigBee Router*/
#define g_LOGICAL_TYPE_ZR_c			0x19

/*MAC Capability flags for ZigBee Router*/
#define g_MAC_CAP_FLAGS_c		0x8E

#endif								   /*g_ZIGBEE_ROUTER_d*/


#if ( g_ZIGBEE_END_DEVICE_d == 1 )

#if ( g_ZIGBEE_ACTIVE_END_DEVICE_d == 1 )
/*MAC Capability flags for ZigBee Active End Device*/
#define g_MAC_CAP_FLAGS_c		0x8C

/*logical type in node descritpor for ZigBee End Device*/
#define g_LOGICAL_TYPE_ZED_c		0x1A

#else
/*MAC Capability flags for ZigBee End Device*/
#define g_MAC_CAP_FLAGS_c		0x84

/*logical type in node descritpor for ZigBee End Device*/
#define g_LOGICAL_TYPE_ZED_c		0x1A

#endif   /*g_ZIGBEE_ACTIVE_END_DEVICE_d*/


#endif 								   /*g_ZIGBEE_END_DEVICE_d*/




/*Manufacturer code*/
#define g_MANUFACTURER_CODE_c		{0x00}

#define g_MAX_BUFFER_SIZE_c			0x69u
/*MaxBuffer Size- ranges from 0x00 - 0x7f*/

/*max transfer size - ranges from 0x0000 - 0x7fff*/
#define g_MAX_INCOMING_TRANSFER_SIZE_c		   {0x00,0x00}
#define g_MAX_OUTGOING_TRANSFER_SIZE_c		   {0x00,0x00}
#define g_DESCRIPTOR_CAPABILITY_FIELD_c        0x03

/*-----------------------------------------------------------------------------
 The below section gives the default values of node power descriptor
 -----------------------------------------------------------------------------*/
#define g_CURRENT_POWER_SOURCE_c 0x01

#define m_AVAILABLE_POWER_SOURCE_c  ( g_MAINS_POWER_c | g_RECHARGEABLE_BATTERY_c \
                                    | g_DISPOSABLE_BATTERY_c )

/*Available power sources is Mains power*/
#define g_MAINS_POWER_c				0x10

/*Available power sources is Rechargeable battery*/
#define g_RECHARGEABLE_BATTERY_c	0x20

/*Available power sources is disposable battery*/
#define g_DISPOSABLE_BATTERY_c		0x40

/*Current power mode receiver synchronized with the receiver on when idle
 parameter of node descriptor*/
#define g_RX_STATUS_BASED_ON_RXON_WHEN_IDLE_c		0x00

/*Receiver on periodically as defined by the node power descriptor*/
#define g_RX_ON_PERIODICALLY_c						0x01

/*Receiver ON when stimulated eg: a button press*/
#define g_RX_ON_WHEN_STIMULATED_c					0x02

/*Current power source level is critical*/
#define g_CURRENT_POWER_SOURCE_LEVEL_CRITICAL		0x00

/*33 % is the power source level*/
#define g_CURRENT_POWER_SOURCE_LEVEL_33_PER			0x40

/*66 % is the power source level*/
#define g_CURRENT_POWER_SOURCE_LEVEL_66_PER			0x80

/*100 % is the power source level*/
#define g_CURRENT_POWER_SOURCE_LEVEL_100_PER		   0xC0

/*Current power mode and available power sources field of node power
 descriptor*/
#define g_CURRENT_PWR_MODE_AVAIL_PWR_SRCS		\
				g_RX_STATUS_BASED_ON_RXON_WHEN_IDLE_c | \
				m_AVAILABLE_POWER_SOURCE_c

/*Current power source and power source level fields of node power
 descriptor*/
#define g_CURRENT_PWR_SRC_AND_PWR_SRC_LEVEL	\
				g_CURRENT_POWER_SOURCE_LEVEL_100_PER | \
				g_CURRENT_POWER_SOURCE_c

/* Returns current channel mask */
#define ZigBeeGetChannelMask() (gp_Active_Startup_Attrib_Set->channel_mask)

/* Set channel mask*/
#define ZigBeeSetChannelMask(value) (value) \
  	((value >= g_MASK_FROM_11_CHANNEL_c) || (value <= g_MASK_FOR_26_CHANNEL_c))? \
	gp_Active_Startup_Attrib_Set->channel_mask = value: \
	gp_Active_Startup_Attrib_Set->channel_mask = g_MASK_FROM_11_CHANNEL_c

/******************************************************************************
 This function allows the Application to send ZDP Active Endpoint request
 ******************************************************************************/

#define ZigBeeActiveEndpointsRequest(shortAddress, APSAckRequired)    \
			 ZigBeeCreateRequest(g_ZDP_Active_Endpoint_Request_c, shortAddress, \
											APSAckRequired, g_NULL_c, 0x02);

/* This function allows the application to send power descriptor request */
#define ZDPSendPowerDescriptorRequest( shortAddress, APSAckRequired) \
			 ZigBeeCreateRequest(g_ZDP_Power_Desc_Request_c, shortAddress, \
											APSAckRequired, g_NULL_c, 0x02);
/* This function allows the application to send node descriptor request */
#define ZDPSendNodeDescriptorRequest( shortAddress, APSAckRequired) \
			 ZigBeeCreateRequest(g_ZDP_Node_Desc_Request_c, shortAddress, \
											APSAckRequired, g_NULL_c, 0x02);

#ifdef ZCP_15
/* This function allows the application to send complex descriptor request */
#define ZDPSendComplexDescriptorRequest( shortAddress, APSAckRequired) \
			 ZigBeeCreateRequest(g_ZDP_Complex_Desc_Request_c, shortAddress, \
											APSAckRequired, g_NULL_c, 0x02);

/* This function allows the application to send user descriptor request */
#define ZDPSendUserDescriptorRequest( shortAddress, APSAckRequired) \
			 ZigBeeCreateRequest(g_ZDP_User_Desc_Request_c, shortAddress, \
											APSAckRequired, g_NULL_c, 0x02);
/* This function allows the application to send user descriptor set */
#define ZDPSendUserDescriptorSet( shortAddress, APSAckRequired) \
			 ZigBeeCreateRequest(g_ZDP_User_Desc_Set_c, shortAddress, \
										    APSAckRequired, g_NULL_c, 0x02);
/* This function allows the application to send discover cache request*/
#define ZDPSendDiscoveryCacheRequest( shortAddress, APSAckRequired) \
			 ZigBeeCreateRequest(g_ZDP_Discovery_Cache_Request_c, shortAddress, \
										    APSAckRequired, g_NULL_c, 0x02);
#endif // #ifdef ZCP_15

/******************************************************************************
 This enumeration Specifies the Message that is exchanged between
 ZDP and APPLICATION.
 ******************************************************************************/
enum ZDP_Request_Ids {
    /*------  Device and Service Discovery Client Service Primitives --------*/
    g_ZDP_NWK_Addr_Request_c = 0x00,
    g_ZDP_IEEE_Addr_Request_c = 0x01,
    g_ZDP_Node_Desc_Request_c = 0x02,
    g_ZDP_Power_Desc_Request_c = 0x03,
    g_ZDP_Simple_Desc_Request_c = 0x04,
    g_ZDP_Active_Endpoint_Request_c = 0x05,
    g_ZDP_Match_Desc_Request_c = 0x06,
#ifdef ZCP_15
    g_ZDP_Complex_Desc_Request_c = 0x10,
    g_ZDP_User_Desc_Request_c = 0x11,
    g_ZDP_Discovery_Cache_Request_c = 0x12,
    g_ZDP_User_Desc_Set_c = 0x14,
#endif // #ifdef ZCP_15
    g_ZDP_Device_Annce_Request_c = 0x13,
    g_ZDP_System_Server_Discovery_Request_c = 0x15,
    g_ZDP_End_Device_Bind_Request_c = 0x20,
    g_ZDP_Bind_Request_c = 0x21,
    g_ZDP_Unbind_Request_c = 0x22,
    /*The following two primitives are added for zigbee 2007*/
    g_ZDP_Extended_Simple_Desc_Request_c = 0x1D,
    g_ZDP_Extended_Active_EP_Request_c = 0x1E,
    /*----------------- Network Mgmt Client Service Primitives---------------*/
    g_ZDP_Mgmt_Permit_Joining_Request_c = 0x36,
    /*The following primitive is added for zigbee 2007*/
    g_ZDP_Mgmt_Nwk_Update_Request_c = 0x38
};

enum ZDP_Response_Ids {
    /*------  Device and Service Discovery Server Service Primitives  --------*/
    g_ZDP_NWK_Addr_Response_c = 0x80,
    g_ZDP_IEEE_Addr_Response_c = 0x81,
    g_ZDP_Node_Desc_Response_c = 0x82,
    g_ZDP_Power_Desc_Response_c = 0x83,
    g_ZDP_Simple_Desc_Response_c = 0x84,
    g_ZDP_Active_Endpoint_Response_c = 0x85,
    g_ZDP_Match_Desc_Response_c = 0x86,
    g_ZDP_End_Device_Bind_Response_c = 0xA0,
    g_ZDP_Bind_Response_c = 0xA1,
    g_ZDP_Unbind_Response_c = 0xA2,
    g_ZDP_System_Server_Discovery_Response_c = 0x95,
    /*----------------- Network Mgmt Server Service Primitives----------------*/
    g_ZDP_Mgmt_Permit_Joining_Response_c = 0xB6,
    g_ZDP_Mgmt_Nwk_Update_Notify_c = 0xB8
};


#ifdef ZCP_15
/* This enumeration specifies the XML Tag Ids that are used for Complex descriptor */
typedef enum {
    g_reserved_xml_tag,
    g_language_char_xml_tag,
    g_manufacturer_name_xml_tag,
    g_model_name_xml_tag,
    g_serial_number_xml_tag,
    g_device_url_xml_tag,
    g_icon_xml_tag,
    g_icon_url_xml_tag,
}Complex_Desc_XML_Tag_Ids;
#endif //#ifdef ZCP_15


typedef union Address_Tag {
    uint16_t short_address;
    uint8_t IEEE_address[8];
} Address;


/* Enums for Join Policy*/
/*
 enum
 {
 g_Association_Join_c,
 g_Rejoin_c
 };
 */
/*****************************************************************************
 ZigBeeJoinStatus
 ******************************************************************************
 ZigBeeNotPartOfNWK -  indicates the device is not part of any network
 g_ZigBeeInTheProcessOfJoiningNWK_c - indicates the device is in the process
                                      of Joining the network
 g_ZigBeeJoinedNWK_c - indicates the device has joined the Network
 g_ZigBeeJoinedNWKNoParent_c - indicates the device has joined the Network,
                               but parent communication  has failed
 g_ZigBeePerformingLeaveFromNWK_c - indicates the device is in the process of
                                    leaving the Network
 *****************************************************************************/
typedef enum {
    g_ZigBeeNotPartOfNWK_c,
    g_ZigBeeInTheProcessOfJoiningNWK_c,
    g_ZigBeeJoinedNWK_c,
    g_ZigBeeJoinedNWKNoParent_c,
    g_ZigBeePerformingLeaveFromNWK_c
} ZigBeeJoinStatus;

/*****************************************************************************
 Type Definitions
 ******************************************************************************/
/* These moved from AP Frame work */
/*******************************************************************************
 Address union holding both short address and extended address.

 short_address
 - two byte address of device.

 IEEE_address[8]
 - 64 bit address of device.
 *******************************************************************************/

typedef union Address_T {
    uint16_t short_address;
    uint8_t IEEE_address[8];
} Address_t;

/*******************************************************************************
 Structure for profile id int data type
 *******************************************************************************/
typedef uint16_t profile_id_t;

/*******************************************************************************
 Structure for cluster id int data type
 *******************************************************************************/
typedef uint16_t cluster_id_t;

/*******************************************************************************
 Structure for device id int data type
 *******************************************************************************/
typedef uint16_t device_id_t;

/*******************************************************************************
 Structure for cluster id for APIs
 *******************************************************************************/
typedef uint16_t ClusterID;
/*******************************************************************************
 Structure for profile ID for APIs
 *******************************************************************************/
typedef uint16_t ProfileID;

/*******************************************************************************
 Structure for Group ID for APIs
 *******************************************************************************/
typedef uint16_t GroupID;

/*******************************************************************************
 node power descriptor specific to a device/node
 Field current_powermode_avail_power_sources - the first 4 bits of LSB gives the
 current sleep/ power saving mode of the node
 The MSB 4 bits gives the power sources available in this node
 Field current_powersource_currentpowersourcelevel - the first 4 bit of LSB gives
 the current power source
 The 4 bits of MSB gives the current power source level

 b3 b2 b1 b0 (bit3 bit2 bit1 bit0)

 0  0  0  0 - Receiver synchronized with the receiver on when idle
 subfield of the node descriptor
 0  0  0  1 - Receiver comes on periodically as defined by the node
 power descriptor
 0  0  1  0 - Receiver comes on when stimulated, e.g. by a user pressing
 a  button
 0  0  1  1
 to
 1  1  1  1 - Reserved
 *******************************************************************************/
typedef struct Node_Power_Descriptor_Tag {
    uint8_t current_powermode_avail_power_sources;
    uint8_t current_powersource_currentpowersourcelevel;
}__attribute__((__packed__)) Node_Power_Descriptor_t;

/*******************************************************************************
 Simple Descriptor_Tag
 Field app_profile_id - gives the Application profile Identifier. Its 16 bit
 value.The endpoints in endpoint description supports application
 profile id.Ex if the endpoint is for HA Profile, the profile id for
 that endpoint is 0x0104.
 Field app_device_id - gives the Application device Identifier. This is 16 bit
 value.This is provided by the Application Profile. The Application
 profile defines the device identifiers supported by the profile.
 Field app_device_version - gives the application device version. This field is
 provided by the Application profile.
 Field incluster_count - gives the count of inclusters supported by the endpoint
 Field p_incluster_list - gives the pointer to incluster List. This pointer points
 to the input cluster ids supported by this endpoint.
 Field outcluster_count - gives the count of output clusters
 Field p_outcluster_list - gives the pointer to outncluster List. This pointer
 points to the output cluster ids supported by this endpoint.
 *******************************************************************************/
typedef struct ZigBeeSimpleDescriptor_Tag {
  uint16_t         profileId;
  uint16_t         deviceId;
  uint8_t          deviceVersion;
  uint8_t          inputClusterCount;
  uint8_t          endpoint;
  uint8_t          outputClusterCount;
  uint16_t         *pInputClusterList;
  uint16_t         *pOutputClusterList;
}ZigBeeSimpleDescriptor_t; //TODO

/******************************************************************************
 Mgmt_NWK_Discovery_Network_List_Table_Tag

 This structure defines the fields for the neighbor table list for the mgmt
 LQI response. The parameters are related to the device whose neighbor info
 is availble in the neighbor table of the current device.

 a_extended_pan_id[8] -  this gives the extended pan id of the operating
 network.

 logical_channel - gives the current logical channel occupied by the network.

 stack_profile_zigbee_version - gives the ZigBee Stack Profile and ZigBee
 Protocol in use in the discovered network.

 beacon_order_superframe_order
 -Beacon order specifies how often the MAC sublayer beacon is to be
 transmitted by a given device in the network.Superframe order
 specifies the length of the active portion of the superframe for
 beacon oriented networks i.e. beacon order < 15.

 permit_joining
 - a value of TRUE indicates that atleast one router in the network
 currently permits joining.

 ******************************************************************************/
typedef struct Mgmt_NWK_Discovery_List_Table_Tag {
    uint8_t a_extended_pan_id[8];
    uint8_t logical_channel;
    uint8_t stack_profile_zigbee_version;
    uint8_t beacon_order_superframe_order;
    uint8_t permit_joining;
}__attribute__((__packed__)) Mgmt_NWK_Discovery_List_Table_t;

/******************************************************************************
 Start_Device_Request_Tag

 join_policy
 -This field indicates the join policy to be used for joining the
 network. The enum values are defined as follows:

 1.g_Association_Join_c

 2.g_Orpahn_Join_c

 3.g_Rejoin_c

 4.g_Silent_Join_c

 Based on this join policy,the devices join the network.

 NOTE: This field is applicable only on ZigBee Pro builds. ZigBee (2007)
 builds will ignore this field. This field is not applicable for
 Coordinator and will be ignored.

 ******************************************************************************/
typedef struct Start_Device_Request_Tag {
    uint8_t join_policy;
}__attribute__((__packed__)) Start_Device_Request_t;

/******************************************************************************
 Start_Device_Response_Tag

 Status

 SUCCESS           - Device Starts or joins the network successfully.

 NOT_PERMITTED     - No potential parent was found in the neighbor
 table.

 START_UP_FAILURE  - Suitable channel could not be found for starting
 the network.

 NO_NETWORKS       - No network available in POS.

 Nwk_Pan_Id        - The 16-bit network PAN ID of the network.

 Nwk_Short_Address - The 16-bit network short address of the device.

 logical_channel   - This field indicates the channel on which device has
 joined or the channel on which the coordinator formed
 the network.

 ******************************************************************************/

/*updates the status of the start device operation*/
typedef struct Start_Device_Response_Tag {
    uint8_t status;
    uint16_t Nwk_Pan_Id;
    uint16_t Nwk_Short_Address;
    uint8_t logical_channel;
} __attribute__((__packed__)) Start_Device_Response_t;

/******************************************************************************
 ZDP_NWK_Addr_Request_Tag

 This structure allows the application to request the ZDP to get the
 network address of a  remote device.

 a_IEEE_addr[8]  - This field indicates the 64-bit IEEE address to be matched
 by remote device.The address must be in Little Endian
 format.

 request_type    - This field is used to specify the request_type. The values
 are as follows:

 0x00 - single device response

 0x01 - Extended  response which includes the address of
 the child device

 0x02 to 0xff - Reserved

 start_index     - This field indicates the following values:
 If request_type is extended response, then this
 field specifies the staringindex of the address in the
 neighbor table, to be listed in the Network AddressResponse.
 If request_type is single device response, then this field
 is not included.

 ******************************************************************************/
typedef struct ZDP_NWK_Addr_Request_Tag {
    uint8_t a_IEEE_addr[8];
    uint8_t request_type;
    uint8_t start_index;
}__attribute__((__packed__)) ZDP_NWK_Addr_Request_t;

/******************************************************************************
 ZDP_IEEE_Addr_Request_Tag

 This structure allows the application to request the ZDP to get the
 64 bit IEEE address of a remote device.

 a_network_addr[2]
 - This field indicates the 16-bit network address to be matched by
 the remote device.The address must be in Little Endian format.

 request_type
 - This field indicates the request_type of the command request:

 0x00          - single device response

 0x01          - Extended response

 0x02 to 0xff  - Reserved

 start_index
 - The values are as follows:

 0x00  - Single device response

 0x01 - For extended device response, the starting index is the
 requested elements of the associated devices list

 ******************************************************************************/
typedef struct ZDP_IEEE_Addr_Request_Tag {
    uint8_t a_network_addr[2];
    uint8_t request_type;
    uint8_t start_index;
}__attribute__((__packed__)) ZDP_IEEE_Addr_Request_t;

/******************************************************************************
 ZDP_Node_Desc_Request_Tag

 This structure allows the application to request the ZDP to request
 the node descriptor of a remote device itself or to an alternative
 device that contains the discovery information of the remote device.

 a_network_addr[2]
 -  This field indicates the network address of remote device.The
 address must be in Little Endian format.

 ******************************************************************************/
typedef struct ZDP_Node_Desc_Request_Tag {
    uint8_t a_network_addr[2];
}__attribute__((__packed__)) ZDP_Node_Desc_Request_t;

#ifdef ZCP_15
/******************************************************************************
 ZDP_Complex_Desc_Request_Tag

 This structure allows the application to request the ZDP to request
 the complex descriptor of a remote device itself or to an alternative
 device that contains the discovery information of the remote device.

 a_network_addr[2]
 -  This field indicates the network address of remote device.The
 address must be in Little Endian format.

 ******************************************************************************/
typedef struct ZDP_Complex_Desc_Request_Tag {
    uint8_t a_network_addr[2];
}__attribute__((__packed__)) ZDP_Complex_Desc_Request_t;

/******************************************************************************
 ZDP_User_Desc_Request_Tag

 This structure allows the application to request the ZDP to request
 the user descriptor of a remote device itself or to an alternative
 device that contains the discovery information of the remote device.

 a_network_addr[2]
 -  This field indicates the network address of remote device.The
 address must be in Little Endian format.

 ******************************************************************************/
typedef struct ZDP_User_Desc_Request_Tag {
    uint8_t a_network_addr[2];
}__attribute__((__packed__)) ZDP_User_Desc_Request_t;

/******************************************************************************
 ZDP_User_Desc_Set_Tag

 This structure allows the application to request the ZDP to request
 the user descriptor set on a remote device itself.

 a_network_addr[2]
 -  This field indicates the network address of remote device.The
 address must be in Little Endian format.
 length
 - This field indicates the number of octets in the user descriptor field.
 a_user_descriptor
 - This field contains the user descriptor i.e to be filled on the Remote Device.
 ******************************************************************************/
typedef struct ZDP_User_Desc_Set_Tag {
    uint8_t a_network_addr[2];
    uint8_t length;
    uint8_t a_user_descriptor[16];
}__attribute__((__packed__)) ZDP_User_Desc_Set_t;

/******************************************************************************
 ZDP_Discovery_Cache_Request_Tag

 This structure allows the application to request the ZDP to request
 the discovery cache command to the remote device with the requesting device's
 network address and ieee address in its payload.

 a_network_addr[2]
 -  This field indicates the network address of the device issuing the command.
 a_IEEE_addr
 - This field contains the IEEE address of the device issuing the command.
 ******************************************************************************/
typedef struct ZDP_Discovery_Cache_Request_Tag {
    uint8_t a_network_addr[2];
    uint8_t a_IEEE_addr[8];
}__attribute__((__packed__)) ZDP_Discovery_Cache_Request_t;

#endif // #ifdef ZCP_15

/******************************************************************************
 ZDP_Power_Desc_Request_Tag

 This structure allows the application to request the  ZDP to
 request the Power descriptor of a remote device itself or to an
 alternative device that contains the discovery  information of the
 remote device.

 a_network_addr[2]
 - This field indicates the network address of remote device.The
 address must be in Little Endian format.

 ******************************************************************************/
typedef struct ZDP_Power_Desc_Request_Tag {
    uint8_t a_network_addr[2];
}__attribute__((__packed__)) ZDP_Power_Desc_Request_t;

/******************************************************************************
 ZDP_Simple_Desc_Request_Tag

 This structure allows the application to request the ZDP to request
 the simple descriptor of a remote device itself or to an
 alternative device that contains    the discovery  information of the
 remote device on the specified endpoint.

 a_network_addr[2]
 - This field indicates the network address of remote device.The
 address  must be in Little Endian format.

 endpoint
 - This field indicates the endpoint of the remote device.

 ******************************************************************************/
typedef struct ZDP_Simple_Desc_Request_Tag {
    uint8_t a_network_addr[2];
    uint8_t endpoint;
}__attribute__((__packed__)) ZDP_Simple_Desc_Request_t;

/******************************************************************************
 ZDP_Active_Endpoint_Request_Tag

 This structure allows the application to request the ZDP to acquire
 the list of active endpoints on a remote device with simple
 descriptors.

 a_network_addr[2]
 - This field indicates the network address of the remote device.
 The address must be in Little Endian format.

 ******************************************************************************/
typedef struct ZDP_Active_Endpoint_Request_Tag {
    uint8_t a_network_addr[2];
}__attribute__((__packed__)) ZDP_Active_Endpoint_Request_t;

/******************************************************************************
 ZDP_Match_Desc_Request_Tag

 This structure allows the applcation to request the ZDP to find
 remote devices supporting a specific simple descriptor matching
 criterion based on incluster list and outcluster list.

 a_network_addr[2]
 - This field indicates the address of the remote device.The address
 must be in Little Endian format.

 a_profileID[2]
 - This field indicates the profile ID to be matched at the
 destination.

 in_cluster_count
 -  This field indicates the number of input clusters provided
 within the InClusterList for matching.

 p_in_cluster_list
 -  This field indicates list of input cluster IDs to be used for
 matching.The InClusterList is the desired list to be matched by
 the remote device.

 out_cluster_count
 -  This field indicates the number of output clusters provided for
 matching within the OutClusterList.

 p_out_cluster_list
 -  This field indicates the list of output clusterIDs to be used
 for matching. The OutClusterListis the desired list to be matched
 by the remote device.

 ******************************************************************************/
typedef struct ZDP_Match_Desc_Request_Tag {
    uint8_t a_network_addr[2];
    uint8_t a_profileID[2];
    uint8_t in_cluster_count;
    uint8_t* p_in_cluster_list;
    uint8_t out_cluster_count;
    uint8_t* p_out_cluster_list;
}__attribute__((__packed__)) ZDP_Match_Desc_Request_t;

/******************************************************************************
 ZDP_Device_Annce_Request_Tag

 This structure allows the application to request the ZDP whenever
 enddevice joins or rejoin a network,informs to the all the zigbee
 devices in a network for its presence.

 a_network_addr[2]
 -  This field indicates the network address of the local device.The
 address must be in Little Endian format.

 a_IEEE_addr[8]
 - This field indicates the 64-bit IEEE address for the local device.
 The address must be in Little Endian format.

 capability
 -  capability of the local device means

 Bit          Capability

 0          Alter PAN Coordinator

 1          Device Type

 2          Power Source

 3          Receiver on when idle

 4-5        Reserved

 6          Security

 7          Allocate short address

 ******************************************************************************/
typedef struct ZDP_Device_Annce_Request_Tag {
    uint8_t a_network_addr[2];
    uint8_t a_IEEE_addr[8];
    uint8_t capability;
}__attribute__((__packed__)) ZDP_Device_Annce_Request_t;

/******************************************************************************
 ZDP_Mgmt_Permit_Joining_Request_Tag

 This structure allows the application to request the ZDP that a
 remote device allow or disallow association.The addressing may be
 unicast or 'broadcast' to  all RxOnWhenIdle devices.

 permit_duration
 - This field indicates the permit join duration. The values are as
 follows:

 0x00        - Remote device does not allow the permit
 association

 0xff        - Remote device always allows the permit
 association

 0x01-0xfe   - Permit association is allowed for the specified
 seconds

 TC_significance
 -This field indicates the TC significance. If this is set to 0x01
 and the remote device is the TC, then the command effects the TC
 authenication policy. If this is set to 0x00, then there is no
 effect on Trust Centre.

 ******************************************************************************/
typedef struct ZDP_Mgmt_Permit_Joining_Request_Tag {
    uint8_t permit_duration;
    /*----------TC means Trust center---------------------------- */
    uint8_t TC_significance;
}__attribute__((__packed__)) ZDP_Mgmt_Permit_Joining_Request_t;

/******************************************************************************
 ZDP_Mgmt_Nwk_Update_ED_Scan_Tag

 scan_count
 - This field represents the number of energy scans to be conducted
 and reported.This field shall be present only if the ScanDuration
 is within the range of 0x00 to 0x05.The valid Range of the field
 is 0x00-0x05.

 ******************************************************************************/
typedef struct ZDP_Mgmt_Nwk_Update_ED_Scan_Tag {
    uint8_t scan_count;
}__attribute__((__packed__)) ZDP_Mgmt_Nwk_Update_ED_Scan_t;

/******************************************************************************
 ZDP_Mgmt_Nwk_Update_Change_Channel_Tag

 nwk_update_id
 - The value of the nwkUpdateId contained in this request. This
 value is set by the Network Channel Manager prior to sending the
 message.

 ******************************************************************************/
typedef struct ZDP_Mgmt_Nwk_Update_Change_Channel_Tag {
    uint8_t nwk_update_id;
}__attribute__((__packed__)) ZDP_Mgmt_Nwk_Update_Change_Channel_t;

/******************************************************************************
 ZDP_Mgmt_Nwk_Update_Change_Nwk_Mger_Addr_Tag

 nwk_update_id
 - The value of the nwkUpdateId contained in this request. This
 value is set by the Network Channel Manager prior to sending the
 message.

 a_nwk_manager_address[2]
 - It indicates the NWK address for the device with the Network
 Manager bit set in its Node Descriptor.

 ******************************************************************************/
typedef struct ZDP_Mgmt_Nwk_Update_Change_Nwk_Mger_Addr_Tag {
    uint8_t nwk_update_id;
    uint8_t a_nwk_manager_address[2];
}__attribute__((__packed__)) ZDP_Mgmt_Nwk_Update_Change_Nwk_Mger_Addr_t;
/******************************************************************************
 ZDP_Mgmt_Nwk_Update_Request_Tag

 This structure allows the application to allow request the ZDP that
 either updating of network configuration parameters or to request
 information from devices on network conditions in the local
 operating environment.The destination addressing on this primitive
 shall be unicast or broadcast to all devices for which
 macRxOnWhenIdle = TRUE.

 a_scan_channels[4]
 - For performing ED_Scan or Change of Channel or Updating
 Nwk_Manager Address.For ED_Scan Scan_Channels will be configured
 by more than one channel,other only single channel will be active.

 scan_duration   -

 0x00 - 0x05    - ED_SCAN

 0xFE           - Change of Channel

 0xFF           - Change of Nwk_Manager_Address

 NOTE: The Mgmt_Nwk_Update_Req length will be varies depends on
 scan_duration parameter.so we used union for different scan
 duration parameters.Those structures are explained above.
 ******************************************************************************/
typedef struct ZDP_Mgmt_Nwk_Update_Request_Tag {
    uint8_t a_scan_channels[4];
    uint8_t scan_duration;

    union ZDP_Mgmt_Nwk_Update_Command {
        ZDP_Mgmt_Nwk_Update_ED_Scan_t ed_scan_req;
        ZDP_Mgmt_Nwk_Update_Change_Channel_t change_channel;
        ZDP_Mgmt_Nwk_Update_Change_Nwk_Mger_Addr_t change_nwk_mger_addr;
    } ZDP_Mgmt_Nwk_Update_Command;

}__attribute__((__packed__)) ZDP_Mgmt_Nwk_Update_Request_t;

/******************************************************************************
 ZDP Response Structures
 ******************************************************************************/
/******************************************************************************
 ZDP_NWK_Addr_Response_Tag

 This structure is used to notify the response to the application
 thorugh ZDP.

 Status

 INV_REQUESTTYPE  - request_type holds one of the reserved values

 DEVICE_NOT_FOUND - Remote device address is not present  in the
 node

 SUCCESS          - Creates the response frame, depending on
 request_type.

 NOTE : Other fields of the structure must be ignored, if the status is
 INV_REQUESTTYPE or DEVICE_NOT_FOUND.


 a_IEEE_addr_remote_device[8]
 -This field indicates the IEEE address of the remote device.The
 address must be in Little Endian format.

 a_NWK_addr_remote_device[2]
 -This field indicates the 16-bit address of the remote device. The
 address must be in Little Endian format.

 assoc_device_count
 -This field indicates the number of associated devices on the
 remote device.

 NOTE : If the request_type of the request is for single device response,
 then this field is not included.

 start_index
 -This field indicates the starting index of the list of associated
 devices for this response.

 NOTE : If the request_type of the request is extended response and there
 are no associate devices on the remote device, then the field
 value is 0.If the request_type of the request is single device
 response, then this field is not included in the frame.

 a_NWK_addr_assoc_device_list[1]
 - This field indicates the list of 16-bit address on the remote
 device. The number of addresses is based on associate device count.
 The address must be in Little Endian format.

 NOTE : If the network address request is for single device response, then
 this field is not included.

 ******************************************************************************/
typedef struct ZDP_NWK_Addr_Response_Tag {
    uint8_t status;
    uint8_t a_IEEE_addr_remote_device[8];
    uint8_t a_NWK_addr_remote_device[2];
    uint8_t assoc_device_count;
    uint8_t start_index;
    uint8_t a_NWK_addr_assoc_device_list[1];
}__attribute__((__packed__)) ZDP_NWK_Addr_Response_t;

/******************************************************************************
 ZDP_IEEE_Addr_Response_Tag

 This structure is used to notify the response to the application
 through ZDP.

 Status

 INV_REQUESTTYPE     -  request_type in the IEEE
 Address Request holds one of the
 reserved values.

 DEVICE_NOT_FOUND    -  Remote device address is not present
 on the remote device
 .
 SUCCESS             -  The response has a valid IEEE address

 a_IEEE_addr_remote_device[8]
 -This field indicates the IEEE address of the remote device. The
 address must be in Little Endian format.

 a_NWK_addr_remote_device[2]
 -This field indicates the 16-bit address of the remote device. The
 address must be in Little Endian format.

 assoc_device_count
 -This field indicates the number of associated devices on the
 remote device.

 NOTE : If the request_type of the request is for single device response,
 then this field is not included in the frame.

 If the request_type of the IEEE address request is extended
 response,  and if there are no associated devices on the remote
 device, then this field is set to 0.
 .
 start_index
 -This field indicates the starting index of the list of associated
 devices for this report.

 NOTE :If the request_type of the request is extended response and if
 there are no associated devices on the remote device, then value of
 this field is zero.

 If the request_type in the IEEE address request is single device
 response, then this field is not included in the frame.

 a_NWK_addr_assoc_device_list[1]
 -This field indicates the list of 16-bit address on the remote
 device. The number of addresses is based on associate_device_count.
 The addresses must be in Little Endian format.

 If the IEEE address is for single device response, then this field
 is not included.

 ******************************************************************************/
typedef struct ZDP_IEEE_Addr_Response_Tag {
    uint8_t status;
    uint8_t a_IEEE_addr_remote_device[8];
    uint8_t a_NWK_addr_remote_device[2];
    uint8_t assoc_device_count;
    uint8_t start_index;
    uint8_t a_NWK_addr_assoc_device_list[1];
}__attribute__((__packed__)) ZDP_IEEE_Addr_Response_t;
/*******************************************************************************
 node descriptor specific to a device/node
 Field logical_type_complex_user_desc_avail - gives logical type of the device in
 the first 3 bits(from LSB), 4th bit says complex descriptor available
 or not 5th bit says user descriptor available or not and rest of the
 3 bits are reserved
 b2 b1 b0 (bit 2 bit 1 bit 0)
 0  0  0 - ZigBee Coordinator
 0  0  1 - ZigBee Router
 0  1  0 - ZigBee Ebd Device
 0  1  1 to
 1  1  1 - Reserved
 b4 (bit 4)
 1  - complex descriptor available
 b5 (bit 5)
 1 - user descriptor available
 bit6,bit7,bit8 are reserved bits
 Field aps_flags_freq_band - gives first 3 bits APS Capability flags(from LSB).
 Currently this is set to 0.
 The upper 5 bits says the frequency band used.
 setting one of the bit of 5 bits gives the required frequency band.

 b4 b3 b2 b1 b0 (bit 4 bit 3 bit 2 bit 1 bit 0)

 0  0  0   0   1 - 868 to 868.6 MHz
 0  0  1   0   0 - 902 to 928 MHz
 0  1  0   0   0 - 2400 to 2483.5 MHz

 Field MAC_capability_flags - This gives the MAC capabilites of a Node
 For ZC - 0x8A
 For ZR - 0x8A
 For ZED - 0x88 if receiver on when idle is true and 0x80 if false
 Field manufacturer_code - gives the code of the manufactures allocated by
 ZigBee Alliance
 Field max_buffer_size - gives the maximum size that can be sent to APS.
 This field is used along with fragmentation
 Field a_incoming_maximum_transfer_size - gives the maximum number of
 bytes that can be received by the device
 Field a_server_mask - gives the system server capabilities of this node
 setting
 bit 0 gives primary trust centre,bit 1 gives back up trust
 center
 bit 6 indicates if the device is network manager
 bit 7 to bit 15 is reserved
 Field a_outgoing_maximum_transfer_size - gives the maximum number of
 bytes that can be transmitted by the device
 Field descriptor_capability_flag - indicates if the extended simple descriptor
 and extended active ep is supported.
 Bit 0 - Extended active endpoint list available
 Bit 1 - Extended simple descriptor list available

 *******************************************************************************/

typedef struct Node_Descriptor_Tag {
    uint8_t logical_type_complex_user_desc_avail;
    uint8_t aps_flags_freq_band;
    uint8_t MAC_capability_flags;
    uint8_t a_manufacturer_code[2];
    uint8_t max_buffer_size;
    uint8_t a_incoming_maximum_transfer_size[2];
    uint8_t a_server_mask[2];
    uint8_t a_outgoing_maximum_transfer_size[2];
    uint8_t descriptor_capability_flag;
}__attribute__((__packed__)) Node_Descriptor_t;

/******************************************************************************
 ZDP_Node_Desc_Response_Tag

 This structure notifies the result of remote device NodeDescriptor
 Request information to the application through ZDP.

 Status

 INVALID_REQUEST_TYPE -  Network address of interest field does not match
 the network address of the remote device and the
 remote device is an end device.

 NO_DESCRIPTOR       -   If the remote device Node Descriptor is not
 available for a child
 indicated by the network address of interest
 field.
 SUCCESS              -  Successfully processes Node Descriptor Request.

 a_NWK_addr[2]
 -This field indicates the network address of remote device.The
 address will be in Little Endian format.     .

 node_descriptor
 -This field contains the node descriptor of the remote device.

 NOTE: This field is not included, if the status is INVALID_REQUEST_TYPE
 or DEVICE_NOT_FOUND. For details of this structure, refer to
 Node Decriptor.

 ******************************************************************************/
typedef struct ZDP_Node_Desc_Response_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
    Node_Descriptor_t node_descriptor;
}__attribute__((__packed__)) ZDP_Node_Desc_Response_t;

#ifdef ZCP_15
/*******************************************************************************
 complex descriptor specific to a device/node

 Field language_char - specifies the language and character set used by the
 character string in the complex descriptor.

 Field manufactures_name - contains the character string representing the name
 of the manufacturer of the device.

 Field model_name - contains the character string representing the name of the
 manufacturers model of the device.

 Field serial_number - contains the character string representing the name of
 the manufacturers serial_number of the device.

 Field device_url - contains the character string representing the URL through
 which more information related to the device can be obtained.

 Field icon - contains an octet string which carries the data for an icon that
 can represent the device on a computer,gateway or PDA.

 Field icon_url - contians the character string representing the URL through
 which the icon for the device can be obtained.

 *******************************************************************************/
//#pragma pack(1)
typedef struct Language_Character_Set_Tag {
  uint8_t xml_tag;
  uint8_t language_code[2];
  uint8_t character_set_identifier;
}__attribute__((__packed__)) Language_Character_Set_t;
//#pragma pack()

typedef struct Complex_Descriptor_Tag {
  uint8_t field_count;
  uint8_t reserved;
  Language_Character_Set_t language_char;
  uint8_t manufacturer_name[16];
  uint8_t model_name[16];
  uint8_t serial_number[16];
  uint8_t device_url[17];
  uint8_t icon[16];
  uint8_t icon_url[17];
}__attribute__((__packed__)) Complex_Descriptor_t;
/******************************************************************************
 ZDP_Complex_Desc_Response_Tag

 This structure notifies the result of remote device ComplexDescriptor
 Request information to the application through ZDP.

 Status

 INVALID_REQUEST_TYPE -  Network address of interest field does not match
 the network address of the remote device and the
 remote device is an end device.

 NO_DESCRIPTOR       -   If the remote device Complex Descriptor is not
 available for a child
 indicated by the network address of interest
 field.
 SUCCESS              -  Successfully processes Complex Descriptor Request.

 a_NWK_addr[2]
 -This field indicates the network address of remote device.The
 address will be in Little Endian format.

 length
 - This field contains the length of the Complex Descriptor.

 complex_descriptor
 -This field contains the complex descriptor of the remote device.

 NOTE: This field is not included, if the status is INVALID_REQUEST_TYPE
 or DEVICE_NOT_FOUND. For details of this structure, refer to
 Node Decriptor.

 ******************************************************************************/
typedef struct ZDP_Complex_Desc_Response_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
    uint8_t length;
    Complex_Descriptor_t complex_descriptor;
}__attribute__((__packed__)) ZDP_Complex_Desc_Response_t;

/*******************************************************************************
 user descriptor specific to a device/node

 Field user_description - contains information that allows the user to identify
 device using a user-friendly character string such as "Bedroom TV" or
 "Stairs light".

 NOTE:This descriptor contains a single field,which uses ASCII character set, and
 shall contain a maximum of 16 characters.

 *******************************************************************************/

typedef struct User_Descriptor_Tag {
    uint8_t user_description[16];
}__attribute__((__packed__)) User_Descriptor_t;
/******************************************************************************
 ZDP_User_Desc_Response_Tag

 This structure notifies the result of remote device User
 Descriptor Request information to the application through ZDP.

 Status

 INVALID_EP          - Either an invalid end point id is sent or End
 point is not active.

 INVALID_REQUEST_TYPE  - Network address of interest field does not
 match the network address of the remote device
 and the remote device is an end device.


 DEVICE_NOT_FOUND    - Network address of interest field does not
 match the network address of the remote device.
 The device is not available in the discovery.


 NOT_ACTIVE          - Endpoint is not active on the remote device.

 SUCCESS             - Successfully processes User DescriptorRequest

 a_NWK_addr[2]
 - This field indicates the network address of the remote device.
 The address must be in Little Endian format.

 length
 -This field indicates the length of the User Descriptor.

 NOTE : This filed takes the value zero, If the status is other than
 success.

 a_user_descriptor[1]
 - This field contains the user descriptor.The format of the
 descriptor needs to be in the same format as received by the node
 over the air.

 NOTE :This field is not present, if  the status is other than Success.

 ******************************************************************************/
typedef struct ZDP_User_Desc_Response_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
    uint8_t length;
    User_Descriptor_t a_user_descriptor[1];
}__attribute__((__packed__)) ZDP_User_Desc_Response_t;

/******************************************************************************
 ZDP_User_Desc_Conf_Tag

 This structure notifies the result of remote device User
 Descriptor Conf information to the application through ZDP.

 Status

 INVALID_EP          - Either an invalid end point id is sent or End
 point is not active.

 INVALID_REQUEST_TYPE  - Network address of interest field does not
 match the network address of the remote device
 and the remote device is an end device.


 DEVICE_NOT_FOUND    - Network address of interest field does not
 match the network address of the remote device.
 The device is not available in the discovery.


 NOT_ACTIVE          - Endpoint is not active on the remote device.

 SUCCESS             - Successfully processes User Descriptor Set

 a_NWK_addr[2]
 - This field indicates the network address of the remote device from which the
   device has received the user descriptor set.
  The address must be in Little Endian format.

 ******************************************************************************/
typedef struct ZDP_User_Desc_Conf_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
}__attribute__((__packed__)) ZDP_User_Desc_Conf_t;

#endif //#ifdef ZCP_15

/******************************************************************************
 ZDP_Power_Desc_Response_Tag

 This structure notifies the result of remote device PowerDescriptor
 Request information to the application through ZDP.

 Status

 INVALID_REQUEST_TYPE - Network address of interest field does not match
 the network address of the remote device and the
 remote device is an end device.

 DEVICE_NOT_FOUND     - Network address of interest field does not match
 the network address of the remote device.

 NO_DESCRIPTOR        - Node Descriptor is not available for a child
 indicated by the network address.

 SUCCESS              - Successfully processes the Power Descriptor
 Request.

 a_NWK_addr[2]
 -This field indicates the network address of remote device. The
 address must be in Little Endian format.

 node_power_descriptor
 - This field contains the power descriptor of the remote device.

 NOTE: This field is not included, if the status is INVALID_REQUEST_TYPE
 or DEVICE_NOT_FOUND. For details refer to Power Descriptor.

 ******************************************************************************/
typedef struct ZDP_Power_Desc_Response_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
    Node_Power_Descriptor_t node_power_descriptor;
}__attribute__((__packed__)) ZDP_Power_Desc_Response_t;

/******************************************************************************
 ZDP_Simple_Desc_Response_Tag

 This structure notifies the result of remote device Simple
 Descriptor Request information to the application through ZDP.

 Status

 INVALID_EP          - Either an invalid end point id is sent or End
 point is not active.

 INVALID_REQUEST_TYPE  - Network address of interest field does not
 match the network address of the remote device
 and the remote device is an end device.


 DEVICE_NOT_FOUND    - Network address of interest field does not
 match the network address of the remote device.
 The device is not available in the discovery.


 NOT_ACTIVE          - Endpoint is not active on the remote device.

 SUCCESS             - Successfully processes Simple DescriptorRequest

 a_NWK_addr[2]
 - This field indicates the network address of the remote device.
 The address must be in Little Endian format.

 length
 -This field indicates the length of the Simple Descriptor.

 NOTE : This filed takes the value zero, If the status is other than
 success.

 a_simple_descriptor[1]
 - This field contains the simple descriptor.The format of the
 descriptor needs to be in the same format as received by the node
 over the air.

 NOTE :This field is not present, if  the status is other than Success.

 ******************************************************************************/
typedef struct ZDP_Simple_Desc_Response_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
    uint8_t length;
    uint8_t a_simple_descriptor[1];
}__attribute__((__packed__)) ZDP_Simple_Desc_Response_t;

/******************************************************************************
 ZDP_Active_Endpoint_Response_Tag

 This structure notifies the result of remote device Active Endpoint
 request information to the application through ZDP.

 Status

 INVALID_REQUEST_TYPE    -  Network address of interest field does not
 match the network address of the remote
 device and the remote device is an end device

 DEVICE_NOT_FOUND        -  Network address of interest field does not
 match the network address of the remote device.

 SUCCESS                 -  Successfully processes Active EndpointRequest

 a_NWK_addr[2]
 -This field indicates the network address of remote device. The
 address must be in Little Endian format.

 active_endpoint_count
 -This field indicates the number of active endpoints on the remote
 device.

 NOTE : The field takes the value zero, if status is other than Success.

 a_active_endpoint_list[1]
 -This field indicates the list of endpoints available on the remote
 device. The number of end points is based on active_endpoint_count.

 NOTE: This field is not present, if the status is other than success.

 ******************************************************************************/
typedef struct ZDP_Active_Endpoint_Response_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
    uint8_t active_endpoint_count;
    uint8_t a_active_endpoint_list[1];
}__attribute__((__packed__)) ZDP_Active_Endpoint_Response_t;

/******************************************************************************
 ZDP_Match_Desc_Response_Tag

 This structure notifies the result of MatchDescriptor Request
 information to the application through ZDP.

 status

 INVALID_REQUEST_TYPE    - Network address of interest field does not
 match with the  network address of the remote
 device and the remote device is an end device.

 SUCCESS                 - Successfully processes the Simple Descriptor
 Request .

 a_NWK_addr[2]
 -This field indicates the network address of remote device. The
 address must be in Little Endian format.

 match_length
 -This field indicates the number of endpoints on the remote device
 that matches the simple descriptor.
 NOTE : This field is zero, if the status is other than Success.
 a_match_list
 -This field indicates the list of endponits.

 ******************************************************************************/
typedef struct ZDP_Match_Desc_Response_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
    uint8_t match_length;
   // uint8_t a_match_list[4];
	uint8_t a_match_list[1];
    /* g_MAX_NO_OF_ENDPOINTS_c changed to 1  because
								max end point is an application configuarable*/
}__attribute__((__packed__)) ZDP_Match_Desc_Response_t;

/******************************************************************************
 App_Match_Desc_Response_Tag

 This structure notifies the result of MatchDescriptor Request
 information to the application through ZDP.

 Status

 INVALID_REQUEST_TYPE  - The nwk addr of interst field does not match
 the  nwk addr of the remote device and it is an
 end device.
 DEVICE_NOT_FOUND       - The nwk addr of interst field does not match the
 nwk addr of  the children of the remote device.
 NO_DESCRIPTOR          - If no simple descriptors are available for the
 child indicated by the NWKAddrOfinterst field.
 SUCCESS               - Profile id matched and atleaset InClusterList
 and OutClusterList is matched.For each matching
 the remote device will sends the
 Match_Desc_response command.
 a_NWK_addr[2]
 - nwk addr for the request.

 a_src_addr[2]
 - src address who responded for the Match Desc request.

 match_length
 - no.of endpoints on the remote device that match the request
 criteria.

 a_match_list
 - List of bytes each of which repersents an 8-bit EP.

 ----------------------------------------------------------------------------**/
typedef struct App_Match_Desc_Response_Tag {
    uint8_t status;
    uint8_t a_NWK_addr[2];
    uint8_t a_src_addr[2];
    uint8_t match_length;
    uint8_t a_match_list[1];
}__attribute__((__packed__)) App_Match_Desc_Response_t;
/******************************************************************************
 ZDP_Mgmt_Permit_Joining_Response_Tag

 This structure notifies the results of Mgmt_Permit_Joining.Request
 information to the application through ZDP.

 Status

 INVALID_REQUEST     - This Mgmt primitive is not supported by remote
 device.

 SUCCESS             -  Permit join request is successful
 ----------------------------------------------------------------------------**/
typedef struct ZDP_Mgmt_Permit_Joining_Response_Tag {
    uint8_t status;
}__attribute__((__packed__)) ZDP_Mgmt_Permit_Joining_Response_t;
/******************************************************************************
 ZDP_Mgmt_Nwk_Update_Notify_Tag

 This structure notifies to enable ZigBee devices to report the
 condition on local channels to the network manager. The scanned
 channel list is the report of channels scanned and it is followed
 by a list of records, one for each channel scanned, each record
 including one byte of the energy level measured during the scan,
 or 0xff if there is too much interference on this channel.

 status
 - The status of the Mgmt_NWK_Update_notify command.

 a_scan_channels[4]
 - List of channels scanned by the request.

 a_tx_total[2]
 - Count of the total transmissions reported by the device.

 a_tx_failure[2]
 - Sum of the total transmission failures reported by the device.

 scan_channel_list_cnt
 - The list shall contain the number of records contained in the
 EnergyValues parameter.

 a_scan_channel_energy_list[1]
 - List of ED values.

 ----------------------------------------------------------------------------**/
typedef struct ZDP_Mgmt_Nwk_Update_Notify_Tag {
    uint8_t status;
    uint8_t a_scan_channels[4];
    uint8_t a_tx_total[2];
    uint8_t a_tx_failure[2];
    uint8_t scan_channel_list_cnt;
    uint8_t a_scan_channel_energy_list[1];
}__attribute__((__packed__)) ZDP_Mgmt_Nwk_Update_Notify_t;

/**----------------------------------------------------------------------------
 ZDP_System_Server_Discovery_Response_Tag

 This structure notifies the results of
 System_Server_Discovery_Request information to the application
 thorugh the ZDP.

 Status

 SUCCESS 		  - which indicates that the parameter matches the server
 mask field in its Node Descriptor.

 a_server_mask[2]
 -For description refer to structure of type
 ZDP_System_Server_Discovery_Reqest.

 a_src_addr[2]
 -This field indicates the address of remote device that matches
 the server mask in the request.

 ----------------------------------------------------------------------------**/

typedef struct ZDP_System_Server_Discovery_Response_Tag {
    uint8_t status;
    uint8_t a_server_mask[2];
    uint8_t a_src_addr[2];
}__attribute__((__packed__)) ZDP_System_Server_Discovery_Response_t;

/**----------------------------------------------------------------------------
 ZDP_System_Server_Discovery_Request_Tag

 This structure allows the application to request the ZDP to
 discover the location of a particular system server(device) or
 servers(devices).

 a_server_mask[2]
 -This field indicates the server_mask to be found in a network,this
 is bitmap type. The following values are assigned:

 Bit Number 			Assignment
 0 					Primary Trust Center

 1 					Backup Trust Center

 2 					Primary Binding Table Cache

 3 					Backup Binding Table Cache

 4 					Primary Discovery Cache

 5 					Backup Discovery Cache

 6 					Network/Channel Manager

 7-15					Reserved

 Note:If any of the above bits are set in the server mask, the device
 will have the corresponding capability.For example, if bit 6 is
 set, then the device is Network Manager and possesses all the
 network manager capabilities.

 ---------------------------------------------------------------------------**/

typedef struct ZDP_System_Server_Discovery_Request_Tag {
    uint8_t a_server_mask[2];
}__attribute__((__packed__)) ZDP_System_Server_Discovery_Request_t;

/**----------------------------------------------------------------------------
 ZDP_End_Device_Bind_Request_Tag

 This structure allows the application to request the ZDP to perform
 enddevice bind with a remote device.

 a_binding_target_addr[2]
 - This field indicates the network address of the binding target.If
 source binding is supported, then this field will have the address
 of the local device The address must be in Little Endian format.

 a_src_IEEE_addr[8]
 - This field indicates the 64-bit IEEE address for the local device
 The address must be in Little Endian format.

 src_endpoint
 - This field indicates the endpoint of the for the bind request.

 a_profile_id[2]
 - This field indicates the profile id of the local device.

 in_cluster_count
 - This field indicates the number of input clusters provided for
 end device bind reques within the InClusterList.

 p_in_cluster_list[1]
 - This field indicates the list of input clusterID's to be used
 for matching.

 out_cluster_count
 - This field indicates the number of output clusters provided for
 end device binding within the OutClusterList.

 p_out_cluster_list[1]
 - This field indicates the list of output cluster ID's to be used
 for matching.

 ----------------------------------------------------------------------------**/

typedef struct ZDP_End_Device_Bind_Request_Tag {
    uint8_t a_binding_target_addr[2];
    uint8_t a_src_IEEE_addr[8];
    uint8_t src_endpoint;
    uint8_t a_profile_id[2];
    uint8_t in_cluster_count;
    uint8_t p_in_cluster_list[2];
    uint8_t out_cluster_count;
    uint8_t p_out_cluster_list[2];
}__attribute__((__packed__)) ZDP_End_Device_Bind_Request_t;

/**----------------------------------------------------------------------------
 ZDP_Bind_Request_Tag

 This structure allows the application to request the ZDP to perform
 binding between the given source and destination contained as
 parameters.

 a_src_addr[8]
 - This field indicates the 64-bit IEEE address of the source device.
 The address must be in Little Endian format.

 src_endpoint
 -  This field indicates the endpoint from where the data
 transmission  is initiated.

 a_cluster_id[2]
 - This field indicates the cluster ID for the bind request.

 dest_addr_mode
 - This field indicates the destination address mode.
 The values are:

 0x00         - Reserved

 0x01         - 16-bit group address for destination address and
 destination endpoint not present

 0x02         - Reserved

 0x03         - 64-bit extended address for destination address
 and destination endpoint present

 0x04 to 0xff - Reserved.

 a_dest_addr[8]
 - This field indicates the destination address for the binding
 entry. Destination address may be 2 bytes or 8 bytes depending on
 destination address mode.

 dest_endpoint
 - This field indicates the destination endpoint to which the data
 frame is sent.

 ----------------------------------------------------------------------------**/

typedef struct ZDP_Bind_Request_Tag {
    uint8_t a_src_addr[8];
    uint8_t src_endpoint;
    uint8_t a_cluster_id[2];
    uint8_t dest_addr_mode;
    /*dest addr may be 2 bytes or 8 bytes depends on dst addr mode */
    uint8_t a_dest_addr[8];
    /*dest endpoint may be 1 byte or zero bytes depends on dst addr mode */
    uint8_t dest_endpoint;
}__attribute__((__packed__)) ZDP_Bind_Request_t;

/**----------------------------------------------------------------------------
 ZDP_Unbind_Request_Tag

 This structure allows the application to request the ZDP to perform
 unbinding between the given source and destination contained as
 parameters.

 a_src_addr[8]
 - This field indicates the 64-bit IEEE address of the source device
 The address must be in Little Endian format.

 src_endpoint
 - This field indicates the source endpoint, of the binding table
 entry to be matched for unbinding.

 a_cluster_id[2]
 - This field indicates the cluster to be matched for unbinding.

 dest_addr_mode
 - This field indicates the following destination address mode:
 0x00 - Reserved
 0x01 - 16-bit group address for destination address and
 destination endpoint not present
 0x02 - Reserved
 0x03 - 64-bit extended address for destination address and
 destination endpoint present
 0x04 to  0xff - Reserved

 a _dest_addr[8]
 - This field indicates the destination address for unbinding the
 entry.

 dest_endpoint
 - This field indicates the destination endpoint of the binding
 table entry, to be matched for unbinding.

 ----------------------------------------------------------------------------**/

typedef struct ZDP_Unbind_Request_Tag {
    uint8_t a_src_addr[8];
    uint8_t src_endpoint;
    uint8_t a_cluster_id[2];
    uint8_t dest_addr_mode;
    /*dest addr may be 2 bytes or 8 bytes depends on dst addr mode */
    uint8_t a_dest_addr[8];
    /*dest endpoint may be 1 byte or zero bytes depends on dst addr mode */
    uint8_t dest_endpoint;
}__attribute__((__packed__)) ZDP_Unbind_Request_t;

/**----------------------------------------------------------------------------
 ZDP_End_Device_Bind_Response_Tag

 This structure notifies the results of ZDP_End_Device_Bind.Request
 information to the application through ZDP.

 Status

 NOT_SUPPORTED - The request was directed to a device which was not the
 ZC or that the ZC does not support enddevice binding.

 INVALID_EP    - Supplied endpoint is not within the valid range
 (0x01-0xF0).

 TIMEOUT       - Timer expires before a second End_Device_Bind_Reqest.

 NO_MATCH      - Either two End_Device_Bind_Request's  profile ID
 parameters are not equal or Profile ID parameter is
 matched but there was no match of any element of the
 InClusterList or OutClusterList.

 SUCCESS 		- Successfully processes End Device Bind Request.

 ----------------------------------------------------------------------------**/

typedef struct ZDP_End_Device_Bind_Response_Tag {
    uint8_t status;
}__attribute__((__packed__)) ZDP_End_Device_Bind_Response_t;

/**----------------------------------------------------------------------------
 ZDP_Bind_Response_Tag

 This structure notifies the results of ZDP_Bind_Request_t
 information to the application through ZDP.

 Status

 NOT_SUPPORTED -
 INVALID_EP 	  - Endpoint is not within the valid range(0x01-0xF0)

 TABLE_FULL 	  - Binding table is full.

 SUCCESS       - Successfully creates binding table entry.

 ----------------------------------------------------------------------------**/

typedef struct ZDP_Bind_Response_Tag {
    uint8_t status;
}__attribute__((__packed__)) ZDP_Bind_Response_t;

/**----------------------------------------------------------------------------
 ZDP_Unbind_Response_Tag

 This structure notifies the results of ZDP_Unbind_Request_t
 information to the application through ZDP.

 Status

 NOT_SUPPORTED -

 INVALID_EP    - Endpoint is not within the valid range(0x01-0xF0).

 NO_ENTRY      - Remote device does not have a binding table entry
 corresponding to the parameters received in the
 request.

 SUCCESS       - Binding table entry has been removed from remote
 device.

 ----------------------------------------------------------------------------**/

typedef struct ZDP_Unbind_Response_Tag {
    uint8_t status;
}__attribute__((__packed__)) ZDP_Unbind_Response_t;

/******************************************************************************
 This structure is used when Application wants to send a Management primitive
 to the  ZDP.
 ----------------------------------------------------------------------------**/

typedef struct ZDP_Request_Tag {
    uint8_t msg_type;
    uint8_t reserved;
    uint8_t a_dest_addr[2];
    union ZDP_Request_Command {
        ZDP_NWK_Addr_Request_t nwk_addr_request;
        ZDP_IEEE_Addr_Request_t ieee_addr_request;
        ZDP_Node_Desc_Request_t node_desc_request;
        ZDP_Power_Desc_Request_t power_desc_request;
        ZDP_Simple_Desc_Request_t simple_desc_request;
        ZDP_Active_Endpoint_Request_t active_endpoint_request;
        ZDP_Match_Desc_Request_t match_desc_request;
        ZDP_Device_Annce_Request_t device_annce_request;
        ZDP_Mgmt_Nwk_Update_Request_t mgmt_nwk_update_request;
        Start_Device_Request_t start_device_request;
        ZDP_System_Server_Discovery_Request_t system_server_discovery_request;
        ZDP_End_Device_Bind_Request_t end_device_bind_request;
        ZDP_Bind_Request_t bind_request;
        ZDP_Unbind_Request_t unbind_request;
      #ifdef ZCP_15
        ZDP_Complex_Desc_Request_t complex_desc_request;
        ZDP_User_Desc_Request_t user_desc_request;
        ZDP_User_Desc_Set_t user_descriptor_set;
        ZDP_Discovery_Cache_Request_t discovery_cache_request;
      #endif // #ifdef ZCP_15
    } ZDP_Request_Command;
}__attribute__((__packed__)) ZDP_Request_t;

/******************************************************************************
 This structure is used to notify the Management confirmation or response to
 the Application.
 ----------------------------------------------------------------------------**/

typedef struct ZDP_Response_Tag {
    uint8_t msgtype;
    union ZDP_Response_Command {
        ZDP_NWK_Addr_Response_t nwk_addr_response;
        ZDP_IEEE_Addr_Response_t ieee_addr_response;
        ZDP_Node_Desc_Response_t node_desc_response;
        ZDP_Power_Desc_Response_t power_desc_response;
        ZDP_Simple_Desc_Response_t simple_desc_response;
        ZDP_Active_Endpoint_Response_t active_endpoint_response;
        ZDP_Match_Desc_Response_t match_desc_response;
        ZDP_Mgmt_Permit_Joining_Response_t mgmt_permit_joining_response;
        ZDP_Mgmt_Nwk_Update_Notify_t mgmt_nwk_update_notify;
        Start_Device_Response_t start_device_response;
        NLME_Network_Discovery_Confirm_t network_discovery_conf;
        NLME_Permit_Joining_Confirmation_t permit_joining_conf;
        NLME_Leave_Confirm_t leave_conf;
        NLME_Route_Discovery_Confirm_t route_discovery_conf;
        NLME_Sync_Confirm_t sync_conf;
        NLME_Leave_Indication_t leave_ind;
        ZDP_System_Server_Discovery_Response_t system_server_discovery_response;
        ZDP_End_Device_Bind_Response_t end_device_bind_response;
        ZDP_Bind_Response_t bind_response;
        ZDP_Unbind_Response_t unbind_response;
      #ifdef ZCP_15
        ZDP_User_Desc_Response_t user_desc_response;
      #endif //#ifdef ZCP_15

    } ZDP_Response_Command;
} __attribute__((__packed__)) ZDP_Response_t;

/******************************************************************************
 Group_Table_t

 This structure gives the fields of group table.

 group_addr
 - gives the 16-bit group address to which endpoints are grouped.Valid
 range of group address is 0x0000 - 0xfff7

 a_endpoint
 - gives the 8 bit value that is grouped with the group address.

 ******************************************************************************/
typedef struct Group_Table_Tag {
    uint16_t group_addr;
    uint8_t a_endpoint[g_MAX_ENDPOINTS_IN_A_GROUP_c];
}__attribute__((__packed__)) Group_Table_t;

/*-----------------------------------------------------------------------------
 Source_Binding_Table_Tag

 This Primitve is used to bind two devices with a unique cluster id and
 then perform a data transmission on them.

 src_endpoint
 - gives the endpoint from where the data transmission is initiated.

 a_cluster_id
 - 16 bit value - cluster id is the parameter over which the src
 endpoint and the destination endpoint are bound.

 dest_addr
 - is 16 bit network address. Since source binding is provided only
 dest address is provided in the structure. This is network addr to
 whom data is sent.

 dest_endpoint
 - is the destination endpoint to which the data frame is sent.

 -----------------------------------------------------------------------------*/
typedef struct Source_Binding_Table_Tag {
    uint8_t src_endpoint;
    uint16_t a_cluster_id[g_APSME_MAX_CLUSTERIDS_c];
    uint16_t dest_addr;
    uint8_t dest_endpoint;
}__attribute__((__packed__)) Source_Binding_Table_t;

/*******************************************************************************
 ZigBeeSimpleDescriptor_t
 endpoint
 - An endpoint of the application on this node.

 profileId
 - Identifies the endpoint's application profile.

 deviceId
 - The endpoint's device ID within the application profile.

 deviceVersion
 - The endpoint's device version.

 inputClusterCount
 - is the destination endpoint to which the data frame is sent.

 pInputClusterList
 - Input clusters the endpoint will accept.

 outputClusterCount
 - The number of output clusters.

 pOutputClusterList
 -Output clusters the endpoint may send.

 *******************************************************************************/
typedef struct Simple_Descriptor_Tag {
  profile_id_t     app_profile_id;
  uint16_t         app_device_id;
  uint8_t          app_device_version;
  uint8_t          incluster_count;
  cluster_id_t const *p_incluster_list;
  uint8_t          outcluster_count;
  cluster_id_t const *p_outcluster_list;
}Simple_Descriptor_t,rsi_GetSimpleDescResp;

/*******************************************************************************
 Endpoint Registration
 Field endpoint_id in the structure Endpoint_Description_t denotes the endpoint
 Id. This is value between 0x01 to 0xF0
 Field p_simple_desc is a pointer to a structure of type Simple_Descriptor_t.
 Simple_Descriptor_t is a struture that gives the descriptor for an
 endpoint.
 Field p_data_response is a pointer of type p_Response_CallBackFunction. This is
 used to handle data confirmation/indication when received at the
 NLLE.
 *******************************************************************************/

typedef struct Endpoint_Description_Tag {
    Simple_Descriptor_t  *p_simple_desc;
    uint8_t endpoint_id;
}Endpoint_Description_t;

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/
extern Endpoint_Description_t *gp_Endpoint_Descriptors;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/* This function is used to process end device annce request */
extern uint8_t ZDP_processDeviceAnnceRequest(uint8_t* p_payload);

/******************************************************************************
 * ZDO_Handle_Mgmt_NWK_Permit_Join_Confirm
 ******************************************************************************
 *
 * Return Value :
 *   None
 *
 * Input Parameters:
 *   buffer_index,
 *   *p_buffer
 *
 * Output Parameters:
 *   NONE.
 *
 * Scope of the function:
 *   This function can be called by a module to send confirmation
 *   information to ZDP.
 *
 * Purpose of the function:
 *   To return the confirmation for NWK layer requests to the ZDP.
 *
 * Before the function is called:
 *   NONE.
 *
 * After the function is called:
 *   NONE.
 *
 ******************************************************************************/

void ZDO_Handle_Mgmt_NWK_Permit_Join_Confirm(uint8_t buffer_index,
        uint8_t* p_buffer);

//#if ( g_Create_Mgmt_Nwk_Update_Req_d == 1 )

void sendMgmtUpdateRequest( uint16_t dstAddress, uint32_t scanchannels, uint8_t scanduration,
        uint16_t mngrAddress );
//#endif
/*******************************************************************************
 * ZigBeeSetPowerDescriptor
 ********************************************************************************
 *
 * Return Value :
 *       returns the Cluster id of the endpoint's simple descriptor located at
 *       the specified index
 *
 * Parameters   :
 *
 * Input Parameters:
 *       Node_Power_Descriptor_t power descriptor for the device
 *
 * Output Parameters:
 *
 *       None
 *
 * Scope of the function:
 *       This function can be called from application
 *
 * Purpose of the function:
 *       This function allows the Application to specify the power descriptor
 *       for the device
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
void ZigBeeSetPowerDescriptor(Node_Power_Descriptor_t nodePowerDesc);

/*******************************************************************************
 * ZigBeeGetEndpointId
 ********************************************************************************
 *
 * Return Value :
 *       returns the Endpoint Id value for the specified Index
 *
 * Parameters   :
 *
 * Input Parameters:
 *      index indicates the index of the array. This value should be less than
 *      the Number of endpoints
 *
 * Output Parameters:
 *
 *       NONE.
 *
 * Scope of the function:
 *       This function can be called from application to send
 *       network status.
 *
 * Purpose of the function:
 *        Application to get the Endpoint id located in the specified index.
 *       The index value should be less than the Number of Endpoints supported
 *        on the device.
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeGetEndpointId(uint8_t index);

/*******************************************************************************
 * ZigBeeGetSimpleDescriptor
 ********************************************************************************
 *
 * Return Value :
 *       returns the TRUE or FALSE
 *
 * Parameters   :
 *
 * Input Parameters:
 *       endpointId indicates the 8-bit endpoint id whose simple descriptor needs
 *       to be retrieved
 *
 * Output Parameters:
 *
 *       pSimpleDescriptor pointer to location where simple descriptor for the
 *       specified endpoint needs to be copied.
 *
 * Scope of the function:
 *       This function can be called from application to get the
 *       SimpleDescriptor
 *
 * Purpose of the function:
 *       This function allows the Application to get the Simple descriptor for
 *       the specified endpoint id.
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
bool ZigBeeGetSimpleDescriptor(uint8_t endpointId,
        ZigBeeSimpleDescriptor_t * pSimpleDescriptor);
/*******************************************************************************
 * ZigBeeGetEndpointCluster
 ********************************************************************************
 *
 * Return Value :
 *       returns the Cluster id of the endpoint's simple descriptor located at
 *       the specified index
 *
 * Parameters   :
 *
 * Input Parameters:
 *       endpointId indicates the 8-bit endpoint id whose cluster id needs to be
 *       retrieved
 *       ClusterType indicates the if incluster list should be read or out
 *       cluster list to be read 0 indicates Incluster List 1 indicates
 *       OutClusterList
 *       ClusterIndex indicates the index of the list of which cluster id is
 *       to be read. This index should be less than the number of clusters
 *       supported in the list as read in the simple descriptor
 * Output Parameters:
 *
 *       None
 *
 * Scope of the function:
 *       This function can be called from application to read the endpoint's
 *       cluster in cluster list
 *
 * Purpose of the function:
 *       This function allows the Application to read the endpoint's cluster in
 *       the specified list at the specified index
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint16_t ZigBeeGetEndpointCluster(uint8_t endpointId, uint8_t ClusterType,
        uint8_t ClusterIndex);
/*******************************************************************************
 * ZigBeeBroadcastNWKManagerRequest
 ********************************************************************************
 *
 * Return Value :
 *   returns SUCCESS or any other error code
 *
 * Input Parameters:
 *   NWKManagerShortAddr indicates the 16-bit network address of the Network Manager.
 *   ActiveChannels indicates the new active channel mask.

 * Output Parameters:
 *  None

 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application broadcasts a request to set the identity
 *   of the network manager and the active channel mask. The mask is used when
 *   scanning for the network after missing a channel update.
 *
 * Before the function is called:
 *     None
 * After the function is called:
 *     None
 *
 *******************************************************************************/
uint8_t ZigBeeBroadcastNWKManagerRequest(uint16_t NWKManagerShortAddr,
        uint32_t ActiveChannels);

/*******************************************************************************
 * ZigBeeBroadcastChannelChangeRequest
 ********************************************************************************
 *
 * Return Value :
 *   returns SUCCESS or any other error code
 *
 * Input Parameters:
 *   NewChannel indicates the new channel to which all the devices need to
 *   change to

 * Output Parameters:
 *   None

 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application broadcasts a request to change the
 *   channel. This request may only be sent by the current Network manager.
 *   There is a delay of several seconds from receipt of the broadcast to
 *   changing the channel, to allow time for the broadcast to propagate.
 *
 * Before the function is called:
 *     None
 * After the function is called:
 *     None
 *
 *******************************************************************************/
uint8_t ZigBeeBroadcastChannelChangeRequest(uint8_t NewChannel);

/*******************************************************************************
 * ZDPSendNWKAddrRequest
 ********************************************************************************
 *
 * Return Value :
 *   Returns SUCCESS if the frame was transmitted successfully else returns
 *   appripriate error code
 *
 * Input Parameters:
 *   pIEEEAddrOfInterest pointer to location of IEEE address whose 16-bit Network
 *   address is to be determined
 *   RequestType is a boolean if TRUE indicates single device response if FALSE
 *   indicates extended device response
 *   StartIndex start index of the child devices list
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application to send ZDP network address request to
 *   determine the 16-bit short address of the device whose IEEE address is known.
 *
 * Before the function is called:
 *     The device should be part of the network to transmit this command successfully.

 * After the function is called:
 *     None
 *
 *******************************************************************************/
uint8_t ZDPSendNWKAddrRequest(uint8_t * pIEEEAddrOfInterest, bool RequestType,
        uint8_t StartIndex);
/*******************************************************************************
 * ZDPSendIEEEAddrRequest
 ********************************************************************************
 *
 * Return Value :
 *   Returns SUCCESS if the frame was transmitted successfully else returns
 *   appripriate error code
 *
 * Input Parameters:
 *   shortAddress pointer to location of IEEE address whose 16-bit Network
 *   address is to be determined
 *   RequestType is a boolean if TRUE indicates single device response if FALSE
 *   indicates extended device response
 *   StartIndex start index of the child devices list\
*   APSAckRequired is a boolean if TRUE indicates APS ack is required if FALSE
 *   indicates APS ack is not required
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application to send ZDP IEEE address request to
 *   determine the 16-bit short address of the device whose IEEE address is known.
 *
 * Before the function is called:
 *     The device should be part of the network to transmit this command successfully.

 * After the function is called:
 *     None
 *
 *******************************************************************************/
uint8_t ZDPSendIEEEAddrRequest(uint16_t shortAddress, bool RequestType,
        uint8_t StartIndex, bool APSAckRequired);

/*******************************************************************************
 * ZDPSendDeviceAnnouncement
 ********************************************************************************
 *
 * Return Value :
 *      Returns SUCCESS if the frame was transmitted successfully else returns
 *      appripriate error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *      None
 *
 * Output Parameters:
 *      None
 *
 * Scope of the function:
 *     This function can be called from application to send device announcement

 *
 * Purpose of the function:
 *      This function allows the Application to senda broadcast for a ZDO Device
 *      announcement. Normally, it is NOT required to call this as the stack
 *      automatically sends a device announcement during joining or rejoing,
 *      as per the spec. However if the device wishes to re-send its device
 *      announcement they can use this call.
 *
 * Before the function is called:
 *     The device should be part of the network to transmit this command successfully.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZDPSendDeviceAnnouncement(void);

/*******************************************************************************
 * ZigBeeInitiateEnergyScanRequest
 ********************************************************************************
 *
 * Return Value :
 *   returns SUCCESS or any other error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *   DestAddr -indicates The network address of the device to perform the scan.
 *   ScanChannels - indicates A mask of the channels to be scanned.
 *   ScanDuration- indicates How long to scan on each channel. Allowed values
 *   are 0..5, with the scan times as specified by 802.15.4 (0 = 31ms, 1 = 46ms,
 *   2 = 77 ms, 3 = 138ms, 4 = 261ms, 5 = 507ms).
 *   ScanRetry -indicates The number of scans to be performed on each channel
 *   (1..8)

 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application to request energy scan be performed
 *   and its results returned. This request may only be sent by the current
 *   network manager and must be unicast, not broadcast.

 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   NONE.
 *******************************************************************************/
uint8_t ZigBeeInitiateEnergyScanRequest(uint16_t DestAddr,
        uint32_t ScanChannels, uint8_t ScanDuration, uint16_t ScanRetry);
/*******************************************************************************
 * ZDPSendrMatchDescriptorsRequest
 ********************************************************************************
 *
 * Return Value :
 *      Returns SUCCESS if the frame was transmitted successfully else returns
 *      appripriate error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *      None
 *
 * Output Parameters:
 *      None
 *
 * Scope of the function:
 *

 *
 * Purpose of the function:
 *
 *
 *
 *
 *
 *
 * Before the function is called:
 *
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/

uint8_t ZDPSendMatchDescriptorsRequest(uint16_t shortAddress,
        uint16_t ProfileId, uint8_t *inClusterList, uint8_t inClusterCount,
        uint8_t *outClusterList, uint8_t outClusterCount, bool APSAckRequired,uint16_t dstAddress);

/*******************************************************************************
 * ZigBeeBindRequest
 ********************************************************************************
 *
 * Return Value :
 *      Returns SUCCESS if the frame was transmitted successfully else returns
 *      appripriate error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *      None
 *
 * Output Parameters:
 *      None
 *
 * Scope of the function:
 *

 *
 * Purpose of the function:
 *
 *
 *
 *
 *
 *
 * Before the function is called:
 *
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t
        ZigBeeBindRequest(uint16_t shortAddress, uint8_t *pIEEEAddrOfSource,
                uint8_t sourceEndpoint, uint16_t clusterId,
                uint8_t destAddrMode, Address_t destAddress,
                uint8_t destinationEndpoint, bool APSAckRequired);
/*******************************************************************************
 * ZigBeeUnBindRequest
 ********************************************************************************
 *
 * Return Value :
 *      Returns SUCCESS if the frame was transmitted successfully else returns
 *      appripriate error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *      None
 *
 * Output Parameters:
 *      None
 *
 * Scope of the function:
 *

 *
 * Purpose of the function:
 *
 *
 *
 *
 *
 *
 * Before the function is called:
 *
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t
        ZigBeeUnBindRequest(uint16_t shortAddress, uint8_t *pIEEEAddrOfSource,
                uint8_t sourceEndpoint, uint16_t clusterId,
                uint8_t destAddrMode, Address_t destAddress,
                uint8_t destinationEndpoint, bool APSAckRequired);
/*******************************************************************************
 * ZigBeeEndDeviceBindRequest
 ********************************************************************************
 *
 * Return Value :
 *      Returns SUCCESS if the frame was transmitted successfully else returns
 *      appripriate error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *      None
 *
 * Output Parameters:
 *      None
 *
 * Scope of the function:
 *

 *
 * Purpose of the function:
 *
 *
 *
 *
 *
 *
 * Before the function is called:
 *
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeEndDeviceBindRequest(uint8_t EndpointId, bool APSAckRequired);

uint8_t ZigBeeGetBindingEntry(uint8_t bindIndex,
							  			ZDP_Bind_Request_t * pGetBindingEntry);

uint8_t ZigBeeSimpleDescriptorRequest(uint16_t shortAddress, uint8_t EndpointId);


/******************************************************************************
 * End Of File
 ******************************************************************************/
#endif                                 /* ZDP_INTERFACE_H */
