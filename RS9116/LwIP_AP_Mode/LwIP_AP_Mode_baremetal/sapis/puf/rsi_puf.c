/**
 * @file     rsi_puf.c
 * @version  0.1
 * @date     15 Dec 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains PUF APIs
 *
 *  @section Description  This file contains PUF APIs
 *
 *
 */
#include "rsi_driver.h"
#include "rsi_puf.h"

#ifdef RSI_PUF_ENABLE


/*==============================================*/
/**
 * @fn         int32_t  rsi_puf_start_req(void)
 * @brief      Start PUF if earlier enrolled
 * @param[in]  None
 * @param[out]
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Starts PUF
 *
 */
int32_t  rsi_puf_start_req(void)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                    *host_desc    = NULL;
	uint16_t                    send_size    = 0;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;


	if(wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE)
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	//! Using host descriptor to set payload length
	send_size = 0;

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send PUF start request command
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PUF_START, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         int32_t  rsi_puf_set_key_req(uint8_t key_index, uint8_t key_size,
 * 						uint8_t *key_ptr, uint8_t *set_key_resp, uint16_t length)
 * @brief      Set Key for PUF
 * @param[in]  Key Index
 * @param[in]  Key Size : 1 - 256bit, 0 - 128bit
 * @param[in]  Pointer to Key
 * @param[out] Pointer to response
 * @param[out]  Length of response buffer
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Does set key operation on key provided
 *
 */
int32_t  rsi_puf_set_key_req(uint8_t key_index, uint8_t key_size, uint8_t *key_ptr, uint8_t *set_key_resp, uint16_t length)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                   *host_desc    = NULL;
	uint16_t                  send_size    = 0;
	rsi_req_puf_set_key_t 	*set_key;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	if((wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE) || (rsi_common_cb->puf_state < RSI_PUF_STATE_STARTED))
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! Parameter check
	if ((key_index > 15) || (key_size  > 1) || (set_key_resp == NULL))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	//! Get Data Pointer
	set_key = (rsi_req_puf_set_key_t *)pkt->data;

	if(set_key_resp != NULL)
	{
		//! attach the buffer given by user
		rsi_common_cb->app_buffer = set_key_resp;

		//! length of the buffer provided by user
		rsi_common_cb->app_buffer_length = length;
	}

	//! Memset before filling
	memset(set_key, 0, sizeof(rsi_req_puf_set_key_t));

	//! Assign key_index
	set_key->key_index = key_index;

	//! Assign Key size
	set_key->key_size = key_size;

	//! Copy data to be sent
	memcpy(&set_key->key[0], key_ptr, (key_size + 1) * 16);

	//! Using host descriptor to set payload length
	send_size = sizeof(rsi_req_puf_set_key_t); //FIXME size should be based on key_size

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send PUF Set Key request command
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PUF_SET_KEY, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         int32_t  rsi_puf_set_key_disable_req(void)
 * @brief      Block Set key operation on PUF
 * @param[in]  None
 * @param[out]
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Blocks further set key operation on PUF
 *
 */
int32_t  rsi_puf_set_key_disable_req(void)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                    *host_desc    = NULL;
	uint16_t                    send_size    = 0;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	
	if(wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE)
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	//! Using host descriptor to set payload length
	send_size = 0;

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send disable set key operation request command
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PUF_DIS_SET_KEY, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         int32_t  rsi_puf_get_key_req(uint8_t *keycode_ptr, uint8_t *get_key_resp,
 * 											uint16_t length)
 * @brief      Get Key using PUF
 * @param[in]  Pointer to KeyCode
 * @param[out] Pointer to response
 * @param[out]  Length of response buffer
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Does Get key operation with keycode provided
 *
 */
