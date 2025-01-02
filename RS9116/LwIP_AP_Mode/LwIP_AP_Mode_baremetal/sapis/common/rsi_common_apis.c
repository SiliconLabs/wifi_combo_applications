/**
 * @file rsi_common_apis.c
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
 *  @brief :This file contains common API's which needs to be called from application
 *
 *  @section Description  This file contains functions that process common protocol commands
 *  to be sent or recieved from the device
 *
 *
 */
#ifdef RSI_M4_INTERFACE
#include "rsi_ipmu.h"
#endif
#include "rsi_driver.h"
#include "rsi_timer.h"
#ifdef RSI_BT_ENABLE
#include "rsi_bt.h"
#include "rsi_bt_config.h"
#if (BT_A2DP_SOURCE_SBC_CODEC)
#include "rsi_bt_sbc_codec.h"
#endif
#endif
#ifdef RSI_BLE_ENABLE
#include "rsi_ble.h"
#endif
#include "rsi_wlan_non_rom.h"
extern rsi_socket_info_non_rom_t *rsi_socket_pool_non_rom;
extern rsi_driver_cb_non_rom_t   *rsi_driver_cb_non_rom; 
/*
 * Global Variables
 * */
rsi_driver_cb_t *rsi_driver_cb = NULL;
#ifdef RSI_M4_INTERFACE 
extern efuse_ipmu_t global_ipmu_calib_data;
#endif
extern void rom_init(void);
int32_t rsi_driver_memory_estimate();
//uint32_t efuse_size = 0;
/*==============================================*/
/**
 * @fn          int32_t rsi_common_get_status()
 * @brief       Return common block status
 * @param[in]   None, 
 * @param[out]  None
 * @return      Common block status
 *
 *
 * @section description
 * This funcion returns common block status
 *
 *
 */

int32_t rsi_common_get_status(void)
{
  return rsi_driver_cb->common_cb->status;
}

/*==============================================*/
/**
 * @fn          void rsi_common_set_status()
 * @brief       Return common block status
 * @param[in]   status, set status of common control block 
 * @param[out]  None
 * @return      Common block status
 *
 *
 * @section description
 * This funcion sets the common block status
 *
 *
 */

void rsi_common_set_status(int32_t status)
{
  rsi_driver_cb->common_cb->status = status;
}

/*==============================================*/
/**
 *
 * @fn         rsi_driver_cb_t *rsi_driver_init(uint8_t *buffer, uint32_t length)
 * @brief      Initicalizes the driver components
 * @param[in]  buffer, application buffer pointer to initialize driver components 
 * @param[in]  length, length of the memory provided by application
 * @param[out] None
 * @return
 *       <=length - If success, returns the memory used, which is less or equal to buffer length provided
 *        >length - If fails, returns total memory required by driver
 *             -1 - If SPI/UART intialization fails
 *             -2 - If no of sockets exceeded
 *
 * @section description
 * This fucntion intializes driver components
 *
 */
