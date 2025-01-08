/**
 * @file      rsi_common.c
 * @version   0.1
 * @date      15 Aug 2015
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains common(protocol independent) functionality
 *
 *  @section Description  This file contains common(protocol independent) functionality
 *
 *
 */

#include "rsi_driver.h"
#include "rsi_hal.h"
#ifdef RSI_M4_INTERFACE
#include "rsi_m4.h"
#endif
#include "rsi_wlan_non_rom.h"
#ifdef RSI_WITH_OS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
extern volatile TickType_t xTickCount;
#endif

/*==============================================*/
/**
 * @fn          int8_t rsi_common_cb_init(rsi_common_cb_t *common_cb)
 * @brief       Initializes common control block structure
 * @param[in]   common_cb , pointer to common cb structure 
 * @param[out]  None
 * @return      Success or Error
 *
 *
 * @section description
 * This function initializes common control block structure
 *
 *
 */

extern rsi_driver_cb_non_rom_t *rsi_driver_cb_non_rom;
int8_t rsi_common_cb_init(rsi_common_cb_t *common_cb)
{
  int8_t retval = RSI_ERR_NONE;

  //! validate input parameter
  if(common_cb == NULL)
  {
    return RSI_ERROR_INVALID_PARAM;
  }

  //! Initialize common state
  common_cb->state = RSI_COMMON_STATE_NONE;

  //! Initializes common status
  common_cb->status = RSI_SUCCESS;

  //! Creates common mutex
  rsi_mutex_create(&common_cb->common_mutex);
  
  //! Creates tx mutex
  rsi_mutex_create(&rsi_driver_cb_non_rom->tx_mutex);

  //! Set expected_response to zero
  common_cb->expected_response = RSI_COMMON_RSP_CLEAR;

  rsi_semaphore_create(&common_cb->common_sem, 0);
  
#ifdef RSI_WITH_OS
  rsi_semaphore_create(&common_cb->common_card_ready_sem, 0);
#endif
  return retval;
}
/*==============================================*/
/**
 * @fn          int32_t rsi_driver_common_send_cmd(rsi_wlan_cmd_request_t cmd, rsi_pkt_t *pkt)
 * @brief       This functions fills commands and places into wlan TX queue
 * @param[in]   cmd, type of the command to send 
 * @param[in]   pkt, pointer of packet to send
 * @param[out]
 * @return     
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This functions fills commands and places into wlan TX queue 
 *
 *
 */
