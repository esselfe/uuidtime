
CFLAGS = -std=c17 -Wall -O2 -D_DEFAULT_SOURCE
LDFLAGS =
PROGNAME = uuidtime

.PHONY: all clean

default: all

all: $(PROGNAME)
	@ls -l --color=auto $(PROGNAME)

$(PROGNAME): uuidtime.c
	gcc $(CFLAGS) uuidtime.c -o $(PROGNAME)

clean:
	@rm -v $(PROGNAME) 2>/dev/null || true

