/*******************************************************************************
 * zdo_interface.h
 ********************************************************************************
 * Program Description:
 *	This file contains all the support functions and structures of the stack
 *	to be used by application.
 *
 *******************************************************************************/

#ifndef ZDO_INTERFACE_H
#define ZDO_INTERFACE_H

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "mac_interface.h"
#include "stack_common.h"
#include "stack_version.h"
#include "zdp_interface.h"
/*******************************************************************************
 * Global Constants
 *******************************************************************************/
#define STACK_MAJOR_VERSION                                 (1)
#define STACK_MINOR_VERSION                                 (2)


#define SOFT_RESET  0
#define HARD_RESET  1

/*******************************************************************************
 g_NWK_MAX_DEPTH_c

 - This macro defines the allowed depth of a device. This value is
 determined  for all devices in the network as per stack profile
 settings. This has to be constant across all devices in the network.

 - nwkMaxDepth = 15

******************************************************************************/
//#if (ZCP_TEST== 1)
//
// #define g_NWK_MAX_DEPTH_c                                   5
//#else
 #define g_NWK_MAX_DEPTH_c                                   15
//#endif


/*******************************************************************************
  This Macro defines the Ack Constant time used to calculate the
	g_APS_ACK_WAIT_DURATION_c
******************************************************************************/
#define g_APS_ACK_CONSTANT_TIME_c                                   0x32

/*******************************************************************************
This Macro defines security encrypt/decrypt delay) to calculate
g_APS_ACK_WAIT_DURATION_c
******************************************************************************/

#define g_APS_ACK_ENC_DEC_DELAY_c                                   0x64

/*******************************************************************************
 g_APS_ACK_WAIT_DURATION_c

 -This macro define the APS acknowledgement wait duration.

This macro define The maximum number of seconds to wait for an acknowledgement to a
transmitted frame. Value is calcuated using  g_APS_ACK_CONSTANT_TIME_c ,
g_APS_ACK_ENC_DEC_DELAY_c,   g_NWK_MAX_DEPTH_c
******************************************************************************/
#define g_APS_ACK_WAIT_DURATION_c      ( g_APS_ACK_CONSTANT_TIME_c *  \
                                      ( g_NWK_MAX_DEPTH_c * 0x02 ) +   \
                                        g_APS_ACK_ENC_DEC_DELAY_c )
/*******************************************************************************
 g_APS_ACK_POLL_TIME_OUT_c

 -Poll rate of end device while waiting for authentication

******************************************************************************/
#define g_APS_ACK_POLL_TIME_OUT_c                 0xFA    /* 250 ms */


/*******************************************************************************
 g_SWITCH_KEY_TIMEOUT_c

 -This macro defines the the time after which active key sequence
 number is changed, once the device receives Switch Key request.

******************************************************************************/
#define g_SWITCH_KEY_TIMEOUT_c                     0x06

/*******************************************************************************
 g_AUTHENTICATION_POLL_RATE_c

 -This macro defines the poll rate of end device while waiting for
 authentication.

******************************************************************************/
#define g_AUTHENTICATION_POLL_RATE_c                0x64    /* 100 ms */


/*******************************************************************************
 g_USE_INSECURE_JOIN_c

 - This macro provides information on whether the device is using
 insecure join when the status is true and not using insecure join
 when status is false.

******************************************************************************/
#define g_USE_INSECURE_JOIN_c              0x01

/*******************************************************************************
 g_SECURITY_TIMEOUT_PERIOD_c  - The time in milliseconds a device
 will wait for an expected  security protocol frame. 200 ms is taken as the
 delay caused because of encryption/decryption.
 150 ms is added for Platform_B due to delay caused because of encryption/
 decryption.Its need to be optimized.
******************************************************************************/
#define g_SECURITY_TIMEOUT_PERIOD_c   (50 * ( 2 * g_NWK_MAX_DEPTH_c) + 200 + 150)


#define g_END_DEVICE_TIMEOUT_c                          0x0a

/*******************************************************************************
 g_AUTOMATIC_POLL_ALLOWED_c

 - This macro provides information on whether automatic poll is
 allowed or not.

******************************************************************************/
#define g_AUTOMATIC_POLL_ALLOWED_c                 0x00



/* This macro is used to enable Frequency agility by application*/
#define STK_NEED_FREQ_AGILITY                               (uint32_t)0x00000001
/* This macro is used to enable Fragmentation by application*/
#define STK_NEED_FRAGMENTATION                              (uint32_t)0x00000002
/* This macro is used to enable Binding by application*/
#define STK_NEED_BINDING                                    (uint32_t)0x00000004
/* This macro is used to enable Source Routing by application*/
#define STK_NEED_SOURCE_ROUTE                               (uint32_t)0x00000008

/*******************************************************************************
 returns the current operating channel
******************************************************************************/
#define ZigBeeGetOperatingChannel( ) 		( m_GET_g_NWK_Logical_Channel_c )

/* returns the short PAN id of the operating Network */
#define ZigBeeGetShortPANId( ) 			( m_GET_g_NWK_panid_c )

#define ZigBeeSetShortPANId(value)          \
    ZigBeeSetShortPANId_t(value)
    //gp_Active_Startup_Attrib_Set->a_pan_id = value
#define ZigBeeSetShortAddress(value)          \
    gp_Active_Startup_Attrib_Set->a_short_address = value

/* Returns the parent's 16 bit short address. The return value is
 undefined for nodes without parents (coordinators and nodes that are not
 joined to a network).  */
#define ZigBeeGetParentShortAddress( ) \
         (m_GET_g_NWK_current_device_type_c != g_COORDINATOR_c)?  \
        gp_Persistent_NIB->NWK_parent_address : g_INVALID_ADDRESS_c

/*  This function allows the Application to retrieve the stack profile
 information to which the device has joined */
#define ZigBeeStackProfile( )		( m_GET_g_NWK_StackProfile_c )

/*  This function allows the Application to retrieve the current
 tree depth where the device has joined */
#define ZigBeeTreeDepth( )			( m_GET_g_NWK_current_device_depth_c )

/*  This function allows the Application to read the number of
 child devices on the node*/
#define ZigBeeReadCountOfChildDevices()       \
    ( m_GET_g_NWK_number_of_end_devices_joined_c + \
										m_GET_g_NWK_number_of_routers_joined_c )

/*  This function allows the Application to read the number of
 child devices on the node*/
#define ZigBeeReadCountOfRouterChildDevices( ) \
										( m_GET_g_NWK_number_of_routers_joined_c )

/*  This function allows the Application to read the maximum count child
 devices on the node */
#define ZigBeeReadMaxChildDevicesCount( )   ( m_GET_g_NWK_MaxChildren_c )


/* MaxRouters not present for ZigBee PRO*/
/*  This function allows the Application to read the maximum count of
 Router child devices on the node */
#define ZigBeeReadMaxChildRouterDevicesCount( )		 ( m_GET_g_NWK_MaxRouter_c )


/* returns the 16-bit short address of the self device */
#define ZigBeeGetSelfShortAddress( ) 	( gp_Persistent_NIB->NWK_short_address )

/* sets the 16-bit short address of the self device */
#define ZigBeeSetSelfShortAddress( value ) 	\
                      ZigBeeSetSelfShortAddress_t(value)
                      //( gp_Persistent_NIB->NWK_short_address = value )


/* returns the 16-bit short address of manager address */
#define ZigBeeGetManagerAddress( ) 	( gp_Persistent_NIB->NWK_manager_address)
/* returns RouteTableSize*/
#define ZigBeeRouteTableSize()       (g_Num_Routing_Entries_c)

/* returns an pointer to 64-bit extended address of the self device */
//#define ZigBeeGetSelfIEEEAddress() 	MAC_getMACExtendedAddress()
#define ZigBeeGetSelfIEEEAddress(a) 	rsi_zigb_get_self_ieee_address(a)

#define g_FREQUENCY_AGILITY_FLAG_d    0x01
#define g_ED_SCAN_FLAG_d              0x02
#define g_ACTIVE_SCAN_FLAG_d          0x03
#define g_NETWORK_FORM_FLAG_d         0x00


/*This function allows the Application to specify the manufacturer code to be
 set in the Node descriptor */
#define ZigBeeSetManufacturerCodeForNodeDesc(ManufacturerCode) \
    (memUtils_memReverseCopy((uint8_t*)&(gp_ZDO_Configuration->node_desc_info.a_manufacturer_code),\
        (uint8_t*)&ManufacturerCode,sizeof(uint16_t)))




/*This enum for searching either short/extended address*/
enum Addr_search {
    g_SEARCH_BY_EXTENDED_ADDRESS_c              = 0x00,
    g_SEARCH_BY_SHORT_ADDRESS_c                 = 0x08
};

/* Enumerations used by the call back function defined for authorizing the
 joining devices in a secured network */
enum {
    g_Unauthorized_Device_c,
    g_Authorized_Device_c
};

/* enums defined for public stack request primitives */
enum Public_Request_Primitives {
    g_APSME_Transport_Key_Request_c = 0xB4,
    g_APSME_Update_Device_Request_c = 0xB6,
    g_APSME_Remove_Device_Request_c = 0xB8,
    g_APSME_Request_Key_Request_c = 0xBA,
    g_APSME_Switch_Key_Request_c = 0xBC,
    g_Start_Device_Request_c = 0xBD,
    g_ZDO_Restart_Request_c = 0xBE,
    g_NLME_Leave_Request_c = 0x7A,
    g_NLME_Network_Discovery_Request_c = 0x86,
    g_APSME_Add_Group_Request_c                 = 0xAB,
    g_APSME_Remove_Group_Request_c              = 0xAD,
    g_APSME_Remove_All_Groups_Request_c         = 0xAF,
    g_APSME_Add_Group_Confirm_c                 = 0xC5,
    g_APSME_Remove_Group_Confirm_c              = 0xC6,
    g_APSME_Remove_All_Groups_Confirm_c         = 0xC7,
	g_APSME_Get_Confirm_c                       = 0xCF,
	g_APSME_Set_Confirm_c                       = 0xC2,
	g_NTS_Route_Discovery_Confirm_c             = 0x93
};

/* enums defined for public stack confirm primitives */
enum Public_Confirm_primitives {
    g_APSME_Transport_Key_Indication_c = 0xC9,
    g_APSME_Remove_Device_Indication_c = 0xCB,
    g_APSME_Switch_Key_Indication_c = 0xCD,
    g_Start_Device_Response_c = 0xCE,
    g_NLME_Network_Discovery_Confirmation_c = 0x87,
    g_NLME_Leave_Confirmation_c = 0x7B,
    g_NLME_Leave_Indication_c = 0x7C,
    g_NLME_Route_Discovery_Confirmation_c = 0x93

};

/*This enum is for KeyTypes of the Transport-Key Primitive */
typedef enum Security_Key_Types_Tag {
    g_Trust_Center_Master_Key_c,
    g_Network_Key_c,
    g_Application_Master_Key_c,
    g_Link_Key_c,
    g_Trust_Center_Link_Key_c,
    g_Next_Network_Key_c
} Security_Key_Types;

/*This enum is for KeyTypes of the Request-Key Primitive */
enum Request_Key_Types {
    g_RequestKey_Network_Key_c = 0x01,
    g_RequestKey_Link_Key_c,
    g_RequestKey_TC_Link_Key_c = 0x04,
};

/*This enum is for KeyTypes of the Transport-Key Primitive */
enum Security_Mode_Types {
    g_No_PreConfig_Key_c,
    g_PreConfig_NWK_Key_c,
    g_PreConfig_Link_Key_c,
};


typedef enum ZigBeeKeyStructBitmask_Tag {

    /* This indicates that the key has a sequence number associated
     with it. (i.e. a Network Key).*/
    g_Key_Has_Sequence_Number_c = 0x01,

    /* This indicates that the key has an outgoing frame counter */
    g_Key_Has_Outgoing_Frame_Counter_c = 0x02,

    /* This indicates that the key has an incoming frame counter */
    g_Key_Has_Incoming_Frame_Counter_c = 0x04,

    /* This indicates that the key has an associated Partner IEEE address
     and the corresponding value within the ZigBeeKeyStructure_t has been
     populated with the data. */
    g_Key_Has_Partner_IEEE_Addr_c = 0x08,

    /* This indicates the key is authorized for use in APS data messages. If the
     key is not authorized for use in APS data messages it has not yet gone
     through a key agreement protocol, such as CBKE (i.e. ECC) */
    g_Key_Is_Authorized_c = 0x10

} ZigBeeKeyStructBitmask_t;

