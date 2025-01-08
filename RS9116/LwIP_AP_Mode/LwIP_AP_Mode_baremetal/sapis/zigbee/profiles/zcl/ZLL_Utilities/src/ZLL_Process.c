/******************************************************************************
* ZLL_Interpan.c
*******************************************************************************
* Program Description:
*   ZLL Commissioning Supports Two types of commands:
*   a.Touchlink cmds : Transmission and Reception happens through Inter-pan method
*   b.Utility cmds : Transmissiong and Reception happens through normal
*                ZigBee method.
*
*
******************************************************************************/

/*****************************************************************************
* Includes
*****************************************************************************/

#include "ZigBeeDataTypes.h"
#include <stdbool.h>

//#ifndef	STM32L1XX_MD_PLUS
//#include PLATFORM_HEADER
//#include "hal/hal.h"
//#include "phy-library.h"
//#include "stm32l1xx.h"
//#else
 // #include "simplemac_library.h"
//#endif /*STM32L1XX_MD_PLUS */

//#include "stm32l1w_discovery.h"
#include "buffer_management.h"
#include "stack_common.h"
#include "ZLL_defines.h"
#include "ZLL_Interpan.h"
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "apsme_interface.h"
#include "common_includes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
//#include "timer.h"
#include"sw_timer.h"
#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1 )

#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
    #include "aps_interpan_stub.h"
#include "apsde_interface.h"
#endif /* g_INTERPAN_DATA_ENABLE_d == 1*/

#include "ZLL_Commissioning_Cluster.h"
#include "AppZllCommissioningClusterUtilities.h"
#include "ZLL_Profile_Attributes.h"
#include "ZLL_Main.h"
#include "ZLL_Interpan.h"
#include "ZLL_Network.h"
#include "ZLL_State_Machine.h"
#include "ZLL_Configuration.h"
#include "ZLL_Process.h"
#include "ZLL_NVM_Utilities.h"
#endif /* g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1*/


#include "ApptimerId.h"
//#include "timer.h"
#include "ApplicationThinLayer.h"


