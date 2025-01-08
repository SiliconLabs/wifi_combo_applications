/******************************************************************************
* ZLL_define.h
*******************************************************************************
* File Description:
*   Provides information about all macros supported by ZLL profiles.
*
******************************************************************************/
#ifndef ZLL_DEFINES_H
#define ZLL_DEFINES_H


/*****************************************************************************
* Includes
*****************************************************************************/
#include "ZigBeeDataTypes.h"
#include "ZLL_Configuration.h"


/*****************************************************************************
* Global Constants
*****************************************************************************/

/* pROFILE iDs used in ZLL profile*/
#define g_ZLL_PROFILE_ID_c                                  (0xc05E)
#define g_HOME_AUTOMATION_PROFILE_ID_c                      (0x0104)


/* Profiel Id used for ZLL touch Link Commisssioning is 0xc05E */
#define g_ZLL_PROFILE_ID_FOR_TOUCH_LINK_COMMANDS_c         (g_ZLL_PROFILE_ID_c)

/* Profiel Id used for ZLL Utility Commands is 0x0104(HA profileId) */
#define g_ZLL_PROFILE_ID_FOR_COMM_UTIL_COMMANDS_c           (g_HOME_AUTOMATION_PROFILE_ID_c)



/* simple descriptors shall be set to 0x02 for all devices of ZLL version 1.0 */
#define g_APP_DEVICE_VERSION_FLAGS_c                        (0x02)

/* ZLL profile Constants*/
/* The maximum length of time an inter-PAN transaction identifier
remains valid . value is 8seconds (8000 ms)*/
#define g_APP_INTERPAN_TRANCSID_LIFETIME_c                  (8000)

/* The maximum number of seconds a factory new router will enable its
permit join flag after its initial network scans did not find any
suitable networks ,60 seconds (60000 ms)*/
#define g_APP_MAX_PERMIT_JOIN_DURATION_c                    (60000)

/* The maximum number of attempts an end device shall use to reacquire
a lost parent before going into a dormant state. The end device can be
reactivated under application control,10 seconds (10000 ms)    */
#define g_APP_MAX_LOST_PARENT_RETRY_ATTEMP_c                (10000)

/* The maximum time an end device is permitted to use as its poll interval
without first attempting a network rejoin before transmitting
any application data.value is 1hour = 3600 seconds (3600 *1000 ms)*/
#define g_APP_MAX_POLL_INTERVAL_c                           (3600 *1000)

/* The minimum time a child must persist in the child table of its parent
before it can be aged out. (4 * aplcMax-PollInterval seconds)*/
#define g_APP_MIN_CHILD_PERSISTENT_TIME_c       (4*g_APP_MAX_POLL_INTERVAL_c)

/* The length of time an initiator waits to ensure that the recipient has
completed its startup procedure.value is 2seconds*/
#define g_APP_MIN_STARTUP_DELAY_TIME_c                      (2*1000)

#define g_APP_LEAVE_DURATION_TIME_c                         (3*1000)

/* The maximum duration that a device leaves its receiver enabled during the
joining procedure for subsequent configuration information.5seconds = 5000ms*/
#define g_APP_RX_WINDOW_DURATION_c                          (5*1000)

/* The base duration for a scan operation during which the receiver is
enabled for scan responses, 0.25 seconds ,(0.25*1000)ms*/
#define g_APP_SCAN_TIME_BASE_DURATION_c                     250//(0.25*1000)

#define g_ACTIVE_SCAN_DURATION_c

/* Following Method indicates whether the the network is set up through touch
link procedure or Zigbee mechanism like zigbeeform or join request.*/
#define g_NETWORK_SETUP_VIA_TOUCH_LINK_COMMISSIONING_c
//#define g_NETWORK_SETUP_VIA_CLASSICAL_METHOD_c            (1)

#define g_ZLL_COMMON_SET_BIT_c                              (1)

#define g_ZLL_COMMON_DONT_SET_BIT_c                         (0)

#define g_ZLL_EXTENDED_PANID_SIZE_c                         (8)

