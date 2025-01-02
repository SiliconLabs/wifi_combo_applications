/*******************************************************************************
* AppThermostatUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Thermostat
*   Cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"
#include "stack_common.h"
#include "zdo_interface.h"
#include "apsde_interface.h"
#include "ZCL_Interface.h"
#include "memory_utility.h"
#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Thermostat_Cluster.h"
#include "AppThermostatUtilities.h"
#include "ApplicationThinLayer.h"

#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_M_And_S_Relative_Humidity_Measurement_Cluster.h"
#endif   /*g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d */


#if(g_HA_1_2_d == 1)
/*-----------------------------------------------------------------------------
* Global constant declarations
*----------------------------------------------------------------------------*/

#define g_THERMOSTAT_MAX_WEEKLY_EVENTS_c				5
#define g_MINUTES_PER_DAY_c                             0x05A0

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
App_Set_Weekly_Schedule_t aWeeklyScheduleServerTable[7];
#endif   /* g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d*/

#if ( g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d == 1 )
App_Set_Weekly_Schedule_t aWeeklyScheduleClientTable[7];
#endif  /*g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d */

extern HVAC_Thermostat_Cluster_Attributes_t Thermostat_server_cluster_data;



/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
uint16_t time_of_the_day;
uint8_t day_of_the_week;
uint8_t scheduled_transitions_for_week;
uint8_t relay_status;
uint16_t current_set_point;
uint8_t no_of_transitions;
uint8_t  schedule_index =0;
#endif   /*g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d */

#if ( g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d == 1 )
uint8_t client_schedule_index;
#endif   /* g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d*/

uint8_t mode_to_return;
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
static uint8_t App_Update_WeeklySchedule(uint8_t * recWeeklyEvent,
										 APSDE_Data_Indication_t *pBuffer );

static void App_createAndSend_CurrentWeeklyScheduleResponse( uint8_t *WeeklySchedule,
															APSDE_Data_Indication_t *pBuffer);

static void App_CreatAndSend_Relay_Status_Log( uint8_t *getRelayStatus,
                                                   APSDE_Data_Indication_t *pBuffer );

static uint8_t  app_validateSetPoints(uint8_t mode,   int16_t set_point );

uint8_t app_checkforoverlap(App_Set_Weekly_Schedule_t *transition_time,uint8_t no_of_transition);

void  App_CreatAndSend_Clear_Weekly_Schedule(  APSDE_Data_Indication_t *pBuffer );

void App_WeeklySchedule_Tick(void);


#endif   /*g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d */

#if ( g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d == 1 )
static void App_Update_CurrentWeeklySchedule(uint8_t *currentWeeklyEvent,
										 APSDE_Data_Indication_t *pBuffer );

#endif   /* g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d*/

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
void App_Thermostat_Task(void)
{
	uint32_t current_time = ZCL_CallBackGetCurrentTime();
    time_of_the_day = (current_time/60);
	if(time_of_the_day > 0x05A0) {
	  time_of_the_day= 0x0000;
	  day_of_the_week++;
	}
	if(day_of_the_week > 6){
	  day_of_the_week =0;
	}
	App_WeeklySchedule_Tick();
}

void App_WeeklySchedule_Tick(void)
{
  uint8_t i;

  if(aWeeklyScheduleServerTable[day_of_the_week].day_of_week_for_sequence < 7 )  {
	
	for (i=0;i <  aWeeklyScheduleServerTable[day_of_the_week]. number_of_transitions_for_sequence;i++) {
		if(aWeeklyScheduleServerTable[day_of_the_week].transition_sequence[i].transition_time == time_of_the_day)
		{
		  if(aWeeklyScheduleServerTable[day_of_the_week].mode_for_sequence == g_Set_Point_Heat_c
			 								||g_Set_Point_Both_Heat_Cool_c ) {
			
			/*  relay_status =  heating operation*/
				relay_status = g_Heat_State_On_c;
				current_set_point = aWeeklyScheduleServerTable[day_of_the_week].transition_sequence[i].heat_set_point;
			break;
			
		  }
		  else if(aWeeklyScheduleServerTable[day_of_the_week].mode_for_sequence == g_Set_Point_Cool_c
				  							|| g_Set_Point_Both_Heat_Cool_c) {
			/*  relay_status =  cooling operation*/
				relay_status = g_Cool_State_On_c;
				current_set_point = aWeeklyScheduleServerTable[day_of_the_week].transition_sequence[i].cool_set_point;
		 			
			break;
		  }
		
		}
	}
	scheduled_transitions_for_week--;
  }
}
#endif   /* g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d*/
/**************************************************************************************/
void App_ThermostatEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
	uint8_t	status,start_of_week; 	
	uint8_t asdulength;
	uint8_t asdu[10];
