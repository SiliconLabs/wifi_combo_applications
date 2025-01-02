/******************************************************************************
 * apsde_interface.h
 ********************************************************************************
 * Program Description:
 *   This file consists of structures, enumerations for status values and API's
 *      for APSDE data transmission and reception.
 *
 *******************************************************************************/
#ifndef APSDE_INTERFACE_H
#define APSDE_INTERFACE_H

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "stdarg.h"
#include "ZigBeeDataTypes.h"
#include "zdp_interface.h"
#include"stack_common.h"
/*None */

/*******************************************************************************
 * Global Constants
 *******************************************************************************/


#define ZERO                                    0x00

/* This is max payload size allowed by the APS layer to transmit the data over
 the Air. This value could change if security is enabled. */
#define g_MAX_PAYLOAD_c                               0x69 // 0x5a
/* This is max payload size allowed by the APS layer to transmit the data over
 the Air with Security */
#define g_MAX_PAYLOAD_WITH_SECURITY_c                   (g_MAX_PAYLOAD_c - 0x0E)


#define CHECKBIT_ON(data,bitno) ((data>>bitno)&0x01)


#define MAX_PAY_LOAD_SIZE  					0x2F
#define MAX_BLOCKS_TRANSMISSION_WINDOW		0x08


#define DEF_FRAGMENT_DATA_PAYLOAD_SIZE		0x2F

#define DEFAULT_RADIUS						0x1e

/*enums for data confirmation/indication
 g_APSDE_Data_Confirmation_c
 - This status value indicates that the message received is
 confirmation.

 g_APSDE_Data_Indication_c
 - This status value indicates that the message received is
 indication.

 g_APSDE_Data_Request_c
 - this is indicates the application, the message is received for
 APSDE data request.
 */
enum APSDE_Primitive_Ids {
    g_APSDE_Data_Confirmation_c = 0xF6,
    g_APSDE_Data_Indication_c = 0xF7,
    g_APSDE_Data_Request_c = 0xF8
};

/*enums for different address modes used during data transmission

 g_Indirect_Address_Mode_c
 - indicates data transmission is done indirecty.

 g_Group_Address_Mode_c
 - indicates data transmission is done using group address table.

 g_Short_Address_Mode_c
 - indicates data transmission is done using 16-bit nwk short
 address.

 g_IEEE_Address_Mode_c
 - indicates data transmission is done using 64-bit Extended
 address.
 */
enum Destination_Address_Modes {
    g_Indirect_Address_Mode_c,
    g_Group_Address_Mode_c,
    g_Short_Address_Mode_c,
    g_IEEE_Address_Mode_c
};

/*enumerations for txoptions
 g_APS_Tx_Opt_Unsecured_c
 - data transmission with out security.

 g_APS_Tx_Opt_Ack_Req_c
 - data transmission without security and with APS Ack.

 g_APS_Tx_Opt_Use_NWK_Key_c
 - data transmission with security and with NWK Key.

 g_APS_Tx_Opt_Fragmentation_Permitted_c
 - data transmission with Fragmentation.

 g_APS_Tx_Opt_Secured_Nwk_Key_Ack_c
 - data transmission with security using NWK Key and with APS Ack.
 */
enum Tx_Options {
    g_APS_Tx_Opt_Unsecured_c = 0x00,
    g_APS_Tx_Opt_Ack_Req_c = 0x04,
    g_APS_Tx_Opt_Use_NWK_Key_c = 0x02,
    g_APS_Tx_Opt_Use_Link_c = 0x01,
    g_APS_Tx_Opt_Fragmentation_Permitted_c = 0x08,
    g_APS_Tx_Opt_Secured_Nwk_Key_Ack_c = 0x07
};

/*Enumerations for Security status of data indication
 g_APS_UNSECURED_c
 - when the ASDU is received without any security.defined in the
 above enumeration.

 g_APS_Secured_Nwk_Key_c
 - when the ASDU is received with security using NWK Key.

 g_APS_Secured_Link_Key_c
 - when the ASDU is received with security using link Key.
 */
enum APSDE_Data_Indication_Status {
    g_Aps_Secured_Link_Key_c = 0xab,
    g_Aps_Secured_Nwk_Key_c = 0xac,
    g_Aps_Unsecured_c = 0xaf
};

/* Enumerations for the destination address mode values indicated in the  APSDE
 Data indication*/
enum APSDE_Data_Indication_Dest_Address_Modes {
    g_APSDE_Ind_Dest_Addr_Mode_Group_c = 0x01,
    g_APSDE_Ind_Dest_Addr_Mode_Short_c = 0x02
};

/* Enumerations for the source address mode values indicated in the  APSDE
 Data indication */
enum Source_Address_Modes {
    g_APSDE_Ind_Src_Addr_Mode_Reserved_c = 0x01,
    g_APSDE_Ind_Src_Addr_Mode_Short_Addr_EP_Present_c = 0x02,
    g_APSDE_Ind_Src_Addr_Mode_Long_Addr_EP_Present_c = 0x03
};

/* Following is the enumeration for the different status values that can be
 recieved from APSDE */

