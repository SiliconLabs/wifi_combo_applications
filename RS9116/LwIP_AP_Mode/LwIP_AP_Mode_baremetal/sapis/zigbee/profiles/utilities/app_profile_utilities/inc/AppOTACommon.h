/******************************************************************************
* AppOTACommon.h
*******************************************************************************
* Program Description:
* This file contains the common includes for OTA application
*******************************************************************************/
#ifndef __APP_OTA_COMMON_H_
/*******************************************************************************
* Includes
*******************************************************************************/

/* NONE */

/*******************************************************************************
* Global Constants
*******************************************************************************/
#define g_OTA_FILE_ID_c                     (0x0BEEF11Eu)

/******************************** OTA Header **********************************/

/* Security credential field bit mask */
#define g_FIELD_CTRL_SEC_CRED_BIT_MASK_c                0x01u

/* file destination field bit mask */
#define g_FIELD_CTRL_DEV_SPEC_BIT_MASK_c                0x02u

/* hardware version field bit mask */
#define g_FIELD_CTRL_HRD_VER_BIT_MASK_c                 0x04u

/* file id field */
#define g_OTA_HDR_FILE_ID_LOC(x)            ((uint8_t*)x)
#define g_OTA_HDR_FILE_ID_IN(x)             \
    (*(uint32_t*)g_OTA_HDR_FILE_ID_LOC(x))

/* header version */
#define g_OTA_HDR_VERSION_LOC(x)            (((uint8_t*)x) + 4u)
#define g_OTA_HDR_VERSION_IN(x)             \
    (*(uint16_t*)g_OTA_HDR_VERSION_LOC(x))

/* header length */
#define g_OTA_HDR_LEN_LOC(x)                (((uint8_t*)x) + 6u)
#define g_OTA_HDR_LEN_IN(x)                 (*(uint16_t*)g_OTA_HDR_LEN_LOC(x))

/* field control */
#define g_OTA_HDR_FIELD_CONTROL_LOC(x)      (((uint8_t*)x) + 8u)
#define g_OTA_HDR_FIELD_CONTROL_IN(x)       \
    (*(uint16_t*)g_OTA_HDR_FIELD_CONTROL_LOC(x))

/* manufacturer code */
#define g_OTA_HDR_MANF_CODE_LOC(x)           (((uint8_t*)x) + 10u)
#define g_OTA_HDR_MANF_CODE_IN(x)           \
    (*(uint16_t*)g_OTA_HDR_MANF_CODE_LOC(x))

/* image type */
#define g_OTA_HDR_IMAGE_TYPE_LOC(x)         (((uint8_t*)x) + 12u)
#define g_OTA_HDR_IMAGE_TYPE_IN(x)          \
    (*(uint16_t*)g_OTA_HDR_IMAGE_TYPE_LOC(x))

/* file version */
#define g_OTA_HDR_FILE_VER_LOC(x)           (((uint8_t*)x) + 14u)
#define g_OTA_HDR_FILE_VER_IN(x)            \
    (*(uint32_t*)g_OTA_HDR_FILE_VER_LOC(x))

/* stack version */
#define g_OTA_HDR_ZIGBEE_STACK_VER_LOC(x)   (((uint8_t*)x) + 18u)
#define g_OTA_HDR_ZIGBEE_STACK_VER_IN(x)    \
    (*(uint16_t*)g_OTA_HDR_ZIGBEE_STACK_VER_LOC(x))

/* header string */
#define g_OTA_HDR_STRING_SIZE_c             (0x20u)
#define g_OTA_HDR_STRING_LOC(x)             (((uint8_t*)x) + 20u)

/* image size */
#define g_OTA_HDR_IMAGE_SIZE_LOC(x)         (((uint8_t*)x) + 52u)
#define g_OTA_HDR_IMAGE_SIZE_IN(x)          \
    (*(uint32_t*)g_OTA_HDR_IMAGE_SIZE_LOC(x))

/* security credential version */
#define g_OTA_HDR_SEC_CRED_VER_LOC(x)       \
    (((uint8_t*)x) + g_OTA_HDR_MANDATORY_FIELDS_SIZE_c)
#define g_OTA_HDR_SEC_CRED_VER_IN(x)        \
    (*(uint8_t*)g_OTA_HDR_SEC_CRED_VER_LOC(x))

