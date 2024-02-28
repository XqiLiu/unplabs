#include "InetAddress.h"

InetAddress::InetAddress()
{

}

InetAddress::InetAddress(const std::string &ip , uint16_t port)
{
        //指定协议族
    addr_.sin_family = AF_INET;
    //第一个参数为ip地址，需要把ip地址转换为sin_addr类型
    // inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    //第二个参数为端口号
    addr_.sin_port = htons(port);

}
// InetAddress::InetAddress(const sockaddr_in addr):addr_(addr)
// {

// }
InetAddress::~InetAddress()
{

}
const char *InetAddress::getIp()const
{
    return inet_ntoa(addr_.sin_addr);
}
uint16_t InetAddress::getPort()const
{
    return ntohs(addr_.sin_port);
}
const sockaddr* InetAddress::getAddr()const
{
    return (sockaddr*) &addr_;
}
void InetAddress::setaddr(sockaddr_in clientaddr)
{
    addr_=clientaddr;
}