/* ****************************************************************************/
/* FOR APIs*/
/******************************************************************************/
typedef enum {
    ZigBeeNWKIsUp, /*ZigBeeNWKIsUp  indicates of the NWK is formed or joined successfully */
    ZigBeeNWKIsDown, /*ZigBeeNWKIsDown indicates of the NWK formation failed or
     the device left the network */
    ZigBeeJoinFailed, /*ZigBeeJoinFailed indicates if the network join failed*/
    ZigBeeCannotJoinAsRouter, /*ZigBeeCannotJoinAsRouter indicates if the network
     was unable to start as Router */
    ZigBeeChangedNodeID, /*ZigBeeChangedNodeID indicates if the NodeID is
     changed after resolving address conflict*/
    ZigBeeChangedPANId, /*ZigBeeChangedPANId indicates if the PANID is changed
     after resolving PAN ID conflict */
    ZigBeeChangedChannel, /*ZigBeeChangedChannel indicates if the the channel is
     changed due to frequency agility mechanism */
    ZigBeeNoBeacons, /* ZigBeeNoBeacons indicates no beacons during Discovery
     procedure*/
    ZigBeeReceivedKeyInClear, /*ZigBeeReceivedKeyInClear indicates the Network Key
     is received in clear */
    ZigBeeNoNWKKeyReceived, /*ZigBeeNoNWKKeyReceived indicates no Network key is
     received */
    ZigBeeNoLinkKeyReceived, /*ZigBeeNoLinkKeyReceived indicates no Link key is
     received */
    ZigBeePreconfiguredKeyRequired, /* ZigBeePreconfiguredKeyRequired indicates
 Preconfigured Link key is required */
    ZigBeeChangedManagerAddress /* ZigBeeChangedManagerAddress indicates
      nwk manager changed*/
} ZigBeeNWKStatusInfo;



/*******************************************************************************
 * Public Memory Declarations
 *******************************************************************************/
/* Externs for ZDO Configuration extern code Constants */
/*extern uint8_t ga_PBC_Address[2];*/
extern uint8_t ga_Commissioning_Device_Address[2];

/* Holds the security material set contents */
extern Security_Material_Descriptor_t *gp_Security_Material_Descriptor;


/* This structure provides flexibility of enabling or disabling Primitves
of Freuency agility Feature for each ZigBee device type*/
typedef struct Freq_Agility_Configuration_Tag {
    uint8_t ZDO_Frequency_Agility_Supported_d;
    uint8_t ZDO_Mgmt_NWK_Update_req_d;
    uint8_t ZDO_Mgmt_NWK_Update_Notify_d;
    uint8_t ZDO_Create_Mgmt_Nwk_Update_Req_d;
} __attribute__((__packed__))Freq_Agility_Configuration_t;



typedef struct Silent_Startup_Param_Tag{
  uint16_t nwk_short_addr;
  uint16_t nwk_pan_id;
  uint8_t nwk_ext_pan_id[8];
  uint8_t nwk_channel;
} __attribute__((__packed__)) Silent_Startup_Param_t;



/* This structure provides flexibility of enabling or disabling Primitves
of Freuency agility Feature for each ZigBee device type*/
extern Freq_Agility_Configuration_t Freq_Agility_Configuration;
/*******************************************************************************
 * Type Definitions
 *******************************************************************************/
/*******************************************************************************
 ZDO_Request_Tag

 This structure is used when Application wants to send a public_Requests
 to the  Stack.
 *******************************************************************************/
typedef struct ZDO_Request_Tag {
    uint8_t msg_type;
    union ZDO_Public_Request_Tag {
        NLME_Network_Discovery_Request_t network_discovery;
        NLME_Permit_Join_Request_t permit_joining;
        NLME_Leave_Request_t leave;
        NLME_Route_Discovery_Request_t route_discovery;
        NLME_Sync_Request_t sync;
    } ZDO_Public_Request_t;
}__attribute__((__packed__)) ZDO_Request_t;
/*******************************************************************************
 Set_Security_Attribute_Tag

 This structure can be used to set security attributes active key
 sequence number or security material descriptor.

 attribute_type
 - indicates the attribute that wants to be set. If active key
 sequence number is to be set, then the value of this field should
 be g_ZDO_SEC_Set_Sec_Material_Request_c.

 If security material  is to be set, then the value of this field
 should be  g_ZDO_SEC_Set_Active_Key_No_Request_c.

 sec_attribute
 - This is a union of active_key_sequence_number and
 security_material_descriptor.
 *******************************************************************************/

typedef struct Set_Security_Attribute_Tag {
    uint8_t attribute_type;
    uint8_t length;
    union {
        uint8_t active_key_sequence_number;
        Security_Material_Descriptor_t security_material_descriptor;
    } sec_attribute;
}__attribute__((__packed__)) Set_Security_Attribute_t;

/*******************************************************************************
 TC_Master_Link_Key_Transport_Key_Data_Tag

 a_ParentAddress[8]
 -gives the 64-bit address taken from the destination addr parameter
 from the request.

 a_TCMasterLinkKey
 - gives the 16 octets of TrustCenter master key.


 *******************************************************************************/

typedef struct TC_Master_Link_Key_Transport_Key_Data_Tag {
    uint8_t a_TCMasterLinkKey[16];
    uint8_t a_ParentAddress[8];
} __attribute__((__packed__)) TC_Master_Link_Key_Transport_Key_Data_t;

/*******************************************************************************
 Network_Key_Transport_Key_Data_Tag

 key_seq_number
 - gives the sequence number assigned by the Trust center and used
 to distinguish different network keys and incoming frame security
 operations.

 a_network_key[16]
 - gives the 16 octets of network key.

 use_parent
 - is a bit value. if 1 says use parent to forwward the key to the
 destination device. If 0 says dont use parent to forward the key.

 a_parent_address[8]
 - 64-bit valid device address if the useparent bit is 1 then the
 parent's address is used to forward the key to the destination
 device.If use parent bit is 0, then this field is not set.

 *******************************************************************************/

typedef struct Network_Key_Transport_Key_Data_Tag {
    uint8_t key_seq_number;
    uint8_t a_network_key[16];
    uint8_t use_parent;
    uint8_t a_parent_address[8];
} __attribute__((__packed__)) Network_Key_Transport_Key_Data_t;

/*******************************************************************************
 APL_Master_Link_Key_Transport_Key_Data_Tag

 This structure is used to transport the link key in APSME Transport
 key Request.

 a_PartnerAddress[8]
 - any valid device address - gives the extended address of the
 device that also sent this master key.

 initiator
 - gives a bit value. If 1, indicates destination device requested
 the key.If 0, indicates detsination device of the master key did
 not request the key.

 a_APLTCMasterLinkKey[16]
 - 16 octets of link key.

 *******************************************************************************/

typedef struct APL_Master_Link_Key_Transport_Key_Data_Tag {
    uint8_t a_PartnerAddress[8];
    uint8_t initiator;
    uint8_t a_APLTCMasterLinkKey[16];
} __attribute__((__packed__)) APL_Master_Link_Key_Transport_Key_Data_t;

/*******************************************************************************
 Application_Master_Key_Transport_Key_Data_Tag

 This structure is used to transport the Application Master key in
 APSME Transport key Request.

 a_partner_address[8]
 - any valid device address - gives the extended address of the device
 that also sent this master key.

 initiator
 - gives a bit value. If 1, indicates destination device requested
 the key.If 0, indicates detsination device of the master key did
 not request the key.

 a_application_master_key[16]
 - 16 octets of link key.



 typedef struct Application_Master_Key_Transport_Key_Data_Tag {
 uint8_t a_partner_address[8];
 uint8_t initiator;
 uint8_t a_application_master_key[16];
 }__attribute__((__packed__))Application_Master_Key_Transport_Key_Data_t;
 *******************************************************************************/

/*******************************************************************************
 Transport_Key_Data_Tag

 This gives the union of different key data based on key type

 TC_Master_Link_Key_Data
 - gives Trust Center Master key if the key type is Trust Center
 master key.

 Network_Key_Data
 - gives the Network key in the transport key data if the key type
 is network key.

 App_Master_Link_Key_Data
 - gives the Application Master Key if the key type is Application
 Master Key.

 *******************************************************************************/

typedef union Transport_Key_Data_Tag {
    TC_Master_Link_Key_Transport_Key_Data_t TC_Master_Link_Key_Data;
    Network_Key_Transport_Key_Data_t Network_Key_Data;
    APL_Master_Link_Key_Transport_Key_Data_t App_Master_Link_Key_Data;
} __attribute__((__packed__)) Transport_Key_Data_t;

/*******************************************************************************
 Network_Key_Data_Indication_Tag

 key_sequence_number
 - gives the sequence number assigned by the TC to distinguish the
 keys.

 a_network_key[16]
 - gives the 16 octets of network key.

 *******************************************************************************/

typedef struct Network_Key_Data_Indication_Tag {
    uint8_t key_sequence_number;
    uint8_t a_network_key[16];
} __attribute__((__packed__)) Network_Key_Data_Indication_t;
/*******************************************************************************
 App_Master_Link_Key_Data_Ind_Tag

 a_partneraddress
 - any valid device address - gives the extended address of the device

 a_link_key[16]
 - gives the 16 octets of Link key.

 *******************************************************************************/

typedef struct App_Master_Link_Key_Data_Ind_Tag {
    uint8_t a_partneraddress[8];
    uint8_t a_link_key[16];
} __attribute__((__packed__)) App_Master_Link_Key_Data_Ind_t;

/*******************************************************************************
 Transport_Key_Data_Indication_Tag

 This is an union. This gives data based on if the key is Trust
 center master key or Network key.

 a_TC_Master_Link_key[16]
 - gives 16 octets of Trust Center Master Key.

 Nwk_Key_Data_Indication
 - explained in the strcuture definition of
 Network_Key_Data_Indication_t basically for network key indication.
 *******************************************************************************/

typedef union Transport_Key_Data_Indication_Tag {
    uint8_t a_TC_Master_Link_key[16];
    Network_Key_Data_Indication_t Nwk_Key_Data_Indication;
    App_Master_Link_Key_Data_Ind_t App_Master_Link_Key_Data_Ind;
} __attribute__((__packed__)) Transport_Key_Data_Indication_t;

/*******************************************************************************
 Transport_Key_Request_Tag

 This struture is for transporting a key to another device.

 a_dest_address[8]
 -This field indicates the valid 64-bit device address.

 NOTE:If this field is set to zeros then this command will be broadcasted.

 key_type :
 - This field indicates the different types of keys as follows:

 0x00 - Trust Centre Master Key

 0x01 - Network Key

 0x02 - Application Master Key

 0x03 - Application Link Key

 APSME_Transport_Key_Data :
 -This field indicates the actual 16 byte key to be transmitted.

 *******************************************************************************/

typedef struct Transport_Key_Request_Tag {
    uint8_t a_dest_address[8];
    uint8_t key_type;
    Transport_Key_Data_t APSME_Transport_Key_Data;
} __attribute__((__packed__)) Transport_Key_Request_t;

/*******************************************************************************
 Transport_Key_Indication_Tag

 a_srcAddress[8]
 - gives the extended address of the device that is the actual
 source for the transported key.

 key_type
 - identifies the type if the key that is transported.

 Key_Data_Indication
 -  gives the actual key based on the key type.This is explained in
 the structure definition Transport_Key_Data_Indication_t.

 *******************************************************************************/

typedef struct Transport_Key_Indication_Tag {
    uint8_t a_srcAddress[8];
    uint8_t key_type;
    Transport_Key_Data_Indication_t Key_Data_Indication;
} __attribute__((__packed__)) Transport_Key_Indication_t;

/*******************************************************************************
 Update_Device_Request_t

 This primitive is issued to inform a device(trust center) about
 the status of a particular device in the network.For Ex whether the
 device joined the Nwk or left the nwk etc.

 a_dest_address[8]
 - 64 bit address - gives the destination address to whom status of
 the device is sent to update.

 a_device_address[8]
 - 64 bit address whose status needs to be updated with the Trust
 Center.

 Status
 - gives the status of the device

 0x00            - device secured join

 0x01            - device unsecured join

 0x02            - device left

 0x03 to 0x08    - reserved

 device_short_address
 - 16 bit nwk addr that indicates the network  address of the device
 whose status is being updated.

 *******************************************************************************/

typedef struct Update_Device_Request_Tag {
    uint8_t a_dest_address[8];
    uint8_t a_device_address[8];
    uint8_t status;
    uint16_t device_short_address;
} __attribute__((__packed__))  Update_Device_Request_t;

/*******************************************************************************
 Update_Device_Indication_Tag

 This primitive is used to indicate the NHLE that an update device
 command is received.

 device_short_address
 - 16 bit network address which indicates the network address of the
 device whose status is being updated.

 a_device_address[8]
 - 64 bit address whose status needs to be updated with the
 Trust Center .

 Status
 - gives the status of the device

 0x00            - device secured join

 0x01            - device unsecured join

 0x02            - device left

 0x03 to 0x08    - reserved

 a_src_address[8]
 - 64 bit address.This gives the source address from where the
 update device command originated.

 *******************************************************************************/

