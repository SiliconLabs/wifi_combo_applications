#ifndef _ZCL_HVAC_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_H_
#define _ZCL_HVAC_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_H_


/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d == 1 )
/* An interface for configuring the user interface of a thermostat which may be
remote from the thermostat */
#define g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_c        0x0204

/************* Thermostat User Interface Configuration Cluster ******************/

#define g_TEMPERATURE_DISPLAY_MODE_ATTRIBUTE_c                        0x0000
#define g_KEYPAD_LOCKOUT_ATTRIBUTE_c                                  0x0001

#if(g_HA_1_2_d == 1)	
#define g_SCHEDULE_PROGRAMMING_VISIBILTY_ATTRIBUTE_c				  0x0002
#endif

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef struct HVAC_Thermostat_User_Interface_Configuration_Cluster_Attributes_Tag
{
   uint8_t temperature_display_mode;
   uint8_t keypad_lockout;
#if(g_HA_1_2_d == 1)	
   uint8_t schedule_programming_visibility;

#endif
}__attribute__((__packed__))HVAC_Thermostat_User_Interface_Configuration_Cluster_Attributes;

#if(g_HA_1_2_d == 1)	
enum schedule_programming_visibility {
     local_schedule_programming_functionality_enabled   =0x00,
	 local_schedule_programming_functionality_disabled  =0x01
};
#endif
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/* Thermostat User Interface Configuration Cluster cluster info */
#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Thermostat_User_Interface_Config_Server_Cluster_Info;
#endif

#if ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_HVAC_Thermostat_User_Interface_Config_Client_Cluster_Info;
#endif


/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
uint8_t ZCL_Handle_HVAC_TUInterface_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseAsduStartLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

#endif              /* ( g_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif        /*_ZCL_HVAC_THERMOSTAT_USER_INTERFACE_CONFIG_CLUSTER_H_*/
