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
 =================================================================================*/

/**
 * Include files
 * */

#include "rsi_per_app.h"
#include "rsi_board.h"

//! Memory length for driver
#define GLOBAL_BUFF_LEN 15000

//! Memory to initialize driver
uint8_t global_buf[GLOBAL_BUFF_LEN];

/**
 * @fn         void per_task()
 * @brief      { state machine to execute per commands }
 */
void per_task(){
	volatile int32_t     status = 0;
	Serial_Init();
	Serial_start_rx();
	while(1){

		switch(per_command){
		case COMMAND_0:
			break;
		case COMMAND_1:
			//wait to receive operation mode parameters
			scanf("%d" "%d",&cmd1.oper_mode,&cmd1.coex_mode);
			status = rsi_common_command(); 			//call function to set received operation mode
			printf("%d",status);
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_2:
			//wait to receive features
#ifdef DEBUG_SERIAL
			scanf("%d" "%d" "%d" "%d" "%d" "%d", &cmd2.pll_mode,&cmd2.rf_type,&cmd2.wireless_mode,&cmd2.enable_ppp,&cmd2.afe_type,&cmd2.feature_enables);
#endif
			status = rsi_send_per_features(); 	//enable features received
			printf("%d",status);    				//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_3:
			//wait to receive transmit parameters
#ifdef DEBUG_SERIAL
			scanf("%d" "%d" "%d" "%d" "%d",&cmd3.channel,&cmd3.mode,&cmd3.length,&cmd3.rate,&cmd3.power);
#endif
			status = rsi_wlan_per_tx_start(); 	//start transmitting with received parameters
			printf("%d",status);    				//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_4:
			//wait to receive channel no for wlan receive
#ifdef DEBUG_SERIAL
			scanf("%d",&cmd4.channel);
#endif
			status = rsi_receive_start(); 			//start receiving on given channel no
			printf("%d",status);    				//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_5:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmd5.wlan_rx_stop);
#endif
			status = rsi_receive_stop(); 				//stop receiving
			printf("%d",status); 								//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_6:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmd6.wlan_tx_stop);
#endif
			status = rsi_wlan_per_tx_stop(); 		//stop transmitting
			printf("%d",status); 								//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_7:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmd7.no_of_stats); //wait for no of stats to receive
#endif
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_8:
			//wait untill BLE transmit parameters received
#ifdef DEBUG_SERIAL
			scanf("%d" "%d" "%x" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d",\
					&cmd8.cmd_ix,&cmd8.transmit_enable,&*(uint32_t *)cmd8.access_addr,&cmd8.phy_rate,&cmd8.rx_chnl_num,&cmd8.tx_chnl_num,&cmd8.scrambler_seed,&cmd8.le_chnl_type,&cmd8.freq_hop_en,&cmd8.ant_sel,&cmd8.pll_mode,&cmd8.rf_type,&cmd8.rf_chain,&*(uint16_t *)cmd8.pkt_len,&cmd8.payload_type,&cmd8.tx_power,&cmd8.transmit_mode,&cmd8.inter_pkt_gap);
#endif
			status = rsi_ble_transmit_start();
			printf("%d",status);	 										//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_9:
			//wait until BLE receive parameters received
#ifdef DEBUG_SERIAL
			scanf("%d" "%d" "%x" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d",\
					&cmd9.cmd_ix,&cmd9.receive_enable,&*(uint32_t *)cmd9.access_addr,&cmd9.phy_rate,&cmd9.rx_chnl_num,&cmd9.tx_chnl_num,&cmd9.scrambler_seed,&cmd9.le_chnl_type,&cmd9.freq_hop_en,&cmd9.ant_sel,&cmd9.pll_mode,&cmd9.rf_type,&cmd9.rf_chain,&cmd9.ext_data_len_indication,&cmd9.loop_back_mode,&cmd9.duty_cycling_en);
#endif
			status = rsi_ble_receive_start();
			printf("%d",status);	 										//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_A:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmd9.receive_enable);
#endif
			status = rsi_ble_receive_stop(); 						//stop receiving BLE stats
			printf("%d",status);	 										//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_B:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmd8.transmit_enable);
#endif
			status = rsi_ble_transmit_stop(); 						//stop transmitting BLE stats
			printf("%d",status);	 										//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_C:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmdC.no_ble_stats);//wait to receive no of stats
#endif
			rsi_ble_stats_receive(); 						//send requested no of stats over uart
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
#ifdef RSI_BT_ENABLE
		case COMMAND_D:
			//wait to receive BT transmit parameters
#ifdef DEBUG_SERIAL
			scanf("%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d",\
					&cmdD.cmd_id,&cmdD.transmit_enable,&*(uint32_t *)&cmdD.device_Addr[0],&*(uint16_t *)&cmdD.device_Addr[4],&*(uint16_t *)&cmdD.pkt_len[0],&cmdD.pkt_type,&cmdD.br_edr_mode,&cmdD.rx_chnl_in,&cmdD.tx_chnl_in,&cmdD.link_type,&cmdD.scrambler_seed,&cmdD.payload_type,&cmdD.tx_power,&cmdD.transmit_mode,&cmdD.hopping_type,&cmdD.ant_sel,&cmdD.inter_pkt_gap,&cmdD.pll_mode,&cmdD.rf_type,&cmdD.rf_chain);
#endif
			rsi_bt_transmit_start(); 						//start BT transmission with given parameters
			printf("%d",0); 										//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_E:
#ifdef DEBUG_SERIAL
			//wait to reveive BT receive parameters
			scanf("%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d" "%d",\
					&cmdE.cmd_id,&cmdE.receive_enable,&*(uint32_t *)&cmdE.device_Addr[0],&*(uint16_t *)&cmdE.device_Addr[4],&*(uint16_t *)&cmdE.pkt_len[0],&cmdE.pkt_type,&cmdE.br_edr_mode,&cmdE.rx_chnl_in,&cmdE.tx_chnl_in,&cmdE.link_type,&cmdE.scrambler_seed,&cmdE.hopping_type,&cmdE.ant_sel,&cmdE.pll_mode,&cmdE.rf_type,&cmdE.rf_chain,&cmdE.loop_back_mode);
#endif
			rsi_bt_receive_start(); 						//start BT receive
			printf("%d",0); 										//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_F:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmdE.receive_enable);
#endif
			rsi_bt_receive_stop(); 							//stop BT receive
			printf("%d",0);	 										//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_G:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmdD.transmit_enable);
#endif
			rsi_bt_transmit_stop(); 						//stop BT receive
			printf("%d",0);	 										//send status
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		case COMMAND_H:
#ifdef DEBUG_SERIAL
			scanf("%d",&cmdH.no_bt_pkts); 	//wait to receive of no stats
#endif
			rsi_bt_stats_receive(); 						//send requested no of stats
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
#endif
		case COMMAND_S:
#ifdef DEBUG_SERIAL
			scanf("%d" "%d" "%d",&cmdS.ant_sel, &cmdS.gain_2g, &cmdS.gain_5g); 	//wait to receive ant_sel params
#endif
			status = rsi_wlan_radio_init();
			status = rsi_wireless_antenna(cmdS.ant_sel,cmdS.gain_2g,cmdS.gain_5g); 								//send antenna select command
       printf("%d",status); 
			Serial_start_rx(); 											//enable uart interrupt to receive next command
			break;

		case COMMAND_Z:
#ifdef DEBUG_SERIAL
			scanf("%x",&cmdZ.address); 					//get AHB address in hex format
#endif
			cmdZ.return_value = *(volatile uint32_t *)cmdZ.address; //read receive address
			printf("%.08x",cmdZ.return_value); 	//send read value over uart
			Serial_start_rx(); 									//enable uart interrupt to receive next command
			break;
		default:
			rsi_wireless_driver_task(); 				//default run wireless driver task
			break;
		}
	}
}


int main(){
	int32_t status;

	//! Driver initialization
	status = rsi_driver_init(global_buf, GLOBAL_BUFF_LEN);
	if((status < 0) || (status > GLOBAL_BUFF_LEN))
	{
		return status;
	}
	//! Redpine module initialization
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

	//!uart initialization added for non blocking mode
#ifdef DEBUG_SERIAL
	__DEBUGINIT();
#endif
	update_default_per();

	per_task();
}
