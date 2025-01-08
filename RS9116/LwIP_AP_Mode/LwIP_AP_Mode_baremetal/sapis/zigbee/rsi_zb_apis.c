/**
 * @file     rsi_intf.c
 * @version  0.1
 * @date     2015-Aug-31
 *
 * Copyright(C) Redpine Signals 2012
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief 
 * Definitions of all ZigBee Frame Descriptors
 *
 * @section Description
 * This file contains definition of all frame descriptors used in ZigBee.
 * These definition are used to construct frames. 
 *
 *
 */

/**
 * Includes
 */
#ifdef RSI_ZB_ENABLE
#include "rsi_driver.h"
#include "rsi_zb_frames.h"
#include "rsi_zb_api.h"

int32_t rsi_zigb_send_cmd(uint8_t *cmd, rsi_pkt_t *pkt);
/*===========================================================================
 *
 * @fn          rsi_zigb_send_pwrmode (uint8_t ps_en,uint8_t deep_sleep_wakeup_period ,uint8_t slp_mode) 
 * @brief       Sends the ZigBee power mode command to ZigBee Module
 * @param[in]   ps_en: 0->Disable,1->Enable
 *              deep_sleep_wakeup_period: deep sleep wkp period in sec.
 *              slp_mode: 1->LP_mode,2-> ULP_mode 
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to send power mode cmd to the ZigBee Fw. 
 *
 */
 int16_t rsi_zigb_send_pwrmode (uint8_t ps_en,uint8_t deep_sleep_wakeup_period ,uint8_t slp_mode)
{

    rsi_pkt_t *pkt;
    pwrModeFrameSnd *pwr_mode = NULL;


  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  pwr_mode = (pwrModeFrameSnd*)pkt->data;
  pwr_mode->ps_en = ps_en;

  pwr_mode->deep_sleep_wkp_period = deep_sleep_wakeup_period;
  
  pwr_mode->slp_mode = slp_mode;
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framePwrMode, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}
/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_init_stack(void)
 * @brief       Sends the ZigBee Stack init command to the ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to init the ZigBee stack. 
 *
 */
