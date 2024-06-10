/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 21:01:07
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditpTime: 2024-05-08 21:09:47
 * @FilePath: /CPP/client/socket_client.cpp
 * @Descrition: Socket客户端实现
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include "socket_client.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

void set_timeout(int sock_fd, int timeout) {
        struct timeval tv;
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) < 0) {
                std::cout << "接收超时设置错误！" << std::endl;
        }

        if (setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(tv)) < 0) {
                std::cout << "发送超时设置错误！" << std::endl;
        }
}

int SocketClient::Socket() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == sockfd) {
                std::cout << "Create socket error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;                
        }
        return 0;
}

int SocketClient::Connect(std::string ip, std::string port) {
        int temp_port;
        int type = AF_INET;
        temp_port = atoi(port.c_str());

        return Connect(ip, temp_port, type);
}

int SocketClient::Connect(std::string ip, int port, int type) {
        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = type;//ipv4或ipv6
        serveraddr.sin_addr.s_addr = inet_addr(ip.c_str());
        serveraddr.sin_port = htons(port);
        if (-1 == connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))) {
                std::cout << "Connect error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;  
        } else {
                std::cout << "Server ip: " << ip << ", server port: " << port << std::endl;
                std::cout << "Connect server successfully!" << std::endl;
        }
        set_timeout(sockfd, TIMEOUT);
        return 0;
}

int SocketClient::Recv() {
        bzero(recv_buff, BUFFSIZE);
        return recv(sockfd, recv_buff, BUFFSIZE-1, 0);
}

int SocketClient::Send() {
        return send(sockfd, send_buff, strlen((char *)send_buff), 0);
}

void SocketClient::Close() {
        close(sockfd);
}
