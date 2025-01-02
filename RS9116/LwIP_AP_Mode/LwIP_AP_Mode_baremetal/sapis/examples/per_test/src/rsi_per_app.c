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
 * @brief : This file contains example application for PER Test
 * @section Description :
 * This application demonstrates the different commands to be used for enabling
 * the Master to configure and log the Wireless performance.
 * It also demonstates the commands to be given for different protocols like WLAN,
 * BLE/BLR and also a few common commands for initialization of Wireless System.
 =================================================================================*/

/**
 * Include files
 * */

#include "rsi_per_app.h"

//! Application global parameters.
static rsi_bt_per_stats_t per_stats;
static rsi_bt_ber_cmd_t ber_cmd;
rsi_per_stats_rsp_t *per_stats_rsp;
static char send_buffer[5100] = {0};
static char buffer[1040];

state_completed_e state_completed;

/**
 * @callback   (void rsi_wlan_stats_receive_handler(uint16_t status, const uint8_t *buffer, const uint32_t length))
 * @brief      { call back to receive requested no of WLAN RX Stats and send to UART }
 *
 * @param[in]  status  status
 * @param[in]  buffer  received packet from WLAN transmitter
 * @param[in]  length  received packet length
 */
void rsi_wlan_stats_receive_handler(uint16_t status, const uint8_t *buffer, const uint32_t length)
{
	per_stats_rsp = (rsi_per_stats_rsp_t *)buffer; 														//assigning received stats to a structure
	if(cmd7.no_of_stats != 0){
		//appending stats to send_buffer untill requested no of stats
		sprintf((send_buffer + strlen(send_buffer)),"%.4d" "%.4d" "%.4d",per_stats_rsp->crc_pass,per_stats_rsp->crc_fail,per_stats_rsp->cal_rssi );
		if(cmd7.no_of_stats == 1){
			printf("%s",send_buffer); 																						//sending the total requested no of WLAN stats over UART 
			memset(send_buffer, 0, strlen(send_buffer)); 													//clearing the buffer
		}
		cmd7.no_of_stats--;
	}
}

/**
 * @callback   (int32_t rsi_bt_get_ber_pkt_from_app_to_buff(uint8_t *pkt, uint16_t pkt_len))
 * @brief      { call back to receive BT RX Stats data and send data over UART }
 *
 * @param      pkt      received packet from BT transmitter
 * @param[in]  pkt_len  received packet length
 *
 */
int32_t rsi_bt_get_ber_pkt_from_app_to_buff(uint8_t *pkt, uint16_t pkt_len)
{
	uint16_t ii;
	static uint8_t pkt_id = 0;
	if(cmdH.no_bt_pkts != 0){
		printf("%.2x%.4x",pkt_id,(pkt_len+3)); 																	//sending packet index, pkt_len+header length(packet index 1byte + pkt_len 2bytes) over UART
		memcpy((buffer+strlen(buffer)), pkt, pkt_len); 													//copying the received pkt to a buffer
		for(ii=0; ii<1030; ii++){
			printf("%.2x",buffer[ii]); 																						//sending byte by byte till the maximum BT packet length(1030)
		}
		memset(buffer,0,sizeof(buffer)); 																				// clearing the buffer for next packet
		pkt_id++;
		cmdH.no_bt_pkts--;
	}else{
		pkt_id = 0;
	}
}

