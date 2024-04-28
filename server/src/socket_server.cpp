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


int SocketServer::Socket() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == sockfd) {
                std::cout << "Create socket error(" << errno << "): " << strerror(errno) << std::endl; 
                return -1;                
        }
        return 0;
}

int SocketServer::Bind(std::string ip, std::string port) {
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

        return Bind(temp_ip, temp_port, type);
}

int SocketServer::Bind(int ip, int port, int type) {
        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = type;//ipv4或ipv6
        serveraddr.sin_addr.s_addr = (ip == -1 ? htonl(INADDR_ANY) : htonl(ip));
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