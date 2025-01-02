/*******************************************************************************
* AppOnOffUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting OnOff
*   Cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "common_includes.h"
#include "stack_common.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "ApplicationThinLayer.h"
#include "ZCL_Interface.h"
#include "zdo_interface.h"
#include "apsde_interface.h"
#include "stack_util.h"
#include "sw_timer.h"
#include "ApptimerId.h"
#if ( g_ON_OFF_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_On_Off_Cluster.h"
#include "AppOnOffUtilities.h"


#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
	#include "AppLevelControlUtilities.h"
#endif
#if(g_SCENES_CLUSTER_ENABLE_d==1)
	#include "AppScenesUtilities.h"

#endif
/*-----------------------------------------------------------------------------
* Global constant declarations
*----------------------------------------------------------------------------*/
#define g_TIMER_COUNT_HUNDRED_MILLISEC_c  	0x186A0
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Level_Control_Cluster.h"
/* on off effect identifier values */
#define ON_OFF_EFFECT_IDENTIFIER_DELAYED_ALL_OFF 0x00
#define ON_OFF_EFFECT_IDENTIFIER_DYING_LIGHT     0x01

/* on off effect variant values (with ON_OFF_EFFECT_IDENTIFIER_DELAYED_ALL_OFF) */
#define ON_OFF_EFFECT_VARIANT_FADE               0x00
#define ON_OFF_EFFECT_VARIANT_NO_FADE            0x01
#define ON_OFF_EFFECT_VARIANT_DIMMING_FADE_12    0x02

/* on off effect variant values (with ON_OFF_EFFECT_IDENTIFIER_DYING_LIGHT) */
#define ON_OFF_EFFECT_VARIANT_DIMMING_FADE_1     0x00

/********************* New OnOff Command: On with Timed off ****************/
#define LC_TICKS_PER_SECOND 4
//1024 ms in a sec, this is ~1/10th of a sec
#define LC_TICK_TIME 102

#define ON_OFF_DEFAULT_TIME_VALUE 0x0000

/* OnOff states */
#define OFF_STATE 0x00
#define ON_STATE  0x01
#define TIMED_ON_STATE 0x02
#define DELAYED_OFF_STATE 0x03
#define TOGGLE_STATE 0x04

#define OFF_EFFECT_STATE 0x05

/* All times are converted to 1/ths of seconds */
#define DIMMING_TIME_0_5_SEC  5
#define DIMMING_TIME_0_8_SEC  8
#define FADE_OFF_TIME_0_8_SEC 8
#define FADE_OFF_TIME_1_SEC   10
#define FADE_OFF_TIME_12_SEC  120

/* On with Timed off: On/Off control field possible values */
#define ACCEPT_ONLY_WHEN_ON 0x01
#define ACCEPT_INCODITIONALLY  0x00

/* OnOffState variable which keeps current OnOff State */


#define LEVEL_MIN_VALUE 0x01
#define LEVEL_MAX_VALUE 0xFE

#define MIN(a,b)   ((a<b)  ? a : b)
#define MAX(a,b)   ((a>=b) ? a : b

// Timer measurements



#endif /*g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d */

uint8_t OnOffState;
uint16_t off_wait_time;
uint8_t on_time;




uint16_t onDuration =0x0000;
uint8_t OnOffState = g_ZERO_c;


#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
extern uint8_t g_Is_On_Off_To_Be_Updated;
#endif

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if(g_HA_1_2_d == 1)
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
static void SendOffCommand(uint8_t timerId );
#endif   /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
#endif	/* g_HA_1_2_d*/
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

