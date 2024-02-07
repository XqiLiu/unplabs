#pragma once

#include "Socket.h"

class TcpServer :public Socket
{
private:

public:
    TcpServer(InetAddress serveraddr);
    ~TcpServer();
    void bind();//server需要用bind
    void listen(int nn=1024);//server需要用listen
    void accept(InetAddress& clientaddr);//server需要用accept

    //virtual
    bool send(const std::string &buffer);
    bool recv(std::string &buffer,const size_t maxlen);
    void closeconn();
    int clientsock_,serversock_;
    InetAddress clientaddr_,serveraddr_;
};