/*******************************************************************************
* AppPriceUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting price cluster
*
*******************************************************************************/
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Common.h"
#include "stack_common.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "memory_utility.h"
#include "sw_timer.h"
#include "zdo_interface.h"
#include "AppTimerUtilities.h"
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
#include "ApplicationThinLayer.h"
#include "ZCL_Price_Cluster.h"
#include "AppPriceClusterUtilities.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

scheduledPublishPriceInfo_t aPriceTable[ g_PRICE_MAX_PRICES_EVENTS_c ];
#if ( g_SE_PROFILE_1_1_d == 1 )
scheduledPublishBlockPeriodInfo_t aBlockPeriodTable[g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c];
#endif


#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
void AppPublishPriceTick( void );
#if ( g_SE_PROFILE_1_1_d == 1 )
void AppPublishBlockPeriodTick( void );
#endif
#endif

void AppPriceactivePeriodTick(void);
#if ( g_SE_PROFILE_1_1_d == 1 )
void AppBlockactivePeriodTick(void);
#endif


/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/


static PublishPriceCmdTxDetails_t PublishPriceCmdTxDetails;


#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )

static uint8_t scheduledPriceSendingIndex = g_PRICE_INVALID_INDEX_c;
static uint32_t scheduledPriceSendingTime = 0x00;
static uint8_t numScheduledPricesLeftToSend = 0x00;

#if ( g_SE_PROFILE_1_1_d == 1 )
static uint8_t numScheduledBlockPeriodsLeftToSend;
static uint32_t scheduledBlockPeriodSendingTime = 0x00;
static uint8_t scheduledBlockPeriodSendingIndex = g_PRICE_INVALID_INDEX_c;

static uint8_t App_getScheduledBlockPeriodCount( void );
#endif
static uint8_t App_getScheduledPriceCount( void );
#endif

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

void App_Update_TxDetails( APSDE_Data_Indication_t *pBuffer )
{
    PublishPriceCmdTxDetails.InterPANMode = g_FALSE_c;
    PublishPriceCmdTxDetails.dstAddrMode = pBuffer->src_addr_mode;
	if( pBuffer->src_addr_mode == g_IEEE_Address_Mode_c ) {
    	memUtils_memCopy( ( uint8_t *)&PublishPriceCmdTxDetails.dstAddr,
               pBuffer->src_address.IEEE_address, g_EXTENDED_ADDRESS_LENGTH_c );
	}
    PublishPriceCmdTxDetails.dstEndPoint = pBuffer->src_endpoint;
    PublishPriceCmdTxDetails.tranSeqNum = pBuffer->a_asdu[1];
}

/******************************************************************************/
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void App_Update_InterPanTxDetails( APSDE_InterPan_Data_Indication_t* pApsdeDataInd )
{
    PublishPriceCmdTxDetails.InterPANMode = g_TRUE_c;
    PublishPriceCmdTxDetails.dstAddrMode = pApsdeDataInd->srcaddrmode;
	if( pApsdeDataInd->srcaddrmode == g_IEEE_Address_Mode_c ) {
    	memUtils_memCopy( ( uint8_t *)&PublishPriceCmdTxDetails.dstAddr,
               pApsdeDataInd->srcaddress.IEEE_address, g_EXTENDED_ADDRESS_LENGTH_c );
	}
    PublishPriceCmdTxDetails.dstInterPANId = pApsdeDataInd->srcpanid;
    PublishPriceCmdTxDetails.tranSeqNum = pApsdeDataInd->a_asdu[1];

}
#endif
/******************************************************************************/

void App_PriceEventsHandler( ZCL_Event_t *pZCLEvent, APSDE_Data_Indication_t *pBuffer )
{
	uint8_t status = g_ZCL_Success_c;
	App_Update_TxDetails( pBuffer );

#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
	if (  ( pZCLEvent->eventId ) == g_GetCurrentPriceEvent_c ){
			App_PriceGetCurrentPriceHandler();
	}
	else if (  ( pZCLEvent->eventId ) == g_GetScheduledPricesEvent_c ) {
			status = AppPriceGetScheduledPricesHandler(
				  ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->startTime,
				 ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->numberOfEvents );

	  }
#if ( g_SE_PROFILE_1_1_d == 1 )
	else if (  ( pZCLEvent->eventId ) == g_GetBlockPeriodsEvent_c ) {
			status = AppPriceGetBlockPeriodHandler(
				  ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->startTime,
				 ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->numberOfEvents );
	}
#endif	   /* g_SE_PROFILE_1_1_d*/

#if ( g_SE_PROFILE_1_2_d == 1 )
	else if (  ( pZCLEvent->eventId ) == g_GetConversionFactorEvent_c ) {
			status = AppPriceGetConversionFactHandler(
				  ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->startTime,
				 ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->numberOfEvents );

	}
	else if (  ( pZCLEvent->eventId ) == g_GetCalorificValueEvent_c ) {
			status = AppPriceGetCalorificValueHandler(
				  ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->startTime,
				 ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->numberOfEvents );

	}
#endif	   /* g_SE_PROFILE_1_2_d*/
#endif     /*g_PRICE_SERVER_CLUSTER_ENABLE_d */

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )

	if (  ( pZCLEvent->eventId ) == g_PublishPriceEvent_c ) {
			status =  App_Update_PriceEventList(
							(scheduledPublishPriceInfo_t*)pZCLEvent->pEventData);

	}
#if ( g_SE_PROFILE_1_1_d == 1 )
	else if (  ( pZCLEvent->eventId ) == g_PublishBlockPeriodEvent_c ) {

			status =  App_Update_BlockPeriodEventList(
						(scheduledPublishBlockPeriodInfo_t*)pZCLEvent->pEventData );

	}
