#include "TcpServer.h"

/*class TcpServer :public Socket
{
private:
    int clientsock_,serversock_;
    InetAddress clientadd_r,serveraddr_;
public:
    TcpServer(InetAddress serveraddr);
    ~TcpServer();
    void bind(const InetAddress& servaddr);//server需要用bind
    void listen(int nn=128);//server需要用listen
    int accept(InetAddress& clientaddr);//server需要用accept

    //virtual
    bool send(const std::string &buffer);
    bool recv(std::string &buffer,const size_t maxlen);
    void closeconn();
    
};*/

TcpServer::TcpServer(InetAddress serveraddr):serveraddr_(serveraddr)
{
    TcpServer::serversock_=createListenFd();
    printf("服务端执行监听于 %s:%d\n",serveraddr_.getIp(),serveraddr_.getPort());
    TcpServer::bind();
    TcpServer::listen(1024);
}
TcpServer::~TcpServer()
{TcpServer::closeconn();
}
void TcpServer::bind()
{
    if(::bind(TcpServer::serversock_, serveraddr_.getAddr(),sizeof(sockaddr)) < 0)
    {
        perror("bind error");
        close(serversock_);
        exit(-1);
    }
}
// void Socket::bind(const InetAddress& servaddr)
// {
//     if(::bind(fd_,servaddr.getAddr(),sizeof(sockaddr)) < 0)
//     {
//         perror("bind error");
//         close(fd_);
//         exit(-1);
//     }
// }
void TcpServer::listen(int nn)
{
        if(::listen(TcpServer::serversock_,nn)!= 0)
    {
        perror("listen error");
        close(serversock_);
        exit(-1);
    }
}
void TcpServer::accept(InetAddress& clientaddr)
{
    sockaddr_in    peeraddr;
    socklen_t peerlen =sizeof(peeraddr);
    int clientfd = ::accept(serversock_, (sockaddr*)&peeraddr, &peerlen);
    clientaddr.setaddr(peeraddr);
    clientsock_=clientfd;
}

bool TcpServer::send(const std::string &buffer)
{
    /*        if( (iret=send(clientsock->getFd(),buffer,strlen(buffer),0)) <=0 )
        {
            perror("send error");

        }
        std::cout <<"发送："<<buffer<<std::endl ;*/
    if (TcpServer::clientsock_==-1)
        return false;
    if (::send(TcpServer::clientsock_, buffer.data(), buffer.size(),0)<=0 )
    {
        return false;
    }
    return true;

}
bool TcpServer::send(const std::string &buffer, int fd)
{
    /*        if( (iret=send(clientsock->getFd(),buffer,strlen(buffer),0)) <=0 )
        {
            perror("send error");

        }
        std::cout <<"发送："<<buffer<<std::endl ;*/
    if (TcpServer::clientsock_==-1)
        return false;
    if (::send(fd, buffer.data(), buffer.size(),0)<=0 )
    {
        return false;
    }
    return true;

}
bool TcpServer::recv(std::string &buffer,const size_t maxlen,int fd)
{
    buffer.clear();
        // memset(buffer,0 ,sizeof(buffer));
        // if ( (iret=recv(clientsock->getFd(),buffer,sizeof(buffer),0)) <=0 )
        // {
        //     std::cout <<"iret="<<iret<<std::endl;

        // }
    buffer.resize(maxlen);
    int readn=::recv(fd,& buffer[0], buffer.size(),0);
    if (readn<=0)
    {   
        buffer.clear();
        return false;
    }    
    buffer.resize(readn);
    return true;
}
bool TcpServer::recv(std::string &buffer,const size_t maxlen)
{
    buffer.clear();
        // memset(buffer,0 ,sizeof(buffer));
        // if ( (iret=recv(clientsock->getFd(),buffer,sizeof(buffer),0)) <=0 )
        // {
        //     std::cout <<"iret="<<iret<<std::endl;

        // }
    buffer.resize(maxlen);
    int readn=::recv(TcpServer::clientsock_,& buffer[0], buffer.size(),0);
    if (readn<=0)
    {   
        buffer.clear();
        return false;
    }    
    buffer.resize(readn);
    return true;
}
void TcpServer::closeconn()
{
    if(TcpServer::clientsock_!=-1)
    {
        ::close(TcpServer::clientsock_);
    }
    if(TcpServer::serversock_!=-1)
    {
        ::close(TcpServer::serversock_);
    }
}
int TcpServer::getServSocket()
{
    return serversock_;
}
int TcpServer::getCliSocket()
{
    return clientsock_;
}