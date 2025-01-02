/*******************************************************************************
* ZCL_DRLC_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting DRLC cluster
*
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_DRLC_CLUSTER_ENABLE_d == 1 )
#include "ZCL_DRLC_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/
#define m_DRLC_LCE_CMD_PAYLOAD_POSITION_c                   0x03


/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_DRLC_Client_Cluster_Attribute_Info [] =
{
    { g_UTILITY_ENROLEMENT_GROUP_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c },

    { g_START_RANDOMIZE_MINUTES_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c },

    { g_STOP_RANDOMIZE_MINUTES_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c },

    { g_DEVICE_CLASS_VALUE_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
};

const ZCL_Cluster_Info_t ga_DRLC_Client_Cluster_Info =
{
   g_DRLC_CLUSTER_c,
   sizeof( ma_DRLC_Client_Cluster_Attribute_Info )/ sizeof(ZCL_Attribute_Info_t),
   ma_DRLC_Client_Cluster_Attribute_Info
};
#endif          /*  ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */


#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_DRLC_Server_Cluster_Info =
{
   g_DRLC_CLUSTER_c,
   g_NULL_c,
   g_NULL_c,
};

#endif          /*  ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 ) */


/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/* None */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* None */

/*******************************************************************************
* Public Functions
*******************************************************************************/

uint8_t ZCL_Handle_DRLC_Cluster_Cmd ( uint8_t endPointId,
                                      uint8_t asduLength,
                                      uint8_t* pReceivedAsdu,
                                      uint8_t* pResponseLocation,
                                      ZCL_ActionHandler_t *pActionHandler,
                                      ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                      uint8_t * pAttrsDataBaseLocation )
{
    pActionHandler->action = Received_Response;

    if( ( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) ) {
        /* Initiate the action to received response, since the commands
           are received */
#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )

        if ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_LoadControlEventCmd_c)
        /* Events are handled by the application.*/
        {
            LoadControlEvent_t* pLCE = (LoadControlEvent_t*)pActionHandler->event.pEventData;
            uint8_t index = m_DRLC_LCE_CMD_PAYLOAD_POSITION_c;

            pActionHandler->event.eventId = g_DRLC_LoadControlEvent_c;

            memUtils_memCopy( (uint8_t*)&pLCE->eventId, pReceivedAsdu + index , 0x04 );
            index += 0x04;
            memUtils_memCopy( (uint8_t*)&pLCE->deviceClass, pReceivedAsdu + index, 0x02 );
            index += 0x02;
            pLCE->utilityEnrollmentGroup = *(pReceivedAsdu + index);
            index += 0x01;
            memUtils_memCopy( (uint8_t*)&pLCE->startTime, pReceivedAsdu + index, 0x04 );
            index += 0x04;
            memUtils_memCopy( (uint8_t*)&pLCE->duration, pReceivedAsdu + index, 0x02 );
            index += 0x02;
            pLCE->criticalityLevel = *(pReceivedAsdu + index);
            index += 0x01;
            pLCE->coolingTempOffset = *(pReceivedAsdu + index);
            index += 0x01;
            pLCE->heatingTempOffset = *(pReceivedAsdu + index);
            index += 0x01;
            memUtils_memCopy( (uint8_t*)&pLCE->coolingTempSetPoint, pReceivedAsdu + index, 0x02 );
            index += 0x02;
            memUtils_memCopy( (uint8_t*)&pLCE->heatingTempSetPoint, pReceivedAsdu + index, 0x02 );
            index += 0x02;
            pLCE->avgLoadPercentage = *(pReceivedAsdu + index);
            index += 0x01;
            pLCE->dutyCycle = *(pReceivedAsdu + index);
            index += 0x01;
            pLCE->eventControl = *(pReceivedAsdu + index);
        }
		else if ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_CancelLoadControlEventCmd_c)
        {
            cancelLoadControlEvent_t * pCancelLCE = (cancelLoadControlEvent_t *)
                pActionHandler->event.pEventData;

            uint8_t index = m_DRLC_LCE_CMD_PAYLOAD_POSITION_c;

            pActionHandler->event.eventId = g_DRLC_CancelLoadControlEvent_c ;

            memUtils_memCopy( (uint8_t*)&pCancelLCE->eventId, pReceivedAsdu + index, 0x04 );
            index += 0x04;
            memUtils_memCopy( (uint8_t*)&pCancelLCE->deviceClass, pReceivedAsdu + index, 0x02 );
            index += 0x02;
            pCancelLCE->utilityEnrollmentGroup = *(pReceivedAsdu + index);
            index += 0x01;
            pCancelLCE->cancelControl =  *(pReceivedAsdu + index);
            index += 0x01;
            memUtils_memCopy( (uint8_t*)&pCancelLCE->effectiveTime, pReceivedAsdu + index, 0x04 );
        }
		else if ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_CancelAllLoadControlEventCmds_c)
        {
            pActionHandler->event.eventId = g_DRLC_CancelAllLoadControlEvents_c ;
            pActionHandler->event.pEventData[0] = *( pReceivedAsdu +
                                                     m_DRLC_LCE_CMD_PAYLOAD_POSITION_c );
        }
        else {
            pActionHandler->action = Send_Response;
        }

