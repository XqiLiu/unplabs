cmake_minimum_required(VERSION 3.10)
project(lab2)


add_executable(client client.cpp InetAddress.cpp Socket.cpp TcpClient.cpp)
target_compile_options(client PRIVATE -fPIC)
add_executable(server server.cpp InetAddress.cpp Socket.cpp TcpServer.cpp)
target_compile_options(server PRIVATE -fPIC)
