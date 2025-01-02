/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include"ZigBeeDataTypes.h"
#include "common_includes.h"
#include "stack_common.h"
#include "ApplicationThinLayer.h"
#include "ApptimerId.h"
#include "AppColorControlUtilities.h"
#include "AppReportingUtilities.h"
#include "ZCL_Interface.h"
#include "ZCL_Foundation.h"
#include "sw_timer.h"
#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"

/*-----------------------------------------------------------------------------
* Global constant declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

uint8_t m_Hue_Rate;
uint8_t m_Lighting_Hue_Data;
uint16_t m_Enhanced_Lighting_Hue_Data ;
uint16_t m_transition_time;
uint8_t m_Hue_Step_Mode;
uint8_t m_Lighting_End_Point;
uint8_t m_Saturation_Rate;
uint8_t m_Lighting_Saturation_Data;
uint8_t m_To_Lighting_Saturation_Data;
uint8_t m_Saturation_Step_Mode;
uint8_t m_Color_Temperature_Mode;
uint8_t m_Color_Loop_Active_Data;
uint8_t m_Update_Flags;
uint8_t m_Action;
uint8_t m_Dirction;
uint16_t m_Start_Hue;
uint16_t m_Enhanced_Hue_Rate;
uint16_t m_color_x;
uint16_t m_color_y;
uint16_t m_rate_x;
uint16_t m_rate_y;
uint16_t m_step_color_x;
uint16_t m_step_color_y;
uint16_t m_color_temperature;
uint8_t m_move_to_hue_value;
uint8_t m_move_to_saturation_value;
uint16_t m_move_to_color_x;
uint16_t m_move_to_color_y;
uint8_t m_Lighting_To_Hue_Value;
uint8_t m_move_to_saturation_value;
uint8_t m_Hue_Value;
uint16_t m_Enhanced_Hue_Value ;
uint8_t m_Hue_Direction;
uint16_t m_tansition_time;
uint16_t m_Transition_Time;
uint16_t Color_Control_Timer_Count;
uint8_t m_Lighting_Saturation_Value;
uint16_t m_color_temperature_data;
uint16_t m_16bit_temporary_data1;
uint16_t m_16bit_temporary_data2;
uint16_t m_16bit_temporary_data3;
uint16_t m_present_value;
uint8_t m_8bit_temporary_data1;
uint8_t m_8bit_temporary_data2;
uint8_t  m_8bit_temporary_data3;
uint16_t Remaining_Time;
uint16_t m_Color_Temperature_Minimum;
uint16_t m_Color_Temperature_Maximum;
uint16_t m_Color_Temperatutre_Rate;
uint16_t m_move_to_enhanced_hue_value;
uint16_t m_Color_Loop_Store_Hue_Data;
uint16_t m_Color_Loop_Time_Data;
uint16_t m_Color_Loop_Start_Hue_Data;



App_Cluster_Def_t *p_cluster_definition;
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

void App_Handle_Color_Control_Cluster_Events( ZCL_Event_t *pActionHandler, uint8_t endpoint ){


	m_Lighting_End_Point = endpoint;
	Color_Control_Timer_Count = g_ZERO_c;
	tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c );
	uint8_t *ptr;

	switch ( pActionHandler->eventId )
	{

		   /* setting the move to hue mode */
		case g_Move_to_Hue_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition );


			ZCL_Read_Write_Attribute_Data (g_Read_Attribute_Data_c,ptr,
			   			   p_cluster_definition->p_cluster_info,
			   g_CURRENT_HUE_ATTRIBUTE_c,
			   &m_Lighting_Hue_Data);

			m_8bit_temporary_data1 = m_Lighting_Hue_Data;
			m_Hue_Value = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
						 move_to_hue_command_data.hue;
			m_Hue_Direction = ((Lighting_Event *)(pActionHandler->pEventData))->
							 event_data.move_to_hue_command_data.direction;
			memUtils_memCopy((uint8_t*) &m_Transition_Time, (uint8_t*)&(((Lighting_Event *)
				  (pActionHandler->pEventData))->event_data.
				   move_to_hue_command_data.a_transition_time ), 2);
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;

			if ( (((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			  move_to_hue_command_data.direction == g_Move_To_Hue_Up_c ) ||
			( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			 move_to_hue_command_data.direction == g_Shortest_Distance_c ) ||
			( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			 move_to_hue_command_data.direction == g_Longest_Distance_c ) ||
			( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			 move_to_hue_command_data.direction == g_Move_To_Hue_Down_c ) )

			{

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
			App_Move_To_Hue_Call_Back );

			}

            break;

		   /* setting the move hue mode */
		case g_Move_hue_Command_Event_c:

			m_Hue_Rate = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				           move_hue_command_data.rate;
			m_Hue_Step_Mode = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                move_hue_command_data.move_mode;
			m_Lighting_End_Point = endpoint;
			if ( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.move_hue_command_data.
				   move_mode == g_Stop_c ){

			  tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c );

			}

			else if ((((Lighting_Event *)(pActionHandler->pEventData))->event_data.
					     move_hue_command_data.move_mode == g_Up_c ) ||
				     (((Lighting_Event *)(pActionHandler->pEventData))->event_data.
					    move_hue_command_data.move_mode == g_Down_c ) )	{
			 tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			  				  g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
			  				  App_Move_Hue_Call_Back );

			}
			break;


           /* setting the step hue mode */
		case g_Step_Hue_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
												m_Lighting_End_Point,
												g_COLOR_CONTROL_CLUSTER_c,
												&p_cluster_definition );
 
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
										p_cluster_definition->p_cluster_info,
					g_CURRENT_HUE_ATTRIBUTE_c,
					&m_Lighting_Hue_Data );

			m_8bit_temporary_data1 = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                       step_hue_command_data.step_mode;
			m_8bit_temporary_data2 = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                       step_hue_command_data.step_size;
			m_Transition_Time = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                  step_hue_command_data.transition_time;
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;
			m_8bit_temporary_data3=m_Lighting_Hue_Data ;
		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			  g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
			  App_Step_Hue_Call_Back );


			break;

           /* setting the move to saturation mode */
		case g_Move_To_Saturation_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											 m_Lighting_End_Point,
											 g_COLOR_CONTROL_CLUSTER_c,
											 &p_cluster_definition );

			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
				  				  p_cluster_definition->p_cluster_info,
				  g_CURRENT_SATURATION_ATTRIBUTE_c,
				  &m_To_Lighting_Saturation_Data );
			m_Lighting_Saturation_Value = ((Lighting_Event *)(pActionHandler->pEventData))->
				                            event_data.move_to_saturation_command_data.saturation;
			memUtils_memCopy ((uint8_t*) &m_Transition_Time, (uint8_t*)&(((Lighting_Event *)
					  (pActionHandler->pEventData))->event_data.move_to_saturation_command_data.
					   a_transition_time ),2 );
			m_8bit_temporary_data1 = m_To_Lighting_Saturation_Data;
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Move_To_Saturation_Call_Back );

			break;

		   /* setting the move saturation mode */
		case g_Move_Saturation_Command_Event_c:

			m_Saturation_Rate = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                  move_hue_command_data.rate;
			m_Saturation_Step_Mode = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                       move_hue_command_data.move_mode;

			if ( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				   move_saturation_command_data.move_mode == g_Stop_c )
			{
			   tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c );

			}

			else if ((((Lighting_Event *)(pActionHandler->pEventData))->event_data.
					   move_saturation_command_data.move_mode == g_Up_c ) ||
				     (((Lighting_Event *)(pActionHandler->pEventData))->event_data.
					    move_saturation_command_data.move_mode == g_Down_c ) )
			{
			 tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			  				  g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
			  				  App_Move_Saturation_Call_Back );

			}

			break;

		   /* setting the step saturation mode */
		case g_Step_Saturation_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
												m_Lighting_End_Point,
												g_COLOR_CONTROL_CLUSTER_c,
												&p_cluster_definition );
 
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
										p_cluster_definition->p_cluster_info,
					g_CURRENT_SATURATION_ATTRIBUTE_c,
					&m_Lighting_Saturation_Data );

			m_8bit_temporary_data1 = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
									   step_saturation_command_data.step_mode;
			m_8bit_temporary_data2 = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                       step_saturation_command_data.step_size;
			m_Transition_Time = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                  step_saturation_command_data.transition_time;
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;
		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Step_Saturation_Call_Back );



			break;

		   /* setting the move to hue and saturation mode */
		case g_Move_To_Hue_And_Saturation_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											   m_Lighting_End_Point,
											   g_COLOR_CONTROL_CLUSTER_c,
											   &p_cluster_definition );
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
					
				   p_cluster_definition->p_cluster_info,
				   g_CURRENT_HUE_ATTRIBUTE_c,
				   &m_Lighting_To_Hue_Value );

			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,
					App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											   m_Lighting_End_Point,
											   g_COLOR_CONTROL_CLUSTER_c,
											   &p_cluster_definition ),
				   p_cluster_definition->p_cluster_info,
				   g_CURRENT_SATURATION_ATTRIBUTE_c,
				   &m_Lighting_Saturation_Data );
			m_move_to_hue_value = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                    move_to_hue_and_saturation_command_data.hue;
			m_move_to_saturation_value = ((Lighting_Event *)(pActionHandler->pEventData))->
				                     event_data.move_to_hue_and_saturation_command_data.saturation;
			memUtils_memCopy ((uint8_t*) &m_Transition_Time, (uint8_t*)&(((Lighting_Event *)
					  (pActionHandler->pEventData))->event_data.
					   move_to_hue_and_saturation_command_data.a_transition_time ),2 );
			m_8bit_temporary_data1 = m_Lighting_To_Hue_Value;
			m_8bit_temporary_data2 = m_Lighting_Saturation_Data;
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Move_Hue_And_Saturation_Call_Back );


			break;


		   /* setting the move to color tepmerature mode */
		case g_Move_To_Color_Temperature_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											   m_Lighting_End_Point,
											   g_COLOR_CONTROL_CLUSTER_c,
											   &p_cluster_definition );
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
					
					 p_cluster_definition->p_cluster_info,
					 g_COLOR_TEMPERATURE_ATTRIBUTE_c,
					 (uint8_t*)&m_color_temperature_data );
			memUtils_memCopy ((uint8_t*) &m_color_temperature,(uint8_t*)&(((Lighting_Event *)
				     (pActionHandler->pEventData))->event_data.
				 move_to_color_temperature_command_data.color_temperature ),2);
			 memUtils_memCopy ((uint8_t*) &m_Transition_Time,(uint8_t*)&(((Lighting_Event *)
					   (pActionHandler->pEventData))->event_data.
				 move_to_color_temperature_command_data.a_transition_time ),2 );
			m_16bit_temporary_data1 = m_color_temperature_data;
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Move_To_Color_temperature_Call_Back );
			break;


		   /* setting the move to color mode */
		case g_Move_To_Color_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											   m_Lighting_End_Point,
											   g_COLOR_CONTROL_CLUSTER_c,
											   &p_cluster_definition );
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
					
					p_cluster_definition->p_cluster_info,
					g_CURRENT_X_ATTRIBUTE_c,
					(uint8_t*)&m_move_to_color_x );
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											   m_Lighting_End_Point,
											   g_COLOR_CONTROL_CLUSTER_c,
											   &p_cluster_definition );
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
					
					p_cluster_definition->p_cluster_info,
					g_CURRENT_Y_ATTRIBUTE_c,
					(uint8_t*)&m_move_to_color_y );
			memUtils_memCopy((uint8_t*) &m_color_x,(uint8_t*)&(((Lighting_Event *)
					 (pActionHandler->pEventData))->event_data.
			         move_to_color_command_data.color_x ),2 );
			memUtils_memCopy ((uint8_t*) &m_color_y,(uint8_t*)&(((Lighting_Event *)
					  (pActionHandler->pEventData))->event_data.
			          move_to_color_command_data.color_y ),2 );
			memUtils_memCopy((uint8_t*) &m_Transition_Time,(uint8_t*)&(((Lighting_Event *)
					 (pActionHandler->pEventData))->event_data.
			         move_to_color_command_data.a_transition_time ),2 );
			m_16bit_temporary_data1 = m_move_to_color_x;
			m_16bit_temporary_data2 = m_move_to_color_y;
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Move_To_Color_Call_Back );
			break;

		   /* setting the move color mode */
		case g_Move_Color_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
																	 m_Lighting_End_Point,
																	 g_COLOR_CONTROL_CLUSTER_c,
																	 &p_cluster_definition );

			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
										   										   p_cluster_definition->p_cluster_info,
										   g_CURRENT_X_ATTRIBUTE_c,
										   (uint8_t*)&m_move_to_color_x );
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
																	   m_Lighting_End_Point,
																	   g_COLOR_CONTROL_CLUSTER_c,
																	   &p_cluster_definition ); 

			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
											
											p_cluster_definition->p_cluster_info,
											g_CURRENT_Y_ATTRIBUTE_c,
											(uint8_t*)&m_move_to_color_y );
			memUtils_memCopy ((uint8_t*) &m_rate_x,(uint8_t*)&(((Lighting_Event *)
				      (pActionHandler->pEventData))->event_data.move_color_command_data.rate_x),2 );
			memUtils_memCopy ((uint8_t*) &m_rate_y,(uint8_t*)&(((Lighting_Event *)
					  (pActionHandler->pEventData))->event_data.move_color_command_data.rate_y),2 );

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Move_Color_Call_Back );

			break;

		   /* setting the step color mode */
		case g_Step_Color_Command_Event_c:
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										       m_Lighting_End_Point,
										       g_COLOR_CONTROL_CLUSTER_c,
										       &p_cluster_definition );
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
					
					p_cluster_definition->p_cluster_info,
					g_CURRENT_X_ATTRIBUTE_c,
					(uint8_t*)&m_move_to_color_x );
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											   m_Lighting_End_Point,
											   g_COLOR_CONTROL_CLUSTER_c,
											   &p_cluster_definition );
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
					
					p_cluster_definition->p_cluster_info,
					g_CURRENT_Y_ATTRIBUTE_c,
					(uint8_t*)&m_move_to_color_y );
			memUtils_memCopy ((uint8_t*) &m_step_color_x,(uint8_t*)&(((Lighting_Event *)
					  (pActionHandler->pEventData))->event_data.step_color_command_data.step_x ),2);
			memUtils_memCopy ((uint8_t*) &m_step_color_y,(uint8_t*)&(((Lighting_Event *)
					  (pActionHandler->pEventData))->event_data.step_color_command_data.step_y ),2);
			memUtils_memCopy ((uint8_t*) &m_Transition_Time,(uint8_t*)&(((Lighting_Event *)
					  (pActionHandler->pEventData))->event_data.step_color_command_data.
					   a_transition_time ),2 );
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;
		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Step_Color_Call_Back );

			break;
			#if(g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d==1)

			/*setting the enhanced move to hue mode */
		case g_Enhanced_Move_To_Hue_Command_Event_c :
			/*Read enhaced current hue value */
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition );
 
			ZCL_Read_Write_Attribute_Data (g_Read_Attribute_Data_c,ptr,
			   			   p_cluster_definition->p_cluster_info,
			   g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
			  (uint8_t*)&m_Enhanced_Lighting_Hue_Data);

			m_16bit_temporary_data1 = m_Enhanced_Lighting_Hue_Data;
			/*Get target enhanced hue value */
			memUtils_memCopy((uint8_t*) &m_Enhanced_Hue_Value, (uint8_t*)&(((Lighting_Event *)
				  (pActionHandler->pEventData))->event_data.
				   enhanced_move_to_hue_command_data.Enhanced_hue ), 2);

			/*Get diection field*/

			m_Hue_Direction = ((Lighting_Event *)(pActionHandler->pEventData))->
							 event_data.enhanced_move_to_hue_command_data.direction;

			/*Get transition time */
			memUtils_memCopy((uint8_t*) &m_Transition_Time, (uint8_t*)&(((Lighting_Event *)
				  (pActionHandler->pEventData))->event_data.
				   enhanced_move_to_hue_command_data.a_transition_time ), 2);
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;

			if ( (((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			  enhanced_move_to_hue_command_data.direction == g_Move_To_Hue_Up_c ) ||
			( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			 enhanced_move_to_hue_command_data.direction == g_Shortest_Distance_c ) ||
			( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			 enhanced_move_to_hue_command_data.direction == g_Longest_Distance_c ) ||
			( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			 enhanced_move_to_hue_command_data.direction == g_Move_To_Hue_Down_c ) )

			{

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
			App_Enhanced_Move_To_Hue_Call_Back );

			}
			break ;


		/* setting the enhanced  move hue mode */
		case g_Enhanced_Move_Hue_Command_Event_c:

			/*Get rate value */
				memUtils_memCopy((uint8_t*) &m_Enhanced_Hue_Rate, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											enhanced_move_hue_command_data.rate ), 2);

			/*Get move mode */
			m_Hue_Step_Mode = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                enhanced_move_hue_command_data.move_mode;
			m_Lighting_End_Point = endpoint;
			if ( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.enhanced_move_hue_command_data.
				   move_mode == g_Stop_c ){

			  tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c );

			}




			else if ((((Lighting_Event *)(pActionHandler->pEventData))->event_data.
					     enhanced_move_hue_command_data.move_mode == g_Up_c ) ||
				     (((Lighting_Event *)(pActionHandler->pEventData))->event_data.
					    enhanced_move_hue_command_data.move_mode == g_Down_c ) )	{
			  //mr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
				//				  g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
				//				  App_Enhanced_Move_Hue_Call_Back );

			}
			break;


           /* setting the step hue mode */
		case g_Enhanced_Step_Hue_Command_Event_c:
			/*Get enhanced current hue value */
				ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
												m_Lighting_End_Point,
												g_COLOR_CONTROL_CLUSTER_c,
												&p_cluster_definition );
				ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
					
					p_cluster_definition->p_cluster_info,
					g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
					(uint8_t*)&m_Enhanced_Lighting_Hue_Data );
			/*Get step mode */
				m_8bit_temporary_data1 = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                       enhanced_step_hue_command.step_mode;

			/*Step size */
				memUtils_memCopy((uint8_t*) &m_16bit_temporary_data1, (uint8_t*)&(((Lighting_Event *)
				  (pActionHandler->pEventData))->event_data.
				   enhanced_step_hue_command.step_size ), 2);
			/*Transition time */
					memUtils_memCopy((uint8_t*) &m_Transition_Time, (uint8_t*)&(((Lighting_Event *)
				  (pActionHandler->pEventData))->event_data.
				   enhanced_step_hue_command.transition_time ), 2);
				m_transition_time =  m_Transition_Time = m_Transition_Time/10;

				m_16bit_temporary_data3= m_Enhanced_Lighting_Hue_Data;

			tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
				g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
				App_Enhanced_Step_Hue_Call_Back );


			break;

		  /* setting the move color temperature  mode */
		case g_Move_Color_Temperature_Event_c:

			/*Get rate value */
				memUtils_memCopy((uint8_t*) &m_Color_Temperatutre_Rate, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											move_color_temperature.rate ), 2);

			/*Get move mode */
			m_Color_Temperature_Mode = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                move_color_temperature.move_mode;
			m_Lighting_End_Point = endpoint;

			/*Get color temperature minimum field */
				memUtils_memCopy((uint8_t*) &m_Color_Temperature_Minimum, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											move_color_temperature.color_temp_min ), 2);

			/*Get color temperature maximum field */
				memUtils_memCopy((uint8_t*) &m_Color_Temperature_Maximum, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											move_color_temperature.color_temp_max ), 2);


			if ( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.move_color_temperature.
				   move_mode == g_Stop_c ){

			 tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c );

			}

			else if ((((Lighting_Event *)(pActionHandler->pEventData))->event_data.
					     move_color_temperature.move_mode == g_Up_c ) ||
				     (((Lighting_Event *)(pActionHandler->pEventData))->event_data.
					    move_color_temperature.move_mode == g_Down_c ) )	{
			  //mr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
				//				  g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
				//				  App_Move_Color_Temperature_Call_Back );

			}
			break;


           /* setting the step color temperature mode */
		case g_Step_Color_Temperature_Event_c:

			/*Get color temperature attribute value */
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
												m_Lighting_End_Point,
												g_COLOR_CONTROL_CLUSTER_c,
												&p_cluster_definition );
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
										p_cluster_definition->p_cluster_info,
					g_COLOR_TEMPERATURE_ATTRIBUTE_c,
					(uint8_t *)&m_color_temperature_data );

			/*Step mode*/

			m_8bit_temporary_data1 = ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
				                       step_color_temperature.step_mode;
			/*step size */
				memUtils_memCopy((uint8_t*) &m_16bit_temporary_data1, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											step_color_temperature.step_size ), 2);

			/*transition time */
			memUtils_memCopy((uint8_t*) &m_Transition_Time, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											step_color_temperature.transition_time ), 2);
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;
			/*color temperature minimum */
			memUtils_memCopy((uint8_t*) &m_Color_Temperature_Minimum, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											step_color_temperature.color_temp_min ), 2);

			/*color temperature maximum */
			memUtils_memCopy((uint8_t*) &m_Color_Temperature_Maximum, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											step_color_temperature.color_temp_max ), 2);

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			  g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
			  App_Step_Color_Temperature_Call_Back );


			break;

		//*setting stop mode */
		case g_Stop_Move_Step_Event_c :

		  /*check color loop active attribute */
		  if(0x01!= m_Color_Loop_Active_Data)
			 {


			/*stop timer */

			tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
			/*store current hue value*/
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								   m_Lighting_End_Point,
								   g_COLOR_CONTROL_CLUSTER_c,
								   &p_cluster_definition );
			ZCL_Read_Write_Attribute_Data (
		 	g_Write_Attribute_Data_c,ptr,
			 p_cluster_definition->p_cluster_info,
		 	g_CURRENT_HUE_ATTRIBUTE_c,
		 	&m_Lighting_Hue_Data );

		 /*store current saturation value*/
		ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								  m_Lighting_End_Point,
								  g_COLOR_CONTROL_CLUSTER_c,
								  &p_cluster_definition );

		 ZCL_Read_Write_Attribute_Data
		(
		g_Write_Attribute_Data_c,ptr,
				p_cluster_definition->p_cluster_info,
		g_CURRENT_SATURATION_ATTRIBUTE_c,
		&m_Lighting_Saturation_Data) ;


		/*store enhanced current hue value */
		ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								   m_Lighting_End_Point,
								   g_COLOR_CONTROL_CLUSTER_c,
								   &p_cluster_definition );
		ZCL_Read_Write_Attribute_Data (
		 g_Write_Attribute_Data_c,ptr,

		 		 p_cluster_definition->p_cluster_info,
		 g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
		 (uint8_t*)&m_Enhanced_Lighting_Hue_Data );

		 /*set remaining time to zero */
			ptr = 				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition );
		 	ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,

				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				g_ZERO_c);
			 }

			break ;
			/*setting enhanced move to hue and saturation */
		case g_Enhanced_Move_To_Hue_And_Saturation_Command_Event_c :

			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											   m_Lighting_End_Point,
											   g_COLOR_CONTROL_CLUSTER_c,
											   &p_cluster_definition );
			/*enhanced current hue attribute */
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
					
				   p_cluster_definition->p_cluster_info,
				   g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
				  (uint8_t*) &m_Enhanced_Lighting_Hue_Data );
			ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											   m_Lighting_End_Point,
											   g_COLOR_CONTROL_CLUSTER_c,
											   &p_cluster_definition );
			/*current saturation attribute */
			ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,

				   p_cluster_definition->p_cluster_info,
				   g_CURRENT_SATURATION_ATTRIBUTE_c,
				   &m_Lighting_Saturation_Data );

			/* target enhanced hue value */
			memUtils_memCopy((uint8_t*) &m_move_to_enhanced_hue_value, (uint8_t*)&(((Lighting_Event *)
										(pActionHandler->pEventData))->event_data.
											enhanced_move_to_hue_and_saturation_command.Enhanced_hue ), 2);
			/*target saturation value */
			m_move_to_saturation_value = ((Lighting_Event *)(pActionHandler->pEventData))->
				                     event_data.enhanced_move_to_hue_and_saturation_command.saturation;

			/*tarnsition time */
			memUtils_memCopy ((uint8_t*) &m_Transition_Time, (uint8_t*)&(((Lighting_Event *)
					  (pActionHandler->pEventData))->event_data.
					   enhanced_move_to_hue_and_saturation_command.a_transition_time ),2 );

			m_16bit_temporary_data1 = m_Enhanced_Lighting_Hue_Data;
			m_8bit_temporary_data1 = m_Lighting_Saturation_Data;
			m_transition_time =  m_Transition_Time = m_Transition_Time/10;

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Enhanced_Move_To_Hue_And_Saturation_Call_Back );


			break;

		/*setting color loop set mode */
		case g_Color_Loop_Set_Event_c :
		ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ); 
		/*enhanced current hue */
		 ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
			  
			  p_cluster_definition->p_cluster_info,
			  g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
			  (uint8_t*)&m_Enhanced_Lighting_Hue_Data );

		/*color loop active attribute */
		ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
		 ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_ACTIVE_ATTRIBUTE_c,
			  &m_Color_Loop_Active_Data );
		ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
		/*color loop stored enhanced hue */
		ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
			  
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_STORE_ENHANCED_HUE_c,
			  (uint8_t*)&m_Color_Loop_Store_Hue_Data );
		ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
		/*color loop time */
		ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_TIME_ATTRIBUTE_c,
			  (uint8_t*)&m_Color_Loop_Time_Data );
		ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
		/*color loop start enhanced hue */
		ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_START_ENHANCED_HUE_c,
			  (uint8_t*)&m_Color_Loop_Start_Hue_Data );



		/*get update flag */
		m_Update_Flags= ((Lighting_Event *)(pActionHandler->pEventData))->
							 event_data.color_loop_set.update_flags;

		/*get action field */
		m_Action=((Lighting_Event *)(pActionHandler->pEventData))->
							 event_data.color_loop_set.action;

		/*direction field */
        m_Hue_Direction =((Lighting_Event *)(pActionHandler->pEventData))->
							 event_data.color_loop_set.direction;

		/*transition time */
		memUtils_memCopy((uint8_t*) &m_Transition_Time, (uint8_t*)&(((Lighting_Event *)
				  (pActionHandler->pEventData))->event_data.
				   color_loop_set.a_transition_time ), 2);

		/*start hue */
		memUtils_memCopy((uint8_t*) &m_Start_Hue, (uint8_t*)&(((Lighting_Event *)
				  (pActionHandler->pEventData))->event_data.
				   color_loop_set.start_hue ), 2);

		if(m_Update_Flags&UPDATE_DIRECTION)
		{
		 ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
		 /* set color loop diection attribute with value of direction field */
		 ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_DIRECTION_ATTRIBUTE_c,
			  &m_Hue_Direction );

		}

		if (m_Update_Flags&UPDATE_TIME)
		 {
		   ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
		    /* set color loop time  attribute with value of time field */
		 ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_TIME_ATTRIBUTE_c,
			  (uint8_t*)&m_Transition_Time );
		 }

		 if(m_Update_Flags&UPDATE_START_HUE)
		  {
		   ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
		      /* set color loop time  attribute with value of time field */
		 ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_START_ENHANCED_HUE_c,
			  (uint8_t*)&m_Start_Hue );

			}

			 if(m_Update_Flags&UPDATE_ACTION)
			 {
			   /*check action field and color loop active attribute value */
			   if((m_Action==0x00)&&(m_Color_Loop_Active_Data==0x01) )
			    {
				 /*set color loop active attribute to 0x00 */
				  m_Color_Loop_Active_Data=0x00 ;
				   ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
				   ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,
							p_cluster_definition->p_cluster_info,
								g_COLOR_LOOP_ACTIVE_ATTRIBUTE_c,
									&m_Color_Loop_Active_Data );

				/*set the enhanced current hue attribute value to color loop stored enhanced hue */
					ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
					ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
			  (uint8_t*)&m_Color_Loop_Store_Hue_Data );

			  }




			  else if (( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			  color_loop_set.action ==0x01 )||(((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			  color_loop_set.action ==0x02 ) &&
			( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			 color_loop_set.direction == g_Move_Hue_Up_c ) ||
			( ((Lighting_Event *)(pActionHandler->pEventData))->event_data.
			 color_loop_set.direction == g_Move_Hue_Down_c ) )

			  {





				m_transition_time=m_Transition_Time=m_Transition_Time/10 ;

				switch(((Lighting_Event *)(pActionHandler->pEventData))->event_data.
								color_loop_set.action)

								{

					              case ACTIVE_MODE_1 :


					 /*set the color loop stored attribute value to  enhanced current hue */
					ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
					ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_STORE_ENHANCED_HUE_c,
			  (uint8_t*)&m_Enhanced_Lighting_Hue_Data );

			   /*set color loop active attribute to 0x01 */
			  m_Color_Loop_Active_Data=0x01 ;



				/*start from color loop start enhanced hue value */
						 ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
						 ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
										p_cluster_definition->p_cluster_info,
										g_COLOR_LOOP_START_ENHANCED_HUE_c,
												(uint8_t*)&m_Enhanced_Lighting_Hue_Data );



					break ;

				case ACTIVE_MODE_2 :


					/*set the color loop stored attribute value to  enhanced current hue */
					ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
					ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,
			  p_cluster_definition->p_cluster_info,
			  g_COLOR_LOOP_STORE_ENHANCED_HUE_c,
			  (uint8_t*)&m_Enhanced_Lighting_Hue_Data );

			   /*set color loop active attribute to 0x01 */
			  m_Color_Loop_Active_Data=0x01 ;



				/*start from color loop start enhanced hue value */
						ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
						 ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,ptr,
										p_cluster_definition->p_cluster_info,
										g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
												(uint8_t*)&m_Enhanced_Lighting_Hue_Data );


					}

				/*set color loop active data attribute */
			   ptr = App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition );
			   ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,ptr,
							p_cluster_definition->p_cluster_info,
								g_COLOR_LOOP_ACTIVE_ATTRIBUTE_c,
									&m_Color_Loop_Active_Data );
				/*call back */
			tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
					g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
								App_Color_Loop_Set_Call_Back );


					}
			 }


					break ;