/**
 * @fn         (int32_t rsi_send_per_features(void))
 * @brief      { Function to Set pll_mode, rf_type, wireless_mode, enable_ppp, afe_type, powersave_features for WLAN }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_send_per_features(void)
{
	rsi_pkt_t       *pkt;
	rsi_feature_frame_t *rsi_feature_frame;
	int8_t          status = RSI_SUCCESS;
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;  									//! Get common cb pointer
  if(!(state_completed & common_command))
  	return -1;
  if(state_completed & features_enabled)
  	rsi_common_command(); 
  rsi_mutex_lock(&common_cb->common_mutex);  																//! lock the mutex
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);   												//! allocate command buffer
  if(pkt == NULL)   																												//! If allocation of packet fails
  {
    rsi_mutex_unlock(&common_cb->common_mutex);     												//! unlock mutex
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;     														//! return packet allocation failure error
	}
  rsi_feature_frame = (rsi_feature_frame_t *)pkt->data;  										//! Take the user provided data and fill it in feature frame structure
  rsi_feature_frame->pll_mode = cmd2.pll_mode;								 							//! pll mode value
  rsi_feature_frame->rf_type = cmd2.rf_type;  								 							//! rf type
  rsi_feature_frame->wireless_mode = cmd2.wireless_mode;  		 							//! wireless mode
  rsi_feature_frame->enable_ppp = cmd2.enable_ppp;   					 							//! enable ppp
  rsi_feature_frame->afe_type = cmd2.afe_type;  							 							//! afe type
  rsi_feature_frame->feature_enables = cmd2.feature_enables;   							//! feature enables
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_FEATURE_FRAME, pkt);   //! send features frame command to Wireless Processor
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);   													//! free the packet
  rsi_mutex_unlock(&common_cb->common_mutex);   														//! unlock mutex
  state_completed |= features_enabled;
  return status;   																													//! Return the status
}

/**
 * @fn         (int32_t rsi_common_command(void))
 * @brief      { Function to set operational mode for wireless interface }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_common_command(void)
{
	int32_t status = RSI_SUCCESS;
	status = rsi_wireless_deinit();
	SystemCoreClockUpdate();
	status = rsi_wireless_init(cmd1.oper_mode, cmd1.coex_mode);							  //! Wireless initialization
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWireless Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWireless Initialization Success\r\n");
	}
	state_completed = common_command; 																				//flag to know the current state
	return status;
}

/**
 * @fn         (int32_t rsi_wlan_per_tx_start(void))
 * @brief      { Function to start WLAN transmission }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_wlan_per_tx_start(void)
{
	int32_t     status       = RSI_SUCCESS;
	
	if(!(state_completed & features_enabled)){ 																//send default feature frame if not sent earlier
		status = rsi_send_per_features();
		if(status == -1)
			return status;
	}

	//! transmit test start with given parameters
	status = rsi_transmit_test_start(cmd3.power, cmd3.rate, cmd3.length, cmd3.mode, cmd3.channel);
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStart Transmit Test Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStart Transmit Test Success\r\n");
	}
	state_completed |= wlan_tx_start; 																				//flag to know the current state
	return status;
}

/**
 * @fn         (int32_t rsi_wlan_per_tx_stop(void))
 * @brief      { Function to stop WLAN transmission }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_wlan_per_tx_stop(void)
{
	int32_t     status       = RSI_SUCCESS;

	if(!(state_completed & common_command))
		return -1;
	status = rsi_transmit_test_stop();  																			//! Stop TX transmit
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStop Transmit Test Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStop Transmit Test Success\r\n");
	}
	state_completed &= ~wlan_tx_start; 																				//flag to know the current state
	return status;
}

/**
 * @fn         (int32_t rsi_receive_start(void))
 * @brief      { Function to enable WLAN receive }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_receive_start(void)
{
	int32_t     status       = RSI_SUCCESS;  
	if(!(state_completed & features_enabled)){
		status = rsi_send_per_features();
		if(status == -1)
			return status;
	}
	status = rsi_wlan_radio_init();																						//! Wlan radio init 
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN Radio Initialization Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWLAN Radio Initialization Success\r\n");
	}

	//! Register Wlan receive stats call back handler
	rsi_wlan_register_callbacks(RSI_WLAN_RECEIVE_STATS_RESPONSE_CB,rsi_wlan_stats_receive_handler);
	status =  rsi_wlan_receive_stats_start(cmd4.channel);											//! Start/Recieve publishing RX stats
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN Start Receiving Stats Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWLAN Start Receiving Stats Success\r\n");
	}
	state_completed |= wlan_rx_start; 																				//flag to know the current state
	return status;
}

/**
 * @fn         (int32_t rsi_receive_stop(void))
 * @brief      { Function to disable WLAN receive }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_receive_stop(void)
{
	int32_t     status       = RSI_SUCCESS;
	status =  rsi_wlan_receive_stats_stop(); 																	//! Stop RX stats  
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nWLAN Stop Receiving Stats Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nWLAN Stop Receiving Stats Success\r\n");
	}
	state_completed &= ~wlan_rx_start; 																				//flag to know the current state
	return status;
}

/**
 * @fn         (int32_t rsi_ble_transmit_start(void))
 * @brief      { Function to start BLE transmission }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_ble_transmit_start(void)
{
	int32_t     status       = RSI_SUCCESS;
	status =  rsi_ble_per_transmit(&cmd8); 																		//start tranmitting requested BLE packets
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStart BLE Transmit Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStart BLE Transmit Success\r\n");
	}
	state_completed |= ble_tx_start; 																					//flag to know the current state
	return status;
}

/**
 * @fn         (int32_t rsi_ble_transmit_stop(void))
 * @brief      { Function to stop BLE transmission }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_ble_transmit_stop(void)
{
	int32_t     status       = RSI_SUCCESS; 
	status =  rsi_ble_per_transmit(&cmd8); 																		//stop transmitting BLE packets
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStop BLE Transmit Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStop BLE Transmit Success\r\n");
	}
	state_completed &= ~ble_tx_start;  																				//flag to know the current state
	return status;
}

/**
 * @fn         (int32_t rsi_ble_receive_start(void))
 * @brief      { Function to enable BLE receive }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_ble_receive_start(void)
{
	int32_t status = RSI_SUCCESS;
	status =  rsi_ble_per_receive(&cmd9); 																		//start receiving BLE packets
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStart BLE Receive Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStart BLE Receive Success\r\n");
	}
	state_completed |= ble_rx_start; 																					//flag to know the current state
	return status;
}

/**
 * @fn         (int32_t rsi_ble_receive_stop(void))
 * @brief      { Function to disable BLE receive }
 *
 * @return     { return 0 : SUCCESS >0: FAILURE }
 */
