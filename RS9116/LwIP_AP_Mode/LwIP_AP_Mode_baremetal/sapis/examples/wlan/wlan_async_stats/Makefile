# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)


RSI_DRIVER_PATH = ../../../include

CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+=  -I$(INCLUDE_PATH)/
CFLAGS+= -I$(PATH_CONFIG)/
CFLAGS+=  -I ../../../rom
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_ENABLE_DEBUG_PRINT

WLAN_ASYNC_STATS = rsi_wlan_async_stats.o 

all: $(WLAN_ASYNC_STATS) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(WLAN_ASYNC_STATS) core *.o *~ rsi_wc_app
