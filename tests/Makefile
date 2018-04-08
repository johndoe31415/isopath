.PHONY: all test

vpath %.c ..

CFLAGS := -std=c11 -Wall -Werror -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -O3 -I.. -g3 -DBUILD_REVISION='"Test"'
ifneq ($(USER),travis)
# On Travis-CI, gcc does not support "undefined" and "leak" sanitizers.
# Furthermore (and worse, actually), there seems to be a kernel < 4.12.8
# installed which causes the address sanitizer to cause spurious fails ("Shadow
# memory range interleaves with an existing memory mapping. ASan cannot proceed
# correctly. ABORTING."), leading to a broken build. Therefore we do not run
# sanitizers on Travis.
CFLAGS += -pie -fPIE -fsanitize=address -fsanitize=undefined -fsanitize=leak
endif
LDFLAGS :=

TEST_COMMON_OBJS := testbed.o
TEST_OBJS := \
	test_adjacency

all: $(TEST_COMMON_OBJS) $(TEST_OBJS)

test_adjacency: $(TEST_COMMON_OBJS) board.o

test: all
	rm -f tests.log
	for testname in $(TEST_OBJS); do ./$$testname; done
	@./$(firstword $(TEST_OBJS)) --summary

clean:
	rm -f $(TEST_COMMON_OBJS) $(TEST_OBJS) ../*.o *.o
	rm -f helper_surface.o
	rm -f tests.log
	rm -f uitest_instruments

.c:
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -include testbed.h -c -o $@ $<

.s.o:
	$(CC) $(CFLAGS) -c -o $@ $<

gtk_helper.o: gtk_helper.c
	$(CC) $(CFLAGS) `pkg-config --cflags gtk+-3.0` -include testbed.h -c -o $@ $<

uitest_instruments: $(TEST_COMMON_OBJS) uitest_instruments.o instrument_visuals.o frequencies.o surface.o font.o vcr-osd-mono-20.o inconsolata-30.o symbol-font.o morse.o gtk_helper.o
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS) `pkg-config --libs gtk+-3.0`
