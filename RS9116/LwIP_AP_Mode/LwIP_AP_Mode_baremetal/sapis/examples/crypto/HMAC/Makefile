# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)
RSI_DRIVER_PATH = ../../../include
RSI_APP_PATH = ../../../crypto/

CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+= -I$(RSI_APP_PATH)/
CFLAGS+=  -I$(INCLUDE_PATH)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_ENABLE_DEBUG_PRINT #Enable to get prints in LINUX
HMAC_SHA_APP = rsi_hmac_sha_app.o 

all: $(HMAC_SHA_APP) 

	make crypto CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  

clean:
	$(rm) $(HMAC_SHA_APP) core *.o *~ rsi_wc_app 
