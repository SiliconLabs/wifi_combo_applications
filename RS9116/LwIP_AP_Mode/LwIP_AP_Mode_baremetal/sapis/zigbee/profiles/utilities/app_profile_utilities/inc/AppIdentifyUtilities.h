#ifndef _APPIDENTIFYUTILITIES_H_
#define _APPIDENTIFYUTILITIES_H_
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define IDENTIFY_EFFECT_IDENTIFIER_BLINK          0x00
#define IDENTIFY_EFFECT_IDENTIFIER_BREATHE        0x01
#define IDENTIFY_EFFECT_IDENTIFIER_OKAY           0x02
#define IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE 0x0b
#define IDENTIFY_EFFECT_IDENTIFIER_FINISH_EFFECT  0xfe
#define IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT    0xff

#define IDENTIFY_EFFECT_IDENTIFIER_NONE           0x1f // TBR ???

// what effect variant are we in
#define IDENTIFY_EFFECT_VARIANT_DEFAULT           0x00
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
void App_IdentifyTask( void );
#endif   /*  ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )  */

#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
void App_IdentifyEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer );
#endif          /* ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 ) */

void ZCL_Callback_Identifier_Trigger_Effect(ZCL_Event_t *pEvents);
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                      /* _APPIDENTIFYUTILITIES_H_ */