/*****************************************************************************/
uint8_t linkInitiator = g_FALSE_c;
uint8_t LogicalType = g_FALSE_c;
uint8_t  ZLL_Process_ScanCommands(uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
  uint8_t command = pApsdeDataInd->a_asdu[2] ;



//#ifdef g_ZLL_LIGHT_DEVICE_c
    Scan_request_record_list_field *pScanRequest =
          (Scan_request_record_list_field*)&(pApsdeDataInd->a_asdu[3]);
  /*Check if the command is Scan request or Scan response command*/
  if( command == g_ZLL_Scan_Req_Command_c )
  {
    /*ZLLTODO: Check if the RSSI value for Manufacture specific threshold
      is meeting the requirements if not, do not send response*/
         /*create scan response*/
      {
          /*if same transaction id is received from the initiator, then
        ignore the scan request and do no send scan response*/
          if( pScanRequest->inter_pan_transaction_identifier ==
              pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier )
          {
            return g_ZCL_Success_c;
          }
          if( pApsdeDataInd->srcaddrmode == g_IEEE_Address_Mode_c )
          {
            linkInitiator = pScanRequest->ZLL_information_t.link_initiator;
            pScanRequest->ZigBee_information_t.logical_type = g_END_DEVICE_c;
            if( ( pScanRequest->ZLL_information_t.link_initiator == g_TRUE_c ) &&
                ( pScanRequest->ZigBee_information_t.logical_type == g_END_DEVICE_c )  )
            {
              pTouchLinkReceiverSectionMemory->initiator_provided_network_parameters = g_TRUE_c;
              pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier = pScanRequest->inter_pan_transaction_identifier;
              if( pApsdeDataInd->srcaddrmode == g_IEEE_Address_Mode_c )

              /*copy the target IEEE address*/
              memUtils_memCopy(pTouchLinkReceiverSectionMemory->initiator_long_address,
                               pApsdeDataInd->srcaddress.IEEE_address,g_EXTENDED_ADDRESS_LENGTH_c);


            }
            ZLL_Create_Scan_Response_Command(
                 pApsdeDataInd->srcaddress.IEEE_address ,
                 pScanRequest->inter_pan_transaction_identifier,
                 pScanRequest->ZLL_information_t.link_initiator
                   );

          }
      }
  }
//#endif //#ifdef g_ZLL_LIGHT_DEVICE_c

#ifdef g_ZLL_LIGHT_CONTROLLER_c
  if ( command == g_ZLL_Scan_Response_Command_c)
  {
    /*excluding the ZCL header and starting from the ZCL payload*/
    Scan_response_field *pScanResponse = (Scan_response_field*)(&pApsdeDataInd->a_asdu[3]);
    /*validate scan response command and move to device information state*/
    if( pTouchLinkInitiatiorSectionMemory->interPAN_transaction_identifier ==
        pScanResponse->inter_pan_transaction_identifier )
    {
      ZLL_AddScanResponseEntry(pScanResponse, pApsdeDataInd->srcaddress.IEEE_address );
      /*if priority is set in the scan response command, set the device as the Target address
      for Touch link procedure*/
      if( pScanResponse->ZLL_information_t.touch_link_priority_request ==  g_ZLL_REQUEST_PRIORITY_c )
      {
        if( pApsdeDataInd->srcaddrmode == g_IEEE_Address_Mode_c )
        {
          /*copy the target IEEE address*/
          memUtils_memCopy(pTouchLinkInitiatiorSectionMemory->Target_long_address,
                           pApsdeDataInd->srcaddress.IEEE_address,g_EXTENDED_ADDRESS_LENGTH_c);
        }
        ZLL_Change_Controller_State(g_I_DEVICE_IDENTIFY_REQUEST_STATE_c);
        return g_ZCL_Success_c;
      }
      /*wait for completing scan*/
      ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);
    }
    else
      return g_ZCL_Not_Authorized_c;

  }
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c
  return g_ZCL_Success_c;
}
/*****************************************************************************/
uint8_t  ZLL_Process_DeviceInformationCommands( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{

  uint8_t command = pApsdeDataInd->a_asdu[2] ;
  /*Check if the command is Device information request or Device information response command*/
#ifdef g_ZLL_LIGHT_DEVICE_c
  if( command == g_ZLL_Device_Information_Req_Command_c )
  {
    Device_information_record_list_field *pDevInformationRequest =
      (Device_information_record_list_field*)(&pApsdeDataInd->a_asdu[3]);
    if( pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier ==
        pDevInformationRequest->inter_pan_transaction_identifier )
    {
      if( pApsdeDataInd->srcaddrmode == g_IEEE_Address_Mode_c )
      {
        /*copy the target IEEE address*/
        memUtils_memCopy(pTouchLinkReceiverSectionMemory->initiator_long_address,
                         pApsdeDataInd->srcaddress.IEEE_address,g_EXTENDED_ADDRESS_LENGTH_c);
      }
      ZLL_Create_Device_Information_Response_Command( pTouchLinkReceiverSectionMemory->initiator_long_address,
                                                   pDevInformationRequest->start_index  );

    }
    else
      return g_ZCL_Not_Authorized_c;
  }
#endif //#ifdef g_ZLL_LIGHT_DEVICE_c
#ifdef g_ZLL_LIGHT_CONTROLLER_c
  if ( command == g_ZLL_Device_Information_Response_Command_c )
  {
    Device_information_response_field *pDevInformationResponse =
      (Device_information_response_field*)(&pApsdeDataInd->a_asdu[3]);
    if( pTouchLinkInitiatiorSectionMemory->interPAN_transaction_identifier ==
        pDevInformationResponse->inter_pan_transaction_identifier )
    {
      /*validate the device information response, and move to Network startup state*/
      ZLL_Change_Controller_State(g_I_DEVICE_NETWORK_START_STATE_c);
    }
    else
      return g_ZCL_Not_Authorized_c;

  }
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c
  return g_ZCL_Success_c;
}
/*****************************************************************************/
uint8_t  ZLL_Process_IdentifyRequestCommands(uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
#ifdef g_ZLL_LIGHT_DEVICE_c
  Identify_record_list_field *pIdentifyRequest =
    (Identify_record_list_field*)(&pApsdeDataInd->a_asdu[3]);;
  if( pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier ==
        pIdentifyRequest->inter_pan_transaction_identifier )
  {
    uint32_t identifyDuration = pIdentifyRequest->identify_duration;
    if (  pIdentifyRequest->identify_duration == 0xFFFF )
    {
        identifyDuration = g_ZLL_DEFAULT_IDENTIFY_TIME_c;
    }
    if( identifyDuration )
    {
      /*blink LED*/
      /*start a timer for identify durtaion and blink an LED*/
    //RP:
      //tmr_startRelativeTimer
      //(
      //  m_APP_ZLL_IDENTIFY_TIMER_c,
      //  identifyDuration * m_MILLISECONDS_c * m_MILLISECONDS_c,
      //  App_ZLL_IdentifyCallBack );
    }
    else
    {
      /*if the timer is running, stop the timer*/
      //if( g_TRUE_c == Is_Timer_Running( m_APP_ZLL_IDENTIFY_TIMER_c ))
      {
        /*stop blinking LED*/
        //RP:
        //tmr_stopTimer( m_APP_ZLL_IDENTIFY_TIMER_c );
      }
    }

      return g_ZCL_Success_c;
  }
  return g_ZCL_Not_Authorized_c;
#else
  return g_ZCL_Not_Authorized_c;
#endif //#ifdef g_ZLL_LIGHT_DEVICE_c
}
/*****************************************************************************/
void  ZLL_Process_ResetToFactory( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
  	/* 	1. check whether target is FN or non FN
  		2.check for inter pan transactionId
  		*/
	if(ZLL_IsFactoryNew() == g_FALSE_c){
  	Reset_to_factory_new_record_list_field *pResetRequest =
      	(Reset_to_factory_new_record_list_field*)&(pApsdeDataInd->a_asdu[3]);

    if( pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier ==
        pResetRequest ->inter_pan_transaction_identifier )
    {
      /* perform network leave*/
      //ZigBeeLeaveNetwork();
      rsi_zigb_leave_network();
    }
	}
}
/*****************************************************************************/

uint8_t  ZLL_Process_NetworkStartCommands( uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
  uint8_t command = pApsdeDataInd->a_asdu[2] ;
#ifdef g_ZLL_LIGHT_DEVICE_c
  uint8_t expandedOutput[16];
  uint8_t networkKey[16];
  uint8_t status;
#endif
#ifdef g_ZLL_LIGHT_DEVICE_c
  if( command == g_ZLL_Network_Start_Req_Command_c )
  {
      /*create random short address*/
      uint16_t node_address = randomT();
      uint8_t localExtendedPanId[8] = { 0x00 };
      Network_start_record_list_field *pNwkStartRequest =
      (Network_start_record_list_field*)&(pApsdeDataInd->a_asdu[3]);
      if( pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier ==
          pNwkStartRequest->inter_pan_transaction_identifier )
      {
        if( pNwkStartRequest->network_address )
        {
           /*configure the short address as given in the network start request*/
            node_address = pNwkStartRequest->network_address;
        }
        MAC_MLMEPibSet(g_MAC_SHORT_ADDRESS_c, g_INVALID_INDEX_c,
                               (uint8_t*) &(node_address));

        if( pNwkStartRequest->pan_identifier )
          pDeviceStartupSectionMemory->activeScan_PANIDS[0] = pNwkStartRequest->pan_identifier;
        else
          pDeviceStartupSectionMemory->activeScan_PANIDS[0] = randomT();;
        pDeviceStartupSectionMemory->recipient_device_network_address =
            pNwkStartRequest->network_address;
        /*copy the target extended pan id*/
        if (g_Memory_Matches_c == (memUtils_memCompare(
            localExtendedPanId,
            pNwkStartRequest->extended_pan_identifier,
            g_EXTENDED_PAN_ID_LENGTH_c)))
        {
          /*the start request is received with zero value*/
          uint8_t i = 0x00;
          for( i =0x00; i < g_EXTENDED_PAN_ID_LENGTH_c; i++ )
          {
            localExtendedPanId[i] = (uint8_t)randomT();
          }
          memUtils_memCopy((uint8_t*)&(pDeviceStartupSectionMemory->activeScan_extendedPANIDs[0]),
                           localExtendedPanId,g_EXTENDED_PAN_ID_LENGTH_c);
        }
        else
        {
          memUtils_memCopy((uint8_t*)&(pDeviceStartupSectionMemory->activeScan_extendedPANIDs[0]),
                           pNwkStartRequest->extended_pan_identifier,g_EXTENDED_PAN_ID_LENGTH_c);
        }
        /*set the network key*/
        if( pNwkStartRequest->key_index )
        {
          /*Expand the ouput by using the interpan transaction id and
          response id and expand it to 128 bit number, the expanded output will have
          encrypted transport key decrypted based on the key index*/
          ZLL_Expand_EncryptTransportKey( pNwkStartRequest->inter_pan_transaction_identifier,
                                         pTouchLinkReceiverSectionMemory->response_transcation_identifier,
                                         expandedOutput , pNwkStartRequest->key_index);

          /*encrypt the network key using the encrypted transport key*/
          uint8_t payload_length = g_ZLL_NETWORK_KEY_SIZE_c;
          status = Encrypt_Decrypt_Using_ECB_Mode(  &payload_length, pNwkStartRequest->encrypted_network_key,
                                                  networkKey, g_TRUE_c );
          /*Get the network key*/
          status = SetNetworkKey( networkKey );

        }
        /*set the network key*/
        pDeviceStartupSectionMemory->zll_channel = ZLL_NetworkGetchannel();
        ZLL_Change_LightDevice_State(g_T_INITIATE_ACTIVE_SCAN_REQ_c);

      }
    else
      return g_ZCL_Not_Authorized_c;

  }
#endif //#ifdef g_ZLL_LIGHT_DEVICE_c
#ifdef g_ZLL_LIGHT_CONTROLLER_c
  if( command == g_ZLL_Nwk_Start_Response_Command_c )
  {
    Network_start_response_field *pNwkStartResponse =
      (Network_start_response_field*)&(pApsdeDataInd->a_asdu[3]);
    if( pTouchLinkInitiatiorSectionMemory->interPAN_transaction_identifier ==
            pNwkStartResponse->inter_pan_transaction_identifier )
    {
       if( pNwkStartResponse->status == g_ZCL_Success_c )
       {

         tmr_stopTimer( m_APP_ZLL_INTERPAN_LIFETIME_TIMER_c );
         /*trigger rejoin command from the initiator*/
         ZLL_StartUpParameters();
         ZLL_aplcMinStartupDelaytime = Get_Current_Time();
         ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);

       }
    }
  }
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c
  return g_ZCL_Success_c;
}
/*****************************************************************************/