#endif	 /* g_SE_PROFILE_1_1_d*/
	if(status != g_ZCL_Success_c) {
				uint8_t asdulength;
				uint8_t asdu[10];
				asdulength = ZCL_CreateDefaultResponseCommand ( pBuffer->a_asdu,
																		asdu,
																		status );

				App_SendData ( PublishPriceCmdTxDetails.dstAddrMode,
										PublishPriceCmdTxDetails.dstAddr,
										PublishPriceCmdTxDetails.dstEndPoint,
										g_PRICE_CLUSTER_c,
										asdulength,
										asdu );
			}
#endif   /*g_PRICE_CLIENT_CLUSTER_ENABLE_d */
}

/**************************************************************************************/
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void App_InterPanPriceEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_InterPan_Data_Indication_t *pBuffer )
{
  	uint8_t status;
	App_Update_InterPanTxDetails( pBuffer );

#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
	  if( ( pZCLEvent->eventId )== g_GetCurrentPriceEvent_c)
     {
			App_PriceGetCurrentPriceHandler();
	 }
	  else if( ( pZCLEvent->eventId )== g_GetScheduledPricesEvent_c){
			status = AppPriceGetScheduledPricesHandler(
				  ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->startTime,
			 ((getScheduledPriceInfo_t*)pZCLEvent->pEventData)->numberOfEvents );
			if(status != g_ZCL_Success_c) {
				uint8_t asdulength;
				uint8_t asdu[10];
				asdulength = ZCL_CreateDefaultResponseCommand ( pBuffer->a_asdu,
																		asdu,
																		status );

				App_SendData ( PublishPriceCmdTxDetails.dstAddrMode,
										PublishPriceCmdTxDetails.dstAddr,
										PublishPriceCmdTxDetails.dstEndPoint,
										g_PRICE_CLUSTER_c,
										asdulength,
										asdu );
			}
	  }
#endif

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
	   if( ( pZCLEvent->eventId )== g_PublishPriceEvent_c)
     {
			if ( ! ((publishPriceInfo_t*)pZCLEvent->pEventData)->startTime ) {
				((publishPriceInfo_t*)pZCLEvent->pEventData)->startTime =
				  ZCL_CallBackGetCurrentTime();
			}
			status =App_Update_PriceEventList( (scheduledPublishPriceInfo_t*)pZCLEvent->pEventData );
			if(status != g_ZCL_Success_c) {
				uint8_t asdulength;
				uint8_t asdu[10];
				asdulength = ZCL_CreateDefaultResponseCommand ( pBuffer->a_asdu,
																		asdu,
																		status );

				App_SendData ( PublishPriceCmdTxDetails.dstAddrMode,
										PublishPriceCmdTxDetails.dstAddr,
										PublishPriceCmdTxDetails.dstEndPoint,
										g_PRICE_CLUSTER_c,
										asdulength,
										asdu );
			}
	 }
#endif
}
#endif
/******************************************************************************/

#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
void App_PriceTableInit( void )
{
	uint8_t i;
	for( i=0; i< g_PRICE_MAX_PRICES_EVENTS_c;i++)
	{
		App_PriceInit ( &aPriceTable[i]);
	}

}
/******************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )
void App_BlockPeriodTableInit(void)
{
	uint8_t i;
	for( i=0; i< g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c;i++)
	{
		App_BlockInit ( &aBlockPeriodTable[i]);
	}
}
/******************************************************************************/
void App_BlockInit( scheduledPublishBlockPeriodInfo_t *block)
{

  block->providerId                        = 0x00000000;
  block->issuerEventId                     = 0x00000000;
  block->blockPeriodStartTime                         = 0xFFFFFFFF;
  memUtils_memSet( (uint8_t*) block->blockPeriodDurationInMinutes, 0xFF,  3);
  block->numberOfPriceTiersAndBlockThresholds           = 0xFF;
  block->blockPeriodControl                      = 0x00;
  block->endTime	                       = 0x00000000;
  block->valid                             = g_FALSE_c;
  block->active                            = g_FALSE_c;
}
#endif
/******************************************************************************/
void App_PriceInit( scheduledPublishPriceInfo_t *price)
{

  price->providerId                        = 0x00000000;
  memUtils_memSet( (uint8_t*)price->rateLabel , 0x00,  sizeof( uint8_t )* 12 );
  price->issuerEventId                     = 0x00000000;
  price->currentTime                       = 0x00000000;
  price->unitOfMeasure                     = 0x00;
  price->currency                          = 0x0000;
  price->priceTrailingDigitAndPriceTier    = 0x00;
  price->numOfPriceTiersAndRegisterTier    = 0x00;
  price->startTime                         = 0xFFFFFFFF;    // 0xFFFFFFFF
  price->duration                          = 0xFFFF;        // 0xFFFF
  price->price                             = 0x00000000;
  price->priceRatio                        = 0xFF;
  price->generationPrice                   = 0xFFFFFFFF;
  price->generationPriceRatio              = 0xFF;
  price->alternateCostDelivered            = 0xFFFFFFFF;
  price->alternateCostUnit                 = 0xFF;
  price->alternateCostTrailingDigit        = 0xFF;
#if ( g_SE_PROFILE_1_1_d == 1 )  
  price->numberOfBlockThresholds           = 0xFF;
  price->priceControl                      = 0x00;
#endif //#if ( g_SE_PROFILE_1_1_d == 1 )  
  price->endTime	                       = 0x00000000;
  price->valid                             = g_FALSE_c;
  price->active                            = g_FALSE_c;
}


