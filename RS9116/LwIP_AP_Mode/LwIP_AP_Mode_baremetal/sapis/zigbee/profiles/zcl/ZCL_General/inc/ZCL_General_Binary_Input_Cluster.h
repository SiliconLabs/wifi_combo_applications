/******************************************************************************
* ZCL_General_Binary_Input_Cluster.h
******************************************************************************
* Program Description:
* This file contains the BInary Input Cluster attribute and cmmnd information
******************************************************************************/
#ifndef _ZCL_GENERAL_BINARY_INPUT_CLUSTER_H_
#define _ZCL_GENERAL_BINARY_INPUT_CLUSTER_H_


#if (g_BINARY_INPUT_CLUSTER_ENABLE_d == 1)
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* Binary input cluster */
#define g_BINARY_INPUT_CLUSTER_c                                    0x000F

/* Attributes for Binary INput Cluster */
#define g_ACTIVE_TEXT_ATTRIBUTE_c                                   0x0004
#define g_DESCRIPTION_ATTRIBUTE_c                                   0x001C
#define g_INACTIVE_TEXT_ATTRIBUTE_c                                 0x002E
#define g_OUT_OF_SERVICE_ATTRIBUTE_c                                0x0051
#define g_POLARITY_ATTRIBUTE_c                                      0x0054
#define g_PRESENT_VALUE_ATTRIBUTE_c                                 0x0055
#define g_RELIABILITY_ATTRIBUTE_c                                   0x0067
#define g_STATUS_FLAGS_ATTRIBUTE_c                                  0x006F
#define g_APPLICATION_TYPE_ATTRIBUTE_c                              0x0100


/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_BINARY_INPUT_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Binary_Input_Server_Cluster_Info;
#endif                              /* g_BINARY_INPUT_SERVER_CLUSTER_ENABLE_d */

#if ( g_BINARY_INPUT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Binary_Input_Client_Cluster_Info;
#endif                              /* g_BINARY_INPUT_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
Device Binary Input cluster structure - attributes
Field: active text
Field: description
Filed: inactive_text
Field: out_of_service
Field: polarity
Field: present_value
Field: reliabilty
Field: status_flag
Field: a_application_type
-----------------------------------------------------------------------------*/
typedef struct Binary_Input_Cluster_Tag
{
#if ( g_ACTIVE_TEXT_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_active_text[16];
#endif                                  /* g_ACTIVE_TEXT_ATTRIBUTE_ENABLE_d */
#if ( g_DESCRIPTION_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_description[16];
#endif                                  /* g_DESCRIPTION_ATTRIBUTE_ENABLE_d */
#if ( g_INACTIVE_TEXT_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_inactive_text[16];
#endif                                  /* g_INACTIVE_TEXT_ATTRIBUTE_ENABLE_d */

    uint8_t a_out_of_service;

#if ( g_POLARITY_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_polarity;
#endif                                  /* g_POLARITY_ATTRIBUTE_ENABLE_d */

    uint8_t a_present_value;

#if ( g_RELIABILITY_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_reliabilty;
#endif                                  /* g_RELIABILITY_ATTRIBUTE_ENABLE_d */

    uint8_t a_status_flag;

#if ( g_APPLICATION_TYPE_ATTRIBUTE_ENABLE_d == 1 )
    uint8_t a_application_type[4];
#endif                              /* g_APPLICATION_TYPE_ATTRIBUTE_ENABLE_d */
}__attribute__((__packed__))Binary_Input_Cluster_t;

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Binary_Input_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

#endif                                      /* g_BASIC_INPUT_CLUSIER_ENABLE_d */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/


#endif                                 /*_ZCL_GENERAL_BINARY_INPUT_CLUSTER_H_*/
