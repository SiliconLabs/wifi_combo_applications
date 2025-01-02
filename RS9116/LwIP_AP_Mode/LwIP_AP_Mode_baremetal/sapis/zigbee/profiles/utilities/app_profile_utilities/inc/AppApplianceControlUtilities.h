/******************************************************************************
* AppApplianceControlUtilities..h
******************************************************************************
* Program Description:
* This file contains the Appliance Control Cluster App type definitions
******************************************************************************/
#ifndef _APP_APPLIANCE_CONTROL_UTILITIES_H_
#define _APP_APPLIANCE_CONTROL_UTILITIES_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_General_Appliance_Control_Cluster.h"


/******************************************************************************
* Global Constants
******************************************************************************/

/******************************************************************************
* Memory Declarations
******************************************************************************/

/* Household Appliance Current status value*/
typedef enum Appliance_Status
{
    /* Appliance in Off */
    g_OFF_STATE_c = 0x01,
    /* Appliance in Stand-By*/
    g_STAND_BY_STATE_c,
    /* Appliance already programmed */
    g_PROGRAMMED_STATE_c,
    /* Appliance already programmed and ready to start*/
    g_PROGRAMMED_WAITING_TO_START_STATE_c,
    /* Appliance is Running */
    g_RUNNING_STATE_c,
    /* Appliance is in Pause */
    g_PAUSE_STATE_c,
    /* Appliance end programmed tasks*/
    g_END_PROGRAMMED_STATE_c,
    /* Appliance is in a failure state */
    g_FAILURE_STATE_c,
    /* The Appliance Programmed tasks have been interrupted*/
    g_PROGRAMME_INTERRUPTED_STATE_c,
    /* Appliance in Idle State*/
    g_IDLE_STATE_c,
    /* Appliance in Rinse Hold */
    g_RINSE_HOLD_STATE_c,
    /* Appliance in Service State */
    g_SERVICE_STATE_c,
    /* Appliance in Superfreezing state */
    g_SUPERFREEZING_STATE_c,
    /* Appliance in SuperCooling State */
    g_SUPERCOOLING_STATE_c,
    /* Appliance in SuperHeating State */
    g_SUPERHEATING_STATE_c

}Appliance_Status;


typedef enum Remote_Enable_Flags
{
    /* Bits 0 to 3 are Remote Enable Flags*/
    /* Note : Rest values for these bits are reserved */

    /* Remote Disabled */
    g_REMOTE_DISABLED_c = 0x00,
    /* Remote Temporarily Disabled */
    g_REMOTE_TEMPORARILY_LOCKED_c = 0x07,
    /* Enable Remote Control */
    g_ENABLED_REMOTE_CONTROL_AND_ENERGY_CONTROL_c = 0x01,

    /* Device Status 2 */
    /* Bits Range 4...7 */
    /* Note : Rest values for these bits are reserved */

    g_PROPRIETARY_c = 0x10,
    g_IRIS_SYMPTOM_CODE_c = 0x20


}Remote_Enable_Flags;


typedef enum Execution_Command_Id
{
    /* Start Appliance Cycle */
    g_START_c = 0x01,
    /* Stop Appliance Cycle */
    g_STOP_c = 0x02,
    /* Pause Appliance Cycle */
    g_PAUSE_c,
    /* Start Superfreezing Cycle */
    g_START_SUPERFREEZING_c,
    /* Stop Superfreezing Cycle */
    g_STOP_SUPERFREEZING_c,
    /* Start SuperCooling Cycle */
    g_START_SUPERCOOLING_c,
    /* Stop SuperCooling Cycle */
    g_STOP_SUPERCOOLING_c,
    /* Disable Gas */
    g_DISABLE_GAS_c,
    /* Enable Gas */
    g_ENABLE_GAS_c

}Execution_Command_Id;





/******************************************************************************
* Type Definitions
******************************************************************************/

/******************************************************************************
   Function Prototypes
******************************************************************************/

void App_ApplianceControlEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer );

void SendApplianceControlStatus(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status ,uint8_t* pRes,uint8_t responseLength);

#if( g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1)
void App_ApplianceActivitySimulated(void );
/* reset */
void App_ApplianceControlReset(void);
#endif /* g_EN50523_APPLIANCE_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 */
/******************************************************************************
* End Of File
******************************************************************************/
#endif          /* _APP_APPLIANCE_CONTROL_UTILITIES_H_ */