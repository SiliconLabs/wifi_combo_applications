/******************************************************************************
* ZCL_General_Simple_Metering_Cluster.h
******************************************************************************
* Program Description:
* This file contains the General_Simple_Metering_Cluster attribute and cmmnd
* information
******************************************************************************/
#ifndef _ZCL_SIMPLE_METERING_CLUSTER_H_
#define _ZCL_SIMPLE_METERING_CLUSTER_H_


#if (g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1)
/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/

/* Binary input cluster */
#define g_SIMPLE_METERING_CLUSTER_c                                    0x0702


#if (g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1)
/* Reading Information Attribute set */
#define g_CURRENT_SUMMATION_DELIVERED_c                                 0x0000
#define g_CURRENT_SUMMATION_RECIEVED_c                                  0x0001
#define g_CURRENT_MAX_DEMAND_DELIVERED_c                                0x0002
#define g_CURRENT_MAX_DEMAND_RECIEVED_c                                 0x0003
#define g_DFT_SUMMATION_c                                               0x0004
#define g_DAILY_FREEZE_TIME_c                                           0x0005
#define g_POWER_FACTOR_c                                                0x0006
#define g_READING_SNAP_SHOT_TIME_c                                      0x0007
#define g_CURRENT_MAX_DEMAND_DELIVERED_TIME_c                           0x0008
#define g_CURRENT_MAX_DEMAND_RECIEVED_TIME_c                            0x0009
#if ( g_SE_PROFILE_1_1_d == 1 )
#define g_DEFAULT_UPDATE_PERIOD_c                           			0x000a
#define g_FAST_POLL_UPDATE_PERIOD_c                           			0x000b
#define g_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERED_c                  0x000c
#define g_DAILY_CONSUMPTION_TARGET_c                           			0x000d
#define g_CURRENT_BLOCK_c                           				    0x000e
#define g_PROFILE_INTERVAL_PERIOD_c                           			0x000f
#define g_INTERVAL_READ_REPORTING_PERIOD_c                           	0x0010
#define g_PRESET_READING_TIME_c                           				0x0011
#define g_VOLUME_PER_REPORT_c                           				0x0012
#define g_FLOW_RESTRICTION_c                           					0x0013
#define g_SUPPLY_STATUS_c                           					0x0014
#define g_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_c    					0x0015
#define g_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_c                  	0x0016
#define g_INLET_TEMPERATURE_SUMMATION_c					                0x0017
#define g_OUTLET_TEMPERATURE_SUMMATION_c				                0x0018
#define g_CONTROL_TEMPERATURE_SUMMATION_c				                0x0019
#define g_CURRENT_INLET_ENERGY_CARRIER_DEMAND_c			                0x001a
#define g_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_c			            0x001b
#endif
#if ( g_SE_PROFILE_1_2_d == 1 )
#define g_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERED_c                 0x001c
#define g_CURRENT_BLOCK_PERIOD_CONSUMPTION_RECIEVED_c                   0x001d
#define g_CURRENT_BLOCK_RECIEVED_c                                      0x001e
#define g_DFT_SUMMATION_RECIEVED_c                                      0x001f
#define g_ACTIVE_REGISTER_TIER_DELIVERED_c                              0x0020
#define g_ACTIVE_REGISTER_TIER_RECIEVED_c                               0x0021
#endif /* g_SE_PROFILE_1_2_d*/

/* TOU Information Attribute Set */
#define g_CURRENT_TIER1_SUMMATION_DELIVERED_c                            0x0100
#define g_CURRENT_TIER1_SUMMATION_RECIEVED_c                             0x0101
#define g_CURRENT_TIER2_SUMMATION_DELIVERED_c                            0x0102
#define g_CURRENT_TIER2_SUMMATION_RECIEVED_c                             0x0103
#define g_CURRENT_TIER3_SUMMATION_DELIVERED_c                            0x0104
#define g_CURRENT_TIER3_SUMMATION_RECIEVED_c                             0x0105
#define g_CURRENT_TIER4_SUMMATION_DELIVERED_c                            0x0106
#define g_CURRENT_TIER4_SUMMATION_RECIEVED_c                             0x0107
#define g_CURRENT_TIER5_SUMMATION_DELIVERED_c                            0x0108
#define g_CURRENT_TIER5_SUMMATION_RECIEVED_c                             0x0109
#define g_CURRENT_TIER6_SUMMATION_DELIVERED_c                            0x010A
#define g_CURRENT_TIER6_SUMMATION_RECIEVED_c                             0x010B
#if ( g_SE_PROFILE_1_1_d == 1 )
#define g_CURRENT_TIER7_SUMMATION_DELIVERED_c                            0x010C
#define g_CURRENT_TIER7_SUMMATION_RECIEVED_c                             0x010D
#define g_CURRENT_TIER8_SUMMATION_DELIVERED_c                            0x010E
#define g_CURRENT_TIER8_SUMMATION_RECIEVED_c                             0x010F
#define g_CURRENT_TIER9_SUMMATION_DELIVERED_c                            0x0110
#define g_CURRENT_TIER9_SUMMATION_RECIEVED_c                             0x0111
#define g_CURRENT_TIER10_SUMMATION_DELIVERED_c                           0x0112
#define g_CURRENT_TIER10_SUMMATION_RECIEVED_c                            0x0113
#define g_CURRENT_TIER11_SUMMATION_DELIVERED_c                           0x0114
#define g_CURRENT_TIER11_SUMMATION_RECIEVED_c                            0x0115
#define g_CURRENT_TIER12_SUMMATION_DELIVERED_c                           0x0116
#define g_CURRENT_TIER12_SUMMATION_RECIEVED_c                            0x0117
#define g_CURRENT_TIER13_SUMMATION_DELIVERED_c                           0x0118
#define g_CURRENT_TIER13_SUMMATION_RECIEVED_c                            0x0119
#define g_CURRENT_TIER14_SUMMATION_DELIVERED_c                           0x011A
#define g_CURRENT_TIER14_SUMMATION_RECIEVED_c                            0x011B
#define g_CURRENT_TIER15_SUMMATION_DELIVERED_c                           0x011C
#define g_CURRENT_TIER15_SUMMATION_RECIEVED_c                            0x011D
#endif /*  ( g_SE_PROFILE_1_1_d == 1 ) */
#if ( g_SE_PROFILE_1_2_d == 1 )
#define g_CURRENT_TIER16_SUMMATION_DELIVERED_c                           0x011E
#define g_CURRENT_TIER16_SUMMATION_RECIEVED_c                            0x011F
#define g_CURRENT_TIER17_SUMMATION_DELIVERED_c                           0x0120
#define g_CURRENT_TIER17_SUMMATION_RECIEVED_c                            0x0121
#define g_CURRENT_TIER18_SUMMATION_DELIVERED_c                           0x0122
#define g_CURRENT_TIER18_SUMMATION_RECIEVED_c                            0x0123
#define g_CURRENT_TIER19_SUMMATION_DELIVERED_c                           0x0124
#define g_CURRENT_TIER19_SUMMATION_RECIEVED_c                            0x0125
#define g_CURRENT_TIER20_SUMMATION_DELIVERED_c                           0x0126
#define g_CURRENT_TIER20_SUMMATION_RECIEVED_c                            0x0127
#define g_CURRENT_TIER21_SUMMATION_DELIVERED_c                           0x0128
#define g_CURRENT_TIER21_SUMMATION_RECIEVED_c                            0x0129
#define g_CURRENT_TIER22_SUMMATION_DELIVERED_c                           0x012A
#define g_CURRENT_TIER22_SUMMATION_RECIEVED_c                            0x012B
#define g_CURRENT_TIER23_SUMMATION_DELIVERED_c                           0x012C
#define g_CURRENT_TIER23_SUMMATION_RECIEVED_c                            0x012D
#define g_CURRENT_TIER24_SUMMATION_DELIVERED_c                           0x012E
#define g_CURRENT_TIER24_SUMMATION_RECIEVED_c                            0x012F
#define g_CURRENT_TIER25_SUMMATION_DELIVERED_c                           0x0130
#define g_CURRENT_TIER25_SUMMATION_RECIEVED_c                            0x0131
#define g_CURRENT_TIER26_SUMMATION_DELIVERED_c                           0x0132
#define g_CURRENT_TIER26_SUMMATION_RECIEVED_c                            0x0133
#define g_CURRENT_TIER27_SUMMATION_DELIVERED_c                           0x0134
#define g_CURRENT_TIER27_SUMMATION_RECIEVED_c                            0x0135
#define g_CURRENT_TIER28_SUMMATION_DELIVERED_c                           0x0136
#define g_CURRENT_TIER28_SUMMATION_RECIEVED_c                            0x0137
#define g_CURRENT_TIER29_SUMMATION_DELIVERED_c                           0x0138
#define g_CURRENT_TIER29_SUMMATION_RECIEVED_c                            0x0139
#define g_CURRENT_TIER30_SUMMATION_DELIVERED_c                           0x013A
#define g_CURRENT_TIER30_SUMMATION_RECIEVED_c                            0x013B
#define g_CURRENT_TIER31_SUMMATION_DELIVERED_c                           0x013C
#define g_CURRENT_TIER31_SUMMATION_RECIEVED_c                            0x013D
#define g_CURRENT_TIER32_SUMMATION_DELIVERED_c                           0x013E
#define g_CURRENT_TIER32_SUMMATION_RECIEVED_c                            0x013F
#define g_CURRENT_TIER33_SUMMATION_DELIVERED_c                           0x0140
#define g_CURRENT_TIER33_SUMMATION_RECIEVED_c                            0x0141
#define g_CURRENT_TIER34_SUMMATION_DELIVERED_c                           0x0142
#define g_CURRENT_TIER34_SUMMATION_RECIEVED_c                            0x0143
#define g_CURRENT_TIER35_SUMMATION_DELIVERED_c                           0x0144
#define g_CURRENT_TIER35_SUMMATION_RECIEVED_c                            0x0145
#define g_CURRENT_TIER36_SUMMATION_DELIVERED_c                           0x0146
#define g_CURRENT_TIER36_SUMMATION_RECIEVED_c                            0x0147
#define g_CURRENT_TIER37_SUMMATION_DELIVERED_c                           0x0148
#define g_CURRENT_TIER37_SUMMATION_RECIEVED_c                            0x0149
#define g_CURRENT_TIER38_SUMMATION_DELIVERED_c                           0x014A
#define g_CURRENT_TIER38_SUMMATION_RECIEVED_c                            0x014B
#define g_CURRENT_TIER39_SUMMATION_DELIVERED_c                           0x014C
#define g_CURRENT_TIER39_SUMMATION_RECIEVED_c                            0x014D
#define g_CURRENT_TIER40_SUMMATION_DELIVERED_c                           0x014E
#define g_CURRENT_TIER40_SUMMATION_RECIEVED_c                            0x014F
#define g_CURRENT_TIER41_SUMMATION_DELIVERED_c                           0x0150
#define g_CURRENT_TIER41_SUMMATION_RECIEVED_c                            0x0151
#define g_CURRENT_TIER42_SUMMATION_DELIVERED_c                           0x0152
#define g_CURRENT_TIER42_SUMMATION_RECIEVED_c                            0x0153
#define g_CURRENT_TIER43_SUMMATION_DELIVERED_c                           0x0154
#define g_CURRENT_TIER43_SUMMATION_RECIEVED_c                            0x0155
#define g_CURRENT_TIER44_SUMMATION_DELIVERED_c                           0x0156
#define g_CURRENT_TIER44_SUMMATION_RECIEVED_c                            0x0157
#define g_CURRENT_TIER45_SUMMATION_DELIVERED_c                           0x0158
#define g_CURRENT_TIER45_SUMMATION_RECIEVED_c                            0x0159
#define g_CURRENT_TIER46_SUMMATION_DELIVERED_c                           0x015A
#define g_CURRENT_TIER46_SUMMATION_RECIEVED_c                            0x015B
#define g_CURRENT_TIER47_SUMMATION_DELIVERED_c                           0x015C
#define g_CURRENT_TIER47_SUMMATION_RECIEVED_c                            0x015D
#define g_CURRENT_TIER48_SUMMATION_DELIVERED_c                           0x015E
#define g_CURRENT_TIER48_SUMMATION_RECIEVED_c                            0x015F
#endif /*  ( g_SE_PROFILE_1_2_d == 1 ) */

/* Meter Status Attribute Set */
#define g_CURRENT_STATUS_c                                              0x0200
#if(g_SE_PROFILE_1_1_d == 1)	
#define g_REMAINING_BATTERY_LIFE_c                                      0x0201
#define g_HOURS_IN_OPERATION_c                                          0x0202
#define g_HOURS_IN_FAULT_c                                              0x0203
#endif /*  ( g_SE_PROFILE_1_1_d == 1 ) */
#if(g_SE_PROFILE_1_2_d == 1)
#define g_EXTENDED_STATUS_c                                             0x0204
#define g_REMAINING_BATTERY_LIFE_IN_DAYS_c                              0x0205
#define g_CURRENT_METER_ID_c                                            0x0206
#define g_LOW_MEDIUM_HIGH_STATUS_c                                      0x0207
#endif /*  ( g_SE_PROFILE_1_2_d == 1 ) */

/* Formatting Attribute Set*/
#define g_UNIT_OF_MEASURE_c                                             0x0300
#define g_MULTIPLIER_c                                                  0x0301
#define g_DIVISOR_c                                                     0x0302
#define g_SUMMATION_FORMATTING_c                                        0x0303
#define g_DEMAND_FORMATTING_c                                           0x0304
#define g_HISTORICAL_CONSUMPTION_FORMATTING_c                           0x0305
#define g_METERING_DEVICE_TYPE_c                                        0x0306
#if(g_SE_PROFILE_1_1_d==1)	
#define g_SITE_ID_c                                       				0x0307
#define g_METER_SERIAL_NUMBER_c	                                        0x0308
#define g_ENERGY_CARRIER_UNIT_OF_MEASURE_c                              0x0309
#define g_ENERGY_CARRIER_SUMMATION_FORMATTING_c                         0x030a
#define g_ENERGY_CARRIER_DEMAND_FORMATTING_c                            0x030b
#define g_TEMPERATURE_UNIT_OF_MEASURE_c		                            0x030c
#define g_TEMPERATURE_FORMATTING_c			                            0x030d
#endif  /*  ( g_SE_PROFILE_1_1_d == 1 ) */
#if(g_SE_PROFILE_1_2_d==1)
#define g_MODULE_SERIAL_NUMBER_c                                        0x030E
#define g_OPERATING_TARIFF_LABEL_DELIVERED_c                            0x030F
#define g_OPERATING_TARIFF_LABEL_RECEIVED_c                             0x0310
#endif  /*  ( g_SE_PROFILE_1_2_d == 1 ) */

/* ESP Historical Attribute Set */
#define g_INSTANTANEOUS_DEMAND_c                                        0x0400
#define g_CURRENT_DAY_CONSUMPTION_DELIVERED_c                           0x0401
#define g_CURRENT_DAY_CONSUMPTION_RECEIVED_c                            0x0402
#define g_PREVIOUS_DAY_CONSUMPTION_DELIVERED_c                          0x0403
#define g_PREVIOUS_DAY_CONSUMPTION_RECEIVED_c                           0x0404
#define g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_c       0x0405
#define g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_RECIEVED_c        0x0406
#define g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_c            0x0407
#define g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_RECIEVED_c             0x0408


