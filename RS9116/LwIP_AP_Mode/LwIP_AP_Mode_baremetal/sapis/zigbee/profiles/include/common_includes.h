/** @file common_includes.h
 *
 * @brief  This modules contains all the common include header files, macros and
 *         variables used across all the modules.
 */

#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H
#include"ZigBeeDataTypes.h"

/**************************** Public Macro Definitions ***********************/

/** @brief  If ARM macro is defined in the workspace the packed macro will be
 used */
#ifdef ARM
#define PACKED      __packed
#else
#define PACKED
#endif

/** @brief  Defines the value for NULL */
#define g_NULL_c                       0 //( void* ) ( 0 )
/** @brief  Defines the value for Reset */
#define g_RESET_c                      0x00

/** @brief  Defines the value for Success */
#define g_SUCCESS_c                    0x00

/** @brief  Defines the value for Failure */
#define g_FAILURE_c                    0xFF

/** @brief  Defines the length of the 64 - bit Address or IEEE MAC Address */
#define g_EXTENDED_ADDRESS_LENGTH_c    0x08

/** @brief  Defines the length of the 16 - bit Short Address */
#define g_SHORT_ADDRESS_LENGTH_c       0x02

/** @brief  Defines the length of the 16 - bit Short PAN ID */
#define g_SHORT_PAN_ID_LENGTH_c        0x02

/** @brief  Defines the length for parameters of 8 bytes */
#define g_EIGHT_BYTE_LENGTH_c          0x08

/** @brief  Defines the broadcast PAN ID */
#define g_BROADCAST_PAN_ID_c           0xFFFF

/** @brief  Defines the broadcast PAN ID */
#define g_BROADCAST_ADDRESS_c          0xFFFF

/** @brief  Defines the broadcast PAN ID */
#define g_INVALID_ADDRESS_c            0xFFFF

/** @brief  Defines the broadcast PAN ID */
#define g_INVALID_PAN_ID_c             0xFFFF

/** @brief  Defines Invalid index */
#define g_INVALID_INDEX_c              0xFF

/** @brief  Defines Invalid value */
#define g_INVALID_VALUE_c              0xFF

/** @brief Short address is not allocated */
#define g_ADDRESS_NOT_ALLOCATED_c      0xFFFE

/** @brief Index to CMD Id field in MAC Cmd Frames */
#define g_IDX_TO_CMD_ID_c      0x00

#define ENTER_CRITICAL_REGION(x) ;
#define LEAVE_CRITICAL_REGION(x) ;

/**************************** Public Type Definitions ************************/

/** @brief Defines a union for Destination/Source/Coordinator Address */
typedef union Addr_Tag {
    /*! Short address */
    uint8_t a_short_addr[2];

    /*! Extended address */
    uint8_t a_extend_addr[8];
} Addr_t;

/** @brief Defines type for time values */
typedef uint32_t Time_t;

/** @brief Defines a type for loop counter variable */
typedef uint32_t Loop_Ctr_t;

/**************************** Public Enum Definitions ************************/

/*! @brief Boolean values for true and false */
typedef enum Bool_Tag {
    /*! -False is defined as 0x00 */
    g_FALSE_c,

    /*! -True is defined as 0x01 */
    g_TRUE_c
} Bool_t;

/*****************************************************************************/
#if 1
/*! @brief Address Mode values */
typedef enum Addr_Mode_Tag {
    /*! -0x00 - No address mode specified */
    g_NO_ADDR_MODE_c,

    /*! -0x01 - Reserved */
    g_RESERVED_MODE_c,

    /*! -0x02 - Short address mode */
    g_SHORT_ADDR_MODE_c,

    /*! -0x03 - Long or extended address mode */
    g_EXTENDED_ADDR_MODE_c
} Addr_Mode_t;
#endif
/**************************** Public Variable Declaration ********************/

/*   Return type for ZLL functions. */
typedef uint8_t ZLL_Status;


/**************************** Public Constant Declaration ********************/

/* None */

/**************************** Public Function Prototypes *********************/

void MAC_PollCompleteHandler(uint8_t status);

#endif  /*COMMON_INCLUDES_H*/
