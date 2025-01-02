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

#include "buffer_management.h"
#include "stack_common.h"
#include "ZLL_defines.h"
#include "ZLL_Interpan.h"
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "common_includes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#include "sw_timer.h"
#include "apsde_interface.h"
#include "apsme_interface.h"
//#include"zb_hil_intf.h"
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
#include "ZLL_Configuration.h"
#include "ApplicationThinLayer.h"
#include "ZLL_State_Machine.h"
#include "ZLL_defines.h"
#endif /* g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1*/



/*****************************************************************************
* Global Constants
*****************************************************************************/


/*****************************************************************************
* Public Memory declarations
*****************************************************************************/



/*****************************************************************************
* Private Memory declarations
*****************************************************************************/


static uint8_t transactionSequenceNumber = 0;



/*****************************************************************************
* Function Prototypes
*****************************************************************************/

static ZigBeeStatus_t ZLL_Send_Inter_PAN_Message(uint16_t destShortAddress,
                                            uint8_t* pDestExtendedAddress,
                                            uint8_t messageLength,
                                            uint8_t* pmessage);


/*****************************************************************************
* Public Functions
*****************************************************************************/


static ZigBeeStatus_t ZLL_Send_Inter_PAN_Message(uint16_t destShortAddress,
                                            uint8_t* pDestExtendedAddress,
                                            uint8_t messageLength,
                                            uint8_t* pmessage)
{
    uint8_t buffer_index;
    APSDE_InterPan_Data_Request_t *pApsdatarequest;


    buffer_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

    if( g_NO_AVAILABLE_BUFFER_c != buffer_index ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 53 */
        allocatedbuffer[buffer_index]= 53;
        freebuffer[buffer_index][0]= 53;
        freebuffer[buffer_index][1]= 0;
#endif
        pApsdatarequest = (APSDE_InterPan_Data_Request_t*)
                                             buffMgmt_getBufferPointer( buffer_index );



        /* dest address could be shortaddress or extended address */

        if( ( destShortAddress == 0xFFFF ) || ( destShortAddress != 0x0000 ) ){
            pApsdatarequest->dstaddress.short_address = destShortAddress;
            pApsdatarequest->dstaddrmode = g_INTRP_16BIT_ADDR_MODE_c;
        }

        pApsdatarequest->dstpanid = g_ZLL_BROADCAST_PANID_c;
        pApsdatarequest->srcaddrmode = g_INTRP_64BIT_ADDR_MODE_c;

        if(pDestExtendedAddress != g_NULL_c){
            pApsdatarequest->dstaddrmode = g_INTRP_64BIT_ADDR_MODE_c;
            memUtils_memCopy((uint8_t*)&(pApsdatarequest->dstaddress.IEEE_address[0]),
                             pDestExtendedAddress,g_EXTENDED_ADDRESS_LENGTH_c);
        }
        /* Nothing but g_ZLL_PROFILE_Id, g_ZLL_PROFILE_ID_FOR_TOUCH_LINK_COMMANDS_c
        is used inorder to differentiate b/w utility commands which uses
        HA profile Id */
        pApsdatarequest->profileid = g_ZLL_PROFILE_ID_FOR_TOUCH_LINK_COMMANDS_c;
        pApsdatarequest->clusterid = g_ZLL_COMMISSIONING_CLUSTER_c;
        pApsdatarequest->asdulength = messageLength;
        pApsdatarequest->asduhandle = App_GetTransSeqNumber();
        if(pmessage!= g_NULL_c){
            memUtils_memCopy( pApsdatarequest->a_asdu, pmessage, messageLength );
        }
        APS_interpan_data_request ( buffer_index, pApsdatarequest );
        return ZigBee_Success;
   }
    else{
        return ZigBee_No_Buffer;
    }
}



/*****************************************************************************/
/* Start of Inter-pAN Commands( ZLL Commissioning Touch link commands )
frame creation */
/*Interpan Command: This function is used by Touch Link procedure to set up the
network ,this command is initiated by the initiator. */

/* Scan Request Frame Creation*/
void ZLL_Create_Scan_Request_Command(void)
{

    ZigBeeStatus_t status;
    ZLL_General_Inter_PAN_Format_t interpan_frame;

    /* In order to keep track of what is sent in scan req, bcoz future
    req uses same param values ex: transaction id is used in future cmds
    also*/
    touchLinkInitiatorSection_t * pInitiatorSection = pTouchLinkInitiatiorSectionMemory;

    /* Build the inter-PAN scan request frame  */
    /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type =
		g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific =
		g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction =
		g_ZCL_DIRECTION_CLIENT_TO_SERVER_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response
		= 1; /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    /* */
    interpan_frame.zll_zcl_header_field.transaction_seq_number = transactionSequenceNumber++;

    /* Command id */
    interpan_frame.zll_zcl_header_field.commandId = g_ZLL_Scan_Req_Command_c;

    /* Set interpan payload with specific scan request fields*/
    /* InterPAN_transaction_identifier is the one set on INIT state of touch link initiator */
    /* A new scan operation with multiple scan using the same new InterPAN_transaction_identifier is going to be started */
    interpan_frame.payload.scan_request.inter_pan_transaction_identifier =
      pInitiatorSection->interPAN_transaction_identifier;


    interpan_frame.payload.scan_request.ZigBee_information_t.logical_type =
      g_ZLL_LOGICAL_TYPE_END_DEVICE_c;
    interpan_frame.payload.scan_request.ZigBee_information_t.rx_on_when_idle =
		g_ZLL_DEVICE_RX_ON_OR_NOT_c;
    interpan_frame.payload.scan_request.ZigBee_information_t.reserved = 0;
    interpan_frame.payload.scan_request.ZLL_information_t.factory_new =
      (ZLL_IsFactoryNew()) ? g_ZLL_FACTORY_NEW_c : g_ZLL_NON_FACTORY_NEW_c;
    interpan_frame.payload.scan_request.ZLL_information_t.address_assignment =
		g_ZLL_ADDRESS_ASSIGNMENT_CAPABLE_c;
    interpan_frame.payload.scan_request.ZLL_information_t.link_initiator =
		g_ZLL_LINK_INITIATOR_c;
    interpan_frame.payload.scan_request.ZLL_information_t.reserved1 = 0;
    interpan_frame.payload.scan_request.ZLL_information_t.undefined = 0; /* it can be 0 or 1 */
    interpan_frame.payload.scan_request.ZLL_information_t.reserved2 = 0;

    /* send scan request interpan message */
    status = ZLL_Send_Inter_PAN_Message (0xFFFF, /* no multicast id */
                                     g_NULL_c, /* no long destination long address */
                                     sizeof(ZLL_ZCL_header_Field_t)+
                                         sizeof(Scan_request_record_list_field),
                                     (uint8_t *) &interpan_frame);


#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("ZSL_scan_request:%x\r\n\r\n", status);
    printf( "scan_request on channel :%x \t status :%x \t\n ",
		   status,ZLL_NetworkGetchannel());
#endif

}