enum APSDE_Confirmation_Status {
    g_Aps_Success_c = 0x00,
    g_Aps_Illegal_Request_c = 0xa3,
    g_Aps_Invalid_Parameter_c = 0xa6,
    g_Aps_No_Ack_c = 0xa7,
    g_Aps_No_Bound_Device_c = 0xa8,
    g_Aps_No_Short_Address_c = 0xa9,
    g_Aps_Not_Supported_c = 0xaa,
    g_Aps_Security_Fail_c = 0xad,
    g_Aps_Transaction_Overflow_c = 0xb1,
    g_Aps_No_Buffer_For_Retries_c = 0xb2
};


typedef enum
{
    ZigBee_Outgoing_Direct,
    ZigBee_Via_Address_Map,
    ZigBee_Via_Binding_Table,
    ZigBee_Via_Multicast,
    ZigBee_Broadcast
}ZigBee_Outgoing_Msg_Type;

typedef enum
{
  g_FragmentReqSuccess_c = 0x00,
  g_FragmentNoPermit_c = 0x01,
  g_FragmentMessageTooLog = 0x02
} FragmentReqStatus;
/*******************************************************************************
 * Type Definitions
 *******************************************************************************/


/*******************************************************************************
 APSDE_Data_Request

 The data request frame is passed to APS layer for further processing.

 dest_address
 - This field holds the individual device address or group address
 of the destination device.

 dest_addr_mode
 -This field specifies the destination address mode. This field can
 take one of the following values:

 0x00       - Indirect data transmission (destination address and
 destination endpoint are not present)

 0x01       - 16-bit group address.

 0x02       - 16-bit address of destination device.

 0x03       - 64-bit extended address of destination device.

 0x04-0xff  - Reserved


 dest_endpoint
 -This field indicates the destination endpoint for the message.

 src_endpoint
 -This field indicates the source endpoint from which the message
 originates.

 profile_id
 - This field indicates  the 16-bit profile ID.

 cluster_id
 - This field indicates the cluster ID.

 asdulength
 - This field indicates the length of the message.

 txoptions
 - This field indicates the message transmission options. The values
 are as follows:

 0x00 - Unsecured message

 0x02 - Secure the message using Network Key

 0x04 - APS Acknowledgement required

 0x07 - Secured with Network Key and APS acknowledgement required

 0x08 - Fragmentation permitted

 radius
 - This field indicates the distance in hops, that a transmitted
 frame is allowed to travel through the network.

 a_reserved[0x31]
 - This field is reserved for use  by stack. Any data written on to
 this field will be ignored.

 asdu[1]
 - This field indicates the actual message to be sent.User needs to
 copy the actual message that needs to be sent out, to this field.

 *******************************************************************************/
typedef struct APSDE_Data_Request_Tag {
    Address dest_address;
    uint8_t dest_addr_mode;
    uint8_t dest_endpoint;
    uint8_t src_endpoint;
    profile_id_t profile_id;
    cluster_id_t cluster_id;
    uint8_t asdulength;
    uint8_t txoptions;
    uint8_t radius;
    uint8_t a_reserved[0x31];
    uint8_t asdu[1];
}__attribute__((__packed__)) APSDE_Data_Request_t;
/*******************************************************************************
APSDE_Data_Request

    The data request frame is passed to APS layer for further processing.

   SrcAddrMode
            - This field holds the individual device address or group address
            of the destination device.

   DstAddrMode
            -This field specifies the destination address mode. This field can
            take one of the following values:

             0x00       - Indirect data transmission (destination address and
                           destination endpoint are not present)

             0x01       - 16-bit group address.

             0x02       - 16-bit address of destination device.

             0x03       - 64-bit extended address of destination device.

             0x04-0xff  - Reserved


   DstPANId
            -This field indicates the destination endpoint for the message.

   DstAddress
            -This field indicates the source endpoint from which the message
            originates.

   ProfileId
            - This field indicates  the 16-bit profile ID.

   cluster_id
            - This field indicates the cluster ID.

   asdulength
            - This field indicates the length of the message.

   txoptions
            - This field indicates the message transmission options. The values
            are as follows:

             0x00 - Unsecured message

             0x02 - Secure the message using Network Key

             0x04 - APS Acknowledgement required

             0x07 - Secured with Network Key and APS acknowledgement required

             0x08 - Fragmentation permitted

   radius
            - This field indicates the distance in hops, that a transmitted
            frame is allowed to travel through the network.



   asdu[1]
            - This field indicates the actual message to be sent.User needs to
            copy the actual message that needs to be sent out, to this field.

*******************************************************************************/
typedef struct APSDE_InterPan_Data_Request_Tag {
    uint8_t         srcaddrmode;
    uint8_t         dstaddrmode;
    uint16_t        dstpanid;
    Address         dstaddress;
    profile_id_t    profileid;
    cluster_id_t    clusterid;
    uint8_t         asdulength;
    uint8_t         asduhandle;
    uint8_t         a_asdu[1];
}__attribute__((__packed__)) APSDE_InterPan_Data_Request_t;

