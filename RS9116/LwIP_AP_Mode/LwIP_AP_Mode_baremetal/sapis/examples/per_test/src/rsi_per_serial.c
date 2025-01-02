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
 * This application demonstrates all the commands that are needed to be sent to
 * UART interface.
 =================================================================================*/

/**
 * Include files
 **/

#include "rsi_per_serial.h"
#include "rsi_board.h"
 
void UART_Signal_Event(uint32_t event);
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
uint32_t recv[1] = {0};

cmd1_t cmd1;
cmd2_t cmd2;
cmd3_t cmd3;
cmd4_t cmd4;
cmd5_t cmd5;
cmd6_t cmd6;
cmd7_t cmd7;
cmd8_t cmd8;
cmd9_t cmd9;
cmdC_t cmdC;
cmdD_t cmdD;
cmdE_t cmdE;
cmdH_t cmdH;
cmdS_t cmdS;
cmdZ_t cmdZ;

FILE __stdout;
FILE __stdin;

PER_COMMAND_T per_command;
/****************************************************************************/

struct __FILE
{
  int dummyVar; //Just for the sake of redefining __FILE, we won't be using it anyways ;)
};


int fputc(int c, FILE * stream)
{
	Serial_send((char)c);
	return 0; //return the character written to denote a successfull write
}

int fgetc(FILE * stream)
{
	char c = Serial_receive();
//	uartWrite(c); //To echo Received characters back to serial Terminal
	return c;
}

/**
 * @fn         void update_default_per(void)
 * @brief      { update all the default command before switching to any mode }
 */
