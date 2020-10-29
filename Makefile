#***************************************************************************
#
#	Copyright (c) 1997-2019 Jeffrey Vernon Merkey
#	All Rights Reserved.
#
#**************************************************************************

INCLUDES=netware-worms.h
UTILFILES=libnetware-screensaver.a libnetware-screensaver.so netware-worms

# user utility build flags
U_CC = gcc
U_CCP = g++
U_CFLAGSP = -g -O3
U_CFLAGS_LIBP = -g -c -O3 
LD = ld
AR = ar

all : utilities

libnetware-screensaver.so: libnetware-screensaver.o 
	$(LD) -shared -lc -o libnetware-screensaver.so libnetware-screensaver.o  

libnetware-screensaver.a: libnetware-screensaver.o
	$(AR) r libnetware-screensaver.a libnetware-screensaver.o  

libnetware-screensaver.o: libnetware-screensaver.c $(INCLUDES)
	$(U_CCP) $(U_CFLAGS_LIBP) -fPIC -Wall libnetware-screensaver.c 

netware-worms: netware-worms.c libnetware-screensaver.a $(INCLUDES)
	$(U_CCP) $(U_CFLAGSP) netware-worms.c -Wall -o netware-worms -lncursesw -lpthread libnetware-screensaver.a

clean:
	rm -rf *.o $(UTILFILES)

utilities: $(UTILFILES)

install: utilities
	install -m 0755 netware-worms /usr/bin
	install -m 0755 libnetware-screensaver.so /usr/lib
	install -m 644 libnetware-screensaver.a /usr/lib
	install -m 644 netware-worms.h /usr/include
	ldconfig
	ldconfig

uninstall: 
	rm -vf /usr/bin/netware-worms
	rm -vf /usr/lib/libnetware-screensaver.so
	rm -vf /usr/lib/libnetware-screensaver.a
	rm -vf /usr/include/netware-worms.h
	ldconfig
	ldconfig


