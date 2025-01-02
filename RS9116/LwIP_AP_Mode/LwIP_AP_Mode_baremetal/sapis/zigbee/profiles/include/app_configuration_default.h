/*******************************************************************************
 * app_configuration_default.h
 *******************************************************************************
 * Program Description:
 *
 *
 *******************************************************************************/


#ifndef APP_CONFIGURATION_DEFAULT_H
#define APP_CONFIGURATION_DEFAULT_H

/*******************************************************************************
 * Includes
 *******************************************************************************/

#ifdef CONFIGURATION_HEADER
  #include CONFIGURATION_HEADER
#endif       /* CONFIGURATION_HEADER*/


/*******************************************************************************
 * global Constants
 *******************************************************************************/
/*******************************************************************************
 g_MAX_ROUTE_TABLE_ENTRIES_c

 - This macro defines the number of entries in the route table. The
 values are:

 Minimum - 8

 Maximum - As per application requirement and memory availability

 *****************************************************************************/
#if ( g_ZIGBEE_ROUTER_d == 1 ) || ( g_ZIGBEE_COORDINATOR_d == 1 )
#ifndef g_MAX_ROUTE_TABLE_ENTRIES_c
#define g_MAX_ROUTE_TABLE_ENTRIES_c                          0x0A
#endif

#else
#ifndef g_MAX_ROUTE_TABLE_ENTRIES_c
#define g_MAX_ROUTE_TABLE_ENTRIES_c                          0x00
#endif
#endif


/*******************************************************************************
 g_MAX_NEIGHBOR_TABLE_ENTRIES_c

 - This macro defines the number of neighbor table entries.

 Maximum - As per application requirement and memory availability

 *****************************************************************************/
#ifndef g_MAX_NEIGHBOR_TABLE_ENTRIES_c
#define g_MAX_NEIGHBOR_TABLE_ENTRIES_c                       0x0A
#endif


/*******************************************************************************
 g_MAX_ROUTE_RECORD_TABLE_ENTRIES_c

 - This macro defines the number of route record table entries.

 Maximum - As per application requirement and memory availability

 *****************************************************************************/
#ifndef g_MAX_ROUTE_RECORD_TABLE_ENTRIES_c
#define g_MAX_ROUTE_RECORD_TABLE_ENTRIES_c          0x06
#endif


/******************************************************************************
 g_MAX_GROUP_TABLE_ENTRIES_c

 - This macro defines the maximum number of entries in a group table:

 Minimum - 16 as per ZigBee Stack Profile

 Maximum - As per application requirement and memory availability

******************************************************************************/

#if ( g_ZIGBEE_END_DEVICE_d == 1 ) && ( g_ZIGBEE_ACTIVE_END_DEVICE_d ==0 )
#ifndef g_MAX_GROUP_TABLE_ENTRIES_c
#define g_MAX_GROUP_TABLE_ENTRIES_c                         0x00
#endif

#else
#ifndef g_MAX_GROUP_TABLE_ENTRIES_c
#define g_MAX_GROUP_TABLE_ENTRIES_c                         0x0A
#endif

#endif        /* #if ( g_ZIGBEE_END_DEVICE_d == 1 ) && (g_ZIGBEE_ACTIVE_END_DEVICE_d==1)*/

/******************************************************************************
 g_MAX_ADDR_MAP_ENTRIES_c

 - This macro defines the maximum number of address map entries:

 Minimum - 5

 Maximum - As per application requirement and memory availability

******************************************************************************/
#ifndef g_MAX_ADDR_MAP_ENTRIES_c
#define g_MAX_ADDR_MAP_ENTRIES_c               0x0A
#endif

