/******************************************************************************
* ZLL_Interpan.h
*******************************************************************************
* Program Description:
*
*
******************************************************************************/
#ifndef _ZLL_INTERPAN_H_
#define _ZLL_INTERPAN_H_


/*****************************************************************************
* Includes
*****************************************************************************/
#include "ZigBeeDataTypes.h"
#include "buffer_management.h"
#include "ZLL_defines.h"

#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZLL_Commissioning_Cluster.h"
#endif



/*****************************************************************************
* Global Constants
*****************************************************************************/


/*****************************************************************************
* Public Memory declarations
*****************************************************************************/

typedef struct Scan_request_record_list_field {
    /* The inter-PAN transaction identifier field is 32-bits in length and
    specifies an identifier for the inter-4 PAN transaction. This field shall
    contain a 32-bit non-zero random number and is used to identify the current
    transaction*/
    uint32_t inter_pan_transaction_identifier;

    struct ZigBee_information_Req_Tag{
       /* The logical type subfield is 2-bits in length and specifies the
        logical type of the device. The value of this subfield shall be set to
        0b00 for a coordinator, 0b01 for a router or 0b10 for an end device*/
        uint8_t logical_type:2;

        /* The Rx on when idle subfield is 1 bit in length and specifies the
        RxOnWhenIdle state of the device. The value of this subfield shall be
        set to 1 to indicate that the receiver is left on when the device is
        idle or 0 otherwise*/
        uint8_t rx_on_when_idle:1;

        /* reserved */
        uint8_t reserved :5;

    }__attribute__((__packed__))ZigBee_information_t;

    struct ZLL_information_Req_Tag{
        /* The factory new subfield is 1 bit in length and specifies whether
        the device is factory new. The value of this subfield shall be set to 1
        to indicate the device is factory new or 0 otherwise.*/
        uint8_t factory_new:1;

        /* Specifies whether the device is capable of assigning addresses.
        The value of this subfield shall be set to 1 to indicate the device is
        capable of assigning addresses or 0 otherwise.*/
        uint8_t address_assignment:1;

        /*reserved */
        uint8_t reserved1:2;

        /* The value of this subfield shall be set to 1 to indicate the device
        is capable of initiating a link (i.e. it supports the ZLL commissioning
        cluster at the client side) or 0 otherwise*/
        uint8_t link_initiator:1;

        /* can be 0 or 1*/
        uint8_t undefined:1;

        /*reserved */
        uint8_t reserved2:2;

    }__attribute__((__packed__))ZLL_information_t;
} __attribute__((__packed__))Scan_request_record_list_field;

typedef struct Device_information_record_list_field {

    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;

    /*The start index field is 8-bits in length and specifies the starting index
    (starting from 0) into the device table from which to get
    device information.*/
    uint8_t start_index;
} __attribute__((__packed__))Device_information_record_list_field;


typedef struct Identify_record_list_field {

    uint32_t inter_pan_transaction_identifier;
  /* specify the length of time the recipient is to remain in identify mode,
    Its value :
    0x0000 = Exit Identify mode
    0x0001- 0x00fe - Number of seconds to remain in identify mode.
    0xffff -Remain in identify mode for a default time known by the receiver  */
  uint16_t identify_duration;
} __attribute__((__packed__))Identify_record_list_field;

typedef struct Reset_to_factory_new_record_list_field {
    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;

}__attribute__((__packed__)) Reset_to_factory_new_record_list_field;

typedef struct Network_start_record_list_field {
    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;
    uint8_t extended_pan_identifier[8];
    uint8_t key_index;
    uint8_t encrypted_network_key[16];
    uint8_t logical_channel;
    uint16_t pan_identifier;
    uint16_t network_address;
    uint16_t group_identifiers_begin;
    uint16_t group_identifiers_end;
    uint16_t free_network_address_range_begin;
    uint16_t free_network_address_range_end;
    uint16_t free_group_identifier_range_begin;
    uint16_t free_group_identifier_range_end;
    uint8_t initiator_IEEE_address[8];
    uint16_t initiator_network_address;
} __attribute__((__packed__))Network_start_record_list_field;

