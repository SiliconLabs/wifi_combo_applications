/** @file stack_configuration.c
 *
 * @brief Initializes various variables that can be used to configure MAC.
 *
 * This file contains definitions for the various buffer sizes, arrays used by
 * buffer management and timer module, and other varibles whose values affect
 * stack behaviour and can be configured by the user.
 */

/**************************** Includes ***************************************/

#include "ZigBeeDataTypes.h"
#include "common_includes.h"
#include "mac_stack_configuration.h"
#include "memory_utility.h"


/* MAC and LMAC PIB include files */
//#if ( g_HIGHEST_LAYER_d >= g_MAC_LAYER_c )

#include "buffer_management.h"
#include "queue.h"
//#include "mac_interface.h"


//#endif /* g_HIGHEST_LAYER_d >= g_MAC_LAYER_c */

/************************************ Externals *****************************/

/* None */

/**************************** Private Macro Definitions **********************/


/**************************** Private type Definitions ***********************/
/* None */

/**************************** Private Enum Definitions ***********************/

/* None */

/**************************** Private Variable Definitions *******************/
/* MAC and LMAC PIBs */




/**************************** Public Variable Definitions ********************/

/**  @brief  Common Buffer pool holding the buffers */
uint8 ga_Heap[(g_TOTAL_NUMBER_OF_SMALL_BUFFERS_c * g_SMALL_BUFFER_SIZE_c) +
(g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c * g_LARGE_BUFFER_SIZE_c)];

/** @brief  buffer link array for queue management */
uint8_t ga_Buffer_Link_Array[g_TOTAL_NUMBER_OF_BUFFERS_c];

/*-------------------- Persistent Storage Related Variables ------------------*/


/**************************** Private Constant Definitions *******************/

/* None */

/**************************** Public Constant Definitions ********************/

/** @brief  Interface to access m_SMALL_BUFFER_SIZE_c from lib */
const uint8_t g_Small_Buffer_Size_c = g_SMALL_BUFFER_SIZE_c;

/** @brief  Interface to access m_LARGE_BUFFER_SIZE_c from lib */
const uint8_t g_Large_Buffer_Size_c = g_LARGE_BUFFER_SIZE_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_SMALL_BUFFERS_c from lib */
const uint8_t g_Total_Number_Of_Small_Buffers_c =
        g_TOTAL_NUMBER_OF_SMALL_BUFFERS_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_LARGE_BUFFERS_c from lib */
const uint8_t g_Total_Number_Of_Large_Buffers_c =
        g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_BUFFERS_c from lib */
const uint8_t g_Total_Number_Of_Buffers_c = g_TOTAL_NUMBER_OF_BUFFERS_c;

/** @brief  The number of timers internally used by the MAC */
const uint8_t g_Number_Of_Mac_Timers_c = 8;

/** @brief  The number of timers used by the application running over the MAC */
const uint8_t g_Number_Of_App_Timers_c = 8;

/** @brief  Total number of timers provided by the timer module */
const uint8_t g_Total_Number_Of_Timers_c = g_TOT_NO_OF_TIMERS_c;

/** @brief  Number of fast response timers provided by the timer module */
const uint8_t g_Number_Of_Fast_Timers_c = g_NO_OF_FAST_TIMERS_c;

/** @brief Constant holding the number of tables stored in the PSM */
const uint8_t g_PSM_Info_Entries_c = g_PSM_INFO_ENTRIES_c;

/** @brief Constant holding the total number of ACL entries in the ACL
 Descriptor Table */
const uint8_t g_MAC_ACL_Descriptor_Size_c = g_MAX_ACL_TABLE_ENTRIES_c;

/**************************** Private Function Prototypes ********************/

/* None */



/******************************************************************************/



/**************************** Private Function Definitions *******************/

/* None */

/******************************************************************************/
