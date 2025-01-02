/******************************************************************************
* ZCL_Price_Cluster.h
******************************************************************************
* Program Description:
* This file contains the Price cluster attribute and cmmnd information
******************************************************************************/

#ifndef _ZCL_PRICE_CLUSTER_H_
#define _ZCL_PRICE_CLUSTER_H_

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#if ( g_PRICE_CLUSTER_ENABLE_d == 1 )
#define g_PRICE_CLUSTER_c                                     0x0700

#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
#define g_PRICE_TIER1_ATTRIBUTE_c                             0x0000
#define g_PRICE_TIER2_ATTRIBUTE_c                             0x0001
#define g_PRICE_TIER3_ATTRIBUTE_c                             0x0002
#define g_PRICE_TIER4_ATTRIBUTE_c                             0x0003
#define g_PRICE_TIER5_ATTRIBUTE_c                             0x0004
#define g_PRICE_TIER6_ATTRIBUTE_c                             0x0005

#if ( g_SE_PROFILE_1_1_d == 1)
#define g_PRICE_TIER7_ATTRIBUTE_c                             0x0006
#define g_PRICE_TIER8_ATTRIBUTE_c                             0x0007
#define g_PRICE_TIER9_ATTRIBUTE_c                             0x0008
#define g_PRICE_TIER10_ATTRIBUTE_c                            0x0009
#define g_PRICE_TIER11_ATTRIBUTE_c                            0x000a
#define g_PRICE_TIER12_ATTRIBUTE_c                            0x000b
#define g_PRICE_TIER13_ATTRIBUTE_c                            0x000c
#define g_PRICE_TIER14_ATTRIBUTE_c                            0x000d
#define g_PRICE_TIER15_ATTRIBUTE_c                            0x000e
#endif    /*g_SE_PROFILE_1_1_d */


#if ( g_SE_PROFILE_1_1_d == 1)

#define g_BLOCK1_THRESHOLD_ATTRIBUTE_c		              	  0x0100
#define g_BLOCK2_THRESHOLD_ATTRIBUTE_c           		  	  0x0101
#define g_BLOCK3_THRESHOLD_ATTRIBUTE_c           		 	  0x0102
#define g_BLOCK4_THRESHOLD_ATTRIBUTE_c           		      0x0103
#define g_BLOCK5_THRESHOLD_ATTRIBUTE_c           		      0x0104
#define g_BLOCK6_THRESHOLD_ATTRIBUTE_c           		      0x0105
#define g_BLOCK7_THRESHOLD_ATTRIBUTE_c           		      0x0106
#define g_BLOCK8_THRESHOLD_ATTRIBUTE_c           		      0x0107
#define g_BLOCK9_THRESHOLD_ATTRIBUTE_c           		      0x0108
#define g_BLOCK10_THRESHOLD_ATTRIBUTE_c           		      0x0109
#define g_BLOCK11_THRESHOLD_ATTRIBUTE_c           	 	      0x010a
#define g_BLOCK12_THRESHOLD_ATTRIBUTE_c           	  	      0x010b
#define g_BLOCK13_THRESHOLD_ATTRIBUTE_c           	 	      0x010c
#define g_BLOCK14_THRESHOLD_ATTRIBUTE_c           	  	      0x010d
#define g_BLOCK15_THRESHOLD_ATTRIBUTE_c           	 	      0x010e
#endif    /*g_SE_PROFILE_1_1_d */

#if ( g_SE_PROFILE_1_1_d == 1)

#define g_START_OF_BLOCK_PERIOD_ATTRIBUTE_c           	 	  0x0200
#define g_BLOCK_PERIOD_DURATION_ATTRIBUTE_c           	 	  0x0201
#define g_THRESHOLD_MULTIPLIER_ATTRIBUTE_c           	 	  0x0202
#define g_THRESHOLD_DEVISOR_ATTRIBUTE_c           	 	 	  0x0203
#endif    /* g_SE_PROFILE_1_1_d*/

#if ( g_SE_PROFILE_1_1_d == 1)

#define g_SERVER_COMMODITY_TYPE_ATTRIBUTE_c           	 	  0x0300
#define g_STANDING_CHARGE_ATTRIBUTE_c           	 	 	  0x0301
#endif    /* g_SE_PROFILE_1_1_d*/



#if ( g_SE_PROFILE_1_2_d == 1)
/*SE 1.1 A update****/
#define g_CONVERSION_FACTOR_ATTRIBUTE_c								0x0302
#define g_CONVERSION_FACTOR_TRAILING_DIGIT_ATTRIBUTE_c  			0x0303
#define g_CALORIFIC_VALUE_ATTRIBUTE_c								0x0304
#define g_CALORIFIC_VALUE_UNIT_ATTRIBUTE_c      					0x0305
#define g_CALORIFIC_VALUE_TRAILING_DIGIT_ATTRIBUTE_c    			0x0306	
#endif /*g_SE_PROFILE_1_2_d */


#if ( g_SE_PROFILE_1_1_d == 1)
#define g_PRICE_NO_TIER_BLOCK1_ATTRIBUTE_c           	 	 	  0x0400
#define g_PRICE_NO_TIER_BLOCK2_ATTRIBUTE_c           	 	 	  0x0401
#define g_PRICE_NO_TIER_BLOCK3_ATTRIBUTE_c           	 	 	  0x0402
#define g_PRICE_NO_TIER_BLOCK4_ATTRIBUTE_c           	  		  0x0403
#define g_PRICE_NO_TIER_BLOCK5_ATTRIBUTE_c           	 	 	  0x0404
#define g_PRICE_NO_TIER_BLOCK6_ATTRIBUTE_c           	 	 	  0x0405
#define g_PRICE_NO_TIER_BLOCK7_ATTRIBUTE_c           	 	 	  0x0406
#define g_PRICE_NO_TIER_BLOCK8_ATTRIBUTE_c           	  		  0x0407
#define g_PRICE_NO_TIER_BLOCK9_ATTRIBUTE_c           	 	 	  0x0408
#define g_PRICE_NO_TIER_BLOCK10_ATTRIBUTE_c           	 	 	  0x0409
#define g_PRICE_NO_TIER_BLOCK11_ATTRIBUTE_c           	 	 	  0x040a
#define g_PRICE_NO_TIER_BLOCK12_ATTRIBUTE_c           	  		  0x040b
#define g_PRICE_NO_TIER_BLOCK13_ATTRIBUTE_c           	 	 	  0x040e
#define g_PRICE_NO_TIER_BLOCK14_ATTRIBUTE_c           	 	 	  0x040d
#define g_PRICE_NO_TIER_BLOCK15_ATTRIBUTE_c           	  		  0x040e
#define g_PRICE_NO_TIER_BLOCK16_ATTRIBUTE_c           	 	 	  0x040f


