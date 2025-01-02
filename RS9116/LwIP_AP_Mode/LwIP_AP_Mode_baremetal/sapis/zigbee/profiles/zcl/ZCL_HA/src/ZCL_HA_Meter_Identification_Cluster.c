/*******************************************************************************
* ZCL_HA_Meter_Identification_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Meter
* Identification cluster
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_METER_IDENTIFICATION_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HA_Meter_Identification_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_METER_IDENTIFICATION_SERVER_CLUSTER_ENABLE_d == 1 )
/* Following are attribute info of
g_METER_IDENTIFICATION_SERVER_CLUSTER_ENABLE_d */
const ZCL_Attribute_Info_t ma_Meter_Identification_Cluster_Attribute_info[] =
{
#if(g_COMPANY_NAME_ATTRIBUTE_ENABLE_c == 1)
    {   g_COMPANY_NAME_c,
        g_Character_String_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_METER_TYPE_ID_ATTRIBUTE_ENABLE_c == 1)
   ,{   g_METER_TYPE_ID_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_DATA_QUALITY_ID_ATTRIBUTE_ENABLE_c == 1)
   ,{   g_DATA_QUALITY_ID_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_CUSTOMER_NAME_ATTRIBUTE_ENABLE_c == 1)
   ,{   g_CUSTOMER_NAME_c,
        g_Character_String_Data_c,
        g_Read_Write_Attribute_c  }
#endif

#if(g_MODEL_ATTRIBUTE_ENABLE_c == 1)
   ,{   g_MODEL_c,
        g_Octet_String_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_PART_NUMBER_ATTRIBUTE_ENABLE_c == 1)
   ,{   g_PART_NUMBER_c,
        g_Octet_String_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_PRODUCTION_REVISION_ATTRIBUTE_ENABLE_c == 1)

   ,{   g_PRODUCTION_REVISION_c,
        g_Octet_String_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_SOFTWARE_REVISION_ATTRIBUTE_ENABLE_c == 1)

   ,{   g_SOFTWARE_REVISION_c,
        g_Octet_String_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_UTILITY_NAME_ATTRIBUTE_ENABLE_c == 1)

   ,{   g_UTILITY_NAME_c,
        g_Character_String_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_POD_ATTRIBUTE_ENABLE_c == 1)

   ,{   g_POD_c,
        g_Character_String_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_AVAILABLE_POWER_ATTRIBUTE_ENABLE_c == 1)

   ,{   g_AVAILABLE_POWER_c,
        g_Signed_Twenty_Four_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  }
#endif

#if(g_THRESHOLD_POWER_ATTRIBUTE_ENABLE_c == 1)

   ,{   g_THRESHOLD_POWER_c,
        g_Signed_Twenty_Four_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c  }
#endif
};

/* Cluster Info for Meter Identification Cluster */
const ZCL_Cluster_Info_t ga_HA_Meter_Identification_Server_Cluster_Info =
{
   g_METER_IDENTIFICATION_CLUSTER_c,
  ( sizeof( ma_Meter_Identification_Cluster_Attribute_info))/ sizeof( ZCL_Attribute_Info_t ),
   ma_Meter_Identification_Cluster_Attribute_info
};

#endif                  /* g_METER_IDENTIFICATION_SERVER_CLUSTER_ENABLE_d */

#if ( g_METER_IDENTIFICATION_CLIENT_CLUSTER_ENABLE_d == 1 )
/* Cluster Info for Meter Identification Cluster */
const ZCL_Cluster_Info_t ga_HA_Meter_Identification_Client_Cluster_Info =
{
    g_METER_IDENTIFICATION_CLUSTER_c,
    /* No CLient Attributes*/
    g_NULL_c,
    /* No CLient Attributes*/
    g_NULL_c
};
#endif                  /* g_METER_IDENTIFICATION_CLIENT_CLUSTER_ENABLE_d */
/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/* None */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* None */

/*******************************************************************************
* Public Functions
*******************************************************************************/

uint8_t ZCL_Handle_HA_MeterIdentification_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
    /* Cluster doesn't have any cluster specific commands. Hence it returns zero
    data length to the calling function. On receipt of zero data length,
    the calling function will create Default response command with status
    Unsupported cluster command */
    pActionHandler->action = Send_Response;

    return g_ZERO_DATA_LENGTH_c;
}

/*******************************************************************************
* Private Functions
*******************************************************************************/

/* None */

/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/

/* None */

/*******************************************************************************
* End Of File
*******************************************************************************/


#endif                  /* g_METER_IDENTIFICATION_CLUSTER_ENABLE_d */