/******************************************************************************
 g_MAX_LINK_KEY_ENTRIES_c

 - This macro defines the maximum number of link key entries:

 Minimum - 5

 Maximum - As per application requirement and memory availability

******************************************************************************/
#ifndef g_MAX_LINK_KEY_ENTRIES_c
#define g_MAX_LINK_KEY_ENTRIES_c               0x0A
#endif
/******************************************************************************
 g_MAX_APP_LINK_KEY_ENTRIES_c

 - This macro defines the maximum number of app link key entries:

 Minimum - 5

 Maximum - As per application requirement and memory availability

******************************************************************************/
#ifndef g_MAX_APP_LINK_KEY_ENTRIES_c
#define g_MAX_APP_LINK_KEY_ENTRIES_c           0x0A
#endif
/******************************************************************************
 g_MAX_BINDING_TABLE_ENTRIES_c

 - This macro defines the maximum number of binding table entries:

 Minimum - 5

 Maximum - As per application requirement and memory availability

******************************************************************************/
#ifndef g_MAX_BINDING_TABLE_ENTRIES_c
#define g_MAX_BINDING_TABLE_ENTRIES_c          0x0A
#endif

/******************************************************************************/
/* Default values of the NWK Information
base*/
/******************************************************************************/



/******************************************************************************/
/* Default values of the ZDO Configuration Table */
/******************************************************************************/
/*******************************************************************************
 g_PERMIT_JOIN_DURATION_c

 - This macro defines the time for which a coordinator or router
 device allows other devices to join to itself. The values are:

 0x00           - Indicates that no devices can join

 0xFF           - Indicates that devices are always allowed to
 join

 0x01 - 0xFE    -  Indicates the time in seconds for which the
 device allows other devices to join
******************************************************************************/
#ifndef g_PERMIT_JOIN_DURATION_c
#define g_PERMIT_JOIN_DURATION_c                   0x00
#endif



/*******************************************************************************
 g_FORMATION_ATTEMPTS_c

 - This macro defines the number of times the devices attempts for
 formation failure.

******************************************************************************/
#ifndef g_FORMATION_ATTEMPTS_c
#define g_FORMATION_ATTEMPTS_c                     0x01
#endif

/*******************************************************************************
 g_SCAN_DURATION_c

 - Its the duration for which a device will scan for beacons.
******************************************************************************/
#ifndef g_SCAN_DURATION_c
#define g_SCAN_DURATION_c                          0x05
#endif

/*******************************************************************************
 g_JOIN_ATTEMPTS_c

 - This macro defines the number of times the devices attempts to
 join when join fails.
******************************************************************************/
#ifndef g_JOIN_ATTEMPTS_c
#define g_JOIN_ATTEMPTS_c                          0x02
#endif

/*******************************************************************************
 g_PRECONFIGURED_KEY_c

 - This macro must be set to 0x01 if supporting only preconfigured nwk key,
or else to be set with 0x02 if we hight security (Application link key and nwk
key to be supported)

- value  = 0x01
- value  = 0x02

******************************************************************************/
#ifndef g_PRECONFIGURED_KEY_c
#define g_PRECONFIGURED_KEY_c                      0x02
#endif

/*******************************************************************************
 g_NO_OF_DEVICES_JOINED_BEFORE_NVM_SAVE_c

 - This macro defines the number of devices that have to join before
 NVM save is performed.

******************************************************************************/
#ifndef g_NO_OF_DEVICES_JOINED_BEFORE_NVM_SAVE_c
#define g_NO_OF_DEVICES_JOINED_BEFORE_NVM_SAVE_c   0x02
#endif

/*******************************************************************************
 g_NO_OF_DATA_TXNS_BFORE_NVM_SAVE_c

 - This macro defines the number of data transmissions that has to
 happen before NVM save is done.

******************************************************************************/
#ifndef g_NO_OF_DATA_TXNS_BFORE_NVM_SAVE_c
#define g_NO_OF_DATA_TXNS_BFORE_NVM_SAVE_c         0x00
#endif

/*******************************************************************************
 g_TRUST_CENTER_SHORT_ADDRESS_c

 - This macro defines the short address of the TC.
******************************************************************************/
#ifndef g_TRUST_CENTER_SHORT_ADDRESS_c
#define g_TRUST_CENTER_SHORT_ADDRESS_c             0x0000
#endif