/**************************************************************************************/
scheduledPublishPriceInfo_t* App_GetCurrentPrice( void )
{

	uint8_t PriceIndex ;
	uint32_t currPricestartTime;
	uint32_t currPriceEndTime;
	uint32_t currentTime;

	currentTime = ZCL_CallBackGetCurrentTime();
	for ( PriceIndex = 0; PriceIndex < g_PRICE_MAX_PRICES_EVENTS_c; PriceIndex++ ) {
	  if( aPriceTable[PriceIndex].valid ==g_TRUE_c) {

			currPricestartTime =(( aPriceTable[PriceIndex].startTime == g_START_TIME_NOW_c )?
	 							 currentTime : aPriceTable[PriceIndex].startTime);
			currPriceEndTime =  (( aPriceTable[PriceIndex].duration == g_PRICE_DURATION_UNTIL_CHANGED_c )?
							  g_PRICE_END_TIME_NEVER_c : ( currPricestartTime  +
									 ( aPriceTable[PriceIndex].duration * 60 ) ));
				if ( ( currPricestartTime < currentTime ) &&
											( currPriceEndTime > currentTime ) ) {
					break ;
				}
	  }
	}
	if ( PriceIndex == g_PRICE_MAX_PRICES_EVENTS_c ) {
		return g_NULL_c;
	}
	/* Returns the index in the price table of the current price.  The first price
	in the table that starts in the past and ends in the future in considered
	the current price.	*/
	return &aPriceTable[PriceIndex];
}
#endif

#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )


/**************************************************************************************/
static uint8_t App_getScheduledPriceCount(void)
{
    uint8_t index;
    uint8_t count = g_ZERO_c;
    for ( index=0; index < g_PRICE_MAX_PRICES_EVENTS_c; index++ ) {
        if ( aPriceTable[index].valid != g_FALSE_c &&
			(  aPriceTable[index].startTime >scheduledPriceSendingTime
			 ||aPriceTable[index].duration == g_PRICE_DURATION_UNTIL_CHANGED_c )) {
            count++;
        }
    }
    return count;
}
/**************************************************************************************/

scheduledPublishPriceInfo_t* App_PriceGetPriceTableEntry( uint8_t index)
{
    if ( index < g_PRICE_MAX_PRICES_EVENTS_c ) {
        return &aPriceTable[index];
    }
    return g_NULL_c;
}

/**************************************************************************************/
void App_PriceGetCurrentPriceHandler( void)
{
    scheduledPublishPriceInfo_t * pPublishTable;

    pPublishTable = App_GetCurrentPrice();
    if ( pPublishTable == g_NULL_c ) {
        return;
    }
    App_PriceSendPublishPriceCommand((publishPriceInfo_t *)pPublishTable );
}

/**************************************************************************************/

uint8_t AppPriceGetScheduledPricesHandler( uint32_t startTime, uint8_t numEvents )
{
    scheduledPriceSendingIndex = 0;
    scheduledPriceSendingTime = startTime;
	uint8_t scheduledPriceCount =0;

	if ( ! scheduledPriceSendingTime ) {
            scheduledPriceSendingTime = ZCL_CallBackGetCurrentTime();
    }
	if(!(scheduledPriceCount =App_getScheduledPriceCount())){
			scheduledPriceSendingIndex = g_PRICE_INVALID_INDEX_c;
			return g_ZCL_Not_Found_c;
	  }

    if ( numEvents == 0 ) {
        numScheduledPricesLeftToSend = scheduledPriceCount;
    }
    else {

		numScheduledPricesLeftToSend = numEvents;
   	}

   return g_ZCL_Success_c;

}
/**************************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )
static uint8_t App_getScheduledBlockPeriodCount( void )
{
    uint8_t index;
    uint8_t count = g_ZERO_c;

	for ( index=0; index < g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c; index++ ) {
        if ( aBlockPeriodTable[index].valid != g_FALSE_c &&
			aBlockPeriodTable[index].blockPeriodStartTime > scheduledPriceSendingTime  ) {
            count++;
        }
    }
    return count;
}

/**************************************************************************************/

scheduledPublishBlockPeriodInfo_t* App_PriceGetBlockPeriodTableEntry( uint8_t index)
{
    if ( index < g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c ) {
        return &aBlockPeriodTable[index];
    }
    return g_NULL_c;
}
/******************************************************************************/

uint8_t AppPriceGetBlockPeriodHandler( uint32_t startTime, uint8_t numEvents )
{
    scheduledBlockPeriodSendingIndex = 0;
    scheduledBlockPeriodSendingTime = startTime;
	uint8_t scheduledBlockCount =0;

	if(!(scheduledBlockCount =App_getScheduledBlockPeriodCount())){
			scheduledBlockPeriodSendingIndex = g_PRICE_INVALID_INDEX_c;
			return g_ZCL_Not_Found_c;
	  }

    if ( numEvents == 0 ) {
        numScheduledBlockPeriodsLeftToSend = scheduledBlockCount;
    }
    else {

		numScheduledBlockPeriodsLeftToSend = numEvents;
   	}

   return g_ZCL_Success_c;
}
#endif /* ( g_SE_PROFILE_1_1_d == 1 ) */

#if ( g_SE_PROFILE_1_2_d == 1 )
/******************************************************************************/
uint8_t AppPriceGetConversionFactHandler( uint32_t startTime, uint8_t numEvents )
{
    scheduledBlockPeriodSendingIndex = 0;
    scheduledBlockPeriodSendingTime = startTime;
	uint8_t scheduledBlockCount =0;

	if(!(scheduledBlockCount =App_getScheduledBlockPeriodCount())){
			scheduledBlockPeriodSendingIndex = g_PRICE_INVALID_INDEX_c;
			return g_ZCL_Not_Found_c;
	  }

    if ( numEvents == 0 ) {
        numScheduledBlockPeriodsLeftToSend = scheduledBlockCount;
    }
    else {

		numScheduledBlockPeriodsLeftToSend = numEvents;
   	}

   return g_ZCL_Success_c;
}
/*******************************************************************************/
uint8_t AppPriceGetCalorificValueHandler( uint32_t startTime, uint8_t numEvents )
{
    scheduledBlockPeriodSendingIndex = 0;
    scheduledBlockPeriodSendingTime = startTime;
	uint8_t scheduledBlockCount =0;

	if(!(scheduledBlockCount =App_getScheduledBlockPeriodCount())){
			scheduledBlockPeriodSendingIndex = g_PRICE_INVALID_INDEX_c;
			return g_ZCL_Not_Found_c;
	  }

    if ( numEvents == 0 ) {
        numScheduledBlockPeriodsLeftToSend = scheduledBlockCount;
    }
    else {

		numScheduledBlockPeriodsLeftToSend = numEvents;
   	}

   return g_ZCL_Success_c;
}
#endif     /* g_SE_PROFILE_1_2_d*/
#endif  /* ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ) */
/**************************************************************************************/
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
void App_PriceTick( void )
{

    AppPriceactivePeriodTick();
#if ( g_SE_PROFILE_1_1_d == 1 )
     AppBlockactivePeriodTick();
#endif
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
   AppPublishPriceTick();
#if ( g_SE_PROFILE_1_1_d == 1 )
   AppPublishBlockPeriodTick();
#endif
#endif

}
#endif

