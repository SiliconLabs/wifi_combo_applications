/******************************************************************************
* ZLL_network.c
*******************************************************************************
* Program Description:
*
*
******************************************************************************/


/*****************************************************************************
* Includes
*****************************************************************************/
#include "ZigBeeDataTypes.h"
#include <stdbool.h>
#include "buffer_management.h"
#include "ZCL_Functionality.h"
#include "stack_util.h"
#include "stack_common.h"
#include "zdp_interface.h"
//#include"zb_hil_intf.h"
#include "memory_utility.h"
#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZLL_Configuration.h"
#include "ZLL_defines.h"
#include "ZLL_Commissioning_Cluster.h"
#include "ZLL_Main.h"
#include "ZLL_NVM_Utilities.h"
#include "ZLL_Interpan.h"
#include "ZLL_Network.h"
#include "ZLL_State_Machine.h"
#include "zdo_interface.h"
#endif

/*****************************************************************************
* Global Constants
*****************************************************************************/

#define STATE_TRANSITION(prefix, prevState, nextState) \
{ ZLL_State.state = prefix##_##nextState; ZLL_State.substate = prefix##_##nextState | 0x0001;}
#define GET_CURRENT_STATE() ZLL_State.state
#define GET_CURRENT_SUBSTATE() ZLL_State.substate
#define ZLL_SUBSTATE_ANY 0xFFFFFFFF


/*****************************************************************************
* Public Memory declarations
*****************************************************************************/

/* ZLL device information table ,no of endpoints == no of endpoints supported
by ZigBee node */
ZLL_device_information_table_t ZLL_device_information_table[g_ZLL_CONFIG_SUB_DEVICES_NUMBER_c];

/* ZLL network address and its ranges  */
ZLL_network_address_assignment_t ZLL_network_addresses;
/* ZLL gROUP Address range */
ZLL_groups_t ZLL_groups;

const uint8_t ZLLDefaultPrimaryScanChannelList[g_ZLL_PRIMARY_CHANNELS_NUMBER_c]
	= { g_ZLL_PRIMARY_CHANNELS_c };
const uint8_t ZLLDefaultSecondaryScanChannelList[g_ZLL_SECONDARY_CHANNELS_NUMBER_c]
	= { g_SECONDARY_CHANNELS_c };
uint8_t ZLLScanChannelListCount[g_ZLL_PRIMARY_CHANNELS_NUMBER_c+
g_ZLL_SECONDARY_CHANNELS_NUMBER_c] = {0} ;
//uint8_t ZLLSecondaryScanChannelListCount[g_ZLL_SECONDARY_CHANNELS_NUMBER_c] = {0} ;
/* ZLL callback section  */

ZLL_CallbackSection_t ZLL_CallbackSection =
{g_FALSE_c,0,g_ZLL_DEFAULT_IDENTIFY_MODE_c,g_FALSE_c,g_FALSE_c,
0x0000, g_FALSE_c,0x0000};

#ifdef g_ZLL_LIGHT_CONTROLLER_c
static uint8_t g_Initiator_Device = g_FALSE_c;
#endif
uint8_t g_ZLL_NVM_StatusFlag = 0x00;


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


/* ZLL utility to determine if a device is factory new or not */
bool ZLL_IsFactoryNew(void)
{
	uint8_t i = rsi_zigb_network_state();
	
    //if ((ZigBeeNetworkState() == g_ZigBeeNotPartOfNWK_c)
    //    || (ZigBeeNetworkState() == g_ZigBeePerformingLeaveFromNWK_c)){
    //if (((i = rsi_zigb_network_state()) == g_ZigBeeNotPartOfNWK_c)
    //    || ((i = rsi_zigb_network_state()) == g_ZigBeePerformingLeaveFromNWK_c)){
    if (((i ) == g_ZigBeeNotPartOfNWK_c)
        || ((i) == g_ZigBeePerformingLeaveFromNWK_c)){

        /* device is factory new device*/
        return g_TRUE_c;
    }
    else {
        return g_FALSE_c;
    }
}

/*****************************************************************************/
bool ZLL_IsAddressAssignmentCapable(void)
{
    /* doubt whether router device also can assign address */

//    if (g_ZLL_DEVICE_TYPE_c == g_ZLL_LOGICAL_TYPE_END_DEVICE_c ||
//        g_ZLL_DEVICE_TYPE_c == g_ZLL_LOGICAL_TYPE_ROUTER_c) {
//        return g_TRUE_c;
//    }
//    else{
//        return g_FALSE_c;
//    }

    /* Address Assignement capability is checked based on
    whether ZLL commissioning is supported as Client or server ,
    basically Controller device which support as client are address
    assignment capable */

    /* Editor's comment on this
    Being address assignment (AA) capable is an implementation issue but
    typically a controller will be AA capable.  There is no mapping
    between ZLL device types (controller/target) and ZigBee logical devices
    (router/end device).It really depends on whether a device supports the
    commissioning client (controller device) or not (lighting device) -
    see table 1 in the ZLL spec.*/

#ifdef g_ZLL_LIGHT_CONTROLLER_c

        return g_TRUE_c;
#elif g_ZLL_LIGHT_DEVICE_c
        return g_FALSE_c;
#else
	return g_FALSE_c;
#endif

}

/*****************************************************************************/

/* zLl get channel function */
uint8_t ZLL_NetworkGetchannel(void)
{
    //return (rsi_802154_get_channel_num());
    //return (rsi_zigb_get_operating_channel());
    return (rsi_zigb_zll_get_oper_channel());
}

/*****************************************************************************/

/* zLl set channel function */
uint8_t ZLL_NetworkSetchannel( uint8_t channel )
{
    //return (rsi_802154_hal_set_channel(channel));
    //return (rsi_zigb_set_operating_channel(channel));
    return(rsi_zigb_zll_set_oper_channel(channel));
}

/*****************************************************************************/

/* returns network address for the receipient device ,which is forming the
network or joining the network */

/* ZLL set network addresses range */
void ZLL_SetNetworkAddressesRange(uint16_t free_min, uint16_t free_max)
{
    /* For now , end device and router are ddress assignment capable */

    /* checking whether device is address assignment capable or not */
    if (g_TRUE_c == ZLL_IsAddressAssignmentCapable()) {

        ZLL_network_addresses.free_min = free_min;
        ZLL_network_addresses.free_max = free_max;
    }
}

/*****************************************************************************/

/* returns the self device's ZLL network address assigned following the
ZLL network address assignment policy */
uint16_t ZLL_DeviceNetworkAddress()
{
    return  ZLL_network_addresses.device_network_address;
}
/*****************************************************************************/
void ZLL_SetDeviceNetworkAddress(uint16_t selfAddress)
{
    ZLL_network_addresses.device_network_address = selfAddress;

}

/*****************************************************************************/

/* return the last assigned network address */
uint16_t ZLL_LastAssignedNetworkAddress(void)
{
    return  (ZLL_network_addresses.free_min -1);
}
/*****************************************************************************/
/* It returns a new network address to be assigned to the specific destination
    device
    NOTE: this function can be called only by an address assignment
    device capable */
uint16_t ZLL_NetworkAddressAssignment(bool initiator)
{
    /* Check address assign capable or not*/
    if(g_FALSE_c == ZLL_IsAddressAssignmentCapable()){
#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
        printf(" Device is not capable of assigning address \n\r");
#endif
    }
    else{
        if (initiator) {
            /* Set the self short address device network address,
            this is applicable only initiator*/
            ZLL_network_addresses.device_network_address = ZLL_network_addresses.free_min;
            /* increment free_min */
            ZLL_network_addresses.free_min +=1;
        }
        /* 1st free address in network address to be assigned */
        ZLL_network_addresses.device_network_address =
            ZLL_network_addresses.free_min;

        /* Range to be updated by incrementing by one*/
        if(ZLL_network_addresses.free_min <= ZLL_network_addresses.free_max){
            ZLL_network_addresses.free_min += 1;
        }
        /* Store the current network addresses range in non volatile memory */


        ZLL_NVM_Parameters.network_address_begin =
          ZLL_network_addresses.free_min ;
        ZLL_NVM_Parameters.network_address_end =
          ZLL_network_addresses.free_max;

//        ZLL_SaveNVMParameters();
        g_ZLL_NVM_StatusFlag |= g_ZLL_NVM_SHORT_ADDR_ASSIGNMENT_BIT;

        /* 0x0000 is reserved , 0x0001 will be used by initiator so address assigned
        should be greater than 1*/
        uint16_t recipient_addr = 0x0000;
        if(ZLL_network_addresses.free_min -1 == 0x0001){
          recipient_addr = ZLL_network_addresses.free_min ;
        }
        else if(ZLL_network_addresses.free_min -1 > 0x0001){
          recipient_addr = ZLL_network_addresses.device_network_address;
        }
        return (recipient_addr);
    }
    return 0;
}

/*****************************************************************************/

/* Split the current network addresses range for an address assignment
capable device */
uint16_t ZLL_SplitNetworkAddresses(uint16_t *new_device_free_min,
                                   uint16_t *new_device_free_max)
{

    /* ZLL_addresses.free_min will be assigned to joining device
    (which is address address assignment capable),
    incremented value is used to split the address and assigned */
    ZLL_network_addresses.free_min +=1;

    /* recipient device is address assignment capable so split network
    addresses and assign to it */
    *new_device_free_min = (ZLL_network_addresses.free_max + ZLL_network_addresses.free_min -1)/2;
    *new_device_free_max = ZLL_network_addresses.free_max;

    ZLL_network_addresses.free_max = *new_device_free_min -1;

    /* Store the current network addresses range in non volatile memory */
    ZLL_NVM_Parameters.network_address_begin =
          ZLL_network_addresses.free_min ;
    ZLL_NVM_Parameters.network_address_end =
          ZLL_network_addresses.free_max;
    g_ZLL_NVM_StatusFlag |= g_ZLL_NVM_SHORT_ADDR_ASSIGNMENT_BIT;
//    ZLL_SaveNVMParameters();
    /* return the network address assigned to the device */
    return (ZLL_network_addresses.free_min -1);
}
/*****************************************************************************/

/* Initializing ZLL set groups identifier range */
void ZLL_SetGroupIdentifierRange(uint16_t free_min, uint16_t free_max)
{

    /* range of group ids assigned to the device itself */
    ZLL_groups.device_start_group_id = 0;
    ZLL_groups.device_end_group_id = 0;

    /* range of the overall group-id's assigned to the different recipients
    devices */
    ZLL_groups.recipients_start_group_id = 0;
    ZLL_groups.recipients_end_group_id = 0;

    /* range of free group ids */
    ZLL_groups.free_min = free_min;
    ZLL_groups.free_max = free_max;
}

/*****************************************************************************/

/* Set group identifiers to the device itself or to the current recipient device */
void ZLL_GroupIdentifierAssignment(bool is_forming_network, uint8_t recipient_total_groupids)
{

    /* Check address assign capable or not*/
    if(g_TRUE_c == ZLL_IsAddressAssignmentCapable()){
#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
        printf(" Device is not capable of assigning address \n\r");
#endif
    }
    else{
        /* Device is forming the network: assign range ids to itself */
        if (is_forming_network) {
            /* start group id start and end is for node and not for particular
            device in a node
            g_ZLL_TOTAL_GROUP_IDENTIFIER_NUMBER_c tells maximum groupid's
            for node */
            ZLL_groups.device_start_group_id =  ZLL_groups.free_min;
            ZLL_groups.device_end_group_id = ZLL_groups.device_start_group_id +
                g_ZLL_TOTAL_GROUP_IDENTIFIER_NUMBER_c  -1;
            /* after assigning ,free min is used for joining device*/
            ZLL_groups.free_min += g_ZLL_TOTAL_GROUP_IDENTIFIER_NUMBER_c;
        }
        /* If recepient is asking for free froup ids , assignment shd start from
        free_min */
        if (recipient_total_groupids !=0){
            ZLL_groups.recipients_start_group_id = ZLL_groups.free_min;
            /* end groupId = start + no of requested id's*/
            ZLL_groups.recipients_end_group_id = ZLL_groups.recipients_start_group_id +
                recipient_total_groupids -1;
            /* If requested*/
            ZLL_groups.free_min += recipient_total_groupids;
        }
        /* Store the current group ids range in non volatile memory */
        ZLL_NVM_Parameters.group_address_begin = ZLL_groups.free_min ;
        ZLL_NVM_Parameters.group_address_end = ZLL_groups.free_max ;
        g_ZLL_NVM_StatusFlag |= g_ZLL_NVM_GRP_ADDR_ASSIGNMENT_BIT;
//        ZLL_SaveNVMParameters();
    }
}
/*****************************************************************************/

/* Split the current group ids range for an address assignment capable device */
void ZLL_SplitGroupIdentifiers(uint16_t *new_device_free_min,
							   uint16_t *new_device_free_max,
							   uint8_t recipient_total_groupids)
{
  /* Set group ids assigned to the current new recipient device based on the recipient_total_groupids value
     coming from the scan response */
  ZLL_groups.recipients_start_group_id = ZLL_groups.free_min;
  ZLL_groups.recipients_end_group_id = ZLL_groups.free_min + recipient_total_groupids -1;
  ZLL_groups.free_min += recipient_total_groupids;

  /* recipient device is address assignment capable: split network addresses and assign to it */
  *new_device_free_min = (ZLL_groups.free_max + ZLL_groups.free_min -1)/2;
  *new_device_free_max = ZLL_groups.free_max;

  ZLL_groups.free_max = *new_device_free_min -1;

  /* Store the current group ids range in non volatile memory */
  ZLL_NVM_Parameters.group_address_begin = ZLL_groups.free_min ;
  ZLL_NVM_Parameters.group_address_end = ZLL_groups.free_max ;
  g_ZLL_NVM_StatusFlag |= g_ZLL_NVM_GRP_ADDR_ASSIGNMENT_BIT;
//  ZLL_SaveNVMParameters();
}
/*****************************************************************************/
/* return one of the group ids assigned to the device  */
uint16_t ZLL_GetDeviceGroupIdentifier(uint8_t index)
{
    if((index < 1) || (index >g_ZLL_TOTAL_GROUP_IDENTIFIER_NUMBER_c)){
#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
        printf("Unsupported Index\n\r");
        return 0;
#endif
    }
  return (ZLL_groups.device_start_group_id + index - 1);
}

/*****************************************************************************/
/* returns the device Information such as its network address,
and free nwk address range, group range */
void ZLL_GetNetworkGroupAddessInfo(void)
{
    if (g_TRUE_c == ZLL_IsAddressAssignmentCapable()){
    /* Only address assignment capable device will have nwk addr range*/
#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
        printf(" NWK_short_addr:%2x , nwk_min: %2x, nwk_max:%2x \r\n ",
               ZigBeeGetSelfShortAddress(),ZLL_network_addresses.free_min,
               ZLL_network_addresses.free_max);
        printf(" grp_min: %2x, grp_max:%2x \r\n ",ZLL_groups.free_min,
           ZLL_groups.free_max);
#endif
    }
    else {
#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf(" NWK_short_addr:%2x \r\n ",
               ZigBeeGetSelfShortAddress());
    printf(" grp_min: %2x, grp_max:%2x \r\n ",ZLL_groups.free_min,
       ZLL_groups.free_max);
#endif
  }
}

