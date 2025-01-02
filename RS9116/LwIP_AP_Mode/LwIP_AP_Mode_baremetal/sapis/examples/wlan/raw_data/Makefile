# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)


RSI_DRIVER_PATH = ../../../include
RSI_ROM_PATH = ../../../rom


CFLAGS =  -I$(RSI_DRIVER_PATH)/
CFLAGS += -I$(RSI_ROM_PATH)/
CFLAGS += -I$(PATH_CONFIG)/
CFLAGS += -D RSI_SAMPLE_HAL

RAW_DATA_APP = rsi_raw_data_app.o 

all: $(RAW_DATA_APP) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) -g *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(RAW_DATA_APP) core *.o *~ rsi_wc_app