int32_t rsi_driver_init(uint8_t *buffer, uint32_t length)
{
  int32_t         status = RSI_SUCCESS;
  uint32_t        actual_length  = 0;

  //! Memset user buffer
  memset(buffer, 0, length);

  //! Checking for Buffer alignment,If not aligned ,align the buffer to 4 bytes
  actual_length = 3 - ((((uint32_t)buffer) +3) & 3);
  buffer = (uint8_t *)((((uint32_t)buffer) + 3) & ~3);

  //! check buffer is enough or not for driver components
  actual_length += sizeof(rsi_driver_cb_t) + RSI_DRIVER_RX_POOL_SIZE +
    sizeof(rsi_common_cb_t) + RSI_COMMON_POOL_SIZE +
    sizeof(rsi_wlan_cb_t) +  RSI_WLAN_POOL_SIZE    +
    sizeof(rsi_nwk_cb_t)
#ifdef RSI_M4_INTERFACE
		+ sizeof(efuse_ipmu_t)
#endif
    + RSI_SOCKET_INFO_POOL_SIZE
    + RSI_SOCKET_INFO_POOL_ROM_SIZE
#ifdef RSI_ZB_ENABLE
    + sizeof(rsi_zigb_cb_t) + RSI_ZIGB_POOL_SIZE + sizeof(rsi_zigb_global_cb_t)
#endif
#if (defined RSI_BT_ENABLE || defined RSI_BLE_ENABLE) 
    + sizeof(rsi_bt_cb_t) + RSI_BT_COMMON_POOL_SIZE
    + sizeof(rsi_bt_global_cb_t)
#endif
#ifdef RSI_BT_ENABLE
    + sizeof(rsi_bt_cb_t) + RSI_BT_CLASSIC_POOL_SIZE + sizeof(rsi_bt_classic_cb_t)
#endif
#ifdef RSI_BLE_ENABLE
    + sizeof(rsi_bt_cb_t) + RSI_BLE_POOL_SIZE + sizeof(rsi_ble_cb_t)
#endif
    ;

  //! If length is not sufficient
  if(length < actual_length)
  {
    return actual_length;
  }

  //! Designate memory for driver cb
  rsi_driver_cb = (rsi_driver_cb_t *)buffer;
  buffer += sizeof(rsi_driver_cb_t);
#ifdef RSI_WLAN_ENABLE
  //! Memory for sockets
  rsi_socket_pool = (rsi_socket_info_t *)buffer;
  buffer += RSI_SOCKET_INFO_POOL_SIZE; 
  rsi_socket_pool_non_rom = (rsi_socket_info_non_rom_t *)buffer;
  buffer += RSI_SOCKET_INFO_POOL_ROM_SIZE; 

  //! Check for max no of sockets 
  if(RSI_NUMBER_OF_SOCKETS > 10)
  {
    status = -2;
    return status;
  }
#endif


  rom_init();

  //! Designate memory for rx_pool
  rsi_pkt_pool_init(&rsi_driver_cb->rx_pool, buffer, RSI_DRIVER_RX_POOL_SIZE, RSI_DRIVER_RX_PKT_LEN);
  buffer += RSI_DRIVER_RX_POOL_SIZE;

  //! Designate memory for common_cb
  rsi_driver_cb_non_rom =(rsi_driver_cb_non_rom_t *)buffer;
  buffer += sizeof(rsi_driver_cb_non_rom_t);

  //! Designate memory for common_cb
  rsi_driver_cb->common_cb =(rsi_common_cb_t *)buffer;
  buffer += sizeof(rsi_common_cb_t);

  //! Initialize common cb
  rsi_common_cb_init(rsi_driver_cb->common_cb);

  //! Designate pool for common block
  rsi_pkt_pool_init(&rsi_driver_cb->common_cb->common_tx_pool, buffer, RSI_COMMON_POOL_SIZE, RSI_COMMON_CMD_LEN);
  buffer += RSI_COMMON_POOL_SIZE;

  //! Designate memory for wlan block
  rsi_driver_cb->wlan_cb =(rsi_wlan_cb_t *)buffer;
  buffer += sizeof(rsi_wlan_cb_t);

  //! Designate memory for network block
  rsi_driver_cb->nwk_cb =(rsi_nwk_cb_t *)buffer;
  buffer += sizeof(rsi_nwk_cb_t);

#ifdef RSI_M4_INTERFACE
	//! Designate memory for efuse ipmu block
    rsi_driver_cb->common_cb->ipmu_calib_data_cb=(efuse_ipmu_t *)buffer;
    //ipmu_calib_data_cb = (efuse_ipmu_t *)buffer;
		//efuse_size = sizeof(efuse_ipmu_t);
    buffer += sizeof(efuse_ipmu_t);
#endif
#ifdef RSI_WLAN_ENABLE
  //! Initialize wlan cb
  rsi_wlan_cb_init(rsi_driver_cb->wlan_cb);
#endif

  //! Designate memory for wlan_cb pool
  rsi_pkt_pool_init(&rsi_driver_cb->wlan_cb->wlan_tx_pool, buffer, RSI_WLAN_POOL_SIZE, RSI_WLAN_CMD_LEN);
  buffer += RSI_WLAN_POOL_SIZE;

  //! Initialize scheduler
  rsi_scheduler_init(&rsi_driver_cb->scheduler_cb);

  //! Initialize events
  rsi_events_init();

  rsi_queues_init(&rsi_driver_cb->wlan_tx_q);

  rsi_queues_init(&rsi_driver_cb->common_tx_q);
#ifdef RSI_M4_INTERFACE
  rsi_queues_init(&rsi_driver_cb->m4_tx_q);

  rsi_queues_init(&rsi_driver_cb->m4_rx_q);

#endif
#ifdef RSI_ZB_ENABLE
  rsi_driver_cb->zigb_cb =(rsi_zigb_cb_t *)buffer;
  buffer += sizeof(rsi_zigb_cb_t);

  //! Initialize zigb cb
  rsi_zigb_cb_init(rsi_driver_cb->zigb_cb);

  //! Designate memory for zigb_cb buffer pool
  rsi_pkt_pool_init(&rsi_driver_cb->zigb_cb->zigb_tx_pool, buffer, RSI_ZIGB_POOL_SIZE, RSI_ZIGB_CMD_LEN);
  buffer += RSI_ZIGB_POOL_SIZE;

#ifdef ZB_MAC_API
  rsi_driver_cb->zigb_cb->zigb_global_mac_cb = (rsi_zigb_global_mac_cb_t*)buffer;
  buffer += sizeof(rsi_zigb_global_mac_cb_t);
  //! Fill in zigb_global_cb
  buffer += rsi_zigb_global_mac_cb_init(buffer);
#else
  rsi_driver_cb->zigb_cb->zigb_global_cb = (rsi_zigb_global_cb_t*)buffer;
  buffer += sizeof(rsi_zigb_global_cb_t);

  //! Fill in zigb_global_cb
  buffer += rsi_zigb_global_cb_init(buffer);
#endif
  rsi_queues_init(&rsi_driver_cb->zigb_tx_q);
#ifdef ZB_DEBUG
  printf("\n ZIGB POOL INIT \n");
#endif
#endif

#if defined(RSI_BT_ENABLE) || defined(RSI_BLE_ENABLE)
  //! Designate memory for bt_common_cb
  rsi_driver_cb->bt_common_cb = (rsi_bt_cb_t *)buffer;
  buffer += ((sizeof(rsi_bt_cb_t) + 3) & ~3);

  rsi_mutex_create(&rsi_driver_cb->bt_single_mutex);
  //! Initialize bt_common_cb
  rsi_bt_cb_init(rsi_driver_cb->bt_common_cb);

  //! Designate memory for bt_common_cb pool
  rsi_pkt_pool_init(&rsi_driver_cb->bt_common_cb->bt_tx_pool, buffer, RSI_BT_COMMON_POOL_SIZE, RSI_BT_COMMON_CMD_LEN);
  buffer += ((RSI_BT_COMMON_POOL_SIZE + 3) & ~3);

  rsi_queues_init(&rsi_driver_cb->bt_common_tx_q);
#endif

#ifdef RSI_BT_ENABLE
  //! Designate memory for bt_classic_cb
  rsi_driver_cb->bt_classic_cb = (rsi_bt_cb_t *)buffer;
  buffer += ((sizeof(rsi_bt_cb_t) + 3) & ~3);

  //! Initialize bt_classic_cb
  rsi_bt_cb_init(rsi_driver_cb->bt_classic_cb);

  //! Designate memory for bt_classic_cb pool
  rsi_pkt_pool_init(&rsi_driver_cb->bt_classic_cb->bt_tx_pool, buffer, RSI_BT_CLASSIC_POOL_SIZE, RSI_BT_CLASSIC_CMD_LEN);
  buffer += ((RSI_BT_CLASSIC_POOL_SIZE + 3) & ~3);

  rsi_queues_init(&rsi_driver_cb->bt_classic_tx_q);

#endif

#ifdef RSI_BLE_ENABLE
  //! Designate memory for ble_cb
  rsi_driver_cb->ble_cb = (rsi_bt_cb_t *)buffer;
  buffer += ((sizeof(rsi_bt_cb_t) + 3) & ~3);

  //! Initialize ble_cb
  rsi_bt_cb_init(rsi_driver_cb->ble_cb);

  //! Designate memory for ble_cb pool
  rsi_pkt_pool_init(&rsi_driver_cb->ble_cb->bt_tx_pool, buffer, RSI_BLE_POOL_SIZE, RSI_BLE_CMD_LEN);
  buffer += ((RSI_BLE_POOL_SIZE + 3) & ~3);

  rsi_queues_init(&rsi_driver_cb->ble_tx_q);
#endif

#ifdef SAPIS_BT_STACK_ON_HOST
  //! Designate memory for bt_classic_cb
  rsi_driver_cb->bt_ble_stack_cb = (rsi_bt_cb_t *)buffer;
  buffer += sizeof(rsi_bt_cb_t);

  //! Initialize bt_classic_cb
  rsi_bt_cb_init(rsi_driver_cb->bt_ble_stack_cb);

  //! Designate memory for bt_classic_cb pool
  rsi_pkt_pool_init(&rsi_driver_cb->bt_ble_stack_cb->bt_tx_pool, buffer, RSI_BT_CLASSIC_POOL_SIZE, RSI_BT_CLASSIC_CMD_LEN);
  buffer += RSI_BT_CLASSIC_POOL_SIZE;

  rsi_queues_init(&rsi_driver_cb->bt_ble_stack_tx_q);

#endif

#if defined(RSI_BT_ENABLE) || defined(RSI_BLE_ENABLE)
  //! Designate memory for bt_common_cb
  rsi_driver_cb->bt_global_cb = (rsi_bt_global_cb_t*)buffer;
  buffer += sizeof(rsi_bt_global_cb_t);

  //! Fill in bt_global_cb
  buffer += rsi_bt_global_cb_init(rsi_driver_cb, buffer);
#endif


#ifndef LINUX_PLATFORM
#ifdef RSI_SPI_INTERFACE
  rsi_timer_start(RSI_TIMER_NODE_0, RSI_HAL_TIMER_TYPE_PERIODIC, RSI_HAL_TIMER_MODE_MILLI,1,rsi_timer_expiry_interrupt_handler);
#endif 

#if (defined(RSI_SPI_INTERFACE) || defined(RSI_SDIO_INTERFACE))
 //! Configure power save GPIOs
  rsi_powersave_gpio_init();
#endif
#endif

#ifdef LINUX_PLATFORM
#if (defined (RSI_USB_INTERFACE) || defined (RSI_SDIO_INTERFACE))
  status = rsi_linux_app_init();
    if(status != RSI_SUCCESS)
    {
      return -1;
    }

#endif
#endif

#ifdef RSI_UART_INTERFACE
  //! UART initialisation
  status = rsi_uart_init();
  if(status != RSI_SUCCESS)
  {
    return -1;
  }
#endif

  return actual_length;
}

