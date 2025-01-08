/******************************************************************************
* ZCL_M_And_S_Illuminance_Measurement_Cluster.c
******************************************************************************
* Program Description:
* This file contains the Illuminanace_Measuremnt_Cluster attribute and cmmnd
* information
******************************************************************************/

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
//#include "ZCL_Common.h"
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Illuminance_Measurement_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* None */


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of g_ILLUMINANCE_MEASUREMENT_CLUSTER_c */
const ZCL_Attribute_Info_t ma_Illuminance_Measurement_Cluster_Attribute_info[] =
{
   /* attribute info of attribute measured value */
    {
       g_MS_ATTRIB_ILLUMINANCE_MEASURED_VALUE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c
    },

   /* attribute info of attribute min measured value */
   {
       g_MS_ATTRIB_ILLUMINANCE_MIN_MEASURED_VALUE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c
   },

   /* attribute info of attribute max measured value */
   {
       g_MS_ATTRIB_ILLUMINANCE_MAX_MEASURED_VALUE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c
   }
#if ( g_MS_ATTRIB_ILLUMINANCE_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 )
   ,
   /* attribute info of attribute tolerance */
   {
       g_MS_ATTRIB_ILLUMINANCE_TOLERANCE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c
   }
#endif          /* g_MS_ATTRIB_ILLUMINANCE_TOLERANCE_ATTRIBUTE_ENABLE_d */

#if ( g_MS_ATTRIB_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_ATTRIBUTE_ENABLE_d == 1 )
   ,
   /* attribute info of attribute light sensor type */
   {
       g_MS_ATTRIB_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Only_Attribute_c
   }
#endif          /* g_MS_ATTRIB_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_ATTRIBUTE_ENABLE_d */

};

/* ServerCluster Info for Illuminance Meaurement Cluster */
const ZCL_Cluster_Info_t ga_MS_Illuminance_Measurement_Server_Cluster_Info =
{
   g_ILLUMINANCE_MEASUREMENT_CLUSTER_c ,
   ( sizeof
   ( ma_Illuminance_Measurement_Cluster_Attribute_info)
   / sizeof( ZCL_Attribute_Info_t ) ),
   ma_Illuminance_Measurement_Cluster_Attribute_info
};
#endif          /* g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
/* ClentCluster Info for Illuminance Meaurement Cluster */
const ZCL_Cluster_Info_t ga_MS_Illuminance_Measurement_Client_Cluster_Info =
{
   g_ILLUMINANCE_MEASUREMENT_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* g_ILLUMINANCE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_MS_IlluminanceSensing_Cluster_Cmd (  uint8_t endPointId,
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
#endif                          /* g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
