/*******************************************************************************
* ZCL_Closures_Shade_Configuration_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Shade configuration
* cluster
*******************************************************************************/

/*-----------------------------------------------------------------------------
 Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Closures_Shade_Configuration_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_SHADE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of Shade Configuration Cluster */
const ZCL_Attribute_Info_t ma_Shade_Configuration_Cluster_Server_Attribute_info [] =
{
#if ( g_PHYSICAL_CLOSED_LIMIT_ENABLE_d == 1 )
    {   g_PHYSICAL_CLOSED_LIMIT_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    },
#endif                      /* g_PHYSICAL_CLOSED_LIMIT_ENABLE_d */

#if ( g_MOTOR_STEP_SIZE_ENABLE_d == 1 )
    {   g_MOTOR_STEP_SIZE_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    },
#endif                      /* g_MOTOR_STEP_SIZE_ENABLE_d */
    {   g_STATUS_c,
        g_Eight_Bit_Bitmap_Data_c,
        g_Read_Write_Attribute_c
    },

    {   g_CLOSED_LIMIT_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c
    },

    {   g_MODE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Write_Attribute_c
    }

};

const ZCL_Cluster_Info_t ga_Shade_Configuration_Cluster_Server_Attribute_info =
{
   g_SHADE_CONFIGURATION_CLUSTER_c,
   sizeof ( ma_Shade_Configuration_Cluster_Server_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Shade_Configuration_Cluster_Server_Attribute_info
};

#endif                      /* g_SHADE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d*/

#if ( g_SHADE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Closures_Shade_Configuration_Client_Cluster_Info =
{
   g_SHADE_CONFIGURATION_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif                      /* g_SHADE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d*/
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

uint8_t ZCL_Handle_Shade_Config_Cluster_Cmd (  uint8_t endPointId,
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

#endif