typedef struct Update_Device_Indication_Tag {
    uint16_t device_short_address;
    uint8_t a_device_address[8];
    uint8_t status;
    uint8_t a_src_address[8];
}__attribute__((__packed__))  Update_Device_Indication_t;

/*******************************************************************************
 Remove_Device_Request_Tag :

 The application must use the Remove Device Request to remove a
 device from the network. The request must be sent to the parent of
 the device, which the Trust Center intends to remove.

 a_parent_address[8]
 - This field indicates the 64-bit address of the device whose child
 needs to be removed from the network.

 a_target_address[8]
 - This field indicates the 64-bit address of the child which needs
 to be removed from the network.
 *******************************************************************************/

typedef struct Remove_Device_Request_Tag {
    uint8_t a_parent_address[8];
    uint8_t a_target_address[8];
}__attribute__((__packed__))  Remove_Device_Request_t;

/*******************************************************************************
 Remove_Device_Indication_Tag

 This primitve is used to indicate the ZDO that the referenced by
 the srcaddress device is requesting the refernced child address to
 be removed from the Nwk.

 a_src_address[8]
 - gives the 64 bit address who initiated the remove device Request
 command.

 a_child_address[8]
 - gives the 64 bit address of the child who should be removed from
 the Nwk.

 *******************************************************************************/

typedef struct Remove_Device_Indication_Tag {
    uint8_t a_src_address[8];
    uint8_t a_target_address[8];
}__attribute__((__packed__))  Remove_Device_Indication_t;

/*******************************************************************************
 Switch_Key_Request_Tag

 Switch Key request is sent to request the device to change to a
 different Network Key.

 a_dest_address[8]
 - This field indicates the 64-bit extended address to which the
 Switch Key command is send.

 NOTE : If this field is set to zeros, then the switch key request will be
 broadcasted.

 key_seq_number
 - This field indicates the key sequence number to switch to.
 *******************************************************************************/

typedef struct Switch_Key_Request_Tag {
    uint8_t a_dest_address[8];
    uint8_t key_seq_number;
}__attribute__((__packed__))  Switch_Key_Request_t;

/*******************************************************************************
 Switch_Key_Indication_Tag

 This Primitve is used to inform the ZDO of the destined device that
 it received a switch key request command.

 a_src_address[8]
 - 64 bit extended address to who initiated the switch key command.

 key_seq_number
 - a byte value sequence number assigned by the Trust center to
 differentiate the keys.
 *******************************************************************************/

typedef struct Switch_Key_Indication_Tag {
    uint8_t a_src_address[8];
    uint8_t key_seq_number;
}__attribute__((__packed__))  Switch_Key_Indication_t;

/*******************************************************************************
 requestKeyRequest

 Switch Key request is sent to request the device to change to a
 different Network Key.

 aDestAddress[8]
 - This field indicates the 64-bit extended address to which the
 Key  request keye command is send.
 keyType
 -The type of key being requested:
 0x01 = Network key
 0x02 = Application key
 0x04 = Trust Center Link Key

 aPartnerAddress
 - If the KeyType parameter indicates an application key, this
 parameter shall indicate an extended 64-bit address of a device that
 shall receive the same key as the device requesting the key.
 *******************************************************************************/

typedef struct requestKeyRequest_Tag {
    uint8_t aDestAddress[8];
    uint8_t keyType;
    uint8_t aPartnerAddress[8];
}__attribute__((__packed__))  requestKeyRequest_t;

/*******************************************************************************
 Security_Commands_Tag
 The application must use the security interface to Transport a new
 key, Switch to the new key and remove a device from the network.
 The following structure defines the requests supported by the
 Security interface.
 *******************************************************************************/

typedef struct Security_Commands_Tag {
    uint8_t msg_type;
    union Security_Request {
        Transport_Key_Request_t transport_key_request;
        Update_Device_Request_t update_device_request;
        Switch_Key_Request_t switch_key_request;
        requestKeyRequest_t requestKeyRequest;
    } Security_Request;
}__attribute__((__packed__))  Security_Commands_t;

/*******************************************************************************
 Add_Group_Request_Tag :

 group_address
 - This field indicates the 16-bit group address to which an
 endpoint needs to be added.

 endpoint
 -This field indicates the endpoint that needs to be added to a
 group.

 *******************************************************************************/
//#pragma pack(1)
typedef struct Add_Group_Request_Tag {
    uint16_t group_address;
    uint8_t endpoint;
}Add_Group_Request_t;

/*******************************************************************************
 Remove_Group_Request_Tag

 group_address
 - This field indicates the 16-bit group address from which an
 endpoint needs to be removed.

 endpoint
 - This field indicates the endpoint that needs to be removed from
 a group.

 *******************************************************************************/

typedef struct Remove_Group_Request_Tag {
    uint16_t group_address;
    uint8_t endpoint;
}__attribute__((__packed__)) Remove_Group_Request_t;
/*******************************************************************************
 Remove_All_Groups_Request

 endpoint
 - end point which should be removed from all groups.

 *******************************************************************************/
typedef struct Remove_All_Groups_Request {
    uint8_t endpoint;
}__attribute__((__packed__)) Remove_All_Groups_Request;

/*******************************************************************************
 Node_Descriptor_Information_Tag

 This is specific to a device/node.

 aps_flags_freq_band
 - gives first 3 bits APS Capability flags(from LSB).Currently this
 is set to 0.The upper 5 bits says the frequency band used.setting
 one of the bit of 5 bits gives the required frequency band.

 b2 b1 b0 (bit 2 bit 1 bit 0)

 0  0  0 - 868 to 868.6 MHz
 0  0  1 - 902 to 928 MHz
 0  1  0 - 2400 to 2483.5 MHz

 MAC_capability_flags
 - This gives the MAC capabilites of a Node

 For ZC  - 0x8A
 For ZR  - 0x8A
 For ZED - 0x88 if receiver on when idle is true and 0x80 if false.

 a_manufacturer_code
 - gives the code of the manufactures allocated by ZigBee Alliance.

 a_max_incoming_transfer_size
 - gives the maximum size that can be sent to APS. This field is
 used along with fragmentation.

 a_max_outgoing_transfer_size
 - gives the maximum size that can be transferred from this node to
 or from the other node in a single message transfer.
 descriptor_capability
 - The descriptor capability field of the node descriptor is eight
 bits in length, with bit settings signifying the descriptor
 capabilities of this node. It is used to facilitate discovery of
 particular features of the descriptor fields by other nodes on the
 system.

 *******************************************************************************/
typedef struct Node_Descriptor_Information_Tag {
    uint8_t aps_flags_freq_band;
    uint8_t MAC_capability_flags;
    uint8_t a_manufacturer_code[2];
    uint8_t max_buffer_size;
    uint8_t a_max_incoming_transfer_size[2];
    uint8_t a_max_outgoing_transfer_size[2];
    uint8_t descriptor_capability;
} __attribute__((__packed__)) Node_Descriptor_Information_t;

/*******************************************************************************
 APSME_Address_Map_Table_Tag

 This structure gives the 16-bit Network Address corresponding to
 the 64-bit IEEE address of a particular device.

 a_IEEE_addr[8]
 - This field indicates the extended 64-bit IEEE address.

 nwk_addr
 -This field indicates the short address corresponding to the
 extended address.

 *******************************************************************************/
typedef struct APSME_Address_Map_Table_Tag {
    uint8_t a_IEEE_addr[8];
    uint16_t nwk_addr;
}APSME_Address_Map_Table_t,rsi_GetAddrMapResp;

/*******************************************************************************

 Startup_Attribute_Set_Tag
 -------------------------

 a_extended_pan_id[8]
 - This field holds the extended PAN ID of the network, in which the
 device needs to be a member.

 channel_mask
 - This field contains the set of channels, the device needs to scan
 as part of the network join or formation procedure.

 protocol_version
 -This field indicates the protocol version available in the node.

 stack_profile
 -This field indicates the stack profile used by the node.

 startup_control
 - This field indicates how the device needs to respond or start
 depending on the startup_control value.

 a_trust_center_address[8]
 - This field contains the 64-bit extended address of the TC.

 a_network_key[16]
 - This field indicates the key that is used for communication in
 the network.

 use_insecure_join
 - This field is a flag which indicates that a device needs to use
 insecure join during startup.

 network_key_seq_num
 - This field contains the sequence number of Network Key.

 network_key_type
 -This field indicates the key type used by the node.

 network_manager_address[8]
 -This field indicates the 64-bit extended address of the network
 manager.

 scan_attempts
 - This field defines the number of scan attempts to be done by the
 device.

 time_between_scans
 - This field defines the time gap between two successive scans.

 rejoin_interval
 - This field indicates the time interval after which a device tries
 to rejoin the network, if disconnected.

 max_rejoin_interval
 - This field indicates the upper time limit within which multiple
 rejoins are done, that are separated by rejoin_intervals.

 security_timeout_period
 - defines the time within which the security authentication
 procedure has to be completed for the device.

 indirect_poll_rate
 - This field indicates the rate at which an end device polls for
 messages, from its parent.

 APS_ack_wait_duration
 -This field indicates the APS acknowledgement time out.

 a_short_address
 -This field indicates the short address of the device being
 commissioned. The device silently joins the network using the short
 address available in this field.

 a_pan_id
 -This field indicates the PAN ID of the device.

 a_master_key[16]
 -This field indicates the master key the node need to use.

 a_preconfigured_link_key[16]
 -This field indicates the link key the node need to use.

 *******************************************************************************/
#if 1
 typedef struct Startup_Attribute_Set_Tag {
    uint8_t a_extended_pan_id[8];
    uint32_t channel_mask;
    uint8_t protocol_version;
    uint8_t stack_profile;
    uint8_t startup_control;
    uint8_t a_trust_center_address[8];
    uint8_t a_network_key[16];
    uint8_t use_insecure_join;
    uint8_t network_key_seq_num;
    uint8_t network_key_type;
    uint16_t network_manager_address;
    uint8_t scan_attempts;
    uint16_t time_between_scans;
    uint16_t rejoin_interval;
    uint16_t max_rejoin_interval;
    uint16_t indirect_poll_rate;
    uint8_t parent_retry_threshold;
    /*Following three attributes are no longer used.
     If we comment this, then code will not be compiled.
     For time being, this is not commented*/
    uint16_t security_timeout_period;
    uint16_t APS_ack_wait_duration;

    uint16_t a_short_address;
    uint16_t a_pan_id;
    uint8_t a_trustcenter_master_key[16];
    uint8_t a_preconfigured_link_key[16];
    /* Concentrator Parameters Attribute Set */
    uint8_t concentrator_flag;
    uint16_t concentrator_radius;
    uint16_t concentrator_discovery_time;

    uint8_t deviceAnnceReq;
    uint8_t end_device_bind_timeout;

}__attribute__((__packed__)) Startup_Attribute_Set_t;
#else

typedef struct Startup_Attribute_Set_Tag {
    uint8_t a_extended_pan_id[8];
    uint32_t channel_mask;
    uint8_t startup_control;
    uint8_t use_insecure_join;
    uint8_t scan_attempts;
    uint8_t parent_retry_threshold;
    uint8_t a_trust_center_address[8];
    uint8_t a_network_key[16];
    uint16_t time_between_scans;
    uint16_t rejoin_interval;
    uint16_t max_rejoin_interval;
    uint16_t indirect_poll_rate;
    uint16_t a_pan_id;
    uint16_t network_manager_address;
    uint8_t a_trustcenter_master_key[16];
    uint8_t a_preconfigured_link_key[16];
    uint8_t end_device_bind_timeout;
} Startup_Attribute_Set_t;
#endif
/******************************************************************************/

/*******************************************************************************

 ZDO_Configuration_Table_Tag
 ---------------------------

 config_permit_join_duration
 - This field indicates the duration for which the device permits
 other devices to join.

 config_NWK_secure_all_frames
 - TC uses this field to determine, whether the network layer
 security is applied to all frames in the network or not.

 config_NWK_alt_protocol_version
 - This field sets the list of protocol version numbers, other than
 the current protocol version number, that the device may choose to
 employ in a PAN that it joins. This attribute is applicable only to
 ZRs or ZEDs. The protocol version numbers in the list needs to
 refer to older versions of the ZigBee Specification.

 config_NVM_save_allowed
 - This field indicates whether NVM save is allowed on the device or
 not.

 config_formation_attempts
 - This field indicates the number of formation retrials, after
 formation failure.

 config_scan_duration
 - The field indicates the duration of active scan while performing
 startup, join or rejoin the network.

 config_join_attempts
 - This field indicates the number of times join is retried once
 the join fails.

 config_preconfigured_key
 - This field indicates whether a preconfigured key is already
 available in the device or not.


 config_no_of_devices_joined_before_NVM_save
 - indicates if there is already a preconfigured key in the device.

 config_no_of_devices_joined_before_NVM_save
 - This field holds the number of devices that have to join before
 doing an NVM store.

 config_no_of_data_txns_before_NVM_save
 - This field  hold the number of data transmissions after which NVM
 store is done.

 a_config_trust_center_short_address
 - This field holds the short address of the TC.

 automatic_poll_allowed
 - This field indicates whether an end device does an auto poll or
 not.

 config_authentication_poll_rate
 - This field indicates the time out in milliseconds for a device to
 wait for authentication after joining the network.

 config_switch_key_time
 - This field indicates the time out in seconds for switching the
 key after receiving a Switch Key Request from the TC.

 config_aps_ack_poll_time_out
 -The field indicates the time out after which an end device polls
 its parent, to retrieve an APS acknowledgement.

 Node_Descriptor_Information node_desc_info
 -This field indicates Node Descriptor information.

 a_reserved[95]
 -This field is reserved for future upgrades.

 *******************************************************************************/
