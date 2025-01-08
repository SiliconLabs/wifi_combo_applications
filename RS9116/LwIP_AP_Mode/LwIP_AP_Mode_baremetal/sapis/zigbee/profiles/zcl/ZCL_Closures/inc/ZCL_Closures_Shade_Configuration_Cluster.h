/******************************************************************************
* ZCL_Closures_Shade_Configuration_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Shade configuration cluster attribute and cmmnd
* information
******************************************************************************/
#ifndef _ZCL_CLOSURES_SHADE_CONFIGURATION_H_
#define _ZCL_CLOSURES_SHADE_CONFIGURATION_H_

#if ( g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/


/* Shade configuration cluster */
#define g_SHADE_CONFIGURATION_CLUSTER_c                         0x0100

#define g_PHYSICAL_CLOSED_LIMIT_c                               0x0000
#define g_MOTOR_STEP_SIZE_c                                     0x0001
#define g_STATUS_c                                              0x0002
#define g_CLOSED_LIMIT_c                                        0x0010
#define g_MODE_c					                            0x0011

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct Shade_Configuration_Cluster_Attributes_Tag
{
#if ( g_PHYSICAL_CLOSED_LIMIT_ENABLE_d == 1 )
   uint8_t a_physical_closed_limit[2];
#endif                              /* g_PHYSICAL_CLOSED_LIMIT_ENABLE_d */
#if ( g_MOTOR_STEP_SIZE_ENABLE_d == 1 )
   uint8_t motor_step_size;
#endif                              /* g_MOTOR_STEP_SIZE_ENABLE_d */
   uint8_t status;
   uint8_t a_closed_limit[2];
   uint8_t mode;
}__attribute__((__packed__))Shade_Configuration_Cluster_Attributes_t;
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_SHADE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Shade_Configuration_Cluster_Server_Attribute_info ;
#endif                      /* g_SHADE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */

#if ( g_SHADE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Closures_Shade_Configuration_Client_Cluster_Info ;
#endif                      /* g_SHADE_CONFIGURATION_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Shade_Config_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

#endif                                 /*g_SHADE_CONFIGURATION_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

#endif                                 /*_ZCL_CLOSURES_SHADE_CONFIGURATION_H_*/
