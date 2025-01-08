#Makefile

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)

RSI_DRIVER_PATH = ../../../include
RSI_ROM_PATH = ../../../rom
CFLAGS+=  -I$(RSI_DRIVER_PATH)/
CFLAGS+=  -I$(RSI_ROM_PATH)/
CFLAGS+=  -I$(PATH_CONFIG)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_BT_ENABLE
CFLAGS+= -D RSI_BLE_ENABLE

DUAL_MODE_APP = rsi_bt_app.o \
  							rsi_ble_app.o \
	  						main.o


all: $(DUAL_MODE_APP) 

	make wlan_dual_mode CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(DUAL_MODE_APP) core *.o *~ rsi_wc_app