void update_default_per(void){
	/*********************Common command***********************************/
	cmd1.oper_mode 				= 		8; 													
	cmd1.coex_mode 				= 		0; 													
	/*********************WLAN Feature Frame command***********************/
	cmd2.pll_mode 				= 		0; 													
	cmd2.rf_type 					= 		1; 													
	cmd2.wireless_mode 		=			0; 													
	cmd2.enable_ppp 			= 		0; 													
	cmd2.afe_type 				=			1; 													
	cmd2.feature_enables 	=			0; 													
	/*********************Transmit Test Start command**********************/
	cmd3.channel					= 		1; 													
	cmd3.length 					=			1000; 											
	cmd3.mode 						= 		RSI_BURST_MODE; 					
	cmd3.power 						= 		10; 												
	cmd3.rate 						= 		RSI_RATE_6; 							
	/*********************Receive Test Start command***********************/
	cmd4.channel					= 		1;
	cmd4.length 					=			1000;
	cmd4.mode 						=			RSI_BURST_MODE;
	cmd4.power 						= 		10;
	cmd4.rate 						= 		RSI_RATE_6;	
	/*********************Receive Test Stop command************************/
	cmd5.wlan_rx_stop			=		  0;
	/*********************Transmit Test Stop command***********************/
	cmd6.wlan_tx_stop 		= 		0;
	/*********************Receive No of stats command**********************/
	cmd7.no_of_stats 			= 		0;
	/*********************BLE Transmit Test Start command******************/
	cmd8.cmd_ix 					= 		BLE_TRANSMIT_CMD_ID;	
  cmd8.transmit_enable 	= 		ENABLE;
  *(uint32_t *)&cmd8.access_addr = BLE_ACCESS_ADDR;
  cmd8.phy_rate 				= 		BLE_PHY_RATE;
  cmd8.rx_chnl_num 			= 		BLE_RX_CHNL_NUM;
  cmd8.tx_chnl_num 			= 		BLE_TX_CHNL_NUM;
  cmd8.scrambler_seed 	= 		SCRAMBLER_SEED;
  cmd8.le_chnl_type 		= 		LE_DATA_CHNL_TYPE;
  cmd8.freq_hop_en 			= 		NO_HOPPING;
  cmd8.ant_sel 					= 		ONBOARD_ANT_SEL;
  cmd8.pll_mode 				= 		PLL_MODE_0;
  cmd8.rf_type 					= 		BLE_INTERNAL_RF;
  cmd8.rf_chain 				= 		BT_HP_CHAIN_BIT;
  *(uint16_t *)&cmd8.pkt_len = BLE_TX_PKT_LEN;
  cmd8.payload_type 		= 		DATA_FOUR_ZEROES_FOUR_ONES;
  cmd8.tx_power 				= 		BLE_TX_POWER_INDEX;
  cmd8.transmit_mode 		= 		BURST_MODE;
  cmd8.inter_pkt_gap 		= 		RSI_INTER_PKT_GAP;
	/*********************BLE Receive Test Start command*******************/
	cmd9.cmd_ix 					= 		BLE_RECEIVE_CMD_ID;
	cmd9.receive_enable 	= 		ENABLE;
	*(uint32_t *)&cmd9.access_addr[0] = BLE_ACCESS_ADDR;
	cmd9.ext_data_len_indication = EXT_DATA_LEN_IND;
	cmd9.phy_rate 				= 		BLE_PHY_RATE;
	cmd9.rx_chnl_num 			= 		BLE_RX_CHNL_NUM;
	cmd9.tx_chnl_num 			= 		BLE_TX_CHNL_NUM;
	cmd9.scrambler_seed 	= 		SCRAMBLER_SEED;
	cmd9.le_chnl_type 		= 		LE_DATA_CHNL_TYPE;
	cmd9.loop_back_mode 	= 		LOOP_BACK_MODE_DISABLE;
	cmd9.freq_hop_en 			=	 		NO_HOPPING;
	cmd9.ant_sel 					= 		ONBOARD_ANT_SEL;
	cmd9.duty_cycling_en 	= 		DUTY_CYCLING_DISABLE;
	cmd9.pll_mode 				= 		PLL_MODE_0;
	cmd9.rf_type 					= 		BLE_INTERNAL_RF;
	cmd9.rf_chain 				= 		BT_LP_CHAIN_BIT;
	/*********************BT Transmit Test Start command*******************/
	cmdD.cmd_id 					=     BT_TRANSMIT_CMD_ID;
	cmdD.transmit_enable 	=  		ENABLE;
	*(uint32_t *)&cmdD.device_Addr[0] = RSI_BT_BD_ADDR_1;
	*(uint16_t *)&cmdD.device_Addr[4] = RSI_BT_BD_ADDR_2;
	*(uint16_t *)&cmdD.pkt_len[0] = PACKET_LEN;
	cmdD.pkt_type 				= 		PACKET_TYPE;
	cmdD.br_edr_mode 			= 		BR_MODE;
	cmdD.rx_chnl_in 			= 		BT_RX_CHNL_NUM;
	cmdD.tx_chnl_in 			= 		BT_TX_CHNL_NUM;
	cmdD.link_type 				= 		ACL_LINK;
	cmdD.scrambler_seed 	= 		SCRAMBLER_SEED;
	cmdD.payload_type 		= 		PAYLOAD_TYPE;
	cmdD.tx_power     		= 		BT_TX_POWER;
	cmdD.transmit_mode 		= 		BURST_MODE;
	cmdD.hopping_type 		= 		NO_HOPPING;
	cmdD.ant_sel 					= 		ONBOARD_ANT_SEL;
	cmdD.inter_pkt_gap 		= 		RSI_INTER_PKT_GAP;
	cmdD.pll_mode 				= 		PLL_MODE_0;
	cmdD.rf_type 					= 		BT_INTERNAL_RF;
	cmdD.rf_chain 				= 		BT_HP_CHAIN_BIT;
	/*********************BT Receive Test Start command********************/
	cmdE.cmd_id 					= 		BT_RECEIVE_CMD_ID;
	cmdE.receive_enable		=			ENABLE;
	*(uint32_t *)&cmdE.device_Addr[0] = RSI_BT_BD_ADDR_1;
	*(uint16_t *)&cmdE.device_Addr[4] = RSI_BT_BD_ADDR_2;
	cmdE.link_type				=			ACL_LINK;
	cmdE.pkt_type					=			PACKET_TYPE;
 *(uint16_t *)&cmdE.pkt_len[0] = PACKET_LEN;   
	cmdE.scrambler_seed		=			SCRAMBLER_SEED;    
	cmdE.br_edr_mode			= 		BR_MODE;
	cmdE.rx_chnl_in				=			BT_RX_CHNL_NUM;
	cmdE.tx_chnl_in				=			BT_TX_CHNL_NUM;
	cmdE.hopping_type			=			NO_HOPPING;
	cmdE.ant_sel					=			ONBOARD_ANT_SEL; 
	cmdE.loop_back_mode		=			LOOP_BACK_MODE_DISABLE;
	cmdE.pll_mode					=			PLL_MODE_0; 
	cmdE.rf_type					= 		BT_INTERNAL_RF;
	cmdE.rf_chain					= 		BT_HP_CHAIN_BIT;
	/*********************Receive No of bt ber pkts************************/
	cmdH.no_bt_pkts 			= 		0;
}

