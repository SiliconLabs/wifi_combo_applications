/******************************************************************************
* ZCL_HA_Appliance_Events_And_Alert_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Appliance Events and Alerts Cluster's attribute and
*   command information
******************************************************************************/

#ifndef _ZCL_HA_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_H_
#define _ZCL_HA_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/******************************************************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Appliance events and alert Cluster */
#define g_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_c                  0x0B02

/*  Appliance events and alert Cluster's attribute  */
    /* None */

/* Appliance Events and Alert Cluster's Server Command*/
enum applianceEventsAndAlertServerCommands
{
    g_GetAlertsResponse_c = 0x00,
    g_AlertsNotificationCommand_c,
    g_EventNotificationCommand_c

};

/*Appliance Events and Alert Cluster's Client Command*/
enum applianceEventsAndAlertClientCommands
{
    g_GetAlertsCommand_c = 0x00
};

/* enum for Appliance Events and Alert Cluster's Events Identifiers*/
enum
{
    g_GetAlertsResponseEvent_c = 0x20,
    g_AlertsNotificationEvent_c,
    g_EventNotification_c,
    g_GetAlertsEvent_c
};

/******************************************************************************
* Memory Declarations
******************************************************************************/

#if ( g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Appliance_Events_And_Alert_Client_Cluster_Info;
#endif

#if ( g_EN50523_APPLIANCE_EVENTS_AND_ALERTS_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Appliance_Events_And_Alert_Server_Cluster_Info;
#endif

/******************************************************************************
* Type Definitions
******************************************************************************/

/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_HA_Appliance_Events_And_Alert_Cluster_Cmd
                                        (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                        /*_ZCL_HA_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_H_*/