#if(g_SE_PROFILE_1_1_d == 1)	
#define g_CURRENT_DAY_MAX_PRESSURE_c             						0x0409
#define g_CURRENT_DAY_MIN_PRESSURE_c            						0x040a
#define g_PREVIOUS_DAY_MAX_PRESSURE_c           						0x040b
#define g_PREVIOUS_DAY_MIN_PRESSURE_c            						0x040c
#define g_CURRENT_DAY_MAX_DEMAND_c             							0x040d
#define g_PREVIOUS_DAY_MAX_DEMAND_c            							0x040e
#define g_CURRENT_MONTH_MAX_DEMAND_c             						0x040f
#define g_CURRENT_YEAR_MAX_DEMAND_c            							0x0410
#define g_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_c             			0x0411
#define g_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_c            			0x0412
#define g_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND_c            			0x0413
#define g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_c            			0x0414
#define g_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_c            			0x0415
#define g_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_c            			0x0416
#endif  /* End of g_SE_PROFILE_1_1_d == 1 */
#if(g_SE_PROFILE_1_2_d == 1)
#define g_PREVIOUS_DAY2_CONSUMPTION_DELIVERED_c                         0x0420
#define g_PREVIOUS_DAY2_CONSUMPTION_RECEIVED_c                          0x0421
#define g_PREVIOUS_DAY3_CONSUMPTION_DELIVERED_c                         0x0422
#define g_PREVIOUS_DAY3_CONSUMPTION_RECEIVED_c                          0x0423
#define g_PREVIOUS_DAY4_CONSUMPTION_DELIVERED_c                         0x0424
#define g_PREVIOUS_DAY4_CONSUMPTION_RECEIVED_c                          0x0425
#define g_PREVIOUS_DAY5_CONSUMPTION_DELIVERED_c                         0x0426
#define g_PREVIOUS_DAY5_CONSUMPTION_RECEIVED_c                          0x0427
#define g_PREVIOUS_DAY6_CONSUMPTION_DELIVERED_c                         0x0428
#define g_PREVIOUS_DAY6_CONSUMPTION_RECEIVED_c                          0x0429
#define g_PREVIOUS_DAY7_CONSUMPTION_DELIVERED_c                         0x042A
#define g_PREVIOUS_DAY7_CONSUMPTION_RECEIVED_c                          0x042B
#define g_PREVIOUS_DAY8_CONSUMPTION_DELIVERED_c                         0x042C
#define g_PREVIOUS_DAY8_CONSUMPTION_RECEIVED_c                          0x042D
#define g_CURRENT_WEEK_CONSUMPTION_DELIVERED_c                          0x0430
#define g_CURRENT_WEEK_CONSUMPTION_RECEIVED_c                           0x0431
#define g_PREVIOUS_WEEK_CONSUMPTION_DELIVERED_c                         0x0432
#define g_PREVIOUS_WEEK_CONSUMPTION_RECEIVED_c                          0x0433
#define g_PREVIOUS_WEEK2_CONSUMPTION_DELIVERED_c                        0x0434
#define g_PREVIOUS_WEEK2_CONSUMPTION_RECEIVED_c                         0x0435
#define g_PREVIOUS_WEEK3_CONSUMPTION_DELIVERED_c                        0x0436
#define g_PREVIOUS_WEEK3_CONSUMPTION_RECEIVED_c                         0x0437
#define g_PREVIOUS_WEEK4_CONSUMPTION_DELIVERED_c                        0x0438
#define g_PREVIOUS_WEEK4_CONSUMPTION_RECEIVED_c                         0x0439
#define g_PREVIOUS_WEEK5_CONSUMPTION_DELIVERED_c                        0x043A
#define g_PREVIOUS_WEEK5_CONSUMPTION_RECEIVED_c                         0x043B
#define g_CURRENT_MONTH_CONSUMPTION_DELIVERED_c                         0x0440
#define g_CURRENT_MONTH_CONSUMPTION_RECEIVED_c                          0x0441
#define g_PREVIOUS_MONTH_CONSUMPTION_DELIVERED_c                        0x0442
#define g_PREVIOUS_MONTH_CONSUMPTION_RECEIVED_c                         0x0443
#define g_PREVIOUS_MONTH2_CONSUMPTION_DELIVERED_c                       0x0444
#define g_PREVIOUS_MONTH2_CONSUMPTION_RECEIVED_c                        0x0445
#define g_PREVIOUS_MONTH3_CONSUMPTION_DELIVERED_c                       0x0446
#define g_PREVIOUS_MONTH3_CONSUMPTION_RECEIVED_c                        0x0447
#define g_PREVIOUS_MONTH4_CONSUMPTION_DELIVERED_c                       0x0448
#define g_PREVIOUS_MONTH4_CONSUMPTION_RECEIVED_c                        0x0449
#define g_PREVIOUS_MONTH5_CONSUMPTION_DELIVERED_c                       0x044A
#define g_PREVIOUS_MONTH5_CONSUMPTION_RECEIVED_c                        0x044B
#define g_PREVIOUS_MONTH6_CONSUMPTION_DELIVERED_c                       0x044C
#define g_PREVIOUS_MONTH6_CONSUMPTION_RECEIVED_c                        0x044D
#define g_PREVIOUS_MONTH7_CONSUMPTION_DELIVERED_c                       0x044E
#define g_PREVIOUS_MONTH7_CONSUMPTION_RECEIVED_c                        0x044F
#define g_PREVIOUS_MONTH8_CONSUMPTION_DELIVERED_c                       0x0450
#define g_PREVIOUS_MONTH8_CONSUMPTION_RECEIVED_c                        0x0451
#define g_PREVIOUS_MONTH9_CONSUMPTION_DELIVERED_c                       0x0452
#define g_PREVIOUS_MONTH9_CONSUMPTION_RECEIVED_c                        0x0453
#define g_PREVIOUS_MONTH10_CONSUMPTION_DELIVERED_c                      0x0454
#define g_PREVIOUS_MONTH10_CONSUMPTION_RECEIVED_c                       0x0455
#define g_PREVIOUS_MONTH11_CONSUMPTION_DELIVERED_c                      0x0456
#define g_PREVIOUS_MONTH11_CONSUMPTION_RECEIVED_c                       0x0457
#define g_PREVIOUS_MONTH12_CONSUMPTION_DELIVERED_c                      0x0458
#define g_PREVIOUS_MONTH12_CONSUMPTION_RECEIVED_c                       0x0459
#define g_PREVIOUS_MONTH13_CONSUMPTION_DELIVERED_c                      0x045A
#define g_PREVIOUS_MONTH13_CONSUMPTION_RECEIVED_c                       0x045B
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */

/* Load Profile Configuration Attribute Set*/
#define g_MAX_NUMBER_OF_PERIODS_DELIVERED_c                             0x0500

/* Supply Limit Attribute Set */
#define g_CURRENT_DEMAND_DELIVERED_c                                    0x0600
#define g_DEMAND_LIMIT_c                                                0x0601
#define g_DEMAND_INTEGRATION_PERIOD_c                                   0x0602
#define g_NUMBER_OF_DEMAND_SUBINTERVALS_c                               0x0603
#if(g_SE_PROFILE_1_2_d == 1)
#define g_DEMAND_LIMIT_ARM_DURATION_IN_MINUTES_c                        0x0604
#define g_LOAD_LIMIT_SUPPLY_STATE_c                                     0x0605
#define g_LOAD_LIMIT_COUNTER_c                                          0x0606
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */

#if(g_SE_PROFILE_1_1_d == 1)	
/* Block Information Set (Delivered) */
#define g_CURRENT_NO_TIER_BLOCK1_SUMMATION_DELIVERED_c                  0x0700
#define g_CURRENT_NO_TIER_BLOCK2_SUMMATION_DELIVERED_c                  0x0701
#define g_CURRENT_NO_TIER_BLOCK3_SUMMATION_DELIVERED_c                  0x0702
#define g_CURRENT_NO_TIER_BLOCK4_SUMMATION_DELIVERED_c                  0x0703
#define g_CURRENT_NO_TIER_BLOCK5_SUMMATION_DELIVERED_c                  0x0704
#define g_CURRENT_NO_TIER_BLOCK6_SUMMATION_DELIVERED_c                  0x0705
#define g_CURRENT_NO_TIER_BLOCK7_SUMMATION_DELIVERED_c                  0x0706
#define g_CURRENT_NO_TIER_BLOCK8_SUMMATION_DELIVERED_c                  0x0707
#define g_CURRENT_NO_TIER_BLOCK9_SUMMATION_DELIVERED_c                  0x0708
#define g_CURRENT_NO_TIER_BLOCK10_SUMMATION_DELIVERED_c                 0x0709
#define g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_c                 0x070a
#define g_CURRENT_NO_TIER_BLOCK12_SUMMATION_DELIVERED_c                 0x070b
#define g_CURRENT_NO_TIER_BLOCK13_SUMMATION_DELIVERED_c                 0x070c
#define g_CURRENT_NO_TIER_BLOCK14_SUMMATION_DELIVERED_c                 0x070d
#define g_CURRENT_NO_TIER_BLOCK15_SUMMATION_DELIVERED_c                 0x070e
#define g_CURRENT_NO_TIER_BLOCK16_SUMMATION_DELIVERED_c                 0x070f

#define g_CURRENT_TIER1_BLOCK1_SUMMATION_DELIVERED_c                    0x0710
#define g_CURRENT_TIER1_BLOCK2_SUMMATION_DELIVERED_c                    0x0711
#define g_CURRENT_TIER1_BLOCK3_SUMMATION_DELIVERED_c                    0x0712
#define g_CURRENT_TIER1_BLOCK4_SUMMATION_DELIVERED_c                    0x0713
#define g_CURRENT_TIER1_BLOCK5_SUMMATION_DELIVERED_c                    0x0714
#define g_CURRENT_TIER1_BLOCK6_SUMMATION_DELIVERED_c                    0x0715
#define g_CURRENT_TIER1_BLOCK7_SUMMATION_DELIVERED_c                    0x0716
#define g_CURRENT_TIER1_BLOCK8_SUMMATION_DELIVERED_c                    0x0717
#define g_CURRENT_TIER1_BLOCK9_SUMMATION_DELIVERED_c                    0x0718
#define g_CURRENT_TIER1_BLOCK10_SUMMATION_DELIVERED_c                   0x0719
#define g_CURRENT_TIER1_BLOCK11_SUMMATION_DELIVERED_c                   0x071a
#define g_CURRENT_TIER1_BLOCK12_SUMMATION_DELIVERED_c                   0x071b
#define g_CURRENT_TIER1_BLOCK13_SUMMATION_DELIVERED_c                   0x071c
#define g_CURRENT_TIER1_BLOCK14_SUMMATION_DELIVERED_c                   0x071d
#define g_CURRENT_TIER1_BLOCK15_SUMMATION_DELIVERED_c                   0x071e
#define g_CURRENT_TIER1_BLOCK16_SUMMATION_DELIVERED_c                   0x071f

#define g_CURRENT_TIER2_BLOCK1_SUMMATION_DELIVERED_c                    0x0720
#define g_CURRENT_TIER2_BLOCK2_SUMMATION_DELIVERED_c                    0x0721
#define g_CURRENT_TIER2_BLOCK3_SUMMATION_DELIVERED_c                    0x0722
#define g_CURRENT_TIER2_BLOCK4_SUMMATION_DELIVERED_c                    0x0723
#define g_CURRENT_TIER2_BLOCK5_SUMMATION_DELIVERED_c                    0x0724
#define g_CURRENT_TIER2_BLOCK6_SUMMATION_DELIVERED_c                    0x0725
#define g_CURRENT_TIER2_BLOCK7_SUMMATION_DELIVERED_c                    0x0726
#define g_CURRENT_TIER2_BLOCK8_SUMMATION_DELIVERED_c                    0x0727
#define g_CURRENT_TIER2_BLOCK9_SUMMATION_DELIVERED_c                    0x0728
#define g_CURRENT_TIER2_BLOCK10_SUMMATION_DELIVERED_c                   0x0729
#define g_CURRENT_TIER2_BLOCK11_SUMMATION_DELIVERED_c                   0x072a
#define g_CURRENT_TIER2_BLOCK12_SUMMATION_DELIVERED_c                   0x072b
#define g_CURRENT_TIER2_BLOCK13_SUMMATION_DELIVERED_c                   0x072c
#define g_CURRENT_TIER2_BLOCK14_SUMMATION_DELIVERED_c                   0x072d
#define g_CURRENT_TIER2_BLOCK15_SUMMATION_DELIVERED_c                   0x072e
#define g_CURRENT_TIER2_BLOCK16_SUMMATION_DELIVERED_c                   0x072f

#define g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_c                    0x0730
#define g_CURRENT_TIER3_BLOCK2_SUMMATION_DELIVERED_c                    0x0731
#define g_CURRENT_TIER3_BLOCK3_SUMMATION_DELIVERED_c                    0x0732
#define g_CURRENT_TIER3_BLOCK4_SUMMATION_DELIVERED_c                    0x0733
#define g_CURRENT_TIER3_BLOCK5_SUMMATION_DELIVERED_c                    0x0734
#define g_CURRENT_TIER3_BLOCK6_SUMMATION_DELIVERED_c                    0x0735
#define g_CURRENT_TIER3_BLOCK7_SUMMATION_DELIVERED_c                    0x0736
#define g_CURRENT_TIER3_BLOCK8_SUMMATION_DELIVERED_c                    0x0737
#define g_CURRENT_TIER3_BLOCK9_SUMMATION_DELIVERED_c                    0x0738
#define g_CURRENT_TIER3_BLOCK10_SUMMATION_DELIVERED_c                   0x0739
#define g_CURRENT_TIER3_BLOCK11_SUMMATION_DELIVERED_c                   0x073a
#define g_CURRENT_TIER3_BLOCK12_SUMMATION_DELIVERED_c                   0x073b
#define g_CURRENT_TIER3_BLOCK13_SUMMATION_DELIVERED_c                   0x073c
#define g_CURRENT_TIER3_BLOCK14_SUMMATION_DELIVERED_c                   0x073d
#define g_CURRENT_TIER3_BLOCK15_SUMMATION_DELIVERED_c                   0x073e
#define g_CURRENT_TIER3_BLOCK16_SUMMATION_DELIVERED_c                   0x073f

#define g_CURRENT_TIER4_BLOCK1_SUMMATION_DELIVERED_c                    0x0740
#define g_CURRENT_TIER4_BLOCK2_SUMMATION_DELIVERED_c                    0x0741
#define g_CURRENT_TIER4_BLOCK3_SUMMATION_DELIVERED_c                    0x0742
#define g_CURRENT_TIER4_BLOCK4_SUMMATION_DELIVERED_c                    0x0743
#define g_CURRENT_TIER4_BLOCK5_SUMMATION_DELIVERED_c                    0x0744
#define g_CURRENT_TIER4_BLOCK6_SUMMATION_DELIVERED_c                    0x0745
#define g_CURRENT_TIER4_BLOCK7_SUMMATION_DELIVERED_c                    0x0746
#define g_CURRENT_TIER4_BLOCK8_SUMMATION_DELIVERED_c                    0x0747
#define g_CURRENT_TIER4_BLOCK9_SUMMATION_DELIVERED_c                    0x0748
#define g_CURRENT_TIER4_BLOCK10_SUMMATION_DELIVERED_c                   0x0749
#define g_CURRENT_TIER4_BLOCK11_SUMMATION_DELIVERED_c                   0x074a
#define g_CURRENT_TIER4_BLOCK12_SUMMATION_DELIVERED_c                   0x074b
#define g_CURRENT_TIER4_BLOCK13_SUMMATION_DELIVERED_c                   0x074c
#define g_CURRENT_TIER4_BLOCK14_SUMMATION_DELIVERED_c                   0x074d
#define g_CURRENT_TIER4_BLOCK15_SUMMATION_DELIVERED_c                   0x074e
#define g_CURRENT_TIER4_BLOCK16_SUMMATION_DELIVERED_c                   0x074f

#define g_CURRENT_TIER5_BLOCK1_SUMMATION_DELIVERED_c                    0x0750
#define g_CURRENT_TIER5_BLOCK2_SUMMATION_DELIVERED_c                    0x0751
#define g_CURRENT_TIER5_BLOCK3_SUMMATION_DELIVERED_c                    0x0752
#define g_CURRENT_TIER5_BLOCK4_SUMMATION_DELIVERED_c                    0x0753
#define g_CURRENT_TIER5_BLOCK5_SUMMATION_DELIVERED_c                    0x0754
#define g_CURRENT_TIER5_BLOCK6_SUMMATION_DELIVERED_c                    0x0755
#define g_CURRENT_TIER5_BLOCK7_SUMMATION_DELIVERED_c                    0x0756
#define g_CURRENT_TIER5_BLOCK8_SUMMATION_DELIVERED_c                    0x0757
#define g_CURRENT_TIER5_BLOCK9_SUMMATION_DELIVERED_c                    0x0758
#define g_CURRENT_TIER5_BLOCK10_SUMMATION_DELIVERED_c                   0x0759
#define g_CURRENT_TIER5_BLOCK11_SUMMATION_DELIVERED_c                   0x075a
#define g_CURRENT_TIER5_BLOCK12_SUMMATION_DELIVERED_c                   0x075b
#define g_CURRENT_TIER5_BLOCK13_SUMMATION_DELIVERED_c                   0x075c
#define g_CURRENT_TIER5_BLOCK14_SUMMATION_DELIVERED_c                   0x075d
#define g_CURRENT_TIER5_BLOCK15_SUMMATION_DELIVERED_c                   0x075e
#define g_CURRENT_TIER5_BLOCK16_SUMMATION_DELIVERED_c                   0x075f

