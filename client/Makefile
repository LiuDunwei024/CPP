CFLAGS += -g -Wall
PROGS = socket_client

all: $(PROGS)

socket_client: main.cpp socket_client.h
	g++ $(CFLAGS) -o $@ $<

clean:
	rm -rf socket_client