/** @file Queue.h
 *
 * @brief  This is a header file is for the Queue Management interface for
 *         Initialization, Enqueue and Dequeue of Queues.
 */


#ifndef __QUEUE_H
#define __QUEUE_H

#include "ZigBeeDataTypes.h"

/**************************** Public Macro Definitions ************************/

/** @brief Defines the value returned when queue is empty */
#define g_QUEUE_EMPTY_c    ( ( uint8_t ) ( 0xFF ) )

/** @brief Defines the value returned when queue is full */
#define g_QUEUE_FULL_c     ( ( uint8_t ) ( 0xFF ) )

/** @brief Defines the value returned when queue head is pointing to the buffer
 id to be dequeued */
#define g_HEAD_c    ( ( uint8_t ) ( 0x01 ) )

/** @brief Used to intialize a queue */
#define Init_Queue( queue , size ) queueMgmt_Init ( &queue , size )

/** @brief Used to intialize a queue, with queue pointer passed */
#define Init_QueuePtr( queue , size ) queueMgmt_Init ( queue , size )

/** @brief Used to enqueue a buffer id into the queue */
#define Enqueue( queue , buffer_id ) queueMgmt_enqueue ( &queue, buffer_id )

/** @brief Used to enqueue a buffer id into the queue, with queue pointer passed */
#define EnqueuePtr( queue , buffer_id ) queueMgmt_enqueue ( queue, buffer_id )

/** @brief Used to dequeue the current buffer id from the queue */
#define Dequeue( queue )    queueMgmt_dequeue ( &queue )

/** @brief Used to dequeue the current buffer id from the queue , with queue pointer passed*/
#define DequeuePtr( queue )    queueMgmt_dequeue ( queue )

/** @brief Used to check if there is a message in the current queue */
#define Is_Msg_Available( queue )   ( queue ).current_capacity

#define Is_Queue_Full( queue )   queueMgmt_isQueueFull(&queue)
/** @brief Used to read the head of the queue */
#define Read_Head( queue )   ( queue.head )

/** @brief Used to dequeue a specific buffer id from the queue */
#define Dequeue_Specific( queue,buffer_id )  queueMgmt_dequeueSpecBuffer ( &queue,buffer_id )

/**************************** Public Type Definitions *************************/

/** @brief  This is the structure to declare a queue */
typedef struct Queue_Tag {
    /*! Holds the head of the queue */
    uint8_t head;
    /*! Holds the tail of the queue */
    uint8_t tail;
    /*! Holds the total capacity of the current queue */
    uint8_t total_capacity;
    /*! Holds the current number of entries in the queue */
    uint8_t current_capacity;
} __attribute__((__packed__))queue_t;

/**************************** Public Enum Definitions *************************/

/* None */

/**************************** Public Variable Declaration *********************/

/* None */

/**************************** Public Constant Declaration *********************/

/* None */

/**************************** Public Function Prototypes **********************/

/** @brief This function initializes the queue
 *
 * @param p_queue_name: This is a pointer to the queue to be initialized
 *
 * @param queue_size: Gives the size of the queue to be initialized
 *
 * @return None
 */

void queueMgmt_Init ( queue_t* p_queue_name, uint8_t queue_size );

/******************************************************************************/

/** @brief This function enqueues a buffer id into the specified queue
 *
 * @param p_queue_name: pointer to the queue into which the buffer id is to be
 *                      enqueued
 *
 * @param buffer_id: The id of the buffer to be enqueued into the queue
 *
 * @return returns g_SUCCESS_c if the buffer id was enqueued successfully else
 *         returns g_QUEUE_FULL_c
 */

uint8_t queueMgmt_enqueue ( queue_t* p_queue_name, uint8_t buffer_id );

/******************************************************************************/

/** @brief This function dequeues the buffer id from the queue
 *
 * @param p_queue_name: pointer to the queue from which the buffer id is to be
 *                      dequeued
 *
 * @return returns the current buffer id in the queue else returns
 *         g_QUEUE_EMPTY_c if there are no buffer id's in the queue
 */

uint8_t queueMgmt_dequeue ( queue_t* p_queue_name );

/******************************************************************************/

/** @brief This function is used to dequeue a specific buffer id from the queue
 *
 * @param p_queue_name: pointer to the queue from which the buffer id is to be
 *                      dequeued
 *
 * @param buffer_id: ID of the buffer to be dequeued
 *
 * @return returns g_HEAD_c if the buffer id is dequeued successfully and
 *                 if the queue head is pointing to that buffer id or
 *                 g_SUCCESS_c if the buffer id is dequeued successfully or
 *                 g_FAILURE_c if the buffer id id is not dequeued successfully
 */
uint8_t queueMgmt_dequeueSpecBuffer ( queue_t* p_queue_name, uint8_t buffer_id );

uint8_t queueMgmt_isQueueFull (queue_t* pQueueName);
#endif  /*__QUEUE_H*/
