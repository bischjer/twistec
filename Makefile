CC=g++
CFLAGS=-Wall -Werror
INC=-I/opt/local/include
all: reactor example

debug: reactor_debug example_debug

reactor:
	$(CC) -g reactor.cpp reactor.h log.h -shared -o bin/libreactor.o $(CFLAGS) $(INC)

reactor_debug:
	$(CC) -g reactor.cpp reactor.h log.h -shared -o bin/libreactor.o $(CFLAGS) $(INC) -DDEBUG

example:
	$(CC) -g example.cpp bin/libreactor.o -o bin/example $(CFLAGS) $(INC)

example_debug:
	$(CC) -g example.cpp bin/libreactor.o -o bin/example $(CFLAGS) $(INC) -DDEBUG

.PHONY: clean

clean:
	rm -f bin/log.o bin/libreactor.o bin/example
