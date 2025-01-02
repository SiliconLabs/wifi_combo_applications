/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/
#include "ZigBeeDataTypes.h"
#include "ZCL_Functionality.h"
#include "common_includes.h"
#include "ZCL_Foundation.h"
#include "memory_utility.h"
#if ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 )
#include "ZCL_HVAC_Thermostat_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_PAYLOAD_LOCATION_c                            0x03

/************* Thermostat Cluster   Attributes *********************************/
#define  m_EVENT_DATA_FOR_SETPOINT_CMD_c                    0x02
#define  IS_NEGATIVE( value)                                value < 0x00
#define  m_LOCAL_TEMP_CALIBRATION_LOWER_c                   0xE7
#define  m_LOCAL_TEMP_CALIBRATION_UPPER_c                   0x19
#define  m_SET_POINT_LOWER_c                                0x02BC
#define  m_SET_POINT_UPPER_c                                0x0BB8
#define  m_SET_POINT_LIMIT_LOWER_c                          0x954D
#define  m_SET_POINT_LIMIT_UPPER_c                          0x7FFF
#define  m_MIN_SET_POINT_DEAD_BAND_LOWER_c                  0x0A
#define  m_MIN_SET_POINT_DEAD_BAND_UPPER_c                  0x19
#define  m_REMOTE_SENSING_LOWER_c                           0x00
#define  m_REMOTE_SENSING_UPPER_c                           0x07
#define  m_CTRL_SEQ_OPERATION_LOWER_c                       0x00
#define  m_CTRL_SEQ_OPERATION_UPPER_c                       0x05
#define  m_SYSTEM_MODE_LOWER_c                              0x00
#define  m_SYSTEM_MODE_UPPER_c                              0x02
#define  m_MIN_HEAT_SET_POINT                               0x07
#define  m_MAX_HEAT_SET_POINT                               0x1E
#define  m_MIN_COOL_SET_POINT                               0x07
#define  m_MAX_COOL_SET_POINT                               0x1E
#define  m_EVENT_LENGTH_THREE_BYTES_c                       0x03
#define m_PI_COOLING_DEMAND_LOWER_c							0x00
#define m_PI_COOLING_DEMAND_UPPER_c							0x64
#define m_PI_HEATING_DEMAND_LOWER_c							0x00
#define m_PI_HEATING_DEMAND_UPPER_c							0x64

#if(g_HA_1_2_d == 1)
/* define range for attributes*/

#define  m_START_OF_WEEK_LOWER_c                              0x00
#define  m_START_OF_WEEK_UPPER_c                              0x06
#define  m_TEMPERATURE_SET_POINT_HOLD_LOWER_c                 0x00
#define  m_TEMPERATURE_SET_POINT_HOLD_UPPER_c                 0x01
#define m_AC_TYPE_LOWER_c									  0x00
#define m_AC_TYPE_UPPER_c									  0x04
#define m_AC_CAPACITY_LOWER_c								  0x00
#define m_AC_CAPACITY_UPPER_c								  0x04
#define m_AC_REFRIGERANT_LOWER_c							  0x00
#define m_AC_REFRIGERANT_UPPER_c							  0x03
#define m_AC_COMPRESSOR_LOWER_c								  0x00
#define m_AC_COMPRESSOR_UPPER_c								  0x03
#define m_AC_LOUVER_POSITION_LOWER_c						  0x00
#define m_AC_LOUVER_POSITION_UPPER_c						  0x05
#define m_AC_COIL_TEMPERATURE_LOWER_c						  0x954d
#define m_AC_COIL_TEMEPRATURE_UPPER_c						  0x7fff
#define m_TEMPERATURE_SET_POINT_HOLD_DURATION_LOWER_c         0x0000
#define m_TEMPERATURE_SET_POINT_HOLD_DURATION_UPPER_c         0x05A0
#define m_THERMOSTAT_RUNNING_MODE_LOWER_c                     0x00
#define m_THERMOSTAT_RUNNING_MODE_UPPER_c					  0x02
#endif  /*(g_HA_1_2_d == 1)*/

/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/

extern ZCL_Cluster_Info_t *mp_cluster_info;
extern uint8_t * mp_attribute_data;