/**************************************************************************************/
void App_OnOffEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
    switch ( pZCLEvent->eventId )
    {
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    case g_On_Off_Cluster_Off_Event_c:
		OnOffState = g_ON_c;                 /* State updated as ON */
#if(g_HA_1_2_d == 1)
#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )

	/* In HA 1.2, ON_TRANSITION_ATTRIBUTE is introduced */
	/*When the  On Command is received, OnOffattribute is attributed updated
		to ON state  with OnTransition Attribute time*/

	g_Is_On_Off_To_Be_Updated = g_TRUE_c;

	App_Move_Current_Level_with_OnOff_command(g_Off_Command_c,ZCL_Callback_Get_Device_EndPoint());
#endif  /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d*/
#endif	/* g_HA_1_2_d*/
        break;
    case g_On_Off_Cluster_On_Event_c:
	     OnOffState = g_OFF_c;                 /* State updated as OFF */
#if(g_HA_1_2_d == 1)
#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )

/* In HA 1.2, ON_TRANSITION_ATTRIBUTE is introduced */

	g_Is_On_Off_To_Be_Updated = g_TRUE_c;

	/*When the  On Command is received, OnOffattribute is attributed updated
		to OFF state  with OffTransition Attribute time*/

	App_Move_Current_Level_with_OnOff_command(g_On_Command_c,ZCL_Callback_Get_Device_EndPoint());
#endif        /* g_LEVEL_CONTROL_CLUSTER_ENABLE_d*/
#endif	/* g_HA_1_2_d*/
        break;
	case g_On_Off_Cluster_Toggle_Event_c:
	     OnOffState = ~OnOffState;
	  break;
#if(g_HA_1_2_d == 1)
	case g_On_Off_Cluster_On_With_Duration_Event_c:
		memUtils_memCopy( (uint8_t *)&onDuration, pZCLEvent->pEventData, 0x02 );

		/* OnOffState is updated to ON state till OnDuration */
		OnOffState = g_ON_c;   /* State updated as ON */

		onDuration = onDuration * m_MILLISECONDS_c;

		/* send OFF Command after OnDuration gets completed */

		tmr_startRelativeTimer (m_APPLICATION_ON_OFF_TIMER_c, onDuration ,
																SendOffCommand);

	break;
#endif	 /* g_HA_1_2_d*/

#endif  /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
    default :
        break;
    }
}

#if(g_HA_1_2_d == 1)
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
static void SendOffCommand(uint8_t timerId )
{

	OnOffState = g_OFF_c;   /* State updated as OFF */
	/* stop time once OnDuration gets completed */
	tmr_stopTimer ( timerId	);
	return;
}
#endif /*g_ON_OFF_SERVER_CLUSTER_ENABLE_d */
#endif  /*g_HA_1_2_d */
/**************************************************************************************/

/**************************************************************************************/

/*******************************************************************************************/

