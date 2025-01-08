/*******************************************************************************
* AppPowerProfileUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Power Profile
*   Cluster
*
*   Description about this cluster
*   ==============================
*       This cluster is about managing power of house hold appliance.
*   Server                  Client
*   (ex: White Goods )      (ex: Home gateway)
*
*   ->  How it is done?
*       It is done through maintaining profiles,Profile contains about energy
*   estimation for particular duration.Profile can be with multiple
*   energy phases or single energy phase.
*
*   -> Power profiles are maintained in server , but it is controlled by
*       client(Home gateway)
*   -> Power profile req can be solicited by Client Or even server can send
*       notification.
*
*   -> Each profile has got its own cost
*           cost is represented as Currency and price ex: Rs 100.50
*   -> Scheduler is Client ( ex: Home Gateway) : Maintains schedule for each
*           profile
*
*   Key words
*   ----------
*   a.ExpectedDuration(minutes) :Estimated duration of specific phase.
*   b.PeakPower: Can be considered as MAX peak power for the specific phase.
*   c.Energy : Energy <= (PeakPower * ExpectedDuration)
*   d.Multiple energy phases
*    Each profile may or may not support multiple energy phases
*                   delay                    delay
*    energyPhase(1)---------> energyphase(2)-------->.... energyphase(n)
*   e.expectedDuration : Time estimated for the completion of the phase.
*   f.PowerProfileRemoteControl: It indicates whether profile is controlled via
*    remote device (ex: Home gateway)
*
*   There are different states of the Profile.
*   ------------------------------------------
*   State 0 : Power Profile Idle
*   State 1 : Power Profile Programmed
*   //State 2 :resered
*   State 3 : Energy Phase Running
*   State 4 : Energy Phase Paused
*   State 5 : Energy Phase Waiting to start
*   State 6 : Energy Phase Waiting Paused
*   State 7 : Power Profile Ended
*
*   A Schedule for when profile should start is maintained by Client device
*   ->Inorder to start profile, Server device should 1st know when to start,
*   Server will come to know by using EnergyPhasesScheduleReq(ProfileID)
*   Client will send the schedule for server EnergyPhasesScheduleResponse,
*   Then profile state will also change state to waiting_to_start state or
*   running_state(if immediate no delay set) on proper schedule .
*
*   Note:
*   =====
*       But the Constraints for starting whole profile is given by Server
*   through PowerProfileScheduleConstraintsNotification
*   (its payload is : profileId ,StartAfter,StopBefore) using which client will
*   create a schedule for energy phases.
*   Profile schedule be such that profile shd start after StartAfter value
*   Profile shd end before time specified in (StopBefore).

*******************************************************************************/

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include <stdlib.h>
#include <string.h>
#include "stack_common.h"
//#include "nvm.h"
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
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

#if(g_POWER_PROFILE_CLUSTER_ENABLE_d == 1)
    #include "ZCL_General_Power_Profile_Cluster.h"
    #include "AppPowerProfileUtilities.h"
    #include "ApplicationThinLayer.h"
#endif /* (g_POWER_PROFILE_CLUSTER_ENABLE_d == 1)*/



/*******************************************************************************
* Global Constants
*******************************************************************************/
/* Following value is application dependent */
#define g_MAXIMUM_NO_OF_PROFILES_c                                  (5)
/* Invalid Identifier for Power Profile */
#define g_INVALID_POWER_PROFILE_ID_c                                (0xff)
/* Invalid Identifier for EnergyPhase  */
#define g_INVALID_ENERGY_PHASE_ID_c                                 (0xff)
/* All Active Power Profiles*/
#define g_ALL_ACTIVE_POWER_PROFILES_c                               (0x00)
/* Parameter is used to initialize the start and stop contraints
defined by user*/
#define g_DURATION_FIELD_SIZE_c                                     (0x02)
#define g_24_HOURS_IN_MINUTES_c                                     (0x05A0)
#define g_ONE_MINUTE_c                                              (0x01)
/* Bit map field indicates next field (startTime) present or not*/
#define g_POWER_PROFILE_STARTTIME_FIELD_PRESENT_c                   (0x01)

#define g_ONE_SECOND_c                                              (0x1770)

#define g_ONE_MINUTE_VALUE_c                                        (0x057e40 )
/*******************************************************************************
* Public Memory declarations
*******************************************************************************/

#if ( DEBUG_TEST == 1 )
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

typedef struct CountTracker_Tag {
    uint8_t profileNo;
    uint8_t energyPhaseCount;
}__attribute__((__packed__))CountTracker_t;

/* Default Profile ID's*/
uint8_t defaultProfileIdArray[g_MAXIMUM_NO_OF_PROFILES_c] =
    {0x00,0x01,0x02,0x03,0x04};

/* Maintains list of all active profiles */
uint8_t ActiveProfiles[g_MAXIMUM_NO_OF_PROFILES_c] =
    {0x01,0x02,0x03,0xff,0xff};


/* following table maintains a timing constraints set by the user to
schedule the profile*/
PowerProfileScheduleConstraints_t ActiveProfilesWithTimeConstraints
[g_MAXIMUM_NO_OF_PROFILES_c] = {
    {0x01,{0x00,0x00},{0x00,0x00}},
    {0x02,{0x00,0x00},{0x00,0x00}},
    {0x03,{0x00,0x00},{0x00,0x00}},
    {0x04,{0x00,0x00},{0x00,0x00}},
    {0x05,{0x00,0x00},{0x00,0x00}}
};


/* Profiles With their State and current energyPhase which they are in */
PowerProfilesWithState_t ProfilesWithStateAndPhase[g_MAXIMUM_NO_OF_PROFILES_c] ={
    {0x01,g_POWER_PROFILE_IDLE_c,0x01},
    {0x02,g_POWER_PROFILE_IDLE_c,0x01},
    {0x03,g_POWER_PROFILE_IDLE_c,0x01},
    {0xff,g_POWER_PROFILE_IDLE_c,0xff},
    {0xff,g_POWER_PROFILE_IDLE_c,0xff}
};

/* This variable keeps track of active profiles count */
uint8_t ActiveProfilesCount = 0x00;


/* LookUp Table to know active energy phases count in each profile */
/* each instance = profileId , ActiveEnergyPhasesCount*/
uint8_t LookUpTableForEneryPhasesCount[g_MAXIMUM_NO_OF_PROFILES_c][0x01] = {
    {0x03},
    {0x03},
    {0x03},
    {0xff},
    {0xff}
};

uint8_t ActiveEnergyPhasesWithActiveProfiles [g_MAXIMUM_NO_OF_PROFILES_c]
    [g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c] [sizeof(AboutEachEnergyPhase_t)]= {
    /* 1st profile with max energy phases */
    {
        {0x01,0x01,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02} ,
        {0x02,0x02,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02} ,
        {0x03,0x03,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02}
    },
    /* 2nd profile with max energy phases */
    {
        {0x01,0x01,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 },
        {0x02,0x02,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 },
        {0x03,0x03,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 }
    },
    /* 3rd profile with max energy phases */
    {
        {0x01,0x01,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 },
        {0x02,0x02,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 },
        {0x03,0x03,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 }
    },
    /* 4th profile with max energy phases */
    {
        {0x01,0x01,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 },
        {0x02,0x02,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 },
        {0x03,0x03,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 }
    },
    /* 5th profile with max energy phases */
    {
        {0x01,0x01,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 },
        {0x02,0x02,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 },
        {0x03,0x03,0x00,0x05,0x00,0x02,0x00,0x0a,0x00,0x02 }
    }
};

uint8_t ActiveEnergyPhasesWithActiveProfilesbck [g_MAXIMUM_NO_OF_PROFILES_c]
    [g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c] [sizeof(AboutEachEnergyPhase_t)];

#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
    uint8_t ResponseLen = 0x00,TempBuffer[80];
    /* This array of stucture maintains details about currently programmed or
    running profiles details,This array is updated after schedule is received
    from scheduler(client) device.*/
    UpdatesMaintainedByServer_t serverUpdate[g_MAXIMUM_NO_OF_PROFILES_c] ={
        {0x01,g_FALSE_c,{0x00,0x00},{{0x01,{0x00,0x00}},{0x02,{0x00,0x00}},{0x03,{0x00,0x00}}}},
        {0x02,g_FALSE_c,{0x00,0x00},{{0x01,{0x00,0x00}},{0x02,{0x00,0x00}},{0x03,{0x00,0x00}}}},
        {0x03,g_FALSE_c,{0x00,0x00},{{0x01,{0x00,0x00}},{0x02,{0x00,0x00}},{0x03,{0x00,0x00}}}},
        {0xff,g_FALSE_c,{0x00,0x00},{{0x01,{0x00,0x00}},{0x02,{0x00,0x00}},{0x03,{0x00,0x00}}}},
        {0xff,g_FALSE_c,{0x00,0x00},{{0x01,{0x00,0x00}},{0x02,{0x00,0x00}},{0x03,{0x00,0x00}}}}
    };
    uint32_t PreviousCurrentTime[g_MAXIMUM_NO_OF_PROFILES_c]={
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000
    };


#endif

#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
ActiveProfilesMaintainedByClient_t ProfileWithCost[g_MAXIMUM_NO_OF_PROFILES_c] ={

    { {0xff,{0xff,0xff},{0xff,0xff,0xff,0xff},0xff},g_FALSE_c,{0x00,0x00},{0xff,0xff},g_POWER_PROFILE_IDLE_c},
    { {0xff,{0xff,0xff},{0xff,0xff,0xff,0xff},0xff},g_FALSE_c,{0x00,0x00},{0xff,0xff},g_POWER_PROFILE_IDLE_c},
    { {0xff,{0xff,0xff},{0xff,0xff,0xff,0xff},0xff},g_FALSE_c,{0x00,0x00},{0xff,0xff},g_POWER_PROFILE_IDLE_c},
    { {0xff,{0xff,0xff},{0xff,0xff,0xff,0xff},0xff},g_FALSE_c,{0x00,0x00},{0xff,0xff},g_POWER_PROFILE_IDLE_c},
    { {0xff,{0xff,0xff},{0xff,0xff,0xff,0xff},0xff},g_FALSE_c,{0x00,0x00},{0xff,0xff},g_POWER_PROFILE_IDLE_c}
};

uint8_t ActiveEnrPhasesWithActiveProfilesCli [g_MAXIMUM_NO_OF_PROFILES_c]
    [g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c] [sizeof(AboutProfileEnergyPhaseClient_t)]= {
    /* 1st profile with max energy phases */
        {
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff}
    },
    /* 2nd profile with max energy phases */
    {
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff}
    },
    /* 3rd profile with max energy phases */
    {
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff}
    },
    /* 4th profile with max energy phases */
    {
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff}
    },
    /* 5th profile with max energy phases */
    {
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff} ,
        {0xff,0xff,0xff,0xff,0xff,0xff,0xff}
    },
};

uint8_t CostArrayTracker = 0x00;
uint32_t Overallcost;
uint32_t price[g_MAXIMUM_NO_OF_PROFILES_c] = { 0x00000000,
                                                0x00000000,
                                                0x00000000,
                                                0x00000000,
                                                0x00000000};

/* This Both Attributes are updated in client through ZCL read response or
   through reporting*/
static uint8_t MultipleSchAttrMaintainedByClient = g_FALSE_c;
static uint8_t RemoteControlAttrMaintainedByClient = g_FALSE_c;

static uint8_t profileCount = 0x00;
uint8_t ArrayTracker = 0x00;

