/*******************************************************************************
* ZCL_Price_Cluster.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for supporting Price cluster
*
*******************************************************************************/
#include "ZigBeeDataTypes.h"
#include "memory_utility.h"
#include "ZCL_Foundation.h"
#include "ZCL_Functionality.h"
#if ( g_PRICE_CLUSTER_ENABLE_d == 1)
#include "ZCL_Interface.h"
#include "ZCL_Price_Cluster.h"

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

#define m_GET_CURRENT_PRICE_CMD_LENGTH_c                0x01
#define m_GET_SCHEDULED_PRICE_CMD_LENGTH_c              0x05

#if(g_SE_PROFILE_1_1_d == 1)	
#define m_GET_BLOCK_PERIOD_CMD_LENGTH_c             	0x05

#endif /*g_SE_PROFILE_1_1_d */

/***********************************************	
 Updates of SE 1.1 A
 **********************************************/
#if(g_SE_PROFILE_1_2_d == 1)
#define m_GET_CONVERSION_FACTOR_CMD_LENGTH_c            0x05
#define m_GET_CALORIFIC_VALUE_CMD_LENGTH_c              0x05
#endif  /*g_SE_PROFILE_1_2_d */


#define m_PAYLOAD_LOCATION_c                            0x03
/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Price_Server_Cluster_Attribute_Info [] =
{

/***************** Tier Label Attribute Set*****************/
#if ( g_PRICE_TIER1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_PRICE_TIER1_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_PRICE_TIER2_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_PRICE_TIER3_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_PRICE_TIER4_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c
    },
#endif                          /* g_PRICE_TIER5_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER6_ATTRIBUTE_ENABLE_d== 1 )
  {
      g_PRICE_TIER6_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Write_Attribute_c
  },
#endif                          /* g_PRICE_TIER6_ATTRIBUTE_ENABLE_d*/

/* SE 1.1 Updates*/
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_PRICE_TIER7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER7_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER8_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER9_ATTRIBUTE_ENABLE_d== 1 )
 {
      g_PRICE_TIER9_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER9_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER10_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER11_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER12_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER13_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER14_ATTRIBUTE_ENABLE_d*/

#if ( g_PRICE_TIER15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_ATTRIBUTE_c,
      g_Octet_String_Data_c,
      g_Read_Only_Attribute_c
    },
#endif                          /* g_PRICE_TIER15_ATTRIBUTE_ENABLE_d*/
#endif       /* g_SE_PROFILE_1_1_d*/

/*  SE 1.1 Updates*/
#if(g_SE_PROFILE_1_1_d == 1)	
/*****************  Block Threshold Attribute Set*****************/
#if (g_BLOCK1_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK1_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if (g_BLOCK2_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK2_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if (g_BLOCK3_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK3_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK4_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK4_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK5_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK5_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK6_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK6_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK7_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK7_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK8_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK8_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK9_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK9_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK10_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK10_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK11_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK11_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK12_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK12_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK13_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK13_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK14_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK14_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK15_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK15_THRESHOLD_ATTRIBUTE_c,
      g_unsigned_Fourty_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#endif       /* g_SE_PROFILE_1_1_d*/

