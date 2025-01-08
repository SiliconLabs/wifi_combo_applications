/*******************************************************************************
 ZLL_State_Machine .c
********************************************************************************
* Program Description:
*
* ZLL controller device
*
********************************************************************************
* Includes
*******************************************************************************/
//#include "stm32l1w_discovery.h"
#include "common_includes.h"
#include <stdbool.h>
#include "ZLL_State_Machine.h"
#include "ZLL_Interpan.h"
#include "ZLL_Network.h"
#include "ZLL_Main.h"
#include "stack_functionality.h"
#include "stack_common.h"
#include "zdo_interface.h"
//#include "timer.h"
#include "ApptimerId.h"
#include "mac_interface.h"
//#include "mmac_task.h"
#include "app_configuration_default.h"
#include "memory_utility.h"
#include "ZLL_NVM_Utilities.h"
#include"sw_timer.h"
//#include "stack_events.h"


/*******************************************************************************
* Global Constants
*******************************************************************************/

#ifdef g_ZLL_LIGHT_CONTROLLER_c

//uint8_t ZLL_Controller_State = g_I_INITIAL_STATE_c;

/*Initiator will */
bool TouchLinkCommissioningTriggered = g_FALSE_c;
/* This variable is used to decide to initiate reset to factory new for target
device*/
bool ResetTargetDevice = g_FALSE_c;
extern uint8_t ZLLScanChannelListCount[g_ZLL_PRIMARY_CHANNELS_NUMBER_c+
	g_ZLL_SECONDARY_CHANNELS_NUMBER_c];
uint32_t ZLL_aplcMinStartupDelaytime = 0x00000000;
uint32_t ZLL_aplcResettime = 0x00000000 ;

#endif /*#ifdef g_ZLL_LIGHT_CONTROLLER_c*/

bool TouchLinkProc_InProgress = g_FALSE_c;


#ifdef g_ZLL_LIGHT_DEVICE_c

bool ZLL_Light_Target_Priority = g_FALSE_c;
//uint8_t ZLL_Light_State = g_T_INITIAL_STATE_c;

#endif //#ifdef g_ZLL_LIGHT_DEVICE_c

uint8_t ZLL_SecurityKeyIndex = 0x00;
uint8_t ZLL_State;

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/