int16_t rsi_zigb_init_stack(void)
{
  rsi_pkt_t *pkt;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameInitStack, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_reset_stack(void)
 * @brief       Sends the ZigBee Stack reset command to the ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to soft reset the ZigBee stack. 
 *
 */
int16_t rsi_zigb_reset_stack(void)
{
  rsi_pkt_t *pkt;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameResetStack, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_form_network(uint8_t RadioChannel, 
 * 				uint8_t tx_power, uint8_t *pExtendedPanId)
 * @brief       Sends the ZigBee Form Network command to ZigBee Module
 * @param[in]   RadioChannel - Channel in which network is to be formed
 *              tx_power - 
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to soft reset the ZigBee stack. 
 *
 */
int16_t rsi_zigb_form_network (uint8_t RadioChannel, uint8_t power,
    uint8_t * pExtendedPanId )
{

  rsi_pkt_t *pkt;
  formNetworkFrameSnd *form_nwk;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  form_nwk = (formNetworkFrameSnd*)pkt->data;
  form_nwk->RadioChannel = RadioChannel;
  form_nwk->power = power;

  rsi_zigb_copy_xt_panid(pExtendedPanId, (uint8_t *)&form_nwk->ExtPanId);

  //! send the command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameFormNWK, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_join_network(uint8_t DeviceType, 
 *              uint8_t RadioChannel,uint8_t power ,uint8_t * pExtendedPanId)
 * @brief       Sends the ZigBee Join Network command to ZigBee Module
 * @param[in]   DeviceType   - Type of device
 *                  - 0: Coordinator
 *                  - 1: Router
 *                  - 2: End Device
 *              RadioChannel   - Channel in which network exists.
 *              tx_power       - tx power
 *              pExtendedPanId - Extended PAN addr of the network.
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to soft reset the ZigBee stack. 
 *
 */
int16_t rsi_zigb_join_network(uint8_t DeviceType, uint8_t RadioChannel,uint8_t power ,uint8_t * pExtendedPanId)
{

  rsi_pkt_t *pkt;
  joinNetworkFrameSnd *join_nwk;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  join_nwk = (joinNetworkFrameSnd*)pkt->data;
  join_nwk->DeviceType = DeviceType;
  join_nwk->RadioChannel = RadioChannel;
  join_nwk->power = power;	

  rsi_zigb_copy_xt_panid(pExtendedPanId, (uint8_t *)&join_nwk->ExtPanId);
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameJoinNWK, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_permit_join(uint8_t PermitDuration)
 * @brief       Sends the permit join command to ZigBee module
 * @param[in]   PermitDuration - Duration to accept associations
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to trigger permit join for the specific amount of time.
 *
 */
int16_t rsi_zigb_permit_join(uint8_t PermitDuration)
{
  rsi_pkt_t *pkt;
  permitJoinFrameSnd *permit_join;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  permit_join = (permitJoinFrameSnd*)pkt->data;
  permit_join->PermitDuration = PermitDuration;
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framePermitJoin, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_leave_network(void)
 * @brief       Sends the leave network command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to leave the network and connect to another device
 *
 */
int16_t rsi_zigb_leave_network(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameLeaveNWK, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}
/*==============================================*/
/**
 * @fn          int32_t rsi_zb_power_save_profile(uint8_t psp_mode, uint8_t psp_type)
 * @brief       Sets power save profile
 * @param[in]   psp_mode.The options are : 
 *              0 - Active
 *              1 - Sleep without SoC turn off when connected else disconnected deep sleep
 *              2 - Sleep with SoC turn off (connected/disconnected)
 *              8 - Deep sleep
 * @param[in]   psp_type. The options are :
 *              0 - Max power save
 *              1 - Fast PSP
 *              2 - UAPSD 
 * @param[out]  none
 * @return      command response status
 *
 * @section description
 * This function sets the different power save profile
 *
 */
int32_t rsi_zb_power_save_profile(uint8_t psp_mode, uint8_t psp_type)
{
  int32_t status = RSI_SUCCESS;
  
  //!Get commmon cb pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //!Updating present zb power save type
  rsi_common_cb->power_save.zb_psp_type = psp_type;

  //!Updating present zb power save mode
  rsi_common_cb->power_save.zb_psp_mode = psp_mode;
  
  //status = rsi_sleep_mode_decision(rsi_common_cb);

  return status; 
}



/*===========================================================================
 *
 * @fn          rsi_zigb_initiate_scan(uint8_t scanType, 
 *                                     uint32_t channelMask, 
 *                                     uint8_t duration)
 * @brief       Sends the scan command to ZigBee module 
 * @param[in]   scanning Type
 * @param[in]   channel Mask
 * @param[in]   duration to scan in  each channel
 * @param[out]  status
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used initiate scan process
 *
 *===========================================================================
 */
int16_t rsi_zigb_initiate_scan(uint8_t ScanType, uint32_t ChannelMask, uint8_t duration)
{

  rsi_pkt_t *pkt;
  initScanFrameSnd *init_scan = NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  init_scan = (initScanFrameSnd*)pkt->data;
  init_scan->ScanType = ScanType;

  init_scan->ChannelMask = ChannelMask;
  
  init_scan->duration = duration;
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameInitScan, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_stop_scan(void)
 * @brief       Sends the stop scan command to ZigBee module 
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to stop the ongoing scan procedure
 *
 */
int16_t rsi_zigb_stop_scan(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameStopScan, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_network_state(void)
 * @brief       Sends the network state command to ZigBee
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to read the cuurent network state
 *
 */
int16_t rsi_zigb_network_state(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameNWKState, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_stack_is_up(void)
 * @brief       Sends the stack is up command to ZigBee
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to read the stack status to know the stack state
 *
 */
int16_t rsi_zigb_stack_is_up(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameStackIsUp, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_self_ieee_address(uint8_t* ieee_addr)
 * @brief       Sends the get self 64-bit extended address from ZigBee module
 * @param[in]   none
 * @param[out]  uint8_t* ieee_addr,ieee_address
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to read back the self IEEE address of the ZigBee device
 *
 */
int16_t rsi_zigb_get_self_ieee_address(uint8_t* ieee_addr)
{
  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetSelfIEEE, pkt);


  rsi_zigb_mcpy(zigb_cb->resp.uCmdRspPayLoad.IEEEAddrResp.Addr,ieee_addr, 8);
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 


}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_device_type(uint8_t *dev_type)
 * @brief       Sends the get device type command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the current decice type.
 *
 */
int16_t rsi_zigb_get_device_type(uint8_t *dev_type)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetDevType, pkt);
  *dev_type = zigb_cb->resp.uCmdRspPayLoad.GetDevResp.type;
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return zigb_cb->resp.uCmdRspPayLoad.GetDevResp.status;

}
/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_rssi(void)
 * @brief       Gets the rssi value 
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the rssi value.
 *
 */
int16_t rsi_zigb_get_rssi(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
   
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetRssi, pkt);
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return zigb_cb->resp.uCmdRspPayLoad.zb_rssi_value.rssi_value;

}
/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_inter_frag_spacing(uint8_t Fragmentspacing)
 * @brief       Sends the inter fragment time  to ZigBee module
 * @param[in]   Fragmentspacing - Time between two fragments
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to set the time for interfragment spacing.
 *
 */
int16_t rsi_zigb_inter_frag_spacing(uint8_t Fragmentspacing)
{
  rsi_pkt_t *pkt;
  interfragmentspacing *fragment_space;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  fragment_space = (interfragmentspacing*)pkt->data;
  fragment_space->Fragmentspacing = Fragmentspacing;
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameinterfragmentspace, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}
/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_update_all_sas(Startup_Attribute_Set_t *Startup)
 * @brief       Sends the update SAS configuration to module
 * @param[in]   Startup - SAS configuration structure
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to update the SAS parameters avialble in ZigBee Stack.
 *
 */
int16_t rsi_zigb_update_all_sas(Startup_Attribute_Set_t *Startup)
{
  rsi_pkt_t *pkt;
  Startup_Attribute_Set_t *Startup_temp=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);
  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  Startup_temp = (Startup_Attribute_Set_t*)pkt->data;
  //  rsi_zigb_mcpy((uint8_t*)&Startup,(uint8_t*)&Startup_temp,sizeof(Startup_Attribute_Set_t));
  memcpy((uint8_t*)&Startup_temp->a_extended_pan_id,(uint8_t*)&Startup->a_extended_pan_id, 8);
  Startup_temp->channel_mask      = Startup->channel_mask;
  Startup_temp->protocol_version  = Startup->protocol_version;
  Startup_temp->stack_profile     = Startup->stack_profile;
  Startup_temp->startup_control   = Startup->startup_control;
  memcpy((uint8_t*)&Startup_temp->a_trust_center_address,(uint8_t*)&Startup->a_trust_center_address,8);
  memcpy((uint8_t*)&Startup_temp->a_network_key,(uint8_t*)&Startup->a_network_key,16);
  Startup_temp->use_insecure_join = Startup->use_insecure_join;
  Startup_temp->network_key_seq_num = Startup->network_key_seq_num;
  Startup_temp->network_key_type    = Startup->network_key_type;
  Startup_temp->network_manager_address =  Startup->network_manager_address;
  Startup_temp->scan_attempts = Startup->scan_attempts;
  Startup_temp->time_between_scans    = Startup->time_between_scans;
  Startup_temp->rejoin_interval       = Startup->rejoin_interval;
  Startup_temp->max_rejoin_interval   = Startup->max_rejoin_interval;
  Startup_temp->indirect_poll_rate    = Startup->indirect_poll_rate;
  Startup_temp->parent_retry_threshold =  Startup->parent_retry_threshold;
  Startup_temp->security_timeout_period=  Startup->security_timeout_period;
  Startup_temp->APS_ack_wait_duration  =  Startup->APS_ack_wait_duration;
  Startup_temp->a_short_address        =  Startup->a_short_address;
  Startup_temp->a_pan_id              = Startup->a_pan_id;
  memcpy((uint8_t*)&Startup_temp->a_trustcenter_master_key,(uint8_t*)&Startup->a_trustcenter_master_key,16);
  memcpy((uint8_t*)&Startup_temp->a_preconfigured_link_key,(uint8_t*)&Startup->a_preconfigured_link_key,16);

  Startup_temp->concentrator_flag       = Startup->concentrator_flag;
  Startup_temp->concentrator_radius     = Startup->concentrator_radius;
  Startup_temp->concentrator_discovery_time = Startup->concentrator_discovery_time;
  Startup_temp->deviceAnnceReq          =     Startup->deviceAnnceReq;
  Startup_temp->end_device_bind_timeout =  Startup->end_device_bind_timeout;
  //! send command

  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameUpdateALLSAS, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}
/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_update_sas(Startup_Attribute_set_t *Startup)
 * @brief       Sends the update SAS configuration to module
 * @param[in]   Startup - SAS configuration structure
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to update the SAS parameters avialble in ZigBee Stack.
 *
 */
int16_t rsi_zigb_update_sas(Startup_Attribute_set_t *Startup)
{
  rsi_pkt_t *pkt;
  Startup_Attribute_set_t *Startup_temp=NULL;
   
  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);
  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  Startup_temp = (Startup_Attribute_set_t*)pkt->data;
//  rsi_zigb_mcpy((uint8_t*)&Startup,(uint8_t*)&Startup_temp,sizeof(Startup_Attribute_Set_t));
  memcpy((uint8_t*)&Startup_temp->a_extended_pan_id,(uint8_t*)&Startup->a_extended_pan_id, 8);
    Startup_temp->channel_mask      = Startup->channel_mask;
    Startup_temp->startup_control   = Startup->startup_control;
    Startup_temp->use_insecure_join = Startup->use_insecure_join;
    Startup_temp->scan_attempts = Startup->scan_attempts;
    Startup_temp->parent_retry_threshold =  Startup->parent_retry_threshold;
    memcpy((uint8_t*)&Startup_temp->a_trust_center_address,(uint8_t*)&Startup->a_trust_center_address,8);
    memcpy((uint8_t*)&Startup_temp->a_network_key,(uint8_t*)&Startup->a_network_key,16);
    Startup_temp->time_between_scans    = Startup->time_between_scans;
    Startup_temp->rejoin_interval       = Startup->rejoin_interval;
    Startup_temp->max_rejoin_interval   = Startup->max_rejoin_interval;
    Startup_temp->indirect_poll_rate    = Startup->indirect_poll_rate;
    Startup_temp->a_pan_id              = Startup->a_pan_id;
    Startup_temp->network_manager_address =  Startup->network_manager_address;
    memcpy((uint8_t*)&Startup_temp->a_trustcenter_master_key,(uint8_t*)&Startup->a_trustcenter_master_key,16);
    memcpy((uint8_t*)&Startup_temp->a_preconfigured_link_key,(uint8_t*)&Startup->a_preconfigured_link_key,16);
    Startup_temp->end_device_bind_timeout =  Startup->end_device_bind_timeout;
   //! send command
  
  
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameUpdateSAS, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_update_all_zdo_configuration(ZDO_Configuration_Table_t *zdo_cnf)
 * @brief       Sends the update ZDO configuration to module
 * @param[in]   zdo_cnf - ZDO configuration structure
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to update the ZDO parameters avialble in ZigBee Stack.
 *
 * This API is called only after updating SAS parameters.
 *
 */
int16_t rsi_zigb_update_all_zdo_configuration(ZDO_Configuration_Table_t *zdo_cnf)
{
  rsi_pkt_t *pkt;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;
  ZDO_Configuration_Table_t *zdo_cnf_temp = NULL;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  zdo_cnf_temp = (ZDO_Configuration_Table_t*)pkt->data;
  //  rsi_zigb_mcpy((uint8_t*)&zdo_cnf,(uint8_t*)&zdo_cnf_temp,sizeof(ZDO_Configuration_Table_t));
  zdo_cnf_temp->config_permit_join_duration = zdo_cnf->config_permit_join_duration;
  zdo_cnf_temp->config_NWK_secure_all_frames= zdo_cnf->config_NWK_secure_all_frames;
  zdo_cnf_temp->config_NWK_alt_protocol_version= zdo_cnf->config_NWK_alt_protocol_version;
  zdo_cnf_temp->config_formation_attempts= zdo_cnf_temp->config_formation_attempts;
  zdo_cnf_temp->config_scan_duration= zdo_cnf->config_scan_duration;
  zdo_cnf_temp->config_join_attempts= zdo_cnf->config_join_attempts;
  zdo_cnf_temp->config_preconfigured_key= zdo_cnf->config_preconfigured_key;
  zdo_cnf_temp->config_no_of_devices_joined_before_NVM_save= zdo_cnf->config_no_of_devices_joined_before_NVM_save;
  zdo_cnf_temp->config_no_of_data_txns_before_NVM_save= zdo_cnf->config_no_of_data_txns_before_NVM_save;
  zdo_cnf_temp->a_config_trust_center_short_address= zdo_cnf->a_config_trust_center_short_address;
  zdo_cnf_temp->automatic_poll_allowed= zdo_cnf->automatic_poll_allowed;
  zdo_cnf_temp->config_authentication_poll_rate= zdo_cnf->config_authentication_poll_rate;
  zdo_cnf_temp->config_switch_key_time= zdo_cnf->config_switch_key_time;
  zdo_cnf_temp->config_security_level= zdo_cnf->config_security_level;
  zdo_cnf_temp->config_aps_ack_poll_time_out= zdo_cnf->config_aps_ack_poll_time_out;

  /* Node Descriptor */
  zdo_cnf_temp->node_desc_info.aps_flags_freq_band = zdo_cnf->node_desc_info.aps_flags_freq_band;
  zdo_cnf_temp->node_desc_info.MAC_capability_flags = zdo_cnf->node_desc_info.MAC_capability_flags;
  /* Manufacturing ID */
 memcpy(zdo_cnf_temp->node_desc_info.a_manufacturer_code,zdo_cnf->node_desc_info.a_manufacturer_code, 2);
  zdo_cnf_temp->node_desc_info.max_buffer_size = zdo_cnf->node_desc_info.max_buffer_size;
  memcpy(zdo_cnf_temp->node_desc_info.a_max_incoming_transfer_size, zdo_cnf->node_desc_info.a_max_incoming_transfer_size, 2);
  memcpy(zdo_cnf_temp->node_desc_info.a_max_outgoing_transfer_size, zdo_cnf->node_desc_info.a_max_outgoing_transfer_size, 2);

  zdo_cnf_temp->node_desc_info.descriptor_capability = zdo_cnf->node_desc_info.descriptor_capability;
  zdo_cnf_temp->current_powermode_avail_power_sources = zdo_cnf->current_powermode_avail_power_sources;
  zdo_cnf_temp->current_powersource_currentpowersourcelevel = zdo_cnf->current_powersource_currentpowersourcelevel;

  //! send command
  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameUpdateALLZDO, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_update_zdo_configuration(ZDO_Configuration_Table_t *zdo_cnf)
 * @brief       Sends the update ZDO configuration to module
 * @param[in]   zdo_cnf - ZDO configuration structure
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to update the ZDO parameters avialble in ZigBee Stack.
 *
 * This API is called only after updating SAS parameters.
 *
 */
int16_t rsi_zigb_update_zdo_configuration(ZDO_Configuration_Table_t *zdo_cnf)
{
  rsi_pkt_t *pkt;
   
  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;
  ZDO_Configuration_Table_t *zdo_cnf_temp = NULL;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
 
  zdo_cnf_temp = (ZDO_Configuration_Table_t*)pkt->data;
//  rsi_zigb_mcpy((uint8_t*)&zdo_cnf,(uint8_t*)&zdo_cnf_temp,sizeof(ZDO_Configuration_Table_t));
   zdo_cnf_temp->config_permit_join_duration = zdo_cnf->config_permit_join_duration;
   zdo_cnf_temp->config_NWK_secure_all_frames= zdo_cnf->config_NWK_secure_all_frames;
   zdo_cnf_temp->config_formation_attempts= zdo_cnf_temp->config_formation_attempts;
   zdo_cnf_temp->config_scan_duration= zdo_cnf->config_scan_duration;
   zdo_cnf_temp->config_join_attempts= zdo_cnf->config_join_attempts;
   zdo_cnf_temp->config_preconfigured_key= zdo_cnf->config_preconfigured_key;
   zdo_cnf_temp->a_config_trust_center_short_address= zdo_cnf->a_config_trust_center_short_address;
   zdo_cnf_temp->automatic_poll_allowed= zdo_cnf->automatic_poll_allowed;
   zdo_cnf_temp->config_authentication_poll_rate= zdo_cnf->config_authentication_poll_rate;
   zdo_cnf_temp->config_switch_key_time= zdo_cnf->config_switch_key_time;
   zdo_cnf_temp->config_security_level= zdo_cnf->config_security_level;
   zdo_cnf_temp->config_aps_ack_poll_time_out= zdo_cnf->config_aps_ack_poll_time_out;
    //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameUpdateZDO, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}



/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_simple_descriptor(uint8_t endpointId, 
 * 						Simple_Descriptor_t *SimpleDesc)
 *
 * @brief       Sends the set simple descriptor command to ZigBee
 *
 * @param[in]   uint8_t endpointId, endpoint to which simple descriptor need to store
 * 				Simple_Descriptor_t *SimpleDesc, pointer of Simple descriptor to store
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to set the simple descriptor for the given endpoint.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_set_simple_descriptor(uint8_t EndPointId, Simple_Descriptor_t *SimpleDesc)
{

  rsi_pkt_t *pkt;
  uint8_t in_cluster_offset = 0;
   
  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;
  setSimpleDescFrameSnd *set_simple_desc=NULL; 

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
 
  set_simple_desc = (setSimpleDescFrameSnd*)pkt->data;
  
  set_simple_desc->EndPointId = EndPointId;

  rsi_zigb_uint16to_buffer(SimpleDesc->app_profile_id, (uint8_t *)&set_simple_desc->ProfileId);

  rsi_zigb_uint16to_buffer(SimpleDesc->app_device_id, (uint8_t *)&set_simple_desc->DevId);

  set_simple_desc->DevVersion = SimpleDesc->app_device_version;

  set_simple_desc->InClusterCnt = SimpleDesc->incluster_count;
  
  set_simple_desc->OutClusterCnt = SimpleDesc->outcluster_count;
  rsi_zigb_mcpy((uint8_t *)SimpleDesc->p_incluster_list,  (uint8_t *)&set_simple_desc->ClusterInfo, (SimpleDesc->incluster_count * sizeof(cluster_id_t)));
  
  in_cluster_offset = (SimpleDesc->incluster_count * sizeof(cluster_id_t));
  rsi_zigb_mcpy((uint8_t *)SimpleDesc->p_outcluster_list, (uint8_t *)&set_simple_desc->ClusterInfo[in_cluster_offset], (SimpleDesc->outcluster_count * sizeof(cluster_id_t)));

  
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetSimpleDesc, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_end_device_poll_for_data(void)
 * @brief       Sends the data request initiate command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to poll the parent for the data. 
 *
 */
int16_t rsi_zigb_end_device_poll_for_data( void )
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framePollForData, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);
  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_send_match_descriptors_request(uint16_t shortAddress,
 *              uint16_t ProfileId, uint8_t *InClusterList, uint8_t InClusterCnt,
 *              uint8_t *OutClusterList, uint8_t OutClusterCnt, BOOL APSAckRequired,
 *              uint16_t dstAddress)
 * @brief       Sends the Match Descriptor Request command to ZigBee module
 *
 * @param[in]   
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send the Match Descriptor Request.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_send_match_descriptors_request(uint16_t shortAddress, uint16_t ProfileId, uint8_t *InClusterList, uint8_t InClusterCnt,
    uint8_t *OutClusterList, uint8_t OutClusterCnt, BOOL APSAckRequired,uint16_t dstAddress)
{

  rsi_pkt_t *pkt;
  uint8_t in_cluster_offset= 0;
  sendMatchDescFrameSnd *match_desc;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  match_desc = (sendMatchDescFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddress, (uint8_t *)&match_desc->ShortAddr);

  rsi_zigb_uint16to_buffer(ProfileId, (uint8_t *)&match_desc->ProfileId);

  match_desc->InClusterCnt = InClusterCnt;

  match_desc->OutClusterCnt = OutClusterCnt;
  match_desc->APSAckRequired = APSAckRequired;

  rsi_zigb_uint16to_buffer(dstAddress, (uint8_t *)&match_desc->DestAddress);
  
  rsi_zigb_mcpy(InClusterList,  (uint8_t *)&match_desc->ClusterInfo, (InClusterCnt * sizeof(cluster_id_t)));
  in_cluster_offset = InClusterCnt * sizeof(cluster_id_t);
  rsi_zigb_mcpy(OutClusterList, (uint8_t *)&match_desc->ClusterInfo[in_cluster_offset], (OutClusterCnt * sizeof(cluster_id_t)));


   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSendMatchDesc, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_send_unicast_data(ZigBee_Outgoing_Msg_Type msgType,
 * 						Address DestAddress, ZigBeeAPSDEDataRequest_t *pAPSDERequest)
 *
 * @brief       Sends the unicast data command to ZigBee module
 *
 * @param[in]   ZigBee_Outgoing_Msg_Type msgType, outgoing message type.
 * 				Address DestAddress, union of 64-bit extended address and 16-bit 
 * 				short address
 * 				ZigBeeAPSDEDataRequest_t *pAPSDERequest, pointer to APSDE_Data_Request_t
 * 				structure
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send a simple unicast data for the specified short address. 
 * 
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_send_unicast_data(ZigBee_Outgoing_Msg_Type msgType, Address DestAddress, ZigBeeAPSDEDataRequest_t *pAPSDERequest)
{
  rsi_pkt_t *pkt;
  uint8_t i=0;
  unicastDataFrameSnd *ucast_buf;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  ucast_buf = (unicastDataFrameSnd*)pkt->data;
  ucast_buf->msgType  = msgType;

  rsi_zigb_mcpy(DestAddress.IEEE_address, (uint8_t *)&ucast_buf->ieee_Addr, 8);

  ucast_buf->DestEndpoint = pAPSDERequest->DestEndpoint;
  ucast_buf->SrcEndpoint = pAPSDERequest->SrcEndpoint;

  rsi_zigb_uint16to_buffer(pAPSDERequest->ProfileId, (uint8_t *)&ucast_buf->ProfileId);

  rsi_zigb_uint16to_buffer(pAPSDERequest->ClusterId, (uint8_t *)&ucast_buf->ClusterId);

  ucast_buf->AsduLength = pAPSDERequest->AsduLength;
  ucast_buf->TxOptions = pAPSDERequest->TxOptions;
  ucast_buf->Radius = pAPSDERequest->Radius;

  for(i = 0; i < pAPSDERequest->AsduLength; i++)
    ucast_buf->Data[i] = pAPSDERequest->aPayload[i];

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameUcastData, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_is_it_self_ieee_address(uint8_t *pIEEEAddress)
 * @brief       Sends the self IEEE Address check command to ZigBee module
 * @param[in]   none
 * @param[out]  uint8_t *pIEEEAddress,IEEEAddress
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to check whether the given address is self IEEE address or not
 *
 */
BOOL rsi_zigb_is_it_self_ieee_address(uint8_t *pIEEEAddress)
{
  rsi_pkt_t *pkt;
  isitSelfIEEEFrameSnd *self_ieee= NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  self_ieee = (isitSelfIEEEFrameSnd*)pkt->data;
  rsi_zigb_copy_xt_panid(pIEEEAddress, (uint8_t *)&self_ieee->ieee_Addr);
  
  //Sending the command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameIsItSelfIEEE, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_self_short_address(void)
 * @brief       Sends the self short Address command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the self short address of the device
 *
 */
int16_t rsi_zigb_get_self_short_address(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetSelfShortAddr, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevShortAddr.short_addr; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_manufacturer_code_for_node_desc(uint16_t
 * 						ManufacturerCode)
 * @brief       Sends the set manufcaturing code command to ZigBee module
 * @param[in]   ManufacturerCode - 16-bit manufcaturer code
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to set the manufacturer  code for node descriptor
 *
 */
int16_t rsi_zigb_set_manufacturer_code_for_node_desc(uint16_t ManufacturerCode)
{
  rsi_pkt_t *pkt;
   
  setManufFrameSnd *set_manuf=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  set_manuf = (setManufFrameSnd*)pkt->data;
  set_manuf->ManufacturerCode = ManufacturerCode;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetManufNodeDesc, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_power_descriptor(Node_Power_Descriptor_t
 * 								*nodePowerDesc)
 * @brief       Sends the set power descriptor command to ZigBee module
 * @param[in]   nodePowerDesc - Node Power descriptor 
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to set the node power descriptor
 *
 */
int16_t rsi_zigb_set_power_descriptor(Node_Power_Descriptor_t *nodePowerDesc)
{
  rsi_pkt_t *pkt;
   
  setPowerDescFrameSnd *set_pwr_desc=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  set_pwr_desc = (setPowerDescFrameSnd*)pkt->data;

  set_pwr_desc->PowerSources = nodePowerDesc->current_powermode_avail_power_sources;
  set_pwr_desc->CurPowerLevel = nodePowerDesc->current_powersource_currentpowersourcelevel;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetPwrDesc, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_maxm_incoming_txfr_size(uint16_t 
 * 							MaxIncomingTxfrSize)
 * @brief       Sends the max incoming frame tx frame size command to ZigBee 
 * @param[in]   MaxIncomingTxfrSize - max possible Incoming tx frame size
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to set the maximum incoming tx frame size to ZigBee stack
 *
 */
int16_t rsi_zigb_set_maxm_incoming_txfr_size(uint16_t MaxIncomingTxfrSize)
{
  rsi_pkt_t *pkt;
   
  setMaxIncomingTxfrFrameSnd *max_in_txfr=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  max_in_txfr = (setMaxIncomingTxfrFrameSnd*)pkt->data;
  max_in_txfr->MaxIncomingTxfrSize = MaxIncomingTxfrSize;


   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetMaxIncmgSize, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_maxm_out_going_txfr_size(uint16_t 
 * 							MaxOutgoingTxfrSize)
 * @brief       Sends the max outgoing frame tx frame size command to ZigBee 
 * @param[in]   MaxOutgoingTxfrSize - max possible outgoing tx frame size
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to set the maximum outgoing tx frame size to ZigBee stack
 *
 */
int16_t rsi_zigb_set_maxm_out_going_txfr_size(uint16_t MaxOutgoingTxfrSize)
{
  rsi_pkt_t *pkt;
   
  setMaxOutTxfrFrameSnd *max_out_txfr=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  max_out_txfr = (setMaxOutTxfrFrameSnd*)pkt->data;
  max_out_txfr->MaxOutgoingTxfrSize = MaxOutgoingTxfrSize;


   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetMaxOutSize, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_operating_channel(uint8_t channel)
 * @brief       Sends the Set channel command to ZigBee 
 * @param[in]   channel - channel in which operations need to be carried out
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to chnage the operating channel in ZigBee stack
 *
 */
int16_t rsi_zigb_set_operating_channel(uint8_t Channel)
{
  rsi_pkt_t *pkt;
   
  setOperChanFrameSnd *set_chan=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  set_chan = (setOperChanFrameSnd*)pkt->data;
  set_chan->Channel = Channel;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetChan, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_operating_channel()
 * @brief       Sends the get cuurent operating channel command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the current operating channel commanf from ZigBee stack
 *
 */
int16_t rsi_zigb_get_operating_channel(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetOperChan, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevChannel.channel; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_short_panid()
 * @brief       Sends the get cuurent short panID  command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the current using PANID from ZigBee stack
 *
 */
int16_t rsi_zigb_get_short_panid(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetShortPanId, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevPanId.pan_id; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_extended_panid(uint8_t* extended_panid)
 * @brief       Sends the get extended panid  command to ZigBee module
 * @param[in]   none
 * @param[out]  uint8_t* extended_panid,extended_panid
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the current using extended PANID from ZigBee stack
 *
 */
int16_t rsi_zigb_get_extended_panid(uint8_t* extended_panid)
{
  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetExtPanId, pkt);

  rsi_zigb_mcpy(zigb_cb->resp.uCmdRspPayLoad.Extnd_PanId.Ext_Pan_Id,
      extended_panid, 8);
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_endpoint_id(uint8_t Index)
 * @brief       Sends the get command Endpoint ID from ZigBee stack
 * @param[in]   Index - Index from which the endpoint need to be read
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the endpoint ID from the ZigBee stack for the given Index
 *
 */
int16_t rsi_zigb_get_endpoint_id(uint8_t Index)
{
  rsi_pkt_t *pkt;
   
  getEndPointIdFrameSnd *get_ep=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_ep = (getEndPointIdFrameSnd*)pkt->data;
  get_ep->Index = Index;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetEP, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.EpClusterResp.cluster; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_simple_descriptor(uint8_t endpointId,Simple_Descriptor_t *simple_desc)
 * @brief       Sends the get Simple Descriptor command to ZigBee module
 * @param[in]   endpointId - Simple descriptor for the specific endpointId 
 * 				to be returned
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the simple descriptor for the sepcific endpoint
 * number provided.
 *
 */
int16_t rsi_zigb_get_simple_descriptor(uint8_t endpointId,Simple_Descriptor_t *simple_desc)
{
  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
   
  getSimpleDescFrameSnd *get_simple_desc=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_simple_desc = (getSimpleDescFrameSnd*)pkt->data;
  get_simple_desc->EndPointId = endpointId;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetSimpleDesc, pkt);

  rsi_zigb_mcpy((uint8_t*)&zigb_cb->resp.uCmdRspPayLoad.GetSimpleDescResp,(uint8_t*)
      &simple_desc,sizeof(Simple_Descriptor_t));
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return status;

}
/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_endpoint_cluster(uint8_t endpointId, 
 * 						uint8_t ClusterType, uint8_t ClusterIndex)
 * @brief       Sends the get endpoint cluster  command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the endpoint cluster 
 *
 */
int16_t rsi_zigb_get_endpoint_cluster(uint8_t EndPointId, uint8_t ClusterType,uint8_t ClusterIndex)
{

  rsi_pkt_t *pkt;
  getEPClusterFrameSnd *get_ep_cluster=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  get_ep_cluster = (getEPClusterFrameSnd*)pkt->data;
  get_ep_cluster->EndPointId = EndPointId;
  get_ep_cluster->ClusterType = ClusterType;
  get_ep_cluster->ClusterIndex = ClusterIndex;

  //! send the command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetEPCluster, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.EpClusterResp.cluster; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_short_addr_for_specified_ieee_addr(uint8_t
 * 						*pIEEEAddress)
 * @brief       Sends the get cuurent short panID  command to ZigBee module
 * @param[in]   pIEEEAddress - 64-bit extended address
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the short address for specific 64-bit IEEE address
 * from ZigBee stack
 *
 */
int16_t rsi_zigb_get_short_addr_for_specified_ieee_addr(uint8_t * pIEEEAddress)
{
  rsi_pkt_t *pkt;
  getShortAddrForIeeeAddrFrameSnd *get_short_for_ieee=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  get_short_for_ieee = (getShortAddrForIeeeAddrFrameSnd*)pkt->data;
  rsi_zigb_copy_xt_panid(pIEEEAddress, (uint8_t *)&get_short_for_ieee->ieee_Addr);
  
  //Sending the command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetShortAddrForIEEE, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevShortAddr.short_addr; 


}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_ieee_addr_for_specified_short_addr(uint16_t shortAddr,uint8_t* ieee_addr)
 * @brief       Sends the get 64-bit extended address using short address command to ZigBee module
 * @param[in]   uint16_t ShortAddr, short address to get 64-bit extended address
 * @param[in]   uint8_t* ieee_addr,64-bit extended address
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the 64-bit extended address for the given short address.
 * 
 */
int16_t rsi_zigb_get_ieee_addr_for_specified_short_addr(uint16_t shortAddr,uint8_t* ieee_addr)
{
  rsi_pkt_t *pkt;
   
  getIeeeAddrForShortAddrFrameSnd *get_ieee_for_short=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  get_ieee_for_short = (getIeeeAddrForShortAddrFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddr, (uint8_t *)&get_ieee_for_short->ShortAddr);

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetSelfIEEE, pkt);


  rsi_zigb_mcpy(zigb_cb->resp.uCmdRspPayLoad.GetIEEEResp.Ieee_Add,
      ieee_addr, 8);
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return zigb_cb->resp.uCmdRspPayLoad.GetIEEEResp.status;

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_read_neighbor_table_entry(uint8_t Index,ZigBeeNeighborTableEntry_t *neigbor_table)
 * @brief       Sends the neighbor table entry read command to ZigBee module
 * @param[in]   uint16_t index, index of neighbor table
 * @param[in]   ZigBeeNeighborTableEntry_t *neigbor_table,neighbor table
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the neighbor table entry for the given index
 * 
 */
int16_t rsi_zigb_read_neighbor_table_entry(uint8_t Index,ZigBeeNeighborTableEntry_t *neigbor_table)
{
  rsi_pkt_t *pkt;
   
  readNeighborTableFrameSnd *read_neighbor_table=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  read_neighbor_table = (readNeighborTableFrameSnd*)pkt->data;
  read_neighbor_table->Index = Index;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameReadNeighborTable, pkt);

  rsi_zigb_mcpy((uint8_t*)&zigb_cb->resp.uCmdRspPayLoad.ReadNeighborTableResp.neighbour_table,
      (uint8_t*)&neigbor_table,sizeof(ZigBeeNeighborTableEntry_t));
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.ReadNeighborTableResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_route_table_entry(uint8_t Index,ZigBeeRoutingTableEntry_t *routing_table)
 * @brief       Sends the route table entry read command to ZigBee module
 * @param[in]   uint16_t index, index of route table
 * @param[in]   ZigBeeRoutingTableEntry_t *routing_table: Routing table
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the route table entry for the given index
 * 
 */
int16_t rsi_zigb_get_route_table_entry(uint8_t Index,ZigBeeRoutingTableEntry_t *routing_table)
{
  rsi_pkt_t *pkt;
   
  getRouteTableFrameSnd *get_route_table=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_route_table = (getRouteTableFrameSnd*)pkt->data;
  get_route_table->Index = Index;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetRouteTable, pkt);

  rsi_zigb_mcpy((uint8_t*)&zigb_cb->resp.uCmdRspPayLoad.ReadRouteTableResp.routing_table,(uint8_t*)&
      routing_table,sizeof(ZigBeeNeighborTableEntry_t));
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.ReadRouteTableResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_neighbor_table_entry_count(void)
 * @brief       Sends the neighbor table entries count read command to ZigBee Module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to know the count of active neighbor table entries.
 *
 */
int16_t rsi_zigb_get_neighbor_table_entry_count(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetNeighborTable, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_child_short_address_for_the_index(uint8_t index)
 * @brief       Sends the child short address read command to ZigBee module
 * @param[in]   uint8_t index, index of the child for which the short address to be read
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to read the 16-bit short address of the child in the specified index.
 *
 */
int16_t rsi_zigb_get_child_short_address_for_the_index(uint8_t ChildIndex)
{
  rsi_pkt_t *pkt;
   
  getChildShortAddrFrameSnd *get_child_short_addr=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_child_short_addr = (getChildShortAddrFrameSnd*)pkt->data;
  get_child_short_addr->ChildIndex = ChildIndex;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetChildShortAddr, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevShortAddr.short_addr; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_child_index_for_specified_short_addr(uint16_t childShortAddr)
 * @brief       Sends the child short address index read command to ZigBee module
 * @param[in]   uint16_t childShortAddr, short address of the device for which the index to be read
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the index for the specified 16-bit child address 
 *
 */
int16_t rsi_zigb_get_child_index_for_specified_short_addr(uint16_t childShortAddr)
{
  rsi_pkt_t *pkt;
   
  getChildIndexForShortAddrFrameSnd *get_child_index=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_child_index = (getChildIndexForShortAddrFrameSnd*)pkt->data;
  get_child_index->ShortAddr = childShortAddr;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetChildIndexForShortAddr, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_child_details(uint8_t Index,uint8_t *ieee_addr,uint8_t DeviceType)
 * @brief       Sends the child short address index read command to ZigBee module
 * @param[in]   uint8_t index, index of the child of interest
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the child details for the specified child index.
 *
 */
int16_t rsi_zigb_get_child_details(uint8_t Index,uint8_t *ieee_addr,uint8_t DeviceType)
{
  rsi_pkt_t *pkt;
   
  getChildDetailsFrameSnd *get_child_details=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_child_details = (getChildDetailsFrameSnd*)pkt->data;
  get_child_details->Index = Index;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetChildDetails, pkt);

  rsi_zigb_mcpy(zigb_cb->resp.uCmdRspPayLoad.GetChildDetails.Ieee_Addr,
      ieee_addr, 8);
  DeviceType = zigb_cb->resp.uCmdRspPayLoad.GetChildDetails.device_type;
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.GetChildDetails.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_read_count_of_child_devices(void)
 * @brief       Sends the get the child count command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to read the number of child devices on the node.
 *
 */
int16_t rsi_zigb_read_count_of_child_devices(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameReadChildCnt, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_read_count_of_child_devices(void)
 * @brief       Sends the get the child count command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to read the number of child devices on the node.
 *
 */
int16_t rsi_zigb_read_count_of_router_child_devices(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameReadRouterChildCnt, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_parent_short_address(void)
 * @brief       Sends the get parent short address command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to read the 16-bit short address of parent.
 *
 */
int16_t rsi_zigb_get_parent_short_address(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
#ifdef ZB_DEBUG
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
#endif
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetParentShortAddr, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevShortAddr.short_addr; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_parent_ieee_address(void)
 * @brief       Sends the get parent IEEE address command to ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to read the 64-bit extended address of parent.
 *
 */
int16_t rsi_zigb_get_parent_ieee_address(uint8_t *ieee_addr)
{
  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetParentIEEEAddr, pkt);


  rsi_zigb_mcpy(zigb_cb->resp.uCmdRspPayLoad.IEEEAddrResp.Addr,ieee_addr, 8);
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return status;

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_initiate_energy_scan_request(uint16_t DestAddr, uint32_t ScanChannels,
 * 							uint8_t ScanDuration, uint8_t ScanRetry
 * @brief       Sends the Energy detection scan initiate command to ZigBee module
 * @param[in]   uint16_t DestAddr,indicates The network address of the device to perform the scan.
 * 				uint32_t ScanChannels, indicates A mask of the channels to be scanned.
 * 				uint8_t ScanDuration, indicates How long to scan on each channel. Allowed values
 *   			are 0..5, with the scan times as specified by 802.15.4 (0 = 31ms, 1 = 46ms,
 *   			2 = 77 ms, 3 = 138ms, 4 = 261ms, 5 = 507ms).
 *   			ScanRetry, indicates The number of scans to be performed on each channel
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API allows to request energy scan be performed and its
 * results returned. This request may only be sent by the current network manager and
 * must be unicast, not broadcast.
 *
 */
int16_t rsi_zigb_initiate_energy_scan_request(uint16_t DestAddr,uint32_t ScanChannels, uint8_t ScanDuration, uint16_t ScanRetry)
{

  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  initEnergyScanFrameSnd *init_scan=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  init_scan = (initEnergyScanFrameSnd*)pkt->data;
  
  rsi_zigb_uint16to_buffer(DestAddr, (uint8_t *)&init_scan->DestAddr);

  rsi_zigb_uint32to_buffer(ScanChannels, (uint8_t *)&init_scan->ScanChannels);

  init_scan->ScanDuration = ScanDuration;

  rsi_zigb_uint16to_buffer(ScanRetry, (uint8_t *)&init_scan->ScanRetry);
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameInitEnergyScan, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return status;

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_broadcast_nwk_manager_request(uint16_t NWKManagerShortAddr,
 * 				uint32_t ActiveChannels)
 *
 * @brief       Sends the Broadcast request command to ZigBee module
 *
 * @param[in]   uint16_t NWKManagerShortAddr,indicates The network address of the Network Manager.
 * 				uint32_t ActiveChannels, indicates the new active channel masks.
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This function allows the Application broadcasts a request to set the identity
 * of the network manager and the active channel mask. The mask is used when
 * scanning for the network after missing a channel update.
 *
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_broadcast_nwk_manager_request(uint16_t NWKManagerShortAddr, uint32_t ActiveChannels)
{

  rsi_pkt_t *pkt;
  bcastNWKManagerFrameSnd *bcast_nwk=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  bcast_nwk = (bcastNWKManagerFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(NWKManagerShortAddr, (uint8_t *)&bcast_nwk->ShortAddr);

  rsi_zigb_uint32to_buffer(ActiveChannels, (uint8_t *)&bcast_nwk->ActiveChannels);

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameBcastNWKManagerReq, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_zdp_send_nwk_addr_request(uint8_t *pIEEEAddrOfInterest,
 * 						bool RequestType, uint8_t StartIndex)
 *
 * @brief       Sends the NWK Address request command to ZigBee module
 *
 * @param[in]   uint8_t *pIEEEAddrOfInterest, pointer to location of IEEE address whose 16-bit Network
 * 				address is to be determined.
 * 				BOOL RequestType, if TRUE indicates single device response if FALSE
 * 				indicates extended device response.
 * 				uint8_t StartIndex, start index of the child devices list.
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This function allows the Application to send ZDP network address request to
 * determine the 16-bit short address of the device whose IEEE address is known.
 *
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_zdp_send_nwk_addr_request(uint8_t * pIEEEAddrOfInterest, BOOL RequestType, uint8_t StartIndex)
{

  rsi_pkt_t *pkt;
  getZDPNWKShortAddrFrameSnd *get_zdp_addr=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_zdp_addr = (getZDPNWKShortAddrFrameSnd*)pkt->data;
  rsi_zigb_mcpy(pIEEEAddrOfInterest, (uint8_t *)&get_zdp_addr->ieee_Addr, 8);

  get_zdp_addr->RequestType = RequestType;
  get_zdp_addr->StartIndex = StartIndex;


  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetZDPNWKShortAddr, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_zdp_send_ieee_addr_request(uint16_t shortAddress,
 * 					BOOL RequestType, uint8_t StartIndex, BOOL APSAckRequired)
 * 					
 *
 * @brief       Sends the NWK Address request command to ZigBee module
 *
 * @param[in]   uint16_t shortAddress, shortAddress whose 64-bit extended address
 * 				is to be determined.
 * 				BOOL RequestType, if TRUE indicates single device response if FALSE
 * 				indicates extended device response.
 * 				uint8_t StartIndex, start index of the child devices list.
 * 				BOOL APSAckRequired, if TRUE indicates APS ack is required if FALSE
 * 				indicates APS ack is not required
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send ZDP IEEE address request to determine the 64-bit 
 * IEEE address of the device whose short address is known.
 *
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_zdp_send_ieee_addr_request(uint16_t shortAddress, BOOL RequestType,uint8_t StartIndex, BOOL APSAckRequired)
{

  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  getZDPIEEEAddrFrameSnd *get_zdp_ieee_addr=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  get_zdp_ieee_addr = (getZDPIEEEAddrFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddress, (uint8_t *)&get_zdp_ieee_addr->ShortAddr);

  get_zdp_ieee_addr->RequestType = RequestType;
  get_zdp_ieee_addr->StartIndex = StartIndex;
  get_zdp_ieee_addr->APSAckRequired = APSAckRequired;

  //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetZDPIEEEAddr, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return status;

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_zdp_send_device_announcement(void)
 *
 * @brief       Sends the Device Announcement command to ZigBee module
 *
 * @param[in]   none
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send a broadcast for a ZDO Device announcement. Normally, 
 * it is NOT required to call this as the stack automatically sends a device 
 * announcement during joining or rejoing, as per the spec. However if the device
 * wishes to re-send its device announcement they can use this call.
 * 
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_zdp_send_device_announcement(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameZDPDevAnnounce, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_active_endpoints_request(uint16_t shortAddress,
 * 						uint8_t APSAckRequired)
 *
 * @brief       Sends the Active Endpoint Request command to ZigBee module
 *
 * @param[in]   uint16_t shortAddress, Device short Address whose active endpoints
 * 				need to be obtained.
 * 				uint8_t APSAckRequired, if TRUE indicates APS ack is required if FALSE
 * 				indicates APS ack is not required
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send a Active Endpoint request for the specified short address. 
 * 
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_active_endpoints_request(uint16_t shortAddress, uint8_t APSAckRequired)
{

  rsi_pkt_t *pkt;
  activeEPOfShortAddrFrameSnd *act_ep_req=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  act_ep_req = (activeEPOfShortAddrFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddress, (uint8_t *)&act_ep_req->ShortAddr);

  act_ep_req->APSAckRequired = (uint8_t)APSAckRequired;

  //! send the command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameActiveEPreq, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_zdp_send_power_descriptor_request(uint16_t shortAddress,
 * 						uint8_t APSAckRequired)
 *
 * @brief       Sends the power descriptor request command to ZigBee module
 *
 * @param[in]   uint16_t shortAddress, Device short Address whose active endpoints
 * 				need to be obtained.
 * 				uint8_t APSAckRequired, if TRUE indicates APS ack is required if FALSE
 * 				indicates APS ack is not required
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send a Power Descriptor request for the specified short address. 
 * 
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_zdp_send_power_descriptor_request(uint16_t shortAddress, uint8_t APSAckRequired)
{

  rsi_pkt_t *pkt;
  powerDescFrameSnd *pwr_desc_req=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  pwr_desc_req = (powerDescFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddress, (uint8_t *)&pwr_desc_req->ShortAddr);

  pwr_desc_req->APSAckRequired = (uint8_t)APSAckRequired;

  //! send the command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameZDPPwrDesc, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_zdp_send_node_descriptor_request(uint16_t shortAddress,
 * 						uint8_t APSAckRequired)
 *
 * @brief       Sends the Node descriptor request command to ZigBee module
 *
 * @param[in]   uint16_t shortAddress, Device short Address whose active endpoints
 * 				need to be obtained.
 * 				uint8_t APSAckRequired, if TRUE indicates APS ack is required if FALSE
 * 				indicates APS ack is not required
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send a Node Descriptor request for the specified short address. 
 * 
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_zdp_send_node_descriptor_request(uint16_t shortAddress, uint8_t APSAckRequired)
{

  rsi_pkt_t *pkt;
  nodeDescFrameSnd *node_desc_req=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  node_desc_req = (nodeDescFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddress, (uint8_t *)&node_desc_req->ShortAddr);

  node_desc_req->APSAckRequired = (uint8_t)APSAckRequired;


  //! send the command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameZDPNodeDesc, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_send_group_data(GroupID GroupAddress, 
 * 						ZigBeeAPSDEDataRequest_t *pAPSDERequest)
 *
 * @brief       Sends the group data command to ZigBee module
 *
 * @param[in]   GroupID GroupAddress, union of 64-bit extended address and 16-bit 
 * 				short address
 * 				ZigBeeAPSDEDataRequest_t *pAPSDERequest, pointer to APSDE_Data_Request_t
 * 				structure
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send a group data for the specified short address. 
 * 
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_send_group_data( GroupID GroupAddress, ZigBeeAPSDEDataRequest_t * pAPSDERequest)
{
  rsi_pkt_t *pkt;
  uint8_t i=0;
  groupDataFrameSnd *group_buf=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  group_buf = (groupDataFrameSnd*)pkt->data;
  group_buf->group_addr = GroupAddress;

  group_buf->DestEndpoint = pAPSDERequest->DestEndpoint;
  group_buf->SrcEndpoint = pAPSDERequest->SrcEndpoint;

  rsi_zigb_uint16to_buffer(pAPSDERequest->ProfileId, (uint8_t *)&group_buf->ProfileId);

  rsi_zigb_uint16to_buffer(pAPSDERequest->ClusterId, (uint8_t *)&group_buf->ClusterId);

  group_buf->AsduLength = pAPSDERequest->AsduLength;
  group_buf->TxOptions = pAPSDERequest->TxOptions;
  group_buf->Radius = pAPSDERequest->Radius;

  for(i = 0; i < pAPSDERequest->AsduLength; i++)
    group_buf->Data[i] = pAPSDERequest->aPayload[i];
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGroupData, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_send_broadcast_data(ZigBeeAPSDEDataRequest_t 
 * 																*pAPSDERequest)
 *
 * @brief       Sends the Broadcast data command to ZigBee module
 *
 * @param[in]   ZigBeeAPSDEDataRequest_t *pAPSDERequest, pointer to APSDE_Data_Request_t
 * 				structure
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to send a broadcast data for the specified short address. 
 * 
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_send_broadcast_data(ZigBeeAPSDEDataRequest_t * pAPSDERequest)
{
  rsi_pkt_t *pkt;
  uint8_t i=0;
  bcastDataFrameSnd *bcast_buf=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  bcast_buf = (bcastDataFrameSnd*)pkt->data;
  bcast_buf->DestEndpoint = pAPSDERequest->DestEndpoint;
  bcast_buf->SrcEndpoint = pAPSDERequest->SrcEndpoint;

  rsi_zigb_uint16to_buffer(pAPSDERequest->ProfileId, (uint8_t *)&bcast_buf->ProfileId);

  rsi_zigb_uint16to_buffer(pAPSDERequest->ClusterId, (uint8_t *)&bcast_buf->ClusterId);

  bcast_buf->AsduLength = pAPSDERequest->AsduLength;
  bcast_buf->TxOptions = pAPSDERequest->TxOptions;
  bcast_buf->Radius = pAPSDERequest->Radius;

  for(i = 0; i < pAPSDERequest->AsduLength; i++)
    bcast_buf->Data[i] = pAPSDERequest->aPayload[i];
  
  //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameBcastData, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_max_aps_payload_length(void)
 *
 * @brief       Sends the get maximum payload length command to ZigBee module
 *
 * @param[in]   none
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to get the maximum payload length Set in ZigBee stack.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_get_max_aps_payload_length(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetMaxAPSSize, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_binding_indices(uint8_t *binding_indices)
 *
 * @brief       Sends the get active binding indices command to ZigBee module
 *
 * @param[in]   none
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to get the active binding indices in ZigBee stack.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_get_binding_indices(uint8_t *binding_indices)
{
  rsi_pkt_t *pkt;
  int16_t no_of_indices = 0; 

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetBindingIndices, pkt);
   
   no_of_indices = zigb_cb->resp.uCmdRspPayLoad.uRspData[0];
   memcpy(binding_indices, &zigb_cb->resp.uCmdRspPayLoad.uRspData[1], no_of_indices);
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  no_of_indices; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_binding_entry(ZDP_Bind_Request_t *pSetBindingEntry)
 *
 * @brief       Sends the set binding entry command to ZigBee module
 *
 * @param[in]   ZDP_Bind_Request_t *pSetBindingEntry, pointer loaction to binding entry
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to set the binding entry in ZigBee stack.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_set_binding_entry(ZDP_Bind_Request_t * pSetBindingEntry)
{
  rsi_pkt_t *pkt;
  setBindEntryFrameSnd *bind_entry=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  bind_entry = (setBindEntryFrameSnd*)pkt->data;
  rsi_zigb_mcpy(pSetBindingEntry->a_src_addr, (uint8_t *)&bind_entry->SrcIEEEAddr, 8);

  bind_entry->SrcEndpoint = pSetBindingEntry->src_endpoint;

  rsi_zigb_mcpy(pSetBindingEntry->a_cluster_id,(uint8_t *)&bind_entry->ClusterId, 2);

  bind_entry->DestAddrMode = pSetBindingEntry->dest_addr_mode;

  rsi_zigb_mcpy(pSetBindingEntry->a_dest_addr,(uint8_t *)&bind_entry->DestIEEEAddr, 8);

  bind_entry->DestEndpoint = pSetBindingEntry->dest_endpoint;
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetBindEntry, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_delete_binding(uint8_t bindIndex)
 *
 * @brief       Sends the delete bind entry command to ZigBee module
 *
 * @param[in]   bindIndex, index whose entry to be deleted
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to delete the binding entry for the specified index in ZigBee stack.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_delete_binding(uint8_t BindIndex)
{
  rsi_pkt_t *pkt;
  delBindEntryFrameSnd *del_bind_entry=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  del_bind_entry = (delBindEntryFrameSnd*)pkt->data;
  del_bind_entry->BindIndex = BindIndex;
  
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameDelBindEntry, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_is_binding_entry_active(uint8_t bindIndex)
 *
 * @brief       Sends the bind entry active command to ZigBee module
 *
 * @param[in]   bindIndex, index of the binding entry.
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to check the binding entry is active or not for the given index.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_is_binding_entry_active(uint8_t BindIndex)
{
  rsi_pkt_t *pkt;
  isBindEntryActiveFrameSnd *bind_entry_buf=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  bind_entry_buf = (isBindEntryActiveFrameSnd*)pkt->data;
  bind_entry_buf->BindIndex = BindIndex;
  
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameIsBindEntryActive, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_clear_binding_table(void)
 *
 * @brief       Sends the clear bind table command to ZigBee module
 *
 * @param[in]   bindIndex, index of the binding entry.
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to clear the binding table.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_clear_binding_table(void)
{
  rsi_pkt_t *pkt;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameClearBindTable, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_bind_request(uint16_t shortAddress,
 *              uint8_t *pIEEEAddrOfSource, uint8_t sourceEndpoint,
 *              uint16_t ClusterId, uint8_t destAddrMode, Address destAddress,
 *              uint8_t destinationEndpoint, BOOL APSAckRequired)
 * @brief       Sends the Binding Request command to ZigBee module
 *
 * @param[in]  
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to create the binding entry between pair of devices
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_bind_request(uint16_t shortAddress, uint8_t *pIEEEAddrOfSource, uint8_t sourceEndpoint, uint16_t ClusterId, uint8_t destAddrMode,
    Address destAddress, uint8_t destinationEndpoint, BOOL APSAckRequired)
{
  rsi_pkt_t *pkt;
  bindReqFrameSnd *bind_req=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  bind_req = (bindReqFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddress, (uint8_t *)&bind_req->SrcShortAddr);

  rsi_zigb_mcpy(pIEEEAddrOfSource, (uint8_t *)&bind_req->SrcIEEEAddr, 8);

  bind_req->SrcEndpoint = sourceEndpoint;

  rsi_zigb_uint16to_buffer(ClusterId, (uint8_t *)&bind_req->ClusterId);

  bind_req->DestAddrMode = destAddrMode;

  if( (destAddrMode == 1) || (destAddrMode == 2)) {
    rsi_zigb_mcpy((uint8_t *)&destAddress.short_address, (uint8_t *)&bind_req->DestAddress, 2);
  }

  if(destAddrMode == 3) {
    rsi_zigb_mcpy(destAddress.IEEE_address, (uint8_t *)&bind_req->DestAddress, 8);
  }

  bind_req->DestEndpoint = destinationEndpoint;

  bind_req->APSAckRequired = (uint8_t)APSAckRequired;
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameBindReq, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_enddevice_bind_request(uint8_t EndPointId,
 * 								BOOL APSAckRequired)
 *
 * @brief       Sends the End Device bind request command to ZigBee module
 *
 * @param[in]   uint8_t EndPointId, Endpoint to which the bindind to be done.
 * 				BOOL APSAckRequired, if TRUE indicates APS ack is required if FALSE
 * 				indicates APS ack is not required
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to initiate ZigBee Binding Request . By default the EndDevice Binding 
 * request timeout is configured to 10 seconds.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_enddevice_bind_request(uint8_t EndPointId, BOOL APSAckRequired)
{
  rsi_pkt_t *pkt;
  endDevBindFrameSnd *enddev_bind=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  enddev_bind = (endDevBindFrameSnd*)pkt->data;
  enddev_bind->EndPointId = EndPointId;
  enddev_bind->APSAckRequired = APSAckRequired;
  
  //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEndDevBind, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_unbind_request(uint16_t shortAddress,
 *              uint8_t *pIEEEAddrOfSource, uint8_t sourceEndpoint,
 *              uint16_t ClusterId, uint8_t destAddrMode,
 *              Address destAddress, uint8_t destinationEndpoint,
 *              BOOL APSAckRequired)
 *
 * @brief       Sends the unbind request command to ZigBee module
 *
 * @param[in]   
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to remove the binding entry between pair of devices
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_unbind_request(uint16_t shortAddress, uint8_t *pIEEEAddrOfSource, uint8_t sourceEndpoint, uint16_t ClusterId, uint8_t destAddrMode,
    Address destAddress, uint8_t destinationEndpoint, BOOL APSAckRequired)
{
  rsi_pkt_t *pkt;
  unbindReqFrameSnd *unbind_buf=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  unbind_buf = (unbindReqFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddress, (uint8_t *)&unbind_buf->SrcShortAddr);

  rsi_zigb_mcpy(pIEEEAddrOfSource, (uint8_t *)&unbind_buf->SrcIEEEAddr, 8);

  unbind_buf->SrcEndpoint = sourceEndpoint;

  rsi_zigb_uint16to_buffer(ClusterId, (uint8_t *)&unbind_buf->ClusterId);

  unbind_buf->DestAddrMode = destAddrMode;

  if( (destAddrMode == 1) || (destAddrMode == 2))	{
    rsi_zigb_mcpy((uint8_t *)&destAddress.short_address, (uint8_t *)&unbind_buf->DestAddress, 2);
  }

  if(destAddrMode == 3) {
    rsi_zigb_mcpy(destAddress.IEEE_address, (uint8_t *)&unbind_buf->DestAddress, 8);
  }

  unbind_buf->DestEndpoint = destinationEndpoint;

  unbind_buf->APSAckRequired = (uint8_t)APSAckRequired;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameUnbind, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_simple_descriptor_request(uint16_t shortAddress,
 * 						uint8_t EndpointId)
 *
 * @brief       Sends the Simple descriptor request command to ZigBee module
 *
 * @param[in]   uint16_t shortAddress, Device short Address whose active endpoints
 * 				need to be obtained.
 * 				uint8_t EndpointId, Endpoint whose simple descriptor to be obtained.
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to get Simple Descriptor request from the specified short address. 
 * 
 * @section Prerequisite
 * The device should be part of the network to transmit this command successfully. 
 *
 */
int16_t rsi_zigb_simple_descriptor_request(uint16_t shortAddress, uint8_t EndPointId)
{

  rsi_pkt_t *pkt;
  getSimpleDescOfShortAddrFrameSnd *get_simple_desc=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_simple_desc = (getSimpleDescOfShortAddrFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddress, (uint8_t *)&get_simple_desc->ShortAddr);

  get_simple_desc->EndPointId = (uint8_t)EndPointId;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSimpleDescReq, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          APSME_Address_Map_Table_t* rsi_zigb_get_address_map_table_entry(uint8_t Index)
 *
 * @brief       Sends the get the address map table entry command to ZigBee
 *
 * @param[in]   Index, index of the address map table entry
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to get the address map table entry for the specified index
 * 
 * @section Prerequisite
 *
 */
APSME_Address_Map_Table_t* rsi_zigb_get_address_map_table_entry(uint8_t Index)
{
  rsi_pkt_t *pkt;
  getAddrMapTableFrameSnd *addr_map=NULL;
   
  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  
  addr_map = (getAddrMapTableFrameSnd*)pkt->data;
  addr_map->Index = (uint8_t)Index;
  
  //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetAddrMapTable, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  &zigb_cb->resp.uCmdRspPayLoad.GetAddrMapResp; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_key(Security_Key_Types keytype)
 *
 * @brief       Sends the Get key command to ZigBee module
 *
 * @param[in]   Security_Key_Types keytype, type of the key to get
 *
 * @param[out]  none
 *
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 *
 * @section description 
 * This API is used to get the specified key and its associated data. This can retrieve
 * the Link Key, Current Network Key, or Next Network Key.
 * 
 * @section Prerequisite
 *
 */
int16_t rsi_zigb_get_key(Security_Key_Types KeyType)
{
  rsi_pkt_t *pkt;
  getKeyFrameSnd *get_key=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  get_key = (getKeyFrameSnd*)pkt->data;
  get_key->KeyType = KeyType;
  
  //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetKey, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_have_link_key(uint8_t *pRemoteDeviceIEEEAddr)
 * @brief       Sends the Reuqest link key command to ZigBee module
 * @param[in]   pRemoteDeviceIEEEAddr - Trust center 64-bit extended address
 * 				partnerIEEEAddr - address of the child
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get trust center linkkey from our device so that we are
 * securing our messages  sent to the child joined
 *
 */
int16_t rsi_zigb_have_link_key(uint8_t *pRemoteDeviceIEEEAddr)
{
  rsi_pkt_t *pkt;
  uint8_t i=0;
   
  haveLinkKeyFrameSnd *link_key=NULL;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  link_key = (haveLinkKeyFrameSnd*)pkt->data;
  for(i =0; i < 8; i++) {
    link_key->ieee_Addr[i] = pRemoteDeviceIEEEAddr[i];
  }
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameHaveLinkKey, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_request_link_key(uint8_t *trustCentreIEEEAddr,  
 * 						uint8_t *partnerIEEEAddr)
 * @brief       Sends the Reuqest link key command to ZigBee module
 * @param[in]   trustCentreIEEEAddr - Trust center 64-bit extended address
 * 				partnerIEEEAddr - address of the child
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get trust center linkkey for the child joined
 *
 */
int16_t rsi_zigb_request_link_key(uint8_t* TrustCenterIEEEAddr, uint8_t* PartnerIEEEAddr)
{

  rsi_pkt_t *pkt;
  reqLinkKeyFrameSnd *req_link_key=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  req_link_key = (reqLinkKeyFrameSnd*)pkt->data;
  
  rsi_zigb_mcpy(TrustCenterIEEEAddr, (uint8_t *)&req_link_key->TrustCenterIEEEAddr, 8);

  rsi_zigb_mcpy(PartnerIEEEAddr, (uint8_t *)&req_link_key->PartnerIEEEAddr, 8);


  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameReqLinkKey, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_key_table_entry(uint8_t index,  
 * 						ZigBeeKeyStructure_t *KeyStruct)
 * @brief       Sends the get key table entry command to ZigBee module
 * @param[in]   Index - index to set the key
 * 				KeyStruct - Key Structure to update
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to get the key configuration for the given index
 *
 */
int16_t rsi_zigb_get_key_table_entry (uint8_t Index, ZigBeeKeyStructure_t *keyStruct)
{

  rsi_pkt_t *pkt;
  getKeyTableFrameSnd *key_table=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  key_table = (getKeyTableFrameSnd*)pkt->data;
  
  key_table->Index = Index;


  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetKeyTable, pkt);

  rsi_zigb_mcpy((uint8_t *)&zigb_cb->resp.uCmdRspPayLoad.GetKeytableResp.key_struct,  (uint8_t *)&keyStruct, sizeof(ZigBeeKeyStructure_t));
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.GetKeytableResp.status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_key_table_entry(uint8_t index, uint8_t 
 * 						*IeeeAddr, Bool linkkey, uint8_t *pKeyData)
 * @brief       Sends the set key table entry command to ZigBee module
 * @param[in]   Index - index to set the key
 * 				IeeeAddr - 64-bit extended addreess
 *              linkkey -  Checking for Link key or not
 *				pKeyData - key to update
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to set the key configuration in the given index.
 *
 */
int16_t rsi_zigb_set_key_table_entry( uint8_t Index, uint8_t * pIEEEAddress, BOOL LinkKey, uint8_t * pKeyData )
{

  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  setKeyTableFrameSnd *key_table=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  key_table = (setKeyTableFrameSnd*)pkt->data;
  
  key_table->Index = Index;

  rsi_zigb_mcpy(pIEEEAddress, (uint8_t *)&key_table->ieee_Addr, 8);

  key_table->LinkKey = (uint8_t)LinkKey;

  rsi_zigb_mcpy(pKeyData, (uint8_t *)&key_table->KeyData, 16);

  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetKeyTable, pkt);
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_add_or_update_key_table_entry(uint8_t *IeeeAddr,
 * 						Bool linkkey, uint8_t *pKeyData, uint8_t Index)
 * @brief       Sends the key update command to ZigBee module
 * @param[in]   IeeeAddr - 64-bit extended addreess
 *              linkkey -  Checking for Link key or not
 *				pKeyData - key to update
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to add any new entry or update the already presented entries
 * in Key Table.
 *
 */
int16_t rsi_zigb_add_or_update_key_table_entry(uint8_t *pIEEEAddress, BOOL LinkKey, uint8_t *pKeyData, uint8_t *indx)
{
  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  addKeyTableFrameSnd *key_table=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  key_table = (addKeyTableFrameSnd*)pkt->data;
  
  rsi_zigb_mcpy(pIEEEAddress, (uint8_t *)&key_table->ieee_Addr, 8);

  key_table->LinkKey = (uint8_t)LinkKey;

  rsi_zigb_mcpy(pKeyData, (uint8_t *)&key_table->KeyData, 16);
  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameAddKeyTable, pkt);
  
  status= zigb_cb->resp.uCmdRspPayLoad.uRspData[0];
  *indx = zigb_cb->resp.uCmdRspPayLoad.uRspData[1];
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_find_key_table_entry(uint8_t *IeeeAddr, Bool Linkkey)
 * @brief       Sends the find key table entry command to ZigBee module
 * @param[in]   IeeeAddr - 64-bit extended address
 *              Linkkey -  Checking for Link key
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to find the table entry using on 64-bit extended address
 *
 */
int16_t rsi_zigb_find_key_table_entry(uint8_t * pIEEEAddress, BOOL LinkKey)
{

  rsi_pkt_t *pkt;
  findKeyTableFrameSnd *key_table=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  key_table = (findKeyTableFrameSnd*)pkt->data;
  
  rsi_zigb_mcpy(pIEEEAddress, (uint8_t *)&key_table->ieee_Addr, 8);

  key_table->LinkKey = (uint8_t)LinkKey;
  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameFindKeyTable, pkt);
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_erase_key_table_entry(uint8_t Index)
 * @brief       Sends the Key table deletion entry from ZigBee module
 * @param[in]   Index - key table Index to erase
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to erase the key table entry based on given Index
 *
 */
int16_t rsi_zigb_erase_key_table_entry(uint8_t Index)
{

  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  eraseKeyTableFrameSnd *key_table=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  key_table = (eraseKeyTableFrameSnd*)pkt->data;
  key_table->Index = Index;
  
  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEraseKeyTable, pkt);
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_set_profile(uint8_t profile_id)
 * @brief       Sends the Zll profile info to the Module 
 * @param[in]   Profile - 1 - ZLL Profile Enable
 *                        0 - ZLL profile Disable
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to chnage the operating channel in ZigBee stack
 *
 */
int16_t rsi_zigb_set_profile(uint8_t profile_id)
{

  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  setProfileFrameSnd *profile=NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  profile = (setProfileFrameSnd*)pkt->data;
  profile->ProfileId = profile_id;
  
  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetProfile, pkt);
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}

/*===========================================================================
 *
 * @fn          Route_Record_Table_t* ZigBeeGetRouteRecordForDevice(uint16_t shortAddr)
 * @brief       Sends the Zll profile info to the Module 
 * @param[in]   Profile - 1 - ZLL Profile Enable
 *                        0 - ZLL profile Disable
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to chnage the operating channel in ZigBee stack
 *
 */
Route_Record_Table_t* ZigBeeGetRouteRecordForDevice(uint16_t shortAddr)
{
  rsi_pkt_t *pkt;
   
  getRouteRecordForDeviceFrameSnd *getRouteRecordForDevice=NULL;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  getRouteRecordForDevice = (getRouteRecordForDeviceFrameSnd*)pkt->data;
  rsi_zigb_uint16to_buffer(shortAddr, (uint8_t *)&getRouteRecordForDevice->ShortAddr);

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetRouteRecord, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return &zigb_cb->resp.uCmdRspPayLoad.GetRouteRecordResp;

}

/*===========================================================================
 *
 * @fn          int16_t APS_APSMEremoveGroup(Remove_Group_Request_t* pRemoveGroupEntry)
 * @brief       Sends the Zll profile info to the Module 
 * @param[in]   Profile - 1 - ZLL Profile Enable
 *                        0 - ZLL profile Disable
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to chnage the operating channel in ZigBee stack
 *
 */
uint8_t APS_APSMEremoveGroup(Remove_Group_Request_t* pRemoveGroupEntry)
{
  rsi_pkt_t *pkt;
  Remove_Group_Request_t* pRemoveGroupEntry_temp = NULL;
   
  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);
  //! If allocation of packet fails
  if(pkt == NULL)
  {
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
    pRemoveGroupEntry_temp = (Remove_Group_Request_t*)pkt->data;
    pRemoveGroupEntry_temp->group_address =  pRemoveGroupEntry->group_address;
    pRemoveGroupEntry_temp->endpoint      =  pRemoveGroupEntry->endpoint;
  
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameApsmeRemGrp, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_find_network_and_perform_rejoin(bool secured,
 * 					uint32_t ChannelMask)
 * @brief       Sends the Find network and perform rejoin command to ZigBee module 
 * @param[in]   Secured - Is rejoin secured or not
 * 				ChannelMask - Channels to scan
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to perform rejoin by finding network by stack itself
 *
 */
int16_t rsi_zigb_find_network_and_perform_rejoin(BOOL Secured, uint32_t ChannelMask)
{

  rsi_pkt_t *pkt;
  findNWKRejoinFrameSnd *find_nwk_rejoin = NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  find_nwk_rejoin = (findNWKRejoinFrameSnd*)pkt->data;
  find_nwk_rejoin->ChannelMask = ChannelMask;
  find_nwk_rejoin->Secured = Secured;
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameFindNWKnRejoin, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_rejoin_network(bool secured)
 * @brief       Sends the perform rejoin command to ZigBee module 
 * @param[in]   Secured - Is rejoin secured or not
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to perform rejoin in the channel connected
 *
 */
int16_t rsi_zigb_rejoin_network(BOOL Secured)
{

  rsi_pkt_t *pkt;
  rejoinNWKFrameSnd *nwk_rejoin = NULL;
   

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }

  nwk_rejoin = (rejoinNWKFrameSnd*)pkt->data;
  nwk_rejoin->Secured = Secured;
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameRejoinNWK, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}


/*==============================================*/
/**
 * @fn          void rsi_nonos_event_loop(void)
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

void rsi_non_os_event_loop(void)
{
  rsi_scheduler(&rsi_driver_cb->scheduler_cb);  
}
/* PER RELATED FUNCTIONS */


/*===========================================================================
 *
 * @fn          uint8_t rsi_zigb_per_transmit(uint8_t power,
 *                uint16_t packet_len, uint8_t mode, uint8_t channel,
 *                uint8_t pkt_count)
 * @brief       Sends the configuration of power,packet len,mode,channel and packet count.
 * @param[in]   power      - This is the transmit power (in dBm) to be used by the module.
 * @param[in]   packet_len - This is the length of the packet, in bytes, to be transmitted.
 * @param[in]   mode       - This parameter is used to choose between Burst0()
 *                           and Continuous(1) modes of transmission.
 * @param[in]   channel - This parameter indicates the channel index, as per the ZigBee standard.
 * @param[in]   pkt_cnt - This is the number of packets to be transmitted
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to send the configuration of power,packet len,mode,channel and packet count.
 *
 */
int16_t rsi_zigb_per_transmit(uint8_t power, uint16_t packet_len, uint8_t mode, uint8_t channel, uint8_t pkt_count)
{
  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  rsi_zigb_per_params_t *per_params;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
  
  
  pkt->data[0] = ZIGB_PER_TRANSMIT;
  pkt->data[2] = sizeof(rsi_zigb_per_params_t) + 4;
  
  per_params = (rsi_zigb_per_params_t *)(&pkt->data[4]);

  per_params->enable = 1;
  per_params->power =  power;
  per_params->pkt_length =packet_len  ;
  per_params->mode =  mode;
  per_params->channel = channel;
  per_params->packet_count = pkt_count ;

  //! send command
  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framepermode, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}

/*===========================================================================
 *
 * @fn          uint8_t rsi_zigb_per_transmit_stop(void)
 * @brief       to stop PER tranmsit
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to to stop PER tranmsit.
 *
 */
int16_t rsi_zigb_per_transmit_stop(void)
{
  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  
  rsi_zigb_per_params_t *per_params;

  //! Get common cb structure pointer
  rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! if state is not in card ready received state
  if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
    RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
    //! Command given in wrong state 
    return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
  }
  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  //! If allocation of packet fails
  if(pkt == NULL)
  {
#ifdef ZB_DEBUG
    RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
    //! unlock mutex
    rsi_mutex_unlock(&zigb_cb->zigb_mutex);

    //! return packet allocation failure error
    return RSI_ERROR_PKT_ALLOCATION_FAILURE;
  }
   
  pkt->data[0] = ZIGB_PER_TRANSMIT;
  pkt->data[2] = 1 + 4;
  
  per_params = (rsi_zigb_per_params_t *)(&pkt->data[4]);

  per_params->enable = 0;

  //! send command
  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framepermode, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}

/*===========================================================================
 *
 * @fn          uint8_t rsi_zigb_perstats(per_stats_info_t *per_stats)
 * @brief       to get PER stats
 * @param[in]   per_stats - per_stats i.e crc pass,crc fail & rssi.
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to to get PER stats.
 *
 */
int16_t rsi_zigb_perstats(per_stats_info_t *per_stats)
{
	rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
	rsi_zigb_stats_t module_stats;

	//! Get common cb structure pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	//! Get zigb cb structure pointer
	rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

	//! if state is not in card ready received state
	if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
	{
#ifdef ZB_DEBUG
		RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
		RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
		//! Command given in wrong state 
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}
	//! take lock on zigb control block
	rsi_mutex_lock(&zigb_cb->zigb_mutex);

	//! allocate command buffer  from zigb pool
	pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
#ifdef ZB_DEBUG
		RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
		//! unlock mutex
		rsi_mutex_unlock(&zigb_cb->zigb_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	pkt->data[0] = ZIGB_PER_STATS;
	pkt->data[2] = 1 + 4;

	//! send command
	rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framepermode, pkt);

	//! free the packet
	rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

	memcpy(&module_stats, zigb_cb->resp.uCmdRspPayLoad.uRspData, sizeof(rsi_zigb_stats_t));

	per_stats->crc_pass = module_stats.crc_pass;
	per_stats->crc_fail = module_stats.crc_fail;
	per_stats->rssi = module_stats.rssi;
	
	//! unlock mutex
	rsi_mutex_unlock(&zigb_cb->zigb_mutex);
	//! Return the status
	return  status; 

}

/*===========================================================================
 *
 * @fn          uint8_t rsi_zigb_ant_sel(uint8_t ant_sel_value)
 * @brief       to select antenna
 * @param[in]   ant_sel_value - antenna select value.
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to select antenna.
 *
 */
int16_t rsi_zigb_ant_sel(uint8_t ant_sel_value)
{
	rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
	rsi_zigb_stats_t module_stats;

	//! Get common cb structure pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	//! Get zigb cb structure pointer
	rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

	//! if state is not in card ready received state
	if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
	{
#ifdef ZB_DEBUG
		RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
		RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
		//! Command given in wrong state 
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}
	//! take lock on zigb control block
	rsi_mutex_lock(&zigb_cb->zigb_mutex);

	//! allocate command buffer  from zigb pool
	pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
#ifdef ZB_DEBUG
		RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
		//! unlock mutex
		rsi_mutex_unlock(&zigb_cb->zigb_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}

	pkt->data[0] = ZIGB_ANT_SEL;
	pkt->data[1] = ant_sel_value;
	pkt->data[2] = 1 + 4;

	//! send command
	rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framepermode, pkt);

	//! free the packet
	rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&zigb_cb->zigb_mutex);
	//! Return the status
	return  status; 

}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_dump_nvm(uint8_t *data,uint16_t offset,uint16_t len)
 * @brief       to select antenna
 * @param[in]   ant_sel_value - antenna select value.
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to dump nvm data.
 *
 */
rsi_pkt_t *pkt;
int16_t rsi_zigb_dump_nvm(uint8_t *data,uint16_t offset,uint16_t len)
{
  int16_t status = RSI_SUCCESS; 
	rsi_zigb_stats_t module_stats;
  uint16_t         *offset_p;
	//! Get common cb structure pointer
	rsi_common_cb_t *rsi_common_cb = rsi_driver_cb->common_cb;

	//! Get zigb cb structure pointer
	rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

	//! if state is not in card ready received state
	if(rsi_common_cb->state < RSI_COMMON_OPERMODE_DONE )
	{
#ifdef ZB_DEBUG
		RSI_DPRINT(RSI_PL1,"\n#########ERROR######\n");
		RSI_DPRINT(RSI_PL1,"common_state: 0x%x",rsi_common_cb->state);
#endif
		//! Command given in wrong state 
		return RSI_ERROR_COMMAND_GIVEN_IN_WRONG_STATE;
	}
	//! take lock on zigb control block
	rsi_mutex_lock(&zigb_cb->zigb_mutex);

	//! allocate command buffer  from zigb pool
	pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

	//! If allocation of packet fails
	if(pkt == NULL)
	{
#ifdef ZB_DEBUG
		RSI_DPRINT(RSI_PL1,"\n######### NULL pkt######\n");
#endif
		//! unlock mutex
		rsi_mutex_unlock(&zigb_cb->zigb_mutex);

		//! return packet allocation failure error
		return RSI_ERROR_PKT_ALLOCATION_FAILURE;
	}
  offset_p = (uint16_t*)(pkt->data);
  *offset_p = offset;
  memcpy((uint8_t*)&pkt->data[2],data,len);

	//! send command
	rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framenvmupdate, pkt);

	//! free the packet
	rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

	//! unlock mutex
	rsi_mutex_unlock(&zigb_cb->zigb_mutex);
	//! Return the status
	return  status; 

}

void rsi_update_nvm_params(uint16_t offset)
{
  uint16_t SASIndex = 0;
  rsi_zigb_app_info_t   *app_info = &rsi_zigb_app_info;
  if(offset == g_NVM_SAS_TABLE_START_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.sas_index,offset,sizeof(app_info->nvm_storage.sas_index));
  }

  if(offset == (g_NVM_SAS_TABLE_START_OFFSET_c + 2))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.gp_ZDO_Configuration,offset,sizeof(app_info->nvm_storage.gp_ZDO_Configuration));
  }
  
  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 1) * m_SIZE_CONFIGURATION_c)))
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.sas_data,offset,m_SIZE_CONFIGURATION_c);
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 2) * m_SIZE_CONFIGURATION_c)))
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.sas_data,offset,m_SIZE_CONFIGURATION_c);
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 3) * m_SIZE_CONFIGURATION_c)))
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.sas_data,offset,m_SIZE_CONFIGURATION_c);
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 4) * m_SIZE_CONFIGURATION_c)))

  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.sas_data,offset,m_SIZE_CONFIGURATION_c);
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 5) * m_SIZE_CONFIGURATION_c)))

  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.sas_data,offset,m_SIZE_CONFIGURATION_c);
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 6) * m_SIZE_CONFIGURATION_c)))

  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.sas_data,offset,m_SIZE_CONFIGURATION_c);
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 7) * m_SIZE_CONFIGURATION_c)))
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.sas_data,offset,m_SIZE_CONFIGURATION_c);
  }
  
  if(offset == g_NVM_TABLES_START_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.nvm_status,offset,2);
  }

  if(offset == g_NEIGHBOR_TABLE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.Neighbor_Table,offset,sizeof(app_info->nvm_storage.Neighbor_Table));
  }

