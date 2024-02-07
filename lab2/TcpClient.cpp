#include "TcpClient.h"

TcpClient::TcpClient(InetAddress addr):clientaddr(addr)
{TcpClient::clientsock_=createClientFd();

}

TcpClient::~TcpClient()
{
    TcpClient::closeconn();
}

// void TcpClient::Tcpconnect(const InetAddress& servaddr)
// {

//     sock_.connect(servaddr);
    
// }
void TcpClient::Tcpconnect(const InetAddress& servaddr)
{
    if (::connect(clientsock_,servaddr.getAddr(),sizeof(servaddr)) != 0)
    {
        perror("connect error");
        close(clientsock_);
        exit(-1);
    };
}
bool TcpClient::send(const std::string &buffer)
{   if (TcpClient::clientsock_==-1)
        return false;
    if (::send(clientsock_, buffer.data(), buffer.size(),0)<=0 )
        {
            return false;
        }
    return true;
    
}
 bool TcpClient::recv(std::string &buffer,const size_t maxlen)
 {
    buffer.clear();
    buffer.resize(maxlen);
    int readn=::recv(TcpClient::clientsock_,& buffer[0], buffer.size(),0);
    if (readn<=0)
    {   
        buffer.clear();
        return false;
    }    
    buffer.resize(readn);
    return true;
    /*if ((iret=recv(client.sock_.getFd(), buffer, MAXLINE,0))<=0)
    {
        // printf("recv msg:%s\n",buffer);
        std::cout <<"iret="<<iret;
        
    }*/
 }
 void TcpClient::closeconn()
 {
    if(TcpClient::clientsock_!=-1)
    {
        ::close(TcpClient::clientsock_);
    }
 }