#pragma pack(1)
typedef struct Network_join_record_list_field {
    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;
    uint8_t extended_pan_identifier[8];
    uint8_t key_index;
    uint8_t encrypted_network_key[16];
    uint8_t network_update_identifier;
    uint8_t logical_channel;
    uint16_t pan_identifier;
    uint16_t network_address;
    uint16_t group_identifiers_begin;
    uint16_t group_identifiers_end;
    uint16_t free_network_address_range_begin;
    uint16_t free_network_address_range_end;
    uint16_t free_group_identifier_range_begin;
    uint16_t free_group_identifier_range_end;
} __attribute__((__packed__))Network_join_record_list_field;
#pragma pack()



typedef struct Network_update_record_list_field {
    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;
    uint8_t extended_pan_identifier[8];
    uint8_t network_update_identifier;
    uint8_t logical_channel;
    uint16_t pan_identifier;
    uint16_t network_address;
} __attribute__((__packed__))Network_update_record_list_field;

typedef struct Scan_response_field {
    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;
    uint8_t rssi_correction;

     struct ZigBee_information_Tag{
       /* The logical type subfield is 2-bits in length and specifies the
        logical type of the device. The value of this subfield shall be set to
        0b00 for a coordinator, 0b01 for a router or 0b10 for an end device*/
        uint8_t logical_type:2;

        /* The Rx on when idle subfield is 1 bit in length and specifies the
        RxOnWhenIdle state of the device. The value of this subfield shall be
        set to 1 to indicate that the receiver is left on when the device is
        idle or 0 16 otherwise*/
        uint8_t rx_on_when_idle:2;

        /* reserved */
        uint8_t reserved :4;

    }__attribute__((__packed__))ZigBee_information_t;

    struct ZLL_information_Tag{
        /* The factory new subfield is 1 bit in length and specifies whether
        the device is factory new. The value of this subfield shall be set to 1
        to indicate the device is factory new or 0 otherwise.*/
        uint8_t factory_new:1;

        /* Specifies whether the device is capable of assigning addresses.
        The value of this subfield shall be set to 1 to indicate the device is
        capable of assigning addresses or 0 otherwise.*/
        uint8_t address_assignment:1;

        /*reserved */
        uint8_t reserved1:2;

        /* The value of this subfield shall be set to 1 to indicate the device
        is capable of initiating a link (i.e. it supports the ZLL commissioning
        cluster at the client side) or 0 otherwise*/
        uint8_t touck_link_initiator:1;

        /* to be set if the target device to be given priority by the initiator
        to start the network*/
        uint8_t touch_link_priority_request:1;

        /*reserved */
        uint8_t reserved2:2;

    }__attribute__((__packed__))ZLL_information_t;

    /* Key bitmask tells about ,what type of key is supported by the device,
    only one key can be used to encrypt the nwk key, higher priority is given to
    certification key*/
    struct Key_bitmask_Tag{
        /* network key is encrypted with development key*/
        uint8_t development_key:1;
        uint8_t reserved1:3;
        /* network key is encrypted with master key*/
        uint8_t master_key:1;
        uint8_t reserved2:3;
        uint8_t reserved3:5;
        uint8_t reserved4:2;
        /* network key is encrypted with certification key*/
        uint8_t certification_key:1;
    } __attribute__((__packed__))key_bitmask_t;
    /* random value , used during key exchange*/
    uint32_t response_identifier;
    /* FN and Ext_PanId = 0 target device is not proposing any new value,
    FN and Ext_PaniD !=0 target device is proposing new extpanid for new n/w ,
    !FN indicates abt n/w in which currently device is working on*/
    uint8_t extended_pan_identifier[8];

    uint8_t network_update_identifier;
    uint8_t logical_channel;
    uint16_t pan_identifier;
    uint16_t network_address;
    uint8_t no_of_subdevices;
    uint8_t total_group_identifiers;
    uint8_t end_point_identifier;
    uint16_t profile_identifier;
    uint16_t device_identifier;


     struct {
        /* Device version: it specifies the version of the device description
            supported by the sub-device.
            ZLL spec version 1.0 ,device ver 0x02 */
        uint8_t  device_version : 4;
        /* Reserved */
        uint8_t  reserved : 4;
    }__attribute__((__packed__))version_t ;
    uint8_t group_identifier_count;
} __attribute__((__packed__))Scan_response_field;

