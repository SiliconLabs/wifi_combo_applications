/*****************************************************************************
* Includes
******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "stack_common.h"
#include "ZCL_Common.h"
//#include "sw_timer.h"
#include "memory_utility.h"
#include "ZCL_Foundation.h"
#if ( g_REPORTING_ENABLE_d == 1 )
#include "ZCL_Foundation_Reporting.h"
#include "buffer_management.h"
#include "ApptimerId.h"
#include "AppReportingUtilities.h"
#include "ApplicationThinLayer.h"
#include "AppTimerUtilities.h"

/*****************************************************************************
* Global Constants
*****************************************************************************/
#define m_Dst_Addr_Mode_Index_c                             0x00
#define m_Dst_Addr_Index_c                                  0x01
#define m_Dst_EP_Index_c                                    0x09
#define m_Src_EP_Index_c                                    0x0A
#define m_ClusterId_Index_c                                 0x0B
#define m_AttriBute_Id_Index_c                              0x0D
#define m_Attr_Data_Type_Index_c                            0x0F
#define m_Attr_Data_Length_Index_c                          0x10
#define m_Attr_Data_Index_c                                 0x11
#define m_TIME_INTERVAL_FIELD_LENGTH_c                      0x02
#define m_ONE_SEC_c                                         0x01
/*****************************************************************************
* Public Memory declarations
*****************************************************************************/

#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

/*****************************************************************************
* Private Memory declarations
******************************************************************************/

#if ( g_REPORTING_ENABLE_d == 1 )
static Reporter_Device_Configuration_t
ga_Reporter_Device_Config_Table[ g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c ];
static Sink_Device_Configuration_t
ga_Sink_Device_Config_Table[ g_MAX_SINK_CONFIG_TABLE_ENTRIES_c ];

static uint32_t lastReportTimeTick = 0x00;

#endif          /* ( g_REPORTING_ENABLE_d == 1 ) */

/****************************************************************************
* Function Prototypes
****************************************************************************/

static void App_sendAttributeReports( void );
static uint8_t App_isReportsPending( void );

static uint8_t Is_Reportable_Change (  uint8_t reportable_attrib_data_len,
                                       uint8_t* p_attrib_data_to_be_written,
                                       uint8_t* p_local_attrib_data,
                                       uint8_t index );

/******************************************************************************
* Public Functions
 ******************************************************************************/

void App_reportingTask( void )
{

    if( App_getDifferance( App_getcurrentTick(), lastReportTimeTick ) >=
       g_ONE_TICK_c ) {
        /*For each tick(1sec) find the reportable attributes and send the
           status of those attributes*/
        ZCL_Callback_Check_Reportable_Attributes( g_Report_After_Timer_Expiry_c,
                                    m_INVALID_CLUSTER_ID_c, g_NULL_c,g_NULL_c );
        lastReportTimeTick = App_getcurrentTick();
    }
    if ( App_isReportsPending() == g_TRUE_c ) {
        App_sendAttributeReports();
    }
}

/*****************************************************************************/

void App_ReportingTableInit ( void )
{
   /* Initializing the Report Device Table with default value . */
   lastReportTimeTick = App_getcurrentTick();


   memUtils_memSet( (uint8_t*)ga_Reporter_Device_Config_Table,
             g_ZCL_INVALID_DATA_c,
             sizeof( Reporter_Device_Configuration_t )* g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c );

   memUtils_memSet ( (uint8_t*)ga_Sink_Device_Config_Table,
             g_ZCL_INVALID_DATA_c,
             sizeof( Sink_Device_Configuration_t ) * g_MAX_SINK_CONFIG_TABLE_ENTRIES_c );
}

/*****************************************************************************************/

void App_setReportingInformation ( uint8_t index, uint8_t addrMode, uint8_t dstEndPoint,
                                   uint8_t* dstAddr )
{
    /* Set the reporting information if there is any free entry in
    the Report Configuration Table .*/
    if ( index < g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c ) {
        ga_Reporter_Device_Config_Table[index].dstAddrMode = addrMode;
        ga_Reporter_Device_Config_Table[index].dstEndpoint = dstEndPoint;
        memUtils_memCopy( ga_Reporter_Device_Config_Table[index].dstAddress, dstAddr, 0x08 );
    }
    return;
}

