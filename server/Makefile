CFLAGS += -g -Wall
PROGS = socket_server

all: $(PROGS)

server: 

socket_server: main.cpp
	g++ $(CFLAGS) -o $@ $<

clean:
	rm -rf socket_server