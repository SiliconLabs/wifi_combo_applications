
#include "rsi_driver.h"
#include "rsi_zb_api.h"

/* Search address */ 
const uint8_t   rsi_zigb_frameSearchAddr[RSI_ZIGB_BYTES_2] = {SECURITY_INTERFACE, ZIGBEESEARCHADDR};


/* Get Group table Index   */ 
const uint8_t   rsi_zigb_frameGetGrpTableIndx[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETGRPTABLEINDX};

/* Get APSME_REMOVE GROUP */ 
const uint8_t   rsi_zigb_frameApsmeAddGrp[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEAPSADDGRP};

/* Rem all grp */ 
const uint8_t   rsi_zigb_frameApsmeRemAllGrp[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEREMALLGRP};

/* Get Grp Id  */ 
const uint8_t   rsi_zigb_frameGetGrpId[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETGRPID};

/* Verfu grp & Endpoint   */ 
const uint8_t   rsi_zigb_frameVerfyGrpEP[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEVERFYGRPEP};

/* Get Nwk Max Depth */ 
const uint8_t   rsi_zigb_frameGetNwkMaxDepth[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETNWKMAXDEPTH};

/*  */ 
const uint8_t   rsi_zigb_frameGetNwkSecurityLvl[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETSECURITYLVL};


/* Get short panid   */ 
const uint8_t   rsi_zigb_frameGetTCShortAddr[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETTCSHORTADDR};

/* Get short panid   */ 
const uint8_t   rsi_zigb_frameGetMaxAPSLen[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETMAXAPSLEN};

/* Get TC Link Key   */ 
const uint8_t   rsi_zigb_frameGetTCLinkKey[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETTCLINKKEY};
/* Set TC Link Key   */ 
const uint8_t   rsi_zigb_frameSetTCLinkKey[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEESETTCLINKKEY};

/* ZDO Request   */ 
const uint8_t   rsi_zigb_frameZdoAppZdpReq[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEZDOAPPZDPREQ};

/* Nwk Send Request   */ 
const uint8_t   rsi_zigb_frameNwkSendReq[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEENWKSENDREQ};

/* ZDO Silent start Request   */ 
const uint8_t   rsi_zigb_frameZdoSilentStartReq[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEZDOSILENTSTARTREQ};

/* Get Set update Id request */ 
const uint8_t   rsi_zigb_frameGetSetUpdateId[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETSETUPDATEID};

/* HAL Set Zll params   */ 
const uint8_t   rsi_zigb_frameSetZllParams[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEESETZLLPARAMS};

/* AES Encrypt Request   */ 
const uint8_t   rsi_zigb_frameAesEncrypt[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEAESENCRYPT};

/* Get NWK Key Request   */ 
const uint8_t   rsi_zigb_frameGetNwkKey[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEGETNWKKEY};

/* Set MAC PIB Request   */ 
const uint8_t   rsi_zigb_frameSetMacPib[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEESETMACPIB};

/* Update ZDO SAS */ 
const uint8_t   rsi_zigb_frameZDOUpdateSAS[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEZDOUPDATESAS};

/* Read Default SAS */ 
const uint8_t   rsi_zigb_framereadSAS[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEREADSAS};

/* APSDE Request   */ 
const uint8_t   rsi_zigb_frameApsdeReq[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEAPSDEREQ};

/* APS_Asdu_handler  */
const uint8_t   rsi_zigb_framesethandler[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEAPSASDUHANDLER};

/* Enable_Profile_Test  */
const uint8_t   rsi_zigb_framesetprofiletest[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEEPROFILETESTENABLE};

/* Set_Stack_Event  */
const uint8_t   rsi_zigb_framesetstkevent[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEESETSTACKEVENT};

/* Clear_Stack_Ecent  */
const uint8_t   rsi_zigb_frameclrstkevent[RSI_ZIGB_BYTES_2] = {MANAGEMENT_INTERFACE, ZIGBEECLRSTACKEVENT};

uint8_t rsi_zigb_set_stack_event(void)
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
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framesetstkevent, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}

uint8_t rsi_zigb_clear_stack_event(void)
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
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameclrstkevent, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}

uint8_t rsi_zigb_enable_profile_test(uint8_t profile_test)
{

  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  setprofiletestFrameSnd *profile=NULL;
   

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
  
  profile = (setprofiletestFrameSnd*)pkt->data;
  profile->profile_test = profile_test;
  
  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framesetprofiletest, pkt);
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}
uint8_t rsi_zigb_aps_asdu_handler(uint8_t handlerid)
{

  rsi_pkt_t *pkt;
  int16_t status = RSI_SUCCESS; 
  sethandlerFrameSnd *handler=NULL;
   

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
  
  handler = (sethandlerFrameSnd*)pkt->data;
  handler->handlerId = handlerid;
  
  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framesethandler, pkt);
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}

