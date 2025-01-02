#ifndef _ZCL_HVAC_FAN_CONTROL_CLUSTER_H_
#define _ZCL_HVAC_FAN_CONTROL_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

# if( g_FAN_CONTROL_CLUSTER_ENABLE_d ==  1 )

/*An interface for controlling a fan in a heating / cooling system */
#define g_FAN_CONTROL_CLUSTER_c                             0x0202

/************* Fan Control Cluster   Attributes ******************************/
#define g_FAN_MODE_c                                        0x0000
#define g_FAN_MODE_SEQUENCE_c                               0x0001

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct HVAC_Fan_Control_Cluster_Attributes_Tag
{
   uint8_t fan_mode;
   uint8_t fan_mode_sequence;
}__attribute__((__packed__))HVAC_Fan_Control_Cluster_Attributes;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/* Fan control  cluster info */
#if ( g_FAN_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Fan_Control_Server_Cluster_Info;
#endif

#if ( g_FAN_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Fan_Control_Client_Cluster_Info;
#endif



/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_HVAC_FanControl_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

#endif                      /* ( g_FAN_CONTROL_CLUSTER_ENABLE_d ==  1) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_HVAC_FAN_CONTROL_CLUSTER_H_*/