/*  SE 1.1 Updates*/
#if(g_SE_PROFILE_1_1_d == 1)	
/***************** Block Period Attribute Set *****************/
#if (g_START_OF_BLOCK_PERIOD_ATTRIBUTE_ENABLE_d== 1)
    {
      g_START_OF_BLOCK_PERIOD_ATTRIBUTE_c,
      g_UTC_Time_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_BLOCK_PERIOD_DURATION_ATTRIBUTE_ENABLE_d== 1)
    {
      g_BLOCK_PERIOD_DURATION_ATTRIBUTE_c,
      g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_THRESHOLD_MULTIPLIER_ATTRIBUTE_ENABLE_d== 1)
    {
      g_THRESHOLD_MULTIPLIER_ATTRIBUTE_c,
      g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_THRESHOLD_DEVISOR_ATTRIBUTE_ENABLE_d== 1)
    {
      g_THRESHOLD_DEVISOR_ATTRIBUTE_c,
      g_Unsigned_Twenty_Four_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
 #endif       /* g_SE_PROFILE_1_1_d*/

/*  SE 1.1 Updates*/
#if(g_SE_PROFILE_1_1_d == 1)	
/*****************Commodity Set*********************/

#if (g_SERVER_COMMODITY_TYPE_ATTRIBUTE_ENABLE_d== 1)
    {
      g_SERVER_COMMODITY_TYPE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_STANDING_CHARGE_ATTRIBUTE_ENABLE_d== 1)
    {
      g_STANDING_CHARGE_ATTRIBUTE_c,
       g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#endif       /* g_SE_PROFILE_1_1_d*/

 /*  Updates of SE 1.1 A        */
#if ( g_SE_PROFILE_1_2_d  == 1 )
#if (g_CONVERSION_FACTOR_ATTRIBUTE_ENABLE_d== 1)
    {
      g_CONVERSION_FACTOR_ATTRIBUTE_c,
       g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif    /* g_CONVERSION_FACTOR_ATTRIBUTE_ENABLE_d*/
#if (g_CONVERSION_FACTOR_TRAILING_DIGIT_ATTRIBUTE_ENABLE_d== 1)
    {
      g_CONVERSION_FACTOR_TRAILING_DIGIT_ATTRIBUTE_c,
       g_Eight_Bit_Bitmap_Data_c,
      g_Read_Only_Attribute_c
    },
#endif   /* g_CONVERSION_FACTOR_TRAILING_DIGIT_ATTRIBUTE_ENABLE_d*/
#if (g_CALORIFIC_VALUE_ATTRIBUTE_ENABLE_d== 1)
    {
      g_CALORIFIC_VALUE_ATTRIBUTE_c,
       g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif     /* g_CALORIFIC_VALUE_ATTRIBUTE_ENABLE_d*/
#if (g_CALORIFIC_VALUE_UNIT_ATTRIBUTE_ENABLE_d== 1)
    {
      g_CALORIFIC_VALUE_UNIT_ATTRIBUTE_c,
       g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c
    },
#endif     /* g_CALORIFIC_VALUE_UNIT_ATTRIBUTE_ENABLE_d*/
#if (g_CALORIFIC_VALUE_TRAILING_DIGIT_ATTRIBUTE_ENABLE_d== 1)
    {
      g_CALORIFIC_VALUE_TRAILING_DIGIT_ATTRIBUTE_c,
       g_Eight_Bit_Bitmap_Data_c,
      g_Read_Only_Attribute_c
    },
#endif /* g_CALORIFIC_VALUE_TRAILING_DIGIT_ATTRIBUTE_ENABLE_d*/
#endif /*g_SE_PROFILE_1_2_d */

/*  SE 1.1 Updates*/
#if(g_SE_PROFILE_1_1_d == 1)	
/***************** Block Price Information set*****************/
#if ( g_PRICE_NO_TIER_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_NO_TIER_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_NO_TIER_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_NO_TIER_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER1_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER1_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER1_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER1_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER1_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER1_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER1_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER2_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER2_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER2_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER2_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER2_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER3_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
 #if ( g_PRICE_TIER3_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER3_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER3_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER4_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER4_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER4_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER4_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER4_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER4_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER4_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER5_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER5_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER5_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER5_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER5_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER6_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER6_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER7_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER7_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER7_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER7_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER7_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER7_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER7_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER8_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER8_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER8_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER8_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER8_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER8_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER8_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER8_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER9_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER9_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER9_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER9_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER9_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER9_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER9_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif


#if ( g_PRICE_TIER10_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER10_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER10_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER10_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER10_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER10_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER10_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER11_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER11_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER11_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER11_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER11_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER11_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER11_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER12_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER12_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER12_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER12_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER12_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER12_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER12_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER13_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER13_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER13_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER13_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER13_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER13_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER13_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER14_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER14_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER14_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER14_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER14_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER14_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER14_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER15_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK1_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK2_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK3_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER15_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK4_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER15_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK5_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER15_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK6_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK7_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( g_PRICE_TIER15_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK8_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK9_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK10_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK11_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK12_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK13_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK14_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK15_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if ( g_PRICE_TIER15_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_TIER15_BLOCK16_ATTRIBUTE_c,
      g_Unsigned_Thirty_Two_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#endif  /* ( g_SE_PROFILE_1_1_d == 1 )*/
	{g_NULL_c, g_NULL_c, g_NULL_c}
};
#endif          /*  ( g_MESSAGING_SERVER_CLUSTER_ENABLE_d == 1 ) */

#if ( g_SE_PROFILE_1_1_d == 1 )
#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
const ZCL_Attribute_Info_t ma_Price_Client_Cluster_Attribute_Info [] =
{

#if ( g_PRICE_INCREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_INCREASE_RANDOMIZE_MINUTES_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif    	
#if ( g_PRICE_DECREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d== 1 )
    {
      g_PRICE_DECREASE_RANDOMIZE_MINUTES_ATTRIBUTE_c,
      g_Unsigned_Eight_Bit_Integer_Data_c,
      g_Read_Only_Attribute_c
    },
#endif
#if (g_CLIENT_COMMODITY_TYPE_ATTRIBUTE_ENABLE_d== 1)
    {
      g_CLIENT_COMMODITY_TYPE_ATTRIBUTE_c,
      g_Eight_Bit_Enumeration_Data_c,
      g_Read_Only_Attribute_c
    },
#endif

#if ( (g_PRICE_INCREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d== 0) &&    \
           (g_PRICE_DECREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d== 0) && \
			 (g_CLIENT_COMMODITY_TYPE_ATTRIBUTE_ENABLE_d==0))
    {g_NULL_c, g_NULL_c, g_NULL_c}
#endif		
};
#endif   /*  ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 ) */
#endif  /* ( g_SE_PROFILE_1_1_d == 1 ) */


#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
const ZCL_Cluster_Info_t ga_Price_Server_Cluster_Info =
{
   g_PRICE_CLUSTER_c,
  ( sizeof( ma_Price_Server_Cluster_Attribute_Info ) -1)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Price_Server_Cluster_Attribute_Info
};

#endif/*  ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ) */


#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
#if ( g_SE_PROFILE_1_1_d == 1 )
const ZCL_Cluster_Info_t ga_Price_Client_Cluster_Info =
{
   g_PRICE_CLUSTER_c,
  (  sizeof( ma_Price_Client_Cluster_Attribute_Info )-1)/ sizeof( ZCL_Attribute_Info_t ),
   ma_Price_Client_Cluster_Attribute_Info
};
#else
const ZCL_Cluster_Info_t ga_Price_Client_Cluster_Info =
{
	g_PRICE_CLUSTER_c,
	g_NULL_c,
	g_NULL_c
};
#endif   /*g_SE_PROFILE_1_1_d */
#endif          /*  ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Price_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation )
{
    pActionHandler->action = No_Response;
    pActionHandler->event.eventLength = asduLength - g_ZCL_HEADER_LENGTH_WITHOUT_MANUF_CODE_c;
   	uint8_t offset = m_PAYLOAD_LOCATION_c;
    if( ( ZCL_GetDirectionOfCommand( pReceivedAsdu ) == g_ZCL_SERVER_TO_CLIENT_DIRECTION_c ) ) {
#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
       pActionHandler->action = Received_Response;

        if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_PublishPriceCmd_c)
        {
			/*Received publish price command from the server,
			  	raise an event to the application */   		
				pActionHandler->event.eventId = g_PublishPriceEvent_c;

			publishPriceInfo_t* pFormatedPriceEvent = (publishPriceInfo_t*)pActionHandler->event.pEventData;

			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->providerId, pReceivedAsdu + offset, 0x04 );
			offset += 0x04;
			memUtils_memCopy( pFormatedPriceEvent->rateLabel, pReceivedAsdu+offset, *( pReceivedAsdu + offset) +1 );
			offset += pFormatedPriceEvent->rateLabel[0] + 1 ;
			
			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->issuerEventId, pReceivedAsdu + offset, 0x04 );
			offset += 0x04;
			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->currentTime, pReceivedAsdu +offset, 0x04 );
			offset += 0x04;
			pFormatedPriceEvent->unitOfMeasure = *( pReceivedAsdu + offset );
			offset += 0x01;
			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->currency, pReceivedAsdu +offset, 0x04 );
			offset += 0x02;
			pFormatedPriceEvent->priceTrailingDigitAndPriceTier = *( pReceivedAsdu + offset );
			offset += 0x01;
			pFormatedPriceEvent->numOfPriceTiersAndRegisterTier = *( pReceivedAsdu + offset );
			offset += 0x01;
			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->startTime, pReceivedAsdu +offset, 0x04 );
			offset += 0x04;
			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->duration, pReceivedAsdu +offset, 0x04 );
			offset += 0x02;
			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->price, pReceivedAsdu +offset, 0x04 );
			offset += 0x04;
			pFormatedPriceEvent->priceRatio = *( pReceivedAsdu + offset );
			offset += 0x01;
			
			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->generationPrice, pReceivedAsdu +offset, 0x04 );
			offset += 0x04;
			pFormatedPriceEvent->generationPriceRatio = *( pReceivedAsdu + offset );
			offset += 0x01;
			
			memUtils_memCopy( (uint8_t*)&pFormatedPriceEvent->alternateCostDelivered, pReceivedAsdu +offset, 0x04 );
			offset += 0x04;
			pFormatedPriceEvent->alternateCostUnit = *( pReceivedAsdu + offset );
			offset += 0x01;
			pFormatedPriceEvent->alternateCostTrailingDigit = *( pReceivedAsdu + offset );
			offset += 0x01;