#ifdef g_ZLL_LIGHT_CONTROLLER_c
void ZLL_HandleControllerStateMachine( void )
{

  static uint32_t scanStartTime = 0x00000000;
  uint8_t aIEEEaddress[8] = { 0x00 };
  uint32_t currentTime = Get_Current_Time();
  if( TouchLinkCommissioningTriggered )
  {
    if( ( scanStartTime ) && ( ( currentTime - scanStartTime ) >=
                               (g_APP_SCAN_TIME_BASE_DURATION_c * m_MILLISECONDS_c )))
    {

      /*If the below index is non-zero, it conveys that scan responses are received
      and stored.*/
      if(  pTouchLinkInitiatiorSectionMemory->scan_response_stored_last_index )
      {
        /*Target address is NULL and the controller state is not in ready state*/
        if( (g_Memory_Matches_c == (memUtils_memCompare(
            pTouchLinkInitiatiorSectionMemory->Target_long_address,
            aIEEEaddress,
            g_EXTENDED_ADDRESS_LENGTH_c))) &&
           ( ZLL_NVM_Parameters.controller_device_state <
              g_I_DEVICE_READY_STATE_c ))
        {
          if( pTouchLinkInitiatiorSectionMemory->scan_response_stored[0 ].scan_entryValid == g_TRUE_c )
          /*The device has not yet started the network*/
          /*copy the target IEEE address*/
          memUtils_memCopy(pTouchLinkInitiatiorSectionMemory->Target_long_address,
              pTouchLinkInitiatiorSectionMemory->scan_response_stored[0].
                src_long_address,g_EXTENDED_ADDRESS_LENGTH_c);
        }
        pTouchLinkInitiatiorSectionMemory->scan_response_stored[0].scan_entryValid
          = g_FALSE_c;
        ZLL_Change_Controller_State(g_I_DEVICE_IDENTIFY_REQUEST_STATE_c);
      }
      else
      {
        ZLL_Change_Controller_State(g_I_DEVICE_DISCOVERY_STATE_c);
      }
    }
    if((ZLL_aplcMinStartupDelaytime)&&
       ((currentTime - ZLL_aplcMinStartupDelaytime) >=
        ( (g_APP_MIN_STARTUP_DELAY_TIME_c )* m_MILLISECONDS_c ))){
        //if(g_TRUE_c == ZigBeeStackIsUp()){
        if(g_TRUE_c == rsi_zigb_stack_is_up()){
          /* Move to Ready state, to parse next scan response*/
            ZLL_Change_Controller_State(g_I_DEVICE_READY_STATE_c);
        }else{
          /* Move to rejoin state*/
          ZLL_Change_Controller_State(g_I_DEVICE_REJOIN_STATE_c);
        }
        ZLL_aplcMinStartupDelaytime = 0;
    }
  }
  switch(ZLL_NVM_Parameters.controller_device_state)
  {
    case g_I_INITIAL_STATE_c:
      /*If touch link commissioning is triggered due to external button press
      and if the device is factory new, set the state to device discovery state to
      trigger scan request command.*/

      if( ( TouchLinkCommissioningTriggered ) && ( ZLL_IsFactoryNew() ==  g_TRUE_c ))
      {
        TouchLinkProc_InProgress = g_TRUE_c;

        Set_RX_On_When_Idle();

        tmr_startRelativeTimer
        (
          m_APP_ZLL_INTERPAN_LIFETIME_TIMER_c,
          (g_APP_INTERPAN_TRANCSID_LIFETIME_c * m_MILLISECONDS_c),
          App_ZLL_InterPAN_Life_Timeout_Callback );
          /* Every time it comes to initial state , uses fresh interpan id*/
          pTouchLinkInitiatiorSectionMemory->interPAN_transaction_identifier =
          randomT();

        ZLL_InitNwkAddress();
        ZLL_SetInitiator();
        ZLL_Change_Controller_State(g_I_DEVICE_DISCOVERY_STATE_c);
      }
      break;

    case g_I_DEVICE_DISCOVERY_STATE_c:
      if( ZLL_IsInitiator() == g_TRUE_c ){
        /*Send out a scan request to discover devices in te vicinity*/
        if( TouchLinkProc_InProgress == g_TRUE_c )
        {
          /*verify the channel to send scan request based on the scan retry in
          each channel*/
          ZLL_Nwk_Verify_Valid_Channel();
          scanStartTime = Get_Current_Time();
          ZLL_Create_Scan_Request_Command();
          /*The state can transit in function ZLL_Nwk_Verify_Valid_Channel()
          if maximum channel count is reached. hence this check is required*/
          if( ZLL_Read_Controller_State() == g_I_DEVICE_DISCOVERY_STATE_c )
          {
            ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);
          }
        }
      }
      break;

    case g_I_DEVICE_IDENTIFY_REQUEST_STATE_c:
      if( ZLL_IsInitiator() == g_TRUE_c ){
        uint8_t index;
        if( TouchLinkProc_InProgress == g_TRUE_c )
        {
          scanStartTime = 0x00000000;
          /*create device identify request to the target device*/
          ZLL_Create_Identify_Request( pTouchLinkInitiatiorSectionMemory->Target_long_address );
          /* check ,whether the device information request need to be initiated
          or not based on the sub devices count in scan response*/
          index = ZLL_ReadScanReponseForTarget( pTouchLinkInitiatiorSectionMemory->Target_long_address );
          /*if the number of subdevices in the scan response is more than 1, only then initiate
          device information request, otherwise send directly Network start request*/
          if(pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].scan_response.no_of_subdevices > 0x01  )
          {
            /* move to device information request state ,later move to network
            start state*/
            ZLL_Change_Controller_State(g_I_DEVICE_INFORMATION_REQUEST_STATE_c);
          }
          else //if(pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].scan_response.no_of_subdevices == 0x01)
          {

            ZLL_Change_Controller_State(g_I_DEVICE_NETWORK_START_STATE_c);
          }

        }
      }
      break;


    case g_I_DEVICE_INFORMATION_REQUEST_STATE_c:
      if( ZLL_IsInitiator() == g_TRUE_c ){
        if( TouchLinkProc_InProgress == g_TRUE_c )
        {
          scanStartTime = 0x00000000;
          /*create device information request to the target device*/
          ZLL_Create_Device_Information_Request_Command( pTouchLinkInitiatiorSectionMemory->Target_long_address );
          ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);
        }
      }
      break;

    case g_I_DEVICE_NETWORK_START_STATE_c:
      if( ZLL_IsInitiator() == g_TRUE_c ){
        uint8_t index;
        if( TouchLinkProc_InProgress == g_TRUE_c )
        {
          uint8_t extended_panId[8] = g_ZLL_CONFIGURE_NETWORK_START_EXTENDED_PANID_c;

          scanStartTime = 0x00000000;
          memUtils_memCopy(pTouchLinkInitiatiorSectionMemory->Extended_PANId,
                           extended_panId,g_EXTENDED_PAN_ID_LENGTH_c);

          pTouchLinkInitiatiorSectionMemory->Short_PanId = g_ZLL_CONFIGURE_NETWORK_START_SHORT_PANID_c;
          index = ZLL_ReadScanReponseForTarget( pTouchLinkInitiatiorSectionMemory->Target_long_address);
          if( index != g_FAILURE_c)
          {
            pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].scan_entryValid = g_FALSE_c;
          }
          /* If reset to factory new is set ,then issue reset to factory new to
          the target device ,before issueing the network start request*/
          if(ResetTargetDevice){
              ZLL_Create_Reset_To_Factor_New_Request(
                pTouchLinkInitiatiorSectionMemory->Target_long_address );
              ResetTargetDevice = g_FALSE_c;
              ZLL_aplcResettime = Get_Current_Time();
              ZLL_Change_Controller_State(g_I_WAIT_FOR_NEXT_COMMAND_STATE_c);

          }
          else
          {
              ZLL_aplcResettime = 0;
              ZLL_Create_Network_Start_Request( pTouchLinkInitiatiorSectionMemory->Target_long_address );
              ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);

          }

        }
      }
      break;


    case g_I_DEVICE_REJOIN_STATE_c:
      /*Touch link commissioning is almost completed*/

      {
        //uint8_t status  = ZigBeeRejoinNetwork ( g_FALSE_c );
        uint8_t status  = rsi_zigb_rejoin_network ( g_FALSE_c );
        if( status != ZigBee_Success )
        {
          /*check if the InterPAN Frame is expired, if exired move to initial state and
          wait for the trigger. If interpan timer is not expired, then
          move to device discovery state*/
          if( ZLL_IsInitiator() == g_TRUE_c )
          {
            if( TouchLinkProc_InProgress )
              ZLL_Change_Controller_State(g_I_DEVICE_DISCOVERY_STATE_c);
            else
              ZLL_Change_Controller_State(g_I_NETWORK_JOIN_STATE_c);
          }
          else /*if not initiator, the controller should move wait for response state*/
            ZLL_Change_Controller_State(g_I_INITIAL_STATE_c);
        }
        else
        {
          ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);
        }
      }
      break;


    case g_I_NETWORK_JOIN_STATE_c:
      if( ZLL_IsInitiator() == g_TRUE_c )
      {
        uint8_t logical_type = 0xff;

        logical_type = pTouchLinkInitiatiorSectionMemory->scan_response_stored[pTouchLinkInitiatiorSectionMemory->scan_response_active_index].
          scan_response.ZigBee_information_t.logical_type ;


        if(logical_type == g_ZLL_LOGICAL_TYPE_ROUTER_c){
          logical_type = g_ZLL_Network_Join_Router_Req_Command_c;

        }
        else if(logical_type == g_ZLL_LOGICAL_TYPE_END_DEVICE_c ){

          logical_type = g_ZLL_Network_Join_End_Device_Req_Command_c;
        }

        ZLL_Create_Network_Join_Request(
             pTouchLinkInitiatiorSectionMemory->scan_response_stored[pTouchLinkInitiatiorSectionMemory->scan_response_active_index].src_long_address,
              logical_type );
        /* reset the entry to false */
        pTouchLinkInitiatiorSectionMemory->scan_response_stored[pTouchLinkInitiatiorSectionMemory->scan_response_active_index].
        scan_entryValid = g_FALSE_c;
        ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);
      }
      break;

    case g_I_WAIT_FOR_RESPONSE_STATE_c:

        if( ZLL_IsInitiator() == g_FALSE_c ){
          if((ZLL_aplcMinStartupDelaytime)&&
            ((currentTime - ZLL_aplcMinStartupDelaytime) >=
            ( (g_APP_MIN_STARTUP_DELAY_TIME_c )* m_MILLISECONDS_c ))){
              /* changing the state to rejoin state*/
              ZLL_aplcMinStartupDelaytime = 0x00000000;
              ZLL_Change_Controller_State(g_I_DEVICE_REJOIN_STATE_c);
          }
        }
