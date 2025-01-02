/*******************************************************************************
* AppCommissioningUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Commissioning
*   Cluster
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include"ZigBeeDataTypes.h"
//#include "nvm.h"
#include "stack_common.h"
#include "stack_common.h"
#include "ZCL_Common.h"
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
//#include "psm.h"

 ZDO_Configuration_Table_t gp_ZDO_Configuration[];
 Startup_Attribute_Set_t gp_Active_Startup_Attrib_Set[];

#if ( g_COMMISSIONING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Commissioning_Cluster.h"
#include "AppZclDefaultAttributesData.h"
#include "AppCommissioningUtilities.h"
#include "commissioning_support.h"
#include "ApptimerId.h"
#include "ApplicationThinLayer.h"
#endif

/*******************************************************************************
* Global Constants
*******************************************************************************/
#define m_COMMISSIONING_RESP_LEN_c              (0x04)

#define m_ONE_KB_c		0x0400

/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if(g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)
    CommissioningResponseCmdPayload_t ResposeCmdPayload;
#endif
/* Default values of the Startup Attribute Set */
extern const Startup_Attribute_Set_t Startup_Attribute_Set_Default;
#if(g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1)
//extern uint8_t halCommonReadFromNvm ( uint32_t address, uint8_t* p_buffer, uint32_t length );

extern const ZDO_Configuration_Table_t g_Table_Default;
extern uint32_t const g_Configuration_Storage_Start_Location_c;
commissioningRestartDeviceCmdPayload_t   RestartCmdPayload;
//extern Commissioning_Cluster_t commissioning_cluster_server_values;
//extern Commissioning_Cluster_t commissioning_cluster_server_default_values;
Commissioning_Cluster_t commissioning_cluster_server_values;
Commissioning_Cluster_t commissioning_cluster_server_default_values;
#endif
uint32_t DelayDuration = 200;
#if(DEBUG_TEST == 1)
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];

#endif
#if(g_COMMISSIONING_CLUSTER_ENABLE_d == 1)
bool LeaveConfFlagForCommission = g_FALSE_c;
#endif
extern uint32_t g_NVM_SAS_Start_Offset_c;
/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

#if (g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
static RestartCmdReturnValue_t updateStartupAttributes(uint8_t restartCmdOption);
static void updateZigBeeStartupAttributes(Startup_Attribute_Set_t *attributeSet);
static void updateZigBeeStartupAttributesToDefault();
static void SendLeaveNetwork(uint8_t timerId );
static void App_Restart_Device(uint8_t timerId);
void App_ZDOupdateConfigurationParameters( uint8_t SAS_index );
#endif


/*******************************************************************************
* Public Functions
*******************************************************************************/