/* Following are attribute info of Thermostat */
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Thermostat_Server_Cluster_Attribute_info[] =
{
    {	g_LOCAL_TEMPERATURE_ATTRIBUTE_c,
        g_Signed_Sixteen_Bit_Integer_Data_c,
        g_Read_Only_Attribute_c | g_Reportable_Attribute_c },

#if ( g_OUTDOOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )
    {  g_OUTDOOR_TEMPERATURE_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c },
#endif                                   /*( g_OUTDOOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_OCCUPANCY_ATTRIBUTE_ENABLE_d == 1 )
    {  g_OCCUPANCY_ATTRIBUTE_c,
       g_Eight_Bit_Bitmap_Data_c,
       g_Read_Only_Attribute_c },
#endif                                   /*( g_OCCUPANCY_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c },
#endif                                  /*( g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c },
#endif                                  /*( g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c },
#endif                                  /*( g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c },
#endif                                  /*( g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_PI_COOLING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
    {  g_PI_COOLING_DEMAND_ATTRIBUTE_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c },
#endif                                  /*( g_PI_COOLING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_PI_HEATING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
    {  g_PI_HEATING_DEMAND_ATTRIBUTE_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c | g_Reportable_Attribute_c },
#endif                                  /*( g_PI_HEATING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )*/

#if(g_HA_1_2_d == 1)
/* ha 1.2 attribute info of Thermostat */
#if ( g_HVAC_SYSTEM_TYPE_CONFIGURATION_ATTRIBUTE_ENABLE_d == 1 )
    {  g_HVAC_SYSTEM_TYPE_CONFIGURATION_ATTRIBUTE_c,
       g_Eight_Bit_Bitmap_Data_c,
       g_Read_Write_Attribute_c},
#endif                                  /*( g_HVAC_SYSTEM_TYPE_CONFIGURATION_ATTRIBUTE_ENABLE_d == 1 )*/
#endif

#if ( g_LOCAL_TEMPERATURE_CALIBRATION_ATTRIBUTE_ENABLE_d == 1 )
    {  g_LOCAL_TEMPERATURE_CALIBRATION_ATTRIBUTE_c,
       g_Signed_Eight_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },
#endif                                  /*( g_LOCAL_TEMPERATURE_CALIBRATION_ATTRIBUTE_ENABLE_d == 1 )*/

    {  g_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },

    {  g_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },

#if ( g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },
#endif                                   /*( g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },
#endif                                   /*( g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },
#endif                                   /*( g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },
#endif                                   /*( g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },
#endif                                   /*( g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },
#endif                                   /*( g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_ENABLE_d == 1 )
    {  g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_c,
       g_Signed_Eight_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c },
#endif                                   /*( g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_REMOTE_SENSING_ATTRIBUTE_ENABLE_d == 1 )
    {  g_REMOTE_SENSING_ATTRIBUTE_c,
       g_Eight_Bit_Bitmap_Data_c,
       g_Read_Write_Attribute_c },
#endif                                   /*( g_REMOTE_SENSING_ATTRIBUTE_ENABLE_d == 1 )*/

    {  g_CONTROL_SEQUENCE_OF_OPERATION_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c },

    {  g_SYSTEM_MODE_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c },

#if ( g_THERMOSTAT_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
    {  g_THERMOSTAT_ALARM_MASK_ATTRIBUTE_c,
       g_Eight_Bit_Bitmap_Data_c,
       g_Read_Only_Attribute_c }
#endif                                   /*( g_THERMOSTAT_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )*/
#if(g_HA_1_2_d == 1)	
#if ( g_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_ENABLE_d == 1 )
    ,{  g_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Only_Attribute_c }
#endif                                   /*( g_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_ENABLE_d == 1 )*/	

#if ( g_START_OF_WEEK_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_START_OF_WEEK_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Only_Attribute_c }
#endif
#if ( g_NUMBER_OF_WEEKLY_TRANSITIONS_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_NUMBER_OF_WEEKLY_TRANSITIONS_ATTRIBUTE_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c }
#endif
#if ( g_NUMBER_OF_DAILY_TRANSITIONS_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_NUMBER_OF_DAILY_TRANSITIONS_ATTRIBUTE_c,
       g_Unsigned_Eight_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c }
#endif
#if ( g_TEMPERATURE_SETPOINT_HOLD_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_TEMPERATURE_SETPOINT_HOLD_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif	
#if ( g_TEMPERATURE_SETPOINT_HOLD_DURATION_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_TEMPERATURE_SETPOINT_HOLD_DURATION_ATTRIBUTE_c,
       g_Unsigned_Sixteen_Bit_Integer_Data_c,
       g_Read_Write_Attribute_c }
#endif	
#if ( g_THERMOSTAT_PROGRAMMING_OPERATIONS_MODE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_THERMOSTAT_PROGRAMMING_OPERATIONS_MODE_ATTRIBUTE_c,
       g_Eight_Bit_Bitmap_Data_c,
       g_Read_Write_Attribute_c | g_Reportable_Attribute_c }
#endif
#if ( g_THERMOSTAT_RUNNING_STATE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_THERMOSTAT_RUNNING_STATE_ATTRIBUTE_c,
       g_Sixteen_Bit_Bitmap_Data_c,
       g_Read_Only_Attribute_c }
#endif	
/* Thermostat Setpoint Change Tracking Attribute Set */
	
