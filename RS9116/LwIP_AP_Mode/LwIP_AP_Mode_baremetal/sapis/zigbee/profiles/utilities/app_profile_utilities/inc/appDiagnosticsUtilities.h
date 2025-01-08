#ifndef _APPDIAGNOSTICSUTILITIES_H_
#define _APPDIAGNOSTICSUTILITIES_H_
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
/* Format of the clear diagnostic attribute Command Payload */
typedef struct Clear_Diagnostic_Attribute_Event_Tag
{
   uint8_t attribute_count;
   uint16_t list_of_attributes[16];
}__attribute__((__packed__))Clear_Diagnostic_Attribute_Event_t;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
void App_DiagnosticsEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer );
#endif          /* ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                      /* _APPIDENTIFYUTILITIES_H_ */
