#pragma once 
#include <functional>

class Eventloop;
class Socket;
class Channel;
class InetAddress;
class Acceptor{
private:
    Eventloop *loop;
    Socket *sock;
    InetAddress *addr;
    Channel *acceptChannel;

public:
    Acceptor(Eventloop *_loop);
    ~Acceptor();
    void acceptConnection(); //用于处理新的连接请求，当有新的客户端尝试连接时，这个函数会被调用
    std::function<void(Socket*)> newConnectionCallback;//这是一个成员变量，存储了一个回调函数
    void setNewConnectionCallback(std::function<void(Socket*)>);//这是一个设置回调函数的方法
};