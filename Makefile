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
LDCONFIG = ldconfig
LIBS = /usr/lib
INCS = /usr/include
BIN = /usr/bin

ARCH := $(shell uname -m)
ifeq ($(ARCH), x86_64)
LIBS = /usr/lib64
endif
ifeq ($(ARCH), aarch64)
LIBS = /usr/lib64
endif
ifeq ($(ARCH), i686)
LIBS = /usr/lib
endif

ifdef LIBDIR
LIBS = $(LIBDIR)
endif

ifdef INCDIR
INCS = $(INCDIR)
endif

ifdef BINDIR
BIN = $(BINDIR)
endif

ifeq ($(NOCHK),1)
LDCONFIG = 
endif

all : utilities

libnetware-screensaver.so: libnetware-screensaver.o 
	$(LD) -shared -lc -o libnetware-screensaver.so libnetware-screensaver.o  

libnetware-screensaver.a: libnetware-screensaver.o
	$(AR) r libnetware-screensaver.a libnetware-screensaver.o  

libnetware-screensaver.o: libnetware-screensaver.c $(INCLUDES)
	$(U_CCP) $(U_CFLAGS_LIBP) -fPIC -Wall libnetware-screensaver.c 

netware-worms: netware-worms.c libnetware-screensaver.a $(INCLUDES)
	$(U_CCP) $(U_CFLAGSP) netware-worms.c libnetware-screensaver.a -Wall -o netware-worms -lncursesw -lpthread -ltinfo

clean:
	rm -rf *.o $(UTILFILES)

utilities: $(UTILFILES)

install: utilities
	install -m 0755 netware-worms $(DESTDIR)$(BIN)
	install -m 0755 libnetware-screensaver.so $(DESTDIR)$(LIBS)
	install -m 644 libnetware-screensaver.a $(DESTDIR)$(LIBS)
	install -m 644 netware-worms.h $(DESTDIR)$(INCS)
	$(LDCONFIG)
	$(LDCONFIG)

uninstall: 
	rm -vf $(DESTDIR)$(BIN)/netware-worms
	rm -vf $(DESTDIR)$(LIBS)/libnetware-screensaver.so
	rm -vf $(DESTDIR)$(LIBS)/libnetware-screensaver.a
	rm -vf $(DESTDIR)$(INCS)/netware-worms.h
	$(LDCONFIG)
	$(LDCONFIG)

