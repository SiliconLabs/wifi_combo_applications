/*******************************************************************************
* AppApplianceControlUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Appliance
*   Control Cluster
*
*   Assumptions for current Implemetation:
*   ======================================
*   There are three attributes for this cluster
*   a.Start Time
*   b.Finish Time
*   c.Remaining Time
*
*   There is an execution cmd with start , stop ,pause and many more activities
*   through which activity can be initiated .
*
*   Execution cmd is used to initiate activity manually ex: if we give
*   execute command with start option, activity will be started and it is not
*   dependent on any attribute (ex:start Time ).
*   Note : Manual Operation :Initiating operation via execute command.
*
*
*   Activity
*   ------------
*   1.Start
*   2.Stop
*   3.Over load Pause
*   4.Over load Resume
*
*
*   It is assumed that start time Finish Time are meant for automatic
*   start/finish of operation.
*   For ex : When device is set with Start Time ,Finish Time
*       Then  Activity is supposed to start at Start Time and supposed to
*       Finish at Finish Time without intervention of commands.
*
*
*******************************************************************************/

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stack_common.h"
#include "ZigBeeDataTypes.h"
#include <stdlib.h>
#include <string.h>

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


#if(g_EN50523_APPLIANCE_CONTROL_CLUSTER_ENABLE_d == 1)
#include "AppZclDefaultAttributesData.h"
#include "ApplicationThinLayer.h"
#include "ZCL_General_Appliance_Control_Cluster.h"
#include "AppApplianceControlUtilities.h"
#endif

#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
#define g_THIRTY_SECONDS_c	                            (30)
#define g_ONE_DAY_IN_SECONDS_c                          (86400)
#define g_ONE_HOUR_IN_SECONDS_c                         (3600)
#define g_ONE_MIN_IN_SECONDS_c                          (60)

#endif
/*******************************************************************************
* Global Constants
*******************************************************************************/



/*******************************************************************************
* Public Memory declarations
*******************************************************************************/
#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
/* changes value according to execution command and state of the appliance */
Appliance_Status CurrentStatusOfAppliance = g_STAND_BY_STATE_c;


uint32_t ActualStartTime = 0x00000000, ActualFinishTime = 0x00000000,
ActualRemainingTime = 0x00000000 ;
Appliance_Status PreviousStatusOfAppliance = g_STAND_BY_STATE_c;
#endif

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


#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
static void processExecutionCommand(uint8_t commandId);
static void parseTheTimeAttrAndConverToSecond(uint32_t *programmedTime);
static void App_sendAttributeReports( uint16_t attributeId,
        uint16_t attributeData,uint8_t attributeDataType,
        APSDE_Data_Indication_t* pBuffer);
static void updateServerAttribute(uint32_t programmedTime,uint16_t attributeId);
#endif /* g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1*/
/*******************************************************************************
* Public Functions
*******************************************************************************/