#endif /*g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
			
			
			
			
			
			
	}
}


/***************************************************************************************/

void App_Move_Hue_Call_Back ( uint8_t timer_id ){

	ZCL_Read_Write_Attribute_Data (
		 g_Read_Attribute_Data_c,

		 App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								   m_Lighting_End_Point,
								   g_COLOR_CONTROL_CLUSTER_c,
								   &p_cluster_definition ),
		 p_cluster_definition->p_cluster_info,
		 g_CURRENT_HUE_ATTRIBUTE_c,
		 &m_Lighting_Hue_Data );
		if ( m_Hue_Step_Mode == g_Up_c )
		{
			if( ( 0xFE - m_Lighting_Hue_Data ) > m_Hue_Rate )
			{
			 m_Lighting_Hue_Data = m_Lighting_Hue_Data + m_Hue_Rate;
			}
			else
			{
			 m_Lighting_Hue_Data = 0xFE;
			 m_Hue_Step_Mode = g_Down_c;
			}
		}
		else if ( m_Hue_Step_Mode == g_Down_c )
		{
			if( m_Lighting_Hue_Data > m_Hue_Rate )
			{
			 m_Lighting_Hue_Data = m_Lighting_Hue_Data - m_Hue_Rate;
			}
			else
			{
			 m_Lighting_Hue_Data = 0x00;
			 m_Hue_Step_Mode = g_Up_c;
			}
		}
		ZCL_Read_Write_Attribute_Data
		(
		g_Write_Attribute_Data_c,
		App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								  m_Lighting_End_Point,
								  g_COLOR_CONTROL_CLUSTER_c,
								  &p_cluster_definition ),
		p_cluster_definition->p_cluster_info,
		g_CURRENT_HUE_ATTRIBUTE_c,
		&m_Lighting_Hue_Data
		);

		if ( m_Lighting_Hue_Data == 0x00 || m_Lighting_Hue_Data == 0xFE )
		{
		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		}

	tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
						   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
						   App_Move_Hue_Call_Back );
}


