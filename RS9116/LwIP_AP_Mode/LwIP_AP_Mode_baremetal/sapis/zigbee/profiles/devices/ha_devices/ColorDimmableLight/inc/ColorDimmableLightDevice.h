/******************************************************************************
* ColorDimmableLightDevice.h
******************************************************************************
* Program Description:
* This file contains the Color DimmableLight Device
******************************************************************************/


#ifndef _COLOR_DIMMABLE_LIGHT_DEVICE_H_
#define _COLOR_DIMMABLE_LIGHT_DEVICE_H_


/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Interface.h"
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


extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Macros
*----------------------------------------------------------------------------*/
//#define m_CHANGE_UP_BY_ONE_c                             0x01
#define m_IGNORE_DATA_c                                  0xFF
#define m_RESET_c                                        0x00
#define m_INVALID_INDEX_c	                         0xFF

#define m_CURRENT_LEVEL_ATTRIBUTE_c                      0x0000
#define m_MAX_VALUE_c                                    0xFF
#define m_MIN_VALUE_c                                    0x00
#define m_CHANGE_DOWN_BY_ONE_c                           -1
#define m_TRANSITION_RATE_c                              0x0A
#define m_MAXIMUM_VALUE_c                                0xFFFF
#define m_REMAINING_TIME_ATTRIB_ID_c                     0x0001
#define m_NO_BUFFER_c				         0xFF



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

#endif /* _COLOR_DIMMABLE_LIGHT_DEVICE_H_*/


/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
