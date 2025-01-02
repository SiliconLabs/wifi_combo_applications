#ifndef _APP_HANDLE_REPORTING_H_
#define _APP_HANDLE_REPORTING_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#if ( g_REPORTING_ENABLE_d == 1 )

#define g_REPORTING_TIME_OUT_DURATION_c                     0x03E8
#define g_MAX_REPORT_CONFIG_TABLE_ENTRIES_c		            0x03
#define g_MAX_SINK_CONFIG_TABLE_ENTRIES_c                   0x03


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct Reporter_Device_Configuration_Tag
{
    uint8_t dstAddrMode;
    uint8_t dstAddress[8];
    uint8_t dstEndpoint;
	uint8_t srcEndpoint;
    uint16_t clusterId;
    uint16_t attributeId;
	uint16_t min_interval;
	uint16_t max_interval;
    uint8_t attrDataType;
    uint8_t reportable_change[6];
	uint16_t time_out_period;
    uint8_t* pAttrDataLocation;
    uint8_t reportState;
}__attribute__((__packed__))Reporter_Device_Configuration_t;

typedef struct Sink_Device_Configuration_Tag
{
   uint16_t cluster_id;
   uint16_t attribute_id;
   uint16_t recieve_report_timeout_period;
   uint16_t timeout_period;
}__attribute__((__packed__))Sink_Device_Configuration_t;

/*-----------------------------------------------------------------------------
* Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

void App_Update_Received_Report ( uint8_t report_after_timer_expiry,	
	                              uint16_t cluster_id,
	                              uint16_t attribute_id );



void App_setReportingInformation ( uint8_t index, uint8_t addrMode, uint8_t dstEndPoint,
                                uint8_t* dstAddr );

void App_ReportingTableInit ( void );

void App_reportingTask( void );

Reporter_Device_Configuration_t *App_GetReporterDeviceConfiguration(uint8_t index);
Sink_Device_Configuration_t *App_GetSinkDeviceConfiguration(uint8_t index);



#endif                                  /* ( g_REPORTING_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                  /* _APP_HANDLE_REPORTING_H_ */
