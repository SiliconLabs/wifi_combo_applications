/** @buffer_management.c
 *
 * @brief  Description of the module
 *
 */


/**************************** Includes ***************************************/
#include "ZigBeeDataTypes.h"
#include "common_includes.h"
#include "mac_stack_configuration.h"
#include "queue.h"
#include "buffer_management.h"

#if(DEBUG_TEST == 1)
#include "memory_utility.h"
#endif /* (DEBUG_TEST == 1) */

/**************************** Private Macro Definitions **********************/

#define DBG_BIT(x)  ((uint32_t)1 << (x))

#define m_IS_SMALL_BUFFER_c( bufferId ) \
                ( ( ( bufferId ) < g_Total_Number_Of_Small_Buffers_c ) ? 1 : 0 )

/**************************** Private type Definitions ***********************/
typedef struct {
    queue_t freeLargeBufferQueue;
    queue_t freeSmallBufferQueue;
} __attribute__((__packed__))buffMgmt_freeBuffersQ_t;

/**************************** Private Enum Definitions ***********************/

/* None */

/**************************** Private Variable Definitions *******************/
static buffMgmt_freeBuffersQ_t m_freeBuffersQ;

static uint8 m_bufferMgmtState;

/**************************** Public Variable Definitions ********************/

#if (DEBUG_TEST == 1)
extern uint8 allocatedbuffer[24];
extern uint8 freebuffer[24][2];
#endif /* DEBUG_TEST */

#if (DEBUG_TEST == 1)
extern uint8 testHrns_passMessageUp(uint8, uint8);
#endif
#ifdef ZB_BUF_DBG
typedef struct zb_dbg_buf_mgmt_s {
uint32_t dbg_buf_status;
uint32_t zigb_invalid_freeing;
uint32_t buf_alloc_called;
uint32_t buf_free_called;
}zb_dbg_buf_mgmt_t;
zb_dbg_buf_mgmt_t zb_dbg_buf_mgmt;
#endif
/**************************** Private Constant Definitions *******************/

/* None */

/**************************** Public Constant Definitions ********************/

/* None */

/**************************** Private Function Prototypes ********************/

/* None */

