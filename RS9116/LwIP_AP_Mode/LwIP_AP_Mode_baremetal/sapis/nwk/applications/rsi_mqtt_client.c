/**
 * @file    rsi_mqtt_client.c
 * @version 0.1
 * @date    15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains MQTT API's
 *
 *  @section Description  This file contains MQTT application related 
 *  API's.
 *
 *
 */


#include "rsi_driver.h"

#include "rsi_mqtt_client.h"

#include "MQTT_wrappers.h"

#include "MQTTClient.h"

#include "rsi_nwk.h"


/*==============================================*/
/**
 * @fn         rsi_mqtt_client_info_t * rsi_mqtt_client_init( int8_t *buffer, uint32_t length, int8_t *server_ip, uint32_t server_port, uint32_t client_port, uint16_t flags, uint16_t keep_alive_interval)
 * @brief      Allocates memory for MQTT client information 
 * @param[in]  buffer , buffer pointer to  allocate  memory for MQTT Client information 
 * @param[in]  length , buffer length
 * @param[in]  server_ip , IPv4 address of the MQTT broker.  
 * @param[in]  server_port ,MQTT broker port number
 * @param[in]  client_port ,MQTT client port number
 * @param[in]  flags,Each bit has its own significance
 *              Bit(0) -IP version of the Server IP 
 *                 0 - IPv4 
 *                 1 - IPv6
 * @param[in]  keep_alive_interval , MQTT client keep alive interval
 *             If there are no transactions between MQTT client and broker with in 
 *             this time period, MQTT Broker disconnects the MQTT client
 *             If 0 -> Server Doesnot disconnects
 * @return     rsi_mqtt_client_info_t 
 *             0>  = Returns MQTT client information structure pointer
 *             NULL = In case of failure 
 * @section description
 * This API is used to allocate memory for the MQTT for a single client.
 * Returns MQTT Client instance pointer which is used for further MQTT client
 * operations
 *
 *
 */

rsi_mqtt_client_info_t * rsi_mqtt_client_init( int8_t *buffer, uint32_t length, int8_t *server_ip, uint32_t server_port, uint32_t client_port, uint16_t flags, uint16_t keep_alive_interval)
{
  rsi_mqtt_client_info_t *rsi_mqtt_client  = NULL;

  //! If any invalid parameter is given , return NULL
  if( !(buffer && length && server_port && client_port && server_port) )
  {
    //! return invalid command error
    return NULL;
  }
  //! Given buffer length for MQTT client information is not sufficient
  if(length < MQTT_CLIENT_INFO_SIZE)
  {
    return NULL;
  }

  rsi_mqtt_client = (rsi_mqtt_client_info_t *)buffer;

  buffer += sizeof(rsi_mqtt_client_info_t);

	rsi_mqtt_client->mqtt_client.ipstack = (Network *)buffer;

  buffer += sizeof(Network);

  rsi_mqtt_client->server_port = server_port;

  rsi_mqtt_client->client_port = client_port;

  rsi_mqtt_client->keep_alive_interval  = keep_alive_interval;

  if(flags & RSI_IPV6)
  {
    memcpy(&rsi_mqtt_client->server_ip.ipv6[0], server_ip, RSI_IPV6_ADDRESS_LENGTH);
  }
  else
  {
    //! Fill IP address
    memcpy(&rsi_mqtt_client->server_ip.ipv4[0], server_ip, RSI_IPV4_ADDRESS_LENGTH);
  }

  rsi_mqtt_client->mqtt_tx_buffer = buffer;

  buffer += MQTT_CLIENT_TX_BUFFER_SIZE;

  rsi_mqtt_client->mqtt_rx_buffer = buffer;

  buffer += MQTT_CLIENT_RX_BUFFER_SIZE;

  //! Initialisation for creating new network(initialse network callbacks)
  NewNetwork(rsi_mqtt_client->mqtt_client.ipstack);

	//! Initialise buffer to the MQTT client
	MQTTClient((Client *)rsi_mqtt_client,rsi_mqtt_client->mqtt_client.ipstack, MQTT_CONNECT_TIME_OUT, (uint8_t *)rsi_mqtt_client->mqtt_tx_buffer, MQTT_CLIENT_TX_BUFFER_SIZE, (uint8_t *)rsi_mqtt_client->mqtt_rx_buffer, MQTT_CLIENT_RX_BUFFER_SIZE);

  return (rsi_mqtt_client);


}


