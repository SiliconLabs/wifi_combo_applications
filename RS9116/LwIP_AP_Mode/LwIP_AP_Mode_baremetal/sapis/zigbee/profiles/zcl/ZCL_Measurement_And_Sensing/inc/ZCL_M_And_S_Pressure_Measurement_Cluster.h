/******************************************************************************
* ZCL_M_And_S_Pressure_Measurement_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Pressure measurement cluster attribute and
* cmmnd information
******************************************************************************/

#ifndef _ZCL_M_AND_S_PRESSURE_MEASUREMENT_CLUSTER_H_
#define _ZCL_M_AND_S_PRESSURE_MEASUREMENT_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_PRESSURE_MEASUREMENT_CLUSTER_c                            0x0403

#define g_MS_ATTRIB_PRESSURE_MEASURED_VALUE_c                       0x0000
#define g_MS_ATTRIB_PRESSURE_MIN_MEASURED_VALUE_c                   0x0001
#define g_MS_ATTRIB_PRESSURE_MAX_MEASURED_VALUE_c                   0x0002
#define g_MS_ATTRIB_PRESSURE_TOLERANCE_c                            0x0003

#define g_MS_ATTRIB_PRESSURE_SCALED_VALUE_c                         0x0010
#define g_MS_ATTRIB_PRESSURE_MIN_SCALED_VALUE_c                     0x0011
#define g_MS_ATTRIB_PRESSURE_MAX_SCALED_VALUE_c                     0x0012
#define g_MS_ATTRIB_PRESSURE_SCALED_TOLERANCE_c                     0x0013
#define g_MS_ATTRIB_PRESSURE_SCALE_c                                0x0014

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct Pressure_Measurement_Cluster_Tag
{
   uint8_t a_measured_value[2];
   uint8_t a_min_measured_value[2];
   uint8_t a_max_measured_value[2];
#if ( g_MS_ATTRIB_PRESSURE_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_tolerance[2];
#endif                      /* g_MS_ATTRIB_PRESSURE_TOLERANCE_ATTRIBUTE_ENABLE_d */

#if ( g_MS_ATTRIB_SCALED_VALUE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_scaled_value[2];
#endif                      /* g_MS_ATTRIB_SCALED_VALUE_ATTRIBUTE_ENABLE_d */

#if ( g_MS_ATTRIB_MIN_SCALED_VALUE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_min_scaled_value[2];
#endif                      /* g_MS_ATTRIB_MIN_SCALED_VALUE_ATTRIBUTE_ENABLE_d */

#if ( g_MS_ATTRIB_MAX_SCALED_VALUE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_max_scaled_value[2];
#endif                      /* g_MS_ATTRIB_MAX_SCALED_VALUE_ATTRIBUTE_ENABLE_d */

#if ( g_MS_ATTRIB_SCALED_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_scaled_tolerance[2];
#endif                      /* g_MS_ATTRIB_SCALED_TOLERANCE_ATTRIBUTE_ENABLE_d */

#if ( g_MS_ATTRIB_SCALE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_scale;
#endif                      /* g_MS_ATTRIB_SCALE_ATTRIBUTE_ENABLE_d */
}__attribute__((__packed__))Pressure_Measurement_Cluster_t;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_PRESSURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Pressure_Measurement_Server_Cluster_Info ;
#endif                      /* g_PRESSURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d */

#if ( g_PRESSURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Pressure_Measurement_Client_Cluster_Info ;
#endif                      /* g_PRESSURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_MS_PressureMeasurement_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_M_AND_S_PRESSURE_MEASUREMENT_CLUSTER_H_*/
