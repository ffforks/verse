#
# Makefile for Verse core; API and reference server.
# This pretty much requires GNU Make, I think.
#

CC = gcc
AR = ar
ARFLAGS = ru

CFLAGS = -I$(shell pwd) -Wall -ansi

TARGETS = libverse.a verse

LIBVERSE_SRC = $(patsubst v_initialize.c,, \
             $(wildcard v_*.c))
LIBVERSE_OBJ = $(LIBVERSE_SRC:%.c=%.o)

VERSE_SRC = $(wildcard vs_*.c)
VERSE_OBJ = $(VERSE_SRC:%.c=%.o)

# -----------------------------------------------------

all: $(TARGETS)

libverse.a:	libverse.a($(LIBVERSE_OBJ))

verse: $(VERSE_OBJ) libverse.a
	$(CC) -o $@ $^

# -----------------------------------------------------

clean:
	rm -f *.o $(TARGETS)
