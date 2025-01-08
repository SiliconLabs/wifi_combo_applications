/*******************************************************************************
* AppApplianceStatisticsUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Appliance
*   Statistics Cluster
*
*******************************************************************************/

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include <stdlib.h>
#include <string.h>
#include "stack_common.h"
//#include "nvm.h"
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
#include "sw_timer.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "zdo_interface.h"
#include "apsde_interface.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "stack_functionality.h"
#include "ZCL_Foundation.h"
#include "commissioning_support.h"

#if ( g_APPLIANCE_STATISTICS_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HA_Appliance_Statistics_Cluster.h"
#include "AppZclDefaultAttributesData.h"
#include "AppApplianceStatisticUtilities.h"
#include "ApplicationThinLayer.h"
#endif
#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)
#define g_LOG_QUEUE_TABLE_SIZE_c        (g_LOG_QUEUE_MAX_SIZE_DEFAULT_DATA_c + 2)
    /* This Payload length is application dependent ,but max size is 40 bytes
        as per spec */
#define g_LOG_PAYLOAD_LENGTH_c          (10)
#endif
/*******************************************************************************
* Global Constants
*******************************************************************************/



/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)


uint8_t logInfo[g_LOG_QUEUE_TABLE_SIZE_c][g_LOG_PAYLOAD_LENGTH_c] = {
    {"start"},
    {"stop"},
    {"pause"}
};

DefaultLogResponse_t defaultLogResponse[g_LOG_QUEUE_TABLE_SIZE_c] =
{
    /* UTC Time , LogId , LogLen, Log Data */

    {{0x05,0x00,0x00,0x00},{0x01,0x00,0x00,0x00},{0x05,0x00,0x00,0x00},{g_NULL_c}},
    {{0x0a,0x00,0x00,0x00},{0x02,0x00,0x00,0x00},{0x04,0x00,0x00,0x00},{g_NULL_c}},
    {{0x0f,0x00,0x00,0x00},{0x03,0x00,0x00,0x00},{0x05,0x00,0x00,0x00},{g_NULL_c}}
};


LogResponseCommandPayload_t logResponseQueue[g_LOG_QUEUE_TABLE_SIZE_c] ;

uint8_t *pResPayload = g_NULL_c;
uint8_t GlobalPayloadLength = 0;
uint8_t QueueSize = 0;


static uint8_t LogExtractingIndex = 0,LogStoringIndex = 0;

#endif /* (g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)*/


#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)
static bool IsRequestedLogEntryPresent(uint8_t logId[],uint8_t *pIndex);
static bool IsValidLogEntryPresent(void);
void SendApplianceStatisticResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status, uint8_t*pResponsePayload ,
    uint8_t responseLen,uint8_t bufferToBeFreed );

static uint8_t FillAvailableLogIdInResponse(void);
static uint8_t InsertLogEntryInQueueTable(void);
#endif

/*******************************************************************************
* Public Functions
*******************************************************************************/
#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)
void App_ApplianceStatisticReset(void)
{
    /* Log Entry Tracker*/
    LogExtractingIndex = 0,LogStoringIndex = 0;

    /* Resets Log Queue Table*/
    memUtils_memSet((uint8_t*)&logResponseQueue,0xff,
        sizeof(LogResponseCommandPayload_t)*g_LOG_QUEUE_TABLE_SIZE_c);

}
#endif

/******************************************************************************/