int32_t rsi_driver_common_send_cmd(rsi_common_cmd_request_t cmd, rsi_pkt_t *pkt)
{
  uint16_t payload_size = 0;
  int32_t  status       = RSI_SUCCESS;

  //! Get host descriptor pointer
  uint8_t *host_desc = (pkt->desc);
#ifdef RSI_PUF_ENABLE
  if(!(((cmd >= RSI_COMMON_REQ_PUF_ENROLL) && ( cmd <= RSI_COMMON_REQ_AES_MAC))||(cmd == RSI_COMMON_REQ_PUF_INTR_KEY)))
#endif
#ifdef RSI_CRYPTO_ENABLE
    if(!(cmd == RSI_COMMON_REQ_ENCRYPT_CRYPTO))
#endif

    {
      //! Memset host descriptor
      memset(host_desc, 0, RSI_HOST_DESC_LENGTH);
    }

  switch(cmd)
  {
#ifdef RSI_CHIP_MFG_EN
  case RSI_COMMON_DEV_CONFIG:
        {
          //! fill payload size
          payload_size = sizeof(common_dev_config_params_t);
        }
        break;
#endif
    case RSI_COMMON_REQ_OPERMODE:
      {
        //! opermode Parameters 
        rsi_opermode_t *rsi_opermode = (rsi_opermode_t *)pkt->data;

        //! fill other parameters from configuration file
        rsi_uint32_to_4bytes(rsi_opermode->feature_bit_map, RSI_FEATURE_BIT_MAP);
        rsi_uint32_to_4bytes(rsi_opermode->tcp_ip_feature_bit_map, RSI_TCP_IP_FEATURE_BIT_MAP);
        rsi_uint32_to_4bytes(rsi_opermode->custom_feature_bit_map, RSI_CUSTOM_FEATURE_BIT_MAP);
        rsi_uint32_to_4bytes(rsi_opermode->ext_custom_feature_bit_map, RSI_EXT_CUSTOM_FEATURE_BIT_MAP);
        rsi_uint32_to_4bytes(rsi_opermode->ext_tcp_ip_feature_bit_map, RSI_EXT_TCPIP_FEATURE_BITMAP);
        rsi_uint32_to_4bytes(rsi_opermode->config_feature_bit_map, RSI_CONFIG_FEATURE_BITMAP);
#ifdef RSI_ENABLE_DEMOS		
        rsi_uint32_to_4bytes(rsi_opermode->bt_feature_bit_map, RSI_BT_FEATURE_BITMAP);
#endif		
#if (defined RSI_BLE_ENABLE || defined RSI_BT_ENABLE)
        if ((((rsi_bytes4R_to_uint32(rsi_opermode->opermode) >> 16) & 0xFFFF) == RSI_OPERMODE_WLAN_BLE)  ||
            (((rsi_bytes4R_to_uint32(rsi_opermode->opermode) >> 16) & 0xFFFF) == RSI_OPERMODE_WLAN_BT_CLASSIC) ||
            (((rsi_bytes4R_to_uint32(rsi_opermode->opermode) >> 16) & 0xFFFF) == RSI_OPERMODE_WLAN_BT_DUAL_MODE))
        {
          rsi_opermode->custom_feature_bit_map[3] |= 0x80;
          rsi_opermode->ext_custom_feature_bit_map[3] |= 0x80;
#ifndef RSI_ENABLE_DEMOS
#if (defined A2DP_POWER_SAVE_ENABLE)
          rsi_opermode->ext_custom_feature_bit_map[2] |= 0x40;
#endif
#endif
#if (( defined RSI_BT_ENABLE) && (defined IAP_PROFILE_ENABLE))
    rsi_opermode->bt_feature_bit_map[3] |= 0x40;  
#endif
#ifdef RSI_BLE_ENABLE
          rsi_opermode->bt_feature_bit_map[3] |= 0x80;
          rsi_uint32_to_4bytes(rsi_opermode->ble_feature_bit_map, ((RSI_BLE_MAX_NBR_SLAVES << 12) | (RSI_BLE_MAX_NBR_MASTERS << 27) | (RSI_BLE_MAX_NBR_ATT_SERV << 8) | RSI_BLE_MAX_NBR_ATT_REC));

          /*BLE custom feature bitmap*/
          rsi_opermode->ble_feature_bit_map[3] |= 0x80;
          rsi_uint32_to_4bytes(rsi_opermode->ble_ext_feature_bit_map, (RSI_BLE_NUM_CONN_EVENTS) | (RSI_BLE_NUM_REC_BYTES << 5) | (RSI_BLE_GATT_INIT << 13));

#endif
#if ((defined RSI_BT_ENABLE) && (defined  A2DP_PROFILE_ENABLE))
          rsi_opermode->bt_feature_bit_map[2] |=  (1 << 7);
#ifndef RSI_ENABLE_DEMOS
#if (A2DP_PROFILE_ROLE == A2DP_SOURCE_ROLE)
          rsi_opermode->bt_feature_bit_map[3] |=  BIT(0);
#if (!(RSI_AUDIO_DATA_TYPE == SBC_AUDIO))
          rsi_opermode->bt_feature_bit_map[3] |=  BIT(1);
#endif
#else
          rsi_opermode->bt_feature_bit_map[3] &= ~BIT(0);
#if (!(RSI_AUDIO_DATA_TYPE == SBC_AUDIO))
          rsi_opermode->bt_feature_bit_map[3] |=  BIT(1);
#else
          rsi_opermode->bt_feature_bit_map[3] &=  ~BIT(1);
#endif
#endif
#endif
#endif
          rsi_opermode->bt_feature_bit_map[3] |=  (RSI_RF_TYPE<<6);
#ifdef RSI_BT_ENABLE
          rsi_opermode->bt_feature_bit_map[0] |= BT_BDR_MODE;
#endif
#ifndef RSI_ENABLE_DEMOS
#if (defined BT_PWR_CTRL_DISABLE)
          rsi_opermode->bt_feature_bit_map[0] |= (BT_PWR_CTRL << 2);
          rsi_opermode->bt_feature_bit_map[0] |= (BT_PWR_INX << 3);
#endif
#endif

#ifdef RSI_BLE_ENABLE
          rsi_opermode->ble_feature_bit_map[2] |= RSI_BLE_PWR_INX ;
          rsi_opermode->ble_feature_bit_map[3] |=  RSI_BLE_PWR_SAVE_OPTIONS;
          rsi_opermode->ble_feature_bit_map[3] |= (1 << 6);
          rsi_opermode->ble_feature_bit_map[3] |= (RSI_BLE_GATT_ASYNC_ENABLE << 5);
#endif
#if (RSI_BT_ENABLE && RSI_BLE_ENABLE && RSI_BT_GATT_ON_CLASSIC)
          rsi_opermode->bt_feature_bit_map[3] |= (1 << 5);   /* to support att over classic acl link */
#endif

        }
#endif

        //! fill payload size
        payload_size = sizeof(rsi_opermode_t);
      }
      break;
    case RSI_COMMON_REQ_ANTENNA_SELECT:
      {
        //! fill payload size
        payload_size = sizeof(rsi_antenna_select_t);
      }
      break;
    case RSI_COMMON_REQ_DEBUG_LOG:
      {
        //! fill payload size
        payload_size = sizeof(rsi_debug_uart_print_t);
      }
      break;
    case RSI_COMMON_REQ_GET_RAM_DUMP:
    {
     payload_size = sizeof(rsi_ram_dump_t); 
    }
    break;
    case RSI_COMMON_REQ_FEATURE_FRAME:
      {
        //! fill payload size
        payload_size = sizeof(rsi_feature_frame_t);
      }
      break;
    case RSI_COMMON_REQ_SOFT_RESET:
      {
        rsi_driver_cb->common_cb->state  = RSI_COMMON_STATE_NONE;

      }			
      break;

    case RSI_COMMON_REQ_UART_FLOW_CTRL_ENABLE:
      {
        payload_size = 1;
      }
      break;
    case RSI_COMMON_REQ_PWRMODE:
      {

        //! Power mode setting
        rsi_power_save_req_t *power_mode = (rsi_power_save_req_t *)pkt->data;
        if(power_mode->power_mode != RSI_POWER_MODE_DISABLE)
        {

          //! set LP/ULP/ULP-without RAM retention
          power_mode->ulp_mode_enable = RSI_SELECT_LP_OR_ULP_MODE;

          //! set DTIM aligment required
          //! 0 - module wakes up at beacon which is just before or equal to listen_interval
          //! 1 - module wakes up at DTIM beacon which is just after or equal to listen_interval
          power_mode->dtim_aligned_type = RSI_DTIM_ALIGNED_TYPE;

          //! Monitor interval for the FAST PSP mode
          //! default is 50 ms, and this parameter is valid for FAST PSP only
          power_mode->monitor_interval = RSI_MONITOR_INTERVAL;
          power_mode->num_of_dtim_skip = RSI_NUM_OF_DTIM_SKIP;

        }

        if(power_mode->ulp_mode_enable == RSI_ULP_WITHOUT_RAM_RET)
        {
          rsi_driver_cb->common_cb->state  = RSI_COMMON_STATE_NONE;
        }

        //! fill payload size
        payload_size = sizeof(rsi_power_save_req_t);
      }
      break;


#ifdef RSI_WAC_MFI_ENABLE
    case RSI_COMMON_REQ_IAP_INIT:
      break;
    case  RSI_COMMON_REQ_IAP_GENERATE_SIGATURE:
      {
        payload_size = sizeof(rsi_mfi_auth_create_request_t);
      }
      break;
    case RSI_COMMON_REQ_IAP_GET_CERTIFICATE:
      break;
#endif

#ifdef RSI_PUF_ENABLE
    case  RSI_COMMON_REQ_PUF_ENROLL:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_PUF_DIS_ENROLL:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_PUF_START:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_PUF_SET_KEY:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_PUF_DIS_SET_KEY:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_PUF_GET_KEY:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_PUF_DIS_GET_KEY:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_PUF_LOAD_KEY:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_AES_ENCRYPT:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_AES_DECRYPT:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_AES_MAC:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
    case  RSI_COMMON_REQ_PUF_INTR_KEY:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
#endif
#ifdef RSI_CRYPTO_ENABLE
    case  RSI_COMMON_REQ_ENCRYPT_CRYPTO:
      {
        payload_size = rsi_bytes2R_to_uint16(host_desc);
      }
      break;
#endif
#ifdef RSI_M4_INTERFACE		
    case  RSI_COMMON_REQ_TA_M4_COMMANDS:
      {
        payload_size = sizeof(ta_m4_handshake_param_t);
      }
      break;
#endif
    default:
      {

      }
  }
  //! Fill payload length
  rsi_uint16_to_2bytes(host_desc, (payload_size & 0xFFF));

#ifdef RSI_CHIP_MFG_EN
  //! common dev config should go in common queue
  if(cmd != RSI_COMMON_DEV_CONFIG)
#endif
  //! Fill frame type
  host_desc[1] |= (RSI_WLAN_MGMT_Q << 4);

  //! Fill frame type
  host_desc[2] = cmd;

  if((cmd == RSI_COMMON_REQ_PWRMODE) ||(cmd == RSI_COMMON_REQ_OPERMODE) ||(cmd == RSI_COMMON_REQ_SOFT_RESET))  
  {
#ifndef RSI_CHIP_MFG_EN
    //! Block WLAN TX queue
    rsi_block_queue(&rsi_driver_cb->wlan_tx_q);
#endif
#ifdef RSI_ZB_ENABLE
    //! Block ZB TX queue
    rsi_block_queue(&rsi_driver_cb->zigb_tx_q);
#endif
#if (defined RSI_BT_ENABLE ||defined RSI_BLE_ENABLE)
    //! Block BT common TX queue
    rsi_block_queue(&rsi_driver_cb->bt_common_tx_q);
#ifdef RSI_BT_ENABLE
    //! Block BT TX queue
    rsi_block_queue(&rsi_driver_cb->bt_classic_tx_q);
#endif  
#ifdef RSI_BLE_ENABLE
    //! Block BLE TX queue
    rsi_block_queue(&rsi_driver_cb->ble_tx_q);
#endif
#endif
  }

  if(rsi_driver_cb->common_cb->expected_response != (rsi_common_cmd_response_t )RSI_WLAN_RSP_ASYNCHRONOUS)
  {
    rsi_driver_cb->common_cb->expected_response = (rsi_common_cmd_response_t )cmd;
  }
  //! Enqueue packet to common TX queue
  rsi_enqueue_pkt(&rsi_driver_cb->common_tx_q, pkt);

  //! Set TX packet pending event
  rsi_set_event(RSI_TX_EVENT);

  //! Wait on common semaphore 
  rsi_semaphore_wait(&rsi_driver_cb->common_cb->common_sem, RSI_WAIT_FOREVER);    

  if(rsi_driver_cb->common_cb->expected_response == (rsi_common_cmd_response_t )RSI_WLAN_RSP_ASYNCHRONOUS)
  {
    rsi_driver_cb->common_cb->expected_response = (rsi_common_cmd_response_t )cmd;
  }
  //! get common cmd response status
  status =  rsi_common_get_status();
  //! Return status
  return status;
}