void ZLL_StartUpParameters(void)
{
    uint8_t aTrustCenterAddress[8] = { 0xFF };
    ZigBeeSetShortPANId(pTouchLinkInitiatiorSectionMemory->Short_PanId);
    //ZigBeeSetOperatingChannel(ZLL_NetworkGetchannel());
    rsi_zigb_set_operating_channel(ZLL_NetworkGetchannel());
    ZigBeeSetExtendedPANId( pTouchLinkInitiatiorSectionMemory->Extended_PANId );
    ZigBeeSetTrustCenterAddress(aTrustCenterAddress);

    ZigBeeSetSelfShortAddress(ZLL_DeviceNetworkAddress());

}

/*****************************************************************************/

uint8_t  ZLL_Process_NetworkJoinCommands(uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
  uint8_t command = pApsdeDataInd->a_asdu[2] ;

  if(( command == g_ZLL_Network_Join_Router_Req_Command_c)||
    (command == g_ZLL_Network_Join_End_Device_Req_Command_c ) )
  {
       Network_join_record_list_field *pNwkJoinRequest =
      (Network_join_record_list_field*)&(pApsdeDataInd->a_asdu[3]);
      if( pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier ==
          pNwkJoinRequest->inter_pan_transaction_identifier )
      {
        pDeviceStartupSectionMemory->activeScan_PANIDS[0] = pNwkJoinRequest->pan_identifier;
        pDeviceStartupSectionMemory->recipient_device_network_address =
            pNwkJoinRequest->network_address;

        /* Set the received short address by the initiator*/
        ZLL_SetDeviceNetworkAddress(pDeviceStartupSectionMemory->recipient_device_network_address);
        /*TO do set address range based on address assignment capability and
        requested address range*/

        /*copy the target IEEE address*/
        memUtils_memCopy((uint8_t*)&(pDeviceStartupSectionMemory->activeScan_extendedPANIDs[0]),
                         pNwkJoinRequest->extended_pan_identifier,g_EXTENDED_PAN_ID_LENGTH_c);
        /* Assuming Logical channel ,ZLL channel and radio channel are same,
        so we are not setting the channel*/
        pDeviceStartupSectionMemory->zll_channel = pNwkJoinRequest->logical_channel;
        ZLL_Create_Network_Join_Response(pApsdeDataInd->srcaddress.IEEE_address,
                                         command, g_ZLL_RESPONSE_STATUS_SUCCESS_c);
#ifdef g_ZLL_LIGHT_DEVICE_c
        ZLL_Change_LightDevice_State(g_T_NETWORK_START_STATE_c);
#endif /* #ifdef g_ZLL_LIGHT_DEVICE_c*/
#ifdef g_ZLL_LIGHT_CONTROLLER_c
        ZLL_aplcMinStartupDelaytime = Get_Current_Time();
        ZLL_Change_Controller_State(g_I_WAIT_FOR_RESPONSE_STATE_c);
        ZLL_StartUpParameters();
#endif /* #ifdef g_ZLL_LIGHT_CONTROLLER_c*/
      }
  }



#ifdef g_ZLL_LIGHT_CONTROLLER_c
  if( command == g_ZLL_Network_Join_Router_Response_Command_c )
  {
    Network_join_response_field *pNwkJoinRouterResponse =
      (Network_join_response_field*)&(pApsdeDataInd->a_asdu[3]);
      if( pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier ==
          pNwkJoinRouterResponse->inter_pan_transaction_identifier )
      {
        /*move back to ready state to process next scan response detail*/
        ZLL_Change_Controller_State(g_I_DEVICE_READY_STATE_c);
      }
  }
  else if( command == g_ZLL_Network_Join_End_Device_Response_Command_c )
  {
      Network_join_response_field *pNwkJoinEndDeviceResponse =
      (Network_join_response_field*)&(pApsdeDataInd->a_asdu[3]);
      if( pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier ==
          pNwkJoinEndDeviceResponse->inter_pan_transaction_identifier )
      {
        /*move back to ready state to process next scan response detail*/
        ZLL_Change_Controller_State(g_I_DEVICE_READY_STATE_c);
      }
  }
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c
  return g_ZCL_Success_c;
}