//#if ( g_NWK_MESH_ROUTING_d == 1 )
#if (defined(ZIGBEE_ROUTER) || (ZIGBEE_COORDINATOR))
  if(offset == g_ROUTE_TABLE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.Route_Table,offset,sizeof(app_info->nvm_storage.Route_Table));
  }
#endif 

  if(offset == g_SECURITY_MATERIAL_DESCRIPTOR_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.Security_Material_Descriptor,offset,sizeof(app_info->nvm_storage.Security_Material_Descriptor));
  }


  if(offset == g_PERSISTENT_NIB_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.Persistent_NIB,offset,sizeof(app_info->nvm_storage.Persistent_NIB));
  }


  if(offset == g_ZDO_INFORMATION_BASE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.ZDO_Information_Base,offset,sizeof(app_info->nvm_storage.ZDO_Information_Base));
  }


//#if ( g_NWK_MANY_TO_ONE_ROUTING_d == 1 )
//#if (g_NWK_ROUTE_RECORD_TABLE_d == 1)
  if(offset == g_ROUTE_RECORD_TABLE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.route_record_table,offset,sizeof(app_info->nvm_storage.route_record_table));
  }
//#endif

  if(offset == g_LINK_KEY_COUNTER_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.LinkKeyFrameCounterTable,offset,sizeof(app_info->nvm_storage.LinkKeyFrameCounterTable));
  }


  if(offset == g_LINK_KEY_TABLE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.LinkKeyDeviceKeyPairTable,offset,sizeof(app_info->nvm_storage.LinkKeyDeviceKeyPairTable));
  }


  if(offset == g_BINDING_TABLE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.src_bind_enrty,offset,sizeof(app_info->nvm_storage.src_bind_enrty));
  }


