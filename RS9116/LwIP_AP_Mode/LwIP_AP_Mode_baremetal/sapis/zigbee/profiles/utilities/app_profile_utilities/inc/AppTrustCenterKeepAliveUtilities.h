/******************************************************************************
* AppTrustCenterKeepAliveUtilities.h
******************************************************************************
* Program Description:
* This file contains the Trust Center SwapOut Feature App type definations
******************************************************************************/
#ifndef _APP_TRUST_CENTER_KEEP_ALIVE_UTILITIES_H_
#define _APP_TRUST_CENTER_KEEP_ALIVE_UTILITIES_H_

/*******************************************************************************
* Includes
********************************************************************************/
#include "ZCL_Functionality.h"
#include "stack_common.h"
/*******************************************************************************
* Global Constants
********************************************************************************/
#define g_TRUST_CENTER_c 0x00
#define g_TRUST_CENTER_KEEPALIVE_FAILURE_LIMIT_c 		  3
#define g_ONE_MINUTES_IN_SECONDS_c                       60

#define g_KEEP_ALIVE_SEND_SIGNAL_RATE_IN_MINUTES_c       20

/*******************************************************************************
* Memory Declarations
********************************************************************************/
/*******************************************************************************
* Type Definitions
********************************************************************************/

typedef enum {
  KEEP_ALIVE_INITIAL_STATE,
  KEEP_ALIVE_SIGNAL_SEND_STATE,
  INITIATE_TRUST_CENTER_SEARCH_STATE,
} TrustCenterKeepaliveState;

/*******************************************************************************/
/*   Function Prototypes                                                       */
/*******************************************************************************/
#if (g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1) &&  ( g_ZIGBEE_ROUTER_d == 1 )
void App_changeRegistrationStatus( uint8_t state );
void App_trustCenterKeepaliveStop(void);
void App_trustCenterKeepaliveUpdate(void);
void App_trustCenterKeepaliveTask(void);

void App_keepAliveCmdRsp(uint16_t ShortAddress );

#endif      /*  g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1 */


/*******************************************************************************
* End Of File
********************************************************************************/
#endif          /* _APP_TRUST_CENTER_KEEP_ALIVE_UTILITIES_H_ */