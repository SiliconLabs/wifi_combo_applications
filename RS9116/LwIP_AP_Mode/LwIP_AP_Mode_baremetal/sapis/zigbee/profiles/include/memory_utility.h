/** @file memory_utility.h
 *
 * @brief  This is the header file for the module which contains memory utility
 *         APIs which can be used by all other modules
 *
 */

#ifndef __MEMORY_UTILITY_H
#define __MEMORY_UTILITY_H
#define m_LENGTH_EIGHT_BYTES_c          0x08

/**************************** Public Macro Definitions ***********************/

/* None */

/**************************** Public Type Definitions ************************/

/* None */

/**************************** Public Enum Definitions ************************/

/*! @brief  Values for memory match and memory no match */
typedef enum {
    /*! -memory locations match */
    g_MEMORY_MATCH_c,

    /*! -memory locations does not match */
    g_MEMORY_NO_MATCH_c

} memUtils_compTag_t;

/**************************** Public Variable Declaration ********************/

/* None */

/**************************** Public Constant Declaration ********************/

/* None */

/**************************** Public Function Prototypes *********************/

/** @brief This function is used to copy bytes from one memory location to
 *          another
 *
 * @param p_destination: Pointer to the start of the detination memory location
 *                       to which bytes have to be copied
 *
 * @param p_source: Pointer to the start of the source memory location from which
 *                  bytes are copied
 *
 * @param length: Length for which bytes have to be copied
 *
 * @return None
 */
void memUtils_memCopy ( uint8_t* p_destination, uint8_t* p_source,
        uint8_t length );

/******************************************************************************/

/** @brief This function is used to set a particular memory location with the
 *          desired  value
 *
 * @param p_destination: Pointer to the start of the detination memory location
 *                       whose value has to be set
 *
 * @param value: Value with which the destination has to be set
 *
 * @param length: Length for which value has to be set
 *
 * @return None
 */
void memUtils_memSet ( uint8_t* p_destination, uint8_t value, uint16_t length );

/******************************************************************************/

/** @brief This function is used to copy bytes in the reverse order from one
 *          memory location to another
 *
 * @param p_destination: Pointer to the start of the detination memory location
 *                       to which bytes have to be copied
 *
 * @param p_source: Pointer to the start of the source memory location from which
 *                  bytes are copied.
 *
 * @param length: Length for which bytes have to be copied
 *
 * @return None
 */
void memUtils_memReverseCopy ( uint8_t* p_destination, uint8_t* p_source,
        uint8_t length );

/******************************************************************************/

/** @brief This function is used to compare the bytes in two  memory locations.
 *
 * @param p_mem1: Pointer to the first memory location whose bytes have to be
 *                compared with those in the second memory location
 *
 * @param p_mem2: Pointer to the second memory location whose bytes have to be
 *                compared with those in the first memory location
 *
 * @param length: Length for which bytes have to be compared
 *
 * @return  returns the result of memory compare operation
 *          returns g_MEMORY_NO_MATCH_c if the memory locations does not match
 *          or g_MEMORY_MATCH_c if the memory locations match
 */
uint8_t memUtils_memCompare ( uint8_t* p_mem1, uint8_t* p_mem2, uint8_t length );

/******************************************************************************/

/** @brief This function is used to copy 8 bytes from one memory location to
 *          another
 *
 * @param p_destination: Pointer to the start of the detination memory location
 *                       to which bytes have to be copied
 *
 * @param p_source: Pointer to the start of the source memory location from which
 *                  bytes are copied
 *
 * @return None
 */
void memUtils_memCopy8Bytes ( uint8_t* p_destination, uint8_t* p_source );

/******************************************************************************/

/** @brief This function is used to shift the bytes in one  memory locations to
 *          right.
 *
 * @param p_source: Pointer to the source memory location from which bytes have
 *                  to be shifted
 *
 * @param total_length: Total number of bytes to be shifted
 *
 * @param shift_length: Length for which bytes have to be shifted
 *
 * @return None
 */
void memUtils_memRightShift ( uint8_t* p_source, uint8_t total_length,
        uint8_t shift_length );

/******************************************************************************/

/** @brief This function is used to swap 2 bytes
 *
 * @param p_byte: Pointer to the first byte
 *
 * @return None
 */
void memUtils_swap2Bytes ( uint8_t* p_byte );

/******************************************************************************/

/** @brief This function is used to compare bytes in two memory locations, in
 * reverse order.
 *
 * @param p_Mem1: Pointer to the first memory location
 *
 * @param p_Mem2: Pointer to the second memory location
 *
 * @param length: length of the memory block
 *
 * @return returns the result of memory compare operation
 *          returns g_MEMORY_NO_MATCH_c if the memory locations does not match
 *          or g_MEMORY_MATCH_c if the memory locations match
 */

uint8_t memUtils_memReverseCmp ( uint8_t *p_Mem1, uint8_t *p_Mem2,
        uint8_t length );

/******************************************************************************/

/** @brief This function is used to shift the octet string in one  memory locations
 * to right.
 *
 * @param p_source: Pointer to the source memory location from which octets have
 *                  to be shifted
 *
 * @param total_length: Total number of bytes to be shifted
 *
 * @param shift_length: number of octets that has to be shifted
 *
 * @return None
 */

void memUtils_shiftRightOctetString ( uint8_t* p_src, uint8_t* p_dest,
        uint8_t no_of_octets );

/******************************************************************************/

/** @brief This function is used to copy bytes from src to dst starting from last byte.
 *
 * @param p_destination: Pointer to the destination memory location to which bytes have
 *                       to be copied
 * @param p_source: Pointer to the source memory location from which bytes have
 *                  to be copied
 *
 * @param total_length: Total number of bytes to be copied
 *
 * @return None
 */
void memUtils_memRevDirection ( uint8_t* p_destination, uint8_t* p_source,
        uint8_t total_length );

/******************************************************************************/
#endif  /*__MEMORY_UTILITY_H*/
