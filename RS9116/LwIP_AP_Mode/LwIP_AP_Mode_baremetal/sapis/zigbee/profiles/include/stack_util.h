/*******************************************************************************
 * stack_util.h
 ********************************************************************************
 * Program Description:
 *     This file contains the APIs of utility function -  Functions to
 *     initialize queue, add to queue and dequeue and memory management APIs.
 *
 *******************************************************************************/

/*******************************************************************************

 This file contains the APIs of utility function to
 Initialize queue
 Add to queue
 Dequeue
 Memory management (MemSet, MemCpy, MemRevCpy, MemCmp)
 *******************************************************************************/

#ifndef STACK_UTIL_H
#define STACK_UTIL_H

/*******************************************************************************
 * Includes
 *******************************************************************************/

#include "ZigBeeDataTypes.h"

/*******************************************************************************
 * Global Constants
 *******************************************************************************/

#define m_MILLISECONDS_c                  0x3E8

/***********************************************************************
 MOVED FROM STACK CONFIURATION .H
************************************************************************/


/*******************************************************************************
 * Type Definitions
 *******************************************************************************/

/*******************************************************************************
 These enumerations are used for Queue and Message Handling

 g_Buffer_Overflow_c  -  Buffer Overflow

 g_Buffer_Empty_c     -  Buffer Empty

 *******************************************************************************/
enum {
    g_Buffer_Overflow_c = 0xFF,
    g_Buffer_Empty_c    = 0xFF
};

/*******************************************************************************
 These enumerations are used while Checking the Queue whether it is empty or
 NOT

 g_Queue_Empty_c      -  Queue is Empty

 g_Queue_Not_Empty_c  -  Queue is Not Empty

 *******************************************************************************/
enum {
    g_Queue_Empty_c     = 0x00,
    g_Queue_Not_Empty_c = 0x01
};

/*  function pointer to handling function   */
typedef void (* fn_ptr)(uint8_t, uint8_t*);

typedef struct FSM_Table_Tag {
    uint8_t state;
    uint8_t pid;
    fn_ptr ptr_primitive;
} __attribute__((__packed__)) FSM_Table_t;

/* Queue is of type uint8_t */
typedef uint8_t Queue;


enum
{
   g_Memory_Matches_c,
   g_Memory_No_Match_c
};

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/

/* None */

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/
/*******************************************************************************
 * ZDO_setExtendedAddressMAC
 ********************************************************************************
 *
 * Return Value :
 *   None
 *
 * Parameters   :
 *
 * Input Parameters:
 *   None
 *
 * Output Parameters:
 *   None
 *
 *
 * Scope of the function:
 *       This function can be called from application
 *
 * Purpose of the function:
 *       This function allows the Application to set Extended Address
 *
 * Before the function is called:
 *
 *       NONE.
 *
 * After the function is called:
 *
 *       NONE.
 *******************************************************************************/
void ZDO_setExtendedAddressMAC(void);

/*******************************************************************************
 * End Of File
 *******************************************************************************/
#endif                                 /* STACK_UTIL_H */