#if 1
typedef struct ZDO_Configuration_Table_Tag {
    uint8_t config_permit_join_duration;
    uint8_t config_NWK_secure_all_frames;
    uint8_t config_NWK_alt_protocol_version;
    uint8_t config_formation_attempts;
    uint8_t config_scan_duration;
    uint8_t config_join_attempts;
    uint8_t config_preconfigured_key;
    uint8_t config_no_of_devices_joined_before_NVM_save;
    uint16_t config_no_of_data_txns_before_NVM_save;
    uint16_t a_config_trust_center_short_address;
    uint8_t automatic_poll_allowed;
    uint8_t config_authentication_poll_rate;
    uint16_t config_switch_key_time;
    uint8_t config_security_level;
    uint8_t config_aps_ack_poll_time_out;
    Node_Descriptor_Information_t node_desc_info;
    uint8_t current_powermode_avail_power_sources;
    uint8_t current_powersource_currentpowersourcelevel;
    uint8_t a_reserved[98];
} __attribute__((__packed__)) ZDO_Configuration_Table_t;
#else
typedef struct ZDO_Configuration_Table_Tag {
    uint8_t config_permit_join_duration;
    uint8_t config_NWK_secure_all_frames;
    uint8_t config_formation_attempts;
    uint8_t config_scan_duration;
    uint8_t config_join_attempts;
    uint8_t config_preconfigured_key;
    uint16_t a_config_trust_center_short_address;
    uint8_t automatic_poll_allowed;
    uint8_t config_authentication_poll_rate;
    uint16_t config_switch_key_time;
    uint8_t config_security_level;
    uint8_t config_aps_ack_poll_time_out;
    uint8_t a_manufacturer_code[2];
} ZDO_Configuration_Table_t;
#endif
/******************************************************************************/

/*******************************************************************************

 ZDO_Information_Base_Tag
 ------------------------

 logical_type
 - gives logical type of the device in the first 3 bits (from LSB),
 4th bit says complex descriptor available or not 5th bit says user
 descriptor available or not and rest of the 3 bits are reserved.

 b2 b1 b0 (bit 2 bit 1 bit 0)
 0  0  0 - ZigBee Coordinator
 0  0  1 - ZigBee Router
 0  1  0 - ZigBee End Device
 0  1  1 to
 1  1  1 - Reserved
 b4 (bit 4)
 1  - complex descriptor available
 b5 (bit 5)
 1 - user descriptor available
 bit6,bit7,bit8 are reserved bits

 a_server_mask[2]
 - gives the system server capabilities of this node setting

 Bit No.
 0          gives primary trust centre
 6 to 15    is reserved

 user_descriptor[8]
 - user descriptor specific to a device/node. This is used to
 identify the device. This shall contain a max of 16 characters.

 a_extended_address[16]
 - holds the 64-bit extended address of the device.
 *******************************************************************************/

typedef struct ZDO_Information_Base_Tag {
    uint8_t logical_type_complex_user_desc_avail;
    uint8_t a_server_mask[2];
    uint8_t a_extended_address[8];
    uint8_t user_descriptor[16];
    uint8_t no_ack_due_to_sync_loss;
#ifdef ZCP_15
    uint8_t field_count;
	uint8_t reserved;
    Language_Character_Set_t language_char;
    uint8_t manufacturer_name[16];
    uint8_t model_name[16];
    uint8_t serial_number[16];
    uint8_t device_url[17];
    uint8_t icon[16];
    uint8_t icon_url[17];
	uint8_t reserved2;
#endif // #ifdef ZCP_15
} __attribute__((__packed__)) ZDO_Information_Base_t;

/*******************************************************************************
 Leave_Request

 This structure allows the Application to enable the current device
 to leave the network or to request the child device to leaves the
 network.

 a_device_address[8]
 - This filed indicates the 64-bit IEEE address of the entity to be
 removed from the network. The value is NULL, if the device performs
 self leave.

 remove_children
 - This field takes the following values:

 * 1 - Removes its child devices
 * 0 - Child devices are not removed from the network

 rejoin
 -  This field takes the following values:

 * 1 - If the device is requested to rejoin the network
 * 0 - Not allowed to rejoin the network

 *******************************************************************************/
typedef struct Leave_Request {
    uint8_t a_device_address[8];
    uint8_t remove_children;
    uint8_t rejoin;
} __attribute__((__packed__)) Leave_Request;

/*******************************************************************************
 Leave_Confirm

 This structure allows the next higher layer to be notified of the
 results of its request for itself or another device to leave the
 network.

 Status
 - This field indicates the status of Leave Request as follows:

 SUCCESS           - Leave Request perfomed successfully

 INVALID_REQUEST   - Device not able to processed the request

 UNKNOWN_DEVICE    - Device address specified in the leave request
 is not part of the network.

 a_device_address[8]
 - This field indicates the 64-bit IEEE address of the device which
 leaves the network. The value is zero, if the device performs a
 self leave.

 *******************************************************************************/
typedef struct Leave_Confirm {
    uint8_t status;
    uint8_t a_device_address[8];
} __attribute__((__packed__)) Leave_Confirm;

/*******************************************************************************
 Leave_Indication

 This structure is used by the device to enable the next higher
 layer to be notified if that device has left the network or if a
 neighboring device has left the network.

 a_device_address[8]
 - This field indicates the 64-bit IEEE address of the device that has
 left the network. The value is zero, if the device performs self
 leave.

 rejoin
 - This filed takes the following values:

 TRUE - If the device is requested to rejoin the network
 FALSE - Not allowed to rejoin

 *******************************************************************************/
typedef struct Leave_Indication {
    uint8_t a_device_address[8];
    uint8_t rejoin;
} __attribute__((__packed__)) Leave_Indication;

/*******************************************************************************
  APSLinkKeySecuriryMaterialSet

    incomingFrameCounter[4];
        - This field indicates the extended 64-bit IEEE address.

    outgoingFrameCounter[4];
        - This structure gives the 16-byte link Key

*******************************************************************************/
typedef struct LinkKeyFrameCounterTag{
    uint8_t incomingFrameCounter[4];
    uint8_t outgoingFrameCounter[4];
}__attribute__((__packed__)) LinkKeyFrameCounter_t;

/*******************************************************************************
  APSLinkKeySecuriryMaterialSet

            This structure gives the 16-bit Network Address corresponding to
            the 64-bit IEEE address of a particular device.

   a_IEEE_addr[8]
            - This field indicates the extended 64-bit IEEE address.

   nwk_addr
            -This field indicates the short address corresponding to the
            extended address.

*******************************************************************************/

typedef struct LinkKeyDeviceKeyPairTag{
    uint8_t deviceAddress[8];
    uint8_t linkKey[16];
} __attribute__((__packed__)) LinkKeyDeviceKeyPair_t;

/*******************************************************************************
 LinkKeyTable

 deviceaddress1[8]
 - Extended of address of device 1
 deviceaddress2[8]
 - Extended of address of device 2
 linkKey[16]
 -  This is a key that is shared exclusively between two devices

 *******************************************************************************/

typedef struct LinkKeyTableTag {
    uint8_t deviceaddress1[8];
    uint8_t deviceaddress2[8];
    uint8_t linkKey[16];
} __attribute__((__packed__))  LinkKeyTable_t;

/*******************************************************************************
 ZigBeeKeyStructure_t
 ZigBeeKeyStructBitmask_t  bitmask -
 g_Key_Has_Sequence_Number_c : This indicates that the key has a sequence
 number associated with it. (i.e. a Network Key).
 g_Key_Has_Outgoing_Frame_Counter_c : This indicates that the key has an
 outgoing frame counter
 g_Key_Has_Outgoing_Frame_Counter_c : This indicates that the key has an
 incoming frame counter
 g_Key_Has_Partner_IEEE_Addr_c : This indicates that the key has an
 associated Partner IEEE address and the corresponding value within
 the ZigBeeKeyStructure_t has been populated with the data.
 g_Key_Is_Authorized_c : This indicates the key is authorized for use in
 APS data messages. If the key is not authorized for use in APS data
 messages it has not yet gone through a key agreement protocol,
 such as CBKE (i.e. ECC)
 Security_Key_Types type-
 g_Trust_Center_Master_Key_c,
 g_Network_Key_c,
 g_Application_Master_Key_c,
 g_Link_Key_c,
 g_Trust_Center_Link_Key_c,
 uint8_t key[16]
 -The actual value of the key to be used for Encryption and
 Decryption.
 uint32_t outgoingFrameCounter -
 This is the outgoing frame counter associated with the key. It will
 contain valid data based on the ZigBeeKeyStructBitmask_t.
 uint32_t incomingFrameCounter-
 This is the incoming frame counter associated with the key. It will
 contain valid data based on the ZigBeeKeyStructBitmask_t.
 uint8_t sequenceNumber -
 This is the sequence number associated with the key. It will contain
 valid data based on the ZigBeeKeyStructBitmask_t.
 uint8_t apartnerIEEEAddress [g_EXTENDED_ADDRESS_LENGTH_c] -
 This is the Partner IEEE Address associated with the key (Link Key).
 It will contain valid data based on the ZigBeeKeyStructBitmask_t.
 *******************************************************************************/
typedef struct ZigBeeKeyStructure_Tag {
    ZigBeeKeyStructBitmask_t bitmask;
    Security_Key_Types type;
    uint8_t key[16];
    uint32_t outgoingFrameCounter;
    uint32_t incomingFrameCounter;
    uint8_t sequenceNumber;
    uint8_t apartnerIEEEAddress[g_EXTENDED_ADDRESS_LENGTH_c];
} __attribute__((__packed__)) ZigBeeKeyStructure_t;

/*******************************************************************************
*******************************************************************************/


typedef struct TrustCenterLinkKeyTableTag {
   LinkKeyDeviceKeyPair_t LinkKeyDevicePair;
   LinkKeyFrameCounter_t  LinkKeyFrameCounter;
} __attribute__((__packed__)) TrustCenterLinkKeyTable_t;

/*******************************************************************************
 * Public Memory Declarations
 *******************************************************************************/
/* Extern declaration of pointers pointing to the ZDO and SAS tables */

extern ZDO_Configuration_Table_t gp_ZDO_Configuration[];
extern Startup_Attribute_Set_t gp_Active_Startup_Attrib_Set[];

/* Extern declaration of pointers for ZDO Information Base */
extern ZDO_Information_Base_t *gp_ZDO_Information_Base;
/* Extern of Default Instance of ZDO Information Base */
extern const ZDO_Information_Base_t Default_ZDO_Information_Base;

#if ( g_ADDRESS_MAP_NVM_STORAGE_d == RSI_DISABLE )
extern APSME_Address_Map_Table_t *gp_Address_Map_Table;
#else
extern uint32_t addressMapTableNVMLocation;
#endif /*( g_ADDRESS_MAP_NVM_STORAGE_d == RSI_DISABLE )*/

#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )
extern LinkKeyDeviceKeyPair_t *gpLinkKeyDeviceKeyPairTable;
#endif /*( g_APS_LAYER_SEC_PROCESSING_d == 1 )*/


#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )
extern LinkKeyFrameCounter_t *gpLinkKeyFrameCounterTable;
#endif /*( g_APS_LAYER_SEC_PROCESSING_d == 1 )*/

