/******************************************************************************
* ZCL_Tunneling_Cluster.h
******************************************************************************
* Program Description:
* This file contains the ZCL_Tunneling_Cluster attribute and command
* information
******************************************************************************/
#ifndef _ZCL_TUNNELING_CLUSTER_H_
#define _ZCL_TUNNELING_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_TUNNELING_CLUSTER_c                                      0x0704
#define g_CLOSE_TUNNEL_TIMEOUT_c                                   0x0000

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Tunneling_Server_Cluster_Info;
#endif				/* g_TUNNELING_SERVER_CLUSTER_ENABLE_d */

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Tunneling_Client_Cluster_Info;
#endif				/* g_TUNNELING_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
Tunneling cluster structure - attributes
-----------------------------------------------------------------------------*/
typedef struct Tunneling_Cluster_Tag  {
	uint8_t CloseTunnelTimeout[2];
}__attribute__((__packed__))Tunneling_Cluster_t;

/* Tunneling Cluster command structure */
typedef struct RequestTunnelCommand_tag {
	uint8_t ProtocolId;
    uint8_t ManufacturerCode[2];
    uint8_t FlowControlSupport;
}__attribute__((__packed__))RequestTunnelCommand_t;

typedef struct TransferDataCommand_tag {
	uint8_t TunnelId[2];
    uint8_t Data[1];
}__attribute__((__packed__))TransferDataCommand_t;

typedef struct TransferDataErrorCommand_tag {
	uint8_t TunnelId[2];
    uint8_t TransferDataStatus;
}__attribute__((__packed__))TransferDataErrorCommand_t;

typedef struct AckTransferDataCommand_tag  {
	uint8_t TunnelId[2];
    uint8_t NumberOfBytesLeft[2];
}__attribute__((__packed__))AckTransferDataCommand_t;

typedef struct ReadyDataCommand_tag {
	uint8_t TunnelId[2];
    uint8_t NumberOfBytesLeft[2];
}__attribute__((__packed__))ReadyDataCommand_t;


typedef struct CloseTunnelCommand_tag   {
	uint8_t TunnelId[2];
}__attribute__((__packed__))CloseTunnelCommand_t;

typedef struct RequestTunnelResponseCommand_tag {
	uint8_t TunnelId[2];
    uint8_t TunnelStatus;
}__attribute__((__packed__))RequestTunnelResponseCommand_t;

/***********************************************	
 Updates of SE 1.1 B
 **********************************************/
#if(g_SE_PROFILE_1_2_d == 1)
typedef struct TunnelClosureNotificCommand_tag
{
	uint8_t TunnelId[2];
}__attribute__((__packed__))TunnelClosureNotificCommand_t;

typedef struct ProtocolField_tag {
	uint8_t ManufactureCode[2];
    uint8_t ProtocolId;
}__attribute__((__packed__)) ProtocolField_t;

typedef struct SuppTunnelProtocolsRespCommand_tag{

 	uint8_t ProtocolCompleteList;
	uint8_t ProtocolCount;
	ProtocolField_t ProtocolList[1];
}__attribute__((__packed__))SuppTunnelProtocolsRespCommand_t;

#endif    /*g_SE_PROFILE_1_2_d */


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef enum Tunneling_Cluster_Event_tag
{
    g_RequestTunnel_Event_c = 0xC0,
    g_ServerTransferData_Event_c,
    g_ServerTransferDataError_Event_c,
    g_ServerAckTransferData_Event_c,
    g_ServerReadyData_Event_c,
	g_CloseTunnel_Event_c,
    g_ClientTransferData_Event_c,
    g_ClientTransferDataError_Event_c,
    g_ClientAckTransferData_Event_c,
    g_ClientReadyData_Event_c,	
    g_RequestTunnelResponse_Event_c
/***********************************************	
 Updates of SE 1.1 A
 **********************************************/	
#if ( g_SE_PROFILE_1_2_d == 1 )
	,g_SupportedTunnelProtocolsResponse_Event_c
	,g_TunnelClosureNotification_Event_c
	,g_GetSupportedTunnelProtocolsCommand_Event_c
#endif	
	
}Tunneling_Cluster_Event_t;

typedef enum Tunneling_Server_Cluster_Command_tag
{
	g_RequestTunnelResponseCommand_c = 0x00,
	g_ServerTransferDataCommand_c,
	g_ServerTransferDataErrorCommand_c,
    g_ServerAckTransferDataCommand_c,
    g_ServerReadyDataCommand_c
/***********************************************	
 Updates of SE 1.1 A
 **********************************************/	
#if ( g_SE_PROFILE_1_2_d == 1 ) 		
	 ,g_SupportedTunnelProtocolsResponse_c = 0x05
	 ,g_TunnelClosureNotification_c	
#endif 									/*g_SE_PROFILE_1_2_d*/		
		
}Tunneling_Server_Cluster_Command_t;

typedef enum Tunneling_Client_Cluster_Command_tag
{
	g_RequestTunnelCommand_c = 0x00,
	g_CloseTunnelCommand_c,
	g_ClientTransferDataCommand_c,
    g_ClientTransferDataErrorCommand_c,
    g_ClientAckTransferDataCommand_c,
    g_ClientReadyDataCommand_c
/***********************************************	
 Updates of SE 1.1 A
 **********************************************/	
#if ( g_SE_PROFILE_1_2_d == 1 ) 	
	,g_GetSupportedTunnelProtocolsCommand_c =0x06
#endif	
}Tunneling_Client_Cluster_Command_t;

typedef enum TunnelStatus_tag
{
    g_Tunnel_Success_c                 = 0x00,
    g_Tunnel_Busy_c                    = 0x01,
    g_Tunnel_NoMoreTunnelIDs_c         = 0x02,
    g_Tunnel_ProtocolNotSupported_c    = 0x03,
    g_Tunnel_FlowControlNotSupported_c = 0x04
}TunnelStatus_t;

typedef enum TransferDataStatus_tag
{
    g_NoSuchTunnel_c            = 0x00,
    g_WrongDevice_c             = 0x01
}TransferDataStatus_t;

typedef enum ProtocolID_tag
{
    g_DLMS_COSEM_IEC62056_c  = 0x00,
    g_IEC61107_c             = 0x01,
    g_ANSIC12_c              = 0x02,
    g_MBUS_c                 = 0x03,
    g_SML_c                  = 0x04,
    g_CLIMATETALK_c          = 0x05
}ProtocolID_t;


/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/

uint8_t ZCL_HandleTunnelingClusterCmd ( uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );


#endif                                 /*g_TUNNELING_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* End Of File
*------------------------------------------------------------------------------*/