/*******************************************************************************

 g_NWK_SECURITY_LEVEL_c

 - This macro defines the security level for outgoing and incoming network
 frames.

 *****************************************************************************/
//RP: changed security level to 0
#ifndef g_NWK_SECURITY_LEVEL_c
#define g_NWK_SECURITY_LEVEL_c                      0x05
#endif


/*************************************************************************/
/* Default Startup Attribute Set */
/*************************************************************************/

/*******************************************************************************
 g_EXTENDED_PAN_ID_c

 - This macro defines the default value of the extended PAN ID of
 the network in which the device is operating.

******************************************************************************/
#ifndef g_EXTENDED_PAN_ID_c
#define g_EXTENDED_PAN_ID_c                {0x00,0x00,0x00,0x00,\
                                           0x00,0x00,0x00,0x00}
#endif

/*******************************************************************************
 g_CHANNEL_MASK_c

 - This macro defines a 32-bit mask starting from the 11th bit from
 the LSB to  the 26th bit, consisting of 16 channels represented by
 each bit.

******************************************************************************/
#define g_MASK_FOR_11_CHANNEL_c  0x00000800
#define g_MASK_FOR_12_CHANNEL_c  0x00001000
#define g_MASK_FOR_13_CHANNEL_c  0x00002000
#define g_MASK_FOR_14_CHANNEL_c  0x00004000
#define g_MASK_FOR_15_CHANNEL_c  0x00008000
#define g_MASK_FOR_16_CHANNEL_c  0x00010000
#define g_MASK_FOR_17_CHANNEL_c  0x00020000
#define g_MASK_FOR_18_CHANNEL_c  0x00040000
#define g_MASK_FOR_19_CHANNEL_c  0x00080000
#define g_MASK_FOR_20_CHANNEL_c  0x00100000
#define g_MASK_FOR_21_CHANNEL_c  0x00200000
#define g_MASK_FOR_22_CHANNEL_c  0x00400000
#define g_MASK_FOR_23_CHANNEL_c  0x00800000
#define g_MASK_FOR_24_CHANNEL_c  0x01000000
#define g_MASK_FOR_25_CHANNEL_c  0x02000000
#define g_MASK_FOR_26_CHANNEL_c  0x04000000

#ifndef g_CHANNEL_MASK_c
#define g_CHANNEL_MASK_c                   g_MASK_FOR_25_CHANNEL_c
#endif



/*******************************************************************************
 g_STARTUP_CONTROL_c

 - This macro defines how certain parameters are used. The values
 are as follows:

 0x00 - Indicates that the device consideres itself as a part of
 the network indicated by the extended PAN ID attribute.
 In this case device does  not perform any explicit join
 or rejoin operation

 0x01 - Indicates that the device forms a network with extended
 PAN ID given by the extended PAN ID attribute. The AIB's
 attribute APS Designated Coordinator is set to TRUE in
 this case

 0x02 - Indicates that the device rejoins the network with
 extended PAN ID given by the extended PAN ID attribute

 0x03 - Indicates that the device  starts "from scratch" and join
 the network using association

 The default value for an un-commissioned device is 0x03.

******************************************************************************/
#ifndef g_STARTUP_CONTROL_c
#define g_STARTUP_CONTROL_c                0x03
#endif

/*******************************************************************************
 g_TRUST_CENTER_ADDRESS_c

 - This macro defines the 64-bit IEEE address of the TC.

******************************************************************************/
#ifndef g_TRUST_CENTER_ADDRESS_c
#define g_TRUST_CENTER_ADDRESS_c           {0x00,0x00,0x00,0x00,\
                                           0x00,0x00,0x00,0x00}
#endif

