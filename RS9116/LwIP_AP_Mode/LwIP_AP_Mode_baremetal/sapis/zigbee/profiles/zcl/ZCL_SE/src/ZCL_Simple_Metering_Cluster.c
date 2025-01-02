/*******************************************************************************
* ZCL_General_Simple_Metering_Cluster.c
********************************************************************************
* Program Description:
* This module provides all the functionality for supporting Simple_metering
* cluster
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZCL_Interface.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
#include "ZCL_Simple_Metering_Cluster.h"
#include "memory_utility.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_GET_PROFILE_PAYLOAD_LENGTH_c                        0x06
#define m_PAYLOAD_LOCATION_c                                  0x03

#if ( g_DAILY_FREEZE_TIME_ATTRIBUTE_ENABLE_d == 1 )
	#define m_DAILY_FREEZE_TIME_LIMIT_LOWER_c  0x0000
	#define m_DAILY_FREEZE_TIME_LIMIT_UPPER_c  0x183C
#endif
#if ( g_POWER_FACTOR_ATTRIBUTE_ENABLE_d == 1 )

	#define  m_POWER_FACTOR_LIMIT_LOWER_c  -100
	#define  m_POWER_FACTOR_LIMIT_UPPER_c   100
#endif

#if ( g_SE_PROFILE_1_1_d == 1 )
#if ( g_CURRENT_BLOCK_ATTRIBUTE_ENABLE_d == 1 )
	#define m_CURRENT_BLOCK_LIMIT_LOWER_c  0x00
	#define m_CURRENT_BLOCK_LIMIT_UPPER_c  0x10
#endif

#if ( g_PRESET_READING_TIME_ATTRIBUTE_ENABLED_d == 1 )
	#define m_PRESET_READING_TIME_LIMIT_LOWER_c  0x0000
	#define m_PRESET_READING_TIME_LIMIT_UPPER_c  0x173B
#endif
#endif  /* g_SE_PROFILE_1_1_d*/
#if(g_SE_PROFILE_1_2_d == 1)
#if ( g_ACTIVE_REGISTER_TIER_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )

	#define  m_ACTIVE_REGISTER_TIER_LIMIT_LOWER_c  0x00
	#define m_ACTIVE_REGISTER_TIER_LIMIT_UPPER_c   0x48
#endif
#if ( g_ACTIVE_REGISTER_TIER_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	#define  m_ACTIVE_REGISTER_TIER_LIMIT_LOWER_c  0x00
	#define  m_ACTIVE_REGISTER_TIER_LIMIT_UPPER_c   0x48
#endif
#if ( g_REMAINING_BATTERY_LIFE_IN_DAYS_ATTRIBUTE_ENABLE_d == 1 )
	#define  m_REMAINING_BATTERY_LIFE_IN_DAYS_LIMIT_LOWER_c   0x0000
	#define  m_REMAINING_BATTERY_LIFE_IN_DAYS_LIMIT_UPPER_c   0x1C84
#endif

#if ( g_LOW_MEDIUM_HIGH_STATUS_ATTRIBUTE_ENABLED_d == 1 )
	#define  m_LOW_MEDIUM_HIGH_STATUS_LIMIT_LOWER_c  0x00
	#define  m_LOW_MEDIUM_HIGH_STATUS_LIMIT_UPPER_c  0x03