#if ( g_SE_PROFILE_1_1_d == 1 )                        
			pFormatedPriceEvent->numberOfBlockThresholds = *( pReceivedAsdu + offset );
			offset += 0x01;
			pFormatedPriceEvent->priceControl = *( pReceivedAsdu + offset );				
#endif //#if ( g_SE_PROFILE_1_1_d == 1 )                        

		}
#if(g_SE_PROFILE_1_1_d == 1)	
		else if( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) == g_PublishBlockPeriodCmd_c){
			/*Received publish block period command from the server,
			  	raise an event to the application */   		
			pActionHandler->event.eventId = g_PublishBlockPeriodEvent_c;

			publishBlockPeriodInfo_t* pFormatedBlockPeriodevent =
			  		(publishBlockPeriodInfo_t*)pActionHandler->event.pEventData;
	
			memUtils_memCopy( (uint8_t*)&pFormatedBlockPeriodevent->providerId,
												pReceivedAsdu + offset, 0x04 );
			offset += 0x04;
			
			memUtils_memCopy( (uint8_t*)&pFormatedBlockPeriodevent->issuerEventId,
												pReceivedAsdu + offset, 0x04 );
			offset += 0x04;
			memUtils_memCopy( (uint8_t*)&pFormatedBlockPeriodevent->blockPeriodStartTime,
										pReceivedAsdu +offset, 0x04 );
			offset += 0x04;
			
			memUtils_memCopy( (uint8_t*)&pFormatedBlockPeriodevent->blockPeriodDurationInMinutes,
										pReceivedAsdu +offset, 0x03 );
			offset += 0x03;
			pFormatedBlockPeriodevent->numberOfPriceTiersAndBlockThresholds =
											*( pReceivedAsdu + offset );
			offset += 0x01;
			pFormatedBlockPeriodevent->blockPeriodControl = *( pReceivedAsdu + offset );
					
				
		}
