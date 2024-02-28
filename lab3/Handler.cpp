#include "Handler.h"

void Handler::handle(TcpServer& tcpserver, int fd)
{
std::string buffer;
if(tcpserver.recv(buffer,1024,fd)==false)
{
    close(fd);
    return;
}
std::cout<< "接收：" <<buffer <<std::endl;
callback_(tcpserver, fd);
}
void Handler::setcallback(const std::function<void(TcpServer, int)>& callback)
{callback_=callback;
}