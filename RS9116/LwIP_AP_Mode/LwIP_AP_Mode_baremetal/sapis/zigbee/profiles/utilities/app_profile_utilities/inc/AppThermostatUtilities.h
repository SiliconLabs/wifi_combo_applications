#ifndef _APPTHERMOSTATUTILITIES_H_
#define _APPTHERMOSTATUTILITIES_H_
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#if(g_HA_1_2_d == 1)
#define g_NUMBER_OF_TRANSITION_FOR_SEQUENCE_c 				3

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct App_Transition_Sequence_Tag
{
  	uint16_t transition_time;
	int16_t  heat_set_point;
	int16_t  cool_set_point;
}__attribute__((__packed__))App_Transition_Sequence_t;

typedef struct App_Set_weekly_Schedule_Tag
{
	uint8_t number_of_transitions_for_sequence;
	uint8_t day_of_week_for_sequence;
	uint8_t mode_for_sequence;
	App_Transition_Sequence_t transition_sequence[g_NUMBER_OF_TRANSITION_FOR_SEQUENCE_c];
}__attribute__((__packed__))App_Set_Weekly_Schedule_t;

enum  {
  g_app_heat_set_point_c =0x01,
  g_app_cool_set_point_c = 0x02,
  g_app_both_heat_and_cool_set_point_c= 0x03
};

typedef struct GetWeeklyScheduleCmdTxDetails_Tag
{
    uint8_t  dstAddrMode;
    uint8_t  dstAddr[8];
    uint16_t dstInterPANId;
    uint8_t  dstEndPoint;
    uint8_t  tranSeqNum;
}__attribute__((__packed__))GetWeeklyScheduleCmdTxDetails_t;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )

void App_WeeklyScheduleInit( void);
void App_Thermostat_Task(void);

#endif          /* ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 ) */
#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )

void App_ThermostatEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer );
#endif

#endif
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                      /* _APPIDENTIFYUTILITIES_H_ */
