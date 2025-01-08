/*******************************************************************************
* AppCBKECertificate.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Key Establishment
*   cluster
*
*******************************************************************************/
#include "ZigBeeDataTypes.h"
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

/*    0x00,0xdc,0x19,0xf7,0x0c,0xbf,0xec,0x8f,0xbf,0x01,0x50,
    0xea,0xfb,0xe6,0x54,0xd8,0x47,0x57,0x19,0x22,0x49 */
/*    0080e102001ba6bf  */
 /*     0x02, 0x66, 0xfb, 0xb2, 0x59, 0x7d, 0xfa, 0x80, 0xd0, 0x2d,
	 0x39, 0x78, 0x74, 0x8e, 0x2c, 0x53, 0x35, 0x3a, 0xb9, 0x7f, 0x71  */

  /*     0080e102001ba5d2  */
	0x00, 0x15, 0xd4, 0x16, 0x07, 0xd5, 0xeb, 0xa1, 0x8d, 0x2b,
	0xe2, 0x6f, 0xb1, 0x4f, 0x93, 0x39, 0xbe, 0xac, 0xe4, 0xbf, 0xae
};

/*This holds the certificate for NON - ESP device*/
uint8_t test_certificate[SECT163K1_CERTIFICATE_SIZE] = {

/*     0x02,0x00,0xbe,0x49,0xef,0x00,0xef,0x16,0x14,0xdc,0x6e,0x3c,0x47,
    0x75,0x62,0xb7,0x79,0xa1,0x49,0xd1,0x0e,0x98,0x00,0x80,0xe1,0x02,
    0x00,0x1b,0xa3,0x52,0x54,0x45,0x53,0x54,0x53,0x45,0x43,0x41,0x01,
    0x09,0x10,0x83,0x01,0x23,0x45,0x67,0x89,0x0a */
/*    0080e102001ba6bf  */
/*  0x02, 0x00,0xf5, 0xdb, 0x09, 0x6b, 0x66, 0x46, 0x91, 0x3d, 0x3b, 0xbc,
  0xd0, 0xef, 0x9e, 0x5d, 0x9c, 0x6e, 0x93, 0xcf, 0x2c, 0x8c,0x00, 0x80,
  0xe1, 0x02, 0x00, 0x1b, 0xa6, 0xbf, 0x54, 0x45, 0x53, 0x54, 0x53, 0x45,
  0x43, 0x41, 0x01, 0x09, 0x10, 0x83, 0x01, 0x23 ,0x45, 0x67, 0x89, 0x0a  */
    /*     0080e102001ba5d2  */
  0x02, 0x00, 0x39, 0x0f ,0x2c, 0x90, 0xb9, 0x4b, 0xab, 0x52, 0x49, 0x03,
  0x65, 0xdb, 0x57, 0x89, 0xd9, 0xac, 0x4f, 0x40, 0x3a, 0xaa,0x00, 0x80,
  0xe1, 0x02, 0x00, 0x1b, 0xa5, 0xd2, 0x54, 0x45, 0x53, 0x54, 0x53, 0x45,
  0x43, 0x41, 0x01, 0x09, 0x10, 0x83, 0x01, 0x23,0x45, 0x67, 0x89, 0x0a
};

/*This holds the certificate authority's public key*/
uint8_t ca_publicKey[SECT163K1_COMPRESSED_PUBLIC_KEY_SIZE] = {

   /*  0x02,0x00,0xfd,0xe8,0xa7,0xf3,0xd1,0x08,0x42,0x24,0x96,
    0x2a,0x4e,0x7c,0x54,0xe6,0x9a,0xc3,0xf0,0x4d,0xa6,0xb8 */

  0x02, 0x00, 0xfd, 0xe8, 0xa7, 0xf3, 0xd1, 0x08, 0x42, 0x24, 0x96,
  0x2a, 0x4e, 0x7c, 0x54, 0xe6, 0x9a, 0xc3, 0xf0, 0x4d, 0xa6, 0xb8
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