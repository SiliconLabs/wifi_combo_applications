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

#include "rsi_mqtt_client.h"

#include "rsi_wlan.h"
#include "rsi_utils.h"
#include "rsi_nwk.h"
#include "rsi_emb_mqtt_client.h"

//! Access point SSID to connect
#define SSID              "REDPINE_AP"

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
#define DEVICE_IP         0x6500A8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY           0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF 

#endif

//! Server IP address. Should be in reverse long format
//! E.g: 0x640AA8C0 == 192.168.10.100
#define SERVER_IP_ADDRESS 0x6900A8C0


//! Server port number
#define SERVER_PORT       1234


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
#define RSI_KEEP_ALIVE_PERIOD     0

//! QOS of the message
#define QOS                        0  

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];


//! MQTT Related Macros and declarations
#define RSI_MQTT_TOPIC       "REDPINE_TEST"

//! Message to publish
uint8_t publish_message[] ="THIS IS MQTT CLIENT DEMO FROM REDPINE";

//! MQTT CLient ID
int8_t clientID[] = "MQTTCLIENT";

//! user name for login credentials
int8_t  username[] = "username";

//! password for login credentials
int8_t  password[] = "password";

volatile int halt = 0;




void rsi_emb_mqtt_remote_socket_terminate_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
}
void rsi_emb_mqtt_pub_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
}
void rsi_emb_mqtt_ka_timeout_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
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

  rsi_mqtt_pubmsg_t publish_msg;

  //! WC initialization
  status = rsi_wireless_init(0, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Scan for Access points
  status = rsi_wlan_scan((int8_t *)SSID, 0, NULL, 0);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Connect to an Access point
  status = rsi_wlan_connect((int8_t *)SSID, SECURITY_TYPE, PSK);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Configure IP
#if DHCP_MODE
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, dhcp_mode, 0, 0, 0, NULL, 0, 0);
#else
  status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&ip_addr, (uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
#endif
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! Initialze remote terminate call back
  rsi_emb_mqtt_register_call_back(RSI_WLAN_NWK_EMB_MQTT_REMOTE_TERMINATE_CB, rsi_emb_mqtt_remote_socket_terminate_handler);
  rsi_emb_mqtt_register_call_back(RSI_WLAN_NWK_EMB_MQTT_PUB_MSG_CB, rsi_emb_mqtt_pub_handler);
  rsi_emb_mqtt_register_call_back(RSI_WLAN_NWK_EMB_MQTT_KEEPALIVE_TIMEOUT_CB, rsi_emb_mqtt_ka_timeout_handler);

  //! MQTT client initialisation
  status = rsi_emb_mqtt_client_init((int8_t *)&server_address,  SERVER_PORT,  CLIENT_PORT, RSI_EMB_MQTT_CLEAN_SESSION,  RSI_KEEP_ALIVE_PERIOD, (int8_t *)clientID, (int8_t *)username, (int8_t *)password);

  if(status != RSI_SUCCESS)
  {
    return status;
  }
  //! Connect to the MQTT brohker/server
  status = rsi_emb_mqtt_connect((RSI_EMB_MQTT_USER_FLAG | RSI_EMB_MQTT_PWD_FLAG), NULL, 0,  NULL);                                 
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  //! Subscribe to the topic given
  status = rsi_emb_mqtt_subscribe(QOS, RSI_MQTT_TOPIC);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

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
    status = rsi_emb_mqtt_publish(RSI_MQTT_TOPIC, &publish_msg);
    if(status != RSI_SUCCESS)
    {
      return status;
    }


    //! UnSubscribe to the topic given
    status = rsi_emb_mqtt_unsubscribe(RSI_MQTT_TOPIC);
    if(status != RSI_SUCCESS)
    {
      //! Error in receiving
      return status;
    }
    //! Disconnect to the MQTT broker
    status = rsi_emb_mqtt_disconnect();
    if(status != RSI_SUCCESS)
    {
      //! Error in receiving
      return status;
    }
    status = rsi_emb_mqtt_destroy();
    if(status != RSI_SUCCESS)
    {
      //! Error in receiving
      return status;
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
    return status;
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

