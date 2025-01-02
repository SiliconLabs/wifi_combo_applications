/** @file memory_utility.c
 *
 * @brief  This module contains memory utility APIs which can be used by all
 *         other modules
 *
 */

/**************************** Includes ***************************************/

#include "ZigBeeDataTypes.h"
#include "common_includes.h"
#include "memory_utility.h"

/**************************** Private Macro Definitions **********************/

/* None */

/**************************** Private type Definitions ***********************/

/* None */

/**************************** Private Enum Definitions ***********************/

/* None */

/**************************** Private Variable Definitions *******************/

/* None */

/**************************** Public Variable Definitions ********************/

/* None */

/**************************** Private Constant Definitions *******************/

/* None */

/**************************** Public Constant Definitions ********************/

/* None */

/**************************** Private Function Prototypes ********************/

/* None */

/**************************** Public Functions Definitions *******************/

void memUtils_memCopy ( uint8_t* pDestination, uint8_t* pSource, uint8_t length )
{
    for ( ; length > 0; length-- ) {
        /* Copying byte by byte for the entire length */
        *pDestination++ = *pSource++;
    }
}

/******************************************************************************/

void memUtils_memSet ( uint8_t* pDestination, uint8_t value, uint16_t length )
{
    for ( ; length > 0; length-- ) {
        /* Setting the entire length with the desired value byte by byte */
        *pDestination++ = value;
    }
}

/******************************************************************************/

void memUtils_memReverseCopy ( uint8_t* pDestination, uint8_t* pSource,
        uint8_t length )
{

    while (length-- > 0) {
        /* Copying from the end of the source memory location byte by byte */
        *pDestination++ = *(pSource + length);
    }
}

/******************************************************************************/

uint8_t memUtils_memCompare ( uint8_t* pMem1, uint8_t* pMem2, uint8_t length )
{

    while (length-- > 0) {
        /* Comparing the two memory locations byte by byte for the entire
         length */
        if (*(pMem1 + length) != *(pMem2 + length)) {
            return g_MEMORY_NO_MATCH_c;
        }
    }
    return g_MEMORY_MATCH_c;
}

/******************************************************************************/

void memUtils_memCopy8Bytes ( uint8_t* pDestination, uint8_t* pSource )
{
    memUtils_memCopy(pDestination, pSource, g_EIGHT_BYTE_LENGTH_c);
}

/******************************************************************************/

void memUtils_memRightShift ( uint8_t* pSource, uint8_t totalLength,
        uint8_t shiftLength )
{
    /* Getting the end of the destination to which bytes needs to be shifted */
    uint8_t* pDestination = pSource + totalLength + shiftLength - 0x01;

    /* Getting the end of the source from which bytes are shifted */
    pSource += totalLength - 0x01;

    for ( ; totalLength > 0; totalLength-- ) {
        /* Copying byte by byte */
        *pDestination-- = *pSource--;
    }
}

void memUtils_memRevDirection ( uint8_t* pDestination, uint8_t* pSource,
        uint8_t totalLength )
{

    for ( ; totalLength > 0; totalLength-- ) {
        /* Copying byte by byte */
        *(pDestination + totalLength - 1) = *(pSource + totalLength - 1);
    }
}
/******************************************************************************/

void memUtils_swap2Bytes ( uint8_t* pByte )
{
    /* Getting the first byte to a variable */
    uint8_t temp = *pByte;

    /* Copying the second byte to the first byte */
    *pByte = *(pByte + 1);

    /* Copying the first byte to the second byte */
    *(pByte + 1) = temp;
}

/******************************************************************************/
void memUtils_shiftRightOctetString ( uint8_t* pSrc, uint8_t* pDest,
        uint8_t noOfOctets )
{
    uint8_t tempBuf[110];

    if ( ( noOfOctets <= 110 ) ) {
        memUtils_memCopy(tempBuf, pSrc, noOfOctets);

        memUtils_memCopy(pDest, tempBuf, noOfOctets);
    }
}

/******************************************************************************/

uint8_t memUtils_memReverseCmp ( uint8_t *pMem1, uint8_t *pMem2, uint8_t length )
{
    uint8_t count;

    for ( count = 0; count < length; count++ ) {
        if (*(pMem1 + count) != *(pMem2 + length - count - 0x01)) {
            return g_MEMORY_NO_MATCH_c;
        }
    }
    return g_MEMORY_MATCH_c;
}

/**************************** Private Function Definitions *******************/

/* None */
