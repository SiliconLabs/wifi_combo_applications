/*******************************************************************************
* ZCL_TOU_Calender_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting DRLC cluster
*
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#if ( g_DEVICE_MANAGEMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Foundation.h"
#include "ZCL_Device_Management_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/


/*******************************************************************************
* Public Memory declarations
*******************************************************************************/
#if ( g_DEVICE_MANAGEMENT_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_DRLC_Client_Cluster_Attribute_Info [] =
{

/* Supply Control Attribute Set */
#if ( g_PROPOSED_CHANGE_SUPPLY_IMPLEMENTATION_TIME_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROPOSED_CHANGE_SUPPLY_IMPLEMENTATION_TIME_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROPOSED_CHANGE_SUPPLY_IMPLEMENTATION_TIME_ATTRIBUTE_ENABLE_d*/	

#if ( g_PROPOSED_CHANGE_SUPPLY_STATE_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROPOSED_CHANGE_SUPPLY_STATE_TIME_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROPOSED_CHANGE_SUPPLY_STATEE_ATTRIBUTE_ENABLE_d*/	

/* Supplier Control Attribute Set*/

#if ( g_PROVIDER_ID_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROVIDER_ID_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROVIDER_ID_ATTRIBUTE_ENABLE_d*/

#if ( g_PROVIDER_NAME_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROVIDER_NAME_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROVIDER_NAME_ATTRIBUTE_ENABLE_d*/

#if ( g_PROPOSED_PROVIDER_ID_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROPOSED_PROVIDER_ID_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROPOSED_PROVIDER_ID_ATTRIBUTE_ENABLE_d*/	

#if ( g_PROPOSED_PROVIDER_NAME_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROPOSED_PROVIDER_NAME_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROPOSED_PROVIDER_NAME_ATTRIBUTE_ENABLE_d*/	


#if ( g_PROPOSED_PROVIDER_CHANGE_DATE_TIME_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROPOSED_PROVIDER_CHANGE_DATE_TIME_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROPOSED_PROVIDER_CHANGE_DATE_TIME_ATTRIBUTE_ENABLE_d*/	

#if ( g_PROPOSED_PROVIDER_CHANGE_CONTROL_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROPOSED_PROVIDER_CHANGE_CONTROL_ATTRIBUTE_c,
      g_Thirty_Two_Bit_Bitmap_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROPOSED_PROVIDER_CHANGE_CONTROL_ATTRIBUTE_ENABLE_d*/	

#if ( g_EX_PROVIDER_ID_ATTRIBUTE_ENABLE_d == 1 )
    { g_EX_PROVIDER_ID_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_EX_ROVIDER_ID_ATTRIBUTE_ENABLE_d*/	

#if ( g_EX_PROVIDER_NAME_ATTRIBUTE_ENABLE_d == 1 )
    { g_EX_PROVIDER_NAME_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_EX_PROVIDER_NAME_ATTRIBUTE_ENABLE_d*/	

#if ( g_EX_PROPOSED_ROVIDER_ID_ATTRIBUTE_ENABLE_d == 1 )
    { g_EX_PROPOSED_PROVIDER_ID_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_EX_PROPOSED_ROVIDER_ID_ATTRIBUTE_ENABLE_d*/	

#if ( g_EX_PROPOSED_PROVIDER_NAME_ATTRIBUTE_ENABLE_d == 1 )
    { g_EX_PROPOSED_PROVIDER_NAME_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_EX_PROPOSED_PROVIDER_NAME_ATTRIBUTE_ENABLE_d*/	

#if ( g_EX_PROPOSED_PROVIDER_CHANGE_DATE_TIME_ATTRIBUTE_ENABLE_d == 1 )
    { g_EX_PROPOSED_PROVIDER_CHANGE_DATE_TIME_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_EX_PROPOSED_PROVIDER_CHANGE_DATE_TIME_ATTRIBUTE_ENABLE_d*/	

#if ( g_EX_PROPOSED_PROVIDER_CHANGE_CONTROL_ATTRIBUTE_ENABLE_d == 1 )
    { g_EX_PROPOSED_PROVIDER_CHANGE_CONTROL_ATTRIBUTE_c,
      g_Thirty_Two_Bit_Bitmap_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_EX_PROPOSED_PROVIDER_CHANGE_CONTROL_ATTRIBUTE_ENABLE_d*/	

/* Tenancy control Attribute Set*/

#if ( g_CHANGE_OF_TENANCY_UPDATE_DATE_TIME_ATTRIBUTE_ENABLE_d == 1 )
    { g_CHANGE_OF_TENANCY_UPDATE_DATE_TIME_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_CHANGE_OF_TENANCY_UPDATE_DATE_TIME_ATTRIBUTE_ENABLE_d*/	

#if ( g_PROPOSED_TENANCY_CHANGE_CONTROL_ATTRIBUTE_ENABLE_d == 1 )
    { g_PROPOSED_TENANCY_CHANGE_CONTROL_TIME_ATTRIBUTE_c,
      g_Thirty_Two_Bit_Bitmap_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_PROPOSED_TENANCY_CHANGE_CONTROL_ATTRIBUTE_ENABLE_d*/	

/* 	Backhaul Control Attribute Set  */
	
#if ( g_WAN_STATUS_ATTRIBUTE_ENABLE_d == 1 )
    { g_WAN_STATUS_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_WAN_STATUS_ATTRIBUTE_ENABLE_d*/	

/* HAN Control Attribute Set*/

#if ( g_LOW_MEDIUM_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    { g_LOW_MEDIUM_THRESHOLD_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_WAN_STATUS_ATTRIBUTE_ENABLE_d*/	


#if ( g_HIGH_MEDIUM_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
    { g_HIGH_MEDIUM_THRESHOLD_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif         /* g_WAN_STATUS_ATTRIBUTE_ENABLE_d*/	

};
#endif  /* g_DEVICE_MANAGEMENT_SERVER_CLUSTER_ENABLE_d == 1 */
/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/* None */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* None */

/*******************************************************************************
* Public Functions
*******************************************************************************/
uint8_t ZCL_Handle_Device_Mamagement_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
	return g_ZERO_DATA_LENGTH_c;


}

/*******************************************************************************/



/*******************************************************************************
* Private Functions
*******************************************************************************/

/* None */

#endif                                /* g_DEVICE_MANAGEMENT_CLUSTER_ENABLE_d */
/*******************************************************************************
* End Of File
*******************************************************************************/