//#if ( g_APS_GROUP_DATA_RX_d == 1 )
//#ifdef ZIGBEE_END_DEVICE 
  if(offset == g_GROUP_TABLE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.Group_Table,offset,sizeof(app_info->nvm_storage.Group_Table));
  }
//#endif       


  if(offset == g_ADDRESS_MAP_TABLE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.Address_Map_Table,offset,sizeof(app_info->nvm_storage.Address_Map_Table));
  }


//#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )
//#if (( g_TRUST_CENTRE_d == 1 ) && ( g_USE_HASH_KEY == 0 ) )
//#if ( g_APP_LINK_KEY_TABLE_app_info->nvm_storage_d == RSI_ENABLE )
#ifdef ZIGBEE_COORDINATOR
  if(offset == g_APP_LINK_KEY_TABLE_OFFSET_c)
  {
    rsi_zigb_dump_nvm((uint8_t*)app_info->nvm_storage.LinkKeyTable,offset,sizeof(LinkKeyTable_t));
  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (1 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[1],offset,sizeof(LinkKeyTable_t));
  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (2 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[2],offset,sizeof(LinkKeyTable_t));
  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (3 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[3],offset,sizeof(LinkKeyTable_t));
  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (4 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[4],offset,sizeof(LinkKeyTable_t));
  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (5 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[5],offset,sizeof(LinkKeyTable_t));
  }

  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (6 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[6],offset,sizeof(LinkKeyTable_t));
  }

  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (7 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[7],offset,sizeof(LinkKeyTable_t));
  }

  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (8 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[8],offset,sizeof(LinkKeyTable_t));
  }

  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (9 * sizeof(LinkKeyTable_t))))
  {
    rsi_zigb_dump_nvm((uint8_t*)&app_info->nvm_storage.LinkKeyTable[9],offset,sizeof(LinkKeyTable_t));
  }