/*******************************************************************************
 APSDE_Request_Tag

 The application invokes APS_APSDErequest() function to send the
 data frame.

 The input parameter of this function is the index to the buffer
 pointer that holds the actual data for transmission. The
 application should allocate the buffer, fill up the required
 information and pass it on to the stack. The layer receiving the
 buffer will be responsible for freeing up the buffer.

 NOTE : Application need to assert if, the return value of
 APS_APSDErequest function is equal to g_Success_c (0x00).
 If the value returned is g_Queue_full_c (0xFF), then the application
 is responsible for freeing the message index.

 *******************************************************************************/
typedef struct APSDE_Request_Tag {
    uint8_t msgType;
    union APSDE_Request_Primitive {
        APSDE_Data_Request_t    apsde_data_request;
        APSDE_InterPan_Data_Request_t apsde_interpan_data_request;
    }APSDE_Request_Primitive;
}__attribute__((__packed__)) APSDE_Request_t;

/*******************************************************************************
 APSDE_Data_Confirmation_Tag

 The data confirmation structure is received from APS layer(APS).

 dest_address
 - This field indicates the individual device address or group
 address of the transmitted message.

 dest_addr_mode
 -This field indicates the destination address mode. This field
 takes one of the following values:

 0x00          - Indirect data transmission (destination address
 and destination endpoint are not present)

 0x01          - 16-bit group address

 0x02          - 16-bit address of destination device

 0x03          - 64-bit extended address of destination device

 0x04 - 0xff   -  Reserved

 dest_endpoint
 - This field indicates the destination endpoint to which the data
 frame was sent.

 src_endpoint
 -This field indicates the source endpoint from which the data
 frame was originated.

 Status
 -This field indicates the status of data confirmation. For details
 of the various status values,

 g_Aps_Success_c (0x00)              This value indicates that the
 message
 is transmitted successfully.

 g_Aps_Asdu_Too_Long_c (0xA0)        This value indicates that the
 ASDU is long and the message
 cannot be transmitted.

 g_Aps_Illegal_Request_c (0xA3)      This value indicates that the
 request cannot be processed
 because the device is not a
 part of any PAN.

 g_Aps_Invalid_Parameter_c (0xA6)    This value indicates that one
 of the parameters in the
 message request had invalid
 value.

 g_Aps_No_Ack_c (0xA7)               This value indicates that, the
 APS acknowledgement is not
 received by the device even
 after retrying the message
 transmission for
 apsAckRetryTimes.

 g_Aps_No_Short_Address_c (0xA9)     This value indicates that the
 APS Layer could not find the
 short address for message
 request made with 64-bit
 address.

 g_Aps_Security_Fail_c (0xAD)        This value indicates that the
 APS layer could NOT secure the
 packet.

 g_Aps_Transaction_Overflow_c        APS Layer maintains a
 (0xb1)                             transaction table holding all
 the requests made by the upper
 layer. This status indicates
 that the transaction table is
 full. The application can retry
 the request after a delay.

 g_Aps_No_Buffer_For_Retries_c       This value indicates that APS
 (0xb2)                             layer could not complete the
 requested message transmission,
 due to insufficient buffers.


 *******************************************************************************/
typedef struct APSDE_Data_Confirmation_Tag {
    Address dest_address;
    uint8_t dest_addr_mode;
    uint8_t dest_endpoint;
    uint8_t src_endpoint;
    uint8_t status;
}__attribute__((__packed__)) APSDE_Data_Confirmation_t;

/*******************************************************************************
 APSDE_Data_Indication_Tag

 The data indication structure is received from APS layer(APS).

 dest_address
 - This field indicates the destination address in the received
 message.

 dest_addr_mode
 -This field indicates the destination address mode in the received
 message. This field can take one of the following values:

 0x00       - Indirect data

 0x01       - 16-bit group address

 0x02       - 16-bit address of destination device

 0x03       - 64-bit extended address of destination device

 dest_endpoint
 - This field indicates the destination endpoint in the received
 message.

 src_addr_mode
 - This field indicates the source address mode in the received
 message. This field can have one of the following values:

 0x00       - Indirect data

 0x01       - 16-bit group address

 0x02       - 16-bit address of destination device

 0x03       - 64-bit extended address of destination device

 src_address -
 -This field indicates the source address from which the message is
 originated. This field can have one of the following values:

 *If the source address mode is 0x01, this field will have 16-bit
 address.

 *If source address mode is 0x03, this field will have 64-bit
 extended address.

 src_endpoint
 - This field indicates the source endpoint from which the data
 frame was originated.

 profile_id
 - This field indicates the 16-bit profile ID.

 cluster_id
 - This field indicates the cluster ID.

 asdulength
 - This field indicates the length of the data received.

 was_broadcast
 - This field indicates whether the data frame is received through
 broadcast.

 security_status
 - This field indicates whether the received message was secured or
 not and type of the security applied. The enum values are as
 follows:

 *g_APS_UNSECURED_c - ASDU is received without any security.

 *g_Recieved_Nwk_Key_Secured_Asdu_c - ASDU is received with security
 using the Network Key.

 *g_Recieved_Link_Key_Secured_Asdu_c - ASDU is received with
 security using the link Key.

 link_quality
 -This field indicates the  LQI of the received message.

 a_asdu[1]
 - This field points to the actual message received.

 *******************************************************************************/
