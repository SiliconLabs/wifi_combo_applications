/*******************************************************************************
* AppZllCommissioningCluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting ZLL's
*   Commissioning cluster
*
*******************************************************************************/

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#ifndef	STM32L1XX_MD_PLUS
//#include PLATFORM_HEADER
//#include "hal/hal.h"
//#include "phy-library.h"
//#else
  //#include "simplemac_library.h"
//#endif /*STM32L1XX_MD_PLUS */

#include "stack_common.h"
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
#include "sw_timer.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "zdo_interface.h"
#include "zdp_interface.h"
#include "apsde_interface.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "stack_functionality.h"
#include "ZCL_Foundation.h"
#include "commissioning_support.h"

#if(g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1)
    #include "ZLL_Commissioning_Cluster.h"
    #include "AppZllCommissioningClusterUtilities.h"
    #include "ApplicationThinLayer.h"
    #include "ZLL_Network.h"
#endif /* (g_ZLL_COMMISSIONING_CLUSTER_ENABLE_d == 1)*/
#include "app_configuration_default.h"
#if g_ZLL_LIGHT_CONTROLLER_c
#include "ZLL_Controller_Application_Framework.h"
#endif /* g_ZLL_LIGHT_CONTROLLER_c */

Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c+2];
Endpoint_Description_t *gp_Endpoint_Descriptors = ga_Endpoint_Descriptors;
/*******************************************************************************
* Global Constants
*******************************************************************************/



/*******************************************************************************
* Public Memory declarations
*******************************************************************************/



#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif
bool SentEndpointInfo = g_FALSE_c;
Get_end_point_list_response_field   get_end_point_list_response_field;
/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
static void SendZLLCommissioningClusterResponse(APSDE_Data_Indication_t *pBuffer,
                uint8_t command_Id,uint8_t status ,uint8_t response_len,
                uint8_t *pResponse);
#endif

/*******************************************************************************
* Public Functions
*******************************************************************************/

