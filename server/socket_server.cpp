/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 20:58:42
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-16 21:29:27
 * @FilePath: /CPP/server/socket_server.cpp
 * @Description: Socket服务端实现
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */

#include "socket_server.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <signal.h>

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

int SocketServer::Socket() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == sockfd) {
                std::cout << "Create socket error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;                
        }
        return 0;
}

int SocketServer::Bind(std::string port) {
        int temp_port;
        temp_port = atoi(port.c_str());

        return Bind(temp_port, AF_INET);
}

int SocketServer::Bind(int port) {
        return Bind(port, AF_INET);
}

int SocketServer::Bind(int port, int type) {
        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = type;//ipv4或ipv6
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(port);
        if (-1 == bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))) {
                std::cout << "Bind error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;  
        }
        return 0;
}

int SocketServer::Listen(int maxlink) {
        if (-1 == listen(sockfd, maxlink)) {
                std::cout << "Listen error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;    
        }
        return 0;
}

int SocketServer::Accept() {
        socklen_t length = sizeof(clientaddr);//需要的内存大小
        connfd = accept(sockfd, (struct sockaddr *)&clientaddr, &length);
        if (-1 == connfd) {
                std::cout << "Accept error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;   
        } else {
                char ip[64] = {0};
                inet_ntop (AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip));
                std::cout << "Client ip : " << ip << ", connect port : " << ntohs(clientaddr.sin_port) << std::endl;
                std::cout << "Accept client connection!" << std::endl;
        }
        set_timeout(connfd, TIMEOUT);
        return 0;
}

int SocketServer::Recv() {
        bzero(recv_buff, BUFFSIZE);
        return recv(connfd, recv_buff, BUFFSIZE-1, 0);
}

int SocketServer::Send() {
        return send(connfd, send_buff, strlen((char *)send_buff), 0);
}

void SocketServer::CloseListenFd() {
        close(sockfd);
}

void SocketServer::CloseConnectFd() {
        close(connfd);
}

void SocketServer::Close() {
        CloseConnectFd();
        CloseListenFd();
}