void ZLLUtilOnOffHandler(ZCL_Event_t *pEvents)
{
#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
#if (g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t effect_identifier,effect_variant;

    uint8_t  newValue;
    uint8_t currentValue = 0;
    uint16_t on_time;
    uint8_t  on_off_control,sceneTableIndex;
    uint8_t onoffvalue;
    uint16_t ontimevalue;
    uint8_t offwaittimevalue;
    uint16_t group_id = 0x0000;
    uint8_t scene_id = 0x00;
    uint8_t endPointId = ZCL_Callback_Get_Device_EndPoint();
    switch ( pEvents->eventId ){
	/*off with effect */
    case g_Off_With_Effect_Event_c:

        /*effect identifier */
        effect_identifier = *pEvents->pEventData;

		/*effect variant */
        effect_variant = *(pEvents->pEventData + 1 );

		/*check effect identifier */
        if (effect_identifier > 0x01)
        {
            return;
        }

		/*read global scene control attribute value */
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_GLOBAL_SCENE_CONTROL_ATTRIBUTE_c,
                               (uint8_t*)&currentValue,
                               g_Read_Attribute_Data_c );


        /* Check if GLOBAL_SCENE_CONTROL_ATTRIBUTE_ID is g_TRUE_c */
        if (currentValue)
        {
            /* Store its settings in its global scene */
            VerifySceneTableEntry ( group_id, scene_id, &sceneTableIndex );
            Handle_StoreAndRecallSceneCmd ( endPointId,
                                           sceneTableIndex, StoreScene );


            /* Set ZCL_ON_OFF_GLOBAL_SCENE_CONTROL_ATTRIBUTE_ID to g_FALSE_c */
            newValue = 0; // set to g_FALSE_c
            App_ReadWriteAttribute(endPointId ,
                                   g_ON_OFF_CLUSTER_c,
                                   g_GLOBAL_SCENE_CONTROL_ATTRIBUTE_c,
                                   (uint8_t*)&newValue,
                                   g_Write_Attribute_Data_c );


        }

        /* Check effect_variant values */
        if (((effect_identifier == g_ZERO_c) && (effect_variant > 0x02)) ||
            ((effect_identifier == 0x01) && (effect_variant > g_ZERO_c)))
        {
            /* set default effect_variant */
            effect_variant = g_ZERO_c;
        }

        /* Apply off effect based on effect identifier and effect variant values */
        if (effect_identifier == ON_OFF_EFFECT_IDENTIFIER_DELAYED_ALL_OFF)
        {
		    /*fade to off */
            if (effect_variant == ON_OFF_EFFECT_VARIANT_FADE)
            {

                /* fade to off in 0.8 seconds: 8 1/10th sec  */

                App_ZLL_Move_Current_Level (effect_variant, effect_identifier, endPointId);
            }
			 /* no fade */
            else if (effect_variant == ON_OFF_EFFECT_VARIANT_NO_FADE)
            {
                /* no fade */
            }
			/*50 percent dim down */
            else if (effect_variant == ON_OFF_EFFECT_VARIANT_DIMMING_FADE_12)
            {

                App_ZLL_Move_Current_Level (effect_variant, effect_identifier, endPointId);
            }
        }
		  /*20% dim up */
        else  if ((effect_identifier == ON_OFF_EFFECT_IDENTIFIER_DYING_LIGHT) &&
                  (effect_variant == ON_OFF_EFFECT_VARIANT_DIMMING_FADE_1))
        {
            /* 20% dim up in 0.5s then fade to off in 1 sec. */

            App_ZLL_Move_Current_Level (effect_variant, effect_identifier, endPointId);


        }
        break;

		/*off with global recall */
    case g_On_With_Recall_Global_Scene_Event_c:



        /* Set current OnOffState */
        // OnOffState = ON_STATE;

        // read current ZCL_ON_OFF_GLOBAL_SCENE_CONTROL_ATTRIBUTE_ID value

        App_ReadWriteAttribute(endPointId,
                               g_ON_OFF_CLUSTER_c,
                               g_GLOBAL_SCENE_CONTROL_ATTRIBUTE_c,
                               (uint8_t*)&currentValue,
                               g_Read_Attribute_Data_c );


        /* Check if GLOBAL_SCENE_CONTROL_ATTRIBUTE_ID is g_FALSE_c */
        if (currentValue == g_FALSE_c)
        {
            /* Recall its settings in its global scene: this will change the on/off
            attributre value based on global scene value */
            VerifySceneTableEntry ( group_id, scene_id, &sceneTableIndex );
            //Handle_StoreAndRecallSceneCmd ( endPointId,
            //                               sceneTableIndex, RecallScene );  //FIXME BY vinoth
            // read current on/off value
            App_ReadWriteAttribute(endPointId,
                                   g_ON_OFF_CLUSTER_c,
                                   g_ON_OFF_ATTRIBUTE_c,
                                   (uint8_t*)&currentValue,
                                   g_Read_Attribute_Data_c );


            /* enter in its appropriate state based on currentValue and update OnOff attribute accordingly */
            zclUtilOnOffSetValue(currentValue, g_FALSE_c);

            /*      ZCL_ON_OFF_GLOBAL_SCENE_CONTROL_ATTRIBUTE_ID has to be set to g_TRUE_c
            after reception of ZCL recall scene */
			newValue=1 ;
			App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_GLOBAL_SCENE_CONTROL_ATTRIBUTE_c,
                               (uint8_t*)&newValue,
                               g_Write_Attribute_Data_c );
            }
        else
        {
		 /*if global scene control attribute ids true discard the command */
           return ;
        }


        break;

		/*on with timed off */
    case g_On_With_Timed_Off_Event_c:


        /*onoff control field */
        on_off_control = *pEvents->pEventData;

		/*on time field */
        on_time = *(pEvents->pEventData + 1);

		/*off wait time field */
        off_wait_time = *(pEvents->pEventData + 3);

        /* Read ZCL_ON_OFF_ATTRIBUTE_ID value */
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_ON_OFF_ATTRIBUTE_c,
                               (uint8_t*)&onoffvalue,
                               g_Read_Attribute_Data_c );


        if ((on_off_control == ACCEPT_ONLY_WHEN_ON) && (onoffvalue == g_FALSE_c))
        {
		/*discard command */
            return;
        }

        //OnOffState = SET_ON_OFF_TIMING_ATTRIBUTES;
        /* Read the ZCL_ON_OFF_OFF_WAIT_TIME_ATTRIBUTE_ID attribute  */
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_OFF_WAIT_TIME_ATTRIBUTE_c,
                               (uint8_t*)&offwaittimevalue,
                               g_Read_Attribute_Data_c );


        /* Check if OffWaitTime attribute value > 0 and OnOff attribute = 0 */
        if ((offwaittimevalue > g_ZERO_c) && (onoffvalue == g_ZERO_c))
        {
            /* check if the requested off_wait_time is less than the ZCL_ON_OFF_OFF_WAIT_TIME_ATTRIBUTE_ID values */
            if (MIN(off_wait_time, offwaittimevalue) == off_wait_time)
            {
                /*  the OffWaitTime attribute shall be set to off_wait_time*/
                App_ReadWriteAttribute(endPointId ,
                                       g_ON_OFF_CLUSTER_c,
                                       g_OFF_WAIT_TIME_ATTRIBUTE_c,
                                       (uint8_t*)&off_wait_time,
                                       g_Write_Attribute_Data_c );


            }
            else
                /* store the value to be used */
                off_wait_time = offwaittimevalue;

            //OnOffState = DELAYED_OFF_STATE;
        }
        else
        {
            /* read current ZCL_ON_OFF_ON_TIME_ATTRIBUTE_ID value */
            App_ReadWriteAttribute(endPointId,
                                   g_ON_OFF_CLUSTER_c,
                                   g_ON_TIME_ATTRIBUTE_c,
                                   (uint8_t*)&ontimevalue,
                                   g_Read_Attribute_Data_c );


            /* check if the requested on_time is greater than the ZCL_ON_OFF_ON_TIME_ATTRIBUTE_ID values */
            if (MAX(ontimevalue, on_time) == on_time))
{
    /* On reception of the off command the OnTime attribute shall be set to 0x0000 */
    App_ReadWriteAttribute(endPointId,
                           g_ON_OFF_CLUSTER_c,
                           g_ON_TIME_ATTRIBUTE_c,
                           (uint8_t*)&on_time,
                           g_Write_Attribute_Data_c );


}
            else
                /* store the value to be used */
                on_time = ontimevalue;

            /*  the OffWaitTime attribute shall be set to off_wait_time*/
            App_ReadWriteAttribute(endPointId,
                                   g_ON_OFF_CLUSTER_c,
                                   g_OFF_WAIT_TIME_ATTRIBUTE_c,
                                   (uint8_t*)&off_wait_time,
                                   g_Write_Attribute_Data_c );




            /* write the new on/off value */
            onoffvalue = 0x01;
            App_ReadWriteAttribute(endPointId,
                                   g_ON_OFF_CLUSTER_c,
                                   g_ON_OFF_ATTRIBUTE_c,
                                   (uint8_t*)&onoffvalue,
                                   g_Write_Attribute_Data_c );



        }/* end else */

    App_ReadWriteAttribute(endPointId ,
                           g_ON_OFF_CLUSTER_c,
                           g_ON_TIME_ATTRIBUTE_c,
                           (uint8_t*)&on_time,
                           g_Read_Attribute_Data_c );
    /*  the OffWaitTime attribute shall be set to off_wait_time*/
    App_ReadWriteAttribute(endPointId ,
                           g_ON_OFF_CLUSTER_c,
                           g_OFF_WAIT_TIME_ATTRIBUTE_c,
                           (uint8_t*)&off_wait_time,
                           g_Read_Attribute_Data_c );

    if ((off_wait_time < 0xFFFF) && (on_time < 0xFFFF))
    {
        App_ReadWriteAttribute(endPointId,
                               g_ON_OFF_CLUSTER_c,
                               g_OFF_WAIT_TIME_ATTRIBUTE_c,
                               (uint8_t*)&off_wait_time,
                               g_Read_Attribute_Data_c );
        App_ReadWriteAttribute(endPointId,
                               g_ON_OFF_CLUSTER_c,
                               g_ON_TIME_ATTRIBUTE_c,
                               (uint8_t*)&on_time,
                               g_Read_Attribute_Data_c );
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_ON_OFF_ATTRIBUTE_c,
                               (uint8_t*)&onoffvalue,
                               g_Read_Attribute_Data_c );

        if (off_wait_time > g_ZERO_c && onoffvalue == 0x00)
            tmr_startRelativeTimer ( m_APPLICATION_ON_OFF_TIMER_c,
                                    g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                                    App_Off_Wait_Time_Timed_Off_Call_Back );
        else if ( on_time > g_ZERO_c && onoffvalue == 0x01 )
            tmr_startRelativeTimer ( m_APPLICATION_ON_OFF_TIMER_c,
                                    g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                                    App_On_Time_Timed_Off_Call_Back );


    }

    break;


    } /*End of switch*/