typedef struct APSDE_Data_Indication_Tag {
    Address dest_address;
    uint8_t dest_addr_mode;
    uint8_t dest_endpoint;
    uint8_t src_addr_mode;
    Address src_address;
    uint8_t src_endpoint;
    profile_id_t profile_id;
    cluster_id_t cluster_id;
    uint8_t asdulength;
    uint8_t was_broadcast;
    uint8_t security_status;
    uint8_t link_quality;
    uint8_t a_asdu[128]; //! Place holder for Data
}APSDE_Data_Indication_t;

/*******************************************************************************
APSDE_Data_Confirmation_Tag

    The data confirmation structure is received from APS layer(APS).

   dest_address
            - This field indicates the individual device address or group
            address of the transmitted message.

   dest_addr_mode
            -This field indicates the destination address mode. This field
            takes one of the following values:

             0x00          - Indirect data transmission (destination address
                              and destination endpoint are not present)

             0x01          - 16-bit group address

             0x02          - 16-bit address of destination device

             0x03          - 64-bit extended address of destination device

             0x04 - 0xff   -  Reserved

   dest_endpoint
            - This field indicates the destination endpoint to which the data
            frame was sent.

   src_endpoint
            -This field indicates the source endpoint from which the data
            frame was originated.

   Status
            -This field indicates the status of data confirmation. For details
            of the various status values,

            g_Aps_Success_c (0x00)              This value indicates that the
                                                message
                                                is transmitted successfully.

            g_Aps_Asdu_Too_Long_c (0xA0)        This value indicates that the
                                                ASDU is long and the message
                                                cannot be transmitted.

            g_Aps_Illegal_Request_c (0xA3)      This value indicates that the
                                                request cannot be processed
                                                because the device is not a
                                                part of any PAN.

            g_Aps_Invalid_Parameter_c (0xA6)    This value indicates that one
                                                of the parameters in the
                                                message request had invalid
                                                value.

            g_Aps_No_Ack_c (0xA7)               This value indicates that, the
                                                APS acknowledgement is not
                                                received by the device even
                                                after retrying the message
                                                transmission for
                                                apsAckRetryTimes.

            g_Aps_No_Short_Address_c (0xA9)     This value indicates that the
                                                APS Layer could not find the
                                                short address for message
                                                request made with 64-bit
                                                address.

            g_Aps_Security_Fail_c (0xAD)        This value indicates that the
                                                APS layer could NOT secure the
                                                packet.

            g_Aps_Transaction_Overflow_c        APS Layer maintains a
             (0xb1)                             transaction table holding all
                                                the requests made by the upper
                                                layer. This status indicates
                                                that the transaction table is
                                                full. The application can retry
                                                the request after a delay.

            g_Aps_No_Buffer_For_Retries_c       This value indicates that APS
             (0xb2)                             layer could not complete the
                                                requested message transmission,
                                                due to insufficient buffers.


*******************************************************************************/
typedef struct APSDE_InterPan_Data_Confirmation_Tag {
    uint8_t asduhandle;
    uint8_t status;
}__attribute__((__packed__)) APSDE_InterPan_Data_Confirmation_t;


/*******************************************************************************
APSDE_Data_Indication_Tag

   The data indication structure is received from APS layer(APS).

   dest_address
            - This field indicates the destination address in the received
            message.

   dest_addr_mode
            -This field indicates the destination address mode in the received
            message. This field can take one of the following values:

             0x00       - Indirect data

             0x01       - 16-bit group address

             0x02       - 16-bit address of destination device

             0x03       - 64-bit extended address of destination device

   dest_endpoint
            - This field indicates the destination endpoint in the received
            message.

   src_addr_mode
            - This field indicates the source address mode in the received
            message. This field can have one of the following values:

             0x00       - Indirect data

             0x01       - 16-bit group address

             0x02       - 16-bit address of destination device

             0x03       - 64-bit extended address of destination device

   src_address -
            -This field indicates the source address from which the message is
            originated. This field can have one of the following values:

            *If the source address mode is 0x01, this field will have 16-bit
            address.

            *If source address mode is 0x03, this field will have 64-bit
            extended address.

   src_endpoint
            - This field indicates the source endpoint from which the data
            frame was originated.

   profile_id
            - This field indicates the 16-bit profile ID.

   cluster_id
            - This field indicates the cluster ID.

   asdulength
            - This field indicates the length of the data received.

   was_broadcast
            - This field indicates whether the data frame is received through
            broadcast.

   security_status
            - This field indicates whether the received message was secured or
            not and type of the security applied. The enum values are as
            follows:

            *g_APS_UNSECURED_c - ASDU is received without any security.

            *g_Recieved_Nwk_Key_Secured_Asdu_c - ASDU is received with security
            using the Network Key.

            *g_Recieved_Link_Key_Secured_Asdu_c - ASDU is received with
            security using the link Key.

   link_quality
            -This field indicates the  LQI of the received message.

   a_asdu[1]
            - This field points to the actual message received.

*******************************************************************************/
typedef struct APSDE_InterPan_Data_Indication_Tag {
    uint8_t srcaddrmode;
    uint16_t srcpanid;
    Address srcaddress;
    uint8_t dstaddrmode;
    uint16_t dstpanid;
    Address dstAddress;
    profile_id_t  profileId;
    cluster_id_t  clusterId;
    uint8_t asdulength;
    uint8_t link_quality;
    uint8_t a_asdu[128];
}__attribute__((__packed__)) APSDE_InterPan_Data_Indication_t;

