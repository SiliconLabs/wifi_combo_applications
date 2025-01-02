# Make File



rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)

RSI_DRIVER_PATH = ../../../include
RSI_ROM_PATH = ../../../rom
CFLAGS+=  -I$(RSI_DRIVER_PATH)/
CFLAGS+=  -I$(PATH_CONFIG)/
CFLAGS+=  -I$(RSI_ROM_PATH)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_BLE_ENABLE
RSI_BLE_APP_PROXIMITY_REPORTER = rsi_ble_proximity.o 

all: $(RSI_BLE_APP_PROXIMITY_REPORTER) 

	make ble CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(RSI_BLE_APP_PROXIMITY_REPORTER) core *.o *~
