/*******************************************************************************
* AppSimpleMeterUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting SimpleMeter cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "memory_utility.h"
#include "stack_common.h"

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ApplicationThinLayer.h"
#include "AppSimpleMeterUtilities.h"
#include "ZCL_Simple_Metering_Cluster.h"

#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Basic_Cluster.h"
#endif
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
#include "AppPriceClusterUtilities.h"
#endif
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_CUURENT_TIME_c                                    0x00000000
#define m_GET_PROFILE_RESP_FIXED_FIELDS_LEN_c               0x0A
#define m_REQUEST_OR_REMOVE_MIRROR_RESP_LEN_c               0x05
#define m_REMOVE_MIRROR_RESP_LEN_c                          0xFF
#define m_INVALID_END_POINT_c                               0xFF
#define m_REQ_MIRROR_RESPONSE_INDEX_c                       0x03
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )

App_ProfileMeteringIntervals_t MeteringProfileData[] =
{
    { 0x00000000, {0x00, 0x00, 0x00 }},
    { 0x00000000, {0x00, 0x00, 0x00 }},
    { 0x00000000, {0x00, 0x00, 0x00 }},
    { 0x00000000, {0x00, 0x00, 0x00 }},
    { 0x00000000, {0x00, 0x00, 0x00 }}
};
Meter_status_t Meter_status = { g_FALSE_c,g_FALSE_c, 0x00000000};


Mirror_Info_t Mirror_Info = { g_FALSE_c, m_INVALID_END_POINT_c, g_FALSE_c };

#if ( g_SE_PROFILE_1_1_d == 1 )
App_fastPollModeIntervals_t  fastPollIntervals;
#endif

#endif
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
static void App_UpdateProfileIntervalData (
                    App_ProfileMeteringIntervals_t *profileMeteringIntervals );

static void App_AddMeterConsumptionData ( uint8_t *data, uint8_t len,
                                          uint8_t consumption );

static void App_CreatAndSend_Get_Profile_Response( uint8_t *getProfileData,
                                                   APSDE_Data_Indication_t *pBuffer );

uint16_t randomT( void );

#if ( g_SE_PROFILE_1_1_d == 1 )
static void App_createAndSendFastPollModeResponse( uint8_t *fastPollModeData,
					APSDE_Data_Indication_t *pBuffer );
static uint8_t App_getDefaultPeriod(void);
static void App_updateInstantaneousDemand(void);

#endif /* ( g_SE_PROFILE_1_1_d == 1 )*/
#endif      /* ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
static void App_CreatAndSend_Request_Mirror_Response( uint8_t *requestMirrorData,
                                                      APSDE_Data_Indication_t *pBuffer );

static void App_CreatAndSend_Mirror_Removed_Response( uint8_t *removeMirrorData,
                                                      APSDE_Data_Indication_t *pBuffer );
#endif   /* ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
void App_MeterClusterInit( void )
{
    memUtils_memSet( (uint8_t*)&MeteringProfileData, 0x00,
             sizeof( App_ProfileMeteringIntervals_t )* g_METER_NUM_PROFILES_SUPPORTED_c );
    memUtils_memSet((uint8_t*)&Meter_status, 0x00, sizeof( Meter_status_t ) );

#if ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 )
    /* For Meter server cluster we need to store profile data. If Time server cluster
       is supported on on same device then Update Enable profile parameters, else if
       Time client cluster is supported then Enabling of profile parameters are done
       when device sync with time server. */
    Meter_status.Meterflag = g_TRUE_c;
    Meter_status.MeterDataCapturedTime = ZCL_CallBackGetCurrentTime();
#if ( g_SE_PROFILE_1_1_d == 1 )
	/* for fast  poll method */
	fastPollIntervals.fastPollFlag =g_FALSE_c;
	fastPollIntervals.PollCapturedTime =  ZCL_CallBackGetCurrentTime();
	fastPollIntervals.updatePeriod = App_getDefaultPeriod();
#endif /* ( g_SE_PROFILE_1_1_d == 1 ) */
#endif     /* ( g_TIME_SERVER_CLUSTER_ENABLE_d == 1 ) */

}
/**************************************************************************************/

