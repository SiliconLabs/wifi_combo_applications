/**
 * @file      rsi_queue.c
 * @version   0.1
 * @date      15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains queue management functionality
 *
 *  @section Description  This file contains queue management functionality
 *
 *
 */
/*==============================================*/
/**
 * Include files
 * */
#ifndef ROM_WIRELESS 
#include "rsi_driver.h"

/*==============================================*/
/**
 *
 * @fn         void ROM_WL_rsi_queues_init(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
 * @brief      Initializes queue
 * @param[in]  queue , pointer to queue 
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[out] None
 * @return     None
 *
 * @section description
 * This function initializes queue
 *
 *
 */
void ROM_WL_rsi_queues_init(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
{
  //! Initialize head pointer to NULL
  queue->head = NULL;

  //! Initialize tail pointer to NULL
  queue->tail = NULL;

  //! Initialize pending packet count to zero
  queue->pending_pkt_count = 0;

  //! Initialize queue mask to zero
  queue->queue_mask = 0;
  
  //! Mutex for  queues 
  RSI_MUTEX_CREATE(&queue->queue_mutex);

}

/*==============================================*/
/**
 * @fn          void ROM_WL_rsi_enqueue_pkt_from_isr(global_cb_t *global_cb_p, rsi_queue_cb_t *queue,rsi_pkt_t *pkt)
 * @brief       Enqueues packet into queue from isr
 * @param[in]   queue , pointer to queue cb
 * @param[in]   pkt , packet pointer which needs to be queued
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[out]  None
 * @return      None
 *
 * @section description
 * This function enqueues packet into queue
 *
 */

void ROM_WL_rsi_enqueue_pkt_from_isr(global_cb_t *global_cb_p, rsi_queue_cb_t *queue,rsi_pkt_t *pkt)
{
  //! check queue is empty
  if(!queue->pending_pkt_count)
  {
    //! if empty then add packet as first packet (head & tail point to first packet)
    queue->head = queue->tail = pkt;
  }
  else
  {
    //! if not empty append the packet to list at tail
    queue->tail->next = pkt;

    //! Make packet as tail
    queue->tail = pkt;
  }

  //! increment packet pending count
  queue->pending_pkt_count++;
}

/*==============================================*/
/**
 * @fn          void ROM_WL_rsi_enqueue_pkt(global_cb_t *global_cb_p, rsi_queue_cb_t *queue,rsi_pkt_t *pkt)
 * @brief       Enqueues packet into queue
 * @param[in]   queue , pointer to queue cb 
 * @param[in]   pkt , packet pointer which needs to be queued 
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[out]  None
 * @return      None
 *
 * @section description
 * This function enqueues packet into queue
 *
 */

void ROM_WL_rsi_enqueue_pkt(global_cb_t *global_cb_p, rsi_queue_cb_t *queue,rsi_pkt_t *pkt)
{

  //! take lock on queue
  RSI_MUTEX_LOCK(&queue->queue_mutex);

  //! check queue is empty
  if(!queue->pending_pkt_count)
  {
    //! if empty then add packet as first packet (head & tail point to first packet)
    queue->head = queue->tail = pkt;
  }
  else
  {
    //! if not empty append the packet to list at tail
    queue->tail->next = pkt;

    //! Make packet as tail
    queue->tail = pkt;
  }

  //! increment packet pending count
  queue->pending_pkt_count++;

  //! Unlock the queue
  RSI_MUTEX_UNLOCK(&queue->queue_mutex);

}
/*==============================================*/
/**
 * @fn          rsi_pkt_t *ROM_WL_rsi_dequeue_pkt(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
 * @brief       Dequeues packet from queue
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   queue , queue pointer from which packet needs to be dequeued 
 * @param[out]  None
 * @return      None
 *
 *
 * @section description
 * This function dequeues packet from queue
 *
 */

rsi_pkt_t *ROM_WL_rsi_dequeue_pkt(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
{
  rsi_pkt_t *pkt;

  //! lock the mutex
  RSI_MUTEX_LOCK(&queue->queue_mutex);
   
  //! check queue is empty
  if(!queue->pending_pkt_count)
  {
   
    RSI_MUTEX_UNLOCK(&queue->queue_mutex);
    //! return NULL if queue is empty
    return NULL;
  }

  //! dequeue the packet from queue 
  pkt = queue->head;

  //! update the queue head and decrement pending count
  queue->head = queue->head->next; 

  //! Decrease pending packet count
  queue->pending_pkt_count--;

  //! if pending count is zero, then reset head and tail 
  if(!queue->pending_pkt_count)
  {
    queue->head = NULL;
    queue->tail = NULL;
  }

  //! Unlock the mutex
  RSI_MUTEX_UNLOCK(&queue->queue_mutex);

  //! return the packet
  return pkt;
}
/*==============================================*/
/**
 * @fn          uint32_t ROM_WL_rsi_check_queue_status(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
 * @brief       returns whether packets pending count in queue
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   queue, pointer to queue  
 * @param[out]  None
 * @return      None
 *
 *
 * @section description
 * This fucntion returns packet pending count in queue
 *
 *
 */

uint32_t ROM_WL_rsi_check_queue_status(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
{
 
  uint32_t pkt_count = 0;
   
  //! lock the mutex
  RSI_MUTEX_LOCK(&queue->queue_mutex);
 
  //! check whether queue is masked or not
  if(queue->queue_mask)
  {

    //! Unlock the mutex
    RSI_MUTEX_UNLOCK(&queue->queue_mutex);

    //! if queue masked return 0
    return 0;
  }
  else
  {
    pkt_count = queue->pending_pkt_count;

    //! Unlock the mutex
    RSI_MUTEX_UNLOCK(&queue->queue_mutex);

    //! if queue is not masked return number of packets pending
    return pkt_count;
  }
}

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_block_queue(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
 * @brief       blocks the queue
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   queue, pointer to queue
 * @return      none  
 * @section description 
 * This function is used to block the queue.
 */
void ROM_WL_rsi_block_queue(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
{
  //! lock the mutex
  RSI_MUTEX_LOCK(&queue->queue_mutex);

  //! Mask the queue
  queue->queue_mask =1;

  //! Unlock the mutex
  RSI_MUTEX_UNLOCK(&queue->queue_mutex);

}

/*====================================================*/
/**
 * @fn          void ROM_WL_rsi_unblock_queue(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
 * @brief       unblocks the queue.
 * @param[in]   global_cb_p,pointer to the global control block
 * @param[in]   queue
 * @return      none  
 * @section description 
 * This function is used to unblock the queue.
 */
void ROM_WL_rsi_unblock_queue(global_cb_t *global_cb_p, rsi_queue_cb_t *queue)
{

  //! lock the mutex
  RSI_MUTEX_LOCK(&queue->queue_mutex);

  //! Unmask the queue
  queue->queue_mask = 0;

  //! Unlock the mutex
  RSI_MUTEX_UNLOCK(&queue->queue_mutex);

}
#endif
