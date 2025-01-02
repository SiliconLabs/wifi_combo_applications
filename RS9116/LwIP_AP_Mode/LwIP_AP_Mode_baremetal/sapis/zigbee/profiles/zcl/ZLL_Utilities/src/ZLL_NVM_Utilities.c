/******************************************************************************
* ZLL_NVM_Utilites.c
*******************************************************************************
* Program Description:
*
*
******************************************************************************/


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
#include "ZLL_Interpan.h"
#include "ZLL_State_Machine.h"
#include "ZLL_NVM_Utilities.h"
#include "ZLL_Network.h"
#endif
#include "app_configuration_default.h"

/*****************************************************************************
* Global Constants
*****************************************************************************/


/*****************************************************************************
* Public Memory declarations
*****************************************************************************/


ZLL_NVM_Parameters_t ZLL_NVM_Parameters;
uint8_t ZLL_NVM_Count = 0;
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

void ZLL_SaveNVMParameters(void)
{
    uint8_t flag = g_ZLL_NVM_StatusFlag;
    if( g_DEVICE_READY_STATE_c == (ZLL_Get_DeviceState()) )
    {
      if( ( g_ZLL_NVM_StatusFlag & g_ZLL_NVM_SHORT_ADDR_ASSIGNMENT_BIT ) ||
         ( g_ZLL_NVM_StatusFlag & g_ZLL_NVM_GRP_ADDR_ASSIGNMENT_BIT ) )
      {
          ZLL_SaveEssentialParams((uint8_t*)&ZLL_NVM_Parameters,
                              g_SIZE_OF_ZLL_NVM_PARAMS_c);
          /*Reset the address and group bits*/
          g_ZLL_NVM_StatusFlag &= ~(flag);
      }
    }
}

/*****************************************************************************/

void ZLL_RestoreNVMParameters(void)
{
    ZLL_NVM_Parameters_t ZLL_NVM_Parameters_temp;

    /* restore from NVM*/
    ZLL_RestoreEssentialParams((uint8_t*)&ZLL_NVM_Parameters_temp,
                               sizeof(ZLL_NVM_Parameters_t));

    /* assign restored values */
#ifdef g_ZLL_LIGHT_CONTROLLER_c
    ZLL_NVM_Parameters.controller_device_state =
        ZLL_NVM_Parameters_temp.controller_device_state;
#endif /* g_ZLL_LIGHT_CONTROLLER_c*/

#ifdef g_ZLL_LIGHT_DEVICE_c
    ZLL_NVM_Parameters.light_device_state =
        ZLL_NVM_Parameters_temp.light_device_state;
#endif /* g_ZLL_LIGHT_DEVICE_c      */

    /* Group Address begin*/
    ZLL_groups.free_min = ZLL_NVM_Parameters.group_address_begin =
        ZLL_NVM_Parameters_temp.group_address_begin;
    /* Group Address end*/
    ZLL_groups.free_max = ZLL_NVM_Parameters.group_address_end =
        ZLL_NVM_Parameters_temp.group_address_end;
    /* Network Address begin*/
    ZLL_network_addresses.free_min = ZLL_NVM_Parameters.network_address_begin =
        ZLL_NVM_Parameters_temp.network_address_begin;
    /* Network Address end*/
    ZLL_network_addresses.free_max = ZLL_NVM_Parameters.network_address_end =
        ZLL_NVM_Parameters_temp.network_address_end;
}
/*****************************************************************************
* Private Functions
*****************************************************************************/

/* None */

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */




/*****************************************************************************
* End Of File
*****************************************************************************/
