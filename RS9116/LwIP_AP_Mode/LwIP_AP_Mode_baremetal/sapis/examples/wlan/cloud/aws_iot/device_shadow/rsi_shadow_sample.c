/*************************************************************************
 * 
 * Copyright (c) 2019 Redpine Signals Incorporated. All Rights Reserved.
 * 
 * NOTICE:  All  information  contained  herein is, and  remains  the  property of 
 * Redpine Signals Incorporated. The intellectual and technical concepts contained
 * herein  are  proprietary to  Redpine Signals Incorporated and may be covered by 
 * U.S. and Foreign Patents, patents in process, and are protected by trade secret
 * or copyright law. Dissemination  of this  information or reproduction  of  this
 * material is strictly forbidden unless prior written permission is obtained from
 * Redpine Signals Incorporated.
 */
/**
 * @file shadow_sample.c
 * @brief A simple connected window example demonstrating the use of Thing Shadow
 */
 
/**
 * Include files
 * */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"
#include "aws_iot_shadow_interface.h"


#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h" 
#include "rsi_wlan_non_rom.h"
#include "rsi_wlan_config.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"
#include "rsi_driver.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#include "rsi_wlan.h"
#include "rsi_nwk.h"
#include "rsi_utils.h"
#ifdef RSI_M4_INTERFACE 
#include "rsi_chip.h"
#include "rsi_board.h"
#endif

//! certificate includes
#include "client_certificate.crt"
#include "privatekey.key"
#include "starfieldca.pem"

/* Private Define */
#define HOST_ADDRESS_SIZE 255
#define PATH_MAX 128
#define RSI_MQTT_TOPIC           "$aws/things/Test_IoT/shadow/update"
#define HOST_ADDRESS_SIZE 255
#define ROOMTEMPERATURE_UPPERLIMIT 32.0f
#define ROOMTEMPERATURE_LOWERLIMIT 25.0f
#define STARTING_ROOMTEMPERATURE ROOMTEMPERATURE_LOWERLIMIT
#define MAX_LENGTH_OF_UPDATE_JSON_BUFFER 200
#define DHCP_MODE         1
#define SSID              "REDPINE_AP"
#define SECURITY_TYPE     RSI_OPEN
#define PSK               NULL
#define AWS_DOMAIN_NAME  "a25jwtlmds8eip-ats.iot.us-east-2.amazonaws.com"
#define GLOBAL_BUFF_LEN   15000

/* Private Decleration */
static char HostAddress[HOST_ADDRESS_SIZE] = AWS_IOT_MQTT_HOST;
static uint32_t port = AWS_IOT_MQTT_PORT;
uint8_t global_buf[GLOBAL_BUFF_LEN];
char JsonDocumentBuffer[MAX_LENGTH_OF_UPDATE_JSON_BUFFER];
uint8_t mac_addr[6] = {0};

static void simulateRoomTemperature(float *pRoomTemperature) {
	static float deltaChange;

	if(*pRoomTemperature >= ROOMTEMPERATURE_UPPERLIMIT) {
		deltaChange = -0.5f;
	} else if(*pRoomTemperature <= ROOMTEMPERATURE_LOWERLIMIT) {
		deltaChange = 0.5f;
	}

	*pRoomTemperature += deltaChange;
}

static void ShadowUpdateStatusCallback(const char *pThingName, ShadowActions_t action, Shadow_Ack_Status_t status,
		const char *pReceivedJsonDocument, void *pContextData) {
	IOT_UNUSED(pThingName);
	IOT_UNUSED(action);
	IOT_UNUSED(pReceivedJsonDocument);
	IOT_UNUSED(pContextData);

	if(SHADOW_ACK_TIMEOUT == status)
	{
		LOG_PRINT("Update Timeout--");
	} else if(SHADOW_ACK_REJECTED == status)
	{
		LOG_PRINT("Update RejectedXX");
	} else if(SHADOW_ACK_ACCEPTED == status)
	{
		LOG_PRINT("Update Accepted !!");
	}
}

static void windowActuate_Callback(const char *pJsonString, uint32_t JsonStringDataLen, jsonStruct_t *pContext) {
	IOT_UNUSED(pJsonString);
	IOT_UNUSED(JsonStringDataLen);

	if(pContext != NULL) {
		LOG_PRINT("Delta - Window state changed to %d", *(bool *) (pContext->pData));
	}
}