#endif   /* g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d*/	
	
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )

#if(g_HA_1_2_d == 1)	
        if (( pZCLEvent->eventId )== g_Set_Weekly_Schedule_Event_c){
		
	/* start of week attribute determines, if the device supports weekly scheduling.
		  by reading the attribute. Successful response means that the weekly
		  scheduling is supported */	
#if ( g_START_OF_WEEK_ATTRIBUTE_ENABLE_d == 1 )
    status = App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_THERMOSTAT_CLUSTER_c,
                                g_START_OF_WEEK_ATTRIBUTE_c,
                                (uint8_t*)&start_of_week,
                                g_Read_Attribute_Data_c );
#else
	/* else weekly scheduling unsupported*/
	status = g_ZCL_UnSup_Cluster_Command_c;
#endif   /* g_HA_1_2_d*/	

		  /* After receiving Set Weekly Schedule command, update the
		  weekly schedule table for the current week*/
		
		  /*  Update the current schedule*/
			if(status == g_ZCL_Success_c )
			{
			  	 status = App_Update_WeeklySchedule( pZCLEvent->pEventData,pBuffer);
			}
			/* send default response */
			asdulength = ZCL_CreateDefaultResponseCommand ( pBuffer->a_asdu,
															asdu,status );
			
			App_SendData (pBuffer->src_addr_mode,pBuffer->src_address.IEEE_address,
							pBuffer->src_endpoint, 	g_THERMOSTAT_CLUSTER_c,
							asdulength, asdu );
		}
		else if (( pZCLEvent->eventId )== g_Get_Weekly_Schedule_Event_c){
		  /* Send Current Weekly schedule as the response*/
		
		       App_createAndSend_CurrentWeeklyScheduleResponse (pZCLEvent->pEventData,
																pBuffer);
            }
		else if (( pZCLEvent->eventId )== g_Clear_Weekly_Schedule_Event_c){
		  /* Clear all the transitions in the weekly schedule and
		  default response as SUCCESS*/
		   App_CreatAndSend_Clear_Weekly_Schedule( pBuffer );
		
		}
		else if (( pZCLEvent->eventId )== g_Get_Relay_Status_Log_Event_c){
			
			 App_CreatAndSend_Relay_Status_Log( pZCLEvent->pEventData, pBuffer );		
		}
#endif   /* g_HA_1_2_d*/		
#endif   /*g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d */

#if ( g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d == 1 )
#if(g_HA_1_2_d == 1)
		if (( pZCLEvent->eventId )== g_Current_Weekly_Schedule_Event_c){
		
		   App_Update_CurrentWeeklySchedule(pZCLEvent->pEventData,pBuffer);
		
		}
		else if (( pZCLEvent->eventId )== g_Relay_Status_Log_Event_c){
		}