/***************************************************************************************/

void App_Move_Saturation_Call_Back ( uint8_t timer_id )
{

		ZCL_Read_Write_Attribute_Data
		(
		g_Read_Attribute_Data_c,
		App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								  m_Lighting_End_Point,
								  g_COLOR_CONTROL_CLUSTER_c,
								  &p_cluster_definition ),
		p_cluster_definition->p_cluster_info,
		g_CURRENT_SATURATION_ATTRIBUTE_c,
		&m_Lighting_Saturation_Data

		);
		if ( m_Saturation_Step_Mode == g_Up_c ){
		if( ( 0xFE - m_Lighting_Saturation_Data ) > m_Saturation_Rate )	{
		 m_Lighting_Saturation_Data = m_Lighting_Saturation_Data + m_Saturation_Rate;
		}
		else{
		 m_Lighting_Saturation_Data = 0xFE;
		 m_Saturation_Step_Mode = g_Down_c;
		}
		}
		else if ( m_Saturation_Step_Mode == g_Down_c ){
		if( m_Lighting_Saturation_Data > m_Saturation_Rate ){
		 m_Lighting_Saturation_Data = m_Lighting_Saturation_Data - m_Saturation_Rate;
		}
		else{
		 m_Lighting_Saturation_Data = 0x00;
		 m_Saturation_Step_Mode = g_Up_c;
		}
		}
		ZCL_Read_Write_Attribute_Data
		(
		g_Write_Attribute_Data_c,
		App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								  m_Lighting_End_Point,
								  g_COLOR_CONTROL_CLUSTER_c,
								  &p_cluster_definition ),
		p_cluster_definition->p_cluster_info,
		g_CURRENT_SATURATION_ATTRIBUTE_c,
		&m_Lighting_Saturation_Data
   		);

		if ( ( m_Lighting_Saturation_Data == 0x00 ) || ( m_Lighting_Saturation_Data == 0xFE) ){
		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		return;
		}

	tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
						   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
						   App_Move_Saturation_Call_Back );

}