/*****************************************************************************************/

uint8_t ZCL_Callback_Add_Reporting_Table_Entry ( uint8_t direction, uint16_t attributeId,
                                                 uint8_t attrDataType, uint16_t minReportingInterval,
                                                 uint16_t maxReportingInterval,
                                                 uint16_t timeOut, uint8_t* ReportableChange,
                                                 uint16_t clusterId, uint8_t * pattrDataLocation )
{
	uint8_t index ;
    uint8_t index_to_be_used = g_INVALID_INDEX_c;
    uint8_t status = g_ZCL_Insufficient_Space_c;

    if ( direction == g_Configure_Sink_Device_c ) {
        for ( index = 0; index < g_MAX_SINK_CONFIG_TABLE_ENTRIES_c; index++ ) {
            /* */
            if ( ( ( ga_Sink_Device_Config_Table[ index ].cluster_id == clusterId ) &&
                 ( ga_Sink_Device_Config_Table[ index ].attribute_id == attributeId ) ) ) {
                  index_to_be_used = index;
                  break;
            }
            if ( ( ga_Sink_Device_Config_Table[ index ].cluster_id == g_TWO_BYTE_INVALID_VALUE_c ) &&
                 ( index_to_be_used == g_INVALID_INDEX_c ) )  {

                    index_to_be_used = index;
            }
        }
        if ( index_to_be_used < g_MAX_SINK_CONFIG_TABLE_ENTRIES_c ) {
           ga_Sink_Device_Config_Table[ index_to_be_used ].cluster_id = clusterId;
           ga_Sink_Device_Config_Table[ index_to_be_used ].attribute_id = attributeId;
           ga_Sink_Device_Config_Table[ index_to_be_used ].recieve_report_timeout_period = timeOut;
           ga_Sink_Device_Config_Table[ index_to_be_used ].timeout_period = timeOut;
           status = g_ZCL_Success_c;
        }
    }
    else {
        for( index = g_ZERO_c ; index < g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c; index++ ) {
            if( ( ga_Reporter_Device_Config_Table[index].srcEndpoint == g_ZCL_INVALID_DATA_c ) &&
                ( index_to_be_used == g_ZCL_INVALID_DATA_c ) ) {
                    index_to_be_used = index;
            }
            else {
                /* check if the cluster id and attribute id is already existing.if so,
                update the same entry with the details received in the command */
                if( ( ga_Reporter_Device_Config_Table[ index ].clusterId == clusterId ) &&
                    ( ga_Reporter_Device_Config_Table[ index ].attributeId == attributeId ) ) {
                    index_to_be_used = index;

                    if( g_TWO_BYTE_INVALID_VALUE_c == maxReportingInterval ) {
                        /* Reset the entry is invalid, means stop reporting, reset
                        only the fields except destination fields,
                        10 bytes is considering the destination address mode, destination endpoint
                        and destination address */
                        memUtils_memSet (
                            &ga_Reporter_Device_Config_Table[index_to_be_used].srcEndpoint,
                            g_INVALID_VALUE_c,
                            sizeof( ga_Reporter_Device_Config_Table ) - 0x0a );
                        return g_ZCL_Success_c;
                    }
                    break;
                }
            }
        }
        if( index_to_be_used < g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c ) {
            ga_Reporter_Device_Config_Table[index_to_be_used].dstAddrMode = m_INDIRECT_ADDR_MODE_c;
            ga_Reporter_Device_Config_Table[index_to_be_used].srcEndpoint = ZCL_Callback_Get_Device_EndPoint();
            ga_Reporter_Device_Config_Table[index_to_be_used].clusterId = clusterId;
            ga_Reporter_Device_Config_Table[index_to_be_used].attributeId = attributeId;
            ga_Reporter_Device_Config_Table[index_to_be_used].min_interval = minReportingInterval;
            ga_Reporter_Device_Config_Table[index_to_be_used].max_interval = maxReportingInterval;
            ga_Reporter_Device_Config_Table[index_to_be_used].time_out_period = maxReportingInterval;
            ga_Reporter_Device_Config_Table[index_to_be_used].attrDataType = attrDataType;
            ga_Reporter_Device_Config_Table[index_to_be_used].pAttrDataLocation = pattrDataLocation;
            ga_Reporter_Device_Config_Table[index_to_be_used].reportState = m_INITIAL_STATE_c;

            memUtils_memCopy ( ga_Reporter_Device_Config_Table[index_to_be_used].reportable_change,
                ReportableChange + 1, ReportableChange[0] );
            status = g_ZCL_Success_c;
         }
    }
    return status;
}