/**************************************************************************************/
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )

void AppPublishPriceTick( void )
{
    scheduledPublishPriceInfo_t *pPriceInfo;
    static  uint32_t lastTickTime=0;
    if( App_getcurrentTick() -lastTickTime > 0x04 ) {
      lastTickTime =App_getcurrentTick();
      if ( scheduledPriceSendingIndex != g_PRICE_INVALID_INDEX_c ) {
          if ( ( scheduledPriceSendingIndex < g_PRICE_MAX_PRICES_EVENTS_c ) &&
               ( numScheduledPricesLeftToSend != 0 ) ) {

                  pPriceInfo = App_PriceGetPriceTableEntry( scheduledPriceSendingIndex );

                  while( ( ( pPriceInfo->valid == g_FALSE_c ) ||
                           ( scheduledPriceSendingTime > pPriceInfo->startTime ) ) &&
                           ( scheduledPriceSendingIndex < g_PRICE_MAX_PRICES_EVENTS_c ) ) {

                        scheduledPriceSendingIndex++;
                        pPriceInfo = App_PriceGetPriceTableEntry( scheduledPriceSendingIndex );
                  }
                  if ( ( scheduledPriceSendingIndex < g_PRICE_MAX_PRICES_EVENTS_c ) &&
                       ( scheduledPriceSendingTime <= pPriceInfo->startTime ) ) {

                          App_PriceSendPublishPriceCommand( (publishPriceInfo_t*)pPriceInfo );

                          scheduledPriceSendingIndex++;
                          numScheduledPricesLeftToSend--;
                      }
          }
          else {
              scheduledPriceSendingIndex = g_PRICE_INVALID_INDEX_c;
              scheduledPriceSendingTime = 0x00000000;
              numScheduledPricesLeftToSend = 0x00;
              PublishPriceCmdTxDetails.InterPANMode = g_FALSE_c;
          }
      }
    }
}

/**************************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )
void AppPublishBlockPeriodTick( void )
{
    scheduledPublishBlockPeriodInfo_t *pBlockPeriodInfo;
    static  uint32_t lastTickTime=0;
    if( App_getcurrentTick() -lastTickTime > 0x04 ) {
      lastTickTime =App_getcurrentTick();
		if ( scheduledBlockPeriodSendingIndex != g_PRICE_INVALID_INDEX_c ) {
			if ( ( scheduledBlockPeriodSendingIndex < g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c ) &&
				 ( numScheduledBlockPeriodsLeftToSend > 0 ) ) {

					pBlockPeriodInfo = App_PriceGetBlockPeriodTableEntry( scheduledBlockPeriodSendingIndex );

					while( ( ( pBlockPeriodInfo->blockPeriodStartTime == g_FALSE_c ) ||
							 ( scheduledBlockPeriodSendingTime > pBlockPeriodInfo->blockPeriodStartTime ) ) &&
							 ( scheduledBlockPeriodSendingIndex < g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c ) ) {

						  scheduledBlockPeriodSendingIndex++;
						  pBlockPeriodInfo = App_PriceGetBlockPeriodTableEntry( scheduledBlockPeriodSendingIndex );
					}
					if ( ( scheduledBlockPeriodSendingIndex < g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c ) &&
						 ( scheduledBlockPeriodSendingTime <= pBlockPeriodInfo->blockPeriodStartTime ) ) {

							App_PriceSendPublishBlockPeriodCommand((publishBlockPeriodInfo_t*)pBlockPeriodInfo );

							scheduledBlockPeriodSendingIndex++;
							numScheduledBlockPeriodsLeftToSend--;
						}
			}
			else {
				scheduledBlockPeriodSendingIndex = g_PRICE_INVALID_INDEX_c;
				scheduledBlockPeriodSendingTime = 0x00000000;
				numScheduledBlockPeriodsLeftToSend = 0x00;
			}
		}
	}
}

#endif

/**************************************************************************************/
void App_PriceSendPublishPriceCommand( publishPriceInfo_t *pPriceTable )
{

     uint8_t asdu[50];
     uint8_t asduLength;

     asduLength = ZCL_CreatePublishPriceCmd ( PublishPriceCmdTxDetails.tranSeqNum,
                                         asdu,
                                         pPriceTable );

    if ( PublishPriceCmdTxDetails.InterPANMode == g_FALSE_c ) {

        App_SendData ( PublishPriceCmdTxDetails.dstAddrMode,
                 PublishPriceCmdTxDetails.dstAddr,
                 PublishPriceCmdTxDetails.dstEndPoint,
                 g_PRICE_CLUSTER_c,
                 asduLength,
                 asdu );

    }
#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
    else {
        App_SendInterPanData( PublishPriceCmdTxDetails.dstAddrMode,
                              PublishPriceCmdTxDetails.dstAddr,
                              PublishPriceCmdTxDetails.dstInterPANId,
                              g_PRICE_CLUSTER_c,
                              asduLength,
                              asdu );
    }
#endif
}