#define g_CURRENT_TIER6_BLOCK1_SUMMATION_DELIVERED_c                    0x0760
#define g_CURRENT_TIER6_BLOCK2_SUMMATION_DELIVERED_c                    0x0761
#define g_CURRENT_TIER6_BLOCK3_SUMMATION_DELIVERED_c                    0x0762
#define g_CURRENT_TIER6_BLOCK4_SUMMATION_DELIVERED_c                    0x0763
#define g_CURRENT_TIER6_BLOCK5_SUMMATION_DELIVERED_c                    0x0764
#define g_CURRENT_TIER6_BLOCK6_SUMMATION_DELIVERED_c                    0x0765
#define g_CURRENT_TIER6_BLOCK7_SUMMATION_DELIVERED_c                    0x0766
#define g_CURRENT_TIER6_BLOCK8_SUMMATION_DELIVERED_c                    0x0767
#define g_CURRENT_TIER6_BLOCK9_SUMMATION_DELIVERED_c                    0x0768
#define g_CURRENT_TIER6_BLOCK10_SUMMATION_DELIVERED_c                   0x0769
#define g_CURRENT_TIER6_BLOCK11_SUMMATION_DELIVERED_c                   0x076a
#define g_CURRENT_TIER6_BLOCK12_SUMMATION_DELIVERED_c                   0x076b
#define g_CURRENT_TIER6_BLOCK13_SUMMATION_DELIVERED_c                   0x076c
#define g_CURRENT_TIER6_BLOCK14_SUMMATION_DELIVERED_c                   0x076d
#define g_CURRENT_TIER6_BLOCK15_SUMMATION_DELIVERED_c                   0x076e
#define g_CURRENT_TIER6_BLOCK16_SUMMATION_DELIVERED_c                   0x076f

#define g_CURRENT_TIER7_BLOCK1_SUMMATION_DELIVERED_c                    0x0770
#define g_CURRENT_TIER7_BLOCK2_SUMMATION_DELIVERED_c                    0x0771
#define g_CURRENT_TIER7_BLOCK3_SUMMATION_DELIVERED_c                    0x0772
#define g_CURRENT_TIER7_BLOCK4_SUMMATION_DELIVERED_c                    0x0773
#define g_CURRENT_TIER7_BLOCK5_SUMMATION_DELIVERED_c                    0x0774
#define g_CURRENT_TIER7_BLOCK6_SUMMATION_DELIVERED_c                    0x0775
#define g_CURRENT_TIER7_BLOCK7_SUMMATION_DELIVERED_c                    0x0776
#define g_CURRENT_TIER7_BLOCK8_SUMMATION_DELIVERED_c                    0x0777
#define g_CURRENT_TIER7_BLOCK9_SUMMATION_DELIVERED_c                    0x0778
#define g_CURRENT_TIER7_BLOCK10_SUMMATION_DELIVERED_c                   0x0779
#define g_CURRENT_TIER7_BLOCK11_SUMMATION_DELIVERED_c                   0x077a
#define g_CURRENT_TIER7_BLOCK12_SUMMATION_DELIVERED_c                   0x077b
#define g_CURRENT_TIER7_BLOCK13_SUMMATION_DELIVERED_c                   0x077c
#define g_CURRENT_TIER7_BLOCK14_SUMMATION_DELIVERED_c                   0x077d
#define g_CURRENT_TIER7_BLOCK15_SUMMATION_DELIVERED_c                   0x077e
#define g_CURRENT_TIER7_BLOCK16_SUMMATION_DELIVERED_c                   0x077f

#define g_CURRENT_TIER8_BLOCK1_SUMMATION_DELIVERED_c                    0x0780
#define g_CURRENT_TIER8_BLOCK2_SUMMATION_DELIVERED_c                    0x0781
#define g_CURRENT_TIER8_BLOCK3_SUMMATION_DELIVERED_c                    0x0782
#define g_CURRENT_TIER8_BLOCK4_SUMMATION_DELIVERED_c                    0x0783
#define g_CURRENT_TIER8_BLOCK5_SUMMATION_DELIVERED_c                    0x0784
#define g_CURRENT_TIER8_BLOCK6_SUMMATION_DELIVERED_c                    0x0785
#define g_CURRENT_TIER8_BLOCK7_SUMMATION_DELIVERED_c                    0x0786
#define g_CURRENT_TIER8_BLOCK8_SUMMATION_DELIVERED_c                    0x0787
#define g_CURRENT_TIER8_BLOCK9_SUMMATION_DELIVERED_c                    0x0788
#define g_CURRENT_TIER8_BLOCK10_SUMMATION_DELIVERED_c                   0x0789
#define g_CURRENT_TIER8_BLOCK11_SUMMATION_DELIVERED_c                   0x078a
#define g_CURRENT_TIER8_BLOCK12_SUMMATION_DELIVERED_c                   0x078b
#define g_CURRENT_TIER8_BLOCK13_SUMMATION_DELIVERED_c                   0x078c
#define g_CURRENT_TIER8_BLOCK14_SUMMATION_DELIVERED_c                   0x078d
#define g_CURRENT_TIER8_BLOCK15_SUMMATION_DELIVERED_c                   0x078e
#define g_CURRENT_TIER8_BLOCK16_SUMMATION_DELIVERED_c                   0x078f

#define g_CURRENT_TIER9_BLOCK1_SUMMATION_DELIVERED_c                    0x0790
#define g_CURRENT_TIER9_BLOCK2_SUMMATION_DELIVERED_c                    0x0791
#define g_CURRENT_TIER9_BLOCK3_SUMMATION_DELIVERED_c                    0x0792
#define g_CURRENT_TIER9_BLOCK4_SUMMATION_DELIVERED_c                    0x0793
#define g_CURRENT_TIER9_BLOCK5_SUMMATION_DELIVERED_c                    0x0794
#define g_CURRENT_TIER9_BLOCK6_SUMMATION_DELIVERED_c                    0x0795
#define g_CURRENT_TIER9_BLOCK7_SUMMATION_DELIVERED_c                    0x0796
#define g_CURRENT_TIER9_BLOCK8_SUMMATION_DELIVERED_c                    0x0797
#define g_CURRENT_TIER9_BLOCK9_SUMMATION_DELIVERED_c                    0x0798
#define g_CURRENT_TIER9_BLOCK10_SUMMATION_DELIVERED_c                   0x0799
#define g_CURRENT_TIER9_BLOCK11_SUMMATION_DELIVERED_c                   0x079a
#define g_CURRENT_TIER9_BLOCK12_SUMMATION_DELIVERED_c                   0x079b
#define g_CURRENT_TIER9_BLOCK13_SUMMATION_DELIVERED_c                   0x079c
#define g_CURRENT_TIER9_BLOCK14_SUMMATION_DELIVERED_c                   0x079d
#define g_CURRENT_TIER9_BLOCK15_SUMMATION_DELIVERED_c                   0x079e
#define g_CURRENT_TIER9_BLOCK16_SUMMATION_DELIVERED_c                   0x079f

#define g_CURRENT_TIER10_BLOCK1_SUMMATION_DELIVERED_c                    0x07a0
#define g_CURRENT_TIER10_BLOCK2_SUMMATION_DELIVERED_c                    0x07a1
#define g_CURRENT_TIER10_BLOCK3_SUMMATION_DELIVERED_c                    0x07a2
#define g_CURRENT_TIER10_BLOCK4_SUMMATION_DELIVERED_c                    0x07a3
#define g_CURRENT_TIER10_BLOCK5_SUMMATION_DELIVERED_c                    0x07a4
#define g_CURRENT_TIER10_BLOCK6_SUMMATION_DELIVERED_c                    0x07a5
#define g_CURRENT_TIER10_BLOCK7_SUMMATION_DELIVERED_c                    0x07a6
#define g_CURRENT_TIER10_BLOCK8_SUMMATION_DELIVERED_c                    0x07a7
#define g_CURRENT_TIER10_BLOCK9_SUMMATION_DELIVERED_c                    0x07a8
#define g_CURRENT_TIER10_BLOCK10_SUMMATION_DELIVERED_c                   0x07a9
#define g_CURRENT_TIER10_BLOCK11_SUMMATION_DELIVERED_c                   0x07aa
#define g_CURRENT_TIER10_BLOCK12_SUMMATION_DELIVERED_c                   0x07ab
#define g_CURRENT_TIER10_BLOCK13_SUMMATION_DELIVERED_c                   0x07ac
#define g_CURRENT_TIER10_BLOCK14_SUMMATION_DELIVERED_c                   0x07ad
#define g_CURRENT_TIER10_BLOCK15_SUMMATION_DELIVERED_c                   0x07ae
#define g_CURRENT_TIER10_BLOCK16_SUMMATION_DELIVERED_c                   0x07af

#define g_CURRENT_TIER11_BLOCK1_SUMMATION_DELIVERED_c                    0x07b0
#define g_CURRENT_TIER11_BLOCK2_SUMMATION_DELIVERED_c                    0x07b1
#define g_CURRENT_TIER11_BLOCK3_SUMMATION_DELIVERED_c                    0x07b2
#define g_CURRENT_TIER11_BLOCK4_SUMMATION_DELIVERED_c                    0x07b3
#define g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_c                    0x07b4
#define g_CURRENT_TIER11_BLOCK6_SUMMATION_DELIVERED_c                    0x07b5
#define g_CURRENT_TIER11_BLOCK7_SUMMATION_DELIVERED_c                    0x07b6
#define g_CURRENT_TIER11_BLOCK8_SUMMATION_DELIVERED_c                    0x07b7
#define g_CURRENT_TIER11_BLOCK9_SUMMATION_DELIVERED_c                    0x07b8
#define g_CURRENT_TIER11_BLOCK10_SUMMATION_DELIVERED_c                   0x07b9
#define g_CURRENT_TIER11_BLOCK11_SUMMATION_DELIVERED_c                   0x07ba
#define g_CURRENT_TIER11_BLOCK12_SUMMATION_DELIVERED_c                   0x07bb
#define g_CURRENT_TIER11_BLOCK13_SUMMATION_DELIVERED_c                   0x07bc
#define g_CURRENT_TIER11_BLOCK14_SUMMATION_DELIVERED_c                   0x07bd
#define g_CURRENT_TIER11_BLOCK15_SUMMATION_DELIVERED_c                   0x07be
#define g_CURRENT_TIER11_BLOCK16_SUMMATION_DELIVERED_c                   0x07bf

#define g_CURRENT_TIER12_BLOCK1_SUMMATION_DELIVERED_c                    0x07c0
#define g_CURRENT_TIER12_BLOCK2_SUMMATION_DELIVERED_c                    0x07c1
#define g_CURRENT_TIER12_BLOCK3_SUMMATION_DELIVERED_c                    0x07c2
#define g_CURRENT_TIER12_BLOCK4_SUMMATION_DELIVERED_c                    0x07c3
#define g_CURRENT_TIER12_BLOCK5_SUMMATION_DELIVERED_c                    0x07c4
#define g_CURRENT_TIER12_BLOCK6_SUMMATION_DELIVERED_c                    0x07c5
#define g_CURRENT_TIER12_BLOCK7_SUMMATION_DELIVERED_c                    0x07c6
#define g_CURRENT_TIER12_BLOCK8_SUMMATION_DELIVERED_c                    0x07c7
#define g_CURRENT_TIER12_BLOCK9_SUMMATION_DELIVERED_c                    0x07c8
#define g_CURRENT_TIER12_BLOCK10_SUMMATION_DELIVERED_c                   0x07c9
#define g_CURRENT_TIER12_BLOCK11_SUMMATION_DELIVERED_c                   0x07ca
#define g_CURRENT_TIER12_BLOCK12_SUMMATION_DELIVERED_c                   0x07cb
#define g_CURRENT_TIER12_BLOCK13_SUMMATION_DELIVERED_c                   0x07cc
#define g_CURRENT_TIER12_BLOCK14_SUMMATION_DELIVERED_c                   0x07cd
#define g_CURRENT_TIER12_BLOCK15_SUMMATION_DELIVERED_c                   0x07ce
#define g_CURRENT_TIER12_BLOCK16_SUMMATION_DELIVERED_c                   0x07cf

#define g_CURRENT_TIER13_BLOCK1_SUMMATION_DELIVERED_c                    0x07d0
#define g_CURRENT_TIER13_BLOCK2_SUMMATION_DELIVERED_c                    0x07d1
#define g_CURRENT_TIER13_BLOCK3_SUMMATION_DELIVERED_c                    0x07d2
#define g_CURRENT_TIER13_BLOCK4_SUMMATION_DELIVERED_c                    0x07d3
#define g_CURRENT_TIER13_BLOCK5_SUMMATION_DELIVERED_c                    0x07d4
#define g_CURRENT_TIER13_BLOCK6_SUMMATION_DELIVERED_c                    0x07d5
#define g_CURRENT_TIER13_BLOCK7_SUMMATION_DELIVERED_c                    0x07d6
#define g_CURRENT_TIER13_BLOCK8_SUMMATION_DELIVERED_c                    0x07d7
#define g_CURRENT_TIER13_BLOCK9_SUMMATION_DELIVERED_c                    0x07d8
#define g_CURRENT_TIER13_BLOCK10_SUMMATION_DELIVERED_c                   0x07d9
#define g_CURRENT_TIER13_BLOCK11_SUMMATION_DELIVERED_c                   0x07da
#define g_CURRENT_TIER13_BLOCK12_SUMMATION_DELIVERED_c                   0x07db
#define g_CURRENT_TIER13_BLOCK13_SUMMATION_DELIVERED_c                   0x07dc
#define g_CURRENT_TIER13_BLOCK14_SUMMATION_DELIVERED_c                   0x07dd
#define g_CURRENT_TIER13_BLOCK15_SUMMATION_DELIVERED_c                   0x07de
#define g_CURRENT_TIER13_BLOCK16_SUMMATION_DELIVERED_c                   0x07df

#define g_CURRENT_TIER14_BLOCK1_SUMMATION_DELIVERED_c                    0x07e0
#define g_CURRENT_TIER14_BLOCK2_SUMMATION_DELIVERED_c                    0x07e1
#define g_CURRENT_TIER14_BLOCK3_SUMMATION_DELIVERED_c                    0x07e2
#define g_CURRENT_TIER14_BLOCK4_SUMMATION_DELIVERED_c                    0x07e3
#define g_CURRENT_TIER14_BLOCK5_SUMMATION_DELIVERED_c                    0x07e4
#define g_CURRENT_TIER14_BLOCK6_SUMMATION_DELIVERED_c                    0x07e5
#define g_CURRENT_TIER14_BLOCK7_SUMMATION_DELIVERED_c                    0x07e6
#define g_CURRENT_TIER14_BLOCK8_SUMMATION_DELIVERED_c                    0x07e7
#define g_CURRENT_TIER14_BLOCK9_SUMMATION_DELIVERED_c                    0x07e8
#define g_CURRENT_TIER14_BLOCK10_SUMMATION_DELIVERED_c                   0x07e9
#define g_CURRENT_TIER14_BLOCK11_SUMMATION_DELIVERED_c                   0x07ea
#define g_CURRENT_TIER14_BLOCK12_SUMMATION_DELIVERED_c                   0x07eb
#define g_CURRENT_TIER14_BLOCK13_SUMMATION_DELIVERED_c                   0x07ec
#define g_CURRENT_TIER14_BLOCK14_SUMMATION_DELIVERED_c                   0x07ed
#define g_CURRENT_TIER14_BLOCK15_SUMMATION_DELIVERED_c                   0x07ee
#define g_CURRENT_TIER14_BLOCK16_SUMMATION_DELIVERED_c                   0x07ef

