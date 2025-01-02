/*******************************************************************************
 * stack_common.h
 ********************************************************************************
 * Program Description:
 *
 *     This file contains the Nwk layer status values returned when a request is
 *     sent to the Nwk Layer from NHLE (APS/ZDO).All common status values are
 *     placed in this file.
 *
 *******************************************************************************/

#ifndef STACK_COMMON_H
#define STACK_COMMON_H

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include <stdbool.h>
#include "ZigBeeDataTypes.h"
/*******************************************************************************
 * global Constants
 *******************************************************************************/

#define g_PROTOCOL_ID_c                               0x00

/* Common Macros Used By Stack */

/* Values for Beacon Order and Superframe Order for Zigbee 2006 */
#define g_BEACON_ORDER_VALUE_c                        0x0F

#define g_SUPERFRAME_ORDER_VALUE_c                    0x0F

/* Length of extended address */
#define g_EXTENDED_ADDRESS_LENGTH_c                   0x08

/* Length of short address */
#define g_SHORT_ADDRESS_LENGTH_c                      0x02

/* Length of network PAN ID */
#define g_PAN_ID_LENGTH_c                             0x02
/* Length of Cluster Id */
#define g_CLUSTER_ID_LENGTH_c                         0x02

/* Extended PAN ID Length */
#define g_EXTENDED_PAN_ID_LENGTH_c                    0x08

/* Attribute ID Of One Byte Length */
#define g_ONE_BYTE_ATTRIBUTE_LENGTH_c                 0x01

/* Invalid device or group address value */
#define g_INVALID_ADDRESS_c                           0xFFFF

/* Invalid value */
#define g_INVALID_VALUE_c                             0xFF

/* Reset Value */
#define g_RESET_c                                     0x00

/* Broadcast Address */
#define g_BROADCAST_ADDRESS_c                         0xFFFF

/* Broadcast Address to all Active Devices */
#define g_BROADCAST_ADDRESS_ACTIVE_DEVICES_c          0xFFFD

/* Broadcast Address to all coordinators and routers Devices */
#define g_BROADCAST_ADDRESS_FOR_ZC_AND_ZR_c           0xFFFC

/* Broadcast Addres LSB */
#define g_BROADCAST_ADDRESS_LSB_c                     0xFF

/* Broadcast Addres MSB */
#define g_BROADCAST_ADDRESS_MSB_c                     0xFF

/* Entry not found */
#define g_ENTRY_NOT_FOUND_c                           0xFF

#define g_ENTRY_FOUND_c								  0x00

/* ZDP end point id */
#define g_ZDO_ENDPOINT_ID_c                           0x00

/* ZDP profile id */
#define g_ZDP_PROFILE_ID_c                            0x0000

/* Endpoint ID's greater than below values are invalid */
#define g_INVALID_ENDPOINT_ID_c                       0xF1

/* This macro is used to check for security level zero */
#define g_NO_SECURITY_c                               0x00

/* Length of Frame Counter */
#define g_FRAME_COUNTER_LENGTH_c                      0x04

/* This macro is used whereever the length of the key is needed.
 Be it whether network key or any other key used by ZigBee. */
#define g_KEY_LENGTH_c                                0x10

/* Length of the primitive ID */
#define g_PRIMITIVE_ID_LENGTH_c                       0x01

/* Invalid value of Cluster ID */
#define g_INVALID_CLUSTER_ID_c                        0xFFFF

/* Invalid value of Attribute ID */
#define g_INVALID_ATTRIBUTE_ID_c                      0xFF,0xFF

/* This macros checks if the given 8 bytes address is null */
#define Is_Address_Null( address )  ( ( address )[ 0 ] | ( address ) [ 1 ] |  \
                                      ( address )[ 2 ] | ( address )[ 3 ] |   \
                                      ( address )[ 4 ] | ( address )[ 5 ] |   \
                                      ( address )[ 6 ] | ( address )[ 7 ] )

#define g_SIZE_OF_EXTENDEDPANID_c                     0x08

#define g_SIZE_OF_PAN_ID_c                            0x02

#define g_SIZE_SCAN_CHANNELS_c                        0x04

/* This mask is used for the 26th bit indicating the 26th channel to be
 scanned.*/
#define g_MASK_FOR_26_CHANNEL_c                       0x04000000

/* This mask is used for the 11th bit indicating the 11th channel to be scanned.*/
#define g_MASK_FROM_11_CHANNEL_c                      0x00000800

