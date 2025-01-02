/******************************************************************************
* ZCL_M_And_S_Flow_Measurement_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Flow measurement cluster attribute and
* cmmnd information
******************************************************************************/
#ifndef _ZCL_M_AND_S_FLOW_MEASUREMENT_CLUSTER_H_
#define _ZCL_M_AND_S_FLOW_MEASUREMENT_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_FLOW_MEASUREMENT_CLUSTER_c                                0x0404

/* Attribute of Flow measurement cluster */
#define g_MS_ATTRIB_FLOW_MEASURED_VALUE_c                           0x0000
#define g_MS_ATTRIB_FLOW_MIN_MEASURED_VALUE_c                       0x0001
#define g_MS_ATTRIB_FLOW_MAX_MEASURED_VALUE_c                       0x0002
#define g_MS_ATTRIB_FLOW_TOLERANCE_c                                0x0003


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_FLOW_MEASUREMENT_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Flow_Measurement_Server_Cluster_Info;
#endif                          /* g_FLOW_MEASUREMENT_SERVER_CLUSTER_ENABLE_d */

#if ( g_FLOW_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Flow_Measurement_Client_Cluster_Info;
#endif                          /* g_FLOW_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct Flow_Measurement_Cluster_Tag
{
   uint8_t a_measured_value[2];
   uint8_t a_min_measured_value[2];
   uint8_t a_max_measured_value[2];
#if ( g_MS_ATTRIB_FLOW_TOLERANCE_ATTRIBUTE_ENABLE_d ==  1 )
   uint8_t a_tolerance[2];
#endif
}__attribute__((__packed__))Flow_Measurement_Cluster_t;
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_MS_FlowMeasurement_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_M_AND_S_FLOW_MEASUREMENT_CLUSTER_H_*/