/* file destination */
#define g_OTA_HDR_FILE_DEST_LOC(x)          (((uint8_t*)x) +\
    ((g_OTA_HDR_FIELD_CONTROL_IN(x) & g_FIELD_CTRL_SEC_CRED_BIT_MASK_c) ? 1u : 0u))

/* minimum hardware version */
#define g_OTA_HDR_MIN_HARDWARE_VER_LOC(x)   ((g_OTA_HDR_FILE_DEST_LOC(x)) +\
    ((g_OTA_HDR_FIELD_CONTROL_IN(x) & g_FIELD_CTRL_DEV_SPEC_BIT_MASK_c) ? 8u : 0u))
#define g_OTA_HDR_MIN_HARDWARE_VER_IN(x)    (*(uint16_t*)g_OTA_HDR_MIN_HARDWARE_VER_LOC(x))

/* maximum hardware version */
#define g_OTA_HDR_MAX_HARDWARE_VER_LOC(x)   ((g_OTA_HDR_FILE_DEST_LOC(x)) +\
    ((g_OTA_HDR_FIELD_CONTROL_IN(x) & g_FIELD_CTRL_DEV_SPEC_BIT_MASK_c) ? 8u : 0u)\
         + 2u)
#define g_OTA_HDR_MAX_HARDWARE_VER_IN(x)    \
    (*(uint16_t*)g_OTA_HDR_MAX_HARDWARE_VER_LOC(x))

/* minimum header size */
#define g_OTA_HDR_MANDATORY_FIELDS_SIZE_c   (0x38u)

/* maximum header size */
#define g_OTA_HDR_MAX_SIZE_IN_BYTES_c       (0x48u)

/****************************** Sub element ***********************************/
/* sub element tag */
#define g_OTA_SUB_ELEMENT_TAG_ID_LOC(x)     (((uint8_t*)x))
#define g_OTA_SUB_ELEMENT_TAG_ID_IN(x)      \
    (*(uint16_t*)g_OTA_SUB_ELEMENT_TAG_ID_LOC(x))

/* sub element length */
#define g_OTA_SUB_ELEMENT_TAG_LEN_LOC(x)    (((uint8_t*)x) + 2u)
#define g_OTA_SUB_ELEMENT_TAG_LEN_IN(x)     \
    (*(uint32_t*)g_OTA_SUB_ELEMENT_TAG_LEN_LOC(x))

/* sub element header size */
#define g_OTA_SUB_ELEMENT_HDR_SIZE_c        (0x06u)

/* ECDSA Signature Sub-element size in bytes (tagID :2, lenghtFiled : 4,
signer IEEE Address : 8, ,signature data: 42 bytes , sum = 56 bytes*/
#define g_OTA_SUB_ELEMENT_ECDSA_SIGNATUTRE_LEN_c	0x38u    /*i.e 56 */

/*ECDSA Signing Certificate Sub-element size in bytes  (tagID :2,
lenghtFiled : 4,  ECDSA Certificate : 48 bytes , sum = 54 bytes  */
#define g_OTA_SUB_ELEMENT_ECDSA_SIGNING_CERTIFICATE_LEN_c	0x36u    /*i.e 54 */


#define g_OTA_UPGRADE_IMAGE_TAG_ID_c        	(0x0000u)
#define g_OTA_ECDSA_SIGNATURE_TAG_ID_c      	(0x0001u)
#define g_OTA_ECDSA_SIGNATURE_LENGTH_FIELD_c    (0x00000032)

#define g_OTA_ECDSA_CERTIFICATE_TAG_ID_c    	(0x0002u)
#define g_OTA_ECDSA_CERTIFCATE_LENGTH_FIELD_c 	(0x00000030)

#define g_MAX_SUPPORTED_TAG_ID_c            (0x0002u)
/********************************************************************************
* Memory Declarations
*********************************************************************************/

/* NONE */

/********************************************************************************
* Type Definitions
*********************************************************************************/

/********************************************************************************
   OTA cluster command and attributes structure
********************************************************************************/

/* NONE */

/********************************************************************************
* Type Definitions
*********************************************************************************/

/* NONE */

/*********************************************************************************/
/* Function Prototypes                                                           */
/*********************************************************************************/

bool App_getImageTagLocation( uint16_t tagId,
                                     uint32_t* tagOffset, uint32_t* tagSize );
/********************************************************************************
* End Of File
*********************************************************************************/
#define __APP_OTA_COMMON_H_
#endif /*__APP_OTA_COMMON_H_*/
