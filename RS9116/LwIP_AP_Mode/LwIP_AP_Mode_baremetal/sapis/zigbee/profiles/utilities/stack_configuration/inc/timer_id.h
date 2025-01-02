/*******************************************************************************
*timer_id.h
********************************************************************************
* Program Description:
*   All time identifiers used by the stack and provided for application is
*   mentioned here
*
*******************************************************************************/

#ifndef TIMER_ID_H
#define TIMER_ID_H

/*******************************************************************************
                  Timer Id Offset Selection
*******************************************************************************/
#if ( g_NWK_BROADCAST_d == 1 )
#define g_MAX_OUTSTANDING_BCAST_c                     8
#else
#define g_MAX_OUTSTANDING_BCAST_c                     0
#endif

#define g_FIRST_MAC_TIMER_ID_c                0x00u
#define g_LAST_MAC_TIMER_ID_c                 0x09u

enum timeID{
   /*************************************************************
   * Note: Values assgined to the constants not to be changed
   *************************************************************/
    
   /*********** ID's of the timer used by the MAC layer ********/
   /*! -The timer used for allowing a device to switch from RX to TX */
   g_TURN_AROUND_TIMER_ID_c                 = g_FIRST_MAC_TIMER_ID_c,

   /*! -The timer used to perform CSMA CA */
   g_CSMA_CA_TIMER_ID_c                     = 0x01,

   /*! -The timer used to wait for acknowledgement after sending a packet */
   g_ACK_WAIT_TIMER_ID_c                    = 0x02u,

   /*! -The timer used for scanning */
   g_SCAN_TIMER_ID_c                        = 0x03u,

   /*! -The timer used to wait for a response after an association request */
   g_MAC_RESPONSE_WAIT_TIMER_ID_c           = 0x04u,

   /*! -The timer used to discard a packet destined for a child when a poll
    * request is not received from the child within specified time */
   g_TRANSACTION_PERSISTENCE_TIMER_ID_c     = 0x05u,

   /*! -The timer used to turn receiver ON after a sepcified delay */
   g_RX_ON_TIMER_ID_c                       = 0x06u,

   /*! -The timer used to wait for a time when an ack is received with frame
    * pending bit set as response to a data request */
   g_MAC_MAX_FRAME_TOTAL_WAIT_TIMER_ID_c    = 0x07u,

   /* - The timer used to handle Inter-frame spacing */
   g_IFS_TIMER_ID_c                         = 0x08u,
   
   /* - The timer used to reset the state of the uart when no data is received */
   m_UART_TIMER_c                           = g_LAST_MAC_TIMER_ID_c,
   
   /*********** ID's of the timer used by the ZigBee layer ********/
   /* Permit Joining Timer */
   g_NWK_PERMIT_JOINING_TIMER_c             = 0x0Au,

#if ( g_NWK_BROADCAST_d == 1 )
    /* Broadcast Delivery Timer */
	g_NWK_BCAST_DELIVERY_TIMER_c            = 0x0Bu,
#endif
#if ( g_NWK_MESH_ROUTING_d == 1 )	
    /* Route Discovery Timer */
	g_NWK_ROUTE_DISCOVERY_TIMER_c           = 0x0Cu,
     
	/* RREQ Rebroadcast Jitter Timer */
	g_NWK_ROUTING_JITTER_TIMER_c            = 0x0Du,
     
	/* nwkcRREQRetryInterval */
	g_NWK_ROUTING_RETRY_INTERVAL_TIMER_c    = 0x0Eu,
#endif
#if (g_REJOIN_REQUEST_d == 1)
	g_NWK_REJOIN_RESPONSE_TIMER_c           = 0x0Fu,
#endif
	g_APS_ACK_TIMER_c                       = 0x10u,
     
	g_DUPLICATE_REJECTION_TIMER_c           = 0x11u,
     
	/* Timer id to switch key  */
	g_ZDO_SEC_SWITCH_KEY_TIMER_ID_c         = 0x12u,
#if ( g_PAN_ID_RESOLUTION_d == 1 ) || ( g_PAN_ID_CONFLICT_REPORT_d  == 1 )
	/*timer for Pan id conflict*/
	g_PAN_ID_CONFLICT_TIMER_ID_c            = 0x13u,
     
    g_PAN_ID_CONFLICT_DETECTION_TIMER_ID_c  = 0x14u,
#endif
//#if( g_FREQUENCY_AGILITY_SUPPORTED_d == 1 )
	/* timer for calclation of failure rate */
	g_INTREFERENCE_DETECTION_TIMER_ID_c     = 0x15u,
     
	/* AGE TIMER FOR Frequency Agility*/
	g_INTERFERENCE_AGE_TIMER_ID_c           = 0x16u,
    