/*******************************************************************************
APSDE_Response_Tag

 User needs to check msgType parameter to assert whether the
 received message is an indication or confirmation. The enumeration
 APSDE_Primitive_Ids defines the msgType. The following table
 describes the same:

 Status

 g_APSDE_Data_Confirmation_c (0xF6)     This status value indicates that
 the message received is
 confirmation.

 g_APSDE_Data_Indication_c  (0xF7)      This status value indicates that
 the message received is
 indication.

 NOTE : For optimum performance, application can queue up the received
 message index and processes the same in the context of the
 application. This happens when the Application_main() function is
 called.

 *******************************************************************************/
typedef struct APSDE_Response_Tag {
    uint8_t msgType;
    union APSDE_Response_Primitive {
        APSDE_Data_Confirmation_t data_Confirmation;
        APSDE_Data_Indication_t  data_Indication;
        APSDE_InterPan_Data_Confirmation_t data_InterPan_Confirmation;
        APSDE_InterPan_Data_Indication_t  data_InterPan_Indication;
    }APSDE_Response_Primitive;
}__attribute__((__packed__)) APSDE_Response_t ;
/******************************************************************************/
/*******************************************************************************
 ZigBeeAPSDEDataRequest_t
 aPayload[1]
 -This field indicates the place holder for the APSDE data

 aReserved[0x25]
 -This section is reserved to fill the below layer's header and
 should be untouched by the Application

 AsduLength
 -This field indicates the length of the ASDU

 ClusterId
 -This field indicates the cluster id meaning the message id for data transmission

 DestEndpoint
 -This field indicates the destination endpoint on the destination node to which
 the data frame is intended


 ProfileId
 -This field indicates the profile id used for data transmission

 Radius
 -This field indicates the Radius for the data transmission

 SrcEndpoint
 -This field indicates the Source endpoint of the data frame transmission

 TxOptions
 -This field indicates the Transmit options namely NWK security, APS security,
 APS ack enabled or disabled

 *******************************************************************************/
typedef struct {
  uint8_t          DestEndpoint;
  uint8_t          SrcEndpoint;
  ProfileID        ProfileId;
  ClusterID        ClusterId;
  uint8_t          AsduLength;
  uint8_t          TxOptions;
  uint8_t          Radius;
  uint8_t          aReserved[0x31];
  uint8_t          aPayload[0x33];
}ZigBeeAPSDEDataRequest_t;

#if ( g_FRAGMENTATION_TEST_d == 1 )
typedef struct FragmentTest_Tag{
	/* This shall be used locate the index of the data that should not to be sent*/
    uint8_t testDiscardFrame;
    /* This shall be used locate the index of the data that should not to be sent*/
    uint8_t testDiscardFrameOne;
}__attribute__((__packed__))FragmentTest_t;
#endif /* ( g_FRAGMENTATION_TEST_d == 1 )*/


 /* This structure provides flexibility of enabling or disabling
Binding support  */
typedef struct Binding_Configuration_Tag{
    uint8_t APS_binding_supported;
}__attribute__((__packed__))Binding_Configuration_t;

extern Binding_Configuration_t Binding_Configuration;

extern uint16_t g_Max_Fragment_Size_c ;

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/
/*******************************************************************************
 APS_APSDErequest

 The application invokes APS_APSDErequest() function to send the data frame.

 The input parameter of this function is the index to the buffer pointer that
 holds the actual data for transmission. The application should allocate the
 buffer, fill up the required information and pass it on to the stack. The
 layer receiving the buffer will be responsible for freeing up the buffer.

 NOTE : Application need to assert if, the return value of
 APS_APSDErequest function is equal to g_Success_c (0x00). If the value
 returned is g_Queue_full_c (0xFF), then the application is responsible
 for freeing the message index.

 *******************************************************************************/
uint8_t APS_APSDErequest(uint8_t message_index);
/*******************************************************************************
 * APS_generateAPSDEdataConfirmation
 ********************************************************************************
 *   Return Value:
 *   None
 *
 *   Parameters:
 *   Input parameter:
 *       uint8_t   buffer_id: this input parameter holds the ID of the buffer
 *       that is being used to send the APSDE-Data-Confirmation to the Application.
 *
 *       uint8_t status: This parammeter holds the information about what
 *       status needs to be indicated in the APSDE-Data-Confirmation to the
 *       application.
 *
 *   Output parameter:
 *   None
 *
 *   Scope of the function:
 *   This is a global function.
 *
 *   Purpose of the function:
 *   This function has been provided for issuing APSDE-Data-Confirmation to the
 *   Application.
 *
 *   Before the function is called:
 *   None
 *
 *   After the function is called:
 *   None
 *
 *******************************************************************************/
void APS_generateAPSDEdataConfirmation(uint8_t buffer_id, uint8_t status);



