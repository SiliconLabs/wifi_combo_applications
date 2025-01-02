#  Make File

rm=/bin/rm -f
CC= cc
AR = ar
DEFS=  
PROGNAME= rsi_wc_app
PROGNAME_ROM= apis_rom
INCLUDES=  -I $(path_config)  \
					 -I ../include \
					 -I ../hal_9116/hal \
					 -I ../rom \
					 -I ../platforms/linux/Application/common 


INCLUDES+=  -I ../nwk/applications/mqtt_client/src
INCLUDES+=  -I ../nwk/applications/mqtt_client/src/MQTTPacket/src

LINKS= -lpthread -lrt


API_PATH = ../
ZB_CORE_SRC = $(API_PATH)/driver/
ZB_REF_APPS_SRC=$(API_PATH)/zigbee/
ZB_MAC_SRC=$(API_PATH)/zigbee/mac_apis

DEFINES= $(INCLUDES) $(DEFS) 
CFLAGS= -g -Wall $(DEFINES) -Os
CFLAGS+= -D RSI_UART_INTERFACE
#CFLAGS+= -D RSI_USB_INTERFACE
#CFLAGS+= -D RSI_M4_INTERFACE
#debug prints are valid in uart case only
CFLAGS+= -D RSI_ENABLE_DEBUG_PRINT
#CFLAGS+= -D RSI_SPI_INTERFACE
#CFLAGS+= -D RSI_SDIO_INTERFACE
CFLAGS+= -D LINUX_PLATFORM
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_WAC_MFI_ENABLE 
CFLAGS+= -D RSI_IPV6_ENABLE

ifeq ($(MAKECMDGOALS),wlan)
CFLAGS+= -D RSI_WLAN_ENABLE
endif

ifeq ($(MAKECMDGOALS),wlan_mqtt)
CFLAGS+= -D RSI_WLAN_ENABLE
CFLAGS+= -D ASYNC_MQTT
endif

ifeq ($(MAKECMDGOALS),ble)
CFLAGS+= -D RSI_BLE_ENABLE
endif

ifeq ($(MAKECMDGOALS),bt_classic)
CFLAGS+= -D RSI_BT_ENABLE
endif

ifeq ($(MAKECMDGOALS),wlan_bt_classic)
CFLAGS+= -D RSI_BT_ENABLE
CFLAGS+= -D RSI_WLAN_ENABLE
endif

ifeq ($(MAKECMDGOALS),wlan_ble)
CFLAGS+= -D RSI_BLE_ENABLE
CFLAGS+= -D RSI_WLAN_ENABLE
endif

ifeq ($(MAKECMDGOALS),wlan_dual_mode)
CFLAGS+= -D RSI_BT_ENABLE
CFLAGS+= -D RSI_BLE_ENABLE
CFLAGS+= -D RSI_WLAN_ENABLE
endif

ifeq ($(MAKECMDGOALS),zb)
CFLAGS+= -D RSI_ZB_ENABLE
CFLAGS+= -D ZB_DEBUG
endif

ifeq ($(MAKECMDGOALS),zb_mac)
CFLAGS+= -D RSI_ZB_ENABLE
CFLAGS+= -D ZB_DEBUG
CFLAGS+= -D ZB_MAC_API

endif

ifeq ($(MAKECMDGOALS),wlan_zb)
CFLAGS+= -D RSI_WLAN_ENABLE
CFLAGS+= -D RSI_ZB_ENABLE
CFLAGS+= -D ZB_DEBUG
endif

ifeq ($(MAKECMDGOALS),wlan_zb_mac)
CFLAGS+= -D RSI_WLAN_ENABLE
CFLAGS+= -D RSI_ZB_ENABLE
CFLAGS+= -D ZB_DEBUG
CFLAGS+= -D ZB_MAC_API

endif

