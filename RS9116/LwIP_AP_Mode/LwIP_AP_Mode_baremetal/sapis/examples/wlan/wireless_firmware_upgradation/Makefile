# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)

INCLUDE_PATH = ../../utilities/webpages
RSI_DRIVER_PATH = ../../../include
RSI_APP_PATH = ../../../nwk/applications
RSI_HTTP_SERVER_PATH = ../../../nwk/applications/http_server

CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+= -I$(RSI_APP_PATH)/
CFLAGS+=  -I$(INCLUDE_PATH)/
CFLAGS+=  -I$(RSI_HTTP_SERVER_PATH)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D LINUX_PLATFORM

PROVISIONING_APP = rsi_wfup_app.o 

all: $(PROVISIONING_APP) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(PROVISIONING_APP) core *.o *~ rsi_wc_app 
