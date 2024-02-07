#pragma once

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include "InetAddress.h"
// 
int createListenFd();
int createClientFd();
class Socket{
protected:
    // const int fd_;
    // InetAddress addr_;

public:
    Socket();
    ~Socket();
 
    // int getFd() const;

    // void bind(const InetAddress& servaddr);//server需要用bind
    // void listen(int nn=128);//server需要用listen
    // int accept(InetAddress& clientaddr);//server需要用accept
    //void connect(const InetAddress& servaddr);//client 需要用connect

    virtual bool send(const std::string &buffer) = 0;
    virtual bool recv(std::string &buffer,const size_t maxlen) = 0;
    virtual void closeconn() = 0;

};