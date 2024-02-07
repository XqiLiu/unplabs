#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include "InetAddress.h"
#include "Socket.h"
#define SERV_PORT 1234
#define MAXLINE 128
int main(int argc, char **argv)
{
    
    int    connfd = 0; //已连接描述符
    // socklen_t clilen;
    char recvMsg[MAXLINE] = {0};
    //服务器和客户端socket信息

    char ip[MAXLINE] = {0};
   
    //第一步，创建socket
    // int    listenfd = 0;//监听描述符
    // listenfd = socket(AF_INET,SOCK_STREAM,0);
    // if (listenfd <0 )
    // {
    //     perror("socket error");
    //     return -1;
    // }
    Socket listensock(createListenFd());
    
    //第二步，服务端用于通信的IP和端口绑定在socket上
    InetAddress servaddr(argv[1],atoi(argv[2]));
    // struct sockaddr_in servaddr;
    // bzero(&servaddr, sizeof(servaddr));
    // servaddr.sin_family = AF_INET;
    // servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    // servaddr.sin_port = htons(atoi(argv[2]));
    //如果输入ip和端口，使用输入的ip和端口

    // if(3 == argc)
    // {   //printf("argv[1] is %s",argv[1]);
    //     inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    //     servaddr.sin_port = htons(atoi(argv[2]));
    // }
    // else
    // {   
    //     //使用默认的ip和port
    //     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //     servaddr.sin_port        = htons(SERV_PORT);
    // }

    // listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // if(-1 == listenfd)
    // {
    //     perror("socket error");
    //     return -1;
    // }
    //绑定指定ip和端口
    // if(0 != bind(listenfd,  (struct sockaddr *) &servaddr, sizeof(servaddr)))
    // {
    //     perror("bind error");
    //     return -1;
    // }
    // if(bind(listenfd.getFd(),servaddr.getAddr(),sizeof(sockaddr)) < 0)
    // {
    //     perror("bind error");
    //     close(listenfd.getFd());
    //     return -1;
    // }
    listensock.bind(servaddr);
    printf("start server at %s:%d\n",servaddr.getIp(),servaddr.getPort());
    //listen(listenfd, 4);
    //socket设置为listen
    // if(listen(listenfd.getFd(),5)!= 0)
    // {
    //     perror("listen error");
    //     close(listenfd.getFd());
    //     return -1;
    // }
    listensock.listen(1024);
    //处理来自客户端的连接
    // sockaddr_in    peeraddr;
    // socklen_t peerlen =sizeof(peeraddr);
    // int clientfd = accept(listenfd.getFd(), (sockaddr*)&peeraddr, &peerlen);
    InetAddress clientaddr;
    Socket *clientsock=new Socket(listensock.accept(clientaddr));
    if (clientsock->getFd()==-1)
    {
        perror("accept error");
        close(listensock.getFd());
        return -1;
    }
    std::cout << "客户端"<<clientaddr.getIp()<<":"<<clientaddr.getPort()<<"已连接 " << std::endl;

    char buffer[1024];
    while(true)
    {
        int iret;
        memset(buffer,0 ,sizeof(buffer));
        if ( (iret=recv(clientsock->getFd(),buffer,sizeof(buffer),0)) <=0 )
        {
            std::cout <<"iret="<<iret<<std::endl;

        }
        std::cout << "接收: "<<buffer ;

        if( (iret=send(clientsock->getFd(),buffer,strlen(buffer),0)) <=0 )
        {
            perror("send error");
            break;
        }
        std::cout <<"发送："<<buffer ;
    }
    delete(clientsock);
    close(listensock.getFd());
    close(clientsock->getFd());
    // clilen = sizeof(cliaddr);
    // connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    // if(-1 == connfd)
    // {
    //     perror("accept failed");
    //     return -1;
    // }

    // printf("connect from %s %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,ip,MAXLINE),ntohs(cliaddr.sin_port));

    // //读取客户端发送的消息
    // if(0 != read(connfd, recvMsg, MAXLINE))
    // {
    //     printf("recv msg:%s\n",recvMsg);
    // }

    // //将读取内容原封不动地发送回去
    // write(connfd, recvMsg, MAXLINE);
    // close(connfd);
    // close(listenfd);
    return 0;
}