/*****************************************************************************/
void ZigBeeGetNWKParameters(ZigBee_Network_Descriptor_t *pDescriptor)
{
  uint8_t g_NWK_ExtendedPan_ID[8] ={0};
  rsi_zigb_get_extended_panid(g_NWK_ExtendedPan_ID);
    if(pDescriptor != g_NULL_c){
        /*To get the 64-bit Extended PAN id of the operating Network */

        memUtils_memCopy(pDescriptor->a_extended_pan_id,
                         (uint8_t *) &g_NWK_ExtendedPan_ID,
                g_EXTENDED_ADDRESS_LENGTH_c);

        memUtils_memCopy((uint8_t*)&(pDescriptor->pan_id),(uint8_t*)rsi_zigb_get_short_panid(),
                         g_SHORT_ADDRESS_LENGTH_c);
        pDescriptor->radio_tx_power = 0xff;
        pDescriptor->logical_channel = rsi_zigb_get_operating_channel();
    }
}

/* Scan Response Frame Creation*/
void ZLL_Create_Scan_Response_Command(uint8_t *pDestExtAddress,uint32_t interPANTransactionId, BOOL Initiator)
{
    ZigBeeStatus_t status;

    ZLL_General_Inter_PAN_Format_t interpan_frame;
    touchLinkReceiverSectionType_t * pReceiverSection=g_NULL_c;
    /* In order to keep track of what is sending in scan response, bcoz future
    res uses same param values ex: response id is used in future cmds
    also*/

	uint8_t scan_response_length = sizeof(Scan_response_field);
    if( Initiator == g_TRUE_c )
    {
      pReceiverSection = pTouchLinkReceiverSectionMemory;
    }
    deviceStartupSection_t * pDevSection = pDeviceStartupSectionMemory;

    /* Build the inter-PAN scan request frame  */
    /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type = g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific = g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    /* Server to client*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction = g_ZCL_DIRECTION_SERVER_TO_CLIENT_c;
    /* Default resposne is not required*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response = 1; /* no default response: only on error */

    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    /* */
    interpan_frame.zll_zcl_header_field.transaction_seq_number = transactionSequenceNumber++;

    /* Command id  = 0x01 scan response*/
    interpan_frame.zll_zcl_header_field.commandId = g_ZLL_Scan_Response_Command_c;

    /* Set interpan payload with specific scan response fields */


    /* same inter-pan transaction id is to be set which is received from
    scan request, contents received from the initiator is stored in
    touchLinkReceiverSectionMemory.
    same inter-pan transaction id is to be set which is received from
    scan request,same id to be used util transaction Id expires or if
    target device choose different network, contents received from the initiator
    is stored in touchLinkReceiverSectionMemory*/
    if( Initiator == g_TRUE_c )
    {
      interpan_frame.payload.scan_response_field.inter_pan_transaction_identifier =
        pReceiverSection->interPAN_transaction_identifier;
    }
    else
    {
      interpan_frame.payload.scan_response_field.inter_pan_transaction_identifier =
        interPANTransactionId;
    }

    /* pre-programmed RSSI correction offset*/
    interpan_frame.payload.scan_response_field.rssi_correction = g_ZLL_RSSI_CORRECTION_OFFSET_c;

    interpan_frame.payload.scan_response_field.ZigBee_information_t.logical_type =
        g_ZLL_DEVICE_TYPE_c;

    interpan_frame.payload.scan_response_field.ZigBee_information_t.rx_on_when_idle
        = g_ZLL_DEVICE_RX_ON_OR_NOT_c;

    interpan_frame.payload.scan_response_field.ZigBee_information_t.reserved = 0;

    interpan_frame.payload.scan_response_field.ZLL_information_t.factory_new =
    (ZLL_IsFactoryNew()) ? g_ZLL_FACTORY_NEW_c : g_ZLL_NON_FACTORY_NEW_c;

    interpan_frame.payload.scan_response_field.ZLL_information_t.address_assignment
        = g_ZLL_ADDRESS_ASSIGNMENT_CAPABLE_c ;

     /*scan response receiver cannot be initiator */
    interpan_frame.payload.scan_response_field.ZLL_information_t.touck_link_initiator = g_ZLL_NOT_LINK_INITIATOR_c;

#ifdef g_ZLL_LIGHT_DEVICE_c
    /* target device is not requesting for priority */
    interpan_frame.payload.scan_response_field.ZLL_information_t.touch_link_priority_request =
        ZLL_Light_Target_Priority;
#endif //#ifdef g_ZLL_LIGHT_DEVICE_c

#ifdef g_ZLL_LIGHT_CONTROLLER_c
    interpan_frame.payload.scan_response_field.ZLL_information_t.touch_link_priority_request =
        g_ZLL_NOT_REQUEST_PRIORITY_c;
#endif //#ifdef g_ZLL_LIGHT_CONTROLLER_c

    interpan_frame.payload.scan_response_field.ZLL_information_t.reserved1 = 0;
    interpan_frame.payload.scan_response_field.ZLL_information_t.reserved2 = 0;

    /* Which security is currently supported can be set by key bitmask*/

    if( ZLL_SecurityKeyIndex )
    {
      interpan_frame.payload.scan_response_field.key_bitmask_t.development_key |=
          (ZLL_SecurityKeyIndex & g_ZLL_KEY_BITMASK_DEVELOPMENT_KEY_c);
      interpan_frame.payload.scan_response_field.key_bitmask_t.master_key |=
          (ZLL_SecurityKeyIndex & g_ZLL_KEY_BITMASK_MASTER_KEY_c);
      interpan_frame.payload.scan_response_field.key_bitmask_t.certification_key |=
          (ZLL_SecurityKeyIndex & g_ZLL_KEY_BITMASK_CERTIFICATION_KEY_c);
    }


    /* response Id is a 4 byte non -zero value*/
    uint32_t tempStorage = randomT();
    interpan_frame.payload.scan_response_field.response_identifier = tempStorage;

    if( Initiator == g_TRUE_c )
    {
      pReceiverSection->response_transcation_identifier = tempStorage;
    }


    if (ZLL_IsFactoryNew()){
        /* FN device */
        /* set the Proposed extended panid and short panid are the ones selected on
        device startup */
        if (g_ZLL_DEVICE_TYPE_c == g_ZLL_LOGICAL_TYPE_ROUTER_c) {
          pDevSection->activeScan_PANIDS[0] = randomT();

           MAC_MLMEPibSet(g_MAC_PAN_ID_c, g_INVALID_INDEX_c,
                               (uint8_t*) &(pDevSection->activeScan_PANIDS[0]));

          /* FN router: on router device startup a panid and extended pan id were selected */
          interpan_frame.payload.scan_response_field.pan_identifier =
            pDevSection->activeScan_PANIDS[0] ;


          memUtils_memCopy ((uint8_t*)&(interpan_frame.payload.scan_response_field.extended_pan_identifier),
                     (uint8_t*)(pDevSection->activeScan_extendedPANIDs[0]),
                    g_ZLL_EXTENDED_PANID_SIZE_c);
        }

        /* FN device: network address is 0xFFFF, target device cannot propose
        new value*/
        interpan_frame.payload.scan_response_field.network_address =  0xFFFF;

    }
    else {
        /* NFN device: take the current ZigBee network parameters */

        ZigBee_Network_Descriptor_t parameters;

        /* If NFN extended pan id is the one on which it operates */
        ZigBeeGetNWKParameters(&parameters);
        memUtils_memCopy ((uint8_t*)&(interpan_frame.payload.scan_response_field.extended_pan_identifier),
                          parameters.a_extended_pan_id,
                          g_ZLL_EXTENDED_PANID_SIZE_c);

        /*set short pan id */
        memUtils_memCopy((uint8_t*)&(interpan_frame.payload.scan_response_field.pan_identifier) ,
                         (uint8_t*)&(parameters.pan_id),g_ZLL_SHORT_PANID_SIZE_c);

        /* set self address*/
        memUtils_memCopy((uint8_t*)&(interpan_frame.payload.scan_response_field.network_address),
            //(uint8_t*)&ZigBeeGetSelfShortAddress(),g_ZLL_SHORT_PANID_SIZE_c);
            (uint8_t*)rsi_zigb_get_self_short_address(),g_ZLL_SHORT_PANID_SIZE_c);
    }

  /* Set the other network parameters (for both case FN or NFN router) */

  /* If FN: Network_Update_Identifier = 0 */
    uint8_t updateId = 0x00;
    getSetUpdateId(g_TRUE_c,&updateId);

    interpan_frame.payload.scan_response_field.network_update_identifier =
      (ZLL_IsFactoryNew()) ? 0 :updateId ;
    interpan_frame.payload.scan_response_field.logical_channel= ZLL_NetworkGetchannel();

    interpan_frame.payload.scan_response_field.no_of_subdevices =
        g_ZLL_CONFIG_SUB_DEVICES_NUMBER_c;
    /* no group id for a router/light */
    interpan_frame.payload.scan_response_field.total_group_identifiers =
      (g_ZLL_DEVICE_TYPE_c == g_ZLL_LOGICAL_TYPE_ROUTER_c) ? 0 :
      g_ZLL_TOTAL_GROUP_IDENTIFIER_NUMBER_c ;

    /* Checking whether the ZigBee Node is supporting single or many
      Zll devices*/
    if (1 == interpan_frame.payload.scan_response_field.no_of_subdevices) {
        /* Following fields are only for node with  sub device support */
        /* endpoint id*/
        interpan_frame.payload.scan_response_field.end_point_identifier =
            ZLL_device_information_table[0].endpoint_identifier;
        /* profile Id*/
        interpan_frame.payload.scan_response_field.profile_identifier =
          ZLL_device_information_table[0].profile_identifier;

        /* device Id*/
        interpan_frame.payload.scan_response_field.device_identifier =
          ZLL_device_information_table[0].device_identifier;


        /* device version*/

		interpan_frame.payload.scan_response_field.version_t.reserved = 0x00;
        interpan_frame.payload.scan_response_field.version_t.device_version =
            (ZLL_device_information_table[0].device_version_t.device_version);

		interpan_frame.payload.scan_response_field.group_identifier_count =
		(g_ZLL_DEVICE_TYPE_c == g_ZLL_LOGICAL_TYPE_END_DEVICE_c) ?
			ZLL_device_information_table[0].number_of_groups_identifiers : 0;
    }
	else{

		/* If no of sub device supported by a node is more than 1 then ,
		payload shd no  contain following fields */

		/* end_point_identifier (1) byte
		profile_identifier(2) byte
		device_identifier(2) byte
		version_t (1) byte
		group_identifier_count(1) byte	*/
		scan_response_length -=  0x07;
	}

    /* send scan response interpan message */
    status = ZLL_Send_Inter_PAN_Message (0,
        pDestExtAddress,sizeof(ZLL_ZCL_header_Field_t)+ scan_response_length,
                                     (uint8_t*)&interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("scan response status: %x \t channel: %x \t\n",status,
           ZLL_NetworkGetchannel());
#endif

}

/*****************************************************************************/
/* Creation of the Device information Request packet */
void ZLL_Create_Device_Information_Request_Command(uint8_t *pDestExtAddress)
{

    /*
        Frame Payload
        1. Transaction Id
        2. Start Index (endpoint)
    */


    ZigBeeStatus_t status;
    ZLL_General_Inter_PAN_Format_t interpan_frame;

    /* In order to keep track of what is sent in scan req, bcoz future
    req uses same param values ex: transaction id is used in future cmds also*/
    touchLinkInitiatorSection_t * pInitiatorSection = pTouchLinkInitiatiorSectionMemory;

    /* Build the inter-PAN Device information request frame  */
    /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type = g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific = g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction = g_ZCL_DIRECTION_CLIENT_TO_SERVER_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response = 1; /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    interpan_frame.zll_zcl_header_field.transaction_seq_number = transactionSequenceNumber++;

    /* Command id :device information request cmd*/
    interpan_frame.zll_zcl_header_field.commandId = g_ZLL_Device_Information_Req_Command_c;

    interpan_frame.payload.device_information_request.inter_pan_transaction_identifier =
      pInitiatorSection->interPAN_transaction_identifier;


    /* For now, start index value is set to zero ,provision can be provided to
    application*/
    interpan_frame.payload.device_information_request.start_index
        = g_ZLL_DEVICE_TABLE_START_INDEX_c;

    /* send device information request interpan message */
    status = ZLL_Send_Inter_PAN_Message (0, pDestExtAddress,
        sizeof(ZLL_ZCL_header_Field_t)+ sizeof(Device_information_record_list_field),
        (uint8_t*)&interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("Device information request status: %x \t channel: %x \t\n",status,
           ZLL_NetworkGetchannel);
#endif

}

/*****************************************************************************/
/* Creation of Device Information Response Cmd frame*/
void ZLL_Create_Device_Information_Response_Command(uint8_t *pDestExtAddress,
                                                    uint8_t startIndex)
{
    /* Frame payload
            1.Interpan transaction Id
            2.No of sub devices
            3.start index
            4.device informaiton record count
            5.device information record */

    ZigBeeStatus_t status;
    ZLL_General_Inter_PAN_Format_t interpan_frame;

    touchLinkReceiverSectionType_t * pReceiverSection = pTouchLinkReceiverSectionMemory;

    /* Build the inter-PAN device information response frame  */
    /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type =
        g_ZLL_FRAME_TYPE_c;
    /* No manufacturer code for ZLL commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific =
        g_ZLL_NO_MANUFACTURER_CODE_c;
    /* Server to client*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction =
        g_ZCL_DIRECTION_SERVER_TO_CLIENT_c;
    /* Default resposne is not required*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response
        = 1;

    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;

    interpan_frame.zll_zcl_header_field.transaction_seq_number = transactionSequenceNumber++;

    /* Command id  = 0x01 scan response*/
    interpan_frame.zll_zcl_header_field.commandId =
        g_ZLL_Device_Information_Response_Command_c;

    /* Set interpan payload with specific device information fields*/


    /* same inter-pan transaction id is to be set which is received from
    scan request, contents received from the initiator is stored in
    touchLinkReceiverSectionMemory. */
    interpan_frame.payload.device_information_response_field.
                     inter_pan_transaction_identifier =
                       pReceiverSection->interPAN_transaction_identifier;


    /* NO OF subdevices = no of endpoints*/
    interpan_frame.payload.device_information_response_field.no_of_subdevices =
        g_ZLL_NUMBER_OF_ENDPOINTS_c;

    if(!startIndex) {
        interpan_frame.payload.device_information_response_field.start_index =
            g_ZLL_DEVICE_TABLE_START_INDEX_c;
    }
    else{
        interpan_frame.payload.device_information_response_field.start_index =
            startIndex;
    }

    interpan_frame.payload.device_information_response_field.device_information_record_count =
        g_ZLL_NUMBER_OF_ENDPOINTS_c;

    uint8_t tempDeviceRecordCount = 0;
    /* start index could be zero or anyother index less than total no*/
    for (uint8_t i = startIndex; i < g_ZLL_NUMBER_OF_ENDPOINTS_c; i++) {

    /* IEEE address of the device*/
    memUtils_memCopy((uint8_t*)&(interpan_frame.payload.device_information_response_field.
                   device_information_record[i].IEEE_address),
                 (uint8_t*)&(ZLL_device_information_table[i].IEEE_address),
                 g_ZLL_EXTENDED_PANID_SIZE_c);
    /* endpoint Id */
    interpan_frame.payload.device_information_response_field.device_information_record[i].end_point_identifier =
        ZLL_device_information_table[i].endpoint_identifier;
    /* profile Id*/
    memUtils_memCopy((uint8_t*)&(interpan_frame.payload.device_information_response_field.device_information_record[i].profile_identifier),
                     (uint8_t*)&(ZLL_device_information_table[i].profile_identifier),
                     g_ZLL_PROFILE_ID_SIZE_c );
    /* device id*/
    memUtils_memCopy((uint8_t*)&(interpan_frame.payload.device_information_response_field.device_information_record[i].device_identifier),
        (uint8_t*)&(ZLL_device_information_table[i].device_identifier),
        g_ZLL_DEVICE_ID_SIZE_c) ;
    /* device version */
    interpan_frame.payload.device_information_response_field.device_information_record[i].version_t.device_version =
        ZLL_device_information_table[i].device_version_t.device_version;
    /* no of group id's supported by device(each endpoint)*/
    interpan_frame.payload.device_information_response_field.device_information_record[i].group_identifier_count =
        ZLL_device_information_table[i].number_of_groups_identifiers;
    /* sorted or not */
    interpan_frame.payload.device_information_response_field.device_information_record[i].sort =
        ZLL_device_information_table[i].sort_tag;
    tempDeviceRecordCount++;
    }
    uint8_t payloadLength = 0x00;

    /*Note: 7 is sizeof device information response's remaining fields */
    payloadLength =  sizeof(ZLL_ZCL_header_Field_t)+
      (tempDeviceRecordCount *sizeof(Device_information_record))+ 7;

    status = ZLL_Send_Inter_PAN_Message(0, /* no multicast id */
                                     pDestExtAddress,
                                     payloadLength,
                                     (uint8_t *) &interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("Device information response status: %x \t channel: %x \t\n",status,
           ZLL_NetworkGetchannel());
#endif
}

/*****************************************************************************/
/* Create Identify Request*/
void ZLL_Create_Identify_Request(uint8_t *pDestExtAddress)
{

    ZigBeeStatus_t status;
    ZLL_General_Inter_PAN_Format_t interpan_frame;

    /* In order to keep track of what is sent in scan req, bcoz future
    req uses same param values ex: transaction id is used in future cmds also*/
    touchLinkInitiatorSection_t * pInitiatorSection = pTouchLinkInitiatiorSectionMemory;

    /* Build the inter-PAN Device information request frame  */
    /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type = g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific = g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction = g_ZCL_DIRECTION_CLIENT_TO_SERVER_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response = 1; /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    interpan_frame.zll_zcl_header_field.transaction_seq_number = transactionSequenceNumber++;

    /* Command id :device information request cmd*/
    interpan_frame.zll_zcl_header_field.commandId = g_ZLL_Identify_Req_Command_c;

    interpan_frame.payload.device_information_request.inter_pan_transaction_identifier =
      pInitiatorSection->interPAN_transaction_identifier;

    /* Set identify_request interpan command frame ID with
    scan_request interPAN_transaction_identifier
    (stored in the touch link inititiator context) */
    interpan_frame.payload.identify_request.inter_pan_transaction_identifier =
      pInitiatorSection->interPAN_transaction_identifier;



    /* identify duration specified in the intiator context*/
    memUtils_memCopy((uint8_t*)&(interpan_frame.payload.identify_request.identify_duration),
        (uint8_t*)&(pInitiatorSection->identify_duration),g_SIZE_OF_IDENTIFY_DURATION_c );

    status = ZLL_Send_Inter_PAN_Message(0, /* no multicast id */
                                     pDestExtAddress,
                                     sizeof(ZLL_ZCL_header_Field_t)+
                                         sizeof(Identify_record_list_field),
                                     (uint8_t *) &interpan_frame);


#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("Identify Request status: %x \t channel: %x \t\n",status,
           ZLL_NetworkGetchannel());
#endif
}

/*****************************************************************************/
void ZLL_Create_Reset_To_Factor_New_Request(uint8_t *pDestExtAddress)
{

    ZigBeeStatus_t status;
    ZLL_General_Inter_PAN_Format_t interpan_frame;

    /* In order to keep track of what is sent in scan req, bcoz future
    req uses same param values ex: transaction id is used in future cmds also*/
    touchLinkInitiatorSection_t * pInitiatorSection = pTouchLinkInitiatiorSectionMemory;

    /* Build the inter-PAN Device information request frame  */
    /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type =
	  g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific =
	  g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction =
	  g_ZCL_DIRECTION_CLIENT_TO_SERVER_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response
	  = 1; /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    interpan_frame.zll_zcl_header_field.transaction_seq_number =
	  transactionSequenceNumber++;

    /* Command id :device information request cmd*/
    interpan_frame.zll_zcl_header_field.commandId =
	  g_ZLL_Reset_To_Factory_New_Req_Command_c;

    interpan_frame.payload.reset_to_factory_new_request.inter_pan_transaction_identifier =
      pInitiatorSection->interPAN_transaction_identifier;


    status = ZLL_Send_Inter_PAN_Message(0, /* no multicast id */
                                 pDestExtAddress,
                                 sizeof(ZLL_ZCL_header_Field_t)+
                                     sizeof(Reset_to_factory_new_record_list_field),
                                 (uint8_t *) &interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("Reset To Factory new Request:status : %x \t channel: %x \t\n",status,
           ZLL_NetworkGetchannel());
#endif
}

/*****************************************************************************/
void ZLL_Create_Network_Update_Request(uint8_t* pDestExtAddress)
{
	ZigBeeStatus_t status;
    ZLL_General_Inter_PAN_Format_t interpan_frame;

    /* In order to keep track of what is sent in scan req, bcoz future
    req uses same param values ex: transaction id is used in future cmds also*/
    touchLinkInitiatorSection_t * pInitiatorSection = pTouchLinkInitiatiorSectionMemory;

    /* Build the inter-PAN Device information request frame  */
    /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type =
	  g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific =
	  g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction =
	  g_ZCL_DIRECTION_CLIENT_TO_SERVER_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response
	  = 1; /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    interpan_frame.zll_zcl_header_field.transaction_seq_number =
	  transactionSequenceNumber++;

    /* Command id :device information request cmd*/
    interpan_frame.zll_zcl_header_field.commandId =
	  g_ZLL_Network_Update_Req_Command_c;

	/* set Interpan transaction Id*/
    interpan_frame.payload.network_update_request.inter_pan_transaction_identifier =
      pInitiatorSection->interPAN_transaction_identifier;

	/* Get Extended panid*/
	//ZigBeeGetExtendedPanId(interpan_frame.payload.network_update_request.extended_pan_identifier);
	rsi_zigb_get_extended_panid(interpan_frame.payload.network_update_request.extended_pan_identifier);

	/* current channel*/
	interpan_frame.payload.network_update_request.logical_channel =
	  ZLL_NetworkGetchannel();

	/* recipient device short address */
	if(pDestExtAddress != g_NULL_c)
		interpan_frame.payload.network_update_request.network_address =
            rsi_zigb_get_short_addr_for_specified_ieee_addr(pDestExtAddress);
            //ZigBeeGetShortAddrForSpecifiedIEEEAddr(pDestExtAddress);

	/* current short panid*/
	interpan_frame.payload.network_update_request.pan_identifier =
    rsi_zigb_get_short_panid();
	  //ZigBeeGetShortPANId();

	uint8_t updateId = 0x00;
	/* retrieve nwk update Id*/
	getSetUpdateId(g_TRUE_c,&updateId);
	interpan_frame.payload.network_update_request.network_update_identifier =
	  updateId;


    status = ZLL_Send_Inter_PAN_Message(0, /* no multicast id */
                                 pDestExtAddress,
                                 sizeof(ZLL_ZCL_header_Field_t)+
                                     sizeof(Reset_to_factory_new_record_list_field),
                                 (uint8_t *) &interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("Reset To Factory new Request:status : %x \t channel: %x \t\n",status,
           ZLL_NetworkGetchannel());
#endif

}
/*****************************************************************************/
/* Create frame for network start request*/
void ZLL_Create_Network_Start_Request(uint8_t*pDestExtAddress)
{
    ZigBeeStatus_t status;
    uint8_t expandedOutput[16];
    uint8_t networkKey[16];
    ZLL_General_Inter_PAN_Format_t interpan_frame;
    uint8_t index;
    uint8_t a[8]; 
    ZigBeeGetSelfIEEEAddress(a);
    /* In order to keep track of what is sent in scan req, bcoz future
    req uses same param values ex: transaction id is used in future cmds also*/
    touchLinkInitiatorSection_t * pInitiatorSection = pTouchLinkInitiatiorSectionMemory;

    /* i.e., self device's extended address */
    uint8_t initiator_IEEE_address[8];
    uint16_t short_address ;


    index = ZLL_ReadScanReponseForTarget(pDestExtAddress);

    memUtils_memCopy(initiator_IEEE_address,
        a,g_ZLL_EXTENDED_PANID_SIZE_c );

    /* Build the inter-PAN Device information request frame  */
    /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type =
        g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific =
        g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction =
        g_ZCL_DIRECTION_CLIENT_TO_SERVER_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response
        = 1; /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    interpan_frame.zll_zcl_header_field.transaction_seq_number = transactionSequenceNumber++;

    /* Command id :device information request cmd*/
    interpan_frame.zll_zcl_header_field.commandId = g_ZLL_Network_Start_Req_Command_c;
    /* Inter-PAN iD*/
    interpan_frame.payload.network_start_request.inter_pan_transaction_identifier =
      pInitiatorSection->interPAN_transaction_identifier;

    /* Extended PAN-Id*/
    memUtils_memCopy((uint8_t*)&(interpan_frame.payload.network_start_request.extended_pan_identifier[0]),
                     (uint8_t*)&pTouchLinkInitiatiorSectionMemory->Extended_PANId[0],g_ZLL_EXTENDED_PANID_SIZE_c);
    /* ZLL kEY supported index */
    interpan_frame.payload.network_start_request.key_index =
     pInitiatorSection->scan_response_stored[index].scan_response.key_bitmask_t.development_key;
    interpan_frame.payload.network_start_request.key_index =
     pInitiatorSection->scan_response_stored[index].scan_response.key_bitmask_t.certification_key;

    if( interpan_frame.payload.network_start_request.key_index )
    {
      /*Expand the ouput by using the interpan transaction id and
      response id and expand it to 128 bit number, the expanded output will have
      encrypted transport key encrypted based on the key index*/
      ZLL_Expand_EncryptTransportKey( pInitiatorSection->interPAN_transaction_identifier,
                       pInitiatorSection->scan_response_stored[index].scan_response.response_identifier,
                       expandedOutput , interpan_frame.payload.network_start_request.key_index);

      /*Get the network key*/
      status = GetNetworkKey( networkKey );
      if( status == g_SUCCESS_c )
      {
         /*encrypt the network key using the encrypted transport key*/
    	 uint8_t payload_length = g_ZLL_NETWORK_KEY_SIZE_c;
         status = Encrypt_Decrypt_Using_ECB_Mode( &payload_length , networkKey,
                    interpan_frame.payload.network_start_request.encrypted_network_key, g_TRUE_c);
      }
    }

  /* Set the pan id logical channel */
    interpan_frame.payload.network_start_request.logical_channel =
        ZLL_NetworkGetchannel();

    interpan_frame.payload.network_start_request.pan_identifier =
      pTouchLinkInitiatiorSectionMemory->Short_PanId ;

    /* assign the network address to the recepient device */
    uint16_t recipient_addr = ZLL_NetworkAddressAssignment(g_TRUE_c);

    /* if recipient addr is zero,then no more devices can be joined*/
    if(recipient_addr == 0)
      return;

    interpan_frame.payload.network_start_request.network_address = recipient_addr;
    /* recipient_addr will be used furture for comm*/
    ZLL_CallbackSection.paired_device = recipient_addr;
    ZLL_GroupIdentifierAssignment(g_TRUE_c,
        pInitiatorSection->scan_response_stored[pInitiatorSection->scan_response_stored_last_index].scan_response.total_group_identifiers);

    /* Set the group ids begin & end for recipient */
    interpan_frame.payload.network_start_request.group_identifiers_begin =
        ZLL_groups.recipients_start_group_id;
    interpan_frame.payload.network_start_request.group_identifiers_end =
        ZLL_groups.recipients_end_group_id;

    uint8_t temp_index = pInitiatorSection->scan_response_stored_last_index - 1;
    bool recipient_is_add_assign_capable =
        pInitiatorSection->scan_response_stored[temp_index].scan_response.ZLL_information_t.address_assignment;

    /* Only if recipient device is address assignement capable then following
    field to be set*/
    uint16_t recipient_nwk_addr_begin = 0,recipient_nwk_addr_end= 0;
    uint16_t recipient_grp_addr_begin = 0,recipient_grp_addr_end= 0;

    if(recipient_is_add_assign_capable){

        ZLL_SplitNetworkAddresses(&recipient_nwk_addr_begin,&recipient_nwk_addr_end );

        interpan_frame.payload.network_start_request.free_network_address_range_begin =
            recipient_nwk_addr_begin;
        interpan_frame.payload.network_start_request.free_network_address_range_end =
            recipient_nwk_addr_end;

        uint8_t recipient_req_no_of_grp_ids =
            pInitiatorSection->scan_response_stored[temp_index].scan_response.total_group_identifiers;
        ZLL_SplitGroupIdentifiers(&recipient_grp_addr_begin,
                                  &recipient_grp_addr_end,
                                  recipient_req_no_of_grp_ids);

        interpan_frame.payload.network_start_request.free_group_identifier_range_begin =
            recipient_grp_addr_begin;
        interpan_frame.payload.network_start_request.free_group_identifier_range_end =
            recipient_grp_addr_end;
    }
    else{
    /* set the field as zero if recipient device is not addr assignment
        capable*/
        interpan_frame.payload.network_start_request.free_network_address_range_begin =
            recipient_nwk_addr_begin;
        interpan_frame.payload.network_start_request.free_network_address_range_end =
            recipient_nwk_addr_end;
    }
    memUtils_memReverseCopy (
    (uint8_t*)(interpan_frame.payload.network_start_request.initiator_IEEE_address),
                    initiator_IEEE_address,
                    g_ZLL_EXTENDED_PANID_SIZE_c);
    interpan_frame.payload.network_start_request.initiator_network_address =
      ZLL_DeviceNetworkAddress();

    short_address = ZLL_DeviceNetworkAddress();

    MAC_MLMEPibSet(g_MAC_SHORT_ADDRESS_c, g_INVALID_INDEX_c,
                               (uint8_t*) &(short_address));


    status = ZLL_Send_Inter_PAN_Message(0, /* no multicast id */
                                 pDestExtAddress,
                                 sizeof(ZLL_ZCL_header_Field_t)+
                                     sizeof(Network_start_record_list_field),
                                 (uint8_t *) &interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("ZLL network start request :status : %x \t channel: %x \t\n",status,
           ZLL_NetworkGetchannel());
#endif

}
/*****************************************************************************/
void ZLL_Create_Network_Start_Response(uint8_t*pDestExtAddress, uint8_t status)
{
    ZigBeeStatus_t interpan_status;
    uint8_t a;
    ZLL_General_Inter_PAN_Format_t interpan_frame;

    touchLinkReceiverSectionType_t * pReceiverSection =
        pTouchLinkReceiverSectionMemory;

     /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type =
        g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific =
        g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction =
        g_ZCL_DIRECTION_SERVER_TO_CLIENT_c;
    /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response = 1;
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    interpan_frame.zll_zcl_header_field.transaction_seq_number =
        transactionSequenceNumber++;

    /* Command id :Start response command */
    interpan_frame.zll_zcl_header_field.commandId =
        g_ZLL_Nwk_Start_Response_Command_c;

    /* Set the Start Response specific field */

    /* same Transaction Id to be used , so retrieving from receiver section */
    interpan_frame.payload.network_start_response_field.
                     inter_pan_transaction_identifier =
                  pReceiverSection->interPAN_transaction_identifier;



    /* This response is just response that device received scan request and
    internal stack startup might not be success */
    interpan_frame.payload.network_start_response_field.status = status ;


    /* Set the the final network parameters (extended PAN identifier,
        logical channel and PAN identifier):
     If router is FN, the network parameters are the ones stored in
    device startup context (on field 0) on device startup.
     If router is NFN the network parameters are the ones stored in
    device startup context (on field 0) after an active scan complete.
     In both case the device startup context is used for storing the
    selected network parameters.
   */



    /* Ext-paind retrieving from device section ,
    Device startup ssection's 1st entryof active scan array will hold the
    ext pan id with which it shd start ,
    How the 1st entry is choosen as best and non -used ext panid is handled in
    diff function */
    memUtils_memCopy(interpan_frame.payload.network_start_response_field.
                     extended_pan_identifier,
                    (pDeviceStartupSectionMemory->activeScan_extendedPANIDs[0]),
                    g_ZLL_EXTENDED_PANID_SIZE_c);




    interpan_frame.payload.network_start_response_field.network_update_identifier = 0;

    /* Is a channel used by the device to start the network */
    interpan_frame.payload.network_start_response_field.logical_channel =
        ZLL_NetworkGetchannel();



    /* Is a short panid used by the device to start the network */
    interpan_frame.payload.network_start_response_field.pan_identifier =
      pDeviceStartupSectionMemory->activeScan_PANIDS[0];


    interpan_status = ZLL_Send_Inter_PAN_Message(0, /* no multicast id */
                                 pDestExtAddress,
                                 sizeof(ZLL_ZCL_header_Field_t)+
                                     sizeof(Network_start_response_field),
                                 (uint8_t *) &interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("ZLL network start response :status : %x \t channel: %x \t\n",interpan_status,
           ZLL_NetworkGetchannel());
#endif
}




/*****************************************************************************/
/* Create frame for network Join request*/
void ZLL_Create_Network_Join_Request(uint8_t*pDestExtAddress, uint8_t deviceType )
{
    ZigBeeStatus_t status;
    ZLL_General_Inter_PAN_Format_t interpan_frame;
     uint8_t expandedOutput[16];
    uint8_t networkKey[16];
    uint8_t index ;
    uint8_t a[8];
    ZigBeeGetSelfIEEEAddress(a);
    /* In order to keep track of what is sent in scan req, bcoz future
    req uses same param values ex: transaction id is used in future cmds also*/
    touchLinkInitiatorSection_t * pInitiatorSection =
        pTouchLinkInitiatiorSectionMemory;

    /* Holds device startup parameters*/


    /* i.e., self device's extended address */
    uint8_t initiator_IEEE_address[8];

    index = ZLL_ReadScanReponseForTarget(pDestExtAddress);

    if(index >= g_MAX_SCAN_RESPONSE_RESULTS_c )
       return;

    bool recipient_is_add_assign_capable =
        pInitiatorSection->scan_response_stored[index].scan_response.ZLL_information_t.address_assignment;

    memUtils_memCopy(initiator_IEEE_address,
        a,g_ZLL_EXTENDED_PANID_SIZE_c );

     /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type =
        g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific =
        g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction =
        g_ZCL_DIRECTION_CLIENT_TO_SERVER_c;
    /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response
        = 1;
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    interpan_frame.zll_zcl_header_field.transaction_seq_number =
        transactionSequenceNumber++;

    /* Command id :NETWORK join request cmd*/
    interpan_frame.zll_zcl_header_field.commandId =
        deviceType;

    interpan_frame.payload.network_join_request.
                     inter_pan_transaction_identifier =
                       pInitiatorSection->interPAN_transaction_identifier;

    //ZigBeeGetExtendedPanId(interpan_frame.payload.network_join_request.extended_pan_identifier);
    rsi_zigb_get_extended_panid(interpan_frame.payload.network_join_request.extended_pan_identifier);
    /* ZLL kEY supported index */
    interpan_frame.payload.network_join_request.key_index =
     pInitiatorSection->scan_response_stored[index].scan_response.key_bitmask_t.development_key;
    interpan_frame.payload.network_join_request.key_index =
     pInitiatorSection->scan_response_stored[index].scan_response.key_bitmask_t.certification_key;

    if( interpan_frame.payload.network_join_request.key_index )
    {
      /*Expand the ouput by using the interpan transaction id and
      response id and expand it to 128 bit number, the expanded output will have
      encrypted transport key encrypted based on the key index*/
      ZLL_Expand_EncryptTransportKey( pInitiatorSection->interPAN_transaction_identifier,
                       pInitiatorSection->scan_response_stored[index].scan_response.response_identifier,
                       expandedOutput , interpan_frame.payload.network_join_request.key_index);

      /*Get the network key*/
      status = GetNetworkKey( networkKey );
      if( status == g_SUCCESS_c )
      {
    	 uint8_t payload_Length=g_ZLL_NETWORK_KEY_SIZE_c;
         /*encrypt the network key using the encrypted transport key*/
         status = Encrypt_Decrypt_Using_ECB_Mode(  &payload_Length, networkKey,
                    interpan_frame.payload.network_join_request.encrypted_network_key, g_TRUE_c );
      }
    }



    uint8_t nwkUpdateId = 0;
    /* g_TRUE_c - tO get current nwkuPDATEiD
       g_FALSE_c - to set nwkUpdateId */
    getSetUpdateId(g_TRUE_c ,&nwkUpdateId);

    interpan_frame.payload.network_join_request.network_update_identifier =
        nwkUpdateId;

    /* assign zll channel*/
    interpan_frame.payload.network_join_request.logical_channel =
        ZLL_NetworkGetchannel();

    /* assign short panid */
    interpan_frame.payload.network_join_request.pan_identifier =
        rsi_zigb_get_short_panid();
        //ZigBeeGetShortPANId();


	uint16_t temp_address = ZLL_NetworkAddressAssignment(g_FALSE_c) ;
	/* check whether the address is assigned or not*/
	if(temp_address){
     /* Assign a network address to the target recipient device ,Also initiator
	  is still capable of making joining of other devices to the network*/
     interpan_frame.payload.network_join_request.network_address = temp_address;
	}
    else{
	  	/* Initiator is not capable of making join of other device to the
	  network*/
		return;
	}

        /* Only if recipient device is address assignement capable then following
    field to be set*/
    uint16_t recipient_nwk_addr_begin = 0,recipient_nwk_addr_end= 0;
    uint16_t recipient_grp_addr_begin = 0,recipient_grp_addr_end= 0;

    if(recipient_is_add_assign_capable){

        ZLL_SplitNetworkAddresses(&recipient_nwk_addr_begin,&recipient_nwk_addr_end );

        interpan_frame.payload.network_join_request.free_network_address_range_begin =
            recipient_nwk_addr_begin;
        interpan_frame.payload.network_join_request.free_network_address_range_end =
            recipient_nwk_addr_end;

        uint8_t recipient_req_no_of_grp_ids =
            pInitiatorSection->scan_response_stored[index].scan_response.total_group_identifiers;
        ZLL_SplitGroupIdentifiers(&recipient_grp_addr_begin,
                                  &recipient_grp_addr_end,
                                  recipient_req_no_of_grp_ids);

        interpan_frame.payload.network_join_request.free_group_identifier_range_begin =
            recipient_grp_addr_begin;
        interpan_frame.payload.network_join_request.free_group_identifier_range_end =
            recipient_grp_addr_end;
    }
    else{
    /* set the field as zero if recipient device is not addr assignment
        capable*/
        interpan_frame.payload.network_start_request.free_network_address_range_begin =
            recipient_nwk_addr_begin;
        interpan_frame.payload.network_start_request.free_network_address_range_end =
            recipient_nwk_addr_end;
    }




    bool target_addr_assign_capable = g_FALSE_c;
    target_addr_assign_capable = ZLL_IsTheDevice_AddrAssignmentCapable();

    if(!target_addr_assign_capable ){
      interpan_frame.payload.network_join_request.free_network_address_range_begin
        = 0x0000;
      interpan_frame.payload.network_join_request.free_network_address_range_end
        = 0x0000;
    }else{
      /* TO DO ,assigning the address range*/
    }

    if(ZLL_IsTheDevice_HasGroups()){
      /* TO DO , assigning groups to the device*/

    }
    else{
      interpan_frame.payload.network_join_request.group_identifiers_begin
        = 0x0000;
      interpan_frame.payload.network_join_request.group_identifiers_end
        = 0x0000;
    }

    /* TO DO :assigning address range to the target device*/


    status = ZLL_Send_Inter_PAN_Message(0, /* no multicast id */
                                 pDestExtAddress,
                                 sizeof(ZLL_ZCL_header_Field_t)+
                                     sizeof(Network_join_record_list_field),
                                 (uint8_t *) &interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("ZLL network join network request :status : %x \t channel: %x \t\n",status,
           ZLL_NetworkGetchannel());
#endif


}

void ZLL_Create_Network_Join_Response(uint8_t*pDestExtAddress,
                                      uint8_t Command_id, uint8_t status)
{
    ZigBeeStatus_t interPAN_Status;
    ZLL_General_Inter_PAN_Format_t interpan_frame;


    touchLinkReceiverSectionType_t * pReceiverSection =
        pTouchLinkReceiverSectionMemory;

     /* Set interpan general command frame fields */
    interpan_frame.zll_zcl_header_field.frame_Control_t.frame_type =
        g_ZLL_FRAME_TYPE_c;
    interpan_frame.zll_zcl_header_field.frame_Control_t.manufacturer_specific =
        g_ZLL_NO_MANUFACTURER_CODE_c; /* No manufacturer code for zll commissioning cluster*/
    interpan_frame.zll_zcl_header_field.frame_Control_t.direction =
        g_ZCL_DIRECTION_SERVER_TO_CLIENT_c;
    /* no default response: only on error */
    interpan_frame.zll_zcl_header_field.frame_Control_t.disable_default_response = 1;
    interpan_frame.zll_zcl_header_field.frame_Control_t.reserved = 0;
    interpan_frame.zll_zcl_header_field.transaction_seq_number =
        transactionSequenceNumber++;

    if(Command_id == g_ZLL_Network_Join_Router_Req_Command_c){
        /* Command id : Join router response*/
      interpan_frame.zll_zcl_header_field.commandId =
          g_ZLL_Network_Join_Router_Response_Command_c;

    }else if(Command_id == g_ZLL_Network_Join_End_Device_Req_Command_c )
    {

      /* Command id : Join end device response*/
      interpan_frame.zll_zcl_header_field.commandId =
          g_ZLL_Network_Join_End_Device_Response_Command_c;

    }
     /* This response is just response that device received scan request and
    internal stack startup might not be success */
    interpan_frame.payload.network_join_response_field.status =
        status;

    /* same Transaction Id to be used , so retrieving from receiver section */
    interpan_frame.payload.network_join_response_field.
                     inter_pan_transaction_identifier =
                  pReceiverSection->interPAN_transaction_identifier;



    interPAN_Status = ZLL_Send_Inter_PAN_Message(0, /* no multicast id */
                                 pDestExtAddress,
                                 sizeof(ZLL_ZCL_header_Field_t)+
                                     sizeof(Network_join_response_field),
                                 (uint8_t *) &interpan_frame);

#if (g_CLI_SUPPORT_d == 1) && ( DEBUG_TEST == 1 )
    printf("ZLL network start response :status : %x \t channel: %x \t\n",interPAN_Status,
           ZLL_NetworkGetchannel());
#endif
}


/*****************************************************************************/

void ZLL_TriggerStartRequest( void )
{
    uint8_t buffer_index;

    buffer_index = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

    Silent_Startup_Param_t silentStartupParam ;
    if( g_NO_AVAILABLE_BUFFER_c != buffer_index ) {

      silentStartupParam.nwk_short_addr =
        pDeviceStartupSectionMemory->recipient_device_network_address;
      silentStartupParam.nwk_pan_id =
        pDeviceStartupSectionMemory->activeScan_PANIDS[0];

      memUtils_memCopy((uint8_t*)(silentStartupParam.nwk_ext_pan_id ),
                    (uint8_t*)(pDeviceStartupSectionMemory->activeScan_extendedPANIDs[0]),
                    g_ZLL_EXTENDED_PANID_SIZE_c);

      silentStartupParam.nwk_channel = pDeviceStartupSectionMemory->zll_channel;
        ZDO_handleSilentStartRequest((uint8_t*)&silentStartupParam,buffer_index);

    }
}





/*****************************************************************************/
void ZLL_AddScanResponseEntry( Scan_response_field *pScanResponse,
                              uint8_t *pSrcIEEEAddr)
{
  uint8_t index_i = 0x00;
  uint8_t index_j = 0x00;
  ZLL_scan_response_stored_t *pStored;
  /*Sort and add the entry into the scan stored structure*/
  uint8_t scanResponseIndex = pTouchLinkInitiatiorSectionMemory->scan_response_stored_last_index ;
      /*copy the IEEE address of the received scan response*/
    /*increment the scan index to hold the next scan response*/
    if ( pTouchLinkInitiatiorSectionMemory->scan_response_stored_last_index == g_MAX_SCAN_RESPONSE_RESULTS_c )
      return;

    /*check for duplicate entry and then update*/
    memUtils_memCopy(pTouchLinkInitiatiorSectionMemory->scan_response_stored[scanResponseIndex].src_long_address,
    pSrcIEEEAddr,g_EXTENDED_ADDRESS_LENGTH_c);

    memUtils_memCopy((uint8_t*)&(pTouchLinkInitiatiorSectionMemory->scan_response_stored[scanResponseIndex].scan_response),
           (uint8_t*)pScanResponse, sizeof(Scan_response_field)  );

    pTouchLinkInitiatiorSectionMemory->scan_response_stored[scanResponseIndex].scan_entryValid = g_TRUE_c;

    pTouchLinkInitiatiorSectionMemory->scan_response_stored_last_index++;


    pStored = &pTouchLinkInitiatiorSectionMemory->scan_response_stored[0];
    /*Sort the scan response fields based on rssi correction. The highest rssi correction value
    indicates the device is the best*/
    for( index_i = 0x00; index_i < g_MAX_SCAN_RESPONSE_RESULTS_c; index_i++)
    {
      for( index_j = 0x00; index_j < (index_i - 1); index_j++ )
      {
        Scan_response_field aScanResponseTemp;

        if(( pStored[index_j].scan_entryValid == g_TRUE_c ) &&
           ( pStored[index_j + 1 ].scan_entryValid == g_TRUE_c ) )
        {
          if( pStored[index_j + 1].scan_response.rssi_correction > pStored[index_j].scan_response.rssi_correction )
          {
              memUtils_memCopy((uint8_t*)&aScanResponseTemp, (uint8_t*)&pStored[index_j].scan_response,
                    sizeof(Scan_response_field) );
              memUtils_memCopy((uint8_t*)&pStored[index_j].scan_response,
                               (uint8_t*)&pStored[index_j + 1].scan_response,
                    sizeof(Scan_response_field) );
              memUtils_memCopy((uint8_t*)&pStored[index_j + 1].scan_response,
                    (uint8_t*)&aScanResponseTemp, sizeof(Scan_response_field) );
          }
        }
      }

    }
}

/*****************************************************************************/

uint8_t ZLL_NextJoiningDevice( void )
{
  uint8_t index;
  for( index = 0x00; index < g_MAX_SCAN_RESPONSE_RESULTS_c; index++)
  {
     if( pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].scan_entryValid == g_TRUE_c )
     {
       return index;
     }
  }
  return g_ZCL_Not_Found_c;
}
/*****************************************************************************/
uint8_t ZLL_IsTheDevice_AddrAssignmentCapable( void )
{
  uint8_t index = pTouchLinkInitiatiorSectionMemory->scan_response_active_index;

  if( pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].scan_response.ZLL_information_t.address_assignment )
  {
    return g_TRUE_c;
  }
  return g_FALSE_c;
}

/*****************************************************************************/
uint8_t ZLL_IsTheDevice_HasGroups( void )
{
  uint8_t index = pTouchLinkInitiatiorSectionMemory->scan_response_active_index;

  if( pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].scan_response.total_group_identifiers )
  {
    return g_TRUE_c;
  }
  return g_FALSE_c;
}
/*****************************************************************************/

uint8_t ZLL_ReadScanReponseForTarget( uint8_t *pTarget_long_address)
{
  uint8_t index = 0xff;
  for( index = 0x00; index < g_MAX_SCAN_RESPONSE_RESULTS_c; index++)
  {
     if( ( pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].scan_entryValid == g_TRUE_c ) &&
         ( g_Memory_Matches_c == (memUtils_memCompare(
            pTouchLinkInitiatiorSectionMemory->scan_response_stored[index].src_long_address,
            pTarget_long_address,
            g_EXTENDED_ADDRESS_LENGTH_c))))
     {
        break;
     }
  }
  return index;
}

 void ZLL_Expand_EncryptTransportKey( uint32_t requestId, uint32_t ResponseId,
              uint8_t *pExpandedOutput, uint8_t keyIndex)
{
  uint8_t currentKey[16];
  uint8_t encryptedTransportKey[16];
  /*created the expanded output*/
  memUtils_memReverseCopy(pExpandedOutput,
						 (uint8_t*)&requestId,0x04);
  memUtils_memReverseCopy(&pExpandedOutput[4],
						 (uint8_t*)&requestId,0x04);
  memUtils_memReverseCopy(&pExpandedOutput[8],
						 (uint8_t*)&ResponseId,0x04);
  memUtils_memReverseCopy(&pExpandedOutput[12],
						 (uint8_t*)&ResponseId,0x04);

  if( keyIndex == g_ZLL_KEY_BITMASK_DEVELOPMENT_KEY_c )
  {
    uint8_t developmentKey[16] = {g_ZLL_DEVELOPMENT_KEY_c};
    memUtils_memCopy(currentKey,
                     developmentKey,g_ZLL_NETWORK_KEY_SIZE_c);
  }
  else if( keyIndex == g_ZLL_KEY_BITMASK_CERTIFICATION_KEY_c )
  {
    uint8_t certificationKey[16] = {g_ZLL_CERTIFICATION_KEY_c};
    memUtils_memCopy(currentKey,
                     certificationKey,g_ZLL_NETWORK_KEY_SIZE_c);
  }
  else if ( keyIndex ==  g_ZLL_KEY_BITMASK_MASTER_KEY_c )
  {
    uint8_t masterKey[16] = {g_ZLL_MASTER_KEY_c};
    memUtils_memCopy(currentKey,
                     masterKey,g_ZLL_NETWORK_KEY_SIZE_c);
  }
  rsi_802154_hal_set_aes_key(currentKey);
  uint8_t payload_length = g_ZLL_NETWORK_KEY_SIZE_c;
  Encrypt_Decrypt_Using_ECB_Mode(  &payload_length, pExpandedOutput,
                                 encryptedTransportKey, g_TRUE_c );
  memUtils_memCopy(pExpandedOutput,
						 (uint8_t*)&encryptedTransportKey,g_ZLL_NETWORK_KEY_SIZE_c);
}

/*****************************************************************************/
/* End of Inter-pAN Commands frame creation */

/*****************************************************************************
* Private Functions
*****************************************************************************/

/* None */

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */

/*****************************************************************************
* End Of File                                    ZLL_Interpan.c
*****************************************************************************/
