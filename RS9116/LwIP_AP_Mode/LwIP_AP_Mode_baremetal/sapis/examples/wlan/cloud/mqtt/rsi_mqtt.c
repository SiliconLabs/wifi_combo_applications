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

/*================================================================================
 * @brief : This file contains example application for MQTT Client
 * @section Description :
 * This application demonstrates how to configure Silicon device as MQTT client
 * and how to establish connection with MQTT broker and how to subscribe, publish
 * and receive the MQTT messages from MQTT broker.
 =================================================================================*/

/**
 * Include files
 * */
//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h" 
#include "rsi_wlan_non_rom.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"
//! Error include files
#include "rsi_error.h"

//! OS include file to refer OS specific functionality
#include "rsi_os.h"

#ifdef RSI_M4_INTERFACE
#include "rsi_board.h"
#endif

//! MQTT wrappers to include
#include "rsi_mqtt_client.h"

#include "rsi_wlan.h"
#include "rsi_utils.h"

#include "rsi_nwk.h"

#define AWS 1
#if AWS
#include "client_certificate.crt"
#include "privatekey.key"
#include "starfieldca.pem"
#endif
//To use asynchronous MQTT please add ASYNC_MQTT macro in IDE configuration

//! Access point SSID to connect
#define SSID              "Silicon_AP"
//! Security type
#define SECURITY_TYPE     RSI_OPEN

//! Password
#define PSK               ""

//! DHCP mode 1- Enable 0- Disable
#define DHCP_MODE         1

//! If DHCP mode is disabled give IP statically
#if !(DHCP_MODE)

//! IP address of the module 
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP         0x650AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 

#endif

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0xC8213A0D


//! Server port number
#define SERVER_PORT       8883


//! Client port number
#define CLIENT_PORT       5001

//! Memory length for driver
#define GLOBAL_BUFF_LEN   15000

//! Wlan task priority
#define RSI_WLAN_TASK_PRIORITY   1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY   1

//! Wlan task stack size
#define RSI_WLAN_TASK_STACK_SIZE  500

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE  500

//! Keep alive period
#define RSI_KEEP_ALIVE_PERIOD     100

//!Memory to initialize MQTT client Info structure
#define MQTT_CLIENT_INIT_BUFF_LEN  3500

//! MQTT Connection ack
#define MQTT_CONNACK               0x20

//! MQTT Subscription ack
#define MQTT_SUBACK                0x90

//! QOS of the message
#define QOS                        0  

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

#define AWS_DOMAIN_NAME  "a25jwtlmds8eip-ats.iot.us-east-2.amazonaws.com"
//#define ALIBABA_DOMAIN_NAME  "a1BLKg93h0r.iot-as-mqtt.cn-shanghai.aliyuncs.com"

//! MQTT Related Macros and declarations
#define RSI_MQTT_TOPIC       "$aws/things/Test_IoT/shadow/update"
#define AWS_UPDATE_TOPIC     "$aws/things/Test_IoT/shadow/update/accepted"


//! Message to publish
uint8_t publish_message[] ="THIS IS MQTT CLIENT DEMO FROM REDPINE";

//! MQTT CLient ID
int8_t clientID[] = "MQTTCLIENT";

//! user name for login credentials
int8_t  username[] = "username";

//! password for login credentials
int8_t  password[] = "password";

volatile int halt = 0;


int8_t mqqt_client_buffer[MQTT_CLIENT_INIT_BUFF_LEN];

#ifdef ASYNC_MQTT
rsi_mqtt_client_info_t *rsi_mqtt_client = NULL;
uint8_t connack,suback ,data_received;
uint8_t rcvbuf[2048];
uint8_t pub=1;

void handleMQTT(uint32_t sock_no, uint8_t *buffer, uint32_t length)
{
	char txt[2048];
	int len = 0;
	int i = 0;

	/*Check for CONNACK packet*/
	if(buffer[0] == MQTT_CONNACK)
	{
		if(buffer[3] == 0)
		{
			rsi_mqtt_client->mqtt_client.isconnected = 1;
			connack = 1;
		}
		else{
			return;
		}
	}

	/*Check for SUBACK packet*/
	else if(buffer[0] == MQTT_SUBACK)
	{
		++suback;
	}

	/*Check for MQTT data packet*/
	else {
		memset(rcvbuf, '\0', sizeof(rcvbuf));
		memset(txt, '\0', sizeof(txt));

		memcpy(txt, buffer, length);

		/*Enable the command flag*/
		data_received =1;
	}
}
#else
//! Call back when a message is received on the published topic
void rsi_message_received(MessageData* md)
{
  //! process the received data
  halt = 1;
  return;
}
#endif

//! Remote socket terminate call back handler
void rsi_remote_socket_terminate_handler(uint16_t status, const uint8_t *buffer, const uint16_t length)
{
  //! Remote socket has been terminated
}


