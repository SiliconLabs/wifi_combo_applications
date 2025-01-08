# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)

INCLUDE_PATH = ../../utilities/certificates
RSI_DRIVER_PATH = ../../../include

CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+=  -I$(INCLUDE_PATH)/
CFLAGS+= -I$(PATH_CONFIG)/
CFLAGS+=  -I ../../../rom
CFLAGS+= -D RSI_SAMPLE_HAL

EAP_CONNECTIVITY_APP = rsi_eap_connectivity.o 

all: $(EAP_CONNECTIVITY_APP) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(EAP_CONNECTIVITY_APP) core *.o *~ rsi_wc_app
