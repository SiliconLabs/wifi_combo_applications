/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Common.h"
#include "timer_id.h"
#include "sw_timer.h"
#include "memory_utility.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "ApptimerId.h"
#include "ApplicationThinLayer.h"
#include "AppWindowsCoveringUtilities.h"
#if( g_WINDOW_COVERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Windows_Covering_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_SERVER_ATTRIBUTES_c                                   0x00


uint8_t Table_Size = 0x03; /*Max table size to store lift and tilt data*/
uint16_t lift_value;
uint16_t tilt_value;
uint8_t lift_percentage;
uint8_t tilt_percentage;


uint16_t aPhysicalLift =0x0000;
uint16_t aPhysicalTilt =0x0000;

uint8_t g_cmd_mode_c;

uint8_t g_Current_Level_Endpoint;
uint16_t g_transition_time;

int8_t g_Current_Level_Change;

uint16_t g_installedOpenLimit_c =g_ZERO_c;
uint16_t  g_installedClosedLimit_c =g_ZERO_c;



/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
static void App_Window_Covering_Up_Cmd_For_Lift_Call_Back ( uint8_t timerId );

static void App_Read_Write_Window_Covering_Attributes (  uint8_t read_or_write,
                       uint8_t *attrib_value,  uint8_t attrib_id  );


