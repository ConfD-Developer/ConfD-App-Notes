#
# Copyright 2020 Tail-f Systems AB
#

CONFD_DIR ?= /usr/local

# Static linking with the ConfD library to avoid ConfD dependencies in the container.
LIBCONFD  = -L$(CONFD_DIR)/lib -l:libconfd.a
CFLAGS   += -Wall -I$(CONFD_DIR)/include

all: confdup

confdup: confdup.c
	$(CC) -o $@ $< $(CFLAGS) $(LIBCONFD) $(LDFLAGS) -lm -lpthread

clean:
	rm -f *.o confdup
