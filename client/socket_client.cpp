/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 21:01:07
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditpTime: 2024-05-08 21:09:47
 * @FilePath: /repose/CPP/client/socket_client.cpp
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

int SocketClient::Socket() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == sockfd) {
                std::cout << "Create socket error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;                
        }
        return 0;
}

int SocketClient::Connect(std::string ip, std::string port) {
        int temp_ip, temp_port;
        int type;
        if (ip == "") {
                temp_ip = -1;
                type =AF_INET;
        } else {
                struct in_addr ipv4;
                struct in6_addr ipv6;
                if (inet_pton(AF_INET, ip.c_str(), &ipv4)) {
                        //ip为ipv4
                        inet_pton(AF_INET, ip.c_str(), &temp_ip);
                        type = AF_INET;
                } else if (inet_pton(AF_INET6, ip.c_str(), &ipv6)) {
                        //ip为ipv6
                        inet_pton(AF_INET6, ip.c_str(), &temp_ip);
                        type = AF_INET6;
                } else {
                        return -1;
                }
        }
        temp_port = atoi(port.c_str());

        return Connect(temp_ip, temp_port, type);
}

int SocketClient::Connect(int ip, int port, int type) {
        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = type;//ipv4或ipv6
        serveraddr.sin_addr.s_addr = htonl(ip);
        serveraddr.sin_port = htons(port);
        if (-1 == connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))) {
                std::cout << "Connect error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;  
        }
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
