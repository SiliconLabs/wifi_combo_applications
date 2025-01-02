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
 
#ifndef __RSI_SERIAL_H__
#define __RSI_SERIAL_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"
#include "stdio.h"
#include "rsi_chip.h"
#include "rsi_ble.h"
#include "rsi_wlan_apis.h"
#include "rsi_per_app.h"
#include <inttypes.h>
#include "USART.h"
#include "RTE_Device.h"

#define USART0_BASECLK     						 32000000
#define ULP_UART_BASECLK   						 32000000

#define ULP_UART_INSTANCE 						 0U
#define M4_UART_INSTANCE  						 1U

#define REF_CLK           						 0
#define SOC_PLL_CLK       						 1

#define SELECT_UART_INSTANCE    			 M4_UART_INSTANCE //!keep ULP_UART_INSTANCE : 0 in case of ULPSS uart , M4_UART_INSTANCE :1 in case of M4 UART
#define CLK_SELECTION           			 SOC_PLL_CLK     

#define BOARD_BAUD_VALUE  						 115200 //UART baud rate
#define DUMMY_VALUE										 'x'

#define DISABLE                        0
#define ENABLE                         1

#define LE_ONE_MBPS                    1
#define LE_TWO_MBPS                    2
#define LE_125_KBPS_CODED              4
#define LE_500_KBPS_CODED              8

#define DATA_PRBS9                     0x00
#define DATA_FOUR_ONES_FOUR_ZEROES     0x01  
#define DATA_ALT_ONES_AND_ZEROES       0x02
#define DATA_PRSB15                    0x03
#define DATA_ALL_ONES                  0x04
#define DATA_ALL_ZEROES                0x05
#define DATA_FOUR_ZEROES_FOUR_ONES     0x06
#define DATA_ALT_ZEROES_AND_ONES       0x07  

#define LE_ADV_CHNL_TYPE               0
#define LE_DATA_CHNL_TYPE              1

#define BURST_MODE                     0
#define CONTIUOUS_MODE                 1

#define NO_HOPPING                     0
#define FIXED_HOPPING                  1
#define RANDOM_HOPPING                 2

#define BT_PER_STATS_CMD_ID            0x08
#define BLE_TRANSMIT_CMD_ID            0x13
#define BLE_RECEIVE_CMD_ID             0x14

#define BLE_ACCESS_ADDR                0x71764129
#define BLE_TX_PKT_LEN                 32
#define BLE_PHY_RATE                   LE_ONE_MBPS
#define BLE_RX_CHNL_NUM                0
#define BLE_TX_CHNL_NUM                0
#define BLE_TX_POWER_INDEX             8
#define SCRAMBLER_SEED                 0
#define NUM_PKTS                       0
#define RSI_INTER_PKT_GAP              0

#define ONBOARD_ANT_SEL                2
#define EXT_ANT_SEL                    3

#define BLE_EXTERNAL_RF                0
#define BLE_INTERNAL_RF                1

#define NO_CHAIN_SEL                   0
#define WLAN_HP_CHAIN_BIT              0
#define WLAN_LP_CHAIN_BIT              1
#define BT_HP_CHAIN_BIT                2
#define BT_LP_CHAIN_BIT                3

#define PLL_MODE_0                     0
#define PLL_MODE_1                     1

#define LOOP_BACK_MODE_DISABLE         0
#define LOOP_BACK_MODE_ENABLE          1

#define EXT_DATA_LEN_IND               1

#define DUTY_CYCLING_DISABLE           0
#define DUTY_CYCLING_ENABLE            1

//! Select Intenal antenna or uFL connector
#define RSI_ANTENNA           				 1

//! Antenna gain in 2.4GHz band
#define RSI_ANTENNA_GAIN_2G            0
                       
//! Antenna gain in 5GHz band
#define RSI_ANTENNA_GAIN_5G            0

#define RSI_BLE_LOCAL_NAME             "PER_TEST"
#define RSI_BT_LOCAL_NAME              "PER_TEST"
#define RSI_SEL_ANTENNA                RSI_SEL_INTERNAL_ANTENNA//RSI_SEL_EXTERNAL_ANTENNA

#define BT_TRANSMIT_CMD_ID             0x15
#define BT_RECEIVE_CMD_ID              0x16
#define BT_BER_CMD_ID                  0x17