int32_t  rsi_puf_get_key_req(uint8_t *keycode_ptr, uint8_t *get_key_resp, uint16_t length)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                   *host_desc    = NULL;
	uint16_t                  send_size    = 0;
	rsi_req_puf_get_key_t 	*get_key;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	if((wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE) || (rsi_common_cb->puf_state < RSI_PUF_STATE_STARTED))
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! Parameter check
	if ((keycode_ptr == NULL) || (get_key_resp == NULL))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	//! Get Data Pointer
	get_key = (rsi_req_puf_get_key_t *)pkt->data;

	if(get_key_resp != NULL)
	{
		//! attach the buffer given by user
		rsi_common_cb->app_buffer = get_key_resp;

		//! length of the buffer provided by user
		rsi_common_cb->app_buffer_length = length;
	}
	//! Memset before filling
	memset(get_key, 0, sizeof(rsi_req_puf_get_key_t));

	//! Copy data to be sent
	memcpy(&get_key->key_code[0], keycode_ptr, KEY_CODE_SIZE_BYTES);

	//! Using host descriptor to set payload length
	send_size = sizeof(rsi_req_puf_get_key_t); //FIXME size should be based on key_size

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send get key request to PUF
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PUF_GET_KEY, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         int32_t  rsi_puf_get_key_disable_req(void)
 * @brief      Block get key operation
 * @param[in]  None
 * @param[out]
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Blocks further get key operation on PUF
 *
 */
int32_t  rsi_puf_get_key_disable_req(void)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                    *host_desc    = NULL;
	uint16_t                    send_size    = 0;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	
	if(wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE)
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	//! Using host descriptor to set payload length
	send_size = 0;

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send disable get key operation on PUF
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PUF_DIS_GET_KEY, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         int32_t  rsi_puf_load_key_req(uint8_t *keycode_ptr)
 * @brief      Load Key using PUF to AES engine
 * @param[in]  Pointer to KeyCode
 * @param[out] None
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Loads key into PUF
 *
 */
int32_t  rsi_puf_load_key_req(uint8_t *keycode_ptr)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                   *host_desc    = NULL;
	uint16_t                  send_size    = 0;
	rsi_req_puf_load_key_t 	*load_key;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	if((wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE) || (rsi_common_cb->puf_state < RSI_PUF_STATE_STARTED))
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! Parameter check
	if (keycode_ptr == NULL)
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	//! Get Data Pointer
	load_key = (rsi_req_puf_load_key_t *)pkt->data;

	//! Memset before filling
	memset(load_key, 0, sizeof(rsi_req_puf_load_key_t));

	//! Copy data to be sent
	memcpy(&load_key->key_code[0], keycode_ptr, KEY_CODE_SIZE_BYTES);

	//! Using host descriptor to set payload length
	send_size = sizeof(rsi_req_puf_load_key_t); //FIXME size should be based on key_size

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send load key request to PUF
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PUF_LOAD_KEY, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         rsi_puf_aes_encrypt_req(uint8_t mode, uint8_t key_source, uint16_t key_size,
 * 					uint8_t *key_ptr, uint16_t data_size, uint8_t *data_ptr, uint16_t iv_size,
 * 					uint8_t *iv_ptr,uint8_t *aes_encry_resp, uint16_t length)
 * @brief      Encrypt using AES
 * @param[in]  AES Mode : 1 - CBC, 0 - EBC
 * @param[in]  Key Source : 0 - PUF, 1 - AES
 * @param[in]  Key Size : 1 - 256bit, 0 - 128bit
 * @param[in]  Pointer to Key
 * @param[in]  Data Size : size of data in bytes
 * @param[in]  Pointer to Data
 * @param[in]  IV Size : 1 - 256bit, 0 - 128bit
 * @param[in]  Pointer to IV
 * @param[out] Pointer to response
 * @param[out] Length of response buffer
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Encrypts data using IV, Key provided depending on mode
 *
 */