/* reference type for storing the scan responses */
typedef struct ZLL_scan_response_stored_Tag{
    Scan_response_field scan_response;
    BOOL scan_entryValid;
    uint8_t src_long_address[8];
}__attribute__((__packed__))ZLL_scan_response_stored_t;


typedef struct Device_information_record {
    uint8_t IEEE_address[8];
    uint8_t end_point_identifier;
    uint8_t profile_identifier[2];
    uint8_t device_identifier[2];
    struct {
        /* Device version: it specifies the version of the device description
            supported by the sub-device.
            ZLL spec version 1.0 ,device ver 0x02 */
        uint8_t  device_version : 4;
        /* Reserved */
        uint8_t  reserved : 4;
    }__attribute__((__packed__))version_t ;
    uint8_t group_identifier_count;
    uint8_t sort;
} __attribute__((__packed__))Device_information_record;

typedef struct Device_information_response_field {
    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;
    uint8_t no_of_subdevices;
    uint8_t start_index;
    uint8_t device_information_record_count;
    /* Maximum so many devices*/
    Device_information_record device_information_record[g_ZLL_NUMBER_OF_ENDPOINTS_c];
} __attribute__((__packed__))Device_information_response_field ;


typedef struct Network_start_response_field {
    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;
    uint8_t status;
    uint8_t extended_pan_identifier[8];
    uint8_t network_update_identifier;
    uint8_t logical_channel;
    uint16_t pan_identifier;
} Network_start_response_field ;
#pragma pack()


typedef struct Network_join_response_field {
    /* transaction identifier field is 32-bits in length and specifies an
    identifier for the inter-5 PAN transaction. This value shall be identical
    to the inter-PAN transaction identifier field of the original scan request
    inter-PAN command frame sent by the initiator.*/
    uint32_t inter_pan_transaction_identifier;
    uint8_t status;

} __attribute__((__packed__))Network_join_response_field ;


/* part of General Interpan frame format */
typedef struct ZLL_MAC_header_Field_Tag{
    uint8_t frame_control[8];
    uint8_t sequence;
    uint8_t dest_PANID[2];
    /* 0xffff If broadcast*/
    uint8_t dest_address[8];
    uint8_t src_PANID[2];
    uint8_t src_address[8];
}__attribute__((__packed__))ZLL_MAC_header_Field_t;


/* part of General Interpan frame format */
typedef struct ZLL_NWK_header_Field_Tag{
    uint8_t frame_control[2];
}__attribute__((__packed__))ZLL_NWK_header_Field_t;

/* part of General Interpan frame format */
typedef struct ZLL_APS_header_Field_Tag{
    uint8_t frame_control;
    uint8_t clusterId[2];
    uint8_t profileId[2];
}__attribute__((__packed__))ZLL_APS_header_Field_t;

/* part of General Interpan frame format */
typedef struct ZLL_ZCL_header_Field_Tag{
    /* Using bit Field*/
    struct frame_Control_Tag{
        /* 2 bits Frametype  = 0b01 cluster specific */
        uint8_t frame_type:2;

        /* 1 bit manufacturer_specific field  = 0 when using Zll
        commissioning cluster.If this value is set to 0, the manufacturer code
        field shall not be included in the ZCL frame */
        uint8_t manufacturer_specific:1;

        /* 1 bit : 0 (client to server ) 1(server to client)*/
        uint8_t direction:1;

        /* 1 bit Disable default response = 1 */
        uint8_t disable_default_response:1;
        /* 3 bit */
        uint8_t reserved:3;

    }__attribute__((__packed__))frame_Control_t;
    /* Manufacture code is not used and in frame control manufacturer_specific
    is set to zero*/
    //uint8_t manufacture_code[2];
    /* Incremented for every transaction of a command */
    uint8_t transaction_seq_number;
    /* ZCL Command identifier field: it specifies the inter-PAN command
    contained in the payload field*/
    uint8_t commandId;
}__attribute__((__packed__))ZLL_ZCL_header_Field_t;

