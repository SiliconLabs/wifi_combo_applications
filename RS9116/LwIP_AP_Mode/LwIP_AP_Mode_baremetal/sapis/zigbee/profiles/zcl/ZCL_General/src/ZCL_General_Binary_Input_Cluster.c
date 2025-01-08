/*******************************************************************************
* ZCL_General_Binary_Input_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Binary Input (Basic)
* cluster
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_BINARY_INPUT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Binary_Input_Cluster.h"



/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_BINARY_INPUT_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Binary_Input_Attribute []=
{
#if ( g_ACTIVE_TEXT_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_ACTIVE_TEXT_ATTRIBUTE_c,
        g_Character_String_Data_c,
        g_Read_Write_Attribute_c
    },
#endif                              /*  g_ACTIVE_TEXT_ATTRIBUTE_ENABLE_d */

#if ( g_DESCRIPTION_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_DESCRIPTION_ATTRIBUTE_c,
        g_Character_String_Data_c,
        g_Read_Write_Attribute_c
    },
#endif                                  /* g_DESCRIPTION_ATTRIBUTE_ENABLE_d */

#if ( g_INACTIVE_TEXT_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_INACTIVE_TEXT_ATTRIBUTE_c,
        g_Character_String_Data_c,
        g_Read_Write_Attribute_c
    },
#endif                                  /* g_INACTIVE_TEXT_ATTRIBUTE_ENABLE_d */

    {
        g_OUT_OF_SERVICE_ATTRIBUTE_c,
        g_Boolean_Data_c,
        g_Read_Write_Attribute_c
    },

#if ( g_POLARITY_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_POLARITY_ATTRIBUTE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Only_Attribute_c
    },
#endif                                  /* g_POLARITY_ATTRIBUTE_ENABLE_d */

    {
        g_PRESENT_VALUE_ATTRIBUTE_c,
        g_Boolean_Data_c,
        g_Read_Write_Attribute_c | g_Reportable_Attribute_c
    },

#if ( g_RELIABILITY_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_RELIABILITY_ATTRIBUTE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Write_Attribute_c
    },
#endif                                  /* g_RELIABILITY_ATTRIBUTE_ENABLE_d */

    {
        g_STATUS_FLAGS_ATTRIBUTE_c,
        g_Eight_Bit_Bitmap_Data_c,
        g_Read_Only_Attribute_c |g_Reportable_Attribute_c
    },

#if ( g_APPLICATION_TYPE_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_APPLICATION_TYPE_ATTRIBUTE_c,
        g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c
    }
#endif                              /* g_APPLICATION_TYPE_ATTRIBUTE_ENABLE_d */
};

/* Cluster Info for Alarms Cluster */
const ZCL_Cluster_Info_t ga_Binary_Input_Server_Cluster_Info =
{
   g_BINARY_INPUT_CLUSTER_c,
   sizeof(ma_Binary_Input_Attribute)/sizeof(ZCL_Attribute_Info_t),
   ma_Binary_Input_Attribute
};
#endif                              /* g_BINARY_INPUT_SERVER_CLUSTER_ENABLE_d */

#if ( g_BINARY_INPUT_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Binary_Input_Client_Cluster_Info =
{
   g_BINARY_INPUT_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif                              /* g_BINARY_INPUT_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/




/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Binary_Input_Cluster_Cmd
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
)
{
/* It doesn't have any cluster specific commands. Hence it returns zero data
    length to the calling function. On receipt of zero data length, the calling
    function will create Default response command with status
    Unsupported cluster command */
    pActionHandler->action = Send_Response;
     pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;
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
#endif                                  /* g_BASIC_INPUT_CLUSIER_ENABLE_d */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