void App_MeterClusterTask( void )
{
    uint32_t currentTime = ZCL_CallBackGetCurrentTime();

    if ( Meter_status.Meterflag == g_TRUE_c ) {
        if ( ( currentTime - Meter_status.MeterDataCapturedTime ) >= g_APP_PROFILE_INTERVAL_PERIOD_c ) {
                App_Update_MeterReadings();
                Meter_status.MeterDataCapturedTime = currentTime;
				Meter_status.MeterReadingFlag = g_TRUE_c;
        }
    }
#if ( g_SE_PROFILE_1_1_d == 1 )
	if ( fastPollIntervals.fastPollFlag == g_TRUE_c) {
	  /* fast poll mode is activated */
        if ( ( currentTime -fastPollIntervals.PollCapturedTime ) >= fastPollIntervals.updatePeriod ) {
		/* If the fast poll mode is activated, for every fast poll intervals
		 instantaneous demand attribute will  be updated */
			App_updateInstantaneousDemand();
			fastPollIntervals.PollCapturedTime = currentTime;
        }
		if(currentTime >= fastPollIntervals.endTime)
		{
		  /* if the current time is greater tahn the fast poll end time,
		  	reset the fast poll mode*/
			fastPollIntervals.fastPollFlag = g_FALSE_c;
			fastPollIntervals.endTime = 0x00000000;
			fastPollIntervals.PollCapturedTime =  currentTime;
			fastPollIntervals.updatePeriod = App_getDefaultPeriod();
		}

	}
	else {

	  /* when not in fast poll mode, for every default period
	  	instantaneous demand will be updated */
	   if ( ( currentTime -fastPollIntervals.PollCapturedTime ) >= fastPollIntervals.updatePeriod ) {
		   fastPollIntervals.PollCapturedTime = currentTime;
		   fastPollIntervals.updatePeriod = App_getDefaultPeriod();
		   App_updateInstantaneousDemand();
	   }

	}
#endif /*  ( g_SE_PROFILE_1_1_d == 1 )*/
}

#endif      /* ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 ) */
/**************************************************************************************/

void App_MeterEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
    switch ( pZCLEvent->eventId )
    {
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
    case  g_Get_Profile_Event_c :
        App_CreatAndSend_Get_Profile_Response( pZCLEvent->pEventData, pBuffer );
        break;
    case g_Request_Mirror_Response_Event_c:
    {
        uint16_t endPoint;
        memUtils_memCopy( (uint8_t*)&endPoint, pBuffer->a_asdu + m_REQ_MIRROR_RESPONSE_INDEX_c, 0x02 );
        if ( endPoint == 0xFFFF ) {

            Mirror_Info.HasMirrorCapability  = g_FALSE_c;
            Mirror_Info.MirrorEndPoint       = m_INVALID_END_POINT_c;
            Mirror_Info.deviceMirroring      = g_FALSE_c;
        }
        else {
            Mirror_Info.HasMirrorCapability  = g_TRUE_c;
            Mirror_Info.MirrorEndPoint       = endPoint;
            Mirror_Info.deviceMirroring    = g_TRUE_c;
        }
        break;
    }
    case g_Remove_Mirror_Response_Event_c:
        Mirror_Info.deviceMirroring = g_FALSE_c;
		break;
#if ( g_SE_PROFILE_1_1_d == 1 )
	case g_Request_Fast_Poll_Mode_Event_c:
	  if ( fastPollIntervals.fastPollFlag == g_FALSE_c) {
	  App_createAndSendFastPollModeResponse ( pZCLEvent->pEventData, pBuffer );
	  }

        break;
#endif /* ( g_SE_PROFILE_1_1_d == 1 )*/
#endif

#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
    case  g_Get_Profile_Respnse_Event_c :
         /* Application should handle reception of Get profile response command */
        break;
    case g_Request_Mirror_Event_c:
        App_CreatAndSend_Request_Mirror_Response( pZCLEvent->pEventData, pBuffer );
        break;
    case g_Remove_Mirror_Event_c:
        App_CreatAndSend_Mirror_Removed_Response( pZCLEvent->pEventData,  pBuffer );
        break;
#if(g_SE_PROFILE_1_1_d == 1)
    case g_Request_Fast_Poll_Mode_Response_Event_c:
        break;
#endif /*  ( g_SE_PROFILE_1_1_d == 1 )*/

#endif
    default :
        break;
    }
}