#endif   /*g_HA_1_2_d */		
#endif   /* g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d*/
}

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/
/***********************************************************************************/
/*  App Update Weekly Schedule*/
/***********************************************************************************/
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
static uint8_t App_Update_WeeklySchedule( uint8_t * recWeeklyEvent,
										 		APSDE_Data_Indication_t *pBuffer )
{
	uint8_t i ,heat_set_point_validation = g_ZCL_Success_c;
	uint8_t cool_set_point_validation = g_ZCL_Success_c,status;
	 App_Set_Weekly_Schedule_t *WeeklyEvent = (App_Set_Weekly_Schedule_t *) recWeeklyEvent ;
	/* If the total number of transitions sent is greater than what the thermostat
	 supports , a default response of INSUFFICIENT_SPACE (0x89) shall be sent
	in response to the last command sent for that transition sequence. */
		if(no_of_transitions < Thermostat_server_cluster_data.number_of_weekly_transitions ){
		
		    /* received day of week for sequence. Day of week is an bitmap type,
			multiple days of an week can be included in single command*/
			if(WeeklyEvent->day_of_week_for_sequence & 0x01) {
			      schedule_index =0;
				  aWeeklyScheduleServerTable[schedule_index].day_of_week_for_sequence = g_Sunday_c;
			
			}
			else if(WeeklyEvent->day_of_week_for_sequence & 0x02) {
			      schedule_index =1;
				  aWeeklyScheduleServerTable[schedule_index].day_of_week_for_sequence = g_Monday_c;
			}
			else if(WeeklyEvent->day_of_week_for_sequence & 0x04 ) {
			      schedule_index =2;
				  aWeeklyScheduleServerTable[schedule_index].day_of_week_for_sequence = g_Tuesday_c;
			}
			else if(WeeklyEvent->day_of_week_for_sequence  & 0x08 ) {
			      schedule_index =3;
				  aWeeklyScheduleServerTable[schedule_index].day_of_week_for_sequence = g_Wednesday_c;
			}
			else if(WeeklyEvent->day_of_week_for_sequence  & 0x10 ) {
			      schedule_index =4;
				  aWeeklyScheduleServerTable[schedule_index].day_of_week_for_sequence = g_Thursday_c;
			}
			else if(WeeklyEvent->day_of_week_for_sequence  & 0x20 ) {
			      schedule_index =5;
				  aWeeklyScheduleServerTable[schedule_index].day_of_week_for_sequence = g_Friday_c;
			}
			else if(WeeklyEvent->day_of_week_for_sequence & 0x40) {
			      schedule_index =6;
				  aWeeklyScheduleServerTable[schedule_index].day_of_week_for_sequence = g_Saturday_c;
			}
			else {
			      schedule_index =7;
				  aWeeklyScheduleServerTable[schedule_index].day_of_week_for_sequence = 0x07;
			}
			
			if(WeeklyEvent->number_of_transitions_for_sequence > g_NUMBER_OF_TRANSITION_FOR_SEQUENCE_c)
			{
				return g_ZCL_Insufficient_Space_c;
			}
			else {
				aWeeklyScheduleServerTable[schedule_index].number_of_transitions_for_sequence =
							   WeeklyEvent->number_of_transitions_for_sequence;
			}
			
			if(WeeklyEvent->mode_for_sequence  == g_app_heat_set_point_c){
			/* if the bit 0 of Mode for sequence is enabled, then heat set point
			present int the payload */
			  aWeeklyScheduleServerTable[schedule_index].mode_for_sequence =  WeeklyEvent->mode_for_sequence;
		
			/*if the requested set points out of range i.e not in range between (AbsMin/MaxSetPointLimit),
			  then  send default response inavlid  value and discard entire setpoints  */	
				for ( i=0;i < WeeklyEvent->number_of_transitions_for_sequence;i++) {
				
				  heat_set_point_validation = app_validateSetPoints(WeeklyEvent->mode_for_sequence,
												WeeklyEvent->transition_sequence[i].heat_set_point);
				
				  status = app_checkforoverlap(WeeklyEvent,
										WeeklyEvent->number_of_transitions_for_sequence);
				
				  if(heat_set_point_validation == g_ZCL_Success_c && status == g_ZCL_Success_c) {
					aWeeklyScheduleServerTable[schedule_index].transition_sequence[i].transition_time =
									WeeklyEvent->transition_sequence[i].transition_time;
					aWeeklyScheduleServerTable[schedule_index].transition_sequence[i].heat_set_point =
									WeeklyEvent->transition_sequence[i].heat_set_point;
				  }
				  else {
					
					   if (( heat_set_point_validation !=g_ZCL_Success_c)){
            					return  g_ZCL_Invalid_Value_c;
                          }
                    if ( status != g_ZCL_Success_c ) {
                        return  g_ZCL_Failure_c;
                    }
				  }
				}
			}
			else if(WeeklyEvent->mode_for_sequence== g_app_cool_set_point_c ){
			/* if the bit 1 of Mode for sequence is enabled, then cool set point
			present int the payload */
			  aWeeklyScheduleServerTable[schedule_index].mode_for_sequence =  WeeklyEvent->mode_for_sequence;
		
				for ( i=0;i < WeeklyEvent->number_of_transitions_for_sequence;i++) {
				
			/*if the requested set points out of range i.e not in range between (AbsMin/MaxSetPointLimit),
			  then  send default response inavlid  value and discard entire setpoints  */	
				  cool_set_point_validation = app_validateSetPoints(WeeklyEvent->mode_for_sequence,
									WeeklyEvent->transition_sequence[i].cool_set_point);
				
				   status = app_checkforoverlap(WeeklyEvent,
										WeeklyEvent->number_of_transitions_for_sequence);
				
				  if(cool_set_point_validation == g_ZCL_Success_c && status == g_ZCL_Success_c) {
		
					aWeeklyScheduleServerTable[schedule_index].transition_sequence[i].transition_time =
									 WeeklyEvent->transition_sequence[i].transition_time;
		
					aWeeklyScheduleServerTable[schedule_index].transition_sequence[i].cool_set_point =
										WeeklyEvent->transition_sequence[i].cool_set_point;
				  }
				  else {
					   if (( cool_set_point_validation !=g_ZCL_Success_c)){
            					return  g_ZCL_Invalid_Value_c;
                          }
                    if ( status != g_ZCL_Success_c ) {
                        return  g_ZCL_Failure_c;
                    }
				  }
				}
			}
			else if(WeeklyEvent->mode_for_sequence  == g_app_both_heat_and_cool_set_point_c ){
			/* if both bit 0 & 1 of Mode for sequence is enabled, then both heat set point
			and cool set point present in the payload */	
			  aWeeklyScheduleServerTable[schedule_index].mode_for_sequence =  WeeklyEvent->mode_for_sequence;
		
				for ( i=0;i < WeeklyEvent->number_of_transitions_for_sequence;i++) {
			/*if the requested set points out of range i.e not in range between (AbsMin/MaxSetPointLimit),
			  then  send default response inavlid  value and discard entire setpoints  */	
				  heat_set_point_validation = app_validateSetPoints(WeeklyEvent->mode_for_sequence,
									WeeklyEvent->transition_sequence[i].heat_set_point);
				
				  cool_set_point_validation = app_validateSetPoints(WeeklyEvent->mode_for_sequence,
									WeeklyEvent->transition_sequence[i].cool_set_point);
				
				  status = app_checkforoverlap(WeeklyEvent,
										WeeklyEvent->number_of_transitions_for_sequence);
				
				  if(heat_set_point_validation == g_ZCL_Success_c && cool_set_point_validation ==g_ZCL_Success_c
					 && status == g_ZCL_Success_c) {		
			
					  aWeeklyScheduleServerTable[schedule_index].transition_sequence[i].transition_time =
										 WeeklyEvent->transition_sequence->transition_time;
					  aWeeklyScheduleServerTable[schedule_index].transition_sequence[i].heat_set_point =
										 WeeklyEvent->transition_sequence->heat_set_point;
					  aWeeklyScheduleServerTable[schedule_index].transition_sequence[i].cool_set_point =
										WeeklyEvent->transition_sequence->cool_set_point;	
				  }
				  else {
                    if ((cool_set_point_validation != g_ZCL_Success_c) || ( cool_set_point_validation !=g_ZCL_Success_c)){
            					return  g_ZCL_Invalid_Value_c;
                          }
                    if ( status != g_ZCL_Success_c ) {
                        return  g_ZCL_Failure_c;
                    }
				  }	
				}
			}
		
			no_of_transitions = no_of_transitions + WeeklyEvent->number_of_transitions_for_sequence;
			scheduled_transitions_for_week =  no_of_transitions;
		
		}
		else {
		  return g_ZCL_Insufficient_Space_c;
		}
		return g_ZCL_Success_c;
	
	
}
/**************************************************************************************/


