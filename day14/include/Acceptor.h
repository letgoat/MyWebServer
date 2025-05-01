//Acceptor主要是用于服务器接收连接，并在连接后做相应的处理
//这个类需要独属于自己的Channel，因此采用智能指针管理
//将Socket相应的操作也封装在Acceptor中，并且为了方便自定义ip和port端口，不直接讲ip和port绑死，而是通过传参的方式
#pragma once
#include "../include/common.h"

class EventLoop;
class Channel;

class Acceptor{
public:
    DISALLOW_COPY_AND_MOVE(Acceptor);
    Acceptor(EventLoop *loop, const char* ip, const int port);
    ~Acceptor();

    void set_new_connection_callback(std::function<void(int) const &callback);

    //创建socket
    void Create();

    //与IP地址绑定
    void Bind(const char* ip, const int port);

    //监听socket
    void Listen();

    //接收连接
    void AcceptConnection();
 
    void new_connection_callback(std::function<void(int)> const &callback);
    
private:
    EventLoop* loop_;
    int listenfd_;
    std::unique_ptr<Channel> channel_;
    std::function<void(int)> new_connection_callback_;
}