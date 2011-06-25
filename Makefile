CC?=gcc
CFLAGS?= -O2 -s -pipe
CFLAGS+= -DNDEBUG
CFLAGS+= -I/usr/local/include -I./include
CFLAGS=-W -Wall -g -O0 -I/usr/local/include -I./include

DESTDIR?=/usr/local

all: sirfdump

clean:
	rm -f *.o sirfdump

sirfdump: sirfdump.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
	sirfdump.c \
	-o sirfdump

install:
	mkdir -p ${DESTDIR}/bin 2> /dev/null
	cp -p sirfdump ${DESTDIR}/bin

