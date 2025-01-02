#ifndef _ZCL_HVAC_DEHUMIDIFICATION_CONTROL_CLUSTER_H_
#define _ZCL_HVAC_DEHUMIDIFICATION_CONTROL_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#if ( g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d == 1 )

/* An interface for controlling dehumidification */
#define g_DEHUMIDIFICATION_CONTROL_CLUSTER_c                    0x0203

/************* Dehumidification Control Attributes ******************************/
/* Attributes of the Dehumidification Information Attribute Set */
#define g_RELATIVE_HUMIDITY_c                                   0x0000
#define g_DEHUMIDIFICATION_COOLING_c                            0x0001

/* Attributes of the Dehumidification Settings Attribute Set */
#define g_RHDEHUMIDIFICATION_SETPOINT_c                         0x0010
#define g_RELATIVE_HUMIDITY_MODE_c                              0x0011
#define g_DEHUMIDIFICATION_LOCKOUT_c                            0x0012
#define g_DEHUMIDIFICATION_HYSTERISIS_c                         0x0013
#define g_DEHUMIDIFICATION_MAX_COOL_c                           0x0014
#define g_RELATIVE_HUMIDITY_DISPLAY_c                           0x0015
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct HVAC_Dehumidification_Control_Cluster_Attributes_Tag
{
#if ( g_RELATIVE_HUMIDITY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t relative_humidity;
#endif

   uint8_t dehumidification_cooling;
   uint8_t RHDehumidification_setpoint;

#if ( g_RELATIVE_HUMIDITY_MODE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t relative_humidity_mode;
#endif

#if ( g_DEHUMIDIFICATION_LOCK_OUT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t dehumidification_lockout;
#endif

   uint8_t dehumidification_hysteresis;
   uint8_t dehumidification_maxCool;

#if ( g_RELATIVE_HUMIDITY_DISPLAY_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t relative_humidity_display;
#endif

}__attribute__((__packed__))HVAC_Dehumidification_Control_Cluster;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

#if ( g_DEHUMIDIFICATION_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Dehumidification_Server_Control_Cluster_Info;
#endif

#if ( g_DEHUMIDIFICATION_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Dehumidification_Client_Control_Cluster_Info;
#endif

#endif              /* ( g_DEHUMIDIFICATION_CONTROL_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_HVAC_DHControl_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_HVAC_DEHUMIDIFICATION_CONTROL_CLUSTER_H_*/
