 /*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <string.h>
#include <timer_platform.h>
#include <network_interface.h>
#include "aws_iot_error.h"
#include "aws_iot_log.h"
#include "network_interface.h"
#include "network_platform.h"
#include "rsi_socket.h"
#include "rsi_wlan_apis.h"
#include "rsi_common_apis.h"
#include "rsi_data_types.h"
#include "rsi_error.h"
#include "rsi_wlan.h"
#include "rsi_nwk.h"


/* This is the value used for ssl read timeout */
#define IOT_SSL_READ_TIMEOUT 10

/* This defines the value of the debug buffer that gets allocated.
 * The value can be altered based on memory constraints
 */
#ifdef ENABLE_IOT_DEBUG
#define MBEDTLS_DEBUG_BUFFER_SIZE 2048
#endif

//! Client port number
#define CLIENT_PORT       4002

#define AWS_DOMAIN_NAME  "a25jwtlmds8eip-ats.iot.us-east-2.amazonaws.com"

/*
 * This is a function to do further verification if needed on the cert received
 */

//static int _iot_tls_verify_cert(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags) {
static int _iot_tls_verify_cert(void *data, int *crt, int depth, uint32_t *flags)
{
	return 0;
}

void _iot_tls_set_connect_params(Network *pNetwork, char *pRootCALocation, char *pDeviceCertLocation,
		char *pDevicePrivateKeyLocation, char *pDestinationURL,
		uint16_t destinationPort, uint32_t timeout_ms, bool ServerVerificationFlag)
{
	pNetwork->tlsConnectParams.DestinationPort = destinationPort;
	pNetwork->tlsConnectParams.pDestinationURL = pDestinationURL;
	pNetwork->tlsConnectParams.pDeviceCertLocation = pDeviceCertLocation;
	pNetwork->tlsConnectParams.pDevicePrivateKeyLocation = pDevicePrivateKeyLocation;
	pNetwork->tlsConnectParams.pRootCALocation = pRootCALocation;
	pNetwork->tlsConnectParams.timeout_ms = timeout_ms;
	pNetwork->tlsConnectParams.ServerVerificationFlag = ServerVerificationFlag;
}

IoT_Error_t iot_tls_init(Network *pNetwork, char *pRootCALocation, char *pDeviceCertLocation,
		char *pDevicePrivateKeyLocation, char *pDestinationURL,
		uint16_t destinationPort, uint32_t timeout_ms, bool ServerVerificationFlag)
{
	_iot_tls_set_connect_params(pNetwork, pRootCALocation, pDeviceCertLocation, pDevicePrivateKeyLocation,
			pDestinationURL, destinationPort, timeout_ms, ServerVerificationFlag);

	pNetwork->connect = iot_tls_connect;

	pNetwork->read = iot_tls_read;
	pNetwork->write = iot_tls_write;
	pNetwork->disconnect = iot_tls_disconnect;
	pNetwork->isConnected = iot_tls_is_connected;
	pNetwork->destroy = iot_tls_destroy;

	return SUCCESS;
}

IoT_Error_t iot_tls_is_connected(Network *pNetwork) {
	/* Use this to add implementation which can check for physical layer disconnect */
	return NETWORK_PHYSICAL_LAYER_CONNECTED;
}