#define g_CURRENT_TIER15_BLOCK1_SUMMATION_DELIVERED_c                    0x07f0
#define g_CURRENT_TIER15_BLOCK2_SUMMATION_DELIVERED_c                    0x07f1
#define g_CURRENT_TIER15_BLOCK3_SUMMATION_DELIVERED_c                    0x07f2
#define g_CURRENT_TIER15_BLOCK4_SUMMATION_DELIVERED_c                    0x07f3
#define g_CURRENT_TIER15_BLOCK5_SUMMATION_DELIVERED_c                    0x07f4
#define g_CURRENT_TIER15_BLOCK6_SUMMATION_DELIVERED_c                    0x07f5
#define g_CURRENT_TIER15_BLOCK7_SUMMATION_DELIVERED_c                    0x07f6
#define g_CURRENT_TIER15_BLOCK8_SUMMATION_DELIVERED_c                    0x07f7
#define g_CURRENT_TIER15_BLOCK9_SUMMATION_DELIVERED_c                    0x07f8
#define g_CURRENT_TIER15_BLOCK10_SUMMATION_DELIVERED_c                   0x07f9
#define g_CURRENT_TIER15_BLOCK11_SUMMATION_DELIVERED_c                   0x07fa
#define g_CURRENT_TIER15_BLOCK12_SUMMATION_DELIVERED_c                   0x07fb
#define g_CURRENT_TIER15_BLOCK13_SUMMATION_DELIVERED_c                   0x07fc
#define g_CURRENT_TIER15_BLOCK14_SUMMATION_DELIVERED_c                   0x07fd
#define g_CURRENT_TIER15_BLOCK15_SUMMATION_DELIVERED_c                   0x07fe
#define g_CURRENT_TIER15_BLOCK16_SUMMATION_DELIVERED_c                   0x07ff
#endif  /* End of g_SE_PROFILE_1_1_d == 1 */
#if(g_SE_PROFILE_1_2_d == 1)	
/* Block Information Set (Received) */
#define g_CURRENT_NO_TIER_BLOCK1_SUMMATION_RECEIVED_c                  0x0900
#define g_CURRENT_NO_TIER_BLOCK2_SUMMATION_RECEIVED_c                  0x0901
#define g_CURRENT_NO_TIER_BLOCK3_SUMMATION_RECEIVED_c                  0x0902
#define g_CURRENT_NO_TIER_BLOCK4_SUMMATION_RECEIVED_c                  0x0903
#define g_CURRENT_NO_TIER_BLOCK5_SUMMATION_RECEIVED_c                  0x0904
#define g_CURRENT_NO_TIER_BLOCK6_SUMMATION_RECEIVED_c                  0x0905
#define g_CURRENT_NO_TIER_BLOCK7_SUMMATION_RECEIVED_c                  0x0906
#define g_CURRENT_NO_TIER_BLOCK8_SUMMATION_RECEIVED_c                  0x0907
#define g_CURRENT_NO_TIER_BLOCK9_SUMMATION_RECEIVED_c                  0x0908
#define g_CURRENT_NO_TIER_BLOCK10_SUMMATION_RECEIVED_c                 0x0909
#define g_CURRENT_NO_TIER_BLOCK11_SUMMATION_RECEIVED_c                 0x090a
#define g_CURRENT_NO_TIER_BLOCK12_SUMMATION_RECEIVED_c                 0x090b
#define g_CURRENT_NO_TIER_BLOCK13_SUMMATION_RECEIVED_c                 0x090c
#define g_CURRENT_NO_TIER_BLOCK14_SUMMATION_RECEIVED_c                 0x090d
#define g_CURRENT_NO_TIER_BLOCK15_SUMMATION_RECEIVED_c                 0x090e
#define g_CURRENT_NO_TIER_BLOCK16_SUMMATION_RECEIVED_c                 0x090f

#define g_CURRENT_TIER1_BLOCK1_SUMMATION_RECEIVED_c                    0x0910
#define g_CURRENT_TIER1_BLOCK2_SUMMATION_RECEIVED_c                    0x0911
#define g_CURRENT_TIER1_BLOCK3_SUMMATION_RECEIVED_c                    0x0912
#define g_CURRENT_TIER1_BLOCK4_SUMMATION_RECEIVED_c                    0x0913
#define g_CURRENT_TIER1_BLOCK5_SUMMATION_RECEIVED_c                    0x0914
#define g_CURRENT_TIER1_BLOCK6_SUMMATION_RECEIVED_c                    0x0915
#define g_CURRENT_TIER1_BLOCK7_SUMMATION_RECEIVED_c                    0x0916
#define g_CURRENT_TIER1_BLOCK8_SUMMATION_RECEIVED_c                    0x0917
#define g_CURRENT_TIER1_BLOCK9_SUMMATION_RECEIVED_c                    0x0918
#define g_CURRENT_TIER1_BLOCK10_SUMMATION_RECEIVED_c                   0x0919
#define g_CURRENT_TIER1_BLOCK11_SUMMATION_RECEIVED_c                   0x091a
#define g_CURRENT_TIER1_BLOCK12_SUMMATION_RECEIVED_c                   0x091b
#define g_CURRENT_TIER1_BLOCK13_SUMMATION_RECEIVED_c                   0x091c
#define g_CURRENT_TIER1_BLOCK14_SUMMATION_RECEIVED_c                   0x091d
#define g_CURRENT_TIER1_BLOCK15_SUMMATION_RECEIVED_c                   0x091e
#define g_CURRENT_TIER1_BLOCK16_SUMMATION_RECEIVED_c                   0x091f

#define g_CURRENT_TIER2_BLOCK1_SUMMATION_RECEIVED_c                    0x0920
#define g_CURRENT_TIER2_BLOCK2_SUMMATION_RECEIVED_c                    0x0921
#define g_CURRENT_TIER2_BLOCK3_SUMMATION_RECEIVED_c                    0x0922
#define g_CURRENT_TIER2_BLOCK4_SUMMATION_RECEIVED_c                    0x0923
#define g_CURRENT_TIER2_BLOCK5_SUMMATION_RECEIVED_c                    0x0924
#define g_CURRENT_TIER2_BLOCK6_SUMMATION_RECEIVED_c                    0x0925
#define g_CURRENT_TIER2_BLOCK7_SUMMATION_RECEIVED_c                    0x0926
#define g_CURRENT_TIER2_BLOCK8_SUMMATION_RECEIVED_c                    0x0927
#define g_CURRENT_TIER2_BLOCK9_SUMMATION_RECEIVED_c                    0x0928
#define g_CURRENT_TIER2_BLOCK10_SUMMATION_RECEIVED_c                   0x0929
#define g_CURRENT_TIER2_BLOCK11_SUMMATION_RECEIVED_c                   0x092a
#define g_CURRENT_TIER2_BLOCK12_SUMMATION_RECEIVED_c                   0x092b
#define g_CURRENT_TIER2_BLOCK13_SUMMATION_RECEIVED_c                   0x092c
#define g_CURRENT_TIER2_BLOCK14_SUMMATION_RECEIVED_c                   0x092d
#define g_CURRENT_TIER2_BLOCK15_SUMMATION_RECEIVED_c                   0x092e
#define g_CURRENT_TIER2_BLOCK16_SUMMATION_RECEIVED_c                   0x092f

#define g_CURRENT_TIER3_BLOCK1_SUMMATION_RECEIVED_c                    0x0930
#define g_CURRENT_TIER3_BLOCK2_SUMMATION_RECEIVED_c                    0x0931
#define g_CURRENT_TIER3_BLOCK3_SUMMATION_RECEIVED_c                    0x0932
#define g_CURRENT_TIER3_BLOCK4_SUMMATION_RECEIVED_c                    0x0933
#define g_CURRENT_TIER3_BLOCK5_SUMMATION_RECEIVED_c                    0x0934
#define g_CURRENT_TIER3_BLOCK6_SUMMATION_RECEIVED_c                    0x0935
#define g_CURRENT_TIER3_BLOCK7_SUMMATION_RECEIVED_c                    0x0936
#define g_CURRENT_TIER3_BLOCK8_SUMMATION_RECEIVED_c                    0x0937
#define g_CURRENT_TIER3_BLOCK9_SUMMATION_RECEIVED_c                    0x0938
#define g_CURRENT_TIER3_BLOCK10_SUMMATION_RECEIVED_c                   0x0939
#define g_CURRENT_TIER3_BLOCK11_SUMMATION_RECEIVED_c                   0x093a
#define g_CURRENT_TIER3_BLOCK12_SUMMATION_RECEIVED_c                   0x093b
#define g_CURRENT_TIER3_BLOCK13_SUMMATION_RECEIVED_c                   0x093c
#define g_CURRENT_TIER3_BLOCK14_SUMMATION_RECEIVED_c                   0x093d
#define g_CURRENT_TIER3_BLOCK15_SUMMATION_RECEIVED_c                   0x093e
#define g_CURRENT_TIER3_BLOCK16_SUMMATION_RECEIVED_c                   0x093f

#define g_CURRENT_TIER4_BLOCK1_SUMMATION_RECEIVED_c                    0x0940
#define g_CURRENT_TIER4_BLOCK2_SUMMATION_RECEIVED_c                    0x0941
#define g_CURRENT_TIER4_BLOCK3_SUMMATION_RECEIVED_c                    0x0942
#define g_CURRENT_TIER4_BLOCK4_SUMMATION_RECEIVED_c                    0x0943
#define g_CURRENT_TIER4_BLOCK5_SUMMATION_RECEIVED_c                    0x0944
#define g_CURRENT_TIER4_BLOCK6_SUMMATION_RECEIVED_c                    0x0945
#define g_CURRENT_TIER4_BLOCK7_SUMMATION_RECEIVED_c                    0x0946
#define g_CURRENT_TIER4_BLOCK8_SUMMATION_RECEIVED_c                    0x0947
#define g_CURRENT_TIER4_BLOCK9_SUMMATION_RECEIVED_c                    0x0948
#define g_CURRENT_TIER4_BLOCK10_SUMMATION_RECEIVED_c                   0x0949
#define g_CURRENT_TIER4_BLOCK11_SUMMATION_RECEIVED_c                   0x094a
#define g_CURRENT_TIER4_BLOCK12_SUMMATION_RECEIVED_c                   0x094b
#define g_CURRENT_TIER4_BLOCK13_SUMMATION_RECEIVED_c                   0x094c
#define g_CURRENT_TIER4_BLOCK14_SUMMATION_RECEIVED_c                   0x094d
#define g_CURRENT_TIER4_BLOCK15_SUMMATION_RECEIVED_c                   0x094e
#define g_CURRENT_TIER4_BLOCK16_SUMMATION_RECEIVED_c                   0x094f

#define g_CURRENT_TIER5_BLOCK1_SUMMATION_RECEIVED_c                    0x0950
#define g_CURRENT_TIER5_BLOCK2_SUMMATION_RECEIVED_c                    0x0951
#define g_CURRENT_TIER5_BLOCK3_SUMMATION_RECEIVED_c                    0x0952
#define g_CURRENT_TIER5_BLOCK4_SUMMATION_RECEIVED_c                    0x0953
#define g_CURRENT_TIER5_BLOCK5_SUMMATION_RECEIVED_c                    0x0954
#define g_CURRENT_TIER5_BLOCK6_SUMMATION_RECEIVED_c                    0x0955
#define g_CURRENT_TIER5_BLOCK7_SUMMATION_RECEIVED_c                    0x0956
#define g_CURRENT_TIER5_BLOCK8_SUMMATION_RECEIVED_c                    0x0957
#define g_CURRENT_TIER5_BLOCK9_SUMMATION_RECEIVED_c                    0x0958
#define g_CURRENT_TIER5_BLOCK10_SUMMATION_RECEIVED_c                   0x0959
#define g_CURRENT_TIER5_BLOCK11_SUMMATION_RECEIVED_c                   0x095a
#define g_CURRENT_TIER5_BLOCK12_SUMMATION_RECEIVED_c                   0x095b
#define g_CURRENT_TIER5_BLOCK13_SUMMATION_RECEIVED_c                   0x095c
#define g_CURRENT_TIER5_BLOCK14_SUMMATION_RECEIVED_c                   0x095d
#define g_CURRENT_TIER5_BLOCK15_SUMMATION_RECEIVED_c                   0x095e
#define g_CURRENT_TIER5_BLOCK16_SUMMATION_RECEIVED_c                   0x095f

#define g_CURRENT_TIER6_BLOCK1_SUMMATION_RECEIVED_c                    0x0960
#define g_CURRENT_TIER6_BLOCK2_SUMMATION_RECEIVED_c                    0x0961
#define g_CURRENT_TIER6_BLOCK3_SUMMATION_RECEIVED_c                    0x0962
#define g_CURRENT_TIER6_BLOCK4_SUMMATION_RECEIVED_c                    0x0963
#define g_CURRENT_TIER6_BLOCK5_SUMMATION_RECEIVED_c                    0x0964
#define g_CURRENT_TIER6_BLOCK6_SUMMATION_RECEIVED_c                    0x0965
#define g_CURRENT_TIER6_BLOCK7_SUMMATION_RECEIVED_c                    0x0966
#define g_CURRENT_TIER6_BLOCK8_SUMMATION_RECEIVED_c                    0x0967
#define g_CURRENT_TIER6_BLOCK9_SUMMATION_RECEIVED_c                    0x0968
#define g_CURRENT_TIER6_BLOCK10_SUMMATION_RECEIVED_c                   0x0969
#define g_CURRENT_TIER6_BLOCK11_SUMMATION_RECEIVED_c                   0x096a
#define g_CURRENT_TIER6_BLOCK12_SUMMATION_RECEIVED_c                   0x096b
#define g_CURRENT_TIER6_BLOCK13_SUMMATION_RECEIVED_c                   0x096c
#define g_CURRENT_TIER6_BLOCK14_SUMMATION_RECEIVED_c                   0x096d
#define g_CURRENT_TIER6_BLOCK15_SUMMATION_RECEIVED_c                   0x096e
#define g_CURRENT_TIER6_BLOCK16_SUMMATION_RECEIVED_c                   0x096f

#define g_CURRENT_TIER7_BLOCK1_SUMMATION_RECEIVED_c                    0x0970
#define g_CURRENT_TIER7_BLOCK2_SUMMATION_RECEIVED_c                    0x0971
#define g_CURRENT_TIER7_BLOCK3_SUMMATION_RECEIVED_c                    0x0972
#define g_CURRENT_TIER7_BLOCK4_SUMMATION_RECEIVED_c                    0x0973
#define g_CURRENT_TIER7_BLOCK5_SUMMATION_RECEIVED_c                    0x0974
#define g_CURRENT_TIER7_BLOCK6_SUMMATION_RECEIVED_c                    0x0975
#define g_CURRENT_TIER7_BLOCK7_SUMMATION_RECEIVED_c                    0x0976
#define g_CURRENT_TIER7_BLOCK8_SUMMATION_RECEIVED_c                    0x0977
#define g_CURRENT_TIER7_BLOCK9_SUMMATION_RECEIVED_c                    0x0978
#define g_CURRENT_TIER7_BLOCK10_SUMMATION_RECEIVED_c                   0x0979
#define g_CURRENT_TIER7_BLOCK11_SUMMATION_RECEIVED_c                   0x097a
#define g_CURRENT_TIER7_BLOCK12_SUMMATION_RECEIVED_c                   0x097b
#define g_CURRENT_TIER7_BLOCK13_SUMMATION_RECEIVED_c                   0x097c
#define g_CURRENT_TIER7_BLOCK14_SUMMATION_RECEIVED_c                   0x097d
#define g_CURRENT_TIER7_BLOCK15_SUMMATION_RECEIVED_c                   0x097e
#define g_CURRENT_TIER7_BLOCK16_SUMMATION_RECEIVED_c                   0x097f

#define g_CURRENT_TIER8_BLOCK1_SUMMATION_RECEIVED_c                    0x0980
#define g_CURRENT_TIER8_BLOCK2_SUMMATION_RECEIVED_c                    0x0981
#define g_CURRENT_TIER8_BLOCK3_SUMMATION_RECEIVED_c                    0x0982
#define g_CURRENT_TIER8_BLOCK4_SUMMATION_RECEIVED_c                    0x0983
#define g_CURRENT_TIER8_BLOCK5_SUMMATION_RECEIVED_c                    0x0984
#define g_CURRENT_TIER8_BLOCK6_SUMMATION_RECEIVED_c                    0x0985
#define g_CURRENT_TIER8_BLOCK7_SUMMATION_RECEIVED_c                    0x0986
#define g_CURRENT_TIER8_BLOCK8_SUMMATION_RECEIVED_c                    0x0987
#define g_CURRENT_TIER8_BLOCK9_SUMMATION_RECEIVED_c                    0x0988
#define g_CURRENT_TIER8_BLOCK10_SUMMATION_RECEIVED_c                   0x0989
#define g_CURRENT_TIER8_BLOCK11_SUMMATION_RECEIVED_c                   0x098a
#define g_CURRENT_TIER8_BLOCK12_SUMMATION_RECEIVED_c                   0x098b
#define g_CURRENT_TIER8_BLOCK13_SUMMATION_RECEIVED_c                   0x098c
#define g_CURRENT_TIER8_BLOCK14_SUMMATION_RECEIVED_c                   0x098d
#define g_CURRENT_TIER8_BLOCK15_SUMMATION_RECEIVED_c                   0x098e
#define g_CURRENT_TIER8_BLOCK16_SUMMATION_RECEIVED_c                   0x098f

