CC?=gcc
CFLAGS?= -O2 -s -pipe
CFLAGS+= -DNDEBUG
CFLAGS=-W  -Wall -g -O0 -I/usr/local/include -I./include -I./util/codec -I./util/proto -I./pal

CFLAGS+= -D_ENDIAN_LITTLE -I./include -I./util/codec -I./util/proto

DESTDIR?=/usr/local

all: sirfdump

clean:
	rm -f *.o sirfdump

sirfdump: sirf_codec_ssb.o sirf_codec_ascii.o sirfdump.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
	sirfdump.c sirf_codec_ssb.o sirf_codec_ascii.o \
	-o sirfdump

sirf_codec_ssb.o: util/codec/sirf_codec_ssb.c
	$(CC) $(CFLAGS) -c util/codec/sirf_codec_ssb.c \

sirf_codec_ascii.o: util/codec/sirf_codec_ascii.c
	$(CC) $(CFLAGS) -c util/codec/sirf_codec_ascii.c \

install:
	mkdir -p ${DESTDIR}/bin 2> /dev/null
	cp -p sirfdump ${DESTDIR}/bin