/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
uint8_t ZCL_CallBack_Windows_Covering( ZCL_Event_t *pEvents )
{
	
	
	static uint8_t tableIndex = g_ZERO_c;
	uint8_t space_available = g_FALSE_c;
	uint8_t status= g_ZCL_Success_c;
	uint8_t setPointValue = g_ZERO_c;
	uint8_t tIndex =0;
	uint16_t  aClosedLimit = 0x0000;
	uint16_t aOpenLimit = 0x0000;
		uint8_t configStatus=g_ZERO_c;
   uint8_t attribute_id;
   	uint16_t velocity =g_ZERO_c;

	attribute_id = g_CONFIG_STATUS_ATTRIBUTE_c;
    g_Current_Level_Endpoint = ZCL_Callback_Get_Device_EndPoint();
	
	/*  read the current velocity value and update it and write it back */
   			 App_Read_Write_Window_Covering_Attributes ( g_Read_Attribute_Data_c,
                                         ( uint8_t *)&configStatus, attribute_id );
	
	switch ( pEvents->eventId ){
		 case g_Up_Open_Command_Event_c:
		    g_cmd_mode_c = g_UP_OPEN_CMD_c;
			
			attribute_id = g_VELOCITY_LIFT_ATTRIBUTE_c;
			
			
    		/*  read the current velocity value and update it and write it back */
   			 App_Read_Write_Window_Covering_Attributes ( g_Read_Attribute_Data_c,
                                         ( uint8_t *)&velocity, attribute_id );
			
			 attribute_id = g_INSTALLED_OPEN_LIMIT_LIFT_ATTRIBUTE_c;
			/* If the devices running in a closed loop,Windows Covering will
			adjust the window so the physical lift is at the InstalledOpenLimit
			– Lift */
			  App_Read_Write_Window_Covering_Attributes ( g_Read_Attribute_Data_c,
                                          ( uint8_t *)&g_installedOpenLimit_c, attribute_id );
			
			 g_transition_time =( g_installedOpenLimit_c/velocity);
			
			tmr_startRelativeTimer ( m_APPLICATION_WINDOW_COVERING_TIMER_c,
				g_transition_time, App_Window_Covering_Up_Cmd_For_Lift_Call_Back );
	
		
		 break;
		 case g_Down_Close_Command_Event_c:
		    g_cmd_mode_c = g_DOWN_CLOSE_CMD_c;
			
			 g_Current_Level_Endpoint = ZCL_Callback_Get_Device_EndPoint();
			
			attribute_id = g_VELOCITY_LIFT_ATTRIBUTE_c;
			
			
    		/*  read the current velocity value and update it and write it back */
   			 App_Read_Write_Window_Covering_Attributes ( g_Read_Attribute_Data_c,
                                         ( uint8_t *)&velocity, attribute_id );
			
			
			/* If the devices running in a closed loop,Windows Covering will
			adjust the window so the physical lift is at the InstalledclosedLimit
			– Lift */
			
			 attribute_id = g_INSTALLED_CLOSED_LIMIT_LIFT_ATTRIBUTE_c;
			  App_Read_Write_Window_Covering_Attributes ( g_Read_Attribute_Data_c,
                                          ( uint8_t *)&g_installedClosedLimit_c, attribute_id );
			
			 g_transition_time =( g_installedClosedLimit_c/velocity);
		
			tmr_startRelativeTimer ( m_APPLICATION_WINDOW_COVERING_TIMER_c,
				g_TIMER_COUNT_ONE_SECOND_c, App_Window_Covering_Up_Cmd_For_Lift_Call_Back );  	
		
		 break;
		 case g_Stop_Command_Event_c:
		
		   tmr_stopTimer( m_APPLICATION_WINDOW_COVERING_TIMER_c	);
		 break;
		
		case g_Go_To_Lift_Set_Point_Command_Event_c:
		
		
		  App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_INSTALLED_OPEN_LIMIT_LIFT_ATTRIBUTE_c,
							   (uint8_t*)&aOpenLimit,
							   g_Read_Attribute_Data_c );
		
		   App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_INSTALLED_CLOSED_LIMIT_LIFT_ATTRIBUTE_c,
							   (uint8_t*)&aClosedLimit,
							   g_Read_Attribute_Data_c );
		
		for ( tIndex =0 ; tIndex < Table_Size ; tIndex++){
			if( (windows_covering_table[tIndex].Set_Point_Type
				== g_LIFT_c)  && (windows_covering_table[tIndex].Set_Point_Index
				== ((Set_point_Version1*)pEvents->pEventData)->a_set_point_index )){
				
				    /* received Lift  will not larger than InstalledOpenLimit –Lift
				  and not smaller than  InstalledClosedLimit – Lift. */
		
				if ( (windows_covering_table[tIndex].Set_Point_Value > aClosedLimit )
					&&(windows_covering_table[tIndex].Set_Point_Value < aOpenLimit)){
					
					 /*  windows covering will adjust the  window so the physical lift is received
					lift value */
			
					 memUtils_memCopy((uint8_t*) &aPhysicalLift,(uint8_t*) pEvents->pEventData  ,2);
					  /* update the current position of lift*/
					 App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_CURRENT_POSITION_LIFT_ATTRIBUTE_c,
							   (uint8_t*)&aPhysicalLift,
							   g_Write_Attribute_Data_c );
					break;
				}
				else{
					return g_ZCL_Invalid_Value_c;
				}
			}
				
