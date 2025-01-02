/*******************************************************************************
* ZCL_Windows_Covering_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Identify cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "stack_common.h"
#include "ZCL_Common.h"
#include "ZCL_Foundation.h"
#include "ZCL_Stack_Utility.h"
#include "ZCL_Interface.h"
#include "memory_utility.h"
#include "common_includes.h"
#if ( g_WINDOW_COVERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Windows_Covering_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_PROGRAM_SETPOINT_CMD_LENGTH_c								0x04


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
/* Attribute information for Color Control Cluster supported attributes */
#if ( g_WINDOW_COVERING_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Windows_Covering_Attribute_Info [] =
{

	{
		g_WINDOW_COVERING_TYPE_ATTRIBUTE_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	},

	#if ( g_PHYSICAL_CLOSED_LIMIT_LIFT_ATTRIBUTE_ENABLE_d == 1)
	{
		g_PHYSICAL_CLOSED_LIMIT_LIFT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	}
	#endif             /* g_PHYSICAL_CLOSED_LIMIT_LIFT_ATTRIBUTE_ENABLE_d  */
	,
	#if ( g_PHYSICAL_CLOSED_LIMIT_TILT_ATTRIBUTE_ENABLE_d == 1 )
	{

		g_PHYSICAL_CLOSED_LIMIT_TILT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	}
	#endif             /* g_PHYSICAL_CLOSED_LIMIT_LIFT_ATTRIBUTE_ENABLE_d  */
	,
	#if ( g_CURRENT_POSITION_LIFT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_POSITION_LIFT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	}
	#endif             /* g_CURRENT_POSITION_LIFT_ATTRIBUTE_ENABLE_d  */
	,
	#if ( g_CURRENT_POSITION_TILT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_POSITION_TILT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	}
	#endif            /* g_CURRENT_POSITION_TILT_ATTRIBUTE_ENABLE_d  */
	,
	#if ( g_NUMBER_OF_ACTUATIONS_LIFT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_NUMBER_OF_ACTUATIONS_LIFT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	}
	#endif             /* g_NUMBER_OF_ACTUATIONS_LIFT_ATTRIBUTE_ENABLE_d  */
	,
	#if ( g_NUMBER_OF_ACTUATIONS_TILT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_NUMBER_OF_ACTUATIONS_TILT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	}
	#endif            /* g_NUMBER_OF_ACTUATIONS_TILT_ATTRIBUTE_ENABLE_d  */
	,
	{
		g_CONFIG_STATUS_ATTRIBUTE_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},

	#if ( g_CURRENT_POSITION_LIFT_PERCENTAGE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_POSITION_LIFT_PERCENTAGE_ATTRIBUTE_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Scenes_Ext_Attribute_c
	}
	#endif           /* g_CURRENT_POSITION_LIFT_PERCENTAGE_ATTRIBUTE_ENABLE_d  */
	,
	#if ( g_CURRENT_POSITION_TILT_PERCENTAGE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_POSITION_TILT_PERCENTAGE_ATTRIBUTE_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Scenes_Ext_Attribute_c
	}
	#endif           /* g_CURRENT_POSITION_TILT_PERCENTAGE_ATTRIBUTE_ENABLE_d  */
	,
	{
		g_INSTALLED_OPEN_LIMIT_LIFT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
	{
		g_INSTALLED_CLOSED_LIMIT_LIFT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
	{
		g_INSTALLED_OPEN_LIMIT_TILT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
	{
		g_INSTALLED_CLOSED_LIMIT_TILT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},

	#if ( g_VELOCITY_LIFT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_VELOCITY_LIFT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Write_Attribute_c
	}
	#endif         /* g_VELOCITY_LIFT_ATTRIBUTE_ENABLE_d  */
	,
	#if ( g_ACCELERATION_TIME_LIFT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_ACCELERATION_TIME_LIFT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Write_Attribute_c
	}
	#endif         /* g_ACCELERATION_TIME_LIFT_ATTRIBUTE_ENABLE_d  */
	,
	#if ( g_DECELERATION_TIME_LIFT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_DECELERATION_TIME_LIFT_ATTRIBUTE_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Write_Attribute_c
	}
	#endif          /* g_DECELERATION_TIME_LIFT_ATTRIBUTE_ENABLE_d  */
	,
	{
		g_MODE_ATTRIBUTE_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	}
	,
	#if ( g_INTERMEDIATE_SET_POINTS_LIFT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_INTERMEDIATE_SET_POINTS_LIFT_ATTRIBUTE_c,
		g_Octet_String_Data_c,
		g_Read_Write_Attribute_c
	}
	#endif            /* g_INTERMEDIATE_SET_POINTS_LIFT_ATTRIBUTE_c  */
	,
	#if ( g_INTERMEDIATE_SET_POINTS_TILT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_INTERMEDIATE_SET_POINTS_TILT_ATTRIBUTE_c,
		g_Octet_String_Data_c,
		g_Read_Write_Attribute_c
	}
	#endif              /* g_INTERMEDIATE_SET_POINTS_TILT_ATTRIBUTE_ENABLE_d  */

};

