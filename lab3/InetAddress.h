#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

class InetAddress
{
private:
    sockaddr_in addr_;
public:
    InetAddress();
    InetAddress(const std::string &ip,uint16_t port);//建立listenfd
    InetAddress(const sockaddr_in addr):addr_(addr){}//连接进来的
    ~InetAddress();

    const char *getIp( )const;
    uint16_t getPort()const;
    const sockaddr* getAddr()const;
    void setaddr(sockaddr_in clientaddr);

};