void App_Handle_ZLL_Commissioning_Cluster_Events( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer )
{

    switch( pZCLEvent->eventId )
    {

#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)

    case g_ZLL_Get_Group_Identifier_Req_Event_c:{
            uint8_t status = g_ZCL_Invalid_Field_c , response_len = 0x00;

            Get_group_identifier_response_field group_id_response;

            if(pZCLEvent->eventLength == sizeof(Get_end_point_record_list_field)){

              /* maintenance of group ids is not clear in spec*/
              group_id_response.count = ZLL_groups.device_end_group_id -
                ZLL_groups.device_start_group_id;
              group_id_response.start_index = 0x00;
              group_id_response.total = g_ZLL_TOTAL_GROUP_IDENTIFIER_NUMBER_c;
              if(group_id_response.count == 0x01){
                group_id_response.group_information_record_list[0].group_identifier = ZLL_groups.device_start_group_id;
                /* according to spec it is zero*/
                group_id_response.group_information_record_list[0].group_type = 0x00;
                response_len = sizeof(Get_group_identifier_response_field);
              }
              else if(group_id_response.count == 0x00){
                response_len = sizeof(Get_group_identifier_response_field) -
                  sizeof(Group_information_record_list);
              }
              else{
                /* spec is not clear*/
              }

              status = g_ZCL_Success_c;
            }
            else{
              /* If request,does not contain valid param*/
              status = g_ZCL_Invalid_Field_c;
              response_len = 0x00;
            }
              SendZLLCommissioningClusterResponse(pBuffer,
                g_ZLL_Get_Group_Identifiers_Response_Command_c,status ,response_len,
                (uint8_t*)&group_id_response);

            }
            break;
        case g_ZLL_Get_Endpoint_List_Req_Event_c:{
            uint8_t status = g_ZCL_Invalid_Field_c;


            Get_end_point_list_response_field response;
            /*uint8_t total;
              uint8_t start_index;
              uint8_t count;*/
            uint8_t fixed_payload_len = 0x03;
            /* total response length*/
            uint8_t response_len = 0x00;
            response.count = 0x00;

            if(pZCLEvent->eventLength == sizeof(Get_end_point_record_list_field)){
                /* start index*/
                uint8_t start_index = *(pZCLEvent->pEventData);
                /* check for valid start index*/


                if(gp_Endpoint_Descriptors[start_index ].endpoint_id == 0xfe){
                    status = g_ZCL_Invalid_Value_c;
                }
                else{
                  response.start_index = start_index;

                  for(uint8_t loop_index = start_index; loop_index < g_MAX_NO_OF_ENDPOINTS_c+2;
                    loop_index++){

                    /* check for invalid endpoint */
                    if(gp_Endpoint_Descriptors[loop_index].endpoint_id != 0xfe &&

                     gp_Endpoint_Descriptors[loop_index].p_simple_desc != g_NULL_c ){

                      /* device id*/
                      response.end_point_information_record_list[response.count].device_identifier =
                        gp_Endpoint_Descriptors[loop_index].p_simple_desc->app_device_id;
                      /*endpoint id */
                      response.end_point_information_record_list[response.count].end_point_identifier=
                         gp_Endpoint_Descriptors[loop_index].endpoint_id;
                      /*network address */
                      response.end_point_information_record_list[response.count].network_address =
                        ZLL_DeviceNetworkAddress();
                      /*profile ID */
                      response.end_point_information_record_list[response.count].profile_identifier =
                        g_HOME_AUTOMATION_PROFILE_ID_c;
                      /*Device version*/
                      response.end_point_information_record_list[response.count].version =
                        gp_Endpoint_Descriptors[loop_index].p_simple_desc->app_device_version;
                      /* for each endpoint parsed , count will be incremented*/
                      response.count++;
                      response_len = fixed_payload_len +
                        sizeof(End_point_information_record_list)*response.count;
                     }
                  }/* end of for*/
                  response.total = g_MAX_NO_OF_ENDPOINTS_c;
                  status = g_ZCL_Success_c;
                }
            }
            else{
                status = g_ZCL_Invalid_Field_c;
                response_len = 0x00;
            }

            SendZLLCommissioningClusterResponse(pBuffer,
                g_ZLL_Get_Endpoint_List_Response_Command_c,status ,response_len,
                (uint8_t*)&response);
          }
          break;

#endif /* g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 */

#if(g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)


        case g_ZLL_Endpoint_Information_Response_Event_c:
          /* Application need to handle*/
            break;
        case g_ZLL_Get_Group_Identifiers_Response_Event_c:
          /* Application need to handle*/
            break;
        case g_ZLL_Get_Endpoint_List_Response_Event_c:
          /* Application need to handle*/
            break;

#endif /* g_ZLL_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d*/
        default :
            break;

    }
}

#ifdef g_ZLL_LIGHT_CONTROLLER_c
/*****************************************************************************/

void ZLL_InitiateEndpointNotification(uint16_t dest_addr)
{

    /* create and send the endpoint information to the dest device*/
    /* Since, One node can support many devices and it is user configurable,
    notification of all endpoints(devices) will be sent*/
    static uint8_t loop_index = 0x00;

    for(; loop_index < g_MAX_NO_OF_ENDPOINTS_c+2; ){

//      if(SentEndpointInfo == g_TRUE_c){
//          /* wait until confirmation is received for prev data request,
//          i.e., unil SentEndpointInfo == g_FALSE_c*/
//          continue;
//      }else{

          ZLL_CreateSendEndpointInfo(dest_addr,loop_index);
          loop_index++;
     // }
    }/* end of for loop*/
    if(loop_index == g_MAX_NO_OF_ENDPOINTS_c+2){
        loop_index = 0x00;
    }
}
/*****************************************************************************/

