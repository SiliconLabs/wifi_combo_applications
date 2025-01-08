/** @file mac_interface.h
 *
 * @brief  This file provides interface to the MAC module
 *
 */

#ifndef MAC_INTERFACE_H
#define MAC_INTERFACE_H

#include "common_includes.h"

/**************************** Public Macro Definitions ***********************/

/** @brief  Defines the max num of PAN descriptors supported */
#define g_MAX_PAN_DESC_SUPPORTED_c  6

/** @brief  Defines the max num of ED scan results supported */
#define g_MAX_ED_SCAN_RESULTS_SUPPORTED_c   16

#define g_SIZE_LONG_INT_c                   4
#define NOBUFFER			                255
#define g_LONG_ADDRESS_LENGTH_c             8

#define g_TX_OPTION_DIRECT_c			    0x00
#define g_TX_OPTION_ACK_c	  			    0x01
#define g_TX_OPTION_INDIRECT_c		        0x04
#define g_TX_OPTION_INDIRECT_WITH_ACK_c		0x05
#define g_aResponse_Time_c                  0x01EB
#define g_SWITCH_OFF_RECIEVER_c             0x00
#define g_SWITCH_ON_RECIEVER_c              0x01
/**************************** Public Type Definitions ************************/

/** @brief Defines the structure for MCPS DATA Request which will be used for
 MAC data related requests from the application */
typedef struct MCPS_Data_Request_Tag {
    /*! Message type of MCPS Data Request */
    uint8_t msg_type;

    /*! Source addressing mode used */
    uint8_t src_addr_mode;

    /*! Destination addressing mode used */
    uint8_t dst_addr_mode;

    /*! Destination PAN ID  */
    uint8_t a_dst_pan_id[2];

    /*! Destination address */
    Addr_t dst_addr;

    /*! The handle associated with the MSDU to be transmitted  */
    uint8_t msdu_handle;

    /*! The transmission options for the MSDU */
    uint8_t tx_options;

    /*! The security level to be used */
    uint8_t security_level;

    /*! The mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! The originator of the key to be used */
    uint8_t a_key_source[8];

    /*! The index of the key to be used */
    uint8_t key_index;

    /*! The number of octets contained in the MSDU */
    uint8_t msdu_length;

    /*! Reserved for LMAC */
    uint8_t a_reserved[23];

    /*! The set of octets forming the MSDU */
    uint8_t a_msdu[1];
}__attribute__((__packed__))MCPS_Data_Request_t;

/******************************************************************************/

/** @brief Defines the structure for MCPS DATA Confirm which will be used for
 reporting the results of MAC data related requests from the
 application */
typedef struct MCPS_Data_Confirm_Tag {
    /*! Message type of MCPS Data Confirm */
    uint8_t msg_type;

    /*! The status of the last MSDU transmission */
    uint8_t status;

    /*! The handle associated with the MSDU being confirmed */
    uint8_t msdu_handle;

    /*! The time, in symbols, at which the data were transmitted */
    uint8_t a_time_stamp[4];
} __attribute__((__packed__))MCPS_Data_Confirm_t;

/******************************************************************************/
/** @brief Defines the structure for MCPS DATA Indication which will be used
 for indicating the transfer of a data packet by the MAC */
typedef struct MCPS_Data_Indication_Tag {
    /*! Message type of MCPS Data Indication */
    uint8_t msg_type;

    /*! Source addressing mode used */
    uint8_t src_addr_mode;

    /*! Source PAN ID  */
    uint8_t a_src_pan_id[2];

    /*! Source address */
    Addr_t src_addr;

    /*! Destination addressing mode used */
    uint8_t dst_addr_mode;

    /*! Destination PAN ID  */
    uint8_t a_dst_pan_id[2];

    /*! LQI value measured during reception of the MPDU */
    uint8_t mpdu_link_quality;

    /*! Destination address */
    Addr_t dst_addr;

    /*! The data sequence number of the received data frame */
    uint8_t dsn;

    /*! The time, in symbols, at which the data were received */
    uint8_t a_time_stamp[4];

    /*! The security level purportedly used by the received data frame */
    uint8_t security_level;

    /*! The mode used to identify the key */
    uint8_t key_id_mode;

    /*! The originator of the key */
    uint8_t a_key_source[8];

    /*! The index of the key */
    uint8_t key_index;

    /*! The number of octets contained in the MSDU being indicated */
    uint8_t msdu_length;

    /*! Reserved for LMAC */
    uint8_t a_reserved[9];

    /*! The set of octets forming the MSDU being indicated */
    uint8_t a_msdu[1];
}__attribute__((__packed__))MCPS_Data_Indication_t;

/******************************************************************************/

/** @brief Defines the structure for MCPS PURGE Request which will be used by
 the application to purge an MSDU from the transaction
 queue */
typedef struct MCPS_Purge_Request_Tag {
    /*! Message type of MCPS Purge Request*/
    uint8_t msg_type;

    /*! The handle associated with the MSDU to be purged from the transaction
     queue */
    uint8_t msdu_handle;
} __attribute__((__packed__))MCPS_Purge_Request_t;

/******************************************************************************/

/** @brief Defines the structure for MCPS PURGE Confirm which will be used by
 the  MAC to notify the application of the status of its request
 to purge an MSDU from the transaction queue */
