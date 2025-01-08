/******************************************************************************
* ZCL_General_Basic_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Basic cluster attribute and cmmnd information
******************************************************************************/

#ifndef _ZCL_GENERAL_BASIC_CLUSTER_H_
#define _ZCL_GENERAL_BASIC_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define g_BASIC_CLUSTER_c                                      0x0000

/* Basic Cluster Attributes */
#define g_ZCL_VERSION_ATTRIBUTE_c                              0x0000
#define g_APPLICATION_VERSION_ATTRIBUTE_c                      0x0001
#define g_STACK_VERSION_ATTRIBUTE_c                            0x0002
#define g_HW_VERSION_ATTRIBUTE_c                               0x0003
#define g_MANUFACTURE_NAME_ATTRIBUTE_c                         0x0004
#define g_MODEL_IDENTIFIER_ATTRIBUTE_c                         0x0005
#define g_DATE_CODE_ATTRIBUTE_c                                0x0006
#define g_POWER_SOURCE_ATTRIBUTE_c                             0x0007
#define g_LOCATION_DESCRIPTION_ATTRIBUTE_c                     0x0010
#define g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_c                     0x0011
#define g_DEVICE_ENABLED_ATTRIBUTE_c                           0x0012
#define g_ALARM_MASK_ATTRIBUTE_c                               0x0013
#define g_DISABLE_LOCAL_CONFIG_ATTRIBUTE_c                     0x0014
#define g_SW_BUILD_ID_ATTRIBUTE_c  							   0x4000 

enum
{
   g_Reset_To_Factory_Defaults_Events_c = 0x03
};

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_BASIC_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Basic_Server_Cluster_Info;
#endif

#if ( g_BASIC_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Basic_Client_Cluster_Info;
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
/* enumerations for commands specific for general clusters */
typedef enum Basic_Cluster
{
   g_Reset_To_Factory_Defaults_Command_c
}Basic_Cluster;

/* Structure to send basic commands */
typedef struct General_Domain_Basic_Request
{
   uint8_t command_type;
   uint8_t manufacturer_specific;
   uint8_t a_manufacturer_code[2];
   uint8_t disable_default_response;
   uint8_t command_identifier;
}__attribute__((__packed__))General_Domain_Basic_Request;


/*-----------------------------------------------------------------------------
Basic Cluster structure - attributes
Field: zcl_version - specifies the version number of the ZigBee Cluster Library
          that all clusters on this endpoint conform to.
Field: application_version - specifies the version number of the application
          software contained in the device.
Filed: stack_version - specifies the version number of the implementation of
          the ZigBee stack contained in the device.
Field: hw_version - specifies the version number of the hardware of the device.
Field: power_source - specifies the source(s) of power available to the device
Field: physical_environment -specifies the type of physical environment in
         which the device will operate
Field: device_enabled - specifies whether the device is enabled or disabled
Field: Alarm_mask - specifies which of a number of general alarms may be
         generated.
Field: manufacture_name - The ManufacturerName attribute is a maximum of 32
         bytes in length and specifies the name of the manufacturer as a ZigBee
         character string.
Field: model_identifier - The ModelIdentifier attribute is a maximum of 32
         bytes in length and specifies the model number (or other identifier)
         assigned by the manufacturer as a ZigBee character string.
Filed: date_code - The DateCode attribute is a ZigBee character string with a
         maximum length of 16 bytes
Field: location_description - The LocationDescription attribute is a maximum
         of 16 bytes in length and describes the physical location of the
         device as a ZigBee character string. This location description may
         be added into the device during commissioning.

-----------------------------------------------------------------------------*/
typedef struct Basic_Cluster_Tag
{
   uint8_t zcl_version;

#if ( g_APPLICATION_VERSION_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t application_version;
#endif
#if ( g_STACK_VERSION_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t stack_version;
#endif
#if ( g_HW_VERSION_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t hw_version;
#endif
#if ( g_MANUFACTURE_NAME_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_manufacture_name[16];
#endif
#if ( g_MODEL_IDENTIFIER_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_model_identifier[16];
#endif
#if ( g_DATE_CODE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_date_code[16];
#endif

   uint8_t power_source;

#if ( g_LOCATION_DESCRIPTION_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_location_description[16];
#endif
#if ( g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t physical_environment;
#endif
#if ( g_DEVICE_ENABLED_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t device_enabled;
#endif
#if ( g_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t Alarm_mask;
#endif
#if ( g_DISABLE_LOCAL_CONFIG_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t DisableLocalConfig;
#endif
#if ( g_SW_BUILD_ID_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t SW_build_Id[16];   
#endif
} __attribute__((__packed__))Basic_Cluster_t;

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* ZCL_Handle_Basic_Cluster_Cmd
*------------------------------------------------------------------------------
*
* Return Value : Length
*		
* Input Parameters:
*	endPointId - Device End point
*   asduLength - Length of received asdu
*   pReceivedAsdu - Pointer to received asdu
*   pZCLClusterInfo - Pointer to Level control cluster info
*   pAttrsDataBaseLocation - Pointer to Basic cluster attributes storage
*
* Output Parameters:
*	pResponseAsduStartLocation - Pointer for response creation
*   pActionHandler - Pointer for Event Updation
*
* Scope of the function: This function is called from out side the file
*
* Purpose of the function:
*	This function handles the Basic cluster command received in the indication
*     	
* Before the function is called:
*   None
* After the function is called:
*   None
*
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Basic_Cluster_Cmd
(
     uint8_t endPointId,
     uint8_t asduLength,
     uint8_t* pReceivedAsdu,
     uint8_t* pResponseAsduStartLocation,
     ZCL_ActionHandler_t *pActionHandler,
     ZCL_Cluster_Info_t const *pZCLClusterInfo,
     uint8_t * pAttrsDataBaseLocation
);

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                      /* g_BASIC_CLUSTER_ENABLE_d == 1 */

#endif                                      /*_ZCL_GENERAL_BASIC_CLUSTER_H_*/