#define g_PRICE_TIER1_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0410
#define g_PRICE_TIER1_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0411
#define g_PRICE_TIER1_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0412
#define g_PRICE_TIER1_BLOCK4_ATTRIBUTE_c           	  		  	 0x0413
#define g_PRICE_TIER1_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0414
#define g_PRICE_TIER1_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0415
#define g_PRICE_TIER1_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0416
#define g_PRICE_TIER1_BLOCK8_ATTRIBUTE_c           	  		 	 0x0417
#define g_PRICE_TIER1_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0418
#define g_PRICE_TIER1_BLOCK10_ATTRIBUTE_c           	 	 	 0x0419
#define g_PRICE_TIER1_BLOCK11_ATTRIBUTE_c           	 	 	 0x041a
#define g_PRICE_TIER1_BLOCK12_ATTRIBUTE_c           	  		 0x041b
#define g_PRICE_TIER1_BLOCK13_ATTRIBUTE_c           	 	 	 0x041e
#define g_PRICE_TIER1_BLOCK14_ATTRIBUTE_c           	 	 	 0x041d
#define g_PRICE_TIER1_BLOCK15_ATTRIBUTE_c           	  		 0x041e
#define g_PRICE_TIER1_BLOCK16_ATTRIBUTE_c           	 	 	 0x041f


#define g_PRICE_TIER2_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0420
#define g_PRICE_TIER2_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0421
#define g_PRICE_TIER2_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0422
#define g_PRICE_TIER2_BLOCK4_ATTRIBUTE_c           	  		  	 0x0423
#define g_PRICE_TIER2_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0424
#define g_PRICE_TIER2_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0425
#define g_PRICE_TIER2_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0426
#define g_PRICE_TIER2_BLOCK8_ATTRIBUTE_c           	  		 	 0x0427
#define g_PRICE_TIER2_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0428
#define g_PRICE_TIER2_BLOCK10_ATTRIBUTE_c           	 	 	  0x0429
#define g_PRICE_TIER2_BLOCK11_ATTRIBUTE_c           	 	 	  0x042a
#define g_PRICE_TIER2_BLOCK12_ATTRIBUTE_c           	  		  0x042b
#define g_PRICE_TIER2_BLOCK13_ATTRIBUTE_c           	 	 	  0x042e
#define g_PRICE_TIER2_BLOCK14_ATTRIBUTE_c           	 	 	  0x042d
#define g_PRICE_TIER2_BLOCK15_ATTRIBUTE_c           	  		  0x042e
#define g_PRICE_TIER2_BLOCK16_ATTRIBUTE_c           	 	 	  0x042f

#define g_PRICE_TIER3_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0430
#define g_PRICE_TIER3_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0431
#define g_PRICE_TIER3_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0432
#define g_PRICE_TIER3_BLOCK4_ATTRIBUTE_c           	  		  	 0x0433
#define g_PRICE_TIER3_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0434
#define g_PRICE_TIER3_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0435
#define g_PRICE_TIER3_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0436
#define g_PRICE_TIER3_BLOCK8_ATTRIBUTE_c           	  		 	 0x0437
#define g_PRICE_TIER3_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0438
#define g_PRICE_TIER3_BLOCK10_ATTRIBUTE_c           	 	 	 0x0439
#define g_PRICE_TIER3_BLOCK11_ATTRIBUTE_c           	 	 	 0x043a
#define g_PRICE_TIER3_BLOCK12_ATTRIBUTE_c           	  		 0x043b
#define g_PRICE_TIER3_BLOCK13_ATTRIBUTE_c           	 	 	 0x043e
#define g_PRICE_TIER3_BLOCK14_ATTRIBUTE_c           	 	 	 0x043d
#define g_PRICE_TIER3_BLOCK15_ATTRIBUTE_c           	  		 0x043e
#define g_PRICE_TIER3_BLOCK16_ATTRIBUTE_c           	 	 	 0x043f


#define g_PRICE_TIER4_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0440
#define g_PRICE_TIER4_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0441
#define g_PRICE_TIER4_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0442
#define g_PRICE_TIER4_BLOCK4_ATTRIBUTE_c           	  		  	 0x0443
#define g_PRICE_TIER4_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0444
#define g_PRICE_TIER4_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0445
#define g_PRICE_TIER4_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0446
#define g_PRICE_TIER4_BLOCK8_ATTRIBUTE_c           	  		 	 0x0447
#define g_PRICE_TIER4_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0448
#define g_PRICE_TIER4_BLOCK10_ATTRIBUTE_c           	 	 	 0x0449
#define g_PRICE_TIER4_BLOCK11_ATTRIBUTE_c           	 	 	 0x044a
#define g_PRICE_TIER4_BLOCK12_ATTRIBUTE_c           	  		 0x044b
#define g_PRICE_TIER4_BLOCK13_ATTRIBUTE_c           	 	 	 0x044e
#define g_PRICE_TIER4_BLOCK14_ATTRIBUTE_c           	 	 	 0x044d
#define g_PRICE_TIER4_BLOCK15_ATTRIBUTE_c           	  		 0x044e
#define g_PRICE_TIER4_BLOCK16_ATTRIBUTE_c           	 	 	 0x044f


#define g_PRICE_TIER5_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0450
#define g_PRICE_TIER5_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0451
#define g_PRICE_TIER5_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0452
#define g_PRICE_TIER5_BLOCK4_ATTRIBUTE_c           	  		  	 0x0453
#define g_PRICE_TIER5_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0454
#define g_PRICE_TIER5_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0455
#define g_PRICE_TIER5_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0456
#define g_PRICE_TIER5_BLOCK8_ATTRIBUTE_c           	  		 	 0x0457
#define g_PRICE_TIER5_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0458
#define g_PRICE_TIER5_BLOCK10_ATTRIBUTE_c           	 	 	 0x0459
#define g_PRICE_TIER5_BLOCK11_ATTRIBUTE_c           	 	 	 0x045a
#define g_PRICE_TIER5_BLOCK12_ATTRIBUTE_c           	  		 0x045b
#define g_PRICE_TIER5_BLOCK13_ATTRIBUTE_c           	 	 	 0x045e
#define g_PRICE_TIER5_BLOCK14_ATTRIBUTE_c           	 	 	 0x045d
#define g_PRICE_TIER5_BLOCK15_ATTRIBUTE_c           	  		 0x045e
#define g_PRICE_TIER5_BLOCK16_ATTRIBUTE_c           	 	 	 0x045f

#define g_PRICE_TIER6_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0460
#define g_PRICE_TIER6_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0461
#define g_PRICE_TIER6_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0462
#define g_PRICE_TIER6_BLOCK4_ATTRIBUTE_c           	  		  	 0x0463
#define g_PRICE_TIER6_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0464
#define g_PRICE_TIER6_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0465
#define g_PRICE_TIER6_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0466
#define g_PRICE_TIER6_BLOCK8_ATTRIBUTE_c           	  		 	 0x0467
#define g_PRICE_TIER6_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0468
#define g_PRICE_TIER6_BLOCK10_ATTRIBUTE_c           	 	 	 0x0469
#define g_PRICE_TIER6_BLOCK11_ATTRIBUTE_c           	 	 	 0x046a
#define g_PRICE_TIER6_BLOCK12_ATTRIBUTE_c           	  		 0x046b
#define g_PRICE_TIER6_BLOCK13_ATTRIBUTE_c           	 	 	 0x046e
#define g_PRICE_TIER6_BLOCK14_ATTRIBUTE_c           	 	 	 0x046d
#define g_PRICE_TIER6_BLOCK15_ATTRIBUTE_c           	  		 0x046e
#define g_PRICE_TIER6_BLOCK16_ATTRIBUTE_c           	 	 	 0x046f

