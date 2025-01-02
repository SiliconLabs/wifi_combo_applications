#ifndef _APP_TEST_HARNESS_H_
#define _APP_TEST_HARNESS_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"

/*-----------------------------------------------------------------------------
* global Constants
*----------------------------------------------------------------------------*/
#define m_DEVICE_DISCOVERY_START_ID_c                    0xC0
#define m_QUEUE_SIZE_c	                                 0x08
#define g_MAX_ZDP_LENGTH_c                               0x64
#define m_SUCCESS_c                                      0x00
#define m_MAC_SET_CONFIRM_c                              0x13
#define m_RESET_REQUEST_c                                0x10
#define m_SYS_SET_REQUEST_c              		         0x12
#define m_START_DEVICE_RSP_c              		         0x17
#define m_PERMIT_JOIN_CONF_c              		         0x8A
#define m_SHORT_ADDRESS_MODE_c                           0x02
#define m_DEFAULT_SIZE_ZDO_CONFIGURATION_c               0x80
#define m_FAST_POLL_DURATION_c 					         0x03E8
#define m_ZCL_SIMPLE_DESCRIPTOR_REQUEST_c                0xC8
#define m_ZCL_MATCH_DESCRIPTOR_REQUEST_c                 0xCC
#define m_ZCL_END_DEVICE_BIND_RESPONSE_c                 0xD7
#define m_ZCL_SIMPLE_DESCRIPTOR_RESPONSE_c               0xC9
#define m_ZCL_MATCH_DESCRIPTOR_RESPONSE_c                0xCD
#define m_SET_TARGET_ADDRESS_c                           0x56
#define m_SET_REPORTING_TARGET_INFO_c                    0x70
#define m_LENGTH_SET_SONFIRM_c                           0x03
#define m_NTS_SYNC_CONFIRM_c                             0x58
#define m_ADD_ZONE_ADDRESS_REQUEST_c                     0x63
#define m_ADD_ZONE_ADDRESS_CONFIRM_c                     0x64
#define m_SET_CONFIRM_c                                  0x13
#define m_LOCAL_WRITE_ATTRIBUTE_REQUEST_c                0x60
#define m_LOCAL_WRITE_ATTRIBUTE_CONFIRM_c                0x61
#define m_UPDATE_SAS_REQUEST_c                           0x65
#define m_NVM_SAVE_ZDO_REQUEST_c                         0x67
#define m_NVM_RESTORE_DEFAULT_SAS_REQUEST_c              0x69
#define g_ACTIVATE_SAS_INDEX_c                           0x5B
#define g_SET_CERTIFICATE_c                              0x84
#define g_SET_CA_PUBLIC_KEY_c                            0x85
#define g_SET_STATIC_PRIVATE_KEY_c                       0x86
#define m_ZCL_BIND_REQUEST_c                			 0xD8
#define m_ZCL_BIND_RESPONSE_c                			 0xD9





/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/


enum
{
    g_Add_Device_To_Black_List_c                    = 0x04,
    g_Remove_Device_To_Black_List_c                 = 0x05,
    g_Restart_Dev_c									= 0x06,
    g_Write_Reportable_Attribute_c                  = 0x07,
    g_Write_Reportable_Attribute_Confirmation_c,
    g_System_Reset_Confirm_c                        = 0x13,
    g_Start_Dev_c                                   = 0x16,
    g_Write_Dest_Address_c                          = 0x56,
    g_Application_Poll_Request_c,
    g_Set_Index_For_Table_c                         = 0x59,
    g_Set_Index_For_Table_Confirm_c,
    g_Activate_SAS_Index_c,
    g_NVM_Save_SAS_Request_c                        = 0x65,
    g_NVM_Save_SAS_Confirm_c,
    g_NVM_Save_ZDO_Request_c,
    g_NVM_Save_ZDO_Confirm_c,
    g_NVM_Restore_Default_SAS_Request_c,
    g_NVM_Restore_Default_SAS_Confirm_c,
    g_NVM_Save_Request_c ,
    g_NVM_Save_Confirm_c,
    g_NVM_Restore_Request_c,
    g_NVM_Restore_Confirm_c,
    g_MAC_Extended_Address_c						= 0x6F,
    g_Initiate_Fast_Poll_c                          = 0x77,
    g_Disable_RT_Ageing_c                           = 0xBF,
    g_Disable_NT_Ageing_c                           = 0xDF,
    g_setRejectDevice                               = 0xE2,
    g_Permit_Join_Request_c                         = 0x75,
    g_setPriceEvent_c                               = 0x76,
    g_setDRLCEvent_c                                = 0x77,
    g_set_MessageEvent_c                            = 0x78,
    g_DRLCOptInOrOptOut_c                           = 0x79,
    g_set_InterPanData_c                            = 0x80,
    g_setSystemTime_c                               = 0x81,
    g_setSystemUTCTime_c                            = 0x82,
    g_getSystemUTCTime_c                            = 0x83,
    g_Set_EndPoint_c                                = 0x84,
    g_set_certificate_c                             = 0x85,
    g_set_ca_public_key_c                           = 0x86,
    g_set_static_private_key_c                      = 0x87,
    g_setBlockPeriodEvent_c                         = 0x88,
    g_get_certificate_c                             = 0x95,
    g_get_ca_public_key_c                           = 0x96,
    g_get_static_private_key_c                      = 0x97,
    g_get_NWK_Extended_Pan_Id_c						          = 0x9A,
    g_set_NWK_Extended_Pan_Id_c						          = 0x9B,
    g_set_trust_center_link_keys_c					        = 0x9C,
    g_get_trust_center_link_keys_c					        = 0x9D,
    g_set_nwk_key_c									                = 0x9E,
    g_ApplianceStaticClusterEvent_c                 = 0x5C,
    g_ApplianceEventsAndAlertEvent_c                = 0x5d,
    g_PowerProfileClusterEvent_c                    = 0x5e,
    g_PollControlSetPollRateEvent_c                 = 0x64,
    g_Issuer_Address_c								              = 0x31,
    g_Corrupt_Certificate_c							            = 0x32,
    g_read_Single_Byte_Of_Certificate_c				      = 0x33,
    g_Write_Single_Byte_Of_Certificate_c			      = 0x34,
    /* Following macro is just for testing interpan command*/
    g_ZLL_Trigger_Touch_link_Procedure							= 0x35,
    g_ZLL_Target_Set_Priority								        = 0x36,
    g_ZLL_Security_Set_KeyIndex						          = 0x37,
    g_ZLL_Reset_Target_Device                       = 0x38,
    g_ZLL_Set_Identify_Duration                     = 0x39,
    g_ZLL_Stop_Identify_Mode                        = 0x40,
    g_ZLL_Trigger_Utility_Notification_c            = 0x41
};