int32_t rsi_mqtt_client_app()
{

  int32_t     status       = RSI_SUCCESS;
#if !(DHCP_MODE)
  uint32_t    ip_addr      = DEVICE_IP;
  uint32_t    network_mask = NETMASK;
  uint32_t    gateway      = GATEWAY;
#else
  uint8_t     dhcp_mode    = (RSI_DHCP | RSI_DHCP_UNICAST_OFFER);
#endif
  uint32_t    server_address =  SERVER_IP_ADDRESS;
  uint8_t fw_version[10]       =  {0};
#ifndef ASYNC_MQTT
  rsi_mqtt_client_info_t *rsi_mqtt_client = NULL;
#endif

  MQTTMessage publish_msg;

  rsi_rsp_dns_query_t dns_query_rsp;

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
    return status;
  }

  status = rsi_wlan_get(RSI_FW_VERSION, fw_version,10);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, NULL, NULL);
  status  = rsi_wlan_set_certificate(RSI_SSL_CLIENT, NULL, NULL);
  status  = rsi_wlan_set_certificate(RSI_SSL_CLIENT_PRIVATE_KEY, NULL, NULL);


#if AWS

  status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, starfieldca, (sizeof(starfieldca)-1));
  //status  = rsi_wlan_set_certificate(RSI_SSL_CA_CERTIFICATE, rootalibaba, (sizeof(rootalibaba)-1));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status  = rsi_wlan_set_certificate(RSI_SSL_CLIENT, awscertificate, (sizeof(awscertificate)-1));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  status  = rsi_wlan_set_certificate(RSI_SSL_CLIENT_PRIVATE_KEY, awsprivatekey, (sizeof(awsprivatekey)-1));
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#endif
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
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0, 0);
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

  
  do{
    //!dns request
    //status = rsi_dns_req(RSI_IP_VERSION_4, (uint8_t *)ALIBABA_DOMAIN_NAME, NULL, NULL, &dns_query_rsp, sizeof(dns_query_rsp));
    status = rsi_dns_req(RSI_IP_VERSION_4, (uint8_t *)AWS_DOMAIN_NAME, NULL, NULL, &dns_query_rsp, sizeof(dns_query_rsp));

  }while(status != RSI_SUCCESS);

  server_address = rsi_bytes4R_to_uint32(dns_query_rsp.ip_address[0].ipv4_address);


  //! Initialze remote terminate call back
  rsi_wlan_register_callbacks(RSI_REMOTE_SOCKET_TERMINATE_CB, rsi_remote_socket_terminate_handler);

#ifdef ASYNC_MQTT
  while(1)
  {
start:
    //! MQTT client initialisation
    rsi_mqtt_client = rsi_mqtt_client_init((uint8_t *)&mqqt_client_buffer, MQTT_CLIENT_INIT_BUFF_LEN,(int8_t *)&server_address,SERVER_PORT,CLIENT_PORT,0,RSI_KEEP_ALIVE_PERIOD);
    if(mqqt_client_buffer == NULL)
    {
      goto start;
    }
    //! Connect to the MQTT brohker/server
    status = rsi_mqtt_connect(rsi_mqtt_client,RSI_SSL_ENABLE,(uint8_t *)&clientID,NULL,NULL, handleMQTT);
    if(status != RSI_SUCCESS)
    {
      rsi_mqtt_disconnect(rsi_mqtt_client);
      continue ;
    }

    while(1)
    {
      rsi_wireless_driver_task();
      if(rsi_mqtt_client->mqtt_client.isconnected == 1)
      {
        //! Subscribe to the topic given
        status = rsi_mqtt_subscribe(rsi_mqtt_client,QOS,RSI_MQTT_TOPIC,NULL);
        if(status != RSI_SUCCESS)
        {
          rsi_mqtt_disconnect(rsi_mqtt_client);
          continue ;
        }

        while(1)
        {
          rsi_wireless_driver_task();

          if((suback) && pub == 1)
          {
            //!The DUP flag MUST be set to 1 by the Client or Server when it attempts to re-deliver a PUBLISH Packet
            //!The DUP flag MUST be set to 0 for all QoS 0 messages
            publish_msg.dup = 0;

            //! This field indicates the level of assurance for delivery of an Application Message. The QoS levels are
            //! 0  - At most once delivery
            //! 1  - At least once delivery
            //! 2  - Exactly once delivery
            publish_msg.qos = QOS;

            //! If the RETAIN flag is set to 1, in a PUBLISH Packet sent by a Client to a Server, the Server MUST store
            //! the Application Message and its QoS, so that it can be delivered to future subscribers whose
            //! subscriptions match its topic name
            publish_msg.retained = 0;

            //! Attach paylaod
            publish_msg.payload = publish_message;

            //! Fill paylaod length
            publish_msg.payloadlen = sizeof(publish_message);


            //! Publish message on the topic
            rsi_mqtt_publish(rsi_mqtt_client,RSI_MQTT_TOPIC,&publish_msg);
            pub=0;
          }
        }
      }
    }
  }
