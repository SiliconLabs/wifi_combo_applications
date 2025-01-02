/** @file timer.c
*
* @brief Implements the timer module functions
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


/**************************** Includes ****************************************/
#include <stdbool.h>
#include "ZigBeeDataTypes.h"
//#include "cpu.h"
#include "common_includes.h"
//#include "stack_events.h"
//#include "mac_stack_configuration.h"
#include "sw_timer.h"
#include "timer_id.h"
//#include "zb_hil_intf.h"
//#include "scheduler.h"
//#include "stack_configuration.h"
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

uint64_t GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

Timer_Record_t ga_Normal_Timers[gc_Total_Number_Of_Timers]; //RP: removed "+ 2"
Timer_Record_t ga_Fast_Timers[gc_Number_Of_Fast_Timers]; //RP: removed "+ 2"
uint8_t ga_Timer_Link_Array[gc_Total_Number_Of_Timers];

uint32 rsi_802154_hal_read_current_timer_value(void);
void rsi_802154_hal_hw_timer(uint32 timeout, uint8 single_shot);
void rsi_802154_hal_stop_timer(void);
void stop_timer(void);
int start_timer(int uSec, void (*timer_func_handler)(void));
int read_time();
void rsi_timer_start(uint8_t timer_node, uint8_t mode,uint8_t type, uint32_t duration,void (* rsi_timer_expiry_handler)());
void rsi_timer_stop(uint8_t timer_node);
uint32_t rsi_timer_read(uint8_t timer_node);
/**************************** Private Macro Definitions ***********************/

/* None */

/**************************** Private type Definitions ************************/

/* None */

typedef struct HWTimerInfo {
	bool IsTimerRunning;
	uint32_t timeout;
}__attribute__((__packed__))HWTimerInfo_t;


/**************************** Private Enum Definitions ************************/

/* None */

/**************************** Private Variable Definitions ********************/

/** @brief  Head of the Expired Timer Queue */
static uint8_t m_Expired_Timer_Head;

/** @brief  Tail of the Expired Timer Queue */
static uint8_t m_Expired_Timer_Tail;

/**************************** Public Variable Definitions *********************/

/** @brief  Head of the Active Timer Queue */
uint8_t g_Active_Timer_Head;

/** @brief  Holds the higher 16 bits of current system time */
volatile uint16_t g_System_Time_High;

/** @brief  Indicates whether the currently active timer has expired */
volatile uint32_t g_Current_Timer_Expired;

volatile uint32_t CurrentSystemTime = 0;

uint32_t CurrentTimeOutValue = 0;

volatile bool FreeRunningTimerActive = true;

static HWTimerInfo_t HwTmrInfo;

/**************************** Private Constant Definitions ********************/

/* None */

/**************************** Public Constant Definitions *********************/

/* None */

/**************************** Private Function Prototypes *********************/

/** @brief Invokes the callbacks of Expired Timers
 *
 * This function parses the expired timer queue, frees up the link, and invokes
 * the callback function for each timer.
 *
 * @return None
 */
static void Invoke_Expired_Timer_Callbacks ( void );

/*----------------------------------------------------------------------------*/
/** @brief Handles timer trigger
 *
 * This function checks whether the currently loaded timer has expired and
 * if so, it adds the timer to the expired timer queue.
 *
 * @return None
 */
static void Handle_Timer_Trigger ( void );

void Init_Timer_Hardware(void);
uint32_t Load_Hardware_Register ( uint32_t target_time );

/*----------------------------------------------------------------------------*/
/**
 * @brief Adds a new timer to the active timer list.
 *
 * This function parses the active timer queue, finds the correct position and
 * inserts the given timer into the queue in such a way that the timers are
 * maintained in ascending order of their target time of expiry.
 *
 * @param timer_id The identifier of the software timer to start
 * @param target_time The absolute time at which the timer should trigger
 * @param timer_callback Pointer to callback function to be invoked on timer
 * expiry
 *
 * @return
 * - @ref g_INVALID_TIMER_ID_c if the timer id is not within range,
 * - @ref g_TIMER_IN_USE_c if the timer is currently active, or
 * - @ref g_TIMER_SUCCESS_c if the timer was added successfully
 */