ifeq ($(MAKECMDGOALS),wlan_ble_homekit)
CFLAGS+= -D RSI_BT_ENABLE
CFLAGS+= -D RSI_BLE_ENABLE
CFLAGS+= -D RSI_WLAN_ENABLE
CFLAGS+= -D RSI_HOMEKIT_APP
endif

ifeq ($(MAKECMDGOALS),puf)
CFLAGS+= -D RSI_PUF_ENABLE
CFLAGS+= -D RSI_WLAN_ENABLE
endif
ifeq ($(MAKECMDGOALS),crypto)
CFLAGS+= -D RSI_CRYPTO_ENABLE
CFLAGS+= -D RSI_WLAN_ENABLE
endif

ifeq ($(MAKECMDGOALS), rom)
CFLAGS+= -D APIS_ROM
#CFLAGS+= -D ROM_WIRELESS
else
CFLAGS+= -D APIS_NON_ROM
CFLAGS+= -D APIS_ROM
endif

ROM_OBJS  = ../rom/rsi_socket_rom.o \
						../rom/rsi_scheduler_rom.o \
					  ../rom/rsi_pkt_mgmt_rom.o \
   					../rom/rsi_utils_rom.o \
					  ../rom/rsi_queue_rom.o \
					  ../rom/rsi_nwk_rom.o \
					  ../rom/rsi_events_rom.o \
						../rom/rsi_apis_rom.o \
						./main.o


WLAN_OBJS = ../driver/rsi_setregion_countryinfo.o \
							../nwk/socket/rsi_socket.o \
							../nwk/applications/rsi_dns.o \
							../nwk/applications/rsi_ftp.o \
							../nwk/applications/rsi_raw_data.o \
							../nwk/applications/rsi_http_client.o \
							../nwk/applications/rsi_web_socket.o \
							../nwk/applications/rsi_smtp_client.o \
         			../nwk/applications/rsi_multicast.o \
         			../nwk/applications/rsi_mdnsd.o \
							../nwk/applications/rsi_firmware_upgradation.o \
              ../nwk/applications/http_server/rsi_http_server.o \
							../nwk/applications/http_server/rsi_json_handlers.o \
							../nwk/applications/rsi_pop3_client.o \
							../nwk/applications/rsi_sntp_client.o \
							../nwk/applications/rsi_ota_fw_up.o \
							../nwk/applications/rsi_dhcp_user_class.o \
							../driver/rsi_nwk.o 

MQTT_OBJS =          			../nwk/applications/rsi_mqtt_client.o \
							../nwk/applications/mqtt_client/src/MQTTClient.o \
							../nwk/applications/mqtt_client/src/MQTT_wrappers.o \
							../nwk/applications/mqtt_client/src/MQTTPacket/src/MQTTConnectClient.o \
							../nwk/applications/mqtt_client/src/MQTTPacket/src/MQTTDeserializePublish.o \
							../nwk/applications/mqtt_client/src/MQTTPacket/src/MQTTFormat.o \
							../nwk/applications/mqtt_client/src/MQTTPacket/src/MQTTPacket.o \
							../nwk/applications/mqtt_client/src/MQTTPacket/src/MQTTSerializePublish.o \
							../nwk/applications/mqtt_client/src/MQTTPacket/src/MQTTSubscribeClient.o \
							../nwk/applications/mqtt_client/src/MQTTPacket/src/MQTTUnsubscribeClient.o 

