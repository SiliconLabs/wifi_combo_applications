/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include"ZigBeeDataTypes.h"
#include "common_includes.h"
#include "stack_common.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "ApplicationThinLayer.h"
#include "AppTimerUtilities.h"
#include "ApptimerId.h"
#include "sw_timer.h"
#if( g_IDENTIFY_CLUSTER_ENABLE_d ==1 )
#include "AppIdentifyUtilities.h"
#include "ZCL_General_Identify_Cluster.h"

/*-----------------------------------------------------------------------------
* Global constant declarations
*----------------------------------------------------------------------------*/
#define LEVEL_MIN_VALUE 0x01
#define LEVEL_MAX_VALUE 0xFE
#define m_ONE_TICK_c                                         0x01

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
static uint32_t lastTimerTick = 0x00;
//extern Identify_Cluster_t identify_server_cluster_values;
Identify_Cluster_t identify_server_cluster_values;
#endif          /* ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 ) */



/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if ( g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
 void App_ZLLIdentifyCallback();

#endif /* #if ( g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )*/
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )

void App_IdentifyTask( void )
{
    if ( App_getDifferance( App_getcurrentTick(), lastTimerTick ) >= g_ONE_TICK_c ) {
        lastTimerTick = App_getcurrentTick();
        if( *((uint16_t*)identify_server_cluster_values.a_identify_time) > 0x0000 ) {
            (*((uint16_t*)identify_server_cluster_values.a_identify_time))--;
        }
    }
}

/**************************************************************************************/

uint8_t ZCL_Callback_Is_Device_In_Identifing(void)
{
    return *((uint16_t*)identify_server_cluster_values.a_identify_time) ?
            g_TRUE_c : g_FALSE_c;
}
#endif /* ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 ) */



/**************************************************************************************/
void App_IdentifyEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
    switch ( pZCLEvent->eventId )
    {
#if ( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
    case g_Identify_Event_c:
        memUtils_memCopy( identify_server_cluster_values.a_identify_time, pZCLEvent->pEventData, 0x02 );
        break;
    case g_Identify_Query_Event_c:
        break;
#endif
    default :
        break;
    }
}

/*********************************************************************************************/

#if ( g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )

void ZCL_Callback_Identifier_Trigger_Effect(ZCL_Event_t *pEvents)
{

  uint16_t identify_effect_identifier;
  uint8_t effect_identifier = *pEvents->pEventData;
  uint8_t effect_variant    = *(pEvents->pEventData + 1);

  /* Check if the effect_variant is supported by the device */
  if(effect_variant > g_ZERO_c)
  {
    /* set default effect_variant */
    effect_variant = g_ZERO_c;
  }

  /* store effect identifier */
  identify_effect_identifier = effect_identifier;


     switch (identify_effect_identifier)
      {
		/*Blink effect */
        case IDENTIFY_EFFECT_IDENTIFIER_BLINK:
#if (g_CLI_SUPPORT_d == 1 )
		   printf("Blink effect command recieved\n") ;
#endif /*(g_CLI_SUPPORT_d == 1)		   */
      //  STM_EVAL_LEDOn( LED4 );

	tmr_startRelativeTimer ( m_APPLICATION_ON_OFF_TIMER_c,
		 60* m_MILLISECONDS_c *m_MILLISECONDS_c,(Timer_Callback_t)App_ZLLIdentifyCallback );


        break;

        /*Breath effect  */
        case IDENTIFY_EFFECT_IDENTIFIER_BREATHE:
#if (g_CLI_SUPPORT_d == 1 )
          printf("Breath effect command recived\n");
#endif /*(g_CLI_SUPPORT_d == 1)		   */
        break;

        /*Okay effect */
        case IDENTIFY_EFFECT_IDENTIFIER_OKAY:
#if (g_CLI_SUPPORT_d == 1 )
		printf("Okay eefect command recived\n");
#endif /*(g_CLI_SUPPORT_d == 1)		   */

        break;

		/*channel change effect */
        case IDENTIFY_EFFECT_IDENTIFIER_CHANNEL_CHANGE:
#if (g_CLI_SUPPORT_d == 1 )
		printf("Channel change effect coomand recived\n");
#endif /*(g_CLI_SUPPORT_d == 1)		   */
        break;

		/*Finish effect */
        case IDENTIFY_EFFECT_IDENTIFIER_FINISH_EFFECT:
          /* Finish effect */
#if (g_CLI_SUPPORT_d == 1 )          /* effect is completed */
          printf("Finish effect command recived");
#endif /*(g_CLI_SUPPORT_d == 1)		   */
        break;

		/*Stop effect */
        case IDENTIFY_EFFECT_IDENTIFIER_STOP_EFFECT:
          /* Stop effect */
 #if (g_CLI_SUPPORT_d == 1 )         /* effect is completed */
          printf("Stop effect command recived");
#endif /*(g_CLI_SUPPORT_d == 1)		   */
		  /* Stop timer */
		 // if(tmr_isTimerRunning){
		//	STM_EVAL_LEDOff( LED4 );
        //	tmr_stopTimer( m_APPLICATION_ON_OFF_TIMER_c	);
		//  }

        break;

        default:

        break;
     }
  /**************************************************/

}/* end zclUtilIdentifyTriggerEffectHandler() */

/******************************************************************************/
void App_ZLLIdentifyCallback()
{
	//STM_EVAL_LEDOff( LED4 );
}


#endif /*g_ZLL_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
/******************************************************************************/
#endif   /* ( g_IDENTIFY_CLUSTER_ENABLE_d ==1 ) */
/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/


/* None */


/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
