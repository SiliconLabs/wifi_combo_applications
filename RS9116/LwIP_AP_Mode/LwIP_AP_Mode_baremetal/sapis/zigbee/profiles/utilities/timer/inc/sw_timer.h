/** @file timer.h
*
* @brief Provides Timer module APIs
*
*/

/*
Tool Chain: 
*/

/******************** (C) COPYRIGHT 2010 XXXXX *********************************
*
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, XXXXX SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
* CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF 
* SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION 
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS
*******************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include <stdbool.h>
#include "ZigBeeDataTypes.h"
extern uint8_t ga_Timer_Link_Array[];

/**************************** Public Macro Definitions ************************/

/** @brief Indicates that the current timer is the last in the list */
#define g_NO_TIMER_c            ((uint8_t)0xFE)

#define SYSTEM_TIME_LOW			system_low()//((FreeRunningTimerActive)?((g_MAX_FREE_RUNNING_TIMER_VALUE_c - rsi_802154_hal_read_current_timer_value()) & 0x0000FFFF):((g_MAX_FREE_RUNNING_TIMER_VALUE_c - CurrentSystemTime) & 0x0000FFFF))

#define SYSTEM_TIME				((FreeRunningTimerActive)?((g_MAX_FREE_RUNNING_TIMER_VALUE_c - rsi_802154_hal_read_current_timer_value())):((g_MAX_FREE_RUNNING_TIMER_VALUE_c - CurrentSystemTime)))

/** @brief Indicates that the current timer link is unused */
#define g_UNUSED_LINK_c         ((uint8_t)0xFF)

/** @brief Return valus indicating success of the requested operation */
#define g_TIMER_SUCCESS_c       ((uint8_t)0x01)

/** @brief Return valus indicating failure of the requested operation */
#define g_TIMER_FAILURE_c       ((uint8_t)0xFF)

/** @brief An invalid timeout was specified as input to a function */
#define g_INVALID_TIMEOUT_c     ((uint8_t)0xFE)

/** @brief An timer ID was specified as input to a function */
#define g_INVALID_TIMER_ID_c    ((uint8_t)0xFD)

/** @brief Indicates that the timer is currently in use */
#define g_TIMER_IN_USE_c        ((uint8_t)0xFC)

/** @brief Indicates that the timer is currently not running */
#define g_TIMER_NOT_RUNNING_c   ((uint8_t)0xFB)

/** @brief Value used for comparing timer values */
#define g_TIMER_COMPARE_VALUE_c ((uint32_t)0x7FFFFFFF)

/** @brief Indicates that the timer is currently not running */
#define g_TIMER_RELOAD_VALUE_c  ((uint32_t)0x0000FFFF)

/** @brief The minimum valid timeout value */
#define g_TIMER_MIN_TIMEOUT_c   ((uint32_t)100)

/** @brief The minimum valid timeout value */
#define g_TIMER_MAX_TIMEOUT_c   ((uint32_t)0x7FFFFFFF)

#define g_MAX_FREE_RUNNING_TIMER_VALUE_c	((uint32_t)0xFFFFFFFF)

/** @brief The minimum delay that can be provided by the delay function */
#define g_TIMER_MIN_DELAY_c     ((uint32_t)10)

/** @brief Returns the higher 16 bits of the time given as argument */
#define Get_High_Time(Time)     ((uint16_t)((Time) >> (16)))

/** @brief Returns the lower 16 bits of the time given as argument */
#define Get_Low_Time(Time)      ((uint16_t)((Time) & (0x0000FFFF)))


/** @brief Subtracts two time values taking care of roll over */
#define Subtract_Time(a, b)     ((a) > (b) ? \
                           ((a) - (b)) : ((g_TIMER_RELOAD_VALUE_c - (b)) + (a)))

/** @brief  Convert value in symbols to microseconds */
#define Convert_Symbols_To_Micro_Sec( value )   \
            ( (uint32_t) ( (uint32_t)(value) * g_ONE_SYMBOL_DURATION_IN_US_c ) )

/** @brief  Converts value in microseconds to symbols */
#define Convert_Micro_Sec_To_Symbols( value )   \
            ( (uint32_t) ( (uint32_t)(value) / g_ONE_SYMBOL_DURATION_IN_US_c ) )

/** @brief  Converts value in seconds to microseconds */
#define Convert_Seconds_To_Micro_Sec( value )   \
            ( (uint32_t) ( (uint32_t)(value) * 1000000 ) )

/** @brief  Converts value in microseconds to seconds */
#define Convert_Micro_Sec_To_Seconds( value )   \
            ( (uint16_t) ( (uint32_t)(value) / 1000000 ) )

#define gc_Total_Number_Of_Timers			(g_ZIGBEE_APP_TIMER_ID + 20) //RP: 10 for Application Timers

#define gc_Number_Of_Fast_Timers			4