#endif
/*******************************************************************************
* Private Memory declarations
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
static bool lookForRequestedProfile(uint8_t profileId);
static void createAndSendProfileResponse(APSDE_Data_Indication_t *apsdeDataPacket,
                                  uint8_t profileId,uint8_t responseType );
static bool timeConstraintsValid(uint8_t profile_id,uint8_t *pDuration,uint8_t length);
static void createAndSendStateResponse(APSDE_Data_Indication_t *apsdeDataPacket,
                                  uint8_t profileId,uint8_t responseType );

static void createAndSendScheduledStateResponse(APSDE_Data_Indication_t *apsdeDataPacket,
                                  uint8_t profileId,uint8_t responseType );
#endif

#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
static uint32_t calculateAverageCost(uint8_t loopIndexToPrice);
static void createAndSendScheduleToServer(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t profileId, bool serverToClient);
static void updateTheSchedule(uint8_t profileId, AboutProfileEnergyPhaseClient_t
                              pEnergyPhaseList[],bool singleResponse,
                              uint8_t energyPhaseCount);
#endif

void SendPowerProfileResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status ,uint8_t* pRes,uint8_t responseLength,
    bool serverToClient);

#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
Power_Profile_Cluster_t power_profile_cluster_values;//remove this on test harness integration//added
#endif
/*******************************************************************************
* Public Functions
*******************************************************************************/
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
void App_UpdateServerValues(void)
{
    uint8_t count = 0x00;

    /* Note :Maintaining profile No and its content is
    application dependent,to simulate an example count is maintained as 3
    or less than 3 ,3 profiles details are initialized.*/

    /* COUNT IS updated based on local write*/
    count = power_profile_cluster_values.totalProfileNumber;

    if(count == 0x00 || count > 0x03){
        ActiveProfilesCount = 0x03;
    }
    else{
        ActiveProfilesCount = count;

    }
    uint16_t actualStartTime = 0x0000,actualCurrentTime = 0x0000;

    uint32_t currentTime = ZCL_CallBackGetCurrentTime();

    uint16_t duration = 0x0000;

    for(uint8_t loopIndex = 0x00;loopIndex < g_MAXIMUM_NO_OF_PROFILES_c;loopIndex++ ){

        if(serverUpdate[loopIndex].scheduled == g_TRUE_c){

            /* Updating the state of the profile */

            if(PreviousCurrentTime[loopIndex] == 0x00000000 ){
                PreviousCurrentTime[loopIndex] = currentTime;
            }

            /* 1minute passed*/
            if((currentTime - PreviousCurrentTime[loopIndex]) > 60 ){

                PreviousCurrentTime[loopIndex] = currentTime;
                /* increment by 1minute*/
                serverUpdate[loopIndex].currentTime[0] += 0x01;

                for(uint8_t loopIndex2 = 0x00;loopIndex2 < g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c;
                    loopIndex2++){

                    memUtils_memCopy((uint8_t*)&(actualCurrentTime),(uint8_t*)&(serverUpdate[loopIndex].currentTime),
                                     g_DURATION_FIELD_SIZE_c);
                    memUtils_memCopy((uint8_t*)&(actualStartTime),
                                     (uint8_t*)&
                                         (serverUpdate[loopIndex].energyPhaseStartTime[loopIndex2].scheduledTime),
                            g_DURATION_FIELD_SIZE_c);

                    memUtils_memCopy((uint8_t*)&duration,
                        (uint8_t*)&(ActiveEnergyPhasesWithActiveProfiles[loopIndex][loopIndex2][3]),
                            g_DURATION_FIELD_SIZE_c);

                    if((actualCurrentTime >= (actualStartTime )) && (actualCurrentTime <= (actualStartTime + duration))) {

                        ProfilesWithStateAndPhase[loopIndex].profileState = g_ENERGY_PHASE_RUNNING_c;
                        ProfilesWithStateAndPhase[loopIndex].energyPhaseId = loopIndex2 + 0x01;
                        /* Notify client about new state of the profile*/
                        createAndSendStateResponse(g_NULL_c,(loopIndex+1),
                                  g_PowerProfilesStateNotificationCommand_c);
                        break;
                    }
                    else if( (loopIndex2 == g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c -1)
                          && (actualCurrentTime >= (actualStartTime + duration)  )){
                               ProfilesWithStateAndPhase[loopIndex].profileState = g_POWER_PROFILE_ENDED_c;
                               serverUpdate[loopIndex].scheduled = g_FALSE_c;
                               if(ActiveProfilesCount) //RP: Need clarification

                        /* Reset */
                        PreviousCurrentTime[loopIndex] = 0x00000000;
                        /* Notify client about new state of the profile*/
                        createAndSendStateResponse(g_NULL_c,(loopIndex+1),
                                  g_PowerProfilesStateNotificationCommand_c);
                    }
                    else {
                        ProfilesWithStateAndPhase[loopIndex].profileState = g_ENERGY_PHASE_WAITING_TO_START_c;
                        /* Notify client about new state of the profile*/
                        createAndSendStateResponse(g_NULL_c,(loopIndex+1),
                                  g_PowerProfilesStateNotificationCommand_c);
                    }

                }

            }
        }
        else{

        }
    }
}
#endif/* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1*/
/******************************************************************************/
#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
void App_UpdateClientValues(void)
{
    /* Profile state and profile paramter is synchronized*/
    for(uint8_t loopIndex = 0x00;loopIndex < g_MAXIMUM_NO_OF_PROFILES_c;loopIndex++){

        if(ProfileWithCost[loopIndex].profileState == g_POWER_PROFILE_ENDED_c){

            ProfileWithCost[loopIndex].active = g_FALSE_c;
        }
    }
}
#endif /* g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1 */
/******************************************************************************/
void App_PowerProfileClusterEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_Data_Indication_t *pBuffer )
{
    uint8_t commandLength = 0x00,status = g_ZCL_Success_c;
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
    uint8_t loopIndex = 0x00;
#endif

    switch ( pZCLEvent->eventId )
    {
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
        case g_PowerProfileRequestEvent_c :{
            /* Check request is for single profile or all profiles,
            value 0x00 = all profiles else particular profile*/
            /* Server shd send multiple profile response for
                ( multiple profiles) */
            /* Response shd contain previous energy phases and about current
            energy phase which is running */

            /* Request validataion */
            commandLength = sizeof(ProfileRequestCommandPayload_t);

            if(pZCLEvent->eventLength == commandLength ){

                if(0x00 == ActiveProfilesCount){
                    /* No profile is active */
                    status = g_ZCL_Invalid_Value_c;
                }
                else{
                    status = g_ZCL_Success_c;
                    /* validate the profile Id */
                    if(lookForRequestedProfile(pZCLEvent->pEventData[0])){
                        /*Requested for Specific Profile */
                        createAndSendProfileResponse(pBuffer,
                            pZCLEvent->pEventData[0],
                            g_PowerProfileResponseCommand_c );
                    }
                    else{
                        if(pZCLEvent->pEventData[0] == 0x00 ){
                            /* Requested for all profiles*/
                        createAndSendProfileResponse(pBuffer,
                            pZCLEvent->pEventData[0],
                            g_PowerProfileResponseCommand_c );
                        }
                        else{
                            /* Requested profile Id does not exist*/
                            status = g_ZCL_Invalid_Value_c;
                        }
                    }
                }
            }
            else{
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;
        case g_PowerProfileStateRequestEvent_c:
            /* Check request is for single profile or all profiles,
            value 0x00 = all profiles else particular profile*/
            /* Server shd send Single StateResponse with multiple profile
            records ( multiple profiles) */

            /* Request validataion */
            commandLength = 0;//sizeof(ProfileRequestCommandPayload_t);

            createAndSendStateResponse(pBuffer,g_ALL_ACTIVE_POWER_PROFILES_c,
                                g_PowerProfileStateResponseCommand_c);

//            if(pZCLEvent->eventLength == commandLength ){
//
//                if((pZCLEvent->pEventData[0]== g_ALL_ACTIVE_POWER_PROFILES_c)
//                   ||(lookForRequestedProfile(pZCLEvent->pEventData[0]))){
//                    createAndSendStateResponse(pBuffer,pZCLEvent->pEventData[0],
//                    g_PowerProfileStateResponseCommand_c);
//                }
//                else{
//                	DEBUG_INV[3] = 3;
//                    status = g_ZCL_Invalid_Value_c;
//                }
//            }
//            else{
//            	DEBUG_INV[4] = 4;
//                status = g_ZCL_Invalid_Value_c;
//            }
//
//            if(status != g_ZCL_Success_c ){
//                /* call default response with status */
//                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
//            }

            break;
        case g_GetPowerProfilePriceResponseEvent_c:
            break;
        case g_GetOverallSchedulePriceResponseEvent_c:
            break;
        case g_EnergyPhasesScheduleNotificationEvent_c:
        case g_EnergyPhasesScheduleResponseEvent_c:{
            /* According to the Schedule Received from Scheduler(Client)
            for profile profile shd start,
            If the same profile is already in running state schedule
            received is ignored*/

            uint8_t profileId = 0xff,noOfScheduledPhases = 0x00;

            /* Payload = ZCL_status,profileId,noofscheduledPhases,energyPhaseid i ,
            scheduledTime(2) i ......*/
            uint8_t tempData[sizeof(UpdateOfEnergyPhase_t)*0x03];
            commandLength = pZCLEvent->eventLength;
            if(commandLength > 0x02 ){
                profileId = pZCLEvent->pEventData[0];
                noOfScheduledPhases = pZCLEvent->pEventData[1];

                UpdateOfEnergyPhase_t *pEnergyPhase = g_NULL_c;

                if(noOfScheduledPhases){
                    memUtils_memCopy(tempData,(uint8_t*)&(pZCLEvent->pEventData[2]),
                                     noOfScheduledPhases *sizeof(UpdateOfEnergyPhase_t));

                    serverUpdate[profileId-0x01].scheduled = g_TRUE_c;

                    pEnergyPhase = (UpdateOfEnergyPhase_t*)tempData ;

                    for(uint8_t loopIndex = 0x00;loopIndex <noOfScheduledPhases;
                    loopIndex++){
                        /* Energy Phase Id is updated */
                        serverUpdate[profileId-0x01].energyPhaseStartTime[loopIndex].
                            energyPhaseId = pEnergyPhase->energyPhaseId;
                        memUtils_memCopy((uint8_t*)&(serverUpdate[profileId-0x01].
                                         energyPhaseStartTime[loopIndex].scheduledTime),
                                             (pEnergyPhase->scheduledTime),
                                             g_DURATION_FIELD_SIZE_c);
                        /* move to next Phase details*/
                        pEnergyPhase++;
                    }
                }
            }

        }
            break;
        case g_PowerProfileScheduleConstraintsRequestEvent_c :{
            /* Request validataion */
            commandLength = sizeof(ProfileRequestCommandPayload_t);

            if(pZCLEvent->eventLength == commandLength ){
                uint8_t profileId = 0x00;

                profileId = pZCLEvent->pEventData[0];

                /* validate the profile Id */
                if(lookForRequestedProfile(profileId )){
                    /*Extract Constraints defined by the user for the Specified
                    Power Profile Id*/
                    for(loopIndex = 0x00;loopIndex < ActiveProfilesCount ;
                        loopIndex++){

                        if(profileId == ActiveProfilesWithTimeConstraints[loopIndex].profileId ){
                            /* found the requested profile*/
                            /* extracting user set starttime and stop time*/
                            status = g_ZCL_Success_c;
                            SendPowerProfileResponse(pBuffer,
                                (uint8_t)g_PowerProfileScheduleConstraintsResponseCommand_c,
                                status,
                                (uint8_t*)&(ActiveProfilesWithTimeConstraints[loopIndex]),
                                sizeof(PowerProfileScheduleConstraints_t),
                                g_TRUE_c );
                        }
                    }
                }
                else{
                    /* Requested Profile Id does not exists*/
                    status = g_ZCL_Invalid_Value_c;
                }
            }
            else{
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }

        break;
        case g_EnergyPhasesScheduleStateRequestEvent_c:{
            /* Request validataion */
            commandLength = sizeof(ProfileRequestCommandPayload_t);

            if(pZCLEvent->eventLength == commandLength ){
                uint8_t profileId = 0x00;

                profileId = pZCLEvent->pEventData[0];

                if(profileId != g_INVALID_POWER_PROFILE_ID_c){
                    createAndSendScheduledStateResponse(pBuffer,profileId,
                    g_EnergyPhasesScheduleStateResponseCommand_c);
                }
                else{
                    status = g_ZCL_Invalid_Value_c;
                }
            }
            else{
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }

        }
            break;
        case g_GetPowerProfilePriceExtendedResponseEvent_c:
            break;
#endif /* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1*/
#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)

        case g_PowerProfileNotificationEvent_c:
            case g_PowerProfileResponseEvent_c:{
            /* This Notification received by client from server */
            uint8_t tempNotification[35];
            PowerProfileNotificationPayload_t *ptempNotification =
                (PowerProfileNotificationPayload_t *)tempNotification;
            /* max energyphase is 3 ,its content is 10 byte each,
                1st 3 bytes about profile
                last +1 byte is zcl status */
            uint8_t maxPayloadCount = 0x03 + (3*10) ;
            uint8_t profileId = 0x00,energyPhaseCount = 0x00;
            AboutEachEnergyPhase_t *pEnergyPhase = g_NULL_c;

            commandLength = pZCLEvent->eventLength ;

                if((commandLength > 0x00) &&(commandLength <= maxPayloadCount)){

                    if(commandLength > sizeof(PowerProfileNotificationPayload_t)){

                        memUtils_memCopy((uint8_t*)(ptempNotification),
                                         (uint8_t*)&(pZCLEvent->pEventData[0]),
                                         commandLength);


                        profileId = ptempNotification->powerProfileId;
                        energyPhaseCount = ptempNotification->noOfTransferredPhases;

                        if(profileId != 0xff && profileId != 0x00){
                            /* Updating the profile details @ client side*/
                            ProfileWithCost[profileId-1].active = g_TRUE_c;

                            ProfileWithCost[profileId-1].profilePriceInfo.
                                powerProfileId = profileId;

                            if(energyPhaseCount){

                                pEnergyPhase =
                                    (AboutEachEnergyPhase_t *)(ptempNotification->aboutEnergyPhaseWithPeakPower);

                                /* updating energy phase details*/
                                for(uint8_t loopIndex = 0x00;loopIndex <energyPhaseCount;
                                    loopIndex++){
                                    ActiveEnrPhasesWithActiveProfilesCli[profileId-1][loopIndex][0]
                                        = loopIndex + 1;

                                    memUtils_memReverseCopy(((uint8_t*)&
                                    (ActiveEnrPhasesWithActiveProfilesCli[profileId-1][loopIndex][3])),
                                        (uint8_t*)(pEnergyPhase->expectedDuration),g_DURATION_FIELD_SIZE_c);

                                    memUtils_memReverseCopy(((uint8_t*)&
                                    (ActiveEnrPhasesWithActiveProfilesCli[profileId-1]
                                     [loopIndex][1])),
                                        (uint8_t*)(pEnergyPhase->maxActivationDelay),g_DURATION_FIELD_SIZE_c);

                                    pEnergyPhase++;
                                }
                            }
                        }
                    }
                }
                else{
                    /* No update required*/
                }
            }
        break;
        case g_PowerProfilePriceEvent_c:{
            /* Check request is for single profile or all profiles,
            value 0x00 = all profiles else particular profile*/
            /* Server shd send multiple profile response for
                ( multiple profiles) */

            /* Request validataion */
            commandLength = sizeof(ProfileRequestCommandPayload_t);

            if(pZCLEvent->eventLength == commandLength ){
                uint8_t profileId = 0x00;


                profileId = pZCLEvent->pEventData[0];
                /* ZCL not found is returned if the cost is not available
                for requested profile*/
                if(profileId != 0xff){

                    createAndSendPriceResponse(pBuffer,
                        (uint8_t)g_GetPowerProfilePriceResponseCommand_c,
                        profileId,
                        g_FALSE_c,g_NULL_c);
                }
                else{
                    /* Request is improper*/
                    status = g_ZCL_Invalid_Value_c;

                }
            }
            else{
                /* Request is improper*/
                status = g_ZCL_Invalid_Value_c;

            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;
        case g_PowerProfileStateResponseEvent_c:
        case g_PowerProfilesStateNotificationEvent_c:{
            /* profile state is updated in table maintained at client side */
            /* Profile as active g_TRUE_c or g_FALSE_c is based on state */
            uint8_t powerProfileCount = 0x00;
            commandLength = pZCLEvent->eventLength;
            uint8_t expectedLength = 0x00;
            PowerProfileRecord_t *pEachRecord = g_NULL_c;

            /* Update Client table if there is any payload to update*/
            if(commandLength){
                powerProfileCount = pZCLEvent->pEventData[0];
                /* ZCL status,ProfileCount ,+ record...*/
                expectedLength = ((powerProfileCount * sizeof(PowerProfileRecord_t) ) + 1);
                if(/*(expectedLength == commandLength )&&*/ (commandLength > 0x02)){
                    pEachRecord = (PowerProfileRecord_t *)&(pZCLEvent->pEventData[1]);
                    /* Updating Remote state */
                    RemoteControlAttrMaintainedByClient = pEachRecord->powerProfileRemoteControl;
                    if(powerProfileCount >0x01 ){
                        for(uint8_t loopIndex = 0x00;loopIndex < powerProfileCount;
                            loopIndex++){
                            /* Updating profileState*/
                            ProfileWithCost[loopIndex].profileState = pEachRecord->profileState;

                            /* pointing to next record*/
                            pEachRecord = pEachRecord +1 ;//((uint8_t*)pEachRecord + 4);
                        }
                    }
                    else if(powerProfileCount == 0x01){
                        /* Updating profileState*/
                        ProfileWithCost[pEachRecord->powerProfileId - 0x01].profileState =
                            pEachRecord->profileState;
                    }
                }
            }
        }
        break;
        case g_GetOverallSchedulePriceEvent_c:{
            /* to retrieve the overall cost associated to all the Power Profiles
            scheduled by the scheduler (the device supporting the
            Power Profile cluster client side) for the next 24 hours */

            commandLength = 0x00;
            if(pZCLEvent->eventLength == commandLength ){

                createAndSendOverAllSchPriceResponse(pBuffer,
                        (uint8_t)g_GetOverallSchedulePriceResponseCommand_c,
                        g_FALSE_c);
            }
            else{
                /* Request is improper*/
                status = g_ZCL_Invalid_Value_c;
            }

            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
        }
        break;
        case g_EnergyPhasesScheduleRequestEvent_c:
            /* Request payload is one byte ie., Profile Id*/
            commandLength = 0x01;
            if(pZCLEvent->eventLength == commandLength ){

                uint8_t profileId = 0x00;
                profileId = pZCLEvent->pEventData[0];

                /* Check Remote Ctrl server attribute is Enable of Disable  */
                /* Inorder to be in track of ServerAttribute Client Side
                RemoteCtrl Attribute is maintained which is updated when
                ZCL READ is req from client side and it is reportable attribute*/

//                /* TBD..... need to be updated from ZCL read interaction*/
//                RemoteControlAttrMaintainedByClient = g_TRUE_c;

                if(RemoteControlAttrMaintainedByClient == g_TRUE_c ){
                    createAndSendScheduleToServer(pBuffer,
                        (uint8_t)g_EnergyPhasesScheduleResponseCommand_c,
                        profileId,g_FALSE_c);
                }
            }
            else{
                /* Request is improper*/
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }
            break;
        case g_EnergyPhasesScheduleStateResponseEvent_c:
            break;
        case g_EnergyPhasesScheduleStateNotificationEvent_c:
            break;
        case g_PowerProfileScheduleConstraintsNotificationEvent_c:
        case g_PowerProfileScheduleConstraintsResponseEvent_c:{

                uint8_t expectedCommandLen = sizeof(PowerProfileScheduleConstraints_t);
                commandLength = pZCLEvent->eventLength ;

                PowerProfileScheduleConstraints_t receivedData;
                if(expectedCommandLen == commandLength ){

                    memUtils_memCopy((uint8_t*)&receivedData ,
                        (uint8_t*)&(pZCLEvent->pEventData[0]),expectedCommandLen);

                    /* Update the Schedule Received from Server only if profile
                    state is not running */
                    if(( g_TRUE_c == ProfileWithCost[receivedData.profileId - 0x01].active) &&
                       (ProfileWithCost[receivedData.profileId - 0x01].profileState < g_ENERGY_PHASE_RUNNING_c))
                    {
                        /* Update the new Schedule give from server for schedule
                           to scheduler (client)*/

                        memUtils_memCopy((uint8_t*)&(ProfileWithCost[receivedData.profileId - 0x01].startTimeMaintainedByClient),
                        (uint8_t*)&(receivedData.startAfter),g_DURATION_FIELD_SIZE_c);

                        memUtils_memCopy((uint8_t*)&(ProfileWithCost[receivedData.profileId - 0x01].stopTimeMaintainedByClient),
                        (uint8_t*)&(receivedData.stopBefore),g_DURATION_FIELD_SIZE_c);

                    }
                }
            }
            break;
        case g_PowerProfilePriceExtendedEvent_c:{
            uint8_t optionValue = 0x00;
            uint8_t profileId = 0xff;
            uint8_t startTime[2] = {0xff,0xff};
            uint8_t *pStartTime = g_NULL_c;
            uint8_t commandLenShouldBe = 0x00;

            commandLength = pZCLEvent->eventLength;
            /* Just sanity check to retrieve ist field */
            if(commandLength > 0 ){
                optionValue = pZCLEvent->pEventData[0];

                if(optionValue == g_POWER_PROFILE_STARTTIME_FIELD_PRESENT_c){
                    commandLenShouldBe = sizeof(GetPowerProfilePriceExtended_t);
                }
                else{
                    /* power profiel start time field should not be there */
                    commandLenShouldBe =
                        sizeof(GetPowerProfilePriceExtended_t) - 0x02 ;
                }

                if(commandLenShouldBe == commandLength){
                    profileId =  pZCLEvent->pEventData[1];

                    if(optionValue == g_POWER_PROFILE_STARTTIME_FIELD_PRESENT_c){
                        memUtils_memCopy((uint8_t*)startTime,
                                         (uint8_t*)&(pZCLEvent->pEventData[2]),
                                         g_DURATION_FIELD_SIZE_c);
                        pStartTime = startTime;
                    }
                    else{
                        pStartTime = g_NULL_c;
                    }
                    /* Command length is proper */
                    if(profileId != 0xff){
                        createAndSendPriceResponse(pBuffer,
                        (uint8_t)g_GetPowerProfilePriceExtendedResponseCommand_c,
                        profileId,
                        g_FALSE_c,pStartTime);
                    }
                }
                else{
                    /* Request is improper*/
                    status = g_ZCL_Invalid_Value_c;
                }
            }
            else{
                /* Request is improper*/
                status = g_ZCL_Invalid_Value_c;
            }
            if(status != g_ZCL_Success_c ){
                /* call default response with status */
                App_SendDefaultResponse(status ,g_FALSE_c,pBuffer );
            }

        }
        break;

#endif  /* g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1*/

        default :
        break;

    }
}
/******************************************************************************/
void SendPowerProfileResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t status ,uint8_t* pRes,uint8_t responseLength,
    bool serverToClient)

{
    uint8_t destAddress[8];
    uint8_t payloadLength = 0;
    /* ZCL Header Creation */

    apsdeDataPacket->a_asdu[0] = g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    if(serverToClient){
        apsdeDataPacket->a_asdu[0] |= g_ZCL_SERVER_TO_CLIENT_DIRECTION_c;

    }
    else{
        apsdeDataPacket->a_asdu[0] |= g_ZCL_CLIENT_TO_SERVER_DIRECTION_c;
    }

    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = responseType;
    //apsdeDataPacket->a_asdu[3] = status;

    payloadLength += 3;
    memUtils_memCopy(destAddress , apsdeDataPacket->src_address.IEEE_address, 0x08 );



    if(g_ZCL_Success_c == status && responseLength > 0){

        apsdeDataPacket->a_asdu[3] =*pRes;

        pRes++;
        memUtils_memReverseCopy((uint8_t*)&(apsdeDataPacket->a_asdu[4]) ,pRes,
                         0x02);

        pRes += 0x02;
        memUtils_memReverseCopy((uint8_t*)&(apsdeDataPacket->a_asdu[6]) ,pRes,
                         0x02);

        payloadLength += responseLength;
    }

    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_POWER_PROFILE_CLUSTER_c,payloadLength ,
        apsdeDataPacket->a_asdu );
}

/******************************************************************************/
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
void InvokeTransmistOfProfileNotification()
{
    createAndSendProfileResponse(g_NULL_c,g_ALL_ACTIVE_POWER_PROFILES_c,
                                 g_PowerProfileNotificationCommand_c);

}
/******************************************************************************/
void InvokeTransmitOfProfileStateNotification()
{
    createAndSendStateResponse(g_NULL_c,g_ALL_ACTIVE_POWER_PROFILES_c,
                                  g_PowerProfilesStateNotificationCommand_c);

}
/******************************************************************************/
void InvokeTransmitOfPhasesScheduleStateNotification()
{
    /* Payload format :
        profile Id,energy phasecount,energyPhaseId(i) scheduledTime(i).......*/

    /* Retrieve the profiles currently programmed or profiles with running state*/

    createAndSendScheduledStateResponse(g_NULL_c,g_ALL_ACTIVE_POWER_PROFILES_c,
        g_EnergyPhasesScheduleStateNotificationCommand_c);

}
/******************************************************************************/
void InvokeTransmitScheduleConstraintsNotification(uint8_t profileId,
                                                   uint8_t *pStartAndStopTime,
                                                   uint8_t length)
{
    uint8_t bufferIndex = g_NO_AVAILABLE_BUFFER_c;
    APSDE_Data_Request_t *p_apsde_data_request = g_NULL_c;

   /*
    This command is generated when the server side of the Power Profile cluster,
    needs to notify a change in the constraints of the Power Profile
    (e.g. the user selected boundaries for the specific behavior of the device).
     */

    if(timeConstraintsValid(profileId,pStartAndStopTime,length)){

        bufferIndex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );

        if( g_NO_AVAILABLE_BUFFER_c != bufferIndex ) {
#if (DEBUG_TEST == 1)
            /* Buffer ID --> 13 */
            allocatedbuffer[bufferIndex]= 13;
            freebuffer[bufferIndex][0]= 13;
            freebuffer[bufferIndex][1]= 0;
#endif
            p_apsde_data_request = ( APSDE_Data_Request_t * )buffMgmt_getBufferPointer( bufferIndex );
            *((uint8_t*)p_apsde_data_request) = g_APSDE_Data_Request_c;

            p_apsde_data_request = (APSDE_Data_Request_t *)&(((APSDE_Request_t *)p_apsde_data_request)->
                                                       APSDE_Request_Primitive.apsde_data_request);

            p_apsde_data_request->dest_addr_mode = g_SHORT_ADDR_MODE_c;
            p_apsde_data_request->dest_address.short_address   = 0xffff;
            p_apsde_data_request->profile_id     = g_APP_PROFILE_ID_c;
            p_apsde_data_request->src_endpoint   = ZCL_Callback_Get_Device_EndPoint();
            p_apsde_data_request->cluster_id     = g_POWER_PROFILE_CLUSTER_c;


            p_apsde_data_request->txoptions =  App_getClusterSecurity( g_POWER_PROFILE_CLUSTER_c );
            p_apsde_data_request->radius = g_DEFAULT_RADIUS_c;


            p_apsde_data_request->asdu[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
                                               g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                               g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
            p_apsde_data_request->asdu[1] = App_GetTransSeqNumber();
            p_apsde_data_request->asdu[2] = g_PowerProfileScheduleConstraintsNotificationCommand_c;
            //p_apsde_data_request->asdu[3] = g_ZCL_Success_c;

            p_apsde_data_request->asdu[3] = profileId;

            if(length >= 0x02){
                memUtils_memReverseCopy(&(p_apsde_data_request->asdu[4]),pStartAndStopTime,
                             0x02);
                if(length == 0x04){
                    pStartAndStopTime +=2;
                    memUtils_memReverseCopy(&(p_apsde_data_request->asdu[6]),pStartAndStopTime,
                             0x02);
                }

            }

            p_apsde_data_request->asdulength  = 0x03 + length +0x01 ;
            //APS_APSDErequest( bufferIndex );
            APS_APSDE_request(bufferIndex,p_apsde_data_request );
        }
    }
}
#endif
/******************************************************************************/
#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
void InvokeTransmistEnergyPhasesSchNotification()
{
    /* Check Remote Ctrl server attribute is Enable of Disable  */
    /* Inorder to be in track of ServerAttribute Client Side
    RemoteCtrl Attribute is maintained which is updated when
    ZCL READ is req from client side and it is reportable attribute*/

    /* Or Through ProfileState Request/Response which contains remoteControl */


   //RemoteControlAttrMaintainedByClient = g_TRUE_c;

    if(RemoteControlAttrMaintainedByClient == g_TRUE_c ){
        createAndSendScheduleToServer(g_NULL_c,
            (uint8_t)g_EnergyPhasesScheduleNotificationCommand_c,
            g_ALL_ACTIVE_POWER_PROFILES_c,g_FALSE_c);
    }
}
/******************************************************************************/
static void createAndSendScheduleToServer(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t profileId, bool serverToClient)
{
    uint8_t profileIndex = 0x00,energyPhaseIndex = 0x00;

    bool singleResponse = g_FALSE_c ;

    uint8_t destAddress[8];
    AboutProfileEnergyPhaseClient_t tempEnergyPhases[g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c];

    singleResponse = MultipleSchAttrMaintainedByClient;

    uint8_t tempProfileId = 0xff;

    uint8_t payloadLength = 0x00,profileCount = 0x00,energyPhaseCount = 0x00;

    uint8_t energyPhaseCountListIndex = 0x00;
    CountTracker_t energyPhaseCountList[g_MAXIMUM_NO_OF_PROFILES_c];

    APSDE_Data_Indication_t tempApsdeDataPacket;
    uint8_t bufferIndex = g_NO_AVAILABLE_BUFFER_c;
    APSDE_Data_Request_t * pAppDataReq = g_NULL_c;
    APSDE_Request_t* pDataReq = g_NULL_c;

    /* Schedule Payload

    ProfileId (1)  NoOfScheduledPhases(1) EnergyPhaseId-i(1) ScheduledTime-i(2)*/


    for(profileIndex = 0x00;profileIndex < g_MAXIMUM_NO_OF_PROFILES_c
            ; profileIndex++){

        /*  If profile is know by client, then only it can proceed with
                scheduling */
        if((ProfileWithCost[profileIndex].active == g_TRUE_c)&&
            (ProfileWithCost[profileIndex].profileState < g_ENERGY_PHASE_RUNNING_c )){
            tempProfileId =
                ProfileWithCost[profileIndex].profilePriceInfo.powerProfileId;

            for(energyPhaseIndex = 0x00;
                (energyPhaseIndex < g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c)
                    ;
                energyPhaseIndex++ ){

                /* [0] is to get energy phase Id field   */
                if(ActiveEnrPhasesWithActiveProfilesCli[profileIndex][energyPhaseIndex][0]
                   != g_INVALID_ENERGY_PHASE_ID_c){

                    /* retrieving old value */
                    memUtils_memCopy((uint8_t*)&(tempEnergyPhases[energyPhaseIndex]) ,
                        (uint8_t*)&(ActiveEnrPhasesWithActiveProfilesCli[profileIndex][energyPhaseIndex]),
                        sizeof(AboutProfileEnergyPhaseClient_t));
                    energyPhaseCount++;
                    if(singleResponse){
                        /* Because server supports only single energy phase */
                        break;
                    }/* end if if*/

                }/* end if if*/
            }/* end of energy Phase loop */

           /*updating the schedule*/
            updateTheSchedule(tempProfileId,tempEnergyPhases,singleResponse,energyPhaseCount);

            /* Update the updated schedule */

            memUtils_memCopy(
                (uint8_t*)&(ActiveEnrPhasesWithActiveProfilesCli[profileIndex]),
                (uint8_t*)&(tempEnergyPhases[0]) ,
                sizeof(AboutProfileEnergyPhaseClient_t)*energyPhaseCount);

            energyPhaseCountList[energyPhaseCountListIndex].profileNo = tempProfileId;
            energyPhaseCountList[energyPhaseCountListIndex].energyPhaseCount = energyPhaseCount;
            energyPhaseCountListIndex++;
            profileCount++;
            /* Reset ,each profile can have different no of energy phases */
            energyPhaseCount = 0x00;
        }
    }/* end of profile loop*/


    /* Power Profile Id */
    if(profileId == g_ALL_ACTIVE_POWER_PROFILES_c){
       /* in case of multiple profiles the server shall send multiple messages,
        one for each Power Profile */

        /* creating back up of information about the requested device */
        if(apsdeDataPacket != g_NULL_c){

            memUtils_memCopy((uint8_t*)&tempApsdeDataPacket,
                             (uint8_t*)apsdeDataPacket,
                             sizeof(APSDE_Data_Indication_t));
        }
    }

    /* Power Profile Id value == 0x00 indicates all active profiles of
    the device */
    if((profileId == g_ALL_ACTIVE_POWER_PROFILES_c) ||
           (responseType == g_EnergyPhasesScheduleNotificationCommand_c)){

        /* Each time buffer is allocated which will be free in APS_APSDErequest
        function*/
        for(uint8_t loopIndex = 0x00;loopIndex <= profileCount ;loopIndex++){

            /* reset bcoz it is used again for next profile */
            payloadLength = 0x00;

            bufferIndex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
            if( g_NO_AVAILABLE_BUFFER_c != bufferIndex ) {
#if (DEBUG_TEST == 1)
                /* Buffer ID --> 88 */
                allocatedbuffer[bufferIndex]= 88;
                freebuffer[bufferIndex][0]= 88;
                freebuffer[bufferIndex][1]= 0;
#endif
                pDataReq =
                    (APSDE_Request_t *)buffMgmt_getBufferPointer( bufferIndex );

                pDataReq->msgType = g_APSDE_Data_Request_c;
                pAppDataReq =
                    (APSDE_Data_Request_t *)&((pDataReq)->APSDE_Request_Primitive.apsde_data_request);


                if(responseType == g_EnergyPhasesScheduleNotificationCommand_c){
                    /*Notification is a broadcast packet */
                    pAppDataReq->dest_addr_mode = g_SHORT_ADDR_MODE_c;
                    pAppDataReq->dest_address.short_address = 0xffff;
                    pAppDataReq->src_endpoint =
                        ZCL_Callback_Get_Device_EndPoint();
                }
                else{

                    pAppDataReq->dest_addr_mode = tempApsdeDataPacket.src_addr_mode;

                    if(pAppDataReq->dest_addr_mode == g_Short_Address_Mode_c ){
                        memUtils_memCopy (
                            (uint8_t *)&(pAppDataReq->dest_address.short_address),
                          ( uint8_t *)&(tempApsdeDataPacket.src_address.short_address),
                          g_SHORT_ADDRESS_LENGTH_c );

                    }
                    else if(pAppDataReq->dest_addr_mode == g_IEEE_Address_Mode_c) {
                        memUtils_memCopy ( pAppDataReq->dest_address.IEEE_address,
                                  ( uint8_t *)&(tempApsdeDataPacket.src_address.IEEE_address),
                                  g_EXTENDED_ADDRESS_LENGTH_c );
                    }

                    pAppDataReq->src_endpoint = tempApsdeDataPacket.dest_endpoint;
                    pAppDataReq->dest_endpoint = tempApsdeDataPacket.src_endpoint;
                }
                pAppDataReq->profile_id = g_APP_PROFILE_ID_c;
                pAppDataReq->cluster_id = g_POWER_PROFILE_CLUSTER_c;

                /* ZCL Header Creation */
                pAppDataReq->asdu[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
                                        g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                            g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
                pAppDataReq->asdu[1] = App_GetTransSeqNumber();
                pAppDataReq->asdu[2] = responseType;

                if(profileCount){

                    //pAppDataReq->asdu[3] = g_ZCL_Success_c;

                    pAppDataReq->asdu[3] = energyPhaseCountList[loopIndex].profileNo;
                    pAppDataReq->asdu[4] = energyPhaseCountList[loopIndex].energyPhaseCount ;
                    uint8_t tempIndex = 0x00,arrayOffset = 0x05;
                    for(;tempIndex < energyPhaseCountList[loopIndex].energyPhaseCount;
                        tempIndex++){
                            /* To get Energy phase Id */
                        pAppDataReq->asdu[arrayOffset++ ] = tempIndex + 0x01;
                        /* To get the Scheduled Time */
                        pAppDataReq->asdu[arrayOffset++]   =
                            ActiveEnrPhasesWithActiveProfilesCli[
                                energyPhaseCountList[loopIndex].profileNo - 0x01][tempIndex][5];

                        pAppDataReq->asdu[arrayOffset++ ] =
                            ActiveEnrPhasesWithActiveProfilesCli[
                                energyPhaseCountList[loopIndex].profileNo - 0x01][tempIndex][6];
                    }
                    payloadLength = arrayOffset;
                }
                else{
                    /* Profile Not Found */
                    pAppDataReq->asdu[3] = g_ZCL_Invalid_Value_c;
                    payloadLength = 0x04;
                }

                pAppDataReq->asdulength = payloadLength;
                pAppDataReq->txoptions =  App_getClusterSecurity( g_POWER_PROFILE_CLUSTER_c );
                pAppDataReq->radius = g_DEFAULT_RADIUS_c;

                //APS_APSDErequest(bufferIndex);
                APS_APSDE_request(bufferIndex,pAppDataReq );
            }
        }
    }
    else{
        /* For Specific Profile which is requested */
        /* ZCL Header Creation */
        apsdeDataPacket->a_asdu[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c |
                                g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                    g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
        apsdeDataPacket->a_asdu[2] = responseType;
        //apsdeDataPacket->a_asdu[3] = status;

        if((ProfileWithCost[profileId-1].active == g_TRUE_c)&&
            (ProfileWithCost[profileId-1].profileState < g_ENERGY_PHASE_RUNNING_c )){

            apsdeDataPacket->a_asdu[3] = profileId;
            apsdeDataPacket->a_asdu[4] = energyPhaseCountList[profileId -1].energyPhaseCount ;
            uint8_t tempIndex = 0x00,arrayOffset = 0x05;
            for(;tempIndex < energyPhaseCountList[profileId -1].energyPhaseCount;
                tempIndex++){
                    /* To get Energy phase Id */
                apsdeDataPacket->a_asdu[arrayOffset++ ] = tempIndex + 0x01;
                /* To get the Scheduled Time */
                apsdeDataPacket->a_asdu[arrayOffset++]   =
                    ActiveEnrPhasesWithActiveProfilesCli[
                        energyPhaseCountList[profileId -1].profileNo - 0x01][tempIndex][5];

                apsdeDataPacket->a_asdu[arrayOffset++ ] =
                    ActiveEnrPhasesWithActiveProfilesCli[
                        energyPhaseCountList[profileId -1].profileNo - 0x01][tempIndex][6];
            }
            payloadLength =  arrayOffset;
        }
        else{
            /* Profile Not Found */
            apsdeDataPacket->a_asdu[3] = g_ZCL_Invalid_Value_c;
            payloadLength = 0x04;
        }
        memUtils_memCopy(destAddress,apsdeDataPacket->src_address.IEEE_address,
                         0x08 );


        App_SendData( apsdeDataPacket->src_addr_mode,
            destAddress,apsdeDataPacket->src_endpoint,
            g_POWER_PROFILE_CLUSTER_c,payloadLength ,
            apsdeDataPacket->a_asdu );
    }
}
/******************************************************************************/
static void updateTheSchedule(uint8_t profileId, AboutProfileEnergyPhaseClient_t
                              pEnergyPhaseList[],bool singleResponse,
                              uint8_t energyPhaseCount)
{
    /* Schedule is created based on the start and stop time constraints ,
    expected duration and maximum activation delay*/



    uint16_t expectedDuration;
    /* This energy phase is scheduled at the following time,it is cleared once
    profile ends*/
    uint16_t scheduledAt;


    uint16_t startTime = 0xffff ,nextPhaseStart = 0x0000;
    uint16_t stopTime;
    uint8_t loopIndex1 = 0x00,loopIndex2 = 0x00;


    for(loopIndex1= 0x00 ;loopIndex1 < g_MAXIMUM_NO_OF_PROFILES_c;loopIndex1++ ){


        /* Profile Id Match*/
        if(ProfileWithCost[loopIndex1].profilePriceInfo.powerProfileId == profileId){

            /* copy scheduled start time */
            memUtils_memCopy((uint8_t*)&startTime,
                (uint8_t*)&(ProfileWithCost[loopIndex1].startTimeMaintainedByClient) ,
                        g_DURATION_FIELD_SIZE_c);

            /* copy scheduled stop time */
            memUtils_memCopy((uint8_t*)&stopTime,
                (uint8_t*)&(ProfileWithCost[loopIndex1].stopTimeMaintainedByClient) ,
                        g_DURATION_FIELD_SIZE_c);


            for(loopIndex2 = 0x00; loopIndex2 < energyPhaseCount ;loopIndex2++){

                /* actual schedule update */
                if(pEnergyPhaseList[loopIndex2].energyPhaseId !=
                    g_INVALID_ENERGY_PHASE_ID_c){

                    memUtils_memCopy((uint8_t*)&expectedDuration,
                    (uint8_t*)&(pEnergyPhaseList[loopIndex2].expectedDuration ) ,
                            g_DURATION_FIELD_SIZE_c);


                if(loopIndex2 == 0x00){
                    /* only in case of 1st phase time is taken from
                    start time*/
                    scheduledAt = (startTime + g_ONE_MINUTE_c );
                    /* 1st phase will end @ nextPhaseStart */
                    nextPhaseStart = scheduledAt + expectedDuration  ;
                }
                else {
                    if(nextPhaseStart < stopTime ){
                        scheduledAt = nextPhaseStart + g_ONE_MINUTE_c ;
                        nextPhaseStart = scheduledAt + expectedDuration  ;

                    }
                }
                    /* Updated Scheduled time for profile and its
                    energy phase */

                      memUtils_memCopy(
                        (uint8_t*)(pEnergyPhaseList[loopIndex2].scheduledAt ) ,
                        (uint8_t*)&scheduledAt,g_DURATION_FIELD_SIZE_c);

                    if(singleResponse){
                        /* only starting energy phase is scheduled ,bcoz server
                        does not support multiple energy phase */
                        break;
                    }
                    else{
                        continue;
                    }
                }
            } /*end of energy phase loop*/
            break;
        }/* end of profile match*/
    }/*end of profile loop */
}
/******************************************************************************/
void SetCostOfProfile(uint8_t *pCostInfo,uint8_t length )
{

    /* 1.check profile Id is not 0xff and 0x00.
       2.profile id's let it be unique, if new cost need to set for same id
         then overwrite entry with new value.
       3.Profile Id cannot exceed TotalProfileNumber,.
    */
    uint8_t profileId = *pCostInfo;
    uint8_t existAlready = g_FALSE_c;
    uint8_t loopIndex = 0x00;

    ActiveProfilesMaintainedByClient_t temp;

    if(profileId != 0xff && profileId != 0x00){
        /* Profile Id is valid*/

        for(loopIndex = 0x00; (loopIndex < g_MAXIMUM_NO_OF_PROFILES_c) ;
            loopIndex++){
            if(ProfileWithCost[loopIndex].profilePriceInfo.powerProfileId ==
               profileId){
                existAlready = g_TRUE_c;
                break;
            }/* end of if*/
        }/* end of for*/

        if(CostArrayTracker < g_MAXIMUM_NO_OF_PROFILES_c ){

            if(existAlready){
                /* Copy new user defined value to existing entry */
                memUtils_memCopy((uint8_t *)&(ProfileWithCost[loopIndex]),
                                 pCostInfo,length);
            }
            else{
                memUtils_memCopy((uint8_t *)&(ProfileWithCost[CostArrayTracker]),
                                 pCostInfo,length);
                CostArrayTracker++;
            }
        }
        else{
            /* Again reset to Zero*/
            CostArrayTracker = 0x00;
        }/* end of if*/
    }/* end of if */

    /* Sorting the array according to profileId */
    for(loopIndex = 0x00; (loopIndex < CostArrayTracker) ; loopIndex++){
        if(ProfileWithCost[loopIndex].profilePriceInfo.powerProfileId >
           ProfileWithCost[loopIndex+1].profilePriceInfo.powerProfileId){

            memUtils_memCopy((uint8_t*)&temp,(uint8_t*)&(ProfileWithCost[loopIndex]),
                             sizeof(ActiveProfilesMaintainedByClient_t));
            memUtils_memCopy((uint8_t*)&(ProfileWithCost[loopIndex]),
                             (uint8_t*)&(ProfileWithCost[loopIndex+1]),
                             sizeof(ActiveProfilesMaintainedByClient_t));
            memUtils_memCopy((uint8_t*)&(ProfileWithCost[loopIndex+1]),(uint8_t*)&temp,
                             sizeof(ActiveProfilesMaintainedByClient_t));

        }
    }
}

/******************************************************************************/
void createAndSendPriceResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType,uint8_t profileId, bool serverToClient,uint8_t*pStartTime)
{
    uint8_t loopIndex = 0x00;
    uint8_t tempData[sizeof(ActiveProfilesMaintainedByClient_t)*
        g_MAXIMUM_NO_OF_PROFILES_c];
    uint8_t destAddress[8];
    uint8_t payloadLength = 0;
    /* This variable is used to use common code for 2 responses,Price Req and
    PriceExtended command*/
    uint8_t copy = g_TRUE_c;

    if(profileId == g_ALL_ACTIVE_POWER_PROFILES_c ){
        /* cost info of all active profiles */
        for(loopIndex = 0x00; loopIndex < g_MAXIMUM_NO_OF_PROFILES_c;
            loopIndex++){
            if(g_TRUE_c == ProfileWithCost[loopIndex].active ){
                if(responseType == g_GetPowerProfilePriceExtendedResponseCommand_c
                   && pStartTime != g_NULL_c){

                   if(g_MEMORY_MATCH_c == memUtils_memCompare(pStartTime,
                        (uint8_t*)&(ProfileWithCost[loopIndex].startTimeMaintainedByClient) ,
                        g_DURATION_FIELD_SIZE_c) ){
                        /* Time Matched */
                        copy = g_TRUE_c;
                   }
                   else {
                       /* StartTime Field is present but Time is mismatched*/
                       copy = g_FALSE_c;
                   }
                }
                else{
                    /* It could be Price Response or startTime field
                    is not present */
                    copy = g_TRUE_c;
                }

                if(copy == g_TRUE_c){
                    /* Time Might be matched for price extended request or
                    it might be response for Price Req command */
                    memUtils_memCopy(
                        &(tempData[loopIndex * sizeof(PowerProfilePriceResponsePayload_t)]),
                   (uint8_t*)&(ProfileWithCost[loopIndex].profilePriceInfo),
                                     sizeof(PowerProfilePriceResponsePayload_t) );

                    payloadLength += sizeof(PowerProfilePriceResponsePayload_t);
                }
            }
        }
    }
    else{
        /*Cost info of specific profile  */
        for(loopIndex = 0x00; loopIndex < g_MAXIMUM_NO_OF_PROFILES_c;
            loopIndex++){
            if((g_TRUE_c == ProfileWithCost[loopIndex].active) &&
               (ProfileWithCost[loopIndex].profilePriceInfo.powerProfileId == profileId  )){

                if(responseType == g_GetPowerProfilePriceExtendedResponseCommand_c
                   && pStartTime != g_NULL_c){

                   if(g_MEMORY_MATCH_c == memUtils_memCompare(pStartTime,
                    (uint8_t*)&(ProfileWithCost[loopIndex].startTimeMaintainedByClient) ,
                    g_DURATION_FIELD_SIZE_c) ){
                        /* Time Matched */
                        copy = g_TRUE_c;
                   }
                   else {
                        /* StartTime Field is present but Time is mismatched*/
                        copy = g_FALSE_c;
                   }
                }
                else{
                    /* It could be Price Response or startTime field
                    is not present */
                    copy = g_TRUE_c;
                }

               if(copy == g_TRUE_c){
                   memUtils_memCopy(tempData,(uint8_t*)&(ProfileWithCost[loopIndex].profilePriceInfo),
                                     sizeof(PowerProfilePriceResponsePayload_t) );
                    payloadLength = sizeof(PowerProfilePriceResponsePayload_t);
               }
            /* Need cost price of only one profile*/
            break;
            }
        }
    }
    apsdeDataPacket->a_asdu[0] = g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    if(serverToClient)
    {
        apsdeDataPacket->a_asdu[0] |= g_ZCL_SERVER_TO_CLIENT_DIRECTION_c;

    }
    else
    {
        apsdeDataPacket->a_asdu[0] |= g_ZCL_CLIENT_TO_SERVER_DIRECTION_c;
    }

    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = responseType;


    if ( 0x00 == payloadLength )
    {
        /* Profile Not Found */
        apsdeDataPacket->a_asdu[3] = g_ZCL_Not_Found_c;
    }

    memUtils_memCopy(destAddress , apsdeDataPacket->src_address.IEEE_address, 0x08 );

    if(payloadLength  > 0x00){
        memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[3]) ,tempData,
                             payloadLength);
    }

    payloadLength += 3;
    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_POWER_PROFILE_CLUSTER_c,payloadLength ,
        apsdeDataPacket->a_asdu );

}

/******************************************************************************/
void createAndSendOverAllSchPriceResponse(APSDE_Data_Indication_t *apsdeDataPacket,
    uint8_t responseType, bool serverToClient)
{
    uint8_t loopIndex = 0x00 ,payloadLength = 0x00 , temp = 0x00;
    uint8_t destAddress[8];
    OverallSchedulePriceResponsePayload_t response;
    uint16_t profileStopTime = 0x0000;
    uint32_t avgPrice;


    /* This to be take care for creating response:
        Response = to send overall cost associated to all profiles
        1.Profile shd be active
        2.profile shd have been scheduled for within 24 hours*/

    for(loopIndex = 0x00; loopIndex < g_MAXIMUM_NO_OF_PROFILES_c;
        loopIndex++){
        /* checking whether profile is active or not */
        if(g_TRUE_c == ProfileWithCost[loopIndex].active){

            /* to convert array value into uint16 value*/
            profileStopTime =
                ProfileWithCost[loopIndex].stopTimeMaintainedByClient[1];
            profileStopTime <<= 8;
            temp = ProfileWithCost[loopIndex].stopTimeMaintainedByClient[0];
            profileStopTime |= temp;


            if(profileStopTime < g_24_HOURS_IN_MINUTES_c){
                /* all specified conditioned is passed so incrementing
                the counter*/
                profileCount++;
                avgPrice = calculateAverageCost(loopIndex );

            }/*end of if */
        }/* end of if */

    }/* end of for  */


    if(profileCount){
        /* dummy value is assigned , application will take care*/
        response.currency[0] = 0x00;
        response.currency[1] = 0x00;

        response.priceTrailingDigit = 0x00;
        /* 0x04 is price field size */
        memUtils_memReverseCopy((uint8_t*)&(response.price),(uint8_t*)&avgPrice ,0x04);

        payloadLength = sizeof(OverallSchedulePriceResponsePayload_t);

        /* doing reset of value */
        Overallcost = 0x00000000;
        profileCount = 0x00;
        ArrayTracker = 0x00;
    }
    else{
        payloadLength = 0x00;
    }

    apsdeDataPacket->a_asdu[0] = g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    if(serverToClient){
        apsdeDataPacket->a_asdu[0] |= g_ZCL_SERVER_TO_CLIENT_DIRECTION_c;

    }
    else{
        apsdeDataPacket->a_asdu[0] |= g_ZCL_CLIENT_TO_SERVER_DIRECTION_c;
    }

    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = responseType;

    if(payloadLength == 0x00){
        apsdeDataPacket->a_asdu[3] = g_ZCL_Not_Found_c;
        payloadLength += 1;
    }
//    else{
//        /* Profile Not Found */
//        apsdeDataPacket->a_asdu[3] = g_ZCL_Success_c;
//        payloadLength += 1;
//    }

    memUtils_memCopy(destAddress , apsdeDataPacket->src_address.IEEE_address, 0x08 );

    if(payloadLength  > 0x00){
        memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[3]),
                         (uint8_t*)&response,
                             payloadLength);
    }

    payloadLength += 3;
    App_SendData( apsdeDataPacket->src_addr_mode,
        destAddress,apsdeDataPacket->src_endpoint,
        g_POWER_PROFILE_CLUSTER_c,payloadLength ,
        apsdeDataPacket->a_asdu );

}