/*******************************************************************************
 g_NETWORK_KEY_c

 - This macro defines the 16-byte Network Key used for security in
 the network.

******************************************************************************/
#ifndef g_NETWORK_KEY_c
#define g_NETWORK_KEY_c                    {0x11,0x22,0x33,0x44,\
                                           0x55,0x66,0x77,0x88,\
                                           0x99,0x00,0xaa,0xbb,\
                                           0xcc,0xdd,0xee,0xFF}
#endif





/*******************************************************************************
 g_NETWORK_KEY_TYPE_c

 - This attribute defines the type of Network Key is being used

 0x01 is used for ZigBee

 0x05 is used for ZigBee Pro
******************************************************************************/
#ifndef g_NETWORK_KEY_TYPE_c
#define g_NETWORK_KEY_TYPE_c                 0x05
#endif

/*******************************************************************************
 g_NETWORK_MANAGER_ADDRESS_c

 - This macro defines the network address of the network manager.

******************************************************************************/
#ifndef g_NETWORK_MANAGER_ADDRESS_c
#define g_NETWORK_MANAGER_ADDRESS_c          0x0000
#endif

/*******************************************************************************
 g_SCAN_ATTEMPTS_c

 - This macro defines the number of scan attempts that is performed
 by the device.

******************************************************************************/
#ifndef g_SCAN_ATTEMPTS_c
#define g_SCAN_ATTEMPTS_c                  0x03
#endif

/*******************************************************************************
 g_TIME_BETWEEN_SCANS_c

 - This macro holds the value of the time intervals between each
 scan retry.

******************************************************************************/
#ifndef g_TIME_BETWEEN_SCANS_c
#define g_TIME_BETWEEN_SCANS_c             0x0032
#endif

/*******************************************************************************
 g_REJOIN_INTERVAL_c

 - This macro defines the rejoin interval in seconds. This is
 converted to milliseconds before the timer module is called. i.e.
 this value has to be multiplied by 0x1F4 or 500.

******************************************************************************/
#ifndef g_REJOIN_INTERVAL_c
#define g_REJOIN_INTERVAL_c                0x01F4
#endif

/*******************************************************************************
 g_MAX_REJOIN_INTERVAL_c

 - This macro defines the maximum rejoin interval in seconds. This
 is converted to milliseconds before the timer module is called.i.e.
 this value has to be multiplied by 0x9C4 or 2500.

******************************************************************************/
#ifndef g_MAX_REJOIN_INTERVAL_c
#define g_MAX_REJOIN_INTERVAL_c            0x09C4
#endif

/*******************************************************************************
 g_POLL_RATE_c

 - This macro defines the maximum time within which an end device
 needs to poll from its parent. It is 7.5 seconds as per the ZigBee
 stack profile. Here it is defined as 7500 milli seconds or 0x1D4C
 in hex.

******************************************************************************/
#ifndef g_POLL_RATE_c
#define g_POLL_RATE_c                                       0x07D0
#endif

/*******************************************************************************
 g_PARENT_RETRY_THRESHOLD_c

 - This macro defines how many times the end device needs to attempt
 to contact its parent before initiating Rejoin process.

******************************************************************************/
#ifndef g_PARENT_RETRY_THRESHOLD_c
#define g_PARENT_RETRY_THRESHOLD_c          0x03
#endif


/*******************************************************************************
 g_SHORT_ADDRESS_c

 - This macro defines the short address of the device

******************************************************************************/
#ifndef g_SHORT_ADDRESS_c
#define g_SHORT_ADDRESS_c                     0xFFFF
#endif

/*******************************************************************************
 g_PANID_c

 - This macro defines the 16-byte Pre configured Link Key used for security in
 the APS {ayer.

******************************************************************************/
#ifndef g_PANID_c
#define g_PANID_c                              0xFFFF
#endif

/*******************************************************************************
 g_TC_MASTER_KEY_c

 - This macro defines the 16-byte Pre configured master Key used for security in
 the APS layer

******************************************************************************/
#ifndef g_TC_MASTER_KEY_c
#define g_TC_MASTER_KEY_c                  {0x11,0x22,0x33,0x44,\
                                           0x55,0x66,0x77,0x88,\
                                           0x99,0x00,0xaa,0xbb,\
                                           0xcc,0xdd,0xee,0xff}