/*==============================================*/

/**
 * @fn         rsi_wireless_init(uint16_t opermode, uint16_t coex_mode)
 * @brief      Initialize WiSeConnect software
 * @param[in]  opermode , WLAN operating mode
 * @param[in]  coex_mode, co-ex mode 
 * @return     int32_t 
 *             0  = success
 *             <0 = failure 
 *
 * @section description
 * This function is used to initialize WiSeConnect software module.
 * Application should call this function 
 *
 */
int32_t rsi_wireless_init(uint16_t opermode,uint16_t coex_mode)
{
  rsi_pkt_t      *pkt;
  rsi_opermode_t *rsi_opermode;
  int32_t        status = RSI_SUCCESS;
  uint32_t timer_instance;
#ifdef RSI_M4_INTERFACE	
  int32_t        ipmu_status = RSI_SUCCESS;

#endif
  //! Get common cb pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  common_cb->ps_coex_mode = coex_mode; 
  common_cb->ps_coex_mode &= ~BIT(0); 
	
#if ((defined RSI_UART_INTERFACE) && (defined RSI_STM32))
	common_cb->state = RSI_COMMON_CARDREADY;	
#else
  rsi_init_timer(&timer_instance,RSI_CARD_READY_WAIT_TIME);
  //! if state is not in card ready received state
  do{  
#ifndef RSI_WITH_OS
    //! Wait untill receives card ready
    rsi_scheduler(&rsi_driver_cb->scheduler_cb);
#else
    rsi_semaphore_wait(&common_cb->common_card_ready_sem, RSI_WAIT_FOREVER);
#endif
    if(rsi_timer_expired(&timer_instance))
    {
      return -1;
    }
  }while(common_cb->state == RSI_COMMON_STATE_NONE);
#endif
  if(common_cb->state != RSI_COMMON_CARDREADY)
  {
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  else if(wlan_cb->auto_config_state == RSI_WLAN_STATE_AUTO_CONFIG_GOING_ON)
  {
    while(1)
    {
      //! Run scheduler
      rsi_scheduler(&rsi_driver_cb->scheduler_cb);

      //! Check auto config state

      if((wlan_cb->auto_config_state == RSI_WLAN_STATE_AUTO_CONFIG_DONE) || (wlan_cb->auto_config_state == RSI_WLAN_STATE_AUTO_CONFIG_FAILED))
      {
        wlan_cb->auto_config_state = 0;
        break;
      }
#ifdef RSI_WLAN_ENABLE      
      if((status = rsi_wlan_get_status()))
      {
        //! return error
        return status;
      }
#endif
    }
    //! auto configuration status.
    return RSI_USER_STORE_CFG_STATUS;
  }

  //! lock the mutex 
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);


  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  //! memset the packet data
  memset(&pkt->data, 0, sizeof(rsi_opermode_t));

  //! Take the user provided data and fill it in opermode mode structure
  rsi_opermode = (rsi_opermode_t *)pkt->data;

  //! Save opermode command
  wlan_cb->opermode = opermode;

  //! Fill coex and opermode parameters
  rsi_uint32_to_4bytes(rsi_opermode->opermode, (coex_mode <<16 | opermode));

  //! send opermode command to driver
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_OPERMODE, pkt);

  //! command response status if success
  if(!status)
  {
    //! update the status 
    status = rsi_common_get_status();
  }

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);
#if defined(RSI_BT_ENABLE) || defined(RSI_BLE_ENABLE)
  if(!status)
  {
    if ( (coex_mode == RSI_OPERMODE_WLAN_BLE) ||
        (coex_mode ==  RSI_OPERMODE_WLAN_BT_CLASSIC))
    {
      //WC waiting for BT Classic/ZB/BLE card ready
      rsi_bt_common_init();
    }
  }