#endif /* g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1 */

/******************************************************************************/
void App_PowerProfileReset(void)
{
	uint8_t loop_index = 0x00;
	uint8_t loop_index2 = 0x00;

    for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
    {
		/* Default Profile ID's*/
		defaultProfileIdArray[loop_index] = loop_index;
    }

    for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
		ActiveProfiles[loop_index] = loop_index+1;
		if (loop_index > 2)
		{
			ActiveProfiles[loop_index] = 0xFF;
		}
	}

    for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
    	ActiveProfilesWithTimeConstraints[loop_index].profileId = loop_index+1;
    	ActiveProfilesWithTimeConstraints[loop_index].startAfter[0] = 0;
    	ActiveProfilesWithTimeConstraints[loop_index].startAfter[1] = 0;
    	ActiveProfilesWithTimeConstraints[loop_index].stopBefore[0] = 0;
    	ActiveProfilesWithTimeConstraints[loop_index].stopBefore[1] = 0;
	}

    for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
    	ProfilesWithStateAndPhase[loop_index].profileId = loop_index+1;
    	ProfilesWithStateAndPhase[loop_index].profileState = g_POWER_PROFILE_IDLE_c;
    	ProfilesWithStateAndPhase[loop_index].energyPhaseId = 1;
    	if (loop_index > 2)
		{
    		ProfilesWithStateAndPhase[loop_index].profileId = 0xFF;
			ProfilesWithStateAndPhase[loop_index].profileState = g_POWER_PROFILE_IDLE_c;
			ProfilesWithStateAndPhase[loop_index].energyPhaseId = 0xFF;
		}
	}

    /* This variable keeps track of active profiles count */
    ActiveProfilesCount = 0x00;

    for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
    	if (loop_index > 2)
    	{
    		LookUpTableForEneryPhasesCount[loop_index][0] = 0xFF;
    	}
    	else
    	{
    		LookUpTableForEneryPhasesCount[loop_index][0] = 0x03;
    	}
	}

    for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
    	for(loop_index2 = 0x00; loop_index2 < g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c;loop_index2++)
    	{
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][0] = 0x01;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][1] = 0x01;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][2] = 0x00;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][3] = 0x05;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][4] = 0x00;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][5] = 0x02;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][6] = 0x00;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][7] = 0x0a;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][8] = 0x00;
    		ActiveEnergyPhasesWithActiveProfiles[loop_index][loop_index2][9] = 0x02;
    	}
	}

