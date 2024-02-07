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
#include "TcpServer.h"
int main(int argc, char **argv)
{
    InetAddress servaddr(argv[1],atoi(argv[2]));
    // std::cout<<argv[1]<<" "<<argv[2]<<std::endl;
    TcpServer tcpserver(servaddr);
    InetAddress clientaddr; 
    
    tcpserver.accept(clientaddr);
    std::cout << "客户端"<<clientaddr.getIp()<<":"<<clientaddr.getPort()<<"已连接 " << std::endl;
    std::string buffer;
    if(tcpserver.recv(buffer,1024)==false)
    {   
        perror("send error");
        return -1;
    }
    std::cout << "接收: "<<buffer <<std::endl;
    if(tcpserver.send(buffer)<=0)
    {
        perror("recv error");
        return -1;
    }
    std::cout <<"发送："<<buffer<<std::endl;
    return 0;
}