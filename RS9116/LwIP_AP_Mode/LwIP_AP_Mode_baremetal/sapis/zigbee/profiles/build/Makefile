#TOOLCHAIN_BASE_DIR = /work/gnu-ta-2.4.6/bin
#TOOLCHAIN_BASE_DIR = /work/trinadh/gnu-ta-2.4.5/bin
#TOOLCHAIN_BASE_DIR = /work/satish/gnu-ta-2.4.6/bin
#TOOLCHAIN_PREFIX = rpine-ta-elf32-

rm=/bin/rm -f
PROGNAME= rsi_wc_app
LINKS= -lpthread -lrt
################### Select Build configuration ##################

#TEST_SUITE = HA_SAMPLE_APP
#TEST_SUITE = SE_SAMPLE_APP
#TEST_SUITE = ZLL_SAMPLE_APP
#TEST_SUITE = MAC_TESTHARNESS
#TEST_SUITE = ZIGBEE_TESTHARNESS
#TEST_SUITE = ZIGBEE_SAMPLE_APP
#TEST_SUITE = ZIGBEE_RZSP_APP
#TEST_SUITE = HA_PROFILE
#TEST_SUITE = SE_PROFILE
#TEST_SUITE = ZLL_PROFILE

############################## HA ###############################
#SUB_SUITE = COLOR_DIMMABLE_LIGHT
#SUB_SUITE = COLOR_DIMMER_SWITCH
#SUB_SUITE = COMBINED_INTERFACE
#SUB_SUITE = CONSUMPTION_AWARENESS
#SUB_SUITE = CONFIGURATION_TOOL
#SUB_SUITE = DOOR_LOCK
#SUB_SUITE = DOOR_LOCK_CONTROLLER
#SUB_SUITE = FLOW_SENSOR
#SUB_SUITE = HEATING_COOLING_UNIT
#SUB_SUITE = HOME_GATEWAY
#SUB_SUITE = IAS_ACE
#SUB_SUITE = IAS_CIE
#SUB_SUITE = IAS_WD
#SUB_SUITE = IAS_ZONE
#SUB_SUITE = LEVEL_CONTROLLABLE_OUTPUT
#SUB_SUITE = LEVEL_CONTROL_SWITCH
#SUB_SUITE = LIGHT_SENSOR
#SUB_SUITE = MAINS_POWER_OUTLET
#SUB_SUITE = METER_INTERFACE
#SUB_SUITE = OCCUPANCY_SENSOR
#SUB_SUITE = ON_OFF_LIGHT
#SUB_SUITE = ON_OFF_LIGHT_SWITCH
#SUB_SUITE = ON_OFF_OUTPUT
#SUB_SUITE = ON_OFF_SWITCH
#SUB_SUITE = PRESSURE_SENSOR
#SUB_SUITE = PUMP
#SUB_SUITE = PUMP_CONTROLLER
#SUB_SUITE = RANGE_EXTENDER
#SUB_SUITE = REMOTE_CONTROL
#SUB_SUITE = SCENE_SELECTOR
#SUB_SUITE = SHADE
#SUB_SUITE = SHADE_CONTROLLER
#SUB_SUITE = SIMPLE_SENSOR
#SUB_SUITE = SMART_PLUG
#SUB_SUITE = TEMPERATURE_SENSOR
#SUB_SUITE = THERMOSTAT
#SUB_SUITE = WHITE_GOODS
#SUB_SUITE = WINDOW_COVERING
#SUB_SUITE = WINDOW_COVERING_CONTROLLER
#SUB_SUITE = DIMMABLE_LIGHT
#SUB_SUITE = DIMMER_SWITCH
############################## HA ###############################

############################## SE ###############################

#SUB_SUITE = ESP1
#SUB_SUITE = ESP
#SUB_SUITE = IPD
#SUB_SUITE = LCD
#SUB_SUITE = METERING
#SUB_SUITE = PCT
#SUB_SUITE = PHYSICAL_DEVICE
#SUB_SUITE = PTD
#SUB_SUITE = RE
#SUB_SUITE = SA
############################## SE ###############################

##############################ZLL################################
#SUB_SUITE = CONTROLLER
#SUB_SUITE = LIGHT

#############################ZLL#################################

################### Select Build configuration ##################

CFLAGS += -Wall -g -c -fno-common -fno-inline -Os
#CFLAGS += -Wall -g -c -fno-common -Os

################################### UTILITITES ################################### 
RSI_INCLUDE = ../include
RSI_DRIVER_PATH = ../../../include

#SW TIMER
RSI_802154_SW_TIMER_H_PATH = ../ZigBee_Stack/HAL/Timer/inc
RSI_802154_SW_TIMER_SRC_PATH = ../ZigBee_Stack/HAL/Timer/src

#TRX ACCESS
RSI_802154_TRX_ACCESS_INC_PATH = ../ZigBee_Stack/HAL/trx_access/inc
RSI_802154_TRX_ACCESS_SRC_PATH = ../ZigBee_Stack/HAL/trx_access/src

#STACK EVENTS
RSI_802154_STACK_EVENTS_INC_PATH = ../ZigBee_Stack/HAL/Stack_Events/inc
RSI_802154_STACK_EVENTS_SRC_PATH = ../ZigBee_Stack/HAL/Stack_Events/src

#BUFFER MANAGEMENT
RSI_802154_BUFFER_MGMT_H_PATH =   ../utilities/buffer_management/inc
RSI_802154_BUFFER_MGMT_SRC_PATH = ../utilities/buffer_management/src
#QUEUE MANAGEMENT
RSI_802154_QUEUE_MGMT_H_PATH =   ../utilities/queue_management/inc
RSI_802154_QUEUE_MGMT_SRC_PATH = ../utilities/queue_management/src
#MEMORY UTILITY
RSI_802154_MEM_UTIL_H_PATH = ../utilities/Memory_Utility/inc
RSI_802154_MEM_UTIL_SRC =    ../utilities/memory_utility/src

#PERSISTENT_STORAGE
RSI_802154_MAC_NVM_H_PATH = ../ZigBee_Stack/MAC_PHY_2003/stack_utility/Persistent_Storage_Module/inc
RSI_802154_MAC_NVM_SRC = ../ZigBee_Stack/MAC_PHY_2003/stack_utility/Persistent_Storage_Module/src

################################### UTILITITES ###################################

################################### STACK COMMON ###################################
RSI_802154_MAC_COMMON_H_PATH = ../ZigBee_Stack/MAC_PHY_2003/inc/mac
RSI_802154_MAC_COMMON_SRC_PATH = ../ZigBee_Stack/MAC_PHY_2003/src/mac

