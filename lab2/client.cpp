#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<iostream>
#include "Socket.h"
#include "TcpClient.h"


#define MAXLINE 1024
int main(int argc, char **argv)
{

    if (argc != 3)//检查参数
    {
        std::cout <<"usage: ./client 172.17.150.27 5055 "<<std::endl;
        return -1;
    }
    InetAddress servaddr(argv[1],atoi(argv[2]));
    TcpClient client(servaddr);
    client.Tcpconnect(servaddr);
    std::string buffer;
    std::cout<<"请输入：";
    std::cin>>buffer;
    if (client.send(buffer)==false)
    {
        perror("send error");
        return -1;
    }
    std::cout <<"发送：" <<buffer<<std::endl ;
    if(client.recv(buffer,1024)<=0)
    {
        perror("recv error");
        return -1;
    }
    std::cout << "收到消息:" << buffer <<std::endl;
    
   return 0;
}