//        else {
//          if((ZLL_aplcMinStartupDelaytime)&&
//            ((currentTime - ZLL_aplcMinStartupDelaytime) >=
//             ( (g_APP_LEAVE_DURATION_TIME_c )* m_MILLISECONDS_c ))){
//                 ZLL_Change_Controller_State(g_I_DEVICE_NETWORK_START_STATE_c);
//             }
//        }

      break;

  case g_I_WAIT_FOR_NEXT_COMMAND_STATE_c:
    if( ZLL_IsInitiator() == g_TRUE_c ) {
      if((ZLL_aplcResettime)&&
         ((currentTime - ZLL_aplcResettime) >=
          ( (g_APP_LEAVE_DURATION_TIME_c )* m_MILLISECONDS_c ))){
            ZLL_Change_Controller_State(g_I_DEVICE_NETWORK_START_STATE_c);
            ZLL_aplcResettime = 0;
          }
    }
    break;

    case g_I_DEVICE_READY_STATE_c:
      /* Save the device state , and nwk address range in NVM*/
      if( ZLL_IsInitiator() == g_TRUE_c ){

        /*Loop through the scan response table and send network join command*/
        uint8_t index = ZLL_NextJoiningDevice();
        if( index != g_ZCL_Not_Found_c )
        {
            pTouchLinkInitiatiorSectionMemory->scan_response_active_index = index;

            ZLL_Change_Controller_State(g_I_NETWORK_JOIN_STATE_c);
        }
        else
        {
        	//Stack_Events_ClearEvent(g_ZLL_PROCESS_EVENT_c);
        }
      }
      break;


    default:
        break;

  }

}
#endif /*#ifdef g_ZLL_LIGHT_CONTROLLER_c*/