static uint8_t Add_Timer ( uint8_t timer_id,
                      uint32_t target_time,
                      Timer_Callback_t timer_callback );

/**************************** Public Functions Definitions ********************/

void TMR_Init ( void )
{
   uint32_t index;

   /* Initialize all the links to 'unused' and all timers to zero */
   for (index = 0; index < gc_Total_Number_Of_Timers; index++)
   {
      ga_Normal_Timers[ index ].target_time = 0;
      ga_Normal_Timers[ index ].timer_callback = g_NULL_c;
      ga_Timer_Link_Array[ index ] = g_UNUSED_LINK_c;
   }

   /* Initialize all fast timers to zero */
   for (index = 0; index < gc_Number_Of_Fast_Timers; index++)
   {
      ga_Fast_Timers[ index ].target_time = 0;
      ga_Fast_Timers[ index ].timer_callback = g_NULL_c;
   }

   /* Initialize all globals */
   g_Active_Timer_Head = m_Expired_Timer_Head
                       = m_Expired_Timer_Tail = g_NO_TIMER_c;
   g_System_Time_High = 0;
   g_Current_Timer_Expired = g_FALSE_c;

   HwTmrInfo.IsTimerRunning = false;
   HwTmrInfo.timeout = 0;

   /* Perform the hardware level initializations */
   Init_Timer_Hardware();
}

/*----------------------------------------------------------------------------*/

void hw_timer_callback(void) __attribute__((section(".zb_tcm_code")));
void hw_timer_callback (void)
{
	g_Current_Timer_Expired = g_TRUE_c;
	rsi_802154_hal_stop_timer();
//	Stack_Events_SetEvent(g_TIMER_TASK_PENDING_c);
  Timer_Task();
//	SET_EVENT_TH3(TESTAPP_BUF_ALLOC);
}

/*----------------------------------------------------------------------------*/

void Init_Timer_Hardware(void)
{
	rsi_802154_hal_stop_timer();
}

/*----------------------------------------------------------------------------*/

uint8_t tmr_startRelativeTimer (	uint8_t timer_id,
                          	  	uint32_t timeout,
                          	  	Timer_Callback_t timer_callback )
{
   /*
   * Add current time to the given relative timeout to get the absolute
   * timeout and then start absolute timer with this timeout value
   */
   /* Check if the timeout value is within limits */
   if ( ( timeout < g_TIMER_MIN_TIMEOUT_c ) ||
        ( timeout > g_TIMER_MAX_TIMEOUT_c ) )
   {
      return g_INVALID_TIMEOUT_c;
   }

   return Add_Timer ( timer_id,
                      timeout,
                      timer_callback );

}

/*----------------------------------------------------------------------------*/

uint8_t tmr_startAbsoluteTimer ( uint8_t timer_id,
                          uint32_t target_time,
                          Timer_Callback_t timer_callback )
{
   uint32_t cur_time = Get_Current_Time();
   uint32_t rel_timeout = Subtract_Time(target_time,cur_time);

   /* Check if the timeout value is within limits */
   if ( ( rel_timeout < g_TIMER_MIN_TIMEOUT_c ) ||
        ( rel_timeout > g_TIMER_MAX_TIMEOUT_c ) )
   {
      return g_INVALID_TIMEOUT_c;
   }

   return Add_Timer ( timer_id,
                      target_time,
                      timer_callback );
}

/*----------------------------------------------------------------------------*/

void Timer_Task ( void )
{
   /* Check the flag and add any expired timers to the expired timer queue */
   Handle_Timer_Trigger();

   /* Go through the expired timer queue and invoke each callback */
   Invoke_Expired_Timer_Callbacks();

   //Stack_Events_ClearEvent(g_TIMER_TASK_PENDING_c);
}