#if ( ( g_TRUST_CENTRE_d == 1 ) && ( g_USE_HASH_KEY == 0 ) )
#if ( g_APP_LINK_KEY_TABLE_NVM_STORAGE_d == RSI_DISABLE )
extern LinkKeyTable_t gp_AppLinkKeyTable;
#endif /* g_APP_LINK_KEY_TABLE_NVM_STORAGE_d */
#endif /* ( ( g_TRUST_CENTRE_d == 1 ) && ( g_USE_HASH_KEY == 0 ) ) */

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/
/*******************************************************************************
 ZDP_processRequest

 *   Return Value:
 *       None
 *
 *
 *  parameters:
 *  Input parameter:
 *      data_indication_index - gives the index of the pointer
 *                                                   data_indication_index
 *  Output parameter:
 *       p_data_indication - this pointer is an ouput pointer and is
 *       restructured to hold the response frame.
 *       None
 *
 * Scope of the function:
 *   This function is called by the APS layer when the data frame is received
 *   for endpoint 0.
 *
 *  Purpose of the function:
 *   This function takes the received pointer and constructs the response
 *   frame using the same buffer and queues it up for the APS layer.
 *   The function creates the zdp payload for response.Queues it up for the
 *   APS layer.
 *
 *
 *  Before the function is called:
 *   p_data_indication, data_indication_index should be allocated since the
 *   buffer is resued to create the response frame.
 *
 *  After the function is called:
 *    data_indication_index should be freed appropriately.
 *
 *******************************************************************************/
void ZDP_processRequest(uint8_t data_indication_index);
/*******************************************************************************
 * ZDO_setExtendedAddress
 ********************************************************************************
 *
 * Return Value :
 *
 *  void
 *       - No return value.
 *
 * Input Parameters:
 *           IEEE ADDRESS
 *
 * Output Parameters:
 *           NONE.
 *
 * Scope of the function:
 *           This function is called in the main function to assign IEEE address
 *           ZigBee Stack.
 *
 * Purpose of the function:
 *           This function Sets the IEEE adddress to the device
 * Before the function is called:
 *          This address function is written only for the testing purpose
 *          By default the IEEE address shall be in the Chip while the stack is Loaded
 * After the function is called:
 *           NONE.
 *
 ******************************************************************************/
void ZDO_setExtendedAddress(uint8_t * address);
/*******************************************************************************
 *ZDO_startDevice
 ********************************************************************************
 *
 * Return Value :
 *
 *  void
 *       - No return value.
 *
 * Input Parameters:
 *           Extended PAN ID.
 *           Short PAN ID
 *           END DEVICE REQUIRED
 *           TIME BW SCAN
 *
 * Output Parameters:
 *           NONE.
 *
 * Scope of the function:
 *           This function is called in the main function to initialize the
 *           ZigBee Stack.
 *
 * Purpose of the function:
 *           This function initializes required parameters to operate the ZigBee N/W
 *           Stack by invoking respective init
 *
 * Before the function is called:
 *           NONE.
 *
 * After the function is called:
 *           NONE.
 *
 ******************************************************************************/
void ZDO_startDevice(uint8_t *extPANid, uint8_t *shortPANid,
        uint8_t endDeviceAnnceReq, uint8_t timeBwScan);
/*******************************************************************************
 * ZDOinitSTK
 ********************************************************************************
 *
 * Return Value :
 *
 *  void
 *       - No return value.
 *
 * Input Parameters:
 *           NONE.
 *
 * Output Parameters:
 *           NONE.
 *
 * Scope of the function:
 *           This function is called in the main function to initialize the
 *           ZigBee Stack.
 *
 * Purpose of the function:
 *           This function initializes the Stack by invoking respective init
 *           functions of Test Profile, APS, NWK ZDO.
 *
 * Before the function is called:
 *           NONE.
 *
 * After the function is called:
 *           NONE.
 *
 *******************************************************************************/
void ZDOinitSTK(void);

/*******************************************************************************
 * ZDOmainSTK
 ********************************************************************************
 *
 * Return Value
 *
 *  void
 *       - No return value.
 *
 * Input Parameters
 *           NONE.
 *
 * Output Parameters
 *           NONE.
 *
 * Scope of the function:
 *           This function is called by the main to check if any message is
 *           pending for the stack.
 *
 * Purpose of the function:
 *           This function in turn checks if any messages are pending in APS,NWK
 *           layer,ZDO and forwards the message to respective layers.
 *
 * Before the function is called:
 *           NONE.
 *
 * After the function is called:
 *           NONE.
 *
 *******************************************************************************/
void ZDOmainSTK(void);

/*******************************************************************************
 * ZDOresetSTK
 ********************************************************************************
 *
 * Return Value :
 *
 *   void
 *           - No return value.
 *
 * Input Parameters:
 *           resetType - This indicates the type of reset
 *              SOFT RESET - Resets only the stack variables
 *              HARD RESET  - Resets the stack variables and clears the NVM
 *                  locations
 *
 * Output Parameters:
 *           NONE.
 *
 * Scope of the function:
 *           This function is called by the main to reset the ZigBee Stack.
 *
 * Purpose of the function:
 *           This function in turn resets the APS layer, ZDO and the NWK Layer.
 *
 * Before the function is called:
 *           NONE.
 *
 * After the function is called:
 *           NONE.
 *
 *******************************************************************************/
void ZDOresetSTK( uint8_t resetType );

/*******************************************************************************
 * ZDO_AppZDPrequest
 ********************************************************************************
 *
 * Return Value :
 *           returns success if queued the message successfully queue_full if
 *           the queue is full.
 *
 * Input Parameters:
 *       message_index
 *           -This provides the index to the buffer pointer that holds a pointer
 *           of type ZDP_Request_t structure.
 *
 * Output Parameters:
 *             NONE.
 *
 * Scope of the function:
 *           This function is called when the message should be queued in
 *           APP_ZDO_Queue to be processed by the ZDO.
 *
 * Purpose of the function:
 *           This function is called by the Application whenever a message for
 *           ZDP need to be posted. In this function, the message is queued up
 *           for the ZDP to process the same. The queue is maintained by ZDP.
 *
 * Before the function is called:
 *           message_index should be allocated.
 *******************************************************************************/
uint8_t ZDO_AppZDPrequest(uint8_t message_index);

/*******************************************************************************
 * ZDP_Get_Nwk_Library_Version
 ********************************************************************************
 *
 * Return Value :
 *           returns the current network library version .
 *
 * Input Parameters:
 *           NONE.
 *
 * Output Parameters:
 *           NONE.
 *
 * Scope of the function:
 *           This function is called by the application when it wishes to know
 *           the network library version.
 *
 * Purpose of the function:
 *           This function gets the network library version and returns it to
 *           the Application or to the function which invoked.
 *
 * Before the function is called:
 *           NONE.
 *
 * After the function is called:
 *          NONE.
 *
 *******************************************************************************/
uint16_t ZDP_Get_Nwk_Library_Version(void);

/*******************************************************************************
 * ZDP_APPresponse
 ********************************************************************************
 *
 * Return Value :
 *  void
 *       - No return value.
 *
 * Input Parameters:
 *
 *     buffer_index
 *           - it holds the data to be sent to the application.
 *
 * Output Parameters:
 *           NONE.
 *
 * Scope of the function:
 *
 *
 * Purpose of the function:
 *           This function is a callback function used by the stack to send any
 *           confirmation or indication message to the application.
 *
 * Before the function is called:
 *           buffer_index should be allocated
 *
 * After the function is called:
 *           NONE.
 *
 *******************************************************************************/
void ZDP_APPresponse(uint8_t buffer_index);

/*******************************************************************************
 APS_APSMEsearchAddress
 *
 *           This primitive is issued to the APS layer from the NHLE when it
 *           wishes to get the IEEE address or a Short address for a known short
 *           addressor IEEE address  respectively.
 *
 *   Input Parameters:
 *
 *  p_short_address
 *           - It is a in/out parameter,It gives the pointer which holds the
 *           16-bit Nwkshort address depending on 'mode' parameter.
 *
 *                   mode = 0x00 this parameter is out parameter.
 *
 *                   mode = 0x08 this parameter is in parameter.
 *
 *
 *   p_extended_address
 *           - It is an in/out parameter,It gives the pointer which holds the
 *           64-bit extended address depending on 'mode' parameter.
 *
 *                   mode = 0x00 this parameter is in parameter.
 *
 *                  mode = 0x08 this parameter is out parameter.
 *
 *    mode
 *           - it is an input parameter.
 *
 *  Scope of the function:
 *           This function is global function.
 *
 *  Purpose of the function:
 *
 *   mode = 0x08:
 *               This function checks if the short address is present in the
 *              address map table or the neighbor table.If found in either of
 *              these, it gives the corresponding IEEE address in the out
 *              pointer and returns success.If the short address is not found in
 *              either of these tables, it returns 0xff meaning failure.
 *
 *    mode = 0x00:
 *               This function checks if the extended address is present in the
 *              address map table or the neighbor table.If found in either of
 *              these, it gives the corresponding short address in the out
 *              pointer and returns success.If the Extended address is not found
 *              in either of these tables, it returns 0xff meaning failure.
 *
 *
 *  Before the function is called:
 *              p_Extended_Address pointer/p_short_address should be allocated
 *              depending on 'mode' parameter.
 *
 *  After the function is called:
 *              p_extended_address pointer/p_short_address pointer should be
 *              de-allocated in case getting of IEEE address/short address is
 *              failure.
 *
 *******************************************************************************/
uint8_t APS_APSMEsearchAddress(uint16_t* p_short_address,
        uint8_t* p_extended_address, uint8_t mode);

/*******************************************************************************
 * APS_APSMEsetAddressMapTable
 *
 *              This primitive is issued when the address map table is to be
 *              updated.
 *
 *   Return Value:
 *          success     - gives if the entry is added successfully.
 *
 *          table_full  - gives if the table is full and entry could not be made.
 *
 *  Parameters:
 *
 *  Input parameter:
 *      p_addr_map_table_entry
 *                       - gives the pointer that holds the address map table
 *                          entry that needs to be added into the address map table.
 *
 *  Output parameter:
 *            NONE.
 *
 *  Scope of the function:
 *           This function is called by the NHLE or the ZDP module to update the
 *           address map table.
 *
 *  Purpose of the function:
 *            This function updates the provided address info into the table by
 *           incrementing the index. If it reaches max address map entries,
 *           returns table full otherwise returns success.If the entry
 *           corresponding to the address is already present,it updates the 16
 *           bit Nwk address corresponding to the IEEE address in the table and
 *           returns success.
 *
 *  Before the function is called:
 *           p_Addr_Map_Table pointer should be allocated.
 *
 *  After the function is called:
 *           p_Addr_Map_Table pointer should be de-allocated.
 *
 *******************************************************************************/
uint8_t APS_APSMEsetAddressMapTable(
        APSME_Address_Map_Table_t* p_addr_map_table_entry);

/*******************************************************************************
 ZDO_SEC_Send_Transport_NWK_Key_Request
 *
 *      This function is called when transport key request is to be created.
 *
 *   Return Value:
 *             NONE.
 *
 *  Parameters:
 *
 *  p_buffer
 *                 -pointer to buffer containing  join indication .
 *
 *  parent_address
 *           - This parameter contains pointer to extended address of parent
 *           address if the transport key is to be send to parent, otherwise
 *           this parameter contains NULL.
 *
 *  Output parameter:
 *            NONE.
 *
 *  Scope of the function:
 *           This function is called by ZDO network manager when it wants to
 *           send transport key request.
 *
 *  Purpose of the function:
 *            This function creates Transport key request.
 *
 *  Before the function is called:
 *           NONE.
 *
 *  After the function is called:
 *           NONE.
 *
 *******************************************************************************/
void ZDO_SEC_Send_Transport_NWK_Key_Request(uint8_t* pbuffer,
        uint8_t* parent_address);

/*******************************************************************************
 ZDO_SEC_Get_NetworkKeyData
 *
 *      This function is used to get Network Key Data
 *
 *   Return Value:
 *             NONE.
 *
 *  Parameters:
 *    pTransportkeykeyData :
 *
 *
 *  Scope of the function:
 *      This function is used to get the Network Key data
 *
 *  Purpose of the function:
 *
 *
 *  Before the function is called:
 *           NONE.
 *
 *  After the function is called:
 *           NONE.
 *
 *******************************************************************************/

void ZDO_SEC_Get_NetworkKeyData
    (Network_Key_Transport_Key_Data_t* pTransportkeykeyData);


/*******************************************************************************
 ZDO_SEC_Send_Update_Device_Request
 *
 *      This function is called when Update_Device request is to be created.
 *
 *   Return Value:
 *             NONE.
 *
 *  Parameters:
 *   Input parameter
 *     p_buffer:
 *           -pointer to buffer containing either join indication or leave
 *           indication.
 *
 *     device_status
 *           -This parameter can contain the following values
 *
 *                  g_Secured_Rejoin_c,
 *                  g_Unsecured_Join_c,
 *                  g_Device_Left_c,
 *                 g_Unsecured_Rejoin_c,
 *                  g_Invalid_Index_c
 *
 *
 *  Output parameter:
 *            NONE.
 *
 *  Scope of the function:
 *           This function is called by ZDO network manager when it wants to
 *           send transport key request.
 *
 *  Purpose of the function:
 *            This function creates Transport key request.
 *
 *  Before the function is called:
 *           NONE.
 *
 *  After the function is called:
 *           NONE.
 *
 *******************************************************************************/