//				tableIndex++;
		}
		if( tIndex == Table_Size)
			return g_ZCL_Not_Found_c;
		
		break;
		case g_Go_To_Lift_Value_Command_Event_c:
		   /*  windows covering will adjust the  window so the physical lift is received
			lift value */
		
		 memUtils_memCopy((uint8_t*) &aPhysicalLift,(uint8_t*) pEvents->pEventData  ,2);
		
		  App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_INSTALLED_OPEN_LIMIT_LIFT_ATTRIBUTE_c,
							   (uint8_t*)&aOpenLimit,
							   g_Read_Attribute_Data_c );
		
		   App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_INSTALLED_CLOSED_LIMIT_LIFT_ATTRIBUTE_c,
							   (uint8_t*)&aClosedLimit,
							   g_Read_Attribute_Data_c );
		
		   /* received tilt will not larger than InstalledOpenLimit –Lift and not smaller than
			InstalledClosedLimit – Lift. */
		
		 if( aPhysicalLift > aClosedLimit  && aPhysicalLift < aOpenLimit) {
		   App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_CURRENT_POSITION_LIFT_ATTRIBUTE_c,
							   (uint8_t*)&aPhysicalLift,
							   g_Write_Attribute_Data_c );
			/*Hard code should be added here*/
		 }
		 else  {
		   return g_ZCL_Not_Found_c;
		 }
		 break;
		case g_Go_To_Lift_Percentage_Command_Event_c:
		
			if (  *pEvents->pEventData > 0x64)
			  return g_ZCL_Invalid_Value_c;
			else{
			  lift_percentage = *pEvents->pEventData;
			  memUtils_memCopy ((uint8_t*) &aPhysicalLift,(uint8_t*)& lift_percentage  ,2 );
			  App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_CURRENT_POSITION_LIFT_PERCENTAGE_ATTRIBUTE_c,
							   &lift_percentage,
							   g_Write_Attribute_Data_c );
			
			/* If the device only supports open loop lift action then a
				zero percentage should be treated as a down/close command and a nonzero
				percentage should be treated as an up/open command */
				
				if( ~(configStatus & 0x08) ){
					if( lift_percentage == 0x00) {
					/*   down/close command */
					}
					else {
					/*  send up/open command */
					} 				
				}
				else if (configStatus & 0x10) {
				 /*  If the device is only a tilt control device then
				  return unsupported command */
					return g_ZCL_UnSup_Cluster_Command_c;
				}
			
			}
		break;  		
		case g_Go_To_Tilt_Set_Point_Command_Event_c:
		
		
		  App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_INSTALLED_OPEN_LIMIT_TILT_ATTRIBUTE_c,
							   (uint8_t*)&aOpenLimit,
							   g_Read_Attribute_Data_c );
		
		   App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_INSTALLED_CLOSED_LIMIT_TILT_ATTRIBUTE_c,
							   (uint8_t*)&aClosedLimit,
							   g_Read_Attribute_Data_c );
		
		
		  for  (tIndex=0 ;  tIndex < Table_Size ;tIndex++ ){
			
			if( (windows_covering_table[tIndex].Set_Point_Type
				== g_TILT_c)  && (windows_covering_table[tIndex].Set_Point_Index
				==((Set_point_Version1*)pEvents->pEventData)->a_set_point_index )){
				
				    /* received setpoint  will not larger than InstalledOpenLimit –Lift
				  and not smaller than  InstalledClosedLimit – Lift. */
		
				if ( (windows_covering_table[tIndex].Set_Point_Value > aClosedLimit )
					&&( windows_covering_table[tIndex].Set_Point_Value < aOpenLimit)){
					
					    /*  windows covering will adjust the  window so the physical tilt is
			at the value received
			tilt value */
		
		  memUtils_memCopy ((uint8_t*) &aPhysicalTilt,(uint8_t*)pEvents->pEventData  ,2 );
					
					 /*  windows covering will adjust the  window so the physical lift is received
					lift value */
			
					 memUtils_memCopy((uint8_t*) &aPhysicalTilt,(uint8_t*) pEvents->pEventData  ,2);
					  App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_CURRENT_POSITION_LIFT_ATTRIBUTE_c,
							   (uint8_t*)&aPhysicalTilt,
							   g_Write_Attribute_Data_c );
					break;
				}
				else
					return g_ZCL_Invalid_Value_c;
			}
				