#define g_CURRENT_TIER9_BLOCK1_SUMMATION_RECEIVED_c                    0x0990
#define g_CURRENT_TIER9_BLOCK2_SUMMATION_RECEIVED_c                    0x0991
#define g_CURRENT_TIER9_BLOCK3_SUMMATION_RECEIVED_c                    0x0992
#define g_CURRENT_TIER9_BLOCK4_SUMMATION_RECEIVED_c                    0x0993
#define g_CURRENT_TIER9_BLOCK5_SUMMATION_RECEIVED_c                    0x0994
#define g_CURRENT_TIER9_BLOCK6_SUMMATION_RECEIVED_c                    0x0995
#define g_CURRENT_TIER9_BLOCK7_SUMMATION_RECEIVED_c                    0x0996
#define g_CURRENT_TIER9_BLOCK8_SUMMATION_RECEIVED_c                    0x0997
#define g_CURRENT_TIER9_BLOCK9_SUMMATION_RECEIVED_c                    0x0998
#define g_CURRENT_TIER9_BLOCK10_SUMMATION_RECEIVED_c                   0x0999
#define g_CURRENT_TIER9_BLOCK11_SUMMATION_RECEIVED_c                   0x099a
#define g_CURRENT_TIER9_BLOCK12_SUMMATION_RECEIVED_c                   0x099b
#define g_CURRENT_TIER9_BLOCK13_SUMMATION_RECEIVED_c                   0x099c
#define g_CURRENT_TIER9_BLOCK14_SUMMATION_RECEIVED_c                   0x099d
#define g_CURRENT_TIER9_BLOCK15_SUMMATION_RECEIVED_c                   0x099e
#define g_CURRENT_TIER9_BLOCK16_SUMMATION_RECEIVED_c                   0x099f

#define g_CURRENT_TIER10_BLOCK1_SUMMATION_RECEIVED_c                   0x09a0
#define g_CURRENT_TIER10_BLOCK2_SUMMATION_RECEIVED_c                   0x09a1
#define g_CURRENT_TIER10_BLOCK3_SUMMATION_RECEIVED_c                   0x09a2
#define g_CURRENT_TIER10_BLOCK4_SUMMATION_RECEIVED_c                   0x09a3
#define g_CURRENT_TIER10_BLOCK5_SUMMATION_RECEIVED_c                   0x09a4
#define g_CURRENT_TIER10_BLOCK6_SUMMATION_RECEIVED_c                   0x09a5
#define g_CURRENT_TIER10_BLOCK7_SUMMATION_RECEIVED_c                   0x09a6
#define g_CURRENT_TIER10_BLOCK8_SUMMATION_RECEIVED_c                   0x09a7
#define g_CURRENT_TIER10_BLOCK9_SUMMATION_RECEIVED_c                   0x09a8
#define g_CURRENT_TIER10_BLOCK10_SUMMATION_RECEIVED_c                  0x09a9
#define g_CURRENT_TIER10_BLOCK11_SUMMATION_RECEIVED_c                  0x09aa
#define g_CURRENT_TIER10_BLOCK12_SUMMATION_RECEIVED_c                  0x09ab
#define g_CURRENT_TIER10_BLOCK13_SUMMATION_RECEIVED_c                  0x09ac
#define g_CURRENT_TIER10_BLOCK14_SUMMATION_RECEIVED_c                  0x09ad
#define g_CURRENT_TIER10_BLOCK15_SUMMATION_RECEIVED_c                  0x09ae
#define g_CURRENT_TIER10_BLOCK16_SUMMATION_RECEIVED_c                  0x09af

#define g_CURRENT_TIER11_BLOCK1_SUMMATION_RECEIVED_c                   0x09b0
#define g_CURRENT_TIER11_BLOCK2_SUMMATION_RECEIVED_c                   0x09b1
#define g_CURRENT_TIER11_BLOCK3_SUMMATION_RECEIVED_c                   0x09b2
#define g_CURRENT_TIER11_BLOCK4_SUMMATION_RECEIVED_c                   0x09b3
#define g_CURRENT_TIER11_BLOCK5_SUMMATION_RECEIVED_c                   0x09b4
#define g_CURRENT_TIER11_BLOCK6_SUMMATION_RECEIVED_c                   0x09b5
#define g_CURRENT_TIER11_BLOCK7_SUMMATION_RECEIVED_c                   0x09b6
#define g_CURRENT_TIER11_BLOCK8_SUMMATION_RECEIVED_c                   0x09b7
#define g_CURRENT_TIER11_BLOCK9_SUMMATION_RECEIVED_c                   0x09b8
#define g_CURRENT_TIER11_BLOCK10_SUMMATION_RECEIVED_c                  0x09b9
#define g_CURRENT_TIER11_BLOCK11_SUMMATION_RECEIVED_c                  0x09ba
#define g_CURRENT_TIER11_BLOCK12_SUMMATION_RECEIVED_c                  0x09bb
#define g_CURRENT_TIER11_BLOCK13_SUMMATION_RECEIVED_c                  0x09bc
#define g_CURRENT_TIER11_BLOCK14_SUMMATION_RECEIVED_c                  0x09bd
#define g_CURRENT_TIER11_BLOCK15_SUMMATION_RECEIVED_c                  0x09be
#define g_CURRENT_TIER11_BLOCK16_SUMMATION_RECEIVED_c                  0x09bf

#define g_CURRENT_TIER12_BLOCK1_SUMMATION_RECEIVED_c                   0x09c0
#define g_CURRENT_TIER12_BLOCK2_SUMMATION_RECEIVED_c                   0x09c1
#define g_CURRENT_TIER12_BLOCK3_SUMMATION_RECEIVED_c                   0x09c2
#define g_CURRENT_TIER12_BLOCK4_SUMMATION_RECEIVED_c                   0x09c3
#define g_CURRENT_TIER12_BLOCK5_SUMMATION_RECEIVED_c                   0x09c4
#define g_CURRENT_TIER12_BLOCK6_SUMMATION_RECEIVED_c                   0x09c5
#define g_CURRENT_TIER12_BLOCK7_SUMMATION_RECEIVED_c                   0x09c6
#define g_CURRENT_TIER12_BLOCK8_SUMMATION_RECEIVED_c                   0x09c7
#define g_CURRENT_TIER12_BLOCK9_SUMMATION_RECEIVED_c                   0x09c8
#define g_CURRENT_TIER12_BLOCK10_SUMMATION_RECEIVED_c                  0x09c9
#define g_CURRENT_TIER12_BLOCK11_SUMMATION_RECEIVED_c                  0x09ca
#define g_CURRENT_TIER12_BLOCK12_SUMMATION_RECEIVED_c                  0x09cb
#define g_CURRENT_TIER12_BLOCK13_SUMMATION_RECEIVED_c                  0x09cc
#define g_CURRENT_TIER12_BLOCK14_SUMMATION_RECEIVED_c                  0x09cd
#define g_CURRENT_TIER12_BLOCK15_SUMMATION_RECEIVED_c                  0x09ce
#define g_CURRENT_TIER12_BLOCK16_SUMMATION_RECEIVED_c                  0x09cf

#define g_CURRENT_TIER13_BLOCK1_SUMMATION_RECEIVED_c                   0x09d0
#define g_CURRENT_TIER13_BLOCK2_SUMMATION_RECEIVED_c                   0x09d1
#define g_CURRENT_TIER13_BLOCK3_SUMMATION_RECEIVED_c                   0x09d2
#define g_CURRENT_TIER13_BLOCK4_SUMMATION_RECEIVED_c                   0x09d3
#define g_CURRENT_TIER13_BLOCK5_SUMMATION_RECEIVED_c                   0x09d4
#define g_CURRENT_TIER13_BLOCK6_SUMMATION_RECEIVED_c                   0x09d5
#define g_CURRENT_TIER13_BLOCK7_SUMMATION_RECEIVED_c                   0x09d6
#define g_CURRENT_TIER13_BLOCK8_SUMMATION_RECEIVED_c                   0x09d7
#define g_CURRENT_TIER13_BLOCK9_SUMMATION_RECEIVED_c                   0x09d8
#define g_CURRENT_TIER13_BLOCK10_SUMMATION_RECEIVED_c                  0x09d9
#define g_CURRENT_TIER13_BLOCK11_SUMMATION_RECEIVED_c                  0x09da
#define g_CURRENT_TIER13_BLOCK12_SUMMATION_RECEIVED_c                  0x09db
#define g_CURRENT_TIER13_BLOCK13_SUMMATION_RECEIVED_c                  0x09dc
#define g_CURRENT_TIER13_BLOCK14_SUMMATION_RECEIVED_c                  0x09dd
#define g_CURRENT_TIER13_BLOCK15_SUMMATION_RECEIVED_c                  0x09de
#define g_CURRENT_TIER13_BLOCK16_SUMMATION_RECEIVED_c                  0x09df

#define g_CURRENT_TIER14_BLOCK1_SUMMATION_RECEIVED_c                   0x09e0
#define g_CURRENT_TIER14_BLOCK2_SUMMATION_RECEIVED_c                   0x09e1
#define g_CURRENT_TIER14_BLOCK3_SUMMATION_RECEIVED_c                   0x09e2
#define g_CURRENT_TIER14_BLOCK4_SUMMATION_RECEIVED_c                   0x09e3
#define g_CURRENT_TIER14_BLOCK5_SUMMATION_RECEIVED_c                   0x09e4
#define g_CURRENT_TIER14_BLOCK6_SUMMATION_RECEIVED_c                   0x09e5
#define g_CURRENT_TIER14_BLOCK7_SUMMATION_RECEIVED_c                   0x09e6
#define g_CURRENT_TIER14_BLOCK8_SUMMATION_RECEIVED_c                   0x09e7
#define g_CURRENT_TIER14_BLOCK9_SUMMATION_RECEIVED_c                   0x09e8
#define g_CURRENT_TIER14_BLOCK10_SUMMATION_RECEIVED_c                  0x09e9
#define g_CURRENT_TIER14_BLOCK11_SUMMATION_RECEIVED_c                  0x09ea
#define g_CURRENT_TIER14_BLOCK12_SUMMATION_RECEIVED_c                  0x09eb
#define g_CURRENT_TIER14_BLOCK13_SUMMATION_RECEIVED_c                  0x09ec
#define g_CURRENT_TIER14_BLOCK14_SUMMATION_RECEIVED_c                  0x09ed
#define g_CURRENT_TIER14_BLOCK15_SUMMATION_RECEIVED_c                  0x09ee
#define g_CURRENT_TIER14_BLOCK16_SUMMATION_RECEIVED_c                  0x09ef

#define g_CURRENT_TIER15_BLOCK1_SUMMATION_RECEIVED_c                   0x09f0
#define g_CURRENT_TIER15_BLOCK2_SUMMATION_RECEIVED_c                   0x09f1
#define g_CURRENT_TIER15_BLOCK3_SUMMATION_RECEIVED_c                   0x09f2
#define g_CURRENT_TIER15_BLOCK4_SUMMATION_RECEIVED_c                   0x09f3
#define g_CURRENT_TIER15_BLOCK5_SUMMATION_RECEIVED_c                   0x09f4
#define g_CURRENT_TIER15_BLOCK6_SUMMATION_RECEIVED_c                   0x09f5
#define g_CURRENT_TIER15_BLOCK7_SUMMATION_RECEIVED_c                   0x09f6
#define g_CURRENT_TIER15_BLOCK8_SUMMATION_RECEIVED_c                   0x09f7
#define g_CURRENT_TIER15_BLOCK9_SUMMATION_RECEIVED_c                   0x09f8
#define g_CURRENT_TIER15_BLOCK10_SUMMATION_RECEIVED_c                  0x09f9
#define g_CURRENT_TIER15_BLOCK11_SUMMATION_RECEIVED_c                  0x09fa
#define g_CURRENT_TIER15_BLOCK12_SUMMATION_RECEIVED_c                  0x09fb
#define g_CURRENT_TIER15_BLOCK13_SUMMATION_RECEIVED_c                  0x09fc
#define g_CURRENT_TIER15_BLOCK14_SUMMATION_RECEIVED_c                  0x09fd
#define g_CURRENT_TIER15_BLOCK15_SUMMATION_RECEIVED_c                  0x09fe
#define g_CURRENT_TIER15_BLOCK16_SUMMATION_RECEIVED_c                  0x09ff
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */
/* Alarms */
#if(g_SE_PROFILE_1_1_d == 1)
#define g_GENERIC_ALARM_MASK_c											0x0800
#define g_ELECTRICITY_ALARM_MASK_c										0x0801
#define g_GENERIC_FLOW_MEASURE_ALARM_MASK_c								0x0802
#define g_WATER_SPECIFIC_ALARM_MASK_c									0x0803
#define g_HEAT_AND_COOLING_SPECIFIC_ALARM_MASK_c						0x0804
#define g_GAS_SPECIFIC_ALARM_MASK_c										0x0805
#endif
#if(g_SE_PROFILE_1_2_d == 1)
#define g_EXTENDED_GENERIC_ALARM_MASK_c                                 0x0806
#define g_MANUFACTURER_ALARM_MASK_c                                     0x0807
#endif  /* End of g_SE_PROFILE_1_2_d == 1 */
/* Billing Attribute Set */
#if (g_SE_PROFILE_1_2_d == 1)
#define g_BILL_TO_DATE_IMPORT_c                                         0x0A00
#define g_BILL_TO_DATE_TIME_STAMP_IMPORT_c                              0x0A01
#define g_PROJECTED_BILL_IMPORT_c                                       0x0A02
#define g_PROJECTED_BILL_TIME_STAMP_IMPORT_c                            0x0A03
#define g_BILL_TO_DATE_EXPORT_c                                         0x0A10
#define g_BILL_TO_DATE_TIME_STAMP_EXPORT_c                              0x0A11
#define g_PROJECTED_BILL_EXPORT_c                                       0x0A12
#define g_PROJECTED_BILL_TIME_STAMP_EXPORT_c                            0x0A13

#endif  /* End of g_SE_PROFILE_1_2_d == 1 */

/* Units of measure*/
enum {
    g_SE_UOM_BINARY_KWH_c                   = 0x00,
    g_SE_UOM_BINARY_CUBIC_METER_c           = 0x01,
    g_SE_UOM_BINARY_CUBIC_FEET_c            = 0x02,
    g_SE_UOM_BINARY_CCF_c                   = 0x03,
    g_SE_UOM_BINARY_US_GAL_c                = 0x04,
    g_SE_UOM_BINARY_IMP_GAL_c               = 0x05,
    g_SE_UOM_BINARY_BTU_c                   = 0x06,
    g_SE_UOM_BINARY_LITER_c                 = 0x07,
    g_SE_UOM_BINARY_KPA_GAUGE_c             = 0x08,
    g_SE_UOM_BINARY_KPA_ABSOLUTE_c          = 0x09,
    g_SE_UOM_BINARY_THOUSAND_CUBIC_FEET_c   = 0x0A,
    g_SE_UOM_BINARY_c                       = 0x0B,
    g_SE_UOM_BINARY_MJ_c                    = 0x0C,
    g_SE_UOM_BINARY_KVAR_c                  = 0x0D,
    g_SE_UOM_BCD_KWH_c                      = 0x80,
    g_SE_UOM_BCD_CUBIC_METER_c              = 0x81,
    g_SE_UOM_BCD_CUBIC_FEET_c               = 0x82,
    g_SE_UOM_BCD_CCF_c                      = 0x83,
    g_SE_UOM_BCD_US_GAL_c                   = 0x84,
    g_SE_UOM_BCD_IMP_GAL_c                  = 0x85,
    g_SE_UOM_BCD_BTU_c                      = 0x86,
    g_SE_UOM_BCD_LITER_c                    = 0x87,
    g_SE_UOM_BCD_KPA_GAUGE_c                = 0x88,
    g_SE_UOM_BCD_KPA_ABSOLUTE_c             = 0x89,
    g_SE_UOM_BCD_THOUSAND_CUBIC_FEET_c      = 0x8A,
    g_SE_UOM_BCD_c                          = 0x8B,
    g_SE_UOM_BCD_MJ_c                       = 0x8C,
    g_SE_UOM_BCD_KVAR_c                     = 0x8D
};