/***************************************************************************************/

void App_Move_To_Hue_Call_Back ( uint8_t timer_id )
{

		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;


		if ( m_Hue_Direction == g_Shortest_Distance_c )	{
			if((m_Lighting_Hue_Data  > m_Hue_Value)    &&
			   ((m_8bit_temporary_data1 - m_Hue_Value) < 0x7F )  )	{
			 m_Lighting_Hue_Data = m_8bit_temporary_data1 - (((m_8bit_temporary_data1 - m_Hue_Value)
								   * Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Lighting_Hue_Data < m_Hue_Value ) &&
					( (m_Hue_Value - m_8bit_temporary_data1) <= 0x7F)) {
			m_Lighting_Hue_Data = m_8bit_temporary_data1 + (((m_Hue_Value - m_8bit_temporary_data1 )
								  *Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Lighting_Hue_Data < m_Hue_Value ) &&
					( (m_Hue_Value - m_8bit_temporary_data1) > 0x7F)) {
			m_Lighting_Hue_Data = m_8bit_temporary_data1 - (((m_8bit_temporary_data1 + (0x0100 -
								  m_Hue_Value))* Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Lighting_Hue_Data > m_Hue_Value ) &&
					( (m_8bit_temporary_data1 - m_Hue_Value) >= 0x7F))	{
			m_Lighting_Hue_Data = m_8bit_temporary_data1 + ((((0x0100 - m_8bit_temporary_data1) +
								  m_Hue_Value)* Color_Control_Timer_Count)/m_transition_time);
			}
		}


		else if ( m_Hue_Direction == g_Longest_Distance_c )
		{
			if((m_Lighting_Hue_Data  > m_Hue_Value)    &&
			   ((m_8bit_temporary_data1 - m_Hue_Value) <= 0x7F )  )	{
			 m_Lighting_Hue_Data = m_8bit_temporary_data1 + (((0x0100 - m_8bit_temporary_data1 +
								   m_Hue_Value)* Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Lighting_Hue_Data < m_Hue_Value ) &&
					( (m_Hue_Value - m_8bit_temporary_data1) < 0x7F)) {
			m_Lighting_Hue_Data = m_8bit_temporary_data1 - (((0x0100 - m_Hue_Value +
							m_8bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Lighting_Hue_Data < m_Hue_Value ) &&
					( (m_Hue_Value - m_8bit_temporary_data1) >= 0x7F))	{
			m_Lighting_Hue_Data = m_8bit_temporary_data1 + (((m_Hue_Value - m_8bit_temporary_data1 )
								  * Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Lighting_Hue_Data > m_Hue_Value ) &&
					( (m_8bit_temporary_data1 - m_Hue_Value) > 0x7F)) {
			m_Lighting_Hue_Data = m_8bit_temporary_data1 - (((m_8bit_temporary_data1 - m_Hue_Value)
								  * Color_Control_Timer_Count)/m_transition_time);
			}
		}

		else if ( m_Hue_Direction == g_Move_To_Hue_Up_c ) {
			if(  m_Lighting_Hue_Data  > m_Hue_Value) {
			 m_Lighting_Hue_Data = m_8bit_temporary_data1 + (((0x0100 - m_8bit_temporary_data1 +
								   m_Hue_Value)* Color_Control_Timer_Count)/m_transition_time);
			}
			else if( m_Lighting_Hue_Data < m_Hue_Value ) {
			m_Lighting_Hue_Data = m_8bit_temporary_data1 + ((( m_Hue_Value - m_8bit_temporary_data1)
								  *Color_Control_Timer_Count)/m_transition_time);
			}
		}

		else if ( m_Hue_Direction == g_Move_To_Hue_Down_c )	{
			if(  m_Lighting_Hue_Data  > m_Hue_Value) {
			 m_Lighting_Hue_Data = m_8bit_temporary_data1 - (((m_8bit_temporary_data1 - m_Hue_Value)
								   * Color_Control_Timer_Count)/m_transition_time);
			}
			else if( m_Lighting_Hue_Data < m_Hue_Value ) {
			m_Lighting_Hue_Data = m_8bit_temporary_data1 - ((( 0x0100 - m_Hue_Value +
					         m_8bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time);
			}
		}


		ZCL_Read_Write_Attribute_Data (	g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_HUE_ATTRIBUTE_c,
				&m_Lighting_Hue_Data
				);


		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time);


		if( Color_Control_Timer_Count >= m_transition_time ){

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		return;
		}




	tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
						   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
						   App_Move_To_Hue_Call_Back );



}

/******************************************************************************************/

void App_Move_To_Saturation_Call_Back ( uint8_t timer_id )
{
		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		if(m_Lighting_Saturation_Value > m_8bit_temporary_data1 ){
		m_To_Lighting_Saturation_Data =m_8bit_temporary_data1 + ((m_Lighting_Saturation_Value -
							m_8bit_temporary_data1) *Color_Control_Timer_Count)/m_transition_time;

		}
		else if(m_Lighting_Saturation_Value < m_8bit_temporary_data1){
		m_To_Lighting_Saturation_Data =m_8bit_temporary_data1 - ((m_8bit_temporary_data1 -
					  m_Lighting_Saturation_Value ) *Color_Control_Timer_Count)/m_transition_time;
		}

		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										   m_Lighting_End_Point,
										   g_COLOR_CONTROL_CLUSTER_c,
										   &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_SATURATION_ATTRIBUTE_c,
				&m_To_Lighting_Saturation_Data );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				 App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
											m_Lighting_End_Point,
											g_COLOR_CONTROL_CLUSTER_c,
											&p_cluster_definition ),
				 p_cluster_definition->p_cluster_info,
				 g_REMAINING_TIME_ATTRIBUTE_c,
				 (uint8_t*)&Remaining_Time );


		if( (Color_Control_Timer_Count >= m_transition_time) ||
		   (m_Lighting_Saturation_Value == m_8bit_temporary_data1) ){

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		return;
		}

	tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
						   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
						   App_Move_To_Saturation_Call_Back );
}

/***************************************************************************************/

void App_Move_Hue_And_Saturation_Call_Back ( uint8_t timer_id )
{

		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		if(  (m_8bit_temporary_data1  > m_move_to_hue_value)    &&
		   ((m_8bit_temporary_data1 - m_move_to_hue_value) < 0x7F )  )
		{
		 m_Lighting_To_Hue_Value = m_8bit_temporary_data1 - (((m_8bit_temporary_data1 -
								m_move_to_hue_value)* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data1 < m_move_to_hue_value ) &&
				( (m_move_to_hue_value - m_8bit_temporary_data1) <= 0x7F))
		{
		m_Lighting_To_Hue_Value = m_8bit_temporary_data1 + (((m_move_to_hue_value -
	                         m_8bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data1 < m_move_to_hue_value ) &&
				( (m_move_to_hue_value - m_8bit_temporary_data1) > 0x7F))
		{
		m_Lighting_To_Hue_Value = m_8bit_temporary_data1 - (((m_8bit_temporary_data1 + (0x0100 -
				               m_move_to_hue_value))* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data1 > m_move_to_hue_value ) &&
				( (m_8bit_temporary_data1 - m_move_to_hue_value) >= 0x7F))
		{
		m_Lighting_To_Hue_Value = m_8bit_temporary_data1 + ((((0x0100 - m_8bit_temporary_data1) +
			                    m_move_to_hue_value)* Color_Control_Timer_Count)/m_transition_time);
		}

		if(  (m_8bit_temporary_data2  > m_move_to_saturation_value)    &&
		   ((m_8bit_temporary_data2 - m_move_to_saturation_value) < 0x7F )  )
		{
		 m_Lighting_Saturation_Data = m_8bit_temporary_data2 - (((m_8bit_temporary_data2 -
						 m_move_to_saturation_value)* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data2 < m_move_to_saturation_value ) &&
				( (m_move_to_saturation_value - m_8bit_temporary_data2) <= 0x7F))
		{
		m_Lighting_Saturation_Data = m_8bit_temporary_data2 + (((m_move_to_saturation_value -
							 m_8bit_temporary_data2 )*Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data2 < m_move_to_saturation_value ) &&
				( (m_move_to_saturation_value - m_8bit_temporary_data2) > 0x7F))
		{
		m_Lighting_Saturation_Data = m_8bit_temporary_data2 - (((m_8bit_temporary_data2 + (0x0100 -
						m_move_to_saturation_value))* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data2 > m_move_to_saturation_value ) &&
				( (m_8bit_temporary_data2 - m_move_to_saturation_value) >= 0x7F))
		{
		m_Lighting_Saturation_Data = m_8bit_temporary_data2 + ((((0x0100 - m_8bit_temporary_data2) +
					     m_move_to_saturation_value)* Color_Control_Timer_Count)/m_transition_time);
		}



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
			  App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
			  p_cluster_definition->p_cluster_info,
			  g_CURRENT_HUE_ATTRIBUTE_c,
			  &m_Lighting_To_Hue_Value );

		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
			  App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
			  p_cluster_definition->p_cluster_info,
			  g_CURRENT_SATURATION_ATTRIBUTE_c,
			  &m_Lighting_Saturation_Data );

		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
			  App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
			  p_cluster_definition->p_cluster_info,
			  g_REMAINING_TIME_ATTRIBUTE_c,
			  (uint8_t*)&Remaining_Time );
		if( Color_Control_Timer_Count >= m_transition_time)	{

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c );
		  return;
		}


	tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
						   g_COLOR_CONTROL_TIME_OUT_DURATION_c * m_MILLISECONDS_c,
						   App_Move_Hue_And_Saturation_Call_Back );


}

/***************************************************************************************/

void App_Move_To_Color_Call_Back ( uint8_t timer_id )
{

		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		if(  (m_move_to_color_x  > m_color_x)    &&
		   ((m_16bit_temporary_data1 - m_color_x) < 0x7FFF )  )	{
		 m_move_to_color_x = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 - m_color_x)*
							 Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_move_to_color_x < m_color_x ) &&
				( (m_color_x - m_16bit_temporary_data1) <= 0x7FFF))	{
		m_move_to_color_x = m_16bit_temporary_data1 + (((m_color_x - m_16bit_temporary_data1 )
							*Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_move_to_color_x < m_color_x ) &&
				( (m_color_x - m_16bit_temporary_data1) > 0x7FFF)){
		m_move_to_color_x = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 + (0x10000 -
							m_color_x))* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_move_to_color_x > m_color_x ) &&
				( (m_16bit_temporary_data1 - m_color_x) >= 0x7FFF)){
		m_move_to_color_x = m_16bit_temporary_data1 + ((((0x10000 - m_16bit_temporary_data1) +
							m_color_x)* Color_Control_Timer_Count)/m_transition_time);
		}

		if(  (m_move_to_color_y  > m_color_y)    &&
			   ((m_16bit_temporary_data2 - m_color_y) < 0x7FFF )  )	{
		 m_move_to_color_y = m_16bit_temporary_data2 - (((m_16bit_temporary_data2 - m_color_y)*
							 Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_16bit_temporary_data2 < m_color_y ) &&
				( (m_color_y - m_16bit_temporary_data2) <= 0x7FFF))	{
		m_move_to_color_y = m_16bit_temporary_data2 + (((m_color_y - m_16bit_temporary_data2 )
							*Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_16bit_temporary_data2 < m_color_y ) &&
				( (m_color_y - m_16bit_temporary_data2) > 0x7FFF)) {
		m_move_to_color_y = m_16bit_temporary_data2 - (((m_16bit_temporary_data2 + (0x10000 -
							m_color_y))* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_16bit_temporary_data2 > m_color_y ) &&
				( (m_16bit_temporary_data2 - m_color_y) >= 0x7FFF))	{
		m_move_to_color_y = m_16bit_temporary_data2 + ((((0x10000 - m_16bit_temporary_data2) +
							m_color_y)* Color_Control_Timer_Count)/m_transition_time);
		}



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_X_ATTRIBUTE_c,
				(uint8_t*)&m_move_to_color_x );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_Y_ATTRIBUTE_c,
				(uint8_t*)&m_move_to_color_y );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time );



		if( Color_Control_Timer_Count >= m_transition_time ){

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c );
		return;
		}
		else{
	tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
						   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
						   App_Move_To_Color_Call_Back );
		}

}