/*****************************************************************************/

uint8_t ZLL_Process_NetworkUpdateCommands(uint8_t buffer_index,
                                  APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
  uint8_t extended_pan_id[8] = {0};
  rsi_zigb_get_extended_panid(extended_pan_id);

  /*1. compare interpan transaction Id
    2. compare extended panid and short panid
    3. compare nwk update id */

    uint8_t command = pApsdeDataInd->a_asdu[2] ;

    /* check command id */
    if(command == g_ZLL_Network_Update_Req_Command_c ){


      Network_update_record_list_field *pNwkUpdateRequest =
            (Network_update_record_list_field*)&(pApsdeDataInd->a_asdu[3]);

      /* check transaction Id*/
      if( pTouchLinkReceiverSectionMemory->interPAN_transaction_identifier ==
         pNwkUpdateRequest->inter_pan_transaction_identifier ){

      /* check extpanid and short panid */
        if((g_Memory_Matches_c == (memUtils_memCompare(
                                pNwkUpdateRequest->extended_pan_identifier,
                                //gp_Persistent_NIB->NWK_extended_panid,
                                extended_pan_id,
                                g_EXTENDED_ADDRESS_LENGTH_c))) &&
           //( pNwkUpdateRequest->pan_identifier == gp_Persistent_NIB->NWK_pan_id
           ( pNwkUpdateRequest->pan_identifier == rsi_zigb_get_short_panid()
                                ) ){

            uint8_t nwkUpdateId = 0x00;
            getSetUpdateId(g_TRUE_c ,&nwkUpdateId);
            if(pNwkUpdateRequest->network_update_identifier > nwkUpdateId){
              /* update nwk updateid*/
              getSetUpdateId(g_FALSE_c ,&nwkUpdateId);
            }

            /* if channel is different set the logical channel*/
            if(pNwkUpdateRequest->logical_channel != ZLL_NetworkGetchannel()){
                ZLL_NetworkSetchannel(pNwkUpdateRequest->logical_channel );
            }
        }

      }

    }
    else{
        return g_ZCL_Malformed_Command_c;
    }
  return g_ZCL_Success_c;
}
/*****************************************************************************/