#endif /*g_ZLL_ON_OFF_CLUSTER_ENABLE_d*/
#endif /*g_ON_OFF_SERVER_CLUSTER_ENABLE_d */
#endif /*g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d*/
}/* end zclUtilOnOffRxOffWithEffectHandler() */

/***********************************************************************************************/

#if(g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d==1)

void zclUtilOnOffSetValue(uint8_t command, uint8_t initiatedByLevelChange)
{
    uint8_t currentValue = g_ZERO_c;
    uint8_t newValue;

    // read current on/off value
     uint8_t endPointId = ZCL_Callback_Get_Device_EndPoint();
    App_ReadWriteAttribute( endPointId,
                           g_ON_OFF_CLUSTER_c,
                           g_ON_OFF_ATTRIBUTE_c,
                           (uint8_t*)&currentValue,
                           g_Read_Attribute_Data_c );



    // if the value is already what we want to set it to then do nothing
    if ( (currentValue == 0 && (command == g_Off_Command_c)) ||
        (currentValue == 1 && (command == g_On_Command_c ))) {

            return;
        }

    // we either got a toggle, or an on when off, or an off when on,
    // so we need to swap the value
    newValue = !currentValue;


    /* Update state only if the device is on OFF_STATE, ON_STATE */
    if ((OnOffState == OFF_STATE) || (OnOffState == ON_STATE))
        OnOffState = (newValue == g_ZERO_c) ? OFF_STATE : ON_STATE;

        // write the new on/off value
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_ON_OFF_ATTRIBUTE_c,
                               (uint8_t*)&newValue,
                               g_Write_Attribute_Data_c );