/*==============================================*/
/**
 * @fn          int32_t rsi_mqtt_connect(rsi_mqtt_client_info_t *rsi_mqtt_client, uint16_t flags, int8_t *client_id,int8_t *username,int8_t *password)
 * @brief       connects to MQTT broker  
 * @param[in]   rsi_mqtt_client, MQTT client information pointer which was returned in rsi_mqtt_client_init() API
 * @param[in]   flags, Network flags,Each bit has its own significance
 *              Bit(0) -IP version 
 *                 0 - IPv4 
 *                 1 - IPv6 
 *              Bit(1) - Enable SSL
 *                 0 - Disable SSL
 *                 1 - Enable SSL
 * @param[in]   client_id , clientID of the MQTT Client and should be unique for each device   
 * @param[in]   username , Username for the login credentials of MQTT server   
 * @param[in]   password , password for the login credentials of MQTT server   
 * @return      int32_t 
 *              0  =  success
 *              <0 = failure 
 * @section description
 * This API establishes TCP connection with the given MQTT client port and establishes
 * MQTT protocol level connection
 *
 */
#ifdef ASYNC_MQTT
int32_t rsi_mqtt_connect(rsi_mqtt_client_info_t *rsi_mqtt_client, uint16_t flags, int8_t *client_id,int8_t *username,int8_t *password, void (*callback)(uint32_t sock_no, uint8_t *buffer, uint32_t length))
#else
int32_t rsi_mqtt_connect(rsi_mqtt_client_info_t *rsi_mqtt_client, uint16_t flags, int8_t *client_id,int8_t *username,int8_t *password)
#endif
{

  int32_t status = 0;

  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

	if((rsi_mqtt_client == NULL )|| (client_id ==  NULL))
	{
		//! return invalid parmater error
		return RSI_ERROR_INVALID_PARAM;
	}
	//! Connect to the new network
#ifdef ASYNC_MQTT
	status = ConnectNetwork((Network *)&rsi_mqtt_client->mqtt_client, flags, (char *)&(rsi_mqtt_client->server_ip), rsi_mqtt_client->server_port, rsi_mqtt_client->client_port, callback);
#else
	status = ConnectNetwork((Network *)&rsi_mqtt_client->mqtt_client, flags, (char *)&(rsi_mqtt_client->server_ip), rsi_mqtt_client->server_port, rsi_mqtt_client->client_port);
#endif
	if(status != 0)
	{
		return status;
	}

  data.willFlag = 0;
  //! MQTT Version
  data.MQTTVersion = MQTT_VERSION;
  //! Assign client ID
	data.clientID.cstring = (char *)client_id;

  //! Fill username and password
  if(username != NULL)
		data.username.cstring = (char *)username;

  if(password != NULL)
		data.password.cstring = (char *)password;
  //! Keep Alive interval
  data.keepAliveInterval = rsi_mqtt_client->keep_alive_interval;
  //! New connection
  data.cleansession = 1;

  //! Connect to  the MQTT broker
  status = MQTTConnect(&rsi_mqtt_client->mqtt_client, &data);

  //! Shut Down the port
  if(status) 
    mqtt_disconnect(rsi_mqtt_client->mqtt_client.ipstack);


  return status;

}


/*==============================================*/
/**
 * @fn         int32_t rsi_mqtt_disconnect(rsi_mqtt_client_info_t  *rsi_mqtt_client)
 * @brief      Disconnects from MQTT Broker  
 * @param[in]  rsi_mqtt_client,MQTT client information structure pointer which was returned in rsi_mqtt_client_init() API
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This function is used to disconnect the client from MQTT broker
 *
 */
int32_t rsi_mqtt_disconnect(rsi_mqtt_client_info_t  *rsi_mqtt_client)
{
  int32_t status = 0;
  //! If MQTT info structure is NULL ,throw error
  if(rsi_mqtt_client == NULL)
  {
    //! return invalid command error
    return RSI_ERROR_INVALID_PARAM;
  }
  //! Call MQTT disconnect
  status = MQTTDisconnect(&rsi_mqtt_client->mqtt_client);
  //! Shut Down the port
  mqtt_disconnect(rsi_mqtt_client->mqtt_client.ipstack);

  return status;
}


/*==============================================*/
/**
 * @fn         int32_t rsi_mqtt_publish(rsi_mqtt_client_info_t *rsi_mqtt_client, int8_t *topic, MQTTMessage *publish_msg)
 * @brief      publishes the message given on the topic specified 
 * @param[in]  MQTT client info structure which was returned in rsi_mqtt_client_init() API
 * @param[in]  topic, string of topic 
 * @param[in]  message strcuture, message to publish 
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * The API is used to publish the given message on the given topic
 *
 *
 */
