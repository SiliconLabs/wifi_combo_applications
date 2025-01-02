


/******************************************************************************
* AppPriceClusterUtilities..h
******************************************************************************
* Program Description:
* This file contains the Price cluster App type definations
******************************************************************************/
#ifndef _APP_GENERIC_TUNNEL_CLUSTER_UTILITIES_H_
#define _APP_GENERIC_TUNNEL_CLUSTER_UTILITIES_H_


/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/
/*******************************************************************************
* App_GenericTunnelEventsHandler
********************************************************************************
*   Return Value: NONE
*
*   Parameters:
*   Input parameter:
*   pZCLEvent ->
*   pBuffer ->
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
void App_GenericTunnelEventsHandler( ZCL_Event_t *pZCLEvent,
                           				  APSDE_Data_Indication_t *pBuffer );
/*******************************************************************************
* App_GenericTunnelMatchProtocolAddressHandler
********************************************************************************
*   Return Value: status
*
*   Parameters:
*   Input parameter:
*
*
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )

void App_GenericTunnelMatchProtocolAddressHandler( uint8_t *ProtocolAddress,
											 APSDE_Data_Indication_t *pBuffer);
#endif
/*******************************************************************************
* App_GenericTunnelGetProtocolAddress
********************************************************************************
*   Return Value: status
*
*   Parameters:
*   Input parameter:
*
*
*
*   Output parameter:
*   None
*
*   Scope of the function:
*   This is a global function.
*
*   Purpose of the function:
*
*
*   Before the function is called:
*   None
*
*   After the function is called:
*   None
*
*******************************************************************************/
uint8_t* App_GenericTunnelGetProtocolAddress(uint8_t endpoint);

/*******************************************************************************
* App_GenericTunnelInit
********************************************************************************
*   Return Value: None
*
*   Parameters:
*   Input parameter:
*		None
*
*   Output parameter:
*   	None
*
*   Scope of the function:
*   	This is a global function.
*
*   Purpose of the function:
*   	This function has been implemented to initialize the generic tunnel
*		attribute	
*
*   Before the function is called:
*   	None
*
*   After the function is called:
*   	None
*
*******************************************************************************/
uint8_t App_GenericTunnelInit( void );

/*******************************************************************************
* App_SendChangedProtocolAddress
********************************************************************************
*   Return Value: None
*
*   Parameters:
*   Input parameter:
*		None
*
*   Output parameter:
*   	None
*
*   Scope of the function:
*   	This is a global function.
*
*   Purpose of the function:
*   	This function has been implemented to initialize the generic tunnel
*		attribute	
*
*   Before the function is called:
*   	None
*
*   After the function is called:
*   	None
*
*******************************************************************************/
#if ( g_GENERIC_TUNNEL_SERVER_CLUSTER_ENABLE_d == 1 )

void App_SendChangedProtocolAddress( uint8_t endpoint,uint16_t clusterId);
#endif

/*******************************************************************************/

#endif          /* _APP_GENERIC_TUNNEL_CLUSTER_UTILITIES_H_ */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

