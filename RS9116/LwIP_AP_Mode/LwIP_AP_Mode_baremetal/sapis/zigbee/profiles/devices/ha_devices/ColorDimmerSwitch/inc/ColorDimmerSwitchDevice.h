/******************************************************************************
* ColorDimmerSwitchDevice.h
******************************************************************************
* Program Description:
* This file contains the Color DimmerSwitch Device
******************************************************************************/


#ifndef _COLOR_DIMMER_SWITCH_H_
#define _COLOR_DIMMER_SWITCH_H_


/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

typedef struct App_Data_Request_Tag
{
   uint16_t short_addr;
   uint8_t dst_endpoint;
   uint8_t src_endpoint;
   uint16_t cluster_id;
   uint8_t asdu_length;
   uint8_t tx_options;
   uint8_t radius;
   uint8_t a_asdu[1];
}App_Data_Request_t;


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
/*General domain*/
extern uint8_t g_Move_Command_Endpoint;
extern uint8_t g_Move_Command_Mode;
extern uint8_t g_Move_Command_Rate;

/*lighting domain*/


extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Stack_Test_Main
*------------------------------------------------------------------------------
*
* Return Value :
*     None
*
* Parameters   :
*
* Input Parameters:
*     None
*
* Output Parameters:
*     None
*
* Scope of the function:
*
* Purpose of the function:
*
*
* Before the function is called:
*     NONE
*
* After the function is called:
*     NONE
*
*----------------------------------------------------------------------------*/
void Stack_Test_Main
(
   void
);

/*-----------------------------------------------------------------------------
* Test_Application_To_ZDO
*------------------------------------------------------------------------------
*
* Return Value :
*			void - No return value	
* Input Parameters:
*	msg_type - gives the type of the message/management id of the message to be
*					queued
*	msg_index - gives the actual management data sent from the test tool that
*		needs to be processed.
*		
* Output Parameters:
*		None
* Scope of the function:
*   	
*
* Purpose of the function:
*
*	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/	
void Test_Application_To_ZDO
(
   uint8_t msg_type,
   uint8_t msg_index
);

/*-----------------------------------------------------------------------------
* Update_Reportable_Attribute
*------------------------------------------------------------------------------
*
* Return Value :
*			None	
* Input Parameters:
*	attrib_id - The attribute id which has to be updated.
*	cluster - The cluster id whose attribute has to be modified.
*	
* Output Parameters:
*	attrib_value - The value of attribute that has to be written.
*
* Scope of the function:
*    Exposed	
*
* Purpose of the function:
*    This function writes the value of reportable attributes and takes care
* of attribute reporting.
*	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/	
void Update_Reportable_Attribute
(
   uint8_t *attrib_value,
   uint16_t attrib_id,
   uint16_t cluster
);

/*-----------------------------------------------------------------------------
* Read_Write_Level_Control_Attributes
*------------------------------------------------------------------------------
*
* Return Value :
*			None	
* Input Parameters:
*	read_or_write - This parameter indicates whether attribute has to be read or
*  write.
*	attrib_value - The value of attribute that has to be written.
*	attrib_id - The cluster id whose attribute has to be modified.
*	
* Output Parameters:
*	   attrib_value - The attribute value that is read.
*
* Scope of the function:
*    Exposed	
*
* Purpose of the function:
*    This function reads or writes the level control attributes
*	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/	
void Read_Write_Level_Control_Attributes
(
   uint8_t read_or_write,
   uint8_t *attrib_value,
   uint8_t attrib_id
);

/*-----------------------------------------------------------------------------
* Write_Current_Level
*------------------------------------------------------------------------------
*
* Return Value :
*			None	
* Input Parameters:
*	level - The value with which current level attribute of level control
*  cluster has to be updated.
*	
* Output Parameters:
*		None
* Scope of the function:
*    Exposed	
*
* Purpose of the function:
*    This function writes the value of current level attribute and updates
*     on off attribute if required.
*	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/	
void Write_Current_Level
(
   uint8_t level
);

#endif /* _COLOR_DIMMER_SWITCH_H_*/


void Test_Harness_To_Stack_Info
(
   uint8_t msg_type,
   uint8_t msg_index
);


void App_Handle_Data_Request
(
   uint8_t msgId,
   uint8_t msg_index
);


/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