/*----------------------------------------------------------------------------*/
#if 0
void Generate_Delay ( uint16_t timeout )
{
   uint32_t ipl_reg;
   DISABLE_INTERRUPTS(ipl_reg);

   if ( timeout < g_TIMER_MIN_DELAY_c )
   {
      /* If delay is less than minimum delay, return immediately */
      return;
   }

   timeout += SYSTEM_TIME_LOW;

   while ( timeout < SYSTEM_TIME_LOW )
   {
    /* Wait till SYSTEM_TIME_LOW resets to zero if it is greater than timeout */
   }

   while ( timeout > SYSTEM_TIME_LOW )
   {
    /* Wait until SYSTEM_TIME_LOW once again becomes greater than timeout */
   }

   ENABLE_INTERRUPTS(ipl_reg);
}
#endif
/*----------------------------------------------------------------------------*/
uint8_t tmr_stopTimer ( uint8_t timer_id )
{
   uint32_t link_index;
   uint32_t time_elapsed=0;

   /* Initial check for sanity of the argument */
   if ( timer_id >= gc_Total_Number_Of_Timers )
   {
      return g_INVALID_TIMER_ID_c;
   }

   /* Check whether the timer is already stopped */
   if ( ga_Timer_Link_Array[ timer_id ] == g_UNUSED_LINK_c )
   {
      return g_TIMER_NOT_RUNNING_c;
   }

   //DISABLE_INTERRUPTS(ipl_reg);
   /*
   * First search for the timer index which links to the timer_id to be stoppped
   * This will be just before our timer in the active or expired timer queue
   */

   for ( link_index = 0; link_index < gc_Total_Number_Of_Timers; link_index++ )
   {
      if ( ga_Timer_Link_Array[ link_index ] == timer_id )
      {

         /*
          * Once the link has been found, break the link and connect it to the
          * next timer in the queue, which could be none
          */
         ga_Timer_Link_Array[ link_index ] = ga_Timer_Link_Array[ timer_id ];
         break;
      }
   }

   time_elapsed = ga_Normal_Timers[ g_Active_Timer_Head ].target_time - rsi_802154_hal_read_current_timer_value();

   /* Look for references to our timer in the queue head and tail pointers */

   if ( g_Active_Timer_Head == timer_id )
   {
      /*
      * Our timer was at the head of active timer queue, so make it point to the
      * next timer and reload hardware register
      */
      g_Active_Timer_Head = ga_Timer_Link_Array[ timer_id ];
	  uint8_t link = g_Active_Timer_Head;

	  if ( g_Active_Timer_Head != g_NO_TIMER_c )
	  {
		  while ( link != g_NO_TIMER_c )
		  {
			 //update time
			  ga_Normal_Timers[ link ].target_time -= time_elapsed;
			  link = ga_Timer_Link_Array[link];
		  }

		  //DISABLE_INTERRUPTS(ipl_reg);
		  rsi_802154_hal_stop_timer();
		  //rsi_802154_hal_hw_timer (ga_Normal_Timers[ g_Active_Timer_Head ].target_time, g_SINGLE_SHOT);
		  rsi_802154_hal_hw_timer (ga_Normal_Timers[ g_Active_Timer_Head ].target_time, g_SINGLE_SHOT);
		  //ENABLE_INTERRUPTS(ipl_reg);
	  }
	  else /* No more timers pending */
	  {
		  //DISABLE_INTERRUPTS(ipl_reg);
		  rsi_802154_hal_stop_timer();
		  //ENABLE_INTERRUPTS(ipl_reg);
	  }
   }

   if ( m_Expired_Timer_Head == timer_id )
   {
      /*
      * Our timer was at the head of expired timer queue, so just make it point
      * to the next timer (which could be g_NO_TIMER_c)
      */
      m_Expired_Timer_Head = ga_Timer_Link_Array[ timer_id ];
   }

   if ( m_Expired_Timer_Tail == timer_id )
   {
      /*
      * Our timer was at the tail of the expired timer queue, so if the queue
      * contained only that timer, make tail point to NO_TIMER, else make
      * it point to the previous timer in the expired list.
      */
      m_Expired_Timer_Tail = ( link_index < gc_Total_Number_Of_Timers ) ?
                               link_index : g_NO_TIMER_c;
   }

   /* Clear the timer parameters from the structure */
   ga_Normal_Timers[ timer_id ].timer_callback = g_NULL_c;

   /* Mark the timer as unused in Timer Link Array and return SUCCESS */
   ga_Timer_Link_Array[ timer_id ] = g_UNUSED_LINK_c;
   
   //ENABLE_INTERRUPTS(ipl_reg);

   return g_TIMER_SUCCESS_c;
}

