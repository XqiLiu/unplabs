#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<iostream>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include "InetAddress.h"
#include "Socket.h"
#include "TcpServer.h"
int main(int argc, char **argv)
{
    InetAddress servaddr(argv[1],atoi(argv[2]));
    TcpServer tcpserver(servaddr);
    InetAddress clientaddr; 

    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        return -1;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = tcpserver.getServSocket();
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, tcpserver.getServSocket(), &ev) == -1) {
        perror("epoll_ctl: tcpserver");
        return -1;
    }

    while (true) {
        struct epoll_event events[10];
        int nfds = epoll_wait(epollfd, events, 10, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            return -1;
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == tcpserver.getServSocket()) {
                tcpserver.accept(clientaddr);
                fcntl(tcpserver.getCliSocket(),fcntl(tcpserver.getCliSocket(),F_GETFL)|O_NONBLOCK);
                std::cout << "客户端"<<clientaddr.getIp()<<":"<<clientaddr.getPort()<<"已连接 " << std::endl;
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = tcpserver.getCliSocket();
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, tcpserver.getCliSocket(), &ev) == -1) {
                    perror("epoll_ctl: clientaddr");
                    return -1;
                }
            } else {
                std::string buffer;
                if(tcpserver.recv(buffer,1024,events[n].data.fd)==false)
                {   
                    close(events[n].data.fd); // Close the file descriptor
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
                    continue;
                }
                std::cout << "接收: "<<buffer <<std::endl;
                if(tcpserver.send(buffer,events[n].data.fd)<=0)
                {
                    close(events[n].data.fd); // Close the file descriptor
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL); // Remove it from the epoll set
                    continue;
                }
                std::cout <<"发送："<<buffer<<std::endl;
            }
        }
    }

    return 0;
}