#endif	 /* g_SE_PROFILE_1_1_d*/
#if(g_SE_PROFILE_1_2_d == 1)	
/*------------------------------------- SE 1.1 A update --------------*/			
		else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ==
													g_PublishConversionFactor_c) {
			   /*Received publish converion factor command from the server,
			  	raise an event to the application */
			pActionHandler->event.eventId = g_PublishConversionFactorEvent_c;													
			publishConversionFactor_t* pFormatedConvFactEvent =
			  		(publishConversionFactor_t*)pActionHandler->event.pEventData;
			
			memUtils_memCopy( (uint8_t*)&pFormatedConvFactEvent->issuerEventId,
												pReceivedAsdu + offset, 0x04 );
			offset += 0x04; 			
			memUtils_memCopy( (uint8_t*)&pFormatedConvFactEvent->startTime,
												pReceivedAsdu + offset, 0x04 );
			offset += 0x04;
		    pFormatedConvFactEvent->conversionFactor =
											*( pReceivedAsdu + offset );
			offset += 0x01;
			pFormatedConvFactEvent->conversionFactorTrailingDigit =
											*( pReceivedAsdu + offset );
			offset += 0x01;
		}
		else if(*( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) ==
													g_PublishCalorificValue_c) {
			/*Received publish calorific value command from the server,
			  	raise an event to the application */   		
			pActionHandler->event.eventId = g_PublishCalorificValueEvent_c;													
			publishCalorificValue_t* pFormatedClorifcValtEvent =
			  		(publishCalorificValue_t*)pActionHandler->event.pEventData;
			
			memUtils_memCopy( (uint8_t*)&pFormatedClorifcValtEvent->issuerEventId,
												pReceivedAsdu + offset, 0x04 );
			offset += 0x04; 			
			memUtils_memCopy( (uint8_t*)&pFormatedClorifcValtEvent->startTime,
												pReceivedAsdu + offset, 0x04 );
			offset += 0x04; 			
			memUtils_memCopy( (uint8_t*)&pFormatedClorifcValtEvent->calorificValue,
												pReceivedAsdu + offset, 0x04 );
			offset += 0x04;
		    pFormatedClorifcValtEvent->calorificValueUnit =
											*( pReceivedAsdu + offset );
			offset += 0x01;
			pFormatedClorifcValtEvent->calorificValueTrailingDigit =
											*( pReceivedAsdu + offset );
			offset += 0x01;
		}
