/*******************************************************************************
* AppPrepaymentUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting prepayment Cluster
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include <string.h>
#include "stack_common.h"
//#include "nvm.h"
#include "ZCL_Common.h"
#include "sw_timer.h"
#include "buffer_management.h"
#include "memory_utility.h"

#include "ZCL_Interface.h"
#include "zdp_interface.h"
#include "ZCL_Functionality.h"
#include "stack_functionality.h"
#include "ZCL_Foundation.h"
#include "commissioning_support.h"
#include "zdo_interface.h"
#include "apsde_interface.h"
#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
#include "AppDiscoveryUtilities.h"
#include "ZCL_Prepayment_Cluster.h"
#include "AppPrepaymentUtilities.h"
#include "ApplicationThinLayer.h"
#include "ApptimerId.h"
#include "ZCL_Simple_Metering_Cluster.h"
#include "AppDiscoveryUtilities.h"


/*******************************************************************************
* Global Constants
*******************************************************************************/

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/


#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
//extern prepaymentAttributes_t prepayment_cluster_server_values;
prepaymentAttributes_t prepayment_cluster_server_values;//extern when defined in app
ChangeSupplyCmdPayload_t current_change_supply_values;
ProposedSupplyValues_t proposedValues[g_PROPOSED_VALUES_MAX_c];
extern PrepaymentSupplyStatus current_supply_status_value;

static uint8_t proposedIndex = 0;
static bool ImplementationTimeMatch;
/* default state of the meter status assumed as ARMED state*/
PrepaymentSupplyStatus g_currentSupplyState = g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_ARMED_c ;

#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
ZCL_Event_t tempZCL_Event;
uint8_t storedMatchedEndpoint = 0xFF;
uint16_t storedMatchedAddress = 0xFFFF;
uint8_t readMeterSerialNo[16];
uint8_t readMeterSiteId[16];
uint8_t BackUpApsDataBufferIndex = 0xFF;
uint8_t BackUpZCLBufferIndex = 0xFF;
#endif      /*g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1  */
#endif     /* ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )*/

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
extern Simple_Metering_Cluster_t simple_metering_server_cluster_values;
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
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
static void supplyStatusResponse(APSDE_Data_Indication_t* apsdeDataPacket,
                                 uint8_t status ,
                                 SupplyStatusResCmdPayload_t supplyResponse );
static uint8_t validateChangeSupplyCmd(ChangeSupplyCmdPayload_t changeSupplyCmdData);

static void insertProposedValues(ChangeSupplyCmdPayload_t changeSupplyVal,
                                 uint8_t addressMode ,Address srcAddress);
static void updateServerValues();
#if(g_PROPOSED_VALUES_MAX_c > 1)
static void sortTheProposedValues();
#endif /*(g_PROPOSED_VALUES_MAX_c > 1)*/
static bool checkifPresentInProposedValues(ChangeSupplyCmdPayload_t changeSupplyVal);
static uint8_t App_CheckAuthorizedOriginator( );

static void App_PrepaymentCommonFunction( ZCL_Event_t *pZCLEvent,
                                        APSDE_Data_Indication_t* p_dataInd );

#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
static void App_ProcessEventIfMeteringServerIsDiabled( void );
#endif /* (g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&
          (g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1) */
#endif /* ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )*/
/*******************************************************************************
* Public Functions
*******************************************************************************/