/* Device types*/
enum {
    g_SE_DT_ELECTRIC_METER_c         		            = 0x00,
    g_SE_DT_GAS_METER_c              		            = 0x01,
    g_SE_DT_WATER_METER_c             		            = 0x02,
    g_SE_DT_THERMAL_METER_c           		            = 0x03,
    g_SE_DT_PRESSURE_METER_c         		            = 0x04,
    g_SE_DT_HEAT_METER_c           	 		            = 0x05,
    g_SE_DT_COOLING_METER_c           		            = 0x06,
    g_SE_DT_ELECTRIC_VEHICLE_CHARGING_METER_c           = 0x07,
    g_SE_DT_PV_GENERATION_METER_c                       = 0x08,
    g_SE_DT_WIND_TURBINE_GENERATION_METER_c             = 0x09,
    g_SE_DT_WATER__TURBINE_GENERATION_METER_c           = 0x0A,
    g_SE_DT_MICRO_GENERATION_METER_c                    = 0x0B,
    g_SE_DT_SOLAR_HOT_GENERATION_METER_c                = 0x0C,
    g_SE_DT_ELECTRIC_METER_ELEMENT_PHASE1_c             = 0x0D,
    g_SE_DT_ELECTRIC_METER_ELEMENT_PHASE2_c             = 0x0E,
    g_SE_DT_ELECTRIC_METER_ELEMENT_PHASE3_c             = 0x0F,
    g_SE_DT_MIRRORED_GAS_METER_c     		            = 0x80,
    g_SE_DT_MIRRORED_WATER_METER_c   		            = 0x81,
    g_SE_DT_MIRRORED_THERMAL_METER_c  		            = 0x82,
    g_SE_DT_MIRRORED_PRESSUREMETER_c                    = 0x83,
    g_SE_DT_MIRRORED_HEAT_METER_c                       = 0x84,
    g_SE_DT_MIRRORED_COOLING_METER_c                    = 0x85,
    g_SE_DT_MIRRORED_ELECTRIC_VEHICLE_CHARGING_METER_c  = 0x86,
    g_SE_DT_MIRRORED_PV_GENERATION_METER_c              = 0x87,
    g_SE_DT_MIRRORED_WIND_TURBINE_GENERATION_METER_c    = 0x88,
    g_SE_DT_MIRRORED_WATER__TURBINE_GENERATION_METER_c  = 0x89,
    g_SE_DT_MIRRORED_MICRO_GENERATION_METER_c           = 0x8A,
    g_SE_DT_MIRRORED_SOLAR_HOT_GENERATION_METER_c       = 0x8B
};

#endif /*  (g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1) */
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Simple_metering_Server_Cluster_Info;
#endif				/* g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d */

#if ( g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Simple_metering_Client_Cluster_Info;
#endif				/* g_SIMPLE_METERING_CLIENT_CLUSTER_ENABLE_d */
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
typedef struct Metering_Boundary_Values_Tag
{
   uint16_t attributeId;
   int16_t lowerBoundaryValue;
   int16_t  upperBoundaryValue;
}__attribute__((__packed__))Metering_Boundary_Values_t;

