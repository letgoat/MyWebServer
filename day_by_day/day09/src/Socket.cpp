#include "../include/Socket.h"
#include "../include/InetAddress.h"
#include "../include/util.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

Socket::Socket():fd(-1){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "create socket failed");
}

Socket::Socket(int _fd):fd(_fd){
    errif(fd == -1, "create socket failed");
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress* _addr){
    
}

void Socket::listen(){
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress* _addr){
    
}

void Socket::connect(InetAddress* _addr){
    struct sockaddr_in addr = _addr->getAddr();
}

int Socket::getFd(){
    return fd;
}