/**************************************************************************************/
#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
static void App_CreatAndSend_Request_Mirror_Response( uint8_t *requestMirrorData,
                                                      APSDE_Data_Indication_t *pBuffer )
{

    /* ZCL Header Creation */
    pBuffer->a_asdu[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c;
    pBuffer->a_asdu[1] = App_GetTransSeqNumber();
    pBuffer->a_asdu[2] = g_Request_Mirror_Response_c;

#if ( g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_ENABLE_d == 0x01 )
    App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_BASIC_CLUSTER_c,
                                g_PHYSICAL_ENVIRONMENT_ATTRIBUTE_c,
                                (uint8_t*)&Mirror_Info.HasMirrorCapability,
                                g_Read_Attribute_Data_c );
#endif

    if ( Mirror_Info.HasMirrorCapability ) {
        pBuffer->a_asdu[3] = pBuffer->dest_endpoint;
        pBuffer->a_asdu[4] = 0x00;
        Mirror_Info.MirrorEndPoint = pBuffer->dest_endpoint;
        Mirror_Info.deviceMirroring = g_TRUE_c;
    }
    else {
        pBuffer->a_asdu[3] = m_INVALID_END_POINT_c;
        pBuffer->a_asdu[4] = m_INVALID_END_POINT_c;
        Mirror_Info.deviceMirroring = g_FALSE_c;
    }
    App_SendData( pBuffer->src_addr_mode,
         pBuffer->src_address.IEEE_address,
         pBuffer->src_endpoint,
         g_SIMPLE_METERING_CLUSTER_c,
         m_REQUEST_OR_REMOVE_MIRROR_RESP_LEN_c,
         pBuffer->a_asdu );
}

