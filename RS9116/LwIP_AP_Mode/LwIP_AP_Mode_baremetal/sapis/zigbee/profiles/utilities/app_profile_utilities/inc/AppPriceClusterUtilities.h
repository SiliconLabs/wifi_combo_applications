/******************************************************************************
* AppPriceClusterUtilities..h
******************************************************************************
* Program Description:
* This file contains the Price cluster App type definations
******************************************************************************/
#ifndef _APP_PRICE_CLUSTER_UTILITIES_H_
#define _APP_PRICE_CLUSTER_UTILITIES_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Functionality.h"
#include "ZCL_Price_Cluster.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_PRICE_INVALID_INDEX_c                             0xFF
#define g_PRICE_MAX_PRICES_EVENTS_c                         0x03
#define g_PRICE_INVALID_STARTTIME_c                         0xFFFFFFFF
#define g_PRICE_INVALID_DURATION_c                          0xFFFF
#define g_PRICE_DURATION_UNTIL_CHANGED_c                    0xFFFF
#define g_PRICE_END_TIME_NEVER_c                    	    0xFFFFFFFF

#define g_START_TIME_NOW_c                                  0x00000000
#define g_PRICE_MAX_BLOCK_PERIODS_EVENTS_c       	        0x02
#define g_PRICE_ACKNOWLEDGEMENT_MASK_c			            0x01

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct PublishPriceCmdTxDetails_Tag
{
    uint8_t  dstAddrMode;
    uint8_t  dstAddr[8];
    uint16_t dstInterPANId;
    uint8_t  InterPANMode;
    uint8_t  dstEndPoint;
    uint8_t  tranSeqNum;
}__attribute__((__packed__))PublishPriceCmdTxDetails_t;


//#pragma pack(1)
typedef struct scheduledPublishPriceInfo_tag
{
    uint32_t  providerId;
    uint8_t   rateLabel[12];
    uint32_t  issuerEventId;
    uint32_t  currentTime;
    uint8_t   unitOfMeasure;
    uint16_t  currency;
    uint8_t   priceTrailingDigitAndPriceTier;
    uint8_t   numOfPriceTiersAndRegisterTier;
    uint32_t  startTime;
    uint16_t  duration;
    uint32_t  price;
    uint8_t   priceRatio;
    uint32_t  generationPrice;
    uint8_t   generationPriceRatio;
    uint32_t  alternateCostDelivered;
    uint8_t   alternateCostUnit;
    uint8_t   alternateCostTrailingDigit;
#if ( g_SE_PROFILE_1_1_d == 1 )
	uint8_t   numberOfBlockThresholds;
	uint8_t   priceControl;
#endif
	uint32_t   endTime;
	bool      valid;
	bool      active;
}__attribute__((__packed__))scheduledPublishPriceInfo_t;
//#pragma pack()

#if ( g_SE_PROFILE_1_1_d == 1 )
typedef struct scheduledPublishBlockPeriodInfo_tag
{
	uint32_t  providerId;
	uint32_t  issuerEventId;
	uint32_t  blockPeriodStartTime;
	uint8_t   blockPeriodDurationInMinutes[3];
	uint8_t   numberOfPriceTiersAndBlockThresholds;
	uint8_t   blockPeriodControl;
	uint32_t   endTime;
	bool      valid;
	bool      active;
}__attribute__((__packed__))scheduledPublishBlockPeriodInfo_t;
#endif


/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )

void App_PriceGetCurrentPriceHandler(void);
uint8_t AppPriceGetScheduledPricesHandler( uint32_t startTime, uint8_t numEvents );
uint8_t AppPriceGetBlockPeriodHandler( uint32_t startTime, uint8_t numEvents );
scheduledPublishPriceInfo_t* App_PriceGetPriceTableEntry( uint8_t index);
scheduledPublishPriceInfo_t* App_GetCurrentPrice( void );
void App_PriceSendPublishPriceCommand( publishPriceInfo_t *pPriceTable );

#if ( g_SE_PROFILE_1_1_d == 1 )
void App_PriceSendPriceAckCommand( priceAcknowledgementInfo_t *pPriceAck);
void App_PriceSendPublishBlockPeriodCommand( publishBlockPeriodInfo_t
														*pBlockPeriodTable );
scheduledPublishBlockPeriodInfo_t* App_PriceGetBlockPeriodTableEntry( uint8_t index);
#endif  /* g_SE_PROFILE_1_1_d */

#if ( g_SE_PROFILE_1_2_d == 1 )
uint8_t  AppPriceGetConversionFactHandler  ( uint32_t startTime, uint8_t numEvents );
uint8_t  AppPriceGetCalorificValueHandler  ( uint32_t startTime, uint8_t numEvents );

#endif      /*g_SE_PROFILE_1_2_d */
#endif      /* ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ) */



uint8_t App_Update_PriceEventList( scheduledPublishPriceInfo_t * PriceEvent );

#if ( g_SE_PROFILE_1_1_d == 1 )
uint8_t App_Update_BlockPeriodEventList( scheduledPublishBlockPeriodInfo_t * BlockEvent );
#endif  /* ( g_SE_PROFILE_1_1_d == 1 ) */


#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
#if ( g_SE_PROFILE_1_1_d == 1 )
void App_PriceSendPriceAckCommand( priceAcknowledgementInfo_t *pPriceAck);
#endif
#endif  /* ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 ) */

#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
uint8_t App_GetCurrentTier( void );
void App_PriceEventsHandler( ZCL_Event_t *pZCLEvent,
                             APSDE_Data_Indication_t *pBuffer );
void App_PriceTick( void );
void App_PriceTableInit( void );
void App_PriceInit( scheduledPublishPriceInfo_t *price);

#if ( g_SE_PROFILE_1_1_d == 1 )
void App_Init_BlockEventList(uint8_t index, scheduledPublishBlockPeriodInfo_t * BlockEvent );
void App_BlockPeriodTableInit(void);
void App_BlockInit( scheduledPublishBlockPeriodInfo_t *block);
uint8_t App_GetCurrentBlock( void );
#endif   /* ( g_SE_PROFILE_1_1_d == 1 ) */


#if ( g_INTERPAN_DATA_ENABLE_d == 1 )
void App_InterPanPriceEventsHandler( ZCL_Event_t *pZCLEvent,
                                    APSDE_InterPan_Data_Indication_t *pBuffer );
#endif  /* ( g_INTERPAN_DATA_ENABLE_d == 1 ) */
#endif  /* ( g_PRICE_CLUSTER_ENABLE_d == 1 ) */


uint8_t  App_AddEntryToPriceTable(publishPriceInfo_t *pPriceData);
void  App_RemoveAllPriceEntries(void);
uint8_t  App_RemovePriceEntry(uint32_t providerID , uint32_t issuerEventID);
uint8_t  App_getPriceCount(void);

#if ( g_SE_PROFILE_1_1_d == 1 )
uint8_t  App_AddEntryToBlockPriceTable(publishBlockPeriodInfo_t *pBlockPriceData, uint8_t index);
void  App_RemoveAllBlockPriceEntries(void);
uint8_t  App_RemoveBlockPriceEntry(uint32_t providerID , uint32_t issuerEventID);
uint8_t App_getBlkPriceCount(void);
#endif


#endif          /* _APP_PRICE_CLUSTER_UTILITIES_H_ */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
