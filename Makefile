CC=g++
CFLAGS=-Wall -Werror

all: log reactor example

log:
	$(CC) -g log.cpp log.h -shared -o bin/log.o $(CFLAGS)

reactor:
	$(CC) -g reactor.cpp reactor.h bin/log.o -shared -o bin/libreactor.o $(CFLAGS)

example:
	$(CC) -g example.cpp bin/libreactor.o -o bin/example $(CFLAGS)

.PHONY: clean

clean:
	rm -f bin/log.o bin/libreactor.o bin/example