uint8_t APS_APSMEsearchAddress(uint16_t* pShortAddr, uint8_t* pLongAddr,
        uint8_t mode)
{

  rsi_pkt_t *pkt;
  uint8_t status = RSI_SUCCESS; 
  setSearchApsmeAddr *ApsmeAddr=NULL;


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

  ApsmeAddr = (setSearchApsmeAddr*)pkt->data;
  ApsmeAddr->mode= mode;
  if (g_SEARCH_BY_SHORT_ADDRESS_c == mode) {

    rsi_zigb_uint16to_buffer((*pShortAddr),(uint8_t *)&ApsmeAddr->address);
  }
  else
  {
    memUtils_memCopy((uint8_t*)&ApsmeAddr->address,pLongAddr,8);
  }
  //! send  command
  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSearchAddr, pkt);

  status= zigb_cb->resp.uCmdRspPayLoad.uRspData[0];

  if (g_SEARCH_BY_SHORT_ADDRESS_c == mode) {

    memcpy(pLongAddr, &zigb_cb->resp.uCmdRspPayLoad.uRspData[1], 8);
  }
  else
  {
    *pShortAddr = (uint16_t*)zigb_cb->resp.uCmdRspPayLoad.uRspData[1];
  }

    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}

uint8_t APS_getGroupTableIndex(uint16_t groupAddress)
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
  rsi_zigb_uint16to_buffer(groupAddress, (uint8_t *)&get_ieee_for_short->ShortAddr);

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetGrpTableIndx, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

uint8_t APS_APSMEaddGroup(Add_Group_Request_t* pAddGroupEntry)
{
  uint8_t i=0;
  rsi_pkt_t *pkt;
  Add_Group_Request_t* pAddGroupEntry_temp = NULL;
   
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
    pAddGroupEntry_temp = (Remove_Group_Request_t*)pkt->data;
    pAddGroupEntry_temp->group_address =  pAddGroupEntry->group_address;
    pAddGroupEntry_temp->endpoint      =  pAddGroupEntry->endpoint;
  
   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameApsmeAddGrp, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

uint8_t APS_APSMEremoveAllGroups(uint8_t endpoint)
{

  rsi_pkt_t *pkt;
  uint8_t status = RSI_SUCCESS; 
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
  profile->ProfileId = endpoint;
  
  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameApsmeRemAllGrp, pkt);
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 

}

uint16_t APS_getGroupId(uint8_t index)
{
  rsi_pkt_t *pkt;
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
  profile->ProfileId = index;
  
  //! send  command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetGrpId, pkt);
  
    //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevPanId.pan_id; 

}


uint8_t APS_Verify_GroupAddrAndEndPoint(uint16_t groupAddress, uint8_t endPoint)
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
  rsi_zigb_uint16to_buffer(groupAddress, (uint8_t *)&get_simple_desc->ShortAddr);

  get_simple_desc->EndPointId = (uint8_t)endPoint;

   //! send command
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameVerfyGrpEP, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

uint8_t Get_Nwk_MaxDepth(void)      
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
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetNwkMaxDepth, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}

uint8_t Get_Nwk_SecurityLevel(void)
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
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetNwkSecurityLvl, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}

int16_t Zigb_Get_TCShortAddr(void)
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
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetTCShortAddr, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevPanId.pan_id; 

}

uint8_t Zigb_GetMaxAPSPayloadLength(void )
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
   rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetMaxAPSLen, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevPanId.pan_id; 

}

uint8_t GetTrustCenterLinkKey (TrustCenterLinkKeyTable_t *LinkKeyTable,uint8_t index)
{

  rsi_pkt_t *pkt;
  getKeyFrameSnd *get_key=NULL;
  int32_t   status = RSI_SUCCESS;

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
  get_key->KeyType = index;

   //! send command
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetTCLinkKey, pkt);
  status = zigb_cb->resp.uCmdRspPayLoad.uRspData[0];
  
  memUtils_memCopy(LinkKeyTable->LinkKeyDevicePair.deviceAddress,zigb_cb->resp.uCmdRspPayLoad.uRspData[1], 8);
  memUtils_memCopy(LinkKeyTable->LinkKeyDevicePair.linkKey,zigb_cb->resp.uCmdRspPayLoad.uRspData[9], 16 );
  memUtils_memCopy(LinkKeyTable->LinkKeyFrameCounter.outgoingFrameCounter,zigb_cb->resp.uCmdRspPayLoad.uRspData[25], 4);
  memUtils_memCopy(LinkKeyTable->LinkKeyFrameCounter.incomingFrameCounter,zigb_cb->resp.uCmdRspPayLoad.uRspData[29],4);
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  status; 
}