int32_t  rsi_puf_aes_encrypt_req(uint8_t mode, uint8_t key_source, uint16_t key_size, uint8_t *key_ptr, uint16_t data_size, uint8_t *data_ptr, uint16_t iv_size, uint8_t *iv_ptr,uint8_t *aes_encry_resp, uint16_t length)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                   *host_desc    = NULL;
	uint16_t                  send_size    = 0;
	rsi_req_aes_encrypt_t 	*aes_encry;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	if((wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE) || (rsi_common_cb->puf_state < RSI_PUF_STATE_STARTED))
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! Response pointer/length check
	if((aes_encry_resp == NULL) || (data_size != length))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	if((mode == AES_CBC_MODE) && (iv_ptr == NULL))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	if((key_size > 1) || (iv_size > 1) || (data_size > 1400) || (data_size < 16) || (mode > 1))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	//Check data size
	if(data_size % ( (key_size+1) * 16))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	if(aes_encry_resp != NULL)
	{
		//! attach the buffer given by user
		rsi_common_cb->app_buffer = aes_encry_resp;

		//! length of the buffer provided by user
		rsi_common_cb->app_buffer_length = length;
	}

	//! Get Data Pointer
	aes_encry = (rsi_req_aes_encrypt_t *)pkt->data;

	//! Memset before filling
	memset(aes_encry, 0, sizeof(rsi_req_aes_encrypt_t));

	if(mode == AES_CBC_MODE)
	{
		//! Copy IV
		memcpy(&aes_encry->iv[0], iv_ptr, ((iv_size + 1) * 16));
	}

	//Copy Key
	if(key_source == AES_AS_KEY_SOURCE)
	{
		memcpy(&aes_encry->key[0], key_ptr, (key_size + 1) * 16);
	}

	//Fill AES Encry Mode
	aes_encry->mode = (mode | (key_size << 1) | (key_source << 2) | (iv_size << 3));

	//! Copy Data
	memcpy(aes_encry->data, data_ptr, data_size);

	//Fill length
	rsi_uint16_to_2bytes(aes_encry->data_size, data_size);

	//! Using host descriptor to set payload length
	send_size = AES_CMDS_SIZE_WTH_OUT_DATA + data_size;

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send AES encrypt request command
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_AES_ENCRYPT, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         rsi_puf_aes_decrypt_req(uint8_t mode, uint8_t key_source, uint16_t key_size,
 * 					uint8_t *key_ptr, uint16_t data_size, uint8_t *data_ptr, uint16_t iv_size,
 * 					uint8_t *iv_ptr,uint8_t *aes_decry_resp, uint16_t length)
 * @brief      Decrypt using AES
 * @param[in]  AES Mode : 1 - CBC, 0 - EBC
 * @param[in]  Key Source : 0 - PUF, 1 - AES
 * @param[in]  Key Size : 1 - 256bit, 0 - 128bit
 * @param[in]  Pointer to Key
 * @param[in]  Data Size : size of data in bytes
 * @param[in]  Pointer to Encrypted Data
 * @param[in]  IV Size : 1 - 256bit, 0 - 128bit
 * @param[in]  Pointer to IV
 * @param[out] Pointer to response
 * @param[out] Length of response buffer
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Decrypts data using IV, Key provided depending on mode
 *
 */
