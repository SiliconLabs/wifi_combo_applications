/******************************************************************************
* ZLL_CONFIGURATION_H
*******************************************************************************
* Program Description:
*
*
******************************************************************************/

#ifndef ZLL_CONFIGURATION_H
#define ZLL_CONFIGURATION_H

/*****************************************************************************
* Includes
*****************************************************************************/



#include "ZLL_defines.h"


/*****************************************************************************
* Global Constants
*****************************************************************************/

#define RSI_ENABLE 1
#define RSI_DISABLE 0

/* Through preprocessor settings g_APP_PROFILE_ID_c can take two values,
g_ZLL_PROFILE_ID_c - 0xc05e
g_HOME_AUTOMATION_PROFILE_ID_c - 0x0104 note: for classical method of joining
								HA profile is used*/
#ifndef g_APP_PROFILE_ID_c
/* By default profile would be ZLL profile 0xc05e*/
#define g_APP_PROFILE_ID_c								g_ZLL_PROFILE_ID_c
#endif

/* Uncomment following macros to configure particular field */
//#define g_ZLL_CONFIG_GLOBAL_SRC_ENDPOINT_c
//#define g_ZLL_CONFIG_GLOBAL_DEST_ENDPOINT_c
//#define g_ZLL_CONFIGURE_EXTENDED_PANID_c
//#define g_ZLL_CONFIGURE_SHORT_PANID_c
//#define g_ZLL_CONFIGURE_LOGICAL_CHANNEL_c

/* Used to configure maximum no of devices in ZLL network*/
#define g_ZLL_MAX_NO_OF_CHILD_DEVICES_c                     (0x0A)
/* start address */

/* Address range should be in the range 0x0001 to 0xfff7:-
0x0000 is reserved for coordinator,
ZLL initiator will have 0x0001 as its address
0xfff7 to 0xffff is for broadcast */
#define g_ZLL_DEFAULT_NWK_FREE_MIN_ADDRESS_c                (0x0000)
/* Free Max is configurable based on g_ZLL_MAX_NO_OF_CHILD_DEVICES_c */
#define g_ZLL_DEFAULT_NWK_FREE_MAX_ADDRESS_c                (0x000A)

/* nothing but no of endpoints,
supporting 1 workspace for all controlling_device,now total controller device is
other workspace for all lighting_device
*/
#ifdef g_ZLL_LIGHT_DEVICE_c
/* can configure no of devices supported Maximum = 7,Minimum = 1 */
#define g_ZLL_CONFIG_SUB_DEVICES_NUMBER_c                          (1)//(3)//(7)
#endif /*g_ZLL_LIGHT_DEVICE_c*/

#ifdef g_ZLL_LIGHT_CONTROLLER_c
/* can configure no of devices supported ,Maximum = 6,Minimum = 1*/
#define g_ZLL_CONFIG_SUB_DEVICES_NUMBER_c                          (1)
#endif /* g_ZLL_LIGHT_CONTROLLER_c*/



/*  Based on number of sub=devices defined above ,enable that many controller
	devices
	Enable controller device the node is supporting.
	Atleast one Controller device should be enabled.
	If Profile is defined as HA profile the only one sub device is
	supported.*/

#ifdef g_ZLL_LIGHT_CONTROLLER_c

#define g_COLOR_CONTROLLER_DEVICE_c				RSI_DISABLE /* ENABLE or DISABLE*/
#define g_COLOR_SCENE_CONTROLLER_DEVICE_c		RSI_ENABLE /* ENABLE or DISABLE*/
#define g_NON_COLOR_CONTROLLER_DEVICE_c			RSI_DISABLE /* ENABLE or DISABLE*/
#define g_NON_COLOR_SCENE_CONTROLLER_DEVICE_c	RSI_DISABLE /* ENABLE or DISABLE*/
#define g_CONTROL_BRIDGE_DEVICE_c				RSI_DISABLE /* ENABLE or DISABLE*/
#define g_ON_OFF_SENSOR_DEVICE_c				RSI_DISABLE /* ENABLE or DISABLE*/