/***************************************************************************************/

void App_Move_Color_Call_Back ( uint8_t timer_id ){
		m_move_to_color_x = m_move_to_color_x + m_rate_x;
		m_move_to_color_y = m_move_to_color_y + m_rate_y;

		ZCL_Read_Write_Attribute_Data( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_X_ATTRIBUTE_c,
				(uint8_t*)&m_move_to_color_x );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_Y_ATTRIBUTE_c,
				(uint8_t*)&m_move_to_color_y );

		if( m_rate_x == 0x0000 && m_rate_y == 0x0000 ){

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		}
		else {
	tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
	g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c, App_Move_Color_Call_Back );
	
  }

}

/***************************************************************************************/

void App_Step_Color_Call_Back ( uint8_t timer_id ){
		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		m_move_to_color_x = m_move_to_color_x + m_step_color_x;
		m_move_to_color_y = m_move_to_color_y + m_step_color_y;


		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_X_ATTRIBUTE_c,
				(uint8_t*)&m_move_to_color_x );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_Y_ATTRIBUTE_c,
				(uint8_t*)&m_move_to_color_y );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time );


		if( Color_Control_Timer_Count >= m_transition_time ){

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		  return;
		}
		else {
	tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
	                       g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
						   App_Step_Color_Call_Back );
		}

}