#define g_PRICE_TIER7_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0470
#define g_PRICE_TIER7_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0471
#define g_PRICE_TIER7_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0472
#define g_PRICE_TIER7_BLOCK4_ATTRIBUTE_c           	  		  	 0x0473
#define g_PRICE_TIER7_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0474
#define g_PRICE_TIER7_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0475
#define g_PRICE_TIER7_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0476
#define g_PRICE_TIER7_BLOCK8_ATTRIBUTE_c           	  		 	 0x0477
#define g_PRICE_TIER7_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0478
#define g_PRICE_TIER7_BLOCK10_ATTRIBUTE_c           	 	 	 0x0479
#define g_PRICE_TIER7_BLOCK11_ATTRIBUTE_c           	 	 	 0x047a
#define g_PRICE_TIER7_BLOCK12_ATTRIBUTE_c           	  		 0x047b
#define g_PRICE_TIER7_BLOCK13_ATTRIBUTE_c           	 	 	 0x047e
#define g_PRICE_TIER7_BLOCK14_ATTRIBUTE_c           	 	 	 0x047d
#define g_PRICE_TIER7_BLOCK15_ATTRIBUTE_c           	  		 0x047e
#define g_PRICE_TIER7_BLOCK16_ATTRIBUTE_c           	 	 	 0x047f

#define g_PRICE_TIER8_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0480
#define g_PRICE_TIER8_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0481
#define g_PRICE_TIER8_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0482
#define g_PRICE_TIER8_BLOCK4_ATTRIBUTE_c           	  		  	 0x0483
#define g_PRICE_TIER8_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0484
#define g_PRICE_TIER8_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0485
#define g_PRICE_TIER8_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0486
#define g_PRICE_TIER8_BLOCK8_ATTRIBUTE_c           	  		 	 0x0487
#define g_PRICE_TIER8_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0488
#define g_PRICE_TIER8_BLOCK10_ATTRIBUTE_c           	 	 	 0x0489
#define g_PRICE_TIER8_BLOCK11_ATTRIBUTE_c           	 	 	 0x048a
#define g_PRICE_TIER8_BLOCK12_ATTRIBUTE_c           	  		 0x048b
#define g_PRICE_TIER8_BLOCK13_ATTRIBUTE_c           	 	 	 0x048e
#define g_PRICE_TIER8_BLOCK14_ATTRIBUTE_c           	 	 	 0x048d
#define g_PRICE_TIER8_BLOCK15_ATTRIBUTE_c           	  		 0x048e
#define g_PRICE_TIER8_BLOCK16_ATTRIBUTE_c           	 	 	 0x048f

#define g_PRICE_TIER9_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x0490
#define g_PRICE_TIER9_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x0491
#define g_PRICE_TIER9_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x0492
#define g_PRICE_TIER9_BLOCK4_ATTRIBUTE_c           	  		  	 0x0493
#define g_PRICE_TIER9_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x0494
#define g_PRICE_TIER9_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x0495
#define g_PRICE_TIER9_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x0496
#define g_PRICE_TIER9_BLOCK8_ATTRIBUTE_c           	  		 	 0x0497
#define g_PRICE_TIER9_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x0498
#define g_PRICE_TIER9_BLOCK10_ATTRIBUTE_c           	 	 	 0x0499
#define g_PRICE_TIER9_BLOCK11_ATTRIBUTE_c           	 	 	 0x049a
#define g_PRICE_TIER9_BLOCK12_ATTRIBUTE_c           	  		 0x049b
#define g_PRICE_TIER9_BLOCK13_ATTRIBUTE_c           	 	 	 0x049e
#define g_PRICE_TIER9_BLOCK14_ATTRIBUTE_c           	 	 	 0x049d
#define g_PRICE_TIER9_BLOCK15_ATTRIBUTE_c           	  		 0x049e
#define g_PRICE_TIER9_BLOCK16_ATTRIBUTE_c           	 	 	 0x049f

#define g_PRICE_TIER10_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x04a0
#define g_PRICE_TIER10_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x04a1
#define g_PRICE_TIER10_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x04a2
#define g_PRICE_TIER10_BLOCK4_ATTRIBUTE_c           	  		  	 0x04a3
#define g_PRICE_TIER10_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x04a4
#define g_PRICE_TIER10_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x04a5
#define g_PRICE_TIER10_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x04a6
#define g_PRICE_TIER10_BLOCK8_ATTRIBUTE_c           	  		 	 0x04a7
#define g_PRICE_TIER10_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x04a8
#define g_PRICE_TIER10_BLOCK10_ATTRIBUTE_c           	 	 		 0x04a9
#define g_PRICE_TIER10_BLOCK11_ATTRIBUTE_c           	 	 	 	 0x04aa
#define g_PRICE_TIER10_BLOCK12_ATTRIBUTE_c           	  		 	 0x04ab
#define g_PRICE_TIER10_BLOCK13_ATTRIBUTE_c           	 	 	 	 0x04ae
#define g_PRICE_TIER10_BLOCK14_ATTRIBUTE_c           	 	 		 0x04ad
#define g_PRICE_TIER10_BLOCK15_ATTRIBUTE_c           	  		 	 0x04ae
#define g_PRICE_TIER10_BLOCK16_ATTRIBUTE_c           	 	 	 	 0x04af

#define g_PRICE_TIER11_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x04b0
#define g_PRICE_TIER11_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x04b1
#define g_PRICE_TIER11_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x04b2
#define g_PRICE_TIER11_BLOCK4_ATTRIBUTE_c           	  		  	 0x04b3
#define g_PRICE_TIER11_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x04b4
#define g_PRICE_TIER11_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x04b5
#define g_PRICE_TIER11_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x04b6
#define g_PRICE_TIER11_BLOCK8_ATTRIBUTE_c           	  		 	 0x04b7
#define g_PRICE_TIER11_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x04b8
#define g_PRICE_TIER11_BLOCK10_ATTRIBUTE_c           	 	 		 0x04b9
#define g_PRICE_TIER11_BLOCK11_ATTRIBUTE_c           	 	 	 	 0x04ba
#define g_PRICE_TIER11_BLOCK12_ATTRIBUTE_c           	  		 	 0x04bb
#define g_PRICE_TIER11_BLOCK13_ATTRIBUTE_c           	 	 	 	 0x04be
#define g_PRICE_TIER11_BLOCK14_ATTRIBUTE_c           	 	 		 0x04bd
#define g_PRICE_TIER11_BLOCK15_ATTRIBUTE_c           	  		 	 0x04be
#define g_PRICE_TIER11_BLOCK16_ATTRIBUTE_c           	 	 	 	 0x04bf