static void App_createAndSend_CurrentWeeklyScheduleResponse( uint8_t *WeeklySchedule,
															APSDE_Data_Indication_t *pBuffer)
{
    uint8_t index,i, asduLength=0x00, offset=0x00;
    Get_weekly_Schedule_t  *GetWeeklyScheduleCmdPayLoadRecvd;
	uint8_t day_to_return ;
	
	GetWeeklyScheduleCmdPayLoadRecvd = (Get_weekly_Schedule_t *) WeeklySchedule;
	
	App_Transition_Sequence_t *getWeeklyTransition;
	
	 /* ZCL Header Creation */
    pBuffer->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c
                            | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    pBuffer->a_asdu[1] = App_GetTransSeqNumber();
    pBuffer->a_asdu[2] = g_Current_Weekly_Schedule_Command_c;
	  /* copy the schedule for requested  day to response payload*/
	
	mode_to_return = GetWeeklyScheduleCmdPayLoadRecvd->mode_to_return;
	 if(GetWeeklyScheduleCmdPayLoadRecvd->days_to_return & 0x01) {
	   day_to_return= g_Sunday_c;
	
	}
	else if(GetWeeklyScheduleCmdPayLoadRecvd->days_to_return & 0x02) {
	   day_to_return= g_Monday_c;
	}
	else if(GetWeeklyScheduleCmdPayLoadRecvd->days_to_return & 0x04 ) {
	   day_to_return= g_Tuesday_c;
	}
	else if(GetWeeklyScheduleCmdPayLoadRecvd->days_to_return  & 0x08 ) {
	   day_to_return= g_Wednesday_c;
	}
	else if(GetWeeklyScheduleCmdPayLoadRecvd->days_to_return  & 0x10 ) {
	   day_to_return= g_Thursday_c;
	}
	else if(GetWeeklyScheduleCmdPayLoadRecvd->days_to_return  & 0x20 ) {
	   day_to_return= g_Friday_c;
	}
	else if(GetWeeklyScheduleCmdPayLoadRecvd->days_to_return & 0x40) {
	   day_to_return= g_Saturday_c;
	}
	
	for ( index = 0 ;index<7;index++ ){
		
		if( aWeeklyScheduleServerTable[index].day_of_week_for_sequence == day_to_return) {		
		
			pBuffer->a_asdu[3] =
					aWeeklyScheduleServerTable[index].number_of_transitions_for_sequence;
			pBuffer->a_asdu[4] =
					GetWeeklyScheduleCmdPayLoadRecvd->days_to_return;
			pBuffer->a_asdu[5] =
								aWeeklyScheduleServerTable[index].mode_for_sequence;
			
			asduLength = 3 + 3;   /* zcl header 3 bytes and weekly schedule cmd
								header 3 bytes*/
			
			getWeeklyTransition =   (App_Transition_Sequence_t *)&pBuffer->a_asdu[6];
			
			for (i=0;i <aWeeklyScheduleServerTable[index].number_of_transitions_for_sequence;
																i++) {
			memUtils_memCopy(  (uint8_t*)getWeeklyTransition+offset,
									 (uint8_t*)&aWeeklyScheduleServerTable[index].transition_sequence[i].transition_time, 0x02 );
			
			asduLength = asduLength +2;
			offset= offset + 2;
			if((mode_to_return ==  g_app_heat_set_point_c) || (mode_to_return == g_app_both_heat_and_cool_set_point_c)){
																
				memUtils_memCopy( (uint8_t*)getWeeklyTransition+offset,
						(uint8_t*)&aWeeklyScheduleServerTable[index].transition_sequence[i].heat_set_point, 0x02 );
					
				asduLength = asduLength +2;
				offset= offset + 2;
			}
		if((mode_to_return == g_app_cool_set_point_c) ||(mode_to_return == g_app_both_heat_and_cool_set_point_c)){
											
			 memUtils_memCopy( (uint8_t*)getWeeklyTransition+offset,
								 (uint8_t*)&aWeeklyScheduleServerTable[index].transition_sequence[i].cool_set_point, 0x02 );
			
			asduLength = asduLength +2;
			offset= offset + 2;
			}
		}
		
		break;			
		}
	}	
	App_SendData( pBuffer->src_addr_mode,
		 pBuffer->src_address.IEEE_address,
		 pBuffer->src_endpoint,
		 g_THERMOSTAT_CLUSTER_c,
		 asduLength,
		 pBuffer->a_asdu );  	
}
/**************************************************************************************/
void App_WeeklyScheduleInit( void)
{
	uint8_t i,index;
	no_of_transitions = 0x00;
	for(i=0;i<7;i++)
	{
		aWeeklyScheduleServerTable[i].number_of_transitions_for_sequence = 0x00;
		aWeeklyScheduleServerTable[i].day_of_week_for_sequence = 0x00;
		aWeeklyScheduleServerTable[i].mode_for_sequence = 0x00;
		
		for ( index=0;index < g_NUMBER_OF_TRANSITION_FOR_SEQUENCE_c;index++) {
		
			memUtils_memSet( (uint8_t*)&aWeeklyScheduleServerTable[i].transition_sequence[index].transition_time,
															0x0000,  sizeof( uint16_t ));
			memUtils_memSet( (uint8_t*)&aWeeklyScheduleServerTable[i].transition_sequence[index].heat_set_point,
															0x0000,  sizeof( int16_t ));
			memUtils_memSet( (uint8_t*)&aWeeklyScheduleServerTable[i].transition_sequence[index].cool_set_point,
															0x0000,  sizeof( int16_t ));
		}
		
	
	}
}


