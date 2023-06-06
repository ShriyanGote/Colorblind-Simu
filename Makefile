CC=clang
DEBUG_CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic -g
RELEASE_CFLAGS=-Wall -Wextra -Wstrict-prototypes -Werror -pedantic
LDFLAGS=-lm

COLORB_EXECBIN=colorb
IOTEST_EXECBIN=iotest

COMMON_OBJS=\
    bmp.o \
    io.o

COLORB_OBJS=\
    colorb.o \
    $(COMMON_OBJS)

IOTEST_OBJS=\
    iotest.o \
    $(COMMON_OBJS)

.PHONY: all debug release clean format

all: debug $(IOTEST_EXECBIN)

debug: CFLAGS=$(DEBUG_CFLAGS)
debug: $(COLORB_EXECBIN)

release: CFLAGS=$(RELEASE_CFLAGS)
release: $(COLORB_EXECBIN)

$(COLORB_EXECBIN): $(COLORB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(IOTEST_EXECBIN): $(IOTEST_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(COLORB_EXECBIN) $(COLORB_OBJS) $(IOTEST_EXECBIN) $(IOTEST_OBJS)

format:
	clang-format -i --style=file *.[ch]