/* This macro is used for resetting the Relationship field in the Device Info
 field of the Neighbor Table.*/
#define g_RESET_RELATION_SHIP_c                       0xC7
/* This macro is used for setting the Relationship field as SIBLING in the
 Device Info field of the Neighbor Table.*/
#define g_MASK_FOR_SET_RELATION_AS_SIBLING_c          0x10

/*
 The Critical Section Macros are defined below, so that NVM save is not
 performed when the device is doing a formation,discovery or join. During
 these operation the transceiver cannot be turned off to perform a NVM
 save.

 First Step  -  Saving the Interrupts
 Second Step -  Disabling Interrupts
 Third Step  -  Enter Critical Section
 Fourth Step -  Restoring Interrupts
 */
#define Enter_Critical_Section()            g_Critical_Section = 0xFF

/*
 First Step  - Restoring the Interrupts

 Second Step - Exit Critical Section
 */

#define Exit_Critical_Section()             g_Critical_Section = 0x00

/*******************************************************************************
 * Type Definitions
 *******************************************************************************/
/*  Error codes returned from MAC to Network layer */
enum MAC_STATUS_VALUES {
    g_MAC_Success_c = 0x00,
    g_PAN_At_Capacity_c = 0x01,
    g_PAN_Access_denied_c = 0x02,
    g_MAC_Scan_In_Progress_c = 0xAA,
    g_MAC_Beacon_Loss_c = 0xE0,
    g_MAC_Channel_Access_Failure_c,
    g_MAC_Denied_c,
    g_MAC_Disable_TRX_Failure_c,
    g_MAC_Failed_Security_Check_c,
    g_MAC_Frame_Too_Long_c,
    g_MAC_Invalid_GTS_c,
    g_MAC_Invalid_Handle_c,
    g_MAC_Invalid_Parameter_c,
    g_MAC_No_ACK_c,
    g_MAC_No_Beacon_c,
    g_MAC_No_Data_c,
    g_MAC_No_Short_Address_c,
    g_MAC_Out_Of_CAP_c,
    g_MAC_PAN_ID_Conflict_c,
    g_MAC_Realignment_c,
    g_MAC_Transaction_Expired_c = 0xF0,
    g_MAC_Transaction_Overflow_c,
    g_MAC_TX_Active_c,
    g_MAC_Unavailable_Key_c,
    g_MAC_Unsupported_Attribute_c,
    g_MAC_Missing_Address_c,
    g_MAC_Past_Time_c
};

/**
 Nwk Layer Enumeration & Status Values Definitions
 **/

enum nwk_layer_status_values {
    g_Success_c = 0x00,
    g_Invalid_Group_c = 0xa5,
    g_Invalid_Parameter_c = 0xc1,
    g_Invalid_Request_c = 0xc2,
    g_Not_Permitted_c = 0xc3,
    g_Startup_Failure_c = 0xc4,
    g_Already_Present_c = 0xc5,
    g_Sync_Failure_c = 0xc6,
    g_neighbor_Table_Full_c = 0xc7,
    g_Unknown_Device_c = 0xc8,
    g_Unsupported_Attribute_c = 0xc9,
    g_No_Networks_c = 0xca,
    g_Leave_Unconfirmed_c = 0xcb,
    g_Max_Frm_Cntr_c = 0xcc,
    g_No_Key_c = 0xcd,
    g_Bad_Ccm_Output_c = 0xce,

    //g_No_Routing_Capacity_c = 0xcf,
    g_Route_Discovery_Failed_c = 0xd0,
    g_Route_Error_c = 0xd1,
    g_Btt_Table_Full_c = 0xd2,
    g_Frame_Not_Buffered_c = 0xd3,
    g_Transaction_Expired_c = 0xF0,

    /* Proprietary status values */
    g_Discovery_Under_Progress_c = 0xd4,
    g_Authentication_Failed_c = 0xd5
};

/**
 Route Table Enumeration & Status Values Definitions.
 Nwk Layer Route Table Enumeration & Status Values Definitions.
 **/

enum route_status_values {
    g_Active_c = 0x00,
    g_Discovery_Underway_c = 0x01,
    g_Discovery_Failed_c = 0x02,
    g_Inactive_c = 0x03,
    g_Validation_Underway_c = 0x04
};

enum DEVICE_TYPE {
   g_COORDINATOR_c,
   g_ROUTER_c,
   g_END_DEVICE_c
};

