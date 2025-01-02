/******************************************************************************
* ZLL_Main.c
*******************************************************************************
* Program Description:
*
*
******************************************************************************/


/*****************************************************************************
* Includes
*****************************************************************************/
#include "ZigBeeDataTypes.h"
#include "buffer_management.h"
#include "ZCL_Functionality.h"
#include "stack_util.h"
#include "stack_common.h"

#include "memory_utility.h"
#include "app_configuration_default.h"
#include "ApplicationThinLayer.h"
#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZLL_defines.h"
#include "ZLL_Commissioning_Cluster.h"
#include "ZLL_Main.h"
#include "ZLL_Interpan.h"
#include "ZLL_Configuration.h"
#include "ZLL_Devices.h"
#include "ZLL_Network.h"

#ifdef g_ZLL_LIGHT_CONTROLLER_c
#include "ZLL_Controller_Application_Framework.h"
#endif /*g_ZLL_LIGHT_CONTROLLER_c */

#ifdef g_ZLL_LIGHT_DEVICE_c
#include "ZLL_Light_Application_Framework.h"
#endif /* g_ZLL_LIGHT_DEVICE_c*/

#endif

#include "apsme_interface.h"

/*****************************************************************************
* Global Constants
*****************************************************************************/


/*****************************************************************************
* Public Memory declarations
*****************************************************************************/



/* Holds information about initiator*/
static touchLinkInitiatorSection_t touchLinkInitiatiorSectionMemory;
touchLinkInitiatorSection_t *pTouchLinkInitiatiorSectionMemory =
    &touchLinkInitiatiorSectionMemory;

/* Holds information about target device*/
static touchLinkReceiverSectionType_t touchLinkReceiverSectionMemory;
touchLinkReceiverSectionType_t *pTouchLinkReceiverSectionMemory =
    &touchLinkReceiverSectionMemory;

/* Holds information about device context*/
static deviceStartupSection_t deviceStartupSectionMemory;
deviceStartupSection_t * pDeviceStartupSectionMemory =
    &deviceStartupSectionMemory ;




/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/
/* None */
/*****************************************************************************
* Public Functions
*****************************************************************************/
void ZLL_ResetStartUpParmas(void)
{
  uint8_t index = 0x00;
  uint8_t extpanid[8] = g_ZLL_CONFIGURE_NETWORK_START_EXTENDED_PANID_c;
  deviceStartupSectionMemory.activeScan_PANIDS[0] = g_ZLL_CONFIGURE_NETWORK_START_SHORT_PANID_c;

  memUtils_memCopy ((uint8_t*)&(deviceStartupSectionMemory.activeScan_extendedPANIDs[0][0]),
                     (extpanid),g_ZLL_EXTENDED_PANID_SIZE_c);

 pTouchLinkInitiatiorSectionMemory->identify_duration = g_ZLL_DEFAULT_IDENTIFY_TIME_c;
 pTouchLinkInitiatiorSectionMemory->scan_response_stored_last_index = 0x00;
 memUtils_memSet( pTouchLinkInitiatiorSectionMemory->Target_long_address, 0x00, g_EXTENDED_ADDRESS_LENGTH_c);

  /*parse the scan responses and send out the touch link commands*/
  for( index = 0x00; index < g_MAX_SCAN_RESPONSE_RESULTS_c; index++)
  {
    pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].scan_entryValid = g_FALSE_c;
  }
  pTouchLinkInitiatiorSectionMemory->scan_response_active_index = 0x00;

}


/*****************************************************************************/
void ZLL_AddSubDevicesInfo(void )
{
	/* This function will add the sub devices info of a zigbee node*/
	uint8_t loop_index = 0x00;
	uint8_t loop_index_2 = 0x00;
	uint8_t a[8];
	ZigBeeGetSelfIEEEAddress(a);
	for(;(loop_index <= g_MAX_NO_OF_ENDPOINTS_c+2) ;loop_index++){

		if(gp_Endpoint_Descriptors[loop_index].endpoint_id == g_DUMMY_ENDPOINT_ID_c){
			continue;
		}

        if(loop_index_2 < g_ZLL_CONFIG_SUB_DEVICES_NUMBER_c){
			/* Sub device Id */
			ZLL_device_information_table[loop_index_2].device_identifier =
				gp_Endpoint_Descriptors[loop_index].p_simple_desc->app_device_id;

			/* ZLL device version*/
			ZLL_device_information_table[loop_index_2].device_version_t.device_version =
				gp_Endpoint_Descriptors[loop_index].p_simple_desc->app_device_version;

			/* End point id of sub device */
			ZLL_device_information_table[loop_index_2].endpoint_identifier =
				gp_Endpoint_Descriptors[loop_index].endpoint_id;
			/* IEEE_address of the node*/
			memUtils_memCopy((uint8_t*)&(ZLL_device_information_table[loop_index_2].IEEE_address[0]),
							 a,g_EXTENDED_ADDRESS_LENGTH_c);

			/* TO DO , no of group id max is 4 ,
			But it is again appl configurable*/
			ZLL_device_information_table[loop_index_2].number_of_groups_identifiers =
				g_ZLL_NUMBER_OF_GROUPS_IDENTIFIER_c;
			/* ZLL profile Id*/
			ZLL_device_information_table[loop_index_2].profile_identifier =
				g_ZLL_PROFILE_ID_c;
			/* Default group id's are sorted in ascending order*/
			ZLL_device_information_table[loop_index_2].sort_tag = g_ZLL_SORT_TAG;

			loop_index_2++;
		}
	}
}


void UpdateSubDeviceExtendedAddress(uint8_t *pExtendedAddr)
{
	uint8_t loop_index_2 = 0x00;
	uint8_t a[8];
	ZigBeeGetSelfIEEEAddress(a);
	for(;loop_index_2 < g_ZLL_CONFIG_SUB_DEVICES_NUMBER_c;loop_index_2++){
		/* IEEE_address of the node*/
		memUtils_memCopy((uint8_t*)&(ZLL_device_information_table[loop_index_2].IEEE_address[0]),
						 a,g_EXTENDED_ADDRESS_LENGTH_c);

	}

}


/*****************************************************************************
* Private Functions
*****************************************************************************/

/* None */

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */




/*****************************************************************************
* End Of File
*****************************************************************************/
