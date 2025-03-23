#include "Socket.h"
#include "util.h"
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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

Socket::bind(InetAddress* addr){
    errIf(bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "bind failed");
}