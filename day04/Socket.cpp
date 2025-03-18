#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

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

void Socket::bind(InetAddress& addr){
    errIf(bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind error");
}

void Socket::listen(){
    errIf(listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking(){
    errIf(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == -1, "setnonblocking error");
}

int accept(InetAddress& addr){
    int clnt_sockfd = accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errIf(clnt_sockfd == -1, "accept error");
    return clnt_sockfd;
}

int connect(InetAddress& addr){
    errIf(connect(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "connect error");
}
int Socket::getFd(){
    return fd;
}