#endif /*g_ZLL_LIGHT_CONTROLLER_c*/

/* Enable Light device the node is supporting
		 Atleast one light device should be enabled*/
#ifdef g_ZLL_LIGHT_DEVICE_c

#define g_ON_OFF_LIGHT_DEVICE_c					RSI_DISABLE /* ENABLE or DISABLE*/
#define g_ON_OFF_PLUG_IN_UNIT_DEVICE_c			RSI_DISABLE /* ENABLE or DISABLE*/
#define g_DIMMABLE_LIGHT_DEVICE_c				RSI_DISABLE /* ENABLE or DISABLE*/
#define g_DIMMABLE_PLUG_IN_UNIT_DEVICE_c		RSI_DISABLE /* ENABLE or DISABLE*/
#define g_COLOR_LIGHT_DEVICE_c					RSI_ENABLE /* ENABLE or DISABLE*/
#define g_EXTENDED_COLOR_LIGHT_DEVICE_c			RSI_DISABLE /* ENABLE or DISABLE*/
#define g_COLOR_TEMPERATURE_LIGHT_DEVICE_c		RSI_DISABLE /* ENABLE or DISABLE*/


#endif /* g_ZLL_LIGHT_DEVICE_c*/


/* Default values for required defines*/
/* the source endpoint used in this application unless specified by the user
 This is also the endpoint for ZDO requests.*/
#ifndef g_ZLL_CONFIG_GLOBAL_SRC_ENDPOINT_c
  #define g_ZLL_CONFIG_GLOBAL_SRC_ENDPOINT_c                    (0x10)
#endif /*g_CONFIG_GLOBAL_SRC_ENDPOINT_c*/

/* the destination endpoint used when sending commands.*/
#ifndef g_ZLL_CONFIG_GLOBAL_DEST_ENDPOINT_c
  #define g_ZLL_CONFIG_GLOBAL_DEST_ENDPOINT_c                   (0x11)
#endif /*g_CONFIG_GLOBAL_DEST_ENDPOINT_c*/

/* Extended PAN ID is used in choosing the correct network when joining*/
#ifndef g_ZLL_CONFIGURE_EXTENDED_PANID_c
    /* can be configured as zero also ,if it is set to zero target device
    which is going to start up the network has to choose extended pan id,
    Similarly with channel,short panid */
    #define g_ZLL_CONFIGURE_NETWORK_START_EXTENDED_PANID_c {'S','T',' ','M','I','C','R','O'}
#else
    #define g_ZLL_CONFIGURE_NETWORK_START_EXTENDED_PANID_c {0,0,0,0,0,0,0,0}
#endif /* g_ZLL_CONFIGURE_EXTENDED_PANID_c*/


#ifndef g_ZLL_CONFIGURE_SHORT_PANID_c
    #define g_ZLL_CONFIGURE_NETWORK_START_SHORT_PANID_c         (0x1122)
#else
    /* If it is set to zero then target device has to choose short panid*/
    #define g_ZLL_CONFIGURE_NETWORK_START_SHORT_PANID_c         (0x0000)
#endif



#ifndef g_ZLL_CONFIGURE_LOGICAL_CHANNEL_c
    #define g_ZLL_CONFIGURE_NETWORK_START_LOGICAL_CHANNEL_c     (13)
#else

    #define g_ZLL_CONFIGURE_NETWORK_START_LOGICAL_CHANNEL_c     (0)
#endif





/*****************************************************************************
* Public Memory declarations
*****************************************************************************/

/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/
/* None */
/*****************************************************************************
* Public Functions
*****************************************************************************/

/* None */
/*****************************************************************************
* Private Functions
*****************************************************************************/

/* None */

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */



#endif /* ZLL_CONFIGURATION_H */
/*****************************************************************************
* End Of File
*****************************************************************************/