RSI_802154_STCK_CONFIG_INC = ../utilities/stack_configuration/inc
RSI_802154_STCK_CONFIG_SRC = ../utilities/stack_configuration/src

################################### STACK COMMON ##################################

###################################### MAC/PHY ####################################

#MMAC
RSI_MMAC_H_PATH = ../ZigBee_Stack/MAC_PHY_2003/MAC/MMAC/inc
RSI_MMAC_SRC_PATH = ../ZigBee_Stack/MAC_PHY_2003/MAC/MMAC/src

#LMAC
RSI_LMAC_H_PATH = ../ZigBee_Stack/MAC_PHY_2003/MAC/LMAC/inc
RSI_LMAC_SRC_PATH = ../ZigBee_Stack/MAC_PHY_2003/MAC/LMAC/src

#PHY
RSI_PHY_H_PATH = ../ZigBee_Stack/MAC_PHY_2003/PHY/inc
RSI_PHY_SRC_PATH = ../ZigBee_Stack/MAC_PHY_2003/PHY/src

###################################### MAC/PHY ####################################

###################################### ZIGBEE #####################################

#STACK INTERFACE/CONFIGURATION
RSI_ZIGBEE_STK_INTRF_H_PATH = ../ZigBee_Stack/ZigBee/stack_interface
RSI_ZIGBEE_CONFIG_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Config/inc

RSI_ZIGBEE_STACK_UTIL_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Stack_Util/inc
RSI_ZIGBEE_STACK_UTIL_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Stack_Util/src

#APS
RSI_ZIGBEE_APS_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/APS/inc
RSI_ZIGBEE_APS_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/APS/src

#BINDING
RSI_ZIGBEE_BINDING_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Binding/inc
RSI_ZIGBEE_BINDING_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Binding/src

#FRAGMENTATION
RSI_ZIGBEE_FRAGMENTATION_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Fragmentation/inc
RSI_ZIGBEE_FRAGMENTATION_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Fragmentation/src

#FREQUENCY AGILITY
RSI_ZIGBEE_FREQ_AGIL_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/FrequencyAgility/inc
RSI_ZIGBEE_FREQ_AGIL_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/FrequencyAgility/src

#NWK
RSI_ZIGBEE_NWK_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/NWK/inc
RSI_ZIGBEE_NWK_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/NWK/src

#SECURITY
RSI_ZIGBEE_SECURITY_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Security/inc
RSI_ZIGBEE_SECURITY_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/Security/src

#ZDO
RSI_ZIGBEE_ZDO_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/ZDO/inc
RSI_ZIGBEE_ZDO_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/ZDO/src

#ZDP
RSI_ZIGBEE_ZDP_H_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/ZDP/inc
RSI_ZIGBEE_ZDP_SRC_PATH = ../ZigBee_Stack/ZigBee/ZigBeePRO/ZDP/src

#ZigBee Utilities
RSI_ZIGBEE_UTILS_H_PATH = ../ZigBee_Stack/ZigBee_Utilities/inc
RSI_ZIGBEE_UTILS_SRC_PATH = ../ZigBee_Stack/ZigBee_Utilities/src

RSI_ZIGBEE_ZLL_CLOSURES_H_PATH = ../../../../ZCL/ZLL_Utilities/inc



###################################### ZIGBEE #####################################

################################### APPLICATION ###################################

#MAC TEST HARNESS
RSI_TESTAPP_MAC_TH_INC = ../TestHarness/MAC_TH/inc
RSI_TESTAPP_MAC_TH_SRC = ../TestHarness/MAC_TH/src

#ZIGBEE TEST HARNESS
RSI_TESTAPP_ZIGBEE_TH_INC = ../TestHarness/ZigBee_TH/inc
RSI_TESTAPP_ZIGBEE_TH_SRC = ../TestHarness/ZigBee_TH/src

#ZIGBEE SAMPLE APP
RSI_TESTAPP_ZIGBEE_SA_INC = ../Application/ZigBee_Application/inc
RSI_TESTAPP_ZIGBEE_SA_SRC = ../Application/ZigBee_Application/src

RSI_TESTAPP_ZIGBEE_RZSP_SRC = ../Application/rzsp_app/src
RSI_TESTAPP_ZIGBEE_RZSP_INC = ../Application/rzsp_app/inc

RSI_ZIGB_HA_DEVICES = ../Devices/HA_Devices/

################################### APPLICATION ###################################
#CBKE
RSI_APP_CBKE_H_PATH = ../utilities/CBKE/inc
RSI_APP_CBKE_SRC_PATH = ../utilities/CBKE/src

#TMR 
RSI_TIMER_INC_PATH = ../utilities/timer/inc
RSI_TIMER_SRC_PATH = ../utilities/timer/src
################################# AppProfile_Utils ################################
#AppProfile_Utilities
RSI_ZIGBEE_APP_PROFILE_UTILS_H_PATH = ../utilities/app_profile_utilities/inc
RSI_ZIGBEE_APP_PROFILE_UTILS_SRC_PATH = ../utilities/app_profile_utilities/src/

################################# AppProfile_Utils ################################

################################### APPLICATION ###################################

#TEST HARNESS
RSI_TESTAPP_CDL_TH_INC = ../inc
RSI_TESTAPP_CDL_TH_SRC = ../src

################################### APPLICATION ###################################

