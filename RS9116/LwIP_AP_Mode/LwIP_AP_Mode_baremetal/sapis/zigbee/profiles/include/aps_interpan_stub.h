/*******************************************************************************
* aps_interpan_stub.h
********************************************************************************
* Program Description:
*   This file exposes the functionlaties provided by aps_interpan_stub.c
*   module.
*
*******************************************************************************/

#ifndef APS_INTERPAN_STUB_H
#define APS_INTERPAN_STUB_H

/*******************************************************************************
* Includes
*******************************************************************************/

#include "apsde_interface.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/

#define g_INTRP_FRAME_DATA_c                          0x03

// Delivery Mode
#define g_INTRP_DELIVERY_DIRECT_c                     0x00
#define g_INTRP_DELIVERY_BROADCAST_c                  0x08
#define g_INTRP_DELIVERY_GROUP_c                      0x0C
#define g_INTRP_FRAME_TYPE_MASK_c                     0x03
#define g_INTRP_DELIVERY_MODE_MASK_c                  0x0C

// Destination Address Modes
#define g_INTRP_GROUP_ADDR_MODE_c                     0x01
#define g_INTRP_16BIT_ADDR_MODE_c                     0x02
#define g_INTRP_64BIT_ADDR_MODE_c                     0x03
//#define g_PROTOCOL_VERSION_c                          0x08
#define g_BROADCAST_SHORT_ADDRESS_c                   0xFFFF
#define g_SHORT_ADDRESS_RANGE_c                       0xFFF7
#define g_INTRP_END_POINT_c                           0x01

/*******************************************************************************
* Type Definitions
*******************************************************************************/

/* None */

/*******************************************************************************
* Memory Declarations
*******************************************************************************/

/* None */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

void APS_handle_interpan_data_indication( uint8_t bufferid );
void APS_interpan_data_request( uint8_t bufferid,
                                APSDE_InterPan_Data_Request_t* pApsDataReq );
void APS_handle_interpan_data_confirmation( uint8_t bufferId );

/*******************************************************************************
* End Of File
*******************************************************************************/

#endif                                                /* APS_INTERPAN_STUB_H */