int32_t rsi_ble_receive_stop(void)
{
	int32_t status = RSI_SUCCESS; 
	status =  rsi_ble_per_receive(&cmd9); 																		//stop receiving BLE packets
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStop BLE Receive Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStop BLE Receive Success\r\n");
	}
	state_completed &= ~ble_rx_start; 																				//flag to know the current state
	return status;
}

/**
 * @fn         (void rsi_ble_stats_receive(void))
 * @brief      { Function to receive requested no of BLE stats per second and send to UART}
 */
void rsi_ble_stats_receive(void)
{
	uint32_t timer_1s;
	if(!(state_completed & ble_rx_start))
		rsi_ble_receive_start();
	rsi_bt_per_stats(BT_PER_STATS_CMD_ID,  &per_stats); 											//requesting BLE stats from wireless interface
	timer_1s  = rsi_hal_gettickcount(); 																							//start timer
	do{
		while((rsi_hal_gettickcount() - timer_1s) < 994); 															// waiting for 1 second for each BLE stat request
		timer_1s  = rsi_hal_gettickcount();
		rsi_bt_per_stats(BT_PER_STATS_CMD_ID,  &per_stats); 										//requesting BLE stats from wireless interface
		//appending stats to a buffer untill requested no of stats
		sprintf((send_buffer + strlen(send_buffer)),"%.4d" "%.4d" "%.4d",per_stats.crc_pass_cnt,per_stats.crc_fail_cnt,per_stats.rssi);
		if(cmdC.no_ble_stats == 1){
			printf("%s",send_buffer); 																						//sending requested no of stats over UART
			memset(send_buffer, 0, strlen(send_buffer));	 												// clearing the buffer
		}
		cmdC.no_ble_stats--;
	}while(cmdC.no_ble_stats); 																								// blocking untill requested no of stats received
}
#ifdef RSI_BT_ENABLE
/**
 * @fn         (int32_t rsi_bt_transmit_start(void))
 * @brief      { Function to start BT transmission }
 *
 * @return     { return 0 : SUCCESS >0 : FAILURE }
 */
