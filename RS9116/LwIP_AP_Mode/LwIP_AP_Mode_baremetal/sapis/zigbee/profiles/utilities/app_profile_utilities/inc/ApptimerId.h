#ifndef _APP_TIMER_ID_H_
#define _APP_TIMER_ID_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"
#include "timer_id.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/* General domain timers */
//#define g_APP_SYSTEM_UTC_TIMER_c             g_NWK_TOTAL_NUMBER_OF_TIMERS_c
#define g_APP_SYSTEM_UTC_TIMER_c              1
#define g_APPLICATION_MOVE_TO_LEVEL_TIMER_c ( g_APP_SYSTEM_UTC_TIMER_c + 1 )
#define m_APPLICATION_LIGHTING_TIMER_ONE_c  ( g_APPLICATION_MOVE_TO_LEVEL_TIMER_c + 1 )
#define m_APPLICATION_COMMI_RESTART_TIMER_c ( m_APPLICATION_LIGHTING_TIMER_ONE_c + 1)
#define m_APPLICATION_XMODEM_TIMER_c        (m_APPLICATION_COMMI_RESTART_TIMER_c+1)
#define m_APPLICATION_WINDOW_COVERING_TIMER_c  (m_APPLICATION_XMODEM_TIMER_c +1)
#define m_APPLICATION_ON_OFF_TIMER_c  (m_APPLICATION_WINDOW_COVERING_TIMER_c +1)

#define m_APP_ZLL_TICK_TIMER_c		(m_APPLICATION_ON_OFF_TIMER_c + 1)

#ifdef HA_SA
#define g_HA_SAMPLE_APP_TIMER_c			g_APP_SYSTEM_UTC_TIMER_c + 1
#define g_HA_SA_POLL_TIMER			g_HA_SAMPLE_APP_TIMER_c + 1
#define LAST_TIMER   g_HA_SAMPLE_APP_TIMER_c
#endif //HA_SA


#ifdef SE_SA
#define g_SE_SAMPLE_APP_TIMER_c			g_APP_SYSTEM_UTC_TIMER_c + 1
#define g_SE_SAMPLE_APP_EVENT_TIMER_c	g_SE_SAMPLE_APP_TIMER_c + 1
#define g_SE_BROADCAST_DELAY_TIMER_c	g_SE_SAMPLE_APP_EVENT_TIMER_c + 1
#define LAST_TIMER   g_SE_BROADCAST_DELAY_TIMER_c
#endif //SE_SA


#ifdef g_ZLL_LIGHT_CONTROLLER_c
#define m_APP_ZLL_INTERPAN_LIFETIME_TIMER_c  (m_APP_ZLL_TICK_TIMER_c +1)
#define LAST_TIMER   m_APP_ZLL_INTERPAN_LIFETIME_TIMER_c
#endif /*g_ZLL_LIGHT_CONTROLLER_c*/

#ifdef ZLL_SA
#define g_ZLL_SAMPLE_APP_TIMER_c            ( g_APP_SYSTEM_UTC_TIMER_c + 1)
#define g_ZLL_SAMPLE_APP_EVENT_TIMER_c	    g_ZLL_SAMPLE_APP_TIMER_c + 1
#define LAST_TIMER   g_ZLL_SAMPLE_APP_EVENT_TIMER_c
#endif //#ifdef g_ZLL_LIGHT_DEVICE_c

#define APP_TIMER_ID LAST_TIMER + 1 

#define g_TIMER_COUNT_ONE_SECOND_c                      0xF4240

extern uint8_t ga_Timer_Link_Array[];

#endif