void App_CommissioningEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer )
{
#if ( g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
    Startup_Attribute_Set_t attributeSet;
#endif
    switch ( pZCLEvent->eventId )
    {
#if ( g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
        case g_CommissioningRestartDeviceReqEvent_c :
        {
            RestartCmdReturnValue_t status;
            /* Store the payload received in the frame, as restarting
            the device should be performed in the background after
            specified delay(sec) and jitter(milliseconds). */
            RestartCmdPayload.Options = pZCLEvent->pEventData[0];
            RestartCmdPayload.Delay = pZCLEvent->pEventData[1];
            RestartCmdPayload.Jitter = pZCLEvent->pEventData[2];

            status = updateStartupAttributes(RestartCmdPayload.Options);

            /* Send restart device response to the commissioning Device.
               The status can be SUCCESS or INCONSISTENT_STARTUP_STATE */
            SendCommissioningClusterResponse(pBuffer,
                g_CommissioningRestartDeviceResp_c,status);

            if(status == g_SUCCESS_ZCL_c ){
                /* Delay to confirm that response is sent */
                DelayDuration = 1000 * m_MILLISECONDS_c;
                /* Restart device after getting leave confirmation */
                tmr_startRelativeTimer (m_APPLICATION_COMMI_RESTART_TIMER_c,
                    DelayDuration ,SendLeaveNetwork);

                /* checks sub-field immediate if val is 1 restart device
                cmd is sent immediately*/
                if(RestartCmdPayload.Options & 0x08)
                {
                    DelayDuration = 1000 * m_MILLISECONDS_c; /*1sec*/

                }
                else {
                    DelayDuration = (RestartCmdPayload.Delay * m_MILLISECONDS_c +
                                     RestartCmdPayload.Jitter )*m_MILLISECONDS_c;
                }
            }
        }
        break;
        case g_CommissioningSaveStartupParametersReqEvent_c:
        {
            uint8_t status = g_ZCL_Insufficient_Space_c;

            CommissioningSaveStartupParamsCmdPayload_t saveCmdParam;
            saveCmdParam.Index = pZCLEvent->pEventData[1];

            if(saveCmdParam.Index < m_MAX_SAS_ATTRIBUTE_SET_c) {
                /* updates current ZCL startup attribute set to
                Zigbee StartupAttributes */
                Startup_Attribute_Set_t localAttributeSet;
                updateZigBeeStartupAttributes(&localAttributeSet);
#if( g_COMMISSIONING_CAPABILITIES_d == 1 )
                /* Updating the SAS parameters with SAS index as zero */
                //ZDOupdateSAS((uint8_t*)&localAttributeSet,
                //             saveCmdParam.Index);
                rsi_zigb_update_sas((uint8_t*)&localAttributeSet);
#endif /*g_COMMISSIONING_CAPABILITIES_d */
                status = g_SUCCESS_ZCL_c;
            }

            /* Send save startup parameters response to the commissioning Device.
               The status will be SUCCESS or g_ZCL_Insufficient_Space_c */
            SendCommissioningClusterResponse(pBuffer,
                g_CommissioningSaveStartupParametersResp_c,status);

        }
        break;
        case g_CommissioningRestoreStartupParametersReqEvent_c:
        {
            CommissioningRestoreStartupParamsCmdPayload_t RestoreCmdPayload;
            RestoreCmdPayload.Index = pZCLEvent->pEventData[1];

            uint8_t status = g_ZCL_Invalid_Field_c;

            if(RestoreCmdPayload.Index < m_MAX_SAS_ATTRIBUTE_SET_c ){
                App_ZDOupdateConfigurationParameters(RestoreCmdPayload.Index);
                if(gp_Active_Startup_Attrib_Set->startup_control != 0xff){
                    RestoreCurrentStartUpAttribute();
                    status = g_SUCCESS_ZCL_c;
                }
                else {
                    status = g_ZCL_Invalid_Field_c;
                }
            }
            /* Send restore startup parameters response to the commissioning
            Device.The status will be SUCCESS  */
            SendCommissioningClusterResponse(pBuffer,
                g_CommissioningRestoreStartupParametersResp_c,status);

        }
        break;
        case g_CommissioningResetStartupParametersReqEvent_c:
        {
//            extern Commissioning_Cluster_t commissioning_cluster_server_values,
//                commissioning_cluster_server_default_values;
            /* Check if RESET_CURRENT bit is set in the option field of the
               reset startup parametes command received */
            if( pZCLEvent->pEventData[0] & g_Reset_Current_c ){
                /* Put the default values onto current commissioning cluster
                attribute set */
                memUtils_memCopy((uint8_t*)&commissioning_cluster_server_values,
                         (uint8_t*)&commissioning_cluster_server_default_values,
                    sizeof(Commissioning_Cluster_t));
            }

            if(pZCLEvent->pEventData[0] & g_Reset_All_c ){
                uint8_t SAS_index ;

//                /* Put the default values onto current commissioning cluster
//                attribute set */
//                memUtils_memCopy((uint8_t*)&commissioning_cluster_server_values,
//                         (uint8_t*)&commissioning_cluster_server_default_values,
//                    sizeof(Commissioning_Cluster_t));

                updateZigBeeStartupAttributesToDefault();
                memUtils_memCopy((uint8_t*)&attributeSet,
                         (uint8_t*)gp_Active_Startup_Attrib_Set,
                    sizeof(Startup_Attribute_Set_t));


                /* To reset all other attributes set*/
                for(SAS_index = 0x00;SAS_index < m_MAX_SAS_ATTRIBUTE_SET_c ;
                SAS_index++){
                    /* Updating the SAS parameters with SAS index as zero */
#if( g_COMMISSIONING_CAPABILITIES_d == 1 )
                    //ZDOupdateSAS((uint8_t*)&attributeSet,SAS_index);
                    rsi_zigb_update_sas((uint8_t*)&attributeSet);
#endif
                }
                //RP: why repeating? updateZigBeeStartupAttributesToDefault();
            }

            if(pZCLEvent->pEventData[0] & g_Erase_Index_c ){
                /* In order to erase particular attribute set,we are making
                startup control attribute set as invalid value */
                updateZigBeeStartupAttributesToDefault();

                /* check for valid SAS Index */
                if(pZCLEvent->pEventData[1] < m_MAX_SAS_ATTRIBUTE_SET_c){
                    /* Setting startupcontrol to invalid value */
                    gp_Active_Startup_Attrib_Set->startup_control = 0xff;
#if( g_COMMISSIONING_CAPABILITIES_d == 1 )
                    /* Updating the SAS parameters with SAS index as zero */
                    //ZDOupdateSAS((uint8_t*)&gp_Active_Startup_Attrib_Set,
                    //             pZCLEvent->pEventData[1]);
                    rsi_zigb_update_sas((uint8_t*)&gp_Active_Startup_Attrib_Set
                                 );
#endif
                }
            }
            /* Send Reset startup parameters response to the commissioning Device.
               The status will be SUCCESS */
            SendCommissioningClusterResponse(pBuffer,
                g_CommissioningResetStartupParametersResp_c,g_SUCCESS_ZCL_c);
        }

        break;

#endif /*g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/
#if (g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d == 1)
        case g_CommissioningRestartDeviceRespEvent_c:
            /* Application Should handle Reception of restart_device_Response */
            break;
        case g_CommissioningSaveStartupParametersRespEvent_c:
            /* Application Should handle Reception of save startup param */
            break;
        case g_CommissioningRestoreStartupParametersRespEvent_c:
            /* Application Should handle Reception of restore startup param*/
            break;
        case g_CommissioningResetStartupParametersRespEvent_c:
            /* Application Should handle Reception of reset startup param*/
            break;
#endif /*g_COMMISSIONING_CLIENT_CLUSTER_ENABLE_d*/
        default:
            break;
    }
}