#if ( ( g_ZIGBEE_ROUTER_d == 1 ) || ( g_ZIGBEE_END_DEVICE_d == 1 ) )
void sendDeviceAnnance( void );
#endif

#if ( ( g_APS_LAYER_SEC_PROCESSING_d == 1) && ( g_TRUST_CENTRE_d == 1 ) )
uint8_t SetAppLinkKey( uint8_t *longAddress1, uint8_t *longAddress2, uint8_t *key );
#endif

/*******************************************************************************
 * AppHandleDataIndication
 ********************************************************************************
 *
 * 	Return Value :
 *   None
 * 	Input Parameters:
 *   	bufferIndex -
 *       	gives the index to the pointer that holds the actual Data
 *       	indication response.
 *    	pDataIndication -
 *       	contains the data indication results.
 * 	Output Parameters:
 *   	None
 * 	Scope of the function:
 *   	This function is called from the stack to the application
 *
 * 	Purpose of the function:
 *   	This function is the data indication handler for the data request.
 *
 * 	Before the function is called:
 *   	None
 * 	After the function is called:
 *   	None
 *
 *******************************************************************************/
void AppHandleDataIndication(uint8_t bufferIndex,
        APSDE_Data_Indication_t * pDataIndication);
/*******************************************************************************
 * AppHandleDataConfirmation
 ********************************************************************************
 *
 * Return Value :
 *   None
 * Input Parameters:
 *   bufferIndex -
 *       gives the index to the pointer that holds the actual Data
 *       confirmation request.
 *    pDataConfirmation -
 *       contains the data confirmation results.
 * Output Parameters:
 *   None
 * Scope of the function:
 *   This function is called from the stack to the application
 *
 * Purpose of the function:
 *   This function is the data confirmation handler for the data request.
 *
 * Before the function is called:
 *   None
 * After the function is called:
 *   None
 *
 *******************************************************************************/

void AppHandleDataConfirmation(uint8_t bufferIndex,
        APSDE_Data_Confirmation_t* pDataConfirmation);
		
/******************************************************************************
* AppAddressConflictHandler
******************************************************************************
*
* Return Value :
*		NONE
*			
* Input Parameters:
*       uint8_t BufferIndex
*       uint16_t oldShortAddress: This value specifies short address of the
*                           device before address conflict resolusion
* Output Parameters:
*       None
* Scope of the function:
*	This is an handler function which will be used in application
* Purpose of the function:
*	This function is used to inform about new short address to the application
* Before the function is called:
*   None
*
* After the function is called:
*   None
******************************************************************************/

void AppAddressConflictHandler(uint8_t bufferIndex ,uint16_t oldShortAddress);


//#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
/*******************************************************************************
* App_InterPanDataindication
********************************************************************************
*
* 	Return Value :
*   None
* 	Input Parameters:
*   	bufferid -
*       	gives the index to the pointer that holds the actual Data
*       	indication response.
*    	pDataIndication -
*       	contains the data indication results.
* 	Output Parameters:
*   	None
* 	Scope of the function:
*   	This function is called from the stack to the application when interpan
*       data received
*
* 	Purpose of the function:
*   	This function is the data Inter Pan indication handler for the data request.
*
* 	Before the function is called:
*   	None
* 	After the function is called:
*   	None
*
*******************************************************************************/
void App_InterPanDataindication( uint8_t bufferid,
                                APSDE_InterPan_Data_Indication_t *pDataIndication );
/*******************************************************************************
* AppHandleDataConfirmation
********************************************************************************
*
* Return Value :
*   None
* Input Parameters:
*   bufferIndex -
*       gives the index to the pointer that holds the actual Data
*       confirmation request.
*    pDataConfirmation -
*       contains the data confirmation results.
* Output Parameters:
*   None
* Scope of the function:
*   This function is called from the stack to the application
*
* Purpose of the function:
*   This function is the data confirmation handler for the data request.
*
* Before the function is called:
*   None
* After the function is called:
*   None
*
*******************************************************************************/

void App_InterPanDataConfirmation ( uint8_t bufferIndex,
                                 APSDE_InterPan_Data_Confirmation_t* pDataConfirmation );

//#endif  /* ( g_INTERPAN_DATA_ENABLE_d == 1 ) */

/*******************************************************************************
 * ZigBeeSendUnicastData
 ********************************************************************************
 *   Return Value:
 *        returns the status of the function call SUCCESS or error code
 *
 *   Parameters:
 *   Input parameter:
 *       BufferID indicates the buffer identifier that holds the APSDE data request
 *           fields
 *       ShortAddress gives the addressing mode to be used. TRUE indicates short
 *           addressing mode FALSE indicates IEEE addressing mode
 *       DestAddress gives the destination address. This takes 16-bit or 64-bit
 *           based on ShortAddress
 *       pAPSDERequest indicates the pointer corresponding to the buffer id that
 *           holds the data request fields
 *
 *   Output parameter:
 *        None
 *
 *   Scope of the function:
 *       This is a global function.
 *
 *   Purpose of the function:
 *       This function allows the Application to initiate APSDE data request
 *       to the specified destination address.
 *
 *   Before the function is called:
 *       The device should be part of the network to initiate data transmission
 *       successfully
 *
 *   After the function is called:
 *       None
 *
 *******************************************************************************/
