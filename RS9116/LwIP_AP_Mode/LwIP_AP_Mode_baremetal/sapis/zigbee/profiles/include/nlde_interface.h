/*******************************************************************************
 * nlde_interface.h
 *******************************************************************************
 * Program Description:
 *     This file contains the interface functions and data structures to be used
 *     by the NHLE/APS.
 *
 *******************************************************************************/

/*******************************************************************************

 NLDE Request Primitives & Enumerations(Msg Id's)

 The NLDE primitives are declared as structures where each member is
 meant for a particular parameter within each primitive.

 The structure member names should match the names of the primitive
 parameters as given in the ZigBee specification. These structures
 may be used to have local instances for processing the primitive
 held by the instance.
 *******************************************************************************/

#ifndef NLDE_INTERFACE_H
#define NLDE_INTERFACE_H
/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "ZigBeeDataTypes.h"
/*******************************************************************************
 * Global Constants
 *******************************************************************************/

/*******************************************************************************
 This enumeration is used while scanning the message received or before
 sending it to NHLE/NLLE.
 *******************************************************************************/
enum NLDE_Primitive_id {
    g_NLDE_Data_Req_c = 0x70,
    g_NLDE_Data_Conf_c = 0x71,
    g_NLDE_Data_Ind_c = 0x72
};

/*******************************************************************************
 NWK_Data_Disc_Route_Options

 These enums are used by the application for specifying whether it wants to
 discover the route for the given data request.

 The different discovery options provided for the application are defined as
 follows :-

 *******************************************************************************/

enum NWK_Data_Disc_Route_Options {
    g_Suppress_Route_Discovery_c    = 0x00,
    g_Enable_Route_Discovery_c      = 0x01
};

/*******************************************************************************
 NWK_Data_Security_options

 These enums are used by the application for specifying the different
 security options that can be specified while issuing the data request.

 The different security options provided for the application are defined as
 follows :-
 *******************************************************************************/

enum NWK_Data_Security_options {
   g_Disable_Security_c     = 0x00,
   g_Enable_Security_c      = 0x01
};

enum {
    g_Dest_Addr_Mode_Multicast_Group_Address_c = 0x01,
    g_Dest_Addr_Mode_Short_Address_c = 0x02
};

/*******************************************************************************
 This MACRO is defined for specifying the number of bytes that should be
 reserved  for the lower layers to add their respective headers to the
 service data unit they recieve from the upper Layer.
 *******************************************************************************/

#define m_BYTES_RESERVED_FOR_HEADERS_c          0x30

/******************************************************************************
 * Type Definitions
 ******************************************************************************/

/******************************************************************************
 NLDE Request Primitives_t

 The NLDE request primitive is declared as struct with in the union
 NLDE_Request_Primitive.

 dst_Addr_Mode
 -The type of destination address supplied by the DstAddr parameter

 0x01  -  16-bit multicast group address

 0x02  -  16-bit NWK address of a device or a 16-bit broadcast
 address

 dst_Addr
 -The destination address supplied by the DstAddr parameter.

 nsdu_length
 -The number of octets comprising the NSDU to be transferred.
 Contains the Nwk layer payload length.

 nsdu_handle
 -The handle associated with the NSDU to be transmitted by the NLDE.

 radius
 -The distance, in hops, that a frame will be allowed to travel
 through the network.

 non_member_radius
 -The distance, in hops, that a multicast frame will be relayed by
 nodes not a member of the group.A value of 0x07 is treated as
 infinity.

 discover_route

 0x00 -   suppress route discovery

 0x01 -   enable route discovery

 security_enable

 TRUE  -   security will be applied

 FALSE -   security will not be applied


 a_reserved[ m_BYTES_RESERVED_FOR_HEADERS_c ]
 -The set of octets reserved for the lower layers to put their
 respective headers.

 a_nsdu[1]
 -The set of octets comprising the NSDU to be transferred .

 *******************************************************************************/

typedef struct NLDE_Data_Request_Tag {
    uint8_t dst_Addr_Mode;
    uint16_t dst_addr;
    uint8_t nsdu_length;
    signed char nsdu_handle;
    uint8_t radius;
    uint8_t non_member_radius;
    uint8_t discover_route;
    uint8_t security_enable;
    uint8_t a_reserved[m_BYTES_RESERVED_FOR_HEADERS_c];
    uint8_t a_nsdu[1];
} __attribute__((__packed__))NLDE_Data_Request_t;

/*******************************************************************************
 NLDE_Data_Confirmation_t

 The NLDE Data Confirm primitive is declared as a structure with in
 the union NLDE_Response_Primitive. This primitive reports the
 results of a request to transfer a data PDU (NSDU) from a local
 NHLE to a single peer NHLE.

 nsduhandle
 -Handle of the data that was sent by the appln(APS). Handle 0x80 to
 0xFF will be used for the frames generated internally or routed
 through the nwk layer.

 Status
 -Status of the data that was sent.

 *******************************************************************************/