#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
	ResponseLen = 0x00;
	//TempBuffer[80];

	for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
		if (loop_index > 2)
		{
			serverUpdate[loop_index].profileId = 0xFF;
		}
		else
		{
			serverUpdate[loop_index].profileId = loop_index+1;
		}
		serverUpdate[loop_index].scheduled = g_FALSE_c;
		serverUpdate[loop_index].currentTime[0] = 0x00;
		serverUpdate[loop_index].currentTime[1] = 0x00;

		for(loop_index2 = 0x00; loop_index2 < g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c;loop_index2++)
		{
			serverUpdate[loop_index].energyPhaseStartTime[loop_index2].energyPhaseId = loop_index2+1;
			serverUpdate[loop_index].energyPhaseStartTime[loop_index2].scheduledTime[0] = 0x00;
			serverUpdate[loop_index].energyPhaseStartTime[loop_index2].scheduledTime[1] = 0x00;
		}
	}

	for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
		PreviousCurrentTime[loop_index] = 0x00000000;
	}
#endif

#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
	for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
		memUtils_memSet((uint8_t *)&ProfileWithCost[loop_index].profilePriceInfo, 0xFF, sizeof(PowerProfilePriceResponsePayload_t));
		ProfileWithCost[loop_index].active = g_FALSE_c;
		memUtils_memSet((uint8_t *)&ProfileWithCost[loop_index].startTimeMaintainedByClient, 0x00, 2);
		memUtils_memSet((uint8_t *)&ProfileWithCost[loop_index].stopTimeMaintainedByClient, 0xFF, 2);
		ProfileWithCost[loop_index].profileState = g_POWER_PROFILE_IDLE_c;
	}

	for(loop_index = 0x00; loop_index < g_MAXIMUM_NO_OF_PROFILES_c;loop_index++)
	{
		for(loop_index2 = 0x00; loop_index2 < g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c;loop_index2++)
		{
			memUtils_memSet((uint8_t *)&ActiveEnrPhasesWithActiveProfilesCli[loop_index][loop_index2], 0xFF, sizeof(AboutProfileEnergyPhaseClient_t));
		}
	}

	CostArrayTracker = 0x00;
	Overallcost = 0x00;

    /* This Both Attributes are updated in client through ZCL read response or
       through reporting*/
    MultipleSchAttrMaintainedByClient = g_FALSE_c;
    RemoteControlAttrMaintainedByClient = g_FALSE_c;

    profileCount = 0x00;
    ArrayTracker = 0x00;

