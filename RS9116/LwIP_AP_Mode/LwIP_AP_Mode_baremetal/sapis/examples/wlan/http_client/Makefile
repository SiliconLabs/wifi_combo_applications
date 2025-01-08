# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)

INCLUDE_PATH = ../../utilities/certificates
RSI_DRIVER_PATH = ../../../include
RSI_APP_PATH = ../../../nwk/applications


CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+= -I$(RSI_APP_PATH)/
CFLAGS+= -I$(INCLUDE_PATH)/
CFLAGS+= -D RSI_SAMPLE_HAL

HTTP_CLIENT_APP = rsi_http_client_app.o 

all: $(HTTP_CLIENT_APP) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(HTTP_CLIENT_APP) core *.o *~ rsi_wc_app 
