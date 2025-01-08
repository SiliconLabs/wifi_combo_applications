/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "common_includes.h"
#include "ZCL_Foundation.h"
#if ( g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Dehumidification_Control_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/* Following are attribute info of Dehumidification Control for Server */
#if ( g_DEHUMIDIFICATION_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Dehumidification_Control_Server_Cluster_Attribute_info[] =
{
#if ( g_RELATIVE_HUMIDITY_ATTRIBUTE_ENABLE_d == 1 )
    {  g_RELATIVE_HUMIDITY_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c } ,
#endif                                  /* ( g_RELATIVE_HUMIDITY_ATTRIBUTE_ENABLE_d == 1 ) */

    {  g_DEHUMIDIFICATION_COOLING_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c },

    {  g_RHDEHUMIDIFICATION_SETPOINT_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },

#if ( g_RELATIVE_HUMIDITY_MODE_ATTRIBUTE_ENABLE_d == 1 )
    {  g_RELATIVE_HUMIDITY_MODE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c },
#endif                                  /* ( g_RELATIVE_HUMIDITY_MODE_ATTRIBUTE_ENABLE_d == 1 ) */

#if ( g_DEHUMIDIFICATION_LOCK_OUT_ATTRIBUTE_ENABLE_d == 1 )

    {  g_DEHUMIDIFICATION_LOCKOUT_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c },
#endif                                  /* ( g_DEHUMIDIFICATION_LOCK_OUT_ATTRIBUTE_ENABLE_d == 1 ) */

    {  g_DEHUMIDIFICATION_HYSTERISIS_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },


    {  g_DEHUMIDIFICATION_MAX_COOL_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },

#if ( g_RELATIVE_HUMIDITY_DISPLAY_ATTRIBUTE_ENABLE_d == 1 )
    {  g_RELATIVE_HUMIDITY_DISPLAY_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif                                  /* ( g_RELATIVE_HUMIDITY_DISPLAY_ATTRIBUTE_ENABLE_d == 1 ) */
};


/* Dehumidification Control cluster info */
const ZCL_Cluster_Info_t ga_HVAC_Dehumidification_Server_Control_Cluster_Info =
{
	g_DEHUMIDIFICATION_CONTROL_CLUSTER_c,
    sizeof(ma_Dehumidification_Control_Server_Cluster_Attribute_info)/ sizeof(ZCL_Attribute_Info_t),
    ma_Dehumidification_Control_Server_Cluster_Attribute_info
};
#endif          /* ( g_DEHUMIDIFICATION_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 ) */

/* Dehumidification Control Client cluster info */
#if ( g_DEHUMIDIFICATION_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_HVAC_Dehumidification_Client_Control_Cluster_Info =
{
	g_DEHUMIDIFICATION_CONTROL_CLUSTER_c,
    g_NULL_c,
    g_NULL_c
};
#endif

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

uint8_t ZCL_Handle_HVAC_DHControl_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler *pActionHandler,
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

#endif /* ( g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d == 1 ) */