#endif
	
#ifdef RSI_M4_INTERFACE	
	//ipmu_status = rsi_cmd_m4_ta_secure_handshake(2,0,NULL,sizeof(efuse_ipmu_t),(uint8_t *)&global_ipmu_calib_data);
	//RSI_IPMU_UpdateIpmuCalibData(&global_ipmu_calib_data);
#endif 
  
	//! Return the status
  return status;
}
/*==============================================*/

#ifdef RSI_CHIP_MFG_EN

#define RSI_WAIT_TIMEOUT   50000000
#define RSI_INC_TIMER timer_count++
int32_t rsi_wait_for_card_ready(uint32_t queue_type)
{
	uint32_t timer_count = 0;
	uint32_t status = RSI_SUCCESS;

	rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;
	rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;
	volatile rsi_common_state_t *state_p;

	if(queue_type == RSI_COMMON_Q) {
		state_p = &common_cb->state;
	} else {
		state_p = &wlan_cb->state;
	}
	//! if state is not in card ready received state
	while(*state_p == RSI_COMMON_STATE_NONE)
	{
		if(RSI_INC_TIMER > RSI_WAIT_TIMEOUT) {
			status = RSI_FAILURE;
			break;
		}
#ifndef RSI_WITH_OS
		//! Wait untill receives card ready
		rsi_scheduler(&rsi_driver_cb->scheduler_cb);
#else
		rsi_semaphore_wait(&common_cb->common_card_ready_sem, RSI_WAIT_FOREVER);
#endif
	}
	return status;
}

