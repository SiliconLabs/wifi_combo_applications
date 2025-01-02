/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"
#include "ZigBeeDataTypes.h"
#include "stack_common.h"
#include "AppTimerUtilities.h"
#include "ApptimerId.h"
#include "memory_utility.h"
#include "sw_timer.h"
#include "ZCL_Common.h"
#include "nlme_interface.h"
#include "zdo_interface.h"
#include "ApplicationThinLayer.h"
#include "ZCL_Interface.h"
#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Time_Cluster.h"
#endif

#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )

Time_Cluster_t time_server_cluster_values;
#endif
/*-----------------------------------------------------------------------------
* Global constant declarations
*----------------------------------------------------------------------------*/
#define m_DEFAULT_YEAR_c                                0x10
#define m_DEFAULT_MONTH_c                               0x08
#define m_DEFAULT_DAY_c                                 0x30
#define m_DEFAULT_HOUR_c                                0x00
#define m_DEFAULT_MINUTE_c                              0x00
#define m_DEFAULT_SECOND_c                              0x00

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

static uint32_t currentTime = 0x00000000;
static uint32_t currentTick = 0x00000000;

#if ( g_TIME_CLUSTER_ENABLE_d == 1 )
extern Time_Cluster_t time_server_cluster_values;
#endif

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

static const uint8_t amonthDaysTable[] =
{
    g_THIRTY_ONE_DAYS_c  ,
    g_TWENTY_EIGHT_DAYS_c,
    g_THIRTY_ONE_DAYS_c  ,
    g_THIRTY_DAYS_c      ,
    g_THIRTY_ONE_DAYS_c  ,
    g_THIRTY_DAYS_c      ,
    g_THIRTY_ONE_DAYS_c  ,
    g_THIRTY_ONE_DAYS_c  ,
    g_THIRTY_DAYS_c      ,
    g_THIRTY_ONE_DAYS_c  ,
    g_THIRTY_DAYS_c      ,
    g_THIRTY_ONE_DAYS_c
};

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

void App_UpdateUTCTimeCallBack( uint8_t timerId );

void App_setRTCtime( elapsedTimeInfo_t* timeInfo );

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint32_t APP_ConvertYearToSeconds( elapsedTimeInfo_t *elapsedTime )
{
    uint8_t i;
    uint32_t numOfSecondsElapsed = 0x00;
    uint32_t numOfDaysElapsed = 0x00;

    numOfDaysElapsed += elapsedTime->year * g_NUMBER_OF_DAYS_IN_A_YEAR_c;
    numOfDaysElapsed += elapsedTime->year / g_DIVISOR_FOR_LEAP_YEAR_c + 0x01;

    /* if the year is a leap year and the month is greater than
       february, increment the day by one for leap year */
    if( ( ( ( elapsedTime->year ) % g_DIVISOR_FOR_LEAP_YEAR_c ) == 0x00 ) &&
                ( ( elapsedTime->month ) > february ) ) {
         numOfDaysElapsed++;
    }
    /* calculate the days elapsed considering the months elapsed in the current
       year leaving current month */
    for( i = 0x00 ; i < (elapsedTime->month - 1) ; i++ ) {
        numOfDaysElapsed +=  amonthDaysTable[i];
    }
    numOfDaysElapsed += elapsedTime->day - 1;
    /* calculate the seconds for the days elapsed */
    numOfSecondsElapsed = numOfDaysElapsed * g_NUMBER_OF_SECONDS_IN_A_DAY_c;

    numOfSecondsElapsed += (((uint32_t)elapsedTime->hour * g_NUMBER_OF_SECONDS_IN_AN_HOUR_c)+
    ((uint32_t)elapsedTime->minute*g_NUMBER_OF_SECONDS_IN_A_MINUTE_c )+(elapsedTime->second));

    return numOfSecondsElapsed;
}

/**************************************************************************************/

void App_setRTCtime( elapsedTimeInfo_t* timeInfo )
{
    /* if RTC present write a code for Updating the RTC */
    currentTime = APP_ConvertYearToSeconds( timeInfo );

}