#if ( g_SETPOINT_CHANGE_SOURCE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_SETPOINT_CHANGE_SOURCE_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Only_Attribute_c }
#endif	
#if ( g_SETPOINT_CHANGE_AMOUNT_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_SETPOINT_CHANGE_AMOUNT_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c }
#endif	
#if ( g_SETPOINT_CHANGE_SOURCE_TIMESTAMP_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_SETPOINT_CHANGE_SOURCE_TIMESTAMP_ATTRIBUTE_c,
       g_UTC_Time_Data_c,
       g_Read_Only_Attribute_c }
#endif	
/* AC Information Attribute Set*/

#if ( g_AC_TYPE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_TYPE_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif	
#if ( g_AC_CAPACITY_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_CAPACITY_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif	
#if ( g_AC_REFRIGERANT_TYPE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_REFRIGERANT_TYPE_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif	
#if ( g_AC_COMPRESSOR_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_COMPRESSOR_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif	
#if ( g_AC_ERROR_CODE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_ERROR_CODE_ATTRIBUTE_c,
       g_Thirty_Two_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif	
#if ( g_AC_LOUVER_POSITION_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_LOUVER_POSITION_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
       g_Read_Write_Attribute_c }
#endif
#if ( g_AC_COIL_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_COIL_TEMPERATURE_ATTRIBUTE_c,
       g_Signed_Sixteen_Bit_Integer_Data_c,
       g_Read_Only_Attribute_c }
#endif		
#endif	
};


/* Thermostat cluster info */
const ZCL_Cluster_Info_t ga_HVAC_Thermostat_Server_Cluster_Info =
{
   g_THERMOSTAT_CLUSTER_c,
   sizeof( ma_Thermostat_Server_Cluster_Attribute_info)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Thermostat_Server_Cluster_Attribute_info
};
#endif          /* ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 ) */


#if ( g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_HVAC_Thermostat_Client_Cluster_Info =
{
   g_THERMOSTAT_CLUSTER_c,
   g_NULL_c,
   g_NULL_c,
};
#endif


#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
const Thermostat_Boundary_Values_t m_Thermostat_Boundary_Value[] =
{
    {	g_LOCAL_TEMPERATURE_ATTRIBUTE_c,
        m_SET_POINT_LIMIT_LOWER_c,
        m_SET_POINT_LIMIT_UPPER_c },

#if ( g_OUTDOOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )
    {  g_OUTDOOR_TEMPERATURE_ATTRIBUTE_c,
       m_SET_POINT_LIMIT_LOWER_c,
       m_SET_POINT_LIMIT_UPPER_c },
#endif                                   /*( g_OUTDOOR_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
       m_SET_POINT_LIMIT_LOWER_c,
       m_SET_POINT_LIMIT_UPPER_c },
#endif                                  /*( g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
       m_SET_POINT_LIMIT_LOWER_c,
       m_SET_POINT_LIMIT_UPPER_c },
#endif                                  /*( g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_c,
       m_SET_POINT_LIMIT_LOWER_c,
       m_SET_POINT_LIMIT_UPPER_c },
#endif                                  /*( g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_c,
       m_SET_POINT_LIMIT_LOWER_c,
       m_SET_POINT_LIMIT_UPPER_c },
#endif                                  /*( g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_PI_COOLING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
    {  g_PI_COOLING_DEMAND_ATTRIBUTE_c,
       m_PI_COOLING_DEMAND_LOWER_c,
       m_PI_COOLING_DEMAND_UPPER_c },
#endif                                  /*( g_PI_COOLING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_PI_HEATING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
    {  g_PI_HEATING_DEMAND_ATTRIBUTE_c,
       m_PI_HEATING_DEMAND_LOWER_c,
       m_PI_HEATING_DEMAND_UPPER_c },
#endif                                  /*( g_PI_HEATING_DEMAND_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_LOCAL_TEMPERATURE_CALIBRATION_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_LOCAL_TEMPERATURE_CALIBRATION_ATTRIBUTE_c,
      m_LOCAL_TEMP_CALIBRATION_LOWER_c,
      m_LOCAL_TEMP_CALIBRATION_UPPER_c
   } ,
#endif

    {  g_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c,
       m_SET_POINT_LOWER_c,
       m_SET_POINT_LIMIT_UPPER_c },

    {  g_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c,
       g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
       m_SET_POINT_LIMIT_UPPER_c },

#if ( g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c,
       m_SET_POINT_LOWER_c,
       m_SET_POINT_LIMIT_UPPER_c },
#endif                                   /*( g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
    {  g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c,
       m_SET_POINT_LOWER_c,
       m_SET_POINT_LIMIT_UPPER_c },
#endif                                   /*( g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )*/

#if ( g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
      m_SET_POINT_LIMIT_LOWER_c,
      m_SET_POINT_LIMIT_UPPER_c
   },
#endif
#if ( g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c,
      m_SET_POINT_LIMIT_LOWER_c,
      m_SET_POINT_LIMIT_UPPER_c
   },