void rsi_remote_socket_terminate_handler1(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	//! Remote socket has been terminated
}

/**
 * @brief	 This API used to configure the basic wireless 
 *         configuration and connect device to access point  
 * @return Return zero on successful wireless configuration.
 */
int32_t rsi_mqtt_client_app(void)
{
	int32_t     status       = RSI_SUCCESS;
#if !(DHCP_MODE)
	uint32_t    ip_addr      = DEVICE_IP;
	uint32_t    network_mask = NETMASK;
	uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
	uint8_t ip_rsp[18]= {0};
	uint8_t fw_version[10]={0};

#ifdef RSI_M4_INTERFACE 
  SystemCoreClockUpdate();
#endif

#ifdef DEBUG_UART
#ifdef RSI_M4_INTERFACE	
	DEBUGINIT();
#endif	
#endif
	
	status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
	if((status < 0) || (status > GLOBAL_BUFF_LEN))
	{
		return status;
	}

	//! Redpine module intialisation
	status = rsi_device_init(LOAD_NWP_FW);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nDevice Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nDevice Initialization Success\r\n");
	}

	//! WC initialization
	status = rsi_wireless_init(0, 0);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWireless Initialization Success\r\n");
	}
	
	status = rsi_send_feature_frame();
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nSend Feature Frame Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
  else
	{
		LOG_PRINT("\r\nSend Feature Frame Success\r\n");
	}

	status = rsi_wlan_get(RSI_FW_VERSION, fw_version,10);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nFirmware get Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
  else
	{
		LOG_PRINT("\r\nFirmware Version :%s\r\n",fw_version);
	}

	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, NULL, NULL);
	status  = rsi_wlan_set_certificate(RSI_SSL_CLIENT, NULL, NULL);
	status  = rsi_wlan_set_certificate(RSI_SSL_CLIENT_PRIVATE_KEY, NULL, NULL);

	status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, starfieldca, (sizeof(starfieldca)-1));
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nLoad SSL CA Certificate Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
  else
	{
		LOG_PRINT("\r\nLoad SSL CA Certificate Success\r\n");
	}	
	
	status  = rsi_wlan_set_certificate(RSI_SSL_CLIENT,awscertificate, (sizeof(awscertificate)-1));
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nLoad SSL Client Certificate Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
  else
	{
		LOG_PRINT("\r\nLoad SSL Client Certificate Success\r\n");
	}	

	status  = rsi_wlan_set_certificate(RSI_SSL_CLIENT_PRIVATE_KEY, awsprivatekey, (sizeof(awsprivatekey)-1));
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nLoad SSL Client Private Certificate Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
  else
	{
		LOG_PRINT("\r\nLoad SSL Client Private Certificate Success\r\n");
	}	
	
	//! Scan for Access points
	status = rsi_wlan_scan((int8_t *)SSID, 0, NULL, 0);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN AP Scan Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
  else
	{
		LOG_PRINT("\r\nWLAN AP Scan Success\r\n");
	}
	

	//! Connect to an Access point
	status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN AP Connect Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
  else
	{
		LOG_PRINT("\r\nWLAN AP Connect Success\r\n");
	}
	

	//! Configure IP
#if DHCP_MODE
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, ip_rsp, 18, 0);
#else
	status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
#endif
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nIP Config Failed, Error Code : 0x%X\r\n", status);
		return status;
	}
  else
	{
		LOG_PRINT("\r\nIP Config Success\r\n");
	}

	//! Initialze remote terminate call back
	rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler1);

	return RSI_SUCCESS;
}

