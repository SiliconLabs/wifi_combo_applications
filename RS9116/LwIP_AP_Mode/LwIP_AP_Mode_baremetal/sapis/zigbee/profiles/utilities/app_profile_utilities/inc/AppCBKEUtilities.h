/******************************************************************************
* AppDRLCClusterUtilities..h
******************************************************************************
* Program Description:
* This file contains the Key Establishment cluster App type definations
******************************************************************************/
#ifndef _APP_CBKE_UTILITIES_H_
#define _APP_CBKE_UTILITIES_H_

#include "eccapi.h"

/*******************************************************************************
* Global Constants
********************************************************************************/
#define g_GET_c                                             g_TRUE_c
#define g_SET_c                                             g_FALSE_c
#define g_CBKE_KEY_ESTABLISHMENT_SUITE_c                    0x0001
/* Location of the device address in the certificate */
#define g_DEVICE_ADDRESS_LOCATION_c                         0x16
/* Location of the issuer address in the certificate */
#define g_ISSUER_ADDRESS_LOCATION_c                         0x1E
#define g_MAX_ADDR_LENGTH_c                                 0x08
#define g_SHORT_ADDR_LENGTH_c                               0x02

/* Time taken by the initiator/responder device to generate the ephemeral public key */
#define g_EPHEMERAL_DATA_GENERATE_TIME_c                0x0d
/* Time taken by the initiator/responder device to generate MACU and MACV */
#define g_CONFIRM_KEY_DATA_GENERATE_TIME_c              0x10

#define g_INITIATE_KEY_RESPONSE_WAIT_TIME_c             0x14
#define g_BROADCAST_DELIVERY_TIMEOUT_c                  0x0A

/* This is the terminate key wait time */
#define g_TERMINATE_KEY_ESTABLISHMENT_WAIT_TIME_c       0x0d //13 seconds
/* Total size of mac key and key data is 32 bytes */
#define g_KEY_DATA_SIZE_c                               0x20

#define g_YIELD_LEVEL_c                                 0x05
/* This is the additional component for initiator and  responder*/
#define g_ADDITIONAL_COMPONENT_FOR_INITIATOR_c          0x02
#define g_ADDITIONAL_COMPONENT_FOR_RESPONDER_c          0x03
#define g_HASH_MAX_LENGTH_c                             0xFF

#define g_CBKE_SUCCESS_c                                0x00
#define g_CBKE_FAILURE_c                                0x01
#define ONE_SECOND                                      0x01
#define g_EVENT_PROCESS_DELAY_TIME_IN_MS_c              300

#define CBKE_ChangeState( state )                       CBKE_KeyEstCurrentState = state
#define CBKE_ChangePreviousState( state )               CBKE_KeyEstPreviousState = state
#define CBKE_GetCurrentState( )                         CBKE_KeyEstCurrentState
#define CBKE_GetPreviousState( )                        CBKE_KeyEstPreviousState

#define g_RAND_M_c                                      100000000
#define g_RAND_M1_c                                     10000
#define g_RAND_B_c                                      31415821

#define g_SELFSTATICPUBLICKEYOFFSET_c			        0
#define g_SELFEPHEMPUBLICKEYOFFSET_c			        22
#define g_SELFEPHEMPRIVATEKEYOFFSET_c			        44
#define g_PARTNERCERTOFFSET_c					        66
#define g_PARTEPHEMPUBLICKEYOFFSET_c			        114
#define g_PARTEPHEMPRIVATEKEYOFFSET_c			        136
#define g_PARTSTATICPUBLICKEYOFFSET_c			        158
#define g_SHAREDSECRETOFFSET_c				            180					
#define g_GENMACKEYKEYDATAOFFSET_c				        202
#define g_GENMACUOFFSET_c						        234
#define g_GENMACVOFFSET_c						        250
#define g_PARTIEEEADDROFFSET_c					        266
#define g_PARTSHORTADDROFFSET_c					        274

typedef enum
{
    INITIAL_STATE                          = 0x00,
    INITIATE_KEY_EST_REQ_STATE             = 0x01,
    PROCESS_KEY_REQ_STATE                  = 0x02,
    PROCESS_KEY_EST_RESP_STATE             = 0x03,
    INITIATE_TERMINATE_KEY_REQ_STATE       = 0x04,
    PROCESS_EPHEMERAL_DATA_REQ_STATE       = 0x05,
    PROCESS_EPHEMERAL_DATA_RESP_STATE      = 0x06,
    PROCESS_CONFIRM_KEY_DATA_REQ_STATE     = 0x07,
    PROCESS_CONFIRM_KEY_DATA_RESP_STATE    = 0x08,
    READY_STATE                            = 0x09,
    INTERMEDIATE_STATE                     = 0x0a,
    INITIATE_MATCH_DESC_REQ_STATE          = 0x0b,
    WAIT_FOR_MATCH_DESC_RESP_STATE         = 0x0c,
    TERMINATE_KEY_STATE                    = 0x0d,
    START_BCAST_DEL_TIMER_STATE            = 0x0e,
    WAIT_FOR_BCAST_DEL_TIME_STATE          = 0x0f,
    INVALID_STATE                          = 0xFF
}CBKEStates;