#endif

/*******************************************************************************
 g_PRECONFG_LINK_KEY_c

 - This macro defines the 16-byte Pre configured Link Key used for security in
 the APS {ayer.

******************************************************************************/
#ifndef g_PRECONFG_LINK_KEY_c
#define g_PRECONFG_LINK_KEY_c              {0x5A,0x69,0x67,0x42,\
                                           0x65,0x65,0x41,0x6C,\
                                           0x6C,0x69,0x61,0x6E,\
                                           0x63,0x65,0x30,0x39}
/*
#define g_PRECONFG_LINK_KEY_c              {0x11,0x22,0x33,0x44,\
                                           0x55,0x66,0x77,0x88,\
                                           0x99,0x00,0xaa,0xbb,\
                                           0xcc,0xdd,0xee,0xff}  */
#endif

/*******************************************************************************
	g_NWK_IS_CONCENTRATOR_VALUE_c
	A flag determining if this device is a concentrator.
	TRUE = Device is a  concentrator.
	FALSE = Device is not a  concentrator.
******************************************************************************/

#ifndef g_NWK_IS_CONCENTRATOR_VALUE_c
#ifdef ZIGBEE_COORDINATOR
#define g_NWK_IS_CONCENTRATOR_VALUE_c               0x01
#else
#define g_NWK_IS_CONCENTRATOR_VALUE_c               0x00
#endif

#endif /* g_NWK_IS_CONCENTRATOR_VALUE_c*/



/*******************************************************************************
	g_NWK_CONCENTRATOR_RADIUS_DEF_VALUE_c
 -The hop count radius for  concentrator route  discoveries.

 -This value decides about the max no of hops the The many to one route
	request is supposed to Travel in network
******************************************************************************/
//#ifndef g_NWK_CONCENTRATOR_RADIUS_DEF_VALUE_c
//#define g_NWK_CONCENTRATOR_RADIUS_DEF_VALUE_c           0x001e
//#endif


#ifndef g_NWK_CONCENTRATOR_RADIUS_VALUE_c
#define g_NWK_CONCENTRATOR_RADIUS_VALUE_c                   0x001e
#endif /*g_NWK_CONCENTRATOR_RADIUS_VALUE_c*/

/*******************************************************************************
	g_NWK_CONCENTRATOR_DISCOVERYTIME_DEF_VALUE_c
	The time in seconds between concentrator route discoveries. If set to
    0x0000, the discoveries are done at start up and by the next higher layer only.
******************************************************************************/
#ifndef g_NWK_CONCENTRATOR_DISCOVERYTIME_DEF_VALUE_c
#define g_NWK_CONCENTRATOR_DISCOVERYTIME_DEF_VALUE_c    0x0000
#endif

/*******************************************************************************
This Flag decides whether device annce should happen or not after join.
By Default  it is set to 1
******************************************************************************/
#ifndef g_DEVICE_ANNCE_FLAG_c
#define g_DEVICE_ANNCE_FLAG_c                           0x01
#endif



/******************************************************************************/
/* default values of NIB attributes that does not require persistent storage */
/******************************************************************************/


/*******************************************************************************
 g_NWK_USE_MULTICAST_c

A flag determining the layer where multicast messaging occurs.
TRUE = multicast occurs at the network layer.
FALSE= multicast occurs at the APS layer and using the APS header.
******************************************************************************/
#ifndef g_NWK_USE_MULTICAST_c
#define g_NWK_USE_MULTICAST_c                               0x00
#endif


/*****************************************************************************
	MAX_SOURCE_ROUTING_DEPTH
	- The maximum number of hops in a source route.

    range - 0x00 to 0xff

******************************************************************************/

