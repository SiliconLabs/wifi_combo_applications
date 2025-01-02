/*******************************************************************************
* ZCL_General_On_Off_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Onoff cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Common.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#include "memory_utility.h"
#if ( g_ON_OFF_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_On_Off_Cluster.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_On_Off_Cluster_Attribute_Info[]  =
{
    {
       g_ON_OFF_ATTRIBUTE_c,
       g_Boolean_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c
    }
#if(g_HA_1_2_d == 1)
#if (g_MAX_DURATION_ATTRIBUTE_ENABLE_d == 1 )
   ,{
       g_MAX_DURATION_ATTRIBUTE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c
    }
#endif
#if (g_ON_DURATION_ATTRIBUTE_ENABLE_d == 1 )
	,{
       g_ON_DURATION_ATTRIBUTE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c
    }
#endif
#endif /* #if(g_HA_1_2_d == 1)*/
 #if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    ,{
       g_GLOBAL_SCENE_CONTROL_ATTRIBUTE_c,
       g_Boolean_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c
    }
    ,{
       g_ON_TIME_ATTRIBUTE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c
    }
    ,{
       g_OFF_WAIT_TIME_ATTRIBUTE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c | g_Scenes_Ext_Attribute_c
    }
#endif /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d */

};

const ZCL_Cluster_Info_t ga_On_Off_Server_Cluster_Info =
{
   g_ON_OFF_CLUSTER_c,
   sizeof(ma_On_Off_Cluster_Attribute_Info)/sizeof(ZCL_Attribute_Info_t) ,
   ma_On_Off_Cluster_Attribute_Info
};
#endif              /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d  */

#if ( g_ON_OFF_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_On_Off_Client_Cluster_Info =
{
   g_ON_OFF_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif              /*  g_ON_OFF_CLIENT_CLUSTER_ENABLE_d */

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

uint8_t ZCL_Handle_OnOff_Cluster_Cmd (uint8_t endPointId, uint8_t asduLength,
                                    uint8_t* pReceivedAsdu,
                                    uint8_t* pResponseAsduStartLocation,
                                    ZCL_ActionHandler_t *pActionHandler,
                                    ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                    uint8_t * pAttrsDataBaseLocation )
{
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t state=0x00;                              /* State of light */
#endif	
    uint8_t responseLength= g_ZERO_c;           /* return variable */
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
	uint16_t attribute_value = 0x0000;
#endif	/* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
#if(g_HA_1_2_d == 1)
	uint16_t maxDuration;
	uint8_t status;
#endif	

    if(!(pReceivedAsdu[0] & g_ZCL_SERVER_TO_CLIENT_DIRECTION_c)) {
#if ( g_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
	  	pActionHandler->action = Send_Response;
		pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;
        switch( pReceivedAsdu[ GetZCLHeaderLength(pReceivedAsdu) - 0x01 ] )
        {
            case g_Off_Command_c:
                state = g_OFF_c;                 /* State updated as OFF */
				pActionHandler->event.eventId = g_On_Off_Cluster_Off_Event_c ;
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )

                   ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
                                    pAttrsDataBaseLocation,
                                    pZCLClusterInfo,
                                    g_ON_TIME_ATTRIBUTE_c,
                                    (uint8_t*)&attribute_value  );
#endif   /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
                break;
            case g_On_Command_c:
                state = g_ON_c;                 /* State updated as ON */
                pActionHandler->event.eventId = g_On_Off_Cluster_On_Event_c ;
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )

ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
                                    pAttrsDataBaseLocation,
                                    pZCLClusterInfo,
                                    g_OFF_WAIT_TIME_ATTRIBUTE_c,
                                    (uint8_t*)&attribute_value );

  if ( attribute_value == g_ZERO_c)
                ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
                                    pAttrsDataBaseLocation,
                                    pZCLClusterInfo,
                                    g_OFF_WAIT_TIME_ATTRIBUTE_c,
                                    (uint8_t*)&attribute_value );
