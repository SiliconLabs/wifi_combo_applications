#ifndef _ZCL_M_AND_S_OCCUPANCY_SENSING_CLUSTER_H_
#define _ZCL_M_AND_S_OCCUPANCY_SENSING_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_OCCUPANCY_SENSING_CLUSTER_c                               0x0406

#define g_MS_ATTRIB_OCCUPANCY_c                                     0x0000
#define g_MS_ATTRIB_OCCUPANCY_SENSOR_TYPE_c                         0x0001
#define g_MS_ATTRIB_PIR_OCCUPIED_TO_UNOCCUPIED_DELAY_c              0x0010
#define g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_DELAY_c              0x0011
#define g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_c			0x0012

#define g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_c               0x0020
#define g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_DELAY_c               0x0021
#define g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_c			0x0022

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_OCCUPANCY_SENSING_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Occupancy_Sensing_Server_Cluster_Info ;
#endif					/* g_OCCUPANCY_SENSING_SERVER_CLUSTER_ENABLE_d */

#if ( g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_MS_Occupancy_Sensing_Client_Cluster_Info ;
#endif					/* g_OCCUPANCY_SENSING_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct Occupancy_Sensing_Cluster_Tag
{
   uint8_t occupancy;
   uint8_t occupancy_sensor_type;
#if ( g_MS_ATTRIB_PIR_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t PIR_occupied_to_unoccupied_delay[2];
#endif

#if ( g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_DELAY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t PIR_unoccupied_to_occupied_delay[2];
#endif

#if ( g_MS_ATTRIB_PIR_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t PIR_unoccupied_to_occupied_threshold;
#endif

#if ( g_MS_ATTRIB_US_OCCUPIED_TO_UNOCCUPIED_DELAY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t ultra_sonic_occupied_to_unoccupied_delay[2];
#endif

#if ( g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_DELAY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t ultra_sonic_unoccupied_to_occupied_delay[2];
#endif

#if ( g_MS_ATTRIB_US_UNOCCUPIED_TO_OCCUPIED_THRESHOLD_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t ultra_sonic_unoccupied_to_occupied_threshold;
#endif

}__attribute__((__packed__))Occupancy_Sensing_Cluster_t;
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_MS_OccupancySensing_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_M_AND_S_OCCUPANCY_SENSING_CLUSTER_H_*/
