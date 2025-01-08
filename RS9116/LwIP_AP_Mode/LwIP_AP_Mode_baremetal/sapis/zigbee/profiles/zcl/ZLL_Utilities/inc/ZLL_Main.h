/******************************************************************************
* _ZLL_MAIN_H_
*******************************************************************************
* Program Description:
*
*
******************************************************************************/
#ifndef _ZLL_MAIN_H_
#define _ZLL_MAIN_H_


/*****************************************************************************
* Includes
*****************************************************************************/
#include "ZigBeeDataTypes.h"

#include "common_includes.h"
#include "buffer_management.h"
#include "ZLL_defines.h"


#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZLL_Commissioning_Cluster.h"
#include "ZLL_Interpan.h"
#endif



/*****************************************************************************
* Global Constants
*****************************************************************************/


/*****************************************************************************
* Public Memory declarations
*****************************************************************************/


/* Section for the ZLL Touch Link Receiver device */
typedef struct touchLinkReceiverSectionType_Tag {
    /* scan request: touch link initiator IEEEAddr */
    uint8_t initiator_long_address[8];
    /* scan_request: touch link initiator interPAN transaction identifier */
    uint32_t interPAN_transaction_identifier;
    /* identifier for the scan_response: it is used for the security key encryption */
    uint32_t response_transcation_identifier;
    /* initiator has provided network parameters */
    bool initiator_provided_network_parameters;
}__attribute__((__packed__))touchLinkReceiverSectionType_t;


extern touchLinkReceiverSectionType_t *pTouchLinkReceiverSectionMemory;


/* Context for the ZSL Touch Link Initiator device */
typedef struct touchLinkInitiatorSection_Tag {
    /* scan_request interPAN transaction identifier */
    uint32_t  interPAN_transaction_identifier;
    /* scan_request interPAN transaction identifier time for tracking id's life time */
    uint16_t interPAN_transaction_identifier_time;
    /* current interpan id is still valid: if not received response with such id are discarded */
    bool interPAN_transaction_identifier_is_valid;

    uint16_t target_network_startup_time;
    /* Scan responses stored, at most g_MAX_SCAN_RESPONSE_RESULTS_c )*/
    ZLL_scan_response_stored_t scan_response_stored[g_MAX_SCAN_RESPONSE_RESULTS_c];
    /* number of actual stored scan responses */
    uint8_t scan_response_active_index;
    /* index of last stored scan response*/
    uint8_t scan_response_stored_last_index;
    /* rrsi value of the last received scan_response */
    int8_t scan_response_rssi_value;

    /* The purpose of the touch link: network action, identify request, reset to factory new .. */
    uint8_t  touchLinkInitiatorPurpose[4];
    /* variable  for tracking the receiver enable time */
    uint16_t device_join_enable_receiver_time;
    /* stop poll during touch-link (if ED is already on a ZLL network) */
    bool stopPoll;
    /* identify duration */
    uint16_t identify_duration;
    /* touch-link should request a device_information_response */
    bool send_device_information_request;
    /* a device information response has been received*/
    bool device_information_response;
    /* scan request: touch link initiator IEEEAddr */
    uint8_t Target_long_address[8];
    /*Extended PAN id*/
    uint8_t Extended_PANId[8];
    /*short pan id of target received in*/
    uint16_t Short_PanId;

} touchLinkInitiatorSection_t;

extern touchLinkInitiatorSection_t *pTouchLinkInitiatiorSectionMemory;


/* Necessary fields used for device startup */

typedef struct deviceStartupSection_Tag {
    /* Flag to indicate that the active scan has been completed */
    bool activeScanComplete;
    /* Number of PANIDs, extended PANIDs found during active scan */
    uint8_t activeScan_PANIDs_number;
        /* address assigned to the recipient device from the Initiator end device */
    uint16_t recipient_device_network_address;
    /* PAN IDs and extended PANIDs reported by active scan results, at most MAX_ACTIVE_SCAN_RESULTS */
    uint16_t activeScan_PANIDS[g_MAX_ACTIVE_SCAN_RESULTS_c];
    uint8_t activeScan_extendedPANIDs[g_MAX_ACTIVE_SCAN_RESULTS_c][g_ZLL_EXTENDED_PANID_SIZE_c];
    /* zll channel selected after the active scan */
    uint8_t zll_channel;
    /* zll interpan channel (for network start request handling */
    uint8_t zll_interpan_channel;
    /* Initiator end device IEEEAddr:
        it is used when starting a new network (FN ED --> NFN R) */
    uint8_t end_device_IEEE_address[g_ZLL_EXTENDED_PANID_SIZE_c];
    /* Initiator end device network address:it is used when starting a
    new network (FN ED --> NFN R) */
    uint8_t end_device_network_address[g_ZLL_SHORT_PANID_SIZE_c];

    /* Flag to indicate the stack status has changed to up */
    bool stackStatusChanged_up;
    /* Flag to indicate the stack status has changed to down */
    bool stackStatusChanged_down;
    /* Flag to indicate a general stack status change */
    bool stackStatusChanged;
    /* Flag to indicate channel change event */
    bool stackStatusChanged_Channel;
    /* interpan message sent flag */
    bool interpan_has_been_sent;
    /* error with interpan message sent */
    bool interpan_has_error;
}__attribute__((__packed__))deviceStartupSection_t;


extern deviceStartupSection_t * pDeviceStartupSectionMemory;

typedef struct {
	/* main state*/
	uint32_t state;
	/* sub state*/
	uint32_t substate;
}__attribute__((__packed__)) ZLL_StateType;


ZLL_StateType topStateMachine(ZLL_StateType state);

/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/


void ZLL_ResetStartUpParmas(void);
void ZLL_AddSubDevicesInfo(void );
void UpdateSubDeviceExtendedAddress(uint8_t *pExtendedAddr);

#endif /*_ZLL_MAIN_H_*/
/*****************************************************************************
* End Of File                                       _ZLL_MAIN_H_
*****************************************************************************/
