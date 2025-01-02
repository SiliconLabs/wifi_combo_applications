#ifndef _WINDOWSCOVERINGUTILITIES_H_
#define _WINDOWSCOVERINGUTILITIES_H_

#if ( g_WINDOW_COVERING_CLUSTER_ENABLE_d == 1 )
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "ZCL_Interface.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_LIFT_c  0x00
#define g_TILT_c  0x01

#define g_UP_OPEN_CMD_c    0x00
#define g_DOWN_CLOSE_CMD_c 0x01

#define m_CHANGE_UP_BY_ONE_c	1
#define m_CHANGE_DOWN_BY_ONE_c    -1


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct WindowsCoveringTable_Tag
{
   uint8_t Set_Point_Type;
   uint8_t Set_Point_Index;
   uint16_t Set_Point_Value;
}__attribute__((__packed__))WindowsCoveringTable_t;


WindowsCoveringTable_t windows_covering_table[3] = {{0xFF, 0xFF, 0xFFFF},
                                                          {0xFF, 0xFF, 0xFFFF},
                                                          {0xFF, 0xFF, 0xFFFF}};


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                      /* g_WINDOW_COVERING_CLUSTER_ENABLE_d */
#endif                      /* _WINDOWSCOVERINGUTILITIES_H_ */
