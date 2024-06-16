/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-06-16 16:02:14
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-16 21:25:36
 * @FilePath: /CPP/epoll/server_epoll.cpp
 * @Description: epoll 服务端
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "socket_server.h"

#define MAXEPOLL 256
#define MAXLINE 10
#define OPEN_MAX 100
#define LISTENQ 20
#define SERVER_PORT "2048"
#define INFIM 1000

SocketServer socket_server;

void setnonblocking(int sock) {
        int opts;
        opts = fcntl(sock, F_GETFL);
        if (opts < 0) {
                perror("fcntl(sock, F_GETFL)");
                exit(1);
        }
        opts = opts | O_NONBLOCK;
        if (fcntl(sock, F_SETFL, opts) < 0) {
                perror("fcntl(sock, F_SETFL, opts)");
                exit(1);
        }
}

int main(int argc, char *argv[]) {
        int epfd, nfds, listenfd;
        //epoll_event结构体，ev用于注册事件，events数组用于回传要处理的事件
        struct epoll_event ev, events[20];
        //生成用于处理accpet的epoll专用的文件描述符，指定生成描述符的最大范围为256
        epfd = epoll_create(MAXEPOLL);
        socket_server.Socket();

        setnonblocking(socket_server.sockfd); //把用于监听的socket的sockfd设置为非阻塞方式
        ev.data.fd = socket_server.sockfd;    //设置于要处理的事件相关的文件描述符
        ev.events = EPOLLIN | EPOLLET;        //设置要处理的事件类型
        epoll_ctl(epfd, EPOLL_CTL_ADD, socket_server.sockfd, &ev); //注册epoll事件
        socket_server.Bind(SERVER_PORT);
        socket_server.Listen(LISTENQ);

        for (;;) {
                nfds = epoll_wait(epfd, events, 20, 500); //等待epoll事件发生
                int i = 0;
                for (i = 0; i < nfds; ++i) {
                        if (events[i].data.fd == socket_server.sockfd) //监听事件
                        {
                                if (-1 == socket_server.Accept()) {
                                        perror("socket_server.connfd < 0");
                                        exit(1);
                                }
                                setnonblocking(socket_server.connfd); //设置客户端的connfd为非阻塞方式
                                ev.data.fd = socket_server.connfd;    //设置用于读操作的文件描述符
                                ev.events = EPOLLIN | EPOLLET;        //设置用于注册的读操作事件
                                epoll_ctl(epfd, EPOLL_CTL_ADD, socket_server.connfd, &ev); //注册ev事件
                        } else if (events[i].events & EPOLLIN) { //读事件
                                if (events[i].data.fd < 0) {
                                        continue;
                                }
                                int n = 0;
                                if ((n = socket_server.Recv()) < 0) {
                                        std::cout << "Read error!" << std::endl;
                                } else if (n == 0) {
                                        socket_server.CloseConnectFd();
                                        events[i].data.fd = -1;
                                } else {
                                        std::cout << "Recv message : " << socket_server.recv_buff << std::endl;
                                }
                                ev.data.fd = socket_server.connfd; //设置用于写操作的文件描述符
                                ev.events = EPOLLOUT | EPOLLET;    //设置用于注册的写操作事件
                                //修改要处理的事件为EPOLLOUT
                                epoll_ctl(epfd, EPOLL_CTL_MOD, socket_server.connfd, &ev);
                        } else if (events[i].events & EPOLLOUT) { //写事件
                                std::string send_str;
                                send_str = "Hello client, I am server!";
                                memcpy(socket_server.send_buff, (void *)send_str.c_str(), send_str.size());
                                socket_server.Send();
                                ev.data.fd = socket_server.connfd; //设置用于读操作的文件描述符
                                ev.events = EPOLLIN | EPOLLET;    //设置用于注册的读操作事件
                                //修改要处理的事件为EPOLLOUT
                                epoll_ctl(epfd, EPOLL_CTL_MOD, socket_server.connfd, &ev);
                        }
                }
        }

        return 0;
}