/* Main application entry point */
int main()
{
	IoT_Error_t rc = FAILURE;

	ShadowInitParameters_t sp     = ShadowInitParametersDefault;
	ShadowConnectParameters_t scp = ShadowConnectParametersDefault;
	size_t sizeOfJsonDocumentBuffer = sizeof(JsonDocumentBuffer) / sizeof(JsonDocumentBuffer[0]);
	float temperature = 0.0;

	bool windowOpen = false;
	jsonStruct_t temperatureHandler;
	jsonStruct_t windowActuator;

	// initialize the mqtt client
	AWS_IoT_Client mqttClient={0};

	windowActuator.cb = windowActuate_Callback;
	windowActuator.pData = &windowOpen;
	windowActuator.dataLength = sizeof(bool);
	windowActuator.pKey = "windowOpen";
	windowActuator.type = SHADOW_JSON_BOOL;

	temperatureHandler.cb = NULL;
	temperatureHandler.pKey = "temperature";
	temperatureHandler.pData = &temperature;
	temperatureHandler.dataLength = sizeof(float);
	temperatureHandler.type = SHADOW_JSON_FLOAT;

	sp.pHost = HostAddress;
	sp.port = port;
	sp.pClientCRT = (char *)awscertificate;
	sp.pClientKey = (char *)awsprivatekey;
	sp.pRootCA = (char *)starfieldca;
	sp.enableAutoReconnect = NULL;
	sp.disconnectHandler = NULL;

	//! Call MQTT client application
	rsi_mqtt_client_app();
	LOG_PRINT("\r\nShadow Initialization Start\r\n");

	rc = aws_iot_shadow_init(&mqttClient, &sp);
	if(SUCCESS != rc)
	{
		LOG_PRINT("\r\nShadow Initialization Error\r\n");
		return rc;		
	}
	else
	{
		LOG_PRINT("\r\nShadow Initialization Done\r\n");	
	}
	
	scp.pMyThingName = AWS_IOT_MY_THING_NAME;
	scp.pMqttClientId = AWS_IOT_MQTT_CLIENT_ID;
	scp.mqttClientIdLen = (uint16_t) strlen(AWS_IOT_MQTT_CLIENT_ID);

	LOG_PRINT("\r\nShadow Connection Start\r\n");
	
	rc = aws_iot_shadow_connect(&mqttClient, &scp);
	if(SUCCESS != rc)
	{
 	  LOG_PRINT("\r\nShadow Connection Error\r\n");
		return rc;
	}
	else
	{
	  LOG_PRINT("\r\nShadow Connection Done\r\n");	
	}
	
	/*
	 * Enable Auto Reconnect functionality. Minimum and Maximum time of Exponential backoff are set in aws_iot_config.h
	 *  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
	 *  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
	 */
	LOG_PRINT("\r\nSet Auto Reconnection Start\r\n");
	
	rc = aws_iot_shadow_set_autoreconnect_status(&mqttClient, true);
	if(SUCCESS != rc)
	{
		LOG_PRINT("\r\nUnable to set Auto Reconnect\r\n");
		return rc;
	}
	else
	{
		LOG_PRINT("\r\nSet Auto Reconnection Done\r\n");
 }
	
	rc = aws_iot_shadow_register_delta(&mqttClient, &windowActuator);
	if(SUCCESS != rc)
	{
		LOG_PRINT("\r\nShadow Register Delta Error \r\n");
	}
  else
	{
		LOG_PRINT("\r\nShadow Register Delta Done\r\n");
	}
	
	temperature = STARTING_ROOMTEMPERATURE;

	// loop and publish a change in temperature
	while(NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc || SUCCESS == rc)
	{
		rc = aws_iot_shadow_yield(&mqttClient, 100);
		if(NETWORK_ATTEMPTING_RECONNECT == rc)
		{
			continue;
		}
		simulateRoomTemperature(&temperature);

		rc = aws_iot_shadow_init_json_document(JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
		if(SUCCESS == rc)
		{
			rc = aws_iot_shadow_add_reported(JsonDocumentBuffer, sizeOfJsonDocumentBuffer, 2, &temperatureHandler,
					&windowActuator);
			if(SUCCESS == rc)
			{
				rc = aws_iot_finalize_json_document(JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
				if(SUCCESS == rc)
				{
					LOG_PRINT("\r\nUpdate Shadow: %s\r\n", JsonDocumentBuffer);
					rc = aws_iot_shadow_update(&mqttClient, AWS_IOT_MY_THING_NAME, JsonDocumentBuffer,
							                        ShadowUpdateStatusCallback, NULL, 40, true);
				}
			}
		}
		rsi_delay_ms(1);
	}
	
	if(SUCCESS != rc)
	{	
		LOG_PRINT("\r\n error occurred in the loop \r\n");
  }

	LOG_PRINT("\r\nDisconnecting Start\r\n");
	
  rc = aws_iot_shadow_disconnect(&mqttClient);
	if(SUCCESS != rc)
	{
		LOG_PRINT("\r\nDisconnect Error \r\n");
	}
	else
	{
		LOG_PRINT("Disconnect Done \n");
	}
	while(1);
}