void App_ApplianceStatisticEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer )
{

#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)

    uint8_t commandLength = 0 ,responseLen = 0;
    uint8_t status = g_ZCL_Success_c,bufferIndex =0;
    uint32_t tempLength = 0;
    uint8_t tempBuffer[60],tempBufferIndex = 0;

    uint8_t tempLogId[4] ,index = 0,*pResponsePayload = g_NULL_c;
#endif

    switch ( pZCLEvent->eventId )
    {

        /* Request received by the server */
#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)

        /* Used to retrieve the log avaliable in server side of the appliable
        cluster about the appliance. if log entry associated with log ref id is
        not availabe its response is log NOT_FOUND else log entry is returned */
        case g_ApplianceStatisticsLogRequestEvent_c:{

            commandLength = sizeof(LogRequestPayload_t);

            if( pZCLEvent->eventLength == commandLength ){



                memUtils_memCopy(tempLogId,pZCLEvent->pEventData,0x04 );

                /* Find whether requested logId entry is present in server's
                log Queue table */
                if(IsRequestedLogEntryPresent(tempLogId,&index)){
                    /* Log Entry is present , send log Response */
                    status = g_ZCL_Success_c;
                    /*extract log entry from the log queue table */
                    pResponsePayload = (uint8_t*)&logResponseQueue[index];

                    /* place holder size is 1 byte*/
                    tempBufferIndex = sizeof(LogResponseCommandPayload_t)-0x01 ;
                    memUtils_memCopy(tempBuffer,pResponsePayload,tempBufferIndex);



                    /* adding response length which is 4 byte in length*/
                    tempLength += logResponseQueue[index].logLength[3];
                    tempLength += logResponseQueue[index].logLength[2];
                    tempLength += logResponseQueue[index].logLength[1];
                    tempLength += logResponseQueue[index].logLength[0];

                    memUtils_memCopy((tempBuffer+tempBufferIndex),
                                     (uint8_t*)(&logInfo[index]),
                                     strlen((char const*)logInfo[index]));


                    /* place holder size is 1 byte and string lenght*/
                    responseLen = sizeof(LogResponseCommandPayload_t) - 0x01 +
                        tempLength;

                }
                else{
                    /* Specified log entry is not present*/
                    status = g_ZCL_Not_Found_c;

                    /* call default response with status */
                    App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
                    return;
                }
                bufferIndex = g_NO_AVAILABLE_BUFFER_c;
                SendApplianceStatisticResponse(pBuffer,(uint8_t )g_LogResponseCommand_c,
                                     status,tempBuffer,responseLen,bufferIndex);
            }
            else{
                /* Request is improper */
                status = g_ZCL_Malformed_Command_c;
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;
        case g_ApplianceStatisticsLogQueueRequestEvent_c:{

            commandLength = 0x00; /* No Command Parameters for Log Queue req*/

            if( pZCLEvent->eventLength == commandLength ){


                bufferIndex = FillAvailableLogIdInResponse();

                status = g_ZCL_Success_c;
                SendApplianceStatisticResponse(pBuffer,(uint8_t )g_LogQueueResponseCommad_c,
                                    status,pResPayload,GlobalPayloadLength ,bufferIndex);

            }
            else{
                /* Request is improper */
                status = g_ZCL_Malformed_Command_c;
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }

        }
        break;
#endif  /*(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)*/
#if(g_APPLIANCE_STATISTICS_CLIENT_CLUSTER_ENABLE_d == 1)

        case g_ApplianceStatisticsLogNotificationEvent_c:{
            /* Application Should handle Reception  */
        }
        break;

        case g_ApplianceStatisticsLogResponseEvent_c:{
            /* Application Should handle Reception */
        }
        break;

        case g_ApplianceStatisticsLogQueueEvent_c : {
            /* Application Should handle Reception */
        }
        break;

        case g_ApplianceStatisticsStatisticsAvailableEvent_c:{
            /* Application Should handle Reception */
        }
        break;

#endif  /*(g_APPLIANCE_STATISTICS_CLIENT_CLUSTER_ENABLE_d == 1) */
        default :
        break;

    }

}


/******************************************************************************/

#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)
void SendApplianceStatisticResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status, uint8_t*pResponsePayload ,
    uint8_t responseLen,uint8_t bufferToBeFreed )
{

    uint8_t destAddress[8];
    uint8_t payloadLength = 0;
    /* ZCL Header Creation */
    apsdeDataPacket->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                            g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = responseType;
    apsdeDataPacket->a_asdu[3] = status;

    payloadLength += 4;
    memUtils_memCopy(destAddress , apsdeDataPacket->src_address.IEEE_address,
                     0x08 );

    if( status == g_ZCL_Success_c) {
        if(pResponsePayload != g_NULL_c && responseLen != 0 ) {
            memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[4]),
                             pResponsePayload, responseLen);
            payloadLength += responseLen;

            if(bufferToBeFreed != 0xff){
                /* This buffer contents is copied into apsdeBuffer,
                Hence this buffer need to be cleared*/
                buffMgmt_freeBuffer(bufferToBeFreed,236);
            }
        }
    }

    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_APPLIANCE_STATISTICS_CLUSTER_c,payloadLength,
        apsdeDataPacket->a_asdu );

}


