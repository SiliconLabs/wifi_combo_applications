/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Occupancy_Sensing_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_OCCUPANCY_SENSING_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of g_OCCUPANCY_SENSING_CLUSTER_c */
const ZCL_Attribute_Info_t ma_Occupancy_Sensing_Cluster_Attribute_info[] =
{
    {   g_MS_ATTRIB_OCCUPANCY_c,
        g_Eight_Bit_Bitmap_Data_c,
        g_Read_Only_Attribute_c | g_Reportable_Attribute_c  },

    {   g_MS_ATTRIB_OCCUPANCY_SENSOR_TYPE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Only_Attribute_c }

#if ( g_MS_ATTRIB_PIR_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d == 1 )
    ,
    {   g_MS_ATTRIB_PIR_OCCUPIED_TO_UNOCCUPIED_DELAY_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c    }

#endif               /* g_MS_ATTRIB_PIR_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d */

#if ( g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_DELAY_ATTRIBUTE_ENABLE_d == 1 )
     ,
    {   g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_DELAY_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c    }

#endif               /* g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_DELAY_ATTRIBUTE_ENABLE_d */
		
#if ( g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
     ,
    {   g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c    }

#endif               /* g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ATTRIBUTE_ENABLE_d */		

#if ( g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d == 1 )
    ,
    {   g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c    }
#endif              /* g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d */

#if ( g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d == 1 )
    ,
    {   g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_DELAY_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c    }
#endif              /* g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d */
		
#if ( g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
     ,
    {   g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c    }

#endif               /* g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ATTRIBUTE_ENABLE_d */		
};

/* Cluster Info for Occupancy Sensing Server Cluster */
const ZCL_Cluster_Info_t ga_MS_Occupancy_Sensing_Server_Cluster_Info =
{
    g_OCCUPANCY_SENSING_CLUSTER_c,
    sizeof ( ma_Occupancy_Sensing_Cluster_Attribute_info) / sizeof( ZCL_Attribute_Info_t ) ,
    ma_Occupancy_Sensing_Cluster_Attribute_info
};
#endif					/* g_OCCUPANCY_SENSING_SERVER_CLUSTER_ENABLE_d */

#if ( g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Occupancy Sensing Client Cluster */
const ZCL_Cluster_Info_t ga_MS_Occupancy_Sensing_Client_Cluster_Info =
{
    g_OCCUPANCY_SENSING_CLUSTER_c,
    g_NULL_c,
	g_NULL_c
};
#endif					/* g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d */
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

uint8_t ZCL_Handle_MS_OccupancySensing_Cluster_Cmd (  uint8_t endPointId,
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


#endif               /* ( g_OCCUPANCY_SENSING_CLUSTER_ENABLE_d == 1 ) */