void ZLL_CreateSendEndpointInfo(uint16_t dest_addr ,uint8_t loop_index)
{
    End_point_information_response_field endpointInfo;


    uint8_t bufferIndex = 0x00;
    APSDE_Request_t* pDataReq = g_NULL_c;
    APSDE_Data_Request_t * pAppDataReq = g_NULL_c;

    uint8_t payloadLength = 0x00;

    /* check the state of the device */
    if ((rsi_zigb_network_state() == g_ZigBeeNotPartOfNWK_c)
        || (rsi_zigb_network_state() == g_ZigBeePerformingLeaveFromNWK_c)){

        /* device is factory new device*/
        return;
    }

    if((gp_Endpoint_Descriptors[loop_index].endpoint_id != 0xfe )&&
       (gp_Endpoint_Descriptors[loop_index].p_simple_desc != g_NULL_c )){


        bufferIndex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
        if( g_NO_AVAILABLE_BUFFER_c != bufferIndex ) {
#if (DEBUG_TEST == 1)
            /* Buffer ID --> 88 */
            allocatedbuffer[bufferIndex]= 88;
            freebuffer[bufferIndex][0]= 88;
            freebuffer[bufferIndex][1]= 0;
#endif
            pDataReq =
                (APSDE_Request_t *)buffMgmt_getBufferPointer( bufferIndex );

            pDataReq->msgType = g_APSDE_Data_Request_c;
            pAppDataReq =
                (APSDE_Data_Request_t *)&((pDataReq)->APSDE_Request_Primitive.apsde_data_request);



            pAppDataReq->dest_addr_mode = g_SHORT_ADDR_MODE_c;
            if(dest_addr != 0xffff)
              pAppDataReq->dest_address.short_address = dest_addr;

            pAppDataReq->profile_id = g_APP_PROFILE_ID_c;
            pAppDataReq->cluster_id = g_ZLL_COMMISSIONING_CLUSTER_c;


            /* ZCL Header Creation */
            pAppDataReq->asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                    g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                        g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
            pAppDataReq->asdu[1] = App_GetTransSeqNumber();
            pAppDataReq->asdu[2] = g_ZLL_Endpoint_Information_Response_Command_c;


            payloadLength = 3;


            /* extended address */
            memUtils_memCopy((uint8_t*)&(endpointInfo.IEEE_address[0]),
              rsi_zigb_get_self_ieee_address(),g_ZLL_EXTENDED_PANID_SIZE_c );
            payloadLength  += g_ZLL_EXTENDED_PANID_SIZE_c ;

            endpointInfo.network_address = ZLL_DeviceNetworkAddress();
            payloadLength  += g_ZLL_SHORT_PANID_SIZE_c ;

            endpointInfo.end_point_identifier = gp_Endpoint_Descriptors[loop_index].endpoint_id;
            payloadLength++;

            endpointInfo.profile_identifier = g_HOME_AUTOMATION_PROFILE_ID_c;
            payloadLength += 0x02;

            endpointInfo.device_identifier =
              gp_Endpoint_Descriptors[loop_index].p_simple_desc->app_device_id;
            payloadLength += 0x02;

            endpointInfo.version =
              gp_Endpoint_Descriptors[loop_index].p_simple_desc->app_device_version;
            payloadLength++;

            memUtils_memCopy((uint8_t*)&(pAppDataReq->asdu[3]),(uint8_t*)&endpointInfo,
                             sizeof(End_point_information_response_field));


            pAppDataReq->asdulength = payloadLength;
            pAppDataReq->txoptions =  App_getClusterSecurity( g_ZLL_COMMISSIONING_CLUSTER_c );
            pAppDataReq->radius = g_DEFAULT_RADIUS_c;

            SentEndpointInfo = g_TRUE_c;
            //APS_APSDErequest( bufferIndex );
            APS_APSDE_request(bufferIndex,pAppDataReq );

        }
    }/* End of if*/

}
#endif /* #ifdef g_ZLL_LIGHT_CONTROLLER_c*/



/*******************************************************************************
* Private Functions
*******************************************************************************/
#if(g_ZLL_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
static void SendZLLCommissioningClusterResponse(APSDE_Data_Indication_t *apsdeDataPacket,
                uint8_t command_Id,uint8_t status ,uint8_t response_len,
                uint8_t *pResponse)
{

    uint8_t destAddress[8];
    uint8_t ZCL_payload_offset = 0x03;
    /* ZCL Header Creation */
    apsdeDataPacket->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                            g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = command_Id;

    if(status == g_ZCL_Success_c){
      memUtils_memCopy(destAddress , apsdeDataPacket->src_address.IEEE_address, 0x08 );

      /* copy response payload */
      if(pResponse != g_NULL_c){
        memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[ZCL_payload_offset]),
                         pResponse, response_len);
      }
      App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_ZLL_COMMISSIONING_CLUSTER_c, response_len + ZCL_payload_offset/* ZCL header*/,
        apsdeDataPacket->a_asdu );

    }
    else{

      /*default response with status */
      App_SendDefaultResponse(status ,g_TRUE_c,apsdeDataPacket );

    }
}
#endif


/*******************************************************************************
                                End of AppZllCommissioningCluster
*******************************************************************************/