#define g_PRICE_TIER12_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x04c0
#define g_PRICE_TIER12_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x04c1
#define g_PRICE_TIER12_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x04c2
#define g_PRICE_TIER12_BLOCK4_ATTRIBUTE_c           	  		  	 0x04c3
#define g_PRICE_TIER12_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x04c4
#define g_PRICE_TIER12_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x04c5
#define g_PRICE_TIER12_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x04c6
#define g_PRICE_TIER12_BLOCK8_ATTRIBUTE_c           	  		 	 0x04c7
#define g_PRICE_TIER12_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x04c8
#define g_PRICE_TIER12_BLOCK10_ATTRIBUTE_c           	 	 		 0x04c9
#define g_PRICE_TIER12_BLOCK11_ATTRIBUTE_c           	 	 	 	 0x04ca
#define g_PRICE_TIER12_BLOCK12_ATTRIBUTE_c           	  		 	 0x04cb
#define g_PRICE_TIER12_BLOCK13_ATTRIBUTE_c           	 	 	 	 0x04ce
#define g_PRICE_TIER12_BLOCK14_ATTRIBUTE_c           	 	 		 0x04cd
#define g_PRICE_TIER12_BLOCK15_ATTRIBUTE_c           	  		 	 0x04ce
#define g_PRICE_TIER12_BLOCK16_ATTRIBUTE_c           	 	 	 	 0x04cf

#define g_PRICE_TIER13_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x04d0
#define g_PRICE_TIER13_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x04d1
#define g_PRICE_TIER13_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x04d2
#define g_PRICE_TIER13_BLOCK4_ATTRIBUTE_c           	  		  	 0x04d3
#define g_PRICE_TIER13_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x04d4
#define g_PRICE_TIER13_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x04d5
#define g_PRICE_TIER13_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x04d6
#define g_PRICE_TIER13_BLOCK8_ATTRIBUTE_c           	  		 	 0x04d7
#define g_PRICE_TIER13_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x04d8
#define g_PRICE_TIER13_BLOCK10_ATTRIBUTE_c           	 	 		 0x04d9
#define g_PRICE_TIER13_BLOCK11_ATTRIBUTE_c           	 	 	 	 0x04da
#define g_PRICE_TIER13_BLOCK12_ATTRIBUTE_c           	  		 	 0x04db
#define g_PRICE_TIER13_BLOCK13_ATTRIBUTE_c           	 	 	 	 0x04de
#define g_PRICE_TIER13_BLOCK14_ATTRIBUTE_c           	 	 		 0x04dd
#define g_PRICE_TIER13_BLOCK15_ATTRIBUTE_c           	  		 	 0x04de
#define g_PRICE_TIER13_BLOCK16_ATTRIBUTE_c           	 	 	 	 0x04df


#define g_PRICE_TIER14_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x04e0
#define g_PRICE_TIER14_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x04e1
#define g_PRICE_TIER14_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x04e2
#define g_PRICE_TIER14_BLOCK4_ATTRIBUTE_c           	  		  	 0x04e3
#define g_PRICE_TIER14_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x04e4
#define g_PRICE_TIER14_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x04e5
#define g_PRICE_TIER14_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x04e6
#define g_PRICE_TIER14_BLOCK8_ATTRIBUTE_c           	  		 	 0x04e7
#define g_PRICE_TIER14_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x04e8
#define g_PRICE_TIER14_BLOCK10_ATTRIBUTE_c           	 	 		 0x04e9
#define g_PRICE_TIER14_BLOCK11_ATTRIBUTE_c           	 	 	 	 0x04ea
#define g_PRICE_TIER14_BLOCK12_ATTRIBUTE_c           	  		 	 0x04eb
#define g_PRICE_TIER14_BLOCK13_ATTRIBUTE_c           	 	 	 	 0x04ee
#define g_PRICE_TIER14_BLOCK14_ATTRIBUTE_c           	 	 		 0x04ed
#define g_PRICE_TIER14_BLOCK15_ATTRIBUTE_c           	  		 	 0x04ee
#define g_PRICE_TIER14_BLOCK16_ATTRIBUTE_c           	 	 	 	 0x04ef

#define g_PRICE_TIER15_BLOCK1_ATTRIBUTE_c           	 	 	  	 0x04f0
#define g_PRICE_TIER15_BLOCK2_ATTRIBUTE_c           	 	 	 	 0x04f1
#define g_PRICE_TIER15_BLOCK3_ATTRIBUTE_c           	 	 	  	 0x04f2
#define g_PRICE_TIER15_BLOCK4_ATTRIBUTE_c           	  		  	 0x04f3
#define g_PRICE_TIER15_BLOCK5_ATTRIBUTE_c           	 	 	 	 0x04f4
#define g_PRICE_TIER15_BLOCK6_ATTRIBUTE_c           	 	 	 	 0x04f5
#define g_PRICE_TIER15_BLOCK7_ATTRIBUTE_c           	 	 	  	 0x04f6
#define g_PRICE_TIER15_BLOCK8_ATTRIBUTE_c           	  		 	 0x04f7
#define g_PRICE_TIER15_BLOCK9_ATTRIBUTE_c           	 	 	  	 0x04f8
#define g_PRICE_TIER15_BLOCK10_ATTRIBUTE_c           	 	 		 0x04f9
#define g_PRICE_TIER15_BLOCK11_ATTRIBUTE_c           	 	 	 	 0x04fa
#define g_PRICE_TIER15_BLOCK12_ATTRIBUTE_c           	  		 	 0x04fb
#define g_PRICE_TIER15_BLOCK13_ATTRIBUTE_c           	 	 	 	 0x04fe
#define g_PRICE_TIER15_BLOCK14_ATTRIBUTE_c           	 	 		 0x04fd
#define g_PRICE_TIER15_BLOCK15_ATTRIBUTE_c           	  		 	 0x04fe
#define g_PRICE_TIER15_BLOCK16_ATTRIBUTE_c           	 	 	 	 0x04ff
#endif /* g_SE_PROFILE_1_1_d*/
#endif      /* g_PRICE_SERVER_CLUSTER_ENABLE_d*/

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
#if ( g_SE_PROFILE_1_1_d == 1 )
#define g_PRICE_INCREASE_RANDOMIZE_MINUTES_ATTRIBUTE_c     	 	 	 0x0000
#define g_PRICE_DECREASE_RANDOMIZE_MINUTES_ATTRIBUTE_c        	 	 0x0001
#define g_CLIENT_COMMODITY_TYPE_ATTRIBUTE_c           	 	 	 	         0x0002
#endif  /* g_SE_PROFILE_1_1_d*/
#endif   /* g_PRICE_CLIENT_CLUSTER_ENABLE_d*/


#define g_CURRENT_TIME_c                                      0x00000000
#define g_DURATION_UNTIL_CHANGED_c                            0xFFFF
#define g_PRICE_CMD_LENGTH_FROM_ISSUER_EVENT_ID_c             0x23

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Price_Server_Cluster_Info;
#endif

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
extern const ZCL_Cluster_Info_t ga_Price_Client_Cluster_Info;
#endif


