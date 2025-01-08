/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include"ZigBeeDataTypes.h"
#include "common_includes.h"
#include "ZCL_Functionality.h"
#include "AppLevelControlUtilities.h"
#if ( g_LEVEL_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Level_Control_Cluster.h"
#endif
#if ( g_ON_OFF_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_On_Off_Cluster.h"
#endif

/*-----------------------------------------------------------------------------
* Global constant declarations
*----------------------------------------------------------------------------*/
#define m_MIN_VALUE_c                                           0x0000
#define m_MAXIMUM_VALUE_c                                       0xFFFF
#define m_MAX_VALUE_c                                           0xFF
#define m_MINIMUM_VALUE_c                                       0x0000
#define m_CHANGE_UP_BY_ONE_c                                    1
#define m_CHANGE_DOWN_BY_ONE_c                                  -1
#define m_IGNORE_DATA_c                                         0xFF
#define m_TRANSITION_RATE_c                                     0x0A
#define m_SERVER_ATTRIBUTES_c                                   0x00
#define m_TRANSITION_TIME_LENGTH_c                              0x02
#define g_TIMER_COUNT_HUNDRED_MILLISEC_c						0x186A0   

#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
/* All times are converted to 1/ths of seconds */
#define DIMMING_TIME_0_5_SEC  5
#define DIMMING_TIME_0_8_SEC  8
#define FADE_OFF_TIME_0_8_SEC 8
#define FADE_OFF_TIME_1_SEC   10
#define FADE_OFF_TIME_12_SEC  120


#define LEVEL_MIN_VALUE 0x01
#define LEVEL_MAX_VALUE 0xFE

#endif /*g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t g_Move_Command_Mode;
uint8_t g_Move_Command_Rate;
uint8_t g_Current_Level_Endpoint;
int8_t g_Current_Level_Change;
uint16_t g_Transition_Time;
uint8_t g_Is_On_Off_To_Be_Updated;
uint8_t g_Current_Level_Final_Value;
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
uint8_t m_timer_count;
uint8_t g_On_Off_trans_time;
uint8_t m_curr_level;
#endif /*g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

static void App_Write_Current_Level ( uint8_t level );

static void App_Move_Current_Level ( uint8_t *p_events );

static void App_Store_Current_Level_End_Values_And_Start_Timer( uint8_t event,
                                                         uint8_t event_value );

static void App_Read_Write_Level_Control_Attributes (  uint8_t read_or_write,
                                                   uint8_t *attrib_value,
                                                   uint8_t attrib_id );

static void App_Update_Reportable_Attribute ( uint8_t *attrib_value,
                                              uint16_t attrib_id,
                                              uint16_t cluster );

static void App_Level_Control_Move_Command_Call_Back ( uint8_t timerId );

static void App_Level_Control_Move_To_Level_Command_Call_Back ( uint8_t timerId );
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

