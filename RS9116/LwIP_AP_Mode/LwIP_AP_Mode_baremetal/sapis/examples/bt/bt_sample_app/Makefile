# Makefile

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_bt_classic_sample_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)

RSI_DRIVER_PATH = ../../../include
RSI_ROM_PATH = ../../../rom
CFLAGS+=  -I$(RSI_DRIVER_PATH)/
CFLAGS+=  -I$(RSI_ROM_PATH)/
CFLAGS+=  -I$(PATH_CONFIG)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D SLAVE_MODE_ENABLE
CFLAGS+= -D RSI_BT_ENABLE

BT_SAMPLE_APP = rsi_bt_sample_app.o 

all: $(BT_SAMPLE_APP) 

	make bt_classic CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(BT_SAMPLE_APP) core *.o *~