//				tableIndex++;
			}
		  if( tIndex == Table_Size)
		   return g_ZCL_Not_Found_c;
			
		break;
		case g_Go_To_Tilt_Value_Command_Event_c:
		  /*  windows covering will adjust the  window so the physical tilt is received
			tilt value */
		
		 memUtils_memCopy ((uint8_t*) &aPhysicalTilt,(uint8_t*)pEvents->pEventData  ,2 );
		
		  App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_INSTALLED_OPEN_LIMIT_TILT_ATTRIBUTE_c,
							   (uint8_t*)&aOpenLimit,
							   g_Read_Attribute_Data_c );
		
		   App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_INSTALLED_CLOSED_LIMIT_TILT_ATTRIBUTE_c,
							   (uint8_t*)&aClosedLimit,
							   g_Read_Attribute_Data_c );
		/* received tilt will not larger than InstalledOpenLimit – Tilt and not smaller than
			InstalledClosedLimit – Tilt. */
		 if(( aPhysicalTilt > aClosedLimit) && (aPhysicalTilt < aOpenLimit))
		 {
			App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_CURRENT_POSITION_TILT_ATTRIBUTE_c,
							   (uint8_t*)&aPhysicalTilt,
							   g_Write_Attribute_Data_c );
			/*Hard code should be added here*/
		 }
		 else
		 {
		   return g_ZCL_Not_Found_c;
		 }
		break;
		case g_Go_To_Tilt_Percentage_Command_Event_c:  	
	
			/* No physical Action will be taken if the received tilt percenatage
			is greater than 100, return invalid value*/
			
			if ( *pEvents->pEventData > 0x64)
			  return g_ZCL_Invalid_Value_c;
			else{
			     tilt_percentage = *pEvents->pEventData;
				/*  windows covering will adjust the  window so the physical tilt is received
				 tilt  percentage */
			  	 memUtils_memCopy ((uint8_t*) &aPhysicalTilt,(uint8_t*)& tilt_percentage  ,2 );

				/* Update current position tilt percentage attribute value
				  to received tilt percentage */
				
				
				App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
							   g_WINDOW_COVERING_CLUSTER_c,
							   g_CURRENT_POSITION_TILT_PERCENTAGE_ATTRIBUTE_c,
							   &tilt_percentage,
							   g_Write_Attribute_Data_c );
				
				/* If the device only supports open loop tilt action then a
				zero percentage should be treated as a down/close command and a nonzero
				percentage should be treated as an up/open command */
				
				if( ~(configStatus & 0x10) ){
					if( tilt_percentage == 0x00) {
					/*  windows covering will adjust as a down/close command */
					}
					else {
					/* windows covering will adjust as a up/open command */
					} 				
				}
				else if (configStatus & 0x80) {
				 /*  If the device is only a lift control device then
				  return unsupported command */
					return g_ZCL_UnSup_Cluster_Command_c;
				}
			}
		break;    		
		
		case g_Program_Set_Point_Command_Version1_Event_c:  		
			while (tableIndex < Table_Size) {   			
	
			  if ( (((Set_point_Version1*)pEvents->pEventData)->a_set_point_type)== g_LIFT_c){
				
					for (uint8_t i=0;i< Table_Size; i++) { 				
						if( (windows_covering_table[i].Set_Point_Type == g_LIFT_c) &&
						   (windows_covering_table[i].Set_Point_Index ==
							((Set_point_Version1*)pEvents->pEventData)->a_set_point_index))
						{
							return g_ZCL_Duplicate_Exists_c;
						}
					}
				
					/* if the set point type is an Lift ,
				  	copy corresponding set point index and set point value*/ 			
				
					windows_covering_table[tableIndex].Set_Point_Type =
					 	((Set_point_Version1*)pEvents->pEventData)->a_set_point_type;
						
					windows_covering_table[tableIndex].Set_Point_Index =
					((Set_point_Version1*)pEvents->pEventData)->a_set_point_index;
					
					memUtils_memCopy ((uint8_t*) &windows_covering_table[tableIndex].Set_Point_Value,
					(uint8_t*)&(((Set_point_Version1*)pEvents->pEventData)->a_set_point_value ),2 );
					space_available = g_TRUE_c;
					 tableIndex++;
					break;
				 }
				 else if ((((Set_point_Version1*)pEvents->pEventData)->a_set_point_type) == g_TILT_c ){
				
				   for (uint8_t i=0;i< Table_Size; i++) { 				
						if( (windows_covering_table[i].Set_Point_Type == g_TILT_c) &&
						   (windows_covering_table[i].Set_Point_Index ==
							((Set_point_Version1*)pEvents->pEventData)->a_set_point_index))
						{
							return g_ZCL_Duplicate_Exists_c;
						}
					}
					/* if the set point type is an tilt ,
				   copy corresponding set point index and set poin value*/ 	
				
					windows_covering_table[tableIndex].Set_Point_Type =
					 ((Set_point_Version1*)pEvents->pEventData)->a_set_point_type;
				
					windows_covering_table[tableIndex].Set_Point_Index =
						((Set_point_Version1*)pEvents->pEventData)->a_set_point_index;
					
					memUtils_memCopy ((uint8_t*) &windows_covering_table[tableIndex].Set_Point_Value,
					(uint8_t*)&(((Set_point_Version1*)pEvents->pEventData)->a_set_point_value ),2 );
					space_available = g_TRUE_c;
					 tableIndex++;
					break;					
				 }
				 else {
						return g_ZCL_Not_Found_c;		
				 }
			}
			/*space not available to store data*/
			if( space_available != g_TRUE_c)
			 	return g_ZCL_Insufficient_Space_c;		
		break;
		case g_Program_Set_Point_Command_Version2_Event_c:
			while (tableIndex < Table_Size) {
			
			  for (uint8_t i=0;i< Table_Size; i++) { 	
				if (
					(( windows_covering_table[i].Set_Point_Type == g_LIFT_c) &&
				( windows_covering_table[i].Set_Point_Index ==
					((Set_point_Version2*)pEvents->pEventData)->a_set_point_index )) ||
				
				 ( ( windows_covering_table[i].Set_Point_Type == g_TILT_c) &&
				( windows_covering_table[i].Set_Point_Index ==
					((Set_point_Version2*)pEvents->pEventData)->a_set_point_index )
				  )) {
					
					return g_ZCL_Duplicate_Exists_c;
					
				  }
			  }
				
				/* if the set point type is an Lift ,
				copy corresponding set point index and set point value*/
			
				windows_covering_table[tableIndex].Set_Point_Type = g_LIFT_c;
			
				windows_covering_table[tableIndex].Set_Point_Index =
				((Set_point_Version2*)pEvents->pEventData)->a_set_point_index;
				
				/* if the set point index exist and corresponding set point type is Tilt,
				, update the set point value to  a current position lift attribute*/
				
				App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
						   g_WINDOW_COVERING_CLUSTER_c,
						   g_CURRENT_POSITION_LIFT_ATTRIBUTE_c,
						   &setPointValue,
						   g_Read_Attribute_Data_c );
				
				memUtils_memCopy((uint8_t*) &windows_covering_table[tableIndex].Set_Point_Value,
									 (uint8_t*)&setPointValue,2 );
				tableIndex++;
				
				if(tableIndex > Table_Size) {
					return g_ZCL_Insufficient_Space_c;	
				}
			
				/* if the set point type is an tilt ,
				copy corresponding set point index and set poin value*/ 				
				
			
				windows_covering_table[tableIndex].Set_Point_Type = g_TILT_c;
				
				windows_covering_table[tableIndex].Set_Point_Index =
					((Set_point_Version2*)pEvents->pEventData)->a_set_point_index;
				
				/* if the set point index exist and corresponding set point type is Tilt,
					  then update the set point value to a current position tilt attribute*/
					
				App_ReadWriteAttribute(ZCL_Callback_Get_Device_EndPoint() ,
								   g_WINDOW_COVERING_CLUSTER_c,
								   g_CURRENT_POSITION_TILT_ATTRIBUTE_c,
								   &setPointValue,
								   g_Read_Attribute_Data_c );
						
				memUtils_memCopy((uint8_t*) &windows_covering_table[tableIndex].Set_Point_Value,
											 (uint8_t*)&setPointValue,2 );
				space_available = g_TRUE_c;
				tableIndex++;
				break;   				
			}
			if( space_available != g_TRUE_c)   			/*space not available to store data*/
			 	return g_ZCL_Insufficient_Space_c;	
		 	 break;
	} /*End of Switch case*/
	return status;
}