/***************************************************************************************/

void App_Move_To_Color_temperature_Call_Back ( uint8_t timer_id ) {

		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		if(m_16bit_temporary_data1 > m_color_temperature) {
		m_color_temperature_data = m_16bit_temporary_data1- ((m_16bit_temporary_data1 -
								 m_color_temperature)*Color_Control_Timer_Count)/m_transition_time;
		}
		else if(m_16bit_temporary_data1 < m_color_temperature){
		m_color_temperature_data = m_16bit_temporary_data1 + ((m_color_temperature -
							m_16bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time;
		}



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_COLOR_TEMPERATURE_ATTRIBUTE_c,
				(uint8_t*)&m_color_temperature_data );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										   m_Lighting_End_Point,
										   g_COLOR_CONTROL_CLUSTER_c,
										   &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time );

		if( Color_Control_Timer_Count >= m_transition_time ) {

			tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		}
		else {
		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Move_To_Color_temperature_Call_Back );
		}

}

/***************************************************************************************/

void App_Step_Saturation_Call_Back ( uint8_t timer_id )	{

		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		if ( m_8bit_temporary_data1 == g_Move_To_Saturation_Up_c ){
			if( ( 0xFE - m_Lighting_Saturation_Data ) > m_8bit_temporary_data2 ){
			  m_Lighting_Saturation_Data = m_Lighting_Saturation_Data + m_8bit_temporary_data2;
			}
			else{
			  m_Lighting_Saturation_Data = 0xFE;
			}
		}
		else if ( m_8bit_temporary_data1 == g_Move_To_Saturation_Down_c ){
			if( m_Lighting_Saturation_Data > m_8bit_temporary_data2 ){
			  m_Lighting_Saturation_Data = m_Lighting_Saturation_Data - m_8bit_temporary_data2;
			}
			else {
			  m_Lighting_Saturation_Data = 0x00;
			}
		}



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_SATURATION_ATTRIBUTE_c,
				&m_Lighting_Saturation_Data  );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										   m_Lighting_End_Point,
										   g_COLOR_CONTROL_CLUSTER_c,
										   &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time );

		if( Color_Control_Timer_Count >= m_transition_time ) {

			tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		}
		else {
		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Step_Saturation_Call_Back );
		}

}

/***************************************************************************************/

void App_Step_Hue_Call_Back ( uint8_t timer_id )
{
		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

        /*m_8bit_temporary_data1  holds the direction*/
		if ( m_8bit_temporary_data1 == g_Up_c )	{


			if( (0xFE - m_Lighting_Hue_Data ) >= m_8bit_temporary_data2) {
			  m_Lighting_Hue_Data = m_8bit_temporary_data3 + ((m_8bit_temporary_data2*Color_Control_Timer_Count)/m_transition_time);
			}
			else {
			  m_8bit_temporary_data2 = m_8bit_temporary_data2 - (0xfe - m_Lighting_Hue_Data);
			  m_Lighting_Hue_Data = 0xFE;
			  m_8bit_temporary_data3 = 0xfe;

			  m_8bit_temporary_data1 = g_Down_c;
			}
		}
        else{
			 if ( m_8bit_temporary_data1 == g_Down_c ){
				if( m_Lighting_Hue_Data >=m_8bit_temporary_data2){
				  m_Lighting_Hue_Data = m_8bit_temporary_data3 - ((m_8bit_temporary_data2*Color_Control_Timer_Count)/m_transition_time);
				}
				else {
				  m_8bit_temporary_data2 = m_8bit_temporary_data2 - m_Lighting_Hue_Data;
				  m_Lighting_Hue_Data = 0x00;
				  m_8bit_temporary_data3 = 0x00;
				  m_8bit_temporary_data1 = g_Up_c;
				}
			}
		}



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_CURRENT_HUE_ATTRIBUTE_c,
				&m_Lighting_Hue_Data  );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										   m_Lighting_End_Point,
										   g_COLOR_CONTROL_CLUSTER_c,
										   &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time);


	if( Color_Control_Timer_Count >= m_transition_time )
		{

		  tmr_stopTimer (  m_APPLICATION_LIGHTING_TIMER_ONE_c );

		}




		else
		{
            tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			 g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c, App_Step_Hue_Call_Back );

		}



}







/***************************************************************************************/
#if (g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )

void App_Enhanced_Move_To_Hue_Call_Back ( uint8_t timer_id )
{

		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;


		if ( m_Hue_Direction == g_Shortest_Distance_c )	{
			if((m_Enhanced_Lighting_Hue_Data  > m_Enhanced_Hue_Value)    &&
			   ((m_16bit_temporary_data1 - m_Enhanced_Hue_Value) < 0x7FFF )  )	{
			 m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 - m_Enhanced_Hue_Value)
								   * Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Enhanced_Lighting_Hue_Data < m_Enhanced_Hue_Value ) &&
					( (m_Enhanced_Hue_Value - m_16bit_temporary_data1) <= 0x7FFF)) {
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + (((m_Enhanced_Hue_Value - m_16bit_temporary_data1 )
								  *Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Enhanced_Lighting_Hue_Data < m_Enhanced_Hue_Value ) &&
					( (m_Hue_Value - m_16bit_temporary_data1) > 0x7FFF)) {
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 + (0x10000 -
								  m_Hue_Value))* Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Enhanced_Lighting_Hue_Data > m_Enhanced_Hue_Value ) &&
					( (m_16bit_temporary_data1 - m_Enhanced_Hue_Value) >= 0x7FFF))	{
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + ((((0x10000 - m_16bit_temporary_data1) +
								  m_Enhanced_Hue_Value)* Color_Control_Timer_Count)/m_transition_time);
			}
		}


		else if ( m_Hue_Direction == g_Longest_Distance_c )
		{
			if((m_Enhanced_Lighting_Hue_Data  > m_Enhanced_Hue_Value)    &&
			   ((m_16bit_temporary_data1 - m_Enhanced_Hue_Value) <= 0x7FFF )  )	{
			 m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + (((0x10000 - m_16bit_temporary_data1 +
								   m_Enhanced_Hue_Value)* Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Enhanced_Lighting_Hue_Data < m_Enhanced_Hue_Value ) &&
					( (m_Enhanced_Hue_Value - m_16bit_temporary_data1) < 0x7FFF)) {
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - (((0x10000 - m_Hue_Value +
							m_16bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Enhanced_Lighting_Hue_Data < m_Enhanced_Hue_Value ) &&
					( (m_Hue_Value - m_16bit_temporary_data1) >= 0x7FFF))	{
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + (((m_Enhanced_Hue_Value - m_16bit_temporary_data1 )
								  * Color_Control_Timer_Count)/m_transition_time);
			}
			else if( ( m_Enhanced_Lighting_Hue_Data > m_Enhanced_Hue_Value ) &&
					( (m_16bit_temporary_data1 - m_Enhanced_Hue_Value) > 0x7FFF)) {
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 - m_Enhanced_Hue_Value)
								  * Color_Control_Timer_Count)/m_transition_time);
			}
		}

		else if ( m_Hue_Direction == g_Move_To_Hue_Up_c ) {
			if(  m_Enhanced_Lighting_Hue_Data  > m_Enhanced_Hue_Value) {
			 m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + (((0x10000 - m_16bit_temporary_data1 +
								   m_Hue_Value)* Color_Control_Timer_Count)/m_transition_time);
			}
			else if( m_Enhanced_Lighting_Hue_Data < m_Enhanced_Hue_Value ) {
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + ((( m_Enhanced_Hue_Value - m_16bit_temporary_data1)
								  *Color_Control_Timer_Count)/m_transition_time);
			}
		}

		else if ( m_Hue_Direction == g_Move_To_Hue_Down_c )	{
			if(  m_Enhanced_Lighting_Hue_Data  > m_Enhanced_Hue_Value) {
			 m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 - m_Enhanced_Hue_Value)
								   * Color_Control_Timer_Count)/m_transition_time);
			}
			else if( m_Enhanced_Lighting_Hue_Data < m_Enhanced_Hue_Value ) {
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - ((( 0x10000 - m_Enhanced_Hue_Value +
					         m_16bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time);
			}
		}


		ZCL_Read_Write_Attribute_Data (	g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
				(uint8_t*)&m_Enhanced_Lighting_Hue_Data);



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time);


		if( Color_Control_Timer_Count >= m_transition_time ){

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		return;
		}




		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Enhanced_Move_To_Hue_Call_Back );



}
/***************************************************************************************************/
void App_Enhanced_Move_Hue_Call_Back ( uint8_t timer_id ){

	ZCL_Read_Write_Attribute_Data (
		 g_Read_Attribute_Data_c,

		 App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								   m_Lighting_End_Point,
								   g_COLOR_CONTROL_CLUSTER_c,
								   &p_cluster_definition ),
		 p_cluster_definition->p_cluster_info,
		 g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
		(uint8_t*) &m_Enhanced_Lighting_Hue_Data );
		if ( m_Hue_Step_Mode == g_Up_c )
		{
			if( ( 0xFFFE - m_Enhanced_Lighting_Hue_Data ) > m_Enhanced_Hue_Rate )
			{
			 m_Enhanced_Lighting_Hue_Data = m_Enhanced_Lighting_Hue_Data + m_Enhanced_Hue_Rate;
			}
			else
			{
			 m_Enhanced_Lighting_Hue_Data = 0xFFFE;
			 m_Hue_Step_Mode = g_Down_c;
			}
		}
		else if ( m_Hue_Step_Mode == g_Down_c )
		{
			if( m_Enhanced_Lighting_Hue_Data > m_Enhanced_Hue_Rate )
			{
			 m_Enhanced_Lighting_Hue_Data = m_Enhanced_Lighting_Hue_Data - m_Enhanced_Hue_Rate;
			}
			else
			{
			 m_Enhanced_Lighting_Hue_Data = 0x0000;
			 m_Hue_Step_Mode = g_Up_c;
			}
		}
		ZCL_Read_Write_Attribute_Data
		(
		g_Write_Attribute_Data_c,
		App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								  m_Lighting_End_Point,
								  g_COLOR_CONTROL_CLUSTER_c,
								  &p_cluster_definition ),
		p_cluster_definition->p_cluster_info,
		g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
		(uint8_t*)&m_Enhanced_Lighting_Hue_Data
		);

		if ( m_Hue_Step_Mode==g_Stop_c)
		{
		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		}

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Enhanced_Move_Hue_Call_Back );
}
/************************************************************************************************/
void App_Enhanced_Step_Hue_Call_Back ( uint8_t timer_id )
{
		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		if ( m_8bit_temporary_data1 == g_Up_c )	{
			if( ( 0xFFFE - m_Enhanced_Lighting_Hue_Data ) >= m_16bit_temporary_data1 ){
			  m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data3 + (m_16bit_temporary_data1*Color_Control_Timer_Count)/m_transition_time;
			}
			else {
			  	 m_16bit_temporary_data1 = m_16bit_temporary_data1 - (0xfffe - m_Enhanced_Lighting_Hue_Data);
			  	m_Enhanced_Lighting_Hue_Data = 0xFFFE;
			  	m_16bit_temporary_data3= 0xFFFE;
			  	m_8bit_temporary_data1 = g_Down_c;
			}
		}

		if(m_8bit_temporary_data1 == g_Down_c){
			if( m_Enhanced_Lighting_Hue_Data >= m_16bit_temporary_data1 ){
			  m_Enhanced_Lighting_Hue_Data = m_Enhanced_Lighting_Hue_Data
				- (m_16bit_temporary_data1*Color_Control_Timer_Count)/m_transition_time;
			}
			else
			{
				m_16bit_temporary_data1 = (m_16bit_temporary_data1 - m_Enhanced_Lighting_Hue_Data );
				m_Enhanced_Lighting_Hue_Data = 0x0000;
				m_16bit_temporary_data3 =0x0000;
				m_8bit_temporary_data1 = g_Up_c;
			}
		}



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
				(uint8_t*)&m_Enhanced_Lighting_Hue_Data  );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										   m_Lighting_End_Point,
										   g_COLOR_CONTROL_CLUSTER_c,
										   &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time);

		if( Color_Control_Timer_Count >= m_transition_time ){

		tmr_stopTimer (  m_APPLICATION_LIGHTING_TIMER_ONE_c );
		}
		else{
		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			 g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c, App_Enhanced_Step_Hue_Call_Back );
		}

}



