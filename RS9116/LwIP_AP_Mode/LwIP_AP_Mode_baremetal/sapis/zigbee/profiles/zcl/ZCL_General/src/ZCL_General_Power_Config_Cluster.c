/*******************************************************************************
* ZCL_General_Power_Config_Cluster.c.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Power Configuration
* functinality
*
*******************************************************************************/

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Power_Config_Cluster.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )

const ZCL_Attribute_Info_t ma_Power_Configuration_Server_Attribute[] =
{
#if ( g_MAINS_VOLTAGE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MAINS_VOLTAGE_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c,
   },
#endif                          /* g_MAINS_VOLTAGE_ATTRIBUTE_ENABLE_d */

#if ( g_MAINS_FREQUENCY_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MAINS_FREQUENCY_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c,

   },
#endif                          /* g_MAINS_FREQUENCY_ATTRIBUTE_ENABLE_d */

#if ( g_MAINS_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MAINS_ALARM_MASK_ATTRIBUTE_c,
      g_Eight_Bit_Bitmap_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif                          /* g_MAINS_ALARM_MASK_ATTRIBUTE_ENABLE_d */

#if ( g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c
   },
#endif                          /* g_MAINS_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d */

#if ( g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c
   },
#endif                          /* g_MAINS_VOLATGE_MAX_THRESHOLD_ATTRIBUTE_ENABLE_d */

#if ( g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c
   },
#endif                          /* g_MAINS_VOLATGE_DWELL_TRIP_POINT_ATTRIBUTE_ENABLE_d */

#if ( g_BATTERY_VOLATGE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_BATTERY_VOLATGE_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
   },
#endif                          /* g_BATTERY_VOLATGE_ATTRIBUTE_ENABLE_d */

#if ( g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_BATTERY_MANUFACTURER_ATTRIBUTE_c,
      g_Character_String_Data_c,
      g_Read_Write_Attribute_c
   },
#endif                          /* g_BATTERY_MANUFACTURER_ATTRIBUTE_ENABLE_d */

#if ( g_BATTERY_SIZE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_BATTERY_SIZE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif                          /* g_BATTERY_SIZE_ATTRIBUTE_ENABLE_d */

#if ( g_BATTERY_AHR_RATING_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_BATTERY_AHR_RATING_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif                          /* g_BATTERY_AHR_RATING_ATTRIBUTE_ENABLE_d */

#if ( g_BATTERY_QUANTITY_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_BATTERY_QUANTITY_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif                          /* g_BATTERY_QUANTITY_ATTRIBUTE_ENABLE_d */

#if ( g_BATTERY_RATED_VOLATGE_ATTRIBUTE_ENABLE_d == 1 )
    {
      g_BATTERY_RATED_VOLATGE_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif                          /* g_BATTERY_RATED_VOLATGE_ATTRIBUTE_ENABLE_d */

#if ( g_BATTERY_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_BATTERY_ALARM_MASK_ATTRIBUTE_c,
      g_Eight_Bit_Bitmap_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif                          /* g_BATTERY_ALARM_MASK_ATTRIBUTE_ENABLE_d */

#if ( g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c,
   },
#endif                          /* g_BATTERY_VOLATGE_MIN_THRESHOLD_ATTRIBUTE_ENABLE_d */


};

/*Cluster Info for Power Configuration Cluster*/
const ZCL_Cluster_Info_t ga_Power_Configuration_Server_Cluster_Info =
{
    g_POWER_CONFIGURATION_CLUSTER_c,
    sizeof(ma_Power_Configuration_Server_Attribute)/sizeof(ZCL_Attribute_Info_t),
    ma_Power_Configuration_Server_Attribute
};
#endif              /* ( g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 ) */

/*Cluster Info for Power Configuration Cluster*/
#if ( g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 )

const ZCL_Cluster_Info_t ga_Power_Configuration_Client_Cluster_Info =
{
  g_POWER_CONFIGURATION_CLUSTER_c,
  g_NULL_c,
  g_NULL_c
};

#endif              /* ( g_POWER_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_PowerConfig_Cluster_Cmd (  uint8_t endPointId,
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

#endif                              /* g_POWER_CONFIGURATION_CLUSTER_ENABLE_d */
