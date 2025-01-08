#ifndef _CONFIGURATIONTOOLDEVICE_H_
#define _CONFIGURATIONTOOLDEVICE_H_

#include "ZCL_Foundation.h"

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

void Handle_ZCL_Message( uint8_t buffer_index, APSDE_Data_Indication_t *p_buffer );


#endif /* _ZCL_TEST_APP_H_*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
