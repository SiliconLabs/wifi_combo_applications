/*******************************************************************************
* ZCL_Commissioning_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Commissioning
*   cluster
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"


#if ( g_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Commissioning_Cluster.h"

/*******************************************************************************
* Global Constants
*******************************************************************************/


/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Commissioning_Server_Cluster_Attribute_Info [] =
{
    {
        g_COMMISSIONING_SHORT_ADDRESS_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_EXTENDED_PANID_c,
        g_IEEE_Address_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_PANID_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_CHANNEL_MASK_c,
        g_Thirty_Two_Bit_Bitmap_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_PROTOCOL_VERSION_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_STACK_PROFILE_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_STARTUP_CONTROL_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_TRUST_CENTER_ADDRESS_c,
        g_IEEE_Address_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_TRUST_CENTER_MASTER_KEY_c,
        g_One_Twenty_Eight_Bit_Sec_Key_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_NETWORK_KEY_c,
        g_One_Twenty_Eight_Bit_Sec_Key_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_USE_INSECURE_JOIN_c,
        g_Boolean_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_PRE_CONFIGURED_LINK_KEY_c,
        g_One_Twenty_Eight_Bit_Sec_Key_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_NETWORK_KEY_SEQ_NUM_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_NETWORK_KEY_TYPE_c,
        g_Eight_Bit_Enumeration_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_NETWORK_MANAGER_ADDRESS_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_SCAN_ATTEMPTS_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_TIME_BETWEEN_SCANS_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_REJOIN_INTERVAL_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_MAX_REJOIN_INTERVAL_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_INDIRECT_POLL_RATE_c,
        g_Unsigned_Sixteen_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_PARENT_RETRY_THRESHOLD_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_CONCENTRATOR_FLAG_c,
        g_Boolean_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_CONCENTRATOR_RADIUS_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c },

    {
        g_COMMISSIONING_CONCENTRATOR_DISCOVERY_TIME_c,
        g_Unsigned_Eight_Bit_Integer_Data_c,
        g_Read_Write_Attribute_c }

};
const ZCL_Cluster_Info_t ga_Commissioning_Server_Cluster_Info =
{
   g_COMMISSIONING_CLUSTER_c,
   sizeof( ma_Commissioning_Server_Cluster_Attribute_Info )/ sizeof(ZCL_Attribute_Info_t),
   ma_Commissioning_Server_Cluster_Attribute_Info
};

#endif   /*  ( g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 ) */


#if ( g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Commissioning_Client_Cluster_Info =
{
   g_COMMISSIONING_CLUSTER_c,
   g_NULL_c,
   g_NULL_c,
};

#endif          /*  ( g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1 ) */



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

uint8_t ZCL_Handle_Commissioning_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{


    pActionHandler->event.eventLength = asduLength - 0x03;

    if( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {

        pActionHandler->action = Received_Response;


#if ( g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1 )
            if(g_CommissioningRestartDeviceResp_c ==
               *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ) {

                pActionHandler->event.eventId =
                    g_CommissioningRestartDeviceRespEvent_c;
               }



            else if(g_CommissioningSaveStartupParametersResp_c ==
                    *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ){
                pActionHandler->event.eventId =
                    g_CommissioningSaveStartupParametersRespEvent_c ;
            }


            else if(g_CommissioningRestoreStartupParametersResp_c ==
               *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ){
                pActionHandler->event.eventId =
                    g_CommissioningRestoreStartupParametersRespEvent_c;
            }

            else if (g_CommissioningResetStartupParametersResp_c ==
                     *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ){

                pActionHandler->event.eventId =
                    g_CommissioningResetStartupParametersRespEvent_c;
            }


            else {
                pActionHandler->action = Send_Response;
                return g_ZERO_c;
            }
#endif /* g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d*/
    }
    else
    {
        pActionHandler->action = No_Response;


#if ( g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
        if(g_CommissioningRestartDeviceReq_c ==
           *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ){

            pActionHandler->event.eventId =
                g_CommissioningRestartDeviceReqEvent_c;
        }

        else if (g_CommissioningSaveStartupParametersReq_c ==
                 *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ){

            pActionHandler->event.eventId =
                g_CommissioningSaveStartupParametersReqEvent_c;
        }

        else if(g_CommissioningRestoreStartupParametersReq_c ==
                *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){
            pActionHandler->event.eventId =
                g_CommissioningRestoreStartupParametersReqEvent_c;
        }

        else if(g_CommissioningResetStartupParametersReq_c ==
                *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )){

            pActionHandler->event.eventId =
                g_CommissioningResetStartupParametersReqEvent_c;
        }
        else{
            pActionHandler->action = Send_Response;
                return g_ZERO_c;

        }

#endif /*g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/

    }
    memUtils_memCopy( pActionHandler->event.pEventData,
              &pReceivedAsdu[g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c],
              asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c );

    return g_ZERO_c;

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
#endif                        /* g_COMMISSIONING_CLUSTER_ENABLE_d */