uint8_t SetTrustCenterLinkKey (TrustCenterLinkKeyTable_t LinkKeyTable,uint8_t index)
{

  rsi_pkt_t *pkt;
  setTCLinkKeySnd *set_key=NULL;
  int32_t   status = RSI_SUCCESS;

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
  
  set_key = (setTCLinkKeySnd*)pkt->data;
  memUtils_memCopy(set_key->LinkKeyTable.LinkKeyDevicePair.deviceAddress,LinkKeyTable.LinkKeyDevicePair.deviceAddress, 8);
  memUtils_memCopy(set_key->LinkKeyTable.LinkKeyDevicePair.linkKey,LinkKeyTable.LinkKeyDevicePair.linkKey, 16 );
  memUtils_memCopy(set_key->LinkKeyTable.LinkKeyFrameCounter.outgoingFrameCounter,LinkKeyTable.LinkKeyFrameCounter.outgoingFrameCounter, 4);
  memUtils_memCopy(set_key->LinkKeyTable.LinkKeyFrameCounter.incomingFrameCounter,LinkKeyTable.LinkKeyFrameCounter.incomingFrameCounter,4);
  set_key->index = index;

   //! send command
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetTCLinkKey, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

uint8_t ZDO_AppZDPrequest(uint8_t messageIndex)
{
   rsi_pkt_t *pkt = NULL;
   uint8_t *p_buffer = NULL;
   int32_t   status = RSI_SUCCESS;

   p_buffer =buffMgmt_getBufferPointer(messageIndex);

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
  
  memUtils_memCopy(pkt->data,p_buffer,RSI_ZIGB_LARGE_BUFFER_SIZE);
  buffMgmt_freeBuffer(messageIndex, 224 );
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameZdoAppZdpReq, pkt);
  
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

uint8_t APS_APSDErequest(uint8_t messageIndex)
{
   rsi_pkt_t *pkt = NULL;
   uint8_t *p_buffer = NULL;
   int32_t   status = RSI_SUCCESS;

   p_buffer =buffMgmt_getBufferPointer(messageIndex);

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
  
  memUtils_memCopy(pkt->data,p_buffer,RSI_ZIGB_LARGE_BUFFER_SIZE);
  buffMgmt_freeBuffer(messageIndex, 220 );
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameApsdeReq, pkt);
  
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}
void NWK_sendRequest(uint8_t messageIndex)
{
   rsi_pkt_t *pkt = NULL;
   uint8_t *p_buffer = NULL;
   int32_t   status = RSI_SUCCESS;

   p_buffer =buffMgmt_getBufferPointer(messageIndex);

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
  
  memUtils_memCopy(pkt->data,p_buffer,RSI_ZIGB_LARGE_BUFFER_SIZE);
  buffMgmt_freeBuffer(messageIndex, 225 );
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameNwkSendReq, pkt);
  
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}

void ZDO_handleSilentStartRequest(uint8_t *silentStartupParam,uint8_t messageIndex)
{
   rsi_pkt_t *pkt = NULL;
   uint8_t *p_buffer = NULL;
   int32_t   status = RSI_SUCCESS;

  buffMgmt_freeBuffer(messageIndex, 225 );
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
  
  memUtils_memCopy(pkt->data,silentStartupParam,sizeof(Silent_Startup_Param_t));

  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameZdoSilentStartReq, pkt);
  
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 


}

void getSetUpdateId(bool opType, uint8_t *updateId)
{
   rsi_pkt_t *pkt = NULL;
   uint8_t *p_buffer = NULL;
   int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  pkt->data[0] = opType;
  pkt->data[1] = *updateId;

  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetSetUpdateId, pkt);
 
  *updateId = zigb_cb->resp.uCmdRspPayLoad.uRspData[0];
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return ; 


}


