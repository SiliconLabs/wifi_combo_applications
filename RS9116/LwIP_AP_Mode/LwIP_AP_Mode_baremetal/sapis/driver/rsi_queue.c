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
#include "rsi_driver.h"

/*==============================================*/
/**
 *
 * @fn         void rsi_queues_init(rsi_queue_cb_t *queue)
 * @brief      Initializes queue
 * @param[in]  queue , pointer to queue 
 * @param[out] None
 * @return     None
 *
 * @section description
 * This function initializes queue
 *
 *
 */
void rsi_queues_init(rsi_queue_cb_t *queue)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_queues_init(global_cb_p, queue);
#else
  api_wl->rsi_queues_init(global_cb_p, queue);
#endif
}
/*==============================================*/
/**
 * @fn          void rsi_enqueue_pkt(rsi_queue_cb_t *queue, rsi_pkt_t *pkt)
 * @brief       Enqueues packet into queue
 * @param[in]   queue , pointer to queue cb 
 * @param[in]   pkt , packet pointer which needs to be queued 
 * @param[out]  None
 * @return      None
 *
 * @section description
 * This function enqueues packet into queue
 *
 */

void rsi_enqueue_pkt(rsi_queue_cb_t *queue,rsi_pkt_t *pkt)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_enqueue_pkt(global_cb_p, queue, pkt);
#else
  api_wl->rsi_enqueue_pkt(global_cb_p, queue, pkt);
#endif
}
/*==============================================*/
/**
 * @fn          rsi_pkt_t *rsi_dequeue_pkt(rsi_queue_cb_t *queue)
 * @brief       Dequeues packet from queue
 * @param[in]   queue , queue pointer from which packet needs to be dequeued 
 * @param[out]  None
 * @return      None
 *
 *
 * @section description
 * This function dequeues packet from queue
 *
 */

rsi_pkt_t *rsi_dequeue_pkt(rsi_queue_cb_t *queue)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_dequeue_pkt(global_cb_p, queue);
#else
  return api_wl->rsi_dequeue_pkt(global_cb_p, queue);
#endif
}
/*==============================================*/
/**
 * @fn          uint32_t rsi_check_queue_status(rsi_queue_cb_t *queue)
 * @brief       returns whether packets pending count in queue
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

uint32_t rsi_check_queue_status(rsi_queue_cb_t *queue)
{
#ifdef ROM_WIRELESS
  return ROMAPI_WL->rsi_check_queue_status(global_cb_p, queue);
#else
  return api_wl->rsi_check_queue_status(global_cb_p, queue);
#endif
}

/*====================================================*/
/**
 * @fn          void rsi_block_queue(rsi_queue_cb_t *queue)
 * @brief       blocks the queue
 * @param[in]   queue, pointer to queue
 * @return      none  
 * @section description 
 * This function is used to block the queue.
 */
void rsi_block_queue(rsi_queue_cb_t *queue)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_block_queue(global_cb_p, queue);
#else
  api_wl->rsi_block_queue(global_cb_p, queue);
#endif
}

/*====================================================*/
/**
 * @fn          void rsi_unblock_queue(rsi_queue_cb_t *queue)
 * @brief       unblocks the queue.
 * @param[in]   queue, pointer to queue
 * @return      none  
 * @section description 
 * This function is used to unblock the queue.
 */
void rsi_unblock_queue(rsi_queue_cb_t *queue)
{
#ifdef ROM_WIRELESS
  ROMAPI_WL->rsi_unblock_queue(global_cb_p, queue);
#else
  api_wl->rsi_unblock_queue(global_cb_p, queue);
#endif
}