#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1)
        // if using level control, the on/off command also adjusts the level
        if (initiatedByLevelChange == g_FALSE_c)
        {

            uint8_t storedLevelValue = LEVEL_MIN_VALUE; //LEVEL_MIN_VALUE

            uint8_t level;
            uint8_t currentLevelValue;
            uint8_t enableLevelWrite = g_FALSE_c;

            // read the current level
            App_ReadWriteAttribute(endPointId ,
                                   g_LEVEL_CONTROL_CLUSTER_c,
                                   g_CURRENT_LEVEL_ATTRIBUTE_c,
                                   (uint8_t*)&currentLevelValue,
                                   g_Read_Attribute_Data_c );

            // store the current setting so it can be restored in storedLevelValue
            storedLevelValue = currentLevelValue;
            /* newvalue is the new on-off cluster value */
            if (newValue == 0) {

                // new level to write is LEVEL_MIN_VALUE
                //level = LEVEL_MIN_VALUE;
                enableLevelWrite = g_TRUE_c;

                //storedLevelValue = currentLevelValue;
                level = storedLevelValue;

                // if with off command storedLevelValue is 0, on next on, storedLevelValue will be 255
                // make sure we arent storing a incorrect value
                if (storedLevelValue == LEVEL_MIN_VALUE) {
                    storedLevelValue = LEVEL_MAX_VALUE;
                }

            }
            /* newvalue is the new on-off cluster value */
            else if (newValue == 1) {
                // return the level back to where it was when the off command was issued
                // if the level isnt already set to nozero - like when we are moving
                // or stepping. Only do this if the level is not zero, otherwise this
                // screws up level, step, and move commands.


                if (storedLevelValue != LEVEL_MIN_VALUE) {
                    level = storedLevelValue;
                    enableLevelWrite = g_TRUE_c;
                }
            }

            // write the level if we are supposed to
            if (g_TRUE_c == enableLevelWrite) {

                // write the level value
                App_ReadWriteAttribute(endPointId ,
                                       g_LEVEL_CONTROL_CLUSTER_c,
                                       g_CURRENT_LEVEL_ATTRIBUTE_c,
                                       (uint8_t*)&level,
                                       g_Write_Attribute_Data_c );

            }

        }
