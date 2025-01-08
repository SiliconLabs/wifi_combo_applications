/**
 * @file        rsi_common.h
 * @version     0.1 
 * @date        15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief :  This file contains scheduler functionality defines and function declarations 
 *            of common control block used in the driver
 *  
 *  @section Description  This file contains scheduler functionality defines and function declarations 
 *            of common control block used in the driver
 *
 */

#ifndef RSI_COMMON_H
#define RSI_COMMON_H
#ifdef RSI_M4_INTERFACE
	#include "rsi_ipmu.h"
#endif
/******************************************************
 * *                      Macros
 * ******************************************************/

//! Power save SLP and WKP frame type
#define RSI_RSP_SLP                        0xDE
#define RSI_RSP_WKP                        0xDD


/******************************************************/

/******************************************************
 * *                    Constants
 * ******************************************************/
/******************************************************
 * *                   Enumerations
 * ******************************************************/

//! enumeration for states used in common control block in driver
typedef enum rsi_common_state_e
{
  RSI_COMMON_STATE_NONE = 0,
  RSI_COMMON_CARDREADY,
  RSI_COMMON_OPERMODE_DONE
}rsi_common_state_t;

//! enumeration for command responses used in common control block
typedef enum rsi_common_cmd_response_e
{
  RSI_COMMON_RSP_CLEAR            = 0x00,	
  RSI_COMMON_RSP_OPERMODE         = 0x10,
  RSI_COMMON_RSP_ANTENNA_SELECT   = 0x1B,
  RSI_COMMON_RSP_FEATURE_FRAME    = 0xC8,
  RSI_COMMON_RSP_SOFT_RESET       = 0x1C,
  RSI_COMMON_RSP_CARDREADY        = 0x89,
  RSI_COMMON_RSP_PWRMODE          = 0x15,
  RSI_COMMON_RSP_ULP_NO_RAM_RETENTION  = 0xCD,
  RSI_COMMON_RSP_ASYNCHRONOUS     = 0xFF,
  RSI_RSP_ENCRYPT_CRYPTO	  		  = 0x76,
  RSI_COMMON_RSP_UART_FLOW_CTRL_ENABLE = 0xA4,
  RSI_COMMON_RSP_TA_M4_COMMANDS   = 0xB0,
  RSI_COMMON_RSP_DEBUG_LOG		    = 0x26

#ifdef RSI_PUF_ENABLE
  ,RSI_COMMON_RSP_PUF_ENROLL	  = 0xD0,
  RSI_COMMON_RSP_PUF_DIS_ENROLL	  = 0xD1,
  RSI_COMMON_RSP_PUF_START		  = 0xD2,
  RSI_COMMON_RSP_PUF_SET_KEY	  = 0xD3,
  RSI_COMMON_RSP_PUF_DIS_SET_KEY  = 0xD4,
  RSI_COMMON_RSP_PUF_GET_KEY	  = 0xD5,
  RSI_COMMON_RSP_PUF_DIS_GET_KEY  = 0xD6,
  RSI_COMMON_RSP_PUF_LOAD_KEY	  = 0xD7,
  RSI_COMMON_RSP_AES_ENCRYPT	  = 0xD8,
  RSI_COMMON_RSP_AES_DECRYPT	  = 0xD9,
  RSI_COMMON_RSP_AES_MAC		  = 0xDA,
  RSI_COMMON_RSP_PUF_INTR_KEY	  = 0xCE
#endif

#ifdef RSI_WAC_MFI_ENABLE
  ,RSI_COMMON_RSP_IAP_GET_CERTIFICATE       = 0xB6,
  RSI_COMMON_RSP_IAP_INIT                  = 0xB7,
  RSI_COMMON_RSP_IAP_GENERATE_SIGATURE     = 0xB8
#endif
  //! Reusing RSI_WLAN_REQ_FW_VERSION as RSI_COMMON_REQ_FW_VERSION
  ,RSI_COMMON_RSP_FW_VERSION             = 0x49,
  RSI_COMMON_RSP_GET_RAM_DUMP  	    = 0x92

}rsi_common_cmd_response_t;