/******************************************************************************/
#if (g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
void SendCommissioningClusterResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status)
{

    uint8_t destAddress[8];
    /* ZCL Header Creation */
    apsdeDataPacket->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                            g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = responseType;
    apsdeDataPacket->a_asdu[3] = status;

    memUtils_memCopy(destAddress , apsdeDataPacket->src_address.IEEE_address, 0x08 );

    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_COMMISSIONING_CLUSTER_c, m_COMMISSIONING_RESP_LEN_c,
        apsdeDataPacket->a_asdu );
}
/******************************************************************************/
void RestoreCurrentStartUpAttribute()
{
    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.extendedPANId),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->a_extended_pan_id),8);

    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.channelMask) ,
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->channel_mask)
             ,4);

    commissioning_cluster_server_values.protocolVersion =
       gp_Active_Startup_Attrib_Set->protocol_version;

    commissioning_cluster_server_values.stackProfile =
    gp_Active_Startup_Attrib_Set->stack_profile;

    commissioning_cluster_server_values.startupControl =
    gp_Active_Startup_Attrib_Set->startup_control;


    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.trustCenterAddress),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->a_trust_center_address),8);



    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.networkKey),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->a_network_key),16);

    commissioning_cluster_server_values.useInsecureJoin =
        gp_Active_Startup_Attrib_Set->use_insecure_join;

    commissioning_cluster_server_values.networkKeySeqNum =
        gp_Active_Startup_Attrib_Set->network_key_seq_num;

    commissioning_cluster_server_values.networkKeyType =
        gp_Active_Startup_Attrib_Set->network_key_type;


    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.
                         networkManagerAddress),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->network_manager_address)
              ,2);

    commissioning_cluster_server_values.scanAttempts =
        gp_Active_Startup_Attrib_Set->scan_attempts;


    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.
                         timeBetweenScans),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->time_between_scans),2);

    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.
                         rejoinInterval),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->rejoin_interval),2);

    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.
                         maxRejoinInterval),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->max_rejoin_interval),2);


    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.
                         indirectPollRate),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->indirect_poll_rate),2);

    commissioning_cluster_server_values.parentRetryThreshold =
        gp_Active_Startup_Attrib_Set->parent_retry_threshold;

    commissioning_cluster_server_values.concentratorFlag =
        gp_Active_Startup_Attrib_Set->concentrator_flag;


    commissioning_cluster_server_values.concentratorRadius =
        gp_Active_Startup_Attrib_Set->concentrator_radius;


    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.concentratorDiscoveryTime),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->concentrator_discovery_time),2);


    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.shortAddr),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->a_short_address),2);

    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.PANId),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->a_pan_id),2);

    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.trustCenterMasterKey),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->a_trustcenter_master_key),
              16);

    memUtils_memCopy((uint8_t*)&(commissioning_cluster_server_values.preconfiguredLinkKey),
             (uint8_t*)&(gp_Active_Startup_Attrib_Set->a_preconfigured_link_key)
             ,16);
}
/******************************************************************************/