/*Commands generated by Server Side of the Price Cluster*/
enum PriceClusterServerGeneratedCmds
{
    g_PublishPriceCmd_c                   = 0x00
#if ( g_SE_PROFILE_1_1_d == 1 )	
	,g_PublishBlockPeriodCmd_c			  = 0x01
#endif	 /*  ( g_SE_PROFILE_1_1_d == 1 )*/   	
/*  Updates of SE 1.1 A  */
#if ( g_SE_PROFILE_1_2_d == 1 )	
	,g_PublishConversionFactor_c		  = 0x02
	,g_PublishCalorificValue_c			  = 0x03
#endif	 /*  ( g_SE_PROFILE_1_2_d == 1 )*/	
};

/*Commands received by Server Side of the Price Cluster*/
enum PriceClusterServerReceivedCmds
{
    g_GetCurrentPriceCmd_c                = 0x00,
    g_GetScheduledPriceCmd_c              = 0x01
#if ( g_SE_PROFILE_1_1_d == 1 )	
	 ,	g_PriceAcknwoledgementCmd_c		  = 0x02
	,g_GetBlockPeriodsCmd_c				  = 0x03
#endif	/* ( g_SE_PROFILE_1_1_d == 1 ) */ 	
/**   Updates of SE 1.1 A **/	
#if(g_SE_PROFILE_1_2_d == 1)
    ,g_GetConversionFactor_c	 = 0x04
	,g_GetCalorificValue_c		=  0x05
#endif	 /*  ( g_SE_PROFILE_1_2_d == 1 )*/
};

/*Events handled by the application*/
enum PriceClusterEvents
{
    g_GetCurrentPriceEvent_c              = 0x95,
    g_GetScheduledPricesEvent_c           = 0x96,
    g_PublishPriceEvent_c                 = 0x97

#if ( g_SE_PROFILE_1_1_d == 1 )	
	,g_PublishBlockPeriodEvent_c		  = 0x98,
	g_PriceAcknowledgementEvent_c		  = 0x99,
	g_GetBlockPeriodsEvent_c			  = 0x9a
#endif /* ( g_SE_PROFILE_1_1_d == 1 )*/
/**	  Updates of SE 1.1 A     */
#if(g_SE_PROFILE_1_2_d == 1)
	,g_PublishConversionFactorEvent_c		 = 0x9b
	,g_PublishCalorificValueEvent_c			 = 0x9c
    ,g_GetConversionFactorEvent_c			 = 0x9d
	,g_GetCalorificValueEvent_c				 = 0x9e
#endif	 /*  ( g_SE_PROFILE_1_2_d == 1 )*/	
	
};

