/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-06-09 21:35:05
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-09 22:31:14
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

        if (-1 == server.Accept()) {
                return -1;
        }

        return 0;
}

void close_socket() {
        server.Close();
        return ;
}

void *send_message(void *arg) {
        std::string send_str;
        while(getline(std::cin, send_str)) {
                memset(server.send_buff, 0, BUFFSIZE);
                memcpy(server.send_buff, send_str.c_str(), send_str.size());
                server.send_buff[send_str.size()] = '\0';
                if (server.Send() == send_str.size()) {
                        std::cout << "\"" <<send_str << "\" has been sent successfully!" << std::endl;
                        if (send_str == "Exit" || send_str == "exit") {
                                return NULL;
                        }
                } else {
                        return NULL;
                }
        }

}

void *recv_message(void *arg) {
        while (1) {
                while(0 != server.Recv()) {
                        if (memcmp(server.recv_buff, "Exit", sizeof("Exit"))|| 
                            memcmp(server.recv_buff,"exit", sizeof("exit"))) {
                                return NULL;
                            }
                        std::cout << "Recv message : " << server.recv_buff << std::endl;
                }
        }
}

int main(int argc, char *argv[]) {
        if (0 != init_socket()) {
                return -1;
        }

        pthread_create(&read_thread, NULL, recv_message, NULL);
        pthread_create(&write_thread, NULL, send_message, NULL);

        pthread_join(read_thread, NULL);
        pthread_join(write_thread, NULL);

        close_socket();

        return 0;
}
