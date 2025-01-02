/*******************************************************************************
* aps_interpan_stub.c
********************************************************************************
* Program Description:
*  The module encapsulates the functionality of aps inter pan stub of
*  APS layer.
*
*******************************************************************************/

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "stack_common.h"
#include "stack_functionality.h"
#include "stack_configuration.h"
#include "stack_util.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "stack_common.h"
#include "zdo_interface.h"
#include "apsde_interface.h"
#include "mac_interface.h"
#include "nwk_data_services_main.h"
//#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
#include "aps_interpan_stub.h"
//#include "nwk_main.h"
//#include "macro_header.h"



/*******************************************************************************
* Global Constants
*******************************************************************************/


/*******************************************************************************
* Public Memory declarations
*******************************************************************************/


/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/* None */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
#if ( g_APS_GROUP_DATA_RX_d == 1 )
uint8_t APS_Verify_GroupAddrAndEndPoint( uint16_t groupAddress, uint8_t endPoint);
#endif
static void APS_add_asdu_handler( uint8_t asduhandle );

/*******************************************************************************
* Public Functions
*******************************************************************************/

void APS_interpan_data_request( uint8_t bufferid,
                                APSDE_InterPan_Data_Request_t* pApsDataReq )
{
    APSDE_InterPan_Data_Request_t ptApsDataReq;
    MCPS_Data_Request_t *pMcpsDataRequest;
    uint8_t *pHeader;

    pApsDataReq = (APSDE_InterPan_Data_Request_t*)buffMgmt_getBufferPointer(  bufferid );
    pMcpsDataRequest = (MCPS_Data_Request_t *)pApsDataReq;
    memUtils_memCopy( (uint8_t*)&ptApsDataReq,
              (uint8_t*)pApsDataReq, sizeof( APSDE_InterPan_Data_Request_t ) );
    pMcpsDataRequest->msg_type = g_MCPS_DATA_REQUEST_c;
    pMcpsDataRequest->dst_addr_mode = ptApsDataReq.dstaddrmode;
    pMcpsDataRequest->src_addr_mode = ptApsDataReq.srcaddrmode;
    pMcpsDataRequest->msdu_handle = ptApsDataReq.asduhandle;
    memUtils_memCopy( pMcpsDataRequest->a_dst_pan_id, (uint8_t*)&ptApsDataReq.dstpanid,
              g_SHORT_PAN_ID_LENGTH_c );

    memUtils_memRevDirection( pMcpsDataRequest->a_msdu, pApsDataReq->a_asdu,
                         ptApsDataReq.asdulength );

    pHeader = pMcpsDataRequest->a_msdu;
    pMcpsDataRequest->tx_options = 0x00;
    pMcpsDataRequest->msdu_length = 0x06;
    pHeader -= 2;
    memUtils_memCopy( pHeader, (uint8_t*)&ptApsDataReq.profileid, g_SHORT_ADDRESS_LENGTH_c );
    pHeader -= 2;
    memUtils_memCopy( pHeader, (uint8_t*)&ptApsDataReq.clusterid, g_SHORT_ADDRESS_LENGTH_c );
    if ( ptApsDataReq.dstaddrmode == g_INTRP_GROUP_ADDR_MODE_c ) {
        pMcpsDataRequest->msdu_length += 0x03;
        pHeader -= 2;
        memUtils_memCopy( pHeader, ptApsDataReq.dstaddress.IEEE_address,
                          g_SHORT_ADDRESS_LENGTH_c );
        *--pHeader = g_INTRP_FRAME_DATA_c | g_INTRP_DELIVERY_GROUP_c;
        pMcpsDataRequest->dst_addr_mode = g_INTRP_16BIT_ADDR_MODE_c;
        *((uint16_t*)pMcpsDataRequest->dst_addr.a_short_addr) = g_BROADCAST_SHORT_ADDRESS_c;
    }
    else if ( ptApsDataReq.dstaddrmode == g_INTRP_16BIT_ADDR_MODE_c ) {
        pMcpsDataRequest->msdu_length += 0x01;
        memUtils_memCopy( pMcpsDataRequest->dst_addr.a_short_addr,
                  ptApsDataReq.dstaddress.IEEE_address, g_SHORT_ADDRESS_LENGTH_c );
        if ( *((uint16_t*)(ptApsDataReq.dstaddress.IEEE_address)) > g_SHORT_ADDRESS_RANGE_c ) {
            *--pHeader = g_INTRP_FRAME_DATA_c | g_INTRP_DELIVERY_BROADCAST_c;
        }
        else {
            *--pHeader = g_INTRP_FRAME_DATA_c | g_INTRP_DELIVERY_DIRECT_c;
            pMcpsDataRequest->tx_options = g_TX_OPTION_ACK_c;
        }
    }
    else if ( ptApsDataReq.dstaddrmode == g_INTRP_64BIT_ADDR_MODE_c ) {
        pMcpsDataRequest->msdu_length += 0x01;
        memUtils_memCopy( pMcpsDataRequest->dst_addr.a_short_addr,
                  ptApsDataReq.dstaddress.IEEE_address, g_EXTENDED_ADDRESS_LENGTH_c );
        *--pHeader = g_INTRP_FRAME_DATA_c | g_INTRP_DELIVERY_DIRECT_c;
        pMcpsDataRequest->tx_options = g_TX_OPTION_ACK_c;
    }
    *--pHeader = 0x00;
    *--pHeader = g_INTRP_FRAME_DATA_c | ( g_PROTOCOL_VERSION_c << 0x02 );
    memUtils_memRightShift ( pHeader,
                      pMcpsDataRequest->msdu_length + ptApsDataReq.asdulength,
                      pMcpsDataRequest->msdu_length );
    pMcpsDataRequest->msdu_length += ptApsDataReq.asdulength;
    rsi_zigb_aps_asdu_handler( ptApsDataReq.asduhandle );
    NWK_sendRequest ( bufferid );

}

/*******************************************************************************/
static void APS_add_asdu_handler( uint8_t asduhandle )
{
    uint8_t index = 0x00;
    Msdu_Handle_Table_t *pMsduHandleEntry;
#if 0
    while ( index < g_NUM_OF_MSDU_HANDLE_ENTRIES_c ) {
        pMsduHandleEntry =  &g_aMsdu_Handle[ index ];
        if ( pMsduHandleEntry->msdu_handle == g_NULL_c )  {
            pMsduHandleEntry->msdu_handle = asduhandle;
            pMsduHandleEntry->module = g_Conf_For_InterPan_Data_c;
            /* Invalid values which we are not using */
            pMsduHandleEntry->NBT_index = 0xFF;
            pMsduHandleEntry->nwk_dest_addr = 0xFFFF;
            pMsduHandleEntry->nwk_src_addr = 0xFFFF;
            break;
        }
        index++;
    }
#endif
}

/*******************************************************************************/
//#endif /* ( g_INTERPAN_DATA_ENABLE_c == 1 ) */
/*******************************************************************************
* Private Functions
******************************************************************************/

/* None */

/*******************************************************************************
* Interrupt Service Routines
******************************************************************************/

/* None */

/*******************************************************************************
* End Of File
******************************************************************************/
