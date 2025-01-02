/** @file stack_configuration.h
 *
 * @brief Exports the configurable parameters of MAC PHY
 *
 */

#ifndef MAC_STACK_CONFIGURATION_H
#define MAC_STACK_CONFIGURATION_H
#include "supported_features.h"

/**************************** Public Macro Definitions ***********************/

/* Enabled macro for silent start */
#define g_MAC_RFD_c
/** @brief  The number of timers internally used by the MAC */
#define g_NO_OF_MAC_TIMERS_c    (20)

/** @brief  The number of timers used by the application running over the MAC */
#define g_NO_OF_APP_TIMERS_c    (10)

/** @brief  Number of fast response timers provided by the timer module */
#define g_NO_OF_FAST_TIMERS_c   (2)

/** @brief  Total number of timers provided by the timer module */
#define g_TOT_NO_OF_TIMERS_c    (g_NO_OF_MAC_TIMERS_c + g_NO_OF_APP_TIMERS_c + g_NO_OF_FAST_TIMERS_c)

/** @brief  Defines the size of small buffer */
#define g_SMALL_BUFFER_SIZE_c  ( (uint8_t)100 )

/** @brief  Defines the size of large buffer */
#define g_LARGE_BUFFER_SIZE_c  ( (uint8_t)200 )

/** @brief  Defines the number of small buffer */
#define g_TOTAL_NUMBER_OF_SMALL_BUFFERS_c  ( (uint8_t)4 )

/** @brief Default value for channels supported */
#define  m_DEFAULT_PHY_CHANNELS_SUPPORTED_c           (0x07FFF800)



/** @brief  Defines the number of large buffer */

#ifndef g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c
#define g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c  ( (uint8_t)20 ) //RP: 8
#endif

/** @brief  Defines the number of total buffer */
#define g_TOTAL_NUMBER_OF_BUFFERS_c \
   (uint8_t)( g_TOTAL_NUMBER_OF_SMALL_BUFFERS_c + g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c )

/******************************************************************************/

///** @brief Macro for indicating MAC Layer is the highest layer */
//#define g_MAC_LAYER_c           (2)
//
///** @brief Macro for indicating PHY Layer is the highest layer */
//#define g_PHY_LAYER_c           (1)
//
///** @brief Macro for indicating there is no highest layer */
//#define g_NO_LAYER_c            (0)

/******************************************************************************/
//#define g_MAC_HIGHEST_LAYER_d

//#ifdef g_PHY_HIGHEST_LAYER_d
//
///** @brief Macro which holds the value of the highest layer */
//#define g_HIGHEST_LAYER_d        g_PHY_LAYER_c
//
//#else /* g_PHY_HIGHEST_LAYER_d */
//
//#ifdef g_MAC_HIGHEST_LAYER_d
//
///** @brief Macro which holds the value of the highest layer */
//#define g_HIGHEST_LAYER_d        g_MAC_LAYER_c
//
//#else
//
///** @brief Macro which holds the value of the highest layer */
//#define g_HIGHEST_LAYER_d        g_NO_LAYER_c
//
//#endif /* g_MAC_HIGHEST_LAYER_d */
//
//#endif /* g_PHY_HIGHEST_LAYER_d */

/******************************************************************************/

//#if   ( g_HIGHEST_LAYER_d == g_MAC_LAYER_c )

/** @brief Number of tables stored in the PSM */
#define g_PSM_INFO_ENTRIES_c    (3)

//#elif ( g_HIGHEST_LAYER_d == g_PHY_LAYER_c )
//
///** @brief Number of tables stored in the PSM */
//#define g_PSM_INFO_ENTRIES_c    (1)
//
//#else
//
///** @brief Number of tables stored in the PSM */
//#define g_PSM_INFO_ENTRIES_c    (0)
//
//#endif /* ( g_HIGHEST_LAYER_d == g_MAC_LAYER_c ) */

/*------------------------ Security Related Macros ------------------------*/

/** @brief Maximum number of ACL entries supported in the MAC Layer */
#define g_MAX_ACL_TABLE_ENTRIES_c                     ( uint8_t )0x08

/**************************** Public Type Definitions ************************/

/* None */

/**************************** Public Enum Definitions ************************/

/* None */

/**************************** Public Variable Declaration ********************/

extern uint8_t ga_Heap[]; /* Common Buffer pool holding the buffers */
extern uint8_t ga_Buffer_Link_Array[]; /* link array for queue management*/

/**************************** Public Constant Declaration ********************/

/** @brief  Interface to access m_SMALL_BUFFER_SIZE_c from lib */
extern const uint8_t g_Small_Buffer_Size_c;

/** @brief  Interface to access m_LARGE_BUFFER_SIZE_c from lib */
extern const uint8_t g_Large_Buffer_Size_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_SMALL_BUFFERS_c from lib */
extern const uint8_t g_Total_Number_Of_Small_Buffers_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_LARGE_BUFFERS_c from lib */
extern const uint8_t g_Total_Number_Of_Large_Buffers_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_BUFFERS_c from lib */
extern const uint8_t g_Total_Number_Of_Buffers_c;

/** @brief Constant holding the total number of ACL entries in the ACL
 Descriptor Table */
extern const uint8_t g_MAC_ACL_Descriptor_Size_c;

/** @brief Structure used for holding information about PSM data*/
typedef struct PSM_Info_Tag {
    /*! The offset in NVM to the starting location of the data */
    uint32_t nvm_offset;

    /*! The pointer to the location of the data in RAM */
    uint8_t* p_data_location;

    /*! The size of the data in bytes */
    uint32_t data_size;
} __attribute__((__packed__))PSM_Info_t;

/******************************************************************************/

extern const uint8_t g_Number_Of_Mac_Timers_c;
extern const uint8_t g_Number_Of_App_Timers_c;
extern const uint8_t g_Total_Number_Of_Timers_c;
extern const uint8_t g_Number_Of_Fast_Timers_c;
extern const uint8_t g_PSM_Info_Entries_c;

/******************************************************************************/
/**
 * @brief Get the persistent storage table pointer
 *
 * @param - None
 * @return - pointer of the persistent storage table
 */
PSM_Info_t* stkConfig_getPSMTablePtr ( void );

/******************************************************************************/
#endif  /*MAC_STACK_CONFIGURATION_H */
