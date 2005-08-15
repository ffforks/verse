#
# Simplistic Makefile to build the sole example program. Should
# help avoid accidentally breaking it (regressing).
#

ALL=list-nodes

ALL:	$(ALL)
.PHONY: clean

CFLAGS+= -I.. -L..
LDLIBS+= -lverse

# ---------------------------------------------------------------------------

clean:
	rm -f $(ALL)