#endif       

}

uint16_t rsi_zigb_get_nvm_payload_len(uint16_t offset)
{
  uint16_t SASIndex = 0;
  uint16_t ret = 0;
  rsi_zigb_app_info_t   *app_info = &rsi_zigb_app_info;
  //SASIndex= app_info->nvm_storage.sas_index;
  if(offset == g_NVM_SAS_TABLE_START_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.sas_index);
  }

  if(offset == (g_NVM_SAS_TABLE_START_OFFSET_c + 2))
  {
    return sizeof(app_info->nvm_storage.gp_ZDO_Configuration);
  }
  
  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 1) * m_SIZE_CONFIGURATION_c)))
  {
    return m_SIZE_CONFIGURATION_c;
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 2) * m_SIZE_CONFIGURATION_c)))
  {
    return m_SIZE_CONFIGURATION_c;

  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 3) * m_SIZE_CONFIGURATION_c)))
  {
    return m_SIZE_CONFIGURATION_c;
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 4) * m_SIZE_CONFIGURATION_c)))

  {
        return m_SIZE_CONFIGURATION_c;
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 5) * m_SIZE_CONFIGURATION_c)))

  {
        return m_SIZE_CONFIGURATION_c;
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 6) * m_SIZE_CONFIGURATION_c)))

  {
        return m_SIZE_CONFIGURATION_c;
  }

  if(offset == ((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 7) * m_SIZE_CONFIGURATION_c)))
  {
        return m_SIZE_CONFIGURATION_c;
  }
  
  if(offset == g_NVM_TABLES_START_OFFSET_c)
  {
    return 2;
  }

  if(offset == g_NEIGHBOR_TABLE_OFFSET_c)
  {
    ret = sizeof(app_info->nvm_storage.Neighbor_Table);
    return ret;
  }

