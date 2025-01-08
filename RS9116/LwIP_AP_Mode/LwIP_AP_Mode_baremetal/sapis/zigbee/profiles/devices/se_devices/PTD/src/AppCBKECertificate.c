/*******************************************************************************
* AppCBKECertificate.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Key Establishment
*   cluster
*
*******************************************************************************/
#include "ZCL_Common.h"
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"
#include "memory_utility.h"
#if( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 )
#include "eccapi.h"
/********************************************************************************
* Global Constants
********************************************************************************/


/*******************************************************************************
* Public Memory declarations
*******************************************************************************/
uint8_t static_privateKey[SECT163K1_PRIVATE_KEY_SIZE] = {

    0x03,0x14,0x85,0xd3,0xc6,0xf6,0x17,0x5d,0xd5,0x8b,0x73,
    0x4b,0x10,0x80,0x23,0x0f,0xfc,0x45,0xca,0xbc,0x9f

};

/*This holds the certificate for NON - ESP device*/
uint8_t test_certificate[SECT163K1_CERTIFICATE_SIZE] = {

    0x03,0x03,0x60,0xf4,0x69,0xc4,0x64,0xd9,0x2b,0xdf,0xd5,
    0xa4,0x51,0xd0,0x8c,0xe9,0x40,0x56,0xfd,0x49,0x8d,0x42,
    0x00,0x80,0xe1,0x02,0x00,0x1a,0xb5,0x85,0x54,0x45,0x53,
    0x54,0x53,0x45,0x43,0x41,0x01,0x09,0x10,0x83,0x01,0x23,
    0x45,0x67,0x89,0x0a

};

/*This holds the certificate authority's public key*/
uint8_t ca_publicKey[SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE] = {

    0x02,0x00,0xfd,0xe8,0xa7,0xf3,0xd1,0x08,0x42,0x24,0x96,
    0x2a,0x4e,0x7c,0x54,0xe6,0x9a,0xc3,0xf0,0x4d,0xa6,0xb8
};

/*******************************************************************************
* Private Memory declarations
*******************************************************************************/


/*******************************************************************************
* Function Prototypes
*******************************************************************************/


/*******************************************************************************
* Public Functions
*******************************************************************************/

uint8_t* App_GetCertificate( void )
{
    return test_certificate;
}
/*******************************************************************************/

uint8_t* App_GetStaticPrivateKey( void )
{
    return static_privateKey;
}

/*******************************************************************************/
uint8_t* App_GetCAPublicKey( void )
{
    return ca_publicKey;
}

/*******************************************************************************/
void App_SetCertificate( uint8_t *pCertificate )
{
    memUtils_memCopy( test_certificate, pCertificate, SECT163K1_CERTIFICATE_SIZE );
}

/*******************************************************************************/
void App_SetStaticPrivateKey( uint8_t *pPrivateKey )
{
    memUtils_memCopy( static_privateKey, pPrivateKey, SECT163K1_PRIVATE_KEY_SIZE );
}

/*******************************************************************************/
void App_SetCAPublicKey( uint8_t *pPublicKey )
{
    memUtils_memCopy( ca_publicKey, pPublicKey, SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE );
}


/*******************************************************************************
* Private Functions
*******************************************************************************/

/* None */

/*******************************************************************************
* Interrupt Service Routines
*******************************************************************************/

/* None */

#endif   /* ( g_KEY_ESTABLISHMENT_CLUSTER_ENABLE_d == 1 ) */
/*******************************************************************************
* End Of File
*******************************************************************************/
