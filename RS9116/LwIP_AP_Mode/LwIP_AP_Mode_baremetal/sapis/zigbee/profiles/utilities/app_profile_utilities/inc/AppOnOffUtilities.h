#ifndef _APPONOFFUTILITIES_H_
#define _APPONOFFUTILITIES_H_
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


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
void App_OnOffEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer );

#endif          /* ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 ) */
#if(g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
void zclUtilScenesRxStoreHandler(uint16_t groupId, uint8_t sceneId, uint8_t send_response);
void App_ZLL_Move_Current_Level (uint8_t effect_variant, uint8_t effect_itentifier, uint8_t ep);
void App_Off_Wait_Time_Timed_Off_Call_Back(uint8_t timer_Id);
void App_On_Time_Timed_Off_Call_Back(uint8_t timer_Id);
void zclUtilOnOffSetValue(uint8_t command, uint8_t initiatedByLevelChange) ;
#endif /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/


/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                      /* _APPIDENTIFYUTILITIES_H_ */