#endif
#if ( g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_c,
        m_SET_POINT_LIMIT_LOWER_c,
        m_SET_POINT_LIMIT_UPPER_c
    },
#endif
#if ( g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
    {
        g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_c,
        m_SET_POINT_LIMIT_LOWER_c,
        m_SET_POINT_LIMIT_UPPER_c
    },
#endif
#if ( g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_c,
      m_MIN_SET_POINT_DEAD_BAND_LOWER_c,
      m_MIN_SET_POINT_DEAD_BAND_UPPER_c
   },
#endif
#if ( g_REMOTE_SENSING_ATTRIBUTE_ENABLE_d == 1 )
   {
      g_REMOTE_SENSING_ATTRIBUTE_c,
      m_REMOTE_SENSING_LOWER_c,
      m_REMOTE_SENSING_UPPER_c
   },
#endif
   {
      g_CONTROL_SEQUENCE_OF_OPERATION_ATTRIBUTE_c,
      m_CTRL_SEQ_OPERATION_LOWER_c,
      m_CTRL_SEQ_OPERATION_UPPER_c
   },
   {
      g_SYSTEM_MODE_ATTRIBUTE_c,
      m_SYSTEM_MODE_LOWER_c,
      m_SYSTEM_MODE_UPPER_c
   }
#if(g_HA_1_2_d == 1)	
#if ( g_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_ENABLE_d == 1 )
    ,{  g_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_c,
       m_THERMOSTAT_RUNNING_MODE_LOWER_c,
       m_THERMOSTAT_RUNNING_MODE_UPPER_c }
#endif                                   /*( g_THERMOSTAT_RUNNING_MODE_ATTRIBUTE_ENABLE_d == 1 )*/	
#if ( g_START_OF_WEEK_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_START_OF_WEEK_ATTRIBUTE_c,
       m_START_OF_WEEK_LOWER_c,
       m_START_OF_WEEK_UPPER_c }
#endif
#if ( g_TEMPERATURE_SETPOINT_HOLD_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_TEMPERATURE_SETPOINT_HOLD_ATTRIBUTE_c,
       m_TEMPERATURE_SET_POINT_HOLD_LOWER_c,
       m_TEMPERATURE_SET_POINT_HOLD_UPPER_c }
#endif	
#if ( g_TEMPERATURE_SETPOINT_HOLD_DURATION_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_TEMPERATURE_SETPOINT_HOLD_DURATION_ATTRIBUTE_c,
       m_TEMPERATURE_SET_POINT_HOLD_DURATION_LOWER_c,
       m_TEMPERATURE_SET_POINT_HOLD_DURATION_UPPER_c }	
	
#endif
#if ( g_AC_TYPE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_TYPE_ATTRIBUTE_c,
       m_AC_TYPE_LOWER_c,
       m_AC_TYPE_UPPER_c }
#endif	
#if ( g_AC_CAPACITY_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_CAPACITY_ATTRIBUTE_c,
       m_AC_CAPACITY_LOWER_c,
       m_AC_CAPACITY_UPPER_c }
#endif	
#if ( g_AC_REFRIGERANT_TYPE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_REFRIGERANT_TYPE_ATTRIBUTE_c,
       m_AC_REFRIGERANT_LOWER_c,
       m_AC_REFRIGERANT_UPPER_c }
#endif	
#if ( g_AC_COMPRESSOR_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_COMPRESSOR_ATTRIBUTE_c,
       m_AC_COMPRESSOR_LOWER_c,
       m_AC_COMPRESSOR_UPPER_c }
#endif	

#if ( g_AC_LOUVER_POSITION_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_LOUVER_POSITION_ATTRIBUTE_c,
       m_AC_LOUVER_POSITION_LOWER_c,
       m_AC_LOUVER_POSITION_UPPER_c }
#endif
#if ( g_AC_COIL_TEMPERATURE_ATTRIBUTE_ENABLE_d == 1 )
	   ,{  g_AC_COIL_TEMPERATURE_ATTRIBUTE_c,
       m_AC_COIL_TEMPERATURE_LOWER_c,
       m_AC_COIL_TEMEPRATURE_UPPER_c }
#endif	
#endif	
};

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

static uint8_t ZCL_Set_SetPoints_Values ( uint8_t mode,
                           int8_t amount,
                           HVAC_Thermostat_Cluster_Attributes_t *p_attrib_data );

