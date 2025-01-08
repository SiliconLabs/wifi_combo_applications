/******************************************************************************
* ZCL_General_Basic_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Basic cluster attribute and cmmnd information
******************************************************************************/

#ifndef _ZCL_HA_DIAGNOSTICS_CLUSTER_H_
#define _ZCL_HA_DIAGNOSTICS_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define g_DIAGNOSTICS_CLUSTER_c                                0x0b05

/* Diagnostics Cluster Attributes */
/* Hardware Information (0x0000)*/

/* Commissionning Information (0x0200*/
/* Application Information*/

#define g_NO_OF_RESETS_ATTRIBUTE_c						0x0000
#define g_PERSISTENT_MEMORY_WRITES_ATTRIBUTE_c		 	0x0001

 /* Stack/Network Information (0x0100 */

#define g_MAC_RX_BCAST_ATTRIBUTE_c						0x0100
#define g_MAC_TX_BCAST_ATTRIBUTE_c						0x0101
#define g_MAC_RX_UCAST_ATTRIBUTE_c						0x0102
#define g_MAC_TX_UCAST_ATTRIBUTE_c						0x0103
#define g_MAC_TX_UCAST_RETRY_ATTRIBUTE_c				0x0104
#define g_MAC_TX_UCAST_FAIL_ATTRIBUTE_c					0x0105
#define g_APS_RX_BCAST_ATTRIBUTE_c			   			0x0106
#define g_APS_TX_BCAST_ATTRIBUTE_c			   			0x0107
#define g_APS_RX_UCAST_ATTRIBUTE_c						0x0108
#define g_APS_TX_UCAST_SUCCESS_ATTRIBUTE_c				0x0109
#define g_APS_TX_UCAST_RETRY_ATTRIBUTE_c				0x010A
#define g_APS_TX_UCAST_FAIL_ATTRIBUTE_c					0x010B
#define g_ROUTE_DISC_INITIATED_ATTRIBUTE_c				0x010C
#define g_NEIGHBOR_ADDED_ATTRIBUTE_c					0x010D
#define g_NEIGHBOR_REMOVED_ATTRIBUTE_c					0x010E
#define g_NEIGHBOR_STALE_ATTRIBUTE_c					0x010F
#define g_JOIN_INDICATION_ATTRIBUTE_c					0x0110
#define g_CHILD_MOVED_ATTRIBUTE_c						0x0111
#define g_NWKFC_FAILURE_ATTRIBUTE_c						0x0112
#define g_APSFC_FAILURE_ATTRIBUTE_c						0x0113
#define g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_c				0x0114
#define g_NWK_DECRYPT_FAILURES_ATTRIBUTE_c				0x0115
#define g_APS_DECRYPT_FAILURES_ATTRIBUTE_c				0x0116
#define g_PACKET_BUFFER_ALLOCATE_FAILURES_ATTRIBUTE_c	0x0117
#define g_RELAYED_UCAST_ATTRIBUTE_c						0x0118
#define g_PHY_TO_MAC_QUEUE_LIMIT_REACHED_ATTRIBUTE_c	0x0119
#define g_PACKET_VALIDATE_DROP_COUNT_ATTRIBUTE_c		0x011A

enum
{
   g_Clear_Diagnostics_Attributes_Events_c = 0xe0
};

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_DIAGNOSTICS_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Diagnostics_Server_Cluster_Info;
#endif

#if ( g_DIAGNOSTICS_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Diagnostics_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
/* enumerations for commands specific for general clusters */
typedef enum Diagnostics_Cluster
{
   g_Clear_diagnostic_attributes_Command_c
}Diagnostics_Cluster;

/* Format of the clear diagnostic attribute Command Payload */
typedef struct Clear_Diagnostic_Attribute_Command_Tag
{
   uint8_t attribute_count;
   uint16_t list_of_attributes[28];
}__attribute__((__packed__))Clear_Diagnostic_Attribute_Command_t;

/* Structure to send basic commands */
typedef struct General_Domain_Diagnostics_Request_tag
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
}__attribute__((__packed__))General_Domain_Diagnostics_Request_t;


