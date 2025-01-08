/** @file queue.c
 *
 * @brief  This module handles all the functionalities of the Queue Management.
 *         It implements Initialization, Enqueue and Dequeue of Queues.
 */


/**************************** Includes ****************************************/

#include "ZigBeeDataTypes.h"
#include "common_includes.h"
#include "queue.h"
#include "mac_stack_configuration.h"

/**************************** Private Macro Definitions ***********************/

/** @brief  Gives the value of the head and tail of the queue when no buffer
 is queued */
#define m_UNKNOWN_LINK_c    ( ( uint8_t ) ( 0xFF ) )

/** @brief  Gives the current capacity of the queue when no buffer is queued */
#define m_NO_ID_QUEUED_c   ( ( uint8_t ) ( 0x00 ) )

/**************************** Private type Definitions ************************/

/* None */

/**************************** Private Enum Definitions ************************/

/* None */

/**************************** Private Variable Definitions ********************/

/* None */

/**************************** Public Variable Definitions *********************/

/* None */

/**************************** Private Constant Definitions ********************/

/* None */

/**************************** Public Constant Definitions *********************/

/* None */

/**************************** Private Function Prototypes *********************/

/* None */

/**************************** Public Functions Definitions ********************/

void queueMgmt_Init ( queue_t* pQueueName, uint8_t queueSize )
{
    /* Initializing the head and tail of the queue to unkonwn values */
    pQueueName->head = pQueueName->tail = m_UNKNOWN_LINK_c;

    /* Initializing the total capacity of the queue to the queue size*/
    pQueueName->total_capacity = queueSize;

    /* Assigning the current capacity of the queue to zero */
    pQueueName->current_capacity = m_NO_ID_QUEUED_c;
}

/******************************************************************************/

uint8_t queueMgmt_enqueue ( queue_t* pQueueName, uint8_t bufferId )
{
    /* Checking if there is any buffer id in the queue */
    if (m_NO_ID_QUEUED_c == pQueueName->current_capacity) {
        /* The head and the tail point to the same id when enqueue is done
         for the first time */
        pQueueName->head = pQueueName->tail = bufferId;

        /* The next link is pointing to unknown when only one buffer id is
         enqueued into the queue */
        ga_Buffer_Link_Array[bufferId] = m_UNKNOWN_LINK_c;
    }

    /* Checking if the queue is full */
    else if (pQueueName->total_capacity == pQueueName->current_capacity) {
        return g_QUEUE_FULL_c;
    }

    /* If its not the first buffer and the queue is not full */
    else {
        /* The tail next will point to the new buffer id */
        ga_Buffer_Link_Array[pQueueName->tail] = bufferId;

        /* The tail points to the new buffer id */
        pQueueName->tail = bufferId;

        /* The new tail next will point to a unknown link */
        ga_Buffer_Link_Array[pQueueName->tail] = m_UNKNOWN_LINK_c;
    }

    /* Incrementing the queue capacity */
    pQueueName->current_capacity++;

    return g_SUCCESS_c;
}

/******************************************************************************/

uint8_t queueMgmt_dequeue ( queue_t* pQueueName )
{
    /* Assigning the current buffer id to a local variable to be returned if
     the queue is not empty */
    uint8_t bufferId = pQueueName->head;

    /* Checking if the current queue has not buffer id's */
    if (m_NO_ID_QUEUED_c != pQueueName->current_capacity) {
        /* Decrementing the current number of buffers held by the queue */
        pQueueName->current_capacity--;

        /* Checking if there are not buffer id's in the queue */
        if (m_NO_ID_QUEUED_c == pQueueName->current_capacity) {
            /* If the queue has no buffer id's head and tail will point to
             unknown link */
            pQueueName->head = pQueueName->tail = m_UNKNOWN_LINK_c;
        }
        else {
        	/* Making the head point to the next buffer id in the queue */
            pQueueName->head = ga_Buffer_Link_Array[pQueueName->head];
        }
        return bufferId;
    }

    /* If there is not buffer id in the queue return queue empty */
    return g_QUEUE_EMPTY_c;
}

/******************************************************************************/

uint8_t queueMgmt_dequeueSpecBuffer ( queue_t* pQueueName, uint8_t bufferId )
{
    uint8_t prev;
    uint8_t current;

    /* Variables holding the queue head */
    current = prev = pQueueName->head;

    /* Checking if there is any buffer id in the queue */
    if (m_NO_ID_QUEUED_c != pQueueName->current_capacity) {

        /* Loop through the queue starting from the head to tail till getting the
         specified buffer id */
        while ((current != bufferId) && (current != 0xFF)) {
            /* Making the variable to hold the current buffer id */
            prev = current;

            /* Making the variable to hold the next buffer id in the queue */
            current = ga_Buffer_Link_Array[current];
        }

        if (current == 0xFF) {
            /* Reached the queue tail without finding the buffer id, return
             failure */
            return g_FAILURE_c;
        }

        /* Decrementing the current number of buffers held by the queue */
        pQueueName->current_capacity--;

        /* If buffer id is present in the Queue as head element */
        if (current == pQueueName->head) {
            /* Making the head point to the next buffer id in the queue */
            pQueueName->head = ga_Buffer_Link_Array[pQueueName->head];

            /* Checking if there is any buffer id in the queue */
            if (!(pQueueName->current_capacity)) {
                /* If the queue has no buffer id's tail will point to unknown
                 link */
                pQueueName->tail = m_UNKNOWN_LINK_c;
            }
            return g_HEAD_c;
        }

        /* If buffer id is present in the Queue as tail element */
        else if (current == pQueueName->tail) {
            /* Buffer link array at prev index was pointing to current (buffer ID),
             make it to point to FF */
            ga_Buffer_Link_Array[prev] = m_UNKNOWN_LINK_c;
            /* Making the tail point to last buffer id in the queue */
            pQueueName->tail = prev;
            return g_SUCCESS_c;
        }

        /* The element is in between the head and tail in the queue */
        else {
            /* Making the buffer link array to hold the next link */
            ga_Buffer_Link_Array[prev] = ga_Buffer_Link_Array[current];
            return g_SUCCESS_c;
        }
    }

    return g_FAILURE_c;
}

uint8_t queueMgmt_isQueueFull (queue_t* pQueueName)
{
    if (pQueueName->current_capacity == pQueueName->total_capacity) {
        return g_TRUE_c;
    }
    return g_FALSE_c;
}
/**************************** Private Function Definitions ********************/

/* None */