uint8_t Encrypt_Decrypt_Using_ECB_Mode(  uint8_t *pPldLen, uint8_t *pPld, uint8_t *pEncryptedPld, bool encrypt )
{
   rsi_pkt_t *pkt = NULL;
   int32_t   status = RSI_SUCCESS;


  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);
  
  pkt->data[0] = *pPldLen;
  memUtils_memCopy(pkt->data[1],pPld,*pPldLen);
  pkt->data[(*pPldLen) + 1] = encrypt;
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameAesEncrypt, pkt);
  
  memUtils_memCopy(pEncryptedPld,zigb_cb->resp.uCmdRspPayLoad.uRspData , *pPldLen);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

uint8_t GetNetworkKey(uint8_t * ptrNetworkKey)
{
   rsi_pkt_t *pkt = NULL;
   int32_t   status = RSI_SUCCESS;


  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);
  
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameGetNwkKey, pkt);
  
  memUtils_memCopy(ptrNetworkKey,zigb_cb->resp.uCmdRspPayLoad.uRspData[1] , 16);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.uRspData[0]; 
}

uint8_t MAC_MLMEPibSet ( uint8_t pibAttributeId, uint8_t pibAttributeIndex,uint8_t* pPibAttributeValue)
{
   rsi_pkt_t *pkt = NULL;
   int32_t   status = RSI_SUCCESS;


  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);
  
  pkt->data[0] = pibAttributeId;
  pkt->data[1] = pibAttributeIndex;
  memUtils_memCopy(&pkt->data[2],pPibAttributeValue,2);
  
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetMacPib, pkt);
  
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}