int32_t  rsi_puf_aes_decrypt_req(uint8_t mode, uint8_t key_source, uint16_t key_size, uint8_t *key_ptr, uint16_t data_size, uint8_t *data_ptr, uint16_t iv_size, uint8_t *iv_ptr,uint8_t *aes_decry_resp, uint16_t length)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                   *host_desc    = NULL;
	uint16_t                  send_size    = 0;
	rsi_req_aes_decrypt_t 	*aes_decry;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	if((wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE) || (rsi_common_cb->puf_state < RSI_PUF_STATE_STARTED))
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! Response pointer/length check
	if((aes_decry_resp == NULL) || (data_size != length))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	if((mode == AES_CBC_MODE) && (iv_ptr == NULL))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	if((key_size > 1) || (iv_size > 1) || (data_size > 1400) || (data_size < 16) || (mode > 1))
	{
		return RSI_ERROR_INVALID_PARAM;
	}
	if((key_source == AES_AS_KEY_SOURCE) && (key_ptr == NULL))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	//Check data size
	if(data_size%((key_size+1)*16))
		return RSI_ERROR_INVALID_PARAM;

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}
	if(aes_decry_resp != NULL)
	{
		//! attach the buffer given by user
		rsi_common_cb->app_buffer = aes_decry_resp;

		//! length of the buffer provided by user
		rsi_common_cb->app_buffer_length = length;
	}
	//! Get Data Pointer
	aes_decry = (rsi_req_aes_decrypt_t *)pkt->data;

	//! Memset before filling
	memset(aes_decry, 0, sizeof(rsi_req_aes_decrypt_t));

	if(mode == AES_CBC_MODE)
	{
		//! Copy IV
		memcpy(&aes_decry->iv[0], iv_ptr, ((iv_size + 1) * 16));
	}

	if(key_source == AES_AS_KEY_SOURCE)
	{
		//! Copy Key
		memcpy(&aes_decry->key[0], key_ptr, (key_size + 1) * 16);
	}

	//Fill AES Encry Mode
	aes_decry->mode = (mode | (key_size << 1) | (key_source << 2) | (iv_size << 3));
	//! Memset before filling
	memset(&aes_decry->data[0], 0, MAX_DATA_SIZE_BYTES);

	//! Copy Data
	memcpy(&aes_decry->data[0], data_ptr, data_size);

	rsi_uint16_to_2bytes(aes_decry->data_size, data_size);

	//! Using host descriptor to set payload length
	send_size = AES_CMDS_SIZE_WTH_OUT_DATA + data_size;

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send data decrypt request command
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_AES_DECRYPT, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         rsi_puf_aes_mac_req(uint8_t key_source, uint16_t key_size, uint8_t *key_ptr, uint16_t data_size,
 * 								 uint8_t *data_ptr, uint16_t iv_size, uint8_t *iv_ptr,
 * 								 uint8_t *aes_mac_resp, uint16_t length)
 * @brief      Calculate AES CBC mode MAC
 * @param[in]  Key Source : 0 - PUF, 1 - AES
 * @param[in]  Key Size : 1 - 256bit, 0 - 128bit
 * @param[in]  Pointer to Key
 * @param[in]  Data Size : size of data in bytes
 * @param[in]  Pointer to Data
 * @param[in]  IV Size : 1 - 256bit, 0 - 128bit
 * @param[in]  Pointer to IV
 * @param[out] Pointer to response
 * @param[out] Length of response buffer
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Cacluates MAC for given data using AES-CBC mode
 *
 */
