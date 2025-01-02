/*******************************************************************************
* AppPollControlUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Poll Control
*   Cluster
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

//#include "eeprom_psm.h"

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
#include "zdo_interface.h"
#include "app_configuration_default.h"

#if(g_POLL_CONTROL_CLUSTER_ENABLE_d == 1)
    #include "ZCL_General_Poll_Control_Cluster.h"
    #include "AppPollControlUtilities.h"
    #include "ApplicationThinLayer.h"
    #include "ZCL_Foundation_Reporting.h"
    #include "AppZclDefaultAttributesData.h"
#endif /* (g_POLL_CONTROL_CLUSTER_ENABLE_d == 1)*/


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

/* Normal poll rate which is set in ZigBee Stack*/
uint16_t g_NormalPollRate_c = g_POLL_RATE_c;
/* Default Fast poll rate according to this cluster is 2 quater seconds,
    0x01f4 milliseconds*/
uint16_t g_ShortPollRate_c = 0x01f4/* in terms of millisecond*/;
/* Default Slow poll rate according to this cluster is 5 seconds = 5000 ms,
in hex 0x1388*/
uint16_t g_LongPollRate_c = 0x1388;

uint32_t PreviousTimeTrack = 0x00000000;
/* Decides to start or stop fast poll*/
bool StartFastPollRate = g_FALSE_c;
/* This variable maintains time at with fast poll shd end,It is in seconds */
uint32_t ModeIntervalExpiry = 0x00000000;
/* Flag to identify whether end device is in fast mode mode or normal mode */
bool FastPollNotStarted = g_TRUE_c;
#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
/* This variable is used to set the checkin response with user set value */
uint8_t UserSetStartFastPoll = g_FALSE_c;
uint16_t UserSetFastPollTimeout = 0x0000;
#endif /*g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1  */

#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
uint32_t PreviousTime = 0x00000000;
#endif


/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
static void SendCheckInResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status ,uint8_t* pRes,uint8_t responseLength,
    bool serverToClient);
#endif /* g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1*/

#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
static void SendCheckInCommand(void);

extern
#endif

/*******************************************************************************
* Public Functions
*******************************************************************************/