uint8_t ZigBeeSendUnicastData(ZigBee_Outgoing_Msg_Type msgType,
        Address DestAddress, ZigBeeAPSDEDataRequest_t *pAPSDERequest);
/*******************************************************************************
 * ZigBeeSendGroupData
 ********************************************************************************
 *   Return Value:
 *        returns the status of the function call SUCCESS or error code
 *
 *   Parameters:
 *   Input parameter:
 *       BufferID - indicates the buffer identifier that holds the APSDE data
 *       request fields
 *       GroupAddress - indicates the group id to which the data is transmitted
 *       pAPSDERequest - indicates the pointer corresponding to the buffer id that
 *       holds the data request fields
 *
 *   Output parameter:
 *        None
 *
 *   Scope of the function:
 *       This is a global function.
 *
 *   Purpose of the function:
 *       This function allows the Application to initiate APSDE data request
 *       to the specified Group address.
 *
 *   Before the function is called:
 *       The device should be part of the network to initiate data transmission
 *       successfully.
 *
 *   After the function is called:
 *       None
 *
 *******************************************************************************/
uint8_t ZigBeeSendGroupData( GroupID GroupAddress,
        ZigBeeAPSDEDataRequest_t * pAPSDERequest);

/*******************************************************************************
 * ZigBeeSendBroadcastData
 ********************************************************************************
 *   Return Value:
 *        returns the status of the function call SUCCESS or error code
 *
 *   Parameters:
 *   Input parameter:
 *       BufferID indicates the buffer identifier that holds the APSDE data
 *       request fields
 *
 *       pAPSDERequest indicates the pointer corresponding to the buffer id that
 *       holds the data request fields
 *
 *   Output parameter:
 *        None
 *
 *   Scope of the function:
 *       This is a global function.
 *
 *   Purpose of the function:
 *       This function allows the Application to broadcast APSDE data request
 *
 *   Before the function is called:
 *       The device should be part of the network to initiate data transmission
 *       successfully.
 *
 *   After the function is called:
 *       None
 *
 *******************************************************************************/
uint8_t ZigBeeSendBroadcastData(ZigBeeAPSDEDataRequest_t * pAPSDERequest);


Group_Table_t ZigBeeGroupTableEntry(uint8_t index);


//uint8_t ZigBeeAddGroupTableEntry(uint16_t groupAddress, uint8_t endpoint);
uint8_t ZigBeeAddGroupTableEntry(uint16_t groupAddress, uint8_t endpoint,
                                 uint8_t *ptrEntryIndex);

#if ( g_APS_GROUP_DATA_RX_d == 1 )
uint8_t APS_Verify_GroupAddrAndEndPoint( uint16_t groupAddress, uint8_t endPoint);
uint8_t APS_getGroupTableIndex(uint16_t group_address);
uint16_t APS_getGroupId(uint8_t index);
#endif

/*******************************************************************************
 * ZigBeeGetMaxAPSPayloadLength
 ********************************************************************************
 *   Return Value:
 *        Returns maximum APS payload length
 *
 *   Parameters:
 *   Input parameter:
 *       None
 *
 *   Output parameter:
 *        None
 *
 *   Scope of the function:
 *       This is a global function.
 *
 *   Purpose of the function:
 *       This function allows the Application to know about maximum APS Payload
 *      Length
 *
 *   Before the function is called:

 *
 *   After the function is called:
 *       None
 *
 *******************************************************************************/
uint8_t ZigBeeGetMaxAPSPayloadLength(void);

#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
void app_APSFC_FailureHandler(void);	
void app_APSDecryptFailuresHandler(void);

/* A counter that is incremented each time the APS layer successfully transmits
a unicast.*/
void app_APSTrxUnicastSuccessHandler(void);

/* A counter that is incremented each time the APS layer 46 retries the sending
of a unicast.*/
void app_APSTrxUnicastRetryHandler(void);

/*A counter that is incremented each time the APS layer fails to send a unicast. */
void app_APSTrxUnicastFailureHandler(void);

/* A counter that is incremented each time the APS layer transmits a broadcast.*/
void app_APSTrxBroadcastHandler(void);

/* A counter that is incremented each time the APS layer receives a broadcast*/
void app_APSRecBroadcastHandler(void);

/* A counter that is incremented each time the APS layer receives a unicast */
void app_APSRecUnicastHandler(void);


void app_PacketValidateDropHandler(void);


#endif /*g_DIAGNOSTICS_CLUSTER_ENABLE_d */


/* This Api is used to set Fragmentation Transmission window size ,range 1 to 8
returns true it is able to set*/
bool ZigBeeSetFragmentTxWinSize(uint8_t txWinSize);

/* default range 0x00 to 0xff.The standard delay, in milliseconds,
between sending two blocks of a fragmented transmission*/
void ZigBeeSetFragmentInterframeDelay(uint8_t interframeDelay);


void ZigBeeFragmentMessageSentHandler(ZigBeeStatus_t status );
void ZigBeeFragmentMessageSent(uint8_t blockTransmitted);

