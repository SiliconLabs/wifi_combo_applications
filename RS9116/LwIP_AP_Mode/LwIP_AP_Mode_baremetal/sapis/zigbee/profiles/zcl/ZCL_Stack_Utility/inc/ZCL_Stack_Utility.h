#ifndef _ZCL_STACK_UTILITY_H_
#define _ZCL_STACK_UTILITY_H_

/*-----------------------------------------------------------------------------

*----------------------------------------------------------------------------*/
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "apsde_interface.h"
#include "ZCL_Functionality.h"
#include "stack_common.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

extern uint8_t a_IEEE_Address_Found[8];
extern uint8_t g_Ieee_Address_Request_For_Zone_cluster;

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if ( g_GROUPS_CLUSTER_ENABLE_d == 1 )

uint8_t ZCL_Stack_Add_Group_Request ( uint8_t *p_group_data, uint8_t end_point );

uint8_t ZCL_Stack_Remove_Group_Request ( uint8_t *p_group_data, uint8_t end_point );

uint8_t ZCL_Stack_Remove_All_Groups_Request ( uint8_t end_point );

uint8_t ZCL_Stack_View_Group_Request ( uint8_t *p_group_data );

uint8_t ZCL_Group_Table_Max_Value(void);
uint16_t ZCL_Stack_Group_Id(uint8_t index);
uint8_t ZCL_Verify_GroupAddrAndEndPoint(uint16_t groupId,uint8_t dstEndPoint);


#endif /* ( g_GROUPS_CLUSTER_ENABLE_d == 1 ) */

#if ( g_IAS_ZONE_CLUSTER_ENABLE_d == 1 )
uint8_t Create_IEEE_Address_Request ( uint16_t dest_addr );
void Handle_IEEE_Address_Response( APSDE_Data_Indication_t *p_buffer );

#endif                                 /*g_IAS_ZONE_CLUSTER_ENABLE_d*/

#endif 									/* _ZCL_STACK_UTILITY_H_ */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
