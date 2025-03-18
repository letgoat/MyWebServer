#pragma once

class InetAddress;

class Socket{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();
    
    void listen();
    void setnonblocking();
    void bind(InetAddress*);
    int accept(InetAddress*);
    int connect(InetAddress*);
    int getFd();
};