COMMON_OBJS = ../common/rsi_common_apis.o \
							../common/rsi_device_init_apis.o \
							../driver/rsi_driver.o \
							../driver/rsi_driver_event_handlers.o \
							../driver/rsi_events.o \
							../driver/rsi_os_none.o \
							../driver/rsi_pkt_mgmt.o \
							../driver/rsi_queue.o \
							../driver/rsi_device_init.o \
							../driver/rsi_scheduler.o \
							../driver/rsi_utils.o \
							../driver/rsi_timer.o \
							../hal/rsi_hal_mcu_interrupt.o \
							../hal/rsi_hal_mcu_ioports.o \
							../hal/rsi_hal_mcu_spi.o  \
							../hal/rsi_hal_mcu_timer.o \
							../hal/rsi_hal_mcu_random.o \
							../hal/rsi_hal_mcu_platform_init.o \
							../driver/intf/spi/rsi_spi_frame_rd_wr.o \
							../driver/intf/spi/rsi_spi_functs.o \
							../driver/intf/spi/rsi_spi_iface_init.o \
							../driver/intf/spi/rsi_spi_mem_rd_wr.o \
							../driver/intf/spi/rsi_spi_reg_rd_wr.o \
							../driver/rsi_common.o  \
							../driver/rsi_iap.o  \
							../driver/rsi_wlan.o \
							../wlan/rsi_wlan_apis.o \
							../platforms/linux/Application/uart/rsi_uart.o \
							../platforms/linux/Application/common/rsi_linux_app_init.o \
							../platforms/linux/Application/common/rsi_nl_app.o \
							../platforms/linux/Application/common/rsi_linux_apis.o \
							../platforms/linux/Application/common/rsi_bootup_config.o \
							../rom/rsi_apis_non_rom.o \
							../nwk/socket/rsi_socket.o \
							../rom/rsi_socket_rom.o \
							../rom/rsi_scheduler_rom.o \
							../rom/rsi_pkt_mgmt_rom.o \
							../rom/rsi_utils_rom.o \
							../rom/rsi_queue_rom.o \
							../rom/rsi_nwk_rom.o \
							../rom/rsi_events_rom.o \
							../rom/rsi_apis_rom.o \

ZB_OBJS = $(ZB_CORE_SRC)/rsi_zb.o \
					$(ZB_CORE_SRC)/rsi_zb_utility.o \
					../hal/rsi_timer.o \
				  $(ZB_REF_APPS_SRC)/rsi_zb_apis.o  \

ZB_MAC_OBJS = $(ZB_MAC_SRC)/rsi_zigb_mac_apis.o 

BT_COMMON_OBJS = ../driver/rsi_bt_ble.o \
					 ../bt_ble/rsi_bt_common_apis.o 

BLE_OBJS = ../bt_ble/rsi_ble_gap_apis.o \
					 ../bt_ble/rsi_ble_gatt_apis.o \
					 ../bt_ble/rsi_ble_l2cap_conn.o

BT_CLASSIC_OBJS = ../bt_ble/rsi_bt_gap_apis.o \
		  ../bt_ble/rsi_bt_spp_apis.o   \
		  ../bt_ble/rsi_bt_a2dp_apis.o  \
		  ../bt_ble/rsi_bt_avrcp_apis.o \
		  ../bt_ble/rsi_bt_hfp_apis.o \
		  ../bt_ble/rsi_bt_pbap_apis.o \
		  ../bt_ble/rsi_bt_sdp_apis.o \
		   ../bt_ble/rsi_bt_hid_apis.o \
		  ../bt_ble/rsi_bt_iap_apis.o

PUF_OBJS = 	../puf/rsi_puf.o

CRYPTO_OBJS = 	../crypto/rsi_crypto.o

.c.o:
	$(RM) $@
	$(CC) $(CFLAGS) $(LINKS)  -o $*.o  -c $*.c

all: $(PROGNAME)

$(PROGNAME) : $(COMMON_OBJS) $(WLAN_OBJS)
	$(CC) $(CFLAGS) $(LINKS) -o $(PROGNAME) $(COMMON_OBJS) $(LIBS) -L. -lRS9116APIROM


rom: $(PROGNAME_ROM) 

$(PROGNAME_ROM) : $(ROM_OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME_ROM) $(ROM_OBJS)

	ar -rvs libRS9116APIROM.a $(ROM_OBJS) 

wlan: clean $(WLAN_OBJS) $(COMMON_OBJS) $(ROM_OBJS)

	ar -rvs librps.a $(COMMON_OBJS) $(WLAN_OBJS) $(ROM_OBJS)

