/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 20:52:55
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-05-08 21:44:10
 * @FilePath: /repose/CPP/server/src/main.cpp
 * @Description: Socket 服务端主函数
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include <iostream>
#include <string>
#include <cstring>
#include "socket_server.h"

#define MAXLINK 1024
SocketServer socket_server;

int main() {
        std::cout << "Please input ip and port to listen!" << std::endl;
        std::string listenip, port;
        std::cin >> listenip;
        std::cin >> port;

        if (socket_server.Socket() != 0) {
                return -1;
        }
        if (socket_server.Bind(listenip, port) != 0) {
                return -1;
        }
        if (socket_server.Listen(MAXLINK) != 0) {
                return -1;
        }
        if (socket_server.Accept() != 0) {
                return -1;
        }
        std::string send_str;
        send_str = "Hello client, I am server!";
        memcpy(socket_server.send_buff, (void *)send_str.c_str(), send_str.size());
        if (socket_server.Send() == -1) {
                std::cout << "Send fail!" << std::endl;
        }
        std::string recv_str;
        int recv_num = socket_server.Recv();
        memcpy((void *)recv_str.c_str(), socket_server.recv_buff, recv_num);
        std::cout << recv_str << std::endl;
        socket_server.Close();
        return 0;
}