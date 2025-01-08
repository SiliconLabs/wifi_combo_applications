 /******************************************************************************
* AppPowerProfileUtilities..h
******************************************************************************
* Program Description:
* This file contains the Power Profile Cluster App type definitions
******************************************************************************/
#ifndef _APP_POWER_PROFILE_UTILITIES_H_
#define _APP_POWER_PROFILE_UTILITIES_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_General_Power_Profile_Cluster.h"

/******************************************************************************
* Defines
******************************************************************************/
/* Following value is application dependent */
#define g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c                 (3)



/******************************************************************************
* Global Constants
******************************************************************************/


/******************************************************************************
* Memory Declarations
******************************************************************************/

typedef struct ProfileRequestCommandPayload_Tag {
    uint8_t powerProfileId;
}__attribute__((__packed__))ProfileRequestCommandPayload_t;

typedef struct PowerProfilePriceResponsePayload_Tag{

    uint8_t powerProfileId;

    uint8_t currency[2];

    uint8_t price[4];

    uint8_t priceTrailingDigit;
}__attribute__((__packed__))PowerProfilePriceResponsePayload_t;

typedef struct OverallSchedulePriceResponsePayload_Tag{
    uint8_t currency[2];
    uint8_t price[4];
    uint8_t priceTrailingDigit;
}__attribute__((__packed__))OverallSchedulePriceResponsePayload_t;




typedef struct EnergyPhasesScheduleNotificationPayload_Tag{
    uint8_t powerProfileId;
    uint8_t NoOfScheduledPhases;
    /* Each Energy phase has PhaseId(1) ScheduledTime(2)*/
    uint8_t energyPhaseId[1];
    uint8_t scheduledTime[2];
}__attribute__((__packed__))EnergyPhasesScheduleNotificationPayload_t;


typedef struct AboutEachEnergyPhase_Tag{
    /* Identifier for energy phase ,range 1 to max no of phases*/
    uint8_t energyPhaseId;
    /* application dependent */
    uint8_t macroPhaseId;
    /* estimated duration(min) for energy phase*/
    uint8_t expectedDuration[2];
    /* estimated power for specific phase (in watts)*/
    uint8_t peakPower[2];
    /* energy Energy <= PeakPower(Watt) * ExpectedDuration(sec).*/
    uint8_t energy[2];
    /* max delay b/w start of next phase to end of previous phase*/
    uint8_t maxActivationDelay[2];
}__attribute__((__packed__))AboutEachEnergyPhase_t;


typedef struct AboutProfileEnergyPhaseClient_Tag{
    /* Identifier for energy phase ,range 1 to max no of phases*/
    uint8_t energyPhaseId;
   /* max delay b/w start of next phase to end of previous phase*/
    uint8_t maxActivationDelay[2];
    /* estimated duration(min) for energy phase*/
    uint8_t expectedDuration[2];
    /* This energy phase is scheduled at the following time,it is cleared once
    profile ends*/
    uint8_t scheduledAt[2];
}__attribute__((__packed__))AboutProfileEnergyPhaseClient_t;

typedef struct PowerProfileNotificationPayload_Tag {
    /* Total no of profiles supported by the device */
    uint8_t totalProfileNumber;
    /* Identifier for the specific profile Id*/
    uint8_t powerProfileId;
    /* No of phases transfered*/
    uint8_t noOfTransferredPhases;
    /* Each Energy phase has EnergyPhaseId(1) ,MacroPhaseId(1), ExpectedDuration(2),
    PeakPower(2),Energy(2),MaxActivationDelay(2)*/
    uint8_t aboutEnergyPhaseWithPeakPower[1];
}__attribute__((__packed__))PowerProfileNotificationPayload_t;



typedef struct PowerProfileStateResponsePayload_Tag{
    /* no of profile records that follow in the message */
    uint8_t powerProfileCount;
    /*Power profile record Holder */
    uint8_t powerProfileRecord[1];
}__attribute__((__packed__))PowerProfileStateResponsePayload_t;


typedef struct PowerProfileScheduleConstraints_Tag{
    /* Active Profile Id*/
    uint8_t profileId;
    /* Constraints set by the user to start after the specified time*/
    uint8_t startAfter[2];
    /* Constraints set by the user to stop before the specified time */
    uint8_t stopBefore[2];
}__attribute__((__packed__))PowerProfileScheduleConstraints_t;


/* Several States of the Power Profile */
typedef enum{
    /* The PP is not defined in its parameter*/
    g_POWER_PROFILE_IDLE_c = 0x00,
    /* The PP is defined in its parameter but without a
    scheduled time reference*/
    g_POWER_PROFILE_PROGRAMMED_c = 0x01,

    /* Note: 0x02 is not defined*/

    /* Energy Phase Running*/
    g_ENERGY_PHASE_RUNNING_c = 0x03,
    /* Energy Phase Paused*/
    g_ENERGY_PHASE_PAUSED_c = 0x04 ,
    /* The Power Profile is in between two energy phases (one ended,
    the other not yet started). If the first Energy Phase is considered,
    this state indicates that the whole power profile is not yet started,
    but it has been already programmed to start*/
    g_ENERGY_PHASE_WAITING_TO_START_c = 0x05,
    /* The Power Profile is set to Pause when being in the
    ENERGY_PHASE_WAITING_TO_START state.*/
    g_ENERGY_PHASE_WAITING_PAUSED_c = 0x06,
    /* The whole Power Profile is terminated.*/
    g_POWER_PROFILE_ENDED_c = 0x07
}PowerProfileState;

