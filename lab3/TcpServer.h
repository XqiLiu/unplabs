#pragma once

#include "Socket.h"

class TcpServer :public Socket
{
private:
    int clientsock_,serversock_;
    InetAddress clientaddr_,serveraddr_;
public:
    TcpServer(InetAddress serveraddr);
    ~TcpServer();
    void bind();//server需要用bind
    void listen(int nn=1024);//server需要用listen
    void accept(InetAddress& clientaddr);//server需要用accept

    //virtual
    bool send(const std::string &buffer);
    bool send(const std::string &buffer, int fd);
    bool recv(std::string &buffer,const size_t maxlen);
    bool recv(std::string &buffer,const size_t maxlen,int fd);
    
    void closeconn();
    int getServSocket();
    int getCliSocket();
};