#define g_ZLL_SHORT_PANID_SIZE_c                            (2)

#define g_ZLL_PROFILE_ID_SIZE_c                             g_ZLL_SHORT_PANID_SIZE_c

#define g_ZLL_DEVICE_ID_SIZE_c                              g_ZLL_SHORT_PANID_SIZE_c

#define g_ZLL_NETWORK_KEY_SIZE_c                            (16)

#define g_ZLL_BROADCAST_SHORT_ADDRESS_c                     (0xFFFF)

#define g_ZLL_BROADCAST_PANID_c                             (0xFFFF)

#define g_ZLL_BROADCAST_SHORT_ADDRESS_ONLY_ROUTERS_c        (0xFFFD)

#define g_ZLL_COMMISSIONING_CLUSTER_ID_c                    (0x1000)

/* No_of Channels in Primary Channel Set*/
#define g_ZLL_PRIMARY_CHANNELS_NUMBER_c                     (4)

/* No_of Channels in Secondary Channel Set*/
#define g_ZLL_SECONDARY_CHANNELS_NUMBER_c                       (11)

/* maximum no of times scan request can be sent by the intitiator is 8 times
on primary channels set,5 times in 1st channel(11th channel) and once
in 15 ,20,25th channel of primary channel set */
#define g_ZLL_PRIMARY_SCANS_NUMBER_c                        (8)

/*Max Number of required scans on secondary channels*/
#define g_ZLL_SECONDARY_SCANS_NUMBER_c                      (11)

/* Number of total required scans */
#define g_ZLL_TOTAL_SCANS_NUMBER_c (g_ZLL_PRIMARY_SCANS_NUMBER_c + g_ZLL_SECONDARY_SCANS_NUMBER_c)

/* Number of scans on first ZLL channel  */
#define g_ZLL_SCANS_NUMBER_ON_FIRST_CHANNEL_c               (5)
/*ZLL startup channel*/
#define g_ZLL_STARTUP_LOGICAL_CHANNEL_c                     (11)

/* Primary Channel Set 11 15 20 25 */
#define g_ZLL_PRIMARY_CHANNEL_MASK_c                        0x2108800
#define g_ZLL_PRIMARY_CHANNELS_c                            11,15,20,25

/* Secondary Channel Set 12 13 14 16 17 18 19 21 22 23 24*/
#define g_SECONDARY_CHANNLE_MASK_c                          0x5EF7000
#define g_SECONDARY_CHANNELS_c                              12, 13, 14, 16, 17, 18, 19, 21, 22, 23, 24

#define g_ZLL_PROFILE_VERSION_c                             (0x00)

#define g_ZLL_NO_MANUFACTURER_CODE_c                        (0x00)

/* Frame Type of a command is cluster specific or not value = 0x01 indicate
specific to cluster and 0x00 indicates common to all cluster */
#define ZLL_FRAME_TYPE 0x01 // 0x00

/* ZLL lOGICAL dEVICES */
#define g_ZLL_LOGICAL_TYPE_COORDINATOR_c                    (0)
#define g_ZLL_LOGICAL_TYPE_ROUTER_c                         (1)
#define g_ZLL_LOGICAL_TYPE_END_DEVICE_c                     (2)


/* uncomment following */


#ifdef g_ZLL_LIGHT_DEVICE_c
    #define g_ZLL_DEVICE_TYPE_c             g_ZLL_LOGICAL_TYPE_ROUTER_c
#endif

#ifdef g_ZLL_LIGHT_CONTROLLER_c
    #define g_ZLL_DEVICE_TYPE_c             g_ZLL_LOGICAL_TYPE_END_DEVICE_c
#endif /* Controller */

/* ZLL RX On When idle */
#define g_ZLL_RX_ON_WHEN_IDLE_c                                (1)

/* ZLL Not RX On When idle */
#define g_ZLL_NOT_RX_ON_WHEN_IDLE_c                            (0)