CC=gcc
CCDEP=gcc
LD=ld
NM=nm
OD=objdump
OC=objcopy
AS=as
AR=ar
RANLIB=ranlib
ARFLAGS=rcs
#CFLAGS += -I  $(COMMON_LIB_H_PATH)/ -I $(COMMON_RAM_API_H_PATH)/ -I $(H_PATH)/ -I $(RSI_802154_H_PATH)/ -I $(RSI_802154_BUFFER_MGMT_H_PATH)/ -I $(RSI_802154_QUEUE_MGMT_H_PATH)/ -I $(RSI_802154_STCK_CONFIG_INC)/ -I $(RSI_802154_MAC_COMMON_H_PATH)/ -I $(RSI_802154_MEM_UTIL_H_PATH)/ -I $(RSI_802154_MAC_NVM_H_PATH)/ -I $(RSI_802154_SW_TIMER_H_PATH)/ -I $(RSI_PHY_H_PATH)/ -I $(RSI_802154_TRX_ACCESS_INC_PATH)/ -I $(RSI_LMAC_H_PATH)/ -I $(RSI_MMAC_H_PATH)/ -I $(RSI_ZIGBEE_STK_INTRF_H_PATH)/ -I $(RSI_ZIGBEE_CONFIG_H_PATH)/ -I $(RSI_ZIGBEE_STK_INTRF_H_PATH)/ -I $(RSI_ZIGBEE_APS_H_PATH)/ -I $(RSI_ZIGBEE_BINDING_H_PATH)/ -I $(RSI_ZIGBEE_FRAGMENTATION_H_PATH)/ -I $(RSI_ZIGBEE_FREQ_AGIL_H_PATH)/ -I $(RSI_ZIGBEE_NWK_H_PATH)/ -I $(RSI_ZIGBEE_SECURITY_H_PATH)/ -I $(RSI_ZIGBEE_STACK_UTIL_H_PATH)/ -I $(RSI_ZIGBEE_ZDO_H_PATH)/ -I $(RSI_ZIGBEE_ZDP_H_PATH)/ -I$(RSI_802154_STACK_EVENTS_INC_PATH)/ -I $(RSI_APP_CBKE_H_PATH)/ -I $(USB_H_PATH)/
CFLAGS += $(COMMON_FLAGS)
#CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS += -I ../../../rom/
CFLAGS += -I  $(RSI_802154_BUFFER_MGMT_H_PATH)/ -I $(RSI_802154_QUEUE_MGMT_H_PATH)/ -I $(RSI_802154_MEM_UTIL_H_PATH)/ -I $(RSI_APP_CBKE_H_PATH )
CFLAGS += -I  $(RSI_802154_BUFFER_MGMT_H_PATH)/ -I $(RSI_802154_QUEUE_MGMT_H_PATH)/ -I $(RSI_802154_STCK_CONFIG_INC)/ -I $(RSI_802154_MEM_UTIL_H_PATH)/ -I $(RSI_DRIVER_PATH)/ -I $(RSI_INCLUDE)/ -I $(RSI_APP_CBKE_H_PATH)/ -I $(RSI_ZIGBEE_APP_PROFILE_UTILS_H_PATH) / -I$(RSI_TIMER_INC_PATH)
-include ../../build/config.mk

####### ZigBee HIL Flags ##########
CFLAGS += $(COMMON_FLAGS)
CFLAGS += -D ZB_ENABLE
CFLAGS += -D COMMON_HOST
CFLAGS += -D COEX_ENABLE
#CFLAGS += -D ZB_LMAC_32MHZ
#CFLAGS += -D ZIGBEE_STANDALONE
#CFLAGS += -D FPGA_VALIDATION
CFLAGS += -D RETAPEOUT_CHIP
CFLAGS += -D EEPROM_MAC_ADDR_READ
CFLAGS += -D ZIGBEE_COEX_CHANGES
CFLAGS += -D ON_DEMAND_RADIO
#CFLAGS += -D NO_HOST
CFLAGS += -D ZIGB_PER_ENABLE
CFLAGS += -D ZB_TEMP_BASE_POWER_OFFSET
#CFLAGS += -D ZB_TEMP_BASE_RX_GAIN_OFFSET
CFLAGS += -D ZB_TEMP_CALIB_EVENT

-include config.mk
			    	
			
			
OBJS_QUEUE_MGMT = $(RSI_802154_QUEUE_MGMT_SRC_PATH)/queue.c
OBJS_BUFFER_MGMT = $(RSI_802154_BUFFER_MGMT_SRC_PATH)/buffer_management.c
OBJS_STCK_CFG = $(RSI_802154_STCK_CONFIG_SRC)/stack_configuration.c
OBJS_MEM_UTIL = $(RSI_802154_MEM_UTIL_SRC)/memory_utility.c

OBJS_RANDOM_GEN = $(RSI_APP_CBKE_SRC_PATH)/DRNG.c \
									$(RSI_APP_CBKE_SRC_PATH)/AES128.c 
OBJS_TIMER      = $(RSI_TIMER_SRC_PATH)/sw_timer.c 
OBJS_API_PATH   = $(RSI_ZIGBEE_APP_PROFILE_UTILS_SRC_PATH)/rsi_zb_api.c

ifeq ($(TEST_SUITE), ZIGBEE_SAMPLE_APP)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_SA_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)
CFLAGS+= -DNO_HOST
CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -DDEBUG_TEST=0
CFLAGS+= -Dg_FRAGMENTATION_TEST_d=1

CFLAGS+= -DZIGBEE_COORDINATOR
CFLAGS+= -DZC
#CFLAGS+= -DZIGBEE_ROUTER
#CFLAGS+= -DZR1
#CFLAGS+= -DZIGBEE_END_DEVICE
#CFLAGS+= -DZIGBEE_ACTIVE_END_DEVICE
#CFLAGS+= -DZE1

CFLAGS+= -DZIGBEE_SA
CFLAGS+= -DTEST_SUITE=8

OBJS_TEST_SUITE = $(OBJS_ZIGBEE_SA) $(OBJS_ZIGBEE)
endif

#Defines for RZSP application
ifeq ($(TEST_SUITE), ZIGBEE_RZSP_APP)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_RZSP_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)
#CFLAGS+= -DNO_HOST
CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -DDEBUG_TEST=0
CFLAGS+= -Dg_ZIGBEE_SECURITY_d=1
#CFLAGS+= -DZCP_TEST=1
CFLAGS+= -Dg_FREQ_AGILITY_TEST_d=1
CFLAGS+= -Dg_FRAGMENTATION_TEST_d=1
CFLAGS+= -Dg_COMMISSIONING_CAPABILITIES_d=0

#CFLAGS+= -DZIGBEE_COORDINATOR
#CFLAGS+= -DZC
#CFLAGS+= -DZIGBEE_ROUTER
#CFLAGS+= -DZR1
CFLAGS+= -DZIGBEE_END_DEVICE
#CFLAGS+= -DZIGBEE_ACTIVE_END_DEVICE
#CFLAGS+= -DZE1

CFLAGS+= -DZIGBEE_RZSP
CFLAGS+= -DTEST_SUITE=8

OBJS_TEST_SUITE = $(OBJS_ZIGBEE_RZSP) $(OBJS_ZIGBEE)

endif


ifeq ($(TEST_SUITE), ZIGBEE_TESTHARNESS)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_TH_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)

CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -Dg_ZIGBEE_SECURITY_d=1
CFLAGS+= -Dg_COMMISSIONING_CAPABILITIES_d=1
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS+= -DZCP_TEST=1
CFLAGS+= -DZIGBEE_LIB_MEMORY_OPTIMIZATION=0
CFLAGS+= -DDEBUG_TEST=0
CFLAGS+= -Dg_FREQ_AGILITY_TEST_d=1
CFLAGS+= -Dg_FRAGMENTATION_TEST_d=1
CFLAGS+= -DAPPLY_R_20_CHANGE

