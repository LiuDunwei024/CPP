CFLAGS += -g -Wall
PROGS = socket_client.o client

all: $(PROGS)

socket_client.o: socket_client.cpp socket_client.h
	g++ $(CFLAGS) -c -o $@ $<

client: main.cpp
	g++ $(CFLAGS) -o $@ $< socket_client.o

clean:
	rm -rf client socket_client.o