void App_ApplianceControlEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer )
{

#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
    uint8_t commandLength = 0x00,status = 0x00;
    SignalStateResponseCommandPayload_t signal_state_response;
    SignalStateNotificationCommandPayload_t signal_state_notification;
    uint8_t *pResponsePayload = g_NULL_c, responseLen = 0;
    uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
    WriteFunctionCommandPayload_t write_command_payload[20];
#endif

    switch ( pZCLEvent->eventId )
    {

        /* Request received by the server */
#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
        case g_ApplianceControlExecutionEvent_c:{

            commandLength = sizeof(ExecutionOfACommandPayload_t);

            if(pZCLEvent->eventLength == commandLength ){
                /* First level of validity*/
                if(pZCLEvent->pEventData[0] >= g_START_c ||
                   pZCLEvent->pEventData[0] <= g_ENABLE_GAS_c ){
                    /* Process execution Command */
                    processExecutionCommand(pZCLEvent->pEventData[0]);
                    status = g_ZCL_Success_c;

                    signal_state_notification.applianceStatus = CurrentStatusOfAppliance;


                    /* Following parameter is actual application dependent,
                        for now temporary value is assigned */
                    signal_state_notification.remoteEnableFlagsAndDeviceStatus2 =
                        g_REMOTE_DISABLED_c;


                    /* Following parameter is actual application dependent,
                        for now temporary value is assigned */
                    signal_state_notification.applianceStatus2[0] = 0xff;
                    signal_state_notification.applianceStatus2[1] = 0xff;
                    signal_state_notification.applianceStatus2[2] = 0xff;

                    pResponsePayload = (uint8_t*)&signal_state_notification;
                    responseLen =
                        sizeof(SignalStateNotificationCommandPayload_t);

                    SendApplianceControlStatus(pBuffer,(uint8_t)g_SignalStateNotificationCommand_c,
                        status,pResponsePayload,responseLen );
                }
                else {
                    status = g_ZCL_Invalid_Value_c;
                    /* call default response with status */
                    App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
                }
            }
            else{
                /* Request is improper */
                status = g_ZCL_Malformed_Command_c;
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;

        case g_ApplianceControlSignalStateEvent_c:{

            /* No Command Parameters for Signal State Request */
            commandLength = 0x00;

            if( pZCLEvent->eventLength == commandLength ){

                status = g_ZCL_Success_c;

                signal_state_response.applianceStatus = CurrentStatusOfAppliance;


                /* Following parameter is actual application dependent,
                    for now temporary value is assigned */
                signal_state_response.remoteEnableFlagsAndDeviceStatus2 =
                    g_REMOTE_DISABLED_c;


                /* Following parameter is actual application dependent,
                    for now temporary value is assigned */
                signal_state_response.applianceStatus2[0] = 0xff;
                signal_state_response.applianceStatus2[1] = 0xff;
                signal_state_response.applianceStatus2[2] = 0xff;

                pResponsePayload = (uint8_t*)&signal_state_response;
                responseLen = sizeof(SignalStateResponseCommandPayload_t);

                SendApplianceControlStatus(pBuffer,(uint8_t)g_SignalStateResponseCommand_c,
                        status,pResponsePayload,responseLen );

            }
            else{
                /* Request is improper */
                status = g_ZCL_Malformed_Command_c;
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer);
            }

        }
        break;

        case g_ApplianceControlWriteFunctionEvent_c : {

            uint8_t expectedAttributeLength = 0;
            uint16_t attributeId,attributeData;

            /* Command Format is :
            Attribute Id (2bytes) , Attribute Datatype(1),Variable data length*/

            expectedAttributeLength = ZCL_Get_Data_length(pZCLEvent->pEventData[2]);


            memUtils_memCopy((uint8_t*)&(write_command_payload) ,
                             (uint8_t*)&(pZCLEvent->pEventData[0]),
                         pZCLEvent->eventLength);


            /* Since Attribute Id for*/
            if( pZCLEvent->eventLength == sizeof(WriteFunctionCommandPayload_t)-
                        0x01 + expectedAttributeLength){

                memUtils_memCopy((uint8_t*)&attributeId,
                                 (uint8_t*)&(pZCLEvent->pEventData[0]),0x02);

                memUtils_memCopy((uint8_t*)&attributeData,
                                 (uint8_t*)&(pZCLEvent->pEventData[3]),0x02);

                if(g_ZCL_Success_c == App_ReadWriteAttribute( EndPoint,
                                       g_APPLIANCE_CONTROL_CLUSTER_c,
                                       attributeId,
                                       (uint8_t*)&attributeData,
                                       g_Write_Attribute_Data_c )){
                    /* Changing the Appliance state as Programmed state */
                    /* Atleast start time shd be programmed */
                   if(attributeId == g_START_TIME_ATTRIBUTE_c){
                       CurrentStatusOfAppliance = g_PROGRAMMED_STATE_c;
                   }

//                   /* check whether reporting attibute is configured or not ,if
//                   it is not configured it is not suppose to send report command
//                   , check is done through read request*/

//
//                   /* Send Report to client device regarding new attribute value*/
//                    App_sendAttributeReports(attributeId,attributeData,
//                                             pZCLEvent->pEventData[2],pBuffer ) ;
                }

            }
            else{
                /* Request is improper */
                status = g_ZCL_Malformed_Command_c;
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;

        case g_ApplianceControlOverloadPauseResumeEvent_c:{

            /* No Command Parameters for Pause Resume Command */
            commandLength = 0x00;

            if( pZCLEvent->eventLength == commandLength ){

                if(CurrentStatusOfAppliance == g_PAUSE_STATE_c ){
                    /* Update the Previous Status of the Appliance i.e.,running
                   etc */
                    CurrentStatusOfAppliance = PreviousStatusOfAppliance;
                }
            }
            else{
                /* Request is improper */
                status = g_ZCL_Malformed_Command_c;
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer);
            }

        }
        break;

        case g_ApplianceControlOverloadPauseEvent_c:
        {
             /* No Command Parameters for Pause Command */
            commandLength = 0x00;

            if( pZCLEvent->eventLength == commandLength ){

                if( CurrentStatusOfAppliance == g_RUNNING_STATE_c ||
                   CurrentStatusOfAppliance == g_SUPERFREEZING_STATE_c ||
                       CurrentStatusOfAppliance == g_SUPERCOOLING_STATE_c ) {

                    /* Update the Current State as PauseState */

                    PreviousStatusOfAppliance = CurrentStatusOfAppliance;
                    CurrentStatusOfAppliance = g_PAUSE_STATE_c;
                }
            }
            else{
                /* Request is improper */
                status = g_ZCL_Malformed_Command_c;
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer);
            }

        }
        break;

        case g_ApplianceControlOverloadWarningEvent_c:
        {
            /* Application Should handle Reception  */
        }
        break;

#endif  /*(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)*/
#if(g_EN50523_APPLIANCE_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1)

        case g_ApplianceControlStateResponseEvent_c:{
            /* Application Should handle Reception  */
        }
        break;
        case g_ApplianceControlStateNotificationEvent_c:{

            /* Application Should handle Reception  */
        }
        break;



#endif  /*(g_EN50523_APPLIANCE_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1) */
        default :
        break;

    }
}

/******************************************************************************/

#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)


void SendApplianceControlStatus(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status ,uint8_t* pRes,uint8_t responseLength)

{

    uint8_t destAddress[8];
    uint8_t payloadLength = 0;
    /* ZCL Header Creation */
    apsdeDataPacket->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                            g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = responseType;
//    apsdeDataPacket->a_asdu[3] = status;

    payloadLength += 3;
    memUtils_memCopy(destAddress , apsdeDataPacket->src_address.IEEE_address, 0x08 );



    if(g_ZCL_Success_c == status && responseLength > 0){

        memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[3]) ,pRes,
                         responseLength);
        payloadLength += responseLength;
    }

    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_APPLIANCE_CONTROL_CLUSTER_c,payloadLength ,
        apsdeDataPacket->a_asdu );

}