typedef struct NLDE_Data_Confirmation_Tag {
    uint8_t nsduhandle;
    uint8_t status;
} __attribute__((__packed__))NLDE_Data_Confirmation_t;

/*******************************************************************************
 NLDE_Data_Indication_t

 The NLDE Indication primitive is declared as struct with in the
 union NLDE_Response_Primitive.This primitive indicates the transfer
 of a data PDU (NSDU) from the NWK layer to the local NHLE.

 dst_Addr_Mode
 -The type of destination address supplied by the DstAddr parameter.

 0x01  - 16-bit multicast group address

 0x02  - 16-bit NWK address of a device or a 16-bit broadcast
 address

 dst_addr
 -Destination address.

 src_addr
 -Source address of the originating device.

 nsdu_length
 -The number of octets comprising the NSDU to be transferred.
 contains the Nwk layer payload length.

 link_quality
 -The link quality indication delivered by the MAC.
 security_use
 - An indication of whether the received data frame is using
 security. This value is set to TRUE if security was applied to the
 received frame or FALSE if the received frame was unsecured.
 *p_nsdu
 -The set of octets comprising the NSDU.


 *******************************************************************************/

typedef struct NLDE_Data_Indication_Tag {
    uint8_t dst_Addr_Mode;
    uint16_t dst_addr;
    uint16_t src_addr;
    uint8_t nsdu_length;
    uint8_t link_quality;
    uint8_t security_use;
    uint8_t* p_nsdu;
} __attribute__((__packed__)) NLDE_Data_Indication_t;

/*******************************************************************************
 NLDE_Request_t

 This structure is used to send a data from NHLE to NWK Layer.The
 NLDE_Request_t structure is defined as follows:-

 The NLDE Request primitive is declared as union with in the
 union NLDE_Request_Primitive.

 primitive
 -NLDE-Data.Request primitive ID.

 NLDE_Request_Primitive
 -Data Request from NHLE to NWK.

 *******************************************************************************/
typedef struct NLDE_Request_Tag {
    uint8_t primitive;
    union NLDE_Request_Primitive {
        NLDE_Data_Request_t NLDE_Data_Req;
    } NLDE_Request_Primitive;
} __attribute__((__packed__))NLDE_Request_t;

/*******************************************************************************
 NLDE_Response_t

 This structure is used to send a data indication and confirmation
 from NWK to NHLE.

 primitive
 -NLDE-Data.confirm message ID /NLDE-Data.indication message ID.

 NLDE_Response_Primitive
 -Conf/Ind from NWK to NHLE .

 *******************************************************************************/

typedef struct NLDE_Response_Tag {
    uint8_t primitive;
    union NLDE_Response_Primitive {
        NLDE_Data_Confirmation_t NLDE_data_conf;
        NLDE_Data_Indication_t NLDE_Data_Ind;
    } NLDE_Response_Primitive;
} __attribute__((__packed__))NLDE_Response_t;

/*******************************************************************************
 Broadcast_Transaction_Table_t

 This structure holds all the information required by the broadcast
 module.Broadcast Transaction Table is updated every time Broadcast
 is initiated by the local device or by a remote device.

 src_address
 -Stores the short address of the device which initiated the
 broadcast.

 broadcast_seq_number
 -Holds a unique sequence number for each broadcast.

 end_device_index
 -Index to neighbor table to start searching for a child end device.
 Initially the value of this entry shall be zero. If a particular
 broadcast does not need unicasting,then it will be set to 0xff.

 broadcast_delivery_timecount
 -Keeps track of the timer count since the initiation of the network
 broadcast.

 p_buffer
 -Pointer to the buffer which holds the broadcast packet which needs
 to be stored till the expiry of the broadcast delivery time. This
 is maintained for performing broadcast retries in case if the
 device does not get passive acks from the receiving devices and for
 performing unicast to sleeping ZED if required.

 passive_ack
 -Used to keep track of the information of all the devices from
 which passive ack has been received by the device. This is to keep
 track off how many neighbors(ZR/ZC) in network has heard the
 broadcast. It is a 4 byte bitmap.This variable is used to know
 which of the neighbor(ZR/ZC) have received the broadcast.

 num_of_retries
 -Number of times that a particular broadcast frame is expected to
 be retried, if passive ack is not received within
 nwkpassiveAckTime.

 *******************************************************************************/