static void SendLeaveNetwork(uint8_t timerId )
{

    //App_CreateAndSendLeaveRequest(g_NULL_c /* self leave*/);
    rsi_zigb_leave_network();
    /* Restart device after delay */
    tmr_startRelativeTimer (m_APPLICATION_COMMI_RESTART_TIMER_c,
        DelayDuration ,App_Restart_Device);
}


#endif /*g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d */

/******************************************************************************/
#if (g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d == 1 )
static RestartCmdReturnValue_t updateStartupAttributes(uint8_t restartCmdOption)
{

    uint8_t startMode ;
#if( g_COMMISSIONING_CAPABILITIES_d == 1 )
    RestartCmdReturnValue_t status;
#endif
    startMode = (restartCmdOption & 0x07);

    if(startMode == 0x00 ) {
#if( g_COMMISSIONING_CAPABILITIES_d == 1 )
        uint8_t SAS_index = 0x00;
        status = validateSAS();
        /* validate the commissioning cluster attribute set, for consistency */
        if( g_INCONSISTENT_STARTUP_STATE_c == status){
            /* Invalid Commissioning Startup Attribute */
            return g_INCONSISTENT_STARTUP_STATE_c;
        }
        /* install the current set of startup parameters.*/
        updateZigBeeStartupAttributes(gp_Active_Startup_Attrib_Set);

        /* Updating the SAS parameters with SAS index as zero */
        //ZDOupdateSAS((uint8_t*)&gp_Active_Startup_Attrib_Set,SAS_index);
        rsi_zigb_update_sas((uint8_t*)&gp_Active_Startup_Attrib_Set);

        //ZDOactivateSASindex(SAS_index);
#endif

    }
    else if(startMode > 0x01 ){
        /* start Mode cannot be more than 0x01 */
        return g_INCONSISTENT_STARTUP_STATE_c;
    }
    return g_SUCCESS_ZCL_c;
}

/******************************************************************************/
static void App_Restart_Device(uint8_t timerId)
{
    uint8_t restartBufferIndex;
    uint8_t *pBufferRx;

    if(LeaveConfFlagForCommission == g_TRUE_c) {

        /*Allocate a buffer to send leave request*/
        restartBufferIndex = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
#if (DEBUG_TEST == 1)
        /* Buffer ID --> 43 */
        allocatedbuffer[restartBufferIndex]=43;
        freebuffer[restartBufferIndex][0]= 43;
        freebuffer[restartBufferIndex][1]= 0;
#endif
        /*This Buffer is used for to issue restart request*/
        if(g_NO_AVAILABLE_BUFFER_c != restartBufferIndex) {
            pBufferRx = buffMgmt_getBufferPointer(restartBufferIndex);
            *pBufferRx = g_ZDO_Restart_Request_c;
            /* Handle Restart Device*/
            //ZDO_AppZDPrequest(restartBufferIndex);
            rsi_zigb_leave_network();
        }
        LeaveConfFlagForCommission = g_FALSE_c;
    }
}

/******************************************************************************/

