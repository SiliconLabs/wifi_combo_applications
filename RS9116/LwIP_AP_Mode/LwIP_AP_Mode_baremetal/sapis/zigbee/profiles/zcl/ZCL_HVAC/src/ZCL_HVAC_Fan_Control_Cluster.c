/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "common_includes.h"
#include "ZCL_Foundation.h"
#if ( g_FAN_CONTROL_CLUSTER_ENABLE_d ==  1 )
#include "ZCL_HVAC_Fan_Control_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/* Following are attribute info of Fan control */
#if ( g_FAN_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Fan_Control_Server_Cluster_Attribute_info[]=
{
    {  g_FAN_MODE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c },

    {  g_FAN_MODE_SEQUENCE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
};

/* Fan control  cluster info */
const ZCL_Cluster_Info_t ga_HVAC_Fan_Control_Server_Cluster_Info =
{
   g_FAN_CONTROL_CLUSTER_c,
   sizeof ( ma_Fan_Control_Server_Cluster_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ) ,
   ma_Fan_Control_Server_Cluster_Attribute_info
};
#endif              /* ( g_FAN_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_FAN_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_HVAC_Fan_Control_Client_Cluster_Info =
{
   g_FAN_CONTROL_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* ( g_FAN_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 ) */
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

uint8_t ZCL_Handle_HVAC_FanControl_Cluster_Cmd (  uint8_t endPointId,
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

#endif /* ( g_FAN_CONTROL_CLUSTER_ENABLE_d ==  1) */
