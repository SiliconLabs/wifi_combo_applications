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

CFLAGS+= -D LINUX_KEYCODE_TESTING

BLE_HID_APP = rsi_ble_hid.o

all: $(BLE_HID_APP) 

	make ble CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(BLE_HID_APP) $(PROGNAME) core *.o *~
