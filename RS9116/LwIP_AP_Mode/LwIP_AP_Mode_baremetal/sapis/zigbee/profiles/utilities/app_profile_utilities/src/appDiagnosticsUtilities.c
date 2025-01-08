/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "common_includes.h"
#include "stack_common.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "ApplicationThinLayer.h"
#include "AppTimerUtilities.h"
#include "ZCL_Interface.h"
#if( g_DIAGNOSTICS_CLUSTER_ENABLE_d ==1 )

#include "ZCL_HA_Diagnostics_Cluster.h"
#include "appDiagnosticsUtilities.h"

/*-----------------------------------------------------------------------------
* Global constant declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/



/**************************************************************************************/


void App_DiagnosticsEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
#if ( g_DIAGNOSTICS_SERVER_CLUSTER_ENABLE_d == 1 )
	uint16_t attribute_value=0x0000;
	uint16_t attribute_id;
	uint8_t index;

    if(g_Clear_Diagnostics_Attributes_Events_c == pZCLEvent->eventId) {
		Clear_Diagnostic_Attribute_Event_t* ClearDiagnosticAttribute = (Clear_Diagnostic_Attribute_Event_t*)pZCLEvent->pEventData;

		for (index=0;index < ClearDiagnosticAttribute->attribute_count;index++){
			  memUtils_memCopy ((uint8_t*)&attribute_id,
              (uint8_t*)ClearDiagnosticAttribute->list_of_attributes, 0x02 );

			 App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DIAGNOSTICS_CLUSTER_c,
                                attribute_id,
                                (uint8_t*)&attribute_value,
                                g_Write_Attribute_Data_c );
		}
    }
 #endif
}
        /* ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 ) */



/**************************************************************************************/

/*********************************************************************************************/
#endif   /* ( g_DIAGNOSTICS_CLUSTER_ENABLE_d ==1 ) */
/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