/*==============================================*/
/**
 * @fn          int32_t rsi_driver_process_common_recv_cmd(rsi_pkt_t *pkt)
 * @brief       Processes commands related to common block
 * @param[in]   pkt, pointer to common block packet 
 * @param[out]  None
 * @return    
 *              Non zero - If fails
 *              0 - If success
 *
 * @section description
 * This function processes commands related to common block
 *
 *
 */
int32_t rsi_driver_process_common_recv_cmd(rsi_pkt_t *pkt)
{
  uint8_t  cmd_type;
  uint16_t status     = RSI_SUCCESS;
  uint8_t  *host_desc = pkt->desc;
  uint16_t copy_length;
  uint8_t  *payload;
  uint16_t payload_length;
  uint8_t i = 0;

  //! Get commmon cb pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get wlan cb pointer
  rsi_wlan_cb_t *rsi_wlan_cb = rsi_driver_cb->wlan_cb;

  //! Get command type
  cmd_type = pkt->desc[2];

  //! Get status
  status = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);

  //! Get payload pointer
  payload = pkt->data;

  //! Get payoad length
  payload_length = (rsi_bytes2R_to_uint16(host_desc) & 0xFFF);

#ifdef RSI_WLAN_ENABLE
  //! Set wlan status
  rsi_wlan_set_status(status);