int32_t rsi_common_dev_params()
{
	int32_t    status = RSI_SUCCESS;
	rsi_pkt_t             *pkt;
	common_dev_config_params_t *rsi_common_dev_params_p;

	//! Get common cb structure pointer
	rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

	//! take lock on common control block
	rsi_mutex_lock(&common_cb->common_mutex);

	//! allocate command buffer  from common pool
	pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
		//! unlock mutex
		rsi_mutex_unlock(&common_cb->common_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	//! Take the user provided data and fill it in debug uart print select structure
	rsi_common_dev_params_p = (common_dev_config_params_t *)pkt->data;

	rsi_common_dev_params_p->lp_sleep_handshake = 0x0;
	rsi_common_dev_params_p->ulp_sleep_handshake = 0x1;
	rsi_common_dev_params_p->sleep_config_param = 0x0;
	rsi_common_dev_params_p->host_wakeup_intr_enable = 0xf;
	rsi_common_dev_params_p->host_wakeup_intr_active_high = 0x4;
	rsi_common_dev_params_p->lp_wakeup_threshold = 0x100;
	rsi_common_dev_params_p->ulp_wakeup_threshold = 0x2030200;
	rsi_common_dev_params_p->wakeup_threshold = 0x0;
	rsi_common_dev_params_p->unused_soc_gpio_bitmap = 0x10000;
	rsi_common_dev_params_p->unused_ulp_gpio_bitmap = 0x13;
	rsi_common_dev_params_p->ext_pa_or_bt_coex_en = 0x0;
	rsi_common_dev_params_p->opermode = 0x0;
	rsi_common_dev_params_p->driver_mode = 0x0;
	rsi_common_dev_params_p->no_of_stations_supported = 0x1;
	rsi_common_dev_params_p->peer_distance = 0x0;
	rsi_common_dev_params_p->bt_feature_bitmap = 0x13;

	//! Magic number to set asyncronous response
	common_cb->expected_response = RSI_COMMON_RSP_ASYNCHRONOUS ;
	//! Send firmware version query request
	status = rsi_driver_common_send_cmd(RSI_COMMON_DEV_CONFIG, pkt);

	//! free the packet
	rsi_pkt_free(&common_cb->common_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&common_cb->common_mutex);

	//! Return the status
	return status;
}
#endif
/**
 * @fn         rsi_cmd_uart_flow_ctrl(uint8_t uartflow_en)
 * @brief      uart flow control enabling or disabling
 * @param[in]  uartflow_en , uart flow control enable or disable
 * @return     int32_t 
 *             0  = success
 *             <0 = failure 
 *
 * @section description
 * This function is used to uart flow control enabling or disabling.
 * Application should call this function 
 *
 */
int32_t rsi_cmd_uart_flow_ctrl(uint8_t uartflow_en)
{
  rsi_pkt_t       *pkt;
 
  int32_t          status = RSI_SUCCESS;

  //! Get common cb pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

  //! lock the mutex 
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  pkt->data[0] = uartflow_en;
  //! send  antenna select command
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_UART_FLOW_CTRL_ENABLE, pkt);

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);

  //! Return the status
  return status;



}
/*==============================================*/
/**
 * @fn
 * @brief
 * @param[in]   , 
 * @param[out]
 * @return
 *
 *
 * @section description
 * This 
 *
 *
 */

#ifdef RSI_M4_INTERFACE
int32_t rsi_cmd_m4_ta_secure_handshake(uint8_t sub_cmd_type, uint8_t input_len,uint8_t *input_data, uint8_t output_len, uint8_t *output_data)
{
  rsi_pkt_t       *pkt;
 
  int32_t          status = RSI_SUCCESS;

  //! Get common cb pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

  //! lock the mutex 
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }


  //! Take the sub_cmd_type for TA and M4 commands
  pkt->data[0] = sub_cmd_type;


  pkt->data[1] = input_len;


  memcpy(&pkt->data[2],input_data,input_len);


  //! attach the buffer given by user
  common_cb->app_buffer = output_data;

  //! length of the buffer provided by user
  common_cb->app_buffer_length = output_len;

  //! send  antenna select command
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_TA_M4_COMMANDS, pkt);

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);

  //! Return the status
  return status;



}
#endif

 



/*==============================================*/
/**
 * @fn           int32_t rsi_wireless_deinit();
 * @brief        Resets the module 
 * @param[in]    none 
 * @return       int32_t 
 *               0  =  success
 *               <0 = failure 
 *
 * @section description
 * This function resets the module     
 */

int32_t rsi_wireless_deinit(void)
{
  rsi_pkt_t *pkt;
  int32_t   status = RSI_SUCCESS;
  int32_t length =0;
#ifdef RSI_M4_INTERFACE
  //! Get common cb structure pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

  //! take lock on common control block
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer  from wlan pool
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  //! Magic number to set asyncronous reswponse
  common_cb->expected_response = RSI_COMMON_RSP_ASYNCHRONOUS ;

  //! send softreset command
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_SOFT_RESET, pkt);

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);

  if(status != RSI_SUCCESS)
  {
    //! Return the status
    return status;
  }
#ifndef RSI_M4_INTERFACE
  //! Mask interrupts
  rsi_hal_intr_mask();
#endif
#ifdef RSI_SPI_INTERFACE
  //! poll for interrupt status
  while(!rsi_hal_intr_pin_status());

  //! SPI interface initialization
  status = rsi_spi_iface_init();
  if(status != RSI_SUCCESS)
  {
    //! Return the status
    return status;
  }
