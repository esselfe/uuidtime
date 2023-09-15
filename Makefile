
CFLAGS = -std=c11 -Wall -O2 -D_GNU_SOURCE
LDFLAGS =
PROGNAME = uuidtime

.PHONY: all clean

default: all

all: $(PROGNAME)
	@ls -li --color=auto $(PROGNAME)

$(PROGNAME): uuidtime.c
	gcc $(CFLAGS) uuidtime.c -o $(PROGNAME)

clean:
	@rm -v $(PROGNAME) 2>/dev/null || true