/**************************************************************************************/
static void App_CreatAndSend_Mirror_Removed_Response( uint8_t *removeMirrorData,
                                                      APSDE_Data_Indication_t *pBuffer )
{
        /* ZCL Header Creation */
    pBuffer->a_asdu[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c;
    pBuffer->a_asdu[1] = App_GetTransSeqNumber();
    pBuffer->a_asdu[2] = g_Remove_Mirror_Response_c;

    if ( Mirror_Info.deviceMirroring ) {
        pBuffer->a_asdu[3] = Mirror_Info.MirrorEndPoint;
        pBuffer->a_asdu[4] = 0x00;
        Mirror_Info.deviceMirroring = g_FALSE_c;
    }
    else  {
        pBuffer->a_asdu[3] = m_INVALID_END_POINT_c;
        pBuffer->a_asdu[4] = m_INVALID_END_POINT_c;
    }
    Mirror_Info.MirrorEndPoint = m_INVALID_END_POINT_c;

    App_SendData( pBuffer->src_addr_mode,
         pBuffer->src_address.IEEE_address,
         pBuffer->src_endpoint,
         g_SIMPLE_METERING_CLUSTER_c,
         m_REQUEST_OR_REMOVE_MIRROR_RESP_LEN_c,
         pBuffer->a_asdu );
}
#endif /* ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/**************************************************************************************/
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
static void App_CreatAndSend_Get_Profile_Response( uint8_t *getProfileData,
                                                   APSDE_Data_Indication_t *pBuffer )
{
    uint8_t i, j;
    uint8_t MeterProfileIndex = 0x00;
    uint8_t numOfAvailableIntervals = 0x00;
    uint8_t asduLength;
#if ( g_MAX_NUMBER_OF_PERIODS_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
    uint16_t numOfPeriods;
#endif
    uint32_t currentTime = ZCL_CallBackGetCurrentTime();

    Get_Profile_Response_Command_t *getProfileRspCmdPayload;
    Get_Profile_Command_t *getProfileCmdPayloadReceived;

    getProfileCmdPayloadReceived = (Get_Profile_Command_t*)getProfileData;

    getProfileRspCmdPayload = (Get_Profile_Response_Command_t*)&pBuffer->a_asdu[3];

    /* If GetProfileRequest command is received with endTime as 0x00000000 then we
       have to consider endTime is currentTime.*/
    if( getProfileCmdPayloadReceived->endTime == m_CUURENT_TIME_c ) {
        getProfileCmdPayloadReceived->endTime = currentTime;
    }
    asduLength = m_GET_PROFILE_RESP_FIXED_FIELDS_LEN_c;

    /* ZCL Header Creation */
    pBuffer->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c;
    pBuffer->a_asdu[1] = App_GetTransSeqNumber();
    pBuffer->a_asdu[2] = g_Get_Profile_Response_c;

    /* It will be UPDATED when GetProfileResponse as Success */
    getProfileRspCmdPayload->noOfPeriods = 0x00;
    getProfileRspCmdPayload->endTime = getProfileCmdPayloadReceived->endTime;
    /* Currently ProfileIntervalPeriod is assigned with 2.5 minutes */
    getProfileRspCmdPayload->profileIntervalPeriod = g_MeteringTwoAndHalfMinutes_c;

#if ( g_MAX_NUMBER_OF_PERIODS_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
    {
    App_ReadWriteAttribute( pBuffer->dest_endpoint,
                    g_SIMPLE_METERING_CLUSTER_c,
                    g_MAX_NUMBER_OF_PERIODS_DELIVERED_c,
                    (uint8_t*)&numOfPeriods,
                    g_Read_Attribute_Data_c );
    }
#endif
    if( ( getProfileCmdPayloadReceived->intervalChannel != g_MeteringConsumptionDelivered_c ) &&
        ( getProfileCmdPayloadReceived->intervalChannel != g_MeteringConsumptionReceived_c  ) ) {

          getProfileRspCmdPayload->status = g_MeteringUndefinedIntervalChannelRequested_c;
    }
    else if ( getProfileCmdPayloadReceived->endTime > currentTime ) {
        getProfileRspCmdPayload->status = g_MeteringInvalidEndTime_c;
    }
#if ( g_MAX_NUMBER_OF_PERIODS_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
    /* More periods requested than can be returned */
    else if ( getProfileCmdPayloadReceived->noOfPeriods > numOfPeriods ){
         getProfileRspCmdPayload->status = g_MeteringMorePeriodsRequestedThanCanBeReturned_c;
    }
#endif
    else {
        /* Check the endTime below captured Intrvals is existed in MeteringProfileData
           array.If existed, copy the data to response paayload else number of profile
           data made zero */
        for( i = 0x00; i < g_METER_NUM_PROFILES_SUPPORTED_c; i++ ) {
           if( ( getProfileCmdPayloadReceived->endTime >= MeteringProfileData[i].utcTime )&&
               ( 0x00000000 != MeteringProfileData[i].utcTime ) ) {

                MeterProfileIndex = i;
                getProfileRspCmdPayload->endTime = MeteringProfileData[MeterProfileIndex].utcTime;
                getProfileRspCmdPayload->status = g_MeteringIntervalSuccess_c;
                break;
            }
        }
        if( i == g_METER_NUM_PROFILES_SUPPORTED_c ) {
            numOfAvailableIntervals = 0x00;
            getProfileRspCmdPayload->status = g_MeteringNoIntervalsAvailableForTheRequestedTime_c;
        }
        if( g_MeteringIntervalSuccess_c == getProfileRspCmdPayload->status ) {

            for( i = MeterProfileIndex,j = 0x00; i < g_METER_NUM_PROFILES_SUPPORTED_c; i++,j++ ) {

                if( MeteringProfileData[i].utcTime ) {
                    memUtils_memCopy((uint8_t*)&(getProfileRspCmdPayload->intevals[j].metringInterval[0]),
                            (uint8_t*)&(MeteringProfileData[i].metringInterval[0]), 0x03 );
                    numOfAvailableIntervals++;
                }
            }
            /* Received GetProfileRequest Command,no.of periods delivered /received is less
              than numOfAvailableIntervals in MeteringProfileData array.
              Then we need send in response frame with requested no.of Intervals.otherwise
              we need send in response frame with numOfAvailableIntervals */
            if ( getProfileCmdPayloadReceived->noOfPeriods < numOfAvailableIntervals ){
                getProfileRspCmdPayload->noOfPeriods = getProfileCmdPayloadReceived->noOfPeriods;
            }
            else {
                getProfileRspCmdPayload->noOfPeriods = numOfAvailableIntervals;
            }
            asduLength += getProfileRspCmdPayload->noOfPeriods * 0x03;

        }
    }
    App_SendData( pBuffer->src_addr_mode,
             pBuffer->src_address.IEEE_address,
             pBuffer->src_endpoint,
             g_SIMPLE_METERING_CLUSTER_c,
             asduLength,
             pBuffer->a_asdu );
}


/**************************************************************************************/
static void  App_UpdateProfileIntervalData(
                        App_ProfileMeteringIntervals_t *profileMeteringIntervals )
{
    uint8_t i;
    /* Received profileMeteringIntervals should be stored at 0 index
       0->1,1->2,2->3,3->4........Last Index value will be removed */

    for ( i = g_METER_NUM_PROFILES_SUPPORTED_c - 1; i > 0; i -- ) {

        memUtils_memCopy ( (uint8_t*)&MeteringProfileData[i].utcTime,
                   (uint8_t*)&MeteringProfileData[i-1].utcTime, 0x04 );

        memUtils_memCopy ( (uint8_t*)&MeteringProfileData[i].metringInterval[0],
            (uint8_t*)&MeteringProfileData[i-1].metringInterval[0], 0x03 );

    }
    memUtils_memCopy ( (uint8_t*)&(MeteringProfileData[0].utcTime),
               (uint8_t*)&(profileMeteringIntervals->utcTime), 0x04 );

    memUtils_memCopy( (uint8_t*)&(MeteringProfileData[0].metringInterval[0]),
               (uint8_t*)&(profileMeteringIntervals->metringInterval[0]), 0x03 );
}


/**************************************************************************************/
static void App_AddMeterConsumptionData ( uint8_t*pdata, uint8_t len,
                                          uint8_t consumption )
{
    uint16_t carryOver = 0;
    uint8_t i;
    carryOver = (uint16_t)( pdata[0]+ consumption);
    pdata[0] = (uint8_t)carryOver;
    /* Increments the meter reading according to the consumption*/

    for( i = 0x01 ; i < len; i++ ) {
        carryOver = (uint16_t)( ( pdata[i] ) + (carryOver>>8));
        pdata[i] = (uint8_t)carryOver;
    }
 }


/**************************************************************************************/
uint8_t App_Update_MeterReadings( void )
{
    uint16_t attributeId= 0x0000;
    //uint8_t meterReading[15];
	uint8_t meterReading[6] = {0x00, 0x00, 0x00, 0x00, 0x00,0x00};
    //uint8_t consumption = (uint8_t)randomT();
    uint8_t consumption = 0;
    uint8_t status = g_ZCL_Failure_c;
    uint8_t currentPartialProfileMeterReading[3]= {0x00, 0x00, 0x00};
    uint32_t currentPartialProfileStartTime;
    App_ProfileMeteringIntervals_t profileMeteringIntervals;
    uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
    uint8_t currentTier = 0xFF;
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
    currentTier = App_GetCurrentTier();
#endif
    switch( currentTier )
    {
        case 1:
            attributeId = g_CURRENT_TIER1_SUMMATION_DELIVERED_c;
        break;

        case 2:
            attributeId = g_CURRENT_TIER2_SUMMATION_DELIVERED_c;
        break;

        case 3:
            attributeId = g_CURRENT_TIER3_SUMMATION_DELIVERED_c;
        break;

        case 4:
            attributeId = g_CURRENT_TIER4_SUMMATION_DELIVERED_c;
        break;

        case 5:
            attributeId = g_CURRENT_TIER5_SUMMATION_DELIVERED_c;
        break;

        case 6:
            attributeId = g_CURRENT_TIER6_SUMMATION_DELIVERED_c;
        break;
#if ( g_SE_PROFILE_1_1_d == 1 )
         case 7:
            attributeId = g_CURRENT_TIER7_SUMMATION_DELIVERED_c;
        break;

		 case 8:
            attributeId = g_CURRENT_TIER8_SUMMATION_DELIVERED_c;
        break;

		case 9:
            attributeId = g_CURRENT_TIER9_SUMMATION_DELIVERED_c;
        break;

		case 10:
            attributeId = g_CURRENT_TIER10_SUMMATION_DELIVERED_c;
        break;

		case 11:
            attributeId = g_CURRENT_TIER11_SUMMATION_DELIVERED_c;
        break;

		case 12:
            attributeId = g_CURRENT_TIER12_SUMMATION_DELIVERED_c;
        break;

		case 13:
            attributeId = g_CURRENT_TIER13_SUMMATION_DELIVERED_c;
        break;

		case 14:
            attributeId = g_CURRENT_TIER14_SUMMATION_DELIVERED_c;
        break;

		case 15:
            attributeId = g_CURRENT_TIER15_SUMMATION_DELIVERED_c;
        break;
#endif /* #if ( g_SE_PROFILE_1_1_d == 1 )*/
        default :
           break;
    }
    status = App_ReadWriteAttribute( EndPoint,
                                g_SIMPLE_METERING_CLUSTER_c,
                                attributeId,
                                meterReading,
                                g_Read_Attribute_Data_c );

    if ( status == g_ZCL_Success_c ) {

        App_AddMeterConsumptionData( meterReading, 0x06, consumption );

        status = App_ReadWriteAttribute( EndPoint,
                            g_SIMPLE_METERING_CLUSTER_c,
                            attributeId,
                            meterReading,
                            g_Write_Attribute_Data_c );

        status = App_ReadWriteAttribute( EndPoint,
                            g_SIMPLE_METERING_CLUSTER_c,
                            g_CURRENT_SUMMATION_DELIVERED_c,
                            meterReading,
                            g_Read_Attribute_Data_c );

        App_AddMeterConsumptionData( meterReading, 0x06, consumption );

        status = App_ReadWriteAttribute( EndPoint,
                            g_SIMPLE_METERING_CLUSTER_c,
                            g_CURRENT_SUMMATION_DELIVERED_c,
                            meterReading,
                            g_Write_Attribute_Data_c );


        status = App_ReadWriteAttribute( EndPoint,
                                g_SIMPLE_METERING_CLUSTER_c,
                                g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_c,
                                currentPartialProfileMeterReading,
                                g_Read_Attribute_Data_c );

        /* Before Updating CurrentPartialProfileIntervalDeliverd value,we need to copy the
           current value into profileIntervalArray.This array is used to send
           GetProfileResponse Command  */

        memUtils_memCopy ( (uint8_t*)&profileMeteringIntervals.metringInterval,
                   currentPartialProfileMeterReading, 0x03 );

        /* Consumption is generated by using rand() function.Assuming, Generated value as
           one byte value.So lower byte is intialized with consumption */

        currentPartialProfileMeterReading[0] = consumption;
        currentPartialProfileMeterReading[1] = 0x00;
        currentPartialProfileMeterReading[2] = 0x00;

        status = App_ReadWriteAttribute( EndPoint,
                        g_SIMPLE_METERING_CLUSTER_c,
                        g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_c,
                        currentPartialProfileMeterReading,
                        g_Write_Attribute_Data_c );

           /* This function is used to get the index of the NVM where the Metering cluster
             attributes are stored */

        status = App_ReadWriteAttribute( EndPoint,
                    g_SIMPLE_METERING_CLUSTER_c,
                    g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_c,
                    (uint8_t*)&currentPartialProfileStartTime,
                    g_Read_Attribute_Data_c );

        profileMeteringIntervals.utcTime = currentPartialProfileStartTime;

        currentPartialProfileStartTime = ZCL_CallBackGetCurrentTime();

        status = App_ReadWriteAttribute( EndPoint,
                    g_SIMPLE_METERING_CLUSTER_c,
                    g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_c,
                    (uint8_t*)&currentPartialProfileStartTime,
                    g_Write_Attribute_Data_c );

        App_UpdateProfileIntervalData( &profileMeteringIntervals );
    }
    return status;
}

/**************************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )
static void App_createAndSendFastPollModeResponse( uint8_t *fastPollModeData,
                                               APSDE_Data_Indication_t *pBuffer )
{
	uint32_t currentTime = ZCL_CallBackGetCurrentTime();
	Request_Fast_Poll_Mode_Response_t *fastPollModeRspCmdPayload;
	static uint32_t fastPollEndTimeUtc = 0;
	uint8_t fastPollUpdatePeriod = 0;
	uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
	Request_Fast_Poll_Mode_t *fastPollModeDataCmdPayloadReceived;
    uint8_t asduLength;
    fastPollModeDataCmdPayloadReceived = (Request_Fast_Poll_Mode_t*)fastPollModeData;
    fastPollModeRspCmdPayload = (Request_Fast_Poll_Mode_Response_t*)&pBuffer->a_asdu[3];

    /* ZCL Header Creation */
    pBuffer->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c;
    pBuffer->a_asdu[1] = App_GetTransSeqNumber();
    pBuffer->a_asdu[2] = g_Request_Fast_Poll_Mode_Response_c;

	/* read fast poll update period attribute value */
	App_ReadWriteAttribute( EndPoint,
                            g_SIMPLE_METERING_CLUSTER_c,
                            g_FAST_POLL_UPDATE_PERIOD_c,
                            &fastPollUpdatePeriod,
                            g_Read_Attribute_Data_c );

    /* check whether the requested fastpollupdateperiod is less than
	fastpollupdateperiod attribute  value  */
	if( fastPollModeDataCmdPayloadReceived->fastPollUpdatePeriod < fastPollUpdatePeriod  )  {

	/* if yes then  appliedupdate period is fastpollupdateperiod attribute value*/
	  	   fastPollModeRspCmdPayload->appliedUpdatePeriod = fastPollUpdatePeriod;
	}
	else {
	/*
	  Otherwise, the Applied Update Period shall be greater than or  equal to
	  the minimum Fast Poll Update Period Attribute and less than or equal to
	  the Requested Fast Poll Rate      */

	      fastPollModeRspCmdPayload->appliedUpdatePeriod =
			fastPollModeDataCmdPayloadReceived->fastPollUpdatePeriod;
	}
    /* there should not be repeated fast poll request till the existing request
	should end its task */
//	if (currentTime > fastPollEndTimeUtc) {

	  /* Maximum fast poll duration is 15 minutes*/
		 if( fastPollModeDataCmdPayloadReceived->duration >g_FIFTEEN_MINUTES_c )
		 {
		       	fastPollEndTimeUtc = currentTime+ g_FIFTEEN_MINUTES_c*g_SIXTY_SECONDS_c;
			    memUtils_memCopy( (uint8_t*)&fastPollModeRspCmdPayload->fastPollModeEndTime ,
               (uint8_t*)&fastPollEndTimeUtc, 0x04 );
		 }
		 else {

		   	fastPollEndTimeUtc =  currentTime + fastPollModeDataCmdPayloadReceived->duration *g_SIXTY_SECONDS_c ;
			memUtils_memCopy( (uint8_t*)&fastPollModeRspCmdPayload->fastPollModeEndTime ,
					(uint8_t*)&fastPollEndTimeUtc, 0x04 );
		 }

//	}
     	 /* data for  processing fast poll mode  */
	 	/*enable  fast poll mode*/
		fastPollIntervals.fastPollFlag =g_TRUE_c;
		/* update period with fast poll update period*/
		fastPollIntervals.updatePeriod = fastPollModeRspCmdPayload->appliedUpdatePeriod;
		/* update end time  with fast poll mode end time*/
		memUtils_memCopy(  (uint8_t*)&fastPollIntervals.endTime,
				 (uint8_t*)&fastPollModeRspCmdPayload->fastPollModeEndTime ,
		 		  0x04 );
		fastPollIntervals.PollCapturedTime =  ZCL_CallBackGetCurrentTime();

		asduLength = 0x08;
		App_SendData( pBuffer->src_addr_mode,
				 pBuffer->src_address.IEEE_address,
				 pBuffer->src_endpoint,
				 g_SIMPLE_METERING_CLUSTER_c,
				 asduLength,
				 pBuffer->a_asdu );


}

