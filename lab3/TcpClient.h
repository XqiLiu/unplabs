#pragma once

#include <string>
#include "Socket.h"
#include "InetAddress.h"

class TcpClient :public Socket
{
private:
    int clientsock_;
    InetAddress clientaddr;
public:
    // Socket sock_; 
    TcpClient(InetAddress addr);
    ~TcpClient();
    void Tcpconnect(const InetAddress& servaddr);

    //virtual
    bool send(const std::string &buffer);
    bool recv(std::string &buffer,const size_t maxlen);
    void closeconn();
    

};