typedef struct Broadcast_Transaction_Table_Tag {
    uint16_t src_address;
    uint8_t broadcast_seq_number;
    uint8_t end_device_index;
    uint8_t broadcast_delivery_timecount;
    uint8_t* p_buffer;
    uint32_t passive_ack;
    uint8_t num_of_retries;
} __attribute__((__packed__))Broadcast_Transaction_Table_t;

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/
/* None */

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/*******************************************************************************
 *
 * Input Parameters:
 *
 *  message_index
 *           -index to the buffer, that holds the data to be sent,from NHLE to
 *           NWK layer.
 *
 * Output Parameters:
 *           NONE.
 *
 * Return Value :
 *           0x00  -  SUCCESS
 *
 *           0xff  -  BUFFER_OVERFLOW
 *
 *
 * Scope of the function:
 *           This function is called by NHLE layer when it wants to pass
 *           NLDE-Data.Request Message to the underlying Network layer.
 *
 * Detailed Description:
 *           This function enables the NHLE to send NLDE-Data.Request message to
 *           the NWK Layer .This function returns SUCCESS(0x00) if the message
 *           is successfully posted to the NWK layer else returns
 *           BUFFER_OVERFLOW(0xFF) indicating that the network layer is
 *           receiving more than the specified number of NLDE-Data.Request
 *           messages simultaneously into its Queue.
 *
 * Before the function is called:
 *           The following steps should be followed to allocate a memory for the
 *           message sent using the buffer_index as the reference to the
 *           function NWK_NLDErequest():
 *
 *              Call the function allocate_buffer() to allocate a buffer and get
 *              the index. Eg: buffer_index = allocate_buffer().
 *
 *              Get the starting address of the buffer_index returned by the
 *              function allocate_buffer() by calling the function
 *              buffMgmt_getBufferPointer().
 *
 *              Eg: myPointerToTheAllocatedBuffer = buffMgmt_getBufferPointer(buffer_index)
 *
 *     NOTE:-myPointerToTheAllocatedBuffer is a pointer of type unsigned
 *           character.Using myPointerToTheAllocatedBuffer create NLDE.Request
 *           message to be sent to the Network layer.
 *
 *           Put the message pointed to by myPointerToTheAllocatedBuffer into
 *           the APS_NLDE message queue by calling the function
 *           NWK_NLDErequest().
 *
 * After the function is called:
 *           Upperlayer(APS) should relinquish its control so that other layers
 *           get a chance to process the request. APS should also wait for a
 *           confirmation for the issued request.
 *
 *
 * Error Handling:
 *           If the NWK_NLDErequest returns 0xff(BUFFER_OVERFLOW) then the NHLE
 *           should free the message allocated for Issuing NLDE-Data.request.
 *
 *******************************************************************************/

uint8_t NWK_NLDErequest(uint8_t message_index);

/*******************************************************************************
 *
 * Input Parameters:
 *
 *  message_index
 *           -index to the buffer holding the data to be sent from NWK to APS
 *           layer.
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
 *           This function gets the message_index of the NLDE confirmation and
 *           indication message sent by the Network layer to the NHLE layer.
 *
 * Before the function is called:
 *           NONE.
 *
 * After the function is called:
 *           If NWK_Handle_NLDE_Message() gets a message_index other than 0xF
 *           (BUFFER_EMPTY), Then NHLE has to perform the below steps to
 *           retrieve the message sent by the Network layer:-
 *
 *              Call buffMgmt_getBufferPointer() to get the pointer to the message
 *              referred to by the Message_Index passed as a parameter to
 *              Handle_NLDE_Message().
 *
 *              Process the Message received.
 *
 *              After the data in the message is consumed by NHLE. NHLE should
 *              free the buffer by calling the function buffMgmt_freeBuffer(Message_Index).
 *
 * Error Handling:
 *           NONE.
 *******************************************************************************/

void Handle_NLDE_Message(uint8_t message_index);

/*******************************************************************************
 * NWK_isParent
 *******************************************************************************
 *Input Parameters:
 *		address
 *			- Destination address
 *
 *Output Parameters:
 *		NONE.
 *
 * Return Value :
 *      NONE
 * Scope of the function:
 *
 * Detailed Description:
 *
 * Before the function is called:
 *		NONE.
 *
 * After the function is called:
 *		NONE
 * Error Handling:
 *       NONE.
 *******************************************************************************/
uint8_t NWK_isParent(uint16_t address);

/*******************************************************************************
 * NWK_NLDEForceRequest
 *******************************************************************************
 *Input Parameters:
 *		messageIndex
 *			- nlde request message index
 *
 *Output Parameters:
 *		SUCCESS.
 *
 * Return Value :
 *      SUCCESS
 * Scope of the function:
 *
 * Detailed Description:
 *
 * Before the function is called:
 *		NONE.
 *
 * After the function is called:
 *		NONE
 * Error Handling:
 *       NONE.
 *******************************************************************************/
uint8_t NWK_NLDEForceRequest(uint8_t messageIndex);

/*******************************************************************************
 * End Of File
 *******************************************************************************/

#endif                                 /*NLDE_INTERFACE_H */