#endif

  switch(cmd_type)
  {
#ifdef RSI_CHIP_MFG_EN
  	//! nlink card ready frame
  	case RSI_COMMON_RSP_CLEAR:
#endif
    case RSI_COMMON_RSP_CARDREADY:
      {
        //! if success, update state common_cb state 
        if(status == RSI_SUCCESS)
        {
          rsi_common_cb->state = RSI_COMMON_CARDREADY;
#ifdef RSI_CHIP_MFG_EN
          //! reset the common_sem
          rsi_driver_cb->common_cb->common_sem = 0;
#endif
          //! Check for auto config status
          if(host_desc[15] & RSI_AUTO_CONFIG_GOING_ON)
          {
            //! Set auto config failed status
            rsi_wlan_cb->auto_config_state = RSI_WLAN_STATE_AUTO_CONFIG_GOING_ON; 

            //! Check for auto config handler
            if(rsi_wlan_cb->callback_list.auto_config_rsp_handler)
            {
              rsi_wlan_cb->callback_list.auto_config_rsp_handler(status, RSI_AUTO_CONFIG_GOING_ON);
            }
          }
          //! Check for auto config status
          else if(host_desc[15] & RSI_AUTO_CONFIG_FAILED)
          {
            //! Set auto config failed status
            rsi_wlan_cb->auto_config_state = RSI_WLAN_STATE_AUTO_CONFIG_FAILED; 

            //! Set auto config failed status
            //!FIXME:

            //! Check for auto config handler
            if(rsi_wlan_cb->callback_list.auto_config_rsp_handler)
            {
              rsi_wlan_cb->callback_list.auto_config_rsp_handler(status, RSI_AUTO_CONFIG_FAILED);
            }
          }

        }
      }
      break;

    case RSI_COMMON_RSP_OPERMODE:
      {
        //! check status
        status  = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);

        //! if success update state common_cb state and wlan_cb state to OPERMODE done
        if(status == RSI_SUCCESS)
        {
          //! Update wlan state to OPERMODE done
          rsi_wlan_cb->state = RSI_WLAN_STATE_OPERMODE_DONE;

          rsi_common_cb->state = RSI_COMMON_OPERMODE_DONE; 

          //! unblock protocol queues
          rsi_unblock_queue(&rsi_driver_cb->wlan_tx_q);

#ifdef RSI_ZB_ENABLE
          //! unblock ZB TX queue
          rsi_unblock_queue(&rsi_driver_cb->zigb_tx_q);
#endif
#if (defined RSI_BT_ENABLE ||defined RSI_BLE_ENABLE)
          //! unblock BT common TX queue
          rsi_unblock_queue(&rsi_driver_cb->bt_common_tx_q);
#ifdef RSI_BT_ENABLE
          //! unblock BT TX queue
          rsi_unblock_queue(&rsi_driver_cb->bt_classic_tx_q);
#endif 
#ifdef RSI_BLE_ENABLE
          //! unblock BLE TX queue
          rsi_unblock_queue(&rsi_driver_cb->ble_tx_q);
#endif
#endif

        }
      }
      break;
    case RSI_COMMON_RSP_PWRMODE:
    {
    	//! check status
    	status  = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);

    	//! if success update state common_cb state and wlan_cb state to OPERMODE done
    	if(status == RSI_SUCCESS)
    	{
    		//! unblock protocol queues
    		rsi_unblock_queue(&rsi_driver_cb->wlan_tx_q);

#ifdef RSI_ZB_ENABLE
    		//! unblock ZB TX queue
    		rsi_unblock_queue(&rsi_driver_cb->zigb_tx_q);
#endif
#if (defined RSI_BT_ENABLE ||defined RSI_BLE_ENABLE)
    		//! unblock BT TX queue
    		rsi_unblock_queue(&rsi_driver_cb->bt_common_tx_q);
#ifdef RSI_BT_ENABLE
          //! unblock BT TX queue
          rsi_unblock_queue(&rsi_driver_cb->bt_classic_tx_q);
#endif
#ifdef RSI_BLE_ENABLE
    		//! unblock BLE TX queue
    		rsi_unblock_queue(&rsi_driver_cb->ble_tx_q);
#endif
#endif
    		

    		//! Flush module state
    		rsi_common_cb->power_save.module_state = RSI_IDLE;
    	}
    }
      break;
    case RSI_COMMON_RSP_ANTENNA_SELECT:
    {
    	//! check status
    	status  = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);
    }
      break;
    case RSI_COMMON_RSP_DEBUG_LOG:
      {
        //! check status
        status  = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);
      }
      break;
    case RSI_COMMON_RSP_FEATURE_FRAME:
    {
    	//! check status
    	status  = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);
    }
      break;
      

    case RSI_COMMON_RSP_SOFT_RESET:
    {
    	//! check status
    	status  = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);

    	//! check common_cb for any task is waiting for response
      //! No need to check for expected response as all variables will be reseted after reset command
      //if(rsi_common_cb->expected_response == cmd_type )
      {
        //! state update
        rsi_common_cb->state = RSI_COMMON_CARDREADY;
        rsi_wlan_cb->state  = RSI_WLAN_STATE_NONE;
#ifdef RSI_WLAN_ENABLE       
        //! added this loop for socket pool not memset/clear sometime while deinit/reset. For more info.
        for(i = 0; i < NUMBER_OF_SOCKETS; i++)
        {
          //! Memset socket info
          memset(&rsi_socket_pool[i], 0, sizeof(rsi_socket_info_t));
        }
#endif
         //! Check for auto config status
          if(host_desc[15] & RSI_AUTO_CONFIG_GOING_ON)
          {
            //! Set auto config failed status
            rsi_wlan_cb->auto_config_state = RSI_WLAN_STATE_AUTO_CONFIG_GOING_ON; 

            //! Check for auto config handler
            if(rsi_wlan_cb->callback_list.auto_config_rsp_handler)
            {
              rsi_wlan_cb->callback_list.auto_config_rsp_handler(status, RSI_AUTO_CONFIG_GOING_ON);
            }
          }
          //! Check for auto config status
          else if(host_desc[15] & RSI_AUTO_CONFIG_FAILED)
          {
            //! Set auto config failed status
            rsi_wlan_cb->auto_config_state = RSI_WLAN_STATE_AUTO_CONFIG_FAILED; 

            //! Set auto config failed status
            //!FIXME:

            //! Check for auto config handler
            if(rsi_wlan_cb->callback_list.auto_config_rsp_handler)
            {
              rsi_wlan_cb->callback_list.auto_config_rsp_handler(status, RSI_AUTO_CONFIG_FAILED);
            }
          }

      }
    }
    break;