int32_t  rsi_puf_aes_mac_req(uint8_t key_source, uint16_t key_size, uint8_t *key_ptr, uint16_t data_size, uint8_t *data_ptr, uint16_t iv_size, uint8_t *iv_ptr,uint8_t *aes_mac_resp, uint16_t length)
{

	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                   *host_desc    = NULL;
	uint16_t                  send_size    = 0;
	rsi_req_aes_mac_t 	*aes_mac;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	if((wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE) || (rsi_common_cb->puf_state < RSI_PUF_STATE_STARTED))
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! Response pointer/length check
	if(aes_mac_resp == NULL)
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	if((key_source == AES_AS_KEY_SOURCE) && (key_ptr == NULL))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	//Check data size
	if(data_size % ( (key_size + 1) * 16))
		return RSI_ERROR_INVALID_PARAM;

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	if(aes_mac_resp != NULL)
	{
		//! attach the buffer given by user
		rsi_common_cb->app_buffer = aes_mac_resp;

		//! length of the buffer provided by user
		rsi_common_cb->app_buffer_length = length;
	}

	//! Get Data Pointer
	aes_mac = (rsi_req_aes_mac_t *)pkt->data;

	//! Memset before filling
	memset(aes_mac, 0, sizeof(rsi_req_aes_mac_t));

	//Fill AES Encry Mode
	aes_mac->mode = ( (key_size << 1) | (key_source << 2) | (iv_size << 3));

	//Fill length
	rsi_uint16_to_2bytes(aes_mac->data_size, data_size);

	//!Data
	//! Memset before filling
	memset(&aes_mac->data[0], 0, MAX_DATA_SIZE_BYTES);

	//! Copy Data
	memcpy(&aes_mac->data[0], data_ptr, data_size);

	//! Key
	//! Memset before filling
	memset(&aes_mac->key[0], 0, MAX_KEY_SIZE_BYTES);
	
	if(key_source == AES_AS_KEY_SOURCE)
	{
		//! Copy Key
		memcpy(&aes_mac->key[0], key_ptr, (key_size + 1) * 16);
	}
	//! IV
	//! Memset before filling
	memset(&aes_mac->iv[0], 0, MAX_KEY_SIZE_BYTES);

	//! Copy Data
	memcpy(&aes_mac->iv[0], iv_ptr, (iv_size + 1) * 16);

	//! Using host descriptor to set payload length
	send_size = AES_CMDS_SIZE_WTH_OUT_DATA + data_size;

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send mac calculation request to module
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_AES_MAC, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

/*==============================================*/
/**
 * @fn         int32_t  rsi_puf_intr_key_req(uint8_t key_index, uint8_t key_size,
 * 					                                 uint8_t *set_key_resp, uint16_t length)
 * @brief      Intrinsic Key for PUF
 * @param[in]  Key Index
 * @param[in]  Key Size : 1 - 256bit, 0 - 128bit
 * @param[out] Pointer to response
 * @param[out]  Length of response buffer
 * @return
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function Does set intrinsic key operation 
 *
 */
int32_t  rsi_puf_intr_key_req(uint8_t key_index, uint8_t key_size, uint8_t *intr_key_resp, uint16_t length)
{
	int32_t status = RSI_SUCCESS;
	rsi_pkt_t                 *pkt;
	uint8_t                   *host_desc    = NULL;
	uint16_t                  send_size    = 0;
	rsi_req_puf_intr_key_t 	  *intr_key;

	//! Get wlan cb structure pointer
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

	//! Get commmon cb pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	if((wlan_cb->state < RSI_WLAN_STATE_OPERMODE_DONE) || (rsi_common_cb->puf_state < RSI_PUF_STATE_STARTED))
	{
		//! Command given in wrong state
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}

	//! Parameter check
	if ((key_index > 15) || (key_size  > 1) || (intr_key_resp == NULL))
	{
		return RSI_ERROR_INVALID_PARAM;
	}

	//! take lock on wlan control block
	rsi_mutex_lock(&rsi_common_cb->common_mutex);

	//! allocate command buffer  from wlan pool
	pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&rsi_common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	if(intr_key_resp != NULL)
	{
		//! attach the buffer given by user
		rsi_common_cb->app_buffer = intr_key_resp;

		//! length of the buffer provided by user
		rsi_common_cb->app_buffer_length = length;
	}

	//! Assign key_index
	intr_key->key_index = key_index;

	//! Assign Key size
	intr_key->key_size = key_size;

		//! Using host descriptor to set payload length
	send_size = sizeof(rsi_req_puf_intr_key_t); //FIXME size should be based on key_size

	//! get the host descriptor
	host_desc = (pkt->desc);

	//! Fill data length in the packet host descriptor
	rsi_uint16_to_2bytes(host_desc, (send_size & 0xFFF));

	//! send PUF Intrinsic Key request command
	status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PUF_INTR_KEY, pkt);

	//! free the packet
	rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&rsi_common_cb->common_mutex);

	//! Return the status
	return status;

}

#endif
