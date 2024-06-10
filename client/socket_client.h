/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-05-08 21:00:59
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-10 14:18:07
 * @FilePath: /CPP/client/socket_client.h
 * @Description: Socket客户端头文件
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#include <netinet/in.h>
#include <string>

#define BUFFSIZE 2048
#define TIMEOUT 30

class SocketClient {
public:
        SocketClient() {}
        ~SocketClient() {}

        //创建socket
        int Socket();

        //建立连接
        int Connect(std::string ip, std::string port);
        int Connect(std::string ip, int port, int type);

        //接收
        int Recv();

        //发送
        int Send();

        void Close();

        //接收缓存
        uint8_t recv_buff[BUFFSIZE];
        //发送缓存
        uint8_t send_buff[BUFFSIZE];
private:
        //创建socket的文件描述符
        int sockfd;
        //连接的服务端ip
        struct sockaddr_in serveraddr;
};