/* Based on the device type Rx on/off decided */
#if defined( ZIGBEE_COORDINATOR  )
    #define g_ZLL_DEVICE_RX_ON_OR_NOT_c                 g_ZLL_RX_ON_WHEN_IDLE_c
#elif defined( ZIGBEE_ROUTER )
    #define g_ZLL_DEVICE_RX_ON_OR_NOT_c                 g_ZLL_RX_ON_WHEN_IDLE_c
#elif defined( ZIGBEE_END_DEVICE )
    #define g_ZLL_DEVICE_RX_ON_OR_NOT_c                 g_ZLL_NOT_RX_ON_WHEN_IDLE_c
#endif




/* Indicates whether the device is a factory new or not */
#define g_ZLL_FACTORY_NEW_c                                 (1)

/* ZLL NON Factory New  */
#define g_ZLL_NON_FACTORY_NEW_c                             (0)

/* ZLL Address assignement capable */
#define g_ZLL_ADDRESS_ASSIGNMENT_CAPABLE_c                  (1)

/* ZLL Not Address assignement capable */
#define g_ZLL_NOT_ADDRESS_ASSIGNMENT_CAPABLE_c              (0)

/* ZLL Link initiator  */
#define g_ZLL_LINK_INITIATOR_c                              (1)

/* ZLL Not Link initiator  */
#define g_ZLL_NOT_LINK_INITIATOR_c                          (0)

/* ZLL Request Priority */
#define g_ZLL_REQUEST_PRIORITY_c                            (1)

/* ZLL Not ZLL Request Priority */
#define g_ZLL_NOT_REQUEST_PRIORITY_c                        (0)

/* ZLL security bitmask x development key (key index 0 ) */
#define g_ZLL_KEY_BITMASK_DEVELOPMENT_KEY_c                 (0x0001)

/* ZLL security bitmask x Master Key (Master Key)  key index 4*/
#define g_ZLL_KEY_BITMASK_MASTER_KEY_c                      (0x0010)

/* ZLL security bitmask x Certification key ( key index 15 )*/
#define g_ZLL_KEY_BITMASK_CERTIFICATION_KEY_c               (0x8000)

/* Currently device supports two types of key , development key and
Master key , certified products can use cerfification key ORing with g_ZLL_KEY_BITMASK_c*/
#define g_ZLL_KEY_BITMASK_c (g_ZLL_KEY_BITMASK_DEVELOPMENT_KEY_c | g_ZLL_KEY_BITMASK_MASTER_KEY_c)


/* ZLL test key index ( key index 0 )*/
#define g_ZLL_KEY_INDEX_TEST_KEY_c                          (0)

/* ZLL Master key  index ( key index 4 ) */
#define g_ZLL_KEY_INDEX_MASTER_KEY_c                        (4)

/* ZLL Certification key index ( key index 15 )  */
#define g_ZLL_KEY_INDEX_CERTIFICATION_KEY_c                 (15)



/* Following are the SAS settings for Startup attributes */

/* no short address */
#define g_ZLL_SAS_SHORT_ADDRESS_c                           (0xffff)


/* no extendedpanid*/
#define g_ZLL_SAS_EXTENDED_PANID_c                          (0x00)

/* Device has not yet joined a network*/
#define g_ZLL_SAS_PANID_c                                   (0xffff)

/* All channels in the frequency band with a commissioning preference to the
ZLL channel set.*/
#define g_ZLL_SAS_CHANNEL_MASK_c                            (0x2108800)

/* ZigBee Spec 2007*/
#define g_ZLL_SAS_PROTOCOL_VERSION_c                        (0x02)

/* ZigBee Pro*/
#define g_ZLL_SAS_STACK_PROFILE_c                           (0x02)

/* Start Up Control : Be able to join a network by association,
under application control*/
#define g_ZLL_SAS_STARTUP_CONTROL_c                         (0x03)

/* Enabled under application Control*/
#define g_ZLL_SAS_PERMIT_JOIN_c                             (0x00)

/* Maximum no of scan responses stored */
#define g_MAX_SCAN_RESPONSE_RESULTS_c                       (5)