int ConnecttoNetwork(Network* n, uint8_t flags,char* addr, int dst_port, int src_port)
{
	int type = SOCK_STREAM;
	struct rsi_sockaddr_in address,clientAddr;
	struct rsi_sockaddr_in6 address_v6,clientAddr_v6;
	int rc = -1,status = 0;

	memset(&address, 0, sizeof(address));
	memset(&address_v6, 0, sizeof(address_v6));

	if(flags == RSI_IPV6)
	{
		address_v6.sin6_family = AF_INET6;

		address_v6.sin6_port = htons(dst_port);

		memcpy(&address_v6.sin6_addr._S6_un._S6_u8, addr, RSI_IPV6_ADDRESS_LENGTH);

		if(flags & RSI_SSL_ENABLE)
			n->socket_id = rsi_socket(AF_INET6, type, RSI_SOCKET_FEAT_SSL);
		else
			n->socket_id = rsi_socket(AF_INET6, type,0);

		clientAddr_v6.sin6_family= AF_INET6;
		clientAddr_v6.sin6_port = htons(src_port);
	}
	else
	{
		//! Set family type
		address.sin_family= AF_INET;

		//! Set local port number
		address.sin_port = htons(dst_port);


		address.sin_addr.s_addr = rsi_bytes4R_to_uint32((uint8_t *)addr);

		if(flags & RSI_SSL_ENABLE)
			n->socket_id = rsi_socket(AF_INET, type, RSI_SOCKET_FEAT_SSL);
		else
			n->socket_id = rsi_socket(AF_INET, type, 0);

		clientAddr.sin_family= AF_INET;
		clientAddr.sin_port = htons(src_port);

		/* Set all bits of the padding field to 0 */
		memset(clientAddr.sin_zero, '\0', sizeof(clientAddr.sin_zero));
	}
	if (n->socket_id == -1)
	{
		status = rsi_wlan_get_status();
		return status;
	}

	if(flags == RSI_IPV6)
	{
		//! Bind socket
		status = rsi_bind(n->socket_id, (struct rsi_sockaddr *) &clientAddr_v6, sizeof(clientAddr_v6));

	}
	else
	{
		//! Bind socket
		status = rsi_bind(n->socket_id, (struct rsi_sockaddr *) &clientAddr, sizeof(clientAddr));


	}
	if(status != 0)
	{
		status = rsi_wlan_get_status();
		//! Shut Down the port
		//  mqtt_disconnect(n);
		return status;
	}
	if(flags == RSI_IPV6)
	{
		rc = rsi_connect(n->socket_id, (struct rsi_sockaddr*)&address_v6, sizeof(address_v6));
	}
	else
	{
		rc = rsi_connect(n->socket_id, (struct rsi_sockaddr*)&address, sizeof(address));

	}
	if(rc == -1)
	{
		status = rsi_wlan_get_status();
		//! Shut Down the port
		//mqtt_disconnect(n);
	}
	return status;
}
IoT_Error_t iot_tls_connect(Network *pNetwork, TLSConnectParams *params)
{
	int ret = 0;
	const char *pers = "aws_iot_tls_wrapper";
	int32_t     status       = 0;

	rsi_rsp_dns_query_t dns_query_rsp;
	uint32_t    server_address =  0;
	do{

		status = rsi_dns_req(RSI_IP_VERSION_4, (uint8_t *)pNetwork->tlsConnectParams.pDestinationURL, NULL, NULL, &dns_query_rsp, sizeof(dns_query_rsp));

	}while(status != RSI_SUCCESS);

	server_address = rsi_bytes4R_to_uint32(dns_query_rsp.ip_address[0].ipv4_address);

	status =   ConnecttoNetwork(pNetwork, 2,(char *) &server_address, pNetwork->tlsConnectParams.DestinationPort, CLIENT_PORT);       //fixme:flags kept as 0

	if(status != 0)
	{
		return status;
	}

	return status;
}

IoT_Error_t iot_tls_write(Network *pNetwork, unsigned char *pMsg, size_t len, Timer *timer, size_t *written_len)
{
	size_t written_so_far;
	bool isErrorFlag = false;
	int frags;
	int ret = 0;
	for(written_so_far = 0, frags = 0;
			written_so_far < len && !has_timer_expired(timer); written_so_far += ret, frags++)
	{
		while(!has_timer_expired(timer) &&
				(ret = rsi_send(pNetwork->socket_id,(int8_t *)(pMsg + written_so_far), len - written_so_far,0)) <= 0)      //FIXME:flags parameter kept as 0
		{
			isErrorFlag = true;
		}
		if(isErrorFlag)
		{
			break;
		}
	}

	*written_len = written_so_far;

	if(isErrorFlag)
	{
		return NETWORK_SSL_WRITE_ERROR;
	} else if(has_timer_expired(timer) && written_so_far != len)
	{
		return NETWORK_SSL_WRITE_TIMEOUT_ERROR;
	}
	return SUCCESS;
}

IoT_Error_t iot_tls_read(Network *pNetwork, unsigned char *pMsg, size_t len, Timer *timer, size_t *read_len)
{
	size_t rxLen = 0;
	int ret;
	while (len > 0)
	{
		// This read will timeout after IOT_SSL_READ_TIMEOUT if there's no data to be read
		ret = rsi_recv(pNetwork->socket_id,pMsg, len, 0);  //FIXME:flags parameter kept as 0
		if (ret > 0)
		{
			rxLen += ret;
			pMsg += ret;
			len -= ret;
		}
		// Evaluate timeout after the read to make sure read is done at least once
		if (has_timer_expired(timer))
		{
			break;
		}
	}

	if (len == 0)
	{
		*read_len = rxLen;
		return SUCCESS;
	}

	if (rxLen == 0)
	{
		return NETWORK_SSL_NOTHING_TO_READ;
	} else
	{
		return NETWORK_SSL_READ_TIMEOUT_ERROR;
	}

}

IoT_Error_t iot_tls_disconnect(Network *pNetwork)
{
	rsi_shutdown(pNetwork->socket_id,0);
	return SUCCESS;
}

IoT_Error_t iot_tls_destroy(Network *pNetwork)
{
	return SUCCESS;
}

#ifdef __cplusplus
}
#endif