uint32_t Load_Hardware_Register ( uint32_t target_time )
{
   //uint32_t ipl_reg;
   if ( g_NO_TIMER_c == g_Active_Timer_Head )
   {
	  return 0;
   }

   /* Load hardware register because head has changed */
   //DISABLE_INTERRUPTS(ipl_reg);
   rsi_802154_hal_stop_timer();
   rsi_802154_hal_hw_timer (target_time, g_SINGLE_SHOT);
   //ENABLE_INTERRUPTS(ipl_reg);
   return 1;
}

uint16_t SysHigh = 0, SysLow = 0;

uint16_t system_low()
{
	//SysLow = rsi_802154_hal_read_current_timer_2_value() & 0x0000FFFF;
	SysLow = rsi_802154_hal_read_current_timer_value() & 0x0000FFFF;
	return SysLow;
}

uint16_t system_hi()
{
	SysHigh = ((FreeRunningTimerActive)?((g_MAX_FREE_RUNNING_TIMER_VALUE_c - rsi_802154_hal_read_current_timer_value()) >> 16):((g_MAX_FREE_RUNNING_TIMER_VALUE_c - CurrentSystemTime) >> 16));
	return SysHigh;
}

//void tmr_generateDelay(uint32_t timeOut)
//{
//
//}

/*----------------------------------------------------------------------------*/
uint8_t Stop_Fast_Timer(uint8_t timer_id)
{
   /* Initial check for sanity of the argument */
   if ( timer_id >= gc_Number_Of_Fast_Timers )
   {
      return g_INVALID_TIMER_ID_c;
   }

   if ( ga_Fast_Timers[ timer_id ].timer_callback )
   {
//      DISABLE_FAST_TIMER_INTERRUPT( timer_id );
      ga_Fast_Timers[ timer_id ].timer_callback = g_NULL_c;
   }

   return g_TIMER_SUCCESS_c;
}

/*----------------------------------------------------------------------------*/
uint32_t current_time;
uint32_t Get_Current_Time ( void )
{
	//current_time = rsi_802154_hal_read_current_timer_2_value();
	////current_time = rsi_802154_hal_read_current_timer_value();
	////return (0xFFFFFFFF - current_time);
	//return current_time;
	return GetTimeStamp();	
}

/*----------------------------------------------------------------------------*/

uint32_t tmr_readTimeStamp ( void )
{
   /*
   * The higher 16 bits are obtained from the global variable and the lower 16
   * bits read from the hardware register. The combined value is returned.
   */
   uint32_t time_stamp = ( uint32_t )g_System_Time_High;
   time_stamp <<= 16;
//   time_stamp |= Read_Timestamp_Register();
   return time_stamp;
}

/*----------------------------------------------------------------------------*/

uint32_t Is_Time_Less ( uint32_t t1,
                   uint32_t t2 )
{
//  uint32_t flag = g_FALSE_c;
//
//  if ( ( t1 & ~g_TIMER_COMPARE_VALUE_c ) == ( t2 & ~g_TIMER_COMPARE_VALUE_c ) )
//  {
//    if ( t1 < t2 )
//    {
//        flag = g_TRUE_c;
//    }
//  }
//  else
//  {
//    if ( ( t1 & g_TIMER_COMPARE_VALUE_c ) > ( t2 & g_TIMER_COMPARE_VALUE_c ) )
//    {
//        flag = g_TRUE_c;
//    }
//  }
//  return flag;
	return (t1 <= t2);
}


/*----------------------------------------------------------------------------*/