/*******************************************************************************************/
/*  Validate the SetPoints received from the set_weekly_ schedule command   */
/*******************************************************************************************/
static uint8_t  app_validateSetPoints(uint8_t mode,   int16_t set_point )
{
	if(mode == g_app_heat_set_point_c )
	{
		/* if the mode is set point heat,
	     heat set point should be within the (AbsMin/MaxHeatSetPointLimit)*/
	
	    int16_t min_heat_set_point =0x0000, max_heat_set_point =0x0000;
		memUtils_memCopy((uint8_t*)&min_heat_set_point,
						 Thermostat_server_cluster_data.a_abs_min_heat_setpoint_limit,2);
		memUtils_memCopy((uint8_t*)&max_heat_set_point,
						 Thermostat_server_cluster_data.a_abs_max_heat_setpoint_limit,2);
		
		if(set_point > min_heat_set_point && set_point < max_heat_set_point){
		  return g_ZCL_Success_c;
		}

	}
	else if(mode == g_app_cool_set_point_c )
	{
	  	/* if the mode is set point cool,
	     cool set point should be within the (AbsMin/MaxCoolSetPointLimit) */
	
	    int16_t min_cool_set_point=0x0000, max_cool_set_point=0x0000;
		memUtils_memCopy((uint8_t*)&min_cool_set_point,
						 Thermostat_server_cluster_data.a_abs_min_cool_setpoint_limit,2);
		memUtils_memCopy((uint8_t*)&max_cool_set_point,
						 Thermostat_server_cluster_data.a_abs_max_cool_setpoint_limit,2);
		
		if(set_point > min_cool_set_point && set_point < max_cool_set_point){
		  return g_ZCL_Success_c;
		}

	}
	else if(mode == g_app_both_heat_and_cool_set_point_c )
	{
	    /* if the mode is g_Set_Point_Both_Heat_Cool_c,
	     cool set point should be within the (AbsMin/MaxCoolSetPointLimit) &
	  	(AbsMin/MaxHeatSetPointLimit)*/
	
	    int16_t min_heat_set_point=0x0000, max_heat_set_point=0x0000;
		int16_t min_cool_set_point=0x0000, max_cool_set_point=0x0000;
		
		memUtils_memCopy((uint8_t*)&min_heat_set_point,
						 Thermostat_server_cluster_data.a_abs_min_heat_setpoint_limit,2);
		memUtils_memCopy((uint8_t*)&max_heat_set_point,
						 Thermostat_server_cluster_data.a_abs_max_heat_setpoint_limit,2);
		
		memUtils_memCopy((uint8_t*)&min_cool_set_point,
						 Thermostat_server_cluster_data.a_abs_min_cool_setpoint_limit,2);
		memUtils_memCopy((uint8_t*)&max_cool_set_point,
						 Thermostat_server_cluster_data.a_abs_max_cool_setpoint_limit,2);

		if((set_point > min_cool_set_point && set_point < max_cool_set_point) ||
		   (set_point > min_heat_set_point && set_point < max_heat_set_point)){
			 return g_ZCL_Success_c;
        
//		if((set_point > min_cool_set_point && set_point < max_cool_set_point) &&
//		   (set_point > min_heat_set_point && set_point < max_heat_set_point)){
//			 return g_ZCL_Success_c;
		}
		
	}
	/*if the heat/cool setpoints out of range , then return  invalid value */	
	return g_ZCL_Invalid_Value_c;
}