#endif

}

/******************************************************************************/
#if(g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d == 1)
static uint32_t calculateAverageCost(uint8_t loopIndexToPrice)
{
    uint64_t tempPrice = 0x0000000000000000;

    /* Same Type of currency is assumed.Based on currency ,calculating
    aggregate is left to application*/

    /* Based on trailing digit calculating aggregate is left to application */

    /* assumption is price is only having mantissa part.*/

    /* 0x04 is size of price field */
    memUtils_memReverseCopy((uint8_t *)&price[ArrayTracker],
        (uint8_t*)&(ProfileWithCost[loopIndexToPrice].profilePriceInfo.price),0x04);


    if(profileCount > 0 ){
        for(uint8_t loopIndex = 0x00 ; loopIndex < profileCount ;loopIndex++){
            tempPrice += price[loopIndex];
        }
        Overallcost = (tempPrice/profileCount);
        ArrayTracker++;
    }

    /* overallcost contains average of current Profile count */
    return Overallcost;
}
#endif /*g_POWER_PROFILE_CLIENT_CLUSTER_ENABLE_d */

/******************************************************************************/
#if(g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1)
static bool lookForRequestedProfile(uint8_t profileId)
{
    uint8_t loopIndex = 0x00;
    bool requestIdPresent = g_FALSE_c;

    for( ; loopIndex < ActiveProfilesCount ; loopIndex++){
        if(ActiveProfiles[loopIndex] == profileId &&
           (profileId != g_INVALID_POWER_PROFILE_ID_c) ){
            requestIdPresent = g_TRUE_c;
            break;
        }
    }
    return requestIdPresent;
}

