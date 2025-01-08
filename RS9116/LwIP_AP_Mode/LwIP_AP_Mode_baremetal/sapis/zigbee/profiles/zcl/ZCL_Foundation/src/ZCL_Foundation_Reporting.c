/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "common_includes.h"
#include "ZCL_Common.h"
#include "ZCL_Functionality.h"
#include "ZCL_Foundation.h"
#if ( g_REPORTING_ENABLE_d == 1 )
#include "ZCL_Foundation_Reporting.h"
#include "stack_util.h"
#include "memory_utility.h"
#include "ZCL_Interface.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_DIRECTION_FIELD_LENGTH_c                          0x01
#define m_TIME_INTERVAL_FIELD_LENGTH_c                      0x02
#define m_ATTRIBUTE_ID_LENGTH_c								0x02
#define m_MIN_REPORTING_INTERVAL_c                          0x0001


#define Is_Reportable_Attribute( access_permissions ) \
        ( access_permissions & g_Reportable_Attribute_c ) ? g_TRUE_c : g_FALSE_c							

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Get_Data_length ( uint8_t attribute_data_type );

/*---------------------------------------------------------------------------*/

uint8_t ZCL_HandleConfigureReportingCommand
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    ZCL_Cluster_Info_t const *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
)
{
	uint8_t direction;
	uint16_t attributeId;
    uint8_t attributeDataType = g_ZERO_c;
	uint16_t timeOut = g_TWO_BYTE_INVALID_VALUE_c;
	uint16_t minReportingInterval = g_ZERO_c;	
	uint16_t maxReportingInterval = g_TWO_BYTE_INVALID_VALUE_c;
	uint8_t status = g_ZCL_Success_c ;
	uint8_t responseDataLength = g_ZERO_c;
	uint8_t reportableChangeArray[7] = { g_ZERO_c, g_ZCL_INVALID_DATA_c,g_ZCL_INVALID_DATA_c,
                                         g_ZCL_INVALID_DATA_c,g_ZCL_INVALID_DATA_c,
                                         g_ZCL_INVALID_DATA_c,g_ZCL_INVALID_DATA_c };
	uint8_t  receivedCommandId;
    uint8_t dataLength;
    uint16_t attrIndex;
    uint16_t dataOffset;



    dataLength = CreateZCLHeaderForGenericResponseCommand (
                                               g_Configure_Reporting_Response_Command_c,
                                               pReceivedAsdu,
                                               pResponseAsduStartLocation,
                                               &receivedCommandId );
    responseDataLength = dataLength;

	while( dataLength < asduLength  )
	{
		memUtils_memCopy
		(
				(uint8_t*)&attributeId,
				pReceivedAsdu + dataLength + 1,
				m_ATTRIBUTE_ID_LENGTH_c
		);
        direction = *( pReceivedAsdu + dataLength );

        dataLength += m_DIRECTION_FIELD_LENGTH_c + m_ATTRIBUTE_ID_LENGTH_c;
	
		if( direction == g_Configure_Reporting_Device_c )
		{
            attributeDataType = *( pReceivedAsdu + dataLength );			
			dataLength ++;

            /* Check whether the valid attribute is requested or not*/
            status = ZCL_Get_AttributeInfo_Index ( pZCLClusterInfo->num_attrib_info_present,
                                attributeId,
                                &attrIndex,
                                pZCLClusterInfo->p_array_of_attrib_info );

			if( status == g_ZCL_Success_c )
			{				
				if( pZCLClusterInfo->p_array_of_attrib_info[attrIndex].data_type != attributeDataType )
				{
					status = g_ZCL_Invalid_Data_type_c;
				}				
				else
				{
                    /* Check whether the attribute requested to report is of
                    reportable attribute or not*/
					if ( Is_Reportable_Attribute( pZCLClusterInfo->
                                p_array_of_attrib_info[attrIndex].access_permission ) )
                    {						
						memUtils_memCopy ( (uint8_t*)&minReportingInterval,
                                  pReceivedAsdu + dataLength, m_TIME_INTERVAL_FIELD_LENGTH_c );	

						memUtils_memCopy ( (uint8_t*)&maxReportingInterval,
                                  pReceivedAsdu + dataLength + 0x02, m_TIME_INTERVAL_FIELD_LENGTH_c );

						if( ( minReportingInterval < m_MIN_REPORTING_INTERVAL_c ) ||
								( maxReportingInterval < minReportingInterval ) )
						{
							status = g_ZCL_Invalid_Value_c;
						}
					}
					else
					{   /* attribute is of type not reportable*/
						status = g_ZCL_UnReportable_Attribute_c;
					}
				}
			}			
			else
			{
				status = g_ZCL_Unsupported_Attribute_c;
			}			
			dataLength += m_TIME_INTERVAL_FIELD_LENGTH_c + m_TIME_INTERVAL_FIELD_LENGTH_c;
			
			if( Is_AttributeAnalogDataType( attributeDataType ) )
			{
				reportableChangeArray[0] = ZCL_Get_Data_length( attributeDataType );				
				memUtils_memCopy
				(
						&reportableChangeArray[1] ,
						pReceivedAsdu + dataLength,
						reportableChangeArray[0]
				);				
				dataLength += reportableChangeArray[0];
			}

		}
		else
		{
		  	 status = ZCL_Get_AttributeInfo_Index ( pZCLClusterInfo->num_attrib_info_present,
                                attributeId,
                                &attrIndex,
                                pZCLClusterInfo->p_array_of_attrib_info );
			
			  if ( (status == g_ZCL_Success_c ) &&( Is_Reportable_Attribute( pZCLClusterInfo->
                                p_array_of_attrib_info[attrIndex].access_permission ) ))
                    {	
				/*copy the timeout value, the timeout value is
				present after direction and attribute id. 0x03 is added for
				the space occupied by direction(1 byte) and attribute id( 2 byte )*/
				memUtils_memCopy ( (uint8_t*)&timeOut, pReceivedAsdu + dataLength,
									m_TIME_INTERVAL_FIELD_LENGTH_c );			
//				dataLength += m_TIME_INTERVAL_FIELD_LENGTH_c;
			 }
			 else{
			    status = g_ZCL_Unsupported_Attribute_c;
			 }
			 dataLength += m_TIME_INTERVAL_FIELD_LENGTH_c;
		}

		/*if the status is success, add/update the entry in the report
		 configuration table, else update the status in Response pointer*/
		if( status == g_ZCL_Success_c )
		{			
            dataOffset = ZCL_Get_AttributeDataOffset(  pZCLClusterInfo->num_attrib_info_present,
                                                       attributeId,
                                                       pZCLClusterInfo->p_array_of_attrib_info );

            /* Adds the attribute with details required in the global
            Reporting table*/
			status = ZCL_Callback_Add_Reporting_Table_Entry
			         (
                        direction,
                        attributeId,
                        attributeDataType,
                        minReportingInterval,
                        maxReportingInterval,
                        timeOut,
                        reportableChangeArray,
                        pZCLClusterInfo->cluster_id,
                        pAttrsDataBaseLocation + dataOffset
			         );

		}
        /* Could not make an entry in the Reporting table*/
		if ( status != g_ZCL_Success_c )
        {
			pResponseAsduStartLocation[responseDataLength++] = status;			
			pResponseAsduStartLocation[responseDataLength++] = direction;			
			memUtils_memCopy
			(
                &pResponseAsduStartLocation[responseDataLength],
                (uint8_t*)&attributeId ,
                m_ATTRIBUTE_ID_LENGTH_c
			);			
			responseDataLength += g_ATTRIBUTE_ID_LENGTH_c;
		}
        else{
            pActionHandler->event.eventId = m_Start_Reporting_Timer_c ;
        }
        status = g_ZCL_Success_c ;
	}
	if( ( GetZCLHeaderLength( pReceivedAsdu ) ) == responseDataLength )
	{
		pResponseAsduStartLocation[responseDataLength++] = status;
	};
	return responseDataLength;

}