#define SEQUENCE_0          					 0
#define SEQUENCE_1          					 1
#define SEQUENCE_2         						 2
#define SEQUENCE_F0        						 3
#define SEQUENCE_PRBS     					   4

#define BR_MODE                        1
#define EDR_MODE                       2

#define SCO_LINK                       0
#define ACL_LINK                       1
#define ESCO_LINK                      2

#define BT_EXTERNAL_RF                 0
#define BT_INTERNAL_RF                 1

#define RSI_BT_BD_ADDR_1            	 0x11111111
#define RSI_BT_BD_ADDR_2            	 0x1111

#define PACKET_TYPE                    15
#define PACKET_LEN                     339
#define BT_RX_CHNL_NUM                 10
#define BT_TX_CHNL_NUM                 10

#define SCRAMBLER_SEED                 0

#define PAYLOAD_TYPE                   SEQUENCE_F0
#define BT_TX_POWER                    10
#define RSI_INTER_PKT_GAP              0

void Serial_send(uint8_t ch);
uint8_t Serial_receive(void);
void Serial_Init(void);
void Serial_start_rx();
void update_default_per(void);

//common command
typedef struct cmd1_s
{
  //! operational mode
	uint16_t oper_mode;
  //! coexistance mode
	uint16_t coex_mode;
}cmd1_t;

//wlan feature frame command
typedef struct cmd2_s
{
	//! PLL mode val   
	uint8_t   pll_mode ;
	//! rf type    
	uint8_t   rf_type; 
	//! wireless mode
	uint8_t   wireless_mode;
	//! enable ppp
	uint8_t   enable_ppp;
	//! afe type
	uint8_t   afe_type;
	//! feature enables
	uint32_t   feature_enables;
}cmd2_t;

//wlan tx command
typedef struct cmd3_s
{
	uint8_t channel;
	uint8_t mode;
	uint32_t length;
	uint32_t rate;
	uint8_t power;
}cmd3_t;

//wlan rx command
typedef struct cmd4_s
{
	uint8_t channel;
	uint8_t mode;
	uint32_t length;
	uint32_t rate;
	uint8_t  power;
}cmd4_t;

//wlan stop rx command
typedef struct cmd5_s
{
	uint8_t wlan_rx_stop;
}cmd5_t;

//wlan stop tx command
typedef struct cmd6_s
{
	uint8_t wlan_tx_stop;
}cmd6_t;

//wlan no of stats to receive command
typedef struct cmd7_s
{
	uint32_t no_of_stats;
}cmd7_t;

//LE Per transmit start command structure
typedef struct cmd8_s
{
	uint8_t cmd_ix;
  uint8_t transmit_enable;
  uint8_t access_addr[4];
  uint8_t phy_rate;
  uint8_t rx_chnl_num;
  uint8_t tx_chnl_num;
  uint8_t scrambler_seed;
  uint8_t le_chnl_type;
  uint8_t freq_hop_en;
  uint8_t ant_sel;
  uint8_t pll_mode;
  uint8_t rf_type;
  uint8_t rf_chain;
  uint8_t pkt_len[2];
  uint8_t payload_type;
  uint8_t tx_power;
  uint8_t transmit_mode;
  uint8_t inter_pkt_gap;
  uint8_t num_pkts[4];
}cmd8_t;

//LE Per receive start command structure
typedef struct cmd9_s {
  uint8_t cmd_ix;
  uint8_t receive_enable;
  uint8_t access_addr[4];
  uint8_t phy_rate;
  uint8_t rx_chnl_num;
  uint8_t tx_chnl_num;
  uint8_t scrambler_seed;
  uint8_t le_chnl_type;
  uint8_t freq_hop_en;
  uint8_t ant_sel;
  uint8_t pll_mode;
  uint8_t rf_type;
  uint8_t rf_chain;
  uint8_t ext_data_len_indication;
  uint8_t loop_back_mode;
  uint8_t duty_cycling_en;
}cmd9_t;

//LE Per no of stats
typedef struct cmdC_s {
  uint32_t no_ble_stats;
}cmdC_t;