/*******************************************************************************/

void App_ApplianceActivitySimulated(void )
{

    uint32_t currentTime;


    currentTime = ZCL_CallBackGetCurrentTime();



    if( CurrentStatusOfAppliance == g_RUNNING_STATE_c ||
       CurrentStatusOfAppliance == g_SUPERFREEZING_STATE_c ||
           CurrentStatusOfAppliance == g_SUPERCOOLING_STATE_c ) {

        if(currentTime >=  ActualFinishTime ){
            /* Means finished appliance cycle ,hence next moving state would be
            Idle state*/
            CurrentStatusOfAppliance = g_IDLE_STATE_c;
            /* After finishing current cycle ,ActualFinishTime and
            ActualStartTime is resetted.*/
            ActualStartTime = ActualFinishTime = 0x00000000;
            /* After finishing the cycle remaining time will be zero*/
            updateServerAttribute(ActualRemainingTime,
                                  g_REMAINING_TIME_ATTRIBUTE_c);
            /* Reset start time and finish time after finishing the cycle */
            updateServerAttribute(ActualStartTime,
                                  g_START_TIME_ATTRIBUTE_c);
            updateServerAttribute(ActualFinishTime,
                                  g_FINISH_TIME_ATTRIBUTE_c);
        }
        else{

            if((currentTime > ActualStartTime) && (currentTime < ActualFinishTime)){
                /* Update Remaining Time while appliance is in running state*/
                updateServerAttribute((ActualFinishTime - currentTime),
                                      g_REMAINING_TIME_ATTRIBUTE_c);
            }

        }
    }
    else if(CurrentStatusOfAppliance == g_PAUSE_STATE_c ){
        /* Increment the Finish Time accordingly */
        ActualFinishTime += g_ONE_TICK_c;

        /* Update the Server Value for Finish Time */
        updateServerAttribute(ActualFinishTime,g_FINISH_TIME_ATTRIBUTE_c);
    }
}
#endif
/*******************************************************************************/