/*******************************************************************************/
static void App_Window_Covering_Up_Cmd_For_Lift_Call_Back ( uint8_t timerId )
{

	int16_t currentPosition = g_ZERO_c;
	
	uint8_t attribute_id = g_CURRENT_POSITION_LIFT_ATTRIBUTE_c;
	

	
	/*  read the current level value and update it and write it back */
	App_Read_Write_Window_Covering_Attributes ( g_Read_Attribute_Data_c,
									  (uint8_t*)&currentPosition, attribute_id );
	
	if( g_cmd_mode_c == g_UP_OPEN_CMD_c){
	
	  	g_Current_Level_Change = m_CHANGE_UP_BY_ONE_c;
		/*  currentPosition should not go beyond installed open lmit and
		installed open limit */
		if ( currentPosition + g_Current_Level_Change >  g_installedOpenLimit_c ) {
			currentPosition = g_installedOpenLimit_c;
		}
		else {
			currentPosition = currentPosition + g_Current_Level_Change;
		}
		
		App_Read_Write_Window_Covering_Attributes ( g_Write_Attribute_Data_c,
									  (uint8_t*)&currentPosition, attribute_id );
		
		if ( currentPosition == g_installedOpenLimit_c)
			{
				tmr_stopTimer ( m_APPLICATION_WINDOW_COVERING_TIMER_c	);
				return;
			}
	
			tmr_startRelativeTimer ( m_APPLICATION_WINDOW_COVERING_TIMER_c,
								   g_transition_time * m_MILLISECONDS_c,
								   App_Window_Covering_Up_Cmd_For_Lift_Call_Back );
	}
	else {
	
	  	g_Current_Level_Change = m_CHANGE_DOWN_BY_ONE_c;
		/*  currentPosition should not go beyond installed closed lmit and
		installed open limit */
		if ( currentPosition + g_Current_Level_Change <  g_installedClosedLimit_c ) {
			currentPosition = g_installedOpenLimit_c;
		}
		else {
			currentPosition = currentPosition + g_Current_Level_Change;
		}
		
		App_Read_Write_Window_Covering_Attributes ( g_Write_Attribute_Data_c,
									  (uint8_t*)&currentPosition, attribute_id );
		
		if ( currentPosition == g_installedClosedLimit_c)
			{
				tmr_stopTimer ( m_APPLICATION_WINDOW_COVERING_TIMER_c	);
				return;
			}
	
			tmr_startRelativeTimer ( m_APPLICATION_WINDOW_COVERING_TIMER_c,
								   g_transition_time * m_MILLISECONDS_c,
								   App_Window_Covering_Up_Cmd_For_Lift_Call_Back );
	
	}


}

/***********************************************************************************************/
static void App_Read_Write_Window_Covering_Attributes (  uint8_t read_or_write,
                       uint8_t *attrib_value,  uint8_t attrib_id  )
{
   App_Cluster_Def_t *p_cluster_definition;

   if(attrib_value == g_NULL_c){
	 return;
   }

   ZCL_Read_Write_Attribute_Data( read_or_write,
        App_Get_AttrDataLocation ( m_SERVER_ATTRIBUTES_c, g_Current_Level_Endpoint,
                                   g_WINDOW_COVERING_CLUSTER_c, &p_cluster_definition ),
        p_cluster_definition->p_cluster_info,
        attrib_id,
        attrib_value );
}

#endif /*g_WINDOW_COVERING_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
