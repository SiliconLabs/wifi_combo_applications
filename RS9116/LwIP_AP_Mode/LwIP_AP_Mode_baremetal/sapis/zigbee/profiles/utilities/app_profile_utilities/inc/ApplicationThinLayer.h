#ifndef _APPLICATION_THINLAYER_H_
#define _APPLICATION_THINLAYER_H_


/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "stack_common.h"
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "apsde_interface.h"
#include "ZCL_Foundation.h"
/*-----------------------------------------------------------------------------
* Global Constants
*-----------------------------------------------------------------------------*/
#define g_DEFAULT_RADIUS_c                              0x0a

#define g_HIGH_SECURITY_c                               0x03
#define g_NETWORK_SECURITY_c                            0x02
#define g_NO_SECURITY_c                                 0x00
#define g_NETWORK_SECURITY_WITH_APS_ACK_c               0x06
#define g_APS_ACK_REQUIRED_c                            0x04

#define g_DUMMY_ENDPOINT_ID_c							0xFE

#if ((defined HA_SA) || (defined SE_SA) || (defined ZLL_SA))

#define g_BCAST_JITTER_TIME_INTERVAL_c					3000000
#define g_TRIGGER_HA_SAMPLE_APP_INTERVAL_c				2000000
#define g_TRIGGER_SE_SAMPLE_APP_INTERVAL_c				10000000
#define g_TRIGGER_SE_APP_ALIVE_INTERVAL_c				1000000 /*1 second*/
#define g_TRIGGER_GET_CURRENT_TIME_DELAY_c				5000000 /*5 second*/
#define g_TRIGGER_PERMIT_JOIN_CONF_DELAY_c				5000000 /*5 second*/
#define g_TOUCH_LINK_TRIGGER_TIME_INTERVAL_c			30000000 /*10 second*/
#define g_TRIGGER_ZLL_APP_ALIVE_INTERVAL_c				1000000 /*1 second*/
#define g_TRIGGER_ZLL_SAMPLE_APP_INTERVAL_c				10000000
#define g_TRIGGER_SEND_ON_TIME_DELAY_c					5000000 /*5 second*/
#endif //HA_SA
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct clusterSpecificSecurity_tag
{
    uint16_t clusterID;
    uint8_t security;
} __attribute__((__packed__))clusterSpecificSecurity_t;


#ifdef HA_SA
typedef enum event_ids{
	EVENT_START_NETWORK,
	EVENT_JOIN_NETWORK,
	EVENT_WAIT_STATE,
	EVENT_NETWORK_READY_STATE,
	EVENT_ERROR_STATE
}event_ids_t;


typedef enum data_events{
	g_DATA_NO_EVENT_c,
	g_TRIGGER_EVENT_c,
	g_MATCH_DESC_REQ_c,
	g_SEND_ON_CMD_c,
	g_SEND_TOGGLE_CMD_c,
	g_READ_ONOFF_ATTRIBUTE_c,
	g_SEND_OFF_CMD_c,
	g_WAIT_FOR_MATCH_RESP_c,
	g_WAIT_TO_COMPLETE_BCAST_c,
	g_WAIT_FOR_ON_CMD_CONF_c,
	g_WAIT_FOR_OFF_CMD_CONF_c,
	g_WAIT_FOR_READ_ATTRIB_RESP_c,
  g_INITATE_SLEEP_STATE_c,
  g_WAIT_FOR_TOGGLE_CMD_CONF_c,
}data_events_t;

typedef struct{
	event_ids_t state;
	data_events_t data_event;
}state_machine_t;


typedef struct lightDeviceInfo_tag{

	uint16_t shortaddress;
	uint8_t endpoint;
}__attribute__((__packed__)) lightDeviceInfo_t;

typedef struct switchDeviceInfo_tag{

	uint16_t shortaddress;
	uint8_t endpoint;
}__attribute__((__packed__)) switchDeviceInfo_t;

void HA_SampleApp_StateMachine(void);

#endif //HA_SA