#ifndef MAX_SOURCE_ROUTING_DEPTH
#define MAX_SOURCE_ROUTING_DEPTH                        0x0c
#endif



/*******************************************************************************
 g_NWK_LINK_STATUS_PERIOD

 -  This macro defines the link status period for coming out a link status  frame
	from coordinator or router.
 -  Default value is 15 (0xE4E1C0) seconds. As per Spec

 Minimum - 0x00

 Maximum - 0xff

 *****************************************************************************/
#if(g_NWK_LINK_STATUS_d == 1 )
#ifndef g_NWK_LINK_STATUS_PERIOD
#define g_NWK_LINK_STATUS_PERIOD                            0xE4E1C0
#endif
#endif


/***********************************************************************/
 /* MOVED FROM STACK CONFIURATION .H */
/************************************************************************/

/*******************************************************************************
 g_MAX_NWK_KEY_c

 - This macro defines the number of Network Keys that is maintained
 by the network layer while operating in a secured network.

******************************************************************************/
#ifndef g_MAX_NWK_KEY_c
#define g_MAX_NWK_KEY_c                                     0x02
#endif


 /*g_NVM_TABLES_START_OFFSET_c

 -This defines the NVM Offset where tables are stored start of NVM
 2 KB Left for MAC and Start of Attributes
******************************************************************************/

#define m_ONE_KB_c					                    0x400

#ifndef g_NVM_TABLES_START_OFFSET_c
//#define g_NVM_TABLES_START_OFFSET_c                 m_ONE_KB_c * 2

#ifdef	STM32L1XX_MD_PLUS

  #define g_NVM_TABLES_START_OFFSET_c                      m_ONE_KB_c * 2

  #define g_NVM_SAS_TABLE_START_OFFSET_c                   m_ONE_KB_c

#else

  #define g_NVM_TABLES_START_OFFSET_c                     m_ONE_KB_c * 2

    #define g_NVM_SAS_TABLE_START_OFFSET_c                   m_ONE_KB_c

#endif  /* STM32L1XX_MD_PLUS */

#endif




/*******************************************************************************
 FRAGMENT_SIZE

 - This macro defines the max space to be allocated for storing the data , which
   need to be fragmented.It is a kind of temporary storage

    MAX_PAY_LOAD_SIZE = 0x2f
    MAX_BLOCKS_TRANSMISSION_WINDOW = 0x08.
*******************************************************************************/

#ifndef FRAGMENT_SIZE
#define FRAGMENT_SIZE                                       0x2f * 8
#endif /* FRAGMENT_SIZE*/

/*******************************************************************************
 g_LOGICAL_TYPE_COMPLEX_USER_DESCRIPTOR_AVAILABLE_c

 - This macro holds the value for the logical type of the device and
 information if complex and user descriptors are available in the
 device.

******************************************************************************/
#ifndef g_LOGICAL_TYPE_COMPLEX_USER_DESCRIPTOR_AVAILABLE_c
#if ( g_ZIGBEE_COORDINATOR_d == 1 )

#define g_LOGICAL_TYPE_COMPLEX_USER_DESCRIPTOR_AVAILABLE_c      0x18

#endif                                 /* g_ZIGBEE_COORDINATOR_d */

#if ( g_ZIGBEE_ROUTER_d == 1 )

#define g_LOGICAL_TYPE_COMPLEX_USER_DESCRIPTOR_AVAILABLE_c      0x19

#endif                                 /* g_ZIGBEE_ROUTER_d */

#if ( g_ZIGBEE_END_DEVICE_d == 1 )

#define g_LOGICAL_TYPE_COMPLEX_USER_DESCRIPTOR_AVAILABLE_c      0x1A

#endif                                 /* g_ZIGBEE_END_DEVICE_d */
#endif

/* Following macro is to enable the interface to test Frequency agility feature
from Application ,This macro is effective only for Application, in real time
these two macro should be disabled*/
#ifndef g_FREQ_AGILITY_TEST_d
#define g_FREQ_AGILITY_TEST_d                               RSI_ENABLE
#endif /* g_FREQ_AGILITY_TEST_d*/