/******************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )
void App_PriceSendPublishBlockPeriodCommand( publishBlockPeriodInfo_t *pBlockPeriodTable )
{
     uint8_t asdu[25];
     uint8_t asduLength;

     asduLength = ZCL_CreatePublishBlockPriceCmd ( PublishPriceCmdTxDetails.tranSeqNum,
                                         asdu,
                                         pBlockPeriodTable );

        App_SendData ( PublishPriceCmdTxDetails.dstAddrMode,
                 PublishPriceCmdTxDetails.dstAddr,
                 PublishPriceCmdTxDetails.dstEndPoint,
                 g_PRICE_CLUSTER_c,
                 asduLength,
                 asdu );
}

#endif
#endif

/******************************************************************************/

uint8_t App_Update_PriceEventList( scheduledPublishPriceInfo_t * PriceEvent )
{
    uint32_t currentEndTime;
    uint32_t currentTime = ZCL_CallBackGetCurrentTime();
	uint8_t status =g_ZCL_Failure_c ,i;
	scheduledPublishPriceInfo_t *price = g_NULL_c ,*last = &aPriceTable[0];
#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 && g_SE_PROFILE_1_1_d ==1)

	priceAcknowledgementInfo_t ackPayLoad;
#endif
	if (PriceEvent->startTime == g_START_TIME_NOW_c) {
		PriceEvent->startTime = currentTime;
	}
	currentEndTime = ((PriceEvent->duration == g_PRICE_DURATION_UNTIL_CHANGED_c )
				? g_PRICE_END_TIME_NEVER_c : PriceEvent->startTime + PriceEvent->duration * 60);
	/*If the price has already expired, don't bother with it. */
	if (currentEndTime <= currentTime) {
		status = g_ZCL_Failure_c;
		return status;
	}
	for (i = 0; i < g_PRICE_MAX_PRICES_EVENTS_c; i++) {
	/* if the price is valid, ignore it but remember the empty slot for later. */
		if (!aPriceTable[i].valid) {
			if (price == g_NULL_c) {
				price = &aPriceTable[i];
			}
			continue;
		}
	/* Assume that issuer event ids are unique and that pricing information
		associated with an issuer event id never changes, so reject duplicate prices
		if same event id is receieved*/

		if (aPriceTable[i].issuerEventId == PriceEvent->issuerEventId) {
			status = g_ZCL_Duplicate_Exists_c;
			return status;
		}

		/* Nested and overlapping prices are not allowed */
		/* all nested prices are replaced with newer issuer event*/
		/* all overlapping prices are replaced with newer issuer event*/
		/* The only exception is when a price with a newer issuer event id overlaps
		 with the end of the current active price.  In this case, the duration of
		 the current active price is changed to "until changed" and it will expire
		 when the new price starts. */
		if (aPriceTable[i].startTime < currentEndTime  &&
			aPriceTable[i].endTime > PriceEvent->startTime) {
			if (aPriceTable[i].issuerEventId <PriceEvent->issuerEventId) {

				if (aPriceTable[i].active && currentTime < PriceEvent->startTime) {
				aPriceTable[i].endTime = PriceEvent->startTime;
				aPriceTable[i].duration = g_PRICE_DURATION_UNTIL_CHANGED_c;
				}
				else {
				  App_PriceInit(&aPriceTable[i]);
				}
			}
			else {
				status = g_ZCL_Failure_c;
				return status;
			}
		}
		/*for new price search for an empty slot and find the latest start time
		in the price table. If there are no empty slot, the drop the price
		depending on the start time*/

		if (price == g_NULL_c) {
			if (!aPriceTable[i].valid) {
				price = &aPriceTable[i];
			}
			else if (last->startTime < aPriceTable[i].startTime) {
				last = &aPriceTable[i];
			}
		}
    }
	/* if the price starts after all the price in tha table and there is no
	empty slots, drop the price. Otherwise drop the price with latest start time
	and replace with this one	*/
	if (price == g_NULL_c) {
		if (last->startTime < PriceEvent->startTime) {
			status = g_ZCL_Insufficient_Space_c;
			return status;
		}
		else {
			price = last;
		}
	}

   memUtils_memCopy( (uint8_t*)price,
                  (uint8_t*)PriceEvent, sizeof(scheduledPublishPriceInfo_t) );
	price->endTime =  currentEndTime;
	price->valid                             = g_TRUE_c;
    price->active                            = g_FALSE_c;

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
#if ( g_SE_PROFILE_1_1_d == 1 )
	if(PriceEvent->priceControl & g_PRICE_ACKNOWLEDGEMENT_MASK_c){

		ackPayLoad.providerId    =  PriceEvent->providerId;
		ackPayLoad.issuerEventId =  PriceEvent->issuerEventId;
		ackPayLoad.priceAckTime  =  PriceEvent->currentTime;
		ackPayLoad.control       =  PriceEvent->priceControl;
		App_PriceSendPriceAckCommand (&ackPayLoad);
	}
#endif
#endif
	status =  g_ZCL_Success_c;

	return status;
}
/******************************************************************************/
void AppPriceactivePeriodTick(void)
{
    uint32_t currentTime = ZCL_CallBackGetCurrentTime();
	uint32_t next = g_PRICE_END_TIME_NEVER_c;
    bool active = g_FALSE_c;
    uint8_t i;
	static  uint32_t lastTickTime=0;
	static uint8_t delay = 0;
	if( App_getcurrentTick() -lastTickTime > delay ) {
	lastTickTime =App_getcurrentTick();
	delay =  (uint8_t)randomT();
		for (i = 0; i < g_PRICE_MAX_PRICES_EVENTS_c; i++) {
		if (!aPriceTable[i].valid) {
		  continue;
		}
		/*If the active price is expired , then remove old price from the table */

		if (aPriceTable[i].endTime <= currentTime) {
		  if (aPriceTable[i].active) {
			aPriceTable[i].active = g_FALSE_c; /* price is  expired */

		  }
		/*   initialize the price  */
		 App_PriceInit(&aPriceTable[i]);
		  continue;
		}
	   /*
		we have to activate the next active Price that should start, if there is no
		current active price. Keep track the start time of the next acive price */

		if (!active && aPriceTable[i].startTime < next) {
		  next = aPriceTable[i].startTime;
		}
		 /*If the price is currently active, keep track the of price whether its duration
		 is completed and price is expired*/

		if (aPriceTable[i].startTime <= currentTime) {
		  if (!aPriceTable[i].active) {
			aPriceTable[i].active = g_TRUE_c; 		/* Price is activated */

		  }
		  active = g_TRUE_c;
		  next = aPriceTable[i].endTime;
		}
	  }
	}
}