/**************************** Public Functions Definitions *******************/
void buffMgmt_init ( void )
{
    Buffer_Id_t bufferId = 0;
#ifdef ZB_BUF_DBG
	zb_dbg_buf_mgmt.dbg_buf_status = 0;
	zb_dbg_buf_mgmt.zigb_invalid_freeing = 0;
	zb_dbg_buf_mgmt.buf_alloc_called = 0;
	zb_dbg_buf_mgmt.buf_free_called = 0;
#endif
    /* Initialize free buffer queue for large buffers */
    queueMgmt_Init(&m_freeBuffersQ.freeLargeBufferQueue,
                   g_Total_Number_Of_Large_Buffers_c);

    /* Initialize free buffer queue for small buffers */
    queueMgmt_Init(&m_freeBuffersQ.freeSmallBufferQueue,
                   g_Total_Number_Of_Small_Buffers_c);

    for ( ; bufferId < g_Total_Number_Of_Small_Buffers_c; bufferId++ ) {
        /* Append the buffer to free small buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeSmallBufferQueue, bufferId);
    }

    for ( ; bufferId < g_Total_Number_Of_Buffers_c; bufferId++ ) {
        /* Append the buffer to free large buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeLargeBufferQueue, bufferId);
    }

}

/******************************************************************************/
Buffer_Id_t buffMgmt_allocateBuffer ( uint8 size )
{
    Buffer_Id_t bufferId = g_NO_AVAILABLE_BUFFER_c;
    if (m_bufferMgmtState == g_NO_AVAILABLE_BUFFER_c) {
        return m_bufferMgmtState;
    }
    m_bufferMgmtState = g_NO_AVAILABLE_BUFFER_c;

    /*
     * Allocate buffer only if size requested is less than or equal to  maximum
     * size that can be allocated.
     */
    if (size <= g_Large_Buffer_Size_c) {
        /*
         * Allocate small buffer if size is less than small buffer size and if
         * small buffer is available allocate from small buffer pool.
         */
        if ((size <= g_Small_Buffer_Size_c)) {
            /* Allocate buffer from free small buffer queue */
            bufferId = queueMgmt_dequeue(&m_freeBuffersQ.freeSmallBufferQueue);
        }

        /*
         * If size is greater than small buffer size or no free small buffer is
         * available, allocate a buffer from large buffer pool if avialable
         */
        if (bufferId == (uint8) 0xFF) {
            /* Allocate buffer from free large buffer queue */
            bufferId = queueMgmt_dequeue(&m_freeBuffersQ.freeLargeBufferQueue);
        }
    }
#ifdef ZB_BUF_DBG
    if (bufferId != 0xFF) {
			zb_dbg_buf_mgmt.buf_alloc_called++;
    	if (zb_dbg_buf_mgmt.dbg_buf_status & DBG_BIT(bufferId)) {
    		while(1);
    	}
      	zb_dbg_buf_mgmt.dbg_buf_status |= DBG_BIT(bufferId);
    }
#endif
    m_bufferMgmtState = 0;
    return bufferId;
} /* buffMgmt_allocateBuffer */

/******************************************************************************/
void buffMgmt_freeBuffer ( Buffer_Id_t bufferId, uint8 location )
{
#if 0
#if(DEBUG_TEST == 1)
    if(allocatedbuffer[bufferId] == 0) {
        uint8 IndexForBufferTest;
        uint8 *pForBufferTest;
        IndexForBufferTest = buffMgmt_allocateBuffer ( g_SMALL_BUFFER_c );
        /* Buffer ID --> 88 */
        allocatedbuffer[IndexForBufferTest] = 88;

        pForBufferTest = buffMgmt_getBufferPointer(IndexForBufferTest);
        if (g_NULL_c != pForBufferTest) {
            memUtils_memSet(pForBufferTest, 0x0, g_SMALL_BUFFER_c);
            *pForBufferTest = 0xFC;
            *(pForBufferTest + 1) = freebuffer[bufferId][0];
            *(pForBufferTest + 2) = freebuffer[bufferId][1];
            *(pForBufferTest + 3) = location;
            testHrns_passMessageUp(IndexForBufferTest, 4);
        }
        return;
    }
    allocatedbuffer[bufferId] = 0;
    freebuffer[bufferId][1] = location;
#endif
#endif
    if (bufferId >= g_Total_Number_Of_Buffers_c) {
        /* If the buffer id is not valid abort free operation */
        return;
    }
 
#ifdef ZB_BUF_DBG
  	zb_dbg_buf_mgmt.buf_free_called++;
    if (bufferId != 0xFF) {
    	if (!(zb_dbg_buf_mgmt.dbg_buf_status & DBG_BIT(bufferId))) {
    		 zb_dbg_buf_mgmt.zigb_invalid_freeing++;
    		 return;
    	}
      	zb_dbg_buf_mgmt.dbg_buf_status &= ~DBG_BIT(bufferId);
    }
#endif
    if (m_IS_SMALL_BUFFER_c( bufferId )) {
        /* Append the buffer into free small buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeSmallBufferQueue, bufferId);
    }
    else {
        /* Append the buffer into free large buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeLargeBufferQueue, bufferId);
    }

}

/******************************************************************************/
uint8* buffMgmt_getBufferPointer ( Buffer_Id_t bufferId )
{
    uint8 *p_buffer_pointer;
    uint16 offset_to_large_buffers;

    if (bufferId >= g_Total_Number_Of_Buffers_c) {
#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )	
		app_PacketBufferAllocateFailuresHandler();
#endif     /* g_DIAGNOSTICS_CLUSTER_ENABLE_d */				
        /* If the buffer id is not valid abort operation */
        return g_NULL_c;
    }

    if (m_IS_SMALL_BUFFER_c( bufferId )) {
        /* Evaluate the base address of the buffer */
        p_buffer_pointer = (uint8*) ga_Heap + (bufferId
                * g_Small_Buffer_Size_c);
    }
    else {
        /* Evaluate the base address of the buffer */
        offset_to_large_buffers = g_Total_Number_Of_Small_Buffers_c *
            g_Small_Buffer_Size_c;

        p_buffer_pointer = (uint8*) ga_Heap +
            (offset_to_large_buffers + (bufferId - g_Total_Number_Of_Small_Buffers_c) *
             g_Large_Buffer_Size_c);
    }
    return p_buffer_pointer;
}

/******************************************************************************/

uint8 buffMgmt_getNumberofFreeBuffers ( uint8 bufferSize )
{
    uint8 freeBuffers;

    if (bufferSize <= g_Small_Buffer_Size_c) {
        freeBuffers = m_freeBuffersQ.freeSmallBufferQueue.current_capacity;
    }
    else {
        freeBuffers = m_freeBuffersQ.freeLargeBufferQueue.current_capacity;
    }
    return freeBuffers;
}

uint8 buffMgmt_isBufferIdValid (Buffer_Id_t bufferId)
{
    if (bufferId >= g_Total_Number_Of_Buffers_c) {
        return g_FALSE_c;
    }
    return g_TRUE_c;
}

/**************************** Private Function Definitions *******************/

/* None */

/******************************************************************************/
