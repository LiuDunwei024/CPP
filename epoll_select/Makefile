CFLAGS += -g -Wall -I../client -I../server -lpthread -std=c++11
PROGS = server client

all: $(PROGS)

server: server_epoll.cpp
	g++ $(CFLAGS) -o $@ $< ../server/socket_server.o

client: client_select.cpp
	g++ $(CFLAGS) -o $@ $< ../client/socket_client.o

clean:
	rm -f $(PROGS)