static void updateZigBeeStartupAttributes(Startup_Attribute_Set_t *pAttributeSet)
{

    memUtils_memCopy((uint8_t*)&(pAttributeSet->a_extended_pan_id),
             (uint8_t*)&(commissioning_cluster_server_values.
                 extendedPANId) ,8);
    memUtils_memCopy((uint8_t*)&(pAttributeSet->channel_mask),
             (uint8_t*)&(commissioning_cluster_server_values.
                 channelMask) ,4);
    pAttributeSet->protocol_version =
        commissioning_cluster_server_values.protocolVersion;

    pAttributeSet->stack_profile =
        commissioning_cluster_server_values.stackProfile;

    pAttributeSet->startup_control =
        commissioning_cluster_server_values.startupControl;

    memUtils_memCopy((uint8_t*)&(pAttributeSet->a_trust_center_address),
             (uint8_t*)&(commissioning_cluster_server_values.
                 trustCenterAddress) ,8);

    memUtils_memCopy((uint8_t*)&(pAttributeSet->a_network_key),
             (uint8_t*)&(commissioning_cluster_server_values.
                 networkKey) ,16);

    pAttributeSet->use_insecure_join =
        commissioning_cluster_server_values.useInsecureJoin;

    pAttributeSet->network_key_seq_num =
        commissioning_cluster_server_values.networkKeySeqNum;

    pAttributeSet->network_key_type =
        commissioning_cluster_server_values.networkKeyType;

    memUtils_memCopy((uint8_t*)&(pAttributeSet->network_manager_address),
             (uint8_t*)&(commissioning_cluster_server_values.
                         networkManagerAddress) ,2);

    pAttributeSet->scan_attempts =
        commissioning_cluster_server_values.scanAttempts;


    memUtils_memCopy((uint8_t*)&(pAttributeSet->time_between_scans),
             (uint8_t*)&(commissioning_cluster_server_values.
                         timeBetweenScans) ,2);

    memUtils_memCopy((uint8_t*)&(pAttributeSet->rejoin_interval),
             (uint8_t*)&(commissioning_cluster_server_values.
                         rejoinInterval) ,2);

    memUtils_memCopy((uint8_t*)&(pAttributeSet->max_rejoin_interval),
             (uint8_t*)&(commissioning_cluster_server_values.
                         maxRejoinInterval) ,2);
    memUtils_memCopy((uint8_t*)&(pAttributeSet->indirect_poll_rate),
             (uint8_t*)&(commissioning_cluster_server_values.
                         indirectPollRate) ,2);

    pAttributeSet->parent_retry_threshold =
        commissioning_cluster_server_values.parentRetryThreshold;

    pAttributeSet->concentrator_flag =
        commissioning_cluster_server_values.concentratorFlag;

//    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->concentrator_radius),
//             (uint8_t*)&(commissioning_cluster_server_values.
//                         concentratorRadius) ,2);

    pAttributeSet->concentrator_radius =
        commissioning_cluster_server_values.concentratorRadius;
    pAttributeSet->concentrator_radius &= 0x00FF;


    memUtils_memCopy((uint8_t*)&(pAttributeSet->concentrator_discovery_time),
             (uint8_t*)&(commissioning_cluster_server_values.
                         concentratorDiscoveryTime) ,2);

    memUtils_memCopy((uint8_t*)&(pAttributeSet->a_short_address),
             (uint8_t*)&(commissioning_cluster_server_values.
                         shortAddr) ,2);

    memUtils_memCopy((uint8_t*)&(pAttributeSet->a_pan_id),
             (uint8_t*)&(commissioning_cluster_server_values.
                         PANId) ,2);

    memUtils_memCopy((uint8_t*)&(pAttributeSet->a_trustcenter_master_key),
             (uint8_t*)&(commissioning_cluster_server_values.
                         trustCenterMasterKey) ,16);

    memUtils_memCopy((uint8_t*)&(pAttributeSet->a_preconfigured_link_key),
             (uint8_t*)&(commissioning_cluster_server_values.
                         preconfiguredLinkKey) ,16);

}
/******************************************************************************/
static void updateZigBeeStartupAttributesToDefault()
{


    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->a_extended_pan_id),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                 extendedPANId) ,8);
    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->channel_mask),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                 channelMask) ,4);
    gp_Active_Startup_Attrib_Set->protocol_version =
        commissioning_cluster_server_default_values.protocolVersion;

    gp_Active_Startup_Attrib_Set->stack_profile =
        commissioning_cluster_server_default_values.stackProfile;

    gp_Active_Startup_Attrib_Set->startup_control =
        commissioning_cluster_server_default_values.startupControl;

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->a_trust_center_address),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                 trustCenterAddress) ,8);

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->a_network_key),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                 networkKey) ,16);

    gp_Active_Startup_Attrib_Set->use_insecure_join =
        commissioning_cluster_server_default_values.useInsecureJoin;

    gp_Active_Startup_Attrib_Set->network_key_seq_num =
        commissioning_cluster_server_default_values.networkKeySeqNum;

    gp_Active_Startup_Attrib_Set->network_key_type =
        commissioning_cluster_server_default_values.networkKeyType;

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->network_manager_address),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         networkManagerAddress) ,2);

    gp_Active_Startup_Attrib_Set->scan_attempts =
        commissioning_cluster_server_default_values.scanAttempts;


    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->time_between_scans),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         timeBetweenScans) ,2);

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->rejoin_interval),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         rejoinInterval) ,2);

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->max_rejoin_interval),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         maxRejoinInterval) ,2);
    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->indirect_poll_rate),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         indirectPollRate) ,2);

    gp_Active_Startup_Attrib_Set->parent_retry_threshold =
        commissioning_cluster_server_default_values.parentRetryThreshold;

    gp_Active_Startup_Attrib_Set->concentrator_flag =
        commissioning_cluster_server_default_values.concentratorFlag;