int32_t rsi_bt_transmit_start(void)
{
	int32_t     status       = RSI_SUCCESS;
	status =  rsi_bt_per_tx(&cmdD); 																					//start tranmitting requested BT packets
	state_completed |= bt_tx_start; 																					//flag to know the current state
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStart BT Transmit Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStart BT Transmit Success\r\n");
	}
	return status;
}

/**
 * @fn         (int32_t rsi_bt_transmit_stop(void))
 * @brief      { function to  stop BT transmission }
 *
 * @return     { return 0 : SUCCESS >0 : FAILURE }
 */
int32_t rsi_bt_transmit_stop(void)
{
	int32_t status = RSI_SUCCESS;
	status =  rsi_bt_per_tx(&cmdD); 																					//stop tranmitting BT packets
	state_completed &= ~bt_tx_start; 																					//flag to know the current state
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStop BT Transmit Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStop BT Transmit Success\r\n");
	}
	return status;
}

/**
 * @fn         (int32_t rsi_bt_receive_start(void))
 * @brief      { Function to enable BT receive }
 *
 * @return     { return 0 : SUCCESS >0 : FAILURE }
 */
int32_t rsi_bt_receive_start(void)
{
	int32_t     status       = RSI_SUCCESS;
	rsi_bt_common_register_callbacks((void *)rsi_bt_get_ber_pkt_from_app_to_buff); 		//! Register BT receive stats call back handler
	status =  rsi_bt_per_rx(&cmdE); 																					//! Start/Recieve publishing RX stats
	state_completed |= bt_rx_start; 																					//flag to know the current state
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStart BT Receive Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStart BT Receive Success\r\n");
	}
	return status;
}

/**
 * @fn         (int32_t rsi_bt_receive_stop(void))
 * @brief      { Function to disable BT reveive }
 *
 * @return     { return 0 : SUCCESS >0 : FAILURE }
 */
int32_t rsi_bt_receive_stop(void)
{
	int32_t     status       = RSI_SUCCESS; 
	status =  rsi_bt_per_rx(&cmdE);
	ber_cmd.cmd_id = BT_BER_CMD_ID; 																					//BT BER command
	ber_cmd.enable = 0;
	rsi_bt_ber_enable_or_disable(&ber_cmd); 																	//! Stop BT stats
	state_completed &= ~bt_rx_start; 																					//flag to know the current state
	if(status != RSI_SUCCESS)
	{
		LOG_PRINT("\r\nStop BT Receive Failed, Error Code : 0x%X\r\n",status);
		return status;
	}
	else
	{
		LOG_PRINT("\r\nStop BT Receive Success\r\n");
	}
	return status;
}

/**
 * @fn         (void rsi_bt_stats_receive(void))
 * @brief      { Function to collect requested no of stats and send to UART }
 */
void rsi_bt_stats_receive(void)
{
	uint32_t timer_1s;
	uint8_t no_of_pkts;
	uint16_t ii;
	if(!(state_completed & bt_rx_start)) 																			//if current state is not bt_rx_start then enable bt RX with default parameters
		rsi_bt_receive_start();
	no_of_pkts = cmdH.no_bt_pkts;
	ber_cmd.cmd_id = BT_BER_CMD_ID; 																					// BT BER command
	ber_cmd.enable = 1;
	rsi_bt_ber_enable_or_disable(&ber_cmd); 																	//start receiving BT BER stats
	timer_1s  = rsi_hal_gettickcount(); 																							//enable timer
	if(cmdH.no_bt_pkts == 0){
		rsi_bt_receive_stop(); 																									//stop receiving BT stats once requested no of stats received
		return;
	}
	while((rsi_hal_gettickcount() - timer_1s) < 994); 																// wait for maximum time to receive Bt packets
	if(no_of_pkts == cmdH.no_bt_pkts){
		for(ii=0; ii<(1033 * no_of_pkts) ; ii++){
			printf("%.2x",buffer[ii]); 																						//if no packet is received in desired time send 0's for maximum BT packet length
		}
		memset(buffer, 0, 1036); 																								//clear the buffer
		rsi_bt_receive_stop(); 																									// stop receiving BT BER packets
	}
}
#endif
