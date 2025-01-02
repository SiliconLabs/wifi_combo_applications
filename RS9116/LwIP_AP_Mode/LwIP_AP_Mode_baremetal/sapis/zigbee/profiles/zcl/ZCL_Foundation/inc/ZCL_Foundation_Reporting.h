#ifndef _ZCL_FOUNDATION_REPORTING_H_
#define _ZCL_FOUNDATION_REPORTING_H_



/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_INVALID_CLUSTER_ID_c								0xffff
#define m_INVALID_ATTRIBUTE_ID_c							0xffff
#define m_INVALID_POINTER_ID_c								0x0000
#define m_INITIAL_STATE_c                                   0x00
#define m_WAIT_FOR_MIN_INTERVAL_c                           0x01
#define m_READY_TO_SEND_REPORT_c                            0x02
#define m_INDIRECT_ADDR_MODE_c                              0x00
/*-----------------------------------------------------------------------------
* Configure Reporting command
*----------------------------------------------------------------------------*/
enum
{	
	g_Report_After_Attribute_Change_c,
	g_Report_After_Timer_Expiry_c
};


enum
{
	g_Configure_Reporting_Device_c,
	g_Configure_Sink_Device_c
};
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
#if ( g_REPORTING_ENABLE_d == 1 )
typedef struct Configure_Reporting_Command_Request
{
	uint8_t command_type;
	uint8_t is_manufacturer_specific_cmd;
	uint8_t manufacturer_code[2];
	uint8_t disable_default_resp;
	uint8_t command_id;
	uint8_t no_of_attribute_records;	
}__attribute__((__packed__))ing_Command_Request;



typedef struct Reportable_Attribute_Status_Record
{
	uint8_t status;
	uint8_t direction;
	uint8_t reportable_attrib_id[2];
}__attribute__((__packed__))Reportable_Attribute_Status_Record;

typedef struct Reporting_Command_Event_Tag
{
   uint8_t event_id;
   uint8_t event_data_length;	
   uint16_t attribute_id;
   uint8_t attrib_data_type;
   uint8_t a_attribute_data[1];
}__attribute__((__packed__))Reporting_Command_Event_t;


void ZCL_Reset_Foundation_Reporting
(
   void
);
/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
enum Reporting_Event
{
    g_Start_Reporting_Timer_event_c = 0x40
};

uint8_t ZCL_HandleReadReportingConfigurationCommand
(
		uint8_t endPointId,
		uint8_t asduLength,
		uint8_t* pReceivedAsdu,
		uint8_t* pResponseAsduStartLocation,
        ZCL_ActionHandler_t *pActionHandler,
		const ZCL_Cluster_Info_t *pZCLClusterInfo,
        uint8_t * pAttrsDataBaseLocation
);

uint8_t ZCL_HandleConfigureReportingCommand
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
);

#endif

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif 						/* _ZCL_FOUNDATION_REPORTING_H_*/