/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/*******************************************************************************
* Public Functions
*******************************************************************************/
#ifdef g_ZLL_LIGHT_CONTROLLER_c
void App_ZLL_InterPAN_Life_Timeout_Callback ( uint8_t timer_id )
{
  TouchLinkProc_InProgress = g_FALSE_c;

  ZLL_Change_Controller_State( g_I_INITIAL_STATE_c );
  TouchLinkCommissioningTriggered = g_FALSE_c;

}
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c
/******************************************************************************/
#ifdef g_ZLL_LIGHT_DEVICE_c
void App_ZLL_IdentifyCallBack( uint8_t timer_id )
{
 // STM_EVAL_LEDOff( LED3);
  /*Stop blinking the LED*/
}
#endif //#ifdef g_ZLL_LIGHT_DEVICE_c

/******************************************************************************/
void ZLL_Tick(void)
{
#ifdef g_ZLL_LIGHT_CONTROLLER_c
  ZLL_HandleControllerStateMachine();
#endif /*#ifdef g_ZLL_LIGHT_CONTROLLER_c*/
#ifdef g_ZLL_LIGHT_DEVICE_c
  ZLL_HandleDeviceStateMachine();
#endif /*#ifdef g_ZLL_LIGHT_DEVICE_c*/
  ZLL_SaveNVMParameters();
}
/******************************************************************************/

void ZLL_Init( void )
{
#ifdef ZIGBEE_ROUTER
   MAC_SetPhyTRXState(g_TRUE_c);
#endif
#ifdef ZIGBEE_END_DEVICE
   MAC_SetPhyTRXState(g_FALSE_c);
#endif
#ifdef g_ZLL_LIGHT_DEVICE_c
  // STM_EVAL_LEDInit( LED3 );
 //  STM_EVAL_LEDOff( LED3 );
   ZLL_Light_Target_Priority = g_FALSE_c;
#endif//#ifdef g_ZLL_LIGHT_DEVICE_c

#ifdef g_ZLL_LIGHT_CONTROLLER_c
   memUtils_memSet(ZLLScanChannelListCount,0x00,
		g_ZLL_PRIMARY_CHANNELS_NUMBER_c+g_ZLL_SECONDARY_CHANNELS_NUMBER_c);
   ZLL_NVM_Parameters.controller_device_state = g_I_INITIAL_STATE_c;
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c

   ZLL_NetworkSetchannel ( g_ZLL_STARTUP_LOGICAL_CHANNEL_c );
   ZLL_ResetStartUpParmas();
   ZLL_AddSubDevicesInfo();
   ZLL_SecurityKeyIndex = 0x00;

}