//#if ( g_NWK_MESH_ROUTING_d == 1 )
#if (defined(ZIGBEE_ROUTER) || (ZIGBEE_COORDINATOR))
  if(offset == g_ROUTE_TABLE_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.Route_Table);
  }
#endif 

  if(offset == g_SECURITY_MATERIAL_DESCRIPTOR_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.Security_Material_Descriptor);
  }


  if(offset == g_PERSISTENT_NIB_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.Persistent_NIB);
  }


  if(offset == g_ZDO_INFORMATION_BASE_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.ZDO_Information_Base);
  }


//#if ( g_NWK_MANY_TO_ONE_ROUTING_d == 1 )
//#if (g_NWK_ROUTE_RECORD_TABLE_d == 1)
  if(offset == g_ROUTE_RECORD_TABLE_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.route_record_table);
  }
//#endif

  if(offset == g_LINK_KEY_COUNTER_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.LinkKeyFrameCounterTable);
  }


  if(offset == g_LINK_KEY_TABLE_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.LinkKeyDeviceKeyPairTable);
  }


  if(offset == g_BINDING_TABLE_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.src_bind_enrty);
  }


//#if ( g_APS_GROUP_DATA_RX_d == 1 )
//#ifdef ZIGBEE_END_DEVICE 
  if(offset == g_GROUP_TABLE_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.Group_Table);
  }