void App_ApplianceControlReset(void)
{

#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
    /* changes value according to execution command and state of the appliance */
    CurrentStatusOfAppliance = g_STAND_BY_STATE_c;

    ActualStartTime = 0x00000000;
    ActualFinishTime = 0x00000000;
    ActualRemainingTime = 0x00000000 ;
    PreviousStatusOfAppliance = g_STAND_BY_STATE_c;
#endif

}


/*******************************************************************************
* Public Functions
*******************************************************************************/


/******************************************************************************/
#if(g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
static void processExecutionCommand(uint8_t commandId)
{

    uint32_t currentTime = 0x00000000,programmedStartTime = 0x00000000,
    programmedFinishTime = 0x00000000 ;

    uint8_t startTime[2],finishTime[2];

    uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
    /* Check Anything programmed if programmed proceed */
    if( CurrentStatusOfAppliance == g_PROGRAMMED_STATE_c ){

        if(commandId == g_START_c || commandId == g_START_SUPERFREEZING_c ||
           commandId == g_START_SUPERCOOLING_c || commandId == g_ENABLE_GAS_c){

               /* This Temporary variable is used to update the current status ,
               when actual start time is reached by the appliance */

           if(commandId == g_START_c){
               CurrentStatusOfAppliance = g_RUNNING_STATE_c;
           }
           else if(commandId == g_START_SUPERFREEZING_c ){
               CurrentStatusOfAppliance = g_SUPERFREEZING_STATE_c;

           }
           else if(commandId == g_START_SUPERCOOLING_c){
               CurrentStatusOfAppliance = g_SUPERCOOLING_STATE_c;

           }
           else if( commandId == g_ENABLE_GAS_c){
               CurrentStatusOfAppliance = g_RUNNING_STATE_c;
           }
            /* Counter initiated to zero indicates ,activity initiated */


            App_ReadWriteAttribute( EndPoint, g_APPLIANCE_CONTROL_CLUSTER_c,
                               g_START_TIME_ATTRIBUTE_c,
                               (uint8_t*)&startTime,g_Read_Attribute_Data_c );


            /* just for using as 4byte value instead of array*/
            memUtils_memCopy((uint8_t*)&programmedStartTime,
                            startTime,0x02 );

            App_ReadWriteAttribute( EndPoint, g_APPLIANCE_CONTROL_CLUSTER_c,
                               g_FINISH_TIME_ATTRIBUTE_c,
                               (uint8_t*)&finishTime,g_Read_Attribute_Data_c );


            /* just for using as 4byte value instead of array*/
            memUtils_memCopy((uint8_t*)&programmedFinishTime,
                            finishTime,0x02 );

            currentTime = ZCL_CallBackGetCurrentTime();



            /* Assumption :if programmed start time is lesser than sending start
            using start execution command ,following logic is used to match the
            time difference */

            parseTheTimeAttrAndConverToSecond(&programmedStartTime);

            if( programmedStartTime < currentTime ){
                ActualStartTime =  programmedStartTime +  currentTime;


            }
            else if(programmedStartTime > currentTime){
                ActualStartTime = programmedStartTime ;

            }

            parseTheTimeAttrAndConverToSecond(&programmedFinishTime);
            if( programmedFinishTime < currentTime ){
                ActualFinishTime =  programmedFinishTime +
                    (currentTime );

            }
            else if(programmedFinishTime > currentTime){
                ActualFinishTime = programmedFinishTime;

            }

            /* If in case Finish Time is less than Start time which is
            set by user then default time (extra) is added to Finish Time */
            /* It is application dependent*/
            if( ActualFinishTime <=  ActualStartTime){
                ActualFinishTime += g_THIRTY_SECONDS_c;
            }

        }
    }

    /* If it is in running state or idle state ,when stop cycle cmd is received
    then status is updated as OFF state */
    if((commandId == g_STOP_c) || (commandId == g_STOP_SUPERFREEZING_c )||
       (commandId == g_STOP_SUPERCOOLING_c) || (commandId == g_DISABLE_GAS_c)){

        /* resets */
        ActualStartTime = 0x00000000, ActualFinishTime = 0x00000000;
        CurrentStatusOfAppliance = g_OFF_STATE_c;
        /* After finishing the cycle remaining time will be zero*/
        updateServerAttribute(ActualRemainingTime,
                              g_REMAINING_TIME_ATTRIBUTE_c);
        /* Reset start time and finish time after finishing the cycle */
        updateServerAttribute(ActualStartTime,
                              g_START_TIME_ATTRIBUTE_c);
        updateServerAttribute(ActualFinishTime,
                              g_FINISH_TIME_ATTRIBUTE_c);

    }

    /* Note :Pause is not handled ... bcoz no resume */
}