//    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->concentrator_radius),
//             (uint8_t*)&(commissioning_cluster_server_values.
//                         concentratorRadius) ,2);

    gp_Active_Startup_Attrib_Set->concentrator_radius =
        commissioning_cluster_server_default_values.concentratorRadius;
    gp_Active_Startup_Attrib_Set->concentrator_radius &= 0x00FF;


    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->concentrator_discovery_time),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         concentratorDiscoveryTime) ,2);

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->a_short_address),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         shortAddr) ,2);

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->a_pan_id),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         PANId) ,2);

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->a_trustcenter_master_key),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         trustCenterMasterKey) ,16);

    memUtils_memCopy((uint8_t*)&(gp_Active_Startup_Attrib_Set->a_preconfigured_link_key),
             (uint8_t*)&(commissioning_cluster_server_default_values.
                         preconfiguredLinkKey) ,16);

}

/******************************************************************************/
RestartCmdReturnValue_t validateSAS()
{
    uint8_t index;

    RestartCmdReturnValue_t status = g_INCONSISTENT_STARTUP_STATE_c;

    /* Verifying Short address */
    if (*((uint16_t*)commissioning_cluster_server_values.shortAddr) > 0xFFF7) {
	    /* short address cannot be reserved values.*/
	    return status;
    }

    /* Verifying Extended PANID */
    for (index = 0; index < g_EXTENDED_PANID_LENGTH_c; index++) {
        /* Check any of the bytes of the extended PANID is not 0xFF */
        if (commissioning_cluster_server_values.extendedPANId[index] != 0xFF) {
            break;
        }
    }
    /* Extended PANID can't be 0xFFFFFFFFFFFFFFFF */
    if (index == g_EXTENDED_PANID_LENGTH_c) {
        return status;
    }

    if (
        /* Channels can be between 0x800 (channel 11) and
        0x04000000(channel 26) */
        (*((uint32_t *)commissioning_cluster_server_values.channelMask) & 0xF80007FF)||
        /* protocol version can only be nwkcProtocolVersion */
        (commissioning_cluster_server_values.protocolVersion != 0x02 )
                || /* stack profile can be ZIGBEE_PRO_STACK_PROFILE*/
        ( commissioning_cluster_server_values.stackProfile != 0x02) ||
        /* Startup control cannot be more than 0x03 */
        (commissioning_cluster_server_values.startupControl > 0x03)) {
        return status;
    }
#if(g_ZIGBEE_COORDINATOR_d == 1)
    /* Coordinator can only form the network */
    if( ( g_FORM_NETWORK_c != commissioning_cluster_server_values.
         startupControl ) && ( g_SILENT_START_NETWORK_c !=
            commissioning_cluster_server_values.startupControl ) ) {
        return status;
    }

#elif ( ( g_ZIGBEE_ROUTER_d == 1 ) || ( g_ZIGBEE_END_DEVICE_d == 1 ) ||\
 ( g_ZIGBEE_ACTIVE_END_DEVICE_d == 1 ) )
    /* If the device is not coordinator, it cannot form the network */
    if (g_FORM_NETWORK_c == commissioning_cluster_server_values.
        startupControl) {
        return status;
    }
    /* Startup parameter */
    if ((commissioning_cluster_server_values.startupControl ==
         g_REJOIN_NETWORK_c)
            || (commissioning_cluster_server_values.startupControl ==
                g_SILENT_START_NETWORK_c)) {
        for (index = 0; index < g_EXTENDED_PANID_LENGTH_c; index++) {
            /* Check any Byte in the extended panid is having non zero value. */
            if (commissioning_cluster_server_values.extendedPANId[index] != 0x00) {
                break;
            }
        }
    }

    if (g_EXTENDED_PANID_LENGTH_c == index) {
        /* If StartupControl is REJOIN_NETWORK, Extended PANID cannot
        be 0x0000000000000000. */
        return status;
    }

    if (commissioning_cluster_server_values.startupControl ==
        g_ASSOC_JOIN_NETWORK_c) {
        for (index = 0; index < g_EXTENDED_PANID_LENGTH_c; index++) {
            /* Check any byte of extended panid has non zero value */
            if (commissioning_cluster_server_values.extendedPANId[index]
                != 0x00) {
                /* If StartupControl is ASSOC_JOIN_NETWORK,
                    Extended PANID should be 0x0000000000000000. */
                return status;
            }
        }
    }

