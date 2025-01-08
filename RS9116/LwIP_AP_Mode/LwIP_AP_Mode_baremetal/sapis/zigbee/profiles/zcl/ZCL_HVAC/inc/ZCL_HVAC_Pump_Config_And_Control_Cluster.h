#ifndef _ZCL_HVAC_PUMP_CONFIG_AND_CONTROL_CLUSTER_H_
#define _ZCL_HVAC_PUMP_CONFIG_AND_CONTROL_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 )
/* An interface for configuring and controlling pumps */
#define g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_c                  0x0200

/* Attributes of the Pump Information Attribute Set */
#define g_MAX_PRESSURE_c                                    0x0000
#define g_MAX_SPEED_c                                       0x0001
#define g_MAX_FLOW_c                                        0x0002
#define g_MIN_CONST_PRESSURE_c                              0x0003
#define g_MAX_CONST_PRESSURE_c                              0x0004
#define g_MIN_COMP_PRESSURE_c                               0x0005
#define g_MAX_COMP_PRESSURE_c                               0x0006
#define g_MIN_CONST_SPEED_c                                 0x0007
#define g_MAX_CONST_SPEED_c                                 0x0008
#define g_MIN_CONST_FLOW_c                                  0x0009
#define g_MAX_CONST_FLOW_c                                  0x000a
#define g_MIN_CONST_TEMP_c                                  0x000b
#define g_MAX_CONST_TEMP_c                                  0x000c

/* Attributes of the Pump Dynamic Information Attribute Set */
#define g_PUMP_STATUS_c                                     0x0010
#define g_EFFECTIVE_OPERATION_MODE_c                        0x0011
#define g_EFFECTIVE_CONTROL_MODE_c                          0x0012
#define g_CAPACITY_c                                        0x0013
#define g_SPEED_c                                           0x0014
#define g_LIFETIME_RUNNING_HOURS_c                          0x0015
#define g_POWER_c                                           0x0016
#define g_LIFETIME_ENERGY_CONSUMED_c                        0x0017

/* Attributes of the Pump Settings Attribute Set */
#define g_OPERATION_MODE_c                                   0x0020
#define g_CONTROL_MODE_c                                     0x0021
#define g_ALARM_MASK_c                                       0x0022

typedef enum
{
   g_Pump_Status_General_Fault = 0x13
}Pump_Status;

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct HVAC_Pump_Configuration_And_Control_Cluster_Attributes_Tag
{
   uint8_t a_max_pressure[2];
   uint8_t a_max_speed[2];
   uint8_t a_max_flow[2];
#if ( g_MIN_CONST_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_min_const_pressure[2];
#endif

#if ( g_MAX_CONST_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_max_const_pressure[2];
#endif

#if ( g_MIN_COMP_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_min_comp_pressure[2];
#endif

#if ( g_MIN_COMP_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_max_comp_pressure[2];
#endif

#if ( g_MIN_CONST_SPEED_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_min_const_speed[2];
#endif

#if ( g_MAX_CONST_SPEED_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_max_const_speed[2];
#endif

#if ( g_MIN_CONST_FLOW_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_min_const_flow[2];
#endif

#if ( g_MAX_CONST_FLOW_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_max_const_flow[2];
#endif

#if ( g_MIN_CONST_TEMP_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_min_const_temp[2];
#endif

#if ( g_MAX_CONST_TEMP_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_max_const_temp[2];
#endif

#if ( g_PUMP_STATUS_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_pump_status[2];
#endif

   uint8_t effective_operation_mode;
   uint8_t effective_control_mode;
   uint8_t a_capacity[2];

#if ( g_SPEED_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_speed[2];
#endif

#if ( g_LIFETIME_RUNNING_HOURS_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_life_time_running_hours[3];
#endif

#if ( g_POWER_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_power[3];
#endif

#if ( g_LIFETIME_ENERGY_CONSUMED_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_life_time_energy_consumed[4];
#endif

   uint8_t operation_mode;

#if ( g_CONTROL_MODE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t control_mode;
#endif

#if ( g_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t a_alarm_mask[2];
#endif

}__attribute__((__packed__))HVAC_Pump_Configuration_And_Control_Cluster_Attributes;

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/* Pump configuration cluster info */
#if ( g_PUMP_CONFIGURATION_AND_CONTROL_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Pump_Config_And_Control_Server_Cluster_Info;
#endif

#if ( g_PUMP_CONFIGURATION_AND_CONTROL_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Pump_Config_And_Control_Client_Cluster_Info;
#endif


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_HVAC_PumpConfig_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

#endif /* ( g_PUMP_CONFIGURATION_AND_CONTROL_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                 /*_ZCL_HVAC_PUMP_CONFIG_AND_CONTROL_CLUSTER_H_*/
