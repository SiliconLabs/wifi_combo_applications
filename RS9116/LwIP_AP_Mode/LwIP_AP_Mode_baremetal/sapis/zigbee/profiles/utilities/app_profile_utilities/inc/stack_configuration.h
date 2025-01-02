/*******************************************************************************
 * stack_configuration.h
 ********************************************************************************
 * Program Description:
 *     This file contains the macros that are used for configuring the stack.
 *
 *---------------------------------------------------------------------------**/

#ifndef STACK_CONFIGURATION_H
#define STACK_CONFIGURATION_H

/*******************************************************************************
 * Includes
 *---------------------------------------------------------------------------**/
#include "common_includes.h"


/*******************************************************************************
 * Global Constants
 *---------------------------------------------------------------------------**/
/*****************************************************************************/
/*                   ZigBee Stack Profile Settings                           */
/*****************************************************************************/

#if ( g_ZIGBEE_STACK_PROFILE_d == 1 )

 /******************************************************************************/
/*         Network Layer Information BaseSettings                             */
/******************************************************************************/

/* g_NWK_LINK_STATUS_PERIOD
 - This macro defines the link status period for coming out a link status
 frame from coordinator or router
 */

#if(g_NWK_LINK_STATUS_d == 1 )
#define g_NWK_START_LINK_STATUS_PERIOD                      0x1E8480
#endif

/*******************************************************************************
 g_NWK_MAX_BROADCAST_JITTER

 -  This macro defines the timedealy before relaying broadcast transmission. 	
 -  Here Value represented in 0x40 uS

 *****************************************************************************/

#if (g_NWK_ADDRESS_CONFLICT_RESOLUTION_d == 1)
#define g_NWK_MAX_BROADCAST_JITTER                          0xFA00
#endif

/*******************************************************************************
 g_APP_LINK_KEY_TIMEOUT_c
	- Timer value is used in handling request key in non preconfigured  Key mode.
	This macro is non configurable value.

 *****************************************************************************/

#define g_APP_LINK_KEY_TIMEOUT_c  0x0A



#define m_MINMUM_NO_OF_TXS_REQ_FOR_CALC_FAILURE_RATE_c  0x14
#define m_LOGICAL_CHANNEL_LENGTH_c                      0x01
#define m_STARTING_CHANNEL_c                            0x0B
#define m_MASK_FOR_ROUTER_IN_DEVICE_INFO_c              0x01

/******************************************************************************/
/*                        APS Layer Settings                                 */
/******************************************************************************/

/*****************************************************************************/
#endif                                 /* g_ZIGBEE_STACK_PROFILE_d */
/*****************************************************************************/

/******************************************************************************/
/*     Network Layer Settings Which are not defined in Stack Profile         */
/******************************************************************************/

/*
 g_MESH_ROUTING_CAPACITY_c

 - This parameter defines the capability of the device to support
 Mesh Routing.

 If set to 1, it supports Mesh Routing.

 If set to ZERO, it does not support Mesh Routing.

 */
#define g_MESH_ROUTING_CAPACITY_c          1
#if ( g_MESH_ROUTING_CAPACITY_c > 1 )
#error "Define g_MESH_ROUTING_CAPACITY_c as either 1 or 0"
#endif

/******************************************************************************/
/*       APS Layer Settings Which are not defined in Stack Profile           */
/******************************************************************************/




/*****************************************************************************/
/*                      Memory Allocation For Tables                       */
/*****************************************************************************/

/*
  g_NVM_TABLES_START_OFFSET_c

 -This defines the NVM Offset where tables are stored start of NVM
  2 KB Left for MAC and Start of Attributes
 */

#define m_ONE_KB_c			                0x400

//#ifdef	STM32L1XX_MD_PLUS
//
//  #define g_NVM_TABLES_START_OFFSET_c                     m_ONE_KB_c * 2
//
//  #define g_NVM_SAS_TABLE_START_OFFSET_c                  m_ONE_KB_c
//
//#else
//
//  #define g_NVM_TABLES_START_OFFSET_c                     m_ONE_KB_c * 2
//
//#endif  /* STM32L1XX_MD_PLUS */



/*
 m_NVM_SAVE_STATUS_BYTE_SIZE_c

 -This status bit specifies whether the Persistent data is stored in
 NVM or Not.

 */
#define g_NVM_SAVE_STATUS_BYTE_SIZE_c               0x02

/*
 m_NVM_SAVE_STATUS_BYTE_LOCATION_c

 -NVM Save status byte is placed in each persistent sector at Zero
 location.By default the location value as 0xFF.If Persistent data
 is stored in NVM this location value will be made as 0x01.
 */

#define m_NVM_SAVE_STATUS_BYTE_LOCATION_c           0x00



/* MACROS to set the write frequencies of each table to the NVM */
#define g_UPDATE_NEIGHBOR_TABLE_TO_NVM_COUNT_c          0x0Au

#define g_UPDATE_ROUTING_TABLE_TO_NVM_COUNT_c           0x03u

#define g_UPDATE_SECURITY_TABLE_TO_NVM_COUNT_c          0x03u

#define g_UPDATE_NIB_TABLE_TO_NVM_COUNT_c               0x03u

#define g_UPDATE_ZDO_INFO_TABLE_TO_NVM_COUNT_c          0x02u

#define g_UPDATE_ROUTE_RECORD_TABLE_TO_NVM_COUNT_c      0x05u

#define g_UPDATE_BIND_TABLE_TO_NVM_COUNT_c              0x01u

#define g_UPDATE_GROUP_TABLE_TO_NVM_COUNT_c             0x03u

#define g_UPDATE_ADDRESS_MAP_TABLE_TO_NVM_COUNT_c       0x03u

#define g_UPDATE_LINK_KEY_TABLE_TO_NVM_COUNT_c          0x01u

/* Frame counter should be frequently updated to avoid join failure
with security*/
#define g_UPDATE_LINK_KEY_FRM_COUNTER_TABLE_TO_NVM_COUNT_c 0x01u //0x64u



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

/* None */

/*******************************************************************************
 * End Of File
 *******************************************************************************/

#endif                                 /* STACK_CONFIGURATION_H */
