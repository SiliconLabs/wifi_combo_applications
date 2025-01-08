/******************************************************************************
* ZCL_HA_Appliance_Identification_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Appliance Identificatio attribute and command
* information
******************************************************************************/

#ifndef _ZCL_HA_APPLIANCE_IDENTIFICATION_CLUSTER_H_
#define _ZCL_HA_APPLIANCE_IDENTIFICATION_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/******************************************************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Appliance Identification Cluster */
#define g_APPLIANCE_IDENTIFICATION_CLUSTER_c                    0x0B00

/* Appliance Identification Attribute information */

/* Attibute Set Zero*/
#define g_BASIC_IDENTIFICATION_c                                0x0000

/* Attibute Set One */
#define g_APPLIANCE_CLUSTER_COMPANY_NAME_c                      0x0010
#define g_COMPANY_ID_c                                          0x0011
#define g_BRAND_NAME_c                                          0x0012
#define g_BRAND_ID_c                                            0x0013
#define g_APPLIANCE_CLUSTER_MODEL_c                             0x0014
#define g_APPLIANCE_CLUSTER_PART_NUMBER_c                       0x0015
#define g_PRODUCT_REVISION_c                                    0x0016
#define g_APPLIANCE_CLUSTER_SOFTWARE_REVISION_c                 0x0017
#define g_PRODUCT_TYPE_NAME_c                                   0x0018
#define g_PRODUCT_TYPE_ID_c                                     0x0019
#define g_CECED_SPECIFICATION_VERSION_c                         0x001a

/******************************************************************************
* Memory Declarations
******************************************************************************/

#if ( g_EN50523_APPLIANCE_IDENTIFICATION_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Appliance_Identification_Client_Cluster_Info;
#endif

#if ( g_EN50523_APPLIANCE_IDENTIFICATION_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Appliance_Identification_Server_Cluster_Info;
#endif

/******************************************************************************
* Type Definitions
******************************************************************************/
/* Appliance Identification Cluster Attributes structure */
typedef struct Appliance_Identification_Cluster_Tag
{
    uint8_t basicIdentification[7];
    uint8_t companyName[16];
    uint8_t companyId[2];
    uint8_t brandName[16];
    uint8_t brandId[2];
    uint8_t model[16];
    uint8_t partNumber[16];
    uint8_t productRevision[16];
    uint8_t softwareRevision[16];
    uint8_t productTypeName[16];
    uint8_t productTypeId[2];
    uint8_t CECEDspecificationVersion[1];
} __attribute__((__packed__))Appliance_Identification_Cluster_t;

/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_HA_ApplianceIdentification_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                          /*_ZCL_HA_APPLIANCE_IDENTIFICATION_CLUSTER_H_*/
