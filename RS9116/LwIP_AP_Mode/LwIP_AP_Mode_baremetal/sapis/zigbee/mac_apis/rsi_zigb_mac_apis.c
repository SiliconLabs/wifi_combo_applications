/**
 * @file     rsi_zigb_mac_apis.c
 * @version  0.1
 * @date     2015-Dec-07
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
#include "rsi_zb_api.h"
#include "rsi_mac_interface.h"
#include "rsi_mac_api.h"

/*
 * Global Variables
 * */
/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_data_req(uint8_t dsn_enable,uint8_t dsn, MCPS_Data_Request_t *Data_Request)
 * @brief       Sends Data Request
 * @param[in]   MCPS_Data_Request_t *Data_Request
 * 				dsn_enable = sequence number valid enable
 * 				dsn = sequence number to transmit
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This request causes an MSDU to be transmitted.
 *
 */
int16_t rsi_zigb_mac_data_req(uint8_t dsn_enable,uint8_t dsn, MCPS_Data_Request_t *Data_Request)
{
  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  pkt->data[0] = dsn_enable;
  pkt->data[1] = dsn;
  memcpy(&pkt->data[2],(uint8_t*)Data_Request,sizeof(MCPS_Data_Request_t)+ Data_Request->msdu_length);
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMcps, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_assoc_req(MLME_Associate_Request_t *assoc_Request)
 * @brief       Sends assoc Request
 * @param[in]   MLME_Associate_Request_t *assoc_Request
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api allows a device to request an association with a coordinator. 
 */

int16_t rsi_zigb_mac_assoc_req(MLME_Associate_Request_t *assoc_request)
{
  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  memcpy(pkt->data,(uint8_t*)assoc_request,sizeof(MLME_Associate_Request_t));
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMlme, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_disassoc_req(MLME_Disassociate_Request_t* pDisassociateRequest)
 * @brief       Sends assoc Request
 * @param[in]   MLME_Disassociate_Request_t* pDisassociateRequest
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api is used is used by an associated device to notify the coordinator of its intent
 * to leave the PAN. It is also used by the coordinator to instruct an associated
 * device to leave the PAN. 
 */
int16_t rsi_zigb_mac_disassoc_req(MLME_Disassociate_Request_t* pDisassociateRequest)
{
  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  memcpy(pkt->data,(uint8_t*)pDisassociateRequest,sizeof(MLME_Disassociate_Request_t));
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMlme, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_poll_req(MLME_Poll_Request_t* pPollRequest)
 * @brief       Sends poll Request
 * @param[in]   MLME_Poll_Request_t* pPollRequest
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api is used to prompts the device to request data from the coordinator.
 */
int16_t rsi_zigb_mac_poll_req(MLME_Poll_Request_t* pPollRequest)
{
  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  memcpy(pkt->data,(uint8_t*)pPollRequest,sizeof(MLME_Poll_Request_t));
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMlme, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_rxenable_req(MLME_RX_Enable_Request_t* pRxenableRequest)
 * @brief       Sends Rx Enable Request
 * @param[in]   MLME_RX_Enable_Request_t* pRxenableRequest
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api allows the next higher layer to request that the receiver is enable
 * for a finite period of time. 
 */
int16_t rsi_zigb_mac_rxenable_req(MLME_RX_Enable_Request_t* pRxenableRequest)
{
  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  memcpy(pkt->data,(uint8_t*)pRxenableRequest,sizeof(MLME_RX_Enable_Request_t));
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMlme, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_scan_req(MLME_Scan_Request_t* pScanRequest)
 * @brief       Sends Scan Request
 * @param[in]   MLME_Scan_Request_t* pScanRequest
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api is used to is used to initiate a channel scan over a given list of channels.
 * A device can use a channel scan to measure the energy on the channel, search for
 * the coordinator with which it associated, or search for all coordinators transmitting
 * beacon frames within the POS of the scanning device.
 */
int16_t rsi_zigb_mac_scan_req(MLME_Scan_Request_t* pScanRequest)
{
  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  memcpy(pkt->data,(uint8_t*)pScanRequest,sizeof(MLME_Scan_Request_t));
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMlme, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_start_req(MLME_Start_Request_t* pStartRequest)
 * @brief       Sends Start Request
 * @param[in]   MLME_Start_Request_t* pStartRequest
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api makes a request for the device to start using a new superframe configuration. 
 */

int16_t rsi_zigb_mac_start_req(MLME_Start_Request_t* pStartRequest)
{
  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  memcpy(pkt->data,(uint8_t*)pStartRequest,sizeof(MLME_Start_Request_t));
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMlme, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_orphan_response(MLME_Orphan_Response_t* pOrphan_response)
 * @brief       Sends Orphan Response
 * @param[in]   MLME_Orphan_Response_t* pOrphan_response
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api allows the next higher layer of a coordinator to respond to the
 * MLME-ORPHAN.indication primitive. 
 */
int16_t rsi_zigb_mac_orphan_response(MLME_Orphan_Response_t* pOrphan_response)
{

  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  memcpy(pkt->data,(uint8_t*)pOrphan_response,sizeof(MLME_Orphan_Response_t));
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMlme, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}
/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_mlmepib_set( uint8_t pibAttributeId, uint8_t pibAttributeIndex,
 *              uint8_t* pPibAttributeValue )
 * @brief       Set MAC PIB Request
 * @param[in]   MLME_Start_Request_t* pStartRequest
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api  attempts to write the given value to the indicated MAC PIB attribute.
 */

int16_t rsi_zigb_mac_mlmepib_set( uint8_t pibAttributeId, uint8_t pibAttributeIndex,
        uint8_t* pPibAttributeValue )
{
  rsi_pkt_t *pkt;
  setMacPibFrameSnd *set_macpib;

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

  set_macpib = (setMacPibFrameSnd*)pkt->data;
  set_macpib->attr_id    = pibAttributeId;
  set_macpib->attr_index = pibAttributeIndex;
  memcpy(set_macpib->attr_value,pPibAttributeValue,rsi_zigb_get_pib_len(pibAttributeId));

  //! send command
  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetMACPib, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_mlmepib_get( uint8_t pibAttributeId, uint8_t pibAttributeIndex,
 *              uint8_t* pPibAttributeValue )
 * @brief       Get MAC PIB Request
 * @param[in]   pibAttributeId
 * @param[in]   pibAttributeIndex
 * @param[out]  pPibAttributeValue
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api is generated by the next higher layer and issued to its MLME to obtain
 * information from the MAC PIB .
 */

int16_t rsi_zigb_mac_mlmepib_get( uint8_t pibAttributeId, uint8_t pibAttributeIndex, uint8_t* pPibAttributeValue )
{
  rsi_pkt_t *pkt;
  getMacPibFrameSnd *get_macpib;
  int16_t status = 0; 

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

  get_macpib = (getMacPibFrameSnd*)pkt->data;
  get_macpib->attr_id    = pibAttributeId;
  get_macpib->attr_index = pibAttributeIndex;

  //! send command
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetMACPib, pkt);

  status = zigb_cb->resp.uCmdRspPayLoad.uRspData[0];
  memcpy(pPibAttributeValue,(uint8_t*)&zigb_cb->resp.uCmdRspPayLoad.uRspData[1],rsi_zigb_get_pib_len(pibAttributeId));
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 
}

/*==============================================*/
/**
 * @fn          int16_t rsi_zigb_mac_assoc_response(MLME_Associate_Response_t* pMLME_Associate_Response)
 * @brief       Sends assoc response
 * @param[in]   MLME_Associate_Response_t* pMLME_Associate_Response: Assoc response structure
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api is used to initiate a response to an MLMEASSOCIATE indication primitive. 
 */
int16_t rsi_zigb_mac_assoc_response(MLME_Associate_Response_t* pMLME_Associate_Response)
{

  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  memcpy(pkt->data,(uint8_t*)pMLME_Associate_Response,sizeof(MLME_Associate_Response_t));
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameEnqueueMacMlme, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

/*==============================================*/
/**
 * @fn          uint8_t rsi_zigb_get_pib_len(uint8_t pib_id)
 * @brief       get pid length based on pib_id
 * @param[in]   uint8_t pib_id
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *               0  = SUCCESS
 *
 * @section description
 * This api is used to get the pib length based on pib_id.
 */

uint8_t rsi_zigb_get_pib_len(uint8_t pib_id)
{
  uint8_t i=0;

  for(i=0 ; i<14; i++ )
  {
    if(pib_id == ga_PIB_Look_Up_Table_c[i][0])
    {
      return ga_PIB_Look_Up_Table_c[i][1];
    }
  }
  return g_EXTENDED_ADDRESS_LENGTH_c;
}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_init_mac(void)
 * @brief       Sends the ZigBee MAC init command to the ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to init the ZigBee MAC. 
 *
 */
int16_t rsi_zigb_init_mac(void)
{
  return rsi_zigb_init_stack();
}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_reset_mac(void)
 * @brief       Sends the ZigBee MAC reset command to the ZigBee module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = SUCCESS
 * @section description 
 * This API is used to soft reset the ZigBee MAC. 
 *
 */
int16_t rsi_zigb_reset_mac(void)
{
  return rsi_zigb_reset_stack();
}

/*===========================================================================
 *
 * @fn          int16_t rsi_zigb_get_cca_status(void)
 * @brief       Read back the cca status from the 802.15.4 module
 * @param[in]   none
 * @param[out]  none
 * @return      errCode
 *              -2 = Command execution failure
 *              -1 = Buffer Full
 *              0  = CHANNEL FREE
 *              1  = CHANNEY BUSY
 * @section description 
 * This API is used to read back the cca status.. 
 *
 */
int16_t rsi_zigb_get_cca_status(void)
{
  rsi_pkt_t *pkt = NULL;
  int32_t   status = RSI_SUCCESS;
  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;
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
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetCcaStatus, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

#endif