/*------------------------------------- SE 1.1 A END --------------*/				
#endif /*g_SE_PROFILE_1_2_d */		
        else {
        		pActionHandler->action = Send_Response;
        }
#endif      /* ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 ) */
    }
    else{
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
       pActionHandler->action = Received_Response;

        switch ( *( pReceivedAsdu + g_ZCL_HEADER_COMMAND_OFFSET_c ) )
        {
			case g_GetCurrentPriceCmd_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetCurrentPriceEvent_c ;
				memUtils_memCopy ( pActionHandler->event.pEventData,
						  				&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
										   m_GET_CURRENT_PRICE_CMD_LENGTH_c );
				break;
	
			case g_GetScheduledPriceCmd_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetScheduledPricesEvent_c;
				memUtils_memCopy ( pActionHandler->event.pEventData,
						  				&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
									   m_GET_SCHEDULED_PRICE_CMD_LENGTH_c );
				break;
#if(g_SE_PROFILE_1_1_d == 1)				
		case g_PriceAcknwoledgementCmd_c:
		  		{
				/* This event is handled by the application.*/
			    pActionHandler->event.eventId = g_PriceAcknowledgementEvent_c;

				 priceAcknowledgementInfo_t* pFormatedPriceAckEvent =
				   (priceAcknowledgementInfo_t*)pActionHandler->event.pEventData;
				
				memUtils_memCopy( (uint8_t*)&pFormatedPriceAckEvent->providerId,
								 pReceivedAsdu + offset, 0x04 );
				offset += 0x04;
			
				memUtils_memCopy( (uint8_t*)&pFormatedPriceAckEvent->issuerEventId,
								 pReceivedAsdu + offset, 0x04 );
				offset += 0x04;
				memUtils_memCopy( (uint8_t*)&pFormatedPriceAckEvent->priceAckTime,
								 pReceivedAsdu +offset, 0x04 );
				offset += 0x04;
				pFormatedPriceAckEvent->control = *( pReceivedAsdu + offset );
			}
				break;
			case g_GetBlockPeriodsCmd_c:
				/* This event is handled by the application.*/
			     pActionHandler->event.eventId = g_GetBlockPeriodsEvent_c;
				 memUtils_memCopy ( pActionHandler->event.pEventData,
						   				&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
										   m_GET_BLOCK_PERIOD_CMD_LENGTH_c );
		  		break;
#endif /* g_SE_PROFILE_1_1_d*/ 		
				
#if(g_SE_PROFILE_1_2_d == 1)	
/*------------------------------------- SE 1.1 A update --------------*/
			case g_GetConversionFactor_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetConversionFactorEvent_c;
				memUtils_memCopy ( pActionHandler->event.pEventData,
							&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
							m_GET_CONVERSION_FACTOR_CMD_LENGTH_c );
			break;
			case g_GetCalorificValueEvent_c:
				/* This event is handled by the application.*/
				pActionHandler->event.eventId = g_GetCalorificValueEvent_c;
				memUtils_memCopy ( pActionHandler->event.pEventData,
							&pReceivedAsdu[ m_PAYLOAD_LOCATION_c ],
							m_GET_CALORIFIC_VALUE_CMD_LENGTH_c );  			
			break;
/*------------------------------------- SE 1.1 A END --------------*/			
#endif	/* g_SE_PROFILE_1_2_d*/
			default:
				pActionHandler->action = Send_Response;
				break;
        }

#endif      /* ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ) */
    }
    return g_ZERO_DATA_LENGTH_c;
}