/******************************************************************************/

static void parseTheTimeAttrAndConverToSecond(uint32_t *programmedTime)
{

    /* Time Attribute :
    0... 5th bit => value 0 t0 59 minutes,
    6... 7th bit => Time Encoding 0x00 Relative 0x01 Absolute
    8... 15th bit => Hours ranging from 0 t0 255 for relative and
                    0 t0 23 for absolute */

    uint32_t hourMask = 0x0000ff00;
    uint32_t minutesMask = 0x0000003f;
 //   uint8_t encodingApplied = 0xc0;

    uint8_t hour ;
    uint8_t minute;

    /* Note : Time Encoding is not applicable for now ,can be used in
    Real time Application*/
//    uint8_t encodedValue;



    uint32_t localTimeVariable = *programmedTime;
    /* 1st byte holds minute */
    minute = localTimeVariable & minutesMask;

  //  encodedValue = localTimeVariable & encodingApplied;

    localTimeVariable >>= 8;

    hour = localTimeVariable & hourMask;

    /* Converting Hours to seconds*/
    *programmedTime = hour * g_ONE_HOUR_IN_SECONDS_c;

    /* Converting Minute to seconds*/
    *programmedTime += minute *g_ONE_MIN_IN_SECONDS_c;

}
/******************************************************************************/