#endif
#ifndef RSI_M4_INTERFACE
  //! Unmask interrupts
  rsi_hal_intr_unmask();
#endif

#ifdef RSI_UART_INTERFACE
  rsi_uart_deinit();

#endif
  //! Wait on common semaphore 
  rsi_semaphore_wait(&rsi_driver_cb->common_cb->common_sem, RSI_WAIT_FOREVER);    

  //! get common status
  status = rsi_common_get_status();
#else
  
  length = rsi_driver_memory_estimate();

  if( rsi_driver_cb != NULL )
  {
    //! Driver initialization
    status = rsi_driver_init((uint8_t *)rsi_driver_cb, length);
    if((status < 0) || (status > length))
    {
      return status;
    }
  }
  else
  {
     return -1; 
  }

  //! Redpine module initialization
  status = rsi_device_init(LOAD_NWP_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //! state update
  rsi_driver_cb->common_cb->state = RSI_COMMON_STATE_NONE;
#ifdef RSI_WLAN_ENABLE       	
  rsi_driver_cb->wlan_cb->state  = RSI_WLAN_STATE_NONE;
#endif
#endif
    return status;
}

/*==============================================*/
/**
 * @fn             int32_t rsi_wireless_antenna(uint8_t type)
 * @brief          This function is used to select antenna on the device
 * @param[in]   ,  type : INTERNAL_OR_RF_OUT1 , UFL_OR_RF_OUT2  
 * @param[out]     None 
 * @return       int32_t 
 *               0  =  success
 *               <0 = failure 
 *
 * @section description
 * This Function is used to select antenna type on the device
 *
 */
int32_t rsi_wireless_antenna(uint8_t type,uint8_t gain_2g, uint8_t gain_5g)
{

  rsi_pkt_t       *pkt;
  rsi_antenna_select_t *rsi_antenna;
  int8_t          status = RSI_SUCCESS;
	
//! Get wlan cb structure pointer
  rsi_wlan_cb_t *wlan_cb = rsi_driver_cb->wlan_cb;

  //! Get common cb pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

  //! pre-condition for antenna selection.
  if((wlan_cb->state < RSI_WLAN_STATE_INIT_DONE))
  {
    //!command given in wrong state
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! lock the mutex 
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }


  //! Take the user provided data and fill it in antenna select structure
  rsi_antenna = (rsi_antenna_select_t *)pkt->data;

  //! antenna type
  rsi_antenna->antenna_value = type;

  //! antena gain in 2.4GHz
  rsi_antenna->gain_2g       = gain_2g;

  //! antenna gain in 5GHz 
  rsi_antenna->gain_5g       = gain_5g;

  //! send  antenna select command
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_ANTENNA_SELECT, pkt);

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);

  //! Return the status
  return status;

}

/*==============================================*/
/**
 * @fn             int32_t rsi_send_feature_frame()
 * @brief          This function is used to send feature frame to connection cmd parser
 * @return       int32_t 
 *               0  =  success
 *               <0 = failure 
 *
 * @section description
 * This Function is used to send feature frame to connection command parser
 *
 */
int32_t rsi_send_feature_frame(void)
{

  rsi_pkt_t       *pkt;
  rsi_feature_frame_t *rsi_feature_frame;
  int8_t          status = RSI_SUCCESS;

  //! Get common cb pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

  //! lock the mutex 
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }


  //! Take the user provided data and fill it in antenna select structure
  rsi_feature_frame = (rsi_feature_frame_t *)pkt->data;

  //! pll mode value
  rsi_feature_frame->pll_mode = PLL_MODE;

  //! rf type
  rsi_feature_frame->rf_type = RF_TYPE;

  //! wireless mode
  rsi_feature_frame->wireless_mode = WIRELESS_MODE;

  //! enable ppp
  rsi_feature_frame->enable_ppp = ENABLE_PPP;
 
  //! afe type
  rsi_feature_frame->afe_type = AFE_TYPE;

  //! feature enables
  rsi_feature_frame->feature_enables = FEATURE_ENABLES;

  //! send antenna select command
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_FEATURE_FRAME, pkt);

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);

  //! Return the status
  return status;

}
/*==============================================*/
/**
 * @fn          int32_t rsi_get_fw_version(uint8_t *response, uint16_t length)
 * @brief       Returns enquired data
 *                
 * @param[out]  application buffer to get the command response payload
 * @param[in]   length of the application buffer to copy command response 
 * @return      command response status
 *
 * @section description
 * The functionality of this function depends on the ion
 *
 * This function returns common status
 *
 */
int32_t rsi_get_fw_version(uint8_t *response, uint16_t length)
{
  int32_t    status = RSI_SUCCESS;
  rsi_pkt_t             *pkt;

  //! Get common cb structure pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

  //! if state is not in card ready received state
  if(common_cb->state == RSI_COMMON_STATE_NONE)
  {
    while(common_cb->state != RSI_COMMON_CARDREADY)
    { 
#ifndef RSI_WITH_OS
      rsi_scheduler(&rsi_driver_cb->scheduler_cb);
#endif
    }
  }

  //! take lock on common control block
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer  from common pool
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  if(response != NULL)
  {
    //! attach the buffer given by user
    common_cb->app_buffer = response;

    //! length of the buffer provided by user 
    common_cb->app_buffer_length = length; 
  }
  else
  {
    //! Assign NULL to the app_buffer to avoid junk
    common_cb->app_buffer = NULL;

    //! length of the buffer to 0 
    common_cb->app_buffer_length = 0; 

  }

  //! Send firmware version query request
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_FW_VERSION, pkt);

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);

  //! Return the status
  return status;

}