void ZLL_Update_Scan_ConfirmationDetails( ZigBeeNetworkDetails networkInformation ,
                                         uint8_t scanStatus )
{
#ifdef g_ZLL_LIGHT_DEVICE_c
  /*validate if the configured PAN id is available in the vicinity else,
  take a random pan id*/
  uint8_t* pExtended_panId = pDeviceStartupSectionMemory->activeScan_extendedPANIDs[0];

  if( scanStatus == g_MAC_Success_c )
  {
    if( pDeviceStartupSectionMemory->activeScan_PANIDS[0] == networkInformation.shortPanId )
    {
      /*The network pan id exists in POS, hence set a random pan id*/
      pTouchLinkInitiatiorSectionMemory->Short_PanId = randomT();
      if (g_Memory_Matches_c == (memUtils_memCompare(
            networkInformation.extendedPanId,
            pExtended_panId,
            g_EXTENDED_PAN_ID_LENGTH_c)))
      {
          uint8_t i = 0x00;
          for( i =0x00; i < g_EXTENDED_PAN_ID_LENGTH_c; i++ )
          {
            pExtended_panId[i] = (uint8_t)randomT();
          }
      }
    }
  }
  /*if the status is other than success, then take the network parameters from the
  start request itself*/
  ZLL_Create_Network_Start_Response( pTouchLinkReceiverSectionMemory->initiator_long_address ,
                                    g_ZLL_RESPONSE_STATUS_SUCCESS_c );
  //usleep(10000);
  usleep(20000);
  ZLL_Change_LightDevice_State(g_T_NETWORK_START_STATE_c);

#endif //#ifdef g_ZLL_LIGHT_DEVICE_c

}