void App_PrepaymentEventsHandler( ZCL_Event_t *pZCLEvent,
                                        APSDE_Data_Indication_t* p_dataInd )
{

#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t status = g_ZCL_Success_c;
    uint8_t cmdlength = 0;
    uint8_t tempvar = 0;
#endif
    switch ( pZCLEvent->eventId )
    {
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
        case g_PrepaymentSelectAvailEmerCreditReqEvent_c:
        {
            /* Application code can be written to handle Select Emergency
            credit command */
        }
        break;
        case g_PrepaymentChangeSupplyReqEvent_c:
        {
            /* assumption is site id 1st byte will have string len corresponding
            and meter serial no,offset of meterserial no would be 8 */
            tempvar = pZCLEvent->pEventData[8] + 1;
            /*Note : 16 is the sizeof meter serial no ,32 sizeof siteId*/
            cmdlength = (sizeof(ChangeSupplyCmdPayload_t ) - 16 - 16  + tempvar
                         + pZCLEvent->pEventData[8 + tempvar] + 1 );

            if( pZCLEvent->eventLength == cmdlength ) {
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
                App_PrepaymentCommonFunction(pZCLEvent, p_dataInd );
#endif
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0) && (g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)
            uint8_t *pBufferRx;
            BackUpApsDataBufferIndex = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
            /*This Buffer is used for to issue restart request*/
            if( g_NO_AVAILABLE_BUFFER_c == BackUpApsDataBufferIndex ) {
                return;
            }
#if ( DEBUG_TEST == 1 )
            /* Buffer ID --> 125 */
            allocatedbuffer[BackUpApsDataBufferIndex] = 125;
            freebuffer[BackUpApsDataBufferIndex][0] = 125;
            freebuffer[BackUpApsDataBufferIndex][1] = 0;
#endif
            /* to create the backup of Aps data payload */
            pBufferRx = buffMgmt_getBufferPointer(BackUpApsDataBufferIndex);
            memUtils_memCopy(pBufferRx,(uint8_t*)p_dataInd,sizeof(APSDE_Data_Indication_t)
                             + p_dataInd->asdulength  );

            BackUpZCLBufferIndex = buffMgmt_allocateBuffer(g_SMALL_BUFFER_c);
            if( g_NO_AVAILABLE_BUFFER_c == BackUpZCLBufferIndex ) {
                return;
            }
#if (DEBUG_TEST == 1)
            /* Buffer ID --> 174 */
            allocatedbuffer[BackUpZCLBufferIndex] = 174;
            freebuffer[BackUpZCLBufferIndex][0] = 174;
            freebuffer[BackUpZCLBufferIndex][1] = 0;
#endif
            /* to create the backup of ZCL data payload */
            pBufferRx = buffMgmt_getBufferPointer(BackUpZCLBufferIndex);
            tempZCL_Event.eventId = pZCLEvent->eventId;
            tempZCL_Event.eventLength = pZCLEvent->eventLength;
            memUtils_memCopy((uint8_t*)(pBufferRx),(uint8_t*)(pZCLEvent->pEventData),
                             pZCLEvent->eventLength);
            App_ProcessEventIfMeteringServerIsDiabled( );
#endif  /* (g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0) && (g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1)*/
            }
            else{
                status = g_ZCL_Malformed_Command_c;
                /*default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,p_dataInd );
            }
        }
        break;
#endif /* g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d */
#if ( g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
        case g_PrepaymentSupplyStatusResEvent_c:
        {
            /* Application code can be written to handle the change supply
            response command */
        }
        break;
#endif  /* g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d */
    default:
        break;
    }
}

/******************************************************************************/
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
static void App_PrepaymentCommonFunction( ZCL_Event_t *pZCLEvent,
                                        APSDE_Data_Indication_t* p_dataInd )

{
    ChangeSupplyCmdPayload_t changeSupplyCmdData;
    SupplyStatusResCmdPayload_t supplyResponse;
    uint8_t payloadOffset = 0,status = 0;

    /* provider id*/
    memUtils_memCopy((uint8_t*)&(changeSupplyCmdData.providerID),
                     (uint8_t*)&(pZCLEvent->pEventData[0]),
                     sizeof (changeSupplyCmdData.providerID));
    payloadOffset += sizeof (changeSupplyCmdData.providerID);

    /* request date time */
    memUtils_memCopy((uint8_t*)&(changeSupplyCmdData.requestDateTime),
        (uint8_t*)&(pZCLEvent->pEventData[payloadOffset]),
        sizeof (changeSupplyCmdData.requestDateTime));
    payloadOffset += sizeof (changeSupplyCmdData.requestDateTime);

    /* since site Id is string ,1st byte of string is considered as
    holding string length */
    memUtils_memCopy((uint8_t*)&(changeSupplyCmdData.siteId),
        (uint8_t*)&(pZCLEvent->pEventData[payloadOffset]),
        pZCLEvent->pEventData[payloadOffset]+1 /* string len byte */);
    payloadOffset += pZCLEvent->pEventData[payloadOffset] + 1 ;

    /* since meter serial no is string ,1st byte of string is considered as
    holding string length */
    memUtils_memCopy((uint8_t*)&(changeSupplyCmdData.meterSerialNumber) ,
                     (uint8_t*)&(pZCLEvent->pEventData[payloadOffset]),
                     pZCLEvent->pEventData[payloadOffset]+1
                         /* string len byte */);
    payloadOffset += pZCLEvent->pEventData[payloadOffset] + 1;

    memUtils_memCopy((uint8_t*)&(changeSupplyCmdData.implementationDateTime),
        (uint8_t*)&(pZCLEvent->pEventData[payloadOffset]),
        sizeof(changeSupplyCmdData.implementationDateTime));

    payloadOffset += sizeof(changeSupplyCmdData.implementationDateTime);

    changeSupplyCmdData.proposedSupplyStatus =
        pZCLEvent->pEventData[payloadOffset++];

    changeSupplyCmdData.originatorIdSupplyControlBits =
        pZCLEvent->pEventData[payloadOffset++];

    /* to find whether the request was from authorized user */
    status = App_CheckAuthorizedOriginator( );
    if(g_ZCL_Not_Authorized_c == status){
        /* originator device is not IPD or ESI */
        App_SendDefaultResponse(status ,g_FALSE_c,p_dataInd );
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
		buffMgmt_freeBuffer(BackUpApsDataBufferIndex, 125 );
#endif
    }

    /* code for validation of the change supply cmd */
    status = validateChangeSupplyCmd(changeSupplyCmdData);

    if(status != g_ZCL_Success_c ){
        /* if change supply validation failed for any reason ex:
         implementation time is not valid*/
        /*default response with status */
        App_SendDefaultResponse(status ,g_FALSE_c,p_dataInd );
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
		buffMgmt_freeBuffer(BackUpApsDataBufferIndex, 125 );
#endif
    }
    else
    {
        supplyResponse.providerID = changeSupplyCmdData.providerID;
        supplyResponse.implementationDateTime =
            changeSupplyCmdData.implementationDateTime;

        /* insert the proposed value in array if implementation
        time is not immediate*/
        if(changeSupplyCmdData.implementationDateTime != 0 &&
           changeSupplyCmdData.implementationDateTime != 0xffffffff){

            insertProposedValues(changeSupplyCmdData,
                p_dataInd->src_addr_mode , p_dataInd->src_address);
            updateServerValues();


        }
        else if(changeSupplyCmdData.implementationDateTime == 0xffffffff ){
            /* If implementationDateTime = 0xffffffff then cancel the
            proposed supply data in proposed array*/
#if(g_PROPOSED_VALUES_MAX_c > 1)
            if(proposedIndex > 0 ){
#endif /* (g_PROPOSED_VALUES_MAX_c > 1)*/
                proposedValues[0].supplyCmdValue.implementationDateTime
                    = 0xffffffff;
                supplyResponse.providerID =  proposedValues[0].supplyCmdValue.providerID;
                supplyResponse.implementationDateTime = 0;
                supplyResponse.supplyStatus = g_currentSupplyState;
#if(g_PROPOSED_VALUES_MAX_c > 1)
                sortTheProposedValues();
                /* to keep track of active proposed values in
                proposed array*/
                proposedIndex--;
#endif /* (g_PROPOSED_VALUES_MAX_c > 1)*/
                /*Cancelled the proposed event successfully */
                status = g_ZCL_Success_c ;


                /* sending response immediately*/
                if(changeSupplyCmdData.originatorIdSupplyControlBits
                    & 0x01){
                    /* Ack req field is enabled */
                    status = g_ZCL_Success_c;
                    supplyStatusResponse(p_dataInd,status,supplyResponse);

                }
#if(g_PROPOSED_VALUES_MAX_c > 1)
            }
            else{
                status = g_ZCL_Failure_c;
                /*cannot cancel proposed value */
                App_SendDefaultResponse(status ,g_FALSE_c,p_dataInd );
				buffMgmt_freeBuffer(BackUpApsDataBufferIndex, 125 );
				return;
            }
#endif  /*(g_PROPOSED_VALUES_MAX_c > 1)*/
        }
        else
        {
            /* If implementationDateTime = 0 considered as
            immediate */
            memUtils_memCopy((uint8_t*)&current_change_supply_values,
              (uint8_t*)&changeSupplyCmdData,
              sizeof(changeSupplyCmdData));
            ImplementationTimeMatch = g_TRUE_c;
            updateServerValues();

            /* updation of supply status in response according to
            current meter supply status ,after implementaion of change
            supply status command */
            supplyResponse.supplyStatus = g_currentSupplyState;

            /* sending response immediately*/
            if(changeSupplyCmdData.originatorIdSupplyControlBits
                & 0x01){
                /* Ack req field is enabled */
                status = g_ZCL_Success_c;
                supplyStatusResponse(p_dataInd,status,supplyResponse);
            }
        }
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
		buffMgmt_freeBuffer(BackUpApsDataBufferIndex, 125 );
#endif

    }
}
#endif  /* g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )*/
/******************************************************************************/
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
void App_prepaymentTask()
{

    APSDE_Data_Indication_t dataIndication;
    uint8_t tempasdu[25],payload = 0;
    uint32_t currentTime;

    SupplyStatusResCmdPayload_t supplyResponse;
    currentTime = ZCL_CallBackGetCurrentTime();

    if(currentTime < proposedValues[0].supplyCmdValue.implementationDateTime){
        return;
    }
    else
    {
        ImplementationTimeMatch = g_TRUE_c;
        memUtils_memCopy((uint8_t*)&current_change_supply_values,
                            (uint8_t*)&(proposedValues[0].supplyCmdValue),
                            sizeof(current_change_supply_values) );

        /* update the server values with the top most row of the
        proposed values, proposed array is sorted in asceding order based on
        immediate date time field*/
        updateServerValues();

        /* if ack is required by the device which asked for the
        change supply cmd then send the change supply res command */
        supplyResponse.providerID = proposedValues[0].supplyCmdValue.providerID;
        supplyResponse.implementationDateTime =
            proposedValues[0].supplyCmdValue.implementationDateTime;

        supplyResponse.supplyStatus = g_currentSupplyState;

        /* sending response to the originator device after implementation of
        the command */
        if(proposedValues[0].supplyCmdValue.originatorIdSupplyControlBits & 0x01){
            /* Ack req field is enabled */

            dataIndication.dest_addr_mode = proposedValues[0].addressMode;
            if(dataIndication.dest_addr_mode == g_Short_Address_Mode_c ){
                memUtils_memCopy((uint8_t*)&(dataIndication.dest_address),
                    (uint8_t*)&(proposedValues[0].srcAddress),0x02);
            }
            else if( dataIndication.dest_addr_mode == g_IEEE_Address_Mode_c){
                memUtils_memCopy((uint8_t*)&(dataIndication.dest_address),
                    (uint8_t*)&(proposedValues[0].srcAddress),0x08);
            }
            dataIndication.dest_endpoint = proposedValues[0].srcEndpoint;

            /* ZCL Header Creation */
            tempasdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                    g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                        g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

            tempasdu[1] = App_GetTransSeqNumber();
            tempasdu[2] = g_SupplyStatusResCmd_c;

            payload = 3;

            memUtils_memCopy((uint8_t*)&(tempasdu[3]),(uint8_t*)&supplyResponse,
                     sizeof(SupplyStatusResCmdPayload_t));
            payload += sizeof(SupplyStatusResCmdPayload_t);

            App_SendData( dataIndication.dest_addr_mode,
                (uint8_t*)&(dataIndication.dest_address),dataIndication.dest_endpoint ,
                g_PREPAYMENT_CLUSTER_c, payload,
                (uint8_t*)&(tempasdu[0] ));
        }
        /* top most row is marked as expired ,so that it is moved to
        last in array after sort*/
        proposedValues[0].supplyCmdValue.implementationDateTime = 0xffffffff;
#if(g_PROPOSED_VALUES_MAX_c > 1)
        /* sort proposedValues array */
        sortTheProposedValues();
        /* To make one entry free */
        proposedIndex--;
#endif /* (g_PROPOSED_VALUES_MAX_c != 1)*/
    }
}
/******************************************************************************/
bool App_changeSupplyState(PrepaymentSupplyStatus newSupplyState)
{
    bool stateChanged = g_FALSE_c;
    /* Assumption is, current state movement is like OFF->ARMED->ON->OFF
    in loop */
    if(g_currentSupplyState == g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_c){
        if(newSupplyState == g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_ARMED_c){
            g_currentSupplyState = g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_ARMED_c;
            stateChanged = g_TRUE_c;
        }
    }
    else if(g_currentSupplyState == g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_ON_c ) {
        if( newSupplyState == g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_c){
            g_currentSupplyState = g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_c;
            stateChanged = g_TRUE_c;
        }
    }
    else if(g_currentSupplyState == g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_ARMED_c){
        if(newSupplyState == g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_ON_c ){
            g_currentSupplyState = g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_ON_c;
            stateChanged = g_TRUE_c;
        }
    }
    return stateChanged;
}

/*******************************************************************************/
#if(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
void App_processSupplyChangeEvent( void )
{
    ZCL_Event_t ZCLEvent;
    APSDE_Data_Indication_t *pdataInd;
    uint8_t *pBufferRx;
    ZCLEvent.pEventData = g_NULL_c;

    pBufferRx = buffMgmt_getBufferPointer(BackUpZCLBufferIndex);
    if(pBufferRx != g_NULL_c ){
        ZCLEvent.eventId = tempZCL_Event.eventId;
        ZCLEvent.eventLength = tempZCL_Event.eventLength ;
        ZCLEvent.pEventData = pBufferRx;
    }
    pdataInd = (APSDE_Data_Indication_t *)buffMgmt_getBufferPointer(BackUpApsDataBufferIndex);
    if ( ( pBufferRx != g_NULL_c ) && ( pdataInd != g_NULL_c ) ) {
        App_PrepaymentCommonFunction( &ZCLEvent, pdataInd );
    }
	buffMgmt_freeBuffer(BackUpZCLBufferIndex, 174 );
    App_ResumDiscovery();

}
/*******************************************************************************/
static void App_ProcessEventIfMeteringServerIsDiabled( void )
{
    storedMatchedEndpoint = 0xFF;
    storedMatchedAddress = 0xFFFF;
    readMeterSerialNo[0] = readMeterSiteId[0]= 0x00;
    memUtils_memSet( &readMeterSerialNo[0], 0xff, 15 );
//    memUtils_memSet( &readMeterSiteId[1], 0xff, 31 );
    App_holdCurrentDiscovery( g_PREPAYMENT_DISCOVERY_STATE_c );
}
#endif  /* (g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0) */
/*******************************************************************************/
static void supplyStatusResponse(APSDE_Data_Indication_t* apsdeDataPacket,
                                 uint8_t status ,
                                 SupplyStatusResCmdPayload_t supplyResponse )
{
    uint8_t destAddress[8],payload = 0;

    /* ZCL Header Creation */
    apsdeDataPacket->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                            g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = g_SupplyStatusResCmd_c;

    payload = 3;
    if( status == g_ZCL_Success_c) {
        memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[3]),(uint8_t*)&supplyResponse,
                 sizeof(SupplyStatusResCmdPayload_t));
        payload += sizeof(SupplyStatusResCmdPayload_t);
    }

    memUtils_memCopy(destAddress , apsdeDataPacket->src_address.IEEE_address, 0x08 );

    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_PREPAYMENT_CLUSTER_c, payload,
        apsdeDataPacket->a_asdu );

}
/******************************************************************************/
static uint8_t validateChangeSupplyCmd(ChangeSupplyCmdPayload_t changeSupplyCmdData)
{
    uint32_t currentTime;

    /* requested date time and impletementation date time
    shd be greater than current time */
    currentTime = ZCL_CallBackGetCurrentTime();

    if((currentTime > changeSupplyCmdData.requestDateTime) && (
        (currentTime > changeSupplyCmdData.implementationDateTime )||
        (currentTime != 0 ))){
        return g_ZCL_Invalid_Value_c;
    }

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
    /* validating meter serial-no left to application, for now it is
    assumed that meter serial no is the current meter serial no
    from metering cluster attributes ?*/
    if(g_MEMORY_MATCH_c != (memUtils_memCompare ((uint8_t*)&(changeSupplyCmdData.meterSerialNumber[1]),
       (uint8_t*)&(simple_metering_server_cluster_values.meterSerialNumber[1]),
       changeSupplyCmdData.meterSerialNumber[0]))){
        return g_ZCL_Invalid_Value_c;
    }

    /* siteID validation can be handled in application, for now it is
    assumed that site id the current site id metering cluster attributes ? */

    if(g_MEMORY_MATCH_c != (memUtils_memCompare((uint8_t*)&(changeSupplyCmdData.siteId[1]) ,
        (uint8_t*)&(simple_metering_server_cluster_values.siteId[1]),
                             changeSupplyCmdData.siteId[0]))){
        return g_ZCL_Invalid_Value_c;
    }
#elif(g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d==0)&&(g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d==1)
    /* ask for match desciptor if other device responds with
    incluster as metering ,extract short address and read meter serial
    no and site id */

    /* Compare the serial no and site id */
    if(g_MEMORY_MATCH_c != (memUtils_memCompare ((uint8_t*)&(changeSupplyCmdData.meterSerialNumber[1]),
       (uint8_t*)&(readMeterSerialNo[1]),
       changeSupplyCmdData.meterSerialNumber[0]))){
        return g_ZCL_Invalid_Value_c;
    }
    if(g_MEMORY_MATCH_c != (memUtils_memCompare((uint8_t*)&(changeSupplyCmdData.siteId[1]) ,
        (uint8_t*)&(readMeterSiteId[1]),changeSupplyCmdData.siteId[0]))){
        return g_ZCL_Invalid_Value_c;
    }

#endif

    /* validate proposed supply status can be one of the followind states */
    if(!((changeSupplyCmdData.proposedSupplyStatus &&
       g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_c) ||
       (changeSupplyCmdData.proposedSupplyStatus &&
        g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_ARMED_c) ||
           (changeSupplyCmdData.proposedSupplyStatus &&
        g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_ON_c))){
        return g_ZCL_Invalid_Value_c;
    }
    /* validation of originator id */
    if(!(((changeSupplyCmdData.originatorIdSupplyControlBits & 0x10) ==
          g_PREPAYMENT_ORIGINATING_DEVICE_ENERGY_SERVICE_INTERFACE_c)||
         ((changeSupplyCmdData.originatorIdSupplyControlBits &0x20) ==
          g_PREPAYMENT_ORIGINATING_DEVICE_IN_PREMISES_DISPLAY_DEVICE_c ))){
        return g_ZCL_Not_Authorized_c;
    }

    if(!(prepayment_cluster_server_values.paymentControlAttribute && g_CONTACTOR_FITTED_c)){
        return g_ZCL_UnSup_Cluster_Command_c;
    }
    return g_ZCL_Success_c;
}
/******************************************************************************/
static void updateServerValues()
{
    uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
    if(ImplementationTimeMatch == g_TRUE_c){
        /* provider ID */
        prepayment_cluster_server_values.proposedChangeProviderId
            = current_change_supply_values.providerID;

        App_ReadWriteAttribute( EndPoint, g_PREPAYMENT_CLUSTER_c,
                               g_PROPOSED_CHANGE_PROVIDER_ID_ATTRIBUTE_c,
                               (uint8_t*)&(prepayment_cluster_server_values.proposedChangeProviderId),
                                                    g_Write_Attribute_Data_c );


#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )

        /* site ID */
        memUtils_memCopy((uint8_t*)&simple_metering_server_cluster_values.siteId ,
                         (uint8_t*)&current_change_supply_values.siteId[0],
                         current_change_supply_values.siteId[0] +1);

        /* meter serial no */
        memUtils_memCopy((uint8_t*)&simple_metering_server_cluster_values.meterSerialNumber ,
                         (uint8_t*)&current_change_supply_values.meterSerialNumber[0],
                         current_change_supply_values.meterSerialNumber[0]+1 );

#endif

        /* implementation date time */
        prepayment_cluster_server_values.proposedChangeImplementationTime =
            current_change_supply_values.implementationDateTime;

        App_ReadWriteAttribute( EndPoint, g_PREPAYMENT_CLUSTER_c,
            g_PROPOSED_CHANGE_IMPLEMENTATION_TIME_ATTRIBUTE_c,
            (uint8_t*)&(prepayment_cluster_server_values.proposedChangeImplementationTime) ,
                                                    g_Write_Attribute_Data_c );



        /* To change the current state of the supply*/
        App_changeSupplyState((PrepaymentSupplyStatus)
            (current_change_supply_values.proposedSupplyStatus));
        /* proposed supply status */
        prepayment_cluster_server_values.proposedChangeSupplyStatus =
            g_currentSupplyState /* current value ll be having either new state
                or the default state */;

        App_ReadWriteAttribute( EndPoint, g_PREPAYMENT_CLUSTER_c,
                               g_PROPOSED_CHANGE_SUPPLY_STATUS_ATTRIBUTE_c,
            (uint8_t*)&(prepayment_cluster_server_values.proposedChangeSupplyStatus) ,
                                                    g_Write_Attribute_Data_c );


        ImplementationTimeMatch = g_FALSE_c;
    }
}

