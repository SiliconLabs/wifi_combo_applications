/******************************************************************************
* AppTimerUtilities.h
******************************************************************************
* Program Description:
* This file contains the Price cluster AppRTC type definations
******************************************************************************/
#ifndef _APPTIMERUTILITIES_H_
#define _APPTIMERUTILITIES_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_THIRTY_DAYS_c                                           0x1E
#define g_THIRTY_ONE_DAYS_c                                       0x1F
#define g_TWENTY_EIGHT_DAYS_c                                     0x1C
#define g_NUMBER_OF_MONTHS_c                                      0x0C
#define g_NUMBER_OF_MONTHS_IN_A_YEAR_c                            0x0C
#define g_NUMBER_OF_DAYS_IN_A_YEAR_c                              0x16D
#define g_NUMBER_OF_HOURS_IN_A_DAY_c                              0x18
#define g_NUMBER_OF_MINUTES_IN_AN_HOUR_c                          0x3C
#define g_NUMBER_OF_SECONDS_IN_A_MINUTE_c                         0x3C
#define g_NUMBER_OF_SECONDS_IN_AN_HOUR_c                          0xE10
#define g_DIVISOR_FOR_LEAP_YEAR_c                                 0x04

#define g_NUMBER_OF_SECONDS_IN_A_YEAR_c                           0x1E13380
#define g_NUMBER_OF_SECONDS_IN_A_DAY_c                            0x15180


typedef enum months_Tag
{
    january     = 0x01,
    february    = 0x02,
    march       = 0x03,
    april       = 0x04,
    may         = 0x05,
    june        = 0x06,
    july        = 0x07,
    august      = 0x08,
    september   = 0x09,
    october     = 0x0a,
    november    = 0x0b,
    december    = 0x0c
}months_t;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct elapsedtimeInfo_Tag
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}__attribute__((__packed__))elapsedTimeInfo_t;

/*---------------------------------------------------------------------------
   Function Prototypes
---------------------------------------------------------------------------*/

uint32_t APP_ConvertYearToSeconds( elapsedTimeInfo_t *elapsedTime );
void App_setRTCtime( elapsedTimeInfo_t* timeInfo );
uint32_t App_getcurrentTick( void );
void App_TimerInit( void );
uint32_t App_getDifferance(uint32_t a, uint32_t b );
void App_checkFreeRunTimer( void );

/*---------------------------------------------------------------------------
   Function Prototypes for APIs
---------------------------------------------------------------------------*/

uint32_t App_readRTCTime(void);
uint8_t App_configureTime(uint8_t year, uint8_t month, uint8_t day,
		uint8_t hour, uint8_t minute, uint8_t second);
uint8_t App_configureUTCTime(uint32_t UTC_Time);

#endif
/*----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