wlan_mqtt: clean $(WLAN_OBJS) $(MQTT_OBJS) $(COMMON_OBJS) $(ROM_OBJS)

	ar -rvs librps.a $(COMMON_OBJS) $(WLAN_OBJS) $(MQTT_OBJS) $(ROM_OBJS)

zb: clean $(ZB_OBJS) $(COMMON_OBJS) 

	ar -rvs librps.a $(COMMON_OBJS) $(ZB_OBJS)

wlan_zb: clean $(ZB_OBJS) $(COMMON_OBJS) $(WLAN_OBJS)

	ar -rvs librps.a $(COMMON_OBJS) $(ZB_OBJS) $(WLAN_OBJS)

ble: clean $(BT_COMMON_OBJS) $(BLE_OBJS) $(COMMON_OBJS)

	ar -rvs librps.a $(BT_COMMON_OBJS) $(COMMON_OBJS) $(BLE_OBJS) 

bt_classic: clean $(BT_COMMON_OBJS) $(BT_CLASSIC_OBJS) $(COMMON_OBJS)

	ar -rvs librps.a $(BT_COMMON_OBJS) $(COMMON_OBJS) $(BT_CLASSIC_OBJS)

wlan_ble: clean $(BT_COMMON_OBJS) $(BLE_OBJS) $(COMMON_OBJS) $(WLAN_OBJS)

	ar -rvs librps.a $(BT_COMMON_OBJS) $(COMMON_OBJS) $(BLE_OBJS) $(WLAN_OBJS)

wlan_dual_mode: clean $(BT_COMMON_OBJS) $(BT_CLASSIC_OBJS) $(BLE_OBJS) $(COMMON_OBJS) $(WLAN_OBJS)

	ar -rvs librps.a $(BT_COMMON_OBJS) $(COMMON_OBJS) $(BT_CLASSIC_OBJS) $(BLE_OBJS) $(WLAN_OBJS)

wlan_bt_classic: clean $(BT_COMMON_OBJS) $(BT_CLASSIC_OBJS) $(COMMON_OBJS) $(WLAN_OBJS)

	ar -rvs librps.a $(BT_COMMON_OBJS) $(COMMON_OBJS) $(BT_CLASSIC_OBJS) $(WLAN_OBJS)

zb_mac: clean $(ZB_OBJS) $(ZB_MAC_OBJS) $(COMMON_OBJS)

	ar -rvs librps.a $(COMMON_OBJS) $(ZB_OBJS) $(ZB_MAC_OBJS)

wlan_zb_mac: clean $(ZB_OBJS) $(ZB_MAC_OBJS) $(COMMON_OBJS) $(WLAN_OBJS)

	ar -rvs librps.a $(COMMON_OBJS) $(ZB_OBJS) $(ZB_MAC_OBJS) $(WLAN_OBJS)

puf: clean $(WLAN_OBJS) $(COMMON_OBJS) $(PUF_OBJS)

	ar -rvs librps.a $(COMMON_OBJS) $(WLAN_OBJS) $(PUF_OBJS)

crypto: clean $(WLAN_OBJS) $(COMMON_OBJS) $(CRYPTO_OBJS)

	 ar -rvs librps.a $(COMMON_OBJS) $(WLAN_OBJS) $(CRYPTO_OBJS)

wlan_ble_homekit: clean $(BT_COMMON_OBJS) $(BLE_OBJS) $(COMMON_OBJS) $(WLAN_OBJS)

	ar -rvs librps.a $(BT_COMMON_OBJS) $(COMMON_OBJS) $(BLE_OBJS) $(WLAN_OBJS)

clean:
	$(rm) librps.a $(COMMON_OBJS) $(ZB_OBJS) $(ZB_MAC_OBJS) $(BLE_OBJS) $(BT_COMMON_OBJS) $(BT_CLASSIC_OBJS) $(WLAN_OBJS) $(MQTT_OBJS) $(PROGNAME) $(ROM_OBJS) core *.o *~
