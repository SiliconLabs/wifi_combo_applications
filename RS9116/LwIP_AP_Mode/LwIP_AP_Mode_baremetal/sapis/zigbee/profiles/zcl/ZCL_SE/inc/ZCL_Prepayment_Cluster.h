/******************************************************************************
* ZCL_Prepayment_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Prepayment cluster attribute and cmmnd
information
******************************************************************************/

#ifndef _ZCL_PREPAYMENT_CLUSTER_H_
#define _ZCL_PREPAYMENT_CLUSTER_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"

/********************************93**********************************************
* Global Constants
******************************************************************************/
/* Cluster ID of Prepayment Cluster */
#define g_PREPAYMENT_CLUSTER_c                              0x0705

/* Prepayment Cluster Attribute Set information */

/*Prepayment Information Set*/
#define g_PAYMENT_CONTROL_ATTRIBUTE_c                    0x0000
#define g_CREDIT_REMAINING_ATTRIBUTE_c                   0x0001
#define g_EMERGENCY_CREDIT_REMAINING_ATTRIBUTE_c         0x0002
#define g_CREDIT_STATUS_ATTRIBUTE_c                      0x0003

/*Top-up Attribute Set*/
#define g_TOP_UP_DATE_TIME_1_ATTRIBUTE_c                 0x0100
#define g_TOP_UP_AMOUNT_1_ATTRIBUTE_c                    0x0101
#define g_ORIGINATING_DEVICE_1_ATTRIBUTE_c               0x0102
#define g_TOP_UP_DATE_TIME_2_ATTRIBUTE_c                 0x0110
#define g_TOP_UP_AMOUNT_2_ATTRIBUTE_c                    0x0111
#define g_ORIGINATING_DEVICE_2_ATTRIBUTE_c               0x0112
#define g_TOP_UP_DATE_TIME_3_ATTRIBUTE_c                 0x0120
#define g_TOP_UP_AMOUNT_3_ATTRIBUTE_c                    0x0121
#define g_ORIGINATING_DEVICE_3_ATTRIBUTE_c               0x0122
#define g_TOP_UP_DATE_TIME_4_ATTRIBUTE_c                 0x0130
#define g_TOP_UP_AMOUNT_4_ATTRIBUTE_c                    0x0131
#define g_ORIGINATING_DEVICE_4_ATTRIBUTE_c               0x0132
#define g_TOP_UP_DATE_TIME_5_ATTRIBUTE_c                 0x0140
#define g_TOP_UP_AMOUNT_5_ATTRIBUTE_c                    0x0141
#define g_ORIGINATING_DEVICE_5_ATTRIBUTE_c               0x0142

/*Debt Attribute Set*/
#define g_FUEL_DEBT_REMAINING_ATTRIBUTE_c                0x0200
#define g_FUEL_DEBT_RECOVERY_RATE_ATTRIBUTE_c            0x0201
#define g_FUEL_DEBT_RECOVERY_PERIOD_ATTRIBUTE_c          0x0202
#define g_NON_FUEL_DEBT_REMAINING_ATTRIBUTE_c            0x0203
#define g_NON_FUEL_DEBT_RECOVERY_RATE_ATTRIBUTE_c        0x0204
#define g_NON_FUEL_DEBT_RECOVERY_PERIOD_ATTRIBUTE_c      0x0205

/*Supply Control Set*/
#define g_PROPOSED_CHANGE_PROVIDER_ID_ATTRIBUTE_c                0x0300
#define g_PROPOSED_CHANGE_IMPLEMENTATION_TIME_ATTRIBUTE_c        0x0301
#define g_PROPOSED_CHANGE_SUPPLY_STATUS_ATTRIBUTE_c              0x0302
#define g_DELAYED_SUPPLY_INTERRUPT_VALUE_REMAINING_ATTRIBUTE_c   0x0303
#define g_DELAYED_SUPPLY_INTERRUPT_VALUE_TYPE_ATTRIBUTE_c        0x0304


/* payment control attribute*/
#define g_DISCONNECTION_ENABLE_c             0x01 /* bit 0*/
                                                  /* bit 1 reserved */
#define g_CREDIT_MANAGEMENT_ENABLED_c        0x04 /* bit 2*/
                                                  /* bit 3 reserved */
#define g_CREDIT_DISPLAY_ENABLED_c           0x10 /* bit 4*/
                                                  /* bit 5 reserved */
#define g_ACCOUNT_BASE_c                     0x40 /* bit 6*/
#define g_CONTACTOR_FITTED_c                 0x80 /* bit 7*/

/* Credit Status Attribute*/
#define g_CREDIT_OK_c                       0x01 /* bit 0*/
#define g_LOW_CREDIT_c                      0x02 /* bit 1*/
#define g_EMERGENCY_CREDIT_ENABLED_c        0x04 /* bit 2*/
#define g_EMERGENCY_CREDIT_AVAILABLE_c      0x08 /* bit 3*/
#define g_EMERGENCY_CREDIT_SELECTED_c       0x10 /* bit 4*/
#define g_EMERGENCY_CREDIT_IN_USE_c         0x20 /* bit 5*/
#define g_EMERGENCY_CREDIT_EXHAUSTED_c      0x40 /* bit 6*/
#define g_RESERVED_c                        0x80 /* bit 7*/


/* enumeration for Prepayment Cluster Client Side Commands*/
enum prepaymentClusterClientCommands
{
    g_SelectAvailableEmergencyCreditReqCmd_c    = 0x00,
    g_ChangeSupplyReqCmd_c                      = 0x01
};

/* enumeration for Prepayment Cluster Server Side Commands*/
enum prepaymentClusterServerCommands
{
    g_SupplyStatusResCmd_c                   = 0x00
};