/*===========================================================================
 *
 * @fn          int16_t ZDOupdateSAS(Startup_Attribute_Set_t *Startup,uint8_t index)
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
int16_t ZDOupdateSAS(Startup_Attribute_Set_t *Startup,uint8_t index)
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
  pkt->data[0] = index;
  Startup_temp = (Startup_Attribute_Set_t*)&pkt->data[1];
  //  rsi_zigb_mcpy((uint8_t*)&Startup,(uint8_t*)&Startup_temp,sizeof(Startup_Attribute_Set_t));
  memcpy((uint8_t*)&Startup_temp->a_extended_pan_id,(uint8_t*)&Startup->a_extended_pan_id, 8);
  Startup_temp->channel_mask            = Startup->channel_mask;
  Startup_temp->protocol_version        = Startup->protocol_version;
  Startup_temp->stack_profile           = Startup->stack_profile;
  Startup_temp->startup_control         = Startup->startup_control;
  memcpy((uint8_t*)&Startup_temp->a_trust_center_address,(uint8_t*)&Startup->a_trust_center_address,8);
  memcpy((uint8_t*)&Startup_temp->a_network_key,(uint8_t*)&Startup->a_network_key,16);
  Startup_temp->use_insecure_join       = Startup->use_insecure_join;
  Startup_temp->network_key_seq_num     = Startup->network_key_seq_num;
  Startup_temp->network_key_type        = Startup->network_key_type;
  Startup_temp->network_manager_address = Startup->network_manager_address;
  Startup_temp->scan_attempts           = Startup->scan_attempts;
  Startup_temp->time_between_scans      = Startup->time_between_scans;
  Startup_temp->rejoin_interval         = Startup->rejoin_interval;
  Startup_temp->max_rejoin_interval     = Startup->max_rejoin_interval;
  Startup_temp->indirect_poll_rate      = Startup->indirect_poll_rate;
  Startup_temp->parent_retry_threshold  = Startup->parent_retry_threshold;
  Startup_temp->security_timeout_period = Startup->security_timeout_period;
  Startup_temp->APS_ack_wait_duration   = Startup->APS_ack_wait_duration;
  Startup_temp->a_short_address         = Startup->a_short_address;
  Startup_temp->a_pan_id                = Startup->a_pan_id;
  memcpy((uint8_t*)&Startup_temp->a_trustcenter_master_key,(uint8_t*)&Startup->a_trustcenter_master_key,16);
  memcpy((uint8_t*)&Startup_temp->a_preconfigured_link_key,(uint8_t*)&Startup->a_preconfigured_link_key,16);
  Startup_temp->concentrator_flag       = Startup->concentrator_flag;
  Startup_temp->concentrator_radius     = Startup->concentrator_radius;
  Startup_temp->concentrator_discovery_time    = Startup->concentrator_discovery_time;
  Startup_temp->deviceAnnceReq          = Startup->deviceAnnceReq;
  Startup_temp->end_device_bind_timeout = Startup->end_device_bind_timeout;
  //! send command


  rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameZDOUpdateSAS, pkt);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 

}

void ZDOreadNVMdefaultSAS(uint8_t *ptr)
{
   rsi_pkt_t *pkt = NULL;
   uint8_t *p_buffer = NULL;
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
  
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_framereadSAS, pkt);
  
  memUtils_memCopy(ptr, (uint8_t*)zigb_cb->resp.uCmdRspPayLoad.uRspData,m_SIZE_CONFIGURATION_c);

  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

int16_t rsi_zigb_handle_zll_set_commands(uint8_t id,uint8_t *data,uint8_t len)
{
   rsi_pkt_t *pkt = NULL;
   int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  pkt->data[0] = id;
  if(data != NULL)
  {
    memUtils_memCopy((uint8_t*)&pkt->data[1],data,len);
  }
  
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetZllParams, pkt);
  
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.statusResp.status; 
}

uint8_t rsi_zigb_handle_zll_get_commands(uint8_t id)
{
   rsi_pkt_t *pkt = NULL;
   int32_t   status = RSI_SUCCESS;

  //! Get zigb cb structure pointer
  rsi_zigb_cb_t *zigb_cb = rsi_driver_cb->zigb_cb;

  //! take lock on zigb control block
  rsi_mutex_lock(&zigb_cb->zigb_mutex);

  //! allocate command buffer  from zigb pool
  pkt = rsi_pkt_alloc(&zigb_cb->zigb_tx_pool);

  pkt->data[0] = id;
  
  status = rsi_zigb_send_cmd((uint8_t *)rsi_zigb_frameSetZllParams, pkt);
  
  //! free the packet
  rsi_pkt_free(&zigb_cb->zigb_tx_pool, pkt);

  //! unlock mutex
  rsi_mutex_unlock(&zigb_cb->zigb_mutex);

  //! Return the status
  return  zigb_cb->resp.uCmdRspPayLoad.DevChannel.channel; 
}

void ZDOactivateSASindex(uint8_t index)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETSASINDEX,&index,1);
}

void ZDO_setExtendedAddress(uint8_t *address)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETEXTADDR,address,8);
}

void rsi_802154_hal_set_aes_key(uint8 *key)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETAESKEY,key,RSI_ZIGB_FRAME_DESC_LEN);
}
uint8_t ZigBeeSetExtendedPANId(uint8_t *pExtPanId)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETEXTPANID,pExtPanId,8);
}

uint8_t ZigBeeSetTrustCenterAddress(uint8_t *pTCAddress)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETTCADDR,pTCAddress,8);
}
#if 1
int16_t ZigBeeSetSelfShortAddress_t(uint16_t short_addr)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETSELFSHORTADDR,(uint8_t*)&short_addr,2);
}

int16_t ZigBeeSetShortPANId_t(uint16_t short_panid)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETSHORTPANID,(uint8_t*)&short_panid,2);
}

void Set_RX_On_When_Idle(void)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETRXONWHENIDLE,NULL,NULL);
}

void MAC_SetPhyTRXState(uint8_t trx_state)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETPHYTRXSTATE,&trx_state,1);
}

uint8_t SetNetworkKey(uint8_t *ptrNetworkKey)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEESETNWKKEY,ptrNetworkKey,RSI_ZIGB_FRAME_DESC_LEN);
}

void rsi_zigb_zll_set_oper_channel(uint8_t channel)
{
  rsi_zigb_handle_zll_set_commands(ZIGBEEZLLSETOPERCHANNEL,(uint8_t *)&channel,1);
}

void rsi_zigb_zll_get_oper_channel(void)
{
  rsi_zigb_handle_zll_get_commands(ZIGBEEZLLGETOPERCHANNEL);
}



void ZLL_SaveEssentialParams(uint8_t *pNVM_params , uint8_t size)
{
  rsi_zigb_app_info_t   *app_info = &rsi_zigb_app_info;
  if((pNVM_params != g_NULL_c )&& (size != 0x00)){
    size = (size %2) ? (size + 1 ) : size;
    memUtils_memCopy((uint8_t*)&app_info->nvm_storage.ZLL_NVM_Parameters,pNVM_params,size);
  }
}
/*******************************************************************************/
void ZLL_RestoreEssentialParams(uint8_t *pNVM_params , uint8_t size)
{
  rsi_zigb_app_info_t   *app_info = &rsi_zigb_app_info;
  if((pNVM_params != g_NULL_c) && (size != 0x00)){
      size = (size %2) ? (size + 1 ) : size;
    memUtils_memCopy(pNVM_params,(uint8_t*)&app_info->nvm_storage.ZLL_NVM_Parameters,size);
  }
}


#endif
