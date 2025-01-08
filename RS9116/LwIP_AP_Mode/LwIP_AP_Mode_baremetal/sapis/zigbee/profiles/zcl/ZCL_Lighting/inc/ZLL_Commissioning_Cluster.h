/******************************************************************************
* ZLL_COMMISSIONING_CLUSTER_H
*******************************************************************************
* Program Description:
*
*
******************************************************************************/

#ifndef ZLL_COMMISSIONING_CLUSTER_H
#define ZLL_COMMISSIONING_CLUSTER_H

/*****************************************************************************
* Includes
*****************************************************************************/

#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"



/*****************************************************************************
* Global Constants
*****************************************************************************/
/* Cluster ID of ZLL Commsissioning Cluster */
#define g_ZLL_COMMISSIONING_CLUSTER_c                       0x1000

/* ZLL Commsissioning Cluster dont have attributes */


/* enum for ZLL Commissioning Cluster Server Generated Commands */
enum ZllCommissioningServerCommands {
    /* ZLL Commissioning Touch Link Commands*/
    g_ZLL_Scan_Response_Command_c = 0x01,
    g_ZLL_Device_Information_Response_Command_c = 0x03,
    g_ZLL_Nwk_Start_Response_Command_c = 0x11,
    g_ZLL_Network_Join_Router_Response_Command_c = 0x13,
    g_ZLL_Network_Join_End_Device_Response_Command_c = 0x15,
    /* ZLL Commissioning Utility Commands*/
    g_ZLL_Endpoint_Information_Response_Command_c = 0x40,
    g_ZLL_Get_Group_Identifiers_Response_Command_c = 0x41,
    g_ZLL_Get_Endpoint_List_Response_Command_c = 0x42
};


/* enum for ZLL Commissioning Cluster Client Generated Commands */
enum ZllCommissioningClientCommands {
    /* Following is the ZLL Commissioning Touch link Commands*/
    g_ZLL_Scan_Req_Command_c = 0x00,
    g_ZLL_Device_Information_Req_Command_c = 0x02,
    g_ZLL_Identify_Req_Command_c = 0x06,
    g_ZLL_Reset_To_Factory_New_Req_Command_c = 0x07,
    g_ZLL_Network_Start_Req_Command_c = 0x10,
    g_ZLL_Network_Join_Router_Req_Command_c = 0x12,
    g_ZLL_Network_Join_End_Device_Req_Command_c= 0x14,
    g_ZLL_Network_Update_Req_Command_c = 0x16,
    /* All other values in the range 0x00 – 0x3f are reserved */
    /* Following is the ZLL Commissioning Utility Commands*/
    g_ZLL_Get_Group_Identifier_Req_Command_c= 0x41,
    g_ZLL_Get_Endpoint_List_Req_Command_c = 0x42
    /*All other values in the range 0x40 – 0xff are reserved */
};

/* enum for  Cluster Events Identifiers*/
enum applianceStatisticsCluserEvents {
    g_ZLL_Scan_Req_Event_c = 0x13,
    g_ZLL_Device_Information_Req_Event_c,
    g_ZLL_Identify_Req_Event_c,
    g_ZLL_Reset_To_Factory_New_Req_Event_c,
    g_ZLL_Network_Start_Req_Event_c,
    g_ZLL_Network_Join_Router_Req_Event_c,
    g_ZLL_Network_Join_End_Device_Req_Event_c,
    g_ZLL_Network_Update_Req_Event_c,
    /* All other values in the range 0x00 – 0x3f are reserved */
    /* Following is the ZLL Commissioning Utility Commands*/
    g_ZLL_Get_Group_Identifier_Req_Event_c,
    g_ZLL_Get_Endpoint_List_Req_Event_c,
    /* ZLL Commissioning Touch Link Commands*/
    g_ZLL_Scan_Response_Event_c,
    g_ZLL_Device_Information_Resonse_Event_c,
    g_ZLL_Start_Response_Event_c,
    g_ZLL_Network_Join_Router_Response_Event_c,
    g_ZLL_Network_Join_End_Device_Response_Event_c,
    /* ZLL Commissioning Utility Commands*/
    g_ZLL_Endpoint_Information_Response_Event_c,
    g_ZLL_Get_Group_Identifiers_Response_Event_c,
    g_ZLL_Get_Endpoint_List_Response_Event_c
};


typedef struct Get_group_identifier_record_list_field {
    uint8_t start_index;
}__attribute__((__packed__)) Get_group_identifier_record_list_field;

typedef struct Get_end_point_record_list_field {
    uint8_t start_index;
}__attribute__((__packed__)) Get_end_point_record_list_field;


typedef struct End_point_information_response_field {
  uint8_t IEEE_address[8];
  uint16_t network_address;
  uint8_t end_point_identifier;
  uint16_t profile_identifier;
  uint16_t device_identifier;
  uint8_t version;
}__attribute__((__packed__)) End_point_information_response_field ;

typedef struct Group_information_record_list {
  uint16_t group_identifier;
  uint8_t group_type;
}__attribute__((__packed__)) Group_information_record_list ;

typedef struct Get_group_identifier_response_field {
  uint8_t total;
  uint8_t start_index;
  uint8_t count;
  Group_information_record_list group_information_record_list[1];
}__attribute__((__packed__)) Get_group_identifier_response_field ;






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

/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* ZCL_Handle_Commissioning_Cluster_Cmd
*****************************************************************************
*
* Return Value :
*		uint8_t - Gives the status of the command operation
* Input Parameters:
*	p_cluster_data - pointer to information about the cluster and attributes
*
* Output Parameters:
*
* Scope of the function:
*  This function is called within ZCL_Handle_Cluster_Specific_Cmd() function,
*  when a Cluster Command is received in the Data indication.
*
* Purpose of the function:
*	This function handles the Color control cluster command received in the
*  Data indication
*
* Before the function is called:
*     None
* After the function is called:
*     None
*
****************************************************************************/
uint8_t ZLL_Handle_Commissioning_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );





#endif /*ZLL_COMMISSIONING_CLUSTER_H*/
/*****************************************************************************
* End Of File
*****************************************************************************/
