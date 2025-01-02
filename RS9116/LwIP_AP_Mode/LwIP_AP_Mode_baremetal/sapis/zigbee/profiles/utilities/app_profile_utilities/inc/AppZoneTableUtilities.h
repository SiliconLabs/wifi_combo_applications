#ifndef _APPZONETABLEUTILITIES_H_
#define _APPZONETABLEUTILITIES_H_
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Interface.h"
#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
    #include "ZCL_Security_And_Safety_IAS_Zone_Cluster.h"
#endif

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_MAXIMUM_ZONE_TABLE_ENTRIES_c                         0x03


/*global array to copy the data indication received. This is a back up
to send the enroll response command for a recieved enroll request command*/



typedef struct Zone_Enroll_Response_Payload_Tag
{
	uint8_t dest_endpoint;
	uint8_t src_endpoint;
	uint8_t trans_seq_num;
	uint16_t zone_type;
}__attribute__((__packed__))Zone_Enroll_Response_Payload_t;

/*-----------------------------------------------------------------------------
* App_Add_Zone_Address
*-----------------------------------------------------------------------------*/
uint8_t App_Add_Zone_Address ( uint8_t *p_IEEE_Address );

/*-----------------------------------------------------------------------------
* App_Reset_Zone_Table
*-----------------------------------------------------------------------------*/
void App_Reset_Zone_Table ( void );


/*-----------------------------------------------------------------------------
* App_Create_Enroll_Response_Command_Payload
*------------------------------------------------------------------------------
*
* Return Value :
*		uint8_t
* Input Parameters:
*	None
* Output Parameters:
*  p_dest_addr - destination address to whom the enroll response needs to be
*                     sent
*  p_payload   - payload for enroll response command
*  p_src_endpoint - src endpoint
*  p_dest_endpoint
*
* Scope of the function:
*  This is called within the module
*
* Purpose of the function:
*	This function creates the enroll response command payload
*     	
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/
void App_Create_Enroll_Response_Command_Payload  (   uint8_t *p_dest_addr,
												   uint8_t *p_payload,
												   uint8_t *p_src_endpoint,
												   uint8_t *p_dest_endpoint  );



#endif