void Handle_Level_Control_Cluster_Events( ZCL_Event_t * pEvent, uint8_t endpoint )
{
    uint16_t attribute_id;
    uint8_t current_level_attrib_value;
    uint8_t a_tran_time[2] = { 0xFF, 0xFF };

    switch ( pEvent->eventId )
    {
        case g_Move_To_Level_With_On_Off_Event_c :
            g_Is_On_Off_To_Be_Updated = g_TRUE_c;

        case g_Move_To_Level_Event_c:
             if( pEvent->eventId == g_Move_To_Level_Event_c ) {
                g_Is_On_Off_To_Be_Updated = g_FALSE_c;
             }
              g_Current_Level_Endpoint = endpoint;
              App_Move_Current_Level ( pEvent->pEventData );
              break;

        case g_Move_With_On_Off_Event_c :
              g_Is_On_Off_To_Be_Updated = g_TRUE_c;

        case g_Move_Event_c :
              if(  pEvent->eventId == g_Move_Event_c ) {
                 g_Is_On_Off_To_Be_Updated = g_FALSE_c;
              }
              g_Current_Level_Endpoint = endpoint;
              g_Move_Command_Mode =  pEvent->pEventData[0];
              g_Move_Command_Rate =  pEvent->pEventData[1];
#if(g_HA_1_2_d == 1)
			   /* rate  is considered in 1/10th of second */
		      g_Move_Command_Rate = g_Move_Command_Rate / m_TRANSITION_RATE_c;
#endif
              /* if current level is not maxm and there is no
              stop event */
              if( ( g_Move_Command_Rate != 0xFF ) ) {
                  tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                         g_TIMER_COUNT_ONE_SECOND_c, App_Level_Control_Move_Command_Call_Back );
              }
              else  {
                 /* valid_cluster_id is reused to write the current level */
                 if ( g_Move_Command_Mode == g_Move_Up_c ) {
                    current_level_attrib_value = 0xFF;
                 }
                 else {
                    current_level_attrib_value = 0x00;
                 }
                 App_Write_Current_Level ( current_level_attrib_value );
              }
              break;
        case g_Step_With_On_Off_Event_c:
            g_Is_On_Off_To_Be_Updated = g_TRUE_c;

        case g_Step_Event_c :
            if( pEvent->eventId == g_Step_Event_c ) {
                g_Is_On_Off_To_Be_Updated = g_FALSE_c;
            }

            g_Current_Level_Endpoint = endpoint;
            memUtils_memCopy ((uint8_t *)&g_Transition_Time,
			(uint8_t *)(( Level_Control_Step_Command_t *)pEvent->pEventData)->a_transition_time,
                 m_TRANSITION_TIME_LENGTH_c );

            g_Current_Level_Change = m_CHANGE_UP_BY_ONE_c;

            if( (( Level_Control_Step_Command_t *)pEvent->pEventData)->step_mode == g_Move_Down_c ) {
                g_Current_Level_Change = m_CHANGE_DOWN_BY_ONE_c;
            }
            current_level_attrib_value = (( Level_Control_Step_Command_t *)pEvent->pEventData)->step_size;

            App_Store_Current_Level_End_Values_And_Start_Timer ( g_Step_Event_c,
                (( Level_Control_Step_Command_t *)pEvent->pEventData)->step_size );
            break;

        case g_Stop_Event_c :
        case g_Stop_With_On_Off_Event_c :
            /* a_tran_time is reused to set attribute - remaining time to zero */
            attribute_id = g_LEVEL_REMAINING_TIME_ATTRIBUTE_c;
            memUtils_memSet( a_tran_time, g_ZERO_c, m_TRANSITION_TIME_LENGTH_c );
            App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                a_tran_time,  attribute_id );

            tmr_stopTimer( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c	);
            break;
        default :
            break;
    }
}

/***********************************************************************************************/
static void App_Write_Current_Level ( uint8_t level )
{
   uint8_t attribute_id = g_CURRENT_LEVEL_ATTRIBUTE_c;
   /* write the current level value. The below function will take care of
   reporting of current level attribute also */
   App_Update_Reportable_Attribute ( &level, attribute_id, g_LEVEL_CONTROL_CLUSTER_c );
   /* update the on-off attribute value if to be updated */
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
   if ( g_Is_On_Off_To_Be_Updated == g_TRUE_c ) {
         uint8_t on_off_value;
      if ( level == m_MIN_VALUE_c ) {
         on_off_value = g_OFF_c;
      }
      else if ( level == m_MAX_VALUE_c ) {
         on_off_value = g_ON_c;
      }
      else {
         /* on off attribute will be updated when current level is maximum or
         minimum */
         return;
      }
      /* write on off attribute value. The below function will take care of
        reporting of current level attribute also */
      App_Update_Reportable_Attribute ( &on_off_value, g_ON_OFF_ATTRIBUTE_c, g_ON_OFF_CLUSTER_c );
      App_Update_Reportable_Attribute ( &level, g_ON_LEVEL_ATTRIBUTE_c, g_LEVEL_CONTROL_CLUSTER_c );
   }
#endif
}

