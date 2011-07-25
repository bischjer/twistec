CC="g++"

all:
	reactor

reactor:
	$(CC) reactor.cpp reactor.h -shared -o bin/libreactor.so

example:
	$(CC) example.cpp -I./bin -L./bin -lreactor -o bin/example