#ifdef SE_SA
typedef enum event_ids{
	EVENT_APP_INIT,
	EVENT_START_NETWORK,
	EVENT_JOIN_NETWORK,
	EVENT_SET_PERMIT_JOIN_STATE,
	EVENT_WAIT_PERMIT_JOIN_CONF_STATE,
	EVENT_WAIT_STATE,
	EVENT_NETWORK_READY_STATE,
	EVENT_ERROR_STATE
}event_ids_t;


typedef enum data_events{
	g_DATA_NO_EVENT_c,
	g_TRIGGER_EVENT_c,
	g_MATCH_DESC_REQ_FOR_TIME_c,
	g_MATCH_DESC_REQ_FOR_PRICE_c,
	g_READ_TIME_ATTRIBUTE_c,
	g_SEND_GET_CURRENT_PRICE_c,
	g_WAIT_FOR_PUBLISH_PRICE_INFO_c,
	g_WAIT_FOR_MATCH_RESP_FOR_TIME_c,
	g_WAIT_FOR_MATCH_RESP_FOR_PRICE_c,
	g_WAIT_TO_COMPLETE_BCAST_c,
	g_WAIT_FOR_ON_CMD_CONF_c,
	g_WAIT_FOR_OFF_CMD_CONF_c,
	g_WAIT_FOR_READ_ATTRIB_RESP_FOR_TIME_c,
	g_WAIT_FOR_READ_ATTRIB_RESP_c,
	g_WAIT_FOR_SET_PRICE_c,
	g_WAIT_FOR_PUBLISH_PRICE_CMD_c,
	g_WAIT_FOR_DISPLAY_MESSAGE_c,
	g_CONFIGURE_PRICE_INFO_c,
	g_CONFIGURE_LOCAL_TIME_c,
	g_CONFIGURE_MESSAGE_INFO_c,
	g_SEND_GET_LAST_MESSAGE_c
}data_events_t;

typedef struct DeviceInfo_tag{

	uint16_t shortaddress;
	uint8_t endpoint;
}__attribute__((__packed__)) DeviceInfo_t;

typedef struct{
	event_ids_t state;
	data_events_t data_event;
}state_machine_t;

void SE_SampleApp_StateMachine(void);

void SE_SampleApp_Init(void);

#endif /*SE_SA*/

#ifdef ZLL_SA

typedef enum event_ids{
	EVENT_TOUCHLINK_INITIALIZED = 0x00,
	EVENT_READ_RESPONSE_RECEIVED = 0x01,
	EVENT_WAIT_FOR_TOUCHLINK_PROCEDURE = 0x02,
	EVENT_DEVICE_READY = 0x03,
	EVENT_SEND_CONFIRM_RECEIVED = 0x04,
	EVENT_SEND_ON_COMMAND = 0x05,
	EVENT_SEND_OFF_COMMAND = 0x06,
	EVENT_ERROR_STATE = 0x07,
	g_Controller_Intitial_State_c = 0x08,
	g_WAIT_FOR_TOUCHLINK_c = 0x09,
	g_SEND_ON_COMMAND_c = 0x0a,
	g_WAIT_FOR_ON_CMD_CONF_c = 0x0b,
	g_WAIT_FOR_READ_ATTRIB_RESP_c = 0x0c,
	g_WAIT_FOR_OFF_WITH_EFFECT_CMD_CONF_c = 0x0d,
	g_READ_ONOFF_ATTRIBUTE_c = 0x0e,
	g_SEND_OFF_WITH_EFFECT_CMD_c = 0x0f,
	g_NO_DATA_EVENT_c = 0x10,
	g_SEND_OFF_CMD_c = 0x11,
	g_WAIT_FOR_OFF_CMD_CONF_c = 0x12,


}event_ids_t;


typedef enum data_events{
	g_On_Command_Sent = 0x0,
	g_Off_Command_Sent = 0x1,
	g_Off_With_Efffect_Command_Sent = 0x2
}data_events_t;

typedef struct{
	event_ids_t state;
	data_events_t data_event;
}state_machine_t;

typedef struct lightDeviceInfo_tag{

	uint16_t shortaddress;
	uint8_t endpoint;
}__attribute__((__packed__)) lightDeviceInfo_t;


