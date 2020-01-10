#
# Copyright 2009 Tail-f Systems AB
#

CONFD_DIR ?= /usr/local

#LIBCONFD  = -L$(CONFD_DIR)/lib -lconfd #-l:libconfd.a
LIBCONFD  = -L$(CONFD_DIR)/lib -l:libconfd.a
CFLAGS   += -Wall -I$(CONFD_DIR)/include

all: confd_cmd confd_load

confd_cmd: confd_cmd.c
	$(CC) -o $@ $< $(CFLAGS) $(LIBCONFD) $(LDFLAGS) -lm -lpthread

confd_load: confd_load.c
	$(CC) -o $@ $< $(CFLAGS) $(LIBCONFD) $(LDFLAGS) -lm -lpthread

clean:
	rm -f *.o confd_cmd confd_load
