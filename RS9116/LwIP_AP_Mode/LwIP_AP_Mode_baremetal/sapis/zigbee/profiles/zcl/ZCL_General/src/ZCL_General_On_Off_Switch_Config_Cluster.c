/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Common.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if(g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_On_Off_Switch_Config_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/*Attribute Info List for Switch Type and Switch Actions -
   OnOff Switch Configuration Cluster*/
#if ( g_ON_OFF_SWITCH_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_On_Off_Switch_Config_Attribute_List[] =
{
   {
      g_SWITCH_TYPE_ATTRIBUTE_c,       /*attribute id*/
      g_Eight_Bit_Enumeration_Data_c,  /*attribute data type*/
      g_Read_Only_Attribute_c,         /*access permission*/
   },
   {
      g_SWITCH_ACTIONS_ATTRIBUTE_c,    /*attribute id*/
      g_Eight_Bit_Enumeration_Data_c,  /*attribute data type*/
      g_Read_Write_Attribute_c,        /*access permission*/
   }
};
/*Cluster Info for OnOff Switch Configuration Cluster*/
const ZCL_Cluster_Info_t ga_On_Off_Switch_Config_Server_Cluster_Info =
{
   g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_c,
   (sizeof(ma_On_Off_Switch_Config_Attribute_List))/sizeof(ZCL_Attribute_Info_t),
   ma_On_Off_Switch_Config_Attribute_List
};
#endif          /* ( g_ON_OFF_SWITCH_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1) */

/*Cluster Info for OnOff Switch Configuration Cluster*/
#if ( g_ON_OFF_SWITCH_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1)
const ZCL_Cluster_Info_t ga_On_Off_Switch_Config_Client_Cluster_Info =
{
   g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* ( g_ON_OFF_SWITCH_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1) */
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

uint8_t ZCL_Handle_OnOff_Switch_Config_Cluster_Cmd (  uint8_t endPointId,
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
#endif    /*g_ON_OFF_SWITCH_CONFIGURATION_CLUSTER_ENABLE_d*/
