CC       = gcc
CFLAGS   = -g -O0 -Wall -std=c99
LIBPQ    = -I/usr/include/postgresql -lpq

TARGETS = main

.c:
	$(CC) $(CFLAGS) $< -o $@ $(LIBPQ)

all: $(TARGETS)

clean:
	rm -f $(TARGETS)