#ifdef RSI_CRYPTO_ENABLE
	case RSI_RSP_ENCRYPT_CRYPTO:
      {
          if(status == RSI_SUCCESS)
          {
              if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
              {
                copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
                memcpy(rsi_common_cb->app_buffer, payload, copy_length);
              }
          }

      }
      break;
#endif	
#ifdef RSI_WAC_MFI_ENABLE
    case RSI_COMMON_RSP_IAP_INIT:
         break;
    case RSI_COMMON_RSP_IAP_GET_CERTIFICATE:
    case RSI_COMMON_RSP_IAP_GENERATE_SIGATURE:
    {
      //! check status
      status  = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);

      //! Get payoad length
      payload_length = (rsi_bytes2R_to_uint16(host_desc) & 0xFFF);

      //! Get payload pointer
      payload = pkt->data;

      //! if success update state common_cb state and wlan_cb state to OPERMODE done
      if(status == RSI_SUCCESS)
      {

        if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
        {
          copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
          memcpy(rsi_common_cb->app_buffer, payload, copy_length);
          rsi_common_cb->app_buffer = NULL;
        }
      }

    }
    break;
#endif

#ifdef RSI_PUF_ENABLE
    case RSI_COMMON_RSP_PUF_ENROLL:
      {
          if(status == RSI_SUCCESS)
          {
        	  rsi_common_cb->puf_state = RSI_PUF_STATE_STARTED;
          }
      }
      break;
    case RSI_COMMON_RSP_PUF_DIS_ENROLL:
      {
          if(status == RSI_SUCCESS)
          {
          }
      }
      break;
    case RSI_COMMON_RSP_PUF_START:
      {
          if(status == RSI_SUCCESS)
          {
        	  rsi_common_cb->puf_state = RSI_PUF_STATE_STARTED;
              //! check common_cb for any task is waiting for response
          }
      }
      break;
    case RSI_COMMON_RSP_PUF_SET_KEY:
      {
          if(status == RSI_SUCCESS)
          {
        	  rsi_common_cb->puf_state = RSI_PUF_STATE_SET_KEY;
              if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
              {
                copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
                memcpy(rsi_common_cb->app_buffer, payload, copy_length);
                rsi_common_cb->app_buffer = NULL;
              }
          }

      }
      break;
    case RSI_COMMON_RSP_PUF_DIS_SET_KEY:
      {
          if(status == RSI_SUCCESS)
          {
          }
      }
      break;

    case RSI_COMMON_RSP_PUF_GET_KEY:
      {
          if(status == RSI_SUCCESS)
          {
              if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
              {
                copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
                memcpy(rsi_common_cb->app_buffer, payload, copy_length);
                rsi_common_cb->app_buffer = NULL;
              }
          }
      }
      break;
    case RSI_COMMON_RSP_PUF_DIS_GET_KEY:
      {
          if(status == RSI_SUCCESS)
          {
          }
      }
      break;
    case RSI_COMMON_RSP_PUF_LOAD_KEY:
      {
          if(status == RSI_SUCCESS)
          {
        	  rsi_common_cb->puf_state = RSI_PUF_STATE_KEY_LOADED;
          }
      }
      break;
    case RSI_COMMON_RSP_AES_ENCRYPT:
      {
          if(status == RSI_SUCCESS)
          {
              if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
              {
                copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
                memcpy(rsi_common_cb->app_buffer, payload, copy_length);
                rsi_common_cb->app_buffer = NULL;
              }
          }
      }
      break;
    case RSI_COMMON_RSP_AES_DECRYPT:
      {
          if(status == RSI_SUCCESS)
          {
              if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
              {
                copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
                memcpy(rsi_common_cb->app_buffer, payload, copy_length);
                rsi_common_cb->app_buffer = NULL;
              }
          }
      }
      break;
    case RSI_COMMON_RSP_AES_MAC:
      {
          if(status == RSI_SUCCESS)
          {
              if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
              {
                copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
                memcpy(rsi_common_cb->app_buffer, payload, copy_length);
                rsi_common_cb->app_buffer = NULL;
              }
          }
      }
      break;
		case RSI_COMMON_RSP_PUF_INTR_KEY:
      {
          if(status == RSI_SUCCESS)
          {
              if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
              {
                copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
                memcpy(rsi_common_cb->app_buffer, payload, copy_length);
                rsi_common_cb->app_buffer = NULL;
              }
          }

      }
      break;

