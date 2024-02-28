#pragma once
#include "TcpServer.h"
#include<iostream>
#include <functional>
class Handler
{
private:
    /* data */
    std::function<void(TcpServer&, int)> callback_;
public:
    Handler();
    ~Handler();
    void handle(TcpServer& tcpserver, int fd);
};