/*-----------------------------------------------------------------------------
* Type Definitions
*----------------------------------------------------------------------------*/
#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )
typedef struct PriceClusterAttribute_Tag
{  	
#if ( g_PRICE_TIER1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier1PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif	
#if ( g_PRICE_TIER2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier2PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif	
#if ( g_PRICE_TIER3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier3PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif
#if ( g_PRICE_TIER4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier4PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif	
#if ( g_PRICE_TIER5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier5PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif
#if ( g_PRICE_TIER6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier6PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif	
#if ( g_SE_PROFILE_1_1_d == 1 )	
/* updates to the SE 1.1 for Tier Label set*/	
#if ( g_PRICE_TIER7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier7PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER7_ATTRIBUTE_ENABLE_d*/
	
#if ( g_PRICE_TIER8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier8PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER8_ATTRIBUTE_ENABLE_d*/
	
#if ( g_PRICE_TIER9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier9PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER9_ATTRIBUTE_ENABLE_d*/
	
#if ( g_PRICE_TIER10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier10PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER10_ATTRIBUTE_ENABLE_d*/
	
#if ( g_PRICE_TIER11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier11PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER11_ATTRIBUTE_ENABLE_d*/
	
#if ( g_PRICE_TIER12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier12PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER12_ATTRIBUTE_ENABLE_d*/
	
#if ( g_PRICE_TIER13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier13PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER13_ATTRIBUTE_ENABLE_d*/
	
#if ( g_PRICE_TIER14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier14PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER14_ATTRIBUTE_ENABLE_d*/
	
#if ( g_PRICE_TIER15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t tier15PriceLabel[g_LENGTH_FOR_OCTET_STRING_DATA_TYPE_c];
#endif                          /* g_PRICE_TIER15_ATTRIBUTE_ENABLE_d*/

#endif          /* g_SE_PROFILE_1_1_d*/	

#if ( g_SE_PROFILE_1_1_d == 1 )			
/* updates to the SE 1.1 for Block Threshold set*/	
#if (g_BLOCK1_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block1Threshold[6];
#endif
	
#if (g_BLOCK2_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block2Threshold[6];
#endif
	
#if (g_BLOCK3_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block3Threshold[6];
#endif
	
#if (g_BLOCK4_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block4Threshold[6];
#endif
	
#if (g_BLOCK5_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block5Threshold[6];
#endif
	
#if (g_BLOCK6_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block6Threshold[6];
#endif
	
#if (g_BLOCK7_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block7Threshold[6];
#endif
	
#if (g_BLOCK8_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block8Threshold[6];
#endif
	
#if (g_BLOCK9_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block9Threshold[6];
#endif
	
#if (g_BLOCK10_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block10Threshold[6];
#endif
	
#if (g_BLOCK11_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block11Threshold[6];
#endif
	
#if (g_BLOCK12_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block12Threshold[6];
#endif
	
#if (g_BLOCK13_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block13Threshold[6];
#endif
	
#if (g_BLOCK14_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block14Threshold[6];
#endif
	
#if (g_BLOCK15_THRESHOLD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  block15Threshold[6];
#endif
	
#endif /*   g_SE_PROFILE_1_1_d     */	
	
#if ( g_SE_PROFILE_1_1_d == 1 )			
/* updates to the SE 1.1 for Block Period set*/	
	
#if (g_START_OF_BLOCK_PERIOD_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  startOfBlockPeriod[4];
#endif
#if (g_BLOCK_PERIOD_DURATION_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  blockPeriodDuration[3];
	
#endif
#if (g_THRESHOLD_MULTIPLIER_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  thresholdMultiplier[3];
#endif
	
#if (g_THRESHOLD_DEVISOR_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  thresholdDevisor[3];
#endif

#endif  /* g_SE_PROFILE_1_1_d*/
	
#if ( g_SE_PROFILE_1_1_d == 1 )			
/* updates to the SE 1.1 for commodity set*/	
	
#if (g_SERVER_COMMODITY_TYPE_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  serverCommodityType;
#endif
	
#if (g_STANDING_CHARGE_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  standingCharge[4];
#endif 	
#endif  /*g_SE_PROFILE_1_1_d */	
	
#if ( g_SE_PROFILE_1_2_d == 1 )			
/* updates to the SE 1.1 A for commodity set*/
	
#if (g_CONVERSION_FACTOR_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  conversionFactor[4];
#endif
	
#if (g_CONVERSION_FACTOR_TRAILING_DIGIT_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  conversionFactorTrailingDigit[4];
#endif

#if (g_CALORIFIC_VALUE_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  calorificValue[4];
#endif
	
#if (g_CALORIFIC_VALUE_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  calorificValueUnit;
#endif
	
#if (g_CALORIFIC_VALUE_TRAILING_DIGIT_ATTRIBUTE_ENABLE_d== 1)
	uint8_t  calorificValueTrailingDigit[4];
#endif 	 	

#endif         /*g_SE_PROFILE_1_2_d */	

#if ( g_SE_PROFILE_1_1_d == 1 )			
/* updates to the SE 1.1 for Block Price Information set*/		
	
#if ( g_PRICE_NO_TIER_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock1[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock2[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock3[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock4[4];
#endif
#if ( g_PRICE_NO_TIER_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock5[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock6[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock7[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock8[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock9[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock10[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock11[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock12[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock13[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock14[4];
#endif
	
#if ( g_PRICE_NO_TIER_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock15[4];
#endif	
	
#if ( g_PRICE_NO_TIER_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceNoTierBlock16[4];
#endif	
	
#if ( g_PRICE_TIER1_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block1[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block2[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block3[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block4[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block5[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block6[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block7[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block8[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block9[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block10[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block11[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block12[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block13[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block14[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block15[4];
#endif
	
#if ( g_PRICE_TIER1_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier1Block16[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block1[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block2[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block3[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block4[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block5[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block6[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block7[4];
#endif
#if ( g_PRICE_TIER2_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block8[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block9[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block10[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block11[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block12[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block13[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block14[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block15[4];
#endif
	
#if ( g_PRICE_TIER2_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier2Block16[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block1[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block2[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block3[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block4[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block5[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block6[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block7[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block8[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block9[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block10[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block11[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block12[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block13[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block14[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block15[4];
#endif
	
#if ( g_PRICE_TIER3_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier3Block16[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block1[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block2[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block3[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block4[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block5[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block6[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block7[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block8[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block9[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block10[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block11[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block12[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block13[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block14[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block15[4];
#endif
	
#if ( g_PRICE_TIER4_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier4Block16[4];
#endif	
	
#if ( g_PRICE_TIER5_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block1[4];
#endif
	
#if ( g_PRICE_TIER5_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block2[4];
#endif
	
#if ( g_PRICE_TIER5_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block3[4];
#endif
	
#if ( g_PRICE_TIER5_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block4[4];
#endif
	
#if ( g_PRICE_TIER5_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block5[4];
#endif
	
#if ( g_PRICE_TIER5_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block6[4];
#endif
	
#if ( g_PRICE_TIER5_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block7[4];
#endif
	
#if ( g_PRICE_TIER5_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block8[4];
#endif
#if ( g_PRICE_TIER5_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block9[4];
#endif
	
#if ( g_PRICE_TIER5_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block10[4];
#endif
#if ( g_PRICE_TIER5_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block11[4];
#endif
#if ( g_PRICE_TIER5_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block12[4];
#endif
#if ( g_PRICE_TIER5_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block13[4];
#endif
#if ( g_PRICE_TIER5_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block14[4];
#endif
#if ( g_PRICE_TIER5_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block15[4];
#endif
#if ( g_PRICE_TIER5_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier5Block16[4];
#endif	
	
#if ( g_PRICE_TIER6_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block1[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block2[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block3[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block4[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block5[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block6[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block7[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block8[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block9[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block10[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block11[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block12[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block13[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block14[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block15[4];
#endif
	
#if ( g_PRICE_TIER6_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier6Block16[4];
#endif
	
#if ( g_PRICE_TIER7_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block1[4];
#endif
#if ( g_PRICE_TIER7_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block2[4];
#endif
	
#if ( g_PRICE_TIER7_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block3[4];
#endif
	
#if ( g_PRICE_TIER7_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block4[4];
#endif
	
#if ( g_PRICE_TIER7_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block5[4];
#endif
	
#if ( g_PRICE_TIER7_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block6[4];
#endif
#if ( g_PRICE_TIER7_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block7[4];
#endif
#if ( g_PRICE_TIER7_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block8[4];
#endif
#if ( g_PRICE_TIER7_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block9[4];
#endif
#if ( g_PRICE_TIER7_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block10[4];
#endif
#if ( g_PRICE_TIER7_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block11[4];
#endif
#if ( g_PRICE_TIER7_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block12[4];
#endif
#if ( g_PRICE_TIER7_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block13[4];
#endif
#if ( g_PRICE_TIER7_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block14[4];
#endif
#if ( g_PRICE_TIER7_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block15[4];
#endif
#if ( g_PRICE_TIER7_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier7Block16[4];
#endif	
	
#if ( g_PRICE_TIER8_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block1[4];
#endif
#if ( g_PRICE_TIER8_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block2[4];
#endif
	
#if ( g_PRICE_TIER8_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block3[4];
#endif
	
#if ( g_PRICE_TIER8_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block4[4];
#endif
	
#if ( g_PRICE_TIER8_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block5[4];
#endif
	
#if ( g_PRICE_TIER8_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block6[4];
#endif
#if ( g_PRICE_TIER8_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block7[4];
#endif
	
#if ( g_PRICE_TIER8_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block8[4];
#endif
#if ( g_PRICE_TIER8_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block9[4];
#endif
#if ( g_PRICE_TIER8_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block10[4];
#endif
#if ( g_PRICE_TIER8_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block11[4];
#endif
#if ( g_PRICE_TIER8_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block12[4];
#endif
#if ( g_PRICE_TIER8_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block13[4];
#endif
#if ( g_PRICE_TIER8_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block14[4];
#endif
#if ( g_PRICE_TIER8_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block15[4];
#endif
#if ( g_PRICE_TIER8_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier8Block16[4];
#endif	
	
#if ( g_PRICE_TIER9_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block1[4];
#endif
#if ( g_PRICE_TIER9_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block2[4];
#endif
#if ( g_PRICE_TIER9_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block3[4];
#endif
	
#if ( g_PRICE_TIER9_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block4[4];
#endif
	
#if ( g_PRICE_TIER9_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block5[4];
#endif
	
#if ( g_PRICE_TIER9_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block6[4];
#endif
#if ( g_PRICE_TIER9_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block7[4];
#endif
	
#if ( g_PRICE_TIER9_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block8[4];
#endif
#if ( g_PRICE_TIER9_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block9[4];
#endif
#if ( g_PRICE_TIER9_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block10[4];
#endif
#if ( g_PRICE_TIER9_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block11[4];
#endif
#if ( g_PRICE_TIER9_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block12[4];
#endif
#if ( g_PRICE_TIER9_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block13[4];
#endif
#if ( g_PRICE_TIER9_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block14[4];
#endif
#if ( g_PRICE_TIER9_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block15[4];
#endif
#if ( g_PRICE_TIER9_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier9Block16[4];
#endif	
	
#if ( g_PRICE_TIER10_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block1[4];
#endif
#if ( g_PRICE_TIER10_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block2[4];
#endif
#if ( g_PRICE_TIER10_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block3[4];
#endif
	
#if ( g_PRICE_TIER10_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block4[4];
#endif
	
#if ( g_PRICE_TIER10_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block5[4];
#endif
	
#if ( g_PRICE_TIER10_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block6[4];
#endif
#if ( g_PRICE_TIER10_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block7[4];
#endif
	
#if ( g_PRICE_TIER10_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block8[4];
#endif
#if ( g_PRICE_TIER10_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block9[4];
#endif
#if ( g_PRICE_TIER10_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block10[4];
#endif
#if ( g_PRICE_TIER10_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block11[4];
#endif
#if ( g_PRICE_TIER10_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block12[4];
#endif
#if ( g_PRICE_TIER10_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block13[4];
#endif
#if ( g_PRICE_TIER10_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block14[4];
#endif
#if ( g_PRICE_TIER10_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block15[4];
#endif
#if ( g_PRICE_TIER10_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier10Block16[4];
#endif	
	
#if ( g_PRICE_TIER11_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block1[4];
#endif
#if ( g_PRICE_TIER11_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block2[4];
#endif
#if ( g_PRICE_TIER11_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block3[4];
#endif
	
#if ( g_PRICE_TIER11_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block4[4];
#endif
	
#if ( g_PRICE_TIER11_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block5[4];
#endif
	
#if ( g_PRICE_TIER11_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block6[4];
#endif
#if ( g_PRICE_TIER11_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block7[4];
#endif
	
#if ( g_PRICE_TIER11_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block8[4];
#endif
#if ( g_PRICE_TIER11_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block9[4];
#endif
#if ( g_PRICE_TIER11_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block10[4];
#endif
#if ( g_PRICE_TIER11_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block11[4];
#endif
#if ( g_PRICE_TIER11_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block12[4];
#endif
#if ( g_PRICE_TIER11_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block13[4];
#endif
#if ( g_PRICE_TIER11_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block14[4];
#endif
#if ( g_PRICE_TIER11_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block15[4];
#endif
#if ( g_PRICE_TIER11_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier11Block16[4];
#endif	
#if ( g_PRICE_TIER12_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block1[4];
#endif
#if ( g_PRICE_TIER12_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block2[4];
#endif
#if ( g_PRICE_TIER12_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block3[4];
#endif
	
#if ( g_PRICE_TIER12_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block4[4];
#endif
	
#if ( g_PRICE_TIER12_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block5[4];
#endif
	
#if ( g_PRICE_TIER12_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block6[4];
#endif
#if ( g_PRICE_TIER12_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block7[4];
#endif
	
#if ( g_PRICE_TIER12_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block8[4];
#endif
#if ( g_PRICE_TIER12_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block9[4];
#endif
#if ( g_PRICE_TIER12_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block10[4];
#endif
#if ( g_PRICE_TIER12_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block11[4];
#endif
#if ( g_PRICE_TIER12_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block12[4];
#endif
#if ( g_PRICE_TIER12_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block13[4];
#endif
#if ( g_PRICE_TIER12_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block14[4];
#endif
#if ( g_PRICE_TIER12_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block15[4];
#endif
#if ( g_PRICE_TIER12_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier12Block16[4];
#endif
#if ( g_PRICE_TIER13_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block1[4];
#endif
#if ( g_PRICE_TIER13_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block2[4];
#endif
#if ( g_PRICE_TIER13_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block3[4];
#endif
	
#if ( g_PRICE_TIER13_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block4[4];
#endif
	
#if ( g_PRICE_TIER13_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block5[4];
#endif
	
#if ( g_PRICE_TIER13_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block6[4];
#endif
#if ( g_PRICE_TIER13_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block7[4];
#endif
	
#if ( g_PRICE_TIER13_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block8[4];
#endif
#if ( g_PRICE_TIER13_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block9[4];
#endif
#if ( g_PRICE_TIER13_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block10[4];
#endif
#if ( g_PRICE_TIER13_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block11[4];
#endif
#if ( g_PRICE_TIER13_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block12[4];
#endif
#if ( g_PRICE_TIER13_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block13[4];
#endif
#if ( g_PRICE_TIER13_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block14[4];
#endif
#if ( g_PRICE_TIER13_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block15[4];
#endif
#if ( g_PRICE_TIER13_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier13Block16[4];
#endif	
	
#if ( g_PRICE_TIER14_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block1[4];
#endif
#if ( g_PRICE_TIER14_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block2[4];
#endif
#if ( g_PRICE_TIER14_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block3[4];
#endif
	
#if ( g_PRICE_TIER14_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block4[4];
#endif
	
#if ( g_PRICE_TIER14_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block5[4];
#endif
	
#if ( g_PRICE_TIER14_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block6[4];
#endif
#if ( g_PRICE_TIER14_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block7[4];
#endif
	
#if ( g_PRICE_TIER14_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block8[4];
#endif
#if ( g_PRICE_TIER14_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block9[4];
#endif
#if ( g_PRICE_TIER14_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block10[4];
#endif
#if ( g_PRICE_TIER14_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block11[4];
#endif
#if ( g_PRICE_TIER14_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block12[4];
#endif
#if ( g_PRICE_TIER14_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block13[4];
#endif
#if ( g_PRICE_TIER14_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block14[4];
#endif
#if ( g_PRICE_TIER14_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block15[4];
#endif
#if ( g_PRICE_TIER14_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier14Block16[4];
#endif
	
#if ( g_PRICE_TIER15_BLOCK1_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block1[4];
#endif
#if ( g_PRICE_TIER15_BLOCK2_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block2[4];
#endif
#if ( g_PRICE_TIER15_BLOCK3_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block3[4];
#endif
	
#if ( g_PRICE_TIER15_BLOCK4_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block4[4];
#endif
	
#if ( g_PRICE_TIER15_BLOCK5_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block5[4];
#endif
	
#if ( g_PRICE_TIER15_BLOCK6_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block6[4];
#endif
#if ( g_PRICE_TIER15_BLOCK7_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block7[4];
#endif
	
#if ( g_PRICE_TIER15_BLOCK8_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block8[4];
#endif
#if ( g_PRICE_TIER15_BLOCK9_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block9[4];
#endif
#if ( g_PRICE_TIER15_BLOCK10_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block10[4];
#endif
#if ( g_PRICE_TIER15_BLOCK11_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block11[4];
#endif
#if ( g_PRICE_TIER15_BLOCK12_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block12[4];
#endif
#if ( g_PRICE_TIER15_BLOCK13_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block13[4];
#endif
#if ( g_PRICE_TIER15_BLOCK14_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block14[4];
#endif
#if ( g_PRICE_TIER15_BLOCK15_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block15[4];
#endif
#if ( g_PRICE_TIER15_BLOCK16_ATTRIBUTE_ENABLE_d== 1 )
	uint8_t  priceTier15Block16[4];
#endif		
#endif /*( g_SE_PROFILE_1_1_d == 1 )*/
}__attribute__((__packed__))PriceClusterAttribute_t;
#endif	