static uint8_t Validate_Set_Points_Value ( uint8_t mode,
                           int16_t set_point,
                           HVAC_Thermostat_Cluster_Attributes_t  *p_attrib_data );

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Check_Boundary_Values ( uint8_t *p_attribute_id,
                                    uint8_t *p_attribute_data,
                                    uint8_t data_length )
{
    int8_t attribute_data_char = 0x00;
    int8_t stored_lower_data = 0x00;
    int8_t stored_upper_data = 0x00;
    uint8_t count;
    uint16_t attribute_id;
    int16_t attribute_data = 0x0000;
    uint16_t max_set_point_attr;
    uint16_t min_set_point_attr;
    int16_t max_set_point_attrData;
    int16_t min_set_point_attrData;
    uint16_t dataOffset;

    memUtils_memCopy ( (uint8_t *)&attribute_id, p_attribute_id, sizeof(uint16_t) );

    if( data_length > 1 ) {
        memUtils_memCopy ( (uint8_t *)&attribute_data, (uint8_t *)p_attribute_data, data_length );
    }
    else{
        attribute_data_char = *p_attribute_data  ;
    }
    for ( count = 0x00; count <  sizeof( m_Thermostat_Boundary_Value )/ sizeof(Thermostat_Boundary_Values_t ); count++ ) {
        if ( attribute_id == m_Thermostat_Boundary_Value[count].attribute_id ) {

            stored_lower_data = m_Thermostat_Boundary_Value[count].lower_boundary_value;
            stored_upper_data = m_Thermostat_Boundary_Value[count].upper_boundary_value;

            if ( ( ( data_length > 1 ) &&
               ( ( attribute_data <  m_Thermostat_Boundary_Value[count].lower_boundary_value ) ||
               ( attribute_data >  m_Thermostat_Boundary_Value[count].upper_boundary_value ) ) ) ||
               ( ( data_length == 1 ) && ( ( attribute_data_char <  stored_lower_data ) ||
               ( attribute_data_char >  stored_upper_data ) ) ) ) {

                return g_ZCL_Invalid_Value_c;
            }
        }
    }
    if ( ( attribute_id == g_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c )
#if ( g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
        || ( attribute_id == g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c )
#endif
     ){
#if ( g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
        max_set_point_attr = g_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c;
#else
        max_set_point_attr = g_ABS_MAX_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c;
#endif

#if ( g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
        min_set_point_attr = g_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c;
#else
        min_set_point_attr = g_ABS_MIN_HEAT_SET_POINT_LIMIT_ATTRIBUTE_c;
#endif
    }
    else if ( ( attribute_id == g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c )
#if ( g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c == 1 )
      || ( attribute_id == g_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c )
#endif
    ){
#if ( g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
        max_set_point_attr = g_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_c;
#else
        max_set_point_attr = g_ABS_MAX_COOL_SET_POINT_LIMIT_ATTRIBUTE_c;
#endif
#if ( g_MIN_SET_POINT_DEAD_BAND_ATTRIBUTE_ENABLE_d == 1 )
        min_set_point_attr = g_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_c;
#else
        min_set_point_attr = g_ABS_MIN_COOL_SET_POINT_LIMIT_ATTRIBUTE_c;
#endif
    }
    else{
        return g_ZCL_Success_c;
    }
    dataOffset = ZCL_Get_AttributeDataOffset( mp_cluster_info->num_attrib_info_present,
                           max_set_point_attr,
                           mp_cluster_info->p_array_of_attrib_info );

    memUtils_memCopy ( (uint8_t*)&max_set_point_attrData, mp_attribute_data + dataOffset, 0x02 );

    dataOffset = ZCL_Get_AttributeDataOffset( mp_cluster_info->num_attrib_info_present,
										   min_set_point_attr,
										   mp_cluster_info->p_array_of_attrib_info );

    memUtils_memCopy ( (uint8_t*)&min_set_point_attrData, mp_attribute_data + dataOffset, 0x02 );

    if ( ( attribute_data > max_set_point_attrData )&&( attribute_data < min_set_point_attrData) ){
        return g_ZCL_Invalid_Value_c;
    }

    return g_ZCL_Success_c;
}
#endif          /* ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 ) */