/******************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )

void AppBlockactivePeriodTick(void)
{
  uint32_t currentTime = ZCL_CallBackGetCurrentTime();
  uint32_t next = g_PRICE_END_TIME_NEVER_c;
  bool active = g_FALSE_c;
  uint8_t i;
  for (i = 0; i < g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c; i++) {
	if (!aBlockPeriodTable[i].valid) {
	  continue;
	}
	/* If the current block period is expired , then remove old blocks from table*/

	if (aBlockPeriodTable[i].endTime <= currentTime) {
	  if (aBlockPeriodTable[i].active) {
		aBlockPeriodTable[i].active = g_FALSE_c; 	/*	price is  expired */
	  }
	  /* initialize the price */
	  App_BlockInit(&aBlockPeriodTable[i]);
	  continue;
	}
	/* Keep track the next active block ,if there is no other active block*/

	if (!active && aBlockPeriodTable[i].blockPeriodStartTime < next) {
	  next = aBlockPeriodTable[i].blockPeriodStartTime;
	}

	/* if the currently active block  is active , keep track the duration of
	currently active block , so that if its duration is complete, notify
	application block is expired*/
	if (aBlockPeriodTable[i].blockPeriodStartTime <= currentTime) {
	  if (!aBlockPeriodTable[i].active) {
		aBlockPeriodTable[i].active = g_TRUE_c;
		/*  block is activated */
	  }
	  active = g_TRUE_c;
	  next = aBlockPeriodTable[i].endTime;
	}
  }
}

/******************************************************************************/

void App_Init_BlockEventList( uint8_t index,scheduledPublishBlockPeriodInfo_t * BlockEvent )
{
    memUtils_memCopy( (uint8_t*)&aBlockPeriodTable[index],
                  (uint8_t*)BlockEvent, sizeof(scheduledPublishBlockPeriodInfo_t) );
	return;
}
#endif  /* ( g_SE_PROFILE_1_1_d == 1 ) */
/******************************************************************************/
#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )

/******************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )
void App_PriceSendPriceAckCommand( priceAcknowledgementInfo_t *pPriceAck)
{
     uint8_t asdu[20];
     uint8_t asduLength;

     asduLength = ZCL_CreatePriceAcknowledgementCmd (
											PublishPriceCmdTxDetails.tranSeqNum,
                                             asdu,
                                              pPriceAck );

     App_SendData ( PublishPriceCmdTxDetails.dstAddrMode,
                 PublishPriceCmdTxDetails.dstAddr,
                 PublishPriceCmdTxDetails.dstEndPoint,
                 g_PRICE_CLUSTER_c,
                 asduLength,
                 asdu );

}


/******************************************************************************/

uint8_t App_Update_BlockPeriodEventList( scheduledPublishBlockPeriodInfo_t * BlockEvent )
{
    uint32_t blockEndTime, blockduration ;
    uint32_t currentTime = ZCL_CallBackGetCurrentTime();
	uint8_t status =g_ZCL_Failure_c ,i;
    scheduledPublishBlockPeriodInfo_t *block = g_NULL_c ,*last = &aBlockPeriodTable[0];;
	priceAcknowledgementInfo_t ackPayLoad;

	if (BlockEvent->blockPeriodStartTime == g_START_TIME_NOW_c) {
		BlockEvent->blockPeriodStartTime = currentTime;
	}

	blockduration = (uint32_t)BlockEvent->blockPeriodDurationInMinutes;
    blockEndTime = (BlockEvent->blockPeriodStartTime
				  +  blockduration* 60);
	 /* If the block has already expired, don't bother with it. */
	if (blockEndTime <= currentTime) {
		status = g_ZCL_Failure_c;
		return status;
	}
    for ( i = 0; i < g_PRICE_MAX_PRICES_EVENTS_c; i++ ) {

	  /*Ignore invalid block and initialise empty slot for next. */
		if (!aBlockPeriodTable[i].valid) {
			if (block == g_NULL_c) {
				block = &aBlockPeriodTable[i];
			}
			continue;
		}
	 /* Reject duplicate blocks based on the issuer event id */

	  if (aBlockPeriodTable[i].issuerEventId == BlockEvent->issuerEventId) {
			status = g_ZCL_Duplicate_Exists_c;
			return status;
		}
        /*

		 Along the way, search for an empty slot for this new price and find the
		 price in the table with the latest start time.  If there are no empty
		 slots, we will either have to drop this price or the last one, depending
		 on the start times.  */
		if (block == g_NULL_c) {
			if (!aBlockPeriodTable[i].valid) {
				block = &aBlockPeriodTable[i];
			}
			else if (last->blockPeriodStartTime < aBlockPeriodTable[i].blockPeriodStartTime) {
				last = &aBlockPeriodTable[i];
			}
		}

    }
	/* If there were no empty slots and this price starts after all of the other
	 prices in the table, drop this price.  Otherwise, drop the price with the
	 latest start time and replace it with this one. */
	if (block == g_NULL_c) {
		if (last->blockPeriodStartTime < BlockEvent->blockPeriodStartTime) {
			status = g_ZCL_Insufficient_Space_c;
			return status;
		}
		else {
			block = last;
		}
	}

	memUtils_memCopy( (uint8_t*)block,
                  (uint8_t*)BlockEvent, sizeof(scheduledPublishPriceInfo_t) );
	aBlockPeriodTable[i].endTime =  blockEndTime;
	block->valid                             = g_TRUE_c;
    block->active                            = g_FALSE_c;

	if(BlockEvent->blockPeriodControl & g_PRICE_ACKNOWLEDGEMENT_MASK_c){

		ackPayLoad.providerId    =  BlockEvent->providerId;
		ackPayLoad.issuerEventId =  BlockEvent->issuerEventId;
		ackPayLoad.priceAckTime  =  currentTime;
		ackPayLoad.control       =  BlockEvent->blockPeriodControl;
		App_PriceSendPriceAckCommand (&ackPayLoad);
	}
	status =  g_ZCL_Success_c;
	return status;
}
#endif
#endif