#define m_MILLISECONDS_c                        0x3E8

#define m_QUARTER_SECOND_c                      0xFA

#define m_ONE_SECOND_c                          0x3E8

#define ENABLE_INTERRUPTS(x)	LEAVE_CRITICAL_REGION(x)

#define DISABLE_INTERRUPTS(x) 	ENTER_CRITICAL_REGION(x)

/*----------------------------------------------------------------------------*/
/**
 * @brief Checks whether a given timer is in use or inactive.
 *
 * This macro checks the value of the timer link array element corresponding
 * to the given timer id. If the value is @ref g_UNUSED_LINK_c, the timer is not
 * in use, if it is any other value, the timer is in use.
 *
 * @param timer_id The identifier of the timer whose status is to be determined.
 *
 * @return
 * - @ref g_TRUE_c, if the timer is in use
 * - @ref g_FALSE_c, if the timer is inactive.
 */
#define Is_Timer_Running(timer_id) \
                ( ( ga_Timer_Link_Array[ timer_id ] == ( g_UNUSED_LINK_c ) ) ? \
                    g_FALSE_c : g_TRUE_c )

/**************************** Public Type Definitions *************************/

/** @brief Callback function type definition */
typedef void (*Timer_Callback_t)(uint8_t);


/** @brief Structure which stores info about the timer callbacks and timeouts */
typedef struct Timer_Record_Tag
{

  /*! The absolute time at which the timer should expire */
  uint32_t target_time;

  /*! The callback function to be executed when the timer expires */
  Timer_Callback_t timer_callback;

}__attribute__((__packed__))Timer_Record_t;


/**************************** Public Enum Definitions *************************/

enum{
	g_PERIODIC = 0,
	g_SINGLE_SHOT
};

/**************************** Public Variable Declaration *********************/

/* None */

/**************************** Public Constant Declaration *********************/

/* None */

/**************************** Public Function Prototypes **********************/
/**
 * @brief Initializes the Timer Module
 *
 * This function should be called before any other functions of the timer
 * module are used. It initializes the active and expired timer queues.
 * It also initializes the timer hardware and starts the free-running counter.
 */
void TMR_Init ( void );

/*----------------------------------------------------------------------------*/
/**
 * @brief Task function to be called periodically within the main while loop.
 *
 * This function loads the hardware register with the required timeout value
 * when possible. It also checks for expired timers and invokes the appropriate
 * callback functions.
 */
void Timer_Task ( void );

/*----------------------------------------------------------------------------*/
/**
 * @brief Starts a new timer with given relative timeout
 *
 * This function checks the validity of the arguments, adds the current time to
 * the relative timeout given to obtain an absolute timeout and calls the add
 * timer function to add a new timer to the queue.
 *
 * @param timer_id The identifier of the software timer to start
 * @param timeout The relative timeout in microseconds
 * @param timer_callback Pointer to callback function to be invoked on timer expiry.
 *
 * @return
 * - @ref g_INVALID_TIMER_ID_c if the timer id is not within range,
 * - @ref g_INVALID_TIMEOUT_c if the timeout is not within range,
 * - @ref g_TIMER_IN_USE_c if the timer is currently active, or
 * - @ref g_TIMER_SUCCESS_c if timer is started successfully
 */
uint8_t tmr_startRelativeTimer (	uint8_t timer_id,
									uint32_t timeout,
                          	  	  	Timer_Callback_t timer_callback );

/*----------------------------------------------------------------------------*/
/**
 * @brief Starts a new timer with the given absolute timeout
 *
 * This function checks the validity of the parameters and calls the Add_Timer
 * function to add a new timer to the queue.
 *
 * @param timer_id The identifier of the software timer to start
 * @param target_time The absolute time(us) at which the timer should trigger
 * @param timer_callback Pointer to callback function to be invoked on timer expiry
 *
 * @return
 * - @ref g_INVALID_TIMER_ID_c if the timer id is not within range,
 * - @ref g_INVALID_TIMEOUT_c if the timeout is not within range,
 * - @ref g_TIMER_IN_USE_c if the timer is currently active, or
 * - @ref g_TIMER_SUCCESS_c if timer is started successfully
 */
uint8_t tmr_startAbsoluteTimer( uint8_t timer_id,
                         uint32_t target_time,
                         Timer_Callback_t timer_callback );

/*----------------------------------------------------------------------------*/
/**
 * @brief Starts a new fast timer with the given relative timeout
 *
 * This function adds the lower half word of current time to the given timeout
 * obtain an absolute timeout value. It then loads this value is directly into
 * a hardware compare register.
 * When compare match occurs, it triggers an interrupt and the callback is
 * invoked directly from the interrupt handler, instead of waiting for the next
 * execution of the timer task. Hence, this timer is more accurate than a normal
 * timer.
 *
 * @param timer_id The identifier of the software timer to start
 * @param timeout The relative timeout(us) after which the timer should trigger
 * @param timer_callback Pointer to callback function to be invoked on expiry
 *
 * @return
 * - @ref g_INVALID_TIMER_ID_c if the timer id is not a valid fast timer ID
 * - @ref g_TIMER_IN_USE_c if the timer is already in use
 * - @ref g_TIMER_SUCCESS_c if timer was started successfully
 */
