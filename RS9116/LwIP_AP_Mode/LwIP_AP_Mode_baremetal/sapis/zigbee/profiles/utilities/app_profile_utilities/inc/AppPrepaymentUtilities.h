/******************************************************************************
* AppPrepaymentUtilities..h
******************************************************************************
* Program Description:
* This file contains the Message cluster App type definations
******************************************************************************/
#ifndef _APP_PREPAYMENT_UTILITIES_H_
#define _APP_PREPAYMENT_UTILITIES_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "ZCL_Functionality.h"
#include "ZCL_Prepayment_Cluster.h"

/******************************************************************************
* Global Constants
******************************************************************************/
#define g_PROPOSED_VALUES_MAX_c     (1)
/******************************************************************************
* Memory Declarations
******************************************************************************/
#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0 )
extern uint8_t storedMatchedEndpoint ;
extern uint16_t storedMatchedAddress ;
extern uint8_t readMeterSerialNo[16];
extern uint8_t readMeterSiteId[16];
#endif
#endif
/******************************************************************************
* Type Definitions
******************************************************************************/
typedef struct ProposedSupplyValues_Tag{
    ChangeSupplyCmdPayload_t supplyCmdValue;
    uint8_t srcAddress[8];   /* needed to keep track of src address of this cmd,
                                bcoz it is helpful in sending response after
                                implementation time */
    uint8_t addressMode;     /* short address or extended address*/
    uint8_t srcEndpoint;     /* source device endpoint */
}__attribute__((__packed__))ProposedSupplyValues_t;

/******************************************************************************
   Function Prototypes
******************************************************************************/
#if ( g_PREPAYMENT_CLUSTER_ENABLE_d == 1 )
void App_PrepaymentEventsHandler( ZCL_Event_t *pZCLEvent,
                                        APSDE_Data_Indication_t* p_dataInd );
#endif

#if ( g_PREPAYMENT_SERVER_CLUSTER_ENABLE_d == 1 )
void App_prepaymentTask();
bool App_changeSupplyState(PrepaymentSupplyStatus newSupplyState);
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 0 )
void App_processSupplyChangeEvent( void );
#endif
#endif
/******************************************************************************
* End Of File
******************************************************************************/
#endif          /* _APP_PREPAYMENT_UTILITIES_H_ */
