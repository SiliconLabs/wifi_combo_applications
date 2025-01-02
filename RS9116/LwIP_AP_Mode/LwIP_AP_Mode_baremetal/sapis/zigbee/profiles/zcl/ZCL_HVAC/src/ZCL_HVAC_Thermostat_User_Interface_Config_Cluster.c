/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "common_includes.h"
#include "ZCL_Foundation.h"
#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Thermostat_User_Interface_Config_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/* Following are attribute info of Thermostat User Interface Configuration Cluster */
#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Thermostat_User_Interface_Config_Server_Cluster_Attribute_info[] =
{
    {  g_TEMPERATURE_DISPLAY_MODE_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c },

    {  g_KEYPAD_LOCKOUT_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#if(g_HA_1_2_d == 1) 	
#if (g_SCHEDULE_PROGRAMMING_VISIBILTY_ATTRIBUTE_ENABLE_d == 1)
     ,{  g_SCHEDULE_PROGRAMMING_VISIBILTY_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif
#endif	
	
};
	
	
const ZCL_Cluster_Info_t ga_HVAC_Thermostat_User_Interface_Config_Server_Cluster_Info =
{
   g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_c,
   sizeof( ma_Thermostat_User_Interface_Config_Server_Cluster_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Thermostat_User_Interface_Config_Server_Cluster_Attribute_info
};
#endif          /* ( g_THERMOSTAT_USER_INTERFACE_CONFIG_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_HVAC_Thermostat_User_Interface_Config_Client_Cluster_Info =
{
   g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLIENT_CLUSTER_ENABLE_d == 1 ) */

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

uint8_t ZCL_Handle_HVAC_TUInterface_Cluster_Cmd (  uint8_t endPointId,
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


#endif /* ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d == 1 ) */