uint8_t Start_Fast_Timer ( uint8_t timer_id,
                      uint16_t timeout,
                      Timer_Callback_t timer_callback )
{
   timeout += SYSTEM_TIME_LOW;

   /* Validity check on arguments */

   if ( timer_id >= gc_Number_Of_Fast_Timers )
   {
      return g_INVALID_TIMER_ID_c;
   }

   if ( g_NULL_c != ga_Fast_Timers[ timer_id ].timer_callback )
   {
      return g_TIMER_IN_USE_c;
   }

   /* Assign values to Timer structure */
   ga_Fast_Timers[ timer_id ].target_time = timeout;
   ga_Fast_Timers[ timer_id ].timer_callback = timer_callback;

   switch ( timer_id )
   {
      case g_FAST_TIMERID0_c:
//         Load_Fast_Timer0(timeout);
         break;

      case g_FAST_TIMERID1_c:
//         Load_Fast_Timer1(timeout);
         break;

      default:
         return g_INVALID_TIMER_ID_c;
   }
   return g_TIMER_SUCCESS_c;
}
/*----------------------------------------------------------------------------*/
uint16_t Get_Timer_Low_Time ( void )
{
   return SYSTEM_TIME_LOW;
}

/**************************** Private Function Definitions ********************/
static void Handle_Timer_Trigger ( void )
{
   //uint32_t ipl_reg;
   /* Set the flag indicating the hardware register has to be loaded,to FALSE */
   uint32_t load_hw_register = g_FALSE_c;

   /*
    * Get the time before entering critical section because the function cannot
    * be called with interrupts disabled
    */
//   uint32_t cur_time = Get_Current_Time();

   /*
    * Disable Interrupts - we do not want any interrupts to occur while within
    * this function.
    */
//   DISABLE_INTERRUPTS(ipl_reg);

   /* Check if the current timer has expired and whether any timer is active */
   while ( ( g_TRUE_c == g_Current_Timer_Expired ) &&
           ( g_Active_Timer_Head != g_NO_TIMER_c ))
   {
      g_Current_Timer_Expired = g_FALSE_c;
      /*
      * If expired timer queue is not empty, make the last timer in the queue
      * point to the newly expired timer.
      */
      if(m_Expired_Timer_Tail != g_NO_TIMER_c)
      {
         ga_Timer_Link_Array[ m_Expired_Timer_Tail ] = g_Active_Timer_Head;
      }

      /* Insert the newly expired timer at the tail */
      m_Expired_Timer_Tail = g_Active_Timer_Head;

      /* Make the head of active timer queue point to next timer in queue */
      g_Active_Timer_Head = ga_Timer_Link_Array[ g_Active_Timer_Head ];

      if(g_Active_Timer_Head != g_NO_TIMER_c)
      {
          load_hw_register = g_TRUE_c;
      }

      /* Mark the newly added timer as the end of list */
      ga_Timer_Link_Array[ m_Expired_Timer_Tail ] = g_NO_TIMER_c;

      /*
      * NOTE: The following 'if' block handles many timers expiring at the same
      * time. It has the effect of increasing the error of the first timeout in
      * case there are many timers expiring at around the same time. However,the
      * difference between the timings of individual callbacks will be smaller.
      * If this code is avoided, the first timer to expire will be more accurate
      * but the offset between individual timers will be greater. The minimum
      * error is smaller but the maximum error is higher in this case. Also, the
      * speed of execution of Timer_Task() improves considerably.
      */
      if ( g_Active_Timer_Head != g_NO_TIMER_c )
      {

         /* Check if the next timer in the active list has already expired */
         if ( Is_Time_Less( ga_Normal_Timers[ g_Active_Timer_Head ].target_time,
        		 	 	 	ga_Normal_Timers[ m_Expired_Timer_Tail ].target_time ))
         {
            /*
            * If so, set the expired flag again, so it will be processed in the
            * next iteration of this while loop
            */
            g_Current_Timer_Expired = g_TRUE_c;
            load_hw_register = g_FALSE_c;
         }
      }

      if ( g_NO_TIMER_c == m_Expired_Timer_Head )
      {
         /* If the expired timer queue was empty, make the head equal to tail */
         m_Expired_Timer_Head = m_Expired_Timer_Tail;
      }

   }

   if ( load_hw_register == g_TRUE_c )
   {
	   uint8_t link = g_Active_Timer_Head;

	   while ( link != g_NO_TIMER_c )
	   {
		     //update time
			   ga_Normal_Timers[ link ].target_time -= ga_Normal_Timers[ m_Expired_Timer_Tail ].target_time;

			   link = ga_Timer_Link_Array[link];
	   }

	   //DISABLE_INTERRUPTS(ipl_reg);
	   rsi_802154_hal_stop_timer();
	   rsi_802154_hal_hw_timer (ga_Normal_Timers[ g_Active_Timer_Head ].target_time, g_SINGLE_SHOT);
	   //ENABLE_INTERRUPTS(ipl_reg);
   }

   /* Re-enable Interrupts */
   //ENABLE_INTERRUPTS(ipl_reg);

}

