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

/*    0x01,0x80,0x12,0x99,0x6b,0x8a,0x5d,0xa7,0x3a,0x6a,0xe7,
    0xc8,0x40,0x08,0x66,0xef,0x51,0x1d,0x10,0xa6,0x9d  */
    0x03, 0xe3, 0xa7, 0x45, 0x17, 0xd3, 0x44 , 0x92, 0x75, 0x69,
	0x01, 0xe8, 0x0b, 0x6a, 0x9e, 0x33, 0x75, 0xc4, 0xde, 0x78, 0x5d
};

/*This holds the certificate for NON - ESP device*/
uint8_t test_certificate[SECT163K1_CERTIFICATE_SIZE] = {

   /* 0x03,0x06,0x73,0x90,0x03,0x00,0x7d,0xee,0x02,0x79,
    0xb1,0x2d,0xed,0xc7,0xe1,0xc8,0xe6,0xfa,0x51,0x6c,
    0xaf,0xc8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
    0x54,0x45,0x53,0x54,0x53,0x45,0x43,0x41,0x01,0x09,
    0x10,0x83,0x01,0x23,0x45,0x67,0x89,0x0a         */

	0x02, 0x00, 0xde, 0xa5, 0xb4, 0x91, 0x68,0xff, 0x98, 0x38,
	0x00, 0x4a, 0x25, 0xd4, 0xc2, 0xe3, 0x10, 0x5a, 0x2d, 0x64,
	0x38, 0xfb, 0x00, 0x80, 0xe1, 0x02, 0x00, 0x1b, 0xa2, 0x05,
	0x54, 0x45, 0x53, 0x54, 0x53, 0x45, 0x43, 0x41, 0x01, 0x09,
	0x10, 0x83, 0x01, 0x23, 0x45, 0x67, 0x89, 0x0a

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
