/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#include "stack_util.h"
#include "common_includes.h"
#include "stdbool.h"
#include "memory_utility.h"
#if ( g_COLOR_CONTROL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Lighting_Color_Control_Cluster.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_COLOR_CONTROL_CLUSTER_NUM_OF_ATTRIBUTES_c    0x03
#define m_ONE_EVENT_c                              0x01
#define m_INVALID_RATE_c                           0x00
#define m_EVENT_LENGTH_TWO_BYTES_c                 0x02
#define m_EVENT_LENGTH_THREE_BYTES_c               0x03
#define m_EVENT_LENGTH_FOUR_BYTES_c                0x04
#define m_MAX_VALUE_c                              0xFF
#define m_MAXIMUM_VALUE_c							0xFFFF

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
/* Attribute information for Color Control Cluster supported attributes */
const ZCL_Attribute_Info_t ga_Color_Control_Attribute_Info [] =
{

#if (g_CURRENT_HUE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_CURRENT_HUE_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c ,

   }
#endif    /*g_CURRENT_HUE_ATTRIBUTE_ENABLE_d*/

#if ( g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d == 1 )
   ,
   {
      g_CURRENT_SATURATION_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c | g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c,
   },
#endif    /*g_CURRENT_SATURATION_ATTRIBUTE_ENABLE_d*/

#if ( g_REMAINING_TIME_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_REMAINING_TIME_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c,

   },
#endif    /*g_REMAINING_TIME_ATTRIBUTE_ENABLE_d*/

   {
      g_CURRENT_X_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c | g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c,
   },


   {
      g_CURRENT_Y_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c | g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c,
   },

#if ( g_DRIFT_COMPENSATION_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_DRIFT_COMPENSATION_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c,
   },
#endif  /*g_DRIFT_COMPENSATION_ATTRIBUTE_ENABLE_d*/

#if ( g_COMPENSATION_TEXT_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_COMPENSATION_TEXT_ATTRIBUTE_c,
      g_Character_String_Data_c,
      g_Read_Only_Attribute_c,
   },
#endif   /*g_COMPENSATION_TEXT_ATTRIBUTE_ENABLE_d*/

#if ( g_COLOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_COLOR_TEMPERATURE_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c,
   },
#endif  /*g_COLOR_TEMPERATURE_ATTRIBUTE_ENABLE_d*/

#if ( g_COLOR_MODE_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_COLOR_MODE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c,

   },
#endif   /*g_COLOR_MODE_ATTRIBUTE_ENABLE_d*/
#if(g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d==1)
#if( g_ENHANCED_CURRENT_HUE_ATTRIBUTE_ENABLE_d==1)
	{
	 g_ENHANCED_CURRENT_HUE_ATTRIBUTE_c,
     g_Unsigned_Sixteen_Bit_Integer_Data_c,
	 g_Read_Only_Attribute_c|g_Reportable_Attribute_c|g_Scenes_Ext_Attribute_c,
	},
#endif /*g_ENHANCED_CURRENT_HUE_ATTRIBUTE_ENABLE_d*/

#if(g_ENHANCED_COLOR_MODE_ATTRIBUTE_ENABLE_d==1)
	{
	  g_ENHANCED_COLOR_MODE_ATTRIBUTE_c,
	  g_Eight_Bit_Enumeration_Data_c,
	  g_Read_Only_Attribute_c,
	 },
#endif /*g_ENHANCED_COLOR_MODE_ATTRIBUTE_ENABLE_d*/

#if(g_COLOR_LOOP_ACTIVE_ATTRIBUTE_ENABLE_d==1)
	{
	  g_COLOR_LOOP_ACTIVE_ATTRIBUTE_c,
	  g_Unsigned_Eight_Bit_Integer_Data_c,
	  g_Read_Only_Attribute_c|g_Reportable_Attribute_c|g_Scenes_Ext_Attribute_c,
	 },
#endif /*g_COLOR_LOOP_ACTIVE_ATTRIBUTE_ENABLE_d*/

#if(g_COLOR_LOOP_DIRECTION_ATTRIBUTE_ENABLE_d==1)
	{
	 g_COLOR_LOOP_DIRECTION_ATTRIBUTE_c,
	 g_Unsigned_Eight_Bit_Integer_Data_c,
	 g_Read_Only_Attribute_c|g_Reportable_Attribute_c|g_Scenes_Ext_Attribute_c,
	},
#endif /*g_COLOR_LOOP_DIRECTION_ATTRIBUTE_ENABLE_d */