/*----------------------------------------------------------------------------*/

static void Invoke_Expired_Timer_Callbacks(void)
{
   //uint32_t ipl_reg;
   Timer_Callback_t p_callback_fn;
   uint8_t link = g_NO_TIMER_c;

   /* Loop until all the timers upto the tail, has been serviced */
   while ( link != m_Expired_Timer_Tail )
   {
	   if ( m_Expired_Timer_Head == g_NO_TIMER_c )
		{
		   m_Expired_Timer_Head = m_Expired_Timer_Tail;
		}

      //DISABLE_INTERRUPTS(ipl_reg);

      link = m_Expired_Timer_Head;

      /* Save callback fn pointer*/
      p_callback_fn = ga_Normal_Timers[ m_Expired_Timer_Head ].timer_callback;

      /* Clear structure values */
      ga_Normal_Timers[ m_Expired_Timer_Head ].timer_callback = g_NULL_c;

      /* Update the expired timer queue head */
      m_Expired_Timer_Head = ga_Timer_Link_Array[ m_Expired_Timer_Head ];

      /* Mark the link as unused */
      ga_Timer_Link_Array[ link ] = g_UNUSED_LINK_c;

      /* After doing all the cleanup, reenable interrupts and invoke callback */
     // ENABLE_INTERRUPTS(ipl_reg);

      if ( g_NULL_c != p_callback_fn )
      {
         p_callback_fn ( link );
      }
   }
   /*
   * Once out of the while loop, the expired timer queue will be empty hence
   * assign null values to both the head and the tail and return
   */
   m_Expired_Timer_Head = m_Expired_Timer_Tail = g_NO_TIMER_c;
   return;
}

/*----------------------------------------------------------------------------*/
#ifdef ZB_TMR_DBG
typedef struct zb_tmr_dbg_s{
uint32_t DBGVal1 ;
uint32_t DBGVal2 ;
uint32_t DBGVal3 ;
uint32_t DBGVal4 ;
uint32_t DBGVal5 ;
}zb_tmr_dbg_t;
zb_tmr_dbg_t zb_tmr_dbg;
#endif