/**************************************************************************************/
uint8_t App_GetCurrentTier( void )
{
    scheduledPublishPriceInfo_t* pPublishTable;
    pPublishTable = App_GetCurrentPrice();

    if ( g_NULL_c == pPublishTable ) {
        return 0xFF;
    }
    else {
        return pPublishTable->priceTrailingDigitAndPriceTier & 0x0F;
    }
}
/**************************************************************************************/
#if ( g_SE_PROFILE_1_1_d == 1 )
uint8_t App_GetCurrentBlock( void )
{
    scheduledPublishBlockPeriodInfo_t* pblockPeriodTable;
	uint8_t index ;
    uint32_t currBlockstartTime;
    uint32_t currBlockEndTime,blockduration;
    uint32_t currentTime;

    currentTime = ZCL_CallBackGetCurrentTime();
    for ( index = 0; index < g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c; index++ ) {
      if ( aBlockPeriodTable[index].valid != g_FALSE_c ) {
          currBlockstartTime = aBlockPeriodTable[index].blockPeriodStartTime;
		  blockduration = (uint32_t) aBlockPeriodTable[index].blockPeriodDurationInMinutes;
    	  currBlockEndTime = ( aBlockPeriodTable[index].blockPeriodStartTime
				  +  blockduration* 60);
          if ( ( currBlockstartTime < currentTime ) && ( currBlockEndTime > currentTime ) ) {
              break ;
          }
      }
    }
    if ( index == g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c ) {
        return g_NULL_c;
    }
/* Returns the index in the price table of the current price.  The first price
 in the table that starts in the past and ends in the future in considered
 the current price.	*/
    pblockPeriodTable= &aBlockPeriodTable[index];
    if ( g_NULL_c == pblockPeriodTable ) {
        return 0xFF;
    }
    else {
        return pblockPeriodTable[index].numberOfPriceTiersAndBlockThresholds & 0x0F;
    }
}

#endif /* ( g_SE_PROFILE_1_1_d == 1 ) */


#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
/**************************************************************************************/
/*App_UpdatePriceTable
 *
 *
 *
 */

/**************************************************************************************/
uint8_t  App_AddEntryToPriceTable(publishPriceInfo_t *pPriceData)
{
    scheduledPublishPriceInfo_t  priceTable;
    uint8_t status = g_ZCL_Failure_c;

/*    memUtils_memCopy( (uint8_t*)&priceTable.providerId, (uint8_t*)&pPriceData->providerId, 0x04 );

	memUtils_memCopy( priceTable.rateLabel, pPriceData->rateLabel, pPriceData->rateLabel[0]  );

	memUtils_memCopy( (uint8_t*)&priceTable.issuerEventId,(uint8_t*)&pPriceData->issuerEventId, 0x04 );

	memUtils_memCopy( (uint8_t*)&priceTable.currentTime,(uint8_t*)&pPriceData->currentTime, 0x04 );

	priceTable.unitOfMeasure = pPriceData->unitOfMeasure;

	memUtils_memCopy( (uint8_t*)&priceTable.currency, (uint8_t*)&pPriceData->currency, 0x02 );

	priceTable.priceTrailingDigitAndPriceTier = pPriceData->priceTrailingDigitAndPriceTier;

	priceTable.numOfPriceTiersAndRegisterTier = pPriceData->numOfPriceTiersAndRegisterTier;

	memUtils_memCopy( (uint8_t*)&priceTable.startTime, (uint8_t*)&pPriceData->startTime, 0x04 );

	memUtils_memCopy( (uint8_t*)&priceTable.duration, (uint8_t*)&pPriceData->duration, 0x02 );

	memUtils_memCopy( (uint8_t*)&priceTable.price,(uint8_t *)& pPriceData->price, 0x04 );

	priceTable.priceRatio = pPriceData ->priceRatio;

	memUtils_memCopy( (uint8_t*)&priceTable.generationPrice,
			(uint8_t*)&pPriceData->generationPrice, 0x04 );

	priceTable.generationPriceRatio = pPriceData->generationPriceRatio;

	memUtils_memCopy( (uint8_t*)&priceTable.alternateCostDelivered,
			(uint8_t*)&pPriceData->alternateCostDelivered, 0x04 );

	priceTable.alternateCostUnit= pPriceData->alternateCostUnit;

	priceTable.alternateCostTrailingDigit= pPriceData->alternateCostTrailingDigit;
*/

    memUtils_memCopy( (uint8_t*)&priceTable, (uint8_t*)pPriceData, sizeof(publishPriceInfo_t ));

#if ( g_SE_PROFILE_1_1_d == 1 )
    priceTable.numberOfBlockThresholds = pPriceData->numberOfBlockThresholds;

	if(pPriceData->priceControl == 0xFF) {
		priceTable.priceControl= 0x00;

	}
	else
	{
		priceTable.priceControl= pPriceData->priceControl;

	}
	memUtils_memSet( (uint8_t*)&priceTable.endTime, 0x00, 0x04 );

	priceTable.valid= g_TRUE_c;
	priceTable.active= g_FALSE_c;
#endif

    if ( ! priceTable.startTime )
    {
    	priceTable.startTime = ZCL_CallBackGetCurrentTime();
    }
    status = App_Update_PriceEventList(&priceTable);
	return status;


}
/**************************************************************************************/
/* App_ResetPriceTable
 *
 * Application Calls this API to reset all entries in Price Table
 * return None
 * input None
 * */