#if(g_COLOR_LOOP_TIME_ATTRIBUTE_ENABLE_d==1)
	{
	 g_COLOR_LOOP_TIME_ATTRIBUTE_c,
	 g_Unsigned_Sixteen_Bit_Integer_Data_c,
	 g_Read_Only_Attribute_c|g_Reportable_Attribute_c|g_Scenes_Ext_Attribute_c,
	},
#endif /*g_COLOR_LOOP_TIME_ATTRIBUTE_ENABLE_d*/

#if(g_COLOR_LOOP_START_ENHANCED_HUE_ATTRIBUTE_ENABLE_d==1)
	{
	  g_COLOR_LOOP_START_ENHANCED_HUE_c,
	  g_Unsigned_Sixteen_Bit_Integer_Data_c,
	  g_Read_Only_Attribute_c,
	},
#endif /*g_COLOR_LOOP_START_ENHANCED_HUE_ATTRIBUTE_ENABLE_d*/

#if(g_COLOR_CAPABILITIES_ATTRIBUTE_ENABLE_d==1)
	{
	  g_CLOLR_CAPABILITIES_c,
	  g_Sixteen_Bit_Bitmap_Data_c,
	  g_Read_Only_Attribute_c,
	},
#endif /*g_COLOR_CAPABILITIES_ATTRIBUTE_ENABLE_d*/

#if(g_COLOR_LOOP_STORE_ENHANCED_HUE_ATTRIBUTE_ENABLE_d==1)
	{
	  g_COLOR_LOOP_STORE_ENHANCED_HUE_c,
	  g_Unsigned_Sixteen_Bit_Integer_Data_c,
	  g_Read_Only_Attribute_c,
	},
#endif /*g_COLOR_LOOP_STORE_ENHANCED_HUE_ATTRIBUTE_ENABLE_d*/

#if(g_COLOR_TEMP_PHYSICAL_MIN_ATTRIBUTE_ENABLE_d==1)
	{
	 g_COLOR_TEMP_PHYSICAL_MIN_c,
	 g_Unsigned_Sixteen_Bit_Integer_Data_c,
	 g_Read_Only_Attribute_c,
	},
#endif /*g_COLOR_TEMP_PHYSICAL_MIN_ATTRIBUTE_ENABLE_d*/

#if(g_COLOR_TEMP_PHYSICAL_MAX_ATTRIBUTE_ENABLE_d==1)
		{
	 g_COLOR_TEMP_PHYSICAL_MAX_c,
	 g_Unsigned_Sixteen_Bit_Integer_Data_c,
	 g_Read_Only_Attribute_c,
	},
#endif /*g_COLOR_TEMP_PHYSICAL_MAX_ATTRIBUTE_ENABLE_d*/

#endif /*g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
 };




/*---------------------------------------------------------------------------*/

/* Cluster Info for Color Server Control Cluster */
const ZCL_Cluster_Info_t ga_Color_Control_Server_Cluster_Info =
{
   g_COLOR_CONTROL_CLUSTER_c ,
   sizeof(ga_Color_Control_Attribute_Info)/sizeof(ZCL_Attribute_Info_t),
   ga_Color_Control_Attribute_Info
};
#endif                              /* g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */

#if ( g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
/*cluster info for Color client control cluster*/
const ZCL_Cluster_Info_t ga_Color_Control_Client_Cluster_Info =
{
   g_COLOR_CONTROL_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif                              /* g_COLOR_CONTROL_CLIENT_CLUSTER_ENABLE_d */
/*---------------------------------------------------------------------------*/

uint8_t *gp_Color_Control_Attribute_Data_List;
ZCL_Cluster_Info_t *gp_Color_Control_Cluster_Info;

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Color_Ctrl_Cluster_Cmd (uint8_t endPointId,uint8_t asduLength,
                                    uint8_t* pReceivedAsdu,
                                    uint8_t* pResponseAsduStartLocation,
                                    ZCL_ActionHandler_t *pActionHandler,
                                    ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                    uint8_t * pAttrsDataBaseLocation )
{

#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
	pActionHandler->event.eventLength = asduLength - m_EVENT_LENGTH_THREE_BYTES_c;
	uint8_t *p_data;

	uint8_t color_mode_value = g_ZERO_c;
#if(g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d==1)
	uint16_t p_data_rate;
	uint16_t p_enhanced_data;
	uint8_t enhanced_color_mode=g_ZERO_c ;
#endif /*g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
	/* pointer to load the lighting events */
	Lighting_Event *p_Lighting_Event = ((Lighting_Event *)(pActionHandler->event.pEventData));
	uint8_t status = g_ZCL_Success_c;
    bool defaultResponseRequired = Is_Default_Response_Required(pReceivedAsdu);

#endif
	uint8_t responseLength = g_ZERO_c;
	pActionHandler->action = No_Response;
//   /* Switching the command id values to check which command it is */
	switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
	{
#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )

	case g_Move_to_Hue_Command_c:
		
		/* Loading the event id, event length and event data */
		pActionHandler->event.eventId = g_Move_to_Hue_Command_Event_c;
		p_Lighting_Event->event_data.move_to_hue_command_data.hue = pReceivedAsdu[3];
		p_Lighting_Event->event_data.move_to_hue_command_data.direction = pReceivedAsdu[4];
		memUtils_memCopy((uint8_t*)&( p_Lighting_Event->event_data.move_to_hue_command_data.
							 a_transition_time),&(pReceivedAsdu[5] ),2);
		color_mode_value= g_ZERO_c;

		p_data = &( p_Lighting_Event->event_data.move_to_hue_command_data.hue);
		if( *p_data == m_MAX_VALUE_c ) {
			status = g_ZCL_Invalid_Value_c;
		}

		break;
		
	case g_Move_Hue_Command_c:		
		/* Loading the event id, event length and event data */		
		pActionHandler->event.eventId = g_Move_hue_Command_Event_c;
		p_Lighting_Event->event_data.move_hue_command_data.move_mode = 	pReceivedAsdu[3];
		p_Lighting_Event->event_data.move_hue_command_data.rate = pReceivedAsdu[4];
		color_mode_value= g_ZERO_c;

		p_data = &( p_Lighting_Event->event_data.move_hue_command_data.rate);
		if( *p_data == m_INVALID_RATE_c ) {
			status = g_ZCL_Invalid_Field_c;
			pActionHandler->event.eventId = g_NULL_c;
		}
		
		break;
		
	case g_Step_Hue_Command_c:
		/* Loading the event id, event length and event data */
		pActionHandler->event.eventId = g_Step_Hue_Command_Event_c;
		p_Lighting_Event->event_data.step_hue_command_data.step_mode = pReceivedAsdu[3];
		p_Lighting_Event->event_data.step_hue_command_data.step_size = pReceivedAsdu[4];
		p_Lighting_Event->event_data.step_hue_command_data.transition_time = pReceivedAsdu[5];
		color_mode_value= g_ZERO_c;		
	
		break;
		
	case g_Move_To_Saturation_Command_c:
		/* Loading the event id, event length and event data */
		pActionHandler->event.eventId = g_Move_To_Saturation_Command_Event_c;
		p_Lighting_Event->event_data.move_to_saturation_command_data.saturation = pReceivedAsdu[3];
		memUtils_memCopy ( (uint8_t*)&( p_Lighting_Event->event_data.
				  move_to_saturation_command_data.a_transition_time ), &pReceivedAsdu[4] ,2 );
		color_mode_value= g_ZERO_c;
		p_data = &( p_Lighting_Event->event_data.move_to_saturation_command_data.saturation);

		if( *p_data == m_MAX_VALUE_c ) {
			status = g_ZCL_Invalid_Value_c;
		}
		
		break;
		
	case g_Move_Saturation_Command_c:
		/* Loading the event id, event length and event data */
		pActionHandler->event.eventId = g_Move_Saturation_Command_Event_c;
		p_Lighting_Event->event_data.move_saturation_command_data.move_mode = pReceivedAsdu[3];
		p_Lighting_Event->event_data.move_saturation_command_data.rate = pReceivedAsdu[4];
		color_mode_value= g_ZERO_c;
		p_data = &( p_Lighting_Event->event_data.move_saturation_command_data.rate);
		/* Checking if the rate value is invalid */
		

		if( *p_data == m_INVALID_RATE_c ) {
			status = g_ZCL_Invalid_Field_c;
			pActionHandler->event.eventId = g_NULL_c;
		}

		
		break;	
		
	case g_Step_Saturation_Command_c:
		/* Loading the event id, event length and event data */
		pActionHandler->event.eventId = g_Step_Saturation_Command_Event_c;
		p_Lighting_Event->event_data.step_saturation_command_data.step_mode = pReceivedAsdu[3];
		p_Lighting_Event->event_data.step_saturation_command_data.step_size = pReceivedAsdu[4];
	    p_Lighting_Event->event_data.step_saturation_command_data.transition_time = pReceivedAsdu[5];
		p_data = &( p_Lighting_Event->event_data.step_saturation_command_data.step_size);
		color_mode_value= g_ZERO_c;
		if( *p_data == m_MAX_VALUE_c ) {
			status = g_ZCL_Invalid_Value_c;
		}

		break;
		
	case g_Move_To_Hue_And_Saturation_Command_c:
		
		/* Loading the event id, event length and event data */
		pActionHandler->event.eventId = g_Move_To_Hue_And_Saturation_Command_Event_c;
		p_Lighting_Event->event_data.move_to_hue_and_saturation_command_data.hue = pReceivedAsdu[3];
		p_Lighting_Event->event_data.move_to_hue_and_saturation_command_data.saturation = pReceivedAsdu[4];
		color_mode_value= g_ZERO_c;
		memUtils_memCopy((uint8_t*)&(p_Lighting_Event->event_data.move_to_hue_and_saturation_command_data.
							 a_transition_time), &(pReceivedAsdu[5]),2 );
		p_data = &( p_Lighting_Event->event_data.move_to_hue_and_saturation_command_data.saturation);

		if( *p_data == m_MAX_VALUE_c ) {
			status = g_ZCL_Invalid_Value_c;
		}
		
		break;
		
		
	case g_Move_To_Color_Temperature_Command_c:
		
		pActionHandler->event.eventId = g_Move_To_Color_Temperature_Command_Event_c;
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_to_color_temperature_command_data.
							  color_temperature), &(pReceivedAsdu[3]),2 );
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_to_color_temperature_command_data.
							  a_transition_time), &(pReceivedAsdu[5]),2 );
		color_mode_value= 0x01;
		
		break;

		
	case g_Move_To_Color_Command_c:
		pActionHandler->event.eventId = g_Move_To_Color_Command_Event_c;
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_to_color_command_data.color_x),
			 &(pReceivedAsdu[3]),2 );
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_to_color_command_data.color_y),
			 &(pReceivedAsdu[5]),2 );
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_to_color_command_data.a_transition_time),
			 &(pReceivedAsdu[7]),2 );
		color_mode_value= 0x01;

		break;	
		
	case g_Move_Color_Command_c:
		
		pActionHandler->event.eventId = g_Move_Color_Command_Event_c;
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_color_command_data.rate_x),
			 &(pReceivedAsdu[3]),2 );
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_color_command_data.rate_y),
			 &(pReceivedAsdu[5]),2 );
		color_mode_value= 0x01;		

		break;
		
		
	case g_Step_Color_Command_c:
		pActionHandler->event.eventId = g_Step_Color_Command_Event_c;
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.step_color_command_data.step_x),
			 &(pReceivedAsdu[3]),2 );
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.step_color_command_data.step_y),
			 &(pReceivedAsdu[5]),2 );
		memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.step_color_command_data.a_transition_time),
			 &(pReceivedAsdu[7]),2 );				
		color_mode_value= 0x01;		

		break;

