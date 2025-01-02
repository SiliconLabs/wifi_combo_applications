/*******************************************************************************
* ZCL_General_Device_Temp_Config_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Device Temp cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Device_Temp_Config_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/* None */
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Device_Temp_Config_Server_Attribute[] =
{
   {
      g_CURRENT_TEMPERATURE_ATTRIBUTE_c,
      g_Signed_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
     }
    ,
#if ( g_MIN_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MIN_TEMP_EXPERIENCED_ATTRIBUTE_c,
      g_Signed_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
   },
#endif                          /* g_MIN_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d */
#if ( g_MAX_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MAX_TEMP_EXPERIENCED_ATTRIBUTE_c,
      g_Signed_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
   },
#endif                          /* g_MAX_TEMP_EXPERIENCED_ATTRIBUTE_ENABLE_d */
#if ( g_OVER_TO_TOTAL_DWELL_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_OVER_TO_TOTAL_DWELL_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
   },
#endif                          /* g_OVER_TO_TOTAL_DWELL_ATTRIBUTE_ENABLE_d */
#if ( g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_c,
      g_Eight_Bit_Bitmap_Data_c,
      g_Read_Write_Attribute_c
   },
#endif                          /* g_DEVICE_TEMP_ALARM_MASK_ATTRIBUTE_ENABLE_d */
#if ( g_LOW_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_LOW_TEMP_THRESHOLD_ATTRIBUTE_c,
      g_Signed_Sixteen_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c
   },
#endif                          /* g_LOW_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d */

#if ( g_HIGH_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_HIGH_TEMP_THRESHOLD_ATTRIBUTE_c,
      g_Signed_Sixteen_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c
   },
#endif                          /* g_HIGH_TEMP_THRESHOLD_ATTRIBUTE_ENABLE_d */

#if ( g_LOW_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_LOW_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_c,
      g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c
   },
#endif                      /* g_LOW_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d */

#if ( g_HIGH_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_HIGH_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_c,
      g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c
   }
#endif                      /* g_HIGH_TEMP_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d */
};

/*Cluster Info for Device Temp Config Cluster*/
const ZCL_Cluster_Info_t ga_Device_Temp_Config_Server_Cluster_Info =
{
   g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
   sizeof(ma_Device_Temp_Config_Server_Attribute)/sizeof(ZCL_Attribute_Info_t) ,
   ma_Device_Temp_Config_Server_Attribute
};

#endif          /* ( g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */

/*Cluster Info for Device Temp Config Cluster*/
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Device_Temp_Config_Client_Cluster_Info =
{
   g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_TempConfig_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
    /* It doesn't have any cluster specific commands. Hence it returns zero data
    length to the calling function. On receipt of zero data length, the calling
    function will create Default response command with status
    Unsupported cluster command */
    pActionHandler->action = Send_Response;

    return g_ZERO_DATA_LENGTH_c;
}

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif              /* g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d */
