/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 21:20:44
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-05-08 21:54:06
 * @FilePath: /repose/CPP/client/src/main.cpp
 * @Description: Socket 客户端主函数
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include <iostream>
#include <string>
#include <cstring>
#include "socket_client.h"

SocketClient socket_client;

int main() {
        std::cout << "Please input server ip and port!" << std::endl;
        std::string serverip, port;
        std::cin >> serverip >> port;

        if (socket_client.Socket() != 0) {
                return -1;
        }
        if (socket_client.Connect(serverip, port) != 0) {
                return -1;
        }

        std::string recv_str;
        int recv_num = socket_client.Recv();
        memcpy((void *)recv_str.c_str(), socket_client.recv_buff, recv_num);
        std::cout << recv_str << std::endl;
        std::string send_str;
        send_str = "Hello server, I am client!";
        memcpy(socket_client.send_buff, (void *)send_str.c_str(), send_str.size());
        if (socket_client.Send() == -1) {
                std::cout << "Send fail!" << std::endl;
        }
        socket_client.Close();
        return 0;
}