void App_PollControlEventHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer )
{

#if((g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1)||(g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1))
        uint8_t commandLength = 0x00,status = g_ZCL_Success_c;
#endif

    switch( pZCLEvent->eventId )
    {

#if(g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)

        case g_CheckinResponseEvent_c :{
            CheckInResponsePayload_t checkInRes;
            commandLength = sizeof(CheckInResponsePayload_t);
            uint16_t tempTimeoutperiod = 0x0000;

            if((pZCLEvent->eventLength) == commandLength ){

                memUtils_memCopy((uint8_t*)&checkInRes,
                    (uint8_t*)&(pZCLEvent->pEventData[0]),commandLength);

                if(checkInRes.startFastPolling){
                    /* Start the Fast Poll rate */
                    StartFastPollRate = g_TRUE_c;
                    /* Time is applicable only if fast poll rate is going
                    to start*/
                    /* Time out indicates when it shd come out of fast poll
                    rate*/
                    memUtils_memReverseCopy((uint8_t*)&tempTimeoutperiod,
                                     (uint8_t*) &(checkInRes.fastPollTimeOut),
                                     sizeof(checkInRes.fastPollTimeOut));
                    /* converting quater seconds to seconds*/
                    /* If new value is greater than previous value then
                    new value is used */
                    if((ModeIntervalExpiry < (tempTimeoutperiod /4)) &&
                       (FastPollNotStarted == g_TRUE_c )){
                        ModeIntervalExpiry = (tempTimeoutperiod /4);
                    }
                }
                else{
                    StartFastPollRate = g_FALSE_c;

                }
            }
            else{
                /* INVALID command payload */
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;

        case g_FastPollStopEvent_c:{
            commandLength = 0x00;
            if(pZCLEvent->eventLength == commandLength ){
                /* stop the fast poll and continue with normal poll rate*/
                StartFastPollRate = g_FALSE_c;
            }
            else{
                /* INVALID command payload */
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;

        case g_SetLongPollIntervalEvent_c:{

            LongPollInterval_t templongPollInterval;
            commandLength = sizeof(templongPollInterval.newLongPollInterval);
            uint32_t valueToSet = 0x000000, checkinAttrValue = 0x000000;
            uint16_t shotPollAttrValue = 0x0000;
            bool validValue = g_FALSE_c;

            if((pZCLEvent->eventLength ) == commandLength ){

                memUtils_memCopy((uint8_t *)&(valueToSet),
                                 (uint8_t*)&(pZCLEvent->pEventData[0]),commandLength);

                /* validating the value to be set
                1.checking with the range  0x04 – 0x6E0000
                2.checking with the relation with other attributes:
                Check-in Interval >= Long Poll Interval >= Short Poll Interval*/

                if(valueToSet > 0x04 && valueToSet < 0x6E0000){
#if(g_CHECK_IN_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
                    if(g_ZCL_Success_c != App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                        g_POLL_CONTROL_CLUSTER_c,
                        g_CHECK_IN_INTERVAL_ATTRIBUTE_ID_c,
                        (uint8_t*)&checkinAttrValue,
                        g_Read_Attribute_Data_c )){
                        /* Could not retrieve the check in attribute */
                        status = g_ZCL_Failure_c;
                    }
#else /* g_CHECK_IN_INTERVAL_ATTRIBUTE_ENABLE_c == 1*/
                /*Attribute is disabled*/
                status = g_ZCL_Invalid_Field_c;
#endif
#if(g_SHORT_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
                    if(g_ZCL_Success_c != App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                        g_POLL_CONTROL_CLUSTER_c,
                        g_SHORT_POLL_INTERVAL_ATTRIBUTE_ID_c,
                        (uint8_t*)&shotPollAttrValue,
                        g_Read_Attribute_Data_c )){
                        /* Could not retrieve the check in attribute */
                        status = g_ZCL_Failure_c;
                    }
#else /* g_SHORT_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1*/
                /*Attribute is disabled*/
                status = g_ZCL_Invalid_Field_c;
#endif
                    if(status == g_ZCL_Success_c){
                        /* After retrieving other attributes,
                        check the relation */
                        if((checkinAttrValue >= valueToSet) &&
                           (valueToSet >= shotPollAttrValue) ){
                            validValue = g_TRUE_c;
                        }
                        else{
                            status = g_ZCL_Invalid_Field_c;
                        }
                    }
                }
                else{
                    validValue = g_FALSE_c;
                }
#if(g_LONG_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
            if(g_TRUE_c == validValue){
                status = App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                            g_POLL_CONTROL_CLUSTER_c,
                            g_LONG_POLL_INTERVAL_ATTRIBUTE_ID_c,
                            (uint8_t*)&valueToSet,
                            g_Write_Attribute_Data_c );
            }
            else{
                status = g_ZCL_Invalid_Field_c;
            }
#else /* g_LONG_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1*/
                    /* Attribute is disabled*/
                status = g_ZCL_Invalid_Field_c;
#endif /* g_LONG_POLL_INTERVAL_ATTRIBUTE_ENABLE_c*/
            }
            else{
                /* INVALID command payload */
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;

        case g_SetShortPollIntervalEvent_c:{
            ShortPollInterval_t tempShortPollInterval;
            commandLength = sizeof(tempShortPollInterval.newShortPollInterval);
            uint16_t valueToSet = 0x0000 ;
            uint32_t checkinAttrValue = 0x000000;
            uint32_t longPollAttrValue = 0x000000;
            bool validValue = g_FALSE_c;

            if((pZCLEvent->eventLength ) == commandLength ){

                memUtils_memCopy((uint8_t *)&(valueToSet),
                                 (uint8_t*)&(pZCLEvent->pEventData[0]),commandLength);

                /* validating the value to be set

                1.checking with the range  0x01 – 0xffff
                2.checking with the relation with other attributes:
                Check-in Interval >= Long Poll Interval >= Short Poll Interval*/

                if(valueToSet > 0x01 && valueToSet < 0xffff){
#if(g_CHECK_IN_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
                    if(g_ZCL_Success_c != App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                        g_POLL_CONTROL_CLUSTER_c,
                        g_CHECK_IN_INTERVAL_ATTRIBUTE_ID_c,
                        (uint8_t*)&checkinAttrValue,
                        g_Read_Attribute_Data_c )){
                        /* Could not retrieve the check in attribute */
                        status = g_ZCL_Failure_c;
                    }
#else /* g_CHECK_IN_INTERVAL_ATTRIBUTE_ENABLE_c == 1*/
                /*Attribute is disabled*/
                status = g_ZCL_Invalid_Field_c;
#endif
#if(g_LONG_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
                    if(g_ZCL_Success_c != App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                        g_POLL_CONTROL_CLUSTER_c,
                        g_LONG_POLL_INTERVAL_ATTRIBUTE_ID_c,
                        (uint8_t*)&longPollAttrValue,
                        g_Read_Attribute_Data_c )){
                        /* Could not retrieve the check in attribute */
                        status = g_ZCL_Failure_c;
                    }
#else /* g_LONG_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1*/
                /*Attribute is disabled*/
                status = g_ZCL_Invalid_Field_c;
#endif  /* g_LONG_POLL_INTERVAL_ATTRIBUTE_ENABLE_c*/
                    if(status == g_ZCL_Success_c){
                        /* After retrieving other attributes,
                        check the relation */
                        if((checkinAttrValue >= longPollAttrValue) &&
                           (longPollAttrValue >= valueToSet) ){
                            validValue = g_TRUE_c;
                        }
                        else{
                            status = g_ZCL_Invalid_Field_c;
                        }
                    }
                }
                else{
                    validValue = g_FALSE_c;
                }
#if(g_SHORT_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1)
            if(g_TRUE_c == validValue){
                status = App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                            g_POLL_CONTROL_CLUSTER_c,
                            g_SHORT_POLL_INTERVAL_ATTRIBUTE_ID_c,
                            (uint8_t*)&valueToSet,
                            g_Write_Attribute_Data_c );
            }
            else{
                status = g_ZCL_Invalid_Field_c;
            }
#else /* g_SHORT_POLL_INTERVAL_ATTRIBUTE_ENABLE_c == 1*/
                    /* Attribute is disabled*/
                status = g_ZCL_Invalid_Field_c;
#endif /* g_SHORT_POLL_INTERVAL_ATTRIBUTE_ENABLE_c*/
            }
            else{
                /* INVALID command payload */
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;
#endif /* g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 */

#if(g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1)
        case g_CheckinEvent_c:{

            commandLength = 0x00;
            if(pZCLEvent->eventLength == commandLength ){

                CheckInResponsePayload_t checkInRes;

                checkInRes.startFastPolling = UserSetStartFastPoll;
                memUtils_memReverseCopy((uint8_t*)&(checkInRes.fastPollTimeOut),
                                 (uint8_t*)&UserSetFastPollTimeout,
                                 sizeof(UserSetFastPollTimeout));

                /* Response is application dependent */
                SendCheckInResponse( pBuffer,
                                    (uint8_t)g_CheckinResponseCommand_c,
                                    (uint8_t)g_ZCL_Success_c,
                                    (uint8_t*)&checkInRes,
                                    sizeof(CheckInResponsePayload_t),
                                    g_FALSE_c );
            }
            else{
                /* INVALID command payload */
                status = g_ZCL_Invalid_Value_c;
            }

            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;
#endif /* g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1*/
        default :
            break;
    }
}


/******************************************************************************/
#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
void App_UpdateUserSetValue(bool startFastPoll,uint16_t fastPollTimeout)
{
    /* Its application dependent */
    /* for now user interface 0x5f is used to set the user option*/
    UserSetStartFastPoll = startFastPoll;
    UserSetFastPollTimeout = fastPollTimeout;
}
#endif

/******************************************************************************/
#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
void CheckTheCheckIntervalAndSendCheckInCommand(void)
{

    /* checking the interval on time expiry*/
    uint32_t checkinAttrValue = 0x00000000,tempValue = 0x00000000 ;
    uint8_t temp[4] = g_CHECK_IN_INTERVAL_DEFAULT_DATA_c;
    uint32_t currentTime = ZCL_CallBackGetCurrentTime();

    if(PreviousTime == 0x00000000 ){
        PreviousTime = currentTime;
    }
    if(g_ZCL_Success_c != App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                            g_POLL_CONTROL_CLUSTER_c,
                            g_CHECK_IN_INTERVAL_ATTRIBUTE_ID_c,
                            (uint8_t*)&tempValue,
                            g_Read_Attribute_Data_c )){

        //tempValue =  g_CHECK_IN_INTERVAL_DEFAULT_DATA_c ;
        memUtils_memCopy((uint8_t*)&tempValue,(uint8_t*)(temp),
                                0x04);

    }
    memUtils_memCopy((uint8_t*)&checkinAttrValue,(uint8_t*)&(tempValue),
                            0x04);

    /* (currentTime - PreviousCurrentTime) gives total seconds ,
    checkinAttrValue(qs) diving by 4 seconds  */
    if(((currentTime - PreviousTime) ) >= (checkinAttrValue /4) ){
        /* reset the previous currentTime */
        PreviousTime = 0x00000000;
        /* checkin Interval matched ,sending  */
        SendCheckInCommand();

    }
}

/******************************************************************************/

void SetTheMACPollRateAccordingly(void)
{
#if ( g_ZIGBEE_END_DEVICE_d == 1 )
    uint16_t fastPollRateSet = 0x0000,fastPollTimeout = 0x0000;

    /* this flag is used to set value of mac poll rate only once*/
    static bool onceSet = g_FALSE_c;
    /* This function is used to set end device poll rate according the
    attribute client's checkin response */

    if((g_TRUE_c == StartFastPollRate) &&( g_TRUE_c == FastPollNotStarted )){

        /* if ModeIntervalExpiry is zero ,ie., user has not set any timeoutperiod
        the timeout period is retrieved from server attributes */

        if(ModeIntervalExpiry == 0x0000){
            if(g_ZCL_Success_c == App_ReadWriteAttribute(
                            ZCL_Callback_Get_Device_EndPoint(),
                            g_POLL_CONTROL_CLUSTER_c,
                            g_FAST_POLL_TIMEOUT_ATTRIBUTE_ID_c,
                            (uint8_t*)&fastPollTimeout,
                            g_Read_Attribute_Data_c )){

                ModeIntervalExpiry = fastPollTimeout /4;
            }
        }

        /*starting fast polling */
        if(ModeIntervalExpiry){
            if(g_ZCL_Success_c == App_ReadWriteAttribute(
                            ZCL_Callback_Get_Device_EndPoint(),
                            g_POLL_CONTROL_CLUSTER_c,
                            g_SHORT_POLL_INTERVAL_ATTRIBUTE_ID_c,
                            (uint8_t*)&fastPollRateSet,
                            g_Read_Attribute_Data_c )){

                /* Set the Fast Poll Rate, to convert in millisecon form,
                                1sec = 1000msec*/
                TH_setPollRate((fastPollRateSet * 1000)/4);
            }
        }
        /* ie., fast poll mode started*/
        FastPollNotStarted = g_FALSE_c;
        onceSet = g_FALSE_c;
    }

    /* check whether it is in fast poll mode */
    if( (g_FALSE_c == FastPollNotStarted) && (g_TRUE_c == StartFastPollRate)  ){
        /* every second fastpollTimeout period is decremented */
        uint32_t currentTime = ZCL_CallBackGetCurrentTime();
        if(PreviousTimeTrack == 0x00000000 ){
            PreviousTimeTrack = currentTime;
        }

        if((currentTime - PreviousTimeTrack ) >= 1){
            PreviousTimeTrack = currentTime;
            if(ModeIntervalExpiry){
                ModeIntervalExpiry = ModeIntervalExpiry -1 ;
            }
        }
    }

    if(g_FALSE_c == onceSet){

        if((ModeIntervalExpiry == 0x00000000) || (g_FALSE_c == StartFastPollRate)){
            /* reset to normal poll rate */
            /* Normal poll rate of stack 2 seconds*/
            TH_setPollRate(g_POLL_RATE_c);
            /* Reset the value */
            onceSet = g_TRUE_c;
            ModeIntervalExpiry = 0x00000000;
            FastPollNotStarted = g_TRUE_c;
            StartFastPollRate = g_FALSE_c;
        }
    }

#endif /* g_ZIGBEE_END_DEVICE_d == 1*/
}

#endif /*g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1  */

/*******************************************************************************
* Private Functions
*******************************************************************************/
#if ( g_POLL_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
static void SendCheckInResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status ,uint8_t* pRes,uint8_t responseLength,
    bool serverToClient)
{

    uint8_t destAddress[8];
    uint8_t payloadLength = 0;
    /* ZCL Header Creation */

    apsdeDataPacket->a_asdu[0] = g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    if(serverToClient){
        apsdeDataPacket->a_asdu[0] |= g_ZCL_SERVER_TO_CLIENT_DIRECTION_c;

    }
    else{
        apsdeDataPacket->a_asdu[0] |= g_ZCL_CLIENT_TO_SERVER_DIRECTION_c;
    }

    apsdeDataPacket->dest_addr_mode = 0x03;


    memUtils_memCopy(destAddress,apsdeDataPacket->src_address.IEEE_address,0x08 );


    apsdeDataPacket->profile_id = g_APP_PROFILE_ID_c;


    apsdeDataPacket->src_endpoint = ZCL_Callback_Get_Device_EndPoint();
    apsdeDataPacket->cluster_id = g_POLL_CONTROL_CLUSTER_c;

    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = responseType;
    //apsdeDataPacket->a_asdu[3] = status;

    payloadLength += 3;






    if(g_ZCL_Success_c == status && responseLength > 0){

        memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[3]) ,pRes,
                         responseLength);

        payloadLength += responseLength;
    }

    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_POLL_CONTROL_CLUSTER_c,payloadLength ,
        apsdeDataPacket->a_asdu );

}
#endif
/******************************************************************************/
#if ( g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
static void SendCheckInCommand(void)
{
    APSDE_Data_Request_t *p_apsde_data_request;
    uint8_t buffindex;

    buffindex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

   if( g_NO_AVAILABLE_BUFFER_c != buffindex ) {
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 73 */
        allocatedbuffer[buffindex]= 73;
        freebuffer[buffindex][0]= 73;
        freebuffer[buffindex][1]= 0;
#endif
        p_apsde_data_request =
            ( APSDE_Data_Request_t * )buffMgmt_getBufferPointer( buffindex );
        *((uint8_t*)p_apsde_data_request) = g_APSDE_Data_Request_c;
        p_apsde_data_request =
            (APSDE_Data_Request_t *)&(((APSDE_Request_t *)p_apsde_data_request)->
                                   APSDE_Request_Primitive.apsde_data_request);

            /* Check In Interval to be sent to all paired devices */
        p_apsde_data_request->dest_addr_mode = m_INDIRECT_ADDR_MODE_c;
        p_apsde_data_request->profile_id = g_APP_PROFILE_ID_c;

        p_apsde_data_request->src_endpoint = ZCL_Callback_Get_Device_EndPoint();
        p_apsde_data_request->cluster_id = g_POLL_CONTROL_CLUSTER_c;

        p_apsde_data_request->asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
                               g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                               g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        p_apsde_data_request->asdu[1] = App_GetTransSeqNumber();
        p_apsde_data_request->asdu[2] = g_CheckinCommand_c;

        p_apsde_data_request->asdulength = 0x03;

        p_apsde_data_request->txoptions  =
            App_getClusterSecurity( g_POLL_CONTROL_CLUSTER_c );
        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;
        //APS_APSDErequest( buffindex );
        APS_APSDE_request(buffindex,p_apsde_data_request );
   }
}
#endif  /* g_POLL_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 */

/*******************************************************************************
                                End of AppPollControlUtilities.c
*******************************************************************************/