/***********************************************************************************************/
void App_Move_Current_Level ( uint8_t *p_events )
{
    uint8_t a_tran_time[2];
    uint8_t current_level_attrib_value;
    uint16_t attribute_id;

    memUtils_memCopy ( a_tran_time, ((Level_Control_Move_To_Level_Command *)p_events )->a_transition_time,
               m_TRANSITION_TIME_LENGTH_c );
    current_level_attrib_value = ((Level_Control_Move_To_Level_Command *)p_events)->level;

    g_Current_Level_Change = m_CHANGE_UP_BY_ONE_c;
    /* if transition time is 0xffff, transition time will
    the value of attribute  g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_c */
    attribute_id = g_ON_OFF_TRANSITION_TIME_ATTRIBUTE_c;
    if( ( a_tran_time[0] & a_tran_time[1]) == m_IGNORE_DATA_c ) {
        App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
                                                 a_tran_time, attribute_id  );
    }
    if ( ( ( a_tran_time[0] | a_tran_time[1] )  == g_RESET_c ) ) {
        a_tran_time[0] = m_IGNORE_DATA_c;
        a_tran_time[1] = m_IGNORE_DATA_c;
    }
    memUtils_memCopy ( (uint8_t*)&g_Transition_Time, a_tran_time, sizeof(uint16_t) );

    App_Store_Current_Level_End_Values_And_Start_Timer( g_NULL_c, current_level_attrib_value );

}
/***********************************************************************************************/
#if(g_HA_1_2_d == 1)
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )

void App_Move_Current_Level_with_OnOff_command ( uint8_t events, uint8_t endpoint  )
{
	uint8_t a_tran_time[2];
	uint16_t attribute_id = 0;
	 uint8_t current_level_attrib_value = 0;

	 g_Current_Level_Endpoint = endpoint;

	 if( events == g_On_Command_c ) {

		attribute_id = g_ON_TRANSITION_TIME_ATTRIBUTE_c;
		App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
													 a_tran_time, attribute_id  );
		current_level_attrib_value = m_MAX_VALUE_c;
	 }
	 else if( events == g_Off_Command_c ) {
	
		attribute_id = g_OFF_TRANSITION_TIME_ATTRIBUTE_c;
		App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
													 a_tran_time, attribute_id  );
		current_level_attrib_value = m_MIN_VALUE_c;
	 }

	  if( ( a_tran_time[0] & a_tran_time[1]) == m_IGNORE_DATA_c ) {
        App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
                                                 a_tran_time, attribute_id  );
    }
    if ( ( ( a_tran_time[0] | a_tran_time[1] )  == g_RESET_c ) ) {
        a_tran_time[0] = m_IGNORE_DATA_c;
        a_tran_time[1] = m_IGNORE_DATA_c;
    }

	memUtils_memCopy ( (uint8_t*)&g_Transition_Time, a_tran_time, sizeof(uint16_t) );
	App_Store_Current_Level_End_Values_And_Start_Timer( g_NULL_c, current_level_attrib_value );
}
#endif
#endif
/***********************************************************************************************/

