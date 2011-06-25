CC?=gcc
CFLAGS?= -O2 -s -pipe
CFLAGS+= -DNDEBUG
CFLAGS+= -I/usr/local/include
CFLAGS=-W -Wall -g -O0 -I/usr/local/include

DESTDIR?=/usr/local

all: sirflog

clean:
	rm -f *.o sirflog

ya_get_nf_direct: sirflog.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
	sirflog.c \
	-o sirflog

install:
	mkdir -p ${DESTDIR}/bin 2> /dev/null
	cp -p sirflog ${DESTDIR}/bin