//BT TX PER structure paramters
typedef struct cmdD_s {
  uint8_t cmd_id;
  uint8_t transmit_enable;
  uint8_t device_Addr[6];   //BT_BD_ADDR  peer_addr;
  uint8_t pkt_len[2];
  uint8_t pkt_type;
  uint8_t br_edr_mode;
  uint8_t rx_chnl_in;
  uint8_t tx_chnl_in;
  uint8_t link_type;
  uint8_t scrambler_seed;    
  uint8_t hopping_type;
  uint8_t ant_sel; 
  uint8_t pll_mode; 
  uint8_t rf_type; 
  uint8_t rf_chain;
  uint8_t payload_type;
  uint8_t tx_power;
  uint8_t transmit_mode;
  uint8_t inter_pkt_gap;
  uint8_t no_of_packets[4];
} cmdD_t;

//BT RX PER structure paramters
typedef struct cmdE_s {
  uint8_t cmd_id;
  uint8_t receive_enable;
  uint8_t device_Addr[6];   //BT_BD_ADDR  peer_addr;
  uint8_t pkt_len[2];
  uint8_t pkt_type;
  uint8_t br_edr_mode;
  uint8_t rx_chnl_in;
  uint8_t tx_chnl_in;
  uint8_t link_type;
  uint8_t scrambler_seed;    
  uint8_t hopping_type;
  uint8_t ant_sel; 
  uint8_t pll_mode; 
  uint8_t rf_type; 
  uint8_t rf_chain;
  uint8_t loop_back_mode;
} cmdE_t;

//bt no of stats command
typedef struct cmdH_s
{
	uint32_t no_bt_pkts;
}cmdH_t;

//wlan antenna select command
typedef struct cmdS_s
{
	uint8_t ant_sel;
	uint8_t gain_2g;
	uint8_t gain_5g;
}cmdS_t;

//read_address
typedef struct cmdZ_s {
  uint32_t address;
	uint32_t return_value;
}cmdZ_t;

//enum to switch between PER commands
typedef enum PER_COMMAND_S
{
	COMMAND_0 = 0,
	COMMAND_1 = 1,
	COMMAND_2 = 2,
	COMMAND_3 = 3,
	COMMAND_4 = 4,
	COMMAND_5 = 5,
	COMMAND_6 = 6,
	COMMAND_7 = 7,
	COMMAND_8 = 8,
	COMMAND_9 = 9,
	COMMAND_A = 17,
	COMMAND_B = 18,
	COMMAND_C = 19,
	COMMAND_D = 20,
	COMMAND_E = 21,
	COMMAND_F = 22,
	COMMAND_G = 23,
	COMMAND_H = 24,
	COMMAND_S = 35,
	COMMAND_Z = 42
}PER_COMMAND_T;

//enum to define current state of operation
typedef enum state_completed{
	common_command   = BIT(0),
	features_enabled = BIT(1),
	wlan_tx_start    = BIT(2),
	wlan_rx_start    = BIT(3),
	ble_tx_start  	 = BIT(4),
	ble_rx_start     = BIT(5),
	bt_tx_start	  	 = BIT(6),
	bt_rx_start 	   = BIT(7)
}state_completed_e;

//externs to use the structures through out the application
extern state_completed_e state_completed;
extern cmd1_t cmd1;
extern cmd2_t cmd2;
extern cmd3_t cmd3;
extern cmd4_t cmd4;
extern cmd5_t cmd5;
extern cmd6_t cmd6;
extern cmd7_t cmd7;
extern cmd8_t cmd8;
extern cmd9_t cmd9;
extern cmdC_t cmdC;
extern cmdD_t cmdD;
extern cmdE_t cmdE;
extern cmdH_t cmdH;
extern cmdS_t cmdS;
extern cmdZ_t cmdZ;
extern PER_COMMAND_T per_command;

#ifdef DEBUG_SERIAL
#define __DEBUGINIT()		Serial_Init()
#define __DEBUGOUT(...)		printf(__VA_ARGS__)
#define __DEBUGSTR(str) 
#ifdef  __GNUC__
#define __DEBUGIN(...) _read(__VA_ARGS__)
#define scanf(...) _read(__VA_ARGS__)
#else
 #define __DEBUGIN() Serial_receive()
#endif
#endif
#ifdef __cplusplus
}
#endif


#endif /*__RSI_SERIAL_H__*/