static void App_Store_Current_Level_End_Values_And_Start_Timer ( uint8_t event,
                                                                 uint8_t event_value )
{
    uint8_t level_difference;
    uint8_t a_trans_time[2];
    uint16_t remaining_time;

    /* read the present value of current level attribute */
    App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
                                &g_Current_Level_Final_Value, g_CURRENT_LEVEL_ATTRIBUTE_c );

	/* if the event is step command , then step by step increment the level,
	step value  is mentioned in the event */

    if( ( event == g_Step_Event_c ) ||( event == g_Step_With_On_Off_Event_c ) ) {
        if( g_Current_Level_Final_Value + ( g_Current_Level_Change * event_value ) > m_MAX_VALUE_c ) {
             /* current level value should not go above maximum */
             g_Current_Level_Final_Value = m_MAX_VALUE_c;
        }
        else if(g_Current_Level_Final_Value + ( g_Current_Level_Change * event_value ) < m_MIN_VALUE_c ) {
             /* current level value should not go melow minimum */
             g_Current_Level_Final_Value = m_MIN_VALUE_c;
        }
        else {
             /* current level should be updated by size in step command
             in up or down direction */
             g_Current_Level_Final_Value = g_Current_Level_Final_Value +
                 ( g_Current_Level_Change * event_value );
        }
    }
    else
    {

        if( g_Current_Level_Final_Value > event_value ) {
         /* current level value should be reduced by level_difference*/
         level_difference = g_Current_Level_Final_Value - event_value;
         g_Current_Level_Change = m_CHANGE_DOWN_BY_ONE_c;
        }
        else {
         /* current level value should be increased by level_difference*/
         level_difference = event_value - g_Current_Level_Final_Value ;
         g_Current_Level_Change = m_CHANGE_UP_BY_ONE_c;

        }
        /* After transition time, current level will be the following */
        g_Current_Level_Final_Value = event_value;

        /* current level value has to be increased or reduced by event_value */
        event_value = level_difference;
    }
    if( g_Transition_Time < m_MAXIMUM_VALUE_c ) {
        /* current value will be updated in transition time, so initially
        remaining time will be transition time */
        remaining_time = g_Transition_Time;

        /* transition time is considered in 1/10th of second */
        g_Transition_Time = g_Transition_Time / m_TRANSITION_RATE_c;

        /* If transition time is less than 1 second update the value
        immediately */
        if( g_Transition_Time == m_MIN_VALUE_c ) {
            g_Transition_Time = m_MAXIMUM_VALUE_c;
            remaining_time = g_RESET_c;
        }
        else{
             /* g_Current_Level_Change indicates the change in current level in
            one second */
            g_Current_Level_Change = g_Current_Level_Change *( event_value / g_Transition_Time );
        }
        memUtils_memCopy ( a_trans_time, (uint8_t *)&remaining_time, m_TRANSITION_TIME_LENGTH_c );
        /* write remaining time */
        App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                                        a_trans_time, g_LEVEL_REMAINING_TIME_ATTRIBUTE_c );
    }
    if( g_Transition_Time == m_MAXIMUM_VALUE_c ) {
        /* write current level immediatly */
        App_Write_Current_Level ( g_Current_Level_Final_Value );
    }
    else{
	tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
						g_TIMER_COUNT_ONE_SECOND_c  ,
						App_Level_Control_Move_To_Level_Command_Call_Back );

    }
}

/***********************************************************************************************/
static void App_Read_Write_Level_Control_Attributes (  uint8_t read_or_write,
                       uint8_t *attrib_value,  uint8_t attrib_id  )
{
   App_Cluster_Def_t *p_cluster_definition;

   ZCL_Read_Write_Attribute_Data( read_or_write,
        App_Get_AttrDataLocation ( m_SERVER_ATTRIBUTES_c, g_Current_Level_Endpoint,
                                   g_LEVEL_CONTROL_CLUSTER_c, &p_cluster_definition ),
        p_cluster_definition->p_cluster_info,
        attrib_id,
        attrib_value );
}

/***********************************************************************************************/

static void App_Update_Reportable_Attribute ( uint8_t *attrib_value, uint16_t attrib_id,
                                              uint16_t cluster )
{
    App_Cluster_Def_t *p_cluster_definition;

    ZCL_Read_Write_Attribute_Data( g_Write_Attribute_Data_c,
         App_Get_AttrDataLocation ( m_SERVER_ATTRIBUTES_c, g_Current_Level_Endpoint,
                                    cluster, &p_cluster_definition ),
         p_cluster_definition->p_cluster_info,
         attrib_id,
         attrib_value );
}