uint8_t Start_Fast_Timer ( uint8_t timer_id,
                      uint16_t timeout,
                      Timer_Callback_t timer_callback);

/*----------------------------------------------------------------------------*/
/**
 * @brief Returns the current 32 bit time
 *
 * This function reads the higher 16 bits of the system time stored in the
 * global variable g_System_Time_High, and the lower 16 bits from the counter
 * value register of the hardware timer. It then combines the two to form a
 * single 32 bit value and returns it. Any overflow is also taken into account.
 * NOTE: Calling this function with interrupts disabled can result in wrong
 * values because overflow will not be taken into account.
 *
 * @return 32-bit system time in microseconds.
 */
uint32_t Get_Current_Time ( void );


/*----------------------------------------------------------------------------*/
/**
 * @brief Returns the timestamp of the currently received packet
 *
 * This function reads the higher 16 bits of the system time stored in the
 * global variable g_System_Time_High, and the lower 16 bits from the capture
 * register of the hardware timer which has been configured for timestamping.
 * It then combines the two to form a single 32 bit value and returns it.
 *
 * @return 32-bit timestamp in microseconds.
 */
uint32_t tmr_readTimeStamp ( void );


/*----------------------------------------------------------------------------*/
/**
 * @brief Stops a running timer
 *
 * This function stops a timer if it is running or if it has expired but its
 * callback function is yet to be invoked.
 *
 * @param timer_id The identifier of the software timer to be stopped.
 *
 * @return
 * - @ref g_INVALID_TIMER_ID_c if the timer id is not within valid range,
 * - @ref g_TIMER_NOT_RUNNING_c if the timer is not running so can't be stopped,
 * - @ref g_TIMER_SUCCESS_c if the timer was stopped successfully.
 */
uint8_t tmr_stopTimer ( uint8_t timer_id );

/*----------------------------------------------------------------------------*/
/**
 * @brief Stops a fast response timer
 *
 * This function stops a fast response timer if it is running.
 *
 * @param timer_id The identifier of the fast timer to be stopped.
 *
 * @return
 * - @ref g_INVALID_TIMER_ID_c if the timer id is not within valid range,
 * - @ref g_TIMER_NOT_RUNNING_c if the timer is not running so can't be stopped,
 * - @ref g_TIMER_SUCCESS_c if the fast timer was stopped successfully.
 */
uint8_t Stop_Fast_Timer ( uint8_t timer_id );

/*----------------------------------------------------------------------------*/
/**
 * @brief Compares two Time values taking into consideration, possible overflow.
 *
 * This function compares two time values and returns true if the first is less
 * than the second.
 * If the most significant bit (bit 31) of the two are equal, a normal
 * comparison is done and the result returned. If the MSBs are not equal, it is
 * assumed that an overflow has occured and hence the time with the smaller
 * value, when considering all bits except the MSB, is considered to be greater.
 *
 * @param t1 The first time parameter
 * @param t2 The second time parameter
 *
 * @return
 * - @ref g_TRUE_c, if t1 < t2
 * - @ref g_FALSE_c, if t1 > t2
 */
uint32_t Is_Time_Less ( uint32_t t1,
                   uint32_t t2 );

/*----------------------------------------------------------------------------*/
/**
 * @brief Generates a delay of a given number of microseconds.
 *
 * This function disables interrupts and waits in a busy loop for the given no.
 * of microseconds, which should be an integer between 10 and 65535.
 *
 * @param timeout The timeout value in microseconds.
 *
 */
void Generate_Delay ( uint16_t timeout );
/*----------------------------------------------------------------------------*/
/**
 * @brief Returns hardware timer value.
 *
 * This function Returns hardware timer value which is required for
 * random number generation.
 *
 * @return System low time.
 *
 */
uint16_t Get_Timer_Low_Time( void );
/*----------------------------------------------------------------------------*/
/**
 * @brief Returns a 16 bit random value
 *
 * This function Returns hardware timer value which is required for
 * random number generation.
 *
 * @return System low time.
 *
 */
uint16_t randomT ( void );

bool Is_tmr_running(uint8_t timer_id);

uint16_t system_low(void);


/*----------------------------------------------------------------------------*/
/**
 * @brief Returns a 32 bit timer tick value
 *
 * This function Returns 32 milliseconds tick
 * .
 *
 * @return timer tick.
 *
 */
uint32_t halCommonGetInt32uMillisecondTick(void);

/**************************** (C) COPYRIGHT 2010 XXXXX *****************EOF****/

#endif  /* _TIMER_H */