//#endif       


  if(offset == g_ADDRESS_MAP_TABLE_OFFSET_c)
  {
    return sizeof(app_info->nvm_storage.Address_Map_Table);
  }


//#if ( g_APS_LAYER_SEC_PROCESSING_d == 1 )
//#if (( g_TRUST_CENTRE_d == 1 ) && ( g_USE_HASH_KEY == 0 ) )
//#if ( g_APP_LINK_KEY_TABLE_app_info->nvm_storage_d == RSI_ENABLE )
#ifdef ZIGBEE_COORDINATOR
  if(offset == g_APP_LINK_KEY_TABLE_OFFSET_c)
  {
    return sizeof(LinkKeyTable_t);
  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (1 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);
  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (2 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);

  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (3 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);

  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (4 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);

  }


  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (5 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);

  }

  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (6 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);

  }

  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (7 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);

  }

  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (8 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);

  }

  if(offset == (g_APP_LINK_KEY_TABLE_OFFSET_c + (9 * sizeof(LinkKeyTable_t))))
  {
    return sizeof(LinkKeyTable_t);

  }
#endif       

}


uint8_t rsi_dump_nvm_to_device()
{
  uint16_t SASIndex = 0;
  uint16_t length=0;
  rsi_zigb_app_info_t   *app_info = &rsi_zigb_app_info;
  RSI_DPRINT(RSI_PL1,"\n ###sas index\n");
  rsi_update_nvm_params(g_NVM_SAS_TABLE_START_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###ZDO\n");
  rsi_update_nvm_params((g_NVM_SAS_TABLE_START_OFFSET_c + 2));
  RSI_DPRINT(RSI_PL1,"\n ###SAS\n");
  RSI_DPRINT(RSI_PL1,"\n ###ID = %d\n",((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 1) * m_SIZE_CONFIGURATION_c)));
  rsi_update_nvm_params(((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 1) * m_SIZE_CONFIGURATION_c)));
  RSI_DPRINT(RSI_PL1,"\n ###ID 2 = %d\n",((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 2) * m_SIZE_CONFIGURATION_c)));
  rsi_update_nvm_params(((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 2) * m_SIZE_CONFIGURATION_c)));
  rsi_update_nvm_params(((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 3) * m_SIZE_CONFIGURATION_c)));
  rsi_update_nvm_params(((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 4) * m_SIZE_CONFIGURATION_c)));
  rsi_update_nvm_params(((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 5) * m_SIZE_CONFIGURATION_c)));
  rsi_update_nvm_params(((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 6) * m_SIZE_CONFIGURATION_c)));
  rsi_update_nvm_params(((g_NVM_SAS_TABLE_START_OFFSET_c) + ((SASIndex + 7) * m_SIZE_CONFIGURATION_c)));
  RSI_DPRINT(RSI_PL1,"\n ###nvm status\n");
  rsi_update_nvm_params(g_NVM_TABLES_START_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###Neighbpr table\n");
  rsi_update_nvm_params(g_NEIGHBOR_TABLE_OFFSET_c);
#if (defined(ZIGBEE_ROUTER) || (ZIGBEE_COORDINATOR))
  rsi_update_nvm_params(g_ROUTE_TABLE_OFFSET_c);
#endif
  RSI_DPRINT(RSI_PL1,"\n ###Press Key\n");
  getchar();
  RSI_DPRINT(RSI_PL1,"\n ###Security\n");
  rsi_update_nvm_params(g_SECURITY_MATERIAL_DESCRIPTOR_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###PIB\n");
  rsi_update_nvm_params(g_PERSISTENT_NIB_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###ZDO info\n");
  rsi_update_nvm_params(g_ZDO_INFORMATION_BASE_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###route record\n");
  rsi_update_nvm_params(g_ROUTE_RECORD_TABLE_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###link key\n");
  rsi_update_nvm_params(g_LINK_KEY_COUNTER_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###lin kkey table\n");
  rsi_update_nvm_params(g_LINK_KEY_TABLE_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###bind\n");
  rsi_update_nvm_params(g_BINDING_TABLE_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###grp\n");
  rsi_update_nvm_params(g_GROUP_TABLE_OFFSET_c);
  RSI_DPRINT(RSI_PL1,"\n ###addr map\n");
  rsi_update_nvm_params(g_ADDRESS_MAP_TABLE_OFFSET_c);
#ifdef ZIGBEE_COORDINATOR
  RSI_DPRINT(RSI_PL1,"\n ###lin keytable\n");
  rsi_update_nvm_params(g_APP_LINK_KEY_TABLE_OFFSET_c);
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (1 * sizeof(LinkKeyTable_t))));
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (2 * sizeof(LinkKeyTable_t))));
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (3 * sizeof(LinkKeyTable_t))));
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (4 * sizeof(LinkKeyTable_t))));
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (5 * sizeof(LinkKeyTable_t))));
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (6 * sizeof(LinkKeyTable_t))));
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (7 * sizeof(LinkKeyTable_t))));
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (8 * sizeof(LinkKeyTable_t))));
  rsi_update_nvm_params((g_APP_LINK_KEY_TABLE_OFFSET_c + (9 * sizeof(LinkKeyTable_t))));
#endif       
}


#endif
