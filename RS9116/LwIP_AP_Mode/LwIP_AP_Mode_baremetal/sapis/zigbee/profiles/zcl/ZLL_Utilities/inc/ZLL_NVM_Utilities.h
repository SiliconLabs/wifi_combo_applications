/******************************************************************************
* _ZLL_NVM_UTILITIES_H_
*******************************************************************************
* Program Description:
*
*
******************************************************************************/
#ifndef _ZLL_NVM_UTILITIES_H_
#define _ZLL_NVM_UTILITIES_H_


/*****************************************************************************
* Includes
*****************************************************************************/
#include "ZigBeeDataTypes.h"

#include "common_includes.h"
#include "buffer_management.h"
#include "ZLL_defines.h"


#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZLL_Commissioning_Cluster.h"
#include "ZLL_Interpan.h"
#endif



/*****************************************************************************
* Global Constants
*****************************************************************************/
#define g_ZLL_NVM_SHORT_ADDR_ASSIGNMENT_BIT       0x01
#define g_ZLL_NVM_GRP_ADDR_ASSIGNMENT_BIT         0x02

/*****************************************************************************
* Public Memory declarations
*****************************************************************************/

typedef struct ZLL_NVM_Parameters_Tag{
#ifdef g_ZLL_LIGHT_CONTROLLER_c
    uint8_t controller_device_state;
#endif /* #ifdef g_ZLL_LIGHT_CONTROLLER_c*/
#ifdef g_ZLL_LIGHT_DEVICE_c
    uint8_t light_device_state;
#endif /* g_ZLL_LIGHT_DEVICE_c*/
    uint16_t network_address_begin;
    uint16_t network_address_end;
    uint16_t group_address_begin;
    uint16_t group_address_end;
}__attribute__((__packed__)) ZLL_NVM_Parameters_t;

extern ZLL_NVM_Parameters_t ZLL_NVM_Parameters;


/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

void ZLL_SaveNVMParameters(void);
void ZLL_RestoreNVMParameters(void);
#endif /*_ZLL_NVM_UTILITIES_H_*/
/*****************************************************************************
* End Of File                                       _ZLL_NVM_UTILITIES_H_
*****************************************************************************/
