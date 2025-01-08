/******************************************************************************
* ZCL_M_And_S_Illuminance_Level_Sensing_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Illuminance level sensing cluster attribute and
* cmmnd information
******************************************************************************/

#ifndef _ZCL_M_AND_S_ILLUMINANCE_LEVEL_SENSING_CLUSTER_H_
#define _ZCL_M_AND_S_ILLUMINANCE_LEVEL_SENSING_CLUSTER_H_

#if ( g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_c                       0x0401

#define g_MS_ATTRIB_ILLUMINANCE_LEVEL_STATUS_c                      0x0000
#define g_MS_ATTRIB_ILLUMINANCE_LEVEL_SENSING_LIGHT_SENSOR_TYPE_c   0x0001
#define g_MS_ATTRIB_ILLUMINANCE_TARGET_LEVEL_c                      0x0010

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_ILLUMINANCE_LEVEL_SENSING_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Illuminance_Level_Sensing_Server_Cluster_Info;
#endif                  /* g_ILLUMINANCE_LEVEL_SENSING_SERVER_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_LEVEL_SENSING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Illuminance_Level_Sensing_Client_Cluster_Info;
#endif                  /* g_ILLUMINANCE_LEVEL_SENSING_CLIENT_CLUSTER_ENABLE_d */

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct IIluminance_Level_Sensing_Cluster_Attributes_Tag
{
   uint8_t level_status;
#if ( g_MS_ATTRIB_ILLUMINANCE_LEVEL_SENSING_LIGHT_SENSOR_TYPE_ENABLE_d == 1 )
   uint8_t light_sensor_type;
#endif                  /* g_MS_ATTRIB_ILLUMINANCE_LEVEL_SENSING_LIGHT_SENSOR_TYPE_ENABLE_d */
   uint16_t illuminance_target_level;
}__attribute__((__packed__))IIluminance_Level_Sensing_Cluster_Attributes;

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_MS_LevelSensing_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

#endif                          /* g_ILLUMINANCE_LEVEL_SENSING_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                      /*_ZCL_M_AND_S_ILLUMINANCE_LEVEL_SENSING_CLUSTER_H_*/
