/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 20:59:41
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-04 21:46:55
 * @FilePath: /CPP/server/socket_server.h
 * @Description: Socket服务端头文件
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */

#include <netinet/in.h>
#include <string>

#define BUFFSIZE 2048
#define TIMEOUT 30

class SocketServer {
public:
        SocketServer() {}
        ~SocketServer() {}

        //创建socket
        int Socket();

        //绑定
        int Bind(std::string port);
        int Bind(int port, int type);
        
        //监听
        int Listen(int maxlink);

        //连接
        int Accept();

        //接收
        int Recv();

        //发送
        int Send();

        void CloseListenFd();

        void CloseConnectFd();
        
        void Close();

        //接收缓存
        uint8_t recv_buff[BUFFSIZE];

        //发送缓存
        uint8_t send_buff[BUFFSIZE];
private:
        //创建socket的文件描述符
        int sockfd;
        //服务端绑定监听ip
        struct sockaddr_in serveraddr;
        //服务端绑定监听ip
        struct sockaddr_in clientaddr;
        //建立连接接收、发送文件描述符
        int connfd;
};