void ZDO_SEC_Send_Update_Device_Request(uint8_t device_status,
        uint8_t* p_buffer);

/*******************************************************************************
 * APS_APSMEaddGroup
 *
 *            This primitve is issed from the NHLE when an endpoint needs to be
 *           added to a particular group.
 *
 *   Return Value:
 *
 *       g_SUCCESS_c  (0x00)     - Success
 *
 *
 *       g_TABLE_FULL_c (0x9C)   - Group Table Full
 *
 *  Parameters:
 *
 *  Input parameter:
 *
 *         group_address  - gives 16 bit address to which the endpoint is
 *                          added.
 *
 *
 *         endpoint        - gives the endpoint id which is added to the given
 *                          group address.
 *
 *  Output parameter:
 *            NONE.
 *
 *  Scope of the function:
 *         This function is invoked by the NHLE when an endpoint needs to be
 *           added to group table.
 *
 *  Purpose of the function:
 *            This function checks for the entry in the group table,if already
 *           present simply returns success, if no entry is there corresponding
 *           to the i/p parameters it adds an entry in the group table and
 *           returns success.Validation of group address and endpoint is
 *           performed in this function.
 *
 *  Before the function is called:
 *         NONE.
 *
 *  After the function is called:
 *         NONE.
 *
 *******************************************************************************/
uint8_t APS_APSMEaddGroup(Add_Group_Request_t* p_add_group_entry);

/*******************************************************************************
 * APS_APSMEremoveGroup
 *
 *            This Primitive is issued by the NHLE to remove an entry
 *           corresponding to the endpoint in the group table.
 *
 *   Return Value:
 *
 *       g_SUCCESS_c (0x00)           - Success.
 *
 *       g_INVALID_PARAMETER_c (0xC1) - Group or Endpoint Not Found or Invalid
 *                                    Parameter.
 *
 *  Parameters:
 *     Input parameter:
 *
 *         group_address - gives 16 bit group address
 *
 *         endpoint - gives the endpoint id which is grouped with the group address
 *
 *  Output parameter:
 *            NONE.
 *
 *  Scope of the function:
 *         This function is invoked by the NHLE when an endpoint needs to be
 *           removed from the group table.
 *
 *  Purpose of the function:
 *            This function checks for the entry corresponding to the endpoint in
 *           the group table. If found, the entry is removed and a return value
 *           of success is returned.Validation of group address and endpoint is
 *           performed in this function.If the given group address or the
 *           endpoint doesn't lie in the valid range a return value with status
 *           invalid parameter is returned.
 *
 *  Before the function is called:
 *         NONE.
 *
 *  After the function is called:
 *         NONE.
 *
 *******************************************************************************/
uint8_t APS_APSMEremoveGroup(Remove_Group_Request_t* p_remove_group_entry);

/*******************************************************************************
 * APS_APSMEremoveAllGroups
 *
 *            This primitve is used to remove an endpoint from the group
 *           membership.Hence group address frames is not delivered to this
 *           endpoint.
 *
 *   Return Value:
 *
 *       g_SUCCESS_c  (0x00)            - Success
 *
 *       g_INVALID_PARAMETER_c (0xC1)   - Endpoint not found or Invalid Parameter
 *
 *  Parameters:
 *     Input parameter:
 *
 *         end_point - gives the endpoint id which is grouped with the group address
 *
 *     Output parameter:
 *             NONE.
 *
 * Scope of the function:
 *           This function is invoked when an endpoint is to be removed from the
 *           group table so that group addressed frames are not delivered to
 *           this endpoint.
 *
 *  Purpose of the function:
 *            This function checks the group table for all entries corresponding
 *           to the given endpoint. If present, removes all the entries.
 *           Validation of endpoint is performed in this function.If the
 *           specified endpoint doesn't lie within the valid range or it is an
 *           inactive endpoint a return value of status invalid parameter is
 *           returned. If all the entries are removed successfully, a return
 *           value of success is returned.
 *
 *  Before the function is called:
 *         NONE.
 *
 *  After the function is called:
 *           NONE.
 *
 *******************************************************************************/
uint8_t APS_APSMEremoveAllGroups(uint8_t end_point);

/*******************************************************************************
 * APS_getSetAuthenticationBitInNBT
 ********************************************************************************
 *
 *  Return Value :
 *           returns whether the device is authenticated or not. The return
 *           value should be ignored if is_set is g_TRUE_c as in this case we
 *           are setting the the value as device authenticated .
 *
 *  Parameters   :
 *
 *  Input Parameters:
 *
 *     is_set
 *           - This parameter indicates whether authentication bit to be set to
 *             a value indicating its authenticated .
 *
 *     extended_address
 *           - extended address of device whose authentication.
 *             info must be set/ read
 *
 *  Output Parameters:
 *           NONE.
 *
 *  Scope of the function:
 *           This function can be called from other modules.
 *
 *  Purpose of the function:
 *              The function sets the authentication bit in NBT of device to a
 *           value indicating authenticated if is_set is set true. If the
 *           parameter is_set is false, this function returns the status of
 *           device whether its authenticated or not.
 *
 *  Before the function is called:
 *           NONE.
 *
 *  After the function is called:
 *           NONE.
 *
 *******************************************************************************/
uint8_t APS_getSetAuthenticationBitInNBT(uint8_t is_set,
        uint8_t *extended_address);
/*******************************************************************************
 * TH_setPollRate
 ********************************************************************************
 *
 *  Return Value :
 *               NONE.
 *
 *  Parameters   :
 *
 *  Input Parameters:
 *       poll_time
 *           -the time at which end device should poll.
 *
 *  Output Parameters:
 *           NONE.
 *
 *  Scope of the function:
 *              This function is exposed to other modules
 *
 *  Purpose of the function:
 *              This function is called when end device should be polled at a
 *           different rate.This function starts the timer also.
 *
 *  Before the function is called:
 *           NONE.
 *
 *  After the function is called:
 *           NONE.
 *******************************************************************************/

#if ( g_ZIGBEE_END_DEVICE_d == 1 )
void TH_setPollRate (uint16_t poll_time);
#endif                                      /* ( g_ZIGBEE_END_DEVICE_d == 1 ) */

/*******************************************************************************
 * ZDO_isAuthorizedDevice
 ********************************************************************************
 *
 *  Return Value :
 *              returns 0x00 if the device is present in black list, otherwise
 *           gives 0xff.
 *
 *  Input Parameters:
 *
 *       p_extended_address
 *           - extended address of device that has to be checked for the
 *           presense in black list.
 *
 *  Output Parameters:
 *             NONE.
 *
 *  Scope of the function:
 *           This function is exposed to application.
 *
 *  Purpose of the function:
 *           This function checks the presence of device in black list.
 *
 *  Before the function is called:
 *           NONE.
 *
 *  After the function is called:
 *           NONE.
 *
 *******************************************************************************/

#if( g_TRUST_CENTRE_d == 1 )
extern uint8_t ZDO_isAuthorizedDevice (uint8_t *p_extended_address);
#endif                                                  /* g_TRUST_CENTRE_d */

/*******************************************************************************
 * STKcheckIdle
 ********************************************************************************
 *
 * Return Value :
 *
 *              returns 1 if there are no activity to be performed by the stack.
 *
 *           returns 0 if there is some activity to be performed by the stack.
 *
 * Input Parameters:
 *
 *             NONE.
 *
 * Output Parameters:
 *
 *            NONE.
 *
 * Scope of the function:
 *           This function is exposed to application to check if the stack is
 *           idle.
 *
 * Purpose of the function:
 *           This function checks if the stack is idle by verifying all queues
 *           for pending messages and if any task is in critical section. If
 *           none of them is happening, the function returns true indicating MCU
 *           is idle.If any queue has pending messages or it is in critical
 *           section, it returns false.
 *
 * Before the function is called:
 *
 *           NONE.
 *
 * After the function is called:
 *
 *           NONE.
 *
 *******************************************************************************/
uint8_t STKcheckIdle(void);
uint8_t STKcheckIdle_modified(void); //FIXME TODO Added by Sateesh for Coex

/*******************************************************************************
 * Address_Conflict_Detection
 ********************************************************************************
 *
 * Return Value :
 *   returns 1 if address conflict found
 *   returns 0 if no address conflict
 * Input Parameters:
 *
 *             None.
 *
 * Output Parameters:
 *
 *            None.
 *
 * Scope of the function:
 *           This function is exposed to other modules
 *
 * Purpose of the function:
 *           This function is used to find out the address conflict of own addr
 *
 * Before the function is called:
 *
 *           None.
 *
 * After the function is called:
 *
 *           None.
 *
 *******************************************************************************/
#if (g_NWK_ADDRESS_CONFLICT_DETECTION_d == 1)
uint8_t Address_Conflict_Detection(uint8_t* short_address,
        uint8_t* extended_address);

#endif

//void ZDO_NVMWrite(uint8_t process);
/*******************************************************************************
 * ZigBeeFindNetworkAndPerformRejoin
 ********************************************************************************
 *
 * Return Value :
 *   returns the SUCCESS or FAILURE
 *
 * Parameters   :
 *
 * Input Parameters:
 *   Secured- indicates if rejoin command is issued with security or not. -
 *   TRUE indicates with security meaning Network key is known, FALSE indicates
 *   without security
 *   channelMask -indicates the channel mask in which rejoin is performed
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *
 *
 * Before the function is called:
 *   The Application should initiate Rejoin with security initially.
 *   Only if this fails, the Application should initiate rejoin without security
 * After the function is called:
 *
 * NONE.
 *******************************************************************************/
#if(( g_ZIGBEE_ROUTER_d == 1 ) || ( g_ZIGBEE_END_DEVICE_d == 1 ))
uint8_t ZigBeeFindNetworkAndPerformRejoin( bool Secured, uint32_t channelMask );
#endif

/*******************************************************************************
 * ZigBeeRejoinNetwork
 ********************************************************************************
 *
 * Return Value :
 *   returns the SUCCESS or FAILURE
 *
 * Parameters   :
 *
 * Input Parameters:
 *   Secured- indicates if rejoin command is issued with security or not. -
 *   TRUE indicates with security meaning Network key is known, FALSE indicates
 *   without security
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *
 *
 * Before the function is called:
 *   The Application should initiate Rejoin with security initially.
 *   Only if this fails, the Application should initiate rejoin without security
 * After the function is called:
 *
 * NONE.
 *******************************************************************************/

#if(( g_ZIGBEE_ROUTER_d == 1 ) || ( g_ZIGBEE_END_DEVICE_d == 1 ))
uint8_t ZigBeeRejoinNetwork ( bool Secured );
#endif

/*******************************************************************************
 * ZigBeeRequestLinkKey
 ********************************************************************************
 *
 * Return Value :
 *   SUCCESS if the call succeeds, or g_FAILURE_c.
 *
 * Parameters   :
 *
 * Input Parameters:
 *   partnerIEEEAddr The IEEE address of the partner device
 *   trustCentreIEEEAddr The IEEE address of the Trust Centre device
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *
 * Before the function is called:
 *
 * After the function is called:
 *
 * NONE.
 *******************************************************************************/
uint8_t ZigBeeRequestLinkKey(uint8_t* trustCentreIEEEAddr,
        uint8_t* partnerIEEEAddr);

/*******************************************************************************
 * ZigBeeHaveLinkKey
 ********************************************************************************
 *
 * Return Value :
 *   boolean Returns TRUE if a link key is available
 *
 * Parameters   :
 *
 * Input Parameters:
 *   pRemoteDeviceIEEEAddr The long address of a some other device in the network
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   Returns TRUE if a link key is available for securing messages sent to the
 *   remote device
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
bool ZigBeeHaveLinkKey(uint8_t *pRemoteDeviceIEEEAddr);
/*******************************************************************************
 * ZigBeeGetKey
 ********************************************************************************
 *
 * Return Value :
 *   SUCCESS if the key was retrieved successfully.or g_No_Key_c
 *   if there is no valid entry is in table, g_FAILURE_c if an attempt
 *   was made to retrieve an g_Trust_Center_Master_Key_c or
 *   g_Application_Master_Key_c etc
 *
 * Parameters   :
 *
 * Input Parameters:
 *   keytype - The Type of key to get (e.g. Link or Network key).
 *   keyStruct - A pointer to the ZigBeeKeyStructure_t data structure that will
 *               be populated with the persistent information
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   Gets the specified key and its associated data. This can retrieve
 *   the Link Key, Current Network Key, or Next Network Key.
 *
 * Before the function is called:
 *
 * After the function is called:
 *
 * NONE.
 *******************************************************************************/