/***********************************************************************************/
static void App_CreatAndSend_Relay_Status_Log( uint8_t *getRelayStatus,
                                                   APSDE_Data_Indication_t *pBuffer )
{
    int16_t localTemperature;
	uint16_t  measured_humidity;
	
	uint8_t  asduLength;
	
	relay_status_log_t *relayStatusLogPayload;
   /* ZCL Header Creation */
    pBuffer->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c;
    pBuffer->a_asdu[1] = App_GetTransSeqNumber();
    pBuffer->a_asdu[2] = g_Relay_Status_Log_Command_c;
	
	relayStatusLogPayload = (relay_status_log_t*)&pBuffer->a_asdu[3];
	/*  This field represents the sample time of the day when the relay status
	is captured for this associated log entry */
    relayStatusLogPayload->time_of_day = time_of_the_day; /*designate the minutes since midnight */
	
	/*presents the relay status for thermostat when the log is captured ,
	Each bit represents one relay used by the thermostat. If the bit is on,
	the associated relay is on and active*/
	
	App_ReadWriteAttribute( pBuffer->dest_endpoint,
                    g_THERMOSTAT_CLUSTER_c,
                    g_LOCAL_TEMPERATURE_ATTRIBUTE_c,
                    (uint8_t*)&localTemperature,
                    g_Read_Attribute_Data_c );
	
	relayStatusLogPayload->local_temperature = localTemperature;
	
	/* presents the local temperature when the log is captured */
	
//    App_ReadWriteAttribute( pBuffer->dest_endpoint,
//                    g_THERMOSTAT_CLUSTER_c,
//                    g_THERMOSTAT_RUNNING_STATE_ATTRIBUTE_c,
//                    (uint8_t*)&relayStatus,
//                    g_Read_Attribute_Data_c );  	
	
    relayStatusLogPayload->relay_status = relay_status;
	
	/* presents the humid in percentage when the log is captured */
#if ( g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ENABLE_d == 1 )	
	 App_ReadWriteAttribute( pBuffer->dest_endpoint,
                    g_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_c,
                    m_MS_ATTRIB_HUMIDITY_MEASURED_VALUE_c,
                    (uint8_t*)&measured_humidity,
                    g_Read_Attribute_Data_c );  	
	
	relayStatusLogPayload->humidity_in_percenatge= measured_humidity /100;
#endif   /* */	
	/* presents the target setpoint temperature when the log is captured.*/
	relayStatusLogPayload->set_point = current_set_point;
	
	/*This field presents the number of unread entries within the thermostat
	internal log system. */
	relayStatusLogPayload->unread_entries = scheduled_transitions_for_week;
	
	  asduLength = 3 + 11;
		App_SendData( pBuffer->src_addr_mode,
				 pBuffer->src_address.IEEE_address,
				 pBuffer->src_endpoint,
				 g_THERMOSTAT_CLUSTER_c,
				 asduLength,
				 pBuffer->a_asdu );
	
	return;

}
/***********************************************************************************/