/******************************************************************************/
void InvokeLogNotificationCommand(void)
{

    uint8_t logLength = 0;
    /* This function inserts the log entry from default table to actual
    queue table*/
    logLength = InsertLogEntryInQueueTable();


    uint8_t bufferIndex = 0x00,tempIndex = 0;
    APSDE_Data_Request_t *p_apsde_data_request = g_NULL_c;



    if(LogStoringIndex != 0x00 && LogStoringIndex <= g_LOG_QUEUE_TABLE_SIZE_c ){

        tempIndex = LogStoringIndex - 0x01;
    }else{
        tempIndex = LogStoringIndex;
    }

    bufferIndex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

    if( g_NO_AVAILABLE_BUFFER_c != bufferIndex ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 73 */
        allocatedbuffer[bufferIndex]= 73;
        freebuffer[bufferIndex][0]= 73;
        freebuffer[bufferIndex][1]= 0;
#endif
        p_apsde_data_request = ( APSDE_Data_Request_t * )buffMgmt_getBufferPointer( bufferIndex );
        *((uint8_t*)p_apsde_data_request) = g_APSDE_Data_Request_c;

        p_apsde_data_request = (APSDE_Data_Request_t *)&(((APSDE_Request_t *)p_apsde_data_request)->
                                                   APSDE_Request_Primitive.apsde_data_request);


        p_apsde_data_request->dest_addr_mode = g_SHORT_ADDR_MODE_c;
        p_apsde_data_request->dest_address.short_address   = 0xffff;
        p_apsde_data_request->profile_id     = g_APP_PROFILE_ID_c;
        p_apsde_data_request->src_endpoint   = ZCL_Callback_Get_Device_EndPoint();
        p_apsde_data_request->cluster_id     = g_APPLIANCE_STATISTICS_CLUSTER_c;


        p_apsde_data_request->asdulength     = 0x04 + logLength;

        memUtils_memCopy( &p_apsde_data_request->asdu[4],
                         (uint8_t*)&logResponseQueue[tempIndex],logLength );

        memUtils_memCopy( &p_apsde_data_request->asdu[4+logLength],
                         logInfo[tempIndex],
                 strlen((char const*)logInfo[tempIndex] ));


        p_apsde_data_request->asdulength  += strlen((char const*)logInfo[tempIndex] );



        p_apsde_data_request->txoptions =  App_getClusterSecurity( g_APPLIANCE_STATISTICS_CLUSTER_c );
        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;


        p_apsde_data_request->asdu[0]        = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
                                           g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                           g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        p_apsde_data_request->asdu[1]        = App_GetTransSeqNumber();
        p_apsde_data_request->asdu[2]        = g_LogNotificationCommand_c;
        p_apsde_data_request->asdu[3]        = g_ZCL_Success_c;




       // APS_APSDErequest( bufferIndex );
        APS_APSDE_request(bufferIndex,p_apsde_data_request );
    }
}




/******************************************************************************/
void InvokeStatisticsAvailableCommand(void)
{
    uint8_t bufferIndex = 0x00,bufferAllotedSize = 0xff;
    APSDE_Data_Request_t *p_apsde_data_request = g_NULL_c;


    bufferAllotedSize = FillAvailableLogIdInResponse();

    if(bufferAllotedSize != g_NO_AVAILABLE_BUFFER_c ){

        bufferIndex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

        if( g_NO_AVAILABLE_BUFFER_c != bufferIndex ) {
#if (DEBUG_TEST == 1)
            /* Buffer ID --> 73 */
            allocatedbuffer[bufferIndex]= 73;
            freebuffer[bufferIndex][0]= 73;
            freebuffer[bufferIndex][1]= 0;
#endif
            p_apsde_data_request = ( APSDE_Data_Request_t * )buffMgmt_getBufferPointer( bufferIndex );
            *((uint8_t*)p_apsde_data_request) = g_APSDE_Data_Request_c;

            p_apsde_data_request = (APSDE_Data_Request_t *)&(((APSDE_Request_t *)p_apsde_data_request)->
                                                       APSDE_Request_Primitive.apsde_data_request);


            p_apsde_data_request->dest_addr_mode = g_SHORT_ADDR_MODE_c;
            p_apsde_data_request->dest_address.short_address   = 0xffff;
            p_apsde_data_request->profile_id     = g_APP_PROFILE_ID_c;
            p_apsde_data_request->src_endpoint   = ZCL_Callback_Get_Device_EndPoint();
            p_apsde_data_request->cluster_id     = g_APPLIANCE_STATISTICS_CLUSTER_c;
            p_apsde_data_request->asdulength     = 0x04 + GlobalPayloadLength;

            memUtils_memCopy( &p_apsde_data_request->asdu[4],pResPayload,GlobalPayloadLength  );
            p_apsde_data_request->txoptions =  App_getClusterSecurity( g_APPLIANCE_STATISTICS_CLUSTER_c );
            p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;

            /* This buffer contents is copied into apsdeBuffer,
                Hence this buffer need to be cleared*/
            buffMgmt_freeBuffer(bufferAllotedSize,236);

      }else {
            p_apsde_data_request->asdu[4] = *pResPayload;
            p_apsde_data_request->asdulength = 0x04 +0x01;
      }


        p_apsde_data_request->asdu[0]        = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
                                               g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                               g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        p_apsde_data_request->asdu[1]        = App_GetTransSeqNumber();
        p_apsde_data_request->asdu[2]        = g_StatisticsAvailableCommand_c;
        p_apsde_data_request->asdu[3]        = g_ZCL_Success_c;

        //APS_APSDErequest( bufferIndex );
        APS_APSDE_request(bufferIndex,p_apsde_data_request );
    }


}

