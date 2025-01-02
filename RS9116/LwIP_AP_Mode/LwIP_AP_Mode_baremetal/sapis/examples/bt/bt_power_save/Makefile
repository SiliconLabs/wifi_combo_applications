#Makefile

rm=/bin/rm -f
CC= cc
DEFS=  
PROGNAME= rsi_wc_app
LINKS= -pthread -L../../../build/ -lrps
PATH_CONFIG=$(shell pwd)


RSI_DRIVER_PATH = ../../../include
RSI_CERTIFICATES_PATH = ../../utilities/certificates

CFLAGS+=  -I$(RSI_DRIVER_PATH)/ -I$(RSI_CERTIFICATES_PATH)
CFLAGS+= -I $(PATH_CONFIG)/
CFLAGS+= -D RSI_SAMPLE_HAL
CFLAGS+= -D RSI_BT_ENABLE\
				 -I ../../../rom/

BT_PWR_SAVE_APP = rsi_bt_power_save_profile.o 


all: $(BT_PWR_SAVE_APP) 

	make wlan_bt_classic CC=$(CC) -C ../../../build/ path_config=$(PATH_CONFIG) 
	$(CC) $(CFLAGS) *.o $(LINKS) -o $(PROGNAME) 
	rm -f  *.o  


clean:
	$(rm) $(BT_PWR_SAVE_APP) core *.o *~ rsi_wc_app
