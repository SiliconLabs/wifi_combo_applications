/******************************************************************************
* ZCL_M_And_S_Relative_Humidity_Measurement_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Relative humidity measurement cluster attribute and
* cmmnd information
******************************************************************************/
#ifndef _ZCL_M_AND_S_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_H_
#define _ZCL_M_AND_S_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_H_
#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_c                   0x0405


/************ Attribute Set Of Cluster Relative Humidity Measurement Cluster *********/

/*** Attributes of the Flow Measurement Information Attribute Set ***/
#define m_MS_ATTRIB_HUMIDITY_MEASURED_VALUE_c                       0x0000
#define m_MS_ATTRIB_HUMIDITY_MIN_MEASURED_VALUE_c                   0x0001
#define m_MS_ATTRIB_HUMIDITY_MAX_MEASURED_VALUE_c                   0x0002
#define m_MS_ATTRIB_HUMIDITY_TOLERANCE_c                            0x0003

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Humidity_Measurement_Server_Cluster_Info;
#endif              /* g_RELATIVE_HUMIDITY_MEASUREMENT_SERVER_CLUSTER_ENABLE_d */

#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Humidity_Measurement_Client_Cluster_Info;
#endif              /* g_RELATIVE_HUMIDITY_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct Humidity_Measurement_Cluster_Attributes_Tag
{
   uint8_t measured_value[2];
   uint8_t min_measured_value[2];
   uint8_t max_measured_value[2];
#if ( g_MS_ATTRIB_RELATIVE_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t tolerance[2];
#endif              /* g_MS_ATTRIB_RELATIVE_TOLERANCE_ATTRIBUTE_ENABLE_d */
}__attribute__((__packed__))Humidity_Measurement_Cluster_Attributes;

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_MS_RH_Measurement_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
#endif                  /* g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                  /*_ZCL_M_AND_S_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_H_*/