/***********************************************************************************************/
static void App_Level_Control_Move_To_Level_Command_Call_Back ( uint8_t timerId )
{
    uint16_t attribute_id = g_CURRENT_LEVEL_ATTRIBUTE_c;
    uint8_t current_level_value;
    uint16_t remaining_time;
    uint8_t a_remaining_time[2];

    /*  read the current level value and update it and write it back */
    App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
                                          &current_level_value, attribute_id );
    /*  current value should not go beyond maximum and minimum */
    if ( current_level_value + g_Current_Level_Change >  m_MAX_VALUE_c ) {
        current_level_value = m_MAX_VALUE_c;
    }
    else if ( current_level_value + g_Current_Level_Change <  m_MIN_VALUE_c ) {
        current_level_value = m_MIN_VALUE_c;
    }
    else {
        current_level_value = current_level_value + g_Current_Level_Change;
    }
    /*  update the current level attribute value */
    App_Write_Current_Level ( current_level_value );

    attribute_id = g_LEVEL_REMAINING_TIME_ATTRIBUTE_c;
    /*  Read the remaining time attribute value */
    App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
        a_remaining_time, attribute_id );

    memUtils_memCopy ( (uint8_t *)&remaining_time, (uint8_t *)a_remaining_time, 0x02 );
    /* update remaining time */
    if ( ( remaining_time / m_TRANSITION_RATE_c)  ==  m_MIN_VALUE_c  ) {
        remaining_time = 0x00;
    }
    else {
       remaining_time = remaining_time - m_TRANSITION_RATE_c;
    }
    memUtils_memCopy (  a_remaining_time, (uint8_t *)&remaining_time, 0x02 );

    attribute_id = g_LEVEL_REMAINING_TIME_ATTRIBUTE_c;

    App_Read_Write_Level_Control_Attributes
    (
        g_Write_Attribute_Data_c,
        a_remaining_time,
        attribute_id
    );
    /* start timer if remaining time is non-zero */
    if( remaining_time != 0x00 ) {
      tmr_startRelativeTimer (  g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                              g_TIMER_COUNT_ONE_SECOND_c,
                              App_Level_Control_Move_To_Level_Command_Call_Back );
    }
    else {
          App_Write_Current_Level ( g_Current_Level_Final_Value );
    }
}

/***********************************************************************************************/
static void App_Level_Control_Move_Command_Call_Back ( uint8_t timerId )
{

   uint8_t current_level;
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
   uint8_t on_off_value;
#endif
   App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
                                            &current_level, g_CURRENT_LEVEL_ATTRIBUTE_c );

   if( g_Move_Command_Mode == g_Move_Up_c ) {
      /* Rate is not 0xFF and the current level is less than 0xFF */
      if(  ( current_level + g_Move_Command_Rate) < 0xFF ) {
         /* increment the current level */
         current_level += g_Move_Command_Rate;
      }
      else {
         /* set the current level to maximum if the current level has reached
         the thershold */
         current_level = 0xFF;
      }
   }
   else {
      /*command rate is not 0xFF, current level is not 0 and the current level
      is greater than rate, decrement the current level*/
      if(  ( current_level >= g_Move_Command_Rate ) ) {
         /*decrement the current level*/
         current_level -= g_Move_Command_Rate;

      }
      else {
         /*reset the current level to zero*/
         current_level = m_MIN_VALUE_c;
      }
   }
   if( ( current_level == m_MIN_VALUE_c ) && ( g_Is_On_Off_To_Be_Updated == g_TRUE_c ) )  {
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
        on_off_value = g_OFF_c;
        App_Update_Reportable_Attribute ( &on_off_value, g_ON_OFF_ATTRIBUTE_c, g_ON_OFF_CLUSTER_c );
		App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                                            &current_level, g_ON_LEVEL_ATTRIBUTE_c );
#endif
   }
   else if ( ( current_level == m_MAX_VALUE_c ) && ( g_Is_On_Off_To_Be_Updated == g_TRUE_c ) ) {
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
       on_off_value = g_ON_c;
       App_Update_Reportable_Attribute ( &on_off_value, g_ON_OFF_ATTRIBUTE_c, g_ON_OFF_CLUSTER_c );
	   App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                                            &current_level, g_ON_LEVEL_ATTRIBUTE_c );