/**************************************************************************************/
void ZCL_callbackSetRTCtime( uint32_t utctime )
{
    /* if RTC present then convert utctime to RTC format and
       set the value */
    currentTime = utctime;
}

/**************************************************************************************/
void App_TimerInit( void )
{
    elapsedTimeInfo_t defaultTime;

    defaultTime.year   = m_DEFAULT_YEAR_c;
    defaultTime.month  = m_DEFAULT_MONTH_c;
    defaultTime.day    = m_DEFAULT_DAY_c;
    defaultTime.hour   = m_DEFAULT_HOUR_c;
    defaultTime.minute = m_DEFAULT_MINUTE_c;
    defaultTime.second = m_DEFAULT_SECOND_c;

    App_setRTCtime( &defaultTime );

    currentTick = 0x00;
}

/*******************************************************************************************/

void App_UpdateUTCTimeCallBack( uint8_t timerId )
{
   currentTime++;
   currentTick++;
   tmr_startRelativeTimer( g_APP_SYSTEM_UTC_TIMER_c, g_TIMER_COUNT_ONE_SECOND_c,
                         App_UpdateUTCTimeCallBack );

}

/*******************************************************************************************/
extern stack_is_up;
void App_checkFreeRunTimer( void )
{
    //if((rsi_zigb_stack_is_up())&&(!Is_Timer_Running(g_APP_SYSTEM_UTC_TIMER_c))){
    if(stack_is_up && (!Is_Timer_Running(g_APP_SYSTEM_UTC_TIMER_c)) ){
        if( (tmr_startRelativeTimer( g_APP_SYSTEM_UTC_TIMER_c,
                             g_TIMER_COUNT_ONE_SECOND_c, App_UpdateUTCTimeCallBack )) != 0x01)
        {
        	while(1);
        }
    }

}
/*******************************************************************************************/

uint32_t ZCL_CallBackGetCurrentTime( void )
{
    return currentTime;
}

/*******************************************************************************************/

uint32_t App_getcurrentTick( void )
{
    return currentTick;
}

/*******************************************************************************************/
uint32_t App_getDifferance(uint32_t a, uint32_t b )
{
    uint32_t diff;
    if ( a >= b ) {
        diff = a - b;
    }
    else {
        diff = b - a;
    }
    return diff;
}
/*******************************************************************************************/
/* Application calls this API to configure UTC time
 * */
/*******************************************************************************************/

uint8_t App_configureUTCTime(uint32_t UTC_Time)
{
#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
	ZCL_callbackSetRTCtime(UTC_Time);
	ZCL_SetTimeAttribute((uint8_t *)&time_server_cluster_values ,UTC_Time );

	return g_ZCL_Success_c;
#else
	return g_ZCL_Failure_c;
#endif
}

/*******************************************************************************************/
/* Application calls this API to configure time
 * */
/*******************************************************************************************/

uint8_t App_configureTime(uint8_t year, uint8_t month, uint8_t day,
		uint8_t hour, uint8_t minute, uint8_t second)
{
#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
	elapsedTimeInfo_t rtcTime;
	rtcTime.year = year;
	rtcTime.month = month;
	rtcTime.day = day;
	rtcTime.hour = hour;
	rtcTime.minute = minute;
	rtcTime.second = second;
    App_setRTCtime( &rtcTime );

    ZCL_SetTimeAttribute((uint8_t *)&time_server_cluster_values ,currentTime );
    currentTick = 0x00;
    return g_ZCL_Success_c;
#else

    return g_ZCL_Failure_c;
#endif

}
/*******************************************************************************************/
/* Application calls this API to configure time
 * */
/*******************************************************************************************/
uint32_t App_readRTCTime(void)
{
	uint32_t rtcTime = 0xffffffff;
#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
	App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(), g_TIME_CLUSTER_c,g_TIME_ATTRIBUTE_c,
							(uint8_t *)&rtcTime, g_Read_Attribute_Data_c );
#endif

	return rtcTime;
}


/*******************************************************************************************/
/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
