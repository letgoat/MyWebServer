#pragma once

#include <arpa/inet.h>

class InetAddress{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char*, uint16_t);
    ~InetAddress();
    void setInetAddr(struct sockaddr_in, socklen_t);
    sockaddr_in getAddr();
    socklen_t getAddr_len();
};