/*-----------------------------------------------------------------------------
Simple Metering cluster structure - attributes
-----------------------------------------------------------------------------*/
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )
typedef struct Simple_Metering_Cluster_Tag
{
	uint8_t currentSummationDelivered[6];

#if ( g_CURRENT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t  currentSummationRecieved[6];
#endif				/* g_CURRENT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_MAX_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentMaxDemandDelivered[6];
#endif				/* g_CURRENT_MAX_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_MAX_DEMAND_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentMaxDemandRecieved[6];
#endif				/* g_CURRENT_MAX_DEMAND_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_DFT_SUMMATION_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  dftSummation[6];
#endif				/* g_DFT_SUMMATION_ATTRIBUTE_ENABLE_d */

#if ( g_DAILY_FREEZE_TIME_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  dailyFreezeTime[2];
#endif				/* g_DAILY_FREEZE_TIME_ATTRIBUTE_ENABLE_d */

#if ( g_POWER_FACTOR_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t powerFactor;
#endif				/* g_POWER_FACTOR_ATTRIBUTE_ENABLE_d */

#if ( g_READING_SNAP_SHOT_TIME_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  readingSnapShotTime[4];
#endif				/* g_READING_SNAP_SHOT_TIME_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_MAX_DEMAND_DELIVERED_TIME_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentMaxDemandDeliveredTime[4];
#endif				/* g_CURRENT_MAX_DEMAND_DELIVERED_TIME_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_MAX_DEMAND_RECIEVED_TIME_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentMaxDemandRecievedTime[4];
#endif				/* g_CURRENT_MAX_DEMAND_RECIEVED_TIME_ATTRIBUTE_ENABLE_d */
#if(g_SE_PROFILE_1_1_d == 1)
	
#if (  g_DEFAULT_UPDATE_PERIOD_ATTRIBUTE_ENABLED_d	== 1 )
	uint8_t  defaultUpdatePeriod;
#endif	          /* g_DEFAULT_UPDATE_PERIOD_ATTRIBUTE_ENABLED_d */
	
#if (  g_FAST_POLL_UPDATE_PERIOD_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  fastPollUpdatePeriod;
#endif            /* g_FAST_POLL_UPDATE_PERIOD_ATTRIBUTE_ENABLED_d */
	
#if (  g_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  currentBlockPeriodConsumptionDelivered[6];
#endif		      /* g_CURRENT_BLOCK_PERIOD_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLED_d */

#if (  g_DAILY_CONSUMPTION_TARGET_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  dailyConsumptionTarget[3];
#endif		      /* g_DAILY_CONSUMPTION_TARGET_ATTRIBUTE_ENABLED_d */	
	
#if (  g_CURRENT_BLOCK_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  currentBlock;
#endif		      /* g_CURRENT_BLOCK_ATTRIBUTE_ENABLED_d */
	
#if (  g_PROFILE_INTERVAL_PERIOD_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  profileIntervalPeriod;
#endif		      /* g_PROFILE_INTERVAL_PERIOD_ATTRIBUTE_ENABLED_d */	

#if (  g_INTERVAL_READ_REPORTING_PERIOD_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  intervalReadReportingPeriod[2];
#endif		      /* g_INTERVAL_READ_REPORTING_PERIOD_ATTRIBUTE_ENABLED_d */	
	
#if (  g_PRESET_READING_TIME_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  presetReadingTime[2];
#endif		      /* g_PRESET_READING_TIME_ATTRIBUTE_ENABLED_d */			

#if (  g_VOLUME_PER_REPORT_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  volumePerReport[2];
#endif		      /* g_VOLUME_PER_REPORT_ATTRIBUTE_ENABLED_d */		
	
#if (  g_FLOW_RESTRICTION_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  flowRestriction;
#endif		      /* g_FLOW_RESTRICTION_ATTRIBUTE_ENABLED_d */	
	
#if (  g_SUPPLY_STATUS_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  supplyStatus;
#endif		      /* g_SUPPLY_STATUS_ATTRIBUTE_ENABLED_d */		

#if (  g_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  currentInletEnergyCarrierSummation[6];
#endif		      /* g_CURRENT_INLET_ENERGY_CARRIER_SUMMATION_ATTRIBUTE_ENABLED_d */	
	
#if (  g_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  currentOutletEnergyCarrierSummation[6];
#endif		      /* g_CURRENT_OUTLET_ENERGY_CARRIER_SUMMATION_ATTRIBUTE_ENABLED_d */	
	
#if (  g_INLET_TEMPERATURE_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  inletTemperature[2];
#endif		      /* g_INLET_TEMPERATURE_ATTRIBUTE_ENABLED_d */	
#if (  g_OUTLET_TEMPERATURE_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  outletTemperature[2];
#endif		      /* g_OUTLET_TEMPERATURE_ATTRIBUTE_ENABLED_d */
	
#if (  g_CONTROL_TEMPERATURE_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  controlTemperature[2];
#endif		      /* g_CONTROL_TEMPERATURE_ATTRIBUTE_ENABLED_d */		

#if (  g_CURRENT_INLET_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  currentInletEnergyCarrierDemand[3];
#endif		      /* g_CURRENT_INLET_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLED_d */		

#if (  g_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLED_d == 1 )
	uint8_t  current_outletEnergyCarrierDemand[3];
#endif		      /* g_CURRENT_OUTLET_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLED_d */	
#endif /* #if ( g_SE_PROFILE_1_1_d == 1 ) */	

#if ( g_SE_PROFILE_1_2_d == 1 )
#if (  g_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  previousBlockPeriodConsumptionDelivered_[6];
#endif		      /* g_PREVIOUS_BLOCK_PERIOD_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d */	
#if (  g_CURRENT_BLOCK_PERIOD_CONSUMPTION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  previousBlockPeriodConsumptionRecieved_[6];
#endif		      /* g_CURRENT_BLOCK_PERIOD_CONSUMPTION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if (  g_CURRENT_BLOCK_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentBlockRecieved;
#endif		      /* g_CURRENT_BLOCK_RECIEVED_ATTRIBUTE_ENABLE_d */
#if (  g_DFT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  DFTSummationRecieved_[6];
#endif		      /* g_DFT_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if (  g_ACTIVE_REGISTER_TIER_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  activeRegisterTierDelivered;
#endif		      /* g_ACTIVE_REGISTER_TIER_DELIVERED_ATTRIBUTE_ENABLE_d */
#if (  g_ACTIVE_REGISTER_TIER_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  activeRegisterTierRecieved;
#endif		      /* g_ACTIVE_REGISTER_TIER_RECIEVED_ATTRIBUTE_ENABLE_d */
#endif /* #if ( g_SE_PROFILE_1_2_d == 1 ) */	

#if ( g_CURRENT_TIER1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1SummationDelivered[6];
#endif				/* g_CURRENT_TIER1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER1_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1SummationRecieved[6];
#endif				/* g_CURRENT_TIER1_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2SummationDelivered[6];
#endif				/* g_CURRENT_TIER2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER2_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2SummationRecieved[6];
#endif				/* g_CURRENT_TIER2_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3SummationDelivered[6];
#endif				/* g_CURRENT_TIER3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER3_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3SummationRecieved[6];
#endif				/* g_CURRENT_TIER3_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4SummationDelivered[6];
#endif				/* g_CURRENT_TIER4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER4_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4SummationRecieved[6];
#endif				/* g_CURRENT_TIER4_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5SummationDelivered[6];
#endif				/* g_CURRENT_TIER5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER5_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5SummationRecieved[6];
#endif				/* g_CURRENT_TIER5_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6SummationDelivered[6];
#endif				/* g_CURRENT_TIER6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER6_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6SummationRecieved[6];
#endif				/* g_CURRENT_TIER6_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_CURRENT_TIER7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7SummationDelivered[6];
#endif				/* g_CURRENT_TIER7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER7_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7SummationRecieved[6];
#endif				/* g_CURRENT_TIER7_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8SummationDelivered[6];
#endif				/* g_CURRENT_TIER8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER8_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8SummationRecieved[6];
#endif				/* g_CURRENT_TIER8_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9SummationDelivered[6];
#endif				/* g_CURRENT_TIER9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER9_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9SummationRecieved[6];
#endif				/* g_CURRENT_TIER9_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10SummationDelivered[6];
#endif				/* g_CURRENT_TIER10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER10_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10SummationRecieved[6];
#endif				/* g_CURRENT_TIER10_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11SummationDelivered[6];
#endif				/* g_CURRENT_TIER11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER11_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11SummationRecieved[6];
#endif				/* g_CURRENT_TIER11_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12SummationDelivered[6];
#endif				/* g_CURRENT_TIER12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER12_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12SummationRecieved[6];
#endif				/* g_CURRENT_TIER12_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13SummationDelivered[6];
#endif				/* g_CURRENT_TIER13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER13_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13SummationRecieved[6];
#endif				/* g_CURRENT_TIER13_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14SummationDelivered[6];
#endif				/* g_CURRENT_TIER14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER14_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14SummationRecieved[6];
#endif				/* g_CURRENT_TIER14_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15SummationDelivered[6];
#endif				/* g_CURRENT_TIER15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_TIER15_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15SummationRecieved[6];
#endif				/* g_CURRENT_TIER15_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */		
#endif /* End of g_SE_PROFILE_1_1_d == 1 */

#if (g_SE_PROFILE_1_2_d == 1)
#if ( g_CURRENT_TIER16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16SummationDelivered[6];
#endif				/* g_CURRENT_TIER16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER16_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16SummationRecieved[6];
#endif				/* g_CURRENT_TIER16_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER17_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier17SummationDelivered[6];
#endif				/* g_CURRENT_TIER17_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER17_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier17SummationRecieved[6];
#endif				/* g_CURRENT_TIER17_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER18_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier18SummationDelivered[6];
#endif				/* g_CURRENT_TIER18_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER18_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier18SummationRecieved[6];
#endif				/* g_CURRENT_TIER18_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER19_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier19SummationDelivered[6];
#endif				/* g_CURRENT_TIER19_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER19_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier19SummationRecieved[6];
#endif				/* g_CURRENT_TIER19_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER20_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier20SummationDelivered[6];
#endif				/* g_CURRENT_TIER20_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER20_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier20SummationRecieved[6];
#endif				/* g_CURRENT_TIER20_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER21_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier21SummationDelivered[6];
#endif				/* g_CURRENT_TIER21_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER21_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier21SummationRecieved[6];
#endif				/* g_CURRENT_TIER21_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER22_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier22SummationDelivered[6];
#endif				/* g_CURRENT_TIER22_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER22_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier22SummationRecieved[6];
#endif				/* g_CURRENT_TIER22_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER23_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier23SummationDelivered[6];
#endif				/* g_CURRENT_TIER23_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER23_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier23SummationRecieved[6];
#endif				/* g_CURRENT_TIER23_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER24_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier24SummationDelivered[6];
#endif				/* g_CURRENT_TIER24_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER24_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier24SummationRecieved[6];
#endif				/* g_CURRENT_TIER24_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER25_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier25SummationDelivered[6];
#endif				/* g_CURRENT_TIER25_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER25_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier25SummationRecieved[6];
#endif				/* g_CURRENT_TIER25_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER26_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier26SummationDelivered[6];
#endif				/* g_CURRENT_TIER26_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER26_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier26SummationRecieved[6];
#endif				/* g_CURRENT_TIER26_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER27_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier27SummationDelivered[6];
#endif				/* g_CURRENT_TIER27_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER27_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier27SummationRecieved[6];
#endif				/* g_CURRENT_TIER27_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER28_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier28SummationDelivered[6];
#endif				/* g_CURRENT_TIER28_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER28_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier28SummationRecieved[6];
#endif				/* g_CURRENT_TIER28_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER29_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier29SummationDelivered[6];
#endif				/* g_CURRENT_TIER29_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER29_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier29SummationRecieved[6];
#endif				/* g_CURRENT_TIER29_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER30_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier30SummationDelivered[6];
#endif				/* g_CURRENT_TIER30_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER30_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier30SummationRecieved[6];
#endif				/* g_CURRENT_TIER30_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER31_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier31SummationDelivered[6];
#endif				/* g_CURRENT_TIER31_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER31_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier31SummationRecieved[6];
#endif				/* g_CURRENT_TIER31_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER32_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier32SummationDelivered[6];
#endif				/* g_CURRENT_TIER32_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER32_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier32SummationRecieved[6];
#endif				/* g_CURRENT_TIER32_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER33_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier33SummationDelivered[6];
#endif				/* g_CURRENT_TIER33_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER33_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier33SummationRecieved[6];
#endif				/* g_CURRENT_TIER33_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER34_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier34SummationDelivered[6];
#endif				/* g_CURRENT_TIER34_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER34_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier34SummationRecieved[6];
#endif				/* g_CURRENT_TIER34_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER35_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier35SummationDelivered[6];
#endif				/* g_CURRENT_TIER35_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER35_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier35SummationRecieved[6];
#endif				/* g_CURRENT_TIER35_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER36_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier36SummationDelivered[6];
#endif				/* g_CURRENT_TIER36_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER36_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier36SummationRecieved[6];
#endif				/* g_CURRENT_TIER36_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER37_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier37SummationDelivered[6];
#endif				/* g_CURRENT_TIER37_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER37_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier37SummationRecieved[6];
#endif				/* g_CURRENT_TIER37_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER38_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier38SummationDelivered[6];
#endif				/* g_CURRENT_TIER38_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER38_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier38SummationRecieved[6];
#endif				/* g_CURRENT_TIER38_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER39_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier39SummationDelivered[6];
#endif				/* g_CURRENT_TIER39_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER39_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier39SummationRecieved[6];
#endif				/* g_CURRENT_TIER39_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER40_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier40SummationDelivered[6];
#endif				/* g_CURRENT_TIER40_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER40_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier40SummationRecieved[6];
#endif				/* g_CURRENT_TIER40_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER41_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier41SummationDelivered[6];
#endif				/* g_CURRENT_TIER41_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER41_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier41SummationRecieved[6];
#endif				/* g_CURRENT_TIER41_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER42_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier42SummationDelivered[6];
#endif				/* g_CURRENT_TIER42_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER42_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier42SummationRecieved[6];
#endif				/* g_CURRENT_TIER42_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER43_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier43SummationDelivered[6];
#endif				/* g_CURRENT_TIER43_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER43_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier43SummationRecieved[6];
#endif				/* g_CURRENT_TIER43_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER44_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier44SummationDelivered[6];
#endif				/* g_CURRENT_TIER44_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER44_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier44SummationRecieved[6];
#endif				/* g_CURRENT_TIER44_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER45_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier45SummationDelivered[6];
#endif				/* g_CURRENT_TIER45_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER45_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier45SummationRecieved[6];
#endif				/* g_CURRENT_TIER45_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER46_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier46SummationDelivered[6];
#endif				/* g_CURRENT_TIER46_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER46_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier46SummationRecieved[6];
#endif				/* g_CURRENT_TIER46_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER47_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier47SummationDelivered[6];
#endif				/* g_CURRENT_TIER47_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER47_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier47SummationRecieved[6];
#endif				/* g_CURRENT_TIER47_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER48_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier48SummationDelivered[6];
#endif				/* g_CURRENT_TIER48_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_TIER48_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier48SummationRecieved[6];
#endif				/* g_CURRENT_TIER48_SUMMATION_RECIEVED_ATTRIBUTE_ENABLE_d */
#endif /* End of g_SE_PROFILE_1_2_d == 1 */

uint8_t status;
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_REMAINING_BATTERY_LIFE_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t remainingBatteryLife;
#endif
#if ( g_HOURS_IN_OPERATION_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t hoursInOperation[3];
#endif
#if ( g_HOURS_IN_FAULT_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t hoursInFault[3];
#endif
#endif /* End of g_SE_PROFILE_1_1_d == 1 */
#if(g_SE_PROFILE_1_2_d == 1)
#if ( g_EXTENDED_STATUS_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t extendedStatus;
#endif
#if ( g_REMAINING_BATTERY_LIFE_IN_DAYS_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t  remainingBatteryLifeInDays[2];
#endif
#if ( g_CURRENT_METER_ID_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t currentMeterId[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif
#if ( g_LOW_MEDIUM_HIGH_STATUS_ATTRIBUTE_ENABLE_d == 1 )
   uint8_t lowMediumHighStatus;
#endif
#endif /* End of g_SE_PROFILE_1_2_d == 1 */
uint8_t unitOfMeasure;

#if ( g_MULTIPLIER_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  multiplier[3];
#endif										/* g_MULTIPLIER_ATTRIBUTE_ENABLE_d */

#if ( g_DIVISOR_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  devisor[3];
#endif										/* g_DIVISOR_ATTRIBUTE_ENABLE_d */

	uint8_t summationFormatting;

#if ( g_DEMAND_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t demandFormatting;
#endif										/* g_DEMAND_FORMATTING_ATTRIBUTE_ENABLE_d */

#if ( g_HISTORICAL_CONSUMPTION_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t historicalConsumptionFormatting;
#endif										/* g_HISTORICAL_CONSUMPTION_FORMATTING_ATTRIBUTE_ENABLE_d */

	uint8_t meteringDevice_type;
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_SITE_ID_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t siteId[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif	

#if ( g_METER_SERIAL_NUMBER_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t meterSerialNumber[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif		

#if ( g_ENERGY_CARRIER_UNIT_OF_MEASURE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t energyCarrierUnitOfMeasure;
#endif		
	
#if ( g_ENERGY_CARRIER_SUMMATION_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t energyCarrierSummationFormatting;
#endif	
	
#if ( g_ENERGY_CARRIER_DEMAND_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t energyCarrierDemandFormatting;
#endif	
#if ( g_TEMPERATURE_UNIT_OF_MEASURE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t temperatureUnitOfMeasure;
#endif
#if ( g_TEMPERATURE_FORMATTING_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t temperatureFormatting;
#endif	
#endif /*g_SE_PROFILE_1_2_d*/	

#if ( g_SE_PROFILE_1_2_d == 1 )
#if ( g_MODULE_SERIAL_NUMBER_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t moduleSerialNumber[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif
#if ( g_OPERATING_TARIFF_LABEL_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t operatingTariffLabelDelivered[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif
#if ( g_OPERATING_TARIFF_LABEL_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t operatingtariffLabelReceived[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif
#endif  /* g_SE_PROFILE_1_2_d*/	

#if ( g_INSTANTANEOUS_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  instantaneousDemand[3];
#endif				/* g_INSTANTANEOUS_DEMAND_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentdayConsumptionDelivered[3];
#endif				/* g_CURRENT_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentdayConsumptionRecieved[3];
#endif				/* g_CURRENT_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_PREVIOUS_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  previousdayConsumptionDelivered[3];
#endif				/* g_PREVIOUS_DAY_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_PREVIOUS_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  previousdayConsumptionRecieved[3];
#endif				/* g_PREVIOUS_DAY_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  profileStartTimeDelivered[4];
#endif				/* g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_DELEVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  profileStartTimeRecieved[4];
#endif				/* g_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME_RECIEVED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  profileIntervalValueDelivered[3];
#endif				/* g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_DELEVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_RECIEVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  profileIntervalValueRecieved[3];
#endif				/* g_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE_RECIEVED_ATTRIBUTE_ENABLE_d */		
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_CURRENT_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  current_dayMaxPressure[6];
#endif				/* g_CURRENT_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  current_dayMinPressure[6];
#endif				/* g_CURRENT_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d */
#if ( g_PREVIOUS_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t   previous_dayMaxPressure[6];
#endif				/* g_PREVIOUS_DAY_MAX_PRESSURE_ATTRIBUTE_ENABLE_d */

#if ( g_PREVIOUS_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  previous_dayMinPressure[6];
#endif				/* g_PREVIOUS_DAY_MIN_PRESSURE_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_DAY_MAX_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  current_dayMaxDemand[3];
#endif				/* g_CURRENT_DAY_MAX_DEMAND_ATTRIBUTE_ENABLE_d */

#if ( g_PREVIOUS_DAY_MAX_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  previous_dayMaxDemand[3];
#endif				/* g_CURRENT_DAY_MIN_DEMAND_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_MONTH_MAX_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentMonthMaxDemand[3];
#endif				/* g_CURRENT_MONTH_MAX_DEMAND_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_YEAR_MAX_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  current_yearMaxDemand[3];
#endif				/* g_CURRENT_YEAR_MAX_DEMAND_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  current_dayMaxEnergyCarrierDemand[3];
#endif				/* g_CURRENT_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
	
#if ( g_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  previous_dayMaxEnergyCarrierDemand[3];
#endif				/* g_PREVIOUS_DAY_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_MONTH_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentMonthMaxEnergyCarrierDemand[3];
	
#endif				/* g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
#if ( g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentMonthMinEnergyCarrierDemand[3];
	
#endif				/* g_CURRENT_MONTH_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */

#if ( g_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  current_yearMaxEnergyCarrierDemand[3];
	
#endif				/* g_CURRENT_YEAR_MAX_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  current_yearMinEnergyCarrierDemand[3];
	
#endif				/* g_CURRENT_YEAR_MIN_ENERGY_CARRIER_DEMAND_ATTRIBUTE_ENABLE_d */	
#endif /*#if ( g_SE_PROFILE_1_1_d == 1 ) */	

#if ( g_SE_PROFILE_1_2_d == 1 )
#if ( g_PREVIOUS_DAY2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day2ConsumptionDelivered[3];
#endif		
#if ( g_PREVIOUS_DAY2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day2ConsumptionReceived[3];
#endif	
#if ( g_PREVIOUS_DAY3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day3ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_DAY3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day3ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_DAY4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day4ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_DAY4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day4ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_DAY5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day5ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_DAY5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day5ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_DAY6_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day6ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_DAY6_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day6ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_DAY7_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day7ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_DAY7_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day7ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_DAY8_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day8ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_DAY8_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_day8ConsumptionReceived[3];
#endif
#if ( g_CURRENT_WEEK_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  current_weekConsumptionDelivered[3];
#endif
#if ( g_CURRENT_WEEK_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  current_weekConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_WEEK_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_weekConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_WEEK_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_weekConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_WEEK2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_week2ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_WEEK2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_week2ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_WEEK3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_week3ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_WEEK3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_week3ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_WEEK4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_week4ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_WEEK4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_week4ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_WEEK5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_week5ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_WEEK5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previous_week5ConsumptionReceived[3];
#endif
#if ( g_CURRENT_MONTH_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  currentMonthConsumptionDelivered[3];
#endif
#if ( g_CURRENT_MONTH_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  currentMonthConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonthConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonthConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH2_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth2ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH2_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth2ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH3_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth3ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH3_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth3ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH4_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth4ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH4_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth4ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH5_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth5ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH5_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth5ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH6_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth6ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH6_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth6ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH7_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth7ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH7_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth7ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH8_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth8ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH8_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth8ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH9_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth9ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH9_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth9ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH10_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth10ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH10_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth10ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH11_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth11ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH11_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth11ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH12_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth12ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH12_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth12ConsumptionReceived[3];
#endif
#if ( g_PREVIOUS_MONTH13_CONSUMPTION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth13ConsumptionDelivered[3];
#endif
#if ( g_PREVIOUS_MONTH13_CONSUMPTION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )   	
	uint8_t  previousMonth13ConsumptionReceived[3];
#endif
#endif /* ( g_SE_PROFILE_1_2_d == 1 ) */	

#if ( g_MAX_NUMBER_OF_PERIODS_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t maxNumberOfPeriodsDelivered;
#endif				/* g_MAX_NUMBER_OF_PERIODS_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentDemandDelivered[3];
#endif				/* g_CURRENT_DEMAND_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_DEMAND_LIMIT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  demandLimit[3];
#endif				/* g_DEMAND_LIMIT_ATTRIBUTE_ENABLE_d */		

#if ( g_DEMAND_INTEGRATION_PERIOD_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t demandIntegrationPeriod;
#endif				/* g_DEMAND_INTEGRATION_PERIOD_ATTRIBUTE_ENABLE_d */	

#if ( g_NUMBER_OF_DEMAND_SUBINTERVALS_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t numOfDemandSubintervals;
#endif				/* g_NUMBER_OF_DEMAND_SUBINTERVALS_ATTRIBUTE_ENABLE_d */	
#if ( g_SE_PROFILE_1_2_d == 1 )
#if ( g_DEMAND_LIMIT_ARM_DURATION_IN_MINUTES_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t demandLimitArmDurationInMinutes[2];
#endif
#if ( g_LOAD_LIMIT_SUPPLY_STATE_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t loadLimitSupplyState;
#endif
#if ( g_LOAD_LIMIT_COUNTER_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t loadLimitCounter;
#endif
#endif /* ( g_SE_PROFILE_1_2_d == 1 ) */
#if(g_SE_PROFILE_1_1_d == 1)	
#if ( g_CURRENT_NO_TIER_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock1Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock2Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock3Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock4Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock5Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock6Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock7Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock8Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_NO_TIER_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock9Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock10Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock11Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock12Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		


#if ( g_CURRENT_NO_TIER_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock13Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_NO_TIER_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock14Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_NO_TIER_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock15Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_NO_TIER_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock16Summation[6];
#endif				/* g_CURRENT_NO_TIER_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER1_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block1Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			

#if ( g_CURRENT_TIER1_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block2Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER1_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block3Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block4Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER1_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block5Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER1_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block6Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

	
#if ( g_CURRENT_TIER1_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block7Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block8Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
	
#if ( g_CURRENT_TIER1_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block9Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			

	
#if ( g_CURRENT_TIER1_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block10Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			

#if ( g_CURRENT_TIER1_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block11Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER1_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block12Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	

#if ( g_CURRENT_TIER1_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block13Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER1_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block14Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER1_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block15Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */			
#if ( g_CURRENT_TIER1_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block16Summation[6];
#endif				/* g_CURRENT_TIER1_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER2_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block1Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block2Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER2_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block3Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER2_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block4Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER2_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block5Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */

  #if ( g_CURRENT_TIER2_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block6Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER2_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block7Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER2_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block8Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER2_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block9Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK89_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block10Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block11Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block12Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block13Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER2_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block14Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block15Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER2_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block16Summation[6];
#endif				/* g_CURRENT_TIER2_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block1Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER3_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block2Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER3_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block3Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER3_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block4Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block5Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block6Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER3_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block7Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER3_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block8Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER3_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block9Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER3_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block10Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER3_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block11Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER3_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block12Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER3_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block13Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER3_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block14Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER3_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block15Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER3_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block16Summation[6];
#endif				/* g_CURRENT_TIER3_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block1Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER4_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block2Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER4_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block3Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block4Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block5Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block6Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER4_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block7Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER4_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block8Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block9Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER4_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block10Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block11Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block12Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER4_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block13Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block14Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block15Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER4_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block16Summation[6];
#endif				/* g_CURRENT_TIER4_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER5_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block1Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER5_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block2Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block3Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block4Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER5_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block5Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER5_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block6Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER5_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block7Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block8Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER5_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block9Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER5_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block10Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block11Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block12Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block13Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block14Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block15Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER5_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block16Summation[6];
#endif				/* g_CURRENT_TIER5_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER6_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block1Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block2Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block3Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block4Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER6_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block5Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER6_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block6Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER6_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block7Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER6_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block8Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block9Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER6_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block10Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block11Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block12Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block13Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block14Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER6_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block15Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER6_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block16Summation[6];
#endif				/* g_CURRENT_TIER6_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER7_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block1Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER7_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block2Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER7_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block3Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER7_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block4Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER7_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block5Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block6Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block7Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block8Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block9Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER7_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block10Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block11Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block12Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER7_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block13Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block14Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block15Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER7_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block16Summation[6];
#endif				/* g_CURRENT_TIER7_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block1Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER8_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block2Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER8_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block3Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER8_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block4Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block5Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER8_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block6Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block7Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER8_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block8Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER8_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block9Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block10Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block11Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block12Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block13Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block14Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block15Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER8_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block16Summation[6];
#endif				/* g_CURRENT_TIER8_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER9_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block1Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER9_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block2Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER9_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block3Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER9_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block4Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER9_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block5Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER9_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block6Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER9_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block7Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER9_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block8Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER9_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block9Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER9_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block10Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER9_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block11Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER9_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block12Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER9_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block13Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER9_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block14Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER9_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block15Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER9_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block16Summation[6];
#endif				/* g_CURRENT_TIER9_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block1Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER10_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block2Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block3Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block4Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block5Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block6Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		

#if ( g_CURRENT_TIER10_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block7Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block8Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block9Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block10Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER10_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block11Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block12Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block13Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER10_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block14Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block15Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER10_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block16Summation[6];
#endif				/* g_CURRENT_TIER10_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER11_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block1Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block2Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block3Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block4Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block5Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block6Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block7Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block8Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block9Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER11_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block10Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block11Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block12Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block13Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block14Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block15Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER11_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block16Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER12_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block1Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block2Summation[6];  	
#endif				/* g_CURRENT_TIER12_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block3Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block4Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block5Summation[6];
#endif				/* g_CURRENT_TIER11_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block6Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER12_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block7Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block8Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block9Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block10Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block11Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block12Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block13Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block14Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block15Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER12_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block16Summation[6];
#endif				/* g_CURRENT_TIER12_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER13_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block1Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block2Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block3Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block4Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block5Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block6Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block7Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block8Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block9Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER13_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block10Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block11Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block12Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block13Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block14Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block15Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER13_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block16Summation[6];
#endif				/* g_CURRENT_TIER13_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
	
#if ( g_CURRENT_TIER14_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block1Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block2Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block3Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block4Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block5Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block6Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER14_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block7Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER14_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block8Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER14_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block9Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block10Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block11Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block12Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block13Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block14Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER14_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block15Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER14_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block16Summation[6];
#endif				/* g_CURRENT_TIER14_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	

#if ( g_CURRENT_TIER15_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block1Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK1_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER15_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block2Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK2_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block3Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK3_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block4Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK4_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER15_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block5Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK5_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block6Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK6_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block7Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK7_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER15_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block8Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK8_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block9Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK9_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block10Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK10_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER15_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block11Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK11_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER15_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block12Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK12_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */
	
#if ( g_CURRENT_TIER15_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block13Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK13_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block14Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK14_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block15Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK15_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */	
	
#if ( g_CURRENT_TIER15_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block16Summation[6];
#endif				/* g_CURRENT_TIER15_BLOCK16_SUMMATION_DELIVERED_ATTRIBUTE_ENABLE_d */		
#endif /* g_SE_PROFILE_1_1_d*/
#if ( g_SE_PROFILE_1_1_d == 1 )
#if ( g_GENERIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  genericAlarmMask[2];

#endif				/* g_GENERIC_ALARM_MASK_ATTRIBUTE_ENABLE_d */	
#if ( g_ELECTRICITY_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  electricityAlarmMask[4];
	
#endif				/* g_ELECTRICITY_ALARM_MASK_ATTRIBUTE_ENABLE_d */

#if ( g_GENERIC_FLOW_MEASURE_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  genericFlowMeasureAlarmMask[2];
	
#endif				/* g_GENERIC_FLOW_MEASURE_ALARM_MASK_ATTRIBUTE_ENABLE_d */	
	
#if ( g_WATER_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  waterSpecificAlarmMask[2];
	
#endif				/* g_WATER_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d */	
	
#if ( g_HEAT_AND_COOLING_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  heatAndCoolingSpecificAlarm[2];
	
#endif				/* g_HEAT_AND_COOLING_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d */		
#if ( g_GAS_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  gasSpecificAlarmMask[2];
	
#endif				/* g_GAS_SPECIFIC_ALARM_MASK_ATTRIBUTE_ENABLE_d */
#endif /*( g_SE_PROFILE_1_1_d == 1 ) */	

#if ( g_SE_PROFILE_1_2_d == 1 )
#if ( g_CURRENT_NO_TIER_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock1SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock2SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock3SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock4SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock5SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock6SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock7SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock8SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock9SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock10SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock11SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock12SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock13SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock14SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock15SummationRecieved[6];
#endif
#if ( g_CURRENT_NO_TIER_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentNoTierBlock16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER1_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block10SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER1_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier1Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER2_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block10SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER2_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier2Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER3_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block10SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER3_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier3Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER4_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block10SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER4_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier4Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER5_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block10SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER5_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier5Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER6_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK10_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block10SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER6_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier6Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER7_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER7_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier7Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER8_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER8_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier8Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER9_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER9_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier9Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER10_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER10_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier10Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER11_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER11_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier11Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER12_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER12_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier12Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER13_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER13_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier13Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER14_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER14_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier14Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER15_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER15_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier15Block16SummationRecieved[6];
#endif

#if ( g_CURRENT_TIER16_BLOCK1_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block1SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK2_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block2SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK3_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block3SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK4_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block4SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK5_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block5SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK6_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block6SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK7_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block7SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK8_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block8SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK9_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block9SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK11_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block11SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK12_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block12SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK13_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block13SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK14_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block14SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK15_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block15SummationRecieved[6];
#endif
#if ( g_CURRENT_TIER16_BLOCK16_SUMMATION_RECEIVED_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  currentTier16Block16SummationRecieved[6];
#endif
#endif /* End of #if ( g_SE_PROFILE_1_2_d == 1 ) */
	
#if ( g_SE_PROFILE_1_2_d == 1 )
#if ( g_BILL_TO_DATE_IMPORT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  billToDateImport[4];
#endif
#if ( g_BILL_TO_DATE_TIME_STAMP_IMPORT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  billToTimeStampImport;
#endif
#if ( g_PROJECTED_BILL_IMPORT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  projectedBillImport[4];
#endif
#if ( g_PROJECTED_BILL_TIME_STAMP_IMPORT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  projectedBillTimeStampImport;
#endif
#if ( g_BILL_TO_DATE_EXPORT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  billToDateExport[4];
#endif
#if ( g_BILL_TO_DATE_TIME_STAMP_EXPORT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  billToDateTimeStampExport;
#endif
#if ( g_PROJECTED_BILL_EXPORT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  projectedBillExport[4];
#endif
#if ( g_PROJECTED_BILL_TIME_STAMP_EXPORT_ATTRIBUTE_ENABLE_d == 1 )
	uint8_t  projectedBillTimeStampExport;
#endif
#endif  /* End of #if ( g_SE_PROFILE_1_2_d == 1 ) */
	
}__attribute__((__packed__))Simple_Metering_Cluster_t;

#endif /* ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )*/
/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/

typedef enum Simple_Metering_Event
{
	g_Get_Profile_Respnse_Event_c = 0xA8,
	g_Request_Mirror_Event_c,
	g_Remove_Mirror_Event_c,
	g_Get_Profile_Event_c,
	g_Request_Mirror_Response_Event_c,
    g_Remove_Mirror_Response_Event_c
#if(g_SE_PROFILE_1_1_d == 1)		
	  ,g_Request_Fast_Poll_Mode_Event_c
	,g_Request_Fast_Poll_Mode_Response_Event_c	
#endif   /* g_SE_PROFILE_1_1_d*/	
#if(g_SE_PROFILE_1_2_d == 1)
	,g_Get_Snapshot_Event_c
	,g_Take_Snapshot_Event_c
	,g_MirrorReport_Attribute_Response_Event_c
	,g_Schedule_SnapShot_Event_c
	,g_Start_Logging_Event_c
	,g_Get_Logging_Event_c
	,g_Reset_DemandLimit_Counter_Event_c
	,g_Get_Notification_Flag_Command_Event_c
    ,g_Get_Snapshot_Response_Event_c
	,g_Schedule_Snapshot_Response_Event_c
	,g_Get_Logging_Response_Event_c
	,g_Configure_Mirror_Event_c
	,g_Configure_Notification_Scheme_Event_c
	,g_Configure_Notification_Flag_Event_c
	
#endif /* g_SE_PROFILE_1_2_d*/	
}Simple_Metering_Event;

typedef enum Simple_Metering_Server_to_Client_tag
{
	g_Get_Profile_Response_c,
	g_Request_Mirror_c,
	g_Remove_Mirror_c
#if(g_SE_PROFILE_1_1_d == 1)		
	,g_Request_Fast_Poll_Mode_Response_c
#endif /* g_SE_PROFILE_1_1_d*/	
#if(g_SE_PROFILE_1_2_d == 1)
	
	,g_Get_Snapshot_Response_c
	,g_Schedule_Snapshot_Response_c
	,g_Get_Logging_Response_c
	,g_Configure_Mirror_c
	,g_Configure_Notification_Scheme_c
	,g_Configure_Notification_Flag_c

#endif	
}Simple_Metering_Server_Cluster_t;

typedef enum Simple_Metering_Client_to_Server_tag
{
	g_Get_Profile_c,
	g_Request_Mirror_Response_c,
	g_Remove_Mirror_Response_c
#if(g_SE_PROFILE_1_1_d == 1)		
	,g_Request_Fast_Poll_Mode_c	
#endif
#if(g_SE_PROFILE_1_2_d == 1)
	,g_Get_Snapshot_c
	,g_Take_Snapshot_c
	,g_MirrorReport_Attribute_Response_c
	,g_Schedule_SnapShot_c
	,g_Start_Logging_c
	,g_Get_Logging_c
	,g_Reset_DemandLimit_Counter_c
	,g_Get_Notification_Flag_Command_c
#endif	/* g_SE_PROFILE_1_2_d*/	

}Simple_Metering_Client_Cluster_t;

typedef enum Get_Profile_Response_Status
{
    g_MeteringIntervalSuccess_c                         = 0x00,
    g_MeteringUndefinedIntervalChannelRequested_c       = 0x01,
    g_MeteringInetervalChannelNotSupported_c            = 0x02,
    g_MeteringInvalidEndTime_c                          = 0x03,
    g_MeteringMorePeriodsRequestedThanCanBeReturned_c   = 0x04,
    g_MeteringNoIntervalsAvailableForTheRequestedTime_c = 0x05
}Get_Profile_Response_Status;

typedef enum Interval_Channel_Values
{
    g_MeteringConsumptionDelivered_c                    = 0x00,
    g_MeteringConsumptionReceived_c                     = 0x01
}Interval_Channel_Values;

typedef enum ProfileIntervalPeriod
{
    g_MeteringDaily_c                                  = 0x00,
    g_MeteringSixtyMinutes_c                           = 0x01,
    g_MeteringThirtyMinutes_c                          = 0x02,
    g_MeteringFifteenMinutes_c                         = 0x03,
    g_MeteringTenMinutes_c                             = 0x04,
    g_MeteringSevenAndHalfMinutes_c                    = 0x05,
    g_MeteringFiveMinutes_c                            = 0x06,
    g_MeteringTwoAndHalfMinutes_c                      = 0x07
}ProfileIntervalPeriod;

typedef enum CurrentBlock
{
	g_NoBlockInUse_c								=0x00,
	g_Block1_c										=0x01,
	g_Block2_c										=0x02,
	g_Block3_c										=0x03,
	g_Block4_c										=0x04,	
	g_Block5_c										=0x05,	
	g_Block6_c										=0x06,	
	g_Block7_c										=0x07,	
	g_Block8_c										=0x08,	
	g_Block9_c										=0x09,	
	g_Block10_c										=0x0a,	
	g_Block11_c										=0x0b,	
	g_Block12_c										=0x0c,	
	g_Block13_c										=0x0d,	
	g_Block14_c										=0x0e,	
	g_Block15_c										=0x0f,	
	g_Block16_c										=0x10		

}CurrentBlock;

typedef enum SupplySatus
{
	g_supply_off_c								    =0x00,
	g_supply_off_armed_c							=0x01,
	g_supply_on_c									=0x02	

}SupplyStatus;

typedef struct MeteringIntervals_tag
{
    uint8_t metringInterval[3];
}__attribute__((__packed__)) MeteringIntervals_t;

typedef struct Get_Profile_Response_Command_tag
{
	uint32_t endTime;
	uint8_t status;
	uint8_t profileIntervalPeriod;
	uint8_t noOfPeriods;
	MeteringIntervals_t intevals[1];
}__attribute__((__packed__))Get_Profile_Response_Command_t;

typedef struct Get_Profile_Command_tag
{
	uint8_t intervalChannel;
	uint32_t endTime;
	uint8_t noOfPeriods;
}Get_Profile_Command_t;

typedef struct Request_Mirror_Response_Command_tag
{
	uint8_t endpointId[2];
}__attribute__((__packed__))Request_Mirror_Response_Command_t;

typedef struct Remove_Mirror_Command_tag
{
	uint8_t removedEndpointId[2];
}__attribute__((__packed__))Remove_Mirror_Command_t;

#if(g_SE_PROFILE_1_1_d == 1)	
typedef struct Request_Fast_Poll_Mode_tag
{
	uint8_t fastPollUpdatePeriod;
	uint8_t duration;
}__attribute__((__packed__))Request_Fast_Poll_Mode_t;

typedef struct Request_Fast_Poll_Mode_Response_tag
{
	uint8_t appliedUpdatePeriod;
	uint8_t fastPollModeEndTime[4];
}__attribute__((__packed__))Request_Fast_Poll_Mode_Response_t;
#endif

#if(g_SE_PROFILE_1_2_d == 1)	
/* get snap shot command  payload format*/
typedef struct Get_Snapshot_command_tag
{
	uint32_t startTime;
	uint8_t noOfSnapshots;
	uint16_t snapshotCause;
}__attribute__((__packed__))Get_Snapshot_command_t;


typedef struct Get_Snapshot_Response_Command_tag
{
	uint32_t  eventId;
	uint32_t  snapshotTime;
	uint8_t   commandIndex;
	uint16_t  snapshotCause;
	uint8_t  snapshotPayloadType;
	
}__attribute__((__packed__))Get_Snapshot_Response_Command_t;

/* schedule snapshot  payload format*/


/*******************************************************************************
 sBitLocater_Tag

 This Structure is helpful in locating each bit data indication.

 NOTE : For optimum performance, in order utilise all the bit in the Byte
 Now this Structure is used for TX_OPTION and for Fragmentation Options

 *******************************************************************************/
//typedef union mBitLocater_Tag {
//    struct {
//        uint8_t BIT00 :1;
//        uint8_t BIT01 :1;
//        uint8_t BIT02 :1;
//        uint8_t BIT03 :1;
//        uint8_t BIT04 :1;
//        uint8_t BIT05 :1;
//        uint8_t BIT06 :1;
//        uint8_t BIT07 :1;
//		uint8_t BIT08 :1;
//        uint8_t BIT09 :1;
//        uint8_t BIT10 :1;
//        uint8_t BIT11 :1;
//        uint8_t BIT12 :1;
//        uint8_t BIT13 :1;
//        uint8_t BIT14 :1;
//        uint8_t BIT15 :1;
//    } iBits;
//    uint8_t u8State;
//} mBitLocater_t;
//
//mBitLocater_t snapshotcause;


/* Options used for automating Fragmentation purpose are as Defined below*/

#define SNAPSHOT_CAUSE_STATUS						snapshot_cause.iBits.status


#define SNAPSHOT_CAUSE_GENERAL						snapshot_cause.iBits.BIT00
#define	SNAPSHOT_CAUSE_EOF_BILLING_PERIOD			snapshot_cause.iBits.BIT01
#define	SNAPSHOT_CAUSE_EOF_BLOCK_PERIODL			snapshot_cause.iBits.BIT02
#define	SNAPSHOT_CAUSE_CHNG_OF_TIF			  		snapshot_cause.iBits.BIT03
#define	SNAPSHOT_CAUSE_CHNG_OF_PM  					snapshot_cause.iBits.BIT04
#define	SNAPSHOT_CAUSE_CHNG_OF_BT					snapshot_cause.iBits.BIT05
#define SNAPSHOT_CAUSE_CHNG_OF_CV					snapshot_cause.iBits.BIT06
#define SNAPSHOT_CAUSE_CHNG_OF_CF					snapshot_cause.iBits.BIT07
#define SNAPSHOT_CAUSE_CHNG_OF_TOUC                 snapshot_cause.iBits.BIT08
#define SNAPSHOT_CAUSE_CRITICAL_PP                  snapshot_cause.iBits.BIT09
#define SNAPSHOT_CAUSE_MTFC                     	snapshot_cause.iBits.BIT10
#define SNAPSHOT_CAUSE_EORP                   		snapshot_cause.iBits.BIT11
#define SNAPSHOT_CAUSE_CHNG_OF_TENENCY              snapshot_cause.iBits.BIT12
#define SNAPSHOT_CAUSE_CHNG_OF_SUPPLIER             snapshot_cause.iBits.BIT13
#define SNAPSHOT_CAUSE_CHNG_OF_MODE                 snapshot_cause.iBits.BIT14
#define SNAPSHOT_CAUSE_DEBT_PAYMENT                 snapshot_cause.iBits.BIT15


typedef struct snapshotPayload_tag{
	uint8_t snapshotScheduleId;
	uint32_t snapshotStartDate;
	uint8_t snapshotSchedule[3];
	uint8_t snapshotTypeCause;
}__attribute__((__packed__))snapshotPayload_t;

typedef struct Schedule_Snapshot_command_tag  {
	uint32_t eventId;
	uint8_t commandIndex;
	uint16_t snapshotCause;
	snapshotPayload_t snapshotPayload[10];
}__attribute__((__packed__))Schedule_Snapshot_command_t;

/*******************************************************************************/
/* start logging payload*/
/*******************************************************************************/
typedef struct start_logging_cmd_tag {
  uint32_t eventId;
  uint32_t startProfileTime;
  uint8_t logId;
  uint16_t periodDuration;
  uint16_t maxNoOfPeriods;
} __attribute__((__packed__))start_logging_cmd_t;
/*******************************************************************************/
/*get logging payload */
/*******************************************************************************/
typedef struct get_logging_cmd_tag {
  uint32_t eventId;
  uint32_t startProfileTime;
  uint8_t logId;
  uint16_t noOfPeriods;

}__attribute__((__packed__))get_logging_cmd_t;
/*******************************************************************************/
/* get logging response payload*/
/*******************************************************************************/
typedef struct get_logging_resp_cmd_tag {
  uint32_t eventId;
  uint32_t startProfileTime;
  uint8_t logId;
  uint16_t noOfPeriods;
  uint8_t intervals[3];
}__attribute__((__packed__)) get_logging_resp_cmd_t;
/*******************************************************************************/
#endif

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/
#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t ZCL_Check_Metering_Boundary_Values ( uint8_t *p_attribute_id,
                                    uint8_t *p_attribute_data,
                                    uint8_t data_length );
#endif                /*#if ( g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d == 1 ) */

uint8_t ZCL_Handle_Simple_Metering_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );



#endif                                 /*g_SIMPLE_METERING_CLUSTER_ENABLE_d*/
/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif   /* _ZCL_SIMPLE_METERING_CLUSTER_H_*/
