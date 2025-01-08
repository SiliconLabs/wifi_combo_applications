/** @buffer_management.h
 *
 * @brief  Description of the module
 *
 */


#ifndef BUFFER_MANAGEMENT_H
#define BUFFER_MANAGEMENT_H

/**************************** Public Macro Definitions ***********************/

/** @brief  Indicates unavailability of a buffer  */
#define g_NO_AVAILABLE_BUFFER_c    (uint8_t)0xFF

/** @brief  Specifies the value for an invalid buffer */
#define g_INVALID_BUFFER_ID_c       (uint8_t)0xFF

/** @brief  Specifies the value to allocatte a Small Buffer */
#define g_SMALL_BUFFER_c    (uint8_t)60

/** @brief  Specifies the value to allocatte a Large Buffer */
#define g_LARGE_BUFFER_c    (uint8_t)156

/**************************** Public Type Definitions ************************/

typedef uint8_t Buffer_Id_t;

/**************************** Public Enum Definitions ************************/

/* None */

/**************************** Public Variable Declaration ********************/

/* None */

/**************************** Public Constant Declaration ********************/

/* None */

/**************************** Public Function Prototypes *********************/
/** @brief This function shall be called at system reset to initialise buffer
 *          management
 *
 * @param None
 *
 * @return None
 */
void buffMgmt_init ( void );

/******************************************************************************/
/** @brief This function shall be called by application to request for dynamic
 *          memory allocation. This API is capable of allocating a buffer of size
 *          c_small_buffer_size or c_large_buffer_size depending on the number of
 *          bytes requested. If number of bytes requested is less than or equal
 *          to c_small_buffer_size, buffer of size c_small_buffer_size will be
 *          allocated else a buffer of size c_large_buffer_size will be allocated
 *
 * @param size: The number of bytes requested.
 *
 * @return The buffer_id of the buffer allocated, or 0xFF if the allocation fails
 */
Buffer_Id_t buffMgmt_allocateBuffer ( uint8_t size );

/******************************************************************************/
/** @brief This function shall be called by application to free a buffer
 *          previously allocated by buffMgmt_allocateBuffer
 *
 * @param buf_id: The buffer_id of the buffer to be freed
 *
 * @return None
 */
void buffMgmt_freeBuffer ( Buffer_Id_t buffer_id, uint8_t location );
/******************************************************************************/
/** @brief This function shall be called by application to get a pointer to a
 *          buffer previously allocated by buffMgmt_allocateBuffer
 *
 * @param buf_id: The buffer_id of the buffer for which pointer is required
 *
 * @return Pointer to the buffer id equal to buffer_id or null pointer if
 *         buffer_id is not a valid buffer_id.
 */
uint8_t* buffMgmt_getBufferPointer ( Buffer_Id_t buffer_id );

/******************************************************************************/

/** @brief This function shall be called by application to get the number of
 *          buffers. The number of small free buffer is returned if size is less
 *          than or equal to c_small_buffer_size else the number of free large
 *          buffers is returnd. The function is meant only for debug purpose.
 *
 * @param size: The size of the buffer, whose availability is getting checked
 *
 * @return The number of free buffers
 */
uint8_t buffMgmt_getNumberofFreeBuffers ( uint8_t size );


uint8_t buffMgmt_isBufferIdValid (Buffer_Id_t buffer_id);
#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
void app_PacketBufferAllocateFailuresHandler(void);
#endif

#endif  /*BUFFER_MANAGEMENT_H*/