typedef struct PowerProfilesWithState_Tag{
    /* Active Profile Id*/
    uint8_t profileId;
    /* Profile State*/
    uint8_t profileState; // PowerProfileState is an enum
   /* Active Energy Phase Id*/
    uint8_t energyPhaseId;
}__attribute__((__packed__))PowerProfilesWithState_t;

typedef struct PowerProfileRecord_Tag{
    /* Active Profile Id*/
    uint8_t powerProfileId;
    /* Active Energy Phase Id*/
    uint8_t energyPhaseId;
    /* it indicates if the PowerProfile is currently remotely controllable
    or not*/
    uint8_t powerProfileRemoteControl;
    /* Profile State*/
    uint8_t profileState;
}__attribute__((__packed__))PowerProfileRecord_t;

typedef struct ActiveProfilesMaintainedByClient_Tag{
    PowerProfilePriceResponsePayload_t profilePriceInfo;
    /* This field give priviledge to the user to set profile price even when
    profile is not active */
    bool active;
    /*This field is updated only when schedule constraints notification received
    from server, or throught schedule constraints response from server for the
    req sent */
    uint8_t startTimeMaintainedByClient[2];
    /*This field is updated only when schedule constraints notification received
    from server, or throught schedule constraints response from server for the
    req sent */
    uint8_t stopTimeMaintainedByClient[2];
    /* Profile State is maintained @ client side ,
    This parameter helps in scheduling,Schedule can take place only if profile
    state is not running and its only idle or programmed state */
    uint8_t profileState; //PowerProfileState is an enum
}__attribute__((__packed__))ActiveProfilesMaintainedByClient_t;

typedef struct GetPowerProfilePriceExtended_Tag{
    /* type of request*/
    uint8_t options;
    /* profile id requested*/
    uint8_t profileId;
    /* represents the relative time when overall power profile can start*/
    uint8_t powerProfileStartTime[2];
}__attribute__((__packed__))GetPowerProfilePriceExtended_t;

/* This cluster is maintain to update the information form client*/
typedef struct UpdatesReceivedFromClient_Tag{
    uint8_t profileId;
    uint8_t enregyPhaseId;
    uint8_t scheduledStartTime[2];
}__attribute__((__packed__))UpdatesReceivedFromClient_t;

typedef struct UpdateOfEnergyPhase_Tag{
    uint8_t energyPhaseId;
    uint8_t scheduledTime[2];
}__attribute__((__packed__))UpdateOfEnergyPhase_t;

/* EnergyPhase detail maintained by the server*/
typedef struct UpdatesMaintainedByServer_Tag{
    uint8_t profileId;
    /* scheduled field decides whether profile shd start its on meeting the
    start time constraints or it shd follow the schedule given by the
    scheduler(client)*/
    bool scheduled;
    /* currentTime Param is used by timer to change the state */
    uint8_t currentTime[2];
    /* max of three energyphase*/
    UpdateOfEnergyPhase_t energyPhaseStartTime[g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c];
}__attribute__((__packed__))UpdatesMaintainedByServer_t;


typedef struct EnergyPhasesScheduleStateResponse_Tag{
    uint8_t profileId;
    /* scheduled field decides whether profile shd start its on meeting the
    start time constraints or it shd follow the schedule given by the
    scheduler(client)*/
    uint8_t numOfScheduledPhases;
    /* max of three energyphase*/
    UpdateOfEnergyPhase_t energyPhase[g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c];
}__attribute__((__packed__))EnergyPhasesScheduleStateResponse_t;


#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
extern Power_Profile_Cluster_t power_profile_cluster_values;
extern const Power_Profile_Cluster_t power_profile_cluster_default_values;
#endif /*  g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d */
/******************************************************************************
* Type Definitions
******************************************************************************/

/******************************************************************************
   Function Prototypes
******************************************************************************/

void App_PowerProfileClusterEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer );


void App_PowerProfileReset(void);
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
void InvokeTransmistOfProfileNotification();
void InvokeTransmitOfProfileStateNotification();
void InvokeTransmitOfPhasesScheduleStateNotification();
void InvokeTransmitScheduleConstraintsNotification(uint8_t profileId,
                                                   uint8_t *pStartAndStopTime,
                                                   uint8_t length);
void App_UpdateServerValues(void);
#endif

#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
void InvokeTransmistEnergyPhasesSchNotification();
void SetCostOfProfile(uint8_t *pCostInfo,uint8_t length );
void createAndSendPriceResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t profileId, bool serverToClient,uint8_t*pStartTime);
void createAndSendOverAllSchPriceResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType, bool serverToClient);
void App_UpdateClientValues(void);
#endif

/******************************************************************************
* End Of File
******************************************************************************/
#endif          /* _APP_POWER_PROFILE_UTILITIES_H_ */