#if(g_ZLL_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d==1)

	case g_Enhanced_Move_To_Hue_Command_c :

		/* Loading the event id, event length and event data */
			pActionHandler->event.eventId = g_Enhanced_Move_To_Hue_Command_Event_c;

		/*Get enhanced hue */
			memUtils_memCopy((uint8_t*)&( p_Lighting_Event->event_data.enhanced_move_to_hue_command_data.
							 Enhanced_hue),&(pReceivedAsdu[3] ),2);
		/* Get direction field value */
			p_Lighting_Event->event_data.enhanced_move_to_hue_command_data.direction = pReceivedAsdu[5];

		/*Get transition time */
			memUtils_memCopy((uint8_t*)&( p_Lighting_Event->event_data.enhanced_move_to_hue_command_data.
							 a_transition_time),&(pReceivedAsdu[6] ),2);


		/*set color mode to zero */
			color_mode_value= g_ZERO_c;
		/*set enhanced color mode value to 0x03 */
			enhanced_color_mode=0x03 ;

			memUtils_memCopy((uint8_t*)&p_enhanced_data,(uint8_t*)&( p_Lighting_Event->event_data.enhanced_move_to_hue_command_data.Enhanced_hue),2);
			if( p_enhanced_data == m_MAXIMUM_VALUE_c )
			{

				status = g_ZCL_Invalid_Value_c;
			}
			/*check default response require or not */
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
				responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, status );
				pActionHandler->action = Send_Response;
				return responseLength;
			}

		break ;

	case g_Enhanced_Move_Hue_Command_c :

		/* Loading the event id, event length and event data */
		pActionHandler->event.eventId = g_Enhanced_Move_Hue_Command_Event_c;
		/*Get move mode field value */
		p_Lighting_Event->event_data.enhanced_move_hue_command_data.move_mode = pReceivedAsdu[3];

		/*Get rate field value */
		memUtils_memCopy((uint8_t*)&( p_Lighting_Event->event_data.enhanced_move_hue_command_data.
							 rate),&(pReceivedAsdu[4] ),2);



		/*set color mode to zero */
		color_mode_value= g_ZERO_c;
		/*set enhanced color mode to 0x03 */
			enhanced_color_mode=0x03 ;


			memUtils_memCopy((uint8_t*)&p_data_rate,(uint8_t*)&( p_Lighting_Event->event_data.enhanced_move_hue_command_data.rate),2)          ;

			/* check rate field */
			if( (p_data_rate == m_INVALID_RATE_c)&&((p_Lighting_Event->event_data.enhanced_move_hue_command_data.move_mode== g_Up_c)||
			                           (p_Lighting_Event->event_data.enhanced_move_hue_command_data.move_mode==g_Down_c)))
			{
				status = g_ZCL_Invalid_Field_c;
				pActionHandler->event.eventId = g_NULL_c;
			}
			/*ignor if rate field is zero */
				else if(p_data_rate==m_INVALID_RATE_c)
					{
						return 0;
						}

			    	/*check default response require or not */
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
				responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, status );
				pActionHandler->action = Send_Response;
				return responseLength;
			}


		break;

	case g_Enhanced_Move_To_Hue_And_Saturation_Command_c :

		/* Loading the event id, event length and event data */
			pActionHandler->event.eventId = g_Enhanced_Move_To_Hue_And_Saturation_Command_Event_c;

		/*Get enhanced hue field value */

			memUtils_memCopy((uint8_t*)&(p_Lighting_Event->event_data.enhanced_move_to_hue_and_saturation_command.
							 Enhanced_hue), &(pReceivedAsdu[3]),2 );

		/*Get saturation field value */
			p_Lighting_Event->event_data.enhanced_move_to_hue_and_saturation_command.saturation = pReceivedAsdu[5];

		/*set color mode to zero */
			color_mode_value= g_ZERO_c;

		/*set enhanced color mode to 0x03 */
			enhanced_color_mode=0x03 ;


		/*Get transition time field value */
			memUtils_memCopy((uint8_t*)&(p_Lighting_Event->event_data.enhanced_move_to_hue_and_saturation_command.
							 a_transition_time), &(pReceivedAsdu[6]),2 );
			p_data = &( p_Lighting_Event->event_data.enhanced_move_to_hue_and_saturation_command.saturation);

		/*Check saturation value */

		if( *p_data == m_MAX_VALUE_c ) {
			status = g_ZCL_Invalid_Value_c;
		}

				/*check default response require or not */
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
				responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, status );
				pActionHandler->action = Send_Response;
				return responseLength;
			}
		break ;

	case g_Enhanced_Step_Hue_Command_c :


		/* Loading the event id, event length and event data */
			pActionHandler->event.eventId = g_Enhanced_Step_Hue_Command_Event_c;
		/*Get step mode field value */
			p_Lighting_Event->event_data.enhanced_step_hue_command.step_mode = pReceivedAsdu[3];

		/*Get step size field value */
			memUtils_memCopy((uint8_t*)&(p_Lighting_Event->event_data.enhanced_step_hue_command.
							 step_size), &(pReceivedAsdu[4]),2 );

		/*Get transition time field value */
			memUtils_memCopy((uint8_t*)&(p_Lighting_Event->event_data.enhanced_step_hue_command.
							 transition_time), &(pReceivedAsdu[6]),2 );

		/*set color mode to zero */
			color_mode_value= g_ZERO_c;

		/*set enhanced color mode to 0x03 */
			enhanced_color_mode=0x03 ;

					/*check default response require or not */
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
				responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, status );
				pActionHandler->action = Send_Response;
				return responseLength;
			}

		break ;

	case g_Move_Color_Temperature_Command_c :

		/*load event id */
			pActionHandler->event.eventId = g_Move_Color_Temperature_Event_c;

		/*Get move mode field */
			p_Lighting_Event->event_data.move_color_temperature.move_mode = pReceivedAsdu[3];

		/*Get rate field value */
			memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_color_temperature.rate),
				&(pReceivedAsdu[4]),2 );

		/*Get color temperature mininum field value */
			memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_color_temperature.color_temp_min),
				&(pReceivedAsdu[6]),2 );

		/*Get color temperature maximum field value  */
			memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.move_color_temperature.color_temp_max),
				&(pReceivedAsdu[8]),2 );

		/*set color mode value to 0x02 */
			color_mode_value= 0x02;

		/*set enhanced color mode value to 0x02 */
			enhanced_color_mode=0x02 ;

					/*check default response require or not */
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
				responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, status );
				pActionHandler->action = Send_Response;
				return responseLength;
			}

		break ;

	case g_Step_Color_Temperature_Command_c :

		/*load event id */
			pActionHandler->event.eventId = g_Step_Color_Temperature_Event_c;

		/*Get step mode field value */
			p_Lighting_Event->event_data.step_color_temperature.step_mode = pReceivedAsdu[3];

		/*Get step size field value */
			memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.step_color_temperature.step_size),
				&(pReceivedAsdu[4]),2 );

		/*Get transition time field value */
			memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.step_color_temperature.transition_time),
				&(pReceivedAsdu[6]),2 );

		/*Get color temperature minimum field value */
			memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.step_color_temperature.color_temp_min),
				&(pReceivedAsdu[8]),2 );

		/*Get color temperature maximum field value */
			memUtils_memCopy ((uint8_t*)&(p_Lighting_Event->event_data.step_color_temperature.color_temp_max),
				&(pReceivedAsdu[10]),2 );

		/*set color mode value to 0x02 */
			color_mode_value= 0x02;

		/*set enhanced color mode value to 0x02 */
			enhanced_color_mode=0x02 ;

				/*check default response require or not */
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
				responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, status );
				pActionHandler->action = Send_Response;
				return responseLength;
			}
		break ;

	case g_Color_Loop_Set_Command_c :

		/*load event id */
			pActionHandler->event.eventId = g_Color_Loop_Set_Event_c;

		/*Get update flags field value */
			p_Lighting_Event->event_data.color_loop_set.update_flags = pReceivedAsdu[3];

		/*Get action field value */
			p_Lighting_Event->event_data.color_loop_set.action = pReceivedAsdu[4];

		/*Get direction field value */
			p_Lighting_Event->event_data.color_loop_set.direction = pReceivedAsdu[5];

		/*Get transition time field value */
			memUtils_memCopy (/*(uint8_t*)&*/(p_Lighting_Event->event_data.color_loop_set.a_transition_time),
				&(pReceivedAsdu[6]),2 );



		/*Get start hue field value */
			memUtils_memCopy (/*(uint8_t*)&*/(p_Lighting_Event->event_data.color_loop_set.start_hue),
				&(pReceivedAsdu[8]),2 );


					/*check default response require or not */
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
				responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, status );
				pActionHandler->action = Send_Response;
				return responseLength;
			}

		break ;



	case g_Stop_Move_Step_Command_c :

		/*load event id */
			pActionHandler->event.eventId = g_Stop_Move_Step_Event_c;




			/*check default response require or not */
				if ( Is_Default_Response_Required(pReceivedAsdu)) {
				responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
															   pResponseAsduStartLocation, status );
				pActionHandler->action = Send_Response;
				return responseLength;
			}
		break ;
#endif  /*g_ZLL_COLOR_CONTROL_CLUSTER_ENABLE_d */

 #endif                     /* g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d */
	default:
		return responseLength;
		
	}
#if ( g_COLOR_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )

    Read_Write_Color_Control_Attributes(g_Write_Attribute_Data_c,
                                        &color_mode_value,
                                        g_COLOR_MODE_ATTRIBUTE_c);


    if ( defaultResponseRequired) {
			responseLength = ZCL_CreateDefaultResponseCommand ( pReceivedAsdu,
                    pResponseAsduStartLocation, status );
			pActionHandler->action = Send_Response;

		}
    return responseLength;
#endif

	
}

/*---------------------------------------------------------------------------*/

void Read_Write_Color_Control_Attributes
(
   uint8_t read_or_write,
   uint8_t *attrib_value,
   uint16_t attrib_id
)
{
   ZCL_Read_Write_Attribute_Data
   (
      read_or_write,
      gp_Color_Control_Attribute_Data_List,
      gp_Color_Control_Cluster_Info,
      attrib_id,
      attrib_value

   );
}


/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */
#endif
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
