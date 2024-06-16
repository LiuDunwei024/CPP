/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-06-09 21:35:05
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-15 22:50:47
 * @FilePath: /CPP/multi_thread/server_test.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include <iostream>
#include <string.h>

#include "socket_server.h"

#define LINSTEN_PORT "2048"
#define MAX_LINK 64

SocketServer server;

pthread_t read_thread;
pthread_t write_thread;

int init_socket() {
        if ( -1 == server.Socket()) {
                return -1;
        }

        if (-1 == server.Bind(LINSTEN_PORT)) {
                return -1;
        }

        if (-1 == server.Listen(MAX_LINK)) {
                return -1;
        }

        return 0;
}

int accept_connect() {
        return server.Accept();
}

void close_socket() {
        server.CloseListenFd();
}

void *send_message(void *arg) {
        std::string send_str;
        while(true) {
                std::cin >> send_str; 
                memset(server.send_buff, 0, BUFFSIZE);
                memcpy(server.send_buff, send_str.c_str(), send_str.size());
                server.send_buff[send_str.size()] = '\0';
                if (server.Send() == (int) send_str.size()) {
                        std::cout << "\"" <<send_str << "\" has been sent successfully!" << std::endl;
                        break;
                } else {
                        break;
                }
        }
        return NULL;
}

void *recv_message(void *arg) {
        while (1) {
                while(0 != server.Recv()) {
                        std::cout << "Recv message : " << server.recv_buff << std::endl;
                        return NULL;
                }
        }
        return NULL;
}

int main(int argc, char *argv[]) {
        if (0 != init_socket()) {
                return -1;
        }

        while (1) {
                if (0 == accept_connect()){
                        pthread_create(&read_thread, NULL, recv_message, NULL);
                        pthread_create(&write_thread, NULL, send_message, NULL);
                        pthread_join(read_thread, NULL);
                        pthread_join(write_thread, NULL);
                        server.CloseConnectFd();
                } else if (-1 == accept_connect()){
                        break;
                }
        }

        close_socket();

        return 0;
}
