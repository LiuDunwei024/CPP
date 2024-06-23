/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-06-19 21:59:21
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-23 18:27:59
 * @FilePath: /CPP/epoll_select/server_epoll.cpp
 * @Description:  server epoll实现的 主要做两件事1）等待客户端连接 2）接收客户端的数据并且回射
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include "net.h"
#include "socket_server.h"

SocketServer server;

#define MAX_EVENTS 10000
#define LISTEN_PORT 2048

int setblock(int sock) {
        int ret = fcntl(sock, F_SETFL, 0);
        if (ret < 0) {
                hand_error("setblock");
        }
        return 0;
}

int setnoblock(int sock) {
        int ret = fcntl(sock, F_SETFL, O_NONBLOCK);
        if (ret < 0) {
                hand_error("setnonblock");
        }
        return 0;
}

int Create_Socket() {
        return server.Socket();
}

int Bind_Socket() {
        return server.Bind(LISTEN_PORT);
}

int Listen_Socket() {
        return server.Listen(SOMAXCONN);
}

int Accept_Socket() {
        return server.Accept();
}

int main(int argc, char *argv[]) {
        signal(SIGPIPE, SIG_IGN);
        if (Create_Socket() < 0) {
                return -1;
        }
        setnoblock(server.sockfd);
        if (Bind_Socket() < 0) {
                return -1;
        }
        if (Listen_Socket() < 0) {
                return -1;
        }
        //epoll初始化
        vector<int> clients;
        int count = 0;
        int epfd = 0; //epoll的文件描述符
        int ret_events; // epoll_wait()的返回值
        struct epoll_event ev_remove, ev, events[MAX_EVENTS]; //events用来存放从内核读取的事件
        ev.events = EPOLLET | EPOLLIN; //边缘方式触发
        ev.data.fd = server.sockfd;
        
        epfd = epoll_create(MAX_EVENTS); //create_epoll,返回值为epoll的文件描述符
        //epfd = epoll_create(EPOLL_CLOEXEC); //新版写法
        if (epfd < 0) {
                hand_error("epoll_create");
        }
        int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, server.sockfd, &ev); //添加事件
        if (ret < 0) {
                hand_error("epoll_ctl");
        }

        while(1) {
                ret_events = epoll_wait(epfd, events, MAX_EVENTS, -1);//类似于select函数，这里是等待事件的到来
                if (ret_events == -1) {
                        cout << "ret_events = " << ret_events << endl;
                        hand_error("epoll_wait");
                }
                if (ret_events == 0) {
                        cout << "ret_events = " << ret_events << endl;
                        continue;
                }

                for (int num = 0; num < ret_events; ++num) {
                        cout << "num = " << num << endl;
                        cout << "events[num].data.fd = " << events[num].data.fd << endl;
                        if (events[num].data.fd == server.sockfd) {
                                //client connect
                                cout << "listen success and listenfd = " << server.sockfd << endl;
                                if (Accept_Socket() < 0) {
                                        hand_error("accept");
                                }
                                cout << "count = " << count++;
                                clients.push_back(server.connfd);
                                setnoblock(server.connfd); //设置为非阻塞模式
                                ev.data.fd = server.connfd; //将连接也加入epoll的监听队列
                                ev.events = EPOLLIN | EPOLLET;
                                if (epoll_ctl(epfd, EPOLL_CTL_ADD, server.connfd, &ev) < 0) {
                                        hand_error("epoll_ctl");
                                }
                        } else if (events[num].events & EPOLLIN) {
                                server.connfd = events[num].data.fd;
                                if (server.connfd < 0) {
                                        hand_error("server.connfd");
                                }
                                int num = 0;
                                int num = server.Recv();
                                if (num == -1) {
                                        hand_error("read have some problem:");
                                }
                                if (num == 0) {
                                        cout << "client have exit" << endl;
                                        server.CloseConnectFd();
                                        ev_remove = events[num];
                                        epoll_ctl(epfd, EPOLL_CTL_DEL, server.connfd, &ev_remove);
                                        clients.erase(remove(clients.begin(), clients.end(), server.connfd), clients.end());
                                }
                                fputs((char *)server.recv_buff, stdout);
                                memcpy(server.send_buff, server.recv_buff, strlen((char *)server.recv_buff));
                                server.Send();
                        }
                }
        }
        return 0;
}