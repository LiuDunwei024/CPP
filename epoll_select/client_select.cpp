/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-06-23 18:28:57
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-23 18:47:59
 * @FilePath: /CPP/epoll_select/client_select.cpp
 * @Description: client select实现
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include "net.h"

#include "socket_client.h"

SocketClient client;

int Create_Socket() {
        return client.Socket();
}

int Connect_Socket(string serverip, string port) {
        return client.Connect(serverip, port);
}

int main(int argc, char *argv[]) {
        if (argc < 3) {
                cout << "Usage: ./client ip port" << endl;
                return -1;
        }
        std::string serverip, port;
        serverip = argv[1];
        port = argv[2];
        
        signal(SIGPIPE, SIG_IGN);
        if (Create_Socket() < 0) {
                hand_error("socket_create");
        }
        if (Connect_Socket(serverip, port) != 0) {
                hand_error("connect");
        }

        fd_set rset;
        FD_ZERO(&rset);

        int nready = 0;
        int maxfd;
        int stdinof = fileno(stdin);
        if (stdinof > client.sockfd) {
                maxfd = stdinof;
        } else {
                maxfd = client.sockfd;
        }

        while (1) {
                //select返回后把原来待检测的但是仍没有就绪的描述字清0了。
                //每次调用select都要重新设置下待检测的描述符
                FD_SET(client.sockfd, &rset);
                FD_SET(stdinof, &rset);
                nready = select(maxfd+1, &rset, NULL, NULL, NULL);
                cout << "nready = " << nready << " maxfd = " << maxfd << endl;
                if (nready == 1) {
                        break;
                } else if (nready == 0) {
                        continue;
                } else {
                        if (FD_ISSET(client.sockfd, &rset)) //检测sock是否已经在集合rset里面
                        {
                                int ret = client.Recv();
                                if (ret == -1) {
                                        hand_error("read");
                                } else if (ret == 0) {
                                        cout << "server have closed" << endl;
                                        client.Close();
                                        break;
                                } else {
                                        fputs((char *)client.recv_buff, stdout);//输出数据
                                }
                        }

                        if (FD_ISSET(stdinof, &rset)) //检测stdin的文件描述符是否已经在集合rset里面
                        {
                                if (fgets((char *)client.send_buff, sizeof(client.send_buff), stdin) != NULL) {
                                        int num = client.Send();//写数据
                                        cout << "send num = " << num << endl;
                                }
                        }
                }
        }
        return 0;
}