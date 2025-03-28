#pragma once

#include <sys/socket.h>

class InetAddress{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, int port);
    ~InetAddress();
};