/*******************************************************************************
 g_MAX_NO_OF_ENDPOINTS_c

 - This macro holds the value for the maximum number of endpoints

******************************************************************************/

#ifndef  g_MAX_NO_OF_ENDPOINTS_c
/* For ZLL profile ,maximum no of endpoints supported can be more than 1*/
#if( ((g_APP_PROFILE_ID_c !=0x0109 ) &&  (g_APP_PROFILE_ID_c !=0x0104 ) )|| (g_APP_PROFILE_ID_c == 0xc05e))

#ifdef  APPLY_R_20_CHANGE
#define g_MAX_NO_OF_ENDPOINTS_c                             0x0A
#else
#define g_MAX_NO_OF_ENDPOINTS_c                             0x04
#endif   /*APPLY_R_20_CHANGE*/
#else
#define g_MAX_NO_OF_ENDPOINTS_c                             0x01
#endif    /*g_APP_PROFILE_ID_c*/

#endif   /*g_MAX_NO_OF_ENDPOINTS_c*/





/*******************************************************************************
 g_SERVER_MASK_c
 - 	This variable will hold information about the server capabilities
 of the device. Currently the configuration is for Enabling primary
 trust center.
******************************************************************************/

#ifndef g_SERVER_MASK_c
#if ( g_ZIGBEE_COORDINATOR_d == 1 )

#define g_SERVER_MASK_c                {0x41,0x00}
#endif                                 /* g_ZIGBEE_COORDINATOR_d */

#if ( g_ZIGBEE_ROUTER_d == 1 )
#define g_SERVER_MASK_c                {0x00,0x00}
#endif                                 /*g_ZIGBEE_ROUTER_d*/

#if ( g_ZIGBEE_END_DEVICE_d == 1 )
#define g_SERVER_MASK_c                {0x00,0x00}
#endif                                 /* g_ZIGBEE_END_DEVICE_d */
#endif

/*******************************************************************************
 g_EXTENDED_ADDRESS_c

 - This holds the default value of the extended address of the
 device.

******************************************************************************/
#ifndef g_EXTENDED_ADDRESS_c
#define g_EXTENDED_ADDRESS_c           {0x00,0x00,0x00,0x00,\
                                       0x00,0x00,0x00,0x00}
#endif


/* This macro is used  while allocating the memory for the binding table ,
 care should g_APS_BINDING_CAPACITY_c should be enabled if binding feature is
needed or this macro should be disabled , along with this macro, InitStack()
should be with parameter STK_NEED_BINDING */

#ifndef g_APS_BINDING_CAPACITY_c
#define g_APS_BINDING_CAPACITY_c                            RSI_ENABLE
#endif /*( g_APS_BINDING_CAPACITY_c)*/

/* This macro is used to while allocating the memory for the SOURCE ROUTE table,
  g_NWK_SOURCE_ROUTING_d should be enabled if source route feature is
needed or this macro should be disabled , along with this macro, InitStack()
should be with parameter STK_NEED_SOURCE_ROUTE  */

#ifndef g_NWK_SOURCE_ROUTING_d
#define g_NWK_SOURCE_ROUTING_d                      RSI_ENABLE
#endif
/*******************************************************************************
 * Type Definitions
 *******************************************************************************/

/*******************************************************************************
 * Memory Declarations
 *******************************************************************************/


/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

void InitStackTables( void );
#if (g_APP_PROFILE_ID_c== 0xc05e)
void ZLL_SaveEssentialParams(uint8_t *pNVM_params , uint8_t size);
void ZLL_RestoreEssentialParams(uint8_t *pNVM_params , uint8_t size);
#endif /* g_APP_PROFILE_ID_c== 0xc05e*/

/*******************************************************************************
 * End Of File
 *******************************************************************************/

#endif                                /* APP_CONFIGURATION_DEFAULT_H */
