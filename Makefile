CC=g++
CFLAGS=-Wall -Werror

all: reactor example

reactor:
	$(CC) reactor.cpp reactor.h -shared -o bin/libreactor.so $(CFLAGS)

example:
	$(CC) example.cpp -I./bin -L./bin -lreactor -o bin/example $(CFLAGS)

.PHONY: clean

clean:
	rm -f bin/libreactor.so bin/example
