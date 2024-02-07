#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void test_echo_server(const std::string& server_ip, int server_port) {
    // 创建一个TCP/IP套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // 设置服务器地址结构体
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if(inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr)<=0) {
        std::cout << "Invalid address/ Address not supported \n";
        return;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cout << "Connection Failed \n";
        return;
    }

    // 发送数据
    std::string message = "This is the test message. This will be echoed back!";
    send(sock, message.c_str(), message.size(), 0);
    std::cout << "Message sent\n";

    // 接收回应
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    std::cout << "Received: " << buffer << "\n";

    // 关闭套接字
    close(sock);
}

int main() {
    // 测试echo服务
    test_echo_server("172.17.150.27", 5055);
    return 0;
}
