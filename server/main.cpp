/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 20:52:55
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-04 22:19:17
 * @FilePath: /CPP/server/main.cpp
 * @Description: Socket 服务端主函数
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include <iostream>
#include <string>
#include <cstring>
#include "socket_server.h"

#define MAXLINK 20
SocketServer socket_server;

int main() {
        std::cout << "Please input port to listen!" << std::endl;
        std::string port;
        // std::cin >> listenip;
        // std::cin >> port;
        port = "1234";

        if (socket_server.Socket() != 0) {
                return -1;
        }
        if (socket_server.Bind(port) != 0) {
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
        } else {
                std::cout << "Send success!" << std::endl;
        }
        int recv_num = socket_server.Recv();
        std::cout << "recv_num " << recv_num << " " << socket_server.recv_buff << std::endl;
        socket_server.Close();
        
        return 0;
}