    /* Broadcast Delivery Timer for  Frequency Agility*/
	g_NWK_MGR_QUERY_INTERVAL_TIMER_ID_c     = 0x17u,
     
	/* Broadcast Delivery Timer for  Frequency Agility*/
	g_FA_BROAD_CAST_TIMER_ID_c              = 0x18u,
//#endif

#if(g_NWK_LINK_STATUS_d == 1 )
	g_LINK_STATUS_TIMER_ID_c                = 0x19u,
#endif
#if( g_NWK_MANY_TO_ONE_ROUTING_d == 1 )
    g_MANY_TO_ONE_TIMER_ID_c                = 0x1Au,
#endif

	/* Last Timer Id after Deciding how many outstanding broadcasts will be supported */
	g_DISCOVERY_TIMER_ID                    = 0x1Bu,
    
#if ( g_NLME_SYNC_d == 1)
	g_NWK_POLL_TIMER_c                      = 0x1Cu,
    
	g_APPLICATION_FAST_POLL_TIMER_c         = 0x1Du,
#endif
#if( g_ZIGBEE_ROUTER_d == 1 ) || ( g_ZIGBEE_END_DEVICE_d == 1 )
	g_ZDO_END_DEVICE_REJOIN_TIMER_ID_c      = 0x1Eu,
    
    g_REJOIN_TIMER_ID                       = 0x1Fu,
#endif
    g_ZDO_SEC_AUTHENTICATION_TIMER_ID_c     = 0x20u,
    
    g_ZDO_SEC_APPLINKKEY_TIMER_ID_c         = 0x21u,
#if (g_NWK_ADDRESS_CONFLICT_DETECTION_d == 1)
	g_ADDRESS_CONFLICT_TIMER_ID_c           = 0x22u,
#endif
//#if(APS_FRAGMENTATION_CAPABILITY == RSI_ENABLE)	
    APS_FRAGMENTATION_TIMER_ID              = 0x23u,

    APS_FRAGMENTATION_ACK_WAIT_TIMER_ID     = 0x24u,
//#endif
    g_NWK_STATUS_CMD_TIMER_ID               = 0x25u,

//#if(g_End_Device_Bind_Response_d == RSI_ENABLE ) 
    g_ZDP_BIND_TIMER_ID_c                   = 0x26u,
#if ( g_ZIGBEE_COORDINATOR_d == RSI_ENABLE )
    g_ZDP_END_DEVICE_BIND_TIMER_ID_c        = 0x27u,
#endif
//#endif /* (g_End_Device_Bind_Response_d == RSI_ENABLE ) */
    /*If the Multicast Data is broadcasted. Until this timer expires.
    The stack do not rebroadcast any other multicast frame. */
    g_MULTICAST_INITIATE_ID                 = 0x28u,

#if( g_ZIGBEE_ROUTER_d == 1 ) || ( g_ZIGBEE_END_DEVICE_d == 1 )
    g_REJOIN_INTERVAL_TIMER_ID              = 0x29u,
#endif

#if (g_PERSISTENT_DATA_STORAGE_d == 1)
    g_NVM_UPDATE_TIMER_ID                   = 0x2Au,
#endif /* g_PERSISTENT_DATA_STORAGE_d == 1*/
/* Offset After Including Mesh Routing.
    This will be the start for the broadcast timer id's */
	g_NWK_TIMER_ID_BCAST_PASSIVE_ACK_c      = 0x2Bu,
	
	/** @brief Timer ID used as Fast Timer 0 */
	g_FAST_TIMERID0_c       = g_NWK_TIMER_ID_BCAST_PASSIVE_ACK_c + 1,

	/** @brief Timer ID used as Fast Timer 1 */
	g_FAST_TIMERID1_c         = g_FAST_TIMERID0_c + 1,
    /*********** timer id's above this can be used by the application ********/
    
    /***************************************************************************
     * Application developer can modify the constants and its values from here
     **************************************************************************/
    g_ZIGBEE_APP_TIMER_ID                   = (g_FAST_TIMERID1_c +\
        g_MAX_OUTSTANDING_BCAST_c + 1)
};

/*! -The first timer ID free for use by the application */
#define g_FIRST_APP_TIMER_ID_c              g_NWK_PERMIT_JOINING_TIMER_c

/*! -Last Timer */
#define g_LAST_TIMER_c                      g_ZIGBEE_APP_TIMER_ID


#define g_NWK_TOTAL_NUMBER_OF_TIMERS_c	    (g_LAST_TIMER_c)

/*Timer Id used by Test Profile*/
#define m_ZDO_PROFILE_TIMER_c  ( g_NWK_TOTAL_NUMBER_OF_TIMERS_c + 1 )

/*******************************************************************************
* End Of File
*******************************************************************************/

#endif                                 /* TIMER_ID_H */