uint8_t channel ;
/*****************************************************************************
* Private Functions
*****************************************************************************/
#ifdef g_ZLL_LIGHT_CONTROLLER_c
void ZLL_Nwk_Verify_Valid_Channel( void )
{
  uint8_t index = 0x00;
  uint8_t temp_channel_list[g_ZLL_PRIMARY_CHANNELS_NUMBER_c +
    g_ZLL_SECONDARY_CHANNELS_NUMBER_c];

  memUtils_memCopy(temp_channel_list,(uint8_t*)ZLLDefaultPrimaryScanChannelList,
						 g_ZLL_PRIMARY_CHANNELS_NUMBER_c);
  memUtils_memCopy(&temp_channel_list[g_ZLL_PRIMARY_CHANNELS_NUMBER_c],(uint8_t*)ZLLDefaultSecondaryScanChannelList,
						 g_ZLL_SECONDARY_CHANNELS_NUMBER_c);


  for( index = 0x00; index < g_ZLL_PRIMARY_CHANNELS_NUMBER_c + g_ZLL_SECONDARY_CHANNELS_NUMBER_c;
 	 index++ )
  {

	if( temp_channel_list[index] == (ZLL_NetworkGetchannel()) )
      {
        ZLLScanChannelListCount[index]++;
        if(  (( index == 0x00 ) && ( ZLLScanChannelListCount[index] >= ( g_ZLL_SCANS_NUMBER_ON_FIRST_CHANNEL_c + 1 )))
          || (( index != 0x00 ) && ( ZLLScanChannelListCount[index] >= ( 1 ))) )
        {
              if( index == ( g_ZLL_PRIMARY_CHANNELS_NUMBER_c + g_ZLL_SECONDARY_CHANNELS_NUMBER_c - 1) )
              {
                  break;
              }
              ZLL_NetworkSetchannel( temp_channel_list[index + 1] );

              return;
        }
        else
        {
            return;
        }
      }
  }
  ZLL_Change_Controller_State(g_I_INITIAL_STATE_c);
  ZLL_NetworkSetchannel( ZLLDefaultPrimaryScanChannelList[0] );
  memUtils_memSet(ZLLScanChannelListCount,0x00,g_ZLL_PRIMARY_CHANNELS_NUMBER_c+
				  g_ZLL_SECONDARY_CHANNELS_NUMBER_c);
  TouchLinkCommissioningTriggered = g_FALSE_c;

}
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c