/* part of General Interpan frame format */
typedef struct ZLL_MAC_footer_Field_Tag{
    uint8_t frame_check_sequence[2];
}__attribute__((__packed__))ZLL_MAC_footer_Field_t;

/* General Inter-pan format*/
typedef struct ZLL_General_Inter_PAN_Format_Tag {
   // ZLL_MAC_header_Field_t zll_mac_header_field;
   // ZLL_NWK_header_Field_t zll_nwk_header_field;
   // ZLL_APS_header_Field_t zll_aps_header_field;
    ZLL_ZCL_header_Field_t zll_zcl_header_field;
    union {
        /* Request Command fields*/
        Scan_request_record_list_field scan_request;
        Device_information_record_list_field   device_information_request;
        Identify_record_list_field   identify_request;
        Reset_to_factory_new_record_list_field   reset_to_factory_new_request;
        Network_start_record_list_field   network_start_request;
        Network_join_record_list_field   network_join_request;
        Network_update_record_list_field   network_update_request;


        /* Response Command fields*/
        Scan_response_field   scan_response_field;
        Device_information_response_field   device_information_response_field;
        Network_start_response_field   network_start_response_field;
        Network_join_response_field   network_join_response_field;


    }payload;
    //ZLL_MAC_footer_Field_t zll_mac_footer_field;
}__attribute__((__packed__))ZLL_General_Inter_PAN_Format_t;



/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

uint8_t ST_RadioSetPowerMode(uint16_t txPowerMode);

/*****************************************************************************
* Public Functions Prototype
*****************************************************************************/
void ZLL_Create_Scan_Request_Command(void);
void ZLL_Create_Scan_Response_Command(uint8_t *pDestExtAddress,uint32_t interPANTransactionId, BOOL Initiator);
void ZLL_Create_Device_Information_Request_Command(uint8_t *pDestExtAddress);
void ZLL_Create_Identify_Request(uint8_t *pDestExtAddress);
void ZLL_Create_Reset_To_Factor_New_Request(uint8_t *pDestExtAddress);
void ZLL_Create_Network_Start_Request(uint8_t*pDestExtAddress);
void ZLL_Create_Device_Information_Response_Command(uint8_t *pDestExtAddress,
                                                    uint8_t startIndex);
void ZLL_TriggerStartRequest( void );
void ZLL_AddScanResponseEntry( Scan_response_field *pScanResponse,
                              uint8_t *pSrcIEEEAddr);
uint8_t ZLL_NextJoiningDevice( void );
void ZLL_Create_Network_Join_Request(uint8_t*pDestExtAddress, uint8_t deviceType );
void ZLL_Create_Network_Join_Response(uint8_t*pDestExtAddress,
                                      uint8_t Command_id, uint8_t status);
uint8_t ZLL_IsTheDevice_AddrAssignmentCapable( void );
uint8_t ZLL_IsTheDevice_HasGroups( void );
void ZLL_Create_Network_Start_Response(uint8_t*pDestExtAddress, uint8_t status);
void ZLL_Expand_EncryptTransportKey( uint32_t requestId, uint32_t ResponseId,
              uint8_t *pExpandedOutput, uint8_t keyIndex);
/*****************************************************************************
* Private Functions
*****************************************************************************/

/* None */

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */



#endif /*_ZLL_INTERPAN_H_*/
/*****************************************************************************
* End Of File                                       ZLL_Interpan.h
*****************************************************************************/
