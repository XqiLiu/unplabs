#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <queue>
#include <pthread.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include "InetAddress.h"
#include "Socket.h"
#include "TcpServer.h"
//#include "Handler.h"
#include "ThreadPool.h"  

#define MAX_THREADS 10
// void handleClient(TcpServer& tcpserver, int fd) {
//     std::string buffer;
//     if(tcpserver.recv(buffer,1024,fd)==false)
//     {   
//         close(fd); 
//         return;
//     }
//     std::cout << "接收: "<<buffer <<std::endl;

//     if(tcpserver.send(buffer,fd)<=0)
//     {
//         close(fd); 
//         return;
//     }
//     std::cout <<"发送："<<buffer<<std::endl;
// }
void handleClient(TcpServer& tcpserver, int fd) {
    std::string buffer;
    if(tcpserver.recv(buffer,1024,fd)==false)
    {   
        close(fd); 
        return;
    }
    std::cout << "接收: "<<buffer <<std::endl;

    // 加密密钥和初始化向量
    unsigned char key[AES_BLOCK_SIZE]; // AES_BLOCK_SIZE = 16
    unsigned char iv[AES_BLOCK_SIZE];

    // 生成一个随机密钥和初始化向量
    RAND_bytes(key, AES_BLOCK_SIZE);
    RAND_bytes(iv, AES_BLOCK_SIZE);

    // 创建并初始化加密上下文
    AES_KEY encryptKey;
    AES_set_encrypt_key(key, 128, &encryptKey);

    // 加密缓冲区
    unsigned char encryptedBuffer[1024];

    // 使用AES_128_CBC加密算法对buffer进行加密
    AES_cbc_encrypt((unsigned char*)buffer.c_str(), encryptedBuffer, buffer.size(), &encryptKey, iv, AES_ENCRYPT);

    // 将加密后的数据转换为std::string类型
    std::string encryptedString(reinterpret_cast<char*>(encryptedBuffer), buffer.size());

    // 将加密后的数据发送回客户端
    if(tcpserver.send(encryptedString, fd)<=0)
    {
        close(fd); 
        return;
    }
    std::cout <<"发送："<<encryptedString<<std::endl;
}

int main(int argc, char **argv)
{
    InetAddress servaddr(argv[1],atoi(argv[2]));
    TcpServer tcpserver(servaddr);
    InetAddress clientaddr; 

    ThreadPool pool(MAX_THREADS,tcpserver);  // 创建一个有MAX_THREADS个线程的线程池
    // pool.setCallBack(& { handleClient(tcpserver, fd); });
    pool.setCallBack([&tcpserver] (TcpServer& tcp, int fd) { handleClient(tcp, fd); });




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
                //pool.setCallBack([&, fd = events[n].data.fd] { handleClient(tcpserver, fd); });
                pool.enqueue(events[n].data.fd);
            }
        }
    }

    return 0;
}