/*******************************************************************************/
/*whenever there is update from meter resding,
App_updateInstantaneousDemand() function is called  */
/*******************************************************************************/
static void App_updateInstantaneousDemand(void)
{
    uint8_t consumption=0x00;
    uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
    uint8_t instantaneousDemandUpdate[3];
    uint8_t maxInstantaneousDemandDelivered[6];
    uint8_t currentInstantaneousDemandDelivered[3];

    uint32_t currentInstantaneousDemand=0;
    static uint32_t maxInstantaneousDemand =0;

    currentInstantaneousDemand = (uint32_t) instantaneousDemandUpdate;
    memUtils_memCopy( (uint8_t*)&maxInstantaneousDemandDelivered ,
                                            0x00, 0x06);
      /*Update Istantaneous Demand whenver there is meter reading*/
	if( Meter_status.MeterReadingFlag )
	{
        consumption = (uint8_t)randomT();
        currentInstantaneousDemand = consumption;

        Meter_status.MeterReadingFlag =g_FALSE_c;

        memUtils_memCopy( (uint8_t*)&instantaneousDemandUpdate ,
                 (uint8_t*)&currentInstantaneousDemand, 0x03);

        memUtils_memCopy( (uint8_t*)&currentInstantaneousDemandDelivered ,
                                    (uint8_t*)&currentInstantaneousDemand, 0x03);

        App_ReadWriteAttribute( EndPoint, g_SIMPLE_METERING_CLUSTER_c,
                                                    g_INSTANTANEOUS_DEMAND_c,
                                                    instantaneousDemandUpdate,
                                                    g_Write_Attribute_Data_c );
         /*Read Current InstantaneousDemandDelivered */
        App_ReadWriteAttribute( EndPoint, g_SIMPLE_METERING_CLUSTER_c,
                                              g_CURRENT_DEMAND_DELIVERED_c,
                                              currentInstantaneousDemandDelivered,
                                              g_Write_Attribute_Data_c );
        /* if the currentInstantaneousDemand is greater than
         maxInstantaneousDemand , then update
         maxInstantaneousDemand as currentInstantaneousDemand*/
        if( maxInstantaneousDemand < currentInstantaneousDemand ){
            maxInstantaneousDemand = currentInstantaneousDemand;
        }
        memUtils_memCopy( (uint8_t*)&maxInstantaneousDemandDelivered ,
                                    (uint8_t*)&maxInstantaneousDemand, 0x06);
        App_ReadWriteAttribute( EndPoint, g_SIMPLE_METERING_CLUSTER_c,
                                          g_CURRENT_MAX_DEMAND_DELIVERED_c,
                                          maxInstantaneousDemandDelivered,
                                          g_Write_Attribute_Data_c );

    }
}
/**************************************************************************************/

static uint8_t App_getDefaultPeriod(void)
{

	uint8_t pollUpdatePeriod = 0;
	uint8_t EndPoint = ZCL_Callback_Get_Device_EndPoint();
    /* Read Default Update Period for current endpoint*/
    App_ReadWriteAttribute( EndPoint,
                            g_SIMPLE_METERING_CLUSTER_c,
                            g_DEFAULT_UPDATE_PERIOD_c,
                            &pollUpdatePeriod,
                            g_Read_Attribute_Data_c );

    return 	pollUpdatePeriod;
}
#endif /*  ( g_SE_PROFILE_1_1_d == 1 )*/
#endif  /* g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d*/

/***************************************************************************
* Private Functions
*****************************************************************************/

/* None */

/****************************************************************************
* Interrupt Service Routines
****************************************************************************/

/* None */

#endif
/****************************************************************************
* End Of File
****************************************************************************/
