/*******************************************************************************
* AppCBKEUtilities.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Key Establishment
*   cluster
*
*******************************************************************************/
#include "ZigBeeDataTypes.h"
//#include "nvm.h"
#include "stack_common.h"
//#include "eeprom_psm.h"
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
#include "stack_functionality.h"
#include "ZCL_Functionality.h"
#include "buffer_management.h"
#include "memory_utility.h"
#include "sw_timer.h"
#include "zdo_interface.h"
#include "AppTimerUtilities.h"
#include "ApplicationThinLayer.h"
#include "apsme_interface.h"
//#include "aps_linkkey_security_manager.h"
#include "app_configuration_default.h"

#if ( g_ZIGBEE_ROUTER_d == 1 )&& (g_TRUST_CENTER_SWAP_OUT_ENABLE_d == 1 )
	#include "AppTrustCenterKeepAliveUtilities.h"
#endif
#if ( g_GENERIC_TUNNEL_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Generic_Tunnel_Cluster.h"
#include "AppGenericTunnelClusterUtilities.h"
#endif
#if ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Key_Establishment.h"
#include "AppCBKEUtilities.h"
#include "AppCBKECertificate.h"
#include "AppDiscoveryUtilities.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define g_MAC_IEEE_ADDR_c                                   0x6F
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
uint8_t cbke_tc_endpoint;
uint8_t cbkestatus;
uint8_t CBKE_KeyEstCurrentState = INVALID_STATE;
uint8_t CBKE_KeyEstPreviousState = INVALID_STATE;
int32_t randomValue = 12;
uint16_t terminateKeyDestAddress;
uint8_t terminateDestEndpoint;
uint8_t terminateEvent;
CBKE_Info_t CBKE_KeyDetails;
uint8_t ephemeralDataTimeout = 0x00;
uint8_t confirmDataTimeout = 0x00;
CBKE_DataTime_t CBKE_DataTimeout;
extern uint8_t target_address[];
#if (DEBUG_TEST == 1)
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];
#endif

extern uint32_t g_NVM_Tables_Start_Offset_c ;

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
uint16_t randomT( void );
static void App_SendInitiateKeyEstablishment( uint8_t *pCertificate, uint8_t initiator );
static void App_SendEphemeralData( uint8_t *pEphemeralData, uint8_t initiator ) ;
static void App_SendConfirmData( uint8_t *pConfirmData, uint8_t initiator );
static void App_SendTerminateKeyEst( uint8_t initiator, uint8_t status ) ;
static int32_t AppRandomMultiplier(int32_t poly, int32_t quot);
static uint16_t GetKeyEstablishmentSuite( bool initiator );

static int32_t AppRandomGenerate( void );
/* Randmo data function which will be invoked by ECC library */
/* This function generates MACU and MACV */
static void App_DeriveMACUandMACV( uint8_t initiator );

void NWK_getMACattributePIB (uint8_t pib_attribute, uint8_t *p_attribute_value );
//bool App_getOrSetCBKEInfo( bool getInfo, uint8_t InfoId, uint8_t* getSetValue );

CBKE_IdOffsetMap_t offsetSizeTable[] = {
{ g_SELFSTATICPUBLICKEYOFFSET_c, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE },
{ g_SELFEPHEMPUBLICKEYOFFSET_c, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE},
{ g_SELFEPHEMPRIVATEKEYOFFSET_c, (SECT163K1_PRIVATE_KEY_SIZE + 1)},
{ g_PARTNERCERTOFFSET_c, SECT163K1_CERTIFICATE_SIZE},
{ g_PARTEPHEMPUBLICKEYOFFSET_c, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE},
{ g_PARTEPHEMPRIVATEKEYOFFSET_c, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE},
{ g_PARTSTATICPUBLICKEYOFFSET_c, (SECT163K1_PRIVATE_KEY_SIZE +1)},
{ g_SHAREDSECRETOFFSET_c, (SECT163K1_SHARED_SECRET_SIZE+ 1)},
{ g_GENMACKEYKEYDATAOFFSET_c, g_KEY_DATA_SIZE_c},
{ g_GENMACUOFFSET_c, AES_MMO_HASH_SIZE},
{ g_GENMACVOFFSET_c, AES_MMO_HASH_SIZE},
{ g_PARTIEEEADDROFFSET_c, g_MAX_ADDR_LENGTH_c },
{ g_PARTSHORTADDROFFSET_c, g_SHORT_ADDR_LENGTH_c } };


#if ( g_CBKE_INFO_TABLE_NVM_STORAGE_d == RSI_ENABLE )
	extern uint32_t cbkeTableNVMLocation;
#endif
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/
#if ( g_CBKE_INFO_TABLE_NVM_STORAGE_d == RSI_ENABLE )

void InitAppCBKETable ( uint32_t cbkeTableOffset)
{
  cbkeTableNVMLocation = cbkeTableOffset + g_NVM_Tables_Start_Offset_c;

}
/**************************************************************************************/
void App_CBKEClearNVM(void)
{

  uint8_t dummyVariable[100];
    memUtils_memSet(dummyVariable, g_ZCL_INVALID_DATA_c,
     	SECT163K1_CERTIFICATE_SIZE + SECT163K1_PRIVATE_KEY_SIZE+SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE+1);
   halCommonWriteToNvm( dummyVariable,
                cbkeTableNVMLocation ,
	(SECT163K1_CERTIFICATE_SIZE + SECT163K1_PRIVATE_KEY_SIZE+SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE+1));
}


#endif

/**************************************************************************************/
bool App_getOrSetCBKEInfo( bool getInfo, uint8_t InfoId, uint8_t* getSetValue )
{

    if ( InfoId > PARTNERSHORTADDRESS ) {
        return g_FALSE_c;
    }
    if ( getInfo == g_TRUE_c ) {

        uint8_t *pKeyDetails = (uint8_t*)&CBKE_KeyDetails;
        memUtils_memCopy( getSetValue, pKeyDetails + offsetSizeTable[InfoId].offset,
                          offsetSizeTable[InfoId].size );

    }
    else {

        uint8_t *pKeyDetails = (uint8_t*)&CBKE_KeyDetails;
        memUtils_memCopy( pKeyDetails + offsetSizeTable[InfoId].offset, getSetValue,
                           offsetSizeTable[InfoId].size );

    }
    return g_TRUE_c;
}


/**************************************************************************************/
void CBKE_Callback_Handle_Status( uint8_t status )
{
    uint8_t addr[g_MAX_ADDR_LENGTH_c];

    if ( status ==  g_CBKE_FAILURE_c ) {
        if ( terminateEvent > g_KeyEstTerminateKeyEstEvent_c ) {
            memUtils_memSet(addr, 0x00, g_MAX_ADDR_LENGTH_c );
            //App_CreateAndSendLeaveRequest( addr );
            rsi_zigb_leave_network();
        }
    }
}

/**************************************************************************************/
void CBKE_KeyInfoStoreDestEndpoint( uint8_t endpoint, uint16_t shortAddress )
{
    CBKE_KeyDetails.partnerEndpoint = endpoint;
    App_getOrSetCBKEInfo( g_SET_c, PARTNERSHORTADDRESS, (uint8_t*)&shortAddress );
	cbke_tc_endpoint =  CBKE_KeyDetails.partnerEndpoint;
}

/**************************************************************************************/
uint8_t CBKE_KeyInfoGetDestEndpoint( void )
{
    return cbke_tc_endpoint;
}
/**************************************************************************************/
void App_getMACaddress( uint8_t* addr )
{
    NWK_getMACattributePIB( g_MAC_IEEE_ADDR_c, addr );
}

