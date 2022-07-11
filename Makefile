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

ifeq ($(NOCHK),1)
LDCONFIG = 
else
LDCONFIG = ldconfig
endif

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
	install -m 0755 netware-worms $(DESTDIR)/usr/bin
	install -m 0755 libnetware-screensaver.so $(DESTDIR)/usr/lib
	install -m 644 libnetware-screensaver.a $(DESTDIR)/usr/lib
	install -m 644 netware-worms.h $(DESTDIR)/usr/include
	$(LDCONFIG)
	$(LDCONFIG)

uninstall: 
	rm -vf $(DESTDIR)/usr/bin/netware-worms
	rm -vf $(DESTDIR)/usr/lib/libnetware-screensaver.so
	rm -vf $(DESTDIR)/usr/lib/libnetware-screensaver.a
	rm -vf $(DESTDIR)/usr/include/netware-worms.h
	$(LDCONFIG)
	$(LDCONFIG)

