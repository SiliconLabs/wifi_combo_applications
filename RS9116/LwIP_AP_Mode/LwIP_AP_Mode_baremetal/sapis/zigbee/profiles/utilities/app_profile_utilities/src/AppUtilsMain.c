/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

#include "ZigBeeDataTypes.h"
//#include "stack_common.h"
#include "AppTestHarness.h"

#include "zdo_interface.h"
#include "apsde_interface.h"
#include "Stack_Info.h"

#include "queue.h"
//#include "sw_timer.h"
#include "buffer_management.h"
#include "mac_interface.h"
#include "ZCL_Interface.h"
#include "ApplicationThinLayer.h"
//#include "FragmentationUtility.h"
#include "stack_functionality.h"
//#include "usb_thin_layer.h"
//#include "scheduler.h"
//#include "zb_hil_intf.h"

//#include "stack_events.h"
//#include "ApptimerId.h"
#include "app_configuration_default.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* Macro that holds the baud rate of UART */
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */


/*-----------------------------------------------------------------------------
* Function Prototypes
*---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
#if 0
int main() __attribute__((section(".zb_tcm_code")));
int main()
{
	Test_Harness_Data_Path = App_Handle_Data_Request;
	Test_Harness_Management_Path = Application_to_ZDO;
	Test_Harness_To_Test_Utility = App_Stack_Information;

	TH3_main();

	zigbee_hw_init();

	/* Timer Initialization */
	TMR_Init();

	/* Buffer Management Initialization */
	buffMgmt_init();

	Init_QueuePtr(get_USBDataQueuePtr(), 4);

	MAC_mainMACInit();
	/* Allows runtime inclusion of required Features such as Frequency agility
	etc */
	InitStack(STK_NEED_FREQ_AGILITY|STK_NEED_FRAGMENTATION|STK_NEED_BINDING|
			  STK_NEED_SOURCE_ROUTE);
	InitStackTables();
	ZDOinitSTK();
	AppFragmentAllocateSpace();
	App_Init();

	StartSchedulerThread3();

	return 0;
}

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */
#endif
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
