/******************************************************************************
* ZLL_Attributes.h
*******************************************************************************
* Program Description:
*
*
******************************************************************************/
#ifndef _ZLL_PROFILE_ATTRIBUTES_H_
#define _ZLL_PROFILE_ATTRIBUTES_H_


/*****************************************************************************
* Includes
*****************************************************************************/
#include "ZigBeeDataTypes.h"
#include "buffer_management.h"
#include "ZLL_defines.h"



/*****************************************************************************
* Global Constants
*****************************************************************************/
#ifdef g_NETWORK_SETUP_VIA_TOUCH_LINK_COMMISSIONING_c
#define g_APL_FREE_NWK_ADDR_RANGE_BEGIN_DEFAULT_VALUE_c             0x0001
#define g_APL_FREE_NWK_ADDR_RANGE_END_DEFAULT_VALUE_c               0xFFF7
#define g_APL_FREE_GROUPID_RANGE_BEGIN_DEFAULT_VALUE_c              0x0001
#define g_APL_FREE_GROUPID_RANGE_END_DEFAULT_VALUE_c                0xFEFF
#else
#define g_APL_FREE_NWK_ADDR_RANGE_BEGIN_DEFAULT_VALUE_c             0x0000
#define g_APL_FREE_NWK_ADDR_RANGE_END_DEFAULT_VALUE_c               0x0000
#define g_APL_FREE_GROUPID_RANGE_BEGIN_DEFAULT_VALUE_c              0x0000
#define g_APL_FREE_GROUPID_RANGE_END_DEFAULT_VALUE_c                0x0000


#endif /* g_NETWORK_SETUP_VIA_TOUCH_LINK_c*/



/*****************************************************************************
* Public Memory declarations
*****************************************************************************/
/*The aplFreeNwkAddrRangeBegin attribute range 0x0000 – 0xfff7
and contains the starting value for address assignment.
Address assignment capable devices should use and maintain this value when
assigning addresses via touchlink commissioning.
If the device is not address assignment capable or it has joined a
non-ZLL network through classical ZigBee joining mechanisms,
this attribute should be set to 0x0000.*/
extern uint16_t aplFreeNwkAddrRangeBegin;

/*contains the end value for address assignment*/
extern uint16_t aplFreeNwkAddrRangeEnd;

/* The aplFreeGroupIDRangeBegin attribute is an unsigned 16-bit integer in the
range 0x0000 – 0xfeff and contains the starting value of the free group
identifier range for address assignment capable devices. Address assignment
capable devices should use and maintain this value when assigning group
identifiers via touchlink commissioning. If the device is not address assignment
capable or it has joined a non-ZLL network through classical ZigBee
joining mechanisms, this attribute should be set to 0x0000.*/
extern uint16_t aplFreeGroupIdRangeBegin;
/*contains the end value for address assignment*/
extern uint16_t aplFreeGroupIdRangeEnd;

/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/
/* None */
/*****************************************************************************
* Public Functions
*****************************************************************************/

/* None */
/*****************************************************************************
* Private Functions
*****************************************************************************/

/* None */

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */



#endif /*_ZLL_PROFILE_ATTRIBUTES_H_*/
/*****************************************************************************
* End Of File                                       _ZLL_PROFILE_ATTRIBUTES_H_
*****************************************************************************/