/*****************************************************************************************/

void ZCL_Callback_ReadReportRecord( uint8_t direction, uint16_t clusterId,
                                    uint16_t attributeId, uint8_t *pResponseAsdu,
                                    uint8_t* pResponseDataLength,
                                    ZCL_Cluster_Info_t const *pZCLClusterInfo )
{

	uint8_t status = g_ZCL_Success_c;
    uint8_t index = g_ZERO_c;
    uint8_t responseLength = *pResponseDataLength;
    uint8_t attributeDataLength;

    if ( direction == g_Configure_Reporting_Device_c ){
        for( index = g_ZERO_c; index < pZCLClusterInfo->num_attrib_info_present; index++ ) {
            if( g_MEMORY_MATCH_c ==  ( memUtils_memCompare( (uint8_t*)&pZCLClusterInfo->
                                         p_array_of_attrib_info[index].a_attribute_id,
                                         (uint8_t*)&attributeId, g_ATTRIBUTE_ID_LENGTH_c ) ) ) {
                if( ( g_Reportable_Attribute_c != ( pZCLClusterInfo->
                      p_array_of_attrib_info[index].access_permission & g_Reportable_Attribute_c ) ) ){

                      status = g_ZCL_UnReportable_Attribute_c;
				}
                break;
           }
        }
    }
    if( status == g_ZCL_Success_c ) {
        if ( direction == g_Configure_Reporting_Device_c ) {
            for( index = g_ZERO_c ; index < g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c; index++ ) {
                if( ( clusterId == ga_Reporter_Device_Config_Table[index].clusterId ) &&
                    ( attributeId == ga_Reporter_Device_Config_Table[index].attributeId  ) ) {
                    break;
                }
            }
        }
        else {
            for( index = g_ZERO_c ; index < g_MAX_SINK_CONFIG_TABLE_ENTRIES_c; index++ ) {
                if( ( clusterId == ga_Sink_Device_Config_Table[index].cluster_id ) &&
                    ( attributeId == ga_Sink_Device_Config_Table[index].attribute_id  ) ) {
                    break;
                }
            }
        }
        if( ( ( direction == g_Configure_Reporting_Device_c )    &&
              ( index == g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c ) ) ||
              ( index == g_MAX_SINK_CONFIG_TABLE_ENTRIES_c ) ) {

                status = g_ZCL_Unsupported_Attribute_c;
        }
    }
    pResponseAsdu[responseLength] = status;
    responseLength++;
    pResponseAsdu[responseLength] = direction;
    responseLength++;
    memUtils_memCopy ( &pResponseAsdu[responseLength], (uint8_t*)&attributeId, g_ATTRIBUTE_ID_LENGTH_c );
    responseLength += g_ATTRIBUTE_ID_LENGTH_c;

    if( status == g_ZCL_Success_c ){
        if( direction == g_Configure_Reporting_Device_c ) {
             attributeDataLength = ZCL_Get_Data_length (
                                    ga_Reporter_Device_Config_Table[index].attrDataType);

            pResponseAsdu[responseLength++] = ga_Reporter_Device_Config_Table[index].attrDataType;

            memUtils_memCopy ( &pResponseAsdu[responseLength],
                (uint8_t*)&(ga_Reporter_Device_Config_Table[index].min_interval),
                m_TIME_INTERVAL_FIELD_LENGTH_c );

            responseLength += m_TIME_INTERVAL_FIELD_LENGTH_c;

            memUtils_memCopy ( &pResponseAsdu[responseLength],
                        (uint8_t*)&(ga_Reporter_Device_Config_Table[index].max_interval),
                        m_TIME_INTERVAL_FIELD_LENGTH_c );
            responseLength += m_TIME_INTERVAL_FIELD_LENGTH_c;
            /*check  if the attribute is of analog, if so copy the reportable change*/
            if( Is_AttributeAnalogDataType( ga_Reporter_Device_Config_Table[index].attrDataType )){
                memUtils_memCopy ( &pResponseAsdu[responseLength],
                    ga_Reporter_Device_Config_Table[index].reportable_change,
                    attributeDataLength );
                /*increment the length for reportable change. The length of reportable
                change is as per the attribute data*/
                responseLength += attributeDataLength;
            }
        }
        else {
            memUtils_memCopy (  &pResponseAsdu[responseLength],
                        (uint8_t*)&ga_Sink_Device_Config_Table[index].timeout_period,
                        m_TIME_INTERVAL_FIELD_LENGTH_c );
            responseLength += m_TIME_INTERVAL_FIELD_LENGTH_c;
        }
    }
    *pResponseDataLength = responseLength;
}