/* ZDP enumerations used for ZDP management primitives. */
enum ZDP_STATUS_VALUES {
    g_Inv_Request_Type_c = 0x80,
    g_Device_Not_Found_c,
    g_Invalid_Ep_c,
    g_Not_Active_c,
    g_Not_Supported_c,
    g_Timeout_c,
    g_No_Match_c,
    /*0x87 - reserved*/
    g_No_Entry_c = 0x88,
    g_No_Descriptor_c,
    g_Insufficient_Space_c,
    g_ZDP_Not_Permitted_c,
    g_Table_Full_c
};

/* Enumeration defined for setting the values of parameter is_set of function
 APS_getSetAuthenticationBitInNBT. */
enum {
    g_Device_Authenticated_c,
    g_Get_Authentication_Status_Of_Device_c,
    g_Wipe_Device_Entry_If_Not_Authenticated_c
};

/*******************************************************************************
 sBitLocater_Tag

 This Structure is helpful in locating each bit data indication.

 NOTE : For optimum performance, in order utilise all the bit in the Byte
 Now this Structure is used for TX_OPTION and for Fragmentation Options

 *******************************************************************************/
typedef union sBitLocater_Tag {
    struct {
        uint8_t BIT00 :1;
        uint8_t BIT01 :1;
        uint8_t BIT02 :1;
        uint8_t BIT03 :1;
        uint8_t BIT04 :1;
        uint8_t BIT05 :1;
        uint8_t BIT06 :1;
        uint8_t BIT07 :1;
    } iBits;
    uint8_t u8State;
} sBitLocater_t;
/******************************************************************************/

typedef uint8_t ZigBeeStatus;

typedef enum ZigBeeStatus_Tag{
    /* No error occured while parsing the required API parameters*/
    ZigBee_Success,
    /* Error occured while parsing the required API parameters*/
    ZigBee_Failure,
    /* Requested address table entry is active*/
    ZigBee_Address_Table_Entry_Is_Active,
    /* requested Stack table is full*/
    ZigBee_Table_Full,
    /* Out of buffers*/
    ZigBee_No_Buffer,
    /* Error occured in stack*/
    ZigBee_Error_Fatal,
    /* Argument passed for API is invalid*/
    ZigBee_Invalid_Argument,
    /* Transmission stopped inbetween in Fragmentation process*/
    ZigBee_Fragment_Tx_Aborted,
    /* Transmission Complete in Fragmentation process*/
    ZigBee_Fragment_Tx_Complete,
    /* Receiving stopped inbetween in Fragmentation process*/
    ZigBee_Fragment_Rx_Aborted,
    /* Receiving Complete in Fragmentation process*/
    ZigBee_Fragment_Reception_Completed,
    /* Message Too Long*/
    ZigBee_Fragment_Message_Too_Long,
    /* Request might be not valid for the flashed device type or it is not in a
    state to receive call*/
    ZigBee_Invalid_Call,
    /* Device is not in network*/
    ZigBee_Device_Down,
    /* Feature not supported */
    ZigBee_Unsupported,
    /* Device type is unknown*/
    ZigBee_Unknown_Device_Type,
    /* No Requested Key*/
    ZigBee_No_Key,
    /* Entry in the table is empty*/
    ZigBee_No_Entry,
    /* Accessing entry is out of range in the table*/
    ZigBee_Index_Out_Of_Range,
    /* No data pending*/
    ZigBee_MAC_No_Data,
    /* No ACK received*/
    ZigBee_MAC_No_ACK,
    /* MAC Channel Access Failure*/
    ZigBee_Channel_Access_Failure,
    /* MAC key unavailable*/
    ZigBee_MAC_Unavailable_Key,
    /* MAC Failed Security Check*/
    ZigBee_Failed_Security_Check,
    /* MAC Invalid Parameter*/
    ZigBee_MAC_Invalid_Parameter

}ZigBeeStatus_t;









/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/

/* Extern access to the variable to save interrupts. */
extern bool g_Save_Interuppts;
/* Variable to hold the Critical Section Information. */
extern uint8_t g_Critical_Section;

/* Variable to hold the Critical Section Information. */
extern uint8_t g_Internal_Poll_Conf_Is_Received;
/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/* None */

/*******************************************************************************
 * End Of File
 *******************************************************************************/
#endif                                                      /* STACK_COMMON_H */