#endif
   }
   App_Update_Reportable_Attribute ( &current_level, g_CURRENT_LEVEL_ATTRIBUTE_c,
                                     g_LEVEL_CONTROL_CLUSTER_c );

   /*if current level reached min threshold or max threshold stop the timer*/
   if( ( current_level == m_MIN_VALUE_c ) || ( current_level == m_MAX_VALUE_c ) ) {
         tmr_stopTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c );
   }
   else {
      /* if the current level is within the min and max threshold */
      tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c, g_TIMER_COUNT_ONE_SECOND_c,
                             App_Level_Control_Move_Command_Call_Back );
   }
}
/***********************************************************************************************/

#endif                    /* ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
#if (g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1)
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )

void App_ZLL_Move_Current_Level (uint8_t effect_variant, uint8_t effect_itentifier, uint8_t ep)
{
	g_Current_Level_Endpoint = ep;
    /* checking effect identifier value is zero */
    if ( effect_itentifier == g_ZERO_c){
        /* if effect identifier value is zero check whethter effect variant is zero */
        if (effect_variant == g_ZERO_c){
            /* Make OnOff udated is true */
            g_Is_On_Off_To_Be_Updated = g_TRUE_c;
            /* Making timer count zero */
            m_timer_count = 0;
            /* Making current level to minimum value */
            m_curr_level = LEVEL_MIN_VALUE;
            /* Writing Current level to currnt level attribute */
            App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
                                   &m_curr_level, g_CURRENT_LEVEL_ATTRIBUTE_c );
            /* Start timer call back function */
        tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                                g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                                App_ZLL_Level_Control_Fade_To_Off_In_0_8_Sec_Command_Call_Back );
        }
        /* check whethter effect variant is two */
        else if ( effect_variant == 0x02 ){
            /* Making timer count zero */
           m_timer_count = 0;
           /* Making OnOff transition time to 20 seconds */
           g_On_Off_trans_time = FADE_OFF_TIME_12_SEC + FADE_OFF_TIME_0_8_SEC;
           /* Read Current level */
           App_Read_Write_Level_Control_Attributes ( g_Read_Attribute_Data_c,
                                   &m_curr_level, g_CURRENT_LEVEL_ATTRIBUTE_c );
           /* Making current level to half of it because this value is used for 50
           percent dim dowm */
           m_curr_level = m_curr_level/2;
           /* Start timer call back */
       tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                               g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                               App_ZLL_Level_Control_50_per_Dim_Fade_To_Off_In_12_Sec_Command_Call_Back );
        }
    }
    /* checking effect identifier value is two */
    else if ( effect_itentifier == 0x01 ){
        /* if effect identifier value is two check whethter effect variant is zero */
        if ( effect_variant == g_ZERO_c){
            /* Making timer count zero */
           m_timer_count = g_ZERO_c;
           m_curr_level = m_curr_level + m_curr_level/5;
           /* Start timer call back */
         tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                       g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                       App_ZLL_Level_Control_20per_Fade_To_Off_In_1_Sec_Command_Call_Back );
        }
    }


 }

void App_ZLL_Level_Control_Fade_To_Off_In_0_8_Sec_Command_Call_Back (  uint8_t timer_id )

{
       m_timer_count++;
       /* Check Whether timer count is lss than 0.8 seconds */
       if ( m_timer_count < DIMMING_TIME_0_8_SEC ){
       /* Start timer call back */
       tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                               g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                               App_ZLL_Level_Control_Fade_To_Off_In_0_8_Sec_Command_Call_Back );
       }
       else{
           /* otherwise make timer count and current level as zero */
          m_timer_count = g_ZERO_c;
          m_curr_level = g_ZERO_c;
          /* Write current level to the attribute */
        App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                                   &m_curr_level, g_CURRENT_LEVEL_ATTRIBUTE_c );
        /* Stop timer */
        tmr_stopTimer( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c	);
       }


}

