CFLAGS += -g -Wall -I../client -I../server -lpthread
PROGS = client_multi_thread server_multi_thread

all: $(PROGS)

server_multi_thread: server_test.cpp
	g++ $(CFLAGS) -o $@ $< ../server/socket_server.o

client_multi_thread: client_test.cpp
	g++ $(CFLAGS) -o $@ $< ../client/socket_client.o

clean:
	rm -f $(PROGS)