/*******************************************************************************
* ZCL_General_Groups_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Groups cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#include "stack_common.h"
#include "ZCL_Stack_Utility.h"
#include "ZCL_Interface.h"
#include "memory_utility.h"
#if( g_GROUPS_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Groups_Cluster.h"

#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Identify_Cluster.h"
#endif    /*g_IDENTIFY_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
/*Group table full - stack status*/
#define m_STACK_GROUP_TABLE_FULL_c                          0x8c
#define m_NULL_GROUP_ID_c                                   0xFFFF
#define g_Invalid_Request_c                                 0xFF
#define m_GROUP_ENTRY_FOUND_c                               0x00
#define m_GROUP_ADDR_LENGTH_c                               0x02
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Name_Support_Server_Attribute =
{
   g_GROUP_ATTRIBUTE_NAME_SUPPORT_ATTRIBUTE_c,
   g_Eight_Bit_Bitmap_Data_c,
   g_Read_Only_Attribute_c
};

/*Cluster Info for Groups server Cluster*/
const ZCL_Cluster_Info_t ga_Group_Server_Cluster_Info =
{
   g_GROUPS_CLUSTER_c,
   sizeof(ma_Name_Support_Server_Attribute)/sizeof(ZCL_Attribute_Info_t),
   &ma_Name_Support_Server_Attribute
};
#endif


/* Cluster Info for Groups client Cluster */
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Group_Client_Cluster_Info =
{
   g_GROUPS_CLUSTER_c,
   g_NULL_c,
   g_NULL_c
};
#endif

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_Groups_Cluster_Cmd
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
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )
    uint8_t status =g_ZERO_c;
    uint8_t groupCount;
    uint16_t groupId;
    uint8_t capacity=g_ZERO_c;          /* Initially taking capacity for group
                                           table as zero */
    uint8_t i;
    uint8_t maxGroupTable;              /* Max capacity of Group Table */
    uint8_t index = 0x01;
    uint8_t rspindex = g_ZERO_c;        /* For response cmmnd */
     /* return value */
    uint8_t respGroupCount=g_ZERO_c;    /* For putting the group count for response */
    uint8_t* respGroupList ;           /* For putting the group ids for response */

    uint8_t cmdDataOffset;
