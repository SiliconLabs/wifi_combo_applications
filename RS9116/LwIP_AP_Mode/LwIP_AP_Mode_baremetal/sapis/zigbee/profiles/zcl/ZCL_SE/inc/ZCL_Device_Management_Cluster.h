/******************************************************************************
* ZCL_Device_Management_Cluster.h
******************************************************************************
* Program Description:
* This file contains the DRLC cluster attribute and cmmnd information
******************************************************************************/

#ifndef _ZCL_DEVICE_MANAGEMENT_CLUSTER_H_
#define _ZCL_DEVICE_MANAGEMENT_CLUSTER_H_

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



/*-----------------------------------------------------------------------------
* ZCL_Handle_TOU_Calender_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : Length
*		
* Input Parameters:
*	endPointId - Device End point
*   asduLength - Length of received asdu
*   pReceivedAsdu - Pointer to received asdu
*   pZCLClusterInfo - Pointer to Level control cluster info
*   pAttrsDataBaseLocation - Pointer to Basic cluster attributes storage
*
* Output Parameters:
*	pResponseLocation - Pointer for response creation
*   pActionHandler - Pointer for event updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the TOU Calender cluster command received in the indication
*     	
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Device_Mamagement_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*----------------------------------------------------------------------------*/

#endif         /* g_DEVICE_MANAGEMENT_CLUSTER_ENABLE_d*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                      /*_ZCL_DEVICE_MANAGEMENT_CLUSTER_H_*/