/******************************************************************************/
#if(g_PROPOSED_VALUES_MAX_c > 1)
static void sortTheProposedValues()
{
    ProposedSupplyValues_t Temp;
    uint8_t i,j;
    for(i = 0 ;i < g_PROPOSED_VALUES_MAX_c ; i++){

        for(j = 0 ;j < g_PROPOSED_VALUES_MAX_c - i ;j++){
            if(proposedValues[j].supplyCmdValue.implementationDateTime >
               proposedValues[j+1].supplyCmdValue.implementationDateTime){
                Temp = proposedValues[j];
                proposedValues[j] = proposedValues[j+1];
                proposedValues[j+1] = Temp;
            }
        }
    }
}
#endif
/******************************************************************************/
static bool checkifPresentInProposedValues(ChangeSupplyCmdPayload_t changeSupplyVal)
{
    uint8_t index;

    for(index = 0 ;index < proposedIndex ; index++)
    {
        if((proposedValues[index].supplyCmdValue.implementationDateTime ==
           changeSupplyVal.implementationDateTime) &&
           (proposedValues[index].supplyCmdValue.providerID ==
            changeSupplyVal.providerID)){
            /* proposed value is present already*/
            return g_TRUE_c;
        }
    }
    return g_FALSE_c;
}

/******************************************************************************/
static void insertProposedValues(ChangeSupplyCmdPayload_t changeSupplyVal,
                                 uint8_t addressMode ,Address srcAddress)
{
    if(g_TRUE_c != checkifPresentInProposedValues(changeSupplyVal)){

        if(proposedIndex < g_PROPOSED_VALUES_MAX_c){

            memUtils_memCopy((uint8_t*)&(proposedValues[proposedIndex].supplyCmdValue),
                         (uint8_t*)&changeSupplyVal,
                         sizeof(ChangeSupplyCmdPayload_t));
            /* src address mode */
            proposedValues[proposedIndex].addressMode = addressMode;
            if(addressMode == g_Short_Address_Mode_c ){
                memUtils_memCopy((uint8_t*)(proposedValues[proposedIndex].srcAddress),
                             (uint8_t*)&srcAddress,0x02);
            }
            else if(addressMode == g_IEEE_Address_Mode_c){
                memUtils_memCopy((uint8_t*)(proposedValues[proposedIndex].srcAddress),
                                 (uint8_t*)&srcAddress,0x08);
            }
            /* insert src endpoint */
            proposedValues[proposedIndex].srcEndpoint =
                ZCL_Callback_Get_Device_Src_EndPoint();

#if(g_PROPOSED_VALUES_MAX_c > 1)
            proposedIndex++;
            if(proposedIndex == g_PROPOSED_VALUES_MAX_c ){
                proposedIndex = 0; /* Array Roll back*/
            }
            sortTheProposedValues();
#endif /*(g_PROPOSED_VALUES_MAX_c > 1)*/
        }
    }
}
/******************************************************************************/
static uint8_t App_CheckAuthorizedOriginator( )
{
    /* Checking the authorization of the originator device is left to the
    application ,if not authorized can return g_ZCL_Not_Authorized*/

    return g_ZCL_Success_c;
}
#endif
/******************************************************************************/
#endif  /*g_PREPAYMENT_CLUSTER_ENABLE_d == 1 */
/******************************************************************************/