int32_t rsi_mqtt_publish(rsi_mqtt_client_info_t *rsi_mqtt_client, int8_t *topic, MQTTMessage *publish_msg)
{
  int32_t status = 0;


  //! If any invalid parameter is received
  if((rsi_mqtt_client == NULL) || (topic == NULL) ||(publish_msg == NULL))
  {
    //! return invalid command parameter error
    return RSI_ERROR_INVALID_PARAM;
  }


  //! Publish the message
	status = MQTTPublish(&rsi_mqtt_client->mqtt_client, (const char *)topic, (MQTTMessage *)publish_msg);

  //! Return Status
  return status;
}



/*==============================================*/
/**
 * @fn         int32_t rsi_mqtt_subscribe(rsi_mqtt_client_info_t *rsi_mqtt_client,uint8_t qos, int8_t *topic,void (*call_back_handler_ptr)(MessageData* md))
 * @brief      Subscribes to the topic given 
 * @param[in]  rsi_mqtt_client, MQTT client structure info pointer which was returned in rsi_mqtt_client_init() API
 * @param[in]  qos, Quality of service of the message 
 * @param[in]  topic, topic string 
 * @param[in]  call_back_handler_ptr , call back pointer to call when a message is received from MQTT broker
 * @return     int32_t
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This API is used to subscribe on the specified topic given.
 * If any other client posts any message on the same topic,
 * That message is received if MQTT client is subscribed to that
 * topic
 */
int32_t rsi_mqtt_subscribe(rsi_mqtt_client_info_t *rsi_mqtt_client,uint8_t qos, int8_t *topic,void (*call_back_handler_ptr)(MessageData* md))
{
  int32_t status = 0;

	//! If any invalid parameter is received
#ifdef ASYNC_MQTT
	if((rsi_mqtt_client == NULL) || (topic == NULL))
#else
	if((rsi_mqtt_client == NULL) || (topic == NULL) || (call_back_handler_ptr == NULL))
#endif
	{
		//! return invalid parameter error
		return RSI_ERROR_INVALID_PARAM;
	}

  if(qos > 2)
  {
    //! return invalid parameter error
    return RSI_ERROR_INVALID_PARAM;
  }
	status = MQTTSubscribe(&rsi_mqtt_client->mqtt_client, (const char *)topic, (enum QoS)qos, call_back_handler_ptr);

  return status;
}




/*==============================================*/
/**
 * @fn         int32_t rsi_mqtt_unsubscribe( rsi_mqtt_client_info_t *rsi_mqtt_client, int8_t *topic)
 * @brief      Unsubscribes to the topic given 
 * @param[in]  MQTT client instance which was returned in rsi_mqtt_client_init() API
 * @param[in]  topic, topic string 
 * @return     int32_t 
 *             0  =  success
 *             <0 = failure 
 * @section description
 * This API is used to unsubscribe on the specified topic given.
 * if unsubscribed, any messages on the topic is not received further
 */

int32_t rsi_mqtt_unsubscribe( rsi_mqtt_client_info_t *rsi_mqtt_client, int8_t *topic)
{
  int32_t status;

  if((rsi_mqtt_client == NULL) || (topic == NULL))
  {
    //! return invalid command error
    return RSI_ERROR_INVALID_PARAM;
  }
  //! Unsubscribe to the topic
  status = MQTTUnsubscribe(&rsi_mqtt_client->mqtt_client, (const char *)topic);

  return status;

}


/*==============================================*/
/**
 * @fn         int32_t rsi_mqtt_poll_for_recv_data(rsi_mqtt_client_info_t *rsi_mqtt_client, uint16_t time_out)
 * @brief      Waits for the messages to the subscribed topics
 * @param[in]  MQTT client instance which was returned in rsi_mqtt_client_init() API
 * @param[in]  time_out, time out
 * @return     int32_t
 *             0  =  success
 *             <0 = failure
 * @section description
 * Waits for the MQTT messages to recv on the specific MQTT client 
 *
 */
int32_t rsi_mqtt_poll_for_recv_data(rsi_mqtt_client_info_t *rsi_mqtt_client, uint16_t time_out)
{
  if(rsi_mqtt_client == NULL)
  {
    //! return invalid command error
    return RSI_ERROR_INVALID_PARAM;
  }

  return MQTTYield(&rsi_mqtt_client->mqtt_client, time_out);
}