#endif

    case RSI_COMMON_RSP_FW_VERSION:
      {

        if(status == RSI_SUCCESS)
        {
          //! check the length of application buffer and copy firmware version
          if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
          {
            copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
            memcpy(rsi_common_cb->app_buffer, payload, copy_length); 
            rsi_common_cb->app_buffer = NULL;
          }
        }

      }
      break;

    case RSI_COMMON_RSP_TA_M4_COMMANDS:
      {
        //! check status
        status  = rsi_bytes2R_to_uint16(host_desc + RSI_STATUS_OFFSET);

        //! Get payoad length
        payload_length = (rsi_bytes2R_to_uint16(host_desc) & 0xFFF);

        //! Get payload pointer
        payload = pkt->data;

        //! if success update state common_cb state and wlan_cb state to OPERMODE done
        if(status == RSI_SUCCESS)
        {

          if((rsi_common_cb->app_buffer != NULL) && (rsi_common_cb->app_buffer_length != 0))
          {
            copy_length = (payload_length < rsi_common_cb->app_buffer_length) ? (payload_length) : (rsi_common_cb->app_buffer_length);
            memcpy(rsi_common_cb->app_buffer, payload, copy_length);
            rsi_common_cb->app_buffer = NULL;
          }
        }
      }
      break;

  }
#ifdef RSI_WITH_OS
  if(cmd_type == RSI_COMMON_RSP_CARDREADY)
  {
		rsi_semaphore_post(&rsi_common_cb->common_card_ready_sem);
  }
#endif
  if((cmd_type != RSI_COMMON_RSP_CARDREADY) 
#ifdef RSI_CHIP_MFG_EN
	  && (cmd_type != RSI_COMMON_RSP_CLEAR)
#endif
  )
  {
      if(rsi_common_cb->expected_response == cmd_type )
       {
         //!  clear expected command
         rsi_common_cb->expected_response = RSI_COMMON_RSP_CLEAR;

         //! signal the common_cb semaphore
         rsi_semaphore_post(&rsi_common_cb->common_sem);

         //! Update Status
         rsi_common_cb->status = status;
       }
  }
 
  return RSI_SUCCESS;
}


/*=================================================*/
/**
 * @fn          void rsi_handle_slp_wkp(uint8_t frame_type)
 * @brief       To handle slp and wkp.
 * @param[in]   uint8_t frame_type
 * @param[out]  none
 * @return      none
 * @section description
 * This API is used to handle slp and wkp.
 */

void rsi_handle_slp_wkp(uint8_t frame_type)
{
  uint8_t i = 0;
  //!Get commmon cb pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get wlan cb pointer
  rsi_wlan_cb_t *rsi_wlan_cb = rsi_driver_cb->wlan_cb;


  //! Handle them here based on power save state machine
  switch (frame_type)
  {
    case RSI_RSP_SLP:
      {
        rsi_common_cb->power_save.module_state = RSI_SLP_RECEIVED;
        rsi_set_event(RSI_TX_EVENT);
      }
      break;
    case RSI_RSP_WKP:
      {
        rsi_common_cb->power_save.module_state = RSI_WKUP_RECEIVED;
#if (RSI_HAND_SHAKE_TYPE == MSG_BASED)
        if(rsi_common_cb->power_save.current_ps_mode == RSI_MSG_BASED_DEEP_SLEEP)
        {
          rsi_common_cb->power_save.module_state = RSI_SLP_RECEIVED;
		  rsi_set_event(RSI_TX_EVENT);
        }
#endif
        rsi_unmask_event(RSI_TX_EVENT);
      }
      break;
    case RSI_COMMON_RSP_ULP_NO_RAM_RETENTION:
      {
        //! state update
        rsi_common_cb->state = RSI_COMMON_CARDREADY;
        rsi_wlan_cb->state  = RSI_WLAN_STATE_NONE;
#ifdef RSI_WLAN_ENABLE
        //! added this loop for socket pool not memset/clear sometime while deinit/reset. For more info (RSC-4333) .
        for(i = 0; i < NUMBER_OF_SOCKETS; i++)
        {
          //! Memset socket info
          memset(&rsi_socket_pool[i], 0, sizeof(rsi_socket_info_t));
        }
#endif
        rsi_allow_sleep();
        memset(&( rsi_common_cb->power_save),0,sizeof(rsi_common_cb->power_save));
        rsi_unmask_event(RSI_TX_EVENT);        
      }
      break;
  }

}

/*====================================================*/
/**
 * @fn        int8_t rsi_req_wakeup()
 * @brief     set wakeup gpio high.
 * @param[in] none
 * @return    0 - success -1 -failure
 * @section description
 * This HAL API is used to set wakeup gpio high and waits for wakeup confirmation pin to get set.
 */
int8_t rsi_req_wakeup()
{
#ifdef RSI_WITH_OS
  uint32_t start;
#endif
#ifndef RSI_M4_INTERFACE
#if (RSI_SELECT_LP_OR_ULP_MODE != RSI_LP_MODE)
  rsi_hal_set_gpio(RSI_HAL_SLEEP_CONFIRM_PIN);
#else
  rsi_hal_set_gpio(RSI_HAL_LP_SLEEP_CONFIRM_PIN);
#endif
#ifdef RSI_WITH_OS
  start = xTickCount;
#endif
  do{
    if(rsi_hal_get_gpio(RSI_HAL_WAKEUP_INDICATION_PIN))
    {
#if (RSI_SELECT_LP_OR_ULP_MODE != RSI_LP_MODE)
#ifdef RSI_SPI_INTERFACE
      rsi_ulp_wakeup_init();
#endif
#endif
      break;
    }
  }
#ifdef RSI_WITH_OS
  while (!RSI_WAIT_TIME || ((xTickCount - start) < RSI_WAIT_TIME));
  if(RSI_WAIT_TIME){
    if(((xTickCount - start) == RSI_WAIT_TIME)){
      return -1;
    }
    }  
#else
  while(1);
#endif
#endif
  return 0;
}