#endif      /* ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */
    }
    else {
#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )

        if ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_ReportEventStatusCmd_c )
        {
            pActionHandler->event.eventId = g_DRLC_ReportEventStatusEvent_c ;
            memUtils_memCopy ( pActionHandler->event.pEventData,
            pReceivedAsdu + m_DRLC_LCE_CMD_PAYLOAD_POSITION_c, sizeof(ReportEventStatus_t) );
        }
        else if(  *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )== g_GetScheduledEventCmd_c)
        {
            GetScheduledEvents_t * pScheduledLCE = (GetScheduledEvents_t *)
                pActionHandler->event.pEventData;

            pActionHandler->action = No_Response;
            pActionHandler->event.eventId = g_DRLC_GetScheduledEvents_c ;
            memUtils_memCopy( (uint8_t*)&pScheduledLCE->startTime, pReceivedAsdu + 0x03, 0x04 );
            pScheduledLCE->numOfEvents = *( pReceivedAsdu + 0x07 );
        }
		else {
            pActionHandler->action = Send_Response;
        }

#endif      /* ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 ) */
    }
    return g_ZERO_DATA_LENGTH_c;
}


/**************************************************************************************/
#if ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 )
uint8_t ZCL_CreateReportEventStatusCommand ( uint8_t transeqnum,
                             ReportEventStatus_t* pReportEvent,
                             uint8_t* pRespLocation )
{
    uint8_t resp_index = 0x00;


    if (pReportEvent == g_NULL_c || pRespLocation == g_NULL_c )
    {

    	return g_ZCL_Failure_c;
    }


    *( pRespLocation + resp_index ) =
        g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    resp_index++;

    *( pRespLocation + resp_index ) = transeqnum;

    resp_index++;

     *( pRespLocation + resp_index ) = g_ReportEventStatusCmd_c;
    resp_index++;

    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pReportEvent->eventId, 0x04 );
    resp_index += 4;
    *(pRespLocation + resp_index ) = pReportEvent->eventStatus;
    resp_index ++;
    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pReportEvent->startTime, 0x04 );
    resp_index += 4;
    *(pRespLocation + resp_index ) = pReportEvent->criticalityLevel;
    resp_index ++;
    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pReportEvent->coolingTempSetPoint, 0x02 );
    resp_index += 2;
    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pReportEvent->heatingTempSetPoint, 0x02 );
    resp_index += 2;
    *(pRespLocation + resp_index ) = pReportEvent->avgLoadPercentage;
    resp_index ++;
    *(pRespLocation + resp_index ) = pReportEvent->dutyCycle;
    resp_index ++;
    *(pRespLocation + resp_index ) = pReportEvent->eventControl;
    resp_index ++;
    *(pRespLocation + resp_index ) = pReportEvent->signatureType;
    resp_index ++;
    memUtils_memCopy( pRespLocation + resp_index, pReportEvent->signature, g_SIGNATURE_SIZE_c );
    resp_index += g_SIGNATURE_SIZE_c;

    return  resp_index;
}
#endif      /* ( g_DRLC_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/**************************************************************************************/

#if ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t ZCL_DRLCcreateLCECommand ( uint8_t transeqnum, uint8_t* pRespLocation,
    LoadControlEvent_t * pLCETable )
{
    uint8_t resp_index = 0x00;

    *( pRespLocation + resp_index ) =
        g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c | g_ZCL_SERVER_TO_CLIENT_DIRECTION_c
            | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c ;

    resp_index++;

    *( pRespLocation + resp_index ) = transeqnum;

    resp_index++;

     *( pRespLocation + resp_index ) = g_LoadControlEventCmd_c;

    resp_index++;

    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pLCETable->eventId, 0x04 );
    resp_index += 4;
    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pLCETable->deviceClass, 0x02 );
    resp_index += 2;
    *(pRespLocation + resp_index) = pLCETable->utilityEnrollmentGroup;
    resp_index += 1;
    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pLCETable->startTime,0x04 );
    resp_index += 4;
    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pLCETable->duration, 0x02 );
    resp_index += 2;
    *(pRespLocation + resp_index) = pLCETable->criticalityLevel;
    resp_index += 1;
    *(pRespLocation + resp_index) = pLCETable->coolingTempOffset;
    resp_index += 1;
    *(pRespLocation + resp_index) = pLCETable->heatingTempOffset;
    resp_index += 1;
    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pLCETable->coolingTempSetPoint, 0x02 );
    resp_index += 2;
    memUtils_memCopy( pRespLocation + resp_index, (uint8_t*)&pLCETable->heatingTempSetPoint, 0x02 );
    resp_index += 2;
    *(pRespLocation + resp_index) = pLCETable->avgLoadPercentage;
    resp_index += 1;
    *(pRespLocation + resp_index) = pLCETable->dutyCycle;
    resp_index += 1;
    *(pRespLocation + resp_index) = pLCETable->eventControl;
    resp_index += 1;

    return resp_index;
}
#endif      /* ( g_DRLC_SERVER_CLUSTER_ENABLE_d == 1 ) */
/*******************************************************************************
* Private Functions
*******************************************************************************/

/* None */

/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/

/* None */

/*******************************************************************************
* End Of File
*******************************************************************************/
#endif                                /* g_DRLC_CLUSTER_ENABLE_d */
