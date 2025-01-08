#ifndef _ZCL_M_AND_S_TEMP_MEASUREMENT_CLUSTER_H_
#define _ZCL_M_AND_S_TEMP_MEASUREMENT_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_TEMPERATURE_MEASUREMENT_CLUSTER_c                      0x0402

#define g_MS_ATTRIB_TEMPERATURE_MEASURED_VALUE_c                 0x0000
#define g_MS_ATTRIB_TEMPERATURE_MIN_MEASURED_VALUE_c             0x0001
#define g_MS_ATTRIB_TEMPERATURE_MAX_MEASURED_VALUE_c             0x0002
#define g_MS_ATTRIB_TEMPERATURE_TOLERANCE_c                      0x0003



/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct Temperature_Measurement_Cluster_Tag
{
   uint8_t a_measured_value[2];
   uint8_t a_min_measured_value[2];
   uint8_t a_max_measured_value[2];
#if ( g_MS_ATTRIB_TEMPERATURE_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_tolerance[2];
#endif       /* ( g_MS_ATTRIB_TEMPERATURE_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 ) */
}__attribute__((__packed__))Temperature_Measurement_Cluster_t;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_TEMPERATURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Temperature_Measurement_Server_Cluster_Info;
#endif       /* ( g_TEMPERATURE_MEASUREMENT_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_TEMPERATURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Temperature_Measurement_Client_Cluster_Info;
#endif       /* ( g_TEMPERATURE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_MS_TempMeasurement_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_M_AND_S_TEMP_MEASUREMENT_CLUSTER_H_*/