#endif
    /* Verifying Trust center address */
    for (index = 0; index < g_EXTENDED_PANID_LENGTH_c; index++) {
        if (commissioning_cluster_server_values.trustCenterAddress[index] != 0xFF) {
            break;
        }
    }
    /* Trust center's extended address can't be 0xFFFFFFFFFFFFFFFF */
    if (index == g_EXTENDED_PANID_LENGTH_c) {
        /* Trust center's extended address in inconsitence state */
        return status;
    }
     //doubt
    /* Trust center's extended address can't be 0x0000000000000000 */
    for (index = 0; index < g_EXTENDED_PANID_LENGTH_c; index++) {
        if (commissioning_cluster_server_values.trustCenterAddress[index] != 0x00) {
            break;
        }
    }

    if (index == g_EXTENDED_PANID_LENGTH_c) {
        /* Trust center's extended address in inconsitence state */
        return status;
    }

    if ( /* UseInsecured join can be true or false */
    ((commissioning_cluster_server_values.useInsecureJoin != g_TRUE_c)
            && (commissioning_cluster_server_values.useInsecureJoin != g_FALSE_c)) ||
    /* Network key type can only be 0x02 or 0x05 */
    ((commissioning_cluster_server_values.networkKeyType != 0x02)
            && (commissioning_cluster_server_values.networkKeyType != 0x05)) ||
    /* Network manager address should be a valid short address */
    (*((uint16_t*)commissioning_cluster_server_values.networkManagerAddress) > 0xFFF7)) {
        /* Commissioning attribute set is in a inconsistence state */
        return status;
    }
    return g_SUCCESS_ZCL_c;

}

/******************************************************************************/
void App_ZDOupdateConfigurationParameters( uint8_t SAS_index )
{

#if ( g_COMMISSIONING_CAPABILITIES_d == 1 )
    uint8_t SASOffset = 1;
    uint32_t offset = (m_ONE_KB_c + 2);

    //halCommonReadFromNvm((uint8_t *)gp_ZDO_Configuration,offset,
    //                     sizeof(ZDO_Configuration_Table_t));

    SASOffset = SAS_index + 2;
    //halCommonReadFromNvm((uint8_t *) gp_Active_Startup_Attrib_Set, m_ONE_KB_c
    //        + SASOffset * m_SIZE_CONFIGURATION_c,
    //        sizeof(Startup_Attribute_Set_t));
#else
    memUtils_memCopy((uint8_t *) gp_ZDO_Configuration,
            (uint8_t *) &g_Table_Default, sizeof(ZDO_Configuration_Table_t));

    memUtils_memCopy((uint8_t *) gp_Active_Startup_Attrib_Set,
            (uint8_t *) &Startup_Attribute_Set_Default,
            sizeof(Startup_Attribute_Set_t));
#endif
}
#endif /* g_COMMISSIONING_SERVER_CLUSTER_ENABLE_d*/

/******************************************************************************/

