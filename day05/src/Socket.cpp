#include "../include/Socket.h"
#include "../include/util.h"
#include "../include/InetAddress.h"
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

Socket::Socket():fd(-1){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errIf(fd == -1, "create socket failed");
}

Socket::Socket(int _fd):fd(_fd){
    errIf(fd == -1, "create socket failed");
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress* addr){
    errIf(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "bind failed"); //::bind表示使用的是全局命名空间的bind函数
}

void Socket::listen(){
    errIf(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

int Socket::accept(InetAddress* addr){
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errIf(clnt_sockfd == -1, "socket accept error");
    return clnt_sockfd;
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL,0) | O_NONBLOCK);
}

int Socket::getFd(){
    return fd;
}