static void App_sendAttributeReports( uint16_t attributeId,
        uint16_t attributeData,uint8_t attributeDataType,
        APSDE_Data_Indication_t* pBuffer)
{

    uint8_t attributeDataLength = g_ZERO_c;
    uint8_t buffindex;
    APSDE_Request_t* pDataReq;
    APSDE_Data_Request_t * pAppDataReq;


    buffindex =  buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );


    if( buffindex != 0xFF ) {

#if (DEBUG_TEST == 1)
        allocatedbuffer[buffindex]= 70;
        freebuffer[buffindex][0]= 70;
        freebuffer[buffindex][1]= 0;
#endif
        pDataReq = (APSDE_Request_t *)buffMgmt_getBufferPointer( buffindex );
        pDataReq->msgType = g_APSDE_Data_Request_c;
        pAppDataReq = (APSDE_Data_Request_t *)&((pDataReq)->
                                                        APSDE_Request_Primitive.apsde_data_request);
        pAppDataReq->asdulength = 0x00;
        pAppDataReq->asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
            g_ZCL_FC_PROFILE_WIDE_COMMAND_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c ;

        pAppDataReq->asdu[1] = App_GetTransSeqNumber() ;
        pAppDataReq->asdu[2] = g_Report_Attributes_Command_c;
        memUtils_memCopy( &pAppDataReq->asdu[3],
                 (uint8_t*)&attributeId, 0x02 );
        pAppDataReq->asdu[5] = attributeDataType;
        pAppDataReq->asdulength = 0x06;
        attributeDataLength = ZCL_Get_Data_length (
                                attributeDataType);
        memUtils_memCopy( &pAppDataReq->asdu[6],
                 (uint8_t*)&attributeData,
                 attributeDataLength );
        pAppDataReq->dest_addr_mode = pBuffer->src_addr_mode;
        memUtils_memCopy((uint8_t*)&(pAppDataReq->dest_address) ,
                         (uint8_t*)&(pBuffer->src_address.IEEE_address),
                         0x08 );

        pAppDataReq->dest_endpoint = pBuffer->src_endpoint;
        pAppDataReq->src_endpoint = ZCL_Callback_Get_Device_EndPoint();;
        pAppDataReq->cluster_id = g_APPLIANCE_CONTROL_CLUSTER_c;
        pAppDataReq->profile_id = g_APP_PROFILE_ID_c;
        pAppDataReq->txoptions = App_getClusterSecurity( pAppDataReq->cluster_id );
        pAppDataReq->radius = g_DEFAULT_RADIUS_c;
        pAppDataReq->asdulength += attributeDataLength;
        APS_APSDErequest( buffindex );
    }

}

/*****************************************************************************/
static void convertSecondsToHourAndMinute(uint32_t seconds,
                                          uint8_t *pHour,uint8_t*pMinute)
{
    *pHour =  seconds /(g_ONE_HOUR_IN_SECONDS_c);
    *pMinute = seconds/ g_ONE_MIN_IN_SECONDS_c;
}
/*****************************************************************************/

static void updateServerAttribute(uint32_t programmedTime,uint16_t attributeId)
{



    uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
    uint8_t time[2];//finishTime[2];
    uint32_t tempTime;
    uint16_t newValue,temp = 0x0000;
    uint8_t hour = 0x00,min = 0x00;

    uint8_t encodingApplied = 0xc0;

    uint8_t encodedValue = 0x00;

    /* To get only time encoding value which was stored previously*/
    App_ReadWriteAttribute( EndPoint, g_APPLIANCE_CONTROL_CLUSTER_c,
                               attributeId,
                               (uint8_t*)&time,g_Read_Attribute_Data_c );

    memUtils_memCopy((uint8_t*)&tempTime,
                            time,0x02 );

    /* encodedValue holder*/
    encodedValue = tempTime &  encodingApplied;

    convertSecondsToHourAndMinute(programmedTime,&hour,&min);

    /* Lower byte*/

    if(encodedValue == 0x01){

        newValue = min | 0x40;
    }
    else if(encodedValue == 0x00 ){
        newValue = min;
    }
    /* Higher byte*/
    /* append hour */
    temp =hour << 8;
    newValue = newValue | temp ;

    /* Update to server value*/
    App_ReadWriteAttribute( EndPoint, g_APPLIANCE_CONTROL_CLUSTER_c,
                               attributeId,
                               (uint8_t*)&newValue,g_Write_Attribute_Data_c );
}

/*****************************************************************************/
#endif
