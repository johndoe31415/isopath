.PHONY: all clean test tests

CFLAGS := $(CFLAGS) -std=c11
CFLAGS += -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=format -Wimplicit-fallthrough -Wshadow
ifneq ($(USER),travis)
CFLAGS += -pie -fPIE -fsanitize=address -fsanitize=undefined -fsanitize=leak
endif
CFLAGS += -O3 -g3
CFLAGS += -mtune=native

OBJS := isopath.o board.o game.o strategy.o

all: isopath

test: all
	./isopath

tests:
	make -C tests test

clean:
	rm -f $(OBJS) isopath

isopath: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