/*******************************************************************************/
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
uint8_t ZCL_CreatePublishPriceCmd ( uint8_t tranSeqNum, uint8_t* pRspLocation,
        publishPriceInfo_t *pPriceInfo )
{

    uint8_t dataLength = 0x03;
    uint32_t currentTime;

    pRspLocation[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
        g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    pRspLocation[1] = tranSeqNum;
    pRspLocation[2] = g_PublishPriceCmd_c;

    memUtils_memCopy ( (uint8_t *)&pRspLocation[dataLength], (uint8_t *)pPriceInfo, 0x04 );
    dataLength += 0x04;
    pRspLocation[dataLength++] = pPriceInfo->rateLabel[0];

    memUtils_memCopy ( (uint8_t *)&pRspLocation[dataLength],
               (uint8_t*)&pPriceInfo->rateLabel[1], pPriceInfo->rateLabel[0] );

    dataLength += pPriceInfo->rateLabel[0];

    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength],(uint8_t*)&pPriceInfo->issuerEventId, 0x04 );
    dataLength += 0x04;

    currentTime = ZCL_CallBackGetCurrentTime();

    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength],(uint8_t*)&currentTime, 0x04 );
    dataLength += 0x04;
    pRspLocation[dataLength] = pPriceInfo->unitOfMeasure;
    dataLength += 0x01;
    memUtils_memCopy ( (uint8_t *)&pRspLocation[dataLength],(uint8_t*)&pPriceInfo->currency, 0x02 );
    dataLength += 0x02;
    pRspLocation[dataLength] = pPriceInfo->priceTrailingDigitAndPriceTier;
    dataLength += 0x01;
    pRspLocation[dataLength] = pPriceInfo->numOfPriceTiersAndRegisterTier;
    dataLength += 0x01;
    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength], (uint8_t*)&pPriceInfo->startTime, 0x04 );
    dataLength += 0x04;
    memUtils_memCopy ( (uint8_t *)&pRspLocation[dataLength],(uint8_t*)&pPriceInfo->duration, 0x02 );
    dataLength += 0x02;
    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength], (uint8_t*)&pPriceInfo->price, 0x04 );
    dataLength += 0x04;
    pRspLocation[dataLength] = pPriceInfo->priceRatio;
    dataLength += 0x01;
    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength],(uint8_t*)&pPriceInfo->generationPrice, 0x04 );
    dataLength += 0x04;
    pRspLocation[dataLength] = pPriceInfo->generationPriceRatio;
    dataLength += 0x01;
    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength],
                                (uint8_t*)&pPriceInfo->alternateCostDelivered, 0x04 );
    dataLength += 0x04;
    pRspLocation[dataLength] = pPriceInfo->alternateCostUnit;
    dataLength += 0x01;
    pRspLocation[dataLength] = pPriceInfo->alternateCostTrailingDigit;
    dataLength += 0x01;