#endif
    uint8_t zclcmdId = pReceivedAsdu[ GetZCLHeaderLength( pReceivedAsdu ) - 0x01 ];
    uint8_t responseLength = g_ZERO_c;

    pActionHandler->action = Send_Response ;
    pActionHandler->event.eventLength = asduLength - g_EVENT_LENGTH_THREE_BYTES_c;

    if( ZCL_GetDirectionOfCommand ( pReceivedAsdu ) ==  g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) {
#if ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 )

        cmdDataOffset = GetZCLHeaderLength( pReceivedAsdu );

        /* Creating the resp cmmnd */
        *( pResponseAsduStartLocation + rspindex ) =
                        ( *pReceivedAsdu )| g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                            g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
            rspindex++;
            *( pResponseAsduStartLocation + rspindex ) = *( pReceivedAsdu + rspindex );
            rspindex++;

        switch( zclcmdId ) {
            case g_Add_Group_Command_c :
                /* Update Event Data */
                pActionHandler->event.eventId = g_Add_Group_Command_Events_c;

                memUtils_memCopy((uint8_t*)&(((Add_Group_Command*)pActionHandler->
                                      event.pEventData)->a_group_id),
                                      &pReceivedAsdu[ cmdDataOffset ],
                                      m_GROUP_ADDR_LENGTH_c  );

                memUtils_memCopy((uint8_t*)&(((Add_Group_Command*)pActionHandler->
                                      event.pEventData)->group_name),
                  &pReceivedAsdu[ cmdDataOffset + m_GROUP_ADDR_LENGTH_c ],
                  ( pReceivedAsdu[ cmdDataOffset + m_GROUP_ADDR_LENGTH_c ] + 1 ) );

                /* To add group addr to group table */
                status = ZCL_Stack_Add_Group_Request ( &pReceivedAsdu[ cmdDataOffset ],
                              endPointId );

                if( status == g_Invalid_Request_c){
                    status = g_ZCL_Failure_c;
                }
                else if( status == m_STACK_GROUP_TABLE_FULL_c ){
                    status = g_ZCL_Insufficient_Space_c;
                }
                /* Response creation */
                pResponseAsduStartLocation[rspindex++] = g_Add_Group_Response_c;
                pResponseAsduStartLocation[rspindex++] = status;
                memUtils_memCopy ((uint8_t*)&pResponseAsduStartLocation[rspindex ],
                          ( pReceivedAsdu + cmdDataOffset ), m_GROUP_ADDR_LENGTH_c );
                responseLength = rspindex + m_GROUP_ADDR_LENGTH_c ;
                break;

            case g_View_Group_Command_c:
                /* Response creation */
                pActionHandler->event.eventId = g_View_Group_Command_Events_c;

                memUtils_memCopy((uint8_t*)&(((View_Group_Command*)pActionHandler->event.pEventData)->a_group_id),
                         ( pReceivedAsdu + cmdDataOffset ), m_GROUP_ADDR_LENGTH_c );

                /* check the group id in group table */
                status = ZCL_Stack_View_Group_Request ( &pReceivedAsdu[ cmdDataOffset ] );

                pResponseAsduStartLocation[rspindex++] = g_View_Group_Response_c;
                pResponseAsduStartLocation[rspindex++] = status;

                memUtils_memCopy ((uint8_t*)&pResponseAsduStartLocation[rspindex],
                          (uint8_t*)&pReceivedAsdu[ cmdDataOffset ], m_GROUP_ADDR_LENGTH_c );

                rspindex += m_GROUP_ADDR_LENGTH_c;
                pResponseAsduStartLocation[rspindex] = g_NULL_c;
                responseLength = ++rspindex;
                break;

            case g_Get_Group_Membership_Command_c:

                pActionHandler->event.eventId = g_Get_Group_Membership_Command_Events_c;
                /* Max capacity of Group Table */
                maxGroupTable = ZCL_Group_Table_Max_Value();
                groupCount = pReceivedAsdu[ cmdDataOffset ];
                (((Get_Group_Membership_Command*)pActionHandler->event.pEventData)->group_count) =
                                                                groupCount;

                memUtils_memCopy( (uint8_t*)&(((Get_Group_Membership_Command*)pActionHandler->
                                       event.pEventData)->a_group_list ),
                         &pReceivedAsdu[ cmdDataOffset + 1 ],
                         (pReceivedAsdu[ cmdDataOffset ] * 0x02) );

                respGroupList = pResponseAsduStartLocation + rspindex + 3;

                if( g_ZERO_c == groupCount ) {

                    for( i=0; i  < maxGroupTable; i++ ){
                        groupId = ZCL_Stack_Group_Id( i );

                        /* checking capacity in group table */
                        if( m_NULL_GROUP_ID_c == groupId ){
                            capacity++;
                        }
                        else{
                            /* if group id is in group table, update the same to response frame */
                            memUtils_memCopy( respGroupList, (uint8_t*)&groupId, m_GROUP_ADDR_LENGTH_c );
                            respGroupCount++;
                            respGroupList +=2;
                            responseLength += 2;
                        }
                    }
                }
                else{
                    for( i= 0; i < groupCount; i++ ) {
                        memUtils_memCopy((uint8_t*)&groupId,
                                 &pReceivedAsdu[ cmdDataOffset + index ], m_GROUP_ADDR_LENGTH_c );

                        /* Verify the group id with endpoint in Group Table */
                        if( m_GROUP_ENTRY_FOUND_c == ZCL_Verify_GroupAddrAndEndPoint(groupId,endPointId) ) {
                            memUtils_memCopy(respGroupList, (uint8_t*)&groupId, m_GROUP_ADDR_LENGTH_c );
                            respGroupCount++;
                            respGroupList +=2;
                            responseLength += m_GROUP_ADDR_LENGTH_c ;
                        }
                        index += m_GROUP_ADDR_LENGTH_c;
                    }
                    for( i=0; i < maxGroupTable; i++ ){
                        if(m_NULL_GROUP_ID_c == ZCL_Stack_Group_Id(i)){
                            capacity++;
                        }
                    }
                }
                /* Create Response */
                pResponseAsduStartLocation[rspindex++] = g_Get_Group_Membership_Reponse_c;
                pResponseAsduStartLocation[rspindex++] = capacity;
                pResponseAsduStartLocation[rspindex++] = respGroupCount;

                responseLength = rspindex + responseLength;
                break;

            case g_Remove_Group_Command_c:

                memUtils_memCopy((uint8_t*)&groupId,
                                 (uint8_t*)&pReceivedAsdu[ cmdDataOffset ],
                                 m_GROUP_ADDR_LENGTH_c );
                pActionHandler->event.eventId = g_Remove_Group_Command_Events_c;
                memUtils_memCopy((uint8_t*)&(((Remove_Group_Command*)pActionHandler->event.pEventData)->a_group_id),
                          &pReceivedAsdu[ cmdDataOffset ], m_GROUP_ADDR_LENGTH_c );

                if( m_GROUP_ENTRY_FOUND_c == ZCL_Verify_GroupAddrAndEndPoint(groupId,endPointId) ){
                    /* Remove the group id corresponding to the end point */
                    status = ZCL_Stack_Remove_Group_Request((uint8_t*)&groupId,endPointId);
                    /* delete the scene entry */
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
                    ZCL_Callback_DeleteScenesTableEntry( endPointId, groupId );
#endif              /* ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 ) */
                }
                else{
                    status = g_ZCL_Not_Found_c;
                }
                pResponseAsduStartLocation[rspindex++] = g_Remove_Group_Response_c;
                pResponseAsduStartLocation[rspindex++] = status;
                memUtils_memCopy ((uint8_t*)&pResponseAsduStartLocation[rspindex],
                          (uint8_t*)&groupId, m_GROUP_ADDR_LENGTH_c );
                responseLength = rspindex + m_GROUP_ADDR_LENGTH_c;

                break;
            case g_Remove_All_Groups_Command_c:

                pActionHandler->event.eventId = g_Remove_All_Groups_Command_Events_c;
                status = ZCL_Stack_Remove_All_Groups_Request(endPointId);
#if ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 )
                for ( i = 0; i < ZCL_Group_Table_Max_Value(); i++ ){

                    groupId = ZCL_Stack_Group_Id( i );
                    ZCL_Callback_DeleteScenesTableEntry( endPointId, groupId );
                }
#endif              /* ( g_SCENES_SERVER_CLUSTER_ENABLE_d == 1 ) */
                if ( Is_Default_Response_Required ( pReceivedAsdu ) ){
                    return (ZCL_CreateDefaultResponseCommand (pReceivedAsdu ,
                                                      pResponseAsduStartLocation, status) );
                }
                pActionHandler->action = No_Response ;

                break;
            case g_Add_Group_If_Identifying_Command_c:

                pActionHandler->event.eventId = g_Add_Group_If_Identifying_Command_Events_c;

                memUtils_memCopy((uint8_t*)&(((Add_Group_If_Identifying_Command*)pActionHandler->
                                      event.pEventData)->a_group_id),
                         (uint8_t*)&pReceivedAsdu[ cmdDataOffset ], m_GROUP_ADDR_LENGTH_c );

                memUtils_memCopy((uint8_t*)&(((Add_Group_If_Identifying_Command*)pActionHandler->
                                      event.pEventData)->a_group_name),
                         &pReceivedAsdu[ cmdDataOffset + 2], pReceivedAsdu[ cmdDataOffset + 2] );

                /* Add group id to group table if the device identify itself */
#if( g_IDENTIFY_SERVER_CLUSTER_ENABLE_d == 1 )
                if(g_TRUE_c == ZCL_Callback_Is_Device_In_Identifing() )
                {
                    memUtils_memCopy((uint8_t*)&groupId,
                         (uint8_t*)&pReceivedAsdu[ cmdDataOffset], m_GROUP_ADDR_LENGTH_c );
                    if( m_GROUP_ENTRY_FOUND_c != ZCL_Verify_GroupAddrAndEndPoint( groupId,
                                                                endPointId ) ) {
                        status = ZCL_Stack_Add_Group_Request((uint8_t*)&groupId, endPointId );
                    }
                }
#endif
                if ( Is_Default_Response_Required ( pReceivedAsdu ) ) {
                    return ZCL_CreateDefaultResponseCommand ( pReceivedAsdu ,
                                                      pResponseAsduStartLocation, status );
                }
                pActionHandler->action = No_Response ;
                break;

            default :
                break;
        }
#endif      /* ( g_GROUPS_SERVER_CLUSTER_ENABLE_d == 1 ) */
    }
    /* Checking direction - Srever to client */
    else {
#if ( g_GROUPS_CLIENT_CLUSTER_ENABLE_d == 1 )

        switch( zclcmdId ) {

            case g_Add_Group_Response_c :
            case g_View_Group_Response_c :
            case g_Get_Group_Membership_Reponse_c :
            case g_Remove_Group_Response_c :
                pActionHandler->action = Received_Response;
                break;
            default :
                break;
        }
#endif      /* g_GROUPS_SERVER_CLUSTER_ENABLE_d */
    }
    return responseLength;





}

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*g_GROUPS_CLUSTER_ENABLE_d*/