#endif  /* g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
                break;
            case g_Toggle_Command_c:
			   pActionHandler->event.eventId = g_On_Off_Cluster_Toggle_Event_c ;
			  memUtils_memCopy( pActionHandler->event.pEventData,
							pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							pActionHandler->event.eventLength  );
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
               ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,
                                pAttrsDataBaseLocation,
                                pZCLClusterInfo,
                                g_ON_TIME_ATTRIBUTE_c,
                                (uint8_t*)&attribute_value  );
               ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,
                                pAttrsDataBaseLocation,
                                pZCLClusterInfo,
                                g_ON_OFF_ATTRIBUTE_c,
                                &state );
               if ( state == 0x00 && attribute_value == 0x0000)
               ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
                                    pAttrsDataBaseLocation,
                                    pZCLClusterInfo,
                                    g_OFF_WAIT_TIME_ATTRIBUTE_c,
                                    (uint8_t*)&attribute_value  );
               else if ( state == 0x01)
               ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
                                    pAttrsDataBaseLocation,
                                    pZCLClusterInfo,
                                    g_ON_TIME_ATTRIBUTE_c,
                                    (uint8_t*)&attribute_value );
#endif
                if (g_OFF_c == *pAttrsDataBaseLocation ) {
                state = g_ON_c;
                }
                else {
                state = g_OFF_c;
                }

                pActionHandler->event.eventId = g_On_Off_Cluster_Toggle_Event_c ;
                break;
#if ( g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d == 1 )
		/*off with effect */
        case g_Off_With_Effect_Command_c:
		/*load event id */
            pActionHandler->event.eventId = g_Off_With_Effect_Event_c ;

		/*get effect identifier */
            *pActionHandler->event.pEventData = pReceivedAsdu[3];

		/*effect variant field */
            *( pActionHandler->event.pEventData + 1 ) = pReceivedAsdu[4];
		/*apply required effect */

            ZLLUtilOnOffHandler(&pActionHandler->event);

            break;
			/*on with global recall */
        case g_On_With_Recall_Global_Scene_Command_c:

		/*load event id */
            pActionHandler->event.eventId = g_On_With_Recall_Global_Scene_Event_c ;

            ZLLUtilOnOffHandler(&pActionHandler->event);
            break;
		/*on with timed off */
        case g_On_With_Timed_Off_Command_c:
		/*load event id */
            pActionHandler->event.eventId = g_On_With_Timed_Off_Event_c ;

		/*OnOff control field */
            *pActionHandler->event.pEventData = pReceivedAsdu[3];

		/*On time field */
            memUtils_memCopy ( pActionHandler->event.pEventData + 1, &pReceivedAsdu[4],2);

		/*Off wait time field */
            memUtils_memCopy ( pActionHandler->event.pEventData + 3, &pReceivedAsdu[6],2);

            ZLLUtilOnOffHandler(&pActionHandler->event);
            break;
#endif          /*g_ZLL_ON_OFF_SERVER_CLUSTER_ENABLE_d*/
				
#if(g_HA_1_2_d == 1)
			case g_On_With_Duration_command_c :
				pActionHandler->event.eventId = g_On_Off_Cluster_On_With_Duration_Event_c ;
				memUtils_memCopy( pActionHandler->event.pEventData,
							pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							pActionHandler->event.eventLength  );
				
				ZCL_Read_Write_Attribute_Data ( g_Read_Attribute_Data_c,
                                pAttrsDataBaseLocation,
                                pZCLClusterInfo,
                                g_MAX_DURATION_ATTRIBUTE_c,
                                (uint8_t*)&maxDuration  );
				if(maxDuration == 0x0000) {
					status =  g_ZCL_Unsupported_Attribute_c;
				}
				else {
					ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
                                pAttrsDataBaseLocation,
                                pZCLClusterInfo,
                                g_ON_DURATION_ATTRIBUTE_c,
                                &pReceivedAsdu[ GetZCLHeaderLength(pReceivedAsdu)]  );
					status =  g_ZCL_Success_c ;
				}
			
				/* Creating default response */
				if ( !(pReceivedAsdu[0] &g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c)) {
					return (ZCL_CreateDefaultResponseCommand (pReceivedAsdu ,
									pResponseAsduStartLocation, status));
				}
				else {
					pActionHandler->action = No_Response;
				}
			return responseLength;
#endif			

            default :
			  break;

        }
        ZCL_Read_Write_Attribute_Data ( g_Write_Attribute_Data_c,
                                        pAttrsDataBaseLocation,
                                        pZCLClusterInfo,
                                        g_ON_OFF_ATTRIBUTE_c,
                                        &state );
        /* Creating default response */
           if ( !(pReceivedAsdu[0] &g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c)) {
                    return (ZCL_CreateDefaultResponseCommand (pReceivedAsdu ,
                                    pResponseAsduStartLocation, g_ZCL_Success_c));
         }
		 else {
			 pActionHandler->action = No_Response;
		   }

#endif          /* g_ON_OFF_SERVER_CLUSTER_ENABLE_d */
    }
    return responseLength;

}

#endif
/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