//! enumeration for command request used in common control block
typedef enum rsi_common_cmd_request_e
{
#ifdef RSI_CHIP_MFG_EN
  RSI_COMMON_DEV_CONFIG				        	= 0x28,
#endif

  RSI_COMMON_REQ_OPERMODE               = 0x10,
  RSI_COMMON_REQ_ANTENNA_SELECT         = 0x1B,
  RSI_COMMON_REQ_FEATURE_FRAME          = 0xC8,
  RSI_COMMON_REQ_SOFT_RESET             = 0x1C,
  RSI_COMMON_REQ_PWRMODE                = 0x15,
  RSI_COMMON_REQ_ENCRYPT_CRYPTO         = 0x76,
  RSI_COMMON_REQ_UART_FLOW_CTRL_ENABLE  = 0xA4,
  RSI_COMMON_REQ_TA_M4_COMMANDS         = 0xB0,
  RSI_COMMON_REQ_DEBUG_LOG		  	      = 0x26

  
#ifdef RSI_WAC_MFI_ENABLE
  ,RSI_COMMON_REQ_IAP_GET_CERTIFICATE       = 0xB6,
  RSI_COMMON_REQ_IAP_INIT                  = 0xB7,
  RSI_COMMON_REQ_IAP_GENERATE_SIGATURE     = 0xB8
#endif

#ifdef RSI_PUF_ENABLE
  ,RSI_COMMON_REQ_PUF_ENROLL		    = 0xD0,
  RSI_COMMON_REQ_PUF_DIS_ENROLL	    = 0xD1,
  RSI_COMMON_REQ_PUF_START		    	= 0xD2,
  RSI_COMMON_REQ_PUF_SET_KEY		  	= 0xD3,
  RSI_COMMON_REQ_PUF_DIS_SET_KEY  	= 0xD4,
  RSI_COMMON_REQ_PUF_GET_KEY		  	= 0xD5,
  RSI_COMMON_REQ_PUF_DIS_GET_KEY  	= 0xD6,
  RSI_COMMON_REQ_PUF_LOAD_KEY		   	= 0xD7,
  RSI_COMMON_REQ_AES_ENCRYPT	     	= 0xD8,
  RSI_COMMON_REQ_AES_DECRYPT	     	= 0xD9,
  RSI_COMMON_REQ_AES_MAC		      	= 0xDA,
  RSI_COMMON_REQ_PUF_INTR_KEY		  	= 0xCE
#endif
  //! Reusing RSI_WLAN_REQ_FW_VERSION as RSI_COMMON_REQ_FW_VERSION
   ,RSI_COMMON_REQ_FW_VERSION             = 0x49,
    RSI_COMMON_REQ_GET_RAM_DUMP  	        = 0x92
}rsi_common_cmd_request_t;

#ifdef RSI_PUF_ENABLE
//! enumeration for Common control block for PUF state machine
typedef enum rsi_puf_state_e
{
  RSI_PUF_STATE_NONE = 0,
  RSI_PUF_STATE_ENROLLED,
  RSI_PUF_STATE_STARTED,
  RSI_PUF_STATE_SET_KEY,
  RSI_PUF_STATE_KEY_LOADED,
  RSI_PUF_STATE_DISABLED
}rsi_puf_state_t;
#endif


#ifdef RSI_M4_INTERFACE
typedef enum ta_m4_commands_e{
 RSI_TAKE_M4_64K = 1,
 RSI_GET_IPMU_PROGRAMMING_VALUES=2,
 RSI_READ_TA_REGISTER=3,
 RSI_WRITE_TA_REGISTER = 4,
//! This enum varibale added for M4 has to give indication to TA, for Configure the Clock switching between 1.3V to 3.3 .For more details check Jira Ticket RSC-3802.   
 RSI_ENABLE_XTAL = 5,

}ta_m4_commands_t;
//!  M4 and TA secure handshake request structure.
typedef struct ta_m4_handshake_param{
 //! sub_cmd form the  enum ta_m4_commands_e(Main command type is RSI_COMMON_REQ_TA_M4_COMMANDS) 
 ta_m4_commands_t sub_cmd;
 //! length of input_data
 uint8_t input_data_size;
 //! Input data. In this input data first byte is reserved for enable(1) or Disable(0) sub_cmd of this structure.
 uint8_t *input_data;
}ta_m4_handshake_param_t;
#endif

#define RSI_DMA_VALID                                0
#define RSI_SKIP_DMA_VALID                           1

/******************************************************
 * *                 Type Definitions
 * ******************************************************/