/*---------------------------------------------------------------------------*/
uint8_t ZCL_HandleReadReportingConfigurationCommand
(
    uint8_t endPointId,
    uint8_t asduLength,
    uint8_t* pReceivedAsdu,
    uint8_t* pResponseAsduStartLocation,
    ZCL_ActionHandler_t *pActionHandler,
    const ZCL_Cluster_Info_t *pZCLClusterInfo,
    uint8_t * pAttrsDataBaseLocation
)
{
	uint8_t direction;
	uint16_t attributeId;
    uint8_t receivedCommandId;
    uint8_t dataLength;
	uint8_t responseDataLength;
    /* This functionality is used to read the reportable entry from the table*/

    dataLength = CreateZCLHeaderForGenericResponseCommand (
                                               g_Read_Reporting_Configuration_Response_Command_c,
                                               pReceivedAsdu,
                                               pResponseAsduStartLocation,
                                               &receivedCommandId );
    responseDataLength = dataLength;

	while( dataLength < asduLength )
	{		
		direction = pReceivedAsdu[dataLength];
		dataLength++;		
		memUtils_memCopy ( (uint8_t*)&attributeId, &pReceivedAsdu[dataLength], g_ATTRIBUTE_ID_LENGTH_c );		
		
        dataLength += g_ATTRIBUTE_ID_LENGTH_c;
		
		ZCL_Callback_ReadReportRecord (  direction,
                                         pZCLClusterInfo->cluster_id,
                                         attributeId,
                                         pResponseAsduStartLocation,
				                         &responseDataLength,
                                         pZCLClusterInfo );
	}
	return responseDataLength;
}


#endif /* ( g_REPORTING_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
