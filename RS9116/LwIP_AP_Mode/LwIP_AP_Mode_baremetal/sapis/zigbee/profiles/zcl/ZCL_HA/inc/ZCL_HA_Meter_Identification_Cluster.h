/******************************************************************************
* ZCL_HA_Meter_Identification_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Meter Identificatio attribute and command
* information
******************************************************************************/

#ifndef _ZCL_HA_METER_IDENTIFICATION_CLUSTER_H_
#define _ZCL_HA_METER_IDENTIFICATION_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/******************************************************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Meter Identification Cluster */
#define g_METER_IDENTIFICATION_CLUSTER_c                    0x0B01

/* Meter Identification Attribute information */
#define g_COMPANY_NAME_c                                    0x0000
#define g_METER_TYPE_ID_c                                   0x0001
/* Note : attribute with id 0x0002 0x0003 not defined */
#define g_DATA_QUALITY_ID_c                                 0x0004
#define g_CUSTOMER_NAME_c                                   0x0005
#define g_MODEL_c                                           0x0006
#define g_PART_NUMBER_c                                     0x0007
#define g_PRODUCTION_REVISION_c                             0x0008
/* Note : attribute with id 0x0008 not defined */
#define g_SOFTWARE_REVISION_c                               0x000A
#define g_UTILITY_NAME_c                                    0x000B
    /* Point Of Delivery Attribute*/
#define g_POD_c                                             0x000C
#define g_AVAILABLE_POWER_c                                 0x000D
#define g_THRESHOLD_POWER_c                                 0x000E



/******************************************************************************
* Memory Declarations
******************************************************************************/

#if ( g_METER_IDENTIFICATION_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HA_Meter_Identification_Client_Cluster_Info;
#endif

#if ( g_METER_IDENTIFICATION_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HA_Meter_Identification_Server_Cluster_Info;
#endif

/******************************************************************************
* Type Definitions
******************************************************************************/
/* Meter Identification Cluster Attributes structure */
typedef struct Meter_Identification_Cluster_Tag
{
    uint8_t companyName[16];
    uint8_t meterTypeId[2];
    uint8_t dataQualityId[2];
    uint8_t customerName[16];
    uint8_t model[16];
    uint8_t partNumber[16];
    uint8_t productRevision[16];
    uint8_t softwareRevision[16];
    uint8_t utilityName[16];
    uint8_t pointOfDelivery[16];
    uint8_t availablePower[3];
    uint8_t powerThreshold[3];
} __attribute__((__packed__))Meter_Identification_Cluster_t;

/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_HA_MeterIdentification_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                               /*_ZCL_HA_METER_IDENTIFICATION_CLUSTER_H_*/
