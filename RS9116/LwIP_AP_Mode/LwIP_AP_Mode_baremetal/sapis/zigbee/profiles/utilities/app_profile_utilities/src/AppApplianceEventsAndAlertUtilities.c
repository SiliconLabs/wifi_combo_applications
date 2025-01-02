/*******************************************************************************
* AppApplianceEventsAndAlert.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Appliance
*   Events And Alert
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

#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1)
    #include "ZCL_HA_Appliance_Events_And_Alert_Cluster.h"
    #include "AppApplianceEventsAndAlertUtilities.h"
    #include "ApplicationThinLayer.h"
#endif /* (g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLUSTER_ENABLE_d == 1)*/


/*******************************************************************************
* Global Constants
*******************************************************************************/



/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

/* Temporary alert list ,used to extract entry and store into
actual alert list */
Alert_Structure_t TempAlertStructure[g_ALERT_LIST_SIZE_c]= {

    { 0x01,((g_ALERT_WARNING_c & g_BITMASK_FOR_ALERT_CATEGORY_c)|
        (g_RECOVERY_c & g_BITMASK_FOR_ALERT_PRESENCE_OR_RECOVERY_c)),0x00 },
    { 0x02,((g_ALERT_DANGER_c & g_BITMASK_FOR_ALERT_CATEGORY_c)|
        (g_PRESENCE_c & g_BITMASK_FOR_ALERT_PRESENCE_OR_RECOVERY_c)),0x00 },
    { 0x03,((g_ALERT_FAILURE_c & g_BITMASK_FOR_ALERT_CATEGORY_c)|
        (g_RECOVERY_c & g_BITMASK_FOR_ALERT_PRESENCE_OR_RECOVERY_c)),0x00 },
    { 0x04,((g_ALERT_WARNING_c & g_BITMASK_FOR_ALERT_CATEGORY_c)|
        (g_RECOVERY_c & g_BITMASK_FOR_ALERT_PRESENCE_OR_RECOVERY_c)),0x00 },
    { 0x05,((g_ALERT_DANGER_c & g_BITMASK_FOR_ALERT_CATEGORY_c)|
        (g_PRESENCE_c & g_BITMASK_FOR_ALERT_PRESENCE_OR_RECOVERY_c)),0x00 }
};


Alert_Structure_t ActualAlertResponseList[g_ALERT_LIST_SIZE_c];

uint8_t CurrentAlertEntries = 0x00;
uint8_t DefaultTableTracker = 0x00;

uint8_t *pResonsePayload = g_NULL_c;




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
#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1)
static uint8_t FillTheResponseContent(void);
#endif/* g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1*/
/*******************************************************************************
* Public Functions
*******************************************************************************/

void App_ApplianceEventsAndAlertHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer )
{
#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1)
    uint8_t status = g_ZCL_Success_c,bufferIndex =0;
    uint8_t tempBuffer[5];
    uint8_t responseLen = 0;
#endif


    switch( pZCLEvent->eventId )
    {

#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1)
        /* event is used retrieve Household Appliance current alerts. */
        case g_GetAlertsEvent_c :
            /* Note :actual alerts is application dependent */

            if(CurrentAlertEntries > g_ZERO_c){

                /* Fill the Response content */
                bufferIndex = FillTheResponseContent();
                responseLen = sizeof(Alert_Structure_t) * CurrentAlertEntries
                    + 0x01;
            }
            else{
                /* For alert count attribute */
                /* current active alert entry is zero*/
                tempBuffer[0] = 0x00;
                responseLen = 0x01;
                pResonsePayload = tempBuffer;

            }
            SendApplianceEventsAndAlertResponse(pBuffer,
                                                (uint8_t )g_GetAlertsResponse_c,
                                     status,pResonsePayload,responseLen,bufferIndex);
            break;

#endif /* g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1 */

#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLIENT_CLUSTER_ENABLE_d == 1)
           /* Application Should handle Reception  */
        case g_GetAlertsResponseEvent_c :
            break;
        case g_AlertsNotificationEvent_c :
            break;
        case g_EventNotification_c :
            break;
#endif
        default :
            break;

    }

}

/******************************************************************************/
void SendApplianceEventsAndAlertResponse(APSDE_Data_Indication_t *apsdeDataPacket
                                         ,uint8_t responseType,
    uint8_t status,uint8_t *pAsdu, uint8_t responseLen,
    uint8_t bufferToBeFree )
{

    uint8_t *pRes = g_NULL_c;
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

        if(responseLen > 0x01 ){

            pRes = buffMgmt_getBufferPointer( bufferToBeFree );

            memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[4]),
                             pRes, responseLen);
            payloadLength += responseLen;

            if(bufferToBeFree != 0xff){
                /* This buffer contents is copied into apsdeBuffer,
                Hence this buffer need to be cleared*/
                buffMgmt_freeBuffer(bufferToBeFree,236);
            }
        }
        else{
            payloadLength += responseLen;
            apsdeDataPacket->a_asdu[4] = *pAsdu;
        }
    }

    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_c,payloadLength,
        apsdeDataPacket->a_asdu );


}

/*****************************************************************************/