/************************************************************************************************/
uint8_t ZCL_Handle_Thermostat_Cluster_Cmd ( uint8_t endPointId,
                                uint8_t asduLength,
                                uint8_t* pReceivedAsdu,
                                uint8_t* pResponseAsduStartLocation,
                                ZCL_ActionHandler_t *pActionHandler,
                                ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                uint8_t * pAttrsDataBaseLocation )
{

    uint8_t status = g_ZCL_Success_c;
    uint8_t responseAsduLength = g_ZERO_c;
	
	pActionHandler->event.eventLength = asduLength - m_EVENT_LENGTH_THREE_BYTES_c;	
    pActionHandler->action = Send_Response;
	
	mp_cluster_info = (ZCL_Cluster_Info_t*)pZCLClusterInfo;
    mp_attribute_data = pAttrsDataBaseLocation;	

    if ( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_CLIENT_TO_SERVER_DIRECTION_c ) {
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )
    	uint8_t offset = m_PAYLOAD_LOCATION_c;
        if ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c )  == g_Set_Point_Raise_Lower_Command_c ){
	
		  Setpoint_Raise_Lower_Command_t   *pFormattedSetPointRaiseLowerEvent =  (Setpoint_Raise_Lower_Command_t*)pActionHandler->event.pEventData;
		
		   pFormattedSetPointRaiseLowerEvent->mode = *( pReceivedAsdu + offset );
			   offset += 0x01;
			   pFormattedSetPointRaiseLowerEvent->amount = *( pReceivedAsdu + offset );
			   offset += 0x01;

            if ( ( g_Set_Point_Heat_c ==  pFormattedSetPointRaiseLowerEvent->mode) ||
                ( g_Set_Point_Both_Heat_Cool_c == pFormattedSetPointRaiseLowerEvent->mode  ) ) {					
				
                status = ZCL_Set_SetPoints_Values ( g_Set_Point_Heat_c,
                                    pFormattedSetPointRaiseLowerEvent->amount ,
                                    (HVAC_Thermostat_Cluster_Attributes_t *)pAttrsDataBaseLocation );

                pActionHandler->event.eventId = g_Heat_Setpoint_Raised_Event_c;

                if ( IS_NEGATIVE( pFormattedSetPointRaiseLowerEvent->amount  ) ){
                    pActionHandler->event.eventId = g_Heat_Setpoint_Lowered_Event_c;
                }
          }
          if ( ( g_Set_Point_Cool_c ==  pFormattedSetPointRaiseLowerEvent->mode ) ||
               ( g_Set_Point_Both_Heat_Cool_c == pFormattedSetPointRaiseLowerEvent->mode  ) ) {
				
            		
             status = ZCL_Set_SetPoints_Values ( g_Set_Point_Cool_c,
                                    pFormattedSetPointRaiseLowerEvent->amount ,
                                    (HVAC_Thermostat_Cluster_Attributes_t *)pAttrsDataBaseLocation );

             pActionHandler->event.eventId = g_Cool_Setpoint_Raised_Event_c;

             if( IS_NEGATIVE( pFormattedSetPointRaiseLowerEvent->amount ) ) {

                pActionHandler->event.eventId = g_Cool_Setpoint_Lowered_Event_c;

             }
          }
       }
#if(g_HA_1_2_d == 1)	
	   else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Set_Weekly_Schedule_Command_c){
		  /* This event is handled by the application.*/ 		

		       pActionHandler->event.eventId = g_Set_Weekly_Schedule_Event_c;
			   Set_Weekly_Schedule_t *pFormattedWeeklySetEvent = (Set_Weekly_Schedule_t*)pActionHandler->event.pEventData;;
			
			   pFormattedWeeklySetEvent->number_of_transitions_for_sequence = *( pReceivedAsdu + offset );
			   offset += 0x01;
			   pFormattedWeeklySetEvent->day_of_week_for_sequence = *( pReceivedAsdu + offset );
			   offset += 0x01;
			   pFormattedWeeklySetEvent->mode_for_sequence = *( pReceivedAsdu + offset );
			   offset += 0x01;			
			
			   for (uint8_t i=0; i< pFormattedWeeklySetEvent->number_of_transitions_for_sequence; i++){
				
				 memUtils_memCopy( (uint8_t*)&pFormattedWeeklySetEvent->transition_sequence[i].transition_time,
												pReceivedAsdu + offset, 0x02 );
				 offset += 0x02;
				
				 if(pFormattedWeeklySetEvent->mode_for_sequence == 0x01){
				
					memUtils_memCopy( (uint8_t*)&pFormattedWeeklySetEvent->transition_sequence[i].heat_set_point,
												pReceivedAsdu + offset, 0x02 );
					offset += 0x02;
				 }
				 else if(pFormattedWeeklySetEvent->mode_for_sequence == 0x02){
				
					memUtils_memCopy( (uint8_t*)&pFormattedWeeklySetEvent->transition_sequence[i].cool_set_point,
												pReceivedAsdu + offset, 0x02 );
					offset += 0x02;
				 }
				 else if(pFormattedWeeklySetEvent->mode_for_sequence == 0x03){
				
				   memUtils_memCopy( (uint8_t*)&pFormattedWeeklySetEvent->transition_sequence[i].heat_set_point,
												pReceivedAsdu + offset, 0x02 );
					offset += 0x02;
					
					memUtils_memCopy( (uint8_t*)&pFormattedWeeklySetEvent->transition_sequence[i].cool_set_point,
												pReceivedAsdu + offset, 0x02 );
					offset += 0x02;
				 }
			   }
			
		}
		else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Get_Weekly_Schedule_Command_c){
		  /* This event is handled by the application.*/
				pActionHandler->event.eventId = g_Get_Weekly_Schedule_Event_c;
				
			   Get_weekly_Schedule_t *pFormattedGetWeeklyEvent = (Get_weekly_Schedule_t*)pActionHandler->event.pEventData;;
			
			   pFormattedGetWeeklyEvent->days_to_return = *( pReceivedAsdu + offset );
			   offset += 0x01;
			   pFormattedGetWeeklyEvent->mode_to_return = *( pReceivedAsdu + offset );
			   offset += 0x01;
		
		}
		else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Clear_Weekly_Schedule_Command_c){
		  /* This event is handled by the application.*/
				pActionHandler->event.eventId = g_Clear_Weekly_Schedule_Event_c;
				memUtils_memCopy( pActionHandler->event.pEventData,
							pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							pActionHandler->event.eventLength  );
		
		}
		else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Get_Relay_Status_Log_Command_c){
		  /* This event is handled by the application.*/
				pActionHandler->event.eventId = g_Get_Relay_Status_Log_Event_c;
				memUtils_memCopy( pActionHandler->event.pEventData,
							pReceivedAsdu + GetZCLHeaderLength( pReceivedAsdu ),
							pActionHandler->event.eventLength  );
						
		}