//! opermode command request structure
typedef struct rsi_opermode_s
{
  //! operating mode 0-client, 1-p2p, 2-EAP, 6-AP, 8-PER
  uint8_t    opermode[4];                       

  //! BIT(0)-Open mode security, BIT(1)-PSK security, BIT(2) JSON objects
  uint8_t    feature_bit_map[4];

  //! BIT(0)-tcp/ip bypass, BIT(1)-HTTP server,BIT(2)-DHCPV4 client, 
  uint8_t    tcp_ip_feature_bit_map[4];          

  //! BIT(3)-DHCPV6 client, BIT(4)-DHCPV4 server, BIT(5)-DHCPV6 server
  uint8_t   custom_feature_bit_map[4]; 

  //! Extended custom feature bitmap
  uint8_t   ext_custom_feature_bit_map[4]; 

  //! BYTE(0): Reserved
  //! BYTE(2): (0-3): Reserved, (4-6): BT classic slaves, 7: A2DP profile enable
  //! BYTE(3): (0): A2DP proifle role 0- sink, 1 - source, (1) - A2DP Accelerated Mode Enable, (2-5): reserved, 6 - RF type, 7-reserved
  uint8_t   bt_feature_bit_map[4]; 

  //! Extended tcp/ip feature bit map
  uint8_t     ext_tcp_ip_feature_bit_map[4];
   
  //! BYTE(0) - BLE nbr of attributes,
  //! BYTE(1): (0 - 3): BLE Nbr of GATT services
  //! BYTE(1): (4 - 7): BLE Nbr of slaves
  //! BYTE(2): (0 - 7) BLE powersave index, 
  //! BYTE(3): (0 - 2)BLE powersave options,
  //! BYTE(3): (3: 5) Reserved,
  //! BYTE(3): bit 6: BLE new features enable
  //! BYTE(3): bit 7: Reserved,
  //! BYTE(4): bits (4 - 5):BLE Nbr of masters 
  uint8_t   ble_feature_bit_map[4];

  //! BYTE(0): (0 - 3): BLE number of connection events
  //! BYTE(1): (4 - 5): BLE number of connection events
  uint8_t   ble_ext_feature_bit_map[4];
  uint8_t   config_feature_bit_map[4];
} rsi_opermode_t;

//! Antenna select command request structure
typedef struct rsi_antenna_select_s 
{
        //! Antenna value to set    
        uint8_t    antenna_value;
        //! Antenna 2G gain value    
        uint8_t   gain_2g; 
        //! Antenna 5G gain value    
        uint8_t   gain_5g; 

} rsi_antenna_select_t;

//! UART  debug prints request structure
typedef struct rsi_debug_uart_print_s
{
        //! assertion_type    
        uint32_t    assertion_type;
        //! assertion_level
        uint32_t   assertion_level; 
} rsi_debug_uart_print_t;

//! RR feature frame command request structure
typedef struct rsi_feature_frame_s 
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

} rsi_feature_frame_t;

/******************************************************
 * *                    Structures
 * ******************************************************/

//! driver power save structure
typedef struct rsi_power_save_s
{
//! wlan driver power save profile mode 
  uint8_t wlan_psp_mode;

  //! bt driver power save profile mode 
  uint8_t bt_psp_mode;

  uint8_t zb_psp_mode;
  //! wlan_driver power save profile type
  uint8_t wlan_psp_type;

  //! bt_driver power save profile type
  uint8_t bt_psp_type;

  uint8_t  zb_psp_type;
  //! module power save state
  uint32_t module_state;

//! Module power save State Machine 
#define RSI_IDLE               0
#define RSI_SLP_RECEIVED       1
#define RSI_SLP_ACK_SENT       2
#define RSI_WKUP_RECEIVED      3  

  //! This is used to detect current module power mode is executing
  uint8_t current_ps_mode;

  //! This is used to detect current power save mode profile type
  uint8_t power_save_enable;
}rsi_power_save_t;

//! Set Sleep Timer structure
typedef struct rsi_set_sleep_timer_s
{
  uint8_t timeval[2];
} rsi_set_sleep_timer_t;

