/******************************************************************************
* APP_ZLL_COMMISSIONING_CLUSTER_UTILITIES_H
*******************************************************************************
* Program Description:
*
*
******************************************************************************/

#ifndef APP_ZLL_COMMISSIONING_CLUSTER_UTILITIES_H
#define APP_ZLL_COMMISSIONING_CLUSTER_UTILITIES_H

/*****************************************************************************
* Includes
*****************************************************************************/

#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "apsde_interface.h"
#include "app_configuration_default.h"

/*****************************************************************************
* Global Constants
*****************************************************************************/



/******************************************************************************
* Memory Declarations
******************************************************************************/

#if ( g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
        ga_ZLL_Commissioning_Cluster_Server_Cluster_Info;
#endif

#if ( g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
        ga_ZLL_Commissioning_Cluster_Client_Cluster_Info;
#endif




/*****************************************************************************
* Public Memory declarations
*****************************************************************************/
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];

extern bool SentEndpointInfo ;

typedef struct End_point_information_record_list {
  uint16_t network_address;
  uint8_t end_point_identifier;
  uint16_t profile_identifier;
  uint16_t device_identifier;
  uint8_t version;
} __attribute__((__packed__)) End_point_information_record_list ;


typedef struct Get_end_point_list_response_field {
  uint8_t total;
  uint8_t start_index;
  uint8_t count;
  End_point_information_record_list end_point_information_record_list[g_MAX_NO_OF_ENDPOINTS_c];
} __attribute__((__packed__)) Get_end_point_list_response_field ;

/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

void App_Handle_ZLL_Commissioning_Cluster_Events( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer );

#ifdef g_ZLL_LIGHT_CONTROLLER_c
extern void ZLL_CreateSendEndpointInfo(uint16_t dest_addr,uint8_t loop_index);
extern void ZLL_InitiateEndpointNotification(uint16_t dest_addr);
#endif /* #ifdef g_ZLL_LIGHT_CONTROLLER_c*/

#endif /*APP_ZLL_COMMISSIONING_CLUSTER_UTILITIES_H*/
/*****************************************************************************
* End Of File
*****************************************************************************/