/*******************************************************************************
 * ZigBeeFragmentSendUnicast
 ********************************************************************************
 *   Return Value:
 *
 *
 *   Parameters:
 *   Input parameter:
 *   	ZigBee_Outgoing_Msg_Type msgType :Direct or with short address etc
 *      ZigBeeAPSDEDataRequest_t * pAPSDERequest:
 *      Address destAddress:address of the destination device
 *      uint8_t maxFragmentSize : max size of each fragment packet size(MAX_PAY_LOAD_SIZE)
 *	    uint8_t interframedelay : delay between each intermediate blocks
 *      uint8_t *pMsgToBeFragmented: pointer to static array payload, application
 *									is suppose to free if it is dynamic array
 *									which is allocated from msg.
 *		uint16_t msgLen : length of the msg .
 *
 *   Output parameter:
 *        None
 *
 *   Scope of the function:
 *       This is a global function.
 *
 *   Purpose of the function:
 *      This function allows the Aps packet data transmission with fragmentation
 *
 *   Before the function is called:

 *
 *   After the function is called:
 *       None
 *
 *******************************************************************************/


ZigBeeStatus ZigBeeFragmentSendUnicast(
                            ZigBee_Outgoing_Msg_Type msgType,
                            ZigBeeAPSDEDataRequest_t * pAPSDERequest,
                            Address destAddress,
                            uint8_t maxFragmentSize,uint8_t interframedelay,
                            uint8_t *pMsgToBeFragmented,uint16_t msgLen);

void ZigBeeIncomingFragmentMessage(uint8_t *pFragmentMsg,uint8_t msgLength,
                                   ZigBeeStatus_t status,
                                   uint8_t blocknoReceived,
                                   uint16_t SourceAddress);


void ZigBeeFragmentIncomingMessageHandler(uint8_t *pFragmentMsg,uint8_t msgLength,
                                   ZigBeeStatus_t status);

/*******************************************************************************
 * APS_startFragmentation
 ********************************************************************************
 *   Return Value:
 *   status of start fragmentation. Returns success() if successfully accepted
 *      fragmented packet, No_Permit(0x01) if no perm(0x00)ission is given for fragmentation
 *
 *   Parameters:
 *   Input parameter:
 *       uint8_t   bufferId: this input parameter holds the ID of the buffer
 *       that is being used to send the APSDE-Data-Indication to the Application.
 *
 *
 *   Output parameter:
 *   None
 *
 *   Scope of the function:
 *   This is a global function.
 *
 *   Purpose of the function:
 *   This function has been provided for issuing APSDE-Data-Indication to the
 *   Application.
 *
 *   Before the function is called:
 *   FrgmtDetails structure has to be updated
 *
 *   After the function is called:
 *   None
 *
 *******************************************************************************/
FragmentReqStatus APS_startFragmentation(uint8_t bufferId );

uint8_t APS_CheckFragmentEnabled( uint8_t txoptions );
/*******************************************************************************
 * APS_SendFragmentedData
 ********************************************************************************
 * Return Value:
 *   None
 *
 *  Parameters:
 *  Input parameter:
 *     bufferId
 *
 *  Output parameter:
 *     None
 *
 * Scope of the function:
 *  This is a global function.
 *
 * Purpose of the function:
 *
 *
 *
 * Before the function is called:
 *  None
 *
 * After the function is called:
 *  None
 *
 *******************************************************************************/
void APS_SendFragmentedData(uint8_t bufferId);

/*******************************************************************************
 * APS_AllocateFragmentationSpace
 ********************************************************************************
 *   Return Value:
 *   None
 *
 *   Parameters:
 *   Input parameter:
 *   None
 *
 *   Output parameter:
 *          fragTxBuffer allocated for transmitting the frgamented packet
 *
 *   Scope of the function:
 *   This is a global function.
 *
 *   Purpose of the function:
 *   This function is provided for Application to inform the stack the pointer
 *    space allocated by Application. This allocated memory could be static or
 *    dynamic and shall contain the transmitted or received data
 *    fragTxBuffer is used for transmission and the fragRxBuffer shall be
 *    used for reception. The Application could initialize same buffer for
 *    both. This is Application's deisgn choice.
 *
 *   Before the function is called:
 *   None
 *
 *   After the function is called:
 *   None
 *
 * Note: The buffers should be allocated static or dynamic. or otherwise
 * there could be indeterminant/unexpected behavior  by the device
 *******************************************************************************/
void APS_AllocateFragmentationSpace(uint8_t* fragTxBuffer, uint8_t *fragRxBuffer);

bool ZigBeeIsFragmentReceptionInProgress( void );

void  ZigBeeFragment_SetFragmentOptions( uint8_t *pBufferRx);
uint8_t ZigBeeGetFragmentTxWinSize( void );
uint8_t ZigBeeGetFragmentInterBlockFrameDelay( void );
void ZigBeeSetFragmentBlockDataLength(uint8_t blockDataLength);
uint16_t ZigBeeGetFragmentLastmemoryLocation( void );
void ZigBeeSetFragmentLastmemoryLocation( uint16_t lastMemoryLoc );
/*******************************************************************************
 * End Of File
 *******************************************************************************/
#endif                                        /*APSDE_INTERFACE_H*/
