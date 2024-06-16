/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-06-09 21:35:05
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-10 20:35:37
 * @FilePath: /CPP/multi_thread/client_test.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include <iostream>
#include <string.h>

#include "socket_client.h"

SocketClient client;

pthread_t read_thread;
pthread_t write_thread;

int init_socket() {
        if ( -1 == client.Socket()) {
                return -1;
        }

        return 0;
}

void close_socket() {
        client.Close();
        return ;
}

void *send_message(void *arg) {
        std::string send_str;
        while(1) {
                std::cin >> send_str;
                memset(client.send_buff, 0, BUFFSIZE);
                memcpy(client.send_buff, send_str.c_str(), send_str.size());
                client.send_buff[send_str.size()] == '\0';
                if (client.Send() == send_str.size()) {
                        std::cout << "\"" <<send_str << "\" has been sent successfully!" << std::endl;
                        return NULL;
                }
        }

}

void *recv_message(void *arg) {
        while (1) {
                while (0 != client.Recv()) {
                        std::cout << "Recv message : " << client.recv_buff << std::endl;
                        return NULL;
                }
        }
}

int main(int argc, char *argv[]) {
        if (argc < 3) {
                std::cout << "Usage : ./client ip port" << std::endl;
                return -1;
        }
        std::string ip = argv[1];
        std::string port = argv[2];

        if (0 != init_socket()) {
                return -1;
        }

        if (-1 == client.Connect(ip, port)) {
                return -1;
        }
        pthread_create(&read_thread, NULL, recv_message, NULL);
        pthread_create(&write_thread, NULL, send_message, NULL);
        pthread_join(read_thread, NULL);
        pthread_join(write_thread, NULL);

        close_socket();

        return 0;
}
