/*
 * @Author: Dunwei Liu llldddwwwc@outlook.com
 * @Date: 2024-06-19 21:55:19
 * @LastEditors: Dunwei Liu llldddwwwc@outlook.com
 * @LastEditTime: 2024-06-19 21:58:28
 * @FilePath: /CPP/epoll_select/net.h
 * @Description: net.h 头文件
 * 
 * Copyright (c) 2024 by Dunwei Liu llldddwwwc@outlook.com, All Rights Reserved. 
 */
#ifndef _NET_H
#define _NET_H

#include <iostream>
#include <vector>
#include <algorithm>

#include <stdio.h>
#include <sys/types.h>
#include <sys/epoll.h>  //epoll ways file
#include <fcntl.h>      //block and noblock

#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

using namespace std;

#define hand_error(msg) do{perror(msg); exit(EXIT_FAILURE);}while(0)
#endif