uint8_t ZigBeeGetKey(Security_Key_Types keytype,  ZigBeeKeyStructure_t * keyStruct);
/*******************************************************************************
 * ZigBeeFindKeyTableEntry
 ********************************************************************************
 * Return Value :
 *   The index that matches the search criteria, or 0xFF if no matching entry
 *   was found.
 *
 * Parameters   :
 *
 * Input Parameters:
 *   pIEEEAddress - The IEEE Address of the partner device that shares the key.
 *       To find the first empty entry pass in an address of all zeros.
 *   linkKey - A boolean indicating whether to search for an entry containing
 *       a Link or Master Key.
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application to find the Key table entry.
 *
 * Purpose of the function:
 *   A function to search the key table and find an entry matching the specified
 *   IEEE address and key type
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeFindKeyTableEntry(uint8_t * pIEEEAddress, bool linkKey);

/*******************************************************************************
 * ZigBeeEraseKeyTableEntry
 ********************************************************************************
 * Return Value :
 *   SUCCESS if the index is valid and the key data was erased.
 *   m_NO_ENTRY_c if the index is out of range for the size of the key table
 *
 * Parameters   :
 *
 * Input Parameters:
 *   index The index in the key table of the entry to erase
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application to EraseKey table entry
 *
 * Purpose of the function:
 *   A function to clear a single entry in the key table
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeEraseKeyTableEntry(uint8_t index);

/*******************************************************************************
 * ZigBeeGetKeyTableEntry
 ********************************************************************************
 * Return Value :
 *   SUCCESS if the index is valid
 *   g_No_Key_c if the index is invalid
 *
 * Parameters   :
 *
 * Input Parameters:
 *   index - The index in the key table of the entry to get.
 *   pResult - A pointer to the location of an ZigBeeKeyStructure_t that will
 *           contain the results retrieved by the stack
 *
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application to get key table entry
 *
 * Purpose of the function:
 *   A function used to obtain data from the Key Table.
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeGetKeyTableEntry(uint8_t index, ZigBeeKeyStructure_t *pResult);



/*******************************************************************************
 * ZigBeeAddOrUpdateKeyTableEntry
 ********************************************************************************
 * Return Value :
 *   SUCCESS or FAILURE.
 *
 * Parameters   :
 *
 * Input Parameters:
 *   pIEEEAddress - The IEEE Address of the partner device that shares the key.
 *   linkKey - A boolean indicating whether this is a Link or Master key.
 *   pKeyData - A pointer to the actual key data.
 *   p_index - An output pointer where the key index of the corresponding link
 *				key table entry is updated. If 0xFF indicates invalid entry or
 *				no entry
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application to set key table entry
 *
 * Purpose of the function:
 *   This function add a new entry in the key table or updates an existing entry
 *   with a new key. It first searches the key table for an entry that has a
 *   matching IEEE Address. If it does not find one it searches for the first
 *   free entry. If it is successful in either case, it sets the entry with the
 *   IEEE address, key data, and flag that indicates if it is a Link or
 *   Master Key. The Incoming Frame Counter for that key is also reset to 0.
 *   If no existing entry was found, and there was not a free entry in the table,
 *   then the call will fail.
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/
uint8_t ZigBeeAddOrUpdateKeyTableEntry(uint8_t *pIEEEAddress, bool linkKey,
        uint8_t *pKeyData, uint8_t *p_index);

/*******************************************************************************
 * ZigBeeGetDeviceType
 ********************************************************************************
 *
 * Return Value :
 *   g_SUCCESS_c in case of successful execution of the request or g_FAILURE_c
 * incase of unknown device type.
 *
 * Parameters   :
 *
 * Input Parameters:
 *       NONE
 *
 * Output Parameters:
 *       pDeviceType pointer to which the current device type shall be copied.
 *
 *
 * Scope of the function:
 *       This function can be called from application to get Device type.
 *
 * Purpose of the function:
 *       This function allows the Application to get the current device type.
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeGetDeviceType(ZigBeeDeviceType *pDeviceType);

/*******************************************************************************
 * ZigBeeSetOperatingChannel
 ********************************************************************************
 *
 * Return Value :
 *   returns SUCCESS if the command is successful else failure
 *
 * Parameters   :
 *
 * Input Parameters:
 *       channel indicates the operating channel to be set
 *
 * Output Parameters:
 *       None
 *
 *
 * Scope of the function:
 *       This function can be called from application to set the current channel.
 *
 * Purpose of the function:
 *       This function allows the Application to set the current channel
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeSetOperatingChannel(uint8_t channel);



/*******************************************************************************
 * ZigBeeSetExtendedPANId
 ********************************************************************************
 *
 * Return Value :
 *   returns SUCCESS if the command is successful else failure
 *
 * Parameters   :
 *
 * Input Parameters:
 *       pExtPanId indicates the extended pan id to be set
 *
 * Output Parameters:
 *       None
 *
 *
 * Scope of the function:
 *       This function can be called from application to set the current extended
 *        PAN id
 *
 * Purpose of the function:
 *       This function allows the Application to set the current extended pan id
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeSetExtendedPANId(uint8_t *pExtPanId);

/*******************************************************************************
 * ZigBeeSetTrustCenterAddress
 ********************************************************************************
 *
 * Return Value :
 *   returns SUCCESS if the command is successful else failure
 *
 * Parameters   :
 *
 * Input Parameters:
 *       pTCAddress Trust Center address
 *
 * Output Parameters:
 *       None
 *
 *
 * Scope of the function:
 *       This function can be called from application to set the current Trust center
 *        address
 *
 * Purpose of the function:
 *       This function allows the Application to set the current trust center address
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeSetTrustCenterAddress(uint8_t *pTCAddress);
/******************************************************************************
 * ZigBeeGetChildShortAddressForTheIndex
 ******************************************************************************
 * Return Value : returns the child address if successful else returns
 *                INVALID_NODE_ID
 *
 * Parameters   :
 *
 * Input Parameters:
 *   childIndex indicates the index from where the 16-bit short address needs
 *   to be retrieved
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *   This function allows the Application to read the 16-bit short address of
 *   the child in the specified index
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint16_t ZigBeeGetChildShortAddressForTheIndex(uint8_t childIndex);

/******************************************************************************
 * ZigBeeGetChildIndexForSpecifiedShortAddr
 ******************************************************************************
 * Return Value : returns the index of the child address received in the input
 *                parameter else returns 0xFF which indicates INVALID
 *
 * Parameters   :
 *
 * Input Parameters:
 *   childShortAddrindicates the 16-bit short address whose index need to be
 *   determined
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *   This function allows the Application to get the index for the specified
 *   16-bit child address
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t ZigBeeGetChildIndexForSpecifiedShortAddr(uint16_t childShortAddr);

/******************************************************************************
 * ZigBeeGetChildDetails
 ******************************************************************************
 * Return Value :returns the SUCCESS if the index is valid and returns
 *   ZigBeeUnknownDevice if index is invalid
 *
 * Parameters   :
 *
 * Input Parameters:
 *   index indicates the index from where the child details are retrieved
 *
 * Output Parameters:
 *   pChildIEEEAddr gives the pointer to location where valid child IEEE address
 *   at the specified index should be copied
 *   pDevicetypeOfChild gives the pointer to location where Devicetype of child
 *   is copied if the index is valid None
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *   This function allows the Application to get the child details at the
 *   specified child index
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t ZigBeeGetChildDetails(uint8_t index, uint8_t * pChildIEEEAddr,
        ZigBeeDeviceType * pDevicetypeOfChild);
/******************************************************************************
 * ZigBeeGetParentIEEEAddress
 ******************************************************************************
 * Return Value : None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   pParentIEEE pointer to location where parent's 64-bit IEEE address should
 *    be copied
 *
 * Scope of the function:
 *   This function is used by Application Layer
 *
 * Purpose of the function:
 *  This function allows the Application to read it parent's 64-bit IEEE address
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void ZigBeeGetParentIEEEAddress(uint8_t *pParentIEEE);

/*******************************************************************************
 * ZigBeeNetworkState
 ********************************************************************************
 *
 * Return Value :
 *       ZigBeeJoinStatus indicates the join status of the device
 *
 * Parameters   :
 *
 * Input Parameters:
 *       None
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
 *       This function allows the Application to know if the device is in the process
 *       of Joining, or already Joined or leaving the network
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
ZigBeeJoinStatus ZigBeeNetworkState(void);

/*******************************************************************************
 * ZigBeeIsItSelfIEEEAddress
 ********************************************************************************
 *
 * Return Value :
 *       TRUE if it is self IEEE address FALSE if it is not matching with the
 *       self IEEE address
 *
 * Parameters   :
 *
 * Input Parameters:
 *       pointer to the IEEE address
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
 *       This function allows the Application to compare the specified IEEE
 *       address with the self IEEE address. This function is implemented by
 *       stack and is invoked by Application
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
bool ZigBeeIsItSelfIEEEAddress(uint8_t *pIEEEAddress);

/*******************************************************************************
 * ZigBeeGetShortAddrForSpecifiedIEEEAddr
 ********************************************************************************
 *
 * Return Value :
 *      returns the 16-bit short address of the corresponding 64-bit IEEE address
 *      if the address is known else returns INVALID_SHORT_ADDRESS
 *
 * Parameters   :
 *
 * Input Parameters:
 *      pIEEEAddress pointer to IEEE address whose 16-bit short address is to be
 *      determined
 *
 * Output Parameters:
 *
 *      NONE.
 *
 * Scope of the function:
 *      This function can be called from application for 16 bit short address
 *
 * Purpose of the function:
 *     This function allows the Application to get the 16-bit short address of
 *     the device for the given 64-bit IEEE address
 *
 * Before the function is called:
 *
 *    The child address shall be INVALID_SHORT_ADDRESS if the IEEE address is
 *    unknown
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint16_t ZigBeeGetShortAddrForSpecifiedIEEEAddr(uint8_t * pIEEEAddress);

/*******************************************************************************
 * ZigBeeGetIEEEAddrForSpecifiedShortAddr
 ********************************************************************************
 *
 * Return Value :
 *      returns SUCCESS or FAILURE
 *
 * Parameters   :
 *
 * Input Parameters:
 *     shortAddr gives the 16-bit short address of which the corresponding
 *     64-bit IEEE address need to be determined
 *
 * Output Parameters:
 *
 *     pIEEEAddress pointer location to which the IEEE address need to be copied
 *
 * Scope of the function:
 *     This function can be called from application for 16 bit short address
 *
 * Purpose of the function:
 *     This function allows the Application to get the 64-bit IEEE address of
 *     the device for the given 16-bit Short address
 *
 * Before the function is called:
 *
 *     NONE.
 * After the function is called:
 *
 *     NONE.
 *******************************************************************************/
uint8_t ZigBeeGetIEEEAddrForSpecifiedShortAddr(uint16_t shortAddr,
        uint8_t * pIEEEAddr);

/*******************************************************************************
 * ZigBeeFormNetwork
 ********************************************************************************
 *
 * Return Value :
 *	uint8_t status - Status indicates if the function call is valid or
 *	invalid. If valid the return value shall be SUCCESS else the returns
 *	appropriate error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *	RadioChannel The channel on which the Network is to be established
 *	pExtendedPanId Extended PAN Id using which the network shall be established.
 *
 * Output Parameters:
 *	None
 *
 * Scope of the function:
 *	This function can be called from application to form Network
 *
 * Purpose of the function:
 *	This function allows the Application establish the Network in the specified
 *	channel with the specified Extended PAN Id. The formation procedure is an
 *	asynchronous call. The stack shall call AppZigBeeStackStatusHandler to
 *	indicate the status of formation to the Application.
 *
 * Before the function is called:
 * 	NONE
 * After the function is called:
 *	 NONE.
 *******************************************************************************/

uint8_t ZigBeeFormNetwork ( uint8_t  RadioChannel, uint8_t power ,uint8_t * pExtendedPanId );

/*******************************************************************************
 * ZigBeePermitJoin
 ********************************************************************************
 *
 * Return Value :
 *   status Indicates the status of enabling permit Join. It returns SUCCESS if
 *   the device is ZIGBEE_COORDINATOR or ZIGBEE_ROUTER and returns ERROR if the
 *   device is ZIGBEE_ENDDEVICE
 *
 * Parameters   :
 *
 * Input Parameters:
 *   PermitDuration Indicates the duration in seconds for which the device shall
 *   allow join permission to the joining devices
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application.

 *
 * Purpose of the function:
 *   This function allows the Application enable join permit on the device
 *   for the specified duration in seconds.
 *
 * Before the function is called:
 *
 * After the function is called:
 *   NONE.
 *******************************************************************************/

uint8_t ZigBeePermitJoin(uint8_t PermitDuration);

