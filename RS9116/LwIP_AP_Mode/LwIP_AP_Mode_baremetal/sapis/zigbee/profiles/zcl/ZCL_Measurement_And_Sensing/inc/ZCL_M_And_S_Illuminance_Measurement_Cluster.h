/******************************************************************************
* ZCL_M_And_S_Illuminance_Measurement_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Illuminance_measurement_Cluster attribute and cmmnd information
******************************************************************************/
#ifndef _ZCL_M_AND_S_ILLUMINANCE_MEASUREMENT_CLUSTER_H_
#define _ZCL_M_AND_S_ILLUMINANCE_MEASUREMENT_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*Attributes and commands for configuring the measurement of illuminance, and
reporting illuminance measurements.0x0400*/
#define g_ILLUMINANCE_MEASUREMENT_CLUSTER_c                         0x0400


/************ Attribute Set Of Cluster Illuminance Measurement Cluster *********/
#define g_MS_ILLUMINANCE_MEASUREMENT_INFO_AS_c     0x000

/* The attribute set of cluster Illuminance Measurement Cluster is
   Illuminance Measurement Information and following are attributes of Illuminance
   Measurement Information attribute set  */

#define g_MS_ATTRIB_ILLUMINANCE_MEASURED_VALUE_c                 0x0000
/* m_ZCL_MS_ATTRIB_MEASURED_VALUE_c is read only, unsigned integer and ranges
from m_ZCL_MS_ATTRIB_MIN_MEASURED_VALUE_c to m_ZCL_MS_ATTRIB_MAX_MEASURED_VALUE_c*/

#define g_MS_ATTRIB_ILLUMINANCE_MIN_MEASURED_VALUE_c             0x0001
/* m_ZCL_MS_ATTRIB_MIN_MEASURED_VALUE_c is read only, unsigned integer and ranges
from 0x0002 - 0xfffd */

#define g_MS_ATTRIB_ILLUMINANCE_MAX_MEASURED_VALUE_c             0x0002
/* m_ZCL_MS_ATTRIB_MAX_MEASURED_VALUE_c is read only, unsigned integer and ranges
from 0x0001 - 0xfffe */


#define g_MS_ATTRIB_ILLUMINANCE_TOLERANCE_c                      0x0003
/* m_ZCL_MS_ATTRIB_TOLERANCE__c is read only, unsigned integer and ranges
from 0x0000 - 0x0800 */

#define g_MS_ATTRIB_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_c  0x0004
/* m_ZCL_MS_ATTRIB_LIGHT_SENSOR_TYPE_c is read only, 8-bit
Enumeration and ranges from 0x00 - 0xff */

/* Values of the LightSensorType Attribute */
enum
{
   g_Photo_Diode_c   = 0x00,
   g_CMOS_c          = 0x01,
   g_Unknown_c       = 0xFF
/* Reserved = from 0x02 to 0x3f */
/* Reserved for manufacturer specific light sensor types = 0x40 to 0xfe */

};
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Illuminance_Measurement_Server_Cluster_Info ;
#endif          /* g_ILLUMINANCE_MEASUREMENT_CLUSTER_ENABLE_d */

#if ( g_ILLUMINANCE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Illuminance_Measurement_Client_Cluster_Info ;
#endif          /* g_ILLUMINANCE_MEASUREMENT_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct IIluminance_Measurement_Cluster_Tag
{
   uint8_t a_measured_value[2];
   uint8_t a_min_measured_value[2];
   uint8_t a_max_measured_value[2];
#if ( g_MS_ATTRIB_ILLUMINANCE_TOLERANCE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_tolerance[2];
#endif

#if ( g_MS_ATTRIB_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t light_sensor_type;
#endif
}__attribute__((__packed__))IIluminance_Measurement_Cluster_t;

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_MS_IlluminanceSensing_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_M_AND_S_ILLUMINANCE_MEASUREMENT_CLUSTER_H_*/
