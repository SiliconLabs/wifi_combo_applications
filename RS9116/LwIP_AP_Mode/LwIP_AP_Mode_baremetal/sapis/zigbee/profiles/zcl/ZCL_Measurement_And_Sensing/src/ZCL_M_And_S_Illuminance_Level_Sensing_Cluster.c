/*******************************************************************************
* ZCL_M_And_S_Illuminance_Level_Sensing_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Illuminanace level
*  cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )

#include "ZCL_M_And_S_Illuminance_Level_Sensing_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_ILLUMINANCE_LEVEL_SENSING_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_c */
const ZCL_Attribute_Info_t ma_Illuminance_Level_Sensing_Cluster_Attribute_info[] =
{

    {   g_MS_ATTRIB_ILLUMINANCE_LEVEL_STATUS_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Only_Attribute_c | g_Reportable_Attribute_c  },

#if ( g_MS_ATTRIB_ILLUMINANCE_LEVEL_SENSING_LIGHT_SENSOR_TYPE_ENABLE_d == 1 )
    {   g_MS_ATTRIB_ILLUMINANCE_LEVEL_SENSING_LIGHT_SENSOR_TYPE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Only_Attribute_c },
#endif                  /* g_MS_ATTRIB_ILLUMINANCE_LEVEL_SENSING_LIGHT_SENSOR_TYPE_ENABLE_d */
    {   g_MS_ATTRIB_ILLUMINANCE_TARGET_LEVEL_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c    }

};

/* Cluster Info for Illuminance Level Sensing Cluster */
const ZCL_Cluster_Info_t ga_MS_Illuminance_Level_Sensing_Server_Cluster_Info =
{
   g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_c,
   sizeof( ma_Illuminance_Level_Sensing_Cluster_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Illuminance_Level_Sensing_Cluster_Attribute_info
};

#endif                  /* g_ILLUMINANCE_LEVEL_SENSING_SERVER_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_LEVEL_SENSING_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Illuminance Level Sensing Cluster */
const ZCL_Cluster_Info_t ga_MS_Illuminance_Level_Sensing_Client_Cluster_Info =
{
   g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif                  /* g_ILLUMINANCE_LEVEL_SENSING_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_MS_LevelSensing_Cluster_Cmd (  uint8_t endPointId,
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


#endif                  /* g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d */
