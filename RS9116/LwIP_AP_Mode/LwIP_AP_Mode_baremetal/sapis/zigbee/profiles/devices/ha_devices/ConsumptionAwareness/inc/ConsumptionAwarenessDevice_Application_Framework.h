/******************************************************************************
* ConsumptionAwarenessDevice_Application_Framework.h
******************************************************************************
* Program Description:
* This file contains the Consumption awareness Device application framework
******************************************************************************/

#ifndef _APPLICATION_FRAMEWORK_H_
#define _APPLICATION_FRAMEWORK_H_

/******************************************************************************
* Includes
******************************************************************************/
/* None */

/******************************************************************************
* Global Constants
******************************************************************************/
#ifndef NULL
#define NULL (void *)(0)
#endif


#define g_HOME_AUTOMATION_PROFILE_ID_c                  0x0104
#define g_CONSUMPTION_AWARENESS_DEVICE_c                0x000D
#define g_CONSUMPTION_AWARENESS_DEVICE_ENDPOINT_c       0x01

/******************************************************************************
* Function Prototypes
******************************************************************************/

/******************************************************************************
* Handle_Data_Message
*
* Return Value :
*			void - No retrun value	
* Input Parameters:
*		The pointer of type APSDE_Response_t. This structure in turn
		gives if the data is confirmation or indication using msgtype
* Output Parameters:
*		None
* Scope of the function:
*   	Whenever a data confirmation or indication is received at the NLLE(APS)
*		this call back function is called to process the same.
*
* Purpose of the function:
*     This function checks if the data received is confirmation or indication
*	  using msgtype member of the structure APSDE_Response_t and calls respective
*	  functions to handle the data confirmation/indication received from the
*	  NLLE
*	  The Application should free the memory allocated for data confirmation
*		indication
*	
* Before the function is called:
*     None
* After the function is called:
*     None
*
******************************************************************************/	
void Handle_ZCL_Data_Message
(
   uint8_t buffer_index
);

/******************************************************************************
* Handle_Broadcast_EP
******************************************************************************
*
* Return Value :
*			void - No retrun value	
* Input Parameters:
*
* Output Parameters:
*		None
* Scope of the function:
*
* Purpose of the function:
*	
* Before the function is called:
*     None
* After the function is called:
*     None
*
******************************************************************************/
void Handle_Broadcast_EP
(
	uint8_t buffer_index
);

#endif
/******************************************************************************
* End Of File
******************************************************************************/
