# sdweb makefile

CC = emcc

DEFS = -static -Wall -Wno-switch -Wno-uninitialized -Wno-deprecated \
			 -Wno-char-subscripts -Wno-narrowing \
			 -fwasm-exceptions

CDEBUGFLAGS = -O3

CFLAGS = $(CDEBUGFLAGS) $(DEFS)

TTY_FLAGS =

SDLIB_OBJS = sdmain.o sdutil.o sdbasic.o sdinit.o \
             sdtables.o sdctable.o sdtop.o sdconcpt.o sdpreds.o \
             sdgetout.o sdmoves.o sdtand.o sdconc.o sdistort.o \
             mapcachefile.o sdpick.o sdsi.o sdmatch.o common.o

HFILES = sdui.h sd.h database.h paths.h mapcachefile.h sort.h \
             deploy.h resource.h sdprint.h sdmatch.h sdchars.h

SDWEB_OBJS = sdweb/sdui-web.o

SDWEB_LINK_OBJS = $(SDLIB_OBJS) sdui-web.o

MKCALLS_OBJS = mkcalls.o common.o

all: sd_calls.dat sdweb.js

prepare:
	rsync --archive --update --recursive --exclude '*.o' ../sd/ .

sdweb.html: prepare $(SDLIB_OBJS) $(SDWEB_OBJS) sd_calls.dat
	$(CC) $(CFLAGS) -o $@ $(SDWEB_LINK_OBJS) --preload-file sd_calls.dat

sdweb.js: prepare $(SDLIB_OBJS) $(SDWEB_OBJS) sd_calls.dat
	$(CC) $(CFLAGS) -o $@ $(SDWEB_LINK_OBJS) --preload-file sd_calls.dat

sd_calls.dat:
	$(MAKE) -C ../sd -f makefile.linux64 sd_calls.dat
	mv ../sd/sd_calls.dat .

.SUFFIXES: .c .cpp

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<

sd.h sdui.h: database.h sdchars.h

mkcalls.o sdmain.o sdsi.o: paths.h

common.o mkcalls.o: database.h

sdinit.cpp: mapcachefile.h sort.h

mapcachefile.cpp: mapcachefile.h

$(SDLIB_OBJS) $(SDWEB_OBJS): sdui.h sd.h sdchars.h database.h paths.h

clean:
	find . -type f ! -name 'Makefile' -delete