/* Initially radio power is set to zero
 Minimum valid radio transmit power in dBm

MIN_RADIO_POWER     -46 // dBm
 Maximum valid radio transmit power in dBm

MAX_RADIO_POWER       6 // dBm
*/
#define g_ZLL_RADIO_POWER_c                                 (0)

/* 2 bits Frametype  = 01 cluster specific */
#define g_ZLL_FRAME_TYPE_c                                  (1)

/* Command Direction 0 (client to server ) 1(server to client)*/
#define g_ZCL_DIRECTION_CLIENT_TO_SERVER_c                  (0)
#define g_ZCL_DIRECTION_SERVER_TO_CLIENT_c                  (1)

/* Size of Inter-Pan Transaction Id*/
#define g_SIZE_OF_INTER_PAN_ID_c                            (4)


/* RSSI value range 0x00 – 0x20*/
#define g_ZLL_RSSI_CORRECTION_OFFSET_c                      (0x10)

/* This macro tells about max no of zll scan response device can store */
#define g_MAX_ACTIVE_SCAN_RESULTS_c                         (10)

/* ZLL Device Information table: values  */

//#define g_ZLL_ENDPOINT_c            ZA_GLOBAL_SRC_ENDPOINT   // Which value ?: (range 0x01 - 0xf0) or ZA_GLOBAL_SRC_ENDPOINT = 0x10

//#define g_ZLL_DEVICE_ID_c           ZA_DEVICE_SPECIFIC_ID      // Which value? : See zsl_devices.h

//#define g_ZLL_DEVICE_VERSION_c      ZA_DEVICE_VERSION_c // Which value : See zsl_config.h

#define g_ZLL_NUMBER_OF_ENDPOINTS_c         g_ZLL_CONFIG_SUB_DEVICES_NUMBER_c
//
//#define g_ZLL_SORT_TAG            0
//
#define g_ZLL_DEVICE_TABLE_START_INDEX_c                      (0)
//
//#define g_ZLL_MAX_NUMBER_OF_SUB_DEVICES_c                       (5)
//

/* 4 group ids for each endpoint */
#define g_ZLL_NUMBER_OF_GROUPS_IDENTIFIER_c                       (4)
/* ascending order*/
#define g_ZLL_SORT_TAG            0
#define g_ZLL_TOTAL_GROUP_IDENTIFIER_NUMBER_c               \
(g_ZLL_NUMBER_OF_GROUPS_IDENTIFIER_c  * g_ZLL_CONFIG_SUB_DEVICES_NUMBER_c)


/* Index related to the first groupid available for the device
(range is 1-ZLL_TOTAL_GROUP_IDENTIFIER_NUMBER) */
#define g_ZLL_GROUPID_INDEX_c                                       (1)


#define g_SIZE_OF_IDENTIFY_DURATION_c                               (0x02)

/* ZLL Exit identify mode */
#define g_ZLL_EXIT_IDENTIFY_MODE_c                                  (0x0000)

/* ZLL Default time (known by the receiver)  */
#define g_ZLL_DEFAULT_IDENTIFY_MODE_c                               (0xFFFF)

/*ZLL Default time (known by the receiver)
7 seconds */
#define g_ZLL_DEFAULT_IDENTIFY_TIME_c                               (7000)


/* ZLL Response status */



/* ZLL response status success */
#define g_ZLL_RESPONSE_STATUS_SUCCESS_c                                 (0x00)

/*  ZlL response status failure  */
#define g_ZLL_RESPONSE_STATUS_FAILURE_c                                 (0x01)

#ifndef  g_ZLL_CERTIFICATION_KEY_c

#define g_ZLL_CERTIFICATION_KEY_c                               0xc0, 0xc1, 0xc2, \
                                                                0xc3, 0xc4 ,0xc5, \
                                                                0xc6 ,0xc7, 0xc8, \
                                                                0xc9 ,0xca ,0xcb, \
                                                                0xcc, 0xcd ,0xce, \
                                                                0xcf
