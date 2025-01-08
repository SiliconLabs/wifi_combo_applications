# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)


RSI_DRIVER_PATH = ../../../include
RSI_APP_PATH = ../../../nwk/applications


CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+= -I$(RSI_APP_PATH)/
CFLAGS+= -D RSI_SAMPLE_HAL

RAM_DUMP = rsi_ram_dump.o 

all: $(RAM_DUMP) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(RAM_DUMP) core *.o *~ rsi_wc_app 
