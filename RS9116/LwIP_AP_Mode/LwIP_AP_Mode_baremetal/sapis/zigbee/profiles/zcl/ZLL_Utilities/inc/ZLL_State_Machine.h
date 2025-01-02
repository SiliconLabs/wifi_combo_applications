/******************************************************************************
* ZLL_State_Machine.h
*******************************************************************************
* Program Description:
*
*
******************************************************************************/
#ifndef _ZLL_STATE_MACHINE_H_
#define _ZLL_STATE_MACHINE_H_


/*****************************************************************************
* Includes
*****************************************************************************/





/*****************************************************************************
* Global Constants
*****************************************************************************/



/*****************************************************************************
* Type Definitions
*****************************************************************************/

/* Following states are used for Touch link process */
/* Initiator's(I) States */
#ifdef g_ZLL_LIGHT_CONTROLLER_c
typedef enum {
	/* Initially controller will be in initially state ,it will not move to
	device discovery state unless user initiates through button press or by
	some command.
	- FN or Non FN to be checked.
	- If FN, reset all parameters and shd initialise with network parameters.
	- If NFN, since it is end device it shd perform rejoin procdeure ,
	  on failure it shd enter dormant state.
		*/
	g_I_INITIAL_STATE_c = 0,
	/* In this state initiator will perform scan req to find suitable target
	device to start network,
	- select based device based on RSSI and Priority bit Value
	 */
   g_I_DEVICE_DISCOVERY_STATE_c = 1,


  /* Device Information request
  */
  g_I_DEVICE_INFORMATION_REQUEST_STATE_c = 2,

  /* Device Network Startup request state
  */
  g_I_DEVICE_IDENTIFY_REQUEST_STATE_c = 3,

  g_I_DEVICE_NETWORK_START_STATE_c = 4,

  g_I_DEVICE_REJOIN_STATE_c = 5,


  g_I_JOIN_ENDDEVICE_STATE_c = 6,

	/* If target device is started successfully then ,initiator will perform
	network rejoin procedure then it will reach device ready state.
	On failure shd return back to discovering state*/

  /*Wait for Response state*/
  g_I_WAIT_FOR_RESPONSE_STATE_c = 7,

  g_I_DEVICE_READY_STATE_c = 8,

  g_DEVICE_READY_STATE_c = 8,

  /* Initiator can ask router device or end device , to join the network.
  Note : router and device info is retrieved from scan response stored during
  device discovery */
  g_I_NETWORK_JOIN_STATE_c = 9,

  g_I_WAIT_FOR_NEXT_COMMAND_STATE_c = 10,
    /* If Initiator device is in ready state, then device shd proceed with
	joining other devices with the network , after time out it can exit
	touchlink process , or can proceed with device discovery for joining other
	device to the nwk with new transaction Id*/
    g_I_END_STATE_c = 11
}InitiatorState;
#endif /* #ifdef g_ZLL_LIGHT_CONTROLLER_c*/

#ifdef g_ZLL_LIGHT_DEVICE_c
/* Target Device's States*/
typedef enum {
	/* Initially Target device will be remain in initial state, waiting for
	Scan request from initiator , on specified channel in SAS*/
	g_T_INITIAL_STATE_c	= 0,
  g_T_WAIT_FOR_SCAN_REQUEST_STATE_c	= 1,
  g_T_ACTIVE_SCAN_REQUEST_STATE_c = 2,
    /* After reception of scan req and transmission of scan response from
	target device it will move to idle state ,it exists from idle state only if it
	receives start req or if it has reached timeout then it will move to initial
	state */
	g_T_IDLE_STATE_c = 3,
	/* From idle state, if it receives identify request it will move to
	identify mode and has to exit after identifytimeout period,
	later moves to idle state*/
	g_T_IDENTIFY_STATE_c = 4,
	/* From Idle state , if it receives nwk start request, then it has to proceed
	with startup procedure, then move to ready state on success */
	g_T_NETWORK_START_STATE_c = 5,
  g_T_INITIATE_ACTIVE_SCAN_REQ_c = 6,
	/* If target device has started successfully , then it will proceed to store
	nwk parameter in NVM */
	g_T_DEVICE_READY_STATE_c = 8,

  g_DEVICE_READY_STATE_c = 8,

	/* prepares to exit from Touch link procedure*/
	g_T_END_STATE_c = 10
}TargetState;

#endif /* g_ZLL_LIGHT_DEVICE_c*/




/*****************************************************************************
* Memory declarations
*****************************************************************************/
extern uint8_t ZLL_State;
#ifdef g_ZLL_LIGHT_CONTROLLER_c
//extern uint8_t ZLL_Controller_State ;
extern bool TouchLinkCommissioningTriggered ;
extern uint32_t ZLL_aplcMinStartupDelaytime ;
extern uint32_t ZLL_aplcResettime ;


//#define ZLL_Change_Controller_State( state ) ZLL_Controller_State = state

#define ZLL_Change_Controller_State( state ) ZLL_NVM_Parameters.controller_device_state = state

//#define ZLL_Read_Controller_State( )          ZLL_Controller_State

#define ZLL_Read_Controller_State( )  ZLL_NVM_Parameters.controller_device_state
#define ZLL_Get_DeviceState()      ZLL_State = ZLL_NVM_Parameters.controller_device_state

#endif /*#ifdef g_ZLL_LIGHT_CONTROLLER_c*/

#ifdef g_ZLL_LIGHT_DEVICE_c


extern bool ZLL_Light_Target_Priority;

//#define ZLL_Read_Light_Device_State( )          ZLL_Light_State
#define ZLL_Read_Light_Device_State( ) ZLL_NVM_Parameters.light_device_state

//#define ZLL_Change_LightDevice_State( state )        ZLL_Light_State = state

#define ZLL_Change_LightDevice_State( state ) ZLL_NVM_Parameters.light_device_state = state
#define ZLL_Get_DeviceState()      ZLL_State = ZLL_NVM_Parameters.light_device_state
//extern uint8_t ZLL_Light_State;
#endif //#ifdef g_ZLL_LIGHT_DEVICE_c

extern uint8_t ZLL_SecurityKeyIndex;


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

void ZLL_Tick(void);
void ZLL_Reset( void );
uint8_t ZLL_ReadScanReponseForTarget( uint8_t *pTarget_long_address);

#ifdef g_ZLL_LIGHT_CONTROLLER_c
void ZLL_HandleControllerStateMachine( void );
void ZLL_TriggerTouchLinkCommisioning ( void );
void ZLL_ResetTargetDevice(bool set);
void App_ZLL_InterPAN_Life_Timeout_Callback ( uint8_t timer_id );

#endif //g_ZLL_LIGHT_CONTROLLER_c

#ifdef g_ZLL_LIGHT_DEVICE_c
void ZLL_HandleDeviceStateMachine( void );
void App_ZLL_IdentifyCallBack( uint8_t timer_id );
void ZLL_SetTargetPriority( bool priority );

#endif /*g_ZLL_LIGHT_DEVICE_c*/
void ZLL_SetKeyIndex( uint8_t keyIndex );


void ZLL_Init( void );

#endif /*_ZLL_STATE_MACHINE_H_*/
/*****************************************************************************
* End Of File                                       _ZLL_STATE_MACHINE_H_
*****************************************************************************/
