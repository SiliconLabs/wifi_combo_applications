#ifndef _STACK_INFO_H_
#define _STACK_INFO_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

#include "ZigBeeDataTypes.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/* These enums are used when propierietary status values are obtained by
   placing a request. Info like Number of free buffers available, nwk managment
   current state, nwk routing current state can be obtained by doing a get
   request with these enums.*/
enum
{
   g_Number_Of_freeBuffers_Available_c   = 0xa6,
   g_Get_MAC_Extended_Address_c           = 0x6f,
 };


/* Structure - get free buffer request - to get a free buffer */
typedef struct Get_Stack_Info_Request_t
{
   /* msg_type = Request */
   uint8_t msg_type;
   uint8_t attribute_id;
}__attribute__((__packed__))Get_Stack_Info_Request_t;


/* Structure - get free buffer confirm - to get a free buffer */
typedef struct Get_Stack_Info_Confirm_t
{
      /* msg_type = confirm*/
   uint8_t msg_type;
   uint8_t attribute_id;
   uint8_t attribute_value[1];
}__attribute__((__packed__))Get_Stack_Info_Confirm_t;


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/






/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

#endif                                 /* _STACK_INFO_H_ */