#CFLAGS+= -DZIGBEE_COORDINATOR
#CFLAGS+= -DZC
#CFLAGS+= -DZC1
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DZR1
#CFLAGS+= -DZR2
#CFLAGS+= -DZR3
#CFLAGS+= -DZR4
#CFLAGS+= -DZR5
#CFLAGS+= -DZIGBEE_END_DEVICE
#CFLAGS+= -DZIGBEE_ACTIVE_END_DEVICE
#CFLAGS+= -DZE1
#CFLAGS+= -DZE2
#CFLAGS+= -DZE3

CFLAGS+= -DZIGBEE_TH
CFLAGS+= -DTEST_SUITE=1

OBJS_TEST_SUITE = $(OBJS_USB_TEST_APP) $(OBJS_ZIGBEE_TH) $(OBJS_ZIGBEE)
endif

ifeq ($(TEST_SUITE), HA_PROFILE)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_TH_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)

CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_APP_PROFILE_ID_c=0x0104
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -Dg_ZIGBEE_SECURITY_d=1
CFLAGS+= -Dg_COMMISSIONING_CAPABILITIES_d=1
CFLAGS+= -DZIGBEE_LIB_MEMORY_OPTIMIZATION=0
CFLAGS+= -DHA_TH

CFLAGS+= -DTEST_SUITE=6

CFLAGS+= -DZC
#CFLAGS+= -DZC1
#CFLAGS+= -DZR1
#CFLAGS+= -DZR2
#CFLAGS+= -DZR3
#CFLAGS+= -DZR4
#CFLAGS+= -DZR5
#CFLAGS+= -DZR6
#CFLAGS+= -DZR7
#CFLAGS+= -DZR8
#CFLAGS+= -DZR9
#CFLAGS+= -DZE1
#CFLAGS+= -DZE2

