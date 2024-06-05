/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 21:20:44
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-05 21:44:07
 * @FilePath: /CPP/client/main.cpp
 * @Description: Socket 客户端主函数
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include <iostream>
#include <string>
#include <cstring>
#include "socket_client.h"

SocketClient socket_client;

int main(int argc, char *argv[]) {
        if (argc < 3) {
                std::cout << "Usage: ./client ip port" << std::endl;
                return -1;
        }
        std::string serverip, port;
        serverip = argv[1];
        port = argv[2];

        if (socket_client.Socket() != 0) {
                return -1;
        }
        if (socket_client.Connect(serverip, port) != 0) {
                return -1;
        }
        int recv_num = socket_client.Recv();
        std::cout << "recv_num " << recv_num << " " << socket_client.recv_buff << std::endl;
        std::string send_str;
        send_str = "Hello server, I am client!";
        memcpy(socket_client.send_buff, (void *)send_str.c_str(), send_str.size());
        if (socket_client.Send() == -1) {
                std::cout << "Send fail!" << std::endl;
        } else {
                std::cout << "Send success!" << std::endl;
        }
        socket_client.Close();
        return 0;
}