/******************************************************************************
 * nwk_data_services_main.h
 ******************************************************************************
 * Program Description:
 *
 ******************************************************************************/
#ifndef NWK_DATA_SERVICES_MAIN_H
#define NWK_DATA_SERVICES_MAIN_H

/******************************************************************************
 * Includes
 ******************************************************************************/

/* None */

/******************************************************************************
 * Global Constants
 ******************************************************************************/
/* Do not change this macro, this macro gives the index to the start of the
 nsdu. This is for Network Layer 2006. This will change for ZigBee Pro*/
#define g_PHY_MAC_NWK_HEADER_SIZES_c                    0x2E
#define m_OFFSET_FOR_NWK_CMD_PLD_c                      0x40
#define g_LEAVE_CMD_PAYLOAD_OFFSET_c                    0x40
#define g_REPORT_CMD_PAYLOAD_OFFSET_c                   0x40
#define g_UPDATE_CMD_PAYLOAD_OFFSET_c                   0x40
#define g_REJOIN_REQUEST_CMD_PAYLOAD_OFFSET_c           0x40
#define g_NUM_OF_MSDU_HANDLE_ENTRIES_c                  0x0a
#define m_INDEX_TO_NLDE_DATA_REQ_NSDU_HANDLE_c          5
#define g_GET_NBT_RELATION_SHIP_BITS_c                  0x03
#define g_MASK_NBT_RELATION_SHIP_BITS_c                 0x07

/* The maximum number of pending entries currently supported
 by MAC is 0x02 this value needs to be updated if the MAC queue
 size for pending data is increased*/
#define g_MAX_PENDING_ENTRY_c                           0x04
#define g_INTERNAL_TIMER_EXPIRY_EVENT_c                 0xaa
#define g_INTERNAL_BUFFERED_DATA_EVENT_c                0xab

/******************************************************************************
 * Type Definitions
 ******************************************************************************/
typedef struct Generic_nwk_packet_tag {
    uint8_t msg_type;
    uint8_t dst_address_mode;
    uint16_t dst_address;
    uint8_t nsdu_length;
    uint8_t nsdu_handle;
    uint8_t radius;
    uint8_t non_member_radius;
    uint8_t discover_route;
    uint8_t security_enable;
    uint8_t nwk_frame_seq_num;
    uint8_t link_quality_ind;
    uint16_t mac_src_add;
    uint16_t nwk_src_addr;
    uint8_t frame_type;
    uint8_t is_from_nlle;
    uint8_t frame_ctrl; /* b0 for SRC and b1 for DEST */
    uint8_t buffer_index;
    uint8_t dst_extended_address[8];
    uint8_t src_extended_address[8];
    uint8_t* nsdu;
} __attribute__((__packed__))Generic_nwk_Packet_t;

typedef struct Msdu_Handle_Table_Tag {
    uint8_t msdu_handle;
    uint8_t module;
    uint8_t NBT_index;
    uint16_t nwk_dest_addr;
    uint16_t nwk_src_addr;
} __attribute__((__packed__))Msdu_Handle_Table_t;

enum {
    g_Passive_Ack_Timer_Expiry_Event_c,
    g_Broadcast_Delivery_Timer_Expiry_Event_c,
    g_Broadcast_Jitter_Timer_Expiry_Event_c,
    g_Route_Discovery_Timer_Expiry_Event_c,
    g_RREQ_Jitter_Timer_Expiry_c,
    g_RREQ_Retry_Interval_Timer_Expiry_c
};


typedef struct {
    uint8_t rejoinResponseState;
    uint8_t nwkNSDUhandle;
    uint16_t nwk_DestAddrForNWKStatusCmd;
    uint16_t nwkStatusCmdDestAddrField;
    uint8_t nwkRetryThreshold;
} __attribute__((__packed__))nwk_dataServicesMain_t;


extern uint8_t multicastControl;

/******************************************************************************
 * Memory Declarations
 ******************************************************************************/
extern nwk_dataServicesMain_t nwk_dataServicesMain_v;
extern Msdu_Handle_Table_t g_aMsdu_Handle[g_NUM_OF_MSDU_HANDLE_ENTRIES_c];

/******************************************************************************
 * Function Prototypes
 *----------------------------------------------------------------------------*/

/* None */

/******************************************************************************
 * NWK_dataServicesMain
 *------------------------------------------------------------------------------
 *
 * Return Value : None
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is called by the main.
 *
 * Purpose of the function:
 *   This function separates out the received frame, depending on the request
 *   type, like it is an broadcast, unicast or leave.
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void NWK_dataServicesMain(uint8_t buffer_index);

/******************************************************************************
 * NWK_generateMCPSdataRequest
 *------------------------------------------------------------------------------
 *
 * Return Value : TRUE
 * Parameters   :
 *
 * Input Parameters:
 *   Generic_nwk_Packet_t* p_nwk_packet_info ->
 *   uint16_t mac_dest_addr->
 *   uint8_t tx_options ->
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *
 * Purpose of the function:
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t NWK_generateMCPSdataRequest(Generic_nwk_Packet_t* p_nwk_packet_info,
        uint16_t mac_dest_addr, uint16_t nwk_dest_addr, uint8_t tx_options,
        uint8_t module, uint8_t NBT_index);

/******************************************************************************
 *    NWK_generateNLDEconfirmation
 ******************************************************************************
 *
 * Return Value :
 * Parameters   :
 *
 * Input Parameters:
 *
 * Output Parameters:
 *
 * Scope of the function:
 *
 * Purpose of the function:
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void NWK_generateNLDEconfirmation(uint8_t buffer_index, uint8_t handle,
        uint8_t status);

/******************************************************************************
 *    NWK_generateNLDEindication
 ******************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   uint8_t buffer_index -> Index of the message containing
 *   MCPS-Data.Indication sent from NLLE
 *
 *   uint8_t *p_buffer -> pointer to the generic ptr
 *   which will be reused to generate NLDE-Data-Indication
 *
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is called by the Data service module within the Network layer
 *
 * Purpose of the function:
 *   This function is called to generate NLDE-Data.Indication to be sent to
 *   the upperlayer
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void NWK_generateNLDEindication(Generic_nwk_Packet_t* p_nwk_msg);

/******************************************************************************
 *  NWK_postTimerExpiryEvent
 ******************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   uint8_t timer_expiry_event_id ->
 *
 *
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *
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
#if ( ( g_NWK_BROADCAST_d == 1 ) || ( g_NWK_MESH_ROUTING_d == 1 ) )
void NWK_postTimerExpiryEvent ( uint8_t timer_expiry_event_id,
        uint8_t timer_id );

#endif                                          /* ( g_NWK_BROADCAST_d == 1 ) ||
                                               ( g_NWK_MESH_ROUTING_d == 1 ) */