#endif   /* g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1*/
/*******************************************************************************
* Private Functions
*******************************************************************************/
#if(g_APPLIANCE_STATISTICS_SERVER_CLUSTER_ENABLE_d == 1)
static bool IsRequestedLogEntryPresent(uint8_t logId[],uint8_t *pIndex)
{
    for( uint8_t index = 0; index < g_LOG_QUEUE_TABLE_SIZE_c ; index++){
        if(g_MEMORY_MATCH_c == (memUtils_memCompare (logId,
                    logResponseQueue[index].logId,0x04 ))){
            *pIndex = index;
            /* Log Entry present in Log Queue Table*/
            return g_TRUE_c;
        }
    }
    return g_FALSE_c;
}
/******************************************************************************/

static bool IsValidLogEntryPresent(void)
{
    uint8_t invalidLogId[4] = {0xff,0xff,0xff,0xff};
    for( uint8_t index = 0; index < g_LOG_QUEUE_TABLE_SIZE_c ; index++){
        if( g_MEMORY_MATCH_c != (memUtils_memCompare(invalidLogId,
                logResponseQueue[index].logId ,0x04))){
            /* Valid Log Entry present*/
            return g_TRUE_c;
        }
    }
    return g_FALSE_c;
}

/******************************************************************************/
static uint8_t FillAvailableLogIdInResponse(void)
{
    uint8_t bufferIndex = 0;
    uint8_t invalidLogId[4] = {0xff,0xff,0xff,0xff};
    uint8_t validEntryCount = 0;



    LogQueueResponseCommandPayload_t *pLogQueueResponse = g_NULL_c;
    if(IsValidLogEntryPresent()) {

        bufferIndex = buffMgmt_allocateBuffer(g_SMALL_BUFFER_c);
        if( g_NO_AVAILABLE_BUFFER_c == bufferIndex ) {
            return g_NO_AVAILABLE_BUFFER_c;
        }
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 74 */
        allocatedbuffer[bufferIndex] = 74;
        freebuffer[bufferIndex][0] = 74;
        freebuffer[bufferIndex][1] = 0;
#endif
        /* Inorder to append logId's*/
        pLogQueueResponse =
            (LogQueueResponseCommandPayload_t*)buffMgmt_getBufferPointer(bufferIndex);


        for( uint8_t index = 0 ;index < g_LOG_QUEUE_TABLE_SIZE_c;
            index++){

                /* If log entry is valid then copy the log id*/
            if( g_MEMORY_MATCH_c != (memUtils_memCompare(invalidLogId,
                logResponseQueue[index].logId ,0x04))){

                memUtils_memCopy(&(pLogQueueResponse->logId[index*4]) ,
                    logResponseQueue[index].logId ,0x04);
                validEntryCount++;
            }
        }
        pLogQueueResponse->logQueueSize = validEntryCount;

        if(pLogQueueResponse->logQueueSize > g_LOG_QUEUE_TABLE_SIZE_c){
            /* Application should handle fragmentation of the payload
            */
        }

        pResPayload = (uint8_t*)pLogQueueResponse;
        /* logQueueSize + validLogId's*/
        GlobalPayloadLength = 0x01 + (validEntryCount *4);

    }
    else{
        /* Here Queue Response is sent with only queue size as
        payload , LogId's are not appended*/
        QueueSize = 0;

        pResPayload = &QueueSize;

        GlobalPayloadLength = 0x01;
        bufferIndex = g_NO_AVAILABLE_BUFFER_c;
    }
    return bufferIndex;
}

/******************************************************************************/
static uint8_t InsertLogEntryInQueueTable(void)
{
    uint8_t logEntryLength = 0;

    /* Ox01 is place holder for string*/
    logEntryLength = sizeof(DefaultLogResponse_t) - 0x01;

    memUtils_memCopy((uint8_t*)&(logResponseQueue[LogStoringIndex]),
                     (uint8_t*)&defaultLogResponse[LogExtractingIndex],
        logEntryLength );

    LogExtractingIndex++;
    LogStoringIndex++;

    if(LogExtractingIndex > g_LOG_QUEUE_TABLE_SIZE_c){
        /* to rollback */
        LogExtractingIndex = 0;
    }

    if(LogStoringIndex > g_LOG_QUEUE_TABLE_SIZE_c){
        /* to rollback */
        LogStoringIndex = 0;
    }
    return logEntryLength;
}
#endif
/******************************************************************************/

                            /* end of AppApplianceStatisticsUtilities.c*/