#endif		
#endif              /* ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 ) */
    }
#if ( g_THERMOSTAT_CLIENT_CLUSTER_ENABLE_d == 1 )
#if(g_HA_1_2_d == 1)	
	else if ( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) {

		uint8_t offset = m_PAYLOAD_LOCATION_c;
		if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Current_Weekly_Schedule_Command_c){
		/* This event is handled by the application.*/
		//				pActionHandler->event.eventId = g_Current_Weekly_Schedule_Event_c;
		
		pActionHandler->event.eventId = g_Current_Weekly_Schedule_Event_c;
		Current_Weekly_Schedule_t *pFormattedCurrentWeeklyScheduleEvent = (Current_Weekly_Schedule_t*)pActionHandler->event.pEventData;;
		
		pFormattedCurrentWeeklyScheduleEvent->number_of_transitions_for_sequence = *( pReceivedAsdu + offset );
		offset += 0x01;
		pFormattedCurrentWeeklyScheduleEvent->day_of_week_for_sequence = *( pReceivedAsdu + offset );
		offset += 0x01;
		pFormattedCurrentWeeklyScheduleEvent->mode_for_sequence = *( pReceivedAsdu + offset );
		offset += 0x01;			
	
		for (i=0; i< pFormattedCurrentWeeklyScheduleEvent->number_of_transitions_for_sequence; i++){
		
			memUtils_memCopy( (uint8_t*)&pFormattedCurrentWeeklyScheduleEvent->transition_sequence[i].transition_time,
										pReceivedAsdu + offset, 0x02 );
			offset += 0x02;
			
			if(pFormattedCurrentWeeklyScheduleEvent->mode_for_sequence == 0x01){
			
				memUtils_memCopy( (uint8_t*)&pFormattedCurrentWeeklyScheduleEvent->transition_sequence[i].heat_set_point,
											pReceivedAsdu + offset, 0x02 );
				offset += 0x02;
			}
			else if(pFormattedCurrentWeeklyScheduleEvent->mode_for_sequence == 0x02){
			
				memUtils_memCopy( (uint8_t*)&pFormattedCurrentWeeklyScheduleEvent->transition_sequence[i].cool_set_point,
											pReceivedAsdu + offset, 0x02 );
				offset += 0x02;
			}
			else if(pFormattedCurrentWeeklyScheduleEvent->mode_for_sequence == 0x03){
			
				memUtils_memCopy( (uint8_t*)&pFormattedCurrentWeeklyScheduleEvent->transition_sequence[i].heat_set_point,
											pReceivedAsdu + offset, 0x02 );
				offset += 0x02;
				
				memUtils_memCopy( (uint8_t*)&pFormattedCurrentWeeklyScheduleEvent->transition_sequence[i].cool_set_point,
											pReceivedAsdu + offset, 0x02 );
				offset += 0x02;
			}
		}
	
	}
	else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_Relay_Status_Log_Command_c){
	  /* This event is handled by the application.*/
			pActionHandler->event.eventId = g_Relay_Status_Log_Event_c;
			
			relay_status_log_t  *pFormattedRelayStatusLogevent= (relay_status_log_t*)pActionHandler->event.pEventData;
			
			memUtils_memCopy( (uint8_t*)&pFormattedRelayStatusLogevent->time_of_day,
											pReceivedAsdu + offset, 0x02 );
			offset += 0x02;
				
			memUtils_memCopy( (uint8_t*)&pFormattedRelayStatusLogevent->relay_status,
											pReceivedAsdu + offset, 0x02 );
			offset += 0x02;
			memUtils_memCopy( (uint8_t*)&pFormattedRelayStatusLogevent->local_temperature,
											pReceivedAsdu + offset, 0x02 );
			offset += 0x02;
			
			pFormattedRelayStatusLogevent->humidity_in_percenatge = *( pReceivedAsdu + offset );
			offset += 0x01;
			
			memUtils_memCopy( (uint8_t*)&pFormattedRelayStatusLogevent->set_point,
										pReceivedAsdu + offset, 0x02 );
			offset += 0x02;
			memUtils_memCopy( (uint8_t*)&pFormattedRelayStatusLogevent->unread_entries,
										pReceivedAsdu + offset, 0x02 );
			offset += 0x02;
	
		}
	}
