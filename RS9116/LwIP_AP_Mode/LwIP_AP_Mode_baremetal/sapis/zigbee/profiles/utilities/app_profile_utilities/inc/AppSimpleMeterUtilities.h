/******************************************************************************
* AppSimpleMeterUtilities.h
******************************************************************************
* Program Description:
* This file contains the SimpleMeter cluster App type definations
******************************************************************************/
#ifndef _APP_SIMPLE_METER_UTILITIES_H_
#define _APP_SIMPLE_METER_UTILITIES_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_DAILY_IN_SECONDS_c                                86400
#define g_SIXTY_MINUTES_IN_SECONDS_c                        3600
#define g_THIRTY_MINUTES_IN_SECONDS_c                       1800
#define g_FIFTEEN_MINUTES_IN_SECONDS_c                      900
#define g_TEN_MINUTES_IN_SECONDS_c                          600
#define g_SEVEN_AND_HALF_MINUTES_IN_SECONDS_c               450
#define g_FIVE_MINUTES_IN_SECONDS_c                         300
#define g_TWO_AND_HALF_MINUTES_IN_SECONDS_c                 150
#define g_TWENTY_SECONDS_c	                            20
#define g_THIRTY_SECONDS_c	                            30
#define g_SIXTY_SECONDS_c                                   60
#define g_FIFTEEN_MINUTES_c                                 15

/*It define no.of interval supported by the meter*/
#define g_METER_NUM_PROFILES_SUPPORTED_c                    0x05

/*It defines Meter supporting profileIntervalPeriod.We can select any one of the
  above profile Interval periods.*/
#define g_APP_PROFILE_INTERVAL_PERIOD_c               g_TWO_AND_HALF_MINUTES_IN_SECONDS_c


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct App_ProfileMeteringIntervals_tag
{
    uint32_t utcTime;
    uint8_t metringInterval[3];
} __attribute__((__packed__))App_ProfileMeteringIntervals_t;

typedef struct Mirror_Info_tag
{
    bool HasMirrorCapability;
    uint8_t MirrorEndPoint;
    bool deviceMirroring;
} __attribute__((__packed__))Mirror_Info_t;


typedef struct Meter_status_tag
{
    bool Meterflag;
	bool MeterReadingFlag;
    uint32_t MeterDataCapturedTime;
} __attribute__((__packed__))Meter_status_t;

#if ( g_SE_PROFILE_1_1_d == 1 )	
typedef struct App_fastPollModeIntervals_tag
{
  	bool fastPollFlag;
    uint8_t updatePeriod;
    uint32_t endTime;
	uint32_t PollCapturedTime;
} __attribute__((__packed__))App_fastPollModeIntervals_t;
#endif

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
void App_MeterEventsHandler( ZCL_Event_t *pZCLEvent,
                             APSDE_Data_Indication_t *pBuffer );
#endif

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
uint8_t App_Update_MeterReadings( void );
void App_MeterClusterTask( void );
void App_MeterClusterInit( void );
#endif

extern Meter_status_t Meter_status;
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif          /* _APP_SIMPLE_METER_UTILITIES_H_ */
