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
CFLAGS+= -D RSI_SAMPLE_HAL

POWER_SAVE_APP = rsi_wlan_power_save_profile.o 

all: $(POWER_SAVE_APP) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) -g *.o $(LINKS) -o $(PROGNAME)  
	rm -f  *.o  


clean:
	$(rm) $(POWER_SAVE_APP) core *.o *~ rsi_wc_app
