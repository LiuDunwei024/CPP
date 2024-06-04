CFLAGS += -g -Wall
PROGS = socket_server.o server

all: $(PROGS)

socket_server.o: socket_server.cpp socket_server.h
	g++ $(CFLAGS) -c -o $@ $<

server: main.cpp
	g++ $(CFLAGS) -o $@ $< socket_server.o

clean:
	rm -rf server socket_server.o