/*====================================================*/
/**
 * @fn         int8_t rsi_wait4wakeup()
 * @brief      wait to get wakeup status to get set by module.
 * @param[in]  none
 * @return     0 - success -1 -failure
 * @description
 * This HAL API is used to set wakeup gpio high and waits for wakeup confirmation pin to get set.
 */
int8_t rsi_wait4wakeup(void)
{
  uint32_t timer_instance;
#ifndef RSI_M4_INTERFACE
  rsi_init_timer(&timer_instance, RSI_WAIT_TIME);    
  do
  {
    if(rsi_hal_get_gpio(RSI_HAL_WAKEUP_INDICATION_PIN))
    {
#if (RSI_SELECT_LP_OR_ULP_MODE != RSI_LP_MODE)
      rsi_hal_set_gpio(RSI_HAL_SLEEP_CONFIRM_PIN);
#ifdef RSI_SPI_INTERFACE
      rsi_ulp_wakeup_init();
#endif
#else
      rsi_hal_set_gpio(RSI_HAL_LP_SLEEP_CONFIRM_PIN);
#endif
      break;
    }
    if(rsi_timer_expired(&timer_instance))
    {
      return RSI_ERROR_GPIO_WAKEUP_TIMEOUT;
  }
  }while(1);
#endif
  return RSI_SUCCESS;
}

/*====================================================*/
/**
 * @fn        void rsi_allow_sleep(void)
 * @brief     set wakeup gpio low.
 * @param[in] none
 * @return    none
 * @description
 * This HAL API is used to set wakeup gpio low .
 */
void rsi_allow_sleep(void)
{

#ifdef RSI_M4_INTERFACE
		P2P_STATUS_REG &= ~M4_wakeup_TA;
#else
#if (RSI_SELECT_LP_OR_ULP_MODE != RSI_LP_MODE)
	rsi_hal_clear_gpio(RSI_HAL_SLEEP_CONFIRM_PIN);
#else
	rsi_hal_clear_gpio(RSI_HAL_LP_SLEEP_CONFIRM_PIN);
#endif
#endif
}

/*====================================================*/
/**
 * @fn        void rsi_powersave_gpio_init(void)
 * @brief     Initializes GPIOs used in power save
 * @param[in] none
 * @return    none
 * @description
 * This HAL API is used to initializes GPIOs used in power save .
 */
void rsi_powersave_gpio_init(void)
{
#ifndef RSI_M4_INTERFACE
	rsi_hal_config_gpio(RSI_HAL_WAKEUP_INDICATION_PIN, 0, 0);
	rsi_hal_set_gpio(RSI_HAL_RESET_PIN);
#if (RSI_SELECT_LP_OR_ULP_MODE != RSI_LP_MODE)
	rsi_hal_config_gpio(RSI_HAL_SLEEP_CONFIRM_PIN, 1, 0);
	rsi_hal_clear_gpio(RSI_HAL_SLEEP_CONFIRM_PIN);
#else
	rsi_hal_config_gpio(RSI_HAL_LP_SLEEP_CONFIRM_PIN, 1, 0);
	rsi_hal_clear_gpio(RSI_HAL_LP_SLEEP_CONFIRM_PIN);
#endif
#endif
}

/*==============================================*/
/**
 * @fn          void rsi_common_packet_transfer_done(rsi_pkt_t *pkt)
 * @brief       Handles packet transfer completion which has an asyncronous response
 * @param[in]   pkt, pointer to packet 
 * @param[out]  None
 * @return      void
 *
 * @section description
 * This function handles packet transfer completion
 *
 */
void rsi_common_packet_transfer_done(rsi_pkt_t *pkt)
{

  if(rsi_driver_cb->common_cb->expected_response == RSI_COMMON_RSP_ASYNCHRONOUS)
  {
#ifdef RSI_WLAN_ENABLE
    //! Set wlan status as success
    rsi_wlan_set_status(RSI_SUCCESS);
#endif

    //! Post the semaphore 
    rsi_semaphore_post(&rsi_driver_cb->common_cb->common_sem);

  }
}


/*====================================================*/
/**
 * @fn          int32_t rsi_sleep_mode_decision(rsi_common_cb_t *rsi_common_cb)
 * @brief       This is a handler which decides the power
 *              mode to program and change 
 * @param[in]   rsi_common_cb ,pointer to the common control block
 * @return       0 = not to program
 *               1 = program non-connected sleep
 *               2 = program connected sleep
 * @section description 
 * This function will send the power mode command.
 */