/*************************************************************************************************/
void App_Move_Color_Temperature_Call_Back ( uint8_t timer_id ){
	/*get color temperature attribute value */
	ZCL_Read_Write_Attribute_Data (
		 g_Read_Attribute_Data_c,

		 App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								   m_Lighting_End_Point,
								   g_COLOR_CONTROL_CLUSTER_c,
								   &p_cluster_definition ),
		 p_cluster_definition->p_cluster_info,
		 g_COLOR_TEMPERATURE_ATTRIBUTE_c,
		 (uint8_t*)&m_color_temperature_data );
		 /*Check move mode */
		if ( m_Color_Temperature_Mode == g_Up_c )
		{
			if( ( m_Color_Temperature_Maximum - m_color_temperature_data ) > m_Color_Temperatutre_Rate )
			{
			 m_color_temperature_data = m_color_temperature_data + m_Color_Temperatutre_Rate;
			}
			else
			{
			 m_color_temperature_data = m_Color_Temperature_Maximum;
			 m_Color_Temperature_Mode = g_Down_c;
			}
		}
		else if ( m_Color_Temperature_Mode == g_Down_c )
		{
			if( m_color_temperature_data > m_Color_Temperatutre_Rate )
			{
			 m_color_temperature_data = m_color_temperature_data - m_Color_Temperatutre_Rate;
			}
			else
			{
			 m_color_temperature_data = m_Color_Temperature_Minimum;
			 m_Color_Temperature_Mode = g_Up_c;
			}
		}
		/*Update color temperature value */
		ZCL_Read_Write_Attribute_Data
		(
		g_Write_Attribute_Data_c,
		App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
								  m_Lighting_End_Point,
								  g_COLOR_CONTROL_CLUSTER_c,
								  &p_cluster_definition ),
		p_cluster_definition->p_cluster_info,
		g_COLOR_TEMPERATURE_ATTRIBUTE_c,
		(uint8_t*)&m_color_temperature_data
		);

		if ( m_color_temperature_data == m_Color_Temperature_Minimum || m_color_temperature_data == m_Color_Temperature_Maximum )
		{
		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		}

		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
							   App_Move_Color_Temperature_Call_Back );
}

/**************************************************************************************************/
void App_Step_Color_Temperature_Call_Back ( uint8_t timer_id )
{
		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		if( Color_Control_Timer_Count >= m_transition_time )
		{

			if ( m_8bit_temporary_data1 == g_Up_c )	{
				if( ( m_Color_Temperature_Maximum - m_color_temperature_data ) > m_16bit_temporary_data1){
			 	 m_color_temperature_data = m_color_temperature_data +( m_16bit_temporary_data1);
				}
				else {
				        /*else stop step operation */
				  		tmr_stopTimer (  m_APPLICATION_LIGHTING_TIMER_ONE_c );
					}
				}
			else if ( m_8bit_temporary_data1 == g_Down_c ){
				if( m_color_temperature_data > m_16bit_temporary_data1){
				  m_color_temperature_data = m_color_temperature_data - (m_16bit_temporary_data1);
				}
				else {
				       /*else stop step operation */
				  	tmr_stopTimer (  m_APPLICATION_LIGHTING_TIMER_ONE_c );

					}
			}



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_COLOR_TEMPERATURE_ATTRIBUTE_c,
				(uint8_t*)&m_color_temperature_data  );
		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										   m_Lighting_End_Point,
										   g_COLOR_CONTROL_CLUSTER_c,
										   &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time);



		tmr_stopTimer (  m_APPLICATION_LIGHTING_TIMER_ONE_c );
		}
		else{
		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
			 g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c, App_Step_Color_Temperature_Call_Back );
		}

}
/****************************************************************************************/
void App_Enhanced_Move_To_Hue_And_Saturation_Call_Back ( uint8_t timer_id )
{

		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;

		if(  (m_16bit_temporary_data1  > m_move_to_enhanced_hue_value)    &&
		   ((m_16bit_temporary_data1 - m_move_to_enhanced_hue_value) < 0x7FFF )  )
		{
		 m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 -
								m_move_to_enhanced_hue_value)* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_16bit_temporary_data1 < m_move_to_enhanced_hue_value ) &&
				( (m_move_to_enhanced_hue_value - m_16bit_temporary_data1) <= 0x7FFF))
		{
		m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + (((m_move_to_enhanced_hue_value -
	                         m_16bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_16bit_temporary_data1 < m_move_to_enhanced_hue_value ) &&
				( (m_move_to_enhanced_hue_value - m_16bit_temporary_data1) > 0x7FFF))
		{
		m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 + (0x10000 -
				               m_move_to_enhanced_hue_value))* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_16bit_temporary_data1 > m_move_to_enhanced_hue_value ) &&
				( (m_16bit_temporary_data1 - m_move_to_enhanced_hue_value) >= 0x7FFF))
		{
		m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + ((((0x10000 - m_16bit_temporary_data1) +
			                    m_move_to_enhanced_hue_value)* Color_Control_Timer_Count)/m_transition_time);
		}

		if(  (m_8bit_temporary_data1> m_move_to_saturation_value)    &&
		   ((m_8bit_temporary_data1 - m_move_to_saturation_value) < 0x7F )  )
		{
		 m_Lighting_Saturation_Data = m_8bit_temporary_data1 - (((m_8bit_temporary_data1 -
						 m_move_to_saturation_value)* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data1 < m_move_to_saturation_value ) &&
				( (m_move_to_saturation_value - m_8bit_temporary_data1) <= 0x7F))
		{
		m_Lighting_Saturation_Data = m_8bit_temporary_data1 + (((m_move_to_saturation_value -
							 m_8bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data1 < m_move_to_saturation_value ) &&
				( (m_move_to_saturation_value - m_8bit_temporary_data1) > 0x7F))
		{
		m_Lighting_Saturation_Data = m_8bit_temporary_data1 - (((m_8bit_temporary_data1 + (0x0100 -
						m_move_to_saturation_value))* Color_Control_Timer_Count)/m_transition_time);
		}
		else if( ( m_8bit_temporary_data1 > m_move_to_saturation_value ) &&
				( (m_8bit_temporary_data1 - m_move_to_saturation_value) >= 0x7F))
		{
		m_Lighting_Saturation_Data = m_8bit_temporary_data1 + ((((0x0100 - m_8bit_temporary_data1) +
					     m_move_to_saturation_value)* Color_Control_Timer_Count)/m_transition_time);
		}



		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
			  App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
			  p_cluster_definition->p_cluster_info,
			  g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
			  (uint8_t*)&m_Enhanced_Lighting_Hue_Data );

		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
			  App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
			  p_cluster_definition->p_cluster_info,
			  g_CURRENT_SATURATION_ATTRIBUTE_c,
			  &m_Lighting_Saturation_Data );

		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
			  App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										 m_Lighting_End_Point,
										 g_COLOR_CONTROL_CLUSTER_c,
										 &p_cluster_definition ),
			  p_cluster_definition->p_cluster_info,
			  g_REMAINING_TIME_ATTRIBUTE_c,
			  (uint8_t*)&Remaining_Time );
		if( Color_Control_Timer_Count >= m_transition_time)	{

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c );
		  return;
		}


		tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
							   g_COLOR_CONTROL_TIME_OUT_DURATION_c * m_MILLISECONDS_c,
							   App_Enhanced_Move_To_Hue_And_Saturation_Call_Back );


}
/****************************************************************************************************/
void App_Color_Loop_Set_Call_Back ( uint8_t timer_id )
{

		Color_Control_Timer_Count++;
		m_Transition_Time--;
		Remaining_Time = m_Transition_Time*10;




		if ( m_Hue_Direction == g_Move_Hue_Up_c ) {
			if(  m_Enhanced_Lighting_Hue_Data  > 0xFFFF) {
			 m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + (((0x1000 - m_16bit_temporary_data1 +
								   0xFFFF)* Color_Control_Timer_Count)/m_transition_time);
			}
			else if( m_Enhanced_Lighting_Hue_Data <0xFFFF ) {
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 + ((( 0xFFFF - m_16bit_temporary_data1)
								  *Color_Control_Timer_Count)/m_transition_time);
			}
		}

		else if ( m_Hue_Direction == g_Move_Hue_Down_c )	{
			if(  m_Enhanced_Lighting_Hue_Data  > 0x0000) {
			 m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - (((m_16bit_temporary_data1 - 0x0000)
								   * Color_Control_Timer_Count)/m_transition_time);
			}
			else if( m_Enhanced_Lighting_Hue_Data < 0x0000) {
			m_Enhanced_Lighting_Hue_Data = m_16bit_temporary_data1 - ((( 0x1000 - 0x0000 +
					         m_16bit_temporary_data1 )*Color_Control_Timer_Count)/m_transition_time);
			}
		}


		ZCL_Read_Write_Attribute_Data (	g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
				(uint8_t*)&m_Enhanced_Lighting_Hue_Data
				);


		ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
				App_Get_AttrDataLocation ( g_ZCL_CLIENT_TO_SERVER_DIRECTION_c,
										  m_Lighting_End_Point,
										  g_COLOR_CONTROL_CLUSTER_c,
										  &p_cluster_definition ),
				p_cluster_definition->p_cluster_info,
				g_REMAINING_TIME_ATTRIBUTE_c,
				(uint8_t*)&Remaining_Time);


		if( Color_Control_Timer_Count >= m_transition_time ){

		tmr_stopTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c	);
		return;
		}




tmr_startRelativeTimer ( m_APPLICATION_LIGHTING_TIMER_ONE_c,
 					   g_COLOR_CONTROL_TIME_OUT_DURATION_c* m_MILLISECONDS_c,
 					   App_Color_Loop_Set_Call_Back );



}
/**********************************************************************************/
#endif /*g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */

#endif                    /*  g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d   */
/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