#endif /*g_ZLL_CERTIFICATION_KEY_c*/


#ifndef  g_ZLL_DEVELOPMENT_KEY_c
#define g_ZLL_DEVELOPMENT_KEY_c                                 0xd0, 0xd1, 0xd2, \
                                                                0xd3, 0xd4 ,0xd5, \
                                                                0xd6 ,0xd7, 0xd8, \
                                                                0xd9 ,0xda ,0xdb, \
                                                                0xdc, 0xdd ,0xde, \
                                                                0xdf

#endif /*g_ZLL_DEVELOPMENT_KEY_c*/


#ifndef  g_ZLL_MASTER_KEY_c
#define g_ZLL_MASTER_KEY_c                                      0xb0, 0xb1, 0xb2, \
                                                                0xb3, 0xb4 ,0xb5, \
                                                                0xb6 ,0xb7, 0xb8, \
                                                                0xb9 ,0xba ,0xbb, \
                                                                0xbc, 0xbd ,0xbe, \
                                                                0xbf
#endif //g_ZLL_MASTER_KEY_c


#define g_SIZE_OF_ZLL_NVM_PARAMS_c                 sizeof(ZLL_NVM_Parameters_t)

/* ZLL device information table parameters  */
typedef struct ZLL_device_information_table_Tag {
    /* IEEE address: it specifies the unique IEEE identifier for each
    single node. */
    uint8_t IEEE_address[8];
    /* Endpoint identifier: it specifies the identifier of the endpoint
    on which the sub-device is implemented.
    This value is determined by the application and can be freely chosen by the
    application in the range 0x01 – 0xf0.
    The ZLL does not rely on any fixed endpoint identifier for its
    applications. */
    uint8_t endpoint_identifier;
    /* Profile identifier: it specifies the identifier of the profile supported
    by the sub-device.*/
    uint16_t profile_identifier;
    /* Device identifier: it specifies the identifier of the device description
    supported by the sub-device. This value shall correspond to the application
    device identifier field of the simple descriptor.*/
    uint16_t device_identifier;

    struct Device_Version_Tag {
        /* Device version: it specifies the version of the device description
            supported by the sub-device.
            ZLL spec version 1.0 ,device ver 0x02 */
        uint8_t  device_version : 4;
        /* Reserved */
        uint8_t  reserved : 4;
    }__attribute__((__packed__)) device_version_t;
    /* no of group id on each endpoint */
    uint8_t number_of_groups_identifiers;

    uint8_t sort_tag;
} __attribute__((__packed__))ZLL_device_information_table_t;


/*  Groupid and group ids ranges can be assigned  */
typedef struct ZLL_groups_Tag{
    uint16_t device_start_group_id;
    uint16_t device_end_group_id;

    uint16_t recipients_start_group_id;
    uint16_t recipients_end_group_id;

    /* min free group identifier */
    uint16_t free_min;
    /* max free group identifier */
    uint16_t free_max;
} __attribute__((__packed__))ZLL_groups_t;

/* This structure is used to maintain the network address*/
typedef struct ZLL_network_address_assignment_Tag{
    /* Free address is used to assign for new joining device */
    /* min free network address */
    uint16_t free_min;
    /* max free netork address */
    uint16_t free_max;
    /*current device's network address */
    uint16_t device_network_address;
}__attribute__((__packed__))ZLL_network_address_assignment_t;


/*****************************************************************************
* Public Memory declarations
*****************************************************************************/
/* None */

/*****************************************************************************
* Private Memory declarations
*****************************************************************************/

/* None */
/*****************************************************************************
* Function Prototypes
*****************************************************************************/
/* None */
/*****************************************************************************
* Public Functions
*****************************************************************************/

/* None */
/*****************************************************************************
* Private Functions
*****************************************************************************/

/* None */

/*****************************************************************************
* Interrupt Service Routines
*****************************************************************************/

/* None */



#endif /* ZLL_DEFINES_H*/
/*****************************************************************************
* End Of File
*****************************************************************************/