/*****************************************************************************************/

uint8_t ZCL_Callback_Check_Reportable_Attributes (  uint8_t report_after_timer_expiry,
                                                    uint16_t cluster_id,
                                                    uint8_t *p_attribute_id,
                                                    uint8_t *p_data  )
{
	uint8_t index;
	uint8_t index_to_access = g_INVALID_INDEX_c;
    uint8_t status = g_ZCL_Failure_c;

	for( index = g_ZERO_c; index < g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c; index++ ) {
	    if ( ga_Reporter_Device_Config_Table[index].srcEndpoint != g_INVALID_VALUE_c ) {
            if ( report_after_timer_expiry == g_Report_After_Timer_Expiry_c ) {
                /* If the max interval timer is 0xFFFF then the attribute will not be
                reported */
                if( ga_Reporter_Device_Config_Table[ index ].max_interval != g_TWO_BYTE_INVALID_VALUE_c )
                {
                    /* Decrement the timeout value. If the value reduces to 0,
                    then upload this in the output list for the attribute to be reported.
                    Reload the time out field with the max interval time */

                    /* This function is called as a result of the expiry of the timer expiry */
                    if ( !( --( ga_Reporter_Device_Config_Table[ index ].time_out_period ) ) ) {
                        index_to_access = index;
                    }
                    else if ( ( ga_Reporter_Device_Config_Table[ index ].reportState == m_WAIT_FOR_MIN_INTERVAL_c ) &&
                              ( ( ga_Reporter_Device_Config_Table[ index ].max_interval -
                                 ga_Reporter_Device_Config_Table[ index ].time_out_period ) >=
                                 ga_Reporter_Device_Config_Table[ index ].min_interval ) ) {
                                index_to_access = index;
                    }
                }
            }
            else {
                /* Reportable attributes are either of two bytes length or one byte length.
                If the data length indicates two bytes which implies the attribute is a two
                byte quantity. */
                if (  ( ga_Reporter_Device_Config_Table[ index ].clusterId == cluster_id ) &&
                      ( g_MEMORY_MATCH_c == ( memUtils_memCompare ( (uint8_t*)&ga_Reporter_Device_Config_Table[ index ].attributeId,
                        p_attribute_id, g_ATTRIBUTE_ID_LENGTH_c ) ) ) ) {
                       if ( Is_AttributeAnalogDataType( ga_Reporter_Device_Config_Table[ index ].attrDataType ) )
                       {
                           if (  Is_Reportable_Change (
                                    ZCL_Get_Data_length (
                                       ga_Reporter_Device_Config_Table[ index ].attrDataType
                                    ),
                                    p_data,
                                    ga_Reporter_Device_Config_Table[index].pAttrDataLocation,
                                    index  ) )
                             {
                                index_to_access = index;
                             }
                       }
					   else
                       {
							   index_to_access = index;
							   ga_Reporter_Device_Config_Table[ index ].reportState = m_WAIT_FOR_MIN_INTERVAL_c;
					   }
                }
            }
            /* If the index to be used is a valid index and this entry has been used
            to ready to send report */
            if( ( index_to_access != g_INVALID_INDEX_c ) ) {
                ga_Reporter_Device_Config_Table[ index_to_access ].time_out_period =
                ga_Reporter_Device_Config_Table[ index_to_access ].max_interval;
                ga_Reporter_Device_Config_Table[ index_to_access ].reportState = m_READY_TO_SEND_REPORT_c;
                status =  g_ZCL_Success_c;
            }
            index_to_access = g_INVALID_INDEX_c;
        }
    }
    return status;
}