/******************************************************************************/
static void createAndSendProfileResponse(APSDE_Data_Indication_t *apsdeDataPacket,
                                  uint8_t profileId,uint8_t responseType )
{

    APSDE_Data_Indication_t tempApsdeDataPacket;
    uint8_t bufferIndex = 0x00;
    APSDE_Request_t* pDataReq = g_NULL_c;
    APSDE_Data_Request_t * pAppDataReq = g_NULL_c;

    uint8_t destAddress[8];
    uint8_t payloadLength = 0x00;
        /* Check the current count of energy phase */
    uint8_t energyPhasesCount = 0x00;

    /* Power Profile Id */
    if(profileId == g_ALL_ACTIVE_POWER_PROFILES_c){
       /* in case of multiple profiles the server shall send multiple messages,
        one for each Power Profile */

        /* creating back up of information about the requested device */
        if(apsdeDataPacket != g_NULL_c){

            memUtils_memCopy((uint8_t*)&tempApsdeDataPacket,
                             (uint8_t*)apsdeDataPacket,
                             sizeof(APSDE_Data_Indication_t));
        }
    }
    /* Power Profile Id value == 0x00 indicates all active profiles of
    the device */
    if((profileId == g_ALL_ACTIVE_POWER_PROFILES_c) ||
           (responseType == g_PowerProfileNotificationCommand_c)){

        /* Each time buffer is allocated which will be free in APS_APSDErequest
        function*/
        for(uint8_t loopIndex = 0x00;loopIndex < ActiveProfilesCount ;loopIndex++){

            /* reset bcoz it is used again for next profile */
            payloadLength = 0x00;

            bufferIndex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
            if( g_NO_AVAILABLE_BUFFER_c != bufferIndex ) {
#if (DEBUG_TEST == 1)
                /* Buffer ID --> 88 */
                allocatedbuffer[bufferIndex]= 88;
                freebuffer[bufferIndex][0]= 88;
                freebuffer[bufferIndex][1]= 0;
#endif
                pDataReq =
                    (APSDE_Request_t *)buffMgmt_getBufferPointer( bufferIndex );

                pDataReq->msgType = g_APSDE_Data_Request_c;
                pAppDataReq =
                    (APSDE_Data_Request_t *)&((pDataReq)->APSDE_Request_Primitive.apsde_data_request);

                if(responseType == g_PowerProfileNotificationCommand_c){
                    /*Notification is a broadcast packet */
                    pAppDataReq->dest_addr_mode = g_SHORT_ADDR_MODE_c;
                    pAppDataReq->dest_address.short_address = 0xffff;
                    pAppDataReq->src_endpoint =
                        ZCL_Callback_Get_Device_EndPoint();
                }
                else{

                    pAppDataReq->dest_addr_mode = tempApsdeDataPacket.src_addr_mode;

                    if(pAppDataReq->dest_addr_mode == g_Short_Address_Mode_c ){
                        memUtils_memCopy (
                            (uint8_t *)&(pAppDataReq->dest_address.short_address),
                          ( uint8_t *)&(tempApsdeDataPacket.src_address.short_address),
                          g_SHORT_ADDRESS_LENGTH_c );

                    }
                    else if(pAppDataReq->dest_addr_mode == g_IEEE_Address_Mode_c) {
                        memUtils_memCopy ( pAppDataReq->dest_address.IEEE_address,
                                  ( uint8_t *)&(tempApsdeDataPacket.src_address.IEEE_address),
                                  g_EXTENDED_ADDRESS_LENGTH_c );
                    }

                    pAppDataReq->src_endpoint = tempApsdeDataPacket.dest_endpoint;
                    pAppDataReq->dest_endpoint = tempApsdeDataPacket.src_endpoint;
                }
                pAppDataReq->profile_id = g_APP_PROFILE_ID_c;
                pAppDataReq->cluster_id = g_POWER_PROFILE_CLUSTER_c;

                /* ZCL Header Creation */
                pAppDataReq->asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                        g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                            g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
                pAppDataReq->asdu[1] = App_GetTransSeqNumber();
                pAppDataReq->asdu[2] = responseType;
                //RP:pAppDataReq->asdu[3] = status;

                payloadLength = 3;
                if((ActiveProfiles[ loopIndex ] != 0xff ) ){
                    /* Check the current count of energy phase */
                    uint8_t energyPhasesCount = 0x00;

                    /* Total Payload Length = 1 + 1 + 1 +
                        (Num of Transfered Phases * 10)*/

                    /* Total no of profiles supported by the device */
                    pAppDataReq->asdu[3] = ActiveProfilesCount;
                    /* Profile Id */
                    pAppDataReq->asdu[4] = ActiveProfiles[ loopIndex];
                    /* no of transferred energy phases */
                    energyPhasesCount = LookUpTableForEneryPhasesCount[loopIndex][0];
                    pAppDataReq->asdu[5] = energyPhasesCount;

                    payloadLength += 3;
                    /* Copy complete energy phases of the profile ,
                    each energy phase size is 10 bytes */
                    for( uint8_t j = 0x00 ; j < energyPhasesCount; j++){
                        memUtils_memCopy((uint8_t*)&(pAppDataReq->asdu[6+(j*(sizeof(AboutEachEnergyPhase_t)))]),
                        (uint8_t*)&(ActiveEnergyPhasesWithActiveProfiles[loopIndex][j][0]),
                        (sizeof(AboutEachEnergyPhase_t)));

                        payloadLength += sizeof(AboutEachEnergyPhase_t);
                    }
                }
                pAppDataReq->asdulength = payloadLength;
                pAppDataReq->txoptions =  App_getClusterSecurity( g_POWER_PROFILE_CLUSTER_c );
                pAppDataReq->radius = g_DEFAULT_RADIUS_c;

                //APS_APSDErequest(bufferIndex);
                APS_APSDE_request(bufferIndex,pAppDataReq );


            }/*end of buffer-if*/
        }
    }
    else{
        /* For Specific Profile which is requested */
        /* ZCL Header Creation */
        apsdeDataPacket->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                    g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
        apsdeDataPacket->a_asdu[2] = responseType;
        //RP:apsdeDataPacket->a_asdu[3] = status;

        payloadLength += 3;
        memUtils_memCopy(destAddress,apsdeDataPacket->src_address.IEEE_address,
                         0x08 );

        if((ActiveProfiles[ profileId - 1  ] != 0xff ) ){

            /* Total Payload Length = 1 + 1 + 1 +
                (Num of Transfered Phases * 10)*/

            /* Total no of profiles supported by the device */
            apsdeDataPacket->a_asdu[3] = ActiveProfilesCount;
            /* Profile Id */
            apsdeDataPacket->a_asdu[4] = ActiveProfiles[(profileId - 1)];
            /* no of transferred energy phases */
            energyPhasesCount = LookUpTableForEneryPhasesCount[profileId - 1][0];
            apsdeDataPacket->a_asdu[5] = energyPhasesCount;

            payloadLength += 3;
            /* Copy complete energy phases of the profile ,
            each energy phase size is 10 bytes */
            for( uint8_t i = 0x00 ; i< energyPhasesCount; i++){
                memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[6 +
                                             (i*sizeof(AboutEachEnergyPhase_t))]),
                (uint8_t*)&(ActiveEnergyPhasesWithActiveProfiles[profileId - 1][i][0]),
                    (sizeof(AboutEachEnergyPhase_t)));

                payloadLength += sizeof(AboutEachEnergyPhase_t);

            }
        }

        App_SendData( apsdeDataPacket->src_addr_mode,
            destAddress,apsdeDataPacket->src_endpoint,
            g_POWER_PROFILE_CLUSTER_c,payloadLength ,
            apsdeDataPacket->a_asdu );
    }
}

