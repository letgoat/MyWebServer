#pragma once

class InetAddress;
class Socket{
private:
    int fd; 
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*); //将fd和InetAddress绑定
    void listen(); //监听fd
    void setnonblocking(); //设置为非阻塞

    int accept(InetAddress*); //接收连接，并且返回新的fd
    void connect(InetAddress*);
    int getFd();
};