/*****************************************************************************/
void ZLL_InitNwkAddress(void)
{
  ZLL_network_addresses.device_network_address = 0x0000;
  ZLL_network_addresses.free_min = g_ZLL_DEFAULT_NWK_FREE_MIN_ADDRESS_c;
  ZLL_network_addresses.free_max = g_ZLL_DEFAULT_NWK_FREE_MAX_ADDRESS_c;
}

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */

#ifdef g_ZLL_LIGHT_CONTROLLER_c
uint8_t ZLL_IsInitiator( void )
{
  return g_Initiator_Device;
}
/*****************************************************************************/

void ZLL_SetInitiator( void )
{
   g_Initiator_Device = g_TRUE_c;
}
/*****************************************************************************/
void ZLL_SetIdentifyDuration(uint16_t duration)
{
    pTouchLinkInitiatiorSectionMemory->identify_duration = duration;
    if(duration == 0x0000){
      /* exit identify mode */
      /*create device identify request to the target device only to exit
        identify mode
      else
      during touchlink process sending identify request is taken care*/
          ZLL_Create_Identify_Request
            ( pTouchLinkInitiatiorSectionMemory->Target_long_address );

    }
}



#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c
/*****************************************************************************
* End Of File                                      ZLL_network.c
*****************************************************************************/
