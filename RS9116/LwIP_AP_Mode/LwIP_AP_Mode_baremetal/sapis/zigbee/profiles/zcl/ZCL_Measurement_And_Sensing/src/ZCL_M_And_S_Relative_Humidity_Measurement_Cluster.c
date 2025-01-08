/*******************************************************************************
* ZCL_M_And_S_Relative_Humidity_Measurement_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Relative humidity
*  cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "common_includes.h"
#include "ZCL_Foundation.h"
#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Relative_Humidity_Measurement_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_c */
const ZCL_Attribute_Info_t ma_Humidity_Measurement_Cluster_Attribute_info[] =
{
    {   m_MS_ATTRIB_HUMIDITY_MEASURED_VALUE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c | g_Reportable_Attribute_c  },


    {   m_MS_ATTRIB_HUMIDITY_MIN_MEASURED_VALUE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c },


    {   m_MS_ATTRIB_HUMIDITY_MAX_MEASURED_VALUE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c }

#if ( g_MS_ATTRIB_RELATIVE_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 )
    ,
    {   m_MS_ATTRIB_HUMIDITY_TOLERANCE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c | g_Reportable_Attribute_c }
#endif                      /* ( g_MS_ATTRIB_RELATIVE_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 ) */
};

/* Cluster Info for Humidity Measurement Server Cluster */
const ZCL_Cluster_Info_t ga_MS_Humidity_Measurement_Server_Cluster_Info =
{
    g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_c,
    sizeof ( ma_Humidity_Measurement_Cluster_Attribute_info) / sizeof( ZCL_Attribute_Info_t ),
    ma_Humidity_Measurement_Cluster_Attribute_info
};
#endif              /* g_RELATIVE_HUMIDITY_MEASUREMENT_SERVER_CLUSTER_ENABLE_d */

#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Humidity Measurement Client Cluster */
const ZCL_Cluster_Info_t ga_MS_Humidity_Measurement_Client_Cluster_Info =
{
    g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_c,
    g_NULL_c,
    g_NULL_c
};
#endif              /* g_RELATIVE_HUMIDITY_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_MS_RH_Measurement_Cluster_Cmd (  uint8_t endPointId,
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


#endif              /* ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 ) */
