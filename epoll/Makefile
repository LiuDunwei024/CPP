CFLAGS += -g -Wall -I../client -I../server -lpthread -std=c++11
PROGS = server_epoll client_epoll

all: $(PROGS)

server_epoll: server_epoll.cpp
	g++ $(CFLAGS) -o $@ $< ../server/socket_server.o

client_epoll: client_epoll.cpp
	g++ $(CFLAGS) -o $@ $< ../client/socket_client.o

clean:
	rm -f $(PROGS)