/*****************************************************************************************/

void App_Update_Received_Report (  uint8_t report_after_timer_expiry,
                                   uint16_t cluster_id,
                                   uint16_t attribute_id )
{
    uint8_t index;

    for( index = g_ZERO_c; index < g_MAX_SINK_CONFIG_TABLE_ENTRIES_c; index++ ) {
        if ( ga_Sink_Device_Config_Table[ index ].cluster_id != g_TWO_BYTE_INVALID_VALUE_c ) {
             if ( report_after_timer_expiry == g_Report_After_Timer_Expiry_c ) {
                if ( !(-- ( ga_Sink_Device_Config_Table[ index ].recieve_report_timeout_period ) ) ) {
                    memUtils_memSet( (uint8_t*)&ga_Sink_Device_Config_Table[ index ], g_ZCL_INVALID_DATA_c,
                             sizeof(Sink_Device_Configuration_t) );
                }
             }
             else {
                 if ( ( ga_Sink_Device_Config_Table[ index ].cluster_id == cluster_id ) &&
                      ( ga_Sink_Device_Config_Table[ index ].cluster_id == attribute_id ) ) {
                        ga_Sink_Device_Config_Table[ index ].recieve_report_timeout_period =
                        ga_Sink_Device_Config_Table[ index ].timeout_period ;
                 }
             }
        }
    }
}
/*****************************************************************************************/
Reporter_Device_Configuration_t *App_GetReporterDeviceConfiguration(uint8_t index)
{
    if ( index > g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c ){
        return g_NULL_c;
    }
	else {
	    return &ga_Reporter_Device_Config_Table[index];
	}

}
/*****************************************************************************************/

Sink_Device_Configuration_t *App_GetSinkDeviceConfiguration(uint8_t index)
{
  	if ( index > g_MAX_SINK_CONFIG_TABLE_ENTRIES_c ){
        return g_NULL_c;
    }
	else {
	    return &ga_Sink_Device_Config_Table[index];
	}

}

/*****************************************************************************************/