#else
  //! MQTT client initialisation
  rsi_mqtt_client = rsi_mqtt_client_init(mqqt_client_buffer, MQTT_CLIENT_INIT_BUFF_LEN,(int8_t *)&server_address,SERVER_PORT,CLIENT_PORT,0,RSI_KEEP_ALIVE_PERIOD);
  if(mqqt_client_buffer == NULL)
  {
    return status;
  }
  //! Connect to the MQTT brohker/server
  status = rsi_mqtt_connect(rsi_mqtt_client,RSI_SSL_ENABLE,clientID,NULL,NULL);
  if(status != RSI_SUCCESS)
  {
    LOG_PRINT("\r\nConnect to the MQTT broker/server Failed, Error Code : 0x%X\r\n",status);
    return status;
  }
  else
  {
    LOG_PRINT("\r\nConnect to the MQTT broker/server Success\r\n");
  }

  //! Subscribe to the topic given
  status = rsi_mqtt_subscribe(rsi_mqtt_client,QOS,RSI_MQTT_TOPIC,rsi_message_received);
  if(status != RSI_SUCCESS)
  {
    LOG_PRINT("\r\nSubscription to Topic Failed, Error Code : 0x%X\r\n",status);
    return status;
  }
  else
  {
    LOG_PRINT("\r\nSubscription to Topic Success\r\n");
  }

  //!The DUP flag MUST be set to 1 by the Client or Server when it attempts to re-deliver a PUBLISH Packet
  //!The DUP flag MUST be set to 0 for all QoS 0 messages
  publish_msg.dup = 0;

  //! This field indicates the level of assurance for delivery of an Application Message. The QoS levels are
  //! 0  - At most once delivery
  //! 1  - At least once delivery
  //! 2  - Exactly once delivery
  if (QOS == QOS0)
  {
    publish_msg.qos = QOS0;
  }
  else if (QOS == QOS1)
  {
    publish_msg.qos = QOS1;
  }
  else
  {
    publish_msg.qos = QOS2;
  }

  //! If the RETAIN flag is set to 1, in a PUBLISH Packet sent by a Client to a Server, the Server MUST store
  //! the Application Message and its QoS, so that it can be delivered to future subscribers whose
  //! subscriptions match its topic name
  publish_msg.retained = 0;

  //! Attach paylaod 
  publish_msg.payload = publish_message;

  //! Fill paylaod length
  publish_msg.payloadlen = sizeof(publish_message);


  //! Publish message on the topic
  status = rsi_mqtt_publish(rsi_mqtt_client,RSI_MQTT_TOPIC,&publish_msg);
  if(status != RSI_SUCCESS)
  {
    LOG_PRINT("\r\n Publish to Topic Failed, Error Code : 0x%X\r\n",status);
    return status;
  }
  else
  {
    LOG_PRINT("\r\n Publish to Topic Success\r\n");
  }

  while (!halt)
  {
    //! Recv data published on the subscribed topic
    status = rsi_mqtt_poll_for_recv_data(rsi_mqtt_client,10);
    if(status != RSI_SUCCESS)
    {
      status = rsi_wlan_socket_get_status(rsi_mqtt_client->mqtt_client.ipstack->my_socket);
      if(status == RSI_ERROR_SOCKET_RCV_TIMEOUT)
        continue;
    }
    if(status != RSI_SUCCESS)
    {
      //! Error in receiving
      return status;
    }
  }
#endif

  //! UnSubscribe to the topic given
  rsi_mqtt_unsubscribe(rsi_mqtt_client,RSI_MQTT_TOPIC);
  if(status != RSI_SUCCESS)
  {
    LOG_PRINT("\r\nUnsubscription to Topic Failed, Error Code : 0x%X\r\n",status);
    return status;
  }
  else
  {
    LOG_PRINT("\r\nUnsubscription to Topic Success\r\n");
  }

  //! Disconnect to the MQTT broker
  rsi_mqtt_disconnect(rsi_mqtt_client);
  if(status != RSI_SUCCESS)
  {
    LOG_PRINT("\r\nDisconnect to the MQTT broker Failed, Error Code : 0x%X\r\n",status);
    return status;
  }
  else
  {
    LOG_PRINT("\r\nDisconnect to the MQTT broker Success\r\n");
  }
  return 0;
}

void main_loop(void)
{
  while(1)
  {
    ////////////////////////
    //! Application code ///
    ////////////////////////

    //! event loop
    rsi_wireless_driver_task();

  }
}

int main()
{
  int32_t status;
#ifdef RSI_WITH_OS	

  rsi_task_handle_t wlan_task_handle = NULL;

  rsi_task_handle_t driver_task_handle = NULL;
#endif


  //! Driver initialization
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

  //! Unmask interrupts


#ifdef RSI_WITH_OS	
  //! OS case
  //! Task created for WLAN task
  rsi_task_create(rsi_mqtt_client, "wlan_task", RSI_WLAN_TASK_STACK_SIZE, NULL, RSI_WLAN_TASK_PRIORITY, &wlan_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

#else
	//! NON - OS case
	//! Call MQTT client application
	status = rsi_mqtt_client_app();

  //! Application main loop
  main_loop();
#endif
  return status;

}