#ifdef RSI_CHIP_MFG_EN
typedef struct common_dev_config_params_s {
  uint8_t lp_sleep_handshake;
  uint8_t ulp_sleep_handshake;
#define NO_HANDSHAKE                    0
#define PKT_BASED_HANDSHAKE             2
#define GPIO_BASED_HANDSHAKE            1
#define M4_BASED_HANDSHAKE              3

#define DEV_TO_HOST_TA_OR_ULP_GPIO      BIT(0)
#define TA_GPIO                         0
#define ULP_GPIO                        1
#define RF_SUPPLY_VOLTAGE_1_9_OR_3_3V   BIT(1)
#define RF_LDO_EXCSS_SETTLE_TIME_1_9V   300
	/* If RF_SUPPLY_VOLTAGE_1_9_OR_3_3V is set, then RF @ 3.3V and if reset
	 * RF @ 1.9V. */
	uint8_t sleep_config_param; /* 0 for no handshake (pm will make the decision),1 for GPIO based handshake, 2 packet handshake */
	uint8_t host_wakeup_intr_enable;
	uint8_t host_wakeup_intr_active_high;
	uint32_t lp_wakeup_threshold;
	uint32_t ulp_wakeup_threshold;
	uint32_t wakeup_threshold; /* should be the last field, this is nt configurable by host */
	//! This variable should be assigned as per the SoC GPIOs unused in the schematics.
	uint32_t unused_soc_gpio_bitmap;
	//! This variable should be assigned as per the ULP GPIOs unused in the schematics.
	uint8_t unused_ulp_gpio_bitmap;
	//! If zero -> niether EXT PA nor EXT BT is enabled
	uint8_t ext_pa_or_bt_coex_en;
	uint8_t opermode;
	uint8_t driver_mode;
	uint8_t no_of_stations_supported;

#define EXT_PA      1
#define EXT_BT_COEX 2

  /*
   *  These will be added later
   *  uint8_t sleep_clock_src;
   *  uint8_t external_pa_enabled;
   *  uint8_t external_bt_coex;
   */
  uint16_t peer_distance;
  uint16_t bt_feature_bitmap;
} common_dev_config_params_t;

#endif

//! driver common block structure
typedef struct rsi_common_cb_s
{
  //! driver common block state 
  volatile rsi_common_state_t state;

  //! driver common block status
  volatile int32_t status;

  //! driver common block mutex
  rsi_mutex_handle_t common_mutex;

  //! driver common block command expected response
  rsi_common_cmd_response_t expected_response;

  //! driver common block semaphore
  rsi_semaphore_handle_t common_sem;

  //! driver common block tx pool
  rsi_pkt_pool_t  common_tx_pool;

  //! power save backups
  rsi_power_save_t power_save;

  //! coex mode variable for power save command
  uint16_t ps_coex_mode;

  //! buffer pointer given by application to driver
  uint8_t *app_buffer;

  //! buffer length given by application to driver
  uint32_t app_buffer_length;

#ifdef RSI_PUF_ENABLE
  //! PUF state variable
  rsi_puf_state_t puf_state;
#endif
#ifdef RSI_M4_INTERFACE
	efuse_ipmu_t *ipmu_calib_data_cb;
#endif
#ifdef RSI_WITH_OS
  rsi_semaphore_handle_t common_card_ready_sem;
#endif  
}rsi_common_cb_t;

typedef struct rsi_driver_cb_non_rom
{
  uint32_t tx_mask_event;
  rsi_mutex_handle_t tx_mutex;
} rsi_driver_cb_non_rom_t;

/******************************************************
 * *                 Global Variables
 * ******************************************************/
/******************************************************
 * *               Function Declarations
 * ******************************************************/
int32_t rsi_driver_process_common_recv_cmd(rsi_pkt_t *pkt);
int32_t rsi_driver_common_send_cmd(rsi_common_cmd_request_t cmd, rsi_pkt_t *pkt);
int8_t rsi_common_cb_init(rsi_common_cb_t *common_cb);
void rsi_handle_slp_wkp(uint8_t frame_type);
int8_t rsi_req_wakeup();
int8_t rsi_wait4wakeup();
void rsi_allow_sleep(void);
void rsi_powersave_gpio_init(void);
void rsi_common_packet_transfer_done(rsi_pkt_t *pkt);
int32_t rsi_sleep_mode_decision(rsi_common_cb_t *rsi_common_cb);
int32_t rsi_cmd_m4_ta_secure_handshake(uint8_t sub_cmd_type, uint8_t input_len,uint8_t *input_data, uint8_t output_len, uint8_t *output_data);

#ifdef RSI_CHIP_MFG_EN
int32_t rsi_common_dev_params();
int32_t rsi_wait_for_card_ready();
#endif


#endif
