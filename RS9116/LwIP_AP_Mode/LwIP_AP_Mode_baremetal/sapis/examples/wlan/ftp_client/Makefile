# Make File

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)


RSI_DRIVER_PATH = ../../../include

CFLAGS=  -I$(RSI_DRIVER_PATH)/
CFLAGS+= -D RSI_SAMPLE_HAL

FTP_CLIENT_APP = rsi_ftp_client.o 

all: $(FTP_CLIENT_APP) 

	make wlan CC=$(CC) -Wall -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(FTP_CLIENT_APP) $(PROGNAME) core *.o *~ rsi_wc_app