#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 ) 		
#if ( g_SE_PROFILE_1_1_d == 1 )
#if ( g_PRICE_INCREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d== 1 ||    \
           g_PRICE_DECREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d== 1|| \
			 g_CLIENT_COMMODITY_TYPE_ATTRIBUTE_ENABLE_d==1)
typedef struct PriceClientClusterAttribute_Tag {
#if ( g_PRICE_INCREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d== 1 )
		uint8_t  priceIncreaseRandomizeMinutes;
#endif    	
#if ( g_PRICE_DECREASE_RANDOMIZE_MINUTES_ATTRIBUTE_ENABLE_d== 1 )
		uint8_t  priceDecreaseRandomizeMinutes;
#endif
#if (g_CLIENT_COMMODITY_TYPE_ATTRIBUTE_ENABLE_d== 1)
		uint8_t  clientCommodityType;
#endif
	
}__attribute__((__packed__))PriceClientClusterAttribute_t;
#endif
#endif /*  g_SE_PROFILE_1_1_d == 1 ) */
#endif


typedef struct publishPriceInfo_Tag
{
    uint32_t  providerId;
    uint8_t   rateLabel[12];
    uint32_t  issuerEventId;
    uint32_t  currentTime;
    uint8_t   unitOfMeasure;
    uint16_t  currency;
    uint8_t   priceTrailingDigitAndPriceTier;
    uint8_t   numOfPriceTiersAndRegisterTier;
    uint32_t  startTime;
    uint16_t  duration;
    uint32_t  price;
    uint8_t   priceRatio;
    uint32_t  generationPrice;
    uint8_t   generationPriceRatio;
    uint32_t  alternateCostDelivered;
    uint8_t   alternateCostUnit;
    uint8_t   alternateCostTrailingDigit;
#if ( g_SE_PROFILE_1_1_d == 1 )
	uint8_t   numberOfBlockThresholds;
	uint8_t   priceControl;
#endif
}__attribute__((__packed__))publishPriceInfo_t;