#endif //ZLL_USING_LEVEL_CONTROL_CLUSTER_SERVER


}

/*******************************************************************************************/

void App_Off_Wait_Time_Timed_Off_Call_Back(uint8_t timer_Id){

  	uint8_t endPointId = ZCL_Callback_Get_Device_EndPoint();
    App_ReadWriteAttribute(endPointId ,
                           g_ON_OFF_CLUSTER_c,
                           g_OFF_WAIT_TIME_ATTRIBUTE_c,
                           (uint8_t*)&off_wait_time,
                           g_Read_Attribute_Data_c );
    if ( off_wait_time > g_ZERO_c ){

	/*decrement the value of off wait time attribute */
        off_wait_time = off_wait_time - 1;

	/*update off wait time */
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_OFF_WAIT_TIME_ATTRIBUTE_c,
                               (uint8_t*)&off_wait_time,
                               g_Write_Attribute_Data_c );
        tmr_startRelativeTimer ( m_APPLICATION_ON_OFF_TIMER_c,
                                g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                                App_Off_Wait_Time_Timed_Off_Call_Back );
    }
    else{
        tmr_stopTimer( m_APPLICATION_ON_OFF_TIMER_c);
    }

}

void App_On_Time_Timed_Off_Call_Back(uint8_t timer_Id){
  	uint8_t endPointId = ZCL_Callback_Get_Device_EndPoint();
    uint16_t onoffvalue;
    App_ReadWriteAttribute(endPointId ,
                           g_ON_OFF_CLUSTER_c,
                           g_ON_TIME_ATTRIBUTE_c,
                           (uint8_t*)&on_time,
                           g_Read_Attribute_Data_c );
    if (on_time > g_ZERO_c ){
	/*decrement value of on tome */
        on_time = on_time - 1;
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_ON_TIME_ATTRIBUTE_c,
                               (uint8_t*)&on_time,
                               g_Write_Attribute_Data_c );
        tmr_startRelativeTimer ( m_APPLICATION_ON_OFF_TIMER_c,
                                g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                                App_On_Time_Timed_Off_Call_Back );
    }
    else{
        off_wait_time = g_ZERO_c;
        onoffvalue = g_ZERO_c;
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_OFF_WAIT_TIME_ATTRIBUTE_c,
                               (uint8_t*)&off_wait_time,
                               g_Write_Attribute_Data_c );
        App_ReadWriteAttribute(endPointId ,
                               g_ON_OFF_CLUSTER_c,
                               g_ON_OFF_ATTRIBUTE_c,
                               (uint8_t*)&onoffvalue,
                               g_Write_Attribute_Data_c );

        tmr_stopTimer( m_APPLICATION_ON_OFF_TIMER_c);
    }

}

#endif /*  g_ZLL_ON_OFF_CLUSTER_ENABLE_d         */

#endif /*g_ON_OFF_CLUSTER_ENABLE_d*/



