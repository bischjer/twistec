CC := g++
CXX := g++
CFLAGS := -Wall -Werror
OBJECTS := 
INCLUDES := -I/opt/local/include

.PHONY: clean $^

all: reactor example

debug: reactor_debug example_debug

reactor:
	$(CC) -g reactor.cpp reactor.h log.h -shared -o bin/libreactor.o $(CFLAGS) $(INCLUDES)

reactor_debug:
	$(CC) -g reactor.cpp reactor.h log.h -shared -o bin/libreactor.o $(CFLAGS) $(INCLUDES) -DDEBUG

example:
	$(CC) -g example.cpp bin/libreactor.o -o bin/example $(CFLAGS) $(INCLUDES)

example_debug:
	$(CC) -g example.cpp bin/libreactor.o -o bin/example $(CFLAGS) $(INCLUDES) -DDEBUG



clean:
	rm -f bin/log.o bin/libreactor.o bin/example