ifeq ($(SUB_SUITE), COLOR_DIMMABLE_LIGHT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/ColorDimmableLight/inc/
endif

ifeq ($(SUB_SUITE), COLOR_DIMMER_SWITCH)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/ColorDimmerSwitch/inc/
endif

ifeq ($(SUB_SUITE), COMBINED_INTERFACE)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/CombinedInterface/inc/
endif

ifeq ($(SUB_SUITE), CONSUMPTION_AWARENESS)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=1
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=1
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/ConsumptionAwareness/inc/
endif


ifeq ($(SUB_SUITE), CONFIGURATION_TOOL)
CFLAGS+= -DZIGBEE_COORDINATOR
#CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/ConfigurationTool/inc/
endif

ifeq ($(SUB_SUITE), DOOR_LOCK)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/DoorLock/inc/
endif

ifeq ($(SUB_SUITE), DOOR_LOCK_CONTROLLER)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/DoorLockController/inc/
endif

ifeq ($(SUB_SUITE), FLOW_SENSOR)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/FlowSensor/inc/
endif

ifeq ($(SUB_SUITE), HEATING_COOLING_UNIT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/HeatingCoolingUnit/inc/
endif

ifeq ($(SUB_SUITE), HOME_GATEWAY)
CFLAGS+= -DZIGBEE_ROUTER
#CFLAGS+= -DZIGBEE_END_DEVICE
CFLAGS+= -Dg_HA_1_2_d=1
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=1
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/HomeGateway/inc/
endif

ifeq ($(SUB_SUITE), IAS_ACE)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/IAS_ACE/inc/
endif

ifeq ($(SUB_SUITE), IAS_CIE)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/IAS_CIE/inc/
endif

ifeq ($(SUB_SUITE), IAS_WD)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/IAS_WD/inc/
endif

ifeq ($(SUB_SUITE), IAS_ZONE)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/IAS_ZONE/inc/
endif

ifeq ($(SUB_SUITE), LEVEL_CONTROLLABLE_OUTPUT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/LevelControllableOutput/inc/
endif

ifeq ($(SUB_SUITE), LEVEL_CONTROL_SWITCH)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/LevelControlSwitch/inc/
endif

ifeq ($(SUB_SUITE), LIGHT_SENSOR)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/LightSensor/inc/

endif

ifeq ($(SUB_SUITE), MAINS_POWER_OUTLET)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/MainsPowerOutlet/inc/

endif

ifeq ($(SUB_SUITE), METER_INTERFACE)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=1
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=1
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/MeterInterface/inc/
endif

ifeq ($(SUB_SUITE), OCCUPANCY_SENSOR)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/OccupancySensor/inc/

endif

ifeq ($(SUB_SUITE), ON_OFF_LIGHT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/OnOffLight/inc/

endif

ifeq ($(SUB_SUITE), ON_OFF_LIGHT_SWITCH)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/OnOffLightSwitch/inc/
endif

ifeq ($(SUB_SUITE), ON_OFF_OUTPUT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/OnOffOutput/inc/
endif

ifeq ($(SUB_SUITE), ON_OFF_SWITCH)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/OnOffSwitch/inc/

endif

ifeq ($(SUB_SUITE), PRESSURE_SENSOR)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/PressureSensor/inc/

endif

ifeq ($(SUB_SUITE), PUMP)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/Pump/inc/
endif

ifeq ($(SUB_SUITE), PUMP_CONTROLLER)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/PumpController/inc/

endif

ifeq ($(SUB_SUITE), RANGE_EXTENDER)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/RangeExtender/inc/
endif

ifeq ($(SUB_SUITE), REMOTE_CONTROL)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/RemoteControl/inc/

endif

ifeq ($(SUB_SUITE), SCENE_SELECTOR)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
CFLAGS += -I $(RSI_ZIGB_HA_DEVICES)/SceneSelector/inc/
endif

ifeq ($(SUB_SUITE), SHADE)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
endif

ifeq ($(SUB_SUITE), SHADE_CONTROLLER)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0

endif

ifeq ($(SUB_SUITE), SIMPLE_SENSOR)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0

endif

ifeq ($(SUB_SUITE), SMART_PLUG)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=1
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=1

endif

ifeq ($(SUB_SUITE), TEMPERATURE_SENSOR)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0

endif

ifeq ($(SUB_SUITE), THERMOSTAT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0

endif

ifeq ($(SUB_SUITE), WHITE_GOODS)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=1
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=1

endif

ifeq ($(SUB_SUITE), WINDOW_COVERING)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0

endif

ifeq ($(SUB_SUITE), WINDOW_COVERING_CONTROLLER)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0

endif

ifeq ($(SUB_SUITE), DIMMABLE_LIGHT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
endif

ifeq ($(SUB_SUITE), DIMMER_SWITCH)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -Dg_HA_1_2_d=0
CFLAGS+= -Dg_DIAGNOSTICS_CLUSTER_ENABLE_d=0
endif

CFLAGS+= -DAPP_PROFILES_TH
CFLAGS+= -DHA_TH
CFLAGS+= -DTEST_SUITE=6

OBJS_TEST_SUITE = $(OBJS_USB_TEST_APP) $(OBJS_ZIGBEE)
endif  

ifeq ($(TEST_SUITE), SE_PROFILE)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_TH_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)

CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -Dg_ZIGBEE_SECURITY_d=1
CFLAGS+= -Dg_COMMISSIONING_CAPABILITIES_d=1
CFLAGS+= -DZIGBEE_LIB_MEMORY_OPTIMIZATION=0
CFLAGS+= -Dg_TOTAL_NUMBER_OF_LARGE_BUFFERS_c=12


ifeq ($(SUB_SUITE), ESP)
CFLAGS+= -DZIGBEE_COORDINATOR
CFLAGS+= -DESP
endif

ifeq ($(SUB_SUITE), IPD)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DIPD
endif

ifeq ($(SUB_SUITE), LCD)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DLCD
endif

ifeq ($(SUB_SUITE), METERING)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DMETERING
endif

ifeq ($(SUB_SUITE), PCT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DPCT
endif

ifeq ($(SUB_SUITE), PHYSICAL_DEVICE)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DPHYSICAL_DEVICE
endif

ifeq ($(SUB_SUITE), PTD)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DPTD
endif

ifeq ($(SUB_SUITE), RE)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DRE
endif

ifeq ($(SUB_SUITE), SA)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DSA
endif

ifeq ($(SUB_SUITE), ESP1)
CFLAGS+= -DZIGBEE_COORDINATOR
CFLAGS+= -DESP1
endif

CFLAGS+= -DAPP_PROFILES_TH
CFLAGS+= -DSE_TH
CFLAGS+= -DTEST_SUITE=4

OBJS_TEST_SUITE = $(OBJS_USB_TEST_APP) $(OBJS_ZIGBEE) $(OBJS_CBKE)
endif 

ifeq ($(TEST_SUITE), ZLL_PROFILE)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_TH_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)
CFLAGS += -I $(RSI_ZIGBEE_ZLL_CLOSURES_H_PATH)

CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -Dg_ZIGBEE_SECURITY_d=1
CFLAGS+= -Dg_COMMISSIONING_CAPABILITIES_d=0
CFLAGS+= -DZIGBEE_LIB_MEMORY_OPTIMIZATION=0

CFLAGS+= -DTEST_SUITE=0x0A
CFLAGS+= -Dg_APP_PROFILE_ID_c=0xc05e
CFLAGS+= -DZLL_TH



CFLAGS+= -DZR1
#CFLAGS+= -DZR2
#CFLAGS+= -DZR3
#CFLAGS+= -DZR4
#CFLAGS+= -DZR7
#CFLAGS+= -DZE1
#CFLAGS+= -DZE2

ifeq ($(SUB_SUITE), CONTROLLER)
CFLAGS+= -DZIGBEE_END_DEVICE
endif 

ifeq ($(SUB_SUITE), LIGHT)
CFLAGS+= -DZIGBEE_ROUTER
endif 

CFLAGS+= -DAPP_PROFILES_TH

OBJS_TEST_SUITE = $(OBJS_USB_TEST_APP) $(OBJS_ZIGBEE)
endif  

ifeq ($(TEST_SUITE), HA_SAMPLE_APP)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_TH_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)

CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
ifeq ($(SUB_SUITE), DIMMABLE_LIGHT)	
CFLAGS+= -DZIGBEE_COORDINATOR
endif
ifeq ($(SUB_SUITE), ON_OFF_LIGHT)	
CFLAGS+= -DZIGBEE_COORDINATOR
endif
ifeq ($(SUB_SUITE), DIMMER_SWITCH)	
CFLAGS+= -DZIGBEE_ROUTER
endif
ifeq ($(SUB_SUITE), ON_OFF_SWITCH)	
#CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DZIGBEE_END_DEVICE	
#CFLAGS+= -DZIGBEE_COORDINATOR
endif
CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_APP_PROFILE_ID_c=0x0104
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -Dg_ZIGBEE_SECURITY_d=1
CFLAGS+= -Dg_COMMISSIONING_CAPABILITIES_d=0
CFLAGS+= -DZIGBEE_LIB_MEMORY_OPTIMIZATION=0
CFLAGS+= -DHA_SA
CFLAGS+= -DAPPLY_R_20_CHANGE
CFLAGS+= -DDEBUG_TEST=0

CFLAGS+= -DTEST_SUITE=7

OBJS_TEST_SUITE = $(OBJS_USB_TEST_APP) $(OBJS_ZIGBEE)

endif
ifeq ($(TEST_SUITE), SE_SAMPLE_APP)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_TH_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)

CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
ifeq ($(SUB_SUITE), IPD)	
	CFLAGS+= -DZIGBEE_ROUTER
	CFLAGS+= -DIPD
endif

ifeq ($(SUB_SUITE), ESP)	
	CFLAGS+= -DZIGBEE_COORDINATOR
	CFLAGS+= -DESP
endif

CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -Dg_ZIGBEE_SECURITY_d=1
CFLAGS+= -Dg_COMMISSIONING_CAPABILITIES_d=0
CFLAGS+= -DZIGBEE_LIB_MEMORY_OPTIMIZATION=0
CFLAGS+= -Dg_TOTAL_NUMBER_OF_LARGE_BUFFERS_c=12
CFLAGS+= -DDEBUG_TEST=0
CFLAGS+= -DSE_SA

CFLAGS+= -DTEST_SUITE=5

OBJS_TEST_SUITE = $(OBJS_USB_TEST_APP) $(OBJS_ZIGBEE)

endif

ifeq ($(TEST_SUITE), ZLL_SAMPLE_APP)
CFLAGS += -I $(RSI_TESTAPP_ZIGBEE_TH_INC)
CFLAGS += -I $(RSI_ZIGBEE_UTILS_H_PATH)
CFLAGS += -I $(RSI_ZIGBEE_ZLL_CLOSURES_H_PATH)

CFLAGS+= -Dg_XMODEM_SUPPORT_d=0
CFLAGS+= -DUNIFIED_NBCN_NSEC
CFLAGS+= -Dg_MAC_2006_d
CFLAGS+= -Dg_CHANNEL_PAGE_0_2450_MHZ_FREQ_BAND_d
CFLAGS+= -Dg_ZIGBEE_SECURITY_d=1
CFLAGS+= -Dg_COMMISSIONING_CAPABILITIES_d=0
CFLAGS+= -DZIGBEE_LIB_MEMORY_OPTIMIZATION=0

CFLAGS+= -DTEST_SUITE=0x0A
CFLAGS+= -Dg_APP_PROFILE_ID_c=0xc05e
CFLAGS+= -DZLL_SA

ifeq ($(SUB_SUITE), CONTROLLER)
CFLAGS+= -DZIGBEE_END_DEVICE
CFLAGS+= -DZE1
endif 

ifeq ($(SUB_SUITE), LIGHT)
CFLAGS+= -DZIGBEE_ROUTER
CFLAGS+= -DZR1
endif 

OBJS_TEST_SUITE = $(OBJS_USB_TEST_APP) $(OBJS_ZIGBEE)
endif  


all: zigbee 

zigbee: 
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS_BUFFER_MGMT) $(OBJS_QUEUE_MGMT) $(OBJS_STCK_CFG) $(OBJS_MEM_UTIL) $(OBJS_RANDOM_GEN) $(OBJS_TIMER) $(OBJS_API_PATH)

#zigbee: clean
#	$(CC) $(CFLAGS) -DZB_ENABLE -DICACHE_QSPI $(CPPFLAGS) $(OBJS) $(OBJS_BUFFER_MGMT) $(OBJS_QUEUE_MGMT) $(OBJS_STCK_CFG) $(OBJS_SW_TIMER) $(OBJS_MEM_UTIL) $(OBJS_MAC_NVM) $(OBJS_PHY) $(OBJS_LMAC) $(OBJS_MMAC) $(OBJS_ZB) $(OBJS_ICACHE) $(OBJS_TRX_ACCESS) $(OBJS_TEST_SUITE)

ifeq ($(TEST_SUITE),HA_SAMPLE_APP)
ifeq ($(SUB_SUITE), DIMMABLE_LIGHT)	
	make -C ../Application/DimmableLight_Sample_Application/Makefile
	
	cp ../Application/DimmableLight_Sample_Application/Makefile/*.o .
	
	rm -rf ../Application/DimmableLight_Sample_Application/Makefile/*.o
endif
ifeq ($(SUB_SUITE), DIMMER_SWITCH)	
	make -C ../Application/DimmerSwitch_Sample_Application/Makefile
	
	cp ../Application/DimmerSwitch_Sample_Application/Makefile/*.o .
	
	rm -rf ../Application/DimmerSwitch_Sample_Application/Makefile/*.o
endif
ifeq ($(SUB_SUITE), ON_OFF_SWITCH)	
	make -C ../Application/OnOffSwitch_Sample_Application/Makefile
	
	cp ../Application/OnOffSwitch_Sample_Application/Makefile/*.o .
	
	rm -rf ../Application/OnOffSwitch_Sample_Application/Makefile/*.o
endif
ifeq ($(SUB_SUITE), ON_OFF_LIGHT)	
	make -C ../Application/OnOffLight_Sample_Application/Makefile
	
	cp ../Application/OnOffLight_Sample_Application/Makefile/*.o .
	
	rm -rf ../Application/OnOffLight_Sample_Application/Makefile/*.o
endif
endif

ifeq ($(TEST_SUITE),ZLL_SAMPLE_APP)
ifeq ($(SUB_SUITE), CONTROLLER)	
	make -C ../Application/ZLL_Controller_Sample_Application/Makefile
	
	cp ../Application/ZLL_Controller_Sample_Application/Makefile/*.o .
	
	rm -rf ../Application/ZLL_Controller_Sample_Application/Makefile/*.o
endif
ifeq ($(SUB_SUITE), LIGHT)	
	make -C ../Application/ZLL_Light_Sample_Application/Makefile
	
	cp ../Application/ZLL_Light_Sample_Application/Makefile/*.o .
	
	rm -rf ../Application/ZLL_Light_Sample_Application/Makefile/*.o
endif
endif

ifeq ($(TEST_SUITE), HA_PROFILE)
ifeq ($(SUB_SUITE), DIMMABLE_LIGHT)
	
	make -C ../devices/ha_devices/DimmableLight/Makefile
	
	cp ../devices/ha_devices/DimmableLight/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/DimmableLight/Makefile/*.o
	
endif
ifeq ($(SUB_SUITE), DIMMER_SWITCH)
	
	make -C ../devices/ha_devices/DimmerSwitch/Makefile
	
	cp ../devices/ha_devices/DimmerSwitch/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/DimmerSwitch/Makefile/*.o
endif

ifeq ($(SUB_SUITE), COLOR_DIMMABLE_LIGHT)
	
	make -C ../devices/ha_devices/ColorDimmableLight/Makefile
	
	cp ../devices/ha_devices/ColorDimmableLight/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/ColorDimmableLight/Makefile/*.o
endif

ifeq ($(SUB_SUITE), COLOR_DIMMER_SWITCH)
	
	make -C ../devices/ha_devices/ColorDimmerSwitch/Makefile
	
	cp ../devices/ha_devices/ColorDimmerSwitch/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/ColorDimmerSwitch/Makefile/*.o
endif

ifeq ($(SUB_SUITE), COMBINED_INTERFACE)
	
	make -C ../devices/ha_devices/CombinedInterface/Makefile
	
	cp ../devices/ha_devices/CombinedInterface/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/CombinedInterface/Makefile/*.o
endif

ifeq ($(SUB_SUITE), CONSUMPTION_AWARENESS)
	
	make -C ../devices/ha_devices/ConsumptionAwareness/Makefile
	
	cp ../devices/ha_devices/ConsumptionAwareness/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/ConsumptionAwareness/Makefile/*.o
endif

ifeq ($(SUB_SUITE), CONFIGURATION_TOOL)
	
	make -C ../devices/ha_devices/ConfigurationTool/Makefile
	
	cp ../devices/ha_devices/ConfigurationTool/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/ConfigurationTool/Makefile/*.o
endif

ifeq ($(SUB_SUITE), DOOR_LOCK)
	
	make -C ../devices/ha_devices/DoorLock/Makefile
	
	cp ../devices/ha_devices/DoorLock/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/DoorLock/Makefile/*.o
endif

ifeq ($(SUB_SUITE), DOOR_LOCK_CONTROLLER)
	
	make -C ../devices/ha_devices/DoorLockController/Makefile
	
	cp ../devices/ha_devices/DoorLockController/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/DoorLockController/Makefile/*.o
endif

ifeq ($(SUB_SUITE), FLOW_SENSOR)
	
	make -C ../devices/ha_devices/FlowSensor/Makefile
	
	cp ../devices/ha_devices/FlowSensor/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/FlowSensor/Makefile/*.o
endif

ifeq ($(SUB_SUITE), HEATING_COOLING_UNIT)
	
	make -C ../devices/ha_devices/HeatingCoolingUnit/Makefile
	
	cp ../devices/ha_devices/HeatingCoolingUnit/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/HeatingCoolingUnit/Makefile/*.o
endif

ifeq ($(SUB_SUITE), HOME_GATEWAY)
	
	make -C ../devices/ha_devices/HomeGateway/Makefile
	
	cp ../devices/ha_devices/HomeGateway/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/HomeGateway/Makefile/*.o
endif

ifeq ($(SUB_SUITE), IAS_ACE)
	
	make -C ../devices/ha_devices/IAS_ACE/Makefile
	
	cp ../devices/ha_devices/IAS_ACE/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/IAS_ACE/Makefile/*.o
endif

ifeq ($(SUB_SUITE), IAS_CIE)
	
	make -C ../devices/ha_devices/IAS_CIE/Makefile
	
	cp ../devices/ha_devices/IAS_CIE/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/IAS_CIE/Makefile/*.o
endif

ifeq ($(SUB_SUITE), IAS_WD)
	
	make -C ../devices/ha_devices/IAS_WD/Makefile
	
	cp ../devices/ha_devices/IAS_WD/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/IAS_WD/Makefile/*.o
endif

ifeq ($(SUB_SUITE), IAS_ZONE)
	
	make -C ../devices/ha_devices/IAS_Zone/Makefile
	
	cp ../devices/ha_devices/IAS_Zone/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/IAS_Zone/Makefile/*.o
endif

ifeq ($(SUB_SUITE), LEVEL_CONTROLLABLE_OUTPUT)
	
	make -C ../devices/ha_devices/LevelControllableOutput/Makefile
	
	cp ../devices/ha_devices/LevelControllableOutput/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/LevelControllableOutput/Makefile/*.o
endif

ifeq ($(SUB_SUITE), LEVEL_CONTROL_SWITCH)
	
	make -C ../devices/ha_devices/LevelControlSwitch/Makefile
	
	cp ../devices/ha_devices/LevelControlSwitch/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/LevelControlSwitch/Makefile/*.o
endif

ifeq ($(SUB_SUITE), LIGHT_SENSOR)
	
	make -C ../devices/ha_devices/LightSensor/Makefile
	
	cp ../devices/ha_devices/LightSensor/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/LightSensor/Makefile/*.o
endif

ifeq ($(SUB_SUITE), MAINS_POWER_OUTLET)
	
	make -C ../devices/ha_devices/MainsPowerOutlet/Makefile
	
	cp ../devices/ha_devices/MainsPowerOutlet/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/MainsPowerOutlet/Makefile/*.o
endif

ifeq ($(SUB_SUITE), METER_INTERFACE)
	
	make -C ../devices/ha_devices/MeterInterface/Makefile
	
	cp ../devices/ha_devices/MeterInterface/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/MeterInterface/Makefile/*.o
endif

ifeq ($(SUB_SUITE), OCCUPANCY_SENSOR)
	
	make -C ../devices/ha_devices/OccupancySensor/Makefile
	
	cp ../devices/ha_devices/OccupancySensor/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/OccupancySensor/Makefile/*.o
endif

ifeq ($(SUB_SUITE), ON_OFF_LIGHT)
	
	make -C ../devices/ha_devices/OnOffLight/Makefile
	
	cp ../devices/ha_devices/OnOffLight/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/OnOffLight/Makefile/*.o
endif

ifeq ($(SUB_SUITE), ON_OFF_LIGHT_SWITCH)
	
	make -C ../devices/ha_devices/OnOffLightSwitch/Makefile
	
	cp ../devices/ha_devices/OnOffLightSwitch/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/OnOffLightSwitch/Makefile/*.o
endif

ifeq ($(SUB_SUITE), ON_OFF_OUTPUT)
	
	make -C ../devices/ha_devices/OnOffOutput/Makefile
	
	cp ../devices/ha_devices/OnOffOutput/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/OnOffOutput/Makefile/*.o
endif

ifeq ($(SUB_SUITE), PRESSURE_SENSOR)
	
	make -C ../devices/ha_devices/PressureSensor/Makefile
	
	cp ../devices/ha_devices/PressureSensor/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/PressureSensor/Makefile/*.o
endif

ifeq ($(SUB_SUITE), ON_OFF_SWITCH)
	
	make -C ../devices/ha_devices/OnOffSwitch/Makefile
	
	cp ../devices/ha_devices/OnOffSwitch/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/OnOffSwitch/Makefile/*.o
endif

ifeq ($(SUB_SUITE), PUMP)
	
	make -C ../devices/ha_devices/Pump/Makefile
	
	cp ../devices/ha_devices/Pump/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/Pump/Makefile/*.o
endif

ifeq ($(SUB_SUITE), PUMP_CONTROLLER)
	
	make -C ../devices/ha_devices/PumpController/Makefile
	
	cp ../devices/ha_devices/PumpController/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/PumpController/Makefile/*.o
endif

ifeq ($(SUB_SUITE), RANGE_EXTENDER)
	
	make -C ../devices/ha_devices/RangeExtender/Makefile
	
	cp ../devices/ha_devices/RangeExtender/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/RangeExtender/Makefile/*.o
endif

ifeq ($(SUB_SUITE), REMOTE_CONTROL)
	
	make -C ../devices/ha_devices/RemoteControl/Makefile
	
	cp ../devices/ha_devices/RemoteControl/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/RemoteControl/Makefile/*.o
endif

ifeq ($(SUB_SUITE), SCENE_SELECTOR)
	
	make -C ../devices/ha_devices/SceneSelector/Makefile
	
	cp ../devices/ha_devices/SceneSelector/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/SceneSelector/Makefile/*.o
endif

ifeq ($(SUB_SUITE), SHADE)
	
	make -C ../devices/ha_devices/Shade/Makefile
	
	cp ../devices/ha_devices/Shade/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/Shade/Makefile/*.o
endif

ifeq ($(SUB_SUITE), SHADE_CONTROLLER)
	
	make -C ../devices/ha_devices/ShadeController/Makefile
	
	cp ../devices/ha_devices/ShadeController/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/ShadeController/Makefile/*.o
endif

ifeq ($(SUB_SUITE), SIMPLE_SENSOR)
	
	make -C ../devices/ha_devices/SimpleSensor/Makefile
	
	cp ../devices/ha_devices/SimpleSensor/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/SimpleSensor/Makefile/*.o
endif

ifeq ($(SUB_SUITE), SMART_PLUG)
	
	make -C ../devices/ha_devices/SmartPlug/Makefile
	
	cp ../devices/ha_devices/SmartPlug/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/SmartPlug/Makefile/*.o
endif

ifeq ($(SUB_SUITE), TEMPERATURE_SENSOR)
	
	make -C ../devices/ha_devices/TemperatureSensor/Makefile
	
	cp ../devices/ha_devices/TemperatureSensor/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/TemperatureSensor/Makefile/*.o
endif

ifeq ($(SUB_SUITE), THERMOSTAT)
	
	make -C ../devices/ha_devices/Thermostat/Makefile
	
	cp ../devices/ha_devices/Thermostat/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/Thermostat/Makefile/*.o
endif

ifeq ($(SUB_SUITE), WHITE_GOODS)
	
	make -C ../devices/ha_devices/WhiteGoods/Makefile
	
	cp ../devices/ha_devices/WhiteGoods/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/WhiteGoods/Makefile/*.o
endif

ifeq ($(SUB_SUITE), WINDOW_COVERING)
	
	make -C ../devices/ha_devices/WindowCovering/Makefile
	
	cp ../devices/ha_devices/WindowCovering/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/WindowCovering/Makefile/*.o
endif

ifeq ($(SUB_SUITE), WINDOW_COVERING_CONTROLLER)
	
	make -C ../devices/ha_devices/WindowCoveringController/Makefile
	
	cp ../devices/ha_devices/WindowCoveringController/Makefile/*.o .
	
	rm -rf ../devices/ha_devices/WindowCoveringController/Makefile/*.o
endif

endif

ifeq ($(TEST_SUITE),SE_SAMPLE_APP)
ifeq ($(SUB_SUITE), IPD)	
	make -C ../Application/IPD_Sample_Application/Makefile
	
	cp ../Application/IPD_Sample_Application/Makefile/*.o .
	
	rm -rf ../Application/IPD_Sample_Application/Makefile/*.o
endif

ifeq ($(SUB_SUITE), ESP)	
	make -C ../Application/ESP_Sample_Application/Makefile
	
	cp ../Application/ESP_Sample_Application/Makefile/*.o .
	
	rm -rf ../Application/ESP_Sample_Application/Makefile/*.o
endif

endif

ifeq ($(TEST_SUITE), SE_PROFILE)
ifeq ($(SUB_SUITE), ESP)
	
	make -C ../devices/se_devices/ESP/Makefile
	
	cp ../devices/se_devices/ESP/Makefile/*.o .
	
	rm -rf ../devices/se_devices/ESP/Makefile/*.o
	
endif
ifeq ($(SUB_SUITE), IPD)
		
	make -C ../devices/se_devices/IPD/Makefile
	
	cp ../devices/se_devices/IPD/Makefile/*.o .
	
	rm -rf ../devices/se_devices/IPD/Makefile/*.o

endif

ifeq ($(SUB_SUITE), LCD)
		
	make -C ../devices/se_devices/LCD/Makefile
	
	cp ../devices/se_devices/LCD/Makefile/*.o .
	
	rm -rf ../devices/se_devices/LCD/Makefile/*.o

endif

ifeq ($(SUB_SUITE), METERING)
		
	make -C ../devices/se_devices/Metering/Makefile
	
	cp ../devices/se_devices/Metering/Makefile/*.o .
	
	rm -rf ../devices/se_devices/Metering/Makefile/*.o

endif

ifeq ($(SUB_SUITE), PCT)
		
	make -C ../devices/se_devices/PCT/Makefile
	
	cp ../devices/se_devices/PCT/Makefile/*.o .
	
	rm -rf ../devices/se_devices/PCT/Makefile/*.o

endif

ifeq ($(SUB_SUITE), PHYSICAL_DEVICE)
		
	make -C ../devices/se_devices/PhysicalDevice/Makefile
	
	cp ../devices/se_devices/PhysicalDevice/Makefile/*.o .
	
	rm -rf ../devices/se_devices/PhysicalDevice/Makefile/*.o

endif

ifeq ($(SUB_SUITE), PTD)
		
	make -C ../devices/se_devices/PTD/Makefile
	
	cp ../devices/se_devices/PTD/Makefile/*.o .
	
	rm -rf ../devices/se_devices/PTD/Makefile/*.o

endif

ifeq ($(SUB_SUITE), RE)
		
	make -C ../devices/se_devices/RE/Makefile
	
	cp ../devices/se_devices/RE/Makefile/*.o .
	
	rm -rf ../devices/se_devices/RE/Makefile/*.o

endif

ifeq ($(SUB_SUITE), SA)
		
	make -C ../devices/se_devices/SA/Makefile
	
	cp ../devices/se_devices/SA/Makefile/*.o .
	
	rm -rf ../devices/se_devices/SA/Makefile/*.o

endif

ifeq ($(SUB_SUITE), ESP1)
	
	make -C ../devices/se_devices/ESP/Makefile
	
	cp ../devices/se_devices/ESP/Makefile/*.o .
	
	rm -rf ../devices/se_devices/ESP/Makefile/*.o
	
endif

endif

ifeq ($(TEST_SUITE), ZLL_PROFILE)
ifeq ($(SUB_SUITE), CONTROLLER)

	make -C ../devices/zll_devices/Controller/Makefile
	
	cp ../devices/zll_devices/Controller/Makefile/*.o .
	
	rm -rf ../devices/zll_devices/Controller/Makefile/*.o
endif

ifeq ($(SUB_SUITE), LIGHT)

	make -C ../devices/zll_devices/Light/Makefile
	
	cp ../devices/zll_devices/Light/Makefile/*.o .
	
	rm -rf ../devices/zll_devices/Light/Makefile/*.o
endif

endif

	#$(CC) $(LINKS) *.o -o $(PROGNAME)  
   
	

*.o:*.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f  *.o rsi_wc* 

#*.o:*.c
#	$(CC) $(CFLAGS) $< -o $@