#if SELECT_UART_INSTANCE
extern ARM_DRIVER_USART Driver_USART0;
static ARM_DRIVER_USART * USARTdrv   = &Driver_USART0; 		//M4 uart driver selection
#else
extern ARM_DRIVER_USART Driver_ULP_UART;
static ARM_DRIVER_USART * USARTdrv   = &Driver_ULP_UART; 	//ULP uart driver selection
#endif


/**
 * @fn         void ARM_UART_SignalEvent(uint32_t event)
 * @brief      { uart event handler }
 *
 * @param[in]  event  send/receive complete events
 */
volatile uint8_t   recv_done_user ,send_done_user=0;
void UART_Signal_Event(uint32_t event)
{
  int32_t     status = 0;
	switch(event)
	{
	case ARM_USART_EVENT_SEND_COMPLETE:
    send_done_user++;
		break;
	case ARM_USART_EVENT_RECEIVE_COMPLETE:
       NVIC_DisableIRQ(USART0_IRQn);  
		   per_command = (recv[0] - '0');      //get the per_command from the received character and switch to command
		   recv[0] = DUMMY_VALUE; 	
  recv_done_user++;
     		break;
	}
}


/**
 * @fn         void Serial_Init(void)
 * @brief      Initializes board UART for output, required for printf redirection.
 * @return     none
 */
void Serial_Init(void)
{		
	USARTdrv->Uninitialize(); 															//unintialize the uart
	
	USARTdrv->Initialize   (UART_Signal_Event); 					//initialize the uart
 
	USARTdrv->PowerControl (ARM_POWER_FULL); 								//enable the clocks to uart

	/* Enable Receiver and Transmitter lines */
	USARTdrv->Control (ARM_USART_CONTROL_TX, 1);

	USARTdrv->Control (ARM_USART_CONTROL_RX, 1);

	USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
			ARM_USART_DATA_BITS_8 |
			ARM_USART_PARITY_NONE |
			ARM_USART_STOP_BITS_1 |
			ARM_USART_FLOW_CONTROL_NONE,BOARD_BAUD_VALUE); 			//setting the baudrate and parameters for uart data transfer

#if SELECT_UART_INSTANCE
NVIC_DisableIRQ(USART0_IRQn); 														//disable uart interrupt
#else	
NVIC_DisableIRQ(ULPSS_UART_IRQn);
#endif
	
	return ;
} 


/**
 * @fn        uint8_t Serial_start_rx(void)
 * @brief     Get a single character from the UART, required for scanf input. 
 * @return    EOF if not character was received, or character value  
 *
 */
void Serial_start_rx()
{
  USARTdrv->Receive(&recv,1); 													//configure uart to receive one character
	NVIC_EnableIRQ(USART0_IRQn); 													//enable uart interrupt
}

/**
 * @fn        uint8_t Serial_receive(void)
 * @brief     Get a single character from the UART, required for scanf input. 
 * @return    EOF if not character was received, or character value  
 *
 */
uint8_t Serial_receive(void)
{
	uint8_t rev[1]={0};
	recv_done_user =0;
	USARTdrv->Receive(&rev,1); 														//configure uart to receive one character
	while(recv_done_user==0)
	{
		#if SELECT_UART_INSTANCE
	  RSI_M4SSUsart0Handler(); 														//receive characters in blocking mode untill requested no of packets
		#else	
		RSI_ULPUartHandler();
		#endif	 
	}
	return rev[0];
}

/**
 * @fn          void Serial_send(uint8_t ch)
 * @brief       Sends a single character on the UART, required for printf redirection. 
 * @param[in]   ch	: character to send 
 * @return      none
 */
void Serial_send(uint8_t ch)
{
	send_done_user=0;
	USARTdrv->Send(&ch, sizeof(ch)); 											//configure uart to send one character at a time
	while(send_done_user==0)
	{
		#if SELECT_UART_INSTANCE
	  RSI_M4SSUsart0Handler(); 														//send characters in blocking mode uitill total no of characters to send
		#else	
		RSI_ULPUartHandler();
		#endif	
	}
	return ;
}