const ZCL_Cluster_Info_t ga_Windows_Covering_Server_Cluster_Info =
{
   g_WINDOW_COVERING_CLUSTER_c,
   sizeof(ma_Windows_Covering_Attribute_Info)/sizeof(ZCL_Attribute_Info_t),
   ma_Windows_Covering_Attribute_Info,
};

#endif          /*  g_WINDOW_COVERING_SERVER_CLUSTER_ENABLE_d  */

#if ( g_WINDOW_COVERING_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Windows_Covering_Client_Cluster_Info =
{
   g_WINDOW_COVERING_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif          /* g_WINDOW_COVERING_CLIENT_CLUSTER_ENABLE_d == 1 */

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


/******************************************************************************/
uint8_t ZCL_Handle_Window_Covering_Cluster_Cmd
(
     uint8_t endPointId,
     uint8_t asduLength,
     uint8_t* pReceivedAsdu,
     uint8_t* pResponseAsduStartLocation,
     ZCL_ActionHandler_t *pActionHandler,
     ZCL_Cluster_Info_t const *pZCLClusterInfo,
     uint8_t * pAttrsDataBaseLocation
)
{

    uint8_t responseLength= g_ZERO_c;   /* return value */
#if ( g_WINDOW_COVERING_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t status = g_ZCL_Success_c;
#endif
     /* pointer to load the lighting events */
    pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;
       if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ){
#if ( g_WINDOW_COVERING_SERVER_CLUSTER_ENABLE_d == 1 )
            switch(  pReceivedAsdu[ GetZCLHeaderLength( pReceivedAsdu )-1] )
            {
                case g_Up_Open_Command_c:
                    pActionHandler->event.eventId = g_Up_Open_Command_Event_c;
                    status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
                    /*Hard code should be added here*/
                break;
                case g_Down_Close_Command_c:
                    pActionHandler->event.eventId = g_Down_Close_Command_Event_c;
                    status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
                    /*Hard code should be added here*/
                break;
                case g_Windows_Stop_Command_c:
                    pActionHandler->event.eventId = g_Stop_Command_Event_c;
                    status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
                    /*Hard code should be added here*/
                break;
                case g_Go_To_Lift_Set_Point_Command_c:

                    pActionHandler->event.eventId = g_Go_To_Lift_Set_Point_Command_Event_c;
                   *pActionHandler->event.pEventData = pReceivedAsdu[3];
                   status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
                break;
                case g_Go_To_Lift_Value_Command_c:
                    pActionHandler->event.eventId = g_Go_To_Lift_Value_Command_Event_c;
                    memUtils_memCopy ( pActionHandler->event.pEventData , &pReceivedAsdu[3],2);
                    status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);

                break;
                case g_Go_To_Lift_Percentage_Command_c:
                    pActionHandler->event.eventId = g_Go_To_Lift_Percentage_Command_Event_c;
                    *pActionHandler->event.pEventData = pReceivedAsdu[3];
                    status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
                break;
                case g_Go_To_Tilt_Set_Point_Command_c:
                    pActionHandler->event.eventId = g_Go_To_Tilt_Set_Point_Command_Event_c;
                    *pActionHandler->event.pEventData = pReceivedAsdu[3];
                    status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
                break;
                case g_Go_To_Tilt_Value_Command_c:
                    pActionHandler->event.eventId = g_Go_To_Tilt_Value_Command_Event_c;
                    memUtils_memCopy ( pActionHandler->event.pEventData , &pReceivedAsdu[3],2);
                    status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
                break;
                case g_Go_To_Tilt_Percentage_Command_c:
                    pActionHandler->event.eventId = g_Go_To_Tilt_Percentage_Command_Event_c;
                    *pActionHandler->event.pEventData = pReceivedAsdu[3];
                    status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
                break;
                case g_Program_Set_Point_Command_c:
					if ( asduLength > m_PROGRAM_SETPOINT_CMD_LENGTH_c) {
						pActionHandler->event.eventId = g_Program_Set_Point_Command_Version1_Event_c;
						*pActionHandler->event.pEventData = pReceivedAsdu[3];
						*( pActionHandler->event.pEventData + 1 ) = pReceivedAsdu[4];
						memUtils_memCopy ( pActionHandler->event.pEventData + 2, &pReceivedAsdu[5],2);
						status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
					}
					else {
						pActionHandler->event.eventId = g_Program_Set_Point_Command_Version2_Event_c;
						*pActionHandler->event.pEventData = pReceivedAsdu[3];
						status = ZCL_CallBack_Windows_Covering(&pActionHandler->event);
					}

                break;
            }
			if ( Is_Default_Response_Required ( pReceivedAsdu ) ) {
			responseLength = ZCL_CreateDefaultResponseCommand (
								  pReceivedAsdu,
								  pResponseAsduStartLocation,
								  status );
			pActionHandler->action = Send_Response;

			}
			else{
			pActionHandler->action = No_Response;
			}
#endif 					/* g_WINDOW_COVERING_SERVER_CLUSTER_ENABLE_d */
       }
    return responseLength;

}

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

#endif                                   /* g_WINDOW_COVERING_CLUSTER_ENABLE_d  */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

