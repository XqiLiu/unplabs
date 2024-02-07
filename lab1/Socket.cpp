#include "Socket.h"
/*class Socket{
private:
    const int fd_;
public:
    Socket(int fd);
    ~Socket();
 
    int getFd() const;
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setTcpNoDelay(bool on);
    void setKeepAlive(bool on);

    void bind(const InetAddress& servaddr);
    void listen(int nn=128);
    void accept(InetAddress& clientaddr);
};
*/
int createListenFd()
{   int listenfd = 0;//监听描述符
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    int opt=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if (listenfd <0 )
    {
        perror("socket error");
        // printf("%s:%s:%d listen socket create error:%d\n",
        // __FILE__,__FUNCTION__,__LINE__,errno);
        exit(-1);
    }
    return listenfd;
}
int createClientFd()
{   int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("%s:%s:%d client socket create error:%d\n", __FILE__,__FUNCTION__,__LINE__,errno);
        exit(-1);
    }
    // printf("sockfd is %d\n",sockfd);
}
Socket::Socket()
{
}

Socket::~Socket()
{
    // ::close(fd_);
}

// int Socket::getFd() const
// {
//     return fd_;
// }

// void Socket::bind(const InetAddress& servaddr)
// {
//     if(::bind(fd_,servaddr.getAddr(),sizeof(sockaddr)) < 0)
//     {
//         perror("bind error");
//         close(fd_);
//         exit(-1);
//     }
// }

// void Socket::listen(int nn)
// {
    //     if(::listen(fd_,nn)!= 0)
    // {
    //     perror("listen error");
    //     close(fd_);
    //     exit(-1);
    // }
// }
// int Socket::accept(InetAddress& clientaddr)
// {
    // sockaddr_in    peeraddr;
    // socklen_t peerlen =sizeof(peeraddr);
    // int clientfd = ::accept(fd_, (sockaddr*)&peeraddr, &peerlen);
    // clientaddr.setaddr(peeraddr);
    // return clientfd;
// }

// void Socket::connect(const InetAddress& servaddr)
// {
//     if (::connect(fd_,servaddr.getAddr(),sizeof(servaddr)) != 0)
//     {
//         perror("connect error");
//         close(fd_);
//         exit(-1);
//     };
// }