/******************************************************************************/
void ZLL_Reset( void )
{
  ZLL_NetworkSetchannel ( g_ZLL_STARTUP_LOGICAL_CHANNEL_c );
#ifdef g_ZLL_LIGHT_CONTROLLER_c
  ZLL_NVM_Parameters.controller_device_state = g_I_INITIAL_STATE_c;
  TouchLinkCommissioningTriggered = g_FALSE_c;
#endif /*#ifdef g_ZLL_LIGHT_CONTROLLER_c*/

 TouchLinkProc_InProgress = g_FALSE_c;
#ifdef g_ZLL_LIGHT_DEVICE_c
// ZLL_Light_State = g_T_INITIAL_STATE_c;
    ZLL_NVM_Parameters.light_device_state = g_T_INITIAL_STATE_c;
#endif //#ifdef g_ZLL_LIGHT_DEVICE_c
 ZLL_Init();

}
/******************************************************************************/
#ifdef g_ZLL_LIGHT_CONTROLLER_c
void ZLL_TriggerTouchLinkCommisioning ( void )
{
  TouchLinkCommissioningTriggered = g_TRUE_c;
  //Stack_Events_SetEvent(g_ZLL_PROCESS_EVENT_c);
}
/******************************************************************************/
void ZLL_ResetTargetDevice(bool set)
{
    ResetTargetDevice = set;
}

#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c

/******************************************************************************/
#ifdef g_ZLL_LIGHT_DEVICE_c
void ZLL_HandleDeviceStateMachine( void )
{
  switch(ZLL_NVM_Parameters.light_device_state )
  {
      case g_T_INITIAL_STATE_c:
        /*If touch link commissioning is triggered due to external button press
        and if the device is factory new, set the state to device discovery state to
        trigger scan request command.*/
        if( ZLL_IsFactoryNew() ==  g_TRUE_c )
        {
          //Stack_Events_SetEvent(g_ZLL_PROCESS_EVENT_c);
          ZLL_Change_LightDevice_State(g_T_WAIT_FOR_SCAN_REQUEST_STATE_c);
        }
        break;

      case g_T_WAIT_FOR_SCAN_REQUEST_STATE_c:
        break;

      case g_T_ACTIVE_SCAN_REQUEST_STATE_c:
          /*Initiate active scan request to choose unique pan id*/
        break;



      case g_T_NETWORK_START_STATE_c:
        ZLL_TriggerStartRequest();
        break;

      case g_T_INITIATE_ACTIVE_SCAN_REQ_c:
        rsi_zigb_initiate_scan( g_MAC_ACTIVE_SCAN_TYPE_c,( 1 << ZLL_NetworkGetchannel()), g_SCAN_DURATION_c);
        //ZigBeeInitiateScan( g_MAC_ACTIVE_SCAN_TYPE_c,( 1 << ZLL_NetworkGetchannel()), g_SCAN_DURATION_c);
        ZLL_Change_LightDevice_State(g_T_IDLE_STATE_c);
        break;

      case g_T_DEVICE_READY_STATE_c:
    	  //Stack_Events_ClearEvent(g_ZLL_PROCESS_EVENT_c);
      case g_T_IDLE_STATE_c:
      case g_T_IDENTIFY_STATE_c:
        break;
      default:
        break;

  }

}
/******************************************************************************/
void ZLL_SetTargetPriority( bool priority )
{
  ZLL_Light_Target_Priority = priority;
}
#endif /*#ifdef g_ZLL_LIGHT_DEVICE_c*/

/******************************************************************************/
void ZLL_SetKeyIndex( uint8_t keyIndex)
{
  ZLL_SecurityKeyIndex |= keyIndex;
}

/******************************************************************************/

/*******************************************************************************
* Private Functions
*******************************************************************************/



/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/

/* None */

/*******************************************************************************
* End Of File
*******************************************************************************/