uint8_t app_checkforoverlap(App_Set_Weekly_Schedule_t *transition_time,
														uint8_t no_of_transition)
{
  uint8_t i;

  /* the received tranitions should not be overlapped, if it is overlapped send
  default response as a failure */

  if(no_of_transition == 1){
	 return g_ZCL_Success_c;
  }
  for (i=0;i<no_of_transition-1;i++)
  {
	 if(transition_time->transition_sequence[i].transition_time <
						transition_time->transition_sequence[i+1].transition_time)
	 	continue;
	 else
	   return g_ZCL_Failure_c;
  }
  return g_ZCL_Success_c;
}

/**********************************************************************************/
void  App_CreatAndSend_Clear_Weekly_Schedule(APSDE_Data_Indication_t *pBuffer )
{
  uint8_t asdulength;
  uint8_t asdu[10];
	/*clear scheduled setpoints for each day and send default response
	  success*/  	
	App_WeeklyScheduleInit ();
	no_of_transitions = 0;
	asdulength = ZCL_CreateDefaultResponseCommand ( pBuffer->a_asdu,  asdu,
														g_ZCL_Success_c );
	App_SendData (  pBuffer->src_addr_mode,
						 pBuffer->src_address.IEEE_address,
						pBuffer->src_endpoint,
						g_THERMOSTAT_CLUSTER_c,
						asdulength,
						asdu );
}
/***********************************************************************************/
#endif   /* g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d*/
#if ( g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d == 1 )