typedef enum
{
    SELFSTATICPUBLICKEY            = 0x00,
    SELFEPHEMERALPUBLICKEY         = 0x01,
    SELFEPHEMERALPRIVATEKEY        = 0x02,
    PARTNERCERTIFICATE             = 0x03,
    PARTNEREPHEMERALPUBLICKEY      = 0x04,
    PARTNEREPHEMERALPRIVATEKEY     = 0x05,
    PARTNERSTATICPUBLICKEY         = 0x06,
    SHAREDSECRET                   = 0x07,
    GENERATEDMACKEYKEYDATA         = 0x08,
    GENERATEDMACU                  = 0x09,
    GENERATEDMACV                  = 0x0a,
    PARTNERIEEEADDRESS             = 0x0b,
    PARTNERSHORTADDRESS            = 0x0c
}CBKEIds_t;


/*******************************************************************************
* Type Definitions
********************************************************************************/
typedef struct CBKE_Info_tag
{
//#if( g_CBKE_INFO_TABLE_NVM_STORAGE_d == RSI_DISABLE )
    uint8_t selfStaticPublicKey[SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE];
    uint8_t selfEphemeralPublicKey[SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE];
    uint8_t selfEphemeralPrivateKey[SECT163K1_PRIVATE_KEY_SIZE+1];
    uint8_t partnerCertificate[SECT163K1_CERTIFICATE_SIZE];
    uint8_t partnerEphemeralPublicKey[SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE];
    uint8_t partnerEphemeralPrivateKey[SECT163K1_PRIVATE_KEY_SIZE+1];
    uint8_t partnerStaticPublicKey[SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE];
    uint8_t sharedSecret[SECT163K1_SHARED_SECRET_SIZE+1];
    uint8_t generatedMacKeyKeyData[g_KEY_DATA_SIZE_c];
    uint8_t generatedMacU[AES_MMO_HASH_SIZE];
    uint8_t generatedMacV[AES_MMO_HASH_SIZE];
    uint8_t partnerIEEEAddress[8];
    uint8_t partnerShortAddress[2];
//#endif
    uint8_t partnerEndpoint;
    uint8_t CBKEInProcess;
}__attribute__((__packed__))CBKE_Info_t;


typedef struct CBKE_IdOffsetMap_tag
{
   uint16_t offset;
   uint8_t size;
}__attribute__((__packed__))CBKE_IdOffsetMap_t;

typedef struct CBKE_DataTimeOut_tag
{
    uint8_t ephemeralDataStartTimer;
    uint32_t ephemeralDataStartTime;
    uint8_t confirmDataStartTimer;
    uint32_t confirmDataStartTime;
    uint8_t initiateKeyStartTimer;
    uint32_t initiateKeyStartTime;
    uint32_t eventReceivedStartTime;
}__attribute__((__packed__))CBKE_DataTime_t;

/*******************************************************************************
* Memory Declarations
********************************************************************************/
extern uint8_t CBKE_KeyEstCurrentState;
extern uint8_t CBKE_KeyEstPreviousState;
extern CBKE_Info_t CBKE_KeyDetails;

/*******************************************************************************
   Function Prototypes
*******************************************************************************/
#if( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
void App_CBKE_Reset( void );
void App_CBKE_StateMachineHandler( void );
void App_KeyEstablishmentEventsHandler( ZCL_Event_t *pZCLEvent,
                                        APSDE_Data_Indication_t *pBuffer);
void CBKE_KeyInfoStoreDestEndpoint( uint8_t endpoint, uint16_t shortAddress );

uint8_t App_CBKEapplyOrVerifyDSAsignature ( uint8_t applyDSA,
                                uint8_t messageLength, uint8_t* pMessage );
uint8_t CBKE_KeyInfoGetDestEndpoint( void );
int AppMCHPYieldFunc(void);
int AppMCHPHashFunc(unsigned char *digest, unsigned long sz, unsigned char *data);
int AppMCHPGetRandomDataFunc(unsigned char *buffer,uint32 sz);
bool App_getOrSetCBKEInfo( bool getInfo, uint8_t InfoId, uint8_t* getSetValue );
#if ( g_CBKE_INFO_TABLE_NVM_STORAGE_d == RSI_ENABLE )

void InitAppCBKETable ( uint32_t cbkeTableOffset);
void App_CBKEClearNVM(void);
#endif   /* g_CBKE_INFO_TABLE_NVM_STORAGE_d*/

#endif  /* ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 ) */
/*******************************************************************************
* End Of File
*******************************************************************************/
#endif          /* _APP_CBKE_UTILITIES_H_ */