void StoreAlertIntoAlertsTable(void)
{
    /* Since its not a real application implementation,
    as interface is invoked used to store alert from default table into
    actual alerts table.*/


    /* To rollback over the table*/
    if(DefaultTableTracker >= g_ALERT_LIST_SIZE_c){
        DefaultTableTracker = DefaultTableTracker % g_ALERT_LIST_SIZE_c;
    }
    if(CurrentAlertEntries >= g_ALERT_LIST_SIZE_c ){
        CurrentAlertEntries = CurrentAlertEntries % g_ALERT_LIST_SIZE_c;
    }

    memUtils_memCopy((uint8_t*)&(ActualAlertResponseList[CurrentAlertEntries++]),
        (uint8_t*)&TempAlertStructure[DefaultTableTracker++],
             sizeof(Alert_Structure_t));
}

/*****************************************************************************/
void InvokeAlertNotificationCommand(void)
{
    /* This functions is used to notify about alerts to client from server */
    /* How notification is done is application dependent.
    For now its done throught test harness interface*/

    uint8_t bufferIndex = g_NO_AVAILABLE_BUFFER_c,tempIndex = 0x00;
    APSDE_Data_Request_t *p_apsde_data_request = g_NULL_c;




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
        p_apsde_data_request->cluster_id     = g_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_c;

        p_apsde_data_request->asdulength     = 0x04 ;

        p_apsde_data_request->asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
            g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        p_apsde_data_request->asdu[1]        = App_GetTransSeqNumber();
        p_apsde_data_request->asdu[2]        = g_AlertsNotificationCommand_c;
        p_apsde_data_request->asdu[3]        = g_ZCL_Success_c;


        if(CurrentAlertEntries ){

            p_apsde_data_request->asdu[4] = CurrentAlertEntries;
            for(tempIndex = 0;tempIndex < CurrentAlertEntries ;tempIndex++){

                memUtils_memCopy( &p_apsde_data_request->asdu[
                                 0x05 +(tempIndex* sizeof(Alert_Structure_t))],
                             (uint8_t*)&ActualAlertResponseList[tempIndex],
                             sizeof(Alert_Structure_t) );
            }
            p_apsde_data_request->asdulength  +=
                CurrentAlertEntries * sizeof(Alert_Structure_t) ;
            p_apsde_data_request->asdulength += 0x01;
        }
        else{
            /* There are no alerts detected*/
            p_apsde_data_request->asdu[4] = 0x00;
            p_apsde_data_request->asdulength  += 0x01;
        }

        p_apsde_data_request->txoptions =  App_getClusterSecurity( g_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_c );
        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;

        //APS_APSDErequest( bufferIndex );
        APS_APSDE_request(  bufferIndex,p_apsde_data_request );
    }
}

/*****************************************************************************/
void InvokeEventNotificationCommand(void)
{

    uint8_t bufferIndex = g_NO_AVAILABLE_BUFFER_c;
    APSDE_Data_Request_t *p_apsde_data_request = g_NULL_c;

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
        p_apsde_data_request->cluster_id     = g_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_c;

        p_apsde_data_request->asdulength     = 0x04 ;

        p_apsde_data_request->asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
            g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        p_apsde_data_request->asdu[1]        = App_GetTransSeqNumber();
        p_apsde_data_request->asdu[2]        = g_EventNotificationCommand_c;
        p_apsde_data_request->asdu[3]        = g_ZCL_Success_c;


        /* Event Notification has two payloads :
        Event Header : 1byte : Reserved
        Event Identification :1byte : event by default assumed as switched off*/

        p_apsde_data_request->asdu[4] = 0x00;


        p_apsde_data_request->asdu[5] = g_SWITCHING_OFF_c;
        p_apsde_data_request->asdulength  += 0x02;


        p_apsde_data_request->txoptions =  App_getClusterSecurity( g_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_c );
        p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;

        //APS_APSDErequest( bufferIndex );
        APS_APSDE_request(  bufferIndex,p_apsde_data_request );
    }



}
/*******************************************************************************/
void App_ResetEventsAndAlert(void)
{
    CurrentAlertEntries = 0x00;
    DefaultTableTracker = 0x00;


}

/*******************************************************************************
* Private Functions
*******************************************************************************/

#if(g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1)
static uint8_t FillTheResponseContent(void)
{

    uint8_t bufferIndex = g_NO_AVAILABLE_BUFFER_c;
    Get_Alerts_Response_t* pAlertResponse = g_NULL_c;

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
    /* Inorder to append Alert entries*/
    pAlertResponse =
        (Get_Alerts_Response_t*)buffMgmt_getBufferPointer(bufferIndex);

    if(CurrentAlertEntries){

        pAlertResponse->alertcount = CurrentAlertEntries;

        for(uint8_t loopIndex = 0; loopIndex < CurrentAlertEntries ;
            loopIndex++){

            /* copying actual alert list into APsde data payload */
            memUtils_memCopy(&
            (pAlertResponse->alertStructure[loopIndex*sizeof(Alert_Structure_t)]) ,
                     (uint8_t*)&(ActualAlertResponseList[loopIndex]),sizeof(Alert_Structure_t));
        }
    }
    return bufferIndex;

}

#endif /*g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1 */




/*******************************************************************************
                                End of Appliance Events and Alert Cluster.c
*******************************************************************************/