/*******************************************************************************
 * ZigBeeJoinNetwork
 ********************************************************************************
 *
 * Return Value :
 *   uint8_t status - Status indicates if the function call is valid or invalid.
 *   If valid the return value shall be SUCCESS else the returns appropriate
 *   error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *   DeviceType The type of the device namely ZIGBEE_ROUTER, ZIGBEE_ENDDEVICE
 *   RadioChannel The channel on which the Network is to be established
 *   pExtendedPanId Extended PAN Id using which the network shall be established.
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application to send device announcement

 *
 * Purpose of the function:
 *  This function allows the Application to join the Network in the specified
 *  channel with the specified Extended PAN Id. The Join procedure is an
 *  asynchronous call. The stack shall call AppZigBeeStackStatusHandler to
 *  indicate the status of device joining the network to the Application.
 *
 *
 * Before the function is called:
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/

uint8_t ZigBeeJoinNetwork(uint8_t DeviceType, uint8_t RadioChannel,
       uint8_t power ,uint8_t * pExtendedPanId);
/*******************************************************************************
 * ZigBeeNetworkRestore
 ********************************************************************************
 *
 * Return Value :
 *   status indicates if the network information is restored from Non-Volatile
 *   Memory successfully or not. It returns appropriate error code
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
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application to retrieve its network information
 *   if it was already part of the network
 *
 *
 * Before the function is called:
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
uint8_t ZigBeeNetworkRestore(void);

/*******************************************************************************
 * ZigBeeEndDevicePollForData
 ********************************************************************************
 *
 * Return Value :
 *   returns the SUCCESS if the device is end device and is part of the network
 *   otherwise appropriate error code
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the End device application to poll the parent for data
 *
 * Before the function is called:
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
#if ( g_ZIGBEE_END_DEVICE_d == 1 )
uint8_t ZigBeeEndDevicePollForData( void );
#endif

/*******************************************************************************
 * ZigBeeStackIsPerformingRejoin
 ********************************************************************************
 *
 * Return Value :
 *   returns the TRUE or FALSE
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application to know if the stack is currently
 *   performing rejoin or not
 *
 * Before the function is called:
 *
 * After the function is called:
 *
 * NONE.
 *******************************************************************************/
bool ZigBeeStackIsPerformingRejoin();

/*******************************************************************************
 * ZigBeeInitiateScan
 ********************************************************************************
 *
 * Return Value :
 *   status returns SUCCESS if the scan begun successful.
 *
 * Parameters   :
 *
 * Input Parameters:
 *   scanType -indicates what type of scan need to be performed. ENERGY_SCAN,
 *   ACTIVE_SCAN
 *   channelMask -indicates the channel mask in which scan need to be done
 *   duration indicates the scan duration in seconds
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application to initiate Scan of specified type in
 *   the specified channel mask for the specified duration.
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   NONE.
 *******************************************************************************/
uint8_t ZigBeeInitiateScan(uint8_t scanType, uint32_t channelMask,
        uint8_t duration);
/*******************************************************************************
 * ZigBeeStopScan
 ********************************************************************************
 *
 * Return Value :
 *   status returns SUCCESS if the scan is stopped successfully
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
 *   This function can be called from application
 *
 * Purpose of the function:
 *   This function allows the Application to stop the scan that was initiated
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   NONE.
 *******************************************************************************/
uint8_t ZigBeeStopScan(void);
/******************************************************************************
 * ZigBeeLeaveNetwork
 ******************************************************************************
 *
 * Return Value :
 *   status indicates if the device could parse the command. If yes returns
 *   SUCCESS or FAILURE
 * Parameters   :
 *
 * Input Parameters:
 *   p_nwk_packet_info : -pointer to structure of type Generic_nwk_Packet_t
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is called from Application
 *
 * Purpose of the function:
 *   This function will perform self leave from the network
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t ZigBeeLeaveNetwork(void);

/******************************************************************************
 * ZigBeeSwitchNetworkKeyHandler
 ******************************************************************************
 *
 * Return Value :
 *		TRUE or FALSE
 *
 * Parameters   :
 *
 * Input Parameters:
 *	sequenceNumber: active key sequence number
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *	This is an handler function which ll be used in application
 * Purpose of the function:
 *	This Function is called whenever devices changes to new key .
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
bool ZigBeeSwitchNetworkKeyHandler(uint8_t sequenceNumber);

/******************************************************************************
 * AppZigBeeStackStatusHandler
 ******************************************************************************
 *
 * Return Value :
 *		NONE
 *
 * Parameters   :
 *
 * Input Parameters:
 *	NetworkStatus  NetworkStatus indicates the Network status
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *	This is an handler function which will be used in application
 * Purpose of the function:
 *	This function is invoked by the ZigBee Stack to indicate any kind of Network
 *	status. For example: upon establishing the network, this function shall be
 *	called by the stack to inidcate status ZigBeeNetworkIsUp. If the device
 *	leaves the network, a status of ZigBeeNWkisDown status is indicated via this
 *	function call.

 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void AppZigBeeStackStatusHandler(ZigBeeNWKStatusInfo *NetworkStatus);

/******************************************************************************
 * AppScanCompleteHandler
 ******************************************************************************
 *
 * Return Value :
 *		NONE
 *
 * Parameters   :
 *
 * Input Parameters:
 *	channel   indicates in which the Energy scan was performed.
 *	status  : MAC_STATUS_VALUES
 *                  1.g_MAC_Success_c
 *                  2.g_MAC_Failed_Security_Check_c
 *                  3.g_MAC_No_Beacon_c
 *                  4.g_MAC_Scan_In_Progress_c
 *                  5.g_MAC_Invalid_Parameter_c
 *                  6.g_MAC_Frame_Too_Long_c
 *                  7.g_MAC_Unavailable_Key_c  etc..

 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *	This is called by the stack
 *
 * Purpose of the function:
 * This function is invoked by the stack after performing the scan in
 * the specified channel with the status of the scan performed.
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void AppScanCompleteHandler(uint32_t channel, uint8_t status);

/******************************************************************************
 * AppEnergyScanResultHandler
 ******************************************************************************
 *
 * Return Value :
 *		NONE
 *
 * Parameters   :
 *
 * Input Parameters:
 *	channel   indicates in which the Energy scan was performed.
 *	maxRSSIValue  indicates the signal strength in the specified channel
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *	This is called by the stack
 * Purpose of the function:
 *	 This function is invoked by the stack after performing the Energy scan in
 *	the specified channel. The maxRSSIValue indicates the RSSI value in the
 *	scanned channel


 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void AppEnergyScanResultHandler(uint32_t channel, uint8_t *maxRSSIValue);

/******************************************************************************
 * AppChildJoinHandler
 ******************************************************************************
 *
 * Return Value :
 *		NONE
 *
 * Parameters   :
 *
 * Input Parameters:
 *	channel   The index of the child of interest.
 *	maxRSSIValue  True if the child is joining, false if the child is leaving.
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *	This is called by the stack
 * Purpose of the function:
 *	 This is called by the stack when a child joins or leaves. 'Joining' is TRUE
 *	if the child is joining and FALSE if leaving.
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void AppChildJoinHandler(uint16_t shortAddress, bool joining);


/******************************************************************************
 * ZigBeeBroadcastNextNetworkKey
 ******************************************************************************
 *
 * Return Value :
 *	    Return value is SUCCESS or FAILURE
 *
 * Parameters   :
 *
 * Input Parameters:
 *	    uint8_t * pKeyData : Its an pointer to array of 16 bytes of next network
 *          key data
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *	This is called by the Application ,This function can be called by only
 *  Trust Centre (Coordinator )
 * Purpose of the function:
 *	This function is used to broadcast next network key
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/

uint8_t ZigBeeBroadcastNextNetworkKey (uint8_t * pKeyData);

/******************************************************************************
 * ZigBeeBroadcastNetworkKeySwitch
 ******************************************************************************
 *
 * Return Value :
 *	    None
 *
 * Parameters   :
 *
 * Input Parameters:
 *      None
 * Output Parameters:
 *      None
 *
 * Scope of the function:
 *	This is called by the Application ,This function can be called by only
 *  Trust Centre (Coordinator )
 * Purpose of the function:
 *	This function is used to broadcast switch key command
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/

void ZigBeeBroadcastNetworkKeySwitch(void);
/******************************************************************************
 * ZigBeeScanAndJoinNetwork
 ******************************************************************************
 *
 * Return Value :
 *	    None
 *
 * Parameters   :
 *
 * Input Parameters:
 *      deviceType : can be router or end device
 *      power
 * Output Parameters:
 *      None
 *
 * Scope of the function:
 *	This is called by the Application
 * Purpose of the function:
 *	This function is used to scan for all channels and join suitable network
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/


uint8_t ZigBeeScanAndJoinNetwork ( uint8_t  DeviceType, uint8_t power );
/******************************************************************************
 * ZigBeeDeviceDepth
 ******************************************************************************
 *
 * Return Value :
 *	    None
 *
 * Parameters   :
 *
 * Input Parameters:
 *      pointer to the device address
 *      addressing mode
 * Output Parameters:
 *      depth of the device
 *
 * Scope of the function:
 *	This is called by the Application
 * Purpose of the function:
 *	Used to get the depth of the device in the network, from the route table
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/


uint8_t ZigBeeDeviceDepth (Address *pAddress, uint8_t addressMode);



uint8_t GetTrustCenterLinkKey (TrustCenterLinkKeyTable_t *LinkKeyTable,
							   uint8_t index);
uint8_t SetTrustCenterLinkKey (TrustCenterLinkKeyTable_t LinkKeyTable,
								 uint8_t index);

uint32_t ZigBeeGetStackVersion(void);

/******************************************************************************
 * ZigBeeGetAddressMapTableEntry
 ******************************************************************************
 *
 * Return Value :
 *	    returns (APSME_Address_Map_Table_t )shortAddress and IEEE Address in
 *      the specified index;
 *
 * Parameters   :
 *
 * Input Parameters:
*      index: this value indicates which entry in the address map table
 * Output Parameters:
 *      none
 *
 * Scope of the function:
 *	This is called by the Application
 * Purpose of the function:
 *	Used to get the short address from Address Map Table.
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
APSME_Address_Map_Table_t ZigBeeGetAddressMapTableEntry(uint8_t index);

/******************************************************************************
 * ZigBeeSetAddressMapTableEntry
 ******************************************************************************
 *
 * Return Value :
 *	returns (APSME_Address_Map_Table_t )shortAddress and IEEE Address in
 *  the specified index;
 *
 * Parameters   :
 *
 * Input Parameters:
 *  index :This value indicates which entry in the address map table
 *  shortAddress : new shortAddress to be set in the Address map table entry
 *  pIEEEADDRESS : new IEEE address to
 * Output Parameters:
 *      none
 *
 * Scope of the function:
 *	This is called by the Application
 * Purpose of the function:
 *	Used to get the short address from Address Map Table.
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/

uint8_t ZigBeeSetAddressMapTableEntry(uint8_t index, uint16_t shortAddress,
                                      uint8_t* pIEEEaddress);

/******************************************************************************
 * ZigBeeSetAddressMapTableEntry
 ******************************************************************************
 *
 * Return Value :
 *
 * ZigBeeJoinStatus indicates TRUE if the device is formed or joined state and is
 	ready to perform data transmission. FALSE otherwise
 *
 * Parameters   :
 *
 * Input Parameters:
 *  	None
 * Output Parameters:
 *      none
 *
 * Scope of the function:
 *	This is called by the Application
 * Purpose of the function:
 *	Used to get the ZigBee Stack  is ready
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/

bool ZigBeeStackIsUp(void);



/******************************************************************************
 * InitStack
 ******************************************************************************
 *
 * Return Value :
 *
 *
 *
 * Parameters   :
 *
 * Input Parameters:
 *  	None
 * Output Parameters:
 *      none
 *
 * Scope of the function:
 *	This is called by the Application
 * Purpose of the function:
 *
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/

void InitStack(uint32_t stackFunctionality);
/******************************************************************************/
#if ( ( g_ZIGBEE_COORDINATOR_d == 1 ) || ( g_ZIGBEE_ROUTER_d == 1 ) )
Route_Record_Table_t* ZigBeeGetRouteRecordForDevice(uint16_t shortAddress);
#endif

void ZDO_handleSilentStartRequest(uint8_t* silentStartParam,
        uint8_t bufferIndex);

void setNetworkMaxDepth(uint8_t maxdepth);

void ZigBeeSetCurrentScanAndJoinChannel (uint8_t channel);
extern uint8_t ZigBeeConfigureChannelAndStartDevice (void);
void ZigBeeConfigureScanDuration(uint8_t scan_duration);
void ZigBeeSendPollConfirmationCB( uint8_t bufferIndex);
uint8_t ZigBeeGetCurrentScanAndJoinChannel (void);
void ZigBeePollCompleteHandler(ZigBeeStatus status);
#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
void app_APSUnauthorizedKeyHandler(void);
void app_ChildMovedHandler(void);
void app_No_OF_ResetsHandler(void);
void app_PersistentMemoryWritesHandler(void);
#endif /* g_DIAGNOSTICS_CLUSTER_ENABLE_d*/

/*******************************************************************************
 * End Of File
 *******************************************************************************/
#endif                                                      /*_ZDO_INTERFACE_H_*/
