/******************************************************************************
* ZCL_General_Power_Profile_Cluster.h
******************************************************************************
* Program Description:
* This file contains the power profile attribute and command
* information
******************************************************************************/

#ifndef _ZCL_GENERAL_POWER_PROFILE_CLUSTER_H_
#define _ZCL_GENERAL_POWER_PROFILE_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/******************************************************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Power Profile Cluster */
#define g_POWER_PROFILE_CLUSTER_c                               0x001A

/* About Attributes of this cluster */
    /* Represents total number of profiles supported by the device*/
#define g_TOTAL_PROFILE_NUMBER_ATTRIBUTE_c                      0x0000
    /* Server supports Multiple energy phases supported  or not at a time */
#define g_MULTIPLE_SCHEDULE_ATTRIBUTE_c                         0x0001
    /* provides a method to decipher the no of digits and decimal location*/
#define g_ENERGY_FORMATTING_ATTRIBUTE_c                         0x0002
    /* Remote energy management enabled/disabled*/
#define g_ENERGY_REMOTE_ATTRIBUTE_c                             0x0003
    /* Describes the criteria used to schedule the power profile*/
#define g_SCHEDULE_MODE_ATTRIBUTE_c                             0x0004


/* Commands received by the server */

enum powerProfileClientCommands
{
    g_PowerProfileRequestCommand_c = 0x00,
    g_PowerProfileStateRequestCommand_c,
    g_GetPowerProfilePriceResponseCommand_c,
    g_GetOverallSchedulePriceResponseCommand_c,
    g_EnergyPhasesScheduleNotificationCommand_c,
    g_EnergyPhasesScheduleResponseCommand_c,
    g_PowerProfileScheduleConstraintsRequestCommand_c,
    g_EnergyPhasesScheduleStateRequestCommand_c,
    g_GetPowerProfilePriceExtendedResponseCommand_c
};

/* Commands received by the Client */
enum powerProfileServerCommands
{
    g_PowerProfileNotificationCommand_c = 0x00,
    g_PowerProfileResponseCommand_c,
    g_PowerProfileStateResponseCommand_c,
    g_PowerProfilePriceCommand_c,
    g_PowerProfilesStateNotificationCommand_c,
    g_GetOverallSchedulePriceCommand_c,
    g_EnergyPhasesScheduleRequestCommand_c,
    g_EnergyPhasesScheduleStateResponseCommand_c,
    g_EnergyPhasesScheduleStateNotificationCommand_c,
    g_PowerProfileScheduleConstraintsNotificationCommand_c,
    g_PowerProfileScheduleConstraintsResponseCommand_c,
    g_PowerProfilePriceExtendedCommand_c
};

/* enum for POWER PROFILE cluster event identifier */
enum powerProfileClusterEvents
{
    g_PowerProfileRequestEvent_c = 0xbd,
    g_PowerProfileStateRequestEvent_c,
    g_GetPowerProfilePriceResponseEvent_c,
    g_GetOverallSchedulePriceResponseEvent_c,
    g_EnergyPhasesScheduleNotificationEvent_c,
    g_EnergyPhasesScheduleResponseEvent_c,
    g_PowerProfileScheduleConstraintsRequestEvent_c,
    g_EnergyPhasesScheduleStateRequestEvent_c,
    g_GetPowerProfilePriceExtendedResponseEvent_c,
    g_PowerProfileNotificationEvent_c,
    g_PowerProfileResponseEvent_c,
    g_PowerProfileStateResponseEvent_c,
    g_PowerProfilePriceEvent_c,
    g_PowerProfilesStateNotificationEvent_c,
    g_GetOverallSchedulePriceEvent_c,
    g_EnergyPhasesScheduleRequestEvent_c,
    g_EnergyPhasesScheduleStateResponseEvent_c,
    g_EnergyPhasesScheduleStateNotificationEvent_c,
    g_PowerProfileScheduleConstraintsNotificationEvent_c,
    g_PowerProfileScheduleConstraintsResponseEvent_c,
    g_PowerProfilePriceExtendedEvent_c
};



/******************************************************************************
* Memory Declarations
******************************************************************************/
#if ( g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HA_Power_Profile_Server_Cluster_Info;
#endif

#if ( g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t
    ga_HA_Power_Proile_Client_Cluster_Info;
#endif



/* Power Profile Cluster Attribute Structure*/

typedef struct Power_Profile_Cluster_Tag{
    uint8_t totalProfileNumber;
    uint8_t multipleScheduling;
    uint8_t energyFormatting;
    uint8_t energyRemote;
    uint8_t scheduleMode;
} __attribute__((__packed__))Power_Profile_Cluster_t;
/******************************************************************************
* Type Definitions
******************************************************************************/
/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_HA_Power_Profile_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                          /*_ZCL_GENERAL_POWER_PROFILE_CLUSTER_H_*/