typedef struct getCurrentPriceInfo_Tag
{
    uint8_t commandOptions;                 /*Unsigned 8 bit Integer*/
}__attribute__((__packed__))getCurrentPriceInfo_t;

typedef struct getScheduledPriceInfo_Tag
{
    uint32_t startTime;                      /*UTC Time - Unsigned 32 bit Integer*/
    uint8_t  numberOfEvents;                 /*Unsigned 8 bit Integer*/
}__attribute__((__packed__))getScheduledPriceInfo_t;



#if ( g_SE_PROFILE_1_1_d == 1 )
typedef struct publishBlockPeriodInfo_tag
{
	uint32_t  providerId;
	uint32_t  issuerEventId;
	uint32_t  blockPeriodStartTime;
	uint8_t   blockPeriodDurationInMinutes[3];
	uint8_t   numberOfPriceTiersAndBlockThresholds;
	uint8_t   blockPeriodControl;
}__attribute__((__packed__))publishBlockPeriodInfo_t;

typedef struct priceAcknwoledgementInfo_Tag
{
	uint32_t  providerId;
	uint32_t  issuerEventId;
	uint32_t  priceAckTime;
	uint8_t   control;	
}__attribute__((__packed__))priceAcknowledgementInfo_t;

typedef struct getBlockPeriodInfo_Tag
{
	uint32_t  startTime;
    uint8_t  numberOfEvents;                 /*Unsigned 8 bit Integer*/
}__attribute__((__packed__))getBlockPeriodInfo_t;

#endif

#if ( g_SE_PROFILE_1_2_d == 1 )
/*------------------------------------- SE 1.1 A update --------------*/	
typedef struct publishConversionFactor_Tag
{
	uint32_t  issuerEventId;
	uint32_t  startTime;
	uint32_t   conversionFactor;
	uint8_t   conversionFactorTrailingDigit;
}__attribute__((__packed__))publishConversionFactor_t;

typedef struct publishCalorificValue_Tag
{
	uint32_t  issuerEventId;
	uint32_t  startTime;
	uint32_t   calorificValue;
	uint8_t   calorificValueUnit;
	uint8_t    calorificValueTrailingDigit;
}__attribute__((__packed__))publishCalorificValue_t;

#endif    /* g_SE_PROFILE_1_2_d*/


enum PriceTiers {
	NoTier 				=   0x00,
	Tier1PriceLabel     =   0x01, 			
	Tier2PriceLabel     = 	0x02,
	Tier3PriceLabel 	= 	0x03,
	Tier4PriceLabel 	= 	0x04,
	Tier5PriceLabel  	=	0x05,
	Tier6PriceLabel		= 	0x06,
	Tier7PriceLabel 	= 	0x07,
	Tier8PriceLabel   	= 	0x08,
	Tier9PriceLabel   	= 	0x09,
	Tier10PriceLabel  	= 	0x10,
	Tier11PriceLabel 	=   0x11,
	Tier12PriceLabel   	= 	0x12,
	Tier13PriceLabel  	=   0x13,
	Tier14PriceLabel    =   0x14,
	Tier15PriceLabel    =   0x15
};

/*---------------------------------------------------------------------------*/
/*   Function Prototypes                                                     */
/*---------------------------------------------------------------------------*/

uint8_t ZCL_Handle_Price_Cluster_Cmd (  uint8_t endPointId,
                                        uint8_t asduLength,
                                        uint8_t* pReceivedAsdu,
                                        uint8_t* pResponseLocation,
                                        ZCL_ActionHandler_t *pActionHandler,
                                        ZCL_Cluster_Info_t const *pZCLClusterInfo,
                                        uint8_t * pAttrsDataBaseLocation );

#if ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 )

uint8_t ZCL_CreatePublishPriceCmd ( uint8_t tranSeqNum, uint8_t* pRspStartLocation,
        publishPriceInfo_t *pPublishPriceInfo );

#if ( g_SE_PROFILE_1_1_d == 1 )
uint8_t ZCL_CreatePublishBlockPriceCmd ( uint8_t tranSeqNum, uint8_t* pRspLocation,
        publishBlockPeriodInfo_t *pBlockPeriodInfo );
#endif /*  ( g_SE_PROFILE_1_1_d == 1 )*/
#endif

#if ( g_PRICE_CLIENT_CLUSTER_ENABLE_d == 1 )
#if ( g_SE_PROFILE_1_1_d == 1 )
uint8_t ZCL_CreatePriceAcknowledgementCmd ( uint8_t tranSeqNum, uint8_t* pRspLocation,
        priceAcknowledgementInfo_t *pPriceAckInfo );
#endif /*  ( g_SE_PROFILE_1_1_d == 1 )*/

#endif    /* ( g_PRICE_SERVER_CLUSTER_ENABLE_d == 1 ) */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
#endif                                      /* g_PRICE_CLUSTER_ENABLE_d == 1 */

#endif                                      /*_ZCL_PRICE_CLUSTER_H_*/
