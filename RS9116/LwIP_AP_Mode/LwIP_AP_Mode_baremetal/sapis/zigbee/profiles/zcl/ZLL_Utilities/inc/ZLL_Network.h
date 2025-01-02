/******************************************************************************
* ZLL_Network.h
*******************************************************************************
* Program Description:
*
*
******************************************************************************/
#ifndef _ZLL_NETWORK_H_
#define _ZLL_NETWORK_H_

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ZigBeeDataTypes.h"
#include "buffer_management.h"
#include "ZCL_Functionality.h"
#include "stack_util.h"
#include "stack_common.h"

#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZLL_defines.h"
#include "ZLL_Commissioning_Cluster.h"
#include "ZLL_Main.h"
#include "ZLL_Interpan.h"
#endif


/*****************************************************************************
* Global Constants
*****************************************************************************/


/*****************************************************************************
* Public Memory declarations
*****************************************************************************/

extern ZLL_device_information_table_t ZLL_device_information_table[];

extern ZLL_groups_t ZLL_groups;




/* To keep track of paired device */
typedef struct ZLL_CallbackSection_Tag{
    bool identify_request_received;
    uint16_t identify_request_received_time;
    uint16_t identify_request_duration;
    bool zcl_add_group_flag;
    bool zcl_remove_group_flag;
    uint16_t recipient_network_address;
    uint8_t abort_scan;
    /* Paired device*/
    uint16_t paired_device;
} __attribute__((__packed__))ZLL_CallbackSection_t;

extern ZLL_CallbackSection_t ZLL_CallbackSection;
extern ZLL_network_address_assignment_t ZLL_network_addresses;
extern uint8_t g_ZLL_NVM_StatusFlag;
/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

extern bool ZLL_IsFactoryNew(void);

extern uint8_t ZLL_NetworkGetchannel(void);
extern uint8_t ZLL_NetworkSetchannel( uint8_t channel );

extern uint16_t ZLL_GetDeviceGroupIdentifier(uint8_t index);
extern void ZLL_GroupIdentifierAssignment(bool is_forming_network,
                                          uint8_t recipient_total_groupids);

extern uint16_t ZLL_NetworkAddressAssignment(bool initiator);
extern uint16_t ZLL_SplitNetworkAddresses(uint16_t *new_device_free_min,
                                   uint16_t *new_device_free_max);
extern void ZLL_SplitGroupIdentifiers(uint16_t *new_device_free_min,
                               uint16_t *new_device_free_max,
                               uint8_t recipient_total_groupids);
extern uint16_t ZLL_DeviceNetworkAddress();

extern void ParseInScanResponseAboutDevice(bool *pAddressAssignCapable);

void ZLL_SetDeviceNetworkAddress(uint16_t selfAddress);
#ifdef g_ZLL_LIGHT_CONTROLLER_c
extern void ZLL_Nwk_Verify_Valid_Channel( void );

#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c

#ifdef g_ZLL_LIGHT_CONTROLLER_c
uint8_t ZLL_IsInitiator( void );
void ZLL_SetInitiator( void );
void ZLL_SetIdentifyDuration(uint16_t duration);
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c

void ZLL_InitNwkAddress(void);




#endif /*_ZLL_NETWORK_H_*/
/*****************************************************************************
* End Of File                                      _ZLL_NETWORK_H_
*****************************************************************************/