/**************************************************************************************/
void  App_RemoveAllPriceEntries(void)
{
	/* Call App_PriceTableInit, which will reset all entries of price table*/
	App_PriceTableInit();
	return;
}

/**************************************************************************************/
/* App_deleteAnEntryFrmPriceTable
 *
 * Application Calls this API to reset all entries in Price Table
 * return : None
 * */

/**************************************************************************************/
uint8_t  App_RemovePriceEntry(uint32_t providerID , uint32_t issuerEventID)
{
	scheduledPublishPriceInfo_t *ptrPriceTable = g_NULL_c;
	uint8_t index;
	uint8_t status = g_ZCL_Failure_c;

	for ( index =0; index < g_PRICE_MAX_PRICES_EVENTS_c; index++)
	{
		ptrPriceTable = App_PriceGetPriceTableEntry(index);
		if (ptrPriceTable != g_NULL_c)
		{

			if ((ptrPriceTable->providerId == providerID) &&
					(ptrPriceTable->issuerEventId == issuerEventID))
			{
				App_PriceInit ( &aPriceTable[index]);
				status = g_ZCL_Success_c;
				break;
			}
		}
	}
	if(index >= g_PRICE_MAX_PRICES_EVENTS_c)
	{
		status = g_ZCL_Not_Found_c;
	}
	return status;

}
/**************************************************************************************/
uint8_t  App_getPriceCount(void)
{
	uint8_t count = 0x00;
	count = App_getScheduledPriceCount();
	return count;
}

#if ( g_SE_PROFILE_1_1_d == 1 )
/**************************************************************************************/
/*App_UpdateBlockPriceTable
 *
 *
 *
 */

/**************************************************************************************/
uint8_t  App_AddEntryToBlockPriceTable(publishBlockPeriodInfo_t *pBlockPriceData, uint8_t index)
{
	scheduledPublishBlockPeriodInfo_t  blockPriceTable;
	uint8_t status = g_ZCL_Failure_c;

	scheduledPublishPriceInfo_t *ptrPriceTable = g_NULL_c;

	ptrPriceTable = App_GetCurrentPrice();
	if ( (ptrPriceTable!=g_NULL_c) && (ptrPriceTable->numberOfBlockThresholds != 0xff) &&
		  (ptrPriceTable->numberOfBlockThresholds != 0))
	{

		if (index > g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c)
		{
			status = g_ZCL_Insufficient_Space_c;
		}
		else {
			memUtils_memCopy( (uint8_t*)&blockPriceTable.providerId,
									(uint8_t*)&pBlockPriceData->providerId, 0x04);

			memUtils_memCopy( (uint8_t*)&blockPriceTable.issuerEventId,
								(uint8_t*)& pBlockPriceData->issuerEventId, 0x04 );

			memUtils_memCopy( (uint8_t*)&blockPriceTable.blockPeriodStartTime,
											(uint8_t*)&pBlockPriceData->blockPeriodStartTime, 0x04 );

			memUtils_memCopy( (uint8_t*)&blockPriceTable.blockPeriodDurationInMinutes
									, (uint8_t*)&pBlockPriceData->blockPeriodDurationInMinutes, 0x03 );

			blockPriceTable.numberOfPriceTiersAndBlockThresholds=
											pBlockPriceData ->numberOfPriceTiersAndBlockThresholds;

			blockPriceTable.blockPeriodControl= pBlockPriceData->blockPeriodControl;

			memUtils_memSet( (uint8_t*)&blockPriceTable.endTime, 0x00, 0x04 );

			blockPriceTable.valid= g_TRUE_c;
			blockPriceTable.active= g_FALSE_c;
			App_Init_BlockEventList (index, &blockPriceTable);
			status = g_ZCL_Success_c;
		}

	}



    return status;

}

/**************************************************************************************/
/* App_ResetBlockPriceTable
 *
 * Application Calls this API to reset all entries in Price Table
 *
 * */

/**************************************************************************************/
void  App_RemoveAllBlockPriceEntries(void)
{
	/* Call App_BlockPeriodTableInit, which will reset all entries of price table*/
	App_BlockPeriodTableInit();
}

/**************************************************************************************/
/* App_deleteAnEntryFrmPriceTable
 *
 * Application Calls this API to reset all entries in Price Table
 * return : None
 * */

/**************************************************************************************/
uint8_t  App_RemoveBlockPriceEntry(uint32_t providerID , uint32_t issuerEventID)
{
	scheduledPublishBlockPeriodInfo_t *pBlkPriceTable= g_NULL_c;
	uint8_t index;
	uint8_t status = g_ZCL_Failure_c;

	for ( index =0; index < g_PRICE_MAX_PRICES_EVENTS_c; index++)
	{
		pBlkPriceTable = App_PriceGetBlockPeriodTableEntry(index);
		if(pBlkPriceTable != g_NULL_c)
		{
			if ((pBlkPriceTable->providerId) && ( pBlkPriceTable->issuerEventId))
			{
				App_PriceInit ( &aPriceTable[index]);
				status = g_ZCL_Success_c;
				break;
			}

		}
	}
	if(index >= g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c)
	{
		status = g_ZCL_Not_Found_c;
	}
	return status;

}
/**************************************************************************************/
uint8_t  App_getBlkPriceCount(void)
{
	uint8_t count = 0x00;
	count = App_getScheduledBlockPeriodCount();
	return count;
}



#endif   /* g_SE_PROFILE_1_1_d*/
#endif   /* g_PRICE_SERVER_CLUSTER_ENABLE_d*/
/*----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */
#endif          /* ( g_PRICE_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