/******************************************************************************/
static void createAndSendStateResponse(APSDE_Data_Indication_t *apsdeDataPacket,
                                  uint8_t profileId,uint8_t responseType )
{
    uint8_t bufferIndex = g_NO_AVAILABLE_BUFFER_c ;
    PowerProfileStateResponsePayload_t stateResponse;
    APSDE_Request_t* pDataReq = g_NULL_c;
    APSDE_Data_Request_t * pAppDataReq = g_NULL_c;

    PowerProfileRecord_t temprecord[g_MAXIMUM_NO_OF_PROFILES_c] =
    {{ 0xff,0xff,0x00,g_POWER_PROFILE_IDLE_c}};
    uint8_t payloadLength = 0x00;
    uint8_t destAddress[8];
    uint8_t num_of_profiles = 0;
    uint8_t rsp_idx = 4;

    if(ActiveProfilesCount > 0x00){

        /* Power Profile Id */
        if(profileId == g_ALL_ACTIVE_POWER_PROFILES_c ||
            responseType == g_PowerProfilesStateNotificationCommand_c ){

            if(responseType != g_PowerProfilesStateNotificationCommand_c){
				apsdeDataPacket->a_asdu[3] = ActiveProfilesCount;
				num_of_profiles = ActiveProfilesCount;

            }
            else
            {
            	num_of_profiles = 0x01;
            }
            payloadLength = 0x01;
            for(uint8_t loopIndex = 0x00 ;loopIndex < num_of_profiles;/*ActiveProfilesCount;*/
                loopIndex++){
                temprecord[loopIndex].powerProfileId = ActiveProfiles[loopIndex];
                payloadLength += 1;
                if(ProfilesWithStateAndPhase[loopIndex].profileId ==
                   temprecord[loopIndex ].powerProfileId){
                    temprecord[loopIndex ].energyPhaseId =
                        ProfilesWithStateAndPhase[loopIndex].energyPhaseId ;
                    payloadLength += 1;

#if(g_ENERGY_REMOTE_ATTRIBUTE_ENABLE_c == 1)
                    App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                                g_POWER_PROFILE_CLUSTER_c,
                                g_ENERGY_REMOTE_ATTRIBUTE_c,
                                (uint8_t*)&temprecord[loopIndex].powerProfileRemoteControl,
                                g_Read_Attribute_Data_c );
                    payloadLength += 1;
#endif
                    temprecord[loopIndex ].profileState =
                        ProfilesWithStateAndPhase[loopIndex].profileState ;
                    payloadLength += 1;

                    /* All active profile info is appended to the payload*/
                    if(responseType != g_PowerProfilesStateNotificationCommand_c){
                    	apsdeDataPacket->a_asdu[rsp_idx++] = temprecord[loopIndex].powerProfileId;
                    	apsdeDataPacket->a_asdu[rsp_idx++] = temprecord[loopIndex ].energyPhaseId;
                    	apsdeDataPacket->a_asdu[rsp_idx++] = temprecord[loopIndex].powerProfileRemoteControl;
                    	apsdeDataPacket->a_asdu[rsp_idx++] = temprecord[loopIndex ].profileState;

//                        memUtils_memCopy((uint8_t*)&
//                            (apsdeDataPacket->a_asdu[4+(loopIndex * sizeof(PowerProfileRecord_t ))]),
//                                         (uint8_t*)&(temprecord[loopIndex]),
//                                         sizeof(PowerProfileRecord_t ));
                    }

                    //payloadLength += sizeof(PowerProfileRecord_t );
                } /* end of if*/

            }/* end of for*/

            if(responseType == g_PowerProfilesStateNotificationCommand_c){
                /* need to allocate new buffer for notification,
                cannot use same buffer */
                bufferIndex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
                if( g_NO_AVAILABLE_BUFFER_c != bufferIndex ) {
#if (DEBUG_TEST == 1)
                    /* Buffer ID --> 98 */
                    allocatedbuffer[bufferIndex]= 98;
                    freebuffer[bufferIndex][0]= 98;
                    freebuffer[bufferIndex][1]= 0;
#endif
                    pDataReq =
                        (APSDE_Request_t *)buffMgmt_getBufferPointer( bufferIndex );

                    pDataReq->msgType = g_APSDE_Data_Request_c;
                    pAppDataReq =
                        (APSDE_Data_Request_t *)&((pDataReq)->APSDE_Request_Primitive.apsde_data_request);

                    /*Notification is a broadcast packet */
                    pAppDataReq->dest_addr_mode = g_SHORT_ADDR_MODE_c;
                    pAppDataReq->dest_address.short_address = 0xffff;
                    pAppDataReq->src_endpoint = ZCL_Callback_Get_Device_EndPoint();

                    pAppDataReq->profile_id = g_APP_PROFILE_ID_c;
                    pAppDataReq->cluster_id = g_POWER_PROFILE_CLUSTER_c;

                    /* ZCL Header Creation */
                    pAppDataReq->asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                            g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
                    pAppDataReq->asdu[1] = App_GetTransSeqNumber();
                    pAppDataReq->asdu[2] = responseType;
                    //pAppDataReq->asdu[3] = status;


                    pAppDataReq->asdu[3] = num_of_profiles ;//ActiveProfilesCount;


                    /* All active profile info is appended to the payload*/

                    /* Note : Same payload length is used */
                    memUtils_memCopy((uint8_t*)&
                        (pAppDataReq->asdu[4]),(uint8_t*)&(temprecord[0]),
                                     payloadLength );

                    payloadLength += 0x03;
                    pAppDataReq->asdulength = payloadLength;
                    pAppDataReq->txoptions =  App_getClusterSecurity( g_POWER_PROFILE_CLUSTER_c );
                    pAppDataReq->radius = g_DEFAULT_RADIUS_c;

                    //APS_APSDErequest(bufferIndex);
                    APS_APSDE_request(bufferIndex,pAppDataReq );

                }
                /* Packet payload is complete*/
                return;
            }/* end of if*/

        }/* end of if*/
        else{
            /* Specific profile*/
            for(uint8_t loopIndex2 = 0x00; loopIndex2 < ActiveProfilesCount;
                loopIndex2++)    {
                if(profileId ==  ActiveProfiles[loopIndex2]){
                    /* Only one profile info is appended */
                    apsdeDataPacket->a_asdu[3] = 0x01;
                    payloadLength = 0x01;
                    temprecord[0].powerProfileId = ActiveProfiles[loopIndex2];
                    if(ProfilesWithStateAndPhase[loopIndex2].profileId ==
                       temprecord[0].powerProfileId){

                        temprecord[0 ].energyPhaseId =
                            ProfilesWithStateAndPhase[loopIndex2].energyPhaseId ;
#if(g_ENERGY_REMOTE_ATTRIBUTE_ENABLE_c == 1)
                        App_ReadWriteAttribute( ZCL_Callback_Get_Device_EndPoint(),
                                g_POWER_PROFILE_CLUSTER_c,
                                g_ENERGY_REMOTE_ATTRIBUTE_c,
                                (uint8_t*)&temprecord[0].powerProfileRemoteControl,
                                g_Read_Attribute_Data_c );
#endif
                        temprecord[0].profileState =
                            ProfilesWithStateAndPhase[loopIndex2].profileState ;

                        memUtils_memCopy((uint8_t*)&
                        (apsdeDataPacket->a_asdu[4]),
                                     (uint8_t*)&(temprecord[0]),
                                     sizeof(PowerProfileRecord_t ));

                        payloadLength += sizeof(PowerProfileRecord_t );

                        /* Because Need only one profile info*/
                        break;
                    }/* end of if*/
                }/* end of if*/
            }/*end of for*/
        }/* end of if*/
    }/* end of if*/
    else{
        stateResponse.powerProfileCount = 0x00;
        payloadLength = 0x01;
        apsdeDataPacket->a_asdu[3] = stateResponse.powerProfileCount;
    }

    /* For Specific Profile which is requested */
        /* ZCL Header Creation */
    apsdeDataPacket->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                            g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
    apsdeDataPacket->a_asdu[2] = responseType;
    //apsdeDataPacket->a_asdu[3] = status;


    memUtils_memCopy(destAddress,apsdeDataPacket->src_address.IEEE_address,
                     0x08 );

    payloadLength += 3;
    App_SendData( apsdeDataPacket->src_addr_mode,
            destAddress,apsdeDataPacket->src_endpoint,
            g_POWER_PROFILE_CLUSTER_c,payloadLength ,
            apsdeDataPacket->a_asdu );

}
/******************************************************************************/
static void createAndSendScheduledStateResponse(
        APSDE_Data_Indication_t *apsdeDataPacket,
                                  uint8_t profileId,uint8_t responseType )
{
    UpdatesMaintainedByServer_t tempData[g_MAXIMUM_NO_OF_PROFILES_c];
    EnergyPhasesScheduleStateResponse_t energyPhaseScheduleStateRsp;
    uint8_t loopIndex = 0x00,loopIndex2 = 0x00;
    uint8_t profileCount =0x00;
    uint8_t payloadLength = 0x00;
    bool profileMatched = g_FALSE_c;
            uint8_t destAddress[8];

    APSDE_Data_Indication_t tempApsdeDataPacket;
    uint8_t bufferIndex = g_NO_AVAILABLE_BUFFER_c, status = g_ZCL_Success_c ;
    APSDE_Data_Request_t * pAppDataReq = g_NULL_c;
    APSDE_Request_t* pDataReq = g_NULL_c;

    for(loopIndex = 0x00; loopIndex < g_MAXIMUM_NO_OF_PROFILES_c;loopIndex++){

        if((profileId == serverUpdate[loopIndex].profileId)||
           (profileId == g_ALL_ACTIVE_POWER_PROFILES_c ) ){

            if(serverUpdate[loopIndex].profileId != 0xff) {

				tempData[profileCount].profileId = serverUpdate[loopIndex].profileId;
				energyPhaseScheduleStateRsp.profileId = serverUpdate[loopIndex].profileId;
				energyPhaseScheduleStateRsp.numOfScheduledPhases = g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c;

				for(loopIndex2 = 0x00;
					loopIndex2 < g_MAXIMUM_NO_OF_ENERGY_PHASES_PER_PROFILE_c;
					loopIndex2++){

					if(profileId == serverUpdate[loopIndex].profileId){

						memUtils_memCopy((uint8_t*)&
						(tempData[profileCount].energyPhaseStartTime[loopIndex2]),
						(uint8_t*)&(serverUpdate[loopIndex].energyPhaseStartTime[loopIndex2]),
						sizeof(UpdateOfEnergyPhase_t));

						//energyPhaseScheduleStateRsp.energyPhase[loopIndex2] =

						memUtils_memCopy((uint8_t*)&
												(energyPhaseScheduleStateRsp.energyPhase[loopIndex2]),
												(uint8_t*)&(serverUpdate[loopIndex].energyPhaseStartTime[loopIndex2]),
												sizeof(UpdateOfEnergyPhase_t));

						profileMatched = g_TRUE_c;

					}
					else{

						memUtils_memCopy((uint8_t*)&
						(tempData[profileCount].energyPhaseStartTime[loopIndex2]),
						(uint8_t*)&(serverUpdate[loopIndex].energyPhaseStartTime[loopIndex2]),
						sizeof(UpdateOfEnergyPhase_t));

					}
				}
				profileCount = profileCount+1;
            }
        }
        if(profileMatched){
            /*bcoz need single response*/
            profileCount = 0x01;
            break;
        }
    }

    /* Power Profile Id */
    if(profileId == g_ALL_ACTIVE_POWER_PROFILES_c){
       /* in case of multiple profiles the server shall send multiple messages,
        one for each Power Profile */

        /* creating back up of information about the requested device */
        if(apsdeDataPacket != g_NULL_c){

            memUtils_memCopy((uint8_t*)&tempApsdeDataPacket,
                             (uint8_t*)apsdeDataPacket,
                             sizeof(APSDE_Data_Indication_t));
        }
    }

    /* Power Profile Id value == 0x00 indicates all active profiles of
    the device */
    if((profileId == g_ALL_ACTIVE_POWER_PROFILES_c) ||
           (responseType == g_EnergyPhasesScheduleStateNotificationCommand_c)){

    /* Each time buffer is allocated which will be free in APS_APSDErequest
        function*/
        for(uint8_t loopIndex = 0x00;loopIndex < profileCount ;loopIndex++){

            /* reset bcoz it is used again for next profile */
            payloadLength = 0x00;

            bufferIndex = buffMgmt_allocateBuffer ( g_LARGE_BUFFER_c );
            if( g_NO_AVAILABLE_BUFFER_c != bufferIndex ) {
#if (DEBUG_TEST == 1)
                /* Buffer ID --> 88 */
                allocatedbuffer[bufferIndex]= 88;
                freebuffer[bufferIndex][0]= 88;
                freebuffer[bufferIndex][1]= 0;
#endif
                pDataReq =
                    (APSDE_Request_t *)buffMgmt_getBufferPointer( bufferIndex );

                pDataReq->msgType = g_APSDE_Data_Request_c;
                pAppDataReq =
                    (APSDE_Data_Request_t *)&((pDataReq)->APSDE_Request_Primitive.apsde_data_request);


                if(responseType == g_EnergyPhasesScheduleStateNotificationCommand_c){
                    /*Notification is a broadcast packet */
                    pAppDataReq->dest_addr_mode = g_SHORT_ADDR_MODE_c;
                    pAppDataReq->dest_address.short_address = 0xffff;
                    pAppDataReq->src_endpoint =
                        ZCL_Callback_Get_Device_EndPoint();
                }
                else{

                    pAppDataReq->dest_addr_mode = tempApsdeDataPacket.src_addr_mode;

                    if(pAppDataReq->dest_addr_mode == g_Short_Address_Mode_c ){
                        memUtils_memCopy (
                            (uint8_t *)&(pAppDataReq->dest_address.short_address),
                          ( uint8_t *)&(tempApsdeDataPacket.src_address.short_address),
                          g_SHORT_ADDRESS_LENGTH_c );

                    }
                    else if(pAppDataReq->dest_addr_mode == g_IEEE_Address_Mode_c) {
                        memUtils_memCopy ( pAppDataReq->dest_address.IEEE_address,
                                  ( uint8_t *)&(tempApsdeDataPacket.src_address.IEEE_address),
                                  g_EXTENDED_ADDRESS_LENGTH_c );
                    }

                    pAppDataReq->src_endpoint = tempApsdeDataPacket.dest_endpoint;
                    pAppDataReq->dest_endpoint = tempApsdeDataPacket.src_endpoint;
                }
                pAppDataReq->profile_id = g_APP_PROFILE_ID_c;
                pAppDataReq->cluster_id = g_POWER_PROFILE_CLUSTER_c;


                 /* ZCL Header Creation */
                pAppDataReq->asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                        g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                            g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
                pAppDataReq->asdu[1] = App_GetTransSeqNumber();
                pAppDataReq->asdu[2] = responseType;



                if(profileCount){
                    //status = g_ZCL_Success_c;

                    payloadLength = sizeof(UpdatesMaintainedByServer_t);
                    memUtils_memCopy((uint8_t*)&(pAppDataReq->asdu[3]),(uint8_t*)&(tempData[loopIndex]),
                                     payloadLength);
                }
                else{
                    status = g_ZCL_Not_Found_c;
                }
                payloadLength += 0x03;
               // pAppDataReq->asdu[3] = status;
                pAppDataReq->asdulength = payloadLength;
                pAppDataReq->txoptions =  App_getClusterSecurity( g_POWER_PROFILE_CLUSTER_c );
                pAppDataReq->radius = g_DEFAULT_RADIUS_c;

                //APS_APSDErequest(bufferIndex);
                APS_APSDE_request(bufferIndex,pAppDataReq );

            }/* end of allocate buffer*/
        }/* end of for loop*/

    }/*end of if*/
    else{
        /* For Specific Profile which is requested */
        /* ZCL Header Creation */
    	payloadLength = 0x00;
        apsdeDataPacket->a_asdu[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c |
                                g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                                    g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
        apsdeDataPacket->a_asdu[1] = App_GetTransSeqNumber();
        apsdeDataPacket->a_asdu[2] = responseType;
        if(profileCount){
            //status = g_ZCL_Success_c;

           // memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[3]),
            //                 (uint8_t*)&(tempData[0]),sizeof(UpdatesMaintainedByServer_t));
          //  payloadLength = sizeof(UpdatesMaintainedByServer_t);



        	memUtils_memCopy((uint8_t*)&(apsdeDataPacket->a_asdu[3]),
        	                             (uint8_t*)&(energyPhaseScheduleStateRsp),sizeof(EnergyPhasesScheduleStateResponse_t));
        	payloadLength = sizeof(EnergyPhasesScheduleStateResponse_t);


        }
        else{
            status = g_ZCL_Not_Found_c;
            payloadLength = 0x00;
        }

       // apsdeDataPacket->a_asdu[3] = status;
        payloadLength += 0x03;

        memUtils_memCopy(destAddress,apsdeDataPacket->src_address.IEEE_address,
                         0x08 );
        App_SendData( apsdeDataPacket->src_addr_mode,
            destAddress,apsdeDataPacket->src_endpoint,
            g_POWER_PROFILE_CLUSTER_c,payloadLength ,
            apsdeDataPacket->a_asdu );

    }
}
/******************************************************************************/
static bool timeConstraintsValid(uint8_t profile_id,uint8_t *pDuration,uint8_t length)
{
/* 1. Check whether Profile Started or not
   2. If started don't notify new start time ,notify only new finish time
      only if it is valid.
      If not started can set new starttime @ server side and can send same to
      client device.
   3. If it is multiple energy phases supporting device,and new finish time
      is set ,then new finish time shd not overwrite timing of any of the
      inbetween energy phases of the current profile which is running.
   4. Finish Time shd always be greater than startime.
   5. If all condition passes,Then duration provided by
    application/Interface is valid and it is updated @ the server side.*/


    uint8_t durationValid = g_FALSE_c ,
        tempData[sizeof(PowerProfileScheduleConstraints_t)];
    uint8_t profileId;
    /* Constraints set by the user to start profile after the specified time*/
    uint8_t startAfter[2] = {0x00 ,0x00};
    uint16_t startTime = 0x0000;
    /* Constraints set by the user to stop profile before the specified time */
    uint8_t stopBefore[2] = { 0xff,0xff};
    uint16_t endTime = 0xffff;
    uint8_t temp = 0xff;

    if(pDuration != g_NULL_c){
        memUtils_memCopy(tempData,pDuration,length);

        profileId = profile_id;

        /* 1st and 2nd Byte start Time */
        memUtils_memCopy(startAfter,(uint8_t*)&tempData[0],
                         g_DURATION_FIELD_SIZE_c);
        /*1st byte*/
        temp = startAfter[0];
        startTime = temp ;
        /* 2nd byte */
        temp = startAfter[1];
        startTime = startTime << 8;
        startTime |= temp;


        if(length >= 5){
            /* 3rd and 4th byte Finish Time*/
            memUtils_memCopy(stopBefore,(uint8_t*)&tempData[2],
                             g_DURATION_FIELD_SIZE_c);
            temp = stopBefore[0];
            endTime = temp;

            temp = stopBefore[1];
            endTime <<= 8;
            endTime |=temp ;
        }

        if(startTime > endTime){
            return durationValid;
        }

        for(uint8_t loopIndex = 0x00 ; loopIndex < g_MAXIMUM_NO_OF_PROFILES_c;
            loopIndex++){

            if(ProfilesWithStateAndPhase[loopIndex].profileId == profileId){

                /* Before Starting a profile Start time could be modified */
                if(ProfilesWithStateAndPhase[loopIndex].profileState == g_POWER_PROFILE_IDLE_c ||
                    ProfilesWithStateAndPhase[loopIndex].profileState == g_POWER_PROFILE_PROGRAMMED_c ||
                    ProfilesWithStateAndPhase[loopIndex].profileState == g_ENERGY_PHASE_WAITING_TO_START_c ||
                        ProfilesWithStateAndPhase[loopIndex].profileState == g_ENERGY_PHASE_WAITING_PAUSED_c){

                    for(uint8_t i = 0x00 ; i < g_MAXIMUM_NO_OF_PROFILES_c;i++){
                        /* Updating the Starttime */
                        if(ActiveProfilesWithTimeConstraints[i].profileId == profileId){
                            /* Setting StarTime*/
//                            memUtils_memCopy(ActiveProfilesWithTimeConstraints[i].startAfter,
//                                             (uint8_t*)&tempData[0],
//                             g_DURATION_FIELD_SIZE_c);

                             memUtils_memCopy(ActiveProfilesWithTimeConstraints[i].startAfter,
                                             (uint8_t*)&tempData[0],
                             g_DURATION_FIELD_SIZE_c);


                            /* Set end Time only if it is greater than previous
                             (incase it is not 0xffff)*/
                            /* initializing with default value*/
                            uint16_t previousEndTime = 0xffff;
                            temp = ActiveProfilesWithTimeConstraints[i].stopBefore[0];
                            previousEndTime = temp;
                            previousEndTime <<= 8;
                            temp = ActiveProfilesWithTimeConstraints[i].stopBefore[1];
                            previousEndTime |= temp;
                            /* checking new endtime is greater than previous one
                            bcoz it might interrrupt in between energy phases
                            which is already scheduled .
                            But can overwrite (it is left to application)

                            start----->EP1---->EP2---->end
                            <--------Profile--------------->
                            */
                            if((previousEndTime != 0xffff) &&( previousEndTime < endTime)){
                                memUtils_memCopy(ActiveProfilesWithTimeConstraints[i].stopBefore,
                                                 (uint8_t*)&tempData[2],
                                 g_DURATION_FIELD_SIZE_c);
                            }
                            durationValid = g_TRUE_c;
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    return durationValid;
}

#endif /* g_POWER_PROFILE_SERVER_CLUSTER_ENABLE_d == 1*/

/*******************************************************************************
                                End of Power Profile Cluster.c
*******************************************************************************/
