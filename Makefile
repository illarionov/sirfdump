CC?=gcc
CFLAGS?= -O2 -s -pipe
CFLAGS+= -DNDEBUG
#CFLAGS=-W  -Wall -g -O0 -DVERBOSITY=LOG_RAW

CFLAGS+= -D_ENDIAN_LITTLE -I./include -I./util/codec -I./util/proto -I./pal

LDFLAGS+= -lm

DESTDIR?=/usr/local

UNAME_O:= $(shell uname -o)

ifeq ($(UNAME_O),GNU/Linux)
	NO_STRLCPY=1
endif

ifeq ($(UNAME_O),Msys)
	NO_STRLCPY=1
	CFLAGS+= -posix -D__USE_MINGW_ANSI_STDIO=1
endif


OBJS=	sirf_codec_ssb.o \
	sirf_codec_ascii.o \
	sirf_codec_nmea.o \
	output_dump.o \
	output_nmea.o \
	output_rinex.o \
	output_rinex_nav.o \
	isgps.o \
	subframe.o

ifdef NO_STRLCPY
	OBJS += strlcat.o
endif

all: sirfdump

clean:
	rm -f *.o sirfdump

sirfdump: ${OBJS} sirfdump.c sirfdump.h
	$(CC) $(CFLAGS) $(LDFLAGS) \
	sirfdump.c ${OBJS} \
	-o sirfdump

sirf_codec_ssb.o: util/codec/sirf_codec_ssb.c
	$(CC) $(CFLAGS) -c util/codec/sirf_codec_ssb.c

sirf_codec_ascii.o: util/codec/sirf_codec_ascii.c
	$(CC) $(CFLAGS) -c util/codec/sirf_codec_ascii.c

sirf_codec_nmea.o: util/codec/sirf_codec_nmea.c
	$(CC) $(CFLAGS) -DSIRF_CODEC_NMEA -c util/codec/sirf_codec_nmea.c

output_dump.o: output_dump.c sirfdump.h
	$(CC) $(CFLAGS) -c output_dump.c

output_nmea.o: output_dump.c sirfdump.h
	$(CC) $(CFLAGS) -c output_nmea.c

output_rinex.o: output_rinex.c sirfdump.h
	$(CC) $(CFLAGS) -c output_rinex.c

output_rinex_nav.o: output_rinex_nav.c sirfdump.h
	$(CC) $(CFLAGS) -c output_rinex_nav.c

subframe.o: gpsd/gps.h gpsd/subframe.c
	$(CC) $(CFLAGS) -c gpsd/subframe.c

isgps.o: gpsd/gps.h gpsd/isgps.c
	$(CC) $(CFLAGS) -c gpsd/isgps.c

strlcat.o: compat/strlcat.c
	$(CC) $(CFLAGS) -c compat/strlcat.c

install:
	mkdir -p ${DESTDIR}/bin 2> /dev/null
	cp -p sirfdump ${DESTDIR}/bin

