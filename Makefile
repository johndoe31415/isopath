.PHONY: all clean test

CFLAGS := $(CFLAGS) -std=c11
CFLAGS += -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=format -Wimplicit-fallthrough -Wshadow
CFLAGS += -O3 -g3
CFLAGS += -mtune=native

OBJS := isopath.o board.o

all: isopath

test: all
	./isopath

clean:
	rm -f $(OBJS) isopath

isopath: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