typedef struct MCPS_Purge_Confirm_Tag {
    /*!  Message type of MCPS Purge Confirm */
    uint8_t msg_type;

    /*! The status of the request */
    uint8_t status;

    /*! Handle associated with the MSDU requested to be purged from the
     transaction queue */
    uint8_t msdu_handle;

} __attribute__((__packed__))MCPS_Purge_Confirm_t;

/******************************************************************************/

/** @brief Defines the structure for  MLME ASSOCIATE Request which will be used
 by the application to request an association */
typedef struct MLME_Associate_Request_Tag {
    /*! Message type of MLME Associate Request */
    uint8_t msg_type;

    /*! The logical channel on which to attempt association */
    uint8_t logical_channel;

    /*! The channel page on which to attempt association */
    uint8_t channel_page;

    /*! Coordinator addressing mode used */
    uint8_t coord_addr_mode;

    /*! The identifier of the PAN with which to associate*/
    uint8_t a_coord_pan_id[2];

    /*! Coordinator address */
    Addr_t coord_addr;

    /*! Operational capabilities of the associating device */
    uint8_t capability_info;

    /*! The security level to be used */
    uint8_t security_level;

    /*! The mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! The originator of the key to be used */
    uint8_t a_key_source[8];

    /*! The index of the key to be used */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Associate_Request_t;

/******************************************************************************/

/** @brief Defines the structure for MLME ASSOCIATE Indication which will be
 used by the MAC to indicate the reception of an association request
 command */
typedef struct MLME_Associate_Indication_Tag {
    /*!  Message type of MLME Associate Indication */
    uint8_t msg_type;

    /*! Extended address of the device requesting association */
    uint8_t a_device_addr[8];

    /*! Operational capabilities of the device requesting association */
    uint8_t capability_info;

    /*! Security level purportedly used by the received MAC command frame*/
    uint8_t security_level;

    /*! The mode used to identify the key used by the originator of frame */
    uint8_t key_id_mode;

    /*! The originator of the key used by the originator of the received frame */
    uint8_t a_key_source[8];

    /*! Index of the key used by the originator of the received frame */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Associate_Indication_t;

/******************************************************************************/

/** @brief  Defines the structure for MLME ASSOCIATE Response which is used to
 initiate a response to an MLME-ASSOCIATE.indication*/
typedef struct MLME_ASSOCIATE_Response_Tag {
    /*! Message type of MLME Associate Response */
    uint8_t msg_type;

    /*! Extended address of the device requesting association */
    uint8_t a_device_addr[8];

    /*! 16-bit short device address allocated by the coordinator on successful
     association */
    uint8_t a_assoc_short_addr[2];

    /*! Status of the association attempt */
    uint8_t status;

    /*! Security level to be used */
    uint8_t security_level;

    /*! The mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! The originator of the key to be used */
    uint8_t a_key_source[8];

    /*! The index of the key to be used */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Associate_Response_t;

/******************************************************************************/

/** @brief Defines a structure for MLME ASSOCIATE Confirm which is used to
 inform the application of the initiating device whether its
 request to associate was successful or unsuccessful */
typedef struct MLME_Associate_Confirm_Tag {
    /*!  Message type of MLME Associate Confirm */
    uint8_t msg_type;

    /*! Status of the association attempt*/
    uint8_t status;

    /*! Short address allocated by the coordinator on successful association */
    uint8_t a_assoc_short_addr[2];

    /*! The security level used */
    uint8_t security_level;

    /*! The mode used to identify the key */
    uint8_t key_id_mode;

    /*! The originator of the key */
    uint8_t a_key_source[8];

    /*! The index of the key */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Associate_Confirm_t;

/******************************************************************************/

/** @brief Defines the structure for  MLME DISASSOCIATE Request which will be
 used by the application to request an disassociation */
typedef struct MLME_Disassociate_Request_Tag {
    /*! Message type of MLME Associate Request */
    uint8_t msg_type;

    /*! Device addressing mode used */
    uint8_t device_addr_mode;

    /*! The identifier of the PAN of the device */
    uint8_t device_pan_id[2];

    /*! Device address */
    Addr_t device_addr;

    /*! The reason for the disassociation */
    uint8_t disassociate_reason;

    /*! TRUE if the disassociation notification command is to be sent indirectly
     */
    Bool_t tx_indirect;

    /*! The security level to be used */
    uint8_t security_level;

    /*! The mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! The originator of the key to be used */
    uint8_t a_key_source[8];

    /*! The index of the key to be used */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Disassociate_Request_t;

/******************************************************************************/
/** @brief Defines the structure for  MLME DISASSOCIATE indication which will be
 used to send disassociation indication to the application. */
typedef struct MLME_Disassociate_Indication_Tag {
    /*! Message type of MLME Associate Request */
    uint8_t msg_type;

    /*! Device address of the device requesting the disassociation*/
    Addr_t device_addr;

    /*! The reason for the disassociation */
    uint8_t disassociate_reason;

    /*! The security level to be used */
    uint8_t security_level;

    /*! The mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! The originator of the key to be used */
    uint8_t a_key_source[8];

    /*! The index of the key to be used */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Disassociate_Indication_t;

/******************************************************************************/

/** @brief Defines the structure for  MLME DISASSOCIATE Confirm which will be
 used to send disassociation Confirmation to the application. */
typedef struct MLME_Disassociate_Confirm_Tag {
    /*! Message type of MLME Associate Request */
    uint8_t msg_type;

    /*! Status of the disassociation attempt*/
    uint8_t status;

    /*! Device addressing mode used */
    uint8_t device_addr_mode;

    /*! The identifier of the PAN of the device */
    uint8_t device_pan_id[2];

    /*! Device address */
    Addr_t device_addr;

} __attribute__((__packed__))MLME_Disassociate_Confirm_t;

/******************************************************************************/
/** @brief  Defines a structure for MAC PAN Descriptor which contains the
 network details of the device from which the beacon is received */
typedef struct MAC_PAN_Descriptor_Tag {
    /*! Coordinator addressing mode */
    uint8_t coord_addr_mode;

    /*! PAN identifier of the coordinator */
    uint8_t a_coord_pan_id[2];

    /*! Coordinator address */
    Addr_t coord_addr;

    /*! The current logical channel occupied by the network */
    uint8_t logical_channel;

    /*! The current channel page occupied by the network */
    uint8_t channel_page;

    /*! Superframe specification as specified in the received beacon frame */
    uint8_t a_superframe_spec[2];

    /*! PAN coordinator is accepting GTS requests or not */
    uint8_t gts_permit;

    /*! The LQI at which the network beacon was received */
    uint8_t link_quality;

    /*! The time at which the beacon frame was received, in symbols */
    uint8_t a_time_stamp[4];

    /*! Security level purportedly used by the received beacon frame */
    uint8_t security_level;

} __attribute__((__packed__))MAC_PAN_Descriptor_t;

/******************************************************************************/
/** @brief   Defines a structure for MLME BEACON NOTIIFY Indication which is
 used to send parameters contained within a beacon frame received
 by the MAC to the application */
typedef struct MLME_Beacon_Notify_Indication_Tag {
    /*! Message type of MLME Beacon Notify Indication */
    uint8_t msg_type;

    /*! Beacon Sequence Number */
    uint8_t bsn;

    /*! The PAN Descriptor for the received beacon */
    MAC_PAN_Descriptor_t mac_pan_descriptor;

    /*! The beacon pending address specification */
    uint8_t pending_addr_spec;

    /*! Number of octets contained in the beacon payload of the beacon frame */
    uint8_t sdu_length;

    /*! Set of octets comprising the beacon payload & the list of addresses of
     the devices forwhich the beacon source has data */
    uint8_t a_bcn_data[1];
} __attribute__((__packed__))MLME_Beacon_Notify_Indication_t;

/******************************************************************************/
/** @brief   Defines the structure for MLME ORPHAN Indication which is used by
 the coordinator to notify the application of the presence of
 an orphaned device */
typedef struct MLME_Orphan_Indication_Tag {
    /*! Message type of MLME Orphan Indication */
    uint8_t msg_type;

    /*! Extended address of the orphaned device */
    uint8_t a_orphan_addr[8];

    /*! Security level purportedly used by the received MAC command frame */
    uint8_t security_level;

    /*! Mode used to identify the key used by originator of received frame */
    uint8_t key_id_mode;

    /*! Originator of the key used by the originator of the received frame */
    uint8_t a_key_source[8];

    /*! Index of the key used by the originator of the received frame */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Orphan_Indication_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME ORPHAN Response which is used by the
 application layer of the coordinator to respond to the MLME ORPHAN
 Indication */
typedef struct MLME_Orphan_Response_Tag {
    /*! Message type of MLME Orphan Response */
    uint8_t msg_type;

    /*! Extended address of the orphaned device */
    uint8_t a_orphan_addr[8];

    /*! Short address allocated to the orphaned device */
    uint8_t a_short_addr[2];

    /*! If the orphaned device is associated with the coordinator or not */
    uint8_t assoc_member;

    /*! Security level to be used */
    uint8_t security_level;

    /*! Mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! Originator of the key to be used */
    uint8_t a_key_source[8];

    /*! Index of the key to be used */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Orphan_Response_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME RESET Request which allows the
 application to request that the MLME of the MAC layer performs a
 reset operation */
typedef struct MLME_Reset_Request_Tag {
    /*! Message type of MLME Reset Request */
    uint8_t msg_type;

    /*! MAC PIB attributes are set to their default values or not during reset */
    uint8_t set_default_pib;
} __attribute__((__packed__))MLME_Reset_Request_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME RESET Confirm which is used to report
 the results of the reset operation */
typedef struct MLME_Reset_Confirm_Tag {
    /*! Message type of MLME Reset Confirm */
    uint8_t msg_type;

    /*! The result of the reset operation */
    uint8_t status;
} __attribute__((__packed__))MLME_Reset_Confirm_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME RX ENABLE Request which allows the
 application to request that the receiver is either enabled
 for a finite period of time or disabled */
typedef struct MLME_RX_Enable_Request_Tag {
    /*! Message type of MLME RX Enable Request */
    uint8_t msg_type;

    /*! The requested operation can be deferred or not */
    uint8_t defer_permit;

    /*! Number of symbols measured before the receiver is to be enabled or
     disabled */
    uint8_t a_rx_on_time[4];

    /*! Number of symbols for which the receiver is to be enabled */
    uint8_t a_rx_on_duration[4];
} __attribute__((__packed__))MLME_RX_Enable_Request_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME RX ENABLE Confirm which is used to
 report the results of the attempt to enable or disable the receiver*/
typedef struct MLME_RX_Enable_Confirm_Tag {
    /*! Message type of MLME RX Enable Confirm */
    uint8_t msg_type;

    /*! Result of the request to enable or disable the receiver */
    uint8_t status;
} __attribute__((__packed__))MLME_RX_Enable_Confirm_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME SCAN Request which is used to
 initiate a channel scan over a given list of channels */
typedef struct MLME_Scan_Request_Tag {
    /*! Message type of MLME Scan Request */
    uint8_t msg_type;

    /*! The type of scan to be performed */
    uint8_t scan_type;

    /*! Indicate which channels are to be scanned */
    uint8_t a_scan_channels[4];

    /*! The time spent on scanning each channel */
    uint8_t scan_duration;

    /*! Channel page on which to perform the scan */
    uint8_t channel_page;

    /*! Security level to be used */
    uint8_t security_level;

    /*! Mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! Originator of the key to be used */
    uint8_t a_key_source[8];

    /*! Index of the key to be used */
    uint8_t key_index;
} __attribute__((__packed__))MLME_Scan_Request_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME SCAN Confirm which is used to report
 the result of the channel scan request */
typedef struct MLME_Scan_Confirm_Tag {
    /*! Message type of MLME Scan Confirm */
    uint8_t msg_type;

    /*! Status of the scan request */
    uint8_t status;

    /*! The type of scan performed */
    uint8_t scan_type;

    /*! Channel page on which the scan was performed */
    uint8_t channel_page;

    /*! Channels given in the request which were not scanned */
    uint8_t a_unscanned_channel[4];

    /*! Number of elements returned in the appropriate result lists */
    uint8_t result_list_size;

    /*! Defines a union of energy measurements list and pan descriptor list */
    union {
        /*! List of energy measurements */
        uint8_t energy_detect_list[g_MAX_ED_SCAN_RESULTS_SUPPORTED_c];

        /*! List of PAN descriptors */
        MAC_PAN_Descriptor_t pan_desc_list[g_MAX_PAN_DESC_SUPPORTED_c];

    } result_list;

} __attribute__((__packed__))MLME_Scan_Confirm_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME COMM STATUS Indication which is used
 by the MAC to indicate a communications status */
typedef struct MLME_Comm_Status_Indication_Tag {
    /*! Message type of MLME Comm Status Indication */
    uint8_t msg_type;

    /*! The 16-bit PAN identifier of the device from which the frame was
     received or to which the frame was being sent */
    uint8_t a_pan_id[2];

    /*! Source addressing mode */
    uint8_t src_addr_mode;

    /*! Source address */
    Addr_t src_addr;

    /*! Destination addressing mode */
    uint8_t dst_addr_mode;

    /*! Destination address */
    Addr_t dst_addr;

    /*! The communications status */
    uint8_t status;

    /*! Security level to be used */
    uint8_t security_level;

    /*! Mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! Originator of the key to be used */
    uint8_t a_key_source[8];

    /*! Index of the key to be used */
    uint8_t key_index;
}__attribute__((__packed__))MLME_Comm_Status_Indication_t;

/******************************************************************************/
/** @brief Defines the structure for MLME START Request which is used by the
 FFDs to initiate a new PAN or to begin using a new superframe
 configuration */
typedef struct MLME_Start_Request_Tag {
    /*! Message type of MLME Start Request */
    uint8_t msg_type;

    /*! PAN identifier to be used by the device */
    uint8_t a_pan_id[2];

    /*! Logical channel on which to begin */
    uint8_t logical_channel;

    /*! Channel page on which to begin */
    uint8_t channel_page;

    /*! Time at which to begin transmitting beacons */
    uint8_t a_start_time[4];

    /*! Indicates how often the beacon is to be transmitted */
    uint8_t beacon_order;

    /*! Length of the active portion of the superframe */
    uint8_t superframe_order;

    /*! Indicates whether the device is a PAN coordinator or not */
    uint8_t pan_coord;

    /*! Indicates if the receiver of the beaconing device is disabled or not */
    uint8_t battery_life_ext;

    /*! Indicates if the coordinator realignment command is to be transmitted */
    uint8_t coord_realignmnt;

    /*! Security level to be used for coordinator realignment command frames */
    uint8_t coord_realign_security_level;

    /*! Mode used to identify the key to be used */
    uint8_t coord_realign_key_id_mode;

    /*! Originator of the key to be used*/
    uint8_t a_coord_realign_key_source[8];

    /*! Index of the key to be used */
    uint8_t coord_realign_key_index;

    /*! Security level to be used for beacon frames */
    uint8_t beacon_security_level;

    /*! Mode used to identify the key to be used */
    uint8_t beacon_key_id_mode;

    /*! Originator of the key to be used */
    uint8_t a_beacon_key_source[8];

    /*! Index of the key to be used */
    uint8_t beacon_key_index;
} __attribute__((__packed__))MLME_Start_Request_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME START Confirm which is used to
 report the results of the attempt to start using a new superframe
 configuration */
typedef struct MLME_Start_Confirm_Tag {
    /*! Message type of MLME Start Confirm */
    uint8_t msg_type;

    /*!Result of the attempt to start using an updated superframe configuration*/
    uint8_t status;
} __attribute__((__packed__))MLME_Start_Confirm_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME SYNC LOSS Indication which is used
 by the MAC to indicate the loss of synchronization with the
 coordinator */
typedef struct MLME_Sync_Loss_Indication_Tag {
    /*! Message type of MLME Sync Loss Indication */
    uint8_t msg_type;

    /*! The reason that synchronization was lost */
    uint8_t loss_reason;

    /*! The PAN identifier with which the device lost synchronization or to
     which it was realigned */
    uint8_t a_pan_id[2];

    /*! The logical channel on which the device lost synchronization or to which
     it was realigned */
    uint8_t logical_channel;

    /*! The channel page on which the device lost synchronization or to which it
     was realigned */
    uint8_t channel_page;

    /*! The security level used by the received MAC frame */
    uint8_t security_level;
} __attribute__((__packed__))MLME_Sync_Loss_Indication_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME POLL Request which prompts the
 device to request data from the coordinator */
typedef struct MLME_Poll_Request_Tag {
    /*! Message type of MLME Poll Request */
    uint8_t msg_type;

    /*! Addressing mode of the coordinator */
    uint8_t coord_addr_mode;

    /*! PAN identifier of the coordinator */
    uint8_t a_coord_pan_id[2];

    /*! Coordinator address */
    Addr_t coord_addr;

    /*! Security level to be used */
    uint8_t security_level;

    /*! Mode used to identify the key to be used */
    uint8_t key_id_mode;

    /*! Originator of the key to be used */
    uint8_t a_key_source[8];

    /*! Index of the key to be used*/
    uint8_t key_index;
} __attribute__((__packed__))MLME_Poll_Request_t;

/******************************************************************************/
/** @brief  Defines the structure for MLME POLL Confirm which is used to report
 the result of a request to poll the coordinator for data */
typedef struct MLME_Poll_Confirm_Tag {
    /*! Message type of MLME Poll Confirm */
    uint8_t msg_type;

    /*! The status of the data request */
    uint8_t status;
} __attribute__((__packed__))MLME_Poll_Confirm_t;

/**************************** Public Enum Definitions ************************/

/** @brief   Message ID of MAC request/response/indication/confirmation */
typedef enum MAC_Message_ID_Tag {
    /*! -MCPS Data Request */
    g_MCPS_DATA_REQUEST_c = (uint8_t) 0x40,

    /*! -MCPS Data Confirm */
    g_MCPS_DATA_CONFIRM_c = (uint8_t) 0x41,

    /*! -MCPS Data Indication */
    g_MCPS_DATA_INDICATION_c = (uint8_t) 0x42,

    /*! -MCPS Purge request */
    g_MCPS_PURGE_REQUEST_c = (uint8_t) 0x43,

    /*! -MCPS Purge Confirm */
    g_MCPS_PURGE_CONFIRM_c = (uint8_t) 0x44,

    /*! -MLME Associate Request */
    g_MLME_ASSOCIATE_REQUEST_c = (uint8_t) 0x45,

    /*! -MLME Associate Confirm */
    g_MLME_ASSOCIATE_CONFIRM_c = (uint8_t) 0x46,

    /*! -MLME Associate Indication */
    g_MLME_ASSOCIATE_INDICATION_c = (uint8_t) 0x47,

    /*! -MLME Associate Response */
    g_MLME_ASSOCIATE_RESPONSE_c = (uint8_t) 0x48,

    /*! -MLME Disassociate Request */
    g_MLME_DISASSOCIATE_REQUEST_c,

    /*! -MLME Disassociate Confirm */
    g_MLME_DISASSOCIATE_CONFIRM_c,

    /*! -MLME Disassociate Indication */
    g_MLME_DISASSOCIATE_INDICATION_c,

    /*! -MLME Beacon Notify Indication */
    g_MLME_BEACON_NOTIFY_INDICATION_c,

    /*! -MLME Get Request */
    g_MLME_GET_REQUEST_c,

    /*! -MLME Get Confirm */
    g_MLME_GET_CONFIRM_c,

    /*! -MLME Orphan Indication */
    g_MLME_ORPHAN_INDICATION_c,

    /*! -MLME Orphan Response */
    g_MLME_ORPHAN_RESPONSE_c,

    /*! -MLME Reset Request */
    g_MLME_RESET_REQUEST_c,

    /*! -MLME Reset Confirm */
    g_MLME_RESET_CONFIRM_c,

    /*! -MLME RX Enable Request */
    g_MLME_RX_ENABLE_REQUEST_c,

    /*! -MLME RX Enable Confirm */
    g_MLME_RX_ENABLE_CONFIRM_c,

    /*! -MLME Scan Request */
    g_MLME_SCAN_REQUEST_c,

    /*! -MLME Scan Confirm */
    g_MLME_SCAN_CONFIRM_c,

    /*! -MLME Comm Status Indication */
    g_MLME_COMM_STATUS_INDICATION_c,

    /*! -MLME Set Request */
    g_MLME_SET_REQUEST_c,

    /*! -MLME Set Confirm */
    g_MLME_SET_CONFIRM_c,

    /*! -MLME Start Request */
    g_MLME_START_REQUEST_c,

    /*! -MLME Start Confirm */
    g_MLME_START_CONFIRM_c,

    /*! -MLME Sync Loss Indication */
    g_MLME_SYNC_LOSS_INDICATION_c,

    /*! -MLME Poll Request */
    g_MLME_POLL_REQUEST_c,

    /*! -MLME Poll Confirm */
    g_MLME_POLL_CONFIRM_c
} MAC_Message_ID_t;

/******************************************************************************/

/** @brief MAC Transmission Options */
typedef enum MAC_Tx_Options_Tag {
    /*! -Acknowledged Transmission */
    g_ACK_TX_c = (uint8_t) 0x01,

    /*! -GTS Transmission */
    g_GTS_TX_c = (uint8_t) 0x02,

    /*! -Indirect Transmission */
    g_INDIRECT_TX_c = (uint8_t) 0x04,

    /*! -Security Enabled Transmission */
    g_SECURED_TX_c = (uint8_t) 0x08
} MAC_Tx_Options_t;

/******************************************************************************/
/** @brief MAC Association Status */
typedef enum MAC_Association_Status_Tag {
    /*! -Association successful */
    g_MAC_ASSO_SUCCESS_c = (uint8_t) 0x00,

    /*! -PAN at capacity */
    g_MAC_PAN_AT_CAPACITY_c = (uint8_t) 0x01,

    /*! -PAN access denied */
    g_MAC_PAN_ACCESS_DENIED_c = (uint8_t) 0x02
} MAC_Association_Status_t;

/******************************************************************************/
/** @brief MAC Disassociation reason codes */
typedef enum MAC_Disassociation_Reason_Tag {
    /*! -The coordinator wishes the device to leave the PAN. */
    g_COORD_REQUESTED_c = (uint8_t) 0x01,

    /*! -The device wishes to leave the PAN. */
    g_DEVICE_REQUESTED_c = (uint8_t) 0x02

} MAC_Disassociation_Reason_t;

/******************************************************************************/
/** @brief MAC Security Mode */
typedef enum MAC_Security_Mode_Tag {
    /*! - MAC Unsecured Mode Security */
    g_MAC_UNSECURED_MODE_c = (uint8_t) 0x00,

    /*! - MAC ACL Mode Security */
    g_MAC_ACL_MODE_c = (uint8_t) 0x01,

    /*! - MAC Secured Mode Security */
    g_MAC_SECURED_MODE_c = (uint8_t) 0x02
} MAC_Security_Mode_t;

/**
 * The maximum size of an MPDU, in octets, that can be
 * followed by a short interframe spacing (SIFS) period.
 * @ingroup apiMacConst
 */
#define aMaxSIFSFrameSize               (18)

/******************************************************************************/
/** @brief This enum contains all the mac pib Ids */
typedef enum MAC_Pib_Ids_Tag {
    /*! -Id of mac attribute mac_ack_wait_duration */
    g_MAC_ACK_WAIT_DURATION_c = (uint8_t) 0x40,

    /*! -Id of mac attribute mac_association_permit */
    g_MAC_ASSOCIATION_PERMIT_c = (uint8_t) 0x41,

    /*! -Id of mac attribute mac_auto_request */
    g_MAC_AUTO_REQUEST_c = (uint8_t) 0x42,

    /*! -Id of mac attribute mac_beacon_payload */
    g_MAC_BEACON_PAYLOAD_c = (uint8_t) 0x45,

    /*! -Id of mac attribute mac_beacon_payload_length */
    g_MAC_BEACON_PAYLOAD_LENGTH_c = (uint8_t) 0x46,

    /*! -Id of mac attribute mac_beacon_order */
    g_MAC_BEACON_ORDER_c = (uint8_t) 0x47,

    /*! -Id of mac attribute mac_bsn */
    g_MAC_BSN_c = (uint8_t) 0x49,

    /*! -Id of mac attribute mac_coord_extended_address */
    g_MAC_COORD_EXTENDED_ADDDRESS_c = (uint8_t) 0x4A,

    /*! -Id of mac attribute mac_coord_short_address */
    g_MAC_COORD_SHORT_ADDRESS_c = (uint8_t) 0x4B,

    /*! -Id of mac attribute mac_dsn */
    g_MAC_DSN_c = (uint8_t) 0x4C,

    /*! -Id of mac attribute mac_max_frame_total_wait_time */
    g_MAC_MAX_FRAME_TOTAL_WAIT_TIME_c = (uint8_t) 0x58,

    /*! -Id of mac attribute mac_max_frame_retries */
    g_MAC_MAX_FRAME_RETRIES_c = (uint8_t) 0x59,

    /*! -Id of mac attribute mac_pan_id */
    g_MAC_PAN_ID_c = (uint8_t) 0x50,

    /*! -Id of mac attribute mac_response_wait_time */
    g_MAC_RESPONSE_WAIT_TIME_c = (uint8_t) 0x5A,

    /*! -Id of mac attribute mac_rx_on_when_idle */
    g_MAC_RX_ON_WHEN_IDLE_c = (uint8_t) 0x52,

    /*! -Id of mac attribute mac_security_enabled */
    g_MAC_SECURITY_ENABLED_c = (uint8_t) 0x5D,

    /*! -Id of mac attribute mac_short_address */
    g_MAC_SHORT_ADDRESS_c = (uint8_t) 0x53,

    /*! -Id of mac attribute mac_superframe_order */
    g_MAC_SUPERFRAME_ORDER_c = (uint8_t) 0x54,

    /*! -Id of mac attribute mac_time_stamp_supported */
    g_MAC_TIMESTAMP_SUPPORTED_c = (uint8_t) 0x5C,

    /*! -Id of mac attribute mac_transaction_persistence_time */
    g_MAC_TRANSACTION_PERSISTENCE_TIME_c = (uint8_t) 0x55,

    /*! -Id of mac attribute mac_max_be */
    g_MAC_MAX_BE_c = (uint8_t) 0x57,

    /*! -Id of mac attribute mac_lifs */
    g_MAC_LIFS_PERIOD_c = (uint8_t) 0x5E,

    /*! -Id of mac attribute mac_sifs */
    g_MAC_SIFS_PERIOD_c = (uint8_t) 0x5F,

    /*! -Id of mac attribute mac_max_csma_backoffs */
    g_MAC_MAX_CSMA_BACKOFFS_c = (uint8_t) 0x4E,

    /*! -Id of mac attribute mac_min_be */
    g_MAC_MIN_BE_c = (uint8_t) 0x4F,

    /*! -Id of pan coordinator */
    g_MAC_PAN_COORDINATOR_c = (uint8_t) 0x10,

    /*! -Id of parent is a pan coordinator */
    g_MAC_ASSOC_PAN_COORDINATOR_c = (uint8_t) 0x11,

    /*! -Id of mac extended address */
    g_MAC_EXTENDED_ADDRESS_c = (uint8_t) 0x6F,

    /*! -Id for MAC ACL Entry */
    g_MAC_ACL_ENTRY_DESCRIPTOR_c = (uint8_t) 0x70,

    /*! -Id for No of ACL Security Descriptor Entries */
    g_MAC_ACL_ENTRY_DESCRIPTOR_SIZE_c = (uint8_t) 0x71,

    /*! -Id for MAC Default Security Support */
    g_MAC_DEFAULT_SECURITY_c = (uint8_t) 0x72,

    /*! -Id for MAC Default Security material length */
    g_MAC_DEFAULT_SECURITY_MATERIAL_LENGTH_c = (uint8_t) 0x73,

    /*! -Id for MAC Default Security Material */
    g_MAC_DEFAULT_SECURITY_MATERIAL_c = (uint8_t) 0x74,

    /*! -Id for MAC Default Security Suite */
    g_MAC_DEFAULT_SECURITY_SUITE_c = (uint8_t) 0x75,

    /*! -Id for MAC Security Mode */
    g_MAC_SECURITY_MODE_c = (uint8_t) 0x76,

    /*! - Proprietary Id for Current number of ACL Entries  */
    g_MAC_CURRENT_ACL_ENTRIES_c = (uint8_t) 0x80,

    /*! - Proprietary Id for Default MAC Security Extended Address  */
    g_MAC_DEFAULT_SECURITY_EXTENDED_ADDRESS_c = (uint8_t) 0x81
} MAC_Pib_Ids_t;

/******************************************************************************/
/** @brief This enum indicates the type of scan to be performed */
typedef enum MAC_Scan_Types_Tag {
    /*! -ED Scan */
    g_MAC_ED_SCAN_TYPE_c = (uint8_t) 0x00,

    /*! -Active Scan */
    g_MAC_ACTIVE_SCAN_TYPE_c,

    /*! -Passive Scan */
    g_MAC_PASSIVE_SCAN_TYPE_c,

    /*! -Orphan Scan */
    g_MAC_ORPHAN_SCAN_TYPE_c
} MAC_Scan_Types_t;
/**************************** Public Variable Declaration ********************/
#if(DEBUG_TEST ==1)
extern const uint8_t g_Total_Number_Of_Large_Buffers_c;
#endif //#if(DEBUG_TEST ==1)

/**************************** Public Constant Declaration ********************/

/* None */

/**************************** Public Function Prototypes *********************/
/** @brief This function is used to reset MAC. Called on MLME_reset_req
 *
 * @param set_default_pib: defines Soft or Hard reset.
 *
 * @return Reset status
 */

uint8_t MAC_mainMACReset ( uint8_t set_default_pib );

/******************************************************************************/
/** @brief This function enqueues the MCPS messages from the upper layer to the
 *          MAC MCPS Queue
 *
 * @param buffer_id: Buffer ID allocated for the message
 *
 * @return None
 */
void MAC_NHLEMCPSMsg ( uint8_t buffer_id );

/******************************************************************************/
/** @brief This function enqueues the MLME messages from the upper layer to the
 *          MAC MLME Queue
 *
 * @param buffer_id: Buffer ID allocated for the message
 *
 * @return None
 */

void MAC_NHLEMLMEMsg ( uint8_t buffer_id );

/******************************************************************************/
/** @brief This function implements the Main MAC Task.This function will be
 called in a continuous loop.
 *
 * @return None
 */

void MAC_mainMACTask ( void );

/******************************************************************************/
/** @brief This function implements the Main MAC_LMACTask.This function will be
 called in a continuous loop.
 *
 * @return None
 */

void MAC_LMACTask ( void );

/******************************************************************************/
/** @brief This function is called to initialize MAC resources to their initial
 *          state.
 *
 * @return None
 */

void MAC_mainMACInit ( void );

/******************************************************************************/
/** @brief This is a callback function which gets called for any MCPS indication
 *          or MCPS confirmation messages coming from the MAC
 *
 * @param mcps_buffer_id : Buffer ID of the buffer allocated by the MAC for MCPS
 *                         indication or confirmation messages
 *
 * @return None
 */
void processMCPSMsgCb ( uint8_t mcps_buffer_id );

/******************************************************************************/
/** @brief This is a callback function which gets called for any MLME indication
 *          or MLME confirmation messages coming from the MAC
 *
 * @param mlme_buffer_id : Buffer ID of the buffer allocated by the MAC for MLME
 *                         indication or confirmation messages
 *
 * @return None
 */
void processMLMEMsgCb ( uint8_t mlme_buffer_id );

/******************************************************************************/
/** @brief this function sets value of the mac attribute to the requested value.
 *
 * @param pib_attrib_id: Attribute Id.
 *
 * @param pib_attrib_index: Index of the attribute
 *
 * @param p_pib_attrib_value: Pointer to the value which has to be set in the
 * given attribute.
 *
 * @return Returns the status of the set request.
 */

uint8_t MAC_MLMEPibSet ( uint8_t pib_attrib_id, uint8_t pib_attrib_index,
        uint8_t* p_pib_attrib_value );

/******************************************************************************/
/** @brief this function get the value of the requested mac attribute.
 *
 * @param pib_attrib_id: Attribute Id.
 *
 * @param pib_attrib_index: Index of the attribute
 *
 * @param p_pib_val_len: Pointer to the attribute length .
 *
 * @param p_pib_attrib_value: Pointer to the value which will contain the
 requested mac attribute value .
 *
 * @return Returns the status of the get request.
 */

uint8_t MAC_MLMEPibGet ( uint8_t pib_attrib_id, uint8_t pib_attrib_index,
        uint8_t* p_pib_val_len, uint8_t* p_pib_attrib_value );

/******************************************************************************/
/**
 * @brief perform a silent start
 *
 * @param deviceType - type of the device that is to be started
 * @return - channel in which the device is started
 */
uint8_t MAC_silentStart ( uint8_t deviceType );

/******************************************************************************/
/**
 * @brief perform a rejoin start
 *
 * @param deviceType - type of the device that is to be started
 * @return - channel in which the device is started
 */
uint8_t MAC_rejoinStart ( uint8_t deviceType );

/******************************************************************************/
/**
 * @brief Get the extended Address of the device
 *
 * @param - None
 * @return - pointer to the extended address
 */
uint8_t* MAC_getMACExtendedAddress ( void );

/******************************************************************************/
/**
 * @brief Set the extended address of the device
 *
 * @param pAddr - pointer to the extended address to be set
 * @return - None
 */
void MAC_setMACExtendedAddress ( uint8_t* pAddr );

/******************************************************************************/
/**
 * @brief Get the count of Number of packets successfully transmitted
 *
 * @param - None
 * @return - number of successfully transmitted packets
 */
uint16_t MAC_getTxPacketCount ( void );

/******************************************************************************/
/**
 * @brief Clear the count of Number of packets successfully transmitted
 *
 * @param - None
 * @return - None
 */
void MAC_clearTxPacketCount ( void );

/******************************************************************************/
/**
 * @brief Get the state of the update to NVM flag
 *
 * @param - None
 * @return - g_TRUE_c / g_FALSE_c
 */
uint8_t MAC_getUpdateToNVM ( void );

/******************************************************************************/
/**
 * @brief Set the state of the update to NVM flag
 *
 * @param - g_TRUE_c / g_FALSE_c
 * @return - None
 */
void MAC_setUpdateToNVM ( uint8_t state );


/**
 * check for the idel state of the MAC layer
 */
uint8_t MAC_checkIdleState( void );

/**
 * @brief check for data pending
 *
 * @param id- Packet
 * @return - None
 */
void LMAC_Check_Data_Pending(uint8_t *Packet);

void MAC_SetPhyTRXState(uint8_t trx_state);

void ZigBeeSetScanType(uint8_t newScanType);

#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
//void app_PacketBufferAllocateFailuresHandler(void);
void app_PhyToMACQueueLimitReachedHandler(void);
void app_MACTrxUnicastRetryHandler(void);
void app_MACTrxUnicastFilureHandler(void);
void app_MACTrxBroadcastHandler(void);
void app_MACTrxUnicastHandler(void);

void app_MACRecBroadcastHandler(void);
void app_MACRecUnicastHandler(void);


#endif

/******************************************************************************/
#endif  /*MAC_INTERFACE_H*/