#if(g_SE_PROFILE_1_1_d == 1)    
    pRspLocation[dataLength] = pPriceInfo->numberOfBlockThresholds;
    dataLength += 0x01;
    pRspLocation[dataLength] = pPriceInfo->priceControl;
    dataLength += 0x01;
#endif //#if(g_SE_PROFILE_1_1_d == 1)    

    return dataLength;
}
/*******************************************************************************/
#if(g_SE_PROFILE_1_1_d == 1)	
uint8_t ZCL_CreatePublishBlockPriceCmd ( uint8_t tranSeqNum, uint8_t* pRspLocation,
        publishBlockPeriodInfo_t *pBlockPeriodInfo )
{

    uint8_t dataLength = 0x03;
    uint32_t blockPeriodStartTime;

    pRspLocation[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
        g_ZCL_SERVER_TO_CLIENT_DIRECTION_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    pRspLocation[1] = tranSeqNum;
    pRspLocation[2] = g_PublishBlockPeriodCmd_c;

    memUtils_memCopy ( (uint8_t *)&pRspLocation[dataLength], (uint8_t *)pBlockPeriodInfo, 0x04 );
    dataLength += 0x04;

    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength],(uint8_t*)&pBlockPeriodInfo->issuerEventId, 0x04 );
    dataLength += 0x04;

    blockPeriodStartTime = ZCL_CallBackGetCurrentTime();

    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength],(uint8_t*)&blockPeriodStartTime, 0x04 );
    dataLength += 0x04;
	
    memUtils_memCopy ( (uint8_t *)&pRspLocation[dataLength],
			  (uint8_t*)&pBlockPeriodInfo->blockPeriodDurationInMinutes, 0x03 );
    dataLength += 0x03;
	
    pRspLocation[dataLength] = pBlockPeriodInfo->numberOfPriceTiersAndBlockThresholds;
    dataLength += 0x01;
    pRspLocation[dataLength] = pBlockPeriodInfo->blockPeriodControl;
    dataLength += 0x01;

    return dataLength;
}
#endif

#endif    /* ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ) */

/*******************************************************************************/

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
#if(g_SE_PROFILE_1_1_d == 1)	
uint8_t ZCL_CreatePriceAcknowledgementCmd ( uint8_t tranSeqNum, uint8_t* pRspLocation,
        priceAcknowledgementInfo_t *pPriceAckInfo )
{

    uint8_t dataLength = 0x03;
    uint32_t priceAckTime;

    pRspLocation[0] = g_ZCL_FC_CLUSTER_SPECIFIC_COMMAND_c |
        g_ZCL_CLIENT_TO_SERVER_DIRECTION_c | g_ZCL_DEFAULT_RESPONSE_NOT_REQUIRED_c;

    pRspLocation[1] = tranSeqNum;
    pRspLocation[2] = g_PriceAcknwoledgementCmd_c;

    memUtils_memCopy ( (uint8_t *)&pRspLocation[dataLength], (uint8_t *)pPriceAckInfo, 0x04 );
    dataLength += 0x04;

    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength],(uint8_t*)&pPriceAckInfo->issuerEventId, 0x04 );
    dataLength += 0x04;

    priceAckTime = ZCL_CallBackGetCurrentTime();

    memUtils_memCopy ( (uint8_t*)&pRspLocation[dataLength],(uint8_t*)&priceAckTime, 0x04 );
    dataLength += 0x04;
    pRspLocation[dataLength] = pPriceAckInfo->control;
    dataLength += 0x01;
    return dataLength;

}
#endif
#endif    /* ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 ) */
/*******************************************************************************/

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                        /* g_PRICE_CLUSTER_ENABLE_d */
