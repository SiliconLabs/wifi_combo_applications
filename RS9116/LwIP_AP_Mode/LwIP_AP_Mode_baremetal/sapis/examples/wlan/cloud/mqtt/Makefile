# Make File


rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../../build/ -lrps
PATH_CONFIG=$(shell pwd)

INCLUDE_PATH = ../../../utilities/certificates
RSI_DRIVER_PATH = ../../../../include


CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+=  -I$(INCLUDE_PATH)/
CFLAGS+= -I$(PATH_CONFIG)/
CFLAGS+=  -I ../../../../rom
CFLAGS+=  -I ../../../../nwk/applications/mqtt_client/src/
CFLAGS+=  -I ../../../../nwk/applications/mqtt_client/src/MQTTPacket/src
CFLAGS+=  -I ../../../../nwk/applications/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D ASYNC_MQTT

MQTT_CLIENT_APP =rsi_mqtt.o 

all: $(MQTT_CLIENT_APP) 

	make wlan_mqtt CC=$(CC) -Wall -C ../../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o  $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(MQTT_CLIENT_APP) core *.o *~ rsi_wc_app
