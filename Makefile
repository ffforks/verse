#
#
#

CC = gcc
AR = ar
ARFLAGS = ru

CFLAGS = -I$(shell pwd) -Wall -ansi

#TUTORIALS = tut_enough tut_connect

TARGETS = libverse.a verse $(TUTORIALS)

LIBVERSE_SRC = $(patsubst v_initialize.c,, \
             $(wildcard v_*.c))
LIBVERSE_OBJ = $(LIBVERSE_SRC:%.c=%.o)

VERSE_SRC = $(wildcard vs_*.c)
VERSE_OBJ = $(VERSE_SRC:%.c=%.o)

TUT_ENOUGH_SRC = tutorials/enough_tutorial.c tutorials/e_storage_node.c
TUT_ENOUGH_OBJ = $(TUT_ENOUGH_SRC:%.c=%.o)

TUT_CONNECT_SRC = tutorial_1_connect.c
TUT_CONNECT_OBJ = $(TUT_CONNECT_SRC:%.c=%.o)

all: $(TARGETS)

libverse.a : $(LIBVERSE_OBJ)
	$(AR) $(ARFLAGS) $@ $(LIBVERSE_OBJ)

verse: $(VERSE_OBJ) libverse.a
	$(CC) -o $@ $^

tut_enough: $(TUT_ENOUGH_OBJ) libverse.a
	$(CC) -o $@ $^

tut_connect: $(TUT_CONNECT_OBJ) libverse.a
	$(CC) -o $@ $^

# -----------------------------------------------------

clean:
	rm -f *.o tutorials/*.o $(TARGETS)
