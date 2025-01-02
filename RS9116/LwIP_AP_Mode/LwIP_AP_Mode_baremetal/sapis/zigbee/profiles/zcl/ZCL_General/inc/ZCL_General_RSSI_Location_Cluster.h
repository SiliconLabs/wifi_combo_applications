#ifndef _ZCL_GENERAL_RSSI_LOCATION_CLUSTER_H_
#define _ZCL_GENERAL_RSSI_LOCATION_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*Attributes and commands that provide a means for
exchanging location information and channel
parameters among devices*/
#define g_RSSI_LOCATION_CLUSTER_c                              0x000b

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef enum RSSI_Location
{
   g_Set_Absolute_Location_Command_c,
   g_Set_Device_Configuration_Command_c,
   g_Get_Device_Configuration_Command_c,
   g_Get_Location_Data_Command_c
}RSSI_Location;


/*-----------------------------------------------------------------------------
* Function Prototypes
-------------------------------------------------------------------------------

**** None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_GENERAL_RSSI_LOCATION_CLUSTER_H_*/