#endif	
#endif	
	
	if ( Is_Default_Response_Required ( pReceivedAsdu ) ) {
		responseAsduLength =	ZCL_CreateDefaultResponseCommand (
								  pReceivedAsdu,
								  pResponseAsduStartLocation,
								  status  );
		 pActionHandler->action = Send_Response;
		
	}
	 else{
		  pActionHandler->action = No_Response;
	 }
    return responseAsduLength;
}

/************************************************************************************************/
#if ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 )

static uint8_t ZCL_Set_SetPoints_Values (uint8_t mode, int8_t amount,
  							 HVAC_Thermostat_Cluster_Attributes_t *p_attrib_data )
{
   int16_t set_point =0x0000;
   uint8_t *p_set_point = g_NULL_c;
   uint8_t status = g_ZCL_Invalid_Value_c;
   uint16_t attributeid;
   if( p_attrib_data->ocupancy )  {
      if( g_Set_Point_Heat_c == mode )
      {
         p_set_point =  p_attrib_data->a_occupied_heating_setpoint;
		 attributeid = g_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c;
      }
      else
      {
         p_set_point =  p_attrib_data->a_occupied_cooling_setpoint;
		 attributeid = g_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c;
      }
   }
   else {
#if ( g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
      if( g_Set_Point_Heat_c == mode )
      {
         p_set_point =  p_attrib_data->a_unoccupied_heating_setpoint;
         attributeid = g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_c;
      }
#endif          /* g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d */
#if ( g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
      else
      {
         p_set_point =  p_attrib_data->a_unoccupied_cooling_setpoint;
		 attributeid = g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_c;
      }
#endif         /* g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d */
   }
   if ( p_set_point != g_NULL_c ){
       memUtils_memCopy ( (uint8_t*)&set_point,  p_set_point, sizeof(uint16_t) );
       set_point += amount;
       status =    Validate_Set_Points_Value ( mode, set_point, p_attrib_data );
       if( g_ZCL_Success_c == status ) {
		   status = ZCL_Read_Write_Attribute_Data( g_Write_Attribute_Data_c,
												  mp_attribute_data,
												  mp_cluster_info,
												  attributeid,
												  (uint8_t*)&set_point );
       }
   }
   return status;
}


/************************************************************************************************/

static uint8_t Validate_Set_Points_Value ( uint8_t mode, int16_t set_point,
   							HVAC_Thermostat_Cluster_Attributes_t  *p_attrib_data )
{
   uint8_t *p_set_point;
   int16_t local_set_point;
   int16_t actual_set_point = set_point;
   if( p_attrib_data->ocupancy )
   {
      if( g_Set_Point_Heat_c == mode ) {
         /* for heating mode check for cooling set point */
         p_set_point =  p_attrib_data->a_occupied_cooling_setpoint;
      }
      else {
         /* for cooling mode check for heating set point */
         p_set_point =  p_attrib_data->a_occupied_heating_setpoint;
      }
   }
   else
   {
#if ( g_UN_OCCUPIED_HEATING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
      if( g_Set_Point_Heat_c == mode ){
         /*for heating mode check for cooling set point*/
         p_set_point =  p_attrib_data->a_unoccupied_cooling_setpoint;
      }
#endif
#if ( g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d == 1 )
      else  {
         p_set_point =  p_attrib_data->a_unoccupied_heating_setpoint;
      }
#endif         /* g_UN_OCCUPIED_COOLING_SET_POINT_ATTRIBUTE_ENABLE_d */
   }
   memUtils_memCopy ( (uint8_t *)&local_set_point, p_set_point, sizeof(uint16_t) );

   if ( ( g_Set_Point_Heat_c == mode ) &&
        ( actual_set_point < local_set_point ) &&
        ( ( local_set_point - actual_set_point ) > p_attrib_data->min_setpoint_dead_band ) ) {

        return g_ZCL_Success_c;
   }
   if ( ( g_Set_Point_Cool_c == mode ) &&
        ( actual_set_point > local_set_point ) &&
        ( ( actual_set_point - local_set_point ) > p_attrib_data->min_setpoint_dead_band ) ){

        return g_ZCL_Success_c;
   }
   return g_ZCL_Invalid_Value_c;
}

#endif          /* ( g_THERMOSTAT_SERVER_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

#endif              /*  ( g_THERMOSTAT_CLUSTER_ENABLE_d == 1 ) */
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