/* PrepaymentRecoveryPeriod*/
typedef enum {
    g_PREPAYMENT_RECOVERY_PERIOD_PER_HOUR_c               = 0x00,
    g_PREPAYMENT_RECOVERY_PERIOD_PER_DAY_c                = 0x01,
    g_PREPAYMENT_RECOVERY_PERIOD_PER_WEEK_c               = 0x02,
    g_PREPAYMENT_RECOVERY_PERIOD_PER_MONTH_c              = 0x03,
    g_PREPAYMENT_RECOVERY_PERIOD_PER_QUARTER_c            = 0x04
} PrepaymentRecoveryPeriod;

/* PrepaymentOriginatingDevice*/
typedef enum {
    g_PREPAYMENT_ORIGINATING_DEVICE_ENERGY_SERVICE_INTERFACE_c    = 0x00,
    g_PREPAYMENT_ORIGINATING_DEVICE_IN_PREMISES_DISPLAY_DEVICE_c  = 0x02
} PrepaymentOriginatingDevice;

/* PrepaymentSupplyStatus*/
typedef enum {
    g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_c               = 0x00,
    g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_OFF_ARMED_c         = 0x01,
    g_PREPAYMENT_SUPPLY_STATUS_SUPPLY_ON_c                = 0x02
}PrepaymentSupplyStatus;



/* enumeration for Prepayment Cluster Events */
enum PrepaymentEvents
{
    g_PrepaymentSelectAvailEmerCreditReqEvent_c           = 0xd0,
    g_PrepaymentChangeSupplyReqEvent_c                    = 0xd1,
    g_PrepaymentSupplyStatusResEvent_c
};
#pragma pack(1)
/* Prepayment Cluster Attributes structure */
typedef struct prepaymentAttributes_Tag
{
    /* Payment Control Attribute Set*/
    uint8_t paymentControlAttribute;
    int32_t creditRemainingAttribute;
    int32_t emergencyCreditRemainingAttribute;
    uint8_t creditStatusAttribute;
    /* Top-up Attribute Set*/
    uint32_t topUp1DateTimeAttribute;
    uint8_t topUp1AmountAttribute[6];
    uint8_t topUp1OriginatorDevice;

    uint32_t topUp2DateTimeAttribute;
    uint8_t topUp2AmountAttribute[6];
    uint8_t topUp2OriginatorDevice;

    uint32_t topUp3DateTimeAttribute;
    uint8_t topUp3AmountAttribute[6];
    uint8_t topUp3OriginatorDevice;

    uint32_t topUp4DateTimeAttribute;
    uint8_t topUp4AmountAttribute[6];
    uint8_t topUp4OriginatorDevice;

    uint32_t topUp5DateTimeAttribute;
    uint8_t topUp5AmountAttribute[6];
    uint8_t topUp5OriginatorDevice;
    /* Fuel Debt Attribute Set*/
    uint8_t fuelDebtRemaining[6];
    uint32_t fuelDebtRecoveryRate;
    uint8_t fuelDebtRecoveryPeriod;

    uint8_t nonfuelDebtRemaining[6];
    uint32_t nonfuelDebtRecoveryRate;
    uint8_t nonfuelDebtRecoveryPeriod;
    /* Supply Control Attribute Set */
    uint32_t proposedChangeProviderId;
    uint32_t proposedChangeImplementationTime;
    uint8_t proposedChangeSupplyStatus;
    uint16_t delayedSupplyInterruptValueRemaining;
    uint8_t delayedSupplyInterruptValueType;
}__attribute__((__packed__))prepaymentAttributes_t;

/* Select Available Emergency Credit Command Payload*/
typedef struct SelectAvailableCreditLimitCmdPayload_Tag
{
    uint32_t cmdDateTime;
    uint8_t originatingDevice;
    uint8_t siteId[16];
    uint8_t meterSerialNum[16];
}__attribute__((__packed__))SelectAvailableCreditLimitCmdPayload_t;


/* Supply status Response command payload */
#pragma pack(1)
typedef struct SupplyStatusResCmdPayload_Tag
{
    uint32_t providerID;
    uint32_t implementationDateTime;
    uint8_t supplyStatus;
}__attribute__((__packed__)) SupplyStatusResCmdPayload_t;

/* Change Supply Command Payload */
#pragma pack(1)
typedef struct ChangeSupplyCmdPayload_Tag
{
    uint32_t providerID;
    uint32_t requestDateTime;
    uint8_t siteId[16];
    uint8_t meterSerialNumber[16];
    uint32_t implementationDateTime;
    uint8_t proposedSupplyStatus;
    uint8_t originatorIdSupplyControlBits;
} __attribute__((__packed__))ChangeSupplyCmdPayload_t;

/* end of Cmd Payload*/


/******************************************************************************
* Memory Declarations
******************************************************************************/

#if ( g_PREPAYMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Prepayment_Client_Cluster_Info;
#endif

#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Prepayment_Server_Cluster_Info;
#endif

/******************************************************************************
* Type Definitions
******************************************************************************/

/******************************************************************************
   Function Prototypes
******************************************************************************/
uint8_t ZCL_Handle_Payment_Cluster_Cmd ( uint8_t endPointId,
                                      uint8_t asduLength,
                                      uint8_t* pReceivedAsdu,
                                      uint8_t* pResponseLocation,
                                      ZCL_ActionHandler_t *pActionHandler,
                                      ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                      uint8_t * pAttrsDataLocation );
/******************************************************************************
   Function Prototypes
******************************************************************************/

#endif                                      /*_ZCL_PREPAYMENT_CLUSTER_H_*/
