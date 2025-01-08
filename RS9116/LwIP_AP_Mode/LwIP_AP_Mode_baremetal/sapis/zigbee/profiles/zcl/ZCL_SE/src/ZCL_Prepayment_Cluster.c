/*******************************************************************************
* ZCL_Prepayment_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting prepayment cluster
*
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Prepayment_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/



/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d == 1 )

const ZCL_Cluster_Info_t ga_Prepayment_Client_Cluster_Info =
{
    g_PREPAYMENT_CLUSTER_c,
    g_NULL_c,
    g_NULL_c
};
#endif          /*  ( g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )

const ZCL_Attribute_Info_t ma_Prepayment_Server_Cluster_Attribute_Info [] =
{
/*Prepayment Information Attribute Set */
#if (g_PAYMENT_CONTROL_ATTRIBUTE_ENABLE_d == 1)
    /*Prepayment Information Set*/
    { g_PAYMENT_CONTROL_ATTRIBUTE_c,
      g_Eight_Bit_Bitmap_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(g_CREDIT_REMAINING_ATTRIBUTE_ENABLE_d == 1)
    { g_CREDIT_REMAINING_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(g_EMERGENCY_CREDIT_REMAINING_ATTRIBUTE_ENABLE_d == 1)
    { g_EMERGENCY_CREDIT_REMAINING_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif

#if(g_CREDIT_STATUS_ATTRIBUTE_ENABLE_d == 1)
    { g_CREDIT_STATUS_ATTRIBUTE_c,
      g_Eight_Bit_Bitmap_Data_c,
      g_Read_Only_Attribute_c },
#endif

/*  SE 1.2 Updates   */
#if(g_SE_PROFILE_1_2_d == 1)	

#if(g_CREDIT_REMAINING_TIMESTAMP_ATTRIBUTE_ENABLE_d == 1)
    { g_CREDIT_REMAINING_TIMESTAMP_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_CREDIT_REMAINING_TIMESTAMP_ATTRIBUTE_ENABLE_d*/	

#if(g_ACCUMULATED_DEBIT_ATTRIBUTE_ENABLE_d == 1)
    { g_ACCUMULATED_DEBIT_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_ACCUMULATED_DEBIT_ATTRIBUTE_ENABLE_d*/	

#if(g_OVERALL_DEBIT_CAP_ATTRIBUTE_ENABLE_d == 1)
    { g_OVERALL_DEBIT_CAP_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_OVERALL_DEBIT_CAP_ATTRIBUTE_ENABLE_d*/	

#if(g_EMERGENCY_CREDIT_LIMIT_ALLOWANCE_ATTRIBUTE_ENABLE_d == 1)
    { g_EMERGENCY_CREDIT_LIMIT_ALLOWANCE_LIMIT_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_EMERGENCY_CREDIT_LIMIT_ALLOWANCE_ATTRIBUTE_ENABLE_d*/	

#if(g_EMERGENCY_CREDIT_THRESHOLD_ATTRIBUTE_ENABLE_d == 1)
    { g_EMERGENCY_CREDIT_THRESHOLD_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_EMERGENCY_CREDIT_THRESHOLD_ATTRIBUTE_ENABLE_d*/	

#if(g_TOTAL_CREDIT_ADDED_ATTRIBUTE_ENABLE_d == 1)
    { g_TOTAL_CREDIT_ADDED_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_TOTAL_CREDIT_ADDED_ATTRIBUTE_ENABLE_d*/	

#if(g_MAX_CREDIT_LIMIT_ATTRIBUTE_ENABLE_d == 1)
    { g_MAX_CREDIT_LIMIT_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_MAX_CREDIT_LIMIT_ATTRIBUTE_ENABLE_d*/	

#if(g_FREINDLY_CREDIT_WARNING_ATTRIBUTE_ENABLE_d == 1)
    { g_FREINDLY_CREDIT_WARNING_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_ACCUMULATED_DEBIT_ATTRIBUTE_ENABLE_d*/	

#if(g_LOW_CREDIT_WARNING_ATTRIBUTE_ENABLE_d == 1)
    { g_LOW_CREDIT_WARNING_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_ACCUMULATED_DEBIT_ATTRIBUTE_ENABLE_d*/	

#if(g_IHD_LOW_CREDIT_WARNING_ATTRIBUTE_ENABLE_d == 1)
    { g_IHD_LOW_CREDIT_WARNING_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_IHD_LOW_CREDIT_WARNING_ATTRIBUTE_ENABLE_d*/	

#if(g_INTERRUPT_SUSPEND_TIME_ATTRIBUTE_ENABLE_d == 1)
    { g_INTERRUPT_SUSPEND_TIME_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_INTERRUPT_SUSPEND_TIME_ATTRIBUTE_ENABLE_d*/	

#if(g_CUT_OFF_VALUE_ATTRIBUTE_ENABLE_d == 1)
    { g_CUT_OFF_VALUE_ATTRIBUTE_c,
      g_Signed_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_CUT_OFF_VALUE_ATTRIBUTE_ENABLE_d*/	

#if(g_PAYMENT_CARD_ID_ATTRIBUTE_ENABLE_d == 1)
    { g_PAYMENT_CARD_ID_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c },
#endif          /* g_CUT_OFF_VALUE_ATTRIBUTE_ENABLE_d*/	
	
	
#endif          /* g_SE_PROFILE_1_2_d*/



#if(g_TOP_UP_DATE_TIME_1_ATTRIBUTE_ENABLE_d == 1)

    /*Top-up  Attribute Set*/
    { g_TOP_UP_DATE_TIME_1_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_TOP_UP_AMOUNT_1_ATTRIBUTE_ENABLE_d == 1)
    { g_TOP_UP_AMOUNT_1_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif

#if(g_ORIGINATING_DEVICE_1_ATTRIBUTE_ENABLE_d == 1)
    { g_ORIGINATING_DEVICE_1_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(g_TOP_UP_DATE_TIME_2_ATTRIBUTE_ENABLE_d == 1)
    { g_TOP_UP_DATE_TIME_2_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_TOP_UP_AMOUNT_2_ATTRIBUTE_ENABLE_d ==1 )
    { g_TOP_UP_AMOUNT_2_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(g_ORIGINATING_DEVICE_2_ATTRIBUTE_ENABLE_d ==1)
    { g_ORIGINATING_DEVICE_2_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif

#if( g_TOP_UP_DATE_TIME_3_ATTRIBUTE_ENABLE_d ==1)

    { g_TOP_UP_DATE_TIME_3_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif

#if(g_TOP_UP_AMOUNT_3_ATTRIBUTE_ENABLE_d ==1)
    { g_TOP_UP_AMOUNT_3_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_ORIGINATING_DEVICE_3_ATTRIBUTE_ENABLE_d ==1)
    { g_ORIGINATING_DEVICE_3_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif

#if(g_TOP_UP_DATE_TIME_4_ATTRIBUTE_ENABLE_d == 1)
    { g_TOP_UP_DATE_TIME_4_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_TOP_UP_AMOUNT_4_ATTRIBUTE_ENABLE_d == 1)
    { g_TOP_UP_AMOUNT_4_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(g_ORIGINATING_DEVICE_4_ATTRIBUTE_ENABLE_d == 1)
    { g_ORIGINATING_DEVICE_4_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(g_TOP_UP_DATE_TIME_5_ATTRIBUTE_ENABLE_d == 1)

    { g_TOP_UP_DATE_TIME_5_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(  g_TOP_UP_AMOUNT_5_ATTRIBUTE_ENABLE_d == 1)
    { g_TOP_UP_AMOUNT_5_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(g_ORIGINATING_DEVICE_5_ATTRIBUTE_ENABLE_d == 1)
    { g_ORIGINATING_DEVICE_5_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_FUEL_DEBT_REMAINING_ATTRIBUTE_ENABLE_d == 1)
    /*Debt Attribute Set*/
    { g_FUEL_DEBT_REMAINING_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_FUEL_DEBT_RECOVERY_RATE_ATTRIBUTE_ENABLE_d == 1)
    { g_FUEL_DEBT_RECOVERY_RATE_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_FUEL_DEBT_RECOVERY_PERIOD_ATTRIBUTE_ENABLE_d == 1)
    { g_FUEL_DEBT_RECOVERY_PERIOD_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_NON_FUEL_DEBT_REMAINING_ATTRIBUTE_ENABLE_d == 1)
    { g_NON_FUEL_DEBT_REMAINING_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if( g_NON_FUEL_DEBT_RECOVERY_RATE_ATTRIBUTE_ENABLE_d == 1)
    { g_NON_FUEL_DEBT_RECOVERY_RATE_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(   g_NON_FUEL_DEBT_RECOVERY_PERIOD_ATTRIBUTE_ENABLE_d == 1)
    { g_NON_FUEL_DEBT_RECOVERY_PERIOD_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif
    /*Supply Control Set*/
#if(g_PROPOSED_CHANGE_PROVIDER_ID_ATTRIBUTE_ENABLE_d == 1)
    { g_PROPOSED_CHANGE_PROVIDER_ID_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(g_PROPOSED_CHANGE_IMPLEMENTATION_TIME_ATTRIBUTE_ENABLE_d == 1)
    { g_PROPOSED_CHANGE_IMPLEMENTATION_TIME_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(   g_PROPOSED_CHANGE_SUPPLY_STATUS_ATTRIBUTE_ENABLE_d == 1)
    { g_PROPOSED_CHANGE_SUPPLY_STATUS_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c },
#endif
#if(   g_DELAYED_SUPPLY_INTERRUPT_VALUE_REMAINING_ATTRIBUTE_ENABLE_d == 1)
    { g_DELAYED_SUPPLY_INTERRUPT_VALUE_REMAINING_ATTRIBUTE_c,
      g_Unsigned_Sixteen_Bit_Integer_Data_c,
      g_Read_Write_Attribute_c },
#endif
#if( g_DELAYED_SUPPLY_INTERRUPT_VALUE_TYPE_ATTRIBUTE_ENABLE_d == 1)
    { g_DELAYED_SUPPLY_INTERRUPT_VALUE_TYPE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Write_Attribute_c },
#endif
	{g_NULL_c, g_NULL_c,g_NULL_c}
};

const ZCL_Cluster_Info_t ga_Prepayment_Server_Cluster_Info =
{
    g_PREPAYMENT_CLUSTER_c,
    (sizeof(ma_Prepayment_Server_Cluster_Attribute_Info)-1)/sizeof(ZCL_Attribute_Info_t),
    ma_Prepayment_Server_Cluster_Attribute_Info,

};
#endif          /*  ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 ) */


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


uint8_t ZCL_Handle_Payment_Cluster_Cmd ( uint8_t endPointId,
                                      uint8_t asduLength,
                                      uint8_t* pReceivedAsdu,
                                      uint8_t* pResponseLocation,
                                      ZCL_ActionHandler_t *pActionHandler,
                                      ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                      uint8_t * pAttrsDataLocation )
{
    uint8_t responseAsduLength = g_ZERO_c;
    pActionHandler->action = No_Response;
    pActionHandler->event.eventLength = asduLength - 0x03;

    if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {
#if ( g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d == 1 )

        if(g_SupplyStatusResCmd_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_PrepaymentSupplyStatusResEvent_c;
			 pActionHandler->action = Received_Response;
           }
        else{
            pActionHandler->action = Send_Response;
            return g_ZERO_c;
        }

#endif /* g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d*/

    }
    else
    {
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )

		if(g_SelectAvailableEmergencyCreditReqCmd_c  ==
			*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ){
			pActionHandler->event.eventId =
			g_PrepaymentSelectAvailEmerCreditReqEvent_c;
		}
		else if(g_ChangeSupplyReqCmd_c ==
			*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ){
			pActionHandler->event.eventId = g_PrepaymentChangeSupplyReqEvent_c;
		}
		else{
			pActionHandler->action = Send_Response;
			return g_ZERO_c; 		
		}
#endif /*g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d*/

    }
	memUtils_memCopy( pActionHandler->event.pEventData,
	 				 &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
	  					asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c );
	
	if ( Is_Default_Response_Required ( pReceivedAsdu ) ) {
		responseAsduLength = ZCL_CreateDefaultResponseCommand (
						  pReceivedAsdu,
						  pResponseLocation,
						  g_ZCL_Success_c );
		pActionHandler->action = Send_Response;
	
	}
    return responseAsduLength;
}
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
#endif                                /* g_PREPAYMENT_CLUSTER_ENABLE_d */
