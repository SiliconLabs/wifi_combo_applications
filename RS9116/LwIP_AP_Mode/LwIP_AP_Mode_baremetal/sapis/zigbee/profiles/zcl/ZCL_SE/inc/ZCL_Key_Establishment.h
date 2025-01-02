/******************************************************************************
* ZCL_Key_Establishment.h
******************************************************************************
* Program Description:
* This file contains the Key Establishment cluster attribute and cmmnd
information
******************************************************************************/

#ifndef _ZCL_KEY_ESTABLISHMEMT_CLUSTER_H_
#define _ZCL_KEY_ESTABLISHMEMT_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZCL_Interface.h"
#include "ZCL_Functionality.h"


/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define g_KEY_ESTABLISHMENT_CLUSTER_c                     0x0800

#define g_KEY_ESTABLISHMENT_SUITE_ATTRIBUTE_c             0x0000


/*enumerations for key establishment server commands*/
enum keyEstServerCommands
{
    g_KeyEstInitiateKeyEstReq_c                 = 0x00,
    g_KeyEstEphemeralDataReq_c                  = 0x01,
    g_KeyEstConfirmKeyDataReq_c                 = 0x02,
    g_KeyEstTerminateKeyEst_c                   = 0x03
};

/*enumerations for key establishment client commands*/
enum keyEstClientCommands
{
    g_KeyEstInitiateKeyEstResp_c                = 0x00,
    g_KeyEstEphemeralDataResp_c                 = 0x01,
    g_KeyEstConfirmKeyDataResp_c                = 0x02
};

/*enumerations for key establishment events*/
enum keyEstEvents
{
    g_KeyEstInitiateKeyEstReqEvent_c            = 0xB9,
    g_KeyEstEphemeralDataReqEvent_c             = 0xBA,
    g_KeyEstConfirmKeyDataReqEvent_c            = 0xBB,
    g_KeyEstTerminateKeyEstEvent_c              = 0xBC,
    g_KeyEstInitiateKeyEstRespEvent_c           = 0xBD,
    g_KeyEstEphemeralDataRespEvent_c            = 0xBE,
    g_KeyEstConfirmKeyDataRespEvent_c           = 0xBF
};

enum keyEstTerminateStatusValues
{
    g_KeyEstTerminateReserved_c                 = 0x00,
    g_KeyEstTerminateUnknownIssuer_c            = 0x01,
    g_KeyEstTerminateBad_Key_Confirm_c          = 0x02,
    g_KeyEstTerminateBadMessage_c               = 0x03,
    g_KeyEstTerminateNoResources_c              = 0x04,
    g_KeyEstTerminateUnSupportedSuite_c         = 0x05,
};

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_KEY_ESTABLISHMENT_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_KeyEstablishment_Client_Cluster_Info;
#endif

#if ( g_KEY_ESTABLISHMENT_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_KeyEstablishment_Server_Cluster_Info;
#endif
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

/*Stucture for Key Establishment Cluster's Attributes*/
typedef struct KeyEstClusterAttributes_tag
{
    uint8_t keyEstSuite[2];
}__attribute__((__packed__))KeyEstClusterAttributes_t;


/*structure for Initiate Key establishment request command*/
typedef struct KeyEstInitiateKeyReqCommand_tag
{
    uint8_t keyEstSuite[2];
    uint8_t ephemeralDataGenerateTime;
    uint8_t confirmKeyGenerateTime;
    uint8_t certificate[48]; /* 48 bytes of certificate.It is fixed size */
}__attribute__((__packed__))KeyEstInitiateKeyReqCommand_t;


/*structure for Initiate Key establishment response command*/
typedef struct KeyEstInitiateKeyRespCommand_tag
{
    uint8_t keyEstSuite[2];
    uint8_t ephemeralDataGenerateTime;
    uint8_t confirmKeyGenerateTime;
    uint8_t certificate[48]; /*48 bytes of certificate.It is fixed size*/
}__attribute__((__packed__))KeyEstInitiateKeyRespCommand_t;



/*structure for Ephemeral Data request command*/
typedef struct KeyEstEphemeralDataReqCommand_tag
{
    uint8_t ephemeralDataNumber[22]; /*22 bytes of ephemeral number.It is fixed size*/
}__attribute__((__packed__))KeyEstEphemeralDataReqCommand_t;


/*structure for Ephemeral Data response command*/
typedef struct KeyEstEphemeralDataRespCommand_tg
{
    uint8_t ephemeralDataNumber[22]; /*22 bytes of ephemeral number.It is fixed size*/
}__attribute__((__packed__))ZCL_KeyEstEphemeralDataRespCommand_t;


/*structure for Confirm Data request command*/
typedef struct KeyEstConfirmDataReqCommand_tag
{
    uint8_t message[16]; /*16 bytes of Message Authentication code -  MAC U or MAC V*/
}__attribute__((__packed__))ZCL_KeyEstConfirmDataReqCommand_t;

/*structure for Confirm Data response command*/
typedef struct KeyEstConfirmDataRespCommand_tag
{
    uint8_t message[16]; /*16 bytes of Message Authentication code -  MAC U or MAC V*/
}__attribute__((__packed__))ZCL_KeyEstConfirmDataRespCommand_t;


/*structure for Terminate Key Establishment command*/
typedef struct KeyEstTerminateKeyEstCommand_tag
{
    uint8_t status;
    uint8_t waitTime;
    uint16_t keyEstSuite;
}__attribute__((__packed__))KeyEstTerminateKeyEstCommand_t;

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

uint8_t ZCL_Handle_KeyEstablishment_Cluster_Cmd ( uint8_t endPointId,
                                      uint8_t asduLength,
                                      uint8_t* pReceivedAsdu,
                                      uint8_t* pResponseLocation,
                                      ZCL_ActionHandler_t *pActionHandler,
                                      ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                      uint8_t * pAttrsDataLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                      /*_ZCL_KEY_ESTABLISHMEMT_CLUSTER_H_*/
