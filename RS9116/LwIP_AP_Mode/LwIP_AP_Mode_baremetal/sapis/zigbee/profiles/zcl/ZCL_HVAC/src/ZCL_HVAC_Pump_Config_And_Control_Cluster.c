/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"

#include "ZCL_Functionality.h"
#include "common_includes.h"
#include "ZCL_Foundation.h"
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Pump_Config_And_Control_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*None*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/* Following are attribute info of Pump Configuration and Control Cluster */
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Pump_Config_And_Server_Control_Cluster_Attribute_info[] =
{
    {  g_MAX_PRESSURE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c },

    {  g_MAX_SPEED_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c },

    {  g_MAX_FLOW_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c },

#if ( g_MAX_CONST_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MIN_CONST_PRESSURE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c },
#endif                              /* ( g_MAX_CONST_PRESSURE_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MIN_CONST_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MAX_CONST_PRESSURE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c },
#endif                              /* ( g_MIN_CONST_PRESSURE_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MIN_COMP_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MIN_COMP_PRESSURE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c },
#endif                              /* ( g_MIN_COMP_PRESSURE_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MAX_COMP_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MAX_COMP_PRESSURE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  },
#endif                              /* ( g_MAX_COMP_PRESSURE_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MIN_CONST_SPEED_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MIN_CONST_SPEED_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  },
#endif                              /* ( g_MIN_CONST_SPEED_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MAX_CONST_SPEED_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MAX_CONST_SPEED_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  },
#endif                              /* ( g_MAX_CONST_SPEED_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MIN_CONST_FLOW_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MIN_CONST_FLOW_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  },
#endif                              /* ( g_MIN_CONST_FLOW_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MAX_CONST_FLOW_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MAX_CONST_FLOW_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  },
#endif                              /* ( g_MAX_CONST_FLOW_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MIN_CONST_TEMP_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MIN_CONST_TEMP_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  },
#endif                              /* ( g_MIN_CONST_TEMP_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_MAX_CONST_TEMP_ATTRIBUTE_ENABLE_d == 1 )
    {   g_MAX_CONST_TEMP_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  },
#endif                              /* ( g_MAX_CONST_TEMP_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_PUMP_STATUS_ATTRIBUTE_ENABLE_d == 1 )
    {   g_PUMP_STATUS_c,
        g_Sixteen_Bit_Bitmap_Data_c,
        g_Read_Only_Attribute_c | g_Reportable_Attribute_c },
#endif                              /* ( g_PUMP_STATUS_ATTRIBUTE_ENABLE_d == 1 ) */

    {  g_EFFECTIVE_OPERATION_MODE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Only_Attribute_c },

    {   g_EFFECTIVE_CONTROL_MODE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Only_Attribute_c },

    {   g_CAPACITY_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c | g_Reportable_Attribute_c },

#if ( g_PUMP_STATUS_ATTRIBUTE_ENABLE_d == 1 )
    {   g_SPEED_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c },
#endif                              /* ( g_PUMP_STATUS_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_LIFETIME_RUNNING_HOURS_ATTRIBUTE_ENABLE_d == 1 )
    {   g_LIFETIME_RUNNING_HOURS_c,
        g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },
#endif                              /* ( g_LIFETIME_RUNNING_HOURS_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_POWER_ATTRIBUTE_ENABLE_d == 1 )
    {   g_POWER_c,
        g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c  },
#endif                              /* ( g_POWER_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_LIFETIME_ENERGY_CONSUMED_ATTRIBUTE_ENABLE_d == 1 )
    {   g_LIFETIME_ENERGY_CONSUMED_c,
        g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c },
#endif                              /* ( g_LIFETIME_ENERGY_CONSUMED_ATTRIBUTE_ENABLE_d == 1 ) */

    {   g_OPERATION_MODE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Write_Attribute_c },

#if ( g_CONTROL_MODE_ATTRIBUTE_ENABLE_d == 1 )
    {   g_CONTROL_MODE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Write_Attribute_c  },
#endif                                  /* ( g_CONTROL_MODE_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    {   g_ALARM_MASK_c,
        g_Sixteen_Bit_Bitmap_Data_c,
        g_Read_Only_Attribute_c  }
#endif                                  /* ( g_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 ) */
};

/* ZCL Cluster information for server side */
const ZCL_Cluster_Info_t ga_HVAC_Pump_Config_And_Control_Server_Cluster_Info =
{
   g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_c,
   sizeof ( ma_Pump_Config_And_Server_Control_Cluster_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Pump_Config_And_Server_Control_Cluster_Attribute_info
};
#endif          /* ( g_PUMP_CONFIGURATION_AND_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 ) */


/* ZCL Cluster information for client side */
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_HVAC_Pump_Config_And_Control_Client_Cluster_Info =
{
   g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* ( g_PUMP_CONFIGURATION_AND_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_HVAC_PumpConfig_Cluster_Cmd (  uint8_t endPointId,
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

#endif /* ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 ) */