void App_ZLL_Level_Control_50_per_Dim_Fade_To_Off_In_12_Sec_Command_Call_Back (  uint8_t timer_id)

{
       m_timer_count++;
       /* Check Whether timer count is lss than 0.8 seconds */
       if ( m_timer_count < DIMMING_TIME_0_8_SEC ){
       /* Start timer call back */
       tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                       g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                       App_ZLL_Level_Control_50_per_Dim_Fade_To_Off_In_12_Sec_Command_Call_Back );

       }
       /* otherwise if the timer count is greater tha 0.8 seconds and it is less than 12 seconds */
       else if (m_timer_count < g_On_Off_trans_time){
        /* Start timer call back */
       tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                       g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                       App_ZLL_Level_Control_50_per_Dim_Fade_To_Off_In_12_Sec_Command_Call_Back );

       }
       /* Check timer count is equal to 0.8 seconds */
       if( m_timer_count == DIMMING_TIME_0_8_SEC)
       /* if timer count is 0.8 seconds write crrrent ( 50 per dim dowm) level to the attribute */
       App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                                   &m_curr_level, g_CURRENT_LEVEL_ATTRIBUTE_c );
       /* if timer count is greater than or equal to 12 seconds */
       if( m_timer_count >= g_On_Off_trans_time){
       /* making timer count as zero */
       m_timer_count = g_ZERO_c;
       /* making current level as zero */
       m_curr_level = g_ZERO_c;
       /* Writing current level to the attribute */
       App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                                   &m_curr_level, g_CURRENT_LEVEL_ATTRIBUTE_c );
       /*stop timer */
       tmr_stopTimer( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c	);

       }
}

void App_ZLL_Level_Control_20per_Fade_To_Off_In_1_Sec_Command_Call_Back (  uint8_t timer_id )

{
       /* increment timer count */
       m_timer_count++;
       /* checking whether timer count is less than 0.5 seconds */
       if ( m_timer_count < DIMMING_TIME_0_5_SEC ){
        /* if timer count is less than 0.5 sec start timer call back for 0.1 seconds */
       tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                           g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                           App_ZLL_Level_Control_20per_Fade_To_Off_In_1_Sec_Command_Call_Back );
       }
       /* checking whether timer count is less than 1 seconds */
       else if ( m_timer_count < FADE_OFF_TIME_1_SEC ){
        /* if timer count is less than 1 sec start timer call back for 0.1 seconds */
      tmr_startRelativeTimer ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c,
                           g_TIMER_COUNT_HUNDRED_MILLISEC_c  ,
                           App_ZLL_Level_Control_20per_Fade_To_Off_In_1_Sec_Command_Call_Back );

       }

       if ( m_timer_count == DIMMING_TIME_0_5_SEC ){
       App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                                   &m_curr_level, g_CURRENT_LEVEL_ATTRIBUTE_c );
       }
        /* if timer count is greater than 1 sec */
       if ( m_timer_count >= FADE_OFF_TIME_1_SEC ){
       /* making timer count as zero */
       m_timer_count = g_ZERO_c;
       /* making current level as zero */
       m_curr_level = g_ZERO_c;
       /* Writing current level to the attribute */
       App_Read_Write_Level_Control_Attributes ( g_Write_Attribute_Data_c,
                                   &m_curr_level, g_CURRENT_LEVEL_ATTRIBUTE_c );
       /*stop timer */
       tmr_stopTimer( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c	);
       }

}

#endif /*g_ZLL_ON_OFF_CLUSTER_ENABLE_d*/
#endif /*g_ON_OFF_SERVER_CLUSTER_ENABLE_d */
#endif /*g_LEVEL_CONTROL_SERVER_CLUSTER_ENABLE_d*/

/***********************************************************************************************/

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