#endif // ZLL_SA

#if ((defined HA_SA) || (defined SE_SA) || (defined ZLL_SA))
void sample_app_change_state(ZigBeeNWKStatusInfo statusInfo);

extern state_machine_t stateMachine;
#endif /* (defined HA_SA) || (defined SE_SA) */


/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

uint8_t App_GetTransSeqNumber( void );

void Handle_ZCL_Data_Message ( uint8_t buffer_index );

void Handle_Broadcast_EP ( uint8_t buffer_index );

void App_SendDefaultResponse ( uint8_t status, uint8_t useNWKSecurity,
                               APSDE_Data_Indication_t* p_dataInd );

void App_Stack_Information ( uint8_t msg_type, uint8_t msg_index );

uint8_t App_Get_Cluster_Definition ( uint8_t framedirection,
                                     uint8_t endpointId,
                                     uint16_t clusterId,
                                     App_Cluster_Def_t ** pZCLClusterDefinition );

uint8_t* App_Get_AttrDataLocation ( uint8_t frame_direction,
                                    uint8_t endpoint_id,
                                    uint16_t cluster_id,
                                    App_Cluster_Def_t ** pZCLClusterDefinition );

void Send_Response_To_Application ( APSDE_Data_Indication_t *p_buffer,
                                    uint8_t buffer_index );
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void Send_InterPan_Response_To_Application ( APSDE_InterPan_Data_Indication_t* p_buffer,
                                             uint8_t buffer_index );
#endif
void Send_Event_To_Application ( ZCL_Event_t * event );

void App_ZCL_Data_Init ( void );

void Create_ZDP_Request ( uint8_t ZDP_request_id, uint8_t msg_index );

void App_Main ( void );

void App_Init( void );

void APSDE_Data_Request ( uint8_t *p_ZCL_Data, uint8_t ZCL_Data_Length,
     uint8_t dest_addr_mode, Address dest_address, uint16_t cluster_id,
     uint8_t dest_endpoint, uint8_t src_endpoint );

uint8_t App_ReadWriteAttribute(  uint8_t endPointId, uint16_t clusterId,
                            uint16_t attributeId, uint8_t* pDpDataToBeCopiedata, uint8_t operation );

uint8_t App_ReadWriteClientAttribute(  uint8_t endPointId,   uint16_t clusterId,
            uint16_t attributeId,  uint8_t* pData,   uint8_t operation );

void Handle_ZCL_Message( uint8_t buffer_index, APSDE_Data_Indication_t *p_buffer );

#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void Handle_ZCL_InterPan_Message( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd );

void App_SendInterPanData(  uint8_t dstAddrMode, uint8_t* dstAddress,
                            uint16_t dstPanId, uint16_t clusterId, uint8_t asduLength,
                            uint8_t* asdu );
#endif

void App_SendData( uint8_t dstAddrMode, uint8_t* dstAddress, uint8_t dstendpoint,
                   uint16_t clusterId, uint8_t asduLength,  uint8_t* asdu );

void App_sendAPSData(uint8_t dstAddrMode, uint8_t* dstAddress, uint8_t dstendpoint,
                     uint16_t clusterId, uint8_t asduLength,  uint8_t* asdu,
                     uint8_t withACK );

uint8_t App_getClusterSecurity( uint16_t clusterId );
void App_CreateAndSendLeaveRequest( uint8_t *pDestAddress );
int16_t rsi_zigb_leave_network(void);

void App_Reset( void );
uint16_t App_getTCShortAddress( void );

uint8_t App_getAPSMaxPayloadForDevice (uint16_t clusterId, Address *pAddress,
                                       uint8_t addr_mode);

#if (g_CLI_SUPPORT_d == 1)
void Print_ZCL_Incoming_Msg ( APSDE_Data_Indication_t *p_buffer, uint8_t buffer_index );
#endif

#if( g_DIAGNOSTICS_CLUSTER_ENABLE_d ==1 )

void app_reset_diagnostic_cluster_variables(void);
#endif


/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

#endif                                 /* _APPLICATION_THINLAYER_H_ */