int32_t rsi_sleep_mode_decision(rsi_common_cb_t *rsi_common_cb)
{
  int32_t status           = RSI_SUCCESS;
  uint8_t selected_ps_mode = 0;
  uint8_t selected_ps_type = 0;
  uint8_t mode_decision = 0;

  switch((rsi_common_cb->ps_coex_mode & 0xF))
  {
    case 1:
      {
        mode_decision = ((rsi_common_cb->power_save.wlan_psp_mode << 4) | (rsi_common_cb->power_save.wlan_psp_mode));
      }
      break;
    case 4:
    case 12:
    case 8:
      {
        mode_decision = ((rsi_common_cb->power_save.bt_psp_mode << 4) | (rsi_common_cb->power_save.bt_psp_mode));
      }
      break;
    case 5:
    case 9:
    case 13:
      {
        mode_decision = ((rsi_common_cb->power_save.wlan_psp_mode << 4) | (rsi_common_cb->power_save.bt_psp_mode));
      }
      break;
     case 3:
      {
         mode_decision = ((rsi_common_cb->power_save.wlan_psp_mode << 4) | (rsi_common_cb->power_save.zb_psp_mode));
      }
      break;
    default:
      break;
  }

  switch(mode_decision)
  {
    case 0x00:
    case 0x10:
    case 0x01:
    case 0x20:
    case 0x02:
    case 0x80:
    case 0x08:
      {
        selected_ps_mode = RSI_ACTIVE;
      }
      break;
    case 0x88:
      {
        //! Update power save profile mode
#if (RSI_HAND_SHAKE_TYPE == GPIO_BASED)
        
		selected_ps_mode = RSI_GPIO_BASED_DEEP_SLEEP;
				
#elif (RSI_HAND_SHAKE_TYPE == M4_BASED)
				
#ifdef RSI_M4_INTERFACE
       selected_ps_mode = RSI_M4_BASED_DEEP_SLEEP;				
#endif				
#else
        selected_ps_mode = RSI_MSG_BASED_DEEP_SLEEP;
#endif
      }
      break;
    case 0x11:
    case 0x81:
    case 0x18:
    case 0x12:
    case 0x21:
      {
        //! Update power save profile mode
        selected_ps_mode = RSI_CONNECTED_SLEEP_PS;

        //! Update power save profile type
        selected_ps_type = rsi_common_cb->power_save.wlan_psp_type;
      }
      break;
    case 0x22:
    case 0x82:
    case 0x28:
      {
        //! Update power save profile mode
#if (RSI_HAND_SHAKE_TYPE == GPIO_BASED)
        selected_ps_mode = RSI_CONNECTED_GPIO_BASED_PS;

#elif (RSI_HAND_SHAKE_TYPE == M4_BASED)
#ifdef RSI_M4_INTERFACE						
    		selected_ps_mode = RSI_CONNECTED_M4_BASED_PS;
#endif				
#else
        selected_ps_mode = RSI_CONNECTED_MSG_BASED_PS;
#endif
        //! Update power save profile type
        selected_ps_type = rsi_common_cb->power_save.wlan_psp_type;
      }
      break;
    default:
      {
        //! Do nothing
      }
      break;
  }


  if(rsi_common_cb->power_save.current_ps_mode != selected_ps_mode)
  {
    status = rsi_send_ps_mode_to_module(selected_ps_mode, selected_ps_type);
  }

  return status;
}


/*====================================================*/
/**
 * @fn          int32_t rsi_send_ps_mode_to_module(uint8_t selected_ps_mode, uint8_t selected_ps_type)
 * @brief       This function sends the power save command 
 *              to module  
 * @param[in]   selected_ps_mode  
 *              selected_ps_type
 * @return       0 = not to program
 *               1 = program non-connected sleep
 *               2 = program connected sleep
 * @section description 
 * This function will send the power mode command to module.
 */
int32_t rsi_send_ps_mode_to_module(uint8_t selected_ps_mode, uint8_t selected_ps_type)
{
  rsi_pkt_t       *pkt;
  rsi_power_save_req_t  *ps_req;
  uint8_t status = 0;

  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;


  //! take lock on common control block
  rsi_mutex_lock(&rsi_common_cb->common_mutex);

  //! allocate command buffer  from common pool
  pkt = rsi_pkt_alloc(&rsi_common_cb->common_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    //! unlock mutex
    rsi_mutex_unlock(&rsi_common_cb->common_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  //! Power mode setting
  ps_req = (rsi_power_save_req_t *)pkt->data;

  //! reset to 0
  memset(&pkt->data,0,sizeof(rsi_power_save_req_t));

  //!take backup of powe save mode
  rsi_common_cb->power_save.current_ps_mode = 0;

  //! send power save command
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PWRMODE, pkt);

  //! unmask Tx event
  rsi_unmask_event(RSI_TX_EVENT);

  if(status != 0)
  {
    //! free the packet
    rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

    //! unlock mutex
    rsi_mutex_unlock(&rsi_common_cb->common_mutex);

    //! return status
    return status;

  }

  rsi_common_cb->power_save.power_save_enable = 0;

  if(selected_ps_mode == 0)
  {
    //! free the packet
    rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

    //! unlock mutex
    rsi_mutex_unlock(&rsi_common_cb->common_mutex);

    //! return status
    return 0;

  }

  memset(&pkt->data,0,sizeof(rsi_power_save_req_t));

  //!take backup of powe save mode
  rsi_common_cb->power_save.current_ps_mode = selected_ps_mode;

  //! Update power save profile mode
  ps_req->power_mode = selected_ps_mode;

  //! Update power save profile type
  ps_req->psp_type   = selected_ps_type;

  //! send power save command
  status = rsi_driver_common_send_cmd(RSI_COMMON_REQ_PWRMODE, pkt);

  if(status == 0)
  {
    //! Enable power save
    rsi_common_cb->power_save.power_save_enable = 1;
  }

  //! for powermode 9 unmask will happen after WKP receive 
  if(selected_ps_mode != RSI_MSG_BASED_DEEP_SLEEP ) 
  {
    //! unmask Tx event
    rsi_unmask_event(RSI_TX_EVENT);
  }

  //! free the packet
  rsi_pkt_free(&rsi_common_cb->common_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&rsi_common_cb->common_mutex);

  return status;
}

/*=============================================================================*/
/**
 * @fn              void RSI_PS_Restore(void)
 * @brief           Dummy for non rom 
 * @param[in]       void
 * @param[in]       void
 * @param[out]      none
 * @return          none
 */
#if 0 
#ifndef ROM_WIRELESS
void RSI_PS_Restore(void)
{
	while(1);
}
#endif
#endif