/**************************************************************************************/
static void App_setTCLinkKey( uint8_t *pIEEEAddress )
{
   uint8_t deviceAddress[8];
   uint8_t generatedMacKeyKeyData[ AES_MMO_HASH_SIZE*2 ];
/*   memUtils_memReverseCopy( deviceAddress, pIEEEAddress, g_MAX_ADDR_LENGTH_c );*/

   memUtils_memCopy( deviceAddress, pIEEEAddress, g_MAX_ADDR_LENGTH_c );

#if ( g_APS_LAYER_SEC_PROCESSING_d == 1)
   App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACKEYKEYDATA, generatedMacKeyKeyData );
   APSAddLinkKey( deviceAddress, &generatedMacKeyKeyData[ AES_MMO_HASH_SIZE ] );
#endif
   MAC_setUpdateToNVM(g_TRUE_c);
}

/**************************************************************************************/
uint8_t App_getLinkKeyEntry( uint8_t* addr )
{
    uint8_t freeEntry[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#if ( g_APS_LAYER_SEC_PROCESSING_d == 1)
    if ( m_NO_ENTRY_c == getLinkKeyEntryIndex( addr ) ) {
        if ( m_NO_ENTRY_c == getLinkKeyEntryIndex( freeEntry ) ) {
            return g_FALSE_c;
        }
    }
#endif
    return g_TRUE_c;
}
/**************************************************************************************/
static uint16_t GetKeyEstablishmentSuite( bool initiator )
{
    uint16_t keyEstablishmentSuite;
    if ( initiator == g_TRUE_c ) {
        App_ReadWriteClientAttribute ( ZCL_Callback_Get_Device_EndPoint(),
                                g_KEY_ESTABLISHMENT_CLUSTER_c,
                                g_KEY_ESTABLISHMENT_SUITE_ATTRIBUTE_c ,
                                (uint8_t*)&keyEstablishmentSuite,
                                g_Read_Attribute_Data_c );
    }
    else {
        App_ReadWriteAttribute ( ZCL_Callback_Get_Device_EndPoint(),
                                g_KEY_ESTABLISHMENT_CLUSTER_c,
                                g_KEY_ESTABLISHMENT_SUITE_ATTRIBUTE_c ,
                                (uint8_t*)&keyEstablishmentSuite,
                                g_Read_Attribute_Data_c );
    }
    return keyEstablishmentSuite;
}
/**************************************************************************************/

static int32_t AppRandomMultiplier(int32_t poly, int32_t quot)
{
    int32_t p1, p0, q1, q0;

    p1 = poly/g_RAND_M1_c;
    p0 = poly % g_RAND_M1_c;

    q1 = quot/g_RAND_M1_c;
    q0 = quot % g_RAND_M1_c;

    return (((p0*q1+p1*q0) % g_RAND_M1_c) * g_RAND_M1_c+p0*q0) % g_RAND_M_c;
}

/**************************************************************************************/
static int32_t AppRandomGenerate(void)
{
    randomValue = randomT();

    randomValue = (AppRandomMultiplier( randomValue, (int32_t)g_RAND_B_c ) + 1 )% g_RAND_M_c;
    return randomValue;
}

/**************************************************************************************/
/*int AppMCHPGetRandomDataFunc( unsigned char *buffer, unsigned long sz )
{
    int32_t ranValue;
    int32_t index;
    uint8_t byteChunks;

    for( index = 0; index < sz; index++ ) {
       ranValue    = AppRandomGenerate();
       byteChunks  = (uint8_t) (ranValue >> 16);
       *(buffer + index)    = byteChunks;
    }

    return(0);
}
*/
/**************************************************************************************/
int AppMCHPYieldFunc(void)
{
    int retVal = 0;
    return retVal;
}

/**************************************************************************************/
int AppMCHPHashFunc(unsigned char *digest, unsigned long sz, unsigned char *data)
{
    int retVal = 1;
    uint8_t i;
    uint8_t output[16];

#if ( g_APS_LAYER_SEC_PROCESSING_d == 1)
    MathyasMeyerAlgorithm(data, (uint8_t)sz, output);
#endif
    memUtils_memCopy( digest, output, 0x10 );

    for ( i=0; i < 0x10; i++ ) {
        if ( digest[i] != 0x00 ) {
            retVal = 0;
            break;
        }
    }
    return retVal;
}
/**************************************************************************************/
static void App_SendInitiateKeyEstablishment( uint8_t *pCertificate, uint8_t initiator )
{

    uint16_t keyEstablishmentSuite = GetKeyEstablishmentSuite( initiator );
    uint8_t partnerShortAddr[2];

    App_getOrSetCBKEInfo( g_GET_c, PARTNERSHORTADDRESS, partnerShortAddr );

    uint8_t asduLength = sizeof( KeyEstInitiateKeyReqCommand_t ) +
                                g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c;
    uint8_t aPayload[sizeof( KeyEstInitiateKeyReqCommand_t ) +
                                g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c];

    if( initiator == g_TRUE_c ) {
        aPayload[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                      g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

        aPayload[2] = g_KeyEstInitiateKeyEstReq_c;
    }
    else {
        aPayload[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                      g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

        aPayload[2] = g_KeyEstInitiateKeyEstResp_c;
    }
    aPayload[1] = App_GetTransSeqNumber();
    *((uint16_t*)&aPayload[3]) = keyEstablishmentSuite;
    aPayload[5] = g_EPHEMERAL_DATA_GENERATE_TIME_c;
    aPayload[6] = g_CONFIRM_KEY_DATA_GENERATE_TIME_c;
    memUtils_memCopy( &aPayload[7],  pCertificate, SECT163K1_CERTIFICATE_SIZE );

    App_SendData ( g_Short_Address_Mode_c,
                 partnerShortAddr,
                 CBKE_KeyDetails.partnerEndpoint,
                 g_KEY_ESTABLISHMENT_CLUSTER_c,
                 asduLength,
                 aPayload );

}

/******************************************************************************/
static void App_SendEphemeralData( uint8_t *pEphemeralData, uint8_t initiator )
{
    uint8_t asduLength = sizeof( KeyEstEphemeralDataReqCommand_t ) +
                                g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c;
    uint8_t aPayload[sizeof( KeyEstEphemeralDataReqCommand_t ) +
                                g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c];
    uint8_t partnerShortAddr[2];

    App_getOrSetCBKEInfo( g_GET_c, PARTNERSHORTADDRESS, partnerShortAddr );

    if( initiator == g_TRUE_c ) {
        aPayload[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                      g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

        aPayload[2] = g_KeyEstEphemeralDataReq_c;
    }
    else {
        aPayload[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                      g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

        aPayload[2] = g_KeyEstEphemeralDataResp_c;
    }
    aPayload[1] = App_GetTransSeqNumber();
    memUtils_memCopy( &aPayload[3], pEphemeralData, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );

    App_SendData ( g_Short_Address_Mode_c,
                 partnerShortAddr,
                 CBKE_KeyDetails.partnerEndpoint,
                 g_KEY_ESTABLISHMENT_CLUSTER_c,
                 asduLength,
                 aPayload );
}

/******************************************************************************/
static void App_SendConfirmData( uint8_t *pConfirmData, uint8_t initiator )
{

    uint8_t asduLength = sizeof( ZCL_KeyEstConfirmDataReqCommand_t ) +
                                g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c;
    uint8_t aPayload[sizeof( ZCL_KeyEstConfirmDataReqCommand_t ) +
                                g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c];

    uint8_t partnerShortAddr[2];

    App_getOrSetCBKEInfo( g_GET_c, PARTNERSHORTADDRESS, partnerShortAddr );

    if( initiator == g_TRUE_c ) {
        aPayload[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                      g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

        aPayload[2] = g_KeyEstConfirmKeyDataReq_c;
    }
    else {
        aPayload[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                      g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

        aPayload[2] = g_KeyEstConfirmKeyDataResp_c;
    }
    aPayload[1] = App_GetTransSeqNumber();
    memUtils_memCopy( &aPayload[3], pConfirmData, AES_MMO_HASH_SIZE );

    App_SendData ( g_Short_Address_Mode_c,
                 partnerShortAddr,
                 CBKE_KeyDetails.partnerEndpoint,
                 g_KEY_ESTABLISHMENT_CLUSTER_c,
                 asduLength,
                 aPayload );
}

/******************************************************************************/
static void App_SendTerminateKeyEst( uint8_t initiator, uint8_t status )
{
    uint8_t asduLength = sizeof(KeyEstTerminateKeyEstCommand_t) +
                         g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c;

    uint8_t aPayload[ sizeof(KeyEstTerminateKeyEstCommand_t) +
                         g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c ];

    uint16_t keyEstablishmentSuite = GetKeyEstablishmentSuite( initiator );

    if( initiator == g_TRUE_c ) {
        aPayload[0] = g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                      g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    }
    else {
        aPayload[0] = g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_CLUSTER_SPECIFIC_COMMAND_c |
                      g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;
    }
    aPayload[1] = App_GetTransSeqNumber();
    aPayload[2] = g_KeyEstTerminateKeyEst_c;
    aPayload[3] = status;
    aPayload[4] = g_TERMINATE_KEY_ESTABLISHMENT_WAIT_TIME_c;
    *((uint16_t*)&aPayload[5]) = keyEstablishmentSuite;

    App_SendData ( g_Short_Address_Mode_c,
                 (uint8_t*)&terminateKeyDestAddress,
                 terminateDestEndpoint,
                 g_KEY_ESTABLISHMENT_CLUSTER_c,
                 asduLength,
                 aPayload );

}

/******************************************************************************/
uint8_t App_KeyDerivationFunction( uint8_t keyDataLen, uint8_t hashLen,
                                   uint8_t sharedSecretLength, uint8_t* pSharedSecret,
                                   uint8_t *pKeyData, uint8_t sharedInfolength,
                                   uint8_t *pSharedInfo )
{

    uint8_t i;
    uint8_t keyDataArray[50];
    /* The counter to be concatenated with the Shared secret */
    uint32_t counter = 0x00000000;
    uint8_t dataLength = 0x00;
    /* If the summation of shared secret length, shared info length and
       the size occupied by the counter is greater than equal to the
       max permitted size of the hash function, return the status as INVALID
       and stop hashing  */
    if( ( sharedSecretLength + sharedInfolength + 0x04 ) >= g_HASH_MAX_LENGTH_c ) {
        return 0x01;
    }
    for( i = 0x00; i < ( keyDataLen/hashLen ); i++ )  {
        counter++;
        /* Copy the shared secret into a single array.The shared secret is of size 21 bytes */
        memUtils_memCopy( &keyDataArray[dataLength], pSharedSecret,  sharedSecretLength );
        dataLength += sharedSecretLength;

        memUtils_memReverseCopy ( &keyDataArray[dataLength], (uint8_t*)&counter,sizeof(uint32_t) );
        dataLength += sizeof(uint32_t);
        if( sharedInfolength ) {
            memUtils_memCopy (  &(keyDataArray[dataLength]), pSharedInfo, sharedInfolength );
            dataLength += sharedInfolength;
        }
#if ( g_APS_LAYER_SEC_PROCESSING_d == 1)
        MathyasMeyerAlgorithm( keyDataArray, dataLength, pKeyData + i * hashLen );
#endif
        dataLength = 0x00;

    }
    return 0x00;
}

/******************************************************************************/
static void App_DeriveMACUandMACV( uint8_t initiator )
{
    uint8_t MACArray[64];
    uint8_t index = 0x00;
    uint8_t aSelfIEEEAddress[8];
    uint8_t aPartnerIEEEAddr[8];
    App_getMACaddress( aSelfIEEEAddress );
    uint8_t *dataHolder22;
    uint8_t *dataHolder48;
    uint8_t bufferIndex;
    uint8_t *pbuffer;
    /* Derive the mac key and key data using the Key derivation function,
      shared info is zero, hence NULL is passed */
//#if ( g_SMALL_BUFFER_c < 60 )
//#error "for local variables 60 bytes are required"
//#endif
     bufferIndex = buffMgmt_allocateBuffer( g_SMALL_BUFFER_c );
     pbuffer = buffMgmt_getBufferPointer( bufferIndex );

#if (DEBUG_TEST == 1)
    /* Buffer ID --> 1 */
    allocatedbuffer[bufferIndex]= 173;
    freebuffer[bufferIndex][0]= 173;
    freebuffer[bufferIndex][1]= 0;
#endif
    dataHolder22 = pbuffer;
    dataHolder48 = &pbuffer[22];

    App_getOrSetCBKEInfo( g_GET_c, SHAREDSECRET, dataHolder22 );

    App_KeyDerivationFunction( g_KEY_DATA_SIZE_c,
                               AES_MMO_HASH_SIZE,
                               SECT163K1_SHARED_SECRET_SIZE,
                               dataHolder22,
                               dataHolder48,
                               0, g_NULL_c );
    App_getOrSetCBKEInfo( g_SET_c, GENERATEDMACKEYKEYDATA, dataHolder48 );
    /* Using the API provided by Certicom, get the Static public key of self device using
       the certifcate */
    ZSE_ECQVReconstructPublicKey( App_GetCertificate(),
                                  App_GetCAPublicKey(),
                                  dataHolder22,
                                  AppMCHPHashFunc,
                                  AppMCHPYieldFunc, g_YIELD_LEVEL_c );

    App_getOrSetCBKEInfo( g_SET_c, SELFSTATICPUBLICKEY, dataHolder22 );

    /* Using the API provided by Certicom, get the Static public key of remote device using the
       certificate*/
    App_getOrSetCBKEInfo( g_GET_c, PARTNERCERTIFICATE, dataHolder48 );

    ZSE_ECQVReconstructPublicKey( dataHolder48,
                                  App_GetCAPublicKey(),
                                  dataHolder22,
                                  AppMCHPHashFunc,
                                  AppMCHPYieldFunc, g_YIELD_LEVEL_c );

    App_getOrSetCBKEInfo( g_SET_c, PARTNERSTATICPUBLICKEY, dataHolder22 );

    /* fill the structure message,Generate MACU using HMAC Algorithm*/
    /* If the initiator is g_g_TRUE_c_c, then it means the MACU and MACV are derived
       on the initiator device */
    App_getOrSetCBKEInfo( g_GET_c, PARTNERIEEEADDRESS, aPartnerIEEEAddr );
    if( g_TRUE_c == initiator ) {
        /*------------------------------------------------------------------
        Note:
        On the Initiator Device to derive MACU, the following message should
        be prepared
        MACU = MAC(Mackkey)( M || ID(U) || ID(V) || E(U) || E(V) )
        -------------------------------------------------------------------*/

        /* additional component for initiator device is 0x02 */
        MACArray[index++] = g_ADDITIONAL_COMPONENT_FOR_INITIATOR_c;

        /* copy the IEEE address of the initiator device in the Big Endian
           format. */
/*        memUtils_memReverseCopy( &MACArray[index], aSelfIEEEAddress, g_MAX_ADDR_LENGTH_c ); */

        memUtils_memCopy( &MACArray[index], aSelfIEEEAddress, g_MAX_ADDR_LENGTH_c );
        index += g_MAX_ADDR_LENGTH_c;

        /* copy the IEEE address of the responder device in the Big Endian
          format */
        memUtils_memCopy ( &MACArray[index], aPartnerIEEEAddr, g_MAX_ADDR_LENGTH_c );
        index += g_MAX_ADDR_LENGTH_c;

        /* copy the Ephemeral Public key of the initiator device */
        App_getOrSetCBKEInfo( g_GET_c, SELFEPHEMERALPUBLICKEY, dataHolder22 );
        memUtils_memCopy( &MACArray[index], dataHolder22, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
        index += SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE;

        /* copy the Ephemeral Public key of the responder device */
        App_getOrSetCBKEInfo( g_GET_c, PARTNEREPHEMERALPUBLICKEY, dataHolder22 );
        memUtils_memCopy( &MACArray[index], dataHolder22, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );

        index += SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE;

#if ( g_APS_LAYER_SEC_PROCESSING_d == 1)
        /* Apply HMAC algorithm to generate the MACU */
        App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACKEYKEYDATA, dataHolder48 );
        HMACAlgorithm( dataHolder48, MACArray, dataHolder22, index );
        App_getOrSetCBKEInfo( g_SET_c, GENERATEDMACU, dataHolder22 );
#endif

        /*------------------------------------------------------------------
        Note:
        On the Initiator Device to derive MACV, the following message should
        be prepared
        MACV = MAC(Mackkey)( M || ID(V) || ID(U) || E(V) || E(U) )
        -------------------------------------------------------------------*/
        index = 0x00;
        /*Generating MACV on the Initiator device*/
        /*Additional component for responder device is 0x03*/
        MACArray[index++] = g_ADDITIONAL_COMPONENT_FOR_RESPONDER_c;

        /*copy the IEEE address of the partner device to derive MACV on the initiator device*/
        memUtils_memCopy( &MACArray[index], aPartnerIEEEAddr, g_MAX_ADDR_LENGTH_c );
        index += g_MAX_ADDR_LENGTH_c;

        /*copy the IEEE address of the initiator device to derive MACV on the initiator device
        in BIG endian format*/
/*        memUtils_memReverseCopy( &MACArray[index], aSelfIEEEAddress, g_MAX_ADDR_LENGTH_c );*/

        memUtils_memCopy( &MACArray[index], aSelfIEEEAddress, g_MAX_ADDR_LENGTH_c );
        index += g_MAX_ADDR_LENGTH_c;

        /*copy the ephemeral public key of the responder device of size 22 bytes*/
        App_getOrSetCBKEInfo( g_GET_c, PARTNEREPHEMERALPUBLICKEY, dataHolder22 );
        memUtils_memCopy ( &MACArray[index], dataHolder22, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
        index += SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE;

        /*copy the ephemeral public key of the initiator device of size 22 bytes*/
        App_getOrSetCBKEInfo( g_GET_c, SELFEPHEMERALPUBLICKEY, dataHolder22 );
        memUtils_memCopy( &MACArray[index], dataHolder22, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
        index += SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE;

#if ( g_APS_LAYER_SEC_PROCESSING_d == 1)
        /* Apply HMAC algorithm to generate the MACV */
        App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACKEYKEYDATA, dataHolder48 );
        HMACAlgorithm( dataHolder48, MACArray, dataHolder22, index );
        App_getOrSetCBKEInfo( g_SET_c, GENERATEDMACV, dataHolder22 );
#endif
    }
    else {
        /*------------------------------------------------------------------
        Note: Generating MACU and MACV on the responder device
        On the Responder Device to derive MACU, the following message should
        be prepared
        MACU = MAC(Mackkey)( M || ID(U) || ID(V) || E(U) || E(V) )
        -------------------------------------------------------------------*/
        /*Generating MACU on responder device*/

        /*Additional component for Initiator device is 0x02*/
        MACArray[index++] = g_ADDITIONAL_COMPONENT_FOR_INITIATOR_c;
        /*
        NOTE:
        For example:
        CBKE_KeyDetails.v_staticPublicKey - On the responder device, this holds the
        static public key of the intiator device.Since for responder device, all the information
        of initiator(remote) device is stored in the field names statring with 'v'*/

        /*copy the IEEE address of the initiator device of size 8 bytes*/
        memUtils_memCopy( &MACArray[index], aPartnerIEEEAddr, g_MAX_ADDR_LENGTH_c );
        index += g_MAX_ADDR_LENGTH_c;

        /*Copy the IEEE address of the responder device of size 8 in Big Indian Format*/
/*        memUtils_memReverseCopy( &MACArray[index], aSelfIEEEAddress, g_MAX_ADDR_LENGTH_c );*/

        memUtils_memCopy( &MACArray[index], aSelfIEEEAddress, g_MAX_ADDR_LENGTH_c );
        index += g_MAX_ADDR_LENGTH_c;

        /*copy the ephemeral public key of the initiator device of size 22 bytes*/
        App_getOrSetCBKEInfo( g_GET_c, PARTNEREPHEMERALPUBLICKEY, dataHolder22 );
        memUtils_memCopy( &MACArray[index], dataHolder22, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
        index += SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE;

        /*copy the ephemeral public key of the responder device of size 22 bytes*/
        App_getOrSetCBKEInfo( g_GET_c, SELFEPHEMERALPUBLICKEY, dataHolder22 );
        memUtils_memCopy( &MACArray[index], dataHolder22, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
        index += SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE;

#if ( g_APS_LAYER_SEC_PROCESSING_d == 1)
        /* Apply HMAC algorithm to generate the MACU */
        App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACKEYKEYDATA, dataHolder48 );
        HMACAlgorithm( dataHolder48, MACArray, dataHolder22,index );
        App_getOrSetCBKEInfo( g_SET_c, GENERATEDMACU, dataHolder22 );
#endif
        /*------------------------------------------------------------------
        On the Responder Device to derive MACV, the following message should
        be prepared
        MACV = MAC(Mackkey)( M || ID(V) || ID(U) || E(V) || E(U) )
        -------------------------------------------------------------------*/

        index = 0x00;

        /* Generating MACV on Responder device */
        /* Additional component for Responder device is 0x03 */
        MACArray[index++] = g_ADDITIONAL_COMPONENT_FOR_RESPONDER_c;

        /*NOTE:
        All self device information is stored in the global instance CBKE_KeyDetails
        with the field names starting with 'u'. For example, on the responder device
        the static public key of responder device is stored in the field
        CBKE_KeyDetails.u_staticPublicKey */
        /*Copy the IEEE address of the responder device of size 8 in Big Indian Format*/
/*        memUtils_memReverseCopy( &MACArray[index], aSelfIEEEAddress, g_MAX_ADDR_LENGTH_c ); */


        memUtils_memCopy( &MACArray[index], aSelfIEEEAddress, g_MAX_ADDR_LENGTH_c );
        index += g_MAX_ADDR_LENGTH_c;

        /*copy the IEEE Address of the Initiator device of size 22 bytes*/
        memUtils_memCopy( &MACArray[index], aPartnerIEEEAddr, g_MAX_ADDR_LENGTH_c );
        index += g_MAX_ADDR_LENGTH_c;

        /*copy the ephemeral public key of the Responder device of size 22 bytes*/
        App_getOrSetCBKEInfo( g_GET_c, SELFEPHEMERALPUBLICKEY, dataHolder22 );
        memUtils_memCopy( &MACArray[index], dataHolder22, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
        index += SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE;

        App_getOrSetCBKEInfo( g_GET_c, PARTNEREPHEMERALPUBLICKEY, dataHolder22 );
        memUtils_memCopy ( &MACArray[index], dataHolder22, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
        index += SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE;
        /*Apply HMAC algorithm to generate the MACV */
#if ( g_APS_LAYER_SEC_PROCESSING_d == 1)
		App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACKEYKEYDATA, dataHolder48 );
        HMACAlgorithm( dataHolder48, MACArray, dataHolder22,index );
        App_getOrSetCBKEInfo( g_SET_c, GENERATEDMACV, dataHolder22 );
#endif
    }
    buffMgmt_freeBuffer( bufferIndex, 122 );
}


/******************************************************************************/
void App_CBKE_StateMachineHandler( void )
{
    switch( CBKE_KeyEstCurrentState )
    {
        case INITIATE_KEY_EST_REQ_STATE:
        {
            /*create the payload for initiate key establishment request command*/
            App_SendInitiateKeyEstablishment( App_GetCertificate(), g_TRUE_c );
            /*change the current state to intermediate state*/
            CBKE_ChangeState( INTERMEDIATE_STATE );
            CBKE_ChangePreviousState( INITIATE_KEY_EST_REQ_STATE );
            /*set the CBKE in process bit to g_g_TRUE_c_c*/
            CBKE_KeyDetails.CBKEInProcess = g_TRUE_c;
            CBKE_DataTimeout.initiateKeyStartTimer = g_TRUE_c;
            CBKE_DataTimeout.initiateKeyStartTime = App_getcurrentTick();
        }
        break;

        case PROCESS_KEY_REQ_STATE:
            /* Create the payload for initiate key establishment
               response command */
            App_SendInitiateKeyEstablishment( App_GetCertificate(), g_FALSE_c );
            CBKE_ChangeState( INTERMEDIATE_STATE );
            CBKE_ChangePreviousState( PROCESS_KEY_REQ_STATE );
            break;

        case PROCESS_KEY_EST_RESP_STATE:
        {
            /* Invoke the ECC api to create the ephemeral public key and create
               ephemeral data request */
            uint8_t bufferIndex,*pbuffer;
            if ( ( ( halCommonGetInt32uMillisecondTick()-
                    CBKE_DataTimeout.eventReceivedStartTime ) < g_EVENT_PROCESS_DELAY_TIME_IN_MS_c ) ) {
                return;
            }
            bufferIndex = buffMgmt_allocateBuffer( g_SMALL_BUFFER_c );
            if ( bufferIndex == g_NO_AVAILABLE_BUFFER_c ) {
                 return;
            }
            pbuffer = buffMgmt_getBufferPointer( bufferIndex );
#if (DEBUG_TEST == 1)
            /* Buffer ID --> 1 */
            allocatedbuffer[bufferIndex]= 13;
            freebuffer[bufferIndex][0]= 13;
            freebuffer[bufferIndex][1]= 0;
#endif

            if( MCE_SUCCESS ==  ZSE_ECCGenerateKey( pbuffer, &pbuffer[22],
                          AppMCHPGetRandomDataFunc, AppMCHPYieldFunc, g_YIELD_LEVEL_c ) ) {

                App_getOrSetCBKEInfo( g_SET_c, SELFEPHEMERALPRIVATEKEY, pbuffer );
                App_getOrSetCBKEInfo( g_SET_c, SELFEPHEMERALPUBLICKEY, &pbuffer[22] );

                App_SendEphemeralData( &pbuffer[22] , g_TRUE_c);

                CBKE_ChangePreviousState( PROCESS_KEY_EST_RESP_STATE );
                CBKE_ChangeState( INTERMEDIATE_STATE );
            }
            else  {
                App_CBKE_Reset();
                CBKE_Callback_Handle_Status( g_CBKE_FAILURE_c );
            }
            buffMgmt_freeBuffer( bufferIndex, 122 );
            break;
        }
        case PROCESS_EPHEMERAL_DATA_REQ_STATE:
        {
            uint8_t bufferIndex,*pbuffer;

            if ( ( ( halCommonGetInt32uMillisecondTick() -
                     CBKE_DataTimeout.eventReceivedStartTime ) < g_EVENT_PROCESS_DELAY_TIME_IN_MS_c ) ) {
                return;
            }

            bufferIndex = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
            if ( bufferIndex == g_NO_AVAILABLE_BUFFER_c ) {
                 return;
            }
            pbuffer = buffMgmt_getBufferPointer( bufferIndex );
#if (DEBUG_TEST == 1)
            /* Buffer ID --> 23 */
            allocatedbuffer[bufferIndex]= 23;
            freebuffer[bufferIndex][0]= 23;
            freebuffer[bufferIndex][1]= 0;
#endif
            /* Invoke the ECC api to create the ephemeral public key */
            if( MCE_SUCCESS == ZSE_ECCGenerateKey( pbuffer,
                &pbuffer[22], AppMCHPGetRandomDataFunc, AppMCHPYieldFunc, g_YIELD_LEVEL_c ) ) {


                App_getOrSetCBKEInfo( g_SET_c, SELFEPHEMERALPRIVATEKEY, pbuffer );
                App_getOrSetCBKEInfo( g_SET_c, SELFEPHEMERALPUBLICKEY,  &pbuffer[22] );

                App_SendEphemeralData( &pbuffer[22] , g_FALSE_c );

                App_getOrSetCBKEInfo( g_GET_c, PARTNEREPHEMERALPUBLICKEY, &pbuffer[44] );
                App_getOrSetCBKEInfo( g_GET_c, PARTNERCERTIFICATE, &pbuffer[88] );
                /* Generate the MAC key and Key Data using the ECC API */


                if( MCE_SUCCESS == ZSE_ECCKeyBitGenerate( App_GetStaticPrivateKey(),
                    &pbuffer[0], &pbuffer[22], &pbuffer[88],&pbuffer[44], App_GetCAPublicKey(),
                    &pbuffer[66], AppMCHPHashFunc, AppMCHPYieldFunc, g_YIELD_LEVEL_c ) ) {
                    /* Derive the MACU and MACV on the Responder device */
                    App_getOrSetCBKEInfo( g_SET_c, SHAREDSECRET,  &pbuffer[66] );

                    App_DeriveMACUandMACV( g_FALSE_c );

                    CBKE_ChangePreviousState( PROCESS_EPHEMERAL_DATA_REQ_STATE );
                    CBKE_ChangeState( INTERMEDIATE_STATE );
                }
                else  {
                    App_CBKE_Reset();
                    CBKE_Callback_Handle_Status( g_CBKE_FAILURE_c );
                }
            }
            else {
                App_CBKE_Reset();
                CBKE_Callback_Handle_Status( g_CBKE_FAILURE_c );
            }

            buffMgmt_freeBuffer( bufferIndex, 122 );
            break;

        }
        case INITIATE_TERMINATE_KEY_REQ_STATE:
            if ( terminateEvent < g_KeyEstTerminateKeyEstEvent_c ) {
                App_SendTerminateKeyEst( g_FALSE_c, cbkestatus );
            }
            else {
                App_SendTerminateKeyEst( g_TRUE_c, cbkestatus );
            }
            CBKE_ChangePreviousState( INITIATE_TERMINATE_KEY_REQ_STATE );
            CBKE_ChangeState( INTERMEDIATE_STATE );
            break;

        case PROCESS_EPHEMERAL_DATA_RESP_STATE:
        {
            uint8_t bufferIndex,*pbuffer;
            /*generate the shared secret by providing the remote device certificate,
            self static private key, ephemeral public and private key and
            the ca public key*/
            if ( ( ( halCommonGetInt32uMillisecondTick()-
                     CBKE_DataTimeout.eventReceivedStartTime) < g_EVENT_PROCESS_DELAY_TIME_IN_MS_c ) ) {
                return;
            }
            bufferIndex = buffMgmt_allocateBuffer(g_LARGE_BUFFER_c);
            if ( bufferIndex == g_NO_AVAILABLE_BUFFER_c ) {
                 return;
            }
            pbuffer = buffMgmt_getBufferPointer( bufferIndex );
#if (DEBUG_TEST == 1)
            /* Buffer ID --> 1 */
            allocatedbuffer[bufferIndex]= 33;
            freebuffer[bufferIndex][0]= 33;
            freebuffer[bufferIndex][1]= 0;
#endif
            App_getOrSetCBKEInfo( g_GET_c, SELFEPHEMERALPRIVATEKEY, pbuffer );
            App_getOrSetCBKEInfo( g_GET_c, SELFEPHEMERALPUBLICKEY,  &pbuffer[22] );
            App_getOrSetCBKEInfo( g_GET_c, PARTNEREPHEMERALPUBLICKEY, &pbuffer[44] );
            App_getOrSetCBKEInfo( g_GET_c, PARTNERCERTIFICATE, &pbuffer[88] );

            if( MCE_SUCCESS == ZSE_ECCKeyBitGenerate( App_GetStaticPrivateKey(),
                pbuffer, &pbuffer[22] , &pbuffer[88],&pbuffer[44], App_GetCAPublicKey(),
                &pbuffer[66], AppMCHPHashFunc, AppMCHPYieldFunc, g_YIELD_LEVEL_c ) ) {
                /*derive the MACU and MACV on the Initiator device*/
                App_getOrSetCBKEInfo( g_SET_c, SHAREDSECRET,  &pbuffer[66] );
                App_DeriveMACUandMACV( g_TRUE_c );

                /*generate confirm data request*/
                App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACU,  &pbuffer[66] );

                App_SendConfirmData( &pbuffer[66], g_TRUE_c );
                CBKE_ChangePreviousState( PROCESS_EPHEMERAL_DATA_RESP_STATE );
                CBKE_ChangeState( INTERMEDIATE_STATE );
            }
            else  {
                App_CBKE_Reset();
                CBKE_Callback_Handle_Status(g_CBKE_FAILURE_c);
            }
            buffMgmt_freeBuffer( bufferIndex, 122 );
            break;
        }
        case PROCESS_CONFIRM_KEY_DATA_REQ_STATE:
        {
            /*Set the generated Application link key for the IEEE address*/
            uint8_t aPartnerIEEEAddr[8];
            uint8_t generatedMacV[AES_MMO_HASH_SIZE];

            App_getOrSetCBKEInfo( g_GET_c, PARTNERIEEEADDRESS, aPartnerIEEEAddr );
            App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACV, generatedMacV );

            App_setTCLinkKey( aPartnerIEEEAddr );
            /*Generate the confirm key data response command*/
            App_SendConfirmData( generatedMacV, g_FALSE_c );
            CBKE_ChangeState( READY_STATE );
            CBKE_ChangePreviousState( PROCESS_CONFIRM_KEY_DATA_REQ_STATE );
            /* Application should take the appropriate action on Successful CBKE. */
            CBKE_Callback_Handle_Status( g_CBKE_SUCCESS_c );
            break;
        }
        case PROCESS_CONFIRM_KEY_DATA_RESP_STATE:
        {
            /* On receipt of confirm key data response command on the initiator
               device, set the Application link key */
            uint8_t aPartnerIEEEAddr[8];

            App_getOrSetCBKEInfo( g_GET_c, PARTNERIEEEADDRESS, aPartnerIEEEAddr );

            App_setTCLinkKey( aPartnerIEEEAddr );
            CBKE_ChangeState( READY_STATE );
            CBKE_ChangePreviousState( PROCESS_CONFIRM_KEY_DATA_RESP_STATE );
            /* Application should take the appropriate action on Successful CBKE. */
            CBKE_Callback_Handle_Status(g_CBKE_SUCCESS_c);
            break;
        }
        case READY_STATE:
            /* All CBKE state and variable are reseted, because of CBKE
               Process is completed */
            App_CBKE_Reset();
            break;

        case TERMINATE_KEY_STATE :
            App_CBKE_Reset();
            CBKE_Callback_Handle_Status( g_CBKE_FAILURE_c );
            break;

        case INTERMEDIATE_STATE:
        case INVALID_STATE:
        /* These states are intermediate states and they perform no action */
        default:
            break;
    }
    /* if CBKE is in process, check if the ephemeral data request/response is received
       within the specified duration, if not change the state to INVALID_STATE for
       error handling purpose */
    if( g_TRUE_c == CBKE_KeyDetails.CBKEInProcess ) {
        /* check if initiate key establishment response is received or not within timeout
           value, otherwise leave the network */
        if( g_TRUE_c == CBKE_DataTimeout.initiateKeyStartTimer ) {
            /* check if time is elapsed */
            if( App_getDifferance( App_getcurrentTick(),
                CBKE_DataTimeout.initiateKeyStartTime ) > g_INITIATE_KEY_RESPONSE_WAIT_TIME_c  ) {
                /* if the previous state is still initiate key establishment request state ,
                   that means the device didnt get key establishment response command,
                   so leave the network.*/
                if( INITIATE_KEY_EST_REQ_STATE == CBKE_GetPreviousState() ) {
                    App_CBKE_Reset();
                    terminateEvent = g_KeyEstInitiateKeyEstRespEvent_c;
                    CBKE_Callback_Handle_Status(g_CBKE_FAILURE_c);
                }
            }
        }
        if( g_TRUE_c == CBKE_DataTimeout.ephemeralDataStartTimer ) {
            /*check if time is elapsed*/
            if( App_getDifferance( App_getcurrentTick(),
                             CBKE_DataTimeout.ephemeralDataStartTime ) > ephemeralDataTimeout ) {
                /* if we didnt receive the Ephemeral data request /response within the duration
                   then change the state to invalid state and reset the CBKE in process bit to g_FALSE_c */
                if( ! ( ( CBKE_KeyEstCurrentState == PROCESS_EPHEMERAL_DATA_REQ_STATE ) ||
                        ( CBKE_KeyEstCurrentState == PROCESS_EPHEMERAL_DATA_RESP_STATE ) ) )  {
                    CBKE_ChangeState( INVALID_STATE );
                    /* Application should take the appropriate action on CBKE_FAILURE */
                    CBKE_Callback_Handle_Status( g_CBKE_FAILURE_c );
                    CBKE_KeyDetails.CBKEInProcess = g_FALSE_c;
                }
                CBKE_DataTimeout.ephemeralDataStartTimer = g_FALSE_c;
            }
        }
        if ( g_TRUE_c == CBKE_DataTimeout.confirmDataStartTimer ) {
            if( App_getDifferance( App_getcurrentTick(),
                             CBKE_DataTimeout.confirmDataStartTime ) > confirmDataTimeout ) {
                /* if we didnt receive the confirm data request /response within the duration
                  then change the state to invalid state and reset the CBKE in process bit
                  to g_FALSE_c */
                if( ! ( ( CBKE_KeyEstCurrentState == PROCESS_CONFIRM_KEY_DATA_REQ_STATE ) ||
                        ( CBKE_KeyEstCurrentState == PROCESS_CONFIRM_KEY_DATA_RESP_STATE )  ) ) {
                    /*change it to invalid states for error handling*/
                    CBKE_ChangeState( INVALID_STATE );
                    /* Application should take the appropriate action on CBKE_FAILRE */
                    CBKE_Callback_Handle_Status( g_CBKE_FAILURE_c );
                    CBKE_KeyDetails.CBKEInProcess = g_FALSE_c;
                }
                CBKE_DataTimeout.confirmDataStartTimer = g_FALSE_c;
            }
        }
    }
}

/******************************************************************************/
void App_KeyEstablishmentEventsHandler( ZCL_Event_t *pZCLEvent,
                                        APSDE_Data_Indication_t* p_dataInd )
{
    uint8_t aIEEEAddress[8];
    uint16_t partnerShortAddress;
    terminateDestEndpoint = p_dataInd->src_endpoint;

    if ( ( pZCLEvent->eventId == g_KeyEstInitiateKeyEstReqEvent_c ) &&
         ( CBKE_GetCurrentState() != INVALID_STATE ) ) {
        return;
    }
    terminateEvent = pZCLEvent->eventId;
    if ( p_dataInd->src_addr_mode == g_Short_Address_Mode_c ) {
        terminateKeyDestAddress = p_dataInd->src_address.short_address;
    }
    else {
         APS_APSMEsearchAddress( &terminateKeyDestAddress,
                           p_dataInd->src_address.IEEE_address, g_SEARCH_BY_EXTENDED_ADDRESS_c );
         p_dataInd->src_address.short_address = terminateKeyDestAddress;
    }

    App_getOrSetCBKEInfo( g_GET_c, PARTNERSHORTADDRESS, (uint8_t*)&partnerShortAddress );

    switch( pZCLEvent->eventId )
    {
        case g_KeyEstInitiateKeyEstReqEvent_c:
        case g_KeyEstInitiateKeyEstRespEvent_c:
        {
            KeyEstInitiateKeyReqCommand_t *pInitiateKey =
                ( KeyEstInitiateKeyReqCommand_t*)pZCLEvent->pEventData ;
            bool initiator = g_FALSE_c ;

            /* if initiate key estblishment response event is received before timeout
               values, whether successful or not, reset the timer. If the response is
               not valid terminate key will be sent */
            if( g_KeyEstInitiateKeyEstRespEvent_c == pZCLEvent->eventId ) {
                initiator = g_TRUE_c;
                CBKE_DataTimeout.initiateKeyStartTimer = g_FALSE_c;
            }
            /* This condition is required for mulitple initiate key establishement request
               command sent from other devices. When CBKE procedure is executed for one device
               the other device is not allowed.Hence terminate key establishment command with
               status No resources is returned to the device */
            if( ( g_TRUE_c == CBKE_KeyDetails.CBKEInProcess ) &&
                ( partnerShortAddress != p_dataInd->src_address.short_address ) ) {

                cbkestatus = g_KeyEstTerminateNoResources_c;
                CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
                return;
            }
            if( ( *((uint16_t*)pInitiateKey->keyEstSuite) & g_CBKE_KEY_ESTABLISHMENT_SUITE_c ) !=
                ( GetKeyEstablishmentSuite( initiator ) & g_CBKE_KEY_ESTABLISHMENT_SUITE_c ) ) {

                cbkestatus = g_KeyEstTerminateUnSupportedSuite_c;
                CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
                return ;
            }
            /* memUtils_memCompare returns 0 if success and 1 if failure */
            if( memUtils_memCompare( &pInitiateKey->certificate[ g_ISSUER_ADDRESS_LOCATION_c ],
                             &( App_GetCertificate()[ g_ISSUER_ADDRESS_LOCATION_c ] ),
                              g_MAX_ADDR_LENGTH_c ) ) {

                cbkestatus = g_KeyEstTerminateUnknownIssuer_c;
                CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
                return;
            }
            /* Returns g_TRUE_c if address is found in the table and updates the
               aIEEE address */
            if( g_ENTRY_NOT_FOUND_c != APS_APSMEsearchAddress( &p_dataInd->src_address.short_address,
                                       aIEEEAddress, g_SEARCH_BY_SHORT_ADDRESS_c ) ) {
                  uint8_t deviceIEEEAddress[8];
/*                  memUtils_memReverseCopy( deviceIEEEAddress, aIEEEAddress, g_MAX_ADDR_LENGTH_c );*/

                  memUtils_memCopy( deviceIEEEAddress, aIEEEAddress, g_MAX_ADDR_LENGTH_c );

                /*  memUtils_memCompare returns 0x00 for success and 0x01 if doesnt match.
                   If the value returned is 0x01, then the IEEE address is not matching
                   hence send bad message */
                if( ( memUtils_memCompare( &pInitiateKey->certificate[ g_DEVICE_ADDRESS_LOCATION_c ],
                                    deviceIEEEAddress, g_MAX_ADDR_LENGTH_c ) ) ) {

                    cbkestatus = g_KeyEstTerminateBadMessage_c;
                    CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
                    return;
                }
            }
            /* All the above conditions are applicable for Initiate key
               establishment request/response commands */
            if( pZCLEvent->eventId == g_KeyEstInitiateKeyEstReqEvent_c ) {
                if( ! App_getLinkKeyEntry( aIEEEAddress ) ) {

                    cbkestatus = g_KeyEstTerminateNoResources_c;
                    CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
                    return;
                }
                /* Validate if the initiate key request command is received from a Trust
                   Center or the received device is a Trust center.otherwise send Terminate
                   key establishment command with No Resources */
                if( ( p_dataInd->src_address.short_address != App_getTCShortAddress() ) &&
                    ( g_GET_NIB(g_NWK_ShortAddress_c) != App_getTCShortAddress() ) ) {

                    cbkestatus = g_KeyEstTerminateNoResources_c;
                    CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
                    return;
                }
                CBKE_ChangeState( PROCESS_KEY_REQ_STATE );
                App_getOrSetCBKEInfo( g_SET_c, PARTNERSHORTADDRESS,
                                     (uint8_t*)&p_dataInd->src_address.short_address );
                CBKE_KeyDetails.partnerEndpoint = p_dataInd->src_endpoint;
            }
            else {
                CBKE_DataTimeout.eventReceivedStartTime =  halCommonGetInt32uMillisecondTick();
                CBKE_ChangeState( PROCESS_KEY_EST_RESP_STATE );
            }
            App_getOrSetCBKEInfo( g_SET_c, PARTNERCERTIFICATE, pInitiateKey->certificate );
            App_getOrSetCBKEInfo( g_SET_c, PARTNERIEEEADDRESS,
                                  &pInitiateKey->certificate[g_DEVICE_ADDRESS_LOCATION_c] );

            ephemeralDataTimeout = pInitiateKey->ephemeralDataGenerateTime;
            confirmDataTimeout = pInitiateKey->confirmKeyGenerateTime;
            CBKE_DataTimeout.ephemeralDataStartTimer = g_TRUE_c;
            CBKE_DataTimeout.ephemeralDataStartTime = App_getcurrentTick();
        }
        break;

        case g_KeyEstEphemeralDataReqEvent_c:
        case g_KeyEstEphemeralDataRespEvent_c:
            /* check if the device is in the middle of CBKE mechanism, if a key
               establishment command is received from a third device, then send
               terminate key establishment command */
            if( ( CBKE_KeyDetails.CBKEInProcess ) &&
                ( p_dataInd->src_address.short_address != partnerShortAddress ) ) {

                cbkestatus = g_KeyEstTerminateBadMessage_c;
                CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
				return;
            }
            /* if ephemeral data request/response command is received and the previous
               key established state is not in the appropriate state, then
               terminate the key establishment command */

            if( ( ( pZCLEvent->eventId == g_KeyEstEphemeralDataRespEvent_c ) &&
                  ( CBKE_GetPreviousState() != PROCESS_KEY_EST_RESP_STATE ) )  ||
                ( ( pZCLEvent->eventId == g_KeyEstEphemeralDataReqEvent_c )  &&
                  ( CBKE_GetPreviousState() != PROCESS_KEY_REQ_STATE ) ) ) {

                cbkestatus = g_KeyEstTerminateBadMessage_c;
                CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
				return;
            }
			/* if all the validations are successful, change the current state to create
			  ephemeral data request/response command */
			CBKE_DataTimeout.eventReceivedStartTime =  halCommonGetInt32uMillisecondTick();
			if( pZCLEvent->eventId == g_KeyEstEphemeralDataReqEvent_c ) {
			   CBKE_ChangeState( PROCESS_EPHEMERAL_DATA_REQ_STATE );
			}
			else {
				CBKE_ChangeState( PROCESS_EPHEMERAL_DATA_RESP_STATE );
			}
			/*copy the received ephemeral public key both on initiator/responder device
			location 0x03 - points to the ephemeral data number*/
			App_getOrSetCBKEInfo( g_SET_c, PARTNEREPHEMERALPUBLICKEY, pZCLEvent->pEventData );

			CBKE_DataTimeout.confirmDataStartTimer = g_TRUE_c;
			CBKE_DataTimeout.confirmDataStartTime = App_getcurrentTick();
			/*reset the timer*/
			CBKE_DataTimeout.ephemeralDataStartTimer = g_FALSE_c;
            break;

        case g_KeyEstConfirmKeyDataReqEvent_c:
        case g_KeyEstConfirmKeyDataRespEvent_c:
        {
            uint8_t generatedMac[AES_MMO_HASH_SIZE];
            /*check if the device is in the middle of CBKE mechanism, if a key
            establishment command is received from a third device, then send
            terminate key establishment command*/
            if( ( CBKE_KeyDetails.CBKEInProcess ) &&
                ( p_dataInd->src_address.short_address != partnerShortAddress ) ) {

                cbkestatus = g_KeyEstTerminateBadMessage_c;
                CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
				return;
            }
            /*if confirm data request/response command is received and the previous
            key established state is not in the appropriate state, then
            terminate the key establishment command*/
            if( ( ( pZCLEvent->eventId == g_KeyEstConfirmKeyDataRespEvent_c ) &&
                  ( CBKE_GetPreviousState() != PROCESS_EPHEMERAL_DATA_RESP_STATE ) ) ||
                ( ( pZCLEvent->eventId == g_KeyEstConfirmKeyDataReqEvent_c ) &&
                  ( CBKE_GetPreviousState() != PROCESS_EPHEMERAL_DATA_REQ_STATE ) ) ) {

                cbkestatus = g_KeyEstTerminateBadMessage_c;
                CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
				return;
            }
            /*if all the validations are successful , change the current state to create
            confirm data request/response command*/
            if( pZCLEvent->eventId ==  g_KeyEstConfirmKeyDataReqEvent_c ) {
                /* if the MACU generated and received doesn't match then send
                   terminate key establishment command with status as Bad key confirm
                   memUtils_memCompare returns 1 if it doesn't match.*/
                App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACU, generatedMac );
                if( ( memUtils_memCompare( pZCLEvent->pEventData, generatedMac, AES_MMO_HASH_SIZE ) ) ) {
                    cbkestatus = g_KeyEstTerminateBad_Key_Confirm_c;
                    CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
                }
                else {
                    CBKE_ChangeState( PROCESS_CONFIRM_KEY_DATA_REQ_STATE );
                }
            }
            if( pZCLEvent->eventId ==  g_KeyEstConfirmKeyDataRespEvent_c ) {
                App_getOrSetCBKEInfo( g_GET_c, GENERATEDMACV, generatedMac );
                if( memUtils_memCompare( pZCLEvent->pEventData, generatedMac, AES_MMO_HASH_SIZE ) ) {
                    cbkestatus = g_KeyEstTerminateBad_Key_Confirm_c;
                    CBKE_ChangeState( INITIATE_TERMINATE_KEY_REQ_STATE );
                }
                else {
                    CBKE_ChangeState( PROCESS_CONFIRM_KEY_DATA_RESP_STATE );
                }
            }
            CBKE_DataTimeout.confirmDataStartTimer = g_FALSE_c;
            break;
        }
        case g_KeyEstTerminateKeyEstEvent_c:
            CBKE_ChangeState( TERMINATE_KEY_STATE );
            if ( p_dataInd->a_asdu[0] & g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {
                terminateEvent  +=  0x01;
            }
            break;
    }
}

/******************************************************************************/
void App_CBKE_Reset( void )
{
    memUtils_memSet( (uint8_t*)&CBKE_KeyDetails, 0x00, sizeof(CBKE_Info_t) );
    CBKE_KeyEstCurrentState = INVALID_STATE;
    CBKE_KeyEstPreviousState = INVALID_STATE;
    terminateKeyDestAddress = 0xFFFF;
    memUtils_memSet( (uint8_t*)&CBKE_DataTimeout, 0x00, sizeof( CBKE_DataTime_t ) );
}

/******************************************************************************/
uint8_t App_CBKEapplyOrVerifyDSAsignature ( uint8_t applyDSA, uint8_t messageLength,
                                            uint8_t* pMessage )
{
    uint8_t status;
    uint8_t digest[16];
    uint8_t rComponent[21];
    uint8_t sComponent[21];
    uint8_t partnerStaticPublicKey[22];

   if( g_TRUE_c == applyDSA ) {
       AppMCHPHashFunc( digest, messageLength, pMessage );
       status = ZSE_ECDSASign ( App_GetStaticPrivateKey(),
                               digest,
                               AppMCHPGetRandomDataFunc,
                               rComponent,
                               sComponent,
                               AppMCHPYieldFunc,
                               g_YIELD_LEVEL_c );

       if( MCE_SUCCESS == status ) {
           /* increment the pointer message by message length to point to
           the signature field.*/
           pMessage += messageLength;
           /* copy the generated 'r' component into the signature field*/
           memUtils_memCopy( pMessage, rComponent, SECT163K1_POINT_ORDER_SIZE );
           /*increment the size for 'r' component*/
           pMessage += SECT163K1_POINT_ORDER_SIZE;
           /*copy the 's' component in the signature field*/
           memUtils_memCopy ( pMessage, sComponent, SECT163K1_POINT_ORDER_SIZE );
       }
   }
   else {
        /*copy the 'r' component of the signature received in the report event status
        command to verify the same.42 bytes is subtracted to copy the 'r' component
        start location*/
        memUtils_memCopy ( rComponent,
          pMessage + messageLength - (SECT163K1_POINT_ORDER_SIZE + SECT163K1_POINT_ORDER_SIZE ),
          SECT163K1_POINT_ORDER_SIZE );
        /*copy the 's' component of the signature received in the report event status
        command to verify the same.21 bytes is subtracted to copy the 's' component
        start location*/
        memUtils_memCopy ( sComponent, pMessage + messageLength - SECT163K1_POINT_ORDER_SIZE,
            SECT163K1_POINT_ORDER_SIZE );

        /* hash the received message leaving the signature */
        AppMCHPHashFunc ( digest,
            (messageLength - (SECT163K1_POINT_ORDER_SIZE + SECT163K1_POINT_ORDER_SIZE)),
            pMessage );

        /* verify the signature, the ECC library returns the status
        based on the verification.The ECDSA signature should be verified with the Public key
        pait corresponding to the private key.Since the message will be signed using the
        private key, hence the remote device while verifying the signature should use the
        partner device's static public key to verify the signature.*/
        App_getOrSetCBKEInfo( g_GET_c, PARTNERSTATICPUBLICKEY, partnerStaticPublicKey );
        status =    ZSE_ECDSAVerify (
                        partnerStaticPublicKey,
                        digest,
                        rComponent,
                        sComponent,
                        AppMCHPYieldFunc,
                        g_YIELD_LEVEL_c );
   }
   return status;
}

/*******************************************************************************
* Private Functions
*******************************************************************************/

/* None */

/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/

/* None */


#endif  /* ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 ) */
/*******************************************************************************
* End Of File
*******************************************************************************/