uint8_t Add_Timer (uint8_t  timer_id,
              uint32_t target_time,
              Timer_Callback_t timer_callback)
{
   //uint32_t ipl_reg;
   uint8_t link = g_Active_Timer_Head;
   uint8_t prev_link = link;

   uint32_t current_target_time = 0;
   uint32_t pending_time_out_val = 0;
   uint32_t time_elapsed = 0;
#ifdef ZB_TMR_DBG
   zb_tmr_dbg.DBGVal1 = 0;
   zb_tmr_dbg.DBGVal2 = 0;
   zb_tmr_dbg.DBGVal3 = 0;
   zb_tmr_dbg.DBGVal4 = 0;
   zb_tmr_dbg.DBGVal5 = 0;
#endif
   /* Check if the timer ID is valid */
   if ( timer_id >= gc_Total_Number_Of_Timers )
   {
      return g_INVALID_TIMER_ID_c;
   }

   /* Check if the specified timer is already in use */
   if ( ga_Timer_Link_Array[ timer_id ] != g_UNUSED_LINK_c )
   {
      return g_TIMER_IN_USE_c;
   }
 
   //DISABLE_INTERRUPTS(ipl_reg);
   pending_time_out_val = rsi_802154_hal_read_current_timer_value();
   if ( g_Active_Timer_Head != g_NO_TIMER_c )
   {
	   current_target_time = ga_Normal_Timers [ g_Active_Timer_Head ].target_time;
	   time_elapsed = current_target_time - pending_time_out_val;
#ifdef ZB_TMR_DBG
	   zb_tmr_dbg.DBGVal4 = current_target_time;
	   zb_tmr_dbg.DBGVal5 = pending_time_out_val;
#endif
   }
   else
   {
	   time_elapsed = 0;
   }

   while ( link != g_NO_TIMER_c )
   {
	  if(ga_Normal_Timers[ link ].target_time < time_elapsed)
	  {
#ifdef ZB_TMR_DBG
		  zb_tmr_dbg.DBGVal1 = 1;
		  zb_tmr_dbg.DBGVal2 = ga_Normal_Timers[ link ].target_time;
		  zb_tmr_dbg.DBGVal3 = time_elapsed;
#endif
		  while(1);
	  }

	  //update time
	  ga_Normal_Timers[ link ].target_time -= time_elapsed;
	  link = ga_Timer_Link_Array[ link ];
   }

   /* Assign values to Timer structure */
   ga_Normal_Timers[ timer_id ].target_time = target_time;
   ga_Normal_Timers[ timer_id ].timer_callback = timer_callback;

   link = g_Active_Timer_Head;
   prev_link = link;

   while ( link != g_NO_TIMER_c )
   {
      /*
      * Loop until a timer is found with a greater timeout value or the
      * end of list is reached
      */
      if ( Is_Time_Less ( ga_Normal_Timers[ timer_id ].target_time,
                          ga_Normal_Timers[ link ].target_time ) )
      {
         break;
      }
      prev_link = link;
      link = ga_Timer_Link_Array[ link ];
   }

   if ( g_Active_Timer_Head == g_NO_TIMER_c )
   {
	   /* Insertion at head of list */
		g_Active_Timer_Head = timer_id;
		ga_Timer_Link_Array[ timer_id ] = link;

		/* Load hardware register because head has changed */
		rsi_802154_hal_stop_timer();
		rsi_802154_hal_hw_timer (target_time, g_SINGLE_SHOT);
   }

   else if ( link == g_Active_Timer_Head )
   {
	  Load_Hardware_Register(target_time);
	  /* Insertion at head of list */
	  g_Active_Timer_Head = timer_id;
	  ga_Timer_Link_Array[ timer_id ] = link;
   }
   else
   {
      /*
      * Insertion at some position inside the list, or at the end of the list
      * in which case (link == g_NO_TIMER_c)
      */
      ga_Timer_Link_Array[ timer_id ] = link;
      ga_Timer_Link_Array[ prev_link ] = timer_id;
   }
   //ENABLE_INTERRUPTS(ipl_reg);

   return g_TIMER_SUCCESS_c;
}
/************************************************************************/
bool Is_tmr_running(uint8_t timer_id)
{
    return ( (ga_Timer_Link_Array[ timer_id ] == ( g_UNUSED_LINK_c ) ) ? false : true );
}
/***************************************************************************/


void rsi_802154_hal_stop_timer(void)
{
  stop_timer();
  //rsi_timer_stop(1);
}

void rsi_802154_hal_hw_timer(uint32 timeout, uint8 single_shot)
{
  start_timer(timeout, &hw_timer_callback);
  //rsi_timer_start(1,0,0,timeout,&hw_timer_callback);
}

uint32_t rsi_802154_hal_read_current_timer_value(void)
{
  return read_time();
//  return rsi_timer_read(1);
}



/**************************** (C) COPYRIGHT 2010 XXXXX *****************EOF****/
