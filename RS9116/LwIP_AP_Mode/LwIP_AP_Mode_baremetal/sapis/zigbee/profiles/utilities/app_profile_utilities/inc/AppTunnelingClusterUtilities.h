/******************************************************************************
* AppTunnelingClusterUtilities.h
******************************************************************************
* Program Description:
* This file contains the Tunneling cluster App type definations
******************************************************************************/
#ifndef _APP_TUNNELING_CLUSTER_UTILITIES_H_
#define _APP_TUNNELING_CLUSTER_UTILITIES_H_

/*******************************************************************************
* Includes
********************************************************************************/
#include "ZigBeeDataTypes.h"
//#include "ZCL_Functionality.h"

/*******************************************************************************
* Global Constants
********************************************************************************/
#define g_MAX_TUNNELING_SERVER_TUNNEL_LIMIT_c                   0x02
#define g_MAX_TUNNELING_CLIENT_TUNNEL_LIMIT_c                   0x02
#define g_TUNNELING_CLUSTER_INVALID_TUNNEL_ID_c                 0xFFFF

#if ( g_SE_PROFILE_1_2_d == 1 )
#define g_MAX_TUNNELING_SUPPORTED_PROTOCOLS_c					0x06
#endif  /* g_SE_PROFILE_1_2_d */

/*******************************************************************************
* Memory Declarations
********************************************************************************/

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern uint8_t protocolId;
extern uint16_t manufacturerCode;
extern bool flowControl;
#endif

/*******************************************************************************
* Type Definitions
********************************************************************************/
typedef struct TunnelingServerTunnel_tag
{
  bool inUse;
  uint16_t client;
  uint8_t clientEndpoint;
  uint8_t serverEndpoint;
  uint16_t tunnelId;
  uint8_t  protocolId;
  uint16_t manufacturerCode;
  bool flowControlSupport;
  uint32_t lastActive;
  uint16_t numberOfBytesLeft;
} __attribute__((__packed__))TunnelingServerTunnel_t;


typedef struct TunnelingClientTunnel_tag
{
  bool inUse;
  uint16_t server;
  uint8_t clientEndpoint;
  uint8_t serverEndpoint;
  uint16_t tunnelId;
  uint8_t  protocolId;
  uint16_t manufacturerCode;
  bool flowControlSupport;
  uint16_t numberOfBytesLeft;
} __attribute__((__packed__))TunnelingClientTunnel_t;

/**************************************************************************************/
/*   Function Prototypes                                                              */
/**************************************************************************************/
#if( g_TUNNELING_CLUSTER_ENABLE_d == 1 )
void App_TunnelTableIntit( void );
void App_tunnelEventsHandler( ZCL_Event_t*pZCLEvent,APSDE_Data_Indication_t *pBuffer );
bool App_tunnelingServerIsProtocolSupported(uint8_t protocolId, uint16_t manufacturerCode);
#endif                  /* ( g_TUNNELING_CLUSTER_ENABLE_d == 1 ) */


#if ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 )
uint8_t App_tunnelingServerFindTunnel( uint16_t tunnelId,
       uint16_t nwkAddress,uint8_t clientEndpoint, TunnelingServerTunnel_t** tunnel );
bool App_tunnelingServerIsFlowControlSupportedCallBack( void );
bool App_tunnelingServerIsProtocolSupportedCallBack( uint8_t protocolId,
                                             uint16_t manufacturerCode );
void App_ServerTunnelTask( void );
#endif          /* ( g_TUNNELING_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 )
uint8_t App_tunnelingClientFindTunnel( uint16_t tunnelId,
       uint16_t nwkAddress,uint8_t serverEndpoint, TunnelingClientTunnel_t** tunnel );
#endif          /* ( g_TUNNELING_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/**************************************************************************************
* End Of File
***************************************************************************************/
#endif          /* _APP_TUNNELING_CLUSTER_UTILITIES_H_ */
