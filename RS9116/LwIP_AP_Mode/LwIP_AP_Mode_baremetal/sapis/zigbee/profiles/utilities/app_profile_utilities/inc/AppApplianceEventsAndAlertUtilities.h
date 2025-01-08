 /******************************************************************************
* AppApplianceEventsAndAlertUtilities..h
******************************************************************************
* Program Description:
* This file contains the Appliance Events and Alert Cluster App type definitions
******************************************************************************/
#ifndef _APP_APPLIANCE_EVENTS_AND_ALERT_UTILITIES_H_
#define _APP_APPLIANCE_EVENTS_AND_ALERT_UTILITIES_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_HA_Appliance_Events_And_Alert_Cluster.h"

/******************************************************************************
* Defines
******************************************************************************/
#define g_BITMASK_FOR_NO_OF_ALERTS_c                            (0x0f)
#define g_BITMASK_FOR_TYPE_OF_ALERT_c                           (0xf0)
#define g_BITMASK_FOR_ALERT_ID_c                                (0xff)
#define g_BITMASK_FOR_ALERT_CATEGORY_c                          (0x0f)
#define g_BITMASK_FOR_ALERT_PRESENCE_OR_RECOVERY_c              (0x30)
#define g_ALERT_LIST_SIZE_c                                     (5)

/******************************************************************************
* Global Constants
******************************************************************************/

/* Member of Alert Structure */
typedef enum Alert_Category
{
    g_ALERT_WARNING_c = 0x01,
    g_ALERT_DANGER_c  = 0x02,
    g_ALERT_FAILURE_c = 0x03
}Alert_Category;

/* member of alert structure */
typedef enum Alert_Presence_Recovery
{
    g_PRESENCE_c = 0x00,
    g_RECOVERY_c = 0x01
}Alert_Presence_Recovery;


/* Type of Alert*/
typedef enum Alert_Type
{
    g_UNSTRUCTURED_c = 0x00
}Alert_Type;


/* Event Identification */
typedef enum Event_Identification
{
    /* End of working cycle reached */
    g_END_OF_CYCLE_c = 0x01,
    /* Set Temperature Reached */
    g_TEMPERATURE_REACHED_c = 0x04,
    /* End of Cooking Process */
    g_END_OF_COOKING_c = 0x05,
    /* Switching Off */
    g_SWITCHING_OFF_c = 0x06,
    /* Wrong Data */
    g_WRONG_DATA_c = 0xfc

}Event_Identification;


typedef struct Alert_Structure_Tag
{
    uint8_t alertId;
    uint8_t categoryAndPresenceRecovery;
    uint8_t reserved;
}__attribute__((__packed__))Alert_Structure_t;


/* Alert Response Payload */
/* Get Alerts Response AND Alert Notification command payload share
same payload */
typedef struct Alert_Response_Tag{
    uint8_t alertcount;
    uint8_t alertStructure[1];
}__attribute__((__packed__))Get_Alerts_Response_t, Alert_Notification_t;

/******************************************************************************
* Memory Declarations
******************************************************************************/



/******************************************************************************
* Type Definitions
******************************************************************************/

/******************************************************************************
   Function Prototypes
******************************************************************************/

void App_ApplianceEventsAndAlertHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer );


void SendApplianceEventsAndAlertResponse(APSDE_Data_Indication_t *apsdeDataPacket
                                         ,uint8_t responseType,
    uint8_t status,uint8_t *pAsdu, uint8_t asduPayloadLength,
    uint8_t bufferToBeFree );

void StoreAlertIntoAlertsTable(void);
void InvokeAlertNotificationCommand(void);
void InvokeEventNotificationCommand(void);
void App_ResetEventsAndAlert(void);
/******************************************************************************
* End Of File
******************************************************************************/
#endif          /* _APP_APPLIANCE_EVENTS_AND_ALERT_UTILITIES_H_ */