#endif
#endif /* g_SE_PROFILE_1_2_d*/


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_simple_metering_Server_Attribute [] =
{  /* Reading Information Attribute set */
	{
		g_CURRENT_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
	
#if ( g_CURRENT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_MAX_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MAX_DEMAND_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_MAX_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_MAX_DEMAND_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MAX_DEMAND_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_MAX_DEMAND_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_DFT_SUMMATION_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_DFT_SUMMATION_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_DFT_SUMMATION_ATTRIBUTE_ENABLE_d */

#if ( g_DAILY_FREEZE_TIME_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_DAILY_FREEZE_TIME_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_DAILY_FREEZE_TIME_ATTRIBUTE_ENABLE_d */

#if ( g_POWER_FACTOR_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_POWER_FACTOR_c,
		g_Signed_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_POWER_FACTOR_ATTRIBUTE_ENABLE_d */

#if ( g_READING_SNAP_SHOT_TIME_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_READING_SNAP_SHOT_TIME_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_READING_SNAP_SHOT_TIME_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_MAX_DEMAND_DELIVERED_TIME_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MAX_DEMAND_DELIVERED_TIME_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_MAX_DEMAND_DELIVERED_TIME_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_MAX_DEMAND_RECIEVED_TIME_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MAX_DEMAND_RECIEVED_TIME_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_MAX_DEMAND_RECIEVED_TIME_ATTRIBUTE_ENABLE_d */

#if (g_SE_PROFILE_1_1_d==1)	
#if( g_DEFAULT_UPDATE_PERIOD_ATTRIBUTE_ENABLED_d == 1)
	{
		g_DEFAULT_UPDATE_PERIOD_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif

#if( g_FAST_POLL_UPDATE_PERIOD_ATTRIBUTE_ENABLED_d == 1)
	{
		g_FAST_POLL_UPDATE_PERIOD_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif

#if (g_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLED_d ==1)
	{
		g_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif
#if (g_DAILY_CONSUMPTION_TARGET_ATTRIBUTE_ENABLED_d ==1)
	{
		g_DAILY_CONSUMPTION_TARGET_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif

#if (g_CURRENT_BLOCK_ATTRIBUTE_ENABLED_d ==1)
	{
		g_CURRENT_BLOCK_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif
#if (g_PROFILE_INTERVAL_PERIOD_ATTRIBUTE_ENABLED_d ==1)
	{
		g_PROFILE_INTERVAL_PERIOD_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif

#if (g_INTERVAL_READ_REPORTING_PERIOD_ATTRIBUTE_ENABLED_d ==1)
	{
		g_INTERVAL_READ_REPORTING_PERIOD_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif
#if (g_PRESET_READING_TIME_ATTRIBUTE_ENABLED_d ==1)
	{
		g_PRESET_READING_TIME_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif
#if (g_VOLUME_PER_REPORT_ATTRIBUTE_ENABLED_d ==1)
	{
		g_VOLUME_PER_REPORT_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif
#if (g_FLOW_RESTRICTION_ATTRIBUTE_ENABLED_d ==1)
	{
		g_FLOW_RESTRICTION_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif
#if (g_SUPPLY_STATUS_ATTRIBUTE_ENABLED_d ==1)
	{
		g_SUPPLY_STATUS_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif

#if (g_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_ATTRIBUTE_ENABLED_d ==1)
	{
		g_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif

#if (g_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_ATTRIBUTE_ENABLED_d ==1)
	{
		g_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif

#if (g_INLET_TEMPERATURE_ATTRIBUTE_ENABLED_d ==1)
	{
		g_INLET_TEMPERATURE_SUMMATION_c,
		g_Signed_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif

#if (g_OUTLET_TEMPERATURE_ATTRIBUTE_ENABLED_d ==1)
	{
		g_OUTLET_TEMPERATURE_SUMMATION_c,
		g_Signed_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif

#if (g_CONTROL_TEMPERATURE_ATTRIBUTE_ENABLED_d ==1)
	{
		g_CONTROL_TEMPERATURE_SUMMATION_c,
		g_Signed_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif
#if (g_CURRENT_INLET_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLED_d ==1)
	{
		g_CURRENT_INLET_ENERGY_CARRIER_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	
	},
#endif

#if (g_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLED_d ==1)
	{
		g_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#endif /* g_SE_PROFILE_1_1_d  */

#if(g_SE_PROFILE_1_2_d == 1)
#if (g_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d ==1)
	{
		g_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if (g_CURRENT_BLOCK_PERIOD_CONSUMPTION_RECIEVED_ATTRIBUTE_ENABLE_d ==1)
	{
		g_CURRENT_BLOCK_PERIOD_CONSUMPTION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif          /* g_CURRENT_BLOCK_PERIOD_CONSUMPTION_RECIEVED_ENABLED_d */
#if (g_CURRENT_BLOCK_RECIEVED_ATTRIBUTE_ENABLE_d ==1)
	{
		g_CURRENT_BLOCK_RECIEVED_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	},
#endif          /* g_CURRENT_BLOCK__RECIEVED_ENABLED_d */
#if (g_DFT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d ==1)
	{
		g_DFT_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif          /* g_DFT_SUMMATION__RECIEVED_ENABLED_d */
#if (g_ACTIVE_REGISTER_TIER_DELIVERED_ATTRIBUTE_ENABLE_d ==1)
	{
		g_ACTIVE_REGISTER_TIER_DELIVERED_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	},
#endif          /* g_ACTIVE_REGISTER_TIER_DELIVERED_ENABLED_d */
#if (g_ACTIVE_REGISTER_TIER_RECIEVED_ATTRIBUTE_ENABLE_d ==1)
	{
		g_ACTIVE_REGISTER_TIER_RECIEVED_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	},
#endif          /* g_ACTIVE_REGISTER_TIER_RECIEVED_ENABLED_d */
#endif /* End of g_SE_PROFILE_1_2_d  */

#if ( g_CURRENT_TIER1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER1_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER2_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER3_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER4_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER5_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER6_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_CURRENT_TIER7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER7_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#if ( g_CURRENT_TIER8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER8_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#if ( g_CURRENT_TIER9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER9_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#if ( g_CURRENT_TIER10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER10_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#if ( g_CURRENT_TIER11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER11_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	

#if ( g_CURRENT_TIER12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER12_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#if ( g_CURRENT_TIER13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER13_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#if ( g_CURRENT_TIER14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER14_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#if ( g_CURRENT_TIER15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER15_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#endif	/*  ( g_SE_PROFILE_1_1_d == 1 )*/	

#if ( g_SE_PROFILE_1_2_d == 1 )
#if ( g_CURRENT_TIER16_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER16_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER17_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER17_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER17_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER17_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER18_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER18_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER18_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER18_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER19_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER19_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER19_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER19_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER20_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER20_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER20_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER20_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER21_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER21_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER21_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER21_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER22_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER22_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER22_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER22_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER23_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER23_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER23_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER23_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER24_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER24_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER24_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER24_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER25_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER25_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER25_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER25_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER26_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER26_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER26_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER26_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER27_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER27_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER27_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER27_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER28_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER28_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER28_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER28_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER29_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER29_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER29_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER29_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER30_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER30_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER30_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER30_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER31_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER31_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER31_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER31_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER32_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER32_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER32_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER32_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER33_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER33_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER33_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER33_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER34_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER34_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER34_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER34_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER35_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER35_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER35_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER35_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER36_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER36_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER36_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER36_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER37_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER37_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER37_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER37_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER38_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER38_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER38_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER38_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER39_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER39_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER39_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER39_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER40_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER40_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER40_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER40_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER41_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER41_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER41_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER41_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER42_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER42_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER42_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER42_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER43_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER43_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER43_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER43_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER44_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER44_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER44_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER44_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif

#if ( g_CURRENT_TIER45_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER45_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER45_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER45_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER46_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER46_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER46_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER46_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER47_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER47_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER47_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER47_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_TIER48_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER48_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				
#if ( g_CURRENT_TIER48_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER48_SUMMATION_RECIEVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif
#endif /*  ( g_SE_PROFILE_1_2_d == 1 )*/

	{
		g_CURRENT_STATUS_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_REMAINING_BATTERY_LIFE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_REMAINING_BATTERY_LIFE_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif	
#if ( g_HOURS_IN_OPERATION_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_HOURS_IN_OPERATION_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_HOURS_IN_FAULT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_HOURS_IN_FAULT_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c  | g_Reportable_Attribute_c
	},
#endif
#endif  /* End of g_SE_PROFILE_1_1_d == 1 */
#if(g_SE_PROFILE_1_2_d == 1)
#if ( g_EXTENDED_STATUS_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_EXTENDED_STATUS_c,
		g_Forty_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_REMAINING_BATTERY_LIFE_IN_DAYS_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_REMAINING_BATTERY_LIFE_IN_DAYS_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif

#if ( g_CURRENT_METER_ID_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_METER_ID_c,
		g_Octet_String_Data_c,
		g_Read_Only_Attribute_c
	},
#endif

#if ( g_LOW_MEDIUM_HIGH_STATUS_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_LOW_MEDIUM_HIGH_STATUS_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#endif   /* End of g_SE_PROFILE_1_2_d == 1 */
	{
		g_UNIT_OF_MEASURE_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	},	
#if ( g_MULTIPLIER_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_MULTIPLIER_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif										/* g_MULTIPLIER_ATTRIBUTE_ENABLE_d */

#if ( g_DIVISOR_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_DIVISOR_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif										/* g_DIVISOR_ATTRIBUTE_ENABLE_d */

	{
		g_SUMMATION_FORMATTING_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},

#if ( g_DEMAND_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_DEMAND_FORMATTING_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},
#endif										/* g_DEMAND_FORMATTING_ATTRIBUTE_ENABLE_d */

#if ( g_HISTORICAL_CONSUMPTION_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_HISTORICAL_CONSUMPTION_FORMATTING_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},
#endif										/* g_HISTORICAL_CONSUMPTION_FORMATTING_ATTRIBUTE_ENABLE_d */

	{
		g_METERING_DEVICE_TYPE_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_SITE_ID_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_SITE_ID_c,
		g_Octet_String_Data_c,
		g_Read_Only_Attribute_c
	},
#endif			
#if ( g_METER_SERIAL_NUMBER_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_METER_SERIAL_NUMBER_c,
		g_Octet_String_Data_c,
		g_Read_Only_Attribute_c
	},
#endif	
#if ( g_ENERGY_CARRIER_UNIT_OF_MEASURE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_ENERGY_CARRIER_UNIT_OF_MEASURE_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	},
#endif	
#if ( g_ENERGY_CARRIER_SUMMATION_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_ENERGY_CARRIER_SUMMATION_FORMATTING_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_ENERGY_CARRIER_DEMAND_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_ENERGY_CARRIER_DEMAND_FORMATTING_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_TEMPERATURE_UNIT_OF_MEASURE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_TEMPERATURE_UNIT_OF_MEASURE_c,
		g_Eight_Bit_Enumeration_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_TEMPERATURE_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_TEMPERATURE_FORMATTING_c,
		g_Eight_Bit_Bitmap_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#endif /* ( g_SE_PROFILE_1_1_d == 1 ) */
#if(g_SE_PROFILE_1_2_d == 1)
#if ( g_MODULE_SERIAL_NUMBER_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_MODULE_SERIAL_NUMBER_c,
		g_Octet_String_Data_c,
		g_Read_Only_Attribute_c
	},
#endif /* ( g_SE_PROFILE_1_1_d == 1 ) */
#if ( g_OPERATING_TARIFF_LABEL_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_OPERATING_TARIFF_LABEL_DELIVERED_c,
		g_Octet_String_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_OPERATING_TARIFF_LABEL_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_OPERATING_TARIFF_LABEL_RECEIVED_c,
		g_Octet_String_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#endif /* ( g_SE_PROFILE_1_2_d == 1 ) */
#if ( g_INSTANTANEOUS_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_INSTANTANEOUS_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_INSTANTANEOUS_DEMAND_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_DAY_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_DAY_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_PREVIOUS_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_PREVIOUS_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_PREVIOUS_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_PREVIOUS_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_RECIEVED_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_RECIEVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_RECIEVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_RECIEVED_ATTRIBUTE_ENABLE_d */
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_CURRENT_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_DAY_MAX_PRESSURE_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_DAY_MIN_PRESSURE_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c  | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d */
#if ( g_PREVIOUS_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY_MAX_PRESSURE_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_PREVIOUS_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d */

#if ( g_PREVIOUS_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY_MIN_PRESSURE_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_PREVIOUS_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_DAY_MAX_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_DAY_MAX_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_DAY_MAX_DEMAND_ATTRIBUTE_ENABLE_d */

#if ( g_PREVIOUS_DAY_MAX_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY_MAX_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_DAY_MIN_DEMAND_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_MONTH_MAX_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MONTH_MAX_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_MONTH_MAX_DEMAND_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_YEAR_MAX_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_YEAR_MAX_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c | g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_YEAR_MAX_DEMAND_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
#if ( g_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_c,
		g_Signed_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif				/* g_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
#endif  /* ( g_SE_PROFILE_1_1_d == 1 )*/
#if(g_SE_PROFILE_1_2_d == 1)
#if ( g_PREVIOUS_DAY2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY2_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY2_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY3_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY3_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY4_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY4_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY5_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY5_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY6_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY6_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY6_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY6_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY7_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY7_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY7_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY7_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY8_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY8_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_DAY8_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_DAY8_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_WEEK_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_WEEK_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_WEEK_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_WEEK_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK2_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK2_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK3_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK3_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK4_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK4_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK5_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_WEEK5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_WEEK5_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_MONTH_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MONTH_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_CURRENT_MONTH_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_MONTH_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH2_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH2_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH3_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH3_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH4_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH4_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH5_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH5_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH6_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH6_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH6_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH6_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH7_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH7_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH7_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH7_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH8_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH8_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH8_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH8_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH9_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH9_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH9_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH9_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH10_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH10_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH10_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH10_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH11_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH11_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH11_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH11_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH12_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH12_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH12_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH12_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH13_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH13_CONSUMPTION_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#if ( g_PREVIOUS_MONTH13_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PREVIOUS_MONTH13_CONSUMPTION_RECEIVED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c| g_Reportable_Attribute_c
	},
#endif
#endif  /* ( g_SE_PROFILE_1_2_d == 1 )*/

/* Load Profile Configuration Attribute Set */
#if ( g_MAX_NUMBER_OF_PERIODS_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_MAX_NUMBER_OF_PERIODS_DELIVERED_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_MAX_NUMBER_OF_PERIODS_DELIVERED_ATTRIBUTE_ENABLE_d */

/* Supply Limit Attributes Set */
#if ( g_CURRENT_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_DEMAND_DELIVERED_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_DEMAND_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_DEMAND_LIMIT_c,
		g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_DEMAND_LIMIT_ATTRIBUTE_ENABLE_d */		

#if ( g_DEMAND_INTEGRATION_PERIOD_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_DEMAND_INTEGRATION_PERIOD_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_DEMAND_INTEGRATION_PERIOD_ATTRIBUTE_ENABLE_d */	

#if ( g_NUMBER_OF_DEMAND_SUBINTERVALS_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_NUMBER_OF_DEMAND_SUBINTERVALS_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_NUMBER_OF_DEMAND_SUBINTERVALS_ATTRIBUTE_ENABLE_d */	
#if(g_SE_PROFILE_1_2_d == 1)
#if ( g_DEMAND_LIMIT_ARM_DURATION_IN_MINUTES_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_DEMAND_LIMIT_ARM_DURATION_IN_MINUTES_c,
		g_Unsigned_Sixteen_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_LOAD_LIMIT_SUPPLY_STATE_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_LOAD_LIMIT_SUPPLY_STATE_c,
		g_Boolean_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_LOAD_LIMIT_COUNTER_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_LOAD_LIMIT_COUNTER_c,
		g_Unsigned_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */

/* Block Information Attribute Set (Delivered) */
#if(g_SE_PROFILE_1_1_d == 1)			
#if ( g_CURRENT_NO_TIER_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_NO_TIER_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_NO_TIER_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_NO_TIER_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_NO_TIER_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		


#if ( g_CURRENT_NO_TIER_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_NO_TIER_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_NO_TIER_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			

#if ( g_CURRENT_TIER1_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER1_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER1_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER1_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

	
#if ( g_CURRENT_TIER1_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			

	
#if ( g_CURRENT_TIER1_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			

#if ( g_CURRENT_TIER1_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER1_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	

#if ( g_CURRENT_TIER1_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER1_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER1_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER1_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER2_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER2_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER2_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

  #if ( g_CURRENT_TIER2_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER2_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER2_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER2_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK89_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER2_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER2_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER2_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER2_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER2_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER2_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER2_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER3_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER3_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER3_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER3_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER3_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER3_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER3_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER4_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER4_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER4_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER4_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER4_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER5_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER5_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER5_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER5_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER5_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER5_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER6_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER6_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER6_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER6_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER6_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER6_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER7_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER7_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER8_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER9_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER10_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER10_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER10_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER11_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER11_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER12_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER12_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER12_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER12_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER13_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER13_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER13_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER14_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER14_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER14_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK1_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK2_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK3_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK4_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK5_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK6_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK7_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK8_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK9_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK10_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK11_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK12_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK13_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK14_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK15_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK16_SUMMATION_DELIVERED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#endif /* #if(g_SE_PROFILE_1_1_d == 1) */

#if (g_SE_PROFILE_1_1_d == 1)
#if ( g_GENERIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_GENERIC_ALARM_MASK_c,
		g_Sixteen_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	},
#endif				/* g_GENERIC_ALARM_MASK_ATTRIBUTE_ENABLE_d */	
#if ( g_ELECTRICITY_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_ELECTRICITY_ALARM_MASK_c,
		g_Thirty_Two_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	},
#endif				/* g_ELECTRICITY_ALARM_MASK_ATTRIBUTE_ENABLE_d */

#if ( g_GENERIC_FLOW_MEASURE_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_GENERIC_FLOW_MEASURE_ALARM_MASK_c,
		g_Sixteen_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	},
#endif				/* g_GENERIC_FLOW_MEASURE_ALARM_MASK_ATTRIBUTE_ENABLE_d */	
#if ( g_WATER_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_WATER_SPECIFIC_ALARM_MASK_c,
		g_Sixteen_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	},
#endif				/* g_WATER_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d */	
#if ( g_HEAT_AND_COOLING_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_HEAT_AND_COOLING_SPECIFIC_ALARM_MASK_c,
		g_Sixteen_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	},
#endif				/* g_HEAT_AND_COOLING_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d */		
#if ( g_GAS_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_GAS_SPECIFIC_ALARM_MASK_c,
		g_Sixteen_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	},
#endif				/* g_GAS_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d */
#endif /* End of g_SE_PROFILE_1_1_d == 1*/	

#if (g_SE_PROFILE_1_2_d == 1)
#if ( g_EXTENDED_GENERIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_EXTENDED_GENERIC_ALARM_MASK_c,
		g_Forty_Eight_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	},
#endif
#if ( g_MANUFACTURER_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_MANUFACTURER_ALARM_MASK_c,
		g_Sixteen_Bit_Bitmap_Data_c,
		g_Read_Write_Attribute_c
	},
#endif
#endif /* #if (g_SE_PROFILE_1_2_d == 1) */

/* Block Information Attribute Set (Received) */
#if(g_SE_PROFILE_1_2_d == 1)
#if ( g_CURRENT_NO_TIER_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_NO_TIER_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_NO_TIER_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_NO_TIER_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_NO_TIER_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		


#if ( g_CURRENT_NO_TIER_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_NO_TIER_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_NO_TIER_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_NO_TIER_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_NO_TIER_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			

#if ( g_CURRENT_TIER1_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER1_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER1_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER1_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

	
#if ( g_CURRENT_TIER1_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			

	
#if ( g_CURRENT_TIER1_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			

#if ( g_CURRENT_TIER1_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER1_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
	

#if ( g_CURRENT_TIER1_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER1_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER1_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER1_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER1_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER1_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER2_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER2_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER2_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */

  #if ( g_CURRENT_TIER2_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER2_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER2_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER2_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK89_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER2_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER2_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER2_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER2_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER2_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER2_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER2_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER2_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER2_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER3_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER3_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER3_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER3_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER3_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER3_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER3_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER3_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER3_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER3_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER3_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER4_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER4_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER4_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER4_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER4_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER4_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER4_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER4_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER4_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER5_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER5_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER5_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER5_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER5_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER5_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER5_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER5_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER5_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER6_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER6_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER6_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER6_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER6_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER6_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER6_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER6_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER6_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER7_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER7_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER7_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER7_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER7_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER7_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER8_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER8_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER8_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER8_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER8_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER9_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER9_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER9_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER9_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER9_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER10_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER10_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER10_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER10_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER10_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER10_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER11_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER11_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER11_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER11_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER12_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER12_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER11_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER12_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER12_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER12_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER12_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER12_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER13_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */		
#if ( g_CURRENT_TIER13_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER13_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER13_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER13_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER13_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER14_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER14_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER14_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER14_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER14_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER14_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK1_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK2_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK3_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK4_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK5_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK6_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK7_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK8_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK9_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK10_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK11_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK12_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER15_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK13_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK14_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK15_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#if ( g_CURRENT_TIER15_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_CURRENT_TIER15_BLOCK16_SUMMATION_RECEIVED_c,
		g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif				/* g_CURRENT_TIER15_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d */	
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */

/* Billing Attribute Set */
#if ( g_SE_PROFILE_1_2_d == 1 )
#if ( g_BILL_TO_DATE_IMPORT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_BILL_TO_DATE_IMPORT_c,
		g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_BILL_TO_DATE_TIME_STAMP_IMPORT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_BILL_TO_DATE_TIME_STAMP_IMPORT_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_PROJECTED_BILL_IMPORT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PROJECTED_BILL_IMPORT_c,
		g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_PROJECTED_BILL_TIME_STAMP_IMPORT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PROJECTED_BILL_TIME_STAMP_IMPORT_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_BILL_TO_DATE_EXPORT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_BILL_TO_DATE_EXPORT_c,
		g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_BILL_TO_DATE_TIME_STAMP_EXPORT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_BILL_TO_DATE_TIME_STAMP_EXPORT_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_PROJECTED_BILL_EXPORT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PROJECTED_BILL_EXPORT_c,
		g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
		g_Read_Only_Attribute_c
	},
#endif
#if ( g_PROJECTED_BILL_TIME_STAMP_EXPORT_ATTRIBUTE_ENABLE_d == 1 )
	{
		g_PROJECTED_BILL_TIME_STAMP_EXPORT_c,
		g_UTC_Time_Data_c,
		g_Read_Only_Attribute_c
	}
#endif
#endif /* End of g_SE_PROFILE_1_2_d == 1*/
};

const Metering_Boundary_Values_t m_Metering_Boundary_Value[] =
{
#if ( g_DAILY_FREEZE_TIME_ATTRIBUTE_ENABLE_d == 1 )

    {	g_DAILY_FREEZE_TIME_c,
        m_DAILY_FREEZE_TIME_LIMIT_LOWER_c,
        m_DAILY_FREEZE_TIME_LIMIT_UPPER_c,
	},
#endif
#if ( g_POWER_FACTOR_ATTRIBUTE_ENABLE_d == 1 )

    {	g_POWER_FACTOR_c,
        m_POWER_FACTOR_LIMIT_LOWER_c,
        m_POWER_FACTOR_LIMIT_UPPER_c,
	},
#endif
#if ( g_SE_PROFILE_1_1_d == 1 )
#if ( g_CURRENT_BLOCK_ATTRIBUTE_ENABLE_d == 1 )

    {	g_CURRENT_BLOCK_c,
        m_CURRENT_BLOCK_LIMIT_LOWER_c,
        m_CURRENT_BLOCK_LIMIT_UPPER_c },
	},
#endif

#if ( g_PRESET_READING_TIME_ATTRIBUTE_ENABLED_d == 1 )

    {	g_PRESET_READING_TIME_c,
        m_PRESET_READING_TIME_LIMIT_LOWER_c,
        m_PRESET_READING_TIME_LIMIT_UPPER_c,
	},
#endif
#endif  /* g_SE_PROFILE_1_1_d*/
#if(g_SE_PROFILE_1_2_d == 1)
#if ( g_ACTIVE_REGISTER_TIER_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )

    {	g_ACTIVE_REGISTER_TIER_DELIVERED_c,
        m_ACTIVE_REGISTER_TIER_LIMIT_LOWER_c,
        m_ACTIVE_REGISTER_TIER_LIMIT_UPPER_c,
	},
#endif
#if ( g_ACTIVE_REGISTER_TIER_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )

    {	g_ACTIVE_REGISTER_TIER_RECIEVED_c,
        m_ACTIVE_REGISTER_TIER_LIMIT_LOWER_c,
        m_ACTIVE_REGISTER_TIER_LIMIT_UPPER_c,
	},
#endif
#if ( g_REMAINING_BATTERY_LIFE_IN_DAYS_ATTRIBUTE_ENABLE_d == 1 )

    {	g_REMAINING_BATTERY_LIFE_IN_DAYS_c,
        m_REMAINING_BATTERY_LIFE_IN_DAYS_LIMIT_LOWER_c,
        m_REMAINING_BATTERY_LIFE_IN_DAYS_LIMIT_UPPER_c,
	},
#endif

#if ( g_LOW_MEDIUM_HIGH_STATUS_ATTRIBUTE_ENABLED_d == 1 )

    {	g_LOW_MEDIUM_HIGH_STATUS_c,
        m_LOW_MEDIUM_HIGH_STATUS_LIMIT_LOWER_c,
        m_LOW_MEDIUM_HIGH_STATUS_LIMIT_UPPER_c,
	},
#endif
#endif /* g_SE_PROFILE_1_2_d*/
	{g_NULL_c,  g_NULL_c, g_NULL_c }
};

/*Cluster Info for Simple Metering Cluster*/
const ZCL_Cluster_Info_t ga_Simple_metering_Server_Cluster_Info =
{
    g_SIMPLE_METERING_CLUSTER_c,
    sizeof(ma_simple_metering_Server_Attribute)/sizeof(ZCL_Attribute_Info_t),
    ma_simple_metering_Server_Attribute
};
#endif				/* g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
/*Cluster Info for Simple Metering Cluster*/
const ZCL_Cluster_Info_t ga_Simple_metering_Client_Cluster_Info =
{
	g_SIMPLE_METERING_CLUSTER_c,
	g_NULL_c,
	g_NULL_c
};
#endif				/* g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
uint8_t ZCL_Check_Metering_Boundary_Values ( uint8_t *p_attribute_id,
                                    uint8_t *p_attribute_data,
                                    uint8_t data_length )
{
    int8_t attribute_data_char = 0x00;
    int8_t stored_lower_data = 0x00;
    int8_t stored_upper_data = 0x00;
    uint8_t count;
    uint16_t attribute_id;
    int16_t attribute_data = 0x0000;

    memUtils_memCopy ( (uint8_t *)&attribute_id, p_attribute_id, sizeof(uint16_t) );

    if( data_length > 1 ) {
        memUtils_memCopy ( (uint8_t *)&attribute_data, (uint8_t *)p_attribute_data, data_length );
    }
    else{
        attribute_data_char = *p_attribute_data  ;
    }
    for ( count = 0x00; count <  sizeof( m_Metering_Boundary_Value )/ sizeof(Metering_Boundary_Values_t ); count++ ) {
        if ( attribute_id == m_Metering_Boundary_Value[count].attributeId ) {

            stored_lower_data = m_Metering_Boundary_Value[count].lowerBoundaryValue;
            stored_upper_data = m_Metering_Boundary_Value[count].upperBoundaryValue;

            if ( ( ( data_length > 1 ) &&
               ( ( attribute_data <  m_Metering_Boundary_Value[count].lowerBoundaryValue ) ||
               ( attribute_data >  m_Metering_Boundary_Value[count].upperBoundaryValue ) ) ) ||
               ( ( data_length == 1 ) && ( ( attribute_data_char <  stored_lower_data ) ||
               ( attribute_data_char >  stored_upper_data ) ) ) ) {

                return g_ZCL_Invalid_Value_c;
            }
        }
    }


    return g_ZCL_Success_c;
}
#endif                /* #if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )*/
uint8_t ZCL_Handle_Simple_Metering_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{

    uint8_t responseLength = 0x00;
    pActionHandler->event.eventLength = asduLength - 0x03;
#if ( (g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 ) && (g_SE_PROFILE_1_2_d == 1))
	uint8_t offset = m_PAYLOAD_LOCATION_c;
#endif

    /* Check the direction of received frame */
    if( ( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) ) {

#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )

        switch( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {
            /* Received Server commands */
            case g_Get_Profile_Response_c:
            {
                /* Received Get profile response */
                pActionHandler->action = Received_Response;

                /* Give the get profile reception event to application. */
                pActionHandler->event.eventId = g_Get_Profile_Respnse_Event_c;

                /* Copy the fixed length data from the payload received */
                memUtils_memCopy( pActionHandler->event.pEventData,
                          &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
                          asduLength - g_ZCL_HEADER_WITHOUT_MANUF_CODE_c );
            }
            break;
            case g_Request_Mirror_c:
            {
                pActionHandler->action = No_Response;
                /* Received Request mirror from metering device to ESP.
                   Issue a event to the application, so that application
                   can create REQUEST_MIRROR_RESPONSE and send it to the
                   requested device */
                pActionHandler->event.eventId = g_Request_Mirror_Event_c;
            }
            break;
            case g_Remove_Mirror_c:
            {
                pActionHandler->action = No_Response;
                /* Received Remove mirror from metering device to ESP.
                   send event to the application, so that application
                   can create MIRROR_REMOVED and send it to the
                   requested device */
                pActionHandler->event.eventId = g_Remove_Mirror_Event_c;
            }
            break;
#if(g_SE_PROFILE_1_1_d == 1)		
			case g_Request_Fast_Poll_Mode_Response_c:
			{
			  	/* Received fast poll mode response */
			  	 pActionHandler->action = Received_Response;
				 /* Give the fast poll mode reception event to application. */
				 pActionHandler->event.eventId = g_Request_Fast_Poll_Mode_Response_Event_c;
				  /* Copy the fixed length data from the payload received */
                memUtils_memCopy( pActionHandler->event.pEventData,
                          &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
                          asduLength - g_ZCL_HEADER_WITHOUT_MANUF_CODE_c );
			}
		    break;
#endif
#if(g_SE_PROFILE_1_2_d == 1)		
			case g_Get_Snapshot_Response_c:
			{
			  	/* Received fast poll mode response */
			  	 pActionHandler->action = Received_Response;
				 /* Give the snap shot response event to application. */
				 pActionHandler->event.eventId = g_Get_Snapshot_Response_Event_c;
				  /* Copy the fixed length data from the payload received */
                memUtils_memCopy( pActionHandler->event.pEventData,
                          &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
                          asduLength - g_ZCL_HEADER_WITHOUT_MANUF_CODE_c );
			}
		    break;
			case g_Schedule_Snapshot_Response_c:
			{
			  	/* Received fast poll mode response */
			  	 pActionHandler->action = Received_Response;
				 /* Give the Schedule snap shot response event to application. */
				 pActionHandler->event.eventId = g_Schedule_Snapshot_Response_Event_c;
				  /* Copy the fixed length data from the payload received */
                memUtils_memCopy( pActionHandler->event.pEventData,
                          &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
                          asduLength - g_ZCL_HEADER_WITHOUT_MANUF_CODE_c );
			}
		    break;
			case g_Get_Logging_Response_c:
			{
			  	/* Received fast poll mode response */
			  	 pActionHandler->action = Received_Response;
				 /* Give the get logging response event to application. */
				 pActionHandler->event.eventId = g_Get_Logging_Response_Event_c;
				  /* Copy the fixed length data from the payload received */
                memUtils_memCopy( pActionHandler->event.pEventData,
                          &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
                          asduLength - g_ZCL_HEADER_WITHOUT_MANUF_CODE_c );
			}
		    break;	
			case g_Configure_Mirror_c:
			{
			  	/* Received fast poll mode response */
			  	 pActionHandler->action = Received_Response;
				 /* Give the Configure Mirror event to application. */
				 pActionHandler->event.eventId = g_Configure_Mirror_Event_c;
				  /* Copy the fixed length data from the payload received */
                memUtils_memCopy( pActionHandler->event.pEventData,
                          &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
                          asduLength - g_ZCL_HEADER_WITHOUT_MANUF_CODE_c );
			}
		    break;	
			case g_Configure_Notification_Scheme_c:
			{
			  	/* Received fast poll mode response */
			  	 pActionHandler->action = Received_Response;
				 /* Give the  Configure Notification Scheme event to application. */
				 pActionHandler->event.eventId = g_Configure_Notification_Scheme_Event_c;
				  /* Copy the fixed length data from the payload received */
                memUtils_memCopy( pActionHandler->event.pEventData,
                          &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
                          asduLength - g_ZCL_HEADER_WITHOUT_MANUF_CODE_c );
			}
		    break;		
			case g_Configure_Notification_Flag_c:
			{
			  	/* Received fast poll mode response */
			  	 pActionHandler->action = Received_Response;
				 /* Give the Configure Notification Flag event to application. */
				 pActionHandler->event.eventId = g_Configure_Notification_Flag_Event_c;
				  /* Copy the fixed length data from the payload received */
                memUtils_memCopy( pActionHandler->event.pEventData,
                          &pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
                          asduLength - g_ZCL_HEADER_WITHOUT_MANUF_CODE_c );
			}
		    break;							
#endif		/* g_SE_PROFILE_1_2_d*/				
            default:
                pActionHandler->action = Send_Response;

            break;
         }
#endif /* ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 ) */
    }
    else
    {   /* Received Client commands */
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
        switch(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {

            case g_Get_Profile_c:
            {
                Get_Profile_Command_t* getProfileCommand;
                getProfileCommand = (Get_Profile_Command_t*)pActionHandler->event.pEventData;
                pActionHandler->action = No_Response;
                /* Received get profile from the client device.
                   Raise an event to the application,
                   so that the application create and send
                   GET_PROFILE_RESPONSE to the requested device. */
                pActionHandler->event.eventId = g_Get_Profile_Event_c;
                getProfileCommand->intervalChannel = pReceivedAsdu[m_PAYLOAD_LOCATION_c];
                memUtils_memCopy ( (uint8_t*)&getProfileCommand->endTime,
                           &pReceivedAsdu[m_PAYLOAD_LOCATION_c + 0x01], 0x04 );
                getProfileCommand->noOfPeriods =  pReceivedAsdu[m_PAYLOAD_LOCATION_c + 0x05];
            }
            break;
            case g_Request_Mirror_Response_c:
            {
                /* Received Request mirror response, Send it to the Test Tool */
                pActionHandler->action = Received_Response;

                /* Copy the ZCL payload */
                pActionHandler->event.pEventData[0] = pReceivedAsdu[m_PAYLOAD_LOCATION_c];
                pActionHandler->event.pEventData[1] = pReceivedAsdu[m_PAYLOAD_LOCATION_c + 1];

                /* Notify the application for handling REQUEST_MIRROR_RESPONSE Command */
                pActionHandler->event.eventId      = g_Request_Mirror_Response_Event_c;
            }
            break;
		case g_Remove_Mirror_Response_c:
            {
                /* Received mirror removed, Send it to the Test Tool */
                pActionHandler->action = Received_Response;

                /* Copy the ZCL payload */
                pActionHandler->event.pEventData[0] = pReceivedAsdu[m_PAYLOAD_LOCATION_c];
                pActionHandler->event.pEventData[1] = pReceivedAsdu[m_PAYLOAD_LOCATION_c + 1];

                /* Notify the application for handling MIRROR_REMOVED Command */
                pActionHandler->event.eventId      = g_Remove_Mirror_Response_Event_c;
            }
            break;
#if(g_SE_PROFILE_1_1_d == 1)	
			case g_Request_Fast_Poll_Mode_c:
			{
			    Request_Fast_Poll_Mode_t *requestFastPollMode;
				requestFastPollMode = (Request_Fast_Poll_Mode_t*)pActionHandler->event.pEventData;			
				pActionHandler->action = No_Response;
				/*Received Request for fast poll mode from the client
			  	raise an event to the application */
				pActionHandler->event.eventId = g_Request_Fast_Poll_Mode_Event_c;
				requestFastPollMode->fastPollUpdatePeriod =   pReceivedAsdu[m_PAYLOAD_LOCATION_c];
				requestFastPollMode->duration = pReceivedAsdu[m_PAYLOAD_LOCATION_c+0x01];
			}
		    break;
#endif

#if(g_SE_PROFILE_1_2_d == 1)	
			case g_Get_Snapshot_c:
			{
			    /* request snapshot data from the server*/
			    Get_Snapshot_command_t* getSnapshotCommand;
                getSnapshotCommand = (Get_Snapshot_command_t*)pActionHandler->event.pEventData;
                pActionHandler->action = No_Response;
                /* Received get snapshotcommand from the client device.
                   Raise an event to the application, so that the application create and send
                   snapshot response to the requested device. */
                pActionHandler->event.eventId = g_Get_Snapshot_Event_c;
                memUtils_memCopy ( (uint8_t*)&getSnapshotCommand->startTime,
                           &pReceivedAsdu[m_PAYLOAD_LOCATION_c], 0x04 );
				getSnapshotCommand->noOfSnapshots = pReceivedAsdu[m_PAYLOAD_LOCATION_c+4];
                memUtils_memCopy ( (uint8_t*)&getSnapshotCommand->snapshotCause,
                           &pReceivedAsdu[m_PAYLOAD_LOCATION_c+5], 0x02 );
			}
		    break;
			case g_Take_Snapshot_c:
			{
				pActionHandler->action = No_Response;
				/*Received Request for fast poll mode from the client
			  	raise an event to the application */
				pActionHandler->event.eventId = g_Take_Snapshot_Event_c;
//				requestFastPollMode->fastPoll_updatePeriod =   pReceivedAsdu[m_PAYLOAD_LOCATION_c];
//				requestFastPollMode->duration = pReceivedAsdu[m_PAYLOAD_LOCATION_c+0x01];
			}
		    break;
			case g_MirrorReport_Attribute_Response_c:
			{
				pActionHandler->action = No_Response;
				/*Received Request for fast poll mode from the client
			  	raise an event to the application */
				pActionHandler->event.eventId = g_MirrorReport_Attribute_Response_Event_c;
//				requestFastPollMode->fastPoll_updatePeriod =   pReceivedAsdu[m_PAYLOAD_LOCATION_c];
//				requestFastPollMode->duration = pReceivedAsdu[m_PAYLOAD_LOCATION_c+0x01];
			}
		    break;
			case g_Schedule_SnapShot_c:
			{
				pActionHandler->event.eventId = g_Schedule_SnapShot_Event_c;

				Schedule_Snapshot_command_t *pformattedScheduledEvent =
				  (Schedule_Snapshot_command_t*)pActionHandler->event.pEventData;
				pActionHandler->action = No_Response;
				/*format the schedule snapshot payload before sending event to
				the application*/
				
				memUtils_memCopy( (uint8_t*)&pformattedScheduledEvent->eventId, pReceivedAsdu + offset, 0x04 );
				offset += 0x04;
				pformattedScheduledEvent->commandIndex = *( pReceivedAsdu + offset );
				offset += 0x01;
				memUtils_memCopy( (uint8_t*)&pformattedScheduledEvent->snapshotCause, pReceivedAsdu +offset, 0x02);
				offset += 0x02;
				uint8_t i=0;				
				do {	
					pformattedScheduledEvent->snapshotPayload[i].snapshotScheduleId = *( pReceivedAsdu + offset );
					offset += 0x01;
			
					memUtils_memCopy( (uint8_t*)&pformattedScheduledEvent->snapshotPayload[i].snapshotStartDate,
									 pReceivedAsdu +offset, 0x04);
					offset += 0x04;
					memUtils_memCopy( (uint8_t*)&pformattedScheduledEvent->snapshotPayload[i].snapshotSchedule,
									 pReceivedAsdu +offset, 0x03);
					offset += 0x03;
					pformattedScheduledEvent->snapshotPayload[i].snapshotTypeCause = *( pReceivedAsdu + offset );
					i++;
				}while(pformattedScheduledEvent->commandIndex !=0xFE);
			}
		    break;	
			case g_Start_Logging_c:
			{
			    start_logging_cmd_t *pformattedStartLoggingEvent =
				  (start_logging_cmd_t*)pActionHandler->event.pEventData;
				pActionHandler->action = No_Response;
				pActionHandler->event.eventId = g_Start_Logging_Event_c;
				
				memUtils_memCopy( (uint8_t*)&pformattedStartLoggingEvent->eventId, pReceivedAsdu + offset, 0x04 );
				offset += 0x04;  				
				memUtils_memCopy( (uint8_t*)&pformattedStartLoggingEvent->startProfileTime, pReceivedAsdu + offset, 0x04 );
				offset += 0x04;
				pformattedStartLoggingEvent->logId = *( pReceivedAsdu + offset );
				offset += 0x01; 				
				memUtils_memCopy( (uint8_t*)&pformattedStartLoggingEvent->periodDuration, pReceivedAsdu + offset, 0x02 );
				offset += 0x02; 				
				memUtils_memCopy( (uint8_t*)&pformattedStartLoggingEvent->maxNoOfPeriods, pReceivedAsdu + offset, 0x02 );
				offset += 0x02;
			}
		    break;	
			case g_Get_Logging_c:
			{
			    get_logging_resp_cmd_t *pformattedStartLoggingEvent =
				  (get_logging_resp_cmd_t*)pActionHandler->event.pEventData;
				pActionHandler->action = No_Response;
				pActionHandler->event.eventId = g_Get_Logging_Event_c;
				
				memUtils_memCopy( (uint8_t*)&pformattedStartLoggingEvent->eventId, pReceivedAsdu + offset, 0x04 );
				offset += 0x04;  				
				memUtils_memCopy( (uint8_t*)&pformattedStartLoggingEvent->startProfileTime, pReceivedAsdu + offset, 0x04 );
				offset += 0x04;
				pformattedStartLoggingEvent->logId = *( pReceivedAsdu + offset );
				offset += 0x01; 				
				memUtils_memCopy( (uint8_t*)&pformattedStartLoggingEvent->noOfPeriods, pReceivedAsdu + offset, 0x02 ); 				
			}
		    break;	
			case g_Reset_DemandLimit_Counter_c:
			{
				pActionHandler->action = No_Response;
				/*Received Request for fast poll mode from the client
			  	raise an event to the application */
				pActionHandler->event.eventId = g_Reset_DemandLimit_Counter_Event_c;
//				requestFastPollMode->fastPoll_updatePeriod =   pReceivedAsdu[m_PAYLOAD_LOCATION_c];
//				requestFastPollMode->duration = pReceivedAsdu[m_PAYLOAD_LOCATION_c+0x01];
			}
		    break;	
			case g_Get_Notification_Flag_Command_c:
			{
				pActionHandler->action = No_Response;
				/*Received Request for fast poll mode from the client
			  	raise an event to the application */
				pActionHandler->event.eventId = g_Get_Notification_Flag_Command_Event_c;
//				requestFastPollMode->fastPoll_updatePeriod =   pReceivedAsdu[m_PAYLOAD_LOCATION_c];
//				requestFastPollMode->duration = pReceivedAsdu[m_PAYLOAD_LOCATION_c+0x01];
			}
		    break;					
#endif			
			
			
            default:
                pActionHandler->action = Send_Response;
            break;
        }
#endif /* ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 ) */

    }
    return responseLength;
}

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/

#endif                              /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