/*==============================================*/
/**
 * @fn          int32_t rsi_common_debug_log(uint32_t assertion_type, uint32_t assertion_level)
 * @brief       enable debug logs.
 *                
 * @param[in]   Assertion_type (Possible values 0 - 4) 
 * @param[in]   Assertion_level (Possible values 0 - 15). value 1 is least value,15 is the highest level,0 is to disable all the prints.
 * @return      command response status
 *
 * @section description
 * The functionality of this function enable the uart debug prints 
 *
 * 
 */
int32_t rsi_common_debug_log(uint32_t assertion_type, uint32_t assertion_level)
{
  int32_t    status = RSI_SUCCESS;
  rsi_pkt_t             *pkt;
  rsi_debug_uart_print_t *debug_uart_print;
  //! Get common cb structure pointer
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;
	
  if(((assertion_type > 4) ||(assertion_type < 0)) ||((assertion_level <	0) || (assertion_level > 15)))
  { 
   return RSI_ERROR_INVALID_PARAM;
  }
  
  //! take lock on common control block
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer  from common pool
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

 //! Take the user provided data and fill it in debug uart print select structure
   debug_uart_print = (rsi_debug_uart_print_t *)pkt->data;

  //! assertion_type
  debug_uart_print->assertion_type = assertion_type;

  //! rf type
  debug_uart_print->assertion_level = assertion_level;
	
  //! Send firmware version query request
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_DEBUG_LOG, pkt);

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);

  //! Return the status
  return status;

}


/*==============================================*/
/**
 * @fn          void rsi_wireless_driver_task(void)
 * @brief       This function handles driver's events
 * @param[in]   None 
 * @param[out]  None
 * @return      void
 *
 *
 * @section description
 * This API is used to handle driver's events for non-OS platforms
 *
 *
 */

void rsi_wireless_driver_task(void)
{
#ifdef RSI_WITH_OS
  while(1)
#endif
  {
    rsi_scheduler(&rsi_driver_cb->scheduler_cb);  
#if (BT_HOST_SBC == 1)
    rsi_bt_a2dp_sbc_encode_task();
#endif

  }
}

//======================================================
/**
 *
 * @fn         rsi_driver_cb_t *rsi_driver_deinit()
 * @brief      De-Initializes the driver components.Clears all the memory 
 *             given for driver operations in rsi_driver_init() API
 *             in OS case USer need to take care of OS variables initialised in 
 *             driver_init .This API has to be called by the thread/task/Master 
 *             thread which is not dependent on OS variables allocated/initialised
 *             in rsi_driver_init() API
 * @param[in]   None
 * @param[out]  None
 * @return
 *              0 - success
 *              1 - Failure 
 * @section description
 * This fucntion de-intializes driver components
 *
 */
int32_t rsi_driver_deinit(void)
{
  uint32_t actual_length=0;
  //! check buffer is enough or not for driver components
  actual_length += sizeof(rsi_driver_cb_t) + RSI_DRIVER_RX_POOL_SIZE +
    sizeof(rsi_common_cb_t) + RSI_COMMON_POOL_SIZE +
    sizeof(rsi_wlan_cb_t) +  RSI_WLAN_POOL_SIZE    +
    sizeof(rsi_nwk_cb_t)
#ifdef RSI_M4_INTERFACE
    + sizeof(efuse_ipmu_t)
#endif
    + RSI_SOCKET_INFO_POOL_SIZE
    + RSI_SOCKET_INFO_POOL_ROM_SIZE
#ifdef RSI_ZB_ENABLE
    + sizeof(rsi_zigb_cb_t) + RSI_ZIGB_POOL_SIZE + sizeof(rsi_zigb_global_cb_t)
#endif
#if (defined RSI_BT_ENABLE || defined RSI_BLE_ENABLE)
    + sizeof(rsi_bt_cb_t) + RSI_BT_COMMON_POOL_SIZE
    + sizeof(rsi_bt_global_cb_t)
#endif
#ifdef RSI_BT_ENABLE
    + sizeof(rsi_bt_cb_t) + RSI_BT_CLASSIC_POOL_SIZE + sizeof(rsi_bt_classic_cb_t)
#endif
#ifdef RSI_BLE_ENABLE
    + sizeof(rsi_bt_cb_t) + RSI_BLE_POOL_SIZE + sizeof(rsi_ble_cb_t)
#endif
    ;
  if(rsi_driver_cb == NULL)
  {
    return RSI_FAILURE; 
  }
#ifndef RSI_M4_INTERFACE
  rsi_hal_intr_mask();
#else
 mask_ta_interrupt(RX_PKT_TRANSFER_DONE_INTERRUPT|TX_PKT_TRANSFER_DONE_INTERRUPT);
#endif

  rsi_semaphore_destroy(&rsi_driver_cb->common_cb->common_sem);
  rsi_mutex_destroy(&rsi_driver_cb->common_cb->common_mutex);
  rsi_mutex_destroy(&rsi_driver_cb_non_rom->tx_mutex);
#ifdef RSI_WLAN_ENABLE
  //! Create wlan semaphore
  rsi_semaphore_destroy(&rsi_driver_cb->wlan_cb->wlan_sem);
  rsi_mutex_destroy(&rsi_driver_cb->wlan_cb->wlan_mutex);
  rsi_socket_pool = NULL;
  rsi_socket_pool_non_rom = NULL;
#endif
#ifdef RSI_BT_ENABLE
  //! Create bt semaphore
  rsi_semaphore_destroy(&rsi_driver_cb->bt_common_cb->bt_sem);
  rsi_mutex_destroy(&rsi_driver_cb->bt_single_mutex);
#endif
#ifdef RSI_ZB_ENABLE
  rsi_semaphore_destroy(&rsi_driver_cb->zigb_cb->zigb_sem);
  rsi_mutex_destroy(&rsi_driver_cb->zigb_cb->zigb_mutex);
#endif
  //!memsetting the global structure
  memset((uint8_t *)rsi_driver_cb,0,actual_length);

  //! Clear the driver control block, assume application will free the buffer
  rsi_driver_cb = NULL;

  return RSI_SUCCESS;

}
/*=============================================================================*/
/**
 * @fn              int32_t rsi_driver_version(uint8_t *request)
 * @brief           get current driver version
 * @param[in]       uint8_t *request,pointer to fill driver version
 * @return          0- Success.
 */