/*-----------------------------------------------------------------------------
Basic Cluster structure - attributes
Field: no_of_resets -
Field: persistent_memory_writes -
Filed: mac_rx_bcast - A counter that is incremented each time the MAC layer
						receives a broadcast.
Field: mac_tx_bcast - A counter that is incremented each time the MAC layer
						transmits a broadcast
Field: mac_rx_ucast - A counter that is incremented each time the MAC layer
						receives a unicast
Field: mac_tx_ucast - A counter that is incremented each time the MAC layer
						transmits a unicast.
Field: mac_tx_ucast_retry - A counter that is incremented each time the MAC layer
						 retries a unicast
Field: mac_tx_ucast_fail -  A counter that is incremented each time the MAC layer
							fails to send a unicast.
Field: aps_rx_bcast -  A counter that is incremented each time the APS layer
							receives a broadcast.
Field: aps_tx_bcast -  A counter that is incremented each time the APS layer
						transmits a broadcast
Filed: aps_rx_ucast -  A counter that is incremented each time the APS layer
						receives a unicast.
Field: aps_tx_ucast_success -  A counter that is incremented each time the APS
						layer successfully transmits a unicast
Field: aps_tx_ucast_retry - A counter that is incremented each time the APS layer
							retries the sending of a unicast.

Field: aps_tx_ucast_fail - A counter that is incremented each time the APS layer
						fails to send a unicast.

Field: Route_Disc_Initiated -  A counter that is incremented each time the network layer s
							ubmits a route discovery message to the MAC.
Field: Neighbor_Added -  A counter that is incremented each time an entry is
							added to the neighbor table.
Field: Neighbor_Removed -  A counter that is incremented each time an entry is
						removed from the neighbor table
Field: Neighbor_Stale -  A counter that is incremented each time a neighbor table
						entry becomes stale because the neighbor has not been heard from.
Field: Join_Indication - A counter that is incremented each time a node joins or
						rejoins the network via this node
Field: Child_Moved - A counter that is incremented each time an entry is removed
						from the child table.
Field: NWKFC_Failure -  A counter that is incremented each time a message is dropped
				at the network layer  because the APS frame counter was not higher
				than the last message seen from that  source.

Field: APSFC_Failure - A counter that is incremented each time a message is dropped
						at the APS layer because the APS frame counter was not
						higher than the last message seen from that source
Field: APS_Unauthorized_Key -  A counter that is incremented each time a message
		is dropped 75 at the APS layer  because it had APS encryption but the
		key associated with the sender has not been  authenticated, and thus
		the key is not authorized for use in APS data messages.

Field: NWK_Decrypt_Failures - A counter that is incremented each time a NWK
					encrypted message was received but dropped because decryption failed.
Field: APS_Decrypt_Failures - A counter that is incremented each time an APS
		encrypted message was received  but dropped because decryption failed.
Field: Packet_Buffer_Allocate_Failures - A counter that is incremented each time
			the stack failed to allocate a packet  buffers. This doesn't necessarily
			mean that the packet buffer count was 0 at the time, but that the
			number requested was greater than the number free.

Field: Relayed_Ucast - A counter that is incremented each time a unicast packet
						is relayed.
Field: Phyto_MAC_queue_limit_reached -
Field: Packet_Validate_drop_count - A counter that is incremented each time a packet
		was dropped due to a packet  validation error. This could be due to
		length or other formatting problems in the packet.
-----------------------------------------------------------------------------*/
typedef struct Diagnostics_Cluster_Tag
{
#if(g_NO_OF_RESETS_ATTRIBUTE_ENABLE_d == 1)
	uint8_t no_of_resets[2];
#endif
#if(g_PERSISTENT_MEMORY_WRITES_ATTRIBUTE_ENABLE_d == 1)
	uint8_t persistent_memory_writes[2];
#endif
#if(g_MAC_RX_BCAST_ATTRIBUTE_ENABLE_d == 1)
	uint8_t mac_rx_bcast[2];
#endif
#if(g_MAC_TX_BCAST_ATTRIBUTE_ENABLE_d == 1)
	uint8_t mac_tx_bcast[2];
#endif
#if(g_MAC_RX_UCAST_ATTRIBUTE_ENABLE_d == 1)
	uint8_t mac_rx_ucast[2];
#endif
#if(g_MAC_TX_UCAST_ATTRIBUTE_ENABLE_d == 1)
	uint8_t mac_tx_ucast[2];
#endif
#if(g_MAC_TX_UCAST_RETRY_ATTRIBUTE_ENABLE_d == 1)
	uint8_t mac_tx_ucast_retry[2];
#endif
#if(g_MAC_TX_UCAST_FAIL_ATTRIBUTE_ENABLE_d == 1)
	uint8_t mac_tx_ucast_fail[2];
#endif
#if(g_APS_RX_BCAST_ATTRIBUTE_ENABLE_d == 1)
	uint8_t aps_rx_bcast[2];
#endif
#if(g_APS_TX_BCAST_ATTRIBUTE_ENABLE_d == 1)
	uint8_t aps_tx_bcast[2];
#endif
#if(g_APS_RX_UCAST_ATTRIBUTE_ENABLE_d == 1)
	uint8_t aps_rx_ucast[2];
#endif
#if(g_APS_TX_UCAST_SUCCESS_ATTRIBUTE_ENABLE_d == 1)
	uint8_t aps_tx_ucast_success[2];
#endif
#if(g_APS_TX_UCAST_RETRY_ATTRIBUTE_ENABLE_d == 1)
	uint8_t aps_tx_ucast_retry[2];
#endif
#if(g_APS_TX_UCAST_FAIL_ATTRIBUTE_ENABLE_d == 1)
	uint8_t aps_tx_ucast_fail[2];
#endif
#if(g_ROUTE_DISC_INITIATED_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Route_Disc_Initiated[2];
#endif
#if(g_NEIGHBOR_ADDED_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Neighbor_Added[2];
#endif
#if(g_NEIGHBOR_REMOVED_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Neighbor_Removed[2];
#endif
#if(g_NEIGHBOR_STALE_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Neighbor_Stale[2];
#endif
#if(g_JOIN_INDICATION_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Join_Indication[2];
#endif
#if(g_CHILD_MOVED_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Child_Moved[2];
#endif
#if(g_NWKFC_FAILURE_ATTRIBUTE_ENABLE_d == 1)
	uint8_t NWKFC_Failure[2];
#endif
#if(g_APSFC_FAILURE_ATTRIBUTE_ENABLE_d == 1)
	uint8_t APSFC_Failure[2];
#endif
#if(g_APS_UNAUTHORIZED_KEY_ATTRIBUTE_ENABLE_d == 1)
	uint8_t APS_Unauthorized_Key[2];
#endif
#if(g_NWK_DECRYPT_FAILURES_ATTRIBUTE_ENABLE_d == 1)
	uint8_t NWK_Decrypt_Failures[2];
#endif
#if(g_APS_DECRYPT_FAILURES_ATTRIBUTE_ENABLE_d == 1)
	uint8_t APS_Decrypt_Failures[2];
#endif
#if(g_PACKET_BUFFER_ALLOCATE_FAILURES_ATTRIBUTE_ENABLE_d == 1)

	uint8_t Packet_Buffer_Allocate_Failures[2];

#endif
#if(g_RELAYED_UCAST_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Relayed_Ucast[2];
#endif
#if(g_PHY_TO_MAC_QUEUE_LIMIT_REACHED_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Phyto_MAC_queue_limit_reached[2];
#endif
#if(g_PACKET_VALIDATE_DROP_COUNT_ATTRIBUTE_ENABLE_d == 1)
	uint8_t Packet_Validate_drop_count[2];
#endif
} __attribute__((__packed__))Diagnostics_Cluster_t;

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_Diagnostics_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : Length
*
* Input Parameters:
*	endPointId - Device End point
*   asduLength - Length of received asdu
*   pReceivedAsdu - Pointer to received asdu
*   pZCLClusterInfo - Pointer to Level control cluster info
*   pAttrsDataBaseLocation - Pointer to Basic cluster attributes storage
*
* Output Parameters:
*	pResponseAsduStartLocation - Pointer for response creation
*   pActionHandler - Pointer for Event Updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the Basic cluster command received in the indication
*
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Diagnostic_Cluster_Cmd
(
     uint8_t endPointId,
     uint8_t asduLength,
     uint8_t* pReceivedAsdu,
     uint8_t* pResponseAsduStartLocation,
     ZCL_ActionHandler_t *pActionHandler,
     ZCL_Cluster_Info_t const *pZCLClusterInfo,
     uint8_t * pAttrsDataBaseLocation
);

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                      /* g_BASIC_CLUSTER_ENABLE_d == 1 */

#endif                                      /*_ZCL_GENERAL_BASIC_CLUSTER_H_*/
