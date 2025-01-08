/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "common_includes.h"
#include "memory_utility.h"
#include "ZCL_Functionality.h"
#if( g_DOOR_LOCK_CLUSTER_ENABLE_d ==1 )
#include "ZCL_Interface.h"
#include "ApplicationThinLayer.h"
#include "AppDoorLockUtilities.h"
#include "ZCL_Closures_Door_Lock_Cluster.h"

/*-----------------------------------------------------------------------------
* Global constant declarations
*----------------------------------------------------------------------------*/
#define m_DOOR_LOCKS_CMD_RSP_LENGTH_c                   0x04


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

uint8_t lockState;
uint8_t doorState;


/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
static void App_CreatAndSend_Lock_Door_Response( uint8_t *LockDoorResp,
                                              APSDE_Data_Indication_t *pBuffer );
static void App_CreatAndSend_unLock_Door_Response( uint8_t *unLockDoorResp,
                                              APSDE_Data_Indication_t *pBuffer );



/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/


void App_DoorLockEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
    switch ( pZCLEvent->eventId )
    {
#if (g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d == 1)
	case g_Lock_Door_Event_c:
	    App_CreatAndSend_Lock_Door_Response( pZCLEvent->pEventData, pBuffer );

		break;
    case g_Unlock_Door_Event_c:
	    App_CreatAndSend_unLock_Door_Response( pZCLEvent->pEventData, pBuffer );
        break;
#endif   /*g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d */
    default :
        break;
    }
}

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

#if ( g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d == 1 )
static void App_CreatAndSend_Lock_Door_Response( uint8_t *LockDoorResp,
                                                      APSDE_Data_Indication_t *pBuffer )
{
	uint8_t	asduLen;
	uint8_t status = g_ZCL_Success_c;
	
	if( LockDoorResp == g_NULL_c || pBuffer == g_NULL_c)
	{
	  status = g_ZCL_Failure_c;
	}

    /* ZCL Header Creation */
    pBuffer->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c;
    pBuffer->a_asdu[1] = App_GetTransSeqNumber();
    pBuffer->a_asdu[2] = g_Lock_Door_Response_c;

    App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DOOR_LOCK_CLUSTER_c,
                                g_LOCK_STATE_ATTRIBUTE_c,
                                (uint8_t*)&lockState,
                                g_Read_Attribute_Data_c );
	
	App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DOOR_LOCK_CLUSTER_c,
                                g_DOOR_STATE_ATTRIBUTE_c,
                                (uint8_t*)&doorState,
                                g_Read_Attribute_Data_c );
	
	
	if( doorState == Error){
		status = g_ZCL_Failure_c;
	}
	else if ( lockState == Locked){
	  status = g_ZCL_Duplicate_Exists_c;
	}
	else {
		lockState =Locked;
		doorState = Closed;
		App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DOOR_LOCK_CLUSTER_c,
                                g_LOCK_STATE_ATTRIBUTE_c,
                                (uint8_t*)&lockState,
                                g_Write_Attribute_Data_c );
		
		App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DOOR_LOCK_CLUSTER_c,
                                g_DOOR_STATE_ATTRIBUTE_c,
                                (uint8_t*)&doorState,
                                g_Write_Attribute_Data_c );
		status = g_ZCL_Success_c;
	
	}

        pBuffer->a_asdu[3] = status;
		asduLen=m_DOOR_LOCKS_CMD_RSP_LENGTH_c;

    App_SendData( pBuffer->src_addr_mode,
         pBuffer->src_address.IEEE_address,
         pBuffer->src_endpoint,
         g_DOOR_LOCK_CLUSTER_c,
         asduLen,
         pBuffer->a_asdu );
}
/*******************************************************************************/

static void App_CreatAndSend_unLock_Door_Response( uint8_t *unLockDoorResp,
                                              APSDE_Data_Indication_t *pBuffer )
{
	uint8_t	asduLen;
	uint8_t status;
	
	if( unLockDoorResp == g_NULL_c || pBuffer == g_NULL_c)
	{
	  status = g_ZCL_Failure_c;
	}

    /* ZCL Header Creation */
    pBuffer->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c;
    pBuffer->a_asdu[1] = App_GetTransSeqNumber();
    pBuffer->a_asdu[2] = g_Unlock_Door_Response_c;

    App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DOOR_LOCK_CLUSTER_c,
                                g_LOCK_STATE_ATTRIBUTE_c,
                                (uint8_t*)&lockState,
                                g_Read_Attribute_Data_c );
	
	App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DOOR_LOCK_CLUSTER_c,
                                g_DOOR_STATE_ATTRIBUTE_c,
                                (uint8_t*)&doorState,
                                g_Read_Attribute_Data_c );  	
	
	
	if( (doorState == Error) || (lockState == Not_Fully_Locked)){
		status = g_ZCL_Failure_c;
	}
	else if ( (lockState == Unlocked )|| (doorState == Open)){
	  status = g_ZCL_Duplicate_Exists_c;
	}
	else {
		lockState =Unlocked;
		doorState = Open;
	
		App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DOOR_LOCK_CLUSTER_c,
                                g_LOCK_STATE_ATTRIBUTE_c,
                                (uint8_t*)&lockState,
                                g_Write_Attribute_Data_c );
		
		App_ReadWriteAttribute( pBuffer->dest_endpoint,
                                g_DOOR_LOCK_CLUSTER_c,
                                g_DOOR_STATE_ATTRIBUTE_c,
                                (uint8_t*)&doorState,
                                g_Write_Attribute_Data_c );
		status = g_ZCL_Success_c;
	
	}

        pBuffer->a_asdu[3] = status;
		asduLen=m_DOOR_LOCKS_CMD_RSP_LENGTH_c;

    App_SendData( pBuffer->src_addr_mode,
         pBuffer->src_address.IEEE_address,
         pBuffer->src_endpoint,
         g_DOOR_LOCK_CLUSTER_c,
         asduLen,
         pBuffer->a_asdu );

}

#endif /*g_DOOR_LOCK_SERVER_CLUSTER_ENABLE_d */
/*******************************************************************************/

#endif   /* ( g_DOOR_LOCK_CLUSTER_ENABLE_d ==1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/