static void App_Update_CurrentWeeklySchedule(uint8_t *currentWeeklyEvent,
										 APSDE_Data_Indication_t *pBuffer )
{
    uint8_t i;
	App_Set_Weekly_Schedule_t *curWeeklyEvent = (App_Set_Weekly_Schedule_t *)currentWeeklyEvent ;
	/* received day of week for sequence. Day of week is an bitmap type,
	multiple days of an week can be included in single command*/
	
	
	if(curWeeklyEvent->day_of_week_for_sequence & 0x01) {
		  client_schedule_index =0;
		  aWeeklyScheduleClientTable[client_schedule_index].day_of_week_for_sequence = g_Sunday_c;
	
	}
	else if(curWeeklyEvent->day_of_week_for_sequence & 0x02) {
		  client_schedule_index =1;
		  aWeeklyScheduleClientTable[client_schedule_index].day_of_week_for_sequence = g_Monday_c;
	}
	else if(curWeeklyEvent->day_of_week_for_sequence & 0x04 ) {
		  client_schedule_index =2;
		  aWeeklyScheduleClientTable[client_schedule_index].day_of_week_for_sequence = g_Tuesday_c;
	}
	else if(curWeeklyEvent->day_of_week_for_sequence  & 0x08 ) {
		  client_schedule_index =3;
		  aWeeklyScheduleClientTable[client_schedule_index].day_of_week_for_sequence = g_Wednesday_c;
	}
	else if(curWeeklyEvent->day_of_week_for_sequence  & 0x10 ) {
		  client_schedule_index =4;
		  aWeeklyScheduleClientTable[client_schedule_index].day_of_week_for_sequence = g_Thursday_c;
	}
	else if(curWeeklyEvent->day_of_week_for_sequence  & 0x20 ) {
		  client_schedule_index =5;
		  aWeeklyScheduleClientTable[client_schedule_index].day_of_week_for_sequence = g_Friday_c;
	}
	else if(curWeeklyEvent->day_of_week_for_sequence & 0x40) {
		  client_schedule_index =6;
		  aWeeklyScheduleClientTable[client_schedule_index].day_of_week_for_sequence = g_Saturday_c;
	}
	else {
		  client_schedule_index =7;
		  aWeeklyScheduleClientTable[client_schedule_index].day_of_week_for_sequence = 0x07;
	}
	
	aWeeklyScheduleClientTable[client_schedule_index].number_of_transitions_for_sequence =
							curWeeklyEvent->number_of_transitions_for_sequence;
	
	aWeeklyScheduleClientTable[client_schedule_index].mode_for_sequence =
							curWeeklyEvent->mode_for_sequence;
	
	for (i=0;i <curWeeklyEvent->number_of_transitions_for_sequence;i++) {
	
		aWeeklyScheduleClientTable[client_schedule_index].transition_sequence[i].transition_time
							= curWeeklyEvent->transition_sequence[i].transition_time;
		
		
		if(curWeeklyEvent->mode_for_sequence == g_app_heat_set_point_c || g_app_both_heat_and_cool_set_point_c){
		
			aWeeklyScheduleClientTable[client_schedule_index].transition_sequence[i].heat_set_point
							= curWeeklyEvent->transition_sequence[i].heat_set_point;
		}
		if(curWeeklyEvent->mode_for_sequence == g_app_cool_set_point_c || g_app_both_heat_and_cool_set_point_c){
		
			aWeeklyScheduleClientTable[client_schedule_index].transition_sequence[i].cool_set_point
							= curWeeklyEvent->transition_sequence[i].cool_set_point;
		}
	}
}

#endif   /*g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d */

#endif   /* */
/* None */
#endif   /* */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/