static void App_sendAttributeReports( void )
{
    uint8_t index;
    uint8_t attributeDataLength = g_ZERO_c;
    uint8_t buffindex;
    APSDE_Request_t* pDataReq;
    APSDE_Data_Request_t * pAppDataReq;

    for ( index = g_ZERO_c; index < g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c; index++ ) {

        if ( ( ga_Reporter_Device_Config_Table[index].srcEndpoint != 0xFF ) &&
             ( ga_Reporter_Device_Config_Table[index].reportState == m_READY_TO_SEND_REPORT_c ) ) {

                buffindex =  buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

                if( buffindex != 0xFF ) {

#if (DEBUG_TEST == 1)
				     /* Buffer ID --> 170 */
                    allocatedbuffer[buffindex]= 170;
                    freebuffer[buffindex][0]= 170;
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
                             (uint8_t*)&ga_Reporter_Device_Config_Table[index].attributeId, 0x02 );
                    pAppDataReq->asdu[5] = ga_Reporter_Device_Config_Table[index].attrDataType;
                    pAppDataReq->asdulength += 0x06;
                    attributeDataLength = ZCL_Get_Data_length (
                                            ga_Reporter_Device_Config_Table[index].attrDataType );
                    memUtils_memCopy( &pAppDataReq->asdu[6],
                             ga_Reporter_Device_Config_Table[index].pAttrDataLocation,
                             attributeDataLength );
                    pAppDataReq->dest_addr_mode = ga_Reporter_Device_Config_Table[index].dstAddrMode;
                    memUtils_memCopy( pAppDataReq->dest_address.IEEE_address,
                              ga_Reporter_Device_Config_Table[index].dstAddress, 0x08 );
                    pAppDataReq->dest_endpoint = ga_Reporter_Device_Config_Table[index].dstEndpoint;
                    pAppDataReq->src_endpoint = ga_Reporter_Device_Config_Table[index].srcEndpoint;
                    pAppDataReq->cluster_id = ga_Reporter_Device_Config_Table[index].clusterId;
                    pAppDataReq->profile_id = g_APP_PROFILE_ID_c;
                    pAppDataReq->txoptions = App_getClusterSecurity( pAppDataReq->cluster_id );
                    pAppDataReq->radius = g_DEFAULT_RADIUS_c;
                    pAppDataReq->asdulength += attributeDataLength;
                    ga_Reporter_Device_Config_Table[index].reportState = m_INITIAL_STATE_c;

                    //APS_APSDErequest( buffindex );
                    APS_APSDE_request(buffindex,pAppDataReq );

                }
          }
    }
}

/*****************************************************************************************/

static  uint8_t App_isReportsPending( void )
{
    uint8_t index;
    /* Check if there is any pending report entry in the report
    configuration table*/
    for ( index = g_ZERO_c; index < g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c; index++ ) {
        if ( ( ga_Reporter_Device_Config_Table[index].srcEndpoint != 0xFF ) &&
             ( ga_Reporter_Device_Config_Table[index].reportState == m_READY_TO_SEND_REPORT_c ) ) {
                return g_TRUE_c;
             }
    }
    return g_FALSE_c;
}

/*****************************************************************************************/

static uint8_t Is_Reportable_Change ( uint8_t attrib_data_len,
                                      uint8_t* p_writting_attrib_data,
                                      uint8_t* p_local_attrib_data, uint8_t index )
{
    uint64_t attribute_data_to_be_written = g_ZERO_c;
    uint64_t local_attribute_data = g_ZERO_c;
    uint64_t delta = g_ZERO_c;
    uint64_t reportablechange = g_ZERO_c;

    memUtils_memCopy( (uint8_t*)&attribute_data_to_be_written, p_writting_attrib_data, attrib_data_len );
    memUtils_memCopy( (uint8_t*)&local_attribute_data, p_local_attrib_data, attrib_data_len );
    memUtils_memCopy( (uint8_t*)&reportablechange,
              ga_Reporter_Device_Config_Table[ index ].reportable_change, attrib_data_len );

    if ( attribute_data_to_be_written > local_attribute_data ){
        delta = attribute_data_to_be_written - local_attribute_data;
    }
    else{

        delta = local_attribute_data - attribute_data_to_be_written;
    }
   /* If the change is more than or equal to the reportable change value that
    has been configured for this particular attribute ID, AND if the time
    elapsed since last report of this attribute is more than the minimum
   reporting time interval, then send the report of this attribute value
   change */
    if ( delta >= reportablechange ){
        if ( ( ( ga_Reporter_Device_Config_Table[ index ].max_interval -
                ga_Reporter_Device_Config_Table[ index ].time_out_period ) >=
              ga_Reporter_Device_Config_Table[ index ].min_interval ) ) {
                  /* Reload the time out as the reporting should start with the max
                    interval value from this instant */
                  ga_Reporter_Device_Config_Table[ index ].time_out_period =
                      ga_Reporter_Device_Config_Table[ index ].max_interval;

                  return g_TRUE_c;
        }
        else{

            ga_Reporter_Device_Config_Table[ index ].reportState = m_WAIT_FOR_MIN_INTERVAL_c;
        }
   }
   return g_FALSE_c;

}

#endif   /* ( g_REPORTING_ENABLE_d == 1 ) */


/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