#define RSI_DRIVER_VERSION "1.1.1a"
int32_t rsi_driver_version(uint8_t *request)
{
  memcpy(request ,(uint8_t *)RSI_DRIVER_VERSION,sizeof(RSI_DRIVER_VERSION));
  return 0;
}

//*==============================================*/
/**
 * @fn          int32_t rsi_get_ram_log(uint32_t addr,uint16_t length)
 * @brief       This API is used to get ram dump 
 * @param[in]   uint32_t addr  
 * @param[in]   uint16_t length
 * This API is used to get ram log on UART  
 *
 */
int32_t rsi_get_ram_log(uint32_t addr, uint32_t length)
{
  int32_t    status = RSI_SUCCESS;
  rsi_pkt_t             *pkt;
  rsi_ram_dump_t        *ram;
  rsi_common_cb_t *common_cb = rsi_driver_cb->common_cb;

  //! take lock on common control block
  rsi_mutex_lock(&common_cb->common_mutex);

  //! allocate command buffer  from common pool
  pkt = rsi_pkt_alloc(&common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

 //! Take the user provided data and fill it in ram_dump structure
  ram = (rsi_ram_dump_t *)pkt->data;
 //! address
  ram->addr = addr;
 //! length
  ram->length = length;
  //! Send ram dump request
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_GET_RAM_DUMP, pkt);

  //! free the packet
  rsi_pkt_free(&common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&common_cb->common_mutex);

  //! Return the status
  return status;

}

/*==============================================*/
/**
 *
 * @fn         int32_t rsi_driver_memory_estimate()
 * @brief      Provides the length of the application
 * @param[in]  None
 * @param[out] None
 * @return
 *       <=length - If success, returns the memory used, which is less or equal to buffer length provided
 *        >length - If fails, returns total memory required by driver
 * @section description
 * This fucntion Provides the length of the application
 *
 */
int32_t rsi_driver_memory_estimate()
{
  uint32_t        actual_length  = 0;

  //! Calculate the Memory length of the application
  actual_length += sizeof(rsi_driver_cb_t) + RSI_DRIVER_RX_POOL_SIZE +
    sizeof(rsi_common_cb_t) + RSI_COMMON_POOL_SIZE +
    sizeof(rsi_wlan_cb_t) +  RSI_WLAN_POOL_SIZE    +
    sizeof(rsi_nwk_cb_t)
#ifdef RSI_M4_INTERFACE
    + sizeof(efuse_ipmu_t)
#endif
    + RSI_SOCKET_INFO_POOL_SIZE
    + RSI_SOCKET_INFO_POOL_ROM_SIZE
#ifdef RSI_ZB_ENABLE
    + sizeof(rsi_zigb_cb_t) + RSI_ZIGB_POOL_SIZE + sizeof(rsi_zigb_global_cb_t)
#endif
#if (defined RSI_BT_ENABLE || defined RSI_BLE_ENABLE) 
    + sizeof(rsi_bt_cb_t) + RSI_BT_COMMON_POOL_SIZE
    + sizeof(rsi_bt_global_cb_t)
#endif
#ifdef RSI_BT_ENABLE
    + sizeof(rsi_bt_cb_t) + RSI_BT_CLASSIC_POOL_SIZE + sizeof(rsi_bt_classic_cb_t)
#endif
#ifdef RSI_BLE_ENABLE
    + sizeof(rsi_bt_cb_t) + RSI_BLE_POOL_SIZE + sizeof(rsi_ble_cb_t)
#endif
    ;
  return actual_length;

}