/******************************************************************************
 *  NWK_getNsduhandle
 ******************************************************************************
 *
 * Return Value :
 *   uint8_t nsdu_handle
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
 *   This function can be called anywhere within the data services module
 *
 * Purpose of the function:
 *   This function generates an NSDU Handle starting from 0x80 till oxFF
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
uint8_t NWK_getNsduhandle(void);

/******************************************************************************
 * NWK_updateNeighborTableAndSecurityTable
 ******************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   p_buffer -   This is a pointer to the Mcps-Data.Ind structure
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   The scope of this function is within this module.
 *
 * Purpose of the function:
 *   This function updates the neighbor table with the extended and source
 *   address, and all security related attributes.
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 *
 ******************************************************************************/
#if ( g_RX_ON_DEVICE_d == 1 )
uint8_t NWK_updateNeighborTableAndSecurityTable ( uint8_t* p_buffer,
        uint8_t* p_mac_src_addr,uint8_t update_extd_addr, uint8_t nwk_hdr_len );

#endif                                                  /*g_RX_ON_DEVICE_d == 1*/

void NWK_rejoinRequestIndication(Generic_nwk_Packet_t* p_nwk_packet_info,
        uint8_t capability_info);

void NWK_rejoinResponseIndication(Generic_nwk_Packet_t* p_nwk_packet_info);

/******************************************************************************
 * NWK_sendNetworkStatusIndication
 ******************************************************************************
 *
 * Return Value : None
 *
 * Parameters   :
 *
 * Input Parameters:
 *                  uint16_t routing_dest_addr
 *                  uint8_t status
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   The scope of this function is global.
 *
 * Purpose of the function:
 *   This function is used for sending Network Status indication to the NHLE.
 *   This function allocates memory to send the indication.
 *
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void NWK_sendNetworkStatusIndication(uint16_t dest_addr, uint8_t status);

/******************************************************************************
 * NWK_MGMT_Handle_Pan_Id_Resolution
 ******************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *
 *   p_nwk_packet_info - points to the network report command received
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called ONLY WITHIN NWK_dataServicesMain.c
 *
 * Purpose of the function:
 *   This function handles report command. Creates a new random pan id.
 *   network update id is incremented by one.creates network update
 *   command frame and broadcasts the command.
 *   It starts a timer for broadcast delivery time. On expire of this
 *   timer, the pan id is updated to the new pan id and then sends
 *   network status indication to the Application
 *
 *
 * Before the function is called:
 *   None
 *
 *
 * After the function is called:
 *   None
 *
 ******************************************************************************/
#if ( g_PAN_ID_RESOLUTION_d == 1 )
void NWK_MGMT_Handle_Pan_Id_Resolution ( uint8_t buffer_ID,
        Generic_nwk_Packet_t* p_nwk_packet_info );
#endif                                                /*g_PAN_ID_RESOLUTION_d*/

/******************************************************************************
 * NWK_MGMThandleUpdatePanId
 ******************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *
 *   p_nwk_packet_info - points to the network update command received
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function can be called ONLY WITHIN NWK_dataServicesMain.c
 *
 * Purpose of the function:
 *   This function handles update command received from the network manager.
 *   It starts timer for broadcast delivery time. On expire of the timer, it
 *   updates the current pan id and update id to the value received in the
 *   network update command frame.
 *
 *
 * Before the function is called:
 *   None
 *
 *
 * After the function is called:
 *   None
 *
 ******************************************************************************/
#if ( g_PAN_ID_CONFLICT_REPORT_d  == 1 )
void NWK_MGMThandleUpdatePanId(Generic_nwk_Packet_t *p_nwk_packet_info);
#endif                                           /*g_PAN_ID_CONFLICT_REPORT_d*/

/******************************************************************************
 * NWK_activeEndDevicehandleNwkStausCmmnd
 *------------------------------------------------------------------------------
 *
 * Return Value : None
 * Parameters   :
 *
 * Input Parameters:
 *   bufferId
 *
 * Output Parameters:
 *   None
 *
 * Scope of the function:
 *   This function is called by the main.
 *
 * Purpose of the function:
 *  For the active end device nwk status command handling
 * Before the function is called:
 *   None
 *
 * After the function is called:
 *   None
 ******************************************************************************/
void NWK_activeEndDevicehandleNwkStausCmmnd(uint8_t bufferId);

#if (DEBUG_TEST == 1)
extern uint8_t testHrns_passMessageUp(uint8_t, uint8_t);
#endif
/******************************************************************************
 * End Of File
 ******************************************************************************/
#endif                                          /* NWK_DATA_SERVICES_MAIN_H */