enum
{
   g_Data_Request_c =           0xA0,
   g_Data_Confirn_c =           0xA1,
   //g_Data_Indication_c =        0xA2,
   g_InterPan_Data_Request_c =  0xA3,
   g_InterPan_Data_Confirn_c =  0xA4,
   g_InterPan_Data_Indication_c=0xA5,
   g_Indirect_Request_c =       0xF0,
   g_Direct_Request_c =         0xF3,
   g_Group_Request_c =          0xF9

};

/* application over test harness has to use the following function pointer.
Test harness invokes this function pointer when it gets data from UART
and if primitive type of message is in the specified range. */
typedef void (*pf_Test_Harness_To_Application)
(
   uint8_t primitive_id,
   uint8_t buffer_index
);

/* Application function pointers   */

/* function pointer to give data to NLLE that is TestProfile */
extern pf_Test_Harness_To_Application Test_Harness_Data_Path;

/* function pointer to give management data to ZDO */
extern pf_Test_Harness_To_Application Test_Harness_Management_Path;

/* Test_Harness_To_Test_Application is invoked irrespective of primitive id
of message received  */
extern pf_Test_Harness_To_Application Test_Harness_To_Test_Application;

/* Test_Harness_To_NLME is invoked when primitive id of message received
from UART is 0x14 */
extern pf_Test_Harness_To_Application Test_Harness_To_Test_Utility;

/* function pointer to allocate memory to test memory starvation */
extern pf_Test_Harness_To_Application Test_Harness_Set_System_Config;



/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

extern uint8_t g_Length_Of_Primitive;

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

void App_Handle_Data_Request ( uint8_t msgId, uint8_t msg_index );


void App_Reset( void );
void Test_Main( void );
/*-----------------------------------------------------------------------------
* Application_to_ZDO
*------------------------------------------------------------------------------
*
* Return Value :
*			void - No return value
* Input Parameters:
*	msg_type - gives the type of the message/management id of the message to be
*					queued
*	msg_index - gives the actual management data sent from the test tool that
*		needs to be processed.
*
* Output Parameters:
*		None
* Scope of the function:
*
*
* Purpose of the function:
*
*
* Before the function is called:
*     None
* After the function is called:
*     None
*
*----------------------------------------------------------------------------*/

void Application_to_ZDO( uint8_t msg_type, uint8_t msg_index );

/*-----------------------------------------------------------------------------
* testHrns_passMessageDown
*------------------------------------------------------------------------------
*
* Return Value:
*     NONE
*
* Parameters:
* Input Parameters:
*     buffer_index - index to buffer containing data.
*
* Output Parameters:
*     NONE
*
* Scope of the function:
*     This function can be called only from test harness.
*
* Purpose of the function:
*     This function is called from UART ISR when data is received. This
*     function removes length from buffer and hands over buffer to corresponding
*     layer based on primitive id
*
* Before the function is called:
*     NONE
*
* After the function is called:
*     NONE
*
*----------------------------------------------------------------------------*/

void testHrns_passMessageDown ( uint8_t buffer_index );


/*-----------------------------------------------------------------------------
* testHrns_passMessageUp
*------------------------------------------------------------------------------
*
* Return Value:
*     NONE
*
* Parameters:
* Input Parameters:
*     buffer_index - index to buffer containing data.
*     length       - length of the frame
*
* Output Parameters:
*     NONE
*
* Scope of the function:
*     This function can be called from any module.
*
* Purpose of the function:
*     This function is called from different tasks when data to be
*     transmitted to the Test harness.This function adds up length to buffer
*     and hands over buffer to UART
*
* Before the function is called:
*     NONE
*
* After the function is called:
*     NONE
*
*----------------------------------------------------------------------------*/

void testHrns_passMessageUp (uint8_t bufferIndex, uint8_t length);

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

#endif                                 /* _APP_TEST_HARNESS_H_ */